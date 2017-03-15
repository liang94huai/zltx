package org.cocos2dx.ext;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;
import java.util.UUID;

import org.hcg.IF.IF;
import org.hcg.stac.empire.publish.IPublishChannel;

import android.accounts.Account;
import android.accounts.AccountManager;
import android.app.backup.BackupManager;
import android.app.backup.RestoreObserver;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.provider.Settings;
import android.provider.Settings.Secure;
import android.telephony.TelephonyManager;
import android.text.TextUtils;
import android.util.Log;

//import com.hcgtech.aoe.util.GameContext;

public class Udid {
	// 新安装的设备--用户
	public static boolean isNewInstallDevice;
	private static final String KEY_UUID = "uuid";

	public final static String PREF_KEY = "cok_openudid";
	public final static String PREFS_NAME = "cok_openudid_prefs";

	/**
	 * 读取UUID
	 * 
	 * @return
	 */
	public static String getUuid() {
		String uuid = "";
		uuid = Settings.System.getString(Jni.getGameActivity()
				.getContentResolver(), KEY_UUID);
		return uuid;
	}

	/**
	 * 读取UUID, api level 23以后
	 * 
	 * @return
	 */
	
	public static String getUuidNew(){
		String uuid = "";
		SharedPreferences pref = Jni.getGameActivity().getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE);
		if ((pref != null) && (pref.getString(PREF_KEY, "") != null) && (pref.getString(PREF_KEY, "").trim().length() > 0)) {
			uuid = pref.getString(PREF_KEY, "");
		}
		
		return uuid;
	}

	/**
	 * 存储UUID
	 * 
	 * @param uuid
	 */
	public static void saveUid(String uuid) {
		Settings.System.putString(Jni.getGameActivity().getContentResolver(),
				KEY_UUID, uuid);
	}

	/**
	 * 存储UUID, api level 23以后
	 * 
	 * @param uuid
	 */
	public static void saveUidNew(String uuid) {
		if(uuid != null){
			SharedPreferences pref = Jni.getGameActivity().getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE);
	    	final Editor e = pref.edit();
			e.putString(PREF_KEY, uuid);
			Boolean b = e.commit();
			if(!b){
				saveUidNew(uuid);
			}
		}
	}

	/**
	 * 取得手机唯一IMEI号
	 * 
	 * @return String
	 */
	public static String getUid() {
		// 取得存储设备的UUID
		String cardUid = getUuid();
		
		if(TextUtils.isEmpty(cardUid) && useNewUuid()){
			cardUid = getUuidNew();
		}
		
		if (TextUtils.isEmpty(cardUid)) {
			Log.d("isEmpty", "isEmpty");
			isNewInstallDevice = true;
			// 取得UUID
			if(useNewUuid()){
				cardUid = generateUUIDNew(Jni.getGameActivity());
				if(TextUtils.isEmpty(cardUid)){
					return getUid();
				}
				// 保存到本地文件
				saveUidNew(cardUid);
			}else{
				cardUid = generateUUID(Jni.getGameActivity());
				// 保存到本地文件
				saveUid(cardUid);
			}
		}
		return cardUid;
	}
	
	/**
	 * 取得手机UUID
	 * 
	 * @param context
	 * @return
	 */
	public static String generateUUID(Context context) {
		String deviceUUID;
		SharedPreferences pref = context.getSharedPreferences("xcuuid", 0);
		if ((pref != null) && (pref.getString("uuid", "") != null)
				&& (pref.getString("uuid", "").trim().length() > 0)) {
			return pref.getString("uuid", "");
		}

		TelephonyManager tm = (TelephonyManager) context
				.getSystemService("phone");
		String deviceId = tm.getDeviceId();

		UUID uuid = UUID.randomUUID();
		String uuidStr = uuid.toString().replaceAll("-", "").substring(0, 15);
		String wifiMAC = getMacAddr(context);
		if (wifiMAC != null) {
			wifiMAC = wifiMAC.replaceAll("\\.|:", "");
		}
		if ((deviceId == null) || (TextUtils.isEmpty(deviceId.trim()))) {
			if ((wifiMAC != null) && (!(TextUtils.isEmpty(wifiMAC)))) {
				deviceUUID = "-" + wifiMAC;
			} else {
				deviceUUID = uuidStr;
			}

		} else if ((wifiMAC != null) && (!(TextUtils.isEmpty(wifiMAC)))) {
			deviceUUID = deviceId + "-" + wifiMAC;
		} else {
			deviceUUID = deviceId + "-" + uuidStr;
		}

		return deviceUUID;
	}

	/**
	 * 取得手机UUID, api level 23以后, ANDROID_ID会重复，加一个手机型号
	 * 
	 * @param context
	 * @return
	 */
	public static String generateUUIDNew(Context context) {
		String deviceUUID = getUuidNew();
		
		if (deviceUUID.equals("")){
			UUID uuid = UUID.randomUUID();
			deviceUUID = uuid.toString().replaceAll("-", "");//.substring(0, 15);
			
			String deviceInfo = getDeviceInfo();
			if(!TextUtils.isEmpty(deviceInfo)){
				deviceUUID += "," + deviceInfo;
			}			
		}

		return deviceUUID;

	}

	/**
	 * 取得mac地址
	 * 
	 * @param context
	 * @return
	 */
	public static String getMacAddr(Context context) {
		WifiManager wifi = (WifiManager) context.getSystemService("wifi");
		WifiInfo info = wifi.getConnectionInfo();
		return info.getMacAddress();
	}
		
	/**
	 * 交叉推广需要用到的方法
	 * 
	 * @return
	 */
	public static String getUidForCpb() {
		return "uuid";
//		TelephonyManager tm = (TelephonyManager) GameContext
//				.getActivityInstance().getSystemService(
//						Context.TELEPHONY_SERVICE);
//		// gets the imei (GSM) or MEID/ESN (CDMA)
//		String uid = tm.getDeviceId();
//		if (TextUtils.isEmpty(uid)) {
//			// 设备不能打电话？好吧，来看看wifi有不
//			// requires ACCESS_WIFI_STATE
//			WifiManager wm = (WifiManager) GameContext.getActivityInstance()
//					.getSystemService(Context.WIFI_SERVICE);
//			// gets the MAC address
//			if (wm.isWifiEnabled()) {
//				// 能打电话就肯定有 TelephonyId，否则只能wifi上网玩游戏，那就肯定有wifi的uid
//				uid = wm.getConnectionInfo().getMacAddress();
//			}
//		}
//		// 保存下来
//
//		return SHA1(uid + "uuid");

	}

	public static String SHA1(String s) {
		try {
			MessageDigest digest = MessageDigest.getInstance("SHA-1");
			digest.update(s.getBytes());
			byte messageDigest[] = digest.digest();
			return toHexString(messageDigest);
		} catch (NoSuchAlgorithmException e) {
			e.printStackTrace();
		}
		return "";
	}

	public static String toHexString(byte[] keyData) {
		if (keyData == null) {
			return null;
		}
		int expectedStringLen = keyData.length * 2;
		StringBuilder sb = new StringBuilder(expectedStringLen);
		for (int i = 0; i < keyData.length; i++) {
			String hexStr = Integer.toString(keyData[i] & 0x00FF, 16);
			if (hexStr.length() == 1) {
				hexStr = "0" + hexStr;
			}
			sb.append(hexStr);
		}
		return sb.toString();
	}
		
	public static boolean useNewUuid(){
		String aid = IF.getInstance().getPublishImpl().getPublishChannel();
		if(Build.VERSION.SDK_INT > android.os.Build.VERSION_CODES.LOLLIPOP_MR1 
				&& (IPublishChannel.PUBLISH_GLOBAL.equals(aid) || IPublishChannel.PUBLISH_BETA.equals(aid))){
			return true;
		}
		return false;
	}
	
	private static String getDeviceInfo(){
		return android.os.Build.MODEL + android.os.Build.BOARD + android.os.Build.ID;
	}
	
	public static void getDataFromCloud(){
		Log.d("data backup get start", "data backup get start");

	}
	
	public static String getUidFromCloudSharedPreferences(){
		String result = "";
		return result;
	}
}
