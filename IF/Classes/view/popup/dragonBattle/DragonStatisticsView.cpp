//
//  DragonStatisticsView.cpp
//  IF
//
//  Created by ganxiaohua on 15/8/31.
//
//

#include "DragonStatisticsView.h"
#include "PopupViewController.h"
#include "RewardController.h"
#include "ToolController.h"
#include "SceneController.h"
#include "YesNoDialog.h"
#include "WorldMapView.h"
#include "DragonBattleFightRankCommand.h"
#include "DragonBattleLeaveCommand.h"
#include "ActivityController.h"
#include "../../../Ext/CCDevice.h"
#include "DynamicResourceController.h"

#define  DRAGON_STATISTICS_CELL_CLICK "DRAGON_STATISTICS_CELL_CLICK"

DragonStatisticsView *DragonStatisticsView::create(){
    DragonStatisticsView *ret = new DragonStatisticsView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonStatisticsView::onEnter(){
    this->setTitleName(_lang("140001"));
    PopupBaseView::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonStatisticsView::updateData), DRAGON_STATISTICS_CELL_CLICK, NULL);

}

void DragonStatisticsView::onExit(){
    if(m_waitInterface){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    GameController::getInstance()->removeWaitInterface();
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, DRAGON_STATISTICS_CELL_CLICK);
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    PopupBaseView::onExit();
}

void DragonStatisticsView::updateData(CCObject* param){
    m_tabView->reloadData();
}

bool DragonStatisticsView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    return true;
}

void DragonStatisticsView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(ccpDistance(pTouch->getStartLocation(), pTouch->getLocation())>30){
        return ;
    }
    if (m_tab4->isEnabled() && isTouchInside(m_selectSpr, pTouch)) {
        m_selectSpr->setVisible(!m_selectSpr->isVisible());
        ActivityController::getInstance()->isShowLine = !m_selectSpr->isVisible();
    }else if (!m_tab4->isEnabled() && isTouchInside(m_videoNode, pTouch)) {
        CCLog("m_videoNode");
        //cocos2d::extension::CCDevice::updateVersion("http://weibo.com/liewangdefenzheng");
        CCCommonUtils::showHelpShiftSingleFAQ("45216");
    }
}

bool DragonStatisticsView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(502, true);
        CCLoadSprite::doResourceByCommonIndex(512, true);
        CCLoadSprite::doResourceByCommonIndex(500, true);
        CCLoadSprite::doResourceByCommonIndex(506, true);
        if (DynamicResourceController::getInstance()->checkActivityAdResource())
        {
            DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_ACTIVITY_AD_TEXTURE,false);
        }
        setCleanFunction([](){
            DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_ACTIVITY_AD_TEXTURE,true);
            CCLoadSprite::doResourceByCommonIndex(502, false);
            CCLoadSprite::doResourceByCommonIndex(500, false);
            CCLoadSprite::doResourceByCommonIndex(506, false);
            CCLoadSprite::doResourceByCommonIndex(512, false);
        });
        auto node = CCBLoadFile("DragonStatisticsView", this, this);
        this->setContentSize(node->getContentSize());
        int prev = m_viewBg->getContentSize().height;
        this->changeBGHeight(m_viewBg);
        int add = m_viewBg->getContentSize().height - prev;
        if (CCCommonUtils::isIosAndroidPad()) {
            add = CCDirector::sharedDirector()->getWinSize().height - 2048;
        }
        m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width,m_listContainer->getContentSize().height+add));
        m_listContainer->setPositionY(m_listContainer->getPositionY() - add);
        
        m_textNode->setContentSize(CCSize(m_textNode->getContentSize().width,m_textNode->getContentSize().height+add));
        m_textNode->setPositionY(m_textNode->getPositionY() - add);
        
        m_buildBG->setContentSize(CCSize(m_buildBG->getContentSize().width,m_buildBG->getContentSize().height+add));
        
        m_bottomNode->setPositionY(m_bottomNode->getPositionY() - add);
        
        m_data = CCArray::create();
        m_srcData = CCArray::create();
        
        m_scrollView = CCScrollView::create(m_textNode->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_textNode->addChild(m_scrollView);
        
        m_tabView = CCTableView::create(this, m_listContainer->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(Touch_Default);
        m_listContainer->addChild(m_tabView);
        
        m_introTxt->setString(_lang("140081"));
        m_introTitle->setString(_lang("140078"));
        
        m_nodetxt1_0->setString(_lang("140072"));
        m_nodetxt1_1->setString(_lang("140073"));
        
        m_nodetxt2_0->setString(_lang("140074"));
        m_nodetxt2_1->setString(_lang("140075"));
        
        m_nodetxt3_0->setString(_lang("140076"));
        m_nodetxt3_1->setString(_lang("140077"));
        
        m_tNode1->removeFromParent();
        m_tNode2->removeFromParent();
        m_tNode3->removeFromParent();
        
        int textH1 = m_nodetxt1_1->getContentSize().height*m_nodetxt1_1->getOriginScaleY() + 48;
        int textH2 = m_nodetxt2_1->getContentSize().height*m_nodetxt2_1->getOriginScaleY() + 48;
        int textH3 = m_nodetxt3_1->getContentSize().height*m_nodetxt3_1->getOriginScaleY() + 48;
        int totalH = textH1 + textH2 + textH3;
        int temp = totalH - 15;
        m_tNode1->setPositionY(temp);
        temp -= textH1;
        m_tNode2->setPositionY(temp);
        temp -= textH2;
        m_tNode3->setPositionY(temp);
        
        m_scrollView->addChild(m_tNode1);
        m_scrollView->addChild(m_tNode2);
        m_scrollView->addChild(m_tNode3);
        
        m_scrollView->setContentSize(CCSize(m_textNode->getContentSize().width,totalH));
        m_scrollView->setContentOffset(ccp(0, m_textNode->getContentSize().height - totalH));
        CCCommonUtils::setButtonTitle(m_tab1, _lang("140062").c_str());
        CCCommonUtils::setButtonTitle(m_tab2, _lang("140064").c_str());
        CCCommonUtils::setButtonTitle(m_tab3, _lang("140063").c_str());
        CCCommonUtils::setButtonTitle(m_tab4, _lang("140072").c_str());
        CCCommonUtils::setButtonTitle(m_leaveBtn, _lang("140061").c_str());
        
        CCSpriteFrame* sf = CCLoadSprite::getSF("57009_ad1.png");
        if (sf!=NULL) {
            m_dragonBg->initWithSpriteFrame(sf);
        }
        
        m_tab1->setEnabled(false);
        
        m_selectSpr->setVisible(!ActivityController::getInstance()->isShowLine);
        
        DragonBattleFightRankCommand* cmd = new DragonBattleFightRankCommand();
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(DragonStatisticsView::getServerData), NULL));
        cmd->sendAndRelease();

        m_time = 40000;
        this->schedule(schedule_selector(DragonStatisticsView::updateTime), 1);
        updateTime(0);
        
        ret = true;
    }
    return ret;
}

void DragonStatisticsView::updateTime(float _time){
    long left = 0;
    double endTime = GlobalData::shared()->dbFightEndTime - GlobalData::shared()->getWorldTime();
    if(ActivityController::getInstance()->dragonBattleState==2){
        left = GlobalData::shared()->dbFightBeginTime - GlobalData::shared()->getWorldTime();
        if (left<0) {
            left = 0;
        }
        m_tipTxt->setString(_lang("140149"));
        m_timeTxt->setString(_lang_1("140052",CC_SECTOA(left)));
    }else if(endTime>0){
        m_tipTxt->setString(_lang("140148"));
        m_timeTxt->setString(_lang_1("140060",CC_SECTOA(endTime)));
    }else{
        m_timeTxt->setString(_lang("140069").c_str());
    }
}

void DragonStatisticsView::onClickHelpBtn(CCObject * pSender, CCControlEvent pCCControlEvent){
    CCCommonUtils::showHelpShiftSingleFAQ("45216");
}

void DragonStatisticsView::onLeaveClick(CCObject * pSender, CCControlEvent pCCControlEvent){

    if (CCCommonUtils::isIosAndroidPad()) {
        leavelDragon();
    }else{
          YesNoDialog* dialog2 = YesNoDialog::show(_lang("140157").c_str(), CCCallFunc::create(this, callfunc_selector(DragonStatisticsView::leavelDragon)));
    }
}

void DragonStatisticsView::leavelDragon(){
    DragonBattleLeaveCommand* cmd = new DragonBattleLeaveCommand();
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(DragonStatisticsView::callSuccess), NULL));
    cmd->sendAndRelease();
    GameController::getInstance()->showWaitInterface();
}

void DragonStatisticsView::getServerData(CCObject* param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dic = _dict(result->getData());
    if(dic){
        CCArray* array = dynamic_cast<CCArray*>(dic->objectForKey("rank"));
        if (array!=NULL) {
            m_srcData->addObjectsFromArray(array);
            if (!m_tab1->isEnabled()) {
                onClickTab1(NULL,Control::EventType::TOUCH_DOWN);
            }else if (!m_tab2->isEnabled()) {
                onClickTab2(NULL,Control::EventType::TOUCH_DOWN);
            }else if (!m_tab3->isEnabled()) {
                onClickTab3(NULL,Control::EventType::TOUCH_DOWN);
            }
        }
    }
}

void DragonStatisticsView::callSuccess(CCObject* param){
    
}

void DragonStatisticsView::onClickTab1(CCObject * pSender, CCControlEvent pCCControlEvent){
    m_tab1->setEnabled(false);
    m_tab2->setEnabled(true);
    m_tab3->setEnabled(true);
    m_tab4->setEnabled(true);
    m_listContainer->setVisible(true);
    m_introNode->setVisible(false);
    m_bottomNode->setVisible(true);
    m_data->removeAllObjects();
    
    int selfIndex = -1;
    int num = m_srcData->count();
    for (int i=0; i<num; i++) {
        auto info1 = dynamic_cast<CCDictionary*>(m_srcData->objectAtIndex(i));
        if (info1) {
            string uid = info1->valueForKey("id")->getCString();
            if (uid!=GlobalData::shared()->playerInfo.uid) {
                m_data->addObject(info1);
            }else{
                selfIndex = i;
            }
        }
    }
    num = m_data->count();
    for (int i=0; i<num; i++) {
        for (int j=0; j<num-i-1; j++) {
            auto info1 = dynamic_cast<CCDictionary*>(m_data->objectAtIndex(j));
            auto info2 = dynamic_cast<CCDictionary*>(m_data->objectAtIndex(j+1));
            if (info1 && info2) {
                int score1 = info1->valueForKey("score")->intValue();
                int score2 = info2->valueForKey("score")->intValue();
                if(score1>score2){
                    m_data->swap(j, j+1);
                }
            }
        }
    }
    m_data->reverseObjects();
    if (selfIndex!=-1 && selfIndex < m_srcData->count()) {
        auto info1 = dynamic_cast<CCDictionary*>(m_srcData->objectAtIndex(selfIndex));
        if (info1) {
            m_data->insertObject(info1, 0);
        }
    }
    m_tabView->reloadData();
}

void DragonStatisticsView::onClickTab2(CCObject * pSender, CCControlEvent pCCControlEvent){
    m_tab1->setEnabled(true);
    m_tab2->setEnabled(false);
    m_tab3->setEnabled(true);
    m_tab4->setEnabled(true);
    m_listContainer->setVisible(true);
    m_introNode->setVisible(false);
    m_bottomNode->setVisible(true);
    m_data->removeAllObjects();
    int num = m_srcData->count();
    for (int i=0; i<num; i++) {
        auto dic = dynamic_cast<CCDictionary*>(m_srcData->objectAtIndex(i));
        if (dic) {
            int camp = dic->valueForKey("campType")->intValue();//0 红 、1 蓝
            if (camp==CAMP_RED) {
                m_data->addObject(dic);
            }
        }
    }
    
    num = m_data->count();
    for (int i=0; i<num; i++) {
        for (int j=0; j<num-i-1; j++) {
            auto info1 = dynamic_cast<CCDictionary*>(m_data->objectAtIndex(j));
            auto info2 = dynamic_cast<CCDictionary*>(m_data->objectAtIndex(j+1));
            if (info1 && info2) {
                int score1 = info1->valueForKey("score")->intValue();
                int score2 = info2->valueForKey("score")->intValue();
                if(score1>score2){
                    m_data->swap(j, j+1);
                }
            }
        }
    }
    m_data->reverseObjects();
    m_tabView->reloadData();
}

void DragonStatisticsView::onClickTab3(CCObject * pSender, CCControlEvent pCCControlEvent){
    m_tab1->setEnabled(true);
    m_tab2->setEnabled(true);
    m_tab3->setEnabled(false);
    m_tab4->setEnabled(true);
    m_listContainer->setVisible(true);
    m_introNode->setVisible(false);
    m_bottomNode->setVisible(true);
    m_data->removeAllObjects();
    int num = m_srcData->count();
    for (int i=0; i<num; i++) {
        auto dic = dynamic_cast<CCDictionary*>(m_srcData->objectAtIndex(i));
        if (dic) {
            int camp = dic->valueForKey("campType")->intValue();//0 红 、1 蓝
            if (camp==CAMP_BLUE) {
                m_data->addObject(dic);
            }
        }
    }
    num = m_data->count();
    for (int i=0; i<num; i++) {
        for (int j=0; j<num-i-1; j++) {
            auto info1 = dynamic_cast<CCDictionary*>(m_data->objectAtIndex(j));
            auto info2 = dynamic_cast<CCDictionary*>(m_data->objectAtIndex(j+1));
            if (info1 && info2) {
                int score1 = info1->valueForKey("score")->intValue();
                int score2 = info2->valueForKey("score")->intValue();
                if(score1>score2){
                    m_data->swap(j, j+1);
                }
            }
        }
    }
    m_data->reverseObjects();
    m_tabView->reloadData();
}

void DragonStatisticsView::onClickTab4(CCObject * pSender, CCControlEvent pCCControlEvent){
    m_tab1->setEnabled(true);
    m_tab2->setEnabled(true);
    m_tab3->setEnabled(true);
    m_tab4->setEnabled(false);
    m_listContainer->setVisible(false);
    m_introNode->setVisible(true);
    m_bottomNode->setVisible(false);
}

bool DragonStatisticsView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_selectSpr", CCNode*, this->m_selectSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipTxt", CCLabelIF*, this->m_tipTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_introTxt", CCLabelIF*, this->m_introTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tab1", CCControlButton*, this->m_tab1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tab2", CCControlButton*, this->m_tab2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tab3", CCControlButton*, this->m_tab3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tab4", CCControlButton*, this->m_tab4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_leaveBtn", CCControlButton*, this->m_leaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_introTitle", CCLabelIF*, this->m_introTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_introNode", CCNode*, this->m_introNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_dragonBg", CCSprite*, this->m_dragonBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_selectBtn", CCSprite*, this->m_selectBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_videoNode", CCNode*, this->m_videoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_textNode", CCNode*, this->m_textNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_buildBG", CCNode*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tNode1", CCNode*, this->m_tNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nodetxt1_0", CCLabelIF*, this->m_nodetxt1_0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nodetxt1_1", CCLabelIF*, this->m_nodetxt1_1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tNode2", CCNode*, this->m_tNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nodetxt2_0", CCLabelIF*, this->m_nodetxt2_0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nodetxt2_1", CCLabelIF*, this->m_nodetxt2_1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tNode3", CCNode*, this->m_tNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nodetxt3_0", CCLabelIF*, this->m_nodetxt3_0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nodetxt3_1", CCLabelIF*, this->m_nodetxt3_1);
    return false;
}

SEL_CCControlHandler DragonStatisticsView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onLeaveClick", DragonStatisticsView::onLeaveClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickHelpBtn", DragonStatisticsView::onClickHelpBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickTab1", DragonStatisticsView::onClickTab1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickTab2", DragonStatisticsView::onClickTab2);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickTab3", DragonStatisticsView::onClickTab3);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickTab4", DragonStatisticsView::onClickTab4);
    return NULL;
}


CCSize DragonStatisticsView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    CCDictionary* info = (CCDictionary*)m_data->objectAtIndex(idx);
    if (info) {
        string flag = info->valueForKey("flag")->getCString();
        if (flag=="1") {
            if (CCCommonUtils::isIosAndroidPad())
            {
                return CCSize(1536, 130*2.4);
            }else{
                return CCSize(640, 130);
            }
        }
    }
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(1536, 75*2.4);
    }
    return CCSize(640, 75);
}

CCSize DragonStatisticsView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(1536, 75*2.4);
    }
    return CCSize(640, 75);
}

CCTableViewCell* DragonStatisticsView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    DragonStatisticsCell* cell = (DragonStatisticsCell*)table->dequeueCell();
    CCDictionary* info = (CCDictionary*)m_data->objectAtIndex(idx);
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info);
        }else{
            cell = DragonStatisticsCell::create(info,m_listContainer);
        }
    }
    return cell;
}

ssize_t DragonStatisticsView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data->count();
}

void DragonStatisticsView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
   
}

DragonStatisticsCell *DragonStatisticsCell::create(CCDictionary* info,CCNode* clickArea){
    DragonStatisticsCell *ret = new DragonStatisticsCell(info,clickArea);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonStatisticsCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
}

void DragonStatisticsCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool DragonStatisticsCell::init(){
    auto bg = CCBLoadFile("DragonStatisticsCell", this, this);
    this->setContentSize(bg->getContentSize());
    setData(m_info);
    return true;
}

void DragonStatisticsCell::setData(CCDictionary* info){
    m_info = info;
    if (m_info==NULL) {
        return;
    }
    string playerName = "";
    string abbr = m_info->valueForKey("abbr")->getCString();
    if (abbr!="") {
        playerName.append("(");
        playerName.append(abbr);
        playerName.append(")");
    }
    playerName.append(m_info->valueForKey("name")->getCString());
    if(playerName.length()>15){
        playerName = CCCommonUtils::subStrByUtf8(playerName,0,14);
        playerName.append("...");
    }
    m_allianceName->setString(playerName);
    string scoreStr = _lang("140096");
    scoreStr.append(":");
    scoreStr.append(m_info->valueForKey("score")->getCString());
    m_scoreTxt->setString(scoreStr.c_str());
    string temp = "X ";
    temp.append(m_info->valueForKey("awardNum")->getCString());
    m_numTxt->setString(temp);
    temp = _lang("140055");
    temp.append(":");
    temp.append(m_info->valueForKey("winNum")->getCString());
    m_winTxt->setString(temp);
    temp = _lang("140056");
    temp.append(":");
    temp.append(m_info->valueForKey("failNum")->getCString());
    m_failTxt->setString(temp);
    temp = _lang("140057");
    temp.append(":");
    temp.append(m_info->valueForKey("killNum")->getCString());
    m_killTxt->setString(temp);
    temp = _lang("140058");
    temp.append(":");
    temp.append(m_info->valueForKey("holdNum")->getCString());
    m_bNumTxt->setString(temp);
    string flag = m_info->valueForKey("flag")->getCString();
    if (flag=="0" || flag=="") {
        m_openNode->setVisible(false);
        m_normalNode->setPositionY(0);
    }else{
        m_openNode->setVisible(true);
        m_normalNode->setPositionY(50);
    }
    int camp = m_info->valueForKey("campType")->intValue();//0 红 、1 蓝
    string uid = m_info->valueForKey("id")->getCString();
    if(uid==GlobalData::shared()->playerInfo.uid){
        m_allianceName->setColor({255,234,64});
        m_scoreTxt->setColor({255,234,64});
    }else{
        m_allianceName->setColor({212,198,161});
        m_scoreTxt->setColor({212,198,161});
    }
    if(m_enemySign){
        auto sf = CCLoadSprite::getSF("dragon_cell_bg1.png");
        if (camp==CAMP_RED) {
            sf = CCLoadSprite::getSF("dragon_cell_bg2.png");
        }
        if (sf!=NULL) {
            m_enemySign->initWithSpriteFrame(sf);
            m_enemySign->setContentSize(CCSize(640,68));
        }
    }
}

bool DragonStatisticsCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    return true;
}

void DragonStatisticsCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (ccpDistance(pTouch->getStartLocation(), pTouch->getLocation())>30) {
        return ;
    }
    if (isTouchInside(m_clickNode, pTouch) && isTouchInside(m_clickArea, pTouch)) {
        string flag = m_info->valueForKey("flag")->getCString();
        if (flag=="0"|| flag=="") {
            m_arrow->setRotation(0);
            m_info->setObject(CCString::create("1"), "flag");
        }else{
            m_arrow->setRotation(270);
            m_info->setObject(CCString::create("0"), "flag");
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(DRAGON_STATISTICS_CELL_CLICK);
    }
}

void DragonStatisticsCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
}

bool DragonStatisticsCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_openNode", CCNode*, this->m_openNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_arrow", CCNode*, this->m_arrow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickNode", CCNode*, this->m_clickNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_allianceName", CCLabelIF*, this->m_allianceName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_scoreTxt", CCLabelIF*, this->m_scoreTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numTxt", CCLabelIF*, this->m_numTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_winTxt", CCLabelIF*, this->m_winTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_failTxt", CCLabelIF*, this->m_failTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_killTxt", CCLabelIF*, this->m_killTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bNumTxt", CCLabelIF*, this->m_bNumTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_normalNode", CCNode*, this->m_normalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_enemySign", CCScale9Sprite*, this->m_enemySign);
    return false;
}
SEL_CCControlHandler DragonStatisticsCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}