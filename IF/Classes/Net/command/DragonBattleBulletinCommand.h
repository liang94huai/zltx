//
//  DragonBattleBulletinCommand.h
//  IF
//
//  Created by ganxiaohua on 15/10/8.
//
//

#ifndef __IF__DragonBattleBulletinCommand__
#define __IF__DragonBattleBulletinCommand__

#include "CommandBase.h"

#define DRAGON_BATTLE_BULLETIN "dragon.battle.bulletin"

class DragonBattleBulletinCommand: public CommandBase {
public:
    DragonBattleBulletinCommand(int start, int end, int index) : CommandBase(DRAGON_BATTLE_BULLETIN){
        putParam("pointId", CCInteger::create(index));
        putParam("start", CCInteger::create(start));
        putParam("end", CCInteger::create(end));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__DragonBattleBulletinCommand__) */
