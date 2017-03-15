//
//  BuildingHospitalPopUpView.cpp
//  IF
//
//  Created by 李锐奇 on 14-3-6.
//
//

#include "BuildingHospitalPopUpView.h"
#include "PopupViewController.h"
#include "BuildMoreInfoView.h"
#include "FunBuildController.h"
#include "fireandcomman.h"
#include "ArmyController.h"
#include "YesNoDialog.h"
#include "TreatCommand.h"
#include "BuildMoreInfoView.h"
#include "QueueController.h"
#include "UseResToolView.h"
#include "TipsView.h"
#include "ConfirmDialogView.h"
#include "FireSoldierView.h"
#include "SoldierIconCell.hpp"

const float numPerRow = 1.0;
static string m_guideKey = "";
BuildingHospitalPopUpView *BuildingHospitalPopUpView::create(FunBuildInfo &info)
{
    BuildingHospitalPopUpView *ret = new BuildingHospitalPopUpView(info);
    if(ret && ret->init()){ret->autorelease();
    }
    else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool BuildingHospitalPopUpView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(8, true);
        CCLoadSprite::doResourceByCommonIndex(204, true);

        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(7, false);
            CCLoadSprite::doResourceByCommonIndex(8, false);
            CCLoadSprite::doResourceByCommonIndex(204, false);

        });
        setIsHDPanel(true);
        auto bg = CCBLoadFile("BuildingHospital", this, this);
        changeBGMaxHeight(m_buildBG);
        this->setContentSize(bg->getContentSize());
        string title = _lang(m_info.name)+" "+_lang_1("102272", CC_ITOA(m_info.level));
        if(FunBuildController::getInstance()->checkOpenUpstar(m_info.type)
           && FunBuildController::getInstance()->building_Miracle_open){
            if(m_info.starNum >= 1){
                title = _lang(m_info.name) +" "+ _lang("160001");
                title.append(" ");
                title.append(CC_ITOA(m_info.starNum));
            }
        }
        setTitleName(title.c_str());
        m_yesSprite->setVisible(false);

        int dh = getExtendHeight();
        this->m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width, m_listContainer->getContentSize().height + dh));
        this->m_downNode->setPositionY(this->m_downNode->getPositionY() - dh);
        CCSize temp = m_renderBG->getContentSize();
        temp.height = temp.height+dh;
        this->m_renderBG->setContentSize(temp);
        m_buildBG->setPositionY(m_buildBG->getPositionY()-dh/2);
        
        this->m_healthLabel->setString(_lang("102147"));
        this->m_healInstantBtnText->setString(_lang("102146").c_str());
        this->m_healInstantBtnText->setMaxScaleXByWidth(240);//防爆框
        this->m_selText->setString(_lang("102145"));
        this->m_selectLabel->setString(_lang("102145"));
        this->m_goldText->setString("0");
        this->m_noJnjureTxt->setString(_lang("102284"));
        this->m_noJnjureTxt->setVisible(false);
        
        m_data = CCArray::create();
        m_tabView = CCMultiColTableView::create(this, m_listContainer->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setMultiColTableViewDelegate(this);
        m_tabView->setTouchPriority(Touch_Popup);
        m_listContainer->addChild(m_tabView);
        
        m_orgTitleY = m_troopText->getPositionY();
        m_orgListY = m_listContainer->getPositionY();
        m_orgHeight = m_listContainer->getContentSize().height;
        
        m_woodTouch = false;
        m_foodTouch = false;
        m_ironTouch = false;
        m_stoneTouch = false;
        m_selectAll = false;
        int mlv = FunBuildController::getInstance()->getMainCityLv();
        if (mlv<FunBuildController::getInstance()->building_base_k3 && mlv<FunBuildController::getInstance()->building_base_k4) {
            m_woodNode->setPositionY(25);
            m_foodNode->setPositionY(25);
            m_ironNode->setVisible(false);
            m_ironIcon->setEnabled(false);
            m_stoneNode->setVisible(false);
            m_stoneIcon->setEnabled(false);
        }
        else if (mlv<FunBuildController::getInstance()->building_base_k3) {
            m_ironNode->setVisible(false);
            m_ironIcon->setEnabled(false);
            m_stoneNode->setPositionX(-179);
        }
        else if (mlv<FunBuildController::getInstance()->building_base_k4) {
            m_stoneNode->setVisible(false);
            m_stoneIcon->setEnabled(false);
        }
        int newBgHeight = m_buildBG->getContentSize().height;
        int BGcount = (newBgHeight-80)/100+1;
        for (int i=0; i<BGcount; i++) {
            auto pic = CCLoadSprite::createSprite("technology_09.png");
            pic->setAnchorPoint(ccp(0, 0));
            m_bgNode->addChild(pic);
            int dh = pic->getContentSize().height;
            pic->setPositionY(-i*100);
            if (CCCommonUtils::isIosAndroidPad())
            {
                pic->setScaleX(2.4);
            }
            
            
        }
        if (GlobalData::shared()->serverType==SERVER_BATTLE_FIELD) {
            m_tipTxt->setString(_lang("138081"));
        }else if(GlobalData::shared()->serverType==SERVER_DRAGON_BATTLE){
            m_tipTxt->setString(_lang("140210"));
        }
        m_tipTxt->setVisible(GlobalData::shared()->serverType==SERVER_BATTLE_FIELD || GlobalData::shared()->serverType==SERVER_DRAGON_BATTLE);
        m_needNode->setVisible(!m_tipTxt->isVisible());

        if(GlobalData::shared()->serverType==SERVER_DRAGON_BATTLE && GlobalData::shared()->playerInfo.useGoldCureCDTime> GlobalData::shared()->getWorldTime()){
            this->schedule(schedule_selector(BuildingHospitalPopUpView::updateTime), 1.0);
            updateTime(0);
        }
        ret = true;
    }
    return ret;
}

void BuildingHospitalPopUpView::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BuildingHospitalPopUpView::refreshView), ARMY_NUM_CHANGE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BuildingHospitalPopUpView::refreshTreatNum), TREAT_NUM_CHANGE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BuildingHospitalPopUpView::refreshView), MSG_QUEUE_REMOVE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BuildingHospitalPopUpView::refreshView), MSG_QUEUE_ADD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BuildingHospitalPopUpView::healthCallBack), GO_TO_HEALTHING, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BuildingHospitalPopUpView::resetTreatNum), "resetTreatNum", NULL);
    refreshView(NULL);
    m_selectAll = false;
    
}
void BuildingHospitalPopUpView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_QUEUE_REMOVE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_QUEUE_ADD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ARMY_NUM_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, TREAT_NUM_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GO_TO_HEALTHING);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "resetTreatNum");
    resetTreatNum();
    setTouchEnabled(false);
    CCNode::onExit();
}

void BuildingHospitalPopUpView::updateTime(float _time){
    double left = GlobalData::shared()->playerInfo.useGoldCureCDTime - GlobalData::shared()->getWorldTime();
    if(left<=0){
        this->unschedule(schedule_selector(BuildingHospitalPopUpView::updateTime));
        m_instantBtn->setEnabled(true);
        this->m_healInstantBtnText->setString(_lang("102146").c_str());
    }else{
        m_instantBtn->setEnabled(false);
        this->m_healInstantBtnText->setString(CC_SECTOA(left));
    }
}

bool BuildingHospitalPopUpView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (m_guideKey == "Hospital_guide" && m_listContainer && isTouchInside(m_listContainer, pTouch)) {
        return true;
    }
    else if (m_guideKey == "Hospital_guide1" && m_guideNode && isTouchInside(m_guideNode, pTouch)) {
        return true;
    }
    
    if (isTouchInside(m_woodTouchNode, pTouch) && m_woodTouch) {
        return true;
    }
    else if (isTouchInside(m_foodTouchNode, pTouch) && m_foodTouch) {
        return true;
    }
    else if (isTouchInside(m_ironTouchNode, pTouch) && m_ironTouch) {
        return true;
    }
    else if (isTouchInside(m_stoneTouchNode, pTouch) && m_stoneTouch) {
        return true;
    }
    return false;
}

void BuildingHospitalPopUpView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void BuildingHospitalPopUpView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (m_guideKey == "Hospital_guide" && m_listContainer && isTouchInside(m_listContainer, pTouch)) {
        m_guideKey = "";
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::create("Hospital_guide"));
    }
    else if (m_guideKey == "Hospital_guide1" && m_guideNode && isTouchInside(m_guideNode, pTouch)) {
        m_guideKey = "";
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::create("Hospital_guide1"));
    }
    
    if (isTouchInside(m_woodTouchNode, pTouch) && m_woodTouch) {
        PopupViewController::getInstance()->addPopupInView(UseResToolView::create(Wood));
    }
    else if (isTouchInside(m_foodTouchNode, pTouch) && m_foodTouch) {
        PopupViewController::getInstance()->addPopupInView(UseResToolView::create(Food));
    }
    else if (isTouchInside(m_ironTouchNode, pTouch) && m_ironTouch) {
        PopupViewController::getInstance()->addPopupInView(UseResToolView::create(Iron));
    }
    else if (isTouchInside(m_stoneTouchNode, pTouch) && m_stoneTouch) {
        PopupViewController::getInstance()->addPopupInView(UseResToolView::create(Stone));
    }
}
void BuildingHospitalPopUpView::resetTreatNum(CCObject* p){
    map<std::string, TreatInfo>::iterator it;
    for(it = GlobalData::shared()->treatList.begin(); it != GlobalData::shared()->treatList.end(); it++){
        it->second.prepareToTreat = 0;;
    }
    if (this->m_instantBtn) {
        this->m_instantBtn->setEnabled(false);
    }
    
    if(this->m_healthBtn) {
        this->m_healthBtn->setEnabled(false);
    }
    this->updateTime(0);
}

void BuildingHospitalPopUpView::healthCallBack(CCObject* p)
{
    PopupViewController::getInstance()->removePopupView(this);
    //PopupViewController::getInstance()->addPopupInView(HealthingView::create(m_info.itemId));
}

bool BuildingHospitalPopUpView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allBtn", CCControlButton*, this->m_allBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_instantBtn",CCControlButton*,this->m_instantBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_healthBtn",CCControlButton*,this->m_healthBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_selectBtn",CCControlButton*,this->m_selectBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_buildBG",CCScale9Sprite*,this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listContainer",CCNode*,this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_downNode",CCNode*,this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bgNode",CCNode*,this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_woodText",CCLabelIF*,this->m_woodText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_stoneText",CCLabelIF*,this->m_stoneText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_ironText",CCLabelIF*,this->m_ironText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_foodText",CCLabelIF*,this->m_foodText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_totalText",CCLabelIF*,this->m_totalText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_troopText",CCLabelIF*,this->m_troopText);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_healInstantBtnText",CCLabelIF*,this->m_healInstantBtnText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_goldText",CCLabelIF*,this->m_goldText);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_woodNode",CCNode*,this->m_woodNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_stoneNode",CCNode*,this->m_stoneNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_ironNode",CCNode*,this->m_ironNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_foodNode",CCNode*,this->m_foodNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_woodTouchNode",CCNode*,this->m_woodTouchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_stoneTouchNode",CCNode*,this->m_stoneTouchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_ironTouchNode",CCNode*,this->m_ironTouchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_foodTouchNode",CCNode*,this->m_foodTouchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_woodAddNode",CCNode*,this->m_woodAddNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_stoneAddNode",CCNode*,this->m_stoneAddNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_ironAddNode",CCNode*,this->m_ironAddNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_foodAddNode",CCNode*,this->m_foodAddNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_woodLabel",CCLabelIF*,this->m_woodLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_stoneLabel",CCLabelIF*,this->m_stoneLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_ironLabel",CCLabelIF*,this->m_ironLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_foodLabel",CCLabelIF*,this->m_foodLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_selText",CCLabelIF*,this->m_selText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_selectLabel",CCLabelIF*,this->m_selectLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_woodIcon",CCControlButton*,this->m_woodIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_foodIcon",CCControlButton*,this->m_foodIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_ironIcon",CCControlButton*,this->m_ironIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_stoneIcon",CCControlButton*,this->m_stoneIcon);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_yesSprite",CCSprite*,this->m_yesSprite);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_renderBG",CCScale9Sprite*,this->m_renderBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_healthLabel",CCLabelIF*,this->m_healthLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeText",CCLabelIF*,this->m_timeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_noJnjureTxt",CCLabelIF*,this->m_noJnjureTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_needNode",CCNode*,this->m_needNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipTxt",CCLabelIF*,this->m_tipTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_guideNode", Node*, m_guideNode);
    return false;
}

SEL_CCControlHandler BuildingHospitalPopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAllClick", BuildingHospitalPopUpView::onAllClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInstantClick", BuildingHospitalPopUpView::onInstantClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHealthClick", BuildingHospitalPopUpView::onHealthClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSelectClick", BuildingHospitalPopUpView::onSelectClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipBtnClick", BuildingHospitalPopUpView::onTipBtnClick);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickWoodIcon", BuildingHospitalPopUpView::onClickWoodIcon);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickFoodIcon", BuildingHospitalPopUpView::onClickFoodIcon);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickIronIcon", BuildingHospitalPopUpView::onClickIronIcon);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickStoneIcon", BuildingHospitalPopUpView::onClickStoneIcon);

    return NULL;
}
void BuildingHospitalPopUpView::onTipBtnClick(CCObject *pSender, CCControlEvent event)
{
    if (GlobalData::shared()->serverType==SERVER_BATTLE_FIELD || GlobalData::shared()->serverType==SERVER_DRAGON_BATTLE) {
        PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("138098")));
    }else{
        PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("102316")));
    }
}
void BuildingHospitalPopUpView::onSelectClick(CCObject *pSender, CCControlEvent event)
{
    if(m_data->count()<=0) {
        return;
    }
    if (GlobalData::shared()->serverType==SERVER_BATTLE_FIELD || GlobalData::shared()->serverType==SERVER_DRAGON_BATTLE) {
        map<std::string, TreatInfo>::iterator it;
        for(it = GlobalData::shared()->treatList.begin(); it != GlobalData::shared()->treatList.end(); it++){
            if(it->second.dead > 0){
                it->second.prepareToTreat = it->second.dead;
            }
        }
        m_tabView->reloadData();
    }else{
        m_yesSprite->setVisible(!m_yesSprite->isVisible());
        
        if (!m_selectAll) {
            int total = ArmyController::getInstance()->getTreatSolidierTreatNum();//全部
            if(total>0){
                m_tabView->reloadData();
            }
            m_selectAll = true;
        }
        else {
            int total = ArmyController::getInstance()->getTreatSolidierMaxNum();//根据当前资源量选择
            if(total>0){
                m_tabView->reloadData();
            }
            m_selectAll = false;
        }
    }
}
void BuildingHospitalPopUpView::onClickWoodIcon(CCObject *pSender, CCControlEvent event)
{
    PopupViewController::getInstance()->addPopupInView(UseResToolView::create(Wood));
}

void BuildingHospitalPopUpView::onClickFoodIcon(CCObject *pSender, CCControlEvent event)
{
    PopupViewController::getInstance()->addPopupInView(UseResToolView::create(Food));
}

void BuildingHospitalPopUpView::onClickIronIcon(CCObject *pSender, CCControlEvent event)
{
    PopupViewController::getInstance()->addPopupInView(UseResToolView::create(Iron));
}

void BuildingHospitalPopUpView::onClickStoneIcon(CCObject *pSender, CCControlEvent event)
{
    PopupViewController::getInstance()->addPopupInView(UseResToolView::create(Stone));
}

void BuildingHospitalPopUpView::onAllClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if(m_data->count()<=0) {
        return;
    }
    
    m_yesSprite->setVisible(!m_yesSprite->isVisible());
    
    if (m_yesSprite->isVisible()) {
        map<std::string, TreatInfo>::iterator it;
        for(it = GlobalData::shared()->treatList.begin(); it != GlobalData::shared()->treatList.end(); it++){
            if(it->second.dead > 0){
                it->second.prepareToTreat = it->second.dead;
            }
        }
        m_tabView->reloadData();
    }
    else {
        resetTreatNum();
        m_tabView->reloadData();
    }
}

void BuildingHospitalPopUpView::onInstantClick(CCObject * pSender, Control::EventType pCCControlEvent){
    treat(1);
}

void BuildingHospitalPopUpView::onHealthClick(CCObject * pSender, Control::EventType pCCControlEvent){
    treat(0);
}

void BuildingHospitalPopUpView::treat(int type){
    if(!checkEmptyQuene()){
        CCCommonUtils::flyText(_lang("102152"));//提示队列已满
        return;
    }
    
    long foodNum = 0;
    long woodNum = 0;
    long ironNum = 0;
    long stoneNum = 0;
    int totalNum = 0;

    map<std::string, TreatInfo>::iterator it;
    for (it = GlobalData::shared()->treatList.begin(); it != GlobalData::shared()->treatList.end(); it++) {
        int num = it->second.prepareToTreat;
        auto &info = it->second;
        foodNum += ArmyController::getInstance()->getTreatFood(num, info);
        ironNum += ArmyController::getInstance()->getTreatIron(num, info);
        woodNum += ArmyController::getInstance()->getTreatWood(num, info);
        stoneNum += ArmyController::getInstance()->getTreatStone(num, info);
        totalNum += it->second.prepareToTreat;
    }
    if (GlobalData::shared()->serverType==SERVER_BATTLE_FIELD || GlobalData::shared()->serverType==SERVER_DRAGON_BATTLE) {
        foodNum = 0;
        ironNum = 0;
        woodNum = 0;
        stoneNum = 0;
    }
    if(GlobalData::shared()->resourceInfo.totalFood()>= foodNum
       && GlobalData::shared()->resourceInfo.totalWood()>= woodNum
       && GlobalData::shared()->resourceInfo.totalStone()>= stoneNum
       && GlobalData::shared()->resourceInfo.totalIron() >= ironNum
       && totalNum > 0
       && type == NORMAL_COMMAND
       ){
        sendCommand(NORMAL_COMMAND);
    }else if(type == USEGOLD_COMMAND){
        int goldNum = atoi(m_goldText->getString().c_str());
        if(!CCCommonUtils::isLackOfGold(goldNum)){
            instantCallBack();
        }else{
            YesNoDialog::gotoPayTips();
        }
    }else{
       // CCCommonUtils::flyText(_lang("102315"));
        
        int needWood = 0;
        int needFood = 0;
        int needSilver = 0;
        int needStone =0;
        int needIron = 0;
        int useGold = 0;
        if(woodNum>GlobalData::shared()->resourceInfo.totalWood()){
            needWood = woodNum-GlobalData::shared()->resourceInfo.totalWood();
            useGold += CCCommonUtils::getResGlodByType(Wood,needWood);
        }
        if(stoneNum>GlobalData::shared()->resourceInfo.totalStone()){
            needStone = stoneNum-GlobalData::shared()->resourceInfo.totalStone();
            useGold += CCCommonUtils::getResGlodByType(Stone,needStone);
        }
        if(ironNum>GlobalData::shared()->resourceInfo.totalIron()){
            needIron = ironNum-GlobalData::shared()->resourceInfo.totalIron();
            useGold += CCCommonUtils::getResGlodByType(Iron,needIron);
        }
        if(foodNum>GlobalData::shared()->resourceInfo.totalFood()){
            needFood = foodNum-GlobalData::shared()->resourceInfo.totalFood();
            useGold += CCCommonUtils::getResGlodByType(Food,needFood);
        }
        //        if(silverNum>GlobalData::shared()->resourceInfo.lMoney){
        //            needSilver = silverNum-GlobalData::shared()->resourceInfo.lMoney;
        //            useGold += CCCommonUtils::getResGlodByType(Silver,needSilver);
        //        }
        PopupViewController::getInstance()->addPopupView(ConfirmDialogView::create(needFood,needWood,needIron,needStone,needSilver,useGold,this,callfunc_selector(BuildingHospitalPopUpView::confirmCallBack),_lang("102315"),_lang("102147")),false);
    }
}
void BuildingHospitalPopUpView::confirmCallBack(){
    auto dict = CCDictionary::create();
    map<std::string, TreatInfo>::iterator it;
    for(it = GlobalData::shared()->treatList.begin(); it != GlobalData::shared()->treatList.end(); it++){
        if(it->second.prepareToTreat > 0){
            dict->setObject(CCInteger::create(it->second.prepareToTreat), it->first);
        }
    }
    if(dict->count() > 0){
        TreatStartCommand *cmd = new TreatStartCommand(dict,1);
        cmd->sendAndRelease();
    }
    resetTreatNum();
    m_tabView->reloadData();
}
bool BuildingHospitalPopUpView::checkEmptyQuene(){
    int qid = QueueController::getInstance()->canMakeItemByType(TYPE_HOSPITAL, m_totalTimes);
    if (qid != QID_MAX) {
        return true;
    }
    return false;//treatFinishTime 这个数据已经没用了
//    return ArmyController::getInstance()->treatFinishTime == 0;
}

void BuildingHospitalPopUpView::instantCallBack(){
    sendCommand(USEGOLD_COMMAND);
}

void BuildingHospitalPopUpView::sendCommand(int type){
    auto dict = CCDictionary::create();
    map<std::string, TreatInfo>::iterator it;
    for(it = GlobalData::shared()->treatList.begin(); it != GlobalData::shared()->treatList.end(); it++){
        if(it->second.prepareToTreat > 0){
            dict->setObject(CCInteger::create(it->second.prepareToTreat), it->first);
        }
    }
    if(dict->count() > 0){
        if(type == NORMAL_COMMAND){
            TreatStartCommand *cmd = new TreatStartCommand(dict);
            cmd->sendAndRelease();
        }else{
            TreatStartUseGoldCommand *cmd = new TreatStartUseGoldCommand(dict);
            cmd->sendAndRelease();
        }
    }
    resetTreatNum();
}

std::string BuildingHospitalPopUpView::getPicPath(){
    string picName = m_info.pic + "_" + CC_ITOA(GlobalData::shared()->contryResType)+".png";
    if(GlobalData::shared()->contryResType == 0){
        picName = m_info.pic + ".png";
    }
    return picName;
}

void BuildingHospitalPopUpView::refreshView(CCObject* p){
    generateArr();
    setNumText();
    showPorgress();
    ArmyController::getInstance()->getTreatSolidierMaxNum();
    m_tabView->reloadData();
    refreshTreatNum(NULL);
    this->m_noJnjureTxt->setVisible(m_data->count()<=0);
}

void BuildingHospitalPopUpView::refreshTreatNum(CCObject* p){
    long totalFood = 0;
    long totalWood = 0;
    long totalIron = 0;
    long totalStone = 0;
    long totalTime = 0;
    long totalNum = 0;
    long useGold = 0;
    m_totalTimes = 0;
    
    map<std::string, TreatInfo>::iterator it;
    for (it = GlobalData::shared()->treatList.begin(); it != GlobalData::shared()->treatList.end(); it++) {
        long num = it->second.prepareToTreat;
        auto &info = it->second;
        totalFood += ArmyController::getInstance()->getTreatFood(num, info);
        totalIron += ArmyController::getInstance()->getTreatIron(num, info);
        totalWood += ArmyController::getInstance()->getTreatWood(num, info);
        totalStone += ArmyController::getInstance()->getTreatStone(num, info);
        totalTime += ArmyController::getInstance()->getTreatTime(num, info);
        totalNum += it->second.prepareToTreat;
    }
    if(GlobalData::shared()->serverType==SERVER_BATTLE_FIELD || GlobalData::shared()->serverType==SERVER_DRAGON_BATTLE){
        totalFood = 0;
        totalIron = 0;
        totalWood = 0;
        totalStone = 0;
    }
    if(totalFood>GlobalData::shared()->resourceInfo.totalFood()){
        useGold += CCCommonUtils::getResGlodByType(Food,totalFood-GlobalData::shared()->resourceInfo.totalFood());
        m_foodText->setColor(ccRED);
        m_foodTouch = true;
        m_foodAddNode->setVisible(true);
    }else{
        m_foodText->setColor(ccWHITE);
        m_foodTouch = false;
        m_foodAddNode->setVisible(false);
    }
    if(totalIron>GlobalData::shared()->resourceInfo.totalIron()){
        useGold += CCCommonUtils::getResGlodByType(Iron,totalIron-GlobalData::shared()->resourceInfo.totalIron());
        m_ironText->setColor(ccRED);
        m_ironTouch = true;
        m_ironAddNode->setVisible(true);
    }else{
        m_ironText->setColor(ccWHITE);
        m_ironTouch = false;
        m_ironAddNode->setVisible(false);
    }
    if(totalWood>GlobalData::shared()->resourceInfo.totalWood()){
        useGold += CCCommonUtils::getResGlodByType(Wood,totalWood-GlobalData::shared()->resourceInfo.totalWood());
        m_woodText->setColor(ccRED);
        m_woodTouch = true;
        m_woodAddNode->setVisible(true);
    }else{
        m_woodText->setColor(ccWHITE);
        m_woodTouch = false;
        m_woodAddNode->setVisible(false);
    }
    if(totalStone>GlobalData::shared()->resourceInfo.totalStone()){
        useGold += CCCommonUtils::getResGlodByType(Stone,totalStone-GlobalData::shared()->resourceInfo.totalStone());
        m_stoneText->setColor(ccRED);
        m_stoneTouch = true;
        m_stoneAddNode->setVisible(true);
    }else{
        m_stoneText->setColor(ccWHITE);
        m_stoneTouch = false;
        m_stoneAddNode->setVisible(false);
    }
//    this->m_totalText->setString(CC_ITOA(totalNum));
    if (m_foodTouch || m_woodTouch || m_ironTouch || m_stoneTouch) {
        this->getAnimationManager()->runAnimationsForSequenceNamed("Default Timeline");
    }
    if(totalNum == 0){
        this->m_instantBtn->setEnabled(false);
        this->m_healthBtn->setEnabled(false);
    }else{
        this->m_instantBtn->setEnabled(true);
        this->m_healthBtn->setEnabled(true);
    }
    m_totalTimes = totalTime;
    this->m_timeText->setString(CCCommonUtils::timeLeftToCountDown(totalTime).c_str());
    int totalGold = useGold + CCCommonUtils::getGoldByTime(totalTime);
    this->m_goldText->setString(CC_ITOA(totalGold));
    this->m_foodText->setString(CC_ITOA_K(totalFood));
    this->m_ironText->setString(CC_ITOA_K(totalIron));
    this->m_woodText->setString(CC_ITOA_K(totalWood));
    this->m_stoneText->setString(CC_ITOA_K(totalStone));

    string tempfood = "/";
    tempfood+=CC_ITOA_K(GlobalData::shared()->resourceInfo.totalFood());
    string tempiron = "/";
    tempiron+=CC_ITOA_K(GlobalData::shared()->resourceInfo.totalIron());
    string tempwood = "/";
    tempwood+=CC_ITOA_K(GlobalData::shared()->resourceInfo.totalWood());
    string tempStone = "/";
    tempStone+=CC_ITOA_K(GlobalData::shared()->resourceInfo.totalStone());
    this->m_foodLabel->setString(tempfood);
    this->m_ironLabel->setString(tempiron);
    this->m_woodLabel->setString(tempwood);
    this->m_stoneLabel->setString(tempStone);
    this->updateTime(0);
}

void BuildingHospitalPopUpView::generateArr(){
    m_data->removeAllObjects();
    map<std::string, TreatInfo>::iterator it;
    for(it = GlobalData::shared()->treatList.begin(); it != GlobalData::shared()->treatList.end(); it++){
        if(it->second.dead > 0){
            m_data->addObject(CCString::create(it->first));
        }
    }
}

void BuildingHospitalPopUpView::setNumText(){
    int currentNum = 0;
    map<std::string, TreatInfo>::iterator it;
    for(it = GlobalData::shared()->treatList.begin(); it != GlobalData::shared()->treatList.end(); it++){
        currentNum += it->second.dead;
    }
    int total = ArmyController::getInstance()->getMaxNumByType(TREAT_ARMY);
    if(GlobalData::shared()->serverType==SERVER_BATTLE_FIELD || GlobalData::shared()->serverType==SERVER_DRAGON_BATTLE){
         m_troopText->setString(_lang_1("138097", CC_ITOA(currentNum)));
    }else{
         m_troopText->setString(_lang_2("102144", CC_ITOA(currentNum), CC_ITOA(total)));
    }
    if(currentNum > total){
        currentNum = total;
    }
    if(currentNum == 0){
        this->m_allBtn->setEnabled(false);
        this->m_instantBtn->setEnabled(false);
        this->m_healthBtn->setEnabled(false);
        this->m_goldText->setString("0");
        this->m_selectBtn->setEnabled(false);
    }
//    float s = this->m_progressBarBG->getContentSize().width * currentNum * 1.0 / (total * this->m_progressBar->getContentSize().width);
//    this->m_progressBar->setScaleX(s);
}

void BuildingHospitalPopUpView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){

}

cocos2d::CCSize BuildingHospitalPopUpView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(1410, 245);
    }
    return CCSize(640, 145);
}

CCTableViewCell* BuildingHospitalPopUpView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, ssize_t idx){
    if(idx >= m_data->count()){
        return NULL;
    }
    std::string str = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();
    BuildingHospitalCell* cell = dynamic_cast<BuildingHospitalCell*>(table->dequeueGrid());
    if(idx < m_data->count()){
        TreatInfo *info = NULL;
        if(str != "button"){
            
            info = &GlobalData::shared()->treatList[str];
        }
        if(cell){
            cell->setData(info);
        }else{
            cell = BuildingHospitalCell::create(info, m_info.itemId);
        }
        cell->setTag(100);
    }
        
    return cell;
}

ssize_t BuildingHospitalPopUpView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    int num = ceil(m_data->count() / numPerRow);
    return num;
}

ssize_t BuildingHospitalPopUpView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return numPerRow;
}

void BuildingHospitalPopUpView::showPorgress(){
}

Node* BuildingHospitalPopUpView::getGuideNode(string _key){
    if (_key == "Hospital_guide") {
        m_guideKey = "Hospital_guide";
        return m_listContainer;
    }
    else if (_key == "Hospital_guide1") {
        m_guideKey = "Hospital_guide1";
        return m_guideNode;
    }
}
////-------------------

void BuildingHospitalCell::onEnter(){
    CCNode::onEnter();
}

void BuildingHospitalCell::onExit(){
    CCNode::onExit();
}

BuildingHospitalCell* BuildingHospitalCell::create(TreatInfo *info, int buildId)
{
    BuildingHospitalCell *ret = new BuildingHospitalCell(info, buildId);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool BuildingHospitalCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_levelNode", CCNode*, this->m_levelNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_editBoxNode", CCNode*, this->m_editBoxNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_armyIconContainer", CCNode*, this->m_armyIconContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_sliderContainer", CCNode*, this->m_sliderContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bg", CCNode*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameText", CCLabelIF*, this->m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numText", CCLabelIF*, this->m_numText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numMaxText", CCLabelIF*, this->m_numMaxText);
    return false;
}

SEL_CCControlHandler BuildingHospitalCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddClick", BuildingHospitalCell::onAddClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSubClick", BuildingHospitalCell::onSubClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInfoClick", BuildingHospitalCell::onInfoClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFireClick", BuildingHospitalCell::onFireClick);
    
    return NULL;
}

void BuildingHospitalCell::onAddClick(CCObject *pSender, CCControlEvent event){
    if (m_guideKey == "") {
        float value = (m_trainSlider->getValue() * m_info->dead + 1) / m_info->dead;
        m_trainSlider->setValue(value);
    }
}

void BuildingHospitalCell::onSubClick(CCObject *pSender, CCControlEvent event){
    if (m_guideKey == "") {
        float value = (m_trainSlider->getValue() * m_info->dead - 1) / m_info->dead;
        m_trainSlider->setValue(value);
    }
}

void BuildingHospitalCell::onInfoClick(CCObject *pSender, CCControlEvent event){
    if (m_guideKey == "") {
        PopupViewController::getInstance()->addPopupView(BuildMoreInfoView::create(m_buildId));
    }
}
void BuildingHospitalCell::onFireClick(CCObject *pSender, CCControlEvent event){
    if (m_guideKey == "") {
        CCLog("onFireClick");
        PopupViewController::getInstance()->addPopupView(FireSoldierView::create(m_info),false,false);
    }
}

void BuildingHospitalCell::treatAll(float dt){
    this->m_trainSlider->setValue(dt);
}

bool BuildingHospitalCell::init(){
    auto bg = CCBLoadFile("BuildingHospitalCellView", this, this);
    this->setContentSize(bg->getContentSize());
    
      int sliderW = 255;
    auto m_sliderBg = CCLoadSprite::createScale9Sprite("cs_jindutiaoBG.png");
    m_sliderBg->setInsetBottom(5);
    m_sliderBg->setInsetLeft(5);
    m_sliderBg->setInsetRight(5);
    m_sliderBg->setInsetTop(5);
    m_sliderBg->setAnchorPoint(ccp(0.5,0.5));
    m_sliderBg->setPosition(ccp(sliderW/2, 25));
    m_sliderBg->setContentSize(CCSize(sliderW,9));
    
    auto bgSp = CCLoadSprite::createSprite("cs_jindutiaoBG.png");
    bgSp->setVisible(false);
    auto proSp = CCLoadSprite::createSprite("cs_jindutiao.png");
    auto thuSp = CCLoadSprite::createSprite("cs_jindutiao_tr.png");

    
    m_trainSlider = CCSliderBar::createSlider(m_sliderBg, proSp, thuSp);
    m_trainSlider->setMinimumValue(0.0f);
    m_trainSlider->setMaximumValue(1.0f);
    m_trainSlider->setProgressScaleX(sliderW/proSp->getContentSize().width);
    m_trainSlider->setPosition(ccp(-33, -49));

    m_trainSlider->setTag(1);
    m_trainSlider->setLimitMoveValue(5);
    m_trainSlider->setTouchPriority(Touch_Popup);
    m_trainSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(BuildingHospitalCell::sliderCallBack), CCControlEventValueChanged);
    m_sliderContainer->addChild(m_trainSlider, 1);

    auto editSize = m_editBoxNode->getContentSize();
    auto editpic =CCLoadSprite::createScale9Sprite("btn_bg_op.png");
    editpic->setContentSize(editSize);
    editpic->setInsetBottom(1);
    editpic->setInsetTop(1);
    editpic->setInsetRight(1);
    editpic->setInsetLeft(1);
    m_editBox = CCEditBox::create(editSize,editpic );
    m_editBox->setInputMode(kEditBoxInputModeNumeric);
    m_editBox->setText("0");
    m_editBox->setDelegate(this);
    m_editBox->setFontColor(ccc3(233, 212, 188));
//    m_editBox->setTouchPriority(Touch_Popup);
    m_editBox->setMaxLength(12);
    m_editBox->setReturnType(kKeyboardReturnTypeDone);
    m_editBox->setPosition(ccp(editSize.width/2, editSize.height/2));
    m_editBoxNode->addChild(m_editBox);
    m_numText->setVisible(false);
    
    refreshView();
    return true;
}

void BuildingHospitalCell::refreshView(){
    this->m_armyIconContainer->removeAllChildren();

    if(m_info == nullptr){
        m_bg->setVisible(false);
        m_trainSlider->setEnabled(false);
    }else{
        m_bg->setVisible(true);
        map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->armyList.find(m_info->armyId);
        if( it == GlobalData::shared()->armyList.end() )
            return;
        std::string iconPath = it->second.getHeadIcon();
        std::string nameStr = it->second.getName();
        this->m_nameText->setString(nameStr.c_str());
//        auto sprite = CCLoadSprite::createSprite(iconPath.c_str());
//        sprite->setScale(100.0 / sprite->getContentSize().width);
        int starnum = ArmyController::getInstance()->getStarlvById(m_info->armyId);
        auto sprite = SoldierIconCell::create(iconPath.c_str(),80,m_info->armyId,true,starnum);
        this->m_armyIconContainer->addChild(sprite);
        
        if( m_info->dead != 0)
        {
            float percent = 1.0 * m_info->prepareToTreat / m_info->dead;
            m_trainSlider->setValue(percent);
        }
        m_trainSlider->setEnabled(true);
        m_trainSlider->setLimitMoveValue(25);
    }
    this->m_levelNode->removeAllChildren();
    string num1 = m_info->armyId.substr(m_info->armyId.size()-2);

    auto numLab = CCLabelIF::create();
    numLab->setFntFile("Arial_Bold_Border.fnt");
    numLab->setFontSize(16);
    numLab->setColor(ccc3(255,225,0));
    numLab->setString(CCCommonUtils::getChineseLevelTxtByNum(atoi(num1.c_str())+1).c_str());
    m_levelNode->addChild(numLab);
     
}
void BuildingHospitalCell::editBoxReturn(CCEditBox *editBox) {
    int value = atoi(editBox->getText());
    
    if (value > m_info->dead) {
        value = m_info->dead;
    } else if (value < 0) {
        value = 0;
    }

    if( m_info->dead != 0)
        m_trainSlider->setValue(1.0f * value / m_info->dead);

}
void BuildingHospitalCell::setData(TreatInfo *info){
    m_info = info;
    refreshView();
}

void BuildingHospitalCell::sliderCallBack(CCObject*sender,CCControlEvent even){
    m_info->prepareToTreat = m_trainSlider->getValue() * m_info->dead;
    refreshNum();
}

void BuildingHospitalCell::refreshNum(){
    int num = m_trainSlider->getValue() * m_info->dead;
    string strTmp = CC_ITOA(num);
    string strTmp1 ="/";
    strTmp1+=CC_ITOA(m_info->dead);
    this->m_numText->setString(strTmp);
    this->m_editBox->setText(strTmp.c_str());
    this->m_numMaxText->setString(strTmp1);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(TREAT_NUM_CHANGE);
}

//////////////////

HealthingView* HealthingView::create(int buildId){
    HealthingView* ret = new HealthingView();
    if(ret && ret->init(buildId)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool HealthingView::init(int buildId)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    
    m_buildId = buildId;
    auto tmpCCB = CCBLoadFile("HealthingView",this,this);
//    this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    this->setContentSize(tmpCCB->getContentSize());
    
    int add = getExtendHeight();
    changeBGMaxHeight(m_buildBG);
    m_mainNode->setPositionY(m_mainNode->getPositionY()+add/2);
    m_btnNode->setPositionY(m_btnNode->getPositionY()-add);
    
    m_msgLabel->setString(_lang("102199"));//剩余治疗时间
    m_btnMsgLabel->setString(_lang("104903"));
    
    FunBuildInfo& m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
    string title = _lang(m_info.name)+" "+_lang_1("102272", CC_ITOA(m_info.level));
    if(FunBuildController::getInstance()->checkOpenUpstar(m_info.type)
       && FunBuildController::getInstance()->building_Miracle_open){
        if(m_info.starNum >= 1){
            title = _lang(m_info.name) +" "+ _lang("160001");
            title.append(" ");
            title.append(CC_ITOA(m_info.starNum));
        }
    }
    setTitleName(title.c_str());
    updateInfo();
    onEnterFrame(0);
    
    return true;
}

void HealthingView::updateInfo()
{
    m_qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_HOSPITAL);
    if (m_qid != QID_MAX) {
        auto& info = GlobalData::shared()->allQueuesInfo[m_qid];
        m_curTime = info.finishTime - GlobalData::shared()->getWorldTime();
        m_sumTime = info.finishTime - info.startTime;
    }
}

void HealthingView::onEnterFrame(float dt)
{
    float pro = m_curTime*1.0/m_sumTime;
    pro = MAX(pro, 1.0);
    m_timeBar->setScaleX(1.0-pro);
    m_timeLabel->setString(CC_SECTOA(m_curTime));
    m_goldNum = CCCommonUtils::getGoldByTime(m_curTime);
    m_inBtnGoldNum->setString(CC_ITOA(m_goldNum));
    
    m_curTime--;
    if (m_curTime<0) {
        PopupViewController::getInstance()->removePopupView(this);
    }
}

void HealthingView::onEnter()
{
    CCNode::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(HealthingView::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
}

void HealthingView::onExit()
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(HealthingView::onEnterFrame), this);
    CCNode::onExit();
}

SEL_CCControlHandler HealthingView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInstantClick", HealthingView::onInstantClick);
    return NULL;
}

bool HealthingView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode", CCNode*, this->m_btnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIF*, this->m_msgLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeBar", CCScale9Sprite*, this->m_timeBar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_instantBtn", CCControlButton*, this->m_instantBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnMsgLabel", CCLabelIF*, this->m_btnMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inBtnGoldNum", CCLabelIF*, this->m_inBtnGoldNum);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    
    return false;
}

void HealthingView::onInstantClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (GlobalData::shared()->playerInfo.gold<m_goldNum) {
        YesNoDialog::gotoPayTips();
    }
    else {
        QueueController::getInstance()->startCCDQueue(m_qid, "", false, m_goldNum,"",3);
        PopupViewController::getInstance()->removePopupView(this);
    }
}
