//
//  DragonBattleFightRankCommand.h
//  IF
//
//  Created by ganxiaohua on 15/9/10.
//
//

#ifndef __IF__DragonBattleFightRankCommand__
#define __IF__DragonBattleFightRankCommand__

#include "CommandBase.h"

#define DRAGON_BATTLE_FIGHT_RANK "dragon.battle.fight.rank"

class DragonBattleFightRankCommand: public CommandBase {
public:
    DragonBattleFightRankCommand() : CommandBase(DRAGON_BATTLE_FIGHT_RANK){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__DragonBattleFightRankCommand__) */
