//
//  WbSocketControl.h
//  WBSOCKETDEMO
//
//  Created by 张世良 on 15/12/28.
//  Copyright © 2015年 apple. All rights reserved.
//
#import "ChatChannel.h"
#import "ChatChannel+ChatMessage.h"
#import "ChannelManager.h"
#import "ServiceInterface.h"
#import "NSString+Extension.h"
#import <Foundation/Foundation.h>
#import "SRWebSocket.h"
#import "WbSocketControl.h"
#import "AFNetworking.h"
#import "UserManager.h"
#import "InCommonUsed.h"
#import "CSMessage.h"
#import "CSMessage+Manager.h"
#define kHeartCount 60
#define kHeartBeat @"heartbeat"
#define cmd_login_success @"login.success"
#define cmd_push_chatroom @"push.chat.room"
#define cmd_getMsgAfterCreatTime @"history.roomsv2"
#define cmd_roomJoin @"room.join"
#define cmd_setUserInfo @"user.setInfo"

typedef enum : NSUInteger {
    /** 0. 获取ip请求失败 */
    WbSocketConnectStatus_GettingIPRequestError = 0,
    /** 1. ip解析失败 */
    WbSocketConnectStatus_GettingIPParsingError = 1,
    /** 2. ip解析成功 */
    WbSocketConnectStatus_gettingIPSuccessed = 2,
    /** 3. socket连接失败 */
    WbSocketConnectStatus_ConnectError = 3,
    /** 4. socket正在连接 */
    WbSocketConnectStatus_Connecting = 4,
    /** 5. socket连接成功 */
    WbSocketConnectStatus_ConnectSuccessed = 5

} WbSocketConnectStatus ;



typedef void(^WbSocketConnectStatusBlock)(WbSocketConnectStatus status ) ;

@interface WbSocketControl : NSObject <SRWebSocketDelegate>
{
    SRWebSocket * wbsocket;
    NSMutableURLRequest * urlRequest;
    NSString * addCountryTime;
    NSString * addAllianceTime;
}
@property(nonatomic,assign)BOOL isConnected;
@property(nonatomic,assign)BOOL isAddCountry;
@property(nonatomic,assign)BOOL isAddAlliance;
+(WbSocketControl*)sharedSingleControl;
-(void)webScoketConnectWithStatusBlock:(WbSocketConnectStatusBlock)vBlock;
-(void)wbSocketOpen;
-(void)wbSocketClose;
-(void)getFirstHistoryMessage;
-(void)wbSocketSendMessageSingle:(NSMutableDictionary*)resultDic;
-(void)wbSocketIntoGroupChatRoom:(NSMutableDictionary*)resultDic;
-(void)wbSocketSendMessageGroup:(NSMutableDictionary*)resultDic;
-(void)wbSocketOutGroupChatRoom:(NSMutableDictionary*)resultDic;
-(void)wbSocketChangRoomName:(NSMutableDictionary*)resultDic;
-(void)wbSocketInitWithHTTPHeaderAppid:(NSString*)appid time:(NSString*)time uid:(NSString*)uid url:(NSString * )url;

-(void)gettingFirstServerMessage;
@end
