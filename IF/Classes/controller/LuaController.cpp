//
//  LuaController.cpp
//  IF
//
//  Created by Émilie.Jiang on 15/1/26.
//
//

#include "LuaController.h"
#include "CCThreadManager.h"
#include "cocosbuilder/lua_cocos2dx_cocosbuilder_manual.h"
#include "CCLuaEngine.h"
#include "PopupViewController.h"
#include "RechargeACTVCell.h"
#include "CCTypesExt.h"
#include "PayController.h"
#include "ParticleController.h"
#include "ToolController.h"
#include "GoldExchangeMoreListView.h"
#include "ButtonLightEffect.h"
#include "AllianceInviteView.h"
#include "../Ext/CCDevice.h"

#include <curl/easy.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "../view/guiExt/md5.h"
#include <stdio.h>
#include "external/unzip/unzip.h"
#include "ActivityController.h"

#include "spine/SkeletonAnimation.h"
#include "lua_cocos2dx_if_manual.h"
#include "EquipmentController.h"
#include "LuaCommand.h"
#include "SceneController.h"
#include "ChatController.h"
#include "Utf8Utils.h"
#include "FlySystemUpdateHint.h"
#include "GuideController.h"
#include "ChatServiceCocos2dx.h"
#include "ActivityDetailView.h"
#include "YesNoDialog.h"
#include "UIComponent.h"
//#include "CCNotificationCenter.h"
#include "SoundController.h"
#include "RewardController.h"
#include "StoreView.h"
#include "TipsView.h"
#include "ChatServiceCocos2dx.h"
//lixu 20151224 cok is a 2d game, no usage for 3d modules
//#include "3d/lua_cocos2dx_3d_manual.h"
#include "ui/lua_cocos2dx_ui_manual.hpp"
#include "extension/lua_cocos2dx_extension_manual.h"
#include "external/xxtea/xxtea.h"
#include "ImperialScene.h"
#include "JoinAllianceView.h"


#define LUA_DOWNLOADED_VERSION "lua_downloaded_version"
#define LUA_CLIENT_VERSION "lua_client_version"
#define LUA_ZIP "Tmp.zip"
#define LUA_BUFFER_SIZE    8192
#define LUA_MAX_FILENAME   512
#define LUA_VERSION_FILE "luaVersion.bin"

#define CHAT_NOTICE_TAG 99999

#define ENCRYPT_KEY "cok_2dxV3_lua"
#define ENCRYPT_SIGN "elexhcg"
#define LUA_COCOS_LIB "cocos"
#define LUA_COCOS_LIB_MD5 "0d2b313e7675651f9ee23f0956f130e7"

static LuaController *_luainstance = NULL;
LuaController* LuaController::getInstance() {
    if (!_luainstance) {
        _luainstance = new LuaController();
    }
    return _luainstance;
}
LuaController::LuaController():/*m_tid1(NULL),m_tid(NULL),*/m_advCellContainer(NULL),m_advCell(NULL),m_iconContainer(NULL),m_isAddPath(false),m_saleViewContainer(NULL),m_clientVersion(""),m_serverVersion(""),m_downedVersion(""),m_initLuaEngine(false)/*,m_isDownload(false)*/,luazipSize(350),luaSize(500),luaMD5Code(""),luaVersion(""),m_initVersion(""),/*m_downloadType(LuaDownloadType_NONE),luapackMD5Code(""),m_packName(""),*/mChatNoticeNode(NULL),m_canTranslate(false),m_orgMsg("")/*,m_storageEnough(true),m_isCheckStorage(false)*/{
    m_downedVersion = CCUserDefault::sharedUserDefault()->getStringForKey(LUA_DOWNLOADED_VERSION);
    m_clientVersion = CCUserDefault::sharedUserDefault()->getStringForKey(LUA_CLIENT_VERSION);
    CCLog("lua___client_version: %s",m_clientVersion.c_str());
    CCLog("lua___download_version: %s",m_downedVersion.c_str());
    m_writeablePath = CCFileUtils::sharedFileUtils()->getWritablePath();
    CCLog("lua___writeablePath: %s",m_writeablePath.c_str());
    m_initVersion = m_clientVersion;
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LuaController::ChatNoticeInView), MSG_POPUP_VIEW_IN, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LuaController::ChatNoticeInSceen), MSG_POPUP_VIEW_OUT, NULL);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    cocos2d::extension::CCDevice::addSkipBackupAttributeToItemAtURL(m_writeablePath);
#endif
    
    //
    //test code
    //
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    m_worldconfig["revive"] = "22d32c8b7dcaa84ef19a3cd65360d27f";
//#endif
    
}
LuaController::~LuaController(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_POPUP_VIEW_IN);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_POPUP_VIEW_OUT);
}

void LuaController::purgeData()
{
    if (_luainstance) {
        _luainstance->purgeLua();
        _luainstance->resetData();
    }
//    CC_SAFE_RELEASE_NULL(_luainstance);
}

void LuaController::resetData()
{
    m_advCellContainer = nullptr;
    m_advCell = nullptr;
    m_iconContainer = nullptr;
    m_saleViewContainer = nullptr;
    mExGiftList.clear();
}

void LuaController::purgeLua()
{
    cocos2d::log("___TEST_LUA_PURGE_0");
    if(isLuaOpen()==false)
        return;
    cocos2d::log("___TEST_LUA_PURGE_1");
    if(initLua()==false)
        return;
    cocos2d::log("___TEST_LUA_PURGE_2");
    if(initMD5Info() == false)
        return;
    cocos2d::log("___TEST_LUA_PURGE_3");
    if (m_clientVersion.compare(this->luaVersion) != 0)
        return;
    cocos2d::log("___TEST_LUA_PURGE_4");
    LuaEngine* pEngine = LuaEngine::getInstance();
    string path = getLuaPath();
    string filepath = path+"/mainCOK2.lua";
    if (!CCFileUtils::sharedFileUtils()->isFileExist(filepath)) {
        return;
    }
    pEngine->addSearchPath(path.c_str());
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
    lua_State* ls = pEngine->getLuaStack()->getLuaState();
    lua_getglobal(ls, "purgeData");
    cocos2d::log("___TEST_LUA_PURGE_5");
    if (!lua_isfunction(ls, -1))
    {
        lua_pop(ls, 1);
        return;
    }
    lua_call(ls, 0, 0);
    cocos2d::log("___TEST_LUA_PURGE_6");
}

bool LuaController::isLuaOpen(){
    return false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    CCLog("lua_server_version [%s]",this->luaVersion.c_str());
    if(this->luaVersion.empty())
        return false;
    if(this->luaVersion.compare("0.0.0") == 0){
        return false;
    }
    return true;
#endif
    return false;
}
bool LuaController::initLua(){
//    return false;
    if(!m_initLuaEngine){
        lua_State *lState = lua_open();
        if(!lState)
            return false;
        lua_close(lState);
        m_initLuaEngine=true;
        // register lua engine
        auto engine = LuaEngine::getInstance();
        engine->getLuaStack()->setXXTEAKeyAndSign(ENCRYPT_KEY, strlen(ENCRYPT_KEY), ENCRYPT_SIGN, strlen(ENCRYPT_SIGN));
        ScriptEngineManager::getInstance()->setScriptEngine(engine);
        lua_State* L = engine->getLuaStack()->getLuaState();
        register_ui_moudle(L);
        //lixu 20151224 no usage for 3d
        //register_cocos3d_module(L);
        register_extension_module(L);
        register_cocosbuilder_module(L);
        register_if_module(L);
//        LuaEngine* pEngine = LuaEngine::getInstance();
//        CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);
//        // register lua ccb
//        tolua_extensions_ccb_open(pEngine->getLuaStack()->getLuaState());
    }
    return true;
}

bool LuaController::checkActLuaValid(ActivityEventObj* obj)
{
    if(isDownloading("lua")==true || isUncompressing("lua") == true){
        return false;
    }
    if (isLuaOpen() == false) {
        return false;
    }
    if (obj->type != 6 && obj->type != 4) {
        return false;
    }
    if(initLua()==false)
        return false;
    if(initMD5Info()==false || checkLuaFiles() == false){
        resetVersionAndUpdate();
        return false;
    }
    if (m_clientVersion.compare(luaVersion) != 0) {
        resetVersionAndUpdate();
        return false;
    }
    string path = getLuaPath();
    string id = obj->id;
    if (!CCFileUtils::sharedFileUtils()->isFileExist(path + "/ccbi/Activity" + id +"AdLuaCell.ccbi")) {
        resetVersionAndUpdate();
        return false;
    }
    if (!CCFileUtils::sharedFileUtils()->isFileExist(path + "/resources/activity_" + id + ".plist")) {
        resetVersionAndUpdate();
        return false;
    }
    if (!CCFileUtils::sharedFileUtils()->isFileExist(path + "/resources/activity_" + id + ".pvr.ccz")) {
        resetVersionAndUpdate();
        return false;
    }
    return true;
}
bool LuaController::showNoramlIcon(std::string popImg){
    string file1 = CCFileUtils::sharedFileUtils()->getWritablePath() + "lua/ccbi/GoldExchange"+popImg+ "LuaView.ccbi";
    string file2 = CCFileUtils::sharedFileUtils()->getWritablePath() + "lua/ccbi/GoldExchange"+popImg+ "LuaIcon.ccbi";
    string bImage1 = CCFileUtils::sharedFileUtils()->getWritablePath() + "lua/resources/"+popImg + ".pvr.ccz";
    string bImagePlist1 = CCFileUtils::sharedFileUtils()->getWritablePath() + "lua/resources/"+popImg + ".plist";
    string bImage2 = CCFileUtils::sharedFileUtils()->getWritablePath() + "lua/resources/"+popImg+ "icon.pvr.ccz";
    string bImagePlist2 = CCFileUtils::sharedFileUtils()->getWritablePath() + "lua/resources/"+ popImg + "icon.plist";
    
    if(CCFileUtils::sharedFileUtils()->isFileExist(file1) && CCFileUtils::sharedFileUtils()->isFileExist(file2) && CCFileUtils::sharedFileUtils()->isFileExist(bImage1) && CCFileUtils::sharedFileUtils()->isFileExist(bImagePlist1)&& CCFileUtils::sharedFileUtils()->isFileExist(bImage2) && CCFileUtils::sharedFileUtils()->isFileExist(bImagePlist2)){
        return true;
    }
    return false;
}
bool LuaController::checkADV(string popImg){
    string file1 = CCFileUtils::sharedFileUtils()->getWritablePath() + "lua/ccbi/GoldExchangeAdv"+popImg+ "LuaCell_NEW.ccbi";
    string bImage1 = CCFileUtils::sharedFileUtils()->getWritablePath() + "lua/resources/"+popImg + "adv.pvr.ccz";
    string bImagePlist1 = CCFileUtils::sharedFileUtils()->getWritablePath() + "lua/resources/"+popImg + "adv.plist";
    
    if(CCFileUtils::sharedFileUtils()->isFileExist(file1) && CCFileUtils::sharedFileUtils()->isFileExist(bImage1) && CCFileUtils::sharedFileUtils()->isFileExist(bImagePlist1)){
        return true;
    }
    return false;
}
bool LuaController::checkLuaValid(std::string popImg,std::string md5,std::string type){
    if(isDownloading("lua")==true || isUncompressing("lua") == true){
        CCLog("checkLuaValid 1 return");
        return false;
    }
    if(isDownloading(popImg)==true || isUncompressing(popImg) == true){
        CCLog("checkLuaValid 2 return");
        return false;
    }
    if(isLuaOpen()==false){
        CCLog("checkLuaValid 3 return");
        return false;
    }
    if(popImg.empty() || md5.empty())
        return false;
    
    if(popImg.compare("1") == 0 || popImg.compare("2")==0 || popImg.compare("new_recharge") == 0 || popImg.compare("month") == 0 ||popImg.compare("throne") == 0 || popImg.compare("equip") == 0)
        return false;
    
    if(initLua()==false){
        CCLog("checkLuaValid 4 return");
        return false;
    }
    if(initMD5Info()==false || checkLuaFiles() == false){
        CCLog("checkLuaValid 5 return");
        resetVersionAndUpdate();
        return false;
    }
    if(m_clientVersion.compare(this->luaVersion) != 0){
        CCLog("checkLuaValid 6 return");
        checkUpdate();
        return false;
    }
    
    string path = getLuaPath();
    if(m_md5Map.find(popImg)==m_md5Map.end()){
        CCLog("checkLuaValid 7 return");
        updateNewResource(popImg,md5);
        return false;
    }
    if((*m_md5Map.find(popImg)).second.compare(md5) != 0){
        CCLog("checkLuaValid 8 return");
        updateNewResource(popImg,md5);
        return false;
    }
    
    bool isEnvelop = false;
    if(type == "1" && popImg == "envelope_gift"){
        isEnvelop=true;
    }
    if(isEnvelop==false){
        bool bCCB1 = CCFileUtils::sharedFileUtils()->isFileExist(path + "/ccbi/GoldExchange" + popImg + "LuaView.ccbi");
        if(!bCCB1){
            updateNewResource(popImg,md5);
            return false;
        }
    }
//    bool bCCB2 = CCFileUtils::sharedFileUtils()->isFileExist(path + "/ccbi/GoldExchangeAdv" + popImg + "LuaCell.ccbi");
//    if(!bCCB2){
//        updateNewResource(popImg,md5);
//        return false;
//    }
    if(isEnvelop==false){
        if(type.compare("1")!=0){
            bool bCCB3 =CCFileUtils::sharedFileUtils()->isFileExist(path + "/ccbi/GoldExchangeLuaCell.ccbi");
            if(!bCCB3){
                updateNewResource(popImg,md5);
                return false;
            }
            bool bCCB4 =CCFileUtils::sharedFileUtils()->isFileExist(path + "/ccbi/GoldExchangeLuaCell1.ccbi");
            if(!bCCB4){
                updateNewResource(popImg,md5);
                return false;
            }
        }
    
        bool bCCB5 =CCFileUtils::sharedFileUtils()->isFileExist(path + "/ccbi/GoldExchange" + popImg + "LuaIcon.ccbi");
        if(!bCCB5){
            updateNewResource(popImg,md5);
            return false;
        }
        bool bImage = CCFileUtils::sharedFileUtils()->isFileExist(path + "/resources/"+popImg + ".pvr.ccz");
        if(!bImage){
            updateNewResource(popImg,md5);
            return false;
        }
        bool bImagePlist = CCFileUtils::sharedFileUtils()->isFileExist(path + "/resources/"+popImg + ".plist");
        if(!bImagePlist){
            updateNewResource(popImg,md5);
            return false;
        }
    }
//    if(type.compare("1")!=0){
        bool bImage1 = CCFileUtils::sharedFileUtils()->isFileExist(path + "/resources/"+popImg + "adv.pvr.ccz");
        if(!bImage1){
            updateNewResource(popImg,md5);
            return false;
        }
        bool bImagePlist1 = CCFileUtils::sharedFileUtils()->isFileExist(path + "/resources/"+popImg + "adv.plist");
        if(!bImagePlist1){
            updateNewResource(popImg,md5);
            return false;
        }
//    }
    if(isEnvelop==false){
        bool bImage2 = CCFileUtils::sharedFileUtils()->isFileExist(path + "/resources/"+popImg + "icon.pvr.ccz");
        if(!bImage2){
            updateNewResource(popImg,md5);
            return false;
        }
        bool bImagePlist2 = CCFileUtils::sharedFileUtils()->isFileExist(path + "/resources/"+popImg + "icon.plist");
        if(!bImagePlist2){
            updateNewResource(popImg,md5);
            return false;
        }
    }
//    m_packName="";
    return true;
}
bool LuaController::checkNewAdvLuaValid(std::string popImg,std::string md5,std::string type)
{
    string path = getLuaPath();
    bool bCCB = CCFileUtils::sharedFileUtils()->isFileExist(path + "/ccbi/GoldExchangeAdv" + popImg + "LuaCell_NEW.ccbi");
    if (!bCCB) {
        return false;
    }
    return true;
}
void LuaController::addLuaPath(){
    if(m_isAddPath==false){
        LuaEngine* pEngine = LuaEngine::getInstance();
        string path = getLuaPath();
        string filepath = getLuaFile();
        pEngine->addSearchPath(path.c_str());
        m_isAddPath=true;
    }
}
std::string LuaController::getLuaPath(){
    string path = m_writeablePath+"lua";
    return path;
}
std::string LuaController::getLuaFile(){
    string path = getLuaPath();
    string filepath = path + "/mainCOK2.lua";
    return filepath;
}

std::string LuaController::getActTips(std::string actId)
{
    addLuaPath();
    if ("" == actId) {
        return "";
    }
    LuaEngine* pEngine = LuaEngine::getInstance();
    string filepath = getLuaFile();
    string path = getLuaPath();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    lua_State* ls = LuaEngine::defaultEngine()->getLuaStack()->getLuaState();
    lua_getglobal(ls, "getActTips");
    lua_pushstring(ls, actId.c_str());
    if (!lua_isfunction(ls, -2))
    {
        lua_pop(ls, 2);
        return "";
    }
    lua_call(ls, 1, 1);
    string ret = (string)lua_tostring(ls, -1);
    return ret;
}

std::string LuaController::getCollectLabel(std::string actId)
{
    addLuaPath();
    if ("" == actId) {
        return "";
    }
    LuaEngine* pEngine = LuaEngine::getInstance();
    string filepath = getLuaFile();
    string path = getLuaPath();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    lua_State* ls = LuaEngine::defaultEngine()->getLuaStack()->getLuaState();
    lua_getglobal(ls, "getCollectLabel");
    lua_pushstring(ls, actId.c_str());
    if (!lua_isfunction(ls, -2)) {
        lua_pop(ls, 2);
        return "";
    }
    lua_call(ls, 1, 1);
    string ret = (string)lua_tostring(ls, -1);
    return ret;
}

std::string LuaController::getBuyBtnLabel(std::string actId)
{
    addLuaPath();
    if ("" == actId) {
        return "";
    }
    LuaEngine* pEngine = LuaEngine::getInstance();
    string filepath = getLuaFile();
    string path = getLuaPath();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    lua_State* ls = LuaEngine::defaultEngine()->getLuaStack()->getLuaState();
    lua_getglobal(ls, "getBuyBtnLabel");
    lua_pushstring(ls, actId.c_str());
    if (!lua_isfunction(ls, -2)) {
        lua_pop(ls, 2);
        return "";
    }
    lua_call(ls, 1, 1);
    string ret = (string)lua_tostring(ls, -1);
    return ret;
}

std::string LuaController::getActRefTip(std::string actId)
{
    addLuaPath();
    if ("" == actId) {
        return "";
    }
    LuaEngine* pEngine = LuaEngine::getInstance();
    string filepath = getLuaFile();
    string path = getLuaPath();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    lua_State* ls = LuaEngine::defaultEngine()->getLuaStack()->getLuaState();
    lua_getglobal(ls, "getActRefTip");
    lua_pushstring(ls, actId.c_str());
    if (!lua_isfunction(ls, -2)) {
        lua_pop(ls, 2);
        return "";
    }
    lua_call(ls, 1, 1);
    string ret = (string)lua_tostring(ls, -1);
    return ret;
}

CCNode* LuaController::createActivityView(ActivityEventObj *obj)
{
    addLuaPath();
    if (!obj) {
        return NULL;
    }
    
    CCDictionary* dic = obj->toDic();
    LuaEngine* pEngine = LuaEngine::getInstance();
    string filepath = getLuaFile();
    string path = getLuaPath();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    lua_State* ls = LuaEngine::defaultEngine()->getLuaStack()->getLuaState();
    lua_getglobal(ls, "createActivityView");
    lua_pushstring(ls, path.c_str());
    lua_newtable(ls);
    CCDictElement* ele = NULL;
    CCDICT_FOREACH(dic, ele)
    {
        if (ele && ele->getObject()) {
            CCString* value = dynamic_cast<CCString*>(ele->getObject());
            std::string key = ele->getStrKey();
            if (!key.empty() && value && value->getCString()) {
                lua_pushstring(ls, key.c_str());
                lua_pushstring(ls, value->getCString());
                lua_settable(ls, -3);
                CCLOG("notice --- key ---%s,value --- %s",key.c_str(),value->getCString());
            }
        }
    }
    if (!lua_isfunction(ls, -3))
    {
        lua_pop(ls, 3);
        return nullptr;
    }
    lua_call(ls, 2, 1);
    CCNode* ret =  *(CCNode**)lua_touserdata(ls, -1);
    return ret;
}

CCNode* LuaController::createActivityRwdView(ActivityEventObj *obj)
{
    addLuaPath();
    if (!obj) {
        return NULL;
    }
    CCDictionary* dic = obj->toDic();
    
    LuaEngine* pEngine = LuaEngine::getInstance();
    string filepath = getLuaFile();
    string path = getLuaPath();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    
    lua_State* ls = pEngine->getLuaStack()->getLuaState();
    lua_getglobal(ls, "createActivityRwdView");
    lua_pushstring(ls, path.c_str());
    lua_newtable(ls);
    CCDictElement* ele = NULL;
    CCDICT_FOREACH(dic, ele)
    {
        if (ele && ele->getObject()) {
            CCString* value = dynamic_cast<CCString*>(ele->getObject());
            std::string key = ele->getStrKey();
            if (!key.empty() && value && value->getCString()) {
                lua_pushstring(ls, key.c_str());
                lua_pushstring(ls, value->getCString());
                lua_settable(ls, -3);
                CCLOG("notice --- key ---%s,value --- %s",key.c_str(),value->getCString());
            }
        }
    }
    if (!lua_isfunction(ls, -3)) {
        lua_pop(ls, 3);
        return nullptr;
    }
    lua_call(ls, 2, 1);
    CCNode* ret =  *(CCNode**)lua_touserdata(ls, -1);
    return ret;
}

CCNode* LuaController::createActivityAdCell(ActivityEventObj *obj)
{
    addLuaPath();
    if (!obj) {
        return NULL;
    }
    CCDictionary* dic = obj->toDic();
    
    LuaEngine* pEngine = LuaEngine::getInstance();
    string filepath = getLuaFile();
    string path = getLuaPath();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    
    lua_State* ls = pEngine->getLuaStack()->getLuaState();
    lua_getglobal(ls, "createActivityAdCell");
    lua_pushstring(ls, path.c_str());
    lua_newtable(ls);
    CCDictElement* ele = NULL;
    CCDICT_FOREACH(dic, ele)
    {
        if (ele && ele->getObject()) {
            CCString* value = dynamic_cast<CCString*>(ele->getObject());
            std::string key = ele->getStrKey();
            if (!key.empty() && value && value->getCString()) {
                lua_pushstring(ls, key.c_str());
                lua_pushstring(ls, value->getCString());
                lua_settable(ls, -3);
                CCLOG("notice --- key ---%s,value --- %s",key.c_str(),value->getCString());
            }
        }
    }
    if(!lua_isfunction(ls, -3))
    {
        lua_pop(ls, 3);
        return nullptr;
    }
    lua_call(ls, 2, 1);
    CCNode* ret =  *(CCNode**)lua_touserdata(ls, -1);
    return ret;
}

CCNode* LuaController::createActivityListCellSprite(ActivityEventObj *obj)
{
    addLuaPath();
    if (!obj) {
        return NULL;
    }
    CCDictionary* dic = obj->toDic();
    
    LuaEngine* pEngine = LuaEngine::getInstance();
    string filepath = getLuaFile();
    string path = getLuaPath();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    
    lua_State* ls = pEngine->getLuaStack()->getLuaState();
    lua_getglobal(ls, "createActivityListCellSprite");
    lua_pushstring(ls, path.c_str());
    lua_newtable(ls);
    CCDictElement* ele = NULL;
    CCDICT_FOREACH(dic, ele)
    {
        if (ele && ele->getObject()) {
            CCString* value = dynamic_cast<CCString*>(ele->getObject());
            std::string key = ele->getStrKey();
            if (!key.empty() && value && value->getCString()) {
                lua_pushstring(ls, key.c_str());
                lua_pushstring(ls, value->getCString());
                lua_settable(ls, -3);
                CCLOG("notice --- key ---%s,value --- %s",key.c_str(),value->getCString());
            }
        }
    }
    if (!lua_isfunction(ls, -3))
    {
        lua_pop(ls, 3);
        return nullptr;
    }
    lua_call(ls, 2, 1);
    CCNode* ret =  *(CCNode**)lua_touserdata(ls, -1);
    return ret;
}

void LuaController::getActExcData()
{
    auto& arr = ActivityController::getInstance()->activityArr;
    if (arr->count() == 0) {
        return;
    }
    CCObject* item = nullptr;
    ActivityEventObj* obj = nullptr;
    std::vector<std::string> vec;
    bool st = false;
    CCARRAY_FOREACH(arr, item)
    {
        obj = dynamic_cast<ActivityEventObj*>(item);
        if (obj->type == 6)
        {
            if (obj->exchange == "2" || obj->exchange == "1") {
                vec.push_back(obj->id);
            }
        }
    }
    std::string ts("");
    if (vec.size()) {
        for (int i = 0; i < vec.size() - 1; ++i) {
            ts += vec[i];
            ts += "|";
        }
        ts += vec[vec.size() - 1];
        CCDictionary* dic = CCDictionary::create();
        dic->setObject(CCString::create(ts), "ids");
        sendCMD("act.exchange", dic);
    }
}

void LuaController::showActRwdViewByActId(std::string actId)
{
    PopupViewController::getInstance()->addPopupView(ActivityRwdView::createByActId(actId));
}

void LuaController::setTitleName(std::string _name)
{
    UIComponent::getInstance()->setPopupTitleName(_name);
}

std::string LuaController::retReward(CCArray* arr)
{
    return RewardController::getInstance()->retReward(arr);
}

void LuaController::flyHint(std::string icon, std::string titleText, std::string context)
{
    CCCommonUtils::flyHint(icon, titleText, context);
}

void LuaController::showTipsView(std::string context, CCTextAlignment align)
{
    PopupViewController::getInstance()->addPopupView(TipsView::create(context, align));
}

void LuaController::showExchangeSale(CCNode* node,GoldExchangeItem* item){
    addLuaPath();
    if(!node)
        return;
    m_saleViewContainer = node;
    GoldExchangeItem* dataItem=item;
    if (dataItem) {
        if(dataItem->popup_image!="new_recharge" && dataItem->popup_image !="month"){//既不是首充,也不是月卡
            
            string pushStr = dataItem->toString();
            LuaEngine* pEngine = LuaEngine::getInstance();
            string path = getLuaPath();
            string filepath = getLuaFile();
            pEngine->addSearchPath(path.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            CCLOG(filepath.c_str());
            CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
            if (pstrFileContent){
                pEngine->executeString(pstrFileContent->getCString());
            }
#else
            filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
            pEngine->executeScriptFile(filepath.c_str());
#endif
            lua_State* ls = pEngine->getLuaStack()->getLuaState();
            lua_getglobal(ls, "showGoldExchangeView");
            lua_pushstring(ls, path.c_str());
            lua_pushlstring(ls, pushStr.c_str(), pushStr.length());
            if (!lua_isfunction(ls, -3))
            {
                lua_pop(ls, 3);
                return;
            }
            lua_call(ls, 2, 0);
            return;
        }
    }
}
CCRect LuaController::createExchangeAdvCell(CCNode* node,std::string &info){
    addLuaPath();
    if(node == NULL)
        return CCRectZero;
    this->m_advCellContainer = node;
    LuaEngine* pEngine = LuaEngine::getInstance();
    string path = getLuaPath();
    string filepath = getLuaFile();
    pEngine->addSearchPath(path.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    if(m_initVersion.compare("1.0.1") == 0 ){
        lua_State* ls = pEngine->getLuaStack()->getLuaState();
        lua_getglobal(ls, "createNewGoldExchangeAdvCell");
//        lua_getglobal(ls, "createGoldExchangeAdvCell");
        lua_pushstring(ls, path.c_str());
        lua_pushlstring(ls, info.c_str(), info.length());
        if (!lua_isfunction(ls, -3))
        {
            lua_pop(ls, 3);
            return CCRectZero;
        }
        lua_call(ls, 2, 0);
    }else{
        lua_State* ls = pEngine->getLuaStack()->getLuaState();
        lua_getglobal(ls, "createNewGoldExchangeAdvCell1");
//        lua_getglobal(ls, "createGoldExchangeAdvCell1");
        lua_pushstring(ls, path.c_str());
        lua_pushlstring(ls, info.c_str(), info.length());
        if (!lua_isfunction(ls, -3))
        {
            lua_pop(ls, 3);
            return CCRectZero;
        }
        lua_call(ls, 2, 4);
        float sizeH  = lua_tonumber(ls, -1);
        lua_pop(ls,1);
        float sizeW  = lua_tonumber(ls, -1);
        lua_pop(ls,1);
        float posY = lua_tonumber(ls, -1);
        lua_pop(ls,1);
        float posX = lua_tonumber(ls, -1);
        lua_pop(ls,1);
        return CCRect(posX,posY+3,sizeW,sizeH);
    }
    return CCRectZero;
}
void LuaController::updateExchangeAdvCell(CCNode* node,std::string &info){
    if(node == NULL)
        return;
    this->m_advCell = node;
    LuaEngine* pEngine = LuaEngine::getInstance();
    string path = getLuaPath();
    string filepath = getLuaFile();
    pEngine->addSearchPath(path.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    lua_State* ls = pEngine->getLuaStack()->getLuaState();
    lua_getglobal(ls, "updateGoldExchangeAdvCell");
    lua_pushlstring(ls, info.c_str(), info.length());
    if (!lua_isfunction(ls, -2))
    {
        lua_pop(ls, 2);
        return;
    }
    lua_call(ls, 1, 0);
}
void LuaController::createExchangeIcon(CCNode *node,std::string &info){
    addLuaPath();
    
    if(node == NULL)
        return;
    this->m_iconContainer=node;
    LuaEngine* pEngine = LuaEngine::getInstance();
    string path = getLuaPath();
    string filepath = getLuaFile();
    pEngine->addSearchPath(path.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    lua_State* ls = pEngine->getLuaStack()->getLuaState();
    lua_getglobal(ls, "createGoldExchangeIcon");
    lua_pushstring(ls, path.c_str());
    lua_pushlstring(ls, info.c_str(), info.length());
    if (!lua_isfunction(ls, -3))
    {
        lua_pop(ls, 3);
        return;
    }
    lua_call(ls, 2, 0);
}
void LuaController::doResourceByCommonIndex(int commonIndex, bool isLoad){
    CCLoadSprite::doResourceByCommonIndex(commonIndex, isLoad);
}
CCParticleSystemQuad* LuaController::createParticleForLua(const char* name){
//    ParticleController::initParticle();
    return ParticleController::createParticleForLua(name);
}
CCParticleSystemQuad* LuaController::createParticleInPoolForLua(const char* name){
    return ParticleController::createParticleInPoolForLua(name);
}
std::string LuaController::getLang(std::string &msg){
    std::string returnVal = _lang(msg);
    return  returnVal;
}
std::string LuaController::getLang1(std::string &msg, std::string &param1){
    std::string returnVal = _lang_1(msg.c_str(), param1.c_str());
    return  returnVal;
}
std::string LuaController::getLang2(std::string &msg, std::string &param1, std::string &param2){
    std::string returnVal = _lang_2(msg.c_str(), param1.c_str(),param2.c_str());
    return  returnVal;
}
std::string LuaController::getLang3(std::string &msg, std::string &param1, std::string &param2, std::string &param3){
    std::string returnVal = _lang_3(msg.c_str(), param1.c_str(), param2.c_str(), param3.c_str());
    return  returnVal;
}
std::string LuaController::getLang4(std::string &msg, std::string &param1, std::string &param2, std::string &param3, std::string &param4){
    std::string returnVal = _lang_4(msg.c_str(), param1.c_str(), param2.c_str(), param3.c_str(), param4.c_str());
    return  returnVal;
}
std::string LuaController::getLang5(std::string &msg, std::string &param1, std::string &param2, std::string &param3, std::string &param4, std::string &param5){
    std::string returnVal = _lang_5(msg.c_str(), param1.c_str(), param2.c_str(), param3.c_str(), param4.c_str(), param5.c_str());
    return  returnVal;
}
std::string LuaController::getDollarString(string &dollar,string &productId){
    std::string returnVal = PayController::getInstance()->getDollarText(dollar,productId);
    return returnVal;
}
std::string LuaController::getCMDLang(std::string &msg){
    int aaa = atoi(msg.c_str());
    std::string returnVal = CC_CMDITOA(aaa);
    return returnVal;
}
void LuaController::addButtonLight(cocos2d::CCNode *node){
    if(!node || !node->getParent())
        return;
    CCNode *animNode = CCNode::create();
    node->getParent()->addChild(animNode);
    animNode->setPosition(node->getPosition());
    CCSize btnSize = node->getContentSize();
    ButtonLightEffect* btnEffect = ButtonLightEffect::create(btnSize);
    animNode->addChild(btnEffect);
//    CCSprite* Spr = CCSprite::create();
//    animNode->addChild(Spr);
//    string curPath = "ButtonLight_%d.png";
//    CCCommonUtils::makeInternalEffectSpr(Spr, curPath, 12, 0.07,0,2.0);
//    Spr->setAnchorPoint(ccp(0.5, 0.5));
//    CCSize size = node->getContentSize();
//    CCSize size1 = Spr->getContentSize();
//    float scalex = node->getContentSize().width/Spr->getContentSize().width;
//    float scaley = 1.0;
//    scaley =(node->getContentSize().height+10)/Spr->getContentSize().height;
//    Spr->setPositionY(3);
//    Spr->setScaleX(scalex);
//    Spr->setScaleY(scaley);
//    CCSize size2 = Spr->getContentSize();
//    Spr->setOpacity(175);
}
std::string LuaController::getResourceNameByType(int itemid){
    return CCCommonUtils::getResourceNameByType(itemid);
}
void LuaController::addItemIcon(cocos2d::CCNode *node,std::string &itemid,Label *lable){
    if(!node)
        return;
//    CCLoadSpriteType loadType = CCLoadSpriteType_DEFAULT;
    string name ="";
    string picStr = "";
    int itemId = atoi(itemid.c_str());
    if (itemId < 10) {// 资源
        picStr = CCCommonUtils::getResourceIconByType(itemId);
        name = CCCommonUtils::getResourceNameByType(itemId);
        CCSprite *iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(2).c_str());
        CCCommonUtils::setSpriteMaxSize(iconBg, 52,true);
        node->addChild(iconBg);
        iconBg->setPosition(ccp(26, 26));
        auto icon = CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_DEFAULT);
        CCCommonUtils::setSpriteMaxSize(icon, 50, true);
        node->addChild(icon);
        icon->setPosition(ccp(25, 25));
    }else {//道具
        CCCommonUtils::createGoodsIcon(itemId, node, CCSize(52, 52));
        auto& toolInfo = ToolController::getInstance()->getToolInfoById(itemId);
        name = toolInfo.getName();
//        picStr = CCCommonUtils::getIcon(itemid);
//        auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(toolInfo.color).c_str());
//        CCCommonUtils::setSpriteMaxSize(iconBg, 52,true);
//        node->addChild(iconBg);
//        iconBg->setPosition(ccp(26, 26));
//        loadType = CCLoadSpriteType_GOODS;
    }
    if(lable){
        lable->setString(name.c_str());
    }
}
void LuaController::addIconByType(cocos2d::CCNode *node,std::string &itemid,Label *lable,int type,int size){
    if(!node)
        return;
    int bgSize = size + 10;
    string name ="";
    string picStr = "";
    if(type == 0){ // goods or resource
        int itemId = atoi(itemid.c_str());
        if (itemId < 10) {// 资源
            picStr = CCCommonUtils::getResourceIconByType(itemId);
            name = CCCommonUtils::getResourceNameByType(itemId);
            CCSprite *iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(2).c_str());
            CCCommonUtils::setSpriteMaxSize(iconBg, bgSize,true);
            node->addChild(iconBg);
            iconBg->setPosition(ccp(bgSize*0.5, bgSize*0.5));
            auto icon = CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_DEFAULT);
            CCCommonUtils::setSpriteMaxSize(icon, size, true);
            node->addChild(icon);
            icon->setPosition(ccp(size*0.5, size*0.5));
        }else {//道具
            CCCommonUtils::createGoodsIcon(itemId, node, CCSize(bgSize, bgSize));
            auto& toolInfo = ToolController::getInstance()->getToolInfoById(itemId);
            name = toolInfo.getName();
        }
    }else if(type == 1){
        auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[atoi(itemid.c_str())];
        name = _lang(eInfo.name);
        string bgPath = CCCommonUtils::getToolBgByColor(eInfo.color);
        auto bgIcon = CCLoadSprite::createSprite(bgPath.c_str());
        CCCommonUtils::setSpriteMaxSize(bgIcon, bgSize, true);
        node->addChild(bgIcon);
        bgIcon->setPosition(ccp(bgSize*0.5, bgSize*0.5));
        picStr = CCCommonUtils::getIcon(itemid);
        auto icon = CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_EQUIP);
        CCCommonUtils::setSpriteMaxSize(icon, size, true);
        node->addChild(icon);
        icon->setPosition(ccp(size*0.5, size*0.5));
    }
    if(lable){
        lable->setString(name.c_str());
    }
}
//CCSprite* LuaController::createRwdIcon(cocos2d::CCDictionary *dict, int size)
//{
//    if (!dict) return nullptr;
//    int type = dict->valueForKey("type")->intValue();
//    int num = 0;
//    
//    string picStr = "";
//    bool dPath = false;
//    if (type == R_GOODS) {
//        auto value = _dict(dict->objectForKey("value"));
//        string rid = value->valueForKey("id")->getCString();
//        num = value->valueForKey("num")->intValue();
//        picStr = CCCommonUtils::getIcon(rid);
//        dPath = true;
//    } else {
//        num = dict->valueForKey("value")->intValue();
//        picStr = RewardController::getInstance()->getPicByType(type, 0);
//    }
//    auto icon = dPath? CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_GOODS):CCLoadSprite::createSprite(picStr.c_str());
//    CCCommonUtils::setSpriteMaxSize(icon, size, true);
//    return icon;
//}
CCSprite* LuaController::createGoodsIcon(std::string &itemId, int size)
{
    std::string picStr = CCCommonUtils::getIcon(itemId);
    auto icon = CCLoadSprite::createSprite(picStr.c_str(),true,CCLoadSpriteType_GOODS);
    if (!icon) return nullptr;
    CCCommonUtils::setSpriteMaxSize(icon, size, true);
    return icon;
}
CCDictionary* LuaController::getGoodsInfo(std::string &itemId)
{
    int toolID = atoi(itemId.c_str());
    if(ToolController::getInstance()->m_toolInfos.find(toolID)!=ToolController::getInstance()->m_toolInfos.end())
    {
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(toolID);
        return info.toDic();
    } else {
        return nullptr;
    }
}
std::string LuaController::getObjectType(CCObject* obj)
{
    if (!obj) return "NULL";
    if(dynamic_cast<CCString*>(obj)) {
        return "CCString";
    } else if (dynamic_cast<CCArray*>(obj)) {
        return "CCArray";
    } else if (dynamic_cast<CCDictionary*>(obj)) {
        return "CCDictionary";
    } else if (dynamic_cast<CCInteger*>(obj)) {
        return "CCInteger";
    } else {
        return "NULL";
    }
}
int LuaController::getWorldTime(){
   return (int)GlobalData::shared()->getWorldTime();
}
int LuaController::getTimeStamp()
{
    return (int)GlobalData::shared()->getTimeStamp();
}
void LuaController::playEffects(std::string name)
{
    SoundController::sharedSound()->playEffects(name.c_str());
}
std::string LuaController::getSECLang(int time){
    return CC_SECTOA(time);
}

void LuaController::callPaymentToFriend(std::string &itemid, std::string touid,std::string toName){
    
    GoldExchangeItem *dataItem = NULL;
    for (map<string, GoldExchangeItem*>::iterator it = GlobalData::shared()->goldExchangeList.begin(); it != GlobalData::shared()->goldExchangeList.end(); it++) {
        dataItem = it->second;
        if(dataItem==NULL)
            continue;
        if(dataItem->type == "3"){
            if(dataItem->bought||dataItem->popup_image=="close"){
                continue;
            }
            if(dataItem->end>GlobalData::shared()->getWorldTime()){
                if(dataItem!=NULL && dataItem->id.compare(itemid) == 0){
                    if(LuaController::getInstance()->canSendExchange(itemid,touid,toName,3,false)){
                        PayController::getInstance()->callPaymentToFriend(dataItem, touid);
                    }
                    return;
                }
            }
        }
    }
}
void LuaController::callPayment(std::string &itemid){
    GoldExchangeItem *dataItem = NULL;
    for (map<string, GoldExchangeItem*>::iterator it = GlobalData::shared()->goldExchangeList.begin(); it != GlobalData::shared()->goldExchangeList.end(); it++) {
        dataItem = it->second;
        if(dataItem==NULL)
            continue;
        if(dataItem->type == "3"){
            if(dataItem->bought||dataItem->popup_image=="close"){
                continue;
            }
            if(dataItem->end>GlobalData::shared()->getWorldTime()){
                if(dataItem!=NULL && dataItem->id.compare(itemid) == 0){
                    PayController::getInstance()->callPayment(dataItem,"GoldExchangePopupView");
                    return;
                }
            }
        }else if(dataItem->type == "1"){
            if(dataItem->bought || dataItem->popup_image=="hide" || dataItem->popup_image.empty()){
                continue;
            }
            if(dataItem->end>GlobalData::shared()->getWorldTime()){
                if(dataItem!=NULL && dataItem->id.compare(itemid) == 0){
                    PayController::getInstance()->callPayment(dataItem,"GoldExchangePopupView");
                    return;
                }
            }
        }
    }
}
void LuaController::removeAllPopup(){
    PopupViewController::getInstance()->removeAllPopupView();
//    ActivityController::getInstance()->enterGameOpenView();
}
void LuaController::removeLastPopup()
{
    PopupViewController::getInstance()->removeLastPopupView();
}
void LuaController::removeAllPopupView()
{
    PopupViewController::getInstance()->removeAllPopupView();
}
void LuaController::openBagView(int type)
{
    auto view = StoreView::create();
    if (type == 0)
    {
        view->onClickBagBtn(NULL, Control::EventType::TOUCH_DOWN);
    }
    else
    {
        view->onClickStoreBtn(NULL, Control::EventType::TOUCH_DOWN);
    }
    PopupViewController::getInstance()->addPopupInView(view);
}
void LuaController::showDetailPopup(std::string &itemid){
    PopupViewController::getInstance()->addPopupView(GoldExchangeMoreListView::create(atoi(itemid.c_str())));
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GOLDEXCHANGE_ADVERTISING_STOP_SCROLL);
}
bool LuaController::checkSkeletonFile(const char *fileName){
    std::string path = LuaController::getInstance()->getLuaPath() + "/skeleton/" + fileName;
    std::string path1 = path + ".atlas";
    std::string path2 = path + ".json";
    if(CCFileUtils::sharedFileUtils()->isFileExist(path1) && CCFileUtils::sharedFileUtils()->isFileExist(path2)){
        return true;
    }
    return false;
}
void LuaController::addSkeletonAnimation(cocos2d::CCNode *node, const char* skeletonDataFile, const char* atlasFile,const char* aniName,float scale){
    spine::SkeletonAnimation* skAni = spine::SkeletonAnimation::createWithFile(skeletonDataFile,atlasFile,scale);
    node->addChild(skAni);
    skAni->setAnimation(0, aniName, true);
}
int LuaController::getStringLength(std::string &str){
    return Utf8Utils::strlen(str.c_str());
}
std::string LuaController::getUTF8SubString(std::string &str,int start,int end){
    return  CCCommonUtils::subStrByUtf8(str,start,end-start+1);
}
void LuaController::toSelectUser(std::string itemid,bool removeAllPop,int backTo){
    if(removeAllPop){
        PopupViewController::getInstance()->removeAllPopupView();
    }
    PayController::getInstance()->setItemId(itemid);
    auto pop = AllianceInviteView::create(0, ITEM_DONATE,itemid);
    pop->setBackTo(backTo);
    PopupViewController::getInstance()->addPopupInView(pop);
}
std::string LuaController::getPlatform(){
    return GlobalData::shared()->analyticID;
}
std::string LuaController::getLanguage(){
    return LocalController::shared()->getLanguageFileName();
}
CCNode* LuaController::getSaleViewContainer(){
    return this->m_saleViewContainer;
}
CCNode* LuaController::getAdvCellContainer(){
    return this->m_advCellContainer;
}
CCNode* LuaController::getAdvCell(){
    return this->m_advCell;
}
CCNode* LuaController::getIconContainer(){
    return this->m_iconContainer;
}
//下载相关内容
void LuaController::downLoadZip(CCObject* ccObj){
    LuaDownLoadRef *downRef = dynamic_cast<LuaDownLoadRef*>(ccObj);
    if (downRef) {
        if(!this->downloadFile(downRef)){
            downRef->release();
            return;
        }
        this->downloadFinish(downRef);
        downRef->release();
    }
}
void LuaController::uncompressZip(CCObject* ccObj){
    LuaDownLoadRef *downRef = dynamic_cast<LuaDownLoadRef*>(ccObj);
    if (downRef) {
        if (!this->unZipFile(downRef)){
            downRef->release();
            return;
        }
        this->unzipFinish(downRef);
        downRef->release();
    }
}
bool LuaController::isDownloading(std::string packname){
    if(find(mDownQuene.begin(),mDownQuene.end(),packname) == mDownQuene.end()){
        return false;
    }
    return true;
}
bool LuaController::isUncompressing(std::string packname){
    if(find(mUnzipQuene.begin(),mUnzipQuene.end(),packname) == mUnzipQuene.end()){
        return false;
    }
    return true;
}
void LuaController::removeFromDown(string packname){
    auto iter = find(mDownQuene.begin(),mDownQuene.end(),packname);
    if(iter != mDownQuene.end()){
        mDownQuene.erase(iter);
    }
}
void LuaController::removeFromUnzip(string packname){
    auto iter = find(mUnzipQuene.begin(),mUnzipQuene.end(),packname);
    if(iter != mUnzipQuene.end()){
        mUnzipQuene.erase(iter);
    }
}
//void* LuaFileDownloadAndUncompress(void *data)
//{
//    LuaController* self = (LuaController*)data;
//    do
//    {
//        if (!self->downloadFile()) {
//            break;
//        }
//        self->downloadFinish();
//        // Uncompress zip file.
////        if (!self->unZipFile()){
////            break;
////        }
////        self->unzipFinish();
//        
//    } while (0);
//    
//    if (self->m_tid)
//    {
//        delete self->m_tid;
//        self->m_tid = NULL;
//    }
//    return NULL;
//}
static size_t downLoadLuaPackage(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    FILE *fp = (FILE*)userdata;
    size_t written = fwrite(ptr, size, nmemb, fp);
    return written;
}
int LuaDownloadProgressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
    //    UpdateManager* manager = (UpdateManager*)ptr;
    //    UpdateManager::UMMessage *msg = new UpdateManager::UMMessage();
    //    msg->what = UpdateMANAGER_MESSAGE_PROGRESS;
    //
    //    ProgressMessage *progressData = new ProgressMessage();
    auto percent = (int)(nowDownloaded/totalToDownload*100);
    percent = percent < 0 ? 0 : percent;
    //    progressData->percent = percent;
    //    progressData->manager = manager;
    //    msg->obj = progressData;
    //    manager->_schedule->sendMessage(msg);
//    CCLOG("lua___downloading... %d%%", percent);
    return 0;
}
//解压线程
void LuaController::startUnzipFile(LuaDownLoadRef *downRef){
    mUnzipQuene.push_back(downRef->getPackName());
    CCThreadManager::getInstance()->runInNetThread(this, callfuncO_selector(LuaController::uncompressZip),downRef);
}

bool LuaController::checkUpdate(){
    m_serverVersion = this->luaVersion;
    if(m_clientVersion.compare(m_serverVersion) == 0){
        return false;
    }
    LuaDownLoadRef *downref = LuaDownLoadRef::create();
    downref->retain();
    downref->setPackName("lua");
    downref->setPackVersion(luaVersion);
    downref->setCheckMD5(this->luaMD5Code);
    downref->setPackType(LuaDownloadType_LUA);
    downref->setSize(this->luaSize);
    downref->setZipSize(this->luazipSize);
    
    if(m_downedVersion.compare(m_serverVersion) == 0){
        int needSize = this->luazipSize;
        if(!cocos2d::extension::CCDevice::hasEnoughSpace(needSize)){
            CCLOG("do not has enough space to unzip lua file!");
            return false;
        }
        startUnzipFile(downref);
        return false;
    }
    mDownQuene.push_back("lua");
    startDownload(downref);
    return true;
}
void LuaController::startDownload(LuaDownLoadRef *downRef){
    int needSize = downRef->getSize() + downRef->getZipSize();
    if(!cocos2d::extension::CCDevice::hasEnoughSpace(needSize))
        return;
    CCThreadManager::getInstance()->runInNetThread(this, callfuncO_selector(LuaController::downLoadZip),downRef);
}
bool LuaController::unZipFile(LuaDownLoadRef *downRef){

    CCLOG("lua___unzip_start");
    // Open the zip file
    string outFileName = m_writeablePath +downRef->getPackName()+ LUA_ZIP;
    if(!CCFileUtils::sharedFileUtils()->isFileExist(outFileName)){
        CCLOG("zip file %s is not exit",outFileName.c_str());
        return false;
    }
    if(true){
        std::ifstream ifs;
        ifs.open(outFileName);
        MD5 md5;
        md5.update(ifs);
        std::string zipStr = md5.toString();
        std::string checkStr = downRef->getCheckMD5();
        if(zipStr.compare(checkStr) != 0){
            CCLOG("zip file %s is not correct. check code [%s], download code [%s]",outFileName.c_str(),checkStr.c_str(),zipStr.c_str());
            return false;
        }
    }
    unzFile zipfile = unzOpen(outFileName.c_str());
    if (! zipfile)
    {
        CCLOG("can not open downloaded zip file %s", outFileName.c_str());
        return false;
    }
    
    // Get info about the zip file
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
    {
        CCLOG("can not read file global info of %s", outFileName.c_str());
        unzClose(zipfile);
        return false;
    }
    
    // Buffer to hold data read from the zip file
    char readBuffer[LUA_BUFFER_SIZE];
    
    CCLOG("start uncompressing");
    
    // Loop to extract all files.
    uLong i;
    for (i = 0; i < global_info.number_entry; ++i)
    {
        // Get info about current file.
        unz_file_info fileInfo;
        char fileName[LUA_MAX_FILENAME];
        if (unzGetCurrentFileInfo(zipfile,
                                  &fileInfo,
                                  fileName,
                                  LUA_MAX_FILENAME,
                                  NULL,
                                  0,
                                  NULL,
                                  0) != UNZ_OK)
        {
            CCLOG("can not read file info");
            unzClose(zipfile);
            return false;
        }
        
        string fullPath = m_writeablePath + fileName;
        
        // Check if this entry is a directory or a file.
        const size_t filenameLength = strlen(fileName);
        if (fileName[filenameLength-1] == '/')
        {
            // Entry is a direcotry, so create it.
            // If the directory exists, it will failed scilently.
            if (!createDirectory(fullPath.c_str()))
            {
                CCLOG("can not create directory %s", fullPath.c_str());
                unzClose(zipfile);
                return false;
            }
        }
        else
        {
            // Entry is a file, so extract it.
            
            // Open current file.
            if (unzOpenCurrentFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not open file %s", fileName);
                unzClose(zipfile);
                return false;
            }
            
            // Create a file to store current file.
            FILE *out = fopen(fullPath.c_str(), "w+");
            if (! out)
            {
                CCLOG("can not open destination file %s", fullPath.c_str());
                unzCloseCurrentFile(zipfile);
                unzClose(zipfile);
                return false;
            }
            
            // Write current file content to destinate file.
            int error = UNZ_OK;
            do
            {
                error = unzReadCurrentFile(zipfile, readBuffer, LUA_BUFFER_SIZE);
                if (error < 0)
                {
                    CCLOG("can not read zip file %s, error code is %d", fileName, error);
                    unzCloseCurrentFile(zipfile);
                    unzClose(zipfile);
                    return false;
                }
                
                if (error > 0)
                {
                    fwrite(readBuffer, error, 1, out);
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
                CCLOG("can not read next file");
                unzClose(zipfile);
                return false;
            }
        }
    }
    CCLOG("end uncompressing");
    return true;
}
bool LuaController::createDirectory(const char *path)
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

bool LuaController::downloadFile(LuaDownLoadRef *downRef){
    string m_packagePath = "";
// COCOS2D_DEBUG : 0是外网,1是内网,2是beta
#if COCOS2D_DEBUG == 1
    string hostIP = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_IP);
    CCLOG("lua_host_url [%s]",hostIP.c_str());
    if (hostIP.empty()) {
        hostIP = SERVER_IP;
    }
    if(downRef->getPackType() == LuaDownloadType_RESOURCE){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(downRef->bPlatform==true){
            m_packagePath = "http://"+hostIP+":8080/gameservice/getfile?luaVersion=" + downRef->getPackName() + "_a";
        }else{
            m_packagePath = "http://"+hostIP+":8080/gameservice/getfile?luaVersion=" + downRef->getPackName();
        }
#else
        m_packagePath = "http://"+hostIP+":8080/gameservice/getfile?luaVersion=" + downRef->getPackName();
#endif
    }else{
        m_packagePath = "http://"+hostIP+":8080/gameservice/getfile?luaVersion=" + downRef->getPackVersion();
    }
#else
    if(downRef->getPackType() == LuaDownloadType_RESOURCE){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(downRef->bPlatform==true){
            m_packagePath = "http://cok.eleximg.com/cok/lua/lua_" + downRef->getPackName()+"_a_"+downRef->getCheckMD5() + ".zip";
        }else{
            m_packagePath = "http://cok.eleximg.com/cok/lua/lua_" + downRef->getPackName()+"_"+downRef->getCheckMD5() + ".zip";
        }
#else
        m_packagePath = "http://cok.eleximg.com/cok/lua/lua_" + downRef->getPackName()+"_"+downRef->getCheckMD5() + ".zip";
#endif
    }else{
        m_packagePath = "http://cok.eleximg.com/cok/lua/lua_" + downRef->getPackVersion() + ".zip";
    }
#endif
    CCLOG("lua_package_url [%s]",m_packagePath.c_str());
    
    m_curl = curl_easy_init();
    if (! m_curl)
    {
        CCLOG("can not init curl");
        return false;
    }
    
    // Create a file to save package.
    string outFileName = m_writeablePath + downRef->getPackName() +LUA_ZIP;
//    CCLOG("lua_package_save_path [%s]",outFileName.c_str());
    FILE *fp = fopen(outFileName.c_str(), "w+");
    if (! fp){
//        CCLOG("can not create file %s", outFileName.c_str());
        curl_easy_cleanup(m_curl);
        return false;
    }
 
    // Download pacakge
    CURLcode res;
    curl_easy_setopt(m_curl, CURLOPT_URL, m_packagePath.c_str());
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, downLoadLuaPackage);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, false);
    curl_easy_setopt(m_curl, CURLOPT_PROGRESSFUNCTION, LuaDownloadProgressFunc);
    curl_easy_setopt(m_curl, CURLOPT_PROGRESSDATA, this);
    curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1);
    res = curl_easy_perform(m_curl);
    curl_easy_cleanup(m_curl);
    if (res != 0)
    {
        CCLOG("error when download package");
        fclose(fp);
        return false;
    }
    
    CCLOG("succeed downloading package %s", m_packagePath.c_str());
    
    fclose(fp);
    return true;
}
void LuaController::downloadFinish(LuaDownLoadRef *downRef){
    LuaDownLoadRef *param = LuaDownLoadRef::create();
    param->copy(downRef);
    param->retain();
    CCThreadManager::getInstance()->runInMainThread(this, callfuncO_selector(LuaController::saveDownloadVersion),param);
}
void LuaController::unzipFinish(LuaDownLoadRef *downRef){
    if (downRef->getPackType() == LuaDownloadType_LUA) {
        LuaDownLoadRef *param = LuaDownLoadRef::create();
        param->copy(downRef);
        param->retain();
        CCThreadManager::getInstance()->runInMainThread(this, callfuncO_selector(LuaController::saveClientVersion),param);
    }else if(downRef->getPackType() == LuaDownloadType_RESOURCE){
        LuaDownLoadRef *param = LuaDownLoadRef::create();
        param->copy(downRef);
        param->retain();
        CCThreadManager::getInstance()->runInMainThread(this, callfuncO_selector(LuaController::saveClientVersion1),param);
    }
}
void LuaController::changeUI(float t){
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GOLDEXCHANGE_LIST_CHANGE);
    setTranslateMark();
}
void LuaController::resetVersionAndUpdate(){
    m_downedVersion="";
    m_clientVersion="";
//    string path = getLuaPath() + "/version.bin";
//    std::remove(path.c_str());
    checkUpdate();
}
void LuaController::saveDownloadVersion(CCObject *ccObj){
    LuaDownLoadRef *downRef = dynamic_cast<LuaDownLoadRef*>(ccObj);
    if(downRef != NULL){
        if(downRef->getPackType() == LuaDownloadType_LUA){
            m_downedVersion = m_serverVersion;
            CCUserDefault::sharedUserDefault()->setStringForKey(LUA_DOWNLOADED_VERSION, m_downedVersion);
            CCUserDefault::sharedUserDefault()->flush();
            std::string aaaa = CCUserDefault::sharedUserDefault()->getStringForKey(LUA_DOWNLOADED_VERSION);
            CCLOG("lua___download_____end:%s,%s",m_downedVersion.c_str(),aaaa.c_str());
        }else if(downRef->getPackType() == LuaDownloadType_RESOURCE){
            
        }
        LuaDownLoadRef *param = LuaDownLoadRef::create();
        param->copy(downRef);
        param->retain();
        startUnzipFile(param);
        removeFromDown(downRef->getPackName());
        downRef->release();
    }
}
void LuaController::saveClientVersion(CCObject *ccObj){
    LuaDownLoadRef *downRef = dynamic_cast<LuaDownLoadRef*>(ccObj);
    if(downRef != NULL){
        std::string oVersionFile = getLuaPath() + "/"+LUA_VERSION_FILE;
        if(CCFileUtils::sharedFileUtils()->isFileExist(oVersionFile)){
            ifstream ifile(oVersionFile);
            string line;
            vector<string> mVector;
            while(getline(ifile,line)){
                if(!line.empty()){
                    mVector.push_back(line);
                }
            }
            ifile.close();
            if(mVector.size() == 0){
                return;
            }
            auto iter = mVector.begin();
            std::string version = (*iter);
            ++iter;
            while (iter!=mVector.end()) {
                vector<std::string> mTmpV;
                CCCommonUtils::splitString((*iter), "=", mTmpV);
                if(mTmpV.size()>1){
                    m_md5Map[mTmpV.at(0)] = mTmpV.at(1);
                }
                ++iter;
            }
            ofstream ofile(oVersionFile,ofstream::out);
            ofile<<version<<endl;
            auto iterMD5 = m_md5Map.begin();
            while (iterMD5!=m_md5Map.end()) {
                string writeStr =(*iterMD5).first + "=" + (*iterMD5).second;
                ofile<<writeStr<<endl;
                ++iterMD5;
            }
            ofile.close();
        }
        m_clientVersion = m_serverVersion;
        m_downedVersion = "";
        CCUserDefault::sharedUserDefault()->setStringForKey(LUA_CLIENT_VERSION, m_clientVersion);
        CCUserDefault::sharedUserDefault()->setStringForKey(LUA_DOWNLOADED_VERSION, "");
        CCUserDefault::sharedUserDefault()->flush();
    //    std::string aaaa = CCUserDefault::sharedUserDefault()->getStringForKey(LUA_CLIENT_VERSION);
    //    CCLOG("lua___unzip_____end:%s,%s",m_clientVersion.c_str(),aaaa.c_str());
        string path = m_writeablePath + downRef->getPackName() +LUA_ZIP;
        std::remove(path.c_str());
        removeFromUnzip(downRef->getPackName());
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(LuaController::changeUI), this, 2, 0, 0, false);
        downRef->release();
    }
}
void LuaController::saveClientVersion1(CCObject *ccObj){
    LuaDownLoadRef *downRef = dynamic_cast<LuaDownLoadRef*>(ccObj);
    if(downRef != NULL){
        std::string nVersionFile = getLuaPath() + "/pack/version.bin";
        int newVInt = getVersionData(nVersionFile);
        std::string oVersionFile = getLuaPath() + "/"+LUA_VERSION_FILE;
        int oldVInt = getVersionData(oVersionFile);
        ofstream ofile(oVersionFile,ofstream::out);
        m_md5Map[downRef->getPackName()] = downRef->getCheckMD5();
        int checkV = oldVInt;
        if(newVInt>oldVInt){
            std::string version = CC_ITOA(newVInt);
            ofile<<version<<endl;
            replaceNewLuaFile();
            checkV = newVInt;
        }else{
            std::string version = CC_ITOA(oldVInt);
            ofile<<version<<endl;
        }
        std::remove(nVersionFile.c_str());
        replaceNewLuaFile1(checkV);
        auto iterMD5 = m_md5Map.begin();
        while (iterMD5!=m_md5Map.end()) {
            string writeStr =(*iterMD5).first + "=" + (*iterMD5).second;
            ofile<<writeStr<<endl;
            ++iterMD5;
        }
        ofile.close();
        string path = m_writeablePath + downRef->getPackName() + LUA_ZIP;
        std::remove(path.c_str());
        CCLOG("lua___unzip_____end");
        removeFromUnzip(downRef->getPackName());
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(LuaController::changeUI), this, 2, 0, 0, false);
        downRef->release();
    }
}
void LuaController::updateNewResource(string packName,string packMd5,bool bplatform){
    LuaDownLoadRef *downRef = LuaDownLoadRef::create();
    downRef->setPackName(packName);
    downRef->setCheckMD5(packMd5);
    downRef->setPackVersion(packMd5);
    downRef->setSize(250);
    downRef->setZipSize(600);
    downRef->setPackType(LuaDownloadType_RESOURCE);
    downRef->bPlatform=bplatform;
    downRef->retain();
    int needSize = 850;
    if(!cocos2d::extension::CCDevice::hasEnoughSpace(needSize))
        return;
    mDownQuene.push_back(packName);
    CCThreadManager::getInstance()->runInNetThread(this, callfuncO_selector(LuaController::downLoadZip),downRef);
    
}

//#define LUA_FILE_COUNT 8
//const std::string luaFileList[LUA_FILE_COUNT] ={"main","GoldExchangeView","GoldExchangeIcon","GoldExchangeCell","GoldExchangeAdvCell","extern","common","CCBReaderLoad"};
void LuaController::replaceNewLuaFile(){
    auto iter = m_md5Map.find("files");
    vector<std::string> tmpFiles;
    if(iter!=m_md5Map.end()){
        CCCommonUtils::splitString((*iter).second, ",", tmpFiles);
    }
    std::string packPath =getLuaPath() + "/pack";
    auto iter1 = tmpFiles.begin();
    while (iter1!=tmpFiles.end()) {
        std::string path = packPath + "/"+(*iter1)+".lua";
        if(CCFileUtils::sharedFileUtils()->isFileExist(path)){
            unsigned char* buf = NULL;
            ssize_t bufSize = 0;
            buf = CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "r", &bufSize);
            std::string newPath = getLuaPath() + "/" + (*iter1)+".lua";
            FILE* file = fopen(newPath.c_str(), "w");
            if(file){
                fwrite(buf, bufSize, 1, file);
                fclose(file);
            }
            std::remove(path.c_str());
        }
        ++iter1;
    }
    tmpFiles.clear();
    std::remove(packPath.c_str());
}
void LuaController::replaceNewLuaFile1(int v){
    std::string packPath =getLuaPath() + "/pack";
    std::string nVersionFile = packPath + "/version1.bin";
    int newV = getVersionData(nVersionFile);
    if (newV==0) {
        return;
    }
    std::remove(nVersionFile.c_str());
    
    vector<string> notReplaceFiles;
    string saveStr = "";
    auto iter =m_md5Map.find("fileV");
    if(iter!=m_md5Map.end()){
        vector<string> tmpV;
        CCCommonUtils::splitString((*iter).second, ";", tmpV);
        auto itr = tmpV.begin();
        while (itr!=tmpV.end()) {
            vector<string> tmpV1;
            CCCommonUtils::splitString((*itr), ":", tmpV1);
            if (tmpV1.size()>1) {
                int oldV = atoi( tmpV1.at(tmpV1.size()-1).c_str());
                if (newV<=oldV) {
                    auto it = tmpV1.end()-2;
                    while (it>=tmpV1.begin()) {
                        notReplaceFiles.push_back((*it));
                        --it;
                    }
                    saveStr = saveStr.empty()?(*itr):(saveStr + ";"+ (*itr));
                }else{
                    string tmpStr = "";
                    auto it = tmpV1.end()-2;
                    while (it>=tmpV1.begin()) {
                        std::string path = packPath + "/"+(*it)+".lua";
                        if(!CCFileUtils::sharedFileUtils()->isFileExist(path)){
                            tmpStr = tmpStr.empty()?(*it):(tmpStr + ":"+ (*it));
                        }
                        --it;
                    }
                    if(!tmpStr.empty()){
                        tmpStr = tmpStr + ":" + CC_ITOA(oldV);
                        saveStr = saveStr.empty()?tmpStr:(saveStr + ";"+ tmpStr);
                    }
                }
            }
            ++itr;
        }
    }
    vector<string> replaceFiles;
    vector<string> tmpFiles;
    auto iter1 =m_md5Map.find("efiles");
    if(iter1!=m_md5Map.end()){
        CCCommonUtils::splitString((*iter1).second, ",", tmpFiles);
    }
    
    auto iter2 = tmpFiles.begin();
    while (iter2!=tmpFiles.end()) {
        std::string path = packPath + "/"+(*iter2)+".lua";
        if(!CCFileUtils::sharedFileUtils()->isFileExist(path)){
            ++iter2;
            continue;
        }
        if (find(notReplaceFiles.begin(), notReplaceFiles.end(), (*iter2)) != notReplaceFiles.end()) {
            ++iter2;
            continue;
        }
        xxtea_long len = 0;
        unsigned char* result = this->getXXTeaDecode(path,len);
        //encrypt XXTEA
        xxtea_long retLen = 0;
        unsigned char * encrypt = xxtea_encrypt(result, len, (unsigned char*)ENCRYPT_KEY, (xxtea_long)strlen(ENCRYPT_KEY), &retLen);
        std::string content((char*)encrypt, retLen);
        content = ENCRYPT_SIGN + content;
        
        std::string newPath = getLuaPath() + "/" + (*iter2)+".lua";
        FILE* file = fopen(newPath.c_str(), "w");
        if(file){
            fwrite(content.c_str(), content.length(), 1, file);
            fclose(file);
        }
        std::remove(path.c_str());
        replaceFiles.push_back((*iter2));
        ++iter2;
    }
    tmpFiles.clear();
    if (v < newV ) {
        auto iter3 = m_md5Map.find("files");
        if(iter3!=m_md5Map.end()){
            CCCommonUtils::splitString((*iter3).second, ",", tmpFiles);
        }
        auto iter4 = tmpFiles.begin();
        while (iter4!=tmpFiles.end()) {
            std::string path = packPath + "/"+(*iter4)+".lua";
            if(!CCFileUtils::sharedFileUtils()->isFileExist(path)){
                ++iter4;
                continue;
            }
            if (find(notReplaceFiles.begin(), notReplaceFiles.end(), (*iter4)) != notReplaceFiles.end()) {
                ++iter4;
                continue;
            }
            
            unsigned char* buf = NULL;
            ssize_t bufSize = 0;
            buf = CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "r", &bufSize);
            std::string newPath = getLuaPath() + "/" + (*iter4)+".lua";
            FILE* file = fopen(newPath.c_str(), "w");
            if(file){
                fwrite(buf, bufSize, 1, file);
                fclose(file);
            }
            std::remove(path.c_str());
            replaceFiles.push_back((*iter4));
            ++iter4;
        }
    }
    
    std::remove(packPath.c_str());
    string reStr = "";
    auto reIt = replaceFiles.begin();
    while (reIt!=replaceFiles.end()) {
        reStr = reStr.empty()?(*reIt):(reStr + ":" + (*reIt));
        ++reIt;
    }
    if (!reStr.empty()) {
        reStr = reStr + ":" + CC_ITOA(newV);
        saveStr = saveStr.empty()?reStr:(saveStr + ";"+ reStr);
    }
    if(!saveStr.empty()){
        m_md5Map["fileV"]=saveStr;
    }
}
bool LuaController::checkLuaFiles(){
    auto iter = m_md5Map.find("files");
    vector<std::string> tmpFiles;
    if(iter!=m_md5Map.end()){
        CCCommonUtils::splitString((*iter).second, ",", tmpFiles);
    }else{
        return false;
    }
    auto iter1 = tmpFiles.begin();
    while (iter1!=tmpFiles.end()) {
        std::string filePath =getLuaPath() + "/" +(*iter1)+".lua";
        if(!CCFileUtils::sharedFileUtils()->isFileExist(filePath)){
            return false;
        }
        ++iter1;
    }
    return true;
}
int LuaController::getVersionData(std::string filePath){
    if(CCFileUtils::sharedFileUtils()->isFileExist(filePath)){
        ifstream ifile(filePath);
        string line;
        getline(ifile,line);
        ifile.close();
        if(!line.empty()){
            return atoi(line.c_str());
        }
    }
    return 0;
}
bool LuaController::initMD5Info(){
    if(m_md5Map.size()>0)
        return true;
    std::string oVersionFile = getLuaPath() + "/"+LUA_VERSION_FILE;
    if(CCFileUtils::sharedFileUtils()->isFileExist(oVersionFile)){
        unsigned char* buf = NULL;
        ssize_t bufSize = 0;
        buf = CCFileUtils::sharedFileUtils()->getFileData(oVersionFile.c_str(), "r", &bufSize);
        string tmpFile= std::string((const char*)buf, bufSize);
        vector<string> mVector;
        CCCommonUtils::splitString(tmpFile, "\n", mVector);
        //防止vector内元素数目不对引起崩溃
        if(mVector.size() == 0){
            return false;
        }
        //
        auto iter = mVector.begin()+1;
        while (iter!=mVector.end()) {
            string tmpStr = (*iter);
            if(!tmpStr.empty() && tmpStr.find("=")!=string::npos){
                vector<std::string> mTmpV;
                CCCommonUtils::splitString(tmpStr, "=", mTmpV);
                if(mTmpV.size()>1){
                    m_md5Map[mTmpV.at(0)]=mTmpV.at(1);
                }
            }
            ++iter;
        }
        return true;
    }
    return false;
}

//
//Chat Notice 相关代码
//
void LuaController::addSystemUpdate(double countDown, bool isLogin,string tip,FlyHintType type,string icon){
    if (GuideController::share()->isInTutorial()) {
        return;
    }
    CCDictionary *dict = CCDictionary::create();
    dict->retain();
    dict->setObject(CCDouble::create(countDown), "countDown");
    dict->setObject(CCBool::create(isLogin), "isLogin");
    dict->setObject(CCString::create(tip), "tip");
    dict->setObject(CCInteger::create(type), "type");
    dict->setObject(CCString::create(icon), "icon");
    dict->setObject(CCString::create("SystemUpdate"), "hintType");
    
    if(type == FLY_HINT_LOGIN){ //停机公告优先处理
        if(mChatNoticeNode){
            mChatNoticeNode->removeAllChildren();
            mChatNoticeNode->stopAllActions();
//            mChatNoticeNode->removeFromParent();
//            mChatNoticeNode=NULL;
        }
        createChatNotice(dict);
    }else if(type == FLY_HINT_SHAKEALLIANCE){
        CCLOGFUNC("fly log for alliance!");
        createChatNotice(dict);
    }
    else{
        if(mChatNoticeNode && mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG)){
            mChatNoticeV.push_back(dict);
        }else{
            createChatNotice(dict);
        }
    }
}
void LuaController::addChatNotice(cocos2d::CCDictionary *dict){
    if(ChatController::getInstance()->getIsNoticeOpen() == false){
        return;
    }
    if (GuideController::share()->isInTutorial()) {
        return;
    }
    dict->retain();
    if(mChatNoticeNode && mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG)){
        bool st = true;
        if (dict->objectForKey("tag"))
        {
            string tag = dict->valueForKey("tag")->getCString();
            if (tag == "alliance_mark")
            {
                for (int i=0; i<mChatNoticeV.size(); i++)
                {
                    if (mChatNoticeV[i]->objectForKey("tag")) {
                        string tagV = mChatNoticeV[i]->valueForKey("tag")->getCString();
                        if (tagV == "alliance_mark")
                        {
                            string msg = dict->valueForKey("msg")->getCString();
                            mChatNoticeV[i]->setObject(CCString::create(msg.c_str()), "msg");
                            st = false;
                        }
                    }
                }
            }
        }
        if (st) {
            mChatNoticeV.push_back(dict);
        }
    }else{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(!ChatServiceCocos2dx::isChatShowing)
            createChatNotice(dict);
        else
            mChatNoticeV.push_back(dict);
#else
         createChatNotice(dict);
#endif
    }
}
void LuaController::createChatNotice(CCDictionary *dict){
    if(mChatNoticeNode && mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG)){
        return;
    }
    float needTime = 0;
    bool bSys = false;
    if(dict->objectForKey("hintType")){
        bSys = true;
        std::string hintType = dict->valueForKey("hintType")->getCString();
        if(hintType.compare("SystemUpdate") == 0){
            double countDown = 0;
            bool isLogin = 0;
            string tip = "";
            string icon = "";
            FlyHintType type = FLY_HINT_LOGIN;
            if(dict->objectForKey("countDown") != nullptr) {
                countDown = dynamic_cast<CCDouble*>(dict->objectForKey("countDown"))->getValue();
            }
            if(dict->objectForKey("isLogin") != nullptr) {
                 isLogin = dynamic_cast<CCBool*>(dict->objectForKey("isLogin"))->getValue();
            }
            if(dict->objectForKey("tip") != nullptr) {
                 tip = dynamic_cast<CCString*>(dict->objectForKey("tip"))->getCString();
            }
            if (dict->objectForKey("type") != nullptr) {
                 type = (FlyHintType)dynamic_cast<CCInteger*>(dict->objectForKey("type"))->getValue();
            }
            if (dict->objectForKey("icon") != nullptr) {
                 icon = dynamic_cast<CCString*>(dict->objectForKey("icon"))->getCString();
            }
            if(type == FLY_HINT_LOGIN){
                double curTime = GlobalData::shared()->getWorldTime();
                double cTime = GlobalData::shared()->changeTime(countDown/1000);
                if( (long)floor( cTime - curTime ) <0)
                    return;
            }
            FlySystemUpdateHint *flySys = FlySystemUpdateHint::create(countDown, isLogin,tip,type,icon);
            if(flySys){
                needTime = flySys->getRollTime();
                flySys->setTag(CHAT_NOTICE_TAG);
                getChatNoticeContainer()->addChild(flySys);
                CCSize winSize = CCDirector::sharedDirector()->getWinSize();
                if (CCCommonUtils::isIosAndroidPad()) {
                    flySys->setPosition(ccp(winSize.width*0.5, winSize.height-420));
                } else {
                    flySys->setPosition(ccp(winSize.width*0.5, winSize.height-210));
                }
            }else{
                return;
            }
        }
    }else{
//        if (CCCommonUtils::isIosAndroidPad())
//        {
//            return;
//        }
        if(this->isUncompressing("lua") || this->isDownloading("lua"))
            return;
        if(initLua()==false)
            return;
        if(initMD5Info()==false || checkLuaFiles() == false){
            resetVersionAndUpdate();
            return;
        }
        if(m_clientVersion.compare(this->luaVersion) != 0){
            checkUpdate();
            return;
        }
        auto iter = m_md5Map.find("noticeFiles");
        vector<std::string> tmpFiles;
        if(iter!=m_md5Map.end()){
            CCCommonUtils::splitString((*iter).second, ",", tmpFiles);
        }else{
            resetVersionAndUpdate();
            return;
        }
        auto iter1 = tmpFiles.begin();
        while (iter1!=tmpFiles.end()) {
            std::string filePath =getLuaPath() + "/" +(*iter1);
            if(!CCFileUtils::sharedFileUtils()->isFileExist(filePath)){
                resetVersionAndUpdate();
                return;
            }
            ++iter1;
        }
        
        CCModelLayer* modelLayer = CCModelLayer::create();
        modelLayer->setContentSize(CCSize(CCDirector::sharedDirector()->getWinSize().width, 80));
        modelLayer->setTag(CHAT_NOTICE_TAG - 2);
        modelLayer->setTouchEnabled(true);
        modelLayer->setTouchPriority(0);
        getChatNoticeContainer()->addChild(modelLayer);
        LuaEngine* pEngine = LuaEngine::getInstance();
        string path = getLuaPath();
        string filepath = getLuaFile();
        pEngine->addSearchPath(path.c_str());
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        CCLOG(filepath.c_str());
        CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
        if (pstrFileContent){
            pEngine->executeString(pstrFileContent->getCString());
        }
    #else
        filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
        pEngine->executeScriptFile(filepath.c_str());
    #endif
        
        lua_State* ls = pEngine->getLuaStack()->getLuaState();
        lua_getglobal(ls, "addChatNotice");
        lua_newtable(ls);
        CCDictElement *ele = NULL;
        CCDICT_FOREACH(dict, ele){
            if (ele && ele->getObject()) {
                CCString *value = dynamic_cast<CCString*>(ele->getObject());
                std::string key = ele->getStrKey();
                if(!key.empty() && value && value->getCString()){
                    string valueStr = value->getCString();
                    if(key == "msg"){
                        auto iter = valueStr.find("\n");
                        while (iter!=string::npos) {
                            valueStr = valueStr.replace(iter, 1, "  ");
                            iter = valueStr.find("\n");
                        }
                    }
                    lua_pushstring(ls, key.c_str());//压入key
                    lua_pushstring(ls,valueStr.c_str());//压入value
                    lua_settable(ls,-3);//弹出key,value，并设置到table里面去
                    CCLOG("notice --- key ---%s,value --- %s",key.c_str(),value->getCString());
                }
            }
        }
        if (CCCommonUtils::isIosAndroidPad())
        {
            lua_pushstring(ls, "isPadHD");
            lua_pushstring(ls, "true");
            lua_settable(ls, -3);
        }
        lua_pushstring(ls, path.c_str());
        int showT = ChatController::getInstance()->getNoticeShowTime();
        lua_pushnumber(ls, showT);
        if (!lua_isfunction(ls, -4))
        {
            lua_pop(ls, 4);
            return;
        }
        lua_call(ls, 3, 1);
        needTime  = lua_tonumber(ls, -1);
    }
    dict->release();
    if(bSys==false){
        if(PopupViewController::getInstance()->getCurrViewCount()>0){
            showSceneNotice(false);
        }else{
            showSceneNotice(true);
        }
    }
    if(mChatNoticeNode){
        CCDelayTime *delay = CCDelayTime::create(needTime);
        CCLOG("chat_notice_play_time [%f]",needTime);
        mChatNoticeNode->runAction(CCSequence::create(delay,CCCallFuncO::create(this, callfuncO_selector(LuaController::changeChatNotice), NULL),NULL));
    }
}
void LuaController::closeChatNotice(){
    if(mChatNoticeNode){
        mChatNoticeNode->removeAllChildren();
        if(mChatNoticeV.size() == 0){
            mChatNoticeNode->stopAllActions();
//            mChatNoticeNode->removeFromParent();
//            mChatNoticeNode=NULL;
        }
    }
}
void LuaController::changeChatNotice(CCObject *ccObj){
    if(mChatNoticeNode){
        mChatNoticeNode->removeAllChildren();
        mChatNoticeNode->stopAllActions();
//        mChatNoticeNode->removeFromParent();
//        mChatNoticeNode=NULL;
    }
    if(mChatNoticeV.size()>0){
        auto iter = mChatNoticeV.begin();
        createChatNotice((*iter));
        mChatNoticeV.erase(iter);
    }
}
CCNode* LuaController::getChatNoticeContainer(){
    
    if(!mChatNoticeNode){
        mChatNoticeNode = CCNode::create();
        auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_GUIDE);
        layer->addChild(mChatNoticeNode,CHAT_NOTICE_TAG);
//        UIComponent::getInstance()->addChild(mChatNoticeNode);
    }
    return mChatNoticeNode;
}
void LuaController::showSceneNotice(bool b){
    if(mChatNoticeNode && mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG-1) && mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG) && mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG-2)){
        CCModelLayer *model=dynamic_cast<CCModelLayer*>(mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG-2));
        if(b==true){
            mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG)->setVisible(true);
            mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG-1)->setVisible(false);
            model->setPosition(mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG)->getPosition());
        }else{
            mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG)->setVisible(false);
            mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG-1)->setVisible(true);
            model->setPosition(mChatNoticeNode->getChildByTag(CHAT_NOTICE_TAG-1)->getPosition());
        }
    }
}
void LuaController::ChatNoticeInView(CCObject *ccObj){
    showSceneNotice(false);
}
void LuaController::ChatNoticeInSceen(CCObject *ccObj){
    if(PopupViewController::getInstance()->getCurrViewCount()==0 && PopupViewController::getInstance()->getGoBackViewCount() == 0){
        showSceneNotice(true);
    }
}
void LuaController::clearChatNotice(){
    if(mChatNoticeNode){
        mChatNoticeNode->stopAllActions();
        mChatNoticeNode->removeFromParent();
        mChatNoticeNode=NULL;
    }
    if(mChatNoticeV.size()>0){
        auto iter = mChatNoticeV.begin();
        while (iter!=mChatNoticeV.end()) {
            (*iter)->release();
            ++iter;
        }
        mChatNoticeV.clear();
    }
}
//
//礼包赠送
//
void LuaController::updateExchangeGiftInfo(std::string toUID,std::string itemid, double sendTime){
    auto iter = mExGiftList.begin();
    while (iter!=mExGiftList.end()) {
        if(toUID.compare((*iter).receiverId) == 0 && itemid.compare((*iter).exchangeId) == 0){
            (*iter).sendTime = sendTime;
            return;
        }
        ++iter;
    }
    ExchangeGiftStruct exStr;
    exStr.sendTime = sendTime;
    exStr.uid = GlobalData::shared()->playerInfo.uid;
    exStr.exchangeId=itemid;
    exStr.receiverId = toUID;
    mExGiftList.push_back(exStr);
}
void LuaController::initExchangeGiftInfo(CCArray *arr){
    CCObject *ccObj = NULL;
    CCARRAY_FOREACH(arr, ccObj){
        CCDictionary *ccDict = _dict(ccObj);
        if(!ccDict)
            continue;
        ExchangeGiftStruct egStr;
        if(ccDict->objectForKey("uid")){
            egStr.uid = ccDict->valueForKey("uid")->getCString();
        }
        if(ccDict->objectForKey("receiverId")){
            egStr.receiverId = ccDict->valueForKey("receiverId")->getCString();
        }
        if(ccDict->objectForKey("exchangeId")){
            egStr.exchangeId = ccDict->valueForKey("exchangeId")->getCString();
        }
        if(ccDict->objectForKey("sendTime")){
            egStr.sendTime = ccDict->valueForKey("sendTime")->doubleValue()/1000;
        }
        mExGiftList.push_back(egStr);
    }
}
bool LuaController::canSendExchange(std::string &itemid,std::string uid,std::string toName,int type,bool showTip){
    if(itemid.empty() || toName.empty() || uid.empty())
        return false;
    if(type==0){
        auto iter= GlobalData::shared()->goldExchangeList.find(itemid);
        if(iter==GlobalData::shared()->goldExchangeList.end()){
            return false;
        }
        type = atoi((*iter).second->type.c_str());
    }
    
    auto iter = mExGiftList.begin();
    while (iter!=mExGiftList.end()) {
        if((*iter).exchangeId.compare(itemid) == 0 && (*iter).receiverId.compare(uid) == 0){
            switch (type) {
                case 2:{
                    double lastTime = GlobalData::shared()->changeTime((*iter).sendTime) - GlobalData::shared()->getWorldTime();
                    if(lastTime>0){
                        if(showTip){
                            YesNoDialog::show(_lang_1("101009", toName.c_str()), NULL);
                        }
                        return false;
                    }else{
                        return true;
                    }
                    break;
                }
                case 3:{
                    if(showTip){
                        YesNoDialog::show(_lang_1("101009", toName.c_str()), NULL);
                    }
                    return false;
                    break;
                }
                default:
                    break;
            }
        }
        ++iter;
    }
    return true;
}
void LuaController::backToExchange(int backTo){
    switch (backTo) {
        case 1:{//弹出礼包
            PopupViewController::getInstance()->addPopupView(RechargeACTVCell::create());
            break;
        }
        case 2:{
            UIComponent::getInstance()->showGoldBuyPanel();
            break;
        }
        default:
            break;
    }
}
void LuaController::parseConfig(CCDictionary *dict){
    if(isLuaOpen()==false)
        return;
    if(initLua()==false)
        return;
    if (m_clientVersion.compare(this->luaVersion) != 0)
        return;
    if (!dict) {
        return;
    }
    LuaEngine* pEngine = LuaEngine::getInstance();
    string path = getLuaPath();
    string filepath = path+"/actMain.lua";
    if (!CCFileUtils::sharedFileUtils()->isFileExist(filepath)) {
        return;
    }
    pEngine->addSearchPath(path.c_str());
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
    
    lua_State* ls = pEngine->getLuaStack()->getLuaState();
    lua_getglobal(ls, "initConfig");
    tolua_pushusertype(ls,(void*)dict,"CCDictionary");
    lua_pushstring(ls, path.c_str());
    if (!lua_isfunction(ls, -3))
    {
        lua_pop(ls, 3);
        return;
    }
    lua_call(ls, 2, 0);
}
void LuaController::sendCMD(std::string cmdName,CCDictionary *dict){
    LuaCommand *luacmd = new LuaCommand(cmdName,dict);
    luacmd->sendAndRelease();
}
void LuaController::backCMD(std::string cmdName,CCDictionary *dict){
    if(isLuaOpen()==false)
        return;
    if(initLua()==false)
        return;
    if (m_clientVersion.compare(this->luaVersion) != 0)
        return;
    if (!dict) {
        return;
    }
    LuaEngine* pEngine = LuaEngine::getInstance();
    string path = getLuaPath();
    string filepath = path+"/actMain.lua";
    if (!CCFileUtils::sharedFileUtils()->isFileExist(filepath)) {
        return;
    }
    pEngine->addSearchPath(path.c_str());
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
    
    lua_State* ls = pEngine->getLuaStack()->getLuaState();
    lua_getglobal(ls, "backCMD");
//    string json = dict->valueForKey("json")->getCString();
//    lua_pushlstring(ls, json.c_str(), json.length());
    tolua_pushusertype(ls,(void*)dict,"CCDictionary");
    lua_pushstring(ls, cmdName.c_str());
    lua_pushstring(ls, path.c_str());
    if (!lua_isfunction(ls, -4)) {
        lua_pop(ls, 4);
        return;
    }
    lua_call(ls, 3, 0);
}
void LuaController::notificationToLua(std::string cmdName,Ref* ret){
    if(isLuaOpen()==false)
        return;
    if(initLua()==false)
        return;
    if (m_clientVersion.compare(this->luaVersion) != 0)
        return;
    if(find(m_notificationList.begin(),m_notificationList.end(),cmdName) == m_notificationList.end()){
        return;
    }
    PopupBaseView * pView = PopupViewController::getInstance()->getCurrentPopupView();
    if(!pView || pView->getIsLua()==false){
        return;
    }
    LuaEngine* pEngine = LuaEngine::getInstance();
    string path = getLuaPath();
    string filepath = path+"/actMain.lua";
    if (!CCFileUtils::sharedFileUtils()->isFileExist(filepath)) {
        return;
    }
    pEngine->addSearchPath(path.c_str());
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
    if (ret == NULL){
        lua_State* ls = pEngine->getLuaStack()->getLuaState();
        lua_getglobal(ls, "backNotification");
        lua_pushstring(ls, cmdName.c_str());
        lua_pushstring(ls, path.c_str());
        if (!lua_isfunction(ls, -3)) {
            lua_pop(ls, 3);
            return;
        }
        lua_call(ls,2, 0);
    }else{
        lua_State* ls = pEngine->getLuaStack()->getLuaState();
        lua_getglobal(ls, "backNotificationWithParams");
        lua_pushstring(ls, cmdName.c_str());
        lua_pushstring(ls, path.c_str());
        if(dynamic_cast<CCString*>(ret)){
            lua_pushstring(ls, (dynamic_cast<CCString*>(ret))->getCString());
        }else if(dynamic_cast<CCInteger*>(ret)){
            lua_pushinteger(ls, dynamic_cast<CCInteger*>(ret)->getValue());
        }else if(dynamic_cast<CCDouble*>(ret)){
            lua_pushnumber(ls, dynamic_cast<CCDouble*>(ret)->getValue());
        }else if(dynamic_cast<CCFloat*>(ret)){
            lua_pushnumber(ls, dynamic_cast<CCFloat*>(ret)->getValue());
        }else if(dynamic_cast<CCBoolean*>(ret)){
            lua_pushboolean(ls, dynamic_cast<CCBoolean*>(ret)->getValue());
        }else if(dynamic_cast<CCDictionary*>(ret)){
            tolua_pushusertype(ls,(void*)ret,"CCDictionary");
        }else if(dynamic_cast<CCArray*>(ret)){
            tolua_pushusertype(ls,(void*)ret,"CCArray");
        }else{
            tolua_pushusertype(ls,(void*)ret,"cc.Ref");
        }
        if (!lua_isfunction(ls, -4)) {
            lua_pop(ls, 4);
            return;
        }
        lua_call(ls, 3, 0);
    }
}
void LuaController::showFAQ(string faq){
    CCCommonUtils::showHelpShiftSingleFAQ(faq);
}
//google translate
void LuaController::setTranslateMark(){
    CCLog("setTranslateMark 1");
    CCLOG("setTranslateMark 1");
    if(canTranslate()){
        CCLOG("setTranslateMark 2");
        CCLog("setTranslateMark 2");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ChatServiceCocos2dx::postTranslateByLuaStart();
#else
        m_canTranslate=true;
#endif
    }else{
        CCLOG("setTranslateMark 3");
        CCLog("setTranslateMark 3");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        //todo set java value
#else
        m_canTranslate=false;
#endif
    }
}
bool LuaController::canTranslate(){
    if(initLua()==false)
        return false;
    if (m_clientVersion.compare(luaVersion) != 0) {
        return false;
    }
    if (isDownloading("lua") == true || isUncompressing("lua") == true ) {
        return false;
    }
    string checkV = "1.0.53";
    std::vector<string> vector1;
    std::vector<string> vector2;
    CCCommonUtils::splitString(checkV, ".", vector1); //要求的版本
    CCCommonUtils::splitString(m_clientVersion, ".", vector2); //当前的版本
    if(vector1.size() == vector2.size()){
        int index = 0;
        while (index<vector1.size()) {
            int tmpV1 = atoi(vector1.at(index).c_str());
            int tmpV2 = atoi(vector2.at(index).c_str());
            if(tmpV1<tmpV2){
                return true;
            }else if(tmpV1>tmpV2){
                return false;
            }
            ++index;
        }
        return true;
    }
    return false;
}
std::string LuaController::encodeUrl(const std::string& szToEncode){
    std::string src = szToEncode;
    char hex[] = "0123456789ABCDEF";
    string dst;
    for (size_t i = 0; i < src.size(); ++i){
        unsigned char cc = src[i];
        if (isascii(cc)){
            if (cc == ' '){
                dst += "%20";
            }else
                dst += cc;
        }else{
            unsigned char c = static_cast<unsigned char>(src[i]);
            dst += '%';
            dst += hex[c / 16];
            dst += hex[c % 16];
        }
    }
    return dst;
}

void LuaController::translate(std::string orgContent,std::string targeLan,int linkidx,TRANSLATETYPE _type){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#else
    if(m_canTranslate==false)
        return;
#endif
    
    addLuaPath();
    LuaEngine* pEngine = LuaEngine::defaultEngine();
    string path = getLuaPath();
    string filepath = getLuaFile();
    pEngine->addSearchPath(path.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    m_orgMsg = orgContent;
    
    lua_State* ls = pEngine->getLuaStack()->getLuaState();
    lua_getglobal(ls, "getTranslateLink");
    lua_pushstring(ls, path.c_str());
    lua_pushlstring(ls, orgContent.c_str(), orgContent.length());
    lua_pushlstring(ls, targeLan.c_str(), targeLan.length());
    lua_pushinteger(ls, linkidx);
    if (!lua_isfunction(ls, -5)) {
        lua_pop(ls, 5);
        return;
    }
    lua_call(ls, 4, 4);
    std::string httpType  = lua_tostring(ls, -1);
    lua_pop(ls,1);
    std::string urlParams  = lua_tostring(ls, -1);
    lua_pop(ls,1);
    std::string url  = lua_tostring(ls, -1);
    lua_pop(ls,1);
    std::string agent  = lua_tostring(ls, -1);
    lua_pop(ls,1);
    
    if(urlParams.empty() && url.empty())
        return;
    
    CURL *curl;
    CURLcode res;
    string cc;
    curl=curl_easy_init();
    if(curl)
    {
        if(httpType.compare("get")==0){
            url = url + "?"+urlParams;
            url = encodeUrl(url);
//            CCLog("translate url: [%s]", url.c_str());
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); //设置请求的地址
        }else{
            url = encodeUrl(url);
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); //设置请求的地址
            curl_easy_setopt(curl, CURLOPT_POST, true); //设置数据类型
            urlParams = encodeUrl(urlParams);
//            CCLog("translate url: [%s],[%s]", url.c_str(),urlParams.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS,urlParams.c_str()); //将操作代码，和连接的网站组合，一起发送！
        }
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        
        if (_type == TRANSLATE_SUPERMAN) {
            curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,LuaController::onTranslate_superMan); //数据处理回调函数
        }else if(_type == TRANSLATE_MAIL){
            curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,LuaController::onTranslate); //数据处理回调函数
        }
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &cc);//缓冲的内存
        curl_easy_setopt(curl,CURLOPT_TIMEOUT_MS,2000);    //设置连接超时时间
        
        curl_easy_setopt(curl,CURLOPT_USERAGENT,agent.c_str());
        
        res=curl_easy_perform(curl);
        if(res!=CURLE_OK){
//            if(res!=CURLE_WRITE_ERROR){
            if(res == CURLE_OPERATION_TIMEDOUT){
                CCLog("curl is error");
                linkidx++;
                LuaController::getInstance()->translate(orgContent, targeLan,linkidx,_type);
            }
        }
        curl_easy_cleanup(curl);
    }else{
        CCLog("curl is null");
    }
}
size_t LuaController::onTranslate(uint8_t* ptr,size_t size,size_t number,void *stream){
    CCString* a=CCString::createWithFormat("%s",ptr);
    std::string serverInfo=a->getCString();
    LuaController::getInstance()->onTranslateBack(serverInfo);
    return  size*number;
}
void LuaController::onTranslateBack(std::string backStr){
    if(initLua()==false)
        return;
//    CCLog("translate back string: [%s]", backStr.c_str());
    LuaController::getInstance()->addLuaPath();
    LuaEngine* pEngine = LuaEngine::defaultEngine();
    string path = LuaController::getInstance()->getLuaPath();
    string filepath = LuaController::getInstance()->getLuaFile();
    pEngine->addSearchPath(path.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    lua_State* ls = pEngine->getLuaStack()->getLuaState();
    lua_getglobal(ls, "praseTranslateMsg");
    lua_pushstring(ls, path.c_str());
    lua_pushlstring(ls, backStr.c_str(), backStr.length());
    if (!lua_isfunction(ls, -3))
    {
        lua_pop(ls, 3);
        return;
    }
    lua_call(ls, 2, 3);
    
    string json = "{\"originalLang\":\"";
    
    std::string orgContent  = lua_tostring(ls, -1);
    CCLOGFUNCF("orgContent %s",orgContent.c_str());
    lua_pop(ls,1);
    std::string orgLan  = lua_tostring(ls, -1);
    CCLOGFUNCF("orgLan %s",orgLan.c_str());
    lua_pop(ls,1);
    std::string targeContent  = lua_tostring(ls, -1);
    CCLOGFUNCF("targeContent %s",targeContent.c_str());
    lua_pop(ls,1);
    if(m_orgMsg=="" || orgContent=="" || targeContent=="")
        return;
    json.append(orgLan).append("\",\"originalMsg\":\"").append(m_orgMsg).append("\",\"translatedMsg\":\"").append(targeContent).append("\"}");
    CCLOGFUNCF("lua translateTet %s",json.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ChatServiceCocos2dx::postTranslatedResult(json);
#endif
    
    m_orgMsg="";
//    CCLog("translate data [%s],[%s],[%s]",orgContent.c_str(),orgLan.c_str(),targeContent.c_str());
    
}

bool LuaController::isIosAndroidPad()
{
    return CCCommonUtils::isIosAndroidPad();
}






size_t LuaController::onTranslate_superMan(uint8_t* ptr,size_t size,size_t number,void *stream){
    CCString* a=CCString::createWithFormat("%s",ptr);
    std::string serverInfo=a->getCString();
    LuaController::getInstance()->onTranslateBack_superMan(serverInfo);
    return  size*number;
}
void LuaController::onTranslateBack_superMan(std::string backStr){
    if(initLua()==false)
        return;
    //    CCLog("translate back string: [%s]", backStr.c_str());
    LuaController::getInstance()->addLuaPath();
    LuaEngine* pEngine = LuaEngine::defaultEngine();
    string path = LuaController::getInstance()->getLuaPath();
    string filepath = LuaController::getInstance()->getLuaFile();
    pEngine->addSearchPath(path.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLOG(filepath.c_str());
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filepath.c_str());
    if (pstrFileContent){
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
#endif
    lua_State* ls = pEngine->getLuaStack()->getLuaState();
    lua_getglobal(ls, "praseTranslateMsg");
    lua_pushstring(ls, path.c_str());
    lua_pushlstring(ls, backStr.c_str(), backStr.length());
    if (!lua_isfunction(ls, -3)) {
        lua_pop(ls, 3);
        return;
    }
    lua_call(ls, 2, 3);
    
    string json = "{\"originalLang\":\"";
    
    std::string orgContent  = lua_tostring(ls, -1);
    CCLOGFUNCF("orgContent %s",orgContent.c_str());
    lua_pop(ls,1);
    std::string orgLan  = lua_tostring(ls, -1);
    CCLOGFUNCF("orgLan %s",orgLan.c_str());
    lua_pop(ls,1);
    std::string targeContent  = lua_tostring(ls, -1);
    CCLOGFUNCF("targeContent %s",targeContent.c_str());
    lua_pop(ls,1);
    if(m_orgMsg=="" || orgContent=="" || targeContent=="")
        return;
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(msg_superman_des,CCStringMake(targeContent));
    
    json.append(orgLan).append("\",\"originalMsg\":\"").append(m_orgMsg).append("\",\"translatedMsg\":\"").append(targeContent).append("\"}");
    CCLOGFUNCF("lua translateTet %s",json.c_str());
 
    
    m_orgMsg="";
    //    CCLog("translate data [%s],[%s],[%s]",orgContent.c_str(),orgLan.c_str(),targeContent.c_str());
    
}

void LuaController::showBindGuide()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CLOSING_RechargeACTVCell);
}

#pragma mark 载入资源
void LuaController::loadResourceByNameAndPath(std::string name,std::string path, bool release/*=false*/){
    std::string fileName = path + name + ".plist";
    CCLog("android_test_loadNameTypeResourceName[%s]",fileName.c_str());
    
    if(!CCFileUtils::sharedFileUtils()->isFileExist(fileName)){
        CCLog("android_test_loadNameTypeResourceName[%s]_file_not_exite",fileName.c_str());
        return;
    }
    if(release==false){
        cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(fileName.c_str());
    }else{
        cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(fileName.c_str());
        std::string textName = path + name + ".pvr.ccz";
        CCTexture2D *texture =  CCTextureCache::sharedTextureCache()->textureForKey(textName.c_str());
        if(texture){
            CCTextureCache::sharedTextureCache()->removeTexture(texture);
        }
        
    }
}

void LuaController::sendNotification(string msg){
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(msg.c_str());
}
LuaActivityInfo* LuaController::getActivityInfo(string actID){
    auto iter = mLuaActMap.find(actID);
    if(iter!=mLuaActMap.end()){
        return (*iter).second;
    }
    return NULL;
}
//void LuaController::clickActButton(int type){
//    showLuaViewByActivity("57034");
//    return;
//    if(type<0 || type>=LUAACTOP_MAX){
//        return;
//    }
//    vector<string> expList;
//    auto iter = mLuaActMap.begin();
//    while (iter!=mLuaActMap.end()) {
//        auto ainfo = (*iter).second;
//        if(ainfo!=NULL){
//            if(ainfo->isExpire()==true){
//                expList.push_back(ainfo->getactID());
//                ++iter;
//                continue;
//            }
//            if(ainfo->isExpire()==false && (int)ainfo->getopenPoint() == type){
//                if(checkValidActivity(ainfo->getresName(),ainfo->getmd5())){
//                     showLuaViewByActivity(ainfo->getactID());
//                }
//                break;
//            }
//        }
//        ++iter;
//    }
//    if(expList.size()>0){
//        auto itr = expList.begin();
//        while (itr!=expList.end()) {
//            mLuaActMap[(*itr)]->release();
//            delete mLuaActMap[(*itr)];
//            ++itr;
//        }
//    }
//}
void LuaController::showLuaViewByActivity(string actID){
    if(isLuaOpen()==false)
        return;
    if(initLua()==false)
        return;
    if (m_clientVersion.compare(this->luaVersion) != 0)
        return;
    LuaEngine* pEngine = LuaEngine::getInstance();
    string path = getLuaPath();
    string filepath = path+"/actMain.lua";
    if (!CCFileUtils::sharedFileUtils()->isFileExist(filepath)) {
        return;
    }
    pEngine->addSearchPath(path.c_str());
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
    lua_State* ls = pEngine->getLuaStack()->getLuaState();
    lua_getglobal(ls, "showActPopup");
    lua_pushstring(ls, path.c_str());
    lua_pushstring(ls, actID.c_str());
    if (!lua_isfunction(ls, -3))
    {
        lua_pop(ls, 3);
        return;
    }
    lua_call(ls, 2, 0);
}
void LuaController::initActivityInfo(CCArray *array){
    auto iter = mLuaActMap.begin();
    while (iter!=mLuaActMap.end()) {
        (*iter).second->release();
        ++iter;
    }
    mLuaActMap.clear();
    if(array){
        CCObject* ccObj = NULL;
        CCARRAY_FOREACH(array, ccObj){
            CCDictionary* _dict = dynamic_cast<CCDictionary*>(ccObj);
            if(!_dict)
                continue;
            if(_dict->objectForKey("id") && _dict->objectForKey("resName") && _dict->objectForKey("iosmd5") && _dict->objectForKey("androidmd5")){
                LuaActivityInfo *ainfo = new LuaActivityInfo();
                ainfo->setInfo(_dict);
                if(!ainfo->getmd5().empty() && ainfo->isExpire()==false){
                    mLuaActMap[ainfo->getactID()]=ainfo;
                    checkValidActivity(ainfo->getresName(),ainfo->getmd5());
                }else{
                    ainfo->release();
                }
            }
        }
    }
}
bool LuaController::checkValidActivity(string resName,string newmd5){
    if(isDownloading(LUA_COCOS_LIB)==true || isUncompressing(LUA_COCOS_LIB) == true){
        return false;
    }
    if(isDownloading(resName) || isUncompressing(resName))
        return false;
    
    if(isLuaOpen()==false)
        return false;
    if(initLua()==false)
        return false;
    if(initMD5Info()==false){
        resetVersionAndUpdate();
        return false;
    }
    if (m_clientVersion.compare(luaVersion) != 0) {
        resetVersionAndUpdate();
        return false;
    }
    
    if(!checkLib(LUA_COCOS_LIB,LUA_COCOS_LIB_MD5)){
        updateNewResource(LUA_COCOS_LIB,LUA_COCOS_LIB_MD5);
//        return false;
    }
    if(!checkLib(resName,newmd5)){
        updateNewResource(resName,newmd5,true);
        return false;
    }
    return true;
}
bool LuaController::checkLib(string libName,string newMD5){
    auto md5iter = m_md5Map.find(libName);
    if(md5iter ==m_md5Map.end())
        return false;
    if((*md5iter).second.compare(newMD5) != 0)
        return false;
    
    string libPath = getLuaPath() + "/" ;
    string libConfig = libPath + libName + "/" + libName +".cfg";
    if(!CCFileUtils::sharedFileUtils()->isFileExist(libConfig)){
        return false;
    }
    auto praseLine = [&](string line,vector<std::string> &mTmpV1){
        if(!line.empty() && line.find("files=")!=string::npos){
            vector<std::string> mTmpV;
            CCCommonUtils::splitString(line, "=", mTmpV);
            if(mTmpV.size()>1){
                CCCommonUtils::splitString(mTmpV.at(1), ",", mTmpV1);
            }
            return 1;
        }else if(!line.empty() && line.find("notice=")!=string::npos){
            vector<std::string> mTmpV;
            CCCommonUtils::splitString(line, "=", mTmpV);
            if(mTmpV.size()>1){
                CCCommonUtils::splitString(mTmpV.at(1), ";", mTmpV1);
            }
            return 2;
        }
        return 0;
    };
    
    auto iter = m_libMap.find(libName);
    bool exist=true;
    if(iter==m_libMap.end()){
        exist=false;
        xxtea_long len = 0;
//        unsigned char* buff = nullptr;
//        ssize_t buffSize = 0;
//        buff = CCFileUtils::sharedFileUtils()->getFileData(libConfig, "rb", &buffSize);
//        // decrypt XXTEA
//        xxtea_long len = 0;
//        auto signLen = strlen(ENCRYPT_SIGN);
//        unsigned char* result = xxtea_decrypt((unsigned char*)buff + signLen,
//                                              (xxtea_long)buffSize - signLen,
//                                              (unsigned char*)ENCRYPT_KEY,
//                                              (xxtea_long)strlen(ENCRYPT_KEY),
//                                              &len);
//        
        unsigned char* result = this->getXXTeaDecode(libConfig,len);
        std::string content((char*)result, len);
        auto iter=content.find("\n");
        string ll;
        vector<std::string> mTmpV;
        while(iter!=string::npos){
            ll = content.substr(0,iter);
            mTmpV.clear();
            int type = praseLine(ll,mTmpV);
            if(mTmpV.size()>0){
                switch (type) {
                    case 1:
                        m_libMap[libName]=mTmpV;
                        break;
                    case 2:{
                        auto iterttt = mTmpV.begin();
                        while (iterttt!=mTmpV.end()) {
                            if(find(m_notificationList.begin(), m_notificationList.end(), (*iterttt) ) == m_notificationList.end()){
                                m_notificationList.push_back((*iterttt));
                            }
                            ++iterttt;
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
            content = content.substr(iter+1);
            iter=content.find("\n");
        }
        ll = content.substr(0,iter);
        mTmpV.clear();
        int type = praseLine(ll,mTmpV);
        if(mTmpV.size()>0){
            switch (type) {
                case 1:
                    m_libMap[libName]=mTmpV;
                    break;
                case 2:{
                    auto iterttt = mTmpV.begin();
                    while (iterttt!=mTmpV.end()) {
                        if(find(m_notificationList.begin(), m_notificationList.end(), (*iterttt) ) == m_notificationList.end()){
                            m_notificationList.push_back((*iterttt));
                        }
                        ++iterttt;
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
    if(exist==false){
        iter = m_libMap.find(libName);
    }
    if(iter!=m_libMap.end()){
        auto iter1 = (*iter).second.begin();
        if(!CCFileUtils::sharedFileUtils()->isFileExist(libPath+(*iter1))){
            return false;
        }
        return true;
    }
    return false;
}
unsigned char* LuaController::getXXTeaDecode(string filepath,uint32_t &len){
    unsigned char* buff = nullptr;
    ssize_t buffSize = 0;
    buff = CCFileUtils::sharedFileUtils()->getFileData(filepath, "rb", &buffSize);
    // decrypt XXTEA
    len = 0;
    auto signLen = strlen(ENCRYPT_SIGN);
    unsigned char* result = xxtea_decrypt((unsigned char*)buff + signLen,
                                          (xxtea_long)buffSize - signLen,
                                          (unsigned char*)ENCRYPT_KEY,
                                          (xxtea_long)strlen(ENCRYPT_KEY),
                                          &len);
    return result;
}
void LuaController::setWorldConfig(CCArray *luaArr){
    m_worldconfig.clear();
    CCObject *ccObj = NULL;
    CCARRAY_FOREACH(luaArr, ccObj){
        CCDictionary *dict = _dict(ccObj);
        string resName = "";
        if(dict->objectForKey("lr")){
            resName = dict->valueForKey("lr")->getCString();
        }
        string resMd5 = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(dict->objectForKey("lma")) {
            resMd5 = dict->valueForKey("lma")->getCString();
        }
#else
        if(dict->objectForKey("lm")) {
            resMd5 = dict->valueForKey("lm")->getCString();
        }
#endif
        if(!resName.empty() && !resMd5.empty()){
            m_worldconfig[resName] = resMd5;
        }
    }
}
void LuaController::addWorldUnderNode(CCPoint pos, unsigned int index,WorldCityInfo *cinfo,CCArray * array){
    if(isLuaOpen()==false)
        return;
    if(initLua()==false)
        return;
    auto iter = m_worldconfig.find(cinfo->luaResName);
    if(iter==m_worldconfig.end())
        return;
    if(checkValidActivity(cinfo->luaResName, (*iter).second)==false){
        return;
    }
    
    
    LuaEngine* pEngine = LuaEngine::getInstance();
    string path = getLuaPath();
    string filepath = path+"/worldMain.lua";
    if (!CCFileUtils::sharedFileUtils()->isFileExist(filepath)) {
        return;
    }
    pEngine->addSearchPath(path.c_str());
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
    lua_State* ls = pEngine->getLuaStack()->getLuaState();
    lua_getglobal(ls, "addWorldUnderNode");
    lua_pushnumber(ls, pos.x);
    lua_pushnumber(ls, pos.y);
    lua_pushnumber(ls, index);
    tolua_pushusertype(ls,(void*)cinfo,"WorldCityInfo");
    tolua_pushusertype(ls,(void*)array,"CCArray");
    lua_pushstring(ls, path.c_str());
    if (!lua_isfunction(ls, -7)) {
        lua_pop(ls, 7);
        return;
    }
    lua_call(ls, 6, 0);
}

//void LuaController::addWorldBatchItem(CCNode *pNode,CCPoint pos, int type, unsigned int index, WorldCityInfo *cinfo,CCArray * array,CCObject *ccObj){
//    if(isLuaOpen()==false)
//        return;
//    if(initLua()==false)
//        return;
//    LuaEngine* pEngine = LuaEngine::getInstance();
//    string path = getLuaPath();
//    string filepath = path+"/worldMain.lua";
//    if (!CCFileUtils::sharedFileUtils()->isFileExist(filepath)) {
//        return;
//    }
//    pEngine->addSearchPath(path.c_str());
//    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
//    pEngine->executeScriptFile(filepath.c_str());
//    lua_State* ls = pEngine->getLuaStack()->getLuaState();
//    int paramCnt = 8;
//    if (ccObj!=NULL) {
//        lua_getglobal(ls, "addBatchItemWithObj");
//        tolua_pushusertype(ls,(void*)ccObj,"cc.Ref");
//        paramCnt=9;
//    }else{
//        lua_getglobal(ls, "addBatchItem");
//        paramCnt=8;
//    }
//    tolua_pushusertype(ls,(void*)pNode,"cc.Node");
//    lua_pushnumber(ls, pos.x);
//    lua_pushnumber(ls, pos.y);
//    lua_pushnumber(ls, type);
//    lua_pushnumber(ls, index);
//    tolua_pushusertype(ls,(void*)cinfo,"WorldCityInfo");
//    tolua_pushusertype(ls,(void*)array,"CCArray");
//    lua_pushstring(ls, path.c_str());
//    lua_call(ls, paramCnt, 0);
//}
NewBaseTileLuaInfo* LuaController::createTileView(WorldCityInfo *cinfo){
    if(isLuaOpen()==false)
        return NULL;
    if(initLua()==false)
        return NULL;
    auto iter = m_worldconfig.find(cinfo->luaResName);
    if(iter==m_worldconfig.end())
        return NULL;
    
    if(checkValidActivity(cinfo->luaResName, (*iter).second)==false){
        return NULL;
    }
    
    LuaEngine* pEngine = LuaEngine::getInstance();
    string path = getLuaPath();
    string filepath = path+"/worldMain.lua";
    if (!CCFileUtils::sharedFileUtils()->isFileExist(filepath)) {
        return NULL;
    }
    pEngine->addSearchPath(path.c_str());
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
    lua_State* ls = pEngine->getLuaStack()->getLuaState();
    lua_getglobal(ls, "createTileView");
    tolua_pushusertype(ls,(void*)cinfo,"WorldCityInfo");
    lua_pushstring(ls, path.c_str());
    if (!lua_isfunction(ls, -3))
    {
        lua_pop(ls, 3);
        return nullptr;
    }
    lua_call(ls, 2, 1);
    NewBaseTileLuaInfo* ret =  *(NewBaseTileLuaInfo**)lua_touserdata(ls, -1);
    lua_pop(ls,1);
    if(ret && ret->getCityInfo()!=NULL)
        return ret;
    return NULL;
}
void LuaController::releaseWorldRes(){
    if(isLuaOpen()==false)
        return;
    if(initLua()==false)
        return;
    if (m_clientVersion.compare(luaVersion) != 0) {
        resetVersionAndUpdate();
        return;
    }
    
    LuaEngine* pEngine = LuaEngine::getInstance();
    string path = getLuaPath();
    string filepath = path+"/worldMain.lua";
    if (!CCFileUtils::sharedFileUtils()->isFileExist(filepath)) {
        return;
    }
    pEngine->addSearchPath(path.c_str());
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
    lua_State* ls = pEngine->getLuaStack()->getLuaState();
    lua_getglobal(ls, "releaseWorldResource");
    if (!lua_isfunction(ls, -1)) {
        lua_pop(ls, 1);
        return;
    }
    lua_call(ls, 0, 0);

}
void LuaController::onImperialEnter()
{
    if(isLuaOpen()==false)
        return;
    if(initLua()==false)
        return;
    if(initMD5Info() == false)
        return;
    if (m_clientVersion.compare(this->luaVersion) != 0)
        return;
    LuaEngine* pEngine = LuaEngine::getInstance();
    string path = getLuaPath();
    string filepath = path+"/actMain.lua";
    if (!CCFileUtils::sharedFileUtils()->isFileExist(filepath)) {
        return;
    }
    int time = getFileVersion("actMain");
    if (time < 20151225) {
        return;
    }
    pEngine->addSearchPath(path.c_str());
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
    lua_State* ls = pEngine->getLuaStack()->getLuaState();
    lua_getglobal(ls, "onImperialEnter");
    if (!lua_isfunction(ls, -1))
    {
        lua_pop(ls, 1);
        return;
    }
    lua_call(ls, 0, 0);
}

Node* LuaController::getImperialTouchLayer()
{
    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    if (layer) {
        return layer->getTouchLayer();
    } else {
        return nullptr;
    }
}

bool LuaController::isInTutorial()
{
    bool ret = false;
    if (GuideController::share()->isInTutorial() || (GlobalData::shared()->playerInfo.level == 1 && GlobalData::shared()->playerInfo.exp == 0))
    {
        ret = true;
    }
    return ret;
}

int LuaController::getFileVersion(string filename)
{
    auto it1 = m_md5Map.find("fileV");
    if (it1 == m_md5Map.end()) {
        return -1;
    }
    auto it2 = m_md5Map.find("files");
    if (it2 == m_md5Map.end()) {
        return -1;
    }
    string s1 = it1->second;
    vector<string> vec1;
    CCCommonUtils::splitString(s1, ";", vec1);
    string s2 = it2->second;
    vector<string> vec2;
    CCCommonUtils::splitString(s2, ",", vec2);
    
    int versionInLuaZip = -1;
    if (std::find(vec2.begin(), vec2.end(), filename) != vec2.end())
    {
        std::string versionFile = getLuaPath() + "/"+LUA_VERSION_FILE;
        versionInLuaZip = getVersionData(versionFile);
        if (versionInLuaZip == 0) {
            versionInLuaZip = -1;
        }
    }
    
    int versionInSmallZip = -1;
    for (auto it3 = vec1.begin(); it3 != vec1.end(); ++it3) {
        string s3 = *it3;
        vector<string> vec3;
        CCCommonUtils::splitString(s3, ":", vec3);
        if (vec3.size() > 1) {
            if (std::find(vec3.begin(), vec3.end(), filename) != vec3.end()) {
                versionInSmallZip = atoi(vec3.back().c_str());
                break;
            }
        }
    }
    
    int ret = versionInLuaZip > versionInSmallZip? versionInLuaZip : versionInSmallZip;
    return ret;
}

Dictionary* LuaController::getDicLuaData(Ref* src)
{
    Dictionary* dic = dynamic_cast<Dictionary*>(src);
    return dic;
}

Array* LuaController::getArrLuaData(Ref* src)
{
    Array* arr = dynamic_cast<Array*>(src);
    return arr;
}

//检查登录返回数据 是否存储在了lua系统里
int LuaController::checkLoginDataState(string name)
{
    if(isLuaOpen()==false)
        return 0;
    if(initLua()==false)
        return 0;
    if (m_clientVersion.compare(this->luaVersion) != 0)
        return 0;
    LuaEngine* pEngine = LuaEngine::getInstance();
    string path = getLuaPath();
    string filepath = path+"/actMain.lua";
    if (!CCFileUtils::sharedFileUtils()->isFileExist(filepath)) {
        return 0;
    }
    
    if (name == "init_knight") {
        string knPath = path+"/knight/KnightTitleController.lua";
        if (!CCFileUtils::sharedFileUtils()->isFileExist(knPath)) {
            return 0;
        }
    }
    
    pEngine->addSearchPath(path.c_str());
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filepath.c_str());
    pEngine->executeScriptFile(filepath.c_str());
    lua_State* ls = pEngine->getLuaStack()->getLuaState();
    lua_getglobal(ls, "checkLoginDataState");
    lua_pushstring(ls, path.c_str());
    lua_pushstring(ls, name.c_str());
    if (!lua_isfunction(ls, -3))
    {
        lua_pop(ls, 3);
        return 0;
    }
    lua_call(ls, 2, 1);
    
    int ret = -1;
    ret = lua_tonumber(ls,-1);
    if (ret == 1) {
        if (LoginInitDataMap.find(name) != LoginInitDataMap.end()) {
            backCMD(name, LoginInitDataMap[name]);
        }
    }
    return 1;
}

void LuaController::saveLoginData(string name, CCDictionary* dict)
{
    LuaController::getInstance()->LoginInitDataMap[ name ] = CCDictionary::createWithDictionary(dict);
    LuaController::getInstance()->LoginInitDataMap[ name ]->retain();
}

void LuaController::openJoinAllianceView()
{
    PopupViewController::getInstance()->addPopupInView(JoinAllianceView::create(11));
}
