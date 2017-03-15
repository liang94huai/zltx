//
//  ActivityExcView.cpp
//  IF
//
//  Created by xxrdsg on 15-6-3.
//
//

#include "ActivityExcView.h"
#include "PopupViewController.h"
#include "ToolController.h"
#include "RewardController.h"
#include "CCMathUtils.h"
#include "YesNoDialog.h"
#include "StoreView.h"
#include "LuaController.h"
#include "TipsView.h"
#include "UIComponent.h"
#include "DynamicResourceController.h"

int cellWith = 67;
int cellHeight = 125;

ActivityExcNewView* ActivityExcNewView::create(ActivityEventObj *obj)
{
    auto ret = new ActivityExcNewView(obj);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ActivityExcNewView::init()
{
    if (!PopupBaseView::init()) return false;
    setIsHDPanel(true);
    this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    if (DynamicResourceController::getInstance()->checkActivityResource())
    {
        DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_ACTIVITY_RES, false);
    }
    CCLoadSprite::doResourceByCommonIndex(500, true);
    CCLoadSprite::doResourceByCommonIndex(502, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(500, false);
        CCLoadSprite::doResourceByCommonIndex(502, false);
        DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_ACTIVITY_RES, true);
    });
    
    CCBLoadFile("ActivityExcNewView", this, this);
    if (DynamicResourceController::getInstance()->checkActivityResource()) {
        string ccbiName = "ActivityExcNewViewBg_";
        ccbiName += m_obj->id;
        ccbiName += ".ccbi";
        string path = DynamicResourceController::getInstance()->getDownloadPath();
        string fullPathName = path + "activity_res_ccbi/" + ccbiName;
        if (FileUtils::getInstance()->isFileExist(fullPathName)) {
            m_bgNode1->removeAllChildrenWithCleanup(true);
            CCBLoadFile(fullPathName.c_str(), m_bgNode1, m_bgNode1);
        }
    }
    auto tbg = CCLoadSprite::loadResource("activity1_7.png");
    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
    int offx = -270;
    CCSprite* bg = nullptr;
    while (offx < 270 - 56) {
        bg = CCLoadSprite::createSprite("activity1_7.png");
        bg->setAnchorPoint(ccp(0, 0.5));
        bg->setPosition(ccp(offx, 240));
        tBatchNode->addChild(bg);
        bg = CCLoadSprite::createSprite("activity1_7.png");
        bg->setFlipY(true);
        bg->setAnchorPoint(ccp(0, 0.5));
        bg->setPosition(ccp(offx, -240));
        tBatchNode->addChild(bg);
        offx += 56;
        if (offx >= 270 - 56) {
            bg = CCLoadSprite::createSprite("activity1_7.png");
            bg->setAnchorPoint(ccp(1, 0.5));
            bg->setPosition(ccp(270, 240));
            tBatchNode->addChild(bg);
            bg = CCLoadSprite::createSprite("activity1_7.png");
            bg->setFlipY(true);
            bg->setAnchorPoint(ccp(1, 0.5));
            bg->setPosition(ccp(270, -240));
            tBatchNode->addChild(bg);
        }
    }
    int offy = -240;
    while (offy < 240 - 55) {
        bg = CCLoadSprite::createSprite("activity1_6.png");
        bg->setAnchorPoint(ccp(0.5, 0));
        bg->setPosition(ccp(-270, offy));
        tBatchNode->addChild(bg);
        bg = CCLoadSprite::createSprite("activity1_6.png");
        bg->setFlipX(true);
        bg->setAnchorPoint(ccp(0.5, 0));
        bg->setPosition(ccp(270, offy));
        tBatchNode->addChild(bg);
        offy += 55;
        if (offy >= 240 - 55) {
            bg = CCLoadSprite::createSprite("activity1_6.png");
            bg->setAnchorPoint(ccp(0.5, 1));
            bg->setPosition(ccp(-270, 240));
            tBatchNode->addChild(bg);
            bg = CCLoadSprite::createSprite("activity1_6.png");
            bg->setFlipX(true);
            bg->setAnchorPoint(ccp(0.5, 1));
            bg->setPosition(ccp(270, 240));
            tBatchNode->addChild(bg);
        }
    }
    m_bgNode->addChild(tBatchNode);
    string tstr = _lang("137491");
    if (LuaController::getInstance()->checkActLuaValid(m_obj)) {
        tstr = LuaController::getInstance()->getBuyBtnLabel(m_obj->id);
    }
    CCCommonUtils::setButtonTitle(m_buyBtn, tstr.c_str());
    m_titleLabel->setString(m_obj->name);
    m_rewardLabel->setString(_lang("102139"));
    tstr = _lang("137497");
    if (LuaController::getInstance()->checkActLuaValid(m_obj))
    {
        tstr = LuaController::getInstance()->getCollectLabel(m_obj->id);
    }
    m_collectLabel->setString(tstr);
    m_refreshLabel->setString(_lang("104932"));
    
    m_itemId = m_obj->id;
    
    if (ActivityController::getInstance()->m_exc2Data.find(m_obj->id) != ActivityController::getInstance()->m_exc2Data.end()) {
        if (!ActivityController::getInstance()->checkIsToday(ActivityController::getInstance()->m_exc2RefInfo[m_obj->id].m_refreshTime)) {
            ActivityController::getInstance()->m_exc2RefInfo[m_obj->id].m_refreshTime = GlobalData::shared()->getTimeStamp();
            ActivityController::getInstance()->m_exc2RefInfo[m_obj->id].m_refresh = 0;
        }
        refreshTopIcons();
        refreshTabView();
        refreshRefBtn();
    } else {
        ActivityController::getInstance()->startGetAct6Data();
        addWaitInter();
    }
    return true;
}

bool ActivityExcNewView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}
void ActivityExcNewView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void ActivityExcNewView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (ccpDistance(pTouch->getLocation(), pTouch->getStartLocation()) < 10)
    {
        if (!isTouchInside(m_touchNode, pTouch)) {
            onClickCloseBtn(NULL, Control::EventType::TOUCH_DOWN);
        } else {
            if (!isTouchInside(m_listNode, pTouch)) {
                onGetTouchBodyMsg(nullptr);
            }
        }
    }
}

void ActivityExcNewView::onEnterFrame(float dt)
{
    if (ActivityController::getInstance()->m_exc2Data.find(m_obj->id) != ActivityController::getInstance()->m_exc2Data.end()) {
        if (!ActivityController::getInstance()->checkIsToday(ActivityController::getInstance()->m_exc2RefInfo[m_obj->id].m_refreshTime)) {
            ActivityController::getInstance()->m_exc2RefInfo[m_obj->id].m_refreshTime = GlobalData::shared()->getTimeStamp();
            ActivityController::getInstance()->m_exc2RefInfo[m_obj->id].m_refresh = 0;
            refreshRefBtn();
        }
    }
}

void ActivityExcNewView::addWaitInter()
{
    CCLOGFUNC();
//    removeWaitInter();
//    m_waitInterface2 = GameController::getInstance()->showWaitInterface(m_refreshBtnNode);
//    m_waitInterface1 = GameController::getInstance()->showWaitInterface(m_buyBtnNode);
    m_refreshBtn->setEnabled(false);
    m_buyBtn->setEnabled(false);
}

void ActivityExcNewView::removeWaitInter()
{
    CCLOGFUNC();
//    if (m_waitInterface1)
//    {
//        m_waitInterface1->remove();
//        m_waitInterface1 = nullptr;
//    }
//    if (m_waitInterface2) {
//        m_waitInterface2->remove();
//        m_waitInterface2 = nullptr;
//    }
    m_buyBtn->setEnabled(true);
    m_refreshBtn->setEnabled(true);
}

void ActivityExcNewView::refreshRefBtn()
{
    auto& map1 = ActivityController::getInstance()->m_exc2RefInfo;
    auto& map2 = ActivityController::getInstance()->m_exc2Data;
    if (map1.find(m_itemId) == map1.end())
    {
        return;
    }
    if (map2.find(m_itemId) == map2.end())
    {
        return;
    }
    int refresh = map1[m_itemId].m_refresh;
    int freeNum = map1[m_itemId].m_freeNum;
    if (refresh < freeNum) {
        m_goldLabel->setString("0");
    } else {
        m_goldLabel->setString(CC_ITOA(map1[m_itemId].m_refreshCost));
    }
}

void ActivityExcNewView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityExcNewView::onGetActDataSuccessMsg), MSG_GET_ACT6_DATA_SUCCESS, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityExcNewView::onGetActDataFailMsg), MSG_GET_ACT6_DATA_FAIL, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityExcNewView::onGetTouchHeadMsg), MSG_TOUCH_CELL_HEAD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityExcNewView::onGetTouchBodyMsg), MSG_TOUCH_CELL_BODY, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityExcNewView::onGetExcSuccessMsg), MSG_ACT6_EXC_SUCCESS, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityExcNewView::onGetExcFailMsg), MSG_ACT6_EXC_FAIL, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityExcNewView::onGetStartExcProcMsg), MSG_START_EXC_PROCESS, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityExcNewView::onGetEndExcProcMsg), MSG_END_EXC_PROCESS, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityExcNewView::onGetRefreshFailMsg), MSG_ACT6_REFRESH_FAIL, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityExcNewView::onGetEndRefreshMsg), MSG_END_REFRESH_PROCESS, NULL);
    
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ActivityExcNewView::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
}

void ActivityExcNewView::onExit()
{
    removeWaitInter();
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_GET_ACT6_DATA_SUCCESS);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_GET_ACT6_DATA_FAIL);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_START_EXC_PROCESS);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_END_EXC_PROCESS);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TOUCH_CELL_HEAD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TOUCH_CELL_BODY);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ACT6_EXC_SUCCESS);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ACT6_EXC_FAIL);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ACT6_REFRESH_FAIL);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_END_REFRESH_PROCESS);
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ActivityExcNewView::onEnterFrame), this);
    CCNode::onExit();
}

void ActivityExcNewView::onGetActDataSuccessMsg(cocos2d::CCObject * obj)
{
    removeWaitInter();
    refreshTopIcons();
    refreshTabView();
    refreshRefBtn();
}

void ActivityExcNewView::onGetActDataFailMsg(cocos2d::CCObject * obj)
{
//    removeWaitInter();
}

void ActivityExcNewView::onGetStartExcProcMsg(cocos2d::CCObject *)
{
    m_isCellRefreshing = true;
    addWaitInter();
    m_openIdx = 0;
    refreshTabView();
}

void ActivityExcNewView::onGetEndExcProcMsg(cocos2d::CCObject *)
{
    m_isCellRefreshing = false;
    removeWaitInter();
    refreshTopIcons();
}

void ActivityExcNewView::onGetExcSuccessMsg(CCObject*)
{
    
}

void ActivityExcNewView::onGetExcFailMsg(CCObject*)
{
    m_isCellRefreshing = false;
    removeWaitInter();
}

void ActivityExcNewView::onGetRefreshFailMsg(cocos2d::CCObject *)
{
    m_isRefreshing = false;
    removeWaitInter();
}

void ActivityExcNewView::onGetTouchHeadMsg(CCObject* obj)
{
    CCInteger* ccIdx = dynamic_cast<CCInteger*>(obj);
    
    if (m_isRefreshing || m_isCellRefreshing) {
        return;
    }
    if (m_openIdx == 0 && ccIdx) {
        int idx = ccIdx->getValue();
        m_openIdx = idx;
        refreshTabView();
        auto offset = m_tableView->getContentOffset();
        int leng = idx * 143 + 157;
        if (m_tableView->getViewSize().height + (-m_tableView->minContainerOffset().y - (-offset.y)) < leng ) {
            m_tableView->setContentOffset(ccp(0, leng - m_tableView->getViewSize().height + m_tableView->minContainerOffset().y));
        }
    } else {
        m_openIdx = 0;
        refreshTabView();
    }
}

void ActivityExcNewView::onGetTouchBodyMsg(CCObject* obj)
{
    if (m_isRefreshing || m_isCellRefreshing) {
        return;
    }
    if (m_openIdx != 0) {
        m_openIdx = 0;
        refreshTabView();
    }
}

void ActivityExcNewView::onGetEndRefreshMsg(cocos2d::CCObject * obj)
{
    m_isRefreshing = false;
    removeWaitInter();
    refreshTopIcons();
    refreshRefBtn();
}

void ActivityExcNewView::refreshTopIcons()
{
    float allW = 450;
    m_iconMainNode->removeAllChildrenWithCleanup(true);
    m_labelMainNode->removeAllChildrenWithCleanup(true);
    auto& info = ActivityController::getInstance()->m_exc2RefInfo[m_itemId];
    int num = info.m_excIds.size();
    float cellW = 0;
    if (num > 1) {
        cellW = allW / (num - 1);
    }
    for (int i = 0; i < info.m_excIds.size(); ++i) {
        auto node1 = CCNode::create();
        ActivityController::getInstance()->createGoodsIcon(node1, info.m_excIds[i], 40);
        node1->setAnchorPoint(ccp(0.5, 0.5));
        node1->setPosition(ccp(cellW * i, 0));
        node1->setTag(i);
        m_iconMainNode->addChild(node1);
        
        auto node2 = CCNode::create();
        auto& tool = ToolController::getInstance()->getToolInfoById(atoi(info.m_excIds[i].c_str()));
        auto label = CCLabelIF::create();
        string str = "x";
        str += CC_ITOA(tool.getCNT());
        label->setString(str);
        label->setAnchorPoint(ccp(0, 0.5));
        label->setPosition(ccp(0, 0));
        label->setFontSize(20);
        label->setColor({255,234,214});
        node2->addChild(label);
        node2->setPosition(ccp(cellW * i, 0));
        node2->setTag(i);
        m_labelMainNode->addChild(node2);
    }
}

void ActivityExcNewView::refreshTabView()
{
    if (!m_tableView) {
        m_tableView = CCTableView::create(this, m_listNode->getContentSize());
        m_tableView->setDirection(kCCScrollViewDirectionVertical);
        m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tableView->setDelegate(this);
        m_tableView->setTouchPriority(0);
        m_listNode->addChild(m_tableView);
    }
    float miny = m_tableView->minContainerOffset().y;
    CCPoint pos  = m_tableView->getContentOffset();
    m_tableView->reloadData();
    float mincurry = m_tableView->minContainerOffset().y;
    pos.y+=(-miny+mincurry);
    m_tableView->setContentOffset(pos);
}

bool ActivityExcNewView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode1", Node*, this->m_bgNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconMainNode", CCNode*, this->m_iconMainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_labelMainNode", CCNode*, this->m_labelMainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardLabel", CCLabelIF*, this->m_rewardLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_collectLabel", CCLabelIF*, this->m_collectLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtn", CCControlButton*, this->m_buyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_refreshBtn", CCControlButton*, this->m_refreshBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_refreshLabel", CCLabelIF*, this->m_refreshLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldLabel", CCLabelIF*, this->m_goldLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtnNode", CCNode*, this->m_buyBtnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_refreshBtnNode", CCNode*, this->m_refreshBtnNode);
    return false;
}
SEL_CCControlHandler ActivityExcNewView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCloseBtn", ActivityExcNewView::onClickCloseBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickRefreshBtn", ActivityExcNewView::onClickRefreshBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBuyBtn", ActivityExcNewView::onClickBuyBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickTipBtn", ActivityExcNewView::onClickTipBtn);
    return NULL;
}

void ActivityExcNewView::onClickCloseBtn(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
}

void ActivityExcNewView::onClickBuyBtn(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent)
{
    AutoSafeRef temp(this);
    
    PopupViewController::getInstance()->removeAllPopupView();
    auto view = StoreView::create();
    view->onClickStoreBtn(NULL, Control::EventType::TOUCH_DOWN);
    PopupViewController::getInstance()->addPopupInView(view);
}

void ActivityExcNewView::onClickRefreshBtn(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent)
{
    if (m_isCellRefreshing || m_isRefreshing) {
        return;
    }
    auto& map1 = ActivityController::getInstance()->m_exc2RefInfo;
    if (map1.find(m_itemId) == map1.end()) {
        return;
    }
    auto& map2 = ActivityController::getInstance()->m_exc2Data;
    if (map2.find(m_itemId) == map2.end()) {
        return;
    }
    int refresh = map1[m_itemId].m_refresh;
    int freeNum = map1[m_itemId].m_freeNum;
    if (refresh < freeNum)
    {
        m_openIdx = 0;
        refreshTabView();
        m_isRefreshing = true;
        removeWaitInter();
        addWaitInter();
        ActivityController::getInstance()->startAct6Refresh(m_itemId);
    } else {
        if (refresh == freeNum) {
            string tips = _lang("137496");
            if (LuaController::getInstance()->checkActLuaValid(m_obj)) {
                tips = LuaController::getInstance()->getActRefTip(m_itemId);
            }
            YesNoDialog::show(tips.c_str(),CCCallFunc::create(this, callfunc_selector(ActivityExcNewView::confirmFun)));
        } else {
            confirmFun();
        }
    }
}

void ActivityExcNewView::confirmFun()
{
    if (GlobalData::shared()->playerInfo.gold >= ActivityController::getInstance()->m_exc2RefInfo[m_obj->id].m_refreshCost)
    {
        CCLOGFUNC();
        m_openIdx = 0;
        refreshTabView();
        m_isRefreshing = true;
        removeWaitInter();
        addWaitInter();
        ActivityController::getInstance()->startAct6Refresh(m_itemId);
        GlobalData::shared()->playerInfo.gold -= ActivityController::getInstance()->m_exc2RefInfo[m_obj->id].m_refreshCost;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);
    } else {
        
        AutoSafeRef temp(this);

        PopupViewController::getInstance()->removeAllPopupView();
//        PopupViewController::getInstance()->addPopupView(GoldExchangeView::create(),false);
        YesNoDialog::gotoPayTips();
    }
}

void ActivityExcNewView::onClickTipBtn(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent)
{
    string tips = _lang("137493");
    if (LuaController::getInstance()->checkActLuaValid(m_obj))
    {
        tips = LuaController::getInstance()->getActTips(m_obj->id);
    }
    tips += "\n \n";
    PopupViewController::getInstance()->addPopupView(TipsView::create(tips, kCCTextAlignmentLeft));
}

CCSize ActivityExcNewView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if (m_openIdx == idx + 1) {
        return CCSize(540, 143 + 157);
    } else {
        return CCSize(540, 143);
    }
}
CCSize ActivityExcNewView::cellSizeForTable(CCTableView *table)
{
    return CCSize(540, 143);
}
CCTableViewCell* ActivityExcNewView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    auto& data = ActivityController::getInstance()->m_exc2Data[m_itemId];
    if(idx >= data.size()){
        return NULL;
    }
    ActivityExcNewCell* cell = (ActivityExcNewCell*)table->dequeueCell();
    if(idx < data.size()){
        if(cell){
            if (m_openIdx == idx + 1) {
                cell->setData(idx + 1, true);
            } else {
                cell->setData(idx + 1, false);
            }
        }else{
            if (m_openIdx == idx + 1) {
                cell = ActivityExcNewCell::create(m_obj, idx + 1, true, m_listNode);
            } else {
                cell = ActivityExcNewCell::create(m_obj, idx + 1, false, m_listNode);
            }
        }
    }
    return cell;
}
ssize_t ActivityExcNewView::numberOfCellsInTableView(CCTableView *table)
{
    return ActivityController::getInstance()->m_exc2Data[m_itemId].size();
}
void ActivityExcNewView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
    
}
void ActivityExcNewView::scrollViewDidScroll(CCScrollView* view)
{
    float mindy = m_tableView->minContainerOffset().y;
    float dy = m_tableView->getContentOffset().y;
    if(dy<mindy){
        m_tableView->setContentOffset(ccp(0, mindy));
    }
    
    float maxdy = m_tableView->maxContainerOffset().y;
    if (dy > maxdy) {
        m_tableView->setContentOffset(ccp(0, maxdy));
    }
}


////---------ActivityExcNewCell--------
ActivityExcNewCell* ActivityExcNewCell::create(ActivityEventObj* obj, int idx, bool open, CCNode* clickArea)
{
    auto ret = new ActivityExcNewCell(obj, idx, open, clickArea);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ActivityExcNewCell::init()
{
    CCBLoadFile("ActivityExcNewCell", this, this);
    p1 = m_middleBg->getPosition();
    p2 = m_rightBg->getPosition();
    CCCommonUtils::setButtonTitle(m_exchangeBtn, _lang("133076").c_str());
    m_glowNode = new CCAniNode();
    m_mainNode->addChild(m_glowNode);
    m_glowNode->release();
    CCBLoadFile("ActExcGlowNode", m_glowNode, m_glowNode);
    m_glowNode->setVisible(false);
    setData(m_idx, m_isOpen);
    return true;
}

void ActivityExcNewCell::setData(int idx, bool open)
{
    m_idx = idx;
    m_isOpen = open;
    
    m_mulNode->removeAllChildrenWithCleanup(true);
    m_iconNode->removeAllChildrenWithCleanup(true);
    m_itemNode->removeAllChildrenWithCleanup(true);
    m_noIconBgNode->removeAllChildrenWithCleanup(true);
    m_exchangeBtn->setEnabled(false);
    m_numLabel->setString("");
    m_desNameLabel->setString("");
    m_desInfoLabel->setString("");
    m_desNode->setVisible(false);
//    if (m_waitInterface) {
//        m_waitInterface->remove();
//        m_waitInterface = nullptr;
//    }
    
    if (m_isOpen) {
        this->setContentSize(CCSize(540, 143 + 157));
        m_mainNode->setPositionY(157);
        m_desNode->setVisible(true);
    } else {
        this->setContentSize(CCSize(540, 143));
        m_mainNode->setPositionY(0);
        m_desNode->setVisible(false);
    }
    m_glowNode->getAnimationManager()->runAnimationsForSequenceNamed("Loop");
    refreshRewardInfo(NULL);
    refreshGlowNode();
    
    string actId = m_obj->id;
    ActExc2Info& info = ActivityController::getInstance()->m_exc2Data[actId][idx];
    int itemNum = info.m_ids.size();
    m_rightBg->setPositionX(p2.x + itemNum * 67);
    m_middleBg->setPreferredSize(CCSize(p2.x - p1.x + itemNum * 67, 140));
    for (int i = 0; i < info.m_ids.size(); ++i) {
        ActivityExcNewCellCell* cell = ActivityExcNewCellCell::create(m_obj->id, m_idx, info.m_ids[i], i+1);
        cell->setPosition(ccp(i * 67, 0));
        cell->setTag(i);
        m_itemNode->addChild(cell);
    }
    for (int i = 0; i < 6 - info.m_ids.size(); ++i) {
        auto cell = CCLoadSprite::createScale9Sprite("activity1_2.png");
        cell->setPreferredSize(CCSize(60, 130));
        cell->setAnchorPoint(ccp(1, 0.5));
        cell->setPosition(ccp(- i* 67, 0));
        cell->setTag(i);
        m_noIconBgNode->addChild(cell);
    }
    
}

void ActivityExcNewCell::refreshRewardInfo(CCObject*)
{
    m_iconNode->removeAllChildrenWithCleanup(true);
    string tactId = m_obj->id;
    ActExc2Info& info = ActivityController::getInstance()->m_exc2Data[tactId][m_idx];
    float size = 80;
    auto dict = _dict(info.m_reward->objectAtIndex(0));
    auto value = _dict(dict->objectForKey("value"));
    string rid = value->valueForKey("id")->getCString();
    int num = value->valueForKey("num")->intValue();
    int multiple = info.m_multiple;
    m_numLabel->setString(CC_ITOA(num * multiple));
    ActivityController::getInstance()->createGoodsIcon(m_iconNode, rid, size);
    auto& tool = ToolController::getInstance()->getToolInfoById(atoi(rid.c_str()));
    m_desNameLabel->setString(tool.getName());
    m_desInfoLabel->setString(_lang(tool.des));
    if (ActivityController::getInstance()->checkCanExchange2(m_obj->id, m_idx)) {
        m_exchangeBtn->setEnabled(true);
    } else {
        m_exchangeBtn->setEnabled(false);
    }
}

void ActivityExcNewCell::refreshRewardInfoWithoutMulIcon(cocos2d::CCObject * obj)
{
    CCLOGFUNC();
//    m_iconNode->removeAllChildrenWithCleanup(true);
    string tactId = m_obj->id;
    ActExc2Info& info = ActivityController::getInstance()->m_exc2Data[tactId][m_idx];
    float size = 80;
    auto dict = _dict(info.m_reward->objectAtIndex(0));
    auto value = _dict(dict->objectForKey("value"));
    string rid = value->valueForKey("id")->getCString();
    int num = value->valueForKey("num")->intValue();
    int multiple = info.m_multiple;
    m_numLabel->setString(CC_ITOA(num));
//    ActivityController::getInstance()->createGoodsIcon(m_iconNode, rid, size);
    auto& tool = ToolController::getInstance()->getToolInfoById(atoi(rid.c_str()));
    m_desNameLabel->setString(tool.getName());
    m_desInfoLabel->setString(_lang(tool.des));
    if (ActivityController::getInstance()->checkCanExchange2(m_obj->id, m_idx)) {
        m_exchangeBtn->setEnabled(true);
    } else {
        m_exchangeBtn->setEnabled(false);
    }
    showRandomIcons(CCInteger::create(5));
}

void ActivityExcNewCell::showRandomIcons(cocos2d::CCObject * obj)
{
    CCLOGFUNC();
    CCInteger* ccint = dynamic_cast<CCInteger*>(obj);
    if (ccint == nullptr) {
        return;
    }
    int times = ccint->getValue();
    if (times < 0) {
        return;
    }
    auto& ids = ActivityController::getInstance()->m_exc2Data[m_obj->id][m_idx].m_rwdIds;
    int idsCnt = ids.size();
    if (idsCnt == 0) {
        times = 0;
    }
    if (times == 0) {
        CCLOG("%d", times);
        string tactId = m_obj->id;
        ActExc2Info& info = ActivityController::getInstance()->m_exc2Data[tactId][m_idx];
        auto dict = _dict(info.m_reward->objectAtIndex(0));
        auto value = _dict(dict->objectForKey("value"));
        string rid = value->valueForKey("id")->getCString();
        
        m_iconNode->stopAllActions();
        CCScaleTo* sc1 = CCScaleTo::create(0.2, 1, 0);
        CCCallFuncO* fun1 = CCCallFuncO::create(this, callfuncO_selector(ActivityExcNewCell::changePic), CCString::create(rid));
        CCScaleTo* sc2 = CCScaleTo::create(0.2, 1, 1);
        m_iconNode->runAction(CCSequence::create(sc1, fun1, sc2, NULL));
    }
    if (times > 0) {
        CCLOG("%d", times);
        int cnt = CCMathUtils::getRandomInt(1, 6) + times;
        cnt = cnt % idsCnt;
        string iconId = ids[cnt];
        m_iconNode->stopAllActions();
        CCScaleTo* sc1 = CCScaleTo::create(0.2, 1, 0);
        CCCallFuncO* fun1 = CCCallFuncO::create(this, callfuncO_selector(ActivityExcNewCell::changePic), CCString::create(iconId));
        CCScaleTo* sc2 = CCScaleTo::create(0.2, 1, 1);
        CCLOG("before m_iconNode->runAction(CCSequence::create(sc1, fun1, sc2, NULL));");
        m_iconNode->runAction(CCSequence::create(sc1, fun1, sc2, NULL));
        CCLOG("after m_iconNode->runAction(CCSequence::create(sc1, fun1, sc2, NULL));");
        times--;
        this->stopAllActions();
        CCLOG("before this->runAction(CCSequence::create(CCDelayTime::create(0.4f), CCCallFuncO::create(this, callfuncO_selector(ActivityExcNewCell::showRandomIcons), CCInteger::create(times)), NULL));");
        this->runAction(CCSequence::create(CCDelayTime::create(0.4f), CCCallFuncO::create(this, callfuncO_selector(ActivityExcNewCell::showRandomIcons), CCInteger::create(times)), NULL));
        CCLOG("after this->runAction(CCSequence::create(CCDelayTime::create(0.4f), CCCallFuncO::create(this, callfuncO_selector(ActivityExcNewCell::showRandomIcons), CCInteger::create(times)), NULL));");
    }
}

void ActivityExcNewCell::changePic(CCObject* obj)
{
    CCLOGFUNC();
    CCString* cs = dynamic_cast<CCString*>(obj);
    if (cs == nullptr) {
        return;
    }
    string id = cs->getCString();
    m_iconNode->removeAllChildrenWithCleanup(true);
    ActivityController::getInstance()->createGoodsIcon(m_iconNode, id, 80);
}

void ActivityExcNewCell::onClickExchangeBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    PopupViewController::getInstance()->addPopupView(ActExcConfirmView::create(m_obj->id, m_idx));
}

void ActivityExcNewCell::onGetEndExcProc(CCObject* obj)
{
    auto ccint = dynamic_cast<CCInteger*>(obj);
    if (ccint == nullptr) {
        return ;
    }
    int idx = ccint->getValue();
    if (idx != m_idx) {
        return;
    }
    refreshGlowNode();
}

void ActivityExcNewCell::onGetStartRefreshPro(cocos2d::CCObject * obj)
{
}

void ActivityExcNewCell::onGetEndRefreshPro(cocos2d::CCObject * obj)
{
    refreshGlowNode();
    m_mulNode->removeAllChildrenWithCleanup(true);
    int mul = ActivityController::getInstance()->m_exc2Data[m_obj->id][m_idx].m_multiple;
    if (mul > 1) {
        string mulStr = "X";
        mulStr += CC_ITOA(mul);
        m_mulLabel = CCLabelIFBMFont::create(mulStr.c_str(), "pve_fnt_boss.fnt");
        m_mulNode->addChild(m_mulLabel);
        m_mulLabel->setAnchorPoint(ccp(0.5, 0.2));
        m_mulLabel->setScale(5);
        m_mulLabel->setOpacity(255);
        CCScaleTo* sc = CCScaleTo::create(0.2f, 1.0);
        CCDelayTime* delay = CCDelayTime::create(0.35);
        CCFadeOut* fadeout = CCFadeOut::create(0.1);
        m_mulLabel->runAction(CCSequence::create(sc, delay, fadeout, NULL));
    }
    this->scheduleOnce(schedule_selector(ActivityExcNewCell::refreshNumLabel), 1.35f);
}

void ActivityExcNewCell::refreshNumLabel(float t)
{
    string tactId = m_obj->id;
    ActExc2Info& info = ActivityController::getInstance()->m_exc2Data[tactId][m_idx];
    float size = 80;
    auto dict = _dict(info.m_reward->objectAtIndex(0));
    auto value = _dict(dict->objectForKey("value"));
    int num = value->valueForKey("num")->intValue();
    int multiple = info.m_multiple;
    m_numLabel->setString(CC_ITOA(num * multiple));
}

void ActivityExcNewCell::refreshGlowNode()
{
    if (ActivityController::getInstance()->checkCanExchange2(m_obj->id, m_idx)) {
        m_glowNode->setVisible(true);
    } else {
        m_glowNode->setVisible(false);
    }
}

void ActivityExcNewCell::refreshRuneIcons()
{
    
}

void ActivityExcNewCell::onGetRefreshSucMsg(cocos2d::CCObject * obj)
{
    if (!m_obj) {
        return;
    }
    if (!obj)
    {
        return;
    }
    __String* ccs = dynamic_cast<__String*>(obj);
    if (!ccs)
    {
        return;
    }
    string str = ccs->getCString();
    if (str != m_obj->id) {
        return;
    }
    CCLOGFUNC();
    refreshRewardInfoWithoutMulIcon(NULL);
}

void ActivityExcNewCell::onGetRefreshFailMsg(cocos2d::CCObject * obj)
{
    
}

void ActivityExcNewCell::onGetExcSucMsg(CCObject* obj)
{
    auto ccint = dynamic_cast<CCInteger*>(obj);
    if (ccint == nullptr) {
        return;
    }
    int idx = ccint->getValue();
    if (idx != m_idx) {
        refreshGlowNode();
        return;
    }
//    refreshRewardInfo(NULL);
    string tactId = m_obj->id;
    ActExc2Info& info = ActivityController::getInstance()->m_exc2Data[tactId][m_idx];
    float size = 80;
    auto dict = _dict(info.m_reward->objectAtIndex(0));
    auto value = _dict(dict->objectForKey("value"));
    string rid = value->valueForKey("id")->getCString();
    int num = value->valueForKey("num")->intValue();
    int multiple = info.m_multiple;
    m_numLabel->setString(CC_ITOA(num * multiple));
    auto& tool = ToolController::getInstance()->getToolInfoById(atoi(rid.c_str()));
    m_desNameLabel->setString(tool.getName());
    m_desInfoLabel->setString(_lang(tool.des));
    if (ActivityController::getInstance()->checkCanExchange2(m_obj->id, m_idx)) {
        m_exchangeBtn->setEnabled(true);
    } else {
        m_exchangeBtn->setEnabled(false);
    }
    CCDelayTime* delay = CCDelayTime::create(2.5);
    CCCallFuncO* fun = CCCallFuncO::create(this, callfuncO_selector(ActivityExcNewCell::showRandomIcons), CCInteger::create(5));
    this->runAction(CCSequence::create(delay, fun, NULL));
}

void ActivityExcNewCell::onGetExcFailMsg(cocos2d::CCObject * obj)
{
    auto ccint = dynamic_cast<CCInteger*>(obj);
    if (ccint == nullptr ){
        return;
    }
    int idx = ccint->getValue();
    if (idx != m_idx) {
        return;
    }
//    if (m_waitInterface) {
//        m_waitInterface->remove();
//        m_waitInterface = nullptr;
//    }
}

void ActivityExcNewCell::onEnter()
{
    CCNode::onEnter();
    setTouchEnabled(true);
   // CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityExcNewCell::onGetEndExcProc), MSG_END_EXC_PROCESS, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityExcNewCell::onGetEndRefreshPro), MSG_END_REFRESH_PROCESS, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityExcNewCell::onGetRefreshSucMsg), MSG_ACT6_REFRESH_SUCCESS, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityExcNewCell::onGetExcSucMsg), MSG_ACT6_EXC_SUCCESS, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityExcNewCell::onGetExcFailMsg), MSG_ACT6_EXC_FAIL, NULL);
}

void ActivityExcNewCell::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ACT6_REFRESH_SUCCESS);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ACT6_EXC_SUCCESS);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ACT6_EXC_FAIL);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_END_REFRESH_PROCESS);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_END_EXC_PROCESS);
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCNode::onExit();
}

bool ActivityExcNewCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}
void ActivityExcNewCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void ActivityExcNewCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (ccpDistance(pTouch->getLocation(), pTouch->getStartLocation())>10)
        return;
    if (isTouchInside(m_desBg, pTouch) && m_isOpen == true && isTouchInside(m_clickArea, pTouch))
        return;
    if (isTouchInside(m_iconBg, pTouch) && isTouchInside(m_clickArea, pTouch))
    {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TOUCH_CELL_HEAD, CCInteger::create(m_idx));
    } else if(isTouchInside(m_touchNode, pTouch) && isTouchInside(m_clickArea, pTouch)) {
//        CCOrbitCamera* O1 = CCOrbitCamera::create(5, 1, 0, 0, 360, 90, 0);
//        CCScaleTo* sc1 = CCScaleTo::create(2.5, 1, 0);
//        CCScaleTo* sc2 = CCScaleTo::create(2.5, 1, 1);
//        m_iconNode->runAction(CCSequence::create(sc1, sc2, NULL));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TOUCH_CELL_BODY, CCInteger::create(m_idx));
    }
}

bool ActivityExcNewCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, this->m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_middleBg", CCScale9Sprite*, this->m_middleBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightBg", CCSprite*, this->m_rightBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode", CCNode*, this->m_itemNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconBg", CCSprite*, this->m_iconBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mulNode", CCNode*, this->m_mulNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noIconBgNode", CCNode*, this->m_noIconBgNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode", CCLayer*, this->m_desNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desBg", CCScale9Sprite*, this->m_desBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNameLabel", CCLabelIF*, this->m_desNameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desInfoLabel", CCLabelIF*, this->m_desInfoLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_exchangeBtn", CCControlButton*, this->m_exchangeBtn);
    return false;
}

SEL_CCControlHandler ActivityExcNewCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickExchangeBtn", ActivityExcNewCell::onClickExchangeBtn);
    return NULL;
}

/////------class ActivityExcNewCellCell
ActivityExcNewCellCell* ActivityExcNewCellCell::create(string actId, int idx, string itemId, int pos)
{
    auto ret = new ActivityExcNewCellCell(actId, idx, itemId, pos);
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ActivityExcNewCellCell::init()
{
    CCBLoadFile("ActivityExcNewCellCell", this, this);
//    ActivityController::getInstance()->createGoodsIcon(m_iconNode, m_itemId, 66);
//    ActivityController::getInstance()->createGoodsIconWithGray(m_iconNode, m_itemId, 66);
    changePicLastTime(__String::create(m_itemId));
    return true;
}

void ActivityExcNewCellCell::onEnter()
{
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityExcNewCellCell::onGetExcSucMsg), MSG_ACT6_EXC_SUCCESS, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityExcNewCellCell::onGetRefreshSucMsg), MSG_ACT6_REFRESH_SUCCESS, NULL);
}

void ActivityExcNewCellCell::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ACT6_REFRESH_SUCCESS);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ACT6_EXC_SUCCESS);
    CCNode::onExit();
}

void ActivityExcNewCellCell::onGetRefreshSucMsg(cocos2d::CCObject * obj)
{
    if (!obj)
    {
        return;
    }
    __String* ccs = dynamic_cast<__String*>(obj);
    if (!ccs)
    {
        return;
    }
    string str = ccs->getCString();
    if (str != m_actId) {
        return;
    }
    CCLOGFUNC();
    CCLOG("%d", m_idx);
    CCLOG("%d", m_pos);
    m_itemId = ActivityController::getInstance()->m_exc2Data[m_actId][m_idx].m_ids[m_pos - 1];
    float dt = 0.3;
    this->scheduleOnce(schedule_selector(ActivityExcNewCellCell::playEffectsByRefresh), (m_pos - 1) * dt);
}

void ActivityExcNewCellCell::onGetExcSucMsg(cocos2d::CCObject * obj)
{
    CCInteger* ccint = dynamic_cast<CCInteger*>(obj);
    if (ccint == nullptr) {
        return;
    }
    int idx = ccint->getValue();
    if (idx != m_idx) {
        refreshSprColor();
        return;
    }
    m_itemId = ActivityController::getInstance()->m_exc2Data[m_actId][m_idx].m_ids[m_pos - 1];
    float dt = 0.3;
    this->scheduleOnce(schedule_selector(ActivityExcNewCellCell::playEffectsByExc), 2.5 + (m_pos - 1) * dt);
}

void ActivityExcNewCellCell::refreshSprColor()
{
    m_iconNode->removeAllChildrenWithCleanup(true);
    changePicLastTime(__String::create(m_itemId));
//    CCSprite* spr = nullptr;
//    spr = dynamic_cast<CCSprite*>(m_iconNode->getChildByTag(888));
//    if (!spr)
//    {
//        return;
//    }
//    ToolInfo& info = ToolController::getInstance()->getToolInfoById(atoi(m_itemId.c_str()));
//    if (info.getCNT() <= 0) {
//        CCCommonUtils::setSpriteGray(spr, true);
//    } else {
//        CCCommonUtils::setSpriteGray(spr, false);
//    }
}

void ActivityExcNewCellCell::playEffectsByExc(float t)
{
    m_startPos = CCMathUtils::getRandomInt(1, ActivityController::getInstance()->m_exc2RefInfo[m_actId].m_excIds.size());
    m_startPos = CCMathUtils::getRandomInt(1, ActivityController::getInstance()->m_exc2RefInfo[m_actId].m_excIds.size());
    showRandomIconByExc(CCInteger::create(6));
}

void ActivityExcNewCellCell::playEffectsByRefresh(float t)
{
    CCLOGFUNC();
    m_startPos = CCMathUtils::getRandomInt(1, ActivityController::getInstance()->m_exc2RefInfo[m_actId].m_excIds.size());
    m_startPos = CCMathUtils::getRandomInt(1, ActivityController::getInstance()->m_exc2RefInfo[m_actId].m_excIds.size());
    showRandomIconByRefresh(CCInteger::create(6));
}

void ActivityExcNewCellCell::showRandomIconByExc(cocos2d::CCObject* obj)
{
    CCInteger* ccint = dynamic_cast<CCInteger*>(obj);
    if (ccint == nullptr) {
        return;
    }
    int times = ccint->getValue();
    if (times < 0) {
        return;
    }
    auto& ids = ActivityController::getInstance()->m_exc2Data[m_actId][m_idx].m_ids;
    int idsCnt = ids.size();
    if (times == 0) {
        m_iconNode->stopAllActions();
        CCScaleTo* sc1 = CCScaleTo::create(0.2, 1, 0);
        CCCallFuncO* fun1 = CCCallFuncO::create(this, callfuncO_selector(ActivityExcNewCellCell::changePicLastTime), CCString::create(m_itemId));
        CCScaleTo* sc2 = CCScaleTo::create(0.2, 1, 1);
        m_iconNode->runAction(CCSequence::create(sc1, fun1, sc2, NULL));
        if (m_pos == idsCnt)
        {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_END_EXC_PROCESS, CCInteger::create(m_idx));
        }
    }
    if (times > 0) {
        int cnt = m_startPos + times;
        cnt = cnt % idsCnt;
        string iconId = ids[cnt];
        m_iconNode->stopAllActions();
        CCScaleTo* sc1 = CCScaleTo::create(0.2, 1, 0);
        CCCallFuncO* fun1 = CCCallFuncO::create(this, callfuncO_selector(ActivityExcNewCellCell::changePic), CCString::create(iconId));
        CCScaleTo* sc2 = CCScaleTo::create(0.2, 1, 1);
        m_iconNode->runAction(CCSequence::create(sc1, fun1, sc2, NULL));
        times--;
        this->stopAllActions();
        this->runAction(CCSequence::create(CCDelayTime::create(0.4f), CCCallFuncO::create(this, callfuncO_selector(ActivityExcNewCellCell::showRandomIconByExc), CCInteger::create(times)), NULL));
    }

}

void ActivityExcNewCellCell::showRandomIconByRefresh(cocos2d::CCObject * obj)
{
    CCLOGFUNC();
    CCInteger* ccint = dynamic_cast<CCInteger*>(obj);
    if (ccint == nullptr) {
        return;
    }
    int times = ccint->getValue();
    if (times < 0) {
        return;
    }
    auto& ids = ActivityController::getInstance()->m_exc2RefInfo[m_actId].m_excIds;
    int idsCnt = ids.size();
    if (times == 0) {
        m_iconNode->stopAllActions();
        CCScaleTo* sc1 = CCScaleTo::create(0.2, 1, 0);
        CCCallFuncO* fun1 = CCCallFuncO::create(this, callfuncO_selector(ActivityExcNewCellCell::changePicLastTime), CCString::create(m_itemId));
        CCScaleTo* sc2 = CCScaleTo::create(0.2, 1, 1);
        m_iconNode->runAction(CCSequence::create(sc1, fun1, sc2, NULL));
        if (m_pos == idsCnt && m_idx == 1)
        {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_END_REFRESH_PROCESS);
        }
    }
    if (times > 0) {
        int cnt = m_startPos + times;
        cnt = cnt % idsCnt;
        string iconId = ids[cnt];
        m_iconNode->stopAllActions();
//        CCOrbitCamera* o1 = CCOrbitCamera::create(0.2, 1, 0, 0, 0, 0, -90);
//        CCCallFuncO* fun1 = CCCallFuncO::create(this, callfuncO_selector(ActivityExcNewCellCell::changePic), CCString::create(iconId));
//        CCOrbitCamera* o2 = CCOrbitCamera::create(0.2, 1, 0, 0, 0, -270, -90);
        CCScaleTo* sc1 = CCScaleTo::create(0.2, 1, 0);
        CCCallFuncO* fun1 = CCCallFuncO::create(this, callfuncO_selector(ActivityExcNewCellCell::changePic), CCString::create(iconId));
        CCScaleTo* sc2 = CCScaleTo::create(0.2, 1, 1);
        m_iconNode->runAction(CCSequence::create(sc1, fun1, sc2, NULL));
        times--;
        this->stopAllActions();
        this->runAction(CCSequence::create(CCDelayTime::create(0.4f), CCCallFuncO::create(this, callfuncO_selector(ActivityExcNewCellCell::showRandomIconByRefresh), CCInteger::create(times)), NULL));
    }
}

void ActivityExcNewCellCell::changePic(CCObject* obj)
{
    CCString* cs = dynamic_cast<CCString*>(obj);
    if (cs == nullptr) {
        return;
    }
    string itemId = cs->getCString();
    m_iconNode->removeAllChildrenWithCleanup(true);
//    ActivityController::getInstance()->createGoodsIcon(m_iconNode, id, 66);
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(atoi(itemId.c_str()));
    int cnt = info.getCNT();
    if (cnt <= 0) {
        ActivityController::getInstance()->createGoodsIconWithGray(m_iconNode, itemId, 66);
    } else {
        ActivityController::getInstance()->createGoodsIcon(m_iconNode, itemId, 66);
    }
}

void ActivityExcNewCellCell::changePicLastTime(Ref* ref)
{
    CCString* cs = dynamic_cast<CCString*>(ref);
    if (cs == nullptr) {
        return;
    }
    string itemId = cs->getCString();
    m_iconNode->removeAllChildrenWithCleanup(true);
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(atoi(itemId.c_str()));
    int cnt = info.getCNT();
    int nowCnt = 0;
    for (int i = 1 ; i < m_pos; ++i) {
        if (itemId == ActivityController::getInstance()->m_exc2Data[m_actId][m_idx].m_ids[i - 1]) {
            nowCnt ++;
        }
    }
    if (cnt <= nowCnt) {
        ActivityController::getInstance()->createGoodsIconWithGray(m_iconNode, itemId, 66);
    } else {
        ActivityController::getInstance()->createGoodsIcon(m_iconNode, itemId, 66);
    }
}

bool ActivityExcNewCellCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    return false;
}

ActExcConfirmView* ActExcConfirmView::create(string actId, int idx)
{
    auto ret = new ActExcConfirmView(actId, idx);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ActExcConfirmView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    auto node = CCBLoadFile("Act6ExcConfirmView", this, this);
    auto& info = ActivityController::getInstance()->m_exc2Data[m_actId][m_idx];
    float size = 100;
    auto dict = _dict(info.m_reward->objectAtIndex(0));
    auto value = _dict(dict->objectForKey("value"));
    string rid = value->valueForKey("id")->getCString();
    int num = value->valueForKey("num")->intValue();
    int multiple = info.m_multiple;
    m_numLabel->setString(CC_ITOA(num * multiple));
    ActivityController::getInstance()->createGoodsIcon(m_iconNode, rid, size);
    auto& tool = ToolController::getInstance()->getToolInfoById(atoi(rid.c_str()));
    m_nameLabel->setString(tool.getName());
    m_desLabel->setString(_lang(tool.des));
    m_confLabel->setString(_lang("137494"));
    CCCommonUtils::setButtonTitle(m_confBtn, _lang("133076").c_str());
    
    return true;
}

void ActExcConfirmView::onClickConfBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_START_EXC_PROCESS, CCInteger::create(m_idx));
    ActivityController::getInstance()->startAct6Exchange(m_actId, m_idx);
    PopupViewController::getInstance()->removePopupView(this);
}

bool ActExcConfirmView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}
void ActExcConfirmView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){}
void ActExcConfirmView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (ccpDistance(pTouch->getLocation(), pTouch->getStartLocation()) < 10)
    {
        if (!isTouchInside(m_touchNode, pTouch)) {
            PopupViewController::getInstance()->removePopupView(this);
        }
    }
}

void ActExcConfirmView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    
   // CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void ActExcConfirmView::onExit()
{
    setTouchEnabled(false);
  //  CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCNode::onExit();
}

bool ActExcConfirmView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, this->m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_confLabel", CCLabelIF*, this->m_confLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_confBtn", CCControlButton*, this->m_confBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, this->m_numLabel);
    return false;
}

SEL_CCControlHandler ActExcConfirmView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickConfBtn", ActExcConfirmView::onClickConfBtn);
    return NULL;
}






#pragma mark -
#pragma mark ActivityExcView Part



ActivityExcView* ActivityExcView::create(ActivityEventObj* obj)
{
    auto ret = new ActivityExcView(obj);
    if (ret && ret->init())
    {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ActivityExcView::init()
{
    PopupBaseView::init();
    
    if (!m_obj) {
        return false;
    }
    if (!ActivityController::getInstance()->m_exc1Data->objectForKey(m_obj->id)) {
        return false;
    }
    auto p = dynamic_cast<Act1Data*>(ActivityController::getInstance()->m_exc1Data->objectForKey(m_obj->id));
    if (!p) {
        return false;
    }
    m_itemId = p->itemId;
    if (ToolController::getInstance()->m_toolInfos.find(atoi(m_itemId.c_str())) == ToolController::getInstance()->m_toolInfos.end()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(513, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(513, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    this->setContentSize(Size(640, 852));
    if (CCCommonUtils::isIosAndroidPad())
    {
        this->setContentSize(Size(1536, 2048));
    }
    CCBLoadFile("ActivityExcView", this, this);
    auto size = Director::getInstance()->getWinSize();
    float add = size.height - 852;
    if (CCCommonUtils::isIosAndroidPad())
    {
        add = size.height - 2048;
        add = add / 2.4;
    }
    m_bgNode->setPositionY(m_bgNode->getPositionY() - add);
    {
        //加背景
        SpriteFrame* fr = CCLoadSprite::loadResource("technology_09.png");
        SpriteBatchNode* batch = SpriteBatchNode::createWithTexture(fr->getTexture());
        float h = 0;
        while (h < 852 + add) {
            auto spr = CCLoadSprite::createSprite("technology_09.png");
            spr->setPositionY(h);
            spr->setPositionX(0);
            spr->setAnchorPoint(Vec2(0, 0));
            batch->addChild(spr);
            h += spr->getContentSize().height;
        }
        m_bgNode->addChild(batch);
    }
    m_listNode->setPositionY(m_listNode->getPositionY() - add);
    m_listNode->setContentSize(Size(m_listNode->getContentSize().width, m_listNode->getContentSize().height + add));
//    m_geLayer->setTouchEnabled(true);
//    m_geLayer->setSwallowsTouches(true);
    auto layer = Layer::create();
    layer->setContentSize(Size(640, 250));
    layer->setAnchorPoint(Vec2(0, 0));
    layer->setTouchEnabled(true);
    layer->setSwallowsTouches(true);
    m_geLayer->addChild(layer);
    
    m_iconNode->removeAllChildren();
    ActivityController::getInstance()->createGoodsIcon(m_iconNode, m_itemId, 50);
    
    m_refreshBtn->setEnabled(true);
    m_desLabel->setColor({197, 130, 71});
    m_timeLabel->setColor({255, 229, 168});
    m_clipNode = CCClipNode::create(m_picNode->getContentSize());
    m_clipNode->setAnchorPoint(Vec2(0, 0));
    m_picNode->addChild(m_clipNode);
    bool st = false;
    if (LuaController::getInstance()->checkActLuaValid(m_obj)) {
        std::string fileName = FileUtils::getInstance()->getWritablePath() + "lua/resources/activity_" + m_obj->id + ".plist";
        if(CCFileUtils::sharedFileUtils()->isFileExist(fileName))
        {
            SpriteFrameCache::getInstance()->addSpriteFramesWithFile(fileName.c_str());
            string picName = m_obj->id + "_ad1.png";
            auto spr = CCLoadSprite::createSprite(picName.c_str());
            if (spr) {
                spr->setAnchorPoint(Vec2(0, 0));
                float my = p->position.y;
                if (my < -166) {
                    my = -166;
                }
                if (my > 0) {
                    my = 0;
                }
                spr->setPositionY(my);
                m_clipNode->addChild(spr);
                st = true;
            }
        }
    }
    if (!st)
    {
        auto spr = CCLoadSprite::createSprite("activity_ad_beiyong.png");
        if (spr) {
            spr->setAnchorPoint(Vec2(0, 0));
            spr->setPositionY(-150);
            m_clipNode->addChild(spr);
        }
    }
    
    m_tabView = TableView::create(this, m_listNode->getContentSize());
    m_tabView->setDirection(ScrollView::Direction::VERTICAL);
    m_tabView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    m_tabView->setDelegate(this);
    m_listNode->addChild(m_tabView);
    
    m_desLabel->setString(_lang("150329"));
    m_timeLabel->setString(CC_SECTOA(0));
    float tot = 0;
    tot += m_desLabel->getContentSize().width * m_desLabel->getOriginScaleX();
    tot += 40;
    tot += m_timeLabel->getContentSize().width * m_timeLabel->getOriginScaleX();
    m_desLabel->setPositionX(-126 - tot * 0.5 + m_desLabel->getContentSize().width * m_desLabel->getOriginScaleX());
    m_timeLabel->setPositionX(m_desLabel->getPositionX() + 40);
    m_shalou->setPositionX(m_desLabel->getPositionX() + 20);
    m_timeLabel->setString("");
    
    if (!ActivityController::getInstance()->isAct1DataInit(m_obj->id)) {
        ActivityController::getInstance()->startGetAct6Data();
    } else {
        onEnterFrame(0);
//        refreshView(nullptr);
    }
    
    return true;
}

void ActivityExcView::refreshBtn()
{
    if (ActivityController::getInstance()->isAct1DataInit(m_obj->id)) {
        auto p = dynamic_cast<Act1Data*>(ActivityController::getInstance()->getInstance()->m_exc1Data->objectForKey(m_obj->id));
        m_btnIconNode->removeAllChildren();
        if (p->flushTimes < p->freeFreshNum) {
            m_btnLabel->setString(_lang("103672"));
            m_btnLabel1->setString("");
            m_btnLabel2->setString("");
        } else {
            int num = 0;
            int out = p->flushTimes - p->freeFreshNum;
            if (p->freshCost.size() > 0) {
                if (out < p->freshCost.size())
                {
                    num = p->freshCost.at(out);
                } else {
                    num = p->freshCost.at(p->freshCost.size() - 1);
                }
            }
            if (num > 0) {
                m_btnLabel->setString("");
                m_btnLabel1->setString(_lang("150330"));
                string str = "×";
                str += CC_CMDITOA(num);
                m_btnLabel2->setString(str);
                ActivityController::getInstance()->createGoodsIcon(m_btnIconNode, m_itemId, 40);
                float totw = 0;
                totw += m_btnLabel1->getContentSize().width * m_btnLabel1->getOriginScaleX();
                totw += 42;
                totw += m_btnLabel2->getContentSize().width * m_btnLabel2->getOriginScaleX();
                m_btnLabel1->setPositionX(164 - totw * 0.5 + m_btnLabel1->getContentSize().width * m_btnLabel1->getOriginScaleX());
                m_btnIconNode->setPositionX(m_btnLabel1->getPositionX() + 21);
                m_btnLabel2->setPositionX(m_btnLabel1->getPositionX() + 42);
            }
        }
    }
}

void ActivityExcView::refreshView(Ref* ref)
{
    if (ActivityController::getInstance()->isAct1DataInit(m_obj->id)) {
        float dy1 = m_tabView->getContentOffset().y;
        float minDy1 = m_tabView->minContainerOffset().y;
        m_tabView->reloadData();
        float minDy2 = m_tabView->minContainerOffset().y;
        float addDy = minDy2 - minDy1;
        m_tabView->setContentOffset(CCPoint(0, dy1 + addDy));
        
        onRefreshToolInfo(NULL);
        
        auto p = dynamic_cast<Act1Data*>(ActivityController::getInstance()->getInstance()->m_exc1Data->objectForKey(m_obj->id));
        int left = p->refreshTime - (int)(GlobalData::shared()->getTimeStamp());
        m_timeLabel->setString(CC_SECTOA(left));
        
        refreshBtn();
    }
}

void ActivityExcView::onEnterFrame(float dt)
{
    if (ActivityController::getInstance()->isAct1DataInit(m_obj->id)) {
        auto p = dynamic_cast<Act1Data*>(ActivityController::getInstance()->getInstance()->m_exc1Data->objectForKey(m_obj->id));
        int left = p->refreshTime - (int)(GlobalData::shared()->getTimeStamp());
        left = left < 0? 0 : left;
        m_timeLabel->setString(CC_SECTOA(left));
    
//        if (ActivityController::getInstance()->isAct1DataShouldRefresh(m_obj->id))
//        {
//            ActivityController::getInstance()->startAct6Refresh(m_obj->id);
//        }
        if (ActivityController::getInstance()->checkAct1ReFreshTimes(m_obj->id)) {
            refreshBtn();
        }
    }
}

CCSize ActivityExcView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    return Size(640, 120);
}
CCSize ActivityExcView::cellSizeForTable(CCTableView *table)
{
    return Size(640, 120);
}
CCTableViewCell* ActivityExcView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    Act1Data* data = dynamic_cast<Act1Data*>(ActivityController::getInstance()->m_exc1Data->objectForKey(m_obj->id));
    if (!data) {
        return  nullptr;
    }
    if (idx >= data->data.size())
    {
        return nullptr;
    }
    auto it = data->data.begin();
    for (int i = 0; i < idx; ++i)
    {
        it++;
        if (it == data->data.end())
        {
            return nullptr;
        }
    }
    ActivityExcCell* cell = (ActivityExcCell*)table->dequeueCell();
    if (cell)
    {
        cell->setData(it->first);
    }
    else {
        cell = ActivityExcCell::create(m_obj->id, it->first);
    }
    return cell;
}
ssize_t ActivityExcView::numberOfCellsInTableView(CCTableView *table)
{
    Act1Data* data = dynamic_cast<Act1Data*>(ActivityController::getInstance()->m_exc1Data->objectForKey(m_obj->id));
    if (!data) {
        return  0;
    }
    return data->data.size();
}
void ActivityExcView::scrollViewDidScroll(CCScrollView* view)
{
    float mindy = m_tabView->minContainerOffset().y ;
    float dy = m_tabView->getContentOffset().y;
    if (dy < mindy)
    {
        m_tabView->setContentOffset(ccp(0, mindy));
    }
}

bool ActivityExcView::onAssignCCBMemberVariable(Ref * pTarget, const char * pMemberVariableName, cocos2d::Node * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt", CCLabelIF*, m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", Node*, m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", Node*, m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_geLayer", Layer*, m_geLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", Node*, m_picNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_shalou", Sprite*, m_shalou);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_refreshBtn", ControlButton*, m_refreshBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", Node*, m_iconNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnLabel", CCLabelIF*, m_btnLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnLabel1", CCLabelIF*, m_btnLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnLabel2", CCLabelIF*, m_btnLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnIconNode", Node*, m_btnIconNode);
    return false;
}

SEL_CCControlHandler ActivityExcView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickRefresh", ActivityExcView::onClickRefresh);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHelpBtnClick", ActivityExcView::onHelpBtnClick);
    return nullptr;
}

void ActivityExcView::onEnter()
{
    Node::onEnter();
    UIComponent::getInstance()->showPopupView(1);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityExcView::onGetMsgDataInit), MSG_GET_ACT6_DATA_SUCCESS, nullptr);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityExcView::onRefreshToolInfo), MSG_REFREASH_TOOL_DATA, nullptr);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityExcView::onGetMsgExchangeEnd), MSG_ACT6_EXC_SUCCESS, nullptr);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityExcView::onGetMsgRefreshEnd), MSG_ACT6_REFRESH_SUCCESS, nullptr);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityExcView::onGetMsgRefreshFail), MSG_ACT6_REFRESH_FAIL, nullptr);
    Director::getInstance()->getScheduler()->scheduleSelector(schedule_selector(ActivityExcView::onEnterFrame), this, 1.0, kRepeatForever, 0, false);
    refreshView(nullptr);
    m_titleTxt->setString(m_obj->name);
}
void ActivityExcView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_GET_ACT6_DATA_SUCCESS);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REFREASH_TOOL_DATA);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ACT6_EXC_SUCCESS);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ACT6_REFRESH_SUCCESS);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ACT6_REFRESH_FAIL);
    Director::getInstance()->getScheduler()->unscheduleSelector(schedule_selector(ActivityExcView::onEnterFrame), this);
    Node::onExit();
}
void ActivityExcView::onRefreshToolInfo(Ref* ref){
    auto& info = ToolController::getInstance()->m_toolInfos[atoi(m_itemId.c_str())];
    string tot = "×";
    tot += CC_CMDITOA(info.getCNT());
    m_numLabel->setString(tot);
}
void ActivityExcView::onGetMsgDataInit(Ref* ref)
{
    if (ActivityController::getInstance()->isAct1DataInit(m_obj->id)) {
        refreshView(nullptr);
    }
}

void ActivityExcView::onGetMsgExchangeEnd(cocos2d::Ref *ref)
{
    if (ActivityController::getInstance()->isAct1DataInit(m_obj->id)) {
        onRefreshToolInfo(NULL);
        
        auto p = dynamic_cast<Act1Data*>(ActivityController::getInstance()->getInstance()->m_exc1Data->objectForKey(m_obj->id));
        int left = p->refreshTime - (int)(GlobalData::shared()->getTimeStamp());
        m_timeLabel->setString(CC_SECTOA(left));
        
        refreshBtn();
    }
}

void ActivityExcView::onGetMsgRefreshEnd(cocos2d::Ref *ref)
{
    if (!m_obj) {
        return;
    }
    if (!ref)
    {
        return;
    }
    __String* ccs = dynamic_cast<__String*>(ref);
    if (!ccs)
    {
        return;
    }
    string str = ccs->getCString();
    if (str != m_obj->id) {
        return;
    }
    if (ActivityController::getInstance()->isAct1DataInit(m_obj->id)) {
        onRefreshToolInfo(NULL);
        
        auto p = dynamic_cast<Act1Data*>(ActivityController::getInstance()->getInstance()->m_exc1Data->objectForKey(m_obj->id));
        int left = p->refreshTime - (int)(GlobalData::shared()->getTimeStamp());
        m_timeLabel->setString(CC_SECTOA(left));
        
        refreshBtn();
    }
}

void ActivityExcView::onGetMsgRefreshFail(Ref* ref)
{
    if (!ref)
    {
        return;
    }
    __Dictionary* params = dynamic_cast<__Dictionary*>(ref);
    if (!params)
    {
        return;
    }
    string actId = params->valueForKey("m_actId")->getCString();
    if (actId == m_obj->id) {
        if (params->objectForKey("errorCode")) {
            CCCommonUtils::flyHint("", "", _lang(params->valueForKey("errorCode")->getCString()));
        }
    }
}

void ActivityExcView::onClickRefresh(Ref* ref, Control::EventType)
{
    if (ActivityController::getInstance()->isAct1DataInit(m_obj->id)) {
        if (!ActivityController::getInstance()->isCanRefreshByhand(m_obj->id)) {
            CCCommonUtils::flyHint("", "", _lang("102198"));
        } else {
            ActivityController::getInstance()->startAct6Refresh(m_obj->id);
        }
    }
}

void ActivityExcView::onHelpBtnClick(cocos2d::Ref *ref, Control::EventType)
{
    string tips = _lang("150331");
    PopupViewController::getInstance()->addPopupView(TipsView::create(tips, kCCTextAlignmentLeft));
}

ActivityExcCell* ActivityExcCell::create(string actId, int level)
{
    auto ret = new ActivityExcCell(actId, level);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ActivityExcCell::init()
{
    CCBLoadFile("ActivityExcCell", this, this);
    if (!m_actId.empty()) {
        Act1Data* data = dynamic_cast<Act1Data*>(ActivityController::getInstance()->m_exc1Data->objectForKey(m_actId));
        string itemId = data->itemId;
        if (ToolController::getInstance()->m_toolInfos.find(atoi(itemId.c_str())) != ToolController::getInstance()->m_toolInfos.end()) {
            m_itemId = itemId;
            ActivityController::getInstance()->createGoodsIcon(m_picNode, m_itemId, 40);
        }
    }
    m_rareLabel->setColor({124, 210, 25});
    m_numLabel->setColor({255, 220, 166});
    m_nameLabel->setColor({101, 116, 126});
    m_rwdedLabel->setString(_lang("150333"));
    m_glowNode->setScaleX(0.85);
    m_glowNode->setScaleY(0.87);
    m_aniNode = new CCAniNode();
    m_glowNode->addChild(m_aniNode);
    m_aniNode->release();
    CCBLoadFile("ActExcGlowNode", m_aniNode, m_aniNode);
    m_aniNode->setVisible(false);
    
    setData(m_level);
    return true;
}
void ActivityExcCell::setData(int level, bool ani)
{
    if (ani)
    {   m_mainNode->setScale(1);
        m_mainNode->stopAllActions();
        DelayTime* delay1 = DelayTime::create(level * 0.033 * 3);
        ScaleTo* sc1 = ScaleTo::create(0.033 * 3, 1, 1.2);
        ScaleTo* sc2 = ScaleTo::create(0.033 * 3, 1, 0.8);
        ScaleTo* sc3 = ScaleTo::create(0.033 * 3, 1, 0);
        CCCallFuncO* fun = CCCallFuncO::create(this, callfuncO_selector(ActivityExcCell::refresh), __Integer::create(level));
        ScaleTo* sc4 = ScaleTo::create(0.033 * 4, 1, 1.1);
        ScaleTo* sc5 = ScaleTo::create(0.033 * 4, 1, 1);
        m_mainNode->runAction(Sequence::create(delay1, sc1, sc2, sc3, fun, sc4, sc5, NULL));
        return;
    }
    
    m_level = level;
    m_nameLabel->setString("");
    m_rareLabel->setString("");
    m_blueBtn->setVisible(false);
    m_greenBtn->setVisible(false);
    m_iconNode->removeAllChildren();
    m_numLabel->setString("");
    m_blueBtn->setEnabled(true);
    m_greenBtn->setEnabled(true);
    m_btnNode->setVisible(false);
    m_rwdedNode->setVisible(false);
    m_needNum->setString("");
    m_aniNode->setVisible(false);
    if (m_actId.empty()) {
        return;
    }
    Act1Data* data = dynamic_cast<Act1Data*>(ActivityController::getInstance()->m_exc1Data->objectForKey(m_actId));
    if (!data) {
        return;
    }
    if (data->data.find(m_level) == data->data.end()) {
        return;
    }
    Act1ExcItem* item = data->data[m_level];
    if (!item->reward || item->reward->count() == 0) {
        return;
    }
    RewardClass* rwd = dynamic_cast<RewardClass*>(item->reward->objectAtIndex(0));
    if (!rwd) {
        return;
    }
    auto it = ToolController::getInstance()->m_toolInfos.find(atoi(rwd->itemId.c_str()));
    if (it == ToolController::getInstance()->m_toolInfos.end()) {
        return;
    }
    CCCommonUtils::createGoodsIcon(atoi(rwd->itemId.c_str()), m_iconNode, Size(77, 77));
    m_numLabel->setString(CC_CMDITOA(rwd->num));
    if (item->color == 0) {//普通物品
        m_blueBtn->setVisible(true);
    } else {
        m_rareLabel->setString(_lang("150360"));
        m_greenBtn->setVisible(true);
        m_aniNode->setVisible(true);
        m_aniNode->getAnimationManager()->runAnimationsForSequenceNamed("Loop");
    }
    if (ActivityController::getInstance()->isAct1ItemHasExchange(m_actId, m_level)) {
        m_rwdedNode->setVisible(true);
        m_btnNode->setVisible(false);
        m_aniNode->setVisible(false);
    } else {
        m_rwdedNode->setVisible(false);
        m_btnNode->setVisible(true);
    }
    m_nameLabel->setString(it->second.getName());
    float w = m_nameLabel->getContentSize().width * m_nameLabel->getOriginScaleX();
    float h = m_nameLabel->getContentSize().height * m_nameLabel->getOriginScaleY();
    if (h > 30)
    {
        if (item->color == 0) {
            m_nameLabel->setPositionY(h / 2.0);
        } else {
            m_nameLabel->setPositionY(14);
        }
    }
    string sss = "×";
    sss += CC_ITOA(item->needNum);
    m_needNum->setString(sss);
}

bool ActivityExcCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rareLabel", CCLabelIF*, m_rareLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_blueBtn", ControlButton*, m_blueBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_greenBtn", ControlButton*, m_greenBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", Node*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_needNum", CCLabelIF*, m_needNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", Node*, m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, m_numLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdedBg", Sprite*, m_rwdedBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdedLabel", CCLabelIF*, m_rwdedLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdedNode", Node*, m_rwdedNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode", Node*, m_btnNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", Node*, m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_glowNode", Node*, m_glowNode);
    return false;
}
SEL_CCControlHandler ActivityExcCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBlue", ActivityExcCell::onClickBlue);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickGreen", ActivityExcCell::onClickGreen);
    return nullptr;
}

void ActivityExcCell::onClickBlue(Ref* ref, Control::EventType)
{
    if(ActivityController::getInstance()->startAct6Exchange(m_actId, m_level))
    {
        m_btnNode->setVisible(false);
        m_rwdedNode->setVisible(true);
    }
}

void ActivityExcCell::onClickGreen(Ref* ref, Control::EventType)
{
    if (ActivityController::getInstance()->startAct6Exchange(m_actId, m_level))
    {
        m_btnNode->setVisible(false);
        m_rwdedNode->setVisible(true);
    }
}

void ActivityExcCell::onEnter()
{
    Node::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityExcCell::onGetMsgExchangeEnd), MSG_ACT6_EXC_SUCCESS, nullptr);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityExcCell::onGetMsgRefreshEnd), MSG_ACT6_REFRESH_SUCCESS, nullptr);
}

void ActivityExcCell::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ACT6_EXC_SUCCESS);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ACT6_REFRESH_SUCCESS);
    Node::onExit();
}

void ActivityExcCell::onGetMsgExchangeEnd(Ref* ref)
{
    if (ref) {
        __Integer* ccint = dynamic_cast<__Integer*>(ref);
        if (ccint) {
            int level = ccint->getValue();
            if (level == m_level) {
                setData(m_level);
            }
        }
    }
}

void ActivityExcCell::onGetMsgRefreshEnd(Ref* ref)
{
    if (!ref)
    {
        return;
    }
    __String* ccs = dynamic_cast<__String*>(ref);
    if (!ccs)
    {
        return;
    }
    string str = ccs->getCString();
    if (str != m_actId) {
        return;
    }
    setData(m_level, true);
}

void ActivityExcCell::refresh(cocos2d::Ref *ref)
{
    if (ref) {
        __Integer* ccint = dynamic_cast<__Integer*>(ref);
        if (ccint) {
            m_level = ccint->getValue();
            setData(m_level);
        }
    }
}