//
//  DragonAllianceRankInfo.cpp
//  IF
//
//  Created by ganxiaohua on 15/9/10.
//
//

#include "DragonAllianceRankInfo.h"

DragonAllianceRankInfo::DragonAllianceRankInfo(cocos2d::CCDictionary *dict){
    this->parse(dict);
}

void DragonAllianceRankInfo::parse(cocos2d::CCDictionary *dict){
    if (dict==NULL) {
        return ;
    }
    kingId = dict->valueForKey("kingId")->intValue();
    rankNum = dict->valueForKey("rankNum")->intValue();
    leader = dict->valueForKey("leader")->getCString();
    maxmember = dict->valueForKey("maxmember")->intValue();
    teamId = dict->valueForKey("teamId")->getCString();
    curmember = dict->valueForKey("curmember")->intValue();
    allianceId = dict->valueForKey("allianceId")->getCString();
    wins = dict->valueForKey("wins")->floatValue();
    kingName = dict->valueForKey("kingName")->getCString();
    teamState = dict->valueForKey("teamState")->intValue();
    icon = dict->valueForKey("icon")->getCString();
    score = dict->valueForKey("score")->intValue();
    name = dict->valueForKey("name")->getCString();
    fighting = dict->valueForKey("fighting")->intValue();
}