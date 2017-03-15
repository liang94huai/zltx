
//
//  PrincessTaskPush.cpp
//  IF
//
//  Created by 王超一 on 16/1/14.
//
//

#include "PrincessTaskPush.hpp"
#include "PrincessQuestController.hpp"
void PrincessTaskComplete::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    CCArray* tasks = dynamic_cast<CCArray*>(params->objectForKey("task"));
    PrincessQuestController::getInstance()->updateTask(tasks);
}

void PrincessTaskNew::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    CCArray* tasks = dynamic_cast<CCArray*>(params->objectForKey("task"));
    PrincessQuestController::getInstance()->updateTask(tasks);
}