//
//  IFShakeGuideLayer.cpp
//  IF
//
//  Created by wangdianzhen on 15/8/19.
//
//

#include "IFShakeGuideLayer.h"
#include "WorldMapView.h"
#include "UIComponent.h"
#define  modelWidth (CCCommonUtils::isIosAndroidPad()? 1536 : 670)
IFShakeGuideLayer* IFShakeGuideLayer::create(bool forceShake)
{
    IFShakeGuideLayer *pRet = new IFShakeGuideLayer(forceShake);
    if (pRet && pRet->initData())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool IFShakeGuideLayer::initData()
{
    this->setVisible(false);
    m_defaultWidth = 140;
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_defaultWidth = 240;
    }
    CCLoadSprite::doResourceByCommonIndex(500, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(500, false);
    });
    
    CCBLoadFile("shakeGuideLayer",this,this);
    string infoStr("");
    int guideTime = CCUserDefault::sharedUserDefault()->getIntegerForKey(MSG_SHAKEGUIDETIME, 0);
    
    string shakePicStr = GlobalData::shared()->shakePicStr;
    vector<string> picVec ;
    CCCommonUtils::splitString(shakePicStr, "|", picVec);
    
    int count = picVec.size();
    int index = -1;
    if(count > 0 ){
        index = rand() % count;
    }
    if (m_forceShake) {
        string picStr = "g007.png";
        auto m_pic= CCLoadSprite::createSprite(picStr.c_str());
        CCCommonUtils::setSpriteMaxSize(m_pic, 100);
        m_head->addChild(m_pic);
    }
    if (index != -1) {
        m_headImgNode = HFHeadImgNode::create();
        m_headImgNode->initHeadImgUrl2(m_head, CCCommonUtils::getShakePicUrl(picVec[index]), 1.0f, 100, true);
        CCLOG("shakelog: picUrl%s",CCCommonUtils::getShakePicUrl(picVec[index]).c_str());
    }
    schedule(schedule_selector(IFShakeGuideLayer::monitorPic), 0.5);
    
    m_startTime  = WorldController::getInstance()->getTime()/1000;
    if (guideTime == 0) {
        infoStr = _lang("113100");
    }else if (guideTime == 1){
        infoStr = _lang("113108");
    }else if (guideTime == 2){
        infoStr = _lang("113103");
    }else{
        int randContainerArr[] = {113100,113101,113102,113103,113108,113109};
        int randNUm = rand() % 6;
        CCLOG("shakelog: randnum  %d",randNUm);
        infoStr = _lang(CC_ITOA(randContainerArr[randNUm]));
    }
    if (m_forceShake) {
        infoStr = _lang("113103");
    }
    m_hintText->setString(infoStr.c_str());
    
    CCUserDefault::sharedUserDefault()->setIntegerForKey(MSG_SHAKEGUIDETIME, ++guideTime);

    m_shakeSp->stopAllActions();
    
    CCRotateBy * roaction = CCRotateBy::create(0.1f,60);
    CCRepeat * r1 = CCRepeat::create(CCSequence::create(roaction,roaction->reverse(),nullptr),4);
    CCDelayTime * d = CCDelayTime::create(2.0f);
    m_shakeSp->runAction(CCRepeatForever::create(CCSequence::create(r1,d,nullptr)));
    
    m_bgNode->setVisible(false);
    m_bgNode->retain();
    m_bgNode->removeFromParent();
    m_clipNode = CCClipNode::create(modelWidth, 150);
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_clipNode = CCClipNode::create(modelWidth, 320);
    }
    m_clipNode->setAnchorPoint(ccp(0, 0));
    m_clipNode->addChild(m_bgNode);
    m_bgNode->release();
    this->addChild(m_clipNode);
//    auto btnPic1 = CCLoadSprite::createScale9Sprite("Mail_btn03.png");
//    if(btnPic1){
//        btnPic1->setScale(0.8);
//        m_btnNode->addChild(btnPic1);
//    }
    return true;
}

void IFShakeGuideLayer::onEnter() {
    setTouchEnabled(true);
    CCNode::onEnter();
}
void IFShakeGuideLayer::onExit() {
    setTouchEnabled(false);
    CCNode::onExit();
}

bool IFShakeGuideLayer::onTouchBegan(CCTouch* pTouch,CCEvent* pEvent){
    if(!isTouchInside(m_touchNode, pTouch)){
        return true;
    }
    if (!WorldController::getInstance()->isInWorld) {
        return false;
    }
    if(WorldMapView::instance()->isMiniMapModel()){
        return false;
    }
    if(isTouchInside(UIComponent::getInstance()->m_miniBG, pTouch)){
        return true;
    }
    if(isTouchInside(WorldMapView::instance()->m_directionIcon, pTouch) && WorldMapView::instance()->m_directionIcon->isVisible()){
        return true;
    }
    if(isTouchInside(UIComponent::getInstance()->m_coordBG, pTouch)){
        return true;
    }
    if(isTouchInside(UIComponent::getInstance()->m_faveBG, pTouch)){
        return true;
    }
    if(isTouchInside(UIComponent::getInstance()->m_skillBG, pTouch)){
        return true;
    }
    if(isTouchInside(UIComponent::getInstance()->m_goHomeBtn, pTouch) && !GlobalData::shared()->playerInfo.isInSelfServer()){
        return true;
    }
    if (isTouchInside(btnPic1, pTouch)) {
        return true;
    }
    return false;
}
bool IFShakeGuideLayer::clickFlag(){
    if (m_forceShake == false) {
        return true;
    }
    else if (m_forceShake == true) {
        if (m_clickCount == 0) {
            m_clickCount++;
            return false;
        }
        else if (m_clickCount ==1) {
            return true;
        }
    }
}

void IFShakeGuideLayer::NotShowAgain(){
    string key = CCUserDefault::sharedUserDefault()->getStringForKey(MSG_NOSHOWSHAKE, "");
    if (key == "") {
        CCUserDefault::sharedUserDefault()->setStringForKey(MSG_NOSHOWSHAKE, "shakeNomoreshow");
        UserDefault::getInstance()->flush();
    }
    removeOneself(0);
}

void IFShakeGuideLayer::onTouchEnded(CCTouch* pTouch,CCEvent* pEvent){
    if(!isTouchInside(m_touchNode, pTouch) && clickFlag()){
        unscheduleAllSelectors();
        removeOneself(0);
        return;
    }
    if(isTouchInside(btnPic1, pTouch)){
        NotShowAgain();
        return;
    }
}
SEL_CCControlHandler IFShakeGuideLayer::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool IFShakeGuideLayer::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_head", CCNode*, this->m_head);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_btnNode", CCNode*, this->m_btnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintText", CCLabelIF*, this->m_hintText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_shakeSp", CCSprite*, this->m_shakeSp);
    return false;
}

void IFShakeGuideLayer::removeOneself(float t){
    if (this->getParent()) {
        this->removeFromParent();
    }
}

void IFShakeGuideLayer::onGameTick(float time){
    int height = m_clipNode->getContentSize().height;
    m_defaultWidth = m_defaultWidth + 100;
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_defaultWidth += 100;
    }
    if(m_defaultWidth >= modelWidth){
        unschedule(schedule_selector(IFShakeGuideLayer::onGameTick));
        scheduleOnce(schedule_selector(IFShakeGuideLayer::removeOneself), 8.0);
        btnPic1 = CCLoadSprite::createScale9Sprite("notShow_btn.png"); //显示”不再显示“按钮
        CCLabelTTF *labelshow = nullptr;
        if (CCCommonUtils::isIosAndroidPad()) {
            labelshow = CCLabelTTF::create(_lang("113992"),"",20);
        }else{
            labelshow = CCLabelTTF::create(_lang("113992"),"",18);
        }
        if(btnPic1){
            btnPic1->setScaleY(1.0);
            btnPic1->setAnchorPoint(ccp(1, 0));
            labelshow->setAnchorPoint(ccp(1, 0));
            labelshow->setColor(ccc3(255,238,222));
            btnPic1->setContentSize(labelshow->getContentSize() + CCSize(30,0));
            if (CCCommonUtils::isIosAndroidPad()) {
                btnPic1->setPosition(ccp(850,12));
                labelshow->setPosition(ccp(835,12));
            }else{
                btnPic1->setPosition(ccp(390,12));
                labelshow->setPosition(ccp(375,12));
            }
            m_bgNode->addChild(btnPic1);
            m_bgNode->addChild(labelshow);
        }
        return;
    }
    m_clipNode->setContentSize(CCSizeMake(m_defaultWidth, height));
    m_bgNode->setVisible(true);
}

bool IFShakeGuideLayer::isPicBack(){
if(m_head->getChildrenCount() > 0)
    {
        return true;
    }
else{
        return false;
    }
}
void IFShakeGuideLayer::monitorPic(float time){
    if (isPicBack()) {
        this->setVisible(true);
        unschedule(schedule_selector(IFShakeGuideLayer::monitorPic));
        schedule(schedule_selector(IFShakeGuideLayer::onGameTick), 0.01);
    }
    else{
        if(WorldController::getInstance()->getTime() / 1000 - m_startTime > 10 ){
            unschedule(schedule_selector(IFShakeGuideLayer::monitorPic));
            CCUserDefault::sharedUserDefault()->setIntegerForKey(MSG_SHAKETIME, -1);
            CCUserDefault::sharedUserDefault()->flush();
            this->removeFromParent();
        }
    }
    
}