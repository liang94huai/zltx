package org.cocos2dx.ext;

import android.content.ActivityNotFoundException;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.text.TextUtils;
import android.util.Log;

import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.net.HttpURLConnection;
import java.util.List;
import java.util.Map;

import org.hcg.stac.empire.pay.PayItemData;
import org.hcg.stac.empire.publish.IPublishChannel;
import org.hcg.util.CommonUtil;
import org.hcg.util.GameContext;
import org.json.JSONObject;
import org.hcg.IF.IF;

// import com.appsflyer.AppsFlyerLib;

public class CppCallJava {
	
	public static String getGaid(){
		String gaid = "";
		if(gaid == null || "".equals(gaid))
			return null;
		return gaid;
	}
	
	public static String getAppsFlyerUID(){
		return "";
		// return AppsFlyerLib.getAppsFlyerUID(GameContext.getGameInstance());
	}
	
	public static void gotoMarket(String url) {
//		return GameContext.getGamePublisher().getPublish_ServerPort();
		try{
			Uri localUri = Uri.parse(url);
			Intent intent = new Intent(Intent.ACTION_VIEW, localUri);
			GameContext.getActivityInstance().startActivity(intent);
		}catch(Exception e)
		{
			
		}
//		GameContext.release();
//		GameContext.getActivityInstance().finish();
//		android.os.Process.killProcess(android.os.Process.myPid());// 杀掉进程彻底退出
//		intent.setAction(Intent.ACTION_VIEW);
	}
	
	public static void sendMail(String address,String title,String content){
		
		final String faddress = address;
		final String ftitle = title;
		final String fcontent = content;
		
		GameContext.getActivityInstance().runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				try {
					Intent email = new Intent(Intent.ACTION_SENDTO);
					email.setType("plain/text");
					email.setData(Uri.parse("mailto:"
							+ faddress));

					email.putExtra(Intent.EXTRA_SUBJECT, ftitle);
					// 设置要默认发送的内容
					email.putExtra(Intent.EXTRA_TEXT, fcontent);
					// 调用系统的邮件系统
					GameContext.getActivityInstance().startActivity(email);
				} catch (ActivityNotFoundException e) {
					// TODO: handle exception
				}
			}
		});
	}
	
	
	public static String getPublishChannel(){
		return GameContext.getGamePublisher().getPublishChannel();
	}
	

	/**
	 * 
	 * @param platform
	 *            :0-Google-plus,1其他
	 * @return
	 */
	public static String getLoginedInfo_Platform(int platform) {
		try {
			if (platform == 0) {
//				return GooglePlusHelper.getInstance().getLoginInfo_GooglePlus();
				return IF.getInstance().getGoogleLoginInfo();
			} else if (platform == 1) {
				return "";//getLoginedInfo_3rd();
			}

		} catch (Exception e) {
		}
		JSONObject obj = new JSONObject();
		return obj.toString();
	}
	
	/**
	 * 登入第三方平台
	 * @param platform
	 */
	public static void login_Platform(int platform){
		if (platform == 0) {
			// GooglePlusHelper.getInstance().login();
			GameContext.getGameInstance().runOnUiThread(new Runnable() {
	            @Override
	            public void run() {
	    			try {
	    				GameContext.getGameInstance().doGoogleSignIn();
	    			} catch (java.lang.IllegalStateException e) {
	    				e.printStackTrace();
	    			}
	            }
	        });
		}
	}
	
	/**
	 * 登出第三方平台
	 * @param platform
	 */
	public static void logout_Platform(int platform){
		if (platform == 0){
//			GooglePlusHelper.getInstance().logout();
			try{
				IF.getInstance().doGoogleSignOut();
			} catch (java.lang.IllegalStateException e) {
				e.printStackTrace();
			}
		}
	}
	
	/**
	 * 是否可使用第三方平台
	 * @param platform
	 * @return
	 */
	public static boolean isAvailable_Platform(int platform){
		if (platform == 0) {
			// return
			// GooglePlusHelper.getInstance().isGooglePlayServicesAvailable();
			try {
				return IF.getInstance().isGooglePlayServicesAvailable();
			} catch (java.lang.IllegalStateException e) {
				e.printStackTrace();
			}
		}
		return false;
	}
	
	/**
	 * 是否已经连接到第三方平台
	 * @param platform
	 * @return
	 */
	public static boolean isConnected_Platform(int platform){
		if (platform == 0){
//			return GooglePlusHelper.getInstance().isConnected();
			try{
				return IF.getInstance().isGoogleSignedIn();
			} catch (java.lang.IllegalStateException e) {
				e.printStackTrace();
			}
		}
		return false;
	}
	
	public static void onUploadPhoto(int uid, int srcCode, int idx){
		try
		{
			if(srcCode == 0)
			{
				// 拍照
				IF.getInstance().showPicturePicker1(uid, idx);
			}
			else if(srcCode == 1)
			{
				// 从本地选取照片
				 IF.getInstance().showPicturePicker2(uid, idx);
			}
		}
		catch(Exception e){
		}
	}
	
	public static String getSDcardPath(){
		String path = "";
		if (CommonUtil.isAvaiableSDcard())//check SDcard available
		{
			try
			{
				path = CommonUtil.getSDcardPath();
				path = path + File.separator + "data"+File.separator+"data"+File.separator+GameContext.getActivityInstance().getPackageName() + File.separator + "head";
	
				File dir = new File(path);
				if(!dir.exists() || !dir.isDirectory())
				{
					if(dir.mkdirs())
					{
						path = dir.getAbsolutePath() + File.separator;
					}
					else
					{
						path = CommonUtil.getSDcardPath() + File.separator;
					}
				}
				else
				{
					path = dir.getAbsolutePath() + File.separator;
				}
				File nomedia = new File(path + File.separator + ".nomedia");
				if(!nomedia.exists() || !nomedia.isDirectory())
				{
					nomedia.mkdirs();
				}
			}
			catch(Exception e){
			}
		}
//		Log.d("getSDcardPath", "getSDcardPath="+path);
		return path;
	}
	
	//第三方统计事件触发start
	public static void triggerEventLoginComplete(String userId,String userName,String userLevel,String cokfbad){
		GameContext.getGamePublisher().triggerEventLoginComplete(userId, userName, userLevel, cokfbad);
	}
	
	public static void triggerEventCompletedRegistration(){
		GameContext.getGamePublisher().triggerEventCompletedRegistration();
	}
	
	public static void triggerEventCompletedTutorial(){
		GameContext.getGamePublisher().triggerEventCompletedTutorial();
	}
	
	public static void triggerEventAchievedLevel(int level){
		GameContext.getGamePublisher().triggerEventAchievedLevel(level);
	}
	
	public static void triggerEventPurchaseInit(String cost,String itemId){
		GameContext.getGamePublisher().triggerEventPurchaseInit(cost, itemId);
	}
	
	public static void triggerEventPurchase(String cost,String itemId){
		GameContext.getGamePublisher().triggerEventPurchase(cost, itemId);
	}
	
	//第三方统计事件触发end
	
	public static void doPlatformLoading(){
		GameContext.getGamePublisher().doPlatformLoading();
	}
	
	public static void doPlatformLogin(){
		GameContext.getGamePublisher().doPlatformLogin();
	}
	
	public static boolean needPlatformQuit(){
		return GameContext.getGamePublisher().needPlatformQuit();
	}
	
	public static void doPlatformQuit(){
		GameContext.getGamePublisher().doPlatformQuit();
	}
	public static void releaseChannelRsc(){
		GameContext.getGamePublisher().releaseChannelRsc();
	}
	
	public static void loginSNS(String pf){
		GameContext.getGamePublisher().loginSNS(pf);
	}

	public static void logoutSNS(String pf){
		GameContext.getGamePublisher().logoutSNS(pf);
	}
	
	public static void doPay(PayItemData payItem) {
		GameContext.getGamePublisher().doPay(payItem);
	}
	
	public static void payClicked(final String pay_id , final String gold_num, final String get_gold_count, final String userId) {
		GameContext.getGamePublisher().payClicked(pay_id, gold_num, get_gold_count, userId);
	}
	
	public static void payClickedToUser(final String pay_id , final String gold_num, final String get_gold_count, final String userId, final String toUID) {
		GameContext.getGamePublisher().payClickedToUser(pay_id, gold_num, get_gold_count, userId, toUID);
	}
	
	public static void trackEvent(String event){
		GameContext.getGamePublisher().trackEvent(event);
	}
	
	public static void onConsumeCallback(String orderId, int state){
		GameContext.getGamePublisher().onConsumeCallback(orderId, state);
	}

	public static void queryHistoryPurchase(){
		GameContext.getGamePublisher().queryHistoryPurchase();
	}
}
