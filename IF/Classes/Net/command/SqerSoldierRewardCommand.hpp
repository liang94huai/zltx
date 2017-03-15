//
//  SqerSoldierRewardCommand.hpp
//  IF
//
//  Created by 王超一 on 15/12/15.
//
//

#ifndef SqerSoldierRewardCommand_hpp
#define SqerSoldierRewardCommand_hpp
#include "CommandBase.h"
#define SQERSOLDIER_REWARD "sd.rwd"

class SqerSoldierRewardCommand: public CommandBase {
public:
    SqerSoldierRewardCommand(Point pt) : CommandBase(SQERSOLDIER_REWARD){
        m_pt = pt;
    };
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    Point m_pt;
};

#endif /* SqerSoldierRewardCommand_hpp */
