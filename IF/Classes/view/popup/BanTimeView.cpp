//
//  BanTimeView.cpp
//  IF
//
//  Created by 三脚猫 on 15/12/17.
//
//

#include "BanTimeView.hpp"
#include "PopupViewController.h"
#include "AllianceInfoView.h"
#include "GameController.h"
#include "SetAccountView.h"
#include "NetController.h"


BanTimeView::BanTimeView()
:isCloseOnNoButton(true)
,disableWhenTouched(false)
,m_function(NULL)
,mTimeDes("")
,m_type(1)
,m_isTimeOver(false)
{
    
}

BanTimeView::~BanTimeView(){
    
}

BanTimeView* BanTimeView::create(int type ,const char *content,const char * buttonName, int timeJson){
    BanTimeView* ret = new BanTimeView();
    if(ret && ret->init(type,content,buttonName, timeJson)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool BanTimeView::init(int type ,const char *content,const char * buttonName, int timeJson){
    m_type = type;
//    this->setIsHDPanel(true);
    CCNode* _rootNode = _rootNode = CCBLoadFile("YesNoDialog", this, this);
//    auto bg = CCBLoadFile("GoldExchangeView_NEW", this, this,true);
//    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
//    this->setContentSize(winsize);
    bool bRet=false;
    if (_rootNode) {
//        mContents.clear();
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        //label可滑动，放在scrollview上
        m_contentLabel = CCLabelTTF::create();
        m_contentLabel->setDimensions(CCSize(360, 0));
        m_contentLabel->setString(content);
        m_contentLabel->setColor(ccWHITE);
        m_contentLabel->setFontSize(24);
        m_contentLabel->setVerticalAlignment(kCCVerticalTextAlignmentTop);
        m_contentLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
        m_contentLabel->setPosition(ccp(0, -10));
        int totalHeight = m_contentLabel->getContentSize().height ;/** m_contentLabel->getOriginScaleY();*/
        if(totalHeight > m_subLayer->getContentSize().height + 5){
            
            CCScrollView* scroll = CCScrollView::create(CCSize(360, 200));
            scroll->setContentSize(CCSize(360, m_contentLabel->getContentSize().height/**m_contentLabel->getOriginScaleY()*/));
            m_contentLabel->setAnchorPoint(Point(0,0));
            scroll->addChild(m_contentLabel);
            m_subLayer->addChild(scroll);
            float gapWidth = 360-m_contentLabel->getContentSize().width/**m_contentLabel->getOriginScaleX()*/;
            scroll->setPosition(ccp(gapWidth/2, 5));
            scroll->setDirection(kCCScrollViewDirectionVertical);
            scroll->setAnchorPoint(ccp(0, 0));
            scroll->setContentOffset(ccp(0, 200-m_contentLabel->getContentSize().height/**m_contentLabel->getOriginScaleY()*/));
            //            }
        }else{
            m_subNode->addChild(m_contentLabel);
            m_contentLabel->setAnchorPoint(ccp(0, 1));
            float gapWidth = 360-m_contentLabel->getContentSize().width/**m_contentLabel->getOriginScaleX()*/;
            m_contentLabel->setPosition(ccp(gapWidth/2, 0));
        }
        CCCommonUtils::setButtonTitle(m_btnOk, buttonName);
        
        this->m_btnText->setString(buttonName);
        
        CCCommonUtils::setButtonTitle(m_btnCancel, _lang("cancel_btn_label").c_str());
        this->m_goldIcon->setVisible(false);
        this->m_goldNum->setVisible(false);
        this->m_btnText->setVisible(false);
        this->m_btnCancel->setVisible(false);
        this->m_muteTimeNode->setVisible(false);
        bRet=true;
        
        // change btn priority to prevent touch swallowed by others (CCControl class must change it's DefaultTouchPriority)
        m_btnOk->setTouchPriority(-1);
        m_btnCancel->setTouchPriority(-1);
        
        m_noticeNode->setVisible(false);
        m_noticeLabel->setString(_lang("102109"));
        m_timeLabel->setString("");
        m_timeLabel->setColor(ccYELLOW);
        
//        m_time=-1;
//        m_isBuild=true;
        m_isAlliance = false;
        m_closeAll = false;
        serverUpdateInfo(content, timeJson);
    }
    return bRet;
}

void BanTimeView::serverUpdateInfo(const char * content , int timeJson)
{
    if (timeJson > 0)
    {
        m_Endtime = timeJson;
        if (m_Endtime < 0 )
        {
            m_Endtime = 0;
        }
        
        std::string timeStr = CC_SECTOA(m_Endtime);
        if (CCCommonUtils::isIosAndroidPad() && CCCommonUtils::getIsHDViewPort()) {
            m_timeLabel->setFontSize(60);
        }
        else
            m_timeLabel->setFontSize(30);
        m_timeLabel->setString(_lang_1("E100214",timeStr.c_str()));
        if (m_Endtime > 0)
        {
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(BanTimeView::onUpdateLabel), this, 1.0,kCCRepeatForever, 0.0f, false);
        }
        
    }
}

void BanTimeView::onUpdateLabel(float dt)
{
    m_Endtime--;
    if (m_Endtime <= 0)
    {
        m_Endtime = 0;
        m_isTimeOver = true;
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(BanTimeView::onUpdateLabel), this);
        if(m_type == 1)
        {
            setYesButtonTitle(_lang("E100215").c_str());
        }
        else if( m_type == 2)
        {
            setYesButtonTitle(_lang("105282").c_str());
        }
    }
    
    std::string timeStr = CC_SECTOA(m_Endtime);
    m_timeLabel->setString(_lang_1("E100214",timeStr.c_str()));
    
    
}


SEL_CCControlHandler BanTimeView::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) {
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "keyPressedBtnOk", BanTimeView::keypressedBtnOk);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "keyPressedBtnCancel", BanTimeView::keypressedBtnCancel);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickNoticeBtn", BanTimeView::onClickNoticeBtn);
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "keyPressedBtnClose", YesNoDialog::keyPressedBtnClose);
    
    return NULL;
}

bool BanTimeView::onAssignCCBMemberVariable(CCObject * pTarget, const char* pMemberVariableName, CCNode * pNode) {
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_layerColorBG", CCModelLayerColor*, this->m_layerColorBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subNode", CCNode*, this->m_subNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subLayer", CCLayer*, this->m_subLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnOk", CCControlButton*, this->m_btnOk);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnCancel", CCControlButton*, this->m_btnCancel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldNum", CCLabelIF*, this->m_goldNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnText", CCLabelIF*, this->m_btnText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldIcon", CCSprite*, this->m_goldIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resNode", CCNode*, this->m_resNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noticeNode", CCNode*, this->m_noticeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noticeBtn", CCControlButton*, this->m_noticeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noticeSpr", CCSprite*, this->m_noticeSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noticeLabel", CCLabelIF*, this->m_noticeLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprBG", CCNode*, this->m_sprBG);
    
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt1", CCLabelIF*, this->m_txt1);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt2", CCLabelIF*, this->m_txt2);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt3", CCLabelIF*, this->m_txt3);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt4", CCLabelIF*, this->m_txt4);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_box1", CCNode*, this->m_box1);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_box2", CCNode*, this->m_box2);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_box3", CCNode*, this->m_box3);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_box4", CCNode*, this->m_box4);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectSpr", CCNode*, this->m_selectSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_muteTimeNode", CCNode*, this->m_muteTimeNode);
    return false;
}

void BanTimeView::keypressedBtnOk(CCObject * pSender, Control::EventType pCCControlEvent) {
    CCLOG("---------keypressedBtnOk--------------");
    if (m_isTimeOver) {
        if (m_type == 2) {
            PopupViewController::getInstance()->addPopupInView(SetAccountNextView::create(2));
        }
        else if(m_type == 1){
            NetController::shared()->disconnect();
            NetController::shared()->setIsChangingServer(true);
            NetController::shared()->setIsLogout(true);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_RELOGIN);
        }
        keyPressedBtnClose(NULL,Control::EventType::TOUCH_DOWN);
    }
    else{
        GameController::getInstance()->quitGame();
    }
    
    
}
void BanTimeView::keypressedBtnCancel(CCObject * pSender, Control::EventType pCCControlEvent) {
    CCLOG("---------keypressedBtnCancel--------------");
    GameController::getInstance()->feedBack();
}

void BanTimeView::keyPressedBtnClose(CCObject * pSender, Control::EventType pCCControlEvent){
    
    PopupViewController::getInstance()->removePopupView(this);
    
}

void BanTimeView::onClickNoticeBtn(CCObject * pSender, Control::EventType pCCControlEvent) {
    if (m_onSel) {
        m_noticeSpr->setVisible(!m_noticeSpr->isVisible());
        m_onSel->execute();
    }
}
void BanTimeView::showCancelButton(){
    this->m_btnCancel->setVisible(true);
    this->m_btnOk->setPositionX(-m_btnCancel->getPositionX());
}
void BanTimeView::setYesButtonTitle(const char *content)
{
    CCCommonUtils::setButtonTitle(m_btnOk, content);
}

void BanTimeView::setNoButtonTitle(const char *content)
{
    CCCommonUtils::setButtonTitle(m_btnCancel, content);
}

void BanTimeView::setYesText(const char *content){
    m_btnText->setString(content);
}

void BanTimeView::onEnter()
{
    CCNode::onEnter();
//    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(YesNoDialog::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
    if (getParent() && (getParent()->getChildByTag(BUILDINGVIEW_TAG) || getParent()->getChildByTag(YESNODLG_TAG))){
        //        m_layerColorBG->setOpacity(0);
    }
    else{
        setTag(YESNODLG_TAG);
    }
}


void BanTimeView::onExit()
{
//    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(YesNoDialog::onEnterFrame), this);
    setTouchEnabled(false);
    CCNode::onExit();
}

bool BanTimeView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_muteTimeNode->isVisible())
    {
        return true;
    }
    if (isTouchInside(m_sprBG, pTouch)) {
        return false;
    }
    //    keyPressedBtnClose(NULL,Control::EventType::TOUCH_DOWN);
    return true;
}

void BanTimeView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(!isTouchInside(m_sprBG,pTouch)){
        keyPressedBtnClose(NULL,Control::EventType::TOUCH_DOWN);
        //        PopupViewController::getInstance()->removePopupView(this);
        return ;
    }
    
}
