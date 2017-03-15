//
//  PlayerInfoController.h
//  IF
//
//  Created by liutianhou on 17-2-23.
//
//
#include "GlobalData.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "HoolaiFastAccess/GameProxy.h"
#include "HoolaiFastAccess/CCAccessSDK.h"
#else
#import <AccessSDK/CCAccessSDK.h>
#endif
#include "PlayerInfoController.h"
#include "VipUtil.h"

static PlayerInfoController *_instance = NULL;

PlayerInfoController::PlayerInfoController() {}
PlayerInfoController::~PlayerInfoController() {
    
}

PlayerInfoController* PlayerInfoController::getInstance() {
    if (!_instance) {
        _instance = new PlayerInfoController();
        _instance->init();
    }
    return _instance;
}

void PlayerInfoController::init() {
    
}

void PlayerInfoController::saveSDKdateEnter()
{
    map < string, string > userExtData;
    userExtData[CCROLE_ID] = GlobalData::shared()->getPlayerInfo()->uid.c_str();
    userExtData[CCROLE_NAME] = GlobalData::shared()->getPlayerInfo()->name.c_str();
    userExtData[CCROLE_LEVEL] = CC_CMDITOA(GlobalData::shared()->getPlayerInfo()->level).c_str();
    userExtData[CCZONE_ID] = CC_CMDITOA(GlobalData::shared()->getPlayerInfo()->currentServerId).c_str();
    userExtData[CCZONE_NAME] = CCUserDefault::sharedUserDefault()->getStringForKey("server_name");
    userExtData[CCBALANCE] = "0";
    
    int vipLevel = VipUtil::getVipLevel(GlobalData::shared()->playerInfo.vipPoints);
    
    userExtData[CCVIP] = CC_CMDITOA(vipLevel).c_str();
    if (GlobalData::shared()->getPlayerInfo()->isInAlliance())
    {
        userExtData[CCPARTYNAME] = GlobalData::shared()->getPlayerInfo()->allianceInfo.name.c_str();
    }else{
        userExtData[CCPARTYNAME] = "unknow";
    }

    userExtData[CCACTION] = CCACTION_ENTER_SERVER;
    CCAccessSDK::sharedInstance()->setUserExtData(userExtData);

}

void PlayerInfoController::saveSDKdateRole()
{
    map < string, string > userExtData;
    userExtData[CCROLE_ID] = GlobalData::shared()->getPlayerInfo()->uid.c_str();
    userExtData[CCROLE_NAME] = GlobalData::shared()->getPlayerInfo()->name.c_str();
    userExtData[CCROLE_LEVEL] = CC_CMDITOA(GlobalData::shared()->getPlayerInfo()->level).c_str();
    userExtData[CCZONE_ID] = CC_CMDITOA(GlobalData::shared()->getPlayerInfo()->currentServerId).c_str();
    userExtData[CCZONE_NAME] = CCUserDefault::sharedUserDefault()->getStringForKey("server_name");
    userExtData[CCBALANCE] = "0";
    
    int vipLevel = VipUtil::getVipLevel(GlobalData::shared()->playerInfo.vipPoints);
    
    userExtData[CCVIP] = CC_CMDITOA(vipLevel).c_str();
    if (GlobalData::shared()->getPlayerInfo()->isInAlliance())
    {
        userExtData[CCPARTYNAME] = GlobalData::shared()->getPlayerInfo()->allianceInfo.name.c_str();
    }else{
        userExtData[CCPARTYNAME] = "unknow";
    }
    
    userExtData[CCACTION] = CCACTION_CREATE_ROLE;
    CCAccessSDK::sharedInstance()->setUserExtData(userExtData);
    
}

void PlayerInfoController::saveSDKdateRoleUp()
{
    map < string, string > userExtData;
    userExtData[CCROLE_ID] = GlobalData::shared()->getPlayerInfo()->uid.c_str();
    userExtData[CCROLE_NAME] = GlobalData::shared()->getPlayerInfo()->name.c_str();
    userExtData[CCROLE_LEVEL] = CC_CMDITOA(GlobalData::shared()->getPlayerInfo()->level).c_str();
    userExtData[CCZONE_ID] = CC_CMDITOA(GlobalData::shared()->getPlayerInfo()->currentServerId).c_str();
    userExtData[CCZONE_NAME] = CCUserDefault::sharedUserDefault()->getStringForKey("server_name");
    userExtData[CCBALANCE] = "0";
    
    int vipLevel = VipUtil::getVipLevel(GlobalData::shared()->playerInfo.vipPoints);
    
    userExtData[CCVIP] = CC_CMDITOA(vipLevel).c_str();
    if (GlobalData::shared()->getPlayerInfo()->isInAlliance())
    {
        userExtData[CCPARTYNAME] = GlobalData::shared()->getPlayerInfo()->allianceInfo.name.c_str();
    }else{
        userExtData[CCPARTYNAME] = "unknow";
    }
    
    userExtData[CCACTION] = CCACTION_LEVEL_UP;
    CCAccessSDK::sharedInstance()->setUserExtData(userExtData);
    
}
