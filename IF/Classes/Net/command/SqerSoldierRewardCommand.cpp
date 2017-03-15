//
//  SqerSoldierRewardCommand.cpp
//  IF
//
//  Created by 王超一 on 15/12/15.
//
//

#include "SqerSoldierRewardCommand.hpp"
#include "GCMRewardController.h"
#include "RewardController.h"
#include "SceneController.h"
#include "ImperialScene.h"
bool SqerSoldierRewardCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(SQERSOLDIER_REWARD) != 0)
        return false;
    CCDictionary* params = _dict(dict->objectForKey("params"));
    if(params==NULL) return false;
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {;
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
    }else{
        if(params->objectForKey("reward")){
            auto arr = dynamic_cast<CCArray*>(params->objectForKey("reward"));
            if (arr->count() > 0) {
                GCMRewardController::getInstance()->retReward3(arr,0.5,true,m_pt);
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFREASH_TOOL_DATA);
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
            }
        }
    }
    return true;
}