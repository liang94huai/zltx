//
//  BedgeComposeView.cpp
//  IF
//
//  Created by 付彬 on 16/1/7.
//
//

#include "BedgeComposeView.hpp"
#include "ActivityController.h"
#include "FunBuildController.h"
#include "TipsView.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "GuideController.h"
#include "UIComponent.h"
#include "ToolController.h"
#include "KnightTitleController.hpp"
#include "ToolController.h"

BedgeComposeView* BedgeComposeView::create(int knightId)
{
    auto ret = new BedgeComposeView();
    if (ret && ret->init(knightId)) {
        ret->autorelease();
    } else {
        CC_SAFE_RELEASE(ret);
    }
    return ret;
}

bool BedgeComposeView::init(int knightId)
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

void BedgeComposeView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
}

void BedgeComposeView::onExit()
{
    if (m_waitInterface != NULL) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    setTouchEnabled(false);
    CCNode::onExit();
}

void BedgeComposeView::retData(CCObject* param)
{
    if (m_waitInterface != NULL) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
}

CCNode* BedgeComposeView::getGuideNode(string _key)
{
    return NULL;
}

SEL_CCControlHandler BedgeComposeView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "OnTipBtnClick", BedgeComposeView::OnTipBtnClick);
    return NULL;
}

bool BedgeComposeView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_topNode", CCNode*, this->m_topNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    return false;
}

bool BedgeComposeView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_infoList, pTouch)){
    }
    return true;
}

void BedgeComposeView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    int endY = pTouch->getLocation().y;
    if (abs(endY - pTouch->getStartLocation().y) >= 20) {
        return;
    }
}

void BedgeComposeView::OnTipBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    CCCommonUtils::showHelpShiftSingleFAQ("45220");
}


/*
 * BedgeBagNode
 */
BedgeBagNode* BedgeBagNode::create(int type)
{
    auto ret = new BedgeBagNode();
    if (ret && ret->init(type)) {
        ret->autorelease();
    } else {
        CC_SAFE_RELEASE(ret);
    }
    return ret;
}

bool BedgeBagNode::init(int type)
{
    if (!CCAniNode::init()) {
        return false;
    }
    m_type = type;
    auto tmpCCB = CCBLoadFile("BedgeBag", this, this);
    setContentSize(tmpCCB->getContentSize());
    
    m_bedgeIdVec = ToolController::getInstance()->m_typeTools[19];
    m_clipNode = CCClipNode::create(m_mainNode->getContentSize().width, m_mainNode->getContentSize().height);
    m_clipNode->setPosition(m_mainNode->getPosition());
    m_mainNode->getParent()->addChild(m_clipNode);
    
    m_mainNode->removeFromParent();
    m_mainNode->setPosition(ccp(0, 0));
    m_clipNode->addChild(m_mainNode);
    
    m_page=0;
    updatePage(m_page);
    
    return true;
}

void BedgeBagNode::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
}

void BedgeBagNode::onExit()
{
    CCNode::onExit();
}
bool BedgeBagNode::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}
void BedgeBagNode::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    
}

SEL_CCControlHandler BedgeBagNode::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickLeftBtn", BedgeBagNode::onClickLeftBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickRightBtn", BedgeBagNode::onClickRightBtn);
    return NULL;
}

bool BedgeBagNode::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftBtn", CCControlButton*, this->m_leftBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightBtn", CCControlButton*, this->m_rightBtn);
    return false;
}

void BedgeBagNode::onClickLeftBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    updatePage(m_page-1);
}

void BedgeBagNode::onClickRightBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    updatePage(m_page+1);
}

void BedgeBagNode::updatePage(int page)
{
    int startIdx = page*10;
    if (startIdx >= m_bedgeIdVec.size()) {
        return;
    }
    m_mainNode->removeAllChildren();
    for (int i=0; i<10; i++)
    {
        int curIdx = startIdx+i;
        if (curIdx < m_bedgeIdVec.size()) {
            auto cell = BedgeCell::create(m_bedgeIdVec[i], m_type);
            cell->setPosition(i%5*80, (1-i/5)*80);
            m_mainNode->addChild(cell);
        }
    }
}


/*
 * BedgeCell
 */
BedgeCell* BedgeCell::create(int bedgeId, int type)
{
    auto ret = new BedgeCell();
    if (ret && ret->init(bedgeId, type)) {
        ret->autorelease();
    } else {
        CC_SAFE_RELEASE(ret);
    }
    return ret;
}

bool BedgeCell::init(int bedgeId, int type)
{
    if (!CCAniNode::init()) {
        return false;
    }
    m_bedgeId = bedgeId;
    m_type = type;
    auto tmpCCB = CCBLoadFile("BedgeCell", this, this);
    setContentSize(tmpCCB->getContentSize());
    
    auto& info = ToolController::getInstance()->getToolInfoById(m_bedgeId);
    string bgStr = CCCommonUtils::getToolBgByColor(info.color);
    string iconStr = CCCommonUtils::getIcon(CC_ITOA(m_bedgeId));
    m_numLabel->setString(CC_ITOA(info.getCNT()));
    
    auto bg = CCLoadSprite::createSprite(bgStr.c_str());
    CCCommonUtils::setSpriteMaxSize(bg, 75, true);
    m_iconNode->addChild(bg);
    
    auto icon = CCLoadSprite::createSprite(iconStr.c_str(),true,CCLoadSpriteType_GOODS);
    CCCommonUtils::setSpriteMaxSize(icon, 75, true);
    m_iconNode->addChild(icon);
    
    return true;
}

void BedgeCell::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
}
void BedgeCell::onExit()
{
    setTouchEnabled(false);
    CCNode::onExit();
}

void BedgeCell::retData(CCObject* param)
{
}

SEL_CCControlHandler BedgeCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}
bool BedgeCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchBg", CCScale9Sprite*, this->m_touchBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, this->m_numLabel);
    return false;
}

bool BedgeCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_touchBg, pTouch)){
        return true;
    }
    return false;
}

void BedgeCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(isTouchInside(m_touchBg, pTouch)){
        CCLOG("bedgeId %d", m_bedgeId);
    }
}
