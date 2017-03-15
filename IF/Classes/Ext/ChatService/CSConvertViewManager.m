//
//  CSConvertViewManager.m
//  IF
//
//  Created by 张彦涛 on 15/11/6.
//
//

#import "CSConvertViewManager.h"
#import "CSCommandManager.h"
#import "MailDataManager.h"

@implementation CSConvertViewManager

-(instancetype)init
{
    self = [super init];
    if (self) {
        
    }
    return self;
}

+ (CSConvertViewManager *)sharedMailInfoManager
{
    static CSConvertViewManager *sharedCSConvertViewManager = nil;
    static dispatch_once_t predicate;
    dispatch_once(&predicate, ^{
        sharedCSConvertViewManager = [[self alloc] init];
    });
    return sharedCSConvertViewManager;
}

-(void)refresh_RewardStatusWithMailID:(NSString *)vMailID andWithStatusFlag:(BOOL)vStatusFlag{
    if (YES ==  vStatusFlag) {
        //已领取
        if ([vMailID isEqual:self.currentmailInfoDataModel.mail_ID]) {
            self.currentmailInfoDataModel.rewardStatus = 1;
            MailData *tempData=[MailDataManager mailDataWithMailInfoData:self.currentmailInfoDataModel];
            self.currentmailInfoDataModel.maildata = tempData;
            if ([_convertViewManagerDelegate respondsToSelector:@selector(serverRefeshDateSucceedWithManager:)]) {
                [_convertViewManagerDelegate serverRefeshDateSucceedWithManager:self];
            }
        }
        
    }
}
-(void)refresh_ReadStatusWithMailID:(NSString *)vMailID andWithStatusFlag:(BOOL)vStatusFlag{
    if (YES ==  vStatusFlag) {
        //已读
        if ([vMailID isEqual:self.currentmailInfoDataModel.mail_ID]) {
            self.currentmailInfoDataModel.status = 1;
            if ([_convertViewManagerDelegate respondsToSelector:@selector(serverRefeshDateSucceedWithManager:)]) {
                [_convertViewManagerDelegate serverRefeshDateSucceedWithManager:self];
            }
        }

    }
}

-(void)currentMailReadingNofityToServer{
    [[CSCommandManager sharedCommandManager]settingreadSatusWithMailID:self.currentmailInfoDataModel.mail_ID andWithMailTpye:self.currentmailInfoDataModel.type];
}
@end
