//
//  PrincessRewardCommand.hpp
//  IF
//
//  Created by 王超一 on 15/11/25.
//
//

#ifndef PrincessRewardCommand_hpp
#define PrincessRewardCommand_hpp

#include "CommandBase.h"
#define PRINCESS_REWARD "princess.rwd"

class PrincessRewardCommand: public CommandBase {
public:
    PrincessRewardCommand() : CommandBase(PRINCESS_REWARD){};
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* PrincessRewardCommand_hpp */
