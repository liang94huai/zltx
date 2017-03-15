//
//  EquipmentBagView.cpp
//  IF
//
//  Created by fubin on 14-12-9.
//
//

#include "EquipmentBagView.h"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "RollingText.h"
#include "YesNoDialog.h"
#include "FunBuildController.h"
#include "QueueController.h"
#include "EquipmentController.h"
#include "SoundController.h"
#include "ToolCreateView.h"

#include "SceneContainer.h"
#include "ImperialScene.h"
#include "StoreView.h"
#include "EquipMeltView.hpp"
#include "EquipComposeView.hpp"

#include "MateCreateView.hpp"
#include "EquipInfoView.hpp"

const int advertiseCellW = 540;
const int advertiseCellW_HD = 1480;
static string BagViewItemId = "";
static bool ForceRefresh = false;

#define BROKEN_EQUIP_SPLIT "_"
#define rp_color {66,156,224}

#pragma mark -
#pragma mark - 材料、装备储藏箱
EquipmentBagView* EquipmentBagView::create(bool isFromWorkshop){
    EquipmentBagView* ret = new EquipmentBagView();
    if(ret && ret->init(isFromWorkshop)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipmentBagView::init(bool isFromWorkshop)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    m_isFromWorkshop=isFromWorkshop;
    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCLoadSprite::doResourceByCommonIndex(306, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::doResourceByCommonIndex(306, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_EQUIP);
    });
    setIsHDPanel(true);
    auto tmpCCB = CCBLoadFile("EquipmentBagView",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else
        this->setContentSize(tmpCCB->getContentSize());
    
    setTitleName(_lang("102160").c_str());
    
    if (CCCommonUtils::isIosAndroidPad()) {
        auto pic = CCLoadSprite::createSprite("Hutch_wall.png");
        auto tBatchNode = CCSpriteBatchNode::createWithTexture(pic->getTexture());
        m_BGNode->addChild(tBatchNode);
        
        int bgcount = (m_infoList->getContentSize().height)/81+2;
        for (int i = 0; i<=bgcount; i++) {
            auto pic = CCLoadSprite::createSprite("Hutch_wall.png");
            pic->setScaleX(2.4);
            pic->setAnchorPoint(CCPoint(0, 0));
            pic->setPosition(ccp(0, -i*81-11));
            tBatchNode->addChild(pic);
            
            auto pic2 = CCLoadSprite::createSprite("Hutch_wall.png");
            pic2->setScaleX(2.4);
            pic2->setAnchorPoint(CCPoint(0, 0));
            pic2->setPosition(ccp(1356, -i*81-11));
            tBatchNode->addChild(pic2);
        }
    }
    else {
        changeBGHeight(m_buildBG);
        int extH = getExtendHeight();
        m_infoList->setContentSize(CCSizeMake(m_infoList->getContentSize().width, m_infoList->getContentSize().height+extH));
        m_bg2->setContentSize(CCSizeMake(m_bg2->getContentSize().width, m_bg2->getContentSize().height+extH));
        auto pic = CCLoadSprite::createSprite("Hutch_wall.png");
        auto tBatchNode = CCSpriteBatchNode::createWithTexture(pic->getTexture());
        m_BGNode->addChild(tBatchNode);
        
        int bgcount = (m_infoList->getContentSize().height+extH)/81+2;
        for (int i = 0; i<=bgcount; i++) {
            auto pic = CCLoadSprite::createSprite("Hutch_wall.png");
            pic->setPosition(ccp(0, -i*81-11));
            tBatchNode->addChild(pic);
            
            auto pic2 = CCLoadSprite::createSprite("Hutch_wall.png");
            pic2->setPosition(ccp(619, -i*81-11));
            tBatchNode->addChild(pic2);
        }
    }
    
    if (CCCommonUtils::isIosAndroidPad()) {
        m_equiBtnTxt->setVisible(true);
        m_mateBtnTxt->setVisible(true);
        m_mateBtnTxt->setString(_lang("119026"));
        m_equiBtnTxt->setString(_lang("119027"));
    }
    else {
        m_equiBtnTxt->setVisible(false);
        m_mateBtnTxt->setVisible(false);
        CCCommonUtils::setButtonTitle(m_mateBtn, _lang("119026").c_str());
        CCCommonUtils::setButtonTitle(m_equiBtn, _lang("119027").c_str());
    }
    
    m_data = CCArray::create();
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_infoList->addChild(m_tabView);
    BagViewItemId = "";
    m_page = 1;
    
    m_equiBtn->setEnabled(true);
    m_mateBtn->setEnabled(false);
    
    for (int j=1; j<=4; j++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",j)->getCString());
        m_fireNode1->addChild(particle);
        
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",j)->getCString());
        m_fireNode2->addChild(particle1);
    }
    m_openNum = 0;
    
    return true;
}

void EquipmentBagView::onEnter(){
    m_tabView->setTouchEnabled(true);
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    UIComponent::getInstance()->showEquipOrBagBtn(2);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);//
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipmentBagView::changeMateCnt), MSG_MATE_CRT_END, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipmentBagView::refreashData), EQUIP_BAG_SHOW_DES, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipmentBagView::setFromWorkshop), "setFromWorkshop", NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipmentBagView::setRefreshView), EQUIP_CLOSE_VIEW, NULL);
    if (m_openNum==0) {
        refreshView();
    }else {
        refreashData(NULL);
    }
    
    setTitleName(_lang("119007"));
    m_openNum++;
}

void EquipmentBagView::onExit(){
    m_tabView->setTouchEnabled(false);
    UIComponent::getInstance()->showEquipOrBagBtn(0);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MATE_CRT_END);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, EQUIP_BAG_SHOW_DES);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "setFromWorkshop");
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, EQUIP_CLOSE_VIEW);
    setTouchEnabled(false);
    if(m_isFromWorkshop){//如果从材料工坊进入，退出时返回材料工坊
//        PopupViewController::getInstance()->addPopupView(ToolCreateView::create());
    }
    CCNode::onExit();
}

void EquipmentBagView::setRefreshView(CCObject *obj){
    refreshView();
}

void EquipmentBagView::setFromWorkshop(CCObject *obj){
    int s=dynamic_cast<CCInteger*>(obj)->getValue();
    if (s>0) {
        m_isFromWorkshop=true;
    }else{
        m_isFromWorkshop=false;
    }
}

bool sortTmpList(string str1, string str2)
{
    int num1 = atoi(str1.c_str());
    int num2 = atoi(str2.c_str());
    if (num1 > num2) {
        return true;
    }else {
        return false;
    }
}

bool sortEquipList1(int itemId1, int itemId2)
{
    auto& eInfo1 = EquipmentController::getInstance()->EquipmentInfoMap[itemId1];
    auto& eInfo2 = EquipmentController::getInstance()->EquipmentInfoMap[itemId2];
    if (eInfo1.level>eInfo2.level) {
        return true;
    }else if (eInfo1.level<eInfo2.level) {
        return false;
    }else {
        if (eInfo1.color>eInfo2.color) {
            return true;
        }else {
            return false;
        }
    }
}

bool sortEquipList(string str1, string str2)
{
    int itemId1 = EquipmentController::getInstance()->m_MyEquipmentMap[str1];
    int itemId2 = EquipmentController::getInstance()->m_MyEquipmentMap[str2];
    return sortEquipList1(itemId1,itemId2);
}
bool sortEquipList2(string str1, string str2)
{
    vector<string> vect1;
    CCCommonUtils::splitString(str1, BROKEN_EQUIP_SPLIT, vect1);
    vector<string> vect2;
    CCCommonUtils::splitString(str2, BROKEN_EQUIP_SPLIT, vect2);
    return sortEquipList1(atoi(vect1.at(0).c_str()),atoi(vect2.at(0).c_str()));
}

void EquipmentBagView::generateData()
{
    m_data->removeAllObjects();
    vector<string> m_curList;
    
    if (m_page == 0) {
        map<int, vector<string>> tmpEquipMap1;
        map<int, vector<string>> tmpEquipMap2;
        map<int,int> tmpBrokenMap;
        map<string, int>::iterator it = EquipmentController::getInstance()->m_MyEquipmentMap.begin();
        for (; it!=EquipmentController::getInstance()->m_MyEquipmentMap.end(); it++)
        {
            auto& tmpInfo = EquipmentController::getInstance()->EquipmentInfoMap[it->second];
            if(tmpInfo.showType == "no")
            {
                tmpEquipMap1[tmpInfo.site].push_back(it->first);
                int newCount = tmpBrokenMap[it->second] + 1;
                tmpBrokenMap[it->second] = newCount;
            }
            else
            {
                tmpEquipMap2[tmpInfo.site].push_back(it->first);
            }
        }
        
        auto tmpItr = tmpBrokenMap.begin();
        while (tmpItr!=tmpBrokenMap.end()) {
            std::string tmpStr = CC_ITOA((*tmpItr).first) + std::string(BROKEN_EQUIP_SPLIT) + CC_ITOA((*tmpItr).second);
            auto tmpItr1 = tmpBrokenMap.find((*tmpItr).first+1);
            if(tmpItr1!=tmpBrokenMap.end()){
                tmpStr = tmpStr + BROKEN_EQUIP_SPLIT +(CC_ITOA((*tmpItr1).second));
            }else{
                tmpStr = tmpStr + BROKEN_EQUIP_SPLIT + "0";
            }
            m_curList.push_back(tmpStr);
            ++tmpItr;
        }
        sort(m_curList.begin(), m_curList.end(),sortEquipList2);
        
//        // 胚子
//        for (int i=0; i<=7; i++)
//        {
//            if (tmpEquipMap1.find(i) != tmpEquipMap1.end()) {
//                sort(tmpEquipMap1[i].begin(), tmpEquipMap1[i].end(), sortEquipList);
//                m_curList.insert(m_curList.end(), tmpEquipMap1[i].begin(), tmpEquipMap1[i].end());
//            }
//        }
        
        
        // 装备
        for (int i=0; i<=7; i++)
        {
            string OnEquipKey = EquipmentController::getInstance()->FindOnEquipBySite(i);
            if (OnEquipKey!="") {
                m_curList.push_back(OnEquipKey);
            }
            
            if (tmpEquipMap2.find(i) != tmpEquipMap2.end()) {
                sort(tmpEquipMap2[i].begin(), tmpEquipMap2[i].end(), sortEquipList);
                m_curList.insert(m_curList.end(), tmpEquipMap2[i].begin(), tmpEquipMap2[i].end());
            }
        }
        
    }
    else {
        for (int i=0; i<ToolController::getInstance()->m_mateTools.size(); i++) {
            int tmpToolId = ToolController::getInstance()->m_mateTools[i];
            ToolInfo& info = ToolController::getInstance()->getToolInfoById(tmpToolId);
            if (info.getCNT()>0){
                m_curList.push_back(CC_ITOA(tmpToolId));
            }
        }
        sort(m_curList.begin(), m_curList.end(), sortTmpList);
    }
    
    string itemIds = "";
    int count = m_curList.size();
    if (CCCommonUtils::isIosAndroidPad()) {
        for (int i=0; i<count; i++)
        {//4个道具 分为一组
            if(i%5==0||i%5==1||i%5==2||i%5==3){
                itemIds.append(m_curList[i]);
                if (i!=(count-1)) {
                    itemIds.append(":");
                }
                else {
                    m_data->addObject(CCString::create(itemIds));
                    itemIds = "";
                }
            }else{
                itemIds += m_curList[i];
                m_data->addObject(CCString::create(itemIds));
                itemIds = "";
            }
        }
        
        int showCount =m_data->count();
        if(showCount<10)
        {//显示空格子 补齐数据
            showCount = 10-showCount;
            for (int j=0; j<showCount; j++) {
                m_data->addObject(CCString::create("-1:-1:-1:-1"));
            }
        }
    }
    else {
        for (int i=0; i<count; i++)
        {//4个道具 分为一组
            if(i%4==0||i%4==1||i%4==2){
                itemIds.append(m_curList[i]);
                if (i!=(count-1)) {
                    itemIds.append(":");
                }
                else {
                    m_data->addObject(CCString::create(itemIds));
                    itemIds = "";
                }
            }else{
                itemIds += m_curList[i];
                m_data->addObject(CCString::create(itemIds));
                itemIds = "";
            }
        }
        
        int showCount =m_data->count();
        if(showCount<10)
        {//显示空格子 补齐数据
            showCount = 10-showCount;
            for (int j=0; j<showCount; j++) {
                m_data->addObject(CCString::create("-1:-1:-1:-1"));
            }
        }
    }
}

void EquipmentBagView::changeMateCnt(CCObject* obj)
{
    ForceRefresh = true;
    refreashData(obj);
    ForceRefresh = false;
}

void EquipmentBagView::refreashData(CCObject* obj){
    float miny = m_tabView->minContainerOffset().y;
    CCPoint pos  = m_tabView->getContentOffset();
    generateData();
    m_tabView->reloadData();
    float mincurry = m_tabView->minContainerOffset().y;
    pos.y+=(-miny+mincurry);
    m_tabView->setContentOffset(pos);
}

void EquipmentBagView::refreshView(){
    generateData();
    m_tabView->reloadData();
}

SEL_CCControlHandler EquipmentBagView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickMateBtn", EquipmentBagView::onClickMateBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickEquiBtn", EquipmentBagView::onClickEquiBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickHelpBtn", EquipmentBagView::onClickHelpBtn);
    return NULL;
}

bool EquipmentBagView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_BGNode", CCNode*, this->m_BGNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg2", CCScale9Sprite*, this->m_bg2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateBtn", CCControlButton*, this->m_mateBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equiBtn", CCControlButton*, this->m_equiBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpBtn", CCControlButton*, this->m_helpBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateBtnTxt", CCLabelIF*, this->m_mateBtnTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equiBtnTxt", CCLabelIF*, this->m_equiBtnTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", CCNode*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", CCNode*, this->m_fireNode2);
    return false;
}

void EquipmentBagView::onClickMateBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    m_equiBtn->setEnabled(true);
    m_mateBtn->setEnabled(false);
    m_page = 1;
    refreshView();
}

void EquipmentBagView::onClickEquiBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    m_equiBtn->setEnabled(false);
    m_mateBtn->setEnabled(true);
    m_page = 0;
    refreshView();
}

void EquipmentBagView::onClickHelpBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    GlobalData::shared()->isBind = true;
#endif
    CCCommonUtils::showHelpShiftSingleFAQ("45213");
}

bool EquipmentBagView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_startPoint = pTouch->getStartLocation();
    if(isTouchInside(m_infoList, pTouch)){
        return false;
    }
    return true;
}

void EquipmentBagView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint endPoint = pTouch->getLocation();
    if(fabs(endPoint.x-m_startPoint.x)>10||fabs(endPoint.y-m_startPoint.y)>10){
        return;
    }
    if(BagViewItemId!=""){
        BagViewItemId="";
        refreashData(NULL);
    }
}

CCSize EquipmentBagView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    
    if (CCCommonUtils::isIosAndroidPad()) {
        if(BagViewItemId==""){
            return CCSize(advertiseCellW_HD, 310);
        }
        string itemId = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();
        if(itemId!=""){
            vector<string> vector1;
            CCCommonUtils::splitString(itemId, ":", vector1);
            int i= 0;
            while (i<vector1.size()) {
                if(vector1[i]!=""){
                    if(vector1[i]==BagViewItemId){
                        //                    return CCSize(advertiseCellW, 366);
                        return CCSize(advertiseCellW_HD, 310);
                    }
                }
                i++;
            }
        }
        return CCSize(advertiseCellW_HD, 310);
    }
    else {
        if(BagViewItemId==""){
            return CCSize(advertiseCellW, 180);
        }
        string itemId = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();
        if(itemId!=""){
            vector<string> vector1;
            CCCommonUtils::splitString(itemId, ":", vector1);
            int i= 0;
            while (i<vector1.size()) {
                if(vector1[i]!=""){
                    if(vector1[i]==BagViewItemId){
                        //                    return CCSize(advertiseCellW, 366);
                        return CCSize(advertiseCellW, 180);
                    }
                }
                i++;
            }
        }
        return CCSize(advertiseCellW, 180);
    }
}

CCSize EquipmentBagView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        CCSize(advertiseCellW_HD, 310);
    }
    return CCSize(advertiseCellW, 360);
}

CCTableViewCell* EquipmentBagView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    
    string itemId = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();
    EquipmentBagCell* cell = (EquipmentBagCell*)table->dequeueCell();
    if(cell){
        cell->setData(itemId, m_page);
    }else{
        cell = EquipmentBagCell::create(itemId, m_page, m_infoList);
    }
    return cell;
}

ssize_t EquipmentBagView::numberOfCellsInTableView(CCTableView *table)
{
    int num = ceil(m_data->count() / 1.0f);
    return num;
}

//class cell
#pragma mark -
#pragma mark - 储藏箱Cell（材料和装备用的同一个）

EquipmentBagCell* EquipmentBagCell::create(string itemIds, int page, CCNode* touchNode)
{
    auto ret = new EquipmentBagCell();
    if (ret && ret->init(itemIds, page, touchNode)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipmentBagCell::init(string itemIds, int page, CCNode* touchNode)
{
    bool ret = true;
    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCLoadSprite::doResourceByCommonIndex(306, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::doResourceByCommonIndex(306, false);
    });
    
    CCBLoadFile("EquipmentBagCell",this,this);
    CCCommonUtils::setButtonTitle(m_crtBtn, _lang("102137").c_str());
    m_selIndex = -99;
    m_waitInterface = NULL;
    m_touchNode = touchNode;
    m_page = page;//0 装备， 1 材料
    setData(itemIds, page);
    return ret;
}

void EquipmentBagCell::setData(string itemIds, int page)
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    m_selIndex=-99;
    m_items = itemIds;
    m_page = page;
    vector<string> tmpVec;
    CCCommonUtils::splitString(m_items, ":", tmpVec);
    m_idsVec.clear();
    if (CCCommonUtils::isIosAndroidPad()) {
        for (int i=0; i<5; i++) {
            if (i<tmpVec.size()) {
                m_idsVec.push_back(tmpVec[i]);
            }
            else {
                m_idsVec.push_back("-1");
            }
        }
        
        bool isDes = false;
        for (int j=0; j<5; j++) {
            if(m_idsVec[j]==BagViewItemId){
                //            isDes = true;
                m_selIndex = j;
                break;
            }
        }
        if(isDes){
            this->setContentSize(CCSize(1480, 620));
        }else{
            this->setContentSize(CCSize(1480, 310));
        }
    }
    else {
        for (int i=0; i<4; i++) {
            if (i<tmpVec.size()) {
                m_idsVec.push_back(tmpVec[i]);
            }
            else {
                m_idsVec.push_back("-1");
            }
        }
        
        bool isDes = false;
        for (int j=0; j<4; j++) {
            if(m_idsVec[j]==BagViewItemId){
                //            isDes = true;
                m_selIndex = j;
                break;
            }
        }
        if(isDes){
            this->setContentSize(CCSize(606, 360));
        }else{
            this->setContentSize(CCSize(606, 180));
        }
    }
    
    m_picNode1->removeAllChildren();
    m_picNode2->removeAllChildren();
    m_picNode3->removeAllChildren();
    m_picNode4->removeAllChildren();
    m_numLabel1->setString("");
    m_numLabel2->setString("");
    m_numLabel3->setString("");
    m_numLabel4->setString("");
    m_itemNode1->setVisible(true);
    m_itemNode2->setVisible(true);
    m_itemNode3->setVisible(true);
    m_itemNode4->setVisible(true);
    m_wor1->setVisible(false);
    m_wor2->setVisible(false);
    m_wor3->setVisible(false);
    m_wor4->setVisible(false);
    m_barBG1->setVisible(false);
    m_barBG2->setVisible(false);
    m_barBG3->setVisible(false);
    m_barBG4->setVisible(false);
    m_yes1Spr->setVisible(false);
    m_yes2Spr->setVisible(false);
    m_yes3Spr->setVisible(false);
    m_yes4Spr->setVisible(false);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_picNode5->removeAllChildren();
        m_numLabel5->setString("");
        m_itemNode5->setVisible(true);
        m_wor5->setVisible(false);
        m_barBG5->setVisible(false);
        m_yes5Spr->setVisible(false);
    }
    if (m_page==1) {
        m_barBG1->setVisible(true);
        m_barBG2->setVisible(true);
        m_barBG3->setVisible(true);
        m_barBG4->setVisible(true);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_barBG5->setVisible(true);
        }
    }
    
    //显示4个单元
    for (int m=0; m<m_idsVec.size(); m++)
    {
        bool isLongjing = false;
        string picPath = "";
        string bgPath = "";
        string num = "";
        int cnt = 0;
        if (m_idsVec[m] != "-1")
        {
            int color = WHITE;
            CCLoadSpriteType equip = CCLoadSpriteType_GOODS;
            if (m_page==1) {
                ToolInfo& info = ToolController::getInstance()->getToolInfoById(atoi(m_idsVec[m].c_str()));
                cnt = info.getCNT();
                num = CC_CMDITOA(cnt);
                picPath = CCCommonUtils::getIcon(CC_ITOA(info.itemId));
                bgPath = CCCommonUtils::getToolBgByColor(info.color);
                color = info.color;
            }
            else {
                int equipId = 0;
                
                if(m_idsVec[m].find(BROKEN_EQUIP_SPLIT)!=string::npos){
                    vector<string> mVec;
                    CCCommonUtils::splitString(m_idsVec[m], BROKEN_EQUIP_SPLIT, mVec);
                    if(mVec.size()>1){
                        equipId = atoi(mVec.at(0).c_str());
                        cnt = atoi(mVec.at(1).c_str());
                        num = CC_CMDITOA(cnt);
                    }
                }else{
                    if (EquipmentController::getInstance()->m_MyOnEquip.find(m_idsVec[m]) != EquipmentController::getInstance()->m_MyOnEquip.end()) {
                        equipId = EquipmentController::getInstance()->m_MyOnEquip[m_idsVec[m]];
                        if (m==0) {
                            m_yes1Spr->setVisible(true);
                        } else if (m==1) {
                            m_yes2Spr->setVisible(true);
                        } else if (m==2) {
                            m_yes3Spr->setVisible(true);
                        } else if (m==3) {
                            m_yes4Spr->setVisible(true);
                        }
                        if (CCCommonUtils::isIosAndroidPad() && m==4) {
                            m_yes5Spr->setVisible(true);
                        }
                    }else {
                        equipId = EquipmentController::getInstance()->m_MyEquipmentMap[m_idsVec[m]];
                    }
                }
                equip=CCLoadSpriteType_EQUIP;
                EquipmentInfo& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[equipId];
                picPath = CCCommonUtils::getIcon(CC_ITOA(eInfo.itemId));
                bgPath = CCCommonUtils::getToolBgByColor(eInfo.color);
                color = eInfo.color;
                if (eInfo.site== 6) {
                    isLongjing = true;
                }
            }
            
            auto iconBg = CCLoadSprite::createSprite(bgPath.c_str());
            auto pic = CCLoadSprite::createSprite(picPath.c_str(),true,equip);
            if (CCCommonUtils::isIosAndroidPad()) {
                CCCommonUtils::setSpriteMaxSize(iconBg, 215,true);
                CCCommonUtils::setSpriteMaxSize(pic, 200,true);
            }
            else {
                CCCommonUtils::setSpriteMaxSize(iconBg, 127,true);
                CCCommonUtils::setSpriteMaxSize(pic, 120,true);
            }
            
            
            if (m==0) {
                m_numLabel1->setString(num);
                m_picNode1->addChild(iconBg);
                m_picNode1->addChild(pic);
                if (/*m_page==1 &&*/ cnt>=4 && color!=GOLDEN){
                    m_wor1->setVisible(true);
                }
                if(cnt>0){
                    m_barBG1->setVisible(true);
                }
                if (isLongjing) {
                    for (int par=0; par<=2; par++) {
                        auto particle = ParticleController::createParticle(CCString::createWithFormat("DragonCristal_%d_%d",color,par)->getCString());
                        if ( CCCommonUtils::isIosAndroidPad() ) {
                            particle->setScale(2.0);
                        }
                        m_picNode1->addChild(particle);
                    }
                }
            } else if (m==1) {
                m_numLabel2->setString(num);
                m_picNode2->addChild(iconBg);
                m_picNode2->addChild(pic);
                if (/*m_page==1 &&*/ cnt>=4 && color!=GOLDEN) {
                    m_wor2->setVisible(true);
                }
                if(cnt>0){
                    m_barBG2->setVisible(true);
                }
                if (isLongjing) {
                    for (int par=0; par<=2; par++) {
                        auto particle = ParticleController::createParticle(CCString::createWithFormat("DragonCristal_%d_%d",color,par)->getCString());
                        if ( CCCommonUtils::isIosAndroidPad() ) {
                            particle->setScale(2.0);
                        }
                        m_picNode2->addChild(particle);
                    }
                }
            } else if (m==2) {
                m_numLabel3->setString(num);
                m_picNode3->addChild(iconBg);
                m_picNode3->addChild(pic);
                if (/*m_page==1 &&*/ cnt>=4 && color!=GOLDEN) {
                    m_wor3->setVisible(true);
                }
                if(cnt>0){
                    m_barBG3->setVisible(true);
                }
                if (isLongjing) {
                    for (int par=0; par<=2; par++) {
                        auto particle = ParticleController::createParticle(CCString::createWithFormat("DragonCristal_%d_%d",color,par)->getCString());
                        if ( CCCommonUtils::isIosAndroidPad() ) {
                            particle->setScale(2.0);
                        }
                        m_picNode3->addChild(particle);
                    }
                }
            } else if (m==3) {
                m_numLabel4->setString(num);
                m_picNode4->addChild(iconBg);
                m_picNode4->addChild(pic);
                if (/*m_page==1 &&*/ cnt>=4 && color!=GOLDEN) {
                    m_wor4->setVisible(true);
                }
                if(cnt>0){
                    m_barBG4->setVisible(true);
                }
                if (isLongjing) {
                    for (int par=0; par<=2; par++) {
                        auto particle = ParticleController::createParticle(CCString::createWithFormat("DragonCristal_%d_%d",color,par)->getCString());
                        if ( CCCommonUtils::isIosAndroidPad() ) {
                            particle->setScale(2.0);
                        }
                        m_picNode4->addChild(particle);
                    }
                }
            } else if (CCCommonUtils::isIosAndroidPad() && m==4) {
                m_numLabel5->setString(num);
                m_picNode5->addChild(iconBg);
                m_picNode5->addChild(pic);
                if (/*m_page==1 &&*/ cnt>=4 && color!=GOLDEN) {
                    m_wor5->setVisible(true);
                }
                if(cnt>0){
                    m_barBG5->setVisible(true);
                }
                if (isLongjing) {
                    for (int par=0; par<=2; par++) {
                        auto particle = ParticleController::createParticle(CCString::createWithFormat("DragonCristal_%d_%d",color,par)->getCString());
                        if ( CCCommonUtils::isIosAndroidPad() ) {
                            particle->setScale(2.0);
                        }
                        m_picNode5->addChild(particle);
                    }
                }
            }
        }
        else
        {
            if (m==0) {
                this->m_itemNode1->setVisible(false);
            } else if (m==1) {
                this->m_itemNode2->setVisible(false);
            } else if (m==2) {
                this->m_itemNode3->setVisible(false);
            } else if (m==3) {
                this->m_itemNode4->setVisible(false);
            } else if (CCCommonUtils::isIosAndroidPad() && m==4){
                this->m_itemNode5->setVisible(false);
            }
        }
    }
    
    m_desNode->setVisible(false);
    if(m_selIndex!=-99){
//        m_desNode->setVisible(true);
//        m_selectSpr->setVisible(true);
//        m_itemsNode->setPositionY(206);
//        m_arrowSpr->setPositionX(93.2+m_selIndex*148);
//        m_selectSpr->setPositionX(85+145*m_selIndex);
        
        if (m_page==1) {
            ToolInfo& info = ToolController::getInstance()->getToolInfoById(atoi(m_idsVec[m_selIndex].c_str()));
            m_nameMateLabel->setString(info.getName().c_str());
//            picPath = CCCommonUtils::getIcon(CC_ITOA(info.itemId));
//            bgPath = CCCommonUtils::getToolBgByColor(info.color);
        }
        else {
            int equipId = 0;
            if(m_idsVec[m_selIndex].find(BROKEN_EQUIP_SPLIT)!=string::npos){
                vector<string> mVec;
                CCCommonUtils::splitString(m_idsVec[m_selIndex], BROKEN_EQUIP_SPLIT, mVec);
                if(mVec.size()>1){
                    equipId = atoi(mVec.at(0).c_str());
                }
            }else{
                if (EquipmentController::getInstance()->m_MyOnEquip.find(m_idsVec[m_selIndex]) != EquipmentController::getInstance()->m_MyOnEquip.end()) {
                    equipId = EquipmentController::getInstance()->m_MyOnEquip[m_idsVec[m_selIndex]];
                }else {
                    equipId = EquipmentController::getInstance()->m_MyEquipmentMap[m_idsVec[m_selIndex]];
                }
            }
            EquipmentInfo& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[equipId];
            m_nameLabel->setString(_lang(eInfo.name).c_str());
            m_desLabel->setString(_lang(eInfo.des).c_str());
        }
        
//        refreshItemData(m_selIndex, ForceRefresh);
        
    }else{
        if (!CCCommonUtils::isIosAndroidPad()) {
            m_itemsNode->setPositionY(20);
        }
        m_desNode->setVisible(false);
        m_selectSpr->setVisible(false);
    }
    
}

void EquipmentBagCell::onEnter()
{
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup_Item, false);
}

void EquipmentBagCell::onExit()
{
    setTouchEnabled(false);
    CCNode::onExit();
}

void EquipmentBagCell::refreashData(CCObject* obj)
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    string numStr =  CC_CMDITOA(info.getCNT());
    if(m_selIndex==0){
        m_numLabel1->setString(numStr);
    }else if(m_selIndex==1){
        m_numLabel1->setString(numStr);
    }else if(m_selIndex==2){
        m_numLabel1->setString(numStr);
    }else if(m_selIndex==3){
        m_numLabel1->setString(numStr);
    }else if (CCCommonUtils::isIosAndroidPad() && m_selIndex==4){
        m_numLabel1->setString(numStr);
    }
    if(info.getCNT() <= 0){
        ToolController::getInstance()->m_currentUseItemId=0;
    }
}

bool EquipmentBagCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_clickInSide = false;
    m_startPoint = pTouch->getStartLocation();
    if (isTouchInside(m_touchNode,pTouch)){
        m_clickInSide = true;
        if(isTouchInside(this,pTouch)){
            return true;
        }
    }
    return false;
}

void EquipmentBagCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint endPoint = pTouch->getLocation();
    if(fabs(endPoint.x-m_startPoint.x)>10||fabs(endPoint.y-m_startPoint.y)>10){
        return;
    }
    
    if (isTouchInside(m_picBG1,pTouch)){
        if(m_idsVec[0]!="-1"){
            showDesByIndex(0);
        }
    }else if (isTouchInside(m_picBG2,pTouch)){
        if(m_idsVec[1]!="-1"){
            showDesByIndex(1);
        }
    }else if (isTouchInside(m_picBG3,pTouch)){
        if(m_idsVec[2]!="-1"){
            showDesByIndex(2);
        }
    }else if (isTouchInside(m_picBG4,pTouch)){
        if(m_idsVec[3]!="-1"){
            showDesByIndex(3);
        }
    }else if (CCCommonUtils::isIosAndroidPad() && isTouchInside(m_picBG5, pTouch)){
        if (m_idsVec[4]!="-1") {
            showDesByIndex(4);
        }
    }
}

void EquipmentBagCell::refreshItemData(int index, bool st)
{
    if(index==-1){
        BagViewItemId="";
    }else{
//        if(m_selIndex==index && !st){
//            return;
//        }
        m_selIndex= index;
        BagViewItemId = m_idsVec[m_selIndex];
        
        m_mateNode1->removeAllChildren();
        m_mateNode2->removeAllChildren();
        m_mateNode3->removeAllChildren();
        m_mateNode4->removeAllChildren();
        m_mateNode5->removeAllChildren();
        
        m_itemId = 0;
        if (m_page==1) {
            // 点击材料
//            PopupViewController::getInstance()->addPopupView(MaterialInfoView::create(atoi(m_idsVec[m_selIndex].c_str())));
            ToolInfo& info = ToolController::getInstance()->getToolInfoById(atoi(m_idsVec[m_selIndex].c_str()));
            if (info.color == GOLDEN) {
                CCCommonUtils::flyHint("", "", _lang("121987"));
                PopupViewController::getInstance()->addPopupView(MaterialInfoView::create(atoi(m_idsVec[m_selIndex].c_str())));
            } else {
                PopupViewController::getInstance()->addPopupInView(MateCreateView::create(info.itemId,0,0,0));
            }
        }
        else {
            // 点击装备
            if(m_idsVec[m_selIndex].find(BROKEN_EQUIP_SPLIT)!=string::npos){
                vector<string> vect;
                CCCommonUtils::splitString(m_idsVec[m_selIndex], BROKEN_EQUIP_SPLIT, vect);
                if(vect.size()>2)
                {
                    map<string, int>::iterator iter = EquipmentController::getInstance()->m_MyEquipmentMap.begin();
                    int equipId = atoi(vect.at(0).c_str());
                    string eUuid = "";
                    while(iter!=EquipmentController::getInstance()->m_MyEquipmentMap.end()){
                        if(iter->second == equipId){
                            eUuid = iter->first;
                            break;
                        }
                        iter++;
                    }
                    bool st = CCCommonUtils::isOpenFormDataCfg(EquipmentController::getInstance()->m_equipComposeKey);
                    if (eUuid != "" && st) {
                        PopupViewController::getInstance()->addPopupView(EquipInfoView::create(eUuid));
                    }
                    else {
                        PopupViewController::getInstance()->addPopupView(EquipInfoView::createWithBrokenEquip(atoi(vect.at(0).c_str()), atoi(vect.at(1).c_str()), atoi(vect.at(2).c_str())));
                    }
                }
            }else{
                int equipId = 0;
                if (EquipmentController::getInstance()->m_MyOnEquip.find(m_idsVec[m_selIndex]) != EquipmentController::getInstance()->m_MyOnEquip.end()) {
                    equipId = EquipmentController::getInstance()->m_MyOnEquip[m_idsVec[m_selIndex]];
                }else {
                    equipId = EquipmentController::getInstance()->m_MyEquipmentMap[m_idsVec[m_selIndex]];
                }
                
                PopupViewController::getInstance()->addPopupView(EquipInfoView::create(m_idsVec[m_selIndex]));
            }
        }
    }
}

void EquipmentBagCell::showDesByIndex(int index)
{
    refreshItemData(index);
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(EQUIP_BAG_SHOW_DES);
}

void EquipmentBagCell::onClickCrtBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if(!m_clickInSide) {
        return ;
    }
    EquipmentController::getInstance()->startMaterialCreate(m_itemId);
}

SEL_CCControlHandler EquipmentBagCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCrtBtn", EquipmentBagCell::onClickCrtBtn);
    return NULL;
}

bool EquipmentBagCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemsNode", CCNode*, m_itemsNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode1", CCNode*, m_itemNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode2", CCNode*, m_itemNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode3", CCNode*, m_itemNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode4", CCNode*, m_itemNode4);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode1", CCNode*, m_picNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode2", CCNode*, m_picNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode3", CCNode*, m_picNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode4", CCNode*, m_picNode4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel1", CCLabelIF*, m_numLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel2", CCLabelIF*, m_numLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel3", CCLabelIF*, m_numLabel3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel4", CCLabelIF*, m_numLabel4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBG1", CCSprite*, m_picBG1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBG2", CCSprite*, m_picBG2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBG3", CCSprite*, m_picBG3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBG4", CCSprite*, m_picBG4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barBG1", CCSprite*, m_barBG1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barBG2", CCSprite*, m_barBG2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barBG3", CCSprite*, m_barBG3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barBG4", CCSprite*, m_barBG4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectSpr", CCSprite*, m_selectSpr);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wor1", CCSprite*, m_wor1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wor2", CCSprite*, m_wor2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wor3", CCSprite*, m_wor3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wor4", CCSprite*, m_wor4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_yes1Spr", CCSprite*, m_yes1Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_yes2Spr", CCSprite*, m_yes2Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_yes3Spr", CCSprite*, m_yes3Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_yes4Spr", CCSprite*, m_yes4Spr);
    
    if (CCCommonUtils::isIosAndroidPad()) {
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode5", CCNode*, m_itemNode5);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode5", CCNode*, m_picNode5);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel5", CCLabelIF*, m_numLabel5);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBG5", CCSprite*, m_picBG5);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barBG5", CCSprite*, m_barBG5);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_wor5", CCSprite*, m_wor5);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_yes5Spr", CCSprite*, m_yes5Spr);
    }
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode", CCNode*, m_desNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemBg", CCScale9Sprite*, m_itemBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrowSpr", CCSprite*, m_arrowSpr);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipNode", CCNode*, m_equipNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode", CCNode*, m_mateNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameMateLabel", CCLabelIF*, m_nameMateLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_crtBtn", CCControlButton*, m_crtBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode1", CCNode*, m_mateNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode2", CCNode*, m_mateNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode3", CCNode*, m_mateNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode4", CCNode*, m_mateNode4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode5", CCNode*, m_mateNode5);
    return false;
}

#pragma mark -
#pragma mark - 材料信息面板

MaterialInfoView* MaterialInfoView::create(int itemId)
{
    auto ret = new MaterialInfoView();
    if (ret && ret->init(itemId)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MaterialInfoView::init(int itemId)
{
    bool ret = true;
    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    
    if (PopupBaseView::init()) {
        setIsHDPanel(true);
        CCBLoadFile("MaterialInfoView",this,this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        setData(itemId);
    }
    return ret;
}

void MaterialInfoView::setData(int itemId)
{
    m_waitInterface = NULL;

    m_itemId = itemId;
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    
    m_nameLabel->setString(_lang("102271"));
    m_name1Label->setString(info.getName());
    m_numLabel->setString(_lang_1("104907", CC_ITOA(info.getCNT())));
    m_msg2Label->setString(_lang("108730"));
    m_info1Label->setString(_lang(info.des));
    m_info2Label->setString(_lang("119078"));
    
    if (info.color == GOLDEN)// 已经达到最大级
    {
        m_mateBtn->setVisible(false);
        m_msg1Label->setVisible(true);
        m_msg1Label->setString(_lang("121987"));
    }
    else if(info.getCNT() < 4) // 数量不足
    {
        m_mateBtn->setVisible(false);
        m_msg1Label->setVisible(true);
        m_msg1Label->setString(_lang("119078"));
    }
    else
    {
        m_mateBtn->setVisible(true);
        m_msg1Label->setVisible(false);
    }
    
    string iconPath = CCCommonUtils::getIcon(CC_ITOA(m_itemId));
    string bgPath = CCCommonUtils::getToolBgByColor(info.color);
    
    auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_EQUIP);
    if (CCCommonUtils::isIosAndroidPad()) {
        CCCommonUtils::setSpriteMaxSize(icon, 200, true);
    }
    else
        CCCommonUtils::setSpriteMaxSize(icon, 100, true);
    auto bg = CCLoadSprite::createSprite(bgPath.c_str());
    if (CCCommonUtils::isIosAndroidPad()) {
        CCCommonUtils::setSpriteMaxSize(bg, 200, true);
    }
    else
        CCCommonUtils::setSpriteMaxSize(bg, 100, true);
    m_picNode->addChild(bg);
    m_picNode->addChild(icon);
    
    CCCommonUtils::setButtonTitle(m_mateBtn, _lang("119028").c_str());
}

void MaterialInfoView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

void MaterialInfoView::onExit()
{
    setTouchEnabled(false);
    CCNode::onExit();
}

bool MaterialInfoView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_touchNode,pTouch)){
        return false;
    }
    return true;
}

void MaterialInfoView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isTouchInside(m_touchNode,pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
    }
}

SEL_CCControlHandler MaterialInfoView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickMateBtn", MaterialInfoView::onClickMateBtn);
    return NULL;
}

bool MaterialInfoView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateBtn", CCControlButton*, m_mateBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name1Label", CCLabelIF*, m_name1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg1Label", CCLabelIF*, m_msg1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg2Label", CCLabelIF*, m_msg2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info1Label", CCLabelIF*, m_info1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info2Label", CCLabelIF*, m_info2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, m_numLabel);
    return false;
}

void MaterialInfoView::onClickMateBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("setFromWorkshop",CCInteger::create(0));
    PopupViewController::getInstance()->addPopupInView(MateCreateView::create(m_itemId,0,0,0));
    closeSelf();
}

#pragma mark -
#pragma mark 装备分解面板
EquipBreakItemView* EquipBreakItemView::create(string data)
{
    auto ret = new EquipBreakItemView();
    if (ret && ret->init(data)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipBreakItemView::init(string data)
{
    bool ret = true;
    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    
    if (PopupBaseView::init()) {
        setIsHDPanel(true);
        CCBLoadFile("EquipBreakItemView",this,this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        
        m_nameLabel->setString(_lang("119054"));
        
        m_tabView = CCTableView::create(this, m_infoList->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(0);
        m_infoList->addChild(m_tabView);
        
        CCCommonUtils::setButtonTitle(m_okBtn, _lang("confirm").c_str());
        
        setData(data);
    }
    return ret;
}

void EquipBreakItemView::setData(string data)
{
    m_data.clear();
    m_numVec.clear();
    int sumNum = 0;
    vector<string> mateVec;
    CCCommonUtils::splitString(data, ";", mateVec);
    for (int i=0; i<mateVec.size(); i++)
    {
        vector<string> mateCell;
        CCCommonUtils::splitString(mateVec[i], "|", mateCell);
        if (mateCell.size()==2) {
            m_data.push_back(atoi(mateCell[0].c_str()));
            m_numVec.push_back(atoi(mateCell[1].c_str()));
            sumNum += atoi(mateCell[1].c_str());
        }
    }
    m_msg2Label->setString(_lang_1("119055", CC_ITOA(sumNum)));
    
    m_tabView->reloadData();
}

void EquipBreakItemView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void EquipBreakItemView::onExit()
{
    setTouchEnabled(false);
    CCNode::onExit();
}

bool EquipBreakItemView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}
void EquipBreakItemView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isTouchInside(m_touchNode,pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
    }
}

SEL_CCControlHandler EquipBreakItemView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickOKBtn", EquipBreakItemView::onClickOKBtn);
    return NULL;
}

bool EquipBreakItemView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_okBtn", CCControlButton*, m_okBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg1Label", CCLabelIF*, m_msg1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg2Label", CCLabelIF*, m_msg2Label);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, m_infoList);
    return false;
}

void EquipBreakItemView::onClickOKBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
}

CCSize EquipBreakItemView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(900, 150);
    }
    return CCSize(620, 90);
}
CCSize EquipBreakItemView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(900, 150);
    }
    return CCSize(620, 90);
}

CCTableViewCell* EquipBreakItemView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if (idx >= m_data.size() || idx >= m_numVec.size()) {
        return NULL;
    }
    
    int itemId = m_data[idx];
    EquipMateCell* cell = (EquipMateCell*)table->dequeueCell();
    if (cell) {
        cell->setData(itemId, m_numVec[idx]);
    } else {
        cell = EquipMateCell::create(itemId, m_numVec[idx]);
    }
    return cell;
}

ssize_t EquipBreakItemView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data.size();
}

//class cell
#pragma mark -
#pragma mark - 草药合成Cell
EquipMateCell* EquipMateCell::create(int itemId, int num)
{
    auto ret = new EquipMateCell();
    if (ret && ret->init(itemId, num)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipMateCell::init(int itemId, int num)
{
    bool ret = true;
    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCBLoadFile("EquipMateCell",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        setContentSize(CCSize(900, 150));
    }
    else
        setContentSize(CCSize(620, 90));
    setData(itemId, num);
    return ret;
}

void EquipMateCell::setData(int itemId, int num)
{
    m_itemId = itemId;
    m_num = num;
    
    auto& tool = ToolController::getInstance()->getToolInfoById(itemId);
    
    string iconPath = CCCommonUtils::getIcon(CC_ITOA(m_itemId));
    string bgPath = CCCommonUtils::getToolBgByColor(tool.color);
    
    auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_GOODS);
    if (CCCommonUtils::isIosAndroidPad()) {
        CCCommonUtils::setSpriteMaxSize(icon, 120, true);
    }
    else
        CCCommonUtils::setSpriteMaxSize(icon, 60, true);
    auto bg = CCLoadSprite::createSprite(bgPath.c_str());
    if (CCCommonUtils::isIosAndroidPad()) {
        CCCommonUtils::setSpriteMaxSize(bg, 120, true);
    }
    else
        CCCommonUtils::setSpriteMaxSize(bg, 60, true);
    m_picNode->addChild(bg);
    m_picNode->addChild(icon);
    
    m_nameLabel->setString(tool.getName());
    
    string numMsg = "";
    numMsg = numMsg + "+" + CC_ITOA(m_num);
    m_levelLabel->setString(numMsg);
}

void EquipMateCell::onEnter() {
    CCNode::onEnter();
}
void EquipMateCell::onExit() {
    CCNode::onExit();
}
SEL_CCControlHandler EquipMateCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}
bool EquipMateCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelLabel", CCLabelIF*, m_levelLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    return false;
}
