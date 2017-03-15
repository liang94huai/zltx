//
//  MonsterSiegeActivityInfoCommand.cpp
//  IF
//
//  Created by ganxiaohua on 15/3/31.
//
//

#include "MonsterSiegeActivityInfoCommand.h"

bool MonsterSiegeActivityInfoCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(MONSTER_SIEGE_ACTIVITY_INFO) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
    }else{
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}