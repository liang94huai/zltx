//
//  AllianceScienceClassView.cpp
//  IF
//
//  Created by 邢晓瑞 on 16/1/8.
//
//

#include "AllianceScienceClassView.hpp"
#include "ParticleController.h"
#include "ScienceController.h"
#include "AllianceScienceTreeView.hpp"
#include "PopupViewController.h"

static int validMaxFloor = 0;

bool AllianceScienceClassView::init()
{
    PopupBaseView::init();
    setIsHDPanel(true);
    Size winSize = Director::getInstance()->getWinSize();
    this->setContentSize(winSize);
    CCLoadSprite::doResourceByCommonIndex(4, true);
    setCleanFunction([](){CCLoadSprite::doResourceByCommonIndex(4, false);});
    CCBLoadFile("AllianceScienceClassView", this, this);
    m_hintText->setString(_lang("115840"));
    m_desLabel->setString(_lang("115841"));
    SpriteBatchNode* bgBatch = SpriteBatchNode::createWithTexture(CCLoadSprite::loadResource("UI_UseSkill_picture_blackwhite.png")->getTexture());
    float curH = 0;
    while (curH < winSize.height) {
        auto spr = CCLoadSprite::createSprite("UI_UseSkill_picture_blackwhite.png");
        spr->setAnchorPoint(Vec2(0, 0));
        spr->setPosition(0, curH);
        bgBatch->addChild(spr);
        curH += spr->getContentSize().height;
    }
    m_bgContainer->addChild(bgBatch);
    auto pbatch = ParticleController::createParticleBatch();
    m_fireNode1->getParent()->addChild(pbatch);
    for (int i=1; i<=4; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
        particle->setPosition(m_fireNode1->getPosition());
        pbatch->addChild(particle);
        
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
        particle1->setPosition(m_fireNode2->getPosition());
        pbatch->addChild(particle1);
    }
    
    int totalPoint = ScienceController::getInstance()->getTotalAllScPoints();
    auto it = ScienceController::getInstance()->aScienceFloorMap.begin();
    validMaxFloor = 0;
    for (; it!=ScienceController::getInstance()->aScienceFloorMap.end(); it++) {
        auto& info = ScienceController::getInstance()->allianceScienceMap[(it->second)[0]];
        if (info.lockPoints <= totalPoint) {
            validMaxFloor = it->first;
        } else {
            break;
        }
    }
    return true;
}

void AllianceScienceClassView::onFadeInCallBack()
{
    vector<int> recmendedTabVec;
    auto& map = ScienceController::getInstance()->allianceScienceMap;
    for (auto it = map.begin(); it != map.end(); ++it)
    {
        if (it->second.isRecommended)
        {
            recmendedTabVec.push_back(it->second.tab);
        }
    }
    for (int i = 0; i < 4; ++i)
    {
        if (std::find(recmendedTabVec.begin(), recmendedTabVec.end(), i + 1) != recmendedTabVec.end()) {
            auto particle = ParticleController::createParticle("researching");
            particle->setScale(1.2);
            m_BtnInfo[i].m_crNode->removeAllChildren();
            m_BtnInfo[i].m_crNode->addChild(particle);
        }
        bool hasValidScienceId = isTabValid(i + 1);
        string name[4] = {"115866", "115839", "115868", "115867"};
        string picName[4] = {"item603.png", "temp_icon.png", "ico_account.png", "icon_military.png"};
        if (hasValidScienceId) {
            m_BtnInfo[i].m_nameLabel->setString(_lang(name[i]));
            m_BtnInfo[i].m_nameBG->setVisible(true);
            m_BtnInfo[i].m_iconNode->removeAllChildren();
            m_BtnInfo[i].m_iconNode->addChild(CCLoadSprite::createSprite(picName[i].c_str()));
        } else {
            m_BtnInfo[i].m_nameBG->setVisible(false);
            m_BtnInfo[i].m_nameLabel->setString("");
            auto spr = CCLoadSprite::createSprite("iron_lock.png");
            CCCommonUtils::setSpriteGray(spr, true);
            m_BtnInfo[i].m_iconNode->removeAllChildren();
            m_BtnInfo[i].m_iconNode->addChild(spr);
        }
    }
    
    
    this->getAnimationManager()->setAnimationCompletedCallback(this, nullptr);
    this->getAnimationManager()->runAnimationsForSequenceNamed("Loop");
}

bool AllianceScienceClassView::isTabValid(int i)
{
    if (i <= 0 || i > 4) return false;
    auto& map = ScienceController::getInstance()->allianceScienceMap;
    auto& tabMap = ScienceController::getInstance()->aScienceTabMap;
    auto& vecOfTab = tabMap[i];
    bool hasValidScienceId = false;
    for (auto it = vecOfTab.begin(); it != vecOfTab.end(); ++it) {
        auto& info = map[*it];
        if (info.floor <= validMaxFloor + 1)
        {
            hasValidScienceId = true;
            break;
        }
    }
    return hasValidScienceId;
}

void AllianceScienceClassView::onFadeOutCallBack()
{
    for (int i = 0; i < 4; ++i) {
        m_BtnInfo[i].m_btn->setEnabled(true);
    }
    this->getAnimationManager()->setAnimationCompletedCallback(this, nullptr);
    this->getAnimationManager()->runAnimationsForSequenceNamed("Loop");
    if (m_OpenTab <= 4 && m_OpenTab > 0)
    {
        auto view = AllianceScienceTreeView::create(m_OpenTab);
        view->setCleanFunction([](){});
        PopupViewController::getInstance()->addPopupInView(view);
    }
    m_OpenTab = 0;
}

void AllianceScienceClassView::onClickBtn(int tab)
{
    if (!isTabValid(tab)) {
        CCCommonUtils::flyHint("", "", _lang("115806"));
        return;
    }
    for (int i = 0; i < 4; ++i) {
        m_BtnInfo[i].m_btn->setEnabled(false);
        m_BtnInfo[i].m_crNode->removeAllChildren();
    }
    m_OpenTab = tab;
    this->getAnimationManager()->setAnimationCompletedCallback(this, CC_CALLFUNC_SELECTOR(AllianceScienceClassView::onFadeOutCallBack));
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
}

void AllianceScienceClassView::onEnter()
{
    Node::onEnter();
    for (int i = 0; i < 4; ++i)
    {
        m_BtnInfo[i].m_crNode->removeAllChildren();
    }
    this->getAnimationManager()->setAnimationCompletedCallback(this, CC_CALLFUNC_SELECTOR(AllianceScienceClassView::onFadeInCallBack));
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
    setTitleName(_lang("115838"));
}

void AllianceScienceClassView::onExit()
{
    this->getAnimationManager()->setAnimationCompletedCallback(this, nullptr);
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
    Node::onExit();
}

void AllianceScienceClassView::onBtn1Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    onClickBtn(1);
}
void AllianceScienceClassView::onBtn2Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    onClickBtn(2);
}
void AllianceScienceClassView::onBtn3Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    onClickBtn(3);
}
void AllianceScienceClassView::onBtn4Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    onClickBtn(4);
}

SEL_CCControlHandler AllianceScienceClassView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn1Click", AllianceScienceClassView::onBtn1Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn2Click", AllianceScienceClassView::onBtn2Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn3Click", AllianceScienceClassView::onBtn3Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn4Click", AllianceScienceClassView::onBtn4Click);
    return nullptr;
}
bool AllianceScienceClassView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgContainer", Node*, this->m_bgContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", Node*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintText", CCLabelIF*, this->m_hintText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, this->m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", Node*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", Node*, this->m_fireNode2);
    for (int i = 0; i < _countof(m_BtnInfo); ++i) {
        char szMemberName[128];
        sprintf(szMemberName, "m_node%d", i + 1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, szMemberName, Node*, m_BtnInfo[i].m_node);
        
        sprintf(szMemberName, "m_crNode%d", i + 1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, szMemberName, Node*, m_BtnInfo[i].m_crNode);
        
        sprintf(szMemberName, "m_btn%d", i + 1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, szMemberName, ControlButton*, m_BtnInfo[i].m_btn);
        
        sprintf(szMemberName, "m_icon%dNode", i + 1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, szMemberName, Node*, m_BtnInfo[i].m_iconNode);
        
        sprintf(szMemberName, "m_name%dBG", i +1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, szMemberName, Scale9Sprite*, m_BtnInfo[i].m_nameBG);
        
        sprintf(szMemberName, "m_name%dLabel", i + 1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, szMemberName, CCLabelIF*, m_BtnInfo[i].m_nameLabel);
    }
    
    return false;
}