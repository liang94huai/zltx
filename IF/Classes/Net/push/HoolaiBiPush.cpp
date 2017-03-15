//
//  HoolaiBiPush.cpp
//  IF
//
//  Created by chenfubi on 15/4/27.
//
//

#include "HoolaiBiPush.h"
#include "HoolaiBISdkUtils.hpp"
void HoolaiBiPaymentPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    HoolaiBISdkUtils::sendPayment(params);
}

void HoolaiBiEconomyPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    HoolaiBISdkUtils::sendEconomy(params);
    
}
