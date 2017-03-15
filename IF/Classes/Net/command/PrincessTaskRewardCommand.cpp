
//
//  PrincessTaskRewardCommand.cpp
//  IF
//
//  Created by 王超一 on 16/1/14.
//
//

#include "PrincessTaskRewardCommand.hpp"
#include "PrincessQuestController.hpp"
#include "RewardController.h"
#include "PopupViewController.h"
#include "GCMRewardController.h"

bool PrincessTaskRewardCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(PRINCESS_TASK_REWARD) != 0){
        return false;
    }
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    CCLOG("%s", m_id.c_str());
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
    }else{
        CCArray *arr = dynamic_cast<CCArray*>(params->objectForKey("reward"));
        string type = CCCommonUtils::getPropById(m_id, "type");
        if (type=="3") {
            GCMRewardController::getInstance()->retReward2(arr,true);
        }else{
            std::string str = RewardController::getInstance()->retReward(arr);
            if (m_showTip) {
                CCCommonUtils::flyHint("quest_icon_1.png", _lang("107519"), str, 3, 0);
            }
        }
        
        CCDictionary *task=_dict(params->objectForKey("task"));
        if(task){
            CCArray* arr = CCArray::create();
            arr->addObject(task);
            PrincessQuestController::getInstance()->updateTask(arr);
        }
        PrincessQuestController::getInstance()->getRewardSuccessHandle(m_id);
    }
    PrincessQuestController::getInstance()->resetGettingRewardFlag(0);
    return true;
}
