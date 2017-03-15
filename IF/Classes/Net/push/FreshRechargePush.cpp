//
//  FreshRechargePush.cpp
//  IF
//
//  Created by 邢晓瑞 on 15/11/12.
//
//

#include "FreshRechargePush.hpp"
#include "ActivityController.h"

void FreshRechargePush::handleResponse(__Dictionary* dict)
{
    if (!dict)
    {
        return;
    }
    __Dictionary* params = _dict(dict->objectForKey("params"));
    if (params->objectForKey("freshRechargeTotal"))
    {
        GlobalData::shared()->freshRechargeTotal = params->valueForKey("freshRechargeTotal")->intValue();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FRESH_RECHARGE_TOTAL_CHANGED);
    }
}