package org.dobots.pictureselectmodule;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;

import org.dobots.pictureselectmodule.R;

import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.provider.MediaStore;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.support.v4.view.GestureDetectorCompat;
import android.util.Log;
import android.view.GestureDetector;
import android.view.Menu;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;


public class MainActivity extends Activity {
	private static final String TAG = "MainActivity";
	private static final String MODULE_NAME = "PictureSelectModule";
	private int mId = -1;
	
	TextView mCallbackText;
	EditText mEditText;
	Button mButtonSend;
	Button mButtonImage;
	Button mButtonLogin;
	ImageView mImageView;
	
	
	Messenger mToMsgService = null;
	final Messenger mFromMsgService = new Messenger(new IncomingMsgHandler());
	boolean mMsgServiceIsBound;
	
	Messenger mPortPositionMessenger = null;
	Messenger mPortImageMessenger = null;
	
	private GestureDetectorCompat mGestureDetector;

		// onCreate -> onStart -> onResume
	// onPause -> onResume
	// onPause -> onStop -> onRestart -> onStart -> onResume
	// onPause -> onStop -> onDestroy

	@Override
	public void onCreate(final Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Log.d(TAG,"onCreate");
		setContentView(R.layout.activity_main);
		
		Intent intent = getIntent();
		mId = intent.getIntExtra("id", -1);
		
		mCallbackText = (TextView) findViewById(R.id.messageOutput);
//		mEditText = (EditText) findViewById(R.id.messageInput);
//		mButtonSend = (Button) findViewById(R.id.buttonSend);
		mButtonImage = (Button) findViewById(R.id.buttonImage);
//		mButtonLogin = (Button) findViewById(R.id.buttonLogin);
		mImageView = (ImageView) findViewById(R.id.imageView);
		
//		mEditText.setOnEditorActionListener(new TextView.OnEditorActionListener() {
//		    @Override
//		    public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
//		        if (actionId == R.id.sendMsg || actionId == EditorInfo.IME_ACTION_SEND) {
//		            sendMessage();
//		            return true;
//		        }
//		        return false;
//		    }
//		});
		
		mButtonImage.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				selectImage();
			}
		});
		
		mGestureDetector = new GestureDetectorCompat(this, new MyGestureListener());
		
		doBindService();
	}
	
	@Override
	public void onStart() {
		super.onStart();
		Log.d(TAG,"onStart");
	}

	@Override
	public void onResume() {
		super.onResume();
		Log.d(TAG,"onResume");
	}

	@Override
	public void onPause() {
		super.onPause();
		Log.d(TAG,"onPause");
	}

	@Override
	public void onStop() {
		super.onStop();
		Log.d(TAG,"onStop");
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		Log.d(TAG, "onDestroy " + mMsgServiceIsBound);
		doUnbindService();
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	private ServiceConnection mMsgServiceConnection = new ServiceConnection() {
		public void onServiceConnected(ComponentName className, IBinder service) {
			// This is called when the connection with the service has been established, giving us the service object
			// we can use to interact with the service.  We are communicating with our service through an IDL 
			// interface, so get a client-side representation of that from the raw service object.
			mToMsgService = new Messenger(service);
			mCallbackText.setText("Connected to Dodedodo.");

			Message msg = Message.obtain(null, AimProtocol.MSG_REGISTER);
			Bundle bundle = new Bundle();
			bundle.putString("package", getPackageName());
			bundle.putString("module", MODULE_NAME);
			bundle.putInt("id", mId);
			msg.setData(bundle);
			msgSend(msg);
			
//	        Toast.makeText(Binding.this, R.string.remote_service_connected, Toast.LENGTH_SHORT).show();
			Log.i(TAG, "Connected to MsgService: " + mToMsgService.toString());
		}

		public void onServiceDisconnected(ComponentName className) {
			// This is called when the connection with the service has been unexpectedly disconnected: its process crashed.
			mToMsgService = null;
			mCallbackText.setText("Disconnected from Dodedodo.");

//	        Toast.makeText(Binding.this, R.string.remote_service_disconnected, Toast.LENGTH_SHORT).show();
			Log.i(TAG, "Disconnected from MsgService");
		}
	};

	// Handle messages from MsgService
	class IncomingMsgHandler extends Handler {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case AimProtocol.MSG_SET_MESSENGER:
				Log.i(TAG, "set port: " + msg.getData().getString("port") + " to: " + msg.replyTo.toString());
				if (msg.getData().getString("port").equals("position"))
					mPortPositionMessenger = msg.replyTo;
				if (msg.getData().getString("port").equals("image"))
					mPortImageMessenger = msg.replyTo;
				break;
			case AimProtocol.MSG_STOP:
				Log.i(TAG, "stopping");
				finish();
				break;
			default:
				super.handleMessage(msg);
			}
		}
	}
	
	void doBindService() {
		// Establish a connection with the service.  We use an explicit class name because there is no reason to be 
		// able to let other applications replace our component.
		//bindService(new Intent(this, XMPPService.class), mConnection, Context.BIND_AUTO_CREATE);
		
		Intent intent = new Intent();
		intent.setClassName("org.dobots.dodedodo", "org.dobots.dodedodo.MsgService");
		bindService(intent, mMsgServiceConnection, Context.BIND_AUTO_CREATE);
		mMsgServiceIsBound = true;
		mCallbackText.setText("Binding to service.");
	}

	void doUnbindService() {
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
			mCallbackText.setText("Unbinding from service.");
		}
	}

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
	
	protected void msgSend(Messenger messenger, Message msg) {
		if (messenger == null)
			return;
		try {
//			msg.replyTo = mFromMsgService;
			messenger.send(msg);
		} catch (RemoteException e) {
			Log.i(TAG, "failed to send msg. " + e);
			// There is nothing special we need to do if the service has crashed.
		}
	}

	
	////////////////////////////////////////////////////////////
	//                     Select image                       //
	////////////////////////////////////////////////////////////
	public void selectImage() {
		Intent intent = new Intent();
		intent.setType("image/*");
		intent.setAction(Intent.ACTION_GET_CONTENT);
		startActivityForResult(Intent.createChooser(intent, "Select Picture"), PICK_IMAGE);
	}
	
	private static final int PICK_IMAGE = 1;
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		if(requestCode == PICK_IMAGE && data != null && data.getData() != null) {
			Uri selectedImage = data.getData();
			String[] columnFilter = {MediaStore.Images.Media.DATA, MediaStore.Images.Media.ORIENTATION};
//			String orientationColumn = MediaStore.Images.Media.ORIENTATION

			Cursor cursor = getContentResolver().query(selectedImage, columnFilter, null, null, null);
			if (!cursor.moveToFirst())
				return;

			int columnIndex = cursor.getColumnIndex(columnFilter[0]);
			if (columnIndex < 0)
				return;
			final String filePath = cursor.getString(columnIndex);
			
			columnIndex = cursor.getColumnIndex(columnFilter[1]);
			if (columnIndex < 0)
				return;
			// The orientation for the image expressed as degrees. Only degrees 0, 90, 180, 270 will work. 
			final int rotation = cursor.getInt(columnIndex);
			
			cursor.close();
			
			File file = new File(filePath);
			Bitmap scaledImage = decodeFile(file, 100*100); //  150*200 is possible for the messenger, but xmpp can't handle it
			
			mImageView.setImageBitmap(scaledImage);

			int[] pixels = new int[scaledImage.getWidth() * scaledImage.getHeight()];
			scaledImage.getPixels(pixels, 0, scaledImage.getWidth(), 0, 0, scaledImage.getWidth(), scaledImage.getHeight());
			
			int width, height;
			width = scaledImage.getWidth();
			height = scaledImage.getHeight();
			
			int[] msgData = new int[width * height * 3 + 4];
			msgData[0] = 3; // Ndims
			msgData[1] = height; // size dim 1
			msgData[2] = width; // size dim 2
			msgData[3] = 3; // size dim 3 (rgb)
			
			Log.d(TAG, "height=" + height + "width=" + width);
//			if (scaledImage.getConfig().equals(Bitmap.Config.RGB_565)) {
//				for (int i=0, j=4; i<pixels.length; ++i, j+=3) {
//					msgData[j] = ((pixels[i] >> 11) & 0x1F)*8; // 5 bits to 8 bits
//					msgData[j+1] = ((pixels[i] >> 5) & 0x3F)*4; // 6 bits to 8 bits
//					msgData[j+2] = (pixels[i] & 0x1F)*8; // 5 bits to 8 bits
//					//Log.i(TAG, "j=" + j + " r=" + msgData[j] + " g=" + msgData[j+1] + " b=" + msgData[j+2]);
//					Log.i(TAG, msgData[j] + " " + msgData[j+1] + " " + msgData[j+2]);
//				}
//			}	
			
			// GetPixels returns ARGB_8888
			for (int i=0, j=4; i<pixels.length; ++i, j+=3) {
				msgData[j] = (pixels[i] >> 16) & 0xFF;
				msgData[j+1] = (pixels[i] >> 8) & 0xFF;
				msgData[j+2] = pixels[i] & 0xFF;
			}
			
			Message msg = Message.obtain(null, AimProtocol.MSG_PORT_DATA);
			Bundle bundle = new Bundle();
			bundle.putIntArray("data", msgData);
			bundle.putInt("datatype", AimProtocol.DATATYPE_INT_ARRAY);
			msg.setData(bundle);
			msgSend(mPortImageMessenger, msg);

			Log.d(TAG, "image: " + filePath + " rotation:" + rotation + " to port: " + mPortImageMessenger);
		}
		super.onActivityResult(requestCode, resultCode, data);
	}
	
	//decodes image and scales it to reduce memory consumption
//	private Bitmap decodeFile(File f, int maxWidth, int maxHeight){
	private Bitmap decodeFile(File f, int maxPixels) {
	    try {
	        //Decode image size
	        BitmapFactory.Options o = new BitmapFactory.Options();
	        o.inJustDecodeBounds = true;
	        BitmapFactory.decodeStream(new FileInputStream(f),null,o);

	        //Find the correct scale value. It should be the power of 2.
	        int scale=1;
	        while (o.outWidth/scale * o.outHeight/scale >= maxPixels)
	            scale*=2;

	        //Decode with inSampleSize
	        BitmapFactory.Options o2 = new BitmapFactory.Options();
	        o2.inSampleSize=scale;
	        return BitmapFactory.decodeStream(new FileInputStream(f), null, o2);
	    } catch (FileNotFoundException e) {}
	    return null;
	}
	
	
	////////////////////////////////////////////////////////////
	//                       Gestures                         //
	////////////////////////////////////////////////////////////
    @Override 
    public boolean onTouchEvent(MotionEvent event){ 
        this.mGestureDetector.onTouchEvent(event);
        return super.onTouchEvent(event);
    }

	class MyGestureListener extends GestureDetector.SimpleOnGestureListener {
		@Override
		public boolean onDown(MotionEvent event) {
			return true;
		}
		
		@Override
		public boolean onSingleTapUp(MotionEvent event) {
//			PointerCoords coords = new PointerCoords();
//			event.getPointerCoords(0, coords);
//			Log.i(TAG, coords.x + " " + coords.y);
			Log.d(TAG, event.getX() + " " + event.getY());
//			Log.i(TAG, event.toString());
			
			Message msg = Message.obtain(null, AimProtocol.MSG_PORT_DATA);
			Bundle bundle = new Bundle();
			float[] data = new float[4];
			data[0] = 1;
			data[1] = 2;
			data[2] = event.getX();
			data[3] = event.getY();
			bundle.putFloatArray("data", data);
			bundle.putInt("datatype", AimProtocol.DATATYPE_FLOAT_ARRAY);
			msg.setData(bundle);
			msgSend(mPortPositionMessenger, msg);
			
			return true;
		}
	}
}
