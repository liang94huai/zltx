package com.google.android.apps.analytics;

import org.cocos2dx.ext.Tracker;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.util.Log;

public class MyAnalyticsReceiver extends BroadcastReceiver {

	static final String INSTALL_ACTION = "com.android.vending.INSTALL_REFERRER";
	private static final String REFERRER_KEY = "referrer";

	@Override
	public void onReceive(Context context, Intent intent) {
		Log.d("COK MyAnalyticsReceiver", "onReceive context=" + context);
		try {
			saveAdRefferToLocal(context, intent);
		} catch (Exception e) {
			Tracker.reportException(context, e);
		}
	}

	private void saveAdRefferToLocal(Context context, Intent intent) throws Exception {
		if (!INSTALL_ACTION.equals(intent.getAction()))
			throw new Exception("wrong intent " + intent.getAction());
		// 接到安装通知
		String referrer = intent.getStringExtra(REFERRER_KEY);
		if ((!INSTALL_ACTION.equals(intent.getAction())) || (referrer == null))
			return;
		// 保存安装通知到本地
		saveAdReferrer(context, referrer);
    }

	public static final String AD_INFO = "AdReferrerStore"; // 广告信息
	public static final String AD_REF = "referrer";

	private static void saveAdReferrer(Context context, String referrer) {
		SharedPreferences sharedPreferences = context.getSharedPreferences(
				AD_INFO, Context.MODE_PRIVATE);
		Editor editor = sharedPreferences.edit();
		editor.putString(AD_REF, referrer);
		editor.commit();
	}

	public static String getAdReferrer(Context context) {
		SharedPreferences sharedPreferences = context.getSharedPreferences(
				AD_INFO, Context.MODE_PRIVATE);
		return sharedPreferences.getString(AD_REF, null);
	}
}
