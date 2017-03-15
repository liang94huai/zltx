package org.hcg.IF;

import java.util.Calendar;
import java.util.Date;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.ScheduledFuture;
import java.util.concurrent.TimeUnit;

import org.cocos2dx.ext.Native;
import org.hcg.notifies.LocalNotification;
import org.hcg.notifies.LocalNotificationManager;
import org.hcg.util.AOEUtil;
import org.hcg.util.AOEUtil.MD5;
import org.hcg.util.GameContext;
import org.json.JSONObject;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.util.Log;

public class Jni {
	public static native String nativeGetAppId();
	private static int notifyCount = 0;
	private static LocalNotificationManager manager = null;
	private static LocalNotificationManager getManager(){
		if(manager == null){
			manager = new LocalNotificationManager(GameContext.getActivityInstance());
		}
		return manager;
	}
	
	public static void cancelNotification(int type){
		if(type == 987 && disconnectFuture != null){
			disconnectFuture.cancel(false);
			return;
		}
		cancel(Integer.toString(type));	
	}
	
	private static ScheduledExecutorService disconnectService = Executors.newSingleThreadScheduledExecutor();
	private static ScheduledFuture<?> disconnectFuture;
	public static void pushNotification(int type,int time, String text, String soundKey, String pushType){
		if(type == 987 && (disconnectFuture == null || disconnectFuture.isCancelled() || disconnectFuture.isDone())){
			disconnectFuture = disconnectService.schedule(new Runnable() {
				@Override
				public void run() {
					Log.d("TimerTask", "Net disconnect");
					//SFS disconnect
					Net.logout();
					Net.disconnect();					
				}
			} , 300000, TimeUnit.MILLISECONDS);
			return;
		}
		sendNotify(text,text,time,Integer.toString(type),soundKey,pushType);

	}


	public static void sendNotify(String title,String body,int seconds,String identify,String soundKey,String pushType){
		Object actionParam = null;
		++notifyCount;
		Log.d("LocalNotify", "send notify after " + seconds + " seconds with body: " + body);
//		seconds = adjustTime(seconds);
		Log.d("LocalNotify", "local notification seconds after adjust: " + seconds);
		LocalNotification notification = new LocalNotification(GameContext.getActivityInstance().getClass().getName());
		notification.code = identify;
		// 设定时间，我们想要在data.getTime()秒之后进行通知
		Calendar calendar = Calendar.getInstance();
		calendar.setTimeInMillis(System.currentTimeMillis());
		calendar.add(Calendar.SECOND, seconds);

		notification.fireDate = new Date(System.currentTimeMillis()+1000*seconds);
		notification.repeatInterval = 0;
		notification.body = body;
		notification.pushType = pushType;
		notification.title = IF.getContext().getString(R.string.app_name);
		notification.numberAnnotation = notifyCount;
		notification.iconResourceId = R.drawable.notification_icon;
		notification.soundKey = soundKey;
		notification.vibrate = true;
		
		if(actionParam == null){
			notification.actionData = new byte[0];
		}else{
//			TODO: support for actionParam
//			notification.actionData = actionParam;
			notification.actionData = new byte[0];
		}
		
		getManager().notify( notification);
		getManager().persistNotification(notification);
	}
	
	public static String getPushRecordData(){
		final SharedPreferences pushRecordSharedPreferences = GameContext.getActivityInstance().getSharedPreferences(LocalNotificationManager.CACHE_NOTIF_RECORD_KEY, Context.MODE_PRIVATE);
		Log.d("COK", "COK getPushRecordData"+pushRecordSharedPreferences.getString(LocalNotificationManager.PUSH_RECORD, ""));
		return pushRecordSharedPreferences.getString(LocalNotificationManager.PUSH_RECORD, "");
	}
	public static String getPushClickData(){
		final SharedPreferences pushRecordSharedPreferences = GameContext.getActivityInstance().getSharedPreferences(LocalNotificationManager.CACHE_NOTIF_RECORD_KEY, Context.MODE_PRIVATE);
		Log.d("COK", "COK getPushClickData"+pushRecordSharedPreferences.getString(LocalNotificationManager.PUSH_CLICK_DATA, ""));
		return pushRecordSharedPreferences.getString(LocalNotificationManager.PUSH_CLICK_DATA, "");
	}
	public static void clearPushCache(){
		final SharedPreferences pushRecordSharedPreferences = GameContext.getActivityInstance().getSharedPreferences(LocalNotificationManager.CACHE_NOTIF_RECORD_KEY, Context.MODE_PRIVATE);
		Editor pushRecordEditor = pushRecordSharedPreferences.edit();
		pushRecordEditor.clear();
		pushRecordEditor.commit();
	}
	

	private static int lastSeconds = 0;
	private static int adjustTime(int seconds)
	{
		
		if(lastSeconds > 0 && (seconds - lastSeconds) < 3600){
			seconds = seconds + 3600;
		}
		
		if(seconds < 1200){
			Log.d("LocalNotify","seconds less then 1200, reset to 1200");
			seconds = 1200;
		}
		
		lastSeconds = seconds;
		Date date = new Date();
		long tmpTime = date.getTime() + seconds*1000;
		date.setTime(tmpTime);
		int h = date.getHours();
		Log.d("LocalNotify", "adjust time for hours: " + h + " date:" + date);
		int ret = seconds;
		if(h<9 || h>=22)
		{
			if(h<9)
			{
				ret += (9-h) * 3600;
			}
			else
			{
				ret += (24+9-h)* 3600;
			}
		}
		return ret;
	}
	
	public static void cancel(String identify){
		if("-1".equals(identify)){
			getManager().cancelAll();
			getManager().unpersistAllNotifications();
		}else{
			getManager().cancel(identify);
			getManager().unpersistNotification(identify);
		}
	}
	
	public static void cancelAll(){
		Log.d("LocalNotify", "start cancel all");
		getManager().cancelAll();
		getManager().unpersistAllNotifications();
		getManager().clearCurrentNotificationContent();
		Log.d("LocalNotify", "end cancel all");
	}
	
	public static String getMD5String(String str){
		return MD5.getMD5Str(str);
	}
	
	public static void recordCmd(String cmd){
		System.out.print("setCmd " + cmd);
		IF.getInstance().recordCmd(cmd);
	}
	
	public static void showNewsView(String url) {
		//showWebView(url, R.string.news_title);
	}

	public static void showContactUsView(String url) {
		//showWebView(url, R.string.btn_email_us);
	}
	
	//clipBoard
	
	public static String clipboardGetText() {
		try {
			return IF.getInstance().clipboard.getText().toString();
		} catch (Exception e) {
//			reportException(Main.getInstance(), e);
		}
		return "";
	}
	
	public static void clipboardSetText(final String text) {
		IF.getInstance().runOnUiThread(new Runnable() {
			public void run() {
				try {
					IF.getInstance().clipboard.setText(text);
				} catch (Exception e) {
//					reportException(Main.getInstance(), e);
				}
			}
		});
	}
	
	public static boolean isNetworkAvailable()
	{
		return AOEUtil.isNetworkAvailable(IF.getInstance());
	}
	
	public static void finishGame() {
		IF.getInstance().finishGame();
	}	
	
	public static void setTestinUser(String _user) {
//		try{
//			TestinAgent.setUserInfo(_user);
//		}catch(Exception e){
//			
//		}
	}
	
	public static void writeLog(String str){
		Log.d("call_by_2dx", str);
	}	
	
	public static void saveCrashPID(){
		IF.getInstance().saveCrashPID();
	}
    
    public static void relogin(){
        Net.logout();
        Net.disconnect();
        Intent i = IF.getInstance().getBaseContext().getPackageManager()
        .getLaunchIntentForPackage( IF.getInstance().getBaseContext().getPackageName() );
        i.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        IF.getInstance().finish();
        IF.getInstance().startActivity(i);
        System.exit(0);
    }
}
