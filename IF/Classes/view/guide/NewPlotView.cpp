//
//  NewPlotView.cpp
//  IF
//
//  Created by 李锐奇 on 14-3-24.
//
//

#include "NewPlotView.h"
#include "SoundController.h"
#include "GuideController.h"
#include "GlobalData.h"
#include "ActivityController.h"
NewPlotView *NewPlotView::create(std::string dialogs) {
    auto ret = new NewPlotView(dialogs);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool NewPlotView::init() {
    m_curGuideId = GuideController::share()->getCurGuideID();
    auto node = CCBLoadFile("PlotViewCCB",this,this);
    this->setContentSize(node->getContentSize());
//    CCCommonUtils::setButtonTitle(m_closeBtn, _lang("3000000").c_str());
    CCCommonUtils::setButtonTitle(m_atkBtn, _lang("3000006").c_str());
    m_atkBtn->setVisible(false);
    this->refreshWord();
    SoundController::sharedSound()->playEffects(Music_Sfx_guide_speech);
    return true;
}

bool NewPlotView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_contentText", CCLabelIF*, m_contentText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_closeBtn", CCControlButton*, m_closeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", CCLabelIF*, m_nameText);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txtNode", CCNode*, m_txtNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_atkBtn", CCControlButton*, m_atkBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_content2Text", CCLabelIF*, m_content2Text);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name2Text", CCLabelIF*, m_name2Text);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_main2Node", CCNode*, m_main2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon2Node", CCNode*, m_icon2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt2Node", CCNode*, m_txt2Node);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextNode", CCNode*, m_nextNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_next2Node", CCNode*, m_next2Node);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftArr", CCSprite*, m_leftArr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightArr", CCSprite*, m_rightArr);

    return false;
}

SEL_CCControlHandler NewPlotView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseClick", NewPlotView::onCloseClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAtkClick", NewPlotView::onAtkClick);
    return NULL;
}

void NewPlotView::onCloseClick(CCObject *pSender, CCControlEvent event){
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::create("plot_1"));
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(PLOT_PLAY_OVER);
}

void NewPlotView::onAtkClick(CCObject *pSender, CCControlEvent event){

    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(PLOT_PLAY_OVER);
}

void NewPlotView::refreshWord(){
    string content = CCCommonUtils::getPropById(m_dialogs, "content");
    string name = CCCommonUtils::getPropById(m_dialogs, "name");
    string seat = CCCommonUtils::getPropById(m_dialogs, "seat");
    string at = CCCommonUtils::getPropById(m_dialogs, "at");
    string figure = CCCommonUtils::getPropById(m_dialogs, "figure");
    
    m_moveType = 1;
    vector<string> vecNames;
    CCCommonUtils::splitString(name, "|", vecNames);
    if (vecNames.size()==1) {
        m_main2Node->setVisible(false);
        this->m_nameText->setString("");//_lang(name)
        this->m_contentText->setString(_lang(content));
        
        
        if (m_dialogs == "3070510" || m_dialogs == "3080110" || m_dialogs == "3150710" || m_dialogs == "3150810") {
            m_atkBtn->setVisible(true);
            m_nextNode->setVisible(false);
            if (m_dialogs == "3080110") {
                CCCommonUtils::setButtonTitle(m_atkBtn, _lang("3000010").c_str());
            }
            
            if (m_dialogs == "3070510") {// && CCCommonUtils::isTestPlatformAndServer("guide_skip_1")
                m_atkBtn->setVisible(false);
            }
            
            if (m_dialogs == "3150710" || m_dialogs == "3150810") {
                CCCommonUtils::setButtonTitle(m_atkBtn, _lang("E100181").c_str());
            }

        }
    }
    else if (vecNames.size()==2) {
        m_moveType = 3;
        m_iconNode->setPositionX(m_iconNode->getPositionX()-600);
        m_txtNode->setPositionX(m_txtNode->getPositionX()+700);
        
        m_icon2Node->setPositionX(m_icon2Node->getPositionX()-600);
        m_txt2Node->setPositionX(m_txt2Node->getPositionX()+700);
        
        m_leftArr->setVisible(true);
        m_rightArr->setVisible(false);
        m_mainNode->setPositionY(m_mainNode->getPositionY()+170);
        vector<string> vecContent;
        CCCommonUtils::splitString(content, "|", vecContent);
        vector<string> vecFigure;
        CCCommonUtils::splitString(figure, "|", vecFigure);
        
        this->m_nameText->setString("");//_lang(vecNames[0])
        this->m_name2Text->setString("");//_lang(vecNames[1])
        
        if (vecContent.size()>=2) {
            this->m_contentText->setString(_lang(vecContent[0]));
            this->m_content2Text->setString(_lang(vecContent[1]));
        }
        
        if (vecFigure.size()>=2) {
            string figure1 = vecFigure[0] + ".png";
            auto spr = CCLoadSprite::createSprite(figure1.c_str());
            spr->setAnchorPoint(ccp(0, 0));
            spr->setFlipX(true);
            m_iconNode->addChild(spr);
            
            string figure2 = vecFigure[1] + ".png";
            auto spr2 = CCLoadSprite::createSprite(figure2.c_str());
            spr2->setAnchorPoint(ccp(0, 0));
            spr2->setFlipX(true);
            m_icon2Node->addChild(spr2);
        }
        
        m_main2Node->setVisible(true);
    }
    
    m_canClick = false;
    m_iconPt = m_iconNode->getPosition();
    m_txtPt = m_txtNode->getPosition();
}

void NewPlotView::onEnter() {
    CCNode::onEnter();
    bool bSwallowsTouches = true;
    if (true || CCCommonUtils::isTestPlatformAndServer("guide_skip_1")) {
        if (m_curGuideId == "3070100"||m_curGuideId == "3070200"||m_curGuideId == "3070300"||m_curGuideId == "3070400"||m_curGuideId == "3070500"||m_curGuideId == "3070600") {
            bSwallowsTouches = false;
        }
    }
    if (m_curGuideId == "3600200") {
        if (CCCommonUtils::isIosAndroidPad()) {
            m_contentText->setFontSize(52);
        }
        else
            m_contentText->setFontSize(24);
    }
    else {
        if (CCCommonUtils::isIosAndroidPad()) {
            m_contentText->setFontSize(45);
        }
        else
            m_contentText->setFontSize(20);
    }
    if (m_curGuideId != "") {/*3077000*/
    setSwallowsTouches(bSwallowsTouches);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1000, bSwallowsTouches);
    }
    if (m_curGuideId == "3130100" && CCCommonUtils::isTestPlatformAndServer("guide_3") && PortActController::getInstance()->m_newPalyerRDMap.size()>0 && PortActController::getInstance()->m_newLoginDay<7) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_NEW_PLAYER_RD_SHOW);
    }
    
//    this->getAnimationManager()->runAnimationsForSequenceNamed("Loop");
    m_nextNode->setVisible(false);
    playMove();
}

void NewPlotView::onExit() {
//    CCDirector::sharedDirector()->getKeypadDispatcher()->removeDelegate(this);
 if (m_curGuideId != "") {
    this->stopAllActions();
    this->unscheduleAllCallbacks();
    setTouchEnabled(false);
	}
    CCNode::onExit();
}

void NewPlotView::playMove()
{
    m_txtNode->stopAllActions();
    m_iconNode->stopAllActions();
    m_txtNode->setPosition(m_txtPt);
    m_iconNode->setPosition(m_iconPt);
  

    if (m_curGuideId=="3070200" || m_curGuideId=="3070300" || m_curGuideId=="3070400") {//自动进行下一步
        if (m_curGuideId=="3070400") {
            this->scheduleOnce(CC_CALLBACK_1(NewPlotView::canClick,this),3,m_curGuideId);
            //this->scheduleOnce(schedule_selector(NewPlotView::canClick), 3);
        }else {
            this->scheduleOnce(CC_CALLBACK_1(NewPlotView::canClick,this),2,m_curGuideId);
            //this->scheduleOnce(schedule_selector(NewPlotView::canClick), 2);
        }
    }else {
        if (true || CCCommonUtils::isTestPlatformAndServer("guide_skip_1")) {
            if (m_curGuideId == "3070500") {
               this->scheduleOnce(CC_CALLBACK_1(NewPlotView::canClick,this),2,m_curGuideId);
            }else {
                this->scheduleOnce(CC_CALLBACK_1(NewPlotView::canClick,this),1,m_curGuideId);
            }
        }else {
            this->scheduleOnce(CC_CALLBACK_1(NewPlotView::canClick,this),1,m_curGuideId);
        }
//        this->scheduleOnce(schedule_selector(NewPlotView::canClick), 1);
    }
}

void NewPlotView::canClick(float t)
{
    m_canClick = true;
    if (m_curGuideId=="3070200" || m_curGuideId=="3070300" || m_curGuideId=="3070400") {//自动进行下一步
        onCloseClick(NULL,Control::EventType::TOUCH_DOWN);
    }else {
        if (true || CCCommonUtils::isTestPlatformAndServer("guide_skip_1")) {
           
            if (m_curGuideId == "3070500") {
                onCloseClick(NULL,Control::EventType::TOUCH_DOWN);
            }
        }
    }
}

bool NewPlotView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if(this->getParent() == NULL || !isVisible()){
        return false;
    }
//    if(isTouchInside(this->m_closeBtn, pTouch)){
//        m_closeBtn->onTouchBegan(pTouch, pEvent);
//        return true;
//    }
    if (m_curGuideId == "3150700" || m_curGuideId == "3150800")
    {
        return true;
    }
    if (!m_canClick) {
        return true;
    }
    if(m_atkBtn->isVisible()){
        if (isTouchInside(this->m_atkBtn, pTouch)) {
            m_atkBtn->onTouchBegan(pTouch, pEvent);
            return true;
        }
    }
    return true;
}

void NewPlotView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
//    m_closeBtn->onTouchEnded(pTouch, pEvent);
//    onCloseClick(NULL,Control::EventType::TOUCH_DOWN);
    if (m_curGuideId == "3150700" || m_curGuideId == "3150800")
    {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_STOP_GUIDE);
        return;
    }
    if (!m_canClick) {
        return;
    }
    
    if (m_curGuideId=="3070200" || m_curGuideId=="3070300" || m_curGuideId=="3070400") {//自动进行下一步
        return;
    }
    if (true || CCCommonUtils::isTestPlatformAndServer("guide_skip_1")) {
        if (m_curGuideId == "3070500") {
            return;
        }
    }
    
    if (m_atkBtn->isVisible()) {
        if (isTouchInside(this->m_atkBtn, pTouch)) {
            m_atkBtn->onTouchEnded(pTouch, pEvent);
        }
    }
    else {
        onCloseClick(NULL,Control::EventType::TOUCH_DOWN);
    }
}

CCNode *NewPlotView::getGuideNode()
{
    if (m_atkBtn->isVisible()) {
        return m_atkBtn;
    }else {
        if (m_curGuideId == "3070500") {
            return NULL;
        }
    }
    return this->m_mainNode;
}
