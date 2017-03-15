//
//  BattleReportMailResPopUpView.cpp
//  IF
//
//  Created by 李锐奇 on 14-3-26.
//
//

#include "BattleReportMailResPopUpView.h"
#include "PopupViewController.h"
#include "RoleInfoView.h"
#include "WorldController.h"
#include "SceneController.h"
#include "MailController.h"
#include "GeneralHeadPic.h"
#include "MailPopUpView.h"
#include "WorldMapView.h"
#include "WorldController.h"
#include "FunBuildController.h"
#include "RewardController.h"
#include "BattleManager.h"
#include "MailBattleDetailView.h"
#include "YesNoDialog.h"
#include "MailSaveCommand.h"
#include "DetectMailPopUpView.h"
#include "ChatController.h"
#include "CountryChatCommand.h"
#include "ChatServiceCocos2dx.h"
#include "ActivityController.h"
#include "MailWritePopUpView.h"
#include "UIComponent.h"
#include "MailMonsterCellInfo.h"
// 使用怪物里面的UI
#include "MailMonsterListView.h"

BattleReportMailResPopUpView::~BattleReportMailResPopUpView(){
    
} 
BattleReportMailResPopUpView *BattleReportMailResPopUpView::create(MailMonsterCellInfo *info){
    BattleReportMailResPopUpView *ret = new BattleReportMailResPopUpView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void BattleReportMailResPopUpView::onEnter(){
    PopupBaseView::onEnter();
    UIComponent::getInstance()->showPopupView(1);
    UIComponent::getInstance()->hideReturnBtn();
        setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattleReportMailResPopUpView::refreshView), MAIL_CONTENT_READ, NULL);
    
    setTouchEnabled(true);
    
    
}

void BattleReportMailResPopUpView::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MAIL_CONTENT_READ);
    PopupBaseView::onExit();
}

bool BattleReportMailResPopUpView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);  CCLoadSprite::doResourceByCommonIndex(6, true);
        CCLoadSprite::doResourceByCommonIndex(8, true);
        CCLoadSprite::doResourceByCommonIndex(105, true);
        CCLoadSprite::doResourceByCommonIndex(500, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(6, false);
            CCLoadSprite::doResourceByCommonIndex(8, false);
            CCLoadSprite::doResourceByCommonIndex(105, false);
            CCLoadSprite::doResourceByCommonIndex(500, false);
            CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
        });
        auto bg = CCBLoadFile("NEW_BattleReportMailCCB", this, this);
        if (CCCommonUtils::isIosAndroidPad()) {
            this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        }
        else
            this->setContentSize(bg->getContentSize());
        if(m_picBG)
        {
            CCCommonUtils::addNewDizi(m_picBG,1);
        }
        
        m_shareBtn->setVisible(false);
        m_mailTitle->setString(_lang("105513"));
        //CCCommonUtils::setButtonTitle(m_reportBtn, _lang("105519").c_str());
        this->m_timeText->setString(CCCommonUtils::timeStampToDate(m_info->createTime).c_str());
        m_ListNode = CCNode::create();
        if (CCCommonUtils::isIosAndroidPad()) {
            int extH = getExtendHeight();
            this->m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width, m_listContainer->getContentSize().height + extH));
            m_downNode->setPositionY(m_downNode->getPositionY() - extH);
            m_bgNode->setPositionY(m_bgNode->getPositionY() - extH);
            
            m_buildBG->setPositionY(m_buildBG->getPositionY() - extH);
            m_buildBG->setContentSize(CCDirector::sharedDirector()->getWinSize());
                              }
        else {
            int extH = getExtendHeight();
            this->m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width, m_listContainer->getContentSize().height + extH));
            m_downNode->setPositionY(m_downNode->getPositionY() - extH);
            m_bgNode->setPositionY(m_bgNode->getPositionY() - extH);
            m_buildBG->setPositionY(m_buildBG->getPositionY() - extH);
            m_buildBG->setContentSize(CCDirector::sharedDirector()->getWinSize());
        }
        m_scrollView = CCScrollView::create(m_listContainer->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_scrollView->setAnchorPoint(ccp(0, 1));
        m_scrollView->setTouchPriority(Touch_Popup);
        m_listContainer->addChild(m_scrollView);
        
        this->m_attackText->setFntFile("Arial_Bold_Regular.fnt");
        this->m_timeText->setFntFile("Arial_Bold_Regular.fnt");
        this->m_hintText->setFntFile("Arial_Bold_Regular.fnt");
        //        m_deleteBtnTitle->setString(_lang("108523").c_str());
        this->m_totalNode->removeChild(this->m_firstNode);
        this->m_totalNode->removeChild(this->m_upNode);
        this->m_totalNode->removeChild(this->m_failNode);
        this->m_scrollView->addChild(this->m_ListNode);
        this->m_ListNode->addChild(m_upNode);
        if (CCCommonUtils::isIosAndroidPad()) {
            this->m_upNode->setPositionX(768);
        }
        else
            this->m_upNode->setPositionX(320);
        m_totalH = 0;
        if (CCCommonUtils::isIosAndroidPad())
        {
            this->m_upNode->setPositionY(0);
            m_totalH = -528;
        }
        else {
            this->m_upNode->setPositionY(0);
            m_totalH-=220;////////
        }
        isaddHeight = false;
        m_Listheight = 305;
        if (CCCommonUtils::isIosAndroidPad()) {
            m_Listheight = 732;
        }
        if(m_info->checkMail){
            ownerUid = m_info->shareUid;
        }else{
            ownerUid = GlobalData::shared()->playerInfo.uid;
        }
        if(!m_info->isReadContent){
            getData();
        }else{
            refreshView();
            this->m_ListNode->setPositionY(m_Listheight);
            m_scrollView->setContentSize(CCSize(m_listContainer->getContentSize().width, m_Listheight));
            m_scrollView->setContentOffset(ccp(0, m_listContainer->getContentSize().height - (m_Listheight)));
        }
        ret = true;
    }
    return ret;
}
void BattleReportMailResPopUpView::refreshView(CCObject* p){

    //add 作战信息
    
    auto nodeinfo = m_info->monster;
    //MailMonsterInfo* info = dynamic_cast<MailMonsterInfo*>(m_info->monster->objectAtIndex(0));
//    auto cell = MailMonsterCell::create(nodeinfo,nullptr,0);
//    m_scrollView->addChild(cell);
//    cell->setPositionY(m_totalH);
    
//    Json *c = Json_create(m_info->showContent.c_str());
//    Json *arrlist=Json_getItem(c,"att");
//    if(!arrlist){
//        return;
//    }
//    int total = Json_getInt(arrlist, "total",0);
    
}
void BattleReportMailResPopUpView::getData(){
    MailController::getInstance()->readMailFromServer(m_info->uid, CC_ITOA(m_info->type));
    refreshView();
}

void BattleReportMailResPopUpView::onReturnClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    PopupViewController::getInstance()->goBackPopupView();
}

void BattleReportMailResPopUpView::onWriteClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    PopupViewController::getInstance()->addPopupInView(MailWritePopUpView::create("", ""));
}
void BattleReportMailResPopUpView::onShareClick(CCObject *pSender, CCControlEvent event){
   
}

void BattleReportMailResPopUpView::shareSuccess(CCObject* param){
    CCCommonUtils::flyText(_lang("115275"));
}

void BattleReportMailResPopUpView::onAddSaveClick(cocos2d::CCObject *pSender, CCControlEvent event){
    if(m_info->save ==1){
        MailCancelSaveCommand *cmd = new MailCancelSaveCommand(m_info->uid, m_info->type);
        cmd->sendAndRelease();
        m_info->save = 0;
        CCCommonUtils::flyHint("quest_icon_1.png", "", _lang("105576"));
    }else{
        if(MailController::getInstance()->isMailFull(SAVEMAIL)){
            CCCommonUtils::flyText("full");
            return;
        }
        MailSaveCommand *cmd = new MailSaveCommand(m_info->uid, m_info->type);
        cmd->sendAndRelease();
        m_info->save = 1;
        CCCommonUtils::flyHint("quest_icon_1.png", "", _lang("105575"));
    }
    if(m_info->save==0){
        m_addSaveBtn->setHighlighted(false);
    }else{
        m_addSaveBtn->setHighlighted(true);
    }
}
void BattleReportMailResPopUpView::onBattleDetailClick(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->addPopupView(MailBattleDetailView::create(m_info), false);
}

void BattleReportMailResPopUpView::onReportClick(CCObject *pSender, CCControlEvent event){
    CCCommonUtils::flyText(_lang("105534"));
    BattleManager::shared()->mailFightReport(m_info->reportUid);
    
}
void BattleReportMailResPopUpView::onOkDeleteMail(){
    MailController::getInstance()->removeMail(m_info->uid, CC_ITOA(m_info->type));
    PopupViewController::getInstance()->goBackPopupView();
}
void BattleReportMailResPopUpView::onDeleteClick(CCObject *pSender, CCControlEvent event){
    if(m_info->save!=0){
        CCCommonUtils::flyHint("","",_lang("105575"));
        return;
    }
    YesNoDialog::showYesDialog(_lang("105570").c_str(),false,CCCallFunc::create(this, callfunc_selector(BattleReportMailResPopUpView::onOkDeleteMail)),true);
}

bool BattleReportMailResPopUpView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailTitle", CCLabelIF*, this->m_mailTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_attackText", CCLabelIF*, this->m_attackText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_failNode", CCNode*, this->m_failNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeText", CCLabelIF*, this->m_timeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintText", CCLabelIF*, this->m_hintText);
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upTitle", CCLabelIF*, this->m_upTitle);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_reportBtn", CCControlButton*, this->m_reportBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deleteBtn", CCControlButton*, this->m_deleteBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_firstNode", CCNode*, this->m_firstNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upNode", CCNode*, this->m_upNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNode", CCNode*, this->m_totalNode);
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_detailBtn", CCControlButton*, this->m_detailBtn);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_unSaveBtn", CCControlButton*, this->m_unSaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_addSaveBtn", CCControlButton*, this->m_addSaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_writeBtn", CCControlButton*, this->m_writeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_returnBtn", CCControlButton*, this->m_returnBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_returnSpr", CCSprite*, this->m_returnSpr);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBG", CCSprite*, this->m_picBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_battlePicNode", CCNode*, this->m_battlePicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_shareBtn", CCControlButton*, this->m_shareBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintBG", CCScale9Sprite*, this->m_hintBG);
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintLine", CCSprite*, this->m_hintLine);
    return false;
}
SEL_CCControlHandler BattleReportMailResPopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
   
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onReportClick", BattleReportMailPopUpView::onReportClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDeleteClick", BattleReportMailResPopUpView::onDeleteClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBattleDetailClick", BattleReportMailResPopUpView::onBattleDetailClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddSaveClick", BattleReportMailResPopUpView::onAddSaveClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onShareClick", BattleReportMailResPopUpView::onShareClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onReturnClick", BattleReportMailResPopUpView::onReturnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onWriteClick", BattleReportMailResPopUpView::onWriteClick);
    
    return NULL;
}

bool BattleReportMailResPopUpView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    return true;
}

void BattleReportMailResPopUpView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}
void BattleReportMailResPopUpView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
}
