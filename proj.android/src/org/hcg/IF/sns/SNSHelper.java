package org.hcg.IF.sns;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.util.Log;

//import com.conquest.empire.main.R;
import org.hcg.IF.IF;
import org.hcg.util.CommonUtil;
import org.hcg.util.MarketUtil;

/**
 * 社区分享辅助类
 * 
 * @author anychen
 * 
 */
public class SNSHelper {
	public static final String TAG = "share-sns";
	public static final String SNS_TWITTER = "twitter";
	public static final String SNS_RENREN = "renren";
	public static final String SNS_TENCENT = "tencent";
	public static final String SNS_SINA = "sina";
	public static final String SNS_GOOGLE_PLAY = "market";
	public static final String SNS_EMAIL = "email";
	public static final String SNS_OTHER = "UI_Other";
	/**
	 * 分享消息
	 */
	public static final int SHARE_NEWS_EVENT = 2;

	// SocialAuth Component
	// private SocialAuthAdapter mGlobalAdapter;
	private Context mContext;
	private SNSCallbackListener mCallbackListener;
	private String mPostFeedMsg;
	TwitterImpl mTwitterImpl;

	private static final String SHARE_PIC_URI = "invite_share.jpg";

	public SNSHelper(Context ctx) {
		this.mContext = ctx;

	}

	public void postFeed(String msg, String pltSns) {

	}

	public void postFeed_sina(String msg) {
		final String msg1 = msg;

	}

	public void postFeed_tenc(String msg) {
		
	}

	public void postFeed_renr(String msg) {
		
	}

	public void postFeed_playStore() {
		IF.getInstance().runOnUiThread(new Runnable() {
			public void run() {
				MarketUtil.openAppsInMarket(IF.getInstance(), -1);
			}
		});
	}

	public void postFeed_facebook(String msg) {
		
	}

	public void postFeed_twitter(String msg) {
		mPostFeedMsg = msg;
		if (mTwitterImpl == null) {
			mTwitterImpl = new TwitterImpl((Activity) mContext);
			mTwitterImpl.registerListener((Activity) mContext);
		}
		mTwitterImpl.postMessage(msg, null, null, null);
	}

	public void postFeed_email(String msg, String subject) {
		Intent mailIntent = new Intent(android.content.Intent.ACTION_SENDTO);
		mailIntent.setType("image/*");
		mailIntent.setData(Uri.parse("mailto:"));
		mailIntent.putExtra(android.content.Intent.EXTRA_SUBJECT, subject);
		mailIntent.putExtra(android.content.Intent.EXTRA_TEXT, msg);
		mailIntent.putExtra(Intent.EXTRA_STREAM,
				CommonUtil.getAssetsBitmapForUri(SHARE_PIC_URI));
		mContext.startActivity(mailIntent);
	}

	public void postFeed_other(String msg, String imgUri, String sendTile) {
		Intent picMessageIntent = new Intent(android.content.Intent.ACTION_SEND);
		picMessageIntent.setType("image/*");
		picMessageIntent.putExtra(Intent.EXTRA_STREAM,
				CommonUtil.getAssetsBitmapForUri(imgUri));
		picMessageIntent.putExtra(android.content.Intent.EXTRA_TEXT, msg);
		mContext.startActivity(Intent.createChooser(picMessageIntent, sendTile));
	}

	public SNSCallbackListener getCallbackListener() {
		return mCallbackListener;
	}

	public void setCallbackListener(SNSCallbackListener mCallbackListener) {
		this.mCallbackListener = mCallbackListener;
	}

	public void release() {
		
		if (mTwitterImpl != null) {
			mTwitterImpl.unRegisterListener((Activity) mContext);
		}
	}

}
