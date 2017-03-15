//
//  DragonBattleApplyInfoCommand.cpp
//  IF
//
//  Created by ganxiaohua on 15/9/1.
//
//

#include "DragonBattleApplyInfoCommand.h"

bool DragonBattleApplyInfoCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(DRAGON_BATTLE_APPLY_INFO) != 0)
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