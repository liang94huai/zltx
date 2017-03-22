//
//  ImperialScene.cpp
//  IF
//
//  Created by fubin on 13-10-31.
//
//

#include "ImperialScene.h"
#include "SceneController.h"
#include "ParticleController.h"
#include "SoundController.h"
#include "FunBuild.h"
#include "FunBuildController.h"
#include "FunBuildInfo.h"
#include "PopupViewController.h"
#include "YesNoDialog.h"
#include "GuideController.h"
#include "SetAccountView.h"
#include "AppLibHelper.h"
#include "UserBindCommand.h"
#include "UIComponent.h"
#include "QueueController.h"
#include "TileOpenView.h"
#include "BattlefieldScene.h"
#include "SuggestionView.h"
#include "MainCityArmy.h"
#include "WorldController.h"
#include "ArmyController.h"
#include "fireandcomman.h"
#include "QuestController.h"
#include "HowToPlayView.h"
#include "NetController.h"
#include "../../view/guiExt/md5.h"
#include "ToolController.h"
#include "RechargeACTVCell.h"
#include "ScienceController.h"
#include "ActivityController.h"
#include "cocos2d.h"
#include "VipUtil.h"
#include "EagleCCB.h"
#include "Utf8Utils.h"
#include "UserUpgradeView.h"
#include "ScienceCommand.h"
#include "NpcTalkView.h"
#include "CityCrashView.h"
#include "PayController.h"
#include "EquipFinishView.h"
#include "PortActView.h"
#include "FeedRewardCommand.h"
#include "ToolCreateView.h"
#include "../../Ext/CCDevice.h"
#include "ActivityEventObj.h"
#include "CCFlagWaveSprite.h"
#include "NewPlayerQianDaoView.h"
//#include "BranchController.h"
#include "AllianceManager.h"
#include "DynamicResourceController.h"
#include "IFSkeletonDataManager.h"
#include "FriendsController.h"
#include "ChatServiceCocos2dx.h"
#include "SkinListView.hpp"
#include "LuaController.h"
#include "TransitionCloud.hpp"
#include "GuideRecordCommand.h"
#include "PrincessRewardCommand.hpp"
#include "NewPlayerBattleScene.hpp"
#include "DragonFly.hpp"
#include "GCMRewardController.h"
#include "SqerSoldierRewardCommand.hpp"
#include "TransitionLayer.hpp"
#include "PrincessQuestView.hpp"
#include "PrincessQuestController.hpp"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

#define HD_SCALE 1.2f

using namespace cocos2d;

static const ccBlendFunc kCCBlendFuncTree = {GL_ONE, GL_ONE};
static int guideTypeNew = 0;
static int enterMainCityTime = 0;

bool ImperialScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    bool isPlay = false;
    if(!FunBuildController::getInstance()->isFirst)
    {
        string gFake = CCUserDefault::sharedUserDefault()->getStringForKey("Guide_Fake","");
        if (GlobalData::shared()->playerInfo.level==1 && GlobalData::shared()->playerInfo.exp==0 && (gFake=="" || gFake=="start_1")) {
            isPlay = true;
            onPlayBattle();
        }
    }
    unsigned long ulc = clock();
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("ImperialScene", "init");
    
    m_personArray = CCArray::create();
    
    m_river1Array = CCArray::create();
    
    m_wave1Array = CCArray::create();
    m_wave2Array = CCArray::create();
    m_wave3Array = CCArray::create();
    
    m_soldierArray = CCArray::create();
    m_princessRwdArr = Array::create();
    m_sSqerTouchArr = Array::create();
    m_prinessPersonArray = CCArray::create();
    m_princess1Node = NULL;
    m_princess2Node = NULL;
    m_princess3Node = NULL;
    m_princessWalkNode = NULL;
    m_princessAni = NULL;
    m_princessWalkAni = NULL;
    m_princessShotAni = NULL;
    m_farmerNode = NULL;
    //添加粒子纹理
    ParticleController::initParticle();
    m_singleTouchState = false;
    m_exit = false;
    m_helpCount = 0;
    startDarkTime = GlobalData::shared()->startDarkTime;
    completeDarkTime = GlobalData::shared()->completeDarkTime;
    startLightTime = GlobalData::shared()->startLightTime;
    completeLightTime = GlobalData::shared()->completeLightTime;

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    
    int sumWidth = 1020+1020+1020+510;
    int extH = 0;
    int sumHight = 1020+1020+200+extH;
//    auto waterSpr = CCLoadSprite::createScale9Sprite("water.png");
//    waterSpr->setAnchorPoint(ccp(0, 0));
//    waterSpr->setPosition(ccp(0,0));
//    waterSpr->setContentSize(winSize);
//    this->addChild(waterSpr, 0);
//    
//    auto particle = ParticleController::createParticle("RippleTest");
//    particle->setPosition(ccp(winSize.width*0.7, winSize.height*0.7));
//    this->addChild(particle, 1);
    
    m_touchLayer = HFScrollView::create();
    m_touchLayer->setVisible(!isPlay);
    m_bgParticleLayer = CCLayer::create();//粒子节点
    m_nightLayer = CCLayer::create();//夜晚笼罩 黑层
    m_nameLayer =CCLabelBatchNode::create("Arial_Bold.fnt");
    m_popLayer = CCLayer::create();
    m_signLayer = CCLayer::create();
    m_funLayer = CCLayer::create();
    m_buildBtnsView = FunBuildBtnsView::create();
    m_buildBtnsView->setVisible(false);
    m_funLayer->addChild(m_buildBtnsView);
    m_speBuildBtnsView = SpeBuildBtnsView::create();
    m_speBuildBtnsView->setVisible(false);
    m_funLayer->addChild(m_speBuildBtnsView);
    m_battleLayer = CCLayer::create();
    m_soldierLayer = CCLayer::create();
    m_torchPar = CCArray::create();
    m_spineLayer = CCLayer::create();
    m_nightLights = CCNode::create();//夜晚灯
    m_sSqerTouchNode = Node::create();
    
    //加名字
    m_touchLayer->setName("m_touchLayer");
    m_bgParticleLayer->setName("m_bgParticleLayer");
    m_nightLayer->setName("m_nightLayer");
    m_nameLayer->setName("m_nameLayer");
    m_popLayer->setName("m_popLayer");
    m_signLayer->setName("m_signLayer");
    m_funLayer->setName("m_funLayer");
    m_buildBtnsView->setName("m_buildBtnsView");
    m_speBuildBtnsView->setName("m_speBuildBtnsView");
    m_battleLayer->setName("m_battleLayer");
    m_soldierLayer->setName("m_soldierLayer");
    m_spineLayer->setName("m_spineLayer");
    m_nightLights->setName("m_nightLights");
    m_sSqerTouchNode->setName("m_sSqerTouchNode");
    
    //公主头像
    initPrincess();
    //公主头像 end

    m_bgParticleLayer->setContentSize(CCSize(sumWidth, sumHight));
    m_touchLayer->setContentSize(CCSize(sumWidth, sumHight));
    m_nightLayer->setContentSize(CCSize(sumWidth, sumHight));
    m_nameLayer->setContentSize(CCSize(sumWidth, sumHight));
    m_popLayer->setContentSize(CCSize(sumWidth, sumHight));
    m_signLayer->setContentSize(CCSize(sumWidth, sumHight));
    m_funLayer->setContentSize(CCSize(sumWidth, sumHight));
    m_spineLayer->setContentSize(CCSize(sumWidth, sumHight));
    this->addChild(m_touchLayer);
    
    //太阳光节点
    m_sunNode = CCNode::create();
    m_sunNode->setName("m_sunNode");
    m_sunNode->setPosition(CCDirector::sharedDirector()->getWinSize().width + 50, CCDirector::sharedDirector()->getWinSize().height - 50);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_sunNode->setScale(2.4);
        m_sunNode->setPosition(50 * 2.4 + CCDirector::sharedDirector()->getWinSize().width, CCDirector::sharedDirector()->getWinSize().height - 50 * 2.4);
    }
    this->addChild(m_sunNode);
    
    //水
    m_waterLayer = CCLayer::create();
    m_waterLayer->setName("m_waterLayer");
    
    
    if (GlobalData::shared()->contryResType==0) {
        auto node = CCBLoadFile("ImperialCities",m_touchLayer,this);
        node->setZOrder(90);
    }
    else {
        string ccbName = "ImperialCities";
        ccbName = ccbName+"_"+CC_ITOA(GlobalData::shared()->contryResType);
        auto node = CCBLoadFile(ccbName.c_str(),m_touchLayer,this);
        node->setZOrder(90);
    }
    
    for (int i=0; i<BUILD_COUNT; i++) {
        int od = m_nodeBuildings[i]->getOrderOfArrival();
        m_nodeBuildings[i]->setZOrder(od);
    }
    
    m_mcNode1->setZOrder(m_mcNode1->getOrderOfArrival());
    m_mcNode2->setZOrder(m_mcNode2->getOrderOfArrival());
    m_mcNode3->setZOrder(m_mcNode3->getOrderOfArrival());
    m_mcNode4->setZOrder(m_mcNode4->getOrderOfArrival());
    m_mcNode5->setZOrder(m_mcNode5->getOrderOfArrival());
    m_desNode1->setZOrder(m_desNode1->getOrderOfArrival());
    m_desNode2->setZOrder(m_desNode2->getOrderOfArrival());
    m_desNode3->setZOrder(m_desNode3->getOrderOfArrival());
    m_flagNode->setZOrder(m_flagNode->getOrderOfArrival());
    m_hdNode->setZOrder(m_hdNode->getOrderOfArrival());
    m_goldMineNode->setZOrder(m_goldMineNode->getOrderOfArrival());
    m_goldBoxNode->setZOrder(m_goldBoxNode->getOrderOfArrival());
    m_chrTreeNode->setZOrder(m_chrTreeNode->getOrderOfArrival());
    m_willOpenNode->setZOrder(m_willOpenNode->getOrderOfArrival());
    m_shipNode->setZOrder(m_shipNode->getOrderOfArrival());
    m_merchanteNode->setZOrder(m_merchanteNode->getOrderOfArrival());
    m_cargoNode->setZOrder(m_cargoNode->getOrderOfArrival());
    m_newRDNode->setZOrder(m_newRDNode->getOrderOfArrival());
    m_lotteryNode->setZOrder(m_lotteryNode->getOrderOfArrival());
    m_xiongdiNode->setZOrder(m_xiongdiNode->getOrderOfArrival());
    m_qijiNode->setZOrder(m_qijiNode->getOrderOfArrival());
    m_dragonNode->setZOrder(m_dragonNode->getOrderOfArrival());
    m_ziyuanmenNode->setZOrder(m_ziyuanmenNode->getOrderOfArrival());
    
    map<int, CCSafeNode<CCNode>>::iterator it=m_bigTileNodes.begin();
    for (; it!=m_bigTileNodes.end(); it++) {
        (it->second)->setZOrder((it->second)->getOrderOfArrival());
    }
    
    //锁的节点
    auto sp = CCLoadSprite::loadResource("build_lock.png");
    m_arrbatchNode = CCSpriteBatchNode::createWithTexture(sp->getTexture());
    
    //火把节点
    m_torchBatchNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("Brasier.png")->getTexture());
    m_torchBatchNode->setName("m_torchBatchNode");
    m_torchNode->addChild(m_torchBatchNode);
    //地格节点
    auto tile = CCLoadSprite::loadResource("water.png");
    m_tilebatchNode = CCSpriteBatchNode::createWithTexture(tile->getTexture());
    m_tilebatchNode->setName("m_tilebatchNode");
    
    //前景树
    auto forest = CCLoadSprite::loadResource("water.png");
    m_forestbatchNode = CCSpriteBatchNode::createWithTexture(forest->getTexture());
    m_forestbatchNode->setName("m_tilebatchNode");
    
    m_touchLayer->setViewTarget(m_forestbatchNode);
    
    //龙
    m_dragonFlyNode = Node::create();//CCB里放大了12倍
//    onMakeDragon();
    m_dragonFlyNode->setScale(1.3);
    m_dragonFlyNode->setAnchorPoint(ccp(0.5, 0.5));
    m_dragonFlyNode->setPosition(ccp(-600, 1020));
    m_dragonFlyNode->setName("m_dragonFlyNode");
    
    
//    m_dragon = CCSprite::create();
//    m_dragon->setScale(16);
//    m_dragon->setFlipX(true);
//    m_dragon->setAnchorPoint(ccp(0.5, 0.5));
//    m_dragon->setOpacity(100);
//    m_dragon->setPosition(ccp(-600, 1020));
//    m_touchLayer->addChild(m_dragon, 2080);
    
//    auto test = CCLoadSprite::createSprite("UI_hand.png");
//    m_touchLayer->addChild(test,2000);
//    test->setPosition(Vec2(500,500));
//    CCCommonUtils::setSpriteDiffuse(test, 50, Color3B::BLUE, test->getContentSize());
//    CCCommonUtils::setSpriteBlur(test, Vec2(0.1,0.1));
    //联盟喷泉
    m_fountainNode = CCNode::create();
    m_fountainNode->setPosition(m_FountainTouchNode->getPosition());
    m_fountainNode->setName("m_fountainNode");
    m_touchLayer->addChild(m_fountainNode, 2120);
    
    //人物引导手
//    m_flyArrow = CCLoadSprite::createSprite("UI_hand.png");//guide_arrow_new.png
//    m_flyArrow->setAnchorPoint(ccp(0, 0.5));

    
    m_flyArrow = CCSprite::create();
    auto per1Spr = CCSprite::create();
    CCCommonUtils::makeEffectSpr(per1Spr, "guide_hand_%d.png", 4, 0.1);
    per1Spr->setAnchorPoint(Vec2(0.5,1));
    per1Spr->setRotation(-135);
    m_flyArrow->addChild(per1Spr);
    m_flyArrow->setVisible(false);
    m_flyArrow->setName("m_flyArrow");

    
    
    //建造科研中心任务的粒子特效位置
    m_tileGlowNode = CCNode::create();
    m_tileGlowNode->setName("m_tileGlowNode");
    
    //云
    m_cloudLayer = CCLayer::create();
    m_cloudLayer->setName("m_cloudLayer");
    //云
    auto cloudSp = CCLoadSprite::loadResource("Cloud.png");
    m_cludeBatchNode = CCSpriteBatchNode::createWithTexture(cloudSp->getTexture());
    m_cludeBatchNode->setBlendFunc({GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA});
    m_cludeBatchNode->setPosition(ccp(0,120));
    m_cludeBatchNode->setName("m_cludeBatchNode");
    
    //地图触摸管理
    m_viewPort = HFViewport::create();
    m_viewPort->setViewPortTarget(m_touchLayer);
    m_viewPort->setSceneSize(sumWidth, sumHight);
    m_viewPort->setTouchDelegate(this);
    m_viewPort->setWorldBound(CCRect(0,250, sumWidth, sumHight));
    
    /*脚手架分层专用*/
    auto scaffolds = CCLoadSprite::loadResource("manin_city.png");    //Imperial_15
    m_scaffoldsBatchNode = CCSpriteBatchNode::createWithTexture(scaffolds->getTexture());
    m_touchLayer->addChild(m_scaffoldsBatchNode, BATCHNODE_ORDER_FALSEWORK_UP);//1503
    
    //建筑特效
    m_effectBlentBatchNode = CCSpriteBatchNode::createWithTexture(scaffolds->getTexture());
    //等级背景
    m_lvLayer = CCSpriteBatchNode::createWithTexture(scaffolds->getTexture());
    
    //装饰层
    auto decorate = CCLoadSprite::loadResource("vegetation_1.png");    //Imperial_13
    m_decorateBatchs[0]=CCSpriteBatchNode::createWithTexture(decorate->getTexture());
    m_touchLayer->addChild( m_decorateBatchs[0], BATCHNODE_ORDER_CITY_GATE1+2);
    m_decorateBatchs[1]=CCSpriteBatchNode::createWithTexture(decorate->getTexture());
    m_touchLayer->addChild( m_decorateBatchs[1], BATCHNODE_ORDER_CITY_GATE1-1);
    m_decorateBatchs[2]=CCSpriteBatchNode::createWithTexture(decorate->getTexture());
    m_touchLayer->addChild( m_decorateBatchs[2], BATCHNODE_ORDER_CITY_GATE3+1);
    
    m_decorateBatchs[3]=CCSpriteBatchNode::createWithTexture(decorate->getTexture());
    m_touchLayer->addChild( m_decorateBatchs[3], BATCHNODE_ORDER_BARRACKS_TRAIN1+1);
    m_decorateBatchs[4]=CCSpriteBatchNode::createWithTexture(decorate->getTexture());
    m_touchLayer->addChild( m_decorateBatchs[4], BATCHNODE_ORDER_BARRACKS_TRAIN2+1);
    m_decorateBatchs[5]=CCSpriteBatchNode::createWithTexture(decorate->getTexture());
    m_touchLayer->addChild( m_decorateBatchs[5], BATCHNODE_ORDER_BARRACKS_TRAIN3+1);
    
    m_touchLayer->setScale(FunBuildController::getInstance()->oldScale);
    m_touchLayer->addChild(m_waterLayer, BATCHNODE_ORDER_WATER);//水 91
    m_touchLayer->addChild(m_bgParticleLayer,101);//粒子
    m_touchLayer->addChild(m_tilebatchNode,BATCHNODE_ORDER_BUILD_STATE_TALK);//野外树2006
    m_touchLayer->addChild(m_forestbatchNode,BATCHNODE_ORDER_BUILD_STATE_TALK);//屏幕下方树2006
    m_touchLayer->addChild(m_nightLayer,BATCHNODE_ORDER_BUILD_NIGHT_BLACK);//夜晚 2001
    m_touchLayer->addChild(m_arrbatchNode,BATCHNODE_ORDER_BUILD_LOCK);//锁 2002
    m_touchLayer->addChild(m_nightLights, BATCHNODE_ORDER_BUILD_NIGHT);//灯 2003
    m_touchLayer->addChild(m_nameLayer,BATCHNODE_ORDER_BUILD_NAME);//名字 2004
    m_touchLayer->addChild(m_lvLayer, BATCHNODE_ORDER_BUILD_NAME-1);//等级 2003
    m_touchLayer->addChild(m_signLayer,BATCHNODE_ORDER_BUILD_EFFECT);//建筑特效层 2005
    m_touchLayer->addChild(m_effectBlentBatchNode, BATCHNODE_ORDER_BUILD_EFFECT);//建筑特效层 2005
    m_touchLayer->addChild(m_popLayer,BATCHNODE_ORDER_BUILD_STATE_TALK);////建筑状态（如升级等）、建筑气泡 2006
    m_touchLayer->addChild(m_cloudLayer,BATCHNODE_ORDER_CLOUD);//云层 2007
    m_touchLayer->addChild(m_cludeBatchNode, BATCHNODE_ORDER_CLOUD);//云 2007
    m_touchLayer->addChild(m_funLayer,BATCHNODE_ORDER_BUILD_BTNS);//城内巡逻兵气泡、建筑按钮菜单 2009
    m_touchLayer->addChild(m_dragonFlyNode, BATCHNODE_ORDER_DRAGON);//飞龙2010
    m_touchLayer->addChild(m_flyArrow,BATCHNODE_ORDER_TASK_HAND);//人物引导手层级 2020
    m_touchLayer->addChild(m_spineLayer, 2510);
    m_touchLayer->addChild(m_tileGlowNode,1180);
    m_touchLayer->addChild(m_battleLayer,BATCHNODE_ORDER_NEW_GUIDE);///新手引导战斗场景 3000
    m_touchLayer->addChild(m_soldierLayer,2999);//士兵 2999
    m_touchLayer->addChild(m_sSqerTouchNode,1996);//先知 1996
    
    
    
    float maxZoomScale = 1.2;
    float minZoomScale = m_viewPort->getMinZoom();
    float curZoomScale = 0.7;
    m_viewPort->setZoomLimitationMax(maxZoomScale);
    m_viewPort->setZoomLimitationMin(minZoomScale);
    m_viewPort->setName("m_viewPort");
    addChild(m_viewPort);
    m_touchLayer->setScale(curZoomScale);
    m_isLogin = false;
    if (FunBuildController::getInstance()->oldx == 99999 || (GlobalData::shared()->playerInfo.level==1 && GlobalData::shared()->playerInfo.exp==0)) {
        m_isLogin = true;
        string gFake = CCUserDefault::sharedUserDefault()->getStringForKey("Guide_Fake","");
        if (GlobalData::shared()->playerInfo.level==1 && GlobalData::shared()->playerInfo.exp==0 && (gFake=="" || gFake=="start_1")) {
//            onMoveToPos(480, 1560, TYPE_POS_MID, 0, 1.2, true);
            m_viewPort->updatePosition(Vec2(-300,-900));
        }
        else {
            if (PortActController::getInstance()->isCanRwd() && !(GuideController::share()->isInTutorial() || (GlobalData::shared()->playerInfo.level == 1 && GlobalData::shared()->playerInfo.exp == 0))) {
                m_viewPort->updatePosition(ccp(-1547, (winSize.height-sumHight+extH)));
            } else {
                m_viewPort->updatePosition(ccp(-1050, (winSize.height-sumHight+extH)));
            }
        }
    }
    else{
        if (PortActController::getInstance()->isCanRwd() && !(GuideController::share()->isInTutorial() || (GlobalData::shared()->playerInfo.level == 1 && GlobalData::shared()->playerInfo.exp == 0))) {
            m_viewPort->updatePosition(ccp(-1547, (winSize.height-sumHight+extH)));
        } else {
            m_viewPort->updatePosition(ccp(FunBuildController::getInstance()->oldx, FunBuildController::getInstance()->oldy));
        }
    }

    //喷泉和龙穴呼吸光
    InitDragonCaveGlow();
    if (m_penquanRainbow) {
        auto rainbow = CCLoadSprite::createSprite("penquanRainbow.png");
        ccBlendFunc cbf = {GL_SRC_ALPHA,GL_ONE};
        rainbow->setScale(0.7);
        rainbow->setBlendFunc(cbf);
        rainbow->setTag(9999);
        m_penquanRainbow->addChild(rainbow);
        m_penquanRainbow->setVisible(false);
    }
    
    m_torchNode->setZOrder(2);
    
    setCleanFunction([](){
        //CCLoadSprite::doResourceByImperialIndex(2, false);
        //CCLoadSprite::doResourceByImperialIndex(22, false);
        //CCLoadSprite::doResourceByImperialIndex(28, false);
    });
    struct timeval  tm;
    gettimeofday(&tm, NULL);
    m_mainPatPlayTime = 60+(tm.tv_usec / 1000)%60;
    //域名修复代码
    // devil 暂时关闭
//    string s1IP = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_IP, "");
//    if (s1IP != "" && s1IP == "184.173.110.102") {
//        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, "s1.cok.elexapp.com");
//        CCUserDefault::sharedUserDefault()->flush();
//    }
//    else if (s1IP != "" && s1IP == "184.173.110.99")
//    {
//        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, "s2.cok.elexapp.com");
//        CCUserDefault::sharedUserDefault()->flush();
//    }
    if (!GlobalData::shared()->isXMLInitFlag) {
        scheduleOnce(schedule_selector(ImperialScene::downloadXML), 3);
    }
//    m_sprBG1->visit();
    CCLOG("ImperialScene Init finish %lu",clock() - ulc);
//    playWatchGlow();
    if(WorldController::getInstance()->selfPoint.x < 0 && ActivityController::getInstance()->ckfNeedLeave!=1 ){
        CCDirector::sharedDirector()->getRunningScene()->addChild(CityCrashView::create());
    }
    return true;
}
void ImperialScene::downloadXML(float _time)
{
    GlobalData::shared()->isXMLInitFlag = true;
    GameController::getInstance()->m_manager = new UpdateManager();
    GameController::getInstance()->m_manager->setDelegate(GameController::getInstance());
    GameController::getInstance()->m_manager->update();
}

void ImperialScene::initPrincess()
{
    m_princessNode = CCNode::create();
    m_princessRootNode = Node::create();
    auto prinBg = CCLoadSprite::createSprite("princessTalkBg.png");
    prinBg->setAnchorPoint(ccp(0, 0));
    m_princessRootNode->addChild(prinBg);
    auto prinIcon = CCLoadSprite::createSprite("princessIcon.png");
    prinIcon->setAnchorPoint(ccp(0, 0));
    prinIcon->setPosition(ccp(20, 30));
    m_princessRootNode->addChild(prinIcon);
    auto prinTalk = CCLoadSprite::createSprite("princessTalk.png");
    prinTalk->setPosition(87, 48);
    m_princessRootNode->addChild(prinTalk);
    m_princessRootNode->setScale(1.2);
    m_princessNode->addChild(m_princessRootNode);
    m_princessNode->setContentSize(prinBg->getContentSize());
    m_popLayer->addChild(m_princessNode, 999999);
    m_princessNode->setVisible(false);
    m_princessBid = 0;
}

void ImperialScene::buildingCallBack(CCObject* params)
{
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("ImperialScene", "buildingCallBack");
    if (m_exit) {
        return;
    }
    if(!GlobalData::shared()->isInitFlag){
        return;
    }

    GuideController::share()->openSciencePanel();
    auto resSp = CCLoadSprite::loadResource("res_tile_1.png");//Imperial_2
    resSp->getTexture()->setAntiAliasTexParameters();
    m_resbatchNode =CCSpriteBatchNode::createWithTexture(resSp->getTexture());
    m_touchLayer->addChild(m_resbatchNode,BATCHNODE_ORDER_TILE);//地块和场景巡逻兵\对话等..活动建筑 199
    
    m_bubbleNode = CCSpriteBatchNode::createWithTexture(resSp->getTexture());
    m_touchLayer->addChild(m_bubbleNode,BATCHNODE_ORDER_BUILD_BTNS);//对话等..活动建筑 199


    ////城堡
    auto mainBuildBatch = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("pic400000_2.png")->getTexture());    //Imperial_10
    m_buildBatchMap[0] = mainBuildBatch;
    m_touchLayer->addChild(mainBuildBatch, BATCHNODE_ORDER_MAINBUILD);
    
    ////四种兵营暂定4 5 6 7
    auto bingyingSp = CCLoadSprite::loadResource("pic423000_2.png"); //Imperial_11
    for (int pos = TILE_BINGYING_1; pos <= TILE_BINGYING_4; ++ pos) {
        auto bingyingBatch = CCSpriteBatchNode::createWithTexture(bingyingSp->getTexture());
        m_buildBatchMap[pos] = bingyingBatch;
        int zorder = BATCHNODE_ORDER_BARRACKS;
        if (pos == TILE_BINGYING_1) {
            zorder = BATCHNODE_ORDER_BARRACKS;
        }else if (pos == TILE_BINGYING_2) {
            zorder = BATCHNODE_ORDER_BARRACKS;
        }
        else if (pos == TILE_BINGYING_3) {
            zorder = BATCHNODE_ORDER_BARRACKS_TRAIN2;
        }
        else if (pos == TILE_BINGYING_4) {
            zorder = BATCHNODE_ORDER_BARRACKS;
        }
        m_touchLayer->addChild(bingyingBatch, zorder);   //层级
    }
    
    ////城门+箭塔   zorder高于最外面的城墙
    auto resSpWall = CCLoadSprite::loadResource("pic418000_1_2.png");    //Imperial_12
    auto bdBatchWall = CCSpriteBatchNode::createWithTexture(resSpWall->getTexture());
    m_buildBatchMap[1] = bdBatchWall;
    m_buildBatchMap[2] = bdBatchWall;
    m_buildBatchMap[3] = bdBatchWall;
    m_touchLayer->addChild(bdBatchWall, BATCHNODE_ORDER_CITY_GATE1);///城门\箭塔 1499
    
    auto chrTree = CCLoadSprite::loadResource("qdt_ditu_.png");
    chrTree->getTexture()->setAntiAliasTexParameters();
    m_chrTreeBatchNode = CCSpriteBatchNode::createWithTexture(chrTree->getTexture());
    m_touchLayer->addChild(m_chrTreeBatchNode,250);
    
    auto chrBlentTree = CCLoadSprite::loadResource("pichuochuanGlow.png");
    chrBlentTree->getTexture()->setAntiAliasTexParameters();
    m_chrTreeBlentBatchNode = CCSpriteBatchNode::createWithTexture(chrBlentTree->getTexture());
    m_chrTreeBlentBatchNode->setBlendFunc(kCCBlendFuncTree);
    m_touchLayer->addChild(m_chrTreeBlentBatchNode,1997);
    
    ////城外建筑
    auto chengwaiSp = CCLoadSprite::loadResource("pic412000_2.png"); //Imperial_14
    auto chengwaiBatch = CCSpriteBatchNode::createWithTexture(chengwaiSp->getTexture());
    m_touchLayer->addChild(chengwaiBatch, BATCHNODE_ORDER_BUILD_OUT);
    for (int pos = TILE_CHENG_WAI_IDX_BEGIN1; pos <= TILE_CHENG_WAI_IDX_END1; ++ pos) {
        m_buildBatchMap[pos] = chengwaiBatch;
    }
    
    ////城内其他建筑
    auto chengneiSp = CCLoadSprite::loadResource("pic401000_2.png"); //Imperial_16
    for (int i = 4; i < BUILD_COUNT; i++) {
        
        //去掉四种兵营
        if (i >= TILE_BINGYING_1 && i <= TILE_BINGYING_4) {
            continue;
        }
        //去掉城外
        if (i >= TILE_CHENG_WAI_IDX_BEGIN1 && i <= TILE_CHENG_WAI_IDX_END1) {
            continue;
        }
        //tmpZorder 不能高于 BATCHNODE_ORDER_BACKWALL 城墙
        int tmpZorder = 1000;
        if(i==8) {//仓库
            tmpZorder = 201;
        }
        else if(i==9) {//学院
            tmpZorder = 200;
        }
        else if(i==10) {//医院
            tmpZorder = 200;
        }
        else if(i==11) {//忠义堂
            tmpZorder = 203;
        }
        else if(i==12) {//驿站
            tmpZorder = 200;
        }
        else if(i==13) {//市场
            tmpZorder = 200;
        }
        else if(i==14) {//校场
            tmpZorder = 200;
        }
        else if(i==15) {//行军帐篷
            tmpZorder = BATCHNODE_ORDER_BUILD_OUT;
        }
        else if(i==16) { //瞭望塔
            tmpZorder = BATCHNODE_ORDER_BUILD_OUT;
        }
        else if (i == 52) {//铁街铺
            tmpZorder = 200;
        }
        else if (i == 53) {//
            tmpZorder = 200;
        }
        else if (i == 54) {//
            tmpZorder = 200;
        }
        
        auto bdBatch = CCSpriteBatchNode::createWithTexture(chengneiSp->getTexture());
        m_buildBatchMap[i] = bdBatch;
        m_touchLayer->addChild(bdBatch,tmpZorder);
    }
    
    //特殊建筑物 Imperial_2
    if(true) {
        auto bdBatch1 = CCSpriteBatchNode::createWithTexture(resSp->getTexture());
        m_buildBatchMap[100] = bdBatch1;
        m_touchLayer->addChild(bdBatch1,1009);
        
        auto bdBatch2 = CCSpriteBatchNode::createWithTexture(resSp->getTexture());
        m_buildBatchMap[101] = bdBatch2;
        m_touchLayer->addChild(bdBatch2,702);
    }

//    auto resSp2 = CCLoadSprite::loadResource("feedback_head.png");//dsg_house_1
//    resSp2->getTexture()->setAntiAliasTexParameters();
//    m_resbatchNode22=CCSpriteBatchNode::createWithTexture(resSp2->getTexture());
//    m_touchLayer->addChild(m_resbatchNode22,199);
    
	//新建筑层次结构
    for (int i=0; i<BUILD_COUNT; i++) {
        auto bdLayer = CCLayer::create();
        m_buildSpineMap[i] = bdLayer;
        m_touchLayer->addChild(bdLayer,m_buildBatchMap[i]->getZOrder());
        
        auto bdBatch = CCSpriteBatchNode::createWithTexture(resSp->getTexture());
        bdBatch->setTag(1);
        bdLayer->addChild(bdBatch);
    }
	
    m_soldierFlagNode = CCNode::create();
    m_touchLayer->addChild(m_soldierFlagNode,210);
    
    m_resBlentBatchNode = NULL;
//    auto resBlentSp = CCLoadSprite::loadResource("pic400000_2.png");//dsg_house_1
//    resBlentSp->getTexture()->setAntiAliasTexParameters();
//    m_resBlentBatchNode = CCSpriteBatchNode::createWithTexture(resBlentSp->getTexture());
//    m_touchLayer->addChild(m_resBlentBatchNode,199);
    
    //鹰
    auto eagle = EagleCCB::create();
    eagle->setPosition(ccp(1000, 1000));
    m_touchLayer->addChild(eagle, 2500);

    //发生 22资源释放不掉的 区域 start start start start
    onUpdateInfo();
    checkTileGlow(NULL);
    
    initMc2();
    m_buildingInitState = true;
    onEnterFrame(0);
    initBgTree();
    initAnimation();
    initBigTile();
    onOpenNewBuild(NULL);
    //发生 22资源释放不掉的 区域 end end end end end end
    
//    initAnimals();
    UIComponent::getInstance()->updateBuildState(true);
    
    bool canMoveToRequest = true;
    if(!FunBuildController::getInstance()->isFirst)
    {
        m_curBuildPosx = 1010;
        m_curBuildPosy = 1256;
        float dt = 0.35;
        float scale = 0.8;
        bool newPlayerST = false;
        string gFake = CCUserDefault::sharedUserDefault()->getStringForKey("Guide_Fake","");
        if (GlobalData::shared()->playerInfo.level==1 && GlobalData::shared()->playerInfo.exp==0 && (gFake=="" || gFake=="start_1")) {
            //onPlayBattle();
            newPlayerST = true;
            m_curBuildPosx = 480;
            m_curBuildPosy = 1560;
            scale = 1.2;
            canMoveToRequest = false;
            dt = 0;
        } else {
            if (!QuestController::getInstance()->m_returnFromWorld && PortActController::getInstance()->isCanRwd() && !(GuideController::share()->isInTutorial() || (GlobalData::shared()->playerInfo.level == 1 && GlobalData::shared()->playerInfo.exp == 0))) {
                m_curBuildPosx = 2530;
                m_curBuildPosy = 1430;
                dt = 0;
                canMoveToRequest = false;
            }
        }
        // devil 新手引导镜头问题现在没做
//        onMoveToPos(m_curBuildPosx, m_curBuildPosy, TYPE_POS_MID, dt, scale, true);
        m_viewPort->updatePosition(Vec2(-704,-569));
        m_viewPort->setCurZoomScale(0.75);
        FunBuildController::getInstance()->isFirst=true;
        scheduleOnce(schedule_selector(ImperialScene::startGuide), 0.5f);
    }
    else
    {
        startGuide(0);
        if (!QuestController::getInstance()->m_returnFromWorld && PortActController::getInstance()->isCanRwd() && !(GuideController::share()->isInTutorial() || (GlobalData::shared()->playerInfo.level == 1 && GlobalData::shared()->playerInfo.exp == 0))) {
            m_curBuildPosx = 2530;
            m_curBuildPosy = 1430;
            float scale = 0.8;
            canMoveToRequest = false;
            onMoveToPos(m_curBuildPosx, m_curBuildPosy, TYPE_POS_MID, 0, scale, true);
        }
    }

    cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Battle/Battle_soldier.plist");
    m_soldierBatchNode = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("a010_0_N_attack_0.png")->getTexture());
    m_touchLayer->addChild(m_soldierBatchNode, 1999);
    
    refreshSoldiers(NULL);
    
    m_talkACTCell = TalkNoticeCell::create(0);
    m_talkACTCell->setVisible(false);
    m_funLayer->addChild(m_talkACTCell, -1);
    
    m_talkACTCell2 = TalkNoticeCell::create(1);
    m_talkACTCell2->setVisible(false);
    m_funLayer->addChild(m_talkACTCell2, -1);
    
    if(canMoveToRequest && QuestController::getInstance()->m_returnFromWorld){
        QuestInfo *info = QuestController::getInstance()->getRecommendQuest(false);
        QuestController::getInstance()->goToQuestTarget(info, true);
    }
    
    getScienceData();
    
    
    //fail fire
    
    auto now = WorldController::getInstance()->getTime();
    if(now < GlobalData::shared()->cityFireStamp){
        if(m_failEffectNode->getChildrenCount()<=0){
           this->addDefenseFailEffect();
        }
    }else{
        m_failEffectNode->removeAllChildrenWithCleanup(true);
    }
    
    int tmpMoveToBuild = FunBuildController::getInstance()->willMoveToBuildType;
    if (tmpMoveToBuild > 0) {
        FunBuildController::getInstance()->willMoveToBuildType = 0;
        int bid = FunBuildController::getInstance()->getMaxLvBuildByType(tmpMoveToBuild);
        if (bid>0) {
            onMoveToBuildAndPlay(bid);
        }else {
            int itemId = findCanBuildTile(tmpMoveToBuild);
            if(itemId==-1){
                itemId = findCanBuildTile(2);
                if(itemId==-1) {
                    return ;
                }
            }
            onMoveToBuildAndPlay(itemId);
        }
    }
    
    GameController::getInstance()->enableQueryHistoryPurchase();
    
    if(!GuideController::share()->isInTutorial() && GlobalData::shared()->playerInfo.isInAlliance() && GlobalData::shared()->dbFightBeginTime < GlobalData::shared()->getWorldTime()  && GlobalData::shared()->dbFightEndTime>GlobalData::shared()->getWorldTime() && ActivityController::getInstance()->isMoveToDragonBuild){
       onMoveToPos(3280, 236, TYPE_POS_MID, 0, 1, true);
    }
}

//void ImperialScene::MiracleCallBack(CCObject* params)
//{
//    GameController::getInstance()->setLoadingLog("ImperialScene", "MiracleCallBack");
//    if (m_exit) {
//        return;
//    }
//    if(!GlobalData::shared()->isInitFlag){
//        return;
//    }
//    GuideController::share()->openSciencePanel();
//    auto resSp = CCLoadSprite::loadResource("pic400000_2.png");//dsg_house_1
//    resSp->getTexture()->setAntiAliasTexParameters();
//    m_mirabathNode = CCSpriteBatchNode::createWithTexture(resSp->getTexture());
//    m_mirabathNode->setPosition(47,47);
//    m_touchLayer->addChild(m_mirabathNode,199);
//    
//  }

void ImperialScene::wallCallBack(CCObject* params)
{
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("ImperialScene", "wallCallBack");
    if (m_exit) {
        return;
    }
    if(!GlobalData::shared()->isInitFlag){
        return;
    }
    if(!GlobalData::shared()->isServerInfoBack){
        WorldController::getInstance()->getServerList();
    }
    
    auto frame = CCLoadSprite::loadResource("pic_ziyuan_men.png");
    frame->getTexture()->setAntiAliasTexParameters();
    m_wallTextureBatchNode = SpriteBatchNode::createWithTexture(frame->getTexture());
    m_touchLayer->addChild(m_wallTextureBatchNode, 2015);
    
    auto wallSp=CCLoadSprite::createSprite("wall_01.png");
    for(int i=0;i<3;i++)
    {
        m_wallBatchs[i] = CCSpriteBatchNode::createWithTexture(wallSp->getTexture());
        if(i==0)
        {
            m_touchLayer->addChild(m_wallBatchs[i], BATCHNODE_ORDER_CITY_GATE1);
        }
        else if (i==1)
        {
            m_touchLayer->addChild(m_wallBatchs[i], BATCHNODE_ORDER_CITY_GATE2);
        }
        else if (i==2)
        {
            m_touchLayer->addChild(m_wallBatchs[i], BATCHNODE_ORDER_CITY_GATE3);
        }
        m_wallBuilds[i]=WallBuild::create(i+1);
        m_wallBuilds[i]->setNamePos(m_wallNodes[0]->getPositionX(), m_wallNodes[0]->getPositionY(), m_signLayer, m_wallBatchs[i],m_wallBatchs[i],0);
        m_wallNodes[i]->addChild(m_wallBuilds[i]);
    }
    
    initSpeBuildInWallTexture();
    
    getFriendsData();
    
}

void ImperialScene::initSpeBuildInWallTexture()
{
    if (true) {
        m_ziyuanmenBuild = SpeBuild::create(SPE_BUILD_ZIYUANMEN);
        m_ziyuanmenNode->addChild(m_ziyuanmenBuild);
        int hod = m_ziyuanmenNode->getZOrder();
        m_ziyuanmenBuild->setNamePos(m_ziyuanmenNode->getPositionX(), m_ziyuanmenNode->getPositionY(), m_signLayer, m_popLayer, nullptr, m_wallBatchs[0],hod);
    }
}

void ImperialScene::videoPlayContinueCallback()
{
    auto videoPlayer=dynamic_cast<cocos2d::experimental::ui::VideoPlayer*>(this->getChildByName("videoPlayer"));
    if(!videoPlayer->isPlaying())
        videoPlayer->play();
}

void ImperialScene::videoPlayOverCallback()
{
    auto videoPlayer=dynamic_cast<cocos2d::experimental::ui::VideoPlayer*>(this->getChildByName("videoPlayer"));
    videoPlayer->stop();
    videoPlayer->setVisible(false);
    m_touchLayer->setVisible(true);
    SoundController::sharedSound()->playBGMusic(Music_M_city_1);
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void ImperialScene::videoEventCallback(Ref* sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType){
    switch (eventType) {
        case cocos2d::experimental::ui::VideoPlayer::EventType::PLAYING:
            break;
        case cocos2d::experimental::ui::VideoPlayer::EventType::PAUSED:
            videoPlayContinueCallback();
            break;
        case cocos2d::experimental::ui::VideoPlayer::EventType::STOPPED:
            videoPlayContinueCallback();
            break;
        case cocos2d::experimental::ui::VideoPlayer::EventType::COMPLETED:
            videoPlayOverCallback();
            break;
        default:
            break;
    }
}
#endif

void ImperialScene::onPlayBattle()
{
    SoundController::sharedSound()->SoundController::stopAllMusic();
    auto size = CCDirector::sharedDirector()->getWinSize();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    auto videoPlayer = cocos2d::experimental::ui::VideoPlayer::create();
    videoPlayer->setPosition(Point(size.width/2 , size.height/2));
    videoPlayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    videoPlayer->setContentSize(Size(size.width , size.height));
    videoPlayer->setName("videoPlayer");
    this->addChild(videoPlayer);
    if (videoPlayer)
    {
        videoPlayer->setFileName("sounds/background/cg_newbie.mp4");
        videoPlayer->play();
    }
    videoPlayer->addEventListener(CC_CALLBACK_2(ImperialScene::videoEventCallback, this));
#endif
    
//    if (CCCommonUtils::isTestPlatformAndServer("Guide_Ani_Test"))
//    {
//        NewPlayerBattleScene* newplayerScence = NewPlayerBattleScene::create();
//        newplayerScence->setTag(9);
//        m_battleLayer->addChild(newplayerScence);
//    }
//    else
//    {
//        BattlefieldScene* battle = BattlefieldScene::create();
//        battle->setTag(9);
//        m_battleLayer->addChild(battle);
//    }
//    
//    m_guideParticleNode = CCNode::create();
//    m_guideParticleNode->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, 0));
//    this->addChild(m_guideParticleNode);
//    for (int i=1; i<=4; i++) {
//        if (i!=3 && i!=4) {
//            auto particle = ParticleController::createParticle(CCString::createWithFormat("OPFire_%d",i)->getCString());
//            addGuideParticleToBatch(particle);
//        }
//    }
}

void ImperialScene::onPlayMoveTroops(int buildId)
{
    MainCityArmy* army = MainCityArmy::create(m_touchLayer, m_soldierBatchNode, buildId);
    if(army) {
        m_soldierLayer->addChild(army);
    }
}

void ImperialScene::startGuide(float _time)
{
    setUnMoveScence(false);
    if(WorldController::getInstance()->selfPoint.x < 0){
        return;
    }
    GuideController::share()->start();
    //    getBuildById("415000");
//    bool isOpen = false;
//    bool showMonthCard = ActivityController::getInstance()->showMonthCard();
//    GoldExchangeItem *monthCard = NULL;
//    for (map<string, GoldExchangeItem*>::iterator it = GlobalData::shared()->goldExchangeList.begin(); it != GlobalData::shared()->goldExchangeList.end(); it++) {
//        if(!it->second)
//            continue;
//        if(it->second->type == "5"){
//            monthCard = it->second;
//        }else{
//            if(it->second->type == "3"){
//                if(it->second->bought||it->second->popup_image=="close"){
//                    continue;
//                }
//                if(it->second->end>GlobalData::shared()->getWorldTime()){
//                    isOpen= true;
//                }
//            }
//        }
//    }
//    if(isOpen == false && showMonthCard && monthCard){
//        if(GlobalData::shared()->monthCardInfoList.find(monthCard->id)!=GlobalData::shared()->monthCardInfoList.end()){
//            MonthCardInfo *card =GlobalData::shared()->monthCardInfoList[monthCard->id];
//            if(card && card->canReward()==false){
//                isOpen= true;
//            }
//        }
//    }
    
    UserUpgradeView*  pop = dynamic_cast<UserUpgradeView*>(PopupViewController::getInstance()->getCurrentPopupView());
    

    if (m_isLogin && !GuideController::share()->isInTutorial() && pop==NULL) {//没有弹开有升级界面才弹活动界面
        if(GlobalData::shared()->analyticID != "common" && GlobalData::shared()->shortItemType.empty()){
//            if (!CCCommonUtils::isIosAndroidPad()) {
                ActivityController::getInstance()->showEnterView();
//            }
        }
        else
        {
            GlobalData::shared()->shortItemType = "";
        }
    }
    
    if (m_isLogin) {//弹开装备锻造完成的界面
        string uuid = QueueController::getInstance()->getLoginFinishQueueUuId(TYPE_FORGE);
        if (uuid != "") {
            string itemId = QueueController::getInstance()->loginFinishQueues[uuid];
            auto equipView = EquipFinishView::create(atoi(itemId.c_str()));
            if (!CCCommonUtils::isIosAndroidPad())
            {
                if (PopupViewController::getInstance()->getCurrViewCount()>0) {
                    PopupViewController::getInstance()->pushPop(equipView);
                }else {
                    PopupViewController::getInstance()->addPopupView(equipView);
                }
            }
        }
    }
    
    LuaController::getInstance()->onImperialEnter();
//    if (m_isLogin) {
//        if (QuestController::getInstance()->onCheckSpeTask("2900101")) {
//            if (PopupViewController::getInstance()->getCurrViewCount()<=0) {
//                PopupViewController::getInstance()->addPopupView(FiveStarTaskView::create());
//            }
//            else {
//                PopupViewController::getInstance()->pushPop(FiveStarTaskView::create());
//            }
//        }
//    }
}

void ImperialScene::onEnter()
{
    CCLayer::onEnter();
    if(enterMainCityTime == 0){
        firstEnterMainCity();
    }
    enterMainCityTime++;
    CCLoadSprite::doLoadResourceAsync(IMPERIAL_PATH, CCCallFuncO::create(this, callfuncO_selector(ImperialScene::buildingCallBack), NULL),16);
    //CCLoadSprite::doLoadResourceAsync(IMPERIAL_PATH, CCCallFuncO::create(this, callfuncO_selector(ImperialScene::buildingCallBack), NULL), 2+10*GlobalData::shared()->contryResType);
    CCLoadSprite::doLoadResourceAsync(IMPERIAL_PATH, CCCallFuncO::create(this, callfuncO_selector(ImperialScene::wallCallBack), NULL), 12);
    
    // devil 关闭动态资源下载
//    CCLoadSprite::doLoadCommonResourceAsync();
    
    UIComponent::getInstance()->onCreateCropSceneUI();
    
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ImperialScene::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    
    
    //    //CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, Touch_Default);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onQijiBottomDelete), MSG_OPENQIJI, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onOpenNewBuild), MSG_FUNBUILD_OPEN, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onGoBackOldPos), MSG_MAINSCENCE_GOBACK, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onSaveCurPos), MSG_MAINSCENCE_SAVEPOS, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onRemoveTmpBuild), MSG_MAINSCENCE_REMOVE_TMPBUILD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::unLockTile), MSG_UNLOCK_TILE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::moveMapToPosition), MSG_MOVE_TO_POSITION, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::refreshSoldiers), MSG_TROOPS_CHANGE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::guideEnd), GUIDE_END, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onPowerADD), MSG_SCIENCE_POWER_ADD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::checkTileGlow), QUEST_STATE_UPDATE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::createActiveBd), MSG_INIT_ACTIVITY_EVENT, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onShowTalkView), MSG_TALK_OPEN, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::showHarvestEffect), MSG_SHANGE_SCENCE_SHOW_HARVEST, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onFinishMoveBuild), MSG_FINISH_MOVE_BUILD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onFlyCargoOutPut), MSG_NEW_TIME_RWD_END, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onStopAllActions), MSG_STOP_CITY_MOVE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onRemoveGuideParticle), GUIDE_BATTLE_END, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::showFirework), "showFirework", NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::showPrincessByMsg), MSG_ShowPrincess, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::showFlyBall), MSG_FLYBALL_EFFECT, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onGetMsgRefreshBuild), MSG_REFRESH_BUILD, nullptr);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onCheckNewPlayerScene), GUIDE_BEGIN3, nullptr);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::setTipsType), MSG_IMPERIAL_SCENE_TIPSON, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::reSetTipsType), MSG_IMPERIAL_SCENE_TIPSOFF, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::princessRwdShow), MSG_PrincessRwd, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::princessRwdNullShow), MSG_PrincessRwdNull, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onShowPatrol), GUIDE_BEGIN4, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onShowRebuild), GUIDE_BEGIN5, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::newrwdGuideEndConfirm), MSG_NEWRWD_GUIDE_END, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::sqerSoldierAniComplete), MSG_SQERSOLDIER_ANIFINISH, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onShowMainCityBuff), GUIDE_MAINCITY_BUFF, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onShowMainCitySkin), GUIDE_MAINCITY_SKIN, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::shakeViewport), MSG_VIEWPORT_SHAKE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::allianceRecommandShow), MSG_ReturnFrom_ChatOrMail, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::addEnergyEff), MSG_STAMINE_ADD_EFF, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ImperialScene::onShowGuideWhenFree), MSG_GUIDE_SHOW_FREE, NULL);
    string gFake = CCUserDefault::sharedUserDefault()->getStringForKey("Guide_Fake","");
    if (GlobalData::shared()->playerInfo.level==1 && GlobalData::shared()->playerInfo.exp==0 && (gFake==""||gFake=="start_1")) {
        
    }else{
        
        SoundController::sharedSound()->playBGMusic(Music_M_city_1);
//        SoundController::sharedSound()->playBGMusic(Music_M_city_1_preview);
    }
    
    m_isDay = true;//默认白天
    checkDayTime(true);//判断是否是白天
    changeDayOrNight(!m_isDay , false);
    
    makeWater(0);
    
    m_dropNode = NULL;
    if(!m_praticle){
        if (GlobalData::shared()->contryResType == 1) {
            
        }else if (GlobalData::shared()->contryResType == 2) {
            int rIdx = CCMathUtils::getRandomInt(0, 9);
            if (/* DISABLES CODE */ (false) && rIdx < 5) {
                showRain();
            }
        }
        else {
            m_dropNode = CCNode::create();
            m_dropNode->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2,CCDirector::sharedDirector()->getWinSize().height/2));
            if (CCCommonUtils::isIosAndroidPad())
            {
                m_dropNode->setScale(2.4);
            }
            this->addChild(m_dropNode);
            
            int rIdx = CCMathUtils::getRandomInt(0, 9);
            if (rIdx < 5) {
                showRain();
            }
        }
    }

    int userLv = GlobalData::shared()->lordInfo.levelUp;
    if(userLv<GlobalData::shared()->playerInfo.level){
        CCDictionary* tmp = CCDictionary::create();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PLAYER_LEVELUP);
    }
    this->scheduleOnce(schedule_selector(ImperialScene::scheduleHarvestEffect), 0.45);
    //    GuideController::share()->start();
    // devil 关闭好友邀请
//    BranchController::getInstance()->excute("InviteForGift");
    
    if (GuideController::share()->getCurrentId() == "3080100") {
        onMoveToPos(2250, 630, TYPE_POS_MID, 0.0, 0.8, true);
    }
    
    isGoIn=false;
}

void ImperialScene::showRain()
{
    string particleName = "rain"; // "snow", "rain"
    m_isRain = true;
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    m_praticle = ParticleController::createFightingParticle(particleName.c_str(), 60, ccp(winSize.width / 2 - 50, winSize.height + 10));
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_praticle->setScale(2.4);
    }
    this->addChild(m_praticle);
    
    if (particleName == "rain") {
        for (int i=1; i<=3; i++) {
            if (m_dropNode) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("Drop_%d",i)->getCString());
                m_dropNode->addChild(particle);
            }
        }
    }
    this->scheduleOnce(schedule_selector(ImperialScene::clearDrop), 65);
}

void ImperialScene::showFirework(CCObject* obj){
//    CCDictionary* dict = dynamic_cast<CCDictionary*>(obj);
//    m_useItemId = dict->valueForKey("itemId")->intValue();
//    int num = 20;
//    if (dict->objectForKey("num")) {
//        num = dict->valueForKey("num")->intValue();
//    }
    if (CCFileUtils::sharedFileUtils()->isFileExist("Spine/Imperial/main_city_upgrade.json") &&
        CCFileUtils::sharedFileUtils()->isFileExist("Imperial/Imperial_31.atlas"))
    {
        auto animationObj = new IFSkeletonAnimation("Spine/Imperial/main_city_upgrade.json","Imperial/Imperial_31.atlas");
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        animationObj->setPosition(winSize.width * 0.5, winSize.height * 0.5);
        this->addChild(animationObj);
        animationObj->setAnimation(0, "animation", false);
        animationObj->autorelease();
        
        this->schedule(schedule_selector(ImperialScene::startShowFirework), 0.0f, 1, 1.7f);
    }
    
//    std::string textName = FileUtils::getInstance()->fullPathForFilename("Imperial/Imperial_31.pvr.ccz");
//    CCTexture2D *texture =  CCTextureCache::sharedTextureCache()->textureForKey(textName.c_str());
//    if(texture){
//        CCTextureCache::sharedTextureCache()->removeTexture(texture);
//    }
//    "main_city_upgrade.json"
//    if (m_useItemId==ITEM_FIREWORK2) {
//        SoundController::sharedSound()->playEffects(Music_Sfx_firework_in2);
//        this->schedule(schedule_selector(ImperialScene::startShowFirework), 0.25, num, 0.0f);
//    }else{
//        SoundController::sharedSound()->playEffects(Music_Sfx_firework_in1);
//        this->schedule(schedule_selector(ImperialScene::startShowFirework), 0.25, 12, 0.0f);
//    }
}

void ImperialScene::startShowFirework(float t){
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto particle = ParticleController::createParticle("main_city_upgrade");
    particle->setAutoRemoveOnFinish(true);
    particle->setPosition(ccp(winSize.width * 0.5 , winSize.height * 0.5));
    particle->setScale(3);
    this->addChild(particle);
    // devil 关闭旧的粒子创建
//    float ww = winSize.width;
//    float wh = winSize.height;
//    int fIdx = CCMathUtils::getRandom(50, ww-50);
//    int fIdy = CCMathUtils::getRandom(300, wh-200);
//    int colorIndex = CCMathUtils::getRandomInt(1,6);
//    for (int i=1; i<=5; i++) {
//        CCParticleSystemQuad* particle=NULL;
//        if (m_useItemId==ITEM_FIREWORK2) {
//            particle = ParticleController::createParticle(CCString::createWithFormat("Fireworks_%d_%d",colorIndex,i)->getCString());
//            particle->setPosition(ccp(fIdx , fIdy));
//        }else{
//            particle = ParticleController::createParticle(CCString::createWithFormat("Fireworks_1_%d",i)->getCString());
//            particle->setPosition(ccp(fIdx , fIdy));
//        }
//        float rdmSize = CCMathUtils::getRandom(0.8,2.5);
//        if (i==1) {
//            rdmSize = 0.9;
//        }
//        particle->setScale(rdmSize);
//        this->addChild(particle);
//    }
}

void ImperialScene::addNightLight(){

    if(!m_nightLights){
        return;
    }
    auto addToNightLightLayer = [](CCNode *parentNode, int tag, const CCPoint &pt){
        std::string ccbPath = CCString::createWithFormat("pic%d_2_Night", tag)->getCString();
        if(tag == 5){
            ccbPath = "picMerchante_Night";
        }else if(tag == 6){
            ccbPath = "picjiuguan_2_Night";
        }
        else if(tag == 7){
            ccbPath = "picxiongdihui_Night";
        }
        else if(tag == 8){
            ccbPath = "pichuochuan_Night";
        }
        
        auto node = CCNode::create();
        node->setTag(tag);
        node->setPosition(pt);
        auto ccb = CCBLoadFile(ccbPath.c_str(), node, NULL, true);
        parentNode->addChild(node);
        //test
//        auto spriteContainer = CCLoadSprite::createSprite("pic400000_2.png");
//        spriteContainer->setTextureRect(CCRectZero);
//        parentNode->addChild(spriteContainer);
//        spriteContainer->setPosition(pt);
//        spriteContainer->setTag(tag);
//        auto node1 = dynamic_cast<CCNode*>(ccb->getChildren()->objectAtIndex(0));
//        auto arr = node1->getChildren();
//        int index = 0;
//        while (index < arr->count()) {
//            CCSprite *sprite = dynamic_cast<CCSprite*>(arr->objectAtIndex(index));
//            if(sprite){
//                sprite->retain();
//                sprite->removeFromParentAndCleanup(false);
//                spriteContainer->addChild(sprite);
//                sprite->release();
//            }
//            index++;
//        }
    };
    
    for (auto it = m_buildItems.begin(); it != m_buildItems.end(); it++) {
        int itemId = it->first / 1000;
        if(
           itemId == FUN_BUILD_MAIN
           || itemId == FUN_BUILD_TAVERN
           || itemId == FUN_BUILD_STABLE
           || itemId == FUN_BUILD_SCIENE
           || itemId == FUN_BUILD_CELLAR
           || itemId == FUN_BUILD_SMITHY
           || itemId == FUN_BUILD_FORT
           || itemId == FUN_BUILD_WATCH_TOWER
           || itemId == FUN_BUILD_BARRACK1
           || itemId == FUN_BUILD_BARRACK2
           || itemId == FUN_BUILD_BARRACK3
           || itemId == FUN_BUILD_BARRACK4
           || itemId == FUN_BUILD_FORGE
           ){
            if (itemId != FUN_BUILD_MAIN)
            {
                if(!m_nightLights->getChildByTag(itemId)){
                    int buildPosX = it->second->getParent()->getPositionX();
                    int buildPosY = it->second->getParent()->getPositionY();
                    addToNightLightLayer(m_nightLights, itemId, ccp(buildPosX, buildPosY));
                }
            } else {
                bool st = false;
                if (!ToolController::getInstance()->getMaincitySkin().empty()) {
                    st = true;
                }
                auto view = PopupViewController::getInstance()->getCurrentPopupView();
                SkinListView* skinView = dynamic_cast<SkinListView*>(view);
                if (view) {
                    st = true;
                }
                if (st) {
                    if (m_nightLights->getChildByTag(itemId)) {
                        m_nightLights->removeChildByTag(itemId);
                    }
                } else {
                    if (!m_nightLights->getChildByTag(itemId)) {
                        int buildPosX = it->second->getParent()->getPositionX();
                        int buildPosY = it->second->getParent()->getPositionY();
                        addToNightLightLayer(m_nightLights, itemId, ccp(buildPosX, buildPosY));
                    }
                }
            }
        }
    }
    if(m_merchanteBuild && m_merchanteBuild->isVisible()){
        if(!m_nightLights->getChildByTag(5)){
            int buildPosX = m_merchanteBuild->getParent()->getPositionX();
            int buildPosY = m_merchanteBuild->getParent()->getPositionY();
            addToNightLightLayer(m_nightLights, 5, ccp(buildPosX, buildPosY));
        }
    }else{
        m_nightLights->removeChildByTag(5);
    }
    
    if(m_goldBoxBuild && m_goldBoxBuild->isVisible() && !m_nightLights->getChildByTag(6)){
        int buildPosX = m_goldBoxBuild->getParent()->getPositionX();
        int buildPosY = m_goldBoxBuild->getParent()->getPositionY();
        addToNightLightLayer(m_nightLights, 6, ccp(buildPosX, buildPosY));
    }
    
    if(m_xiongdiBuild && m_xiongdiBuild->isVisible()){
        if(!m_nightLights->getChildByTag(7)){
            int buildPosX = m_xiongdiBuild->getParent()->getPositionX();
            int buildPosY = m_xiongdiBuild->getParent()->getPositionY();
            addToNightLightLayer(m_nightLights, 7, ccp(buildPosX, buildPosY));
        }
    }else{
        m_nightLights->removeChildByTag(7);
    }
    
    if(m_shipBuild && m_shipBuild->isVisible()){
        if(!m_nightLights->getChildByTag(8)){
            int buildPosX = m_shipBuild->getParent()->getPositionX();
            int buildPosY = m_shipBuild->getParent()->getPositionY();
            addToNightLightLayer(m_nightLights, 8, ccp(buildPosX, buildPosY));
        }
    }else{
        m_nightLights->removeChildByTag(8);
    }
}

void ImperialScene::removeAllNightLight(){
    if(m_nightLights){
        m_nightLights->removeAllChildren();
    }
}

void ImperialScene::changeDayOrNight(bool isNight, bool isAni)
{
    struct tm tm;
    char h[100];
    char m[100];

    time_t timer = time(0);
    tm = *localtime(&timer);
    strftime(h, sizeof(h), "%H", &tm);
    int hh = atoi(h);
    strftime(m, sizeof(m), "%M", &tm);
    int mm = atoi(m);
    
    int maxOpacity1 = 50;
    int maxOpacity2 = 50;
    int maxOpacity3 = 180;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    maxOpacity1 = 90;
    maxOpacity2 = 90;
    maxOpacity3 = 200;
#endif
    bool addTorchFlag = false;
    if(((startDarkTime <= completeLightTime) && (hh >= completeLightTime || hh < startDarkTime)) || ((startDarkTime > completeLightTime) && (hh < startDarkTime && hh >= completeLightTime))){
        addTorchFlag = false;
        m_nightLayer->removeAllChildren();
        if(!m_isDay){
            m_isDay = true;
            showSunshine();
        }
        m_isDay = true;
    }else{
        m_sunNode->removeAllChildren();
        this->unschedule(schedule_selector(ImperialScene::setSunAngle));

        auto layerSize = m_nightLayer->getContentSize();
        CCLayerColor *blacklayer = NULL;
        int layerColorTag = 10;
        int blueColorTag = 11;
        int whiteColorTag = 12;
        
        int opacity1 = 0;
        int opacity2 = 0;
        int opacity3 = 0;
        if(((completeDarkTime <= startLightTime) && (hh >= completeDarkTime && hh < startLightTime)) || ((completeDarkTime > startLightTime) && (hh >= completeDarkTime || hh < startLightTime))){
            opacity1 = maxOpacity1;
            opacity2 = maxOpacity2;
            opacity3 = maxOpacity3;
            addTorchFlag = true;
        }else{
            float f = 1.0;
            if ((startDarkTime <= completeDarkTime) && (hh >= startDarkTime && hh < completeDarkTime)) {
                int totalDarkTime = (completeDarkTime - startDarkTime) * 60;
                int dh = hh - startDarkTime;
                int totalTime = dh * 60 + mm;
                f = totalTime * 1.0 / totalDarkTime;
            } else if ((startDarkTime > completeDarkTime) && (hh >= startDarkTime || hh < completeDarkTime)) {
                int totalDarkTime = (24 - startDarkTime + completeDarkTime) * 60;
                int dh = hh - startDarkTime;
                if (dh < 0)
                {
                    dh += 24;
                }
                int totalTime = dh * 60 + mm;
                f = totalTime * 1.0 / totalDarkTime;
            } else if ((startLightTime <= completeLightTime) && (hh >= startLightTime && hh < completeLightTime)) {
                int totalLightTime = (completeLightTime - startLightTime) * 60;
                int dh = completeLightTime - hh;
                int totalTime = dh * 60 - mm;
                f = totalTime * 1.0 / totalLightTime;
            } else if ((startLightTime > completeLightTime) && (hh >= startLightTime || hh < completeLightTime)) {
                int totalLightTime = (completeLightTime - startLightTime + 24) * 60;
                int dh = completeLightTime - hh;
                if (dh < 0) {
                    dh += 24;
                }
                int totalTime = dh * 60 - mm;
                f = totalTime * 1.0 / totalLightTime;
            }
            f = MAX(0.0, f);
            f = MIN(1.0, f);
            opacity1 = f * maxOpacity1;
            opacity2 = f * maxOpacity2;
            opacity3 = f * maxOpacity3;
            addTorchFlag = true;
        }
        
        if(m_nightLayer->getChildByTag(layerColorTag) == NULL){
            blacklayer = CCLayerColor::create(ccc4(0,0,0,255), layerSize.width, layerSize.height);
            blacklayer->setTag(layerColorTag);
            m_nightLayer->addChild(blacklayer);
        }else{
            blacklayer = dynamic_cast<CCLayerColor*>(m_nightLayer->getChildByTag(layerColorTag));
        }
        blacklayer->setOpacity(opacity1);
        
        CCScale9Sprite *blueSpr = NULL;
        if(m_nightLayer->getChildByTag(blueColorTag) == NULL){
            blueSpr = CCLoadSprite::createScale9Sprite("nightShadow.png");
            blueSpr->setAnchorPoint(ccp(0, 0));
            blueSpr->setPreferredSize(CCSize(layerSize.width, layerSize.height));
            blueSpr->setRotation(180);
            blueSpr->setPosition(ccp(layerSize.width, layerSize.height));
            blueSpr->setColor(ccc3(0, 55, 255));
            m_nightLayer->addChild(blueSpr);
            blueSpr->setTag(blueColorTag);
        }else{
            blueSpr = dynamic_cast<CCScale9Sprite*>(m_nightLayer->getChildByTag(blueColorTag));
        }
        blueSpr->setOpacity(opacity2);
        
        CCScale9Sprite *whiteSpr = NULL;
        if(m_nightLayer->getChildByTag(whiteColorTag) == NULL){
            whiteSpr = CCLoadSprite::createScale9Sprite("nightShadow.png");
            whiteSpr->setAnchorPoint(ccp(0, 0));
            whiteSpr->setPreferredSize(CCSize(layerSize.width, layerSize.height/4));
            whiteSpr->setRotation(180);
            whiteSpr->setPosition(ccp(layerSize.width, layerSize.height));
            whiteSpr->setColor(ccBLACK);
            whiteSpr->setTag(whiteColorTag);
            m_nightLayer->addChild(whiteSpr);
        }else{
            whiteSpr = dynamic_cast<CCScale9Sprite*>(m_nightLayer->getChildByTag(whiteColorTag));
        }
        whiteSpr->setOpacity(opacity3);
        m_isDay = false;
    }
    
    auto addTorch = [&](CCNode *parParent, CCNode * picParent){
        auto sprite = CCLoadSprite::createSprite("Brasier.png");
        picParent->addChild(sprite);
        sprite->setAnchorPoint(ccp(0.5, 0));
        sprite->setPosition(parParent->getPosition());

        string tmpStart = "UiFire_";
        int count = 4;
        for (int i=1; i<count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
            particle->setPosition(parParent->getPosition() + ccp(-5, 55));
            addCloudParticleToBatch(particle, true);
        }
    };

    // devil 关闭夜晚灯光和火把显示
    if(/* DISABLES CODE */ (false) && addTorchFlag){
        if(m_torchPar->count() == 0){
            addTorch(m_torchNode1, m_torchBatchNode);
            addTorch(m_torchNode2, m_torchBatchNode);
            addTorch(m_torchNode3, m_torchBatchNode);
            addTorch(m_torchNode4, m_torchBatchNode);
            addTorch(m_torchNode5, m_torchBatchNode);
            addTorch(m_torchNode6, m_torchBatchNode);
            addTorch(m_torchNode7, m_torchBatchNode);
            addTorch(m_torchNode8, m_torchBatchNode);
        }
        addNightLight();
    }else{
        CCObject *obj = NULL;
        CCARRAY_FOREACH(m_torchPar, obj){
            auto node = dynamic_cast<CCNode*>(obj);
            if(node){
                node->removeFromParent();
            }
        }
        m_torchPar->removeAllObjects();
        m_torchBatchNode->removeAllChildren();
        removeAllNightLight();
    }
}

void ImperialScene::checkDayTime(bool isInit) {
    struct tm tm;
    char s[100];
    
    char d[100];
    time_t timerG = GlobalData::shared()->getTimeStamp();
    tm = *localtime(&timerG);
    strftime(d, sizeof(d), "%d", &tm);
    int day = atoi(d);
    if (ChristmasActController::getInstance()->m_Today!=0 && ChristmasActController::getInstance()->m_Today!=day) {
        ChristmasActController::getInstance()->m_isNewDay=true;
    }
    ChristmasActController::getInstance()->m_Today = day;
    if (PortActController::getInstance()->m_Today != 0 && PortActController::getInstance()->m_Today != day) {
        PortActController::getInstance()->m_isNewDay = true;
    }
    PortActController::getInstance()->m_Today = day;
    
    time_t timer = time(0);
    tm = *localtime(&timer);
    strftime(s, sizeof(s), "%H", &tm);
    int hh = atoi(s);
    if ((startDarkTime <= completeLightTime && hh >= startDarkTime && hh < completeLightTime) || (startDarkTime > completeLightTime && ((hh >= startDarkTime && hh <= 23) || hh < completeLightTime))) {
        if (m_isDay) {
            m_sunNode->removeAllChildren();
            this->unschedule(schedule_selector(ImperialScene::setSunAngle));
        }
        m_isDay = false;
    }else{
        if (m_sysTime>60) {
            if (!m_isDay) {
                m_isDay = true;
                showSunshine();
            }
        }
        m_isDay = true;
    }
}

void ImperialScene::showSunshine()
{
    if (!m_isDay || m_isRain) {
        return;
    }
    m_sunNode->removeAllChildren();
    CCBLoadFile("sunshine_01",m_sunNode,this);
    
    this->unschedule(schedule_selector(ImperialScene::setSunAngle));
    this->schedule(schedule_selector(ImperialScene::setSunAngle), 1.0/24);
}

void ImperialScene::setSunAngle(float _time) {
    if (!m_isDay) {
        return;
    }
    auto cornerPos = onGetSceenPt(3590, 2250);//右上角顶点在屏幕中的位置
    float dx = cornerPos.x - CCDirector::sharedDirector()->getWinSize().width * 0.5;
    float dy = cornerPos.y;
    auto cornerAngle = atan2(dx, dy) * 180 / PI;
    auto maxcornerAngle = atan2(3565 - CCDirector::sharedDirector()->getWinSize().width * 0.5, CCDirector::sharedDirector()->getWinSize().height) * 180 / PI;
    auto mincornerAngle = atan2(CCDirector::sharedDirector()->getWinSize().width * 0.5, 2190) * 180 / PI;
    auto angle = (cornerAngle - mincornerAngle) / (maxcornerAngle - mincornerAngle) * 5;
//    auto sunOrgPos = Vec2(Director::getInstance()->getWinSize().width + 50, Director::getInstance()->getWinSize().height - 50);
//    auto detlaX = (sunOrgPos.x - cornerPos.x * 0.07);
//    auto detlaY = (sunOrgPos.y - cornerPos.y * 0.05);
//    CCLOG("setSunAngle detlaY,detlaY: %f,%f", detlaX, detlaY);
//    m_sunNode->setPositionX(detlaX);
    m_sunNode->setRotation(angle);
}

void ImperialScene::clearDrop(float _time){
    m_isRain = false;
    if (m_dropNode) {
        m_dropNode->removeAllChildren();
    }
}

void ImperialScene::moveMapToPosition(CCObject* obj){
    auto dic = _dict(obj);
    if(dic){
        float time = dic->valueForKey("time")->floatValue();
        float posx = dic->valueForKey("x")->floatValue();
        float posy = dic->valueForKey("y")->floatValue();
        float scale = dic->valueForKey("scale")->floatValue();
        onMoveToPos(posx, posy, TYPE_POS_MID, time, scale, true);
    }
}

void ImperialScene::guideEnd(CCObject* obj)
{
    PopupBaseView* curView = PopupViewController::getInstance()->getCurrentPopupView();
    if (curView) {
        ToolCreateView* toolCreateView = dynamic_cast<ToolCreateView*>(curView);
        if (toolCreateView) {
            return;
        }
    }
    
    if (guideTypeNew==1) {//跳转到 m_newRDBuild
//        onMoveToSpeBuildAndPlay(SPE_BUILD_NEWED);
//        return;
    }
    
    m_curGuideEnd = true;
    scheduleOnce(schedule_selector(ImperialScene::retGuideEnd), 0.5f);
    scheduleOnce(schedule_selector(ImperialScene::clearGuideState), 2.0f);
}
void ImperialScene::retGuideEnd(float _time)
{
    auto size = CCDirector::sharedDirector()->getWinSize();
    auto tmp2Pt = m_touchLayer->convertToNodeSpace(ccp(size.width/2, size.height/2));
    float _scale = 0.8;
    if (CCCommonUtils::isIosAndroidPad())
    {
        _scale = 1.3;
    }
    onMoveToPos(tmp2Pt.x, tmp2Pt.y, TYPE_POS_MID, 0.5, _scale, true);
}
void ImperialScene::clearGuideState(float _time)
{
    m_curGuideEnd = false;
}

void ImperialScene::onExit()
{
    m_exit = true;
    if(GlobalData::shared()->isUiInti){
        UIComponent::getInstance()->updateBuildState(false);
        //        UIComponent::getInstance()->onDeleteCropSceneUI();
    }
    
    FunBuildController::getInstance()->oldScale = m_touchLayer->getScale();
    FunBuildController::getInstance()->oldx = m_touchLayer->getPositionX();
    FunBuildController::getInstance()->oldy = m_touchLayer->getPositionY();
    setTouchEnabled(false);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ImperialScene::onEnterFrame), this);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ImperialScene::onShowFeedbackAnim), this);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ImperialScene::onShowFeedbackAgainAnim), this);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ImperialScene::onShowFeedbackAgainLaterAnim), this);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FUNBUILD_OPEN);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MAINSCENCE_GOBACK);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MAINSCENCE_SAVEPOS);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MAINSCENCE_REMOVE_TMPBUILD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_UNLOCK_TILE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MOVE_TO_POSITION);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TROOPS_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GUIDE_END);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SCIENCE_POWER_ADD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, QUEST_STATE_UPDATE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_INIT_ACTIVITY_EVENT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TALK_OPEN);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SHANGE_SCENCE_SHOW_HARVEST);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FINISH_MOVE_BUILD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_NEW_TIME_RWD_END);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_STOP_CITY_MOVE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GUIDE_BATTLE_END);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "showFirework");
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_OPENQIJI);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ShowPrincess);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FLYBALL_EFFECT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REFRESH_BUILD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GUIDE_BEGIN3);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_IMPERIAL_SCENE_TIPSON);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_IMPERIAL_SCENE_TIPSOFF);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PrincessRwd);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PrincessRwdNull);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GUIDE_BEGIN4);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GUIDE_BEGIN5);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_NEWRWD_GUIDE_END);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SQERSOLDIER_ANIFINISH);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GUIDE_MAINCITY_BUFF);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GUIDE_MAINCITY_SKIN);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_VIEWPORT_SHAKE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ReturnFrom_ChatOrMail);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_GUIDE_SHOW_FREE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_STAMINE_ADD_EFF);
    if (m_praticle) {
        m_praticle->stopAllActions();
        this->removeChild(m_praticle);
    }
    m_praticle = NULL;
    m_feedBackHead = NULL;
    m_feedlabel = NULL;
    for (int i=0; i<m_personArray->count(); i++) {
        Person* person = dynamic_cast<Person*>(m_personArray->objectAtIndex(i));
        if(person) {
            person->onPersonDelete();
        }
    }
    m_personArray->removeAllObjects();
    for (int i=0; i<m_soldierArray->count(); i++) {
        BattleSoldier2* soldier = dynamic_cast<BattleSoldier2*>(m_soldierArray->objectAtIndex(i));
        if (soldier) {
            soldier->removeObserver(1);//1为校场士兵的监听type
        }
    }
    m_soldierArray->removeAllObjects();
    for (int i=BUILD_COUNT-1; i>=0; i--) {
        FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[i]->getChildByTag(i));
        if (build) {
            build->onBuildDelete();
        }
        if(m_nodeBuildings[i]) {
            m_nodeBuildings[i]->removeAllChildren();
            m_nodeBuildings[i]->release();
        }
       
    }
    m_buildItems.clear();
    
    if (m_flagBuild) {
        m_flagBuild->onBuildDelete();
        m_flagBuild->removeFromParent();
    }
    
    if (m_hdBuild) {
        m_hdBuild->onBuildDelete();
        m_hdBuild->removeFromParent();
    }
    
    if (m_goldMineBuild) {
        m_goldMineBuild->onBuildDelete();
        m_goldMineBuild->removeFromParent();
    }
    
    if (m_goldBoxBuild) {
        m_goldBoxBuild->onBuildDelete();
        m_goldBoxBuild->removeFromParent();
    }
    
    if (m_chrTreeBuild) {
        m_chrTreeBuild->onBuildDelete();
        m_chrTreeBuild->removeFromParent();
    }
    if(m_merchanteBuild){
        m_merchanteBuild->onBuildDelete();
        m_merchanteBuild->removeFromParent();
    }
    if (m_cargoBuild) {
        m_cargoBuild->onBuildDelete();
        m_cargoBuild->removeFromParent();
    }
    if (m_xiongdiBuild) {
        m_xiongdiBuild->onBuildDelete();
        m_xiongdiBuild->removeFromParent();
    }
    if (m_qijiBuild) {
        m_qijiBuild->onBuildDelete();
        m_qijiBuild->removeFromParent();
    }
    if (m_newRDBuild) {
        m_newRDBuild->onBuildDelete();
        m_newRDBuild->removeFromParent();
    }
    if (m_ziyuanmenBuild) {
        m_ziyuanmenBuild->onBuildDelete();
        m_ziyuanmenBuild->removeFromParent();
    }
    
    if (m_lotteryBuild)
    {
        m_lotteryBuild->onBuildDelete();
        m_lotteryBuild->removeFromParent();
    }
    
    if (m_shipBuild) {
        m_shipBuild->onBuildDelete();
        m_shipBuild->removeFromParent();
    }
    
    if (m_willOpenBuild) {
        m_willOpenBuild->onBuildDelete();
        m_willOpenBuild->removeFromParent();
    }
    
    if (m_mailBuild) {
        m_mailBuild->onBuildDelete();
        m_mailBuild->removeFromParent();
    }
    if (m_dragonBuild) {
        m_dragonBuild->onBuildDelete();
        m_dragonBuild->removeFromParent();
    }
    if (m_nameLayer) {
        m_nameLayer->removeAllChildren();
    }
    if (m_popLayer) {
        m_popLayer->removeAllChildren();
    }
    if (m_signLayer) {
        m_signLayer->removeAllChildren();
    }
    if (m_spineLayer) {
        m_spineLayer->removeAllChildren();
    }
    if (m_resbatchNode) {
        m_resbatchNode->removeAllChildren();
    }
    if(m_rescustombatchNode){
        m_rescustombatchNode->removeAllChildren();
    }
    if (m_chrTreeBatchNode) {
        m_chrTreeBatchNode->removeAllChildren();
    }
    if (m_arrbatchNode) {
        m_arrbatchNode->removeAllChildren();
    }
    if (m_resBlentBatchNode) {
        m_resBlentBatchNode->removeAllChildren();
    }
    if (m_chrTreeBlentBatchNode) {
        m_chrTreeBlentBatchNode->removeAllChildren();
    }
    
    map<int,CCSpriteBatchNode*>::iterator it;
    for (it=m_buildBatchMap.begin(); it!=m_buildBatchMap.end(); it++) {
        if (it->second) {
            (it->second)->removeAllChildren();
        }
    }
    m_buildBatchMap.clear();
    
    m_buildingInitState = false;
    if (m_touchLayer) {
        m_touchLayer->removeAllChildren();
    }
    if (m_failEffectNode) {
        m_failEffectNode->removeAllChildrenWithCleanup(true);
    }
    IFSkeletonDataManager::shared()->purge();
    if(!mMaincitySkin.empty()){
        DynamicResourceController::getInstance()->loadNameTypeResource(mMaincitySkin, true);
        mMaincitySkin = "";
    }
    ToolController::getInstance()->setMerchante_state(0);
    
    if (m_dragonCaveGlow) {
        Sprite* sp1 = (Sprite*)m_dragonCaveGlow->getChildByTag(8888);
        if (sp1) {
            sp1->stopAllActions();
        }
        m_dragonCaveGlow->stopAllActions();
        m_dragonCaveGlow->removeAllChildren();
    }
    if (m_penquanRainbow) {
        m_penquanRainbow->removeAllChildren();
    }
    
    CCLayer::onExit();
}

void ImperialScene::onQijiBottomDelete(CCObject* obj){
    if (m_qijiBuild) {
        m_qijiBuild->onBuildDelete();
        m_qijiBuild->removeFromParent();
    }
    //新建奇迹建筑
    int pos = 55;
    int QijiID = FUN_BUILD_QIJI*1000+pos;
    onOpenBuildEnd(QijiID);
    onUpgradeNewBuild(QijiID);
    onResetBuild(QijiID);
}
void ImperialScene::onOpenNewBuild(CCObject* obj)
{
    map<int,FunBuildInfo>::iterator it;
    it = FunBuildController::getInstance()->curBuildsInfo->begin();
    for (; it != FunBuildController::getInstance()->curBuildsInfo->end(); it++)
    {
        if (m_buildItems.find(it->first) != m_buildItems.end()) {
            m_buildItems[it->first]->onOpenLock();
        }
    }
    
    refreashTile();
}

void ImperialScene::onGetMsgRefreshBuild(cocos2d::Ref *ref)
{
    if (!ref)
    {
        return;
    }
    __String* ccstr = dynamic_cast<__String*>(ref);
    string buildId = ccstr->getCString();
    onUpgradeNewBuild(atoi(buildId.c_str()));
}

void ImperialScene::onUpgradeNewBuild(int buildId)
{
    //    CCLOG("build: %d *********************", buildId);
    //    struct timeval  tm;
    //    unsigned long ms;
    //    gettimeofday(&tm, NULL);
    //    ms = (tm.tv_sec%1000) * 1000 + tm.tv_usec / 1000;
    //    CCLOG("start time: %ld", ms);
    
    int key = buildId;
    int pos =  buildId%1000  ;
    if( pos >= BUILD_COUNT)
        return;
    
    if (m_buildItems.find(key)!=m_buildItems.end()) {
        m_buildItems.erase(m_buildItems.find(key));
    }
    
    FunBuild* delbuild = dynamic_cast<FunBuild*>(m_nodeBuildings[pos]->getChildByTag(pos));
    if( delbuild )
    {
        delbuild->onBuildDelete();
        m_nodeBuildings[pos]->removeChild(delbuild);
    }
    
    FunBuild* build = FunBuild::create( key , m_nameLayer ,m_lvLayer);
    build->setTag(pos);
    m_nodeBuildings[pos]->addChild(build);
    m_nodeBuildings[pos]->setContentSize(CCSizeMake(build->mainWidth, build->mainHeight));
    m_buildItems[key] = build;
    
    CCSpriteBatchNode* curBatch = m_resbatchNode;
    CCSpriteBatchNode* curBlentBatch = m_resBlentBatchNode;
    if (m_buildBatchMap.find(pos) != m_buildBatchMap.end()) {
        curBatch = m_buildBatchMap[pos];
        curBlentBatch = NULL;
    }
    
    int od = m_nodeBuildings[pos]->getZOrder();
    build->m_key = 1000-od;
    build->setNamePos(m_nodeBuildings[pos]->getPositionX()
                      , m_nodeBuildings[pos]->getPositionY(), m_signLayer, m_popLayer, m_arrbatchNode, curBatch,m_resbatchNode,m_scaffoldsBatchNode, od, curBlentBatch);
    if (m_buildSpineMap.find(pos) != m_buildSpineMap.end()) {
        build->setSpineLayer( m_buildSpineMap[pos] ); //a by ljf
    }
    if (buildId == FUN_BUILD_WALL_ID){
        for(int i=0;i<3;i++)
        {
            if(m_wallBuilds[i])
            {
                m_wallNodes[i]->removeChild(m_wallBuilds[i]);
                m_wallBuilds[i]->onBuildDelete();
                m_wallBuilds[i] = NULL;
                m_wallBuilds[i] = WallBuild::create(i+1);
                m_wallBuilds[i]->setNamePos(m_wallNodes[i]->getPositionX(), m_wallNodes[i]->getPositionY(), m_signLayer,m_wallBatchs[i], m_wallBatchs[i],0);
                m_wallNodes[i]->addChild(m_wallBuilds[i]);
            }
        }
    }
}

Vec2 ImperialScene::getWorldPosition(int pos)
{
    if (pos >= BUILD_COUNT || m_nodeBuildings[pos] == nullptr || m_nodeBuildings[pos]->getParent() == nullptr)
    {
        return Vec2(Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height);
    }
    Vec2 p1 = m_nodeBuildings[pos]->convertToWorldSpace(Vec2(0, 0));
    p1 += Vec2(80, 60);
    return p1;
}

float ImperialScene::getTouchLayerScale(){
    return m_touchLayer->getScale();
}

void ImperialScene::onMoveToBuild(int itemId, bool st)
{
    if(itemId == 1){
        itemId = 0;
    }
    
    if(m_buildItems.size()<=0)
    {
        return;
    }
    float s = m_touchLayer->getScale();
    float endS = 0.8;
    if(CCCommonUtils::isIosAndroidPad())
    {
        endS = HD_SCALE;
    }
    if(itemId == FUN_BUILD_MAIN){
        m_touchLayer->setScale(endS);
    }else{
        endS = 1.0f;
        if(st && itemId != FUN_BUILD_BARRACK1 && itemId != FUN_BUILD_BARRACK2) {
            endS = 1.3f;
        }
        if (st && (itemId == FUN_BUILD_BARRACK1 || itemId == FUN_BUILD_BARRACK2)) {
            TargetPlatform target = CCApplication::sharedApplication()->getTargetPlatform();
            if(target == kTargetIpad || CCCommonUtils::isAdriodPad()){
                endS = 0.75f;
            }
            if (CCCommonUtils::isIosAndroidPad())
            {
                endS = HD_SCALE;
            }
        }
        m_touchLayer->setScale(endS);
    }
    int id = FunBuildController::getInstance()->getMaxLvBuildByType(itemId);
    
    map<int, CCSafeNode<FunBuild>>::iterator it = m_buildItems.find(id);
    float f = m_touchLayer->getScale();
    cocos2d::CCPoint anchor = ccp(m_touchLayer->getContentSize().width * m_touchLayer->getAnchorPoint().x,
                                  m_touchLayer->getContentSize().height * m_touchLayer->getAnchorPoint().y);
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    anchor = ccpMult(anchor, (1.0f - f));
    FunBuild *build = NULL;
    CCNode *node = NULL;
    m_isSave = false;
    if(it != m_buildItems.end()){
        build = m_buildItems[id];
        int buildPosX = m_buildItems[id]->getParent()->getPositionX() + m_buildItems[id]->getContentSize().width / 2 ;
        int buildPosY = m_buildItems[id]->getParent()->getPositionY() + m_buildItems[id]->getContentSize().height / 2;
        m_viewPort->updatePosition(ccp(winSize.width / 2 - buildPosX * f - anchor.x, winSize.height / 2 - buildPosY * f - anchor.y), true);
    }
    if(itemId < BUILD_COUNT - 1){
        node = m_nodeBuildings[itemId];
        if (!node->getChildren().empty()) {
            build = dynamic_cast<FunBuild*>(node->getChildren().at(0));
            int buildPosX = m_nodeBuildings[itemId]->getPositionX() + build->mainWidth / 2 ;
            int buildPosY = m_nodeBuildings[itemId]->getPositionY() + build->mainHeight / 2;
            m_viewPort->updatePosition(ccp(winSize.width / 2 - buildPosX * f - anchor.x, winSize.height / 2 - buildPosY * f - anchor.y), true);
        }
    }
    m_touchLayer->setScale(s);
    m_touchLayer->runAction(CCScaleTo::create(1.0, endS));
    if(build){
        build->playShadow();
    }
}

void ImperialScene::onMoveToSpeBuildAndPlay(int itemId, bool st)
{
    SpeBuild* speBuild = NULL;
    switch (itemId)
    {
        case SPE_BUILD_SHIP:
            speBuild = m_shipBuild;
            break;
        
        case SPE_BUILD_HD:
            speBuild = m_hdBuild;
            break;
            
        case SPE_BUILD_GOLD:
            speBuild = m_goldMineBuild;
            break;
            
        case SPE_BUILD_GOLD_BOX:
            speBuild = m_goldBoxBuild;
            break;
        
        case SPE_BUILD_CHR_TREE:
            speBuild = m_chrTreeBuild;
            break;
        
        case SPE_BUILD_WILL_OPEN:
            speBuild = m_willOpenBuild;
            break;
            
        case SPE_BUILD_MERCHANTE:
            speBuild = m_merchanteBuild;
            break;
            
        case SPE_BUILD_CARGO:
            speBuild = m_cargoBuild;
            break;
        case SPE_BUILD_XIONGDI:
            speBuild = m_xiongdiBuild;
            break;
        case SPE_BUILD_QIJI:
            speBuild = m_qijiBuild;
            break;
        case SPE_BUILD_NEWED:
            speBuild = m_newRDBuild;
            break;
        case SPE_BUILD_DRAGON:
            speBuild = m_dragonBuild;
            break;
        case SPE_BUILD_ZIYUANMEN:
            speBuild = m_ziyuanmenBuild;
            break;
        default:
            break;
    }
    if (speBuild == NULL)
    {
        if (GuideController::share()->getCurGuideID() == "3130300") {
            GuideController::share()->setGuideEnd();
        }
        return;
    }
    int buildPosX = speBuild->getParent()->getPositionX() + speBuild->mainWidth / 2;
    int buildPosY = speBuild->getParent()->getPositionY() + speBuild->mainHeight / 2;
    
    float endS = 1.3f;
    onMoveToPos(buildPosX, buildPosY, TYPE_POS_CLICK, 1.0f, endS, true);
    
    if (!GuideController::share()->isInTutorial()) {
        m_flyArrow->setPosition(ccp(buildPosX, buildPosY));
        m_flyArrow->setVisible(true);
        scheduleOnce(schedule_selector(ImperialScene::hideFlyArrow), 5.0f);
    }
    
    m_forceMove = st;
}

void ImperialScene::onMoveToBuildAndPlay(int itemId, bool st, bool train)
{
    FunBuild *build = NULL;
    if(itemId>1000){
        map<int, CCSafeNode<FunBuild>>::iterator it = m_buildItems.find(itemId);
        if( it == m_buildItems.end()){
            return;
        }
//        m_touchLayer->setScale(m_oldScale);
        m_viewPort->updatePosition(m_touchLayer->getPosition(), false, 0, true);
        if (train == true) {
            onMoveToBuildAndOpen(itemId, TYPE_POS_CLICK, 0.0, true);
        }
        else
            onMoveToBuildAndOpen(itemId, TYPE_POS_CLICK, 1.0, true);
        build = m_buildItems[itemId];

    }else{
//        m_touchLayer->setScale(m_oldScale);
        m_viewPort->updatePosition(m_touchLayer->getPosition(), false, 0, true);
        onMoveToBuildAndOpen(itemId, TYPE_POS_CLICK, 1.0, true);
        CCNode *node = m_nodeBuildings[itemId];
        if (!node->getChildren().empty()) {
            build = dynamic_cast<FunBuild*>(node->getChildren().at(0));
        }
    }
    int buildPosX = build->getParent()->getPositionX() + build->mainWidth / 2;
    int buildPosY = build->getParent()->getPositionY() + build->mainHeight / 2;
    if (itemId == FUN_BUILD_MAIN_CITY_ID) {
        buildPosX += 100;
        buildPosY -= 30;
    }
    m_flyArrow->setPosition(ccp(buildPosX, buildPosY));
    m_flyArrow->setVisible(true);
    
    m_forceMove = st;
    scheduleOnce(schedule_selector(ImperialScene::hideFlyArrow), 5.0f);
}
void ImperialScene::hideFlyArrow(float _time){
    m_forceMove = false;
    m_flyArrow->stopAllActions();
    m_flyArrow->setVisible(false);
}
void ImperialScene::setPointArrowAni(int buildId)
{
    FunBuild *build = NULL;
    if(buildId>1000){
        map<int, CCSafeNode<FunBuild>>::iterator it = m_buildItems.find(buildId);
        if( it == m_buildItems.end()){
            return;
        }
        build = m_buildItems[buildId];
        
    }else
        return;
    
    int buildPosX = build->getParent()->getPositionX() + build->mainWidth / 2;
    int buildPosY = build->getParent()->getPositionY() + build->mainHeight / 2;

    m_flyArrow->setPosition(ccp(buildPosX, buildPosY + 80));
    m_flyArrow->setVisible(true);
    
    scheduleOnce(schedule_selector(ImperialScene::hideFlyArrow), 5.0f);

}
void ImperialScene::onMoveToBuildAndOpen(int itemId, int type, float dt, bool bound, bool up)
{
    float endS = 1;
    if (CCCommonUtils::isIosAndroidPad())
    {
        endS = 2.4;
    }
    if(itemId>1000){
        int buildType = itemId/1000;
        if (buildType == FUN_BUILD_MAIN || buildType == FUN_BUILD_WALL || buildType == FUN_BUILD_WATCH_TOWER || buildType == FUN_BUILD_PRISON) {
            endS = 0.8f;
            if (CCCommonUtils::isIosAndroidPad())
            {
                endS = 1.5;
            }
        }else if(buildType == FUN_BUILD_QIJI) {   //奇迹建筑靠近图像边缘，所以作了倍数放大的特殊处理
            endS =1.2f;
            if (CCCommonUtils::isIosAndroidPad())
            {
                endS = 2.7;
            }
        }else{
            NULL;
        }
    }
//    endS = m_touchLayer->getScale();///移动到建筑上时不进行镜头缩放
    FunBuild *build = NULL;
    if(1){
        if (itemId>1000) {
            map<int, CCSafeNode<FunBuild>>::iterator it = m_buildItems.find(itemId);
            if (it != m_buildItems.end()) {
                build = m_buildItems[itemId];
            }else {
                return;
            }
        }
        else {
            CCNode *node = m_nodeBuildings[itemId];
            if (!node->getChildren().empty()) {
                build = dynamic_cast<FunBuild*>(node->getChildren().at(0));
            }
        }
        if(build == NULL || build->getParent() == NULL){
            return;
        }
        if(up == true){
            m_curBuildPosx = build->getParent()->getPositionX() + build->mainWidth*2/3 ;
            m_curBuildPosy = build->getParent()->getPositionY() + build->mainHeight/2;
        }else{
            m_curBuildPosx = build->getParent()->getPositionX() + build->mainWidth/2 ;
            m_curBuildPosy = build->getParent()->getPositionY() + build->mainHeight/2;
        }
        if (CCCommonUtils::isIosAndroidPad())
        {
            if(up == true){
                 m_curBuildPosy = build->getParent()->getPositionY() + build->mainHeight/2;
            }else{
//                m_curBuildPosy = build->getParent()->getPositionY() + build->mainHeight;
                 m_curBuildPosy = build->getParent()->getPositionY() + build->mainHeight/2;
            }
        }
        onMoveToPos(m_curBuildPosx, m_curBuildPosy, type, dt, endS, bound);
    }
}

void ImperialScene::onMoveToPos(float x, float y, int type, float dt, float scale, bool bound)
{
    float s = m_touchLayer->getScale();

    float _wf = 0.31;
    float _hf = 0.55;
    if (type == TYPE_POS_UP) {
        _wf = 0.31;
        _hf = 0.63;
    }
    else if (type == TYPE_POS_MID) {
        _wf = 0.5;
        _hf = 0.5;
    }
    
    float f = scale;
    if (type == TYPE_POS_CLICK) {
        _wf = 0.5;
        _hf = 0.5;
        f = m_touchLayer->getScale();
        if(y<1000 && bound==false) {
            bound = false;
        }
        else {
            bound = true;
        }
    }
    else {
        m_touchLayer->setScale(scale);
        
    }
    
    if (m_curGuideEnd) {
        f = 0.8;
        m_touchLayer->setScale(0.8);
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_touchLayer->setScale(HD_SCALE);
            f = HD_SCALE;
        }
        
    }
    
    cocos2d::CCPoint anchor = ccp(m_touchLayer->getContentSize().width * m_touchLayer->getAnchorPoint().x,
                                  m_touchLayer->getContentSize().height * m_touchLayer->getAnchorPoint().y);
    
    anchor = ccpMult(anchor, (1.0f - f));
    m_curBuildPosx = x;
    m_curBuildPosy = y;
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    m_tmpMoveX = winSize.width*_wf-m_curBuildPosx*f-anchor.x;
    m_tmpMoveY = winSize.height*_hf-m_curBuildPosy*f-anchor.y;
    
    if (dt<=0) {
        m_viewPort->updatePosition(ccp(m_tmpMoveX, m_tmpMoveY), false, dt, bound);
    }
    else {
//        if(m_viewPort->notMove) {
//            return;
//        }
        setUnMoveScence(true);
        m_viewPort->updatePosition(ccp(m_tmpMoveX, m_tmpMoveY), true, dt, bound);
        m_touchLayer->setScale(s);
        m_touchLayer->runAction(CCScaleTo::create(dt, f));
        scheduleOnce(schedule_selector(ImperialScene::onEnableMoveScence), dt+0.1f);
    }
}

void ImperialScene::onEnableMoveScence(float _time)
{
    setUnMoveScence(false);
}

CCPoint ImperialScene::onGetSceenPt(float x, float y)
{
    cocos2d::CCPoint anchor = ccp(m_touchLayer->getContentSize().width * m_touchLayer->getAnchorPoint().x,
                                  m_touchLayer->getContentSize().height * m_touchLayer->getAnchorPoint().y);
    float f = m_touchLayer->getScale();
    anchor = ccpMult(anchor, (1.0f - f));
    auto tPt = m_touchLayer->getPosition();
    return ccp(x, y)*f + anchor +tPt;
}

float ImperialScene::onGetTouchLayerScale()
{
    return m_touchLayer->getScale();
}

void ImperialScene::onMoveToPosDir(float sx, float sy, float ex, float ey, float dt, bool bound)
{
    if(m_viewPort->notMove) {
        return;
    }
    float f = m_touchLayer->getScale();
    cocos2d::CCPoint anchor = ccp(m_touchLayer->getContentSize().width * m_touchLayer->getAnchorPoint().x,
                                  m_touchLayer->getContentSize().height * m_touchLayer->getAnchorPoint().y);
    
    anchor = ccpMult(anchor, (1.0f - f));
    m_curBuildPosx = sx;
    m_curBuildPosy = sy;
    m_tmpMoveX = ex-m_curBuildPosx*f-anchor.x;
    m_tmpMoveY = ey-m_curBuildPosy*f-anchor.y;
    if (dt<=0) {
        m_viewPort->updatePosition(ccp(m_tmpMoveX, m_tmpMoveY), false, dt, bound);
    }
    else {
        setUnMoveScence(true);
        m_viewPort->updatePosition(ccp(m_tmpMoveX, m_tmpMoveY), true, dt, bound);
        scheduleOnce(schedule_selector(ImperialScene::onEnableMoveScence), dt+0.1f);
    }
}

void ImperialScene::onSaveCurPos(CCObject* obj)
{
    m_isSave = true;
    m_oldScale = m_touchLayer->getScale();
    m_oldPosX = m_touchLayer->getPositionX();
    m_oldPosY = m_touchLayer->getPositionY();
}

void ImperialScene::setPxPy(float x,float y)
{
    m_oldPosX = x;
    m_oldPosY = y;
    
    if (CCCommonUtils::isIosAndroidPad()) {
        m_oldScale = 1.2;
    }else{
        m_oldScale = 0.8;
    }
}

void ImperialScene::onGoBackOldPos(CCObject* obj)
{
    lastTouchBuildId = -1;
    if (m_isSave) {
        if(m_flyArrow->isVisible() && m_forceMove){
            m_isSave = false;
            return;
        }
        setUnMoveScence(true);
        float s = m_touchLayer->getScale();
//        s= 2.4;
        m_touchLayer->setScale(m_oldScale);
        m_viewPort->updatePosition(ccp(m_oldPosX, m_oldPosY), true, 0.15);
        m_touchLayer->setScale(s);
        m_touchLayer->runAction(CCScaleTo::create(0.15, m_oldScale));
        m_isSave = false;
        scheduleOnce(schedule_selector(ImperialScene::onEnableMoveScence), 0.15+0.1f);
    }
}

void ImperialScene::onRemoveTmpBuild(CCObject* obj)
{
    hideTmpBuild(m_tmpBuildPos);
}

void ImperialScene::onTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
}

void ImperialScene::onTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
}

void ImperialScene::onTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
}

void ImperialScene::onTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
    ccTouchesEnded(pTouches,pEvent);
}

void ImperialScene::onSingleTouchBegin(CCTouch* pTouch)
{
    if (!m_canClick || m_battleLayer->getChildrenCount()>0) {
        Node* tmpNode = m_battleLayer->getChildByTag(9);
        if (tmpNode && tmpNode->isVisible()) {
            return;
        }
        if (!m_canClick) {
            return;
        }
    }
    int key = 99999;
    if (curTouchBuildId > -1 && curTouchBuildId < BUILD_COUNT ) {
        FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[curTouchBuildId]->getChildByTag(curTouchBuildId));
        if (build) {
            build->setWhite();
            build->stopShadow();
        }
    }
    curTouchBuildId = -1;
    
    m_beginTouchType = 0;
    
    if (m_beginTouchType==0 && m_talkACTCell && m_talkACTCell->isVisible() && m_talkACTCell->onTouchBegan(pTouch, NULL)) {
        m_beginTouchType = 1;
    }
    
    if (m_beginTouchType==0 && m_talkACTCell2 && m_talkACTCell2->isVisible() && m_talkACTCell2->onTouchBegan(pTouch, NULL)) {
        m_beginTouchType = 1;
    }
    
    if (m_beginTouchType==0 && m_princessNode && m_princessNode->isVisible() && isTouchInside(m_princessNode, pTouch)) {
        m_beginTouchType = 1;
    }
    
    if (m_beginTouchType==0 && m_tilebatchNode->getChildByTag(999888) && isTouchInside(m_tilebatchNode->getChildByTag(999888), pTouch)) {
        m_beginTouchType = 1;
    }
    
    if (m_beginTouchType==0) {
        for (int i=0; i<BUILD_COUNT; i++) {
            FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[i]->getChildByTag(i));
            if(build && build->onTouchBegan(pTouch, NULL))
            {
                if (build->m_key < key) {
                    key = build->m_key;
                    curTouchBuildId = i;
                }
            }
        }
    }
    
    if(curTouchBuildId == -1 || FunBuildController::getInstance()->OpenMoveBuildStat) {
        lastTouchBuildId = -1;
    }
}

void ImperialScene::unLockTouch(float _time) {
    m_canClick = true;
}

void ImperialScene::onSingleTouchEnd(CCTouch* pTouch)
{
    if (!m_canClick || m_battleLayer->getChildrenCount()>0) {
        Node* tmpNode = m_battleLayer->getChildByTag(9);
        if (tmpNode && tmpNode->isVisible()) {
            return;
        }
        if (!m_canClick) {
            return;
        }
    }
    m_canClick = false;
    this->scheduleOnce(schedule_selector(ImperialScene::unLockTouch), 0.5);
    
    m_buildBtnsView->onHide();
    m_speBuildBtnsView->onHide();
    if (curTouchBuildId > -1) {
        if(lastTouchBuildId != curTouchBuildId)
        {
            lastTouchBuildId = curTouchBuildId;
            FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[curTouchBuildId]->getChildByTag(curTouchBuildId));
            if (build) {
                if (m_singleTouchState) {
                    return;
                }
                m_singleTouchState = true;
                build->onTouchEnded(pTouch, NULL);
            }
        }
        else {
            lastTouchBuildId = -1;
        }
    }
    else {
        bool st = true;
        if (FunBuildController::getInstance()->OpenMoveBuildStat) {
            st = false;
        }
        
        if(st && m_beginTouchType==1 && m_talkACTCell && m_talkACTCell->isVisible() && m_talkACTCell->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_talkACTCell->onTouchEnded(pTouch, NULL);
        }
        if(st && m_beginTouchType==1 && m_talkACTCell2 && m_talkACTCell2->isVisible() && m_talkACTCell2->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_talkACTCell2->onTouchEnded(pTouch, NULL);
        }
        if (st) {
            map<int, CCSafeNode<SpeBuild>>::iterator it = m_bigTiles.begin();
            for (; it!=m_bigTiles.end(); it++) {
                if (it->second && it->second->onTouchBegan(pTouch, NULL)) {
                    it->second->onTouchEnded(pTouch, NULL);
                    st = false;
                    break;
                }
            }
        }
        if (st && m_flagBuild && m_flagBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_flagBuild->onTouchEnded(pTouch, NULL);
        }
        if (st && m_hdBuild && m_hdBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_hdBuild->onTouchEnded(pTouch, NULL);
        }
        if (st && m_goldMineBuild && m_goldMineBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_goldMineBuild->onTouchEnded(pTouch, NULL);
        }
        if (st && m_goldBoxBuild && m_goldBoxBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_goldBoxBuild->onTouchEnded(pTouch, NULL);
        }
        if (st && m_chrTreeBuild && m_chrTreeBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_chrTreeBuild->onTouchEnded(pTouch, NULL);
        }
        if (st && m_shipBuild && m_shipBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_shipBuild->onTouchEnded(pTouch, NULL);
        }
        if (st && m_merchanteBuild && m_merchanteBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_merchanteBuild->onTouchEnded(pTouch, NULL);
        }
        if (st && m_cargoBuild && m_cargoBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_cargoBuild->onTouchEnded(pTouch, NULL);
        }
        if (st && m_xiongdiBuild && m_xiongdiBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_xiongdiBuild->onTouchEnded(pTouch, NULL);
        }
        if (st && m_qijiBuild && m_qijiBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_qijiBuild->onTouchEnded(pTouch, NULL);
        }
        if (st && m_newRDBuild && m_newRDBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_newRDBuild->onTouchEnded(pTouch, NULL);
        }
        if (st && m_lotteryBuild && m_lotteryBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_lotteryBuild->onTouchEnded(pTouch, NULL);
        }
        if (st && m_willOpenBuild && m_willOpenBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_willOpenBuild->onTouchEnded(pTouch, NULL);
        }
        if (st && m_ziyuanmenBuild && m_ziyuanmenBuild->onTouchBegan(pTouch, nullptr)) {
            st = false;
            m_ziyuanmenBuild->onTouchEnded(pTouch, nullptr);
        }
        if (st && (isTouchInside(m_desNode1, pTouch) || (m_HTPHead && m_HTPHead->isVisible() && isTouchInside(m_HTPHead, pTouch)))) {//help
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            st = false;
            if ( m_mailBuild ) {
                m_mailBuild->getAnimationManager()->runAnimationsForSequenceNamed("Default");
            }
            CCCommonUtils::showHelpShiftFAQ();
//                config->writeToFile("/Users/zhangjun/Desktop/dictf/config_dt");//test
//                PopupViewController::getInstance()->addPopupInView(SuggestionView::create());
        }
        if (st && isTouchInside(m_mcNode4, pTouch) && false) {//HOW TO PLAY
            st = false;
            HowToPlayView * popUp = HowToPlayView::create();
            PopupViewController::getInstance()->addPopupView(popUp,true);
        }
        if (st && m_dragonBuild && m_dragonBuild->onTouchBegan(pTouch, NULL)) {
            st = false;
            m_dragonBuild->onTouchEnded(pTouch, NULL);
        }
        if (st && m_wallBuilds[0] && m_wallBuilds[0]->onTouchBegan(pTouch, NULL)) {
            st = false;
            curTouchBuildId = FUN_BUILD_WALL_ID%100;
            if (lastTouchBuildId != curTouchBuildId) {
                lastTouchBuildId = curTouchBuildId;
                FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[curTouchBuildId]->getChildByTag(curTouchBuildId));
                if (build) {
                    if (m_singleTouchState) {
                        return;
                    }
                    m_singleTouchState = true;
                    build->onTouchEnded(pTouch, NULL);
                }
            }
            else {
                lastTouchBuildId = -1;
            }
        }
        if (st && m_beginTouchType==1 && isTouchInside(m_princessNode, pTouch) && m_princessNode->isVisible()) {
            st = false;
            FunBuildController::getInstance()->TalkViewBId = "";
            m_princessNode->setVisible(false);
            m_talkTime = 0;
            if (FunBuildController::getInstance()->TalkViewContent!="" && princessNodeOnce == false) {
                PopupViewController::getInstance()->setPrincessShow(true);
                
                PrincessRewardCommand * cmd = new PrincessRewardCommand();
                cmd->sendAndRelease();
            }
            if (princessNodeOnce == true) {
                vector<string> tmpTalkInfos;
                string loadingTip1 = "";
                if (FunBuildController::getInstance()->talkTipsVec0.size() > 0) {
                    int rIdx = CCMathUtils::getRandomInt(0, FunBuildController::getInstance()->talkTipsVec0.size()-1);
                    string dialog = CCCommonUtils::getPropById(FunBuildController::getInstance()->talkTipsVec0[rIdx], "content");
                    loadingTip1 = _lang(dialog);
                }
                if (loadingTip1==""){
                    loadingTip1 = _lang("3140012");
                }

                tmpTalkInfos.push_back(loadingTip1);
                PopupViewController::getInstance()->addPopupView(NpcTalkView::create(tmpTalkInfos));
                princessNodeOnce = false;
            }
        }
        if (st && ((m_princess1Node && isTouchInside(m_princess1Node, pTouch)) || (m_princess2Node && isTouchInside(m_princess2Node, pTouch)))) {
            st = false;
            if (FunBuildController::getInstance()->LoadingTips.size() > 0) {
                int index = rand()%FunBuildController::getInstance()->LoadingTips.size();
                string loadingTip = _lang(FunBuildController::getInstance()->LoadingTips[index].c_str());
                string loadingTip1 = "";
                if (FunBuildController::getInstance()->talkTipsVec0.size() > 0) {
                    int rIdx = CCMathUtils::getRandomInt(0, FunBuildController::getInstance()->talkTipsVec0.size()-1);
                    string dialog = CCCommonUtils::getPropById(FunBuildController::getInstance()->talkTipsVec0[rIdx], "content");
                    loadingTip1 = _lang(dialog);
                }
                if (loadingTip1==""){
                    loadingTip1 = _lang("3140012");
                }
                if (loadingTip!="") {
                    vector<string> tmpTalkInfos;
                    tmpTalkInfos.push_back(loadingTip1);
                    tmpTalkInfos.push_back(loadingTip);
                    PopupViewController::getInstance()->addPopupView(NpcTalkView::create(tmpTalkInfos));
                }
            }
        }
        if (st && (m_princess3Node && isTouchInside(m_princess3Node, pTouch)) && m_princessAniEnd) {
            st = false;
            m_princessAniEnd = false;
            int type = rand()%2;
            if (type == 0) {
                princessShotAni(0);
            }
            else {
                princessShotAni(1);
            }
        }
        if (st && isTouchInside(m_FountainTouchNode, pTouch)) { // && m_fountainNode->getChildrenCount()<=0
            st = false;
            playFountain();
        }
        if (st && m_tilebatchNode->getChildByTag(999888) && isTouchInside(m_tilebatchNode->getChildByTag(999888), pTouch)) {
            bool noQuest = true;
            map<std::string, PrincessQuestInfo*>::iterator it = PrincessQuestController::getInstance()->currentAchList.begin();
            for (; it != PrincessQuestController::getInstance()->currentAchList.end(); it++) {
                if (it->second->main == 1) {
                    noQuest = false;
                    break;
                }
            }
            if (noQuest == false) {
                st = false;
                PopupViewController::getInstance()->addPopupInView(PrincessQuestView::create());
            }
            
        }
        if (st && m_sSqerTouchArr->count() > 0 && m_sSqerTouchNode) {
            for (int i = 0; i < m_sSqerTouchArr->count(); i++) {
                Node* touchNode = dynamic_cast<Node*>(m_sSqerTouchArr->objectAtIndex(i));
                if (isTouchInside(touchNode, pTouch))
                {
                    st = false;
                    Point fromPt = onGetSceenPt(touchNode->getPositionX(), touchNode->getPositionY());
                    SqerSoldierRewardCommand * cmd = new SqerSoldierRewardCommand(fromPt);
                    cmd->sendAndRelease();
                    m_sSoldierClickTime++;
                    if (m_sSoldierClickTime >= 10) {
                        m_sSoldierClickTime = 0;
                        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SQERSOLDIER_ANIMATION, Integer::create(100));
                        SoundController::sharedSound()->playEffects(Music_Sfx_city_train);
                    }
                    else {
                        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SQERSOLDIER_ANIMATION, Integer::create(i));
                        SoundController::sharedSound()->playEffects(Music_Sfx_city_train);
                    }
                    Node* parNode = Node::create();
                    for (int i = 0; i < 2; i++) {
                        auto particle = ParticleController::createParticle(CCString::createWithFormat("ArmyResponse_%d",i)->getCString());
                        particle->setPosition(CCPointZero);
                        parNode->addChild(particle);
                    }
                    parNode->setTag(9999);
                    parNode->setScale(1.3);
                    parNode->setPosition(touchNode->getPosition() + ccp(5, -20));
                    m_sSqerTouchNode->addChild(parNode);
                    this->scheduleOnce(schedule_selector(ImperialScene::removeSqerTouchPar), 5.0);
                    break;
                }
            }
        }
        
        if(st) {
            auto pt = pTouch->getLocation();
            UIComponent::getInstance()->showClickEff(pt);
        }
        if (st && isTouchInside(m_crossClickNode, pTouch)) {
            st = false;
            ActivityController::getInstance()->openCrossServerActivity();
        }
    }
    
    scheduleOnce(schedule_selector(ImperialScene::canSingleTouch), 0.2f);
}

void ImperialScene::onResetLastBuildId()
{
    lastTouchBuildId = -1;
}

void ImperialScene::canSingleTouch(float _time){
    m_singleTouchState = false;
}
void ImperialScene::onShowFeedbackAgainAnim(float dt){
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ImperialScene::onShowFeedbackAgainAnim), this);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ImperialScene::onShowFeedbackAgainLaterAnim), this);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ImperialScene::onShowFeedbackAgainLaterAnim), this, 60,kCCRepeatForever, 0.0f, false);
    
    if(m_feedlabel!=NULL){
        m_feedlabel->setVisible(false);
    }
    if(m_feedBackHead){
        m_feedBackHead->setVisible(false);
    }
}
void ImperialScene::onShowFeedbackAgainLaterAnim(float dt){
    
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ImperialScene::onShowFeedbackAnim), this, 0.2,kCCRepeatForever, 0.0f, false);
}
void ImperialScene::onShowFeedbackAnim(float dt){
    string feedbak = "Help";
    int totallen = feedbak.length();
    if(m_feedlabel!=NULL){
        m_feedlabel->setVisible(true);
        if(m_feedBackHead){
            m_feedBackHead->setVisible(true);
        }
        if(m_noticeCount>0){
            m_icon_op_mail->setVisible(true);
            m_HTPlabel->setString("");
            return ;
        }else{
            m_icon_op_mail->setVisible(false);
        }
        string msg =  m_feedlabel->getString();
        int len = msg.length();
        if(len==totallen){
            msg =feedbak[0];
        }else{
            msg+=feedbak[len];
        }
        if(len==totallen-1){
            CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ImperialScene::onShowFeedbackAnim), this);
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ImperialScene::onShowFeedbackAgainAnim), this, 2,1, 0.0f, false);
        }
        m_feedlabel->setString(msg.c_str());
    }
}
void ImperialScene::onShowHTPAgainAnim(float dt){
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ImperialScene::onShowHTPAgainAnim), this);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ImperialScene::onShowHTPAgainLaterAnim), this);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ImperialScene::onShowHTPAgainLaterAnim), this, 10,kCCRepeatForever, 0.0f, false);
    
    if(m_HTPlabel!=NULL){
        m_HTPlabel->setVisible(false);
    }
    if(m_HTPHead){
        m_HTPHead->setVisible(false);
    }
    if(m_HTPHead1){
        m_HTPHead1->setVisible(false);
    }
    if(m_icon_op_mail){
        m_icon_op_mail->setVisible(false);
    }
}
void ImperialScene::onShowHTPAgainLaterAnim(float dt){
    
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ImperialScene::onShowHTPAnim), this, 0.2,kCCRepeatForever, 0.0f, false);
}
void ImperialScene::onShowHTPAnim(float dt){
    string HTP = _lang("101230")+_lang("101231");//todo:放到语言文件中

    int totallen = Utf8Utils::strlen(HTP.c_str());
    if(m_HTPlabel!=NULL){
        if(m_HTPHead){
            m_HTPHead->setVisible(true);
        }
        if(m_HTPHead1){
            m_HTPHead1->setVisible(true);
        }
        m_icon_op_mail->setVisible(m_noticeCount>0);
       // m_HTPlabel->setVisible(!m_icon_op_mail->isVisible());
        std::string language = LocalController::shared()->getLanguageFileName();
        m_HTPHead->setScaleX(-1.5);//-1.2
        if (language=="id"||language=="ru") {
            m_HTPHead->setScaleX(-1.6);
        }
        if (language=="zh_CN"||language=="zh_TW") {
            m_HTPHead->setScaleX(-0.8);
        }
        if(m_noticeCount>0){
            m_HTPHead->setScaleX(-0.9);
        }

        string msg =  m_HTPlabel->getString();

        int len = Utf8Utils::strlen(msg.c_str());

        if(len==totallen){
            msg =Utf8Utils::utf8sub(HTP.c_str(),0,1);
        }else{
            msg =Utf8Utils::utf8sub(HTP.c_str(),0,len+1);
        }
        if(len==totallen-1){
            CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ImperialScene::onShowHTPAnim), this);
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ImperialScene::onShowHTPAgainAnim), this, 2,1, 0.0f, false);
        }
        if (language=="ru") {
            return;
        }
//        if (msg=="") {
//            return;
//        }
        m_HTPlabel->setString(msg.c_str());
    }
}
void ImperialScene::onEnterFrame(float dt)
{
    if(!GlobalData::shared()->isInitFlag){
        return;
    }
    m_helpCount += 1;
    if(m_helpCount%5==0){
        m_noticeCount = 0;
    }
    if (!m_buildingInitState) {
        return;
    }
    map<int, FunBuildInfo>::iterator it;
    map<int, FunBuildInfo> *curBuildsInfo = FunBuildController::getInstance()->curBuildsInfo;
    for(it=curBuildsInfo->begin(); it != curBuildsInfo->end(); ++it)
    {
        bool tmpSt = false;
        if ((it->second).itemId > 0 && ((it->second).state==FUN_BUILD_CREATE || (it->second).state==FUN_BUILD_UPING || (it->second).state==FUN_BUILD_DESTROY || (it->second).state==FUN_BUILD_INIT))
        {
            auto curTime = GlobalData::shared()->getWorldTime();
            if ( (it->second).updateTime <= curTime)
            {
                if((it->second).state==FUN_BUILD_CREATE || (it->second).state==FUN_BUILD_INIT)
                {
                    if((it->second).state==FUN_BUILD_INIT)
                    {
                        onCreateBuild((it->second).itemId);
                    }
                    (it->second).state = FUN_BUILD_CHANGE;
                }
                else if((it->second).state==FUN_BUILD_DESTROY)
                {
                    (it->second).state = FUN_BUILD_DESTROY_END;
                }
                else
                {
                    (it->second).state = FUN_BUILD_UPING_END;
                }
                tmpSt = true;
            }
            else
            {
                if (m_buildItems.find(it->first) != m_buildItems.end()) {
                    int langTime = (it->second).updateTime - curTime;
                    m_buildItems[it->first]->setTime(langTime);
                }
            }
        }
        
        if((it->second).state==FUN_BUILD_NORMAL && m_buildItems.find(it->first) != m_buildItems.end()) {
            m_buildItems[it->first]->onUpdateUpIcon(0);
        }
        
        if((it->second).state==FUN_BUILD_INIT)//可建造状态 转变为 建造中状态
        {
            (it->second).state = FUN_BUILD_CREATE;
            onCreateBuild((it->second).itemId);
            showBuildBtns((it->second).itemId);
            if ((it->second).pos<=16) {
                checkTileGlow(NULL);
            }
        }
        
        if((it->second).state==FUN_BUILD_CHANGE)//建造中状态 转变为 正常状态
        {
            if( FunBuildController::getInstance()->completeUpOrCreate((it->second).itemId, true) )
            {
                onOpenBuildEnd((it->second).itemId);
                onUpgradeNewBuild((it->second).itemId);
                onResetBuild((it->second).itemId);
//                SoundController::sharedSound()->playEffects(Music_Sfx_city_build);
                if (tmpSt) {
                    FunBuildController::getInstance()->startFinish((it->second).itemId);
                }
                else {
                    int qid = QueueController::getInstance()->getQueueQidByKey(CC_ITOA((it->second).itemId));
                    QueueController::getInstance()->startFinishQueue(qid, false);
                    FunBuildController::getInstance()->clearUpBuildingInfo((it->second).itemId);
                }
            }
        }
        
        if((it->second).state==FUN_BUILD_UPING_END)//升级中状态 转变为 正常状态
        {
//            SoundController::sharedSound()->playEffects(Music_Sfx_city_build);
            if (FunBuildController::getInstance()->completeUpOrCreate((it->second).itemId))
            {
                onUpgradeNewBuild((it->second).itemId);
                onResetBuild((it->second).itemId);
                //                onUpgradeNewBuild((it->second).itemId);
                if(FunBuildController::getInstance()->building_Miracle_open && !GlobalData::shared()->isMaricleunlock){
                    if (!m_qijiBuild) {
                        onQijiBottomShow(20);
                    }else if(FunBuildController::getInstance()->getMainCityLv()==29){
                        m_qijiBuild->onBuildDelete();
                        m_qijiBuild->removeFromParent();
                        onQijiBottomShow(29);
                    }
                }
                if (tmpSt) {
                    FunBuildController::getInstance()->startFinish((it->second).itemId);
                }
                else {
                    int qid = QueueController::getInstance()->getQueueQidByKey(CC_ITOA((it->second).itemId));
                    QueueController::getInstance()->startFinishQueue(qid, false);
                    FunBuildController::getInstance()->clearUpBuildingInfo((it->second).itemId);
                }
            }
        }
        
        if (it->first==FUN_BUILD_MAIN_CITY_ID || (it->second).type==FUN_BUILD_SCIENE || (it->second).type==FUN_BUILD_BARRACK1 || (it->second).type==FUN_BUILD_BARRACK2 || (it->second).type==FUN_BUILD_BARRACK3 || (it->second).type==FUN_BUILD_BARRACK4 || (it->second).type==FUN_BUILD_FORT || (it->second).type==FUN_BUILD_HOSPITAL || (it->second).type==FUN_BUILD_STABLE || (it->second).type==FUN_BUILD_SACRIFICE || (it->second).type==FUN_BUILD_FORGE || (it->second).type==FUN_BUILD_WORKSHOP)
        {
            if ((it->second).state==FUN_BUILD_UPING || (it->second).state==FUN_BUILD_NORMAL) {
                if (m_buildItems.find((it->second).itemId) != m_buildItems.end()) {
                    m_buildItems[(it->second).itemId]->canShowState();
                }
            }
        }
        else if ((it->second).type==FUN_BUILD_WOOD || (it->second).type==FUN_BUILD_FOOD || (it->second).type==FUN_BUILD_IRON || (it->second).type==FUN_BUILD_STONE) {
            if ((it->second).state==FUN_BUILD_NORMAL) {
                if (m_buildItems.find((it->second).itemId) != m_buildItems.end()) {
                    m_buildItems[(it->second).itemId]->canShowState();
                }
            }
        }
        
        if((it->second).type==FUN_BUILD_WOOD || (it->second).type==FUN_BUILD_FOOD || (it->second).type==FUN_BUILD_IRON || (it->second).type==FUN_BUILD_STONE){
            if (m_buildItems.find((it->second).itemId) != m_buildItems.end()) {
                m_buildItems[(it->second).itemId]->checkIncreasedStatus();
            }
        }
        if ((it->second).state==FUN_BUILD_CREATE_ERROR || (it->second).state == FUN_BUILD_DESTROY_END || (it->second).state == FUN_BUILD_CANCEL_CREATE)
        {
            bool bOK = true;
            if ((it->second).state == FUN_BUILD_DESTROY_END) {
                bOK = FunBuildController::getInstance()->completeDestroy(it->first);
                if (bOK) {
                    FunBuildController::getInstance()->startFinish((it->second).itemId);
                }
                else {
                    FunBuildController::getInstance()->clearUpBuildingInfo((it->second).itemId);
                }
            }
            //建造出错
            if (bOK) {
                int key = it->first;
                int pos = it->first%1000;
                if (m_buildItems.find(key)!=m_buildItems.end()) {
                    m_buildItems.erase(m_buildItems.find(key));
                }
                FunBuild* delbuild = dynamic_cast<FunBuild*>(m_nodeBuildings[pos]->getChildByTag(pos));
                if( delbuild )
                {
                    delbuild->onBuildDelete();
                    m_nodeBuildings[pos]->removeChild(delbuild);
                }
                FunBuild* build = FunBuild::create( pos , m_nameLayer,m_lvLayer);
                build->setTag(pos);
                m_nodeBuildings[pos]->addChild(build);
                m_nodeBuildings[pos]->setContentSize(CCSizeMake(build->mainWidth, build->mainHeight));
                int od = m_nodeBuildings[pos]->getZOrder();
                CCSpriteBatchNode* displayBatch = m_resbatchNode;
                if (m_buildBatchMap.find(pos) != m_buildBatchMap.end()) {
                    displayBatch = m_buildBatchMap[pos];
                }
                build->setTileBatch(m_nodeBuildings[pos]->getPositionX(), m_nodeBuildings[pos]->getPositionY(),displayBatch, m_resbatchNode, m_scaffoldsBatchNode,od);
                build->m_key = 1000-od;
                
                curBuildsInfo->erase(it);
                break;
            }
        }
        else if((it->second).state==FUN_BUILD_UPING_ERROR)
        {
            //升级出错
            (it->second).state = FUN_BUILD_NORMAL;
            if (m_buildItems.find((it->second).itemId) != m_buildItems.end()) {
                m_buildItems[(it->second).itemId]->setTime(0);
            }
        }
        else if((it->second).state==FUN_BUILD_NORMAL_ERROR)
        {
            //已经升完级 更新级别信息
            (it->second).state = FUN_BUILD_NORMAL;
             if (m_buildItems.find((it->second).itemId) != m_buildItems.end()) {
                 m_buildItems[(it->second).itemId]->setTime(0,true);
             }
        }
        else if((it->second).state==FUN_BUILD_CANCEL_OTH)
        {
            //已经升完级 更新级别信息
            (it->second).state = FUN_BUILD_NORMAL;
             if (m_buildItems.find((it->second).itemId) != m_buildItems.end()) {
                 m_buildItems[(it->second).itemId]->setTime(0,true);
             }
        }
        
        //主城自定义状态
        if(SceneController::getInstance()->getMaincityCustom()==true && (it->second).type ==FUN_BUILD_MAIN){
            onUpgradeNewBuild((it->second).itemId);
            SceneController::getInstance()->setMaincityCustom(false);
        }
    }
    
    checkDayTime();//判断是否是白天
    changeDayOrNight(true, true);
    m_sysTime++;
    m_talkTime++;
    m_tipsTime++;
    m_guideFreeTime++;
    if (m_sysTime%2==0) {
        //每隔2秒执行一次
    }

    if (m_sysTime>70 && m_sysTime%80==0) {
//    if (m_sysTime>3 && m_sysTime%15==0) {
        //每隔80秒执行一次
        if (m_sysTime%160==0) {
            int rIdx = CCMathUtils::getRandomInt(0, 9);
            if (/* DISABLES CODE */ (false) && rIdx < 5) {
                showRain();
            }else {
                if (m_isDay && !m_isRain) {
                    showSunshine();
                }
            }
        }else {
            if (m_isDay && !m_isRain) {
                showSunshine();
            }
        }
    }
    if (m_sysTime>10 && m_sysTime%70==0) {
//        onDragonFly();
    }
    
    auto now = WorldController::getInstance()->getTime();
    if(now < GlobalData::shared()->cityFireStamp){
        if(m_failEffectNode->getChildrenCount()<=0){
            this->addDefenseFailEffect();
        }
    }else{
        m_failEffectNode->removeAllChildrenWithCleanup(true);
    }
    
    if (m_sysTime>1) {
        onStartShowPrincess();
    }
    checkTipsTime();
}

void ImperialScene::onQijiBottomShow(int lv){
    if(FunBuildController::getInstance()->getMainCityLv()>=lv) {
            m_qijiBuild = SpeBuild::create(SPE_BUILD_QIJI);
            m_qijiBuild->getOrderOfArrival();
            m_qijiNode->addChild(m_qijiBuild,1);
            int hod = m_qijiNode->getZOrder();
            m_qijiBuild->setNamePos(m_qijiNode->getPositionX(), m_qijiNode->getPositionY(), m_signLayer, m_popLayer, m_arrbatchNode, m_resbatchNode,hod);
    }
}

void ImperialScene::addToMovingLayer(CCNode* node){
    this->m_touchLayer->addChild(node);
}

void ImperialScene::stopShadow(int buildId, CCTouch *pTouch, CCEvent *pEvent)
{
    map<int, CCSafeNode<FunBuild>>::iterator it;
    for(it=m_buildItems.begin(); it != m_buildItems.end(); ++it)
    {
        if ( it->first == buildId && it->second != NULL) {
            (it->second)->stopShadow();
        }
        else{
            //            (it->second)->retTouch(pTouch, pEvent);
        }
    }
}

void ImperialScene::onResetBuild(int itemId)
{
    if (m_buildItems.find(itemId) != m_buildItems.end()) {
        m_buildItems[itemId]->setTime(0,true);
        m_buildItems[itemId]->onPlayUpEnd();
    }
}

void ImperialScene::onFlyOutPut(int itemId, int output, int forceResType)
{
    if (m_buildItems.find(itemId) != m_buildItems.end())
    {
        if (m_flyArrow->isVisible()) {
            stopArrowClick(0);
        }
        
        FunBuild *build = m_buildItems[itemId];
        if (forceResType == -1) {
            build->onFlyOutPut(0);
        }
        if (forceResType == -2){
            build->onFlyOutPutByValue(output);
        }
        
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        int startPosx = build->getParent()->getPositionX() + build->mainWidth/2 ;
        int startPosy = build->getParent()->getPositionY() + build->mainHeight/2;
        auto pt = onGetSceenPt(startPosx, startPosy);
        pt.y = pt.y-winSize.height;
        
        string picStr = "Food";
        auto endPt = ccp(0,0);
        int resType = Wood;
        if (itemId/1000 == FUN_BUILD_FOOD || forceResType==Food) {
            SoundController::sharedSound()->playEffects(Music_Sfx_city_harvest_farm);
            endPt = ccp(200,-60);//winSize.height
            if(CCCommonUtils::isIosAndroidPad())
            {
                endPt = ccpMult(endPt, 2.4);
            }
            picStr = "Food";
            resType = Food;
        }else if (itemId/1000 == FUN_BUILD_WOOD || forceResType==Wood) {
            SoundController::sharedSound()->playEffects(Music_Sfx_city_harvest_wood);
            endPt = ccp(90,-40);
            if(CCCommonUtils::isIosAndroidPad())
            {
                endPt = ccpMult(endPt, 2.4);
            }
            picStr = "Wood";
            resType = Wood;
        }
        else if (itemId/1000 == FUN_BUILD_IRON || forceResType==Iron) {
            SoundController::sharedSound()->playEffects(Music_Sfx_city_harvest_iron);
            endPt = ccp(320,-40);
            if(CCCommonUtils::isIosAndroidPad())
            {
                endPt = ccpMult(endPt, 2.4);
            }
            picStr = "Iron";
            resType = Iron;
        }
        else if (itemId/1000 == FUN_BUILD_STONE || forceResType==Stone) {
            SoundController::sharedSound()->playEffects(Music_Sfx_city_harvest_mithril);
            endPt = ccp(430,-40);
            if(CCCommonUtils::isIosAndroidPad())
            {
                endPt = ccpMult(endPt, 2.4);
            }
            picStr = "Stone";
            resType = Stone;
        }
        
        srand((unsigned)time(0));
//        int tmp = output*10/FunBuildController::getInstance()->getMaxOutPut(itemId);
        int tmp = output*10/1000;
        tmp = MAX(tmp, 2);
        tmp = MIN(tmp, 10);
        int idx = 0;
        while (tmp>0) {
            
//            auto flyCell = FlyCell::create(pt, endPt, picStr, 5, m_collectBatch, idx*0.2);
//            m_collectNode->addChild(flyCell);
            
            auto flyCell = FlyCell::create(pt, endPt,resType, picStr, 5, UIComponent::getInstance()->m_collectBatch, idx*0.2);
            UIComponent::getInstance()->m_collectNode->addChild(flyCell);
            
//            flyOutPut(itemId, m_flyNodes[tmp-1], idx);
            tmp--;
            idx++;
        }
    }
}

void ImperialScene::onFlyCargoOutPut(CCObject* obj)
{
    this->cargoShipGoOut();
    return;
    if (!m_cargoBuild || !m_cargoNode) {
        return;
    }
    auto dict = dynamic_cast<CCDictionary*>(obj);
    if (dict->objectForKey("reward")) {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        int startPosx = m_cargoNode->getPositionX() + m_cargoBuild->mainWidth/2 ;
        int startPosy = m_cargoNode->getPositionY() + m_cargoBuild->mainHeight/2;
        auto pt = onGetSceenPt(startPosx, startPosy);
        pt.y = pt.y-winSize.height;
        
        string picStr = "Food";
        auto endPt = ccp(0,0);
        int resType = Wood;
        
        auto arr = dynamic_cast<CCArray*>(dict->objectForKey("reward"));
        int rwdCnt = arr->count();
        if (rwdCnt < 1) {
            return;
        }
        CCDictionary* tdic;
        for (int i = 0; i < 1; ++i) {
            tdic = dynamic_cast<CCDictionary*>(arr->objectAtIndex(i));
            int type = tdic->valueForKey("type")->intValue();
            if (type == R_FOOD || type == R_WOOD || type == R_IRON || type == R_STONE) {
                switch (type) {
                    case R_FOOD:
                        endPt = ccp(200,-60);//winSize.height
                        picStr = "Food";
                        resType = Food;
                        break;
                        
                    case R_WOOD:
                        endPt = ccp(90,-40);
                        picStr = "Wood";
                        resType = Wood;
                        
                    case R_IRON:
                        endPt = ccp(320,-40);
                        picStr = "Iron";
                        resType = Iron;
                        
                    case R_STONE:
                        endPt = ccp(430,-40);
                        picStr = "Stone";
                        resType = Stone;
                        
                    default:
                        break;
                }
                int num = tdic->valueForKey("value")->intValue();
                int tmp = num*10/1000;
                tmp = MAX(tmp, 2);
                tmp = MIN(tmp, 10);
                int idx = 0;
                while (tmp>0) {
                    auto flyCell = FlyCell::create(pt, endPt,resType, picStr, 5, UIComponent::getInstance()->m_collectBatch, idx*0.2);
                    UIComponent::getInstance()->m_collectNode->addChild(flyCell);
                    tmp--;
                    idx++;
                }
            }
        }
    }
}

void ImperialScene::onUpdateInfo()
{
    if( FunBuildController::getInstance()->curBuildsInfo == NULL)
        return;
    //已有建筑物
    map<int,FunBuildInfo>::iterator it;
    it = FunBuildController::getInstance()->curBuildsInfo->begin();
    for (; it != FunBuildController::getInstance()->curBuildsInfo->end(); it++)
    {
        if((it->second).type == FUN_BUILD_QIJI && !FunBuildController::getInstance()->building_Miracle_open){
            continue;
        }
        if (it->first>0 && ((it->second).state != FUN_BUILD_LOCK) && (it->second).pos<BUILD_COUNT)
        {
            if(it->second.level==0) {
                it->second.level=1;
            }
            FunBuild* build = FunBuild::create( it->first , m_nameLayer,m_lvLayer);
            if(build==NULL){
                continue;
            }
            build->setTag((it->second).pos);
            m_nodeBuildings[(it->second).pos]->addChild(build);
            m_nodeBuildings[(it->second).pos]->setContentSize(CCSizeMake(build->mainWidth, build->mainHeight));
            
            CCSpriteBatchNode* curBatch = m_resbatchNode;
            CCSpriteBatchNode* curBlentBatch = m_resBlentBatchNode;
            if (m_buildBatchMap.find((it->second).pos) != m_buildBatchMap.end()) {
                curBatch = m_buildBatchMap[(it->second).pos];
                curBlentBatch = NULL;
            }
            
            int od = m_nodeBuildings[(it->second).pos]->getZOrder();
            build->setNamePos(m_nodeBuildings[(it->second).pos]->getPositionX()
                              , m_nodeBuildings[(it->second).pos]->getPositionY(), m_signLayer, m_popLayer, m_arrbatchNode, curBatch,m_resbatchNode,m_scaffoldsBatchNode, od, curBlentBatch);
            if (m_buildSpineMap.find((it->second).pos) != m_buildSpineMap.end()) {
                build->setSpineLayer( m_buildSpineMap[(it->second).pos] ); //a by ljf
            }
            m_buildItems[it->first] = build;
            build->m_key = 1000-od;
        }
    }
    //空余的格子...
    for (int i = 0; i<BUILD_COUNT; i++) {
        int cnt = m_nodeBuildings[i]->getChildrenCount();
        if (cnt<=0) {
            //加入地块
            FunBuild* build = FunBuild::create(i,m_nameLayer,m_lvLayer);
            build->setTag(i);
            m_nodeBuildings[i]->addChild(build);
            m_nodeBuildings[i]->setContentSize(CCSizeMake(build->mainWidth, build->mainHeight));
            int od = m_nodeBuildings[i]->getZOrder();
            m_nodeBuildings[i]->setZOrder(od);
            CCSpriteBatchNode* displayBatch = m_resbatchNode;
            if (m_buildBatchMap.find(i) != m_buildBatchMap.end()) {
                displayBatch = m_buildBatchMap[i];
            }
            build->setTileBatch(m_nodeBuildings[i]->getPositionX(), m_nodeBuildings[i]->getPositionY(),displayBatch,m_resbatchNode,m_scaffoldsBatchNode, od);
            build->m_key = 1000-od;
                //begin a by ljf
                if (m_buildSpineMap.find(i) != m_buildSpineMap.end()) {
                    build->setSpineLayer( m_buildSpineMap[(it->second).pos] ); //a by ljf
                }
                //end a by ljf
        }
    }
}

int ImperialScene::findCanBuildTile(int pos)
{
    if (pos>=0 && pos<BUILD_COUNT) {
        FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[pos]->getChildByTag(pos));
        if (build && build->m_buildingKey == pos) {
            return pos;
        }
        else {
            int tileType = FunBuildController::getInstance()->getTileType(pos);
            if (tileType == 2) {
                int posEnd = pos + 4;//城外建筑从每个区域的最小值开始向后查找总共5个地块,如果向后查找5个找不到则从17开始查找
                posEnd = posEnd >= BUILD_COUNT ? (BUILD_COUNT - 1) : posEnd;
                for (int i = pos; i <= posEnd; i++) {
                    FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[i]->getChildByTag(i));
                    if (build && build->m_buildingKey == i) {
                        auto& tileInfo = FunBuildController::getInstance()->m_tileMap[i];
                        if (!(tileInfo.state == FUN_BUILD_LOCK || tileInfo.xmlOpen==1) && tileInfo.type == 2) {
                            return i;
                        }
                    }
                }
            }
            for (int i=0; i<BUILD_COUNT; i++) {
                FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[i]->getChildByTag(i));
                if (build && build->m_buildingKey == i) {
                    auto& tileInfo = FunBuildController::getInstance()->m_tileMap[i];
                    if (!(tileInfo.state == FUN_BUILD_LOCK || tileInfo.xmlOpen==1) && tileInfo.type == tileType) {
                        return i;
                    }
                }
            }
        }
    }
    return -1;
}

void ImperialScene::onOpenBuildEnd(int itemId)
{
    if (m_buildItems.find(itemId) == m_buildItems.end()) {
        onCreateBuild(itemId);
    }
    
    if (m_buildItems.find(itemId) != m_buildItems.end())
        m_buildItems[itemId]->changeBg();
}

void ImperialScene::onCreateBuild(int itemId)
{
    int pos = itemId%1000;
    if( pos>=BUILD_COUNT )
        return;
    
    FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[pos]->getChildByTag(pos));
    if (build) {
        build->initFunBuild(itemId, m_nameLayer,m_lvLayer);
        CCSpriteBatchNode* curBatch = m_resbatchNode;
        CCSpriteBatchNode* curBlentBatch = m_resBlentBatchNode;
        if (m_buildBatchMap.find(pos) != m_buildBatchMap.end()) {
            curBatch = m_buildBatchMap[pos];
            curBlentBatch = NULL;
        }
        int od = m_nodeBuildings[pos]->getZOrder();
        build->setNamePos(m_nodeBuildings[pos]->getPositionX()
                          , m_nodeBuildings[pos]->getPositionY(),
                          m_signLayer,
                          m_popLayer,
                          m_arrbatchNode,
                          curBatch,
                          m_resbatchNode,m_scaffoldsBatchNode,od,curBlentBatch);
        if (m_buildSpineMap.find(pos) != m_buildSpineMap.end()) {
            build->setSpineLayer( m_buildSpineMap[pos] ); //a by ljf
        }
        m_buildItems[itemId] = build;
        build->m_key = 1000-od;
    }
}

void ImperialScene::onOpenBuild(int itemId)
{
}

CCNode* ImperialScene::getBuildById(int itemId){
    if (itemId == SPE_BUILD_NEWED || itemId == SPE_BUILD_MERCHANTE || itemId == SPE_BUILD_GOLD_BOX || itemId == SPE_BUILD_CARGO) {
        if (m_newRDBuild && itemId == SPE_BUILD_NEWED) {
            return m_newRDBuild->m_guideNode;
        }else if (m_merchanteBuild && itemId == SPE_BUILD_MERCHANTE) {
            return m_merchanteBuild->m_guideNode;
        }else if (m_goldBoxBuild && itemId == SPE_BUILD_GOLD_BOX) {
            return m_goldBoxBuild->m_guideNode;
        }else if (m_cargoNode && itemId == SPE_BUILD_CARGO) {
                return m_cargoBuild->m_guideNode;
        }else {
            return NULL;
        }
    }
    if(m_buildItems.empty()){
        return NULL;
    }
    int id = FunBuildController::getInstance()->getMaxLvBuildByType(itemId);
    
    map<int, CCSafeNode<FunBuild>>::iterator it = m_buildItems.find(id);
    if(it != m_buildItems.end()){
        if(itemId == FUN_BUILD_FOOD) {
            setUnMoveScence(true);
            return m_buildItems[id]->m_spr;
        }
        setUnMoveScence(true);
        return m_buildItems[id];
    }
    if(itemId < BUILD_COUNT - 1){
        map<int, FunBuildInfo>::iterator it;
        for(it = GlobalData::shared()->imperialInfo.begin(); it != GlobalData::shared()->imperialInfo.end(); it++){
            if(it->second.pos == itemId){
                return NULL;
            }
        }
        FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[itemId]->getChildByTag(itemId));
        setUnMoveScence(true);
        return build->m_tile;
    }
    return NULL;
}

void ImperialScene::onFinishMoveBuild(CCObject* obj)
{
    CCDictionary* posData = dynamic_cast<CCDictionary*>(obj);
    if (posData) {
        int oldBuild1 = posData->valueForKey("oldBuild1")->intValue();
        int oldBuild2 = posData->valueForKey("oldBuild2")->intValue();
        int newBuild1 = posData->valueForKey("newBuild1")->intValue();
        int newBuild2 = posData->valueForKey("newBuild2")->intValue();
        int pos1 = posData->valueForKey("pos1")->intValue();
        int pos2 = posData->valueForKey("pos2")->intValue();
        
        if (oldBuild1>0 && m_buildItems.find(oldBuild1)!=m_buildItems.end()) {
            m_buildItems.erase(m_buildItems.find(oldBuild1));
        }
        if (oldBuild2>0 && m_buildItems.find(oldBuild2)!=m_buildItems.end()) {
            m_buildItems.erase(m_buildItems.find(oldBuild2));
        }
        
        FunBuild* delbuild1 = dynamic_cast<FunBuild*>(m_nodeBuildings[pos1]->getChildByTag(pos1));
        if( delbuild1 )
        {
            delbuild1->onBuildDelete();
            m_nodeBuildings[pos1]->removeChild(delbuild1);
        }
        FunBuild* delbuild2 = dynamic_cast<FunBuild*>(m_nodeBuildings[pos2]->getChildByTag(pos2));
        if( delbuild2 )
        {
            delbuild2->onBuildDelete();
            m_nodeBuildings[pos2]->removeChild(delbuild2);
        }
        if (newBuild1>0) {
            FunBuild* build = FunBuild::create( newBuild1 , m_nameLayer,m_lvLayer);
            build->setTag(pos2);
            m_nodeBuildings[pos2]->addChild(build);
            m_nodeBuildings[pos2]->setContentSize(CCSizeMake(build->mainWidth, build->mainHeight));
            m_buildItems[newBuild1] = build;
            
            CCSpriteBatchNode* curBatch = m_resbatchNode;
            CCSpriteBatchNode* curBlentBatch = m_resBlentBatchNode;
            if (m_buildBatchMap.find(pos2) != m_buildBatchMap.end()) {
                curBatch = m_buildBatchMap[pos2];
                curBlentBatch = NULL;
            }
            
            curBatch->setPosition(ccp(0, 0));
            int od = m_nodeBuildings[pos2]->getZOrder();
            build->m_key = 1000-od;
            build->setNamePos(m_nodeBuildings[pos2]->getPositionX()
                              , m_nodeBuildings[pos2]->getPositionY(), m_signLayer, m_popLayer, m_arrbatchNode, curBatch,m_resbatchNode,m_scaffoldsBatchNode,od, curBlentBatch);
            build->onPlayUpEnd(true);
        }else {
            FunBuild* build = FunBuild::create( pos2 , m_nameLayer,m_lvLayer);
            build->setTag(pos2);
            m_nodeBuildings[pos2]->addChild(build);
            m_nodeBuildings[pos2]->setContentSize(CCSizeMake(build->mainWidth, build->mainHeight));
            int od = m_nodeBuildings[pos2]->getZOrder();
            m_nodeBuildings[pos2]->setZOrder(od);
            CCSpriteBatchNode* displayBatch = m_resbatchNode;
            if (m_buildBatchMap.find(pos2) != m_buildBatchMap.end()) {
                displayBatch = m_buildBatchMap[pos2];
            }
            build->setTileBatch(m_nodeBuildings[pos2]->getPositionX(), m_nodeBuildings[pos2]->getPositionY(),displayBatch, m_resbatchNode,m_scaffoldsBatchNode, od);
            build->m_key = 1000-od;
        }
        
        if (newBuild2>0) {
            FunBuild* build = FunBuild::create( newBuild2 , m_nameLayer,m_lvLayer);
            build->setTag(pos1);
            m_nodeBuildings[pos1]->addChild(build);
            m_nodeBuildings[pos1]->setContentSize(CCSizeMake(build->mainWidth, build->mainHeight));
            m_buildItems[newBuild2] = build;
            
            CCSpriteBatchNode* curBatch = m_resbatchNode;
            CCSpriteBatchNode* curBlentBatch = m_resBlentBatchNode;
            if (m_buildBatchMap.find(pos1) != m_buildBatchMap.end()) {
                curBatch = m_buildBatchMap[pos1];
                curBlentBatch = NULL;
            }
            
            curBatch->setPosition(ccp(0, 0));
            int od = m_nodeBuildings[pos1]->getZOrder();
            build->m_key = 1000-od;
            build->setNamePos(m_nodeBuildings[pos1]->getPositionX()
                              , m_nodeBuildings[pos1]->getPositionY(), m_signLayer, m_popLayer, m_arrbatchNode, curBatch,m_resbatchNode,m_scaffoldsBatchNode,od, curBlentBatch);
            build->onPlayUpEnd(true);
        }else {
            FunBuild* build = FunBuild::create( pos1 , m_nameLayer,m_lvLayer);
            build->setTag(pos1);
            m_nodeBuildings[pos1]->addChild(build);
            m_nodeBuildings[pos1]->setContentSize(CCSizeMake(build->mainWidth, build->mainHeight));
            int od = m_nodeBuildings[pos1]->getZOrder();
            m_nodeBuildings[pos1]->setZOrder(od);
            CCSpriteBatchNode* displayBatch = m_resbatchNode;
            if (m_buildBatchMap.find(pos1) != m_buildBatchMap.end()) {
                displayBatch = m_buildBatchMap[pos1];
            }
            build->setTileBatch(m_nodeBuildings[pos1]->getPositionX(), m_nodeBuildings[pos1]->getPositionY(),displayBatch, m_resbatchNode,m_scaffoldsBatchNode, od);
            build->m_key = 1000-od;
        }
    }
}

void ImperialScene::setUnMoveScence(bool st)
{
    if(m_viewPort && m_viewPort->notMove!=st){
        m_viewPort->notMove = st;
    }
}

void ImperialScene::showBuildBtns(int itemId)
{
    map<int, CCSafeNode<FunBuild>>::iterator it = m_buildItems.find(itemId);
    if(it != m_buildItems.end()){
        lastTouchBuildId = -1;
        m_buildItems[itemId]->onSetClickType();
        m_buildItems[itemId]->onClickThis(0);
    }
}
void ImperialScene::clickWorkshop(int itemId){
//    m_buildItems[itemId]->onClickThis(0);
    map<int, CCSafeNode<FunBuild>>::iterator it = m_buildItems.find(itemId);
    if(it!=m_buildItems.end() && m_buildItems[itemId]->isUnlockThis()){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_SAVEPOS);
        setUnMoveScence(false);
        onMoveToBuildAndOpen(itemId, TYPE_POS_MID);
        PopupViewController::getInstance()->addPopupView(ToolCreateView::create());
    }
}

CCNode* ImperialScene::getBuildNameById(int itemId, string _key)
{
    if(m_viewPort->notMove) {
        return NULL;
    }
    if (_key=="up" || _key=="detail" || _key=="soldier" || _key=="science" || _key=="forge" || _key=="heal") {
        return m_buildBtnsView->getGuideNode(_key);
    }
    else if (_key=="cd") {
        CCNode* ret=NULL;
        int id = FunBuildController::getInstance()->getMaxLvBuildByType(itemId);
        map<int, CCSafeNode<FunBuild>>::iterator it = m_buildItems.find(id);
        if(it != m_buildItems.end()){
            ret = m_buildItems[id]->getGuideNode(_key);
        }
        if (ret==NULL) {
            ret = m_buildBtnsView->getGuideNode(_key);
        }
        return ret;
    }
    else if (_key=="troop" || _key=="collect") {
        int id = FunBuildController::getInstance()->getMaxLvBuildByType(itemId);
        map<int, CCSafeNode<FunBuild>>::iterator it = m_buildItems.find(id);
        if(it != m_buildItems.end()){
            return m_buildItems[id]->getGuideNode(_key);
        }
    }
    return NULL;
}

CCNode* ImperialScene::getSpeBuildNameById(int itemId, string _key)
{
    if(m_viewPort->notMove) {
        return NULL;
    }
    if (_key=="longjing" && itemId == SPE_BUILD_GOLD_BOX) {
        return m_speBuildBtnsView->getGuideNode(_key);
    }
    return NULL;
}

void ImperialScene::onPowerADD(CCObject* obj)
{
    int bid = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_SCIENE);
    if (bid >0 ) {
        flyBall(bid, 1, false);
    }
}

void ImperialScene::showFlyBall(CCObject* obj)
{
    int buildId = -1;
    if(obj==NULL || (dynamic_cast<CCInteger*>(obj)==NULL)){
        buildId = -1;
    }else {
        CCInteger* intObj = dynamic_cast<CCInteger*>(obj);
        buildId = intObj->getValue();
    }

    flyBall(buildId, 0, false);
}

void ImperialScene::flyBall(int buildId, int type, bool toScene)
{
    auto &info = (*FunBuildController::getInstance()->curBuildsInfo)[buildId];
    if (m_buildItems.find(buildId) == m_buildItems.end()) {
        return;
    }
//    auto sp = m_nodeBuildings[info.pos]->getPosition();
//    auto &sz = m_buildItems[buildId]->getContentSize();
//    sp.x += sz.width/2;
//    sp.y += sz.height/2;
    
    auto pt = ccp(CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2);
    long addPower = 0;
    if (type == 0) {
         if(info.level<= 30 && info.starNum == 0&& info.starRate == 0){
             auto it = info.mapPower.find(info.level);
             if(it!=info.mapPower.end()){
                 addPower = info.mapPower[info.level];
             }
        }else{
            addPower = info.getStarPower(info.starNum,info.starRate);
        }
        GlobalData::shared()->playerInfo.lastBattlePower =  GlobalData::shared()->playerInfo.battlePower;
        GlobalData::shared()->playerInfo.buildingPower = GlobalData::shared()->playerInfo.buildingPower + addPower;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_COLLECT_SOLDIER_ADD_POWER);
    }
    if (type == 1) { // 只播放战斗力
        onFlyTroopParticle(pt.x,pt.y,1);
    }
    else {
        long addPower = 0;
        if (type == 0) {
             if(info.level<= 30 && info.starNum == 0&& info.starRate == 0){
                addPower = info.mapPower[info.level];
            }else{
                addPower = info.getStarPower(info.starNum,info.starRate);
            }
        }
        int addExp = info.oldExp;
        if (addExp > 0 && addPower <= 0) {
            onFlyTroopParticle(pt.x,pt.y,2);
        }else if (addExp <= 0 && addPower > 0) {
            onFlyTroopParticle(pt.x,pt.y,1);
        }else {
            onFlyTroopParticle(pt.x,pt.y,0);
        }
    }
//    if(buildId/1000 == FUN_BUILD_QIJI && info.starNum == 0 && (int)info.starRate == 0){
//        auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
//        layer->onMoveToBuildAndPlay(buildId);
//    }
}

void ImperialScene::flyBallEnd(CCNode* pSender)
{
    pSender->stopAllActions();
    pSender->setVisible(false);
}

void ImperialScene::flyOutPut(int buildId, CCNode* flNode, int idx)
{
    flNode->stopAllActions();
    flNode->removeAllChildren();
    
    auto &info = (*FunBuildController::getInstance()->curBuildsInfo)[buildId];
    makeFlyMc(info.type, flNode);
    
    auto sp = m_nodeBuildings[info.pos]->getPosition();
    auto &sz = m_buildItems[buildId]->getContentSize();
    sp.x += sz.width/2;
    sp.y += sz.height/2;
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    int tx = 0;
    int ty = 0;
    if(info.type == 1) {
        tx = (winSize.width*0.8 - (m_touchLayer->getPositionX()-m_viewPort->bottomLeft.x))/m_touchLayer->getScale();
        ty = (winSize.height*0.9 - (m_touchLayer->getPositionY()-m_viewPort->bottomLeft.y))/m_touchLayer->getScale();
    }
    else if(info.type == 2) {
        tx = (winSize.width*0.8 - (m_touchLayer->getPositionX()-m_viewPort->bottomLeft.x))/m_touchLayer->getScale();
        ty = (winSize.height*0.8 - (m_touchLayer->getPositionY()-m_viewPort->bottomLeft.y))/m_touchLayer->getScale();
    }
    
    int sx = sp.x-0;
    int sy = sp.y;
    
    //    CCActionInterval * delaytime = CCDelayTime::create(0.2*idx);
    
    int fh = 0;
    int fw = 0;
    float ration = 0;
    int add = rand()%6 * 15 - 45;
    int rand = sz.height*1.0 + add;
    float ra = 15;
    if (idx==0) {
        fh = rand;
    }
    else {
        if (idx%2==0) {
            ration = (idx/2)*ra*3.14/180;
            fw = -rand*sinf(ration);
            fh = rand*cosf(ration);
        }
        else {
            ration = (idx+1/2)*ra*3.14/180;
            fw = rand*sinf(ration);
            fh = rand*cosf(ration);
        }
    }
    
    CCActionInterval* moveBy = CCMoveBy::create(0.3,ccp(fw, fh));
    CCMoveTo* move = CCMoveTo::create(1.0, ccp(tx,ty));
    CCActionInterval* act = CCEaseSineIn::create(move);
    //delaytime,
    auto  actionBy = CCSequence::create(moveBy,act,CCCallFuncN::create(this, callfuncN_selector(ImperialScene::flyOutPutEnd)), NULL);
    flNode->setPosition(ccp(sx, sy));
    flNode->setVisible(true);
    flNode->runAction(actionBy);
}

void ImperialScene::flyOutPutEnd(CCNode* pSender)
{
    pSender->setVisible(false);
    pSender->stopAllActions();
    pSender->removeAllChildren();
}

void ImperialScene::makeFlyMc(int type, CCNode* flNode)
{
    string picName = "";
    if (type == 1) {
        picName = "fly_money_";
    }
    else if (type == 2) {
        picName = "fly_wood_";
    }
    else if (type == 3) {
        picName = "fly_iron_";
    }
    if (picName == "") {
        return;
    }
    
    auto sprAni = CCSprite::create();
    sprAni->setAnchorPoint(ccp(0.5, 0.5));
    sprAni->setScale(1.0);
    int size = 8;
    Vector<SpriteFrame*> myArray;
    CCSpriteFrame *myframe[size];
    for (int i=0; i<size; i++) {
        string tmp = picName+CC_ITOA(i+1)+".png";
        myframe[i] = CCLoadSprite::loadResource(CCString::createWithFormat(tmp.c_str(), i+1)->getCString());
        myframe[i]->getTexture()->setAntiAliasTexParameters();
        myArray.pushBack(myframe[i]);
    }
    int idx = rand()%8;
    sprAni->setDisplayFrame(myframe[idx]);
    CCAnimation *animation=CCAnimation::createWithSpriteFrames(myArray, 0.07f);
    CCAnimate* animate = CCAnimate::create(animation);
   
    int ra = rand()%18 * 10;
    sprAni->setRotation(ra-180);
    sprAni->stopAllActions();
    sprAni->runAction(CCRepeatForever::create(animate));
    
    flNode->addChild(sprAni);
}

void ImperialScene::initMc2()
{
    m_crossServeActFlag = false;
    if (ActivityController::getInstance()->activityArr && ActivityController::getInstance()->activityArr->count()>0) {
        int num = ActivityController::getInstance()->activityArr->count();
        for (int i=0; i<num; i++) {
            auto temp = dynamic_cast<ActivityEventObj*>(ActivityController::getInstance()->activityArr->objectAtIndex(i));
            if (temp && temp->type==7) {
                m_crossServeActFlag = true;
                break;
            }
        }
    }
    
    if (m_crossServeActFlag) {
        int zOrder = m_mcNode2->getZOrder();
        auto pArray = CCPointArray::create(20);
        pArray->addControlPoint(ccp(7.0, 0));
        pArray->addControlPoint(ccp(36.0, 16.0));
        pArray->addControlPoint(ccp(68, 34));
        pArray->addControlPoint(ccp(90.0, 20));
        pArray->addControlPoint(ccp(60.0, 5));
        pArray->addControlPoint(ccp(34.0, -12));
        pArray->addControlPoint(ccp(75.0, -25.0));
        pArray->addControlPoint(ccp(129.0, 4.0));
        for (int i=0; i<8; i++) {
            string sIcon = "soldier_Pikeman.png";
            if(i>5){
                sIcon = "soldier_banner.png";
            }
            auto bing = CCLoadSprite::createSprite(sIcon.c_str());
            bing->setPosition(pArray->getControlPointAtIndex(i) + m_crossNode->getPosition());
            m_resbatchNode->addChild(bing);
        }
        auto soldier_flag = CCLoadSprite::loadResource("soldier_flag.png");
        CCFlagWaveSprite* flag = CCFlagWaveSprite::create(soldier_flag);
        m_soldierFlagNode->addChild(flag);
        flag->setAnchorPoint(ccp(0, 1));
        flag->setFlipX(true);
        flag->setScale(0.2);
        flag->setPosition(ccp(49, 9) + m_crossNode->getPosition());
        
        auto soldier_flag2 = CCLoadSprite::loadResource("soldier_flag.png");
        CCFlagWaveSprite* flag2 = CCFlagWaveSprite::create(soldier_flag2);
        m_soldierFlagNode->addChild(flag2);
        flag2->setAnchorPoint(ccp(0, 1));
        flag2->setFlipX(true);
        flag2->setScale(0.2);
        flag2->setPosition(ccp(103, 39) + m_crossNode->getPosition());
        
        auto pArr = CCPointArray::create(20);
        pArr->addControlPoint(ccp(1, 0));
        pArr->addControlPoint(ccp(1, 0));
        
        m_pt2Array = CCPointArray::create(20);
        auto person = Person::create(m_crossNode->getPositionX() + 30,m_crossNode->getPositionY() -30 , m_resbatchNode, zOrder*1000, m_pt2Array, 12,1);
        m_personArray->addObject(person);
        person->onSetType(1);
    }
    
//    if (1) {
//        int zOrder = m_mcNode2->getZOrder();
//        m_ptArray = CCPointArray::create(20);
//        m_ptArray->addControlPoint(ccp(400, -200));
//        m_ptArray->addControlPoint(ccp(-400, 200));
//        
//        for (int i=0; i<5; i++) {
//            auto person = Person::create(m_mcNode2->getPositionX()-45+i*20,m_mcNode2->getPositionY()-25+i*(-10), m_resbatchNode, zOrder*1000, m_ptArray, 15);
//            m_personArray->addObject(person);
//            if(i==0) {
//                person->onSetType(0);
//            }
//        }
//        for (int i=0; i<5; i++) {
//            auto person = Person::create(m_mcNode2->getPositionX()-5+i*20,m_mcNode2->getPositionY()-5+i*(-10), m_resbatchNode, zOrder*1000, m_ptArray, 15);
//            m_personArray->addObject(person);
//            if(i==0) {
//                person->onSetType(0);
//            }
//        }
//    }

    // devil 暂时关闭feedback
    if (0) {
        int zOrder = m_desNode1->getZOrder();
        m_mailBuild = SpeBuild::create(SPE_BUILD_MAIL);
        m_desNode1->addChild(m_mailBuild);
        m_mailBuild->setNamePos(m_desNode1->getPositionX()+7, m_desNode1->getPositionY()+75, m_signLayer, m_popLayer, m_arrbatchNode, m_resbatchNode, zOrder);
        
        int m = 5;

        m_HTPHead = CCLoadSprite::createSprite("feedback_head.png");
        CCSize size = m_HTPHead->getContentSize();
        std::string language = LocalController::shared()->getLanguageFileName();
        m_HTPHead->setScaleX(-1.5);//-2
        if (language=="id"||language=="ru") {
            m_HTPHead->setScaleX(-1.6);
        }
        if (language=="zh_CN"||language=="zh_TW") {
            m_HTPHead->setScaleX(-0.8);
        }
        m_HTPHead->setScaleY(0.6);
        m_HTPHead->setPosition(m_desNode1->getPosition()+ccp(0, 80));
        m_funLayer->addChild(m_HTPHead);
        m_HTPHead->setZOrder(zOrder*1000+1+m);
        
        m_HTPHead1 = CCLoadSprite::createSprite("feedback_head1.png");
        m_HTPHead1->setPosition(m_desNode1->getPosition()+ccp(0, 40));
        m_HTPHead1->setScaleX(-1);
        m_funLayer->addChild(m_HTPHead1);
        m_HTPHead1->setZOrder(zOrder*1000+2+m);
        
        m_desNode1->setContentSize(CCSizeMake(150, 150));
        m_HTPlabel = CCLabelIF::create();
        m_HTPlabel->setString(_lang("101230")+_lang("101231"));//todo:放到语言文件中
        m_HTPlabel->setColor({108,58,25});
        m_HTPlabel->setAnchorPoint(ccp(0.5,0.5));
        m_HTPlabel->setVisible(false);
        m_cloudLayer->addChild(m_HTPlabel);
        m_HTPlabel->setPosition(m_HTPHead->getPosition()+ccp(0,0));
        
        m_icon_op_mail = CCLoadSprite::createSprite("icon_op_mail.png");
        m_icon_op_mail->setScale(1.2);
        m_icon_op_mail->setPosition(m_HTPHead->getPosition());
        m_funLayer->addChild(m_icon_op_mail);
        m_icon_op_mail->setZOrder(zOrder*1000+2+m);
        
        CCSequence* sequ = CCSequence::create(CCScaleTo::create(0.8, 1.3),CCScaleTo::create(1, 1.0), NULL);
        m_icon_op_mail->runAction(CCRepeatForever::create(sequ));
        
        m_icon_op_mail->setVisible(false);

        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ImperialScene::onShowHTPAnim), this);
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ImperialScene::onShowHTPAnim), this, 0.2,kCCRepeatForever, 0.0f, false);

        m_noticeCount = 0;

        if(m_noticeCount>0){
            m_HTPHead->setScaleX(-0.9);
        }
        m_mailBuild->getAnimationManager()->runAnimationsForSequenceNamed("Default");
    }
}

#pragma mark -
#pragma mark CocosBuilder Part
bool ImperialScene::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCLOG("_______________%s",pMemberVariableName);
    if (pTarget == this && strncmp(pMemberVariableName, "m_node",6) == 0) {
        char index[5] = "";
        strncpy(index, pMemberVariableName + 6, strlen(pMemberVariableName) - 6);
        
        int cityIndex = atoi(index);
        CCAssert(cityIndex < BUILD_COUNT,"Node citys could not more than m_nodeCity!");
        m_nodeBuildings[cityIndex] = pNode;
        m_nodeBuildings[cityIndex]->retain();
        return true;
    }
    else if (pTarget == this && strncmp(pMemberVariableName, "m_river1",8) == 0) {
        m_river1Array->addObject(pNode);
        return true;
    }
    else if (pTarget == this && strncmp(pMemberVariableName, "m_wave1",7) == 0) {
        m_wave1Array->addObject(pNode);
        return true;
    }
    else if (pTarget == this && strncmp(pMemberVariableName, "m_wave2",7) == 0) {
        m_wave2Array->addObject(pNode);
        return true;
    }
    else if (pTarget == this && strncmp(pMemberVariableName, "m_wave3",7) == 0) {
        m_wave3Array->addObject(pNode);
        return true;
    }
    else if (pTarget == this && strncmp(pMemberVariableName, "m_bigTileNode",13) == 0) {
        char index[5] = "";
        strncpy(index, pMemberVariableName + 13, strlen(pMemberVariableName) - 13);
        int idx = atoi(index);
        m_bigTileNodes[idx] = pNode;
        return true;
    }
    
    else if (pTarget == this && strncmp(pMemberVariableName, "m_animation",11) == 0) {
        char index[5] = "";
        strncpy(index, pMemberVariableName + 11, strlen(pMemberVariableName) - 11);
        int idx = atoi(index);
        m_animationNodes[idx] = pNode;
        return true;
    }
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_Stand", CCNode*, this->m_animation_Stand);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sequence", CCNode*, this->m_animation_sequence);
    

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mcNode1", CCNode*, this->m_mcNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mcNode2", CCNode*, this->m_mcNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mcNode3", CCNode*, this->m_mcNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mcNode4", CCNode*, this->m_mcNode4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mcNode5", CCNode*, this->m_mcNode5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode1", CCNode*, this->m_desNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode2", CCNode*, this->m_desNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode3", CCNode*, this->m_desNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_torchNode", CCNode*, this->m_torchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_torchNode1", CCNode*, this->m_torchNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_torchNode2", CCNode*, this->m_torchNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_torchNode3", CCNode*, this->m_torchNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_torchNode4", CCNode*, this->m_torchNode4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_torchNode5", CCNode*, this->m_torchNode5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_torchNode6", CCNode*, this->m_torchNode6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_torchNode7", CCNode*, this->m_torchNode7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_torchNode8", CCNode*, this->m_torchNode8);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_decoratem_1", CCNode*, this->m_decorateNodes[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_decoratem_2", CCNode*, this->m_decorateNodes[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_decoratem_3", CCNode*, this->m_decorateNodes[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_decoratem_4", CCNode*, this->m_decorateNodes[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_decoratem_5", CCNode*, this->m_decorateNodes[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_decoratem_6", CCNode*, this->m_decorateNodes[5]);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flagNode", CCNode*, this->m_flagNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hdNode", CCNode*, this->m_hdNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wallNode1", CCNode*, this->m_wallNodes[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wallNode2", CCNode*, this->m_wallNodes[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wallNode3", CCNode*, this->m_wallNodes[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_FountainTouchNode", CCNode*, this->m_FountainTouchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldMineNode", CCNode*, this->m_goldMineNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldBoxNode", CCNode*, this->m_goldBoxNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_chrTreeNode", CCNode*, this->m_chrTreeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_willOpenNode", CCNode*, this->m_willOpenNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_shipNode", CCNode*, this->m_shipNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_failEffectNode", CCNode*, this->m_failEffectNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_merchanteNode", CCNode*, this->m_merchanteNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cargoNode", CCNode*, this->m_cargoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newRDNode", CCNode*, this->m_newRDNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lotteryNode", CCNode*, this->m_lotteryNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_xiongdiNode", CCNode*, this->m_xiongdiNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_qijiNode", CCNode*, this->m_qijiNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_forestNode", CCNode*, this->m_forestNode);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_couldNode", CCNode*, this->m_couldNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_crossNode", CCNode*, this->m_crossNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_crossClickNode", CCNode*, this->m_crossClickNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particalNode", CCNode*, this->m_particalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragonNode", CCNode*, this->m_dragonNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ziyuanmenNode", CCNode*, this->m_ziyuanmenNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragonCaveGlow", Node*, m_dragonCaveGlow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_penquanRainbow", Node*, m_penquanRainbow);
    return false;
}

void ImperialScene::showAttention(){
    std::string steps = CCUserDefault::sharedUserDefault()->getStringForKey(GUIDE_STEP);
    if(steps != ""){
        GlobalData::shared()->focusBuildId = -1;
        return;
    }
    if(GlobalData::shared()->focusBuildId != -1){
        this->onMoveToBuild(GlobalData::shared()->focusBuildId);
        GlobalData::shared()->focusBuildId = -1;
        return;
    }
    map<int, FunBuildInfo>::iterator buildingIt;
    for(buildingIt = GlobalData::shared()->imperialInfo.begin(); buildingIt != GlobalData::shared()->imperialInfo.end(); buildingIt++){
        if(buildingIt->second.state == FUN_BUILD_CREATE || buildingIt->second.state == FUN_BUILD_UPING){
            this->onMoveToBuild(buildingIt->second.itemId);
            return;
        }
    }
    
    auto& build = GlobalData::shared()->imperialInfo.find(400000)->second;
    int level = build.level;
    
    auto dict = _dict(LocalController::shared()->DBXMLManager()->getGroupByKey("building")->objectForKey("400000"));
    std::string guideStr = dict->valueForKey("guard")->getCString();
    vector<std::string> bvector;
    vector<std::string> lvector;
    vector<std::string> vector;
    CCCommonUtils::splitString(guideStr, "|", vector);
    
    int buildingId = -1;
    int i = 0;
    while(i < vector.size()){
        std::string levelStr = vector[i];
        lvector.clear();
        CCCommonUtils::splitString(levelStr, ",", lvector);
        if (lvector.size()>=2)
        {
            int xmlLevel = atoi(lvector[0].c_str());
            std::string xmlStr = lvector[1];
            if(xmlLevel == level){
                CCCommonUtils::splitString(xmlStr, ";", bvector);
                int j = 0;
                while (j < bvector.size()) {
                    int xmlBuildingId = atoi(bvector[j].c_str()) / 1000 * 1000;
                    int xmlBuildingLv = atoi(bvector[j].c_str()) - xmlBuildingId;
                    map<int, FunBuildInfo>::iterator it;
                    it = GlobalData::shared()->imperialInfo.find(xmlBuildingId);
                    if(it != GlobalData::shared()->imperialInfo.end()){
                        auto& xmlBuild = it->second;
                        if(xmlBuild.level < xmlBuildingLv && isGeneralOnFormationBuilding(it->first)){
                            buildingId = xmlBuildingId;
                            break;
                        }
                    }
                    j++;
                }
                break;
            }
        }
        i++;
    }
    if(buildingId != -1){
        map<int, CCSafeNode<FunBuild>>::iterator it = m_buildItems.find(buildingId);
        if(it == m_buildItems.end()){
            return;
        }
        
        m_count = 0;
        m_viewPort->applyZoom(1);
        this->onMoveToBuild(buildingId);
        m_touchLayer->runAction(CCSequence::create(CCDelayTime::create(1.05)
                                                   , CCCallFunc::create(this, callfunc_selector(ImperialScene::showAttention1))
                                                   , NULL
                                                   ));
    }
}

bool ImperialScene::isGeneralOnFormationBuilding(int buildingId){
    return true;
}

void ImperialScene::showAttention1(){
    if(m_touchLayer->getScale() > 1.35){
        return;
    }
    if(m_count == 0){
        auto pt = ccp(-m_touchLayer->getPosition().x + CCDirector::sharedDirector()->getWinSize().width / 2
                      , -m_touchLayer->getPosition().y + CCDirector::sharedDirector()->getWinSize().height / 2);
        
        m_viewPort->setNSZoomCenter(pt);
        m_viewPort->setWSZoomCenter(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2));
    }
    m_count++;
    
    m_viewPort->applyZoom(m_touchLayer->getScale() + 0.04);
    m_touchLayer->runAction(CCSequence::create(CCDelayTime::create(0.01)
                                               , CCCallFunc::create(this, callfunc_selector(ImperialScene::showAttention1))
                                               , NULL
                                               ));
}

void ImperialScene::addParticleToBatch(cocos2d::CCParticleSystemQuad *particle) {
    auto batchCount = m_parVec.size();
    while (batchCount--) {
        auto &batch = m_parVec[batchCount];
        if (batch) {
            auto batchBlend = batch->getBlendFunc();
            auto particleBlend = particle->getBlendFunc();
            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                batch->addChild(particle);
                return;
            }
        }
    }
    auto newBatch = ParticleController::createParticleBatch();
    m_waterLayer->addChild(newBatch);
    newBatch->addChild(particle);
    m_parVec.push_back(newBatch);
}

void ImperialScene::addCloudParticleToBatch(cocos2d::CCParticleSystemQuad *particle, bool isTorch) {
    auto batchCount = m_cloudVec.size();
    if(isTorch){
        m_torchPar->addObject(particle);
    }
    while (batchCount--) {
        auto &batch = m_cloudVec[batchCount];
        if (batch) {
            auto batchBlend = batch->getBlendFunc();
            auto particleBlend = particle->getBlendFunc();
            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                batch->addChild(particle);
                return;
            }
        }
    }
    auto newBatch = ParticleController::createParticleBatch();
    m_cloudLayer->addChild(newBatch);
    newBatch->addChild(particle);
    m_cloudVec.push_back(newBatch);
}

void ImperialScene::makeWater(int type)
{
    m_waterLayer->removeAllChildren();
    
    CCObject* pObj;
    CCARRAY_FOREACH(m_river1Array,pObj)
    {
        CCNode* pNode=dynamic_cast<CCNode*>(pObj);
        auto water_root = Sprite::create();
        pNode->addChild(water_root);
        auto per1Spr = CCSprite::create();
        per1Spr->setAnchorPoint(Vec2(0,0));
        CCCommonUtils::makeEffectSpr(per1Spr, "river_01_%d.png", 10, 0.1);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        per1Spr->setBlendFunc({GL_ONE, GL_ONE});
#endif
        water_root->addChild(per1Spr);
    }
}

void ImperialScene::playWaveParticle(float _time)
{
    for (int i=0; i<m_waveVec.size(); i++) {
        m_waveVec[i]->removeFromParent();
    }
    m_waveVec.clear();
    
    CCObject* pObj;
    CCARRAY_FOREACH(m_wave1Array,pObj)
    {
        CCNode* pNode=dynamic_cast<CCNode*>(pObj);
        auto particle = ParticleController::createParticle("Ripple", CCPointZero,70, m_ripple);
        particle->setStartSize(30);
        particle->setStartSizeVar(20);
        particle->setEndSize(60);
        particle->setEndSizeVar(10);
        
        pNode->setScale(2.2);
        pNode->addChild(particle);
        pNode->setRotation(-29);
        m_waveVec.push_back(particle);
    }
    CCARRAY_FOREACH(m_wave2Array,pObj)
    {
        CCNode* pNode=dynamic_cast<CCNode*>(pObj);
        auto particle = ParticleController::createParticle("Ripple", CCPointZero,30, m_ripple);
        
        particle->setStartSize(75);
        particle->setStartSizeVar(30);
        particle->setEndSize(120);
        particle->setEndSizeVar(15);
        
        pNode->addChild(particle);
        pNode->setRotation(-29);
        m_waveVec.push_back(particle);
    }
    CCARRAY_FOREACH(m_wave3Array,pObj)
    {
    }
}

void ImperialScene::showTmpBuild(int pos, int itemId)
{
    m_tmpBuildPos = pos;
    FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[pos]->getChildByTag(pos));
    build->showTmpBuild(itemId);
    if (build->m_tile)
        build->m_tile->setVisible(false);
}

void ImperialScene::hideTmpBuild(int pos)
{
    FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[pos]->getChildByTag(pos));
    if(build){
        build->hideTmpBuild();
        if (build->m_tile)
            build->m_tile->setVisible(true);
    }
}

void ImperialScene::initBigTile()
{
    if (m_exit) {
        return;
    }
    map<int, BigTileInfo>::iterator it=FunBuildController::getInstance()->m_bigTileMap.begin();
    for (; it!=FunBuildController::getInstance()->m_bigTileMap.end(); it++) {
        if(it->second.state == FUN_BUILD_LOCK) {
            int pos = it->second.pos;
            if (m_bigTileNodes.find(pos) != m_bigTileNodes.end()) {
                if(m_bigTileNodes[pos]) {
                    SpeBuild* build = SpeBuild::create(it->first);
                    if (build) {
                        m_bigTileNodes[pos]->addChild(build);
                        
                        int od = m_bigTileNodes[pos]->getZOrder();
                        build->setNamePos(m_bigTileNodes[pos]->getPositionX()
                                          , m_bigTileNodes[pos]->getPositionY(), m_signLayer, m_popLayer, m_arrbatchNode, m_tilebatchNode, od);
                        m_bigTiles[it->first] = build;
                        build->m_key = 1000-od;
                    }
                }
            }
        }
    }
    
    m_flagBuild = SpeBuild::create(SPE_BUILD_FLAG);
    m_flagNode->addChild(m_flagBuild);
    int od = m_flagNode->getZOrder();
    m_flagBuild->setNamePos(m_flagNode->getPositionX(), m_flagNode->getPositionY(), m_signLayer, m_popLayer,m_arrbatchNode, m_tilebatchNode,od);
    
//    if(ActivityController::getInstance()->activityIsOpen()) {
        m_hdBuild = SpeBuild::create(SPE_BUILD_HD);
        m_hdNode->addChild(m_hdBuild);
        int hod = m_hdNode->getZOrder();
        m_hdBuild->setNamePos(m_hdNode->getPositionX(), m_hdNode->getPositionY(), m_signLayer, m_popLayer,m_arrbatchNode, m_resbatchNode, hod);
//    }
    
    if(/* DISABLES CODE */ (false)) {//false
        m_goldMineBuild = SpeBuild::create(SPE_BUILD_GOLD);
        m_goldMineNode->addChild(m_goldMineBuild);
        int hod = m_goldMineNode->getZOrder();
        m_goldMineBuild->setNamePos(m_goldMineNode->getPositionX(), m_goldMineNode->getPositionY(), m_signLayer,m_popLayer, m_arrbatchNode, m_resbatchNode, hod);
    }
    if(true) {// GlobalData::shared()->playerInfo.gmFlag==1
        m_goldBoxBuild = SpeBuild::create(SPE_BUILD_GOLD_BOX);
        m_goldBoxNode->addChild(m_goldBoxBuild);
        int hod = m_goldBoxNode->getZOrder();
        m_goldBoxBuild->setNamePos(m_goldBoxNode->getPositionX(), m_goldBoxNode->getPositionY(), m_signLayer,m_popLayer, m_arrbatchNode, m_buildBatchMap[101], hod);
    }
    if(/* DISABLES CODE */ (false)) {// GlobalData::shared()->playerInfo.gmFlag==1 关闭圣诞树
        m_chrTreeBuild = SpeBuild::create(SPE_BUILD_CHR_TREE);
        m_chrTreeNode->addChild(m_chrTreeBuild);
        int hod = m_chrTreeNode->getZOrder();
        m_chrTreeBuild->setNamePos(m_chrTreeNode->getPositionX(), m_chrTreeNode->getPositionY(), m_signLayer,m_popLayer, m_arrbatchNode, m_chrTreeBatchNode, hod, m_chrTreeBlentBatchNode);
    }
    
//    if(true) {// GlobalData::shared()->playerInfo.gmFlag==1
//        bool tmpShowSpeWillOpen = false;
//        if (FunBuildController::getInstance()->m_tileMap.find(53) == FunBuildController::getInstance()->m_tileMap.end()) {
//            tmpShowSpeWillOpen = true;
//        }else {
//            auto& tileInfo = FunBuildController::getInstance()->m_tileMap[53];
//            if (tileInfo.state == FUN_BUILD_LOCK || tileInfo.xmlOpen==1) {//53号地块不显示，则显示脚手架
//                tmpShowSpeWillOpen = true;
//            }
//        }
//        
//        if (tmpShowSpeWillOpen) {
//            m_willOpenBuild = SpeBuild::create(SPE_BUILD_WILL_OPEN);
//            m_willOpenNode->addChild(m_willOpenBuild);
//            int hod = m_willOpenNode->getZOrder();
//            m_willOpenBuild->setNamePos(m_willOpenNode->getPositionX(), m_willOpenNode->getPositionY(), m_signLayer, m_arrbatchNode, m_buildBatchMap[100], hod);
//        }
//    }
    if (true) {
        m_shipBuild = SpeBuild::create(SPE_BUILD_SHIP);
        m_shipNode->addChild(m_shipBuild);
        int hod = m_shipNode->getZOrder();
        m_shipBuild->setNamePos(m_shipNode->getPositionX(), m_shipNode->getPositionY(), m_signLayer, m_popLayer,m_arrbatchNode, m_chrTreeBatchNode, hod);
        m_shipBuild->updateShipState();
    }
    if(false) {
        m_merchanteBuild = SpeBuild::create(SPE_BUILD_MERCHANTE);
        m_merchanteNode->addChild(m_merchanteBuild);
        int hod = m_merchanteNode->getZOrder();
        m_merchanteBuild->setNamePos(m_merchanteNode->getPositionX(), m_merchanteNode->getPositionY(), m_signLayer, m_popLayer,m_arrbatchNode, m_resbatchNode, hod);
    }
    if(true) {//!GlobalData::shared()->isXiaoMiPlatForm() && GlobalData::shared()->analyticID != "cn1"
        m_xiongdiBuild = SpeBuild::create(SPE_BUILD_XIONGDI);
        m_xiongdiNode->addChild(m_xiongdiBuild);
        int hod = m_xiongdiNode->getZOrder();
        m_xiongdiBuild->setNamePos(m_xiongdiNode->getPositionX(), m_xiongdiNode->getPositionY(), m_signLayer, m_popLayer,m_arrbatchNode, m_resbatchNode, hod);
    }
    if(/* DISABLES CODE */ (false) && FunBuildController::getInstance()->building_Miracle_open) {
        if(FunBuildController::getInstance()->getMainCityLv()>=20 && !GlobalData::shared()->isMaricleunlock){  //--sun3
            m_qijiBuild = SpeBuild::create(SPE_BUILD_QIJI);
            m_qijiBuild->getOrderOfArrival();
            m_qijiNode->addChild(m_qijiBuild,1);
            int hod = m_qijiNode->getZOrder();
            m_qijiBuild->setNamePos(m_qijiNode->getPositionX(), m_qijiNode->getPositionY(), m_signLayer,m_popLayer, m_arrbatchNode, m_resbatchNode, hod);
        }
    }
    if(CCCommonUtils::isTestPlatformAndServer("guide_3") && PortActController::getInstance()->m_newPalyerRDMap.size()>0 && PortActController::getInstance()->m_newLoginDay<7) {
        m_newRDBuild = SpeBuild::create(SPE_BUILD_NEWED);
        m_newRDNode->addChild(m_newRDBuild);
        int hod = m_newRDNode->getZOrder();
        m_newRDBuild->setNamePos(m_newRDNode->getPositionX(), m_newRDNode->getPositionY(), m_signLayer, m_popLayer,m_arrbatchNode, m_resbatchNode, hod);
    }
    if (PortActController::getInstance()->m_isNewTimeRwd) {
        m_cargoBuild = SpeBuild::create(SPE_BUILD_CARGO);
        m_cargoNode->addChild(m_cargoBuild);
        int hod = m_cargoNode->getZOrder();
        m_cargoBuild->setNamePos(m_cargoNode->getPositionX(), m_cargoNode->getPositionY(), m_signLayer, m_popLayer,m_arrbatchNode, m_resbatchNode, hod);
        
//        IFDockWalloperNode* worker = IFDockWalloperNode::create();
//        if (worker) {
//            m_spineLayer->addChild(worker);
//        }
    }
    if (true)
    {
//        m_lotteryBuild = SpeBuild::create(SPE_BUILD_LOTTERY);
//        m_lotteryNode->addChild(m_lotteryBuild);
//        int hod = m_lotteryNode->getZOrder();
//        m_lotteryBuild->setNamePos(m_lotteryNode->getPositionX(), m_lotteryNode->getPositionY(), m_signLayer, m_arrbatchNode, m_resbatchNode, hod);
    }
    if(ActivityController::getInstance()->checkHaveDragonActivity() && !CCCommonUtils::isKuaFuWangZhan()) {
        m_dragonBuild = SpeBuild::create(SPE_BUILD_DRAGON);
        m_dragonNode->addChild(m_dragonBuild);
        int hod = m_dragonNode->getZOrder();
        m_dragonBuild->setNamePos(m_dragonNode->getPositionX(), m_dragonNode->getPositionY(), m_signLayer, m_popLayer,m_arrbatchNode, m_resbatchNode, hod);
    }
    
    if (FunBuildController::getInstance()->getMainCityLv() >= 3 && CCCommonUtils::isTestPlatformAndServer("Npc_princess") && !FunBuildController::getInstance()->IsHaveRecordeByKey(CLIENT_RECORD_PRINESS)) {//小公主动画非引导触发打点
        FunBuildController::getInstance()->SendRecordeToServer(CLIENT_RECORD_PRINESS);
    }
//    onShowPrincess(0);
}

void ImperialScene::onPrincessAniFuc(CCObject* obj)
{
    CCInteger* integerNum = dynamic_cast<CCInteger*>(obj);
    if (integerNum) {
        int num = integerNum->getValue();
        if (num == 1) {
            m_princessAni->setAnimation(0, "dj", true);
        }else if (num == 2) {
            m_princessAni->setAnimation(0, "zl", true);
        }else if (num == 3) {
            m_princessAni->setAnimation(0, "zl_1", true);
        }
    }
}

void ImperialScene::onShowPrincess(int type)
{
    if (!FunBuildController::getInstance()->IsHaveRecordeByKey(CLIENT_RECORD_PRINESS)) {
        return;
    }
    if (!CCCommonUtils::isTestPlatformAndServer("Npc_princess")) {
        return;
    }
    
    if (m_princess1Node) {
        m_princess1Node->removeFromParent();
    }
    if (m_princess2Node) {
        m_princess2Node->removeFromParent();
    }
    if (m_princess3Node) {
        m_princess3Node->removeFromParent();
    }
    m_princess1Node = NULL;
    m_princess2Node = NULL;
    m_princess3Node = NULL;
    int st = rand()%3;
    if (st == 0 || type == 1)
    {
        if (CCFileUtils::sharedFileUtils()->isFileExist("Spine/Imperial/xgz_zc_daiji.json") &&
            CCFileUtils::sharedFileUtils()->isFileExist("Imperial/Imperial_30.atlas"))
        {
            m_princess2Node = Node::create();
            m_princess2Node->setContentSize(CCSize(120,100));
            auto animationObj = new IFSkeletonAnimation("Spine/Imperial/xgz_zc_daiji.json","Imperial/Imperial_30.atlas");
            animationObj->setPosition(ccp(100, -30));
            m_princess2Node->addChild(animationObj);
            m_princess2Node->setPosition(ccp(1080, 1450));
            m_spineLayer->addChild(m_princess2Node);
            animationObj->setToSetupPose();
            spTrackEntry* entry = animationObj->setAnimation(0, "animation", true);
            animationObj->setTimeScale(entry->endTime/2);
            animationObj->autorelease();
        }
    }
    else if (st == 1) {
        if (CCFileUtils::sharedFileUtils()->isFileExist("Spine/Imperial/xgz.json") &&
            CCFileUtils::sharedFileUtils()->isFileExist("Imperial/Imperial_30.atlas"))
        {
            m_princessAni = NULL;
            m_princess1Node = Node::create();
            m_princess1Node->setContentSize(CCSize(50,50));
            m_princessAni = new IFSkeletonAnimation("Spine/Imperial/xgz.json","Imperial/Imperial_30.atlas");
            m_princessAni->setPosition(ccp(20, 0));
            m_princess1Node->setPosition(ccp(460, 680));
            m_spineLayer->addChild(m_princess1Node);
            m_princess1Node->addChild(m_princessAni);
            m_princessAni->setToSetupPose();
            m_princessAni->setAnimation(0, "zl_1", true);
            m_princessAni->autorelease();
            
            CCActionInterval * moveTo1 = CCMoveTo::create(12,ccp(660, 580));
            CCActionInterval * moveTo2 = CCMoveTo::create(12,ccp(460, 680));
            CCActionInterval * delay1 = CCDelayTime::create(2);
            CCActionInterval * delay2 = CCDelayTime::create(2);
            CCCallFuncO* fun1 = CCCallFuncO::create(this, callfuncO_selector(ImperialScene::onPrincessAniFuc), CCInteger::create(1));
            CCCallFuncO* fun2 = CCCallFuncO::create(this, callfuncO_selector(ImperialScene::onPrincessAniFuc), CCInteger::create(2));
            CCCallFuncO* fun3 = CCCallFuncO::create(this, callfuncO_selector(ImperialScene::onPrincessAniFuc), CCInteger::create(1));
            CCCallFuncO* fun4 = CCCallFuncO::create(this, callfuncO_selector(ImperialScene::onPrincessAniFuc), CCInteger::create(3));
            CCSequence* aniSequence = CCSequence::create(moveTo1,fun1,delay1,fun2,moveTo2,fun3,delay2,fun4, NULL);
            m_princess1Node->runAction(CCRepeatForever::create(aniSequence));
        }
    }
    else {
        if (CCFileUtils::sharedFileUtils()->isFileExist("Spine/Imperial/xiaogongzhuyewai_.json") &&
            CCFileUtils::sharedFileUtils()->isFileExist("Imperial/Imperial_30.atlas"))
        {
            m_princessShotAni = NULL;
            m_princess3Node = Node::create();
            m_princess3Node->setContentSize(CCSize(300,400));
            m_princessShotAni = new IFSkeletonAnimation("Spine/Imperial/xiaogongzhuyewai_.json","Imperial/Imperial_30.atlas");
            m_princessShotAni->setPosition(ccp(-10, 90));
            m_princess3Node->addChild(m_princessShotAni);
            m_princess3Node->setPosition(ccp(3260, 1165));//3360, 510
            m_spineLayer->addChild(m_princess3Node);
            m_princessShotAni->setToSetupPose();
            m_princessShotAni->setAnimation(0, "dj", true);
            m_princessShotAni->autorelease();
        }
    }
}

void ImperialScene::showPrincessByMsg(CCObject *obj){
    if (CCCommonUtils::isTestPlatformAndServer("Npc_princess") && !FunBuildController::getInstance()->IsHaveRecordeByKey(CLIENT_RECORD_PRINESS) && m_princessWalkNode == nullptr && !GuideController::share()->isInTutorial() && m_curGuideEnd == false){
        scheduleOnce(schedule_selector(ImperialScene::princessShowWithTransform), 0.1);
        scheduleOnce(schedule_selector(ImperialScene::princessShowOnByMsg), 1.1);
        GuideController::share()->setLagStopTouch(7.5);
    }
}

void ImperialScene::newrwdGuideEndConfirm(CCObject *obj)
{
    if (CCCommonUtils::isTestPlatformAndServer("Npc_princess") && !FunBuildController::getInstance()->IsHaveRecordeByKey(CLIENT_RECORD_PRINESS) && m_princessWalkNode == nullptr && !GuideController::share()->isInTutorial()){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_NEWRD_GUIDE_ANIM, Integer::create(0));
        scheduleOnce(schedule_selector(ImperialScene::princessShowOnByEndGuide), 3.0);
        GuideController::share()->setLagStopTouch(9.0);
    }
    else
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_NEWRD_GUIDE_ANIM, Integer::create(1));
}

void ImperialScene::princessShowOnByMsg(float dt){
    onStartShowPrincess(1);
}

void ImperialScene::princessShowOnByEndGuide(float dt){
    onStartShowPrincess(0);
}

void ImperialScene::princessShowWithTransform(float t)
{
    TransitionLayer::create();
}

void ImperialScene::onStartShowPrincess(int forceType)
{
    if (CCCommonUtils::isTestPlatformAndServer("Npc_princess") && !FunBuildController::getInstance()->IsHaveRecordeByKey(CLIENT_RECORD_PRINESS) && m_princessWalkNode == nullptr && !GuideController::share()->isInTutorial() && m_curGuideEnd == false && m_princessWalkStart == false) {
        FunBuildInfo& main = FunBuildController::getInstance()->getFunbuildById(FUN_BUILD_MAIN_CITY_ID);
        if( PopupViewController::getInstance()->getCurrViewCount()==0 && (main.level >= 3 || forceType != -1)) {
            m_princessWalkStart = true;
            if (forceType == -1) {
                GuideController::share()->setLagStopTouch(6);
            }
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PERSON_VISIBLE, Integer::create(0));
            UIComponent::getInstance()->setVisible(false);
            showExtraBtns(false);
            onMoveToPos(1920, 920, TYPE_POS_MID, 0.0, 1.5, true);
            if (forceType == 1) {//正常点升级按钮时触发小公主入城动画为1，引导结束触发为0，默认为-1
                scheduleOnce(schedule_selector(ImperialScene::onShowPrincessWalk), 1.6);
            }
            else
                scheduleOnce(schedule_selector(ImperialScene::onShowPrincessWalk), 0.1);
        }
    }
}
void ImperialScene::onShowPrincessWalk(float dt)
{
    onCleanPrincessWalk();
    if (CCFileUtils::sharedFileUtils()->isFileExist("Spine/Imperial/xiaogongzhu.json") &&
        CCFileUtils::sharedFileUtils()->isFileExist("Imperial/Imperial_30.atlas"))
    {
        auto pt = ccp(2230, 670);//2510, 490
        m_prinessPersonArray = CCArray::create();
        m_princessWalkNode = Node::create();
        
        auto tmpPt = ccp(-10, -30);
        for (int i=0; i<3; i++) {
            auto m_per1Spr = CCSprite::create();
            m_per1Spr->setPosition(tmpPt.x+10+i*20,tmpPt.y-5+i*(-10));
            m_per1Spr->setFlipX(true);
            CCCommonUtils::makeEffectSpr(m_per1Spr, "bubing_0_%d.png", 8, 0.1);
            m_prinessPersonArray->addObject(m_per1Spr);
            m_princessWalkNode->addChild(m_per1Spr);
        }
        
        m_princessWalkAni = new IFSkeletonAnimation("Spine/Imperial/xiaogongzhu.json","Imperial/Imperial_30.atlas");
        m_princessWalkNode->addChild(m_princessWalkAni);
        m_princessWalkNode->setPosition(pt);
        m_spineLayer->addChild(m_princessWalkNode);
        m_princessWalkAni->setToSetupPose();
        m_princessWalkAni->setAnimation(0, "walk", true);
        m_princessWalkAni->autorelease();
        
        for (int i=0; i<3; i++) {
            auto m_per1Spr = CCSprite::create();
            m_per1Spr->setPosition(tmpPt.x-18+i*20,tmpPt.y-25+i*(-10));
            m_per1Spr->setFlipX(true);
            CCCommonUtils::makeEffectSpr(m_per1Spr, "bubing_0_%d.png", 8, 0.1);
            m_prinessPersonArray->addObject(m_per1Spr);
            m_princessWalkNode->addChild(m_per1Spr);
        }
        
        CCActionInterval * moveTo1 = CCMoveTo::create(5,ccp(1990, 795));//2150, 670
        CCActionInterval * delay1 = CCDelayTime::create(0.5);
        CCCallFunc* fun1 = CCCallFunc::create(this, callfunc_selector(ImperialScene::onStopPrincessWalk));
        CCCallFunc* fun2 = CCCallFunc::create(this, callfunc_selector(ImperialScene::onShowPrincessWalkTalk));
        CCSequence* aniSequence = CCSequence::create(moveTo1,fun1,delay1,fun2, NULL);
        m_princessWalkNode->runAction(aniSequence);
    }
}
void ImperialScene::onStopPrincessWalk() {
    
    if (m_princessWalkAni) {
        m_princessWalkAni->setToSetupPose();
        m_princessWalkAni->setAnimation(0, "waiting", true);
    }
    for (int i=0; i<m_prinessPersonArray->count(); i++) {
        CCSprite* node = dynamic_cast<CCSprite*>(m_prinessPersonArray->objectAtIndex(i));
        if (node) {
            node->stopAllActions();
            auto frame = CCLoadSprite::loadResource("bubing_0_7.png");
            if (frame) {
                node->setDisplayFrame(frame);
            }
        }
    }
}
void ImperialScene::onShowPrincessWalkTalk()
{
    vector<string> tmpTalkInfos;
    tmpTalkInfos.push_back(_lang("3140011"));
    tmpTalkInfos.push_back(_lang("3140013"));
    auto view = NpcTalkView::create(tmpTalkInfos);
    view->setCallback(this, callfunc_selector(ImperialScene::onPrincessWalkTalkBack));
    PopupViewController::getInstance()->addPopupView(view);
    FunBuildController::getInstance()->SendRecordeToServer(CLIENT_RECORD_PRINESS);
}
void ImperialScene::onPrincessWalkTalkBack()
{
    TransitionLayer::create();
    scheduleOnce(schedule_selector(ImperialScene::onPrincessWalkEnd), 1.5);
}
void ImperialScene::onPrincessWalkEnd(float dt)
{
    onCleanPrincessWalk();
    onShowPrincess(1);
    UIComponent::getInstance()->setVisible(true);
    m_princessWalkStart = false;
    onMoveToPos(1100, 1580, TYPE_POS_MID, 0.0, 1.5, true);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PERSON_VISIBLE, Integer::create(1));
    showExtraBtns(true);
    scheduleOnce(schedule_selector(ImperialScene::princessWalkEndStartTalk), 1.0);
}
void ImperialScene::princessWalkEndStartTalk(float t)
{
    vector<string> tmpTalkInfos;
    tmpTalkInfos.push_back(_lang("3140015"));
    auto view = NpcTalkView::create(tmpTalkInfos);
    view->setCallback(this, callfunc_selector(ImperialScene::onShowTalkViewOnce));
    PopupViewController::getInstance()->addPopupView(view);
}
void ImperialScene::onCleanPrincessWalk()
{
//    if (m_princessWalkAni) {
//        m_princessWalkAni->setToSetupPose();
//        m_princessWalkAni->removeFromParent();
//        m_princessWalkAni = NULL;
//    }
//    if (m_prinessPersonArray) {
//        for (int i=0; i<m_prinessPersonArray->count(); i++) {
//            CCNode* node = dynamic_cast<CCNode*>(m_prinessPersonArray->objectAtIndex(i));
//            if (node) {
//                node->removeFromParent();
//            }
//        }
//        m_prinessPersonArray->removeAllObjects();
//        m_prinessPersonArray = NULL;
//    }
//    if (m_princessWalkNode) {
//        m_princessWalkNode->removeFromParent();
//        m_princessWalkNode = NULL;
//    }
    if (m_princessWalkNode) {
        m_princessWalkNode->setVisible(false);
    }
}

void ImperialScene::unLockTile(CCObject* params)
{
    if(params) {
        int itemId = dynamic_cast<CCInteger*>(params)->getValue();
        map<int, CCSafeNode<SpeBuild>>::iterator it = m_bigTiles.find(itemId);
        if (it != m_bigTiles.end()) {
            m_removeSpeBId = itemId;
            it->second->onPlayUnlock();
        }
    }
}

void ImperialScene::onRemoveSpeBuild()
{
    int itemId = m_removeSpeBId;
    map<int, CCSafeNode<SpeBuild>>::iterator it = m_bigTiles.find(itemId);
    if (it != m_bigTiles.end()) {
        it->second->onBuildDelete();
        int pos = FunBuildController::getInstance()->m_bigTileMap[itemId].pos;
        m_bigTileNodes[pos]->removeAllChildren();
        m_bigTiles.erase(it);
    }
}

void ImperialScene::refreashTile()
{
    map<int, BigTileInfo>::iterator it=FunBuildController::getInstance()->m_bigTileMap.begin();
    for (; it!=FunBuildController::getInstance()->m_bigTileMap.end(); it++) {
        if(it->second.state == FUN_BUILD_LOCK && it->second.level<=FunBuildController::getInstance()->getMainCityLv()) {
            int itemId = it->first;
            int pos = it->second.pos;
            
            map<int, CCSafeNode<SpeBuild>>::iterator tileIt = m_bigTiles.find(itemId);
            if (tileIt != m_bigTiles.end() && tileIt->second->m_isOpen==false)
            {
                tileIt->second->onBuildDelete();
                m_bigTileNodes[pos]->removeAllChildren();
                m_bigTiles.erase(tileIt);
                
                if (m_bigTileNodes.find(pos) != m_bigTileNodes.end()) {
                    
                    SpeBuild* build = SpeBuild::create(it->first);
                    m_bigTileNodes[pos]->addChild(build);
                    
                    int od = m_bigTileNodes[pos]->getZOrder();
                    build->setNamePos(m_bigTileNodes[pos]->getPositionX()
                                      , m_bigTileNodes[pos]->getPositionY(), m_signLayer, m_popLayer, m_arrbatchNode, m_tilebatchNode, od);
                    m_bigTiles[it->first] = build;
                    build->m_key = 1000-od;
                }
            }
        }
    }
}

void ImperialScene::setMakeEffectSpr(CCSprite* spr,string direction,int animationType)
{
    if(animationType==1||animationType==3)//牛夫
    {
        if (direction=="t")
        {
            CCCommonUtils::makeEffectSpr(spr, "laonongheniu_3_%d.png", 9, 0.1);
        }
        else if(direction=="r")
        {
            CCCommonUtils::makeEffectSpr(spr, "laonongheniu_%d.png", 9, 0.1);
        }
        else if(direction=="d")
        {
            CCCommonUtils::makeEffectSpr(spr, "laonongheniu_2_%d.png", 9, 0.1);
        }
        else if(direction=="l")
        {
            CCCommonUtils::makeEffectSpr(spr, "laonongheniu_1_%d.png", 9, 0.1);
        }
    }
    else if(animationType==6||animationType==7||animationType==10||animationType==11)//市民
    {
        if (direction=="t")
        {
            spr->setFlipX(true);
            CCCommonUtils::makeEffectSpr(spr, "nongminzoulu2_%d.png", 10, 0.1);
        }
        else if(direction=="r")
        {
            spr->setFlipX(false);
            CCCommonUtils::makeEffectSpr(spr, "nongminzoulu1_%d.png", 10, 0.1);
        }
        else if(direction=="d")
        {
            spr->setFlipX(true);
            CCCommonUtils::makeEffectSpr(spr, "nongminzoulu1_%d.png", 10, 0.1);
        }
        else if(direction=="l")
        {
            spr->setFlipX(false);
            CCCommonUtils::makeEffectSpr(spr, "nongminzoulu2_%d.png", 10, 0.1);
        }
        
    }else if(animationType==4||animationType==12||animationType==13||animationType==16||animationType==17||animationType==18||animationType==19)//巡逻兵
    {
        if (direction=="t")
        {
            spr->setFlipX(true);
            CCCommonUtils::makeEffectSpr(spr, "xunluobing1_%d.png", 10, 0.1);
        }
        else if(direction=="r")
        {
            spr->setFlipX(false);
            CCCommonUtils::makeEffectSpr(spr, "xunluobing_%d.png", 10, 0.1);
        }
        else if(direction=="d")
        {
            spr->setFlipX(true);
            CCCommonUtils::makeEffectSpr(spr, "xunluobing_%d.png", 10, 0.1);
        }
        else if(direction=="l")
        {
            spr->setFlipX(false);
            CCCommonUtils::makeEffectSpr(spr, "xunluobing1_%d.png", 10, 0.1);
        }

    }else if(animationType==2||animationType==5||animationType==14){//挑夫
        if (direction=="t")
        {
            spr->setFlipX(true);
            CCCommonUtils::makeEffectSpr(spr, "tiaobiandanderen_%d.png", 10, 0.1);
        }
        else if(direction=="r")
        {
            spr->setFlipX(false);
            CCCommonUtils::makeEffectSpr(spr, "tiaobiandanderen1_%d.png", 10, 0.1);
        }
        else if(direction=="d")
        {
            spr->setFlipX(true);
            CCCommonUtils::makeEffectSpr(spr, "tiaobiandanderen1_%d.png", 10, 0.1);
        }
        else if(direction=="l")
        {
            spr->setFlipX(false);
            CCCommonUtils::makeEffectSpr(spr, "tiaobiandanderen_%d.png", 10, 0.1);
        }
    }else if(animationType==8){//主公
        if (direction=="t")
        {
            spr->setFlipX(true);
            CCCommonUtils::makeEffectSpr(spr, "zhucheng_ren1_0_%d.png", 10, 0.1);
        }
        else if(direction=="r")
        {
            spr->setFlipX(false);
            CCCommonUtils::makeEffectSpr(spr, "zhucheng_ren_%d.png", 10, 0.1);
        }
        else if(direction=="d")
        {
            spr->setFlipX(true);
            CCCommonUtils::makeEffectSpr(spr, "zhucheng_ren_%d.png", 10, 0.1);
        }
        else if(direction=="l")
        {
            spr->setFlipX(false);
            CCCommonUtils::makeEffectSpr(spr, "zhucheng_ren1_0_%d.png", 10, 0.1);
        }
    }else if(animationType==9){//主公马车
        if (direction=="t")
        {
            spr->setFlipX(true);
            CCCommonUtils::makeEffectSpr(spr, "zhuchengmache1_%d.png", 8, 0.1);
        }
        else if(direction=="r")
        {
            spr->setFlipX(false);
            CCCommonUtils::makeEffectSpr(spr, "zhuchengmache1_%d.png", 8, 0.1);
        }
        else if(direction=="d")
        {
            spr->setFlipX(true);
            CCCommonUtils::makeEffectSpr(spr, "zhuchengmache0_%d.png", 8, 0.1);
        }
        else if(direction=="l")
        {
            spr->setFlipX(false);
            CCCommonUtils::makeEffectSpr(spr, "zhuchengmache0_%d.png", 8, 0.1);
        }
    }else if(animationType==15){//鸟
        if(direction=="l")
        {
            spr->setFlipX(false);
            CCCommonUtils::makeEffectSpr(spr, "niao_%d.png", 9, 0.1);
        }
    }else if(animationType==20){//农民
        if (direction=="t")
        {
            spr->setFlipX(true);
            CCCommonUtils::makeEffectSpr(spr, "cm2_%d.png", 10, 0.1);
        }
        else if(direction=="r")
        {
            spr->setFlipX(false);
            CCCommonUtils::makeEffectSpr(spr, "cm2_%d.png", 10, 0.1);
        }
        else if(direction=="d")
        {
            spr->setFlipX(true);
            CCCommonUtils::makeEffectSpr(spr, "cm1_%d.png", 10, 0.1);
        }
        else if(direction=="l")
        {
            spr->setFlipX(false);
            CCCommonUtils::makeEffectSpr(spr, "cm1_%d.png", 10, 0.1);
        }
    }else if(animationType==21||animationType==22){//祭祀
        if (direction=="t")
        {
            spr->setFlipX(true);
            CCCommonUtils::makeEffectSpr(spr, "jitai_zoulu_2_%d.png", 10, 0.1);
        }
        else if(direction=="r")
        {
            spr->setFlipX(false);
            CCCommonUtils::makeEffectSpr(spr, "jitai_zoulu_1_%d.png", 10, 0.1);
        }
        else if(direction=="d")
        {
            spr->setFlipX(true);
            CCCommonUtils::makeEffectSpr(spr, "jitai_zoulu_1_%d.png", 10, 0.1);
        }
        else if(direction=="l")
        {
            spr->setFlipX(false);
            CCCommonUtils::makeEffectSpr(spr, "jitai_zoulu_2_%d.png", 10, 0.1);
        }
    }
}

void ImperialScene::onClearAnimationCallback(Node* animationNode)
{
    auto tag=animationNode->getTag();
    auto animationSprite=dynamic_cast<CCSprite*>(animationNode->getChildByName("animationSprite"));
    if(tag==21)//祭祀
    {
        CCSprite* spr=animationSprite;
        auto animate=CCCommonUtils::makeEffectAnimate(animationSprite,"jitai_jitian_%d.png",16, 0.1,0);
        auto callback1 = [=](){
            createAnimation(22,BATCHNODE_ORDER_CITY_GATE3,30,0,0,false);//祭祀
        };
        auto callback2 = [=](){
            spr->removeFromParentAndCleanup(true);
        };
        CallFunc* deleteSpr1 = CallFunc::create(callback1);
        CallFunc* deleteSpr2 = CallFunc::create(callback2);
        CCSequence* sequence = CCSequence::create(animate,animate,animate,deleteSpr1,CCDelayTime::create(0.1),deleteSpr2,NULL);
        spr->stopAllActions();
        spr->runAction(sequence);
    }else if (tag==22)
    {
        animationSprite->removeFromParentAndCleanup(true);
        createAnimation(21,BATCHNODE_ORDER_CITY_GATE3,30,0,3,false);//祭祀
    }
}

void ImperialScene::onChangeAnimationCallback(Node* animationNode)
{
    auto animationSprite=dynamic_cast<CCSprite*>(animationNode->getChildByName("animationSprite"));
    auto count=animationSprite->getTag();
    auto tag=animationNode->getTag();
    auto num=m_animationAllPos[tag]->count();
    auto targetPos=m_animationAllPos[tag]->getControlPointAtIndex(count%num);
    auto currentPos=animationNode->getPosition();
    auto direction=targetPos-currentPos;//方向
    auto distance=currentPos.distance(targetPos);//距离
    auto posNodes=m_animationPosNodes[tag];
    auto index=((count-1)>0?(count-1):0)%(posNodes.size());
    auto posNode=posNodes[index];
    if(tag==8)//主公出城
    {
        auto m_perSpr =animationNode->getParent()->getChildByName("zhuchengmache_xiache");
        if(!m_perSpr)
        {
            m_perSpr = CCLoadSprite::createSprite("zhuchengmache_xiache.png");//马车
            m_perSpr->setName("zhuchengmache_xiache");
            animationNode->getParent()->addChild(m_perSpr,animationNode->getZOrder());
            m_perSpr->setPosition(posNodes[2]->getPosition());
        }
        
        if(index==0||index==1||index==9||index==10)
        {
            tag=8;
        }else
        {
            tag=9;
        }
        
        if(index==0)//起点
        {
            if(m_perSpr)
            {
                m_perSpr->setVisible(true);
            }
        }else if(index==2)//换车动画
        {
            if(m_perSpr)
            {
                m_perSpr->setVisible(false);
            }
        }else if(index==9)//车消失
        {
            if(m_perSpr)
            {
                m_perSpr->setVisible(true);
            }
        }
    }
    if(!posNode->isVisible())
    {
        animationSprite->runAction(CCFadeOut::create(0.2));
    }
    count=count+1;
    animationSprite->setTag(count);
    if(distance<1||!posNode->isVisible())
    {
        return;
    }
    string type="t";
    if(direction.x>=0&&direction.y>=0)//上
    {
        type="t";
    }
    else if(direction.x>=0&&direction.y<0)//右
    {
        type="r";
    }
    else if(direction.x<0&&direction.y<0)//下
    {
        type="d";
    }
    else if(direction.x<0&&direction.y>=0)//左
    {
        type="l";
    }
    setMakeEffectSpr(animationSprite,type,tag);
    int Opacity=animationSprite->getOpacity();
    if (Opacity==0)
    {
        animationSprite->runAction(CCFadeIn::create(0.2));
    }
}

void ImperialScene::addAnimation(AnimationInfo animationInfo)
{
    int index=animationInfo.index;
    int zOrder=animationInfo.zOrder;
    int speed=animationInfo.speed;
    float intervalTiem=animationInfo.intervalTiem;
    bool isRepeat=animationInfo.isRepeat;
    
    if(!m_animationNodes[index]->getChildByName("animationSprite"))
    {
        auto m_perSpr = CCSprite::create();
        m_perSpr->setTag(0);
        m_perSpr->setName("animationSprite");
        m_animationNodes[index]->setZOrder(zOrder);
        m_animationNodes[index]->setTag(index);
        m_animationNodes[index]->addChild(m_perSpr);
    }
   
    Vector<FiniteTimeAction*> arrayOfActions;
    auto animationPos=m_animationAllPos[index];
    auto topPos=animationPos->getControlPointAtIndex(0);
    for (int i=0; i<animationPos->count(); i++) {
        auto distance=topPos.distance(animationPos->getControlPointAtIndex(i));
        auto duration=distance/speed==0?0.01:distance/speed;
        topPos=animationPos->getControlPointAtIndex(i);
        auto moveTo = CCMoveTo::create(duration,animationPos->getControlPointAtIndex(i));
        auto func = CCCallFuncN::create(this, callfuncN_selector(ImperialScene::onChangeAnimationCallback));
        arrayOfActions.pushBack(func);
        arrayOfActions.pushBack(moveTo);
    }
    if(intervalTiem>0)
    {
        arrayOfActions.pushBack(CCDelayTime::create(intervalTiem));
    }
    if(isRepeat)
    {
        CCSequence* sequenceAction = CCSequence::create(arrayOfActions);
        m_animationNodes[index]->runAction(CCRepeatForever::create(sequenceAction));
    }else
    {
        auto func = CCCallFuncN::create(this, callfuncN_selector(ImperialScene::onClearAnimationCallback));
        arrayOfActions.pushBack(func);
        CCSequence* sequenceAction = CCSequence::create(arrayOfActions);
        m_animationNodes[index]->runAction(sequenceAction);
    }
}

void ImperialScene::createAnimationCallback(Node* node)
{
    auto index=node->getTag();
    auto animationInfo=m_animationInfos[index];
    addAnimation(animationInfo);
    node->removeFromParent();
}
void ImperialScene::createAnimation(int index,int zOrder,int speed,float intervalTiem,float delayTime,bool isRepeat)
{
    int count=(int)m_animationInfos.size();
    AnimationInfo info;
    info.index=index;
    info.zOrder=zOrder;
    info.speed=speed;
    info.intervalTiem=intervalTiem;
    info.delayTime=delayTime;
    info.isRepeat=isRepeat;
    m_animationInfos[count]=info;
    auto node=CCNode::create();
    node->setTag(count);
    this->addChild(node);
    auto func = CCCallFuncN::create(this, callfuncN_selector(ImperialScene::createAnimationCallback));
    node->runAction(CCSequence::create(CCDelayTime::create(delayTime),func,NULL));
}

void ImperialScene::setSequenceAnimation(CCSprite * spr)
{
    auto FileName=spr->displayFrame()->getTextureFileName();
    if(FileName=="chuniang_1.png")
    {
        CCCommonUtils::makeInternalEffectSpr(spr,"chuniang_%d.png", 14, 0.1,1,0.5);
    }else if(FileName=="dianpumenkou_1.png")
    {
        CCCommonUtils::makeInternalEffectSpr(spr,"dianpumenkou_%d.png", 13, 0.1,1,0.5);
    }else if(FileName=="furen_1.png")
    {
        CCCommonUtils::makeInternalEffectSpr(spr,"furen_%d.png", 44, 0.1,1,0.5);
    }else if(FileName=="saodilaoren_1.png")
    {
        CCCommonUtils::makeInternalEffectSpr(spr,"saodilaoren_%d.png", 41, 0.1,1,0.5);
    }else if(FileName=="saodilaoren1_1.png")
    {
        CCCommonUtils::makeInternalEffectSpr(spr,"saodilaoren1_%d.png", 41, 0.1,1,0.5);
    }else if(FileName=="xiaohai_fangentou_1.png")
    {
        CCCommonUtils::makeInternalEffectSpr(spr,"xiaohai_fangentou_%d.png", 50, 0.1,1,0.5);
    }else if(FileName=="xtg_1.png")
    {
        CCCommonUtils::makeInternalEffectSpr(spr,"xtg_%d.png", 25, 0.1,1,0.5);
    }else if(FileName=="xtg1_1.png")
    {
        CCCommonUtils::makeInternalEffectSpr(spr,"xtg1_%d.png", 24, 0.1,1,0.5);
    }else if(FileName=="xtg1_1.png")
    {
        CCCommonUtils::makeInternalEffectSpr(spr,"xtg1_%d.png", 25, 0.1,1,0.5);
    }else if(FileName=="yaodianmenkoudelaofuren_1.png")
    {
        CCCommonUtils::makeInternalEffectSpr(spr,"yaodianmenkoudelaofuren_%d.png", 42, 0.1,1,0.5);
    }else if(FileName=="yizhan_ma_20_1.png")
    {
        CCCommonUtils::makeInternalEffectSpr(spr,"yizhan_ma_20_%d.png", 22, 0.1,1,0.5);
    }else if(FileName=="zuigui_1.png")
    {
        CCCommonUtils::makeInternalEffectSpr(spr,"zuigui_%d.png", 48, 0.1,1,0.5);
    }
}

void ImperialScene::initAnimation()
{
    auto resSp = CCLoadSprite::loadResource("res_tile_1.png");//Imperial_2
    resSp->getTexture()->setAntiAliasTexParameters();
    auto batchNode =CCSpriteBatchNode::createWithTexture(resSp->getTexture());
    m_touchLayer->addChild(batchNode,BATCHNODE_ORDER_NPC);//巡逻兵层
    
    //固定动画
    auto children = this->m_animation_Stand->getChildren();
    for(auto iter : children)
    {
        CCSprite *child  = dynamic_cast<CCSprite *>(iter);
        if (child){
            child->removeFromParent();
            batchNode->addChild(child);
        }
    }
    //序列动画
    children = this->m_animation_sequence->getChildren();
    for(auto iter : children)
    {
        CCSprite *child  = dynamic_cast<CCSprite *>(iter);
        if (child){
            child->removeFromParent();
            batchNode->addChild(child);
            if(child->getTag()==-1)
            {
                setSequenceAnimation(child);
            }else if(child->getTag()==1)//武将
            {
                auto daiji=CCCommonUtils::makeEffectAnimate(child,"dianjiangtai_dj_%d.png",8, 0.2,1);
                auto huidao=CCCommonUtils::makeEffectAnimate(child,"dianjiangtai_jj_hh_%d.png", 15, 0.2,1);
                auto sequence=CCSequence::create(daiji,daiji,daiji,daiji,daiji,daiji,daiji,daiji,huidao,huidao,NULL);
                CCRepeatForever* action = CCRepeatForever::create(sequence);
                child->runAction(action);
            }
        }
    }

    //移动动画
    for(auto it = m_animationNodes.begin(); it != m_animationNodes.end(); it++){
        auto pNode=it->second;
        pNode->removeFromParent();
        m_touchLayer->addChild(pNode,BATCHNODE_ORDER_CITY_GATE3);
        auto pPointArray=CCPointArray::create(10);
        int count=(int)(pNode->getChildrenCount());
        for (int i=0;i<count;i++)
        {
            auto posNode=pNode->getChildByTag(i+1);
            if(posNode)
            {
                pPointArray->addControlPoint(posNode->getPosition());
                m_animationPosNodes[it->first].push_back(posNode);
            }
        }
        m_animationAllPos[it->first]=pPointArray;
        if(it->first!=13
           &&it->first!=16
           &&it->first!=17
           &&it->first!=18
           &&it->first!=19
           &&it->first!=1
           &&it->first!=2
           &&it->first!=12
           &&it->first!=8
           &&it->first!=21
           &&it->first!=22
           &&it->first!=15
           &&it->first!=23
           &&it->first!=24
           )//非循环线路
        {
            auto reversePointArray=pPointArray->reverse();
            for(int i=0;i<reversePointArray->count();i++)
            {
                m_animationAllPos[it->first]->addControlPoint(reversePointArray->getControlPointAtIndex(i));
            }
        }
    }
    
    auto speed=30;
    createAnimation(1,BATCHNODE_ORDER_CITY_GATE3+1,20,0.5,0);//牛
    createAnimation(2,BATCHNODE_ORDER_CITY_GATE3,speed,0.5,0);
    createAnimation(3,BATCHNODE_ORDER_CITY_GATE3,20,0.5,0);//牛
    createAnimation(4,BATCHNODE_ORDER_CITY_GATE3,speed,0.5,0);
    createAnimation(5,BATCHNODE_ORDER_CITY_GATE3,speed,0.5,0);
    createAnimation(6,BATCHNODE_ORDER_CITY_GATE3,speed,0.5,0);
    createAnimation(7,BATCHNODE_ORDER_CITY_GATE3,speed,0.5,0);

    createAnimation(10,BATCHNODE_ORDER_CITY_GATE3,speed,0.5,0);
    createAnimation(11,BATCHNODE_ORDER_CITY_GATE3,speed,0.5,0);
    createAnimation(12,BATCHNODE_ORDER_CITY_GATE3,speed,0.5,0);
    
    createAnimation(14,BATCHNODE_ORDER_CITY_GATE1+1,speed,0.5,0);
    createAnimation(8,BATCHNODE_ORDER_CITY_GATE1+1,35,30,0);//主公
    createAnimation(15,BATCHNODE_ORDER_CLOUD,80,0.5,0);//鸟
    createAnimation(20,BATCHNODE_ORDER_CITY_GATE3,speed,0.5,0);
    
    createAnimation(13,BATCHNODE_ORDER_CITY_GATE3,speed,0.5,0);
    createAnimation(16,BATCHNODE_ORDER_CITY_GATE3,speed,0.5,1);
    createAnimation(17,BATCHNODE_ORDER_CITY_GATE3,speed,0.5,2);
    createAnimation(18,BATCHNODE_ORDER_CITY_GATE3,speed,0.5,3);
    createAnimation(19,BATCHNODE_ORDER_CITY_GATE3,speed,0.5,4);
    
    createAnimation(21,BATCHNODE_ORDER_CITY_GATE3,speed,0,0,false);//祭祀
}

void ImperialScene::onCargoShiAnimationCallback(Node* animationNode)
{
    m_cargoBuild->addSpeBuildState();
    m_cargoBuild->setShow(true);
}

void ImperialScene::cargoShipGoIn()
{
    if(isGoIn)
        return;
    if(m_cargoBuild)
        m_cargoBuild->setShow(false);
    auto index=23;
    auto time=10.0f;
    auto animationPos=m_animationAllPos[index];
    auto topPos=animationPos->getControlPointAtIndex(0);
    auto distance=0.0f;
    for (int i=0; i<animationPos->count(); i++) {
        distance+=topPos.distance(animationPos->getControlPointAtIndex(i));
        topPos=animationPos->getControlPointAtIndex(i);
    }
    auto speed=distance/time;
    
    Vector<FiniteTimeAction*> arrayOfActions;
    topPos=animationPos->getControlPointAtIndex(0);
    for (int i=0; i<animationPos->count(); i++) {
        auto distance=topPos.distance(animationPos->getControlPointAtIndex(i));
        auto duration=distance/speed;
        topPos=animationPos->getControlPointAtIndex(i);
        auto moveTo = CCMoveTo::create(duration,animationPos->getControlPointAtIndex(i));
        arrayOfActions.pushBack(moveTo);
    }
    
    auto func = CCCallFuncN::create(this, callfuncN_selector(ImperialScene::onCargoShiAnimationCallback));
    arrayOfActions.pushBack(func);
    arrayOfActions.pushBack(CCRemoveSelf::create());
    
    m_animationNodes[index]->stopAllActions();
    if(!m_animationNodes[index]->getChildByName("animationSprite"))
    {
        auto m_perSpr = CCSprite::createWithSpriteFrameName("picChuan_1.png");
        m_perSpr->setTag(0);
        m_perSpr->setScale(0.8);
        m_perSpr->setName("animationSprite");
        m_animationNodes[index]->setZOrder(BATCHNODE_ORDER_CITY_GATE3);
        m_animationNodes[index]->setTag(index);
        m_animationNodes[index]->addChild(m_perSpr);
    }

    m_animationNodes[index]->setPosition(animationPos->getControlPointAtIndex(0));
    CCSequence* sequenceAction = CCSequence::create(arrayOfActions);
    m_animationNodes[index]->runAction(sequenceAction);
    isGoIn=true;
}

void ImperialScene::cargoShipGoOut()
{
    if(m_cargoBuild)
        m_cargoBuild->setShow(false);
    auto index=24;
    auto time=10.0f;
    auto animationPos=m_animationAllPos[index];
    auto topPos=animationPos->getControlPointAtIndex(0);
    auto distance=0.0f;
    for (int i=0; i<animationPos->count(); i++) {
        distance+=topPos.distance(animationPos->getControlPointAtIndex(i));
        topPos=animationPos->getControlPointAtIndex(i);
    }
    auto speed=distance/time;
    
    Vector<FiniteTimeAction*> arrayOfActions;
    topPos=animationPos->getControlPointAtIndex(0);
    for (int i=0; i<animationPos->count(); i++) {
        auto distance=topPos.distance(animationPos->getControlPointAtIndex(i));
        auto duration=distance/speed;
        topPos=animationPos->getControlPointAtIndex(i);
        auto moveTo = CCMoveTo::create(duration,animationPos->getControlPointAtIndex(i));
        arrayOfActions.pushBack(moveTo);
    }
    arrayOfActions.pushBack(CCRemoveSelf::create());
    m_animationNodes[index]->stopAllActions();
    if(!m_animationNodes[index]->getChildByName("animationSprite"))
    {
        auto m_perSpr = CCSprite::createWithSpriteFrameName("picChuan_2.png");
        m_perSpr->setTag(0);
        m_perSpr->setScale(0.8);
        m_perSpr->setName("animationSprite");
        m_animationNodes[index]->setZOrder(BATCHNODE_ORDER_CITY_GATE3);
        m_animationNodes[index]->setTag(index);
        m_animationNodes[index]->addChild(m_perSpr);
    }
    
    m_animationNodes[index]->setPosition(animationPos->getControlPointAtIndex(0));
    CCSequence* sequenceAction = CCSequence::create(arrayOfActions);
    m_animationNodes[index]->runAction(sequenceAction);
    isGoIn=false;
}



void ImperialScene::initBgTree()
{
    //装饰树
    for(int i=0;i<6;i++)
    {
        auto decoratemNode=m_decorateNodes[i];
        auto & treeArray = decoratemNode->getChildren();
        for (int j=0; j<treeArray.size(); j++)
        {
            CCSprite* tmpSpr = dynamic_cast<CCSprite*>(treeArray.at(j));
            if (tmpSpr) {
                tmpSpr->removeFromParent();
                m_decorateBatchs[i]->addChild(tmpSpr);
                j--;
            }
        }
    }
    //下面的树
    auto & nodeArr = m_forestNode->getChildren();
    if (true) {
        for (int i=0; i<nodeArr.size(); i++)
        {
            CCNode* curNode = dynamic_cast<CCNode*>(nodeArr.at(i));
            if (curNode) {
                auto & treeArray = curNode->getChildren();
                for (int j=0; j<treeArray.size(); j++)
                {
                    CCSprite* tmpSpr = dynamic_cast<CCSprite*>(treeArray.at(j));
                    if (tmpSpr) {
                        tmpSpr->retain();
                        tmpSpr->removeFromParent();
                        tmpSpr->getTexture()->setAntiAliasTexParameters();
                        tmpSpr->setPosition(tmpSpr->getPosition()+curNode->getPosition()+m_forestNode->getPosition());
                        m_forestbatchNode->addChild(tmpSpr);
                        tmpSpr->release();
                        j--;
                    }
                }
            }
        }
    }
    
    this->getAnimationManager()->runAnimationsForSequenceNamed("Default");
    return;
    
    int preW = 256;
    int preH = 60;
    int stictW = 3060+510;
    int maxNum=stictW/preW + 2;
    for (int j=0; j<14; j++) {
        int cnt = maxNum;
        int dx = -preW/2;
        if(j%2==1) {
            cnt -= 1;
            dx = 0;
        }
        for (int i=0; i<cnt; i++) {
            auto tree = CCLoadSprite::createSprite("fill_trees.png");
            tree->setPosition(ccp(preW*i+dx, -j*preH+40));
            m_tilebatchNode->addChild(tree);
        }
    }
}

void ImperialScene::onShowBtnsView(int x, int y, int buildId)
{
//    m_buildBtnsView->setVisible(true);
    int buildType = buildId/1000;
    if (m_princessNode->isVisible() && FunBuildController::getInstance()->TalkViewBId == CC_ITOA(buildType)) {
        m_princessNode->setVisible(false);
        FunBuildController::getInstance()->TalkViewBId = "";
    }
    switch (buildType) {
        case FUN_BUILD_MAIN:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_castle);
            break;
        case FUN_BUILD_BARRACK1://barrack步兵
            SoundController::sharedSound()->playEffects(Music_Sfx_city_barrack);
            break;
        case FUN_BUILD_BARRACK2:{//stable骑兵
            SoundController::sharedSound()->playEffects(Music_Sfx_city_stable);
            break;
        }
        case FUN_BUILD_BARRACK3://range弓箭手
            SoundController::sharedSound()->playEffects(Music_Sfx_city_range);
            break;
        case FUN_BUILD_BARRACK4://chario plant 战斗车
            SoundController::sharedSound()->playEffects(Music_Sfx_city_chariot);
            break;
        case FUN_BUILD_CELLAR://仓库
            SoundController::sharedSound()->playEffects(Music_Sfx_city_depot);
            break;
        case FUN_BUILD_FORT://
            SoundController::sharedSound()->playEffects(Music_Sfx_city_fortress);
            break;
        case FUN_BUILD_TAVERN://市场
            SoundController::sharedSound()->playEffects(Music_Sfx_city_market);
            break;
        case FUN_BUILD_STABLE://Embassy
            SoundController::sharedSound()->playEffects(Music_Sfx_city_embassy);
            break;
        case FUN_BUILD_SCIENE:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_college);
            break;
        case FUN_BUILD_STONE:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_mithril);
            break;
        case FUN_BUILD_WOOD:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_wood);
            break;
        case FUN_BUILD_IRON:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_iron);
            break;
        case FUN_BUILD_FOOD:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_farm);
            break;
        case FUN_BUILD_HOSPITAL:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_hospital);
            break;
        case FUN_BUILD_WATCH_TOWER:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_watchtower);
            break;
        case FUN_BUILD_ARROW_TOWER:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_turret);
            break;
        case FUN_BUILD_WALL:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_walls);
            break;
        case FUN_BUILD_BARRACK://兵营帐篷
            SoundController::sharedSound()->playEffects(Music_Sfx_city_military);//
            break;
        case FUN_BUILD_SMITHY:{
            SoundController::sharedSound()->playEffects(Music_Sfx_city_hall);//
            break;
        }
        case FUN_BUILD_TRAINFIELD:{
            SoundController::sharedSound()->playEffects(Music_Sfx_city_drill);
            break;
        }
        case FUN_BUILD_FORGE:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_blacksmith);
            break;
        case FUN_BUILD_SACRIFICE:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_wishing);
            break;
        case FUN_BUILD_QIJI: 
            SoundController::sharedSound()->playEffects(Music_Sfx_city_building);
            break;
        case  SPE_BUILD_QIJI:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_building);
            break;
        default:
            SoundController::sharedSound()->playEffects(Music_Sfx_city_building);
            break;
    }
    
    m_buildBtnsView->setPosition(ccp(x,y));
    if (m_questEffect != 0) {
        m_buildBtnsView->onShow(buildId,m_questEffect);
    }
    else
        m_buildBtnsView->onShow(buildId);
}

void ImperialScene::onShowSpeBtnsView(int x, int y, int buildId)
{
    if (m_speBuildBtnsView->isVisible() && buildId == m_speBuildBtnsView->m_buildingKey) {
        m_speBuildBtnsView->onHide();
        return;
    }
    m_speBuildBtnsView->setPosition(ccp(x,y));
    m_speBuildBtnsView->onShow(buildId);
}

void ImperialScene::onFlyTroopParticle(int x, int y, int onlyPower)
{
    m_flyX = x;
    m_flyY = y;
    m_isOnlyPower = onlyPower;
    flyCount = 1;
    playFlyTroopParticle(0);
    this->schedule(schedule_selector(ImperialScene::playFlyTroopParticle), 0.2);
    this->scheduleOnce(schedule_selector(ImperialScene::playPowerAni), 0.1);
}

void ImperialScene::playFlyTroopParticle(float _time){
    if(flyCount>0){
        flyCount = flyCount - 1;
        int toX = CCDirector::sharedDirector()->getWinSize().width / 2;
        int toY = CCDirector::sharedDirector()->getWinSize().height-80;
        
        CCPoint startPoint = ccp(m_flyX, m_flyY);
        CCPoint endPoint = ccp(toX, toY);
        
        if (m_isOnlyPower != 2)//战斗力
        {
            string tmpStart = "Collection_expF_";//Collection_Star_ Collection_expF_
            int count = 3;
            for (int i=1; i<=count; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
                if (i==1) {
                    particle->setStartColor(ccc4f(1.0, 0.77, 0, 1.0));
                    particle->setEndColor(ccc4f(0, 0, 0, 0));
                }else if (i==2) {
                    particle->setStartColor(ccc4f(1.0, 0.55, 0, 1.0));
                    particle->setEndColor(ccc4f(1, 0.96, 0.5, 0));
                }else if (i==3) {
                    particle->setStartColor(ccc4f(1.0, 0.9, 0, 1.0));
                    particle->setEndColor(ccc4f(0, 0, 0, 0));
                }
                
                if (i == 2) {
                    particle->setPositionType(kCCPositionTypeFree);
                } else {
                    particle->setPositionType(kCCPositionTypeRelative);
                }
                particle->setPosition(startPoint);
                this->addChild(particle);
                
                auto func = CCCallFuncO::create(this, callfuncO_selector(ImperialScene::onFlyTroopEnd), particle);
                
                CCActionInterval * moveTo = CCMoveTo::create(1,endPoint);
                CCActionInterval * easeExponentialIn= CCEaseExponentialOut::create(moveTo);
                
                CCActionInterval * delT = CCDelayTime::create(0.3);
                CCActionInterval * fadeOut = CCFadeOut::create(0.2);
                if (flyCount == 0 && i==count) {
                    auto funcPowerUI = CCCallFuncO::create(this, callfuncO_selector(ImperialScene::onFlyTroopEndToUI), CCString::create("power"));
                    CCActionInterval * seqFadeOut = CCSequence::create(delT, funcPowerUI, fadeOut, func, NULL);
                    particle->runAction(CCSpawn::create(easeExponentialIn,seqFadeOut,NULL));
                }else {
                    CCActionInterval * seqFadeOut = CCSequence::create(delT, fadeOut, func, NULL);
                    particle->runAction(CCSpawn::create(easeExponentialIn,seqFadeOut,NULL));
                }
            }
        }
        
        if(m_isOnlyPower != 1)//经验
        {
            string tmpStart = "Collection_expF_";//Collection_Star_
            int count = 3;
            for (int i=1; i<=count; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
                if (i == 2) {
                    particle->setPositionType(kCCPositionTypeFree);
                } else {
                    particle->setPositionType(kCCPositionTypeRelative);
                }
                
                particle->setPosition(ccp(m_flyX, m_flyY));
                this->addChild(particle);
                
                CCActionInterval * moveTo = CCMoveTo::create(1,ccp(80, toY));
                CCActionInterval * easeExponentialIn= CCEaseExponentialOut::create(moveTo);
                
                CCActionInterval * delT = CCDelayTime::create(0.3);
                CCActionInterval * fadeOut = CCFadeOut::create(0.2);
                auto func = CCCallFuncO::create(this, callfuncO_selector(ImperialScene::onFlyTroopEnd), particle);
                
                if (flyCount == 0 && i==count) {
                    auto funcUI = CCCallFuncO::create(this, callfuncO_selector(ImperialScene::onFlyTroopEndToUI), CCString::create("exp"));
                    CCActionInterval * seqFadeOut = CCSequence::create(delT, funcUI, fadeOut, func, NULL);
                    particle->runAction(CCSpawn::create(easeExponentialIn,seqFadeOut,NULL));
                }else {
                    CCActionInterval * seqFadeOut = CCSequence::create(delT, fadeOut, func, NULL);
                    particle->runAction(CCSpawn::create(easeExponentialIn,seqFadeOut,NULL));
                }
            }
        }

    }else{
        this->unschedule(schedule_selector(ImperialScene::playFlyTroopParticle));
        flyCount = 0;
    }
}

void ImperialScene::onFlyTroopEnd(CCObject *obj)
{
    CCNode* node = dynamic_cast<CCNode*>(obj);
    if (node) {
        node->stopAllActions();
        node->removeFromParent();
    }
}

void ImperialScene::onFlyTroopEndToUI(CCObject *obj)
{
    CCString* ccStr = dynamic_cast<CCString*>(obj);
    if (ccStr) {
        string msg = ccStr->getCString();
        if (msg == "power") {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SHOW_POWER_PARTICLE);
        }else if (msg == "exp") {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SHOW_EXP_PARTICLE);
        }else if(msg == "stamine"){
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SHOW_STAMINE_PARTICLE);
        }
    }
}

void ImperialScene::playPowerAni(float _time){
    
}

void ImperialScene::refreshSoldiers(CCObject* obj)
{
    if (!m_soldierBatchNode) {
        return;
    }
    m_soldierBatchNode->removeAllChildren();
    for (int i=0; i<m_soldierArray->count(); i++) {
        BattleSoldier2* soldier = dynamic_cast<BattleSoldier2*>(m_soldierArray->objectAtIndex(i));
        soldier->removeObserver(1);//1为校场士兵的监听type
    }
    m_soldierArray->removeAllObjects();
    m_sSqerTouchArr->removeAllObjects();
    m_sSqerTouchNode->removeAllChildren();
    
    FunBuildController::getInstance()->lastSqerPos.clear();
    int numPerSqer = 3600/10;
    int sqerIdx = 0;
    vector<long> soldierSqerNum = ArmyController::getInstance()->getSoldierSqerNum();//返回各兵种方阵数量，依次为 骑， 步， 弓， 车
    for (int i=0; i<4; i++)
    {
        int sType = i+1;
        if (i==0) {
            sType = 2;
        }
        else if (i==1) {
            sType = 1;
        }
        else if (i==2) {
            sType = 3;
        }
        else if (i==3) {
            sType = 4;
        }
        
        long soldierCnt = soldierSqerNum[i+4];
        int sqerCnt = soldierSqerNum[i];
        if (i==3) {
            if (sqerIdx>=0 && sqerIdx<6) {
                if(sqerIdx>0 && sqerCnt<=(7+7+6+4)) {
                    sqerIdx = 6;
                }
            }
            else if (sqerIdx>=6 && sqerIdx<13) {
                if(sqerIdx>6 && sqerCnt<=(7+6+4)) {
                    sqerIdx = 13;
                }
            }
            else if (sqerIdx>=13 && sqerIdx<20) {
                if(sqerIdx>13 && sqerCnt<=(6+4)) {
                    sqerIdx = 20;
                }
            }
            else if (sqerIdx>=20 && sqerIdx<26) {
                if(sqerIdx>20 && sqerCnt<=4) {
                    sqerIdx = 26;
                }
            }
            else if (sqerIdx>=26) {
                //车兵已经到最后一排了
            }
        }
        
        for (int j=0; j<sqerCnt; j++) {
            int sqrSoldierNum = numPerSqer;
            if (soldierCnt>=numPerSqer) {
                soldierCnt -= numPerSqer;
            }else {
                sqrSoldierNum = soldierCnt;
            }
            auto pt = FunBuildController::getInstance()->sqerPoints[sqerIdx];
            FunBuildController::getInstance()->lastSqerPos[sType] = sqerIdx;
            addSoldierToMap(sType, sqrSoldierNum, pt.x, pt.y, sqerIdx);
            Node* touchNode = Node::create();
            touchNode->setAnchorPoint(ccp(0.5, 1));
            touchNode->setPosition(pt);
            touchNode->setContentSize(Size(50, 50));
            if (m_sSqerTouchNode && m_sSqerTouchArr) {
                m_sSqerTouchNode->addChild(touchNode);
                m_sSqerTouchArr->addObject(touchNode);
            }
            sqerIdx++;
        }
    }
    
    for (int st=1; st<=4; st++) {
        if (FunBuildController::getInstance()->lastSqerPos.find(st) == FunBuildController::getInstance()->lastSqerPos.end()) {
            FunBuildController::getInstance()->lastSqerPos[st] = sqerIdx;
            sqerIdx++;
        }
    }
}

void ImperialScene::addSoldierToMap(int stype, int num, int ptx, int pty, int sqerType)
{
    int rowWidth = 20;//不同行x偏移
    int colHeight = 10;//不同行y偏移
    int perX = 10;//同一行，每一个兵的x偏移
    int perY = 5;//同一行，每一个兵的y偏移
    int preNum = 200/10;
    int m_row = 6;
    int m_col = 3;
    string m_icon = "";
    float scale = 0.7;
    
    int tX = 0;
    int tY = 0;
    
    switch (stype) {
        case 1://步兵
            m_icon = "a010";
            break;
        case 5://弓兵
            m_icon = "a030";
            break;
        case 2://骑兵
            rowWidth = 30;
            colHeight = 15;
            perX = 15;
            perY = 7;
            m_row = 4;
            m_col = 2;
            m_icon = "a020";
            preNum = 450/10;
            scale = 0.7;
            break;
        case 4://战车
            m_row = 3;
            m_col = 1;
            perX = 20;
            perY = 10;
            m_icon = "zhanche";
            preNum = 1200/10;
            scale = 0.7;
            tX = 0;
            tX = -20;
            break;
        case 3://长弓兵
            m_icon = "a030";
            break;
        default:
            m_icon = "a020";
            break;
    }
    
    int sNum = ceil(num*1.0/preNum);
    
//    sNum = 18;
//    if (stype == 4) {
//        sNum = 3;
//    }
//    else if (stype == 2) {
//        sNum = 8;
//    }
    
    for (int i=0; i<sNum; i++) {
        int row = i%m_row;//某排上第几位
        int col = i/m_row;//第几排
        CCPoint endPos = ccp(ptx+row*perX-col*rowWidth,pty-row*perY-col*colHeight);
        int firstInSqer = -1;
        if (i == 0) {
            firstInSqer = 1;
        }
        BattleSoldier2* soldier = BattleSoldier2::create(m_soldierBatchNode, NULL,0,0,m_icon,"NE",false,sqerType,firstInSqer);
        soldier->addObserver(1);//1为校场士兵的监听type
        soldier->setSoldierPosition(endPos+ccp(tX, tY));
        soldier->setAnchorPoint(ccp(0.5, 0.5));
        soldier->setSprScale(scale);
        m_soldierArray->addObject(soldier);
    }
}

void ImperialScene::scheduleHarvestEffect(float _time)
{
    showHarvestEffect(NULL);
}

void ImperialScene::showHarvestEffect(CCObject* obj){
    string harvestInfo =  GlobalData::shared()->m_harvestInfo;
    if(harvestInfo==""){
        return ;
    }
    GlobalData::shared()->m_harvestInfo = "";
    std::vector<std::string> data;
    CCCommonUtils::splitString(harvestInfo,"|",data);
    if(data.size()!=3) return ;
    int outPutSec = atoi(data[0].c_str());
    int px = atoi(data[1].c_str());
    int py = atoi(data[2].c_str());
    onMoveToPos(px, py, TYPE_POS_MID, 0.5, 1, true);
    
    FunBuildController::getInstance()->addOutPut(outPutSec);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SHOW_SKILL_HARVEST,CCString::create(CC_ITOA(outPutSec)));
}

void ImperialScene::playFountain()
{
    auto node = CCNode::create();
    int tag = m_fountainNode->getChildrenCount();
    node->setTag(tag);
    m_fountainNode->addChild(node);
    
    struct timeval  tm;
    gettimeofday(&tm, NULL);
    int randIdx = tm.tv_usec%10;
    int start = 1;
    int end = 6;
    bool showPar = true;
    if(randIdx>=0 && randIdx<=5) {
        start = 1;
        end = 4;
        showPar = false;
    }
    else if (randIdx>=6 && randIdx<=8) {
        start = 5;
        end = 6;
        showPar = false;
    }
    
    for (int i=start; i<=end; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("FountainOut_%d",i)->getCString());
        if (i==1) {
            particle->setPosition(ccp(42, 105));
        }
        else if (i==2) {
            particle->setPosition(ccp(108, 105));
        }
        else if (i==3) {
            particle->setPosition(ccp(105, 75));
        }
        else if (i==4) {
            particle->setPosition(ccp(42, 75));
        }
        else {
            particle->setPosition(ccp(75, 100));
        }
        node->addChild(particle);
    }
    if (showPar && m_fountainFinish) {
        m_fountainFinish = false;
        Sprite* rainbow = (Sprite*)m_penquanRainbow->getChildByTag(9999);
        if (rainbow) {
            rainbow->setOpacity(0);
        }
        m_penquanRainbow->setVisible(true);
        
        Sequence* sequence = Sequence::create(FadeTo::create(1.0, 255), CallFunc::create(this,callfunc_selector(ImperialScene::addFountainRainbowPar)), DelayTime::create(3.0),CallFunc::create(this, callfunc_selector(ImperialScene::removeFountainRainbow)), NULL);
        rainbow->runAction(sequence);
    }
    CCActionInterval * delayT = CCDelayTime::create(3.0);
    auto func = CCCallFuncO::create(this, callfuncO_selector(ImperialScene::removeFountain), CCInteger::create(tag));
    node->runAction(CCSequence::create(delayT, func, NULL));
}

void ImperialScene::removeFountainRainbow()
{
    Sequence* sequence = Sequence::create(FadeTo::create(5.0, 0), CallFunc::create(this, callfunc_selector(ImperialScene::removeFountainRainbowPar)),NULL);
    Sequence* sequence1 = Sequence::create(FadeTo::create(5.0, 0),NULL);
    Sprite* rainbow = (Sprite*)m_penquanRainbow->getChildByTag(9999);
    if (rainbow) {
        rainbow->runAction(sequence);
    }
    Node* parNode = m_fountainNode->getChildByTag(10000);
    if (parNode) {
        parNode->runAction(sequence1);
    }
}

void ImperialScene::addFountainRainbowPar()
{
    Node* parNode = Node::create();
    parNode->setTag(10000);
    for (int i = 0; i < 3; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("Rainbow_%d",i)->getCString());
        parNode->addChild(particle);
        parNode->setPosition(0, 0);
    }
    m_penquanRainbow->addChild(parNode);
}

void ImperialScene::removeFountainRainbowPar()
{
    if (m_penquanRainbow->isVisible()) {
        m_penquanRainbow->setVisible(false);
        m_penquanRainbow->removeChildByTag(10000);
        m_fountainFinish = true;
    }
}

void ImperialScene::removeFountain(CCObject *obj)
{
    int tag = dynamic_cast<CCInteger*>(obj)->getValue();
    m_fountainNode->removeChildByTag(tag);
}

void ImperialScene::playWatchGlow()
{
    auto spr = CCLoadSprite::createSprite("watch_golw.png");
    spr->setAnchorPoint(ccp(0, 0.5));
    spr->setScaleX(15);
    spr->setScaleY(3);
    spr->setPosition(ccp(1462, 1745));
    m_touchLayer->addChild(spr,3001);
    
    CCActionInterval * rotateto1 = CCRotateTo::create(2, 180);
    CCActionInterval * rotateto2 = CCRotateTo::create(2, 360);
    CCActionInterval * rotateto3 = CCRotateTo::create(0, 0);
    CCActionInterval * seq = CCSequence::create(rotateto1, rotateto2, rotateto3, NULL);
    spr->runAction(CCRepeatForever::create(seq));
    
    CCActionInterval * fadeTo1 = CCFadeTo::create(0.5,200);
    CCActionInterval * fadeTo2 = CCFadeTo::create(0.5, 100);
    CCActionInterval * fadeSeq = CCSequence::create(fadeTo1, fadeTo2, NULL);
    spr->runAction(CCRepeatForever::create(fadeSeq));
}

void ImperialScene::checkTileGlow(CCObject* obj)
{
    return; //去掉新手引导中建造翰林院的特效
    QuestInfo *qinfo = QuestController::getInstance()->getRecommendQuest(true);
    int buildType = 0;
    if (qinfo && qinfo->itemId == "2200101") {//建造步兵营任务
        buildType = FUN_BUILD_BARRACK1;
    }
    else if (qinfo && qinfo->itemId == "2400101") {//建造学院任务
        buildType = FUN_BUILD_SCIENE;
    }
    
    if (buildType == 0) {
        m_tileGlowNode->removeAllChildren();
    }
    else {
        int bid = FunBuildController::getInstance()->getMaxLvBuildByType(buildType);
        int pos = -1;
        if (bid > 0) {
            FunBuildInfo& info = FunBuildController::getInstance()->getFunbuildById(bid);
            pos = info.pos;
        }
        else {
            pos = findCanBuildTile(qinfo->go);
        }
        if (pos>0) {
            m_tileGlowNode->removeAllChildren();
            m_tileGlowNode->setPosition( m_nodeBuildings[pos]->getPosition()+ccp(130,65) );
            for (int i=1; i<=8; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("GuideRegional_%d",i)->getCString());
                m_tileGlowNode->addChild(particle);
            }
        }
    }
}

void ImperialScene::makeCanPlayArrowPoint()
{
    float __scale = 0.7;
    if (CCCommonUtils::isIosAndroidPad())
    {
        __scale = HD_SCALE;
    }
    onMoveToPos(2250, 630, TYPE_POS_MID, 0.8, __scale, true);
    
    m_ptArrowClick = CCPointArray::create(20);
    for (int i=17; i<=33; i++) {//城门门口16个地块
        int bid = FunBuildController::getInstance()->getBuildIdByPos(i);
        if (bid > 0) {
            auto& info = FunBuildController::getInstance()->getFunbuildById(bid);
            if (info.type == FUN_BUILD_WOOD || info.type == FUN_BUILD_FOOD) {
                FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[i]->getChildByTag(i));
                if (build) {
                    int buildPosX = build->getParent()->getPositionX() + build->mainWidth / 2 ;
                    int buildPosY = build->getParent()->getPositionY() + build->mainHeight + 50;
                    m_ptArrowClick->addControlPoint(ccp(buildPosX, buildPosY));
                    break;
                }
            }
        }
    }
    _arrowPtIdx = 0;
    playArrowClickEnd();
    
    scheduleOnce(schedule_selector(ImperialScene::stopArrowClick), 2.0f);
}

void ImperialScene::playArrowClickEnd()
{
    if (_arrowPtIdx>=m_ptArrowClick->count()) {
        _arrowPtIdx = 0;
    }
    m_flyArrow->setPosition( m_ptArrowClick->getControlPointAtIndex(_arrowPtIdx) );
    playArrowClick();
}

void ImperialScene::playArrowClick()
{
    _arrowPtIdx++;
    m_flyArrow->setVisible(true);
    auto delay = DelayTime::create(2);
    CCCallFunc * callfunc = CCCallFunc::create(this, callfunc_selector(ImperialScene::playArrowClickEnd));
    m_flyArrow->runAction(CCSequence::create(delay, callfunc, NULL));
}

void ImperialScene::stopArrowClick(float _time)
{
    m_flyArrow->stopAllActions();
    m_flyArrow->setVisible(false);
    if(PortActController::getInstance()->m_isGetNewRD==0 && PortActController::getInstance()->m_newLoginDay<7 && PortActController::getInstance()->m_newPalyerRDMap.size() > 0){
        guideTypeNew = 1;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("fengshou1"));
}

void ImperialScene::getFriendsData()
{
    if (FriendsController::getInstance()->m_bOpen)
    {
        if (FriendsController::getInstance()->m_isDataInit) {
            FriendsController::getInstance()->checkHasNewApplyForMe();
        } else {
            FriendsController::getInstance()->startGetFriendsData();
        }
    }
    ToolController::getInstance()->downloadCustomSkin();
}

void ImperialScene::getScienceData()
{
    if (GlobalData::shared()->scienceInfo.IsHasData == false) {
        ScienceDataInitCommand * cmd=new ScienceDataInitCommand();
        cmd->sendAndRelease();
        auto& map = ToolController::getInstance()->m_heartTellMap;
        for (auto it = map.begin(); it != map.end(); ++it)
        {
            for (auto it1 = it->second.begin(); it1 != it->second.end(); ++it1)
            {
                if (it->first == 1) {
                    CCCommonUtils::flyHint("", "", _lang_1("150538", (*it1).c_str()));
                }
            }
        }
    }
    ActivityController::getInstance()->loginEventData();
    recordGaid();
    GameController::getInstance()->recordPushData();
    
    AllianceManager::getInstance()->starGetNewAlliance();
    
    
//    if (ChristmasActController::getInstance()->m_actTime.size()<=0) {
//        ChristmasActController::getInstance()->startGetActTime();
//    }
}

void ImperialScene::recordGaid(){
//     ImperialScene::trackTencentForZW();
    if(GlobalData::shared()->gaid == ""){
        GlobalData::shared()->gaid = cocos2d::extension::CCDevice::getGaid();
        CCLOG("idfa %s ",GlobalData::shared()->gaid.c_str());
        UserBindGaidCommand* cmd = new UserBindGaidCommand(GlobalData::shared()->gaid.c_str());
        cmd->sendAndRelease();
//        ImperialScene::trackDBM();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//        ImperialScene::trackTencent();
//#endif
    }
}

//void ImperialScene::trackDBM(){
//    std::transform(GlobalData::shared()->gaid.begin(),GlobalData::shared()->gaid.end(),GlobalData::shared()->gaid.begin(),::toupper);
//    MD5 md5;
//    md5.update(GlobalData::shared()->gaid.c_str(), GlobalData::shared()->gaid.length());
//    std::string muid = md5.toString();
//    //            std::transform(muid.begin(),muid.end(),muid.begin(),::toupper);
//    CCString* url;
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    url = CCString::createWithFormat("http://ad.doubleclick.net/ddm/activity/src=4755477;cat=9bdrezia;type=invmedia;dc_muid=%s;ord=%ld",muid.c_str(),GlobalData::shared()->getTimeStamp());
//#endif
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    if(!GlobalData::shared()->isGoogle())
//        return;
//    url = CCString::createWithFormat("http://ad.doubleclick.net/ddm/activity/src=4755474;cat=s3y7ybnb;type=invmedia;dc_muid=%s;ord=%ld",muid.c_str(),GlobalData::shared()->getTimeStamp());
//#endif
//    CCLOG("DBM track URL: %s",url->getCString());
//    CCHttpRequest* request = new CCHttpRequest();
//    request->setUrl(url->getCString());
//    request->setRequestType(CCHttpRequest::Type::GET);
//    request->setResponseCallback(this, httpresponse_selector(ImperialScene::onTrackResponse));
//    CCHttpClient::getInstance()->setTimeoutForConnect(10);
//    CCHttpClient::getInstance()->send(request);
//    request->release();
//}
//
//void ImperialScene::trackTencent(){
//    std::transform(GlobalData::shared()->gaid.begin(),GlobalData::shared()->gaid.end(),GlobalData::shared()->gaid.begin(),::toupper);
//    MD5 md5;
//    md5.update(GlobalData::shared()->gaid.c_str(), GlobalData::shared()->gaid.length());
//    std::string muid = md5.toString();
//    long time = GlobalData::shared()->getTimeStamp();
//    string sign_key = "809e06139b668e64";
//    string query_string = CCString::createWithFormat("muid=%s&conv_time=%ld",muid.c_str(),time)->getCString();
//    string page = CCString::createWithFormat("http://t.gdt.qq.com/conv/app/945274928/conv?%s",query_string.c_str())->getCString();
//    std::string property = CCString::createWithFormat("%s&GET&%s",sign_key.c_str(),ImperialScene::UrlEncode(page).c_str())->getCString();
//    MD5 md5_;
//    md5_.update(property.c_str(), property.length());
//    std::string base_data = CCString::createWithFormat("%s&sign=%s",query_string.c_str(),ImperialScene::UrlEncode(md5_.toString().c_str()).c_str())->getCString();
//    std::string encrypt_key = "8ac0a2161253cb5e";
//    unsigned char *xorResult = new unsigned char[base_data.size()];
//    ImperialScene::simple_xor(base_data, encrypt_key, xorResult);
//    std::string data = cocos2d::extension::CCDevice::base64Encode(xorResult,base_data.size());
//    CCString* url = CCString::createWithFormat("http://t.gdt.qq.com/conv/app/945274928/conv?v=%s&conv_type=MOBILEAPP_ACTIVITE&app_type=IOS&advertiser_id=552722",data.c_str());
//    CCLOG("Tencent track URL: %s",url->getCString());
//    CCHttpRequest* request = new CCHttpRequest();
//    request->setUrl(url->getCString());
//    request->setRequestType(CCHttpRequest::Type::GET);
//    request->setResponseCallback(this, httpresponse_selector(ImperialScene::onTrackResponse));
//    CCHttpClient::getInstance()->setTimeoutForConnect(10);
//    CCHttpClient::getInstance()->send(request);
//    request->release();
//}
//void ImperialScene::trackTencentForZW(){//掌握时代代理广点通接入
//    std::transform(GlobalData::shared()->gaid.begin(),GlobalData::shared()->gaid.end(),GlobalData::shared()->gaid.begin(),::toupper);
//    MD5 md5;
//    md5.update(GlobalData::shared()->gaid.c_str(), GlobalData::shared()->gaid.length());
//    std::string muid = md5.toString();
//    CCLOG("idfa_TestMD5 %s ",muid.c_str());
//    
//    long time = GlobalData::shared()->getTimeStamp();
//    string sign_key = "77a1ef3fa021af1d";
//    string query_string = CCString::createWithFormat("muid=%s&conv_time=%ld",muid.c_str(),time)->getCString();
//    string page = CCString::createWithFormat("http://t.gdt.qq.com/conv/app/945274928/conv?%s",query_string.c_str())->getCString();
//    std::string property = CCString::createWithFormat("%s&GET&%s",sign_key.c_str(),ImperialScene::UrlEncode(page).c_str())->getCString();
//    MD5 md5_;
//    md5_.update(property.c_str(), property.length());
//    std::string base_data = CCString::createWithFormat("%s&sign=%s",query_string.c_str(),ImperialScene::UrlEncode(md5_.toString().c_str()).c_str())->getCString();
//    std::string encrypt_key = "4dc7f2a6c7c89510";
//    unsigned char *xorResult = new unsigned char[base_data.size()];
//    ImperialScene::simple_xor(base_data, encrypt_key, xorResult);
//    std::string data = cocos2d::extension::CCDevice::base64Encode(xorResult,base_data.size());
//    CCString* url = CCString::createWithFormat("http://t.gdt.qq.com/conv/app/945274928/conv?v=%s&conv_type=MOBILEAPP_ACTIVITE&app_type=IOS&advertiser_id=1187663",data.c_str());
//    CCLOG("Tencent track URL: %s",url->getCString());
//    CCHttpRequest* request = new CCHttpRequest();
//    request->setUrl(url->getCString());
//    request->setRequestType(CCHttpRequest::Type::GET);
//    request->setResponseCallback(this, httpresponse_selector(ImperialScene::onTrackResponse));
//    CCHttpClient::getInstance()->setTimeoutForConnect(10);
//    CCHttpClient::getInstance()->send(request);
//    request->release();
//}

std::string ImperialScene::UrlEncode(const std::string& str)
{
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (isalnum((unsigned char)str[i]) ||
            (str[i] == '-') ||
            (str[i] == '_') ||
            (str[i] == '.') ||
            (str[i] == '~'))
            strTemp += str[i];
        else if (str[i] == ' ')
            strTemp += "+";
        else
        {
            strTemp += '%';
            unsigned char upper = (unsigned char)str[i] >> 4;
            strTemp += upper > 9 ? upper + 55 : upper + 48;
            unsigned char lower = (unsigned char)str[i] % 16;
            strTemp += lower > 9 ? lower + 55 : lower + 48;
        }
    }
    return strTemp;
}

bool ImperialScene::simple_xor(const std::string& info,const std::string& key,unsigned char* result) {
    if (result == NULL || info.empty() || key.empty()) {
        return false;
    }
//    result->clear();
    uint32_t i = 0;
    uint32_t j = 0;
    for (; i < info.size(); ++i) {
        result[i] = static_cast<unsigned char>(info[i] ^ key[j]);
//        result->push_back(static_cast<unsigned char>(info[i] ^ key[j]));
        j = (++j) % (key.length());
    }
    return true;
}

void ImperialScene::onTrackResponse(CCHttpClient* client, CCHttpResponse* response)
{
    CCLOG("response code: %d", response->getResponseCode());
    if (!response->isSucceed())
    {
        CCLOG("response failed!\nerror buffer: %s", response->getErrorBuffer());
    }else{
        std::vector<char>* iter = response->getResponseData();
        std::string serverInfo(iter->begin(),iter->end());
        CCLOG("track response %s", serverInfo.c_str());
    }
}

void ImperialScene::createActiveBd(CCObject* obj)
{
    if (m_hdBuild) {
    }
    else if(m_buildingInitState && m_resbatchNode && ActivityController::getInstance()->activityIsOpen()){
        m_hdBuild = SpeBuild::create(SPE_BUILD_HD);
        m_hdNode->addChild(m_hdBuild);
        int hod = m_hdNode->getZOrder();
        m_hdBuild->setNamePos(m_hdNode->getPositionX(), m_hdNode->getPositionY(), m_signLayer, m_popLayer, m_arrbatchNode, m_resbatchNode, hod);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PERSON_TALK);
    }
}

void ImperialScene::onShowTalkView(CCObject* obj)
{
    int time = FunBuildController::getInstance()->getTalkTipsTime();
    if ( FunBuildController::getInstance()->IsHaveRecordeByKey(CLIENT_RECORD_PRINESS) && (m_talkTime>=time || GlobalData::shared()->princessShow) && !GuideController::share()->isInTutorial() && CCCommonUtils::isTestPlatformAndServer("Npc_princess") && !m_princessNode->isVisible() && FunBuildController::getInstance()->getMainCityLv() >= 3)
    {//600
        m_princessBid = 0;
        GlobalData::shared()->princessShow = false;
        string content = FunBuildController::getInstance()->getTalkTips();
        auto princeSize = m_princessNode->getContentSize();
        string buildItemId = FunBuildController::getInstance()->TalkViewBId;
        if (buildItemId == "") {
            if (m_princess2Node) {
                m_princessNode->setPosition(ccp(1030, 1550));
            }
            else
                m_princessNode->setPosition(ccp(933, 1450));
            m_princessNode->setVisible(true);
            return;
        }
        int itemId = atoi(buildItemId.c_str());
        if (itemId == SPE_BUILD_FLAG) {
            m_princessNode->setPosition(ccp(70, 1040));
            m_princessNode->setVisible(true);
            return;
        }
        
        int offY = 177;
        if (itemId == FUN_BUILD_FOOD || itemId == FUN_BUILD_WOOD || itemId == FUN_BUILD_STONE || itemId == FUN_BUILD_IRON || itemId == FUN_BUILD_BARRACK || itemId == FUN_BUILD_HOSPITAL) {
            offY = 100;
        }
        if (itemId == FUN_BUILD_WATCH_TOWER) {
            offY = 270;
        }
        int bid = FunBuildController::getInstance()->getMaxLvBuildByType(itemId);
        if(bid>0 && m_buildItems.find(bid) != m_buildItems.end()){
            if ( m_buildItems[bid]->IsNullBuildState() ) {
                m_princessBid = bid;
                int buildPosX = m_buildItems[bid]->getParent()->getPositionX() + m_buildItems[bid]->getContentSize().width / 2 ;
                int buildPosY = m_buildItems[bid]->getParent()->getPositionY() + offY;
                m_princessNode->setPosition(ccp(buildPosX-princeSize.width/2, buildPosY));
            }else {
                return;
            }
        }
        else {
            if (m_princess2Node) {
                m_princessNode->setPosition(ccp(1030, 1550));
            }
            else
                m_princessNode->setPosition(ccp(933, 1450));
        }
        m_princessNode->setVisible(true);
//        m_talkTime = 0;
    }
}
void ImperialScene::onShowTalkViewOnce()
{
    if (CCCommonUtils::isIosAndroidPad()) {
        onMoveToPos(1100, 1580, TYPE_POS_MID, 0.5, 1.0, true);
    } else {
        onMoveToPos(1100, 1580, TYPE_POS_MID, 0.5, 0.8, true);
    }
    if ( FunBuildController::getInstance()->IsHaveRecordeByKey(CLIENT_RECORD_PRINESS) && !GuideController::share()->isInTutorial() && CCCommonUtils::isTestPlatformAndServer("Npc_princess") && !m_princessNode->isVisible())
    {//600
        if (m_princess2Node) {
            m_princessNode->setPosition(ccp(1030, 1550));
        }
        else
            m_princessNode->setPosition(ccp(933, 1450));
        m_princessNode->setVisible(true);
        m_talkTime = 0;
        princessNodeOnce = true;
    }
}
void ImperialScene::hidePrincess(int bid)
{
    if (m_princessBid == bid) {
        m_princessNode->setVisible(false);
        m_princessBid = 0;
    }
}
void ImperialScene::reSetTalkTime()
{
    m_talkTime = 0;
}

void ImperialScene::onDragonFly()
{
    m_dragonFlyNode->stopAllActions();
    CCActionInterval * move1 = CCMoveTo::create(0,ccp(-600, 1020));
    CCActionInterval * move2 = CCMoveTo::create(24,ccp(4100, 1020));
    m_dragonFlyNode->runAction(CCSequence::create(move1, move2, NULL));
}

void ImperialScene::onMakeDragon()
{
    auto dragon = DragonFly::create();
    m_dragonFlyNode->addChild(dragon);
}

void ImperialScene::addDefenseFailEffect(){
    m_failEffectNode->removeAllChildrenWithCleanup(true);
    CCSpriteFrame* cf1 = CCLoadSprite::getSF("fireBomb.png");
    CCSpriteFrame* cf2 = CCLoadSprite::getSF("a010_0_N_attack_0.png");
    if(cf1==NULL || cf2==NULL){
        return ;
    }
    CCSpriteBatchNode* bombBN = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("fireBomb.png")->getTexture());
    m_failEffectNode->addChild(bombBN);
    
    CCSpriteBatchNode* soldierBN = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("a010_0_N_attack_0.png")->getTexture());
    m_failEffectNode->addChild(soldierBN);
    
    string fireName = "WarFire_";
    vector<CCParticleBatchNode*> parVec;
    auto addPar = [](vector<CCParticleBatchNode*> parVec, CCNode* node, cocos2d::CCParticleSystemQuad *particle) {
        auto batchCount = parVec.size();
        while (batchCount--) {
            auto &batch = parVec[batchCount];
            if (batch) {
                auto batchBlend = batch->getBlendFunc();
                auto particleBlend = particle->getBlendFunc();
                if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                    batch->addChild(particle);
                    return;
                }
            }
        }
        auto newBatch = ParticleController::createParticleBatch();
        node->addChild(newBatch);
        newBatch->addChild(particle);
        parVec.push_back(newBatch);
    };
    
    //城门点
    CCPoint pos = ccp(1931,820);
    auto bg1 = CCLoadSprite::createSprite("fireBomb.png");
    bg1->setScale(1.5);
    bg1->setOpacity(GlobalData::shared()->getRand(80,120));
    bg1->setPosition(pos+ccp(30,0));
    bombBN->addChild(bg1);
    
    auto soldier1 = CCLoadSprite::createSprite("a010_0_N_die_5.png");
    soldier1->setPosition(pos+ccp(30,-60));
    soldierBN->addChild(soldier1);

    auto soldier1_1 = CCLoadSprite::createSprite("a030_0_N_die_5.png");
    soldier1_1->setPosition(pos+ccp(30,90));
    soldierBN->addChild(soldier1_1);
    
    
    for (int j=1; j<=5; j++) {
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",fireName.c_str(),j)->getCString());
        particle1->setScale(0.2);
        particle1->setPosition(pos+ccp(80,0));
        addPar(parVec,m_failEffectNode,particle1);
    }
    
    //城门内点
    pos = ccp(1519,998);
    auto bg2 = CCLoadSprite::createSprite("fireBomb.png");
    bg2->setScale(2);
    bg2->setOpacity(120);
    bg2->setPosition(pos);
    bombBN->addChild(bg2);
    
    auto soldier2 = CCLoadSprite::createSprite("a010_0_N_die_5.png");
    soldier2->setPosition(pos+ccp(40,-15));
    soldierBN->addChild(soldier2);
    
    auto soldier2_1 = CCLoadSprite::createSprite("a030_0_N_die_5.png");
    soldier2_1->setPosition(pos+ccp(-50,-25));
    soldierBN->addChild(soldier2_1);
    
    auto soldier2_2 = CCLoadSprite::createSprite("a030_0_N_die_5.png");
    soldier2_2->setPosition(pos+ccp(8,-30));
    soldierBN->addChild(soldier2_2);
    
    auto soldier2_3 = CCLoadSprite::createSprite("a020_0_N_die_5.png");
    soldier2_3->setPosition(pos+ccp(-70,100));
    soldierBN->addChild(soldier2_3);
    
    for (int j=1; j<=5; j++) {
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",fireName.c_str(),j)->getCString());
        particle1->setPosition(pos);
        addPar(parVec,m_failEffectNode,particle1);
    }
    
    string tmpFile = "CombustionUI_";
    for (int i=1; i<=2; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpFile.c_str(),i)->getCString());
        particle->setPosition(pos);
        m_failEffectNode->addChild(particle);
    }
    
    //下陷的点
    pos = ccp(976,1189);
    auto bg3 = CCLoadSprite::createSprite("fireBomb.png");
    bg3->setScale(1.5);
    bg3->setOpacity(120);
    bg3->setPosition(pos);
    bombBN->addChild(bg3);
    
    auto soldier3 = CCLoadSprite::createSprite("a010_0_N_die_5.png");
    soldier3->setPosition(pos+ccp(-40, -40));
    soldierBN->addChild(soldier3);
    
    auto soldier3_1 = CCLoadSprite::createSprite("a030_0_NW_die_5.png");
    soldier3_1->setPosition(pos+ccp(80, 90));
    soldierBN->addChild(soldier3_1);
    
    auto soldier3_2 = CCLoadSprite::createSprite("a030_0_S_die_5.png");
    soldier3_2->setPosition(pos+ccp(100, 120));
    soldierBN->addChild(soldier3_2);
    
    for (int j=1; j<=5; j++) {
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",fireName.c_str(),j)->getCString());
        particle1->setPosition(pos);
        addPar(parVec,m_failEffectNode,particle1);
    }
    
    //大本营前面的点
    pos = ccp(1152,1509);
    auto bg4 = CCLoadSprite::createSprite("fireBomb.png");
    bg4->setScale(2);
    bg4->setOpacity(120);
    bg4->setPosition(pos);
    bombBN->addChild(bg4);
    
    auto soldier4 = CCLoadSprite::createSprite("a010_0_N_die_5.png");
    soldier4->setPosition(pos+ccp(30,-30));
    soldierBN->addChild(soldier4);
    
    auto soldier4_1 = CCLoadSprite::createSprite("a020_0_S_die_5.png");
    soldier4_1->setPosition(pos+ccp(-30,-30));
    soldierBN->addChild(soldier4_1);
    
    auto soldier4_2 = CCLoadSprite::createSprite("a030_0_N_die_5.png");
    soldier4_2->setPosition(pos+ccp(90,-20));
    soldierBN->addChild(soldier4_2);
    
    for (int j=1; j<=5; j++) {
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",fireName.c_str(),j)->getCString());
        particle1->setPosition(pos);
        addPar(parVec,m_failEffectNode,particle1);
    }
    
    //右前的点
    pos = ccp(1400,1718);
    auto bg5 = CCLoadSprite::createSprite("fireBomb.png");
    bg5->setScale(1.5);
    bg5->setOpacity(120);
    bg5->setPosition(pos);
    bombBN->addChild(bg5);
    
    auto soldier5 = CCLoadSprite::createSprite("a010_0_N_die_5.png");
    soldier5->setPosition(pos+ccp(30,-30));
    soldierBN->addChild(soldier5);
    
    auto soldier5_1 = CCLoadSprite::createSprite("a030_0_N_die_5.png");
    soldier5_1->setPosition(pos+ccp(-30,-30));
    soldierBN->addChild(soldier5_1);
    
    for (int j=1; j<=5; j++) {
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",fireName.c_str(),j)->getCString());
        particle1->setScale(0.8);
        particle1->setPosition(pos);
        addPar(parVec,m_failEffectNode,particle1);
    }
    
    //左前的点
    pos = ccp(47,1222);
    auto bg6 = CCLoadSprite::createSprite("fireBomb.png");
    bg6->setScale(2);
    bg6->setOpacity(120);
    bg6->setPosition(pos);
    bombBN->addChild(bg6);
    
    auto soldier6 = CCLoadSprite::createSprite("a010_0_N_die_5.png");
    soldier6->setPosition(pos+ccp(30,-30));
    soldierBN->addChild(soldier6);
    
    auto soldier6_1 = CCLoadSprite::createSprite("a030_0_NW_die_5.png");
    soldier6_1->setPosition(pos+ccp(50,-10));
    soldierBN->addChild(soldier6_1);
    
    auto soldier6_2 = CCLoadSprite::createSprite("a030_0_N_die_5.png");
    soldier6_2->setPosition(pos+ccp(-30,-20));
    soldierBN->addChild(soldier6_2);
    
    for (int j=1; j<=5; j++) {
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",fireName.c_str(),j)->getCString());
        particle1->setPosition(pos);
        addPar(parVec,m_failEffectNode,particle1);
    }
    
    //左下排兵的点
    pos = ccp(513,705);
    auto bg7 = CCLoadSprite::createSprite("fireBomb.png");
    bg7->setScale(2.5);
    bg7->setOpacity(GlobalData::shared()->getRand(80,120));
    bg7->setPosition(pos);
    bombBN->addChild(bg7);
    
    auto soldier7 = CCLoadSprite::createSprite("a010_0_N_die_5.png");
    soldier7->setPosition(pos+ccp(30,-40));
    soldierBN->addChild(soldier7);
    
    auto soldier7_1 = CCLoadSprite::createSprite("a030_0_NW_die_5.png");
    soldier7_1->setPosition(pos+ccp(-50,-10));
    soldierBN->addChild(soldier7_1);
    
    auto soldier7_2 = CCLoadSprite::createSprite("a030_0_N_die_5.png");
    soldier7_2->setPosition(pos+ccp(20,-60));
    soldierBN->addChild(soldier7_2);
    
    auto soldier7_3 = CCLoadSprite::createSprite("a020_0_N_die_5.png");
    soldier7_3->setPosition(pos+ccp(-80,40));
    soldierBN->addChild(soldier7_3);
    
    for (int j=1; j<=5; j++) {
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("%s%d",fireName.c_str(),j)->getCString());
        particle1->setPosition(pos);
        addPar(parVec,m_failEffectNode,particle1);
    }
    
    pos = ccp(576,1291);
    auto bg8 = CCLoadSprite::createSprite("fireBomb.png");
    bg8->setScale(1.5);
    bg8->setOpacity(GlobalData::shared()->getRand(80,120));
    bg8->setPosition(pos);
    bombBN->addChild(bg8);
    
    auto soldier8 = CCLoadSprite::createSprite("a020_0_S_die_5.png");
    soldier8->setPosition(pos+ccp(GlobalData::shared()->getRand(-30,-60), GlobalData::shared()->getRand(-30,-70)));
    soldierBN->addChild(soldier8);
    
    auto soldier8_1 = CCLoadSprite::createSprite("a030_0_N_die_5.png");
    soldier8_1->setPosition(pos+ccp(GlobalData::shared()->getRand(-60,30), GlobalData::shared()->getRand(-50,50)));
    soldierBN->addChild(soldier8_1);
    
    pos = ccp(806,1330);
    auto bg9 = CCLoadSprite::createSprite("fireBomb.png");
    bg9->setScale(1.5);
    bg9->setOpacity(GlobalData::shared()->getRand(90,110));
    bg9->setPosition(pos);
    bombBN->addChild(bg9);
    
    auto soldier9 = CCLoadSprite::createSprite("a010_0_N_die_5.png");
    soldier9->setPosition(pos+ccp(GlobalData::shared()->getRand(-40,40), GlobalData::shared()->getRand(-30,70)));
    soldierBN->addChild(soldier9);
    
    auto soldier9_1 = CCLoadSprite::createSprite("a020_0_W_die_5.png");
    soldier9_1->setPosition(pos+ccp(30, GlobalData::shared()->getRand(-50,70)));
    soldierBN->addChild(soldier9_1);
    
    
    auto bg10 = CCLoadSprite::createSprite("fireBomb.png");
    bg10->setScale(0.7);
    bg10->setOpacity(GlobalData::shared()->getRand(90,110));
    bg10->setPosition(ccp(1887, 1137));
    bombBN->addChild(bg10);
    
    auto bg11 = CCLoadSprite::createSprite("fireBomb.png");
    bg11->setScale(0.9);
    bg11->setOpacity(GlobalData::shared()->getRand(80,110));
    bg11->setPosition(ccp(1107, 985));
    bombBN->addChild(bg11);
    
    auto bg12 = CCLoadSprite::createSprite("fireBomb.png");
    bg12->setScale(0.6);
    bg12->setOpacity(GlobalData::shared()->getRand(80,110));
    bg12->setPosition(ccp(1648, 1481));
    bombBN->addChild(bg12);
    
    auto bg13 = CCLoadSprite::createSprite("fireBomb.png");
    bg13->setScale(0.7);
    bg13->setOpacity(GlobalData::shared()->getRand(80,100));
    bg13->setPosition(ccp(836, 606));
    bombBN->addChild(bg13);
    
    auto bg14 = CCLoadSprite::createSprite("fireBomb.png");
    bg14->setScale(0.8);
    bg14->setOpacity(GlobalData::shared()->getRand(70,110));
    bg14->setPosition(ccp(1785, 1313));
    bombBN->addChild(bg14);
    
    auto bg15 = CCLoadSprite::createSprite("fireBomb.png");
    bg15->setScale(0.6);
    bg15->setOpacity(GlobalData::shared()->getRand(80,100));
    bg15->setPosition(ccp(1343, 1131));
    bombBN->addChild(bg15);
    
    auto bg16 = CCLoadSprite::createSprite("fireBomb.png");
    bg16->setScale(0.7);
    bg16->setOpacity(GlobalData::shared()->getRand(70,100));
    bg16->setPosition(ccp(1445, 1370));
    bombBN->addChild(bg16);
}
void ImperialScene::updateShipState(string state){
    if(m_shipBuild){
        m_shipBuild->updateShipState(state);
    }
}

void ImperialScene::onStopAllActions(CCObject* params)
{
    m_touchLayer->stopAllActions();
}

CCBAnimationManager* ImperialScene::getAnimationManager()
{
    return mActionManager;
}

void ImperialScene::setAnimationManager(CCBAnimationManager *pAnimationManager)
{
    mActionManager = pAnimationManager;
}

void ImperialScene::onRemoveGuideParticle(CCObject* obj)
{
    if (m_guideParticleNode) {
        m_guideParticleVec.clear();
        m_guideParticleNode->removeAllChildren();
        m_guideParticleNode->removeFromParent();
    }
}

void ImperialScene::addGuideParticleToBatch(cocos2d::CCParticleSystemQuad *particle)
{
    auto batchCount = m_guideParticleVec.size();
    while (batchCount--) {
        auto &batch = m_guideParticleVec[batchCount];
        if (batch) {
            auto batchBlend = batch->getBlendFunc();
            auto particleBlend = particle->getBlendFunc();
            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                batch->addChild(particle);
                return;
            }
        }
    }
    auto newBatch = ParticleController::createParticleBatch();
    m_guideParticleNode->addChild(newBatch);
    newBatch->addChild(particle);
    m_guideParticleVec.push_back(newBatch);
}

CCSpriteBatchNode* ImperialScene::createCustomBatchLayer(string image){
    if(m_rescustombatchNode){
        auto &array = m_rescustombatchNode->getChildren();
        CCTexture2D *texture = m_rescustombatchNode->getTexture();
        if(texture){
            string releaseFile = texture->getFileName();
            auto pos1 = releaseFile.find_last_of("/");
            std::string picName = "";
            if(pos1!=std::string::npos){
                picName = releaseFile.substr(pos1+1);
                pos1 = picName.find_first_of(".");
                if(pos1!=std::string::npos){
                    picName = picName.substr(0,pos1);
                }
            }
            if(picName!=image){
                auto pos =releaseFile.find_last_of(".");
                releaseFile = releaseFile.substr(0,pos);
                pos =releaseFile.find_last_of(".");
                releaseFile = releaseFile.substr(0,pos);
                releaseFile.append(".plist");
//                cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(releaseFile.c_str());
//                CCTextureCache::sharedTextureCache()->removeTexture(texture);
                DynamicResourceController::getInstance()->loadNameTypeResource(picName, true);
                mMaincitySkin="";
                m_rescustombatchNode->removeFromParent();
                m_rescustombatchNode=NULL;
            }
        }
    }
    if(m_rescustombatchNode == nullptr){
        DynamicResourceController::getInstance()->loadNameTypeResource(image, false);
        mMaincitySkin = image;
        image.append(".png");
        auto resSp = CCLoadSprite::loadResource(image.c_str());//dsg_house_1
        resSp->getTexture()->setAntiAliasTexParameters();
        m_rescustombatchNode = CCSpriteBatchNode::createWithTexture(resSp->getTexture());
        m_touchLayer->addChild(m_rescustombatchNode,200);
        m_rescustombatchNode->setPosition(m_resbatchNode->getPosition());
        return m_rescustombatchNode;
    }else{
        m_rescustombatchNode->removeAllChildren();
        return m_rescustombatchNode;
    }
    return NULL;
}
void ImperialScene::removeCustomBatchLayer(){
    if(m_rescustombatchNode){
        m_rescustombatchNode->removeAllChildren();
        m_touchLayer->removeChild(m_rescustombatchNode);
        m_rescustombatchNode=NULL;
    }
}

void ImperialScene::firstEnterMainCity(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ChatServiceCocos2dx::loadingFinishedSingel();
#endif

}

void ImperialScene::showExtraBtns(bool visible){
    for (int i=17; i<=26; i++) {//城门门口10个地块
        int bid = FunBuildController::getInstance()->getBuildIdByPos(i);
        if (bid > 0) {
            FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[i]->getChildByTag(i));
            if (build) {
                build->setStateBtnsVisible(visible);
            }
        }
    }
}

Point ImperialScene::getMailPosOnScreen(){
    auto pt = onGetSceenPt(m_desNode1->getPosition().x, m_desNode1->getPosition().y);
    return pt;
}

Point ImperialScene::getNewRwdPosOnScreen(){
    auto pt = onGetSceenPt(m_newRDNode->getPosition().x, m_newRDNode->getPosition().y);
    return pt;
}

void ImperialScene::setQuestEffect(int type){
    m_questEffect = type;
}

void ImperialScene::onCheckNewPlayerScene(CCObject* obj)
{
    Node* tmpNode = m_battleLayer->getChildByTag(9);
    if (tmpNode == nullptr ) {
        NewPlayerBattleScene* newplayerScence = NewPlayerBattleScene::create();
        newplayerScence->setTag(9);
        newplayerScence->setVisible(false);
        m_battleLayer->addChild(newplayerScence);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_BEGIN3);
    }
}

void ImperialScene::checkTipsTime()
{
    if (m_tipsTime == 15) {
        m_tipsTime = 0;
        if (m_tipsType != -1) {
            m_tipsType++;
        }
        if (m_tipsType >= 4) {
            m_tipsType = 1;
        }
        if (m_crossServeActFlag == false && m_tipsType == 1) {
            m_tipsType = 2;
        }
        if (FunBuildController::getInstance()->getMainCityLv() < 5 && m_tipsType == 3) {
            m_tipsType = 1;
            if (m_crossServeActFlag == false && m_tipsType == 1) {
                m_tipsType = 2;
            }
        }
        switch (m_tipsType) {
            case 1:
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_IMPERIAL_SCENE_TIPS, Integer::create(1));
                break;
            case 2:
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_IMPERIAL_SCENE_TIPS, Integer::create(0));
                break;
            case 3:
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_IMPERIAL_SCENE_TIPS, Integer::create(3));
                break;
            default:
                break;
        }
    }
}

void ImperialScene::setTipsType(CCObject *obj)
{
    m_tipsType = 0;
}

void ImperialScene::reSetTipsType(CCObject *obj)
{
    m_tipsType = -1;
}

void ImperialScene::princessRwdNullShow(CCObject *obj)
{
    vector<string> tmpTalkInfos;
    tmpTalkInfos.push_back(FunBuildController::getInstance()->TalkViewContent);
    PopupViewController::getInstance()->addPopupView(NpcTalkView::create(tmpTalkInfos));
}

void ImperialScene::princessRwdShow(CCObject *obj)
{
    m_princessRwdType = -1;
    if(obj==NULL || (dynamic_cast<CCInteger*>(obj)==NULL)){
        m_princessRwdType = -1;
    }else {
        CCInteger* intObj = dynamic_cast<CCInteger*>(obj);
        m_princessRwdType = intObj->getValue();
    }
    princessRwdShowLater(0);
}

void ImperialScene::princessRwdShowLater(float t)
{
    vector<string> tmpTalkInfos;
    if (FunBuildController::getInstance()->TalkViewContent != "") {
        tmpTalkInfos.push_back(FunBuildController::getInstance()->TalkViewContent);
    }
    string temp = "";
    switch (m_princessRwdType) {
        case R_WOOD:{
            temp = _lang("3140017");
            break;
        }
        case R_STONE:{
            temp = _lang("3140020");
            break;
        }
        case R_IRON:{
            temp = _lang("3140019");
            break;
        }
        case R_FOOD:{
            temp = _lang("3140018");
            break;
        }
        case R_GOLD:{
            temp = _lang("3140022");
            break;
        }
        case R_GOODS:{
            temp = _lang("3140023");
            break;
        }
        case R_EXP:{
            temp = _lang("3140021");
            break;
        }
        default:
            break;
    }
    tmpTalkInfos.push_back(temp);
    auto view = NpcTalkView::create(tmpTalkInfos);
    view->setCallback(this, callfunc_selector(ImperialScene::princessRwdCallback));
    PopupViewController::getInstance()->addPopupView(view);
    m_princessRwdType = -1;
}

void ImperialScene::princessRwdCallback()
{
    if (m_princessRwdArr && m_princessRwdArr->count() > 0) {
        GCMRewardController::getInstance()->retReward3(m_princessRwdArr,0.5);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFREASH_TOOL_DATA);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
        m_princessRwdArr->removeAllObjects();
        PopupViewController::getInstance()->setPrincessShow(false);
    }
}

void ImperialScene::onShowPatrol(CCObject *obj)
{
    if (CCCommonUtils::isTestPlatformAndServer("Guide_Ani_Test")) {
        showExtraBtns(false);
        onMoveToPos(1920, 920, TYPE_POS_MID, 1.5, 1.5, true);
        scheduleOnce(schedule_selector(ImperialScene::onShowPatrolStart), 1.6);
    }
}

void ImperialScene::onShowPatrolStart(float t)
{
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_WALLSOILDER_VISIBLE, Integer::create(1));
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PERSON_VISIBLE, Integer::create(2));
    scheduleOnce(schedule_selector(ImperialScene::onShowPatrolEnd), 6.0);
}

void ImperialScene::onShowPatrolEnd(float t)
{
    showExtraBtns(true);
    onMoveToPos(1920, 920, TYPE_POS_MID, 1.0, 0.8, true);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("begin4"));
}

void ImperialScene::onShowRebuild(CCObject *obj)
{
    if (CCCommonUtils::isTestPlatformAndServer("Guide_Ani_Test")) {
        showExtraBtns(false);
        onMoveToPos(2300, 520, TYPE_POS_MID, 1.5, 1.5, true);
        scheduleOnce(schedule_selector(ImperialScene::onShowFarmer), 1.5);
    }
}

void ImperialScene::onShowFarmer(float t)
{
    onCleanFarmer();
    onCreateFarmer();
    scheduleOnce(schedule_selector(ImperialScene::onShowFarmerStart), 0.5);
    scheduleOnce(schedule_selector(ImperialScene::onShowFarmerEnd), 5.0);
}

void ImperialScene::onCreateFarmer()
{
    m_farmerNode = Node::create();
    auto spr1 = CCSprite::create();
    CCCommonUtils::makeEffectSpr(spr1, "gengtian_%d.png", 21);
    auto spr2 = CCSprite::create();
    CCCommonUtils::makeEffectSpr(spr2, "gengtian_%d.png", 21);
    auto spr3 = CCSprite::create();
    CCCommonUtils::makeEffectSpr(spr3, "gengtian_%d.png", 21);
    spr1->setPosition(ccp(103, 83));
    spr1->setAnchorPoint(ccp(0.5, 0.5));
    spr1->setTag(9997);
    m_farmerNode->addChild(spr1);
    spr2->setPosition(ccp(63, 63));
    spr2->setAnchorPoint(ccp(0.5, 0.5));
    spr2->setTag(9998);
    m_farmerNode->addChild(spr2);
    spr3->setPosition(ccp(109, 44));
    spr3->setAnchorPoint(ccp(0.5, 0.5));
    spr3->setTag(9999);
    m_farmerNode->addChild(spr3);
    m_farmerNode->setPosition(ccp(0, 0));
    if (m_nodeBuildings[21]) {
        m_farmerNode->setPosition(m_nodeBuildings[21]->getPosition());
        m_spineLayer->addChild(m_farmerNode);
    }
    spr1->setOpacity(0);
    spr2->setOpacity(0);
    spr3->setOpacity(0);
}

void ImperialScene::onCleanFarmer()
{
    if (m_farmerNode) {
        m_farmerNode->setVisible(false);
    }
}

void ImperialScene::onShowFarmerStart(float t)
{
    CCSequence *sequene = CCSequence::create(CCFadeTo::create(1.0, 255), NULL);
    CCSequence *sequene1 = CCSequence::create(CCFadeTo::create(1.0, 255), NULL);
    CCSequence *sequene2 = CCSequence::create(CCFadeTo::create(1.0, 255), NULL);
    m_farmerNode->getChildByTag(9997)->runAction(CCRepeat::create(sequene, 1));
    m_farmerNode->getChildByTag(9998)->runAction(CCRepeat::create(sequene1, 1));
    m_farmerNode->getChildByTag(9999)->runAction(CCRepeat::create(sequene2, 1));
    scheduleOnce(schedule_selector(ImperialScene::onShowFarmerEffect), 1.0);
}

void ImperialScene::onShowFarmerEnd(float t)
{
    showExtraBtns(true);
    onMoveToPos(2300, 520, TYPE_POS_MID, 1.0, 0.8, true);
    onCleanFarmer();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("begin5"));
}

void ImperialScene::onShowFarmerEffect(float t)
{
    int bid = FunBuildController::getInstance()->getBuildIdByPos(21);
    if (bid > 0) {
        FunBuild* build = dynamic_cast<FunBuild*>(m_nodeBuildings[21]->getChildByTag(21));
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        int startPosx = build->getParent()->getPositionX() + build->mainWidth/2 ;
        int startPosy = build->getParent()->getPositionY() + build->mainHeight/2;
        auto pt = onGetSceenPt(startPosx, startPosy);
        pt.y = pt.y-winSize.height;
        
        string picStr = "Food";
        auto endPt = ccp(0,0);
        int resType = Food;
        if (bid/1000 == FUN_BUILD_FOOD) {
            SoundController::sharedSound()->playEffects(Music_Sfx_city_harvest_farm);
            endPt = ccp(200,-60);//winSize.height
            if(CCCommonUtils::isIosAndroidPad())
            {
                endPt = ccpMult(endPt, 2.4);
            }
            picStr = "Food";
            resType = Food;
        }
        srand((unsigned)time(0));
        int tmp = 25;
        int idx = 0;
        while (tmp>0) {
            auto flyCell = FlyCell::create(pt, endPt,resType, picStr, 5, UIComponent::getInstance()->m_collectBatch, idx*0.1);
            UIComponent::getInstance()->m_collectNode->addChild(flyCell);
            tmp--;
            idx++;
        }
    }
}

void ImperialScene::sqerSoldierAniComplete(CCObject *obj)
{
}

void ImperialScene::showMainCityBtnAni()
{
    onMoveToPos(700, 1600, TYPE_POS_MID, 1.0, 1.0, true);
    scheduleOnce(schedule_selector(ImperialScene::onShowMainCityBtn), 1.1);
}

void ImperialScene::onShowMainCityBtn(float t)
{
    string icon = "";
    if (FunBuildController::getInstance()->getMainCityLv() == 4) {
        icon = "tile_pop_icon20.png";
    }
    else if (FunBuildController::getInstance()->getMainCityLv() == 5)
    {
        icon = "cityskin_status.png";
    }
    else
        return;
    
    if (m_mainCityBtnAni == NULL) {
        m_mainCityBtnAni = Node::create();
    }
    Node* btnNode = Node::create();
    auto btnBg = CCLoadSprite::createSprite("bnt_02.png");
    btnNode->addChild(btnBg);
    auto sprite = CCLoadSprite::createSprite(icon.c_str());
    btnNode->addChild(sprite);
    m_mainCityBtnAni->addChild(btnNode);
    
    auto topLayer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_POPUP);
    int centerX = CCDirector::sharedDirector()->getWinSize().width / 2;
    int centerY = CCDirector::sharedDirector()->getWinSize().height / 2;
    
    topLayer->addChild(m_mainCityBtnAni);
    m_mainCityBtnAni->setPosition(centerX, centerY);
    
    CCActionInterval * particle = CCSequence::create(CCDelayTime::create(0.3), CCCallFunc::create(this, callfunc_selector(ImperialScene::addMainCityBtnParticle)), NULL);
    CCActionInterval * scaleto = CCScaleTo::create(1.0, 1.2);
    if (CCCommonUtils::isIosAndroidPad()) {
        scaleto = ScaleTo::create(1.0, 2.4);
    }
    CCActionInterval * easeBackOut1 = CCEaseBackOut ::create(scaleto);
    CCActionInterval * moveto = CCMoveTo ::create(0.5, ccp(centerX, 90 + centerY));
    CCActionInterval * easeBackOut2 = CCEaseBackOut ::create(moveto);
    CCFiniteTimeAction * spawn1 =CCSpawn::create(easeBackOut1,particle,easeBackOut2,NULL);
    
    CCActionInterval * delayT = CCDelayTime ::create(1.25);
    
    CCActionInterval * scaleto2 = CCScaleTo ::create(1.0, 0);
    CCActionInterval * CCEaseBackIn1 = CCEaseBackIn ::create(scaleto2);
    CCActionInterval * moveto1 = CCMoveTo ::create(1.0, ccp(centerX - 50, 500 + centerY));
    CCActionInterval * CCEaseBackIn2 = CCEaseBackIn ::create(moveto1);
    CCFiniteTimeAction * spawn2 =CCSpawn::create(CCEaseBackIn1,NULL);
    if (CCCommonUtils::isIosAndroidPad()) {
        spawn2 = CCSpawn::create(CCEaseBackIn1, CCEaseBackIn2, NULL);
    }
    
    m_mainCityBtnAni->setScale(0);
    m_mainCityBtnAni->setPosition(centerX, centerY);
    m_mainCityBtnAni->runAction(CCSequence::create(spawn1,delayT,spawn2,CCCallFunc::create(this, callfunc_selector(ImperialScene::mainCityBtnAniComplete)), NULL));
}

void ImperialScene::addMainCityBtnParticle()
{
    Node* node = Node::create();
    for (int i=1; i<=7; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("MallBag_%d",i)->getCString());
        particle->setPosition(0, 0);
        node->addChild(particle);
    }
    node->setZOrder(-1);
    m_mainCityBtnAni->addChild(node);
}

void ImperialScene::mainCityBtnAniComplete()
{
    if (m_mainCityBtnAni) {
        m_mainCityBtnAni->removeAllChildren();
        if (m_mainCityBtnAni->getParent()) {
            m_mainCityBtnAni->removeFromParent();
        }
    }
    
    if (FunBuildController::getInstance()->getMainCityLv() == 4) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("maincitybuff1"));
    }
    else if (FunBuildController::getInstance()->getMainCityLv() == 5) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("maincityskin1"));
    }
}

void ImperialScene::onShowMainCityBuff(CCObject *obj)
{
    showMainCityBtnAni();
}

void ImperialScene::onShowMainCitySkin(CCObject *obj)
{
    showMainCityBtnAni();
}

void ImperialScene::removeSqerTouchPar(float t)
{
    Node* parNode = m_sSqerTouchNode->getChildByTag(9999);
    if (parNode && parNode->getParent()) {
        parNode->removeFromParentAndCleanup(true);
    }
}

void ImperialScene::shakeViewport(CCObject *obj)
{
    m_viewPort->shakeViewport(25.0,2.2);
}

void ImperialScene::InitDragonCaveGlow()
{
    auto glowSprite1 = CCLoadSprite::createSprite("dragonCaveShine.png");
    ccBlendFunc cbf = {GL_SRC_ALPHA,GL_ONE};
    glowSprite1->setBlendFunc(cbf);
    glowSprite1->setScale(1.5);
    Sequence* sequence1 = Sequence::create(FadeTo::create(2.5, 120), FadeTo::create(2.5, 255), NULL);
    glowSprite1->runAction(RepeatForever::create(sequence1));
    glowSprite1->setTag(8888);
    m_dragonCaveGlow->addChild(glowSprite1);
}

void ImperialScene::princessShotAni(int type)
{
    if (m_princessShotAni) {
        if (type == 0) {
            spTrackEntry* entry = m_princessShotAni->setAnimation(0, "sj", false);
            this->scheduleOnce(schedule_selector(ImperialScene::onShowPrincessHappy), 1.5);
        }else if (type == 1) {
            spTrackEntry* entry = m_princessShotAni->setAnimation(0, "wsz", false);
            this->scheduleOnce(schedule_selector(ImperialScene::onShowPrincessSad), 1.5);
        }else if (type == 2) {
            spTrackEntry* entry = m_princessShotAni->setAnimation(0, "hh", false);
            this->scheduleOnce(schedule_selector(ImperialScene::onShowPrincessShotEnd), 1.5);
        }else if (type == 3) {
            spTrackEntry* entry = m_princessShotAni->setAnimation(0, "sl", false);
            this->scheduleOnce(schedule_selector(ImperialScene::onShowPrincessShotEnd), 1.5);
        }
    }
}

void ImperialScene::onShowPrincessHappy(float t)
{
    princessShotAni(2);
}

void ImperialScene::onShowPrincessSad(float t)
{
    princessShotAni(3);
}

void ImperialScene::onShowPrincessShotEnd(float t)
{
    m_princessAniEnd = true;
    spTrackEntry* entry = m_princessShotAni->setAnimation(0, "dj", true);
}

Node* ImperialScene::getTouchLayer()
{
    return m_touchLayer;
}

void ImperialScene::allianceRecommandShow(CCObject *obj)
{
    CCLOGFUNC("allianceRecommand");
    if (PopupViewController::getInstance()->getCurrViewCount()==0) {
        CCLOGFUNC("allianceRecommandPopup");
        AllianceManager::getInstance()->showRecAlliance();
    }
}

void ImperialScene::onShowGuideWhenFree(CCObject *obj)
{
    if (m_guideFreeTime > 300 && !GuideController::share()->isInTutorial() && PopupViewController::getInstance()->getCurrViewCount() == 0 && m_princessWalkNode==NULL && m_curGuideEnd == false && m_princessWalkStart == false)
    {
        if (ArmyController::getInstance()->getTotalDead()>0 && CCCommonUtils::isTestPlatformAndServer("Guide_Hospital") && (!FunBuildController::getInstance()->IsHaveRecordeByKey(CLIENT_RECORD_HOSPITAL1) || !FunBuildController::getInstance()->IsHaveRecordeByKey(CLIENT_RECORD_HOSPITAL2))) {//伤兵引导 触发两次 向后台存两个不同的数据
            FunBuildInfo& main = FunBuildController::getInstance()->getFunbuildById(FUN_BUILD_MAIN_CITY_ID);
            int bid = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_HOSPITAL);
            if(main.level >= 4 && main.level <=9 && bid > 0){
                if (!FunBuildController::getInstance()->IsHaveRecordeByKey(CLIENT_RECORD_HOSPITAL1)) {
                    GuideController::share()->checkSubGuide("3270100",false);
                }
                else if (!FunBuildController::getInstance()->IsHaveRecordeByKey(CLIENT_RECORD_HOSPITAL2)) {
                    GuideController::share()->checkSubGuide("3270100");
                }
            }
        }
        m_guideFreeTime = 0;
    }
}


void ImperialScene::addEnergyEff(CCObject *obj){
        int m_flyX = CCDirector::sharedDirector()->getWinSize().width / 2;
        int m_flyY = CCDirector::sharedDirector()->getWinSize().height / 2;
        int toX = 80;
        int toY = CCDirector::sharedDirector()->getWinSize().height-80;
        CCPoint startPoint = ccp(m_flyX, m_flyY);
        CCPoint endPoint = ccp(toX, toY);

        string tmpStart = "Collection_expF_";
        int count = 3;
        for (int i=1; i<=count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
            if (i == 2) {
                particle->setPositionType(kCCPositionTypeFree);
            } else {
                particle->setPositionType(kCCPositionTypeRelative);
            }
            particle->setStartColor(ccc4f(0, 1.0, 0, 1.0));
            particle->setEndColor(ccc4f(0, 0, 0, 0));
            particle->setPosition(ccp(m_flyX, m_flyY));
            this->addChild(particle);
            
            CCActionInterval * moveTo = CCMoveTo::create(1.5,ccp(toX, toY));
            CCActionInterval * easeExponentialIn= CCEaseExponentialOut::create(moveTo);
            
            CCActionInterval * delT = CCDelayTime::create(0.3);
            CCActionInterval * fadeOut = CCFadeOut::create(0.2);
            auto func = CCCallFuncO::create(this, callfuncO_selector(ImperialScene::onFlyTroopEnd), particle);
            
            auto funcUI = CCCallFuncO::create(this, callfuncO_selector(ImperialScene::onFlyTroopEndToUI), CCString::create("stamine"));
            CCActionInterval * seqFadeOut = CCSequence::create(delT, funcUI, fadeOut, func, NULL);
            CCActionInterval * seqFadeOut1 = CCSequence::create(delT,func, NULL);
            particle->runAction(CCSpawn::create(easeExponentialIn,seqFadeOut,seqFadeOut1,NULL));
        }
}

void ImperialScene::initPrincessTask()
{
    if (CCCommonUtils::isTestPlatformAndServer("princess_quest")) {
        auto badge = CCLoadSprite::createSprite("princess_badge.png");
        auto flag = CCLoadSprite::createSprite("princess_flag.png");
        badge->setPosition(1120, 1500);
        flag->setPosition(1085, 1565);
        flag->setTag(999888);
        m_tilebatchNode->addChild(badge);
        m_tilebatchNode->addChild(flag);
    }
}
