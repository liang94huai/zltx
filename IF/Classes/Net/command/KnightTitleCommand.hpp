//
//  KnightTitleCommand.hpp
//  IF
//
//  Created by 付彬 on 16/1/7.
//
//

#ifndef KnightTitleCommand_hpp
#define KnightTitleCommand_hpp

#include "CommandBase.h"
#define KNIGHT_PUTON_COMMAND "knight.puton.command"
#define KNIGHT_UNLOCK_COMMAND "knight.unlock.command"

class KnightTitlePutOnCommand: public CommandBase {
public:
    KnightTitlePutOnCommand(int itemId):CommandBase(KNIGHT_PUTON_COMMAND){
        putParam("knightId", CCInteger::create(itemId));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class KnightTitleUnlockCommand: public CommandBase {
public:
    KnightTitleUnlockCommand(int itemId):CommandBase(KNIGHT_UNLOCK_COMMAND){
        putParam("knightId", CCInteger::create(itemId));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

#endif /* KnightTitleCommand_hpp */
