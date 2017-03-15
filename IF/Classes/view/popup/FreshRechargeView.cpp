//
//  FreshRechargeView.cpp
//  IF
//
//  Created by 邢晓瑞 on 15/11/12.
//
//

#include "FreshRechargeView.hpp"
#include "ActivityController.h"
#include "ToolController.h"
#include "EquipmentController.h"
#include "RewardController.h"
#include "PopupViewController.h"
#include "GoldExchangeView_NEW.hpp"

bool FreshRechargeView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", Node*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", Node*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_label", CCLabelIF*, this->m_label);
    return false;
}

SEL_CCControlHandler FreshRechargeView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return nullptr;
}

FreshRechargeView* FreshRechargeView::create()
{
    FreshRechargeView* ret = new FreshRechargeView();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
bool FreshRechargeView::init()
{
    PopupBaseView::init();
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(513, true);
    CCLoadSprite::doResourceByCommonIndex(8, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(513, false);
        CCLoadSprite::doResourceByCommonIndex(8, false);
    });
    CCBLoadFile("FreshRechargeView", this, this);
    setContentSize(Size(640, 852));
    if (CCCommonUtils::isIosAndroidPad())
    {
        setContentSize(Size(1536, 2048));
    }
    auto size = CCDirector::sharedDirector()->getWinSize();
    float add = size.height - 852;
    if (CCCommonUtils::isIosAndroidPad()) {
        add = size.height - 2048;
        add = add / 2.4;
    }
    m_bgNode->setPositionY(m_bgNode->getPositionY() - add);
    m_listNode->setPositionY(m_listNode->getPositionY() - add);
    m_listNode->setContentSize(Size(m_listNode->getContentSize().width, m_listNode->getContentSize().height + add));
    float height = 0;
    SpriteFrame* frame = CCLoadSprite::loadResource("technology_09.png");
    SpriteBatchNode* batch = SpriteBatchNode::createWithTexture(frame->getTexture());
    float totalHeight = size.height;
    if (CCCommonUtils::isIosAndroidPad()) {
        totalHeight = totalHeight / 2.4;
    }
    while (height < totalHeight) {
        Sprite* spr = CCLoadSprite::createSprite("technology_09.png");
        spr->setAnchorPoint(Vec2(0.5, 0));
        spr->setPositionX(0);
        spr->setPositionY(height);
        batch->addChild(spr);
        height += spr->getContentSize().height;
    }
    m_bgNode->addChild(batch);
    
    m_tabView = TableView::create(this, m_listNode->getContentSize());
    m_tabView->setDirection(ScrollView::Direction::VERTICAL);
    m_tabView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    m_tabView->setDelegate(this);
    m_tabView->setPosition(0, 0);
    m_listNode->addChild(m_tabView);
    
//    m_label->setString(_lang("101061"));
    refreshLabel();
    return true;
}

void FreshRechargeView::onEnter()
{
    Node::onEnter();
    setTitleName(_lang("101061"));
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FreshRechargeView::onGetMsgRwdEnd), FRESH_RECHARGE_REWARD_END, NULL);
    refreshView();
}
void FreshRechargeView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, FRESH_RECHARGE_REWARD_END);
    Node::onExit();
}

void FreshRechargeView::refreshLabel()
{
    m_label->setString("");
    if (FreshRechargeController::getInstance()->isCanRewardAll()) {
        m_label->setString(_lang("101063"));
    } else {
        int left = FreshRechargeController::getInstance()->getLeftToNextStage();
        m_label->setString(_lang_1("101062", CC_CMDITOA(left).c_str()));
    }
}

void FreshRechargeView::onGetMsgRwdEnd(Ref* ref)
{
    refreshView();
}

void FreshRechargeView::scrollViewDidScroll(ScrollView* view)
{
    float mindy = m_tabView->minContainerOffset().y ;
    float dy = m_tabView->getContentOffset().y;
    if (dy < mindy)
    {
        m_tabView->setContentOffset(ccp(0, mindy));
    }
}
void FreshRechargeView::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
}
cocos2d::CCSize FreshRechargeView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(640, 210);
}
cocos2d::CCSize FreshRechargeView::cellSizeForTable(TableView *table)
{
    return Size(640, 210);
}
TableViewCell* FreshRechargeView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    if (idx >= FreshRechargeController::getInstance()->m_freshRechargeVec.size())
    {
        return nullptr;
    }
    FreshRechargeCell* cell = (FreshRechargeCell*)table->dequeueCell();
    if (cell)
    {
        cell->setData(idx);
    }
    else {
        cell = FreshRechargeCell::create(idx);
    }
    return cell;
}
ssize_t FreshRechargeView::numberOfCellsInTableView(TableView *table)
{
    return FreshRechargeController::getInstance()->m_freshRechargeVec.size();
}


void FreshRechargeView::refreshView()
{
    float dy1 = m_tabView->getContentOffset().y;
    float minDy1 = m_tabView->minContainerOffset().y;
    m_tabView->reloadData();
    float minDy2 = m_tabView->minContainerOffset().y;
    float addDy = minDy2 - minDy1;
    m_tabView->setContentOffset(CCPoint(0, dy1 + addDy));
    
    refreshLabel();
}



FreshRechargeCell* FreshRechargeCell::create(int idx)
{
    FreshRechargeCell* ret = new FreshRechargeCell(idx);
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool FreshRechargeCell::init()
{
    CCBLoadFile("FreshRechargeCell", this, this);
    setData(m_idx);
    return true;
}

void FreshRechargeCell::setData(int idx)
{
    m_groupLabel->setString("");
    m_showNode1->setVisible(false);
    m_showNode2->setVisible(false);
    m_rwdedBg->setVisible(false);
    m_rwdedLabel->setString("");
    m_rwdBtn->setVisible(false);
    m_desNode->setVisible(false);
    m_rwdBtn->setEnabled(true);
    m_buyBtn->setVisible(false);
    
    m_idx = idx;
    
    vector<FreshRechargeItem>& vec = FreshRechargeController::getInstance()->m_freshRechargeVec;
    if (idx >= vec.size()) {
        return;
    }
    FreshRechargeItem item = vec.at(idx);
    if (item.m_rwds.size() > 0)
    {
        FrReReward rwd0 = item.m_rwds.at(0);
        bool ok = isValid(rwd0);
        if (ok) {
            createIcon(rwd0, m_iconNode1);
            m_numLabel1->setString(CC_CMDITOA(rwd0.count));
            m_showNode1->setVisible(true);
        }
    }
    if (item.m_rwds.size() > 1) {
        FrReReward rwd1 = item.m_rwds.at(1);
        bool ok = isValid(rwd1);
        if (ok)
        {
            createIcon(rwd1, m_iconNode2);
            m_numLabel2->setString(CC_CMDITOA(rwd1.count));
            m_showNode2->setVisible(true);
        }
    }
    
    
    if (FreshRechargeController::getInstance()->isHasExchange(item.m_itemId)) {
        m_rwdedBg->setVisible(true);
        m_rwdedLabel->setString(_lang("101312"));//101312=已领取
    } else {
        m_rwdBtn->setVisible(true);
        CCCommonUtils::setButtonTitle(m_rwdBtn, _lang("101313").c_str());//101313=领取
        if (FreshRechargeController::getInstance()->isCanExchange(item.m_itemId)) {
            m_rwdBtn->setVisible(true);
        } else {
            m_rwdBtn->setVisible(false);
            //显示去充值按钮
            if (idx>=1) {
                FreshRechargeItem lastItem = vec.at(idx-1);
                if (FreshRechargeController::getInstance()->isHasExchange(lastItem.m_itemId)) {
                    m_buyBtn->setVisible(true);
                    CCCommonUtils::setButtonTitle(m_buyBtn, _lang("101101").c_str());//101101		去充值
                }
            }else if (idx==0){
                m_buyBtn->setVisible(true);
                CCCommonUtils::setButtonTitle(m_buyBtn, _lang("101101").c_str());//101101		去充值
            }
        }
    }
    
    m_groupLabel->setString(_lang_2("133258", CC_ITOA(m_idx + 1), CC_CMDITOA(item.m_node).c_str()));//133258=奖励{0}（购买{1}金币可获得）
}

bool FreshRechargeCell::isValid(FrReReward& rwd)
{
    bool ok = true;
    if (rwd.type == 7)
    {
        if (ToolController::getInstance()->m_toolInfos.find(atoi(rwd.itemId.c_str())) == ToolController::getInstance()->m_toolInfos.end()) {
            ok = false;
        }
    } else if (rwd.type == 14){
        if (EquipmentController::getInstance()->EquipmentInfoMap.find(atoi(rwd.itemId.c_str())) == EquipmentController::getInstance()->EquipmentInfoMap.end()) {
            ok = false;
        }
    }
    return ok;
}

void FreshRechargeCell::createIcon(FrReReward& rwd, Node* parent)
{
    if (rwd.type == 7)
    {
        CCCommonUtils::createGoodsIcon(atoi(rwd.itemId.c_str()), parent, Size(85, 85));
    } else if (rwd.type == 14) {
        int eID = atoi(rwd.itemId.c_str());
        auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[eID];
        string ebgPath = CCCommonUtils::getToolBgByColor(eInfo.color);
        auto ebgIcon = CCLoadSprite::createSprite(ebgPath.c_str());
        CCCommonUtils::setSpriteMaxSize(ebgIcon, 85, true);
        parent->addChild(ebgIcon);
        string epicStr = CCCommonUtils::getIcon(rwd.itemId);
        auto eicon = CCLoadSprite::createSprite(epicStr.c_str(),true,CCLoadSpriteType_EQUIP);
        CCCommonUtils::setSpriteMaxSize(eicon, 80, true);
        parent->addChild(eicon);
    } else {
        int oID = rwd.type;
        auto pic = CCLoadSprite::createSprite(RewardController::getInstance()->getPicByType(rwd.type, atoi(rwd.itemId.c_str())) .c_str());
        if(pic){
            Sprite* iconBg = NULL;
            if(oID == R_GOLD){
                iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(ORANGE).c_str());
            }else{
                iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(WHITE).c_str());
            }
            CCCommonUtils::setSpriteMaxSize(iconBg, 85,true);
            parent->addChild(iconBg);
            CCCommonUtils::setSpriteMaxSize(pic, 80,true);
            parent->addChild(pic);
        }
    }
}

bool FreshRechargeCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_desNode->setVisible(false);
    if (isTouchInside(m_touchNode1, pTouch) && m_showNode1->isVisible()) {
        showDescription(0);
    } else if (isTouchInside(m_touchNode2, pTouch) && m_showNode2->isVisible()) {
        showDescription(1);
    }
    return true;
}
void FreshRechargeCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_desNode->setVisible(false);
}
void FreshRechargeCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
}

bool FreshRechargeCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdedBg", Sprite*, this->m_rwdedBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_groupLabel", CCLabelIF*, this->m_groupLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdedLabel", CCLabelIF*, this->m_rwdedLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdBtn", ControlButton*, this->m_rwdBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtn", ControlButton*, this->m_buyBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_showNode1", Node*, this->m_showNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_showBg1", Sprite*, this->m_showBg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode1", Node*, this->m_iconNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel1", CCLabelIF*, this->m_numLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode1", Node*, this->m_touchNode1);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_showNode2", Node*, this->m_showNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_showBg2", Sprite*, this->m_showBg2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode2", Node*, this->m_iconNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel2", CCLabelIF*, this->m_numLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode2", Node*, this->m_touchNode2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode", Node*, this->m_desNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desBg", Scale9Sprite*, this->m_desBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, this->m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desName", CCLabelIF*, this->m_desName);
    return false;
}

SEL_CCControlHandler FreshRechargeCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickRwdBtn", FreshRechargeCell::onClickRwdBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBuyBtnClick", FreshRechargeCell::onBuyBtnClick);
    return nullptr ;
}

void FreshRechargeCell::onEnter()
{
    Node::onEnter();
    setTouchEnabled(true);
    setSwallowsTouches(false);
}

void FreshRechargeCell::onExit()
{
    Node::onExit();
}

void FreshRechargeCell::onClickRwdBtn(Ref*, Control::EventType)
{
    FreshRechargeController::getInstance()->startReward(m_idx);
    m_rwdBtn->setEnabled(false);
}

void FreshRechargeCell::onBuyBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    CCLOGFUNC();
    PopupViewController::getInstance()->goBackPopupView();
}

void FreshRechargeCell::showDescription(int pos)
{
    m_desNode->setVisible(false);
    m_desBg->setContentSize(Size(295, 149));
    m_desBg->setPreferredSize(Size(295, 149));
    if (pos < 0 || pos > 1)
    {
        return;
    }
    
    Vec2 vec;
    if (pos == 1) {
        vec.x = 494;
        vec.y = 78;
    } else {
        vec.x = 364;
        vec.y = 78;
    }
    m_desNode->setPosition(vec);
    
    if (m_idx >= FreshRechargeController::getInstance()->m_freshRechargeVec.size()) {
        return;
    }
    FreshRechargeItem item = FreshRechargeController::getInstance()->m_freshRechargeVec[m_idx];
    if (pos >= item.m_rwds.size()) {
        return;
    }
    FrReReward rwd = item.m_rwds.at(pos);
    string name("");
    string des("");
    if (rwd.type == R_GOODS) {
        int tId = atoi(rwd.itemId.c_str());
        if (ToolController::getInstance()->m_toolInfos.find(tId) != ToolController::getInstance()->m_toolInfos.end())
        {
            auto& info = ToolController::getInstance()->getToolInfoById(tId);
            name = info.getName();
            des = _lang(info.des);
        }
    } else if(rwd.type == R_EQUIP) {
        int tId = atoi(rwd.itemId.c_str());
        if (EquipmentController::getInstance()->EquipmentInfoMap.find(tId) != EquipmentController::getInstance()->EquipmentInfoMap.end())
        {
            auto& info = EquipmentController::getInstance()->EquipmentInfoMap[tId];
            name = _lang(info.name) + "   " + _lang("102203") + " " + CC_ITOA(info.level);
            des = _lang(info.des);
        }
    } else {
        name = RewardController::getInstance()->getNameByType(rwd.type);
        des = "";
    }
    m_desName->setString(name);
    m_desLabel->setString(des);
    
    float addH = 0;
    addH = m_desLabel->getContentSize().height * m_desLabel->getOriginScaleY() - 105;
    addH = addH < 0? 0 : addH;
    m_desBg->setPreferredSize(Size(295, 149 + addH));
    m_desBg->setContentSize(Size(295, 149 + addH));
    m_desNode->setPositionY(m_desNode->getPositionY() + addH);
    
    m_desNode->setVisible(true);
}

FreshRechargeIcon* FreshRechargeIcon::create()
{
    auto ret = new FreshRechargeIcon();
    ret->init();
    ret->autorelease();
    return ret;
}

bool FreshRechargeIcon::init()
{
    CCBLoadFile("FreshRechargeIcon", this, this);
    this->setContentSize(Size(90, 90));

    char strJsonFileName[256] = {0};
    char strAtlasFilename[256] = {0};
    sprintf(strJsonFileName, "Spine/UIComponent/jinbi.json");
    sprintf(strAtlasFilename, "UIComponent/UIComponent_1.atlas");
    
    if (FileUtils::sharedFileUtils()->isFileExist(strJsonFileName) && FileUtils::sharedFileUtils()->isFileExist(strAtlasFilename))
    {
        SkeletonAnimation* animation = SkeletonAnimation::createWithFile(strJsonFileName, strAtlasFilename);
        if (animation) {
            m_node1->removeAllChildren();
            m_node1->addChild(animation);
            animation->setToSetupPose();
            animation->setAnimation(0, "putong", true);
            animation->setTag(999);
            animation->setPositionY(-43);
            animation->setPositionX(-5);
            animation->setCompleteListener([&](int trackIndx, int loopCnt){
                if (trackIndx == 1 && loopCnt == 1) {
                    animation->setToSetupPose();
                    animation->setAnimation(0, "putong", true);
                    auto view = FreshRechargeView::create();
                    if (view) {
                        view->setCleanFunction([](){
                            CCLOG("AAA");
                        });
                    }
                    PopupViewController::getInstance()->addPopupInView(view);
                }
            });
        }
    }
    
    auto p = FreshRechargeTip::create();
    if (p)
    {
        m_node2->addChild(p);
    }
    
    return true;
}

bool FreshRechargeIcon::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", Node*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node1", Node*, this->m_node1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node2", Node*, this->m_node2);
    return false;
}

bool FreshRechargeIcon::onTouchBegan(Touch *touch, Event *unused_event)
{
    if (isTouchInside(this, touch) && PopupViewController::getInstance()->getCurrViewCount()==0) {
        m_mainNode->setScale(1.1);
        return true;
    }
    return false;
}
void FreshRechargeIcon::onTouchMoved(Touch *touch, Event *unused_event)
{
    if (!isTouchInside(this, touch)) {
        m_mainNode->setScale(1.0);
    }
}
void FreshRechargeIcon::onTouchEnded(Touch *touch, Event *unused_event)
{
    if (m_mainNode->getScale() > 1.05) {
        m_mainNode->setScale(1);
        auto view = FreshRechargeView::create();
        if (view) {
            view->setCleanFunction([](){
                CCLOG("AAA");
            });
        }
        PopupViewController::getInstance()->addPopupInView(view);
    }
    m_mainNode->setScale(1.0);
}

void FreshRechargeIcon::onEnter()
{
    Node::onEnter();
    setTouchEnabled(true);
    setSwallowsTouches(true);
}
void FreshRechargeIcon::onExit()
{
    Node::onExit();
}

FreshRechargeEnter* FreshRechargeEnter::create()
{
    auto ret = new FreshRechargeEnter();
    ret->init();
    ret->autorelease();
    return ret;
}

bool FreshRechargeEnter::init()
{
    CCBLoadFile("FreshRechargeEnter", this, this);
    CCCommonUtils::setButtonTitle(m_enterBtn, _lang("133005").c_str());
    auto tip = FreshRechargeTip::create();
    if (tip) {
        m_tipNode->addChild(tip);
    }
    return true;
}

void FreshRechargeEnter::refreshLabel(Ref* ref)
{
    m_groupLabel->setString("");
    if (FreshRechargeController::getInstance()->isCanRewardAll()) {
        m_groupLabel->setString(_lang("101063"));
    } else {
        int left = FreshRechargeController::getInstance()->getLeftToNextStage();
        if (left == 0) {
            m_groupLabel->setString(_lang("101063"));
        } else {
            m_groupLabel->setString(_lang_1("101060", CC_CMDITOA(left).c_str()));
        }
    }
}

void FreshRechargeEnter::onEnter()
{
    Node::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FreshRechargeEnter::refreshLabel), MSG_FRESH_RECHARGE_TOTAL_CHANGED, nullptr);
    refreshLabel(nullptr);
}

void FreshRechargeEnter::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRESH_RECHARGE_TOTAL_CHANGED);
    Node::onExit();
}

void FreshRechargeEnter::onClickEnterBtn(Ref*, Control::EventType)
{
    auto view = FreshRechargeView::create();
    if (view)
    {
        view->setCleanFunction([](){
            CCLOG("AAA");
        });
        PopupViewController::getInstance()->addPopupInView(view);
    }
}

bool FreshRechargeEnter::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_enterBtn", ControlButton*, this->m_enterBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_groupLabel", CCLabelIF*, this->m_groupLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipNode", Node*, this->m_tipNode);
    return false;
}

cocos2d::SEL_MenuHandler FreshRechargeEnter::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
    return nullptr;
}
cocos2d::extension::Control::Handler FreshRechargeEnter::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickEnterBtn", FreshRechargeEnter::onClickEnterBtn);
    return nullptr;
}

FreshRechargeTip* FreshRechargeTip::create()
{
    auto ret = new FreshRechargeTip();
    if (ret && ret->init())
    {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool FreshRechargeTip::init()
{
    m_mainNode = Node::create();
    this->addChild(m_mainNode);
    
    auto spr = CCLoadSprite::createSprite("info_green.png");
    spr->setAnchorPoint(Vec2(0.5, 0.5));
    m_mainNode->addChild(spr);
    
    m_label = CCLabelIF::create();
    m_label->setAnchorPoint(Vec2(0.5, 0.5));
    m_label->setScale(0.5);
    m_label->setFontSize(40);
    m_mainNode->addChild(m_label);
    
    CCPoint bagTipPos = m_mainNode->getPosition();
    CCEaseExponentialIn* easeInAction = CCEaseExponentialIn::create(dynamic_cast<CCActionInterval*>(CCMoveTo::create(0.2, ccp(bagTipPos.x,bagTipPos.y+10))));
    CCFiniteTimeAction* easeInActionRevers =CCMoveTo::create(0.2, ccp(bagTipPos.x,bagTipPos.y));
    
    CCEaseExponentialIn* easeInAction1 = CCEaseExponentialIn::create(dynamic_cast<CCActionInterval*>(CCMoveTo::create(0.2, ccp(bagTipPos.x, bagTipPos.y-10))));
    CCMoveTo* easeInAction1Revers = CCMoveTo::create(0.2, ccp(bagTipPos.x, bagTipPos.y));
    CCRepeat* repeatAction = CCRepeat::create(CCSequence::create(
                                                                 easeInAction
                                                                 , easeInActionRevers
                                                                 , easeInAction1
                                                                 ,easeInAction1Revers
                                                                 ,NULL)
                                              , 3);
    CCSequence* ccseq = CCSequence::create(repeatAction,CCDelayTime::create(5),NULL);
    this->m_mainNode->runAction(CCRepeatForever::create(ccseq));
    
    return true;
}

void FreshRechargeTip::onEnter()
{
    Node::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FreshRechargeTip::refresh), MSG_FRESH_RECHARGE_TOTAL_CHANGED, nullptr);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FreshRechargeTip::refresh), FRESH_RECHARGE_REWARD_END, nullptr);
    refresh(nullptr);
}

void FreshRechargeTip::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRESH_RECHARGE_TOTAL_CHANGED);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, FRESH_RECHARGE_REWARD_END);
    Node::onExit();
}

void FreshRechargeTip::refresh(Ref* ref)
{
    int cnt = FreshRechargeController::getInstance()->getCanExchangeCnt();
    m_label->setString(CC_ITOA(cnt));
    if (cnt > 0) {
        m_mainNode->setVisible(true);
    } else {
        m_mainNode->setVisible(false);
    }
}