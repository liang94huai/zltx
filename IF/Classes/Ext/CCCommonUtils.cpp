//
//  CCCommonUtils.cpp
//  IF
//
//  Created by 邹 程 on 13-9-16.
//
//

#include "CCCommonUtils.h"
#include "SceneController.h"
#include "GuideRecordCommand.h"
#include "GeneralManager.h"
#include "YesNoDialog.h"
#include "PopupViewController.h"
#include "FlyHint.h"
#include "GuideController.h"
#include "GeneralManager.h"
#include "FunBuildController.h"
#include "ScienceController.h"
#include "FlySystemUpdateHint.h"
#include "InitCommand.h"
#include "SceneController.h"
#include "MailController.h"
#include "NetController.h"
#include "PopupViewController.h"
#include "FunBuildController.h"
#include "FunBuildInfo.h"
#include "QuestController.h"
#include "GeneralManager.h"
#include "PayController.h"
#include "AppLibHelper.h"
#include "GCMRewardController.h"
#include "ChatController.h"
#include "WorldController.h"
#include "ChatShieldInfo.h"
#include "QueueController.h"
#include "WorldActivityCommand.h"
#include "EnemyInfoController.h"
#include "ToolController.h"
#include "TroopsController.h"
#include "CCDevice.h"
#include "CityCrashView.h"
#include "ActivityController.h"
#include "ScienceController.h"
#include "ActivityController.h"
#include "SocialController.h"
#include "Utf8Utils.h"
#include "RewardController.h"
#include "FreeListView.h"
#include "TimerController.h"
#include "EquipmentController.h"
#include "KnightTitleController.hpp"
#include "ShowStatusItemCommand.h"
#include "LotteryController.h"
#include "FilterSprite.h"
#include "ChatServiceCocos2dx.h"
#include "AchievementController.h"
#include "AchieveGetCommand.h"
#include "../view/guiExt/md5.h"
#include "UploadImageController.h"
#include "LuaController.h"
#include "GetInviteeCommand.h"
#include "DailyActiveController.h"
#include "VipUtil.h"
#include "UserBindCommand.h"
#include "PhoneDeviceCommand.h"
#include "AllianceManager.h"
#include "FriendsController.h"
#include "DailyActiveController.h"
#include "AllianceDailyController.h"
#include "FriendsController.h"
#include "DeviceInfoController.h"
#include "DynamicResourceController.h"
#include "ParticleFireAni.h"
#include "PrincessQuestController.hpp"
#include "external/unzip/unzip.h"
#include <sys/stat.h>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "anti/anti.h"
#include "android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"

#endif
#include "PlayerInfoController.h"
bool g_performanceCrashSwitch = false;
bool g_swithDynamicFPS = false;
float g_HIGH_FPS = (1.0/40);
float g_LOW_FPS = (1.0/15);
bool g_IS_NEON_SUPPORT = false;
bool g_IS_SOFTFP_SUPPORT = false;

//std::string CCCommonUtils::to_string(int d) {
//    char buffer[50];
//    snprintf(buffer, 49, "%d",d);
//    return std::string(buffer);
//}
//
//std::string CCCommonUtils::to_string(double d) {
//    char buffer[50];
//    snprintf(buffer, 49, "%.3f",d);
//    return std::string(buffer);
//}
//
//std::string CCCommonUtils::to_string(float d) {
//    char buffer[50];
//    snprintf(buffer, 49, "%.3f",d);
//    return std::string(buffer);
//}
std::string CCCommonUtils::timeStampToUTCDate(time_t &t){
    struct tm tm;
    char s[100];
    
    tm = *gmtime(&t);
    strftime(s, sizeof(s), "%Y-%m-%d  %H:%M:%S", &tm);
    return s;
}
std::string CCCommonUtils::timeStampToUTCHSM(time_t &t)
{
    struct tm tm;
    char s[100];
    
    tm = *gmtime(&t);
    strftime(s, sizeof(s), "%H:%M:%S", &tm);
    return s;
}
std::string CCCommonUtils::timeStampToDate(time_t &t){
    struct tm tm;
    char s[100];
    
    tm = *localtime(&t);
    strftime(s, sizeof(s), "%Y-%m-%d  %H:%M:%S", &tm);
    return s;
}
std::string CCCommonUtils::timeStampToHMS(time_t &t){
    struct tm tm;
    char s[100];
    
    tm = *localtime(&t);
    strftime(s, sizeof(s), "%H:%M:%S", &tm);
    return s;
}
std::string CCCommonUtils::timeStampToDHM(time_t &t){
    struct tm tm;
    char s[100];
    
    tm = *localtime(&t);
    strftime(s, sizeof(s), "%Y-%m-%d  %H:%M", &tm);
    return s;
}

std::string CCCommonUtils::timeStampToYMD(time_t &t){
    struct tm tm;
    char s[100];
    
    tm = *localtime(&t);
    strftime(s, sizeof(s), "%Y-%m-%d", &tm);
    return s;
}

std::string CCCommonUtils::timeStampToHour(time_t &t){
    struct tm tm;
    char s[100];
    
    tm = *localtime(&t);
    strftime(s, sizeof(s), "%H:%M", &tm);
    return s;
}

std::string CCCommonUtils::timeStampToMD(time_t &t){
    struct tm tm;
    char s[100];
    
    tm = *localtime(&t);
    strftime(s, sizeof(s), "%m-%d", &tm);
    return s;
}

long CCCommonUtils::UTCDataToTimeStamp(const char* timeStamp)
{
    //timeStamp like this:"2015-03-21-01-00-00"
    struct tm tm;
    strptime(timeStamp,"%Y-%m-%d-%H-%M-%S", &tm);
    time_t ft=mktime(&tm);
    return ft;
}

string CCCommonUtils::subStrByUtf8(const string &str,int index,int len){
    int totalLen = Utf8Utils::strlen(str.c_str());
    string retStr = "";
    if(index<=totalLen-1){
        int realLen = len<(totalLen-index) ? len:(totalLen-index);
        retStr = Utf8Utils::utf8sub(str.c_str(),index,realLen);
    }
    return retStr;
}
CCDictionary* CCCommonUtils::castDict(cocos2d::CCObject *obj) {
    auto dict = dynamic_cast<CCDictionary*>(obj);
    if (dict) {
        return dict;
    } else {
        return CCDictionary::create();
    }
}

CCPoint CCCommonUtils::getPointByString(std::string string,std::string separator) {
    vector<std::string> tmp;
    CCPoint ret;
    CCCommonUtils::splitString(string, separator, tmp);
    ret.x = atof(tmp.front().c_str());
    ret.y = atof(tmp.back().c_str());
    return ret;
}

int CCCommonUtils::getNormalPray(){
    auto& build = GlobalData::shared()->imperialInfo.find(423000)->second;
    double effect = (GlobalData::shared()->scienceEffect["210"] + 100) / 100.0;
    int product = build.para1 * effect;
    return product;
}

int CCCommonUtils::getIronPray(){
    auto& build = GlobalData::shared()->imperialInfo.find(423000)->second;
    double effect = (GlobalData::shared()->scienceEffect["210"] + 100) / 100.0;
    int product = build.para3 * effect;
    return product;
}

bool CCCommonUtils::checkGeneralState(int _state)
{
    map<string,GeneralInfo> *generals = &(GlobalData::shared()->generals);
    if(!generals->empty()){
        map<string, GeneralInfo>::iterator it;
        for(it=generals->begin(); it != generals->end(); ++it)
        {
            if (it->second.state == _state) {
                return true;
            }
        }
    }
    return false;
}

void CCCommonUtils::localRecordLog(const char * format, ...)
{
//    va_list args;
//    va_start(args, format);
//    _log(format, args);
//    va_end(args);
//    
//    cocos2d::log(format, ##__VA_ARGS__)
//    CCLOG("%d",1);
}

int CCCommonUtils::getGoldByTime(int t)
{
    if(t == 0){
        return 0;
    }
    int ret = 0;
//    int tmpTime = t;
//    bool _bTmp = true;
//    int maxIdx = GlobalData::shared()->cd_time.size()-1;
//    while (_bTmp) {
//        if (tmpTime > GlobalData::shared()->cd_time[maxIdx]) {
//            ret += GlobalData::shared()->cd_cost[maxIdx];
//            tmpTime -= GlobalData::shared()->cd_time[maxIdx];
//            _bTmp = true;
//        }
//        else{
//            _bTmp = false;
//            for (int idx = 0; idx<=maxIdx; idx++) {
//                if (tmpTime <= GlobalData::shared()->cd_time[idx]) {
//                    ret += GlobalData::shared()->cd_cost[idx];
//                    break;
//                }
//            }
//        }
//    }
//    return ret;
    
    //秒CD花费金币数=max（time（秒）/(k1*(time（秒）/3600)^(k2/100))，1）
    float k1 = GlobalData::shared()->cd_gold["k1"];
    float k2 = GlobalData::shared()->cd_gold["k2"];
    float tmpv = k1*powf(t*1.0/3600, k2/100);
    ret = t/tmpv;
    ret = MAX(ret, 1);
    return ret;
}

int CCCommonUtils::getResGlodByType(int type, int num)
{
    int ret = 0;
    float a,b,c,d;
    if (num <= GlobalData::shared()->res_cost["k1"]) {
        a = GlobalData::shared()->res1_num["k1"];
        b = GlobalData::shared()->res1_num["k2"];
        c = GlobalData::shared()->res1_num["k3"];
    }else if (num > GlobalData::shared()->res_cost["k1"] && num <= GlobalData::shared()->res_cost["k2"]) {
        a = GlobalData::shared()->res1_num["k4"];
        b = GlobalData::shared()->res1_num["k5"];
        c = GlobalData::shared()->res1_num["k6"];
    }else if (num > GlobalData::shared()->res_cost["k2"] && num <= GlobalData::shared()->res_cost["k3"]) {
        a = GlobalData::shared()->res1_num["k7"];
        b = GlobalData::shared()->res1_num["k8"];
        c = GlobalData::shared()->res1_num["k9"];
    }else if (num > GlobalData::shared()->res_cost["k3"] && num <= GlobalData::shared()->res_cost["k4"]) {
        a = GlobalData::shared()->res1_num["k10"];
        b = GlobalData::shared()->res1_num["k11"];
        c = GlobalData::shared()->res1_num["k12"];
    }else if (num > GlobalData::shared()->res_cost["k4"]) {
        a = GlobalData::shared()->res2_num["k1"];
        b = GlobalData::shared()->res2_num["k2"];
        c = GlobalData::shared()->res2_num["k3"];
    }
    
    if (type == Food) {
        d = GlobalData::shared()->res3_num["k1"];
    }else if (type == Wood) {
        d = GlobalData::shared()->res3_num["k2"];
    }else if (type == Iron) {
        d = GlobalData::shared()->res3_num["k3"];
    }else if (type == Stone) {
        d = GlobalData::shared()->res3_num["k4"];
    }else if (type == Silver) {
        d = GlobalData::shared()->res3_num["k5"];
        //金币数=（资源*d）钢材单独处理
        ret = num*d;
        ret = MAX(ret, 1);
        return ret;
    }
    
    //金币数=int（a*（资源*d）^b+c）
    ret = powf(num*d, b)*a+c;
    ret = MAX(ret, 1);
    return ret;
    
    //////
//    map<string,float>* tmpVec = NULL;
//    if (type == Food || type == Wood) {
//        tmpVec = &(GlobalData::shared()->res1_num);
//    }
//    else if (type == Stone) {
//        tmpVec = &(GlobalData::shared()->res3_num);
//    }
//    else if (type == Iron) {
//        tmpVec = &(GlobalData::shared()->res2_num);
//    }
//    else {
//        return 0;
//    }
//    
//    int tmpNum = num;
//    bool _bTmp = true;
//    int maxIdx = tmpVec->size();
//    while (_bTmp) {
//        string key = "k";
//        key = key+CC_ITOA(maxIdx);
//        if (tmpVec->find(key) != tmpVec->end()) {
//            float curNum = (*tmpVec)[key];
//            if (tmpNum > curNum) {
//                ret += GlobalData::shared()->res_cost[key];
//                tmpNum -= curNum;
//                _bTmp = true;
//            }
//            else{
//                _bTmp = false;
//                for (int idx = 1; idx<=maxIdx; idx++) {
//                    string ck = "k";
//                    ck = ck+CC_ITOA(idx);
//                    if (tmpVec->find(ck) != tmpVec->end()) {
//                        float cn = (*tmpVec)[ck];
//                        if (tmpNum <= cn) {
//                            ret += GlobalData::shared()->res_cost[ck];
//                            break;
//                        }
//                    }
//                }
//            }
//        }
//    }
//    return ret;
}

CCSprite *CCCommonUtils::createGraySprite(const char* imageName)
{
    // create CCSprite
    CCSprite* sprite = CCLoadSprite::createSprite(imageName);
    return sprite;
    unsigned int width = sprite->getTexture()->getPixelsWide();
    unsigned int height = sprite->getTexture()->getPixelsHigh();
    
    // create CCRenderTexture base on the CCSprite
    CCRenderTexture* r = CCRenderTexture::create(width, height);
    r->beginWithClear(1, 1, 1, 0);
    sprite->setPosition(ccp(width / 2.0, height/ 2.0)); // Node: set position here!
    sprite->visit();
    r->end();
    
    // create a new CCImage
    CCImage* image = r->newCCImage();
    
    width = image->getWidth();
    height= image->getHeight();
    
    // this data is the texture data in memery
    unsigned char* data = image->getData();
    
    typedef enum {
        RED = 0,
        GREEN = 1,
        BLUE = 2,
        ALPHA = 3
    } PIXELS;
    
    // convert unsigned char*(1 Byte) to uint_32_t(4 Bytes)
    uint32_t *pixels = (uint32_t *)data;
    
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            uint8_t *rgbaPixel = (uint8_t *) &pixels[y * width + x];
            
            // convert to grayscale using recommended method: http://en.wikipedia.org/wiki/Grayscale#Converting_color_to_grayscale
            uint32_t gray = 0.3 * rgbaPixel[RED] + 0.59 * rgbaPixel[GREEN] + 0.11 * rgbaPixel[BLUE];
            
            // set the pixels to gray
            rgbaPixel[RED] = gray;
            rgbaPixel[GREEN] = gray;
            rgbaPixel[BLUE] = gray;
        }
    }
    
    // create a new CCTexture2D based on the CCImage data modified above
    CCTexture2D* texture = new CCTexture2D();
    texture->initWithImage(image);
    
    // create a new CCSprite to return
    CCSprite* s = CCSprite::create();
    s->initWithTexture(texture);
    // release other resources
    //    r->release();
    //    image->release();
    //    sprite->release();
    
    return s;
}


void CCCommonUtils::flyText(std::string floatText, cocos2d::ccColor3B color, float time){
    if(GuideController::share()->hasGuide()){
        return;
    }
    YesNoDialog::showYesDialog(floatText.c_str());
//    CCFloatingText* fly = CCFloatingText::create(floatText.c_str());
//    fly->setDuration(time);
//    fly->setColor(color);
//    auto topLayer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_POPUP);
//    fly->cocos2d::CCNode::setPosition(CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2);
//    topLayer->addChild(fly);
//    fly->startAnimation();
}
void CCCommonUtils::flyTextOnParent(std::string floatText, cocos2d::CCNode *parent,cocos2d::CCPoint point,cocos2d::ccColor3B color){
    if(!parent){
        flyText(floatText,color);
    }else{
        CCFloatingText* fly = CCFloatingText::create(floatText.c_str());
        fly->setColor(color);
        fly->cocos2d::CCNode::setPosition(point);
        parent->addChild(fly);
        fly->startAnimation();
    }
}
void CCCommonUtils::flyUiResText(std::string floatText, cocos2d::CCNode *parent,cocos2d::CCPoint point,cocos2d::ccColor3B color, FloatingTypeEnum floatType, int fontSize){
    if(!parent){
        flyText(floatText,color);
    }else{
        CCFloatingText* fly = CCFloatingText::create(floatText.c_str(), fontSize);
        fly->setColor(color);
        fly->setFloatingType(floatType);
        fly->cocos2d::CCNode::setPosition(point);
        parent->addChild(fly);
        fly->startAnimation();
    }
}
void CCCommonUtils::createGoodsIcon(int toolID, cocos2d::CCNode *mParent,CCSize defSize,CCLabelIF* mNum,CCLabelIF* mName, CCLabelIF* mDes, float op){
    if(ToolController::getInstance()->m_toolInfos.find(toolID)!=ToolController::getInstance()->m_toolInfos.end()){
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(toolID);
        CCSprite* pic = NULL;
        if(mParent->getChildByTag(GOODS_ICON_TAG)){
            pic = dynamic_cast<CCSprite*>(mParent->getChildByTag(GOODS_ICON_TAG));
        }
        CCSprite* iconBg = NULL;
        if(mParent->getChildByTag(GOODS_BG_TAG)){
            iconBg= dynamic_cast<CCSprite*>(mParent->getChildByTag(GOODS_BG_TAG));
        }
        CCSize pSize = mParent->getContentSize();
        if(iconBg){
            iconBg->setDisplayFrame(CCLoadSprite::loadResource(CCCommonUtils::getToolBgByColor(info.color).c_str()));
            CCCommonUtils::setSpriteMaxSize(iconBg, defSize.width*0.9,true);
        }else{
            iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(info.color).c_str());
            CCCommonUtils::setSpriteMaxSize(iconBg, defSize.width*0.9,true);
            mParent->addChild(iconBg);
            iconBg->setTag(GOODS_BG_TAG);
        }
        if(pic){
            pic->setDisplayFrame(CCLoadSprite::loadLoacalResource(CCCommonUtils::getIcon(CC_ITOA(info.itemId)).c_str(),true,CCLoadSpriteType_GOODS));
            CCCommonUtils::setSpriteMaxSize(pic, defSize.width*0.7,true);
            pic->setOpacity(op);
        }else{
            pic = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(info.itemId)).c_str(),true,CCLoadSpriteType_GOODS);
            CCCommonUtils::setSpriteMaxSize(pic, defSize.width*0.7,true);
            mParent->addChild(pic);
            pic->setTag(GOODS_ICON_TAG);
            pic->setOpacity(op);
        }
        if(info.type==3 && !info.getPara().empty()){
            CCScale9Sprite* numBG=NULL;
            if(mParent->getChildByTag(GOODS_NUM_BG_TAG)){
                numBG = dynamic_cast<CCScale9Sprite*>(mParent->getChildByTag(GOODS_NUM_BG_TAG));
                if(!numBG){
                    mParent->removeChildByTag(GOODS_NUM_BG_TAG);
                }
            }
            if(!numBG){
                numBG = CCLoadSprite::createScale9Sprite("BG_quatnity.png");
                mParent->addChild(numBG);
                numBG->setTag(GOODS_NUM_BG_TAG);
            }
            numBG->setColor(getItemColor(info.color));
            numBG->setOpacity(200);
            
            CCLabelBMFont* numIF=NULL;
            if(mParent->getChildByTag(GOODS_NUM_LBL_TAG)){
                numIF = dynamic_cast<CCLabelBMFont*>(mParent->getChildByTag(GOODS_NUM_LBL_TAG));
                if(!numIF){
                    mParent->removeChildByTag(GOODS_NUM_LBL_TAG);
                }
            }
            if(!numIF){
                numIF = CCLabelBMFont::create(CC_ITOA_K(atol(info.getPara().c_str())), "pve_fnt_boss.fnt");
                mParent->addChild(numIF);
                numIF->setTag(GOODS_NUM_LBL_TAG);
            }else{
                numIF->setString(CC_ITOA_K(atol(info.getPara().c_str())));
            }
            if(numIF && numBG){
                CCSize numSize = numIF->getContentSize();
                const float constScale = 0.3;
                float scale = defSize.height*constScale/numSize.height;
                if((numSize.width * scale) > defSize.width){
                    scale = defSize.width/numSize.width;
                }
                numIF->setScale(scale);
                numSize.height *= scale;
                numBG->setPreferredSize(CCSize(defSize.width, defSize.height*constScale));
                if(!pSize.equals(CCSizeZero)){
                    float pos = defSize.width*0.5;
                    numIF->setPosition(ccp(pos, pos + defSize.height * constScale * 0.5));
                }else{
                    numIF->setPositionY(defSize.height * constScale * 0.5);
                }
                numBG->setPosition(numIF->getPosition());
            }
        }else{
            CCNode* node1 = mParent->getChildByTag(GOODS_NUM_BG_TAG);
            if(node1){
                node1->removeFromParent();
            }
            CCNode* node2 = mParent->getChildByTag(GOODS_NUM_LBL_TAG);
            if(node2){
                node2->removeFromParent();
            }
        }
        if(!pSize.equals(CCSizeZero) && pic && iconBg){
            float pos = defSize.width*0.5;
            iconBg->setPosition(ccp(pos, pos));
            pos = (defSize.width - 5)*0.5;
            pic->setPosition(ccp(pos, pos));
        }
        
        if(mName){
            mName->setString(info.getName().c_str());
        }
        if(mDes){
            mDes->setString(_lang(info.des).c_str());
        }
        if(mNum){
            mNum->setString(CC_CMDITOA(info.getCNT()));
        }
    }else{
        if(mName){
            mName->setString("");
        }
        if(mDes){
            mDes->setString("");
        }
        if(mNum){
            mNum->setString("");
        }
    }
}
void CCCommonUtils::setButtonTitle(CCControlButton *button, const char *str) {
    string title = str;
    if (button) {
        button->setTitleForState(title, CCControlStateNormal);
        button->setTitleForState(title, CCControlStateHighlighted);
        button->setTitleForState(title, CCControlStateDisabled);
    }
    else {
        CCLOG("CCCommonUtils::setButtonTitle - Invalid button pointer.");
    }
}

void CCCommonUtils::setButtonTitleColor(CCControlButton *button, ccColor3B color) {
    if (button) {
        button->setTitleColorForState(color, CCControlStateNormal);
        button->setTitleColorForState(color, CCControlStateHighlighted);
        button->setTitleColorForState(color, CCControlStateDisabled);
    }
    else {
        CCLOG("CCCommonUtils::setButtonTitleColor - Invalid button pointer.");
    }
}
void CCCommonUtils::setButtonTitlePoint(CCControlButton *button, const CCPoint point) {
    if (button && button->getTitleLabel()) {
        button->getTitleLabel()->setAnchorPoint(CCPoint(0.5, 0.5));
        button->getTitleLabel()->setPosition(point);
    }
    else {
        CCLOG("CCCommonUtils::setButtonTitlePoint - Invalid button pointer.");
    }
}

void CCCommonUtils::setButtonSprite(CCControlButton *button, const char *str) {
    if (button && str) {
        button->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite(str), CCControlStateNormal);
        button->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite(str), CCControlStateHighlighted);
        button->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite(str), CCControlStateDisabled);
    }
    else {
        CCLOG("CCCommonUtils::setButtonSprite - Invalid button or str pointer.");
    }
}


 
void CCCommonUtils::addNewDizi(Node* sprite, int iType)
{
    string strResLine = "map_line_K.png";
    string strResJiao = "map_txt_jiao.png";
    int iRate = 1;
    if(iType == 1)
    {
        strResLine = "alliance_mem_line.png";
        strResJiao = "alliance_mem_jiao.png";
        iRate = 2;
    }
    const char* strLine = strResLine.c_str();
    const char* strJiao = strResJiao.c_str();
    //1.left 2.right 3.top, 4. bottom 5.增加4个角map_line_K.png  map_txt_jiao.png
    if(sprite)
    {
        //1.
        auto size = sprite->getContentSize();
        
        auto sp_1 = CCLoadSprite::createSprite(strLine);
        auto tBatchNode = CCSpriteBatchNode::createWithTexture(sp_1->getTexture());
        
        auto cellHeight = sp_1->getContentSize().height;

        int  numHeight = ceil(size.height/cellHeight);
        
        int  numWidth = ceil(size.width/cellHeight);
        //left right
        for(int i = 0 ; i < numHeight; i++)
        {
            auto bg_1 = CCLoadSprite::createSprite(strLine);
            bg_1->setAnchorPoint(ccp(0.5,0));
            bg_1->setPosition(ccp(0,i*cellHeight));
            
            auto bg_2 = CCLoadSprite::createSprite(strLine);
            bg_2->setAnchorPoint(ccp(0.5,0));
            bg_2->setPosition(ccp(size.width,i*cellHeight));
            
            tBatchNode->addChild(bg_1);
            tBatchNode->addChild(bg_2);
            if(i+1 == numHeight)
            {
                bg_1->setAnchorPoint(ccp(0.5,1));
                bg_1->setPosition(ccp(0,size.height));
                
                bg_2->setAnchorPoint(ccp(0.5,1));
                bg_2->setPosition(ccp(size.width,size.height));

            }
        }
        
        //top bottom
        for(int i = 0 ; i < numWidth; i++)
        {
            auto bg_1 = CCLoadSprite::createSprite(strLine);
            bg_1->setRotation(90);
            bg_1->setAnchorPoint(ccp(0.5,0));
            bg_1->setPosition(ccp(i*cellHeight,size.height));
            
            auto bg_2 = CCLoadSprite::createSprite(strLine);
            bg_2->setRotation(90);
            bg_2->setAnchorPoint(ccp(0.5,0));
            bg_2->setPosition(ccp(i*cellHeight,0));
            
            tBatchNode->addChild(bg_1);
            tBatchNode->addChild(bg_2);
            if(i+1 == numWidth)
            {
                bg_1->setAnchorPoint(ccp(0.5,1));
                bg_1->setPosition(ccp(size.width,size.height));
                
                bg_2->setAnchorPoint(ccp(0.5,1));
                bg_2->setPosition(ccp(size.width,0));
                
            }
        }
        sprite->addChild(tBatchNode);
        
        //jiao
        
        auto jia_1 = CCLoadSprite::createSprite(strJiao);
        jia_1->setAnchorPoint(ccp(0,0));
        jia_1->setPosition(ccp(0,0)+ccp(-3/iRate,-3/iRate));
        sprite->addChild(jia_1);
        
        auto jia_2 = CCLoadSprite::createSprite(strJiao);
        jia_2->setFlipX(true);
        jia_2->setAnchorPoint(ccp(1,0));
        jia_2->setPosition(ccp(size.width,0)+ccp(3/iRate,-3/iRate));
        sprite->addChild(jia_2);
        
        auto jia_3 = CCLoadSprite::createSprite(strJiao);
        jia_3->setFlipX(true);
        jia_3->setFlipY(true);
        jia_3->setAnchorPoint(ccp(1,1));
        jia_3->setPosition(ccp(size.width,size.height )+ccp(3/iRate,3/iRate));
        sprite->addChild(jia_3);
        
        auto jia_4 = CCLoadSprite::createSprite(strJiao);
        jia_4->setFlipY(true);
        jia_4->setAnchorPoint(ccp(0,1));
        jia_4->setPosition(ccp(0,size.height)+ccp(-3/iRate,3/iRate));
        sprite->addChild(jia_4);


        
    }
}


bool CCCommonUtils::FileToVector(std::string const& strFileName, std::vector<std::string>& vLines)
{
    std::ifstream ifile(strFileName);
    if (ifile.is_open() == false) {
        CCLOG("FileToVector - file error (%s).", strFileName.c_str());
        return false;
    }
        
    string line;
    while(getline(ifile,line)){
        if(!line.empty()){
            vLines.push_back(line);
        }
    }
    ifile.close();
    
    return true;
}

bool CCCommonUtils::FileToMap(std::string const& strFileName, std::map<std::string, std::string>& kv)
{
    // 只适合纯文本且文件行内容为key=value的形式。
    std::ifstream ifile(strFileName);
    if (ifile.is_open() == false) {
        CCLOG("FileToMap - file error (%s).", strFileName.c_str());
        return false;
    }
    
    string line;
    vector<string> mVector;
    while(getline(ifile,line)){
        if(line.empty()){
            continue;
        }
        
        if(line.find("=")!=string::npos){
            vector<std::string> mTmpV;
            CCCommonUtils::splitString(line, "=", mTmpV);
            if(mTmpV.size()>1){
                kv[mTmpV.at(0)]=mTmpV.at(1);
            }
        }
    }
    ifile.close();
    return true;
}

bool CCCommonUtils::FileToMD5(std::string const& strFileName, std::string& strMD5)
{
    std::ifstream ifs(strFileName);
    if (ifs.is_open() == false) {
        CCLOG("FileToMD5 - file error (%s).", strFileName.c_str());
        return false;
    }
    
    MD5 md5;
    md5.update(ifs);
    strMD5 = md5.toString();
    
    return true;
}

bool CCCommonUtils::SaveMapToFile(std::map<std::string, std::string>& kv, std::string const& strFileName)
{
    ofstream ofile(strFileName,ofstream::out);
    if (ofile.is_open() == false) {
        CCLOG("SaveMapToFile - file error (%s).", strFileName.c_str());
        return false;
    }
    
    auto iterMD5 = kv.begin();
    while (iterMD5!=kv.end()) {
        string writeStr =(*iterMD5).first + "=" + (*iterMD5).second;
        ofile<<writeStr<<endl;
        ++iterMD5;
    }
    ofile.close();
    return true;
}

CCArray* CCCommonUtils::split(const char* src, const char* sym)
{
    CCArray * ret = CCArray::create();
    
    std::string s = src;
    
    int nend=0;
    int nbegin=0;
    while(nend != -1)
    {
        nend = s.find(sym, nbegin);
        if(nend == -1)
            ret->addObject(CCString::create(s.substr(nbegin, s.length()-nbegin)));
        else
            ret->addObject(CCString::create(s.substr(nbegin, nend-nbegin)));
        nbegin = nend + strlen(sym);
    }
    return ret;
}

void CCCommonUtils::splitString(const std::string& strSrc, const std::string& strFind, std::vector<std::string>& arSplit) {
    string tmpStr(strSrc.data(),strSrc.length());
    
    if(tmpStr.length() > strFind.length())
    {
        while (!tmpStr.empty() && tmpStr.find(strFind.c_str()) == 0) {
            tmpStr = tmpStr.substr(strFind.length(), tmpStr.length()-strFind.length());
        }
        while (!tmpStr.empty() && tmpStr.rfind(strFind.c_str()) == (tmpStr.length()-strFind.length())) {
            tmpStr = tmpStr.substr(0, tmpStr.length()-strFind.length());
        }
    }
    
    char* src = const_cast<char*>(tmpStr.c_str());
    while (src != NULL && !tmpStr.empty()) {
        arSplit.push_back(std::string(strtok_r(src, strFind.c_str(), &src)));
    }
}
std::string CCCommonUtils::replaceString(const std::string &str, const std::string &src, const std::string &dest){
    string ret;
    
    string::size_type pos_begin = 0;
    string::size_type pos       = str.find(src);
    while (pos != string::npos)
    {
        ret.append(str.data() + pos_begin, pos - pos_begin);
        ret += dest;
        pos_begin = pos + 1;
        pos       = str.find(src, pos_begin);
    }
    if (pos_begin < str.length())
    {
        ret.append(str.begin() + pos_begin, str.end());
    }
    return ret;
}
int CCCommonUtils::getGoldBuyItem(std::string itemId){
    int value = atoi(CCCommonUtils::getPropById(itemId, "price").c_str());
    return value;
}

void CCCommonUtils::recordStep(std::string index){
    if(index == ""){
        return;
    }
    GuideRecordCommand* cmd = new GuideRecordCommand(index);
    cmd->sendAndRelease();
}

static long  _loadingStartTime;
void CCCommonUtils::setLoadingStartTime()
{
    _loadingStartTime = GameController::getInstance()->millisecondNow();
}

void CCCommonUtils::finishLoadingTotalTime()
{
    CCLog("==_loadingStartTime=%ld", _loadingStartTime);
    long _time = GameController::getInstance()->millisecondNow() - _loadingStartTime;
     CCLog("==_loadingStartTime=%ld", _time);
    char tmp[256] = {};
    sprintf(tmp, "loading_LoadingFinishTime_%ld", _time);
    postEventLogToServer(tmp);
}

void CCCommonUtils::postEventLogToServer(const std::string& paramlog)
{
    std::string uid = CCUserDefault::sharedUserDefault()->getStringForKey(GAME_UID, "");
    string deviceId = cocos2d::extension::CCDevice::getDeviceUid();
    std::string country = GlobalData::shared()->fromCountry;
    std::string platform = GlobalData::shared()->analyticID;
    std::string version = cocos2d::extension::CCDevice::getVersionName();
    int intStatusValue = cocos2d::extension::CCDevice::getNetworkStatus();
    std::string networkStatus = CCString::createWithFormat("%d",intStatusValue)->getCString();
    std::string step  = paramlog;
    
    
//    NetController_onConnectionLost_6331|NetController:getServerStatusBack_isSucceedFalse_55|GameController_onConnectError_0|NetController_onConnectionLost_8398
    
    //    std::vector<std::string> vector;
    //    cocos2d::extension::CCDevice::getLocalIP(vector);
    std::string ip = ""; // 手机IP
    //    if (vector.size() > 0) {
    //        ip = vector[0];
    //    }
    
    std::string serverIp = NetController::shared()->getRealIp();
    std::string serverZone = NetController::shared()->getZone();
    int serverPort = NetController::shared()->getRealPort();
    std::string connection = CCString::createWithFormat("%s:%d:%s", serverIp.c_str(), serverPort, serverZone.c_str())->getCString();
    
    CCHttpRequest* request = new CCHttpRequest();
    std::string param = CCString::createWithFormat("uid=%s&deviceId=%s&country=%s&ip=%s&version=%s&platform=%s&step=%s&connection=%s&networkStatus=%s",
                                                   uid.c_str(),
                                                   deviceId.c_str(),
                                                   country.c_str(),
                                                   ip.c_str(),
                                                   version.c_str(),
                                                   platform.c_str(),
                                                   step.c_str(),
                                                   connection.c_str(),
                                                   networkStatus.c_str())->getCString();
    //    CCString *url = CCString::createWithFormat("http://10.1.5.196:9000/client/loading");
    CCString *url = CCString::createWithFormat(POST_EVENT_LOG_URL,nullptr);
    CCLog("loadingLog:%s", url->getCString());
    request->setUrl(url->getCString());
    request->setRequestType(CCHttpRequest::Type::POST);
    const char* postData = param.c_str();
    request->setRequestData(postData, strlen(postData));
    
    CCHttpClient::getInstance()->send(request);
    request->release();
}

//void CCCommonUtils::recordStepByHttp(std::string index){
//    string _uuid = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_UUID,"");
//    if(_uuid != ""){
//        return;
//    }
//    CCHttpRequest* request = new CCHttpRequest();
//    
//    CCString* url;
//    url = CCString::createWithFormat("http://%s:8080/gameservice/settutorialstep?id=%s",SERVERLIST_IP, index.c_str());
//    
//    request->setUrl(url->getCString());
//    request->setRequestType(CCHttpRequest::Type::POST);
//    CCHttpClient::getInstance()->setTimeoutForConnect(1);
//    CCHttpClient::getInstance()->send(request);
//    request->release();
//}
std::string CCCommonUtils::getBonusString(cocos2d::CCArray *rewards){
    std::string ret = "";
    CCObject* reward;
    CCARRAY_FOREACH(rewards, reward){
        auto bon = _dict(reward);
        int type = bon->valueForKey("type")->intValue();
        std::string typeString = "";
        std::string itemId = "";
        switch (type) {
            case Silver:
                typeString = "105002";
                break;
            case Wood:
                typeString = "105003";
                break;
            case Iron:
                typeString = "105005";
                break;
            default:
                break;
        }
    }
    if(ret != ""){
       return ret.substr(0, ret.length() - 1);
    }
    return ret;
    rewards->release();
}
std::string CCCommonUtils::getMoreLanguageType(std::string sysTemKey){
    string type = "";
    if(sysTemKey=="zh_TW"){
        type = "1";
    }else if(sysTemKey=="zh_CN"){
        type = "1";
    }else if(sysTemKey=="pt"){
        type = "1";
    }else if(sysTemKey=="tr"){
        type = "1";
    }else if(sysTemKey=="no"){
        type = "1";
    }else if(sysTemKey=="ja"){
        type = "1";
    }else if(sysTemKey=="nl"){
        type = "1";
    }else if(sysTemKey=="it"){
        type = "1";
    }else if(sysTemKey=="de"){
        type = "1";
    }else if(sysTemKey=="es"){
        type = "1";
    }else if(sysTemKey=="th"){
        type = "1";
    }else if(sysTemKey=="ru"){
        type = "1";
    }else if(sysTemKey=="en"){
        type = "1";
    }
    return type;
}
std::string CCCommonUtils::getGeneralPicById(std::string generalId){
    std::string face = CCCommonUtils::getPropById(generalId, "icon");
    return face.append(".png");
}

std::string CCCommonUtils::getGeneralBustPicById(std::string generalId){
    std::string face = CCCommonUtils::getPropById(generalId, "icon");
    return face.append("_bust.png");
}

std::string CCCommonUtils::getNameById(std::string xmlId){
    auto titer = ToolController::getInstance()->m_toolInfos.find(atoi(xmlId.c_str()));
    if(titer!=ToolController::getInstance()->m_toolInfos.end()){
        return (*titer).second.getName();
    }
    std::string dialogId = CCCommonUtils::getPropById(xmlId, "name");
    return _lang(dialogId);
}

std::string CCCommonUtils::getPropById(std::string xmlId, std::string propName){
    CCDictionary* retDict = LocalController::shared()->DBXMLManager()->getObjectByKey(xmlId);
    if (!retDict) {
        CCLOG("loginfo---cant't find this itemId %s",xmlId.c_str());
        return string("");
    }

    CCString* ret = dynamic_cast<CCString*>(retDict->objectForKey(propName));
    if (ret == NULL) {
        CCLOG("CCCommonUtils::getPropById no name(%s). dictsize(%d).", propName.c_str(), retDict->count());
        return std::string();
    }
    
    if (ret->length() == 0) {
        CCLOG("CCCommonUtils::getPropById name(%s) empty.", propName.c_str());
        return string("");
    }
    
    return string(ret->getCString());
}
std::string CCCommonUtils::getParamByGroupAndKey(std::string groupId,std::string baseKey,std::string baseInfo, std::string desKey){
    string resStr("");
    auto allDic = GET_GROUPDIC_BY_GROUPNAME(groupId);
    CCDictElement* pelem = nullptr;
    CCDICT_FOREACH(allDic, pelem){
        auto modelDic = _dict(pelem->getObject());
        string content = modelDic->valueForKey(baseKey)->getCString();
        if (content == baseInfo) {
            resStr = modelDic->valueForKey(desKey)->getCString();
        }
    }
    return resStr;
}
std::string CCCommonUtils::getResourceIconByType(int resourceType) {
    std::string fix = ".png";
    switch (resourceType) {
        case Silver:
            return "ui_steel" + fix;
        case Wood:
            return "ui_wood" + fix;
        case Iron:
            return "ui_iron" + fix;
        case Food:
            return "ui_food" + fix;
        case Stone:
            return "ui_stone" + fix;
        case Gold:
            return "ui_gold" + fix;
        case 6:
            return "item409" + fix;
        case 7:
            return "item410" + fix;
        case 10://R_HONOR
            return "Contribution_icon" + fix;
        case 11://R_ALLIANCE_POINT
            return "Contribution_icon1" + fix;
        case 14://R_EQUIP
//            return "item410" + fix;
        case DragonFood:
            return "ui_food" + fix;
        case DragonGold:
            return "ui_gold" + fix;
        case LongJing:
            return "shuijing" + fix;
        default:
            break;
    }
    return std::string("");
}
std::string CCCommonUtils::getResourceIconByTypeNew(int resourceType) {
    std::string fix = ".png";
    switch (resourceType) {
        case Silver:
            return "ui_steel" + fix;
        case Wood:
            return "ui_wood_new" + fix;
        case Iron:
            return "ui_iron_new" + fix;
        case Food:
            return "ui_food_new" + fix;
        case Stone:
            return "ui_stone_new" + fix;
        case Gold:
            return "ui_gold" + fix;
        case 6:
            return "item409" + fix;
        case 7:
            return "item410" + fix;
        case 10://R_HONOR
            return "Contribution_icon" + fix;
        case 11://R_ALLIANCE_POINT
            return "Contribution_icon1" + fix;
        case 14://R_EQUIP
            //            return "item410" + fix;
        case DragonFood:
            return "ui_food" + fix;
        case DragonGold:
            return "ui_gold" + fix;
        case LongJing:
            return "shuijing" + fix;
        default:
            break;
    }
    return std::string("");
}


CCSprite* CCCommonUtils::getRomanSprite(int num){
    string path = "Roman_";
    path.append(CC_ITOA(num));
    path.append(".png");
    CCSprite* spr = CCLoadSprite::createSprite(path.c_str());
    return spr;
}
int CCCommonUtils::getResourceLoadByType(int type){
    switch (type) {
        case Wood:
            return GlobalData::shared()->resources_weight_1;
        case Stone:
            return GlobalData::shared()->resources_weight_2;
        case Iron:
            return GlobalData::shared()->resources_weight_3;
        case Food:
            return GlobalData::shared()->resources_weight_4;
        case Silver:
            return GlobalData::shared()->resources_weight_5;
        case Gold:
            return GlobalData::shared()->resources_weight_6;
        default:
            break;
    }
    return 1;
}

std::string CCCommonUtils::getArmIconByType(int armType) {
    std::string fix = "arm";
    fix = fix+CC_ITOA(armType)+"_icon.png";
    return fix;
}

std::string CCCommonUtils::getQueueIconByType(int queueType) {
    std::string fix = "_icon.png";
    switch (queueType) {
        case QueueType::TYPE_BUILDING:
            return "building" + fix;
        case QueueType::TYPE_FORCE:
        case QueueType::TYPE_RIDE_SOLDIER:
        case QueueType::TYPE_BOW_SOLDIER:
        case QueueType::TYPE_CAR_SOLDIER:
            return "force" + fix;
        case QueueType::TYPE_FORT:
            return "fort" + fix;
        case QueueType::TYPE_HOSPITAL:
            return "hospital" + fix;
        case QueueType::TYPE_AFFAIRS:
            return "affairs" + fix;
        case QueueType::TYPE_ALLIANCE_AFFAIRS:
            return "affairs" + fix;
        case QueueType::TYPE_OCCUPY_RESOURCE:
        case QueueType::TYPE_OCCUPY_ALLIANCE:
        case QueueType::TYPE_OCCUPY_MAZE:
        case QueueType::TYPE_OCCUPY_CAMP:
        case QueueType::TYPE_MARCH:
            return "march"+fix;
        case QueueType::TYPE_ARMY_MASS:{
            return "jijie.png";
        }
        case QueueType::TYPE_SCIENCE:
            return "science"+fix;
        case QueueType::TYPE_FORGE:
            return "science"+fix;
        case QueueType::TYPE_MATE:
            return "science"+fix;
        case QueueType::TYPE_BUILDING_TERRITORY:
            return "tile_pop_icon23.png";
        case QueueType::TYPE_REPAIR_TERRITORY:
            return "tile_pop_icon25.png";
        case QueueType::TYPE_STATION_TERRITORY:
            return "tile_pop_icon24.png";
        case QueueType::TYPE_DESTROY_TERRITORY:
            return "territory_destroy.png";
        case QueueType::TYPE_OCCUPY_TERRITORY_RESOURCE:
            return "tile_pop_icon27.png";
        default:
            break;
    }
    return std::string("");
}

std::string CCCommonUtils::getQueueNameByType(int queueType) {
    switch (queueType) {
        case QueueType::TYPE_BUILDING:
            return _lang("103650");
        case QueueType::TYPE_MARCH:
            return _lang("103651");
        case QueueType::TYPE_FORCE:
            return _lang("103652");
        case QueueType::TYPE_FORT:
            return _lang("103655");
        case QueueType::TYPE_HOSPITAL:
            return _lang("103653");
        case QueueType::TYPE_AFFAIRS:
            return _lang("103654");
        case TYPE_ALLIANCE_AFFAIRS:
            return _lang("103668");
        case QueueType::TYPE_SCIENCE:
            return _lang("103656");
        case QueueType::TYPE_FORGE:
            return _lang("103656");
        case QueueType::TYPE_MATE:
            return _lang("103656");
        default:
            break;
    }
    return std::string("");
}

int CCCommonUtils::getQueueSortByType(int queueType) {
    switch (queueType) {
        case QueueType::TYPE_BUILDING://建筑
            return 1;
        case QueueType::TYPE_SCIENCE://科技
            return 3;
        case QueueType::TYPE_FORCE://兵力
            return 4;
        case QueueType::TYPE_FORT://城防
            return 8;
        case QueueType::TYPE_HOSPITAL://医院
            return 5;
        case QueueType::TYPE_AFFAIRS://事务
            return 6;
        case TYPE_ALLIANCE_AFFAIRS:
            return 9;
        case QueueType::TYPE_MARCH://出征
            return 2;
        case QueueType::TYPE_RIDE_SOLDIER://骑兵
            return 10;
        case QueueType::TYPE_BOW_SOLDIER://弓兵
            return 11;
        case QueueType::TYPE_CAR_SOLDIER://车兵
            return 12;
        case QueueType::TYPE_FORGE://造装备
            return 13;
        case QueueType::TYPE_MATE://造材料
            return 14;
        default:
            break;
    }
    return 0;
}

std::string CCCommonUtils::getResourceProductByType(int resourceType){
    switch (resourceType) {
        case Silver:
            return _lang("108580");
        case Wood:
            return _lang("108576");
        case Iron:
            return _lang("108578");
        case Food:
            return _lang("108579");
        case Stone:
            return _lang("108577");
        case Gold:
            return _lang("108581");
        default:
            break;
    }
    return std::string("");
}

std::string CCCommonUtils::getResourceNameByType(int resourceType) {
    switch (resourceType) {
        case Silver:
            return _lang("102212");
        case Wood:
            return _lang("102209");
        case Iron:
            return _lang("102210");
        case Food:
            return _lang("102211");
        case Stone:
            return _lang("102208");
        case Gold:
            return _lang("107518");
        case Chip:
            return _lang("111101");
        case Diamond:
            return _lang("111102");
        case DragonFood:
            return _lang("102211");
        case DragonGold:
            return _lang("107518");
        case LongJing:
            return _lang("111654");//水晶
        default:
            break;
    }
    return std::string("");
}


bool CCCommonUtils::isEnoughResourceByType(int resourceType, int num) {
    switch (resourceType) {
        case Silver:
            if (GlobalData::shared()->resourceInfo.lMoney >= num) {
                return true;
            }
            return false;
        case Gold:
            if (GlobalData::shared()->playerInfo.gold >= num) {
                return true;
            }
            return false;

        case Wood:
            if (GlobalData::shared()->resourceInfo.totalWood() >= num) {
                return true;
            }
            return false;
        case Iron:
            if (GlobalData::shared()->resourceInfo.totalIron() >= num) {
                return true;
            }
            return false;
        case Food:
            if (GlobalData::shared()->resourceInfo.totalFood() >= num) {
                return true;
            }
            return false;
        case Stone:
            if (GlobalData::shared()->resourceInfo.totalStone() >= num) {
                return true;
            }
            return false;
        case LongJing:
            if (GlobalData::shared()->playerInfo.longJing >= num) {
                return true;
            }
            return false;
        default:
            break;
    }
    return false;
}

int CCCommonUtils::getCurResourceByType(int resourceType) {
    switch (resourceType) {
        case Silver:
            return GlobalData::shared()->resourceInfo.lMoney;
        case Wood:
            return GlobalData::shared()->resourceInfo.totalWood();
        case Iron:
            return GlobalData::shared()->resourceInfo.totalIron();
        case Food:
            return GlobalData::shared()->resourceInfo.totalFood();
        case Stone:
            return GlobalData::shared()->resourceInfo.totalStone();
        default:
            break;
    }
    return 0;
}

void CCCommonUtils::fitContainerSizeAndAddChild(cocos2d::CCNode *container, cocos2d::CCNode *child , int type) {
    auto containerSize = container->getContentSize();
    auto childSize = child->getContentSize();
    switch (type) {
        case 0:{
            child->setScaleX(containerSize.width/childSize.width);
            child->setScaleY(containerSize.height/childSize.height);
        }
            break;
        case 1:{
            child->setScale(containerSize.width/childSize.width);
        }
            break;
        case 2:{
            child->setScale(containerSize.height/childSize.height);
        }
            break;
            
        default:
            return;
    }
    child->setAnchorPoint(ccp(0.5, 0.5));
    child->setPosition(ccp(containerSize.width/2, containerSize.height/2));
    container->addChild(child);
}

std::string CCCommonUtils::getIcon(std::string id){
    return CCCommonUtils::getPropById(id, "icon").append(".png");
}

bool CCCommonUtils::isEquip(int itemId){
    if ( itemId >= 1000000 && itemId <= 1999999) {
        return true;
    }
    return false;
}

cocos2d::ccColor3B CCCommonUtils::getItemColor(int color){
    switch (color) {
        case WHITE:
            return ccc3(227, 227, 227);
            break;
        case GREEN:
            return ccc3(36, 255, 0);
            break;
        case BLUE:
            return ccc3(0, 132, 255);
            break;
        case PURPLE:
            return ccc3(246, 0, 255);
            break;
        case ORANGE:
            return ccc3(255, 138, 0);
            break;
        case GOLDEN:
            return ccc3(255, 210, 0);
            break;
        case RED:
            return ccc3(255, 0, 0);
            break;
        default:
            break;
    }
    return ccWHITE;
}

string CCCommonUtils::getColorName(int color)
{
    switch (color) {
        case WHITE:
            return _lang("119043");
            break;
        case GREEN:
            return _lang("119044");
            break;
        case BLUE:
            return _lang("119045");
            break;
        case PURPLE:
            return _lang("119046");
            break;
        case ORANGE:
            return _lang("119047");
            break;
        case GOLDEN:
            return _lang("119048");
            break;
        case RED:
            return _lang("119049");
            break;
        default:
            break;
    }
    return "";
}

std::string CCCommonUtils::getToolBgByColor(int color)
{
    switch (color) {
        case WHITE:
            return "tool_1.png";
            break;
        case GREEN:
            return "tool_2.png";
            break;
        case BLUE:
            return "tool_3.png";
            break;
        case PURPLE:
            return "tool_4.png";
            break;
        case ORANGE:
            return "tool_5.png";
            break;
        case GOLDEN:
            return "tool_6.png";
            break;
        case RED:
            return "tool_7.png";
            break;
        default:
            break;
    }
    return "tool_1.png";
}

std::string CCCommonUtils::getBoxBgByColor(int color)
{
    switch (color) {
        case WHITE:
            return "reward_white_red.png";
            break;
        case GREEN:
            return "reward_green_red.png";
            break;
        case BLUE:
            return "reward_blue_red.png";
            break;
        case PURPLE:
            return "reward_red_red.png";
            break;
        case ORANGE:
            return "reward_orange_red.png";
            break;
        case GOLDEN:
            return "reward_gold_red.png";
            break;
        case RED:
            return "reward_red_red.png";
            break;
        default:
            break;
    }
    return "reward_white_red.png";
}
std::string CCCommonUtils::getGenHeadBgByColor(int color)
{
    string ret = "general_pic_bg_";
    ret = ret + CC_ITOA(color)+".png";
    return ret;
}

bool CCCommonUtils::isBtnOpen(int btnIndex){
    vector<std::string>vector;
    if (GlobalData::shared()->playerInfo.openedSystem == "") {
        return false;
    }
    splitString(GlobalData::shared()->playerInfo.openedSystem, ",", vector);
    int i = 0;
    while(i < vector.size()){
        if(atoi(vector[i].c_str()) == btnIndex){
            return true;
        }
        i++;
    }
    return false;
}

bool CCCommonUtils::isLackOfGold(int i){
    if(GlobalData::shared()->playerInfo.gold < i){
        return true;
    }
    return false;
}
std::string CCCommonUtils::timeLeftToCountDown(int time,const char* separator) {
    unsigned int tmpTime = time < 0 ? 0 : time;

    std::string ret = string("");
    int hours = tmpTime/3600;
    if (hours < 10) {
        ret = ret + "0";
    }
    ret = ret + CC_ITOA(hours) + separator;
    int tmp = tmpTime%3600;
    int minutes = tmp/60;
    if (minutes < 10) {
        ret = ret + "0";
    }
    ret = ret + CC_ITOA(minutes) +separator;
    int seconds = tmp%60;
    if (seconds < 10) {
        ret = ret + "0";
    }
    ret = ret + CC_ITOA(seconds);
    return ret;
}
std::string CCCommonUtils::getTalentStr(std::string talent, int type){
    std::string ret = "";
    if(strstr(talent.c_str(), "|") != NULL){
        std::string talentTypeStr = talent.substr(talent.find_first_of("|") + 1);
        std::string valueStr = talent.substr(0, talent.find_first_of("|"));
        vector<std::string> vector;
        CCCommonUtils::splitString(talentTypeStr, ";", vector);
        int i = 0;
        while(i < vector.size()){
            int talentType = atoi(vector[i].c_str());
            std::string dialogId = "1054";
            dialogId.append(CC_ITOA(talentType + 2)).append(CC_ITOA(type));
            ret.append(_lang(dialogId.c_str())).append("\n");
            i++;
        }
    }
    return ret;
}

std::string CCCommonUtils::getTacticsStr(std::string talent, int type){
    if(talent == ""){
        return "";
    }
    std::string valueStr = talent.substr(0, talent.find_first_of("|"));
    std::string dialogId = "";
    switch (type) {
        case MOUNTAIN:
            dialogId = "105411";
            break;
        case WATER:
            dialogId = "105412";
            break;
        case PLAIN:
            dialogId = "105413";
            break;
        case CITY:
            dialogId = "105414";
            break;
        default:
            return "";
            break;
    }
    return _lang(dialogId).append(" + ").append(valueStr).append("%");
}

std::string CCCommonUtils::getItemPropStr(EquipInfo* info, std::string att, int value){
    std::string result = "";
    if(att == ""){
        return "";
    }
    int max = atoi(CCCommonUtils::getPropById(info->itemId, "max").c_str());
    result = _lang(CC_ITOA(atoi(info->attr1.c_str()) + 105800));
    result = _lang_2("105800", result.c_str(), CC_ITOA(value));
    if(info->attrLevel1 >= max){
        result.append(_lang("105815"));
    }
    return result;
}

int CCCommonUtils::getSoulExChangeNeedByColor(int color){
    int num = 1;
    if(color == GREEN){
        num = GlobalData::shared()->generalConfig.greenNum;
    }else if(color == BLUE){
        num = GlobalData::shared()->generalConfig.blueNum;
    }else if(color == PURPLE){
        num = GlobalData::shared()->generalConfig.purpleNum;
    }
    return num;
}

std::string CCCommonUtils::getGeneralSkill(std::string generalId){
    vector<std::string> vector;
    std::string skillId = CCCommonUtils::getPropById(generalId, "skill");
    CCCommonUtils::splitString(skillId, ";", vector);
    return vector[1];
}

bool CCCommonUtils::isFormationOpen(int index){
    return true;
}

int CCCommonUtils::getFormationOpenNum(){
    int level = GlobalData::shared()->playerInfo.level;
    int count = 0;
    map<int, int>::iterator it;
    for(it = GlobalData::shared()->generalConfig.formationOpen.begin()
        ; it != GlobalData::shared()->generalConfig.formationOpen.end()
        ; it++){
        if(level >= it->first && it->second > count){
            count = it->second;
        }
    }
    return count;
}

int CCCommonUtils::getNextFormationOpenLevel(){
    int level = GlobalData::shared()->playerInfo.level;
    int result = -1;
    map<int, int>::iterator it;
    for(it = GlobalData::shared()->generalConfig.formationOpen.begin()
        ; it != GlobalData::shared()->generalConfig.formationOpen.end()
        ; it++){
        if(level < it->first && (result > it->first || result == -1)){
            result = it->first;
        }
    }
    return result;
}

float CCCommonUtils::getStateEffectValueByNum(int num)
{
    float ret = 0;
    
    if(GlobalData::shared()->effectStateMap.find(num) != GlobalData::shared()->effectStateMap.end())
    {
        auto& info = GlobalData::shared()->effectStateMap[num];
        for (int i=0; i<info.size(); i++)
        {
            if (GlobalData::shared()->statusMap.find(info[i].stateId) != GlobalData::shared()->statusMap.end() &&
                GlobalData::shared()->getWorldTime()<GlobalData::shared()->statusMap[info[i].stateId]) {
                ret += info[i].value;
            }
        }
    }
    return ret;
}

float CCCommonUtils::getEffectValueByNum(int num, bool isInludeGeneral)
{
    float ret = 0;
    if (1) {
        if(GlobalData::shared()->effectStateMap.find(num) != GlobalData::shared()->effectStateMap.end())
        {
            auto& info = GlobalData::shared()->effectStateMap[num];
            for (int i=0; i<info.size(); i++)
            {
                //zym 2015.10.15
                if( GlobalData::shared()->statusMap.find(info[i].stateId) != GlobalData::shared()->statusMap.end() )
                {
                    if (GlobalData::shared()->getWorldTime()<GlobalData::shared()->statusMap[info[i].stateId]) {
                        ret += info[i].value;
                    }
                }
               
            }
        }
    }
    
    //zym 2015.10.15 判断有没有再加
    float fEffectValue = 0.f;
    if(  GlobalData::shared()->effectValues.find(num) !=  GlobalData::shared()->effectValues.end() )
        fEffectValue = GlobalData::shared()->effectValues[num];
    
    if(isInludeGeneral){
//        if(num == ARMY_LOAD_EFFECT){
//            ret += GlobalData::shared()->effectValues[num] + getVipEffectValueByNum(num) + GeneralManager::getInstance()->getValueByEffect(GENERAL_LOAD_EFFECT);
//        }else{
            ret += fEffectValue + getVipEffectValueByNum(num) + GeneralManager::getInstance()->getValueByEffect(num) + getOfficerValueByNum(num) + getTerritoryEffectValueByNum(num);
//        }
    }else{
        ret += fEffectValue + getVipEffectValueByNum(num) + getOfficerValueByNum(num) + getTerritoryEffectValueByNum(num);
    }
    
    if( EquipmentController::getInstance()->EquipEffMap.find(num) != EquipmentController::getInstance()->EquipEffMap.end() )
    {
        ret += EquipmentController::getInstance()->EquipEffMap[num] + EquipmentController::getInstance()->getEffectSciValueByNum(num);
    }
    
    if(GlobalData::shared()->playerInfo.isInAlliance() && ScienceController::getInstance()->alScEffMap.find(num) != ScienceController::getInstance()->alScEffMap.end() ) {
        ret += ScienceController::getInstance()->alScEffMap[num];
    }
    if(GlobalData::shared()->serverType==SERVER_DRAGON_BATTLE){
        ret += getDragonValueByNum(num);
    }
    return ret;
}
float CCCommonUtils::getOfficerValueByNum(int num){
    float ret = 0.0;
    if(GlobalData::shared()->playerInfo.officer != ""){
        std::string param1 = getPropById(GlobalData::shared()->playerInfo.officer, "para1");
        std::string param2 = getPropById(GlobalData::shared()->playerInfo.officer, "para2");
        if(param1 != "" && param2 != ""){
            std::vector<std::string> effect;
            std::vector<std::string> value;
            CCCommonUtils::splitString(param1, ";", effect);
            CCCommonUtils::splitString(param2, ";", value);
            if(effect.size() != value.size()){
                return ret;
            }
            int index = 0;
            while (index < effect.size()) {
                int effectValue = atoi(effect[index].c_str());
                if(effectValue == num){
                    float valueValue = atoi(value[index].c_str());
                    return valueValue;
                }
                index++;
            }
        }
    }
    return ret;
}

float CCCommonUtils::getDragonValueByNum(int num){
    float ret = 0;
    for (auto it=WorldController::getInstance()->m_dbuildingInfo.begin(); it!=WorldController::getInstance()->m_dbuildingInfo.end(); it++) {
        int left = (WorldController::getInstance()->getTime() - it->second.occupyST)/1000;
        if(left>=0 && it->second.ai!="" && it->second.ai==GlobalData::shared()->playerInfo.allianceInfo.uid){
            string parm1 = CCCommonUtils::getPropById(it->second.itemId, "parm1");
            string parm2 = CCCommonUtils::getPropById(it->second.itemId, "parm2");
            if (parm1!="" && parm2!="") {
                std::vector<std::string> effect;
                std::vector<std::string> value;
                CCCommonUtils::splitString(parm1, ";", effect);
                CCCommonUtils::splitString(parm2, ";", value);
                if(effect.size() != value.size()){
                    continue;
                }
                int index = 0;
                while (index < effect.size()) {
                    int effectValue = atoi(effect[index].c_str());
                    if(effectValue == num){
                        ret += atoi(value[index].c_str());
                    }
                    index++;
                }
            }
        }
    }
    return ret;
}

float CCCommonUtils::getTerritoryEffectValueByNum(int num){
    float ret = 0.0;
    if (GlobalData::shared()->alliance_territory_positive_effect > 0 || GlobalData::shared()->alliance_territory_positive_effect_gather == 1) {
        map<int, int> territoryEffectMap;
        CCDictElement *element;
        auto dictEffect = LocalController::shared()->DBXMLManager()->getGroupByKey("territory_effect");
        CCDICT_FOREACH(dictEffect, element){
            std::string effectId = element->getStrKey();
            std::string buffStr = CCCommonUtils::getPropById(effectId, "k2");
            std::string numStr = CCCommonUtils::getPropById(effectId, "k1");
            if( numStr.empty() )
                continue;
            std::vector<std::string> effect;
            CCCommonUtils::splitString(numStr, ";", effect);
            if (effect.size() == 1) {
                int value = atoi(buffStr.c_str());
                int numTemp = atoi(effect[0].c_str());
                territoryEffectMap[numTemp] = value;
            }
            else if (effect.size() == 4) {
                for (int i = 0; i < 4; i++) {
                    int value = atoi(buffStr.c_str());
                    int numTemp = atoi(effect[i].c_str());
                    territoryEffectMap[numTemp] = value;
                }
            }
        }
        if (num != 65) {
            map<int, int>::iterator tItr;
            tItr = territoryEffectMap.find(num);
            if (tItr != territoryEffectMap.end()) {
                ret += tItr->second * GlobalData::shared()->alliance_territory_positive_effect;
                return ret;
            }
            else
                return ret;
        }
        else {
            if (GlobalData::shared()->alliance_territory_positive_effect_gather == 1) {
                map<int, int>::iterator tItr;
                tItr = territoryEffectMap.find(num);
                if (tItr != territoryEffectMap.end()) {
                    ret += tItr->second *  GlobalData::shared()->alliance_territory_positive_effect_gather;
                }
                GlobalData::shared()->alliance_territory_positive_effect_gather = 0;
                return ret;
            }
            else
                return ret;
        }
    }
    return ret;
}

float CCCommonUtils::getTerritoryNegativeEffectValueByNum(int num){
    float ret = 0.0;
    if (GlobalData::shared()->alliance_territory_negative_effect > 0) {
        map<int, int> territoryEffectMap;
        CCDictElement *element;
        auto dictEffect = LocalController::shared()->DBXMLManager()->getGroupByKey("territory_effect");
        CCDICT_FOREACH(dictEffect, element){
            std::string effectId = element->getStrKey();
            std::string buffStr = CCCommonUtils::getPropById(effectId, "k2");
            std::string numStr = CCCommonUtils::getPropById(effectId, "k1");
            std::vector<std::string> effect;
            CCCommonUtils::splitString(numStr, ";", effect);
            if (effect.size() == 1) {
                int value = atoi(buffStr.c_str());
                int numTemp = atoi(effect[0].c_str());
                territoryEffectMap[numTemp] = value;
            }
            else if (effect.size() == 4) {
                for (int i = 0; i < 4; i++) {
                    int value = atoi(buffStr.c_str());
                    int numTemp = atoi(effect[i].c_str());
                    territoryEffectMap[numTemp] = value;
                }
            }
        }
        
        if( territoryEffectMap.find(num) != territoryEffectMap.end() )
            return territoryEffectMap[num] * GlobalData::shared()->alliance_territory_negative_effect;
    }
    return ret;
}

string CCCommonUtils::getEffFormat(string num)
{
    if(num == "" || num.length() == 0){
        return "%";
    }

    map<string, int>::iterator iter = GlobalData::shared()->effToFormatMap.find(num);
    if (iter != GlobalData::shared()->effToFormatMap.end()) {
        if (iter->second == 1) {
            return "";
        }else {
            return "%";
        }
    }
    else {
        return "%";
    }
}

string CCCommonUtils::getEffPM(string num)
{
    if(num == "" || num.length() == 0){
        return "+";
    }

    map<string, int>::iterator iter = GlobalData::shared()->effPM_Map.find(num);
    if (iter != GlobalData::shared()->effPM_Map.end()) {
        if (iter->second == 1) {
            return "-";
        }else {
            return "+";
        }
    }
    else {
        return "+";
    }
}

float CCCommonUtils::getVipEffectValueByNum(int num)
{
    float ret = 0;
    if (GlobalData::shared()->playerInfo.vipEndTime > GlobalData::shared()->getWorldTime()) {
        if(GlobalData::shared()->vipEffectMap.find(num) != GlobalData::shared()->vipEffectMap.end())
        {
            ret += GlobalData::shared()->vipEffectMap[num];
        }
    }
    return ret;
}

int CCCommonUtils::getQueueTypeByBuildType(int buildType)
{
    if (buildType == FUN_BUILD_SCIENE) {
        return TYPE_SCIENCE;
    }
    else if (buildType == FUN_BUILD_BARRACK1) {
        return TYPE_FORCE;
    }
    else if (buildType == FUN_BUILD_BARRACK2) {
        return TYPE_RIDE_SOLDIER;
    }
    else if (buildType == FUN_BUILD_BARRACK3) {
        return TYPE_BOW_SOLDIER;
    }
    else if (buildType == FUN_BUILD_BARRACK4) {
        return TYPE_CAR_SOLDIER;
    }
    else if (buildType == FUN_BUILD_FORT) {
        return TYPE_FORT;
    }
    else if (buildType == FUN_BUILD_HOSPITAL) {
        return TYPE_HOSPITAL;
    }
    else if (buildType == FUN_BUILD_FORGE) {
        return TYPE_FORGE;
    }
    else if (buildType == FUN_BUILD_WORKSHOP) {
        return TYPE_MATE;
    }
//    else if (buildType == DBUILDING_HATCHER) {
//        return TYPE_DRAGON_BRON;
//    }
//    else if (buildType == DBUILDING_CAVE) {
//        return TYPE_DRAGON_EGG;
//    }

    return -1;
}

CCSprite* CCCommonUtils::setSpriteMaxSize(CCSprite* spr, int limitNum, bool isForce)
{
    if( spr == NULL )
        return NULL;
    
    if (isForce || spr->getContentSize().width>limitNum || spr->getContentSize().height>limitNum) {
        float sc1 = limitNum*1.0/spr->getContentSize().width;
        float sc2 = limitNum*1.0/spr->getContentSize().height;
        spr->setScale( sc1<sc2?sc1:sc2 );
    }
    return spr;
}

void CCCommonUtils::setSpriteGray(CCSprite *sprite, bool gray){
    if (sprite == NULL) {
        return;
    }
    if (gray)
    {
//        TODO 暂时去掉etc
        if (sprite->getTexture()->getAlphaName() > 0)
        {
            sprite->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(GLProgram::SHADER_NAME_POSITION_ETC1_GRAY_NO_MVP));
        }
        else
        {
            sprite->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureGray));
        }
    }
    else
    {
        if (sprite->getTexture()->getAlphaName() > 0)
        {
            sprite->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(GLProgram::SHADER_NAME_POSITION_ETC1_A_NO_MVP));
        }
        else
        {
//            sprite->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture));
            sprite->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
    
        }
    }
}

void CCCommonUtils::setSpriteBlur(Sprite* sprite, const Vec2& blurSize)
{
    ///
    auto glProgram = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_BLUR);
    auto glState = GLProgramState::create(glProgram);
    sprite->setGLProgramState(glState);
//    float width = 1.0f / loadingBG->getContentSize().width;
//    float height = 1.0f / loadingBG->getContentSize().height;
    glState->setUniformVec2(glProgram->getUniform("blurSize")->location, blurSize);
    
}

void CCCommonUtils::setSpriteDiffuse(cocos2d::Sprite *sprite, float outlineSize, cocos2d::Color3B outlineColor, cocos2d::Size textureSize, cocos2d::Color3B foregroundColor/* = cocos2d::Color3B::WHITE*/)
{
    auto glProgram = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_BLUR);
    
    if (!glProgram)
    {
        std::string fragmentSource = FileUtils::getInstance()->getStringFromFile(FileUtils::getInstance()->fullPathForFilename("shaders/stroke.fsh"));
        glProgram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragmentSource.c_str());
        GLProgramCache::getInstance()->addGLProgram(glProgram, GLProgram::SHADER_NAME_POSITION_BLUR);
    }
    
    auto glprogramState = GLProgramState::create(glProgram);
    
    glprogramState->setUniformFloat("outlineSize", outlineSize);
    glprogramState->setUniformVec3("outlineColor", Vec3(outlineColor.r / 255.0f, outlineColor.g / 255.0f, outlineColor.b / 255.0f));
    glprogramState->setUniformVec2("texSize", Vec2(textureSize.width, textureSize.height));
    glprogramState->setUniformVec3("foregroundColor", Vec3(foregroundColor.r / 255.0f, foregroundColor.g / 255.0f, foregroundColor.b / 255.0f));
    
}

CCSprite* CCCommonUtils::graylightWithCCSprite(CCSprite* oldSprite,bool isLight)
{
    //CCSprite转成CCimage
    CCPoint p = oldSprite->getAnchorPoint();
    oldSprite->setAnchorPoint(ccp(0,0));
    CCRenderTexture *outTexture = CCRenderTexture::create((int)oldSprite->getContentSize().width,(int)oldSprite->getContentSize().height);
    outTexture->begin();
    oldSprite->visit();
    outTexture->end();
    oldSprite->setAnchorPoint(p);
    
    CCImage* finalImage = outTexture->newCCImage();
    unsigned char *pData = finalImage->getData();
    int iIndex = 0;
    
    if(isLight)
    {
        for (int i = 0; i < finalImage->getHeight(); i ++)
        {
            for (int j = 0; j < finalImage->getWidth(); j ++)
            {
                // highlight
                int iHightlightPlus = 50;
                int iBPos = iIndex;
                unsigned int iB = pData[iIndex];
                iIndex ++;
                unsigned int iG = pData[iIndex];
                iIndex ++;
                unsigned int iR = pData[iIndex];
                iIndex ++;
                //unsigned int o = pData[iIndex];
                iIndex ++;  //原来的示例缺少
                iB = (iB + iHightlightPlus > 255 ? 255 : iB + iHightlightPlus);
                iG = (iG + iHightlightPlus > 255 ? 255 : iG + iHightlightPlus);
                iR = (iR + iHightlightPlus > 255 ? 255 : iR + iHightlightPlus);
                //            iR = (iR < 0 ? 0 : iR);
                //            iG = (iG < 0 ? 0 : iG);
                //            iB = (iB < 0 ? 0 : iB);
                pData[iBPos] = (unsigned char)iB;
                pData[iBPos + 1] = (unsigned char)iG;
                pData[iBPos + 2] = (unsigned char)iR;
            }
        }
    }else{
        for (int i = 0; i < finalImage->getHeight(); i ++)
        {
            for (int j = 0; j < finalImage->getWidth(); j ++)
            {
                // gray
                int iBPos = iIndex;
                unsigned int iB = pData[iIndex];
                iIndex ++;
                unsigned int iG = pData[iIndex];
                iIndex ++;
                unsigned int iR = pData[iIndex];
                iIndex ++;
                //unsigned int o = pData[iIndex];
                iIndex ++; //原来的示例缺少
                unsigned int iGray = 0.3 * iR + 0.4 * iG + 0.2 * iB;
                pData[iBPos] = pData[iBPos + 1] = pData[iBPos + 2] = (unsigned char)iGray;
            }
        }
    }
    
    CCTexture2D *texture = new CCTexture2D;
    texture->initWithImage(finalImage);
    CCSprite* newSprite = CCSprite::createWithTexture(texture);
    delete finalImage;
    texture->release();
    return newSprite;
}



string CCCommonUtils::getResourceStr(int resource) {
    if (resource < 0) {
        resource = 0;
    }
    string ret = CC_ITOA(resource);
    unsigned int size = ret.size();
    while (size > 3) {
        size -= 3;
        ret.insert(size, ",");
    }
    return ret;
}




bool CCCommonUtils::ButtonNode::isTouchInNode(cocos2d::CCTouch *pTouch) {
    auto parent = getParent();
    if (parent) {
        auto touch = parent->convertTouchToNodeSpace(pTouch);
        auto position = getPosition();
        float offsetX = m_touchSize.width/2.0;
        float offsetY = m_touchSize.height/2.0;
        if (touch.x > (position.x - offsetX)
            && touch.x < (position.x + offsetX)
            && touch.y > (position.y - offsetY)
            && touch.y < (position.y + offsetY))
        {
            return true;
        }
    }
    return false;
}

bool CCCommonUtils::ButtonNode::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if (m_touchBegan != NULL) {
        return m_touchBegan(this,pTouch,pEvent);
    }
    bool touchInNode = isTouchInNode(pTouch);
    if (touchInNode) {
        if (m_touchDoBegin != NULL) {
            m_touchDoBegin(touchInNode);
        }
        return true;
    }
    return false;
}

void CCCommonUtils::ButtonNode::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if (m_touchMoved != NULL) {
        return m_touchMoved(this,pTouch,pEvent);
    }
    if (m_touchDoMove != NULL) {
        m_touchDoMove(isTouchInNode(pTouch));
    }
}

void CCCommonUtils::ButtonNode::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if (m_touchEnded != NULL) {
        return m_touchEnded(this,pTouch,pEvent);
    }
    if (m_touchDoEnd != NULL) {
        m_touchDoEnd(isTouchInNode(pTouch));
    }
}

void CCCommonUtils::ButtonNode::onTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if (m_touchCancelled != NULL) {
        return m_touchCancelled(this,pTouch,pEvent);
    }
    if (m_touchDoCancel != NULL) {
        m_touchDoCancel(isTouchInNode(pTouch));
    }
}

void CCCommonUtils::ButtonNode::onEnter() {
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_priority, m_swallow);
}

void CCCommonUtils::ButtonNode::onExit() {
    setTouchEnabled(false);
    CCNode::onExit();
}

void CCCommonUtils::ButtonNode::setPriority(int priority) {
    if (m_priority != priority) {
        m_priority = priority;
        resetTouch();
    }
}

void CCCommonUtils::ButtonNode::setSwallow(bool isSwallow) {
    if (m_swallow != isSwallow) {
        m_swallow = isSwallow;
        resetTouch();
    }
}

void CCCommonUtils::ButtonNode::resetTouch() {
    // CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    // CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_priority, m_swallow);
}

void CCCommonUtils::flyHint(std::string icon, std::string titleText, std::string contentText, float time, float dy, bool isMid, float proY, std::string mailId){
    CCLOG("CCCommonUtils::flyHint");
    auto hint = FlyHint::create(icon, titleText, contentText, time,dy);
    
    if (isMid && hint != NULL) {
        auto size = CCDirector::sharedDirector()->getWinSize();
        hint->setPosition(size.width/2, size.height/2);
    }
    if (!isMid && proY>0 && hint != NULL) {
        if (isTestPlatformAndServer("FlyHint")) {
            auto size = CCDirector::sharedDirector()->getWinSize();
            hint->setPosition(size.width/2, size.height*0.2);
        }
        else {
            auto size = CCDirector::sharedDirector()->getWinSize();
            hint->setPosition(size.width/2, size.height*proY);
        }
    }

    if (mailId!="" && hint != NULL) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(!ChatServiceCocos2dx::isChatShowing)
        {
            hint->setMailId(mailId);
        }
#else
        if (ChatServiceCocos2dx::isChatShowing_fun()) {
              hint->setMailId(mailId);
        }
      
#endif
        
    }
    //hint->show();
}
void CCCommonUtils::flyHintWithDefault(std::string icon, std::string titleText, std::string contentText,float time, float dy){
    auto hint = FlyHint::create(icon, titleText,contentText,time,dy,true);
}
void CCCommonUtils::flySystemUpdateHint(double countDown, bool isLogin,string tip,FlyHintType type,string icon){
    LuaController::getInstance()->addSystemUpdate(countDown,isLogin,tip,type,icon);
}

CCAnimate* CCCommonUtils::makeEffectAnimate(CCSprite* spr, string path, int num, float dt, int idx)
{
    int size = num;
    Vector<SpriteFrame*> myArray;
    CCSpriteFrame *myframe[size];
    for (int i=0; i<size; i++) {
        myframe[i] = CCLoadSprite::loadResource(CCString::createWithFormat(path.c_str(), i+1)->getCString());
        myframe[i]->getTexture()->setAntiAliasTexParameters();
        myArray.pushBack(myframe[i]);
    }
    spr->setDisplayFrame(myframe[idx]);
    CCAnimation *animation=CCAnimation::createWithSpriteFrames(myArray, dt);
    CCAnimate* animate = CCAnimate::create(animation);
    return animate;
}

void CCCommonUtils::makeEffectSpr(CCSprite* spr, string path, int num, float dt, int idx)
{
    int size = num;
    Vector<SpriteFrame*> myArray;
    CCSpriteFrame *myframe[size];
    for (int i=0; i<size; i++) {
        myframe[i] = CCLoadSprite::loadResource(CCString::createWithFormat(path.c_str(), i+1)->getCString());
        myframe[i]->getTexture()->setAntiAliasTexParameters();
        myArray.pushBack(myframe[i]);
    }
    spr->setDisplayFrame(myframe[idx]);
    CCAnimation *animation=CCAnimation::createWithSpriteFrames(myArray, dt);
    CCAnimate* animate = CCAnimate::create(animation);
   // myArray->release();
    CCRepeatForever* action = CCRepeatForever::create(animate);
    spr->stopAllActions();
    spr->runAction(action);
}

void CCCommonUtils::makeEffectSprWithAP(CCSprite* spr, const Vec2 &anchorPos, string path, int num, float dt, int idx)
{
    int size = num;
    spr->setAnchorPoint(anchorPos);
    spr->setScale(1.0);
    Vector<SpriteFrame*> myArray;
    CCSpriteFrame *myframe[size];
    for (int i=0; i<size; i++) {
        myframe[i] = CCLoadSprite::loadResource(CCString::createWithFormat(path.c_str(), i+1)->getCString());
        myframe[i]->getTexture()->setAntiAliasTexParameters();
        myArray.pushBack(myframe[i]);
    }
    spr->setDisplayFrame(myframe[idx]);
    CCAnimation *animation=CCAnimation::createWithSpriteFrames(myArray, dt);
    CCAnimate* animate = CCAnimate::create(animation);
    CCRepeatForever* action = CCRepeatForever::create(animate);
    spr->stopAllActions();
    spr->runAction(action);
}

void CCCommonUtils::makeInternalEffectSpr(CCSprite* spr, string path, int num, float dt, int idx,float delaytime){
    int size = num;
    spr->setScale(1.0);
    Vector<SpriteFrame*> myArray;
    CCSpriteFrame *myframe[size];
    for (int i=0; i<size; i++) {
        myframe[i] = CCLoadSprite::loadResource(CCString::createWithFormat(path.c_str(), i+1)->getCString());
        myframe[i]->getTexture()->setAntiAliasTexParameters();
        myArray.pushBack(myframe[i]);
    }
    spr->setDisplayFrame(myframe[idx]);
    CCAnimation *animation=CCAnimation::createWithSpriteFrames(myArray, dt);
    CCAnimate* animate = CCAnimate::create(animation);
   // myArray->release();
    CCActionInterval * delayAction = CCDelayTime::create(delaytime);
    CCSequence* sequence = CCSequence::create(delayAction,animate,NULL);
    CCRepeatForever* action = CCRepeatForever::create(sequence);
    spr->stopAllActions();
    spr->runAction(action);

}
void CCCommonUtils::makeOnceEffectSpr(CCSprite* spr, string path, int num, float delate, float dt)
{
    int size = num;
    Vector<SpriteFrame*> myArray;
    CCSpriteFrame *myframe[size];
    for (int i=0; i<size; i++) {
        myframe[i] = CCLoadSprite::loadResource(CCString::createWithFormat(path.c_str(), i+1)->getCString());
        myframe[i]->getTexture()->setAntiAliasTexParameters();
        myArray.pushBack(myframe[i]);
    }
    spr->setDisplayFrame(myframe[0]);
    CCAnimation *animation=CCAnimation::createWithSpriteFrames(myArray, dt);
    CCAnimate* animate = CCAnimate::create(animation);
   // myArray->release();
    CCActionInterval * delaytime = CCDelayTime::create(delate);
    
    auto callback = [=](){
        spr->removeFromParentAndCleanup(true);
    };
    CallFunc* deleteSpr = CallFunc::create(callback);
    CCSequence* sequence = CCSequence::create(delaytime,animate,deleteSpr,NULL);
    spr->stopAllActions();
    spr->runAction(sequence);
}

void CCCommonUtils::makeTwoAnimatSpr(CCSprite* spr, string path, int num, int splitNum, float dt, int idx)
{
    int size = num;
    spr->setAnchorPoint(ccp(0,0));
    spr->setScale(1.0);
    Vector<SpriteFrame*> myArray;
    Vector<SpriteFrame*> myArray1;
    CCSpriteFrame *myframe[size];
    for (int i=0; i<size; i++) {
        myframe[i] = CCLoadSprite::loadResource(CCString::createWithFormat(path.c_str(), i+1)->getCString());
        myframe[i]->getTexture()->setAntiAliasTexParameters();
        if (i<splitNum) {
            myArray.pushBack(myframe[i]);
        }
        else {
            myArray1.pushBack(myframe[i]);
        }
    }
    spr->setDisplayFrame(myframe[idx]);
    CCAnimation *animation=CCAnimation::createWithSpriteFrames(myArray, dt);
    CCAnimate* animate = CCAnimate::create(animation);
   // myArray->release();
    
    CCAnimation *animation1=CCAnimation::createWithSpriteFrames(myArray1, dt);
    CCAnimate* animate1 = CCAnimate::create(animation1);
   // myArray1->release();
    
    CCRepeat* repet = CCRepeat::create(animate, 4+rand()%20);
    CCRepeat* repet1 = CCRepeat::create(animate1, 1);
    CCActionInterval * delaytime = CCDelayTime::create(2+rand()%3);
    
    CCSequence* sequence = CCSequence::create(repet,repet1,delaytime,NULL);
    CCActionInterval * seqForever =CCRepeatForever::create((CCActionInterval* )sequence);
    spr->stopAllActions();
    spr->runAction(seqForever);
}

void CCCommonUtils::getRGBAProtocols(CCNode* parent, vector<CCNode*> * proVec)
{
    auto arr = parent->getChildren();
    if (arr.size()>0) {
        for (auto child : arr)
        {
            CCRGBAProtocol *pRGBAProtocol = dynamic_cast<CCRGBAProtocol*>(child);
            CCNode *pNode = dynamic_cast<CCNode*>(child);
            if (pRGBAProtocol && pNode) {
                proVec->push_back(pNode);
            }
            else {
                if (pNode) {
                    getRGBAProtocols(pNode, proVec);
                }
            }
        }
//        
//        for (int i=0; i<arr->count(); i++) {
//            CCRGBAProtocol *pRGBAProtocol = dynamic_cast<CCRGBAProtocol*>(arr->objectAtIndex(i));
//            CCNode *pNode = dynamic_cast<CCNode*>(arr->objectAtIndex(i));
//            if (pRGBAProtocol && pNode) {
//                proVec->push_back(pNode);
//            }
//            else {
//                if (pNode) {
//                    getRGBAProtocols(pNode, proVec);
//                }
//            }
//        }
    }
}

CCPoint CCCommonUtils::getCrossPoint(vector<CCPoint> &vector, CCPoint &c, CCPoint &d){
    int index = 0;
    while(index < vector.size()){
        CCPoint a = vector[index++];
        CCPoint b = vector[index++];
        
        // 三角形abc 面积的2倍
        float area_abc = (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
        
        // 三角形abd 面积的2倍
        float area_abd = (a.x - d.x) * (b.y - d.y) - (a.y - d.y) * (b.x - d.x);
        
        // 面积符号相同则两点在线段同侧,不相交 (对点在线段上的情况,本例当作不相交处理);
        if ( area_abc*area_abd>=0 ) {
            //                    return false;
            continue;
        }
        
        // 三角形cda 面积的2倍
        float area_cda = (c.x - a.x) * (d.y - a.y) - (c.y - a.y) * (d.x - a.x);
        // 三角形cdb 面积的2倍
        // 注意: 这里有一个小优化.不需要再用公式计算面积,而是通过已知的三个面积加减得出.
        float area_cdb = area_cda + area_abc - area_abd ;
        if (  area_cda * area_cdb >= 0 ) {
            //                    return false;
            continue;
        }
        
        //计算交点坐标
        float t = area_cda / ( area_abd- area_abc );
        float dx= t*(b.x - a.x),
        dy = t*(b.y - a.y);
        return ccp(a.x + dx, a.y + dy);
    }
    return ccp(-1, -1);
};

std::vector<CCPoint>& CCCommonUtils::getCrossPoints(vector<CCPoint> &vector, CCPoint &c, CCPoint &d, int& nSize){
    // zym 大小小心
    static std::vector<CCPoint> m_pts(8, CCPoint());
    int index = 0;
    nSize = 0;
    while(index < vector.size()){
        CCPoint a = vector[index++];
        CCPoint b = vector[index++];
        
        // 三角形abc 面积的2倍
        float area_abc = (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
        
        // 三角形abd 面积的2倍
        float area_abd = (a.x - d.x) * (b.y - d.y) - (a.y - d.y) * (b.x - d.x);
        
        // 面积符号相同则两点在线段同侧,不相交 (对点在线段上的情况,本例当作不相交处理);
        if ( area_abc*area_abd>=0 ) {
            //                    return false;
            continue;
        }
        
        // 三角形cda 面积的2倍
        float area_cda = (c.x - a.x) * (d.y - a.y) - (c.y - a.y) * (d.x - a.x);
        // 三角形cdb 面积的2倍
        // 注意: 这里有一个小优化.不需要再用公式计算面积,而是通过已知的三个面积加减得出.
        float area_cdb = area_cda + area_abc - area_abd ;
        if (  area_cda * area_cdb >= 0 ) {
            //                    return false;
            continue;
        }
        
        //计算交点坐标
        float t = area_cda / ( area_abd- area_abc );
        float dx= t*(b.x - a.x),
        dy = t*(b.y - a.y);
        CCPoint pt = ccp(a.x + dx, a.y + dy);
        //m_pts.push_back(pt);
        m_pts[nSize++] = pt;

      
    }
    return m_pts;
}

std::string CCCommonUtils::changeTimeAddUnit(int secs){
    int day = secs/(24*3600);
    int hour = secs/3600%24;
    int min = secs/60%60;
    std::string dayStr = _lang("105592");
    std::string hourStr = _lang("105591");
    std::string minStr = _lang("105590");
    if(day > 0){
        return CCString::createWithFormat("%d%s",day,dayStr.c_str())->getCString();
    }else if(hour>0){
        if(min>0){
           return CCString::createWithFormat("%d%s:%d%s",hour,hourStr.c_str(),min,minStr.c_str())->getCString();
        }else{
            return CCString::createWithFormat("%d%s",hour,hourStr.c_str())->getCString();
        }
    }
    if(min<=0){
        min = 1;
    }
    return CCString::createWithFormat("%d%s",min,minStr.c_str())->getCString();
}

bool CCCommonUtils::isAdriodPad(){
    auto target = CCApplication::sharedApplication()->getTargetPlatform();
    if (target == kTargetAndroid) {
        auto winSize =Director::getInstance()->getWinSize() ;
        int dpi = Device::getDPI();
        float size = sqrt(pow(winSize.width, 2) + pow(winSize.height, 2)) / dpi;
        if(size >= 7.0f){
            return true;
        }
    }
    return false;
}

bool CCCommonUtils::isAdriodPadHD(){
    auto target = CCApplication::sharedApplication()->getTargetPlatform();
    if (target == kTargetAndroid) {
        auto winSize = Director::getInstance()->getOpenGLView()->getFrameSize();
        int dpi = cocos2d::Device::getDPI();
        float size = sqrt(pow(winSize.width, 2) + pow(winSize.height, 2)) / dpi;
        
//        CCLOG("isAdriodPadHD==%f", size);
        
        if(((size>=7.8 && size<=8.0) || (size>=9.6 && size<=10.0) || (size>=12.8 && size<=13.0))){
            return true;
        }
    }
    return false;
}

bool CCCommonUtils::isIOSPad() {
    
    return kTargetIpad == CCApplication::sharedApplication()->getTargetPlatform();
}

static bool __isUseHD = false;  // 本地开关
static bool __curIsHD = true;

bool CCCommonUtils::isPad()
{
    
    return (__isUseHD && (isIOSPad() || isAdriodPadHD()));
}

bool CCCommonUtils::isIosAndroidPad()
{
    return false;
    
    int __hd_switch =  CCUserDefault::sharedUserDefault()->getIntegerForKey(SERVER_SWITCH_HD_2, 0);  // 服务器开关
//    bool __ccbFinish =  CCUserDefault::sharedUserDefault()->getBoolForKey(DOWNLOAD_HD_CCB, false);  // ccb下载完毕
    bool __isResFinish = CCUserDefault::sharedUserDefault()->getBoolForKey(DOWNLOAD_HD_RES, false); //资源是否下载完毕
    
    bool __isFlag = (__isUseHD && __isResFinish && __hd_switch>0 && (isIOSPad() || isAdriodPadHD()));
    
//    if (__isFlag)
//    {
//        addResSearchPath();
//    }
    
    return __isFlag;
}


void CCCommonUtils::setIsUseHD(bool isHD)
{
    __isUseHD = isHD;
}

void CCCommonUtils::setServerHDSwitch()
{
    int _switch = CCUserDefault::sharedUserDefault()->getIntegerForKey(SERVER_SWITCH_HD, 0);
    string  _filepath = CCFileUtils::sharedFileUtils()->getWritablePath() + "dresource/" + "Common/Common_101_hd.plist";
    if (!CCFileUtils::sharedFileUtils()->isFileExist(_filepath))
    {
        _switch = 0;
    }
    CCUserDefault::sharedUserDefault()->setIntegerForKey(SERVER_SWITCH_HD_2, _switch);
    CCUserDefault::sharedUserDefault()->flush();
}

bool CCCommonUtils::getIsUseHD()
{
    return __isUseHD;
}

void CCCommonUtils::setIsHDViewPort(bool isHD)
{
    __curIsHD = isHD;
    
    if(isHD && isIosAndroidPad()) {
        Director::getInstance()->getOpenGLView()->setDesignResolutionSize(1536, 2048, kResolutionFixedWidth);
    }else{
        Director::getInstance()->getOpenGLView()->setDesignResolutionSize(640, 852, kResolutionFixedWidth);
    }
}

bool CCCommonUtils::getIsHDViewPort()
{

    return __curIsHD;
}

void CCCommonUtils::addResSearchPath()
{
    static bool isFirstAdd = false;
    if (!isFirstAdd)
    {
        isFirstAdd = true;
        std::vector<std::string> _paths;
        std::string m_writeablePath = CCFileUtils::sharedFileUtils()->getWritablePath();
        _paths.push_back(m_writeablePath + "dresource/");
        _paths.push_back(m_writeablePath);
        _paths.push_back("");
        CCFileUtils::sharedFileUtils()->setSearchPaths(_paths);
    }
    
}

void CCCommonUtils::setCurUISkinType()
{
    int lastType = CCUserDefault::sharedUserDefault()->getIntegerForKey(UI_SKIN_TYPE_LAST_TIME, 0);
    if (lastType > 0) {
        bool isHasResource = false;
        long _size1 = 0, _size2 = 0, _size3 = 0;
        string _filepath1, _filepath2, _filepath3;
        string writablePath = FileUtils::getInstance()->getWritablePath();
        _filepath1 = writablePath + "dresource/" + UI_RES;
        _filepath1 = CCString::createWithFormat(_filepath1.c_str(), lastType)->getCString();
        _size1 = FileUtils::getInstance()->getFileSize(_filepath1);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        _filepath2 = writablePath + "dresource/" + "ui_res/_alpha_ui_res_%d.pkm";
        _filepath2 = __String::createWithFormat(_filepath2.c_str(), lastType)->getCString();
        _filepath3 = writablePath + "dresource/" + "ui_res/_alpha_ui_res_%d_alpha.pkm";
        _filepath3 = __String::createWithFormat(_filepath3.c_str(), lastType)->getCString();
#else
        _filepath2 = writablePath + "dresource/" + "ui_res/ui_res_%d.pvr.ccz";
        _filepath2 = __String::createWithFormat(_filepath2.c_str(), lastType)->getCString();
        _filepath3 = _filepath2;
#endif
        _size2 = FileUtils::getInstance()->getFileSize(_filepath2);
        _size3 = FileUtils::getInstance()->getFileSize(_filepath3);
        auto cleanFunc = [=](){
            std::remove(_filepath1.c_str());
            std::remove(_filepath2.c_str());
            std::remove(_filepath3.c_str());
            string _filepath = writablePath + "dresource/" + UI_RES;
            _filepath = __String::createWithFormat(_filepath.c_str(), 1)->getCString();
            std::remove(_filepath.c_str());
        };
        if (_size1 <= 0 || _size2 <= 0 || _size3 <= 0)
        {
            cleanFunc();
        } else {
            isHasResource = true;
        }
        
        
        bool isHasCCBI = false;
        if (CCCommonUtils::isIosAndroidPad())
        {
            string _name = "hdccbi/gameUI_HD_act";
            _name += CC_ITOA(lastType);
            _name += ".ccbi";
            if (FileUtils::getInstance()->isFileExist(_name.c_str())) {
                isHasCCBI = true;
            }
        } else {
            string _name = "ccbi/gameUI_act";
            _name += CC_ITOA(lastType);
            _name += ".ccbi";
            if (FileUtils::getInstance()->isFileExist(_name.c_str()))
            {
                isHasCCBI = true;
            }
        }
        
        if (!isHasResource || !isHasCCBI) {
            lastType = 0;
        }
    }
    CCUserDefault::sharedUserDefault()->setIntegerForKey(UI_SKIN_TYPE, lastType);
    CCUserDefault::sharedUserDefault()->flush();
}

int CCCommonUtils::getUISkinType()
{
    return CCUserDefault::sharedUserDefault()->getIntegerForKey(UI_SKIN_TYPE, 0);
}

bool CCCommonUtils::isUseSpeUI()
{
    int _type = getUISkinType();
    return (_type > 0);
}

void CCCommonUtils::resetData(CCDictionary *params){
    GlobalData::shared()->isInDataParsing = true;
    GlobalData::shared()->m_harvestInfo = "";
    GlobalData::shared()->m_skillFortInfo = "";
    GlobalData::shared()->installFriendsInfo = "";
    GlobalData::shared()->inviteFriendsInfo = "";
    
    PortActController::getInstance()->m_loginDayMap.clear();
    PortActController::getInstance()->m_timeExMap.clear();
    AllianceManager::getInstance()->clearEvents();
    
    TimerController::getInstance();
    CCUserDefault::sharedUserDefault()->setStringForKey(ALLIANCE_MEMBER_BTN_STATUS, "");
    
    Autolock lock(g_platmMutex);
    GlobalData::shared()->playerInfo.setAllianceId("");
    ChatController::getInstance()->m_chatAlliancePool.clear();
    ChatController::getInstance()->m_latestAllianceMsg.sequenceId=0;
    ActivityController::getInstance()->ckfNeedLeave = 0;
    
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::reset();
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    ChatServiceCocos2dx::resetIOS();
#endif
    
    ChatController::getInstance()->isFirstRequestCountry=true;
    ChatController::getInstance()->canCountryRequest=true;
    ChatController::getInstance()->canAllianceRequest=true;
    ChatController::getInstance()->m_commentPool.clear();
    ChatController::getInstance()->m_chatCountryPool.clear();
    ChatController::getInstance()->m_chatInfoSendDic->removeAllObjects();
    MailController::getInstance()->m_mailInfoSendDic->removeAllObjects();
    MailController::getInstance()->m_mailDataDic->removeAllObjects();
    MailController::getInstance()->m_allianceMemberInfoDic->removeAllObjects();
    ChatController::getInstance()->m_userInfoDic->removeAllObjects();
}

void CCCommonUtils::initConfigData(CCDictionary *params){
    //初始化dataconfig
    if(params == nullptr) {
        return;
    }
    CCDictionary* dataConfig = _dict(params->objectForKey("dataConfig"));
    if (dataConfig == nullptr) {
        return;
    }
    
    //initconfig 在 initplayer前面调用，会导致取不到serverid，这里做个特殊处理
    CCDictionary* pUserInfo = _dict(params->objectForKey("user"));
    if(pUserInfo->objectForKey("serverId")){
        GlobalData::shared()->playerInfo.selfServerId = pUserInfo->valueForKey("serverId")->intValue();
        if(!GlobalData::shared()->isPressedHomeKey || GlobalData::shared()->playerInfo.currentServerId == -1)
        {
            GlobalData::shared()->playerInfo.currentServerId = GlobalData::shared()->playerInfo.selfServerId;
        }
    }
    //
    
    CCDictionary* account = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("account"));
    if (account) {
        if(account->objectForKey("k1"))
        {
            GlobalData::shared()->maxGameCnt = account->valueForKey("k1")->intValue();
        }
        if (account->objectForKey("k2")) {
            GlobalData::shared()->cnt_forbidden_swith = account->valueForKey("k2")->intValue();
        }
        if (account->objectForKey("k3")) {
            GlobalData::shared()->cnt_forbidden_level = account->valueForKey("k3")->intValue();
        }
        if (account->objectForKey("k4")) {
            GlobalData::shared()->cnt_forbidden_playingtime = account->valueForKey("k4")->doubleValue();
        }
        if (account->objectForKey("k5")) {
            GlobalData::shared()->trans_res_forbidden_swith = account->valueForKey("k5")->intValue();
        }
        if (account->objectForKey("k6")) {
            GlobalData::shared()->trans_res_limit = account->valueForKey("k6")->longValue();
        }
        GlobalData::shared()->foodMinimumSwith = 0;
        if (account->objectForKey("k7")) {
            GlobalData::shared()->foodMinimumSwith = account->valueForKey("k7")->intValue();
        }
        GlobalData::shared()->robResLevel = 0;
        if (account->objectForKey("k8")) {
            GlobalData::shared()->robResLevel = account->valueForKey("k8")->intValue();
        }
        GlobalData::shared()->robResSwith = 0;
        if (account->objectForKey("k9")) {
            GlobalData::shared()->robResSwith = account->valueForKey("k9")->intValue();
        }
    }

    CCDictionary* scoutTimeDic = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("scoutmark_time"));
    if(scoutTimeDic){
        WorldController::getInstance()->scoutTime = scoutTimeDic->valueForKey("k1")->intValue();
    }
    CCDictionary* NewIconConfigDic = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("icon"));
    if (NewIconConfigDic) {
        if(NewIconConfigDic->objectForKey("k1"))
        {
            GlobalData::shared()->newIcon_version = NewIconConfigDic->valueForKey("k1")->getCString();
        }
        if(NewIconConfigDic->objectForKey("k2"))
        {
            GlobalData::shared()->newIcon_name = NewIconConfigDic->valueForKey("k2")->getCString();
        }
        if(NewIconConfigDic->objectForKey("k3"))
        {
            GlobalData::shared()->newIcon_cost = NewIconConfigDic->valueForKey("k3")->intValue();
        }
        if(NewIconConfigDic->objectForKey("k4"))
        {
            GlobalData::shared()->newIcon_startTime = NewIconConfigDic->valueForKey("k4")->doubleValue() / 1000;
        }
        if(NewIconConfigDic->objectForKey("k5"))
        {
            GlobalData::shared()->newIcon_endTime = NewIconConfigDic->valueForKey("k5")->doubleValue() / 1000;
        }
        if(NewIconConfigDic->objectForKey("k6"))
        {
            GlobalData::shared()->newIcon_country = NewIconConfigDic->valueForKey("k6")->getCString();
        }
    }
    
    ///////小公主对话等级时间的限制talktips_time
    CCDictionary* TalkTipsTime = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("talktips_time"));
    FunBuildController::getInstance()->talkTipsTimes.clear();
    if (TalkTipsTime) {
        std::string tmpStr = "";
        if(TalkTipsTime->objectForKey("k1"))
        {
            tmpStr = "";
            tmpStr = TalkTipsTime->valueForKey("k1")->getCString();
            FunBuildController::getInstance()->talkTipsTimes.push_back(tmpStr);
        }
        if(TalkTipsTime->objectForKey("k2"))
        {
            tmpStr = "";
            tmpStr = TalkTipsTime->valueForKey("k2")->getCString();
            FunBuildController::getInstance()->talkTipsTimes.push_back(tmpStr);
        }
        if(TalkTipsTime->objectForKey("k3"))
        {
            tmpStr = "";
            tmpStr = TalkTipsTime->valueForKey("k3")->getCString();
            FunBuildController::getInstance()->talkTipsTimes.push_back(tmpStr);
        }
        if(TalkTipsTime->objectForKey("k4"))
        {
            tmpStr = "";
            tmpStr = TalkTipsTime->valueForKey("k4")->getCString();
            FunBuildController::getInstance()->talkTipsTimes.push_back(tmpStr);
        }
        if(TalkTipsTime->objectForKey("k5"))
        {
            tmpStr = "";
            tmpStr = TalkTipsTime->valueForKey("k5")->getCString();
            FunBuildController::getInstance()->talkTipsTimes.push_back(tmpStr);
        }
        if(TalkTipsTime->objectForKey("k6"))
        {
            tmpStr = "";
            tmpStr = TalkTipsTime->valueForKey("k6")->getCString();
            FunBuildController::getInstance()->talkTipsTimes.push_back(tmpStr);
        }
        if(TalkTipsTime->objectForKey("k7"))
        {
            tmpStr = "";
            tmpStr = TalkTipsTime->valueForKey("k7")->getCString();
            FunBuildController::getInstance()->talkTipsTimes.push_back(tmpStr);
        }
        if(TalkTipsTime->objectForKey("k8"))
        {
            tmpStr = "";
            tmpStr = TalkTipsTime->valueForKey("k8")->getCString();
            FunBuildController::getInstance()->talkTipsTimes.push_back(tmpStr);
        }
    }
    if(dataConfig->objectForKey("pop_rate")){
        ActivityController::getInstance()->initViewConfig(_dict(dataConfig->objectForKey("pop_rate")));
    }else{
        ActivityController::getInstance()->initViewConfig(NULL);
    }
    if(dataConfig->objectForKey("cross_open")){
        WorldController::getInstance()->isServerCrossOpen = dataConfig->valueForKey("cross_open")->boolValue();
    }else{
        WorldController::getInstance()->isServerCrossOpen = false;
    }
    if (dataConfig->objectForKey("wl_res_tile")) {
        auto wlResTile = _dict(dataConfig->objectForKey("wl_res_tile"));
        WorldController::getInstance()->resTileKey["k1"] = wlResTile->valueForKey("k1")->intValue();
        WorldController::getInstance()->resTileKey["k2"] = wlResTile->valueForKey("k2")->intValue();
        WorldController::getInstance()->resTileKey["k3"] = wlResTile->valueForKey("k3")->intValue();
        WorldController::getInstance()->resTileKey["k4"] = wlResTile->valueForKey("k4")->intValue();
        WorldController::getInstance()->resTileKey["k5"] = wlResTile->valueForKey("k5")->intValue();
        WorldController::getInstance()->resTileKey["k6"] = wlResTile->valueForKey("k6")->intValue();
        WorldController::getInstance()->resTileKey["k7"] = wlResTile->valueForKey("k7")->intValue();
        WorldController::getInstance()->resTileKey["k8"] = wlResTile->valueForKey("k8")->intValue();
    }
    
    auto aAreaConfigDic = _dict(dataConfig->objectForKey("alliance_territory"));
    if (aAreaConfigDic) {
        WorldController::getInstance()->m_bAAreaCost = aAreaConfigDic->valueForKey("k3")->intValue();
        WorldController::getInstance()->m_aTerritory10 = aAreaConfigDic->valueForKey("k10")->intValue();
        WorldController::getInstance()->m_aTerritory11 = aAreaConfigDic->valueForKey("k11")->intValue();
        WorldController::getInstance()->m_aTerritory12 = aAreaConfigDic->valueForKey("k12")->intValue();
    }
    auto aAreaConfigDic1 = _dict(dataConfig->objectForKey("alliance_territory1"));
    WorldController::getInstance()->m_territoryBannerCost = 0;
    GlobalData::shared()->alliance_territory_supermine_limit = 1;
    if (aAreaConfigDic1) {
        WorldController::getInstance()->m_territoryBannerCost = aAreaConfigDic1->valueForKey("k3")->intValue();
        GlobalData::shared()->alliance_territory_supermine_limit = aAreaConfigDic1->valueForKey("k9")->intValue();
        
    }
    
    auto effectDic = _dict(dataConfig->objectForKey("on_event"));
    if (effectDic) {
        WorldController::getInstance()->effectTag = effectDic->valueForKey("k1")->intValue();
        WorldController::getInstance()->monsterCountry = effectDic->valueForKey("k2")->getCString();
        WorldController::getInstance()->monsterSkinId = effectDic->valueForKey("k3")->intValue();
        GlobalData::shared()->xmlReloadFlag = effectDic->valueForKey("k4")->getCString();
        GlobalData::shared()->xmlReloadVersion = effectDic->valueForKey("k5")->getCString();
        if (effectDic->objectForKey("k6"))
        {
            GlobalData::shared()->worldMarchParticleType = effectDic->valueForKey("k6")->intValue();
        }
    }
    auto on_event4 = _dict(dataConfig->objectForKey("on_event4"));
    {
        if (on_event4->objectForKey("k1")) {
            GlobalData::shared()->on_event4_k1 = on_event4->valueForKey("k1")->getCString();
        }
        if (on_event4->objectForKey("k2")) {
            GlobalData::shared()->on_event4_k2 = on_event4->valueForKey("k2")->getCString();
        }
        if (on_event4->objectForKey("k3")) {
            GlobalData::shared()->on_event4_k3 = on_event4->valueForKey("k3")->getCString();
        }
        if (on_event4->objectForKey("k4")) {
            GlobalData::shared()->on_event4_k4 = on_event4->valueForKey("k4")->getCString();
        }
        if (on_event4->objectForKey("k5")) {
            GlobalData::shared()->on_event4_k5 = on_event4->valueForKey("k5")->getCString();
        }
    }
    //联盟领地增减益初始化
    if (params->objectForKey("territory_positive_effect")) {
        GlobalData::shared()->alliance_territory_positive_effect = params->valueForKey("territory_positive_effect")->intValue();
    }
    else
        GlobalData::shared()->alliance_territory_positive_effect = 0;
    
    if (params->objectForKey("territory_negative_effect")) {
        GlobalData::shared()->alliance_territory_negative_effect = params->valueForKey("territory_negative_effect")->intValue();
    }
    else
        GlobalData::shared()->alliance_territory_negative_effect = 0;
    
    if (params->objectForKey("storage")) {
        GlobalData::shared()->territory_warehouse_dayStorage = params->valueForKey("storage")->longValue();
    }
    else
        GlobalData::shared()->territory_warehouse_dayStorage = 0;
    
    if (params->objectForKey("storageTotal")) {
        GlobalData::shared()->territory_warehouse_totalStorage =  params->valueForKey("storageTotal")->longValue();
    }
    else
        GlobalData::shared()->territory_warehouse_totalStorage = 0;
    
    auto allianceCost = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("alliance_cost"));
    if(allianceCost){
        GlobalData::shared()->alliance_cost_k1 = allianceCost->valueForKey("k1")->intValue();
        GlobalData::shared()->alliance_cost_k2 = allianceCost->valueForKey("k2")->intValue();
        GlobalData::shared()->alliance_cost_k3 = allianceCost->valueForKey("k3")->intValue();
        GlobalData::shared()->alliance_cost_k4 = allianceCost->valueForKey("k4")->intValue();
        GlobalData::shared()->alliance_cost_k5 = allianceCost->valueForKey("k5")->intValue();
        GlobalData::shared()->alliance_cost_k6 = allianceCost->valueForKey("k6")->intValue();
        GlobalData::shared()->alliance_cost_k7 = allianceCost->valueForKey("k7")->intValue();
        GlobalData::shared()->alliance_cost_k8 = allianceCost->valueForKey("k8")->intValue();
        GlobalData::shared()->alliance_cost_k11 = allianceCost->valueForKey("k11")->intValue();
        GlobalData::shared()->alliance_cost_k12 = allianceCost->valueForKey("k12")->intValue();
    }
    auto allianceScience = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("alliance_science"));
    if(allianceScience){
        GlobalData::shared()->alliance_science_k1 = allianceScience->valueForKey("k1")->intValue();
        GlobalData::shared()->alliance_science_k2 = allianceScience->valueForKey("k2")->intValue();
        GlobalData::shared()->alliance_science_k3 = allianceScience->valueForKey("k3")->intValue();
        GlobalData::shared()->alliance_science_k4 = allianceScience->valueForKey("k4")->intValue();
        GlobalData::shared()->alliance_science_k5 = allianceScience->valueForKey("k5")->intValue();
        GlobalData::shared()->alliance_science_k6 = allianceScience->valueForKey("k6")->intValue();
        GlobalData::shared()->alliance_science_k7 = allianceScience->valueForKey("k7")->intValue();
        GlobalData::shared()->alliance_science_k8 = allianceScience->valueForKey("k8")->intValue();
        GlobalData::shared()->alliance_science_k9 = allianceScience->valueForKey("k9")->intValue();
        GlobalData::shared()->alliance_science_k10 = allianceScience->valueForKey("k10")->intValue();
        GlobalData::shared()->alliance_science_k11 = allianceScience->valueForKey("k11")->intValue();
        GlobalData::shared()->alliance_science_k12 = allianceScience->valueForKey("k12")->intValue();
    }
    auto alliance_gift = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("alliance_gift"));
    if(alliance_gift){
        GlobalData::shared()->alliance_gift_k2 = alliance_gift->valueForKey("k2")->intValue();
        GlobalData::shared()->alliance_gift_k3 = alliance_gift->valueForKey("k3")->longValue();
    }
    
    auto resourcesWeight = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("resources_weight"));
    if(resourcesWeight){
        GlobalData::shared()->resources_weight_1 = resourcesWeight->valueForKey("k1")->intValue();
        GlobalData::shared()->resources_weight_2 = resourcesWeight->valueForKey("k2")->intValue();
        GlobalData::shared()->resources_weight_3 = resourcesWeight->valueForKey("k3")->intValue();
        GlobalData::shared()->resources_weight_4 = resourcesWeight->valueForKey("k4")->intValue();
        GlobalData::shared()->resources_weight_5 = resourcesWeight->valueForKey("k5")->intValue();
        GlobalData::shared()->resources_weight_6 = resourcesWeight->valueForKey("k6")->intValue();
        GlobalData::shared()->resources_weight_7 = resourcesWeight->valueForKey("k7")->intValue();
    }
    
    auto worldRally = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("world_rally"));
    if(worldRally){
        GlobalData::shared()->world_rally_k1 = worldRally->valueForKey("k1")->intValue();
        GlobalData::shared()->world_rally_k2 = worldRally->valueForKey("k2")->intValue();
        GlobalData::shared()->world_rally_k3 = worldRally->valueForKey("k3")->intValue();
        GlobalData::shared()->world_rally_k4 = worldRally->valueForKey("k4")->intValue();
        GlobalData::shared()->world_rally_k5 = worldRally->valueForKey("k5")->intValue();
        GlobalData::shared()->world_rally_k6 = worldRally->valueForKey("k6")->intValue();
        GlobalData::shared()->world_rally_k7 = worldRally->valueForKey("k7")->intValue();
        GlobalData::shared()->world_rally_k8 = worldRally->valueForKey("k8")->intValue();
    }
    auto worldAllianceWar = _dict(dataConfig->objectForKey("world_alliancewar"));
    if(worldAllianceWar){
        GlobalData::shared()->world_alliance_war_k1 = worldAllianceWar->valueForKey("k1")->intValue();
        GlobalData::shared()->world_alliance_war_k2 = worldAllianceWar->valueForKey("k2")->intValue();
    }
    
    auto inviteCost = _dict(dataConfig->objectForKey("invite_cost"));
    if(inviteCost){
        GlobalData::shared()->invite_cost_k1 = inviteCost->valueForKey("k1")->intValue();
    }
    
    auto fbGift = _dict(dataConfig->objectForKey("facebook_gift"));
    if(fbGift){
        GlobalData::shared()->facebook_gift_k7 = fbGift->valueForKey("k7")->intValue();
    }
    
    auto glodmine_fb = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("goldmine_fb"));
    if(glodmine_fb){
        GlobalData::shared()->glodMine_fb_k1 = glodmine_fb->valueForKey("k1")->intValue();
        GlobalData::shared()->glodMine_fb_k2 = glodmine_fb->valueForKey("k2")->intValue();
        GlobalData::shared()->glodMine_fb_k3 = glodmine_fb->valueForKey("k3")->intValue();
        GlobalData::shared()->glodMine_fb_k4 = glodmine_fb->valueForKey("k4")->intValue();
    }
    
    auto &worldConfig = GlobalData::shared()->worldConfig;
    if(dataConfig->objectForKey("ckf_world_row")){
        worldConfig.ckf_world_row = dataConfig->valueForKey("ckf_world_row")->intValue();
    }
    if(dataConfig->objectForKey("dragon_world_row")){
        worldConfig.dragon_world_row = dataConfig->valueForKey("dragon_world_row")->intValue();
    }
    if(dataConfig->objectForKey("monster_count")){
        worldConfig.stamineMax = dataConfig->valueForKey("monster_count")->intValue();
    }
    if(dataConfig->objectForKey("monster_interval")){
        vector<std::string> vector;
        CCCommonUtils::splitString(dataConfig->valueForKey("monster_interval")->getCString(), ";", vector);
        worldConfig.stamineIntervalTime = atoi(vector[1].c_str());
        worldConfig.stamineIntervalNum = atoi(vector[0].c_str());
    }
    if(dataConfig->objectForKey("monster_decr")){
        worldConfig.stamineCostPerTime = dataConfig->valueForKey("monster_decr")->intValue();
    }
    if(dataConfig->objectForKey("boss_decr")){
        worldConfig.boss_decr = dataConfig->valueForKey("boss_decr")->intValue();
    }
    if(dataConfig->objectForKey("boss_max_member")){
        worldConfig.bossLimitNum = dataConfig->valueForKey("boss_max_member")->intValue();
    }
    
    auto worldMapTime = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("worldmap_time"));
    if(worldMapTime){
        for(int i= 0; i < 4; i++){
            auto key = CCString::createWithFormat("k%d",i+1)->getCString();
            int value = worldMapTime->valueForKey(key)->intValue();
            worldConfig.monsterAttackTime.push_back(value);
        }
        if (worldMapTime->objectForKey("k5"))
        {
            GlobalData::shared()->startDarkTime = worldMapTime->valueForKey("k5")->intValue();
        }
        if (worldMapTime->objectForKey("k6"))
        {
            GlobalData::shared()->completeDarkTime = worldMapTime->valueForKey("k6")->intValue();
        }
        if (worldMapTime->objectForKey("k7"))
        {
            GlobalData::shared()->startLightTime = worldMapTime->valueForKey("k7")->intValue();
        }
        if (worldMapTime->objectForKey("k8"))
        {
            GlobalData::shared()->completeLightTime = worldMapTime->valueForKey("k8")->intValue();
        }
    }
    
    auto worldMapMap = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("worldmap_map"));
    if(worldMapMap){
        for(int i= 0; i < 4; i++){
            auto key = CCString::createWithFormat("k%d",i+1)->getCString();
            int value = worldMapMap->valueForKey(key)->intValue();
            worldConfig.relicConfig.push_back(value);
        }
    }
    
    auto worldDistance = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("world_marchtime"));
    if (worldDistance) {
        for (int i=0; i<6; i++) {
            auto key = CCString::createWithFormat("k%d",i+1)->getCString();
            auto ki = worldDistance->valueForKey(key)->getCString();
            worldConfig.distance[i] = atof(ki);
        }
    }
    auto march = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("march"));
    if(march){
        TroopsController::getInstance()->m_limitHeroNum = march->valueForKey("k1")->intValue();
    }else{
        TroopsController::getInstance()->m_limitHeroNum = 1;
    }
    
    auto worldFire = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("world_fire2"));
    if (worldFire) {
        for (int i=0; i<3; i++) {
            auto key = CCString::createWithFormat("k%d",i+1)->getCString();
            auto ki = worldFire->valueForKey(key)->intValue();
            worldConfig.fire[i] = ki;
        }
    }
    auto wFire1 = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("world_fire1"));
    if (wFire1) {
        for (int i=0; i<4; i++) {
            auto key = CCString::createWithFormat("k%d",i+1)->getCString();
            auto ki = wFire1->valueForKey(key)->intValue();
            worldConfig.fire1[i] = ki;
        }
    }
    
    if(dataConfig->objectForKey("resKey")){
        GlobalData::shared()->worldConfig.reskey = dataConfig->valueForKey("resKey")->intValue();
    }
    
    auto worldPve = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("worldmap_pve"));
    if (worldPve) {
        worldConfig.pveHitMax = worldPve->valueForKey("k1")->intValue();
        worldConfig.pveBuyMax = worldPve->valueForKey("k2")->intValue();
    }
    auto worldPveBuy = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("worldmap_pvenum"));
    if (worldPveBuy) {
        CCDictElement* pEle;
        CCDICT_FOREACH(worldPveBuy, pEle) {
            string key = pEle->getStrKey();
            if(key.size() < 2) {
                continue;
            }
            unsigned int order = atoi(&key[1]);
            CCString *strval  = dynamic_cast<CCString*>(pEle->getObject());
            if (strval) {
                unsigned int val = strval->intValue();
                worldConfig.pveBuyPrice[order] = val;
            }
        }
    }
    
    // 新加的科技判定表，策划控制的是5，6，7，8这几个科技是否打开。1-4科技写死打开。科技对应槽位为0-8
    auto scienceCfg = dynamic_cast<CCDictionary*>(dataConfig->objectForKey("science-test"));
    if (scienceCfg) {
        map<std::string, std::string>& m = ScienceController::getInstance()->mapServerCfg;
        for (int i=1;i<=10;++i) {
            char szKey[32];
            sprintf(szKey, "k%d", i);
            m[szKey] = scienceCfg->valueForKey(szKey)->getCString();
        }
    }
    
    
    GlobalData::shared()->cityUpdateDefenseDelayMin = dataConfig->valueForKey("cityDefInterval")->intValue();
    
    CCDictionary* buildingBase = _dict(dataConfig->objectForKey("building_base"));
    FunBuildController::getInstance()->building_base_k1 = buildingBase->valueForKey("k1")->intValue();
    FunBuildController::getInstance()->building_base_k2 = buildingBase->valueForKey("k2")->intValue();
    FunBuildController::getInstance()->building_base_k3 = buildingBase->valueForKey("k3")->intValue();
    FunBuildController::getInstance()->building_base_k4 = buildingBase->valueForKey("k4")->intValue();
    FunBuildController::getInstance()->building_base_k5 = buildingBase->valueForKey("k5")->intValue();
    FunBuildController::getInstance()->building_base_k6 = buildingBase->valueForKey("k6")->intValue();
    FunBuildController::getInstance()->building_base_k7 = buildingBase->valueForKey("k7")->intValue();
    FunBuildController::getInstance()->building_base_k8 = buildingBase->valueForKey("k8")->intValue();
    FunBuildController::getInstance()->building_base_k9 = buildingBase->valueForKey("k9")->intValue();
    
    GlobalData::shared()->freeSpdT = dataConfig->valueForKey("freeSpdT")->intValue();
    FunBuildController::getInstance()->dataConfig_freeSpdT = GlobalData::shared()->freeSpdT;
    FunBuildController::getInstance()->checkFreeTime();
    CCDictionary* mailConfig = _dict(dataConfig->objectForKey("mail"));
    if(mailConfig){
        GlobalData::shared()->mailConfig.userMailMax = mailConfig->valueForKey("k1")->intValue();
        GlobalData::shared()->mailConfig.sysMailMax = mailConfig->valueForKey("k2")->intValue();
        GlobalData::shared()->mailConfig.saveMailMax = mailConfig->valueForKey("k3")->intValue();
    }
    CCDictionary* generalConfig = _dict(dataConfig->objectForKey("hero"));
    
    GlobalData::shared()->generalConfig.greenNum = generalConfig->valueForKey("k1")->intValue();
    GlobalData::shared()->generalConfig.blueNum = generalConfig->valueForKey("k2")->intValue();
    GlobalData::shared()->generalConfig.purpleNum = generalConfig->valueForKey("k3")->intValue();
    GlobalData::shared()->generalConfig.orangeNum = generalConfig->valueForKey("k4")->intValue();
    GlobalData::shared()->generalConfig.k5 = generalConfig->valueForKey("k5")->floatValue();

    if(dataConfig->objectForKey("hero_yutao")){
        CCDictionary* picConfigDic  = _dict(dataConfig->objectForKey("hero_yutao"));
        GlobalData::shared()->generalConfig.picUploadLv_limit = picConfigDic->valueForKey("k9")->intValue();
        GlobalData::shared()->generalConfig.picModelTime = picConfigDic->valueForKey("k8")->floatValue();
    }
        

    
    CCDictionary* equip_make = _dict(dataConfig->objectForKey("equip"));
    if(equip_make){
        EquipmentController::getInstance()->retInitConf(equip_make);
    }
    CCDictionary* generalConfig1 = _dict(dataConfig->objectForKey("hero_up"));
    GlobalData::shared()->generalConfig.up1 = generalConfig1->valueForKey("k1")->floatValue();
    GlobalData::shared()->generalConfig.up2 = generalConfig1->valueForKey("k2")->floatValue();
    GlobalData::shared()->generalConfig.up3 = generalConfig1->valueForKey("k3")->floatValue();
    GlobalData::shared()->generalConfig.up4 = generalConfig1->valueForKey("k4")->floatValue();
    
    CCDictionary* generalConfig2 = _dict(dataConfig->objectForKey("hero_max"));
    GlobalData::shared()->generalConfig.maxGeneral = generalConfig2->valueForKey("k1")->intValue();
    
    CCDictionary* generalConfig3 = _dict(dataConfig->objectForKey("fight_number"));
    CCDictElement* element1;
    CCDICT_FOREACH(generalConfig3, element1){
        if (dynamic_cast<CCString*>(element1->getObject())) {
            std::string value = dynamic_cast<CCString*>(element1->getObject())->getCString();
            vector<std::string> vector;
            CCCommonUtils::splitString(value, "|", vector);
            if (vector.size() >2) {
                GlobalData::shared()->generalConfig.formationOpen[atoi(vector[0].c_str())] = atoi(vector[1].c_str())
                ;
            }
        }
    }
    CCDictionary* generalConfig4 = _dict(dataConfig->objectForKey("hero_number"));
    
    GlobalData::shared()->generalConfig.bagMaxGeneralNum = generalConfig4->valueForKey("k1")->intValue();
    GlobalData::shared()->generalConfig.bagMaxGoldBuyNum = generalConfig4->valueForKey("k3")->intValue();
    GlobalData::shared()->generalConfig.bagFreeNum = generalConfig4->valueForKey("k2")->intValue();
    std::string goldStr = generalConfig4->valueForKey("k4")->getCString();
    vector<std::string> goldVector;
    CCCommonUtils::splitString(goldStr, "|", goldVector);
    int i = 0;
    while (i < goldVector.size()) {
        GlobalData::shared()->generalConfig.posPrice[i + 1] = atoi(goldVector[i].c_str());
        i++;
    }
    
    auto soulDict = _dict(dataConfig->objectForKey("hero_soulstone"));
    GlobalData::shared()->generalConfig.soul0 = soulDict->valueForKey("k1")->intValue();
    GlobalData::shared()->generalConfig.soul1 = soulDict->valueForKey("k2")->intValue();
    GlobalData::shared()->generalConfig.soul2 = soulDict->valueForKey("k3")->intValue();
    GlobalData::shared()->generalConfig.soul3 = soulDict->valueForKey("k4")->intValue();
    
    CCDictionary* flushHero = _dict(dataConfig->objectForKey("flush_hero"));
    GlobalData::shared()->generalConfig.freePerDay = flushHero->valueForKey("k3")->intValue();
    GlobalData::shared()->generalConfig.oneTimeCost = flushHero->valueForKey("k1")->intValue();
    GlobalData::shared()->generalConfig.tenTimeCost = flushHero->valueForKey("k2")->intValue();
    CCDictionary* cd_cost = _dict(dataConfig->objectForKey("cd_cost"));
    CCDictionary* cd_time = _dict(dataConfig->objectForKey("cd_time"));
    for (int i=0; ; i++) {
        char buffer[32] = {0};
        sprintf(buffer, "k%d",i+1);
        if (cd_time->objectForKey(buffer) && cd_cost->objectForKey(buffer)) {
            GlobalData::shared()->cd_time.push_back(cd_time->valueForKey(buffer)->intValue());
            GlobalData::shared()->cd_cost.push_back(cd_cost->valueForKey(buffer)->intValue());
        }
        else {
            break;
        }
    }
    sort(GlobalData::shared()->cd_time.begin(), GlobalData::shared()->cd_time.end());
    sort(GlobalData::shared()->cd_cost.begin(), GlobalData::shared()->cd_cost.end());
    
    CCDictionary* cd_gold = _dict(dataConfig->objectForKey("cd_gold"));
    for (int i=0; ; i++) {
        char buffer[32] = {0};
        sprintf(buffer, "k%d",i+1);
        if (cd_gold->objectForKey(buffer)) {
            GlobalData::shared()->cd_gold[buffer] = cd_gold->valueForKey(buffer)->floatValue();
        }
        else {
            break;
        }
    }
    
    CCDictionary* resource_cost = _dict(dataConfig->objectForKey("resource_cost"));
    CCDictionary* resource_num1 = _dict(dataConfig->objectForKey("resource_num1"));
    CCDictionary* resource_num2 = _dict(dataConfig->objectForKey("resource_num2"));
    CCDictionary* resource_num3 = _dict(dataConfig->objectForKey("resource_num3"));
    CCDictElement* ele;
    CCDICT_FOREACH(resource_cost, ele)
    {
        string key = ele->getStrKey();
        GlobalData::shared()->res_cost[key] = resource_cost->valueForKey(key)->floatValue();
    }
    CCDICT_FOREACH(resource_num1, ele)
    {
        string key = ele->getStrKey();
        GlobalData::shared()->res1_num[key] = resource_num1->valueForKey(key)->floatValue();
    }
    CCDICT_FOREACH(resource_num2, ele)
    {
        string key = ele->getStrKey();
        GlobalData::shared()->res2_num[key] = resource_num2->valueForKey(key)->floatValue();
    }
    CCDICT_FOREACH(resource_num3, ele)
    {
        string key = ele->getStrKey();
        GlobalData::shared()->res3_num[key] = resource_num3->valueForKey(key)->floatValue();
    }
    CCDictionary* abilityCost = _dict(dataConfig->objectForKey("ability_cost"));
    if(abilityCost){
        GlobalData::shared()->generalConfig.abilityChangeGold = abilityCost->valueForKey("k1")->intValue();
        GlobalData::shared()->generalConfig.skillResetGold = abilityCost->valueForKey("k2")->intValue();
        GlobalData::shared()->generalConfig.abilityResetGold = abilityCost->valueForKey("k3")->intValue();
    }
    
    CCDictionary* physical = _dict(dataConfig->objectForKey("physical"));
    GlobalData::shared()->lordInfo.physical_k1 = physical->valueForKey("k1")->intValue();
    GlobalData::shared()->lordInfo.physical_k2 = physical->valueForKey("k2")->intValue();
    GlobalData::shared()->lordInfo.physical_k3 = physical->valueForKey("k3")->intValue();
    GlobalData::shared()->lordInfo.physical_k4 = physical->valueForKey("k4")->intValue();
    CCDictionary* physical_bug = _dict(dataConfig->objectForKey("physical_bug"));
    CCDictElement* phy_buy;
    CCDICT_FOREACH(physical_bug, phy_buy){
        string key = phy_buy->getStrKey();
        CCString * str = dynamic_cast<CCString*>(phy_buy->getObject());
        if (str) {
            int value = dynamic_cast<CCString*>(phy_buy->getObject())->intValue();
            GlobalData::shared()->lordInfo.physical_bug[key] = value;
        }
    }
    
    auto &userLvAboutInfo = GlobalData::shared()->userLvAboutInfo;
    
    userLvAboutInfo.cd_cost_k1 = cd_cost->valueForKey("k1")->floatValue();
    userLvAboutInfo.cd_cost_k2 = cd_cost->valueForKey("k2")->floatValue();
    userLvAboutInfo.cd_cost_k3 = cd_cost->valueForKey("k3")->floatValue();
    userLvAboutInfo.cd_cost_k4 = cd_cost->valueForKey("k4")->floatValue();
    
    CCDictionary* ranking = _dict(dataConfig->objectForKey("ranking"));
    if(ranking!=NULL){
        
        userLvAboutInfo.ranking_k3 = ranking->valueForKey("k3")->intValue();
    }
    
    CCDictionary* essence = _dict(dataConfig->objectForKey("essence"));
    if(essence!=NULL){
        GlobalData::shared()->essenceK1 = essence->valueForKey("k1")->intValue();
        GlobalData::shared()->essenceK2 = essence->valueForKey("k2")->intValue();
        GlobalData::shared()->essenceK3 = essence->valueForKey("k3")->intValue();
    }
    
    GlobalData::shared()->quickEndLevel = dataConfig->valueForKey("quickEndBattle")->intValue();//k5
    GlobalData::shared()->recruitedGeneralCount = params->valueForKey("recruitedGeneralCount")->intValue();
    
    CCDictionary* serverConfig = _dict(dataConfig->objectForKey("serverConfig"));
    if (serverConfig) {
        ChatController::getInstance()->chat_interval = serverConfig->valueForKey("chat_interval")->intValue();
        GlobalData::shared()->lordInfo.feed_max = serverConfig->valueForKey("feed_maxCount")->intValue();
    }
    
    CCDictionary* activityconfig = _dict(dataConfig->objectForKey("activityconfig"));
    if (activityconfig) {
        ActivityController::getInstance()->parseActConfig(activityconfig);
    }
    CCDictionary* activityconfig2 = _dict(dataConfig->objectForKey("activityconfig2"));
    if (activityconfig2) {
        ActivityController::getInstance()->parseActConfig2(activityconfig2);
        if (activityconfig2->objectForKey("k12"))
        {
            int t = activityconfig2->valueForKey("k12")->intValue();
            CCUserDefault::sharedUserDefault()->setIntegerForKey(UI_SKIN_TYPE_LAST_TIME, t);
            CCUserDefault::sharedUserDefault()->flush();
        } else {
            CCUserDefault::sharedUserDefault()->setIntegerForKey(UI_SKIN_TYPE_LAST_TIME, 0);
            CCUserDefault::sharedUserDefault()->flush();
        }
    }
    CCDictionary *heroAbility = _dict(dataConfig->objectForKey("hero_ability"));
    if(heroAbility){
        GlobalData::shared()->generalConfig.generalMaxAbility[0] = heroAbility->valueForKey("k1")->intValue();
        GlobalData::shared()->generalConfig.generalMaxAbility[1] = heroAbility->valueForKey("k2")->intValue();
        GlobalData::shared()->generalConfig.generalMaxAbility[2] = heroAbility->valueForKey("k3")->intValue();
        GlobalData::shared()->generalConfig.generalMaxAbility[3] = heroAbility->valueForKey("k4")->intValue();
        GlobalData::shared()->generalConfig.generalMaxAbility[4] = heroAbility->valueForKey("k5")->intValue();
        GlobalData::shared()->generalConfig.generalMaxAbility[5] = heroAbility->valueForKey("k6")->intValue();
    }
    
    CCDictionary *abilityLv = _dict(dataConfig->objectForKey("ability_lv"));
    if(abilityLv){
        int i = 1;
        while(abilityLv->objectForKey(std::string("k") + CC_ITOA(i))){
            int level = abilityLv->valueForKey(std::string("k") + CC_ITOA(i))->intValue();
            GlobalData::shared()->generalConfig.abilityOpenedLevel[level] = i;
            i++;
        }
    }
    
    CCDictionary *heroSkillPoint = _dict(dataConfig->objectForKey("hero_skillpoint"));
    if(heroSkillPoint){
        std::string l = heroSkillPoint->valueForKey("k1")->getCString();
        GlobalData::shared()->generalConfig.skillPointAddPerLevel[0] = heroSkillPoint->valueForKey("k1")->intValue();
        GlobalData::shared()->generalConfig.skillPointAddPerLevel[1] = heroSkillPoint->valueForKey("k2")->intValue();
        GlobalData::shared()->generalConfig.skillPointAddPerLevel[2] = heroSkillPoint->valueForKey("k3")->intValue();
        GlobalData::shared()->generalConfig.skillPointAddPerLevel[3] = heroSkillPoint->valueForKey("k4")->intValue();
        GlobalData::shared()->generalConfig.skillPointAddPerLevel[4] = heroSkillPoint->valueForKey("k5")->intValue();
        GlobalData::shared()->generalConfig.skillPointAddPerLevel[5] = heroSkillPoint->valueForKey("k6")->intValue();
    }
    
    CCDictionary* timeDiv = _dict(dataConfig->objectForKey("online_reward"));
    if (timeDiv) {
        PortActController::getInstance()->parseTimeDiv(timeDiv);
    }
    
    GlobalData::shared()->wonder_k9 = 0;
    if (dataConfig->objectForKey("wonder")) {
        CCDictionary* wonder = _dict(dataConfig->objectForKey("wonder"));
        GlobalData::shared()->wonder_k9 = wonder->valueForKey("k9")->intValue();
    }
    
    CCDictionary* cargo = _dict(params->objectForKey("cargo"));
    if (cargo) {
        if (cargo->objectForKey("rewardTime")) {
            PortActController::getInstance()->m_nextRewardTime = cargo->valueForKey("rewardTime")->doubleValue() / 1000;
        }
        if (cargo->objectForKey("rewardInfo")) {
            PortActController::getInstance()->m_rewardInfo = cargo->valueForKey("rewardInfo")->getCString();
        }
    }
    CCDictionary* flushAttr = _dict(params->objectForKey("flushAttr"));
    if(flushAttr!=NULL){
        GlobalData::shared()->flushFreeTimes = flushAttr->valueForKey("freeTimes")->intValue();
        GlobalData::shared()->flushGoldTimes = flushAttr->valueForKey("goldTimes")->intValue();
        GlobalData::shared()->flushCurrentTime = flushAttr->valueForKey("time")->intValue();
    }
    
    GlobalData::shared()->MaxPlayerLv = 99999;
    GlobalData::shared()->MaxHeroLv = 99999;
    GlobalData::shared()->MaxBuildLv = 99999;
    CCDictionary* player_max = _dict(dataConfig->objectForKey("player_max"));
    if(player_max){
        GlobalData::shared()->MaxPlayerLv = player_max->valueForKey("k1")->intValue();
        GlobalData::shared()->MaxHeroLv = player_max->valueForKey("k2")->intValue();
        GlobalData::shared()->MaxBuildLv = player_max->valueForKey("k3")->intValue();
        GlobalData::shared()->starnum_k6 = player_max->valueForKey("k6")->intValue();
//        GlobalData::shared()->starnum_k6 = 10;
    }
    
    if (dataConfig->objectForKey("shield_base_2")) {
        GlobalData::shared()->shield_base = dataConfig->valueForKey("shield_base_2")->intValue();
    }
    
    if (dataConfig->objectForKey("showstarskill")) {
        CCDictionary* skilldict = _dict(dataConfig->objectForKey("showstarskill"));
        GlobalData::shared()->soldierskillStar = skilldict->valueForKey("k1")->getCString();
    }
    
    if (dataConfig->objectForKey("dragonglass")) {
        CCDictionary* dragonglass = _dict(dataConfig->objectForKey("dragonglass"));
        EquipmentController::getInstance()->m_refineFreeTimes = dragonglass->valueForKey("k2")->intValue();
        EquipmentController::getInstance()->m_longSellLv = dragonglass->valueForKey("k1")->intValue();
        EquipmentController::getInstance()->m_meltKey = dragonglass->valueForKey("k3")->getCString();
        EquipmentController::getInstance()->m_equipComposeKey = dragonglass->valueForKey("k4")->getCString();
    }
    
    if (dataConfig->objectForKey("chat"))
    {
        CCDictionary* chat = _dict(dataConfig->objectForKey("chat"));
        if(chat)
        {
            if (chat->objectForKey("k10")) {
                CCLOGFUNCF("chat k10: %s",chat->valueForKey("k10")->getCString());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                ChatServiceCocos2dx::postSwitch("chat_k10", chat->valueForKey("k10")->getCString());
#endif
            }
            if (chat->objectForKey("k11")) {
                CCLOGFUNCF("chat k11: %s",chat->valueForKey("k11")->getCString());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

                ChatServiceCocos2dx::postSwitch("chat_k11", chat->valueForKey("k11")->getCString());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                //zyt   聊天等级限制开关
                
                string k11 = chat->valueForKey("k11")->getCString();
                vector<string>strVec;
                CCCommonUtils::splitString(k11, "|", strVec);
                if (strVec.size()>=1) {
                    int lever = atoi(strVec.at(0).c_str());
                    ChatServiceCocos2dx::CS_Country_SpeakingLevelLimit = lever;
 
                }

               
#endif
            }
        }
    }
    
    if (dataConfig->objectForKey("chat_switch"))
    {
        CCDictionary* chat = _dict(dataConfig->objectForKey("chat_switch"));
        if(chat)
        {
            if (chat->objectForKey("k1")) {
                CCLOGFUNCF("chat_switch K1: %d",chat->valueForKey("k1")->intValue());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//                ChatServiceCocos2dx::m_autoTranslateWithGoogle=2;
                ChatServiceCocos2dx::m_autoTranslateWithGoogle=chat->valueForKey("k1")->intValue();
#endif
            }
/** zyt */
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            
            if (chat-> objectForKey("k2")) {
                //谷歌HTTP翻译
                ChatServiceCocos2dx::m_autoTranslateWithGoogle=chat->valueForKey("k2")->intValue();

            }
            if (chat-> objectForKey("k4")) {
                //iOS端 多人聊天管理
                ChatServiceCocos2dx::enableChatRoom =chat->valueForKey("k4")->intValue();
            }
            if (chat-> objectForKey("k7")) {
                //iOS端 数据库
                string db_k7 = chat->valueForKey("k7")->getCString();
                vector<string>strVec;
                CCCommonUtils::splitString(db_k7, "|", strVec);
                if (strVec.size()>=1) {
                    
                    bool isOpen =  ChatServiceCocos2dx::compareVersionWithKeyVersionString(strVec[0]);
                    
                    ChatServiceCocos2dx::DB_UserInfo_switch = isOpen;
                    
                }
                if (strVec.size()>=2)
                {
                    bool isOpen =  ChatServiceCocos2dx::compareVersionWithKeyVersionString(strVec[1]);
                    ChatServiceCocos2dx::DB_ChatChannel_switch = isOpen;
                    
                    
                }
                if (strVec.size()>=3){
                    bool isOpen =  ChatServiceCocos2dx::compareVersionWithKeyVersionString(strVec[2]);
                    ChatServiceCocos2dx::DB_MsgItem_switch = isOpen;
                    
                }
                if (strVec.size()>= 4){
                    bool isOpen =  ChatServiceCocos2dx::compareVersionWithKeyVersionString(strVec[3]);
                    ChatServiceCocos2dx::DB_SystemMail_switch = isOpen;
                }
            }
            
            //  ！！！k9废弃
//            if (chat-> objectForKey("k9")) {
//                
//                string db_k9 = chat->valueForKey("k9")->getCString();
//                vector<string>strVec;
//                CCCommonUtils::splitString(db_k9, "|", strVec);
//                if (strVec.size()>=1) {
//                 原生邮件开关
//                    bool isOpen =  ChatServiceCocos2dx::compareVersionWithKeyVersionString(strVec[0]);
//                    
//                    ChatServiceCocos2dx::Mail_OC_Native_Enable = isOpen;
//                    
//                }
//                if (strVec.size()>=2)
//                {
//                    原生邮件新UI开关
//                    bool isOpen =  ChatServiceCocos2dx::compareVersionWithKeyVersionString(strVec[1]);
//                    ChatServiceCocos2dx::Mail_OC_UI_New_Native_Enable = isOpen;
//                    
//                    
//                }
//            }
           
            
#endif
            if (chat->objectForKey("k3")) {
                CCLOGFUNCF("chat_switch K3: %s",chat->valueForKey("k3")->getCString());
                string k3=chat->valueForKey("k3")->getCString();
                std::vector<string> strVector;
                splitString(k3, "|", strVector);
                if(strVector.size()>0)
                {
                    CCLOGFUNCF("android switch:%s",strVector[0].c_str());
                    if (strVector[0]=="0") {
                        MailController::getInstance()->setIsNewMailListEnable(false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                        ChatServiceCocos2dx::setIsNewMailListEnable(false);
#endif
                    }
                    else if (strVector[0]=="1") {
                        MailController::getInstance()->setIsNewMailListEnable(true);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                        ChatServiceCocos2dx::setIsNewMailListEnable(true);
#endif
                    }
                }

            }
            if (chat->objectForKey("k6")) {
                CCLOGFUNCF("chat_switch k6: %s",chat->valueForKey("k6")->getCString());
                string k6=chat->valueForKey("k6")->getCString();
                std::vector<string> strVector;
                splitString(k6, "|", strVector);
                if(strVector.size()>0)
                {
                    bool isOn = false;
                    if (strVector[0]==""||strVector[0]=="0") {
                        isOn=false;
                    }else if (strVector[0]=="1"){
                        isOn=true;
                    }else {
                        if (checkVersion(strVector[0])){
                            isOn=true;
                        }else{
                            isOn=false;
                        }
                    }
                    CCLOGFUNCF("mailUI android switch:%s isOn :%d",strVector[0].c_str(),isOn);
                    if (!isOn) {
                        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                        ChatServiceCocos2dx::setMailNewUIEnable(false);
                        MailController::getInstance()->setIsNewMailUIEnable(false);
#endif
                    }
                    else {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                        ChatServiceCocos2dx::setMailNewUIEnable(true);
                        MailController::getInstance()->setIsNewMailUIEnable(true);
#endif
                    }
                }
                if  (strVector.size() >=2){
                    //iOS
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                    //zyt OC_邮件列表
                     bool isOpen =  ChatServiceCocos2dx::compareVersionWithKeyVersionString(strVector[1]);
                     ChatServiceCocos2dx::Mail_OC_Native_Enable = isOpen;
                    CCLog("%d",isOpen);
#endif
                }
            }
            
            if (chat->objectForKey("k8")) {
                CCLOGFUNCF("chat_switch K8: %d",chat->valueForKey("k8")->intValue());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                ChatServiceCocos2dx::setMailSortType(chat->valueForKey("k8")->intValue());
#endif
            }
            
            if (chat->objectForKey("k10")) {
                int k10 = chat->valueForKey("k10")->intValue();
                CCLOGFUNCF("chat_switch K10: %d",k10);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                if (k10 == 0) {
                    ChatServiceCocos2dx::setDefaultTranslateEnable(false);
                }
                else if (k10 == 1)
                {
                    ChatServiceCocos2dx::setDefaultTranslateEnable(true);
                }
                
#endif
            }
 
            
            if (chat->objectForKey("k11")) {
                string banTime = chat->valueForKey("k11")->getCString();
                CCLOGFUNCF("chat_switch k11: %s",banTime.c_str());
                ChatController::getInstance()->setBanTimeSet(banTime);
			#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                ChatServiceCocos2dx::postBanTime(banTime);
           #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                ChatServiceCocos2dx::settingDisableSendMsgWithServiewListKey(banTime);
                
            #endif
            }
 
        }
    }
    
    if(dataConfig->objectForKey("chat_standalone")){
        CCDictionary* chat = _dict(dataConfig->objectForKey("chat_standalone"));
        if(chat)
        {
            CCLOGFUNCF("chat_standalone selfServerId: %d", GlobalData::shared()->playerInfo.selfServerId);
            
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
            bool enableWSVersion = false;
            if (chat->objectForKey("k9")) {
                string enableWSVersionStr = chat->valueForKey("k9")->getCString();
                CCLOGFUNCF("chat_standalone k9: %s",enableWSVersionStr.c_str());
                enableWSVersion = CCCommonUtils::checkVersion(enableWSVersionStr);
            }
            
            if (chat->objectForKey("k4")) {
                string enableWSServer = chat->valueForKey("k4")->getCString();
                CCLOGFUNCF("chat_standalone k4: %s",enableWSServer.c_str());
                if(GlobalData::shared()->playerInfo.selfServerId == -1){
                    ChatServiceCocos2dx::setStandaloneServerEnable(1, enableWSVersion);
                }else{
                    ChatServiceCocos2dx::setStandaloneServerEnable(1, enableWSVersion && CCCommonUtils::checkServer(enableWSServer));
                }
            }
            
            if (chat->objectForKey("k2")) {
                string enableRecieveServer = chat->valueForKey("k2")->getCString();
                CCLOGFUNCF("chat_standalone k2: %s",enableRecieveServer.c_str());
                // 内网的服号为0，checkServer函数对它无效，总是返回false
                // beta的服号为0、2、3，可以配置为0-3
                if(GlobalData::shared()->playerInfo.selfServerId == -1){
                    ChatServiceCocos2dx::setStandaloneServerEnable(2, true);
                }else{
                    ChatServiceCocos2dx::setStandaloneServerEnable(2, CCCommonUtils::checkServer(enableRecieveServer));
                }
            }
            if (chat->objectForKey("k3")) {
                string enableSendServer = chat->valueForKey("k3")->getCString();
                CCLOGFUNCF("chat_standalone k3: %s",enableSendServer.c_str());
                if(GlobalData::shared()->playerInfo.selfServerId == -1){
                    ChatServiceCocos2dx::setStandaloneServerEnable(3, false);
                }else{
                    ChatServiceCocos2dx::setStandaloneServerEnable(3, CCCommonUtils::checkServer(enableSendServer));
                }
            }
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
            //zyt
            if (chat->objectForKey("k5")) {
                string serverVersion = chat->valueForKey("k5")->getCString();
                 CCLOGFUNCF("chat_standalone k5: %s",serverVersion.c_str());
                bool isOpen =  ChatServiceCocos2dx::compareVersionWithKeyVersionString(serverVersion);
                ChatServiceCocos2dx::CS_WebSocket_Opened = false; //isOpen;
            }
            if (chat->objectForKey("k6")) {
                string enableSendServer = chat->valueForKey("k6")->getCString();
                CCLOGFUNCF("chat_standalone k6: %s",enableSendServer.c_str());
                //独立服务器接收消息开关
                ChatServiceCocos2dx::CS_WebSocket_Msg_Get = false; //CCCommonUtils::checkServer(enableSendServer);
   
            }
            if (chat->objectForKey("k7")) {
                string enableSendServer = chat->valueForKey("k7")->getCString();
                CCLOGFUNCF("chat_standalone k7: %s",enableSendServer.c_str());
                //独立服务器发送消息开关
                ChatServiceCocos2dx::CS_WebSocket_Msg_Send = false; //CCCommonUtils::checkServer(enableSendServer);
            }
#endif
        }
    }
    
    if(dataConfig->objectForKey("scoutmark_on")){
        string version = dataConfig->valueForKey("scoutmark_on")->getCString();
        CCLOGFUNCF("scoutmark_on: %s",version.c_str());
        if (CCCommonUtils::checkVersion(version)) {
            WorldController::getInstance()->isDetectFlag = 1;
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
            ChatServiceCocos2dx::setDetectInfoEnable(true);
#endif
        }
        else{
            WorldController::getInstance()->isDetectFlag = 0;
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
            ChatServiceCocos2dx::setDetectInfoEnable(false);
#endif
        }
    }
    else{
        WorldController::getInstance()->isDetectFlag = 0;
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
        ChatServiceCocos2dx::setDetectInfoEnable(false);
#endif
    }
    
    if (dataConfig->objectForKey("function_on")) {
        CCDictionary* function_on = _dict(dataConfig->objectForKey("function_on"));
        CCDictionary* function_on3 = _dict(dataConfig->objectForKey("function_on3"));
        GlobalData::shared()->android_native_chat = function_on->valueForKey("k2")->intValue();
        LotteryController::shared()->setOpenFlag(function_on->valueForKey("k3")->intValue());
        PortActController::getInstance()->m_isNewTimeRwd = function_on->valueForKey("k4")->boolValue();
        GlobalData::shared()->isOpenForum = function_on->valueForKey("k5")->boolValue();
        LotteryController::shared()->setOpenFlag2(function_on->valueForKey("k6")->intValue());
        LotteryController::shared()->setOpenFlag2_ko(function_on->valueForKey("k8")->intValue());
        AchievementController::getInstance()->setOpenFlag(function_on->valueForKey("k9")->intValue());
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
        if(function_on->objectForKey("k10")){
            GlobalData::shared()->isInviteOpen = function_on->valueForKey("k10")->boolValue();
        }else{
            GlobalData::shared()->isInviteOpen = false;
        }
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(function_on->objectForKey("k11")){
            GlobalData::shared()->isInviteOpen = function_on->valueForKey("k11")->boolValue();
        }else{
            GlobalData::shared()->isInviteOpen = false;
        }
#endif
        
        CCLOG("GlobalData::shared()->android_native_chat %d,%d",GlobalData::shared()->android_native_chat,LotteryController::shared()->getOpenFlag());
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ChatServiceCocos2dx::enableNativeChat = function_on->valueForKey("k2")->intValue();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        //原生聊天开关
        
/** zyt */
        
        int  keyValue =function_on3->valueForKey("k8")->intValue();
        if (keyValue == 0){
            ChatServiceCocos2dx::enableNativeChat =false;
        }else if (keyValue == 1){
            ChatServiceCocos2dx::enableNativeChat = true;
        }else if (keyValue == 2){
            //判断是否为4S，为4S，就关掉
            if (CCDirector::sharedDirector()->getWinSize().height<= 960) {
                ChatServiceCocos2dx::enableNativeChat = false;
            }else{
                ChatServiceCocos2dx::enableNativeChat = true;
            }
        }
        
 
#endif
    }else{
        GlobalData::shared()->android_native_chat = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ChatServiceCocos2dx::enableNativeChat = false;
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ChatServiceCocos2dx::enableNativeChat = false;
#endif
    }
    
    if (dataConfig->objectForKey("function_on2")) {
        CCLOG("dataConfig->objectForKey(function_on2");
        CCDictionary* function_on2 = _dict(dataConfig->objectForKey("function_on2"));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(function_on2->objectForKey("k6"))
        {
            CCLOG("function_on2->objectForKey k6");
            ChatServiceCocos2dx::enableNativeMail = function_on2->valueForKey("k6")->intValue();
            CCLOG("ChatServiceCocos2dx::enableNativeMail %d",(int)(ChatServiceCocos2dx::enableNativeMail));
        }
        
#endif
        if (function_on2->objectForKey("k10"))
        {
            UploadImageController::shared()->setUploadImageFlag(function_on2->valueForKey("k10")->intValue());
        }
        
    }
    else{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ChatServiceCocos2dx::enableNativeMail =false;
#endif
    }
    
    // chat notice
    if(dataConfig->objectForKey("function_on2")){
        CCDictionary* function_on2 = _dict(dataConfig->objectForKey("function_on2"));
        if(function_on2){
            bool isOpen = function_on2->valueForKey("k4")->boolValue();
            ChatController::getInstance()->setIsNoticeOpen(isOpen);
        }
    }
    GlobalData::shared()->isOpenTaiwanFlag = false;
    if (dataConfig->objectForKey("function_on3"))
    {
        CCDictionary* function_on3 = _dict(dataConfig->objectForKey("function_on3"));
        GlobalData::shared()->alliance_fun_on3_k5 =function_on3->valueForKey("k5")->intValue();
        if(function_on3->objectForKey("k9")){
            std::string connectHKServer = function_on3->valueForKey("k9")->getCString();
            CCUserDefault::sharedUserDefault()->setStringForKey("connectHKServer", connectHKServer);
        }else
        {
            CCUserDefault::sharedUserDefault()->setStringForKey("connectHKServer", "");
        }

        if (function_on3->objectForKey("k3"))
        {
            DailyActiveController::shared()->setDailyActiveOpenFlag(function_on3->valueForKey("k3")->intValue());
        }
        if (function_on3->objectForKey("k4")) {
            GlobalData::shared()->isOpenTaiwanFlag = function_on3->valueForKey("k4")->boolValue();
        }
        if(function_on3->objectForKey("k5")){
            int flag=function_on3->valueForKey("k5")->intValue();
            CCLOGFUNCF("function_on3 K5:%d",flag);
            if(flag==1)
                MailController::getInstance()->setChatRoomEnable(true);
            else
                MailController::getInstance()->setChatRoomEnable(false);
        }
        if (function_on3->objectForKey("k6")) {
//            GlobalData::shared()->isOpenTaiwanFlag = function_on3->valueForKey("k4")->boolValue();
            int flag=function_on3->valueForKey("k6")->intValue();
            CCLOGFUNCF("function_on3 K6:%d",flag);
        }
        //联盟领地超级矿开门初始化
        if (function_on3->objectForKey("k7")) {
            GlobalData::shared()->alliance_territory_tower_switch = function_on3->valueForKey("k7")->intValue();
        }
        if (!function_on3->objectForKey("k7")) {
            GlobalData::shared()->alliance_territory_tower_switch = 0;
        }
        //GlobalData::shared()->alliance_territory_tower_switch = 0;
        
        //周年庆开关1开0关
        GlobalData::shared()->isOpenAnniversary = false;
        if (function_on3->objectForKey("k10")) {
            GlobalData::shared()->isOpenAnniversary = function_on3->valueForKey("k10")->boolValue();
        }
        
        GlobalData::shared()->march_formation_switch = 0;
        if (function_on3->objectForKey("k11")) {
            GlobalData::shared()->march_formation_switch = function_on3->valueForKey("k11")->intValue();
        }
        //周年庆分享照片开关1开0关
        GlobalData::shared()->isAnniSharePhoto = false;
        if(function_on3->objectForKey("k12")){
            GlobalData::shared()->isAnniSharePhoto = function_on3->valueForKey("k12")->boolValue();
        }
    }
    if(dataConfig->objectForKey("function_on4")){
        CCDictionary* function_on4 = _dict(dataConfig->objectForKey("function_on4"));
        //Branch开关
        GlobalData::shared()->isBranchOn = false;
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(function_on4->objectForKey("k1")){
            GlobalData::shared()->isBranchOn = function_on4->valueForKey("k1")->boolValue();
        }
        #endif
        if(function_on4->objectForKey("k3")){
            GlobalData::shared()->anniSharePhoto = function_on4->valueForKey("k3")->intValue();
        }
        if(function_on4->objectForKey("k4")){
            GlobalData::shared()->branchOnStr = function_on4->valueForKey("k4")->getCString();
        }
        if(function_on4->objectForKey("k8")){
            GlobalData::shared()->shakeFlag = function_on4->valueForKey("k8")->intValue();
        }
        if(function_on4->objectForKey("k7")){
            GlobalData::shared()->shakeGuideFlag = function_on4->valueForKey("k7")->intValue();
        }
        
        GlobalData::shared()->alliance_territory_banner_switch = 0;
        if(function_on4->objectForKey("k9")){
            GlobalData::shared()->alliance_territory_banner_switch = function_on4->valueForKey("k9")->intValue();
        }

        if(function_on4->objectForKey("k10")){
//            GlobalData::shared()->hd_switch = function_on4->valueForKey("k10")->intValue();
//            CCUserDefault::sharedUserDefault()->setIntegerForKey(SERVER_SWITCH_HD, GlobalData::shared()->hd_switch);
//            CCUserDefault::sharedUserDefault()->flush();
        }
        if(function_on4->objectForKey("k11")){
            GlobalData::shared()->replayTime = function_on4->valueForKey("k11")->intValue();
        }
        if(function_on4->objectForKey("k12")){
            GlobalData::shared()->cnReplayFlag = function_on4->valueForKey("k12")->intValue();
        }
    }
    GlobalData::shared()->chinaSwitchFlag = 0;
    if(dataConfig->objectForKey("function_on5")){
    CCDictionary* function_on5 = _dict(dataConfig->objectForKey("function_on5"));
        if(function_on5->objectForKey("k1")){
            GlobalData::shared()->shakeAllianceFlag = function_on5->valueForKey("k1")->intValue();
        }
        if(function_on5->objectForKey("k2")){
            GlobalData::shared()->picUploadFlag = function_on5->valueForKey("k2")->intValue();
        }
        if(function_on5->objectForKey("k3")){
            GlobalData::shared()->sceneResourceStepLoad = function_on5->valueForKey("k3")->intValue();
        }
        if(function_on5->objectForKey("k4")){
            GlobalData::shared()->chinaSwitchFlag = function_on5->valueForKey("k4")->intValue();
        }
        if(function_on5->objectForKey("k5")){
            CCUserDefault::sharedUserDefault()->setStringForKey("forbiddenDevice", function_on5->valueForKey("k5")->getCString());
        }
        if(function_on5->objectForKey("k6")){
            CCUserDefault::sharedUserDefault()->setStringForKey("deviceServer", function_on5->valueForKey("k6")->getCString());
        }
        
        GlobalData::shared()->equipShare_control = "0";
        if(function_on5->objectForKey("k7")){
            GlobalData::shared()->equipShare_control = function_on5->valueForKey("k7")->getCString();
        }
        if (function_on5->objectForKey("k11"))
        {
            GlobalData::shared()->hd_switch_version = function_on5->valueForKey("k11")->getCString();
            if (CCCommonUtils::checkVersion(GlobalData::shared()->hd_switch_version)) {
                GlobalData::shared()->hd_switch = 1;
            } else {
                GlobalData::shared()->hd_switch = 0;
            }
//            CCLOG("==SERVER_SWITCH_HD===>>>,%s", GlobalData::shared()->hd_switch_version.c_str());
            CCUserDefault::sharedUserDefault()->setIntegerForKey(SERVER_SWITCH_HD, GlobalData::shared()->hd_switch);
//            CCUserDefault::sharedUserDefault()->setIntegerForKey(SERVER_SWITCH_HD, 1);
            CCUserDefault::sharedUserDefault()->flush();
        }
        else
        {
            CCUserDefault::sharedUserDefault()->setIntegerForKey(SERVER_SWITCH_HD, 0);
            CCUserDefault::sharedUserDefault()->flush();
        }
        if(function_on5->objectForKey("k10")){
            string versionFlag = function_on5->valueForKey("k10")->getCString();
            if (checkVersion(versionFlag)) {
                GlobalData::shared()->superManFlag = 1;
            }
            else{
                GlobalData::shared()->superManFlag = 0;
            }
            
        }
    }
    GlobalData::shared()->on_other_chinaFlag_k1 = 0;
    if(dataConfig->objectForKey("on_other")){
        CCDictionary* on_other = _dict(dataConfig->objectForKey("on_other"));
        GlobalData::shared()->on_other_chinaFlag_k1 = on_other->valueForKey("k1")->intValue();
        GlobalData::shared()->protectPostDelayTime = on_other->valueForKey("k3")->intValue();
        if (on_other->objectForKey("k4")) {
            GlobalData::shared()->isCrossServerSendGiftValid = on_other->valueForKey("k4")->intValue();
        }
        if (on_other->objectForKey("k5")) {
            GlobalData::shared()->bindGuideInterval = on_other->valueForKey("k5")->intValue();
        }
        if (on_other->objectForKey("k6"))
        {
            GlobalData::shared()->isDirectBind = on_other->valueForKey("k6")->intValue();
        }
        if (on_other->objectForKey("k8"))
        {
            GlobalData::shared()->protectDisappearPushMin = on_other->valueForKey("k8")->intValue();
        }
        
        
    }
    if(dataConfig->objectForKey("function_on7")){
        CCDictionary* function_on7 = _dict(dataConfig->objectForKey("function_on7"));

        GlobalData::shared()->connectionFlag = function_on7->valueForKey("k3")->intValue();
    }
    ////根据版本控制兵营升星
    if(dataConfig->objectForKey("bingying_star_on")){
        CCDictionary* function_on6 = _dict(dataConfig->objectForKey("bingying_star_on"));
        std::string ServerStr = function_on6->valueForKey("k1")->getCString();
        std::string versionStr = function_on6->valueForKey("k2")->getCString();
        FunBuildController::getInstance()->building_barracck_on = 0;
        if (checkVersion(versionStr) && checkServer(ServerStr))
        {
            FunBuildController::getInstance()->building_barracck_on = 1;
        }
    }
    
    //根据版本号判断是否开放奇迹建筑
    if(dataConfig->objectForKey("function_on6")){
        CCDictionary* function_on6 = _dict(dataConfig->objectForKey("function_on6"));
        if(function_on6->objectForKey("k5")){
            std::string versionStr = function_on6->valueForKey("k5")->getCString();
//            versionStr = "1.1.8";
            if (checkVersion(versionStr)) {
                FunBuildController::getInstance()->building_Miracle_open = 1;
            }else{
                FunBuildController::getInstance()->building_Miracle_open = 0;
            }
        }
        if(function_on6->objectForKey("k6")){
            int k6Val = function_on6->valueForKey("k6")->intValue();
            if(k6Val==0){
                PayController::getInstance()->setPayCheck(false);
            }else{
                PayController::getInstance()->setPayCheck(true);
            }
        }
        if(function_on6->objectForKey("k11")){
            std::string versionStr = function_on6->valueForKey("k11")->getCString();
            std::vector<string> strVector;
            splitString(versionStr, "|", strVector);
            if(strVector.size()>0)
            {
                bool isOn = false;
                if (strVector[0]==""||strVector[0]=="0") {
                    isOn=false;
                }else if (strVector[0]=="1"){
                    isOn=true;
                }else {
                    if (checkVersion(strVector[0])){
                        isOn=true;
                    }else{
                        isOn=false;
                    }
                }
                CCLOGFUNCF("functionOn6 k11:%s isOn :%d",strVector[0].c_str(),isOn);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                GlobalData::shared()->isOpenTranslateOptimization = isOn;
#endif
            }
            if  (strVector.size() >=2){
                //iOS
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                
                bool isOpen =  ChatServiceCocos2dx::compareVersionWithKeyVersionString(strVector[1]);
                CCLog("%d",isOpen);
#endif
            }
        }

    }


    GlobalData::shared()->startNewGameFlag = 0;
    if(dataConfig->objectForKey("function_on6")){
        CCDictionary* function_on6 = _dict(dataConfig->objectForKey("function_on6"));
        if(function_on6->objectForKey("k2")){
            GlobalData::shared()->deviceCollect = function_on6->valueForKey("k2")->getCString();
        }

        if(function_on6->objectForKey("k3")){
            GlobalData::shared()->startNewGameFlag = function_on6->valueForKey("k3")->intValue();
        }
        if(function_on6->objectForKey("k8")){
            GlobalData::shared()->recordFlag = function_on6->valueForKey("k8")->intValue();
        }
        if (function_on6->objectForKey("k9")) {
            //好友系统开关
            string version = function_on6->valueForKey("k9")->getCString();
            if (CCCommonUtils::checkVersion(version)) {
                FriendsController::getInstance()->m_bOpen = true;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                ChatServiceCocos2dx::setFriendEnable(true);
#endif
            } else {
                FriendsController::getInstance()->m_bOpen = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                ChatServiceCocos2dx::setFriendEnable(false);
#endif
            }
        } else {
            FriendsController::getInstance()->m_bOpen = false;
        }
        if(function_on6->objectForKey("k10")){
            GlobalData::shared()->checkSimulatorFlag = function_on6->valueForKey("k10")->intValue();
        }
    }
    
    
    if (dataConfig->objectForKey("function_on7")) {
        CCDictionary* function_on7 = _dict(dataConfig->objectForKey("function_on7"));
        if (function_on7->objectForKey("k1")) {
            GlobalData::shared()->bImpMusicOpen = function_on7->valueForKey("k1")->intValue();
        }
        if (function_on7->objectForKey("k2")) {
            GlobalData::shared()->bWorldMusicOpen = function_on7->valueForKey("k2")->intValue();
        }
        if (function_on7->objectForKey("k8")) {
            GlobalData::shared()->bImpMusicLocalOpen = function_on7->valueForKey("k8")->intValue();
        }
        if (function_on7->objectForKey("k9")) {
            GlobalData::shared()->bWorldMusicLocalOpen = function_on7->valueForKey("k9")->intValue();
        }
        if (function_on7->objectForKey("k3")) {
            GlobalData::shared()->connectionFlag = function_on7->valueForKey("k3")->intValue();
        }
//        if (function_on7->objectForKey("k10")) {
//            GlobalData::shared()->xmlReloadFlag = function_on7->valueForKey("k10")->getCString();
//        }
        if (function_on7->objectForKey("k5")) {
            GlobalData::shared()->userChangeCheckVersion = function_on7->valueForKey("k5")->getCString();
        }
        if (function_on7->objectForKey("k6")) {
            GlobalData::shared()->changeCheckServer = function_on7->valueForKey("k6")->getCString();
        }
    }

    if (dataConfig->objectForKey("serviceTerms_on")) {
        GlobalData::shared()->permitiCountry = dataConfig->valueForKey("serviceTerms_on")->getCString();
    }
    else
        GlobalData::shared()->permitiCountry = "";
    
    GlobalData::shared()->dragonglass_control = "0";
    if(dataConfig->objectForKey("dragonglass_control")){
        CCDictionary* dragonglass_control = _dict(dataConfig->objectForKey("dragonglass_control"));
        if(dragonglass_control->valueForKey("k2")){
            string dragonglass_k2 = dragonglass_control->valueForKey("k2")->getCString();
            if (dragonglass_k2=="1"){
                if(dragonglass_control->valueForKey("k1")){
                    string dragonglass_k1 = dragonglass_control->valueForKey("k1")->getCString();
                    GlobalData::shared()->dragonglass_control = dragonglass_k1;
                }
            }
        }
        if (dragonglass_control->valueForKey("k3")) {
            GlobalData::shared()->longjingShowTime = dragonglass_control->valueForKey("k3")->intValue();
        }
    }
//    GlobalData::shared()->dragonglass_control = "1";
    
    //新版新手迁城等级限制
    GlobalData::shared()->new_trans_kingdom_level = 0;
    if (dataConfig->objectForKey("beginner_move")) {
        GlobalData::shared()->new_trans_kingdom_level = dataConfig->valueForKey("beginner_move")->intValue();
    }
    
    if(dataConfig->objectForKey("shake")){
        CCDictionary* shakeDic = _dict(dataConfig->objectForKey("shake"));
        if(shakeDic->objectForKey("k5")){
            GlobalData::shared()->shakeCmdDelayTime = shakeDic->valueForKey("k5")->intValue();
        }
        if(shakeDic->objectForKey("k7")){
            GlobalData::shared()->shakePicStr = shakeDic->valueForKey("k7")->getCString();
        }
    }
    
    if(dataConfig->objectForKey("notice")){
        CCDictionary* chatNotice = _dict(dataConfig->objectForKey("notice"));
        if(chatNotice){
            int showT = chatNotice->valueForKey("k1")->doubleValue() / 1000;
            ChatController::getInstance()->setNoticeShowTime(showT);
            int maxNum = chatNotice->valueForKey("k2")->intValue();
            ChatController::getInstance()->setNoticeCharMax(maxNum);
            int price = chatNotice->valueForKey("k3")->intValue();
            ChatController::getInstance()->setNoticePrice(price);
            
        }
    }
    if (dataConfig->objectForKey("world_despoil"))
    {
        CCDictionary* chatNotice = _dict(dataConfig->objectForKey("world_despoil"));
        if(chatNotice){
            int littlelevel = chatNotice->valueForKey("k2")->intValue();
            GlobalData::shared()->robLittleResLevel=littlelevel;
            float littlemaxNum = chatNotice->valueForKey("k3")->floatValue();
            GlobalData::shared()->robLittleResCount=littlemaxNum;
        }
    }
    //抽奖数据初始化
    CCDictionary* lotteryData = _dict(params->objectForKey("lottery"));
    LotteryController::shared()->parseLotteryData(lotteryData);
    LotteryController::shared()->parseLotteryData2(dataConfig);
    //聊天屏蔽
    auto &chatShieldInfo = GlobalData::shared()->chatShieldInfo;
    CCArray* arr = dynamic_cast<CCArray*>(params->objectForKey("chatShield"));
    if(arr){
        chatShieldInfo.parse(arr);
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
        ChatServiceCocos2dx::initBanList(chatShieldInfo.shieldList);
#endif
    }
   ChatController::getInstance()->postShieldUids(GlobalData::shared()->chatShieldInfo.shieldList);
    if (dataConfig->objectForKey("chatShieldMax")) {
        chatShieldInfo.maxNum = dataConfig->valueForKey("chatShieldMax")->intValue();
    }
    
//    CCDictionary *tavernConfig = _dict(dataConfig->objectForKey("tavern_base"));
//    GlobalData::shared()->tavernConfig.parse(tavernConfig);
    
    CCArray* tmpArr = dynamic_cast<CCArray*>(params->objectForKey("exchange_gift"));
    if(tmpArr){
        LuaController::getInstance()->initExchangeGiftInfo(tmpArr);
    }
    if(params->objectForKey("repayinfo")){
        CCDictionary* repayConfig = _dict(params->objectForKey("repayinfo"));
        if(repayConfig){
            RepayController::getInstance()->initRepayInfo(repayConfig);
        }
    }else{
        RepayController::getInstance()->initRepayInfo(NULL);
    }
    if (dataConfig->objectForKey("wb_fire")) {
        CCDictionary *wb_fire = _dict(dataConfig->objectForKey("wb_fire"));
        int k1 = wb_fire->valueForKey("k1")->intValue();
        GlobalData::shared()->wb_fire_k1 = k1;//k1表示每小时的燃烧城防值
    }
    
    if (dataConfig->objectForKey("wb_condition")) {//控制跨服进入条件
        CCDictionary *wb_condition = _dict(dataConfig->objectForKey("wb_condition"));
        int k1 = wb_condition->valueForKey("k1")->intValue();
        int k2 = wb_condition->valueForKey("k2")->intValue();
        int k3 = wb_condition->valueForKey("k3")->intValue();
        ActivityController::getInstance()->wb_condition_k1 = k1;
        ActivityController::getInstance()->wb_condition_k2 = k2;
        ActivityController::getInstance()->wb_condition_k3 = k3;
    }
    if (dataConfig->objectForKey("wb_info")) {
        CCDictionary *wb_info = _dict(dataConfig->objectForKey("wb_info"));
        int k7 = wb_info->valueForKey("k7")->intValue();
        int k6 = wb_info->valueForKey("k6")->intValue();
        int k9 = wb_info->valueForKey("k9")->intValue();
        float k10 = wb_info->valueForKey("k10")->floatValue();
        int k11 = wb_info->valueForKey("k11")->intValue();
        ActivityController::getInstance()->wb_info_k2 = wb_info->valueForKey("k2")->getCString();
        if(k7<=0){
            k7 = 1;
        }
        if(k9<=0){
            k9 = 1;
        }
        if(k10<=0){
            k10 = 1;
        }
        ActivityController::getInstance()->wb_info_k6 = k6;
        ActivityController::getInstance()->wb_info_k7 = k7;
        ActivityController::getInstance()->wb_info_k9 = k9;
        ActivityController::getInstance()->wb_info_k10 = k10;
        ActivityController::getInstance()->wb_info_k11 = k11;
    }
    if (dataConfig->objectForKey("wa_army")) {
        CCDictionary *wa_army = _dict(dataConfig->objectForKey("wa_army"));
        int k1 = wa_army->valueForKey("k1")->intValue();
        float k2 = wa_army->valueForKey("k2")->floatValue();
        int k3 = wa_army->valueForKey("k3")->intValue();
        int k4 = wa_army->valueForKey("k4")->intValue();
        ActivityController::getInstance()->wa_army_k1 = k1;
        ActivityController::getInstance()->wa_army_k2 = k2;
        ActivityController::getInstance()->wa_army_k3 = k3;
        ActivityController::getInstance()->wa_army_k4 = k4;
        
    }
    
    if(dataConfig->objectForKey("wa_time")){
        CCDictionary *wa_time = _dict(dataConfig->objectForKey("wa_time"));
        if (ActivityController::getInstance()->wa_time_list.size()>0) {
            ActivityController::getInstance()->wa_time_list.clear();
        }
        for (int i=1; i<10; i++) {
            std::string key = "k"+Value(i).asString();
            if (wa_time->objectForKey(key)) {
                int value = wa_time->valueForKey(key)->intValue();
                ActivityController::getInstance()->wa_time_list.push_back(value);
            }
            else{
                break;
            }
        }
    }

    ActivityController::getInstance()->wbScoreMap.clear();
    if (dataConfig->objectForKey("wb_score")) {
        CCDictionary *wb_score = _dict(dataConfig->objectForKey("wb_score"));
        CCDictElement *pObj;
        CCDICT_FOREACH(wb_score, pObj) {
            string key = pObj->getStrKey();
            string value = dynamic_cast<CCString*>(pObj->getObject())->getCString();
            ActivityController::getInstance()->wbScoreMap[key] = value;
        }
    }
    if (dataConfig->objectForKey("wb_timing")) {
        CCDictionary *wb_timing = _dict(dataConfig->objectForKey("wb_timing"));
        int k1 = wb_timing->valueForKey("k1")->intValue();
        int k2 = wb_timing->valueForKey("k2")->intValue();
        ActivityController::getInstance()->wb_timing_k1 = k1;
        ActivityController::getInstance()->wb_timing_k2 = k2;
    }
    if (dataConfig->objectForKey("wb_switch")) {
        CCDictionary *wb_switch = _dict(dataConfig->objectForKey("wb_switch"));
        int k1 = wb_switch->valueForKey("k1")->intValue();
        string k2 = wb_switch->valueForKey("k2")->getCString();
        int k3 = wb_switch->valueForKey("k3")->intValue();
        string k4 = wb_switch->valueForKey("k4")->getCString();
        int k6 = wb_switch->valueForKey("k6")->intValue();
        string k7 = wb_switch->valueForKey("k7")->getCString();
        ActivityController::getInstance()->wb_switch_k1 = k1;
        ActivityController::getInstance()->wb_switch_k2 = k2;
        ActivityController::getInstance()->wb_switch_k3 = k3;
        ActivityController::getInstance()->wb_switch_k4 = k4;
        ActivityController::getInstance()->wb_switch_k6 = k6;
        ActivityController::getInstance()->wb_switch_k7 = k7;
    }
    ActivityController::getInstance()->wa_switch_k3 = 0;
    if (dataConfig->objectForKey("wa_switch")) {
        CCDictionary *wa_switch = _dict(dataConfig->objectForKey("wa_switch"));
        int k1 = wa_switch->valueForKey("k1")->intValue();
        string k2 = wa_switch->valueForKey("k2")->getCString();
        int k3 = wa_switch->valueForKey("k3")->intValue();
        string k4 = wa_switch->valueForKey("k4")->getCString();
        ActivityController::getInstance()->wa_switch_k1 = k1;
        ActivityController::getInstance()->wa_switch_k2 = k2;
        ActivityController::getInstance()->wa_switch_k3 = k3;
        ActivityController::getInstance()->wa_switch_k4 = k4;
    }
    if (dataConfig->objectForKey("smithy_history")) {
        CCDictionary *smithy_history = _dict(dataConfig->objectForKey("smithy_history"));
        string k1 = smithy_history->valueForKey("k1")->getCString();
        EquipmentController::getInstance()->smithy_history_mate=3;
        EquipmentController::getInstance()->smithy_history_equip=3;
        EquipmentController::getInstance()->smithy_history_pary=10;
        vector<string> vec;
        CCCommonUtils::splitString(k1, "|", vec);
        if (vec.size()==3) {
            EquipmentController::getInstance()->smithy_history_mate=atoi(vec[0].c_str());
            EquipmentController::getInstance()->smithy_history_equip=atoi(vec[1].c_str());
            EquipmentController::getInstance()->smithy_history_pary=atoi(vec[2].c_str());
        }
        int k2 = smithy_history->valueForKey("k2")->intValue();
        EquipmentController::getInstance()->equipMake_newTips_mLv=k2;
    }
    ActivityController::getInstance()->wa_info_k9 = 0;
    if (dataConfig->objectForKey("wa_info")) {
        CCDictionary *wa_info = _dict(dataConfig->objectForKey("wa_info"));
        int k11 = wa_info->valueForKey("k11")->intValue();
        int k9 = wa_info->valueForKey("k9")->intValue();
        ActivityController::getInstance()->wa_info_k11 = k11;
        ActivityController::getInstance()->wa_info_k9 = k9;
    }
    ActivityController::getInstance()->wa_fire_k1 = 0;
    if (dataConfig->objectForKey("wa_fire")) {
        CCDictionary *wa_fire = _dict(dataConfig->objectForKey("wa_fire"));
        int k1 = wa_fire->valueForKey("k1")->intValue();
        ActivityController::getInstance()->wa_fire_k1 = k1;
    }
    ActivityController::getInstance()->wa_score_k10 = dataConfig->valueForKey("dbf_maxScoreView")->doubleValue();
    if (dataConfig->objectForKey("alliance_quest")) {
        AllianceDailyController::getInstance()->swDaily = dataConfig->valueForKey("alliance_quest")->getCString();
    }
    AllianceDailyController::getInstance()->swDailyChat = "0";
    if (dataConfig->objectForKey("alliance_quest_chat")) {
        AllianceDailyController::getInstance()->swDailyChat = dataConfig->valueForKey("alliance_quest_chat")->getCString();
    }
    
    FriendsController::getInstance()->fbSwitch = "0";
    if (dataConfig->objectForKey("function_on9")) {
        CCDictionary* function_on9 = _dict(dataConfig->objectForKey("function_on9"));
        if (function_on9->objectForKey("k1")) {
            FriendsController::getInstance()->fbSwitch = function_on9->valueForKey("k1")->getCString();
        }
    }
    GlobalData::shared()->fun_bindRes_switch = false;
    if (dataConfig->objectForKey("on_trumpet_k2"))
    {
        int on_trumpet_k2 = dataConfig->valueForKey("on_trumpet_k2")->intValue();
        if (on_trumpet_k2 == 1 ) {
            GlobalData::shared()->fun_bindRes_switch = true;
        }
    }
    CCDictionary *wk_info = _dict(dataConfig->objectForKey("wk_info"));
    ActivityController::getInstance()->parseWKActivityConfig(wk_info);
    LuaController::getInstance()->parseConfig(dataConfig);
}

void CCCommonUtils::initEffect(CCDictionary *params){
    // effectValues map init
    if (params->objectForKey("effect")) {
        auto &effectMap = GlobalData::shared()->effectValues;
        auto effectDict = _dict(params->objectForKey("effect"));
        CCDictElement *pObj;
        CCDICT_FOREACH(effectDict, pObj) {
            string key = pObj->getStrKey();
            float value = atoi((dynamic_cast<CCString*>(pObj->getObject())->getCString()));
            effectMap[atoi(key.c_str())] = value;
        }
        EquipmentController::getInstance()->reSetEquipSci(0, true);
    }
    
    GlobalData::shared()->effToFormatMap.clear();
    auto effFormatDictInfo = LocalController::shared()->DBXMLManager()->getGroupByKey("effect_format");
    if(effFormatDictInfo)
    {
        CCDictElement* ele;
        CCDICT_FOREACH(effFormatDictInfo, ele)
        {
            CCDictionary* dictInfo = _dict(ele->getObject());
            string effectNums = dictInfo->valueForKey("effect")->getCString();
            int format = dictInfo->valueForKey("format")->intValue();
            int plus_minus = dictInfo->valueForKey("plus_minus")->intValue();
            vector<string> tmpVec;
            CCCommonUtils::splitString(effectNums, ";", tmpVec);
            for (int i=0; i<tmpVec.size(); i++) {
                GlobalData::shared()->effToFormatMap[ tmpVec[i] ] = format;
                GlobalData::shared()->effPM_Map[ tmpVec[i] ] = plus_minus;
            }
        }
    }
}

void CCCommonUtils::initTime(CCDictionary *params){
    //时间初始化
    GlobalData::shared()->setWorldTime(params->valueForKey("db_timezone_offset")->intValue(), params->valueForKey("db_utc_timestamp")->intValue());
    
    // init timeStamp
    auto timeStampForever = TimeStamp();
    timeStampForever.type = Stamp_FOREVER;
    timeStampForever.finishTime = std::numeric_limits<int>::max() - 1;
    GlobalData::shared()->timeStampMap[0] = timeStampForever;
}

void CCCommonUtils::initTranslation(CCDictionary *params){
    //自动翻译
    if(params->objectForKey("translation")){
        GlobalData::shared()->translation = params->valueForKey("translation")->boolValue();
    }
    if(params->objectForKey("mail_translation")){
        GlobalData::shared()->mail_translation = params->valueForKey("mail_translation")->boolValue();
    }else{
        GlobalData::shared()->mail_translation = false;
    }
}

void CCCommonUtils::initGaid(CCDictionary *params){
    //gaid
    if(params->objectForKey("gaid")){
        GlobalData::shared()->gaid = params->valueForKey("gaid")->getCString();
    }
}

void CCCommonUtils::initEffectState(CCDictionary *params){
    // play state
    if (params->objectForKey("effectState")) {
        // search for type 500000 for protect stamp
        auto stateDict = dynamic_cast<CCDictionary*>(params->objectForKey("effectState"));
        if (stateDict) {
            string tempKey = CC_ITOA(PLAYER_PROTECTED_TEN_MIN);
            GlobalData::shared()->playerInfo.protectTimeStamp = stateDict->valueForKey(tempKey)->doubleValue();
            for(int i=0;i<10;i++){
                string temp = CC_ITOA(PLAYER_PROTECTED_TIME1+i);
                if(stateDict->objectForKey(temp)){
                    GlobalData::shared()->playerInfo.protectTimeStamp = stateDict->valueForKey(temp)->doubleValue();
                }
            }
            GlobalData::shared()->playerInfo.resourceProtectTimeStamp = stateDict->valueForKey(CC_ITOA(LORD_SKILL_PROTECTED))->doubleValue();
            
            CCDictElement* element;
            CCDICT_FOREACH(stateDict, element)
            {
                string key = element->getStrKey();
                double time = stateDict->valueForKey(key)->doubleValue()/1000;
                if (time>0) {
                    time = GlobalData::shared()->changeTime(time);
                }
                if(key == "500526"){
                    time = -1;
                }
                GlobalData::shared()->statusMap[atoi(key.c_str())] = time;
            }
            ToolController::getInstance()->checkMaincityCustom();
        }
    }
}

void CCCommonUtils::initStatus(CCDictionary *params){
    if (params->objectForKey("status")) {
        auto arr = dynamic_cast<CCArray*>(params->objectForKey("status"));
        CCDictionary *item = NULL;
        GlobalData::shared()->effectStateMap.clear();
        for (int i=0; i<arr->count(); i++) {
            item = _dict(arr->objectAtIndex(i));
            auto effState = stateEffect();
            effState.effectId = item->valueForKey("effNum")->intValue();
            effState.value = item->valueForKey("effVal")->intValue();
            effState.stateId = item->valueForKey("stateId")->intValue();
            GlobalData::shared()->effectStateMap[effState.effectId].push_back(effState);
        }
    }
}

void CCCommonUtils::initWorld(CCDictionary *params){
    TimeUpMarchProcesser::getInstance()->clear();
    // world info
    if (params->objectForKey("world")) {
        auto worldInfo = _dict(params->objectForKey("world"));
        unsigned int point = worldInfo->valueForKey("point")->intValue();
        WorldController::getInstance()->enterMap();
        if (!point) {
            WorldController::getInstance()->selfPoint = ccp(-1, -1);
        } else {
            WorldController::getInstance()->selfPoint = WorldController::getPointByIndex(point);
        }
        // init world time
        double stamp = (double)params->valueForKey("db_timezone_offset")->intValue() * 1000.0;
        WorldController::getInstance()->resetTime(stamp);
        GlobalData::shared()->cityDefenseVal = worldInfo->valueForKey("cityDefValue")->intValue();
        GlobalData::shared()->cityFireStamp = worldInfo->valueForKey("ft")->doubleValue();
        GlobalData::shared()->cityStartFireStamp = WorldController::getInstance()->getTime();
        GlobalData::shared()->cityLastUpdateDefenseStamp = worldInfo->valueForKey("lastCityDefTime")->doubleValue();
        GlobalData::shared()->cityTileCountry = worldInfo->valueForKey("gridType")->intValue();
        ActivityController::getInstance()->ckfNeedLeave = worldInfo->valueForKey("ckfNeedLeave")->intValue();
        WorldController::getInstance()->freePortCnt = worldInfo->valueForKey("freePortCnt")->intValue();
        GlobalData::shared()->campType = (CampType)worldInfo->valueForKey("dbfCamp")->intValue();
        if(worldInfo->valueForKey("sct")){
            GlobalData::shared()->playerInfo.protectCDTime = worldInfo->valueForKey("sct")->doubleValue();
        }
        ActivityController::getInstance()->ckfProtectTime = 0;
        if(worldInfo->objectForKey("ckfProtectTime")){
            double ckfProtectTime = worldInfo->valueForKey("ckfProtectTime")->doubleValue() / 1000;
            if (ckfProtectTime!=0) {
                ActivityController::getInstance()->ckfProtectTime = GlobalData::shared()->changeTime(ckfProtectTime);
            }
        }
        ActivityController::getInstance()->abfEnterCDTime = 0;
        if(worldInfo->objectForKey("abfEnterCDTime")){
            double abfEnterCDTime = worldInfo->valueForKey("abfEnterCDTime")->doubleValue() / 1000;
            if (abfEnterCDTime!=0) {
                ActivityController::getInstance()->abfEnterCDTime = GlobalData::shared()->changeTime(abfEnterCDTime);
            }
        }
        
        if(GlobalData::shared()->cityTileCountry == NEUTRALLY)
        {
            if(WorldController::getInstance()->selfPoint.x<574||WorldController::getInstance()->selfPoint.x>626||WorldController::getInstance()->selfPoint.y<574||WorldController::getInstance()->selfPoint.y>626)
            {
                GlobalData::shared()->cityTileCountry=SPACE;
            }
        }
        WorldController::getInstance()->m_mainThreadID = pthread_self();
        
        // init self march info
        WorldController::getInstance()->initMarchInfo(dynamic_cast<CCArray*>(worldInfo->objectForKey("m")));
        if(worldInfo->objectForKey("stamina")){
            WorldController::getInstance()->currentStamine = worldInfo->valueForKey("stamina")->intValue();
        }
        if(worldInfo->objectForKey("lastStaminaTime")){
            WorldController::getInstance()->lastStamineTime = worldInfo->valueForKey("lastStaminaTime")->doubleValue();
        }
        if(worldInfo->objectForKey("DBFBuilding")){
            CCArray* arr = dynamic_cast<CCArray*>(worldInfo->objectForKey("DBFBuilding"));
            if (arr) {
                int num = arr->count();
                for(int i=0;i<num;i++){
                    auto info = _dict(arr->objectAtIndex(i));
                    unsigned id = info->valueForKey("id")->intValue();
                    std::string ai = info->valueForKey("ai")->getCString();
                    double occupyST = info->valueForKey("occupyST")->doubleValue();
                    DBuildinginfo portInfo;
                    portInfo.index = id;
                    portInfo.ai = ai;
                    portInfo.occupyST = occupyST;
                    WorldController::getInstance()->m_dbuildingInfo[id] = portInfo;

                }
            }
        }
    }
   
    if(params->objectForKey("treasureMap")){
        WorldController::getInstance()->initTreasureInfo(dynamic_cast<CCArray*>(params->objectForKey("treasureMap")));
    }else{
        WorldController::getInstance()->clearTreasureInfo();
    }
}

void CCCommonUtils::initPray(CCDictionary *params){
    if (params->objectForKey("pray")) {
        auto sacrificeDict = _dict(params->objectForKey("pray"));
        auto &sacrifice = GlobalData::shared()->sacrificeInfo;
        sacrifice.parse(sacrificeDict);
        
    }
}

void CCCommonUtils::initLord(CCDictionary *params){
    CCDictionary* lord = _dict(params->objectForKey("lord"));
    
    GlobalData::shared()->generalConfig.oneTimeFree = lord->valueForKey("singleRndGenFreeNum")->intValue();
    GlobalData::shared()->generalConfig.firstRndGenFlag = lord->valueForKey("firstRndGenFlag")->intValue();
    GlobalData::shared()->bagGoldBuyNum = lord->valueForKey("buyGeneralTimes")->intValue();
    WorldController::getInstance()->currentMonsterLevel = lord->valueForKey("pveLevel")->intValue();
    GlobalData::shared()->medalid = lord->valueForKey("medal")->getCString();
    if (lord->objectForKey("freshRechargeTotal"))
    {
        GlobalData::shared()->freshRechargeTotal = lord->valueForKey("freshRechargeTotal")->intValue();
    }
    
    if (lord->objectForKey("nextDay")) {
        int tmpTime = lord->valueForKey("nextDay")->doubleValue()/1000;
        if (tmpTime>0) {
            ToolController::getInstance()->m_lastTime = GlobalData::shared()->changeTime(tmpTime);
        }
    }
    if (lord->objectForKey("exchange")) {
        ToolController::getInstance()->m_canBuy = lord->valueForKey("exchange")->boolValue();
    }
    //马厩领取时间
    GlobalData::shared()->lastAcceptEnergyTime = lord->valueForKey("lastAcceptEnergyTime")->doubleValue() / 1000;
    
    if (lord->objectForKey("firstRndGenFlag")) {
        GlobalData::shared()->generalConfig.firstRndGenFlag = lord->valueForKey("firstRndGenFlag")->intValue();
    }
    if (lord->objectForKey("levelUp")) {
        GlobalData::shared()->lordInfo.levelUp= lord->valueForKey("levelUp")->intValue();
    }
//    auto &loginReward = GlobalData::shared()->loginReward;
//    loginReward.canDoLottery = lord->valueForKey("loginAward")->boolValue();
    
    GlobalData::shared()->lordInfo.energy = lord->valueForKey("energy")->intValue();
    GlobalData::shared()->lordInfo.nextEnergyRecoverTime = lord->valueForKey("nextEnergyRecoverTime")->doubleValue();
    GlobalData::shared()->lordInfo.buyEnergyTimesDaily = lord->valueForKey("buyEnergyTimesDaily")->intValue();
    if (lord->objectForKey("feedCount")) {
        GlobalData::shared()->lordInfo.feed_count = lord->valueForKey("feedCount")->intValue();
    }
}

void CCCommonUtils::initPushSetting(CCDictionary *params){
    if (params->objectForKey("pushSetting")) {
        auto pushSettingArr = dynamic_cast<CCArray*>(params->objectForKey("pushSetting"));
        CCObject* pushSettingObj = NULL;
        CCARRAY_FOREACH(pushSettingArr, pushSettingObj){
            PushSettingInfo* pushSettingInfo = NULL;
            CCDictionary *dict = dynamic_cast<CCDictionary*>(pushSettingObj);
            if(!dict){
                continue;
            }
            int type = -1;
            if (dict->objectForKey("type")) {
                type = dict->valueForKey("type")->intValue();
            }
            
            int index = 0;
            int totalIndex = GlobalData::shared()->pushSettingVector.size();
            while (index < totalIndex) {
                PushSettingInfo *info = GlobalData::shared()->pushSettingVector[index];
                if(type == info->type){
                    pushSettingInfo = info;
                    break;
                }
                index++;
            }
            
            if(pushSettingInfo == NULL){
                pushSettingInfo = PushSettingInfo::create();
                pushSettingInfo->retain();
                GlobalData::shared()->pushSettingVector.push_back(pushSettingInfo);
            }
            pushSettingInfo->parse(dynamic_cast<CCDictionary*>(pushSettingObj));
        }
    }
}

void CCCommonUtils::initBusinessMan(CCDictionary *params){
    //行走商人
    if(params->objectForKey("businessMan")){
        ToolController::getInstance()->initMerchanteInfo(dynamic_cast<CCDictionary*>(params->objectForKey("businessMan")));
    }
}

void CCCommonUtils::initMonthLyCards(CCDictionary *params){
    //月卡信息
    GlobalData::shared()->tomorrow_time = params->valueForKey("tomorrow")->doubleValue();
    GlobalData::shared()->monthCardInfoList.clear();
    if(params->objectForKey("monthLyCards")){
        auto arr = dynamic_cast<CCArray*>(params->objectForKey("monthLyCards"));
        CCDictionary *item = NULL;
        for (int i=0; i<arr->count(); i++) {
            item = _dict(arr->objectAtIndex(i));
            std::string cardid = item->valueForKey("itemId")->getCString();
            if(!cardid.empty()){
                auto cardInfo = new MonthCardInfo(item);
                GlobalData::shared()->monthCardInfoList[cardInfo->getItemId()] =  cardInfo;
            }
        }
    }
}

void CCCommonUtils::initFreshRecharge(__Dictionary* params)
{
    FreshRechargeController::getInstance()->initFreshRecharge(params);
    FreshRechargeController::getInstance()->initFreshRechargeRewardedIds(params);
    if (FreshRechargeController::getInstance()->hasRwdedAll())
    {
        GlobalData::shared()->bFreshRechargeOpen = false;
    } else {
        CCDictionary* dataConfig = _dict(params->objectForKey("dataConfig"));
        if (dataConfig->objectForKey("recharge_event")) {
            auto recharge_event = _dict(dataConfig->objectForKey("recharge_event"));
            if (recharge_event->objectForKey("k5")) {
                string recharge_k5 = recharge_event->valueForKey("k5")->getCString();
                if (!recharge_k5.empty()) {
                    std::vector<string> vec;
                    CCCommonUtils::splitString(recharge_k5, ";", vec);
                    if (vec.size() > 1)
                    {
                        if (vec.at(1) == "0") {
                            GlobalData::shared()->bFreshRechargeOpen = false;
                        } else {
                            if (!checkVersion(vec.at(0)))
                            {
                                GlobalData::shared()->bFreshRechargeOpen = false;
                            } else {
                                if (vec.at(1) == "all") {
                                    GlobalData::shared()->bFreshRechargeOpen = true;
                                } else {
                                    std::vector<std::string> vec1;
                                    CCCommonUtils::splitString(vec[1], ",", vec1);
                                    for (auto it = vec1.begin(); it != vec1.end(); ++it) {
                                        std::vector<std::string> vec2;
                                        CCCommonUtils::splitString(*it, "-", vec2);
                                        if (vec2.size() == 1) {
                                            if (atoi(vec2.at(0).c_str()) == GlobalData::shared()->playerInfo.selfServerId) {
                                                GlobalData::shared()->bFreshRechargeOpen = true;
                                                break;
                                            }
                                        } else if (vec.size() == 2) {
                                            int start = atoi(vec.at(0).c_str());
                                            int end = atoi(vec.at(1).c_str());
                                            int self = GlobalData::shared()->playerInfo.selfServerId;
                                            if (self >= start && self <= end)
                                            {
                                                GlobalData::shared()->bFreshRechargeOpen = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void CCCommonUtils::initPlayerInfo(CCDictionary *params){
    //用户信息初始化
    auto &playerInfo = GlobalData::shared()->playerInfo;
    if(params->objectForKey("isVerifyChangeAccount")){
        GlobalData::shared()->userNeedChangeCheck = params->valueForKey("isVerifyChangeAccount")->boolValue();
    }
    // new user
    if (params->objectForKey("newAccount")) {
        GlobalData::shared()->isFirstLoginGame = params->valueForKey("newAccount")->boolValue();
    } else {
        GlobalData::shared()->isFirstLoginGame = false;
    }
    
    if (params->objectForKey("isShowCDkey")) {
        GlobalData::shared()->isShowCDkey = params->valueForKey("isShowCDkey")->boolValue();
    } else {
        GlobalData::shared()->isShowCDkey = false;
    }
    GlobalData::shared()->dbFightBeginTime = params->valueForKey("dbFightBeginTime")->doubleValue()/1000;
    if(GlobalData::shared()->dbFightBeginTime>0){
        GlobalData::shared()->dbFightBeginTime = GlobalData::shared()->changeTime(GlobalData::shared()->dbFightBeginTime);
    }
    GlobalData::shared()->dbFightEndTime = params->valueForKey("dbFightEndTime")->doubleValue()/1000;
    if(GlobalData::shared()->dbFightEndTime>0){
        GlobalData::shared()->dbFightEndTime = GlobalData::shared()->changeTime(GlobalData::shared()->dbFightEndTime);
    }
    
    CCDictionary* pUserInfo = _dict(params->objectForKey("user"));
    GlobalData::shared()->serverMax = pUserInfo->valueForKey("serverMax")->intValue();
    if(pUserInfo->objectForKey("phoneDevice")){
        GlobalData::shared()->phoneDevice = pUserInfo->valueForKey("phoneDevice")->getCString();
    }
    WorldController::getInstance()->initServerMapPos();
    if(pUserInfo->objectForKey("serverId")){
        playerInfo.selfServerId = pUserInfo->valueForKey("serverId")->intValue();
        if(!GlobalData::shared()->isPressedHomeKey || playerInfo.currentServerId == -1)//(for change serve && press home btn;for first login should init)
        {
            playerInfo.currentServerId = playerInfo.selfServerId;
        }
        
        
        CCDictionary* dataConfig = _dict(params->objectForKey("dataConfig"));
        if (dataConfig->objectForKey("function_on7")) {
            CCDictionary* function_on7 = _dict(dataConfig->objectForKey("function_on7"));
            if (function_on7->objectForKey("k11")) {
                string k11String = function_on7->valueForKey("k11")->getCString();
                if(k11String.compare("0")==0){
                    g_performanceCrashSwitch=false;
                }else if(k11String.compare("all")==0){
                    g_performanceCrashSwitch=true;
                }else if(!k11String.empty()){
                    int selfserver = GlobalData::shared()->playerInfo.selfServerId;
                    vector<string> serverlist;
                    CCCommonUtils::splitString(k11String, ";", serverlist);
                    auto iterK = serverlist.begin();
                    while (iterK!=serverlist.end()) {
                        auto posK = (*iterK).find("-");
                        if(posK!=string::npos){
                            int sIdx = atoi((*iterK).substr(0,posK).c_str());
                            int eIdx = atoi((*iterK).substr(posK+1).c_str());
                            if(selfserver>=sIdx && selfserver<=eIdx){
                                g_performanceCrashSwitch=true;
                                break;
                            }
                        }else{
                            int cIdx = atoi((*iterK).c_str());
                            if(cIdx == selfserver){
                                g_performanceCrashSwitch=true;
                                break;
                            }
                        }
                        ++iterK;
                    }
                }
            }
        }
        if(dataConfig->objectForKey("function_on8"))
        {
            CCDictionary* function_on8 = _dict(dataConfig->objectForKey("function_on8"));
            GlobalData::shared()->useNewKey = false;
            std::string versionStr = function_on8->valueForKey("k3")->getCString();
            std::string serverStr =function_on8->valueForKey("k4")->getCString();
            if (checkVersion(versionStr))
            {
                if(checkServer(serverStr))
                {
                    GlobalData::shared()->useNewKey = true;
                }
            }
//            bool teleport_limit_open;//开关
//            int teleport_limit_time;//时间
//            std::string teleport_limit_openServers;
            versionStr = function_on8->valueForKey("k6")->getCString();
            serverStr =function_on8->valueForKey("k8")->getCString();
            GlobalData::shared()->teleport_limit_open = false;
            if (checkVersion(versionStr))
            {
                if(checkServer(serverStr))
                {
                    GlobalData::shared()->teleport_limit_open = true;
                }
            }

            
            if(function_on8->objectForKey("k7"))
            {
                GlobalData::shared()->teleport_limit_time = atoi(function_on8->valueForKey("k7")->getCString());
            }
            
            if(function_on8->objectForKey("k9")){  // 桌面的3D touch开关
                std::string _switch = function_on8->valueForKey("k9")->getCString();
                if (checkVersion(_switch)) {
                    GlobalData::shared()->shortItem_switch = true;
                }
                
            }
            
            if(function_on8->objectForKey("k10")){  // 游戏内的Peek pop开关
                std::string _switch = function_on8->valueForKey("k10")->getCString();
                if (checkVersion(_switch)) {
                    GlobalData::shared()->peek_switch = true;
                }
                
            }
            
            if(function_on8->objectForKey("k11")){  // 清理缓存开关
                std::string _switch = function_on8->valueForKey("k11")->getCString();
                if (checkVersion(_switch)) {
                    GlobalData::shared()->clearCache_switch = true;
                }
                
            }
            
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            GlobalData::shared()->func_qemuBkpt_open = false;
            if(function_on8->objectForKey("k12"))
            {
                if(GlobalData::shared()->analyticID == "cn1")
                {
                    serverStr = function_on8->valueForKey("k12")->getCString();
                    if(checkServer(serverStr))
                    {
                        GlobalData::shared()->func_qemuBkpt_open = true;
                    }
                }
            }
#endif

        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if (dataConfig->objectForKey("function_on9")) {
            CCDictionary* function_on9 = _dict(dataConfig->objectForKey("function_on9"));
            GlobalData::shared()->func_checkEmulator_open = false;
            std::string versionStr;
            std::string serverStr;
            if (function_on9->objectForKey("k2") && function_on9->objectForKey("k3"))
            {
                versionStr = function_on9->valueForKey("k3")->getCString();
                serverStr =function_on9->valueForKey("k2")->getCString();
                if(GlobalData::shared()->analyticID == "cn1")
                {
                    if (checkVersion(versionStr))
                    {
                        if(checkServer(serverStr))
                        {
                            GlobalData::shared()->func_checkEmulator_open = true;
                        }
                    }
                }
            }
        }
#endif
    }
    if(pUserInfo->objectForKey("buildcode")){
        playerInfo.buildcode=pUserInfo->valueForKey("buildcode")->getCString();
        CCUserDefault::sharedUserDefault()->setStringForKey(BUILDCODE, playerInfo.buildcode);
    }
    if(pUserInfo->objectForKey("chNameCount")){
        playerInfo.renameTime = pUserInfo->valueForKey("chNameCount")->intValue();
    }
    playerInfo.level = pUserInfo->valueForKey("level")->intValue();
    playerInfo.exp = pUserInfo->valueForKey("exp")->intValue();
    playerInfo.maxExp = pUserInfo->valueForKey("maxExp")->intValue();
    playerInfo.gold = pUserInfo->valueForKey("gold")->intValue();
    playerInfo.longJing = pUserInfo->valueForKey("crystal")->intValue();//龙晶
    playerInfo.payTotal = pUserInfo->valueForKey("payTotal")->intValue();
    playerInfo.name = pUserInfo->valueForKey("name")->getCString();
    playerInfo.nickName = pUserInfo->valueForKey("nickName")->getCString();
    playerInfo.uid = pUserInfo->valueForKey("uid")->getCString();
    playerInfo.pic = pUserInfo->valueForKey("pic")->getCString();
    playerInfo.country = pUserInfo->valueForKey("country")->intValue();
    playerInfo.openedSystem = pUserInfo->valueForKey("openedSystem")->getCString();
    playerInfo.gmFlag = pUserInfo->valueForKey("gmFlag")->intValue();
//    playerInfo.gmFlag = 1;
    playerInfo.isfirstJoin = pUserInfo->valueForKey("isfirstJoin")->intValue();
    playerInfo.registerTime = pUserInfo->valueForKey("registerTime")->doubleValue() / 1000;
    if(pUserInfo->objectForKey("regTime"))
        playerInfo.regTime = pUserInfo->valueForKey("regTime")->doubleValue() / 1000;
    playerInfo.bindFlag = pUserInfo->valueForKey("bindFlag")->boolValue();
    playerInfo.inviCode = pUserInfo->valueForKey("inviCode")->getCString();
    playerInfo.regCountry = pUserInfo->valueForKey("regCountry")->getCString();
    playerInfo.officer = pUserInfo->valueForKey("officer")->getCString();
    playerInfo.relinquishFlag = pUserInfo->valueForKey("relinquishFlag")->boolValue();
    playerInfo.relinquishEndTime = pUserInfo->valueForKey("relinquishEndTime")->doubleValue();
    GlobalData::shared()->serverType = pUserInfo->valueForKey("serverType")->intValue();
    
    if (pUserInfo->objectForKey("allianceId")) {
        playerInfo.setAllianceId(pUserInfo->valueForKey("allianceId")->getCString());
    }
    if (pUserInfo->objectForKey("lastUpdateTime")) {
        playerInfo.lastUpdateTime = pUserInfo->valueForKey("lastUpdateTime")->intValue();
    }
    
    if (pUserInfo->objectForKey("openedPos")) {
        FunBuildController::getInstance()->initOpenTile(pUserInfo->valueForKey("openedPos")->getCString());
    }
    if (pUserInfo->objectForKey("picVer")) {
        playerInfo.picVer = pUserInfo->valueForKey("picVer")->intValue();
    }
    if (pUserInfo->objectForKey("deviceBindTimes")) {
        GlobalData::shared()->nowGameCnt = pUserInfo->valueForKey("deviceBindTimes")->intValue();
    }
    if(pUserInfo->objectForKey("noticeBanTime")){
        ChatController::getInstance()->setNoticeBanTime(pUserInfo->valueForKey("noticeBanTime")->doubleValue()/1000);
    }
    CCDictionary* allianceInfo = _dict(params->objectForKey("alliance"));
    if (allianceInfo)
    {
        playerInfo.allianceInfo.updateAllianceInfo(allianceInfo);
        if (allianceInfo->objectForKey("targetScience")) {
            string targetScience = allianceInfo->valueForKey("targetScience")->getCString();
            vector<string> vec;
            splitString(targetScience, ";", vec);
            auto& asmap = ScienceController::getInstance()->allianceScienceMap;
            for (auto it = vec.begin(); it != vec.end(); ++it) {
                int scienceId = atoi((*it).c_str());
                if (asmap.find(scienceId) != asmap.end()) {
                    asmap[scienceId].isRecommended = true;
                }
            }
        }
    }
    GlobalData::shared()->playerInfo.crossFightSrcServerId = -1;
    if (pUserInfo->objectForKey("crossFightSrcServerId")) {
        int crossFightSrcServerId = pUserInfo->valueForKey("crossFightSrcServerId")->intValue();
        GlobalData::shared()->playerInfo.crossFightSrcServerId = crossFightSrcServerId;
    }
    
//#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
//    ChatServiceCocos2dx::setPlayerAllianceInfo(playerInfo.allianceInfo.shortName.c_str(),playerInfo.getAllianceId().c_str(),playerInfo.allianceInfo.rank,playerInfo.isfirstJoin);
//#endif
    
    if(pUserInfo->objectForKey("signIn")){
        PortActController::getInstance()->m_isRdLoginDay = pUserInfo->valueForKey("signIn")->intValue();
    }
    
    CCDictionary* roleInfo = _dict(params->objectForKey("playerInfo"));
    playerInfo.updateInfo(roleInfo);
 
    //查看装备
    CCDictionary* seeUserEquip = _dict(params->objectForKey("mixedInfo"));
    if(seeUserEquip->objectForKey("showEquip")){//是否装备别人可见
        playerInfo.showEquip = seeUserEquip->valueForKey("showEquip")->boolValue();
    }
    if(seeUserEquip->objectForKey("showMedal")){//是否装备别人可见
        playerInfo.shouldShowMedal = seeUserEquip->valueForKey("showMedal")->boolValue();
    }
    if(seeUserEquip->objectForKey("scrollEquip")){//
        playerInfo.scrollEquip = seeUserEquip->valueForKey("scrollEquip")->boolValue();
    }
    //材料工坊 材料建造位置数
    if(seeUserEquip->objectForKey("materialBackup")){//材料建造位置数
        playerInfo.materialBackup = seeUserEquip->valueForKey("materialBackup")->intValue();
    }
    if(seeUserEquip->objectForKey("taskCompleteId")){//当前阶段任务ID
        QuestController::getInstance()->m_stageRdId = seeUserEquip->valueForKey("taskCompleteId")->intValue();
    }
    
    if(seeUserEquip->objectForKey("colorblind")){
        playerInfo.isDaltonOn = seeUserEquip->valueForKey("colorblind")->boolValue();
    }
    playerInfo.useGoldCureCDTime = seeUserEquip->valueForKey("useGoldCureCDTime")->doubleValue()/1000;
    if (playerInfo.useGoldCureCDTime>0) {
        playerInfo.useGoldCureCDTime = GlobalData::shared()->changeTime(playerInfo.useGoldCureCDTime);
    }
    //永久皮肤
    if(seeUserEquip->objectForKey("skin")){
        playerInfo.skin = seeUserEquip->valueForKey("skin")->getCString();
    }
    //小公主上次领奖时间
    GlobalData::shared()->princessRwd_lastTime = 0;
    GlobalData::shared()->princessShow = false;
    if (seeUserEquip->objectForKey("princessClick")) {
        GlobalData::shared()->princessRwd_lastTime = seeUserEquip->valueForKey("princessClick")->doubleValue() / 1000;
    }
    double princessRwd_timeGap = GlobalData::shared()->getTimeStamp() - GlobalData::shared()->princessRwd_lastTime;
    if (princessRwd_timeGap > 3 * 60) {
        GlobalData::shared()->princessShow = true;
    }
    
    //校场士兵点击上次领奖时间
    GlobalData::shared()->sqerSoldierRwd_lastTime = 0;
    if (seeUserEquip->objectForKey("soldierClick")) {
        GlobalData::shared()->sqerSoldierRwd_lastTime = seeUserEquip->valueForKey("soldierClick")->doubleValue() / 1000;
    }
    
    //账号绑定
    string _gp = pUserInfo->valueForKey("googlePlay")->getCString();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    _gp = pUserInfo->valueForKey("appStoreId")->getCString();
#endif
    string _localGP = CCUserDefault::sharedUserDefault()->getStringForKey(GP_USERID,"");
    if (_gp != _localGP) {
        CCUserDefault::sharedUserDefault()->setStringForKey(GP_USERID, _gp);
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#else
    string _gpAccountname = pUserInfo->valueForKey("googleAccountName")->getCString();
    string _localAccountname = CCUserDefault::sharedUserDefault()->getStringForKey(GP_USERNAME,"");
    if (_gpAccountname != _localAccountname) {
        CCUserDefault::sharedUserDefault()->setStringForKey(GP_USERNAME, _gpAccountname);
    }
#endif
    
    string _fb = pUserInfo->valueForKey("facebook")->getCString();
    string _localFB = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID,"");
    if (_fb != _localFB) {
        CCUserDefault::sharedUserDefault()->setStringForKey(FB_USERID, _fb);
    }
    string _fbAccountname = pUserInfo->valueForKey("facebookAccountName")->getCString();
    string _localFBAccountname = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERNAME,"");
    if (_fbAccountname != _localFBAccountname) {
        CCUserDefault::sharedUserDefault()->setStringForKey(FB_USERNAME, _fbAccountname);
    }
    playerInfo.m_platFormBindInfo.clear();
    if(GlobalData::shared()->isWeiboEnabled()){
        playerInfo.m_platFormBindInfo[WEIBO].bindPf = "weibo";
    }
    if(GlobalData::shared()->isVKEnabled()){
        playerInfo.m_platFormBindInfo[VK].bindPf = "vk";
    }
    if(pUserInfo->objectForKey("bindInfo")){
        CCLog("bindInfo:");
        auto arr = dynamic_cast<CCArray*>(pUserInfo->objectForKey("bindInfo"));
        if(arr){
            CCObject *obj;
            CCARRAY_FOREACH(arr, obj) {
                CCDictionary *bindDict = _dict(obj);
                std::string bindPf = bindDict->valueForKey("bindPf")->getCString();
                CCLog("bindPf:%s", bindPf.c_str());
                int type = getBindType(bindPf);
                if(playerInfo.m_platFormBindInfo.find(type) == playerInfo.m_platFormBindInfo.end()){
                    playerInfo.m_platFormBindInfo[type] = UserPlatformInfo();
                }
                playerInfo.m_platFormBindInfo[type].parse(bindDict);
            }
        }
    }
    
    string _device = pUserInfo->valueForKey("deviceId")->getCString();
    string _localDevice = CCUserDefault::sharedUserDefault()->getStringForKey(DEVICE_UID,"");
    if (_device != _localDevice) {
        CCUserDefault::sharedUserDefault()->setStringForKey(DEVICE_UID, _device);
    }
    string _mail = pUserInfo->valueForKey("email")->getCString();
    string _localMail = CCUserDefault::sharedUserDefault()->getStringForKey(CUSTOM_UID,"");
    if (_mail != _localMail) {
        CCUserDefault::sharedUserDefault()->setStringForKey(CUSTOM_UID, _mail);
    }
    int _emailConfirm = pUserInfo->valueForKey("emailConfirm")->intValue();
    int _localEmailConfirm = CCUserDefault::sharedUserDefault()->getIntegerForKey(Email_Confirm,0);
    if (_emailConfirm != _localEmailConfirm) {
        CCUserDefault::sharedUserDefault()->setIntegerForKey(Email_Confirm, _emailConfirm);
    }
    
    playerInfo.roomName = "";
    if (params->objectForKey("roomName")) {
        playerInfo.roomName = params->valueForKey("roomName")->getCString();
    }
    
    GlobalData::shared()->focusBuildId = -1;
    //uuid
    if(pUserInfo->objectForKey("uuid")){
        string _uuid = pUserInfo->valueForKey("uuid")->getCString();
        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID, _uuid);
    }
    if(playerInfo.uid != "")
        CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, playerInfo.uid);
    CCUserDefault::sharedUserDefault()->flush();
    GlobalData::shared()->contryResType = 0;
    if (playerInfo.country == 1) {
        GlobalData::shared()->contryResType = 0;
    }
    else if (playerInfo.country == 2)
    {
        GlobalData::shared()->contryResType = 1;
    }
    else if (playerInfo.country == 3)
    {
        GlobalData::shared()->contryResType = 2;
    }
    GlobalData::shared()->contryResType = 2;
    
    
    // add midnight update
    GameController::getInstance()->addMidnightObserver(0.0);
    
    CCDictionary* vipInfo = dynamic_cast<CCDictionary*>(params->objectForKey("vip"));
    if (vipInfo)
    {
        playerInfo.vipPoints = vipInfo->valueForKey("score")->intValue();
        playerInfo.vipEndTime = vipInfo->valueForKey("vipEndTime")->doubleValue()/1000;
        if (playerInfo.vipEndTime>0) {
            playerInfo.vipEndTime = GlobalData::shared()->changeTime(playerInfo.vipEndTime);
        }
        playerInfo.nextDayLoginVipReward = vipInfo->valueForKey("nextDayScore")->intValue();
        playerInfo.consecutiveLoginDays = vipInfo->valueForKey("loginDays")->intValue();
        if(vipInfo->objectForKey("spoint")){
            playerInfo.SVIPPoint = vipInfo->valueForKey("spoint")->intValue();
        }
        if(vipInfo->objectForKey("slevel")){
            playerInfo.SVIPLevel = vipInfo->valueForKey("slevel")->intValue();
        }
        if(VipUtil::isSVIP() && playerInfo.vipPoints>atoi(getPropById("7010", "point").c_str())){
            ToolController::getInstance()->startTrimVipPoint();
        }
        playerInfo.updateVipEffect(true);
    }
    if(params->objectForKey("firstPayReward")){
        GlobalData::shared()->payRewardInfo.parse(params);
        
    }
    
    if(pUserInfo->objectForKey("guideStep")){
        GuideController::share()->m_netSaveStep = pUserInfo->valueForKey("guideStep")->getCString();
    }
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::setCurrentUserId(playerInfo.uid);
    bool isNewUser = playerInfo.level <= 2;
    ChatServiceCocos2dx::initDatabase(isNewUser);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ChatServiceCocos2dx::setPlayerInfo();
    ChatServiceCocos2dx::setPlayerAllianceInfo(playerInfo.allianceInfo.shortName.c_str(),playerInfo.getAllianceId().c_str(),playerInfo.allianceInfo.rank,playerInfo.isfirstJoin);
    ChatServiceCocos2dx::initDatabase();
#endif
    
    //登陆成功android记录玩家uid，用于xingcloud数据统计
    GameController::getInstance()->platformCollectUserInfo(GlobalData::shared()->playerInfo.uid,GlobalData::shared()->playerInfo.name,CC_ITOA(GlobalData::shared()->playerInfo.level));
    //登录成功事件，解析fb广告的deeplink
    string deepLink = CCUserDefault::sharedUserDefault()->getStringForKey("COKDeepLink","");
    string cokfbad = "";
    CCLog("deeplink = %s",deepLink.c_str());
    if(deepLink!=""){
        //解析key为cokfbad的值
        CCArray* deepLinkSplit = CCCommonUtils::split(deepLink.c_str(), "?");
        if(deepLinkSplit->count() > 1){
            string params = ((CCString*)deepLinkSplit->objectAtIndex(1))->getCString();
            CCArray* paramsSplit = CCCommonUtils::split(params.c_str(), "&");
            if (paramsSplit->count() > 1) {
                for (int index = 0; index < paramsSplit->count(); index++) {
                    string paramDetail = ((CCString*)paramsSplit->objectAtIndex(index))->getCString();                    
                    long pos = paramDetail.find("=");
                    if(pos > 0){
                        string paramKey = paramDetail.substr(0,pos);
                        string paramValue = paramDetail.substr(pos + 1,paramDetail.size() - pos - 1);
                        if (paramKey == "cokfbad") {
                            cokfbad = paramValue;
                            break;
                        }
                    }
                }
            }
        }
        CCLog("cokfbad cokfbad=%s",cokfbad.c_str());
    }
    AppLibHelper::triggerEventLoginComplete(GlobalData::shared()->playerInfo.uid,GlobalData::shared()->playerInfo.name,CC_ITOA(GlobalData::shared()->playerInfo.level),cokfbad);
    
    //地区价格
    if (params->objectForKey("countryPrice")) {
        CCDictionary *dict = dynamic_cast<CCDictionary*>(params->objectForKey("countryPrice"));
        string moneyStr = "";
        if(dict->objectForKey("Money")){
            moneyStr = dict->valueForKey("Money")->getCString();
        }
        if(dict->objectForKey("gold_1")){
            string str = moneyStr +" "+ dict->valueForKey("gold_1")->getCString();
            PayController::getInstance()->countryPriceMap["gold_1"] = str;
        }
        if(dict->objectForKey("gold_2")){
            string str = moneyStr +" "+ dict->valueForKey("gold_2")->getCString();
            PayController::getInstance()->countryPriceMap["gold_2"] = str;
        }
        if(dict->objectForKey("gold_3")){
            string str = moneyStr +" "+ dict->valueForKey("gold_3")->getCString();
            PayController::getInstance()->countryPriceMap["gold_3"] = str;
        }
        if(dict->objectForKey("gold_4")){
            string str = moneyStr +" "+ dict->valueForKey("gold_4")->getCString();
            PayController::getInstance()->countryPriceMap["gold_4"] = str;
        }
        if(dict->objectForKey("gold_5")){
            string str = moneyStr +" "+ dict->valueForKey("gold_5")->getCString();
            PayController::getInstance()->countryPriceMap["gold_5"] = str;
        }
        if(dict->objectForKey("gold_8")){
            string str = moneyStr +" "+ dict->valueForKey("gold_8")->getCString();
            PayController::getInstance()->countryPriceMap["gold_8"] = str;
        }
    }
}

void CCCommonUtils::initMails(CCDictionary *params){
    //邮件列表初始化
    CCArray* mailArr = dynamic_cast<CCArray*>(params->objectForKey("mails"));
    if(mailArr){
        int length = mailArr->count();
        if(length>0 && GlobalData::shared()->mailList.size()>0){
            GlobalData::shared()->mailList.clear();
        }
        CCDictionary* info = NULL;
        for(int i = 0; i < length; i++){
            info = _dict(mailArr->objectAtIndex(i));
            MailController::getInstance()->addMail(info,false,false);
        }
    }
    
    CCDictionary* mailcountobj = _dict(params->objectForKey("mailCount"));
    GlobalData::shared()->mailCountObj.saveR = mailcountobj->valueForKey("saveR")->intValue();
    GlobalData::shared()->mailCountObj.perR = mailcountobj->valueForKey("perR")->intValue();
    GlobalData::shared()->mailCountObj.sysR = mailcountobj->valueForKey("sysR")->intValue();
    GlobalData::shared()->mailCountObj.saveT = mailcountobj->valueForKey("saveT")->intValue();
    GlobalData::shared()->mailCountObj.sysT = mailcountobj->valueForKey("sysT")->intValue();
    GlobalData::shared()->mailCountObj.perT = mailcountobj->valueForKey("perT")->intValue();
    GlobalData::shared()->mailCountObj.upR = mailcountobj->valueForKey("upR")->intValue();
    
    GlobalData::shared()->mailCountObj.studioR = mailcountobj->valueForKey("studioR")->intValue();
    GlobalData::shared()->mailCountObj.studioT = mailcountobj->valueForKey("studioT")->intValue();
    GlobalData::shared()->mailCountObj.fightR = mailcountobj->valueForKey("fightR")->intValue();
    GlobalData::shared()->mailCountObj.fightT = mailcountobj->valueForKey("fightT")->intValue();
    GlobalData::shared()->mailCountObj.modR = mailcountobj->valueForKey("modR")->intValue();
    GlobalData::shared()->mailCountObj.modT = mailcountobj->valueForKey("modT")->intValue();
    GlobalData::shared()->mailCountObj.total = mailcountobj->valueForKey("total")->intValue();
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MAIL_LIST_CHANGE);
    
    return;
}

void CCCommonUtils::initTrain(CCDictionary *params){
    //训练的英雄
}

void CCCommonUtils::initResource(CCDictionary *params){
    //资源初始化
    CCDictionary* cityInfo = _dict(params->objectForKey("resource"));
    GlobalData::shared()->resourceInfo.setResourceData(cityInfo);
}

void CCCommonUtils::initEnemyInfo(CCDictionary *params){
    //世界信息初始化
    CCDictionary* worldDict = _dict(params->objectForKey("world"));
    EnemyInfoController::getInstance()->initEnemyData(worldDict);
    if (worldDict->objectForKey("srcServerPointId")) {
        int srcServerPointId = worldDict->valueForKey("srcServerPointId")->intValue();
        GlobalData::shared()->playerInfo.srcServerPointId = srcServerPointId;
    }
}

void CCCommonUtils::initAffair(CCDictionary *params){
//    CCArray *affairArr = dynamic_cast<CCArray*>(params->objectForKey("affairs"));
//    if(affairArr){
//        AffairsController::getInstance()->addAffairs(affairArr);
//        CCDictionary *affair = dynamic_cast<CCDictionary*>(params->objectForKey("affairsValue"));
//        AffairsController::getInstance()->peopleReward = affair->valueForKey("reward")->getCString();
//        AffairsController::getInstance()->setPeople(affair->valueForKey("people")->intValue());
//        AffairsController::getInstance()->setRefreshTime(affair->valueForKey("time")->doubleValue() / 1000, NORMAL_AFFAIR);
//        AffairsController::getInstance()->setRefreshTime(affair->valueForKey("time")->doubleValue() / 1000, LEAGUE_AFFAIR);
//    }
}

void CCCommonUtils::initWall(CCDictionary *params){
    //城墙信息初始化
    CCDictionary* dictWall = _dict(params->objectForKey("wall"));
    if (dictWall->objectForKey("autoType")) {
        TroopsController::getInstance()->m_autoType = dictWall->valueForKey("autoType")->intValue();
    }
    if (dictWall->objectForKey("defense")) {
        TroopsController::getInstance()->initDefenceData(_dict(dictWall->objectForKey("defense")));
    }
}

void CCCommonUtils::initActivity(CCDictionary *params){
    CCDictionary* scoreAct = _dict(params->objectForKey("scoreAct"));
    ActivityController::getInstance()->eventInfo.parse(scoreAct);
    
    CCArray* activityEvents = dynamic_cast<CCArray*>(params->objectForKey("activity"));
    ActivityController::getInstance()->paraseEventObj(activityEvents);
    
    CCDictionary* mine = _dict(params->objectForKey("mine"));
    SocialController::getInstance()->parse(mine);
    
    CCDictionary* trialDic = _dict(params->objectForKey("trialField"));
    ActivityController::getInstance()->parseTrialInfo(trialDic);
}

void CCCommonUtils::initQueue(CCDictionary *params){
    //建造队列信息初始化
    QueueController::getInstance()->initQueueData(params);
    QueueController::getInstance()->initLoginFinishQueues(params);
}

void CCCommonUtils::initBuilding(CCDictionary *params){
    //建筑信息初始化
    FunBuildController::getInstance()->retDataFromServer(params);
}
void CCCommonUtils::initQijiBuilding(CCDictionary *params){
    //奇迹建筑信息初始化
   // FunBuildController::getInstance()->retDataFromServerforQiji(params);
}
void CCCommonUtils::initDragonBuilding(CCDictionary *params){
    //初始化龙建筑信息
//    DragonBuildingController::getInstance()->retDataFromServer(params);
}
void CCCommonUtils::initDragonsInfo(CCDictionary *params){
    //初始化龙信息
//    DragonController::getInstance()->setDragonDataFromServer(params);
}

void CCCommonUtils::initAppLibHelper(CCDictionary *params){
    //主城等级统计
    AppLibHelper::triggerEventMainBuildingLevel(FunBuildController::getInstance()->getMainCityLv());
    AppLibHelper::updatePlatformScore(FunBuildController::getInstance()->getMainCityLv());
}

void CCCommonUtils::initGeneral(CCDictionary *params){
    //将军信息初始化
    CCArray *generalArr = NULL;
    generalArr = dynamic_cast<CCArray*>(params->objectForKey("userGenerals"));
    GeneralManager::getInstance()->initLordDetails();
    if (generalArr != NULL && generalArr->count() > 0) {
        std::map<std::string, SkillCDInfo*>::iterator cdIt;
        for(cdIt = GeneralManager::getInstance()->SkillCDMap.begin(); cdIt != GeneralManager::getInstance()->SkillCDMap.end(); cdIt++){
            cdIt->second->release();
        }
        GeneralManager::getInstance()->SkillCDMap.clear();
        int length = generalArr->count();
        CCDictionary *generalItem = NULL;
        for (int i=0; i<length; i++) {
            generalItem = _dict(generalArr->objectAtIndex(i));
            GeneralManager::getInstance()->addGeneral(generalItem);
            if(i==0 && generalItem->objectForKey("skill")){
                GeneralInfo* info = &GlobalData::shared()->generals.begin()->second;
                info->parseCDTime(generalItem);
            }
        }
        GeneralManager::getInstance()->resetGeneralSkillEffectValue();
    }
}

void CCCommonUtils::initAllianceSceice(CCDictionary *params){
    //联盟科技初始化
    CCArray *alScienceArr = dynamic_cast<CCArray*>(params->objectForKey("allianceScience"));
    ScienceController::getInstance()->initAllianceScienceData(alScienceArr);
}

void CCCommonUtils::initArmy(CCDictionary *params){
    //army初始化
    CCArray *armyArr = dynamic_cast<CCArray*>(params->objectForKey("army"));
    CCObject *armyObject;
    CCARRAY_FOREACH(armyArr, armyObject){
        CCDictionary *armyDic = _dict(armyObject);
        if( armyDic== nullptr) {
            continue;
        }
        std::string itemId = armyDic->valueForKey("id")->getCString();
        int unLockLv = 0;
        if (GlobalData::shared()->armyList.find(itemId) != GlobalData::shared()->armyList.end()) {
            unLockLv = GlobalData::shared()->armyList[itemId].unlockLevel;
        }
        GlobalData::shared()->armyList[itemId] = ArmyInfo(armyDic);
        GlobalData::shared()->armyList[itemId].unlockLevel = unLockLv;
    }
}

void CCCommonUtils::initFort(CCDictionary *params){
    //fort 初始化
    CCArray *fortArr = dynamic_cast<CCArray*>(params->objectForKey("fort"));
    CCObject *fortObject;
    CCARRAY_FOREACH(fortArr, fortObject){
        CCDictionary *forDic = _dict(fortObject);
        std::string itemId = forDic->valueForKey("id")->getCString();
        int unLockLv = 0;
        if (GlobalData::shared()->fortList.find(itemId) != GlobalData::shared()->fortList.end()) {
            unLockLv = GlobalData::shared()->fortList[itemId].unlockLevel;
        }
        GlobalData::shared()->fortList[itemId] = ArmyInfo(forDic);
        GlobalData::shared()->fortList[itemId].isArmy = false;
        GlobalData::shared()->fortList[itemId].unlockLevel = unLockLv;
    }
}

void CCCommonUtils::initTreat(CCDictionary *params){
    //treat 初始化
    CCArray *treatArr = dynamic_cast<CCArray*>(params->objectForKey("hospital"));
    CCObject *treatObject;
    CCARRAY_FOREACH(treatArr, treatObject){
        CCDictionary *treatDic = _dict(treatObject);
        std::string itemId = treatDic->valueForKey("id")->getCString();
        GlobalData::shared()->treatList[itemId] = TreatInfo(treatDic);
    }
}

void CCCommonUtils::initExchange(CCDictionary *params){
    //兑换金币初始化goldExchangeList
    CCArray *goldExchangeArr = dynamic_cast<CCArray*>(params->objectForKey("exchange"));
    if(GlobalData::shared()->goldExchangeList.empty()){
        CCObject *obj;
        CCARRAY_FOREACH(goldExchangeArr, obj){
            CCDictionary* dict = _dict(obj);
            GoldExchangeItem* goldExchangeItem = new GoldExchangeItem(dict);
            GlobalData::shared()->goldExchangeList[goldExchangeItem->id] = goldExchangeItem;
        }
        GlobalData::shared()->goldExchageRefreshStamp = GlobalData::shared()->getWorldTime();
    }
}

void CCCommonUtils::initEquip(CCDictionary *params){
    //装备初始化
    if (params->objectForKey("equip")) {
        EquipmentController::getInstance()->retInitData(params);
    }
}

void CCCommonUtils::initKnightTitle(CCDictionary *params){
    //骑士称号
    if (params->objectForKey("knight_title")) {
        KnightTitleController::getInstance()->retInitData(params);
    }
}

void CCCommonUtils::initItems(CCDictionary *params){
    //新商店和背包
    if (params->objectForKey("items")) {
        ToolController::getInstance()->initToolData(params);
    }
    //保存礼包道具奖励内容数据
    if (params->objectForKey("store")) {
        ToolController::getInstance()->initToolReward(params);
    }
}

void CCCommonUtils::initScience(CCDictionary *params){
    //science info init
    GlobalData::shared()->scienceInfo.initSelf();
    if (params->objectForKey("science")) {
        auto scienceInfo = dynamic_cast<CCArray*>(params->objectForKey("science"));
        GlobalData::shared()->scienceInfo.initScienceState(scienceInfo);
    }
}

void CCCommonUtils::initTalent(CCDictionary *params){
    //天赋数据
    if (params->objectForKey("talent")) {
    }
}

void CCCommonUtils::initGCMReward(CCDictionary *params){
    //初始化GCM奖励
    GCMRewardController::getInstance()->initGCMRewardData(params);
}

void CCCommonUtils::initNewPlayerReward(CCDictionary *params){
    if (params->objectForKey("newSignIn")) {
        CCDictionary* newSignIn = dynamic_cast<CCDictionary*>(params->objectForKey("newSignIn"));
        if (newSignIn) {
            PortActController::getInstance()->initNewPlayerDailyRwdData(newSignIn);
        }
    }
}

void CCCommonUtils::initTask(CCDictionary *params){
    CCArray* tasks = dynamic_cast<CCArray*>(params->objectForKey("task"));
    QuestController::getInstance()->initQuest(tasks);
    CCArray* ckfTask = dynamic_cast<CCArray*>(params->objectForKey("ckfTask"));
    QuestController::getInstance()->initCKFTask(ckfTask);
    if (params->objectForKey("taskCompleteNum")) {
        QuestController::getInstance()->m_stageNum = params->valueForKey("taskCompleteNum")->intValue();
    }
    if (isTestPlatformAndServer("princess_quest")) {
        CCArray* princessTask = dynamic_cast<CCArray*>(params->objectForKey("princessTask"));
        PrincessQuestController::getInstance()->initQuest(princessTask);
    }
}

void CCCommonUtils::initServerStopTime(CCDictionary *params){
    //停服更新计时
    if(params->objectForKey("serverStoptime")){
        auto time = params->valueForKey("serverStoptime")->doubleValue();
        long curTime = GlobalData::shared()->getWorldTime();
        auto m_countDown = GlobalData::shared()->changeTime(time/1000);
        auto m_time = (long)floor( m_countDown - curTime );
        string delayTime = CC_ITOA(m_time/60);
        string tip = CCString::createWithFormat("%s",_lang_1("105324", delayTime.c_str()))->getCString();
        CCCommonUtils::flySystemUpdateHint(time, true,tip,FLY_HINT_LOGIN);
    }
}

void CCCommonUtils::initMixInfo(CCDictionary *params){
    CCDictionary* mixedInfo = dynamic_cast<CCDictionary*>(params->objectForKey("mixedInfo"));
    GlobalData::shared()->isOpenEvaluate = false;
    GlobalData::shared()->popupTimes = 7;
    if (mixedInfo)
    {
        GlobalData::shared()->isOpenEvaluate = mixedInfo->valueForKey("scoreSwitch")->boolValue();
        if (mixedInfo->objectForKey("popupTimes"))
        {
            GlobalData::shared()->popupTimes = mixedInfo->valueForKey("popupTimes")->intValue();
        }
        if (mixedInfo->objectForKey("dailyTaskId"))
        {
            DailyActiveController::shared()->m_HaveDailyTaskIds = mixedInfo->valueForKey("dailyTaskId")->getCString();
        }
        if (mixedInfo->objectForKey("refineTimes"))
        {
            EquipmentController::getInstance()->m_refineTimes = mixedInfo->valueForKey("refineTimes")->intValue();
        }
    }
}

void CCCommonUtils::initFBShare(CCDictionary *params){
    CCDictionary* fbShare = _dict(params->objectForKey("fbShare"));
    if (params->objectForKey("fbShare")) {
        CCArray* shareArr = dynamic_cast<CCArray*>(params->objectForKey("fbShare"));
        if (shareArr) {
            int sNum = shareArr->count();
            for (int i=0; i<sNum; i++) {
                auto oneDic = _dict(shareArr->objectAtIndex(i));
                oneDic->retain();
                string idkey = oneDic->valueForKey("id")->getCString();
                GlobalData::shared()->shareFbmap[idkey] = oneDic;
            }
        }
    }
}

void CCCommonUtils::getGameDataAfterInit(){
//    if(isServerCrossOpen()){
//        WorldController::getInstance()->getServerList();
//    }
    WorldActivityGetCommand *cmd = new WorldActivityGetCommand();
    cmd->sendAndRelease();
    
    ShowStatusItemCommand *statusCmd = new ShowStatusItemCommand();
    statusCmd->sendAndRelease();
    
    AchievementController::getInstance()->purgeData();
    AchievementController::getInstance()->getDataFromServer();
    AchievementController::getInstance()->getMedalDataFormServer();
    
    bool isInAlliance = true;
    isInAlliance = GlobalData::shared()->playerInfo.isInAlliance();
    
    //获取联盟日常任务信息
    if(AllianceDailyController::getInstance()->isSwitchOpen() && isInAlliance){
        AllianceDailyController::getInstance()->removeAllMap();
        AllianceDailyController::getInstance()->startGetRecord();
        AllianceDailyController::getInstance()->startGetDailyQuests();
    }
    //favor清理，在进入世界地图时加载数据
    if(!WorldController::getInstance()->m_favoriteTiles.empty()){
         WorldController::getInstance()->m_favoriteTiles.clear();
    }
    
    DailyActiveController::shared()->getDataFormServer();
    
    
    ChatController::getInstance()->setDisableTranslateLang();
    //zym 2015.10.23 先于聊天初始化
    UIComponent::getInstance();
    ChatController::getInstance()->getKingOfServer();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::initChatService();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
     ChatServiceCocos2dx::gettingChatMsgWithAppOpen();
#endif

    ChatController::getInstance()->getHandledRedPackages();
    ChatController::getInstance()->getRedPackageTime();
}

void CCCommonUtils::doWhenInitComplete(CCDictionary *params){
    if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if (GlobalData::shared()->isGoogle()) {
            PayController::getInstance()->initGoogle();
        }
#endif
        if (params->objectForKey("first_login")) {
            AppLibHelper::triggerEventCompletedRegistration();
        }
        if (params->objectForKey("two_days_login")) {
            AppLibHelper::sendAdjustTrack("two_days_login");
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_INIT_FINISH);
    }
    else {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    }
    GlobalData::shared()->isInitFlag = true;
    GlobalData::shared()->isInitFinish = true;
    GlobalData::shared()->isInDataParsing = false;
    
    
    std::string collect_bas = CCUserDefault::sharedUserDefault()->getStringForKey("collect_bas", "");
    CCCommonUtils::splitString(collect_bas, "|", GlobalData::shared()->batteryStates);
//    CCLog("GlobalData::shared()->batteryStates.size:::::::::%d",GlobalData::shared()->batteryStates.size());
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(GlobalData::shared()->func_checkEmulator_open == true)
    {
//        string handSetInfo = cocos2d::extension::CCDevice::getHandSetInfo();
////        CCLog("_handSetInfo::::::%s",handSetInfo.c_str());
//        
//        vector<string> handSetInfoV;
//        CCCommonUtils::splitString(handSetInfo, ",", handSetInfoV);
//        bool openQemu = false;
//        if (handSetInfoV.size() > 0) {
//            for (int i=0; i<handSetInfoV.size(); i++) {
//                string it = handSetInfoV[i];
//                vector<string> handSetInfoV2;
//                CCCommonUtils::splitString(it, ":", handSetInfoV2);
//                std::string str = handSetInfoV2[0].c_str();
//                if (str.compare("SYSVersion") == 0) {
//                    string it2 =handSetInfoV2[1].c_str();
//                    vector<string> handSetInfoV3;
//                    CCCommonUtils::splitString(it2, ":", handSetInfoV3);
//                    if (atoi(handSetInfoV3[0].c_str()) <= 4 ) {
//                        openQemu = true;
//                    }
//                }
//            }
//        }
//        if(openQemu)
//        {
//            int bkpt = qemuBkpt();
//            std::string qemuBkpt= CC_ITOA(bkpt);
//            //        CCLog("qemuBkpt:::::%s", qemuBkpt.c_str());
//            CCUserDefault::sharedUserDefault()->setStringForKey("qemuBkpt", qemuBkpt);
//        }
        
    }
    
#endif
    
    std::string device = CCUserDefault::sharedUserDefault()->getStringForKey("phoneDevice", "");
    if(device != ""){
        std::string qemuBkpt = CCUserDefault::sharedUserDefault()->getStringForKey("qemuBkpt", "");
        PhoneDeviceCommand *cmd = new PhoneDeviceCommand(device,qemuBkpt,collect_bas);
        cmd->sendAndRelease();
//        CCLog("%s", ("phoneDevice = " + device).c_str());
    }
    
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(GlobalData::shared()->func_checkEmulator_open == true)
    {
        DeviceInfoController::getInstance()->startBatteryStateCollect();
    }
    
#endif
}

void CCCommonUtils::initTestSwitches(CCDictionary *params)
{
    auto arr = dynamic_cast<CCArray*>(params->objectForKey("switches"));
    if( arr==NULL ){
        return;
    }
    
    GlobalData::shared()->testSwitchMap.clear();
    CCDictionary *item = NULL;
    for (int i=0; i<arr->count(); i++) {
        item = _dict(arr->objectAtIndex(i));
        string name = item->valueForKey("name")->getCString();
        int stat = item->valueForKey("stat")->intValue();
        string version = item->valueForKey("appVersion")->getCString();
        GlobalData::shared()->testSwitchMap[name] = stat;
        GlobalData::shared()->testSwitchVersionMap[name] = version;
    }
}

void CCCommonUtils::initAssets(CCDictionary *params)
{
    auto assets = dynamic_cast<CCDictionary*>(params->objectForKey("assets"));
    if( assets==NULL ){
        return;
    }
    long winPoint = (long)(assets->valueForKey("winPoint")->doubleValue());
    GlobalData::shared()->playerInfo.winPoint = winPoint;
    
}

void CCCommonUtils::initSendHeart(Dictionary* params)
{
    auto heart_allsender = dynamic_cast<__String*>(params->objectForKey("heart_allsender"));
    auto heart_effecttype = dynamic_cast<__String*>(params->objectForKey("heart_effecttype"));
    auto heart_time = dynamic_cast<__String*>(params->objectForKey("heart_time"));
    if (heart_allsender && heart_effecttype && heart_time)
    {
        int type = heart_effecttype->intValue();
        int time = heart_time->doubleValue() / 1000;
        string sender = heart_allsender->getCString();
        if (sender != "-1" && type == 1) {
            ToolController::getInstance()->m_heartTellMap.clear();
            CCCommonUtils::splitString(sender, ",", ToolController::getInstance()->m_heartTellMap[type]);
            ToolController::getInstance()->m_heartEndTime[WorldController::getIndexByPoint(WorldController::getInstance()->selfPoint)] = time;
        }
    }
}

void CCCommonUtils::initCheckEmulatorData(CCDictionary *params)
{
    auto eData = dynamic_cast<CCDictionary*>(params->objectForKey("checkOpen"));
    if( eData==NULL ){
        return;
    }
    bool checkQemu = false;//eData->valueForKey("checkQemu")->boolValue();
    if(eData->objectForKey("cQ"))
    {
        checkQemu = eData->valueForKey("cQ")->boolValue();
    }
    bool checkBattery = false;
    if(eData->objectForKey("cB"))
    {
        checkBattery = eData->valueForKey("cB")->boolValue();
    }
    float waitTime = 120.0f;
    if (eData->objectForKey("wT")) {
        waitTime =eData->valueForKey("wT")->floatValue();
    }
    string cProcess = "";
    if (eData->objectForKey("cP")) {
        cProcess =eData->valueForKey("cP")->getCString();
    }
    
    
    if (checkQemu) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        
//        int bkpt = qemuBkpt();
//        std::string qemuBkpt= CC_ITOA(bkpt);
////        CCLog("qemuBkpt:::::%s", qemuBkpt.c_str());
//        CCUserDefault::sharedUserDefault()->setStringForKey("qemuBkpt", qemuBkpt);
        
#endif
    }

    if(checkBattery)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        
        DeviceInfoController::getInstance()->startBatteryStateCollect();
        
#endif
    }
    
    if (cProcess.compare("") != 0) {
        DeviceInfoController::getInstance()->collectProcess(cProcess);
    }

    DeviceInfoController::getInstance()->waitToSendCheckCmd(waitTime);
}

void CCCommonUtils::initImperialSceneTexture(CCDictionary *params, int index)
{
    if( index == InitImperialSceneBegin ) {
        CCLoadSprite::doResourceByImperialIndex(0, true);
    } else if ( index == InitImperialSceneBegin + 1 ) {
        CCLoadSprite::doResourceByImperialIndex(1, true);
    } else if ( index == InitImperialSceneBegin + 2 ) {
        CCLoadSprite::doResourceByImperialIndex(2, true);
    } else if ( index == InitImperialSceneBegin + 3 ) {
        CCLoadSprite::doResourceByImperialIndex(3, true);
    } else if ( index == InitImperialSceneBegin + 4 ) {
        CCLoadSprite::doResourceByImperialIndex(4, true);
    } else if ( index == InitImperialSceneBegin + 5 ) {
        CCLoadSprite::doResourceByImperialIndex(10, true);
    } else if ( index == InitImperialSceneBegin + 6 ) {
        CCLoadSprite::doResourceByImperialIndex(11, true);
    } else if ( index == InitImperialSceneBegin + 7 ) {
        CCLoadSprite::doResourceByImperialIndex(12, true);
    } else if ( index == InitImperialSceneBegin + 8 ) {
        CCLoadSprite::doResourceByImperialIndex(13, true);
    } else if ( index == InitImperialSceneBegin + 9 ) {
        CCLoadSprite::doResourceByImperialIndex(14, true);
    } else if ( index == InitImperialSceneBegin + 10 ) {
        CCLoadSprite::doResourceByImperialIndex(15, true);
    } else if ( index == InitImperialSceneBegin + 11 ) {
            CCLoadSprite::doResourceByImperialIndex(16, true);
    } else if ( index == InitImperialSceneBegin + 12 ) {
        CCLoadSprite::doResourceByImperialIndex(30, true);
    } else if ( index == InitImperialSceneBegin + 13 ) {
        CCLoadSprite::doResourceByImperialIndex(31, true);
        
    } else if ( index == InitImperialSceneBegin + 14 ) {
        CCLoadSprite::doResourceByCommonIndex(7, true);
    } else if ( index == InitImperialSceneBegin + 15) {
        CCLoadSprite::doResourceByCommonIndex(8, true);
    } else if ( index == InitImperialSceneBegin + 16) {
        CCLoadSprite::doResourceByCommonIndex(11, true);
    }
}

void CCCommonUtils::initOperation(int initType, CCDictionary *params){
    if(initType == InitStart || initType == InitEnd || params == nullptr){
        return;
    }
    
    CCLOG(" CCCommonUtils::initOperation begin!!! type = %d", initType);
    
    auto start = clock();
    if(initType == InitTime){
        initTime(params);
    }
    else if(initType == InitEffect){
        initEffect(params);
        
    }
    else if(initType == InitTranslation){
        initTranslation(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initTranslation");
        }
    }
    else if(initType == InitGaid){
        initGaid(params);
    }
    else if(initType == InitEffectState){
        initEffectState(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initEffectState");
        }
    }
    else if(initType == InitStatus){
        initStatus(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initStatus");
        }
    }
    else if(initType == InitWorld){
        initWorld(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initWorld");
        }
    }
    else if(initType == InitPray){
        initPray(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initPray");
        }
    }
    else if(initType == InitLord){
        initLord(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initLord");
        }
    }
    else if(initType == InitPushSetting){
        initPushSetting(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initPushSetting");
        }
    }
    else if(initType == InitBusinessMan){
        initBusinessMan(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initBusinessMan");
        }
    }
    else if(initType == InitMonthLyCards){
        initMonthLyCards(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initMonthLyCards");
        }
    }
    else if(initType == InitPlayerInfo){
        initPlayerInfo(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initPlayerInfo");
        }
    }
    else if(initType == InitRecharge) {
        initFreshRecharge(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initFreshRecharge");
        }
    }
    else if(initType == InitMails){
        initMails(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initMails");
        }
    }
    else if(initType == InitTrain){
        initTrain(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initTrain");
        }
    }
    else if(initType == InitResource){
        initResource(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initResource");
        }
    }
    else if(initType == InitEnemyInfo){
        initEnemyInfo(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initEnemyInfo");
        }
    }
    else if(initType == InitConfigData){
        initConfigData(params);
    }
    else if(initType == InitAffair){
        initAffair(params);
    }
    else if(initType == InitWall){
        initWall(params);
    }
    else if(initType == InitActivity){
        initActivity(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initActivity");
        }
    }
    else if(initType == InitQueue){
        initQueue(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initQueue");
        }
    }
    else if(initType == InitBuilding){
        initBuilding(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initBuilding");
        }
    }
    else if(initType == InitDragonBuilding){
        initDragonBuilding(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initDragonBuilding");
        }
    }
    else if(initType == InitDragonsInfo){
        initDragonsInfo(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initDragonsInfo");
        }
    }
    else if(initType == InitAppLibHelper){
        initAppLibHelper(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initAppLibHelper");
        }
    }
    else if(initType == InitGeneral){
        initGeneral(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initGeneral");
        }
    }
    else if(initType == InitAllianceSceice){
        initAllianceSceice(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initAllianceSceice");
        }
    }
    else if(initType == InitArmy){
        initArmy(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initArmy");
        }
    }
    else if(initType == InitFort){
        initFort(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initFort");
        }
    }
    else if(initType == InitTreat){
        initTreat(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initTreat");
        }
    }
    else if(initType == InitExchange){
        initExchange(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initExchange");
        }
    }
    else if(initType == InitEquip){
        initEquip(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initEquip");
        }
    }
    else if(initType == InitKnight){
        initKnightTitle(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initKnightTitle");
        }
    }
    else if(initType == InitItems){
        initItems(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initItems");
        }
    }
    else if(initType == InitScience){
        initScience(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initScience");
        }
    }
    else if(initType == InitTalent){
        initTalent(params);
    }
    else if(initType == InitGCMReward){
        initGCMReward(params);
    }
    else if(initType == InitNewPlayerReward){
        initNewPlayerReward(params);
    }
    else if(initType == InitTask){
        initTask(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "initTask");
        }
    }
    else if(initType == InitServerStopTime){
        initServerStopTime(params);
    }
    else if(initType == InitMixInfo){
        initMixInfo(params);
    }
    else if(initType == InitFBShare){
        initFBShare(params);
    }
    else if(initType == initparticle){
        DynamicResourceController::getInstance()->uncompressLocalZip(1);
    }
    else if(initType == initspine){
        DynamicResourceController::getInstance()->uncompressLocalZip(2);
    }
    else if(initType == DoWhenInitComplete){
        doWhenInitComplete(params);
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_LOADING)
        {
            GameController::getInstance()->setLoadingLog("CCCommonUtils:initOperation", "doWhenInitComplete");
        }
    }
    else if(initType == GetGameDataAfterInit){
        getGameDataAfterInit();
    }
    else if(initType == InitOpenUUID){
        refreshOpenUUID(params);
    }
    else if(initType == InitTestSwitches){
        initTestSwitches(params);
    }else if(initType == InitAssets){
        initAssets(params);
    }
    else if (initType == InitSendHeart)
    {
        initSendHeart(params);
    }
    else if (initType == InitCheckEmulatorData){
        initCheckEmulatorData(params);
    }
    else if( initType >= InitImperialSceneBegin && initType <= InitImperialSceneEnd ) {
        initImperialSceneTexture(params, initType);
    }
    
    CCLOG(" CCCommonUtils::initOperation end !!! type = %d", initType);
    
}

void CCCommonUtils::initOperationOver()
{
    // 这里加入当数据传递完之后统一处理整合的逻辑
    ScienceController::getInstance()->ParseScienceCfg();
    CCNotificationCenter::getInstance()->postNotification(MSG_LoginInitFinish);
    return;
}

void CCCommonUtils::refreshOpenUUID(CCDictionary *params){
    //通过后台通知更新保存的deviceid的旧值
    if(params->objectForKey("updateDeviceId")){
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
        cocos2d::extension::CCDevice::getOpenUDID();
        CCLOG("deviceId update");
#endif
        
    }
}

void CCCommonUtils::parseData(cocos2d::CCDictionary *params){
    resetData(params);
    int i = InitStart;
    while(i < InitEnd){
        initOperation(i, params);
        i++;
    }
}

int CCCommonUtils::getBindType(std::string bindStr){
    if(bindStr == BIND_QQ){
        return QQ;
    }else if(bindStr == BIND_WEIBO){
        return WEIBO;
    }else if(bindStr == BIND_WEIXIN){
        return WEIXIN;
    }else if(bindStr == BIND_VK){
        return VK;
    }
    return -1;
}

bool CCCommonUtils::payParseData(cocos2d::CCDictionary *params)
{
    int status = params->valueForKey("status")->intValue();
    string orderId = params->valueForKey("orderId")->getCString();
    CCLOG("payment check callback,status is %d",status);
    CCLOG("payment check callback,order id is %s",orderId.c_str());
    switch (status) {
        case 0:{
            PayController::getInstance()->addToPurchaseIdList(orderId);
            // pay check success
            // update gold
            bool isFirstPay = false;
            int intPayTotal = GlobalData::shared()->playerInfo.payTotal;
            UIComponent::getInstance()->updateGold(params->valueForKey("gold")->intValue());
            GlobalData::shared()->playerInfo.payTotal = params->valueForKey("payTotal")->intValue();
            if(intPayTotal<=0&&GlobalData::shared()->playerInfo.payTotal>0){
                isFirstPay = true;
            }
            
            string key = params->valueForKey("itemId")->getCString();
            string itemName = "";
            bool sendGift = false;
            if(params->objectForKey("exchangegift") && params->valueForKey("exchangegift")->boolValue() == true){
                sendGift = true;
            }
            
            if ( GlobalData::shared()->goldExchangeList.find(key.c_str()) != GlobalData::shared()->goldExchangeList.end()) {
                ToolController::getInstance()->m_canBuy = 0;
                map<string, GoldExchangeItem*>::iterator it =GlobalData::shared()->goldExchangeList.find(key.c_str());
                itemName = _lang(it->second->name);
                if(sendGift==false){
                    if(it->second->type == "5"){
                        it->second->bought = true;
                        if(GlobalData::shared()->monthCardInfoList.find(key)!=GlobalData::shared()->monthCardInfoList.end()){
                                GlobalData::shared()->monthCardInfoList[key]->buyCard();
                        }
                    }else if(it->second->type!="1"){
                        it->second->bought = true;
                        PayController::getInstance()->setGoldExchangeSaleBoughtFlag(key, true);
                    }
                    
                }
                if (params->objectForKey("reward")) {
                    CCArray* arr = dynamic_cast<CCArray*>(params->objectForKey("reward"));
                    if(arr){
                        PopupViewController::getInstance()->removeAllPopupView();
                        PopupViewController::getInstance()->addPopupView(FreeListView::create(atoi(key.c_str())));
                        RewardController::getInstance()->retReward(arr, true);
                    }else if (isFirstPay){
                        PopupViewController::getInstance()->removeAllPopupView();
                    }
                }else if (isFirstPay){
                    if(sendGift==false){
                        PopupViewController::getInstance()->removeAllPopupView();
                    }
                }
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(PAYMENT_COMMAND_RETURN, CCString::create(key));
                if (params->objectForKey("exchange")) {
                    
                    CCDictionary* dict = _dict(params->objectForKey("exchange"));
                    GoldExchangeItem* goldExchangeItem = new GoldExchangeItem(dict);
                    map<string, GoldExchangeItem*>::iterator it =GlobalData::shared()->goldExchangeList.find(goldExchangeItem->id);
                    if ( it!= GlobalData::shared()->goldExchangeList.end()){
                        GoldExchangeItem* goldExchangeItemtemp =it->second;
                        delete goldExchangeItemtemp;
                        goldExchangeItemtemp = NULL;
                    }
                    GlobalData::shared()->goldExchangeList[goldExchangeItem->id] = goldExchangeItem;
                    
                    
                }
                if (params->objectForKey("rmId"))
                {
                    string rmidStr = params->valueForKey("rmId")->getCString();
                    vector<string> tv;
                    CCCommonUtils::splitString(rmidStr, ",", tv);
                    if (tv.size() > 0) {
                        for (auto it = tv.begin(); it != tv.end(); ++it) {
                            auto tit = GlobalData::shared()->goldExchangeList.find(*it);
                            if (tit != GlobalData::shared()->goldExchangeList.end()) {
                                if (tit->second) {
                                    delete tit->second;
                                    tit->second = nullptr;
                                }
                                GlobalData::shared()->goldExchangeList.erase(tit);
                            }
                        }
                    }
                }
                if(params->objectForKey("equip")){
                    CCArray *equipArray = dynamic_cast<CCArray*>(params->objectForKey("equip"));
                    if(equipArray){
                        EquipmentController::getInstance()->addEquip(equipArray);
                    }
                }
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GOLDEXCHANGE_LIST_CHANGE);
                
            }
            
            // send log to AF
            AppLibHelper::sendPayLog(params->valueForKey("cost")->getCString(),key);
            AppLibHelper::triggerEventPurchase(params->valueForKey("cost")->getCString(),key);
            if (params->objectForKey("first_pay")) {
                AppLibHelper::sendAdjustTrack("first_pay");
            }
            if (params->objectForKey("first_pay_today")) {
                AppLibHelper::sendAdjustTrack("first_pay_today");
            }
            if (params->objectForKey("repay_add")){
                int addValue = params->valueForKey("repay_add")->intValue();
                RepayController::getInstance()->addPoint(addValue);
            }
            if (sendGift==true){
                if(!itemName.empty() && params->objectForKey("exchangeto")){
                    auto exchangeTo = _dict(params->objectForKey("exchangeto"));
                    if(exchangeTo){
                        if(exchangeTo->objectForKey("receiverName")){
                            string content = _lang_1("101005",itemName.c_str());
                            string toName = exchangeTo->valueForKey("receiverName")->getCString();
                            MailController::getInstance()->sendMail(toName, "", content,"","",false,MAIL_SELF_SEND,"",false);
                        }
                        
                        if(exchangeTo->objectForKey("sendTime") && exchangeTo->valueForKey("sendTime")->doubleValue()>0){
                            if(exchangeTo->objectForKey("receiverId")){
                                LuaController::getInstance()->updateExchangeGiftInfo(exchangeTo->valueForKey("receiverId")->getCString(),key,exchangeTo->valueForKey("sendTime")->doubleValue()/1000);
                                
                            }
                        }
                    }
                }
                LuaController::toSelectUser(key, false, 0);
                YesNoDialog::showYesDialog(_lang("101004").c_str());
//                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFRESH_INVITE_DATA);
            }else{
                if (!params->objectForKey("google_code")) {
                    if (params->objectForKey("reward")) {
                        CCArray* arr = dynamic_cast<CCArray*>(params->objectForKey("reward"));
                        if(arr){
                        }else{
                            YesNoDialog::showYesDialog(_lang("E100076").c_str());//E100076=支付成功！
                        }
                    }else{
                        YesNoDialog::showYesDialog(_lang("E100076").c_str());//E100076=支付成功！
                    }
                }
            }
        }
            break;
        case 1:{
            PayController::getInstance()->removeOrderCache(orderId);
            // pay check fail
            YesNoDialog::showYesDialog(_lang("E100042").c_str());
        }
            break;
        case 2:{
            PayController::getInstance()->addToPurchaseIdList(orderId);
            // order exist
        }
            break;
            
        default:
            CCLOG("Payment net cmd error: error status :%d",status);
            return false;
    }
    return true;
}

bool CCCommonUtils::isServerCrossOpen(){
    return true;
}
bool CCCommonUtils::isPushNotify(int type){
    vector<PushSettingInfo*>::iterator it;
    for (it = GlobalData::shared()->pushSettingVector.begin(); it!=GlobalData::shared()->pushSettingVector.end(); it++) {
        if((*it)->type==type){
            if((*it)->status==0){
                return false;
            }
        }
    }

    return true;
}

std::string CCCommonUtils::getLanguage()
{
    
    std::string preferredLanguage = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_PREFERRED_LANGUAGE,"");
    if(preferredLanguage==""){
        preferredLanguage = LocalController::shared()->getLanguageFileNameBasedOnUserSystem();
    }
    return preferredLanguage;
}

bool CCCommonUtils::hasSpeedEffect(int type){
    map<int, CCDictionary* >::iterator it = ToolController::getInstance()->m_statusItems.find(type);
    double time = 0;
    if(it!=ToolController::getInstance()->m_statusItems.end()){
        time =(it->second->valueForKey("endTime")->doubleValue() - WorldController::getInstance()->getTime())/1000;
    }
    
    return time > 0;
}

double CCCommonUtils::getSpeedEffectTime(int type){
    map<int, CCDictionary* >::iterator it = ToolController::getInstance()->m_statusItems.find(type);
    double speedTime = 0;
    if(it!=ToolController::getInstance()->m_statusItems.end()){
        speedTime =(it->second->valueForKey("endTime")->doubleValue() - WorldController::getInstance()->getTime())/1000;
    }
    return speedTime;
}

//根据用户头像名称获取加载哪个大图
int CCCommonUtils::getBustIdxByName(string name)
{
    if (name=="g008" || name=="g015" || name=="g024" || name=="g026") {
        return 1;
    }else if (name=="g032" || name=="g038" || name=="g041" || name=="g044") {
        return 2;
    }else if (name=="g007" || name=="g012" || name=="g045" || name=="g046") {
        return 3;
    }
    return 1;
}

std::string CCCommonUtils::getLanguageFNByLocalSN(std::string localN){
    string langId = "";
    CCDictionary* dic_all = LocalController::shared()->DBXMLManager()->getGroupByKey("language");
    int idx = 9500;
    int num = dic_all->count();
    if(localN.find("zh_CN")<localN.length()||localN.find("zh-Hans")<localN.length() || localN.find("zh_TW")<localN.length() || localN.find("zh-Hant")<localN.length()){
        localN = "zh";
    }
    for (int i=0; i<num; ++i) {
        CCDictionary* dic_one = _dict(dic_all->objectForKey(CC_ITOA(idx+i)));
        string mark = dic_one->valueForKey("mark")->getCString();
        string lang_ma = dic_one->valueForKey("lang_ma")->getCString();
        string lang_id = dic_one->valueForKey("lang_id")->getCString();
        if(mark!="" && lang_ma!="" && localN.find(lang_ma)<localN.length()){
            langId = lang_id;
            break;
        }
    }
    if(langId==""){
        langId = "115601";
    }
    return _lang(langId);
}

CCSprite *CCCommonUtils::addFilterSprite(std::string fileName, const ccColor3B &color, float bright, float contrast, float saturation){
    auto sprite = FilterSprite::createWithSpriteFrameName(fileName);
    if(!sprite){
        return CCSprite::create("");
    }
    sprite->setParam(color, bright, contrast, saturation);
    return sprite;
}

ccColor3B CCCommonUtils::covertHSBToRGB(int h, float s, float v){
    float r = 0, g = 0, b = 0;
    int i = (int) ((h / 60) % 6);
    float f = (h * 1.0 / 60) - i;
    float p = v * (1.0 - s);
    float q = v * (1.0 - f * s);
    float t = v * (1.0 - (1.0 - f) * s);
    switch (i) {
        case 0:
            r = v;
            g = t;
            b = p;
            break;
        case 1:
            r = q;
            g = v;
            b = p;
            break;
        case 2:
            r = p;
            g = v;
            b = t;
            break;
        case 3:
            r = p;
            g = q;
            b = v;
            break;
        case 4:
            r = t;
            g = p;
            b = v;
            break;
        case 5:
            r = v;
            g = p;
            b = q;
            break;
        default:
            break;  
    }
    int resultR = floor(r * 255.0);
    int resultG = floor(g * 255.0);
    int resultB = floor(b * 255.0);
    return ccc3(resultR, resultG, resultB);
}

CCSprite *CCCommonUtils::getItemBGByColor(int color){
    int hValue = 0;
    CCSprite *sprite = NULL;
    if(color == WHITE){
        sprite = CCLoadSprite::createSprite(CCString::createWithFormat("tool_%d.png", 1)->getCString());
    }else if(color == GREEN){
        hValue = 137;
        sprite = CCCommonUtils::addFilterSprite("tool_1.png", CCCommonUtils::covertHSBToRGB(hValue, 0.7, 1.0), 0.02, 1.06, 0.94);
    }else if(color == BLUE){
        hValue = 201;
        sprite = CCCommonUtils::addFilterSprite("tool_1.png", CCCommonUtils::covertHSBToRGB(hValue, 0.81, 1.0), 0.02, 1.06, 2.45);
    }else if(color == PURPLE){
        hValue = 293;
        sprite = CCCommonUtils::addFilterSprite("tool_1.png", CCCommonUtils::covertHSBToRGB(hValue, 0.61, 1.0), 0.02, 1.04, 0.94);
    }else if(color == ORANGE){
        hValue = 27;
        sprite = CCCommonUtils::addFilterSprite("tool_1.png", CCCommonUtils::covertHSBToRGB(hValue, 0.7, 1.0), 0.06, 1.06, 0.94);
    }else if(color == GOLDEN){
        hValue = 48;
        sprite = CCCommonUtils::addFilterSprite("tool_1.png", CCCommonUtils::covertHSBToRGB(hValue, 0.75, 1.0), 0.02, 1.24, 1.64);
    }else{
        sprite = CCLoadSprite::createSprite("no_iconFlag.png");
    }
    return sprite;
}

void CCCommonUtils::changeAllianceFlagBGByRank(CCSprite *sprite, int index, bool isGray){
    if(sprite->getParent() == NULL){
        return;
    }
    auto parent = sprite->getParent();
    CCSprite *newSprite = NULL;
    if(isGray){
        if(index == 5){
            newSprite = CCCommonUtils::addFilterSprite("Alliance_Flag_Gre.png", CCCommonUtils::covertHSBToRGB(0, 0, 1.0), 0.04, 1.01, 0);
        }else{
            newSprite = CCLoadSprite::createSprite("Alliance_Flag_Gre.png");
        }
    }else{
        if(index == 4){
            newSprite = CCLoadSprite::createSprite("Alliance_Flag_04.png");
        }else if(index == 5){
            newSprite = CCLoadSprite::createSprite("Alliance_Flag_05.png");
        }else{
            if(index == 1){
                newSprite = CCCommonUtils::addFilterSprite("Alliance_Flag_Gre.png", CCCommonUtils::covertHSBToRGB(48, 1, 1.0), 0, 1, 3.12);
            }else if(index == 2){
                newSprite = CCCommonUtils::addFilterSprite("Alliance_Flag_Gre.png", CCCommonUtils::covertHSBToRGB(76, 1, 1.0), 0, 1, 3.12);
            }else{
                newSprite = CCCommonUtils::addFilterSprite("Alliance_Flag_Gre.png", CCCommonUtils::covertHSBToRGB(197, 1, 1.0), 0, 1, 3.12);
            }
        }
    }
    if(newSprite){
        sprite = newSprite;
        parent->addChild(newSprite);
    }
}

CCArray* CCCommonUtils::getTypeArray(int type){
    int city_lv = FunBuildController::getInstance()->getMainCityLv();
    auto goodDic = LocalController::shared()->DBXMLManager()->getGroupByKey("goods");
    CCArray* array = CCArray::create();//new CCArray();
    //array->init();
    int num = 0;
    if(goodDic)
    {
        CCDictElement* element;
        CCDICT_FOREACH(goodDic, element)
        {
            CCDictionary* dictInfo = _dict(element->getObject());
            if(type==dictInfo->valueForKey("type2")->intValue() && dictInfo->valueForKey("type")->intValue()==4 && city_lv>=dictInfo->valueForKey("lv")->intValue()){
                array->addObject(dictInfo);
            }
        }
    }
    num = array->count();
    for (int i=0; i<num; i++) {
        for (int j=i; j<num; j++) {
            CCDictionary* dict1 = _dict(array->objectAtIndex(i));
            CCDictionary* dict2 = _dict(array->objectAtIndex(j));
            if(dict1->valueForKey("order_num")->intValue()<dict2->valueForKey("order_num")->intValue()){
                array->swap(i, j);
            }
        }
    }
    return array;
}

void CCCommonUtils::onUploadPhoto(string uid, int srcCode, int idx)
{
    int idd = atoi(uid.c_str());
    cocos2d::extension::CCDevice::onUploadPhoto(idd, srcCode, idx);
}

string CCCommonUtils::getCustomPicUrl(string uid, int picVer)
{
    if (uid.length() < 6)
    {
        return "";
    }
    //http://cok.eleximg.com/cok/img/710001/bd65ca6d7d40cf95a3fe6b7fbb5cef7c.jpg
    string url = CUSTOM_PIC_URL;
    
    string md5Str = uid + "_" + CC_ITOA(picVer);
    MD5 md5;
    md5.update(md5Str.c_str(), md5Str.length());
    
    string tempStr = uid;
    url.append(tempStr.substr(tempStr.length()-6,tempStr.length()));
    url.append("/");
    url.append(md5.toString().c_str()).append(".jpg");
    
    return url;
}

string CCCommonUtils::getCustomPicUrl1(const char * uid, int picVer)
{
    if (uid == NULL) {
        return "";
    }
    
    //http://cok.eleximg.com/cok/img/710001/bd65ca6d7d40cf95a3fe6b7fbb5cef7c.jpg
    string url = CUSTOM_PIC_URL;
    string uidString = uid;
    if (uidString.length()==0){
        return "";
    }
    string md5Str = uidString + "_" + CC_ITOA(picVer);
    MD5 md5;
    md5.update(md5Str.c_str(), md5Str.length());
    
    string tempStr = uid;
    url.append(tempStr.substr(tempStr.length()-6,tempStr.length()));
    url.append("/");
    url.append(md5.toString().c_str()).append(".jpg");
    
    return url;
}
string CCCommonUtils::getShakePicUrl(string picName){
    string num = string("000") + picName.substr(4,3);
    
    string url = CUSTOM_PIC_URL;
    MD5 md5;
    md5.update(picName.c_str(), picName.length());
    
    url = url + num;
    url.append("/");
    url.append(md5.toString().c_str()).append(".jpg");
    return url;
}

bool CCCommonUtils::isUseCustomPic(int picVer)
{
    if (UploadImageController::shared()->getUploadImageFlag() == 0)
    {
        return false;
    }
    if (picVer>0 && picVer<1000000)
    {
        return true;
    }
    return false;
}

//字符串替换函数
std::string& CCCommonUtils::StringReplace(std::string& strBig, const std::string& strsrc, const std::string& strdst)
{
    std::string::size_type pos = 0;
    std::string::size_type srclen = strsrc.size();
    std::string::size_type dstlen = strdst.size();
    while( (pos = strBig.find(strsrc, pos)) != std::string::npos)
    {
        strBig.replace(pos, srclen, strdst);
        pos += dstlen;
    }
    return strBig;
}

bool CCCommonUtils::isTestPlatformAndServer(string key)
{
    bool ret = false;
//    string zone = NetController::shared()->getZone();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if ( GlobalData::shared()->testSwitchMap.find(key) != GlobalData::shared()->testSwitchMap.end() )
    {
        if (GlobalData::shared()->testSwitchMap[key] == 1) {
            ret = true;
            string tmpVersion = GlobalData::shared()->testSwitchVersionMap[key];
            if (tmpVersion != "" && !CCCommonUtils::checkVersion(tmpVersion)) {
                ret = false;
            }
        }
    }
//#endif
    return ret;
}

bool CCCommonUtils::isOpenFormDataCfg(string key)
{
    if (key == "0") {
        return false;
    }else if (key == "1") {
        return true;
    }else if (key != "") {
        if (CCCommonUtils::checkVersion(key)) {
            return true;
        }else {
            return false;
        }
    }else {
        return true;
    }
}

// 同步下载一个文件
static size_t curl_write_func(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    FILE *fp = (FILE*)userdata;
    size_t written = fwrite(ptr, size, nmemb, fp);
    return written;
}
//
//static int curl_progress(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
//{
//    auto percent = (int)(nowDownloaded/totalToDownload*100);
//    percent = percent < 0 ? 0 : percent;
//    //    CCLOG("DynamicResource___downloading... %d%%", percent);
//    return 0;
//}

bool CCCommonUtils::downloadFile(std::string const& url, std::string const& localfile)
{
    if(url.empty() || localfile.empty())
    {
        CCLOG("downloadFile - param error, url(%s), localfile(%s).", url.c_str(), localfile.c_str());
        return false;
    }
    
    // Create a file to save package.
    CCLOG("downloadFile - url[%s] to local[%s]", url.c_str(), localfile.c_str());
    FILE *fp = fopen(localfile.c_str(), "w+");
    if (! fp){
        CCLOG("downloadFile - can not create file %s", localfile.c_str());
        return false;
    }
    
    try
    {
        CURL * m_curl = curl_easy_init();
        if (! m_curl) {
            CCLOG("downloadFile - can not init curl");
            return false;
        }
        
        // Download pacakge
        curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1);
        
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, fp);
        
        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, curl_write_func);
        
        // for debug
        //        curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, false);
        //        curl_easy_setopt(m_curl, CURLOPT_PROGRESSDATA, this);
        //        curl_easy_setopt(m_curl, CURLOPT_PROGRESSFUNCTION, curl_progress);
        
        CURLcode res = curl_easy_perform(m_curl);
        curl_easy_cleanup(m_curl);
        fclose(fp);
        
        if (res == CURLE_OK)
        {
            CCLOG("downloadFile - succeed downloading package %s", url.c_str());
            return true;
        }
        
        // 下载失败删除文件
        remove(localfile.c_str());
        CCLOG("downloadFile - error when download package, error(%d).", res);
    } catch (...) {
        CCLog("downloadFile - curl exception when download package");
    }
    
    CCLog("downloadFile - error.");
    return false;
}

bool CCCommonUtils::createDirectory(const char *path)
{
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    
    return true;
}

bool CCCommonUtils::unzipToDirectory(std::string const& strZip, std::string const& strDir, bool error_clear, bool s)
{
    // strDir
    std::vector<std::string> vExactFiles;
    if (error_clear) {
        vExactFiles.reserve(10);
    }
    
#define UNZIP_BUFFER_SIZE    (8192*2)
#define UNZIP_MAX_FILENAME   512
    
    // 解压文件至相应目录
    unzFile zipfile = unzOpen(strZip.c_str());
    if (! zipfile)
    {
        CCLOG("unzipToDirectory - can not open zip file %s", strZip.c_str());
        return false;
    }
    
    // Get info about the zip file
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
    {
        CCLOG("unzipToDirectory - can not read file global info of %s", strZip.c_str());
        unzClose(zipfile);
        return false;
    }
    
    // Buffer to hold data read from the zip file
    char readBuffer[UNZIP_BUFFER_SIZE];
    std::string fullPath;
    
    //CCLOG("start uncompressing");
    
    // Loop to extract all files.
    uLong i;
    for (i = 0; i < global_info.number_entry; ++i)
    {
        // Get info about current file.
        unz_file_info fileInfo;
        char fileName[UNZIP_MAX_FILENAME];
        if (unzGetCurrentFileInfo(zipfile,
                                  &fileInfo,
                                  fileName,
                                  _countof(fileName),
                                  NULL,
                                  0,
                                  NULL,
                                  0) != UNZ_OK)
        {
            CCLOG("unzipToDirectory - can not read file info");
            goto UNZIP_ERROR;
//            unzClose(zipfile);
//            return false;
        }
        
        fullPath = strDir + fileName;
        if (error_clear) {
            vExactFiles.push_back(fullPath);
        }
        
        // Check if this entry is a directory or a file.
        const size_t filenameLength = strlen(fileName);
        if (fileName[filenameLength-1] == '/')
        {
            // Entry is a direcotry, so create it.
            // If the directory exists, it will failed scilently.
            if (!CCCommonUtils::createDirectory(fullPath.c_str()))
            {
                CCLOG("unzipToDirectory - can not create directory %s", fullPath.c_str());
                goto UNZIP_ERROR;
//                unzClose(zipfile);
//                return false;
            }
        }
        else
        {
            // Entry is a file, so extract it.
            
            // Open current file.
            if (unzOpenCurrentFile(zipfile) != UNZ_OK)
            {
                CCLOG("unzipToDirectory - can not open file %s", fileName);
                goto UNZIP_ERROR;
//                unzClose(zipfile);
//                return false;
            }
            
            // Create a file to store current file.
            FILE *out = fopen(fullPath.c_str(), "w+");
            if (! out)
            {
                CCLOG("unzipToDirectory - can not open destination file %s", fullPath.c_str());
                unzCloseCurrentFile(zipfile);
                goto UNZIP_ERROR;
//                unzClose(zipfile);
//                return false;
            }
            
            // Write current file content to destinate file.
            int error = UNZ_OK;
            do
            {
                error = unzReadCurrentFile(zipfile, readBuffer, sizeof(readBuffer));
                if (error < 0)
                {
                    CCLOG("unzipToDirectory - can not read zip file %s, error code is %d", fileName, error);
                    fclose(out);
                    unzCloseCurrentFile(zipfile);
                    goto UNZIP_ERROR;
//                    unzClose(zipfile);
//                    return false;
                }
                
                if (error > 0)
                {
                    int wsize = fwrite(readBuffer, error, 1, out);
                    if (wsize != 1) {
                        // 理论这里应该也算错误，写入文件出错了？这种情况少见。。。。
                        CCLOG("unzipToDirectory - write file %s error, ret %d.", fileName, wsize);
                        fclose(out);
                        unzCloseCurrentFile(zipfile);
                        goto UNZIP_ERROR;
                        //                    unzClose(zipfile);
                        //                    return false;

                    }
                }
            } while(error > 0);
            
            fclose(out);
        }
        
        unzCloseCurrentFile(zipfile);
        
        // Goto next entry listed in the zip file.
        if ((i+1) < global_info.number_entry)
        {
            if (unzGoToNextFile(zipfile) != UNZ_OK)
            {
                CCLOG("unzipToDirectory - can not read next file");
                goto UNZIP_ERROR;
                //unzClose(zipfile);
                //return false;
            }
        }
        
        // 加入这个usleep的原因是当文件解压的时候，IO压力太大，会造成手机卡机（至少NOTE3如此）
        // 因为一般资源下载解压是在异步线程，所以这里sleep 50ms不会造成任何影响。但是用户感觉会好。
        // 如果需要，可以在上面的解压文件内容的时候也加上sleep。这样理论影响会更小。
        if (s) {
            usleep(50 * 1000); // 1 ms
        }
    }
    
    unzClose(zipfile);
    CCLOG("unzipToDirectory - ok. (%s) -> (%s)", strZip.c_str(), strDir.c_str());
    return true;
    
UNZIP_ERROR:
    // 错误清理
    unzClose(zipfile);
    
    // 从后向前删除文件及相关文件夹
    for (std::vector<std::string>::reverse_iterator r = vExactFiles.rbegin(); r!=vExactFiles.rend(); ++r) {
        std::string& s = (*r);
        std::remove(s.c_str());
    }
    
    CCLOG("unzipToDirectory - error. (%s) -> (%s)", strZip.c_str(), strDir.c_str());
    return false;
}

void CCCommonUtils::showBanHelpShift(){

}

void CCCommonUtils::showEmulatorBanHelpShift(){

}

void CCCommonUtils::showHelpShiftFAQ()
{

}

void CCCommonUtils::showDynamicForm()
{


}

void CCCommonUtils::sendHelpshiftLog(const char* IP, const char* uid, const char* tag)
{
    
}

void CCCommonUtils::showHelpShiftFAQ(string itemId)
{

}

void CCCommonUtils::showHelpShiftSingleFAQ(string itemId)
{

}

bool CCCommonUtils::checkTaiWanFlag(){
    bool flag = false;//true为关
    if(GlobalData::shared()->playerInfo.regCountry=="CN" && !GlobalData::shared()->isOpenTaiwanFlag && GlobalData::shared()->playerInfo.gmFlag!=1){
        flag = true;
    }
    return flag;
}

std::string CCCommonUtils::changeChinaFlag(string flag){
    string banner = flag;
    std::string language = LocalController::shared()->getLanguageFileName();
    if(flag=="CN" && (GlobalData::shared()->playerInfo.regCountry=="CN" || language == "zh_CN") && GlobalData::shared()->chinaSwitchFlag==1 && GlobalData::shared()->playerInfo.gmFlag!=1){
        banner = "CN_2";
    }
    return banner;
}

std::string CCCommonUtils::changeHKToChinaFlag(string flag){
    string banner = flag;
    std::string language = LocalController::shared()->getLanguageFileName();
    if(flag=="HK" && (GlobalData::shared()->playerInfo.regCountry=="CN" || language == "zh_CN") && GlobalData::shared()->on_other_chinaFlag_k1==1 && GlobalData::shared()->playerInfo.gmFlag!=1){
        banner = "CN";
    }
    return banner;
}

bool CCCommonUtils::isShowFlagForChinese(){
    bool flag = true;
    std::string language = LocalController::shared()->getLanguageFileName();
    if((GlobalData::shared()->playerInfo.regCountry=="CN" || language == "zh_CN") && GlobalData::shared()->chinaSwitchFlag==2 && GlobalData::shared()->playerInfo.gmFlag!=1){
        flag = false;
    }
    return flag;
}

std::string CCCommonUtils::getVersionName(){
    return cocos2d::extension::CCDevice::getVersionName();
}

bool CCCommonUtils::canOpenNewIcon(){
//    return true;//testcode
    bool canOpen  = true ;
    if (!CCCommonUtils::checkVersion(GlobalData::shared()->newIcon_version)) {
        canOpen = false;
    }
    if (GlobalData::shared()->newIcon_country.find(CCCommonUtils::getLanguage()) == string::npos) {
        canOpen = false;
    }
    if (GlobalData::shared()->newIcon_endTime < WorldController::getInstance()->getTime() / 1000) {
        canOpen = false;
    }
    return canOpen;
}
bool CCCommonUtils::isNewIcon(string iconName){
    return GlobalData::shared()->newIcon_name.find(iconName) != string::npos;
}
bool CCCommonUtils::checkVersion(string version){
    bool flag = false;
    std::vector<std::string> _updateVersion;
    CCCommonUtils::splitString(version,".", _updateVersion);
    //当前app版本
    std::string _appVersion = cocos2d::extension::CCDevice::getVersionName();
    std::vector<std::string> _appXMLVersion;
    CCCommonUtils::splitString(_appVersion,".", _appXMLVersion);
    //如果app的版本大于成功下载xml版本，使用apk内xml
    if (_updateVersion.size() == 3 && _appXMLVersion.size()==3) {
        int aV1 = atoi(_appXMLVersion[0].c_str());
        int uV1 = atoi(_updateVersion[0].c_str());
        if(aV1 > uV1){
            flag = true;
        }else if(aV1 == uV1){
            int aV2 = atoi(_appXMLVersion[1].c_str());
            int uV2 = atoi(_updateVersion[1].c_str());
            if (aV2>uV2) {
                flag = true;
            }else if(aV2==uV2){
                int aV3 = atoi(_appXMLVersion[2].c_str());
                int uV3 = atoi(_updateVersion[2].c_str());
                if(aV3>=uV3){
                    flag = true;
                }
            }
        }
    }
    return flag;
}

bool CCCommonUtils::isInSimulator()
{
    if(GlobalData::shared()->checkSimulatorFlag==0)
    {
        return false;
    }
    std::string device = CCUserDefault::sharedUserDefault()->getStringForKey("phoneDevice", "");
    
    if(device == "")
    {
        return false;
    }
    if(device.find("qemu+")!=string::npos||device.find("86+")!=string::npos)
    {
        return true;
    }
    return false;
}
bool CCCommonUtils::isChina(){
    bool isChinaPlatForm = (GlobalData::shared()->isChinaPlatForm() || GlobalData::shared()->analyticID == "common");
    bool isChinaLan = (CCCommonUtils::getLanguage() == "zh_CN");
    return isChinaPlatForm || isChinaLan;
}

string CCCommonUtils::getDragonBuildingNameByType(int type){
    string name = "";
    switch (type) {
        case Crystal:
            name = _lang("140082");
            break;
        case Armory:
            name = _lang("140083");
            break;
        case TrainingField:
            name = _lang("140084");
            break;
        case SupplyPoint:
            name = _lang("140086");
            break;
        case BessingTower:
            name = _lang("140087");
            break;
        case MedicalTower:
            name = _lang("140085");
            break;
        case DragonTower:
            name = _lang("140090");
            break;
        case Barracks:
            name = _lang("140089");
            break;
        case TransferPoint:
            name = _lang("140088");
            break;
    }
    return name;
}

string CCCommonUtils::getDragonBuildingInfoByType(int type){
    string name = "";
    switch (type) {
        case Crystal:
            name = _lang("140126");
            break;
        case Armory:
            name = _lang("140128");
            break;
        case TrainingField:
            name = _lang("140127");
            break;
        case SupplyPoint:
            name = _lang("140129");
            break;
        case BessingTower:
            name = _lang("140131");
            break;
        case MedicalTower:
            name = _lang("140132");
            break;
        case DragonTower:
            name = _lang("140133");
            break;
        case Barracks:
            name = _lang("140134");
            break;
        case TransferPoint:
            name = _lang("140130");
            break;
    }
    return name;
}

bool CCCommonUtils::useCaptcha(){
    return GlobalData::shared()->userNeedChangeCheck;
    //    if(!GlobalData::shared()->userNeedChangeCheck){
    //        return false;
    //    }
    //    if(GlobalData::shared()->userChangeCheckVersion == "" || !checkVersion(GlobalData::shared()->userChangeCheckVersion)){
    //        return false;
    //    }
    //    std::string str = GlobalData::shared()->changeCheckServer;
    //    if(str == "all"){
    //        return true;
    //    }else if(str == "0" || str == ""){
    //        return false;
    //    }
    //    int serverId = GlobalData::shared()->playerInfo.selfServerId;
    //    std::vector<std::string> m_vector;
    //    CCCommonUtils::splitString(str.c_str(), ";", m_vector);
    //    int index = 0;
    //    while (index < m_vector.size()) {
    //        std::string serverStr = m_vector[index];
    //        std::vector<std::string> serverVector;
    //        CCCommonUtils::splitString(serverStr.c_str(), "-", serverVector);
    //        if(serverVector.size() == 0){
    //            break;
    //        }
    //        int startServer = atoi(serverVector[0].c_str());
    //        int endServer = startServer;
    //        if(serverVector.size() == 2){
    //            endServer = atoi(serverVector[1].c_str());
    //        }
    //        if(serverId < startServer || serverId > endServer){
    //
    //        }else{
    //            return true;
    //        }
    //        index++;
    //    }
    //    return false;
}
bool CCCommonUtils::isFirstJoinAlliance(){
    return GlobalData::shared()->playerInfo.isfirstJoin == 1;
}

void CCCommonUtils::clearGameCache()
{
    #if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    string purshaseInfoList = CCUserDefault::sharedUserDefault()->getStringForKey(COK_PURCHASE_KEY,"");
    string idList = CCUserDefault::sharedUserDefault()->getStringForKey(COK_PURCHASE_SUCCESSED_KEY,"");
    
    CCUserDefault::resetUserDefault();

    CCUserDefault::sharedUserDefault()->setStringForKey(COK_PURCHASE_KEY,purshaseInfoList);
    CCUserDefault::sharedUserDefault()->setStringForKey(COK_PURCHASE_SUCCESSED_KEY,idList);
    CCUserDefault::sharedUserDefault()->flush();

    
//    需要删除xml文件
    std::string update1 = CCFileUtils::sharedFileUtils()->getWritablePath() + "database.local.xml";
    CCFileUtils::getInstance()->removeFile(update1);
    #endif
}

bool CCCommonUtils::checkServer(string serverID)
{
    // 服务器全开使用all，全关使用close。不要再使用-1了，因为这段代码处理-1，相当于处理0-1。。
    bool ret= false;
    string k11String = serverID;
    if(k11String.compare("0")==0){
        ret=false;
    }else if(k11String.compare("all")==0){
        ret=true;
    }
    else if(k11String.compare("close")==0){
        ret=false;
    }
    else if(!k11String.empty()){
        int selfserver = GlobalData::shared()->playerInfo.selfServerId;
        vector<string> serverlist;
        CCCommonUtils::splitString(k11String, ";", serverlist);
        auto iterK = serverlist.begin();
        while (iterK!=serverlist.end()) {
            auto posK = (*iterK).find("-");
            if(posK!=string::npos){
                int sIdx = atoi((*iterK).substr(0,posK).c_str());
                int eIdx = atoi((*iterK).substr(posK+1).c_str());
                if(selfserver>=sIdx && selfserver<=eIdx){
                    ret=true;
                    break;
                }
            }else{
                int cIdx = atoi((*iterK).c_str());
                if(cIdx == selfserver){
                    ret=true;
                    break;
                }
            }
            ++iterK;
        }
    }
    return ret;
}

void CCCommonUtils::onNetworkAvailable(CCObject* p)
{
    CCLog("jinpeng post onNetworkAvailable");

    CCBool* _status = dynamic_cast<CCBool*>(p);
    if (!_status->getValue())
    {
        NetController::shared()->checkNetWork("");
    }
    else
    {
        NetController::shared()->connect();
    }
}

bool CCCommonUtils::IsBandOfAcount()
{
    string facebookUid = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERID, "");
    string facebookName = CCUserDefault::sharedUserDefault()->getStringForKey(FB_USERNAME, "");
    string googleUid = CCUserDefault::sharedUserDefault()->getStringForKey(GP_USERID, "");
    string googleName = CCUserDefault::sharedUserDefault()->getStringForKey(GP_USERNAME, "");
    
    bool isAccountBind = false;
    for (auto it = GlobalData::shared()->playerInfo.m_platFormBindInfo.begin(); it != GlobalData::shared()->playerInfo.m_platFormBindInfo.end(); it++) {
        if(it->second.bindPf != "" && it->second.bindId != ""){
            isAccountBind = true;
        }
    }
    
    if (facebookUid=="" && facebookName=="" && googleUid=="" && googleName=="" && !isAccountBind) {
        return false;
    }
    return true;
}

bool CCCommonUtils::isUserNewUid(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    if (!JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/ext/Udid", "useNewUuid", "()Z") ) {
        CCLOGFUNC("jni: no method useNewUuid");
        return false;
    }
    
    jboolean ret = minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    return ret;
#endif
    return false;
}

//type 0登录成功耗时 1在世界地图刷新一次的耗时 2登陆失败
void CCCommonUtils::sendConnectTimeToServer(int type, double time){
    if(GlobalData::shared()->analyticID != "cn1"){
        return;
    }
    std::string connectionType = NetController::shared()->getConnectType();
    if(time == 0){
        return;
    }
    double currentTime = getLocalTime();
    int dTime = currentTime - time;
    if(dTime < 0){
        return;
    }
    std::string timeStr = CC_ITOA(dTime);
    std::string typeStr = CC_ITOA(type);
    std::string zoneStr = NetController::shared()->getZone();
    std::string serverId = "";
    if(zoneStr != "" && zoneStr.length() > 3){
        serverId = zoneStr.substr(3);
    }

    CCString *url = CCString::createWithFormat("http://119.29.68.236/accept.php?type=%s&time=%s&connectionType=%s&serverId=%s", typeStr.c_str(), timeStr.c_str(), connectionType.c_str(), serverId.c_str());

    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl(url->getCString());
    request->setRequestType(CCHttpRequest::Type::POST);
    CCHttpClient::getInstance()->setTimeoutForConnect(10);
    CCHttpClient::getInstance()->send(request);
    request->release();
}

double CCCommonUtils::getLocalTime(){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

bool CCCommonUtils::isKuaFu(){
    bool flag = false;
    if(GlobalData::shared()->serverType==SERVER_BATTLE_FIELD){
        flag = true;
    }else if(GlobalData::shared()->serverType==SERVER_DRAGON_BATTLE){
        flag = true;
    }else if(isKuaFuWangZhan()){
        flag = true;//王国远争
    }
    return flag;
}

bool CCCommonUtils::isKuaFuWangZhan(){
    bool flag = false;
    if(GlobalData::shared()->serverType<SERVER_BATTLE_FIELD && GlobalData::shared()->playerInfo.crossFightSrcServerId>0){
        flag = true;//王国远争
    }
    return flag;
}
void CCCommonUtils::onAddFire(CCNode * parentNode){
    if (parentNode) {
        parentNode->addChild(ParticleFireAni::create());
    }
}


//获取士兵等级 根据int
string CCCommonUtils::getChineseLevelTxtByNum(int nlevel)
{
    string bigNum[31] = {"零","一","二","三","四","五","六","七","八","九","十","十一","十二","十三","十四","十五","十六","十七","十八","十九","二十","廿一","廿二","廿三","廿四","廿五","廿六","廿七","廿八","廿九","三十"};
    if( nlevel >= 0 && nlevel <= 30 )
    {
        return bigNum[nlevel];
    }
    return "";
}


string CCCommonUtils::getCountryIconByName(string banner)
{
    string ret="";
    if (banner=="") {
        banner = "UN.png";
    }else{
        //大陆地区，看到台湾的，都成了大陆的国旗
        if(banner=="TW" && CCCommonUtils::checkTaiWanFlag()){
            banner = "CN";
        }else if(banner=="HK"){
            banner = CCCommonUtils::changeHKToChinaFlag(banner);
        }
        banner = CCCommonUtils::changeChinaFlag(banner);
        banner = banner + ".png";
    }
    ret = banner;
    return ret;
}
