package org.dobots.bmptojpgmodule;

import org.dobots.aim.AimProtocol;
import org.dobots.aim.AimService;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.ByteArrayOutputStream;
import java.util.HashMap;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;

import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.util.Log;

public class BmpToJpgModuleService extends AimService {
	private static final String TAG = "BmpToJpgModuleService";
	private int mQuality = 75;

	Messenger mPortBmpInMessenger = new Messenger(new Handler() {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case AimProtocol.MSG_PORT_DATA: {
				int[] readVal = msg.getData().getIntArray("data");
				int dataType = readVal[0];
				int nArrays = readVal[1];
				int nDims = readVal[2];
				if (nDims != 3) {
					Log.w(TAG, "bmp data should have 3 dimensions");
					break;
				}
				int height = readVal[3];
				int width = readVal[4];
				int channels = readVal[5];
				if (channels != 3) {
					Log.w(TAG, "bmp data should have 3 channels");
					break;
				}
				
				int[] pixels = new int[height*width];
//				for (int x=0; x<width; x++) {
//					for (int y=0; y<height; y++) {
//						for (int c=0; c<channels; c++) {						
//						}
//					}
//				}
				
				Log.d(TAG,  "height=" + height + " width=" + width + " channels=" + channels);
				for (int i=0, j=0; i < height*width*channels; i+=3, j++) {
					int r = readVal[6+i+0]; int g = readVal[6+i+1]; int b = readVal[6+i+2];
//					Log.i(TAG, "i=" + i + " r=" + r + " g=" + g + " b=" + b);
					pixels[j] = 0xFF000000 | (r & 0xFF) << 16 | (g & 0xFF) << 8 | (b & 0xFF);
				}

				Bitmap bmp = Bitmap.createBitmap(pixels, width, height, Bitmap.Config.ARGB_8888);
				
				// Convert to jpeg
				ByteArrayOutputStream os = new ByteArrayOutputStream();
				bmp.compress(CompressFormat.JPEG, mQuality, os);
				byte[] bytes = os.toByteArray();
				
				// Send message
				String msgData = android.util.Base64.encodeToString(bytes, android.util.Base64.NO_WRAP);
				sendData(getOutMessenger("jpg"), msgData);

				break;
			}
			default:
				super.handleMessage(msg);
			}
		}
	});
	
	Messenger mPortCommandInMessenger = new Messenger(new Handler() {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case AimProtocol.MSG_PORT_DATA: {
				JSONObject json;
				try {
					json = new JSONObject(msg.getData().getString("data"));
					int quality = json.getInt("quality");
					if (0 <= mQuality && mQuality <= 100) {
						mQuality = quality;
						Log.i(TAG, "Set quality to " + quality);
					}
				} catch (JSONException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}

				break;
			}
			default:
				super.handleMessage(msg);
			}
		}
	});
	
	@Override
	public String getModuleName() {
		return "BmpToJpgModule";
	}

	@Override
	public void defineInMessenger(HashMap<String, Messenger> list) {
		list.put("bmp", mPortBmpInMessenger);
		list.put("command", mPortCommandInMessenger);
	}

	@Override
	public void defineOutMessenger(HashMap<String, Messenger> list) {
		list.put("jpg", null);
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
