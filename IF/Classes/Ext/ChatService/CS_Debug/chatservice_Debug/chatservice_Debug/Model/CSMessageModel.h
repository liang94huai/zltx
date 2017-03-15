//
//  CSMessageModel.h
//  chatservice_Debug
//
//  Created by 张彦涛 on 15/12/7.
//  Copyright © 2015年 张彦涛. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "ChatServiceManagerDefs.h"
#import "YYKit.h"
@class CSMessage;
@interface CSMessageModel : NSObject

/** 消息体 */
@property (nonatomic,strong) CSMessage  * message;
/** cell告诉 */
@property (nonatomic ,assign) CGFloat cellHeight;
/** 消息ID */
@property (strong, nonatomic ) NSString *messageId;
/** 消息服务器序号  */
@property (nonatomic,assign) long       sequenceId;
/** 消息发送状态 */
@property (nonatomic,assign) CSMessageSendState messageSendStatus;
/** 消息体类型 */
@property (nonatomic,assign) CSMessageBodyType messageBodyType;
/** 是否是当前登录者发送的消息 */
@property (nonatomic) BOOL isSelfSender;
/** 是否显示昵称 */
@property (nonatomic,assign)BOOL isNickNameShow;
/** 消息显示的昵称 */
@property (strong, nonatomic) NSString *nickname;
/** 消息显示的头像的网络地址 */
@property (strong, nonatomic) NSString *avatarCustomURLPath;
/** 消息显示的默认头像 */
@property (strong, nonatomic) NSString *avatarDefaultImageName;
/** 是否显示时间 */
@property (assign,nonatomic)BOOL isTimeLabelShow;
/** 消息时间 */
@property (strong,nonatomic)NSString *timeString;

//****************消息体相关*****************
//文本消息：文本
/** 现实的消息 */
@property (strong, nonatomic) NSString *showContentsString;
@property (strong,nonatomic)NSMutableAttributedString * showAttContentsString;
/**  翻译出处 */
@property (strong,nonatomic)NSString *translatExplainString;
/** 内容的size */
@property (nonatomic,assign) CGSize  showContentsSize;
/** 时间的size */
@property (nonatomic,assign) CGSize  showTranslatExplainSize;
/** bubble 的contents size */
@property (assign,nonatomic) CGSize  textBubbleSize;
/** bubble 的backImage size */
@property (assign,nonatomic) CGSize textBubbleBackImageSize;

- (instancetype)initWithMessage:(CSMessage *)message;
-(void)calculateWithTextBubbleView;
@end
