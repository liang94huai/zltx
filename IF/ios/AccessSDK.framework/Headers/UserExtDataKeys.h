//
//  UserExtDataKeys.h
//  accesssdk
//
//  Created by Hoolai on 15/3/23.
//  Copyright (c) 2015年 wsj_hoolai. All rights reserved.
//

#import <Foundation/Foundation.h>

extern NSString* const ACTION;//进入游戏(必填) 可选值为 ACTION_ENTER_SERVER／ACTION_LEVEL_UP／ACTION_CREATE_ROLE
extern NSString* const ACTION_ENTER_SERVER;//进入服务器
extern NSString* const ACTION_LEVEL_UP;//角色升级
extern NSString* const ACTION_CREATE_ROLE;//创建角色

extern NSString* const ROLE_ID;//角色id(必填)
extern NSString* const ROLE_NAME;//角色昵称(必填)
extern NSString* const ROLE_LEVEL;//角色等级(必填)
extern NSString* const ZONE_ID;//服务器ID(必填)
extern NSString* const ZONE_NAME;//服务器名称(必填)
extern NSString* const BALANCE;//代币余额(选填)
extern NSString* const VIP;//vip等级(选填)
extern NSString* const PARTYNAME;//公会(选填)

extern NSString* const APP_VERSION;//版本号(选填)
extern NSString* const APP_RES_VERSION;//资源版本号(选填)

@interface UserExtDataKeys : NSObject

@end
