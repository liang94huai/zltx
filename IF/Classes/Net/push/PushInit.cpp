//
//  PushInit.cpp
//  IF
//
//  Created by 邹 程 on 13-9-13.
//
//

#include "PushInit.h"
#include "CommonInclude.h"
#include "YesNoDialog.h"
#include "CCThreadManager.h"
#include "NetController.h"

void PushInit::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;

    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_LoginInit);

    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (params && !GlobalData::shared()->isInitFlag) {
        DataParse *parse = new DataParse();
        parse->parseObject(params);
    }
}

void DataParse::parseObject(cocos2d::CCDictionary *dict){
    m_dict = dict;
    m_dict->retain();
    startIndex = CCCommonUtils::InitStart;
    CCCommonUtils::resetData(dict);
    
    update(0.0f);
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void DataParse::update(float time){
    if(startIndex > CCCommonUtils::InitEnd){
        CCCommonUtils::initOperationOver();
        killSelf();
    }else{
        CCCommonUtils::initOperation(startIndex, m_dict);
        startIndex++;
    }
}

void DataParse::killSelf(CCObject *obj){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    m_dict->release();
    this->release();
}

void PushUserOff::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDirector::sharedDirector()->pause();

    YesNoDialog::showVariableTitle(_lang("E100083").c_str(),CCCallFuncO::create(GameController::getInstance(), callfuncO_selector(GameController::quitGame), NULL),_lang("confirm").c_str(),true);
}

void PushUserKick::handleResponse(cocos2d::CCDictionary *dict){
    if (!dict)
        return;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    std::string str = _lang("E100087").c_str();
    if (!params) {
        cocos2d::CCDirector::sharedDirector()->pause();
        YesNoDialog::showVariableTitle(_lang(str).c_str(),CCCallFuncO::create(GameController::getInstance(), callfuncO_selector(GameController::quitGame), NULL),_lang("confirm").c_str(),true);
        return;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    
    bool isEmulatorBan = false;
    if (pStr->compare("")!=0) {
        str = pStr->getCString();
        isEmulatorBan = true;
    }
    cocos2d::CCDirector::sharedDirector()->pause();
    if(isEmulatorBan)
    {
        NetController::shared()->setNeedRelogin(false);
        auto _dialog = YesNoDialog::show(_lang(str).c_str()
                                         , CCCallFuncO::create(GameController::getInstance(), callfuncO_selector(GameController::quitGame),NULL)
                                         , 0
                                         , true
                                         , CCCallFunc::create(GameController::getInstance(),callfunc_selector(GameController::feedBack))
                                         );
        _dialog->showCancelButton();
        _dialog->setYesButtonTitle(_lang("confirm").c_str());
        _dialog->setNoButtonTitle(_lang("E100075").c_str());
    }
    else
    {
        YesNoDialog::showVariableTitle(_lang(str).c_str(),CCCallFuncO::create(GameController::getInstance(), callfuncO_selector(GameController::quitGame), NULL),_lang("confirm").c_str(),true);
    }
    

    
}



void PushUserGmOff::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict) return;
    cocos2d::CCDirector::sharedDirector()->pause();
    
    YesNoDialog::showVariableTitle(_lang("E100048").c_str(),CCCallFuncO::create(GameController::getInstance(), callfuncO_selector(GameController::quitGame), NULL),_lang("105248").c_str(),true);
}

void PushInitError::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_LoginInitError);
}

void PushInitBan::handleResponse(cocos2d::CCDictionary *dict)
{
    if (!dict)
        return;
    auto params = _dict(dict->objectForKey("params"));
    if(!params)
    {
        return;
    }
    string str=_lang("E100090");
    string name="";
    string level="";
    string serverId="";
    string errorCode="";
    int banTime=0;
    if(params->objectForKey("name"))
    {
        name = params->valueForKey("name")->getCString();
    }
    if(params->objectForKey("level"))
    {
        level = params->valueForKey("level")->getCString();
    }
    if(params->objectForKey("serverId"))
    {
        serverId = params->valueForKey("serverId")->getCString();
    }
    if(params->objectForKey("errorCode"))
    {
        errorCode = params->valueForKey("errorCode")->getCString();
    }
    if(params->objectForKey("banTime"))
    {
        int startTimeStamp = params->valueForKey("banTime")->intValue();
        int timeStamp = params->valueForKey("db_timezone_offset")->intValue();
        banTime =startTimeStamp - timeStamp;
    }
//    if(banTime>0)
//    {
//        str.append("\n").append(_lang_1("E100214",CC_SECTOA(banTime)));
//    }
 str.append("\n").append(_lang_1("E100198",name.c_str())).append("\n").append(_lang_1("E100208",serverId.c_str())).append("\n");
    str.append(_lang_1("E100209",level.c_str())).append("\n").append(_lang_1("E100210",_lang(errorCode.c_str()).c_str()));
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_LoginInitBan);
    if (banTime > 0) {
        GameController::getInstance()->showBanTimeAlert(str,banTime);
    }
    else
        GameController::getInstance()->showBanAlert(str);
}