//
//  PromotionCommand.cpp
//  IF
//
//  Created by liruiqi on 15/10/10.
//
//

#include "PromotionCommand.hpp"

bool PromotionCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(PROMOTION_COMMAND) != 0)
        return false;
    
    return true;
}
