//
//  accesssdk.h
//  accesssdk
//
//  Created by zhao shuan on 11/17/14.
//  Copyright (c) 2014 zs_hoolai. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AccessAppDelegate.h"
#import "ACUserLoginResponse.h"
#import "ACReturnValue.h"
#import "ACServerInfo.h"
#import "ServerInfos.h"
#import "ACPushMessage.h"

@protocol AccessSDKDelegate <NSObject>

@required
/**
 *  初始化成功回调
 *
 *  @param result 回调信息描述
 */
- (void)onInitSucceeded:(NSString *) result;

/**
 *  初始化失败回调
 *
 *  @param result 回调信息描述
 */
- (void)onInitFailed:(NSString *) result;

/**
 *  用户登陆成功回调
 *
 *  @param userLoginResponse 用户信息
 */
- (void)onLoginSucceeded:(ACUserLoginResponse*) userLoginResponse;

/**
 *  用户登陆失败回调
 *
 *  @param returnValue 服务器返回失败信息
 */
- (void)onLoginFailed:(ACReturnValue*) returnValue;

/**
 *  用户登出
 *
 *  @param msg 登出信息描述
 */
- (void)onLogout:(NSString*)msg;

/**
 *  支付成功回调
 *
 *  @param param 支付信息描述
 */
- (void)onPaySuccess:(NSString*)param;

/**
 *  支付失败回调
 *
 *  @param param 支付信息描述
 */
- (void)onPayFail:(NSString*)param;

/**
 *  支付取消回调（暂时不用，预留）
 *
 *  @param description 描述
 */
- (void)onPayCancle:(NSString*)description;


/**
 *  游戏退出回调
 *
 *  @param msg 描述
 */
- (void)onExit:(NSString*)msg;

@optional
/**
 *  获取服务器列表成功回调
 *
 *  @param serverInfos 服务器列表信息
 */
- (void)onGetServerListSuccess:(ServerInfos*)serverInfos;

/**
 *  获取服务器列表失败回调
 *
 *  @param msg 失败信息
 */
- (void)onGetServerListFail:(NSString*)msg;

/**
 *  选服成功回调
 *
 *  @param serverId 选服id
 */
- (void)onSelectServerSuccess:(NSString*)serverId;

/**
 *  选服失败回调
 *
 *  @param serverId 选服id
 */
- (void)onSelectServerFail:(NSString*)serverId;


/**
 *  报送BI回调
 *
 *  @param msg 报送结果
 */
- (void)onSendBIResult:(NSString*)msg;


/**
 *  绑定手机回调
 *
 *  @param msg 信息
 */
- (void)onBindPhone:(NSString*)msg;


/**
 服务器维护回调（返回true默认使用系统UI，返回false不弹框游戏自己定制）
 
 @param msg 维护提示信息
 */
- (BOOL)onMaintenance:(NSString*)msg;

@end

typedef enum _AC_LOGIN_STATE {
    kLOGIN_STATE_NOT_LOGIN = 0,
    kLOGIN_STATE_GUEST_LOGIN,
    kLOGIN_STATE_NORMAL_LOGIN
} AC_LOGIN_STATE;

@interface AccessSDK : NSObject

@property(nonatomic, assign) id<AccessSDKDelegate> delegate;

@property(nonatomic, readonly)AC_LOGIN_STATE currentLoginState;

@property(nonatomic, readonly)ACUserLoginResponse* userInfo;

+ (AccessSDK *)sharedInstance;

/**
 *  初始化接口，必须在设置完delegate后调用
 *  自动登陆
 */
- (void)initSDK;

/**
 *  初始化接口，必须在设置完delegate后调用
 *  自动登陆：YES  在sdk有账户的前提下调用登陆不弹登陆界面直接后台登陆并回调，没有账户直接弹出登陆框
 *  非自动登陆：NO 在有没有保存的账户是都弹出登陆界面
 *
 *  @param withSDKAutoLogin 是否自动登陆
 */
- (void)initSDK:(BOOL)withSDKAutoLogin;

/**
 *  版本更新
 *
 *  @param version 客户端版本号，不能为空
 */
- (void)checkUpdate:(NSString*)version;

/**
 *  登陆
 */
- (void)login;

/**
 *  登出
 */
- (void)logout;

/**
 *  账号管理
 */
- (void)accountManager;

/**
 *  支付
 *
 *  @param itemName     商品名，不能为空
 *  @param amount       支付金额（单位：分）
 *  @param callbackInfo 透传参数，不能为空
 */
- (void)payName:(NSString*)itemName amount:(int)amount callbackInfo:(NSString*)callbackInfo;

/**
 *  切换账号
 */
- (void)switchAccount;

/**
 *  退出游戏，不带确认框
 *
 *  @param msg
 */
- (void)exitGame:(NSString*)msg;

/**
 *  退出游戏，带确认框
 *
 *  @param msg
 */
- (void)exitGameDialog:(NSString *)msg;

/**
 *  获取服务器列表
 *
 *  @param version 服务器版本
 */
- (void)getServerList:(NSString*)version;

/**
 *  选服
 *
 *  @param selectServerId 服务器id，不能为空
 */
- (void)selectServer:(NSString*)selectServerId;

/**
 *  上报用户信息至SDK
 *
 *  @param userExtData 用户信息，不能为空
 */
- (void)setUserExtData:(NSMutableDictionary*)userExtData;

/**
 *  BI数据报送
 *
 *  @param metric   报送指标类型，不能为空
 *  @param jsonData 报送指标json串，不能为空
 */
- (void)sendBIData:(NSString*)metric jsonData:(NSString*)jsonData;

/**
 *  打开社区
 */
- (void)enterBBS;

/**
 *  推送
 *
 *  @param message 推送配置
 */
- (void)pushMsg:(ACPushMessage*)message;

/*
 * 获取渠道
 *
 */
- (NSString*)channel;

/*
 * 获取BI渠道
 *
 */
- (NSString*)biChannel;

@end
