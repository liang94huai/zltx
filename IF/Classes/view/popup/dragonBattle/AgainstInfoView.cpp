//
//  AgainstInfoView.cpp
//  IF
//
//  Created by ganxiaohua on 15/8/27.
//
//

#include "AgainstInfoView.h"
#include "PopupViewController.h"
#include "CommandBase.h"
#include "ActivityController.h"
#include "AllianceFlagPar.h"

AgainstInfoView *AgainstInfoView::create(){
    AgainstInfoView *ret = new AgainstInfoView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AgainstInfoView::onEnter(){
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);//Touch_Default
    PopupBaseView::onEnter();
}

void AgainstInfoView::onExit(){
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    PopupBaseView::onExit();
}

bool AgainstInfoView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void AgainstInfoView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (ccpDistance(pTouch->getStartLocation(), pTouch->getLocation())>30) {
        return ;
    }
    if (!isTouchInside(m_clickArea, pTouch)) {
        PopupViewController::getInstance()->removePopupView(this);
    }
}

bool AgainstInfoView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(205, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(7, false);
            CCLoadSprite::doResourceByCommonIndex(205, false);
        });
        auto node = CCBLoadFile("AgainstInfoView", this, this);
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        
        string icon = "Allance_flay.png";
        int rank = 1;
        if (ActivityController::getInstance()->dragonEnemy) {
            string abbr = ActivityController::getInstance()->dragonEnemy->valueForKey("abbr")->getCString();
            string name = ActivityController::getInstance()->dragonEnemy->valueForKey("name")->getCString();
            string leader = ActivityController::getInstance()->dragonEnemy->valueForKey("leader")->getCString();
            string kingName = ActivityController::getInstance()->dragonEnemy->valueForKey("kingName")->getCString();
            icon = ActivityController::getInstance()->dragonEnemy->valueForKey("icon")->getCString();
            double power = ActivityController::getInstance()->dragonEnemy->valueForKey("fighting")->doubleValue();
            rank = ActivityController::getInstance()->dragonEnemy->valueForKey("rankNum")->intValue();
            int serverId = ActivityController::getInstance()->dragonEnemy->valueForKey("kingId")->intValue();
            double wins = ActivityController::getInstance()->dragonEnemy->valueForKey("wins")->doubleValue();
            int curmember = ActivityController::getInstance()->dragonEnemy->valueForKey("curmember")->intValue();
            int maxmember = ActivityController::getInstance()->dragonEnemy->valueForKey("maxmember")->intValue();
            string allianceName = "";
            if (abbr!="") {
                allianceName.append("(");
                allianceName.append(abbr);
                allianceName.append(")");
            }
            allianceName.append(name);
            m_allianceNameTxt->setString(allianceName);
            string leaderStr = _lang_1("115015",leader.c_str());
            m_leaderTxt->setString(leaderStr.c_str());
            m_powerTxt->setString(_lang_1("140030",CC_CMDITOAL(power).c_str()));
            if (kingName=="") {
                kingName.append("#");
                kingName.append(CC_ITOA(serverId));
            }
            m_kingdomTxt->setString(_lang_1("140031",kingName.c_str()));
            m_memberTxt->setString(_lang_2("140032",CC_ITOA(curmember),CC_ITOA(maxmember)));
            m_rankTxt->setString(_lang_1("140023",CC_CMDITOAL(rank).c_str()));
            string winStr = CC_ITOA(wins*100);
            winStr.append("%");
            m_winTxt->setString(_lang_1("140033",winStr.c_str()));
            
        }
        m_infoTxt->setString(_lang("140021"));
        
        if(icon==""){
            icon = "Allance_flay";
        }
        icon.append(".png");
        m_flagNode->removeAllChildrenWithCleanup(true);
        AllianceFlagPar* flag = AllianceFlagPar::create(icon.c_str());
        flag->setScale(0.6);
        m_flagNode->addChild(flag);
        
        int rankType = 6;
        for (auto it=ActivityController::getInstance()->dragonCatalogMap.begin(); it!=ActivityController::getInstance()->dragonCatalogMap.end(); it++) {
            if (rank >= it->second.min_A &&  rank <= it->second.max_A) {
                rankType = it->second.rank;
                break;
            }
        }
        m_titleNode->removeAllChildrenWithCleanup(true);
        auto spr = CCLoadSprite::createSprite(CCString::createWithFormat("duanwei_%d.png",rankType)->getCString());
        spr->setScale(0.8);
        m_titleNode->addChild(spr);
        
        this->schedule(schedule_selector(AgainstInfoView::updateTime), 1);
        updateTime(0);
        ret = true;
    }
    return ret;
}

void AgainstInfoView::updateTime(float _time){
    long left = ActivityController::getInstance()->fightBeginTime - GlobalData::shared()->getWorldTime();
    if (left<=0) {
        left = 0;
        this->unschedule(schedule_selector(AgainstInfoView::updateTime));
    }
        m_timeTxt->setString(_lang_1("140022",CC_SECTOA(left)));
}

bool AgainstInfoView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickArea", CCNode*, this->m_clickArea);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoTxt", CCLabelIF*, this->m_infoTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_allianceNameTxt", CCLabelIF*, this->m_allianceNameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_leaderTxt", CCLabelIF*, this->m_leaderTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_powerTxt", CCLabelIF*, this->m_powerTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rankTxt", CCLabelIF*, this->m_rankTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_kingdomTxt", CCLabelIF*, this->m_kingdomTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_memberTxt", CCLabelIF*, this->m_memberTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_winTxt", CCLabelIF*, this->m_winTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_flagNode", CCNode*, this->m_flagNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleNode", CCNode*, this->m_titleNode);
    return false;
}

SEL_CCControlHandler AgainstInfoView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    
    return NULL;
}