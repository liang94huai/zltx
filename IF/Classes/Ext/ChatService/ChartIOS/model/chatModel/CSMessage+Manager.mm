//
//  CSMessage+Manager.m
//  IF
//
//  Created by 张彦涛 on 15/12/28.
//
//
 
#import "CSMessage+Manager.h"
#import "UserManager.h"
#import "ChatServiceCocos2dx.h"
@implementation CSMessage (Manager)
+(CSMessage *)messageWithSelfSendString:(NSString *)vSendString andWithChannelType:(CSChannelType )vChannelType andWithMessageBodyType:(CSMessageBodyType)vMsgBodyType{
    CSMessage *message =[[CSMessage alloc]init];
    message.msg = vSendString;
    message.createTime = [[NSDate date]timeIntervalSince1970] ;
    if (ChatServiceCocos2dx::CS_WebSocket_Msg_Get && ChatServiceCocos2dx::CS_WebSocket_Opened) {
        message.createTime =message.createTime * 1000;
    }
    message.sendLocalTime = message.createTime;
    message.post = (int )vMsgBodyType;
    message.channelType = vChannelType;
    message.uid = [UserManager sharedUserManager].currentUser.uid;
    message.sendState = CSMessageSendState_Delivering;
    return message;
}
+(NSMutableDictionary*)TransformationHistoryToArrModel:(NSDictionary *)dataDic
{
    
    NSMutableArray * dataArrAlliance=[[NSMutableArray alloc]init];
    NSMutableArray * dataArrCountry=[[NSMutableArray alloc]init];
    NSArray * resultArr=(NSArray*)[[dataDic objectForKey:@"result"] objectForKey:@"rooms"];
    for (int i=0; i<resultArr.count; i++) {
        if ([[[resultArr objectAtIndex:i] objectForKey:@"roomId"] isEqualToString:[NSString stringWithFormat:@"alliance_%ld_%@",[UserManager sharedUserManager].currentUser.serverId,[UserManager sharedUserManager].currentUser.allianceId]]) {
            NSArray * arr=[[resultArr objectAtIndex:i]objectForKey:@"msgs"];
            for (int i=0; i<arr.count; i++) {
                NSDictionary * dic=[arr objectAtIndex:i];
                
                CSMessage * message=[[CSMessage alloc]init];
                message.sendState=2;
                message.sequenceId=[[dic objectForKey:@"seqId"] intValue];
                message.uid=[dic objectForKey:@"sender"];
                NSString *transString = [NSString stringWithString:[[dic objectForKey:@"msg"] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
                message.msg=transString;
                
                NSString * obsendLocalTime=[NSString stringWithFormat:@"%ld",[[dic objectForKey:@"sendTime"] integerValue]];
                if (obsendLocalTime.length>=10) {
                message.sendLocalTime=[[obsendLocalTime substringToIndex:10] intValue];
                }
                
                NSString * obTime=[NSString stringWithFormat:@"%ld",[[dic objectForKey:@"serverTime"] integerValue]];
                if (obTime.length>=10) {
                message.createTime=[[obTime substringToIndex:10] intValue];
                }
                
                message.post=[[dic objectForKey:@"type"] intValue];
                message.originalLang=[dic objectForKey:@"originalLang"];
                message.translatedLang=[dic objectForKey:@"translationMsg"];
                [dataArrAlliance addObject:message];
            }

        }else if ([[[resultArr objectAtIndex:i] objectForKey:@"roomId"] isEqualToString:[NSString stringWithFormat:@"country_%ld",[UserManager sharedUserManager].currentUser.serverId ] ]){
            NSArray * arr=[[resultArr objectAtIndex:i]objectForKey:@"msgs"];
            for (int i=0; i<arr.count; i++) {
                NSDictionary * dic=[arr objectAtIndex:i];
               
                
                CSMessage * message=[[CSMessage alloc]init];
                message.sendState=2;
                message.sequenceId=[[dic objectForKey:@"seqId"] intValue];
                message.uid=[dic objectForKey:@"sender"];
                NSString *transString = [NSString stringWithString:[[dic objectForKey:@"msg"] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
                message.msg=transString;
                
                NSString * obsendLocalTime=[NSString stringWithFormat:@"%ld",[[dic objectForKey:@"sendTime"] integerValue]];
                if (obsendLocalTime.length>=10) {
                    message.sendLocalTime=[[obsendLocalTime substringToIndex:10] intValue];
                }
                
                NSString * obTime=[NSString stringWithFormat:@"%ld",[[dic objectForKey:@"serverTime"] integerValue]];
                if (obTime.length>=10) {
                    message.createTime=[[obTime substringToIndex:10] intValue];
                }

                message.post=[[dic objectForKey:@"type"] intValue];
                message.originalLang=[dic objectForKey:@"originalLang"];
                message.translatedLang=[dic objectForKey:@"translationMsg"];
                [dataArrCountry addObject:message];
            }
        }
    }

    NSMutableDictionary * dic=[[NSMutableDictionary alloc]init];
    [dic setObject:dataArrCountry forKey:@"CountryArr"];
    [dic setObject:dataArrAlliance forKey:@"AllianceArr"];
    
    return dic;
}
+(CSMessage *)messageFromDataDic:(NSDictionary *)msgDic
{
  /*  
   {
        clientid = 812;
        cmd = "push.chat.room";
        data =     {
            appId = 100001;
            extra =         {
                seqId = 463148;
                userLang = ko;
            };
            group = country;
            msg = "hi, everybody";
            originalLang = en;
            roomId = "country_532";
            sendTime = 1452663233;
            sender = 1015957589000354;
            senderInfo = "<null>";
            seqId = 809;
            serverTime = 1452663234626;
            type = 0;
        };
        server = s1;
        serverTime = 1452663234840;
    }
   */


    NSDictionary * datadic=[msgDic objectForKey:@"data"];
    CSMessage * message=[[CSMessage alloc]init];
    message.sequenceId=[[[datadic objectForKey:@"extra"]objectForKey:@"seqId"] longLongValue];
    message.sendState=2;
    message.uid=[datadic objectForKey:@"sender"];
    message.msg=[datadic objectForKey:@"msg"];
    message.sendLocalTime=[[datadic objectForKey:@"sendTime"] longLongValue];
    if ([[datadic objectForKey:@"group"] isEqualToString:@"country"]) {
        message.channelType = (int )CSChannelType_Country;
    }else if ([[datadic objectForKey:@"group"] isEqualToString:@"alliance"]){
        message.channelType = (int )CSChannelType_Alliance;
    }
    message.createTime =[[datadic objectForKey:@"serverTime"] longLongValue];
    message.post=[[datadic objectForKey:@"type"] intValue];
    message.originalLang=[datadic objectForKey:@"originalLang"];
    message.translateMsg=[datadic objectForKey:@"translationMsg"];
    
    return message;
}

-(int)gettingRedPackageStatusWithMessage{
    NSArray *arr = [self.attachmentId componentsSeparatedByString:@"|"];
    if ([arr count] == 2) {
        NSString *status = arr[1];
        return [status intValue];
    }else{
        return -1;
    }
    
}
@end
