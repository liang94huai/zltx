//
//  DragonBattleApplyInfoCommand.h
//  IF
//
//  Created by ganxiaohua on 15/9/1.
//
//

#ifndef __IF__DragonBattleApplyInfoCommand__
#define __IF__DragonBattleApplyInfoCommand__

#include "CommandBase.h"

#define DRAGON_BATTLE_APPLY_INFO "dragon.battle.apply.info"

class DragonBattleApplyInfoCommand: public CommandBase {
public:
    DragonBattleApplyInfoCommand() : CommandBase(DRAGON_BATTLE_APPLY_INFO){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__DragonBattleApplyInfoCommand__) */
