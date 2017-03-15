//
//  PrincessLimitedQuestInfo.hpp
//  IF
//
//  Created by 王超一 on 16/1/5.
//
//

#ifndef PrincessLimitedQuestInfo_hpp
#define PrincessLimitedQuestInfo_hpp

#include "CommonInclude.h"

class PrincessQuestInfo : public cocos2d::CCObject {
public:
    CREATE_FUNC(PrincessQuestInfo);
    ~PrincessQuestInfo(){if(rwdInTime)rwdInTime->release();if(rwdOutOfTime)rwdOutOfTime->release();};
    virtual bool init(){
        rwdInTime = NULL;
        rwdOutOfTime = NULL;
        return true;
    };
    void parse(CCDictionary *dict);
    void parseFromServer(CCDictionary *dict);
    std::string itemId;
    int state;
    cocos2d::CCArray *rwdInTime;
    cocos2d::CCArray *rwdOutOfTime;
    std::string name;
    std::string description;
    double limitedTime;
    double finishTime;
    
    std::string target;
    std::string guide;
    std::string icon1;
    std::string icon2;
    std::string rewardshow;
    int type;
    int type1;
    int maxtype;//用来解析max字段
    int level;
    int order;
    int order1;
    int order2;
    int maxStar;
    int curValue;
    int maxValue;
    int currPhase;
    int maxPhase;
    bool haveOpen;
    int go;
    int gotype;
    int isShow;
    
    int classType;
    int para1;
    int newSortType;
    int trainOrder;//训练士兵顺序
    int main;//是否为主线任务
    std::string subTask;//前台表读取的支线任务字段
    std::string mainTask;//前台表读取的主线任务字段
    std::string sub;//后台返回的存储完成的支线任务字段
};


#endif /* PrincessLimitedQuestInfo_hpp */
