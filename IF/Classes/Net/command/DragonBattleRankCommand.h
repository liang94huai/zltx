//
//  DragonBattleRankCommand.h
//  IF
//
//  Created by ganxiaohua on 15/9/2.
//
//

#ifndef __IF__DragonBattleRankCommand__
#define __IF__DragonBattleRankCommand__

#include "CommandBase.h"

#define DRAGON_BATTLE_BATTLE_RANK "dragon.battle.rank"

class DragonBattleRankCommand: public CommandBase {
public:
    DragonBattleRankCommand(std::string index_limit) : CommandBase(DRAGON_BATTLE_BATTLE_RANK){
        putParam("index_limit", CCString::create(index_limit));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__DragonBattleRankCommand__) */
