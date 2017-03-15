//
//  WallBuild.cpp
//  IF
//
//  Created by fubin on 14-7-25.
//
//

#include "WallBuild.h"
#include "SceneController.h"
#include "SoundController.h"
#include "PopupViewController.h"
#include "ScienceListView.h"
#include "WatchtowerView.h"
#include "MainCityView.h"
#include "FunBuildController.h"
#include "BuildingHospitalPopUpView.h"
#include "AllianceMarketView.h"
#include "BattleView.h"
#include "QueueController.h"
#include "ToolController.h"
#include "WorldController.h"
#include "ImperialScene.h"

#include "TileOpenView.h"
#include "CityDefenseView.h"

using namespace cocos2d;

WallBuild* WallBuild::create(int type)
{
    WallBuild *pRet = new WallBuild();
    if (pRet && pRet->initWallBuild(type))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool WallBuild::initWallBuild(int type)
{
    m_backWalls = CCArray::create();
    m_frontWalls = CCArray::create();
    m_touchNodes = CCArray::create();
    
    if(true)
    {
        m_mainNode = CCNode::create();
        this->addChild(m_mainNode);
        string ccbName = "pic_wall";
        ccbName = ccbName+CC_ITOA(type);
        auto& info = FunBuildController::getInstance()->getFunbuildById(FUN_BUILD_WALL_ID);
        
        int lv = 1;
//        for (int i=0; i<info.picVec.size(); i++) {
//            int blv = atoi(info.picVec[i].c_str());
//            if (info.level<=blv) {
//                lv = i+1;
//                break;
//            }
//            else {
//                if (i == info.picVec.size()-1) {
//                    lv = 5;
//                }
//            }
//        }
        
        ccbName = ccbName+"_"+CC_ITOA(lv);
        CCBLoadFile(ccbName.c_str(),m_mainNode,this);
        
        m_effectSpr = CCSprite::create();
    }
    this->setAnchorPoint(ccp(0,0));
    m_fTouchNode->setVisible(false);
    
    m_signNode = CCNode::create();
    m_upEffectNode = CCNode::create();
    return true;
}

void WallBuild::setNamePos(int x, int y, CCLayer* sginLayer, CCSpriteBatchNode* backBatchNode, CCSpriteBatchNode* frontBatchNode, int zOrder)
{
    parentX = x;
    parentY = y;
    
    m_signLayer = sginLayer;
    m_backBatchNode = backBatchNode;
    m_frontBatchNode = frontBatchNode;
    
    m_signNode->setPosition(ccp(x,y));
    m_upEffectNode->setPosition(ccp(x,y));
    sginLayer->addChild(m_signNode);
    sginLayer->addChild(m_upEffectNode);
    
    for (int i=0; i<m_backWalls->count(); i++) {
        CCSprite* spr = dynamic_cast<CCSprite*>(m_backWalls->objectAtIndex(i));
        spr->removeFromParent();
        spr->getTexture()->setAntiAliasTexParameters();
        spr->setPosition(ccp(spr->getPositionX()+parentX, spr->getPositionY()+parentY));
        m_backBatchNode->addChild(spr);
    }
    
    for (int i=0; i<m_frontWalls->count(); i++) {
        CCSprite* spr = dynamic_cast<CCSprite*>(m_frontWalls->objectAtIndex(i));
        spr->removeFromParent();
        spr->getTexture()->setAntiAliasTexParameters();
        spr->setPosition(ccp(spr->getPositionX()+parentX, spr->getPositionY()+parentY));
        m_frontBatchNode->addChild(spr);
    }
    
    drowEffectSpr(zOrder, 0);
    drowPersonSpr(zOrder, 0);
}

void WallBuild::onBuildDelete()
{
    if (m_signLayer) {
        m_signLayer->removeChild(m_signNode);
        m_signLayer->removeChild(m_upEffectNode);
        
        auto batchCount = m_parVec.size();
        while (batchCount--) {
            m_signLayer->removeChild(m_parVec[batchCount]);
        }
        m_parVec.clear();
    }
    
    for (int i=0; i<m_backWalls->count(); i++) {
        CCSprite* spr = dynamic_cast<CCSprite*>(m_backWalls->objectAtIndex(i));
        m_backBatchNode->removeChild(spr, true);
    }
    m_backWalls->removeAllObjects();
    
    for (int i=0; i<m_frontWalls->count(); i++) {
        CCSprite* spr = dynamic_cast<CCSprite*>(m_frontWalls->objectAtIndex(i));
        m_frontBatchNode->removeChild(spr, true);
    }
    m_frontWalls->removeAllObjects();
}

void WallBuild::onEnter() {
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(WallBuild::playShadow), MSG_PLAY_SHADOW, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(WallBuild::stopShadow), MSG_STOP_SHADOW, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(WallBuild::setGary), MSG_SET_GARY, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(WallBuild::setWhite), MSG_SET_WHITE, NULL);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(WallBuild::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
}

void WallBuild::onExit() {
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PLAY_SHADOW);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_STOP_SHADOW);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SET_GARY);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SET_WHITE);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(WallBuild::onEnterFrame), this);
    CCNode::onExit();
}

bool WallBuild::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    bool ret = false;
    for (int i=0; i<m_touchNodes->count(); i++) {
        CCNode* touchNode = dynamic_cast<CCNode*>(m_touchNodes->objectAtIndex(i));
        if (touchNode && isTouchInside(touchNode, pTouch)) {
            return true;
        }
    }
    if (m_fTouchNode && m_fTouchNode->isVisible() && isTouchInside(m_fTouchNode, pTouch)) {
        PopupViewController::getInstance()->addPopupInView(CityDefenseView::create(FUN_BUILD_MAIN_CITY_ID));
        return true;
    }
    
    return ret;
}

void WallBuild::setGary(CCObject* obj)
{
    for (int i=0; i<m_backWalls->count(); i++) {
        CCSprite* spr = dynamic_cast<CCSprite*>(m_backWalls->objectAtIndex(i));
        if (spr) {
            spr->setColor(ccGRAY);
        }
    }
    
    for (int i=0; i<m_frontWalls->count(); i++) {
        CCSprite* spr = dynamic_cast<CCSprite*>(m_frontWalls->objectAtIndex(i));
        if (spr) {
            spr->setColor(ccGRAY);
        }
    }
}

void WallBuild::setWhite(CCObject* obj)
{
    for (int i=0; i<m_backWalls->count(); i++) {
        CCSprite* spr = dynamic_cast<CCSprite*>(m_backWalls->objectAtIndex(i));
        if (spr) {
            spr->setColor(ccWHITE);
        }
    }
    
    for (int i=0; i<m_frontWalls->count(); i++) {
        CCSprite* spr = dynamic_cast<CCSprite*>(m_frontWalls->objectAtIndex(i));
        if (spr) {
            spr->setColor(ccWHITE);
        }
    }
}

void WallBuild::playShadow(CCObject* obj)
{
    for (int i=0; i<m_backWalls->count(); i++) {
        CCSprite* spr = dynamic_cast<CCSprite*>(m_backWalls->objectAtIndex(i));
        if (spr && spr->isVisible()) {
            CCTintTo* tinto1 = CCTintTo::create(0.5, 166, 166, 166);
            CCTintTo* tinto2 = CCTintTo::create(0.5, 255, 255, 255);
            CCSequence* seq = CCSequence::create(tinto1,tinto2,NULL);
            spr->runAction( CCRepeatForever::create(seq) );
        }
    }
    
    for (int i=0; i<m_frontWalls->count(); i++) {
        CCSprite* spr = dynamic_cast<CCSprite*>(m_frontWalls->objectAtIndex(i));
        if (spr && spr->isVisible()) {
            CCTintTo* tinto1 = CCTintTo::create(0.5, 166, 166, 166);
            CCTintTo* tinto2 = CCTintTo::create(0.5, 255, 255, 255);
            CCSequence* seq = CCSequence::create(tinto1,tinto2,NULL);
            spr->runAction( CCRepeatForever::create(seq) );
        }
    }
}

void WallBuild::stopShadow(CCObject* obj)
{
    for (int i=0; i<m_backWalls->count(); i++) {
        CCSprite* spr = dynamic_cast<CCSprite*>(m_backWalls->objectAtIndex(i));
        if (spr) {
            spr->stopAllActions();
            spr->setColor(ccWHITE);
        }
    }
    
    for (int i=0; i<m_frontWalls->count(); i++) {
        CCSprite* spr = dynamic_cast<CCSprite*>(m_frontWalls->objectAtIndex(i));
        if (spr) {
            spr->stopAllActions();
            spr->setColor(ccWHITE);
        }
    }
}

void WallBuild::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isCanClick) {
        return;
    }
    isCanClick=false;
    setGary(NULL);
    scheduleOnce(schedule_selector(WallBuild::onClickThis), 0.1f);
}

void WallBuild::onClickThis(float _time)
{
    setWhite(NULL);
    
    scheduleOnce(schedule_selector(WallBuild::onCanClick), 0.2f);
}

void WallBuild::onCanClick(float _time)
{
    isCanClick = true;
}

void WallBuild::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
}

void WallBuild::canShowState()
{
}

void WallBuild::drowEffectSpr(int zOrder, int tmpOrd)
{
}

void WallBuild::drowPersonSpr(int zOrder, int tmpOrd)
{
}

#pragma mark -
#pragma mark CocosBuilder Part
SEL_MenuHandler WallBuild::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

SEL_CCControlHandler WallBuild::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

bool WallBuild::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    if (pTarget == this && strncmp(pMemberVariableName, "m_backWall",10) == 0) {
        m_backWalls->addObject(pNode);
        return true;
    }
    else if (pTarget == this && strncmp(pMemberVariableName, "m_frontWall",11) == 0) {
        m_frontWalls->addObject(pNode);
        return true;
    }
    else if (pTarget == this && strncmp(pMemberVariableName, "m_touchNode",11) == 0) {
        m_touchNodes->addObject(pNode);
        return true;
    }
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fTouchNode", CCNode*, this->m_fTouchNode);
    return false;
}

void WallBuild::addParticleToBatch(cocos2d::CCParticleSystemQuad *particle) {
    auto batchCount = m_parVec.size();
    while (batchCount--) {
        auto &batch = m_parVec[batchCount];
        if (batch) {
            auto batchBlend = batch->getBlendFunc();
            auto particleBlend = particle->getBlendFunc();
            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                batch->addChild(particle);
                return;
            }
        }
    }
    auto newBatch = ParticleController::createParticleBatch();
    m_signLayer->addChild(newBatch);
    newBatch->addChild(particle);
    m_parVec.push_back(newBatch);
}

void WallBuild::onShowParticle()
{
}

void WallBuild::onHideParticle()
{
    auto batchCount = m_parVec.size();
    while (batchCount--) {
        m_signLayer->removeChild(m_parVec[batchCount]);
    }
    m_parVec.clear();
}

void WallBuild::onPlayUnlock()
{
    isCanClick = false;
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(WallBuild::playFadeOut));
    this->getAnimationManager()->runAnimationsForSequenceIdTweenDuration(1, 0);
    onShowParticle();
}

void WallBuild::playFadeOut()
{
    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    if( layer )
        layer->onRemoveSpeBuild();
    onHideParticle();
}

void WallBuild::onEnterFrame(float dt)
{
    unsigned int fireRate = 0;
    if (GlobalData::shared()->cityTileCountry == NEUTRALLY) {
        fireRate = GlobalData::shared()->worldConfig.fire[2];
    }
    else {
        fireRate = GlobalData::shared()->worldConfig.fire[0];
    }
    
    auto now = WorldController::getInstance()->getTime();
    unsigned int fix = 0;
    if (GlobalData::shared()->cityStartFireStamp < GlobalData::shared()->cityFireStamp) {
        double start = MIN(now, GlobalData::shared()->cityFireStamp);
        double duration = start - GlobalData::shared()->cityStartFireStamp;
        fix = (unsigned int)(duration * (double)fireRate / 3600000.0);
    }
    
    unsigned int cityDefMax = GlobalData::shared()->imperialInfo[FUN_BUILD_WALL_ID].para2+ CCCommonUtils::getEffectValueByNum(125);
    unsigned int cityDefFix = GlobalData::shared()->cityDefenseVal - fix;
    
    // devil 暂时关闭城墙破损动画
    if (m_st!=0 && cityDefFix >= cityDefMax) {
        m_st=0;
//        m_fTouchNode->setVisible(false);
//        this->getAnimationManager()->runAnimationsForSequenceNamed("Default");
    }else if (m_st!=1 && cityDefFix < cityDefMax) {
        m_st=1;
//        m_fTouchNode->setVisible(true);
//        this->getAnimationManager()->runAnimationsForSequenceNamed("Fire");
    }
}
