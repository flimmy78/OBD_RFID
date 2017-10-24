package com.dwm.base;

import android.util.Log;

public class CInterface {

	public native void Init(int iw, int ih, int ip, int im);
	public native boolean RequestConnectToServer(String serveraddr, int port);
	public native boolean SendBuff(int iType, byte[] bDate, long lLen,int iValue1,int iValue2);
	public native boolean RequestToScan();
	public native boolean RequestToStopScan();
	public native boolean RequestToPay();
	public native int GetSendBuffCount();

	public static void NotifyForSystemString(int action, String pBuf, int iLen)
	{
		Log.i("NotifyForSystemString", pBuf);
	}
	
	static {
		System.loadLibrary("avcodec-57");
		System.loadLibrary("avdevice-57");
		System.loadLibrary("avfilter-6");
		System.loadLibrary("avformat-57");
		System.loadLibrary("avutil-55");
		System.loadLibrary("postproc-54");
		System.loadLibrary("swresample-2");
		System.loadLibrary("swscale-4");
		System.loadLibrary("gnustl_shared");
		System.loadLibrary("mainControl");
	}
	
}
