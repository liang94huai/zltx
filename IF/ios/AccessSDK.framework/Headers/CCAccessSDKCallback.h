//
//  CCAccessSDKCallback.h
//  Access
//
//  Created by Hoolai on 15/11/17.
//  Copyright © 2015年 wsj_hoolai. All rights reserved.
//

#ifndef CCAccessSDKCallback_h
#define CCAccessSDKCallback_h

#include "CCACUserLoginResponse.h"
#include "CCACServerInfos.h"
#include "CCACReturnValue.h"
#include <string>
#include <iostream>

using namespace std;

class CCAccessSDKCallback {
    
public:
    CCAccessSDKCallback(){}
//    virtual ~CCAccessSDKCallback();
    
public:
    virtual void onInitSucceed(string result)=0;
    virtual void onInitFail(string result)=0;
    virtual void onLoginSucceeded(CCACUserLoginResponse userLoginResponse)=0;
    virtual void onLoginFailed(CCACReturnValue returnValue)=0;
    virtual void onLogout(string msg)=0;
    virtual void onPaySuccess(string param)=0;
    virtual void onPayFail(string param)=0;
    virtual void onPayCancle(string desc)=0;
    
    virtual void onGetServerListSuccess(CCACServerInfos serverInfos);
    virtual void onSelectServerSuccess(string serverId);
    virtual void onGameExit(string msg);
    virtual void onChannelExit(string msg);
    virtual void onGetServerListFail(string msg);
    virtual void onSelectServerFail(string serverId);
    
    /**
     服务器维护回调（返回true默认使用系统UI，返回false不弹框游戏自己定制）
     
     @param msg 维护提示信息
     */
    virtual bool onMaintenance(string msg);
    
    virtual void onSendBIResult(string msg);
    
    virtual void onBindPhone(string msg);
};

#endif /* CCAccessSDKCallback_h */
