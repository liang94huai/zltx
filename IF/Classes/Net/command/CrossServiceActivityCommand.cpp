//
//  CrossServiceActivityCommand.cpp
//  IF
//
//  Created by ganxiaohua on 15/6/4.
//
//

#include "CrossServiceActivityCommand.h"
#include "SceneController.h"
#include "WorldMapView.h"
#include "NetController.h"
#include "LogoutCommand.h"
#include "FlyHint.h"
#include "ActivityController.h"

bool CrossServiceActivityCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(CROSS_SERVER_ACCESS) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    GameController::getInstance()->removeWaitInterface();
    if (pStr->compare("")!=0) {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
    }else{
        if(!params->objectForKey("serverInfo")){
            CCLog("ckf no cross server Info");
            return true;
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(REMOVE_ALL_HINT);
        if(SceneController::getInstance()->showBG){
            SceneController::getInstance()->showBG->release();
            SceneController::getInstance()->showBG = NULL;
        }
        if (WorldMapView::instance()) {
            WorldMapView::instance()->removeCover();
        }
        auto scene = CCDirector::sharedDirector()->getRunningScene();
        auto texture = CCRenderTexture::create(CCDirector::sharedDirector()->getWinSize().width, CCDirector::sharedDirector()->getWinSize().height);
        texture->beginWithClear(0, 0, 0, 1.0);
        scene->visit();
        texture->end();
        SceneController::getInstance()->showBG = texture;
        SceneController::getInstance()->showBG->retain();
        
        string originalGameUid = CCUserDefault::sharedUserDefault()->getStringForKey(GAME_UID,"");
        int originalPort = CCUserDefault::sharedUserDefault()->getIntegerForKey(ACCOUNT_PORT, 0);
        string originalServerIp = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_IP, "");
        string originalServerZone = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_ZONE, "");
        CCUserDefault::sharedUserDefault()->setStringForKey(ORIGINAL_ACCOUNT_IP, originalServerIp);
        CCUserDefault::sharedUserDefault()->setStringForKey(ORIGINAL_ACCOUNT_ZONE, originalServerZone);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ORIGINAL_ACCOUNT_ACCOUNT_PORT, originalPort);
        CCUserDefault::sharedUserDefault()->setStringForKey(ORIGINAL_ACCOUNT_GAME_UID, originalGameUid);
        CCUserDefault::sharedUserDefault()->flush();
        
        auto serverInfo = _dict(params->objectForKey("serverInfo"));
        std::string ip = serverInfo->valueForKey("ip")->getCString();
        std::string zone = serverInfo->valueForKey("zone")->getCString();
        int port = serverInfo->valueForKey("port")->intValue();
        std::string gameUid = params->valueForKey("uid")->getCString();
        int currentServerId = serverInfo->valueForKey("id")->intValue();
        GlobalData::shared()->playerInfo.currentServerId = currentServerId;
        
        CCLog("ckf ip=%s zone=%s port=%d currentServerId=%d",ip.c_str(),zone.c_str(),port,currentServerId);
        
        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, ip);
        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, zone);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT, port);
        CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, gameUid);
        CCUserDefault::sharedUserDefault()->flush();
        
        NetController::shared()->setIp(ip);
        NetController::shared()->setZone(zone);
        NetController::shared()->setPort(port);
        NetController::shared()->setIsLogout(true);
        NetController::shared()->disconnect();
        
        LogoutCommand* cmd = new LogoutCommand();
        CCDictionary* _params = CCDictionary::create();
        _params->setObject(CCString::create(zone), "zoneName");
        CCDictionary* dict = CCDictionary::create();
        dict->setObject(CCString::create("logout"), "cmd");
        dict->setObject(_params, "params");
        cmd->handleRecieve(dict);
        if(!GlobalData::shared()->firstEnterCrossService){
            ToolInfo& info = ToolController::getInstance()->getToolInfoById(ActivityController::getInstance()->wb_condition_k2);
            if (info.getCNT()>0) {
                int num = info.getCNT() - 1;
                info.setCNT(num);
            }
        }
        GlobalData::shared()->firstEnterCrossService = false;

    }
    return true;
}
