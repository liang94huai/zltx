//
//  ActivityView.cpp
//  IF
//
//  Created by xxrdsg on 15-6-3.
//
//

#include "ActivityView.h"
#include "LuaController.h"
#include "ActivityExcView.h"
#include "PopupViewController.h"

ActivityView* ActivityView::create(ActivityEventObj * obj)
{
    auto ret = new ActivityView(obj);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ActivityView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    if (m_obj->type != 6) return false;
    CCLoadSprite::doResourceByCommonIndex(500, true);
    CCLoadSprite::doResourceByCommonIndex(502, true);
    CCLoadSprite::doResourceByCommonIndex(506, true);
    
    CCLoadSprite::doResourceByCommonIndex(510, true);
    auto size = CCDirector::sharedDirector()->getWinSize();
    this->setContentSize(CCSize(640, 852));
    if (CCCommonUtils::isIosAndroidPad())
    {
        this->setContentSize(CCSize(1536, 2048));
    }
    CCBLoadFile("ActivityView", this, this);
    if (CCCommonUtils::isIosAndroidPad()) {
        m_bottomNode->setPositionY(m_bottomNode->getPositionY() - (size.height - 2048));
        m_listNode->setPositionY(m_listNode->getPositionY() - (size.height - 2048));
        m_listNode->setContentSize(CCSize(m_listNode->getContentSize().width, m_listNode->getContentSize().height + size.height - 2048));
    } else {
        m_bottomNode->setPositionY(m_bottomNode->getPositionY() - (size.height - 852));
        m_listNode->setPositionY(m_listNode->getPositionY() - (size.height - 852));
        m_listNode->setContentSize(CCSize(m_listNode->getContentSize().width, m_listNode->getContentSize().height + size.height - 852));
    }

    if (m_obj->exchange == "1" || m_obj->exchange == "2") {
        m_bottomNode->setVisible(true);
    } else {
        m_bottomNode->setVisible(false);
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_listNode->setPositionY(m_listNode->getPositionY() - 178);
            m_listNode->setContentSize(CCSize(m_listNode->getContentSize().width, m_listNode->getContentSize().height + 178));
        } else {
            m_listNode->setPositionY(m_listNode->getPositionY() - 92);
            m_listNode->setContentSize(CCSize(m_listNode->getContentSize().width, m_listNode->getContentSize().height + 92));
        }
    }
    m_titleLabel->setString(_lang("150215"));
    CCCommonUtils::setButtonTitle(m_rwdBtn, _lang("133076").c_str());
    Node* node = Node::create();
    CCLabelIF* label = nullptr;
    Sprite* spr = nullptr;
    float height = 0;
    {
        if (!m_obj->rewardIds.empty()) {
            if (m_obj->showDialogs.size() > 0) {
                height -= 20;
                if (CCCommonUtils::isIosAndroidPad())
                {
                    height -= 20;
                }
            }
            for (auto it = m_obj->showDialogs.begin(); it != m_obj->showDialogs.end(); ++it) {
                vector<string> tVec;
                CCCommonUtils::splitString(*it, ";", tVec);
                string showStr("");
                if (tVec.size() == 1)
                {
                    showStr = _lang(tVec[0]);
                } else if (tVec.size() == 2) {
                    showStr = _lang_1(tVec[0].c_str(), (CCCommonUtils::getNameById(tVec[1])).c_str());
                } else if (tVec.size() == 3) {
                    showStr = _lang_2(tVec[0].c_str(), (CCCommonUtils::getNameById(tVec[1])).c_str(), (CCCommonUtils::getNameById(tVec[2])).c_str());
                } else if (tVec.size() == 4) {
                    showStr = _lang_3(tVec[0].c_str(), (CCCommonUtils::getNameById(tVec[1])).c_str(), (CCCommonUtils::getNameById(tVec[2])).c_str(), (CCCommonUtils::getNameById(tVec[3])).c_str());
                }
                label = CCLabelIF::create(showStr.c_str());
                label->setFontSize(20);
                if (CCCommonUtils::isIosAndroidPad())
                {
                    label->setFontSize(40);
                }
                label->setColor(ccBLACK);
                label->setAnchorPoint(Vec2(0.5, 1));
                label->setDimensions(Size(580, 0));
                if (CCCommonUtils::isIosAndroidPad())
                {
                    label->setDimensions(Size(1300, 0));
                }
                label->setHorizontalAlignment(TextHAlignment::CENTER);
                label->setVerticalAlignment(TextVAlignment::TOP);
                label->setPosition(Vec2(0, height));
                node->addChild(label);
                height -= label->getContentSize().height * label->getOriginScaleY();
            }
            height -= 5;
            if (CCCommonUtils::isIosAndroidPad())
            {
                height -= 5;
            }
            //有几率获得
            label = CCLabelIF::create(_lang("150217").c_str());
            label->setFontSize(18);
            if (CCCommonUtils::isIosAndroidPad())
            {
                label->setFontSize(36);
            }
            label->setAnchorPoint(Vec2(0.5, 1));
            label->setPosition(Vec2(0, height));
            label->setColor({139, 29, 20});
            label->setHorizontalAlignment(TextHAlignment::CENTER);
            label->setVerticalAlignment(TextVAlignment::TOP);
            node->addChild(label);
            height -= label->getContentSize().height * label->getOriginScaleY();
            
            //展示奖励
            int cnt = m_obj->rewardIds.size();
            int hang = ceil(cnt / 6.0);
            for (int i = 1; i <= hang; ++i) {
                if (i < hang) {
                    for (int j = 1; j <= 6; ++j) {
                        ActivityRwdShowCell* cell = ActivityRwdShowCell::create(m_obj->rewardIds.at(j - 1 + (i - 1) * 6));
                        cell->setPositionX(- 80 * 6 / 2 + 80 / 2 + (j - 1) * 80);
                        cell->setPositionY(height - 40);
                        if (CCCommonUtils::isIosAndroidPad()) {
                            cell->setPositionX(- 160 * 6 / 2 + 160 / 2 + (j - 1) * 160);
                            cell->setPositionY(height - 80);
                        }
                        node->addChild(cell);
                    }
                    height -= 80;
                    if (CCCommonUtils::isIosAndroidPad())
                    {
                        height -= 80;
                    }
                } else {
                    int leftCnt = cnt - (i - 1) * 6;
                    for (int j = 1; j <= leftCnt; ++j) {
                        ActivityRwdShowCell* cell = ActivityRwdShowCell::create(m_obj->rewardIds.at((i - 1) * 6 + j - 1));
                        cell->setPositionX(- (80 * leftCnt) / 2 + 80 / 2 + (j - 1) * 80);
                        cell->setPositionY(height - 40);
                        if (CCCommonUtils::isIosAndroidPad()) {
                            cell->setPositionX(- 160 * leftCnt / 2 + 160 / 2 + (j - 1) * 160);
                            cell->setPositionY(height - 80);
                        }
                        node->addChild(cell);
                    }
                    height -= 80;
                    if (CCCommonUtils::isIosAndroidPad())
                    {
                        height -= 80;
                    }
                }
            }
            //空
            height -= 10;
            if (CCCommonUtils::isIosAndroidPad()) {
                height -= 10;
            }
            //横线
            spr = CCLoadSprite::createSprite("Items_tips3_lottery.png");
            spr->setAnchorPoint(Vec2(0.5, 0.5));
            spr->setPosition(0, height);
            node->addChild(spr);
            if (CCCommonUtils::isIosAndroidPad())
            {
                spr->setScale(2);
            }
            //空
            height -= 10;
            if (CCCommonUtils::isIosAndroidPad()) {
                height -= 10;
            }
            //剧情简介
            label = CCLabelIF::create(_lang("150216").c_str());
            label->setAnchorPoint(Vec2(0.5, 1));
            label->setColor(ccBLACK);
            label->setFontSize(20);
            if (CCCommonUtils::isIosAndroidPad()) {
                label->setFontSize(40);
            }
            label->setPosition(0, height);
            label->setHorizontalAlignment(TextHAlignment::CENTER);
            label->setVerticalAlignment(TextVAlignment::TOP);
            node->addChild(label);
            height -= label->getContentSize().height * label->getOriginScaleY();
            //空
            height -= 10;
            if (CCCommonUtils::isIosAndroidPad()) {
                height -= 10;
            }
        }
        //正文
        string mStory = _lang(m_obj->story);
        mStory.append("\n\n\n");
        label = CCLabelIF::create(mStory.c_str());
        label->setFontSize(20);
        if (CCCommonUtils::isIosAndroidPad()) {
            label->setFontSize(40);
        }
        label->setDimensions(Size(580, 0));
        if (CCCommonUtils::isIosAndroidPad()) {
            label->setDimensions(Size(1300, 0));
        }
        label->setAnchorPoint(Vec2(0.5, 1));
        label->setColor({85, 62, 31});
        label->setPosition(0, height);
        label->setHorizontalAlignment(TextHAlignment::CENTER);
        label->setVerticalAlignment(TextVAlignment::TOP);
        node->addChild(label);
        height -= label->getContentSize().height * label->getOriginScaleY();
    }
    ScrollView* view = ScrollView::create(m_listNode->getContentSize());
    if (abs(height) < m_listNode->getContentSize().height) {
        node->setPositionY(m_listNode->getContentSize().height / 2.0 + abs(height) / 2.0);
        height = -m_listNode->getContentSize().height;
    } else {
        node->setPositionY(abs(height));
    }
    view->setContentSize(Size(m_listNode->getContentSize().width, abs(height)));
    view->addChild(node);
    node->setPositionX(view->getContentSize().width / 2);
//    node->setPositionY(abs(height));
    view->setContentOffset(Vec2(0, m_listNode->getContentSize().height - view->getContentSize().height));
    view->setDirection(ScrollView::Direction::VERTICAL);
    m_listNode->addChild(view);
    view->setDelegate(this);
    
    bool st = false;
    if (LuaController::getInstance()->checkActLuaValid(m_obj))
    {
        CCNode* node = LuaController::getInstance()->createActivityAdCell(m_obj);
        if (node && node->getChildrenCount()) {
            m_picNode->addChild(node);
            if (CCCommonUtils::isIosAndroidPad())
            {
                float w = (1536.0 - 1156.0) / 2.0;
                float h = 645;
                node->setContentSize(CCSize(0, 0));
                node->setScaleX(1156.0 / 640.0);
                node->setScaleY(645.0 / 357.0);
                auto layer1 = CCModelLayerColor::create();
                layer1->setAnchorPoint(ccp(0, 0));
                layer1->setContentSize(CCSize(w, h));
                layer1->setPosition(-w, 0);
                layer1->setOpacity(255);
                layer1->setColor(ccBLACK);
//                layer1->setTouchPriority(-999);
                m_picNode->addChild(layer1);
                
                layer1 = CCModelLayerColor::create();
                layer1->setAnchorPoint(CCPointZero);
                layer1->setPosition(ccp(1156, 0));
                layer1->setContentSize(CCSize(w, h));
                layer1->setOpacity(255);
                layer1->setColor(ccBLACK);
//                layer1->setTouchPriority(-999);
                m_picNode->addChild(layer1);
            }
        } else {
            st = true;
        }
    }
    else
    {
        st = true;
    }
    
    if (st) {
        auto nameTTF = CCLabelIF::create(m_obj->name.c_str());
        auto infoTTF = CCLabelIF::create(m_obj->desc.c_str());
        auto bgImg = CCLoadSprite::createSprite("activity_ad_beiyong.png");
        
        nameTTF->setPosition(ccp(16,245));
        nameTTF->setAnchorPoint(ccp(0, 1));
        nameTTF->setFontSize(36);
        nameTTF->setColor({255,219,117});
        nameTTF->setHorizontalAlignment(kCCTextAlignmentLeft);
        nameTTF->setVerticalAlignment(kCCVerticalTextAlignmentTop);
        nameTTF->setDimensions(CCSize(0, 0));
        
        infoTTF->setPosition(ccp(16,201));
        infoTTF->setAnchorPoint(ccp(0, 1));
        infoTTF->setColor({0, 249, 0});
        infoTTF->setHorizontalAlignment(kCCTextAlignmentLeft);
        infoTTF->setVerticalAlignment(kCCVerticalTextAlignmentTop);
        infoTTF->setDimensions(CCSize(300, 0));
        
        
        bgImg->setAnchorPoint(ccp(0, 0));
        bgImg->setPosition(ccp(0, 0));
        auto node = CCNode::create();
        node->addChild(bgImg);
        node->addChild(nameTTF);
        node->addChild(infoTTF);
        m_picNode->addChild(node);
        if (CCCommonUtils::isIosAndroidPad()) {
            float w = (1536.0 - 1156.0) / 2.0;
            float h = 645;
            node->setContentSize(CCSize(0, 0));
            node->setScaleX(1156.0 / 640.0);
            node->setScaleY(645.0 / 357.0);
            auto layer1 = CCModelLayerColor::create();
            layer1->setAnchorPoint(ccp(0, 0));
            layer1->setContentSize(CCSize(w, h));
            layer1->setPosition(-w, 0);
            layer1->setOpacity(255);
            layer1->setColor(ccBLACK);
            //                layer1->setTouchPriority(-999);
            m_picNode->addChild(layer1);
            
            layer1 = CCModelLayerColor::create();
            layer1->setAnchorPoint(CCPointZero);
            layer1->setPosition(ccp(1156, 0));
            layer1->setContentSize(CCSize(w, h));
            layer1->setOpacity(255);
            layer1->setColor(ccBLACK);
            //                layer1->setTouchPriority(-999);
            m_picNode->addChild(layer1);
        }
    }
    
    if (m_obj->exchange == "2") {
        if (ActivityController::getInstance()->m_exc2Data.find(m_obj->id) == ActivityController::getInstance()->m_exc2Data.end()) {
            ActivityController::getInstance()->startGetAct6Data();
        } else {
            if (!ActivityController::getInstance()->checkIsToday(ActivityController::getInstance()->m_exc2RefInfo[m_obj->id].m_refreshTime)) {
                ActivityController::getInstance()->m_exc2RefInfo[m_obj->id].m_refreshTime = GlobalData::shared()->getTimeStamp();
                ActivityController::getInstance()->m_exc2RefInfo[m_obj->id].m_refresh = 0;
            }
        }
    } else if (m_obj->exchange == "1") {
        if (!ActivityController::getInstance()->isAct1DataInit(m_obj->id)) {
            ActivityController::getInstance()->startGetAct6Data();
        } else {
            ActivityController::getInstance()->checkAct1ReFreshTimes(m_obj->id);
//            if (ActivityController::getInstance()->isAct1DataShouldRefresh(m_obj->id))
//            {
//                ActivityController::getInstance()->startAct6Refresh(m_obj->id);
//            }
        }
    }

    return true;
}

void ActivityView::scrollViewDidScroll(ScrollView* view)
{
    float mindy = view->minContainerOffset().y ;
    float maxdy = view->maxContainerOffset().y ;
    float dy = view->getContentOffset().y;
    if (dy < mindy)
    {
        view->setContentOffset(ccp(0, mindy));
    }
    if (dy > maxdy) {
        view->setContentOffset(ccp(0, maxdy));
    }
}

void ActivityView::onEnter()
{
    CCNode::onEnter();
    setTitleName("");
}

void ActivityView::onExit()
{
    CCLoadSprite::doResourceByCommonIndex(500, true);
    CCLoadSprite::doResourceByCommonIndex(502, true);
    CCNode::onExit();
}

bool ActivityView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCLayerColor*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCLayerColor*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdBtn", CCControlButton*, this->m_rwdBtn);
    return false;
}

SEL_CCControlHandler ActivityView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickRwdBtn", ActivityView::onClickRwdBtn);
    return NULL;
}

void ActivityView::onClickRwdBtn(CCObject *pSender, CCControlEvent event)
{
//    if (m_obj->exchange != "1" && m_obj->exchange != "2") {
//        return;
//    }
    if (m_obj->exchange == "2") {
        PopupViewController::getInstance()->addPopupView(ActivityExcNewView::create(m_obj));
    } else if (m_obj->exchange == "1") {
        PopupViewController::getInstance()->addPopupInView(ActivityExcView::create(m_obj));
    }
}


//////////
ActivityRwdShowCell* ActivityRwdShowCell::create(string itemId)
{
    auto ret = new ActivityRwdShowCell();
    if (ret && ret->init(itemId))
    {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ActivityRwdShowCell::init(string itemId)
{
    Sprite* bg = CCLoadSprite::createSprite("icon_kuang.png");
    CCCommonUtils::setSpriteMaxSize(bg, 75);
    this->addChild(bg);
//    Sprite* icon = CCLoadSprite::createSprite(CCCommonUtils::getIcon(itemId).c_str(),true,CCLoadSpriteType_GOODS);
    CCCommonUtils::createGoodsIcon(atoi(itemId.c_str()), this, Size(68, 68));
    if (CCCommonUtils::isIosAndroidPad())
    {
        this->setScale(2.0);
    }
    return true;
}
