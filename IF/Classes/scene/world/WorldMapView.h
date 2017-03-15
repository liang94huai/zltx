//
//  WorldMapView.h
//  IF
//
//  Created by 邹 程 on 13-11-19.
//
//

#ifndef __IF__WorldMapView__
#define __IF__WorldMapView__

#include "CommonInclude.h"
#include "WorldController.h"
#include "BaseTileInfo.h"
#include "HFViewport.h"
#include "WorldCommand.h"
#include "CCLabelBatch.h"
#include <deque>
#include "CCLineBatchedSprite.h"
#include "NewWorldMarchArmy.h"
#include "NewBaseTileInfo.h"
#include "IFSkeletonBatchLayer.h"
#include "IFHeiqishiNode.h"
#define LAYER_COUNT  7
#define  MSG_SHAKETIME              "shakeTime"
#define  MSG_SHAKECMDTIME              "shakeCmdTime"
#define  MSG_SHAKEGUIDETIME              "shakeGuideTime"
#define  MSG_NOSHOWSHAKE                 "noshowshake"


#define WM_BETWEEN_SERVER_MAP_TAG 100000
#define WM_MASK_TAG 200000
#define WM_COVER_TAG 10000

#define WM_BETWEEN_SERVER_MAP 0
#define WM_CITY      1
#define WM_ROAD      2
#define WM_TILE      3
#define WM_GUI       4
#define WM_POPUP     5
#define WM_COVER     6

#define WM_GUI_TAG 453567
#define WM_POPUP_TAG 453568

#define TILE_LAST_TIME 3000.0   // millisecond
#define TILE_FADE_TIME 1000.0   // millisecond
#define miniMapTag 99999999
#define KING_TIME_TAG 88888888
#define MinimapLoadingTag 66666666
#define DEFAULT_THRONE_INDEX 302

#define WORLD_HD_MAX 1.8
#define WORLD_HD_MIN 1.2
#define WORLD_DEFAULT_HD_SCALE 1.4


enum BatchTagType {
    defaultTag
    ,LevelTag
    ,FireTag
    ,ProtectTag
    ,ResourceProtectTag
    ,SmokeTag
    ,CityMoveInTag
    ,CityMoveOutTag
    ,ShengdanNameBg
    ,NameTag
    ,NameTag1
    ,NameTag2
    ,EdgyTag
    ,EdgyTag1
    ,WorldBorder
    ,WorldBorder1
    ,CityTag
    ,CityMoveInViewTag
    ,MonsterAttack
    ,MonsterAttack1
    ,MonsterDead
    ,MonsterDead1
    ,MonsterBreath
    ,MonsterAppear
    ,Rock
    ,Rock1
    ,Rock2
    ,Rock3
    ,WarFire
    ,MonsterBreath1
    ,MonsterProBG1
    ,AllianceFlag
    ,Shadow
    ,MonsterTalkText
    ,MonsterTalkLine
    ,AllianceParticle
    ,MonsterAttackParticle
    ,TrebuchetWait
    ,TrebuchetAttack
    ,OfficerTagBG
    ,OfficerTag
    ,OfficerTagParticle
    ,WarBuildTextTag1
    ,WarBuildTextTag2
    ,WarBuildTextTag3
    ,WarBuildStateBG
    ,ThroneProtectParticle
    ,Partical_fieldMonster
    ,Partical_AAreaBuild
    ,Partical_mapMask
    ,AllianceTerritoryParticle
    ,CSProtectTag
//    ,TreasureMapCDIcon
    ,TreasureMapCDBarBG
//    ,TreasureMapCDBar
    ,Rs_Spreading
    ,Rs_Flying
    ,Rs_fadein
    ,Rs_fadeout
    ,CityStar
    ,Partical_castleWing
    ,Partical_castleWing2
    ,CityCustomTag
    ,DragonTowerParticleTag
    ,ScoutBgTag
    ,ScoutTag
    ,CrossThroneAttackerNormalTag
    ,CrossThroneAttackerFireTag
    ,SendHeartTag
    ,NewResLevelTag
    ,CityNameDiTag
    ,NewNameTag
    ,NewNameImgTag
    ,ResFlagTag
    ,ResFlagNameTag
    ,ResAreaTag
    ,CccupySpineTag
};

enum MarchAniType {
    // single frame
    AniCityProtect
    ,AniCityResourceProtect
    // double frame
    ,AniCollect
    ,AniCollectCloth
    ,AniScout
    ,AniScoutCloth
    ,AniDeal
    ,AniDealCloth
    ,AniEdgy
    ,AniMonsterAttack
    ,AniMonsterDead
    ,AniMonsterBreath
    ,AniTrebuchetWait
    ,AniTrebuchetAttack
    ,AniAllianceArea
};

struct CellMap{
    int level;
    std::string leagueUid;
    bool isCity;
    double locateTime;
    int cityIndex;
};


class WorldMapView;

class CloudLayer : public CCParallaxNode {
public:
    static CloudLayer* create() {
        CloudLayer *pRet = new CloudLayer();
        pRet->autorelease();
        return pRet;
    }
    
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
};

class DynamicTiledMap : public TMXTiledMap {
public:
    
    static DynamicTiledMap* create(const char *tmxFile,const CCPoint& pos, int loopSize) {
        
        DynamicTiledMap *pRet = new DynamicTiledMap();
        pRet->isChangingServer = false;
        pRet->currentTilePoint = pos;
        pRet->lastDisplayTilePoint = pos;
        pRet->centerTilePoint = ccp((int)(WorldController::getInstance()->_current_tile_count_x/2),(int)(WorldController::getInstance()->_current_tile_count_y/2));
        pRet->centerViewPoint = ccp(WorldController::getInstance()->_current_map_width/2, WorldController::getInstance()->_current_map_height/2);
        CCPoint serverViewPoint =  WorldController::getInstance()->getServerViewPosByPos(WorldController::getInstance()->getServerPosById(GlobalData::shared()->playerInfo.currentServerId));
        if(WorldController::getInstance()->currentMapType == SERVERFIGHT_MAP){
            serverViewPoint = ccp(0, 0);
        }
        //todo liudi
//        pRet->initWithTMXFile(tmxFile,pos,serverViewPoint, loopSize)
        if (pRet->initWithTMXFile(tmxFile,pos,serverViewPoint, loopSize))
        {
            pRet->autorelease();
            return pRet;
        }
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
    
    virtual void setPosition(const CCPoint &position);
    bool isNeedUpdate();
    void updateDynamicMap(CCPoint point = ccp(-1, -1),int forceType = -1);
    void updataBoderMap(CCPoint point,int serverId);
    CCPoint getViewPointByTilePoint(const CCPoint &tilePoint,int forceServerId = -1);
    CCPoint getTilePointByViewPoint(const CCPoint &viewPoint);
    
    CCPoint getTileMapPointByViewPoint(const CCPoint &viewPoint);
    
    int getServerIdByViewPoint(const CCPoint &viewPoint);
    void onShowMapMask();
    CCPoint lastDisplayTilePoint;
    CCPoint currentTilePoint;
    CCPoint centerTilePoint;
    CCPoint centerViewPoint;
    bool isSendCmd;
    bool isChangingServer;
    
//    static Vector<DynamicTiledMap*> mZombieCaches;
//    static void saveToCache(const std::string& key, DynamicTiledMap* );
//    static DynamicTiledMap* loadFromCache(const std::string& key);
};

class CCWorldMapSpriteBatchNode : public CCNode {
    
public:
    CCWorldMapSpriteBatchNode() {};
    
static CCWorldMapSpriteBatchNode* createWithTexture(Texture2D* tex, ssize_t capacity/* = DEFAULT_CAPACITY*/)
    {
        CCWorldMapSpriteBatchNode *batchNode = new (std::nothrow) CCWorldMapSpriteBatchNode();
        //batchNode->initWithTexture(tex, capacity);
        batchNode->autorelease();
        return batchNode;
    }
    
protected:
    bool isInVisibleArea();
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
};

class WorldMapView : public CCLayer,public ITouchDelegate
{
public:
    static WorldMapView* instance();
    static void setInstance(WorldMapView* view);
    static void unsetInstance(WorldMapView* view);
    
    static WorldMapView* create(CCPoint& tilePoint, MapType mapType);
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
    void getCityPoss(WorldCityInfo &info,vector<int> &pos);
    CCNode *getNodeByType(std::string typeStr);
    void leaveWorld();
    void clearPopupView();
    void addPopupView(BaseTileInfo* view);
    void addPopupView2(CCObject* view);
    void addPopupView1(CCObject* view);
    void addPopupView3(NewBaseTileInfo* view);
    void addPopupView4(NewBaseTileInfo* view);
    void addPopupView5(NewBaseTileLuaInfo* view);
    //    void autoAddResourceView(MarchInfo &info);
    void resetClickState(CCObject* obj);
    void updateSelfName();
    bool isMiniMapModel();
    void showTroopInfo(const string  marchId);
    void addCover();
    void removeCover();
    void addCityMoveParticle(CCObject* obj);
    void onCloseTilePopup();
    WorldMapView():
    m_cityInfo(WorldController::getInstance()->m_cityInfo)
    ,m_updateTimeStamp(0)
    ,m_isIdle(true)
    ,m_isIosAndroidPad(false)
    {
        WorldMapView::setInstance(this);
        pthread_mutex_init(&m_addCityLock, NULL);
    }
    void onShowShakeGuide(float time);
    virtual bool init(CCPoint& tilePoint, MapType mapType);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onEnter();
    virtual void onExit();
    virtual void update(float delta);
    
    void showResFlagAni(CCObject* obj);
    void hideGUI(CCObject* obj);
    void showGUI(CCObject* obj);
    void showFirework(cocos2d::CCObject *obj);
    void startShowFirework(int firePosistion,int useItemId);
//    int firePosistion;
//    int m_useItemId;
    void updateGUI(bool setVisible);
    void touchBeganTilePanel(unsigned int index);
    void openTilePanel(unsigned int index);
    void changeServer(int serverId);
    void afterMarchDeploy(CCDictionary* dict,int bType=-1,std::string other="", int targetType=-1);
    void attackChristmas(int cityIndex,MarchMethodType marchType);
    void onExperienceAni(int num);
    void afterMarchSpeedUp(CCDictionary* dict);
    void afterMarchCancel(CCDictionary* dict);
    void afterCityMove(CCDictionary* dict);
    void afterCrossCityMove(CCDictionary* dict);
    void refreshMonster(CCObject *obj);
    void addAttackParticle(CCObject *dict);
    void onMarchCallback(CCObject* obj);
    void onAddResFlag(WorldCityInfo &info,int iCreateType = 0);
    bool updateMarchTarget(MarchInfo &info,double now,float delta);
    
    void gotoTilePoint(const CCPoint &point,bool isForceUpdate = false,int forceServerId = -1);
    void gotoTilePoint(const CCPoint &point, int serverId, bool isForceUpdate = false);
    void smallcityDataBack(CCObject* obj);
    void addFavorite(const std::string &name , unsigned int index, unsigned int type,bool flag = true);
    void onAddFavoriteCallback(CCDictionary *info);
    
    void onCityMoveCallback(CCObject* obj);
    void doTeleport(unsigned int type,unsigned int index,const string& name);
    void pushAddTower(unsigned int type, unsigned int index ,int dialog,string aaid,bool isBanner = false,std::string banner = "");
    void finishTeleport(CCObject* obj);
    void onFinishAddTower(CCObject* obj);
    
    void doAllianceArea(unsigned int type ,unsigned int index,bool isSuperMine = false,bool isWarehouse = false,WorldResourceType resType = Wood,string aaid = "");
    void doCreateMonster(int pointIndex,__Dictionary * dic);
    void finishAllianceArea(CCObject* pObj);
    
    void finishBattleAni(CCObject* obj);
    void onFinishSpinBattle(CCObject* obj);
    
    //送红心 by xxr 2016.1.16
    void addSendHeartParticle(int index, int endstamp, int type, bool force);
    void addSendHeartParticleToBatch(ParticleSystemQuad *particle);
    void removeSendHeartParticle(Ref* ref);
    vector<ParticleBatchNode*> m_sendHeartBatchVec;
    int getHeartParTag(int index, int type, int i);
    
    unsigned int getBatchTag(BatchTagType type,unsigned int index);
    void addBatchItem(BatchTagType type,unsigned int index,CCObject* obj = NULL);
    void delBatchItem(BatchTagType type,unsigned int index);
    void removeWalkParticle(int tag);
    CCAnimate* createMarchAni(MarchAniType type,int direction = 0, float deltaTime = 1.0, std::string para = "");
    CCSprite* createMarchSprite(MarchInfo& info);
    IFHeiqishiNode* createHeiqishiSoldier(MarchInfo& info);
    void trebuchetAttack(CCDictionary *params);
    void resetTrebuchet(CCObject *obj);
    CCPoint getViewCenterTilePoint();
    // ITouchDelegate
    virtual bool onBeginScroll(const cocos2d::CCPoint& worldLocation);
    virtual void onStepScroll(const cocos2d::CCPoint& worldLocation,const cocos2d::CCPoint& delta);
    virtual void onEndScroll(const cocos2d::CCPoint& worldLocation);
    virtual void onSingleTouchBegin(const cocos2d::CCPoint& worldLocation);
    virtual void onSingleTouchEnd(const cocos2d::CCPoint& worldLocation);
    virtual void onBeginZoom(const CCPoint& point1,const CCPoint& point2);
    virtual void onStepZoom(const cocos2d::CCPoint& point1, const cocos2d::CCPoint& point2);
    virtual void onEndZoom(float targetZoom);
    virtual void autoMoveEnd();
    void playFreshResAni(int index);
    int getBestLocIndex(int index);
    bool isFourTileCanRelocate(int index);
    bool isCanRelocationTile(int index);
    void updateWinNum(int index);
    void createWinNum(int index);
    void showBuildingLeftTime(int index);
    void updateBuildingTime();
    //放置资源点
    void gotoAddWorldItems(int itemid);
    void afterAddWorldItems(cocos2d::CCDictionary *dict);
    // 放置传送点显示的范围
    void addTransferArea();
    void initBirthPoint();
    vector<unsigned int> m_cacheTransferTag;//缓存传送点的
    
    CCSafeObject<HFViewport> m_touchDelegateView;
    
    vector<unsigned int> m_cacheToAddCity;
    bool m_inCityUpdate;
    
    vector<unsigned int> m_untouchableTiles;
    
    CCPoint m_directionLBPoint;
    CCPoint m_directionRTPoint;
    CCSafeObject<CCSprite> m_directionIcon;
    CCSafeObject<CCLabelIF> m_directionLabel;
    void updateDirection();
    CCSafeObject<CCSprite> m_loadingIcon;
    
    CCPoint tmpTouchPoint;
    CCPoint selfViewPoint;
    CCSize  halfWinSize;
    
    double m_lastScrollTime;
    
    bool inFollowView;
    string followMarchId;
    
    CCSafeObject<CCNode> m_drawRoadNode;
    CCSafeObject<CCNode> m_touchTroopNode;
    CCSafeObject<CCNode> m_simNameUnbatchNode;
    CCSafeObject<CCNode> m_unBatchLabelNode;//非batch的文本，主要用于名字的显示
    CCSafeObject<CCNode> m_aAreaUnbachLbNode;
    CCSafeObject<CCWorldMapSpriteBatchNode> m_towerRangeMaskNode;
    CCSafeObject<CCNode> m_mapMaskNode;
    CCSafeObject<CCNode> m_mapMaskCloudNode;
    
    CCSafeObject<CCNode> m_HarvestBatch;//收获节点
    CCSafeObject<CCNode> m_flagBatch;//旗帜
    CCSafeObject<CCWorldMapSpriteBatchNode> m_mapMarchNode;//侦查，交易
    CCSafeObject<IFSkeletonBatchLayer> m_mapMonsterNode;
    //    IFSkeletonBatchLayer* m_mapMonsterBossNode;//世界boss
    CCSafeObject<IFSkeletonBatchLayer> m_mapHeiqishiMarchNode;//黑骑士
    CCSafeObject<IFSkeletonBatchLayer> m_wingNode;//wing
    CCSafeObject<CCNode> m_spinLabelNode;//黑骑士 lb
    CCSafeObject<CCWorldMapSpriteBatchNode> m_heiqishiLvBgBachNode;//黑骑士lvBg
    CCSafeObject<CCWorldMapSpriteBatchNode> m_heiqishiShadowBachNode;
    CCSafeObject<CCLabelBatchNode> m_heiqishiLvLbNode;
    CCSafeObject<CCWorldMapSpriteBatchNode> m_csNode;
    CCSafeObject<CCNode> m_lianNode;//连胜名字的显示
    CCSafeObject<CCSpriteBatchNode> m_zhengYingBNode;//显示阵营
    CCSafeObject<CCNode> m_dragonFixBNode;//
    CCSafeObject<CCSpriteBatchNode> m_dragonBirthNode;//
    
    CCSafeObject<CCWorldMapSpriteBatchNode> m_mapArrowSpNode;
    CCSafeObject<CCWorldMapSpriteBatchNode> m_mapMonsterShadowNode;
    CCSafeObject<CCWorldMapSpriteBatchNode> m_mapMarchNode1;// 部队行军
    
    CCSafeObject<CCWorldMapSpriteBatchNode> m_mapTowerBatchNode;
    
    //    CCSpriteBatchNode* m_mapMarchNameBG;// 部队名字底板
    //    CCNode *m_armyNameNode;
    CCSafeObject<CCWorldMapSpriteBatchNode> m_occupyPointerNode;
    CCSafeObject<CCWorldMapSpriteBatchNode> m_line;
    CCSafeObject<CCWorldMapSpriteBatchNode> m_conflictBatchNode;
    CCSafeObject<CCNode> m_cityBatchNode;
    CCSafeObject<CCWorldMapSpriteBatchNode> m_ascriptionBatchNode;
    
    
    CCSafeObject<CCNode> m_aAreaAniNode;
    CCSafeObject<CCNode> m_bannerNode;
    CCSafeObject<CCWorldMapSpriteBatchNode> m_cityAttackNode;
    CCSafeObject<CCNode> m_throneNode;
    CCSafeObject<CCNode> m_towerNode;
    CCSafeObject<CCWorldMapSpriteBatchNode> m_throneTowerNode;
    CCSafeObject<CCWorldMapSpriteBatchNode> m_batchNode;
    CCSafeObject<CCWorldMapSpriteBatchNode> m_dragonBatchNode;
    CCSafeObject<IFSkeletonBatchLayer> m_dragonSkeNode;
    CCSafeObject<Sprite> m_dragonAni;
    CCSafeObject<IFSkeletonBatchLayer> m_kufuWangZhan;//跨服王战，用于显示名字的底板spine

    CCSafeObject<CCNode> m_armyDustParticleNode;
    CCSafeObject<CCNode> m_flagParticleNode;
    
    CCSafeObject<IFSkeletonBatchLayer> m_flagNode;//占领旗子
    CCSafeObject<IFSkeletonBatchLayer> m_CccupySpineNode;//占领SPINE

    
    CCSafeObject<CCLabelBatchNode> m_labelNode;
    CCSafeObject<CCLabelBatchNode> m_labelNode1;
    CCSafeObject<CCNode> tilePopNode;
    vector<CCNode*> m_parBatchVec;
    vector<CCNode*> m_parWalkBatchVec;
    vector<CCNode*> m_parFlagBatchVec;
    
    
    map<unsigned int , vector<CCSafeObject<CCParticleSystemQuad> > > m_particleVec;
    map<unsigned int , vector<CCSafeObject<CCParticleSystemQuad> > > m_walkparticleVec;
    map<unsigned int , vector<CCSafeObject<CCParticleSystemQuad> > > m_flagParticleVec;
    
    unordered_map<unsigned int , vector<CCSafeObject<CCNode> > > m_cityItem;
    map<unsigned int , vector<CCSafeObject<CCNode> > > m_dragonFixItem;
    std::list<unsigned int> m_displayedCity;
    map<unsigned int, MarchArmy*> m_marchArmy;
    DynamicTiledMap* m_map;
    CCTMXLayer* m_tileLayer;
    
    
    CCSafeObject<CCControlButton>  m_btnGoto;
    
    // for city info update
    int m_updateTimeStamp;
    void callBackMapInfo(CCDictionary *param);
    
    void asyncCityInfoParse(CCObject* obj);
    void addCityInMainThread(CCObject* obj);
    void asyncReleaseInMainThread(CCObject* obj);
    void clearAllMarch();
    ~WorldMapView() {
        pthread_mutex_destroy(&m_addCityLock);
        WorldMapView::unsetInstance(this);
    }
    
    void createCity(WorldCityInfo& info);
    void releaseCity(WorldCityInfo& info);
    
    void addParticleToBatch(CCParticleSystemQuad* particle,unsigned int batchTag);
    void addParticleToUnderCity(CCParticleSystemQuad* particle,unsigned int batchTag);
    void addWalkParticleToBatch(CCParticleSystemQuad* particle,unsigned int batchTag);
    void addFlagParticleToBatch(CCParticleSystemQuad* particle,unsigned int batchTag);
    
    std::string getSpriteName(WorldCityInfo &info);
    void removeTouchEffect();
    void showAndHideUnBatchLabel();
    void showAndHideFieldMonster();
    void onShowAndHideSpinLb();
    void onShowAndHideCloud();
    std::map<int, unsigned int> getCurrentTile(CCPoint &pt, int rowNum, int colNum, int tileX);
    CCPoint getMarchPoint(MarchInfo &info);
    bool needClearAll;
    bool isCrossServer;
    void addAllianceArea(int cityIndex,bool isMyAlliance);
    bool isDownloadingMinimap;
    //
    void monsterDeathCB(CCObject* obj);
    void resetAAreaInfo(int tileIndex,int range);
    void initAAreaBlankInfo(int tileIndex,int range,bool _isMyalliance);
    void onRefreshAAreaBuilding(int tileIndex, AAreaState state,bool isSuperMine = false ,bool isTower = false,bool isWarehouse = false,bool isBanner = false);
    void initTowerRange(int tileIndex,bool isInRange);
    CC_SYNTHESIZE(bool, m_needOpenInfoView, NeedOpenInfoView);
    int getPicIdByCityInfo(WorldCityInfo &info);
    string getResNameByType(WorldResourceType type);
    bool isInWinsize(CCPoint viewPoint,int modelNum = 0);
    void onShowTowerRange(bool visable);
    void onSendHelpCmd();
    void onShake(CCObject* pObj);
    void testCastle(int level);
    void addZhengYing(WorldCityInfo &info);
    void initDragonBuildingConfig();
    void addDragonTowerRange(int index);
    int getDragonTowerAttack(int index,MarchInfo& info);
    
    void setCastleCityCustom(CCObject* pObj);
    void clearCityCustomSkin();
    void removeDragonFix(int index);
    void addBirthPoint(const CCPoint &startPt, int len, CCSpriteBatchNode *parent,bool left=true,bool isBlue=false,int index=1);
    void updateTransferPoint(int index);
    void showDragonFix(int index);
    void hideDragonFix(int index);
    void showDragonTower();
    void hideDragonTower();
    void onMakeDragonAni();
    void dragonFly();
    void closeCurrentTile();
    //出征后处理弹开部队功能面板
    void showViewAfterDeploy(CCObject* obj);
    //显示战斗报告
    void showViewCombatReport(int cityIndex,int reportType,MarchInfo marchInfo);
    // for lua  -- add by js
//    void addBatchItemForLua(int type,unsigned int index,CCObject* obj = NULL);
    unsigned int getBatchTagForLua(int type,unsigned int index);
    CCNode* getBatchNode(){
        return m_batchNode;
    }
    CCNode* getCityBatchNode(){
        return m_cityBatchNode;
    }
    CCNode* getUnBatchLabelNode(){
        return m_unBatchLabelNode;
    }
    CCNode* getAreaAniNode(){
        return m_aAreaAniNode;
    }
    CCLabelBatchNode* getLabelNode(){
        return m_labelNode;
    }
    // end
private:
    void setPraticesEffect( bool isEnalbe );
    void setAscription(WorldCityInfo &info,unsigned int index);
    void addUnderNode(WorldCityInfo &info,unsigned int index);
    void delUnderNode(unsigned int index);
    void addFlagPar(std::string type, const CCPoint &pt, int index);
    int getThroneIndex(int picId);
    void loadThroneResource(int index);
    
    MapType m_mapType;
    
    unordered_map<unsigned int, WorldCityInfo>& m_cityInfo;
    map<unsigned int, bool> mp;
    
    pthread_mutex_t m_addCityLock;
    
    CCSafeObject<CCNode> m_layers[LAYER_COUNT];
    CCSafeObject<CCArray> tilePops;
    int roadIndex;
    WorldActivityState m_currentState;
    int currentThroneIndex;
    CCRect m_viewPort;
    CCSize m_winSize;
    CCRect m_monsterRect;
    void onPurgeSpinNode(CCObject* obj);
    void onShowAAreaLb(bool visiable);
    void onSAHaAreaParcital(CCObject* pObj);
    void hideSameBoard(int cityIndex);
    void onTowerAttack(CCNode * marchNode ,MarchInfo& info);
    void onTowerAttackAnimation(int startCityIndex,CCPoint endViewPos,int marchTag);
    void addTowerRange(int tileIndex);
    void onShowMarkLine(CCObject* obj);
    bool canShowShakeGuide();
    //城堡换皮
    CCWorldMapSpriteBatchNode* getCityBatchNode(string cityskin);
    void initCityParAndSkBatchNode(string effcfile,const CCPoint &pos,int cityindex);
   
    vector<string> mcitySkin;
    CCSafeObject<CCWorldMapSpriteBatchNode> m_cityCustomBatch;
    CCNode* m_cityCustomParUpBatch;
    CCNode* m_cityCustomParDownBatch;
    std::vector<IFSkeletonBatchLayer*> m_cityCustomSkBatchVec;
    bool m_isIdle;
    
    struct FlagParInfo {
        std::string type;
        CCPoint pt;
        int index;
    };
    
    vector<FlagParInfo> m_flagParDatas;
    bool m_isIosAndroidPad;
};

#endif /* defined(__IF__WorldMapView__) */
