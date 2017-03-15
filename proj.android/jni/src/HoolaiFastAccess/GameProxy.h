#ifndef GAMEPROXY_H
#define GAMEPROXY_H
#include <string.h>
#include <string>
#include "CCACServerInfos.h"

class InitCallBack {
public:
	virtual ~InitCallBack() {
	}
	virtual void onInitSuccess(const char *sucessInfo) = 0;
	virtual void onInitFail(const char *failInfo) =0;
};

class PayCallBack {
public:
	virtual ~PayCallBack() {
	}
	virtual void onPaySuccess(const char *sucessInfo) = 0;
	virtual void onPayFail(const char *failInfo) =0;
};

class ExitCallBack {
public:
	virtual ~ExitCallBack() {
	}
	virtual void onGameExit(const char *info) = 0;
	virtual void onCustomExit(const char *info) = 0;
};

class GetServerListCallback {
public:
	virtual ~GetServerListCallback() {
	}
	virtual void onSuccess(CCACServerInfos serverInfos) = 0;
	virtual void onFail(const char *code, const char *desc) = 0;
};

class SelectServerCallback {
public:
	virtual ~SelectServerCallback() {
	}
	virtual void onSuccess() = 0;
	virtual void onFail(const char *code, const char *desc) = 0;
};

class SendBICallback {
public:
	virtual ~SendBICallback() {
	}
	virtual void onSendResult(const char *msg) = 0;
};

class User {
	std::string userId;
	std::string userName;
	std::string token;
	std::string channelId;
	std::string channelUserId;
	std::string productId;
public:
	void setUserId(std::string uid) {
		userId = uid;
	}
	void setUserName(std::string uname) {
		userName = uname;
	}
	void setToken(std::string tok) {
		token = tok;
	}
	void setChannelId(std::string chid) {
		channelId = chid;
	}
	void setChannelUserId(std::string cuid) {
		channelUserId = cuid;
	}
	void setProductId(std::string pid) {
		productId = pid;
	}
	const char *getUserId() const {
		return userId.c_str();
	}
	const char *getChannelUserId() const {
		return channelUserId.c_str();
	}
	const char *getUserName() const {
		return userName.c_str();
	}
	const char *getChannelId() const {
		return channelId.c_str();
	}
	const char *getToken() const {
		return token.c_str();
	}
	const char *getProductId() const {
		return productId.c_str();
	}

};
class LoginCallback {
public:
	virtual ~LoginCallback() {
	}
	// 当登陆成功时，系统会调用该注册的Listener实例的该方法
	// user  中包含了登陆用户的相关信息, 如用户ID，渠道代码，登陆令牌（token）等
	// customParams  该字符串是游戏发起登陆调用login时传入的，对登陆本身没有任何作用，只是为游戏配对发起登陆和登陆成功两个动作的
	virtual void onLoginSuccess(User user, const char *customParams) = 0;

	// 当发起登陆失败时，系统会调用该函数，通知游戏登陆失败
	// reason  登陆失败原因
	// customParams 同onLoginSuccess中定义
	virtual void onLoginFailed(const char *reason, const char *customParams) = 0;

	// 当游戏中用户点击登出 游戏调用logout发起登出后，登出完成后系统会调用该函数
	// 当用户点击渠道悬浮窗中的登出按钮后，完成登出后系统会调用该函数
	// 开发者应该在该函数中实现用户在游戏中的登出逻辑
	// customParams 同onLoginSuccess中定义
	virtual void onLogout(const char *customParams) = 0;
};

class GameProxy {
public:
	// 设置游戏初始化、登录、支付监听器
	static void ApplicationInit(InitCallBack *initCallBack, LoginCallback *listener, PayCallBack *payCallBack);

	// 发起用户登陆，系统会调起相应渠道SDK的登陆界面进行登陆，登陆结果发送至用户事件监听器
	// customParams  该字符串只有在MSDK中才有效，true为QQ登录，false为微信登录，与其它渠道SDK登陆本身毫无关系，由开发者自行定义，该字符串会在登陆结果通知中完整传回用户事件监听器
	static void Login(const char *customParams);

	// 发起用户登出，并不是退出，只是退出本次登陆，系统会调用渠道SDK进行渠道用户登出，登出结果会发送给用户事件监听器
	// customParams  同Login中定义
	static void Logout(const char *customParams);

	// 发起用户充值，即用户不定额购买游戏道具或者货币，系统会调用对应渠道SDK充值界面，用户选择金额后进行充值，系统会将充值的数额和结果发送至游戏服务器
	// itemName  游戏道具名称
	// unitPrice  游戏道具单位价格，单位为人民币分
	// callbackInfo  由游戏开发者定义传入的字符串，回与支付结果一同发送给游戏服务器，游戏服务器可通过该字段判断交易的详细内容（金额 角色等）
	static void Pay(const char *itemName, int unitPrice, const char *callBackInfo);

	// 退出接口，当用户在游戏中需要退出游戏时调用该接口
	// 当渠道SDK提供了退出界面时，系统会调起渠道SDK退出界面，当用户在渠道SDK提供的退出界面中确定要退出游戏时，系统会调用 exitcallback->onGameExit()
	// 当渠道SDK没有提供退出界面时，系统会直接调用 exitcallback->onNo3rd()
	static void Exit(ExitCallBack *exitcallback);

	// 切换帐号接口
	static void Switch(const char *customParams);

	// 帐号管理接口
	static void AccountManage(const char *customParams);

	// 获取余额接口（MSDK）
	static int GetBalance(const char *customParams);

	static const char* GetAndroidManifestMeta(const char* name);

	static void SetUserExtData(const char *json);

	static void getServerList(const char *version, GetServerListCallback* callback);

	static void selectServer(const char *serverId, SelectServerCallback* callback);

	static void sendBIData(const char *metric, const char *jsonData, SendBICallback* callback);
};

#endif
