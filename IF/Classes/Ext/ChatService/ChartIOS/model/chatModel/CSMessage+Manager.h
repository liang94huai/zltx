//
//  CSMessage+Manager.h
//  IF
//
//  Created by 张彦涛 on 15/12/28.
//
//
#import "CSMessage.h"
#import "CSMessage+Manager.h"
#import "ChatServiceManagerDefs.h"
@interface CSMessage (Manager)
+(CSMessage *)messageWithSelfSendString:(NSString *)vSendString andWithChannelType:(CSChannelType )vChannelType andWithMessageBodyType:(CSMessageBodyType)vMsgBodyType;
+(CSMessage *)messageFromDataDic:(NSDictionary *)dataDic;
+(NSMutableDictionary * )TransformationHistoryToArrModel:(NSDictionary *)msgDic;//多条拉历史
/** 获取当前红包消息的领取状态 */
-(int)gettingRedPackageStatusWithMessage;
@end
