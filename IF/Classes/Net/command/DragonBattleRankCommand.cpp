//
//  DragonBattleRankCommand.cpp
//  IF
//
//  Created by ganxiaohua on 15/9/2.
//
//

#include "DragonBattleRankCommand.h"
#include "ActivityController.h"

bool DragonBattleRankCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(DRAGON_BATTLE_BATTLE_RANK) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        //CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
        callFail(NetResult::create(Error_OK, params));
    }else{
        CCArray* array = dynamic_cast<CCArray*>(params->objectForKey("rank"));
        if (array && array->count()>0) {
            int num = array->count();
            for (int i=0; i<num; i++) {
                auto oneDic = dynamic_cast<CCDictionary*>(array->objectAtIndex(i));
                if (oneDic) {
                    string allianceId = oneDic->valueForKey("allianceId")->getCString();
                    std::map<string,DragonAllianceRankInfo>::iterator* it;
                    auto rankIt = ActivityController::getInstance()->dragonRankMap.find(allianceId);
                    if (rankIt==ActivityController::getInstance()->dragonRankMap.end()) {
                        ActivityController::getInstance()->dragonRankMap[allianceId] = new DragonAllianceRankInfo(oneDic);
                    }else{
                        if (rankIt->second) {
                            rankIt->second->parse(oneDic);
                        }
                    }
                }
            }
        }
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}