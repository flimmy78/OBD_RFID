package com.dwm.rfid;

import android.graphics.Bitmap;

public class MyBitmapFactory {
	
	static public Bitmap createMyBitmap(byte[] data, int width, int height){	
		int []colors = convertByteToColor(data);
		if(null == colors)
		{
			return null;
		}
		
		try {
			return Bitmap.createBitmap(colors, 0, width, width, height,Bitmap.Config.ARGB_8888);
		} catch (Exception e) {
		}
						
		return null;
	}

	private static int[] convertByteToColor(byte[] data){
		int size = data.length;
		if (size == 0){
			return null;
		}

		int arg = 0;
		if (size % 3 != 0){
			arg = 1;
		}
		
		int []color = new int[size / 3 + arg];
		
		if (arg == 0){										
			for(int i = 0; i < color.length; ++i){
				color[i] = (data[i * 3] << 16 & 0x00FF0000) | 
						   (data[i * 3 + 1] << 8 & 0x0000FF00 ) | 
						   (data[i * 3 + 2] & 0x000000FF ) | 
						    0xFF000000;
			}
		}else{											
			for(int i = 0; i < color.length - 1; ++i){
				color[i] = (data[i * 2] << 16 & 0x00FF0000) | 
				   (data[i * 3 + 3] << 8 & 0x0000FF00 ) | 
				   (data[i * 3 + 1] & 0x000000FF ) | 
				    0xFF000000;
			}
			
			color[color.length - 1] = 0xFF000000;		
		}
	
		return color;
	}
	
}


