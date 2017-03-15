//
//  TransitionCloud.cpp
//  IF
//
//  Created by 付彬 on 15/11/11.
//
//

#include "TransitionCloud.hpp"
#include "SceneController.h"
#include "UIComponent.h"
#include "HFViewport.h"
#include "SoundController.h"
#include "DynamicResourceController.h"

TransitionCloud *TransitionCloud::create(float dt, int type) {
    auto overLayer = CCDirector::sharedDirector()->getRunningScene();
    if (overLayer->getChildByTag(TRANSITION_SPEC_TAG)) {
        CCLOG("There is a transition cloud already!");
        return NULL;
    }
    
    auto transition = new TransitionCloud();
    if(transition && transition->init(dt, type)){
        transition->autorelease();
        if (type==1) {
            overLayer->addChild(transition,8888);
        }else {
            overLayer->addChild(transition,9999);
        }
    }else{
        CC_SAFE_DELETE(transition);
    }
    return transition;
}

bool TransitionCloud::init(float dt, int type)
{
    auto size = CCDirector::sharedDirector()->getWinSize();
    string ccbName = "Cloud";
    if (type == 1) {
        ccbName = "GuideEndView";
        CCLoadSprite::doResourceByCommonIndex(8, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(8, false);
        });
        
        auto cLy = CCLayerColor::create(ccc4(0, 0, 0, 100));
        this->addChild(cLy);
    }
    auto ccb = CCBLoadFile(ccbName.c_str(), this, this);
    if (type == 0) {
        ccb->setPosition(ccp(size.width / 2, size.height / 2 + 100));
    }else if (type == 1) {
        ccb->setPosition(ccp(size.width / 2, size.height / 2));
        if (m_parNode) {
            for (int i=1; i<=7; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("Guidend_%d",i)->getCString());
                particle->setPosition(0, 0);
                m_parNode->addChild(particle);
            }
        }
        if (m_par1Node) {
            this->scheduleOnce(schedule_selector(TransitionCloud::playVectoryPar), 0.5);
        }
    }
    m_delayTime = dt;
    m_type = type;
    return true;
}

void TransitionCloud::playVectoryPar(float t)
{
    auto particle = ParticleController::createParticle(CCString::createWithFormat("vectory_flower")->getCString());
    particle->setPosition(CCPointZero);
    m_par1Node->addChild(particle);
}

void TransitionCloud::onEnter() {
    CCNode::onEnter();
    
    SoundController::sharedSound()->playEffects(Music_Sfx_scene_change);
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(TransitionCloud::doFinishFadeIn));
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
}

void TransitionCloud::onExit() {
    CCNode::onExit();
}

void TransitionCloud::doFinishFadeIn()
{
    this->scheduleOnce(schedule_selector(TransitionCloud::playTransitionEnd), m_delayTime);
}

void TransitionCloud::playTransitionEnd(float t){
    if (m_type == 0) {
        this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(TransitionCloud::doFinish));
        this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
    }
    else if (m_type == 1) {
        this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(TransitionCloud::doFinish));
        this->getAnimationManager()->runAnimationsForSequenceNamed("Loop");
    }
}

void TransitionCloud::doFinish() {
    this->removeFromParentAndCleanup(true);
}

bool TransitionCloud::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_parNode", Node*, m_parNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_par1Node", Node*, m_par1Node);
    return false;
}
