//
//  AllianceReinforceSoldier.h
//  IF
//
//  Created by ganxiaohua on 14-8-11.
//
//

#ifndef __IF__AllianceReinforceSoldier__
#define __IF__AllianceReinforceSoldier__

#include "CommandBase.h"

#define ALLIANCE_AL_REINFORCE_SOLDIER   "al.ref.cnt"
#define ALLIANCE_AL_REINFORCE_THRONE_TREBUCHET    "get.target.simple.troops"



class AllianceReinforceSoldier: public CommandBase {
public:
    AllianceReinforceSoldier(std::string reinforceType) : CommandBase(reinforceType){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__AllianceReinforceSoldier__) */
