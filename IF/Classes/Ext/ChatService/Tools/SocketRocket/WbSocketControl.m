//
//  WbSocketControl.m
//  WBSOCKETDEMO
//
//  Created by 张世良 on 15/12/28.
//  Copyright © 2015年 apple. All rights reserved.
//

#import "WbSocketControl.h"
#import "UserManager.h"
#import "NSString+Cocos2dHelper.h"
#import "NSDate+Extension.h"
static WbSocketControl *singleton = nil;
@implementation WbSocketControl
+(WbSocketControl*)sharedSingleControl
{
    @synchronized(self) {
        static dispatch_once_t predicate;
        dispatch_once(&predicate, ^{
            if (singleton==nil) {
                singleton=[[WbSocketControl alloc]init];
            }

        });
//        if (singleton==nil) {
//            singleton=[[WbSocketControl alloc]init];
//        }
    }
    return singleton;
    
//    static Cellconfig *sharedCellConfig = nil;
//    static dispatch_once_t predicate;
//    dispatch_once(&predicate, ^{
//        sharedCellConfig = [[Cellconfig alloc] _init];
//    });
//    return sharedCellConfig;
}

-(void)webScoketConnectWithStatusBlock:(WbSocketConnectStatusBlock)vBlock
{
    self.isConnected=NO;
    self.isAddAlliance=NO;
    self.isAddCountry=NO;
    addCountryTime=@"";
    addAllianceTime=@"";
    
    NSString * t=[self getCurrentTime];
    NSString * s=[[[[t substringToIndex:3] md5] stringByAppendingString:[[t substringFromIndex:t.length-3] md5]] md5];
    NSMutableDictionary * json=[[NSMutableDictionary alloc]init];
    [json setObject:t forKey:@"t"];
    [json setObject:s forKey:@"s"];
    
    AFHTTPRequestOperationManager *manager = [AFHTTPRequestOperationManager manager];
    manager.responseSerializer.acceptableContentTypes = [NSSet setWithObjects:@"text/html", nil];
    manager.requestSerializer = [AFHTTPRequestSerializer serializer];
    manager.responseSerializer = [AFHTTPResponseSerializer serializer];
    [manager POST:kWebSocketLinkUrl parameters:json success:^(AFHTTPRequestOperation *operation, id responseObject) {
        
        NSDictionary * dic = [NSJSONSerialization JSONObjectWithData:responseObject options:NSJSONReadingAllowFragments error:nil];
        if ([[dic objectForKey:@"message"] isEqualToString:@"success"]) {
            NSString * appid=@"100001";
            NSString * uid=[UserManager sharedUserManager].currentUser.uid;
            NSString * url=[NSString stringWithFormat:@"%@://%@:%@",
                            [[dic objectForKey:@"data"] objectForKey:@"protocol"],
                            [[dic objectForKey:@"data"] objectForKey:@"ip"],
                            [[dic objectForKey:@"data"] objectForKey:@"port"]
                            ];
            [self wbSocketInitWithHTTPHeaderAppid:appid time:t uid:uid url:url];
            [self wbSocketOpen];
//            if (vBlock) {
//                vBlock(WbSocketConnectStatus_Connecting);
//            }
            
        }else{
            if (vBlock) {
                vBlock(WbSocketConnectStatus_GettingIPParsingError);
            }
            
        }
        
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        if (vBlock) {
            vBlock(WbSocketConnectStatus_GettingIPRequestError);
        }
        
    }];

}

-(void)wbSocketInitWithHTTPHeaderAppid:(NSString*)appid time:(NSString*)time uid:(NSString*)uid url:(NSString * )url{
    
    
    if (urlRequest==nil) {
        urlRequest=[[NSMutableURLRequest alloc]initWithURL:[NSURL URLWithString:url]];
        urlRequest.timeoutInterval=10;
    }
    NSString * result=[[[[[appid stringByAppendingString:uid] md5] stringByAppendingString:time] md5] lowercaseString];
    NSString * sign=result;
    [urlRequest setValue:appid forHTTPHeaderField:@"appid"];
    [urlRequest setValue:time forHTTPHeaderField:@"time"];
    [urlRequest setValue:uid forHTTPHeaderField:@"uid"];
    [urlRequest setValue:sign forHTTPHeaderField:@"sign"];
    if (wbsocket==nil) {
        wbsocket=[[SRWebSocket alloc]initWithURLRequest:urlRequest protocols:nil allowsUntrustedSSLCertificates:YES];
        wbsocket.delegate = self;
    }
}

-(void)wbSocketOpen{
    
    [wbsocket open];
}
-(void)wbSocketClose{
    
    [wbsocket close];
}
-(void)wbSocketChangRoomName:(NSMutableDictionary *)resultDic{
    
    [wbsocket send:[self dictionaryToJson:resultDic]];
}
-(void)wbSocketSendMessageSingle:(NSMutableDictionary*)resultDic{
    
    [wbsocket send:[self dictionaryToJson:resultDic]];
}
-(void)wbSocketSendMessageGroup:(NSMutableDictionary*)resultDic{
    
    [wbsocket send:[self dictionaryToJson:resultDic]];
}
-(void)wbSocketIntoGroupChatRoom:(NSMutableDictionary*)resultDic{
    
    [wbsocket send:[self dictionaryToJson:resultDic]];
}
-(void)wbSocketOutGroupChatRoom:(NSMutableDictionary*)resultDic{
    
    [wbsocket send:[self dictionaryToJson:resultDic]];
}

-(void)timerFired
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        [wbsocket send:kHeartBeat];
    });
}
- (void)webSocket:(SRWebSocket *)webSocket didCloseWithCode:(NSInteger)code reason:(NSString *)reason wasClean:(BOOL)wasClean;
{
    NSLog(@"wbsoket连接断开 %ld %@ %d",code,reason,wasClean);
    [self webScoketConnectWithStatusBlock:^(WbSocketConnectStatus status) {
        
    }];
}
- (void)webSocketDidOpen:(SRWebSocket *)webSocket;
{
    NSLog(@"wbsoket连接成功");
    NSTimer *timer = [NSTimer scheduledTimerWithTimeInterval:kHeartCount target:self selector:@selector(timerFired) userInfo:nil repeats:YES];
    [timer fire];
}
- (void)webSocket:(SRWebSocket *)webSocket didFailWithError:(NSError *)error;
{
    NSLog(@"wbsoket连接错误 %@", error);
    [self webScoketConnectWithStatusBlock:^(WbSocketConnectStatus status) {
        
    }];
}
- (void)webSocket:(SRWebSocket *)webSocket didReceiveMessage:(id)message;
{
    
    
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
    //心跳
    if ([(NSString*)message isEqualToString:kHeartBeat]) {
        NSString *timeString =[NSDate currentTimeStringForSecond ];
        DLog(@"weSocket======心跳  %@ %@", message, timeString);
        return;
    }
        
    NSDictionary * dic=[self parseJSONStringToNSDictionary:(NSString*)message];
    //判断连接是否成功
    if (self.isConnected==NO)
    {
        
        if ([[dic objectForKey:@"cmd"] isEqualToString:cmd_login_success])
        {
            DLog(@"独立服务器连接成功!!!");
            self.isConnected=YES;
            
            [self settingUserInfo];
            
          }
        
    }
        
        if ([[dic objectForKey:@"cmd"] isEqualToString:cmd_setUserInfo]) {
            //进入国家
            if (self.isAddCountry==NO) {
                [self addCountryGroup];
            }
            
            //进入联盟
            if ([UserManager sharedUserManager].currentUser.allianceId.length> 0) {
                
                if (self.isAddAlliance==NO) {
                    [self addAllianceGroup];
                }
                
            }else{
                self.isAddAlliance=YES;
            }

        }
        
        if ([[dic objectForKey:@"cmd"] isEqualToString:cmd_roomJoin]) {
            //进入国家    成功会返回 sendtime  失败的话不会返回
            DLog(@"加入房间成功  :%@",dic);
            if (self.isAddCountry==NO) {
                NSString * sendTimeString =[NSString stringWithFormat:@"%@",[dic objectForKey:@"sendTime"]];
                if ([sendTimeString isEqualToString:addCountryTime]) {
                    self.isAddCountry=YES;
                    //            [self getFirstHistoryMessage];
                    [self gettingFirstServerMessage];
                }
            }
            
            //进入联盟     成功会返回 sendtime  失败的话不会返回
            if ([UserManager sharedUserManager].currentUser.allianceId.length>0) {
                if (self.isAddAlliance==NO)
                {
                    NSString * sendTimeString =[NSString stringWithFormat:@"%@",[dic objectForKey:@"sendTime"]];
                    if ([sendTimeString isEqualToString:addAllianceTime]) {
                        self.isAddAlliance=YES;
                        //                [self getFirstHistoryMessage];
                    }
                }
            }

        }
   
        
        
        
    //push的消息
    if ([[dic objectForKey:@"cmd"] isEqualToString:cmd_push_chatroom]) {
         DLog(@"收到push   :%@",dic);
        CSMessage *message = [CSMessage messageFromDataDic:dic];
        ChatChannel *cc =  nil;
        if (message) {
            if (message.channelType == CSChannelType_Country) {
                cc =   [[ChannelManager sharedChannelManager] gettingCountryChannel];
            }else if (message.channelType == CSChannelType_Alliance){
                cc =   [[ChannelManager sharedChannelManager] gettingAllianceChannel];
            }
            if (cc ) {
                [cc savingMessageToDBWithMsg:message];
                dispatch_async(dispatch_get_main_queue(), ^{
                    [cc chatNewMsgPushed:@[message]];//同步UI
                });
            }

        }
        
   }
    
    
    
    //获取某条消息时间之后的消息
    if ([[dic objectForKey:@"cmd"] isEqualToString:cmd_getMsgAfterCreatTime]) {
        DLog(@"获得的消息数组   %@",dic);
        ChatChannel *ccCountry = [[ChannelManager sharedChannelManager] gettingCountryChannel];
        ChatChannel *ccAlliance = [[ChannelManager sharedChannelManager] gettingAllianceChannel];
        NSDictionary * dataDic = [CSMessage TransformationHistoryToArrModel:dic];
        NSArray * CountryArr=[dataDic objectForKey:@"CountryArr"];
        NSArray * AllianceArr=[dataDic objectForKey:@"AllianceArr"];
        for (int i=0; i<CountryArr.count; i++) {
            [ccCountry savingMessageToDBWithMsg:(CSMessage*)[CountryArr objectAtIndex:i]];//循环进数据库
        }
        for (int i=0; i<AllianceArr.count; i++) {
            [ccAlliance savingMessageToDBWithMsg:(CSMessage*)[AllianceArr objectAtIndex:i]];//循环进数据库
        }
        DVLog(@"ccCountry:  %@",CountryArr);
        
        DVLog(@"ccCountry:  %@",AllianceArr);
       
    }
        
     });
}




-(void)settingUserInfo{
    NSString *userUid =[UserManager sharedUserManager].currentUser.uid;
//    NSDictionary *infoDic =[NSDictionary dictionaryWithObjectsAndKeys:userUid,"uid" ,[NSString currentLanguageType],"lang" , nil];
     NSDictionary *infoDic  =@{@"uid":userUid,@"lang":[NSString currentLanguageType]};
    NSDictionary *params =@{@"info":infoDic};
    NSDictionary *paramsDic =@{@"cmd":cmd_setUserInfo,
                               @"params":params,
                               @"sendTime":[self getCurrentTime] };
    NSString *jsonParams =[self dictionaryToJson:paramsDic];
    [wbsocket send:jsonParams];
}

-(void)getFirstHistoryMessage
{
    return;
    //国家和联盟都进入后  拉取历史消息
//    if (self.isAddAlliance==YES&&self.isAddCountry==YES) {
        NSMutableDictionary * resultDic=[[NSMutableDictionary alloc]init];
        NSMutableDictionary * params=[[NSMutableDictionary alloc]init];
        [resultDic setObject:@"history.roomsv2" forKey:@"cmd"];
        [resultDic setObject:[self getCurrentTime] forKey:@"sendTime"];
        NSMutableDictionary * roomsDic=[[NSMutableDictionary alloc]init];
        [roomsDic setObject:@"0" forKey:@"test_country_1"];
        if ([UserManager sharedUserManager].currentUser.allianceId.length>0) {
            [roomsDic setObject:@"0" forKey:[NSString stringWithFormat:@"test_alliance_%ld_%@",[UserManager sharedUserManager].currentUser.serverId,[UserManager sharedUserManager].currentUser.allianceId]];
        }
        [params setObject:roomsDic forKey:@"rooms"];
        [params setObject:@"" forKey:@"lang"];
        [resultDic setObject:params forKey:@"params"];
        [wbsocket send:[self dictionaryToJson:resultDic]];
//    }
}

-(void)gettingFirstServerMessage{
    NSDictionary *rooms = [ServiceInterface serviceInterfaceSharedManager].roomsDic;
    NSMutableDictionary * resultDic=[[NSMutableDictionary alloc]init];
    NSMutableDictionary * params=[[NSMutableDictionary alloc]init];
    [resultDic setObject:@"history.roomsv2" forKey:@"cmd"];
    [resultDic setObject:[self getCurrentTime] forKey:@"sendTime"];
    [params setObject:rooms forKey:@"rooms"];
    [params setObject:@"" forKey:@"lang"];
    [resultDic setObject:params forKey:@"params"];
    NSString *jsonParams = [self dictionaryToJson:resultDic];
    [wbsocket send:jsonParams];
}
-(void)addAllianceGroup
{
    NSMutableDictionary * resultDic=[[NSMutableDictionary alloc]init];
    NSMutableDictionary * params=[[NSMutableDictionary alloc]init];
    [resultDic setObject:cmd_roomJoin forKey:@"cmd"];
    long long time=[[self getCurrentTime] longLongValue]+1;//时间+1区分返回的是国家还是联盟
    [resultDic setObject:[NSString stringWithFormat:@"%lld",time]forKey:@"sendTime"];
    [params setObject:[NSString stringWithFormat:@"alliance_%ld_%@",[UserManager sharedUserManager].currentUser.serverId,[UserManager sharedUserManager].currentUser.allianceId] forKey:@"roomId"];//id 需要对应一下
    [params setObject:@"alliance" forKey:@"group"];
    [resultDic setObject:params forKey:@"params"];
    addAllianceTime=[resultDic objectForKey:@"sendTime"];
    [wbsocket send:[self dictionaryToJson:resultDic]];
}
-(void)addCountryGroup
{
    NSMutableDictionary * resultDic=[[NSMutableDictionary alloc]init];
    NSMutableDictionary * params=[[NSMutableDictionary alloc]init];
    [resultDic setObject:cmd_roomJoin forKey:@"cmd"];
    [resultDic setObject:[self getCurrentTime] forKey:@"sendTime"];
    [params setObject:[NSString stringWithFormat:@"country_%ld",[UserManager sharedUserManager].currentUser.serverId] forKey:@"roomId"];
    [params setObject:@"country" forKey:@"group"];
    [resultDic setObject:params forKey:@"params"];
    addCountryTime=[resultDic objectForKey:@"sendTime"];
    [wbsocket send:[self dictionaryToJson:resultDic]];
}
- (NSString*)dictionaryToJson:(NSDictionary *)dic
{
    NSError *parseError = nil;
    NSData  *jsonData = [NSJSONSerialization dataWithJSONObject:dic options:NSJSONWritingPrettyPrinted error:&parseError];
    return [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
}
-(NSDictionary *)parseJSONStringToNSDictionary:(NSString *)JSONString {
    NSData *JSONData = [JSONString dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *responseJSON = [NSJSONSerialization JSONObjectWithData:JSONData options:NSJSONReadingMutableLeaves error:nil];
    return responseJSON;
}
-(NSString*)getCurrentTime
{
    NSDate* dat = [NSDate dateWithTimeIntervalSinceNow:0];
    //NSTimeInterval a=[dat timeIntervalSince1970] * 1000;
    NSTimeInterval a=[dat timeIntervalSince1970];
    long long int date = (long long int)a;
    NSString* time=[NSString stringWithFormat:@"%lld",date];
    return [NSString stringWithFormat:@"%@000",time];
}
@end
