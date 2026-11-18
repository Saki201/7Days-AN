package org.hebdomad.android;
import android.os.Bundle;
import android.view.ViewGroup;
import android.media.AudioManager;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.SeekBar;
import org.libsdl.app.SDLActivity;

public final class GameActivity extends SDLActivity {
    private TouchControls controls;
    private boolean editingBack;
    static native void touchKey(int scan,boolean down);
    static native void clearTouch();
    static native void setFilter(int mode);
    static native void setRenderScale(int scale);
    static native void setAspect(int mode);
    static native String renderInfo();
    protected String[] getLibraries(){return new String[]{"SDL3","main"};}
    protected String[] getArguments(){return new String[]{getFilesDir().getAbsolutePath()};}
    protected void onCreate(Bundle state){
        super.onCreate(state);
        setVolumeControlStream(AudioManager.STREAM_MUSIC);
        setFilter(getPreferences(0).getInt("filter",0));
        setRenderScale(getPreferences(0).getInt("renderScale",1));
        setAspect(getPreferences(0).getInt("aspect",0));
        if(mLayout!=null){controls=new TouchControls(this);mLayout.addView(controls,new ViewGroup.LayoutParams(-1,-1));}
    }
    void showDisplaySettings(){
        if(controls!=null)controls.releaseAll();
        new AlertDialog.Builder(this).setTitle("游戏设置").setItems(new String[]{"内部渲染分辨率","画面滤镜","画面比例","虚拟按键调节"},new DialogInterface.OnClickListener(){
            public void onClick(DialogInterface dialog,int which){if(which==0)showResolution();else if(which==1)showFilters();else if(which==2)showAspect();else showTouchSettings();}
        }).setNegativeButton("关闭",null).show();
    }
    private void showTouchSettings(){
        LinearLayout box=new LinearLayout(this);box.setOrientation(LinearLayout.VERTICAL);int pad=(int)(24*getResources().getDisplayMetrics().density);box.setPadding(pad,0,pad,0);
        final TextView alphaText=new TextView(this);alphaText.setText("不透明度："+(int)(controls.controlOpacity()*100)+"%");box.addView(alphaText);
        SeekBar alpha=new SeekBar(this);alpha.setMax(90);alpha.setProgress((int)(controls.controlOpacity()*100)-10);box.addView(alpha);
        alpha.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener(){public void onProgressChanged(SeekBar b,int v,boolean user){if(user){controls.setControlOpacity((v+10)/100f);alphaText.setText("不透明度："+(v+10)+"%");}}public void onStartTrackingTouch(SeekBar b){}public void onStopTrackingTouch(SeekBar b){}});
        final TextView sizeText=new TextView(this);sizeText.setText("大小："+(int)(controls.controlSize()*100)+"%");box.addView(sizeText);
        SeekBar size=new SeekBar(this);size.setMax(110);size.setProgress((int)(controls.controlSize()*100)-50);box.addView(size);
        size.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener(){public void onProgressChanged(SeekBar b,int v,boolean user){if(user){controls.setControlSize((v+50)/100f);sizeText.setText("大小："+(v+50)+"%");}}public void onStartTrackingTouch(SeekBar b){}public void onStopTrackingTouch(SeekBar b){}});
        new AlertDialog.Builder(this).setTitle("虚拟按键调节").setView(box).setPositiveButton("拖动位置",new DialogInterface.OnClickListener(){public void onClick(DialogInterface d,int w){controls.editPositions();}}).setNeutralButton("恢复默认",new DialogInterface.OnClickListener(){public void onClick(DialogInterface d,int w){controls.resetLayout();}}).setNegativeButton("完成",null).show();
    }
    private void showResolution(){
        new AlertDialog.Builder(this).setTitle("内部渲染："+renderInfo()).setSingleChoiceItems(new String[]{"1× 屏幕原生分辨率","2× 超采样（立即生效）"},getPreferences(0).getInt("renderScale",1)-1,new DialogInterface.OnClickListener(){
            public void onClick(DialogInterface dialog,int which){getPreferences(0).edit().putInt("renderScale",which+1).apply();setRenderScale(which+1);dialog.dismiss();}
        }).setNegativeButton("关闭",null).show();
    }
    private void showAspect(){
        new AlertDialog.Builder(this).setTitle("画面比例").setSingleChoiceItems(new String[]{"自动适配横屏","16:9","4:3（游戏原始视野）","5:3"},getPreferences(0).getInt("aspect",0),new DialogInterface.OnClickListener(){
            public void onClick(DialogInterface dialog,int which){getPreferences(0).edit().putInt("aspect",which).apply();setAspect(which);dialog.dismiss();}
        }).setNegativeButton("关闭",null).show();
    }
    private void showFilters(){
        new AlertDialog.Builder(this).setTitle("画面滤镜").setSingleChoiceItems(new String[]{"原始画面（最清晰）","xBR 边缘修整","HQ2x 边缘修整","Dot 点阵","扫描线"},getPreferences(0).getInt("filter",0),new DialogInterface.OnClickListener(){
            public void onClick(DialogInterface dialog,int which){setFilter(which);getPreferences(0).edit().putInt("filter",which).apply();dialog.dismiss();}
        }).setNegativeButton("关闭",null).show();
    }
    protected void onPause(){if(controls!=null)controls.releaseAll();super.onPause();}
    public boolean dispatchKeyEvent(android.view.KeyEvent event){
        if(event.getKeyCode()==android.view.KeyEvent.KEYCODE_BACK){
            if(event.getAction()==android.view.KeyEvent.ACTION_DOWN&&controls!=null&&controls.finishEditing())editingBack=true;
            if(editingBack){if(event.getAction()==android.view.KeyEvent.ACTION_UP)editingBack=false;return true;}
        }
        return super.dispatchKeyEvent(event);
    }
}
