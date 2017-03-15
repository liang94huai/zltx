//
//  CSALeaveCommand.cpp
//  IF
//
//  Created by ganxiaohua on 15/6/10.
//
//

#include "CSALeaveCommand.h"
#include "SceneController.h"
#include "WorldMapView.h"
#include "NetController.h"
#include "LogoutCommand.h"
#include "FlyHint.h"
#include "ActivityController.h"

bool CSALeaveCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(CROSS_SERVER_LEAVE) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    GameController::getInstance()->removeWaitInterface();
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
    }else{
        ActivityController::getInstance()->changeServerByInfo(params);
    }
    return true;
}

