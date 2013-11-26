package org.dobots.bmptojpgmodule.aim;

import java.io.ByteArrayOutputStream;

import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;

import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;
//import android.util.Base64;

public class BmpToJpgModuleService extends Service {
	private static final String TAG = "BmpToJpgModuleService";
	private static final String MODULE_NAME = "BmpToJpgModule";
	int mId = -1;
	Messenger mToMsgService = null;
	final Messenger mFromMsgService = new Messenger(new IncomingMsgHandler());
	boolean mMsgServiceIsBound;

	Messenger mPortBmpInMessenger = new Messenger(new PortBmpMessengerHandler());
	Messenger mPortJpgOutMessenger = null;

	class PortBmpMessengerHandler extends Handler {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case AimProtocol.MSG_PORT_DATA: {
				int[] readVal = msg.getData().getIntArray("data");
				int nDims = readVal[0];
				if (nDims != 3)
					break;
				int height = readVal[1];
				int width = readVal[2];
				int channels = readVal[3];
				
				int[] pixels = new int[height*width];
//				for (int x=0; x<width; x++) {
//					for (int y=0; y<height; y++) {
//						for (int c=0; c<channels; c++) {						
//						}
//					}
//				}
				
				Log.d(TAG,  "height=" + height + " width=" + width + " channels=" + channels);
				for (int i=0, j=0; i < height*width*channels; i+=3, j++) {
					int r = readVal[4+i+0]; int g = readVal[4+i+1]; int b = readVal[4+i+2];
//					Log.i(TAG, "i=" + i + " r=" + r + " g=" + g + " b=" + b);
					pixels[j] = 0xFF000000 | (r & 0xFF) << 16 | (g & 0xFF) << 8 | (b & 0xFF);
				}

				Bitmap bmp = Bitmap.createBitmap(pixels, width, height, Bitmap.Config.ARGB_8888);
				
				// Convert to jpeg
				ByteArrayOutputStream os = new ByteArrayOutputStream();
				bmp.compress(CompressFormat.JPEG, 100, os);
				byte[] bytes = os.toByteArray();
				
				// Send message
				String msgData = android.util.Base64.encodeToString(bytes, android.util.Base64.NO_WRAP);
				Message msgOut = Message.obtain(null, AimProtocol.MSG_PORT_DATA);
				Bundle bundle = new Bundle();
				bundle.putInt("datatype", AimProtocol.DATATYPE_STRING);
				bundle.putString("data", msgData);
				msgOut.setData(bundle);
				msgSend(mPortJpgOutMessenger, msgOut);

/*
				// Convert to int array
				int[] msgData = new int[bytes.length + 2];
				msgData[0] = 1; // Ndims
				msgData[1] = bytes.length;
				for (int i=0; i<bytes.length; ++i) {
					msgData[i+2] = bytes[i] & 0xFF;
				}
				
				// Send message
//				Log.i(TAG, "outputJpg=" + outputJpg.getVal().toString() + " ");
				Message msgOut = Message.obtain(null, AimProtocol.MSG_PORT_DATA);
				Bundle bundle = new Bundle();
				bundle.putInt("datatype", AimProtocol.DATATYPE_INT_ARRAY);
				bundle.putIntArray("data", msgData);
				msgOut.setData(bundle);
				msgSend(mPortJpgOutMessenger, msgOut);
*/
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
			bundle.putString("module", MODULE_NAME);
			bundle.putInt("id", mId);
			msg.setData(bundle);
			msgSend(msg);
			{
				Message msgPort = Message.obtain(null, AimProtocol.MSG_SET_MESSENGER);
				msgPort.replyTo = mPortBmpInMessenger;
				Bundle bundlePort = new Bundle();
				bundlePort.putString("module", MODULE_NAME);
				bundlePort.putInt("id", mId);
				bundlePort.putString("port", "bmp");
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
				if (msg.getData().getString("port").equals("jpg"))
					mPortJpgOutMessenger = msg.replyTo;
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