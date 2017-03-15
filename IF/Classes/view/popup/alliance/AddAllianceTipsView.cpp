//
//  AddAllianceTipsView.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/11/2.
//
//

#include "AddAllianceTipsView.hpp"
#include "AllianceManager.h"
#include "ApplyAllianceCommand.h"
#include "PopupViewController.h"
#include "AllianceInfoView.h"
#include "ChatController.h"
#include "CreateAllianceView.h"
AddAllianceTipsView* AddAllianceTipsView::create(int type) {
    auto ret = new AddAllianceTipsView(type);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AddAllianceTipsView::init() {
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(504, true);
        CCLoadSprite::doResourceByCommonIndex(6, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(504, false);
            CCLoadSprite::doResourceByCommonIndex(6, false);
        });
        
        auto ccb = CCBLoadFile("AddAllianceTipsView",this,this);
        this->setContentSize(ccb->getContentSize());
        if (m_type == 0) {//join
            m_titleLabel->setString(_lang("115001"));
            m_rewardLabel->setString(_lang_1("115572", CC_ITOA(GlobalData::shared()->alliance_cost_k4)));
            m_tip1Label->setString(_lang("115573"));
            m_tip2Label->setString(_lang("115574"));
            m_tip3Label->setString(_lang("115575"));
            m_tip4Label->setString(_lang("115576"));
            CCCommonUtils::setButtonTitle(m_btnAlliance, _lang("103741").c_str());
        }else if (m_type == 1){//create
            m_titleLabel->setString(_lang("115007"));
            m_rewardLabel->setString(_lang_1("115584", CC_ITOA(GlobalData::shared()->alliance_cost_k4)));
            m_tip1Label->setString(_lang("115585"));
            m_tip2Label->setString(_lang("115586"));
            m_tip3Label->setString(_lang("115587"));
            m_tip4Label->setString(_lang("115588"));
            CCCommonUtils::setButtonTitle(m_btnAlliance, _lang("115589").c_str());
        }
        
       
        
        
        this->setModelLayerTouchCallback([&](cocos2d::CCTouch *pTouch){
            // detect touch inside panel
            if (!isTouchInside(m_touchNode, pTouch)) {
                closeSelf();
            }
        });
        
        return true;
        
    }
    return false;
}

bool AddAllianceTipsView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardLabel", CCLabelIF*, this->m_rewardLabel)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tip1Label", CCLabelIF*, this->m_tip1Label)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tip2Label", CCLabelIF*, this->m_tip2Label)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tip3Label", CCLabelIF*, this->m_tip3Label)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tip4Label", CCLabelIF*, this->m_tip4Label)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", Node*, this->m_touchNode)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnAlliance", CCControlButton*, this->m_btnAlliance)
    
    return false;
}

SEL_CCControlHandler AddAllianceTipsView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickClose", AddAllianceTipsView::onClickClose);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtnClick", AddAllianceTipsView::onBtnClick);
    return NULL;
}
void AddAllianceTipsView::onBtnClick(CCObject * pSender, Control::EventType pCCControlEvent){
    if(CCCommonUtils::isKuaFu()){
        closeSelf();
        return ;
    }
    if(m_type == 1){//create
        PopupViewController::getInstance()->addPopupView(CreateAllianceView::create(11));
        closeSelf();
    }else{//join
        string tempAid = AllianceManager::getInstance()->getrecommendAid();
        if (!tempAid.empty()) {
            ApplyAllianceCommand* cmd = new ApplyAllianceCommand(tempAid);
            cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(AddAllianceTipsView::joinSuccess), NULL));
            cmd->sendAndRelease();
        }
        else{
            closeSelf();
        }
        
    }
}
void AddAllianceTipsView::joinSuccess(Ref* data){
    AutoSafeRef temp(this);
    if(GlobalData::shared()->playerInfo.isInAlliance()){
        CCCommonUtils::setIsHDViewPort(true);
        PopupViewController::getInstance()->removeAllPopupView();
        PopupViewController::getInstance()->addPopupInView(AllianceInfoView::create(&GlobalData::shared()->playerInfo.allianceInfo));
       }
    
    //发聊天消息，加入联盟
    string dialog = "115188";
    ChatController::getInstance()->sendCountryChat(_lang("115188").append("  (").append(_lang("115181")).append(")").c_str(), CHAT_ALLIANCE, 2, "", dialog.c_str(), NULL);
}
void AddAllianceTipsView::onClickClose(CCObject * pSender, Control::EventType pCCControlEvent) {
    this->closeSelf();
}
void AddAllianceTipsView::onEnter() {
    PopupBaseView::onEnter();
}

void AddAllianceTipsView::onExit() {
    PopupBaseView::onExit();
}