//
//  DragonBattleJoinView.cpp
//  IF
//
//  Created by ganxiaohua on 15/8/25.
//
//

#include "DragonBattleJoinView.h"
#include "PopupViewController.h"
#include "CommandBase.h"
#include "YesNoDialog.h"
#include "DragonBattleWarListView.h"
#include "DragonManageView.h"
#include "AgainstInfoView.h"
#include "DragonHistoryView.h"
#include "DragonBattleApplyCommand.h"
#include "DragonBattleInfoCommand.h"
#include "ActivityController.h"
#include "GameController.h"
#include "DragonBattleEnterCommand.h"
#include "DragonBattleLeaveCommand.h"
#include "DragonStatisticsView.h"
#include "DynamicResourceController.h"
#include "QueueController.h"
#include "ArmyController.h"
#include "FunBuildController.h"
#include "BuildingHospitalPopUpView.h"
#include "ActivityController.h"

DragonBattleJoinView *DragonBattleJoinView::create(){
    DragonBattleJoinView *ret = new DragonBattleJoinView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonBattleJoinView::onEnter(){
    this->setTitleName(_lang("140001"));
    PopupBaseView::onEnter();
    updateState(0);
}

void DragonBattleJoinView::onExit(){
    if (m_waitInterface) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    GameController::getInstance()->removeWaitInterface();
    PopupBaseView::onExit();
}

bool DragonBattleJoinView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void DragonBattleJoinView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){

}

bool DragonBattleJoinView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        bool haveFlag = CCLoadSprite::checkHaveLoadCommonRes(500);
        if (DynamicResourceController::getInstance()->checkActivityAdResource())
        {
            DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_ACTIVITY_AD_TEXTURE,false);
        }
        if (!haveFlag) {
            CCLoadSprite::doResourceByCommonIndex(512, true);
            CCLoadSprite::doResourceByCommonIndex(8, true);
            CCLoadSprite::doResourceByCommonIndex(500, true);
            CCLoadSprite::doResourceByCommonIndex(506, true);
            setCleanFunction([](){
                CCLoadSprite::doResourceByCommonIndex(512, false);
                CCLoadSprite::doResourceByCommonIndex(8, false);
                CCLoadSprite::doResourceByCommonIndex(500, false);
                CCLoadSprite::doResourceByCommonIndex(506, false);
                DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_ACTIVITY_AD_TEXTURE,true);
            });
        }else{
            CCLoadSprite::doResourceByCommonIndex(512, true);
            CCLoadSprite::doResourceByCommonIndex(8, true);
            setCleanFunction([](){
                CCLoadSprite::doResourceByCommonIndex(512, false);
                CCLoadSprite::doResourceByCommonIndex(8, false);
            });
        }

        auto node = CCBLoadFile("DragonBattleJoinView", this, this);
        this->setContentSize(node->getContentSize());
        if(!CCCommonUtils::isIosAndroidPad()){
            int prev = m_viewBg->getContentSize().height;
            this->changeBGHeight(m_viewBg);
            int add = m_viewBg->getContentSize().height - prev;
            m_bottomNode->setPositionY(m_bottomNode->getPositionY() - add);
            m_fixbg->setScaleY((add+646.0*0.6)/646.0);
            m_moveNode->setPositionY(m_moveNode->getPositionY() - add/2);
        }
        m_battleTile->setString(_lang("140001"));
        m_battleDesc->setString(_lang("140002"));
        m_pipeiTxt->setString(_lang("140008"));

        CCCommonUtils::setButtonTitle(m_joinBtn, _lang("140005").c_str());
        CCCommonUtils::setButtonTitle(m_helpBtn, _lang("140006").c_str());
        CCCommonUtils::setButtonTitle(m_historyBtn, _lang("140007").c_str());
        CCCommonUtils::setButtonTitle(m_infoBtn, _lang("140010").c_str());
        CCCommonUtils::setButtonTitle(m_manageBtn, _lang("140043").c_str());
        
        CCSpriteFrame* sf = CCLoadSprite::getSF("57009_ad1.png");
        if (sf!=NULL) {
            m_dragonBg->initWithSpriteFrame(sf);
        }
        
        refreshFlag = false;
        
        DragonBattleInfoCommand* cmd = new DragonBattleInfoCommand();
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(DragonBattleJoinView::getData), NULL));
        cmd->sendAndRelease();
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_moveNode);
        m_waitInterface->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2-200));
        
        this->schedule(schedule_selector(DragonBattleJoinView::updateState), 1);
        updateState(0);

        ActivityController::getInstance()->isMoveToDragonBuild = false;
        ret = true;
    }
    return ret;
}

void DragonBattleJoinView::getData(CCObject *param){
    if (m_waitInterface) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dic = _dict(result->getData());
    if(dic){
        ActivityController::getInstance()->parssDragonData(dic);
        this->updateState(0);
    }
}

void DragonBattleJoinView::updateState(float _time){
    m_joinBtn->setVisible(false);
    m_helpBtn->setVisible(false);
    m_historyBtn->setVisible(false);
    m_infoBtn->setVisible(false);
    m_manageBtn->setVisible(false);
    m_endNode->setVisible(false);
    m_startNode->setVisible(false);
    m_pipeiNode->setVisible(false);
    m_joinBtn->setEnabled(true);
    m_endTime->setColor({255,234,64});
    double left = 0;
    if(!ActivityController::getInstance()->checkServerInOpenList(ActivityController::getInstance()->wa_switch_k4)){
        m_startTime->setString(_lang("140193").c_str());
        m_startNode->setVisible(true);
        return ;
    }
    if (ActivityController::getInstance()->bye==1 && ActivityController::getInstance()->dragonBattleState>=2 && ActivityController::getInstance()->dragonTeamApplyState==1) {
        m_helpBtn->setVisible(true);
        m_historyBtn->setVisible(true);
        m_startTime->setString(_lang("140137").c_str());
        m_startNode->setVisible(true);
        return ;
    }
    if (ActivityController::getInstance()->dragonBattleState>=2 && ActivityController::getInstance()->dragonTeamApplyState==0) {
        m_helpBtn->setVisible(true);
        m_historyBtn->setVisible(true);
        double left = ActivityController::getInstance()->nextApplyBeginTime - GlobalData::shared()->getWorldTime();
        if (left<=0) {
            left = 0;
        }
        m_startTime->setString(_lang_1("140144",CC_SECTOA(left)));
        m_startNode->setVisible(true);
        m_endTime->setString(_lang("140138").c_str());
        m_endNode->setVisible(true);
        return;
    }
    if (ActivityController::getInstance()->dragonBattleState==1 && ActivityController::getInstance()->dragonTeamApplyState==0) {//报名
        m_joinBtn->setVisible(true);
        m_helpBtn->setVisible(true);
        m_historyBtn->setVisible(true);
    }else if(ActivityController::getInstance()->dragonBattleState==1 && ActivityController::getInstance()->dragonTeamApplyState==1){//匹配中
        m_manageBtn->setVisible(true);
        m_helpBtn->setVisible(true);
        m_historyBtn->setVisible(true);
        m_endNode->setVisible(true);
        m_pipeiNode->setVisible(true);
        m_endTime->setString(_lang("140155"));
    }else if(ActivityController::getInstance()->dragonBattleState==2 && ActivityController::getInstance()->dragonTeamApplyState==1){
        m_infoBtn->setVisible(true);
        m_manageBtn->setVisible(true);
        m_helpBtn->setVisible(true);
        m_historyBtn->setVisible(true);
        m_joinBtn->setVisible(true);
        CCCommonUtils::setButtonTitle(m_joinBtn, _lang("140013").c_str());
        m_joinBtn->setEnabled(false);
    }else if(ActivityController::getInstance()->dragonBattleState==3 && ActivityController::getInstance()->dragonTeamApplyState==1){
        m_manageBtn->setVisible(true);
        m_helpBtn->setVisible(true);
        m_historyBtn->setVisible(true);
        m_joinBtn->setVisible(true);
        if (GlobalData::shared()->serverType==SERVER_DRAGON_BATTLE) {
            CCCommonUtils::setButtonTitle(m_joinBtn, _lang("140061").c_str());
        }else{
            CCCommonUtils::setButtonTitle(m_joinBtn, _lang("140013").c_str());
        }
        left = GlobalData::shared()->dbFightEndTime - GlobalData::shared()->getWorldTime();
        if (left>0) {
            m_startTime->setString(_lang_1("140060",CC_SECTOA(left)));
            m_startNode->setVisible(true);
        }
    }else{
        m_helpBtn->setVisible(true);
        m_historyBtn->setVisible(true);
        if (ActivityController::getInstance()->dragonBattleState>=4) {
            m_startTime->setString(_lang("140143").c_str());
            m_startNode->setVisible(true);
            left = ActivityController::getInstance()->nextApplyBeginTime - GlobalData::shared()->getWorldTime();
            if (left>0) {
                m_endTime->setString(_lang_1("140144",CC_SECTOA(left)));
                m_endNode->setVisible(true);
            }
        }
    }
    
    //如果没有踢出去，可以主动出去
   if(ActivityController::getInstance()->dragonBattleState>3 && ActivityController::getInstance()->dragonTeamApplyState==1 && GlobalData::shared()->serverType==SERVER_DRAGON_BATTLE){
       m_joinBtn->setVisible(true);
       CCCommonUtils::setButtonTitle(m_joinBtn, _lang("140061").c_str());
   }
    
    left = ActivityController::getInstance()->fightBeginTime - GlobalData::shared()->getWorldTime();
    if (left>=0) {
        m_startTime->setString(_lang_1("140003",CC_SECTOA(left)));
        m_startNode->setVisible(true);
        if (left==0) {
            if (!refreshFlag) {
                DragonBattleInfoCommand* cmd = new DragonBattleInfoCommand();
                cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(DragonBattleJoinView::getData), NULL));
                cmd->sendAndRelease();
                if (m_waitInterface) {
                    m_waitInterface->remove();
                    m_waitInterface = NULL;
                }
                m_waitInterface = GameController::getInstance()->showWaitInterface(m_moveNode);
                m_waitInterface->setPosition(ccp(320, 300));
            }
            refreshFlag = true;
        }
    }
    
    left = ActivityController::getInstance()->applyEndTime - GlobalData::shared()->getWorldTime();
    if (left>=0  && ActivityController::getInstance()->applyEndTime >0 && ActivityController::getInstance()->dragonBattleState==1) {
        m_endTime->setString(_lang_1("140004",CC_SECTOA(left)));
        m_endNode->setVisible(true);
        m_infoBtn->setVisible(false);
    }
    left = ActivityController::getInstance()->enterFightCdEndTime - GlobalData::shared()->getWorldTime();
    if(left>0 && ActivityController::getInstance()->dragonBattleState==3 && ActivityController::getInstance()->dragonTeamApplyState==1 && GlobalData::shared()->serverType != SERVER_DRAGON_BATTLE){
        m_endTime->setString(_lang_1("140173",CC_SECTOA(left)));
        m_endNode->setVisible(true);
        m_endTime->setColor({255,0,0});
    }
}

void DragonBattleJoinView::onClickJoin(CCObject *pSender, CCControlEvent event){
    if(ActivityController::getInstance()->dragonBattleState>=2 && ActivityController::getInstance()->dragonTeamApplyState==1 && GlobalData::shared()->serverType==SERVER_DRAGON_BATTLE){
        string tip = _lang("140157");
        if (ActivityController::getInstance()->dragonBattleState==3) {
            YesNoDialog* dialog2 = YesNoDialog::show(tip.c_str(), CCCallFunc::create(this, callfunc_selector(DragonBattleJoinView::leavelDragon)));
        }else{
            leavelDragon();
        }
        return ;
    }else if (ActivityController::getInstance()->dragonBattleState==1 && ActivityController::getInstance()->dragonTeamApplyState==0) {//报名
        if(GlobalData::shared()->playerInfo.isInAlliance() && GlobalData::shared()->playerInfo.allianceInfo.rank>=4){
            PopupViewController::getInstance()->addPopupInView(DragonBattleWarListView::create());
        }else{
            YesNoDialog::show(_lang("140106").c_str(), NULL);
        }
    }
    else if(ActivityController::getInstance()->dragonBattleState>=2 && ActivityController::getInstance()->dragonTeamApplyState==1){
        if(ActivityController::getInstance()->canFight==1){
            if (GlobalData::shared()->serverType!=SERVER_DRAGON_BATTLE && (ArmyController::getInstance()->getTotalDead()>0 || QueueController::getInstance()->getQueueNumByType(TYPE_HOSPITAL)>0)) {
                YesNoDialog* dialog = YesNoDialog::show(_lang("140146").c_str(), CCCallFunc::create(this, callfunc_selector(DragonBattleJoinView::healSoldier)));
                CCCommonUtils::setButtonTitle(dialog->m_btnOk, _lang("138113").c_str());
                return ;
            }
            double left = ActivityController::getInstance()->enterFightCdEndTime - GlobalData::shared()->getWorldTime();
            if(left>0){
                int minute = left/60;
                minute = MAX(minute,1);
                YesNoDialog::show(_lang_1("140169",CC_ITOA(minute)), NULL);
                return ;
            }
            if(DynamicResourceController::getInstance()->checkDragonMapResource()){
                DragonBattleEnterCommand* cmd = new DragonBattleEnterCommand();
                cmd->sendAndRelease();
                GameController::getInstance()->showWaitInterface();
            }else{
                CCCommonUtils::flyHint("", "", _lang("140218"), 3, 0, true);
            }
        }else{
            YesNoDialog::show(_lang("140121").c_str(), NULL);
        }
    }
}

void DragonBattleJoinView::leavelDragon(){
    DragonBattleLeaveCommand* cmd = new DragonBattleLeaveCommand();
    cmd->sendAndRelease();
    GameController::getInstance()->showWaitInterface();
}

void DragonBattleJoinView::healSoldier(){
    int buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_HOSPITAL);
    if (buildId<=0) {
        return ;
    }
    if (QueueController::getInstance()->getQueueNumByType(TYPE_HOSPITAL)>0) {
        int m_qType = CCCommonUtils::getQueueTypeByBuildType(buildId/1000);
        if (m_qType != -1) {
            m_qid = QueueController::getInstance()->getMinTimeQidByType(m_qType);
        }
        auto it = GlobalData::shared()->allQueuesInfo.find(m_qid);
        if (it!=GlobalData::shared()->allQueuesInfo.end()) {
            double lastTime = it->second.finishTime - it->second.startTime;
            m_gold = CCCommonUtils::getGoldByTime(lastTime);
            if(PopupViewController::getInstance()->CanPopPushView) {
                YesNoDialog::showTime( _lang("140147").c_str() , CCCallFunc::create(this, callfunc_selector(DragonBattleJoinView::onCostGoldBack)), lastTime, _lang("104903").c_str(), false);
            }
            else {
                if (m_gold > 0) {
                    if (GlobalData::shared()->playerInfo.gold < m_gold) {
                        YesNoDialog::gotoPayTips();
                        return;
                    }
                }
                QueueController::getInstance()->startCCDQueue(m_qid, "", false, m_gold,"",1);
            }
            return ;
        }
    }else{
        auto& info = FunBuildController::getInstance()->getFunbuildById(buildId);
        this->retain();
        PopupViewController::getInstance()->removeAllPopupView();
        PopupViewController::getInstance()->addPopupInView(BuildingHospitalPopUpView::create(info));
        this->release();
    }
}

void DragonBattleJoinView::onCostGoldBack(){
    if (m_gold > 0) {
        if (GlobalData::shared()->playerInfo.gold < m_gold) {
            YesNoDialog::gotoPayTips();
            return;
        }
    }
    QueueController::getInstance()->startCCDQueue(m_qid, "", false, m_gold,"",1);
}

void DragonBattleJoinView::onClickInfo(CCObject *pSender, CCControlEvent event){
   PopupViewController::getInstance()->addPopupView(AgainstInfoView::create());
}

void DragonBattleJoinView::onWarRankClick(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->addPopupInView(DragonHistoryView::create());
}

void DragonBattleJoinView::onClickHelp(CCObject *pSender, CCControlEvent event){
    CCCommonUtils::showHelpShiftSingleFAQ("45216");
    //PopupViewController::getInstance()->addPopupInView(DragonStatisticsView::create());
}

void DragonBattleJoinView::onManageClick(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->addPopupInView(DragonManageView::create());
}

bool DragonBattleJoinView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_joinBtn", CCControlButton*, this->m_joinBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoBtn", CCControlButton*, this->m_infoBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_historyBtn", CCControlButton*, this->m_historyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_helpBtn", CCControlButton*, this->m_helpBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_manageBtn", CCControlButton*, this->m_manageBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_battleTile", CCLabelIF*, this->m_battleTile);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_battleDesc", CCLabelIF*, this->m_battleDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_startTime", CCLabelIF*, this->m_startTime);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_endTime", CCLabelIF*, this->m_endTime);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_fixbg", CCSprite*, this->m_fixbg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_moveNode", CCNode*, this->m_moveNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_dragonBg", CCSprite*, this->m_dragonBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_startNode", CCNode*, this->m_startNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_endNode", CCNode*, this->m_endNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pipeiNode", CCNode*, this->m_pipeiNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pipeiTxt", CCLabelIF*, this->m_pipeiTxt);
    return false;
}
SEL_CCControlHandler DragonBattleJoinView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickJoin", DragonBattleJoinView::onClickJoin);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onManageClick", DragonBattleJoinView::onManageClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onWarRankClick", DragonBattleJoinView::onWarRankClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickHelp", DragonBattleJoinView::onClickHelp);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickInfo", DragonBattleJoinView::onClickInfo);
    
    return NULL;
}