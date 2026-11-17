package org.hebdomad.android;
import android.content.Context;
import android.graphics.*;
import android.view.*;
import java.util.*;
import org.libsdl.app.SDLActivity;

/** Multi-touch keyboard overlay. Releases every held key on pause/cancellation. */
final class TouchControls extends View {
    private static final class Button {
        final RectF area;final int key;final String label;
        Button(float l,float t,float r,float b,int k,String s){area=new RectF(l,t,r,b);key=k;label=s;}
    }
    private final ArrayList<Button> buttons=new ArrayList<>();
    private final Set<Integer> held=new HashSet<>();
    private final Paint paint=new Paint(3);
    private float scale=1,offset,bottom;
    TouchControls(Context c){super(c);setFocusable(false);setContentDescription("游戏触屏按键");}
    protected void onSizeChanged(int w,int h,int oldw,int oldh){
        scale=Math.min(w/960f,h/540f);offset=w/scale-960;bottom=h/scale-540;buttons.clear();
        add(70,296,150,376,KeyEvent.KEYCODE_W,"↑");
        add(0,376,80,456,KeyEvent.KEYCODE_A,"←");
        add(140,376,220,456,KeyEvent.KEYCODE_D,"→");
        add(70,456,150,536,KeyEvent.KEYCODE_S,"↓");
        add(838+offset,414,940+offset,514,KeyEvent.KEYCODE_J,"确认/动作");
        add(718+offset,414,820+offset,514,KeyEvent.KEYCODE_K,"物品/返回");
        add(838+offset,20,940+offset,80,KeyEvent.KEYCODE_M,"地图");
        add(20,20,122,80,KeyEvent.KEYCODE_ESCAPE,"返回");
    }
    private void add(float l,float t,float r,float b,int key,String label){
        float dy=t>100?bottom:0;
        buttons.add(new Button(l,t+dy,r,b+dy,key,label));
    }
    protected void onDraw(Canvas c){
        c.save();c.scale(scale,scale);paint.setTextAlign(Paint.Align.CENTER);paint.setTextSize(23);
        for(Button b:buttons){paint.setColor(held.contains(b.key)?0xc0b4a08c:0x68504b45);c.drawRoundRect(b.area,12,12,paint);
            paint.setColor(0xc0ffffff);c.drawText(b.label,b.area.centerX(),b.area.centerY()+8,paint);}
        c.restore();
    }
    public boolean onTouchEvent(MotionEvent e){
        Set<Integer> next=new HashSet<>();int action=e.getActionMasked();
        if(action!=MotionEvent.ACTION_CANCEL && action!=MotionEvent.ACTION_UP){
            for(int p=0;p<e.getPointerCount();++p){if(action==MotionEvent.ACTION_POINTER_UP&&p==e.getActionIndex())continue;
                float x=e.getX(p)/scale,y=e.getY(p)/scale;
                for(Button b:buttons)if(b.area.contains(x,y))next.add(b.key);
            }
        }
        if(action==MotionEvent.ACTION_DOWN && next.isEmpty())return false;
        for(int key:new HashSet<>(held))if(!next.contains(key))SDLActivity.onNativeKeyUp(key);
        for(int key:next)if(!held.contains(key))SDLActivity.onNativeKeyDown(key);
        held.clear();held.addAll(next);invalidate();return true;
    }
    void releaseAll(){for(int key:held)SDLActivity.onNativeKeyUp(key);held.clear();invalidate();}
}
