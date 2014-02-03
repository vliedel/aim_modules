package org.dobots.jpgtobmpmodule;

import org.dobots.aim.AimProtocol;
import org.dobots.aim.AimService;

import java.util.HashMap;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.util.Log;

public class JpgToBmpModuleService extends AimService {
	private static final String TAG = "JpgToBmpModuleService";

	Messenger mPortJpgInMessenger = new Messenger(new Handler() {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case AimProtocol.MSG_PORT_DATA: {
				
				byte[] bytes = android.util.Base64.decode(msg.getData().getString("data"), android.util.Base64.NO_WRAP);
				Bitmap bmp = BitmapFactory.decodeByteArray(bytes, 0, bytes.length);
				
				int height = bmp.getHeight();
				int width = bmp.getWidth();
			
				int[] msgData = new int[width * height * 3 + 6];
				msgData[0] = 0; // datatype
				msgData[1] = 1; // Narrays
				msgData[2] = 3; // Ndims
				msgData[3] = height; // size dim 1
				msgData[4] = width; // size dim 2
				msgData[5] = 3; // size dim 3 (rgb)
				
				Log.d(TAG, "height=" + height + "width=" + width);
				
				// GetPixels returns ARGB_8888
				
//				// Super slow
//				long start1 = System.currentTimeMillis();
//				for (int y=0, i=6; y<height; ++y) {
//					for (int x=0; x<width; ++x, i+=3) {
//						msgData[i] = (bmp.getPixel(x, y) >> 16) & 0xFF;
//						msgData[i+1] = (bmp.getPixel(x, y) >> 8) & 0xFF;
//						msgData[i+2] = bmp.getPixel(x, y) & 0xFF;
//					}
//				}
//				Log.d(TAG, "loop: " + (System.currentTimeMillis() - start1));
				
			
//				long start = System.currentTimeMillis();
				int[] pixels = new int[width * height];
				bmp.getPixels(pixels, 0, width, 0, 0, width, height);
//				long getpixels = System.currentTimeMillis();
//				Log.d(TAG, "getpixels: " + (getpixels - start));
				for (int i=0, j=6; i<pixels.length; ++i, j+=3) {
					msgData[j] = (pixels[i] >> 16) & 0xFF;
					msgData[j+1] = (pixels[i] >> 8) & 0xFF;
					msgData[j+2] = pixels[i] & 0xFF;
				}
//				Log.d(TAG, "loop: " + (System.currentTimeMillis() - getpixels));
				
				Bundle bundle = new Bundle();
				bundle.putInt("datatype", AimProtocol.DATATYPE_INT_ARRAY);
				bundle.putIntArray("data", msgData);
				sendData(getOutMessenger("bmp"), bundle);

				break;
			}
			default:
				super.handleMessage(msg);
			}
		}
	});
	
	@Override
	public String getModuleName() {
		return "JpgToBmpModule";
	}

	@Override
	public void defineInMessenger(HashMap<String, Messenger> list) {
		list.put("jpg", mPortJpgInMessenger);
	}

	@Override
	public void defineOutMessenger(HashMap<String, Messenger> list) {
		list.put("bmp", null);
	}

	
	public void start() {
		super.start();
	}

	public void onCreate() {
		super.onCreate();
	}

	public void onDestroy() {
		super.onDestroy();
	}

	@Override
	public IBinder onBind(final Intent intent) {
		Log.i(TAG,"onBind: " + intent.toString());
		return null; // No binding provided
		//		return mFromMsgService.getBinder();
	}
}
