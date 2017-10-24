package com.dwm.rfid.base;

import android.util.Log;

public class Mutex {
	private boolean bIsLock;  
	public Mutex()  
	{
		bIsLock = false;
	}
	
	public synchronized void lock()  
	{
		while(true == bIsLock) 
		{
			try {  
                wait();  
            }  
            catch (Exception e) {  
                Log.e("Mutex", "wait error");
            };  
		}
	}
	
	public synchronized void unlock()  
    {  
		bIsLock = false;  
        notifyAll();  
    }  
}
