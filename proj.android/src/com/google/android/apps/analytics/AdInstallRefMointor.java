package com.google.android.apps.analytics;

import org.hcg.util.GameContext;

import android.app.Activity;

/**
 * 推广渠道跟踪
 */
public class AdInstallRefMointor {
	
	//hasoffers 可以使用两种链接：1跳转链接无法跟踪渠道数据，2 使用直接链接可以跟踪渠道数据
	//1跳转链接：http://hastrk1.com/serve?action=click&publisher_id=23508&site_id=28826&offer_id=255398
	//转换后的URL:
	//https://play.google.com/store/apps/details?id=com.stac.empire.main&referrer=tracking_id%3Dd341a8280c41906d498c721bad1c4517
	//特别说明 hasoffers无法跟踪渠道参数，只能通过 tracking_id来识别
	//D/AdInstall(24219): decode referrer=tracking_id=de76924dd6c12f8e08d0ccd95617b0e7
	//2 直接链接：market://details?id=com.stac.empire.main&referrer=sub_publisher%3Dhasoffers%26sub_adgroup%3Dus%253Bage%2540androidmarket%253Bad_channel%253Bregion_ad%26publisher_id%3D23508%26offer_id%3D255398
	//referrer=sub_publisher=hasoffers&sub_adgroup=us;age@androidmarket;ad_channel;region_ad 
	
	//结论：直接链接会加大推广难度【运营方面】，确定使用跳转链接方式
	
	public static String checkAdInstalledRef(Activity activity) {
		String referrer = MyAnalyticsReceiver.getAdReferrer(GameContext
				.getActivityInstance());
		return referrer;
	}
}
