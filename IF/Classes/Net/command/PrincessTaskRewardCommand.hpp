//
//  PrincessTaskRewardCommand.hpp
//  IF
//
//  Created by 王超一 on 16/1/14.
//
//

#ifndef PrincessTaskRewardCommand_hpp
#define PrincessTaskRewardCommand_hpp

#include "CommandBase.h"
#define PRINCESS_TASK_REWARD "princess.task.reward"

class PrincessTaskRewardCommand: public CommandBase {
public:
    PrincessTaskRewardCommand(std::string itemId, bool showTip = true) : CommandBase(PRINCESS_TASK_REWARD){
        m_id = itemId;
        m_showTip = showTip;
        putParam("id", CCString::create(itemId));
        CCLOG("%s", m_id.c_str());
    };
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_id;
    bool m_showTip;
};


#endif /* PrincessTaskRewardCommand_hpp */
