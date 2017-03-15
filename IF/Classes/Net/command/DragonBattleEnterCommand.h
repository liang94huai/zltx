//
//  DragonBattleEnterCommand.h
//  IF
//
//  Created by ganxiaohua on 15/9/6.
//
//

#ifndef __IF__DragonBattleEnterCommand__
#define __IF__DragonBattleEnterCommand__

#include "CommandBase.h"

#define DRAGON_BATTLE_ENTER "dragon.battle.enter"

class DragonBattleEnterCommand: public CommandBase {
public:
    DragonBattleEnterCommand() : CommandBase(DRAGON_BATTLE_ENTER){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};

#endif /* defined(__IF__DragonBattleEnterCommand__) */
