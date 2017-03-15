//
//  GlobalData.cpp
//  IF
//
//  Created by 吴 凌江 on 13-8-28.
//
//

#include "GlobalData.h"
#include "CCCommonUtils.h"
#include "../Ext/CCDevice.h"

static GlobalData* _sharedInstance = NULL;

GlobalData* GlobalData::shared()
{
    if (_sharedInstance==NULL){
        _sharedInstance=new GlobalData();
        _sharedInstance->initRand();
        _sharedInstance->deviceCollect = "";
        _sharedInstance->battleSpeedFactor = 1.0;
        _sharedInstance->m_harvestInfo = "";
        _sharedInstance->facebook_gift_k7 = 0;
        _sharedInstance->m_isFirstOpenDailyView = true;
        _sharedInstance->m_isFirstAskFor = false;
        _sharedInstance->isPressedHomeKey = false;
        _sharedInstance->isUploadPic = false;
        _sharedInstance->hd_switch = 0;
        _sharedInstance->alliance_territory_index = -1;//0哨塔 1超级矿 2箭塔 3仓库
        _sharedInstance->serverMax = 0;
        _sharedInstance->replayTime = 100000;
        _sharedInstance->cnReplayFlag = 0;
        _sharedInstance->sceneResourceStepLoad = 0;
        _sharedInstance->superManFlag = 0;
        _sharedInstance->recordFlag = 0;
        _sharedInstance->isOpenTranslateOptimization = false;
        _sharedInstance->checkSimulatorFlag=0;
        _sharedInstance->connectionFlag = 0;
        _sharedInstance->robLittleResLevel =0;
        _sharedInstance->robLittleResCount =1.0f;
        _sharedInstance->bImpMusicOpen = 0;//主城音乐开关
        _sharedInstance->bWorldMusicOpen = 0;//世界音乐开关
        _sharedInstance->bImpMusicLocalOpen = 0;//本地主城音乐开关
        _sharedInstance->bWorldMusicLocalOpen = 0;//本地的世界音乐 开关
        _sharedInstance->xmlReloadFlag = "0";
        _sharedInstance->xmlReloadVersion = "";
        _sharedInstance->changeCheckServer = "";
        _sharedInstance->userChangeCheckVersion = "";
        _sharedInstance->userNeedChangeCheck = true;
        _sharedInstance->bLotteryShareOpen = true;
        _sharedInstance->lotteryShareType = 2;

        _sharedInstance->isfirstInOneLogin = 1;//1表示是本次登陆中首次打开加入联盟提示面板
        _sharedInstance->useNewKey =true;
        
        _sharedInstance->newIcon_version = "1.1.15";
        _sharedInstance->newIcon_name = "";
        _sharedInstance->newIcon_cost = 0;
        _sharedInstance->newIcon_startTime = 0;
        _sharedInstance->newIcon_endTime = 0;
        _sharedInstance->newIcon_country = "";

        _sharedInstance->shortItem_switch = false;
        _sharedInstance->peek_switch = false;
        _sharedInstance->clearCache_switch = false;

        _sharedInstance->isServerInfoBack = false;
        
        _sharedInstance->freshRechargeTotal = 0;
        _sharedInstance->bFreshRechargeOpen = false;
        _sharedInstance->func_qemuBkpt_open = false;
        _sharedInstance->teleport_limit_open = false;
        _sharedInstance->teleport_limit_time = 0;
        _sharedInstance->func_checkEmulator_open = false;
        
        _sharedInstance->startDarkTime = 1;
        _sharedInstance->completeDarkTime = 2;
        _sharedInstance->startLightTime = 3;
        _sharedInstance->completeLightTime = 4;
        
        _sharedInstance->bindGuideInterval = -1;
        _sharedInstance->isDirectBind = false;
        
        _sharedInstance->fun_bindRes_switch = false;
        
        _sharedInstance->isCrossServerSendGiftValid = true;
        
        _sharedInstance->on_event4_k1 = "";
        _sharedInstance->on_event4_k2 = "";
        _sharedInstance->on_event4_k3 = "";
        _sharedInstance->on_event4_k4 = "";
        _sharedInstance->on_event4_k5 = "";
        
        _sharedInstance->protectDisappearPushMin = 0;
        _sharedInstance->worldMarchParticleType = 0;
    }
    return _sharedInstance;
}

void GlobalData::purgeData() {
    CC_SAFE_RELEASE_NULL( _sharedInstance );
     _sharedInstance = NULL;
}



time_t GlobalData::getWorldTime()
{
    return worldTime + (time(NULL) - localTime) + timeZone*60*60;
}

time_t GlobalData::getTimeStamp() {
    return worldTime + (time(NULL) - localTime);
}

time_t GlobalData::changeTime(time_t t)
{
    if(t<=0) return 0;
    return t + timeZone*60*60;
}

double GlobalData::renewTime(double t)
{
    return t - timeZone*60*60*1000;
}

void GlobalData::setWorldTime(time_t t, int tz)
{
    worldTime=t;
    localTime=time(NULL);
    timeZone = tz;
}

int GlobalData::getTimeZone(){
    return timeZone;
}

void GlobalData::initRand(){
    srand((int)time(NULL)+11);//随机数的种子
    speedFactor = 1.0;
}

int GlobalData::getRand(int min,int max){
    return min+rand()%(max-min);
}

//判断是否走google play的支付
bool GlobalData::isGoogle(){
    if(GlobalData::shared()->analyticID == "market_global")
        return true;
    return false;
}

bool GlobalData::needLogin(){
    // devil hoolai sdk always need login
    return true;
//    return isXiaoMiPlatForm();
}

bool GlobalData::needLogout(){
    if (analyticID == "cn_uc"
        || analyticID == "cn_360"
        ) {
        return true;
    }
    if (analyticID == "cn_mi"
        || analyticID == "cn_mihy"
        || analyticID == "cn_baidu"
        ) {
        return false;
    }
    if(analyticID.size() > 3){
        std::string sub = analyticID.substr(0, 3);
        if(sub == "cn_"){
            return cocos2d::extension::CCDevice::needPlatformQuit();
        }
    }
    return false;
}

bool GlobalData::isChinaPlatForm(){
    if(analyticID == "cn1" || analyticID == "tencent" || analyticID == "hoolai"){
        return true;
    }else{
        if(analyticID.size() > 3){
            std::string sub = analyticID.substr(0, 3);
            if(sub == "cn_"){
                return true;
            }
        }
    }
    return false;
}

bool GlobalData::isXiaoMiPlatForm(){
    bool flag = false;
    if(analyticID == "tencent"){
        flag = true;
    }else{
        if(analyticID.size() > 3){
            std::string sub = analyticID.substr(0, 3);
            if(sub == "cn_"){
                flag = true;
            }
        }
    }
    return flag;
}

bool GlobalData::cantSwtichAccount(){
    if (isXiaoMiPlatForm()
        ||analyticID == "common"
        ) {
        return true;
    }
    return false;    
}

bool GlobalData::isWeiboEnabled(){
    if(analyticID == "cn1" || (analyticID == "AppStore" && CCCommonUtils::getLanguage() == "zh_CN"))
        return true;
    return false;
}

bool GlobalData::isVKEnabled(){
    if(analyticID == "market_global" && CCCommonUtils::getLanguage() == "ru")
        return true;
    if(analyticID == "AppStore" && CCCommonUtils::getLanguage() == "ru")
        return true;
    return false;
}

bool GlobalData::isScienceValid(int iScienceId)
{
    if (scienceInfo.infoMap.find(iScienceId) == scienceInfo.infoMap.end()) {
        return false;
    }
    
    return true;
}


