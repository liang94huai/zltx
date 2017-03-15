//
//  LoadingScene.cpp
//  IF
//
//  Created by  on 13-8-28.
//
//

#include "LoadingScene.h"
#include "../../Ext/CCDevice.h"
#include "SceneController.h"
#include "SoundController.h"
#include "LoginCommand.h"
#include "PopupViewController.h"
#include "KoreaPermitPopUPView.h"
#include "AppLibHelper.h"
#include "NetController.h"
#include "YesNoDialog.h"
#include "ParticleController.h"
#include "InitCommand.h"
#include <spine/Json.h>
#include "IFLoadingSceneArmyNode.h"
#include "FunBuildController.h"
#include "IFSkeletonDataManager.h"
#include "md5.h"
#include "CCThreadManager.h"
#include "PermitionForServicePopupView.hpp"
#include "UIComponent.h"
#include "GuideController.h"
#include "FlySystemUpdateHint.h"
#include "ServerUpdateView.h"
#include "GmController.h"
#include "DynamicResourceController.h"
#include "PlayerInfoController.h"
#include "NoticeView.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "HoolaiFastAccess/GameProxy.h"
#include "HoolaiFastAccess/CCAccessSDK.h"
#else
#import <AccessSDK/CCAccessSDK.h>
#endif
#include "HoolaiSdkDelegate.hpp"
#include "HoolaiBiCommand.h"

#define MSG_MAIN_THREAD_XML 453
#define LOADING_STEP        80
#define LOADING_LOGO_TAG    666
#define LOADING_1 667
#define LOADING_2 668
#define LOADING_3 669
#define LOADING_4 670

#define LOADING_GLOW 671

#define MSG_SELECT_SERVER   "select.server.cell"



LoadingScene::~LoadingScene(){
}

LoadingScene::LoadingScene()
: m_steps(LOADING_STEP)
{
    
}

#pragma mark * init functions

void LoadingScene::addVersionLabel()
{
    string _uuid = cocos2d::extension::CCDevice::getDeviceUid();
    GlobalData::shared()->version = cocos2d::extension::CCDevice::getVersionName();
    
    // devil 版本号暂时不显示
    string _version = GlobalData::shared()->version.c_str();
    //        _version = _version.substr(0,_version.length()-3);
#if COCOS2D_DEBUG > 0
    CCLabelIF* label = CCLabelIF::create(CCString::createWithFormat("Inner-V: %s \n hudson Code: %s \n deviceID: %s",_version.c_str(),cocos2d::extension::CCDevice::getVersionCode().c_str(),_uuid.c_str())->getCString());
#else
    // devil 正式服暂时不显示版本号
    //    CCLabelIF* label = CCLabelIF::create(CCString::createWithFormat("V %s (%s)",_version.c_str(),cocos2d::extension::CCDevice::getVersionCode().c_str())->getCString());
    CCLabelIF* label = CCLabelIF::create("");
#endif
    
    label->setScale(0.5f);
    if (CCCommonUtils::isIosAndroidPad())
    {
        label->setScale(0.8f);
    }
    label->setFontSize(40);
    label->setColor({0,255,0});
    label->setAnchorPoint(CCPoint(1,1));
    label->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width-10,CCDirector::sharedDirector()->getWinSize().height));
    
    addChild(label);
}

bool LoadingScene::init()
{
    bool bRet = false;
    m_iTouchType = 0;
    m_isEnterGame = 0;
    m_curServerId = "";
    ParticleController::initParticle();
    if (CCLayer::init()) {
        if (CCCommonUtils::isIosAndroidPad() && !CCCommonUtils::getIsHDViewPort())
        {
            CCCommonUtils::setIsHDViewPort(true);
        }
        m_getServerListRetryTime = 0;
        CCLabelIF::initFont();
        GameController::getInstance();
        //loadingLog统计
        GameController::getInstance()->setLoadingLog("LoadingScene", "init");
        
        Size size = Director::getInstance()->getWinSize();
        CCBLoadFile("LoadingView",this,this);
        // bg
        m_loadingBg->setPosition(size.width * 0.5, size.height * 0.5);
        float scale = max(size.width / m_loadingBg->getContentSize().width, size.height / m_loadingBg->getContentSize().height);
        m_loadingBg->setScale(scale);

        m_logo->setPosition(size.width * 0.5, size.height - 50);
        // devil 动画效果
        auto kingSpine = IFLoadingSceneArmyNode::create("Loading/Loading_3.atlas", "Spine/Loading/loading.json", "animation", 1);
        m_spineNode->addChild(kingSpine);
        auto loading_niao = IFLoadingSceneArmyNode::create("Loading/Loading_3.atlas", "Spine/Loading/loading_niao.json", "animation", 1);
        loading_niao->setPosition(Vec2(170,656));
        m_spineNode->addChild(loading_niao);
        
        addVersionLabel();
        
        m_loadingBarBG->setPosition(ccp(size.width/2, 150));
        m_loadingBar->setPosition(ccp(size.width/2, 150));
        
        CCSize lpsz = m_loadingProgress->getContentSize();
        m_barSize = lpsz+CCSizeMake(0, 30);
        m_loadingProgress->setAnchorPoint(ccp(0, 0.5));
        m_loadingProgress->setPosition(ccp(0, 0));
        m_loadingProgress->removeFromParent();
        m_barClipNode = CCClipNode::create(m_barSize.width, m_barSize.height);
        m_barClipNode->addChild(m_loadingProgress);
        m_barClipNode->setPosition(ccp(size.width/2 - lpsz.width/2, 150 - lpsz.height ));
        m_barClipNode->setContentSize(CCSizeMake(1, m_barSize.height));
        addChild(m_barClipNode,1);
        
        m_loadingTips->enableStroke(ccBLACK, 1.0);
        m_loadingTips->setPosition(ccp(size.width/2, m_barClipNode->getPositionY()));
        
        
        setCleanFunction([](){
            CCLoadSprite::releaseResourceBySceneId(SCENE_ID_LOADING);
        });
        m_btnLogin->setTitleForState("登陆", cocos2d::extension::Control::State::NORMAL);
        m_btnLogin->setTitleForState("登陆", cocos2d::extension::Control::State::HIGH_LIGHTED);
        m_btnLogin->setTitleForState("登陆", cocos2d::extension::Control::State::SELECTED);
        _setProgressVisiable(false);
        initLoginLabel();
        initScrolView();
        m_serverListNode->setVisible(false);
        CCCommonUtils::addNewDizi(m_serverListViewNode);
        
        bRet = true;
    }
    
    CCLOG("test init end");
    return bRet;
}


bool LoadingScene::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spineNode", Node*, this->m_spineNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loadingBg", Sprite*, this->m_loadingBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnLogin", ControlButton*, this->m_btnLogin);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loadingBarBG", Sprite*, this->m_loadingBarBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loadingBar", Sprite*, this->m_loadingBar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loadingProgress", Sprite*, this->m_loadingProgress);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loadingTips", CCLabelIF*, this->m_loadingTips);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_logo", Sprite*, this->m_logo);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progressNode", Node*, this->m_progressNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectServerNode", Node*, this->m_selectServerNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_serverListNode", Node*, this->m_serverListNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sdkTouchNode", Node*, this->m_sdkTouchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_serverListTouchNode", Node*, this->m_serverListTouchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_serverListViewNode", Node*, this->m_serverListViewNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sdkLabel", CCLabelIF*, this->m_sdkLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_severLabel", CCLabelIF*, this->m_severLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_changeSeverLabel", CCLabelIF*, this->m_changeSeverLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_myServerName", CCLabelIF*, this->m_myServerName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allServerName", CCLabelIF*, this->m_allServerName);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotNode", Node*, this->m_hotNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newNode", Node*, this->m_newNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "my_serverlist", Node*, this->my_serverlist);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "all_serverlist", Node*, this->all_serverlist);
    
    return false;
}



SEL_CCControlHandler LoadingScene::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onLoginClick", LoadingScene::onLoginClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onEnterGame", LoadingScene::onEnterGame);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onNoticeCallback", LoadingScene::onNoticeCallback);
    return NULL;
}

void LoadingScene::onGetNoticeContent(CCHttpClient* client, CCHttpResponse* response)
{
    if (!response)
    {
        CCLOG("Get_Notice_Content,response failed!\nerror buffer: %s", response->getErrorBuffer());
        return;
    }
    int responseCode = response->getResponseCode();
    CCLOG("response code: %d", responseCode);
    if (!response->isSucceed())
    {
        CCLOG("response failed!\nerror buffer: %s", response->getErrorBuffer());
        return;
    }
    std::vector<char>* iter = response->getResponseData();
    std::string serverInfo(iter->begin(),iter->end());
    if(serverInfo == "")
    {
        CCLOG("response data is null!");
        PopupViewController::getInstance()->removeAllPopupView();
        YesNoDialog::showVariableTitle(_lang("E100092").c_str(),CCCallFuncO::create(this, NULL, NULL),_lang("E100075").c_str(),false);
        return;
    }
    
    Json *c = Json_create(serverInfo.c_str());
    if(!c){
        return;
    }

    string noticeContent = Json_getString(c,"noticeContent","");
    
    PopupViewController::getInstance()->addPopupView(NoticeView::create(noticeContent));
}

void LoadingScene::onNoticeCallback(CCObject *pSender, CCControlEvent event)
{
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl(__String::createWithFormat("http://%s:9000/gameservice/getservernotice",SERVERLIST_IP)->getCString());
    request->setRequestType(CCHttpRequest::Type::GET);
    request->setResponseCallback(this, httpresponse_selector(LoadingScene::onGetNoticeContent));
    request->setTag("Get_Notice_Content");
    CCHttpClient::getInstance()->setTimeoutForConnect(30);
    CCHttpClient::getInstance()->send(request);
    request->release();
}


bool LoadingScene::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!m_selectServerNode->isVisible())
        return false;
    if (!m_serverListNode->isVisible() && isTouchInside(this->m_sdkTouchNode, pTouch))
    {
        m_sdkTouchNode->setScale(1.05);
        m_iTouchType = 1;   //登录/注销按钮
        return true;

    }else if (!m_serverListNode->isVisible() && isTouchInside(this->m_serverListTouchNode, pTouch))
    {
        m_serverListTouchNode->setScale(1.05);
        m_iTouchType = 2;   //服务器列表
        return true;
    }else if (m_serverListNode->isVisible())
    {
        m_iTouchType = 3;
        return true;

    }
    return false;
}
void LoadingScene::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!m_selectServerNode->isVisible())
        return ;
    switch (m_iTouchType) {
            
        case 1:
            if (isTouchInside(this->m_sdkTouchNode, pTouch))
            {
                onLoginClick(NULL,Control::EventType::TOUCH_DOWN);
            }
            break;
            
        case 2:
            if (isTouchInside(this->m_serverListTouchNode, pTouch))
            {
                _setServerListViewVisiable(true);
            }
            break;
        case 3:
            if (m_serverListNode->isVisible() && !isTouchInside(this->m_serverListViewNode, pTouch))
            {
                _setServerListViewVisiable(false);
            }
            break;
    }
    m_sdkTouchNode->setScale(1);
    m_serverListTouchNode->setScale(1);
    m_iTouchType = 0;
    
}


void LoadingScene::initLoginLabel()
{
//    m_sdkLabel->setString("账号登录");
    
    m_sdkLabel->setString(_lang("new100042").c_str());
    m_severLabel->setString(_lang("new100044").c_str());
    m_changeSeverLabel->setString(_lang("new100045").c_str());
    m_myServerName->setString(_lang("new100046").c_str());
    m_allServerName->setString(_lang("new100047").c_str());
    
}

void LoadingScene::onLoginClick(CCObject *pSender, CCControlEvent event)
{
    if (!m_selectServerNode->isVisible())
        return;
    //登录账号后改成注销 方法
    if(GameController::m_isLoginSDK)
    {
        //注销
        auto _dialog = YesNoDialog::show(/*_lang("E100090").c_str()*/"你要退出这个账号吗?"
                                         , CCCallFunc::create([](){
             CCAccessSDK::sharedInstance()->logout();
        })
                                         , 0
                                         , true
                                         , nullptr
                                         );
        _dialog->showCancelButton();
       //会直接关闭游戏
        return;
    }
//    if (GameController::m_isInitSDK == 0) {
//        CCAccessSDK::sharedInstance()->initSDK(new HoolaiSdkDelegate());
//        GameController::m_isInitSDK = 1;
//    }
//    else {
    CCAccessSDK::sharedInstance()->login("");
//    }
    
}

void LoadingScene::onEnterGame(CCObject *pSender, CCControlEvent event)
{
    if (!m_selectServerNode->isVisible())
        return;
    //判断一下有没有登录,没有登录调用onLoginClick,登陆完 直接进游戏
    if (GameController::m_isLoginSDK == 0)
    {
        m_isEnterGame = 1;
        onLoginClick(NULL,Control::EventType::TOUCH_DOWN);
    }else{
        CCSafeNotificationCenter::getInstance()->postNotification(MSG_GameStart);
    }
}

void LoadingScene::_setProgressVisiable(bool bVisiable)
{
    m_progressNode->setVisible(bVisiable);
    m_btnLogin->setVisible(!bVisiable);
}

static std::thread* loadWorldMap = NULL;


void LoadingScene::logoutSDK(float dt) //退出游戏 时 主动注销
{
    //只有进入loadingscene 时才发送注销请求
    
    CCAccessSDK::sharedInstance()->logout();
    
}

void LoadingScene::onEnter(){
    CCLayer::onEnter();
    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    this->setTouchEnabled(true);
    
    //延时 发送SDK请求
    //只有进入loadingscene 时才发送注销请求
    if (GameController::m_isLoginSDK) //登录状态 应该注销
    {
        this->scheduleOnce(schedule_selector(LoadingScene::logoutSDK), 0.25);
    }
    
    CCSafeNotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LoadingScene::doLogin), MSG_GameStart, NULL);//
    CCSafeNotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LoadingScene::doLoginInitError), MSG_LoginInitError, NULL);//
    CCSafeNotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LoadingScene::doLoginInitBan), MSG_LoginInitBan, NULL);//
    CCSafeNotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LoadingScene::doResourceUpdate), MSG_LoginSucess, NULL);//
//    CCSafeNotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LoadingScene::doLoginInit), MSG_LoginInit, NULL);
    CCSafeNotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LoadingScene::doLoginInitFinish), MSG_LoginInitFinish, NULL);//
    
    CCSafeNotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LoadingScene::stepLoading), MSG_INIT_FINISH, NULL);//
    CCSafeNotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LoadingScene::onRelogin), MSG_RELOGIN, NULL);//
    
    CCSafeNotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LoadingScene::checkCanGotoMainScene), PERMITION_CHECK_CONFIRM,NULL);//
    CCSafeNotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LoadingScene::onNetworkAvailable), "NetworkAvailable", NULL);//

    CCSafeNotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LoadingScene::onServerUpdate), MSG_SERVER_UPDATE, NULL);//

    CCSafeNotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LoadingScene::sendCmdGetServerList), MSG_SdkLoginSuccess, NULL); //sdk callback
    CCSafeNotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LoadingScene::selectServerList), MSG_SELECT_SERVER, NULL); //选择cell
    CCSafeNotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LoadingScene::gameLogout), MSG_SdkLogoutSuccess, NULL); //注销
    std::string _shortItemType = GlobalData::shared()->shortItemType;
    if (!_shortItemType.compare(shortItemType_clearCache) )
    {
        auto _dialog = YesNoDialog::show(_lang("139505").c_str()
                                         , CCCallFuncO::create(this, callfuncO_selector(LoadingScene::clearCacheOK), NULL)
                                         , 0
                                         , true
                                         , CCCallFuncO::create(this, callfuncO_selector(LoadingScene::clearCacheCancel), NULL)
                                         );
        _dialog->showCancelButton();
        _dialog->setYesButtonTitle(_lang("101274").c_str());
        _dialog->setNoButtonTitle(_lang("108532").c_str());
        
        return;
        
    }
//    初始化的数据移到下面的方法
    onEnterInit();
}

void LoadingScene::onEnterInit()
{
    GlobalData::shared()->analyticID = cocos2d::extension::CCDevice::getPublishChannel();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    GlobalData::shared()->fromCountry = AppLibHelper::getLocalCountry();
#else
    // devil hoolai的android只有中国地区
    GlobalData::shared()->fromCountry = "CN";
#endif
    if (GlobalData::shared()->needLogin()) {
        if(GlobalData::shared()->analyticID != "cn_mi"
           && GlobalData::shared()->analyticID != "cn_mihy"
           ){
            GlobalData::shared()->platformUID = "";
            //重登时候清除掉悬浮窗等
            cocos2d::extension::CCDevice::doPlatformLoading();
        }
    }
    
    if(isChina()){
        
        auto layer = CCLayerColor::create(ccc4(0, 0, 0, 200), Director::getInstance()->getWinSize().width, 70);
        this->addChild(layer);
        layer->setAnchorPoint(ccp(0.5, 0));
        
        auto label = CCLabelIF::create(_lang("4100009").c_str());
        this->addChild(label);
        label->setDimensions(CCSize(CCDirector::sharedDirector()->getWinSize().width - 16, 0));
        label->setColor(ccc3(141, 94, 75));
        label->setFontSize(21);
        label->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, 35));
        label->setAlignment(kCCTextAlignmentCenter);
        label->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        
        auto addY = [](CCNode *node){
            if(node){
                int add = 30;
                if (CCCommonUtils::isIosAndroidPad())
                {
                    add *= 2.f;
                }
                node->setPositionY(node->getPositionY() + add);
            }
        };

        addY(m_barClipNode);
        addY(m_loadingTips);
        addY(m_loadingBarBG);
        addY(m_loadingBar);
        addY(m_loadingProgress);
        addY(m_btnLogin);
    }
    
    m_tipCount = 1;
    m_tipDialog = "";
    setLoadingTips();
    
    CCLog("LoadingScene onEnterInit end" );
}

void LoadingScene::_refreshSelectView()
{
    // sdk登录成功后调用
    // 选服界面选择服务器后调用
    CCLOG("_refreshSelectView");
    auto serverInfo = m_serverList.find(m_curServerId);
    if (serverInfo != m_serverList.end())
    {
        m_severLabel->setString(serverInfo->second.name.c_str());
    }
    refreshScrolView();
}

void LoadingScene::refreshSelectViewByScrol()
{
    CCLOG("refreshSelectViewByScrol");
    auto serverInfo = m_serverList.find(m_curServerId);
    if (serverInfo != m_serverList.end())
    {
        m_severLabel->setString(serverInfo->second.name.c_str());
    }

}
void LoadingScene::_setServerListViewVisiable(bool bVisiable)
{
    // 点击选服调用
    CCLOG("_setServerListViewVisiable");
    if (bVisiable)
    {
        //如果未登录账号
        if (GameController::m_isLoginSDK == 0)
        {
            onLoginClick(NULL,Control::EventType::TOUCH_DOWN);
            return;
        }
    }
    m_serverListNode->setVisible(bVisiable);
    //runAction
    if (bVisiable)
    {
        m_logo->setPositionY(CCDirector::sharedDirector()->getWinSize().height + 50);
    }else{
        m_logo->setPositionY(CCDirector::sharedDirector()->getWinSize().height - 50);
    }
}

void LoadingScene::initScrolView()  //服务器列表更新
{
    m_myScrollView = CCScrollView::create(my_serverlist->getContentSize());
    m_myScrollView->setDirection(kCCScrollViewDirectionVertical);
    my_serverlist->addChild(m_myScrollView);
    
    m_allScrollView = CCScrollView::create(all_serverlist->getContentSize());
    m_allScrollView->setDirection(kCCScrollViewDirectionVertical);
    all_serverlist->addChild(m_allScrollView);
}
void LoadingScene::refreshScrolView()
{
    auto it = m_serverList.begin();
    //所有服务器的
    for (int i = 0;it != m_serverList.end(); i++,it++)
    {
        int x = i%2;
        int y = i/2+1;
        auto cell = ServerInfoCell::create(1,it->second.id.c_str(),it->second.name.c_str(),it->second.name.c_str());
        cell->setPosition(x*283,300-70*y);
        m_allScrollView->addChild(cell);
    }
    //我自己的服务器
    auto serverInfo = m_serverList.find(m_curServerId);
    if (serverInfo != m_serverList.end())
    {
        auto cell = ServerInfoCell::create(0,serverInfo->second.id.c_str(),serverInfo->second.name.c_str(),serverInfo->second.name.c_str());
        cell->setPosition(0*283,300-70);
        m_myScrollView->addChild(cell);
    }else{
        auto cell = ServerInfoCell::create(0,m_serverList.begin()->second.id.c_str(),m_serverList.begin()->second.name.c_str(),m_serverList.begin()->second.name.c_str());
        cell->setPosition(0*283,300-70);
        m_myScrollView->addChild(cell);
    }
    
    
    CCLOG("refreshScrolView add view");
}

void LoadingScene::clearCacheOK(CCObject *obj)
{
    GlobalData::shared()->shortItemType = "";
    CCCommonUtils::clearGameCache();
    GameController::getInstance()->quitGame();
}

void LoadingScene::clearCacheCancel(CCObject *obj)
{
    GlobalData::shared()->shortItemType = "";
    onEnterInit();
}

void LoadingScene::onExit(){

    CCSafeNotificationCenter::getInstance()->removeObserver(this, "NetworkAvailable");//
    this->unschedule(schedule_selector(LoadingScene::show45SecondAlert));
    this->unschedule(schedule_selector(LoadingScene::resendGetServerList));
    CCSafeNotificationCenter::getInstance()->removeObserver(this, MSG_LoginInitError);//
    CCSafeNotificationCenter::getInstance()->removeObserver(this, MSG_LoginSucess);//
    CCSafeNotificationCenter::getInstance()->removeObserver(this, MSG_GameStart);//
    CCSafeNotificationCenter::getInstance()->removeObserver(this, MSG_LoginInitFinish);//
    CCSafeNotificationCenter::getInstance()->removeObserver(this, MSG_INIT_FINISH);//
    CCSafeNotificationCenter::getInstance()->removeObserver(this, MSG_RELOGIN);//
    
    CCSafeNotificationCenter::getInstance()->removeObserver(this, PERMITION_CHECK_CONFIRM);//
    CCSafeNotificationCenter::getInstance()->removeObserver(this, MSG_SERVER_UPDATE);//
    CCSafeNotificationCenter::getInstance()->removeObserver(this, MSG_LoginInitBan);//
    CCSafeNotificationCenter::getInstance()->removeObserver(this, MSG_SdkLoginSuccess);
    
    CCSafeNotificationCenter::getInstance()->removeObserver(this, MSG_SdkLogoutSuccess);
    
    CCSafeNotificationCenter::getInstance()->removeObserver(this, MSG_SELECT_SERVER);
    setTouchEnabled(false);
    IFSkeletonDataManager::shared()->purge();
    
    if( loadWorldMap != NULL ) {
        loadWorldMap->join();
        CC_SAFE_DELETE(loadWorldMap);
    }
    //初始化角色 //进入游戏  
    auto &playerInfo = GlobalData::shared()->playerInfo;
    if (playerInfo.uid != "")
    {
        //将服务器名字 加上
        auto serverInfo = m_serverList.find(m_curServerId);
        if (serverInfo != m_serverList.end())
        {
            playerInfo.serverName = m_serverList[m_curServerId].name.c_str();
        }
        PlayerInfoController::getInstance()->saveSDKdateEnter();
        if (CCUserDefault::sharedUserDefault()->getBoolForKey(playerInfo.uid.c_str(),true))
        {
            PlayerInfoController::getInstance()->saveSDKdateRole();
            CCUserDefault::sharedUserDefault()->setBoolForKey(playerInfo.uid.c_str(), false);
        }
    }
    
    CCLayer::onExit();
}

void LoadingScene::onServerUpdate(CCObject* p)
{
    CCLog("LoadingScene onServerUpdate" );
    
    if( m_rollHint == nullptr )
    {
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        m_rollHint = FlySystemUpdateHint::create(0,true,_lang("113864"),FLY_HINT_SERVER_STOP,"");
        if( m_rollHint )
        {
            addChild(m_rollHint);
            if (CCCommonUtils::isIosAndroidPad()) {
                m_rollHint->setPosition(ccp(size.width*0.5, size.height-420));
            } else {
                m_rollHint->setPosition(ccp(size.width*0.5, size.height-210));
            }
        }
    }
    
    PopupViewController::getInstance()->removeAllPopupView();
    PopupViewController::getInstance()->addPopupView( ServerUpdateView::create() );
    
    _setProgressVisiable(false);
}

void LoadingScene::onEnterFrame(CCObject* p)
{
    GameController::getInstance()->setLoadingLog("LoadingScene", "onEnterFrame");
    
    PopupViewController::getInstance()->removeAllPopupView();
    auto popupview = PopupViewController::getInstance()->getCurrentPopupView();
    auto yesnoView = dynamic_cast<YesNoDialog*>(popupview);
    if(!yesnoView)
    {
        YesNoDialog::showVariableTitle(_lang("E100085").c_str(),CCCallFuncO::create(this,NULL, NULL),_lang("E100075").c_str(),false);
    }
}

void LoadingScene::setLoadingTips(){
    // get tips content
    int level = CCUserDefault::sharedUserDefault()->getIntegerForKey(FUN_BUILD_MAIN_LEVEL);
    if(level<=0){
        level = 1;
    }
    auto tipsDict = LocalController::shared()->DBXMLManager()->getGroupByKey("loadingTips");
    if(!tipsDict){
        return;
    }
    m_loadingTipsArr.clear();
    CCDictElement *pObj;
    string loadingTip = "";
    CCDICT_FOREACH(tipsDict, pObj) {
        auto infoDict = _dict(pObj->getObject());
        auto levelRange = string(infoDict->valueForKey("level_range")->getCString());
        auto id = string(infoDict->valueForKey("id")->getCString());
        auto description = string(infoDict->valueForKey("description")->getCString());
        
        vector<string> levels;
        CCCommonUtils::splitString(levelRange, ";", levels);
        if( levels.size()<2 )
            continue;
        
        if (level < atoi(levels[0].c_str()) || level > atoi(levels[1].c_str())) {
            continue;
        }
        CCCommonUtils::splitString(description, ";", m_loadingTipsArr);
    }
    if( m_loadingTipsArr.size()<=0 )
        return;
    
    srand(time(NULL));
    if(m_loadingTipsArr.size()>0){
        int index = rand()%m_loadingTipsArr.size();
        loadingTip = _lang(m_loadingTipsArr[index].c_str());
        CCLOG("index=%d",index);
        m_tipDialog = m_loadingTipsArr[index];
        m_loadingTips->setString(loadingTip.c_str());
    }
}

void LoadingScene::updateLoadingTip(float _time){
    m_tipCount += 1;
    if(m_loadingTipsArr.size()>1 && m_loadingTips){
        int index = 0;
        string loadingTip = "";
        for (int i=0; i<50; i++) {
            index = rand()%m_loadingTipsArr.size();
            string dialog = m_loadingTipsArr[index];
            int pos = m_tipDialog.find(dialog);
            if (pos==-1) {
                loadingTip = _lang(m_loadingTipsArr[index].c_str());
                m_tipDialog.append(",");
                m_tipDialog.append(dialog);
                break;
            }
        }
        if(loadingTip!=""){
            m_loadingTips->setString(loadingTip.c_str());
        }
    }
    if (m_tipCount==m_loadingTipsArr.size()) {
        m_tipDialog = "";
        m_tipCount= 0;
    }
}


////根据不同情况选择登录方式
//void LoadingScene::selectLogin(float t){
//    //sdk登陆成功 获取服务器列表
//    sendCmdGetServerList(NULL);
//}

void LoadingScene::sendCmdGetServerList(CCObject* p){
    //sdk登录成功 按钮改成注销
    GameController::m_isLoginSDK = 1;
    m_sdkLabel->setString(_lang("new100043").c_str());  //注销
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "sendCmdGetServerList");
    
    CCHttpRequest* request = new CCHttpRequest();
    CCString* url;
    string _uuid = cocos2d::extension::CCDevice::getDeviceUid();
    string _platformUID = GlobalData::shared()->platformUID;
    string _channelUid = GlobalData::shared()->channelUid;
    string _platformToken = GlobalData::shared()->platformToken;
    string _platformUserUid = GlobalData::shared()->platformUserUid;
    string _platformProductId = GlobalData::shared()->platformProductId;
    string _Country = GlobalData::shared()->fromCountry;
    string _server_group = cocos2d::extension::CCDevice::getServerGroup();
    string param = "";
    
    auto currentTime = time(NULL);
    std::string timeStr = CC_ITOA(double(currentTime));
    
    std::string key = _uuid + timeStr + "@z4LugUwrDN0MqOJtzmHPK7Ss-k4taAUOZh1M3Yu9";
    MD5 md5(key);
    std::string md5Str = md5.toString();
    
    if (_platformUID != "") {
        string _PublishRegion = cocos2d::extension::CCDevice::getPublishChannel();
        param = CCString::createWithFormat("deviceId=%s&channel=%s&channelUid=%s&accessToken=%s&productId=%s&server_group=%s",_platformUserUid.c_str(),_platformUID.c_str(),_channelUid.c_str(),_platformToken.c_str(),_platformProductId.c_str(),_server_group.c_str())->getCString();
        // devil 临时用ip  192.168.20.55
//                url = CCString::createWithFormat("http://192.168.20.55:9000/gameservice/getserverlist?%s", param.c_str());
        url = CCString::createWithFormat("http://%s:9000/gameservice/getserverlist?%s", SERVERLIST_IP, param.c_str());
        
        //        url = CCString::createWithFormat("http://%s:9001/ZLTXProxy/serverlist",SERVERLIST_IP);
    }
    CCLOG("COK server list URL: %s",url->getCString());
    request->setUrl(url->getCString());
    request->setRequestType(CCHttpRequest::Type::GET);
    request->setResponseCallback(this, httpresponse_selector(LoadingScene::onGetServerList));
    request->setTag("get_server_list");
    CCHttpClient::getInstance()->setTimeoutForConnect(30);
    CCHttpClient::getInstance()->send(request);
    request->release();
    m_getServerListRetryTime++;
    int maxTime = 3;
    if(m_getServerListRetryTime <= maxTime){
        this->scheduleOnce(schedule_selector(LoadingScene::resendGetServerList), 2);
    }
}

void LoadingScene::resendGetServerList(float time){
    sendCmdGetServerList(NULL);
}

void LoadingScene::onGetServerList(CCHttpClient* client, CCHttpResponse* response)
{
    this->unschedule(schedule_selector(LoadingScene::resendGetServerList));
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "onGetServerList");
    
    if (!response)
    {
        PopupViewController::getInstance()->removeAllPopupView();
        YesNoDialog::showVariableTitle(_lang("E100048").c_str(),CCCallFuncO::create(this, callfuncO_selector(LoadingScene::sendCmdGetServerList), NULL),_lang("105248").c_str(),true);
        return;
    }
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        CCLOG("%s completed", response->getHttpRequest()->getTag());
        if (strcmp("get_server_list",response->getHttpRequest()->getTag())!=0) {
            
        }
    }
    m_responseCode = response->getResponseCode();
    CCLOG("response code: %d", m_responseCode);
    if (!response->isSucceed())
    {
        CCLOG("response failed!\nerror buffer: %s", response->getErrorBuffer());
        
        // lzy ping玩家设备和服务器连接状态并将数据上传服务器
        string _uid = GlobalData::shared()->playerInfo.uid;
        CCCommonUtils::sendHelpshiftLog(NetController::shared()->getIp().c_str(), _uid.c_str(), "getIP-onGetServerList-failed");
        CCCommonUtils::sendHelpshiftLog(NetController::shared()->getRealIp().c_str(), _uid.c_str(), "getRealIP-onGetServerList-failed");
        
        PopupViewController::getInstance()->removeAllPopupView();
        YesNoDialog::showVariableTitle(_lang("E100048").c_str(),CCCallFuncO::create(this, callfuncO_selector(LoadingScene::sendCmdGetServerList), NULL),_lang("105248").c_str(),true);
        return;
    }
    std::vector<char>* iter = response->getResponseData();
    std::string serverInfo(iter->begin(),iter->end());
    if(serverInfo == "")
    {
        CCLOG("response data is null!");
        PopupViewController::getInstance()->removeAllPopupView();
        YesNoDialog::showVariableTitle(_lang("E100092").c_str(),CCCallFuncO::create(this, NULL, NULL),_lang("E100075").c_str(),false);
        return;
    }
    //    {"id":3,"name":"测试2","ip":"192.168.20.206","port":"9933","zone":"2","gameUid":"117828747","uuid":"117828747","buildCode":"117828747"}
    //    serverInfo = "{'serverList':{'id':'1','name':'inner1','ip':'120.27.105.196','port':'9933','zone':'COK1'}}";
    Json *c = Json_create(serverInfo.c_str());
    if(!c){
        return;
    }
    Json *serverList=Json_getItem(c,"serverList");
    
    if(!serverList){
        return;
    }
    GlobalData::shared()->loginServerInfo.recommandCountry = Json_getInt(c,"country",0);
    
    int serverNum = Json_getSize(serverList);
    for (int i = 0; i < serverNum; ++i) {
        Json *item0 = Json_getItemAt(serverList,i);
        sServerListCellInfo info;
        info.ip = Json_getString(item0, "ip","-1");
        info.port = Json_getString(item0, "port","-1");
        info.zone = Json_getString(item0, "zone","-1");
        info.name = Json_getString(item0, "name","inner0");
        info.id = Json_getString(item0,"id","-1");
        info.gameUid = Json_getString(item0, "gameUid", "");
        info._uuid = Json_getString(item0, "uuid", "");
        info._buildCode = Json_getString(item0, "buildCode", "");
        info.lastLogin = Json_getString(item0, "lastLogin", "");
        m_serverList[info.id] = info;
    }
    
    Json_dispose(c);
    
    // devil 通过json初始化登陆列表
    //本地上次服务器ID 判断
    auto lastServerID = CCUserDefault::sharedUserDefault()->getStringForKey(SERVER_ID, "");
    if (lastServerID != "")
    {
        m_curServerId = lastServerID.c_str();
    }else
        m_curServerId = "1"; // 暂时外网为1 小龙为2
    // devil 显示默认选择服务器
    _refreshSelectView();
    
}

void LoadingScene::doLoginInit(float t)
{
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "doLoginInit");
    if (!GlobalData::shared()->isInitFlag && !GlobalData::shared()->isInDataParsing)
    {
        CCLog( "zym doLoginInit");
        GameController::getInstance()->setLoadingLog("LoadingScene", "doLoginInit-false");
        InitCommand* cmd=new InitCommand();
        cmd->sendAndRelease();
    }
    else
    {
        unschedule(schedule_selector(LoadingScene::doLoginInit));
    }
}

void LoadingScene::doLoginInitFinish(Ref* p)
{
    schedule(schedule_selector(LoadingScene::doLoginInit), 1.0f);
}

void LoadingScene::doLoginInitError(CCObject* p){
    unschedule(schedule_selector(LoadingScene::doLoginInit));
    PopupViewController::getInstance()->removeAllPopupView();
    YesNoDialog::showVariableTitle(_lang("E100211").c_str(),CCCallFuncO::create(this, NULL, NULL),_lang("E100075").c_str(),false);
}

void LoadingScene::doLoginInitBan(CCObject* p)
{
    unschedule(schedule_selector(LoadingScene::doLoginInit));
    this->unschedule(schedule_selector(LoadingScene::show45SecondAlert));
}

void LoadingScene::show45SecondAlert(float time){
    
    this->unschedule(schedule_selector(LoadingScene::show45SecondAlert));
    
    PopupViewController::getInstance()->removeAllPopupView();
    
    auto _dialog = YesNoDialog::show(_lang("E100093").c_str()
                                     , CCCallFuncO::create(this, callfuncO_selector(LoadingScene::onRelogin), NULL)
                                     );
    _dialog->showCancelButton();
    _dialog->setYesButtonTitle(_lang("105248").c_str());
    _dialog->setNoButtonTitle(_lang("E100075").c_str());
}

void LoadingScene::onRelogin(CCObject* p)
{
    CCSafeNotificationCenter::getInstance()->postNotification(MSG_GameStart);
}


void LoadingScene::doLogin(CCObject* p)
{
    if (m_curServerId != "") {
        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID, m_serverList[m_curServerId]._uuid);
        CCUserDefault::sharedUserDefault()->setStringForKey(SERVER_ID, m_serverList[m_curServerId].id);
        CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, m_serverList[m_curServerId].gameUid);
        CCUserDefault::sharedUserDefault()->setStringForKey(BUILDCODE, m_serverList[m_curServerId]._buildCode);
        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, m_serverList[m_curServerId].ip);
        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, m_serverList[m_curServerId].zone);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT,atoi(m_serverList[m_curServerId].port.c_str()));
        
        //暂时方法,存区服名字
        CCUserDefault::sharedUserDefault()->setStringForKey("server_name", m_serverList[m_curServerId].name.c_str());
        CCUserDefault::sharedUserDefault()->flush();
        
        NetController::shared()->setIp(m_serverList[m_curServerId].ip);
        NetController::shared()->setZone(m_serverList[m_curServerId].zone);
    }
    
    sendCmdLogin();
    // devil 隐藏按钮和其他界面 显示进度条
    m_selectServerNode->setVisible(false);
    m_serverListNode->setVisible(false);
    showLoading(NULL);
}

void LoadingScene::sendCmdLogin()
{
    CCLog("in sendCmdLogin");
    schedule(schedule_selector(LoadingScene::onLoginTimeout), 0.5);
    // password retry count reset
    GlobalData::shared()->isLoginFlag = true;
    
    if( NetController::shared()->getIslogining() )
    {
        CCLog("zym doLogin skip");
        GameController::getInstance()->setLoadingLog("LoadingScene", "doLoginskip");
        //zym  测试一个
        if( GlobalData::shared()->analyticID == "cn1" )
            return;
    }
    else
        GameController::getInstance()->setLoadingLog("LoadingScene", "doLogin");
    
    NetController::shared()->setIslogining(true);
    
    NetController::shared()->clearRequestQueue();
    LoginCommand* cmd=new LoginCommand();
    cmd->setDefaultUserName();
    cmd->setFailCallback(CCCallFuncO::create(this, callfuncO_selector(LoadingScene::onConnectionLost), NULL));
    cmd->sendAndRelease();
}

void LoadingScene::onLoginTimeout(float t)
{
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "onLoginTimeout");
    unschedule(schedule_selector(LoadingScene::onLoginTimeout));
    CCHttpRequest* request = new CCHttpRequest();
    CCString* url;
    string _uuid = cocos2d::extension::CCDevice::getDeviceUid();
    string _platformUID = GlobalData::shared()->platformUID;
    string _Country = GlobalData::shared()->fromCountry;
    string _gameUid = CCUserDefault::sharedUserDefault()->getStringForKey(GAME_UID,"");
    string _lang = LocalController::shared()->getLanguageFileName();
    string _serverId = CCUserDefault::sharedUserDefault()->getStringForKey(SERVER_ID, "");
    string _serverIp = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_IP, "");

//    url = CCString::createWithFormat("http://%s/probe.php?uuid=%s&loginFlag=%d&country=%s&gameuid=%s&lang=%s&serverId=%s&serverIp=%s", GET_SERVERLIST_IP, _uuid.c_str(),1,_Country.c_str(),_gameUid.c_str(),_lang.c_str(),_serverId.c_str(),_serverIp.c_str());
    url = CCString::createWithFormat("http://%s:9001/ZLTXProxy/probe",GET_SERVERLIST_IP);
    CCLOG("get_server_status list URL: %s",url->getCString());
    
    
    request->setUrl(url->getCString());
    request->setRequestType(CCHttpRequest::Type::POST);
    request->setResponseCallback(this, httpresponse_selector(LoadingScene::onGetServerStatus));
    request->setTag("get_server_status");
    CCHttpClient::getInstance()->setTimeoutForConnect(15);
    CCHttpClient::getInstance()->send(request);
    request->release();
    
}

void LoadingScene::onGetServerStatus(CCHttpClient* client, CCHttpResponse* response)
{
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "onGetServerStatus");
    if (!response)
    {
        return;
    }

    m_responseCode = response->getResponseCode();
    CCLOG("response code: %d", m_responseCode);
    if (!response->isSucceed())
    {
        CCLOG("response failed!\nerror buffer: %s", response->getErrorBuffer());
        string _tmp = response->getErrorBuffer();
        GameController::getInstance()->setLoadingLog("LoadingScene", "onGetServerStatus2-" + _tmp);
        return;
    }
    std::vector<char>* iter = response->getResponseData();
    std::string serverInfo(iter->begin(),iter->end());
    Json *c = Json_create(serverInfo.c_str());
    if(!c){
        return;
    }
    int _code =Json_getInt(c,"cokerrcode",0);//新版用这个cokerrcode为0 和1  代替code 防止http 200也进来了
    if (_code != 0) {
        
        std::string _message= Json_getString(c, "message", "");
        if (_message == "")
        {
            _message = _lang("E100069");
          
        }
        int time = Json_getInt(c, "timelist", 0);
        auto popupview = PopupViewController::getInstance()->getCurrentPopupView();
        auto yesnoView = dynamic_cast<ServerUpdateView*>(popupview);
        if(!yesnoView)
        {
            CCSafeNotificationCenter::getInstance()->postNotification( MSG_SERVER_UPDATE );
        }
    }
    Json_dispose(c);
}


bool LoadingScene::isJP(){
    std::string country = GlobalData::shared()->fromCountry;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    country = AppLibHelper::getLocalCountry();
#endif
    if(country == "JA" || country == "ja"){
        return true;
    }
    return false;
}

bool LoadingScene::isChina(){
    // devil all is china
    return true;
//    bool isChinaPlatForm = (GlobalData::shared()->isChinaPlatForm() || GlobalData::shared()->analyticID == "common");
//    bool isChinaLan = (CCCommonUtils::getLanguage() == "zh_CN");
//    return isChinaPlatForm || isChinaLan;
}


void LoadingScene::loadingRest(float t)
{
    unschedule(schedule_selector(LoadingScene::loadingRest));
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    unzipLocalfileAsyn(0);
#endif
    loadingRestAsyn(nullptr);
}

void LoadingScene::unzipLocalfileAsyn(int type)
{
    DynamicResourceController::getInstance()->uncompressLocalZip(type);
}

void LoadingScene::loadingRestAsyn(Ref* arg)
{
    if( NULL == loadWorldMap )
    {
        CCLOG("loadingRestAsyn begin");
        if( TMXTiledMap::getCacheMapInfo("WorldMap.tmx") == NULL ||
           TMXTiledMap::getCacheMapInfo("WorldMap1.tmx") == NULL )
        {
            TMXMapInfo* mapInfo = new TMXMapInfo();
            if(mapInfo->initWithTMXFile("WorldMap.tmx"))
            {
                TMXTiledMap::setCacheMapInfo("WorldMap.tmx", mapInfo);
            }
            else {
                mapInfo->release();
            }
            TMXMapInfo* mapInfo1 = new TMXMapInfo();
            if(mapInfo1->initWithTMXFile("WorldMap1.tmx")) {
                TMXTiledMap::setCacheMapInfo("WorldMap1.tmx", mapInfo1);
            }
            else {
                mapInfo1->release();
            }
            CCLOG("loadingRestAsyn end");
        }
        
    }
    int loadCount = CCUserDefault::sharedUserDefault()->getIntegerForKey("day_load_count");
    loadCount++;
    CCUserDefault::sharedUserDefault()->setIntegerForKey("day_load_count", loadCount);
}

void LoadingScene::showLoading(CCObject* p)
{
    scheduleOnce(schedule_selector(LoadingScene::show45SecondAlert), 45);
    schedule(schedule_selector(LoadingScene::loadingAni));
    schedule(schedule_selector(LoadingScene::loadingRest), 0.2f);
    schedule(schedule_selector(LoadingScene::updateLoadingTip), 7.0f);
}

void LoadingScene::stopLoading(CCObject* p)
{
    // 待解决问题
    m_selectServerNode->setVisible(true);
    _setProgressVisiable(false);
    m_steps = LOADING_STEP;
    unschedule(schedule_selector(LoadingScene::loadingAni));
    unschedule(schedule_selector(LoadingScene::show45SecondAlert));
    unschedule(schedule_selector(LoadingScene::loadingRest));
    unschedule(schedule_selector(LoadingScene::updateLoadingTip));
    //test
    NetController::shared()->disconnect();
    LogoutCommand* cmd = new LogoutCommand();
    CCDictionary* _params = CCDictionary::create();
    _params->setObject(CCString::create("cok"), "zoneName");
    
    CCDictionary* dict1 = CCDictionary::create();
    dict1->setObject(CCString::create("logout"), "cmd");
    dict1->setObject(_params, "params");
    cmd->handleRecieve(dict1);

}

void LoadingScene::loadingAni(float t)
{
    
    if (m_steps == LOADING_STEP) {
        _setProgressVisiable(true);
        SoundController::sharedSound()->playBGMusic(Music_Sfx_logo_loading);
    }
    if (m_steps <= LOADING_STEP/2)
    {
        unschedule(schedule_selector(LoadingScene::loadingAni));
    } else {
        float pro = (LOADING_STEP-m_steps+1)*1.0/(LOADING_STEP*1.0);
        
        float curX = m_barSize.width*pro;
        m_barClipNode->setContentSize(CCSizeMake(curX, m_barSize.height));
        if (CCCommonUtils::isIosAndroidPad())
        {
            curX *= 2.f;
        }
        
        if (m_steps == 65)
        {
            unschedule(schedule_selector(LoadingScene::loadingAni));
            schedule(schedule_selector(LoadingScene::loadingAni), 0.167f);
        }
        --m_steps;
    }
}

void LoadingScene::stepLoading(CCObject* p)
{
    //loadingLog统计
    time_t worldTime = GlobalData::shared()->getWorldTime();
    GameController::getInstance()->setLoadingLog("LoadingScene", CCString::createWithFormat("stepLoading:serverTime:%s ",CCCommonUtils::timeStampToDate(worldTime).c_str())->getCString());
    unschedule(schedule_selector(LoadingScene::doLoginInit));
    //提示更新版本
    std::string str;
    GlobalData::shared()->lang = cocos2d::extension::CCDevice::getLanguage();
    GlobalData::shared()->platform = "ios";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    GlobalData::shared()->platform = "android";
    //根据平台获得提示语
    if(GlobalData::shared()->analyticID == "market_global"){
        str = "GOOGLE";
    }else{
        /**
         tstore
         nstore
         amazon
         cafebazaar
         **/
        str = GlobalData::shared()->analyticID;
    }
#else
#ifdef Channel_91
    str = "91Store";
    GlobalData::shared()->analyticID = "91Store";
#else
    str = "AppStore";
    GlobalData::shared()->analyticID = "AppStore";
#endif
#endif
    if (GlobalData::shared()->updateType==1)
    {
        auto _dialog = YesNoDialog::show(CCString::createWithFormat(_lang("login_error_client_ver_must").c_str(), str.c_str())->getCString(),
                                         CCCallFuncO::create(this, callfuncO_selector(LoadingScene::forceUpdate), NULL),0,
                                         true,
                                         CCCallFuncO::create(this, callfuncO_selector(LoadingScene::offerUpdate), NULL));
        _dialog->showCancelButton();
        return;
    }
    if (GlobalData::shared()->updateType==2)
    {
        YesNoDialog::showVariableTitle(CCString::createWithFormat(_lang("login_error_client_ver_must").c_str(), str.c_str())->getCString(),CCCallFuncO::create(this, callfuncO_selector(LoadingScene::forceUpdate),NULL), _lang("confirm").c_str(),true);
        return;
    }

    gotoMainScene(0.1);
}


void LoadingScene::gotoMainScene(float t)
{
    unschedule(schedule_selector(LoadingScene::loadingAni));
    if (m_steps > 1) {
        float pro = (LOADING_STEP-m_steps+1)*1.0/(LOADING_STEP*1.0);
        
        float curX = m_barSize.width*pro;
        
        m_barClipNode->setContentSize(CCSizeMake(curX, m_barSize.height+30));
        if (CCCommonUtils::isIosAndroidPad())
        {
            curX *= 2.f;
        }
        
        --m_steps;
        if ( m_steps <= 1 ) m_steps = 1;
        unschedule(schedule_selector(LoadingScene::gotoMainScene));
        schedule(schedule_selector(LoadingScene::gotoMainScene), 0.005f);
    }
    else if(!GlobalData::shared()->isInitFlag){
        unschedule(schedule_selector(LoadingScene::gotoMainScene));
        schedule(schedule_selector(LoadingScene::gotoMainScene), 0.01f);
    }
    else {
        //loadingLog统计
        GameController::getInstance()->setLoadingLog("LoadingScene", "gotoMainScene");
        unschedule(schedule_selector(LoadingScene::doLoginInit));
        unschedule(schedule_selector(LoadingScene::gotoMainScene));
        m_barClipNode->setContentSize(m_barSize);
        
        unschedule(schedule_selector(LoadingScene::onLoginTimeout));
        
        std::string _shortItemType = GlobalData::shared()->shortItemType;
        bool _is3Dtouch = false;
        if(GlobalData::shared()->shortItem_switch && !(GuideController::share()->isInTutorial() || (GlobalData::shared()->playerInfo.level == 1 && GlobalData::shared()->playerInfo.exp == 0)))
        {
             if (!_shortItemType.compare(shortItemType_chat))
            {
                GlobalData::shared()->isUiInti = false;

                UIComponent::getInstance()->showAllianceChat();
                CCDirector::sharedDirector()->setVisitFlag(true);
                schedule(schedule_selector(LoadingScene::onEnterMainScene), 0.1);
                return;
            }
            
            if (!_shortItemType.compare(shortItemType_mail))
            {
                GlobalData::shared()->isUiInti = false;
                CCDirector::sharedDirector()->setVisitFlag(false);
                UIComponent::getInstance()->showMailUI();
                CCDirector::sharedDirector()->setVisitFlag(true);
                schedule(schedule_selector(LoadingScene::onEnterMainScene), 0.1);
                return;
            }
        }
        
        // lzy
        CCCommonUtils::postEventLogToServer("loading_loadingFinish_1");
        
        SceneController::getInstance()->addGUI();

        if(GlobalData::shared()->shortItem_switch && !(GuideController::share()->isInTutorial() || (GlobalData::shared()->playerInfo.level == 1 && GlobalData::shared()->playerInfo.exp == 0)))
        {
            if (!_shortItemType.compare(shortItemType_world))
            {
                _is3Dtouch = true;
                SceneController::getInstance()->gotoScene(SCENE_ID_WORLD);
            }
        }

        if(SceneController::getInstance()->showBG){
            SceneController::getInstance()->showBG->release();
            SceneController::getInstance()->showBG = NULL;
            SceneController::getInstance()->gotoScene(SCENE_ID_WORLD);
        }else if(!_is3Dtouch)
        {
            showMainScene();
        }
        CCCommonUtils::recordStep("5");
    }
    
}

void LoadingScene::showMainScene()
{
    bool showServiceTerms = false;
    // devil 暂时关闭服务协议确认
//    showServiceTerms = CCUserDefault::sharedUserDefault()->getStringForKey("service_terms_check","") == "";
    if (showServiceTerms) {
        if (GlobalData::shared()->permitiCountry != "") {
            vector<std::string> mTmpV;
            bool showService =true;
            CCCommonUtils::splitString(GlobalData::shared()->permitiCountry, "|", mTmpV);
            for (int i = 0; i < mTmpV.size(); i++) {
                if (GlobalData::shared()->playerInfo.regCountry == mTmpV[i]) {
                    showService = false;
                    this->unschedule(schedule_selector(LoadingScene::show45SecondAlert));
                    UIComponent::getInstance()->setVisible(false);
                    PopupViewController::getInstance()->addPopupView(PermitionForServicePopupView::create());
                    break;
                }
            }
            if (showService) {
                SceneController::getInstance()->gotoScene(SCENE_ID_MAIN);
            }
        }
        else
            SceneController::getInstance()->gotoScene(SCENE_ID_MAIN);
    }
    else
        SceneController::getInstance()->gotoScene(SCENE_ID_MAIN);
    
    
    // lzy
    CCCommonUtils::finishLoadingTotalTime();
    CCCommonUtils::postEventLogToServer("loading_enterMainScene_1");
    
}

void LoadingScene::onEnterMainScene(float t)
{
    unschedule(schedule_selector(LoadingScene::onEnterMainScene));
    
    SceneController::getInstance()->addGUI();
    
    if(SceneController::getInstance()->showBG){
        SceneController::getInstance()->showBG->release();
        SceneController::getInstance()->showBG = NULL;
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD);
    }else{
        showMainScene();
    }
    
    CCDirector::sharedDirector()->setVisitFlag(true);
    
    CCCommonUtils::recordStep("5");
}

void LoadingScene::forceUpdate(CCObject* p) {
    //服务器下发地址为空
    bool isUrlNull =  GlobalData::shared()->downloadUrl.empty() || GlobalData::shared()->downloadUrl=="null";
    
    if (isUrlNull)
        GameController::getInstance()->quitGame(NULL);
    else
        GameController::getInstance()->updateVersion();
}

void LoadingScene::onConnectionLost(cocos2d::CCObject *obj)
{
    CCLOG("ConnectionLost");
    auto ret = dynamic_cast<NetResult*>(obj);
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "onLoginFail");
    NetController::shared()->setIslogining(false);
    if (Error_Network_Lost==ret->getErrorCode() && !GlobalData::shared()->isPause) {
        NetController::shared()->checkNetWork("login 1");
    };
}

void LoadingScene::doResourceUpdate(CCObject* p) {
    unschedule(schedule_selector(LoadingScene::onLoginTimeout));
    //loadingLog统计
    GameController::getInstance()->setLoadingLog("LoadingScene", "doResourceUpdate");
}

void LoadingScene::onNetworkAvailable(CCObject* p)
{
    CCLog("jinpeng post onNetworkAvailable");
    CCCommonUtils::onNetworkAvailable(p);
}

void LoadingScene::offerUpdate(CCObject* p)
{
    GlobalData::shared()->updateType=0;
    stepLoading(NULL);
}

void LoadingScene::checkCanGotoMainScene(CCObject* p)
{
    UIComponent::getInstance()->setVisible(true);
    SceneController::getInstance()->gotoScene(SCENE_ID_MAIN);
}


void LoadingScene::selectServerList(CCObject* p)
{
    if (!m_selectServerNode->isVisible())
        return ;
    if(p && dynamic_cast<CCString*>(p)){  //
        m_curServerId = dynamic_cast<CCString*>(p)->getCString();
        
        CCLog("selectServerList ServerId %s",m_curServerId.c_str());
        refreshSelectViewByScrol();
        _setServerListViewVisiable(false);
    }
}
void LoadingScene::gameLogout(CCObject* p)
{
    //注销成功
    CCCommonUtils::flyHint("", "", "注销成功");
    GameController::m_isLoginSDK = 0;
    m_sdkLabel->setString(_lang("new100042").c_str());  //注销
    //当正在进行loading中的时候!!
    if (!m_selectServerNode->isVisible())
    {
        stopLoading(nullptr);
    }
}

//////////class  ServerInfoCell

ServerInfoCell* ServerInfoCell::create(int type,string serverID,string playName,string serverName)
{
    auto ret = new ServerInfoCell( type, serverID, playName, serverName);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
void ServerInfoCell::onEnter() {
    CCNode::onEnter();
   // setTouchEnabled(true);
    
}

void ServerInfoCell::onExit() {
    //setTouchEnabled(false);
    CCNode::onExit();
}

bool ServerInfoCell::init()
{
    CCBLoadFile("ServerInfoCell",this,this);
    if (m_type == 0)
    {
        my_ServerNode->setVisible(true);
        all_ServerNode->setVisible(false);
    }else{
     //all服务器
        my_ServerNode->setVisible(false);
        all_ServerNode->setVisible(true);
    }
    m_serverName->setString(m_serverNameStr.c_str());
    m_playName->setString(m_playNameStr.c_str());
    return true;
}

SEL_CCControlHandler ServerInfoCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSelectClick", ServerInfoCell::onSelectClick);
    return NULL;
}

void ServerInfoCell::onSelectClick(CCObject *pSender, CCControlEvent event)
{
    auto serverID = CCString::create(m_serverID);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SELECT_SERVER,serverID);
    CCLog("send msg MSG_SELECT_SERVER");
}
bool ServerInfoCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
   if (m_type == 0)
    {
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_serverName", CCLabelIF*, this->m_serverName);
        
    }else{
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_serverName_2", CCLabelIF*, this->m_serverName);
        
    }
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_playName", CCLabelIF*, this->m_playName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "my_ServerNode", CCNode*, this->my_ServerNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "all_ServerNode", CCNode*, this->all_ServerNode);
    
    return false;
}
