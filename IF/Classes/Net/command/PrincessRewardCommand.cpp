//
//  PrincessRewardCommand.cpp
//  IF
//
//  Created by 王超一 on 15/11/25.
//
//

#include "PrincessRewardCommand.hpp"
#include "GCMRewardController.h"
#include "RewardController.h"
#include "SceneController.h"
#include "ImperialScene.h"
bool PrincessRewardCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(PRINCESS_REWARD) != 0)
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
                auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
                if (layer) {
                    layer->m_princessRwdArr = arr;
                }
                auto dict = dynamic_cast<CCDictionary*>(arr->objectAtIndex(0));
                int type = -1;
                if (dict->objectForKey("type")) {
                    type = dict->valueForKey("type")->intValue();
                }
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PrincessRwd, Integer::create(type));
            }
            else
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PrincessRwdNull);
        }
        else
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PrincessRwdNull);
    }
    return true;
}
