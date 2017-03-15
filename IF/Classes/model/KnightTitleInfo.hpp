//
//  KnightTitleInfo.hpp
//  IF
//
//  Created by 付彬 on 16/1/7.
//
//

#ifndef KnightTitleInfo_hpp
#define KnightTitleInfo_hpp

#include "CommonInclude.h"

class KnightTitleInfo : public cocos2d::CCObject {
public:
    KnightTitleInfo(CCDictionary* dict);
    KnightTitleInfo():
    itemId(0)
    ,name("")
    ,des("")
    ,icon("")
    ,bd_level(0)
    ,color(0)
    ,power(0)
    {};
    
    int itemId;
    string name;
    string des;
    string icon;
    int bd_level;
    int color;
    int power;
    
    map<int, string> showDias;
    map<int, float> values;
    map<int, vector<float>> parasMap;
    
    map<int, int> mateUseMap;//激活所需材料
    map<int, int> mateUnlockMap;//解锁所需材料

    string getEffFormatByOrd(int order);
    string getEffPMByOrd(int order);
};

#endif /* KnightTitleInfo_hpp */
