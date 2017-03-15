//
//  AllianceEventCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-10-8.
//
//

#include "AllianceEventCommand.h"

bool AllianceEventCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_AL_EVENT) != 0)
        return false;
    
    CCDictionary *params=dynamic_cast<CCDictionary*>(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(pStr->getCString());
        callFail(NetResult::create());
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}