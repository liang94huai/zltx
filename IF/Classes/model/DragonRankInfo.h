//
//  DragonRankInfo.h
//  IF
//
//  Created by ganxiaohua on 15/9/9.
//
//

#ifndef __IF__DragonRankInfo__
#define __IF__DragonRankInfo__
#include "CommonInclude.h"

class DragonRankInfo : public cocos2d::CCObject{
public:
    DragonRankInfo(cocos2d::CCDictionary *dict);
    DragonRankInfo():id(0),min_A(0),max_A(0),rank(0),level(0),icon(std::string()){};
    int id;
    int min_A;
    int max_A;
    int rank;
    int level;
    std::string icon;
};
#endif /* defined(__IF__DragonRankInfo__) */
