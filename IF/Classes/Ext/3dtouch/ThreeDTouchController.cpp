//
//  ThreeDTouchController.cpp
//  IF
//
//  Created by liuzengyou on 14-1-21.
//
//

#include "ThreeDTouchController.h"
#include "WorldController.h"
#include "WorldCommand.h"
#include "GetUserInfoCommand.h"
#include "AchievementController.h"
#include "OCCppHelper.h"

static ThreeDTouchController* _instance = NULL;

ThreeDTouchController* ThreeDTouchController::getInstance() {
    if (!_instance) {
        _instance = new ThreeDTouchController();
    }
    return _instance;
}

void ThreeDTouchController::purge()
{

}

void ThreeDTouchController::openPlayerInfo(WorldCityInfo& cityInfo)
{
    if(cityInfo.playerName == GlobalData::shared()->playerInfo.name)
        return;
    
    GlobalData::shared()->peekPageType = peekType_playerInfo;
    
    int index = cityInfo.cityIndex;
    if(cityInfo.cityType == MonsterTile || cityInfo.cityType == CityTile){
        if(cityInfo.parentCityIndex != -1){
            index = cityInfo.parentCityIndex;
        }
    }
    
    auto cmd = new WorldDetailCommand(index, cityInfo.tileServerId);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ThreeDTouchController::onDetailCallback), nullptr));
    cmd->sendAndRelease();
    
}

void ThreeDTouchController::onDetailCallback(CCObject* pObj)
{
    auto ret = dynamic_cast<NetResult*>(pObj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        return;
    }
    
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    if (!info) {
        return;
    }
    std::string _playerUid = info->valueForKey("uid")->getCString();
    if(!_playerUid.empty())
    {
        sendGetUserInfoCommand(_playerUid);
    }
}

void ThreeDTouchController::sendGetUserInfoCommand(const std::string& player_uid)
{
    if (player_uid.empty())
    {
        return;
    }
    auto cmd = new GetUserInfoCommand(player_uid);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ThreeDTouchController::onGetPlayerInfoCallback), nullptr));
    cmd->sendAndRelease();
}


void ThreeDTouchController::onGetPlayerInfoCallback(cocos2d::CCObject *obj) {
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        return;
    }
    
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    if (!info) {
        return;
    }
    
    
    auto playerInfo = new PlayerInfo();
    playerInfo->updateInfo(info);
    
    if(info->objectForKey("alliance")){
        auto dict = dynamic_cast<CCDictionary*>(info->objectForKey("alliance"));
        if(dict->objectForKey("alliance")){
            auto alliance = dynamic_cast<CCDictionary*>(dict->objectForKey("alliance"));
            if(alliance){
                playerInfo->setAllianceId(alliance->valueForKey("uid")->getCString());
            }
            playerInfo->allianceInfo.updateAllianceInfo(alliance);
            playerInfo->allianceInfo.serverId = playerInfo->selfServerId;
        }
    }
    std::string m_medalIconId = info->valueForKey("medal")->getCString();
    float pro = info->valueForKey("achieveNum")->floatValue();
    std::string m_achievePro = CCString::createWithFormat("%.f", pro*100)->getCString();
    CCArray* medalList = dynamic_cast<CCArray*>(info->objectForKey("medalList"));
    std::string m_medalCntStr = "0";
    if (medalList)
    {
        int count = medalList->count();
        m_medalCntStr = CC_ITOA(count);
    }
    AchievementController::getInstance()->getOtherMedalInfo(medalList);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    OCCppHelper::share()->notificationUpdatePlayerInfo(playerInfo, m_medalIconId, m_achievePro, m_medalCntStr);
#endif
    
    playerInfo->release();
}