//
//  AllianceDailyQuestInfo.h
//  IF
//
//  Created by 张军 on 15/10/22.
//
//

#ifndef AllianceDailyQuestInfo_h
#define AllianceDailyQuestInfo_h

#include "CommonInclude.h"

class AllianceDailyQuestInfo : public cocos2d::CCObject {
public:
    ~AllianceDailyQuestInfo(){};
    AllianceDailyQuestInfo(CCDictionary *dict);
    AllianceDailyQuestInfo():m_id("")
    ,m_name("")
    ,m_description("")
    ,m_color("")
    ,m_stat("")
    ,m_uid("")
    ,m_publishUid("")
    ,m_extraReward("")
    ,m_baseReward("")
    ,m_finishTime(0)
    ,m_vanishTime(0)
    ,m_allianceId("")
    ,m_acceptReward("")
    ,m_publishName("")
    ,m_acceptStat("")
    ,m_publishStat("")
    ,m_acceptUid("")
    ,m_startTime(0)
    ,m_publishTime(0)
    ,m_acceptName("")
    ,m_freequeue(0)
    ,m_nextgold(0)
    ,m_picVer(-1)
    ,m_pic("")
    {};
    
    void setDataByDict(CCDictionary *dict);
    string orderRewards(string rewards);
    int getIndexByName(std::string name);
    
    string m_id;
    string m_name;
    string m_description;
    string m_color;
    string m_nameid;
    
    int m_freequeue;
    int m_nextgold;
    int m_picVer;
    string m_pic;
    string m_stat;//发布情况
    string m_uid;
    string m_publishUid;
    string m_extraReward;
    string m_baseReward;
    double m_finishTime;
    double m_vanishTime;
    double m_startTime;
    double m_publishTime;
    
    string m_allianceId;
    string m_acceptReward;
    string m_publishName;
    string m_acceptStat;
    string m_publishStat;
    
    string m_acceptUid;
    string m_acceptName;

};
#endif /* AllianceDailyQuestInfo_h */
