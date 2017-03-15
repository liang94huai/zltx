//
//  IFGoogleTip.cpp
//  IF
//
//  Created by wangdianzhen on 15/8/19.
//
//

#include "IFGoogleTip.h"
#include "WorldMapView.h"
#include "UIComponent.h"
#include "UserBindCommand.h"
#include "AchievementController.h"
#include "../../Ext/CCDevice.h"
#define  modelWidth (CCCommonUtils::isIosAndroidPad()? 1536 : 670)
IFGoogleTip* IFGoogleTip::create()
{
    IFGoogleTip *pRet = new IFGoogleTip();
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

bool IFGoogleTip::initData()
{
    m_state = 0;
    m_defaultWidth = 140;
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_defaultWidth = 240;
    }

    
    CCBLoadFile("googleTip",this,this);
    m_loginLb->setString(_lang("113860"));
//    CCCommonUtils::setButtonTitle(m_loginBtn, _lang("113860").c_str());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(cocos2d::extension::CCDevice::isConnected_Platform(0)){//是否已经登录google
        m_state = 1;
        m_loginLb->setString(_lang("113861"));
//            CCCommonUtils::setButtonTitle(m_loginBtn, _lang("113861").c_str());
    }
    else
    {
        m_loginLb->setString(_lang("113860"));
//            CCCommonUtils::setButtonTitle(m_loginBtn, _lang("113860").c_str());
    }
#else
#endif
    
    
    m_secondLb->setString(_lang("113863"));
    m_thirdLb->setString(_lang("113862"));
//        CCCommonUtils::setButtonTitle(m_secondBtn, _lang("137401").c_str());
//        CCCommonUtils::setButtonTitle(m_thirdBtn, _lang("150289").c_str());
    return true;
}

void IFGoogleTip::onEnter() {
    setTouchEnabled(true);
    CCNode::onEnter();
}
void IFGoogleTip::onExit() {
    setTouchEnabled(false);
    CCNode::onExit();
}

bool IFGoogleTip::onTouchBegan(CCTouch* pTouch,CCEvent* pEvent){
//    if(isTouchInside(m_touchNode, pTouch)){
//        return true;
//    }
    return true;
}



void IFGoogleTip::onTouchEnded(CCTouch* pTouch,CCEvent* pEvent){
    
    if(!isTouchInside(m_touchNode, pTouch)){
        removeOneself();
        return;
    }
    if(isTouchInside(m_sp1, pTouch)){
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(m_state == 1){
            cocos2d::extension::CCDevice::logout_Platform(0);//已经登录，执行登出
        }
        else{
            cocos2d::extension::CCDevice::logout_Platform(0);//执行登出
            cocos2d::extension::CCDevice::login_Platform(0);
        }
#else
#endif
        removeOneself();
        return;
    }
    if(isTouchInside(m_sp2, pTouch)){
        AchievementController::getInstance()->doOpenGooglePlayAchievement();
        removeOneself();
        return;
    }
    if(isTouchInside(m_sp3, pTouch)){
        AchievementController::getInstance()->doOpenGooglePlay();
        removeOneself();
        return;
    }

    
}
SEL_CCControlHandler IFGoogleTip::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onLoginBtnClick", IFGoogleTip::onLoginBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSecondBtnClick", IFGoogleTip::onSecondBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onThirdBtnClick", IFGoogleTip::onThirdBtnClick);
    
    
    return NULL;
}
void IFGoogleTip::onLoginBtnClick(CCObject* pSender,CCControlEvent event){
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    if(m_state == 1){
//        cocos2d::extension::CCDevice::logout_Platform(0);//已经登录，执行登出
//    }
//    else{
//        cocos2d::extension::CCDevice::logout_Platform(0);//执行登出
//        cocos2d::extension::CCDevice::login_Platform(0);
//    }
//#else
//#endif
//    removeOneself();
}

void IFGoogleTip::onSecondBtnClick(CCObject* pSender,CCControlEvent event){
//    AchievementController::getInstance()->doOpenGooglePlayAchievement();
//    removeOneself();
}
void IFGoogleTip::onThirdBtnClick(CCObject* pSender,CCControlEvent event){
//    AchievementController::getInstance()->doOpenGooglePlay();
//    removeOneself();
}
bool IFGoogleTip::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loginLb", CCLabelIF*, this->m_loginLb);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_secondLb", CCLabelIF*, this->m_secondLb);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_thirdLb", CCLabelIF*, this->m_thirdLb);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", Node*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loginBtn", CCControlButton*, this->m_loginBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_secondBtn", CCControlButton*, this->m_secondBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_thirdBtn", CCControlButton*, this->m_thirdBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sp1", CCSprite*, this->m_sp1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sp2", CCSprite*, this->m_sp2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sp3", CCSprite*, this->m_sp3);

 
    
    return false;
}

void IFGoogleTip::removeOneself(){
    if (this->getParent()) {
        this->removeFromParent();
    }
}




