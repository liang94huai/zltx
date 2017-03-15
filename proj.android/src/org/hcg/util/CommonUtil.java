package org.hcg.util;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import android.net.Uri;

import org.cocos2dx.ext.Device;
import org.cocos2dx.ext.Udid;

public class CommonUtil {

	/**
	 * 根据Id取得名字
	 * 
	 * @param id
	 * @return
	 */
	public static String getStringName(int id) {
		if (GameContext.getActivityInstance() == null)
			return "";
		if (GameContext.getActivityInstance().getResources() == null) {
			return "";
		}
		String name = GameContext.getActivityInstance().getResources()
				.getString(id);
		return name;
	}
	
	public static boolean isEmpty(String value) {
		if (value == null || value.equals("")) {
			return true;
		} else {
			return false;
		}
	}
	
	public final static byte[] getAssetBitmap(String file) {
		try {
			// "image/icon.png"
			InputStream is = GameContext.getActivityInstance().getAssets()
					.open(file);
			int size = is.available();
			byte[] buf = new byte[size];
			is.read(buf, 0, size);
			return buf;
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}

	private static final String SHARE_IMG_DIR = "share_img";

	public final static Uri getAssetsBitmapForUri(String fileName) {
		try {
			if (!isAvaiableSDcard()) {
				return null;
			}
			File dir = new File(getSDcardPath() + File.separator
					+ SHARE_IMG_DIR);
			if (!dir.exists()) {
				dir.mkdir();
			}
			String share_imgPath = dir.getCanonicalPath() + File.separator
					+ fileName;
			File file = new File(share_imgPath);
			if (!file.exists()) {
				byte[] jpgData = getAssetBitmap(fileName);
				if (jpgData == null)
					return null;
				FileOutputStream fos = new FileOutputStream(file);
				fos.write(jpgData);
				fos.flush();
				fos.close();
				// DebugLog.d("TAG", "file:" + file.getCanonicalPath());
				// FileInputStream fis = new FileInputStream(file);
				// byte[] jpgData1 = new byte[fis.available()];
				// fis.read(jpgData1);
				// DebugLog.d("TAG", "size:" + jpgData1.length);
			}
			return Uri.fromFile(file);
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}

	public static boolean isAvaiableSDcard() {
		// 可能出异常 NullPointerException
		// Attempt to invoke interface method 'android.os.storage.StorageVolume[] android.os.storage.IMountService.getVolumeList()' on a null object reference
		try{
			String sdCardState = android.os.Environment.getExternalStorageState();
			return android.os.Environment.MEDIA_MOUNTED.equals(sdCardState);
		}
		catch (Exception e)
		{
			return false;
		}
	}

	public static String getSDcardPath() {
		try {
			File SDFile = android.os.Environment.getExternalStorageDirectory();
			return SDFile.getCanonicalPath();
		} catch (IOException e) {
			return null;
		}
	}
	
	public static boolean isVersion_Ad_Mdd() {
		return GameContext.getActivityInstance().getPackageName()
				.equals("com.stac.cok.main.mdd");
	}

	public static boolean isVersion_Global_Free() {
		return GameContext.getActivityInstance().getPackageName()
				.equals("com.stac.cok.main.free");
	}
	
	public static void setNewDeviceForTest() {
		// TODO
	}
	
}
