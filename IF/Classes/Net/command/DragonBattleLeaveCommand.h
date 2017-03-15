//
//  DragonBattleLeaveCommand.h
//  IF
//
//  Created by ganxiaohua on 15/9/6.
//
//

#ifndef __IF__DragonBattleLeaveCommand__
#define __IF__DragonBattleLeaveCommand__

#include "CommandBase.h"

#define DRAGON_BATTLE_LEAVE "dragon.battle.leave"

class DragonBattleLeaveCommand: public CommandBase {
public:
    DragonBattleLeaveCommand() : CommandBase(DRAGON_BATTLE_LEAVE){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};

#endif /* defined(__IF__DragonBattleLeaveCommand__) */
