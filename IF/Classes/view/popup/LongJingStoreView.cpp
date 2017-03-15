//
//  LongJingStoreView.cpp
//  IF
//
//  Created by zhangjun-zj on 15-8-14.
//
//

#include "StoreView.h"
#include "LongJingStoreView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "YesNoDialog.h"
#include "ToolController.h"
#include "UseCDToolView.h"
#include "YesNoDialog.h"
#include "FunBuildController.h"
#include "ChangeNickNameView.h"
#include "WorldMapView.h"
#include "ChangePicView.h"
#include "BuildingAddOutPutCommand.h"
#include "SceneController.h"
#include "LongJingBuyView.h"
#include "StoreMallCellTip.h"
#include "ParticleController.h"

#include "EquipmentBagView.h"
#include "EquipmentController.h"

#define r1_color {216,177,146}
#define r2_color {224,149,78}
#define r3_color {66,156,224}

static int advertiseCellW = 540;
static int advertiseCellW_HD = 1480;
const float numPerRow = 1.0;

LongJingStoreView* LongJingStoreView::create(StoreType storeType, StoreResourceType storeResourceType){
    LongJingStoreView* ret = new LongJingStoreView(storeType, storeResourceType);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool LongJingStoreView::init()
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
    auto tmpCCB = CCBLoadFile("LongJingStoreView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    setTitleName(_lang("102160").c_str());
    
    int oldBgHeight = m_buildBG->getContentSize().height;
    changeBGHeight(m_buildBG);
    addHeight = getExtendHeight();
    m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
    m_infoList->setContentSize(m_infoList->getContentSize() + CCSizeMake(0, addHeight));
    
    m_bagNode->setPositionY(m_bagNode->getPositionY()-addHeight);
    m_bagNode->setContentSize(m_bagNode->getContentSize() + CCSizeMake(0, addHeight));
    
    CCCommonUtils::setButtonTitle(m_storeBtn, _lang("111652").c_str());
    CCCommonUtils::setButtonTitle(m_bagBtn, _lang("111718").c_str());

    auto size = CCDirector::sharedDirector()->getWinSize();
    int count = size.height/44+1;
    for (int i=0; i<count; i++) {
        auto spr = CCLoadSprite::createSprite("Items_bg_big.png");
        if (CCCommonUtils::isIosAndroidPad()){
            spr->setScaleX(1.2);
        }
        this->m_BGNode->addChild(spr);
        spr->setPositionY(-i*44);
    }
    
    listSize = m_infoList->getContentSize();
    m_data = CCArray::create();
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_tabView->setDelegate(this);
    m_infoList->addChild(m_tabView);
    
    m_cityLv = FunBuildController::getInstance()->getMainCityLv();

    updateInfo();
    changeData(1);
    
    m_desNode->setVisible(false);
    
    return true;
}

void LongJingStoreView::onEnter()
{
    CCNode::onEnter();
    setTouchEnabled(true);
}

void LongJingStoreView::onExit()
{
    setTouchEnabled(false);
    CCNode::onExit();
}

void LongJingStoreView::updateInfo()
{
    generateData();
    m_tabView->reloadData();
}

void LongJingStoreView::changeData(int type)
{
    m_type = type;
    m_bagNode->removeAllChildren();
    if (m_type == 1)
    { //商店
        m_bagNode->setVisible(false);
        if (m_bagNode->getPositionX() > -99999) {
            m_bagNode->setPositionX( m_bagNode->getPositionX()-999999 );
        }
        m_infoList->setVisible(true);
        if (m_infoList->getPositionX() < -99999) {
            m_infoList->setPositionX( m_infoList->getPositionX()+999999 );
        }
        
        m_storeBtn->setEnabled(false);
        m_bagBtn->setEnabled(true);
        m_storeSubNode->setVisible(true);
        
        m_tabView->reloadData();
    }
    else {//背包
        m_infoList->setVisible(false);
        if (m_infoList->getPositionX() > -99999) {
            m_infoList->setPositionX( m_infoList->getPositionX()-999999 );
        }
        m_bagNode->setVisible(true);
        if (m_bagNode->getPositionX() < -99999) {
            m_bagNode->setPositionX( m_bagNode->getPositionX()+999999 );
        }
        
        m_storeBtn->setEnabled(true);
        m_bagBtn->setEnabled(false);
        m_storeSubNode->setVisible(false);
        
        auto bagView = LongJingBagView::create(m_bagNode->getContentSize());
        m_bagNode->addChild(bagView);
    }
}

CCSize LongJingStoreView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(advertiseCellW, 300);
    }
    return CCSize(advertiseCellW, 200);
}

CCSize LongJingStoreView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(advertiseCellW, 300);
    }
    return CCSize(advertiseCellW, 200);
}

CCTableViewCell* LongJingStoreView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    
    string itemId = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();
    vector<string> vector1;
    CCCommonUtils::splitString(itemId, ":", vector1);
    if(vector1.size()==2){
        string item1=vector1[0];
        string item2=vector1[1];
        LongJingCell* cell = (LongJingCell*)table->dequeueCell();
        if(cell){
            cell->setData(atoi(item1.c_str()),atoi(item2.c_str()));
        }else{
            cell = LongJingCell::create(atoi(item1.c_str()),atoi(item2.c_str()),m_infoList);
        }
        cell->delegate=this;
        return cell;
    }
    return NULL;
}

ssize_t LongJingStoreView::numberOfCellsInTableView(CCTableView *table)
{
    int num = ceil(m_data->count() / 1.0f);
    return num;
}

void LongJingStoreView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
    
}

void LongJingStoreView::scrollViewDidScroll(CCScrollView* view){
    //    CCLOG("%f", view->getContentOffset().x);
    float mindy = m_tabView->minContainerOffset().y-20;
    float dy = m_tabView->getContentOffset().y;
    if(dy<mindy){
        m_tabView->setContentOffset(ccp(0, mindy));
    }
}

SEL_CCControlHandler LongJingStoreView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickHelpBtn", LongJingStoreView::onClickHelpBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBagBtn", LongJingStoreView::onClickBagBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickStoreBtn", LongJingStoreView::onClickStoreBtn);
    return NULL;
}

bool LongJingStoreView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_BGNode", CCNode*, this->m_BGNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_storeBtn", CCControlButton*, this->m_storeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bagBtn", CCControlButton*, this->m_bagBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_storeSubNode", CCNode*, this->m_storeSubNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bagNode", CCNode*, this->m_bagNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode", CCNode*, this->m_desNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desbg", CCScale9Sprite*, this->m_desbg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desName", CCLabelIF*, this->m_desName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLine1", CCSprite*, this->m_desLine1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_valueNode", CCNode*, this->m_valueNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNum1", CCLabelIF*, this->m_desNum1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNum2", CCLabelIF*, this->m_desNum2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNum3", CCLabelIF*, this->m_desNum3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNum4", CCLabelIF*, this->m_desNum4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNum5", CCLabelIF*, this->m_desNum5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desPara1", CCLabelIF*, this->m_desPara1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desPara2", CCLabelIF*, this->m_desPara2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desPara3", CCLabelIF*, this->m_desPara3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desPara4", CCLabelIF*, this->m_desPara4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desPara5", CCLabelIF*, this->m_desPara5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLine2", CCSprite*, this->m_desLine2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLv", CCLabelIF*, this->m_desLv);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIF*, this->m_msgLabel);
    return false;
}

void LongJingStoreView::showTipNode(int itemId,CCTouch *pTouch){
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(itemId);
    
    string itemName = info.getName();
    m_desName->setString(itemName);
    string limitLv = "Lv:";
    limitLv+=CC_ITOA(info.limitLv);
    m_desLv->setString(limitLv);
    
    int extH = 0;
    m_msgLabel->setString("");
    m_msgLabel->setPositionY(33);
    m_desLine2->setPositionY(38);
    int equipItemId = atoi(info.equipId.c_str());
    if (EquipmentController::getInstance()->EquipmentInfoMap.find(equipItemId) != EquipmentController::getInstance()->EquipmentInfoMap.end() )
    {
        auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[equipItemId];
        if (eInfo.prv_level > 0) {
            auto& needEquip = EquipmentController::getInstance()->EquipmentInfoMap[eInfo.prv_level];
            string colorName = CCCommonUtils::getColorName(needEquip.color);
            string equipName = _lang(needEquip.name);
            m_msgLabel->setString(_lang_2("111688", colorName.c_str(), equipName.c_str()));
            if ( EquipmentController::getInstance()->IsHaveLongjing(eInfo.prv_level, eInfo.color) ) {
                m_msgLabel->setColor(ccGREEN);
            }else {
                m_msgLabel->setColor(ccRED);
            }
            int msgH = m_msgLabel->getContentSize().height*m_msgLabel->getOriginScaleY();
            if (msgH > 40) {
                extH = msgH - 20;
                m_msgLabel->setPositionY(m_msgLabel->getPositionY() + extH );
                m_desLine2->setPositionY(m_desLine2->getPositionY() + extH );
            }
        }
    }
    
    string show1 = CCCommonUtils::getPropById(info.equipId, "show1");
    string num1 = CCCommonUtils::getPropById(info.equipId, "num1");
    string para1 = CCCommonUtils::getPropById(info.equipId, "para1");
    string dialog_initial = CCCommonUtils::getPropById(info.equipId, "dialog_initial");
    string num_initial = CCCommonUtils::getPropById(info.equipId, "num_initial");
    string ran_initial = CCCommonUtils::getPropById(info.equipId, "ran_initial");
    
    vector<string> sVec;
    CCCommonUtils::splitString(show1, "|", sVec);
    vector<string> nVec;
    CCCommonUtils::splitString(num1, "|", nVec);
    vector<string> pVec;
    CCCommonUtils::splitString(para1, "|", pVec);
    vector<string> extSVec;
    CCCommonUtils::splitString(dialog_initial, "|", extSVec);
    vector<string> extNVec;
    CCCommonUtils::splitString(num_initial, "|", extNVec);
    vector<string> extPVec;
    CCCommonUtils::splitString(ran_initial, "|", extPVec);
    
    m_desPara1->setString("");
    m_desPara2->setString("");
    m_desPara3->setString("");
    m_desPara4->setString("");
    m_desPara5->setString("");
    m_desNum1->setString("");
    m_desNum2->setString("");
    m_desNum3->setString("");
    m_desNum4->setString("");
    m_desNum5->setString("");
    m_desPara1->setColor(r1_color);
    m_desPara2->setColor(r1_color);
    m_desPara3->setColor(r1_color);
    m_desPara4->setColor(r1_color);
    m_desPara5->setColor(r1_color);
    m_desNum1->setColor(r2_color);
    m_desNum2->setColor(r2_color);
    m_desNum3->setColor(r2_color);
    m_desNum4->setColor(r2_color);
    m_desNum5->setColor(r2_color);
    
    float nodeW = m_desNode->getContentSize().width;
    float cNodeH = 150;
    if (sVec.size()==1 && nVec.size()==1 && pVec.size()==1)
    {
        string format1 = "%";
        string pm1 = "+";
        vector<string> tmpVec1;
        CCCommonUtils::splitString(pVec[0], "@", tmpVec1);
        if (tmpVec1.size()>0) {
            format1 = CCCommonUtils::getEffFormat(tmpVec1[0]);
            pm1 = CCCommonUtils::getEffPM(tmpVec1[0]);
        }
        
        m_desPara1->setString(_lang(sVec[0].c_str()));
        m_desNum1->setString( pm1+nVec[0]+format1);
        cNodeH = 30;
        m_desName->setColor({83,151,33});
        if (extSVec.size()==1 && extNVec.size()==1 && extPVec.size()==1) {
            string fEx1 = "%";
            string exPm1 = "+";
            vector<string> tpVec1;
            CCCommonUtils::splitString(extPVec[0], "@", tpVec1);
            if (tpVec1.size()>0) {
                fEx1 = CCCommonUtils::getEffFormat(tpVec1[0]);
                exPm1 = CCCommonUtils::getEffPM(tpVec1[0]);
            }
            m_desPara2->setString(_lang(extSVec[0]).c_str());
            m_desNum2->setString( exPm1+extNVec[0]+fEx1 );
            m_desPara2->setColor(r3_color);
            m_desNum2->setColor(r3_color);
            cNodeH = 60;
        }
        if (info.color>=2) {
            m_desName->setColor({71,132,225});
        }
    }
    else if (sVec.size()==2 && nVec.size()==2 && pVec.size()==2)
    {
        string format1 = "%";
        string pm1 = "+";
        vector<string> tmpVec1;
        CCCommonUtils::splitString(pVec[0], "@", tmpVec1);
        if (tmpVec1.size()>0) {
            format1 = CCCommonUtils::getEffFormat(tmpVec1[0]);
            pm1 = CCCommonUtils::getEffPM(tmpVec1[0]);
        }
        string format2 = "%";
        string pm2 = "+";
        vector<string> tmpVec2;
        CCCommonUtils::splitString(pVec[1], "@", tmpVec2);
        if (tmpVec2.size()>0) {
            format2 = CCCommonUtils::getEffFormat(tmpVec2[0]);
            pm2 = CCCommonUtils::getEffPM(tmpVec2[0]);
        }
        
        m_desPara1->setString(_lang(sVec[0].c_str()));
        m_desNum1->setString( pm1+nVec[0]+format1);
        m_desPara2->setString(_lang(sVec[1].c_str()));
        m_desNum2->setString( pm2+nVec[1]+format2);
        
        if (extSVec.size()>=1 && extNVec.size()>=1 && extPVec.size()>=1) {
            string fEx1 = "%";
            string exPm1 = "+";
            vector<string> tpVec1;
            CCCommonUtils::splitString(extPVec[0], "@", tpVec1);
            if (tpVec1.size()>0) {
                fEx1 = CCCommonUtils::getEffFormat(tpVec1[0]);
                exPm1 = CCCommonUtils::getEffPM(tpVec1[0]);
            }
            m_desPara3->setString(_lang(extSVec[0]).c_str());
            m_desNum3->setString( exPm1+extNVec[0]+fEx1 );
            m_desPara3->setColor(r3_color);
            m_desNum3->setColor(r3_color);
            cNodeH = 90;
        }
        if (extSVec.size()>=2 && extNVec.size()>=2 && extPVec.size()>=2) {
            string fEx1 = "%";
            string exPm1 = "+";
            vector<string> tpVec1;
            CCCommonUtils::splitString(extPVec[1], "@", tpVec1);
            if (tpVec1.size()>0) {
                fEx1 = CCCommonUtils::getEffFormat(tpVec1[0]);
                exPm1 = CCCommonUtils::getEffPM(tpVec1[0]);
            }
            m_desPara4->setString(_lang(extSVec[1]).c_str());
            m_desNum4->setString( exPm1+extNVec[1]+fEx1 );
            m_desPara4->setColor(r3_color);
            m_desNum4->setColor(r3_color);
            cNodeH = 120;
        }
        if (info.color>=3) {
            m_desName->setColor({112,72,194});
        }
        if (info.color>=4) {
            m_desName->setColor({193,129,72});
        }
    }
    else if (sVec.size()==3 && nVec.size()==3 && pVec.size()==3)
    {
        string format1 = "%";
        string pm1 = "+";
        vector<string> tmpVec1;
        CCCommonUtils::splitString(pVec[0], "@", tmpVec1);
        if (tmpVec1.size()>0) {
            format1 = CCCommonUtils::getEffFormat(tmpVec1[0]);
            pm1 = CCCommonUtils::getEffPM(tmpVec1[0]);
        }
        string format2 = "%";
        string pm2 = "+";
        vector<string> tmpVec2;
        CCCommonUtils::splitString(pVec[1], "@", tmpVec2);
        if (tmpVec2.size()>0) {
            format2 = CCCommonUtils::getEffFormat(tmpVec2[0]);
            pm2 = CCCommonUtils::getEffPM(tmpVec2[0]);
        }
        string format3 = "%";
        string pm3 = "+";
        vector<string> tmpVec3;
        CCCommonUtils::splitString(pVec[2], "@", tmpVec3);
        if (tmpVec3.size()>0) {
            format3 = CCCommonUtils::getEffFormat(tmpVec3[0]);
            pm3 = CCCommonUtils::getEffPM(tmpVec3[0]);
        }
        m_desPara1->setString(_lang(sVec[0].c_str()));
        m_desNum1->setString( pm1+nVec[0]+format1);
        m_desPara2->setString(_lang(sVec[1].c_str()));
        m_desNum2->setString( pm2+nVec[1]+format2);
        m_desPara3->setString(_lang(sVec[2].c_str()));
        m_desNum3->setString( pm3+nVec[2]+format3);
        if (extSVec.size()>=1 && extNVec.size()>=1 && extPVec.size()>=1) {
            string fEx1 = "%";
            string exPm1 = "+";
            vector<string> tpVec1;
            CCCommonUtils::splitString(extPVec[0], "@", tpVec1);
            if (tpVec1.size()>0) {
                fEx1 = CCCommonUtils::getEffFormat(tpVec1[0]);
                exPm1 = CCCommonUtils::getEffPM(tpVec1[0]);
            }
            m_desPara4->setString(_lang(extSVec[0]).c_str());
            m_desNum4->setString( exPm1+extNVec[0]+fEx1 );
            m_desPara4->setColor(r3_color);
            m_desNum4->setColor(r3_color);
        }
        if (extSVec.size()>=2 && extNVec.size()>=2 && extPVec.size()>=2) {
            string fEx1 = "%";
            string exPm1 = "+";
            vector<string> tpVec1;
            CCCommonUtils::splitString(extPVec[1], "@", tpVec1);
            if (tpVec1.size()>0) {
                fEx1 = CCCommonUtils::getEffFormat(tpVec1[0]);
                exPm1 = CCCommonUtils::getEffPM(tpVec1[0]);
            }
            m_desPara5->setString(_lang(extSVec[1]).c_str());
            m_desNum5->setString( exPm1+extNVec[1]+fEx1 );
            m_desPara5->setColor(r3_color);
            m_desNum5->setColor(r3_color);
        }
        m_desName->setColor({205,183,63});
    }
    
    m_valueNode->setPositionY(m_desLine2->getPositionY() + cNodeH );
    m_desLine1->setPositionY(m_valueNode->getPositionY() + 10);
    m_desName->setPositionY(m_desLine1->getPositionY() + 20 );
    m_desNode->setContentSize(CCSize(nodeW, m_desName->getPositionY()+20));
    m_desbg->setContentSize(m_desNode->getContentSize());
    
    CCPoint tp = pTouch->getStartLocation();
    int dH=addHeight;
    if (CCCommonUtils::isIosAndroidPad())
    {
        dH=0;
        nodeW*=2;
    }
    m_desNode->setPositionY(tp.y-dH);
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float px = tp.x<(size.width-nodeW)?tp.x:(size.width-nodeW);
    m_desNode->setPositionX(px);
    m_desNode->setVisible(true);
    return;
    
}

void LongJingStoreView::hideTipNode(){
    
}

bool LongJingStoreView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (!isTouchInside(m_desNode,pTouch)) {
        if (m_desNode->isVisible()) {
            m_desNode->setVisible(false);
        }
    }
    return false;
}
void LongJingStoreView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}

void LongJingStoreView::onClickHelpBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    CCCommonUtils::showHelpShiftSingleFAQ("45214");
}

void LongJingStoreView::onClickBagBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    if (m_desNode->isVisible()) {
        m_desNode->setVisible(false);
    }
    changeData(2);
}

void LongJingStoreView::onClickStoreBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    if (m_desNode->isVisible()) {
        m_desNode->setVisible(false);
    }
    changeData(1);
}

void LongJingStoreView::generateData(){
    m_data->removeAllObjects();

//    m_dataList = &ToolController::getInstance()->m_warList;
    m_dataList = &ToolController::getInstance()->m_typeTools[9];
    
    m_curList.clear();
    for (int i=0; i<m_dataList->size(); i++) {
        int tmpToolId = (*m_dataList)[i];
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(tmpToolId);
        if (info.LongJing_price>0){// && info.limitLv<=m_cityLv
             m_curList.push_back(tmpToolId);
         }
    }
    string itemId = "";
    int count = m_curList.size();
    for (int i=0; i<count; i++) {
        int tmpToolId = (m_curList)[i];
        if(i%2==0){
            itemId = CC_ITOA(tmpToolId);
        }else{
            itemId.append(":");
            itemId += CC_ITOA(tmpToolId);
            m_data->addObject(CCString::create(itemId));
            itemId = "";
        }
    }
    if(count%2==1){
        itemId.append(":");
        itemId.append("-1");
        m_data->addObject(CCString::create(itemId));
    }
}

# pragma mark LongJingCell
LongJingCell::LongJingCell():m_itemId(0),m_itemId1(0),m_price(0),m_boostBid(0),m_touchNode(NULL),m_isClick(false){
}
LongJingCell* LongJingCell::create(int itemId, int itemId1, CCNode* touchNode)
{
    auto ret = new LongJingCell();
    if (ret && ret->init(itemId, itemId1, touchNode)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool LongJingCell::init(int itemId, int itemId1, CCNode* touchNode)
{
    bool ret = true;
    CCBLoadFile("StoreMallCell",this,this);
    this->setContentSize(CCSize(606, 200));
    if(CCCommonUtils::isIosAndroidPad())
    {
        this->setContentSize(CCSize(1450, 300));
    }
    m_touchNode = touchNode;
    m_bLayer = CCLayer::create();
    this->addChild(m_bLayer, 91);
    setData(itemId, itemId1);
    return ret;
}

void LongJingCell::setData(int itemId, int itemId1)
{
    m_itemId = itemId;
    m_itemId1 = itemId1;
    m_desNode->setVisible(false);
    m_priceLabel1->setColor({255,255,0});
    m_priceLabel2->setColor({255,255,0});
//    m_goldSpr1->setColor({255,255,255});
//    m_goldSpr2->setColor({255,255,255});
    //龙晶图标
    auto coinSpr1 = CCLoadSprite::createSprite("shuijing.png");
    coinSpr1->setPosition(m_goldSpr1->getPosition());
//    CCCommonUtils::setSpriteMaxSize(coinSpr1, 40);
    if(CCCommonUtils::isIosAndroidPad())
    {
        CCCommonUtils::setSpriteMaxSize(coinSpr1, 40*2);
    }
    else{
        CCCommonUtils::setSpriteMaxSize(coinSpr1, 40);
    }
    m_goldSpr1->getParent()->addChild(coinSpr1);
    m_goldSpr1->setVisible(false);
    auto coinSpr2 = CCLoadSprite::createSprite("shuijing.png");
    coinSpr2->setPosition(m_goldSpr2->getPosition());
    if(CCCommonUtils::isIosAndroidPad())
    {
        CCCommonUtils::setSpriteMaxSize(coinSpr2, 40*2);
    }
    else{
        CCCommonUtils::setSpriteMaxSize(coinSpr2, 40);
    }
    m_goldSpr2->getParent()->addChild(coinSpr2);
    m_goldSpr2->setVisible(false);
    
    m_buyBtngraySpr2->setVisible(false);
    m_buyBtngraySpr1->setVisible(false);
    ToolInfo& infol = ToolController::getInstance()->getToolInfoById(m_itemId);
    //    m_nameLabel1->setString(_lang(info.name).c_str());
    m_priceLabel1->setString(CC_ITOA_K(infol.LongJing_price));
    m_picNode1->removeAllChildren();
    if(CCCommonUtils::isIosAndroidPad())
    {
        CCCommonUtils::createGoodsIcon(m_itemId, m_picNode1, CCSize(92*1.8, 92*1.8),NULL,m_nameLabel1);
    }
    else{
        CCCommonUtils::createGoodsIcon(m_itemId, m_picNode1, CCSize(92, 92),NULL,m_nameLabel1);
    }
    //特效
    m_bLayer->removeAllChildren();
    m_particleL->removeAllChildren();
    m_frontLNode->removeAllChildren();
    for (int par=0; par<=2; par++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("DragonCristal_%d_%d",infol.color,par)->getCString());
        m_frontLNode->addChild(particle);
    }
    
    float hotPara = infol.hotPara;
    setLeftNewHot(hotPara);
    if(itemId1!=-1){
        this->m_itemNode2->setVisible(true);
        ToolInfo& infor = ToolController::getInstance()->getToolInfoById(m_itemId1);
        m_priceLabel2->setString(CC_ITOA_K(infor.LongJing_price));
        m_picNode2->removeAllChildren();
        if(CCCommonUtils::isIosAndroidPad())
        {
            CCCommonUtils::createGoodsIcon(m_itemId1, m_picNode2, CCSize(92*1.8, 92*1.8),NULL,m_nameLabel2);
        }
        else{
            CCCommonUtils::createGoodsIcon(m_itemId1, m_picNode2, CCSize(92, 92),NULL,m_nameLabel2);
        }
        //特效
        
        m_frontRNode->removeAllChildren();
        for (int par=0; par<=2; par++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("DragonCristal_%d_%d",infor.color,par)->getCString());
            m_frontRNode->addChild(particle);
        }
        
        float hotPara1 = infor.hotPara;
        setRightNewHot(hotPara1);
        
        if (CCCommonUtils::isIosAndroidPad()) {
            m_frontLNode->setScale(2.0);
            m_frontRNode->setScale(2.0);
        }
        
    }else{
        this->m_itemNode2->setVisible(false);
    }
}

void LongJingCell::addParticleToBatch(cocos2d::CCParticleSystemQuad *particle) {
    auto batchCount = m_parVec.size();
    while (batchCount--) {
        auto &batch = m_parVec[batchCount];
        if (batch) {
            auto batchBlend = batch->getBlendFunc();
            auto particleBlend = particle->getBlendFunc();
            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                batch->addChild(particle);
                return;
            }
        }
    }
    auto newBatch = ParticleController::createParticleBatch();
    m_bLayer->addChild(newBatch);
    newBatch->addChild(particle);
    m_parVec.push_back(newBatch);
}

int LongJingCell::getParticalNum(int color){
    int part=1;
    if(1==color){//绿
        part = 3;
    }else if (2==color){//蓝
        part = 1;
    }else if (3==color){//紫
        part = 4;
    }else if (4==color){//橙
        part = 5;
    }else if (5==color){//金
        part = 2;
    }
    return part;
}

void LongJingCell::onEnter() {
    CCNode::onEnter();
    setTouchEnabled(true);
    ToolController::getInstance()->m_mallCurrItemId=0;
}

void LongJingCell::onExit() {
    setTouchEnabled(false);
    CCNode::onExit();
}

void LongJingCell::refreashData(CCObject* obj)
{
    ToolController::getInstance()->m_mallCurrItemId =0;
    this->m_desNode->setVisible(false);
}

SEL_CCControlHandler LongJingCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}


bool LongJingCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_clickInSide = false;
    m_num = 0;
    if (isTouchInside(m_touchNode,pTouch)) {
        m_clickInSide = true;
        if (isTouchInside(this,pTouch)) {
            m_startPoint=pTouch->getStartLocation();
            if(isTouchInside(m_picBG1,pTouch)){
//                this->schedule(schedule_selector(LongJingCell::setLeft),0.2);
            }else if(isTouchInside(m_picBG2,pTouch)){
//                if(m_itemId1==-1){
//                    return false;
//                }
//                this->scheduleOnce(schedule_selector(LongJingCell::setRight),0.2);
//                
            }else if(isTouchInside(m_buyBtnSpr1,pTouch)){
                ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
                if(info.ishot&&info.isShowHot&&info.hotnum<=0){
                    return false;
                }else{
                    m_buyBtnSpr1->setScale(1.2);
                    
                }
                
            }else if(isTouchInside(m_buyBtnSpr2,pTouch)){
                if(m_itemId1==-1){
                    return false;
                }
                ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId1);
                if(info.ishot&&info.isShowHot&&info.hotnum<=0){
                    return false;
                }else{
                    m_buyBtnSpr2->setScale(1.2);
                }
                
            }
            return true;
        }
    }
    return false;
}
void LongJingCell::setLeft(float dt){
    this->unschedule(schedule_selector(LongJingCell::setLeft));
    setDesNode(m_itemId,0);
}
void LongJingCell::setRight(float dt){
    this->unschedule(schedule_selector(LongJingCell::setRight));
    setDesNode(m_itemId1,1);
}
void LongJingCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_buyBtnSpr1->setScale(1.0);
    m_buyBtnSpr2->setScale(1.0);
    m_desNode->setVisible(false);
    this->unschedule(schedule_selector(LongJingCell::setLeft));
    this->unschedule(schedule_selector(LongJingCell::setRight));
    CCPoint endPoint = pTouch->getLocation();
    if(fabs(endPoint.x-m_startPoint.x)>10||fabs(endPoint.y-m_startPoint.y)>10){
        return;
    }
    if(isTouchInside(m_buyBtnSpr1,pTouch)){
        onClickBuyBtn1(NULL,CCControlEvent::TOUCH_DOWN);
    }else if(isTouchInside(m_buyBtnSpr2,pTouch)){
        onClickBuyBtn2(NULL,CCControlEvent::TOUCH_DOWN);
    }else if(isTouchInside(m_picBG1,pTouch)){
//        onClickBuyBtn1(NULL,NULL);
        delegate->showTipNode(m_itemId, pTouch);
    }else if(isTouchInside(m_picBG2,pTouch)){
        if(m_itemId1==-1){
            return;
        }
//        onClickBuyBtn2(NULL,NULL);
        delegate->showTipNode(m_itemId1, pTouch);
    }
    auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_TIP);
    layer->removeAllChildren();
}


void LongJingCell::setDesNode(int itemId,int index){
    m_desNode->setVisible(true);
    ToolController::getInstance()->m_mallCurrItemId = 99;
    if(index==0){//left
        m_desNode->setPositionX(290);
    }else{
        m_desNode->setPositionX(172);
    }
    if(CCCommonUtils::isIosAndroidPad())
    {
        float _p = m_desNode->getPositionX();
        m_desNode->setPositionX(_p*2);
        if (index!=0)
            m_desNode->setPositionX(_p*4-40);
    }
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(itemId);
    if(info.type==5 && !info.getPara().empty()){
        m_desNode->setVisible(false);
        auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_TIP);
        if(layer->getChildrenCount()<=0){
            StoreMallCellTip* tip = StoreMallCellTip::create(info.getPara(),info.getName());
            layer->addChild(tip);
            float x = m_startPoint.x;
            if(index==1){
                x =100;
            }
            tip->setPosition(x, m_startPoint.y);
        }
    }else{
        string name = info.getName()+" Lv:"+ CC_ITOA(info.limitLv);
        m_desName->setString(name.c_str());
        m_desLabel->setString(info.des);//龙晶道具的des已经处理成txt
    }
    CCSize size = m_desLabel->getContentSize();
    size.width *= m_desLabel->getOriginScaleX();
    size.height *= m_desLabel->getOriginScaleY();
    CCScale9Sprite* bgScale9 = dynamic_cast<CCScale9Sprite*>(m_desNode->getChildByTag(10));
    CCSize psize = bgScale9->getContentSize();
    if(size.height>80){
        if(bgScale9 && psize.height<=149){
            psize.height +=  size.height - 80;
            bgScale9->setPreferredSize(psize);
            bgScale9->setPositionY(bgScale9->getPositionY() - (size.height - 80)*0.5);
            m_desNode->setPositionY(m_desNode->getPositionY() +  (size.height - 80)*0.5);
        }
    }else{
        if(bgScale9){
            bgScale9->setPreferredSize(CCSize(psize.width, 149));
            bgScale9->setPositionY(bgScale9->getPositionY() + (psize.height - 149)*0.5);
            m_desNode->setPositionY(m_desNode->getPositionY()- (psize.height - 149)*0.5);
        }
    }
}
void LongJingCell::onClickBuyBtn1(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    if(!m_clickInSide){
        return;
    }
    if(m_isClick)
        return;
    m_isClick = true;
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    int price = 0;
//    if(info.ishot&&info.isShowHot)
//        price = info.hotprice;
//    else
    price = info.LongJing_price;
    int cCount = 0;
    if(info.type==3 && !info.getPara().empty()){
        cCount = atoi(info.getPara().c_str());
    }
    string name = info.getName() + "   "+_lang("102203")+" " + CC_ITOA(info.limitLv);
    LongJingBuyView* dialog = LongJingBuyView::show(CCCommonUtils::getIcon(CC_ITOA(info.itemId)).c_str(), name.c_str(), info.des.c_str(), price,info.color, CCCallFuncO::create(this, callfuncO_selector(LongJingCell::onBuyTool1), NULL),m_picBG1->convertToWorldSpace(CCPointZero),LongJing,cCount);
    if(dialog){
        dialog->setCloseCallback(CCCallFunc::create(this, callfunc_selector(LongJingCell::onCloseConfirm)));
        dialog->setLimitMsg(atoi(info.equipId.c_str()));
    }
}
void LongJingCell::onClickBuyBtn2(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    if(!m_clickInSide){
        return;
    }
    if(m_isClick)
        return;
    m_isClick = true;
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId1);
    int price = 0;
//    if(info.ishot&&info.isShowHot)
//        price = info.hotprice;
//    else
    price = info.LongJing_price;
    int cCount = 0;
    if(info.type==3 && !info.getPara().empty()){
        cCount = atoi(info.getPara().c_str());
    }
    string name = info.getName() + "   "+_lang("102203")+" " + CC_ITOA(info.limitLv);
    LongJingBuyView* dialog = LongJingBuyView::show(CCCommonUtils::getIcon(CC_ITOA(info.itemId)).c_str(), name.c_str(), info.des.c_str(), price,info.color, CCCallFuncO::create(this, callfuncO_selector(LongJingCell::onBuyTool2), NULL),m_picBG2->convertToWorldSpace(CCPointZero),LongJing,cCount);
    if(dialog){
        dialog->setCloseCallback(CCCallFunc::create(this, callfunc_selector(LongJingCell::onCloseConfirm)));
        dialog->setLimitMsg(atoi(info.equipId.c_str()));
    }
}
void LongJingCell::onBuyTool1(CCObject *ccObj)
{
    int buyCount = 1;
//    if(ccObj){
//        CCInteger* cInt = dynamic_cast<CCInteger*>(ccObj);
//        if(cInt){
//            buyCount = cInt->getValue();
//        }
//    }
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    ToolController::getInstance()->buyLongJingEquip(m_itemId, buyCount, CCCallFunc::create(this, callfunc_selector(LongJingCell::onRetBuyTool)),0,false,true,"LongJingStoreView");
}
void LongJingCell::onBuyTool2(CCObject *ccObj)
{
    int buyCount = 1;
//    if(ccObj){
//        CCInteger* cInt = dynamic_cast<CCInteger*>(ccObj);
//        if(cInt){
//            buyCount = cInt->getValue();
//        }
//    }
    ToolInfo& info1 = ToolController::getInstance()->getToolInfoById(m_itemId1);
    ToolController::getInstance()->buyLongJingEquip(m_itemId1,buyCount, CCCallFunc::create(this, callfunc_selector(LongJingCell::onRetBuyTool)),0,false,true,"LongJingStoreView");
}
void LongJingCell::onRetBuyTool()
{
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    if(info.ishot&&info.isShowHot)
        setLeftHot(true,info.hotnum);
    
    if(m_itemId1!=-1){
        ToolInfo& info1 = ToolController::getInstance()->getToolInfoById(m_itemId1);
        if(info1.ishot&&info1.isShowHot)
            setRightHot(true, info1.hotnum);
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUY_CONFIRM_OK);
    m_isClick = false;
}

void LongJingCell::onYes()
{
    //    ToolController::getInstance()->buyTool(m_itemId, 1, CCCallFunc::create(this, callfunc_selector(LongJingCell::onUseTool)));
}
void LongJingCell::onCloseConfirm(){
    m_isClick=false;
}


bool LongJingCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel1", CCLabelIF*, m_nameLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel2", CCLabelIF*, m_nameLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode1", CCNode*, m_picNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode2", CCNode*, m_picNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_priceLabel1", CCLabelIF*, m_priceLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_priceLabel2", CCLabelIF*, m_priceLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode1", CCNode*, m_itemNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode2", CCNode*, m_itemNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBG1", CCSprite*, m_picBG1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBG2", CCSprite*, m_picBG2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode", CCNode*, m_desNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desName", CCLabelIF*, m_desName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtnSpr1", CCScale9Sprite*, m_buyBtnSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtnSpr2", CCScale9Sprite*, m_buyBtnSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_saleoutSpr1", CCScale9Sprite*, m_saleoutSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_saleoutSpr2", CCScale9Sprite*, m_saleoutSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtngraySpr1", CCScale9Sprite*, m_buyBtngraySpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtngraySpr2", CCScale9Sprite*, m_buyBtngraySpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldSpr1", CCSprite*, m_goldSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldSpr2", CCSprite*, m_goldSpr2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemHotNode1", CCNode*, m_itemHotNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotSpr1", CCSprite*, m_hotSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotgraySpr1", CCSprite*, m_hotgraySpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotNumSpr1", CCSprite*, m_hotNumSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotNumgraySpr1", CCSprite*, m_hotNumgraySpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotdesText1", CCLabelIF*, m_hotdesText1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotNum1Text", CCLabelIF*, m_hotNum1Text);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotLineSpr1", CCScale9Sprite*, m_hotLineSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotpriceLabel1", CCLabelIF*, m_hotpriceLabel1);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemHotNode2", CCNode*, m_itemHotNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotSpr2", CCSprite*, m_hotSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotgraySpr2", CCSprite*, m_hotgraySpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotNumSpr2", CCSprite*, m_hotNumSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotNumgraySpr2", CCSprite*, m_hotNumgraySpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotdesText2", CCLabelIF*, m_hotdesText2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotNum2Text", CCLabelIF*, m_hotNum2Text);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotLineSpr2", CCScale9Sprite*, m_hotLineSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotpriceLabel2", CCLabelIF*, m_hotpriceLabel2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleL", CCNode*, m_particleL);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleR", CCNode*, m_particleR);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_frontLNode", CCNode*, m_frontLNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_frontRNode", CCNode*, m_frontRNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newHotNodeL", CCNode*, m_newHotNodeL);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newHotSprPL", CCSprite*, m_newHotSprPL);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newHotTxtL", CCLabelIF*, m_newHotTxtL);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newHotSprRL", CCSprite*, m_newHotSprRL);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newHotNodeR", CCNode*, m_newHotNodeR);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newHotSprPR", CCSprite*, m_newHotSprPR);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newHotTxtR", CCLabelIF*, m_newHotTxtR);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newHotSprRR", CCSprite*, m_newHotSprRR);
    return false;
}
void LongJingCell::setLeftHot(bool ishot,int num){
    m_itemHotNode1->setVisible(ishot);
    m_hotpriceLabel1->setVisible(ishot);
    m_hotNum1Text->setString(CC_CMDITOA(num));
    m_hotdesText1->setString(_lang("101224"));
    
    m_hotLineSpr1->setVisible(ishot);
    m_priceLabel1->setPositionY(11);
    if(ishot){
        if(num>0){
            m_buyBtngraySpr1->setVisible(false);
            m_saleoutSpr1->setVisible(false);
            m_hotgraySpr1->setVisible(false);
            m_hotSpr1->setVisible(true);
            m_hotNumSpr1->setVisible(true);
            m_hotNumgraySpr1->setVisible(false);
            m_nameLabel1->setColor({194,158,116});
            m_priceLabel1->setColor({255,255,0});
            m_hotpriceLabel1->setColor({221,172,33});
        }else{
            m_buyBtngraySpr1->setVisible(true);
            m_saleoutSpr1->setVisible(true);
            m_hotNumSpr1->setVisible(false);
            m_hotNumgraySpr1->setVisible(true);
            m_hotgraySpr1->setVisible(true);
            m_hotSpr1->setVisible(false);
            m_nameLabel1->setColor({156,156,156});
            m_priceLabel1->setColor({156,156,156});
            m_hotpriceLabel1->setColor({156,156,156});
            auto pic = dynamic_cast<CCSprite*>(m_picNode1->getChildByTag(GOODS_ICON_TAG));
            if(pic){
                pic->setColor({90,85,81});
            }
            auto pic1 = dynamic_cast<CCSprite*>(m_picNode1->getChildByTag(GOODS_BG_TAG));
            if(pic1){
                pic1->setColor({90,85,81});
            }
            m_goldSpr1->setColor({90,85,81});
            
        }
    }else{
        m_priceLabel1->setPositionY(0);
    }
}
void LongJingCell::setRightHot(bool ishot,int num){
    m_itemHotNode2->setVisible(ishot);
    m_hotpriceLabel2->setVisible(ishot);
    m_hotNum2Text->setString(CC_CMDITOA(num));
    m_hotdesText2->setString(_lang("101224"));
    m_hotLineSpr2->setVisible(ishot);
    m_priceLabel2->setPositionY(11);
    if(ishot){
        if(num>0){
            m_saleoutSpr2->setVisible(false);
            m_buyBtngraySpr2->setVisible(false);
            m_hotNumSpr2->setVisible(true);
            m_hotNumgraySpr2->setVisible(false);
            m_hotgraySpr2->setVisible(false);
            m_hotSpr2->setVisible(true);
            m_nameLabel2->setColor({194,158,116});
            m_priceLabel2->setColor({255,255,0});
            m_hotpriceLabel2->setColor({221,172,33});
        }else{
            m_saleoutSpr2->setVisible(true);
            m_buyBtngraySpr2->setVisible(true);
            m_hotNumSpr2->setVisible(false);
            m_hotNumgraySpr2->setVisible(true);
            m_hotgraySpr2->setVisible(true);
            m_hotSpr2->setVisible(false);
            m_nameLabel2->setColor({156,156,156});
            m_priceLabel2->setColor({156,156,156});
            m_hotpriceLabel2->setColor({156,156,156});
            auto pic = dynamic_cast<CCSprite*>(m_picNode2->getChildByTag(GOODS_ICON_TAG));
            if(pic){
                pic->setColor({90,85,81});
            }
            auto pic1 = dynamic_cast<CCSprite*>(m_picNode2->getChildByTag(GOODS_BG_TAG));
            if(pic1){
                pic1->setColor({90,85,81});
            }
            m_goldSpr2->setColor({90,85,81});
        }
    }else{
        m_priceLabel2->setPositionY(0);
    }
}

void LongJingCell::setLeftNewHot(float flag){
    m_itemHotNode1->setVisible(false);
    m_hotLineSpr1->setVisible(false);
    m_hotpriceLabel1->setVisible(false);
    if (flag==99.0 || (flag>=0.1 && flag<=9.9)) {
        m_newHotNodeL->setVisible(true);
        if (flag==99.0) {
            m_newHotSprPL->setVisible(false);
            m_newHotTxtL->setVisible(false);
            m_newHotSprRL->setVisible(true);
        }else if (flag>=0.1 && flag<=9.9){
            m_newHotSprPL->setVisible(true);
            m_newHotTxtL->setVisible(true);
            int count = 100-flag*10;
            string value = "-";
            value+=CC_ITOA(count);
            value+="%";
            m_newHotTxtL->setString(value);
            m_newHotSprRL->setVisible(false);
        }
    }else{
        m_newHotNodeL->setVisible(false);
    }
    m_priceLabel1->setPositionY(0);
}
void LongJingCell::setRightNewHot(float flag){
    m_itemHotNode2->setVisible(false);
    m_hotpriceLabel2->setVisible(false);
    m_hotLineSpr2->setVisible(false);
    if (flag==99.0 || (flag>=0.1 && flag<=9.9)) {
        m_newHotNodeR->setVisible(true);
        if (flag==99.0) {
            m_newHotSprPR->setVisible(false);
            m_newHotTxtR->setVisible(false);
            m_newHotSprRR->setVisible(true);
        }else if (flag>=0.1 && flag<=9.9){
            m_newHotSprPR->setVisible(true);
            m_newHotTxtR->setVisible(true);
            int count = 100-flag*10;
            string value = "-";
            value+=CC_ITOA(count);
            value+="%";
            m_newHotTxtR->setString(value);
            m_newHotSprRR->setVisible(false);
        }
    }else{
        m_newHotNodeR->setVisible(false);
    }
    m_priceLabel2->setPositionY(0);
}


// LongJingBagView
LongJingBagView* LongJingBagView::create(CCSize size){
    LongJingBagView* ret = new LongJingBagView();
    if(ret && ret->init(size)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool LongJingBagView::init(CCSize size)
{
    m_listNode = CCNode::create();
    m_listNode->setContentSize(size);
    m_listNode->setAnchorPoint(Vec2(0, 0));
    this->addChild(m_listNode);
    
    m_data = CCArray::create();
    m_tabView = CCTableView::create(this, m_listNode->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_tabView->setDelegate(this);
    m_listNode->addChild(m_tabView);
    
    generateBagData();
    m_tabView->reloadData();
    
    return true;
}

void LongJingBagView::onEnter()
{
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LongJingBagView::refreashData), EQUIP_CLOSE_VIEW, NULL);
}
void LongJingBagView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, EQUIP_CLOSE_VIEW);
    CCNode::onExit();
}

void LongJingBagView::refreashData(CCObject* obj)
{
    generateBagData();
    m_tabView->reloadData();
}

CCSize LongJingBagView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(advertiseCellW_HD, 310);
    }
    else {
        return CCSize(advertiseCellW, 180);
    }
}

CCSize LongJingBagView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(advertiseCellW_HD, 310);
    }
    else {
        return CCSize(advertiseCellW, 180);
    }
}

CCTableViewCell* LongJingBagView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    
    string itemId = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();
    EquipmentBagCell* cell = (EquipmentBagCell*)table->dequeueCell();
    if(cell){
        cell->setData(itemId, 0);
    }else{
        cell = EquipmentBagCell::create(itemId, 0, m_listNode);
    }
    return cell;
}

ssize_t LongJingBagView::numberOfCellsInTableView(CCTableView *table)
{
    int num = ceil(m_data->count() / 1.0f);
    return num;
}

bool sortLongjingList1(int itemId1, int itemId2)
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

bool sortLongjingList(string str1, string str2)
{
    int itemId1 = EquipmentController::getInstance()->m_MyEquipmentMap[str1];
    int itemId2 = EquipmentController::getInstance()->m_MyEquipmentMap[str2];
    return sortLongjingList1(itemId1,itemId2);
}

void LongJingBagView::generateBagData()
{
    m_data->removeAllObjects();
    vector<string> m_curList;
    if (true) {
        map<string, int>::iterator it = EquipmentController::getInstance()->m_MyEquipmentMap.begin();
        for (; it!=EquipmentController::getInstance()->m_MyEquipmentMap.end(); it++)
        {
            auto& tmpInfo = EquipmentController::getInstance()->EquipmentInfoMap[it->second];
            if(tmpInfo.site == 6)
            {
                m_curList.push_back(it->first);
            }
        }
        sort(m_curList.begin(), m_curList.end(),sortLongjingList);
        
        string OnEquipKey = EquipmentController::getInstance()->FindOnEquipBySite(6);
        if (OnEquipKey!="") {
            m_curList.insert(m_curList.begin(), OnEquipKey);
        }
    }
    
    string itemIds = "";
    int count = m_curList.size();
    if (CCCommonUtils::isIosAndroidPad()) {
        for (int i=0; i<count; i++)
        {//5个道具 分为一组
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
                m_data->addObject(CCString::create("-1:-1:-1:-1:-1"));
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