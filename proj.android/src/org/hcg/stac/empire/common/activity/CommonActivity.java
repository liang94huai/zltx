package org.hcg.stac.empire.common.activity;

import java.util.Locale;

import org.cocos2dx.ext.Native;
import org.hcg.IF.IF;
import org.hcg.stac.empire.publish.IPublishChannel;
import org.hcg.util.GameContext;

import android.content.Intent;
import android.os.Bundle;

public abstract class CommonActivity extends IF{

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Native.nativeSetFromCountry(Locale.getDefault().getCountry());
		Native.nativeSetParseRegisterId(GameContext.getGamePublisher().getParseNotifyToken());
	}

	@Override
	public void onResume() {
		super.onResume();
	}

	@Override
	protected void onPause() {
		super.onPause();
	}
	
	@Override
	protected void onStop() {
		super.onStop();
	}
	
	@Override
	protected void onDestroy() {
		super.onDestroy();
	}

	@Override
	protected void onSaveInstanceState(Bundle outState) {
		super.onSaveInstanceState(outState);
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
	}

	@Override
	public IPublishChannel getPublishImpl() {
		return null;
	}
}
