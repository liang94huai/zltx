//
//  Person.cpp
//  IF
//
//  Created by fubin on 14-5-5.
//
//

#include "Person.h"
#include "TalkNoticeCell.h"
#include "ActivityController.h"
#include "GuideController.h"
#include "FunBuildController.h"

using namespace cocos2d;

Person* Person::create(int x, int y, CCSpriteBatchNode* batchNode, int zOrder, CCPointArray* ptArray, float duration,int pType)
{
    Person *pRet = new Person();
    if (pRet && pRet->initPerson(x,y, batchNode, zOrder, ptArray, duration,pType))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool Person::initPerson(int x, int y, CCSpriteBatchNode* batchNode, int zOrder, CCPointArray* ptArray, float duration,int pType)
{
    m_batchNode = batchNode;
    m_ptArray = ptArray;
    p_type = pType;
    m_duration = duration;
    m_x = x;
    m_y = y;
    
    m_per1Spr = CCSprite::create();
    m_per1Spr->setAnchorPoint(ccp(0,0));
    CCCommonUtils::makeEffectSpr(m_per1Spr, "xunluobing1_%d.png", 9, 0.1);
    m_batchNode->addChild(m_per1Spr);
    m_per1Spr->setZOrder(zOrder);
    
    m_per2Spr = CCSprite::create();
    m_per2Spr->setAnchorPoint(ccp(0,0));
    CCCommonUtils::makeEffectSpr(m_per2Spr, "xunluobing_%d.png", 9, 0.1);
    m_batchNode->addChild(m_per2Spr);
    m_per2Spr->setZOrder(zOrder);
    m_per1Spr->setPosition(ccp(m_x, m_y));
    m_per2Spr->setPosition(ccp(m_x, m_y));
    
    if (pType==1) {
        m_per1Spr->setVisible(false);
        m_per2Spr->setVisible(false);
    }else{
        Vector<FiniteTimeAction*> arrayOfActions1;
        Vector<FiniteTimeAction*> arrayOfActions2;
        for (int i=0; i<m_ptArray->count(); i++) {
            auto func = CCCallFuncO::create(this, callfuncO_selector(Person::changePersonState), CCInteger::create(i));
            CCActionInterval * moveBy1 = CCMoveBy::create(duration,m_ptArray->getControlPointAtIndex(i));
            CCActionInterval * moveBy2 = CCMoveBy::create(duration,m_ptArray->getControlPointAtIndex(i));
            arrayOfActions1.pushBack(func);
            arrayOfActions1.pushBack(moveBy1);
            arrayOfActions2.pushBack(moveBy2);
        }
        CCSequence* sequenceAction1 = CCSequence::create(arrayOfActions1);
        CCSequence* sequenceAction2 = CCSequence::create(arrayOfActions2);
        m_per1Spr->runAction(CCRepeatForever::create(sequenceAction1));
        m_per2Spr->runAction(CCRepeatForever::create(sequenceAction2));
    }
    canTalk = true;
    return true;
}

void Person::onEnter() {
    CCNode::onEnter();
}
void Person::onExit() {
    CCNode::onExit();
}

void Person::onPersonDelete()
{
    if(m_type > 1) {
//        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(Person::testSend), this);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TALK_REMOVE, m_per1Spr);
    }
//    if (s_type>1) {
//        CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PERSON_TALK);
//        CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PERSON_STOP_TALK);
//    }
    if (s_type == 1 || s_type == 0) {
        CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_IMPERIAL_SCENE_TIPS);
    }
    if (v_type == 0) {
        CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PERSON_VISIBLE);
    }
    m_batchNode->removeChild(m_per1Spr, true);
    m_batchNode->removeChild(m_per2Spr, true);
}

void Person::onSetType(int type)
{
    s_type = type;
    m_type = type;
//    if (s_type > 1) {//s_type > -1变为s_type > 1,0和1的情况是远古战场以及城内士兵，作特殊处理
//        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Person::openTalk), MSG_PERSON_TALK, NULL);
//        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Person::stopSendTalk), MSG_PERSON_STOP_TALK, NULL);
//    }
    if (s_type == 1 || s_type == 0) {
        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Person::personTalkOnce), MSG_IMPERIAL_SCENE_TIPS, NULL);
    }
    if (GlobalData::shared()->playerInfo.level==1 && GlobalData::shared()->playerInfo.exp==0) {
        return;
    }
    if (FunBuildController::getInstance()->getMainCityLv()<3) {
        return;
    }
//    if (ActivityController::getInstance()->activityIsOpen()) {
//        m_type = type;
//        if (m_type > 1) {
//            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(Person::testSend), this, m_type*0.5+3,kCCRepeatForever, 0.0f, false);
//        }
//    }
}

void Person::onSetVisible(int type){
    v_type = type;
    if (v_type == 0) {
        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Person::personVisible), MSG_PERSON_VISIBLE, NULL);
    }
}

void Person::personVisible(CCObject *obj) {
    int type = -1;
    if(obj==NULL || (dynamic_cast<CCInteger*>(obj)==NULL)){
        type = -1;
    }else {
        CCInteger* intObj = dynamic_cast<CCInteger*>(obj);
        type = intObj->getValue();
    }
    
    if (v_type == 0) {
        if (type == 0) {
            m_per1Spr->stopAllActions();
            m_per1Spr->setVisible(false);
            m_per2Spr->stopAllActions();
            m_per2Spr->setVisible(false);
        }
        else if (type == 1) {
            CCCommonUtils::makeEffectSpr(m_per1Spr, "xunluobing1_%d.png", 9, 0.1);
            CCCommonUtils::makeEffectSpr(m_per2Spr, "xunluobing_%d.png", 9, 0.1);
            m_per1Spr->setPosition(ccp(m_x, m_y));
            m_per2Spr->setPosition(ccp(m_x, m_y));
            m_per1Spr->setVisible(true);
            m_per2Spr->setVisible(true);
            while (m_ptArray->count() > 0) {
                m_ptArray->removeControlPointAtIndex(0);
            }
            m_ptArray->addControlPoint(ccp(200, -100));
            m_ptArray->addControlPoint(ccp(-200, 100));
            Vector<FiniteTimeAction*> arrayOfActions1;
            Vector<FiniteTimeAction*> arrayOfActions2;
            for (int i=0; i<m_ptArray->count(); i++) {
                auto func = CCCallFuncO::create(this, callfuncO_selector(Person::changePersonState), CCInteger::create(i));
                CCActionInterval * moveBy1 = CCMoveBy::create(m_duration,m_ptArray->getControlPointAtIndex(i));
                CCActionInterval * moveBy2 = CCMoveBy::create(m_duration,m_ptArray->getControlPointAtIndex(i));
                arrayOfActions1.pushBack(func);
                arrayOfActions1.pushBack(moveBy1);
                arrayOfActions2.pushBack(moveBy2);
            }
            CCSequence* sequenceAction1 = CCSequence::create(arrayOfActions1);
            CCSequence* sequenceAction2 = CCSequence::create(arrayOfActions2);
            m_per1Spr->runAction(CCRepeatForever::create(sequenceAction1));
            m_per2Spr->runAction(CCRepeatForever::create(sequenceAction2));
        }
        else if (type == 2) {
            CCCommonUtils::makeEffectSpr(m_per1Spr, "xunluobing1_%d.png", 9, 0.1);
            CCCommonUtils::makeEffectSpr(m_per2Spr, "xunluobing_%d.png", 9, 0.1);
            m_per1Spr->setPosition(ccp(m_x + 242, m_y - 121));
            m_per2Spr->setPosition(ccp(m_x + 242, m_y - 121));
            m_per1Spr->setVisible(true);
            m_per2Spr->setVisible(true);
            while (m_ptArray->count() > 0) {
                m_ptArray->removeControlPointAtIndex(0);
            }
            m_ptArray->addControlPoint(ccp(-190, 95));
            m_ptArray->addControlPoint(ccp(190, -95));
            Vector<FiniteTimeAction*> arrayOfActions1;
            Vector<FiniteTimeAction*> arrayOfActions2;
            for (int i=0; i<m_ptArray->count(); i++) {
                auto func = CCCallFuncO::create(this, callfuncO_selector(Person::changePersonState), CCInteger::create(i));
                CCActionInterval * moveBy1 = CCMoveBy::create(5,m_ptArray->getControlPointAtIndex(i));
                CCActionInterval * moveBy2 = CCMoveBy::create(5,m_ptArray->getControlPointAtIndex(i));
                arrayOfActions1.pushBack(func);
                arrayOfActions1.pushBack(moveBy1);
                arrayOfActions2.pushBack(moveBy2);
            }
            CCSequence* sequenceAction1 = CCSequence::create(arrayOfActions1);
            CCSequence* sequenceAction2 = CCSequence::create(arrayOfActions2);
            m_per1Spr->runAction(CCRepeatForever::create(sequenceAction1));
            m_per2Spr->runAction(CCRepeatForever::create(sequenceAction2));
        }
    }
}

void Person::openTalk(CCObject *obj)
{
    if (ActivityController::getInstance()->activityIsOpen() && s_type>1) {
        m_type = s_type;
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(Person::testSend), this, m_type*0.5+3,kCCRepeatForever, 0.0f, false);
    }
}

void Person::personTalkOnce(CCObject *obj)
{
    int type = -1;
    if(obj==NULL || (dynamic_cast<CCInteger*>(obj)==NULL)){
        type = -1;
    }else {
        CCInteger* intObj = dynamic_cast<CCInteger*>(obj);
        type = intObj->getValue();
    }
    
    if (type != -1 && m_type == type) {
        testSend(0.1);
    }
}

void Person::testSend(float dt)
{
    if (GuideController::share()->isInTutorial()) {
        return;
    }
    if (!canTalk) {
        return;
    }
    if(m_type==1){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TALK_CROSS_SERVER_ACTIVITY, m_per1Spr);
    }else if (m_type == 0){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TALK_ACT, m_per1Spr);
    }
}

void Person::stopSendTalk(CCObject *obj)
{
    if(m_type > 1) {
        canTalk = false;
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(Person::testSend), this);
    }
}

void Person::changePersonState(CCObject *obj)
{
    int idx = dynamic_cast<CCInteger*>(obj)->getValue();
    if (m_ptArray && idx<m_ptArray->count() && m_ptArray->count()>0 && idx>=0) {
        auto pt = m_ptArray->getControlPointAtIndex(idx);
        if (pt.x>=0 && pt.y>=0) {
            m_per1Spr->setVisible(true);
            m_per1Spr->setFlipX(true);
            m_per2Spr->setVisible(false);
        }
        else if (pt.x<=0 && pt.y>=0) {
            m_per1Spr->setVisible(true);
            m_per1Spr->setFlipX(false);
            m_per2Spr->setVisible(false);
        }
        else if (pt.x<=0 && pt.y<=0) {
            m_per1Spr->setVisible(false);
            m_per2Spr->setVisible(true);
            if (p_type!=1) {
                m_per2Spr->setFlipX(true);
            }
        }
        else if (pt.x>=0 && pt.y<=0) {
            m_per1Spr->setVisible(false);
            m_per2Spr->setVisible(true);
            if (p_type!=1) {
                m_per2Spr->setFlipX(false);
            }
        }
    }
}
