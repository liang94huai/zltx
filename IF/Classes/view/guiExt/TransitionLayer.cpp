//
//  TransitionLayer.cpp
//  IF
//
//  Created by 王超一 on 15/12/16.
//
//

#include "TransitionLayer.hpp"
#include "GuideController.h"
#include "HFViewport.h"
#include "SoundController.h"

bool TransitionLayer::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_contentText", CCLabelIF*, m_contentText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_colorLayer", CCLayerColor*, m_colorLayer);
    
    return false;
}

TransitionLayer *TransitionLayer::create(){
    auto overLayer = CCDirector::sharedDirector()->getRunningScene();
    if (overLayer->getChildByTag(TRANSITION_SPEC_TAG)) {
        CCLOG("There is a transition cloud already!");
        return NULL;
    }
    
    auto transition = new TransitionLayer();
    if(transition && transition->init()){
        transition->autorelease();
        overLayer->addChild(transition,29999);
    }else{
        CC_SAFE_DELETE(transition);
    }
    return transition;
}

bool TransitionLayer::init(){
    CCBLoadFile("GuidePlotViewCCB", this, this);
    m_bg->setVisible(false);
    m_contentText->setVisible(false);
    
    CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(TransitionLayer::doFinish));
    CCActionInterval * fadeTo1 = CCFadeTo::create(1.0,255);
    CCActionInterval * delay1Time = CCDelayTime::create(0.5);
    CCActionInterval * fadeTo2 = CCFadeTo::create(1.0,0);
    m_colorLayer->runAction(CCSequence::create(fadeTo1, delay1Time, fadeTo2, funcall, NULL));
    return true;
}


void TransitionLayer::onEnter() {
    CCNode::onEnter();
//    SoundController::sharedSound()->playEffects(Music_Sfx_scene_change);
}

void TransitionLayer::onExit() {
    CCNode::onExit();
}

void TransitionLayer::doFinish() {
    this->removeFromParentAndCleanup(true);
}