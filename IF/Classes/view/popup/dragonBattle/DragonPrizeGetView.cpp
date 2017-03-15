//
//  DragonPrizeGetView.cpp
//  IF
//
//  Created by xiaoming on 15/12/15.
//
//

#include "DragonPrizeGetView.hpp"
#include "AllianceActivityRewardView.h"
#include "ActivityController.h"
#include "DragonBattleLeaveCommand.h"
#include "YesNoDialog.h"
#include "GCMRewardController.h"
#include "RewardController.h"
#include "ParticleController.h"

DragonPrizeGetView* DragonPrizeGetView::create(CCDictionary *prizeData)
{
    DragonPrizeGetView* ret = new DragonPrizeGetView();
    if(ret && ret->init(prizeData)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DragonPrizeGetView::init(CCDictionary *prizeData)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(512, true);
    CCLoadSprite::doResourceByCommonIndex(502, true);
    CCLoadSprite::doResourceByCommonIndex(505, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(512, false);
        CCLoadSprite::doResourceByCommonIndex(502, false);
        CCLoadSprite::doResourceByCommonIndex(505, false);

    });
    auto ccb = CCBLoadFile("DragonBattleReward",this,this);
    this->setContentSize(ccb->getContentSize());
    
    CCCommonUtils::setButtonTitle(m_getPrizeBtn, _lang("140205").c_str());
    if (prizeData==NULL) {
        auto* cmd = new DragonComfirmCommand();
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(DragonPrizeGetView::comfirmRewardCallBack), NULL));
        cmd->sendAndRelease();
    }
    else{
        this->getRewardDataBack(prizeData);
    }
    return true;
}


cocos2d::SEL_MenuHandler DragonPrizeGetView::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
    return NULL;
}
cocos2d::extension::Control::Handler DragonPrizeGetView::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGetClick", DragonPrizeGetView::onGetClick);
    return NULL;
}
bool DragonPrizeGetView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_descLabel", CCLabelIF*, this->m_descLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchLayer", CCLayer*, this->m_touchLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_prizeSprite", CCScale9Sprite*, this->m_prizeSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_winNode", CCNode*, this->m_winNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loseNode", CCNode*, this->m_loseNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_winEffectNode", CCNode*, this->m_winEffectNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loseEffectNode", CCNode*, this->m_loseEffectNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getPrizeBtn", CCControlButton*, this->m_getPrizeBtn);
    return false;
}

void DragonPrizeGetView::comfirmRewardCallBack(CCObject *obj)
{
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        return;
    }
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    this->getRewardDataBack(info);
}

void DragonPrizeGetView::getRewardDataBack(CCDictionary *dataDic)
{
    if (dataDic->objectForKey("isWin")) {
        bool isWin = dataDic->valueForKey("isWin")->boolValue();
        if (isWin) {
            m_titleLabel->setString(_lang("140202"));
            this->addWinParticleEffect();
        }
        else{
            m_titleLabel->setString(_lang("140203"));
            this->addLoseParticleEffect();
        }
        m_winNode->setVisible(isWin);
        m_loseNode->setVisible(!isWin);
        
    }
    if (dataDic->objectForKey("reward")) {
        CCArray *dataArr = (CCArray *)dataDic->objectForKey("reward");
        this->setPrizeScrollView(dataArr);
    }
    
    std::string strDesc="";
    
    int score=0;
    int teamRank=0;
    int teamScore=0;
    if (dataDic->objectForKey("score")) {
        score=dataDic->valueForKey("score")->intValue();
    }
    if (dataDic->objectForKey("teamRank")) {
        teamRank=dataDic->valueForKey("teamRank")->intValue();
    }
    if (dataDic->objectForKey("teamScore")) {
        teamScore=dataDic->valueForKey("teamScore")->intValue();
    }
    
    string temp = ActivityController::getInstance()->getTitileByRank(teamRank);
    strDesc=std::string(_lang_4("140204", Value(teamScore).asString().c_str(), Value(teamRank).asString().c_str(), temp.c_str(), Value(score).asString().c_str()));
    m_descLabel->setString(strDesc);
    
}

void DragonPrizeGetView::addWinParticleEffect()
{
    auto effect1 = ParticleController::createParticle("DragonBattleRewards_win_0");
    auto effect2 = ParticleController::createParticle("DragonBattleRewards_win_1");
    m_winEffectNode->addChild(effect1);
    m_winEffectNode->addChild(effect2);
}
void DragonPrizeGetView::addLoseParticleEffect()
{
    auto effect = ParticleController::createParticle("DragonBattleRewards_lost");
    m_loseEffectNode->addChild(effect);
}


void DragonPrizeGetView::setPrizeScrollView(CCArray *dataArr)
{
    CCScrollView *m_scrollView = CCScrollView::create(m_prizeSprite->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_prizeSprite->getParent()->addChild(m_scrollView,100);
    CCNode* scrollLayer = CCNode::create();
    m_scrollView->setContainer(scrollLayer);
    m_scrollView->setPosition(m_prizeSprite->getPosition());
    float rewardHeight = 0;
    float cellHeight = 80;
    float cellWidth = 598;
    for (int i=dataArr->count()-1; i>=0; i--) {
        CCDictionary *dicTemp = (CCDictionary *)dataArr->objectAtIndex(i);
        AllianceActivityRewardCell *cell = AllianceActivityRewardCell::create(dicTemp);
        if (cell==NULL) {
            continue;
        }
        scrollLayer->addChild(cell);
        cell->setPosition(Vec2(5, rewardHeight));
        rewardHeight=rewardHeight+cellHeight;
    }
    scrollLayer->setContentSize(Size(cellWidth, rewardHeight));
    scrollLayer->setPosition(Vec2(0, m_scrollView->getViewSize().height-scrollLayer->getContentSize().height));
    m_scrollView->updateInset();
}

void DragonPrizeGetView::onGetClick(CCObject *pSender, CCControlEvent event)
{
    auto* cmd = new DragonGetCommand();
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(DragonPrizeGetView::getRewardCallBack), NULL));
    cmd->sendAndRelease();
}


void DragonPrizeGetView::getRewardCallBack(CCObject *obj)
{
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        return;
    }
    auto func = [](){
        DragonBattleLeaveCommand* cmd = new DragonBattleLeaveCommand();
        cmd->sendAndRelease();
    };
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    
    if (info->objectForKey("done") && info->valueForKey("done")->boolValue()==true) {
        if(info->objectForKey("reward")){
            auto arr = dynamic_cast<CCArray*>(info->objectForKey("reward"));
            GCMRewardController::getInstance()->retReward2(arr,true);
            this->scheduleOnce(schedule_selector(DragonPrizeGetView::getPrizeAnimeFinish), 0.5);
            
        }
    }
    else{
        YesNoDialog::show(_lang("140207").c_str(), func);
    }
}

void DragonPrizeGetView::getPrizeAnimeFinish(float dt)
{
    DragonBattleLeaveCommand* cmd = new DragonBattleLeaveCommand();
    cmd->sendAndRelease();
}

