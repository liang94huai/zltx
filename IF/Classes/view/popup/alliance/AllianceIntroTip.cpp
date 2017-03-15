//
//  AllianceIntroTip.cpp
//  IF
//
//  Created by ganxiaohua on 14-11-27.
//
//

#include "AllianceIntroTip.h"
#include "CCMathUtils.h"
#include "FunBuildController.h"
#include "PopupViewController.h"
#include "VipDetailView.h"

AllianceIntroTip *AllianceIntroTip::create(bool up,CCPoint pos,string tipStr,string icon){
    AllianceIntroTip *ret = new AllianceIntroTip(up,pos,tipStr,icon);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
AllianceIntroTip *AllianceIntroTip::createAutoAdjust(AllianceIntroTip_Direction dir,string tipStr,int maxWidth,int arrowOffsetX){
    AllianceIntroTip *ret = new AllianceIntroTip(dir,CCPointZero,tipStr);
    if(ret && ret->initAutoAdjust(maxWidth,arrowOffsetX)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
bool AllianceIntroTip::init(){
    auto ccbNode = CCBLoadFile("AllianceIntroTip", this, this);
    this->setContentSize(ccbNode->getContentSize());
    m_tip->setString(m_tipStr);
    int textH = m_tip->getContentSize().height*m_tip->getOriginScaleY() +40;
    if(m_up){
        m_arrow->setRotation(90);
    }else{
        m_arrow->setRotation(-90);
    }
    m_arrow->setPosition(m_pos);
    
    if(m_icon!="" && m_iconNode){
        m_tip->setDimensions(CCSize(380, 0));
        textH = MAX(textH,200);
        auto spr = CCLoadSprite::createSprite(m_icon.c_str());
        spr->setPositionY(textH/2);
        m_iconNode->addChild(spr);
        CCCommonUtils::setSpriteMaxSize(spr, 120);
        m_bg->setInsetBottom(30);
        m_bg->setInsetTop(30);
        m_bg->setInsetLeft(30);
        m_bg->setInsetRight(30);
        m_bg->setContentSize(CCSize(520,textH));
        m_tip->setPosition(ccp(120 + 380/2, textH/2));
        m_tip->setHorizontalAlignment(kCCTextAlignmentLeft);
    }else if(textH>m_bg->getContentSize().height){
        m_bg->setInsetBottom(30);
        m_bg->setInsetTop(30);
        m_bg->setInsetLeft(30);
        m_bg->setInsetRight(30);
        m_bg->setContentSize(CCSize(m_bg->getContentSize().width,textH));
        m_tip->setPositionY(textH/2);
        if(m_up){
            m_arrow->setPosition(ccp(m_pos.x, textH + 10));
        }else{
            m_arrow->setPosition(ccp(m_pos.x, m_arrow->getPositionY()));
        }
        
    }
    
    m_isAutoAjust=false;
    return true;
}
bool AllianceIntroTip::initAutoAdjust(int maxWidth,int arrowOffset){
//    CCLoadSprite::doResourceByImperialIndex(22, true);
//    setCleanFunction([](){
//        CCLoadSprite::doResourceByImperialIndex(22, false);
//    });
    
    m_touchNode = CCNode::create();
    this->addChild(m_touchNode);
    
    m_bg = CCLoadSprite::createScale9Sprite("feedback_head.png");
    m_bg->setInsetBottom(30);
    m_bg->setInsetTop(30);
    m_bg->setInsetLeft(30);
    m_bg->setInsetRight(30);
    m_bg->setAnchorPoint(ccp(0.5, 0.5));
    
    auto _bg = CCLoadSprite::createSprite("feedback_head.png");
    if(_bg)
    {
        auto _size = _bg->getContentSize();
        if(m_bg)
        {
            m_bg->setCapInsets(CCRect(_size.width/2,_size.height/2, 1, 1));
        }
    }
    this->addChild(m_bg);
    
    m_arrow = CCLoadSprite::createSprite("feedback_head1.png");
    m_arrow->setPosition({0,17});
    this->addChild(m_arrow);
    CCSize size;
    if(maxWidth>0){
        size = CCSize(maxWidth,0);
        m_tip = CCLabelIF::create();
        m_tip->setColor({108,58,25});
        m_tip->setAnchorPoint(ccp(0.5,0.5));
        m_tip->setDimensions(CCSize(maxWidth-30, 0));
        m_tip->setString(m_tipStr);
        this->addChild(m_tip);
        size.height = m_tip->getContentSize().height + 20;
    }else{
        m_tip = CCLabelIF::create();
        m_tip->setAlignment(kCCTextAlignmentCenter);
        m_tip->setColor({108,58,25});
        m_tip->setAnchorPoint(ccp(0.5,0.5));
        m_tip->setString(m_tipStr);
        this->addChild(m_tip);
        size.width = m_tip->getContentSize().width + 45;
        size.height = m_tip->getContentSize().height + 45;
    }
    m_bg->setContentSize(size);
    if (CCCommonUtils::isIosAndroidPad())
    {
//        m_tip->setFontSize(40);
//         m_bg->setContentSize(CCSizeMake(size.width, size.height+45));
    }
    switch (m_direction) {
        case AllianceIntroTip_Right:
        case AllianceIntroTip_Left:{
            m_arrow->setPositionY(arrowOffset);
            break;
        }
        case AllianceIntroTip_Bottom:
        case AllianceIntroTip_Top:
        default:{
            m_arrow->setPositionX(arrowOffset);
            break;
        }
    }
    autoAjustLayout();
    m_isAutoAjust=true;
    return true;
}
void AllianceIntroTip::setAutoChangeText(std::vector<string> mTextID, int delayTime, int playTime){
    m_dialogID = mTextID;
    m_delayTime = delayTime;
    m_showTime = playTime;
    changeText();
    playAnimation();
    this->runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(m_showTime+0.3+1),CCCallFunc::create(this,callfunc_selector(AllianceIntroTip::changeText)),CCDelayTime::create(m_delayTime + 0.3-1),NULL)));
}
void AllianceIntroTip::changeText(){
    int count = m_dialogID.size();
    int iIndex = CCMathUtils::getRandomInt(0, count * 1000 - 1);
    iIndex = floor(iIndex/1000);
    std::string showTip = _lang(m_dialogID[iIndex]);
    setText(showTip);
}
void AllianceIntroTip::setAnimationAndPlay(int delayTime,int playTime){
    m_delayTime = delayTime;
    m_showTime = playTime;
    playAnimation();
}
void AllianceIntroTip::playAnimation(){
    if (m_tipsType == 4) {
        m_arrow->runAction(CCRepeatForever::create(CCSequence::create(CCCallFuncO::create(this, callfuncO_selector(AllianceIntroTip::setTouch), CCInteger::create(1)),CCDelayTime::create(m_showTime),CCFadeOut::create(0.3),CCCallFuncO::create(this, callfuncO_selector(AllianceIntroTip::setTouch), CCInteger::create(0)),CCDelayTime::create(m_delayTime),CCFadeIn::create(0.3),NULL)));
        m_bg->runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(m_showTime),CCFadeOut::create(0.3),CCDelayTime::create(m_delayTime),CCFadeIn::create(0.3),NULL)));
        m_tip->runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(m_showTime),CCFadeOut::create(0.3),CCDelayTime::create(m_delayTime),CCFadeIn::create(0.3),NULL)));
    }
    else {
        m_arrow->runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(m_showTime),CCFadeOut::create(0.3),CCDelayTime::create(m_delayTime),CCFadeIn::create(0.3),NULL)));
        m_bg->runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(m_showTime),CCFadeOut::create(0.3),CCDelayTime::create(m_delayTime),CCFadeIn::create(0.3),NULL)));
        m_tip->runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(m_showTime),CCFadeOut::create(0.3),CCDelayTime::create(m_delayTime),CCFadeIn::create(0.3),NULL)));
    }
}
void AllianceIntroTip::setText(std::string tipStr){
    m_tip->setString(tipStr);
    if(m_isAutoAjust){
        CCSize newSize;
        newSize.width = m_tip->getContentSize().width + 45;
        newSize.height = m_tip->getContentSize().height + 45;
        m_bg->setContentSize(newSize);
        autoAjustLayout();
    }
}
void AllianceIntroTip::autoAjustLayout(){
    CCSize size = m_bg->getContentSize();
    
    CCSize arrowSize = m_arrow->getContentSize();
    switch (m_direction) {
        case AllianceIntroTip_Bottom:{
            m_arrow->setRotation(180);
            m_arrow->setScaleX(-1);
            m_arrow->setScaleY(1);
            m_bg->setPosition(ccp(0, -(arrowSize.height * 0.35 + size.height*0.5 - 5)));
            m_tip->setPosition(m_bg->getPosition());
            setContentSize(CCSize(size.width, size.height + arrowSize.height));
            break;
        }
        case AllianceIntroTip_Right:{
            
            m_arrow->setRotation(-85);
            m_arrow->setScaleX(1);
            m_arrow->setScaleY(-1);
            m_arrow->setPosition(Vec2(m_arrow->getPosition()+Vec2(17, -10)));
            
            m_bg->setContentSize(Size(size.height*2,size.width));
            m_bg->setScaleX(0.5);
            m_bg->setRotation(90);
            m_bg->setPosition(ccp(arrowSize.width*0.3 + size.width * 0.5 - 5, 0));
            m_tip->setPosition(m_bg->getPosition());
            setContentSize(CCSize(arrowSize.width + size.width, size.height));
            break;
        }
        case AllianceIntroTip_Left:{
            m_arrow->setRotation(-85);
            m_arrow->setScaleX(-1);
            m_arrow->setScaleY(1);
            m_arrow->setPosition(Vec2(m_arrow->getPosition()+Vec2(-32, -5)));
            
            m_bg->setContentSize(Size(size.height*2,size.width));
            m_bg->setScaleX(0.5);
            m_bg->setRotation(90);
            m_bg->setPosition(ccp(-(arrowSize.width*0.5 + + size.width * 0.5 - 5), 0));
            m_tip->setPosition(m_bg->getPosition());
            setContentSize(CCSize(arrowSize.width + size.width, size.height));
            break;
        }
        case AllianceIntroTip_Top:
        default:{
            m_arrow->setRotation(0);
            m_arrow->setScaleX(1);
            m_arrow->setScaleY(1);
            m_bg->setPosition(ccp(0, arrowSize.height * 0.35 + size.height*0.5 - 5));
            m_tip->setPosition(m_bg->getPosition());
            setContentSize(CCSize(size.width, size.height + arrowSize.height));
            break;
        }
    }
    refreshTouchNode();
}
void AllianceIntroTip::onEnter(){
    setSwallowsTouches(true);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    CCNode::onEnter();
}
void AllianceIntroTip::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
    if (m_tipsType == 3) {
        CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_IMPERIAL_SCENE_TIPS);
    }
    m_dialogID.clear();
    m_arrow->stopAllActions();
    m_bg->stopAllActions();
    m_tip->stopAllActions();
}
SEL_CCControlHandler AllianceIntroTip::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    
    return NULL;
}

bool AllianceIntroTip::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrow", CCNode*, this->m_arrow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tip", CCLabelIF*, this->m_tip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    return false;
}

void AllianceIntroTip::refreshTouchNode()
{
    m_touchNode->setContentSize(m_bg->getContentSize());
    m_touchNode->setPosition(m_bg->getPosition() + ccp(-m_bg->getContentSize().width/2, -m_bg->getContentSize().height/2));
}

void AllianceIntroTip::setTipsType(int type)
{
    m_tipsType = type;
    if (m_tipsType == 3) {
        this->setVisible(false);
        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceIntroTip::playAnimationOnce), MSG_IMPERIAL_SCENE_TIPS, NULL);
    }
}

void AllianceIntroTip::playAnimationOnce(CCObject *obj)
{
    int type = -1;
    if(obj==NULL || (dynamic_cast<CCInteger*>(obj)==NULL)){
        type = -1;
    }else {
        CCInteger* intObj = dynamic_cast<CCInteger*>(obj);
        type = intObj->getValue();
    }
    
    if (type != -1 && m_tipsType == type) {
        this->setVisible(true);
        scheduleOnce(schedule_selector(AllianceIntroTip::onHide), 5.0);
    }
}

void AllianceIntroTip::onHide(float t)
{
    this->setVisible(false);
}

bool AllianceIntroTip::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_touchNode && isTouchInside(m_touchNode, pTouch) && m_tipsType == 4) {
        return true;
    }
    return false;
}

void AllianceIntroTip::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_touchNode && isTouchInside(m_touchNode, pTouch) && m_tipsType == 4) {
        if (m_bg->getOpacity() == 255) {
            PopupViewController::getInstance()->addPopupInView(VipDetailView::create(2));
        }
    }
}

void AllianceIntroTip::setTouch(CCObject* obj)
{
    int type = -1;
    if(obj==NULL || (dynamic_cast<CCInteger*>(obj)==NULL)){
        type = -1;
    }else {
        CCInteger* intObj = dynamic_cast<CCInteger*>(obj);
        type = intObj->getValue();
    }
    
    if (type == 0) {
        setTouchEnabled(false);
    }
    else if (type == 1) {
        setTouchEnabled(true);
    }
}
