#include <android/log.h>
#include <jni.h>
#include "GameProxy.h"
#include <iostream>
#include "CCACServerInfos.h"
#include <list>

#define LOG_TAG "JniSDK"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

JavaVM *gs_jvm = NULL;
jobject jniContext;

extern "C" {

JNIEXPORT void JNICALL Java_com_hoolai_open_fastaccess_proxy_JniHelper_prepared(JNIEnv *, jobject, jobject, jobject);

JNIEXPORT void JNICALL Java_com_hoolai_open_fastaccess_proxy_JniHelper_onLoginSuccess(JNIEnv *, jclass, jobject, jobject);

JNIEXPORT void JNICALL Java_com_hoolai_open_fastaccess_proxy_JniHelper_onLoginFailed(JNIEnv *, jclass, jstring, jobject);

JNIEXPORT void JNICALL Java_com_hoolai_open_fastaccess_proxy_JniHelper_onLogout(JNIEnv *, jclass, jobject);

JNIEXPORT void JNICALL Java_com_hoolai_open_fastaccess_proxy_JniHelper_payCallBackSuccess(JNIEnv *, jclass, jobject);

JNIEXPORT void JNICALL Java_com_hoolai_open_fastaccess_proxy_JniHelper_payCallBackFail(JNIEnv *, jclass, jobject);

JNIEXPORT void JNICALL Java_com_hoolai_open_fastaccess_proxy_JniHelper_exitCallBackOnExit(JNIEnv *, jclass, jobject);

JNIEXPORT void JNICALL Java_com_hoolai_open_fastaccess_proxy_JniHelper_exitCallBackOnCustomExit(JNIEnv *, jclass, jobject);

JNIEXPORT void JNICALL Java_com_hoolai_open_fastaccess_proxy_JniHelper_onInitSuccess(JNIEnv *, jclass, jobject);

JNIEXPORT void JNICALL Java_com_hoolai_open_fastaccess_proxy_JniHelper_onInitFail(JNIEnv *, jclass, jobject);

JNIEXPORT void JNICALL Java_com_hoolai_open_fastaccess_proxy_JniHelper_onGetServerListSuccess(JNIEnv *, jclass, jobjectArray, jobjectArray);
JNIEXPORT void JNICALL Java_com_hoolai_open_fastaccess_proxy_JniHelper_onGetServerListFail(JNIEnv *, jclass, jstring, jstring);

JNIEXPORT void JNICALL Java_com_hoolai_open_fastaccess_proxy_JniHelper_onSelectServerSuccess(JNIEnv *, jclass);
JNIEXPORT void JNICALL Java_com_hoolai_open_fastaccess_proxy_JniHelper_onSelectServerFail(JNIEnv *, jclass, jstring, jstring);

JNIEXPORT void JNICALL Java_com_hoolai_open_fastaccess_proxy_JniHelper_onSendBIDataResult(JNIEnv *, jclass, jstring);

//JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
//	gs_jvm = vm;
//	return JNI_VERSION_1_4;
//}
}

static bool getEnv(JNIEnv **env) {
	bool bRet = false;
	do {
		if (gs_jvm->GetEnv((void**) env, JNI_VERSION_1_4) != JNI_OK) {
			LOGD("Failed to get the environment using GetEnv()");
			break;
		}
		if (gs_jvm->AttachCurrentThread(env, 0) < 0) {
			LOGD("Failed to get the environment using AttachCurrentThread()");
			break;
		}
		bRet = true;
	} while (0);
	return bRet;
}

jobject GlobalGameProxyObject;

jmethodID login;
jmethodID logout;
jmethodID applicationInit;
jmethodID setXMRoleData;
jmethodID setExtRoleData;

PayCallBack *payCallBack;
InitCallBack *initCallBack;
LoginCallback *loginCallBack;
ExitCallBack *exitCallBack;
GetServerListCallback *getServerListCallback;
SelectServerCallback *selectServerCallback;
SendBICallback *sendBICallback;

static std::string jstringTostr(JNIEnv* env, jstring jstr) {
	if (jstr == NULL) {
		return "";
	}
	const char* chars = env->GetStringUTFChars(jstr, NULL);
	std::string ret(chars);
	env->ReleaseStringUTFChars(jstr, chars);
	return ret;
}

void Java_com_hoolai_open_fastaccess_proxy_JniHelper_prepared(JNIEnv * env, jobject thiz, jobject context, jobject gameProxyObject) {
	env->GetJavaVM(&gs_jvm);
	jniContext = env->NewGlobalRef(context);
	GlobalGameProxyObject = env->NewGlobalRef(gameProxyObject);
	LOGD("prepared 完成");
}

void Java_com_hoolai_open_fastaccess_proxy_JniHelper_onLoginSuccess(JNIEnv* env, jclass clazz, jobject xmUser, jobject customParams) {
	LOGD("onLoginSuccess 开始");
	jclass XMUserClass = env->FindClass("com/hoolai/open/fastaccess/proxy/User");
	jmethodID getUserID = env->GetMethodID(XMUserClass, "getUserID", "()Ljava/lang/String;");
	jmethodID getChannelUserID = env->GetMethodID(XMUserClass, "getChannelUserId", "()Ljava/lang/String;");
	jmethodID getChannelID = env->GetMethodID(XMUserClass, "getChannelID", "()Ljava/lang/String;");
	jmethodID getUsername = env->GetMethodID(XMUserClass, "getUsername", "()Ljava/lang/String;");
	jmethodID getToken = env->GetMethodID(XMUserClass, "getToken", "()Ljava/lang/String;");
	jmethodID getProductId = env->GetMethodID(XMUserClass, "getProductId", "()Ljava/lang/String;");

	jstring userId = static_cast<jstring>(env->CallObjectMethod(xmUser, getUserID));
	jstring channelUserId = static_cast<jstring>(env->CallObjectMethod(xmUser, getChannelUserID));
	jstring channelId = static_cast<jstring>(env->CallObjectMethod(xmUser, getChannelID));
	jstring userName = static_cast<jstring>(env->CallObjectMethod(xmUser, getUsername));
	jstring token = static_cast<jstring>(env->CallObjectMethod(xmUser, getToken));
	jstring productId = static_cast<jstring>(env->CallObjectMethod(xmUser, getProductId));
	LOGI("%1$s", jstringTostr(env, userId).c_str());
	LOGI("%1$s", jstringTostr(env, channelUserId).c_str());
	LOGI("%1$s", jstringTostr(env, channelId).c_str());
	LOGI("%1$s", jstringTostr(env, userName).c_str());
	LOGI("%1$s", jstringTostr(env, token).c_str());
	LOGI("%1$s", jstringTostr(env, productId).c_str());

	User user;
	user.setUserId(jstringTostr(env, userId).c_str());
	user.setChannelUserId(jstringTostr(env, channelUserId).c_str());
	user.setUserName(jstringTostr(env, userName).c_str());
	user.setChannelId(jstringTostr(env, channelId).c_str());
	user.setToken(jstringTostr(env, token).c_str());
	user.setProductId(jstringTostr(env, productId).c_str());

	loginCallBack->onLoginSuccess(user, jstringTostr(env, (jstring) customParams).c_str());
	LOGD("onLoginSuccess 结束");
}
;

void Java_com_hoolai_open_fastaccess_proxy_JniHelper_onLoginFailed(JNIEnv* env, jclass clazz, jstring reason, jobject customParams) {
	loginCallBack->onLoginFailed(jstringTostr(env, reason).c_str(), jstringTostr(env, (jstring) customParams).c_str());
}
;

void Java_com_hoolai_open_fastaccess_proxy_JniHelper_onLogout(JNIEnv* env, jclass clazz, jobject customParams) {
	loginCallBack->onLogout(jstringTostr(env, (jstring) customParams).c_str());
}
;

void Java_com_hoolai_open_fastaccess_proxy_JniHelper_payCallBackSuccess(JNIEnv* env, jclass clazz, jobject successinfo) {
	LOGD("JniHelper_payCallBackSuccess 进行中");
	payCallBack->onPaySuccess(jstringTostr(env, (jstring) successinfo).c_str());
}
void Java_com_hoolai_open_fastaccess_proxy_JniHelper_payCallBackFail(JNIEnv* env, jclass clazz, jobject failinfo) {
	payCallBack->onPayFail(jstringTostr(env, (jstring) failinfo).c_str());
}

void Java_com_hoolai_open_fastaccess_proxy_JniHelper_onInitSuccess(JNIEnv* env, jclass clazz, jobject info) {
	LOGD("JniHelper_onInitSuccess 进行中");
	initCallBack->onInitSuccess(jstringTostr(env, (jstring) info).c_str());
}

void Java_com_hoolai_open_fastaccess_proxy_JniHelper_onInitFail(JNIEnv* env, jclass clazz, jobject info) {
	LOGD("JniHelper_onInitFail 进行中");
	initCallBack->onInitFail(jstringTostr(env, (jstring) info).c_str());
}

void Java_com_hoolai_open_fastaccess_proxy_JniHelper_exitCallBackOnExit(JNIEnv* env, jclass clazz, jobject info) {
	exitCallBack->onGameExit(jstringTostr(env, (jstring) info).c_str());
}

void Java_com_hoolai_open_fastaccess_proxy_JniHelper_exitCallBackOnCustomExit(JNIEnv* env, jclass clazz, jobject info) {
	exitCallBack->onCustomExit(jstringTostr(env, (jstring) info).c_str());
}

static void convertToServerInfo(JNIEnv *env, jobject jObj, CCACServerInfo &serverInfo) {
	jclass serverInfoClass = env->FindClass("com/hoolai/open/fastaccess/proxy/ServerInfoNative");
	jmethodID getServerId = env->GetMethodID(serverInfoClass, "getServerId", "()Ljava/lang/String;");
	jmethodID getProductId = env->GetMethodID(serverInfoClass, "getProductId", "()I");
	jmethodID getServerAddress = env->GetMethodID(serverInfoClass, "getServerAddress", "()Ljava/lang/String;");
	jmethodID getServerName = env->GetMethodID(serverInfoClass, "getServerName", "()Ljava/lang/String;");
	jmethodID getServerStatus = env->GetMethodID(serverInfoClass, "getServerStatus", "()Ljava/lang/String;");
	jmethodID getServerOrder = env->GetMethodID(serverInfoClass, "getServerOrder", "()I");
	jmethodID getServerType = env->GetMethodID(serverInfoClass, "getServerType", "()I");
	jmethodID getHot = env->GetMethodID(serverInfoClass, "getHot", "()I");
	jmethodID getDescription = env->GetMethodID(serverInfoClass, "getDescription", "()Ljava/lang/String;");
	jmethodID getPurchaseOpen = env->GetMethodID(serverInfoClass, "isPurchaseOpen", "()Z");
	jmethodID getPurchaseNotifyUrl = env->GetMethodID(serverInfoClass, "getPurchaseNotifyUrl", "()Ljava/lang/String;");

	jstring serverId = static_cast<jstring>(env->CallObjectMethod(jObj, getServerId));
	int productId = env->CallIntMethod(jObj, getProductId);
	jstring serverAddress = static_cast<jstring>(env->CallObjectMethod(jObj, getServerAddress));
	jstring serverName = static_cast<jstring>(env->CallObjectMethod(jObj, getServerName));
	jstring serverStatus = static_cast<jstring>(env->CallObjectMethod(jObj, getServerStatus));
	int serverOrder = env->CallIntMethod(jObj, getServerOrder);
	int serverType = env->CallIntMethod(jObj, getServerType);
	int hot = env->CallIntMethod(jObj, getHot);
	jstring description = static_cast<jstring>(env->CallObjectMethod(jObj, getDescription));
	bool purchaseOpen = env->CallBooleanMethod(jObj, getPurchaseOpen);
	jstring purchaseNotifyUrl = static_cast<jstring>(env->CallObjectMethod(jObj, getPurchaseNotifyUrl));

	serverInfo.serverId = jstringTostr(env, serverId);
	serverInfo.productId = productId;
	serverInfo.serverAddress = jstringTostr(env, serverAddress);
	serverInfo.serverName = jstringTostr(env, serverName);
	serverInfo.status = jstringTostr(env, serverStatus);
	serverInfo.order = serverOrder;
//	serverInfo.serverType = jstringTostr(env, serverType);
	serverInfo.hot = hot;
	serverInfo.description = jstringTostr(env, description);
	serverInfo.purchaseOpen = purchaseOpen;
	serverInfo.purchaseNotifyUri = jstringTostr(env, purchaseNotifyUrl);
}

static void addToList(JNIEnv *env, jobjectArray array, std::list<CCACServerInfo> &list) {
	int num = env->GetArrayLength(array);
	for (int i = 0; i < num; ++i) {
		jobject jObj = env->GetObjectArrayElement(array, i);
		CCACServerInfo serverInfo;
		convertToServerInfo(env, jObj, serverInfo);
		list.push_back(serverInfo);
	}
}

void Java_com_hoolai_open_fastaccess_proxy_JniHelper_onGetServerListSuccess(JNIEnv *env, jclass clazz, jobjectArray allArray, jobjectArray userArray) {
	LOGI("onGetServerListSuccess");
	CCACServerInfos serverInfos;

	std::list<CCACServerInfo> list1;
	addToList(env, allArray, list1);
	serverInfos.serverList = list1;

	std::list<CCACServerInfo> list2;
	addToList(env, userArray, list2);
	serverInfos.userServerList = list2;
	getServerListCallback->onSuccess(serverInfos);
}

void Java_com_hoolai_open_fastaccess_proxy_JniHelper_onGetServerListFail(JNIEnv *env, jclass clazz, jstring code, jstring desc) {
	LOGI("onGetServerListFail: code=%s,desc=%s", jstringTostr(env, code).c_str(), jstringTostr(env, desc).c_str());
	getServerListCallback->onFail(jstringTostr(env, code).c_str(), jstringTostr(env, desc).c_str());
}

void Java_com_hoolai_open_fastaccess_proxy_JniHelper_onSelectServerSuccess(JNIEnv *env, jclass clazz) {
	LOGI("onSelectServerSuccess");
	selectServerCallback->onSuccess();
}

void Java_com_hoolai_open_fastaccess_proxy_JniHelper_onSelectServerFail(JNIEnv *env, jclass clazz, jstring code, jstring desc) {
	LOGI("onSelectServerFail: code=%s,desc=%s", jstringTostr(env, code).c_str(), jstringTostr(env, desc).c_str());
	selectServerCallback->onFail(jstringTostr(env, code).c_str(), jstringTostr(env, desc).c_str());
}

void Java_com_hoolai_open_fastaccess_proxy_JniHelper_onSendBIDataResult(JNIEnv *env, jclass clazz, jstring msg) {
	LOGI("onSendBIDataResult: %s", jstringTostr(env, msg).c_str());
	sendBICallback->onSendResult(jstringTostr(env, msg).c_str());
}

void GameProxy::ApplicationInit(InitCallBack *initCallBackLocal, LoginCallback *listener, PayCallBack *payCallBackLocal) {
	LOGD("applicationInit 开始");
	loginCallBack = listener;
	payCallBack = payCallBackLocal;
	initCallBack = initCallBackLocal;

	JNIEnv *jniEnv = 0;
	if (!getEnv(&jniEnv)) {
		__android_log_print(ANDROID_LOG_WARN, "JNIMsg", "getEnv 失败！");
		return;
	}
	jclass native_clazz = jniEnv->GetObjectClass(GlobalGameProxyObject);
	if (applicationInit == NULL) {
		applicationInit = jniEnv->GetMethodID(native_clazz, "executeMethod",
				"(Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
	}
	jclass initCls = jniEnv->FindClass("com/hoolai/open/fastaccess/proxy/InitCallBackNativeImpl");
	jmethodID initMethodId = jniEnv->GetMethodID(initCls, "<init>", "()V");
	jobject initCallBack = jniEnv->NewObject(initCls, initMethodId);

	jclass loginCls = jniEnv->FindClass("com/hoolai/open/fastaccess/proxy/LoginCallBackNativeImpl");
	jmethodID loginMethodId = jniEnv->GetMethodID(loginCls, "<init>", "()V");
	jobject loginCallBack = jniEnv->NewObject(loginCls, loginMethodId);

	jclass payCls = jniEnv->FindClass("com/hoolai/open/fastaccess/proxy/PayCallBackNativeImpl");
	jmethodID payMethodId = jniEnv->GetMethodID(payCls, "<init>", "()V");
	jobject payCallBack = jniEnv->NewObject(payCls, payMethodId);

	jstring methodName = jniEnv->NewStringUTF("applicationInit");
	jniEnv->CallObjectMethod(GlobalGameProxyObject, applicationInit, methodName, jniContext, initCallBack, loginCallBack, payCallBack);
	jniEnv->DeleteLocalRef(methodName);
	LOGD("applicationInit 完毕");
}
;

void GameProxy::Login(const char *customParams) {
	JNIEnv *jniEnv = 0;
	if (!getEnv(&jniEnv)) {
		__android_log_print(ANDROID_LOG_WARN, "JNIMsg", "getEnv 失败！");
		return;
	}

	jclass native_clazz = jniEnv->GetObjectClass(GlobalGameProxyObject);
	jmethodID login = jniEnv->GetMethodID(native_clazz, "executeMethod",
			"(Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

	jstring methodName = jniEnv->NewStringUTF("login");
	jstring jCustomParams = jniEnv->NewStringUTF(customParams);
	jniEnv->CallObjectMethod(GlobalGameProxyObject, login, methodName, jniContext, jCustomParams, NULL, NULL);
	jniEnv->DeleteLocalRef(methodName);
	jniEnv->DeleteLocalRef(jCustomParams);
	LOGD("调用Login方法完毕");
}

void GameProxy::Logout(const char *customParams) {
	JNIEnv *jniEnv = 0;
	if (!getEnv(&jniEnv)) {
		__android_log_print(ANDROID_LOG_WARN, "JNIMsg", "getEnv 失败！");
		return;
	}

	jclass native_clazz = jniEnv->GetObjectClass(GlobalGameProxyObject);
	jmethodID logout = jniEnv->GetMethodID(native_clazz, "executeMethod",
			"(Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

	jstring methodName = jniEnv->NewStringUTF("logout");
	jniEnv->CallObjectMethod(GlobalGameProxyObject, logout, methodName, jniContext, NULL, NULL, NULL);
	jniEnv->DeleteLocalRef(methodName);
	LOGD("调用 logout 方法完毕");
}

void GameProxy::Pay(const char *itemName, int unitPrice, const char *callBackInfo) {
	JNIEnv *jniEnv = 0;
	if (!getEnv(&jniEnv)) {
		__android_log_print(ANDROID_LOG_WARN, "JNIMsg", "getEnv 失败！");
		return;
	}

	jclass objClass = jniEnv->FindClass("java/lang/Object");
	jclass native_clazz = jniEnv->GetObjectClass(GlobalGameProxyObject);
	jmethodID pay = jniEnv->GetMethodID(native_clazz, "executeMethod",
			"(Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

	jclass integerClass = jniEnv->FindClass("java/lang/Integer");
	jmethodID integerM = jniEnv->GetMethodID(integerClass, "<init>", "(I)V");
	jobject integerO = jniEnv->NewObject(integerClass, integerM, unitPrice);

	jstring methodName = jniEnv->NewStringUTF("pay");
	jstring jItemName = jniEnv->NewStringUTF(itemName);
	jstring jCallBackInfo = jniEnv->NewStringUTF(callBackInfo);
	jniEnv->CallObjectMethod(GlobalGameProxyObject, pay, methodName, jniContext, jItemName, integerO, jCallBackInfo);
	jniEnv->DeleteLocalRef(methodName);
	jniEnv->DeleteLocalRef(jItemName);
	jniEnv->DeleteLocalRef(jCallBackInfo);
	LOGD("do charge,调用支付——结束");
}

void GameProxy::Exit(ExitCallBack *callback) {
	JNIEnv *jniEnv = 0;
	if (!getEnv(&jniEnv)) {
		__android_log_print(ANDROID_LOG_WARN, "JNIMsg", "getEnv 失败！");
		return;
	}

	exitCallBack = callback;

	jclass native_clazz = jniEnv->GetObjectClass(GlobalGameProxyObject);
	jmethodID exit = jniEnv->GetMethodID(native_clazz, "executeMethod",
			"(Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

	jclass ecbcls = jniEnv->FindClass("com/hoolai/open/fastaccess/proxy/ExitCallBackNativeImpl");
	jmethodID ecbid = jniEnv->GetMethodID(ecbcls, "<init>", "()V");
	jobject ecb = jniEnv->NewObject(ecbcls, ecbid);

	jstring methodName = jniEnv->NewStringUTF("exit");
	jniEnv->CallObjectMethod(GlobalGameProxyObject, exit, methodName, jniContext, NULL, ecb, NULL);
	jniEnv->DeleteLocalRef(methodName);
}

void GameProxy::Switch(const char *customParams) {
	JNIEnv *jniEnv = 0;
	if (!getEnv(&jniEnv)) {
		__android_log_print(ANDROID_LOG_WARN, "JNIMsg", "getEnv 失败！");
		return;
	}
	jclass native_clazz = jniEnv->GetObjectClass(GlobalGameProxyObject);
	jmethodID switchAccount = jniEnv->GetMethodID(native_clazz, "executeMethod",
			"(Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

	jstring methodName = jniEnv->NewStringUTF("switch");
	jniEnv->CallObjectMethod(GlobalGameProxyObject, switchAccount, methodName, jniContext, NULL, NULL, NULL);
	jniEnv->DeleteLocalRef(methodName);
	LOGD("调用Switch方法完毕");
}

void GameProxy::AccountManage(const char *customParams) {
	JNIEnv *jniEnv = 0;
	if (!getEnv(&jniEnv)) {
		__android_log_print(ANDROID_LOG_WARN, "JNIMsg", "getEnv 失败！");
		return;
	}
	jclass native_clazz = jniEnv->GetObjectClass(GlobalGameProxyObject);
	jmethodID accountManage = jniEnv->GetMethodID(native_clazz, "executeMethod",
			"(Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

	jstring methodName = jniEnv->NewStringUTF("accountManage");
	jniEnv->CallObjectMethod(GlobalGameProxyObject, accountManage, methodName, jniContext, NULL, NULL, NULL);
	jniEnv->DeleteLocalRef(methodName);
	LOGD("调用AccountManage方法完毕");
}

// bool GameProxy::hasAccountManage() {
// 	JNIEnv *jniEnv = 0;
// 	if (!getEnv(&jniEnv)) {
// 		__android_log_print(ANDROID_LOG_WARN, "JNIMsg", "getEnv 失败！");
// 		return false;
// 	}
// 	jclass native_clazz = jniEnv->GetObjectClass(GlobalGameProxyObject);
// 	jmethodID hasAccountManage = jniEnv->GetMethodID(native_clazz, "hasAccountManage", "()Z");
// 	jboolean value = jniEnv->CallBooleanMethod(GlobalGameProxyObject, hasAccountManage);
// 	return value;
// }

int GameProxy::GetBalance(const char *customParams) {
	JNIEnv *jniEnv = 0;
	if (!getEnv(&jniEnv)) {
		__android_log_print(ANDROID_LOG_WARN, "JNIMsg", "getEnv 失败！");
		return 0;
	}
	LOGD("GetBalance");
	jclass native_clazz = jniEnv->GetObjectClass(GlobalGameProxyObject);
	jmethodID getBalance = jniEnv->GetMethodID(native_clazz, "getBalance", "()I");

	int integer = jniEnv->CallIntMethod(GlobalGameProxyObject, getBalance);
	return integer;
}

const char* GameProxy::GetAndroidManifestMeta(const char* name) {
	JNIEnv *jniEnv = 0;
	if (!getEnv(&jniEnv)) {
		__android_log_print(ANDROID_LOG_WARN, "JNIMsg", "getEnv 失败！");
		return 0;
	}
	LOGD("GetAndroidManifestMeta");
	jclass native_clazz = jniEnv->GetObjectClass(GlobalGameProxyObject);
	jmethodID method = jniEnv->GetMethodID(native_clazz, "getMetaDataConfig", "(Landroid/content/Context;Ljava/lang/String;)Ljava/lang/String;");

	jstring jName = jniEnv->NewStringUTF(name);
	jobject value = jniEnv->CallObjectMethod(GlobalGameProxyObject, method, jniContext, jName);
	jniEnv->DeleteLocalRef(jName);
	return jstringTostr(jniEnv, (jstring) value).c_str();
}

void GameProxy::SetUserExtData(const char *json) {
	JNIEnv *jniEnv = 0;
	if (!getEnv(&jniEnv)) {
		__android_log_print(ANDROID_LOG_WARN, "JNIMsg", "getEnv 失败！");
		return;
	}
	LOGD("SetUserExtData");
	jclass native_clazz = jniEnv->GetObjectClass(GlobalGameProxyObject);
	jmethodID executeMethod = jniEnv->GetMethodID(native_clazz, "executeMethod",
				"(Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

	jstring jJson = jniEnv->NewStringUTF(json);
	jstring methodName = jniEnv->NewStringUTF("setUserExtData");
	jniEnv->CallObjectMethod(GlobalGameProxyObject, executeMethod, methodName, jniContext, jJson, NULL, NULL);
	jniEnv->DeleteLocalRef(jJson);
	jniEnv->DeleteLocalRef(methodName);
}

void GameProxy::getServerList(const char *version, GetServerListCallback* callback) {
	JNIEnv *jniEnv = 0;
	if (!getEnv(&jniEnv)) {
		__android_log_print(ANDROID_LOG_WARN, "JNIMsg", "getEnv 失败！");
		return;
	}
	getServerListCallback = callback;

	jclass native_clazz = jniEnv->GetObjectClass(GlobalGameProxyObject);
	jmethodID methodId = jniEnv->GetMethodID(native_clazz, "executeMethod",
			"(Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

	jclass ecbcls = jniEnv->FindClass("com/hoolai/open/fastaccess/proxy/GetServerListCallbackNativeImpl");
	jmethodID ecbid = jniEnv->GetMethodID(ecbcls, "<init>", "()V");
	jobject ecb = jniEnv->NewObject(ecbcls, ecbid);

	jstring methodName = jniEnv->NewStringUTF("getServerList");
	jstring jversion = jniEnv->NewStringUTF(version);
	jniEnv->CallObjectMethod(GlobalGameProxyObject, methodId, methodName, jniContext, jversion, ecb, NULL);
	jniEnv->DeleteLocalRef(methodName);
	jniEnv->DeleteLocalRef(jversion);
}

void GameProxy::selectServer(const char *serverId, SelectServerCallback* callback) {
	JNIEnv *jniEnv = 0;
	if (!getEnv(&jniEnv)) {
		__android_log_print(ANDROID_LOG_WARN, "JNIMsg", "getEnv 失败！");
		return;
	}
	selectServerCallback = callback;

	jclass native_clazz = jniEnv->GetObjectClass(GlobalGameProxyObject);
	jmethodID methodId = jniEnv->GetMethodID(native_clazz, "executeMethod",
			"(Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

	jclass ecbcls = jniEnv->FindClass("com/hoolai/open/fastaccess/proxy/SelectServerCallbackNativeImpl");
	jmethodID ecbid = jniEnv->GetMethodID(ecbcls, "<init>", "()V");
	jobject ecb = jniEnv->NewObject(ecbcls, ecbid);

	jstring methodName = jniEnv->NewStringUTF("selectServer");
	jstring jserverId = jniEnv->NewStringUTF(serverId);
	jniEnv->CallObjectMethod(GlobalGameProxyObject, methodId, methodName, jniContext, jserverId, ecb, NULL);
	jniEnv->DeleteLocalRef(methodName);
	jniEnv->DeleteLocalRef(jserverId);
}

void GameProxy::sendBIData(const char *metric, const char *jsonData, SendBICallback* callback) {
	JNIEnv *jniEnv = 0;
	if (!getEnv(&jniEnv)) {
		__android_log_print(ANDROID_LOG_WARN, "JNIMsg", "getEnv 失败！");
		return;
	}
	sendBICallback = callback;

	jclass native_clazz = jniEnv->GetObjectClass(GlobalGameProxyObject);
	jmethodID methodId = jniEnv->GetMethodID(native_clazz, "executeMethod",
			"(Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

	jclass ecbcls = jniEnv->FindClass("com/hoolai/open/fastaccess/proxy/SendBICallbackNativeImpl");
	jmethodID ecbid = jniEnv->GetMethodID(ecbcls, "<init>", "()V");
	jobject ecb = jniEnv->NewObject(ecbcls, ecbid);

	jstring methodName = jniEnv->NewStringUTF("sendBIData");
	jstring jmetric = jniEnv->NewStringUTF(metric);
	jstring jjsonData = jniEnv->NewStringUTF(jsonData);
	jniEnv->CallObjectMethod(GlobalGameProxyObject, methodId, methodName, jniContext, jmetric, jjsonData, ecb, NULL);
	jniEnv->DeleteLocalRef(methodName);
	jniEnv->DeleteLocalRef(jmetric);
	jniEnv->DeleteLocalRef(jjsonData);
}

