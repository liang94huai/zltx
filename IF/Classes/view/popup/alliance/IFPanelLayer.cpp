//
//  IFPanelLayer.cpp
//  IF
//
//  Created by wangdianzhen on 16/1/13.
//
//

#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "YesNoDialog.h"
#include "IFPanelLayer.hpp"
#include "SoundController.h"
#include "CheckAllianceInfoView.h"
#include "AllianceFlagPar.h"
#include "GetAllianceListCommand.h"
#include "AllianceManager.h"
#include "TipsView.h"
#include "UIComponent.h"
#include "JoinAllianceView.h"
IFPanelLayer* IFPanelLayer::create(LayerType type){
    IFPanelLayer* ret = new IFPanelLayer(type);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool IFPanelLayer::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCLoadSprite::doResourceByCommonIndex(8, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::doResourceByCommonIndex(8, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
    });
    auto tmpCCB = CCBLoadFile("IFPanelLayer",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    m_titleTxt->setString(_lang("102160").c_str());
    int oldBgHeight = m_buildBG->getContentSize().height;
    changeBGHeight(m_buildBG);
    int newBgHeight = m_buildBG->getContentSize().height;
    int addHeight = newBgHeight - oldBgHeight;
    int oldWidth = m_infoList->getContentSize().width;
    int oldHeight = m_infoList->getContentSize().height;
    m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
    m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
    m_bottomNode->setPositionY(m_bottomNode->getPositionY() - addHeight);
    int bgcount = newBgHeight/100+1;
    for (int i = 0; i<=bgcount; i++) {
        auto pic = CCLoadSprite::createSprite("technology_09.png");
        pic->setPositionY(-i*100);
        pic->setScaleX(0.98);
        if (CCCommonUtils::isIosAndroidPad())
        {
            pic->setPositionY((i-3)*100);
            pic->setScaleX(2.4f);
            pic->setAnchorPoint(ccp(0, 1));
        }
        m_BGNode->addChild(pic);
    }
    CCCommonUtils::onAddFire(m_fireNode1);
    CCCommonUtils::onAddFire(m_fireNode2);
    CCCommonUtils::setButtonTitle(m_storeBtn, _lang("104900").c_str());
    CCCommonUtils::setButtonTitle(m_bagBtn, _lang("104901").c_str());
    CCCommonUtils::setButtonTitle(m_btn, _lang("115851").c_str());
    allianceArray = CCArray::create();
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_infoList->addChild(m_tabView);
    
    CCCommonUtils::setButtonTitle(m_bagBtn, _lang("115846").c_str());
    CCCommonUtils::setButtonTitle(m_bagBtn, _lang("115847").c_str());
    GameController::getInstance()->showWaitInterface();
    /*testcommand*/
    GetAllianceListCommand* cmd = new GetAllianceListCommand("",1,0);
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(IFPanelLayer::updateAlliances), NULL));
    cmd->sendAndRelease();
    //todowdz add cmd
    updateInfo();
    return true;
}
void IFPanelLayer::updateAlliances(CCObject* data){
    GameController::getInstance()->removeWaitInterface();
    NetResult* result = dynamic_cast<NetResult*>(data);
    if( !result )
        return;
    
    auto params = _dict(result->getData());
    if( !params )
        return;
    
    CCArray* arr =  (CCArray*)params->objectForKey("list");
    if( !arr )
        return;
    
    m_total_receive = params->valueForKey("total")->intValue();
    allianceArray->removeAllObjects();
    AllianceManager::getInstance()->maxAlliancePage = m_total_receive/20+(m_total_receive%20==0?0:1);
    int num = arr->count();
    for (int i=0; i<num; i++) {
        CCDictionary* dicAlliance = dynamic_cast<CCDictionary*>(arr->objectAtIndex(i));
        if( dicAlliance == NULL )
            continue;
        
        AllianceInfo* alliance = new AllianceInfo();
        alliance->updateAllianceInfo(dicAlliance);
        if (i == 0) {
            AllianceManager::getInstance()->setrecommendAid(alliance->uid);
        }
        allianceArray->addObject(alliance);
        alliance->release();
    }
    //搜索无结果处理
    if(m_total_receive == 0){
        CCLabelIFTTF* noResult = CCLabelIFTTF::create();
        noResult->setFontSize(30);
        noResult->setDimensions(CCSizeMake(400,0));
        noResult->setPosition(m_infoList->getContentSize()/2);
        noResult->setString(_lang("115204").c_str());
        m_infoList->addChild(noResult, 10, 404);
    }else{
        m_infoList->removeChildByTag(404);
    }
    m_tabView->reloadData();
    updateInfo();
}
void IFPanelLayer::updateInfo()
{
    if (m_type==TYPE_BLACK) {
        m_storeBtn->setEnabled(false);
        m_bagBtn->setEnabled(true);
    }
    else {
        m_storeBtn->setEnabled(true);
        m_bagBtn->setEnabled(false);
    }

}

void IFPanelLayer::refreashData()
{
   
}


CCSize IFPanelLayer::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= allianceArray->count()){
        return CCSizeZero;
    }
   return CCSize(640, 220);
}
CCSize IFPanelLayer::cellSizeForTable(CCTableView *table)
{
    return CCSize(640, 220);
}
CCTableViewCell* IFPanelLayer::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= allianceArray->count()){
        return NULL;
    }
    
    IFListCell* cell = (IFListCell*)table->dequeueCell();
    if(idx < allianceArray->count()){
        if(cell){
            cell->updateAllianceInfo((AllianceInfo*)allianceArray->objectAtIndex(idx));
        }else{
            cell = IFListCell::create((AllianceInfo*)allianceArray->objectAtIndex(idx),m_infoList);
        }
        
    }
    return cell;
}
ssize_t IFPanelLayer::numberOfCellsInTableView(CCTableView *table)
{
    return allianceArray->count();
}
void IFPanelLayer::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

void IFPanelLayer::onEnter()
{
    CCNode::onEnter();
    UIComponent::getInstance()->showPopupView(1);
}

void IFPanelLayer::onExit()
{
    CCNode::onExit();
}

IFPanelLayer::~IFPanelLayer()
{
 
}

SEL_CCControlHandler IFPanelLayer::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseClick", IFPanelLayer::onCloseClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickStoreBtn", IFPanelLayer::onClickStoreBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBagBtn", IFPanelLayer::onClickBagBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtnClick", IFPanelLayer::onBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipBtnClick", IFPanelLayer::onTipBtnClick);
    return NULL;
}

bool IFPanelLayer::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_storeBtn", CCControlButton*, this->m_storeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bagBtn", CCControlButton*, this->m_bagBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_BGNode", CCNode*, this->m_BGNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", CCNode*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", CCNode*, this->m_fireNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn", CCControlButton*, this->m_btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt", CCLabelIF*, this->m_titleTxt);
    return false;
}

void IFPanelLayer::onTipBtnClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->addPopupView(TipsVipView::create(_lang("115859"),_lang("115860")));
}
void IFPanelLayer::onCloseClick(cocos2d::CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
}

void IFPanelLayer::onClickStoreBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_type != 0) {
        m_type = 0;
        updateInfo();
    }
}

void IFPanelLayer::onBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->addPopupInView(JoinAllianceView::create(11,OPEN_LIST));
}
void IFPanelLayer::onClickBagBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_type != 1) {
        m_type = 1;
        updateInfo();
    }
}

void IFListCell::updateAllianceInfo(AllianceInfo* info){
    m_info = info;
    if(!m_info) return;
    
    std::string str = "(";
    str.append(m_info->shortName.c_str());
    str.append(") ");
    str.append(m_info->name.c_str());
    if(m_allianceTxt){
        m_allianceTxt->setString(str.c_str());
    }
    str = _lang_1("115015",m_info->leader.c_str());
    if(m_leaderTxt){
        m_leaderTxt->setString(str.c_str());
    }
    if(m_allianceNumTxt){
        m_allianceNumTxt->setString(CCString::createWithFormat("%d/%d",m_info->currentNum,m_info->maxNum)->getCString());
    }
    str = CC_CMDITOA(m_info->totalForce);
    if(m_forceTxt){
        m_forceTxt->setString(str.c_str());
    }
    m_allianceIcon->removeAllChildrenWithCleanup(true);
    CCLoadSprite::doResourceByCommonIndex(205, true);
    AllianceFlagPar* flag = AllianceFlagPar::create(m_info->getAllianceIcon().c_str());
    m_allianceIcon->addChild(flag,0,444);
    string lang = ( strcmp(m_info->language.c_str(), "")==0?"115600":m_info->language );
    if(m_language){
        m_language->setString(_lang(lang).c_str());
    }
    
    m_appNode->setVisible(false);
    m_language->setVisible(true);
    if(m_info->isApplied){
        m_appNode->setVisible(true);
        m_language->setVisible(false);
    }

    m_RestrictIcon->removeAllChildrenWithCleanup(true);
    if (info->powerRestriction != 0 || info->castleRestriction != 0) {  //---
        if(info->recruit == 0 ){
            CCLoadSprite::doResourceByCommonIndex(7, true);
            CCSprite* spr = CCLoadSprite::createSprite("Alliance_Restriction.png");
            if (CCCommonUtils::isIosAndroidPad()) {
                spr->setScale(2);
            }
            m_RestrictIcon->addChild(spr);
        }
    }
}

IFListCell* IFListCell::create(AllianceInfo* info,CCNode* touchNode)
{
    auto ret = new IFListCell(info,touchNode);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool IFListCell::init()
{
    auto ccb = CCBLoadFile("JoinAllianceCell",this,this);
    ccb->setTag(111);
    
    m_allianceTxt = CCLabelIFTTF::create("", 24);
    m_allianceTxt->setColor(m_nameTxt->getColor());
    m_allianceTxt->setFontSize(m_nameTxt->getFontSize());
    m_allianceTxt->setAnchorPoint(m_nameTxt->getAnchorPoint());
    m_allianceTxt->setPosition(m_nameTxt->getPosition());
    m_nameTxt->getParent()->addChild(m_allianceTxt);
    m_appTxt->setString(_lang("115056").c_str());

    return true;
}


bool IFListCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_touchPos = pTouch->getLocation();
    
    if (m_touchNode && isTouchInside(m_touchNode,pTouch)) {
        return true;
    }
    return false;
}
void IFListCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
    CCPoint end = pTouch->getLocation();
    
    if (m_touchNode && isTouchInside(m_touchNode,pTouch) && isTouchInside(m_clickNode, pTouch) && fabsf(end.y - m_touchPos.y)<30 && m_info!=NULL) {
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        PopupViewController::getInstance()->addPopupInView(CheckAllianceInfoView::create(m_info,OPEN_LIST));
        
    }
}

void IFListCell::onEnter() {
    CCNode::onEnter();
    setTouchEnabled(true);
    this->updateAllianceInfo(m_info);
}

void IFListCell::onExit() {
    CCNode::onExit();
    setTouchEnabled(false);
}
SEL_CCControlHandler IFListCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}
bool IFListCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIFTTF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leaderTxt", CCLabelIFTTF*, this->m_leaderTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceNumTxt", CCLabelIFTTF*, this->m_allianceNumTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_appTxt", CCLabelIFTTF*, this->m_appTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_forceTxt", CCLabelIFTTF*, this->m_forceTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnView", CCNode*, this->m_btnView);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_appNode", CCNode*, this->m_appNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceIcon", CCNode*, this->m_allianceIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode", CCNode*, this->m_clickNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_language", CCLabelIFTTF*, this->m_language);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_RestrictIcon", CCNode*, this->m_RestrictIcon);
    
    return false;
}

void IFListCell::setHighlight(){
    if(!this->getChildByTag(111)->isVisible()) return;
    m_bg->setColor(ccc3(128,128,128));
    ((CCSprite*)m_allianceIcon->getChildByTag(444)->getChildByTag(444))->setColor(ccc3(128,128,128));
}
void IFListCell::unHighlight(){
    if(this==NULL || !this->getChildByTag(111)->isVisible()) return;
    m_bg->setColor(ccc3(255,255,255));
    ((CCSprite*)m_allianceIcon->getChildByTag(444)->getChildByTag(444))->setColor(ccc3(255,255,255));
}