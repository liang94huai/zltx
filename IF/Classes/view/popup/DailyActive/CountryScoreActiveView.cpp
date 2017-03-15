//
//  CountryScoreActiveView.cpp
//  IF
//
//  Created by 付彬 on 15/12/28.
//
//

#include "CountryScoreActiveView.hpp"
#include "ActivityController.h"
#include "FunBuildController.h"
#include "TipsView.h"
#include "PopupViewController.h"
#include "ChestUseView.h"
#include "ParticleController.h"
#include "GuideController.h"
#include "UIComponent.h"

#include "ToolController.h"
#include "RewardController.h"
#include "SingleScoreRankView.hpp"
#include "JoinAllianceView.h"

CountryScoreActiveView* CountryScoreActiveView::create(ActivityEventObj* info, bool isInner)
{
    auto ret = new CountryScoreActiveView();
    if (ret && ret->init(info, isInner)) {
        ret->autorelease();
    } else {
        CC_SAFE_RELEASE(ret);
    }
    return ret;
}

bool CountryScoreActiveView::init(ActivityEventObj* info, bool isInner)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByCommonIndex(208, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(7, false);
        CCLoadSprite::doResourceByCommonIndex(208, false);
    });
    
    m_actEventObj = info;
    auto tmpCCB = CCBLoadFile("SingleScoreActiveView3", this, this);
    setContentSize(tmpCCB->getContentSize());
    
    setTitleName( m_actEventObj->name );
    m_subTitleLabel->setString(_lang("150293"));
    m_scoreTLabel->setString(_lang_1("150364", ""));
    m_cup1Label->setString(_lang("150363")+":");
    m_cup2Label->setString(_lang("150290"));
    m_cup3Label->setString(_lang("150291"));
    if (ActivityController::getInstance()->KingAlScoreRank > 0)
    {
        m_cup2Label->setString( CC_ITOA(ActivityController::getInstance()->KingAlScoreRank) );
    }else if (ActivityController::getInstance()->KingAlScoreRank == 0)
    {
        m_cup2Label->setString( "5000+" );
    }
    m_cup2Label->setPositionX(m_cup1Label->getPositionX()+m_cup1Label->getContentSize().width*m_cup1Label->getOriginScaleX()+5);
    
    m_sub2TitleLabel->setString(_lang("150433"));
    m_msgLabel->setString(_lang("150431"));
    
    changeBGHeight(m_viewBg);
    float extH = getExtendHeight();
    
    auto tbg = CCLoadSprite::loadResource("technology_09.png");
    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
    int maxHight = CCDirector::sharedDirector()->getWinSize().height;
    int curHight = 0;
    while (abs(curHight)<maxHight) {
        auto bg = CCLoadSprite::createSprite("technology_09.png");
        bg->setAnchorPoint(ccp(0, 1));
        bg->setPosition(ccp(0, curHight));
        curHight -= bg->getContentSize().height;
        tBatchNode->addChild(bg);
    }
    m_bgNode->addChild(tBatchNode);
    
    int innerH = 0;
    if (!isInner) {
        m_infoList->setPositionY(m_infoList->getPositionY()-extH);
    }else {
        innerH = 80;
    }
    m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height+extH-innerH));
    m_scrollView = CCScrollView::create(m_infoList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_infoList->addChild(m_scrollView);
    m_scrollView->setTouchPriority(Touch_Popup);
    
    m_actAlInfo = NULL;
    m_actPlInfo = NULL;
    m_waitInterface = NULL;
    m_guideKey = "";
    
    return true;
}

void CountryScoreActiveView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    
    setTitleName(_lang("150427"));
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CountryScoreActiveView::retData), MSG_FRESH_SINGLE_SCORE_VIEW, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CountryScoreActiveView::retRankData), MSG_FRESH_SINGLE_SCORE_RANK_VIEW, NULL);
    
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(CountryScoreActiveView::onGameTick), this, 1,kCCRepeatForever, 0.0f, false);
    onGameTick(0);
    
    if (ActivityController::getInstance()->m_kingScoreRwdMap.size() <= 0) {
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_infoList);
        m_waitInterface->setPosition(ccp(m_infoList->getContentSize().width/2, m_infoList->getContentSize().height/2));
        ActivityController::getInstance()->getSingleScoreData(m_actEventObj->activityid);
    }else {
        retData(NULL);
    }
}

void CountryScoreActiveView::onExit()
{
    if (m_waitInterface != NULL) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRESH_SINGLE_SCORE_VIEW);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRESH_SINGLE_SCORE_RANK_VIEW);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(CountryScoreActiveView::onGameTick), this);
    setTouchEnabled(false);
    CCNode::onExit();
}

void CountryScoreActiveView::retDataS(CCObject* param)
{
    if (m_actPlInfo == NULL) {
        return;
    }
    m_scoreTLabelS->setString(_lang_1("150292", ""));
    m_cup1LabelS->setString(_lang("150289")+":");
    m_cup2LabelS->setString(_lang("150290"));
    m_cup3LabelS->setString(_lang("150291"));
    if (ActivityController::getInstance()->KingPlScoreRank > 0)
    {
        m_cup2LabelS->setString( CC_ITOA(ActivityController::getInstance()->KingPlScoreRank) );
    }else if (ActivityController::getInstance()->KingPlScoreRank == 0)
    {
        m_cup2LabelS->setString( "5000+" );
    }
    m_cup2LabelS->setPositionX(m_cup1LabelS->getPositionX()+m_cup1LabelS->getContentSize().width*m_cup1LabelS->getOriginScaleX()+5);
    
    m_sub2TitleLabelS->setString(_lang("150432"));
    
    ///
    float pro = 0;
    m_pt1LabelS->setString("0");
    m_scoreLabelS->setString( CC_CMDITOA(m_actPlInfo->currentScore));
    m_scoreLabelS->setPositionX( m_scoreTLabelS->getPositionX()+m_scoreTLabelS->getContentSize().width*m_scoreTLabelS->getOriginScaleX() + 5 );
    m_scoreIconS->setPositionX(m_scoreLabelS->getPositionX()+m_scoreLabelS->getContentSize().width*m_scoreLabelS->getOriginScaleX()+5);
    
    int lastNum = 0;
    for (int i=0; i<m_actPlInfo->m_tagVec.size(); i++)
    {
        int curTagScore = m_actPlInfo->m_tagVec[i];
        double tmpScore = curTagScore;
        if (i==0)
        {
            m_pt2LabelS->setString(CC_CMDITOA(tmpScore));
            if (curTagScore > m_actPlInfo->currentScore){
                CCCommonUtils::setSpriteGray(m_box1OpenIconS, false);
                if (m_actPlInfo->currentScore >= lastNum) {
                    pro += 0.33333*(m_actPlInfo->currentScore-lastNum)/(curTagScore-lastNum);
                }
            }else {
                CCCommonUtils::setSpriteGray(m_box1OpenIconS, true);
                lastNum = curTagScore;
                pro = 0.33333;
            }
        }
        else if (i==1)
        {
            m_pt3LabelS->setString(CC_CMDITOA(tmpScore));
            if (curTagScore > m_actPlInfo->currentScore){
                CCCommonUtils::setSpriteGray(m_box2OpenIconS, false);
                if (m_actPlInfo->currentScore >= lastNum) {
                    pro += 0.33333*(m_actPlInfo->currentScore-lastNum)/(curTagScore-lastNum);
                }
            }else {
                CCCommonUtils::setSpriteGray(m_box2OpenIconS, true);
                lastNum = curTagScore;
                pro = 0.66666;
            }
        }
        else if (i==2)
        {
            m_pt4LabelS->setString(CC_CMDITOA(tmpScore));
            if (curTagScore > m_actPlInfo->currentScore){
                CCCommonUtils::setSpriteGray(m_box3OpenIconS, false);
                if (m_actPlInfo->currentScore >= lastNum) {
                    pro += 0.33333*(m_actPlInfo->currentScore-lastNum)/(curTagScore-lastNum);
                }
            }else {
                CCCommonUtils::setSpriteGray(m_box3OpenIconS, true);
                pro = 1.0;
            }
        }
    }
    
    pro = MIN(pro, 1);
    pro = MAX(pro, 0);
    m_barS->setScaleX(pro);
    m_bar2S->setScaleX(pro);
    if (pro >= 1.0) {
        m_bar2S->setVisible(true);
        m_barS->setVisible(false);
    }else {
        m_bar2S->setVisible(false);
        m_barS->setVisible(true);
    }
    m_arrowSprS->setPositionX( m_barS->getPositionX() + m_barS->getContentSize().width*pro );
    
}

void CountryScoreActiveView::retData(CCObject* param)
{
    map<string, ActivityInfo*>::iterator it = ActivityController::getInstance()->m_kingScoreRwdMap.begin();
    for (; it != ActivityController::getInstance()->m_kingScoreRwdMap.end(); it++)
    {
        if (it->second->type == 4 && m_actAlInfo==NULL) {
            m_actAlInfo = it->second;
        }
        if (it->second->type == 3 && m_actPlInfo==NULL) {
            m_actPlInfo = it->second;
        }
    }
    if (m_actAlInfo == NULL && m_actPlInfo == NULL) {
        return;
    }
    
    if (m_waitInterface != NULL) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    
    m_vsNode->setVisible(false);
    CCArray* vsCountry = ActivityController::getInstance()->m_vsCountry;
//    if (ActivityController::getInstance()->m_historyKingRankMap.size()>0) {
//        vsCountry = ActivityController::getInstance()->m_historyKingRankMap.begin()->second;
//    }
    if (vsCountry && vsCountry->count()>=2)
    {
        m_kingLNode->removeAllChildren();
        m_kingRNode->removeAllChildren();
        m_vsLabel->setFntFile("pve_fnt_title.fnt");
        m_vsLabel->setString("VS");
        CCDictionary* lcountryDict = dynamic_cast<CCDictionary*>( vsCountry->objectAtIndex(0) );
        CCDictionary* rcountryDict = dynamic_cast<CCDictionary*>( vsCountry->objectAtIndex(1) );
        
        string serverL = CC_ITOA( lcountryDict->valueForKey("id")->intValue() );
        serverL = "#"+serverL;
        string nameL = lcountryDict->valueForKey("name")->getCString();
        string picL = lcountryDict->valueForKey("banner")->getCString();
        picL = CCCommonUtils::getCountryIconByName(picL);
        m_kingLLabel->setString(nameL+" "+serverL);
        auto iconL = CCLoadSprite::createSprite(picL.c_str());
        iconL->setScale(0.4);
        m_kingLNode->addChild(iconL);
        
        string serverR = CC_ITOA( rcountryDict->valueForKey("id")->intValue() );
        serverR = "#"+serverR;
        string nameR = rcountryDict->valueForKey("name")->getCString();
        string picR = rcountryDict->valueForKey("banner")->getCString();
        picR = CCCommonUtils::getCountryIconByName(picR);
        m_kingRLabel->setString(serverR+" "+nameR);
        auto iconR = CCLoadSprite::createSprite(picR.c_str());
        iconR->setScale(0.4);
        m_kingRNode->addChild(iconR);
        
        m_vsNode->setVisible(true);
    }
    
    if (m_actAlInfo == NULL || !GlobalData::shared()->playerInfo.isInAlliance())
    {
        m_joinLabel->setString(_lang("150434"));
        CCCommonUtils::setButtonTitle(m_joinBtn, _lang("150473").c_str());
        m_noAlNode->setVisible(true);
        m_alBoxNode->setVisible(false);
        m_rkBtnSpr->setVisible(false);
        m_cup3Label->setString("");
    }
    else
    {
        m_noAlNode->setVisible(false);
        m_alBoxNode->setVisible(true);
        m_rkBtnSpr->setVisible(true);
        m_cup3Label->setString(_lang("150291"));
        
        float pro = 0;
        m_pt1Label->setString("0");
        m_scoreLabel->setString( CC_ITOA(m_actAlInfo->currentScore));
        m_scoreLabel->setPositionX( m_scoreTLabel->getPositionX()+m_scoreTLabel->getContentSize().width*m_scoreTLabel->getOriginScaleX() + 5 );
        m_scoreIcon->setPositionX(m_scoreLabel->getPositionX()+m_scoreLabel->getContentSize().width*m_scoreLabel->getOriginScaleX()+5);
        
        int lastNum = 0;
        for (int i=0; i<m_actAlInfo->m_tagVec.size(); i++)
        {
            int curTagScore = m_actAlInfo->m_tagVec[i];
            double tmpScore = curTagScore;
            if (i==0)
            {
                m_pt2Label->setString(CC_CMDITOA(tmpScore));
                if (curTagScore > m_actAlInfo->currentScore){
                    CCCommonUtils::setSpriteGray(m_box1OpenIcon, false);
                    if (m_actAlInfo->currentScore >= lastNum) {
                        pro += 0.33333*(m_actAlInfo->currentScore-lastNum)/(curTagScore-lastNum);
                    }
                }else {
                    CCCommonUtils::setSpriteGray(m_box1OpenIcon, true);
                    lastNum = curTagScore;
                    pro = 0.33333;
                }
            }
            else if (i==1)
            {
                m_pt3Label->setString(CC_CMDITOA(tmpScore));
                if (curTagScore > m_actAlInfo->currentScore){
                    CCCommonUtils::setSpriteGray(m_box2OpenIcon, false);
                    if (m_actAlInfo->currentScore >= lastNum) {
                        pro += 0.33333*(m_actAlInfo->currentScore-lastNum)/(curTagScore-lastNum);
                    }
                }else {
                    CCCommonUtils::setSpriteGray(m_box2OpenIcon, true);
                    lastNum = curTagScore;
                    pro = 0.66666;
                }
            }
            else if (i==2)
            {
                m_pt4Label->setString(CC_CMDITOA(tmpScore));
                if (curTagScore > m_actAlInfo->currentScore){
                    CCCommonUtils::setSpriteGray(m_box3OpenIcon, false);
                    if (m_actAlInfo->currentScore >= lastNum) {
                        pro += 0.33333*(m_actAlInfo->currentScore-lastNum)/(curTagScore-lastNum);
                    }
                }else {
                    CCCommonUtils::setSpriteGray(m_box3OpenIcon, true);
                    pro = 1.0;
                }
            }
        }
        
        pro = MIN(pro, 1);
        pro = MAX(pro, 0);
        m_bar->setScaleX(pro);
        m_bar2->setScaleX(pro);
        if (pro > 1.0) {
            m_bar2->setVisible(true);
            m_bar->setVisible(false);
        }else {
            m_bar2->setVisible(false);
            m_bar->setVisible(true);
        }
        m_arrowSpr->setPositionX( m_bar->getPositionX() + m_bar->getContentSize().width*pro );
    }
    retDataS(NULL);
    
    refreshData();
}

void CountryScoreActiveView::onGameTick(float time)
{
    if(m_actPlInfo == nullptr){
        m_timeLabel->setString("");
        return;
    }
    
    double nowTime = GlobalData::shared()->getWorldTime();
    
    if (m_actPlInfo->startTime<nowTime && m_actPlInfo->endTime>nowTime)//活动中
    {
        double leftTime = m_actPlInfo->endTime - nowTime;
        m_ttLabel->setString(_lang_1("150284", ""));
        m_timeLabel->setString(CC_SECTOA(leftTime));
    }
    else if (m_actPlInfo->startTime > nowTime)//活动准备中
    {
        double leftTime = m_actPlInfo->startTime - nowTime;
        m_ttLabel->setString(_lang_1("150283", ""));
        m_timeLabel->setString(CC_SECTOA(leftTime));
    }
    else
    {//活动结束
        m_ttLabel->setString("");
        m_timeLabel->setString("");
    }
    int w1 = m_ttLabel->getContentSize().width*m_ttLabel->getOriginScaleX();
    int w2 = m_timeLabel->getContentSize().width*m_timeLabel->getOriginScaleX();
    int posX = w1-(w1+w2)/2;
    m_ttLabel->setPositionX(posX);
    m_timeLabel->setPositionX(posX);
}

void CountryScoreActiveView::refreshData()
{
    if (m_actPlInfo==NULL) {
        return;
    }
    string scoreIds = m_actPlInfo->getScoreMeth;//"47087,47088,47089,47090,47091,47092,47093,47094,47095,47096,47097,47098";
    vector<string> socreIdVec;
    CCCommonUtils::splitString(scoreIds, "|", socreIdVec);
    
    int cellH = 60;
    
    int bgH = 0;
    auto tbg = CCLoadSprite::loadResource("Commond7_6_2.png");
    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
    auto tbg1 = CCLoadSprite::createSprite("Commond7_6_2.png");
    tbg1->setFlipY(true);
    auto tbg2 = CCLoadSprite::createSprite("Commond7_6_2.png");
    tbg2->setFlipX(true);
    tbg2->setFlipY(true);
    tbg1->setPosition(ccp(161, bgH+51));
    tbg2->setPosition(ccp(479, bgH+51));
    tBatchNode->addChild(tbg1);
    tBatchNode->addChild(tbg2);
    bgH += 99;
    
    int limitH = m_midNode->getContentSize().height + socreIdVec.size()*cellH - 100;
    
    while(bgH < limitH)
    {
        auto tbgL = CCLoadSprite::createSprite("Commond7_6_3.png");
        auto tbgR = CCLoadSprite::createSprite("Commond7_6_3.png");
        tbgR->setFlipX(true);
        tbgL->setPosition(ccp(161, bgH + 50));
        tbgR->setPosition(ccp(479, bgH + 50));
        tBatchNode->addChild(tbgL);
        tBatchNode->addChild(tbgR);
        bgH += 99;
    }
    
    auto tbg3 = CCLoadSprite::createSprite("Commond7_6_2.png");
    auto tbg4 = CCLoadSprite::createSprite("Commond7_6_2.png");
    tbg4->setFlipX(true);
    tbg3->setPosition(ccp(161, bgH+49));
    tbg4->setPosition(ccp(479, bgH+49));
    tBatchNode->addChild(tbg3);
    tBatchNode->addChild(tbg4);
    bgH += 99;
    
    tBatchNode->setPositionX(0);
    m_scrollView->addChild(tBatchNode);
    
    int curY = bgH - socreIdVec.size()*cellH - m_midNode->getContentSize().height;
    for (int i=0; i<socreIdVec.size(); i++) {
        auto cell = SingleScoreCell::create(socreIdVec[i]);
        cell->setPosition(ccp(0, curY));
        m_scrollView->addChild(cell);
        curY += 60;
    }
    
    m_midNode->removeFromParent();
    m_midNode->setPosition(ccp(5, bgH - m_midNode->getContentSize().height));
    m_scrollView->addChild(m_midNode);
    int sumY = bgH;
    //    sumY -= 50;
    
    m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,sumY));
    m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - sumY));
}

SEL_CCControlHandler CountryScoreActiveView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "OnTipBtnClick", CountryScoreActiveView::OnTipBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onJoinBtnClick", CountryScoreActiveView::onJoinBtnClick);
    return NULL;
}

bool CountryScoreActiveView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_topNode", CCNode*, this->m_topNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_midNode", CCNode*, this->m_midNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tabList", CCNode*, this->m_tabList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCScale9Sprite*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipBtn", CCControlButton*, this->m_tipBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subTitleLabel", CCLabelIF*, this->m_subTitleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sub2TitleLabel", CCLabelIF*, this->m_sub2TitleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIF*, this->m_msgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ttLabel", CCLabelIF*, this->m_ttLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scoreTLabel", CCLabelIF*, this->m_scoreTLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scoreLabel", CCLabelIF*, this->m_scoreLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bar", CCScale9Sprite*, this->m_bar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bar2", CCScale9Sprite*, this->m_bar2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rkBtnSpr", CCSprite*, this->m_rkBtnSpr);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scoreIcon", CCSprite*, this->m_scoreIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_box1OpenIcon", CCSprite*, this->m_box1OpenIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_box2OpenIcon", CCSprite*, this->m_box2OpenIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_box3OpenIcon", CCSprite*, this->m_box3OpenIcon);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cup1Label", CCLabelIF*, this->m_cup1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cup2Label", CCLabelIF*, this->m_cup2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cup3Label", CCLabelIF*, this->m_cup3Label);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touch1Node", CCNode*, this->m_touch1Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touch2Node", CCNode*, this->m_touch2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touch3Node", CCNode*, this->m_touch3Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touch4Node", CCNode*, this->m_touch4Node);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrowSpr", CCSprite*, this->m_arrowSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pt1Label", CCLabelIF*, this->m_pt1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pt2Label", CCLabelIF*, this->m_pt2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pt3Label", CCLabelIF*, this->m_pt3Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pt4Label", CCLabelIF*, this->m_pt4Label);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_alBoxNode", CCNode*, this->m_alBoxNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noAlNode", CCNode*, this->m_noAlNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_joinBtn", CCControlButton*, this->m_joinBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_joinLabel", CCLabelIF*, this->m_joinLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sub2TitleLabelS", CCLabelIF*, this->m_sub2TitleLabelS);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scoreTLabelS", CCLabelIF*, this->m_scoreTLabelS);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scoreLabelS", CCLabelIF*, this->m_scoreLabelS);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barS", CCScale9Sprite*, this->m_barS);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bar2S", CCScale9Sprite*, this->m_bar2S);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scoreIconS", CCSprite*, this->m_scoreIconS);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_box1OpenIconS", CCSprite*, this->m_box1OpenIconS);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_box2OpenIconS", CCSprite*, this->m_box2OpenIconS);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_box3OpenIconS", CCSprite*, this->m_box3OpenIconS);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cup1LabelS", CCLabelIF*, this->m_cup1LabelS);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cup2LabelS", CCLabelIF*, this->m_cup2LabelS);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cup3LabelS", CCLabelIF*, this->m_cup3LabelS);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touch1NodeS", CCNode*, this->m_touch1NodeS);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touch2NodeS", CCNode*, this->m_touch2NodeS);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touch3NodeS", CCNode*, this->m_touch3NodeS);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touch4NodeS", CCNode*, this->m_touch4NodeS);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrowSprS", CCSprite*, this->m_arrowSprS);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pt1LabelS", CCLabelIF*, this->m_pt1LabelS);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pt2LabelS", CCLabelIF*, this->m_pt2LabelS);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pt3LabelS", CCLabelIF*, this->m_pt3LabelS);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pt4LabelS", CCLabelIF*, this->m_pt4LabelS);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vsNode", CCNode*, this->m_vsNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_kingLNode", CCNode*, this->m_kingLNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_kingRNode", CCNode*, this->m_kingRNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_kingLLabel", CCLabelIF*, this->m_kingLLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_kingRLabel", CCLabelIF*, this->m_kingRLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vsLabel", CCLabelIFBMFont*, this->m_vsLabel);
    
    return false;
}

bool CountryScoreActiveView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_infoList, pTouch)){
    }
    m_startY = pTouch->getLocation().y;
    return true;
}

void CountryScoreActiveView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    int endY = pTouch->getLocation().y;
    if (abs(endY - m_startY) >= 20) {
        return;
    }
    if (m_alBoxNode->isVisible() && isTouchInside(m_touch1Node, pTouch)) {
        clickShowRD(0);
    }else if (m_alBoxNode->isVisible() && isTouchInside(m_touch2Node, pTouch)) {
        clickShowRD(1);
    }else if (m_alBoxNode->isVisible() && isTouchInside(m_touch3Node, pTouch)) {
        clickShowRD(2);
    }else if (isTouchInside(m_touch4Node, pTouch)) {
        if (ActivityController::getInstance()->KingAlScoreRank < 0)
        {
            CCCommonUtils::flyHint("", "", _lang("150294"));
        }
        else
        {
            if (ActivityController::getInstance()->m_kingAlRankKeys.size() <= 0) {
                m_waitInterface = GameController::getInstance()->showWaitInterface(m_infoList);
                m_waitInterface->setPosition(ccp(m_infoList->getContentSize().width/2, m_infoList->getContentSize().height/2));
                m_getRankRwdType = 3;
                ActivityController::getInstance()->getCurRankRwd( m_actAlInfo->activityId , 3);
            }else {
                PopupViewController::getInstance()->addPopupInView(SingleScoreRankView::create(3));
            }
        }
    }
    else if (isTouchInside(m_touch4NodeS, pTouch)) {
        if (ActivityController::getInstance()->KingPlScoreRank < 0)
        {
            CCCommonUtils::flyHint("", "", _lang("150294"));
        }
        else
        {
            if (ActivityController::getInstance()->m_kingPlRankKeys.size() <= 0) {
                m_waitInterface = GameController::getInstance()->showWaitInterface(m_infoList);
                m_waitInterface->setPosition(ccp(m_infoList->getContentSize().width/2, m_infoList->getContentSize().height/2));
                m_getRankRwdType = 2;
                ActivityController::getInstance()->getCurRankRwd( m_actPlInfo->activityId , 2);
            }else {
                PopupViewController::getInstance()->addPopupInView(SingleScoreRankView::create(2));
            }
        }
    }
    else if (isTouchInside(m_touch1NodeS, pTouch)) {
        clickShowRD(3);
    }else if (isTouchInside(m_touch2NodeS, pTouch)) {
        clickShowRD(4);
    }else if (isTouchInside(m_touch3NodeS, pTouch)) {
        clickShowRD(5);
    }
}

void CountryScoreActiveView::retRankData(CCObject* param)
{
    if (m_waitInterface != NULL) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    PopupViewController::getInstance()->addPopupInView(SingleScoreRankView::create(m_getRankRwdType));
}

void CountryScoreActiveView::OnTipBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    CCCommonUtils::showHelpShiftSingleFAQ("45220");
}

void CountryScoreActiveView::onJoinBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    PopupViewController::getInstance()->removeAllPopupView();
    PopupViewController::getInstance()->addPopupInView(JoinAllianceView::create(11));
}

void CountryScoreActiveView::clickShowRD(int day)
{
    if (day < 3)
    {
        if (m_actAlInfo && day < m_actAlInfo->m_tagVec.size())
        {
            int tagPoint = m_actAlInfo->m_tagVec[day];
            if (m_actAlInfo->m_scoreRwdMap.find(tagPoint) != m_actAlInfo->m_scoreRwdMap.end()
                && m_actAlInfo->m_rwdPriceMap.find(tagPoint) != m_actAlInfo->m_rwdPriceMap.end())
            {
                CCArray* tmpArr = m_actAlInfo->m_scoreRwdMap[tagPoint];
                int price = m_actAlInfo->m_rwdPriceMap[tagPoint];
                bool isGet = false;
                if (tagPoint <= m_actAlInfo->currentScore) {
                    isGet = true;
                }
                string title = "";
                if (day == 0) {
                    title = _lang("150464");
                }else if (day == 1) {
                    title = _lang("150465");
                }else if (day == 2) {
                    title = _lang("150466");
                }
                PopupViewController::getInstance()->addPopupView(SingleScoreRwdPopView::create(tmpArr, tagPoint, price, isGet, title));
            }
        }
    }
    else
    {
        int curD = day-3;
        if (m_actPlInfo && curD < m_actPlInfo->m_tagVec.size())
        {
            int tagPoint = m_actPlInfo->m_tagVec[curD];
            if (m_actPlInfo->m_scoreRwdMap.find(tagPoint) != m_actPlInfo->m_scoreRwdMap.end()
                && m_actPlInfo->m_rwdPriceMap.find(tagPoint) != m_actPlInfo->m_rwdPriceMap.end())
            {
                CCArray* tmpArr = m_actPlInfo->m_scoreRwdMap[tagPoint];
                int price = m_actPlInfo->m_rwdPriceMap[tagPoint];
                bool isGet = false;
                if (tagPoint <= m_actPlInfo->currentScore) {
                    isGet = true;
                }
                string title = "";
                if (curD == 0) {
                    title = _lang("150461");
                }else if (curD == 1) {
                    title = _lang("150462");
                }else if (curD == 2) {
                    title = _lang("150463");
                }
                PopupViewController::getInstance()->addPopupView(SingleScoreRwdPopView::create(tmpArr, tagPoint, price, isGet, title));
            }
        }
    }
}
