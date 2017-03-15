//
//  NpcTalkView.cpp
//  IF
//
//  Created by fubin on 14-11-11.
//
//

#include "NpcTalkView.h"
#include "GlobalData.h"
#include "PopupViewController.h"
#include "ImperialScene.h"
#include "SceneController.h"

NpcTalkView *NpcTalkView::create(vector<string> dialogs) {
    auto ret = new NpcTalkView();
    if (ret && ret->init(dialogs)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool NpcTalkView::init(vector<string> dialogs) {
    if (PopupBaseView::init()) {
        setIsHDPanel(true);
        auto node = CCBLoadFile("NpcTalkView",this,this);
        this->setContentSize(node->getContentSize());
        m_iconNode->setPositionX(m_iconNode->getPositionX()-300);
        m_txtNode->setPositionX(m_txtNode->getPositionX()+300);
        m_dialogs = dialogs;
        idx = 0;
        m_callbackTarget = NULL;
        m_callbackFunc = NULL;
        animFinish = true;
        refreshWord();
    }
    return true;
}

void NpcTalkView::setCallback(CCObject *target,SEL_CallFunc func)
{
    m_callbackTarget = target;
    m_callbackFunc = func;
}

bool NpcTalkView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_contentText", CCLabelIF*, m_contentText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", CCLabelIF*, m_nameText);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txtNode", CCNode*, m_txtNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextNode", CCNode*, m_nextNode);
    return false;
}

SEL_CCControlHandler NpcTalkView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseClick", NewPlotView::onCloseClick);
    return NULL;
}

void NpcTalkView::onCloseClick(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->removePopupView(this);
    if (m_callbackTarget && m_callbackFunc) {
        (m_callbackTarget->*m_callbackFunc)();
    }
}

void NpcTalkView::refreshWord()
{
    if (idx >= m_dialogs.size()) {
        onCloseClick(NULL,Control::EventType::TOUCH_DOWN);
        return;
    }
    m_contentText->setString(m_dialogs[idx]);
    idx ++ ;
}

void NpcTalkView::onEnter() {
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    
    m_iconNode->runAction(CCMoveBy::create(0.2, ccp(300,0)));
    m_txtNode->runAction(CCMoveBy::create(0.2, ccp(-300,0)));
//    scheduleOnce(schedule_selector(NpcTalkView::onClose), 5);
}

void NpcTalkView::onExit() {
    setTouchEnabled(false);
    CCNode::onExit();
}

void NpcTalkView::onClose()
{
    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    if(layer) {
        layer->reSetTalkTime();
    }
    PopupViewController::getInstance()->removePopupView(this);
}

bool NpcTalkView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    return true;
}

void NpcTalkView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    wordAnim();
//    onCloseClick(NULL,Control::EventType::TOUCH_DOWN);
}

void NpcTalkView::wordAnim()
{
    if (animFinish == true) {
        animFinish = false;
        if (idx >= m_dialogs.size()) {
            refreshWord();
        }else {
            m_txtNode->runAction(Sequence::create(CCMoveBy::create(0.2, ccp(500,0)),CallFunc::create(this, callfunc_selector(NpcTalkView::refreshWord)),DelayTime::create(0.2),CCMoveBy::create(0.2, ccp(-500,0)),NULL));
            this->scheduleOnce(schedule_selector(NpcTalkView::animCallBack), 0.6);
        }
    }
}

void NpcTalkView::animCallBack(float t)
{
    animFinish = true;
}