//
//  ArmyInfo.h
//  IF
//
//  Created by 李锐奇 on 14-2-28.
//
//

#ifndef __IF__ArmyInfo__
#define __IF__ArmyInfo__

#include "cocos2d.h"

class ArmyInfo : public cocos2d::CCObject{
public:
    ArmyInfo(cocos2d::CCDictionary *dict);
    ArmyInfo():power(1.0),train(0),march(0){};
    int range;
    std::string armyId;
    long healSilver;
    long healRes;
    long healTime;
    long time;
    long free;
    float upkeep;
    long defen;
    int move;
    long attack;
    long defence;
    long pve;
    long wood;
    long food;
    long silver;
    long iron;
    long stone;
    int level;
    int armyLevel;
    int load;
    float power;
    int speed;
    time_t finishTime;
    long train;
    bool isArmy;
    long march;
    long health;
    int unlockLevel;//解锁等级
    void setFinishiTime(cocos2d::CCDictionary *dict);
    void setEndTime(double time, bool isCancel = false);
    void setArmyInfo(cocos2d::CCDictionary *dict);
    void update();
    std::string getRealItemId();
    std::string getName();
    std::string getIcon();
    std::string getBodyIcon();
    std::string getHeadIcon();
    std::string getDesc();
    std::string getBuildId();
    std::string getAttrIcon(int index);
    void getAttrDesc(int index, std::vector<std::string> &descVec);
    void getLvInfo(int index, int star, std::vector<std::string> &descVec);
    void getNextLvInfo(int index, int star, std::vector<std::string> &descVec);
};

#endif /* defined(__IF__ArmyInfo__) */
