//
//  NewPlayerBattleScene.cpp
//  IF
//
//  Created by 付彬 on 15/11/23.
//
//

#include "NewPlayerBattleScene.hpp"
#include "SceneController.h"
#include "UIComponent.h"
#include "SoundController.h"
#include "GuideController.h"
#include "TransitionCloud.hpp"
#include "TransitionLayer.hpp"
extern float g_HIGH_FPS;
extern float g_LOW_FPS;
extern bool g_swithDynamicFPS;

bool NewPlayerBattleScene::init(){
    bool bRet = false;
    if(CCLayer::init()){
        this->setTouchEnabled(true);
        bRet = true;
        if (CCCommonUtils::isIosAndroidPad())
        {
            this->setScale(1.25);
        }
        
        setCleanFunction([](){
            std::string textName = FileUtils::getInstance()->fullPathForFilename("Imperial/Imperial_31.pvr.ccz");
            CCTexture2D *texture =  CCTextureCache::sharedTextureCache()->textureForKey(textName.c_str());
            if(texture){
                CCTextureCache::sharedTextureCache()->removeTexture(texture);
            }
        });
        
        CCLoadSprite::doResourceByPathIndex(IMPERIAL_PATH,28, true);
        
        m_atkCheNodes = CCArray::create();
        m_attackChe = CCArray::create();
        
        m_warParNodes = CCArray::create();
        m_warSFiresNodes = CCArray::create();
        m_wolfSmokeNodes = CCArray::create();
        
        CCBLoadFile("BattleSceneCCB_2",this,this, true);
        
        m_wallDesAni = CCNode::create();
        m_wallDesAni->setPosition(m_gateNode->getPosition());
        this->addChild(m_wallDesAni, 20);
        
        m_spainLayer = CCLayer::create();
        this->addChild(m_spainLayer, 25);
        
        m_warParLayer = CCLayer::create();
        this->addChild(m_warParLayer, 40);
        
        m_solParLayer = CCLayer::create();
        this->addChild(m_solParLayer, 50);
        
        initBattle();
        
    }
    return bRet;
}

void NewPlayerBattleScene::initBattle()
{
    m_mainAnimation = NULL;
    if (CCFileUtils::sharedFileUtils()->isFileExist("Spine/Imperial/guide.json") &&
        CCFileUtils::sharedFileUtils()->isFileExist("Imperial/Imperial_31.atlas"))
    {
        m_mainAnimation = new IFSkeletonAnimation("Spine/Imperial/guide.json","Imperial/Imperial_31.atlas");
        m_mainAnimation->setPosition(ccp(1020, 1260));
        m_spainLayer->addChild(m_mainAnimation);
        
        m_mainAnimation->setToSetupPose();
        spTrackEntry* entry = m_mainAnimation->setAnimation(0, "guide", true);
        m_mainAnimation->setTimeScale(entry->endTime/2);
        m_mainAnimation->autorelease();
    }
    
    for (int i=0; i<m_atkCheNodes->count(); i++) { //攻击 车兵
        auto node = dynamic_cast<CCNode*>(m_atkCheNodes->objectAtIndex(i));
        if (true) {
            auto animationObj = new IFSkeletonAnimation("Spine/Imperial/tsc.json","Imperial/Imperial_30.atlas");
            if (animationObj) {
                animationObj->setVisibleStop(false);
                animationObj->setPosition(node->getPosition());
                m_spainLayer->addChild(animationObj);
                spTrackEntry* entry = animationObj->setAnimation(0, "tsc_dj", true);
                m_attackChe->addObject(animationObj);
                animationObj->setScale(0.9);
                animationObj->autorelease();
            }
        }
    }
    
    onShowWarParticle();
}

void NewPlayerBattleScene::onEnter(){
    CCLayer::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(NewPlayerBattleScene::goBack), GUIDE_BEGIN1, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(NewPlayerBattleScene::noticeToGen), GUIDE_BEGIN2, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(NewPlayerBattleScene::aniToBegin3), GUIDE_BEGIN3, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(NewPlayerBattleScene::skipBegin1), GUIDE_SKIP_BEGIN1, NULL);
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(NewPlayerBattleScene::removeSelf), GUIDE_BATTLE_END, NULL);
    //添加背景音乐
    SoundController::sharedSound()->playBGMusic(Music_Begin_Music);
    GuideController::share()->IsStartAni = true;
    UIComponent::getInstance()->setVisible(false);
    
    if( g_swithDynamicFPS ) {
        Director::getInstance()->setAnimationInterval(g_HIGH_FPS);
    }
}
void NewPlayerBattleScene::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GUIDE_BEGIN1);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GUIDE_BEGIN2);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GUIDE_BEGIN3);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GUIDE_SKIP_BEGIN1);
//    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GUIDE_BATTLE_END);
    CCLayer::onExit();
    if( g_swithDynamicFPS ) {
        Director::getInstance()->setAnimationInterval(g_LOW_FPS);
    }
}

void NewPlayerBattleScene::goBack(cocos2d::CCObject *obj) //引导第一步开始
{
    goBack(0.0);
}
void NewPlayerBattleScene::goBack(float _time)
{
    CCDictionary* dic = CCDictionary::create();
    dic->setObject(CCString::create("5.0"), "time");
    dic->setObject(CCString::create("2140"), "x");
    dic->setObject(CCString::create("760"), "y");
    dic->setObject(CCString::create("1.0"), "scale");
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MOVE_TO_POSITION,dic);
    this->scheduleOnce(schedule_selector(NewPlayerBattleScene::guidSoldier), 7.5);
}

void NewPlayerBattleScene::guidSoldier(float _time) //引导第一步结束
{//跑动停下来之后，弹出剧情面板
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("begin1"));
}

void NewPlayerBattleScene::noticeToGen(cocos2d::CCObject *obj)//播放黑骑士攻城 第二步
{
    noticeToGen(0.0);
}
void NewPlayerBattleScene::noticeToGen(float _time)
{
    SoundController::sharedSound()->playEffects(Music_Sfx_guide_attack_city);
    this->scheduleOnce(schedule_selector(NewPlayerBattleScene::startPlayChe), 0.5);
}

void NewPlayerBattleScene::startPlayChe(float _time)
{
    if (m_mainAnimation)
    {
        m_mainAnimation->setToSetupPose();
        spTrackEntry* entry = m_mainAnimation->setAnimation(0, "guide_dKnight", true);
        m_mainAnimation->setTimeScale(entry->endTime/8);
    }
    
    for (int m=0; m<m_attackChe->count(); m++) {
        IFSkeletonAnimation* animationObj = dynamic_cast<IFSkeletonAnimation*>(m_attackChe->objectAtIndex(m));
        if (animationObj) {
            spTrackEntry* entry = animationObj->setAnimation(0, "tsc_gj", true);
            animationObj->setTimeScale(entry->endTime/2.0f);
        }
    }
    this->scheduleOnce(schedule_selector(NewPlayerBattleScene::playBigFireBall), 0.7);

    this->scheduleOnce(schedule_selector(NewPlayerBattleScene::moveScreen11), 1);
    this->scheduleOnce(schedule_selector(NewPlayerBattleScene::playShake), 3.5);
//    this->scheduleOnce(schedule_selector(NewPlayerBattleScene::moveScreen12), 4.3);
    
    this->scheduleOnce(schedule_selector(NewPlayerBattleScene::addCoverLayer), 5); // 2秒钟后 播放云彩
    this->scheduleOnce(schedule_selector(NewPlayerBattleScene::moveScreen1), 6); //2.5秒后，切换场景， 隐藏自己
    this->scheduleOnce(schedule_selector(NewPlayerBattleScene::atBattleAttackEnd), 6.5);// 3秒后，进行下一步引导
}
void NewPlayerBattleScene::moveScreen11(float _time)
{
    CCDictionary* dic = CCDictionary::create();
    dic->setObject(CCString::create("2.5"), "time");
    dic->setObject(CCString::create("1800"), "x");
    dic->setObject(CCString::create("1000"), "y");
    dic->setObject(CCString::create("1.5"), "scale");
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MOVE_TO_POSITION,dic);
}

void NewPlayerBattleScene::moveScreen12(float _time)
{
    CCDictionary* dic = CCDictionary::create();
    dic->setObject(CCString::create("3.0"), "time");
    dic->setObject(CCString::create("1780"), "x");
    dic->setObject(CCString::create("1000"), "y");
    dic->setObject(CCString::create("1.5"), "scale");
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MOVE_TO_POSITION,dic);
}

void NewPlayerBattleScene::addCloud(float _time)//播放云彩
{
    TransitionCloud::create();
}
void NewPlayerBattleScene::moveScreen1(float _time)
{
    CCDictionary* dic = CCDictionary::create();
    dic->setObject(CCString::create("0.5"), "time");
    if (CCCommonUtils::isIosAndroidPad())
    {
        dic->setObject(CCString::create("1000"), "x");
    }
    else
        dic->setObject(CCString::create("940"), "x");
    
    dic->setObject(CCString::create("1040"), "y");
    dic->setObject(CCString::create("1.0"), "scale");
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MOVE_TO_POSITION,dic);
    
    if (m_mainAnimation)
    {
        m_mainAnimation->setToSetupPose();
        spTrackEntry* entry = m_mainAnimation->setAnimation(0, "guide_aKnight", true);
        m_mainAnimation->setTimeScale(entry->endTime/8);
    }
    
    this->setVisible(false);
}
void NewPlayerBattleScene::atBattleAttackEnd(float _time) //黑骑士攻城结束 第二步结束
{
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("begin2"));
}

void NewPlayerBattleScene::aniToBegin3(CCObject* obj)//开始播放 反攻黑骑士 第三步开始
{
//    TransitionCloud::create();
    TransitionLayer::create();
    
    if (m_mainAnimation) {
        m_mainAnimation->setToSetupPose();
        spTrackEntry* entry = m_mainAnimation->setAnimation(0, "guide", true);
        m_mainAnimation->setTimeScale(entry->endTime/8);
    }
    
    this->scheduleOnce(schedule_selector(NewPlayerBattleScene::moveScreen2), 1.0); //0.5秒后，切换场景， 隐藏自己
    this->scheduleOnce(schedule_selector(NewPlayerBattleScene::startAttackHei), 1.5);// 1秒后，进行下一步引导
}
void NewPlayerBattleScene::moveScreen2(float _time)
{
    CCDictionary* dic = CCDictionary::create();
    dic->setObject(CCString::create("0.5"), "time");
    if (CCCommonUtils::isIosAndroidPad())
    {
        dic->setObject(CCString::create("2300"), "x");
    }
    else
        dic->setObject(CCString::create("2000"), "x");
    
    dic->setObject(CCString::create("780"), "y");
    dic->setObject(CCString::create("1.0"), "scale");
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MOVE_TO_POSITION,dic);
    
    if (m_gateNode) {
        m_gateNode->setVisible(false);
    }
    if (CCFileUtils::sharedFileUtils()->isFileExist("Spine/Imperial/chengqiang.json") &&
        CCFileUtils::sharedFileUtils()->isFileExist("Imperial/Imperial_31.atlas"))
    {
        IFSkeletonAnimation* wallAni = new IFSkeletonAnimation("Spine/Imperial/chengqiang.json","Imperial/Imperial_31.atlas");
        wallAni->setPosition(ccp(1090, 1230));
        m_spainLayer->addChild(wallAni);
        
        wallAni->setToSetupPose();
        spTrackEntry* entry = wallAni->setAnimation(0, "chengqiang", true);
        wallAni->setTimeScale(entry->endTime/2);
        wallAni->autorelease();
    }
    
    this->setVisible(true);
}
void NewPlayerBattleScene::startAttackHei(float _time)
{
    if (m_mainAnimation)
    {
        m_mainAnimation->setToSetupPose();
        spTrackEntry* entry = m_mainAnimation->setAnimation(0, "guide_aKnight", false);
        m_mainAnimation->setTimeScale(entry->endTime/8);
    }
    //加载城门打开动画
    
    CCDictionary* dic = CCDictionary::create();
    dic->setObject(CCString::create("1.5"), "time");
    if (CCCommonUtils::isIosAndroidPad())
    {
        dic->setObject(CCString::create("2200"), "x");
    }
    else
        dic->setObject(CCString::create("1970"), "x");
    
    dic->setObject(CCString::create("800"), "y");
    dic->setObject(CCString::create("1.5"), "scale");
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MOVE_TO_POSITION,dic);
    
    //加载城门打开动画结束
    this->scheduleOnce(schedule_selector(NewPlayerBattleScene::showVectory), 7.5);
    this->scheduleOnce(schedule_selector(NewPlayerBattleScene::addCoverLayer), 11); // 2秒钟后 播放云彩
    this->scheduleOnce(schedule_selector(NewPlayerBattleScene::moveScreen3), 12.0); //0.5秒后，切换场景， 隐藏自己
    this->scheduleOnce(schedule_selector(NewPlayerBattleScene::endToBegin3), 12.5);// 1秒后，进行下一步引导
}

void NewPlayerBattleScene::showVectory(float _time)
{
    TransitionCloud::create(0.0,1);
}

void NewPlayerBattleScene::moveScreen3(float _time)
{
    CCDictionary* dic = CCDictionary::create();
    dic->setObject(CCString::create("0.5"), "time");
    if (CCCommonUtils::isIosAndroidPad())
    {
        dic->setObject(CCString::create("1200"), "x");
    }
    else
        dic->setObject(CCString::create("940"), "x");
    
    dic->setObject(CCString::create("1040"), "y");
    dic->setObject(CCString::create("1.0"), "scale");
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MOVE_TO_POSITION,dic);
    
    this->setVisible(false);
}

void NewPlayerBattleScene::endToBegin3(float _time)
{
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_BATTLE_END);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("begin3"));
    removeSelf(0.0);
}

void NewPlayerBattleScene::removeSelf(cocos2d::CCObject *obj)
{
    removeSelf(0.0);
}
void NewPlayerBattleScene::removeSelf(float _time)
{
    this->removeFromParent();
    SoundController::sharedSound()->playBGMusic(Music_M_city_1);
    GuideController::share()->IsStartAni = false;
    UIComponent::getInstance()->setVisible(true);
}


SEL_MenuHandler NewPlayerBattleScene::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

SEL_CCControlHandler NewPlayerBattleScene::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

bool NewPlayerBattleScene::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    if (pTarget == this && strncmp(pMemberVariableName, "m_atk_che",9) == 0) {
        m_atkCheNodes->addObject(pNode);
        return true;
    }
    else if (pTarget == this && strncmp(pMemberVariableName, "m_war_fire",10) == 0) {
        m_warParNodes->addObject(pNode);
        return true;
    }
    else if (pTarget == this && strncmp(pMemberVariableName, "m_war_s_fire",12) == 0) {
        m_warSFiresNodes->addObject(pNode);
        return true;
    }
    else if (pTarget == this && strncmp(pMemberVariableName, "m_wolfSmoke",11) == 0) {
        m_wolfSmokeNodes->addObject(pNode);
        return true;
    }
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_gateNode", CCNode*, this->m_gateNode);
    return false;
}

void NewPlayerBattleScene::addWarParticleToBatch(cocos2d::CCParticleSystemQuad *particle) {
    auto batchCount = m_warParVec.size();
    while (batchCount--) {
        auto &batch = m_warParVec[batchCount];
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
    m_warParLayer->addChild(newBatch);
    newBatch->addChild(particle);
    m_warParVec.push_back(newBatch);
}

void NewPlayerBattleScene::onHideWarParticle()
{
    auto batchCount = m_warParVec.size();
    while (batchCount--) {
        m_warParLayer->removeChild(m_warParVec[batchCount]);
    }
    m_warParVec.clear();
}

void NewPlayerBattleScene::onShowWarParticle()
{
    for (int i=0; i<m_warParNodes->count(); i++) {
        for (int j=1; j<=5; j++) {
            auto node = dynamic_cast<CCNode*>(m_warParNodes->objectAtIndex(i));
            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("WarFire_%d",j)->getCString());
            particle1->setPosition(node->getPosition());
            addWarParticleToBatch(particle1);
        }
    }
    
    for (int i=0; i<m_warSFiresNodes->count(); i++) {
        for (int j=2; j<=4; j++) {
            auto node = dynamic_cast<CCNode*>(m_warSFiresNodes->objectAtIndex(i));
            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",j)->getCString());
            particle1->setPosition(node->getPosition());
            addWarParticleToBatch(particle1);
        }
    }
    
    for (int i=0; i<m_wolfSmokeNodes->count(); i++) {
        for (int j=1; j<=2; j++) {
            auto node = dynamic_cast<CCNode*>(m_wolfSmokeNodes->objectAtIndex(i));
            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("WarSmoke_%d",j)->getCString());
            particle1->setPosition(node->getPosition());
            addWarParticleToBatch(particle1);
        }
    }
}

void NewPlayerBattleScene::playBigFireBall(float _time)
{
    auto startArray = CCPointArray::create(20);
    startArray->addControlPoint(ccp(2360, 480));
    startArray->addControlPoint(ccp(2480, 520));
    startArray->addControlPoint(ccp(2660, 570));
    startArray->addControlPoint(ccp(2820, 620));
    startArray->addControlPoint(ccp(2980, 670));
    
    startArray->addControlPoint(ccp(2600, 520));
    startArray->addControlPoint(ccp(2700, 570));
    startArray->addControlPoint(ccp(2550, 620));
    startArray->addControlPoint(ccp(2650, 670));
    
    auto endArray = CCPointArray::create(20);
    endArray->addControlPoint(ccp(1640, 860));
    endArray->addControlPoint(ccp(1750, 920));
    endArray->addControlPoint(ccp(1860, 980));
    endArray->addControlPoint(ccp(1980, 1050));
    endArray->addControlPoint(ccp(2070, 1080));
    
    endArray->addControlPoint(ccp(1800, 900));
    endArray->addControlPoint(ccp(1900, 1050));
    endArray->addControlPoint(ccp(1700, 840));
    endArray->addControlPoint(ccp(1830, 970));
    
    vector<float> delayTimeVec;
    delayTimeVec.push_back(0);
    delayTimeVec.push_back(0.3);
    delayTimeVec.push_back(0);
    delayTimeVec.push_back(0.3);
    delayTimeVec.push_back(0);
    
    delayTimeVec.push_back(2.3);
    delayTimeVec.push_back(2);
    delayTimeVec.push_back(4.3);
    delayTimeVec.push_back(4);
    
    vector<float> moveTimeVec;
    moveTimeVec.push_back(2.0);
    moveTimeVec.push_back(2.1);
    moveTimeVec.push_back(2.2);
    moveTimeVec.push_back(2.1);
    moveTimeVec.push_back(2.0);
    
    moveTimeVec.push_back(2.1);
    moveTimeVec.push_back(2.2);
    moveTimeVec.push_back(2.1);
    moveTimeVec.push_back(2.0);
    
    int zorder = 3000;
    m_guideParticleNode = ParticleController::createParticleBatch();
    this->addChild(m_guideParticleNode, zorder-10);
    
    for (int i=0; i<9; i++)
    {
        auto tmpNode = CCNode::create();
        tmpNode->setPosition(startArray->getControlPointAtIndex(i));
        this->addChild(tmpNode, zorder+i);
        
        for (int j=1; j<=3; j++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("Fireball_%d",j)->getCString());
            particle->setPositionType(kCCPositionTypeRelative);
            
            if (j==1) {
                particle->setPosition(startArray->getControlPointAtIndex(i));
                m_guideParticleNode->addChild(particle);
                
                CCActionInterval * delay1 = CCDelayTime::create(delayTimeVec[i]);
                CCActionInterval * moveTo1 = CCMoveTo::create(moveTimeVec[i], endArray->getControlPointAtIndex(i));
                CCActionInterval * easeSineOut1 = CCEaseSineOut::create(moveTo1);
                auto func1 = CCCallFuncO::create(this, callfuncO_selector(NewPlayerBattleScene::playEndBigBall1), particle);
                particle->runAction(CCSequence::create(delay1, easeSineOut1, func1, NULL));
                
            }else {
                tmpNode->addChild(particle);
            }
        }
        
        CCActionInterval * delay = CCDelayTime::create(delayTimeVec[i]);
        CCActionInterval * moveTo = CCMoveTo::create(moveTimeVec[i], endArray->getControlPointAtIndex(i));
        CCActionInterval * easeSineOut = CCEaseSineOut::create(moveTo);
        auto func = CCCallFuncO::create(this, callfuncO_selector(NewPlayerBattleScene::playEndBigBall), tmpNode);
        tmpNode->runAction(CCSequence::create(delay, easeSineOut, func, NULL));
    }
}

void NewPlayerBattleScene::playEndBigBall(CCObject *obj)
{
    CCNode* node = dynamic_cast<CCNode*>(obj);
    if (node) {
        for (int j=1; j<=5; j++) {
            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("Boom_%d",j)->getCString());
            particle1->setPosition(node->getPosition());
            addWarParticleToBatch(particle1);
        }
//        for (int j=1; j<=8; j++) {
//            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("Collapse_%d",j)->getCString());
//            particle1->setPosition(node->getPosition());
//            addWarParticleToBatch(particle1);
//        }
//        for (int j=1; j<=5; j++) {
//            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("Collapse_U_%d",j)->getCString());
//            particle1->setPosition(node->getPosition());
//            addWarParticleToBatch(particle1);
//        }
//        for (int j=1; j<=4; j++) {
//            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("Collapse_L_%d",j)->getCString());
//            particle1->setPosition(node->getPosition());
//            addWarParticleToBatch(particle1);
//        }
        
        for (int j=1; j<=5; j++) {
            auto particle1 = ParticleController::createParticle(CCString::createWithFormat("WarFire_%d",j)->getCString());
            particle1->setPosition(node->getPosition()+ccp(50,0));
            addWarParticleToBatch(particle1);
        }
        
        node->stopAllActions();
        node->removeFromParent();
    }
}

void NewPlayerBattleScene::playEndBigBall1(CCObject* obj)
{
    CCNode* node = dynamic_cast<CCNode*>(obj);
    if (node) {
        node->removeFromParent();
    }
}

void NewPlayerBattleScene::skipBegin1(CCObject *obj)
{
    this->scheduleOnce(schedule_selector(NewPlayerBattleScene::addCloud), 0.1); //播放云彩
    this->scheduleOnce(schedule_selector(NewPlayerBattleScene::moveScreen1), 1.1); //1秒后，切换场景， 隐藏自己
    this->scheduleOnce(schedule_selector(NewPlayerBattleScene::endBegin1), 1.6);//1.6秒后，进行下一步引导
}

void NewPlayerBattleScene::endBegin1(float t)
{
    GuideController::share()->setGuide("3600400");
}

void NewPlayerBattleScene::addCoverLayer(float t)//播放渐隐渐显
{
    TransitionLayer::create();
}

void NewPlayerBattleScene::playShake(float _time)
{
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_VIEWPORT_SHAKE);
}