//
//  DragonBattleAllianceInfoCommand.h
//  IF
//
//  Created by ganxiaohua on 15/9/1.
//
//

#ifndef __IF__DragonBattleAllianceInfoCommand__
#define __IF__DragonBattleAllianceInfoCommand__

#include "CommandBase.h"

#define DRAGON_BATTLE_ALLIANCE_INFO "dragon.battle.alliance.info"

class DragonBattleAllianceInfoCommand: public CommandBase {
public:
    DragonBattleAllianceInfoCommand() : CommandBase(DRAGON_BATTLE_ALLIANCE_INFO){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__DragonBattleAllianceInfoCommand__) */
