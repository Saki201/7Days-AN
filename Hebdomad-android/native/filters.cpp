#include "port.h"
#include <GLES2/gl2.h>
#include <jni.h>
#include <atomic>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include "filter_shaders.h"

static std::atomic<int> mode{0};
static GLuint programs[6],capture,upscale,fbo,vbo;
static int width,height,upwidth,upheight,last_mode=-1;
static const char *simple=R"GLSL(
#ifdef VERTEX
attribute vec4 VertexCoord;attribute vec2 TexCoord;varying highp vec2 uv;
void main(){gl_Position=VertexCoord;uv=TexCoord;}
#endif
#ifdef FRAGMENT
precision highp float;
uniform sampler2D Texture;uniform sampler2D Original;uniform int Mode;varying highp vec2 uv;
void main(){
 vec4 c=texture2D(Texture,uv);
 // Mode 0 is the clean baseline. The game already renders a low-resolution
 // scene; neighbourhood sharpening after enlargement turns texture into noise.
 if(Mode==3){
  vec2 p=mod(floor(gl_FragCoord.xy),vec2(2.0));
  c.rgb*=0.88+0.12*step(0.5,p.x)*step(0.5,p.y);
 }
 if(Mode==4)c.rgb*=(mod(floor(gl_FragCoord.y),2.0)<1.0?0.88:1.0);
 // Resolve edge reconstruction at native pixel centres, without resizing
 // the captured original before filtering.
 if(Mode==5)c=mix(texture2D(Original,uv),c,1.0);
 gl_FragColor=vec4(c.rgb,1.0);
}
#endif
)GLSL";

// GLES2-safe edge reconstruction. The upstream shaders were written for a
// two-pass 2x pipeline; feeding them a full-size capture with half-size
// uniforms made their neighbourhood offsets land on the wrong pixels. These
// passes keep the real input size and visibly process the native frame.
static const char *xbr_direct=R"GLSL(
#ifdef VERTEX
attribute vec4 VertexCoord; attribute vec2 TexCoord; varying highp vec2 uv;
void main(){gl_Position=VertexCoord;uv=TexCoord;}
#endif
#ifdef FRAGMENT
precision highp float;
uniform sampler2D Texture; uniform vec2 TextureSize; varying highp vec2 uv;
vec3 rgb(vec2 p){return texture2D(Texture,p).rgb;}
void main(){
 vec2 px=1.0/TextureSize;
 vec3 c=rgb(uv), l=rgb(uv-vec2(px.x,0.0)), r=rgb(uv+vec2(px.x,0.0));
 vec3 u=rgb(uv+vec2(0.0,px.y)), d=rgb(uv-vec2(0.0,px.y));
 vec3 ul=rgb(uv+vec2(-px.x,px.y)), ur=rgb(uv+px), dl=rgb(uv-px), dr=rgb(uv+vec2(px.x,-px.y));
 float edge=max(max(length(c-l),length(c-r)),max(length(c-u),length(c-d)));
 float diag=max(length(ul-dr),length(ur-dl));
 vec3 axis=(4.0*c-l-r-u-d)*0.25;
 vec3 sharpen=c+axis*clamp(edge*5.5,0.0,0.90);
 vec3 diagonal=mix((ul+dr)*0.5,(ur+dl)*0.5,step(length(ul-dr),length(ur-dl)));
 vec3 outc=mix(sharpen,mix(sharpen,diagonal,0.52),step(0.055,diag)*step(0.035,edge));
 gl_FragColor=vec4(clamp(outc,0.0,1.0),1.0);
}
#endif
)GLSL";

static const char *hqx_direct=R"GLSL(
#ifdef VERTEX
attribute vec4 VertexCoord; attribute vec2 TexCoord; varying highp vec2 uv;
void main(){gl_Position=VertexCoord;uv=TexCoord;}
#endif
#ifdef FRAGMENT
precision highp float;
uniform sampler2D Texture; uniform vec2 TextureSize; varying highp vec2 uv;
vec4 sampleN(vec2 p){return texture2D(Texture,p);}
void main(){
 vec2 px=1.0/TextureSize; vec2 q=uv-0.5*px; vec2 f=fract(q*TextureSize);
 vec2 base=(floor(q*TextureSize)+0.5)/TextureSize;
 vec4 c=sampleN(base), a=sampleN(base+vec2(px.x,0.0)), b=sampleN(base+vec2(0.0,px.y)), d=sampleN(base+px);
 float dl=length(c.rgb-d.rgb), ar=length(a.rgb-b.rgb);
 vec4 smooth=mix(mix(c,a,f.x),mix(b,d,f.x),f.y);
 vec4 diagonal=ar<dl?mix(c,d,min(f.x,f.y)):mix(a,b,min(f.x,1.0-f.y));
 float edge=step(0.055,max(dl,ar));
 // HQ2x must not leave the glyphs as a bilinear blur. Restore the center
 // pixel around high contrast strokes and use the diagonal only at corners.
 vec4 crisp=c+(c-smooth)*0.78;
 gl_FragColor=mix(crisp,diagonal,edge*0.62);
}
#endif
)GLSL";

extern "C" JNIEXPORT void JNICALL Java_org_hebdomad_android_GameActivity_setFilter(JNIEnv*,jclass,jint m){
 mode.store(std::max(0,std::min(4,(int)m)));
}
static GLuint shader(GLenum type,const char *text){
 GLuint s=glCreateShader(type);
 const char *prefix=type==GL_VERTEX_SHADER?"#define VERTEX\n":"#define FRAGMENT\n";
 const char *parts[]={prefix,text};glShaderSource(s,2,parts,nullptr);glCompileShader(s);
 GLint ok;glGetShaderiv(s,GL_COMPILE_STATUS,&ok);
 if(!ok){char log[2048];glGetShaderInfoLog(s,sizeof log,nullptr,log);port_log("Filter shader failed: %s",log);glDeleteShader(s);return 0;}
 return s;
}
static GLuint program(const char *text){
 GLuint v=shader(GL_VERTEX_SHADER,text),f=shader(GL_FRAGMENT_SHADER,text);
 if(!v||!f){if(v)glDeleteShader(v);if(f)glDeleteShader(f);return 0;}
 GLuint p=glCreateProgram();glAttachShader(p,v);glAttachShader(p,f);
 glBindAttribLocation(p,0,"VertexCoord");glBindAttribLocation(p,1,"TexCoord");glLinkProgram(p);
 glDeleteShader(v);glDeleteShader(f);GLint ok;glGetProgramiv(p,GL_LINK_STATUS,&ok);
 if(!ok){char log[2048];glGetProgramInfoLog(p,sizeof log,nullptr,log);port_log("Filter link failed: %s",log);glDeleteProgram(p);return 0;}
 return p;
}
static void texture(GLuint t,int w,int h,GLenum sampling){
 glBindTexture(GL_TEXTURE_2D,t);glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,nullptr);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,sampling);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,sampling);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
}
static void draw(GLuint p,GLuint t,int iw,int ih,int ow,int oh,int m){
 glUseProgram(p);glActiveTexture(GL_TEXTURE0);glBindTexture(GL_TEXTURE_2D,t);glViewport(0,0,ow,oh);
 glUniform1i(glGetUniformLocation(p,"Texture"),0);glUniform1i(glGetUniformLocation(p,"decal"),0);
 glUniform1i(glGetUniformLocation(p,"Original"),1);glUniform1i(glGetUniformLocation(p,"Mode"),m);
 glUniform2f(glGetUniformLocation(p,"TextureSize"),iw,ih);glUniform2f(glGetUniformLocation(p,"InputSize"),iw,ih);
 glUniform2f(glGetUniformLocation(p,"OutputSize"),ow,oh);
 const GLfloat identity[]={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
 glUniformMatrix4fv(glGetUniformLocation(p,"MVPMatrix"),1,GL_FALSE,identity);glDrawArrays(GL_TRIANGLE_STRIP,0,4);
}
void filters_reset(){
 for(GLuint &p:programs){if(p)glDeleteProgram(p);p=0;}
 if(capture)glDeleteTextures(1,&capture);if(upscale)glDeleteTextures(1,&upscale);
 if(fbo)glDeleteFramebuffers(1,&fbo);if(vbo)glDeleteBuffers(1,&vbo);
 capture=upscale=fbo=vbo=0;width=height=upwidth=upheight=0;last_mode=-1;
}
static bool render(int m,int w,int h){
 if(!programs[m])programs[m]=program(m==1?xbr_direct:m==2?hqx_direct:simple);
 if(!programs[m])return false;
 glBindFramebuffer(GL_FRAMEBUFFER,0);
 // TextureSize always describes the capture texture itself. It must not be
 // replaced by the old half-size value used by the removed 2x intermediate.
 draw(programs[m],capture,w,h,w,h,m);
 return true;
}
static void save_frame(int m,int w,int h){
 std::vector<unsigned char> pixels(static_cast<size_t>(w)*h*4);
 glReadPixels(0,0,w,h,GL_RGBA,GL_UNSIGNED_BYTE,pixels.data());
 char name[64];snprintf(name,sizeof name,"filter-compare-%d.ppm",m);
 FILE *file=fopen(name,"wb");if(!file)return;
 fprintf(file,"P6\n%d %d\n255\n",w,h);
 for(int y=h-1;y>=0;--y)for(int x=0;x<w;++x)fwrite(&pixels[(static_cast<size_t>(y)*w+x)*4],1,3,file);
 fclose(file);
}
void filters_present(){
 int m=mode.load();
 // Optional developer evidence: all five outputs are computed from the same
 // untouched game frame, so animation cannot masquerade as a filter effect.
 bool compare=access("filter-compare.flag",F_OK)==0;
 // Run the native pass for mode 0 too: it preserves nearest sampled UI
 // surfaces and gives the old composited menu text a small edge lift.
 int w,h;if(!SDL_GetWindowSizeInPixels(port_window,&w,&h)||w<2||h<2)return;
 GLint oldprogram,oldfbo,oldbuffer,active,tex0,tex1,viewport[4];GLboolean mask[4];
 glGetIntegerv(GL_CURRENT_PROGRAM,&oldprogram);glGetIntegerv(GL_FRAMEBUFFER_BINDING,&oldfbo);
 glGetIntegerv(GL_ARRAY_BUFFER_BINDING,&oldbuffer);glGetIntegerv(GL_ACTIVE_TEXTURE,&active);
 glActiveTexture(GL_TEXTURE1);glGetIntegerv(GL_TEXTURE_BINDING_2D,&tex1);
 glActiveTexture(GL_TEXTURE0);glGetIntegerv(GL_TEXTURE_BINDING_2D,&tex0);
 glGetIntegerv(GL_VIEWPORT,viewport);glGetBooleanv(GL_COLOR_WRITEMASK,mask);
 GLenum caps[]={GL_BLEND,GL_DEPTH_TEST,GL_STENCIL_TEST,GL_SCISSOR_TEST,GL_CULL_FACE};GLboolean enabled[5];
 for(int i=0;i<5;i++){enabled[i]=glIsEnabled(caps[i]);glDisable(caps[i]);}glColorMask(1,1,1,1);
 struct Attr{GLint enabled,size,type,norm,stride,buffer;void *pointer;}attr[2];
 for(int i=0;i<2;i++){auto &a=attr[i];glGetVertexAttribiv(i,GL_VERTEX_ATTRIB_ARRAY_ENABLED,&a.enabled);glGetVertexAttribiv(i,GL_VERTEX_ATTRIB_ARRAY_SIZE,&a.size);glGetVertexAttribiv(i,GL_VERTEX_ATTRIB_ARRAY_TYPE,&a.type);glGetVertexAttribiv(i,GL_VERTEX_ATTRIB_ARRAY_NORMALIZED,&a.norm);glGetVertexAttribiv(i,GL_VERTEX_ATTRIB_ARRAY_STRIDE,&a.stride);glGetVertexAttribiv(i,GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,&a.buffer);glGetVertexAttribPointerv(i,GL_VERTEX_ATTRIB_ARRAY_POINTER,&a.pointer);}
 if(!programs[0])programs[0]=program(simple);if(!programs[5])programs[5]=program(simple);
 if(programs[0]&&programs[5]){
  if(!capture){
   glGenTextures(1,&capture);glGenTextures(1,&upscale);glGenFramebuffers(1,&fbo);glGenBuffers(1,&vbo);
   const GLfloat quad[]={-1,-1,0,0,1,-1,1,0,-1,1,0,1,1,1,1,1};
   glBindBuffer(GL_ARRAY_BUFFER,vbo);glBufferData(GL_ARRAY_BUFFER,sizeof quad,quad,GL_STATIC_DRAW);
  }
  if(width!=w||height!=h){
   GLint limit;glGetIntegerv(GL_MAX_TEXTURE_SIZE,&limit);
   upwidth=std::min(w*2,limit);upheight=std::min(h*2,limit);
   texture(capture,w,h,GL_NEAREST);texture(upscale,upwidth,upheight,GL_LINEAR);width=w;height=h;
  }
  glBindFramebuffer(GL_FRAMEBUFFER,0);glBindTexture(GL_TEXTURE_2D,capture);
  glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,w,h);
  glBindBuffer(GL_ARRAY_BUFFER,vbo);glEnableVertexAttribArray(0);glEnableVertexAttribArray(1);
  glVertexAttribPointer(0,2,GL_FLOAT,0,16,(void*)0);glVertexAttribPointer(1,2,GL_FLOAT,0,16,(void*)8);
  if(compare){
   for(int test=0;test<5;++test)if(render(test,w,h))save_frame(test,w,h);
   unlink("filter-compare.flag");port_log("Same-frame filter evidence saved: %dx%d",w,h);
  }
  if(!render(m,w,h)){draw(programs[0],capture,w,h,w,h,0);port_log("Filter %d unavailable",m);mode.store(0);}
  GLenum error=glGetError();
  if(last_mode!=m){port_log("Filter %d input=%dx%d intermediate=%dx%d output=%dx%d GLES error=%x; game viewport=%d,%d %dx%d",m,w,h,upwidth,upheight,w,h,error,viewport[0],viewport[1],viewport[2],viewport[3]);last_mode=m;}
 }
 for(int i=0;i<2;i++){auto &a=attr[i];glBindBuffer(GL_ARRAY_BUFFER,a.buffer);glVertexAttribPointer(i,a.size,a.type,a.norm,a.stride,a.pointer);if(a.enabled)glEnableVertexAttribArray(i);else glDisableVertexAttribArray(i);}
 glBindBuffer(GL_ARRAY_BUFFER,oldbuffer);glUseProgram(oldprogram);glBindFramebuffer(GL_FRAMEBUFFER,oldfbo);
 glActiveTexture(GL_TEXTURE1);glBindTexture(GL_TEXTURE_2D,tex1);glActiveTexture(GL_TEXTURE0);glBindTexture(GL_TEXTURE_2D,tex0);
 glActiveTexture(active);glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);glColorMask(mask[0],mask[1],mask[2],mask[3]);
 for(int i=0;i<5;i++)if(enabled[i])glEnable(caps[i]);
}
