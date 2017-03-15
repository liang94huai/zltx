//
//  HoolaiBiPush.h
//  IF
//
//  Created by chenfubi on 15/5/6.
//
//

#ifndef __IF__HoolaiBiPush__
#define __IF__HoolaiBiPush__

#include "CommonInclude.h"

class HoolaiBiPaymentPush{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};


class HoolaiBiEconomyPush{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__HoolaiBiPush__) */
