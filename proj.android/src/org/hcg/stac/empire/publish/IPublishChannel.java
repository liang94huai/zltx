package org.hcg.stac.empire.publish;

import org.hcg.stac.empire.pay.PayItemData;

public interface IPublishChannel {
	
	public static final String PUBLISH ="common";//国内提审版本
	public static final String PUBLISH_HOOLAI = "hoolai"; // hoolai
	public static final String PUBLISH_BETA = "beta";//测试服
	public static final String PUBLISH_GLOBAL="market_global";//谷歌
	public static final String PUBLISH_TSTORE = "tstore";//韩国t
	public static final String PUBLISH_NSTORE = "nstore";//韩国n
	public static final String PUBLISH_AMAZON = "amazon";//亚马逊
	public static final String PUBLISH_CN1 = "cn1";//国内
	public static final String PUBLISH_ELEX337="elex337";
	public static final String PUBLISH_CAFEBAZAAR="cafebazaar";//中东
	public static final String PUBLISH_MOL="mol";//泰国
	public static final String PUBLISH_TENCENT="tencent";//腾讯
	public static final String PUBLISH_MYCARD="mycard";//台湾mycard
	public static final String PUBLISH_GASH="gash";//台湾gash
	public static final String PUBLISH_MI="cn_mi";//小米
	public static final String PUBLISH_MIHY="cn_mihy";//小米互娱
	public static final String PUBLISH_UC="cn_uc";//uc
	public static final String PUBLISH_360="cn_360";//360
	public static final String PUBLISH_BAIDU="cn_baidu";//百度
	public static final String PUBLISH_XG="cn_";//西瓜
	public static final String PUBLISH_GOODGAMES="goodgames";//泰国
	

	public abstract void initlize();

    public abstract String getPublishChannel();
    
	public abstract void doPay(PayItemData payItem);
	
	public abstract void triggerEventAppForground();
	
	public abstract void triggerEventAppBackground();

	public abstract void triggerEventLoginComplete(String userId,String userName,String userLevel,String cokfbad);
	
	public abstract void triggerEventCompletedRegistration();
	
	public abstract void triggerEventCompletedTutorial();
	
	public abstract void triggerEventAchievedLevel(int level);
	
	public abstract void triggerEventPurchaseInit(String cost,String itemId);
	
	public abstract void triggerEventPurchase(String cost,String itemId);
	
	public abstract void doPlatformLoading();
	
	public abstract void doPlatformLogin();
	
	public abstract void loginSNS(String pf);

	public abstract void logoutSNS(String pf);
	
	public abstract void payClicked(final String pay_id , final String gold_num, final String get_gold_count, final String userId);
	
	public abstract void payClickedToUser(final String pay_id , final String gold_num, final String get_gold_count, final String userId, final String toUID);
	
	public abstract void trackEvent(String event);
	
	public abstract void onConsumeCallback(String orderId, int state);

	public abstract void queryHistoryPurchase();
	
	public abstract boolean needPlatformQuit();
	
	public abstract void doPlatformQuit();

	public abstract void releaseChannelRsc();
	
	public abstract String getParseNotifyToken();
	
	public abstract void parseInit();
	
	public abstract void parseTrackAppOpened();
}
