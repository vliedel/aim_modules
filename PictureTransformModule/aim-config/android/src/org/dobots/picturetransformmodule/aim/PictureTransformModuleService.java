package org.dobots.picturetransformmodule.aim;

import java.util.ArrayList;
import java.util.List;

import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;

public class PictureTransformModuleService extends Service {
  private static final String TAG = "PictureTransformModuleService";
  private static final String MODULE_NAME = "PictureTransformModule";
  private int mId = -1;
  Messenger mToMsgService = null;
  final Messenger mFromMsgService = new Messenger(new IncomingMsgHandler());
  boolean mMsgServiceIsBound;
  
  Messenger mPortInImageInMessenger = new Messenger(new PortInImageMessengerHandler());
  private List<vector_int> mPortInImageInBuffer = new ArrayList<vector_int>(0);
  Messenger mPortOutImageOutMessenger = null;
  
  class PortInImageMessengerHandler extends Handler {
    @Override
    public void handleMessage(Message msg) {
      switch (msg.what) {
      case AimProtocol.MSG_PORT_DATA:
        int[] readVal = msg.getData().getIntArray("data");
        vector_int bufVal = new vector_int(readVal.length);
        
        // Debug
        StringBuffer str = new StringBuffer("Read msg: ");
        for (int i=0; i<readVal.length; i++) {
          bufVal.set(i, readVal[i]);
          str.append(readVal[i]);
          str.append(" ");
        }
        Log.d(TAG, str.toString());
        
        synchronized(mPortInImageInBuffer) {
          mPortInImageInBuffer.add(bufVal);
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
        msgPort.replyTo = mPortInImageInMessenger;
        Bundle bundlePort = new Bundle();
        bundlePort.putString("package", getPackageName());
        bundlePort.putString("module", MODULE_NAME);
        bundlePort.putInt("id", mId);
        bundlePort.putString("port", "inimage");
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
        if (msg.getData().getString("port").equals("outimage"))
          mPortOutImageOutMessenger = msg.replyTo;
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
  

	AIMRun mAIMRun = null;
	
	public void start() {
		if (mId < 0)
			return;
		if (!mMsgServiceIsBound)
			bindToMsgService();
		if (mAIMRun == null) {
			mAIMRun = new AIMRun();
			mAIMRun.execute(mId);
		}
	}
	
	public void onCreate() {
		// On create mId is unknown!
	}
	
	public void onDestroy() {
		super.onDestroy();
		mAIMRun.cancel(true);
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
		mId = intent.getIntExtra("id", 0);
		Log.d(TAG, "onStart " + mId);
		start();
	}
	
	// Called each time a client uses startService()
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
//		handleStartCommand(intent);
		// We want this service to continue running until it is explicitly stopped, so return sticky.
		mId = intent.getIntExtra("id", 0);
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
			Log.i(TAG, "failed to send msg. " + e);
			// There is nothing special we need to do if the service has crashed.
		}
	}
	

  // AsyncTask<Params, Progress, Result>
  private class AIMRun extends AsyncTask<Integer, Void, Boolean> {
    protected Boolean doInBackground(Integer... id) {
      Log.i(TAG, "Starting AIMRun " + id);
      PictureTransformModuleExt aim = new PictureTransformModuleExt();
      //aim.Init("0"); // TODO: pass on the id
      AndroidOutImageRead_t outputOutImage;
      
      while (true) {
        synchronized(mPortInImageInBuffer) {
          if (!mPortInImageInBuffer.isEmpty()) {
            aim.androidWriteInImage(mPortInImageInBuffer.get(0));
            mPortInImageInBuffer.remove(0);
          }
        }
        
        aim.Tick();
        outputOutImage = aim.androidReadOutImage();
        if (outputOutImage.getSuccess()) {
          
          // Debug
          StringBuffer str = new StringBuffer();
          str.append("outputOutImage=");
          str.append(outputOutImage.getVal().toString());
          str.append(" ");
          int[] data = new int[(int) outputOutImage.getVal().size()];
          for (int i=0; i<outputOutImage.getVal().size(); i++) {
            data[i] = outputOutImage.getVal().get(i);
            str.append(outputOutImage.getVal().get(i));
            str.append(" ");
          }
          Log.d(TAG, str.toString());
          
          Message msg = Message.obtain(null, AimProtocol.MSG_PORT_DATA);
          Bundle bundle = new Bundle();
          bundle.putInt("datatype", AimProtocol.DATATYPE_INT_ARRAY);
          bundle.putIntArray("data", data);
          msg.setData(bundle);
          msgSend(mPortOutImageOutMessenger, msg);
        }
        
        if (isCancelled()) break;
      }
      return true;
    }
    
    protected void onCancelled() {
      Log.i(TAG, "Stopped AIMRun");
    }
  }
  
  // static constructor
  static {
    System.loadLibrary("PictureTransformModule");
  }
}
