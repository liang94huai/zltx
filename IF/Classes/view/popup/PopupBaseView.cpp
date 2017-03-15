//
//  PopupBaseView.cpp
//  IF
//
//  Created by 邹 程 on 13-8-29.
//
//

#include "PopupBaseView.h"
#include "PopupViewController.h"
#include "SceneController.h"
#include "UIComponent.h"
bool IsQuitGame();

void PopupBaseView::closeSelf() {
    PopupViewController::getInstance()->removePopupView(this);
}
bool PopupBaseView::init(bool islua,int luaInViewType) {
    m_bopenAnim = false;
    m_isAniComplete = true;
    m_isSystemMail = false;
    m_isDetectMailFromAndroid = false;
    m_isHDPanel = false;
    m_changeAniType = 0;
    m_opacity = 175;
    _modelLayer = CCModelLayerColor::create();
    _modelLayer->setOpacity(m_opacity);
    _modelLayer->setColor(ccBLACK);
    _modelLayer->setContentSize(CCDirector::sharedDirector()->getWinSize());
    _modelLayer->setAnchorPoint(ccp(0.5, 0.5));
    nativeType = DEFAULT;
    this->addChild(_modelLayer,-1);
    
    if (CCCommonUtils::isIosAndroidPad() && CCCommonUtils::getIsHDViewPort())
    {
        CCCommonUtils::setIsHDViewPort(false);
    }
    m_isLua=islua;
    m_luaInViewType=luaInViewType;
    setLua(m_isLua);
    return true;
}

PopupBaseView * PopupBaseView::create()
{
	PopupBaseView * pRet = new PopupBaseView();
    if (pRet)
    {
        pRet->autorelease();
    }
	return pRet;
}

PopupBaseView::~PopupBaseView()
{
    if(  IsQuitGame() == false && Ref::ms_bEnableAutoRelease )
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_POPUP_CLOSE_EVENT,CCInteger::create(_id));
}

void PopupBaseView::onExit()
{
        if(m_isDetectMailFromAndroid)
        {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_POPUP_VIEW_OUT);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCENE_CHANGED);
        }
        m_isDetectMailFromAndroid = false;
        CCAniNode::onExit();
}

void PopupBaseView::setModelLayerTouchCallback(std::function<void(cocos2d::CCTouch *pTouch)> function) {
    if (_modelLayer) {
        _modelLayer->setTouchFunction(function);
    }
}

void PopupBaseView::setModelLayerDisplay(bool isDisplay) {
    if (_modelLayer) {
        _modelLayer->setVisible(isDisplay);
    }
}

void PopupBaseView::setModelLayerOpacity(int opacity) {
    m_opacity = opacity;
    if (_modelLayer) {
        _modelLayer->setOpacity(opacity);
    }
}

void PopupBaseView::setModelLayerColor(ccColor3B color) {
    if (_modelLayer) {
        _modelLayer->setColor(color);
    }
}

void PopupBaseView::setModelLayerTouchPriority(int _priority)
{
    if (_modelLayer) {
        _modelLayer->setTouchPriority(_priority);
    }
}

void PopupBaseView::removeModelLayer() {
    this->removeChild(_modelLayer, true);
    _modelLayer = NULL;
}

void PopupBaseView::setModelLayerPosition(cocos2d::CCPoint position) {
    _modelLayer->setPosition(position);
}

void PopupBaseView::setModelLayerContentSize(cocos2d::CCSize size) {
    _modelLayer->setContentSize(size);
}
void PopupBaseView::PlayCloseAnim(){
    float x = this->getPositionX()-30;
    float y = this->getPositionY();
    //this->setPositionX(this->getPositionX()+this->getContentSize().width);
    CCMoveTo* userMove = CCMoveTo::create(0.4f, ccp(x,y));
    this->runAction(CCSequence::create(
                                       userMove
                                       , CCCallFunc::create(this, callfunc_selector(PopupBaseView::whenPalyFinishAnim))
                                       , NULL
                                       ));

}
void PopupBaseView::whenPalyFinishAnim(){
    this->setPositionX(this->getPositionX()+30);
    this->setPositionY(this->getPositionY());
    this->getParent()->removeChild(this,false);
   // CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_LAST_POPUP_CLOSE_EVENT);

}
void PopupBaseView::addToLayer(int x, int y){
    if(m_useAnimation){

        m_isAniComplete = false;
        if(x == -1 && y == -1){
            x = CCDirector::sharedDirector()->getWinSize().width / 2;
            y = CCDirector::sharedDirector()->getWinSize().height / 2;
        }
        m_x = x;
        m_y = y;
        m_isAniComplete = false;
        this->setScale(0.001f);
        float time = 0.15;
        this->setPosition(ccp(x, y));
        CCActionInterval* scaleIn = CCScaleTo::create(time, 1.0f);
        CCMoveTo* moveTo = CCMoveTo::create(time, ccp(0, CCDirector::sharedDirector()->getWinSize().height));
        if(this->getAnchorPoint().x < 0.1){
            if(this->getAnchorPoint().y > 0.9){
                moveTo = CCMoveTo::create(time, ccp(0, CCDirector::sharedDirector()->getWinSize().height));
            }else{
                moveTo = CCMoveTo::create(time, ccp(0, 0));
            }
        }else{
            _modelLayer->setOpacity(0);
            moveTo = CCMoveTo::create(time, ccp(CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2));

        }
        CCSpawn *spawn = CCSpawn::create(scaleIn, moveTo, NULL);
        CCEaseIn *out = CCEaseIn::create(spawn, 2.0f);
        this->runAction(CCSequence::create(
                                           out
                                           , CCCallFunc::create(this, callfunc_selector(PopupBaseView::setAniFlag))
                                           , NULL
                                           ));
        
        

//        m_isAniComplete = false;
//        if(_modelLayer){
//            _modelLayer->setOpacity(0);
//            this->_modelLayer->runAction(CCFadeTo::create(0.1, 80));
//        }
//        this->runAction(CCFadeTo::create(0.1, 255));
//        this->runAction(CCSequence::create(CCScaleTo::create(0.05*this->getScale(), 1.15*this->getScale())
//                                           ,CCScaleTo::create(0.05*this->getScale(), this->getScale())
//                                           , CCCallFunc::create(this, callfunc_selector(PopupBaseView::setAniFlag))
//                                           ,NULL
//                                           ));
    }else if(m_bopenAnim){
        float x = this->getPositionX();
        float y = this->getPositionY();
        this->setPositionX(this->getPositionX()+this->getContentSize().width);
        CCMoveTo* userMove = CCMoveTo::create(0.3f, ccp(x,y));
        this->runAction(CCSequence::create(
                                           CCEaseSineOut::create(userMove)
                                           , CCCallFunc::create(this, callfunc_selector(PopupBaseView::playOpenFinish))
                                           , NULL
                                           ));
     //   m_bopenAnim = false;
        
    }
    else if (m_changeAniType > 0)
    {
        float x = this->getPositionX();
        float y = this->getPositionY();
        if (m_changeAniType == PopupViewController::ChangePopType::cNEXT)
        {
            this->setPositionX(this->getPositionX() + this->getContentSize().width);
        }
        else if (m_changeAniType == PopupViewController::ChangePopType::cPREVIOUS)
        {
            this->setPositionX(this->getPositionX() - this->getContentSize().width);
        }
        MoveTo* move = MoveTo::create(0.3f, Vec2(x,y));
        this->runAction(CCSequence::create(
                                           EaseSineOut::create(move)
                                           , NULL
                                           ));
    }
}
void PopupBaseView::playOpenFinish(){
    //CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_POPUP_OPEN_EVENT);
}
void PopupBaseView::setReturnCloseAnim(){
    float x = this->getPositionX()+this->getContentSize().width;
    float y = this->getPositionY();
  //  this->setPositionX(this->getPositionX()+this->getContentSize().width);
    CCMoveTo* userMove = CCMoveTo::create(0.4f, ccp(x,y));
    this->runAction(CCSequence::create(
                                       userMove
                                       , CCCallFunc::create(this, callfunc_selector(PopupBaseView::setAniFlag))
                                       , NULL
                                       ));
    m_bopenAnim = false;

}
void PopupBaseView::setReturnPlayAnim(){
    float x = this->getPositionX()+100;
    float y = this->getPositionY();
    this->setPositionX(this->getPositionX()-100);
    CCMoveTo* userMove = CCMoveTo::create(0.2f, ccp(x,y));
    this->runAction(CCSequence::create(
                                       CCEaseSineOut::create(userMove)
                                       , CCCallFunc::create(this, callfunc_selector(PopupBaseView::playReturnFinish))
                                       , NULL
                                       ));
   // m_bopenAnim = false;

}
void PopupBaseView::playReturnFinish(){
    this->setPositionX(0);
}
void PopupBaseView::setAniFlag(){
    if(_modelLayer){
        _modelLayer->setOpacity(m_opacity);
    }

    m_isAniComplete = true;
}

bool PopupBaseView::isAniComplete(){
    return m_isAniComplete;
}

void PopupBaseView::removeFromLayer(bool returnFlag,bool isNewMailListPopup){
    if(m_useAnimation){
        float time = 0.15;
        _modelLayer->setOpacity(0);
        CCActionInterval* scaleIn = CCScaleTo::create(time, 0.01);
        CCMoveTo* moveTo = CCMoveTo::create(time, ccp(m_x, m_y));
        
        CCSpawn *spawn = CCSpawn::create(scaleIn, moveTo, NULL);
        CCEaseOut *out = CCEaseOut::create(spawn, 2.0f);
        this->runAction(CCSequence::create(
                                           out
                                           , CCCallFunc::create(this, callfunc_selector(PopupBaseView::onRemoveAniCallBack))
                                           , NULL
                                           ));

//        if(_modelLayer){
//            this->_modelLayer->runAction(CCFadeTo::create(0.1, 0));
//        }
//        this->runAction(CCSequence::create(CCScaleTo::create(0.1*this->getScale(), 1.15*this->getScale())
//                                           ,CCCallFunc::create(this, callfunc_selector(PopupBaseView::onRemoveAnimationPlayBack))
//                                           ,NULL
//                                           ));
    //}if(m_bopenAnim){
       // setReturnCloseAnim();
    }
    else if(isNewMailListPopup)
    {
        CCLOGFUNC("isNewMailListPopup");
        _modelLayer->setOpacity(0);
        CCLOGFUNCF("width %f",getContentSize().width);
        float x = this->getPositionX() + this->getContentSize().width;
        float y = this->getPositionY();
//        CCMoveTo* userMove = CCMoveTo::create(0.3f, ccp(x,y));
        CCDelayTime* delay = CCDelayTime::create(0.1f);
//        onRemoveAnimationPlayBack();
        this->runAction(CCSequence::create(
//                                           CCEaseSineOut::create(userMove)
                                           delay
                                           , CCCallFunc::create(this, callfunc_selector(PopupBaseView::onRemoveAniCallBack))
                                           , NULL
                                           ));
    }
    else{
        onRemoveAnimationPlayBack(returnFlag);
    }
}

void PopupBaseView::onRemoveAniCallBack()
{
    onRemoveAnimationPlayBack();
}
void PopupBaseView::onRemoveAnimationPlayBack(bool returnFlag){
    if(this->getParent()){
        if(_modelLayer){
            _modelLayer->getParent()->removeChild(_modelLayer);
            _modelLayer = NULL;
        }
        if (nativeType != DEFAULT) {
            
        }
        if( IsQuitGame() == false )
        {
            this->retain();
            this->autorelease();
        }
        this->getParent()->removeChild(this);
       
    }
    else{
        _modelLayer = NULL;
        this->removeAllChildren();
        
    }
    
    if (returnFlag) {
//        doWhenClose();
    }
    
}


void PopupBaseView::ForceClear( bool bAutoRelease )
{
    if(this->getParent()){
        if(_modelLayer){
            _modelLayer->getParent()->removeChild(_modelLayer);
            _modelLayer = NULL;
        }
        
        // 暂时先都autorelease吧
        //if( bAutoRelease )
        {
            if( IsQuitGame() == false )
            {
                this->retain();
                this->autorelease();
            }
        }
        
        this->getParent()->removeChild(this);

    }
    else{
        this->removeAllChildren();
        _modelLayer = NULL;
    }
}


void PopupBaseView::setOpenAnimation(bool b){

    this->m_bopenAnim = b;
    
}

bool PopupBaseView::getOpenAnimation()
{
    return m_bopenAnim;
}
void PopupBaseView::setUseAnimation(bool b){
    this->m_useAnimation = b;
}

void PopupBaseView::setTitleName(string _name)
{
    UIComponent::getInstance()->setPopupTitleName(_name);
}

void PopupBaseView::changeBGHeight(CCNode *control)
{
    CCSize temp = control->getContentSize();
    temp.height = getBGHeight();
    control->setContentSize(temp);
}

float PopupBaseView::getBGHeight()
{
 
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    if (CCCommonUtils::isIosAndroidPad() && CCCommonUtils::getIsHDViewPort())
    {
        return winSize.height - TOP_HEIGHT_HD;
    }
    return winSize.height - TOP_HEIGHT;
}

void PopupBaseView::changeBGMaxHeight(CCNode* control)
{

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize temp = control->getContentSize();
    temp.height = winSize.height;
    control->setContentSize(temp);
}

void PopupBaseView::setHDPanelFlag(bool flag)
{
    setIsHDPanel(flag);
}

float PopupBaseView::getExtendHeight()
{

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    if(CCCommonUtils::isIosAndroidPad()&& CCCommonUtils::getIsHDViewPort())
    {
         return winSize.height - DEF_HEIGHT_HD;
    }
    return winSize.height - DEF_HEIGHT;
}

float PopupBaseView::getTopHeight()
{
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    if(CCCommonUtils::isIosAndroidPad()&& CCCommonUtils::getIsHDViewPort())
    {
        return TOP_HEIGHT_HD;
    }
    return TOP_HEIGHT;
}

float PopupBaseView::getExtendHeightWithoutTop()
{
    return getExtendHeight() - getTopHeight();
}

void PopupBaseView::doWhenClose(){

}

void PopupBaseView::setIsHDPanel(bool flag)
{
    if (!CCCommonUtils::isIosAndroidPad())
    {
        return;
    }
    
    m_isHDPanel = flag;
    
//    if (CCCommonUtils::isIosAndroidPad() && CCCommonUtils::getIsHDViewPort()!= m_isHDPanel)
//    {
        CCCommonUtils::setIsHDViewPort(m_isHDPanel);
//    }
    _modelLayer->setContentSize(CCDirector::sharedDirector()->getWinSize());
}

void PopupBaseView::initCommonBg(){
    auto tbg = CCLoadSprite::loadResource("technology_09.png");
    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
    int maxHight = CCDirector::sharedDirector()->getWinSize().height;
    int curHight = -500;
    while (curHight<maxHight) {
        auto bg = CCLoadSprite::createSprite("technology_09.png");
        bg->setAnchorPoint(ccp(0, 1));
        bg->setPosition(ccp(0, curHight));
        curHight += bg->getContentSize().height;
        tBatchNode->addChild(bg);
        if (CCCommonUtils::isIosAndroidPad())
        {
            bg->setScale(2.4f);
        }
    }
    this->addChild(tBatchNode);
}