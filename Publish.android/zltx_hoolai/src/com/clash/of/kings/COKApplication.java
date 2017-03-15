package com.clash.of.kings;


import org.hcg.IF.UtilApplication;

import android.support.multidex.MultiDexApplication;
import android.util.Log;

public class COKApplication extends MultiDexApplication {
	
	@Override
	public void onCreate() {
		super.onCreate();
		UtilApplication.onCreate(this);
		UtilCOKApplication.onCreate(this);
	}
}