//
//  WorldCommand.h
//  IF
//
//  Created by 邹 程 on 14-2-18.
//
//

#ifndef __IF__WorldCommand__
#define __IF__WorldCommand__

#include "CommandBase.h"
#include "WorldController.h"
#include "GuideController.h"
#include "md5.h"
#include <ctime>

#define MD5CONSTSTR "wNSc5Mm8YwiOhAab"
#define WORLD_RESOURCE_UPDATE_COMMAND "common.res.syn"

#define GET_MARCH_DETAIL_UPDATE_COMMAND "GET_MARCH_DETAIL_UPDATE_COMMAND"

class WorldResourceUpdateCommand: public CommandBase {
public:
    WorldResourceUpdateCommand(const string& marchUuid):
    CommandBase(WORLD_RESOURCE_UPDATE_COMMAND)
    ,m_uuid(marchUuid)
    {
        putParam("uuid", CCString::create(marchUuid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string m_uuid;
};



#define MARCH_DETAIL_COMMAND "world.user.army"
class WorldMarchDetailCommand: public CommandBase {
public:
    WorldMarchDetailCommand(unsigned int index):
    CommandBase(MARCH_DETAIL_COMMAND)
    ,m_index(index)
    {
        putParam("point", CCInteger::create(index));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    unsigned int m_index;
};


#define MARCH_DETAIL_UPDATE_COMMAND "world.get.march"
class WorldMarchDetailUpdateCommand: public CommandBase {
public:
    WorldMarchDetailUpdateCommand(const string& uuid):
    CommandBase(MARCH_DETAIL_UPDATE_COMMAND)
    ,m_uuid(uuid)
    {
        putParam("uuid", CCString::create(uuid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    const string  m_uuid;
};


#define WORLD_GET_OCCUPIED_TROOPS "world.get.occupied.troops"
class WorldGetOccupiedTroopsCommand: public CommandBase {
public:
    WorldGetOccupiedTroopsCommand(int pointId):
    CommandBase(WORLD_GET_OCCUPIED_TROOPS)
    ,m_pointId(pointId)
    {
        putParam("pointId", CCInteger::create(m_pointId));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    int m_pointId;
};



#define WORLD_COMMAND "world.get"
class WorldCommand: public CommandBase {
public:
    WorldCommand(const CCPoint& centerPoint,int time,unsigned int type = 0,int forceServerId = -1):
    CommandBase(WORLD_COMMAND)
    ,m_point(centerPoint)
    ,m_timeStamp(time)
    ,m_type(type)
    {
        putParam("x", CCInteger::create((int)m_point.x));
        putParam("y", CCInteger::create((int)m_point.y));
        putParam("timeStamp", CCInteger::create(m_timeStamp));
        putParam("type", CCInteger::create(type)); // 0 : normal 1 : single point update
        if(GuideController::share()->getCurrentId() == "3030200" && GuideController::share()->arr->count() == 0){
            putParam("guide", CCInteger::create(1));
        }
        if (WorldController::getInstance()->openMarchId != "") {
            putParam("extendMarchUuid", CCStringMake(WorldController::getInstance()->openMarchId));
        }
        int sid = GlobalData::shared()->playerInfo.currentServerId;
        if(forceServerId != -1){
            sid = forceServerId;
        }
        m_serverId = sid;
        CCLOG("server=%d x=%f y=%f",sid,m_point.x,m_point.y);
        putParam("serverId", CCInteger::create(sid));
        WorldController::getInstance()->setisAsyEnd(false);
        m_startTime = CCCommonUtils::getLocalTime();
    }
public:
    int getServerId(){
        return m_serverId;
    };
    
    void sendAndRelease()
    {
        CommandBase::sendAndRelease();        
    };
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    int m_timeStamp;
    CCPoint m_point;
    int m_type;
    int m_serverId;
    double m_startTime;//统计用
};


#define WORLD_LEAVE_COMMAND "world.leave"
class WorldLeaveCommand: public CommandBase {
public:
    WorldLeaveCommand():
    CommandBase(WORLD_LEAVE_COMMAND)
    {}
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};



#define WORLD_DETAIL_COMMAND "world.get.detail"
#define MSG_WORLD_DEFAIL_VIEW "msg.world.defail.view"
class WorldDetailCommand: public CommandBase {
public:
    WorldDetailCommand(unsigned int index,int serverId):
    CommandBase(WORLD_DETAIL_COMMAND)
    ,m_point(index)
    {
        putParam("point", CCInteger::create(m_point));
        putParam("serverId", CCInteger::create(serverId));
        CCLOG("logwdz+++++worldDetailCommand point+++++%d",m_point);
        CCLOG("logwdz+++++worldDetailCommand serverId+++++%d",serverId);
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    unsigned int m_point;
};


#define WORLD_BUY_PVE_COMMAND "world.pve.count.buy"
class WorldBuyPVECommand: public CommandBase {
public:
    WorldBuyPVECommand(unsigned int index):
    CommandBase(WORLD_BUY_PVE_COMMAND)
    ,m_point(index)
    {
        putParam("point", CCInteger::create(m_point));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    unsigned int m_point;
};


#define WORLD_MARCH_COMMAND "world.march"
class WorldMarchCommand: public CommandBase {
public:
    WorldMarchCommand(unsigned int index,MarchMethodType marchType,unsigned int ownerFlag = 1,CCDictionary* army = NULL,CCArray* resArr = NULL,int monsterIndex = -1, CCArray *generals = NULL,int wtIndex=-1,std::string teamId="",int warehouse = -1/*是否从联盟仓库取*/,bool showPanel = false/*是否显示部队面板*/):
    CommandBase(WORLD_MARCH_COMMAND)
    {
        std::string md5Str = std::string(CC_ITOA(int(index))) + "1021" + CC_ITOA((marchType + 1) * 1000);
        putParam("point", CCInteger::create(index));
        putParam("type", CCInteger::create((int)marchType));
        putParam("ownerFlag", CCInteger::create(ownerFlag)); // 0 : no owner 1 : have owner
        if (army) {
            putParam("soldiers", army);
            std::map<int, int> armyMap;
            CCDictElement *ele;
            CCDICT_FOREACH(army, ele){
                std::string key = ele->getStrKey();
                int keyInt = atoi(key.c_str());
                if(dynamic_cast<CCInteger*>(army->objectForKey(key))){
                    int num = dynamic_cast<CCInteger*>(army->objectForKey(key))->getValue();
                    armyMap[keyInt] = num;
                }
            }
            
            for(auto it = armyMap.begin(); it != armyMap.end(); it++){
                int keyInt = it->first;
                std::string key = CC_ITOA(keyInt);
                auto armyIt = GlobalData::shared()->armyList.find(key);
                if(armyIt == GlobalData::shared()->armyList.end()){
                    continue;
                }
                int num = it->second;
                std::string load = CC_ITOA(armyIt->second.load);
                std::string speed = CC_ITOA(armyIt->second.speed);
                md5Str = md5Str + key + speed + load + CC_ITOA(num);
            }
        }
        if (resArr) {
            if (warehouse == -1)
                putParam("res", resArr);
            else if (warehouse == 0)
                putParam("retrieveRes", resArr);
            
            putParam("soldiers", CCDictionary::create());
        }
        if (monsterIndex != -1) {
            putParam("ept", CCInteger::create(monsterIndex));
        }
        
        if(generals){
            putParam("generals", generals);
        }
        if(wtIndex!=-1){
            putParam("wtIndex", CCInteger::create(wtIndex));
        }
        if(teamId!=""){
            putParam("team", CCString::create(teamId));
        }
        m_type = marchType;
        usePower= 0;
        if (m_type == MethodFieldMonster || MethodChristmasMarch == m_type) {
            usePower = WorldController::getInstance()->getMonsterUsePower(index);
        }
        md5Str += MD5CONSTSTR;
        MD5 md5;
        md5.update(md5Str.c_str(), md5Str.length());
        std::string md5Key = md5.toString();
        putParam("marchKey", CCString::create(md5Key));
        m_showPanel = showPanel;
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    MarchMethodType m_type;
    int usePower;
    bool m_showPanel;
};

#define WORLD_MARCH_SPEED_UP_COMMAND "world.march.spd"
class WorldMarchSpeedUpCommand: public CommandBase {
public:
    WorldMarchSpeedUpCommand(const string& uuid,const string& itemUuid):
    CommandBase(WORLD_MARCH_SPEED_UP_COMMAND)
    {
        putParam("uuid", CCString::create(uuid));
        putParam("itemUUid", CCString::create(itemUuid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#define WORLD_MARCH_CANCEL_COMMAND "world.march.retreat"
class WorldMarchCancelCommand: public CommandBase {
public:
    WorldMarchCancelCommand(const string& uuid,const string& itemUuid):
    CommandBase(WORLD_MARCH_CANCEL_COMMAND)
    {
        putParam("uuid", CCString::create(uuid));
        putParam("itemUUid", CCString::create(itemUuid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};


#define WORLD_ADD_FAVORITE_COMMAND "world.favo.add"

class WorldAddFavoriteCommand: public CommandBase {
public:
    WorldAddFavoriteCommand(const std::string &name,unsigned int index,unsigned int type,int serverId,int citytype,int restype) :
    CommandBase(WORLD_ADD_FAVORITE_COMMAND)
    ,m_point(index)
    ,m_name(name)
    ,m_type(type)
    ,m_citytype(citytype)
    ,m_restype(restype)
    {
        putParam("name", CCString::create(name));
        putParam("point",CCInteger::create(index));
        putParam("type", CCInteger::create(type));
        putParam("server", CCInteger::create(serverId));
        putParam("cityType", CCInteger::create(citytype));
        putParam("resType", CCInteger::create(restype));
        m_serverType = (ServerType)GlobalData::shared()->serverType;
        if(m_serverType==SERVER_BATTLE_FIELD && GlobalData::shared()->playerInfo.selfServerId != GlobalData::shared()->playerInfo.currentServerId){
            m_serverType = SERVER_NORMAL;
        }
        putParam("serverType", CCInteger::create(m_serverType));
        m_serverId = serverId;
    }
    
protected:
    unsigned int m_point;
    std::string m_name;
    unsigned int m_type;
    int m_serverId;
    int m_citytype;
    int m_restype;
    ServerType m_serverType;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#define WORLD_DEL_FAVORITE_COMMAND "world.favo.del"

class WorldDelFavoriteCommand: public CommandBase {
public:
    WorldDelFavoriteCommand(unsigned int index, int serverId) :
    CommandBase(WORLD_DEL_FAVORITE_COMMAND)
    ,m_point(index)
    {
        putParam("point",CCInteger::create(index));
        putParam("server", CCInteger::create(serverId));
        m_server = serverId;
    }
    
protected:
    unsigned int m_point;
    int m_server;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#define WORLD_DEL_FAVOR_MULTI_COMMAND "world.favo.delme"

class WorldDelFavormultiCommand: public CommandBase {
public:
    WorldDelFavormultiCommand(string strcmd ,CCArray* para, CCArray* data):
    CommandBase(WORLD_DEL_FAVOR_MULTI_COMMAND)
     ,m_strcmd(strcmd)
    {
        if(para->count() > 0){
            putParam("point", para);
        }
        if(data->count() > 0){
            putParam("server", data);
        }
    }
    
protected:
    string m_strcmd;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#define WORLD_GET_FAVORITE_COMMAND "world.favo.get"

class WorldGetFavoriteCommand: public CommandBase {
public:
    WorldGetFavoriteCommand() :
    CommandBase(WORLD_GET_FAVORITE_COMMAND)
    {
        putParam("server", CCInteger::create(GlobalData::shared()->playerInfo.currentServerId));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};



#define WORLD_SCOUT_DETAIL_COMMAND "world.scout.detail"

class WorldScoutDetailCommand: public CommandBase {
public:
    WorldScoutDetailCommand(unsigned int index) :
    CommandBase(WORLD_SCOUT_DETAIL_COMMAND)
    ,m_point(index)
    {
        putParam("point", CCInteger::create(index));
    }
    
protected:
    unsigned int m_point;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};



#define WORLD_CITY_MOVE_COMMAND "world.mv"

class WorldCityMoveCommand: public CommandBase {
public:
    WorldCityMoveCommand(unsigned int index,const string& itemUuid) :
    CommandBase(WORLD_CITY_MOVE_COMMAND)
    ,m_point(index)
    ,m_mailUid("")
    ,m_alMoveType(0)
    {
        putParam("point", CCInteger::create(index));
        putParam("itemUUid", CCString::create(itemUuid));
    }
    
    void addMailUid(string mailUid);
    void setAlMovType(int type);
protected:
    string m_mailUid;
    int m_alMoveType;
    unsigned int m_point;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#define WORLD_ADD_ITEM_COMMAND "world.use"

class WorldAddItemCommand: public CommandBase {
public:
    WorldAddItemCommand(unsigned int index,const string& itemUuid) :
    CommandBase(WORLD_ADD_ITEM_COMMAND)
    ,m_point(index)
    {
        putParam("point", CCInteger::create(index));
        putParam("itemUUid", CCString::create(itemUuid));
    }
    
protected:
    unsigned int m_point;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};


#define  WORLD_AAREA_CREATE "territory.create"

class WorldAllianceAreaCreateCommand: public CommandBase{
public:
    WorldAllianceAreaCreateCommand(unsigned int index, string& uid):
    CommandBase(WORLD_AAREA_CREATE)
    ,m_point(index)
    ,m_uid(uid)
    {
        putParam("point", CCInteger::create(index));
        putParam("uid", CCString::create(uid));
    }
protected:
    int m_point;
    string m_uid;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#define  WORLD_MONSTER_CREATE "create.boss"

class WorldMonsterCreateCommand: public CommandBase{
public:
    WorldMonsterCreateCommand(unsigned int index, string& uid,int num):
    CommandBase(WORLD_MONSTER_CREATE)
    ,m_point(index)
    ,m_uid(uid)
    ,m_num(num)
    {
        putParam("num", CCInteger::create(num));
        putParam("pointIndex", CCInteger::create(index));
        putParam("uuid", CCString::create(uid));
    }
protected:
    int m_point;
    string m_uid;
    int m_num;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#define  WORLD_AAREA_CANCEL "territory.cancel"

class WorldAllianceAreaCancelCommand: public CommandBase{
public:
    WorldAllianceAreaCancelCommand(unsigned int index, string& uid):
    CommandBase(WORLD_AAREA_CANCEL)
    ,m_point(index)
    ,m_uid(uid)
    {
        putParam("point", CCInteger::create(index));
        putParam("uid", CCString::create(uid));
    }
protected:
    int m_point;
    string m_uid;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
};


#define WORLD_TERRITORY_RES_CREATE "territory.resource.create"

class WorldTerritoryResCreateCommand :public CommandBase{
public:
    WorldTerritoryResCreateCommand(unsigned int index, WorldResourceType resType):
    CommandBase(WORLD_TERRITORY_RES_CREATE)
    ,m_point(index)
    ,m_resType(resType)
    {
        putParam("point", CCInteger::create(index));
        putParam("resType", CCInteger::create(resType));
    }
protected:
    int m_point;
    WorldResourceType m_resType;
    virtual bool handleRecieve(cocos2d::CCDictionary* pDic);
};
#define WORLD_TERRITORY_RES_CANCEL "territory.resource.cancel"

class WorldTerritoryResCancelCommand :public CommandBase{
public:
    WorldTerritoryResCancelCommand(std::string uid, unsigned int index, WorldResourceType resType):
    CommandBase(WORLD_TERRITORY_RES_CANCEL)
    ,m_uid(uid)
    ,m_point(index)
    ,m_resType(resType)
    {
        putParam("uid", CCString::create(uid));
        putParam("point", CCInteger::create(index));
        putParam("resType", CCInteger::create(resType));
    }
protected:
    std::string m_uid;
    int m_point;
    WorldResourceType m_resType;
    virtual bool handleRecieve(cocos2d::CCDictionary* pDic);
};

#define WORLD_TOWER_CREATE "territory.turret.create"

class WorldTowerCreateCommand :public CommandBase{
public:
    WorldTowerCreateCommand(unsigned int index,string uid):
    CommandBase(WORLD_TOWER_CREATE)
    ,m_point(index)
    ,m_uid(uid)
    {
        putParam("point", CCInteger::create(index));
        putParam("uid", CCStringMake(uid));
    }
protected:
    int m_point;
    string m_uid;
    virtual bool handleRecieve(cocos2d::CCDictionary* pDic);
};
#define WORLD_TOWER_CANCEL "territory.turret.cancel"

class WorldTowerCancelCommand :public CommandBase{
public:
    WorldTowerCancelCommand(std::string uid, unsigned int index):
    CommandBase(WORLD_TOWER_CANCEL)
    ,m_uid(uid)
    ,m_point(index)
    {
        putParam("uid", CCString::create(uid));
        putParam("point", CCInteger::create(index));
    }
protected:
    std::string m_uid;
    int m_point;
    virtual bool handleRecieve(cocos2d::CCDictionary* pDic);
};

#define WORLD_TERRITORY_WAREHOUSE_CREATE "territory.warehouse.create"

class WorldTerritoryWarehouseCreateCommand :public CommandBase{
public:
    WorldTerritoryWarehouseCreateCommand(unsigned int index):
    CommandBase(WORLD_TERRITORY_WAREHOUSE_CREATE)
    ,m_point(index)
    {
        putParam("point", CCInteger::create(index));
    }
protected:
    int m_point;
    virtual bool handleRecieve(cocos2d::CCDictionary* pDic);
};
#define WORLD_TERRITORY_WAREHOUSE_CANCEL "territory.warehouse.cancel"

class WorldTerritoryWarehouseCancelCommand :public CommandBase{
public:
    WorldTerritoryWarehouseCancelCommand(std::string uid, unsigned int index):
    CommandBase(WORLD_TERRITORY_WAREHOUSE_CANCEL)
    ,m_uid(uid)
    ,m_point(index)
    {
        putParam("uid", CCString::create(uid));
        putParam("point", CCInteger::create(index));
    }
protected:
    std::string m_uid;
    int m_point;
    virtual bool handleRecieve(cocos2d::CCDictionary* pDic);
};

#define WORLD_TERRITORY_BANNER_CREATE "territory.flag.create"

class WorldTerritoryBannerCreateCommand :public CommandBase{
public:
    WorldTerritoryBannerCreateCommand(unsigned int index, std::string banner):
    CommandBase(WORLD_TERRITORY_BANNER_CREATE)
    ,m_point(index)
    ,m_banner(banner)
    {
        putParam("point", CCInteger::create(index));
        putParam("flag", CCString::create(banner));
    }
protected:
    int m_point;
    std::string m_banner;
    virtual bool handleRecieve(cocos2d::CCDictionary* pDic);
};
#define WORLD_TERRITORY_BANNER_CANCEL "territory.flag.cancel"

class WorldTerritoryBannerCancelCommand :public CommandBase{
public:
    WorldTerritoryBannerCancelCommand(std::string uid, unsigned int index):
    CommandBase(WORLD_TERRITORY_BANNER_CANCEL)
    ,m_uid(uid)
    ,m_point(index)
    {
        putParam("point", CCInteger::create(index));
        putParam("uid", CCString::create(uid));
    }
protected:
    std::string m_uid;
    int m_point;
    virtual bool handleRecieve(cocos2d::CCDictionary* pDic);
};

#define WORLD_CROSS_MOVE_COMMAND "cross.world.mv"

class WorldCrossCityMoveCommand: public CommandBase {
public:
    WorldCrossCityMoveCommand(unsigned int index,const string& itemUuid, int serverId) :
    CommandBase(WORLD_CROSS_MOVE_COMMAND)
    ,m_point(index)
    {
        putParam("point", CCInteger::create(index));
        putParam("itemUUid", CCString::create(itemUuid));
        putParam("serverId", CCInteger::create(serverId));
        m_serverId = serverId;
    }
    
protected:
    unsigned int m_point;
    int m_serverId;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#define MARCH_CHECK_COMMAND "world.user.march"

class WorldMarchTimeUpCheckCommand: public CommandBase {
public:
    WorldMarchTimeUpCheckCommand() : CommandBase(MARCH_CHECK_COMMAND){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};


#define WORLD_CITY_MOVE_GOLD_COMMAND "world.mv.gold"

class WorldCityMoveGoldCommand: public CommandBase {
public:
    WorldCityMoveGoldCommand(unsigned int index) :
    CommandBase(WORLD_CITY_MOVE_GOLD_COMMAND)
    ,m_point(index)
    ,m_mailUid("")
    {
        putParam("point", CCInteger::create(index));
    }
    
    void addMailUid(string mailUid);
    
protected:
    string m_mailUid;
    unsigned int m_point;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};


#define WORLD_TREASUREMAP_GET_COMMAND "treasuremap.get"
class WorldTreasureMapGetCommand: public CommandBase {
public:
    WorldTreasureMapGetCommand(string itemid) :
    CommandBase(WORLD_TREASUREMAP_GET_COMMAND)
    ,mitemid(itemid)
    {
        putParam("itemid", CCString::create(itemid));
    }
    
protected:
    string mitemid;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};
#define WORLD_TREASUREMAP_DESTROY_COMMAND "treasuremap.destroy"
class WorldTreasureMapDestroyCommand: public CommandBase {
public:
    WorldTreasureMapDestroyCommand(string treasureUUID) :
    CommandBase(WORLD_TREASUREMAP_DESTROY_COMMAND)
    ,mtreasureUUID(treasureUUID){
        putParam("treasureuuid", CCString::create(treasureUUID));
    }
protected:
    string mtreasureUUID;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};
#define WORLD_TREASUREMAP_SPD_COMMAND "treasuremap.spd"
class WorldTreasureMapSpeedupCommand: public CommandBase {
public:
    WorldTreasureMapSpeedupCommand(string marchid) :
    CommandBase(WORLD_TREASUREMAP_SPD_COMMAND),mmarchid(marchid){
        putParam("marchid", CCString::create(marchid));
    }
protected:
    string mmarchid;
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};




#define MARCH_OCCUPIED_KICK_COMMAND "occupied.march.kick"
class WorldMarchKickCommand: public CommandBase {
public:
    WorldMarchKickCommand(const string& uuid):
    CommandBase(MARCH_OCCUPIED_KICK_COMMAND)
    ,m_uuid(uuid)
    {
        putParam("uuid", CCString::create(uuid));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    const string  m_uuid;
};

//test city
#define MAP_GET_MAINCITY_COMMAND "map.get.maincity"
class MapGetMainCityCommand: public CommandBase {
public:
    MapGetMainCityCommand():
    CommandBase(MAP_GET_MAINCITY_COMMAND)
     
    {
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);

private:
    
};

//test city
#define MAP_GET_CITY_RES_COMMAND "world.get.cityresource"
class MapGetCityResourceCommand: public CommandBase {
public:
    MapGetCityResourceCommand(int  point):
    CommandBase(MAP_GET_CITY_RES_COMMAND)
    ,m_point(point)
    {
        putParam("point", CCInteger::create(point));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    unsigned int   m_point;
};

//city main info
#define MAP_GET_CITY_MAIN_COMMAND "get.maincity.info"
class MapGetCityMainCommand: public CommandBase {
public:
    MapGetCityMainCommand():
    CommandBase(MAP_GET_CITY_MAIN_COMMAND)
    {
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:

};
//city small info
#define MAP_GET_CITY_SMALL_COMMAND "get.smallcity.info"
class MapGetCitySmallCommand: public CommandBase {
public:
    MapGetCitySmallCommand():
    CommandBase(MAP_GET_CITY_SMALL_COMMAND)
    {
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
    
};



#endif /* defined(__IF__WorldCommand__) */
