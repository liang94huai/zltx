//
//  KnightListView.cpp
//  IF
//
//  Created by 付彬 on 16/1/7.
//
//

#include "KnightListView.hpp"
#include "ActivityController.h"
#include "FunBuildController.h"
#include "TipsView.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "GuideController.h"
#include "UIComponent.h"
#include "ToolController.h"
#include "KnightTitleController.hpp"

KnightListView* KnightListView::create(int knightId)
{
    auto ret = new KnightListView();
    if (ret && ret->init(knightId)) {
        ret->autorelease();
    } else {
        CC_SAFE_RELEASE(ret);
    }
    return ret;
}

bool KnightListView::init(int knightId)
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
    
    //    changeBGHeight(m_viewBg);
    float extH = getExtendHeight();
    
    m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height+extH));
    m_scrollView = CCScrollView::create(m_infoList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_infoList->addChild(m_scrollView);
    m_scrollView->setTouchPriority(Touch_Popup);
    
    m_waitInterface = NULL;
    
    return true;
}

void KnightListView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
}

void KnightListView::onExit()
{
    if (m_waitInterface != NULL) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    setTouchEnabled(false);
    CCNode::onExit();
}

void KnightListView::retData(CCObject* param)
{
    if (m_waitInterface != NULL) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
}

CCNode* KnightListView::getGuideNode(string _key)
{
    return NULL;
}

SEL_CCControlHandler KnightListView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "OnTipBtnClick", KnightListView::OnTipBtnClick);
    return NULL;
}

bool KnightListView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_topNode", CCNode*, this->m_topNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    return false;
}

bool KnightListView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_infoList, pTouch)){
    }
    return true;
}

void KnightListView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    int endY = pTouch->getLocation().y;
    if (abs(endY - pTouch->getStartLocation().y) >= 20) {
        return;
    }
}

void KnightListView::OnTipBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    CCCommonUtils::showHelpShiftSingleFAQ("45220");
}

/*
 * KnightTitleCell
 */
KnightTitleCell* KnightTitleCell::create(int knightId)
{
    auto ret = new KnightTitleCell();
    if (ret && ret->init(knightId)) {
        ret->autorelease();
    } else {
        CC_SAFE_RELEASE(ret);
    }
    return ret;
}

bool KnightTitleCell::init(int knightId)
{
    if (!CCAniNode::init()) {
        return false;
    }
    m_knightId = knightId;
    auto tmpCCB = CCBLoadFile("KnightTitleCell", this, this);
    setContentSize(tmpCCB->getContentSize());
    
    auto& info = KnightTitleController::getInstance()->KnightTitleInfoMap[m_knightId];
    string bgStr = CCCommonUtils::getToolBgByColor(info.color);
    string iconStr = CCCommonUtils::getIcon(CC_ITOA(info.itemId));
    
    auto bg = CCLoadSprite::createSprite(bgStr.c_str());
    CCCommonUtils::setSpriteMaxSize(bg, 75, true);
    m_iconNode->addChild(bg);
    
    auto icon = CCLoadSprite::createSprite(iconStr.c_str(),true,CCLoadSpriteType_GOODS);
    CCCommonUtils::setSpriteMaxSize(icon, 75, true);
    m_iconNode->addChild(icon);
    
    m_titleLabel->setString(_lang(info.name));
    
    return true;
}

void KnightTitleCell::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
}
void KnightTitleCell::onExit()
{
    setTouchEnabled(false);
    CCNode::onExit();
}

void KnightTitleCell::retData(CCObject* param)
{
}

SEL_CCControlHandler KnightTitleCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}
bool KnightTitleCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchBg", CCScale9Sprite*, this->m_touchBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_gotoBtn", CCControlButton*, this->m_gotoBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
    return false;
}

bool KnightTitleCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_touchBg, pTouch)){
        return true;
    }
    return false;
}

void KnightTitleCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(isTouchInside(m_touchBg, pTouch)){
        CCLOG("m_knightId %d", m_knightId);
    }
}
