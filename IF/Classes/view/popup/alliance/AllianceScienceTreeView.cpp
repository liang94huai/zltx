//
//  AllianceScienceTreeView.cpp
//  IF
//
//  Created by 邢晓瑞 on 16/1/8.
//
//

#include "AllianceScienceTreeView.hpp"
#include "ScienceController.h"
#include "ParticleController.h"
#include "AllianceScienceDonateView.h"
#include "PopupViewController.h"

static int validMaxFloor = 1;
static const int celldw = 80;
static const int celldh = 160;

AllianceScienceTreeView* AllianceScienceTreeView::create(int tab)
{
    auto ret = new AllianceScienceTreeView(tab);
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
bool AllianceScienceTreeView::init()
{
    if (m_tab > 4 || m_tab <= 0) {
        return false;
    }
    PopupBaseView::init();
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(4, true);
    setCleanFunction([](){CCLoadSprite::doResourceByCommonIndex(4, false);});
    setContentSize(Size(640, 852));
    if (CCCommonUtils::isIosAndroidPad())
    {
        setContentSize(Size(1536, 2048));
    }
    CCBLoadFile("AllianceScienceTreeView", this, this);
    Size winSize = Director::getInstance()->getWinSize();
    float add = winSize.height - 852;
    if (CCCommonUtils::isIosAndroidPad())
    {
        add = winSize.height - 2048;
        add = add / 2.4;
    }
    m_bottomNode->setPositionY(m_bottomNode->getPositionY() - add);
    m_bgContainer->setPositionY(m_bgContainer->getPositionY() - add);
    m_infoList->setPositionY(m_infoList->getPositionY() - add);
    m_infoList->setContentSize(Size(m_infoList->getContentSize().width, m_infoList->getContentSize().height + add));
    float height = 0;
    SpriteBatchNode* batch = SpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("technology_09.png")->getTexture());
    while (height < winSize.height) {
        auto sprite = CCLoadSprite::createSprite("technology_09.png");
        sprite->setAnchorPoint(Vec2(0, 0));
        sprite->setPositionX(0);
        sprite->setPositionY(height);
        batch->addChild(sprite);
        height += sprite->getContentSize().height;
    }
    m_bgContainer->addChild(batch);
    
    ParticleBatchNode* pBatch = ParticleBatchNode::createWithTexture(CCLoadSprite::loadResource("Effect_0016.png")->getTexture());
    m_fireNode1->getParent()->addChild(pBatch);
    for (int i=1; i<=4; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
        particle->setPosition(m_fireNode1->getPosition());
        pBatch->addChild(particle);
        
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
        particle1->setPosition(m_fireNode2->getPosition());
        pBatch->addChild(particle1);
    }
    
    m_desLabel->setString(_lang("115841"));
    
    Layer* layer = Layer::create();
    layer->setTouchEnabled(true);
    layer->setSwallowsTouches(true);
    layer->setContentSize(Size(640, 100));
    layer->setAnchorPoint(Vec2(0, 0));
    layer->setPosition(Vec2(0, -101));
    Node* node = Node::create();
    m_infoList->getParent()->addChild(node, 1);
    node->setPosition(m_infoList->getPosition());
    node->addChild(layer);
    
    m_scrollView = ScrollView::create(m_infoList->getContentSize());
    m_scrollView->setDirection(ScrollView::Direction::VERTICAL);
    m_scrollView->setContentSize(m_infoList->getContentSize());
    m_scrollView->setAnchorPoint(Vec2(0, 0));
    m_infoList->addChild(m_scrollView);
    
    m_mainNode = Node::create();
    m_scrollView->addChild(m_mainNode);
    m_lineBatch = SpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("kji_line.png")->getTexture());
    m_scrollView->addChild(m_lineBatch, -1);
    m_mainNode->setPositionX(m_scrollView->getContentSize().width * 0.5);
    m_lineBatch->setPositionX(m_scrollView->getContentSize().width * 0.5);
    
    int totalPoint = ScienceController::getInstance()->getTotalAllScPoints();
    auto it = ScienceController::getInstance()->aScienceFloorMap.begin();
    for (; it!=ScienceController::getInstance()->aScienceFloorMap.end(); it++) {
        auto& info = ScienceController::getInstance()->allianceScienceMap[(it->second)[0]];
        if (info.lockPoints <= totalPoint) {
            validMaxFloor = it->first;
        } else {
            break;
        }
    }
    
//    refreshView();
    
    return true;
}

void AllianceScienceTreeView::refreshView()
{
    auto addLineFun = [=](Vec2 sp, Vec2 ep){
        Vec2 dv = ep - sp;
        float dx = dv.x * celldw;
        float dy = -dv.y * celldh;
        Vec2 v1(dx, dy);
        float len = v1.length();
        Vec2 startP = Vec2((sp.x - 4) * celldw, -(sp.y - 1) *  celldh);
        double angle = atan2(dy, dx) * 180 / PI;
        
        auto spr = CCLoadSprite::createSprite("kji_line.png");
        float scaley = len / spr->getContentSize().height;
        spr->setAnchorPoint(Vec2(0.5, 0));
        spr->setPosition(startP);
        spr->setRotation(90 - angle);
        spr->setScaleY(scaley);
        m_lineBatch->addChild(spr);
    };
    m_mainNode->removeAllChildren();
    m_lineBatch->removeAllChildren();
    auto& vec = ScienceController::getInstance()->aScienceTabMap[m_tab];
    auto& aSMap = ScienceController::getInstance()->allianceScienceMap;
    int maxYLineNum = 0;
    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        if (aSMap.find(*it) == aSMap.end())
        {
            continue;
        }
        auto& info = aSMap[*it];
        auto cell = AllianceScienceTreeCell::create(*it);
        if (!cell)
        {
            continue;
        }
        maxYLineNum = maxYLineNum > info.posY? maxYLineNum : info.posY;
        float posX = (info.posX - 4) * celldw;
        float posY = -(info.posY - 1) * celldh;
        cell->setZOrder(1);
        cell->setPosition(posX, posY);
        m_mainNode->addChild(cell);
        string relation = info.relation;
        vector<string> vec;
        CCCommonUtils::splitString(relation, "|", vec);
        for (auto it1 = vec.begin(); it1 != vec.end(); ++it1)
        {
            auto& locationMap = ScienceController::getInstance()->aScienLocationMap;
            if (locationMap[m_tab].find(*it1) == locationMap[m_tab].end()) {
                continue;
            }
            auto tInfo = aSMap[locationMap[m_tab][*it1]];
            //全部科技都显示
//            if (tInfo.floor > validMaxFloor + 1) {
//                continue;
//            }
            vector<string> vec1;
            CCCommonUtils::splitString(*it1, ";", vec1);
            if (vec1.size() <= 1) {
                continue;
            }
            int nPosX = atoi(vec1[1].c_str());
            int nPosY = atoi(vec1[0].c_str());
            addLineFun(Vec2(info.posX, info.posY), Vec2(nPosX, nPosY));
        }
    }
    float totalH = (maxYLineNum + 0.3) * celldh;
    m_scrollView->setContentSize(Size(m_infoList->getContentSize().width, totalH));
    m_scrollView->setContentOffset(Vec2(0, m_infoList->getContentSize().height - totalH));
    m_mainNode->setPositionY(totalH - celldh * 0.65);
    m_lineBatch->setPositionY(m_mainNode->getPositionY());
}

void AllianceScienceTreeView::onEnter()
{
    Node::onEnter();
    setTitleName(_lang("115838"));
    refreshView();
}
void AllianceScienceTreeView::onExit()
{
    Node::onExit();
}


SEL_CCControlHandler AllianceScienceTreeView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    
    return nullptr;
}
bool AllianceScienceTreeView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgContainer", Node*, this->m_bgContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", Node*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", Node*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, this->m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", Node*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", Node*, this->m_fireNode2);
    return false;
}


AllianceScienceTreeCell* AllianceScienceTreeCell::create(int itemId)
{
    auto ret = new AllianceScienceTreeCell(itemId);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
bool AllianceScienceTreeCell::init()
{
    this->setContentSize(Size(0, 0));
//    auto& info = ScienceController::getInstance()->allianceScienceMap[m_id];
    //没有隐藏的科技
//    if (info.floor > validMaxFloor + 1)
//        return false;
    CCBLoadFile("AllianceScienceTreeCell", this, this);
    refresh();
    return true;
}

void AllianceScienceTreeCell::refresh(){
    m_particleNode->removeAllChildren();
    m_bottomNode->setVisible(true);
    CCCommonUtils::setSpriteGray(m_bg, false);
    m_lockSpr->setVisible(false);
    m_newIcon->setVisible(false);
    m_timershow->setVisible(false);
    m_timecntslabel->setString("");
    auto& info = ScienceController::getInstance()->allianceScienceMap[m_id];
    string picName = CCCommonUtils::getIcon(CC_ITOA(m_id));
    m_picNode->removeAllChildren();
    auto pic = CCLoadSprite::createSprite(picName.c_str());
    CCCommonUtils::setSpriteMaxSize(pic, 120, true);
    string lvstr = CC_ITOA(info.level);
    lvstr += "/";
    lvstr += CC_ITOA(info.maxLv);
    m_lvLabel->setString(lvstr);
    m_nameLabel->setString(_lang(info.name));
    if (info.floor > validMaxFloor + 1) {
        pic = CCLoadSprite::createSprite("iron_lock.png");
//        CCCommonUtils::setSpriteMaxSize(pic, 120, true);
        CCCommonUtils::setSpriteGray(pic, true);
        CCCommonUtils::setSpriteGray(m_bg, true);
        m_bottomNode->setVisible(false);
        m_lvLabel->setString("");
        m_nameLabel->setString("");
    } else if (info.floor == validMaxFloor + 1) {
        CCCommonUtils::setSpriteGray(pic, true);
        CCCommonUtils::setSpriteGray(m_bg, true);
    } else {
        if (info.isRecommended) {
            m_timecntslabel->setString(_lang("115862"));
            m_timershow->setVisible(true);
            auto batch = ParticleBatchNode::createWithTexture(CCLoadSprite::loadResource("Effect_0096.png")->getTexture());
            batch->addChild(ParticleController::createParticle("researching"));
            m_particleNode->addChild(batch);
        } else {
            m_timecntslabel->setString("");
            m_timershow->setVisible(false);
        }
    }
    m_picNode->addChild(pic);
}

bool AllianceScienceTreeCell::onTouchBegan(Touch *touch, Event *unused_event)
{
    if (isTouchInside(m_mainNode, touch)) {
        return true;
    }
    return false;
}
void AllianceScienceTreeCell::onTouchMoved(Touch *touch, Event *unused_event)
{
    
}
void AllianceScienceTreeCell::onTouchEnded(Touch *touch, Event *unused_event)
{
    if (touch->getStartLocation().distance(touch->getLocation()) < 10) {
        auto& info = ScienceController::getInstance()->allianceScienceMap[m_id];
        if (info.floor <= validMaxFloor)
        {
            auto view = AllianceScienceDonateView::create(m_id);
            view->setFromTreeCell(this);
            PopupViewController::getInstance()->addPopupView(view);
        } else if (info.floor >= validMaxFloor + 1) {
            CCCommonUtils::flyHint("", "", _lang("115806"));
        }
    }
}

void AllianceScienceTreeCell::onEnter()
{
    Node::onEnter();
    setTouchEnabled(true);
    setSwallowsTouches(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, CC_CALLFUNCO_SELECTOR(AllianceScienceTreeCell::onGetMsgRecSciEnd), "msg.recommend.al.science", nullptr);
}
void AllianceScienceTreeCell::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "msg.recommend.al.science");
    Node::onExit();
}

void AllianceScienceTreeCell::onGetMsgRecSciEnd(Ref* ref)
{
    auto dic = _dict(ref);
    if (!dic->objectForKey("m_scienceId")) {
        return;
    }
    __Integer* cint = dynamic_cast<__Integer*>(dic->objectForKey("m_scienceId"));
    if (!cint) {
        return;
    }
    int scienceId = cint->getValue();
    if (scienceId != m_id) {
        return;
    }
    refresh();
}

bool AllianceScienceTreeCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode", Node*, this->m_particleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_completePar", Node*, this->m_completePar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", Node*, this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvLabel", CCLabelIF*, this->m_lvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", Node*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockSpr", Sprite*, this->m_lockSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newIcon", Sprite*, this->m_newIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timershow", Sprite*, this->m_timershow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timecntslabel", CCLabelIF*, this->m_timecntslabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", Node*, this->m_bottomNode);
    return false;
}
