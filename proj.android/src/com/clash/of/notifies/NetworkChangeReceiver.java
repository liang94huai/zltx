package com.clash.of.notifies;

import org.cocos2dx.ext.Native;
import org.hcg.IF.IF;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.util.Log;


public class NetworkChangeReceiver extends BroadcastReceiver {

    @Override
    public void onReceive(final Context context, final Intent intent) {
    	Log.d("NetworkChangeState","NetworkChangeReceiver triggered 111!!");
         if (isOnline(context) && IF.getInstance() !=null) {
            IF.getInstance().runOnGLThread(new Runnable() {
                public void run() { 
                    if(IF.getInstance() !=null)
                    {
                        Native.networkAvailable(true);
                    }
                    
                        }
                    });
            
        }
    }

    public boolean isOnline(Context context) {

        Log.d("NetworkChangeState","NetworkChangeReceiver triggered 222!!");

        ConnectivityManager cm = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo netInfo = cm.getActiveNetworkInfo();
        //should check null because in air plan mode it will be null
        return (netInfo != null && netInfo.isConnected());
    }

}