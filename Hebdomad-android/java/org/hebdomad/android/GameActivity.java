package org.hebdomad.android;
import android.os.Bundle;
import android.view.ViewGroup;
import org.libsdl.app.SDLActivity;

public final class GameActivity extends SDLActivity {
    private TouchControls controls;
    protected String[] getLibraries(){return new String[]{"SDL3","main"};}
    protected String[] getArguments(){return new String[]{getFilesDir().getAbsolutePath()};}
    protected void onCreate(Bundle state){
        super.onCreate(state);
        if(mLayout!=null){controls=new TouchControls(this);mLayout.addView(controls,new ViewGroup.LayoutParams(-1,-1));}
    }
    protected void onPause(){if(controls!=null)controls.releaseAll();super.onPause();}
}
