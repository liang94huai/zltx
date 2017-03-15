//
//  GetUserInfoCommand.cpp
//  IF
//
//  Created by fubin on 14-4-3.
//
//

#include "GetUserInfoCommand.h"
#include "PopupViewController.h"
#include "RoleInfoView.h"
#include "PlayerInfo.h"

bool GetUserInfoCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(GET_USER_INFO_CMD) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
        CCCommonUtils::flyText(_lang(pStr->getCString()));
        callFail(NetResult::createWithFail(params));
    }else{
        callSuccess(NetResult::create(Error_OK, (CCObject*)(params)));
    }
    return true;
}

bool SetUserEquipShowCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(USER_EQUIP_SHOW_CMD) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    int show = params->valueForKey("show")->intValue();
    if (show==0) {
        //关闭
//        CCCommonUtils::flyText(_lang(pStr->getCString()));
    }else{
        //打开
    }
    return true;
}

bool daltoncmd::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(DALTON_CMD) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    int show = params->valueForKey("show")->intValue();
    if (show==0) {
        GlobalData::shared()->playerInfo.isDaltonOn = false;
    }else{
        GlobalData::shared()->playerInfo.isDaltonOn = true;
    }
    return true;
}
bool medelshowcmd::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(MEDELSHOW_CMD) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    int show = params->valueForKey("show")->intValue();
    if (show==0) {
        GlobalData::shared()->playerInfo.shouldShowMedal = false;
    }else{
        GlobalData::shared()->playerInfo.shouldShowMedal = true;
    }
    return true;
}
bool supermanDesGetCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(SUPERMAN_GET_COMMAND) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
        CCCommonUtils::flyText(_lang(pStr->getCString()));
    }else{
        string desStr = params->valueForKey("description")->getCString();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHANGE_SUPERMAN_TIP,CCStringMake(desStr));
    }
    return true;
}
bool supermanDesSaveCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(SUPERMAN_SAVE_COMMAND) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
        CCCommonUtils::flyText(_lang(pStr->getCString()));
        callFail(NetResult::createWithFail(params));
    }else{
         
    }
    return true;
}

