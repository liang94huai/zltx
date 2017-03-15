//
//  HoolaiSdkDelegate.cpp
//  zltx
//
//  Created by devil on 2017/2/13.
//
//

#include "HoolaiSdkDelegate.hpp"
#include "GlobalData.h"
#include <spine/Json.h>

void HoolaiSdkDelegate::onInitFail(string result) {
   printf("onInitFail result:%s\n", result.c_str());
}

void HoolaiSdkDelegate::onInitSucceed(string result) {
    printf("onInitSuccess result:%s\n", result.c_str());
    CCAccessSDK::sharedInstance()->login("");// 自定义参数
    Json *c = Json_create(result.c_str());
    if(!c){
        return;
    }
    std::string biChannel= Json_getString(c, "biChannel", "");
    GlobalData::shared()->biChannel = biChannel;
    
}

void HoolaiSdkDelegate::onLoginSucceeded(CCACUserLoginResponse userLoginResponse) {
   printf("onLoginSucced\n");
   printf("\n uid:%lld\n channel:%s\n channelUid:%s\n accessToken:%s\n productId:%d\n",
          userLoginResponse.uid, userLoginResponse.channel.c_str(),
          userLoginResponse.channelUid.c_str(), userLoginResponse.accessToken.c_str(),
          userLoginResponse.productId);
   //    map < string, string > userExtData;
   //    userExtData[CCROLE_ID] = "123456";
   //    userExtData[CCROLE_NAME] = "张三";
   //    userExtData[CCROLE_LEVEL] = "10";
   //    userExtData[CCZONE_ID] = "1";
   //    userExtData[CCZONE_NAME] = "一服";
   //    userExtData[CCBALANCE] = "0";
   //    userExtData[CCVIP] = "1";
   //    userExtData[CCPARTYNAME] = "unknow";
   //
   //    userExtData[CCACTION] = CCACTION_ENTER_SERVER;
   //    CCAccessSDK::sharedInstance()->setUserExtData(userExtData);
   //
   //    userExtData[CCACTION] = CCACTION_CREATE_ROLE;
   //    CCAccessSDK::sharedInstance()->setUserExtData(userExtData);
   //
   //    userExtData[CCACTION] = CCACTION_LEVEL_UP;
   //    CCAccessSDK::sharedInstance()->setUserExtData(userExtData);
   
   GlobalData::shared()->channelUid = userLoginResponse.channelUid.c_str();
   GlobalData::shared()->platformUID = userLoginResponse.channel.c_str();
   GlobalData::shared()->platformToken = userLoginResponse.accessToken.c_str();
   GlobalData::shared()->platformUserUid = __String::createWithFormat("%lld",userLoginResponse.uid)->getCString();
   GlobalData::shared()->platformProductId = __String::createWithFormat("%d",userLoginResponse.productId)->getCString();
   
   CCSafeNotificationCenter::getInstance()->postNotification( MSG_SdkLoginSuccess );
}

void HoolaiSdkDelegate::onLoginFailed(CCACReturnValue returnValue) {
   printf("onLoginFailed\n code:%s\n desc:%s\n", returnValue.code.c_str(), returnValue.desc.c_str());
}

void HoolaiSdkDelegate::onLogout(string msg) {

    printf("onLogoutSuccess \n");
    //GameController::getInstance()->quitGame();
    CCSafeNotificationCenter::getInstance()->postNotification( MSG_SdkLogoutSuccess );

}

void HoolaiSdkDelegate::onPayFail(string param) {
   printf("payOnFail param:%s\n", param.c_str());
    CCSafeNotificationCenter::getInstance()->postNotification(MSG_PayHoolaiFailed);
}

void HoolaiSdkDelegate::onPaySuccess(string param) {
   printf("paySuccess param:%s \n", param.c_str());
    CCSafeNotificationCenter::getInstance()->postNotification(MSG_PayHoolaiSuccess);
}

void HoolaiSdkDelegate::onPayCancle(string desc) {
   printf("pay cancle desc:%s \n", desc.c_str());
}

void HoolaiSdkDelegate::onGetServerListSuccess(CCACServerInfos serverInfos) {
   printf("getServerInfoSuccess \n");
}

void HoolaiSdkDelegate::onGetServerListFail(string msg) {
   printf("getServerListFail msg:%s \n", msg.c_str());
}

void HoolaiSdkDelegate::onSelectServerFail(string serverId) {
   printf("selectServerFail serverId:%s \n", serverId.c_str());
}

void HoolaiSdkDelegate::onSelectServerSuccess(string serverId) {
   printf("selectServerSuccess serverId:%s \n", serverId.c_str());
}

void HoolaiSdkDelegate::onGameExit(string msg) {
   printf("onExit\n");
   //exit(0);
   CCSafeNotificationCenter::getInstance()->postNotification( MSG_SdkExitSuccess );
}

void HoolaiSdkDelegate::onChannelExit(string msg) {
   printf("onExit\n");
   //exit(0);
   CCSafeNotificationCenter::getInstance()->postNotification( MSG_ExitChannelSuccess );
}

void HoolaiSdkDelegate::onSendBIResult(string msg) {
   printf("sendBIResult:%s \n", msg.c_str());
}

void HoolaiSdkDelegate::onBindPhone(string msg) {
   printf("onBindPhone:%s \n", msg.c_str());
}

bool HoolaiSdkDelegate::onMaintenance(string msg) {
   return true;
}
