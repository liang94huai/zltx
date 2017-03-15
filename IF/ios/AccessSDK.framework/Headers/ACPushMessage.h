//
//  ACPushMessage.h
//  accesssdk
//
//  Created by Hoolai on 15/11/25.
//  Copyright © 2015年 wsj_hoolai. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ACPushMessage : NSObject

@property(nonatomic, readwrite) NSDate* fireDate; //推送时间
@property(nonatomic, readwrite) NSString* alertBody; //消息内容
@property(nonatomic, readwrite) NSInteger badge;//改变app角标，如果为－1，则不改变
@property(nonatomic, readwrite) NSMutableDictionary* userInfo;//自定义参数替
@property(nonatomic, readwrite) NSString* alertAction;//换弹框的按钮文字内容（默认为"启动"）

@end
