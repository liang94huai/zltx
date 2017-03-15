//
//  DragonBattleBuildingInfoCommand.h
//  IF
//
//  Created by ganxiaohua on 15/9/30.
//
//

#ifndef __IF__DragonBattleBuildingInfoCommand__
#define __IF__DragonBattleBuildingInfoCommand__

#include "CommandBase.h"

#define DRAGON_BATTLE_BUILDING_INFO "dragon.battle.building.info"

class DragonBattleBuildingInfoCommand: public CommandBase {
public:
    DragonBattleBuildingInfoCommand(int pointId) : CommandBase(DRAGON_BATTLE_BUILDING_INFO){
        putParam("pointId", CCInteger::create(pointId));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__DragonBattleBuildingInfoCommand__) */
