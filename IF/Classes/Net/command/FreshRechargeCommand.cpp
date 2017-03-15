//
//  RefreshRechargeCommand.cpp
//  IF
//
//  Created by 邢晓瑞 on 15/11/12.
//
//

#include "FreshRechargeCommand.hpp"
#include "ActivityController.h"

bool FreshRcgRwdCmd::handleRecieve(__Dictionary* dict)
{
    if (dict->valueForKey("cmd")->compare(FRESH_RECHARGE_RWD) != 0) {
        return false;
    }
    __Dictionary* params = _dict(dict->objectForKey("params"));
    params->setObject(__String::create(m_itemId), "m_itemId");
    params->setObject(__String::create(CC_ITOA(m_idx)), "m_idx");
    FreshRechargeController::getInstance()->endReward(params);
    return true;
}