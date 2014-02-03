package org.dobots.pictureselectmodule;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.HashMap;

import org.dobots.pictureselectmodule.R;

import android.net.Uri;
import android.os.Bundle;
import android.os.Messenger;
import android.provider.MediaStore;
import android.content.Intent;
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

import org.dobots.aim.AimActivity;
import org.dobots.aim.AimProtocol;


public class MainActivity extends AimActivity {
	private static final String TAG = "MainActivity";
	
	TextView mCallbackText;
	EditText mEditText;
	Button mButtonSend;
	Button mButtonImage;
	Button mButtonLogin;
	ImageView mImageView;
	
	@Override
	public String getModuleName() {
		return "PictureSelectModule";
	}

	@Override
	public void defineInMessenger(HashMap<String, Messenger> list) {
	}

	@Override
	public void defineOutMessenger(HashMap<String, Messenger> list) {
		list.put("image", null);
		list.put("position", null);
	}
	
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
	}
	
	@Override
	public void onStart() {
		super.onStart();
	}

	@Override
	public void onResume() {
		super.onResume();
		mCallbackText.setText("");
	}

	@Override
	public void onPause() {
		super.onPause();
	}

	@Override
	public void onStop() {
		super.onStop();
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
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
			Bitmap scaledImage = decodeFile(file, 50*1000);
			if (scaledImage == null) {
				Log.e(TAG, "error: could not open image: " + file);
				return;
			}
			mImageView.setImageBitmap(scaledImage);

			int[] pixels = new int[scaledImage.getWidth() * scaledImage.getHeight()];
			scaledImage.getPixels(pixels, 0, scaledImage.getWidth(), 0, 0, scaledImage.getWidth(), scaledImage.getHeight());
			
			int width, height;
			width = scaledImage.getWidth();
			height = scaledImage.getHeight();
			
			int[] msgData = new int[width * height * 3 + 6];
			msgData[0] = 0; // datatype
			msgData[1] = 1; // Narrays
			msgData[2] = 3; // Ndims
			msgData[3] = height; // size dim 1
			msgData[4] = width; // size dim 2
			msgData[5] = 3; // size dim 3 (rgb)
			
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
			for (int i=0, j=6; i<pixels.length; ++i, j+=3) {
				msgData[j] = (pixels[i] >> 16) & 0xFF;
				msgData[j+1] = (pixels[i] >> 8) & 0xFF;
				msgData[j+2] = pixels[i] & 0xFF;
			}
			
			Bundle bundle = new Bundle();
			bundle.putInt("datatype", AimProtocol.DATATYPE_INT_ARRAY);
			bundle.putIntArray("data", msgData);
			sendData(getOutMessenger("image"), bundle);
			Log.d(TAG, "image: " + filePath + " rotation:" + rotation + " to port: " + getOutMessenger("image"));
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
			
//			float[] data = new float[4];
//			data[0] = 1;
//			data[1] = 2;
//			data[2] = event.getX();
//			data[3] = event.getY();
			float[] data = new float[2];
			data[0] = event.getX();
			data[1] = event.getY();

			sendData(getOutMessenger("position"), data);
			return true;
		}
	}
}
