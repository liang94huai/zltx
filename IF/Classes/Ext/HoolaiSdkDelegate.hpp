//
//  HoolaiSdkDelegate.hpp
//  zltx
//
//  Created by devil on 2017/2/13.
//
//

#ifndef HoolaiSdkDelegate_hpp
#define HoolaiSdkDelegate_hpp

#include <stdio.h>
#include "CommonInclude.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
   #include "HoolaiFastAccess/CCAccessSDK.h"
   #include "HoolaiFastAccess/GameProxy.h"
#else
   #import <AccessSDK/CCAccessSDK.h>
#endif

class HoolaiSdkDelegate: public CCAccessSDKCallback {
   
public:
   void onInitSucceed(string result);
   void onInitFail(string result);
   void onLoginSucceeded(CCACUserLoginResponse userLoginResponse);
   void onLoginFailed(CCACReturnValue returnValue);
   void onLogout(string msg);
   void onPaySuccess(string param);
   void onPayFail(string param);
   void onPayCancle(string desc);
   void onGetServerListSuccess(CCACServerInfos serverInfos);
   void onSelectServerSuccess(string serverId);
   void onGameExit(string msg);
   void onChannelExit(string msg);
   void onGetServerListFail(string msg);
   void onSelectServerFail(string serverId);
   
   void onSendBIResult(string msg);
   
   void onBindPhone(string msg);
   
   bool onMaintenance(string msg);
};

#endif /* HoolaiSdkDelegate_hpp */
