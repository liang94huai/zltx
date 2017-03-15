//
//  DragonAllianceRankInfo.h
//  IF
//
//  Created by ganxiaohua on 15/9/10.
//
//

#ifndef __IF__DragonAllianceRankInfo__
#define __IF__DragonAllianceRankInfo__

#include "CommonInclude.h"

class DragonAllianceRankInfo : public cocos2d::CCObject{
public:
    DragonAllianceRankInfo(cocos2d::CCDictionary *dict);
    DragonAllianceRankInfo():kingId(0),rankNum(-1),leader(std::string()),maxmember(0),teamId(std::string()),icon(std::string()),curmember(0),type(0),wins(0.0),allianceId(std::string()),kingName(std::string()),teamState(0),score(0),name(std::string()),fighting(0){};
    void parse(cocos2d::CCDictionary *dict);
    int kingId;
    int rankNum;
    std::string leader;
    int maxmember;
    std::string teamId;
    int curmember;
    int type;
    float wins;
    std::string allianceId;
    std::string kingName;
    int teamState;
    std::string icon;
    int score;
    std::string name;
    int fighting;
};
#endif /* defined(__IF__DragonAllianceRankInfo__) */
