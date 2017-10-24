package com.dwm.rfid;

public class CInterface {
	private static RfidActivity pRfidActivity = null;
	
	public native boolean Init();
	public native boolean Close();
	public native boolean ConnectToServer(String serveraddr, int port);
	public native int IsConnected();
	public native boolean RegardToServer();
	public native boolean RequestToPay(double dM);
	public native boolean DisConnect();

	public static void NotifyForSystemBuf(int action, byte[] pBuf, int iLen, int iValue1, int iValue2)
	{
		if(getpRfidActivity() != null && iLen == iValue1*iValue2*3/2)
		{
			getpRfidActivity().DrawVideo(pBuf, iValue1, iValue2);
		}
	}
	
	public static void NotifyForSystemString(int action, String pBuf, int iLen)
	{
		String[] BufArray = pBuf.toString().split(">");
		if(BufArray.length > 7)
		{
			int costoff = 100;
			if(BufArray[7].split("\"").length > 0)
			{
				costoff = Integer.parseInt(BufArray[7].split("\"")[0]) ;
			}
			
			double iCurrentData = (int)(Integer.parseInt(BufArray[6]) * Integer.parseInt(BufArray[4]) * costoff) / 100;
			
			String sText = "商品: " + BufArray[1] + "   单价:" + BufArray[6] +  "     数量:" + BufArray[4] + "    折扣:" + costoff + "%     价格:" + iCurrentData;
			if(getpRfidActivity() != null)
			{
				getpRfidActivity().SetPayTextView(action,sText,iCurrentData);
			}
		}
		else
		{
			if(getpRfidActivity() != null)
			{
				getpRfidActivity().SetPayTextView(action,pBuf,0);
			}
		}
		
	}
	
	public static RfidActivity getpRfidActivity() {
		return pRfidActivity;
	}
	public static void setpRfidActivity(RfidActivity pRfidActivity) {
		CInterface.pRfidActivity = pRfidActivity;
	}

	static {
		System.loadLibrary("opencv_java3");
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
