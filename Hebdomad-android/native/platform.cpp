#include "port.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <vector>

SDL_Window *port_window;
static SDL_GLContext port_context;
static void *pulse_handle=reinterpret_cast<void*>(0x5a170001);

struct PulseSpec {int format;uint32_t rate;uint8_t channels;};
struct PulseStream {SDL_AudioStream *stream;int rate,channels;size_t bytes;bool logged;};
static void *pulse_new(const char*,const char*,int,const char*,const char*,const PulseSpec *spec,const void*,const void*,int *error) {
    port_log("Audio requested format=%d rate=%u channels=%u",spec->format,spec->rate,spec->channels);
    if(spec->format!=3 || !spec->channels || !spec->rate){if(error)*error=1;return nullptr;}
    SDL_AudioSpec target={SDL_AUDIO_S16LE,spec->channels,static_cast<int>(spec->rate)};
    SDL_AudioStream *s=SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,&target,nullptr,nullptr);
    if(!s){port_log("SDL audio open failed: %s",SDL_GetError());if(error)*error=1;return nullptr;}
    SDL_ResumeAudioStreamDevice(s);
    return new PulseStream{s,static_cast<int>(spec->rate),spec->channels,0,false};
}
static int pulse_write(PulseStream *s,const void *data,size_t length,int *error) {
    if(!SDL_PutAudioStreamData(s->stream,data,static_cast<int>(length))){if(error)*error=1;return -1;}
    // pa_simple_write is blocking. Bound latency and prevent the original mixer thread from spinning.
    int threshold=s->rate*s->channels*2/8;
    while(SDL_GetAudioStreamQueued(s->stream)>threshold)SDL_Delay(3);
    s->bytes+=length;
    if(!s->logged && s->bytes>=static_cast<size_t>(s->rate*s->channels*4)){
        port_log("Audio stream consumed at least two seconds of PCM");s->logged=true;
    }
    return 0;
}
static void pulse_free(PulseStream *s){if(s){SDL_DestroyAudioStream(s->stream);delete s;}}
void *port_dlopen(const char *name,int flags) {
    if(name && !strcmp(name,"libpulse-simple.so.0"))return pulse_handle;
    // The Android renderer replaces X11 and EGL setup. Other runtime libraries use Android's loader.
    return dlopen(name,flags);
}
void *port_dlsym(void *handle,const char *name) {
    if(handle==pulse_handle){
        if(!strcmp(name,"pa_simple_new"))return reinterpret_cast<void*>(pulse_new);
        if(!strcmp(name,"pa_simple_write"))return reinterpret_cast<void*>(pulse_write);
        if(!strcmp(name,"pa_simple_free"))return reinterpret_cast<void*>(pulse_free);
        return nullptr;
    }
    return dlsym(handle,name);
}
extern "C" int port_dlclose(void *handle){return handle==pulse_handle?0:dlclose(handle);}

static SDL_Window *create_window(const char *title,int width,int height,SDL_WindowFlags flags) {
    port_log("Create Android window: %s, %dx%d, flags=%llx",title,width,height,static_cast<unsigned long long>(flags));
    flags|=SDL_WINDOW_OPENGL|SDL_WINDOW_FULLSCREEN;
    port_window=SDL_CreateWindow(title,width,height,flags);
    if(!port_window)port_log("Window failed: %s",SDL_GetError());
    return port_window;
}
static int create_context() {
    port_context=SDL_GL_CreateContext(port_window);
    bool ok=port_context && SDL_GL_MakeCurrent(port_window,port_context);
    port_log("Android GLES context: %s",ok?"created":SDL_GetError());
    return ok?1:0;
}
static void destroy_context(){if(port_context){SDL_GL_DestroyContext(port_context);port_context=nullptr;}}
static void refresh_context(){if(port_context)SDL_GL_MakeCurrent(port_window,port_context);}
static void *gl_proc(const char *name){return reinterpret_cast<void*>(SDL_GL_GetProcAddress(name));}
static void swap(){SDL_GL_SwapWindow(port_window);}
static void swap_interval(int interval){SDL_GL_SetSwapInterval(interval);}
static void no_dynapi_init(){}

// SDL guarantees this append-only ABI order. The original 3.4.18 executable has
// exactly 1,270 entries, matching the 3.4.20 header included with this project.
static const char *api_names[]={
#define SDL_DYNAPI_PROC(ret,name,params,args,result) #name,
#include "../third_party/SDL_dynapi_procs.h"
#undef SDL_DYNAPI_PROC
};
static void unsupported_sdl(){port_log("Unavailable SDL API invoked from %p",__builtin_return_address(0));abort();}
bool install_platform_hooks() {
    static_assert(sizeof(api_names)/sizeof(*api_names)==1270,"SDL dispatch layout changed");
    auto **table=reinterpret_cast<void**>(0x3d6b50);
    for(size_t i=0;i<1270;++i) {
        if(reinterpret_cast<uintptr_t>(table[i])<0x120000 || reinterpret_cast<uintptr_t>(table[i])>=0x135000){port_log("Unexpected SDL dispatch entry %u",static_cast<unsigned>(i));return false;}
    }
    void *sdl=dlopen("libSDL3.so",RTLD_NOW|RTLD_GLOBAL);
    if(!sdl){port_log("Could not open native SDL: %s",dlerror());return false;}
    for(size_t i=0;i<1270;++i){
        void *fn=dlsym(sdl,api_names[i]);
        table[i]=!strcmp(api_names[i],"SDL_CreateWindow")?reinterpret_cast<void*>(create_window):
            (fn?fn:reinterpret_cast<void*>(unsupported_sdl));
    }
    port_patch(0x11fff0,reinterpret_cast<void*>(no_dynapi_init));
    port_patch(0xe3cf0,reinterpret_cast<void*>(create_context));
    port_patch(0xe46c8,reinterpret_cast<void*>(destroy_context));
    port_patch(0xe4760,reinterpret_cast<void*>(refresh_context));
    port_patch(0xe47bc,reinterpret_cast<void*>(gl_proc));
    port_patch(0xe4828,reinterpret_cast<void*>(swap));
    port_patch(0xe484c,reinterpret_cast<void*>(swap_interval));
    port_log("SDL dispatch and Android graphics hooks installed");
    return true;
}

extern "C" __attribute__((visibility("default"))) int SDL_main(int argc,char **argv) {
    const char *directory=argc>1?argv[1]:getenv("HEBDOMAD_DATA");
    if(!directory || chdir(directory)!=0){SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Hebdomad","Cannot open private game directory.",nullptr);return 1;}
    freopen("engine-output.txt","w",stdout);freopen("engine-errors.txt","w",stderr);
    setvbuf(stdout,nullptr,_IOLBF,0);setvbuf(stderr,nullptr,_IONBF,0);
    if(!load_engine("engine.arm")){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Hebdomad","The ARM engine could not initialize. See port-log.txt.",nullptr);return 1;
    }
    initialize_engine();
    char arg0[]="hebdomad",arg1[]="--data",arg2[]=".",arg3[]="--spk",arg4[]="res.spk";
    char *args[]={arg0,arg1,arg2,arg3,arg4,nullptr};
    return run_engine(5,args);
}
