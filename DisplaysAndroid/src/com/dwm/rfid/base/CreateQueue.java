package com.dwm.rfid.base;

import java.util.ArrayList;
import java.util.List;

public class CreateQueue {
	private Mutex mKeyMutex = null;
	private List<ReceiveBuf> lList = new ArrayList<ReceiveBuf>();
	
	public CreateQueue()
	{
		mKeyMutex = new Mutex();
	}
	
	public void AddBuffQueue(ReceiveBuf frBuf)
	{
		if(null == frBuf)
		{
			return;
		}
		mKeyMutex.lock();
		lList.add(frBuf);
		mKeyMutex.unlock();
	}
	
	public ReceiveBuf getBuffQueue()
	{
		if(lList.size() == 0)
		{
			return null;
		}
		mKeyMutex.lock();
		ReceiveBuf frBuf = lList.get(0);
		lList.remove(0);
		mKeyMutex.unlock();
		return frBuf;
	}
	
	public int getBuffQueueCount()
	{
		mKeyMutex.lock();
		int iSize = lList.size();
		mKeyMutex.unlock();
		return iSize;
	}
}
