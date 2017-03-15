//
//  DragonBattleAlliancePlayCommand.cpp
//  IF
//
//  Created by ganxiaohua on 15/9/1.
//
//

#include "DragonBattleAlliancePlayCommand.h"
#include "ActivityController.h"

bool DragonBattleAlliancePlayCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(DRAGON_BATTLE_ALLIANCE_PLAY) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
    }else{
        if(m_state == 1 && m_uid!="" && m_uid==GlobalData::shared()->playerInfo.uid){
            ActivityController::getInstance()->canFight = 1;
        }
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}