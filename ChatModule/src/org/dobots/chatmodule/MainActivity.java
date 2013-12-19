package org.dobots.chatmodule;

import java.util.HashMap;

import org.dobots.aim.AimActivity;
import org.dobots.aim.AimProtocol;
import org.dobots.aim.AimProtocol.AimDataTypeException;
import org.dobots.aim.AimUtils;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.Messenger;
import android.app.Activity;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.inputmethod.EditorInfo;
import android.widget.TextView;
import android.widget.EditText;
import android.widget.TextView.OnEditorActionListener;

public class MainActivity extends AimActivity {
	private final String TAG = "Main";

	TextView mTextViewChatOut;
	TextView mTextViewChatIn;
	EditText mEditTextChat;
	
	private Messenger mChatInMessenger = new Messenger(new Handler() {

		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case AimProtocol.MSG_PORT_DATA:
				// handle chat data
				try {
					handleChatIn(AimUtils.getStringData(msg));
				} catch (AimDataTypeException e) {
					e.printStackTrace();
				}
				break;
			default:
				super.handleMessage(msg);
			}
		}
	});
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		mTextViewChatOut = (TextView) findViewById(R.id.textChatOut);
		mTextViewChatIn = (TextView) findViewById(R.id.textChatIn);
		mEditTextChat = (EditText) findViewById(R.id.editChat);
		
		mTextViewChatOut.setText("");
		
		mEditTextChat.setOnEditorActionListener(new OnEditorActionListener() {
			@Override
			public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
				boolean handled = false;
				if (actionId == EditorInfo.IME_ACTION_SEND) {
					sendChat();
					handled = true;
				}
				return handled;
			}
		});
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
//		getMenuInflater().inflate(R.menu.main, menu);
//		return true;
		return false;
	}

	@Override
	public String getModuleName() {
		return "ChatModule";
	}

	@Override
	public void defineInMessenger(HashMap<String, Messenger> list) {
		list.put("textin", mChatInMessenger);
	}

	@Override
	public void defineOutMessenger(HashMap<String, Messenger> list) {
		list.put("textout", null);
	}
	
	public void handleChatIn(String text) {
		Log.i(TAG, "Received: " + text);
		mTextViewChatIn.setText(text);
	}
	
	public void sendChat() {
		Log.i(TAG, "Sending: " + mEditTextChat.getText().toString());
		mTextViewChatOut.setText(mEditTextChat.getText());
		sendData(getOutMessenger("textout"), mEditTextChat.getText().toString());
		mEditTextChat.setText("");
	}

}
