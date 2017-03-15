//
//  NSUserInfo+UserManager.m
//  IF
//
//  Created by 张彦涛 on 15/12/28.
//
//

#import "NSUserInfo+UserManager.h"
#import "UserManager.h"
#import "NSString+Cocos2dHelper.h"
@implementation NSUserInfo (UserManager)
+(void)userCheckWithChatMailInfo:(ChatMailInfo *)vChatMailInfo{
    
    NSString * ns_uid = [NSString stringWithUTF8String:vChatMailInfo->uid.c_str()];
    
    NSString * ns_vip = [NSString stringWithUTF8String:vChatMailInfo->vip.c_str()];
  
    NSString * ns_name = [NSString stringWithUTF8String:vChatMailInfo->name.c_str()];
    NSString * ns_asn = [NSString stringWithUTF8String:vChatMailInfo->asn.c_str()];
     NSString * ns_headPic = [NSString stringWithUTF8String:vChatMailInfo->headPic.c_str()];
    int gmod = vChatMailInfo->gmod;
    int headPicVer = vChatMailInfo->headPicVer;
    NSInteger lastUpdateTime = vChatMailInfo->lastUpdateTime;
    NSUserInfo *user = [[UserManager sharedUserManager]gettingUserInfoForMemoryAndDBWithUidString:ns_uid];
    if (user == nil){
        if  (ns_name.length> 0){
            NSUserInfo *user = [[NSUserInfo alloc]init];
            user.userName = ns_name;
            user.uid = ns_uid;
            user.asn = ns_asn;
            user.vipLevel = [ns_vip intValue];
            user.headPic = ns_headPic;
            user.headPicVer = headPicVer;
            user.lastUpdateTime = 0;
            user.isDummy = NO;
            user.allianceId = @"";
            user.mGmod = gmod;
            [user updateToDBWithSelf];
            [[UserManager sharedUserManager] addUser:user];
        }else{
            NSUserInfo *user = [[NSUserInfo alloc]init];
            user.allianceId = @"";
            user.uid = ns_uid;
            user.userName =  ns_uid ;
            user.lastUpdateTime = 0;
            user.isDummy = false;
            user.mGmod = gmod;
            [user updateToDBWithSelf];
            [[UserManager sharedUserManager] addUser:user];
        }
    }else{
        if (user.lastUpdateTime <lastUpdateTime){
            [[UserManager sharedUserManager] gettingServicerUser:user.uid];
        }
    }
}

-(NSString *)vipStringWithSelf{
 
    NSInteger dtime = self.vipEndTime - (NSUInteger)[[NSDate date] timeIntervalSince1970];
    if (dtime) {
        if (self.sVipLevel> 0) {
            // svip
            NSString *vipString =[NSString stringWithMultilingualWithKey:@"103075"];//103075=SVIP
            vipString = [vipString stringByAppendingFormat:@"%d",self.sVipLevel ];
            return vipString;
        }else{
            if (self.vipLevel) {
                NSString *vipString =[NSString stringWithMultilingualWithKey:@"105002"];// 105002=VIP
                vipString = [vipString stringByAppendingFormat:@"%d",self.vipLevel ];
                return vipString;
            }else{
                return @"";
            }
        }
    }else{
        return @"";
    }
}
-(BOOL)isVipShow{
    NSInteger dtime = self.vipEndTime - (NSUInteger)[[NSDate date] timeIntervalSince1970];
    if(dtime> 0){
        if (self.sVipLevel> 0 || self.vipLevel > 0){
            return YES;
        }else{
            return NO;
        }
    }else{
        return NO;
    }
//    return  (self.vipLevel > 0 && dtime > 0) ? YES:NO;
}


@end
