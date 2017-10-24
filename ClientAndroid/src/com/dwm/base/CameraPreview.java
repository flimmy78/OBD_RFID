package com.dwm.base;

import java.io.IOException;
import java.util.List;
import android.util.Log;
import android.view.SurfaceView;
import android.view.SurfaceHolder;

import android.content.Context;

import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.hardware.Camera.PreviewCallback;
import android.hardware.Camera.AutoFocusCallback;
import android.hardware.Camera.Size;

public class CameraPreview extends SurfaceView implements SurfaceHolder.Callback{
    private SurfaceHolder mHolder;
    private Camera mCamera;
    private PreviewCallback previewCallback;
    private AutoFocusCallback autoFocusCallback;
    private int iWidth = 0;
    private int iHeight = 0;

	@SuppressWarnings("deprecation")
	public CameraPreview(Context context, Camera camera,
                         PreviewCallback previewCb,
                         AutoFocusCallback autoFocusCb, int iCurrentW, int iCurrentH) {
        super(context);
    	mCamera = camera;
    	previewCallback = previewCb;
        autoFocusCallback = autoFocusCb;
        iWidth = iCurrentW;
        iHeight = iCurrentH;
        
    	mHolder = getHolder();
        mHolder.addCallback(this);
        mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
    }

    public void surfaceCreated(SurfaceHolder holder) {
        // The Surface has been created, now tell the camera where to draw the preview.
        try {
            mCamera.setPreviewDisplay(holder);
        } catch (IOException e) {
            Log.d("DBG", "Error setting camera preview: " + e.getMessage());
        }
    }

    public void surfaceDestroyed(SurfaceHolder holder) {
        // Camera preview released in activity
    }

    @SuppressWarnings("deprecation")
	public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        /*
         * If your preview can change or rotate, take care of those events here.
         * Make sure to stop the preview before resizing or reformatting it.
         */
        if (mHolder.getSurface() == null){
          // preview surface does not exist
          return;
        }

        // stop preview before making changes
        try {
            mCamera.stopPreview();
        } catch (Exception e){
          // ignore: tried to stop a non-existent preview
        }

        try {
            // Hard code camera surface rotation 90 degs to match Activity view in portrait
        	Camera.Parameters parameters = mCamera.getParameters();
        	List<Size> previewSizes = mCamera.getParameters()
					.getSupportedPreviewSizes();
        	for (int i = 0; i < previewSizes.size(); i++) {
				Size s = previewSizes.get(i);
				if (s.width == 640 && s.height == 480) {
					//parameters.setPreviewSize(480, 320);
					break;
				}
			}
        	parameters.setPreviewSize(iWidth,iHeight);
        	//parameters.setPreviewFrameRate(20);
            parameters.setPreviewFormat(ImageFormat.YV12);

            mCamera.setParameters(parameters);
            mCamera.setDisplayOrientation(90);
            mCamera.setPreviewDisplay(mHolder);
            mCamera.setPreviewCallback(previewCallback);
            mCamera.startPreview();
            //mCamera.autoFocus(autoFocusCallback);
        } catch (Exception e){
            Log.d("DBG", "Error starting camera preview: " + e.getMessage());
        }
    }
}
