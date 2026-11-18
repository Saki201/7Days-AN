package org.hebdomad.android;
import android.graphics.*;
import android.view.*;
import java.util.*;
import org.libsdl.app.SDLActivity;
final class TouchControls extends View {
    private static final class Button {
        float x,y; final float r; final int scan,key,color; final String label;
        Button(float x,float y,float r,int scan,int key,String label,int color){this.x=x;this.y=y;this.r=r;this.scan=scan;this.key=key;this.label=label;this.color=color;}
        boolean hit(float px,float py){return Math.hypot(px-x,py-y)<r+12;}
    }
    private final ArrayList<Button> buttons=new ArrayList<>();
    private final Set<Integer> held=new HashSet<>();
    private final Map<Integer,Button> owners=new HashMap<>();
    private final Paint paint=new Paint(Paint.ANTI_ALIAS_FLAG);
    private float scale=1,width=960,height=540,jx=120,jy=420,dx,dy;
    private int joystick=-1;
    private float controlSize=1,opacity=.8f;
    private boolean editing,dragJoystick;
    private Button dragButton;
    private int dragPointer=-1;
    private float offsetX,offsetY;
    private final android.content.SharedPreferences prefs;
    float controlSize(){return controlSize;}
    float controlOpacity(){return opacity;}
    void setControlSize(float value){releaseAll();controlSize=Math.max(.5f,Math.min(1.6f,value));clampPositions();prefs.edit().putFloat("size",controlSize).apply();savePositions();invalidate();}
    void setControlOpacity(float value){opacity=Math.max(.1f,Math.min(1,value));prefs.edit().putFloat("opacity",opacity).apply();invalidate();}
    void editPositions(){releaseAll();editing=true;invalidate();}
    boolean finishEditing(){if(!editing)return false;editing=false;dragPointer=-1;dragButton=null;dragJoystick=false;savePositions();invalidate();return true;}
    void resetLayout(){finishEditing();prefs.edit().clear().apply();controlSize=1;opacity=.8f;onSizeChanged(getWidth(),getHeight(),0,0);invalidate();}
    private float inside(float value,float radius,float edge){return Math.max(radius,Math.min(edge-radius,value));}
    private void clampPositions(){jx=inside(jx,94*controlSize,width);jy=inside(jy,94*controlSize,height);for(Button b:buttons){float r=(b.r+8)*controlSize;b.x=inside(b.x,r,width);b.y=inside(b.y,r,height);}}
    private void savePositions(){android.content.SharedPreferences.Editor e=prefs.edit().putFloat("jx",jx/width).putFloat("jy",jy/height);for(Button b:buttons)e.putFloat("x"+b.scan,b.x/width).putFloat("y"+b.scan,b.y/height);e.apply();}
    private boolean hits(Button b,float x,float y){return Math.hypot(x-b.x,y-b.y)<(b.r+8)*controlSize;}
    private boolean editTouch(MotionEvent e){
        int a=e.getActionMasked(),index=a==MotionEvent.ACTION_DOWN?0:e.findPointerIndex(dragPointer);
        if(index<0){if(a==MotionEvent.ACTION_CANCEL){dragPointer=-1;dragButton=null;dragJoystick=false;}return true;}
        float x=e.getX(index)/scale,y=e.getY(index)/scale;
        if(a==MotionEvent.ACTION_DOWN){
            if(x>width/2-180&&x<width/2+180&&y<65){finishEditing();return true;}
            dragPointer=e.getPointerId(0);
            dragButton=null;dragJoystick=false;
            for(Button b:buttons)if(hits(b,x,y)){dragButton=b;offsetX=x-b.x;offsetY=y-b.y;break;}
            if(dragButton==null&&Math.hypot(x-jx,y-jy)<110*controlSize){dragJoystick=true;offsetX=x-jx;offsetY=y-jy;}
        }else if(a==MotionEvent.ACTION_MOVE){
            if(dragButton!=null){float r=(dragButton.r+8)*controlSize;dragButton.x=inside(x-offsetX,r,width);dragButton.y=inside(y-offsetY,r,height);}
            else if(dragJoystick){float r=94*controlSize;jx=Math.max(r,Math.min(width-r,x-offsetX));jy=Math.max(r,Math.min(height-r,y-offsetY));}
        }else if(a==MotionEvent.ACTION_UP||a==MotionEvent.ACTION_CANCEL||(a==MotionEvent.ACTION_POINTER_UP&&e.getPointerId(e.getActionIndex())==dragPointer)){dragPointer=-1;dragButton=null;dragJoystick=false;savePositions();}
        invalidate();return true;
    }
    private final GameActivity activity;
    TouchControls(GameActivity c){super(c);activity=c;prefs=c.getSharedPreferences("touch",0);controlSize=Math.max(.5f,Math.min(1.6f,prefs.getFloat("size",1)));opacity=Math.max(.1f,Math.min(1,prefs.getFloat("opacity",.8f)));setFocusable(false);setContentDescription("虚拟摇杆和游戏按键");}
    protected void onSizeChanged(int w,int h,int oldw,int oldh){releaseAll();scale=Math.min(w/960f,h/540f);width=w/scale;height=h/scale;jx=120;jy=height-120;buttons.clear();buttons.add(new Button(width-104,height-112,50,13,KeyEvent.KEYCODE_J,"A",0xff45b5a5));buttons.add(new Button(width-221,height-67,43,14,KeyEvent.KEYCODE_K,"B",0xffe49a43));buttons.add(new Button(width-62,54,31,16,KeyEvent.KEYCODE_M,"地图",0xff548dce));buttons.add(new Button(64,54,31,41,KeyEvent.KEYCODE_ESCAPE,"返回",0xff596a7c));buttons.add(new Button(width-154,54,31,0,0,"画面",0xff7966b0));jx=prefs.getFloat("jx",jx/width)*width;jy=prefs.getFloat("jy",jy/height)*height;for(Button b:buttons){b.x=prefs.getFloat("x"+b.scan,b.x/width)*width;b.y=prefs.getFloat("y"+b.scan,b.y/height)*height;}clampPositions();}
    private void metalButton(Canvas c,Button b,boolean active){c.save();c.translate(b.x,b.y);c.scale(controlSize,controlSize);c.translate(-b.x,-b.y);float press=active?3:0,r=b.r;paint.clearShadowLayer();paint.setStyle(Paint.Style.FILL);paint.setColor(0x99000000);c.drawCircle(b.x+2,b.y+6,r+5,paint);paint.setShader(new LinearGradient(b.x-r,b.y-r,b.x+r,b.y+r,active?0xfff2f6f7:0xffdce5e9,active?0xff697982:0xff596975,Shader.TileMode.CLAMP));c.drawCircle(b.x,b.y+press,r+5,paint);paint.setShader(null);paint.setShader(new LinearGradient(b.x-r,b.y-r,b.x+r,b.y+r,active?0xffd9fff5:b.color,active?0xff247d75:0xff263947,Shader.TileMode.CLAMP));c.drawCircle(b.x,b.y+press,r,paint);paint.setShader(null);paint.setStyle(Paint.Style.STROKE);paint.setStrokeWidth(1.5f);paint.setColor(active?0xfff5ffff:0x99e9f5fa);c.drawCircle(b.x,b.y+press,r-3,paint);paint.setStyle(Paint.Style.FILL);paint.setTypeface(Typeface.create("sans-serif-condensed",Typeface.BOLD));paint.setTextAlign(Paint.Align.CENTER);paint.setTextSize(b.label.length()>1?11:24);paint.setColor(active?0xff183235:0xfff3f8fa);c.drawText(b.label,b.x,b.y+press+(b.label.length()>1?4:8),paint);c.restore();}
    private void joystick(Canvas c){c.save();c.translate(jx,jy);c.scale(controlSize,controlSize);c.translate(-jx,-jy);float r=89;paint.clearShadowLayer();paint.setStyle(Paint.Style.FILL);paint.setColor(0x88000000);c.drawCircle(jx,jy+5,r+5,paint);paint.setShader(new LinearGradient(jx-r,jy-r,jx+r,jy+r,0xb9455660,0x99404c55,Shader.TileMode.CLAMP));c.drawCircle(jx,jy,r,paint);paint.setShader(null);paint.setStyle(Paint.Style.STROKE);paint.setStrokeWidth(2);paint.setColor(0x99e1edf2);c.drawCircle(jx,jy,r,paint);paint.setStrokeWidth(1);paint.setColor(0x557f99a5);c.drawCircle(jx,jy,60,paint);paint.setStyle(Paint.Style.FILL);float kx=jx+dx,ky=jy+dy;paint.setColor(0x66000000);c.drawCircle(kx+1,ky+4,38,paint);paint.setShader(new LinearGradient(kx-30,ky-33,kx+32,ky+38,joystick>=0?0xffe8f1f2:0xffaab8bf,joystick>=0?0xff687985:0xff56656e,Shader.TileMode.CLAMP));c.drawCircle(kx,ky,36,paint);paint.setShader(null);paint.setStyle(Paint.Style.STROKE);paint.setStrokeWidth(2);paint.setColor(0xffe0e8ec);c.drawCircle(kx,ky,35,paint);paint.setStyle(Paint.Style.FILL);c.restore();}
    protected void onDraw(Canvas c){c.save();c.scale(scale,scale);int layer=c.saveLayerAlpha(0,0,width,height,editing?255:(int)(opacity*255));joystick(c);for(Button b:buttons)metalButton(c,b,held.contains(b.scan)&&b.scan!=0);c.restoreToCount(layer);if(editing){paint.setColor(0xee15232f);c.drawRoundRect(width/2-180,8,width/2+180,65,12,12,paint);paint.setColor(Color.WHITE);paint.setTextSize(15);paint.setTextAlign(Paint.Align.CENTER);c.drawText("拖动按键和摇杆 · 点击此处完成",width/2,42,paint);}c.restore();}
    public boolean onTouchEvent(MotionEvent e){if(editing)return editTouch(e);int action=e.getActionMasked(),idx=e.getActionIndex(),id=e.getPointerId(idx);if(action==MotionEvent.ACTION_CANCEL){releaseAll();return true;}if(action==MotionEvent.ACTION_DOWN||action==MotionEvent.ACTION_POINTER_DOWN){float x=e.getX(idx)/scale,y=e.getY(idx)/scale;if(joystick<0&&Math.hypot(x-jx,y-jy)<110*controlSize)joystick=id;else for(Button b:buttons)if(hits(b,x,y)){if(b.scan==0){releaseAll();activity.showDisplaySettings();return true;}owners.put(id,b);performHapticFeedback(HapticFeedbackConstants.VIRTUAL_KEY);break;}}if(action==MotionEvent.ACTION_UP||action==MotionEvent.ACTION_POINTER_UP){owners.remove(id);if(id==joystick){joystick=-1;dx=dy=0;}}Set<Integer> next=new HashSet<>();for(int p=0;p<e.getPointerCount();p++){int pid=e.getPointerId(p);if((action==MotionEvent.ACTION_UP||action==MotionEvent.ACTION_POINTER_UP)&&p==idx)continue;float x=e.getX(p)/scale,y=e.getY(p)/scale;if(pid==joystick){float vx=x-jx,vy=y-jy;double len=Math.hypot(vx,vy);float f=len>62*controlSize?(float)(62*controlSize/len):1;dx=vx*f/controlSize;dy=vy*f/controlSize;if(len>17*controlSize){if(Math.abs(vx)>len*0.38)next.add(vx<0?4:7);if(Math.abs(vy)>len*0.38)next.add(vy<0?26:22);}}Button b=owners.get(pid);if(b!=null&&hits(b,x,y))next.add(b.scan);}for(int scan:new HashSet<>(held))if(!next.contains(scan))send(scan,false);for(int scan:next)if(!held.contains(scan))send(scan,true);held.clear();held.addAll(next);invalidate();return true;}
    private void send(int scan,boolean down){GameActivity.touchKey(scan,down);if(scan==41){if(down)SDLActivity.onNativeKeyDown(KeyEvent.KEYCODE_ESCAPE);else SDLActivity.onNativeKeyUp(KeyEvent.KEYCODE_ESCAPE);}}
    void releaseAll(){for(int scan:held)send(scan,false);held.clear();owners.clear();joystick=-1;dx=dy=0;GameActivity.clearTouch();invalidate();}
}
