//
//  ScoreTableView.cpp
//  IF
//
//  Created by 付彬 on 16/1/5.
//
//

#include "ScoreTableView.hpp"
#include "ActivityController.h"
#include "FunBuildController.h"
#include "TipsView.h"
#include "PopupViewController.h"
#include "ChestUseView.h"
#include "ParticleController.h"
#include "GuideController.h"
#include "UIComponent.h"
#include "ToolController.h"
#include "RewardController.h"
#include "SingleScoreActiveView.hpp"
#include "AllianceScoreActiveView.hpp"
#include "CountryScoreActiveView.hpp"

ScoreTableView* ScoreTableView::create(ActivityEventObj* info)
{
    auto ret = new ScoreTableView();
    if (ret && ret->init(info)) {
        ret->autorelease();
    } else {
        CC_SAFE_RELEASE(ret);
    }
    return ret;
}

bool ScoreTableView::init(ActivityEventObj* info)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    
    m_actEventObj = info;
    auto tmpCCB = CCBLoadFile("SingleScoreTableView", this, this);
    setContentSize(tmpCCB->getContentSize());
    float extH = getExtendHeight();
    m_bodyNode->setPositionY(m_bodyNode->getPositionY()-extH);
    
    m_topScrollView = CCScrollView::create(m_tabList->getContentSize());
    m_topScrollView->setDirection(kCCScrollViewDirectionHorizontal);
    m_tabList->addChild(m_topScrollView);
    m_topScrollView->setTouchPriority(Touch_Popup);
    m_btnNode->removeFromParent();
    m_topScrollView->addChild(m_btnNode);
    m_topScrollView->setContentSize(m_btnNode->getContentSize());
    
    m_actHeroObj = NULL;
    m_actAllianceObj = NULL;
    m_actCountryObj = NULL;
    CCArray * m_data = ActivityController::getInstance()->activityArr;
    if (true) {
        int num = m_data->count();
        int curT = GlobalData::shared()->getWorldTime();
        for (int i=0; i<num; i++)
        {
            auto info = dynamic_cast<ActivityEventObj*>(m_data->objectAtIndex(i));
            if (info && (info->type==10 || info->type==12 || info->type==14) )
            {
                if (curT < info->endTime) {
                    if (info->type == 10) {
                        m_actHeroObj = info;
                    }else if (info->type == 12) {
                        m_actAllianceObj = info;
                    }else if (info->type == 14) {
                        m_actCountryObj = info;
                    }
                }
            }
        }
    }
    if (m_actHeroObj && m_actHeroObj->type == m_actEventObj->type) {
        m_actEventObj = m_actHeroObj;
    }else if (m_actAllianceObj && m_actAllianceObj->type == m_actEventObj->type) {
        m_actEventObj = m_actAllianceObj;
    }else if(m_actCountryObj && m_actCountryObj->type == m_actEventObj->type) {
        m_actEventObj = m_actCountryObj;
    }
    onUpdateView();
    return true;
}

void ScoreTableView::onEnter()
{
    CCNode::onEnter();
    setTitleName(_lang("150350"));
}
void ScoreTableView::onExit()
{
    CCNode::onExit();
}

SEL_CCControlHandler ScoreTableView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHeroBtnClick", ScoreTableView::onHeroBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAllianceBtnClick", ScoreTableView::onAllianceBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCountryBtnClick", ScoreTableView::onCountryBtnClick);
    return NULL;
}

bool ScoreTableView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bodyNode", CCNode*, this->m_bodyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_heroBtn", CCControlButton*, this->m_heroBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceBtn", CCControlButton*, this->m_allianceBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_countryBtn", CCControlButton*, this->m_countryBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode", CCNode*, this->m_btnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tabList", CCNode*, this->m_tabList);
    return false;
}

void ScoreTableView::onHeroBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    if (m_actHeroObj==NULL) {
        CCCommonUtils::flyHint("", "", _lang(""));
        return;
    }
    m_actEventObj = m_actHeroObj;
    onUpdateView();
}

void ScoreTableView::onAllianceBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    if (m_actAllianceObj==NULL) {
        CCCommonUtils::flyHint("", "", _lang(""));
        return;
    }
    m_actEventObj = m_actAllianceObj;
    onUpdateView();
}

void ScoreTableView::onCountryBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    if (m_actCountryObj==NULL) {
        CCCommonUtils::flyHint("", "", _lang(""));
        return;
    }
    m_actEventObj = m_actCountryObj;
    onUpdateView();
}

void ScoreTableView::onUpdateView()
{
    if (m_actEventObj==NULL) {
        return;
    }
    
    m_bodyNode->removeAllChildren();
    if (m_actEventObj->type == 10) {
        auto view = SingleScoreActiveView::create(m_actEventObj, true);
        m_bodyNode->addChild(view);
    }else if (m_actEventObj->type == 12) {
        auto view = AllianceScoreActiveView::create(m_actEventObj, true);
        m_bodyNode->addChild(view);
    }else if (m_actEventObj->type == 14) {
        auto view = CountryScoreActiveView::create(m_actEventObj, true);
        m_bodyNode->addChild(view);
    }
}
