package cn.sharesdk.onekeyshare;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import android.content.Context;
import android.content.Intent;
import android.os.PowerManager;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import cn.sharesdk.framework.Platform;
import cn.sharesdk.framework.ShareSDK;

//import com.elex.chatservice.view.ICocos2dxScreenLockListener;
import com.mob.tools.FakeActivity;

public class PlatformListFakeActivity extends FakeActivity {
	protected HashMap<String, Object> shareParamsMap;
	protected boolean silent;
	protected ArrayList<CustomerLogo> customerLogos;
	protected HashMap<String, String> hiddenPlatforms;
	private boolean canceled = false;
	protected View backgroundView;

	protected OnShareButtonClickListener onShareButtonClickListener;
	protected boolean dialogMode = false;
	protected ThemeShareCallback themeShareCallback;
	
	// ---------------------------------------------------
	//				锁屏超过1分钟，返回后自动退出
	// ---------------------------------------------------

	public static boolean isReturnFromScreenLock=false;
	private long screenLockTime;
	protected boolean isExiting = false; // 是否是主动触发的退出（否则可能是锁屏）
	private boolean isScreenLocked = false;
//	public static ICocos2dxScreenLockListener previousActivity;

	public static interface OnShareButtonClickListener {
		void onClick(View v, List<Object> checkPlatforms);
	}

	public void onCreate() {
		super.onCreate();
//		System.out.print("PlatformListFakeActivity==onCreate");
//		Log.v("PlatformListFakeActivity", "onCreate");
		canceled = false;

		if(themeShareCallback == null) {
			finish();
		}
	}
	
	// 主动打开或锁屏返回两种情况
		@Override
		public void onResume()
		{
			System.out.println("PlatformListFakeActivity onResume: " + this);
			
			super.onResume();

//			if (isScreenLocked)
//			{
//				isScreenLocked = false;
//				// 锁屏返回，超时，退出聊天界面
//				// 仅调用2dx的onResume
//				if ((System.currentTimeMillis() - screenLockTime) > (1000 * 60))
//				{
//					isReturnFromScreenLock = true;
////					ShareSDKController.showGameActivity(activity);
//				}
//				// 锁屏返回，未超时，不退出聊天界面
//				else
//				{
////					if (previousActivity != null && (previousActivity instanceof ICocos2dxScreenLockListener))
////					{
////						previousActivity.handle2dxResume();
////					}
//				}
//			}

		}
	
		@Override
		public void onPause()
		{
			System.out.println("PlatformListFakeActivity onPause: " + this);

			// 当打开其它原生activity时，会出现并非锁屏的onPause，需要直接判断是否锁屏
//			PowerManager pm = (PowerManager) activity. getSystemService(Context.POWER_SERVICE);
//			boolean isScreenOn = pm.isScreenOn();
//			System.out.println("isScreenOn: " + isScreenOn);

			super.onPause();
//			if (!isScreenOn) // !isExiting && 
//			{
//				// 聊天界面锁屏
//				isScreenLocked = true;
//				screenLockTime = System.currentTimeMillis();
//			}
			
			// 仅调用2dx的onPause
//			if(!isJumpToInnerActivity()){
//				System.out.println("previousActivity: " + previousActivity);
//				if (previousActivity != null && (previousActivity instanceof ICocos2dxScreenLockListener))
//				{
//					System.out.println("PlatformListFakeActivity handle2dxPause() ");
//					previousActivity.handle2dxPause();
//				}
//			}
		}

	public boolean onKeyEvent(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK) {
			canceled = true;
		}
		return super.onKeyEvent(keyCode, event);
	}

	protected void setCanceled(boolean canceled) {
		this.canceled = canceled;
	}

	public boolean onFinish() {

		// 取消分享菜单的统计
		if (canceled) {
			ShareSDK.logDemoEvent(2, null);
		}
		
		return super.onFinish();
	}
	

	public void showContext(Context context) {
//		previousActivity = (ICocos2dxScreenLockListener) context;
		
	}

	@Override
	public void show(Context context, Intent i) {
		super.show(context, i);
	}

	public HashMap<String, Object> getShareParamsMap() {
		return shareParamsMap;
	}

	public void setShareParamsMap(HashMap<String, Object> shareParamsMap) {
		this.shareParamsMap = shareParamsMap;
	}

	public boolean isSilent() {
		return silent;
	}

	public void setSilent(boolean silent) {
		this.silent = silent;
	}

	public ArrayList<CustomerLogo> getCustomerLogos() {
		return customerLogos;
	}

	public void setCustomerLogos(ArrayList<CustomerLogo> customerLogos) {
		this.customerLogos = customerLogos;
	}

	public HashMap<String, String> getHiddenPlatforms() {
		return hiddenPlatforms;
	}

	public void setHiddenPlatforms(HashMap<String, String> hiddenPlatforms) {
		this.hiddenPlatforms = hiddenPlatforms;
	}

	public View getBackgroundView() {
		return backgroundView;
	}

	public void setBackgroundView(View backgroundView) {
		this.backgroundView = backgroundView;
	}

	public OnShareButtonClickListener getOnShareButtonClickListener() {
		return onShareButtonClickListener;
	}

	public void setOnShareButtonClickListener(OnShareButtonClickListener onShareButtonClickListener) {
		this.onShareButtonClickListener = onShareButtonClickListener;
	}

	public boolean isDialogMode() {
		return dialogMode;
	}

	public void setDialogMode(boolean dialogMode) {
		this.dialogMode = dialogMode;
	}

	public ThemeShareCallback getThemeShareCallback() {
		return themeShareCallback;
	}

	public void setThemeShareCallback(ThemeShareCallback themeShareCallback) {
		this.themeShareCallback = themeShareCallback;
	}

	protected void onShareButtonClick(View v, List<Object> checkedPlatforms) {

		if(onShareButtonClickListener != null) {
			onShareButtonClickListener.onClick(v, checkedPlatforms);
		}

		HashMap<Platform, HashMap<String, Object>> silentShareData = new HashMap<Platform, HashMap<String,Object>>();
		final List<Platform> supportEditPagePlatforms = new ArrayList<Platform>();

		Platform plat;
		HashMap<String, Object> shareParam;
		for(Object item : checkedPlatforms) {
			if(item instanceof CustomerLogo){
				CustomerLogo customerLogo = (CustomerLogo)item;
				customerLogo.listener.onClick(v);
				continue;
			}

			plat = (Platform)item;
			String name = plat.getName();

			// EditPage不支持微信平台、Google+、QQ分享、Pinterest、信息和邮件，总是执行直接分享
			if(silent || ShareCore.isDirectShare(plat)) {
				shareParam = new HashMap<String, Object>(shareParamsMap);
				shareParam.put("platform", name);
				silentShareData.put(plat, shareParam);
			} else {
				supportEditPagePlatforms.add(plat);
			}
		}
		if (silentShareData.size() > 0) {
			themeShareCallback.doShare(silentShareData);
		}

		// 跳转EditPage分享
		if(supportEditPagePlatforms.size() > 0) {
			showEditPage(supportEditPagePlatforms);
		}

		finish();
	}

	protected void showEditPage(List<Platform> platforms) {
		showEditPage(getContext(), platforms);
	}

	public void showEditPage(Context context, Platform platform) {
		ArrayList<Platform> platforms = new ArrayList<Platform>(1);
		platforms.add(platform);
		showEditPage(context, platforms);
	}

	protected void showEditPage(Context context, List<Platform> platforms) {
		// 编辑分享内容的统计
		ShareSDK.logDemoEvent(3, null);

		EditPageFakeActivity editPageFakeActivity;
		String editPageClass = ((Object)this).getClass().getPackage().getName()+".EditPage";
		try {
			editPageFakeActivity = (EditPageFakeActivity) Class.forName(editPageClass).newInstance();
		} catch (Exception e) {
			e.printStackTrace();
			return;
		}

		editPageFakeActivity.setBackgroundView(backgroundView);
		editPageFakeActivity.setShareData(shareParamsMap);
		editPageFakeActivity.setPlatforms(platforms);
		if (dialogMode) {
			editPageFakeActivity.setDialogMode();
		}
		editPageFakeActivity.showForResult(context, null, new FakeActivity() {
			public void onResult(HashMap<String, Object> data) {
				if(data == null)
					return;
				if (data.containsKey("editRes")) {
					@SuppressWarnings("unchecked")
					HashMap<Platform, HashMap<String, Object>> editRes
							= (HashMap<Platform, HashMap<String, Object>>) data.get("editRes");
					themeShareCallback.doShare(editRes);
				}
			}
		});
	}
}
