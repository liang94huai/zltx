//
//  KnightTitileView.cpp
//  IF
//
//  Created by 付彬 on 16/1/7.
//
//

#include "KnightTitileView.hpp"
#include "ActivityController.h"
#include "FunBuildController.h"
#include "TipsView.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "GuideController.h"
#include "UIComponent.h"
#include "ToolController.h"
#include "KnightTitleController.hpp"
#include "BedgeComposeView.hpp"
#include "KnightListView.hpp"

KnightTitileView* KnightTitileView::create(int knightId)
{
    auto ret = new KnightTitileView();
    if (ret && ret->init(knightId)) {
        ret->autorelease();
    } else {
        CC_SAFE_RELEASE(ret);
    }
    return ret;
}

bool KnightTitileView::init(int knightId)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    
    m_knightId = knightId;
    auto tmpCCB = CCBLoadFile("KnightTitileView", this, this);
    setContentSize(tmpCCB->getContentSize());
    setTitleName( "" );
    
    changeBGHeight(m_buildBG);
    float extH = getExtendHeight();
    m_bottomNode->setPositionY(m_bottomNode->getPositionY()-extH);
    
    m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height+extH));
    m_scrollView = CCScrollView::create(m_infoList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_infoList->addChild(m_scrollView);
    m_scrollView->setTouchPriority(Touch_Popup);
    
    auto bedgeBage = BedgeBagNode::create(0);
    m_bottomNode->addChild(bedgeBage);
    
    m_waitInterface = NULL;
    
    return true;
}

void KnightTitileView::onEnter()
{
    CCNode::onEnter();
//    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
//    setTouchEnabled(true);
}

void KnightTitileView::onExit()
{
    if (m_waitInterface != NULL) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
//    setTouchEnabled(false);
    CCNode::onExit();
}

void KnightTitileView::retData(CCObject* param)
{
    if (m_waitInterface != NULL) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
}

CCNode* KnightTitileView::getGuideNode(string _key)
{
    return NULL;
}

SEL_CCControlHandler KnightTitileView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickListBtn", KnightTitileView::onClickListBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickComposeBtn", KnightTitileView::onClickComposeBtn);
    return NULL;
}

bool KnightTitileView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listBtn", CCControlButton*, this->m_listBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_composeBtn", CCControlButton*, this->m_composeBtn);
    return false;
}

//bool KnightTitileView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
//    if(isTouchInside(m_infoList, pTouch)){
//    }
//    return true;
//}
//
//void KnightTitileView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
//{
//    int endY = pTouch->getLocation().y;
//    if (abs(endY - pTouch->getStartLocation().y) >= 20) {
//        return;
//    }
//}

void KnightTitileView::onClickListBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    CCCommonUtils::showHelpShiftSingleFAQ("45220");
}

void KnightTitileView::onClickComposeBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    CCCommonUtils::showHelpShiftSingleFAQ("45220");
}
