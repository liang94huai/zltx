//
//  MiniMapController.cpp
//  IF
//
//  Created by 杜 威 on 16-11-2.
//
//

#include "MiniMapController.h"

void MiniCityInfo::initData(cocos2d::CCDictionary *dict)
{
    setXmlData(dict);
}

void MiniCityInfo::initData(string medalId)
{
    auto itemDict = LocalController::shared()->DBXMLManager()->getObjectByKey(medalId);
    initData(itemDict);
}

void MiniCityInfo::setXmlData(string medalId)
{
    auto itemDict = LocalController::shared()->DBXMLManager()->getObjectByKey(medalId);
    setXmlData(itemDict);
}

void MiniCityInfo::setXmlData(cocos2d::CCDictionary *dict)
{
    m_CityId=dict->valueForKey("id")->intValue();
    m_Name = _lang(dict->valueForKey("name")->getCString());
    m_Order=dict->valueForKey("order")->intValue();
    auto areaID =dict->valueForKey("areaType")->getCString();
    m_areaID = atoi(areaID);
    auto dict_2 = LocalController::shared()->DBXMLManager()->getObjectByKey(areaID);
    m_ZoneName = _lang(dict_2->valueForKey("name")->getCString());
    auto sPos = dict->valueForKey("pos")->getCString();
    vector<string> strVec;
    CCCommonUtils::splitString(sPos, ";", strVec);
    m_Pos=Vec2(atoi(strVec[0].c_str())+1,atoi(strVec[1].c_str())+1);
    setColourId(m_Order);
}

void MiniCityInfo::setColourId(int colourId)
{
    m_ColourId=CC_ITOA(colourId);
    m_Colour=MiniMapController::getInstance()->getWorldCityColorByIndex(colourId);
    if (m_AllianceID!=""&&GlobalData::shared()->playerInfo.getAllianceId()==m_AllianceID)
    {
        m_Colour=Color3B::GREEN;
    }
}

void MiniCityInfo::setOccupiedState(int state)
{
    m_OccupiedState=OccupiedState(state);
}

void MiniCityInfo::setServerInfo(CCDictionary* dict)
{
    
    string allanceId=dict->valueForKey("allanceId")->getCString();
    int state=0;
    if(allanceId!="")
    {
       if (GlobalData::shared()->playerInfo.getAllianceId()==allanceId)
        {
            state=1;
        }
        else
        {
            state=2;
        }
    }
    setOccupiedState(state);
    m_AllianceID = dict->valueForKey("allanceId")->getCString();
    m_AllianceName = dict->valueForKey("allanceName")->getCString();
    setColourId(dict->valueForKey("color")->intValue());
}

static MiniMapController* _instance = NULL;

MiniMapController* MiniMapController::getInstance() {
    if (!_instance) {
        _instance = new MiniMapController();
        _instance->init();
    }
    return _instance;
}

void MiniMapController::init() {
    m_worldCityPos.clear();
    //读取建筑数据
    CCDictionary* d = LocalController::shared()->DBXMLManager()->getGroupByKey("world_city");
    CCDictElement* pelem = nullptr;
    CCDictionary* p = nullptr;
    CCDICT_FOREACH(d, pelem)
    {
        p = dynamic_cast<CCDictionary*>(pelem->getObject());
        auto order = p->valueForKey("order")->intValue();
        if (order!=0)
        {
            auto id = p->valueForKey("id")->intValue();
            m_worldCityPos[id]=MiniCityInfo();
            m_worldCityPos[id].initData(p);
        }
    }
    
    m_worldCityColor.clear();
    CCDictionary* Color = LocalController::shared()->DBXMLManager()->getGroupByKey("world_city_color");
    pelem = nullptr;
    p = nullptr;
    CCDICT_FOREACH(Color, pelem)
    {
        p = dynamic_cast<CCDictionary*>(pelem->getObject());
        auto order = p->valueForKey("order")->intValue();
        auto color = p->valueForKey("color")->getCString();
        vector<string> strVec;
        CCCommonUtils::splitString(color, ";", strVec);
        m_worldCityColor[order]=Color3B(atoi(strVec[0].c_str()),atoi(strVec[1].c_str()),atoi(strVec[2].c_str()));
    }
    
    m_worldMapCityIds.clear();
    CCDictionary* map = LocalController::shared()->DBXMLManager()->getGroupByKey("world_map");
    pelem = nullptr;
    p = nullptr;
    CCDICT_FOREACH(map, pelem)
    {
        p = dynamic_cast<CCDictionary*>(pelem->getObject());
        auto areaType = p->valueForKey("areaType")->intValue();
        if (areaType!=0)
        {
            auto sCity = p->valueForKey("city")->getCString();
            vector<string> strVec;
            CCCommonUtils::splitString(sCity, ";", strVec);
            m_worldMapCityIds[areaType]=strVec;
        }
    }
    
    //test
    logicAllianceInfo();
    logicPowerInfo();
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

MiniCityInfo* MiniMapController::getCityPosByOrder(int order)
{
    for (auto it = m_worldCityPos.begin(); it != m_worldCityPos.end(); it++) {
        if(it->second.m_Order==order)
        {
            return &it->second;
        }
    }
    return nullptr;
}

MiniCityInfo* MiniMapController::getCityPosById(int id)
{
    return &m_worldCityPos[id];
}

vector<string> MiniMapController::getCityIdsByOrder(int order)
{
    return m_worldMapCityIds[order];
}

Color3B MiniMapController::getWorldCityColorByIndex(int order){
    return m_worldCityColor[order];
}


void MiniMapController::update(float dt) {
    
    
}

void MiniMapController::parseCityInfo(CCDictionary* obj)
{
    if(!obj->objectForKey("result")){
        return;
    }
    CCArray *arr = dynamic_cast<CCArray*>(obj->objectForKey("result"));
    int index = 0;
    while (index < arr->count()) {
        auto dict = _dict(arr->objectAtIndex(index));
        if (dict) {
            
            int id = dict->valueForKey("id")->intValue();
            m_worldCityPos[id].setServerInfo(dict);
        }
        index++;
    }
    logicAllianceInfo();
    logicPowerInfo();
}

//区域城池信息
void MiniMapController::logicCityInfo()
{
    //m_worldCityPos;
//    for (auto it = m_worldCityPos.begin(); it != m_worldCityPos.end(); it++) {
//        
//    }
    //m_mapCityVector; //init
}

int MiniMapController::getCityInfoSize()
{
    int iSize = (int)m_worldCityPos.size();
    return iSize;
}

MiniCityInfo* MiniMapController::getCityInfoByIndex(int iIndex)
{
     //int iSize = (int)m_worldCityPos.size();
    //int index = 0;
    for (auto it = m_worldCityPos.begin(); it != m_worldCityPos.end(); it++) {
        if(it->second.m_Order == (iIndex+1))
        {
            return &(it->second);
        }
        //index++;
    }
    return NULL;
}

int MiniMapController::getCityMapSize(int iMapIndex)
{
    int index = iMapIndex;
    for (auto it = m_worldMapCityIds.begin(); it != m_worldMapCityIds.end(); it++) {
        if(index == it->first)
        {
            return (int)(it->second).size();
        }
        
    }
    return 0;
}


MiniCityInfo* MiniMapController::getCityMapByIndex(int iMapIndex,int iIndex)
{
    if(getCityMapSize(iMapIndex) == 0)
    {
        return NULL;
    }
    if(iIndex<0 || iIndex >= getCityMapSize(iMapIndex))
    {
        return NULL;
    }
    int iCityID = atoi(m_worldMapCityIds[iMapIndex][iIndex].c_str());
    return getCityPosById(iCityID);
}

//区域 同盟联盟信息
void MiniMapController::logicAllianceInfo()
{
    //m_allianceVector;
    m_allianceVector.clear();
    m_mapAllianceVector.clear();
    for (auto it = m_worldCityPos.begin(); it != m_worldCityPos.end(); it++) {
        if(GlobalData::shared()->playerInfo.getAllianceId()=="")
        {
            break;
        }
        if(it->second.m_AllianceID == GlobalData::shared()->playerInfo.getAllianceId()) // 联盟
        {
            m_allianceVector.push_back(it->second.m_CityId);
            
            m_mapAllianceVector[it->second.m_areaID].push_back(it->second.m_CityId);
        }
    }
    //m_mapAllianceVector;
}

int MiniMapController::getAllianceInfoSize()
{
    int iSize = (int)m_allianceVector.size();
    return iSize;
}

MiniCityInfo* MiniMapController::getAllianceByIndex(int iIndex)
{
    int iSize = (int)m_allianceVector.size();
    if(iIndex<0 || iIndex >= iSize)
    {
        return NULL;
    }
    int iCityID = m_allianceVector[iIndex];
    return getCityPosById(iCityID);
    
}

int MiniMapController::getAllianceMapSize(int iMapIndex)
{
    int index = iMapIndex;
    for (auto it = m_mapAllianceVector.begin(); it != m_mapAllianceVector.end(); it++) {
        auto itemDict = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(it->first));
        int iZoneIndex = itemDict->valueForKey("areaType")->intValue();
        if(index == iZoneIndex)
        {
            return (int)(it->second).size();
        }
    }
    return 0;

    
}

MiniCityInfo* MiniMapController::getAllianceMapByIndex(int iMapIndex,int iIndex)
{
    if(getAllianceMapSize(iMapIndex) == 0)
    {
        return NULL;
    }
    if(iIndex<0 || iIndex >= getAllianceMapSize(iMapIndex))
    {
        return NULL;
    }
    
    
    for (auto it = m_mapAllianceVector.begin(); it != m_mapAllianceVector.end(); it++) {
        auto itemDict = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(it->first));
        int iZoneIndex = itemDict->valueForKey("areaType")->intValue();
        if(iMapIndex == iZoneIndex)
        {
            int iCityID = it->second[iIndex];

            return getCityPosById(iCityID);
        }
    }

    
    return NULL;

    
    
}

//区域  联盟势力信息
void MiniMapController::logicPowerInfo()
{
   // m_mapAlliancePower;  //init
    m_mapAlliancePower.clear();
    
    
    int itest = 0;
    for (auto it = m_worldCityPos.begin(); it != m_worldCityPos.end(); it++,itest++) {
        string iAlliance =  it->second.m_AllianceID.c_str();
        if(iAlliance == "")
        {
            continue;
        }
        if(m_mapAlliancePower[iAlliance].m_cityVector.size() == 0)
        {
            m_mapAlliancePower[iAlliance].m_AllianceID = iAlliance.c_str();
            m_mapAlliancePower[iAlliance].m_AllianceName = it->second.m_AllianceName;
            m_mapAlliancePower[iAlliance].m_Colour = it->second.m_Colour;
        }
        m_mapAlliancePower[iAlliance].m_cityVector.push_back(it->second.m_Order);
    }
}

int MiniMapController::getPowerInfoSize()
{
    int iSize = (int)m_mapAlliancePower.size();
    return iSize;
}


AlliancePowerInfo* MiniMapController::getPowerInfoByIndex(int iIndex)
{
    int index = 0;
    for (auto it = m_mapAlliancePower.begin(); it != m_mapAlliancePower.end(); it++) {
        if(index == iIndex)
        {
            return &(it->second);
        }
        index++;
    }
    return NULL;
}

AlliancePowerInfo* MiniMapController::getSelfPowerInfo()
{
    if(GlobalData::shared()->playerInfo.getAllianceId()=="")
    {
        return NULL;
    }
    for (auto it = m_mapAlliancePower.begin(); it != m_mapAlliancePower.end(); it++) {
        if(it->second.m_AllianceID == GlobalData::shared()->playerInfo.getAllianceId())
        {
            return &(it->second);
        }
    }
    return NULL;
}

//void MiniMapController::parseServerList(CCObject *obj){
//    auto list = _dict(obj);
//    if(!list->objectForKey("list")){
//        return;
//    }
//    GlobalData::shared()->isServerInfoBack = true;
//    m_serverList.clear();
//    CCArray *arr = dynamic_cast<CCArray*>(list->objectForKey("list"));
//    if(arr){
//        int num = arr->count();
//        int unitNum = ceil(num * 1.0 / CITY_NUM_PER_UNIT);
//        int totalNum = unitNum * NUM_PER_UNIT;
//        int xNum = ceil(sqrt(totalNum)) + 2 * ADD_NUM;
//        xNum = (xNum + 1) / 2;
//        auto currentPt = ccp(int(xNum * 3 / 2) - 1, int(xNum * 3 / 2));
//        auto centerPt = ccp(int(xNum * 3 / 2), int(xNum * 3 / 2));;
//        int direction = 0;//0 down 1 left 2 up 3 right
//        int totalDirection = 4;
//        int totalPerSide = 0;
//        int count = 1;
//        
//        auto getCityIconIndex = [](CCPoint &centerPt, CCPoint &currentPt){
//            int dy = centerPt.y - currentPt.y;
//            int dx = centerPt.x - currentPt.x;
//            int cityIconIndex = ((3 + dy * 2 + dx) % 6 + 6) % 6 + 1;
//            return cityIconIndex = cityIconIndex;
//        };
//        
//        int index = 0;
//        while(index < arr->count()){
//            auto dict = _dict(arr->objectAtIndex(index));
//            ServerListInfo info = ServerListInfo();
//            info.serverId = dict->valueForKey("id")->intValue();
//            
//            if(dict->objectForKey("name")){
//                info.serverName = dict->valueForKey("name")->getCString();
//            }else{
//                info.serverName = CCString::createWithFormat("Kingdom#%d", info.serverId)->getCString();
//            }
//
//            if(dict->objectForKey("hot")){
//                info.isHot = true;
//            }else{
//                info.isHot = false;
//            }
//            
//            if(dict->objectForKey("banner")){
//                info.banner = dict->valueForKey("banner")->getCString();
//                if(info.banner == ""){
//                    info.banner = "UN";
//                }
//            }else{
//                info.banner = "UN";
//            }
////            info.allianceName = dict->valueForKey("kingAllianceName")->getCString();
//            if(dict->objectForKey("abbr")){
//                info.abbr = dict->valueForKey("abbr")->getCString();
//            }else{
//                info.abbr = "";
//            }
////            info.allianceFlag = dict->valueForKey("allianceIcon")->getCString();
//            if(dict->objectForKey("kingName")){
//                info.kingName = dict->valueForKey("kingName")->getCString();
//            }else{
//                info.kingName = "";
//            }
//            if(dict->objectForKey("enemyList")){
//                CCArray* arr = dynamic_cast<CCArray*>(dict->objectForKey("enemyList"));
//                if(arr!=NULL){
//                    int num = arr->count();
//                    for(int k=0;k<num;k++){
//                        ServerEnemyInfo enemyInfo;
//                        auto oneEnemy = _dict(arr->objectAtIndex(k));
//                        enemyInfo.serverId = oneEnemy->valueForKey("serverId")->intValue();
//                        enemyInfo.fightState = (ServerStateType)oneEnemy->valueForKey("fightState")->intValue();
//                        info.enemyList.push_back(enemyInfo);
//                        
//                    }
//                }
//            }
//            if(dict->objectForKey("fightState")){
//                info.fightState = (ServerStateType)dict->valueForKey("fightState")->intValue();
//            }else{
//                info.fightState = PEACE;
//            }
//            if(info.serverId == GlobalData::shared()->playerInfo.selfServerId){
//                GlobalData::shared()->playerInfo.serverName = info.serverName;
//            }
//            int iconIndex = 1;
//            if(index == 0){
//                iconIndex = getCityIconIndex(centerPt, currentPt);
//                info.cityIconIndex = iconIndex;
//                info.x = currentPt.x;
//                info.y = currentPt.y;
//                m_serverList.push_back(info);
//                index++;
//                continue;
//            }
//            int addX = 0;
//            int addY = 0;
//            int sideLen = totalPerSide;
//            if(direction == 0){
//                addY = -1;
//                sideLen = totalPerSide + 1;
//            }else if(direction == 1){
//                addX = -1;
//                sideLen = totalPerSide - 1;
//            }else if(direction == 2){
//                addY = 1;
//            }else if(direction == 3){
//                addX = 1;
//            }
//            
//            currentPt = currentPt + ccp(addX, addY);
//            iconIndex = getCityIconIndex(centerPt, currentPt);
//            info.cityIconIndex = iconIndex;
//            info.x = currentPt.x;
//            info.y = currentPt.y;
//            m_serverList.push_back(info);
//            
//            ++count;
//            if(count >= sideLen){
//                count = 0;
//                direction = ++direction % totalDirection;
//                if(direction == 1){
//                    totalPerSide = totalPerSide / 2 * 2 + 2;
//                }
//            }
//            
//            ++index;
//            CCLOG("index:%d, %d", info.x, info.y);
//        }
//    }
//}

//void MiniMapController::sendGetTreasureCMD(int itemid){
//    if(ToolController::getInstance()->m_toolInfos.find(itemid)!=ToolController::getInstance()->m_toolInfos.end()){
//        WorldTreasureMapGetCommand *cmd = new WorldTreasureMapGetCommand(CC_ITOA(itemid));
//        cmd->sendAndRelease();
//    }
//}
//void MiniMapController::speedupTreasureMap(string marchid){
//    WorldTreasureMapSpeedupCommand *cmd = new WorldTreasureMapSpeedupCommand(marchid);
//    cmd->sendAndRelease();
//}
