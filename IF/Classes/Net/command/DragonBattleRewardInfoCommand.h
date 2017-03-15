//
//  DragonBattleRewardInfoCommand.h
//  IF
//
//  Created by ganxiaohua on 15/9/16.
//
//

#ifndef __IF__DragonBattleRewardInfoCommand__
#define __IF__DragonBattleRewardInfoCommand__

#include "CommandBase.h"

#define DRAGON_BATTLE_REWARD_INFO "dragon.battle.reward.info"

class DragonBattleRewardInfoCommand: public CommandBase {
public:
    DragonBattleRewardInfoCommand() : CommandBase(DRAGON_BATTLE_REWARD_INFO){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__DragonBattleRewardInfoCommand__) */
