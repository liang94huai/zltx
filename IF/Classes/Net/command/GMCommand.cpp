//
//  GMCommand.cpp
//  IF
//
//  Created by 李锐奇 on 15/3/30.
//
//

#include "GMCommand.h"
#include "LogoutCommand.h"

#include "NetController.h"
#include "FunBuildController.h"
#include "GmController.h"

bool GMCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(GM_CHANGE_SERVER_COMMAND) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyHint("", "", _lang(pStr->getCString()));
    }else{
        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, info_ip);
        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, info_zone);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT, info_port);
        CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, info_gameUid);
        CCUserDefault::sharedUserDefault()->setStringForKey(BUILDCODE, info_gameUid);
        NetController::shared()->setIp(info_ip);
        NetController::shared()->setZone(info_zone);
        NetController::shared()->setPort(info_port);
        
        NetController::shared()->setIsLogout(true);
        NetController::shared()->disconnect();
        GmController::getInstance()->gmUid = GlobalData::shared()->playerInfo.uid;
        LogoutCommand* cmd = new LogoutCommand();
        CCDictionary* _params = CCDictionary::create();
        _params->setObject(CCString::create(info_zone), "zoneName");
        _params->setObject(CCString::create("true"), "isReLogin");
        CCDictionary* dict = CCDictionary::create();
        dict->setObject(CCString::create("logout"), "cmd");
        dict->setObject(_params, "params");
        GmController::getInstance()->isGmLogin = 1;
        cmd->handleRecieve(dict);
    }
    return true;
}
