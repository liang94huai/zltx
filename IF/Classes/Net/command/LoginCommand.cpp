///
//  LoginCommand.cpp
//  IF
//
//  Created by zhaohf on 13-9-04.
//
//

#include "LoadingScene.h"
#include "LoginCommand.h"
#include "CommonInclude.h"
#include "../../Ext/CCDevice.h"
#include "NetController.h"
#include "YesNoDialog.h"
#include "../../view/guiExt/md5.h"
#include "GmController.h"
#include "LuaController.h"
#include "DynamicResourceController.h"
#include "AppLibHelper.h"

static int retryTime = 0;
static const std::string NEW_DEVICE_MD5_KEY = "B4FvQWnTp718vr6AHyvdGlrHBGNcvuM4y3jUeRCgXxIwBZIbt";

LoginCommand::LoginCommand(int loginType/*= 5*/,bool bIsResume /*= false*/)
: CommandBase("login"), m_bIsResume(bIsResume)
{
    m_startTime = CCCommonUtils::getLocalTime();
    string _gameUid = CCUserDefault::sharedUserDefault()->getStringForKey(GAME_UID, "");
    //新号
    if (_gameUid == "")
    {
        
        //国家
//        if(GlobalData::shared()->loginServerInfo.country != 0){
//            //选择国家
//            putParam("country", CCInteger::create(GlobalData::shared()->loginServerInfo.country));
//            //推荐国家
//            if(GlobalData::shared()->loginServerInfo.country==GlobalData::shared()->loginServerInfo.recommandCountry){
//                putParam("suggestCountry", CCBool::create(true));
//            }
//        }
        GlobalData::shared()->loginServerInfo.country = 1;
        putParam("country", CCInteger::create(GlobalData::shared()->loginServerInfo.country));
        putParam("suggestCountry", CCBool::create(false));
        
        //设备信息
        string _handSetInfo = cocos2d::extension::CCDevice::getHandSetInfo();
        putParam("mt", CCString::create(_handSetInfo));
        //分辨率
        CCSize screenSize = Director::getInstance()->getOpenGLView()->getFrameSize();
        putParam("phone_screen", CCString::createWithFormat("%f*%f",screenSize.width,screenSize.height));
        
        // 客户端类型 0 ios 1 android
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        putParam("platform", CCString::create("1"));
        putParam("gcmRegisterId", CCString::create(GlobalData::shared()->gcmRegisterId));
        putParam("referrer", CCString::create(GlobalData::shared()->referrer));
#else
        putParam("platform", CCString::create("0"));
#endif
    }
    std::string phoneDevice;
    std::string signCode;
    std::string subscriberId;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //CCUserDefault::sharedUserDefault()->setStringForKey("phoneDevice", SysUtilies::getDevicePlatformInfo());
#endif
    phoneDevice = CCUserDefault::sharedUserDefault()->getStringForKey("phoneDevice", "default");
    putParam("phoneDevice", CCString::create(phoneDevice));
    
    signCode = CCUserDefault::sharedUserDefault()->getStringForKey("theSignCode", "default");
    putParam("signCode", CCString::create(signCode));
//    CCLog("signCode::::::::%s",signCode.c_str());
    
    subscriberId = CCUserDefault::sharedUserDefault()->getStringForKey("subscriberId", "default");
    putParam("subscriberId", CCString::create(subscriberId));
//    CCLog("subscriberId::::::::%s",subscriberId.c_str());
    std::string buildcode= CCUserDefault::sharedUserDefault()->getStringForKey(BUILDCODE, "");
    putParam("buildCode", CCString::create(buildcode));
    
    //设备ID devil 用hoolai uid代替deviceid
//    string _deviceId = cocos2d::extension::CCDevice::getDeviceUid();
    string _deviceId = GlobalData::shared()->platformUserUid;
    putParam("deviceId", CCString::create(_deviceId));
    CCUserDefault::sharedUserDefault()->setStringForKey(DEVICE_UID, _deviceId);
    
    MD5 md5;
    string _time = m_data->valueForKey("cmdBaseTime")->getCString();
    string key = _time + "e01684719c1bec876c10bb124a6d65aa" + _gameUid;
    md5.update(key.c_str(), key.length());
    putParam("SecurityCode",CCString::create(md5.toString()));
    
    //传入设备ID或者GooglePlay Id  getAccountInfo
    string _googlePlay = cocos2d::extension::CCDevice::getAccountInfo();
    putParam("googlePlay", CCString::create(_googlePlay));
    
    // devil 关闭HD判断
//    if(CCCommonUtils::isIOSPad() || CCCommonUtils::isAdriodPadHD()) // lzy 判断是否是hd版本登录
//    {
//        putParam("isHDLogin", CCString::create("1"));
//    }
//    else
//    {
         putParam("isHDLogin", CCString::create("0"));
//    }
    
    putParam("pf", CCString::create(GlobalData::shared()->analyticID));
    putParam("pfId", CCString::create(GlobalData::shared()->platformUID));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    putParam("parseRegisterId", CCString::create(GlobalData::shared()->parseRegisterId));
#else
    putParam("parseRegisterId", CCString::create(cocos2d::extension::CCDevice::getParseInstallationId()));
#endif
    putParam("fromCountry", CCString::create(GlobalData::shared()->fromCountry));
//    _gameUid = "e5b123011c9a4d18a96c3f9f22441be7";
    putParam("gameUid", CCString::create(_gameUid));
    //uuid
    string _uuid = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_UUID,"");
    putParam("uuid", CCString::create(_uuid));
    
    //deviceId 变了以后的处理
    bool changeFlag = false;
    std::string deviceMd5Str = _deviceId + "to";

#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    string _openUUID = cocos2d::extension::CCDevice::getOpenUDID(false);
    if(_openUUID != _deviceId){
        putParam("newDeviceId", CCString::create(_openUUID));
        changeFlag = true;
        deviceMd5Str = deviceMd5Str + _openUUID;
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string _andriodOpenUid = cocos2d::extension::CCDevice::getCurrentAndroidUDID();
    CCString *android = CCString::createWithFormat("newDeviceId: %s, deviceId: %s", _deviceId.c_str(), _andriodOpenUid.c_str());
//    CCLog(android->getCString());
    
//    if(_deviceId != "" && _andriodOpenUid != "" && _andriodOpenUid != _deviceId && CCCommonUtils::isUserNewUid()){
//        putParam("newDeviceId", CCString::create(_deviceId));
//        putParam("deviceId", CCString::create(_andriodOpenUid));
//        changeFlag = true;
//        deviceMd5Str = deviceMd5Str + _andriodOpenUid;
//    }
#endif
    if(changeFlag){
        deviceMd5Str = deviceMd5Str + NEW_DEVICE_MD5_KEY;
        MD5 deviceMD5;
        deviceMD5.update(deviceMd5Str.c_str(), deviceMd5Str.length());
        putParam("updateDeviceSecurityCode",CCString::create(deviceMD5.toString()));
    }
    //前台版本
    putParam("appVersion", CCString::create(cocos2d::extension::CCDevice::getVersionName()));
    // 语言
    string language= CCCommonUtils::getLanguage();
    putParam("lang", CCString::create(language));
    //服ID
    string server_id = CCUserDefault::sharedUserDefault()->getStringForKey(SERVER_ID,"");
    putParam("serverId", CCString::create(server_id));
    //gm登陆
    putParam("gmUid", CCString::create(GmController::getInstance()->gmUid.c_str()));
    GmController::getInstance()->gmUid = "";
    putParam("gmLogin", CCInteger::create(GmController::getInstance()->isGmLogin));
    GmController::getInstance()->isGmLogin = 0;
    string packageName = CCUserDefault::sharedUserDefault()->getStringForKey("android_packageName","");
    putParam("packageName", CCString::create(packageName));
    // devil AppsFlyerUID 去掉此功能
    putParam("afUID", CCString::create(""));
//    //fb 数据来源，做一个记录
//    string appLink = CCUserDefault::sharedUserDefault()->getStringForKey("fbAPPLink","");
//    CCLog("appLink=%s",appLink.c_str());
//    if(appLink!=""){
//        int ftype = 1;
//        if(appLink=="request"){
//            ftype = 2;
//        }
//        putParam("fbType",CCInteger::create(ftype));
//        CCUserDefault::sharedUserDefault()->setStringForKey("fbAPPLink", "");
//        CCUserDefault::sharedUserDefault()->flush();
//    }
//    if(BranchController::getInstance()->m_branchData.find("telkomselpromotion") != BranchController::getInstance()->m_branchData.end()){
//        std::string telkomselpromotion = BranchController::getInstance()->m_branchData["telkomselpromotion"];
//        YesNoDialog::showYesDialog(telkomselpromotion.c_str());
//        if(telkomselpromotion != ""){
//            putParam("telkomselpromotion", CCString::create(telkomselpromotion));
//        }
//    }
}

bool LoginCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    CCLOG("LoginCommand::handleRecieve");
    if (dict->valueForKey("cmd")->compare(LOGIN_COMMAND) != 0)
        return false;

    
    CCLog("login success");
    CCCommonUtils::sendConnectTimeToServer(0, m_startTime);
    CCDictionary *params=_dict(dict->objectForKey("params"));

    if (params && params->objectForKey("errorMessage")) {
        string errorCode = params->valueForKey("errorMessage")->getCString();
        GameController::getInstance()->setLoadingLog("LoginCommand", CCString::createWithFormat("handleRecieve_%s", errorCode.c_str())->getCString());
        
        NetController::shared()->setIslogining(false);
        
        if (errorCode == "4") {
            // ban user
//            LocalController::shared()->init();
//            YesNoDialog::showYesDialog("Your account has been banned!",false,CCCallFuncO::create(GameController::getInstance(), callfuncO_selector(GameController::quitGame),NULL),true);
            
            auto _dialog = YesNoDialog::show(_lang("E100090").c_str()
                                             , CCCallFuncO::create(GameController::getInstance(), callfuncO_selector(GameController::quitGame),NULL)
                                             , 0
                                             , true
                                             , CCCallFunc::create(this, callfunc_selector(LoginCommand::feedBack))
                                             );
            _dialog->showCancelButton();
            _dialog->setYesButtonTitle(_lang("E100089").c_str());
            _dialog->setNoButtonTitle(_lang("E100075").c_str());

            return true;
        }
        else if (errorCode == "E001") {
            if(showRetryTimeAlert()){
                return true;
            }
            retryTime++;
            CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID,"");
            CCUserDefault::sharedUserDefault()->setStringForKey(BUILDCODE,"");
            CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID,"");
            CCUserDefault::sharedUserDefault()->flush();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_RELOGIN);
            return true;
        }
        else if (errorCode == "E002"){
            if(showRetryTimeAlert()){
                return true;
            }
            retryTime++;

//            string zone = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_ZONE,"");
//            int len = zone.length() - 3;
//            string str2 = zone.substr(3,len);
//            int srcServerId = atoi(str2.c_str());
//            if (srcServerId>CROSS_SERVER_START_INDEX) {
                CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID,"");
                CCUserDefault::sharedUserDefault()->setStringForKey(BUILDCODE,"");
                CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID,"");
                CCUserDefault::sharedUserDefault()->flush();
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_RELOGIN);
//            }else{
//                CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID,"");
//                CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID,"");
//                CCUserDefault::sharedUserDefault()->flush();
//                NetController::shared()->disconnect();
//                LogoutCommand* cmd = new LogoutCommand();
//                cmd->sendAndRelease();
//            }
            return true;
        }
        else if (errorCode == "E003"){
            string originalGameUid = CCUserDefault::sharedUserDefault()->getStringForKey(ORIGINAL_ACCOUNT_GAME_UID,"");
            int originalPort = CCUserDefault::sharedUserDefault()->getIntegerForKey(ORIGINAL_ACCOUNT_ACCOUNT_PORT, 9933);
            string originalServerIp = CCUserDefault::sharedUserDefault()->getStringForKey(ORIGINAL_ACCOUNT_IP, "");
            string originalServerZone = CCUserDefault::sharedUserDefault()->getStringForKey(ORIGINAL_ACCOUNT_ZONE, "");
            
            if(originalGameUid!="" && originalServerIp!="" && originalServerZone!=""){
                CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, originalServerIp);
                CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, originalServerZone);
                CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT, originalPort);
                CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, originalGameUid);
                NetController::shared()->disconnect();
                NetController::shared()->setIp(originalServerIp);
                NetController::shared()->setZone(originalServerZone);
                NetController::shared()->setPort(originalPort);
                
                CCUserDefault::sharedUserDefault()->setStringForKey(ORIGINAL_ACCOUNT_IP, "");
                CCUserDefault::sharedUserDefault()->setStringForKey(ORIGINAL_ACCOUNT_ZONE, "");
                CCUserDefault::sharedUserDefault()->setIntegerForKey(ORIGINAL_ACCOUNT_ACCOUNT_PORT, 9933);
                CCUserDefault::sharedUserDefault()->setStringForKey(ORIGINAL_ACCOUNT_GAME_UID, "");
            }else{
                CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID,"");
                CCUserDefault::sharedUserDefault()->setStringForKey(BUILDCODE,"");
                CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID,"");
            }
            CCUserDefault::sharedUserDefault()->flush();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_RELOGIN);
            return true;
        }
        else if (errorCode == "110093"){
            YesNoDialog::showYesDialog(_lang(errorCode).c_str());
//            CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID,"");
//            CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_UUID,"");
//            CCUserDefault::sharedUserDefault()->flush();
//            NetController::shared()->disconnect();
//            LogoutCommand* cmd = new LogoutCommand();
//            cmd->sendAndRelease();
            return true;
        }else if(errorCode == "E100094"){
            auto _dialog = YesNoDialog::show(_lang("E100094").c_str()
                                             , CCCallFuncO::create(GameController::getInstance(), callfuncO_selector(GameController::quitGame),NULL)
                                             , 0
                                             , true
                                             , CCCallFunc::create(this, callfunc_selector(LoginCommand::feedBack))
                                             );
            _dialog->showCancelButton();
            _dialog->setYesButtonTitle(_lang("E100089").c_str());
            _dialog->setNoButtonTitle(_lang("E100075").c_str());
            return true;
        }else if(errorCode == "E100190" || errorCode == "E100189" || errorCode == "E100191"){
            auto _dialog = YesNoDialog::show(_lang(errorCode).c_str()
                                             , CCCallFuncO::create(GameController::getInstance(), callfuncO_selector(GameController::quitGame),NULL)
                                             , 0
                                             , true
                                             , CCCallFunc::create(this, callfunc_selector(LoginCommand::feedBack))
                                             );
            _dialog->showCancelButton();
            _dialog->setYesButtonTitle(_lang("E100089").c_str());
            _dialog->setNoButtonTitle(_lang("E100075").c_str());
            
            return true;
        }
        else if(errorCode == "E100190" || errorCode == "E100189" || errorCode == "E100191"){
            auto _dialog = YesNoDialog::show(_lang(errorCode).c_str()
                                             , CCCallFuncO::create(GameController::getInstance(), callfuncO_selector(GameController::quitGame),NULL)
                                             , 0
                                             , true
                                             , CCCallFunc::create(this, callfunc_selector(LoginCommand::feedBack))
                                             );
            _dialog->showCancelButton();
            _dialog->setYesButtonTitle(_lang("E100089").c_str());
            _dialog->setNoButtonTitle(_lang("E100075").c_str());
            
            return true;
        }
        else if(errorCode == "114141")
        {
            auto _dialog = YesNoDialog::show(_lang(errorCode).c_str()
                                             , CCCallFuncO::create(GameController::getInstance(), callfuncO_selector(GameController::quitGame),NULL)
                                             , 0
                                             , true
                                             , CCCallFunc::create(this, callfunc_selector(LoginCommand::feedEmulatorBack))
                                             );
            _dialog->showCancelButton();
            _dialog->setYesButtonTitle(_lang("E100089").c_str());
            _dialog->setNoButtonTitle(_lang("E100075").c_str());
            return  true;
        }
        else
        {
            std::string errorStr = _lang(errorCode);
            if(errorStr != ""){
                auto _dialog = YesNoDialog::show(errorStr.c_str()
                                                 , CCCallFuncO::create(GameController::getInstance(), callfuncO_selector(GameController::quitGame),NULL)
                                                 , 0
                                                 , true
                                                 , CCCallFunc::create(this, callfunc_selector(LoginCommand::feedBack))
                                                 );
                _dialog->showCancelButton();
                _dialog->setYesButtonTitle(_lang("E100089").c_str());
                _dialog->setNoButtonTitle(_lang("E100075").c_str());
            }
            return  true;
        }

    }
    
    
    CCDictionary* data = NULL;
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    data = params;
//#else
//    data = _dict( params->objectForKey("data"));
//#endif
    //版本更新
    CCString* url = dynamic_cast<CCString*>(data->objectForKey("downloadurl"));
    if (url) {
        GlobalData::shared()->downloadUrl = data->valueForKey("downloadurl")->getCString();
        GlobalData::shared()->updateType = data->valueForKey("updateType")->intValue();
    } else {
        GlobalData::shared()->updateType = 0;
    }
    CCLog("downloadurl = %s",GlobalData::shared()->downloadUrl.c_str());
    CCLog("xmlVersion server = %s",data->valueForKey("xmlVersion")->getCString());
    CCLog("serverVersion = %s",data->valueForKey("serverVersion")->getCString());
    
    
    // xml version
    if (data->objectForKey("xmlVersion")) {
        GlobalData::shared()->xmlVersion = data->valueForKey("xmlVersion")->getCString();
    } else {
        GlobalData::shared()->xmlVersion = "";
    }
    //客户端版本号
    if (data->objectForKey("serverVersion")) {
        GlobalData::shared()->serverVersion = data->valueForKey("serverVersion")->getCString();
    } else {
        GlobalData::shared()->serverVersion = "";
    }
    //lua版本号
    if (data->objectForKey("luaVersion")) {
        std::string luaV = data->valueForKey("luaVersion")->getCString();
        if(!luaV.empty()){
            LuaController::getInstance()->luaVersion = luaV;
        }
    }
    //lua zip文件md5校验码
    if (data->objectForKey("luaCode")) {
       LuaController::getInstance()->luaMD5Code = data->valueForKey("luaCode")->getCString();
    }
    //lua zip包大小
    if (data->objectForKey("luazipSize")) {
        LuaController::getInstance()->luazipSize = data->valueForKey("luazipSize")->intValue();
    }
    //lua解压后文件大小
    if (data->objectForKey("luaSize")) {
        LuaController::getInstance()->luaSize = data->valueForKey("luaSize")->intValue();
    }
    LuaController::getInstance()->setTranslateMark();
    
    // devil 关闭动态资源
//    DynamicResourceController::getInstance()->InitServerRes(data);
    
    CCLog("xmlVersion = %s",GlobalData::shared()->xmlVersion.c_str());
    callSuccess(NetResult::create());
    
    
    // Zbm 打点LoginSucess
    CCCommonUtils::postEventLogToServer("Login_Sucess_1");
                                        
    // record login success
    NetController::shared()->setIsLogout(false);
    GameController::getInstance()->setLoadingLog("LoginCommand","LoginSucess");
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_LoginSucess);
    // devil 关闭好友邀请
//    BranchController::getInstance()->sendPormotionCmd();
    retryTime = 0;
    return true;
}

void LoginCommand::feedBack(){
    CCCommonUtils::showBanHelpShift();
}

void LoginCommand::feedEmulatorBack(){
    CCCommonUtils::showEmulatorBanHelpShift();
}
bool LoginCommand::showRetryTimeAlert(){
    
    
    
    int maxTime = 3;
    if(retryTime >= maxTime){
        auto _dialog = YesNoDialog::show(_lang("E100196").c_str()
                                         , CCCallFuncO::create(GameController::getInstance(), callfuncO_selector(GameController::quitGame),NULL)
                                         , 0
                                         , true
                                         , CCCallFunc::create(this, callfunc_selector(LoginCommand::feedBack))
                                         );
        _dialog->showCancelButton();
        _dialog->setYesButtonTitle(_lang("E100089").c_str());
        _dialog->setNoButtonTitle(_lang("E100075").c_str());

        return true;
    }
    return false;
}
