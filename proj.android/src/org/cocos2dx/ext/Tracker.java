package org.cocos2dx.ext;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.content.Context;
import android.util.Log;

/** For events & error tracking */
public class Tracker {

	public static void init(Cocos2dxActivity ctx) {
	}
	
	public static void sendEvent(String event) {
		sendEvent(Jni.getGameActivity(), event);
	}

	public static void sendEvent(String event, String param) {
		sendEvent(Jni.getGameActivity(), event, param);
	}

	public static void sendEvent(String event, int param) {
		sendEvent(Jni.getGameActivity(), event, param);
	}

	public static void sendEvent(Context ctx, String event) {
		Log.d(Jni.LOG_TAG, "event:" + event);
	}

	public static void sendEvent(Context ctx, String event, String param) {
		Log.d(Jni.LOG_TAG, "event:" + event + " " + param);
	}

	public static void sendEvent(Context ctx, String event, int param) {
		Log.d(Jni.LOG_TAG, "event:" + event + " " + param);
	}

	public void reportException(Exception e) {
		reportException(Jni.getGameActivity(), e);
	}

	public static void reportException(Context c, Exception e) {
		reportException(c, Log.getStackTraceString(e));
	}

	public static void reportException(String error) {
		reportException(Jni.getGameActivity(), error);
	}

	public static void reportException(Context ctx, String error) {
		Log.w(Jni.LOG_TAG, "exception:" + error);
	}
}
