//
//  DragonBattleWarListView.cpp
//  IF
//
//  Created by ganxiaohua on 15/8/25.
//
//

#include "DragonBattleWarListView.h"
#include "PopupViewController.h"
#include "CommandBase.h"
#include "YesNoDialog.h"
#include "DragonBattleApplyCommand.h"
#include "DragonBattleApplyInfoCommand.h"
#include "ActivityController.h"
#include "DragonManageView.h"
#include "DragonHistoryView.h"

#define UPDATE_WAR_ALLIANCE_DATA "UPDATE_WAR_ALLIANCE_DATA"
static double fightBeginTime = 0;

DragonBattleWarListView *DragonBattleWarListView::create(){
    DragonBattleWarListView *ret = new DragonBattleWarListView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonBattleWarListView::onEnter(){
    PopupBaseView::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonBattleWarListView::refreshData), UPDATE_WAR_ALLIANCE_DATA, NULL);
}

void DragonBattleWarListView::onExit(){
    if (m_waitInterface) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, UPDATE_WAR_ALLIANCE_DATA);
    cellList.clear();
    PopupBaseView::onExit();
}

void DragonBattleWarListView::refreshData(CCObject *param){
    if (m_tabView) {
        CCPoint curr = m_tabView->getContentOffset();
        CCPoint max = m_tabView->maxContainerOffset();
        CCPoint min = m_tabView->minContainerOffset();
        m_tabView->reloadData();
        curr.y += m_tabView->minContainerOffset().y - min.y;
        m_tabView->setContentOffset(curr);
    }
}

bool DragonBattleWarListView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){

    return true;
}

void DragonBattleWarListView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    
}

bool DragonBattleWarListView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        CCLoadSprite::doResourceByCommonIndex(502, true);
        CCLoadSprite::doResourceByCommonIndex(504, true);
        CCLoadSprite::doResourceByCommonIndex(306, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(502, false);
            CCLoadSprite::doResourceByCommonIndex(504, false);
            CCLoadSprite::doResourceByCommonIndex(306, false);
        });
        setIsHDPanel(true);
        auto node = CCBLoadFile("DragonBattleWarListView", this, this);
        this->setContentSize(node->getContentSize());
        int prev = m_viewBg->getContentSize().height;
        this->changeBGHeight(m_viewBg);
        int add = m_viewBg->getContentSize().height - prev;
        if (CCCommonUtils::isIosAndroidPad()) {
            add = CCDirector::sharedDirector()->getWinSize().height - 2048;
        }
        m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width,m_infoList->getContentSize().height + add));
        m_infoList->setPositionY(m_infoList->getPositionY() - add);
        m_bottomNode->setPositionY(m_bottomNode->getPositionY() - add);
        
        CCSpriteFrame* sf = CCLoadSprite::getSF("57009_ad1.png");
        if (sf!=NULL) {
            m_dragonBg->initWithSpriteFrame(sf);
        }
        
        m_tipTxt->setString(_lang("140015").c_str());
        m_timeTip->setString(_lang("140206").c_str());
        m_data = CCArray::create();
        fightBeginTime = ActivityController::getInstance()->fightBeginTime;
        
        m_tabView = CCMultiColTableView::create(this, m_infoList->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(Touch_Default);
        m_infoList->addChild(m_tabView);
        
        CCCommonUtils::setButtonTitle(m_helpBtn, _lang("140006").c_str());
        CCCommonUtils::setButtonTitle(m_historyBtn, _lang("140007").c_str());
        CCCommonUtils::setButtonTitle(m_manageBtn, _lang("140043").c_str());
        CCCommonUtils::setButtonTitle(m_sureBtn, _lang("140020").c_str());
        m_manageBtn->setVisible(false);
        
        selectedIndex=-1;
        m_sureBtn->setEnabled(false);
        
        DragonBattleApplyInfoCommand* cmd = new DragonBattleApplyInfoCommand();
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(DragonBattleWarListView::getData), NULL));
        cmd->sendAndRelease();
        
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_infoList);
        m_waitInterface->setPosition(ccp(m_infoList->getContentSize().width/2, m_infoList->getContentSize().height/2));
        
        ret = true;
    }
    return ret;
}

void DragonBattleWarListView::getData(CCObject *param){
    if (m_waitInterface) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dic = _dict(result->getData());
    if(dic){
        CCArray* array = dynamic_cast<CCArray*>(dic->objectForKey("rooms"));
        if (array) {
            m_data->removeAllObjects();
            m_data->addObjectsFromArray(array);
            m_tabView->reloadData();
        }
    }
}


void DragonBattleWarListView::onWarRankClick(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->addPopupInView(DragonHistoryView::create());
}

void DragonBattleWarListView::onClickHelp(CCObject *pSender, CCControlEvent event){
    CCCommonUtils::showHelpShiftSingleFAQ("45216");
}

void DragonBattleWarListView::onManageClick(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->addPopupInView(DragonManageView::create());
}


void DragonBattleWarListView::onSureClick(CCObject *pSender, CCControlEvent event)
{
    if(selectedIndex==-1||this->checkAlreadySigned()){
        return;
    }
    auto cell = this->getCellByIndex(selectedIndex);
    if (cell) {
        ((DragonWarCell *)cell)->warApplyClick();
    }
}
bool DragonBattleWarListView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipTxt", CCLabelIF*, this->m_tipTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_helpBtn", CCControlButton*, this->m_helpBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_manageBtn", CCControlButton*, this->m_manageBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_historyBtn", CCControlButton*, this->m_historyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_dragonBg", CCSprite*, this->m_dragonBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeTip", CCLabelIF*, this->m_timeTip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_sureBtn", CCControlButton*, this->m_sureBtn);
    return false;
}

SEL_CCControlHandler DragonBattleWarListView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onManageClick", DragonBattleWarListView::onManageClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onWarRankClick", DragonBattleWarListView::onWarRankClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickHelp", DragonBattleWarListView::onClickHelp);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSureClick", DragonBattleWarListView::onSureClick);
    return NULL;
}

void DragonBattleWarListView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize DragonBattleWarListView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1536, 130*2.4);
    }
    return CCSize(640, 130);
}

CCTableViewCell* DragonBattleWarListView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, ssize_t idx){
    if(idx >= m_data->count()){
        return NULL;
    }
    DragonWarCell* cell = (DragonWarCell*)table->dequeueGrid();
    CCDictionary* dic = dynamic_cast<CCDictionary*>(m_data->objectAtIndex(idx));
    if(idx < m_data->count()){
        if(cell){
            cell->setData(dic);
        }else{
            cell = DragonWarCell::create(dic,m_infoList);
            cell->setDelegate(this);
            cellList.push_back((CCTableViewCell *)cell);
        }
    }
    cell->updateCellSelectState(cell->getCellIndex()==selectedIndex);
    return cell;
}

ssize_t DragonBattleWarListView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    int num = m_data->count();
    return num;
}

ssize_t DragonBattleWarListView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 1;
}



void DragonBattleWarListView::dragonWarCellClick(TableViewCell *cell)
{
    if (cell->getIdx()==selectedIndex) {
        return;
    }
    if (selectedIndex!=-1) {
        auto cellTemp = this->getCellByIndex(selectedIndex);
        if (cellTemp) {
            ((DragonWarCell *)cellTemp)->updateCellSelectState(false);
        }
    }
    auto dragonCell = (DragonWarCell *)cell;
    selectedIndex=dragonCell->getIdx();
    dragonCell->updateCellSelectState(true);
    
    if (ActivityController::getInstance()->dragonTeamApplyState != 1 || ActivityController::getInstance()->dragonBattleState != 1) {
        m_sureBtn->setEnabled(true);
    }
}

bool DragonBattleWarListView::checkAlreadySigned()
{
    return (ActivityController::getInstance()->dragonTeamApplyState == 1 && ActivityController::getInstance()->dragonBattleState == 1);
}


TableViewCell * DragonBattleWarListView::getCellByIndex(int index)
{
    TableViewCell *cell = NULL;
    for (int i=0; i<cellList.size(); i++) {
        TableViewCell *cellTemp = cellList.at(i);
        if(((DragonWarCell *)cellTemp)->getCellIndex() == index){
            cell = cellTemp;
            break;
        }
    }
    return cell;
}

void DragonBattleWarListView::dragonWarApplySuccess()
{
    m_sureBtn->setEnabled(false);
}

DragonWarCell *DragonWarCell::create(CCDictionary* info,CCNode* clickArea){
    DragonWarCell *ret = new DragonWarCell(info,clickArea);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonWarCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    
}

void DragonWarCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool DragonWarCell::init(){
    auto bg = CCBLoadFile("DragonWarCell", this, this);
    this->setContentSize(bg->getContentSize());
    m_index = 0;
    m_signTxt->setString(_lang("140018"));
    myDelegate=NULL;
    setData(m_info);
    return true;
}

void DragonWarCell::setData(CCDictionary* info){
    m_info = info;
    if (m_info==NULL) {
        return ;
    }
    m_index = m_info->valueForKey("index")->intValue();
    string temp = _lang("140016");
    temp.append(": ");
    m_txt1->setString(temp);
    int num = m_info->valueForKey("num")->intValue();
    m_txt2->setString(_lang_1("140017",CC_ITOA(num)));
    int state = m_info->valueForKey("state")->intValue();
    beginTime = m_info->valueForKey("beginTime")->getCString();
    endtime = m_info->valueForKey("endtime")->getCString();
    m_signNode->setVisible(state==1);
    m_txt4->setString(CC_ITOA(m_index+1));
    m_txt3->setString(_lang_1("140168",CC_ITOA(m_index+1)));
    string timeStr = "";
    time_t startTime = fightBeginTime + atoi(beginTime.c_str())*3600;
    time_t eTime = fightBeginTime + atoi(endtime.c_str())*3600;
    string time1 = CCCommonUtils::timeStampToDate(startTime);
    string time2 = CCCommonUtils::timeStampToDate(eTime);
    int offx = m_txt1->getPositionX() + m_txt1->getContentSize().width*m_txt1->getOriginScaleX() + 20;
    m_time1->setString(time1);
    m_time2->setString(time2);
    m_time1->setPositionX(offx);
    m_time2->setPositionX(offx);
}

bool DragonWarCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_clickNode, pTouch) && isTouchInside(m_clickArea, pTouch)){
        return true;
    }
    return false;
}

void DragonWarCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (ccpDistance(pTouch->getStartLocation(), pTouch->getLocation())>30) {
        return ;
    }
    if(myDelegate!=NULL){
        myDelegate->dragonWarCellClick(this);
    }
}


void DragonWarCell::warApplyClick()
{
    if (ActivityController::getInstance()->dragonTeamApplyState == 1 && ActivityController::getInstance()->dragonBattleState == 1) {
        YesNoDialog::show(_lang("140139").c_str(), NULL);
        return ;
    }
    YesNoDialog::show(_lang_1("140019",CC_ITOA(m_index+1)), CCCallFunc::create(this, callfunc_selector(DragonWarCell::confirmFun)));
}

void DragonWarCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
}

void DragonWarCell::confirmFun(){
    DragonBattleApplyCommand* cmd = new DragonBattleApplyCommand(m_index);
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(DragonWarCell::successCallBack), NULL));
    cmd->sendAndRelease();
}

void DragonWarCell::successCallBack(CCObject* param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    if (result==NULL || m_info==NULL) {
        return;
    }
    auto dic = _dict(result->getData());
    if(dic->objectForKey("state")){
        int state = dic->valueForKey("state")->intValue();
        if (state==0) {
            ActivityController::getInstance()->dragonTeamApplyState = 1;
            ActivityController::getInstance()->dragonBattleState = 1;
            int num = m_info->valueForKey("num")->intValue()+1;
            m_info->setObject(CCString::create(CC_ITOA(num)), "num");
            double fightBeginTime = dic->valueForKey("fightBeginTime")->doubleValue()/1000;
            if (fightBeginTime>=0) {
                ActivityController::getInstance()->fightBeginTime = GlobalData::shared()->changeTime(fightBeginTime);
            }
            m_info->setObject(CCString::create("1"), "state");
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(UPDATE_WAR_ALLIANCE_DATA);
            YesNoDialog::show(_lang("140155").c_str(), NULL);
        }
    }
    if (myDelegate) {
        myDelegate->dragonWarApplySuccess();
    }
}

bool DragonWarCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickNode", CCNode*, this->m_clickNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt1", CCLabelIF*, this->m_txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt2", CCLabelIF*, this->m_txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt3", CCLabelIF*, this->m_txt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt4", CCLabelIF*, this->m_txt4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_time1", CCLabelIF*, this->m_time1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_time2", CCLabelIF*, this->m_time2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_icon", CCNode*, this->m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_signNode", CCNode*, this->m_signNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_signTxt", CCLabelIF*, this->m_signTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_selectNode", CCScale9Sprite*, this->m_selectNode);
    return false;
}

SEL_CCControlHandler DragonWarCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}

int DragonWarCell::getCellIndex()
{
    return m_index;
}

void DragonWarCell::setDelegate(DragonWarCellDelegate *delegate)
{
    myDelegate=delegate;
}

void DragonWarCell::updateCellSelectState(bool selected)
{
    m_selectNode->setVisible(selected);
}
