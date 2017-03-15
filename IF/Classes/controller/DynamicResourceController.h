//
//  DynamicResourceController.h
//  IF
//
//  Created by Émilie.Jiang on 15/3/25.
//
//

#ifndef __IF__DynamicResourceController__
#define __IF__DynamicResourceController__

#include "CommonInclude.h"


#define MSG_DYNAMIC_DOWNLOAD_FINISH "msg.dynamic.download.finish"
#define DynamicResource_Folder "dresource/"
#define DynamicSound_Folder "sounds/"

#define DynamicResource_GOODS_TEXTURE "goods"
#define DynamicResource_MONSTER_TEXTURE "monster"
#define DynamicResource_SERVERFIGHT_TEXTURE "ckf_map"
#define DynamicResource_MONSTER_TEXTURE_layer "monsterLayer"
#define DynamicResource_HEAD_TEXTURE_ICON "HeadIcon"
#define DynamicResource_HEAD_TEXTURE_ICON_BUST "HeadIcon_bust"
#define DynamicResource_HEAD_TEXTURE_ICON_MIDDLE "HeadIcon_middle"
#define DynamicResource_MINIMAPVIEW_TEXTURE "miniMapView"
#define DynamicResource_THORNE_TEXTURE "thorne"
#define DynamicResource_HD_CCB "hdccb"
#define DynamicResource_HD_RES "hdres"
#define DynamicResource_DRAGON_MINI_MAP_TEXTURE "DragonMinimap"
#define DynamicResource_ACTIVITY_AD_TEXTURE "activity_ad"
#define DynamicResource_UI_RES "uires"
#define DynamicResource_DRAGON_BUILDING "dragon_building"
#define DynamicResource_DRAGON_MAP "dragon_map"
#define DynamicResource_ACTIVITY_RES "activity_res"
#define DynamicResource_EQUIPMENT_TEXTURE "equipment"
#define DynamicResource_SCIENCE_TEXTURE "science"

enum DynamicResourceType{
    DynamicResourceType_None,
    DynamicResourceType_Version,
    DynamicResourceType_Name
};

struct DynamicResource{
    std::string name;//资源名称
    std::string oldV;//老版本号
    std::string newV;//新版本号
    std::string md5;//
    int needSize;//检测大小
    bool platform;
    DynamicResourceType type;
};

//HTREAD PARAMS
class DynamicResourceParam
{
public:
    DynamicResource drr;
    bool _isRestart;
    bool _downret;
};

class DynamicResourceController:public CCObject{
public:
    DynamicResourceController();    // for lua use
    static DynamicResourceController* getInstance();
    string getDownloadPath();
    
    bool InitServerRes(CCDictionary* ccDict);
    
    void initGoodsInfo(CCDictionary* ccDict);
    bool checkGoodsResource();
    
    void initEquipmentInfo(__Dictionary* ccDict);
    bool checkEquipmentResource();
    
    void initMonsterInfo(CCDictionary* ccDict);
    bool checkMonsterResource();
    
    void initServerFightInfo(CCDictionary* ccDict);
    bool checkServerFightResource();
    
    void initMinimapViewInfo(CCDictionary* ccDict);
    bool checkMinimapViewResource();
    
//    lzy 高清ccb
    void initHDCCBInfo(CCDictionary* ccDict);
    bool checkHDCCB();
    
    // xxr uires
    void initUIRESInfo(CCDictionary*);
    bool checkUIRES();
    
    void initHDResourceInfo(CCDictionary* ccDict);
    bool checkHDResource();
    
    void initMonsterLayerInfo(CCDictionary* ccDict);
    bool checkMonsterLayerRes();
    
    void initHeadIconInfo(CCDictionary* ccDict);
    bool checkHeadIconRes();

    void initHeadIcon_bustInfo(CCDictionary* ccDict);
    bool checkHeadIcon_bustRes();
    
    void initHeadIcon_middleInfo(CCDictionary* ccDict);
    bool checkHeadIcon_middleRes();
    
    void initThorneInfo(CCDictionary *dict);
    bool checkThorneResource();
    // dragon
    void initDragonMiniMapInfo(CCDictionary* ccDict);
    bool checkDragonMiniMapResource();
    
    // activity_ad
    void initActivityAdInfo(CCDictionary* ccDict);
    bool checkActivityAdResource();
    
    // activity_res
    void initActivityResInfo(__Dictionary* dict);
    bool checkActivityResource();
    
    //dragon Building
    void initDragonBuildingInfo(CCDictionary* ccDict);
    bool checkDragonBuildingResource();
    //dragon map
    void initDragonMapInfo(CCDictionary* ccDict);
    bool checkDragonMapResource();
    
    // science
    void initScienceInfo(CCDictionary* ccDict);
    bool checkScienceResource();
    
    void sendGetCommonCMD();
    void downLoadAllCommon();
    void initCommonInfo(CCObject* ccDict);
    bool checkCommonResource(string name);
    
    bool checkGeneralResource(string name);
    //sound
    void checkSoundResource();
    void downloadSoundResource(string name);
    
    //maincity
    bool checkMainCityResource(string name, string md5);
    void initMainCityConfig(string name,map<string,string> &skinMap);

    // .
    bool hasNameTypeResource(std::string name,int needSize,std::string md5,bool platform=true);
    bool hasVersionTypeResource(std::string name,std::string newV,int needSize,std::string md5);
    
    //check device
    void sendDeviceLog();
    void loadNameTypeResource(std::string name, bool release, bool hasAlpha = true);
    void uncompressLocalZip(int type);

    
protected:
    bool checkResourceFile(std::string fileName,bool platform = true);
    void uncompressZip(CCObject *ccObj);
    void uncompressLocalZipInThread(CCObject *ccObj);
    
    ///////////////////////////////////////////////////
    // 下载线程环境相关函数
    void DownloadThread(CCObject *ccObj);
    bool getDownloadUrl(DynamicResource const& dr, std::string& url);
    bool DownAndExtrace(DynamicResourceParam* drparams);
    ///////////////////////////////////////////////////
    
    // 开始下载
    void startDownload(DynamicResource ddr);
    void DownloadRet(CCObject *ccObj);
    void saveClientVersion(DynamicResourceParam* param);
    void dealwithError(DynamicResourceParam* param);
    
    bool isDownloading(std::string const& name);
    bool removeDownloading(std::string const& name);
    
private:
    std::string m_writeablePath;

    // 正在下载列表
    vector<string> mQueue;

    // 服务器资源列表
    vector<DynamicResource> mServerQueue;
    
    CC_SYNTHESIZE_READONLY(bool, m_isInitedCommon, InitedCommon);
    
    void saveCommonStatus(int idx,bool downloaded);
    void saveGeneralStatus(int idx,bool downloaded);

    //md5类型的文件版本号信息，存放在dresource里
    // 当资源要放在sdcard的时候可能需要这个
    std::map<string,string> m_md5Map;
    void initMD5Map();
    void saveMD5Map();
    
    //send device
    CC_SYNTHESIZE_BOOL(m_checkDevice, CheckDevice);
};


#endif /* defined(__IF__DynamicResourceController__) */
