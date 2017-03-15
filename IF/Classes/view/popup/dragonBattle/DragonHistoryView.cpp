//
//  DragonHistoryView.cpp
//  IF
//
//  Created by ganxiaohua on 15/8/27.
//
//

#include "DragonHistoryView.h"
#include "QuestController.h"
#include "PopupViewController.h"
#include "RewardController.h"
#include "UIComponent.h"
#include "ChangePicCommand.h"
#include "ToolController.h"
#include "UseToolView.h"
#include "WorldController.h"
#include "ParticleFireAni.h"
#include "YuanJunDetailView.h"
#include "CancelAllianceTeamCommand.h"
#include "AllianceFlagPar.h"
#include "AllianceManager.h"
#include "AllianceFightInfo.h"
#include "WorldCommand.h"
#include "DragonBattleRankCommand.h"
#include "DragonBattleRewardView.h"
#include "DragonRankInfo.h"
#include "ActivityController.h"
#include "DragonBattlePrizeView.hpp"

#define  HISTORY_CELL_CLICK "HISTORY_CELL_CLICK"
#define  HISTORY_CELL_UPDATE_DATA "HISTORY_CELL_UPDATE_DATA"
static int dataLen = 16;

DragonHistoryView *DragonHistoryView::create(){
    DragonHistoryView *ret = new DragonHistoryView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonHistoryView::onEnter(){
    this->setTitleName(_lang("140034"));
    PopupBaseView::onEnter();
    setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonHistoryView::updateData), HISTORY_CELL_CLICK, NULL);
    //CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonHistoryView::updateCellData), HISTORY_CELL_UPDATE_DATA, NULL);
}

void DragonHistoryView::onExit(){
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, HISTORY_CELL_CLICK);
    //CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, HISTORY_CELL_UPDATE_DATA);
    PopupBaseView::onExit();
}

bool DragonHistoryView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void DragonHistoryView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){

}

void DragonHistoryView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){

}

bool DragonHistoryView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(204, true);
        CCLoadSprite::doResourceByCommonIndex(504, true);
        CCLoadSprite::doResourceByCommonIndex(11, true);
        CCLoadSprite::doResourceByCommonIndex(205, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(204, false);
            CCLoadSprite::doResourceByCommonIndex(504, false);
            CCLoadSprite::doResourceByCommonIndex(7, false);
            CCLoadSprite::doResourceByCommonIndex(11, false);
            CCLoadSprite::doResourceByCommonIndex(205, false);
        });
        auto node = CCBLoadFile("DragonHistoryView", this, this);
        this->setContentSize(node->getContentSize());
        
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
        }
        if (CCCommonUtils::isIosAndroidPad()) {
            tBatchNode->setScaleX(1536.0 / 640.0);
            tBatchNode->setScaleY(2048.0 / 852.0);
        }
        this->addChild(tBatchNode,-10);
        int preHeight = this->m_viewBg->getContentSize().height;
        changeBGHeight(m_viewBg);
        m_viewBg->setVisible(false);
        int dh = m_viewBg->getContentSize().height - preHeight;
        if (CCCommonUtils::isIosAndroidPad()) {
            dh = CCDirector::sharedDirector()->getWinSize().height - 2048;
        }
        this->m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height + dh));
        m_infoList->setPositionY(m_infoList->getPositionY()-dh);
        m_buttom->setPositionY(m_buttom->getPositionY() - dh);
 
        m_aTitleTxt->setString(_lang_1("140070",""));
        m_aRankTxt->setString(_lang_1("140071",""));
        
        CCCommonUtils::setButtonTitle(m_rewardBtn, _lang("138003").c_str());
        
        m_data = CCArray::create();
        
        m_scrollView = CCScrollView::create(m_infoList->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_infoList->addChild(m_scrollView);
        
        for (int i=0; i<6; i++) {
            DragonHistoryRecordCell* record = DragonHistoryRecordCell::create(i,m_infoList);
            m_scrollView->addChild(record);
            m_data->addObject(record);
        }
        m_scrollView->setVisible(false);
        
        string index_limit = "";
        std::map<std::string,DragonRankInfo>::iterator it;
        for (it = ActivityController::getInstance()->dragonCatalogMap.begin(); it!=ActivityController::getInstance()->dragonCatalogMap.end(); it++){
            if (index_limit!="") {
                index_limit.append(";");
            }
            index_limit.append(CC_ITOA(it->second.min_A-1));
            index_limit.append("|");
            index_limit.append(CC_ITOA(dataLen));
        }
        DragonBattleRankCommand* cmd = new DragonBattleRankCommand(index_limit);
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(DragonHistoryView::getSerVerData), NULL));
        cmd->sendAndRelease();
        
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_infoList);
        m_waitInterface->setPosition(m_infoList->getContentSize().width/2, m_infoList->getContentSize().height/2);
        ret = true;
    }
    return ret;
}

void DragonHistoryView::getSerVerData(CCObject* param){
    if (m_waitInterface) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    m_scrollView->setVisible(true);
    updatePosition(0,0);
    string allianceId = GlobalData::shared()->playerInfo.allianceInfo.uid;
    std::map<std::string,DragonAllianceRankInfo*>::iterator it = ActivityController::getInstance()->dragonRankMap.find(allianceId);
    if (it!=ActivityController::getInstance()->dragonRankMap.end()) {
        string temp = ActivityController::getInstance()->getTitileByRank(it->second->rankNum);
        m_titleValue->setString(temp.c_str());
        m_rankValue->setString(CC_ITOA(it->second->rankNum));
        
    }else{
        m_titleValue->setString(_lang("140145").c_str());
        m_rankValue->setString("10000+");
    }
    
}

void DragonHistoryView::updateData(CCObject* param){
    CCString* strObj = dynamic_cast<CCString*>(param);
    if (strObj) {
        string str = strObj->getCString();
        int index = -1;
        int type = -1;
        std::vector<std::string> vArr;
        CCCommonUtils::splitString(str,"_",vArr);
        if (vArr.size()==1) {
            type = atoi(vArr[0].c_str());
        }
        if (vArr.size()==2) {
            type = atoi(vArr[0].c_str());
            index = atoi(vArr[1].c_str());
        }
        this->updatePosition(type,index);
    }
}

void DragonHistoryView::updateCellData(CCObject* param){
    CCString* strObj = dynamic_cast<CCString*>(param);
    if (strObj) {
        int total = 0;
        int num = m_data->count();
        bool subOpenFlag = false;
        CCPoint offPos = m_scrollView->getContentOffset();
        for (int i=0; i<num; i++) {
            DragonHistoryRecordCell* record = dynamic_cast<DragonHistoryRecordCell*>(m_data->objectAtIndex(i));
            record->upatePosition();
            total += record->getCellHeight();
        }
        int add = total - m_totalH;
        m_totalH = total;
        total -= 80;
        for (int i=0; i<num; i++) {
            DragonHistoryRecordCell* record = dynamic_cast<DragonHistoryRecordCell*>(m_data->objectAtIndex(i));
            record->setPositionY(total);
            total -= record->getCellHeight();
        }
        
        m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,m_totalH));
        m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - m_totalH - add));
    }
}

void DragonHistoryView::updatePosition(int type,int index,bool update){
    int total = 0;
    int num = m_data->count();
    bool subOpenFlag = false;
    CCPoint offPos = m_scrollView->getContentOffset();
    for (int i=0; i<num; i++) {
        DragonHistoryRecordCell* record = dynamic_cast<DragonHistoryRecordCell*>(m_data->objectAtIndex(i));
        if(type==i && index != -1){
            subOpenFlag = record->showTowMembers(index);
        }else if (type==i && !record->isOpen) {
            subOpenFlag = record->showTowMembers(index);
        }else{
            record->reset();
        }
        total += record->getCellHeight();
    }
    m_totalH = total;
    total -= 80;
    if (CCCommonUtils::isIosAndroidPad()) {
        total -= 80*2.4;
    }
    for (int i=0; i<num; i++) {
        DragonHistoryRecordCell* record = dynamic_cast<DragonHistoryRecordCell*>(m_data->objectAtIndex(i));
        record->setPositionY(total);
        total -= record->getCellHeight();
    }
    m_scrollView->setTouchStop(!subOpenFlag);
    int offH = 0;
    if (subOpenFlag && type!=0) {
        if (CCCommonUtils::isIosAndroidPad()) {
            offH = type * 94 * 2.4 + index * 64 * 2.4 + 30 * 4;
        }else{
            offH = type*94 + index*64 + 30;
        }
    }
    m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,m_totalH));
    m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - m_totalH +offH));
    
}

void DragonHistoryView::onRewardClick(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->addPopupInView(DragonBattlePrizeView::create());
}

bool DragonHistoryView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_aTitleTxt", CCLabelIF*, this->m_aTitleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_aRankTxt", CCLabelIF*, this->m_aRankTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardBtn", CCControlButton*, this->m_rewardBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tip", CCLabelIF*, this->m_tip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_buttom", CCNode*, this->m_buttom);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleValue", CCLabelIF*, this->m_titleValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rankValue", CCLabelIF*, this->m_rankValue);
    return false;
}

SEL_CCControlHandler DragonHistoryView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRewardClick", DragonHistoryView::onRewardClick);
    return NULL;
}

//

DragonHistoryRecordCell *DragonHistoryRecordCell::create(int type,CCNode* clickArea){
    DragonHistoryRecordCell *ret = new DragonHistoryRecordCell(type,clickArea);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonHistoryRecordCell::onEnter(){
    setSwallowsTouches(false);
    setTouchEnabled(true);
    
    CCNode::onEnter();
}

void DragonHistoryRecordCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool DragonHistoryRecordCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(m_bg, pTouch) && isTouchInside(m_clickArea, pTouch)) {
        return true;
    }
    if(m_type==0 && isTouchInside(m_topNode, pTouch) && isOpen && !m_haveSend){
        return true;
    }
    return false;
}

void DragonHistoryRecordCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void DragonHistoryRecordCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (ccpDistance(pTouch->getStartLocation(), pTouch->getLocation())>50) {
        if (m_type==0 && isOpen && !m_haveSend && isTouchInside(m_topNode, pTouch)) {
            string index_limit = "";
            std::string mapKey = CC_ITOA(m_type+1);
            mapKey.append("_");
            mapKey.append(CC_ITOA(0));
            int len = 0;
            std::map<std::string,DragonRankInfo>::iterator it = ActivityController::getInstance()->dragonCatalogMap.find(mapKey);
            if (it!=ActivityController::getInstance()->dragonCatalogMap.end()) {
                index_limit.append(CC_ITOA(it->second.min_A-1+dataLen));
                index_limit.append("|");
                len = it->second.max_A-it->second.min_A +1 - dataLen;
                if (len>200) {
                    len = 200;
                }
            }
            index_limit.append(CC_ITOA(len));
            DragonBattleRankCommand* cmd = new DragonBattleRankCommand(index_limit);
            cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(DragonHistoryRecordCell::getServerData), NULL));
            cmd->sendAndRelease();
            m_haveSend = true;
        }
        return ;
    }
    if (isTouchInside(m_bg, pTouch) && isTouchInside(m_clickArea, pTouch)) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(HISTORY_CELL_CLICK,CCString::create(CC_ITOA(m_type)));
    }
}

void DragonHistoryRecordCell::getServerData(CCObject* param){
    string temp = CC_ITOA(m_type);
    temp.append("_");
    temp.append(CC_ITOA(0));
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(HISTORY_CELL_CLICK,CCString::create(temp.c_str()));
}

bool DragonHistoryRecordCell::init(){
    auto node = CCBLoadFile("DragonHistoryRecordCell", this, this);
    this->setContentSize(node->getContentSize());
    
    m_haveSend = false;
    m_srcData = CCArray::create();
    m_data = CCArray::create();
    m_topData = CCArray::create();
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    int m_tableHeight = winSize.height - 260;
    
    int w = 640;
    if (CCCommonUtils::isIosAndroidPad()) {
        w = winSize.width;
    }
    m_topContainer = CCNode::create();
    this->addChild(m_topContainer);

    string titleStr = "";
    switch (m_type) {
        case 0:
            titleStr = _lang("140029");
            break;
        case 1:
            titleStr = _lang_1("140028","");
            break;
        case 2:
            titleStr = _lang_1("140027","");
            break;
        case 3:
            titleStr = _lang_1("140026","");
            break;
        case 4:
            titleStr = _lang_1("140025","");
            break;
        case 5:
            titleStr = _lang_1("140024","");
            break;
    }
    m_titleTxt->setString(titleStr.c_str());
    
    m_rankIcon->removeAllChildrenWithCleanup(true);
    auto spr = CCLoadSprite::createSprite(CCString::createWithFormat("duanwei_%d.png",m_type+1)->getCString());
    spr->setScale(0.7);
    m_rankIcon->addChild(spr);
    
    isOpen = false;
    m_totalH = 0;
    m_topTabView = NULL;
    
    std::vector<int> vArr;
    std::map<std::string,DragonRankInfo>::iterator it;
    for (it = ActivityController::getInstance()->dragonCatalogMap.begin(); it!=ActivityController::getInstance()->dragonCatalogMap.end(); it++) {
        int tempType = it->second.rank - 1;
        if (m_type==tempType) {
            vArr.push_back(it->second.level);
        }
    }
    if(m_type!=0){
        int num = vArr.size();
        for (int i=0; i<num; i++) {
            DragonHistoryTowCell* record = DragonHistoryTowCell::create(CCDictionary::create(),vArr[i],m_type,m_clickArea);
            m_data->addObject(record);
        }
    }else{
        m_topNode = CCNode::create();
        m_topNode->setContentSize(CCSize(w,m_tableHeight));
        m_topNode->setAnchorPoint(ccp(0, 0));
        m_topContainer->addChild(m_topNode);
        m_topNode->setPositionY(-m_tableHeight);
        
        m_topTabView = CCMultiColTableView::create(this, m_topNode->getContentSize());
        m_topTabView->setDirection(kCCScrollViewDirectionVertical);
        m_topTabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_topTabView->setTouchPriority(Touch_Default);
        m_topNode->addChild(m_topTabView);
        m_topTabView->setTouchStop(false);
        showTowMembers(0);
    }
    return true;
}

void DragonHistoryRecordCell::initTopData(){
    m_topData->removeAllObjects();
    CCArray* arr = ActivityController::getInstance()->getDataByRankAndLevel(m_type, 0);
    m_topData->addObjectsFromArray(arr);
    m_topTabView->reloadData();
    int tempH = 0;
    if (CCCommonUtils::isIosAndroidPad()) {
        tempH = 180*2.4*arr->count();
    }else{
        tempH = 180*arr->count();
    }
    if(tempH>0){
        tempH = MAX(tempH,600);
    }
    m_totalH -= tempH;
}

bool DragonHistoryRecordCell::updateTowData(int index){
    int num = m_data->count();
    bool subOpenFlag = false;
    int tempIndex = index - 1;
    for (int i=0; i<num; i++) {
        DragonHistoryTowCell* record = dynamic_cast<DragonHistoryTowCell*>(m_data->objectAtIndex(i));
        if (tempIndex==i && record && !record->isOpen) {
            record->showMember();
            subOpenFlag = true;
        }else{
            record->reset();
        }
        if (record->getParent()==NULL) {
            this->addChild(record);
        }
    }
    upatePosition();
    return subOpenFlag;
}

void DragonHistoryRecordCell::upatePosition(){
    int total = -80;
    if (CCCommonUtils::isIosAndroidPad()) {
        total = -80 * 2.4;
    }
    int num = m_data->count();
    for (int i=0; i<num; i++) {
        DragonHistoryTowCell* record = dynamic_cast<DragonHistoryTowCell*>(m_data->objectAtIndex(i));
        record->setPositionY(total);
        total -= record->getCellHeight();
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        m_totalH = -total - 40 *2.4;
    }else{
        m_totalH = -total - 40;
    }
    
}

int DragonHistoryRecordCell::getCellHeight(){
    int h = 94;
    if (CCCommonUtils::isIosAndroidPad()) {
        h = 94*2.4;
    }
    if (isOpen) {
        h += m_totalH;
    }
    return h;
}

void DragonHistoryRecordCell::reset(){
    isOpen = false;
    m_arrow->setRotation(0);
    int num = m_data->count();
    for (int i=0; i<num; i++) {
        DragonHistoryTowCell* record = dynamic_cast<DragonHistoryTowCell*>(m_data->objectAtIndex(i));
        record->reset();
        record->removeFromParent();
    }
    m_topData->removeAllObjects();
    if (m_topTabView) {
        m_topTabView->reloadData();
        m_topTabView->setTouchStop(false);
    }
}

bool DragonHistoryRecordCell::showTowMembers(int index){
    isOpen = true;
    m_arrow->setRotation(90);
    bool subOpenFlag = false;
    if (m_type==0 && m_topTabView) {
        initTopData();
        subOpenFlag = true;
        m_topTabView->setTouchStop(true);
    }else{
        subOpenFlag = updateTowData(index);
    }
    return subOpenFlag;
}

void DragonHistoryRecordCell::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize DragonHistoryRecordCell::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1536, 180*2.4);
    }
    return CCSize(642, 180);
}

CCTableViewCell* DragonHistoryRecordCell::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, ssize_t idx){
    if(idx >= m_topData->count()){
        return NULL;
    }
    DragonHistoryTopCell* cell = (DragonHistoryTopCell*)table->dequeueGrid();
    DragonAllianceRankInfo* info = (DragonAllianceRankInfo*)m_topData->objectAtIndex(idx);
    if(idx < m_topData->count()){
        if(cell){
            cell->setData(info);
        }else{
            cell = DragonHistoryTopCell::create(info);
        }
    }
    return cell;
}

ssize_t DragonHistoryRecordCell::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    int num = m_topData->count();
    return num;
}

ssize_t DragonHistoryRecordCell::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 1;
}

bool DragonHistoryRecordCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIFTTF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rankIcon", CCNode*, this->m_rankIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bg", CCNode*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_arrow", CCSprite*, this->m_arrow);
    return false;
}

SEL_CCControlHandler DragonHistoryRecordCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}
// tow
DragonHistoryTowCell *DragonHistoryTowCell::create(CCDictionary* info,int index,int type,CCNode* clickArea){
    DragonHistoryTowCell *ret = new DragonHistoryTowCell(info,index,type,clickArea);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonHistoryTowCell::onEnter(){
    setSwallowsTouches(false);
    setTouchEnabled(true);
    CCNode::onEnter();
}

void DragonHistoryTowCell::onExit(){
    setTouchEnabled(false);
    
    CCNode::onExit();
}

bool DragonHistoryTowCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){

    return true;
}

void DragonHistoryTowCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void DragonHistoryTowCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (ccpDistance(pTouch->getStartLocation(), pTouch->getLocation())>30) {
        if (!m_haveSend && isOpen && isTouchInside(m_tNode, pTouch) & isTouchInside(m_clickArea, pTouch)) {
            string index_limit = "";
            std::string mapKey = CC_ITOA(m_type+1);
            mapKey.append("_");
            mapKey.append(CC_ITOA(m_index));
            int len = 0;
            std::map<std::string,DragonRankInfo>::iterator it = ActivityController::getInstance()->dragonCatalogMap.find(mapKey);
            if (it!=ActivityController::getInstance()->dragonCatalogMap.end()) {
                index_limit.append(CC_ITOA(it->second.min_A-1+dataLen));
                index_limit.append("|");
                len = it->second.max_A-it->second.min_A +1 - dataLen;
                if (len>200) {
                    len = 200;
                }
            }
            index_limit.append(CC_ITOA(len));
            DragonBattleRankCommand* cmd = new DragonBattleRankCommand(index_limit);
            cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(DragonHistoryTowCell::callMoreBack), NULL));
            cmd->sendAndRelease();
            //m_waitInterface = GameController::getInstance()->showWaitInterface(m_tNode);
            //m_waitInterface->setPosition(m_tNode->getContentSize().width/2, m_tNode->getContentSize().height/2);
            m_haveSend = true;
        }
        return ;
    }
    if (isTouchInside(m_clickNode, pTouch) && isTouchInside(m_clickArea, pTouch)) {
        string temp = CC_ITOA(m_type);
        temp.append("_");
        temp.append(CC_ITOA(m_index));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(HISTORY_CELL_CLICK,CCString::create(temp.c_str()));
    }
}

void DragonHistoryTowCell::callMoreBack(CCObject* param){
//    if (m_waitInterface) {
//        m_waitInterface->remove();
//        m_waitInterface = NULL;
//    }
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dic = _dict(result->getData());
    if(dic && !dic->objectForKey("errorCode")){
        string temp = CC_ITOA(m_type);
        temp.append("_");
        temp.append(CC_ITOA(m_index));
        //CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(HISTORY_CELL_UPDATE_DATA,CCString::create(temp.c_str()));
        if(isOpen){
            showMember();
        }
    }

}

bool DragonHistoryTowCell::init(){
    auto node = CCBLoadFile("DragonHistoryTowCell", this, this);
    this->setContentSize(node->getContentSize());
    
    m_srcData = CCArray::create();
    for(int i=0;i<50;i++){
        m_srcData->addObject(CCDictionary::create());
    }
    
    m_data = CCArray::create();
    isOpen = false;
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    m_tableHeight = winSize.height - 270;
    
    int w = 640;
    if (CCCommonUtils::isIosAndroidPad()) {
        w = winSize.width;
    }
    m_tNode = CCNode::create();
    m_tNode->setContentSize(CCSize(w,m_tableHeight));
    m_tNode->setAnchorPoint(ccp(0, 0));
    node->addChild(m_tNode,-1000);
    m_tNode->setPositionY(-m_tableHeight);
    
    m_tabView = CCMultiColTableView::create(this, m_tNode->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_tNode->addChild(m_tabView);
    m_tabView->setTouchStop(false);
    
    m_haveSend = false;
    string titleStr = "";
    switch (m_type) {
        case 1:
            titleStr = _lang_1("140028",CC_ITOA(m_index));
            break;
        case 2:
            titleStr = _lang_1("140027",CC_ITOA(m_index));
            break;
        case 3:
            titleStr = _lang_1("140026",CC_ITOA(m_index));
            break;
        case 4:
            titleStr = _lang_1("140025",CC_ITOA(m_index));
            break;
        case 5:
            titleStr = _lang_1("140024",CC_ITOA(m_index));
            break;
    }
    m_titleTxt->setString(titleStr.c_str());
    m_rankIcon->removeAllChildrenWithCleanup(true);
    auto spr = CCLoadSprite::createSprite(CCString::createWithFormat("duanwei_%d.png",m_type+1)->getCString());
    spr->setScale(0.5);
    m_rankIcon->addChild(spr);
    
    auto levelBg = CCLoadSprite::createSprite("title_level_bg.png");
    levelBg->setScale(0.5);
    levelBg->setPositionY(-12);
    m_rankIcon->addChild(levelBg);
    
    auto roman = CCCommonUtils::getRomanSprite(m_index);
    roman->setScale(0.8);
    roman->setPositionY(-12);
    m_rankIcon->addChild(roman);
    
    return true;
}

int DragonHistoryTowCell::getCellHeight(){
    int h = 64;
    if (CCCommonUtils::isIosAndroidPad()) {
        h = 64*2.4;
        if (isOpen) {
            int temp = m_tableHeight+100*2.4;
            int num = m_data->count()/2 + (m_data->count()%2==0?0:1);
            h += num*150*2.4;
            h = MAX(temp, h);
        }
    }else{
        h = 64;
        if (isOpen) {
            int temp = m_tableHeight+100;
            int num = m_data->count()/2 + (m_data->count()%2==0?0:1);
            h += num*150;
            h = MAX(temp, h);
        }
    }
    return h;
}

void DragonHistoryTowCell::reset(){
    isOpen = false;
    m_arrow->setRotation(270);
    bool flag = m_data->count()>0;
    m_data->removeAllObjects();
    if (flag) {
        m_tabView->reloadData();
    }
    m_tabView->setTouchStop(false);
}

void DragonHistoryTowCell::showMember(){
    isOpen = true;
    m_arrow->setRotation(0);
    m_data->removeAllObjects();
    CCArray* arr = ActivityController::getInstance()->getDataByRankAndLevel(m_type, m_index);
    m_data->addObjectsFromArray(arr);
    m_tabView->setTouchStop(true);
    if (m_data->count()>0) {
         m_tabView->reloadData();
    }
}

void DragonHistoryTowCell::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize DragonHistoryTowCell::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(320*2.4, 150*2.4);
    }
    return CCSize(320, 150);
}

CCTableViewCell* DragonHistoryTowCell::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, ssize_t idx){
    if(idx >= m_data->count()){
        return NULL;
    }
    DragonHistoryMemberCell* cell = (DragonHistoryMemberCell*)table->dequeueGrid();
    DragonAllianceRankInfo* info = (DragonAllianceRankInfo*)m_data->objectAtIndex(idx);
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info);
        }else{
            cell = DragonHistoryMemberCell::create(info);
        }
    }
    return cell;
}

ssize_t DragonHistoryTowCell::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    int num = m_data->count();
    return num/2 + (num%2==1?1:0);
}

ssize_t DragonHistoryTowCell::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 2;
}

void DragonHistoryTowCell::setData(CCDictionary* info,int index){
    
}

bool DragonHistoryTowCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIFTTF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rankIcon", CCNode*, this->m_rankIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickNode", CCNode*, this->m_clickNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_arrow", CCSprite*, this->m_arrow);
    return false;
}

SEL_CCControlHandler DragonHistoryTowCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}

//
DragonHistoryMemberCell *DragonHistoryMemberCell::create(DragonAllianceRankInfo* info){
    DragonHistoryMemberCell *ret = new DragonHistoryMemberCell(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonHistoryMemberCell::onEnter(){
    CCNode::onEnter();
}

void DragonHistoryMemberCell::onExit(){
    CCNode::onExit();
}

bool DragonHistoryMemberCell::init(){
    auto node = CCBLoadFile("DragonHistoryMemberCell", this, this);
    this->setContentSize(node->getContentSize());
    setData(m_info);
    return true;
}

void DragonHistoryMemberCell::setData(DragonAllianceRankInfo* info){
    m_info = info;
    if (m_info==NULL) {
        return;
    }
    string temp = m_info->name;
    if(temp.length()>9){
        temp = CCCommonUtils::subStrByUtf8(temp, 0, 9);
        temp.append("...");
    }
    m_allianceNameTxt->setString(temp);
    m_rankTxt->setString(_lang_1("140023", CC_CMDITOA(m_info->rankNum).c_str()));
    m_powerTxt->setString(CC_CMDITOAD(m_info->score));
}

bool DragonHistoryMemberCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_allianceNameTxt", CCLabelIF*, this->m_allianceNameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rankTxt", CCLabelIF*, this->m_rankTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_powerTxt", CCLabelIF*, this->m_powerTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_flagNode", CCNode*, this->m_flagNode);
    return false;
}

SEL_CCControlHandler DragonHistoryMemberCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}

//
DragonHistoryTopCell *DragonHistoryTopCell::create(DragonAllianceRankInfo* info){
    DragonHistoryTopCell *ret = new DragonHistoryTopCell(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonHistoryTopCell::onEnter(){
    CCNode::onEnter();
}

void DragonHistoryTopCell::onExit(){
    CCNode::onExit();
}

bool DragonHistoryTopCell::init(){
    auto node = CCBLoadFile("DragonHistoryTopCell", this, this);
    this->setContentSize(node->getContentSize());
    setData(m_info);
    return true;
}

void DragonHistoryTopCell::setData(DragonAllianceRankInfo* info){
    m_info = info;
    if (m_info==NULL) {
        return;
    }
    string name = info->name;
    if(name.length()>13){
        name = CCCommonUtils::subStrByUtf8(name,0,12);
        name.append("...");
    }
    m_allianceNameTxt->setString(name);
    m_rankTxt->setString(_lang_1("140023", CC_CMDITOA(info->rankNum).c_str()));
    m_powerTxt->setString(CC_CMDITOAD(info->score));
    int win = info->wins*100;
    string temp = CC_ITOA(win);
    temp.append("%");
    m_winTxt->setString(_lang_1("140033", temp.c_str()));
    temp = info->kingName;
    if (info->kingName=="") {
        temp.append("#");
        temp.append(CC_ITOA(info->kingId));
    }
    string kstr = _lang_1("140031", temp.c_str());
    if(kstr.length()>17){
        kstr = CCCommonUtils::subStrByUtf8(kstr,0,16);
        kstr.append("...");
    }
    m_kingdomTxt->setString(kstr);
    string icon = info->icon;
    if(icon==""){
        icon = "Allance_flay";
    }
    icon.append(".png");
    m_flagNode->removeAllChildrenWithCleanup(true);
    AllianceFlagPar* flag = AllianceFlagPar::create(icon);
    flag->setScale(0.7);
    m_flagNode->addChild(flag);
}

bool DragonHistoryTopCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rankTxt", CCLabelIF*, this->m_rankTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_powerTxt", CCLabelIF*, this->m_powerTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_kingdomTxt", CCLabelIF*, this->m_kingdomTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_winTxt", CCLabelIF*, this->m_winTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_allianceNameTxt", CCLabelIF*, this->m_allianceNameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_flagNode", CCNode*, this->m_flagNode);
    return false;
}

SEL_CCControlHandler DragonHistoryTopCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}