package com.clash.of.kings;

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.HashMap;
import java.util.Locale;
import java.util.Map;
import java.util.Vector;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import org.cocos2dx.ext.Native;
import org.hcg.IF.IF;
import org.hcg.stac.empire.common.activity.CommonActivity;
import org.hcg.stac.empire.pay.PayItemData;
import org.hcg.stac.empire.publish.IPublishChannel;
import org.hcg.util.CommonUtil;
import org.hcg.util.Constants;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.content.Intent;
// import android.content.IntentFilter;
// import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.RemoteException;
import android.util.Log;
import android.widget.Toast;

import com.clash.of.util.Purchase;
import com.clash.of.publish.PublishImpl;

//hoolai sdk
 import java.util.HashMap;
 import java.util.Map;
 import com.hoolai.open.fastaccess.channel.FastSdk;
 import com.hoolai.open.fastaccess.proxy.HandlerThreadRunner;

// import com.crashlytics.android.Crashlytics;
// import io.fabric.sdk.android.Fabric;
// import com.crashlytics.android.ndk.CrashlyticsNdk;

public class COK extends CommonActivity {
	private static final String TAG = "Hoolai";

	@Override
	public void onCreate(Bundle savedInstanceState) {

		googleplayEnabled = false;
		super.onCreate(savedInstanceState);
		new SdkInitTask().execute(new Object());
		HandlerThreadRunner glRunner = new HandlerThreadRunner() {

			@Override
			public void runOnThread(Runnable pRunnable) {
				COK.this.runOnGLThread(pRunnable);
			}
		};
		// 带参数初始化
		Map<String, String> parms = new HashMap<String, String>();
		// 如果要用到版本更新功能，需要传入当前应用版本号，不使用版本更新功能不需要此行
		parms.put(FastSdk.CURRENT_VERSION_CODE, "1.1");
		// 这里可以传入游戏自己申请bugly的AppID，没有的话不需要此行
		parms.put(FastSdk.CRASH_REPORT_APP_ID, "");// Bugly APP_ID
		// 这里可以传入游戏自己申请信鸽参数，没有的话不需要下面两行
		parms.put(FastSdk.XG_V2_ACCESS_ID, "");// 推送 ACCESS_ID
		parms.put(FastSdk.XG_V2_ACCESS_KEY, "");// 推送 ACCESS_KEY
		FastSdk.initWithParms(this, glRunner, parms);

		FastSdk.onCreate(this);

		// Fabric.with(this, new Crashlytics(), new CrashlyticsNdk());
//		final SharedPreferences sharedPreferences = this.getSharedPreferences(LocalNotificationManager.CACHE_GAME_UID_KEY, Context.MODE_PRIVATE);
//		String gameUid = sharedPreferences.getString("GAMEUID", "");
//        
//        final SharedPreferences nameSharedPreferences = this.getSharedPreferences("crashCommand", Context.MODE_PRIVATE);
//        String name = nameSharedPreferences.getString("name", "");
//        
//        Log.d("COK UID", gameUid);
//        Crashlytics.setUserIdentifier(gameUid + "_" + name);
	}

	@Override
	public void onStart() {
		super.onStart();
		// hoolai sdk
		try {
			FastSdk.onStart(this);
		} catch (Exception e) {
		}
	}

	@Override
	public void onResume() {
		googleplayEnabled = false;
		super.onResume();
		// hoolai sdk
		try {
			FastSdk.onResume(this);
		} catch (Exception e) {
		}
	}

	@Override
	protected void onRestart() {
		super.onRestart();
		// hoolai sdk
		try {
			FastSdk.onRestart(this);
		} catch (Exception e) {
		}
	}

	@Override
	protected void onPause() {
		super.onPause();
		// hoolai sdk
		try {
			FastSdk.onPause(this);
		} catch (Exception e) {
		}
	}

	@Override
	protected void onStop() {
		super.onStop();
		// hoolai sdk
		try {
			FastSdk.onStop(this);
		} catch (Exception e) {
		}
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		// hoolai sdk
		try {
			FastSdk.onDestroy(this);
		} catch (Exception e) {
		}
	}

	@Override
	public IPublishChannel getPublishImpl() {
		return new PublishImpl();
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		Log.d(TAG, "onActivityResult");
		super.onActivityResult(requestCode, resultCode, data);
		// hoolai sdk
		FastSdk.onActivityResult(this, requestCode, resultCode, data);
	}

}
