//
//  DragonBattleRewardInfoCommand.cpp
//  IF
//
//  Created by ganxiaohua on 15/9/16.
//
//

#include "DragonBattleRewardInfoCommand.h"
#include "ActivityController.h"

bool DragonBattleRewardInfoCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(DRAGON_BATTLE_REWARD_INFO) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
    }else{
        CCArray* rewards = dynamic_cast<CCArray*>(params->objectForKey("rewards"));
        if (rewards) {
            ActivityController::getInstance()->getDragonRewards()->addObjectsFromArray(rewards);
        }
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}