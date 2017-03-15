//
//  ScoutInfo.cpp
//  IF
//
//  Created by 邹 程 on 14-3-5.
//
//

#include "ScoutInfo.h"
#include "WorldCommand.h"
#include "WorldMapView.h"
#include "YesNoDialog.h"
#include "MailReadCommand.h"
#include "DetectMailPopUpView.h"
#include "ChatServiceCocos2dx.h"
#include "IFCommonUtils.hpp"
bool ScoutInfo::init() {

    
    
    BaseTileInfo::init();
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(4, true);
    CCLoadSprite::doResourceByCommonIndex(502, true);
    CCLoadSprite::doResourceByCommonIndex(504, true);
    auto ccb = CCBLoadFile("ScoutInfo",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else
        this->setContentSize(ccb->getContentSize());
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(4, false);
        CCLoadSprite::doResourceByCommonIndex(504, false);
        CCLoadSprite::doResourceByCommonIndex(502, false);
        CCLoadSprite::doResourceByCommonIndex(4, false);
    });
    
    //    int passTime = WorldController::getInstance()->getTime()/1000 - m_cityInfo.beDetected_Time;
    //    if(WorldController::getInstance()->isDetectFlag == 1 && passTime < (60*WorldController::getInstance()->scoutTime) ){
    //        m_mailUid = m_cityInfo.beDetected_mailUid;
    //    }
    //    else{
    //        m_mailUid = "";
    //    }
    //    auto info  = GlobalData::shared()->mailList.find(m_mailUid);
    //    auto tempInfo = (*info).second;
    //    if (GlobalData::shared()->mailList.find(m_mailUid) == GlobalData::shared()->mailList.end()) {
    //        m_mailUid = "";
    //    }
//    CCLOG("scoutLog playerName===== %s",m_cityInfo.playerName.c_str());
//    CCLOG("scoutLog flag=====%d",WorldController::getInstance()->isDetectFlag);
    auto tempIt = GlobalData::shared()->m_detectMailMap.begin();
    while (tempIt != GlobalData::shared()->m_detectMailMap.end()) {
        string name =(*tempIt).first;
        string uid = (*tempIt).second.first;
        int createTime = (*tempIt).second.second;
//        CCLOG("whilelog=====name: %s,uid:  %s,createtime  %d",name.c_str(),uid.c_str(),createTime);
        tempIt++;
    }
    
    if(GlobalData::shared()->m_detectMailMap.find(m_cityInfo.playerName) != GlobalData::shared()->m_detectMailMap.end() && WorldController::getInstance()->isDetectFlag == 1 )
    {
        int scoutTime = GlobalData::shared()->m_detectMailMap[m_cityInfo.playerName].second;
        int timeNow =  WorldController::getInstance()->getTime() / 1000;
        int modelCanShowTime = WorldController::getInstance()->scoutTime;
//        CCLOG("socutLog=====scoutTime: %d,nowTime:  %d,modelCanshowTime  %d",scoutTime,timeNow,modelCanShowTime);
        if(60 * modelCanShowTime > timeNow - scoutTime){
            m_mailUid = GlobalData::shared()->m_detectMailMap[m_cityInfo.playerName].first;
//            CCLOG("scoutLog mailUid===== %s",m_mailUid.c_str());
        }
    }

    CCLOG("scoutLog mailUid+++++ %s",m_mailUid.c_str());
    
    //    map<string,MailInfo*>::iterator tempIt =    GlobalData::shared()->mailList.begin();
    //
    //    while (tempIt != GlobalData::shared()->mailList.end()) {
    //        if (tempIt->second->type == MAIL_DETECT_REPORT) {
    //                string uid =    (*tempIt).second->uid;
    //                CCLOG("logwdz==========%s",uid.c_str());
    //                //                PopupViewController::getInstance()->addPopupInViewWithAnim(MailReadPopUpView::create(*(tempIt->second)), true);
    //        }
    //        tempIt++;
    //    }
    
    
    initMailInfo();
    m_costFood = 1000; //default value
    //m_subNode->setVisible(false);
    m_titleNew->setString(_lang("108539"));
//    m_title->setString(_lang("108539"));
    //setFavoButtonVisible(false);
//    setButtonCount(1);
//    setButtonName(1, "");
//    setButtonCallback(1, cccontrol_selector(ScoutInfo::onClickButtonScout));
    //    m_owner->setString(CCString::createWithFormat("%s %s",_lang("108549").c_str(),m_cityInfo.playerName.c_str())->getCString());
    m_owner->setString(_lang("108563"));
    m_buttonName->setString(_lang("108539"));
    m_country->setString("");
    m_information->setString(_lang("108738"));
    m_tilePoint->setString(_lang_2("108502",CC_ITOA((int)m_cityInfo.cityTilePoint.x),CC_ITOA((int)m_cityInfo.cityTilePoint.y)));
    m_hideNode->setVisible(false);
    if (m_cityInfo.cityType == Tile_allianceArea) {
        if (m_cityInfo.m_allianceAreaOwnerInfo.ownerName == "") {
            std::string nameStr = "(";
            nameStr.append(m_cityInfo.m_aArea_simpleName);
            nameStr.append(")");
            nameStr.append(m_cityInfo.m_aArea_nickname);
            m_name->setString(nameStr.c_str());
        }
        else {
            std::string nameStr = "";
            if(m_cityInfo.m_allianceAreaOwnerInfo.allianceAbbr != ""){
                nameStr = "(";
                nameStr.append(m_cityInfo.m_allianceAreaOwnerInfo.allianceAbbr);
                nameStr.append(")");
                nameStr.append(m_cityInfo.m_allianceAreaOwnerInfo.ownerName);
            }
            else
                nameStr = m_cityInfo.m_allianceAreaOwnerInfo.ownerName;
            m_name->setString(nameStr.c_str());
        }
    }
    else if (m_cityInfo.cityType == tile_banner) {
        if (m_cityInfo.m_allianceAreaOwnerInfo.ownerName == "") {
            std::string nameStr = "(";
            nameStr.append(m_cityInfo.m_aArea_simpleName);
            nameStr.append(")");
            nameStr.append(m_cityInfo.m_aArea_nickname);
            m_name->setString(nameStr.c_str());
        }
        else {
            std::string nameStr = "";
            if(m_cityInfo.m_allianceAreaOwnerInfo.allianceAbbr != ""){
                nameStr = "(";
                nameStr.append(m_cityInfo.m_allianceAreaOwnerInfo.allianceAbbr);
                nameStr.append(")");
                nameStr.append(m_cityInfo.m_allianceAreaOwnerInfo.ownerName);
            }
            else
                nameStr = m_cityInfo.m_allianceAreaOwnerInfo.ownerName;
            m_name->setString(nameStr.c_str());
        }
    }
    else {
        //对方名字
        std::string nameStr = m_cityInfo.playerName;
        if(m_cityInfo.leagueAsn != ""){
            nameStr = std::string("(") + m_cityInfo.leagueAsn + ")" + m_cityInfo.playerName;
        }
        m_name->setString(nameStr.c_str());
    }
    
    m_scoutTime->setString(_lang("137425")+":");
    m_information->setVisible(false);
    
    unsigned int index = m_cityInfo.cityIndex;
    if(m_cityInfo.parentCityIndex != -1){
        index = m_cityInfo.parentCityIndex;
    }
    auto cmd = new WorldScoutDetailCommand(index);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ScoutInfo::onDetailCallback), NULL));
    cmd->sendAndRelease();
    
    return true;
}
void ScoutInfo::initMailInfo(){
    int passTime = getLastTime();
//    CCLOG("logPassTime=====%d",passTime);
    if(!m_mailUid.empty() && passTime < WorldController::getInstance()->scoutTime * 60 ){
//        CCLOGFUNC("show!");
        m_mailNode->setVisible(true);
        m_mailBtnLb->setString(_lang("108804"));
        onGameTick(0);
        m_scoutNode->setPositionY(m_scoutNode->getPositionY() + 50);
        //m_subNode->setPositionY(m_subNode->getPositionY() + 50);
        schedule(schedule_selector(ScoutInfo::onGameTick), 1.0);
    }
    else{
//        CCLOGFUNC("hide!");
        m_mailNode->setVisible(false);
    }
    
}
int ScoutInfo::getLastTime(){
    int timeNow = WorldController::getInstance()->getTime() / 1000;
//    int scoutTime =  m_cityInfo.beDetected_Time;
    int scoutTime =  0;
    
    if(GlobalData::shared()->m_detectMailMap.find(m_cityInfo.playerName) != GlobalData::shared()->m_detectMailMap.end()){
        scoutTime = GlobalData::shared()->m_detectMailMap[m_cityInfo.playerName].second;
    }
    int lastTime = timeNow - scoutTime;
//    CCLOGFUNCF("timeNow: %d",timeNow);
//    CCLOGFUNCF("scoutTime: %d",scoutTime);
    return lastTime;
}
void ScoutInfo::onGameTick(float Time){
    int lastTime = getLastTime();
    if (lastTime < 0) {
//        CCLOGFUNC("");
        unschedule(schedule_selector(ScoutInfo::onGameTick));
    }else if (lastTime < 60){
        m_lastScoutTimeLb->setString(_lang_1("108798", CC_ITOA(lastTime)));
    }else{
//        m_lastScoutTimeLb->setString(_lang_1("108798", CC_ITOA(lastTime)));//todowdz
        m_lastScoutTimeLb->setString(_lang_1("108799", CC_ITOA(lastTime / 60)));
    }
    
}
bool ScoutInfo::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hideNode", CCNode*, this->m_hideNode)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_country", CCLabelIF*, this->m_country)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_owner", CCLabelIF*, this->m_owner)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tilePoint", CCLabelIF*, this->m_tilePoint)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cost", CCLabelIF*, this->m_cost)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_information", CCLabelIF*, this->m_information)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_time", CCLabelIF*, this->m_time)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buttonName", CCLabelIF*, this->m_buttonName);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picContainer", CCNode*, this->m_picContainer)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scoutTime", CCLabelIF*, this->m_scoutTime)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name", Label*, this->m_name)
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleNew", CCLabelIF*, this->m_titleNew)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnScout", CCControlButton*, this->m_btnScout)

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scoutNode", CCNode*, this->m_scoutNode)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailNode", CCNode*, this->m_mailNode)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailBtnLb", CCLabelIF*, this->m_mailBtnLb)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lastScoutTimeLb", CCLabelIF*, this->m_lastScoutTimeLb)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tileBg", CCScale9Sprite*, this->m_tileBg)
    
    
    return BaseTileInfo::onAssignCCBMemberVariable(pTarget, pMemberVariableName, pNode);
}

SEL_CCControlHandler ScoutInfo::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickButtonScout", ScoutInfo::onClickButtonScout);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onMialBtnCilck", ScoutInfo::onMialBtnCilck);
    return BaseTileInfo::onResolveCCBCCControlSelector(pTarget, pSelectorName);
}

void ScoutInfo::onDetailCallback(cocos2d::CCObject *obj) {
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        return;
    }
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    m_costTime = info->valueForKey("scoutTime")->intValue();
    m_costFood = info->valueForKey("scoutCost")->intValue();
    m_costTime /= 1000;
    //对方头像
    std::string picStr="";
    if (m_cityInfo.cityType == Tile_allianceArea) {
        if (m_cityInfo.m_allianceAreaOwnerInfo.ownerName == "") {
            picStr = "territory_building_lv0";
        }
        else {
            if (info->valueForKey("pic")->getCString()!="") {
                picStr=info->valueForKey("pic")->getCString();
            }
            if (picStr=="") {
                picStr="g026";
            }
        }
    }
    else if (m_cityInfo.cityType == tile_banner) {
        if (m_cityInfo.m_allianceAreaOwnerInfo.ownerName == "") {
            picStr = "territory_tower0";
        }
        else {
            if (info->valueForKey("pic")->getCString()!="") {
                picStr=info->valueForKey("pic")->getCString();
            }
            if (picStr=="") {
                picStr="g026";
            }
        }
    }
    else {
        if (info->valueForKey("pic")->getCString()!="") {
            picStr=info->valueForKey("pic")->getCString();
        }
        if (picStr=="") {
            picStr="g026";
        }
    }
    if(IFCommonUtils::shouldForceChangePic(picStr)){
        picStr = "g044";
    }
    if(m_cityInfo.cityType>=Crystal && m_cityInfo.cityType<=TransferPoint){
        picStr = CCString::createWithFormat("dragon_b%d",m_cityInfo.cityType)->getCString();
    }
    
    auto picSpr = CCLoadSprite::createSprite((picStr + ".png").c_str(),true,CCLoadSpriteType_HEAD_ICON);
    this->m_picContainer->addChild(picSpr);
    CCCommonUtils::setSpriteMaxSize(picSpr, 90);//100
    
    // update view
    m_hideNode->setVisible(true);
    m_cost->setString(CC_ITOA((int)m_costFood));
    m_time->setString(CCCommonUtils::timeLeftToCountDown(m_costTime).c_str());
    
}

void ScoutInfo::onMialBtnCilck(CCObject * pSender, Control::EventType pCCControlEvent) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(!m_mailUid.empty()){
       MailController::getInstance()->getDetectMailByMailUid(m_mailUid);
       this->closeThis();
    }
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(!m_mailUid.empty()){
        ChatServiceCocos2dx::popUpNewMail(m_mailUid);
        this->closeThis();
    }
#endif
    
//    MailReadCommand* cmd = new MailReadCommand(m_mailUid, CC_ITOA(MAIL_DETECT_REPORT));
//    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(ScoutInfo::readMailSuccess), NULL));
//    cmd->sendAndRelease();
//    GameController::getInstance()->showWaitInterface();
}
void ScoutInfo::readMailSuccess(CCObject* param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    if(result==NULL) return;
    auto dictInfo = _dict(result->getData());
    CCArray* arr = NULL;
    if(dictInfo && dictInfo->objectForKey("result")){
        arr = dynamic_cast<CCArray*>(dictInfo->objectForKey("result"));
    }
    if(arr && arr->count()>0){
        auto dic = _dict(arr->objectAtIndex(0));
        MailInfo* m_mailInfo = MailInfo::create();
        m_mailInfo->uid = m_mailUid;
        m_mailInfo->type = MAIL_DETECT_REPORT;
        if(dic->objectForKey("contents")){
//            string infoStr = dic->valueForKey("contents")->getCString();
//            if (infoStr == "114005") {
//                m_mailInfo->pointType = MAIL_DETECT_REPORT_PROTECT;
//            }
        }
        
        m_mailInfo->fromName = _lang("105522");
        m_mailInfo->isReadContent = true;
        m_mailInfo->detectReport = dic;
        m_mailInfo->detectReport->retain();
        CCString *str = dynamic_cast<CCString*>(dic->objectForKey("pointType"));
        if(str && str->getCString()){
            m_mailInfo->pointType =  str->intValue();
        }
        m_mailInfo->isFromChat = true;
        m_mailInfo->createTime = dic->valueForKey("createTime")->doubleValue() / 1000;
        m_mailInfo->retain();
        this->closeThis();
        PopupViewController::getInstance()->addPopupInViewWithAnim(DetectMailPopUpView::create(m_mailInfo));
    }else{
        CCCommonUtils::flyHint("", "", _lang("115299"));
    }
}
void ScoutInfo::onClickButtonScout(CCObject * pSender, Control::EventType pCCControlEvent) {
    auto button = dynamic_cast<CCControlButton*>(pSender);
    if (button) {
        button->setEnabled(false);
    }
    auto food = GlobalData::shared()->resourceInfo.totalFood();
    unsigned int current = WorldController::getInstance()->getCurrentMarchCount();
    unsigned int max = WorldController::getInstance()->getMaxMarchCount();
    if (current >= max) {
        WorldController::getInstance()->showMarchAlert(max);
    } else if (food < m_costFood) {
        CCCommonUtils::flyText(_lang("108654"));
    } else {
        unsigned int index = m_cityInfo.cityIndex;
        if(m_cityInfo.parentCityIndex != -1){
            index = m_cityInfo.parentCityIndex;
        }
        auto cmd = new WorldMarchCommand(index,MethodScout);
        cmd->setCallback(CCCallFuncO::create(WorldMapView::instance(), callfuncO_selector(WorldMapView::onMarchCallback), NULL));
        cmd->sendAndRelease();
//        GlobalData::shared()->resourceInfo.lFood -= m_costFood;
        GlobalData::shared()->resourceInfo.reduceFood(m_costFood);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    }
    
    closeThis();
}
