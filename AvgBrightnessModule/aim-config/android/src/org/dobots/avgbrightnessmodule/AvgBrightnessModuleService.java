package org.dobots.avgbrightnessmodule;

import java.util.HashMap;
import java.util.Timer;
import java.util.TimerTask;

import org.dobots.aim.AimProtocol;
import org.dobots.aim.AimService;
import org.dobots.aim.AimUtils;
import org.dobots.aim.AimProtocol.AimDataTypeException;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.util.Base64;
import android.util.Log;

public class AvgBrightnessModuleService extends AimService {
	
	private static final String TAG = "AvgBrightnessService";
	Timer mSetCameraTimer = new Timer();

	private Messenger mMessengerImage = new Messenger(new Handler() {

		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case AimProtocol.MSG_PORT_DATA:
				// handle image data
				try {
					handleImage(AimUtils.getStringData(msg));
				} catch (AimDataTypeException e) {
					e.printStackTrace();
				}
				break;
			default:
				super.handleMessage(msg);
			}
		}
	});
	
	public void start() {
		super.start();
		mSetCameraTimer.schedule(setCamParameters, 0, 20);
	};
	
	@Override
	public String getModuleName() {
		return "AvgBrightnessModule";
	}

	@Override
	public void defineInMessenger(HashMap<String, Messenger> list) {
		list.put("image", mMessengerImage);
	}

	@Override
	public void defineOutMessenger(HashMap<String, Messenger> list) {
		list.put("brightness", null);
		list.put("commandout", null);
	}

	@Override
	public IBinder onBind(Intent arg0) {
		return null;
	}
	
	// TODO: this only works 1 time
	TimerTask setCamParameters = new TimerTask() {
		@Override
		public void run() {
			Messenger camMessenger = getOutMessenger("commandout");
			if (camMessenger != null) {
				sendData(camMessenger, "{\"data\":{\"command\":\"setAutoExposure\",\"parameter\":[false]},\"id\":2}");
				sendData(camMessenger, "{\"data\":{\"command\":\"setFrameRate\",\"parameter\":[20.0]},\"id\":2}");
				sendData(camMessenger, "{\"data\":{\"command\":\"setSize\",\"parameter\":[320, 240]},\"id\":2}");
				mSetCameraTimer.cancel();
			}
		}
	};
	
	void handleImage(String base64) {
		byte[] jpg = Base64.decode(base64, Base64.NO_WRAP);
		Bitmap bmp = BitmapFactory.decodeByteArray(jpg, 0, jpg.length);
		
		// TODO: increase speed by first scaling?
		// createScaledBitmap(Bitmap src, int dstWidth, int dstHeight, boolean filter)
		
		int[] pixels = new int[bmp.getWidth() * bmp.getHeight()];
		bmp.getPixels(pixels, 0, bmp.getWidth(), 0, 0, bmp.getWidth(), bmp.getHeight());
		
		// Pixels is in ARGB 8888 format
		float sum=0;
		int r,g,b;
		for (int i : pixels) {
			r = (i & 0x00FF0000) >> 16;
			g = (i & 0x0000FF00) >> 8;
			b = (i & 0x000000FF);
			sum += r+g+b;
//			Log.d(TAG, "r=" + r + " g=" + g + " b=" + b);
		}
		
		float avg = sum / pixels.length / 3;
		
		sendData(getOutMessenger("brightness"), avg);
	}

}
