//
//  TransitionCallback.cpp
//  IF
//
//  Created by 邹 程 on 13-10-24.
//
//

#include "TransitionCallback.h"
#include "SceneController.h"
#include "UIComponent.h"
#include "HFViewport.h"
#include "SoundController.h"
#include "DynamicResourceController.h"
#include "FunBuildController.h"

SceneTransition *SceneTransition::createTransition(int sceneId,bool shouldSave,bool shouldCleanUp, bool playTransition,int subSceneId, MapType mapType) {
    auto overLayer = CCDirector::sharedDirector()->getRunningScene();
    if (overLayer->getChildByTag(TRANSITION_SPEC_TAG)) {
        CCLOG("There is a transition layer already!");
        return NULL;
    }
    
    auto transition = new SceneTransition();
    if(transition && transition->init(sceneId, shouldSave, shouldCleanUp, playTransition,subSceneId, mapType)){
        transition->autorelease();
        overLayer->addChild(transition,10000,TRANSITION_SPEC_TAG);
    }else{
        CC_SAFE_DELETE(transition);
    }
    return transition;
}

bool SceneTransition::init(int sceneId,bool shouldSave,bool shouldCleanUp, bool playTransition,int subSceneId, MapType mapType) {
    // init params
    auto ccb = CCBLoadFile("Cloud", this, this);
    auto size = CCDirector::sharedDirector()->getWinSize();
    ccb->setPosition(ccp(size.width / 2, size.height / 2 + 100));
    m_sceneId = sceneId;
    m_shouldSave = shouldSave;
    m_shouldCleanUp = shouldCleanUp;
    m_playTransition = playTransition;
    m_isSendedFinishMsg = false;
    m_isTransitionMiddleFinished = false;
    m_subSceneId = subSceneId;
    m_waitForResource = false;
    m_mapType = mapType;
    if (GlobalData::shared()->isUiInti == true) {
        UIComponent::getInstance()->UIMove(false);
    }

    return true;
}

void SceneTransition::onEnter() {
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SceneTransition::handleFinishMsg), MSG_SCENE_CHANGE_FINISH, NULL);
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void SceneTransition::onExit() {
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SCENE_CHANGE_FINISH);
    CCNode::onExit();
}

void SceneTransition::start(){
    if(m_mapType == SERVERFIGHT_MAP && m_sceneId == SCENE_ID_WORLD){
        DynamicResourceController::getInstance()->checkServerFightResource();
    }
    if(m_mapType == DRAGON_MAP && m_sceneId == SCENE_ID_WORLD){
        DynamicResourceController::getInstance()->checkDragonMapResource();
    }
    SoundController::sharedSound()->playEffects(Music_Sfx_scene_change);
    if(m_playTransition){
        auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE);
        if( layer )
        {
            auto arr = layer->getChildren();
            int index = 0;
            bool hfViewFlag = false;
            int currentId = SceneController::getInstance()->currentSceneId;
            while (index < arr.size() && m_sceneId == SCENE_ID_WORLD) {
                HFViewport *hfView = dynamic_cast<HFViewport*>(arr.at(index++));
                if(hfView){
                    hfViewFlag = true;
                    hfView->changeSceneZoom();
                    break;
                }
            }
            if(!hfViewFlag){
                float s = layer->getScale();
                layer->runAction(CCScaleTo::create(0.2, s - 0.2));
            }
        }
        this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(SceneTransition::onTransitionMiddleFinish));
        this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
    }else{
        onTransitionMiddleFinish();
    }
}

void SceneTransition::update(float time){
    if(m_waitForResource){
        if(m_mapType ==SERVERFIGHT_MAP && DynamicResourceController::getInstance()->checkServerFightResource()){
            m_waitForResource = false;
            doWhenDownloadCompelete();
        }else if(m_mapType ==DRAGON_MAP && DynamicResourceController::getInstance()->checkDragonMapResource()){
            m_waitForResource = false;
            doWhenDownloadCompelete();
        }else{
            
        }
    }
}

void SceneTransition::videoPlayOverCallback()
{
    auto videoPlayer=dynamic_cast<cocos2d::experimental::ui::VideoPlayer*>(this->getChildByName("videoPlayer"));
    videoPlayer->stop();
    videoPlayer->retain();
    videoPlayer->setVisible(false);
    doWhenDownloadCompelete();
    videoPlayer->release();
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void SceneTransition::videoEventCallback(Ref* sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType){
    switch (eventType) {
        case cocos2d::experimental::ui::VideoPlayer::EventType::PLAYING:
            break;
        case cocos2d::experimental::ui::VideoPlayer::EventType::PAUSED:
            break;
        case cocos2d::experimental::ui::VideoPlayer::EventType::STOPPED:
            break;
        case cocos2d::experimental::ui::VideoPlayer::EventType::COMPLETED:
            videoPlayOverCallback();
            break;
        default:
            break;
    }
}
#endif

void SceneTransition::onPlayAnimation()
{
    auto size = CCDirector::sharedDirector()->getWinSize();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    auto videoPlayer = cocos2d::experimental::ui::VideoPlayer::create();
    videoPlayer->setPosition(Point(size.width/2 , size.height/2));
    videoPlayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    videoPlayer->setContentSize(Size(size.width , size.height));
    videoPlayer->setName("videoPlayer");
    this->addChild(videoPlayer);
    if (videoPlayer)
    {
        videoPlayer->setFileName("sounds/background/cg_newbie.mp4");
        videoPlayer->play();
    }
    videoPlayer->addEventListener(CC_CALLBACK_2(SceneTransition::videoEventCallback, this));
#endif
}

void SceneTransition::onTransitionMiddleFinish() {
    if(m_mapType == SERVERFIGHT_MAP && m_sceneId == SCENE_ID_WORLD){
        if(DynamicResourceController::getInstance()->checkServerFightResource()){
            doWhenDownloadCompelete();
        }else{
            m_waitForResource = true;
        }
    }else if(m_mapType == DRAGON_MAP && m_sceneId == SCENE_ID_WORLD){
        if(DynamicResourceController::getInstance()->checkDragonMapResource()){
            doWhenDownloadCompelete();
        }else{
            m_waitForResource = true;
        }
    }else{
        doWhenDownloadCompelete();
    }
}

void SceneTransition::doWhenDownloadCompelete(){
    SceneController::getInstance()->doLastSceneClear(m_sceneId);
    if(GlobalData::shared()->sceneResourceStepLoad == 1){
        SceneController::getInstance()->loadSceneResource(m_sceneId,m_shouldSave,m_shouldCleanUp,true,m_subSceneId, m_mapType);
    }else{
        SceneController::getInstance()->doSceneInit(m_sceneId,m_shouldSave,m_shouldCleanUp,true,m_subSceneId, m_mapType);
    }
}

void SceneTransition::handleFinishMsg(CCObject* p) {
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SCENE_CHANGE_FINISH);
    if (GlobalData::shared()->isUiInti == true) {
        UIComponent::getInstance()->UIMove(true);
    }
    if(m_playTransition){
        this->scheduleOnce(schedule_selector(SceneTransition::playTransitionEnd), 0.1f);
    }else{
        doFinish();
    }
}

void SceneTransition::playTransitionEnd(float t){
    auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE);
    
    if( layer )
    {
        layer->setScale(0.7);
        layer->runAction(CCScaleTo::create(0.25, 1.0));
    }
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(SceneTransition::doFinish));
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
}

void SceneTransition::doFinish() {
    this->removeFromParentAndCleanup(true);
}

void SceneTransition::forceTransitionFinish() {
    handleFinishMsg(NULL);
}