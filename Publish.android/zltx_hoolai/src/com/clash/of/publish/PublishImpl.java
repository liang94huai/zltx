package com.clash.of.publish;

import org.hcg.IF.IF;
import org.hcg.stac.empire.pay.PayItemData;
import org.hcg.stac.empire.publish.AbstractPublishImpl;
import org.hcg.util.GameContext;

import android.util.Log;

import com.clash.of.kings.COK;

/**
 * 重构于20150908
 * @author Still4
 *
 */
public class PublishImpl extends AbstractPublishImpl
{
	
	@Override
	public void initlize() {
		miAnalyticsEnabled = true;
		super.initlize();
	}
	
	@Override
	public String getPublishChannel() {
		return PUBLISH_HOOLAI;
	}

	@Override
	public void doPlatformLogin() {
//		COK.doLogin();
	}

	@Override
	public void doPay(PayItemData payItem) {
//		COK.doPay(payItem);
	}
	
	@Override
	public void onConsumeCallback(String orderId, int state){
//		if(state == 0 || state == 2)
//			COK.consumePurchase(orderId);
	}

	@Override
	public void queryHistoryPurchase(){
		Log.d("COK", "COK queryPurchaseOrder start");
		IF.getInstance().runOnUiThread(new Runnable() {
			public void run() {
				try {
//					COK.queryHistoryPurchase();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}
	
	@Override 
	public void parseInit(){
		try{

		}catch(Exception e){
			e.printStackTrace();
		}catch(Throwable e){
			e.printStackTrace();
		}
    }
	
	@Override
	public void parseTrackAppOpened(){

	}

}
