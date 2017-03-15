//
//  AllianceScoreActiveView.cpp
//  IF
//
//  Created by 付彬 on 15/12/2.
//
//

#include "AllianceScoreActiveView.hpp"
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

AllianceScoreActiveView* AllianceScoreActiveView::create(ActivityEventObj* info, bool isInner)
{
    auto ret = new AllianceScoreActiveView();
    if (ret && ret->init(info, isInner)) {
        ret->autorelease();
    } else {
        CC_SAFE_RELEASE(ret);
    }
    return ret;
}

bool AllianceScoreActiveView::init(ActivityEventObj* info, bool isInner)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    
    m_actEventObj = info;
    auto tmpCCB = CCBLoadFile("SingleScoreActiveView2", this, this);
    setContentSize(tmpCCB->getContentSize());
    
    setTitleName( m_actEventObj->name );
    m_subTitleLabel->setString(_lang("150293"));
    m_scoreTLabel->setString(_lang_1("150364", ""));
    m_cup1Label->setString(_lang("150363")+":");
    m_cup2Label->setString(_lang("150290"));
    m_cup3Label->setString(_lang("150291"));
    if (ActivityController::getInstance()->AllianceScoreRank > 0)
    {
        m_cup2Label->setString( CC_ITOA(ActivityController::getInstance()->AllianceScoreRank) );
    }else if (ActivityController::getInstance()->AllianceScoreRank == 0)
    {
        m_cup2Label->setString( "5000+" );
    }
    m_cup2Label->setPositionX(m_cup1Label->getPositionX()+m_cup1Label->getContentSize().width*m_cup1Label->getOriginScaleX()+5);
    
    m_sub2TitleLabel->setString(_lang("150420"));
    m_msgLabel->setString(_lang("150365"));
    
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
    
    m_actInfo = NULL;
    m_waitInterface = NULL;
    m_guideKey = "";
    
    return true;
}

void AllianceScoreActiveView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    
    setTitleName(_lang("150350"));
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceScoreActiveView::retData), MSG_FRESH_SINGLE_SCORE_VIEW, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceScoreActiveView::retRankData), MSG_FRESH_SINGLE_SCORE_RANK_VIEW, NULL);
    
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(AllianceScoreActiveView::onGameTick), this, 1,kCCRepeatForever, 0.0f, false);
    onGameTick(0);
    
    if (ActivityController::getInstance()->m_allScoreRwdMap.size() <= 0) {
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_infoList);
        m_waitInterface->setPosition(ccp(m_infoList->getContentSize().width/2, m_infoList->getContentSize().height/2));
        ActivityController::getInstance()->getSingleScoreData(m_actEventObj->activityid);
    }else {
        retData(NULL);
    }
}

void AllianceScoreActiveView::onExit()
{
    if (m_waitInterface != NULL) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRESH_SINGLE_SCORE_VIEW);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRESH_SINGLE_SCORE_RANK_VIEW);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(AllianceScoreActiveView::onGameTick), this);
    setTouchEnabled(false);
    CCNode::onExit();
}

void AllianceScoreActiveView::retData(CCObject* param)
{
    map<string, ActivityInfo*>::iterator it = ActivityController::getInstance()->m_allScoreRwdMap.begin();
    for (; it != ActivityController::getInstance()->m_allScoreRwdMap.end(); it++)
    {
        ActivityInfo* info = it->second;
        if (it->second->type == 2) {
            m_actInfo = it->second;
            break;
        }
    }
    if (m_actInfo == NULL) {
        return;
    }
    
    if (m_waitInterface != NULL) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    
    float pro = 0;
    m_pt1Label->setString("0");
    m_scoreLabel->setString( CC_ITOA(m_actInfo->currentScore));
    m_scoreLabel->setPositionX( m_scoreTLabel->getPositionX()+m_scoreTLabel->getContentSize().width*m_scoreTLabel->getOriginScaleX() + 5 );
    m_scoreIcon->setPositionX(m_scoreLabel->getPositionX()+m_scoreLabel->getContentSize().width*m_scoreLabel->getOriginScaleX()+5);
    
    int lastNum = 0;
    for (int i=0; i<m_actInfo->m_tagVec.size(); i++)
    {
        int curTagScore = m_actInfo->m_tagVec[i];
        double tmpScore = curTagScore;
        if (i==0)
        {
            m_pt2Label->setString(CC_CMDITOA(tmpScore));
            if (curTagScore > m_actInfo->currentScore){
                CCCommonUtils::setSpriteGray(m_box1OpenIcon, false);
                if (m_actInfo->currentScore >= lastNum) {
                    pro += 0.33333*(m_actInfo->currentScore-lastNum)/(curTagScore-lastNum);
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
            if (curTagScore > m_actInfo->currentScore){
                CCCommonUtils::setSpriteGray(m_box2OpenIcon, false);
                if (m_actInfo->currentScore >= lastNum) {
                    pro += 0.33333*(m_actInfo->currentScore-lastNum)/(curTagScore-lastNum);
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
            if (curTagScore > m_actInfo->currentScore){
                CCCommonUtils::setSpriteGray(m_box3OpenIcon, false);
                if (m_actInfo->currentScore >= lastNum) {
                    pro += 0.33333*(m_actInfo->currentScore-lastNum)/(curTagScore-lastNum);
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
    
    refreshData();
}

void AllianceScoreActiveView::onGameTick(float time)
{
    if(m_actInfo == nullptr){
        m_timeLabel->setString("");
        return;
    }
    
    double nowTime = GlobalData::shared()->getWorldTime();
    
    if (m_actInfo->startTime<nowTime && m_actInfo->endTime>nowTime)//活动中
    {
        double leftTime = m_actInfo->endTime - nowTime;
        m_ttLabel->setString(_lang_1("150284", ""));
        m_timeLabel->setString(CC_SECTOA(leftTime));
    }
    else if (m_actInfo->startTime > nowTime)//活动准备中
    {
        double leftTime = m_actInfo->startTime - nowTime;
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

void AllianceScoreActiveView::refreshData()
{
    string scoreIds = m_actInfo->getScoreMeth;//"47087,47088,47089,47090,47091,47092,47093,47094,47095,47096,47097,47098";
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

SEL_CCControlHandler AllianceScoreActiveView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "OnTipBtnClick", AllianceScoreActiveView::OnTipBtnClick);
    return NULL;
}

bool AllianceScoreActiveView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
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
    
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode", CCNode*, this->m_particleNode);
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
    return false;
}

bool AllianceScoreActiveView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_infoList, pTouch)){
    }
    m_startY = pTouch->getLocation().y;
    return true;
}

void AllianceScoreActiveView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    int endY = pTouch->getLocation().y;
    if (abs(endY - m_startY) >= 20) {
        return;
    }
    if (isTouchInside(m_touch1Node, pTouch)) {
        clickShowRD(0);
    }else if (isTouchInside(m_touch2Node, pTouch)) {
        clickShowRD(1);
    }else if (isTouchInside(m_touch3Node, pTouch)) {
        clickShowRD(2);
    }else if (isTouchInside(m_touch4Node, pTouch)) {
        if (ActivityController::getInstance()->AllianceScoreRank < 0)
        {
            CCCommonUtils::flyHint("", "", _lang("150294"));
        }
        else
        {
            if (ActivityController::getInstance()->m_allRankKeys.size() <= 0) {
                m_waitInterface = GameController::getInstance()->showWaitInterface(m_infoList);
                m_waitInterface->setPosition(ccp(m_infoList->getContentSize().width/2, m_infoList->getContentSize().height/2));
                ActivityController::getInstance()->getCurRankRwd( m_actInfo->activityId , 1);
            }else {
                PopupViewController::getInstance()->addPopupInView(SingleScoreRankView::create(1));
            }
        }
    }
}

void AllianceScoreActiveView::retRankData(CCObject* param)
{
    if (m_waitInterface != NULL) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    PopupViewController::getInstance()->addPopupInView(SingleScoreRankView::create(1));
}

void AllianceScoreActiveView::OnTipBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    CCCommonUtils::showHelpShiftSingleFAQ("45220");
//    PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("150373"),kCCTextAlignmentLeft));
}

void AllianceScoreActiveView::clickShowRD(int day)
{
    if (m_actInfo && day < m_actInfo->m_tagVec.size())
    {
        int tagPoint = m_actInfo->m_tagVec[day];
        if (m_actInfo->m_scoreRwdMap.find(tagPoint) != m_actInfo->m_scoreRwdMap.end()
            && m_actInfo->m_rwdPriceMap.find(tagPoint) != m_actInfo->m_rwdPriceMap.end())
        {
            CCArray* tmpArr = m_actInfo->m_scoreRwdMap[tagPoint];
            int price = m_actInfo->m_rwdPriceMap[tagPoint];
            bool isGet = false;
            if (tagPoint <= m_actInfo->currentScore) {
                isGet = true;
            }
            string title = "";
            if (day == 0) {
                title = _lang("150438");
            }else if (day == 1) {
                title = _lang("150439");
            }else if (day == 2) {
                title = _lang("150440");
            }
            PopupViewController::getInstance()->addPopupView(SingleScoreRwdPopView::create(tmpArr, tagPoint, price, isGet, title));
        }
        
    }
    
}
