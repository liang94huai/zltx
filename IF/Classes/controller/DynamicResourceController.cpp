//
//  DynamicResourceController.cpp
//  IF
//
//  Created by Émilie.Jiang on 15/3/25.
//
//

//
// 动态资源上传方法
//
// 1. 首先将新资源打包，一般就是plist + pkm(android) 或 pvz.ccz(ios)，具体规则问美术
// 2. 将处理好的资源包，放入 docs/locale 中，一般是dr_xxxx.zip
//    同时修改 docs/locale/LUA.txt 中该资源对应的版本号，譬如1.0.1修改成1.0.2
//    修改之后将 SVN 修改提交
// 3. 使用scp将资源包上传到指定服务器，命令如下。然后后台应该就有脚本自动将最新版本号更新到服务器配置中。
//    scp <lua.zip> cokserver@10.1.6.72:/IF/trunk/src/server/smartfoxserver/SFS2X/resource/lua
//    password : elexhcg
//

#include "DynamicResourceController.h"
#include "ToolController.h"
#include "../Ext/CCDevice.h"
#include <curl/easy.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "../view/guiExt/md5.h"
#include <stdio.h>
#include "external/unzip/unzip.h"
#include "DRConfigCommand.h"
#include "CCThreadManager.h"

#define DynamicResource_Client_VersionKey "DRVersion_client_"
#define DynamicResource_Download_VersionKey "DRVersion_download_"
#define DynamicResource_Zip "tmpZR_"
#define DynamicResource_BUFFER_SIZE    8192
#define DynamicResource_MAX_FILENAME   512

#define DynamicSound_BG "bgsound"
#define DynamicSound_EFFECT "effsound"

#define DynamicMD5_Version "dynamicmd5version.bin"
#define DynamicMD5Unzip_Version "dynamicmd5UnzipVersion.bin"


static DynamicResourceController *_instance = NULL;
DynamicResourceController* DynamicResourceController::getInstance() {
    if (!_instance) {
        _instance = new DynamicResourceController();
    }
    return _instance;
}
DynamicResourceController::DynamicResourceController():m_isInitedCommon(false),m_checkDevice(false){
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    m_writeablePath = cocos2d::extension::CCDevice::getSDcardPath();
//    CCLOG("sdcard path [%s]",m_writeablePath.c_str());
//#endif
    if(m_writeablePath.empty()){
        m_writeablePath = CCFileUtils::sharedFileUtils()->getWritablePath();
        CCLOG("writeablePath - %s", m_writeablePath.c_str());
        
        CCFileUtils::sharedFileUtils()->addSearchPath(m_writeablePath);
    }
    initMD5Map();
}

bool DynamicResourceController::InitServerRes(CCDictionary* data)
{    
    DynamicResourceController::getInstance()->initGoodsInfo(data);
    if((CCCommonUtils::isIOSPad() || CCCommonUtils::isAdriodPadHD()))  // lzy 下载高清资源
    {
        //如果是pad 则高清资源的下载排在第二位
        DynamicResourceController::getInstance()->initHDResourceInfo(data);
        DynamicResourceController::getInstance()->initHDCCBInfo(data);
    }
    DynamicResourceController::getInstance()->initEquipmentInfo(data);
    DynamicResourceController::getInstance()->initServerFightInfo(data);
    DynamicResourceController::getInstance()->initMonsterInfo(data);
    DynamicResourceController::getInstance()->initThorneInfo(data);
    DynamicResourceController::getInstance()->initMonsterLayerInfo(data);
    DynamicResourceController::getInstance()->initHeadIconInfo(data);
    DynamicResourceController::getInstance()->initHeadIcon_bustInfo(data);
    DynamicResourceController::getInstance()->initHeadIcon_middleInfo(data);
    DynamicResourceController::getInstance()->initMinimapViewInfo(data);
    DynamicResourceController::getInstance()->initDragonMiniMapInfo(data);
    DynamicResourceController::getInstance()->initDragonBuildingInfo(data);
    DynamicResourceController::getInstance()->initDragonMapInfo(data);
    DynamicResourceController::getInstance()->initActivityAdInfo(data);
    DynamicResourceController::getInstance()->initUIRESInfo(data);
    DynamicResourceController::getInstance()->setCheckDevice(true);
    DynamicResourceController::getInstance()->initActivityResInfo(data);
    DynamicResourceController::getInstance()->initScienceInfo(data);

    return true;
}

void DynamicResourceController::sendGetCommonCMD(){
    if(m_isInitedCommon == false){
        DRConfigCommand *drCommand = new DRConfigCommand();
        drCommand->sendAndRelease();
    }
}

void DynamicResourceController::downLoadAllCommon(){
    if(CCTexture2D::useDownloadResource()==true){
        int index=0;
        while (index<CC_SAMLL_TEXTURE_CNT) {
            char buffer[16];
            sprintf(buffer,"Common_%d",CCTexture2D::SmallCommonList[index]);
            std::string name = buffer;
            auto iter = mServerQueue.begin();
            while (iter!=mServerQueue.end()) {
                if((*iter).name.compare(name) == 0){
                    //CCLog("android_test_checkCommonResource[%s]",name.c_str());
                    bool isNew = hasNameTypeResource(name,(*iter).needSize,(*iter).md5);
                    auto pos = name.find("_");
                    if(pos != string::npos){
                        int commonidx = atoi(name.substr(pos+1).c_str());
                        saveCommonStatus(commonidx,isNew);
                    }
                }
                ++iter;
            }
            ++index;
        }
        
        index=0;
        while (index < CC_SMALL_GENERAL_TEXTURE_CNT) {
            char buffer[16];
            sprintf(buffer,"General_%d",CCTexture2D::SmallGeneralList[index]);
            std::string name = buffer;
            auto iter = mServerQueue.begin();
            while (iter!=mServerQueue.end()) {
                if((*iter).name.compare(name) == 0){
                    bool isNew = hasNameTypeResource(name,(*iter).needSize,(*iter).md5);
                    auto pos = name.find("_");
                    if(pos != string::npos){
                        int commonidx = atoi(name.substr(pos+1).c_str());
                        saveGeneralStatus(commonidx,isNew);
                    }
                }
                ++iter;
            }
            ++index;
        }
    }
}
void DynamicResourceController::initCommonInfo(CCObject *obj){
    if(!obj){
        m_isInitedCommon=true;
        return;
    }
    CCArray* array = dynamic_cast<CCArray*>(obj);
    if(array){
        CCObject* ccObj = NULL;
        CCARRAY_FOREACH(array, ccObj){
            CCDictionary* _dict = dynamic_cast<CCDictionary*>(ccObj);
            if(!_dict)
                continue;
            if(_dict->objectForKey("name") && _dict->objectForKey("version") && _dict->objectForKey("checksize") && _dict->objectForKey("iosmd5") && _dict->objectForKey("androidmd5")){
                
                bool isIn = false;
                int index0=0;
                while (index0<CC_SAMLL_TEXTURE_CNT) {
                    char buffer[16];
                    sprintf(buffer,"Common_%d",CCTexture2D::SmallCommonList[index0]);
                    string aa = _dict->valueForKey("name")->getCString();
                    if(aa.compare(buffer) == 0){
                        isIn = true;
                        break;
                    }
                    ++index0;
                }
                
                if(!isIn){
                    index0=0;
                    while (index0<CC_SMALL_GENERAL_TEXTURE_CNT) {
                        char buffer[16];
                        sprintf(buffer,"General_%d", CCTexture2D::SmallGeneralList[index0]);
                        string aa = _dict->valueForKey("name")->getCString();
                        if(aa.compare(buffer) == 0){
                            isIn = true;
                            break;
                        }
                        ++index0;
                    }
                }
                
                if(isIn ==false){
                    string name = _dict->valueForKey("name")->getCString();
                    if(name == DynamicSound_BG ||  name  == DynamicSound_EFFECT){
                        isIn=true;
                    }else{
                        continue;
                    }
                }
                DynamicResource dr;
                dr.name = _dict->valueForKey("name")->getCString();
                dr.needSize = _dict->valueForKey("checksize")->intValue();
                dr.newV = _dict->valueForKey("version")->getCString();
                
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                std::string code = _dict->valueForKey("androidmd5")->getCString();
#else
                std::string code = _dict->valueForKey("iosmd5")->getCString();
#endif
                
                if(!code.empty()){
                    dr.md5 =code;
                }
                if(!dr.md5.empty()){
                    mServerQueue.push_back(dr);
                }
            }
        }
    }
    m_isInitedCommon=true;
    downLoadAllCommon();
    checkSoundResource();
    
}

bool DynamicResourceController::checkGeneralResource(string name){
    std::string oldV = "";
    oldV = CCUserDefault::sharedUserDefault()->getStringForKey("dr_general_oldv");
//    if(!oldV.empty()){
//        m_md5Map["dr_general_oldv"] = oldV;
//        saveMD5Map();
//    }
    //    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    int idx = 0;
    bool isIn = false;
    while (idx < CC_SMALL_GENERAL_TEXTURE_CNT) {
        string cmpName = string("General_")+CC_ITOA(CCTexture2D::SmallGeneralList[idx]);
        if(name.compare(cmpName) == 0){
            if(oldV.empty() || (idx>=oldV.size())){
                return false;
            }else{
                if(oldV.at(idx) == '1'){
                    return false;
                }else{
                    std::string fPath  =  m_writeablePath + "dresource/"+ cmpName + ".pkm";
                    if(CCFileUtils::sharedFileUtils()->isFileExist(fPath)){
                        return true;
                    }else{
                        return false;
                    }
                }
            }
            break;
        }
        ++idx;
    }
#else
    if(CCTexture2D::useDownloadResource()){
        int idx = 0;
        bool isIn = false;
        while (idx < CC_SMALL_GENERAL_TEXTURE_CNT) {
            string cmpName = string("General_")+CC_ITOA(CCTexture2D::SmallGeneralList[idx]);
            if(name.compare(cmpName) == 0){
                if(oldV.empty() || (idx>=oldV.size())){
                    return false;
                }else{
                    if(oldV.at(idx) == '1'){
                        return false;
                    }else{
                        std::string fPath  =  m_writeablePath + "dresource/"+ cmpName + ".pvr.ccz";
                        if(CCFileUtils::sharedFileUtils()->isFileExist(fPath)){
                            return true;
                        }else{
                            return false;
                        }
                    }
                }
                break;
            }
            ++idx;
        }
    }
#endif
    return false;
}

bool DynamicResourceController::checkCommonResource(string name){
    std::string oldV = "";

//    if(m_md5Map.find("dr_common_oldv")!=m_md5Map.end()){
//        oldV = m_md5Map["dr_common_oldv"];
//    }else{
        oldV = CCUserDefault::sharedUserDefault()->getStringForKey("dr_common_oldv");
//        if(!oldV.empty()){
//            m_md5Map["dr_common_oldv"] = oldV;
//            saveMD5Map();
//        }
//    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    int idx = 0;
    bool isIn = false;
    while (idx < CC_SAMLL_TEXTURE_CNT) {
        string cmpName = string("Common_")+CC_ITOA(CCTexture2D::SmallCommonList[idx]);
        if(name.compare(cmpName) == 0){
            if(oldV.empty() || (idx>=oldV.size())){
                return false;
            }else{
                if(oldV.at(idx) == '1'){
                    return false;
                }else{
                    std::string fPath  =  m_writeablePath + "dresource/_alpha_"+ cmpName + ".pkm";
                    if(CCFileUtils::sharedFileUtils()->isFileExist(fPath)){
                        return true;
                    }else{
                        return false;
                    }
                }
            }
            break;
        }
        ++idx;
    }
#else
    if(CCTexture2D::useDownloadResource()){
        int idx = 0;
        bool isIn = false;
        while (idx < CC_SAMLL_TEXTURE_CNT) {
            string cmpName = string("Common_")+CC_ITOA(CCTexture2D::SmallCommonList[idx]);
            if(name.compare(cmpName) == 0){
                if(oldV.empty() || (idx>=oldV.size())){
                    return false;
                }else{
                    if(oldV.at(idx) == '1'){
                        return false;
                    }else{
                        std::string fPath  =  m_writeablePath + "dresource/"+ cmpName + ".pvr.ccz";
                        if(CCFileUtils::sharedFileUtils()->isFileExist(fPath)){
                            return true;
                        }else{
                            return false;
                        }
                    }
                }
                break;
            }
            ++idx;
        }
    }
#endif
    return false;
}
void DynamicResourceController::saveCommonStatus(int commonidx,bool downloaded){
    std::string oldV = "";

//    if(m_md5Map.find("dr_common_oldv")!=m_md5Map.end()){
//        oldV = m_md5Map["dr_common_oldv"];
//    }else{
        oldV = CCUserDefault::sharedUserDefault()->getStringForKey("dr_common_oldv");
//        if(!oldV.empty()){
//            m_md5Map["dr_common_oldv"] = oldV;
//            saveMD5Map();
//        }
//    }

    if(oldV.size() < CC_SAMLL_TEXTURE_CNT){
        int index=oldV.size();
        while (index<CC_SAMLL_TEXTURE_CNT) {
            oldV.append("0");
            ++index;
        }
    }
    bool isfind = false;
    int index=0;
    while (index<CC_SAMLL_TEXTURE_CNT) {
        if(commonidx == CCTexture2D::SmallCommonList[index]){
            isfind=true;
            break;
        }
        ++index;
    }
    if(isfind==true){
        oldV.replace(index, 1, downloaded?"0":"1");
        CCUserDefault::sharedUserDefault()->setStringForKey("dr_common_oldv",oldV);
        CCUserDefault::sharedUserDefault()->flush();

        CCLOG("res common ver(%s).", oldV.c_str());
//        m_md5Map["dr_common_oldv"]=oldV;
//        saveMD5Map();
    }
}

void DynamicResourceController::saveGeneralStatus(int idx,bool downloaded){
    std::string oldV = "";
    
    oldV = CCUserDefault::sharedUserDefault()->getStringForKey("dr_general_oldv");
//    if(!oldV.empty()){
//        m_md5Map["dr_general_oldv"] = oldV;
//        saveMD5Map();
//    }
    if(oldV.size() < CC_SMALL_GENERAL_TEXTURE_CNT){
        int index = oldV.size();
        while (index < CC_SMALL_GENERAL_TEXTURE_CNT) {
            oldV.append("0");
            ++index;
        }
    }
    bool isfind = false;
    int index = 0;
    while (index < CC_SMALL_GENERAL_TEXTURE_CNT) {
        if(idx == CCTexture2D::SmallGeneralList[index]){
            isfind=true;
            break;
        }
        ++index;
    }
    if(isfind==true){
        oldV.replace(index, 1, downloaded?"0":"1");
        CCUserDefault::sharedUserDefault()->setStringForKey("dr_general_oldv",oldV);
        CCUserDefault::sharedUserDefault()->flush();
        CCLOG("res general ver(%s).", oldV.c_str());
//        m_md5Map["dr_general_oldv"]=oldV;
//        saveMD5Map();
    }
}

void DynamicResourceController::checkSoundResource()
{
    if(m_isInitedCommon==false){
        return;
    }
    vector<DynamicResource> pushDr;
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end()) {
        if((*iter).name.compare(DynamicSound_BG) == 0 || (*iter).name.compare(DynamicSound_EFFECT) == 0){
            std::string vClient;
            std::string& strServerMD5 = (*iter).md5;

            std::string keyclient = DynamicResource_Client_VersionKey + (*iter).name;
            vClient = CCUserDefault::sharedUserDefault()->getStringForKey(keyclient.c_str());
            //m_md5Map[(*iter).name]=vClient;
            //saveMD5Map();
            
            if(vClient != strServerMD5){
                pushDr.push_back((*iter));
            }
            else{
                if((*iter).name.compare(DynamicSound_BG) == 0){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                    string file1 = m_writeablePath + DynamicSound_Folder + "background/m_city_new.ogg";
                    string file2 = m_writeablePath + DynamicSound_Folder + "background/m_field_new.ogg";
#else
                    string file1 = m_writeablePath + DynamicSound_Folder + "background/m_city_new.aac";
                    string file2 = m_writeablePath + DynamicSound_Folder + "background/m_field_new.aac";
#endif
                    if(CCFileUtils::sharedFileUtils()->isFileExist(file1) &&CCFileUtils::sharedFileUtils()->isFileExist(file2)){
                        ++iter;
                        continue;
                    }
                    pushDr.push_back((*iter));
                }else{
                    ++iter;
                    continue;
                }
            }
        }
        ++iter;
    }
    
    saveMD5Map();
    
    // 没有下载的去下载之
    auto iter0 = pushDr.begin();
    while (iter0!=pushDr.end()) {
        if (isDownloading((*iter0).name) == false) {
            mQueue.push_back((*iter0).name);
            DynamicResource dr = {(*iter0).name,"",(*iter0).md5,(*iter0).md5,(*iter0).needSize,true,DynamicResourceType_None};
            startDownload(dr);
        }
        ++iter0;
    }
}
void DynamicResourceController::downloadSoundResource(string name)
{
    if (isDownloading(name)) {
        return;
    }
    
    auto iter0 = mServerQueue.begin();
    while (iter0!=mServerQueue.end()) {
        if((*iter0).name.compare(name) == 0){
            DynamicResource dr = {name,"",(*iter0).md5,(*iter0).md5,1024,true,DynamicResourceType_None};
            mQueue.push_back(name);
            startDownload(dr);
            break;
        }
        ++iter0;
    }
}

bool DynamicResourceController::checkMainCityResource(string name, string md5){
    return hasNameTypeResource(name,512,md5,true);
}
void DynamicResourceController::initMainCityConfig(string name,map<string,string> &skinMap){
    string drPath = getDownloadPath() + "skinparticle/";
    string configfile = drPath + name + ".config";
    skinMap.clear();
    CCCommonUtils::FileToMap(configfile, skinMap);
}

bool DynamicResourceController::hasNameTypeResource(std::string name,int needSize,std::string md5,bool platform)
{
    if (isDownloading(name)) {
        return false;
    }
    
    std::string vClient;
    std::string keyclient = DynamicResource_Client_VersionKey + name;
    
    // 这里存的应该是md5...
    vClient = CCUserDefault::sharedUserDefault()->getStringForKey(keyclient.c_str());
    //m_md5Map[name]=vClient;
    //saveMD5Map();
    
//    if (name.find("castle_1") != std::string::npos) {
//        int a = 0;
//    }
    
    // 当前版本和文件正确，那么就表示OK了
    if (vClient == md5 &&checkResourceFile(name, platform))
    {
        CCLOG("*res (%s) is lastest. md5(%s)", name.c_str(), vClient.c_str());
        return true;
    }
    
    DynamicResource dr = {name,"",md5,md5,needSize,platform,DynamicResourceType_Name};
    mQueue.push_back(name);
    startDownload(dr);
    CCLOG("*res (%s) need update. md5 (%s) -> (%s)", name.c_str(), vClient.c_str(), md5.c_str());
    
    return false;
}

bool DynamicResourceController::hasVersionTypeResource(std::string name,std::string newV,int needSize,std::string md5)
{
    if (isDownloading(name)) {
        return false;
    }
    
    std::string keyclient = DynamicResource_Client_VersionKey + name;
    std::string vClient = CCUserDefault::sharedUserDefault()->getStringForKey(keyclient.c_str());
    
//    if (name == "uires") {
//        int a = 0;
//    }
    
    // 当前版本和文件正确，那么就表示OK了
    if (vClient == newV &&
        checkResourceFile(name))
    {
        CCLOG("*res (%s) is lastest. (%s)", name.c_str(), vClient.c_str());
        return true;
    }

    DynamicResource dr = {name,vClient,newV,md5,needSize,true,DynamicResourceType_Version};
    mQueue.push_back(name);
    startDownload(dr);
    CCLOG("*res (%s) need update. (%s) -> (%s)", name.c_str(), vClient.c_str(), newV.c_str());
    
    return false;
}

bool DynamicResourceController::checkResourceFile(std::string name,bool platform)
{
    std::vector<std::string> filelist;
    std::string fileName1 = m_writeablePath + DynamicResource_Folder + name + ".plist";
    if(name == DynamicResource_MONSTER_TEXTURE){
        fileName1 = m_writeablePath + DynamicResource_Folder + "World_3.atlas";
    }else if(name == DynamicResource_THORNE_TEXTURE){
        fileName1 = m_writeablePath + DynamicResource_Folder + "Common_301.plist";
    } else if (CCCommonUtils::isPad() && DynamicResource_HD_RES == name)
    {
        fileName1 = m_writeablePath + DynamicResource_Folder + "Common/Common_101_hd.plist";
    }
    else if (name == DynamicResource_HD_CCB)
    {
        fileName1 = m_writeablePath + DynamicResource_Folder + "hdccbi/";
    }
    else if (name == DynamicResource_UI_RES)
    {
        fileName1 = m_writeablePath + DynamicResource_Folder + "ui_res/ui_res_%d.plist";
        int uiType = CCUserDefault::sharedUserDefault()->getIntegerForKey(UI_SKIN_TYPE_LAST_TIME, 0);
        uiType = uiType <= 0? 1 : uiType;
        fileName1 = CCString::createWithFormat(fileName1.c_str(), uiType)->getCString();
    }
    
    filelist.push_back(fileName1);
    
    if(platform==true){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string fileName2 = m_writeablePath + DynamicResource_Folder +"_alpha_" + name + ".pkm";
        std::string fileName3 = m_writeablePath + DynamicResource_Folder +"_alpha_" + name + "_alpha.pkm";
        if(name == DynamicResource_MONSTER_TEXTURE){
            fileName2 = m_writeablePath + DynamicResource_Folder + "_alpha_World_3.pkm";
            fileName3 = m_writeablePath + DynamicResource_Folder + "_alpha_World_3_alpha.pkm";
        }else if(name == DynamicResource_THORNE_TEXTURE){
            fileName2 = m_writeablePath + DynamicResource_Folder + "_alpha_Common_301.pkm";
            fileName3 = m_writeablePath + DynamicResource_Folder + "_alpha_Common_301_alpha.pkm";
        }
        else if(name == DynamicResource_HD_RES){
            fileName2 = m_writeablePath + DynamicResource_Folder + "Common/_alpha_Common_101_hd.pkm";
            fileName3 = m_writeablePath + DynamicResource_Folder + "Common/_alpha_Common_101_hd_alpha.pkm";
        }
        else if (name == DynamicResource_HD_CCB)
        {
            fileName2 = m_writeablePath + DynamicResource_Folder + "hdccbi/";
            fileName3 = m_writeablePath + DynamicResource_Folder + "hdccbi/";
        }
        else if (name == DynamicResource_UI_RES)
        {
            fileName2 = m_writeablePath + DynamicResource_Folder + "ui_res/_alpha_ui_res_%d.pkm";
            fileName3 = m_writeablePath + DynamicResource_Folder + "ui_res/_alpha_ui_res_%d_alpha.pkm";
            int uiType = CCUserDefault::sharedUserDefault()->getIntegerForKey(UI_SKIN_TYPE_LAST_TIME, 0);
            uiType = uiType <= 0? 1 : uiType;
            fileName2 = CCString::createWithFormat(fileName2.c_str(), uiType)->getCString();
            fileName3 = CCString::createWithFormat(fileName3.c_str(), uiType)->getCString();
        }
        
        if (name == DynamicResource_HEAD_TEXTURE_ICON_BUST) {
            string tempFileName = m_writeablePath + DynamicResource_Folder + name + ".pkm";
            filelist.push_back(tempFileName);
        }
        else{
            filelist.push_back(fileName2);
            filelist.push_back(fileName3);
        }
        
#else
        std::string fileName2 = m_writeablePath + DynamicResource_Folder + name + ".pvr.ccz";
        if(name == DynamicResource_MONSTER_TEXTURE){
            fileName2 = m_writeablePath + DynamicResource_Folder + "World_3.pvr.ccz";
        }else if(name == DynamicResource_THORNE_TEXTURE){
            fileName2 = m_writeablePath + DynamicResource_Folder + "Common_301.pvr.ccz";
        }else if(name == DynamicResource_HD_RES){
            fileName2 = m_writeablePath + DynamicResource_Folder + "Common/Common_101_hd.pvr.ccz";
        } else if (name == DynamicResource_HD_CCB){
            fileName2 = m_writeablePath + DynamicResource_Folder + "hdccbi/";
        } else if (name == DynamicResource_UI_RES) {
            fileName2 = m_writeablePath + DynamicResource_Folder + "ui_res/ui_res_%d.pvr.ccz";
            int uiType = CCUserDefault::sharedUserDefault()->getIntegerForKey(UI_SKIN_TYPE_LAST_TIME, 0);
            uiType = uiType <= 0? 1 : uiType;
            fileName2 = CCString::createWithFormat(fileName2.c_str(), uiType)->getCString();
        }
        
        filelist.push_back(fileName2);
#endif
    }else{
        std::string fileName2 = m_writeablePath + DynamicResource_Folder + name + ".pvr.ccz";
        if(name == DynamicResource_MONSTER_TEXTURE){
            fileName2 = m_writeablePath + DynamicResource_Folder + "World_3.pvr.ccz";
        }else if(name == DynamicResource_THORNE_TEXTURE){
            fileName2 = m_writeablePath + DynamicResource_Folder + "Common_301.pvr.ccz";
        }else if(name == DynamicResource_HD_RES){
            fileName2 = m_writeablePath + DynamicResource_Folder + "Common/Common_101_hd.pvr.ccz";
        } else if (name == DynamicResource_HD_CCB) {
            fileName2 = m_writeablePath + DynamicResource_Folder + "hdccbi/";
        } else if (name == DynamicResource_UI_RES) {
            fileName2 = m_writeablePath + DynamicResource_Folder + "ui_res/ui_res_%d.pvr.ccz";
            int uiType = CCUserDefault::sharedUserDefault()->getIntegerForKey(UI_SKIN_TYPE_LAST_TIME, 0);
            uiType = uiType <= 0? 1 : uiType;
            fileName2 = CCString::createWithFormat(fileName2.c_str(), uiType)->getCString();
        }
        filelist.push_back(fileName2);
    }
    if(name == DynamicResource_DRAGON_MAP){
        std::string fileName2 = m_writeablePath + DynamicResource_Folder + "DragonMap.tmx";
        filelist.push_back(fileName2);
    }
    auto iter = filelist.begin();
    while (iter!=filelist.end()) {
        if(!CCFileUtils::sharedFileUtils()->isFileExist((*iter))){
            return false;
        }
        ++iter;
    }
    return true;
}

void DynamicResourceController::DownloadThread(CCObject *ccObj)
{
    DynamicResourceParam* drparams = reinterpret_cast<DynamicResourceParam*>(ccObj);
    if(drparams!=NULL){
        drparams->_downret = this->DownAndExtrace(drparams);
        CCThreadManager::getInstance()->runInMainThread(this, callfuncO_selector(DynamicResourceController::DownloadRet), reinterpret_cast<CCObject*>(drparams));
    }
    
    return;
}

void DynamicResourceController::DownloadRet(CCObject *ccObj)
{
    DynamicResourceParam* drparams = reinterpret_cast<DynamicResourceParam*>(ccObj);
    if (drparams->_downret) {
        saveClientVersion(drparams);
    }
    else {
        dealwithError(drparams);
    }
    
    delete drparams;
    return;
}

bool DynamicResourceController::isDownloading(std::string const& name)
{
    if (mQueue.end() == std::find(mQueue.begin(), mQueue.end(), name)) {
        return false;
    }
    return true;
}

bool DynamicResourceController::removeDownloading(std::string const& name)
{
    auto iter = find(mQueue.begin(),mQueue.end(), name);
    if(iter!=mQueue.end()){
        mQueue.erase(iter);
        CCLOG("removeDownloading ok %s.", name.c_str());
        return true;
    }
    
    CCLOG("removeDownloading not found %s.", name.c_str());
    return false;
}

void DynamicResourceController::saveClientVersion(DynamicResourceParam* drparams)
{
    std::string keyclient = DynamicResource_Client_VersionKey + drparams->drr.name;
    
    if(drparams->drr.type == DynamicResourceType_Version) {
        CCUserDefault::sharedUserDefault()->setStringForKey(keyclient.c_str(), drparams->drr.newV);
        CCLOG("*res (%s) save ver (%s).", drparams->drr.name.c_str(), drparams->drr.newV.c_str());
        if (drparams->drr.name == DynamicResource_HD_RES) {
            CCUserDefault::sharedUserDefault()->setBoolForKey(DOWNLOAD_HD_RES, true);
        }
    }
    else if (drparams->drr.type == DynamicResourceType_Name){
        CCUserDefault::sharedUserDefault()->setStringForKey(keyclient.c_str(), drparams->drr.md5);
        CCLOG("*res (%s) save md5 (%s).", drparams->drr.name.c_str(), drparams->drr.md5.c_str());
        //m_md5Map[drparams->drr.name]=drparams->drr.md5;
        //saveMD5Map();
    }
    else{
        if(drparams->drr.name.find("sound") != string::npos){
            CCUserDefault::sharedUserDefault()->setStringForKey(keyclient.c_str(), drparams->drr.newV);
            CCLOG("*res sound (%s) save ver (%s).", drparams->drr.name.c_str(), drparams->drr.newV.c_str());
            //m_md5Map[drparams->drr.name]=drparams->drr.newV;
            //saveMD5Map();
        }
    }
    
    if(drparams->drr.name.find("Common")!=string::npos){
        auto pos = drparams->drr.name.find("_");
        if(pos != string::npos){
            int commonidx = atoi(drparams->drr.name.substr(pos+1).c_str());
            saveCommonStatus(commonidx,true);
        }
    }
    if(drparams->drr.name.find("General")!=string::npos){
        auto pos = drparams->drr.name.find("_");
        if(pos != string::npos){
            int generalidx = atoi(drparams->drr.name.substr(pos+1).c_str());
            saveGeneralStatus(generalidx,true);
        }
    }
   
    CCUserDefault::sharedUserDefault()->flush();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_DYNAMIC_DOWNLOAD_FINISH,CCString::create(drparams->drr.name));
    
    removeDownloading(drparams->drr.name);
    string zipfile = m_writeablePath + DynamicResource_Zip + drparams->drr.name + ".zip";
    std::remove(zipfile.c_str());
    
    CCLOG("*res (%s) all ok.", drparams->drr.name.c_str());
    
    if (mQueue.empty()) {
        CCLOG("mQueue empty!!!!!");
    }
    return;
}

void DynamicResourceController::dealwithError(DynamicResourceParam* drparams)
{
    removeDownloading(drparams->drr.name);
    return;
}

bool DynamicResourceController::getDownloadUrl(DynamicResource const& dr, std::string& url)
{
    std::string platform;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    platform = "android";
#else
    platform = "ios";
#endif
    
    std::string m_packagePath;
    
    // COCOS2D_DEBUG : 0是外网,1是内网,2是beta
#if COCOS2D_DEBUG == 1
    string hostIP = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_IP);
    CCLOG("lua_host_url [%s]",hostIP.c_str());
    if (hostIP.empty()) {
        hostIP = SERVER_IP;
    }
    if(dr.type == DynamicResourceType_Version){
        m_packagePath = "http://"+hostIP+":8080/gameservice/getfile?drVersion=" + dr.name + dr.newV;
    }else{
        m_packagePath = "http://"+hostIP+":8080/gameservice/getfile?drVersion=" + dr.name;
    }
    
    if(dr.platform==true){
        m_packagePath.append("_");
        m_packagePath.append(platform);
    }
#else
    m_packagePath = "http://cok.eleximg.com/cok/lua/dr_" + dr.name + dr.newV;
    if(dr.platform==true){
        m_packagePath.append("_");
        m_packagePath.append(platform);
    }
    m_packagePath.append(".zip");
#endif
    
    CCLOG("DynamicResource_package_url [%s]",m_packagePath.c_str());
    url = m_packagePath;
    
    return true;
}


bool DynamicResourceController::DownAndExtrace(DynamicResourceParam* drparams)
{
    std::string strMD5;
    bool bNeedDown = true;
    
    string m_downFile = m_writeablePath + DynamicResource_Zip + drparams->drr.name+".zip";
    
    // 磁盘空间不足，不足以下载。直接*2把下载和解压的空间一起都判断一下。
    if (!cocos2d::extension::CCDevice::hasEnoughSpace(drparams->drr.needSize * 2)) {
        CCLOG("*res DownAndExtrace - not enough disk space!");
        return false;
    }
    
    // 如果文件已经本地存在，并且md5正确，那么就不用再下载。
    if (CCFileUtils::sharedFileUtils()->isFileExist(m_downFile)){
        if (CCCommonUtils::FileToMD5(m_downFile, strMD5) && (strMD5 == drparams->drr.md5)) {
            bNeedDown = false;
            CCLOG("*res DownAndExtrace - file already exist. %s.", m_downFile.c_str());
        }
        else {
            std::remove(m_downFile.c_str());
        }
    }
    
    // 需要下载的话再去下载
    if (bNeedDown)
    {
        std::string url;
        getDownloadUrl(drparams->drr, url);
        
        if (CCCommonUtils::downloadFile(url, m_downFile) == false) {
            return false;
        }
        
        // 下载完毕需要再次检查MD5，有可能下载失败
        if (CCCommonUtils::FileToMD5(m_downFile, strMD5) && (strMD5 == drparams->drr.md5)) {
        } else {
            CCLOG("*res DownAndExtrace - file(%s) download but signature error. local(%s) server(%s).",
                  drparams->drr.name.c_str(), strMD5.c_str(), drparams->drr.md5.c_str());
            std::remove(m_downFile.c_str());
            return false;
        }
    }
    
    // 解压到指定目录。这里可能出现正在解压目录，结果CCSprite正好创建导致错误。所以这里依赖于mQueue，只要在mQueue列队中的资源就表示暂时禁止访问。
    if (CCCommonUtils::unzipToDirectory(m_downFile, m_writeablePath, true, true) == false) {
        return false;
    }
    
    CCLOG("*res DownAndExtrace - dr ok(%s).", drparams->drr.name.c_str());
    
    // for test
    usleep(100 * 1000); // 10 ms
    
    return true;
}

void DynamicResourceController::uncompressLocalZip(int type)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //    if(!cocos2d::extension::CCDevice::hasEnoughSpace(10*1024)){
    //
    //        return;
    //    }
    
    std::string _appVersion = cocos2d::extension::CCDevice::getVersionName();
    std::string _version_code = cocos2d::extension::CCDevice::getVersionCode();
    
    std::string _unzip_version = CCUserDefault::sharedUserDefault()->getStringForKey("unzip_version", "");
    std::string _unzip_version_code = CCUserDefault::sharedUserDefault()->getStringForKey("unzip_version_code", "");
    
    if (!_unzip_version.compare(_appVersion) && !_unzip_version_code.compare(_version_code))
    {
        return;
    }
    
    std::string unzipfiles[] = {"ccbi/ccbi.zip", "particle/particle.zip", "hdccbi/hdccbi.zip", "Spine/Spine.zip"};
    
    if (type == 2 && FileUtils::getInstance()->isFileExist(unzipfiles[0]))
    {
        CCUserDefault::sharedUserDefault()->setStringForKey("unzip_version", _appVersion);
        CCUserDefault::sharedUserDefault()->setStringForKey("unzip_version_code", _version_code);
        CCUserDefault::sharedUserDefault()->flush();
    }

    
    if (type==0 && FileUtils::getInstance()->isFileExist(unzipfiles[0]))
    {
        std::string strPath = FileUtils::getInstance()->fullPathForFilename(unzipfiles[0]);
        uncompressLocalZipInThread(CCString::create(strPath));
        //        CCThreadManager::getInstance()->runInNewThread(this, callfuncO_selector(DynamicResourceController::uncompressLocalZipInThread), CCString::create(strPath));
    }
    
    if (type==1 && FileUtils::getInstance()->isFileExist(unzipfiles[1]))
    {
        std::string strPath = FileUtils::getInstance()->fullPathForFilename(unzipfiles[1]);
        uncompressLocalZipInThread(CCString::create(strPath));
        //        CCThreadManager::getInstance()->runInNewThread(this, callfuncO_selector(DynamicResourceController::uncompressLocalZipInThread), CCString::create(strPath));
    }
    
    if (type==2 && FileUtils::getInstance()->isFileExist(unzipfiles[3]))
    {
        std::string strPath = FileUtils::getInstance()->fullPathForFilename(unzipfiles[3]);
        uncompressLocalZipInThread(CCString::create(strPath));
        //        CCThreadManager::getInstance()->runInNewThread(this, callfuncO_selector(DynamicResourceController::uncompressLocalZipInThread2), CCString::create(strPath));
    }
    
    if (type==0 && FileUtils::getInstance()->isFileExist(unzipfiles[2]))
    {
        
        std::string strPath = FileUtils::getInstance()->fullPathForFilename(unzipfiles[2]);
        //        uncompressLocalZipInThread(CCString::create(strPath));
        CCThreadManager::getInstance()->runInNetThread(this, callfuncO_selector(DynamicResourceController::uncompressLocalZipInThread), CCString::create(strPath));
    }
#endif
}

void DynamicResourceController::uncompressLocalZipInThread(CCObject *ccObj){
    
    CCString* strObj = dynamic_cast<CCString*>(ccObj);
    if (!strObj)
    {
        CCLOG("param is NULL!");
        return;
    }
    
    if (CCCommonUtils::unzipToDirectory(strObj->getCString(), m_writeablePath))
    {
        CCLOG(" unzip is OK!");
    }
//    else
//    {
//        CCThreadManager::getInstance()->runInNetThread(this, callfuncO_selector(DynamicResourceController::uncompressLocalZipInThread), CCString::create(strPath));
//    }
}

void DynamicResourceController::startDownload(DynamicResource ddr)
{
    // 开始下载资源，主体在另外线程中
    DynamicResourceParam *params = new DynamicResourceParam;
    params->drr = ddr;
    params->_isRestart = false;
    params->_downret = false;
    
    CCThreadManager::getInstance()->runInNetThread(this, callfuncO_selector(DynamicResourceController::DownloadThread), reinterpret_cast<CCObject*>(params));
}

void DynamicResourceController::loadNameTypeResource(std::string name, bool release, bool hasAlpha)
{
    //如果文件错误 则删除文件
    auto cleanFunc = [=](){
        std::string fileName = m_writeablePath + DynamicResource_Folder + name + ".plist";
        std::remove(fileName.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string textName1 = m_writeablePath + DynamicResource_Folder +"_alpha_" + name + ".pkm";
        std::string textName2 = m_writeablePath + DynamicResource_Folder +"_alpha_" + name + "_alpha.pkm";
        if (DynamicResource_HEAD_TEXTURE_ICON_BUST == name || !hasAlpha) {
            textName1 = m_writeablePath + DynamicResource_Folder + name + ".pkm";
            textName2 = m_writeablePath + DynamicResource_Folder + name + ".pkm";
        }
        std::remove(textName1.c_str());
        std::remove(textName2.c_str());
#else
        std::string textName = m_writeablePath + DynamicResource_Folder + name + ".pvr.ccz";
        std::remove(textName.c_str());
#endif
    };
    
    
    std::string fileName = m_writeablePath + DynamicResource_Folder + name + ".plist";
    CCLOG("android_test_loadNameTypeResourceName[%s]",fileName.c_str());
    
    if(!CCFileUtils::sharedFileUtils()->isFileExist(fileName)){
        CCLOG("android_test_loadNameTypeResourceName[%s]_file_not_exite",fileName.c_str());
        return;
    }
    if(release==false){
        long size0 = FileUtils::getInstance()->getFileSize(fileName);
        if (size0 <= 0 && size0 >= -1) {
            cleanFunc();
            return;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string textName1 = m_writeablePath + DynamicResource_Folder +"_alpha_" + name + ".pkm";
        std::string textName2 = m_writeablePath + DynamicResource_Folder +"_alpha_" + name + "_alpha.pkm";
        if (DynamicResource_HEAD_TEXTURE_ICON_BUST == name || !hasAlpha) {
            textName1 = m_writeablePath + DynamicResource_Folder + name + ".pkm";
            textName2 = m_writeablePath + DynamicResource_Folder + name + ".pkm";
        }
        if (!FileUtils::getInstance()->isFileExist(textName1) || !FileUtils::getInstance()->isFileExist(textName2)) {
            cleanFunc();
            return;
        }
        long size1 = FileUtils::getInstance()->getFileSize(textName1);
        long size2 = FileUtils::getInstance()->getFileSize(textName2);
        if ((size1 <= 0 && size1 >= -1) || (size2 <= 0 && size2 >= -1))
        {
            cleanFunc();
            return;
        }
#else
        std::string textName = m_writeablePath + DynamicResource_Folder + name + ".pvr.ccz";
        if (!FileUtils::getInstance()->isFileExist(textName)) {
            cleanFunc();
            return;
        }
        long size1 = FileUtils::getInstance()->getFileSize(textName);
        if (size1 <= 0 && size1 >= -1) {
            cleanFunc();
            return;
        }
#endif
        cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(fileName.c_str());
    }else{
        cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(fileName.c_str());
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string textName1 = m_writeablePath + DynamicResource_Folder +"_alpha_" + name + ".pkm";
        if (DynamicResource_HEAD_TEXTURE_ICON_BUST == name || !hasAlpha) {
            textName1 = m_writeablePath + DynamicResource_Folder + name + ".pkm";
        }
        
        CCTexture2D *texture1 =  CCTextureCache::sharedTextureCache()->textureForKey(textName1.c_str());
        if(texture1){
            CCTextureCache::sharedTextureCache()->removeTexture(texture1);
            CCLOG("DynamicResource_texutre_remove:[%s]",textName1.c_str());
        }
        //        std::string textName2 = m_writeablePath + DynamicResource_Folder +"_alpha_" + name + "_alpha.pkm";
        //        CCTexture2D *texture2 =  CCTextureCache::sharedTextureCache()->textureForKey(textName2.c_str());
        //        if(texture2){
        //            CCTextureCache::sharedTextureCache()->removeTexture(texture2);
        //            CCLOG("DynamicResource_texutre_remove:[%s]",textName2.c_str());
        //        }
#else
        std::string textName = m_writeablePath + DynamicResource_Folder + name + ".pvr.ccz";
        CCTexture2D *texture =  CCTextureCache::sharedTextureCache()->textureForKey(textName.c_str());
        if(texture){
            CCTextureCache::sharedTextureCache()->removeTexture(texture);
        }
#endif
        
    }
}

void DynamicResourceController::initMD5Map()
{
//    if(m_md5Map.size()>0)
//        return;
//    
//    std::string oVersionFile = getDownloadPath() +DynamicMD5_Version;
//    CCCommonUtils::FileToMap(oVersionFile, m_md5Map);
    return;
}

void DynamicResourceController::saveMD5Map()
{
//    std::string oVersionFile = getDownloadPath() +DynamicMD5_Version;
//    
//    CCCommonUtils::SaveMapToFile(m_md5Map, oVersionFile);
}

string DynamicResourceController::getDownloadPath()
{
    return m_writeablePath + DynamicResource_Folder;
}

void DynamicResourceController::sendDeviceLog()
{
    if(m_checkDevice){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        SendDeviceCollectCommand *send = new SendDeviceCollectCommand();
        if(send->getHasParams()){
            send->sendAndRelease();
        }else{
            send->release();
        }
#endif
        m_checkDevice=false;
    }
}














































































// 第二期再重构这里，防止问题过多
#if 1

void DynamicResourceController::initGoodsInfo(cocos2d::CCDictionary *ccDict){
    if(!ccDict)
        return;
    
    DynamicResource dr;
    dr.name = DynamicResource_GOODS_TEXTURE;
    if (ccDict->objectForKey("goodsVersion") && ccDict->objectForKey("goodsMd5Android") && ccDict->objectForKey("goodsMd5Ios")) {
        std::string version = ccDict->valueForKey("goodsVersion")->getCString();
        if(!version.empty()){
            if(version.compare("0.0.0") == 0){
                return;
            }
            dr.newV = version;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string code = ccDict->valueForKey("goodsMd5Android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        std::string code = ccDict->valueForKey("goodsMd5Ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        mServerQueue.push_back(dr);
        checkGoodsResource();
    }
}
bool DynamicResourceController::checkGoodsResource(){
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end()) {
        if((*iter).name.compare(DynamicResource_GOODS_TEXTURE) == 0){
            return hasVersionTypeResource(DynamicResource_GOODS_TEXTURE,(*iter).newV,1024,(*iter).md5);
        }
        ++iter;
    }
    return false;
}

void DynamicResourceController::initMonsterInfo(CCDictionary* ccDict){
    if(!ccDict)
        return;
    
    DynamicResource dr;
    dr.name = DynamicResource_MONSTER_TEXTURE;
    if (ccDict->objectForKey("monsterVersion") && ccDict->objectForKey("monsterMd5Android") && ccDict->objectForKey("monsterMd5Ios")) {
        std::string version = ccDict->valueForKey("monsterVersion")->getCString();
        if(!version.empty()){
            if(version.compare("0.0.0") == 0){
                return;
            }
            dr.newV = version;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string code = ccDict->valueForKey("monsterMd5Android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        std::string code = ccDict->valueForKey("monsterMd5Ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        mServerQueue.push_back(dr);
        checkMonsterResource();
    }
}

bool DynamicResourceController::checkMonsterResource(){
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end()) {
        if((*iter).name.compare(DynamicResource_MONSTER_TEXTURE) == 0){
            return hasVersionTypeResource(DynamicResource_MONSTER_TEXTURE,(*iter).newV,1024,(*iter).md5);
        }
        ++iter;
    }
    return false;
}

void DynamicResourceController::initThorneInfo(CCDictionary *ccDict){
    if(!ccDict)
        return;
    
    DynamicResource dr;
    dr.name = DynamicResource_THORNE_TEXTURE;
    if (ccDict->objectForKey("thorneVersion") && ccDict->objectForKey("thorneMd5Android") && ccDict->objectForKey("thorneMd5Ios")) {
        std::string version = ccDict->valueForKey("thorneVersion")->getCString();
        if(!version.empty()){
            if(version.compare("0.0.0") == 0){
                return;
            }
            dr.newV = version;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string code = ccDict->valueForKey("thorneMd5Android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        std::string code = ccDict->valueForKey("thorneMd5Ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        mServerQueue.push_back(dr);
        checkThorneResource();
    }
}

bool DynamicResourceController::checkThorneResource(){
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end()) {
        if((*iter).name.compare(DynamicResource_THORNE_TEXTURE) == 0){
            return hasVersionTypeResource(DynamicResource_THORNE_TEXTURE,(*iter).newV,1024,(*iter).md5);
        }
        ++iter;
    }
    return false;
}

void DynamicResourceController::initServerFightInfo(CCDictionary* ccDict){
    if(!ccDict)
        return;
    
    DynamicResource dr;
    dr.name = DynamicResource_SERVERFIGHT_TEXTURE;
    if (ccDict->objectForKey("ckf_mapVersion") && ccDict->objectForKey("ckf_mapMd5Android") && ccDict->objectForKey("ckf_mapMd5Ios")) {
        std::string version = ccDict->valueForKey("ckf_mapVersion")->getCString();
        if(!version.empty()){
            if(version.compare("0.0.0") == 0){
                return;
            }
            dr.newV = version;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string code = ccDict->valueForKey("ckf_mapMd5Android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        std::string code = ccDict->valueForKey("ckf_mapMd5Ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        mServerQueue.push_back(dr);
        checkServerFightResource();
    }
}

bool DynamicResourceController::checkServerFightResource(){
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end()) {
        if((*iter).name.compare(DynamicResource_SERVERFIGHT_TEXTURE) == 0){
            return hasVersionTypeResource(DynamicResource_SERVERFIGHT_TEXTURE,(*iter).newV,1024,(*iter).md5);
        }
        ++iter;
    }
    return false;
}

void DynamicResourceController::initDragonMiniMapInfo(CCDictionary* ccDict){
    if(!ccDict)
        return;
    
    DynamicResource dr;
    dr.name = DynamicResource_DRAGON_MINI_MAP_TEXTURE;
    if (ccDict->objectForKey("DragonMinimapVersion") && ccDict->objectForKey("DragonMinimapMd5Android") && ccDict->objectForKey("DragonMinimapMd5Ios")) {
        std::string version = ccDict->valueForKey("DragonMinimapVersion")->getCString();
        if(!version.empty()){
            if(version.compare("0.0.0") == 0){
                return;
            }
            dr.newV = version;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string code = ccDict->valueForKey("DragonMinimapMd5Android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        std::string code = ccDict->valueForKey("DragonMinimapMd5Ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        mServerQueue.push_back(dr);
        checkDragonMiniMapResource();
    }
}

bool DynamicResourceController::checkDragonMiniMapResource(){
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end()) {
        if((*iter).name.compare(DynamicResource_DRAGON_MINI_MAP_TEXTURE) == 0){
            return hasVersionTypeResource(DynamicResource_DRAGON_MINI_MAP_TEXTURE,(*iter).newV,1024,(*iter).md5);
        }
        ++iter;
    }
    return false;
}

void DynamicResourceController::initActivityAdInfo(CCDictionary* ccDict){
    if(!ccDict)
        return;
    
    DynamicResource dr;
    dr.name = DynamicResource_ACTIVITY_AD_TEXTURE;
    if (ccDict->objectForKey("activity_adVersion") && ccDict->objectForKey("activity_adMd5Android") && ccDict->objectForKey("activity_adMd5Ios")) {
        std::string version = ccDict->valueForKey("activity_adVersion")->getCString();
        if(!version.empty()){
            if(version.compare("0.0.0") == 0){
                return;
            }
            dr.newV = version;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string code = ccDict->valueForKey("activity_adMd5Android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        std::string code = ccDict->valueForKey("activity_adMd5Ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        mServerQueue.push_back(dr);
        checkActivityAdResource();
    }
}

bool DynamicResourceController::checkActivityAdResource(){
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end()) {
        if((*iter).name.compare(DynamicResource_ACTIVITY_AD_TEXTURE) == 0){
            return hasVersionTypeResource(DynamicResource_ACTIVITY_AD_TEXTURE,(*iter).newV,1024,(*iter).md5);
        }
        ++iter;
    }
    return false;
}

void DynamicResourceController::initActivityResInfo(__Dictionary* ccDict)
{
    if(!ccDict)
        return;
    
    DynamicResource dr;
    dr.name = DynamicResource_ACTIVITY_RES;
    if (ccDict->objectForKey("activity_resVersion") && ccDict->objectForKey("activity_resMd5Android") && ccDict->objectForKey("activity_resMd5Ios")) {
        std::string version = ccDict->valueForKey("activity_resVersion")->getCString();
        if(!version.empty()){
            if(version.compare("0.0.0") == 0){
                return;
            }
            dr.newV = version;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string code = ccDict->valueForKey("activity_resMd5Android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        std::string code = ccDict->valueForKey("activity_resMd5Ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        mServerQueue.push_back(dr);
        checkActivityResource();
    }
}

bool DynamicResourceController::checkActivityResource()
{
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end()) {
        if((*iter).name.compare(DynamicResource_ACTIVITY_RES) == 0){
            return hasVersionTypeResource(DynamicResource_ACTIVITY_RES,(*iter).newV,1024,(*iter).md5);
        }
        ++iter;
    }
    return false;
}

void DynamicResourceController::initMinimapViewInfo(CCDictionary* ccDict){
    if(!ccDict)
        return;
    
    DynamicResource dr;
    dr.name = DynamicResource_MINIMAPVIEW_TEXTURE;
    if (ccDict->objectForKey("miniMapView_version") && ccDict->objectForKey("miniMapView_md5_android") && ccDict->objectForKey("miniMapView_md5_ios")) {
        std::string version = ccDict->valueForKey("miniMapView_version")->getCString();
        if(!version.empty()){
            if(version.compare("0.0.0") == 0){
                return;
            }
            dr.newV = version;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string code = ccDict->valueForKey("miniMapView_md5_android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        std::string code = ccDict->valueForKey("miniMapView_md5_ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        mServerQueue.push_back(dr);
        checkMinimapViewResource();
    }
}

bool DynamicResourceController::checkMinimapViewResource(){
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end()) {
        if((*iter).name.compare(DynamicResource_MINIMAPVIEW_TEXTURE) == 0){
            return hasVersionTypeResource(DynamicResource_MINIMAPVIEW_TEXTURE,(*iter).newV,1024,(*iter).md5);
        }
        ++iter;
    }
    return false;
}

// lzy
void DynamicResourceController::initHDCCBInfo(CCDictionary* ccDict){
    if(!ccDict)
        return;
    
    DynamicResource dr;
    dr.name = DynamicResource_HD_CCB;
    if (ccDict->objectForKey("HDCCBVersion")) {
        std::string version = ccDict->valueForKey("HDCCBVersion")->getCString();
        if(!version.empty()){
            if(version.compare("0.0.0") == 0){
                return;
            }
            dr.newV = version;
        }
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string code = ccDict->valueForKey("HDCCBMd5Android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        std::string code = ccDict->valueForKey("HDCCBMd5Ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        mServerQueue.push_back(dr);
        checkHDCCB();
    }
}

bool DynamicResourceController::checkHDCCB(){
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end()) {
        if((*iter).name.compare(DynamicResource_HD_CCB) == 0){
            return hasVersionTypeResource(DynamicResource_HD_CCB,(*iter).newV,1024,(*iter).md5);
        }
        ++iter;
    }
    
    
    return false;
}

void DynamicResourceController::initHDResourceInfo(CCDictionary* ccDict){
    if(!ccDict)
        return;
    
    DynamicResource dr;
    dr.name = DynamicResource_HD_RES;
    if (ccDict->objectForKey("HDResVersion")) {
        std::string version = ccDict->valueForKey("HDResVersion")->getCString();
        if(!version.empty()){
            if(version.compare("0.0.0") == 0){
                return;
            }
            dr.newV = version;
        }
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string code = ccDict->valueForKey("HDResMd5Android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        std::string code = ccDict->valueForKey("HDResMd5Ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        
        
        
        mServerQueue.push_back(dr);
        checkHDResource();
    }
}

bool DynamicResourceController::checkHDResource(){
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end()) {
        if((*iter).name.compare(DynamicResource_HD_RES) == 0){
            return hasVersionTypeResource(DynamicResource_HD_RES,(*iter).newV,1024,(*iter).md5);
        }
        ++iter;
    }
    
    
    return false;
}

void DynamicResourceController::initUIRESInfo(CCDictionary* dic)
{
    if (!dic) {
        return;
    }
    
    DynamicResource dr;
    dr.name = DynamicResource_UI_RES;
    if (dic->objectForKey("uiresVersion"))
    {
        string version = dic->valueForKey("uiresVersion")->getCString();
        if (!version.empty()) {
            if (version.compare("0.0.0") == 0)
                return;
            dr.newV = version;
        } else {
            return;
        }
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        string code = dic->valueForKey("uiresMd5Android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        string code = dic->valueForKey("uiresMd5Ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        
        mServerQueue.push_back(dr);
        checkUIRES();
    }
}

bool DynamicResourceController::checkUIRES()
{
    auto it = mServerQueue.begin();
    while (it != mServerQueue.end()) {
        if ((*it).name.compare(DynamicResource_UI_RES) == 0) {
            return hasVersionTypeResource(DynamicResource_UI_RES, (*it).newV, 1024, (*it).md5);
        }
        ++it;
    }
    return false;
}

void DynamicResourceController::initEquipmentInfo(__Dictionary* dic)
{
    if (!dic) {
        return;
    }
    
    DynamicResource dr;
    dr.name = DynamicResource_EQUIPMENT_TEXTURE;
    if (dic->objectForKey("equipmentVersion"))
    {
        string version = dic->valueForKey("equipmentVersion")->getCString();
        if (!version.empty()) {
            if (version.compare("0.0.0") == 0)
                return;
            dr.newV = version;
        } else {
            return;
        }
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        string code = dic->valueForKey("equipmentMd5Android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        string code = dic->valueForKey("equipmentMd5Ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        
        mServerQueue.push_back(dr);
        checkEquipmentResource();
    }
}

bool DynamicResourceController::checkEquipmentResource()
{
    auto it = mServerQueue.begin();
    while (it != mServerQueue.end()) {
        if ((*it).name.compare(DynamicResource_EQUIPMENT_TEXTURE) == 0) {
            return hasVersionTypeResource(DynamicResource_EQUIPMENT_TEXTURE, (*it).newV, 1024, (*it).md5);
        }
        ++it;
    }
    return false;
}

void DynamicResourceController::initDragonBuildingInfo(CCDictionary* ccDict){
    if(!ccDict)
        return;
    
    DynamicResource dr;
    dr.name = DynamicResource_DRAGON_BUILDING;
    if (ccDict->objectForKey("dragon_buildingVersion") && ccDict->objectForKey("dragon_buildingMd5Android") && ccDict->objectForKey("dragon_buildingMd5Ios")) {
        std::string version = ccDict->valueForKey("dragon_buildingVersion")->getCString();
        if(!version.empty()){
            if(version.compare("0.0.0") == 0){
                return;
            }
            dr.newV = version;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string code = ccDict->valueForKey("dragon_buildingMd5Android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        std::string code = ccDict->valueForKey("dragon_buildingMd5Ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        mServerQueue.push_back(dr);
        checkDragonBuildingResource();
    }
}

bool DynamicResourceController::checkDragonBuildingResource(){
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end()) {
        if((*iter).name.compare(DynamicResource_DRAGON_BUILDING) == 0){
            return hasVersionTypeResource(DynamicResource_DRAGON_BUILDING,(*iter).newV,1024,(*iter).md5);
        }
        ++iter;
    }
    return false;
}

void DynamicResourceController::initDragonMapInfo(CCDictionary* ccDict){
    if(!ccDict)
        return;
    
    DynamicResource dr;
    dr.name = DynamicResource_DRAGON_MAP;
    if (ccDict->objectForKey("dragon_mapVersion") && ccDict->objectForKey("dragon_mapMd5Android") && ccDict->objectForKey("dragon_mapMd5Ios")) {
        std::string version = ccDict->valueForKey("dragon_mapVersion")->getCString();
        if(!version.empty()){
            if(version.compare("0.0.0") == 0){
                return;
            }
            dr.newV = version;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string code = ccDict->valueForKey("dragon_mapMd5Android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        std::string code = ccDict->valueForKey("dragon_mapMd5Ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        mServerQueue.push_back(dr);
        checkDragonMapResource();
    }
}

bool DynamicResourceController::checkDragonMapResource(){
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end()) {
        if((*iter).name.compare(DynamicResource_DRAGON_MAP) == 0){
            return hasVersionTypeResource(DynamicResource_DRAGON_MAP,(*iter).newV,1024,(*iter).md5);
        }
        ++iter;
    }
    return false;
}


// 科技图动态加载
void DynamicResourceController::initScienceInfo(cocos2d::CCDictionary *ccDict){
    if(!ccDict)
        return;
    
    DynamicResource dr;
    dr.name = DynamicResource_SCIENCE_TEXTURE;
    if (ccDict->objectForKey("scienceVersion") && ccDict->objectForKey("scienceMd5Android") && ccDict->objectForKey("scienceMd5Ios")) {
        std::string version = ccDict->valueForKey("scienceVersion")->getCString();
        if(!version.empty()){
            if(version.compare("0.0.0") == 0){
                return;
            }
            dr.newV = version;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string code = ccDict->valueForKey("scienceMd5Android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        std::string code = ccDict->valueForKey("scienceMd5Ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        mServerQueue.push_back(dr);
        checkScienceResource();
    }
}

bool DynamicResourceController::checkScienceResource(){
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end()) {
        if((*iter).name.compare(DynamicResource_SCIENCE_TEXTURE) == 0){
            return hasVersionTypeResource(DynamicResource_SCIENCE_TEXTURE,(*iter).newV,1024,(*iter).md5);
        }
        ++iter;
    }
    return false;
}


void DynamicResourceController::initMonsterLayerInfo(CCDictionary* ccDict){
    if(!ccDict)
        return;
    
    DynamicResource dr;
    dr.name = DynamicResource_MONSTER_TEXTURE_layer;
    if (ccDict->objectForKey("monsterLayerVersion") && ccDict->objectForKey("monsterLayerMd5Android") && ccDict->objectForKey("monsterLayerMd5Ios")) {
        std::string version = ccDict->valueForKey("monsterLayerVersion")->getCString();
        if(!version.empty()){
            if(version.compare("0.0.0") == 0){
                return;
            }
            dr.newV = version;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string code = ccDict->valueForKey("monsterLayerMd5Android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        std::string code = ccDict->valueForKey("monsterLayerMd5Ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        mServerQueue.push_back(dr);
        checkMonsterLayerRes();
    }
}

bool DynamicResourceController::checkMonsterLayerRes(){
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end()) {
        if((*iter).name.compare(DynamicResource_MONSTER_TEXTURE_layer) == 0){
            return hasVersionTypeResource(DynamicResource_MONSTER_TEXTURE_layer,(*iter).newV,1024,(*iter).md5);
        }
        ++iter;
    }
    return false;
}
void DynamicResourceController::initHeadIconInfo(CCDictionary* ccDict){
    if(!ccDict)
        return;
    
    DynamicResource dr;
    dr.name = DynamicResource_HEAD_TEXTURE_ICON;
    if (ccDict->objectForKey("HeadIconVersion") && ccDict->objectForKey("HeadIconMd5Android") && ccDict->objectForKey("HeadIconMd5Ios")) {
        std::string version = ccDict->valueForKey("HeadIconVersion")->getCString();
        if(!version.empty()){
            if(version.compare("0.0.0") == 0){
                return;
            }
            dr.newV = version;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string code = ccDict->valueForKey("HeadIconMd5Android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        std::string code = ccDict->valueForKey("HeadIconMd5Ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        mServerQueue.push_back(dr);
        checkHeadIconRes();
    }
}
bool DynamicResourceController::checkHeadIconRes(){
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end())
    {
        if((*iter).name.compare(DynamicResource_HEAD_TEXTURE_ICON )== 0){
            return hasVersionTypeResource(DynamicResource_HEAD_TEXTURE_ICON,(*iter).newV,1024,(*iter).md5);
        }
        ++iter;
    }
    return false;
}

void DynamicResourceController::initHeadIcon_bustInfo(CCDictionary* ccDict){
    if(!ccDict)
        return;
    
    DynamicResource dr;
    dr.name = DynamicResource_HEAD_TEXTURE_ICON_BUST;
    if (ccDict->objectForKey("HeadIcon_bustVersion") && ccDict->objectForKey("HeadIcon_bustMd5Android") && ccDict->objectForKey("HeadIcon_bustMd5Ios")) {
        std::string version = ccDict->valueForKey("HeadIcon_bustVersion")->getCString();
        if(!version.empty()){
            if(version.compare("0.0.0") == 0){
                return;
            }
            dr.newV = version;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string code = ccDict->valueForKey("HeadIcon_bustMd5Android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        std::string code = ccDict->valueForKey("HeadIcon_bustMd5Ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        mServerQueue.push_back(dr);
        checkHeadIcon_bustRes();
    }
}
bool DynamicResourceController::checkHeadIcon_bustRes(){
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end())
    {
        if((*iter).name.compare(DynamicResource_HEAD_TEXTURE_ICON_BUST )== 0){
            return hasVersionTypeResource(DynamicResource_HEAD_TEXTURE_ICON_BUST,(*iter).newV,1024,(*iter).md5);
        }
        ++iter;
    }
    return false;
}

void DynamicResourceController::initHeadIcon_middleInfo(CCDictionary* ccDict)
{
    if(!ccDict)
        return;
    
    DynamicResource dr;
    dr.name = DynamicResource_HEAD_TEXTURE_ICON_MIDDLE;
    if (ccDict->objectForKey("HeadIcon_middleVersion") && ccDict->objectForKey("HeadIcon_middleMd5Android") && ccDict->objectForKey("HeadIcon_middleMd5Ios")) {
        std::string version = ccDict->valueForKey("HeadIcon_middleVersion")->getCString();
        if(!version.empty()){
            if(version.compare("0.0.0") == 0){
                return;
            }
            dr.newV = version;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string code = ccDict->valueForKey("HeadIcon_middleMd5Android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        std::string code = ccDict->valueForKey("HeadIcon_middleMd5Ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        mServerQueue.push_back(dr);
        checkHeadIcon_middleRes();
    }
}
bool DynamicResourceController::checkHeadIcon_middleRes(){
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end())
    {
        if((*iter).name.compare(DynamicResource_HEAD_TEXTURE_ICON_MIDDLE )== 0){
            return hasVersionTypeResource(DynamicResource_HEAD_TEXTURE_ICON_MIDDLE,(*iter).newV,1024,(*iter).md5);
        }
        ++iter;
    }
    return false;
}


#endif


