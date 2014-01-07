package org.dobots.jpgtobmpmodule;

import org.dobots.jpgtobmpmodule.aim.AimProtocol;
import java.io.ByteArrayOutputStream;

import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.BitmapFactory;

import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;
//import android.util.Base64;

public class JpgToBmpModuleService extends Service {
	private static final String TAG = "JpgToBmpModuleService";
	public static final String MODULE_NAME = "JpgToBmpModule";
	int mId = -1;
	Messenger mToMsgService = null;
	final Messenger mFromMsgService = new Messenger(new IncomingMsgHandler());
	boolean mMsgServiceIsBound;

	Messenger mPortJpgInMessenger = new Messenger(new PortJpgMessengerHandler());
	Messenger mPortBmpOutMessenger = null;

	class PortJpgMessengerHandler extends Handler {
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
				
				Message msgBmp = Message.obtain(null, AimProtocol.MSG_PORT_DATA);
				Bundle bundle = new Bundle();
				bundle.putIntArray("data", msgData);
				bundle.putInt("datatype", AimProtocol.DATATYPE_INT_ARRAY);
				msgBmp.setData(bundle);
				msgSend(mPortBmpOutMessenger, msg);

				break;
			}
			default:
				super.handleMessage(msg);
			}
		}
	}

	private ServiceConnection mMsgServiceConnection = new ServiceConnection() {
		public void onServiceConnected(ComponentName className, IBinder service) {
			// This is called when the connection with the service has been established, giving us the service object
			// we can use to interact with the service.  We are communicating with our service through an IDL
			// interface, so get a client-side representation of that from the raw service object.
			mToMsgService = new Messenger(service);
			Message msg = Message.obtain(null, AimProtocol.MSG_REGISTER);
			Bundle bundle = new Bundle();
			bundle.putString("package", getPackageName());
			bundle.putString("module", MODULE_NAME);
			bundle.putInt("id", mId);
			msg.setData(bundle);
			msgSend(msg);
			{
				Message msgPort = Message.obtain(null, AimProtocol.MSG_SET_MESSENGER);
				msgPort.replyTo = mPortJpgInMessenger;
				Bundle bundlePort = new Bundle();
				bundlePort.putString("package", getPackageName());
				bundlePort.putString("module", MODULE_NAME);
				bundlePort.putInt("id", mId);
				bundlePort.putString("port", "jpg");
				msgPort.setData(bundlePort);
				msgSend(mToMsgService, msgPort);
			}


			Log.i(TAG, "Connected to MsgService: " + mToMsgService.toString());
		}
		public void onServiceDisconnected(ComponentName className) {
			// This is called when the connection with the service has been unexpectedly disconnected: its process crashed.
			mToMsgService = null;
			Log.i(TAG, "Disconnected from MsgService");
		}
	};

	// Handle messages from MsgService
	class IncomingMsgHandler extends Handler {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case AimProtocol.MSG_SET_MESSENGER:
				if (msg.getData().getString("port").equals("bmp"))
					mPortBmpOutMessenger = msg.replyTo;
				break;
			case AimProtocol.MSG_STOP:
				Log.i(TAG, "stopping");
				stopSelf();
				break;
			default:
				super.handleMessage(msg);
			}
		}
	}

	public void start() {
		if (mId < 0)
			return;
		if (!mMsgServiceIsBound)
			bindToMsgService();
	}

	public void onCreate() {
		// mId is not set yet on create
		//bindToMsgService();
	}

	public void onDestroy() {
		super.onDestroy();
		unbindFromMsgService();
		Log.d(TAG, "onDestroy");
	}

	@Override
	public IBinder onBind(final Intent intent) {
		Log.i(TAG,"onBind: " + intent.toString());
		return null; // No binding provided
		//		return mFromMsgService.getBinder();
	}

	// Called when all clients have disconnected from a particular interface of this service.
	@Override
	public boolean onUnbind(final Intent intent) {
		return super.onUnbind(intent);
	}

	// Deprecated since API level 5 (android 2.0)
	@Override
	public void onStart(Intent intent, int startId) {
		//		handleStartCommand(intent);
		mId = intent.getIntExtra("id", -1);
		Log.d(TAG, "onStart " + mId);
		start();
	}

	// Called each time a client uses startService()
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		//	    handleStartCommand(intent);
		// We want this service to continue running until it is explicitly stopped, so return sticky.
		mId = intent.getIntExtra("id", -1);
		Log.d(TAG, "onStartCommand " + mId);
		start();
		return START_STICKY;
	}

	void bindToMsgService() {
		// Establish a connection with the service.  We use an explicit class name because there is no reason to be 
		// able to let other applications replace our component.
		Intent intent = new Intent();
		intent.setClassName("org.dobots.dodedodo", "org.dobots.dodedodo.MsgService");
		bindService(intent, mMsgServiceConnection, Context.BIND_AUTO_CREATE);
		mMsgServiceIsBound = true;
		Log.i(TAG, "Binding to msgService");
	}

	void unbindFromMsgService() {
		if (mMsgServiceIsBound) {
			// If we have received the service, and registered with it, then now is the time to unregister.
			if (mToMsgService != null) {
				Message msg = Message.obtain(null, AimProtocol.MSG_UNREGISTER);
				Bundle bundle = new Bundle();
				bundle.putString("module", MODULE_NAME);
				bundle.putInt("id", mId);
				msg.setData(bundle);
				msgSend(msg);
			}
			// Detach our existing connection.
			unbindService(mMsgServiceConnection);
			mMsgServiceIsBound = false;
			Log.i(TAG, "Unbinding from msgService");
		}
	}

	// Send a msg to the msgService
	protected void msgSend(Message msg) {
		if (!mMsgServiceIsBound) {
			Log.i(TAG, "Can't send message to service: not bound");
			return;
		}
		try {
			msg.replyTo = mFromMsgService;
			mToMsgService.send(msg);
		} catch (RemoteException e) {
			Log.i(TAG, "Failed to send msg to service. " + e);
			// There is nothing special we need to do if the service has crashed.
		}
	}

	// Send a msg to some messenger
	protected void msgSend(Messenger messenger, Message msg) {
		if (messenger == null || msg == null)
			return;
		try {
			//msg.replyTo = mFromMsgService;
			messenger.send(msg);
		} catch (RemoteException e) {
			Log.i(TAG, "failed to send msg to service. " + e);
			// There is nothing special we need to do if the service has crashed.
		}
	}
}
