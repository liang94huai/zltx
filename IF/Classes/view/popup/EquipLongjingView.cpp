//
//  EquipLongjingView.cpp
//  IF
//
//  Created by 付彬 on 15/12/15.
//
//

#include "EquipLongjingView.hpp"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "ToolController.h"
#include "EquipmentController.h"
#include "EquipmentInfo.h"
#include "QueueController.h"
#include "SoundController.h"
#include "EquipInfoView.hpp"
#include "LongJingBuyView.h"
#include "YesNoDialog.h"

#define BOOK_NUM_MAX 5

static int dataSize = 0;
static string CurEquipUUid = "";
static string m_guideKey = "";

static int bgColorH[5] = {108, 190, 285, 24, 52};
static float bgColorS[5] = {0.7, 0.7, 1.0, 1.0, 1.0};
static float bgColorB[5] = {1.0, 1.0, 1.0, 1.0, 1.0};
//static float bgColorS[5] = {0.7, 0.7, 1.0, 0.76, 0.93};
//static float bgColorB[5] = {1.0, 1.0, 0.8, 0.88, 0.95};

EquipLongjingView* EquipLongjingView::create(){
    EquipLongjingView* ret = new EquipLongjingView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipLongjingView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    CCLoadSprite::doResourceByCommonIndex(11, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
    });
    
    setIsHDPanel(true);
    auto bg = CCBLoadFile("EquipLongjingView",this,this);
    this->setContentSize(bg->getContentSize());
    changeBGHeight(m_buildBG);
    int extH = getExtendHeight();
    m_extH = extH;
    
    m_infoNode->setContentSize(CCSize(m_infoNode->getContentSize().width, m_infoNode->getContentSize().height+extH));
    m_infoNode->setPositionY(m_infoNode->getPositionY()-extH);
    m_parNode->setPositionY(m_parNode->getPositionY()+extH/2);
    
    m_scrollView = CCScrollView::create(m_infoNode->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_infoNode->addChild(m_scrollView);
    
    m_bottomNode->setPositionY(m_bottomNode->getPositionY()-extH);
    m_btn->setSwallowsTouches(false);
    
    CCCommonUtils::setButtonTitle(m_color1Btn, _lang("111734").c_str());
    CCCommonUtils::setButtonTitle(m_color2Btn, _lang("111735").c_str());
    CCCommonUtils::setButtonTitle(m_color3Btn, _lang("111736").c_str());
    CCCommonUtils::setButtonTitle(m_color4Btn, _lang("111737").c_str());
    CCCommonUtils::setButtonTitle(m_color5Btn, _lang("111738").c_str());
    
//    EquipmentController::getInstance()->getScrollInfo();
    
    CCCommonUtils::setButtonTitle(m_btn, _lang("111732").c_str());
    m_btn->setTouchPriority(Touch_Popup_Item);
    
    m_curIdx = 4;
    m_isInitEnd = false;
    m_waitInterface = NULL;
    return true;
}

void EquipLongjingView::onEnter(){
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipLongjingView::onRetSellEquip), EQUIP_BAG_SHOW_DES, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipLongjingView::onRetBuyEquip), MSG_BUY_CONFIRM_OK, NULL);
    
    m_isInitEnd = false;
    
//    UIComponent::getInstance()->showEquipOrBagBtn(1);
    
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    
    setTitleName(_lang("111652"));
    onRefreshData(1);
}

void EquipLongjingView::onExit()
{
    UIComponent::getInstance()->showEquipOrBagBtn(0);
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, EQUIP_BAG_SHOW_DES);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BUY_CONFIRM_OK);
    CCNode::onExit();
}

bool EquipLongjingView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void EquipLongjingView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    Vector<CCNode*>& arr = m_scrollView->getContainer()->getChildren();
    for (int i=0; i<arr.size(); i++) {
        EquipLongjingCell* cell = dynamic_cast<EquipLongjingCell*>(arr.at(i));
        if (cell) {
            bool st = cell->cellTouchEnded(pTouch);
            if (st) {
                break;
            }
        }
    }
}

CCNode* EquipLongjingView::getGuideNode(string _key)
{
    if (_key == "Longjing_Reset") {
        m_guideKey = _key;
        return m_btn;
    }
    return NULL;
}

bool sortLongjing(int itemId1, int itemId2)
{
    ToolInfo& info1 = ToolController::getInstance()->getToolInfoById(itemId1);
    ToolInfo& info2 = ToolController::getInstance()->getToolInfoById(itemId2);
    if (info1.limitLv<info2.limitLv) {
        return true;
    }else if (info1.limitLv>info2.limitLv) {
        return false;
    }else {
        return false;
    }
}

void EquipLongjingView::onRefreshData(int type)
{
    m_page = type;
    vector<int> m_dataList = ToolController::getInstance()->m_typeTools[9];
    m_curList.clear();
    CurEquipUUid = "";
    m_sumLongPrice = 0;
    
    for (int i=0; i<m_dataList.size(); i++) {
        int tmpToolId = m_dataList[i];
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(tmpToolId);
        if (info.LongJing_price>0 && info.color == type){// && info.limitLv<=m_cityLv
            m_curList.push_back(tmpToolId);
            if (CurEquipUUid == "") {
                CurEquipUUid = EquipmentController::getInstance()->IsHaveEquipById( atoi(info.equipId.c_str()) );
                
                map<string, int>::iterator it = EquipmentController::getInstance()->m_MyEquipmentMap.begin();
                for (; it!=EquipmentController::getInstance()->m_MyEquipmentMap.end(); it++)
                {
                    if (info.equipId == CC_ITOA(it->second)) {
                        m_sumLongPrice += info.LongJing_price;
                    }
                }
                
                string tmpUUid = EquipmentController::getInstance()->FindOnEquipBySite(6);
                if (tmpUUid!="" && EquipmentController::getInstance()->m_MyOnEquip[tmpUUid] == atoi(info.equipId.c_str())) {
                    m_sumLongPrice += info.LongJing_price;
                }
            }
        }
    }
    sort(m_curList.begin(), m_curList.end(), sortLongjing);
    
    if (m_sumLongPrice>0) {
        m_btn->setEnabled(true);
    }else {
        m_btn->setEnabled(false);
    }
    
    onShowData();
}

void EquipLongjingView::onShowData()
{
    m_bgNode->removeAllChildren();
    
    auto tbg = CCLoadSprite::loadResource("UI_UseSkill_picture_blackwhite.png");
    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
    int maxHight = CCDirector::sharedDirector()->getWinSize().height;
    int curHight = -500;
    while (curHight<maxHight) {
        auto bg = CCCommonUtils::addFilterSprite("UI_UseSkill_picture_blackwhite.png", CCCommonUtils::covertHSBToRGB(bgColorH[m_page-1], bgColorS[m_page-1], bgColorB[m_page-1]), 0, 1.04, 4.4);
        bg->setAnchorPoint(ccp(0, 1));
        bg->setPosition(ccp(0, curHight));
        curHight += bg->getContentSize().height;
        tBatchNode->addChild(bg);
    }
    m_bgNode->addChild(tBatchNode);
    
    m_parNode->removeAllChildrenWithCleanup(true);
    for (int i=1; i<3; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("SkillBG_G_%d",i)->getCString());
        if (m_page == 1) {
            particle->setStartColor(ccc4f(0, 1, 0, 1));
        }else if (m_page == 2) {
            particle->setStartColor(ccc4f(0, 0.5, 1, 1));
        }else if (m_page == 3) {
            particle->setStartColor(ccc4f(1, 0, 1, 1));
        }else if (m_page == 4) {
            particle->setStartColor(ccc4f(1, 0.4, 0, 1));
        }else if (m_page == 5) {
            particle->setStartColor(ccc4f(1, 1, 0, 1));
        }
        particle->setPositionY(i==1?-m_extH/2:m_extH/2);
        m_parNode->addChild(particle);
    }
    
    int cellW = 204;
    int offX = 116;
    int offY = 120;
    dataSize = m_curList.size();
    
    int sumH = cellW * (2 + (dataSize-2)/2 );
    if (dataSize%2==0) {
        sumH -= cellW;
    }
    sumH = offY + sumH + offY;
    
    int curY = sumH - offY;
    m_scrollView->getContainer()->removeAllChildren();
    for (int i=0; i<m_curList.size(); i++)
    {
        auto cell = EquipLongjingCell::create(m_curList[i], i);
        if (i==0) {
            cell->setPosition(ccp(offX+cellW, curY));
            curY -= cellW;
        }
        else
        {
            int curX = offX;
            if (i%4==0 || i%4==3) {
                curX += cellW*2;
            }
            if (i%2==0) {
                curY -= cellW;
            }
            cell->setPosition(ccp(curX, curY));
        }
        m_scrollView->addChild(cell);
    }
    
    m_scrollView->setContentSize(CCSize(m_infoNode->getContentSize().width,sumH));
    m_scrollView->setContentOffset(ccp(0, m_infoNode->getContentSize().height - sumH));
    
    m_color1Btn->setEnabled(true);
    m_color2Btn->setEnabled(true);
    m_color3Btn->setEnabled(true);
    m_color4Btn->setEnabled(true);
    m_color5Btn->setEnabled(true);
    if (m_page == 1) {
        m_color1Btn->setEnabled(false);
    }else if (m_page == 2) {
        m_color2Btn->setEnabled(false);
    }else if (m_page == 3) {
        m_color3Btn->setEnabled(false);
    }else if (m_page == 4) {
        m_color4Btn->setEnabled(false);
    }else if (m_page == 5) {
        m_color5Btn->setEnabled(false);
    }
}

SEL_CCControlHandler EquipLongjingView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickSite1Btn", EquipLongjingView::onClickSite1Btn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickSite2Btn", EquipLongjingView::onClickSite2Btn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickSite3Btn", EquipLongjingView::onClickSite3Btn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickSite4Btn", EquipLongjingView::onClickSite4Btn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickSite5Btn", EquipLongjingView::onClickSite5Btn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOkBtnClick", EquipLongjingView::onOkBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickHelpBtn", EquipLongjingView::onClickHelpBtn);
    return NULL;
}

bool EquipLongjingView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode", CCNode*, this->m_infoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn", CCControlButton*, this->m_btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_color1Btn", CCControlButton*, this->m_color1Btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_color2Btn", CCControlButton*, this->m_color2Btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_color3Btn", CCControlButton*, this->m_color3Btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_color4Btn", CCControlButton*, this->m_color4Btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_color5Btn", CCControlButton*, this->m_color5Btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_color5Btn", CCControlButton*, this->m_color5Btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpBtn", CCControlButton*, this->m_helpBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_parNode", CCNode*, this->m_parNode);
    return false;
}

void EquipLongjingView::onClickSite1Btn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    onRefreshData(1);
}
void EquipLongjingView::onClickSite2Btn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    onRefreshData(2);
}
void EquipLongjingView::onClickSite3Btn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    onRefreshData(3);
}
void EquipLongjingView::onClickSite4Btn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    onRefreshData(4);
}
void EquipLongjingView::onClickSite5Btn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    onRefreshData(5);
}
void EquipLongjingView::onClickHelpBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    CCCommonUtils::showHelpShiftSingleFAQ("45214");
}

void EquipLongjingView::onOkBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_guideKey != "") {
        m_guideKey = "";
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::create("Longjing_Reset"));
        return;
    }
    string uuid = EquipmentController::getInstance()->FindOnEquipBySite(6);
    if (uuid != "") {
        int tmpItemId = EquipmentController::getInstance()->m_MyOnEquip[uuid];
        auto& tmpInfo = EquipmentController::getInstance()->EquipmentInfoMap[tmpItemId];
        if (tmpInfo.color == m_page) {
            string colorName = CCCommonUtils::getColorName(tmpInfo.color);
            CCCommonUtils::flyHint("", "", _lang_2("111741", colorName.c_str(), _lang(tmpInfo.name).c_str()) );
            return;
        }
    }
    
    m_sumLongPrice = 0;
    for (int i=0; i<m_curList.size(); i++) {
        int tmpToolId = m_curList[i];
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(tmpToolId);
        map<string, int>::iterator it = EquipmentController::getInstance()->m_MyEquipmentMap.begin();
        for (; it!=EquipmentController::getInstance()->m_MyEquipmentMap.end(); it++)
        {
            if (info.equipId == CC_ITOA(it->second)) {
                m_sumLongPrice += info.LongJing_price;
            }
        }
        
        string tmpUUid = EquipmentController::getInstance()->FindOnEquipBySite(6);
        if (tmpUUid!="" && EquipmentController::getInstance()->m_MyOnEquip[tmpUUid] == atoi(info.equipId.c_str())) {
            m_sumLongPrice += info.LongJing_price;
        }
    }
    
    YesNoDialog::show(_lang_1("111733", CC_ITOA(m_sumLongPrice)), CCCallFunc::create(this, callfunc_selector(EquipLongjingView::onYesDestroyLongEquip)));
}

void EquipLongjingView::onYesDestroyLongEquip(){
    if(!CurEquipUUid.empty()){
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_btn);
        EquipmentController::getInstance()->startSellLongEquipment(CurEquipUUid);
    }
}

void EquipLongjingView::onRetSellEquip(CCObject* obj)
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    onRefreshData(m_page);
}

void EquipLongjingView::onRetBuyEquip(CCObject* obj)
{
    m_btn->setEnabled(true);
}

//class cell
EquipLongjingCell* EquipLongjingCell::create(int itemIds, int idx)
{
    auto ret = new EquipLongjingCell();
    if (ret && ret->init(itemIds, idx)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipLongjingCell::init(int itemIds, int idx)
{
    bool ret = true;
    CCBLoadFile("EquipLongjingCell",this,this);
    setData(itemIds, idx);
    return ret;
}

void EquipLongjingCell::setData(int itemId, int idx)
{
    m_itemId = itemId;
    m_idx = idx;
    m_isClick = false;
    
    if (idx%2==1) {
        m_subLineNode->setVisible(false);
        m_openSubLNode->setVisible(false);
        m_lineNode->setRotation(90);
        m_openLNode->setRotation(90);
    }else {
        m_subLineNode->setVisible(true);
        m_openSubLNode->setVisible(true);
        if (idx == 0) {
            m_lineNode->setRotation(90);
            m_openLNode->setRotation(90);
            m_subLineNode->setRotation(90);
            m_openSubLNode->setRotation(90);
        }
        else if (idx%4==0) {
            m_lineNode->setRotation(180);
            m_openLNode->setRotation(180);
        }
    }
    
    ToolInfo& toolInfo = ToolController::getInstance()->getToolInfoById(m_itemId);
    int equipId = atoi(toolInfo.equipId.c_str());
    m_equipId = equipId;
    
    makeDataState();
    
    if (dataSize-1 == m_idx) {
        m_lineNode->setVisible(false);
        m_openLNode->setVisible(false);
    }
}

void EquipLongjingCell::makeDataState()
{
    m_equipUUid = "";
    m_state = 0;
    m_openLNode->setVisible(true);
    m_picNode->removeAllChildren();
    m_parNode->removeAllChildren();
    m_nameLabel->setString("");
    m_newTipSpr->setVisible(false);
    
    auto& info = EquipmentController::getInstance()->EquipmentInfoMap[m_equipId];
    m_lockEquipId = info.prv_level;
    m_nameLabel->setString(_lang(info.name));
    string picStr = CCCommonUtils::getIcon(CC_ITOA(m_itemId));
    auto icon = CCLoadSprite::createSprite(picStr.c_str(), true,CCLoadSpriteType_EQUIP);
    
    string bgName = "";
    bgName = bgName + "color_c_"+CC_ITOA(info.color)+".png";
    auto iconBg = CCLoadSprite::createSprite(bgName.c_str());
    
    m_equipUUid = EquipmentController::getInstance()->IsHaveEquipById(m_equipId);
    if ( m_equipUUid!="" ) //有该龙晶
    {
//        if (info.color==1) {
//            iconBg->setColor(ccc3(0, 205, 17));
//        }else if (info.color==2) {
//            iconBg->setColor(ccc3(0, 125, 203));
//        }else if (info.color==3) {
//            iconBg->setColor(ccc3(204, 0, 191));
//        }else if (info.color==4) {
//            iconBg->setColor(ccc3(224, 120, 53));
//        }else if (info.color==5) {
//            iconBg->setColor(ccc3(241, 204, 17));
//        }
//        iconBg->setOpacity(100);
        
        if (dataSize-1 == m_idx) {
            m_openLNode->setVisible(false);
        }
        CCCommonUtils::setSpriteMaxSize(icon, 90, true);
        m_picNode->addChild(iconBg);
        m_picNode->addChild(icon);
//        for (int par=0; par<=2; par++) {
//            auto particle = ParticleController::createParticle(CCString::createWithFormat("DragonCristal_%d_%d",info.color,par)->getCString());
//            m_picNode->addChild(particle);
//        }
        
        int curOnEquip = 0;
        string tmpEquip = EquipmentController::getInstance()->FindOnEquipBySite(6);
        if (tmpEquip!="") {
            curOnEquip = EquipmentController::getInstance()->m_MyOnEquip[tmpEquip];
        }
        if (m_equipId == curOnEquip) {
            m_putMarkNode->setVisible(true);
            m_state = 0;
        }else {
            m_putMarkNode->setVisible(false);
            m_state = 1;
        }
        
        int isNew = CCUserDefault::sharedUserDefault()->getIntegerForKey(CC_ITOA(m_equipId), 0);
        if (isNew==1) {
            m_newTipSpr->setVisible(true);
        }
    }
    else
    {
        m_openLNode->setVisible(false);
        m_putMarkNode->setVisible(false);
        CCCommonUtils::setSpriteGray(icon, true);
        CCCommonUtils::setSpriteGray(iconBg, true);
        CCCommonUtils::setSpriteMaxSize(icon, 90, true);
        m_picNode->addChild(iconBg);
        m_picNode->addChild(icon);
        string tmpEquipUUid = EquipmentController::getInstance()->IsHaveEquipById(m_lockEquipId);
        if ( m_lockEquipId==0  || tmpEquipUUid!="") { //有前置龙晶，可以购买该龙晶
            m_state = 2;
            auto particle = ParticleController::createParticle("researching");
            if (info.color == 1) {
                particle->setStartColor(ccc4f(0, 1, 0, 1));
            }else if (info.color == 2) {
                particle->setStartColor(ccc4f(0, 0.5, 1, 1));
            }else if (info.color == 3) {
                particle->setStartColor(ccc4f(1, 0, 1, 1));
            }else if (info.color == 4) {
                particle->setStartColor(ccc4f(1, 0.4, 0, 1));
            }else if (info.color == 5) {
                particle->setStartColor(ccc4f(1, 1, 0, 1));
            }
            particle->setScale(0.75);
            m_parNode->addChild(particle);
        }
        else // 不能购买
        {
            m_state = 3;
        }
    }
    
}

void EquipLongjingCell::setPutOn(CCObject* obj)
{
    CCString* equipItemId = dynamic_cast<CCString*>(obj);
    if (equipItemId) {
        string uuid = equipItemId->getCString();
        if (uuid == m_equipUUid) {
            makeDataState();
        }
        else {
            if (m_putMarkNode->isVisible()) {
                makeDataState();
            }
        }
    }
}

void EquipLongjingCell::onEnter() {
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipLongjingCell::onRetBuyTool), MSG_BUY_LONGJING, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipLongjingCell::setPutOn), EQUIP_PUTON_MSG, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipLongjingCell::setPutOn), EQUIP_TAKEOFF_MSG, NULL);
}

void EquipLongjingCell::onExit() {
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BUY_LONGJING);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, EQUIP_PUTON_MSG);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, EQUIP_TAKEOFF_MSG);
    CCNode::onExit();
}

bool EquipLongjingCell::cellTouchEnded(CCTouch* pTouch)
{
    if (isTouchInside(m_touchNode, pTouch)) {
        CCLOG("state %d",m_state);
        
        if (m_state==0 || m_state==1) {
            if (m_newTipSpr->isVisible()) {
                m_newTipSpr->setVisible(false);
                CCUserDefault::sharedUserDefault()->setIntegerForKey(CC_ITOA(m_equipId), 0);
                CCUserDefault::sharedUserDefault()->flush();
            }
            PopupViewController::getInstance()->addPopupView(EquipInfoView::create(m_equipUUid));
        }else {
            if(m_isClick) {
                return true;
            }
            m_isClick = true;
            
            ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
            int price = info.LongJing_price;
            int cCount = 1;
            string name = info.getName() + "   "+_lang("102203")+" " + CC_ITOA(info.limitLv);
            LongJingBuyView* dialog = LongJingBuyView::show(CCCommonUtils::getIcon(CC_ITOA(info.itemId)).c_str(), name.c_str(), info.des.c_str(), price,info.color, CCCallFuncO::create(this, callfuncO_selector(EquipLongjingCell::onBuyTool), NULL),m_picNode->convertToWorldSpace(CCPointZero),LongJing,cCount);
            if(dialog){
                dialog->setCloseCallback(CCCallFunc::create(this, callfunc_selector(EquipLongjingCell::onCloseConfirm)));
                dialog->setLimitMsg(atoi(info.equipId.c_str()));
            }
        }
        return true;
    }
    return false;
}

void EquipLongjingCell::onBuyTool(CCObject *ccObj)
{
    int buyCount = 1;
    ToolInfo& info1 = ToolController::getInstance()->getToolInfoById(m_itemId);
    ToolController::getInstance()->buyLongJingEquip(m_itemId,buyCount, NULL,0,false,true,"");
}

void EquipLongjingCell::onCloseConfirm(){
    m_isClick=false;
}

void EquipLongjingCell::onRetBuyTool(CCObject* obj)
{
    CCString* equipItemId = dynamic_cast<CCString*>(obj);
    if (equipItemId) {
        int retEquipId = equipItemId->intValue();
        if (retEquipId == m_equipId || retEquipId == m_lockEquipId) {
            m_isClick = false;
            makeDataState();
            if (retEquipId == m_equipId) {
                CurEquipUUid = EquipmentController::getInstance()->IsHaveEquipById(m_equipId);
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUY_CONFIRM_OK);
                
                CCUserDefault::sharedUserDefault()->setIntegerForKey(CC_ITOA(m_equipId), 1);
                CCUserDefault::sharedUserDefault()->flush();
                m_newTipSpr->setVisible(true);
            }
        }
    }
}

SEL_CCControlHandler EquipLongjingCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onForgeBtnClick", EquipLongjingCell::onForgeBtnClick);
    return NULL;
}

bool EquipLongjingCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode", CCNode*, m_particleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lineNode", CCNode*, m_lineNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subLineNode", CCNode*, m_subLineNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_openLNode", CCNode*, m_openLNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_openSubLNode", CCNode*, m_openSubLNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_putMarkNode", CCNode*, m_putMarkNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_parNode", CCNode*, m_parNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newTipSpr", CCSprite*, m_newTipSpr);
    return false;
}