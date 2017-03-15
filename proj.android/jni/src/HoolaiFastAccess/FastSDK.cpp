//
//  CCAccessSDK.cpp
//  Access
//
//  Created by Hoolai on 15/11/17.
//  Copyright © 2015年 wsj_hoolai. All rights reserved.
//

#include <cstdio>
#include <android/log.h>
#include "CCAccessSDK.h"
#include "GameProxy.h"
#include <stdlib.h>
#include <list>

#define LOG_TAG "JniSDK"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

InitCallBack* _initCallback;
LoginCallback* _loginCallback;
PayCallBack* _payCallback;
ExitCallBack* _exitCallback;

//android init callback
class PlatformAndroidInit: public InitCallBack {

public:
	void onInitSuccess(const char *sucessInfo) {
		LOGD("onInitSuccess:%s", sucessInfo);
		CCAccessSDK::sharedInstance()->sdkCallback->onInitSucceed(sucessInfo);
	}
	void onInitFail(const char *failInfo) {
		CCAccessSDK::sharedInstance()->sdkCallback->onInitFail(failInfo);
	}
};

//android login callback
class PlatformAndroidLogin: public LoginCallback {

public:
	// 当登陆成功时，系统会调用该注册的Listener实例的该方法
	// user  中包含了登陆用户的相关信息, 如用户ID，渠道代码，登陆令牌（token）等
	// customParams  该字符串是游戏发起登陆调用login时传入的，对登陆本身没有任何作用，只是为游戏配对发起登陆和登陆成功两个动作的
	void onLoginSuccess(User user, const char *customParams) {
		CCACUserLoginResponse userLoginResponse;
		// userLoginResponse.uidStr = user.getUserId();
		userLoginResponse.uid = atoll(user.getUserId());
		userLoginResponse.channel = user.getChannelId();
		userLoginResponse.channelUid = user.getChannelUserId();
		userLoginResponse.accessToken = user.getToken();
		userLoginResponse.nickName = user.getUserName();
		userLoginResponse.productId = atoi(user.getProductId());
		LOGD("onLoginSucceeded start");
		CCAccessSDK::sharedInstance()->sdkCallback->onLoginSucceeded(userLoginResponse);
		LOGD("onLoginSucceeded end");
	}

	// 当发起登陆失败时，系统会调用该函数，通知游戏登陆失败
	// reason  登陆失败原因
	// customParams 同onLoginSuccess中定义
	void onLoginFailed(const char *reason, const char *customParams) {
		CCACReturnValue returnValue;
		returnValue.code = "";
		returnValue.desc = reason;
		returnValue.group = "";
		CCAccessSDK::sharedInstance()->sdkCallback->onLoginFailed(returnValue);
	}

	// 当游戏中用户点击登出 游戏调用logout发起登出后，登出完成后系统会调用该函数
	// 当用户点击渠道悬浮窗中的登出按钮后，完成登出后系统会调用该函数
	// 开发者应该在该函数中实现用户在游戏中的登出逻辑
	// customParams 同onLoginSuccess中定义
	void onLogout(const char *customParams) {
		CCAccessSDK::sharedInstance()->sdkCallback->onLogout(customParams);
	}
};

//android pay callback
class PlatformAndroidPay: public PayCallBack {

public:
	void onPaySuccess(const char *sucessInfo) {
		CCAccessSDK::sharedInstance()->sdkCallback->onPaySuccess(sucessInfo);
	}
	void onPayFail(const char *failInfo) {
		CCAccessSDK::sharedInstance()->sdkCallback->onPayFail(failInfo);
	}
};

//android exit callback
class PlatformAndroidExit: public ExitCallBack {

public:
	void onGameExit(const char *info) {
		CCAccessSDK::sharedInstance()->sdkCallback->onGameExit(info);
	}
	void onCustomExit(const char *info) {
		CCAccessSDK::sharedInstance()->sdkCallback->onChannelExit(info);
	}
};

static void printServerInfoList(list<CCACServerInfo> &list) {
	std::list<CCACServerInfo>::iterator it;
	for (it = list.begin(); it != list.end(); ++it) {
		CCACServerInfo si = *it;
		LOGI("serverInfos: id=%s,product=%d,name=%s,addr=%s,status=%s,order=%d,type=%d,hot=%d,des=%s,open=%d,url=%s", si.serverId.c_str(), si.productId, si.serverName.c_str(),
				si.serverAddress.c_str(), si.status.c_str(), si.order, 1, si.hot, si.description.c_str(), si.purchaseOpen, si.purchaseNotifyUri.c_str());
	}
}

//android getServerList callback
class PlatformAndroidGetServerList: public GetServerListCallback {
public:
	void onSuccess(CCACServerInfos serverInfos) {
		printServerInfoList(serverInfos.serverList);
		printServerInfoList(serverInfos.userServerList);

		CCAccessSDK::sharedInstance()->sdkCallback->onGetServerListSuccess(serverInfos);
	}
	void onFail(const char *code, const char *desc) {
		CCAccessSDK::sharedInstance()->sdkCallback->onGetServerListFail(desc);
	}
};

//android getServerList callback
class PlatformAndroidSelectServer: public SelectServerCallback {
	void onSuccess() {
		CCAccessSDK::sharedInstance()->sdkCallback->onSelectServerSuccess("");
	}
	void onFail(const char *code, const char *desc) {
		CCAccessSDK::sharedInstance()->sdkCallback->onSelectServerFail(desc);
	}
};

//android  callback
class PlatformAndroidSendBIData: public SendBICallback {
	void onSendResult(const char *msg) {
		CCAccessSDK::sharedInstance()->sdkCallback->onSendBIResult(msg);
	}
};

/** ———————————————————————————————————————————————— **/

CCAccessSDK* CCAccessSDK::sharedInstance() {
	static CCAccessSDK *platform_pInstance;
	if (platform_pInstance == NULL) {
		platform_pInstance = new CCAccessSDK();
	}
	return platform_pInstance;
}

void CCAccessSDK::initSDK(CCAccessSDKCallback* sdkCallback) {
	CCAccessSDK::sharedInstance()->sdkCallback = sdkCallback;
	GameProxy::ApplicationInit(new PlatformAndroidInit(), new PlatformAndroidLogin(), new PlatformAndroidPay());
}

void CCAccessSDK::checkUpdate(string version) {
}

void CCAccessSDK::login(string customParam) {
	GameProxy::Login(customParam.c_str());
}

void CCAccessSDK::logout() {
	const char *customParams = "logout";
	GameProxy::Logout(customParams);
}

void CCAccessSDK::switchAccount() {
	logout();
}

void CCAccessSDK::accountManager() {
	const char *customParams = "accountManage";
	GameProxy::AccountManage(customParams);
}

// bool CCAccessSDK::hasAccountManage() {
// 	return GameProxy::hasAccountManage();
// }

void CCAccessSDK::pay(string itemName, int amount, string callbackInfo) {
	GameProxy::Pay(itemName.c_str(), amount, callbackInfo.c_str());
}

void CCAccessSDK::exitGame(string msg) {
	GameProxy::Exit(new PlatformAndroidExit());
}

void CCAccessSDK::enterBBS() {
}

void CCAccessSDK::setUserExtData(std::map<string, string> userExtData) {
	string json;
	for (map<string, string>::iterator i = userExtData.begin(); i != userExtData.end(); i++) {
		json += "\"" + i->first + "\":\"" + i->second + "\",";
	}
	json = json.substr(0, json.length() - 1);
	json = "{" + json + "}";
	LOGD("%s", json.c_str());
	GameProxy::SetUserExtData(json.c_str());
}

void CCAccessSDK::pushMsg(CCACPushMessage msg) {
	LOGD("pushMsg");
}

void CCAccessSDK::sendBIData(string metric, string jsonData) {
	GameProxy::sendBIData(metric.c_str(), jsonData.c_str(), new PlatformAndroidSendBIData());
}

void CCAccessSDK::getServerList(string version) {
	GameProxy::getServerList(version.c_str(), new PlatformAndroidGetServerList());
}

void CCAccessSDK::selectServer(string serverId) {
	GameProxy::selectServer(serverId.c_str(), new PlatformAndroidSelectServer());
}
