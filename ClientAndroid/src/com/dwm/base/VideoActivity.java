package com.dwm.base;
import android.hardware.Camera;
import android.hardware.Camera.AutoFocusCallback;
import android.hardware.Camera.PreviewCallback;
import android.os.Bundle;
import android.os.Handler;
import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.FrameLayout;

public class VideoActivity extends Activity {

	private Camera mCamera = null;
    private CameraPreview mPreview = null;
    private Button bAutoStart = null;
    private Button bStopStart = null;
    private Button bPayStart = null;
    
    private Handler autoFocusHandler = null;
    private boolean previewing = true;
    private boolean bIsConnect = false;
    private boolean bIsNeedToSend = false;
    private CInterface pCInterface = null;
    private int iCurrentW = 320;
    private int iCurrentH = 240;  
    
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_video);

        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

        if(null == bAutoStart)
        {
        	bAutoStart = (Button)findViewById(R.id.button_scan);
        	bAutoStart.setOnClickListener(Buttonlistener);
        }
        
        if(null == bStopStart)
        {
        	bStopStart = (Button)findViewById(R.id.button_clear);
        	bStopStart.setOnClickListener(Buttonlistener);
        }
        
        if(null == bPayStart)
        {
        	bPayStart = (Button)findViewById(R.id.button_pay);
        	bPayStart.setOnClickListener(Buttonlistener);
        }
            
        if(null == autoFocusHandler)
        {
        	autoFocusHandler = new Handler();
        }
        
        if(null == pCInterface)
        {
        	pCInterface = new CInterface();
        	pCInterface.Init(iCurrentW,iCurrentH,20,20);
        	bIsConnect = pCInterface.RequestConnectToServer("10.0.0.14", 15566);
        }
        
        mCamera = getCameraInstance();
        
        mPreview = new CameraPreview(this, mCamera, previewCb, autoFocusCB,iCurrentW,iCurrentH);
        FrameLayout preview = (FrameLayout)findViewById(R.id.VideoSurFasce);
        preview.addView(mPreview);
   
    }
    
    OnClickListener Buttonlistener = new OnClickListener() {
		@Override
		public void onClick(View v) {
			Log.i("OnClickListener", "start");
			if(null == pCInterface)
			{
				Log.i("OnClickListener", "pCInterface is NULL");
				return;
			}
			
			switch(v.getId())
			{
				case R.id.button_scan:
				{
					bIsNeedToSend = true;
					pCInterface.RequestToScan();
					break;
				}
				case R.id.button_clear:
				{
					pCInterface.RequestToStopScan();
					break;
				}
				case R.id.button_pay:
				{
					pCInterface.RequestToPay();
					break;
				}
				default:
				{
					
				}
			}
		}
	};


    public void onPause() {
        super.onPause();
        releaseCamera();
    }

    public static Camera getCameraInstance(){
        Camera CurrentCamera = null;
        
        int numCameras = Camera.getNumberOfCameras();
	    if (numCameras == 0) {
	      Log.w("getCameraInstance", "No cameras!");
	      return null;
	    }
	    
	    int iIndex = 0;
		int iCameraId = 0;
		while (iIndex < numCameras) {
	    	Camera.CameraInfo cameraInfo = new Camera.CameraInfo();
	        Camera.getCameraInfo(iIndex, cameraInfo);
	        if (cameraInfo.facing == Camera.CameraInfo.CAMERA_FACING_BACK) {
	          break;
	        }
	        iIndex++;
	    }
		
		iCameraId = iIndex;
		Log.w("getCameraInstance", "camers ID is " + iCameraId);
	    CurrentCamera = Camera.open(iCameraId);
	    
	    if(null == CurrentCamera)
	    {
	    	Log.w("getCameraInstance", "CurrentCamera is null!");
	    	return null;
	    }
	    
        return CurrentCamera;
    }

    private void releaseCamera() {
        if (mCamera != null) {
            previewing = false;
            mCamera.setPreviewCallback(null);
            mCamera.release();
            mCamera = null;
        }
    }

    private Runnable doAutoFocus = new Runnable() {
            public void run() {
                if (previewing)
                {
                	mCamera.autoFocus(autoFocusCB);
                }
            }
        };

    PreviewCallback previewCb = new PreviewCallback() {
            public void onPreviewFrame(byte[] data, Camera camera) {
                if(pCInterface != null)
                {           	
                	if(bIsConnect)
                	{
                		if(bIsNeedToSend && pCInterface.GetSendBuffCount() == 0)
                		{
                			bIsNeedToSend = false;
                			pCInterface.SendBuff(0x11, data, data.length,iCurrentW,iCurrentH);
                		}
                	}
                }
            }
        };

    AutoFocusCallback autoFocusCB = new AutoFocusCallback() {
            public void onAutoFocus(boolean success, Camera camera) {
                autoFocusHandler.postDelayed(doAutoFocus, 1000);
            }
        };
    
}
