//
//  LotteryActCommand.cpp
//  IF
//
//  Created by chenfubi on 15/1/23.
//
//

#include "LotteryActCommand.h"
#include "GCMRewardController.h"
#include "ToolController.h"

bool LotteryActCommand::handleRecieve(CCDictionary* dict)
{
    if (dict->valueForKey("cmd")->compare(LOTTERY_START_COMMAND) != 0) {
        return false;
    }
    LotteryController::shared()->setSendCMD(false);
    CCDictionary* params = _dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0)
    {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
//        return true;
    }
    
    if (params->objectForKey("times1"))
    {
        LotteryController::shared()->lotteryInfo.lotteryCnt = params->valueForKey("times1")->intValue();
    }
    
    if (params->objectForKey("times2"))
    {
        LotteryController::shared()->lotteryInfo.lotteryCnt2 = params->valueForKey("times2")->intValue();
    }
    
    if (m_type == 1)
    {
        handleRecieve1(params);
    }
    else if (m_type == 2)
    {
        handleRecieve2(params);
    }
    
    return true;
}

bool LotteryActCommand::handleRecieve1(cocos2d::CCDictionary *dict)
{
    CCDictionary* resourceDic = _dict(dict->objectForKey("resource"));
    
    if (resourceDic->objectForKey("chip"))
    {
        GlobalData::shared()->resourceInfo.lChip = resourceDic->valueForKey("chip")->intValue();
    }
    if (resourceDic->objectForKey("gold"))
    {
        GlobalData::shared()->playerInfo.gold = resourceDic->valueForKey("gold")->intValue();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    }
    
    LotteryController::shared()->lotteryActCmdBack(dict);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(LOTTERYACTVIEWCMD, NetResult::createWithSuccess(dict));
    
    return true;
}

bool LotteryActCommand::handleRecieve2(cocos2d::CCDictionary *dict)
{
    CCDictionary* resourceDic = _dict(dict->objectForKey("resource"));
    if (resourceDic->objectForKey("diamond"))
    {
        GlobalData::shared()->resourceInfo.lDiamond = resourceDic->valueForKey("diamond")->intValue();
    }
    if (resourceDic->objectForKey("gold"))
    {
        GlobalData::shared()->playerInfo.gold = resourceDic->valueForKey("gold")->intValue();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    }
    dict->setObject(CCString::createWithFormat("%d", m_position), "position");
    LotteryController::shared()->lotteryAct2CmdBack(dict);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(LOTTERYACTVIEWCMD2, NetResult::createWithSuccess(dict));
    
    return true;
}
bool LotteryCanCelCmd::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(LOTTERY_CANCEL_COMMAND) != 0) {
        return false;
    }
    return true;
}
bool LotteryBuyCmd::handleRecieve(CCDictionary* dict)
{
    if (dict->valueForKey("cmd")->compare(LOTTERY_BUY_COMMAND) != 0) {
        return false;
    }
    
    CCDictionary* params = _dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0)
    {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
//        return true;
    }
    
    if (params->objectForKey("chip"))
    {
        GlobalData::shared()->resourceInfo.lChip = params->valueForKey("chip")->intValue();
    }
    
    if (params->objectForKey("gold"))
    {
        GlobalData::shared()->playerInfo.gold = params->valueForKey("gold")->intValue();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    }
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("BUYCHIPBACK", params);
    
    return true;
}

bool Lottery10TimesCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(LOTTERY_10TIMES_COMMAND) != 0) {
        return false;
    }
    LotteryController::shared()->setSendCMD(false);
    CCDictionary* params = _dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0)
    {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
        return false;
    }
    CCDictionary* resourceDic = _dict(params->objectForKey("resource"));
    if (resourceDic->objectForKey("chip"))
    {
        GlobalData::shared()->resourceInfo.lChip = resourceDic->valueForKey("chip")->intValue();
    }
    
    if (params->objectForKey("batchResult"))
    {
        CCArray* batchResult = dynamic_cast<CCArray*>(params->objectForKey("batchResult"));
        if(batchResult && batchResult->count()>0){
            for(int i=0;i<batchResult->count();i++){
                CCDictionary* iDict = dynamic_cast<CCDictionary*>(batchResult->objectAtIndex(i));
                if (iDict->valueForKey("type")->intValue()==1) {
                    continue;
                }
                CCDictionary* rewards = _dict(iDict->objectForKey("result"));
                CCArray* arr = CCArray::create();
                arr->addObject(rewards);
                GCMRewardController::getInstance()->retReward2(arr, false);
            }
        }
    }
    if (params->objectForKey("boxTimes"))
    {
        LotteryController::shared()->lotteryInfo.boxTimes = params->valueForKey("boxTimes")->intValue();
    }
//    LotteryController::shared()->lotteryActCmdBack(dict);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(LOTTERYACTVIEWCMD10, NetResult::createWithSuccess(params));
    
    return true;
}

bool LotteryUseChestCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(LOTTERY_USECHEST_COMMAND) != 0) {
        return false;
    }
    CCDictionary* params = _dict(dict->objectForKey("params"));
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0)
    {
        CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
        return false;
    }
    if (params->objectForKey("boxTimes")) {
        LotteryController::shared()->lotteryInfo.boxTimes = params->valueForKey("boxTimes")->intValue();
    }
    
    if (params->objectForKey("lottery")) {
        LotteryController::shared()->lotteryInfo.rewardSort.clear();
        string lottery = params->valueForKey("lottery")->getCString();
        if (lottery.length() > 0)
        {
            vector<string> lotVec;
            CCCommonUtils::splitString(lottery, "|", lotVec);
            vector<string> strVec;
            for (int i = 0; i < lotVec.size(); i++)
            {
                strVec.clear();
                CCCommonUtils::splitString(lotVec.at(i), ":", strVec);
                LotteryController::shared()->setRewardInfo(i, strVec);
            }
        }
        LotteryController::shared()->lotteryInfo.type = 2;
        LotteryController::shared()->lotteryInfo.hasResetReward = false;
        
        CCUserDefault::sharedUserDefault()->setStringForKey(LOTTERY_REWARD_INFO, lottery);
        CCUserDefault::sharedUserDefault()->setStringForKey(LotteryAct2ShowView_PREVIEW_REWARD, "");
        CCUserDefault::sharedUserDefault()->setStringForKey(LotteryAct2ShowView_PREVIEW_REWARD0, "");
        CCUserDefault::sharedUserDefault()->setStringForKey(LOTTERYACT2_PREVIEW_REWARD, "");
        CCUserDefault::sharedUserDefault()->flush();
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(LOTTERY_USE_CHEST);
    }
    return true;
}