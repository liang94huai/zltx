//
//  GetUserInfoCommand.cpp
//  IF
//
//  Created by fubin on 14-4-3.
//
//

#include "GetMultiUserInfoCommand.h"
#include "PopupViewController.h"
#include "RoleInfoView.h"
#include "PlayerInfo.h"
#include "ChatController.h"

bool GetMultiUserInfoCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(GET_MULTI_USER_INFO_CMD) != 0)
        return false;

    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }

    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
        callFail(NetResult::createWithFail(params));
    }else{
        ChatController::getInstance()->getMultiUserInfoCallback(params);
    }
    return true;
}
