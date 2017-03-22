//
//  ImperialScene.h
//  IF
//
//  Created by fubin on 13-10-31.
//
//

#ifndef IF_ImperialScene_h
#define IF_ImperialScene_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"
#include "FunBuild.h"
#include "HFViewport.h"
#include "CCLabelBatch.h"
#include "ArcPopupBaseView.h"
#include "SpeBuild.h"
#include "FunBuildBtnsView.h"
#include "SpeBuildBtnsView.h"
#include "WallBuild.h"
#include "FlyCell.h"
#include "TalkNoticeCell.h"
#include "IFSkeletonBatchLayer.h"
#include "CCAniNode.h"
#include "ui/UIVideoPlayer.h"
#include "ui/CocosGUI.h"

#define BINDING_TIME "bindingTime"
#define ANNOUCE_TIME "Time"
#define FIRST_OPEN_AFTER_UPDATE "firstOpenAfterUpdate1"
#define MSG_TALK_OPEN "msg.talk.open"
#define BUILD_COUNT 59
#define MSG_SHOW_STAMINE_PARTICLE "msg.show.stamine.particle"
#define MSG_SHOW_EXP_PARTICLE "msg.show.exp.particle"
#define MSG_SHOW_POWER_PARTICLE "msg.show.power.particle"

//造兵建筑
#define TILE_BINGYING_1 4
#define TILE_BINGYING_2 5
#define TILE_BINGYING_3 6
#define TILE_BINGYING_4 7
//野外建筑
#define TILE_CHENG_WAI_IDX_BEGIN1 17
#define TILE_CHENG_WAI_IDX_END1 51

///水
#define BATCHNODE_ORDER_WATER  91
///建筑地格未解锁废墟
#define BATCHNODE_ORDER_BUILD_LOCK_TILE 201
///城内其他建筑

///地块和场景巡逻兵\对话等..活动建筑
#define BATCHNODE_ORDER_TILE 199
///城墙2
#define BATCHNODE_ORDER_CITY_GATE2 200
///城墙3
#define BATCHNODE_ORDER_CITY_GATE3 200
///脚手架下层
#define BATCHNODE_ORDER_FALSEWORK_DOWN 201
///主城
#define BATCHNODE_ORDER_MAINBUILD 202
///四种兵营
#define BATCHNODE_ORDER_BARRACKS 210
//出兵动画
#define BATCHNODE_ORDER_BARRACKS_TRAIN1 204
#define BATCHNODE_ORDER_BARRACKS_TRAIN2 206
#define BATCHNODE_ORDER_BARRACKS_TRAIN3 208
#define BATCHNODE_ORDER_BARRACKS_TRAIN4 208
///城墙
#define BATCHNODE_ORDER_BACKWALL 1498


///城门\箭塔
#define BATCHNODE_ORDER_CITY_GATE1 1499
///城外建筑
#define BATCHNODE_ORDER_BUILD_OUT 1502  //BATCHNODE_ORDER_FRONTWALL + 2
///脚手架上层
#define BATCHNODE_ORDER_FALSEWORK_UP 1503   //BATCHNODE_ORDER_BUILD_OUT + 1
///城内兵力和资源产量牌子
#define BATCHNODE_ORDER_TROOP_RESOURCE_BOARD 2000
///夜晚笼罩 黑层
#define BATCHNODE_ORDER_BUILD_NIGHT_BLACK 2001
///建筑未解锁-锁
#define BATCHNODE_ORDER_BUILD_LOCK 2002
///夜晚建筑灯效果
#define BATCHNODE_ORDER_BUILD_NIGHT 2003
///建筑名字层
#define BATCHNODE_ORDER_BUILD_NAME 2004
///建筑特效层
#define BATCHNODE_ORDER_BUILD_EFFECT 2005
///建筑状态（如升级等）、建筑气泡
#define BATCHNODE_ORDER_BUILD_STATE_TALK 2006
///云层
#define BATCHNODE_ORDER_CLOUD  2007
///巡逻兵
#define BATCHNODE_ORDER_NPC 2008
///城内巡逻兵气泡、建筑按钮菜单
#define BATCHNODE_ORDER_BUILD_BTNS 2009
///人物引导手层级
#define BATCHNODE_ORDER_TASK_HAND 2020
///飞龙
#define BATCHNODE_ORDER_DRAGON 2100
///新手引导战斗场景
#define BATCHNODE_ORDER_NEW_GUIDE  3000


//创建动画结构体
struct AnimationInfo {
    int index;
    int zOrder;
    int speed;
    float intervalTiem;
    float delayTime;
    bool isRepeat;
};


class ImperialScene:public CCLayer,public ITouchDelegate,public CCBMemberVariableAssigner
{
public:
    ImperialScene():lastTouchBuildId(-1),curTouchBuildId(-1),m_count(0),m_singleTouchState(false),m_mainPatPlayTime(0),m_waterType(0)
    ,m_curBuildId(0),m_curBuildPosx(0),m_curBuildPosy(0),m_tmpMoveX(0),m_tmpMoveY(0),m_canClick(true),m_buildingInitState(false)
    ,m_oldScale(1.0),m_oldPosX(0),m_oldPosY(0),m_isOnlyPower(0),m_isSave(false),m_tmpBuildPos(0),m_removeSpeBId(0),m_curGuideEnd(false),m_isLogin(false),m_beginTouchType(0),m_forceMove(false),m_exit(false),m_isDay(false),m_isRain(false),m_sysTime(0),m_talkTime(0), m_lotteryBuild(NULL),mActionManager(NULL),m_nightLights(NULL),m_rescustombatchNode(NULL),mMaincitySkin(""),princessNodeOnce(false),m_questEffect(0),m_tipsTime(0),m_tipsType(0),m_princessRwdType(-1),m_sSoldierClickTime(0),m_princessAniEnd(true),m_fountainFinish(true),m_princessWalkStart(false),m_guideFreeTime(300){};
    
    virtual ~ImperialScene(){};
    CREATE_FUNC(ImperialScene);
    bool init();
    bool initUI();
    //CC_SYNTHESIZE_SAFEOBJECT(CCLayer , buildingLayer, BuildingLayer);
    //CC_SYNTHESIZE_SAFEOBJECT(CCLayer , fixLayer, FixLayer);
    //CC_SYNTHESIZE_SAFEOBJECT(CCSpriteBatchNode , mbatchNode, BatchNode);
    
    virtual void onEnter();
    virtual void onExit();
    void onUpdateInfo();
    void onCreateBuild(int itemId);
    void onOpenBuild(int itemId);
    void onResetBuild(int itemId);
    void onEnterFrame(float dt);
    void onShowFeedbackAnim(float dt);
    void onShowFeedbackAgainAnim(float dt);
    void onShowFeedbackAgainLaterAnim(float dt);
    void onShowHTPAnim(float dt);
    void onShowHTPAgainAnim(float dt);
    void onShowHTPAgainLaterAnim(float dt);
    void onOpenBuildEnd(int itemId);
    void onQijiBottomShow(int lv);
    
    void onMoveToBuild(int itemId, bool st=false);
    void onMoveToBuildAndPlay(int itemId, bool st=false, bool train = false);
    void onMoveToSpeBuildAndPlay(int itemId, bool st=false);
    void onMoveToBuildAndOpen(int itemId, int type=TYPE_POS_DEF, float dt=0.25, bool bound=false,bool up = false);
    void onMoveToPos(float x, float y, int type=TYPE_POS_DEF, float dt=0.25, float scale=1.3, bool bound=false);
    void onMoveToPosDir(float sx, float sy, float ex, float ey, float dt=0.25, bool bound=false);
    CCPoint onGetSceenPt(float x, float y);
    float onGetTouchLayerScale();
    void onSaveCurPos(CCObject* obj);
    void setPxPy(float x,float y);
    void onGoBackOldPos(CCObject* obj);
    void onFlyOutPut(int itemId, int output, int forceResType=-1);
    void onFlyCargoOutPut(CCObject* obj);
    void onGetMsgRefreshBuild(Ref* ref);
    
    virtual void onTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void onTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void onTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    virtual void onTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
    void stopShadow(int buildId, CCTouch *pTouch, CCEvent *pEvent);
    
    void showTmpBuild(int pos, int itemId);
    void hideTmpBuild(int pos);
    
    virtual void onSingleTouchBegin(CCTouch* pTouch);
    virtual void onSingleTouchEnd(CCTouch* pTouch);
    
    CCNode* getBuildById(int itemId);
    CCNode* getBuildNameById(int itemId, string _key);
    CCNode* getSpeBuildNameById(int itemId, string _key);
    void addToMovingLayer(CCNode* node);
    float getTouchLayerScale();
    void onRemoveSpeBuild();
    void setUnMoveScence(bool st);
    void showBuildBtns(int itemId);
    void clickWorkshop(int itemId);//点材料工坊
    void onResetLastBuildId();
    
    void onShowBtnsView(int x, int y, int buildId);
    void onShowSpeBtnsView(int x, int y, int buildId);
    void onFlyTroopParticle(int x, int y, int onlyPower=0);
    void onFlyTroopEnd(CCObject *obj);
    void onFlyTroopEndToUI(CCObject *obj);
    void onPlayMoveTroops(int buildId);
    
    int findCanBuildTile(int pos);
    void onPlayBattle();
    void flyBall(int buildId, int type, bool toScene);
    
    void makeCanPlayArrowPoint();//计算哪些建筑播放 手指点击
    void reSetTalkTime();
    void hideFlyArrow(float _time);
    
    void updateShipState(string state);
    CCBAnimationManager* getAnimationManager();
    void setAnimationManager(CCBAnimationManager *pAnimationManager);
    CCSpriteBatchNode* createCustomBatchLayer(string image);
    void removeCustomBatchLayer();
    
    
    bool isGoIn;
    void cargoShipGoIn();//货船入场
    void onCargoShiAnimationCallback(Node* animationNode);
    void cargoShipGoOut();//货船出场
    
    void hidePrincess(int bid);
    Point getMailPosOnScreen();
    Point getNewRwdPosOnScreen();
    void setQuestEffect(int type);
    CCSafeObject<CCArray> m_princessRwdArr;
    Vec2 getWorldPosition(int pos);
    Node* getTouchLayer();
    void setPointArrowAni(int buildId);
    void videoPlayContinueCallback();
    void videoPlayOverCallback();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    void videoEventCallback(Ref* sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType);
#endif
private:
    int lastTouchBuildId;
    int curTouchBuildId;
    void onResourcesUpdate(cocos2d::CCObject* params);
    void onBattleUpdate(CCObject* params);
    void unLockTile(CCObject* params);
    void refreashTile();
    void initBgTree();
    void setMakeEffectSpr(CCSprite* spr,string direction,int animationType);
    void onChangeAnimationCallback(Node* animationNode);
    void onClearAnimationCallback(Node* animationNode);
    void addAnimation(AnimationInfo animationInfo);
    void createAnimationCallback(Node* node);
    void createAnimation(int index,int zOrder,int speed,float intervalTiem=0,float delayTime=0,bool isRepeat=true);
    void setSequenceAnimation(CCSprite * spr);
    void initAnimation();
    void onQijiBottomDelete(CCObject* obj);
    void onOpenNewBuild(CCObject* obj);
    void onUpgradeNewBuild(int buildId);
    void flyBallEnd(CCNode* pSender);
    void makeFlyMc(int type, CCNode* flNode);
    void flyOutPut(int buildId, CCNode* flNode, int idx);
    void flyOutPutEnd(CCNode* pSender);
    void showAttention();
    void showAttention1();
    
    void initSpeBuildInWallTexture();
    void initMc2();
    void initBigTile();
    void startGuide(float _time);
    
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    bool isGeneralOnFormationBuilding(int buildingId);
    void canSingleTouch(float _time);
    void addParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    void addCloudParticleToBatch(cocos2d::CCParticleSystemQuad *particle, bool isTorch = false);
    void makeWater(int type);
    
    void onRemoveTmpBuild(CCObject* obj);
    void onEnableMoveScence(float _time);
//    void trackDBM();
//    void trackTencent();
//    void trackTencentForZW();

    std::string UrlEncode(const std::string& str);
    bool simple_xor(const std::string& info,const std::string& key,unsigned char* result);
    void onTrackResponse(CCHttpClient* client, CCHttpResponse* response);

//    void MiracleCallBack(CCObject* params);
    void buildingCallBack(CCObject* params);
    void wallCallBack(CCObject* params);
    void showHarvestEffect(CCObject* obj);
    void scheduleHarvestEffect(float _time);
    void guideEnd(CCObject* obj);
    void retGuideEnd(float _time);
    void clearGuideState(float _time);
    void playFlyTroopParticle(float _time);
    void playPowerAni(float _time);
    void downloadXML(float _time);
    
    void onFinishMoveBuild(CCObject* obj);
    
    void onPowerADD(CCObject* obj);
    
    void onStopAllActions(CCObject* params);
    
    map<int, CCSafeNode<FunBuild>  > m_buildItems;
    map<int, CCSafeNode<SpeBuild>> m_bigTiles;
    map<int, CCSafeNode<CCNode>> m_bigTileNodes;
    void moveMapToPosition(CCObject* obj);
    
    void refreshSoldiers(CCObject* obj);
    void addSoldierToMap(int stype, int num, int ptx, int pty, int sqerType);//sqerType为方阵的索引
    
    void playFountain();
    void removeFountain(CCObject *obj);
    
    void unLockTouch(float _time);
    
    void playWatchGlow();
    
    void clearDrop(float _time);
    
    void checkTileGlow(CCObject* obj);
    
    void onRemoveGuideParticle(CCObject* obj);

    void playArrowClickEnd();//播放连续点击 位置切换
    void playArrowClick();//播放连续点击
    void stopArrowClick(float _time);//停止播放
    
    void getFriendsData();
    void getScienceData();
    void recordGaid();
    void createActiveBd(CCObject* obj);
    
    void playWaveParticle(float _time);
    
    void showSunshine();//雨过天晴
    void setSunAngle(float _time);//设定阳光角度
    void checkDayTime(bool isInit=false);//判断是否是白天
    void showRain();//下雨
    void showFirework(CCObject* obj);//烟花
    void startShowFirework(float t);//烟花序列特效
    
    void onShowPrincess(int type);
    void onPrincessAniFuc(CCObject* obj);
    void onStartShowPrincess(int forceType = -1);
    void onShowPrincessWalk(float dt=0);
    void onStopPrincessWalk();
    void onShowPrincessWalkTalk();
    void onPrincessWalkTalkBack();
    void onPrincessWalkEnd(float dt);
    void onCleanPrincessWalk();
    void showPrincessByMsg(CCObject* obj);
    void princessShowOnByMsg(float dt);
    void princessShowOnByEndGuide(float dt);
    void newrwdGuideEndConfirm(CCObject* obj);
    void onCheckNewPlayerScene(CCObject* obj);
    
    void onShowTalkView(CCObject* obj);
    void onShowTalkViewOnce();
    void onShowGuideWhenFree(CCObject* obj);
    void onDragonFly();
    void onMakeDragon();
    void addDefenseFailEffect();//城防着火时主城死兵的效果
    //主城夜晚灯光
    void addNightLight();
    void removeAllNightLight();
    void changeDayOrNight(bool isNight, bool isAni);
    void addTorchToNode(CCNode *node, int index);
    void firstEnterMainCity();
    void showExtraBtns(bool visible);
    void initPrincess();
    void showFlyBall(CCObject* obj);
    void checkTipsTime();
    void setTipsType(CCObject* obj);
    void reSetTipsType(CCObject* obj);
    void princessRwdShow(CCObject* obj);
    void princessRwdShowLater(float t);
    void princessRwdNullShow(CCObject* obj);
    void onShowPatrol(CCObject* obj);
    void onShowPatrolStart(float t);
    void onShowPatrolEnd(float t);
    void onShowRebuild(CCObject* obj);
    void onShowFarmer(float t);
    void onCleanFarmer();
    void onCreateFarmer();
    void onShowFarmerStart(float t);
    void onShowFarmerEnd(float t);
    void onShowFarmerEffect(float t);
    void princessRwdCallback();
    void sqerSoldierAniComplete(CCObject* obj);
    void showMainCityBtnAni();
    void onShowMainCityBuff(CCObject* obj);
    void onShowMainCitySkin(CCObject* obj);
    void onShowMainCityBtn(float t);
    void addMainCityBtnParticle();
    void mainCityBtnAniComplete();
    void removeSqerTouchPar(float t);
    void shakeViewport(CCObject* obj);
    void princessShotAni(int type);
    void onShowPrincessHappy(float t);
    void onShowPrincessSad(float t);
    void onShowPrincessShotEnd(float t);
    void removeFountainRainbow();
    void addFountainRainbowPar();
    void removeFountainRainbowPar();
    void princessShowWithTransform(float t);
    void princessWalkEndStartTalk(float t);
    void allianceRecommandShow(CCObject* obj);
    void addEnergyEff(CCObject* obj);
    void initPrincessTask();
    CCSafeObject<CCNode> m_nodeBuildings[BUILD_COUNT];
    
    CCSafeObject<CCNode> m_flagNode;
    CCSafeObject<SpeBuild> m_flagBuild;
    CCSafeObject<CCNode> m_hdNode;
    CCSafeObject<SpeBuild> m_hdBuild;
    CCSafeObject<CCNode> m_goldMineNode;
    CCSafeObject<SpeBuild> m_goldMineBuild;
    CCSafeObject<CCNode> m_goldBoxNode;
    CCSafeObject<SpeBuild> m_goldBoxBuild;
    CCSafeObject<CCNode> m_chrTreeNode;
    CCSafeObject<SpeBuild> m_chrTreeBuild;
    CCSafeObject<CCNode> m_shipNode;
    CCSafeObject<SpeBuild> m_shipBuild;
    CCSafeObject<Node> m_ziyuanmenNode;
    CCSafeObject<SpeBuild> m_ziyuanmenBuild;
    //商人
    CCSafeObject<CCNode> m_merchanteNode;
    CCSafeObject<SpeBuild> m_merchanteBuild;
    //转盘
    CCSafeObject<CCNode> m_lotteryNode;
    CCSafeObject<SpeBuild> m_lotteryBuild;
    //货物
    CCSafeObject<CCNode> m_cargoNode;
    CCSafeObject<SpeBuild> m_cargoBuild;
    //兄弟会
    CCSafeObject<CCNode> m_xiongdiNode;
    CCSafeObject<SpeBuild> m_xiongdiBuild;
    //奇迹
    CCSafeObject<CCNode> m_qijiNode;
    CCSafeObject<SpeBuild> m_qijiBuild;
    //新手物资
    CCSafeObject<CCNode> m_newRDNode;
    CCSafeObject<SpeBuild> m_newRDBuild;
    //巨龙战入口
    CCSafeObject<CCNode> m_dragonNode;
    CCSafeObject<SpeBuild> m_dragonBuild;
    //火把
    CCSafeObject<CCNode> m_torchNode;
    CCSafeObject<CCNode> m_torchBatchNode;
    CCSafeObject<CCNode> m_torchNode1;
    CCSafeObject<CCNode> m_torchNode2;
    CCSafeObject<CCNode> m_torchNode3;
    CCSafeObject<CCNode> m_torchNode4;
    CCSafeObject<CCNode> m_torchNode5;
    CCSafeObject<CCNode> m_torchNode6;
    CCSafeObject<CCNode> m_torchNode7;
    CCSafeObject<CCNode> m_torchNode8;
    
    CCSafeObject<CCNode> m_willOpenNode;
    CCSafeObject<SpeBuild> m_willOpenBuild;
    
    
    
    CCSafeObject<SpeBuild> m_mailBuild;
    
    CCSafeObject<CCNode> m_bg1;
    CCSafeObject<CCNode> m_bg2;
    CCSafeObject<CCNode> m_bg3;
    CCSafeObject<CCNode> m_bg4;
    CCSafeObject<CCNode> m_bg5;
    CCSafeObject<CCNode> m_bg6;
    CCSafeObject<CCNode> m_mcNode1;
    CCSafeObject<CCNode> m_mcNode2;
    CCSafeObject<CCNode> m_mcNode3;
    CCSafeObject<CCNode> m_mcNode4;
    CCSafeObject<CCNode> m_mcNode5;
    CCSafeObject<CCNode> m_desNode1;
    CCSafeObject<CCNode> m_desNode2;
    CCSafeObject<CCNode> m_desNode3;
    CCSafeObject<HFScrollView> m_touchLayer;
    CCSafeObject<CCLayer> m_waterLayer;
    CCSafeObject<CCLayer> m_cloudLayer;
    CCSafeObject<CCLayer> m_bgParticleLayer;
    CCSafeObject<CCLayer> m_popLayer;
    CCSafeObject<CCLayer> m_signLayer;
    CCSafeObject<CCLayer> m_spineLayer;
    CCSafeObject<CCLayer> m_nightLayer;
    CCSafeObject<CCLayer> m_funLayer;
    CCSafeObject<CCLayer> m_battleLayer;
    CCSafeObject<CCLayer> m_soldierLayer;
    
//    CCSafeObject<CCSprite> m_sprBG1;
    CCSafeObject<CCLabelBatchNode> m_nameLayer;
    CCSafeObject<CCSpriteBatchNode> m_lvLayer;
    CCSafeObject<HFViewport> m_viewPort;
    CCSafeObject<CCParticleSystemQuad>  m_praticle;
    
    CCSafeObject<CCNode> m_flyNodes[10];
    
    CCSafeObject<CCNode> m_collectNode;
    CCSafeObject<CCSpriteBatchNode> m_collectBatch;
    
    CCSafeObject<CCNode> m_sunNode;
    CCSafeObject<CCSprite> m_sunshine[4];
    CCSafeObject<CCSprite> m_flyArrow;
    CCSafeObject<CCSprite> m_dragon;
    
    CCSafeObject<CCSpriteBatchNode> m_wallBackbatchNode;
    CCSafeObject<CCSpriteBatchNode> m_wallFrontbatchNode;
    CCSafeObject<CCSpriteBatchNode> m_tilebatchNode;
    CCSafeObject<CCSpriteBatchNode> m_forestbatchNode;
    CCSafeObject<CCSpriteBatchNode> m_arrbatchNode;
    CCSafeObject<CCSpriteBatchNode> m_resbatchNode;
    CCSafeObject<CCSpriteBatchNode> m_bubbleNode;
    CCSafeObject<CCSpriteBatchNode> m_rescustombatchNode;
    CCSafeObject<SpriteBatchNode> m_resTmpCustomBatchNode;
    CCSafeObject<CCSpriteBatchNode> m_resBlentBatchNode;
    CCSafeObject<CCSpriteBatchNode> m_soldierBatchNode;
    CCSafeObject<CCSpriteBatchNode> m_chrTreeBatchNode;
    CCSafeObject<CCSpriteBatchNode> m_chrTreeBlentBatchNode;
    CCSafeObject<CCSpriteBatchNode> m_cludeBatchNode;
    CCSafeObject<SpriteBatchNode> m_wallTextureBatchNode;
    
    /*脚手架分层专用*/
    CCSafeObject<CCSpriteBatchNode> m_scaffoldsBatchNode;
    //建筑效果
    CCSafeObject<CCSpriteBatchNode> m_effectBlentBatchNode;    //混合模式
    
    
    CCSafeObject<CCNode> m_soldierFlagNode;
    map<int,CCSpriteBatchNode*> m_buildBatchMap;
 	map<int,CCLayer*> m_buildSpineMap;
    CCSafeObject<CCNode> m_wallNodes[3];
    CCSafeObject<WallBuild> m_wallBuilds[3];
    CCSafeObject<CCSpriteBatchNode> m_wallBatchs[3];
    //装饰层
    CCSafeObject<CCNode> m_decorateNodes[6];
    CCSafeObject<CCSpriteBatchNode> m_decorateBatchs[6];
    //动画层
    map<int, CCSafeNode<CCNode>> m_animationNodes;
    map<int, vector<CCNode*>> m_animationPosNodes;
    map<int, CCSafeObject<CCPointArray>> m_animationPos;
    map<int, CCSafeObject<CCPointArray>> m_animationAllPos;
    map<int, CCSafeNode<CCSpriteBatchNode>> m_animationBatchs;
    map<int, AnimationInfo> m_animationInfos;
    CCSafeObject<CCNode> m_animation_Stand;
    CCSafeObject<CCNode> m_animation_sequence;
    

    CCSafeObject<CCArray> m_personArray;
    CCSafeObject<CCArray> m_prinessPersonArray;
    CCSafeObject<CCArray> m_river1Array;
    CCSafeObject<CCArray> m_river2Array;
    CCSafeObject<CCArray> m_river3Array;
    CCSafeObject<CCArray> m_river4Array;
    CCSafeObject<CCArray> m_river5Array;
    CCSafeObject<CCArray> m_river6Array;
    CCSafeObject<CCArray> m_river7Array;
    CCSafeObject<CCArray> m_wave1Array;
    CCSafeObject<CCArray> m_wave2Array;
    CCSafeObject<CCArray> m_wave3Array;
    CCSafeObject<CCPointArray> m_ptArray;
    CCSafeObject<CCPointArray> m_pt1Array;
    CCSafeObject<CCPointArray> m_ptArrowClick;
    CCSafeObject<CCPointArray> m_pt2Array;
    CCSafeObject<CCArray> m_soldierArray;
    
    vector<CCParticleBatchNode*> m_parVec;
    vector<CCParticleBatchNode*> m_cloudVec;
    vector<CCParticleSystemQuad*> m_waveVec;
    
    CCSafeObject<CCNode> m_bigTileNode1;
    CCSafeObject<CCNode> m_bigTileNode2;
    CCSafeObject<CCNode> m_bigTileNode3;
    CCSafeObject<CCNode> m_bigTileNode4;
    CCSafeObject<CCNode> m_bigTileNode5;
    CCSafeObject<CCNode> m_FountainTouchNode;
    CCSafeObject<CCNode> m_fountainNode;
    CCSafeObject<CCNode> m_forestNode;
    CCSafeObject<CCNode> m_couldNode;
    CCSafeObject<CCNode> m_crossNode;
    CCSafeObject<CCNode> m_crossClickNode;

    //龙穴和喷泉
    CCSafeObject<Node> m_penquanRainbow;
    CCSafeObject<Node> m_dragonCaveGlow;
    void InitDragonCaveGlow();
    
    CCSafeObject<FunBuildBtnsView> m_buildBtnsView;
    CCSafeObject<SpeBuildBtnsView> m_speBuildBtnsView;
    CCLabelIF* m_feedlabel;
    CCSafeObject<CCSprite> m_feedBackHead;
    
    CCSafeObject<TalkNoticeCell> m_talkACTCell;
    CCSafeObject<TalkNoticeCell> m_talkACTCell2;
    
    CCSafeObject<CCNode> m_dropNode;
    CCSafeObject<CCNode> m_tileGlowNode;
    
    CCSafeObject<CCSprite> m_HTPHead;
    CCSafeObject<CCSprite> m_HTPHead1;
    CCLabelIF* m_HTPlabel;
    CCSafeObject<CCDictionary> m_ripple;
    CCSafeObject<CCNode> m_failEffectNode;
    CCSafeObject<CCNode> m_icon_op_mail;
    CCSafeObject<CCArray> m_torchPar;
    CCSafeObject<CCNode> m_nightLights;
    CCSafeObject<CCNode> m_particalNode;
    
    CCSafeObject<CCNode> m_princessNode;
    CCSafeObject<Node> m_princessRootNode;
    void addGuideParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    CCSafeObject<CCNode> m_guideParticleNode;
    vector<CCSafeObject<CCParticleBatchNode>> m_guideParticleVec;
    
    CCSafeObject<CCNode> m_princess1Node;
    CCSafeObject<CCNode> m_princess2Node;
    CCSafeObject<CCNode> m_princess3Node;
    CCSafeObject<CCNode> m_princessWalkNode;
    IFSkeletonAnimation* m_princessAni;
    IFSkeletonAnimation* m_princessWalkAni;
    IFSkeletonAnimation* m_princessShotAni;
    CCSafeObject<Node> m_farmerNode;
    CCSafeObject<Node> m_dragonFlyNode;
    CCSafeObject<Array> m_sSqerTouchArr;
    CCSafeObject<Node> m_sSqerTouchNode;
    CCSafeObject<Node> m_mainCityBtnAni;
    int m_count;
    bool m_singleTouchState;
    bool m_buildingInitState;
    int m_mainPatPlayTime;
    int m_waterType;
    
    int m_curBuildId;
    int m_curBuildPosx;
    int m_curBuildPosy;
    int m_tmpMoveX;
    int m_tmpMoveY;
    bool m_canClick;
    
    float m_oldScale;
    float m_oldPosX;
    float m_oldPosY;
    float m_flyX;
    float m_flyY;
    int flyCount;
    int m_isOnlyPower;
    bool m_isSave;
    int m_tmpBuildPos;
    int m_removeSpeBId;
    bool m_curGuideEnd;
    bool m_isLogin;
    int m_beginTouchType;
    
    int _arrowPtIdx;
    bool m_forceMove;
    bool m_exit;
    
    bool m_isDay;
    bool m_isRain;
    int m_sysTime;
    int m_talkTime;
    int m_noticeCount;
    int m_helpCount;
    int startDarkTime;
    int completeDarkTime;
    int startLightTime;
    int completeLightTime;
    int m_useItemId;
    int m_guideFreeTime;
    CCBAnimationManager* mActionManager;
    //自定义的图
    string mMaincitySkin;
    
    int m_princessBid;
    bool princessNodeOnce;
    int m_questEffect;//默认为0，传1时为推荐任务
    int m_tipsTime;//建筑和士兵头顶出现对话的时间间隔
    int m_tipsType;//默认为0，远古战场为1，移动士兵为2，旅行商人为3
    bool m_crossServeActFlag;//默认为false，远古战场开启时为true
    
    int m_princessRwdType;//
    int m_sSoldierClickTime;
    bool m_princessAniEnd;//小公主动画是否结束
    bool m_fountainFinish;//喷泉动画结束
    bool m_princessWalkStart;//小公主入城动画是否开始
};
#endif /* defined(__IF__ImperialScene__) */
