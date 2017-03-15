//
//  DragonRankInfo.cpp
//  IF
//
//  Created by ganxiaohua on 15/9/9.
//
//

#include "DragonRankInfo.h"

DragonRankInfo::DragonRankInfo(cocos2d::CCDictionary *dict){
    if (dict==NULL) {
        return ;
    }
    id = dict->valueForKey("id")->intValue();
    min_A = dict->valueForKey("min_A")->intValue();
    max_A = dict->valueForKey("max_A")->intValue();
    rank = dict->valueForKey("rank")->intValue();
    level = dict->valueForKey("level")->intValue();
    icon = dict->valueForKey("icon")->getCString();
}