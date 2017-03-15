//
//  ToolCommand.cpp
//  IF
//
//  Created by fubin on 14-3-4.
//
//

#include "ToolCommand.h"
#include "ToolController.h"
#include "ToolInfo.h"
#include "ChatServiceCocos2dx.h"

bool ToolBuyCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TOOL_BUY_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        ToolController::getInstance()->retBuyTool(params1,m_buyAndUse,m_num);
        return true;
    }else if (dict->valueForKey("cmd")->compare(LONG_BUY_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        ToolController::getInstance()->retBuyLongJingEquip(params1,m_buyAndUse,m_num);
        return true;
    }
    return false;
}

bool ToolUseCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TOOL_USE_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        ToolController::getInstance()->retUseTool(params1);
        return true;
    }
    return false;
}

bool ToolHotGetCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TOOL_HOT_GET_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        ToolController::getInstance()->retHotItems(params1);
        if(!params1->objectForKey("errorCode")){
            callSuccess(NetResult::createWithSuccess(params1));
        }
        return true;
    }
    return false;
}

bool ToolHotBuyCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TOOL_HOT_BUY_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        ToolController::getInstance()->retBuyTool(params1);
        return true;
    }
    return false;
}

bool ToolMerchantGetCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TOOL_MERCHANT_GET_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        if(!params1->objectForKey("errorCode")){
            ToolController::getInstance()->retMerchantItems(params1);
            if (params1->objectForKey("remainGold")) {
                GlobalData::shared()->playerInfo.gold = params1->valueForKey("remainGold")->doubleValue();
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
            }
            callSuccess(NetResult::createWithSuccess(params1));
        }
        return true;
    }
    return false;
}
bool ToolMerchantRefreshCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TOOL_MERCHANT_REFRESH_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        if(!params1->objectForKey("errorCode")){
            ToolController::getInstance()->retMerchantItems(params1);
            if (params1->objectForKey("remainGold")) {
                GlobalData::shared()->playerInfo.gold = params1->valueForKey("remainGold")->doubleValue();
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
            }
            callSuccess(NetResult::createWithSuccess(params1));
        }
        return true;
    }
    return false;
}
bool ToolMerchantBuyCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TOOL_MERCHANT_BUY_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        if(!params1->objectForKey("errorCode")){
            if (params1->objectForKey("remainGold")) {
                GlobalData::shared()->playerInfo.gold = params1->valueForKey("remainGold")->doubleValue();
//                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
            }
            if (params1->objectForKey("remainCrystal")) {
                GlobalData::shared()->playerInfo.longJing = params1->valueForKey("remainCrystal")->doubleValue();
            }
            if (params1->objectForKey("resource")) {
                auto resInfo = _dict(params1->objectForKey("resource"));
                GlobalData::shared()->resourceInfo.setResourceData(resInfo);
            }
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
            ToolController::getInstance()->retBuyMerchantTool(params1);
        }
        return true;
    }
    return false;
}

bool ToolCreateCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TOOL_CREATE_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        ToolController::getInstance()->retMakeTool(params1);
        return true;
    }
    return false;
}

bool ToolHavestCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TOOL_HAVEST_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        ToolController::getInstance()->retGetMakeTool(params1);
        return true;
    }
    return false;
}
//添加建造材料空位
bool ToolQueueAddCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TOOL_QUEUE_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        ToolController::getInstance()->endAddMakeToolQueue(params1);
        return true;
    }
    return false;
}

//取消建造材料
bool ToolQueueCancelCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TOOL_QUEUE_CANCEL_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        ToolController::getInstance()->endCancelMakeTool(params1);
        return true;
    }
    return false;
}

bool ItemComposeCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TOOL_ITEM_COMPOSE) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        //ToolController::getInstance()->retBuyTool(params1);
        if(!params1->objectForKey("errorCode")){
            ////////////////
            ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
            /////////////////
            string param1 = info.para1;
            string param2 = info.para2;
            string param3 = info.para3;
            string uuid = params1->valueForKey("uuid")->getCString();
            ToolInfo& infotaget  = ToolController::getInstance()->getToolInfoById(std::atoi((param1).c_str()));
            ToolController::getInstance()->addTool(infotaget.itemId,infotaget.getCNT()+m_perNum,uuid);
            //--------------------------------
            vector<std::string> pVector1;
            vector<std::string> pVector2;
            if (param2.find('|') != string::npos) {
                CCCommonUtils::splitString(param2, "|", pVector1);
                CCCommonUtils::splitString(param3, "|", pVector2);
            }
            else {
                CCCommonUtils::splitString(param2, ";", pVector1);
                CCCommonUtils::splitString(param3, ";", pVector2);
                int pos = find(pVector1.begin(), pVector1.end(), CC_ITOA(m_itemId))-pVector1.begin();
                pVector1.clear();
                pVector1.push_back(CC_ITOA(m_itemId));
                string strnum = pVector2[pos];
                pVector2.clear();
                pVector2.push_back(strnum);
            }
            //--------------------------------
            vector<std::string>:: iterator pVect2 = pVector2.begin();
            vector<std::string>:: iterator pVect1 = pVector1.begin();
            for (; pVect2 != pVector2.end(); pVect1++,pVect2++) {
                ToolInfo& info2 = ToolController::getInstance()->getToolInfoById(std::atoi((*pVect1).c_str()));
                int a = info2.getCNT();
                int b =std::atoi((*pVect2).c_str())*m_perNum;
                info2.setCNT(a - b);
            }
 
            ///////////////////
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFREASH_TOOL_DATA);
            callSuccess(NetResult::createWithSuccess(params1));
        }
        return true;
    }
    return false;
}

#pragma mark 红包
bool RedPacketsSendCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TOOL_REDPACKET_SEND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        ToolController::getInstance()->endSendRedPacket(params1);
        return true;
    }
    return false;
}

bool RedPacketsGetCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TOOL_REDPACKET_GET) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        string redPackage = m_uid;
        redPackage.append("_").append(m_serverId);
        ToolController::getInstance()->endGetRedPacket(params1,redPackage);
        return true;
    }
    return false;
}

bool RedPacketUidsGetCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TOOL_REDPACKET_GET_IDS) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        if (params1->objectForKey("errorCode")) {
            const CCString *pStr = params1->valueForKey("errorCode");
            CCCommonUtils::flyText(_lang(pStr->getCString()));
            return false;
        }
        else
        {
            Array* uidArray = dynamic_cast<Array*>(params1->objectForKey("records"));
            if(uidArray && uidArray->count()>0)
            {
                string uids = "";
                for (int i=0; i<uidArray->count(); i++) {
                    Dictionary* redDic = dynamic_cast<Dictionary*>(uidArray->objectAtIndex(i));
                    if (redDic) {
                        string redPackageStr = "";
                        if (redDic->objectForKey("uid")) {
                            redPackageStr.append(redDic->valueForKey("uid")->getCString());
                        }
                        redPackageStr.append("_");
                        if (redDic->objectForKey("server")) {
                            redPackageStr.append(redDic->valueForKey("server")->getCString());
                        }
                        redPackageStr.append("|");
                        if (redDic->objectForKey("status")) {
                            CCLOGFUNCF("status:%s",redDic->valueForKey("status")->getCString());
                            redPackageStr.append(redDic->valueForKey("status")->getCString());
                        }
                        if(redPackageStr!="")
                        {
                            if(uids!="")
                                uids.append(",");
                            uids.append(redPackageStr);
                        }
                    }
                }
                if(uids!="")
                {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                    ChatServiceCocos2dx::postRedPackageGotUids(uids);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                    ChatServiceCocos2dx::postRedPackageGotUids(uids);
#endif
                }
            }

            return true;
        }
    }
    return false;
}

bool GetRedPacketStatusCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TOOL_REDPACKET_GET_STATUS) == 0)
    {
        CCDictionary *params=_dict(dict->objectForKey("params"));
        if (params->objectForKey("errorCode")) {
            const CCString *pStr = params->valueForKey("errorCode");
            CCCommonUtils::flyText(_lang(pStr->getCString()));
            return false;
        }
        else
        {
            if(params->objectForKey("status"))
            {
                int status = params->valueForKey("status")->intValue();
                CCLOGFUNCF("status:%d",status);
                string redStatusStr = m_redPackageUid.append("_").append(m_serverId);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                ChatServiceCocos2dx::postRedPackageStatus(redStatusStr,status);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                ChatServiceCocos2dx::postRedPackageStatus(redStatusStr,status);
#endif
            }
            return true;
        }
    }
    return false;
}

bool TrimVipPointCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(TOOL_VIPX_SCORE) == 0)
    {
//        CCDictionary *params=_dict(dict->objectForKey("params"));
//        if (params->objectForKey("errorCode")) {
////            const CCString *pStr = params->valueForKey("errorCode");
////            CCCommonUtils::flyText(_lang(pStr->getCString()));
//            return false;
//        }
//        else
//        {
            ToolController::getInstance()->endTrimVipPoint(dict);
            return true;
//        }
    }
    return false;
}

bool EquipBuyCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(EQUIP_BUY_COMMAND) == 0)
    {
        CCDictionary *params1=_dict(dict->objectForKey("params"));
        ToolController::getInstance()->retBuyEquip(params1);
        return true;
    }
    return false;
}