//
//  KnightTitleCommand.cpp
//  IF
//
//  Created by 付彬 on 16/1/7.
//
//

#include "KnightTitleCommand.hpp"
#include "KnightTitleController.hpp"

bool KnightTitlePutOnCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(KNIGHT_PUTON_COMMAND) != 0) {
        return false;
    }

    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    else {
        KnightTitleController::getInstance()->retPutOnKnightTitle(params);
    }
    return true;
}

bool KnightTitleUnlockCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(KNIGHT_UNLOCK_COMMAND) != 0) {
        return false;
    }
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    else {
        KnightTitleController::getInstance()->retUnlockKnightTitle(params);
    }
    return true;
}