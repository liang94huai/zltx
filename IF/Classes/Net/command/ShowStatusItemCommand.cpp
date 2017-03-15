//
//  ShowStatusItemCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-4-21.
//
//

#include "ShowStatusItemCommand.h"
#include "NetController.h"
#include "ToolController.h"
#include "WorldController.h"

ShowStatusItemCommand::ShowStatusItemCommand()
: CommandBase(SHOW_STATUS_ITEM)
{
}

bool ShowStatusItemCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(SHOW_STATUS_ITEM) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
    }else{
        ToolController::getInstance()->m_statusItems.clear();
        CCArray* arr =  (CCArray*)params->objectForKey("statusItems");
        int num = arr->count();
        for (int i=0; i<num; i++) {
            CCDictionary* dic = (CCDictionary*)arr->objectAtIndex(i);
            dic->retain();
            int type = dic->valueForKey("type2")->intValue();
            ToolController::getInstance()->m_statusItems[type] = dic;  
        }
        callSuccess(NetResult::create());
    }
    return true;
}
bool CancelStatusItemCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(CANCEL_STATUS_ITEM) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
    }else{
        auto iter = GlobalData::shared()->statusMap.find(m_statusId);
        if(iter!=GlobalData::shared()->statusMap.end()){
            GlobalData::shared()->statusMap.erase(iter);
        }
        auto iter1 = ToolController::getInstance()->m_statusItems.find(100);
        if(iter1!=ToolController::getInstance()->m_statusItems.end()){
            ToolController::getInstance()->m_statusItems.erase(iter1);
        }
        ToolController::getInstance()->resetMaincityCustom(0);
        callSuccess(NetResult::create());
    }
    return true;
}

bool SkinUseCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(SKIN_USE_CMD) != 0)
        return false;

    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
    }else{
        ToolController::getInstance()->retUseSkin(params);
    }
    return true;
}

void StatusPush::handleResponse(__Dictionary* dict)
{
    if (!dict)
    {
        return;
    }
    __Dictionary* params = _dict(dict->objectForKey("params"));

    if (params->objectForKey("oldStatus")) {//删除该状态的作用
        int reStatusId = params->valueForKey("oldStatus")->intValue();
        if (GlobalData::shared()->statusMap.find(reStatusId) != GlobalData::shared()->statusMap.end()) {
            GlobalData::shared()->statusMap[reStatusId] = 0;
        }
    }
//    int tempTime = 0;
    if (params->objectForKey("effectState")) {
        auto stateDict = _dict(params->objectForKey("effectState"));
        CCDictElement* element;
        CCDICT_FOREACH(stateDict, element)
        {
            string key = element->getStrKey();
//            int effectId = atoi(key.c_str());

            double time = stateDict->valueForKey(key)->doubleValue()/1000;
            if (time>0) {
                time = GlobalData::shared()->changeTime(time);
            }
            if (key!="startTime") {
                GlobalData::shared()->statusMap[atoi(key.c_str())] = -1;//time;
//                tempTime = time;
            }
            map<int, CCDictionary* >::iterator it = ToolController::getInstance()->m_statusItems.find(100);
            CCObject* obj = element->getObject();
            CCString* str = (CCString*)obj;

            if(it!=ToolController::getInstance()->m_statusItems.end()){
                auto dic = it->second;
                if(!dic->objectForKey("startTime")){
                    dic->setObject(CCString::create(CC_ITOA(WorldController::getInstance()->getTime())), "startTime");
                }else if(key!="" && key!="startTime"){
                    dic->setObject(CCString::create(str->getCString()), "endTime");
                }else if(key=="startTime"){
                    dic->setObject(CCString::create(str->getCString()), "startTime");
                }
                ToolController::getInstance()->m_statusItems[100] = dic;
            }else{
                auto infoDic = CCDictionary::create();
                infoDic->retain();
                if(key!="" && key!="startTime"){
                    infoDic->setObject(CCString::create(str->getCString()), "endTime");
                }else if(key=="startTime"){
                    infoDic->setObject(CCString::create(str->getCString()), "startTime");
                }
                ToolController::getInstance()->m_statusItems[100] = infoDic;
            }
            

            ToolController::getInstance()->checkMaincityCustom();

        }
    }

    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ITME_STATUS_TIME_CHANGE,CCInteger::create(100));

}
