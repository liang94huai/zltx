//
//  IFAppDelegate.cpp
//  IF
//
//  Created by 吴 凌江 on 13-8-28.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//

#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "GlobalData.h"
#include "LoadingScene.h"
#include "SceneContainer.h"
#include "SceneController.h"
#include "CCSafeNotificationCenter.h"
//#include "CCLuaEngine.h"
//#include "Lua_extensions_CCB.h" //TODO lua
#include "Ext/CCDevice.h"
#include "ChatServiceCocos2dx.h"
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
//    #include "NetControllerSFSC.h"
//#endif
#include "WorldController.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "AppLibHelper.h"
#endif
#include <curl/curl.h>
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <cpu-features.h>
#include <unistd.h>
#include "android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"

extern bool g_IS_NEON_SUPPORT;
extern bool g_IS_SOFTFP_SUPPORT;
extern float g_HIGH_FPS;
extern float g_LOW_FPS;

static void checkBlueStack();

static bool isNeonSupport() {
    if (android_getCpuFamily() == ANDROID_CPU_FAMILY_ARM && (android_getCpuFeatures() & ANDROID_CPU_ARM_FEATURE_NEON) != 0)
        return  true;
    return false;
}

static bool isSoftFPSupport() {
    if (android_getCpuFamily() == ANDROID_CPU_FAMILY_ARM && (android_getCpuFeatures() & ANDROID_CPU_ARM_FEATURE_ARMv7) != 0)
        return  true;
    return false;
}

#endif

// SDK
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "HoolaiFastAccess/GameProxy.h"
#include "HoolaiFastAccess/CCAccessSDK.h"
#else
#import <AccessSDK/CCAccessSDK.h>
#endif
#include "HoolaiSdkDelegate.hpp"

extern float g_HIGH_FPS;

#define IS_HD 1  // 1 是HD版本 0 不是

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    g_IS_NEON_SUPPORT = isNeonSupport();
    g_IS_SOFTFP_SUPPORT = isSoftFPSupport();
#endif
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // lzy  内部服务器打点
    CCCommonUtils::postEventLogToServer("AppDelegate_clickIcon_1");
    CCCommonUtils::setLoadingStartTime();
    
#if (COCOS2D_DEBUG == 0 || COCOS2D_DEBUG == 2)
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // crash dumper init
    //CCCrashHandler::sharedHandler()->init();  // -- added by Zouc
    #else
    #endif
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    checkBlueStack();
#endif
    // initialize director
    Director* pDirector = Director::getInstance();
//    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());//400ms
    auto glview = pDirector->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Clash of king");
        pDirector->setOpenGLView(glview);
    }
    
    pDirector->setOpenGLView(glview);
    // set PVR enable
    CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
    // set resolution size (width free adjust , width fixed 768)
//    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(640, 852, kResolutionFixedWidth);
    

    if (CCCommonUtils::isIOSPad() || CCCommonUtils::isAdriodPadHD())
    {
        CCCommonUtils::setIsUseHD(IS_HD); // 客户端开关
        CCCommonUtils::setServerHDSwitch();// 设置上次登录服务器给的开关
    }
    
    if(CCCommonUtils::isIosAndroidPad()) {
        CCCommonUtils::setIsHDViewPort(true);
//        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(1536, 2048, kResolutionFixedWidth);

    }else{
        CCCommonUtils::setIsHDViewPort(false);
//        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(640, 852, kResolutionFixedWidth);
    }
    CCCommonUtils::setCurUISkinType();
#if COCOS2D_DEBUG > 0
    // 打开这个测多分辨率
    //    radio=1.78; // iphone5
    //    radio=1.67; // WVGA
    //    radio=1.33; // Ipad
    //    radio=1.5; //Iphone4/4s
    CCSize winSize = pDirector->getWinSize();
    CCSize screenSize = glview->getFrameSize();
    float radio = winSize.width/winSize.height;
    float scale = 852/screenSize.width;
    CCLOG("screen width: %d, height: %d \n resolution width: %d, height: %d \n radio:%.2f, scale:%.2f",
          (int)screenSize.width,(int)screenSize.height,
          (int)winSize.width,(int)winSize.height,
          radio,scale);
    
    // turn on display FPS
    pDirector->setDisplayStats(true);
#endif

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval( g_HIGH_FPS);
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    if(android_getCpuCount() > 4) {
//        pDirector->setAnimationInterval(1.0 / 15);
//    }
//    else
//    {
//        pDirector->setAnimationInterval(1.0 / 30);
//    }
//#endif

////    //xml init

//////    pDirector->setDisplayStats(true);
    
    //SDK init
    //初始化sdk
    if (GameController::m_isInitSDK == 0) {
        CCAccessSDK::sharedInstance()->initSDK(new HoolaiSdkDelegate());
        GameController::m_isInitSDK = 1;
    }
    
        //xml init
    LocalController::shared();//600ms
 
    // load common resource
//    unsigned long ulc2 = clock();
    CCLoadSprite::loadCommonResource();
    CCLoadSprite::doResourceByGeneralIndexAsync(100);
    //    CCLoadSprite::doResourceByGeneralIndex(100, true);


    // register lua engine
    //CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
    //CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);
    // register lua ccb
    //tolua_extensions_ccb_open(pEngine->getLuaStack()->getLuaState());
    curl_global_init(CURL_GLOBAL_ALL);
    // run
    pDirector->runWithScene(SceneContainer::create());
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
//    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(NetControllerSFSC::updateEvent), NetControllerSFSC::getInstance(), 0, false);
//#endif
    SceneController::getInstance()->gotoLoading();//2300ms
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    AppLibHelper::initLibs();
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG("Reset Screen");
    resetContentView();
#endif
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCLOG("applicationDidEnterBackground");
    #if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
       ChatServiceCocos2dx::CSApplicationWillDidGotoBackGround();
    #endif
    cocos2d::CCDirector::sharedDirector()->pause();
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
    GlobalData::shared()->isPause = true;
    GlobalData::shared()->pauseTime = GlobalData::shared()->getWorldTime();
    if (SceneController::getInstance()->currentSceneId != SCENE_ID_LOADING) {
        cocos2d::extension::CCDevice::pushNotice(987, 60);
    }
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCLOG("applicationWillEnterForeground");
    cocos2d::extension::CCDevice::cancelNotice(987);
    cocos2d::CCDirector::sharedDirector()->resume();
    CCDirector::sharedDirector()->startAnimation();
    if (CCUserDefault::sharedUserDefault()->getBoolForKey(BG_MUSIC_ON, true))
    {
        SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    }
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
    GlobalData::shared()->isPause = false;
    GlobalData::shared()->isPressedHomeKey = true;
  //  CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_APP_FOREGROUND);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (GlobalData::shared()->getWorldTime() - GlobalData::shared()->pauseTime < 60) {
        ChatServiceCocos2dx::CSApplicationDidBecomeActiveLessThanSixtySec();
    }else{
        ChatServiceCocos2dx::resetResume();
    }
#endif
    
    WorldController::getInstance()->clearMarchInfo();
    //zym 2015.9.22 resume时游戏内容相关的放到WorldController::doResumeForGLSource，切忌和OpenGl的资源相关的操作放到这里
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(WorldController::doResumeForGLSource), WorldController::getInstance());
  
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(WorldController::doResumeForGLSource), WorldController::getInstance(), 0.0f,  0, 0.0f, false );
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLog("Reset Screen AppEnter");
    resetContentView();
#endif
  //  WorldController::getInstance()->doWhenBackToGame(NULL);
}
                                                                   

void AppDelegate::applicationDidBecomeActive()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(CCUserDefault::sharedUserDefault()->getStringForKey(GAME_UID, "") != ""){
        AppLibHelper::triggerEventExistUser();
    }
    AppLibHelper::triggerEventActivate();
#endif
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
static void checkBlueStack() {

#define STR(x) string(x)
    string knownBluestacksFiles[5] = {
        STR("b") + STR("s") + STR("t") + STR("c") + STR("m") + STR("d") + STR("_") + STR("s") + STR("h") + STR("i") + STR("m"),
        STR("b") + STR("s") + STR("t") + STR("f") + STR("o") + STR("l") + STR("d") + STR("e") + STR("r") + STR("_") + STR("c") + STR("t") + STR("l"),
        STR("b") + STR("s") + STR("t") + STR("f") + STR("o") + STR("l") + STR("d") + STR("e") + STR("r") + STR("d"),
        STR("b") + STR("s") + STR("t") + STR("s") + STR("h") + STR("u") + STR("t") + STR("d") + STR("o") + STR("w") + STR("n"),
        STR("b") + STR("s") + STR("t") + STR("s") + STR("h") + STR("u") + STR("t") + STR("d") + STR("o") + STR("w") + STR("n") + STR("_") + STR("c") + STR("o") + STR("r") + STR("e")
    };
    bool isBlueStack = false;
    for( int i = 0; i < 5; i++ ) {
        string path = STR("/system/bin/") + knownBluestacksFiles[i].c_str();
        if( access( path.c_str(), F_OK ) != -1 ) {
            isBlueStack = true;
            break;
        } else {
            // file doesn't exist
        }
    }
    
    if( isBlueStack ) {
        string val = STR("a") + STR("9") + STR("5") + STR("0") + STR("3") + STR("7") + STR("a") + STR("9") + STR("f") + STR("4") + STR("c") + STR("e") + STR("5") + STR("H") + STR("D");
        CCUserDefault::sharedUserDefault()->setStringForKey("phoneDevice", val);
        return;
    }
    bool isQemu = false;
    string QemuFiles[3] = {
        STR("/") + STR("s") + STR("y") + STR("s") + STR("t") + STR("e") + STR("m") + STR("/") + STR("l") + STR("i") + STR("b")+STR("/")+STR("l")+STR("i")+STR("b")+STR("c")+STR("_")+STR("m")+STR("a")+STR("l")+STR("l")+STR("o")+STR("c")+STR("_")+STR("d")+STR("e")+STR("b")+STR("u")+STR("g")+STR("_")+STR("q")+STR("e")+STR("m")+STR("u")+STR(".")+STR("s")+STR("o"),
        STR("/") + STR("s") + STR("y") + STR("s") + STR("/") + STR("q") + STR("e") + STR("m") + STR("u") + STR("_") + STR("t") + STR("r") + STR("a")+STR("c")+STR("e"),
        STR("/") + STR("s") + STR("y") + STR("s") + STR("t") + STR("e") + STR("m") + STR("/") + STR("b") + STR("i")+STR("n")+STR("/")+STR("q")+STR("e")+STR("m")+STR("u")+STR("-")+STR("p")+STR("r")+STR("o")+STR("p")+STR("s")
    };
    for( int i = 0; i < 3; i++ ) {
        string path = QemuFiles[i].c_str();
        if( access( path.c_str(), F_OK ) != -1 ) {
            isQemu = true;
            break;
        } else {
            // file doesn't exist
        }
    }
	if( isQemu ) {
        string val = STR("a") + STR("9") + STR("5") + STR("0") + STR("3") + STR("7") + STR("a") + STR("9") + STR("f") + STR("4") + STR("c") + STR("e") + STR("2") + STR("D") + STR("F");
        CCUserDefault::sharedUserDefault()->setStringForKey("phoneDevice", val);
        return;
    }
}
#endif
