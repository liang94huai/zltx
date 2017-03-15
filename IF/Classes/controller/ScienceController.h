//
//  ScienceController.h
//  IF
//
//  Created by fubin on 14-3-19.
//
//

#ifndef IF_ScienceController_h
#define IF_ScienceController_h

#define MSG_SCIENCE_UPDATE "msg_science_update"
#define MSG_SCIENCE_CELL_STATE "msg_science_cell_state"
#define MSG_SCIENCE_DONATE "msg_science_donate"
#define MSG_AL_SCIENCE_PUSH "msg_al_science_push"
#define MSG_AL_SCIENCE_POINT "msg_al_science_point"
#define MSG_SCIENCE_POWER_ADD "msg_science_power_add"
#define MSG_SCIENCE_LACKOF_RESOURCE "msg_science_lackof_resource"
#include "CommonInclude.h"
#include "ScienceInfo.h"

class ScienceController : public CCObject{
public:
    static ScienceController* getInstance();
    static void purgeData();
    ScienceController();
    ~ScienceController();
    
    bool costCD(int itemId, string toolUUID);
    
    bool startResearchScience(int scienceid, int gold=0);
    void retResearchScience(CCDictionary* dict);
    
    bool startDirResearchScience(int scienceid);
    void retDirResearchScience(CCDictionary* dict);
    
    bool startFinishScience(int scienceid, string qUUid);
    void retFinishScience(CCDictionary* dict);
    int getScienceLevel(int scienceId);
    CCSafeObject<CCCallFunc> m_callBackFunc;
    
    std::vector<int> m_scienceType;
    
    void initAllianceScienceData(CCArray* array);
    int getTotalAllScPoints();
    
    bool startResearchAllSc(int sid);
    void endResearchAccSc(CCDictionary* dict);
    
    bool startDonate(int sid, int type);
    void endDonate(CCDictionary* dict);
    
    bool startDonateRefresh(int sid);
    void endDonateRefresh(CCDictionary* dict);
    
    void pushUpdateAllInfo(CCDictionary* dict);
    void onQuitAlliance();
    int  findResearchingAlSc();
    
    bool costCDDonate();
    void endCDDonate(CCDictionary* dict);
    
    // 科技开关
    void ParseScienceCfg();
    int SetScienceCfg(int iSciencePos, const char* serverList, const char* strVersion);
    int GetScienceCfg(int iSciencePos);
    
    void updateSciencePower(int power);
    
    //获得当前的推荐的科技数量
    int getRecommendedSciCnt();
    int getFirstRecScienceIdPos(vector<int>&);
    
    map<int, AllianceScienceInfo> allianceScienceMap;
    map<int, vector<int>> aScienceFloorMap;
    map< int, vector<int> > aScienceTabMap;//用于联盟科技的科技树分类
    map<int, map<string, int> > aScienLocationMap;//用于联盟科技树连线
    map<int, float> alScEffMap;
    
    // 科技服务器开关
    map<std::string, std::string> mapServerCfg;
    
    // 服务器的科技配置表
    int m_iScienceCfg[10];
};

#endif
