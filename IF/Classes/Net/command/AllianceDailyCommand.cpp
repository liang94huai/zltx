//
//  AllianceDailyCommand.cpp
//  IF
//
//  Created by 张军 on 15/10/22.
//
//

#include "AllianceDailyCommand.h"
#include "AllianceDailyController.h"

bool AllianceDailyCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(DAILY_GET) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        AllianceDailyController::getInstance()->EndGetDailyQuests(params1);
        return true;
    }
    return false;
}

bool AllianceGetSendQuestsCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(DAILY_GET_SEND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        AllianceDailyController::getInstance()->endGetSendQuests(params1);
        return true;
    }
    return false;
}
//刷新待发任务
bool AllianceRefreshSendQuestsCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(DAILY_REFRESH_SEND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        AllianceDailyController::getInstance()->endRefreshSendQuests(params1);
        return true;
    }
    return false;
}

bool AllianceSendQuestsCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(DAILY_SEND_QUEST) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        AllianceDailyController::getInstance()->endSendQuests(params1);
        return true;
    }
    return false;
}

//获取奖励详情
bool AllianceGetDetailCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(DAILY_GET_DETAIL) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        AllianceDailyController::getInstance()->endGetRewardDetail(params1);
        return true;
    }
    return false;
}

void PushAllianceDailyPublishCommand::handleResponse(CCDictionary *dict)
{
    if (!dict) return;
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    AllianceDailyController::getInstance()->addOneDailyQuest(params);
    
}
void PushAllianceDailyAcceptCommand::handleResponse(CCDictionary *dict)
{
    if (!dict) return;
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    AllianceDailyController::getInstance()->updateOneDailyQuest(params);
}

//领任务
bool AllianceAcceptQuestCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(DAILY_ACCEPT_QUEST) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        AllianceDailyController::getInstance()->endAcceptQuest(params1);
        return true;
    }
    return false;
}

//领奖
bool AllianceDailyGetRewardCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(DAILY_GET_REWARD) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        AllianceDailyController::getInstance()->endGetReward(params1);
        return true;
    }
    return false;
}

//加速
bool AllianceDailySpeedUpCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(DAILY_SPEEDUP) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        AllianceDailyController::getInstance()->endSpeedUp(params1);
        return true;
    }
    return false;
}

//获取记录信息
bool AllianceDailyRecordCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(DAILY_RECORD) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        AllianceDailyController::getInstance()->endGetRecord(params1);
        return true;
    }
    return false;
}

//获取自动刷新信息
bool AllianceDailyFreshCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(DAILY_AUTO_FRESH) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        AllianceDailyController::getInstance()->endGetfreshInfo(params1);
        return true;
    }
    return false;
}





