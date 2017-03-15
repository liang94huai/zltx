package org.hcg.stac.empire.common.manager;

import org.cocos2dx.ext.Native;
import org.hcg.IF.IF;

import android.app.Activity;
import android.content.Context;
import android.os.AsyncTask;
import android.text.TextUtils;
import android.util.Log;

import com.google.android.gcm.GCMBaseIntentService;
import com.google.android.gcm.GCMRegistrar;
/**
 * 根据不同的发布版本，从友盟在线参数中获取GCM通信的SenderId,进行GCM注册
 * @author Jimin
 *
 */
public class GCMAsyncTask extends AsyncTask<Activity,Void,Void>{

	@Override
	protected Void doInBackground(Activity... params) {
		try {
			Context context = IF.getInstance();

			String senderId = GCMBaseIntentService.GCM_DEFAULT_SENDER_ID;
			GCMIntentService.safeSetSenderId(senderId);
			GCMRegistrar.checkDevice(context);
//			GCMRegistrar.checkManifest(context); //发布阶段此行代码可注释掉,否则有的机型报异常
			// 获取此设备的GCM注册ID
			final String regId = GCMRegistrar.getRegistrationId(context);
			if (TextUtils.isEmpty(regId)) {
				// 若取不到GCM注册ID，则立即发起注册请求
				// tao.yu 下面这行在关闭helpshift时就处于注释状态
//				GCMRegistrar.register(context, senderId);
			} else {
				// 设备已经注册到了GCM
				Native.nativeSetGcmRegisterId(regId);
			}
		} catch (UnsupportedOperationException e) {
			Log.e("CommonActivity", e.getMessage());
		} catch (Exception e) {
			Log.e("CommonActivity", e.getMessage());
		}
		return null;
	}

	
}
