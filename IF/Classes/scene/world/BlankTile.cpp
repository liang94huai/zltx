//
//  BlankTile.cpp
//  IF
//
//  Created by 邹 程 on 14-2-26.
//
//

#include "BlankTile.h"
#include "WorldCommand.h"
#include "WorldMapView.h"
#include "YesNoDialog.h"
#include "AddFavorite.h"
#include "MoveCityPopUpView.h"
#include "CrossServerMoveCityPopUpView.h"
#include "FunBuildController.h"
#include "SoundController.h"
#include "AllianceInfoMembersView.h"
#include "AllianceManager.h"
#include "AllianceTerritoryView.h"
#include "TerritoryInformationView.h"
#include "ToolController.h"
#include "ActivityController.h"

void BlankTile::onEnter(){
    NewBaseTileInfo::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BlankTile::secondConfirm), TRANS_KINGDOM_SECOND_CONFIRM, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BlankTile::confirmed), TRANS_KINGDOM_CONFIRMED, NULL);
}

void BlankTile::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, TRANS_KINGDOM_SECOND_CONFIRM);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, TRANS_KINGDOM_CONFIRMED);
    NewBaseTileInfo::onExit();
}

bool BlankTile::init() {
    NewBaseTileInfo::init();
    m_title->setString(_lang(m_cityInfo.cityName).c_str());
    
    if(!WorldController::isInSelfServer(m_cityInfo.tileServerId)){
        auto &tool = ToolController::getInstance()->getToolInfoById(getNewBeginnerMoveItemId());
        bool is_teleport_limit_open = GlobalData::shared()->teleport_limit_open;
        bool isGm = GlobalData::shared()->playerInfo.gmFlag == 1;
        bool haveBtn = true;
        if(is_teleport_limit_open && !isGm)//限制功能开启
        {
            double closeTime = GlobalData::shared()->playerInfo.regTime + GlobalData::shared()->teleport_limit_time * 3600;
            double currentTime = GlobalData::shared()->getTimeStamp();
            if(currentTime >= closeTime)
            {
                haveBtn = false;
            }
        }
        
            
        if (haveBtn && tool.getCNT()>0 && FunBuildController::getInstance()->getMainCityLv() < GlobalData::shared()->new_trans_kingdom_level&&!CCCommonUtils::isInSimulator()) {
            setButtonCount(2);
            setButtonName(2,_lang("108719")); // teleport
            setButtonState(2, ButtonTeleport);
            setButtonCallback(2,cccontrol_selector(BlankTile::onClickTeleport));
            setButtonName(3,_lang("E100184")); // teleport
            setButtonState(3, ButtonTransKingdom);
            setButtonCallback(3,cccontrol_selector(BlankTile::onClickTransKingdom));
        }
        else {
            setButtonCount(1);
            setButtonName(1,_lang("108719")); // teleport
            setButtonState(1, ButtonTeleport);
            setButtonCallback(1,cccontrol_selector(BlankTile::onClickTeleport));
        }
    }else{
//        setButtonCount(2);
//        setButtonName(3,_lang("108720")); // occupy
//        setButtonName(2,_lang("108719")); // teleport
//        setButtonState(3, ButtonOccupy);
//        setButtonState(2, ButtonTeleport);
//        setButtonCallback(3,cccontrol_selector(BlankTile::onClickOccupy));
//        setButtonCallback(2,cccontrol_selector(BlankTile::onClickTeleport));
        int rank = GlobalData::shared()->playerInfo.allianceInfo.rank;
        if (rank >= 4) {
            if ((GlobalData::shared()->serverType==SERVER_BATTLE_FIELD || GlobalData::shared()->serverType==SERVER_DRAGON_BATTLE) && WorldController::getInstance()->isInCrossMap()) {
                setButtonCount(3);
                setButtonName(3, _lang("108720")); // occupy
                setButtonName(2, _lang("108719")); // teleport
                setButtonName(1, _lang("115285")); // invite teleport
                setButtonState(3, ButtonOccupy);
                setButtonState(2, ButtonTeleport);
                setButtonState(1, ButtonWorldInviteTeleport);
                setButtonCallback(3, cccontrol_selector(BlankTile::onClickOccupy));
                setButtonCallback(2,cccontrol_selector(BlankTile::onClickTeleport));
                setButtonCallback(1,cccontrol_selector(BlankTile::onClickInviteTeleport));
                if(GlobalData::shared()->serverType==SERVER_DRAGON_BATTLE && ActivityController::getInstance()->wa_switch_k3==0){
                    setButtonCount(2);
                    setButtonName(3, _lang("108720")); // occupy
                    setButtonName(2, _lang("108719")); // teleport
                    setButtonState(3, ButtonOccupy);
                    setButtonState(2, ButtonTeleport);
                    setButtonCallback(3, cccontrol_selector(BlankTile::onClickOccupy));
                    setButtonCallback(2,cccontrol_selector(BlankTile::onClickTeleport));
                }else{
                    setButtonCount(3);
                    setButtonName(3, _lang("108720")); // occupy
                    setButtonName(2, _lang("108719")); // teleport
                    setButtonName(1, _lang("115285")); // invite teleport
                    setButtonState(3, ButtonOccupy);
                    setButtonState(2, ButtonTeleport);
                    setButtonState(1, ButtonWorldInviteTeleport);
                    setButtonCallback(3, cccontrol_selector(BlankTile::onClickOccupy));
                    setButtonCallback(2,cccontrol_selector(BlankTile::onClickTeleport));
                    setButtonCallback(1,cccontrol_selector(BlankTile::onClickInviteTeleport));
                }
            }else{
                setButtonCount(4);
                setButtonName(3, _lang("108720")); // occupy
                setButtonName(2, _lang("108719")); // teleport
                setButtonName(4, _lang("115285")); // invite teleport
                setButtonName(5, _lang("115302"));// territory
                setButtonState(3, ButtonOccupy);
                setButtonState(2, ButtonTeleport);
                setButtonState(4, ButtonWorldInviteTeleport);
                setButtonState(5, ButtonPlace);
                setButtonCallback(3, cccontrol_selector(BlankTile::onClickOccupy));
                setButtonCallback(2,cccontrol_selector(BlankTile::onClickTeleport));
                setButtonCallback(4,cccontrol_selector(BlankTile::onClickInviteTeleport));
                setButtonCallback(5,cccontrol_selector(BlankTile::onClickPlace));
                
//                setButtonCount(3);
//                setButtonName(3, _lang("108720")); // occupy
//                setButtonName(2, _lang("108719")); // teleport
//                setButtonName(1, _lang("115285")); // invite teleport
//                setButtonState(3, ButtonOccupy);
//                setButtonState(2, ButtonTeleport);
//                setButtonState(1, ButtonWorldInviteTeleport);
//                setButtonCallback(3, cccontrol_selector(BlankTile::onClickOccupy));
//                setButtonCallback(2,cccontrol_selector(BlankTile::onClickTeleport));
//                setButtonCallback(1,cccontrol_selector(BlankTile::onClickInviteTeleport));
            }
        }else if (rank >= 3){
            setButtonCount(3);
            setButtonName(3, _lang("108720")); // occupy
            setButtonName(2, _lang("108719")); // teleport
            setButtonName(1, _lang("115285")); // invite teleport
            setButtonState(3, ButtonOccupy);
            setButtonState(2, ButtonTeleport);
            setButtonState(1, ButtonWorldInviteTeleport);
            setButtonCallback(3, cccontrol_selector(BlankTile::onClickOccupy));
            setButtonCallback(2,cccontrol_selector(BlankTile::onClickTeleport));
            setButtonCallback(1,cccontrol_selector(BlankTile::onClickInviteTeleport));
        }
        else {
            setButtonCount(2);
            setButtonName(3, _lang("108720")); // occupy
            setButtonName(2, _lang("108719")); // teleport
            setButtonState(3, ButtonOccupy);
            setButtonState(2, ButtonTeleport);
            setButtonCallback(3, cccontrol_selector(BlankTile::onClickOccupy));
            setButtonCallback(2,cccontrol_selector(BlankTile::onClickTeleport));
        }
    }
    setBtnState();
    
    std::string str = "";
    if(m_cityInfo.tileGid < 98){
        int index = m_cityInfo.tileGid % 98;
        if(index < 10){
            str = CCString::createWithFormat("00%s.png", CC_ITOA(index))->getCString();
        }else{
            str = CCString::createWithFormat("0%s.png", CC_ITOA(index))->getCString();
        }
    }else{
        int index = m_cityInfo.tileGid % 98;
        if(index < 10){
            str = CCString::createWithFormat("000%s.png", CC_ITOA(index))->getCString();
        }else{
            str = CCString::createWithFormat("00%s.png", CC_ITOA(index))->getCString();
        }
    };

    addToParent();
    return true;
}
int BlankTile::getNewBeginnerMoveItemId(){
    return ITEM_NEW_BEGINNER_MOVE;
}

bool BlankTile::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    return NewBaseTileInfo::onAssignCCBMemberVariable(pTarget, pMemberVariableName, pNode);
}

SEL_CCControlHandler BlankTile::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NewBaseTileInfo::onResolveCCBCCControlSelector(pTarget, pSelectorName);
}

void BlankTile::setBtnState(){
    if (!canOccupy()) {
        setButtonEnable(3);
    }
}

bool BlankTile::canOccupy(){
    if (m_cityInfo.cityType == OriginTile && (m_cityInfo.cityOriginType == "stop" || m_cityInfo.cityOriginType == "neutrally")) {
        return false;
    }
    return true;
}

BaseTileInfo* BlankTile::getFavoriteView() {
    return AddFavorite::create(m_cityInfo.cityIndex,_lang(m_cityInfo.cityName));
}

void BlankTile::onClickTeleport(CCObject * pSender, Control::EventType pCCControlEvent) {
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    //王国远争后，迁回原服
    if(CCCommonUtils::isKuaFuWangZhan() &&  GlobalData::shared()->playerInfo.crossFightSrcServerId==GlobalData::shared()->playerInfo.currentServerId){
        PopupViewController::getInstance()->addPopupView(MoveCityPopUpView::create(MoveCity_Castle));
		closeThis();
        return;
    }
    if (!WorldController::isInSelfServer(m_cityInfo.tileServerId)) {
        if(ActivityController::getInstance()->checkServerCanJoin(GlobalData::shared()->playerInfo.currentServerId) && m_cityInfo.cityOriginType == "neutrally"){
            PopupViewController::getInstance()->addPopupView(CrossServerMoveCityPopUpView::create());
            closeThis();
            return ;
        }
    }
    if(WorldController::getInstance()->isInFresherForbidenArea(m_cityInfo.cityIndex) && FunBuildController::getInstance()->getMainCityLv() <= _limit_cross_server){
        if(!WorldController::isInSelfServer(m_cityInfo.tileServerId)){
            CCCommonUtils::flyText(_lang("104948"));
        }else{
            CCCommonUtils::flyText(_lang("108878"));
        }
        return;
    }
    
    if(!WorldController::isInSelfServer(m_cityInfo.tileServerId)){
        
        bool is_teleport_limit_open = GlobalData::shared()->teleport_limit_open;
        bool isGm = GlobalData::shared()->playerInfo.gmFlag == 1;
        bool haveLimit = false;
        if(is_teleport_limit_open && !isGm)//限制功能开启
        {
            double closeTime = GlobalData::shared()->playerInfo.regTime + GlobalData::shared()->teleport_limit_time * 3600;
            double currentTime = GlobalData::shared()->getTimeStamp();
            if(currentTime >= closeTime)
            {
                haveLimit = true;
            }
        }
        
       if( haveLimit || FunBuildController::getInstance()->getMainCityLv() > _limit_cross_server){
            CCCommonUtils::flyHint("", "", _lang("108751"));
            return;
        }

        PopupViewController::getInstance()->addPopupView(CrossServerMoveCityPopUpView::create());
    }else{
        PopupViewController::getInstance()->addPopupView(MoveCityPopUpView::create(MoveCity_Castle));
    }

    closeThis();
}

void BlankTile::onClickTransKingdom(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent)
{
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    PopupViewController::getInstance()->addPopupView(TransKingdomAlertPopUpView::create(1));
}

void BlankTile::secondConfirm(CCObject* obj){
    PopupViewController::getInstance()->addPopupView(TransKingdomAlertPopUpView::create(2));
}

void BlankTile::confirmed(CCObject* obj){
    auto dict = CCDictionary::create();
    dict->setObject(CCString::createWithFormat("%d",0), "point");
    auto& info = ToolController::getInstance()->getToolInfoById(ITEM_NEW_BEGINNER_MOVE);
    if (info.getCNT() > 0) {
        dict->setObject(CCString::create(info.uuid), "goodsId");
        if (GlobalData::shared()->trans_res_forbidden_swith == 1) {
            bool flag = ActivityController::getInstance()->checkServerCanJoin(GlobalData::shared()->playerInfo.currentServerId);
            if (!flag && (GlobalData::shared()->resourceInfo.lFood > GlobalData::shared()->trans_res_limit || GlobalData::shared()->resourceInfo.lWood > GlobalData::shared()->trans_res_limit)) {
                CCCallFuncO* fun = CCCallFuncO::create(this, callfuncO_selector(BlankTile::alertShow), dict);
                YesNoDialog::marchAlertShow(_lang_1("104959", CC_ITOA(GlobalData::shared()->trans_res_limit)), fun, NULL);
            }
            else {
                WorldMapView::instance()->afterCrossCityMove(dict);
                this->closeSelf();
            }
        }
        else {
            WorldMapView::instance()->afterCrossCityMove(dict);
            this->closeSelf();
        }
    } else {
        closeSelf();
    }
    return;
}

void BlankTile::alertShow(CCObject *dict){
    CCDictionary* dic = dynamic_cast<CCDictionary*>(dict);
    WorldMapView::instance()->afterCrossCityMove(dic);
    this->closeSelf();
}

void BlankTile::onClickInviteTeleport(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    AllianceManager::getInstance()->tmpCityIndex = m_cityInfo.cityIndex;
    PopupViewController::getInstance()->addPopupInView(AllianceInfoMembersView::create(GlobalData::shared()->playerInfo.allianceInfo.uid, false, 1));
    closeThis();
}
void BlankTile::onClickPlace(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    AllianceManager::getInstance()->tmpCityIndex = m_cityInfo.cityIndex;
    PopupViewController::getInstance()->addPopupInView(AllianceTerritoryView::create(m_cityInfo.cityIndex,true));
    closeThis();
}
void BlankTile::onClickOccupy(CCObject * pSender, Control::EventType pCCControlEvent) {
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    if (!canOccupy()) {
        CCCommonUtils::flyHint("", "", _lang("108733"));
        return;
    }
    auto button = dynamic_cast<CCControlButton*>(pSender);
    if (button) {
        button->setEnabled(false);
    }
    WorldController::getInstance()->alertProectFlag = true;
    if (!m_cityInfo.playerName.empty())
    {
        // have owner
        WorldController::getInstance()->m_alertStateFlag = 2;
    }
    else
    {
        // no owner
        WorldController::getInstance()->m_alertStateFlag = 0;
        
        for(auto it :WorldController::getInstance()->m_marchInfo)
        {
            if(it.second.stateType == StateMarch && it.second.endPointIndex == this->m_cityInfo.cityIndex)
            {
                WorldController::getInstance()->m_alertStateFlag = 1;
                break;
            }
        }
    }
    WorldController::getInstance()->openMarchDeploy(m_cityInfo.cityIndex,0);

    closeThis();
}

////////////
bool TransKingdomAlertPopUpView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        auto bg = CCBLoadFile("crossserveralert", this, this);
        this->setContentSize(bg->getContentSize());
        this->m_title->setString(_lang("105261"));
        this->m_leftTime->setString("");
        if (m_type == 1) {
            this->m_hintText->setString(_lang("E100182"));
            bool is_teleport_limit_open = GlobalData::shared()->teleport_limit_open;
            bool isGm = GlobalData::shared()->playerInfo.gmFlag == 1;
            if(is_teleport_limit_open && !isGm)
            {
                this->unschedule(schedule_selector(TransKingdomAlertPopUpView::onGameTick));
                this->schedule(schedule_selector(TransKingdomAlertPopUpView::onGameTick), 1.0f);
                onGameTick(0);
            }
        }
        else if (m_type == 2) {
            this->m_hintText->setString(_lang("E100183"));
        }
        CCCommonUtils::setButtonTitle(m_confirmBtn, _lang("confirm").c_str());
        CCCommonUtils::setButtonTitle(m_cancelBtn, _lang("cancel_btn_label").c_str());
        
        ret = true;
    }
    return ret;
}

void TransKingdomAlertPopUpView::onGameTick(float dt)
{
    double closeTime = GlobalData::shared()->playerInfo.regTime + GlobalData::shared()->teleport_limit_time * 3600;
    double currentTime = GlobalData::shared()->getTimeStamp();
    if(currentTime >= closeTime)
    {
        m_leftTime->setString("");
        this->unschedule(schedule_selector(TransKingdomAlertPopUpView::onGameTick));
        return;
    }
    else
    {
        double lastTime = closeTime- currentTime;
//        m_leftTime->setString(CC_SECTOA(lastTime));
        m_leftTime->setString(_lang_1("108803", CC_SECTOA(lastTime)));
        if(dt == 0)
        {
            float sizeWidth = m_leftTime->getContentSize().width;
           
            m_leftTime->setPositionX( -(sizeWidth*m_leftTime->getOriginScaleX())/2 );
        }
    }
    
}

void TransKingdomAlertPopUpView::onEnter(){
    PopupBaseView::onEnter();
}

void TransKingdomAlertPopUpView::onExit(){
    PopupBaseView::onExit();
}

cocos2d::SEL_MenuHandler TransKingdomAlertPopUpView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}

bool TransKingdomAlertPopUpView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_confirmBtn", CCControlButton*, this->m_confirmBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cancelBtn", CCControlButton*, this->m_cancelBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title", CCLabelIF*, this->m_title);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintText", CCLabelIF*, this->m_hintText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftTime", CCLabelIF*, this->m_leftTime);
    
    return false;
}

SEL_CCControlHandler TransKingdomAlertPopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickOKBtn", TransKingdomAlertPopUpView::onClickOKBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCancelBtn", TransKingdomAlertPopUpView::onClickCancelBtn);
    return NULL;
}

void TransKingdomAlertPopUpView::onClickOKBtn(CCObject * pSender, CCControlEvent pCCControlEvent){
    if (m_type == 1) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(TRANS_KINGDOM_SECOND_CONFIRM);
    }
    else if (m_type ==2) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(TRANS_KINGDOM_CONFIRMED);
    }
    this->closeSelf();
}

void TransKingdomAlertPopUpView::onClickCancelBtn(CCObject * pSender, CCControlEvent pCCControlEvent){
    this->closeSelf();
}
