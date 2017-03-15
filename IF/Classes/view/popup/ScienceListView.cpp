//
//  ScienceListView.cpp
//  IF
//
//  Created by fubin on 14-3-19.
//
//

#include "CCCommonUtils.h"
#include "ScienceListView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "ScienceController.h"
#include "ScienceView.h"
#include "ParticleController.h"
#include "QueueController.h"
#include "UserUpgradeView.h"
#include "CCThreadManager.h"
#include "TipsView.h"
#include "YesNoDialog.h"


ScienceListView* ScienceListView::create(int buildId){
    ScienceListView* ret = new ScienceListView();
    if(ret && ret->init(buildId)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

ScienceListView::ScienceListView():m_buildId(0),m_title(""),m_clickType(-1),m_isInit(false)
{
    memset(m_BtnInfo, 0, sizeof(m_BtnInfo));
    memset(m_BtnState, 0, sizeof(m_BtnState));
}

bool ScienceListView::init(int buildId)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(8, true);
    CCLoadSprite::doResourceByCommonIndex(4, true);
    CCLoadSprite::doResourceByCommonIndex(101, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(4, false);
        CCLoadSprite::doResourceByCommonIndex(8, false);
        CCLoadSprite::doResourceByCommonIndex(101, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_SCIENCE);
    });
    
    m_buildId = buildId;
    
    m_smokeLayer = CCLayer::create();
    
    // 添加背景图，因为这个界面其实是贴在主界面上的。
    // auto tbg = CCLoadSprite::loadResource("technology_09.png");
    // auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
    // int maxHight = CCDirector::sharedDirector()->getWinSize().height;
    // int curHight = -100;
    // while (curHight<maxHight) {
    //     auto bg = CCLoadSprite::createSprite("technology_09.png");
    //     bg->setAnchorPoint(ccp(0, 1));
    //     if (CCCommonUtils::isIosAndroidPad()) {
    //         bg->setScale(2.4);
    //     }
    //     bg->setPosition(ccp(0, curHight));
    //     curHight += bg->getContentSize().height;
    //     bg->runAction(CCFadeIn::create(0.5));
    //     tBatchNode->addChild(bg);
    // }
    // this->addChild(tBatchNode);
    this->addChild(m_smokeLayer);
    
    auto node = CCBLoadFile("ScienceListView1",this,this);
    if (node == NULL) {
        CCLOG("ScienceListView1 not found.");
        return false;
    }
    m_hintText->setHorizontalAlignment(TextHAlignment::CENTER);
    
    Size winSize = CCDirector::sharedDirector()->getWinSize();
    this->setContentSize(winSize);
    
    // 屏幕自适应
    int addHeight = getExtendHeight();
    //m_sceinceNode->setPositionY(m_sceinceNode->getPositionY()+addHeight);
    m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height+addHeight));
    
    // 将齿轮节点从主界面移除并添加到新增的滚动区域中
    m_scrollView = CCScrollView::create(m_infoList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_infoList->addChild(m_scrollView);
    m_scrollView->setTouchPriority(Touch_Popup);
    
    m_scrollNode->removeFromParent();
    m_scrollNode->setPosition(0, 0);
    m_scrollView->addChild(m_scrollNode);
    
    m_scrollView->setViewSize(m_infoList->getContentSize());
    m_scrollView->setContentSize(m_scrollNode->getContentSize());
    m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - m_scrollNode->getContentSize().height));
    
    // ?
    FunBuildInfo& m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
    m_title = _lang(m_info.name)+" "+_lang_1("102272", CC_ITOA(m_info.level));
    m_smokeLayer->setPosition(m_mainNode->getPosition());
    if(FunBuildController::getInstance()->checkOpenUpstar(m_info.type)
       && FunBuildController::getInstance()->building_Miracle_open){
        if(m_info.starNum >= 1){
            m_title = _lang(m_info.name) +" "+ _lang("160001");
            m_title.append(" ");
            m_title.append(CC_ITOA(m_info.starNum));
        }
    }
    setTitleName(m_title.c_str());
    
    //m_desLabel->setString(_lang("121990"));
    
    // 下方的图片栏暂时去掉
#if 0
    for (int i=1; i<=4; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
        m_fireNode1->addChild(particle);
        
        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
        m_fireNode2->addChild(particle1);
    }
#endif
    
    for (int i=0;i<_countof(m_BtnInfo);++i) {
        m_BtnInfo[i].m_nameBG->setVisible(false);
        m_BtnInfo[i].m_nameLabel->setString("");
    }
    
    CCCommonUtils::setButtonTitle(m_btn, _lang("120153").c_str());
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(ScienceListView::AnimationCallback));
    //    addBGPic();
    
    auto pos = m_progrossBar->getPosition();
    m_barClipNode = CCClipNode::create(m_progrossBar->getContentSize().width, m_progrossBar->getContentSize().height);
    m_progrossBar->getParent()->addChild(m_barClipNode);
    m_barClipNode->setAnchorPoint(ccp(0, 0));
    m_progrossBar->retain();
    m_progrossBar->removeFromParent();
    m_barClipNode->addChild(m_progrossBar);
    m_progrossBar->setPosition(0, 0);
    m_progrossBar->release();
    m_barClipNode->setPosition(pos);
    m_timeText->setZOrder(1);
    
    update(1.0f);
    return true;
}

void ScienceListView::addBGPic()
{
    int totalH = 0;
    auto size = CCDirector::sharedDirector()->getWinSize();
    while (totalH < size.height) {
        //        auto sprite = CCLoadSprite::createSprite("kji_bg.png");
        auto sprite = CCCommonUtils::addFilterSprite("UI_UseSkill_picture_blackwhite.png", CCCommonUtils::covertHSBToRGB(55, 0.77, 1.0), 0, 1.04, 4.4);
        this->m_bgContainer->addChild(sprite);
        sprite->setAnchorPoint(ccp(0, 0));
        sprite->setPosition(ccp(0, totalH));
        totalH += sprite->getContentSize().height;
    }
}

void ScienceListView::updateInfo()
{
    for (int i=0;i<_countof(m_BtnInfo);++i) {
        m_BtnInfo[i].m_iType = 0;
        m_BtnInfo[i].m_iState = 0;
    }
    
    // 更新按钮对应的类型
    int iScienceCount = ScienceController::getInstance()->m_scienceType.size();
    for (int i=0;i<iScienceCount;++i)
    {
        int type = ScienceController::getInstance()->m_scienceType[i];
        string posStr = CCCommonUtils::getPropById(CC_ITOA(type), "position");  // 1~8
        
        int pos = atoi(posStr.c_str());
        pos -= 1;
        if (pos >=0 && pos < _countof(m_BtnInfo)) {
            m_BtnInfo[pos].m_iType = type;
        }
        else {
            CCLOG("Invalid science position.");
        }
    }
    
    // 通过按钮类型来显示正确的图片
    for (int i=0;i<_countof(m_BtnInfo);++i) {
        updateBtn(i);
    }
    
    return;
}

int ScienceListView::updateBtn(int pos)
{
    if (pos < 0 || pos >= _countof(m_BtnInfo)) {
        return -1;
    }
    
    int iType = m_BtnInfo[pos].m_iType;
    
    // 未实现
    if (iType <= 0) {
        SetBtnLock(pos);
        m_BtnInfo[pos].m_iState = 0;
        return pos;
    }
    
    string picName = CCCommonUtils::getIcon(CC_ITOA(iType));
    string sName = CCCommonUtils::getNameById(CC_ITOA(iType));
    
    // 没有图片的话，进行上锁处理
    if (picName.empty() || sName.empty())
    {
        CCLOG("ScienceListView::updateBtn - pos(%d) invalid data.");
    }
    
    // 设置按钮的图片状态
    // 未开放
    int iSwitch = ScienceController::getInstance()->GetScienceCfg(pos);
    if (iSwitch == 0) {
        m_BtnInfo[pos].m_iState = 1;
    }
    // 正常科技按钮
    else {
        m_BtnInfo[pos].m_iState = 2;
    }
    
    // 按钮文字
    m_BtnInfo[pos].m_nameLabel->setString(sName);
    if (CCCommonUtils::isIosAndroidPad()) {
        float fw = m_BtnInfo[pos].m_nameLabel->getContentSize().width;
        float s = m_BtnInfo[pos].m_nameLabel->getScaleX();
        if (fw>220) {//防爆框
            m_BtnInfo[pos].m_nameLabel->setScaleX(220/fw*s);
        }
    }
    else {
        m_BtnInfo[pos].m_nameLabel->setMaxScaleXByWidth(135);
    }
    
    // 添加按钮图片
    auto picSpr = CCLoadSprite::createSprite(picName.c_str(), true, CCLoadSpriteType_SCIENCE);
    if (m_BtnInfo[pos].m_iState == 1) {
        CCCommonUtils::setSpriteGray(picSpr, true);
    }
    picSpr->setScale(1.2);
    m_BtnInfo[pos].m_iconNode->removeAllChildren();
    m_BtnInfo[pos].m_iconNode->addChild(picSpr);
    
    //
    m_BtnInfo[pos].m_nameLabel->setVisible(true);
    m_BtnInfo[pos].m_nameBG->setVisible(true);
    
    return pos;
}

void ScienceListView::SetBtnLock(int pos)
{
    auto picSpr = CCLoadSprite::createSprite("iron_lock.png");
    m_BtnInfo[pos].m_iconNode->removeAllChildren();
    m_BtnInfo[pos].m_iconNode->addChild(picSpr);
    
    m_BtnInfo[pos].m_nameLabel->setVisible(false);
    m_BtnInfo[pos].m_nameBG->setVisible(false);
    return;
}

void ScienceListView::update(float time)
{
    bool flag = false;
    auto &map = GlobalData::shared()->scienceInfo.infoMap;
    for (auto it = map.begin(); it != map.end(); it++)
    {
        if(it->second.finishTime > GlobalData::shared()->getWorldTime()){
            flag = true;
            m_btn->setVisible(true);
            m_hintText->setString("");
            std::string name = it->second.getName();
            std::string description = it->second.getDescription();
            m_nameText->setString(name);
            
            float maxWidth = CCDirector::sharedDirector()->getWinSize().width;
            maxWidth = maxWidth/5 - 5; // 因为一行有5个按钮
            
            float fw = m_nameText->getContentSize().width;
            if (fw>0 && fw>maxWidth) {//防爆框
                m_nameText->setScaleX(maxWidth/fw);
            }
            
            m_descriptionText->setString(description);
            m_progressBG->setVisible(true);
            m_progrossBar->setVisible(true);
            int time = it->second.finishTime - GlobalData::shared()->getWorldTime();
            m_timeText->setString(CCCommonUtils::timeLeftToCountDown(time));
            int sumTime = it->second.time_need;
            int totalW = 342;
            if(time > sumTime){
                time = sumTime;
            }
            float w = totalW - time * 1.0  * totalW / sumTime;
            m_barClipNode->setContentSize(CCSize(w, m_progrossBar->getContentSize().height));
            break;
        }
    }
    if(!flag){
        m_progressBG->setVisible(false);
        m_progrossBar->setVisible(false);
        m_nameText->setString("");
        m_timeText->setString("");
        m_descriptionText->setString("");
        m_hintText->setString(_lang("120154"));
        m_btn->setVisible(false);
        
        stopCircleGlow(NULL);
    }
}

void ScienceListView::AnimationCallback()
{
    this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
    string name = getAnimationManager()->getLastCompletedSequenceName();
    if(name == "FadeIn") {
        m_isInit = true;
        playCircleGlow();
        updateInfo();
    }
}

void ScienceListView::playClose(int type, bool force)
{
    // force = true 由主界面触发
    if (force == false)
    {
        int pos = type - 1;
        if (pos < 0 || pos >= _countof(m_BtnInfo)) {
            return;
        }
        
        // 未实现
        if (m_BtnInfo[pos].m_iState == 0) {
            return;
        }
        
        // 未开放，弹出一个提示
        if (m_BtnInfo[pos].m_iState == 1) {
            const char* lang_str = NULL;
            switch (pos) {
                case 4: lang_str = "122000"; break;
                case 5: lang_str = "122001"; break;
                case 6: lang_str = "122002"; break;
                case 7: lang_str = "122003"; break;
            }
            if (lang_str) {
                YesNoDialog::show(_lang(lang_str).c_str(), NULL);
            }
            return;
        }
    }
    
    
    for (int i=0;i<_countof(m_BtnInfo);++i) {
        m_BtnInfo[i].m_crNode->removeAllChildren();
    }
    
    // 记录最后点击的按钮，等动画播放完了处理，貌似只是指导
    m_clickType = type;
    stopCircleGlow(NULL);
    
    for (int i=0;i<_countof(m_BtnInfo);++i) {
        m_BtnInfo[i].m_nameBG->setVisible(false);
        m_BtnInfo[i].m_nameLabel->setString("");
    }
    
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(ScienceListView::AnimationClose));
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
}

void ScienceListView::AnimationClose()
{
    if (m_clickType == 0) {
        ScienceListView::closeView(NULL);
    }
    else {
        if (m_clickType>=1 && m_clickType<=_countof(m_BtnInfo)) {
            int type = m_BtnInfo[m_clickType-1].m_iType;
            PopupViewController::getInstance()->addPopupInView(ScienceView::create(m_buildId, type));
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                                   , CCString::createWithFormat("SC1_%d", type));
        }
    }
}

void ScienceListView::closeView(CCObject* params)
{
    
    PopupViewController::getInstance()->goBackPopupView();
}

void ScienceListView::playBtnSmoke(float _time)
{
    return;
    //    auto btnSmoke1 = ParticleController::createParticle("ScienceSmoke");
    //    auto btnSmoke2 = ParticleController::createParticle("ScienceSmoke");
    //    auto btnSmoke3 = ParticleController::createParticle("ScienceSmoke");
    //    auto btnSmoke4 = ParticleController::createParticle("ScienceSmoke");
    //    auto btnSmoke5 = ParticleController::createParticle("ScienceSmoke");
    //
    //    btnSmoke1->setPosition(m_node1->getPosition());
    //    btnSmoke2->setPosition(m_node2->getPosition());
    //    btnSmoke3->setPosition(m_node3->getPosition());
    //    btnSmoke4->setPosition(m_node4->getPosition());
    //    btnSmoke5->setPosition(m_node5->getPosition());
    //
    //    addParticleToBatch(btnSmoke1);
    //    addParticleToBatch(btnSmoke2);
    //    addParticleToBatch(btnSmoke3);
    //    addParticleToBatch(btnSmoke4);
    //    addParticleToBatch(btnSmoke5);
}

void ScienceListView::addParticleToBatch(cocos2d::CCParticleSystemQuad *particle) {
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
    m_smokeLayer->addChild(newBatch);
    newBatch->addChild(particle);
    m_parVec.push_back(newBatch);
}

void ScienceListView::onEnter()
{
    CCNode::onEnter();
    
    CCLoadSprite::doResourceByCommonIndex(4, true);
    
    setTitleName(m_title.c_str());
    
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(ScienceListView::AnimationCallback));
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ScienceListView::stopCircleGlow), MSG_SCIENCE_RESEARCH_FINISH, NULL);
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void ScienceListView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SCIENCE_RESEARCH_FINISH);
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    m_smokeLayer->removeAllChildren();
    m_parVec.clear();
    if (false) {
        stopCircleGlow(NULL);
        this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
    }
    m_isInit = false;
    CCNode::onExit();
}

SEL_CCControlHandler ScienceListView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn1Click", ScienceListView::onBtn1Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn2Click", ScienceListView::onBtn2Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn3Click", ScienceListView::onBtn3Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn4Click", ScienceListView::onBtn4Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn5Click", ScienceListView::onBtn5Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn6Click", ScienceListView::onBtn6Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn7Click", ScienceListView::onBtn7Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn8Click", ScienceListView::onBtn8Click);
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClearCDClick", ScienceListView::onClearCDClick);
    return NULL;
}

bool ScienceListView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    //CCLOG("onAssignCCBMemberVariable :%s", pMemberVariableName);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scrollNode", CCNode*, this->m_scrollNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", CCNode*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", CCNode*, this->m_fireNode2);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, this->m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sceinceNode", CCNode*, this->m_sceinceNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_descriptionText", CCLabelIF*, this->m_descriptionText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", CCLabelIF*, this->m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeText", CCLabelIF*, this->m_timeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintText", CCLabelIF*, this->m_hintText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn", CCControlButton*, this->m_btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progressBG", CCScale9Sprite*, this->m_progressBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progrossBar", CCScale9Sprite*, this->m_progrossBar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgContainer", CCNode*, this->m_bgContainer);
    
    for (int i=0;i<_countof(m_BtnInfo);++i) {
        char szMemberName[128];
        
        sprintf(szMemberName, "m_icon%dNode", i+1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, szMemberName, CCNode*, m_BtnInfo[i].m_iconNode);
        
        sprintf(szMemberName, "m_name%dLabel", i+1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, szMemberName, CCLabelIF*, m_BtnInfo[i].m_nameLabel);
        
        sprintf(szMemberName, "m_btn%d", i+1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, szMemberName, CCControlButton*, m_BtnInfo[i].m_btn);
        
        sprintf(szMemberName, "m_node%d", i+1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, szMemberName, CCNode*, m_BtnInfo[i].m_node);
        
        sprintf(szMemberName, "m_crNode%d", i+1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, szMemberName, CCNode*, m_BtnInfo[i].m_crNode);
        
        sprintf(szMemberName, "m_name%dBG", i+1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, szMemberName, CCScale9Sprite*, m_BtnInfo[i].m_nameBG);
        
    }
    
    return false;
}

CCNode *ScienceListView::getGuideNode(std::string sid)
{
    if (m_isInit) {
        string posStr = CCCommonUtils::getPropById(sid, "position");
        int pos = atoi(posStr.c_str());
        pos -= 1;
        
        // 如果按钮顺序和位置是从左到右从上到下，那么就可以这样
        if (pos >= 0 && pos < _countof(m_BtnInfo)) {
            return m_BtnInfo[pos].m_btn;
        }
    }
    return NULL;
}

void ScienceListView::onBtn1Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    playClose(1);
}

void ScienceListView::onBtn2Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    playClose(2);
}

void ScienceListView::onBtn3Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    playClose(3);
}

void ScienceListView::onBtn4Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    playClose(4);
}

void ScienceListView::onBtn5Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    playClose(5);
}

void ScienceListView::onBtn6Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    playClose(6);
}

void ScienceListView::onBtn7Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    playClose(7);
}

void ScienceListView::onBtn8Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    playClose(8);
}

void ScienceListView::onClearCDClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    auto &map = GlobalData::shared()->scienceInfo.infoMap;
    for (auto it = map.begin(); it != map.end(); it++) {
        if(it->second.finishTime > GlobalData::shared()->getWorldTime()){
            PopupViewController::getInstance()->addPopupView(SciencePopupView::create(it->second.scienceId));
            return;
        }
    }
}

void ScienceListView::playCircleGlow()
{
    int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_SCIENCE);
    if (qid != QID_MAX) {
        string key = GlobalData::shared()->allQueuesInfo[qid].key;
        int sid = QueueController::getInstance()->getItemId(key);
        auto& info = GlobalData::shared()->scienceInfo.infoMap[sid];
        int sType = info.scienceType;
        string posStr = CCCommonUtils::getPropById(CC_ITOA(sType), "position");
        int pos = atoi(posStr.c_str());
        pos -= 1;
        
        if (pos >= 0 && pos < _countof(m_BtnInfo)) {
            //TODO:外大圆
            auto particle = ParticleController::createParticle("researching");
            particle->setScale(1.2);
            
            m_BtnInfo[pos].m_crNode->addChild(particle);
        }
        
        this->getAnimationManager()->runAnimationsForSequenceNamed("Loop");
    }
    else {
        this->getAnimationManager()->runAnimationsForSequenceNamed("Default");
    }
}

void ScienceListView::stopCircleGlow(CCObject* obj)
{
    // 移除齿轮高亮背景动画
    for (int i=0;i<_countof(m_BtnInfo);++i) {
        m_BtnInfo[i].m_crNode->removeAllChildren();
    }
}

