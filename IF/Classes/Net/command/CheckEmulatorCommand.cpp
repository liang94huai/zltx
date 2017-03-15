//
//  CheckEmulatorCommand.h
//  IF
//
//  Created by wangguoqiang on 15-11-16.
//
//

#include "CheckEmulatorCommand.h"

bool CheckEmulatorCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(CHECK_RESULT) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(pStr->getCString());
        // callFail(NetResult::create());
    }else{
        // if(params->objectForKey("remainGold")){
        //     int gold = params->valueForKey("remainGold")->intValue();
        //     GlobalData::shared()->playerInfo.gold = gold;
        //     CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
        // }
        // callSuccess(NetResult::create(Error_OK, params));
    }
    
    return true;
}