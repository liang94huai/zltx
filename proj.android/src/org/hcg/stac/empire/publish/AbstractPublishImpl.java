package org.hcg.stac.empire.publish;

import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.lang.StringUtils;
import org.cocos2dx.ext.Native;
import org.hcg.IF.IF;
import org.hcg.notifies.LocalNotificationManager;
import org.hcg.stac.empire.pay.PayItemData;
import org.hcg.util.CommonUtil;
import org.hcg.util.GameContext;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.util.Log;


// import com.appsflyer.AFInAppEventParameterName;
// import com.appsflyer.AFInAppEventType;
// import com.appsflyer.AppsFlyerLib;
// import com.mobileapptracker.MATEvent;
// import com.mobileapptracker.MATEventItem;
import com.xiaomi.migamechannel.MiGameChannel;

public abstract class AbstractPublishImpl implements IPublishChannel {

	protected boolean miAnalyticsEnabled = false;
	protected String parseNotifyToken = null;
	
	public AbstractPublishImpl() {
		//每次加载都用到的逻辑，如参数设置
	}
	
	@Override
	public void initlize(){
		Log.d("COK AbstractPublishImpl", "doinit");
		if(miAnalyticsEnabled)
	        MiGameChannel.Initialize(GameContext.getActivityInstance(), "com.ywj.zltx.mi", getPublishChannel());
	}
	
	/**
	 * 统计部分
	 */
	@Override
	public void triggerEventAppForground(){
		if(miAnalyticsEnabled)
			MiGameChannel.setForegroundTime();
	}

	@Override
	public void triggerEventAppBackground(){
		if(miAnalyticsEnabled)
			MiGameChannel.UploadDuration();
	}

	@Override
	public void triggerEventLoginComplete(String userId,String userName,String userLevel,String cokfbad){
		// AppsFlyerLib.setAppUserId(userId);

		// if(miAnalyticsEnabled)
		// 	MiGameChannel.Login(userId, userLevel);
		
		// //记录注册用户uid，用于数据分析
		// // MATEvent matEvent = new MATEvent(1039280379);
		// // matEvent.withAttribute1(userId);
		// // matEvent.withAttribute2(cokfbad);
		// // if(GameContext.getGameInstance().mobileAppTracker != null){
		// // 	GameContext.getGameInstance().mobileAppTracker.measureEvent(matEvent);
		// // }

		// final SharedPreferences sharedPreferences = GameContext.getActivityInstance().getSharedPreferences(LocalNotificationManager.CACHE_GAME_UID_KEY, Context.MODE_PRIVATE);
		// Editor editor = sharedPreferences.edit();
		// editor.putString("GAMEUID", userId);
		// editor.commit();
	}

	@Override
	public void triggerEventCompletedRegistration(){
		
		// AppsFlyerLib.trackEvent(IF.getInstance(),AFInAppEventType.COMPLETE_REGISTRATION,null);
	}
	
	@Override
	public void triggerEventCompletedTutorial(){

		// AppsFlyerLib.trackEvent(IF.getInstance(),AFInAppEventType.TUTORIAL_COMPLETION,null);
	}
	
	@Override
	public void triggerEventAchievedLevel(int level){
		

		// Map<String, Object> eventValue = new HashMap<String, Object>();
		// eventValue.put(AFInAppEventParameterName.LEVEL,level);
		// AppsFlyerLib.trackEvent(IF.getInstance(),AFInAppEventType.LEVEL_ACHIEVED,eventValue);
		
		// if(miAnalyticsEnabled)
		// 	MiGameChannel.UserLevelUpgrade(Native.nativeGetUID(), String.valueOf(level));
	}
	
	@Override
	public void triggerEventPurchaseInit(String cost,String itemId){
		if(miAnalyticsEnabled){
			String amount = String.valueOf(PayItemData.getPaidAmountInCents(PayItemData.Currency.CNY, Float.parseFloat(cost)));
			String level = String.valueOf(Native.nativeGetLevel());
	        MiGameChannel.BeforeRecharge(Native.nativeGetUID(), level, amount);
		}
	}
	
	@Override
	public void triggerEventPurchase(String cost,String itemId){
		
		// Map<String, Object> eventValue = new HashMap<String, Object>();
		// eventValue.put(AFInAppEventParameterName.REVENUE,cost);
		// eventValue.put(AFInAppEventParameterName.CONTENT_ID,itemId);
		// eventValue.put(AFInAppEventParameterName.CURRENCY,"USD");
		// AppsFlyerLib.trackEvent(IF.getInstance(),AFInAppEventType.PURCHASE,eventValue);
		
		if(miAnalyticsEnabled){
			String amount = String.valueOf(PayItemData.getPaidAmountInCents(PayItemData.Currency.CNY, Float.parseFloat(cost)));
			String level = String.valueOf(Native.nativeGetLevel());
	        MiGameChannel.AfterRecharge(Native.nativeGetUID(), level, amount);
		}
		
		double price = Double.parseDouble(cost);
		
		// MATEventItem matEventItem = new MATEventItem(itemId);
		// MATEvent matEvent = new MATEvent(1021911653);
		// matEvent.withRevenue(price);
		// matEvent.withCurrencyCode("USD");
		// matEvent.withEventItems(Arrays.asList(matEventItem));
		// if(GameContext.getGameInstance().mobileAppTracker != null){
		// 	GameContext.getGameInstance().mobileAppTracker.measureEvent(matEvent);
		// }
	}
	
	/**
	 * 平台登陆部分
	 */
	@Override
	public void doPlatformLoading(){
		//原用于home键回来的百度悬浮窗清除后再登陆
	}
	
	@Override
	public void doPlatformLogin(){
	}
	
	@Override
	public boolean needPlatformQuit(){
		return false;
	}
	
	@Override
	public void doPlatformQuit(){
	}

	 @Override
    public void releaseChannelRsc() {
    	// TODO Auto-generated method stub
    	
    }
	
	@Override
	public void loginSNS(String pf){
	}

	@Override
	public void logoutSNS(String pf){
	}
	
	/**
	 * 原JNI接口
	 */
	@Override
	public void doPay(PayItemData payItem) {
		//未重载的工程清除正在购买标记
    	Native.nativeAndroidPaymentFail();
	}
	
	@Override
	public void payClicked(final String pay_id , final String gold_num, final String get_gold_count, final String userId) {
		IF.getInstance().runOnUiThread(new Runnable() {
			public void run() {
				try {
					if(StringUtils.isBlank(userId)){
						GameContext.getGameInstance().showToast("Payment Error, please try again later");
						Native.nativeAndroidPaymentFail();
						return;
					}
					PayItemData payItem = new PayItemData();
					payItem.setItemId(pay_id);
					payItem.setPrice(Float.parseFloat(gold_num));//购买所需钱数
					payItem.setCoin(Integer.parseInt(get_gold_count));//购买金币数量，用于显示名字
					payItem.setUserId(userId);
					GameContext.getGamePublisher().doPay(payItem);
				} catch (Exception e) {
					// tracker.reportException(e);
				}
			}
		});
	}
	
	@Override
	public void payClickedToUser(final String pay_id , final String gold_num, final String get_gold_count, final String userId, final String toUID) {
		IF.getInstance().runOnUiThread(new Runnable() {
			public void run() {
				try {
					if(StringUtils.isBlank(userId)){
						GameContext.getGameInstance().showToast("Payment Error, please try again later");
						Native.nativeAndroidPaymentFail();
						return;
					}
					PayItemData payItem = new PayItemData();
					payItem.setItemId(pay_id);
					payItem.setPrice(Float.parseFloat(gold_num));//购买所需钱数
					payItem.setCoin(Integer.parseInt(get_gold_count));//购买金币数量，用于显示名字
					payItem.setUserId(userId);
					payItem.setToUserID(toUID);
					GameContext.getGamePublisher().doPay(payItem);
				} catch (Exception e) {
					// tracker.reportException(e);
				}
			}
		});
	}
	
	@Override
	public void trackEvent(String event){
		if("two_days_login".equals(event)){
			// AppsFlyerLib.trackEvent(IF.getInstance(),"cok_one_day_retention",null);
		}
		int eventId = 0;
		if("two_days_login".equals(event))
			eventId = 1021831415;
		if("tutorial_over".equals(event))
			eventId = 1021833341;
//		if("first_pay".equals(event))
//			eventId = 0;
	    if("reach_level_4".equals(event))
	    	eventId = 1021834205;
	    if("reach_level_5".equals(event))
	    	eventId = 1088099481;
	    if("reach_level_6".equals(event))
	    	eventId = 1021834769;
		// if(eventId > 0 && GameContext.getGameInstance().mobileAppTracker != null){
		// 	GameContext.getGameInstance().mobileAppTracker.measureEvent(eventId);
		// }
	}
	
	@Override
	public void onConsumeCallback(String orderId, int state){
	}

	@Override
	public void queryHistoryPurchase(){
	}
	
	@Override
	public String getParseNotifyToken(){
		if(parseNotifyToken == null){
			parseInit();
		}
		parseTrackAppOpened();
		return parseNotifyToken;
	}
	
}
