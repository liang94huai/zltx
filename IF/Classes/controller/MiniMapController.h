//
//  MiniMapController.h
//  IF
//
//  Created by 杜 威 on 16-11-2.
//
//

#ifndef __IF__MiniMapController__
#define __IF__MiniMapController__

#include "CommonInclude.h"
#include <spine/Json.h>

#define MAP_COUNT 13
#define REGION_COUNT 62
enum OccupiedState
{
    OCCUPIED_NOT,//未占领
    OCCUPIED_MY,//我占领
    OCCUPIED_OTHERS,//敌方占领
};

struct AlliancePowerInfo
{
    string m_AllianceID;
    string m_AllianceName;
    
    Color3B m_Colour;
    std::vector<int> m_cityVector;  //占领的city的orderID
    public:
    AlliancePowerInfo(){
        m_AllianceName = "";
        m_AllianceID = "";
        m_Colour = ccc3(0,255,0);;
        m_cityVector.clear();
    }
};
//地图资源结构
class MiniCityInfo
{
public:
    MiniCityInfo():m_CityId(0),m_Name(""),m_Order(0),m_ColourId(""),m_OccupiedState(OccupiedState::OCCUPIED_NOT),m_AllianceName(""),m_AllianceID(""),m_areaID(0),m_ZoneName(""),m_Colour(ccYELLOW){};
    ~MiniCityInfo(){};
    int m_CityId;
    int m_Order;
    string m_Name;
    string m_ColourId;
    Vec2 m_Pos;
    Color3B m_Colour;
    OccupiedState m_OccupiedState;
    string m_AllianceName;
    string m_AllianceID;
    int    m_areaID;
    string m_ZoneName;
    
    void initData(CCDictionary* dict);
    void initData(string medalId);
    void setXmlData(CCDictionary* dict);
    void setXmlData(string medalId);
    void setColourId(int colourId);
    void setOccupiedState(int state);
    void setServerInfo(CCDictionary* dict);
};


class MiniMapController : public CCObject {

public:
    static MiniMapController* getInstance();
    void init();
    void update(float dt);
    MiniCityInfo* getCityPosByOrder(int order);
    MiniCityInfo* getCityPosById(int id);
    vector<string> getCityIdsByOrder(int order);
    void parseCityInfo(CCDictionary* dict);
    
    //区域城池信息
    void logicCityInfo();
    int getCityInfoSize();
    MiniCityInfo* getCityInfoByIndex(int iIndex);
    int getCityMapSize(int iMapIndex);
    MiniCityInfo* getCityMapByIndex(int iMapIndex,int iIndex);
    
    //区域 同盟联盟信息
    void logicAllianceInfo();
    int getAllianceInfoSize();
    MiniCityInfo* getAllianceByIndex(int iIndex);
    int getAllianceMapSize(int iMapIndex);
    MiniCityInfo* getAllianceMapByIndex(int iMapIndex,int iIndex);

    //区域  联盟势力信息
    void logicPowerInfo();
    int getPowerInfoSize();
    AlliancePowerInfo* getPowerInfoByIndex(int iIndex);
    
    AlliancePowerInfo* getSelfPowerInfo();
    
    Color3B getWorldCityColorByIndex(int order);
protected:
    std::map<int, Color3B> m_worldCityColor;
    std::map<int, MiniCityInfo> m_worldCityPos;
    std::map<int, vector<string>> m_worldMapCityIds;
    
    //区域信息
    
    std::map<string,AlliancePowerInfo> m_mapAlliancePower;
    
    
    std::vector<int> m_allianceVector;
    
    std::map<int, std::vector<int>>   m_mapAllianceVector;   //区域里的同盟标记信息
    

};

#endif /* defined(__IF__MiniMapController__) */
