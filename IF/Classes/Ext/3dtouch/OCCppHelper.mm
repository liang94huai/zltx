//
//  OCCppHelp.mm
//  IF
//
//  Created by liuzengyou on 15/10/25.
//
//

#import <Foundation/Foundation.h>

#import "OCCppHelper.h"
#import <string.h>

#include "../../model/GlobalData.h"

class OCCppHelper;

@interface OCCppHelperIOS : NSObject

+ (OCCppHelperIOS *)getInstance;

@end

@implementation OCCppHelperIOS

+ (OCCppHelperIOS *)getInstance
{
    static OCCppHelperIOS *_instance = nil;
    static dispatch_once_t predicate;
    dispatch_once(&predicate, ^{
        _instance = [[self alloc] init];
    });
    return _instance;
}

+ (void) notificationUpdatePlayerInfo
{
    if (GlobalData::shared()->peekPageType == peekType_peek && OCCppHelper::share()->m_info!=NULL)
    {
        [[NSNotificationCenter defaultCenter]postNotificationName:IOS_kUPDATE_PLAYERINFO object:nil];
    }
}

@end

OCCppHelper::OCCppHelper():m_info(NULL),m_medalIconId(""),m_achievePro(""),m_medalCntStr("")
{
    
}

OCCppHelper* OCCppHelper::share()
{
    static OCCppHelper* _OCCppHelp = NULL;
    if (!_OCCppHelp) {
        _OCCppHelp = new OCCppHelper();
    }
    
    return _OCCppHelp;
}

void OCCppHelper::notificationUpdatePlayerInfo(PlayerInfo* info, std::string medalIconId, std::string achievePro, std::string medalCntStr)
{
    m_info = info;
    m_medalIconId = medalIconId;
    m_achievePro = achievePro;
    m_medalCntStr = medalCntStr;
    
    notificationIOSUpdatePlayerInfo();
}

void OCCppHelper::notificationIOSUpdatePlayerInfo()
{
     [OCCppHelperIOS notificationUpdatePlayerInfo];
}

