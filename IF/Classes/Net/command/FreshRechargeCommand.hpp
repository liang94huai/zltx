//
//  RefreshRechargeCommand.hpp
//  IF
//
//  Created by 邢晓瑞 on 15/11/12.
//
//

#ifndef RefreshRechargeCommand_hpp
#define RefreshRechargeCommand_hpp

#include "CommandBase.h"

#define  FRESH_RECHARGE_RWD "fresh.recharge.reward"

class FreshRcgRwdCmd: public CommandBase
{
public:
    FreshRcgRwdCmd(int idx = -1, string itemId = "-1"): CommandBase(FRESH_RECHARGE_RWD), m_itemId(itemId), m_idx(idx)
    {
        putParam("id", __String::create(itemId));
    }
protected:
    virtual bool handleRecieve(__Dictionary* dict);
private:
    string m_itemId;
    int m_idx;
};

#endif /* RefreshRechargeCommand_hpp */
