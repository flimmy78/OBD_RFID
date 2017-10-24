package com.dwm.rfid;
import com.dwm.rfid.base.CreateQueue;
import com.dwm.rfid.base.ReceiveBuf;
import android.os.Bundle;
import android.os.Handler;
import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.drawable.BitmapDrawable;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Gravity;
import android.view.Menu;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.TextView;

public class RfidActivity extends Activity {
	private CInterface pCInterface = null;
	private Handler RunHandler=new Handler();
	private TextView tUserView = null;
	private TextView tUserPay = null;
	private TextView tUserSpeed = null;
	private TextView tPayView = null;
	private SurfaceView  m_prevewview = null;
	private SurfaceHolder surfaceHolder = null;
	Bitmap bmpPanel = null;
	private int iScreenWidth = 0;
	private int iScreenHeight = 0;
	
	private Rect rARect = new Rect();
	private Matrix mt = new Matrix();
	private Paint paint = new Paint();
	
	private CreateQueue cQueue = null;
	private double dAllCount = 0;
	
	private Handler gameHandler = new Handler();
	
	private Runnable TimeThread=new  Runnable(){
		@Override
		public void run() {
			if(pCInterface != null)
			{
				pCInterface.RegardToServer();
			}
		}
	};
	
	private Runnable RuningThread=new  Runnable(){
		@Override
		public void run() {
			if(pCInterface != null)
			{
				int iRet = pCInterface.IsConnected();
				if(iRet > 0)
				{
					if(null != tUserPay)
					{
						tUserSpeed.setText("网速:" + iRet/1024 + "kb/s");
					}
				}
				else
				{
					if(null != tUserPay)
					{
						tUserSpeed.setText("网速:0kb/s");
					}
					
					ConnectToServer();
				}
			}
			RunHandler.postDelayed(RuningThread, 5000);
		}
	};
	
	private void ConnectToServer()
	{
        if(pCInterface != null)
		{
        	pCInterface.Close();
        	pCInterface.Init();
        	if(pCInterface.ConnectToServer("10.0.0.14", 15566))
        	{
        		DrawPanel(bmpPanel);
    			RunHandler.postDelayed(TimeThread, 3000);
        	}
		}
	}
	
	public void SetRfidActivity()
	{
		CInterface.setpRfidActivity(this);
	}
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_rfid);
        SetRfidActivity();
        if(null == tUserView)
        {
        	tUserView = (TextView)findViewById(R.id.textview_user_info);
        	tUserView.setText("物品价格表");
        }
        
        if(null == tUserPay)
        {
        	tUserPay = (TextView)findViewById(R.id.textview_user_All);
        	tUserPay.setText("应付款: 0 元");
        }
        
        if(null == tUserSpeed)
        {
        	tUserSpeed = (TextView)findViewById(R.id.textview_speed);
        	tUserSpeed.setText("网速:0kb/s");
        }
        
        if(null == tPayView)
        {
        	tPayView = (TextView)findViewById(R.id.textview_pay);
        }  
        
        if(null == m_prevewview)
        {
        	m_prevewview = (SurfaceView) findViewById(R.id.surface_view);
            surfaceHolder = m_prevewview.getHolder();
            DisplayMetrics dm = getResources().getDisplayMetrics();  
    		iScreenWidth = dm.widthPixels;  
    		iScreenHeight = dm.heightPixels-100; 
    		rARect.set(0, 0, 160, 120);
    		paint.setAntiAlias(true);
    		mt.postScale(1.5f, 2.5f);
    		mt.postRotate(90);
        }  
        
        if(null == bmpPanel)
        {
        	bmpPanel = ((BitmapDrawable)getResources().getDrawable(R.drawable.ktd)).getBitmap();
        }
        
        
        if(null == cQueue)
        {
        	cQueue = new CreateQueue();
        }
        
        if(null == pCInterface)
        {
        	pCInterface = new CInterface();
        }
        
        RunHandler.postDelayed(RuningThread, 1000);
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.rfid, menu);
        return true;
    }
    
	private Runnable drawThread=new  Runnable(){

		@Override
		public void run() {
			update();
		}
	};
	
	protected void update() {
		
		ReceiveBuf frBuf = cQueue.getBuffQueue();
		switch(frBuf.iType)
		{
		case 0x97:
		{
			if(0 == dAllCount)
			{
				tPayView.setText("");
			}
			String tCurrentTest = tPayView.getText().toString();
			tPayView.setText(tCurrentTest + "\n" + frBuf.pString);
			dAllCount += frBuf.iCost;
			tUserPay.setText("应付款: " + dAllCount + "元");
			break;
		}
		case 0x98:
		{
			tPayView.setText("");
			dAllCount = 0;
			tUserPay.setText("应付款: 0 元");
			DrawPanel(bmpPanel);
			break;
		}
		case 0x99:
		{
			pCInterface.RequestToPay(dAllCount);
			dAllCount = 0;
			tUserPay.setText("应付款: 0 元");
			DrawPanel(bmpPanel);
			tPayView.setText("谢谢惠顾!");
			break;
		}
		default:
		{
			break;
		}
		}
	}
	
	public void DrawPanel(Bitmap PanelBitmap) {
		Canvas canvas = surfaceHolder.lockCanvas();
		if (canvas != null) {
			canvas.drawBitmap(PanelBitmap, null, rARect, null);
			surfaceHolder.unlockCanvasAndPost(canvas);
		}
	}
    
    public void SetUserTextView()
    {
    	
    }
    
    public void SetPayTextView(int iType, String tTest, double iCurrentData)
    {
    	ReceiveBuf frBuf = new ReceiveBuf();
    	frBuf.iType = iType;
    	frBuf.pString = tTest;
    	frBuf.iCost = iCurrentData;
    	cQueue.AddBuffQueue(frBuf);
    	gameHandler.postDelayed(drawThread, 10);
    }
 
	public void DrawVideo(byte[] strBuf, int iA, int iB) {
		Canvas canvas = surfaceHolder.lockCanvas();
		if (canvas != null) {
			Bitmap bt = Bitmap.createBitmap(MyBitmapFactory.createMyBitmap(strBuf, iA,iB), 0, 0, iA, iB, mt, false);
			canvas.drawBitmap(bt, null, rARect, null);
			surfaceHolder.unlockCanvasAndPost(canvas);
		}
	}
}
