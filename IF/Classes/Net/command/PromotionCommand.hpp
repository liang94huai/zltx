//
//  PromotionCommand.hpp
//  IF
//
//  Created by liruiqi on 15/10/10.
//
//

#ifndef PromotionCommand_hpp
#define PromotionCommand_hpp

#include "CommandBase.h"

#define PROMOTION_COMMAND "exchange.promotion"

class PromotionCommand : public CommandBase{
public:
    PromotionCommand(std::string promotion) :
    CommandBase(PROMOTION_COMMAND){
        putParam("telkomselpromotion", CCString::create(promotion));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* PromotionCommand_hpp */
