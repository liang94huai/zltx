package cn.sharesdk.onekeyshare;

import com.mob.tools.FakeActivity;

import android.app.Activity;
import android.content.Intent;
//import cn.sharesdk.sina.weibo.SinaWeibo;


public class ShareSDKController {
	
	public static Activity					hostActivity;					// IF或wrapper的activity，原生未打开时依然存在
	private static Class<?>					hostClass;

	public static boolean					isNativeShowing		= false;	// 仅在IF.onResume中重置为false，主要被IF使用
	public static FakeActivity currentActivity;
	
	public static void init(Activity a)
	{
		hostActivity = a;
		hostClass = a.getClass();
	}
	
	public static FakeActivity getCurrentActivity()
	{
		return currentActivity;
	}
	public static void setCurrentActivity(FakeActivity a)
	{
		currentActivity = a;
	}
	
	public static void showGameActivity(Activity a) {
		showGameActivity(a, false);
	}
	
	public static void showGameActivity(Activity a, boolean reverseAnimation) {
		System.out.println("showGameActivity()");
		
		Intent intent = new Intent(a, hostClass);
		intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_SINGLE_TOP);
		a.startActivity(intent);
		
	}
}
