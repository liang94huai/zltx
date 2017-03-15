//
//  VipUtil.h
//  IF
//
//  Created by chenliang on 14-3-20.
//
//

#ifndef __IF__VipUtil__
#define __IF__VipUtil__

#include "VipConstants.h"

class VipUtil
{
public:
    static int getVipLevel(long vipPoints);
    static int getVipItemValueByKey(std::string itemId,std::string key);
    static int getCurVipValueByKey(std::string key);
    
    static bool showVIPActiveView();
    
    static bool isSVIP();
    static int getSvipShowLevel();
    static int getMaxSpoint();
    static int getMaxPoint();
    static int getVip10Point();
    static int getSvip1Point();
    
    static bool isSvipPointId(int itemId);
    static bool isVipPointId(int itemId);
    static bool isSvipTimeId(int itemId);
    static bool isVipTimeId(int itemId);
};

#endif /* defined(__IF__VipUtil__) */
