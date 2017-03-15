//
//  AllianceDailyPublishView.cpp
//  IF
//
//  Created by 张军 on 15/10/20.
//
//

#include "AllianceDailyPublishView.h"
#include "AllianceDailyController.h"
#include "RewardController.h"
#include "WorldController.h"
#include "YesNoDialog.h"
#include "UseToolView.h"
#include "ToolController.h"
#include "PopupViewController.h"
#include "FunBuildController.h"
#include "GuideController.h"
#include "UIComponent.h"
#include "ChatServiceCocos2dx.h"
#include "ChatController.h"

#define cellW 620

//#define cellH 235
#define cellH 235
#define cellDH 45

int g_tab = 0;
bool  g_bpublish = true;  //放置点击速度过快引起的cell显示出错
const Color3B NameWhite = {234, 223, 210};
const Color3B NameGreen = {107, 153, 26};
const Color3B NameBlue = {33, 122, 179};
const Color3B NamePurple = {153, 82, 201};
const Color3B NameOrange = {179, 102, 33};
const Color3B NameYellow = {219, 176, 58};
const Color3B NameRed = {255, 0, 0};


AllianceDailyPublishView* AllianceDailyPublishView::create(){
    AllianceDailyPublishView* ret = new AllianceDailyPublishView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool AllianceDailyPublishView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);

    CCLoadSprite::doResourceByCommonIndex(305, true);
    CCLoadSprite::doResourceByCommonIndex(306, true);
    CCLoadSprite::doResourceByCommonIndex(505, true);
    CCLoadSprite::doResourceByCommonIndex(513, true);
    CCLoadSprite::doResourceByCommonIndex(502, true);
    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(305, false);
        CCLoadSprite::doResourceByCommonIndex(306, false);
        CCLoadSprite::doResourceByCommonIndex(505, false);
        CCLoadSprite::doResourceByCommonIndex(513, false);
        CCLoadSprite::doResourceByCommonIndex(502, false);
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    auto tmpCCB = CCBLoadFile("AllianceDailyPublishView",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    }else{
        this->setContentSize(tmpCCB->getContentSize());
    }
    if (!CCCommonUtils::isIosAndroidPad()) {
        int oldBgHeight = m_viewBg->getContentSize().height;
        changeBGMaxHeight(m_viewBg);
        int newBgHeight = m_viewBg->getContentSize().height;
        int addHeight = newBgHeight - oldBgHeight;
        int oldWidth = m_infoList_2->getContentSize().width;
        int oldHeight = m_infoList_2->getContentSize().height;
        m_infoList_2->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
        oldWidth = m_infoList_1->getContentSize().width;
        oldHeight = m_infoList_1->getContentSize().height;
        m_infoList_1->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
        oldWidth = m_infoList_3->getContentSize().width;
        oldHeight = m_infoList_3->getContentSize().height;
        m_infoList_3->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
        m_bottomNode->setPositionY(m_bottomNode->getPositionY()-addHeight);
    }
    int extH = getExtendHeight();
    
    auto tbg = CCLoadSprite::loadResource("technology_09.png");
    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
    int maxHight = CCDirector::sharedDirector()->getWinSize().height;
    int width = CCDirector::sharedDirector()->getWinSize().width;
    int curHight = -maxHight/2.0f;
    while (curHight<maxHight) {
        auto bg = CCLoadSprite::createSprite("technology_09.png");
        bg->setAnchorPoint(ccp(0, 1));
        bg->setPosition(ccp(-width/2.0f, curHight));
        if (CCCommonUtils::isIosAndroidPad())
        {
            bg->setScaleX(1536 * 1.0 / 640);
        }
        curHight += bg->getContentSize().height;
        bg->runAction(CCFadeIn::create(0.5));
        tBatchNode->addChild(bg);
    }
    m_totalNode->addChild(tBatchNode);
    m_totalNode->setPositionY(m_totalNode->getPositionY()-extH);
    
    infoListH = m_infoList_2->getContentSize().height;
    m_bottomNode->setVisible(false);

    
    CCCommonUtils::setButtonTitle(m_downBtn3, _lang("134028").c_str());//发布新任务
    CCCommonUtils::setButtonTitle(m_downBtn1, _lang("134022").c_str());//刷新任务
    CCCommonUtils::setButtonTitle(m_downBtn2, _lang("134017").c_str());//已发布任务
    CCCommonUtils::setButtonTitle(m_tabBtn1, _lang("134003").c_str());//帮助列表
    CCCommonUtils::setButtonTitle(m_tabBtn2, _lang("134017").c_str());//已发布任务
    CCCommonUtils::setButtonTitle(m_tabBtn3, _lang("134018").c_str());//宝藏列表
    
    m_viewType=3;
    m_waitInterface = NULL;
    b_cmdcontral = true;
    m_flyuuid = "";
    m_mytasknum = 10;
    g_tab = 3;
    b_fly = true;
    g_bpublish = true;
    m_particleEff = NULL;
    m_mytasknum = AllianceDailyController::getInstance()->getMydailyTaskNum();
    m_tasknum->setString(CC_ITOA(m_mytasknum));
    if (m_mytasknum == 0) {
        m_infogreen->setVisible(false);
    }else{
        m_infogreen->setVisible(true);
    }
    //初始化免费队列
    getFreeQueueTime();
    updateMineQueueinfo();
    updateRefreshTime();
    cellPalyAni=false;
    b_fly = false;
    

    m_tabView_1 = CCTableView::create(this, m_infoList_1->getContentSize());
    m_tabView_1->setDirection(kCCScrollViewDirectionVertical);
    m_tabView_1->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView_1->setTouchPriority(Touch_Popup);
    m_tabView_1->setDelegate(this);
    m_tabView_1->setTag(11);
    m_infoList_1->addChild(m_tabView_1);

    m_tabView_2 = CCTableView::create(this, m_infoList_2->getContentSize());
    m_tabView_2->setDirection(kCCScrollViewDirectionVertical);
    m_tabView_2->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView_2->setTouchPriority(Touch_Popup);
    m_tabView_2->setDelegate(this);
    m_tabView_2->setTag(12);
    m_infoList_2->addChild(m_tabView_2);
    
    m_tabView_3 = CCTableView::create(this, m_infoList_3->getContentSize());
    m_tabView_3->setDirection(kCCScrollViewDirectionVertical);
    m_tabView_3->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView_3->setTouchPriority(Touch_Popup);
    m_tabView_3->setDelegate(this);
    m_tabView_3->setTag(13);
    m_infoList_3->addChild(m_tabView_3);

    return true;
}

void AllianceDailyPublishView::onEnter()
{
    CCNode::onEnter();
    UIComponent::getInstance()->showPopupView(1);
    m_titleTxt->setString(_lang("134000").c_str());//联盟任务
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceDailyPublishView::updateTimesTip), MSG_NOTIME_TIP, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceDailyPublishView::updateInfo), OnGetDailyQuests, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceDailyPublishView::updateSendInfo), OnGetDailySendQuests, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceDailyPublishView::refreshTableViewData), AllianceDailyRefreshTableViewData, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceDailyPublishView::noticeRemoveInterFace), "noticeRemoveInterFace", NULL);
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceDailyPublishView::getfreeQueueInfo), AllianceMineQueue, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceDailyPublishView::CellFlyAniCall), cellflyAnimationCall, NULL);

    //获取数据
    AllianceDailyController::getInstance()->isNumTip=false;//取消完成跳角标提示
    showInterFaceCover(1);
    AllianceDailyController::getInstance()->startGetSendQuests();
    this->schedule(schedule_selector(AllianceDailyPublishView::updateTime), 1);//维护队列信息
    setData();
}

void AllianceDailyPublishView::onExit()
{
    AllianceDailyController::getInstance()->isNumTip=false;//取消完成跳角标提示
    showInterFaceCover(0);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, OnGetDailyQuests);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, OnGetDailySendQuests);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, AllianceDailyRefreshTableViewData);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "noticeRemoveInterFace");
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, cellflyAnimationCall);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_NOTIME_TIP);
    CCNode::onExit();
}

void AllianceDailyPublishView::updateTimesTip(CCObject *p){
    g_bpublish = true;
}

void AllianceDailyPublishView::CellFlyAniCall(CCObject *p){
    auto dic = dynamic_cast<CCDictionary*>(p);
    if(dic){
        string uuid = dic->valueForKey("uuid")->getCString();
        if (uuid != "") {
            m_flyuuid = uuid;
        }
        string cx = dic->valueForKey("cx")->getCString();
        string cy = dic->valueForKey("cy")->getCString();
        if (cx != "" && cy != "") {
            FlyToMine(std::atoi(cx.c_str()),std::atoi(cy.c_str()));
        }
    }
}

void AllianceDailyPublishView::FlyToMine(int x,int y){
    b_fly = true;
    //进度条收缩
    double proscale = m_progressSpr->getScaleX();
    if( abs(m_progressSpr->getScaleX() - 1) < 0.001 || proscale < 0.00001) {
        CCActionInterval * scaleto = CCScaleTo ::create(0.01,1,1);
        CCActionInterval * scaleto1 = CCScaleTo ::create(0.8,0,1);
        CCCallFunc* prograssAni  = CCCallFunc::create(this, callfunc_selector(AllianceDailyPublishView::prograssAnicall));
        m_progressSpr->runAction(CCSequence::create(scaleto,scaleto1,prograssAni,NULL));
    }

    //飞至Mine
    CCPoint pd = ccp(0,0);
    CCPoint localP = m_particleNode->convertToNodeSpaceAR(ccp(x,y));
    m_particleNode->stopAllActions();
    PaticleEff1= ParticleController::createParticle(CCString::createWithFormat("RaisingStars_%d",2)->getCString());  //--sun粒子效果
    PaticleEff1->setVisible(true);
    PaticleEff1->setScale(3);
    PaticleEff1->setPosition(localP);
    m_particleNode->addChild(PaticleEff1);
    
    CCActionInterval *forward2 = CCMoveTo::create(0.05, localP);
    CCActionInterval *forward3 = CCMoveTo::create(0.4, pd);
    CCActionInterval *forward4 = CCMoveTo::create(0.05, pd);
    CCCallFunc* call  = CCCallFuncN::create(this, callfuncN_selector(AllianceDailyPublishView::getfreeQueueInfo));
    CCCallFunc* ParticlStopcall  = CCCallFunc::create(this, callfunc_selector(AllianceDailyPublishView::paticleStopcall));
    PaticleEff1->setPositionType(kCCPositionTypeGrouped);
    PaticleEff1->runAction(CCSequence::create(CCDelayTime::create(0.1),forward2,forward3,call,forward4,ParticlStopcall,NULL));
}

void AllianceDailyPublishView::prograssAnicall(){
//    CCActionInterval * scaleto = CCScaleTo ::create(0.1, 1);
//    CCActionInterval * scaleto1 = CCScaleTo ::create(0.5, 0);
//    m_progressSpr->runAction(CCSequence::create(scaleto,scaleto1,NULL));
}

void AllianceDailyPublishView::paticleStopcall(){
    g_bpublish = true;
    if(PaticleEff1){
        PaticleEff1->stopAllActions();
        PaticleEff1->setVisible(false);
        m_particleNode->removeChild(PaticleEff1);
    }
}

void AllianceDailyPublishView::getfreeQueueInfo(CCNode *p){
    string tempuuid = m_flyuuid;
    if(AllianceDailyController::getInstance()->freequeue_c == 1 && tempuuid != ""){//根据队列是否空闲，确定新发布任务是否占用空闲队列
        m_itemUuid = tempuuid;
        if( AllianceDailyController::getInstance()->DailySendMap.find(m_itemUuid) != AllianceDailyController::getInstance()->DailySendMap.end()){
            saveQuestInfo = AllianceDailyController::getInstance()->DailySendMap.find(m_itemUuid)->second;
        }
    }
    deleteInfoInSendMap(tempuuid);
    changeTaskNum();
}

void AllianceDailyPublishView::changeTaskNum(){
    m_mytasknum = AllianceDailyController::getInstance()->getMydailyTaskNum();
    m_tasknum->setString(CC_ITOA(m_mytasknum));
    if (m_mytasknum == 0) {
        m_infogreen->setVisible(false);
    }else{
        m_infogreen->setVisible(true);
    }
}

void AllianceDailyPublishView::deleteInfoInSendMap(string uuid){
    if(uuid == ""){
        return;
    }
    if( AllianceDailyController::getInstance()->DailySendMap.find(uuid) != AllianceDailyController::getInstance()->DailySendMap.end()){
        AllianceDailyController::getInstance()->DailySendMap.erase(uuid);
        vector<string>::iterator it = AllianceDailyController::getInstance()->DailySendMapData.begin();
        for(;it<AllianceDailyController::getInstance()->DailySendMapData.end();it++){
            if((*it)==uuid){
                AllianceDailyController::getInstance()->DailySendMapData.erase(it);
                refreshTableViewData(NULL);
                break;
            }
        }
    }
}

void AllianceDailyPublishView::updateTime(float t){
    if (m_viewType == 3) {
        updateMineQueueinfo();
        updateRefreshTime();
    }
}

void AllianceDailyPublishView::progressAni(float t){
}

double AllianceDailyPublishView::getfreequeueTotleTime(){
    double totaltime = 0;
    if (AllianceDailyController::getInstance()->freequeue_c) {
        if(AllianceDailyController::getInstance()->MyDailyMap.find(saveQuestInfo.m_uid) != AllianceDailyController::getInstance()->MyDailyMap.end()){
            totaltime = AllianceDailyController::getInstance()->MyDailyMap[saveQuestInfo.m_uid].m_vanishTime - AllianceDailyController::getInstance()->MyDailyMap[saveQuestInfo.m_uid].m_publishTime;
        }
    }else{
        totaltime = AllianceDailyController::getInstance()->freequeuetime_c - AllianceDailyController::getInstance()->freequeuestarttime_c;
    }
    if(totaltime < 0){
        totaltime = 0;
    }
    return totaltime;
}

double AllianceDailyPublishView::getfreequeueFinishTime(){
    double finishTime = 0;
    if (AllianceDailyController::getInstance()->freequeue_c) {
        if(AllianceDailyController::getInstance()->MyDailyMap.find(saveQuestInfo.m_uid) != AllianceDailyController::getInstance()->MyDailyMap.end()){
            finishTime = AllianceDailyController::getInstance()->MyDailyMap[saveQuestInfo.m_uid].m_vanishTime;
        }
    }else{
        finishTime = AllianceDailyController::getInstance()->freequeuetime_c;
    }
    if (finishTime < 0) {
        finishTime = 0;
    }
    return finishTime;
}

void AllianceDailyPublishView::getFreeQueueTime(){
    if(!AllianceDailyController::getInstance()->freequeue_c){
        freequeueTime = AllianceDailyController::getInstance()->freequeuetime_c;
    }else{
        freequeueTime = 0;
    }
}

void AllianceDailyPublishView::updateRefreshTime(){
    double leftTime = 0;
    double curTime = WorldController::getInstance()->getTime();
    static int cmdcnt = 0;
    double timetemp = AllianceDailyController::getInstance()->refreshTime;
    if (timetemp - curTime > 0) {
        leftTime = timetemp - curTime;
        if(leftTime < 0){
            return;
        }
        leftTime/=1000;
        b_cmdcontral = true;  //该变量控制命令的调用次数
        cmdcnt = 0;
    }else{
        //发送命令，请求新的数据
        if(b_cmdcontral){
            cmdcnt++;
            AllianceDailyController::getInstance()->startGetFreshInfo();
            if (cmdcnt >=3) {
                cmdcnt = 0;
                b_cmdcontral = false;
            }
        }
    }
//    m_freshtimelabel->setString(CC_SECTOA(leftTime));
    m_freshtimelabel->setString(_lang_1("134075",CC_SECTOA(leftTime)));
}

void AllianceDailyPublishView::updateMineQueueinfo(){ //"f7b127305a0b418ca17b6396591984e4"
    double leftTime = 0;
    float len = 0;
    double max = 0;
    double curTime = WorldController::getInstance()->getTime();
    max = AllianceDailyController::getInstance()->freequeueTotalTime;
    freequeueTime = AllianceDailyController::getInstance()->freequeuetime_c;
    if(!AllianceDailyController::getInstance()->freequeue_c){//免费队列被占用
        if (freequeueTime > curTime) {
            leftTime = freequeueTime - curTime;
            leftTime/=1000;
        }
        if(max != 0){
            max/=1000;
            max = MAX(max, 1);
            len = 1.0-leftTime/max;
            if(len>1) len = 1;
            if(len == 1){
                AllianceDailyController::getInstance()->freequeue_c = true;   //到时间，设置为空闲队列
            }else{
                AllianceDailyController::getInstance()->freequeue_c = false;  //任务未完成，占用队列
            }
        }
        if (b_fly) {
            m_progressSpr->setScaleX(len);
        }
        if (len == 1 ) {
            b_fly = false;
        }else{
            b_fly = true;
        }
        m_progressTxt->setString(_lang_1("134078",CC_SECTOA(leftTime)));
        m_progressTxt->setVisible(true);
        m_partiEffNode->setVisible(false);
    }else{//免费队列空闲
        m_progressSpr->setScaleX(1);
        m_progressTxt->setString(_lang("134079"));
        if(!m_particleEff){
            string tmpStart = "AllianceGlow_";
            for (int i=1; i<=2; i++) {
                m_particleEff = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
                m_particleEff->setScale(0.9);
                m_partiEffNode->addChild(m_particleEff);
            }
        }
        m_partiEffNode->setVisible(true);
    }
    int sendCount = AllianceDailyController::getInstance()->m_sendCount>0?AllianceDailyController::getInstance()->m_sendCount:0;
    m_timeTxt->setString(_lang_1("134077",CC_ITOA(sendCount)));
    m_freshtimelabel->setString(_lang("102164"));
}

void AllianceDailyPublishView::noticeRemoveInterFace(CCObject* p)
{
    showInterFaceCover(0);
}

void AllianceDailyPublishView::updateInfo(CCObject* p)
{
    showInterFaceCover(0);
    setViewByType(m_viewType);
}

void AllianceDailyPublishView::updateSendInfo(CCObject* p)
{
    showInterFaceCover(0);
    setViewByType(3);
}

void AllianceDailyPublishView::refreshTableViewData(CCObject* p)
{
    showInterFaceCover(0);
    
    int i = 0;
    if(p){
        i = dynamic_cast<CCInteger*>(p)->getValue();
    }
    if (i==1) {
        cellPalyAni=false;
        ////////
        setData();
        m_tabView_3->setVisible(true);
        float miny = m_tabView_3->minContainerOffset().y;
        CCPoint pos  = m_tabView_3->getContentOffset();
        m_tabView_3->reloadData();
        float mincurry = m_tabView_3->minContainerOffset().y;
        pos.y+=(-miny+mincurry);
        m_tabView_3->setContentOffset(pos);
        m_tabView_1->setVisible(false);
        m_tabView_2->setVisible(false);
        ////////
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("updataAllianceDailyPublishCell");
        return;
    }else{
        cellPalyAni=false;
    }
    
    setData();
    if (m_viewType==1) {
        m_tabView_1->setVisible(true);
        float miny = m_tabView_1->minContainerOffset().y;
        CCPoint pos  = m_tabView_1->getContentOffset();
        m_tabView_1->reloadData();
        float mincurry = m_tabView_1->minContainerOffset().y;
        pos.y+=(-miny+mincurry);
        m_tabView_1->setContentOffset(pos);
        m_tabView_2->setVisible(false);
        m_tabView_3->setVisible(false);
    }else if (m_viewType==2){
        m_tabView_2->setVisible(true);
        float miny = m_tabView_2->minContainerOffset().y;
        CCPoint pos  = m_tabView_2->getContentOffset();
        m_tabView_2->reloadData();
        float mincurry = m_tabView_2->minContainerOffset().y;
        pos.y+=(-miny+mincurry);
        m_tabView_2->setContentOffset(pos);
        m_tabView_1->setVisible(false);
        m_tabView_3->setVisible(false);
    }else if(m_viewType == 3){
        m_tabView_3->setVisible(true);
        float miny = m_tabView_3->minContainerOffset().y;
        CCPoint pos  = m_tabView_3->getContentOffset();
        m_tabView_3->reloadData();
        float mincurry = m_tabView_3->minContainerOffset().y;
        pos.y+=(-miny+mincurry);
        m_tabView_3->setContentOffset(pos);
        m_tabView_1->setVisible(false);
        m_tabView_2->setVisible(false);
    }
    cellPalyAni=false;
}

void AllianceDailyPublishView::setViewByType(int viewType){
    m_viewType=viewType;
    setData();
    if (m_viewType==1) {
        m_tabView_1->setVisible(true);
        m_tabView_1->reloadData();
        m_tabView_2->setVisible(false);
        m_tabView_3->setVisible(false);
        GuideController::share()->checkSubGuide("3200100");
    }else if (m_viewType==2){
        m_tabView_2->setVisible(true);
        m_tabView_2->reloadData();
        m_tabView_1->setVisible(false);
        m_tabView_3->setVisible(false);
        GuideController::share()->checkSubGuide("3200200");
    }else if(m_viewType == 3){
        m_tabView_3->setVisible(true);
        m_tabView_3->reloadData();
        m_tabView_1->setVisible(false);
        m_tabView_2->setVisible(false);
        GuideController::share()->checkSubGuide("3200300");
    }
}

void AllianceDailyPublishView::setData(){
    m_noCellBtn->setVisible(false);
    if (m_viewType==1) {
        m_data_1.clear();
        m_tabBtn1->setEnabled(false);
        m_tabBtn2->setEnabled(true);
        m_tabBtn3->setEnabled(true);
        m_queueNode->setVisible(false);
        m_bottomNode->setVisible(false);
        m_upNode->setVisible(true);
        
        m_subBg1->setVisible(false);
        m_subBg2->setVisible(true);
        m_subBg3->setVisible(false);
        m_subBgTxt2->setString(_lang("134046"));
        m_subBgTxt2->setColor({251,212,143});
        //设置数据-
        for(int i=0;i<AllianceDailyController::getInstance()->DailyMapData.size();i++){
            string uuid = AllianceDailyController::getInstance()->DailyMapData[i];
            //剔除自己的数据
            if (uuid != "") {
                if (AllianceDailyController::getInstance()->DailyMap.find(uuid) != AllianceDailyController::getInstance()->DailyMap.end()) {
                    AllianceDailyQuestInfo temp = AllianceDailyController::getInstance()->DailyMap.find(uuid)->second;
                    if (temp.m_publishUid==GlobalData::shared()->playerInfo.uid) {
                        continue;
                    }
                    m_data_1.push_back(AllianceDailyController::getInstance()->DailyMapData[i]);
                }
            }
        }
        if(m_data_1.size()<1){//没有可接的任务
            m_noCellText->setVisible(true);
            m_noCellText->setString(_lang("134043"));//目前没有可以接取的联盟任务，赶紧抢先去发布一个吧
            CCCommonUtils::setButtonTitle(m_noCellBtn, _lang("134028").c_str());//发布新任务
            m_noCellBtn->setVisible(true);
        }else{
            m_noCellText->setVisible(false);
            m_noCellText->setString("");
        }
    }else if (m_viewType==2){
        m_queueNode->setVisible(false);
        m_tabBtn1->setEnabled(true);
        m_tabBtn2->setEnabled(false);
        m_tabBtn3->setEnabled(true);
        m_bottomNode->setVisible(false);
        m_data_2.clear();
        m_upNode->setVisible(true);
        
        m_downBtn3->setVisible(true);
        m_downBtn1->setVisible(false);
        m_priceNode->setVisible(false);
        m_downBtn2->setVisible(false);
        m_countNode1->setVisible(false);
        m_countNode2->setVisible(false);
        
        m_subBg1->setVisible(false);
        m_subBg2->setVisible(false);
        m_subBg3->setVisible(true);
//        m_subBg2->setPositionY(138);
        m_subBgTxt3->setString(_lang("134047"));
        m_subBgTxt3->setColor({251,212,143});
        //设置数据-
        for(int i=0;i<AllianceDailyController::getInstance()->MyDailyMapData.size();i++){
            m_data_2.push_back(AllianceDailyController::getInstance()->MyDailyMapData[i]);
        }
        
        if(m_data_2.size()<1){//没有发布任何任务
            m_noCellText->setVisible(true);
            m_noCellText->setString(_lang("134044"));//你当前没有发布中的任务
            CCCommonUtils::setButtonTitle(m_noCellBtn, _lang("134028").c_str());//发布新任务
            m_noCellBtn->setVisible(true);
            m_downBtn3->setVisible(false);
        }else{
            m_noCellText->setVisible(false);
            m_noCellText->setString("");
            m_downBtn3->setVisible(true);
        }
    }else if(m_viewType==3){
        m_queueNode->setVisible(true);
        m_bottomNode->setVisible(true);
        m_tabBtn1->setEnabled(true);
        m_tabBtn2->setEnabled(true);
        m_tabBtn3->setEnabled(false);
        m_data_3.clear();
        m_upNode->setVisible(true);
        m_downBtn3->setVisible(false);
        m_downBtn1->setVisible(true);
        m_priceNode->setVisible(false);
        m_subBg1->setVisible(true);
        m_subBg2->setVisible(false);
        m_subBg3->setVisible(false);
//        m_subBg2->setPositionY(230);
        m_subBgTxt1->setString(_lang("134048"));
        m_subBgTxt1->setColor({251,212,143});
        //显示金币
        int gold = AllianceDailyController::getInstance()->getRefreshNeedGold();
        if(gold>0 && AllianceDailyController::getInstance()->m_refreshCount<1){
            if(gold==1111){gold=100;}
            m_priceNode->setVisible(true);
            CCCommonUtils::setButtonTitle(m_downBtn1, "");
            m_priceLabel->setString(CC_ITOA(gold));
            m_inBtnLabel->setString(_lang("134022"));
        }else{
            CCCommonUtils::setButtonTitle(m_downBtn1, _lang("134022").c_str());//刷新任务
        }
        m_downBtn2->setVisible(true);
        m_countNode1->setVisible(true);
        //设置数据-
        for(int i=0;i<AllianceDailyController::getInstance()->DailySendMapData.size();i++){
            m_data_3.push_back(AllianceDailyController::getInstance()->DailySendMapData[i]);
        }
        if(m_data_3.size() > 3){
            CCLog("sdssddfsfs");
        }
        
        if(m_data_3.size()<1){//没有可发布的任务
            m_noCellText->setVisible(true);
            m_noCellText->setString(_lang("134045"));//当前批次任务已经发布完，请刷新
        }else{
            m_noCellText->setVisible(false);
            m_noCellText->setString("");
        }
    }
    string txt1Str = _lang("134021");//剩余发布次数
    int sendCount = AllianceDailyController::getInstance()->m_sendCount>0?AllianceDailyController::getInstance()->m_sendCount:0;
    txt1Str += CC_ITOA(sendCount);
    m_addTxt1->setString(txt1Str);
    
    string txt2Str = _lang("134023");//免费刷新次数
    int refreshCount = AllianceDailyController::getInstance()->m_refreshCount>0?AllianceDailyController::getInstance()->m_refreshCount:0;
    txt2Str += CC_ITOA(refreshCount);
    m_addTxt2->setString(txt2Str);
    
    m_mytasknum = AllianceDailyController::getInstance()->getMydailyTaskNum();
    m_tasknum->setString(CC_ITOA(m_mytasknum));
    if (m_mytasknum == 0) {
        m_infogreen->setVisible(false);
    }else{
        m_infogreen->setVisible(true);
    }
    //过期重新获取
//    if(AllianceDailyController::getInstance()->refreshTime<WorldController::getInstance()->getTime()){
//        AllianceDailyController::getInstance()->startGetRecord();
//    }
}

void AllianceDailyPublishView::goBack(){
    cellPalyAni=false;
    PopupViewController::getInstance()->goBackPopupView();
}

CCSize AllianceDailyPublishView::cellSizeForTable(CCTableView *table)
{
//    if (CCCommonUtils::isIosAndroidPad()) {
//        return CCSize(cellW*2.4, cellH*2.4);
//    }
    return CCSize(cellW, cellH);
}
#pragma mark tableCellAtIndex
CCTableViewCell* AllianceDailyPublishView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    string itemUuid = "";
    if(table->getTag()==11){
        itemUuid =m_data_1[idx];
    }else if (table->getTag()==12){
        itemUuid =m_data_2[idx];
    }else if (table->getTag()==13){
        itemUuid =m_data_3[idx];
    }
    
    int aniIdx=0;
    if (cellPalyAni) {
        aniIdx=(idx+1);
    }
    AllianceDailyPublishCell* cell = (AllianceDailyPublishCell*)table->dequeueCell();
    if(cell){
        cell->setData(itemUuid,m_subType,m_viewType,aniIdx);
    }else{
        cell = AllianceDailyPublishCell::create(itemUuid,m_subType,m_viewType,aniIdx);
    }
    return cell;
}
ssize_t AllianceDailyPublishView::numberOfCellsInTableView(CCTableView *table)
{
    int number=0;
    if(table->getTag()==11){
        number =m_data_1.size();
    }else if (table->getTag()==12){
        number =m_data_2.size();
    }else if (table->getTag()==13){
        number =m_data_3.size();
    }
    return number;
}
void AllianceDailyPublishView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

void AllianceDailyPublishView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
}

SEL_CCControlHandler AllianceDailyPublishView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTabBtn3Click", AllianceDailyPublishView::onTabBtn3Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTabBtn1Click", AllianceDailyPublishView::onTabBtn1Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTabBtn2Click", AllianceDailyPublishView::onTabBtn2Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDownBtn1Click", AllianceDailyPublishView::onDownBtn1Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDownBtn2Click", AllianceDailyPublishView::onDownBtn2Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDownBtn3Click", AllianceDailyPublishView::onDownBtn3Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddBtn1Click", AllianceDailyPublishView::onAddBtn1Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddBtn2Click", AllianceDailyPublishView::onAddBtn2Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onNoCellBtnClick", AllianceDailyPublishView::onNoCellBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipBtnClick", AllianceDailyPublishView::onTipBtnClick);
    return NULL;
}

bool AllianceDailyPublishView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_partiEffNode", CCNode*, this->m_partiEffNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_freshtimelabel", CCLabelIF*, this->m_freshtimelabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList_3", CCNode*, this->m_infoList_3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList_2", CCNode*, this->m_infoList_2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList_1", CCNode*, this->m_infoList_1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCScale9Sprite*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progressSpr", CCScale9Sprite*, m_progressSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progressTxt", CCLabelIF*, m_progressTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeTxt", CCLabelIF*, m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode", CCNode*, m_particleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode1", CCNode*, m_particleNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_queueNode", CCNode*, m_queueNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upNode", CCNode*, this->m_upNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tabBtn1", CCControlButton*, this->m_tabBtn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tabBtn2", CCControlButton*, this->m_tabBtn2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tabBtn3", CCControlButton*, this->m_tabBtn3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tasknum", CCLabelIF*, this->m_tasknum);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downBtn1", CCControlButton*, this->m_downBtn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_priceNode", CCNode*, this->m_priceNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_priceLabel", CCLabelIF*, this->m_priceLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inBtnLabel", CCLabelIF*, this->m_inBtnLabel);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downBtn2", CCControlButton*, this->m_downBtn2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downBtn3", CCControlButton*, this->m_downBtn3);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_countNode1", CCNode*, this->m_countNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addBtn1", CCControlButton*, this->m_addBtn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addTxt1", CCLabelIF*, this->m_addTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_countNode2", CCNode*, this->m_countNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addBtn2", CCControlButton*, this->m_addBtn2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addTxt2", CCLabelIF*, this->m_addTxt2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noCellText", CCLabelIF*, m_noCellText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noCellBtn", CCControlButton*, m_noCellBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipBtn", CCControlButton*, m_tipBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNode", CCNode*, m_totalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infogreen", CCNode*, m_infogreen);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subBg1", CCNode*, m_subBg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subBg2", CCNode*, m_subBg2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subBg3", CCNode*, m_subBg3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subBgTxt1", CCLabelIF*, m_subBgTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subBgTxt2", CCLabelIF*, m_subBgTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subBgTxt3", CCLabelIF*, m_subBgTxt3);

    return false;
}

void AllianceDailyPublishView::onTabBtn1Click(CCObject * pSender, CCControlEvent pCCControlEvent){
    g_tab = 1;
    setViewByType(1);
}
void AllianceDailyPublishView::onTabBtn2Click(CCObject * pSender, CCControlEvent pCCControlEvent){
    g_tab = 2;
    setViewByType(2);
}
void AllianceDailyPublishView::onTabBtn3Click(CCObject * pSender, CCControlEvent pCCControlEvent){
    g_tab = 3;
    b_cmdcontral = true;  //使得数据可刷新，防止死锁
    updateTime(0);
    setViewByType(3);
}
void AllianceDailyPublishView::onDownBtn1Click(CCObject * pSender, CCControlEvent pCCControlEvent){
    if(AllianceDailyController::getInstance()->m_refreshCount<1){
        int gold = AllianceDailyController::getInstance()->getRefreshNeedGold();
        if (gold == 1111) {
            refreshByGold();
            return;
        }
        //是否花费金币进行刷新？
        YesNoDialog::showButtonAndGold(_lang("134032").c_str(), CCCallFunc::create(this, callfunc_selector(AllianceDailyPublishView::refreshByGold)), _lang("102148").c_str(), gold);
        return;
    }
    refreshByGold();
}

void AllianceDailyPublishView::refreshByGold(){
    
//    setData();
//    m_tabView_3->reloadData();
//
    cellPalyAni=true;
    showInterFaceCover(1);
    AllianceDailyController::getInstance()->startRefreshSendQuests();
    
}
void AllianceDailyPublishView::showInterFaceCover(int type){
    if(type==1){
        if(m_waitInterface==NULL){
            m_waitInterface = GameController::getInstance()->showWaitInterface(this);
            auto size = CCDirector::sharedDirector()->getWinSize();
            m_waitInterface->setPosition(ccp(size.width / 2, -size.height / 2));
        }
    }else if(type==0){
        if (m_waitInterface != NULL) {
            m_waitInterface->remove();
            m_waitInterface = NULL;
        }
    }
}

void AllianceDailyPublishView::onDownBtn2Click(CCObject * pSender, CCControlEvent pCCControlEvent){
    cellPalyAni=false;
    CCLOGFUNC();
    setViewByType(2);
}
void AllianceDailyPublishView::onDownBtn3Click(CCObject * pSender, CCControlEvent pCCControlEvent){
    CCLOGFUNC();
//    setViewByType(2);
    //获取可发任务数据
    showInterFaceCover(1);
    AllianceDailyController::getInstance()->startGetSendQuests();
}
void AllianceDailyPublishView::onAddBtn1Click(CCObject * pSender, CCControlEvent pCCControlEvent){
    PopupViewController::getInstance()->addPopupInView(UseToolView::create(USE_TOOL_allianceDaily_addSend,NULL,"134000"));
}
void AllianceDailyPublishView::onAddBtn2Click(CCObject * pSender, CCControlEvent pCCControlEvent){
    PopupViewController::getInstance()->addPopupInView(UseToolView::create(USE_TOOL_allianceDaily_addRefresh,NULL,"134000"));
}

void AllianceDailyPublishView::onNoCellBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent){//发布新任务
    //获取可发任务数据
    showInterFaceCover(1);
    AllianceDailyController::getInstance()->startGetSendQuests();
}

void AllianceDailyPublishView::onTipBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent){//发布新任务
    CCCommonUtils::showHelpShiftSingleFAQ("45217");//3717
}

#pragma mark cell
AllianceDailyPublishCell* AllianceDailyPublishCell::create(string itemId, int subType, int viewType, int aniIdx)
{
    auto ret = new AllianceDailyPublishCell();
    if (ret && ret->init(itemId, subType, viewType, aniIdx)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllianceDailyPublishCell::init(string itemId, int subType, int viewType, int aniIdx)
{
    bool ret = true;
    m_itemUuid = "";
    cellType = "";
    cellState = "";
    saveQuestInfo = NULL;
    playAni=0;
    m_particle = NULL;
    m_headImgNode = NULL;
    m_headImgNode = HFHeadImgNode::create();
    
    CCBLoadFile("AllianceDailyPublishCell",this,this);
    this->setContentSize(CCSize(cellW, cellH));

    CCCommonUtils::setButtonTitle(m_sendBtn, _lang("134019").c_str());//发布
    CCCommonUtils::setButtonTitle(m_speedBtn, _lang("104903").c_str());//加速
    CCCommonUtils::setButtonTitle(m_rewardBtn, _lang("137539").c_str());//领取奖励
    CCCommonUtils::setButtonTitle(m_startBtn, _lang("134058").c_str());//开始
    
    setData(itemId, subType, viewType, aniIdx);

    return ret;
}

void AllianceDailyPublishCell::setData(string itemId, int subType, int viewType, int aniIdx)
{
    m_itemUuid = itemId;
    m_viewType = viewType;
    playAni = aniIdx;
    playAnimation(playAni);
    if (aniIdx==0) {
        showData();
    }
}

string AllianceDailyPublishCell::getIconPic(string str){
    if (str == "134007") {
        return "daily_kingdoms.png";
    }else if(str == "134008"){
        return "daily_dragon.png";
    }else if(str == "134009"){
        return "daily_jazz.png";
    }else if(str == "134010"){
        return "daily_forest.png";
    }else if(str == "134011"){
        return "daily_shortmen.png";
    }else if(str == "134006"){
        return "daily_pirate.png";
    }else{
        return "daily_pirate.png";
    }
}

void AllianceDailyPublishCell::showData(){
    if(m_particle){
        m_particle->setVisible(true);
    }
    m_touchNode->setContentSize(CCSize(cellW, cellH));
    m_mainNode->setVisible(true);
    m_msgNode->setVisible(true);
    m_helplabel->setVisible(false);
    m_vanishTimeNode->setVisible(false);
    m_speedBtn->setVisible(false);
    m_rewardBtn->setVisible(false);
    m_startBtn->setVisible(false);
    m_sendBtn->setVisible(false);
    m_helpBtn->setVisible(false);
    m_sendStateTxt->setVisible(true);
    m_helpNode->setVisible(false);
    m_helpDesNode->setVisible(false);
    m_itemDesNode->setVisible(false);
    m_priceNode->setVisible(false);
    m_superNode->setVisible(false);
    m_handsNode->setVisible(false);
    
    exRwdId[0]="";
    exRwdId[1]="";
    for (int i = 0; i < 6; i++) {
        baseRwdId[i] = "";
        baseRwdIdother[i] = "";
    }
    string iconPath = "";
    if(AllianceDailyController::getInstance()->DailySendMap.find(m_itemUuid)!=AllianceDailyController::getInstance()->DailySendMap.end() && m_viewType==3){//可发
        saveQuestInfo = AllianceDailyController::getInstance()->DailySendMap.find(m_itemUuid)->second;
        cellType = "1";
        m_sendBtn->setVisible(true);
        m_vanishTimeNode->setVisible(true);
        m_msgLabel->setString(CC_SECTOA(saveQuestInfo.m_vanishTime/1000));
        iconPath=getIconPic(saveQuestInfo.m_nameid);
    }else if(AllianceDailyController::getInstance()->MyDailyMap.find(m_itemUuid)!=AllianceDailyController::getInstance()->MyDailyMap.end() && m_viewType==2){//已发
        saveQuestInfo = AllianceDailyController::getInstance()->MyDailyMap.find(m_itemUuid)->second;
        cellType = "2";
        iconPath=getIconPic(saveQuestInfo.m_nameid);
    }else if(AllianceDailyController::getInstance()->DailyMap.find(m_itemUuid)!=AllianceDailyController::getInstance()->DailyMap.end() && m_viewType==1){//可接
        saveQuestInfo = AllianceDailyController::getInstance()->DailyMap.find(m_itemUuid)->second;
        cellType = "3";
        if (saveQuestInfo.m_pic!="") {
            m_iconNode->removeAllChildren();
            string m_pic = saveQuestInfo.m_pic+".png";
            auto head = CCLoadSprite::createSprite(m_pic.c_str(), true, CCLoadSpriteType_HEAD_ICON);
            if(!head){
                head = CCLoadSprite::createSprite("g044.png");
            }
            CCCommonUtils::setSpriteMaxSize(head, 120);
            m_iconNode->addChild(head);
            if (CCCommonUtils::isUseCustomPic(saveQuestInfo.m_picVer))
            {
                if(m_headImgNode){
                    m_headImgNode->initHeadImgUrl2(m_iconNode, CCCommonUtils::getCustomPicUrl(saveQuestInfo.m_publishUid, saveQuestInfo.m_picVer), 1.0f, 120, true);
                }
            }
            auto sp = CCLoadSprite::createSprite("daily-icon_kuang1.png");
            if (sp) {
                m_iconNode->addChild(sp);
            }
        }else{   //当头像没有加载上时，使用默认头像
            m_iconNode->removeAllChildren();
            auto head = CCLoadSprite::createSprite("g044.png");;
            if(head){
                CCCommonUtils::setSpriteMaxSize(head, 120);
                m_iconNode->addChild(head);
            }
            auto sp = CCLoadSprite::createSprite("daily-icon_kuang1.png");
            if (sp) {
                m_iconNode->addChild(sp);
            }
        }
    }else{
        m_iconNode->removeAllChildren();
        auto head = CCLoadSprite::createSprite("g044.png");;
        if(head){
            CCCommonUtils::setSpriteMaxSize(head, 120);
            m_iconNode->addChild(head);
        }
        auto sp = CCLoadSprite::createSprite("daily-icon_kuang1.png");
        if (sp) {
            m_iconNode->addChild(sp);
        }
    }
    if (iconPath=="") {
        CCLOGFUNC();
    }
    string bgPath = CCCommonUtils::getToolBgByColor(atoi(saveQuestInfo.m_color.c_str()));
    auto bg = CCLoadSprite::createSprite(bgPath.c_str());
    CCCommonUtils::setSpriteMaxSize(bg, 120, true);
    bg->setScale(1.4);
    m_iconBgNode->removeAllChildren();
    m_iconBgNode->addChild(bg);
    if(m_viewType != 1){
        auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_EQUIP);
        m_iconNode->removeAllChildren();
        if(icon){
            m_iconNode->addChild(icon);
        }
    }
    m_nameLabel->setString(saveQuestInfo.m_name);
    if (saveQuestInfo.m_color=="0") {
        m_nameLabel->setColor(NameWhite);
    }else if (saveQuestInfo.m_color=="1"){
        m_nameLabel->setColor(NameGreen);
    }else if (saveQuestInfo.m_color=="2"){
        m_nameLabel->setColor(NameBlue);
    }else if (saveQuestInfo.m_color=="3"){
        m_nameLabel->setColor(NamePurple);
    }else if (saveQuestInfo.m_color=="4"){
        m_nameLabel->setColor(NameOrange);
    }else if (saveQuestInfo.m_color=="5"){
        m_nameLabel->setColor(NameYellow);
    }else if (saveQuestInfo.m_color=="6"){
        m_nameLabel->setColor(NameRed);
    }
    if (cellType=="3") {
        string nameStr = _lang_1("134005",saveQuestInfo.m_publishName.c_str());//134005={0}正在开采中…
        m_playerName->setString(nameStr.c_str());
        m_playerName->setVisible(false);
    }else{
        m_playerName->setString("");
    }
    //设置特效
    if(cellType == "1" && atoi(saveQuestInfo.m_color.c_str()) >= 3 ){
        showAnibyColor(saveQuestInfo.m_color);//加颜色特效
    }else{
        callback();   //去除颜色特效
    }
    
    //设置奖品
    if (cellType=="1" || cellType=="2") {
        m_queueNode->setVisible(false);
        m_hideMsgN1->setVisible(true);
        m_hideMsgN2->setVisible(true);
        m_playerName->setVisible(false);
        if (cellType == "1" && cellState != "1") {
            m_sendBtn->setEnabled(true);
            m_sendStateTxt->setString("");
        }
        m_priceNode->setVisible(false);
        if (AllianceDailyController::getInstance()->freequeue_c != 1) {
            int gold = AllianceDailyController::getInstance()->m_nextGold;
            m_sendBtn->setEnabled(true);
            CCCommonUtils::setButtonTitle(m_sendBtn, "");//花费金币可开采
            m_buyBtn->setString(_lang("134019"));
            m_priceBtn->setString(CC_ITOA(gold));
            m_priceNode->setVisible(true);
        }else{
            m_priceNode->setVisible(false);
            CCCommonUtils::setButtonTitle(m_sendBtn, _lang("134062").c_str());
        }
        
        m_N1TitleTxt->setString(_lang("134024").c_str());//发布奖励
        string baseRewardStr = saveQuestInfo.m_baseReward;
//        string baseRewardStr = "exp,0,4860|food,0,77760|goods,200201,6|goods,200604,2|goods,200201,6|goods,200604,2|";
        vector<string> baseRewardVec;
        CCCommonUtils::splitString(baseRewardStr, "|", baseRewardVec);//"exp,0,4860|food,0,77760|goods,200201,6|goods,200604,2|goods,200201,6|goods,200604,2|"
        int baseSize = baseRewardVec.size();
        for (int i=0; i<6; i++) {
            if (i<baseSize) {
                m_n1n[i]->setVisible(true);
                string str = baseRewardVec[i];
                vector<string> vec;
                CCCommonUtils::splitString(str, ",", vec);
                if (vec.size()==3) {
                    string rType = vec[0];
                    string rId = vec[1];
                    string rNum = CC_ITOA_K(atol(vec[2].c_str()));
                    if(rType != "exp"){
                        rNum = "x"+ rNum;
                    }
                    baseRwdId[i]=rId;
                    int tp = RewardController::getInstance()->getTypeByName(rType);
                    m_n1n[i]->removeChildByTag(1000);
                    if(R_GOODS==tp){//品质背景
                        string goodsColor = CCCommonUtils::getPropById(rId, "color");
                        string colorPath = CCCommonUtils::getToolBgByColor(atoi(goodsColor.c_str()));
                        CCSprite* colorSpr = CCLoadSprite::createSprite(colorPath.c_str());
                        CCCommonUtils::setSpriteMaxSize(colorSpr,50);
                        colorSpr->setPosition(m_n1Spr[i]->getPosition());
                        m_n1n[i]->addChild(colorSpr);
                        colorSpr->setTag(1000);
                    }
                    /////////
                    string icon_name = RewardController::getInstance()->getPicByType(tp, atoi(rId.c_str()));
                    CCSprite* picSpr = CCLoadSprite::createSprite(icon_name.c_str());
                    CCCommonUtils::setSpriteMaxSize(picSpr,50);
                    picSpr->setPosition(m_n1Spr[i]->getPosition());
                    /////////
                    auto& tool = ToolController::getInstance()->getToolInfoById(atoi(rId.c_str()));
                    if(tool.type==3 && !tool.getPara().empty()){
                        CCScale9Sprite* numBG = CCLoadSprite::createScale9Sprite("BG_quatnity.png");
                        numBG->setColor(CCCommonUtils::getItemColor(tool.color));
                        numBG->setOpacity(200);
                        numBG->setPreferredSize(CCSize(50*0.3, 50*0.3));
                        numBG->setPosition(m_n1Spr[i]->getPosition());
                        CCLabelBMFont* numIF = CCLabelBMFont::create(CC_ITOA_K(atol(tool.getPara().c_str())), "pve_fnt_boss.fnt");
                        CCSize numSize = numIF->getContentSize();
                        numIF->setScale(0.3);
                        numIF->setPosition(m_n1Spr[i]->getPosition());
                        m_n1n[i]->removeChildByTag(100);
                        m_n1n[i]->addChild(picSpr);
                        m_n1n[i]->addChild(numBG);
                        m_n1n[i]->addChild(numIF);
                        picSpr->setTag(100);
                        m_n1Txt[i]->setString(rNum);
                    }else{
                        m_n1n[i]->removeChildByTag(100);
                        m_n1n[i]->addChild(picSpr);
                        picSpr->setTag(100);
                        m_n1Txt[i]->setString(rNum);
                    }
                }
            }else{
                m_n1n[i]->setVisible(false);
            }
        }

        if (saveQuestInfo.m_acceptUid=="" && cellType=="2") {
            m_vanishTimeNode->setVisible(false);
        }
    }else if(cellType=="3") {//可接
        m_hideMsgN2->setVisible(true);
        m_hideMsgN2->setVisible(false);
        m_N1TitleTxt->setString(_lang("105114").c_str());//奖励
        string acceptRewardStr = saveQuestInfo.m_acceptReward;
        if(acceptRewardStr==""){
            AllianceDailyController::getInstance()->startGetRewardDetail(m_itemUuid);
            return;
        }
        
        vector<string> acceptRewardVec;
        CCCommonUtils::splitString(acceptRewardStr, "|", acceptRewardVec);
        int baseSize = acceptRewardVec.size();
        for (int i=0; i<6; i++) {
            if (i<baseSize) {
                m_n1n[i]->setVisible(true);
                string str = acceptRewardVec[i];
                vector<string> vec;
                CCCommonUtils::splitString(str, ",", vec);
                if (vec.size()==3) {
                    string rType = vec[0];
                    string rId = vec[1];
                    string rNum = CC_ITOA_K(atol(vec[2].c_str()));//vec[2];
                    if(rType != "exp"){
                        rNum = "x"+ rNum;
                    }
                    baseRwdIdother[i]= rId;
                    int tp = RewardController::getInstance()->getTypeByName(rType);
                    m_n1n[i]->removeChildByTag(1000);
                    if(R_GOODS==tp){//品质背景
                        string goodsColor = CCCommonUtils::getPropById(rId, "color");
                        string colorPath = CCCommonUtils::getToolBgByColor(atoi(goodsColor.c_str()));
                        CCSprite* colorSpr = CCLoadSprite::createSprite(colorPath.c_str());
                        CCCommonUtils::setSpriteMaxSize(colorSpr,50);
                        colorSpr->setPosition(m_n1Spr[i]->getPosition());
                        m_n1n[i]->addChild(colorSpr);
                        colorSpr->setTag(1000);
                    }
                    string icon_name = RewardController::getInstance()->getPicByType(tp, atoi(rId.c_str()));
                    CCSprite* picSpr = CCLoadSprite::createSprite(icon_name.c_str());
                    CCCommonUtils::setSpriteMaxSize(picSpr,50);
                    picSpr->setPosition(m_n1Spr[i]->getPosition());
                    /////////
                    auto& tool = ToolController::getInstance()->getToolInfoById(atoi(rId.c_str()));
                    if(tool.type==3 && !tool.getPara().empty()){
                        CCScale9Sprite* numBG = CCLoadSprite::createScale9Sprite("BG_quatnity.png");
                        numBG->setColor(CCCommonUtils::getItemColor(tool.color));
                        numBG->setOpacity(200);
                        numBG->setPreferredSize(CCSize(50*0.3, 50*0.3));
                        numBG->setPosition(m_n1Spr[i]->getPosition());
                        CCLabelBMFont* numIF = CCLabelBMFont::create(CC_ITOA_K(atol(tool.getPara().c_str())), "pve_fnt_boss.fnt");
                        CCSize numSize = numIF->getContentSize();
                        numIF->setScale(0.3);
                        numIF->setPosition(m_n1Spr[i]->getPosition());
                        m_n1n[i]->removeChildByTag(100);
                        m_n1n[i]->addChild(picSpr);
                        m_n1n[i]->addChild(numBG);
                        m_n1n[i]->addChild(numIF);
                        picSpr->setTag(100);
                        m_n1Txt[i]->setString(rNum);
                    }else{
                        m_n1n[i]->removeChildByTag(100);
                        m_n1n[i]->addChild(picSpr);
                        picSpr->setTag(100);
                        m_n1Txt[i]->setString(rNum);
                    }
                }
            }else{
                m_n1n[i]->setVisible(false);
            }
        }
        m_hideMsgN2->setVisible(false);
    }
    
    if (cellType=="2"){
        //设置协助按钮
        m_helpNode->removeAllChildren();
        m_helpNode->setVisible(true);
        CCSprite* picSpr = CCLoadSprite::createSprite("daily-icon_kuang2.png");
        picSpr->setPosition(CCPoint(40, 40));
        m_helpNode->addChild(picSpr);
        
        CCSprite* rwdSpr0 = dynamic_cast<CCSprite*>(m_n1n[0]->getChildByTag(100));
        CCSprite* rwdSpr1 = dynamic_cast<CCSprite*>(m_n1n[1]->getChildByTag(100));
        CCSprite* rwdSpr2 = dynamic_cast<CCSprite*>(m_n1n[2]->getChildByTag(100));
        CCSprite* rwdSpr3 = dynamic_cast<CCSprite*>(m_n1n[3]->getChildByTag(100));
        CCSprite* rwdSpr4 = dynamic_cast<CCSprite*>(m_n1n[4]->getChildByTag(100));
        CCSprite* rwdSpr5 = dynamic_cast<CCSprite*>(m_n1n[5]->getChildByTag(100));
        CCSprite* bgSpr0 = dynamic_cast<CCSprite*>(m_n1n[0]->getChildByTag(1000));
        CCSprite* bgSpr1 = dynamic_cast<CCSprite*>(m_n1n[1]->getChildByTag(1000));
        CCSprite* bgSpr2 = dynamic_cast<CCSprite*>(m_n1n[2]->getChildByTag(1000));
        CCSprite* bgSpr3 = dynamic_cast<CCSprite*>(m_n1n[3]->getChildByTag(1000));
        CCSprite* bgSpr4 = dynamic_cast<CCSprite*>(m_n1n[4]->getChildByTag(1000));
        CCSprite* bgSpr5 = dynamic_cast<CCSprite*>(m_n1n[5]->getChildByTag(1000));
        
        if(saveQuestInfo.m_acceptUid==""){
            picSpr->setVisible(true);
            if(rwdSpr0){
                CCCommonUtils::setSpriteGray(rwdSpr0, true);
            }
            if(rwdSpr1){
                CCCommonUtils::setSpriteGray(rwdSpr1, true);
            }
            if(rwdSpr2){
                CCCommonUtils::setSpriteGray(rwdSpr2, true);
            }
            if(rwdSpr3){
                CCCommonUtils::setSpriteGray(rwdSpr3, true);
            }
            if(rwdSpr4){
                CCCommonUtils::setSpriteGray(rwdSpr4, true);
            }
            if(rwdSpr5){
                CCCommonUtils::setSpriteGray(rwdSpr5, true);
            }
            if(bgSpr0){
                CCCommonUtils::setSpriteGray(bgSpr0, true);
            }
            if(bgSpr1){
                CCCommonUtils::setSpriteGray(bgSpr1, true);
            }
            if(bgSpr2){
                CCCommonUtils::setSpriteGray(bgSpr2, true);
            }
            if(bgSpr3){
                CCCommonUtils::setSpriteGray(bgSpr3, true);
            }
            if(bgSpr4){
                CCCommonUtils::setSpriteGray(bgSpr4, true);
            }
            if(bgSpr5){
                CCCommonUtils::setSpriteGray(bgSpr5, true);
            }
            m_N2TitleTxt->setColor(NameRed);
        }else{
            picSpr->setVisible(false);
            if(rwdSpr0){
                CCCommonUtils::setSpriteGray(rwdSpr0, false);
            }
            if(rwdSpr1){
                CCCommonUtils::setSpriteGray(rwdSpr1, false);
            }
            if(rwdSpr2){
                CCCommonUtils::setSpriteGray(rwdSpr2, false);
            }
            if(rwdSpr3){
                CCCommonUtils::setSpriteGray(rwdSpr3, false);
            }
            if(rwdSpr4){
                CCCommonUtils::setSpriteGray(rwdSpr4, false);
            }
            if(rwdSpr5){
                CCCommonUtils::setSpriteGray(rwdSpr5, false);
            }
            if(bgSpr0){
                CCCommonUtils::setSpriteGray(bgSpr0, false);
            }
            if(bgSpr1){
                CCCommonUtils::setSpriteGray(bgSpr1, false);
            }
            if(bgSpr2){
                CCCommonUtils::setSpriteGray(bgSpr2, false);
            }
            if(bgSpr3){
                CCCommonUtils::setSpriteGray(bgSpr3, false);
            }
            if(bgSpr4){
                CCCommonUtils::setSpriteGray(bgSpr4, false);
            }
            if(bgSpr5){
                CCCommonUtils::setSpriteGray(bgSpr5, false);
            }
            m_N2TitleTxt->setColor(NameGreen);
        }
    }
    m_touchNode->setContentSize(CCSize(cellW, cellH));

}

void AllianceDailyPublishCell::showAnibyColor(string color){
    
    ////////////
//    string particleName = "Task_white";
//    switch (color) {
//        case 0:
//            particleName = "Task_white";
//            break;
//        case 1:
//            particleName = "Task_green";
//            break;
//        case 2:
//            particleName = "Task_blue";
//            break;
//        case 3:
//            particleName = "Task_purple";
//            break;
//        case 4:
//            particleName = "Task_org";
//            break;
//        case 5:
//            particleName = "Task_yell";
//            break;
//        default:
//            break;
//    }
//    auto particle = ParticleController::createParticle(CCString::createWithFormat(particleName.c_str())->getCString());
//    particle->setAnchorPoint(ccp(0.5, 0.5));
//    particle->setPosition(ccp(50, 50));
//    particle->setScale(1.2f);
//    m_allianceNode->addChild(particle);

    ////////////
    if (color!= "" && std::atoi(color.c_str()) >= 3 && std::atoi(color.c_str()) <= 6) {
//        m_superNode->setVisible(true);
//        m_lblSuper->setString(_lang("104933"));
        string shiningcolor;
        if(std::atoi(color.c_str()) == 3){
            shiningcolor = "Alliancebox_pur";
        }else if(std::atoi(color.c_str()) == 4){
            shiningcolor = "Alliancebox_org";
        }else if(std::atoi(color.c_str()) == 5){
            shiningcolor = "Alliancebox_gol";
        }
        if(!m_particle){
            m_particle = ParticleController::createParticle(shiningcolor);
            m_particle->setAnchorPoint(ccp(0.5, 0.5));
            m_particle->setScale(1.0f);
            m_shiningNode->addChild(m_particle);
        }
    }
}

void AllianceDailyPublishCell::onEnter() {
    CCNode::onEnter();
    setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceDailyPublishCell::freshData), OnCellRefreshData, NULL);
    if(cellType == "1"){
        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceDailyPublishCell::cellShapeShift), CELLSHAPESHIFT, NULL);
    }
    freshData(NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceDailyPublishCell::updateThisCellWithAni), "updataAllianceDailyPublishCell", NULL);
    freshData(NULL);
    this->schedule(schedule_selector(AllianceDailyPublishCell::updateTime), 1);
//    if (CCCommonUtils::isIosAndroidPad()) {
//        m_touchNode->setScale(2.4);
//    }else {
        m_touchNode->setScale(1);
//    }
    
}

void AllianceDailyPublishCell::onExit() {
    
    setTouchEnabled(false);
//    if (CCCommonUtils::isIosAndroidPad()) {
//        m_touchNode->setScale(2.4);
//    }else {
        m_touchNode->setScale(1);
//    }
    

    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, OnCellRefreshData);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "updataAllianceDailyPublishCell");
    if(cellType == "1"){
        CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, CELLSHAPESHIFT);
    }
    CCNode::onExit();
}

void AllianceDailyPublishCell::updateThisCellWithAni(CCObject* obj){
    int cellIndex = this->getIdx();
    if (AllianceDailyController::getInstance()->DailySendMapData.size()>cellIndex) {
        string itemUid = AllianceDailyController::getInstance()->DailySendMapData[cellIndex];
        if (AllianceDailyController::getInstance()->DailySendMap.find(itemUid)!=AllianceDailyController::getInstance()->DailySendMap.end()) {
            setData(itemUid, 2, 3, -1);
            int aniIndex = cellIndex+1;
            playAnimation(aniIndex);
        }
    }
}

void AllianceDailyPublishCell::updateTime(float t){
    freshData(NULL);
}

void AllianceDailyPublishCell::freshData(CCObject* obj){
    if(cellType=="1"){
        m_queueNode->setVisible(false);
        m_priceNode->setVisible(false);
        if (saveQuestInfo.m_stat=="1") {
            m_sendBtn->setEnabled(false);
            CCCommonUtils::setButtonTitle(m_sendBtn, _lang("134020").c_str());//发布中
            m_vanishTimeNode->setVisible(false);
            //计算时间
            if(AllianceDailyController::getInstance()->MyDailyMap.find(saveQuestInfo.m_uid)!=AllianceDailyController::getInstance()->MyDailyMap.end()){
                double leftTime = 0;
                double curTime = WorldController::getInstance()->getTime();
                if (AllianceDailyController::getInstance()->MyDailyMap[saveQuestInfo.m_uid].m_vanishTime > curTime) {
                    leftTime = AllianceDailyController::getInstance()->MyDailyMap[saveQuestInfo.m_uid].m_vanishTime - curTime;
                    leftTime/=1000;
                }
                m_progressTxt2->setString(CC_SECTOA(leftTime));  //已经不要了
            }
        }else{
            m_progressTxt2->setVisible(false);
            if(!AllianceDailyController::getInstance()->freequeue_c){
                int gold = AllianceDailyController::getInstance()->m_nextGold;
                m_sendBtn->setEnabled(true);
                CCCommonUtils::setButtonTitle(m_sendBtn, "");//花费金币可开采
                m_buyBtn->setString(_lang("134019"));
                m_priceBtn->setString(CC_ITOA(gold));
                m_priceNode->setVisible(true);
            }else{
                m_priceNode->setVisible(false);
                m_sendBtn->setVisible(true);
                m_sendBtn->setEnabled(true);
                CCCommonUtils::setButtonTitle(m_sendBtn, _lang("134062").c_str());//免费开采
            }
        }
    }
    if(cellType=="2"){
        m_queueNode->setVisible(false);
        m_priceNode->setVisible(false);
        m_sendStateTxt->setVisible(false);
        double leftTime = 0;
        double curTime = WorldController::getInstance()->getTime();
        
        if (saveQuestInfo.m_vanishTime > curTime) {
            leftTime = saveQuestInfo.m_vanishTime - curTime;
            leftTime/=1000;
        }
        if (saveQuestInfo.m_acceptUid=="") {
            if (saveQuestInfo.m_vanishTime > curTime) {
                cellState="1";//没被领且没到时间
                m_handsNode->setVisible(true);
                m_sendStateTxt->setVisible(false);//todo
//                m_sendStateTxt->setString(_lang("134020"));//开采中
                m_rewardBtn->setVisible(true);
                m_rewardBtn->setEnabled(false);
                string str = _lang("134025");//额外奖励
                m_N2TitleTxt->setString(str);
                double max = saveQuestInfo.m_vanishTime - saveQuestInfo.m_publishTime;
                max/=1000;
                max = MAX(max, 1);
                float len = 1.0-leftTime/max;
                if(len>1) len = 1;
                m_progressTxt->setVisible(true);
                m_progressTxt->setColor(ccc3(225, 0, 0));
                m_progressTxt->setString(_lang_1("134072",CC_SECTOA(leftTime)));
                m_helpBtn->setVisible(true);
                m_helplabel->setVisible(true);
                m_helplabel->setColor(ccc3(225, 0, 0));
                m_helplabel->setString(_lang("134071").c_str()); //..todo 赶快帮助吧
            }else{
                cellState="3";//没被领且到时间（可领奖）
                m_vanishTimeNode->setVisible(false);
                m_rewardBtn->setVisible(true);
                m_rewardBtn->setEnabled(false);
                m_progressTxt->setVisible(false);
                m_helplabel->setVisible(false);
                m_sendStateTxt->setVisible(true);
//                m_sendStateTxt->setString(_lang("108805"));//已过期
                //这个位置应该删除该cell，因为没有人帮助
                
                this->unschedule(schedule_selector(AllianceDailyPublishCell::updateTime));
            }
        }else{
            if (saveQuestInfo.m_vanishTime > curTime) {//saveQuestInfo.m_finishTime > curTime
                cellState="2";//被领且没到时间
                m_helpNode->setVisible(false);
                m_handsNode->setVisible(false);
                double max = saveQuestInfo.m_vanishTime - saveQuestInfo.m_publishTime;
                max/=1000;
                max = MAX(max, 1);
                float len = 1.0-leftTime/max;
                if(len>1) len = 1;
                m_progressTxt->setColor(ccc3(0, 225, 0));
                m_progressTxt->setString(_lang_1("134076",CC_SECTOA(leftTime)));
//                m_progressTxt->setString(CC_SECTOA(leftTime));
                m_progressTxt->setVisible(true);
//                m_sendStateTxt->setVisible(true);
//                m_sendStateTxt->setString(_lang("134020"));//完成中  采集中。。。
                m_rewardBtn->setEnabled(false);
                m_rewardBtn->setVisible(true);
                string str = _lang("134027") + ":" + saveQuestInfo.m_acceptName;
                m_playerName->setString(str.c_str());//134060 协助中
                m_playerName->setVisible(true);
            }else{
                m_helpNode->setVisible(false);
                m_handsNode->setVisible(false);
                cellState="4";//被领且到时间（可领奖）
                m_vanishTimeNode->setVisible(false);
                m_rewardBtn->setVisible(true);
                m_rewardBtn->setEnabled(true);
                m_msgLabel->setVisible(false);
                m_progressTxt->setVisible(false);
                m_playerName->setVisible(false);
                this->unschedule(schedule_selector(AllianceDailyPublishCell::updateTime));
            }
            m_vanishTimeNode->setVisible(false);
        }
    }else if (cellType=="3"){
        m_queueNode->setVisible(true);
        m_priceNode->setVisible(false);
        m_sendStateTxt->setVisible(false);
        m_progressTxt2->setVisible(true);
        m_playerfinish->setVisible(false);
        m_progressTxt2->setString(_lang_1("134005",saveQuestInfo.m_publishName.c_str()));
        double leftTime = 0;
        double curTime = WorldController::getInstance()->getTime();
        if (saveQuestInfo.m_acceptUid=="") {
            m_progressTxt3->setVisible(false);
            m_progressTxt->setVisible(false); //..todo
            if (saveQuestInfo.m_vanishTime > curTime) {
                cellState="1";//没被领且没到时间
                m_vanishTimeNode->setVisible(true);
                m_msgLabel->setString(CC_SECTOA(saveQuestInfo.m_finishTime/1000));//用时
                m_msgLabel->setVisible(true);
                m_startBtn->setVisible(true);
                m_queueNode->setVisible(false);
                if (AllianceDailyController::getInstance()->isHaveQuestAndNotFinish()){
                    m_startBtn->setEnabled(false);
                }else{
                    m_startBtn->setEnabled(true);
                }
                if(saveQuestInfo.m_publishUid == GlobalData::shared()->playerInfo.uid){
                    m_startBtn->setVisible(false);
                    m_sendStateTxt->setVisible(true);
                    m_sendStateTxt->setString(_lang("134052"));//无法领取
                }
            }else{
                cellState="3";//没被领且到时间（删除）
                this->unschedule(schedule_selector(AllianceDailyPublishCell::updateTime));
                if(AllianceDailyController::getInstance()->DailyMap.find(saveQuestInfo.m_uid)!=AllianceDailyController::getInstance()->DailyMap.end()){
                    AllianceDailyController::getInstance()->DailyMap.erase(AllianceDailyController::getInstance()->DailyMap.find(saveQuestInfo.m_uid));
                }
                //更新数据
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(AllianceDailyRefreshTableViewData);
            }
        }else{
            if (saveQuestInfo.m_finishTime > curTime) {
                cellState="2";//被领且没到时间
                leftTime = saveQuestInfo.m_finishTime - curTime;
                leftTime/=1000;
                double max = saveQuestInfo.m_finishTime - saveQuestInfo.m_startTime;
                max/=1000;
                max = MAX(max, 1);
                float len = 1.0-leftTime/max;
                if(len>1) len = 1;
                m_progressSpr->setScaleX(len);
                m_progressTxt3->setVisible(true);
                m_progressTxt3->setString(CC_SECTOA(leftTime));
                m_speedBtn->setVisible(true);
                string str = _lang_1("134060", saveQuestInfo.m_publishName.c_str());//134060=你正在协助{0}开采联盟物资
                m_playerName->setString(str.c_str());//134060 协助中
                m_playerName->setVisible(true);
            }else{
                cellState="4";//被领且到时间（可领奖）
                m_queueNode->setVisible(false);
                this->unschedule(schedule_selector(AllianceDailyPublishCell::updateTime));
                m_rewardBtn->setVisible(true);
                m_playerfinish->setString(_lang("134061"));//134061 开采完成
                m_playerfinish->setVisible(true);
                m_playerName->setVisible(false);
            }
            m_vanishTimeNode->setVisible(false);
        }
    }
}

void AllianceDailyPublishCell::SpeedUpByGold(){
    AllianceDailyController::getInstance()->startSpeedUp(m_itemUuid, "");
}

void AllianceDailyPublishCell::onSpeedBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    double max = saveQuestInfo.m_finishTime - saveQuestInfo.m_startTime;
    max/=1000;
    int maxTime = MAX(max, 1);
    double cur = saveQuestInfo.m_finishTime - WorldController::getInstance()->getTime();
    cur/=1000;
    int curTime = MAX(cur, 0);
    int gold = AllianceDailyController::getInstance()->getSpeedGoldByColor(saveQuestInfo.m_color);
    YesNoDialog::showTime( _lang("102283").c_str() , CCCallFunc::create(this, callfunc_selector(AllianceDailyPublishCell::SpeedUpByGold)), curTime, _lang("104903").c_str(), false, maxTime, gold);
}
void AllianceDailyPublishCell::onRewardBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    CCLOGFUNC();
    AllianceDailyController::getInstance()->startGetReward(m_itemUuid);
}

void AllianceDailyPublishCell::flycallback(){
    //飞过并实现view数据的更新
    auto dict = CCDictionary::create();
    CCPoint p = m_touchNode->getPosition();
    CCDirector::sharedDirector()->convertToGL(p);
    CCPoint p2 = m_touchNode->getParent()->convertToWorldSpace(p);
    string cx = CC_ITOA(p2.x);
    string cy = CC_ITOA(p2.y);
    dict->setObject(CCString::create(cx), "cx");
    dict->setObject(CCString::create(cy), "cy");
    dict->setObject(CCString::create(m_itemUuid), "uuid");
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(cellflyAnimationCall,dict);
}

void AllianceDailyPublishCell::cellShapeShift(CCObject* obj){
    if(cellType != "1"){
        return;
    }
    string tempuuid = dynamic_cast<CCString*>(obj)->getCString();
    if (tempuuid == "" || tempuuid != m_itemUuid) {
        return;
    }
    m_touchNode->stopAllActions();
    CCScaleTo* scaleTo1 = CCScaleTo::create(0.15,1.2,1.2);
    CCScaleTo* scaleTo2 = CCScaleTo::create(0.15,0.5,0.5);
    CCScaleTo* scaleTo3 = CCScaleTo::create(0.15,0.2,0.2);
    CCScaleTo* scaleTo4 = CCScaleTo::create(0.15,0,0);
    CCCallFunc* callRemoveShining  = CCCallFunc::create(this, callfunc_selector(AllianceDailyPublishCell::callback));
    CCCallFunc* call  = CCCallFunc::create(this, callfunc_selector(AllianceDailyPublishCell::flycallback));
    m_touchNode->runAction(CCSequence::create(CCDelayTime::create(0.3),scaleTo1,callRemoveShining,scaleTo2,scaleTo3,scaleTo4,call,NULL));
}

void AllianceDailyPublishCell::startSendQuestFun(){
    g_bpublish = false;
    int freequeue = AllianceDailyController::getInstance()->freequeue_c==1?1:0;
    AllianceDailyController::getInstance()->startSendQuests(m_itemUuid,0,freequeue);
    
}

void AllianceDailyPublishCell::onSendBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    if (!g_bpublish) {
        CCLOG("too fast!!!!");    //放置点击速度过快引起的cell显示出错
//        g_bpublish = true;
        return;
    }
    
    if(AllianceDailyController::getInstance()->sendFinishCount()>=AllianceDailyController::getInstance()->maxPublishRewardCount){
        YesNoDialog::show(_lang("134035").c_str(), NULL);//请先领取任务奖励后，再发布任务
        return;
    }
    if(!AllianceDailyController::getInstance()->freequeue_c){
        int gold = AllianceDailyController::getInstance()->m_nextGold;
        YesNoDialog::show(_lang_1("134063",CC_ITOA(gold)),CCCallFunc::create(this, callfunc_selector(AllianceDailyPublishCell::startSendQuestFun)));
        return;
    }else{
        int freequeue = AllianceDailyController::getInstance()->freequeue_c==true?1:0;
        AllianceDailyController::getInstance()->startSendQuests(m_itemUuid,0,freequeue);
        return;
    }
}

void AllianceDailyPublishCell::onHelpBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    //跳转联盟聊天界面发布信息
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    CCLOG("GlobalData::shared()->android_native_chat = %d", GlobalData::shared()->android_native_chat);
    if(!ChatServiceCocos2dx::isChatShowing){
        CCLOG("onChatBtnClick 1 showChatActivityFrom2dx");
        ChatServiceCocos2dx::m_channelType=CHANNEL_TYPE_ALLIANCE;
        ChatServiceCocos2dx::m_isNoticItemUsed=false;
        ChatServiceCocos2dx::showAllianceDialog();
        ChatServiceCocos2dx::showChatActivityFrom2dx();
    }
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
        CCLOG("onChatBtnClick 1 showChatActivityFrom2dx");
        ChatServiceCocos2dx::m_channelType=CHANNEL_TYPE_ALLIANCE;
        ChatServiceCocos2dx::CS_AllianceTask_Helped = true;
        ChatServiceCocos2dx::showChatIOSFrom2dx();
#endif
    
}

void AllianceDailyPublishCell::onStartBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    if(AllianceDailyController::getInstance()->m_acceptCount<1){
        YesNoDialog::show(_lang("134031").c_str(), NULL);//今日接取任务次数已经达到上限
        return;
    }
    if(AllianceDailyController::getInstance()->dailyFinishCount()>=AllianceDailyController::getInstance()->maxAcceptRewardCount){
        YesNoDialog::show(_lang("134030").c_str(), NULL);//请先领取任务奖励后，再接取任务
        return;
    }
    m_startBtn->setEnabled(false);
    //检测是否能接受该任务
    if(AllianceDailyController::getInstance()->DailyMap.find(m_itemUuid)!=AllianceDailyController::getInstance()->DailyMap.end()){
        double tmpCurTime = WorldController::getInstance()->getTime();
        if (AllianceDailyController::getInstance()->isHaveQuestAndNotFinish()) {
            //有任务正在进行
            YesNoDialog::show(" ", NULL);
        }else{
            if(AllianceDailyController::getInstance()->DailyMap[m_itemUuid].m_vanishTime>tmpCurTime){
                AllianceDailyController::getInstance()->startAcceptQuest(m_itemUuid);
            }else{
                //任务过期了
                YesNoDialog::show(_lang("134029").c_str(), NULL);//手慢啦，该任务已经被其他玩家领走
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(OnGetDailyQuests);//刷新列表
            }
        }
        
    }else{
        //任务被别人领
        YesNoDialog::show(_lang("134029").c_str(), NULL);//手慢啦，该任务已经被其他玩家领走
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(OnGetDailyQuests);//刷新列表
    }
}

SEL_CCControlHandler AllianceDailyPublishCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSpeedBtnClick", AllianceDailyPublishCell::onSpeedBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRewardBtnClick", AllianceDailyPublishCell::onRewardBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSendBtnClick", AllianceDailyPublishCell::onSendBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onStartBtnClick", AllianceDailyPublishCell::onStartBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHelpBtnClick", AllianceDailyPublishCell::onHelpBtnClick);
    return NULL;
}

bool AllianceDailyPublishCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_queueNode", CCNode*, m_queueNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblSuper", CCLabelIF*, m_lblSuper);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_superNode", CCNode*, m_superNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_handsNode", CCNode*, m_handsNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_speedBtn", CCControlButton*, m_speedBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardBtn", CCControlButton*, m_rewardBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sendBtn", CCControlButton*, m_sendBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpBtn", CCControlButton*, m_helpBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_startBtn", CCControlButton*, m_startBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sendStateTxt", CCLabelIF*, m_sendStateTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIF*, m_msgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helplabel", CCLabelIFTTF*, m_helplabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_playerName", CCLabelIFTTF*, m_playerName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_playerfinish", CCLabelIF*, m_playerfinish);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vanishTimeNode", CCNode*, m_vanishTimeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_shiningNode", CCNode*, m_shiningNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnLabel", CCLabelIF*, m_btnLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_priceNode", CCNode*, m_priceNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_priceBtn", CCLabelIF*, m_priceBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtn", CCLabelIF*, m_buyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progressSpr", CCScale9Sprite*, m_progressSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgNode", CCNode*, m_msgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconBgNode", CCNode*, m_iconBgNode);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hideMsgN1", CCNode*, m_hideMsgN1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_N1TitleTxt", CCLabelIF*, m_N1TitleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_n1n0", CCNode*, m_n1n[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_n1n1", CCNode*, m_n1n[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_n1n2", CCNode*, m_n1n[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_n1n3", CCNode*, m_n1n[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_n1n4", CCNode*, m_n1n[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_n1n5", CCNode*, m_n1n[5]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_n1Spr0", CCSprite*, m_n1Spr[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_n1Spr1", CCSprite*, m_n1Spr[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_n1Spr2", CCSprite*, m_n1Spr[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_n1Spr3", CCSprite*, m_n1Spr[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_n1Spr4", CCSprite*, m_n1Spr[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_n1Spr5", CCSprite*, m_n1Spr[5]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_n1Txt0", CCLabelIF*, m_n1Txt[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_n1Txt1", CCLabelIF*, m_n1Txt[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_n1Txt2", CCLabelIF*, m_n1Txt[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_n1Txt3", CCLabelIF*, m_n1Txt[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_n1Txt4", CCLabelIF*, m_n1Txt[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_n1Txt5", CCLabelIF*, m_n1Txt[5]);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hideMsgN2", CCNode*, m_hideMsgN2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_N2TitleTxt", CCLabelIF*, m_N2TitleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_n2n0", CCNode*, m_n2n[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_n2n1", CCNode*, m_n2n[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_n2Spr0", CCSprite*, m_n2Spr[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_n2Spr1", CCSprite*, m_n2Spr[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_n2Txt0", CCLabelIF*, m_n2Txt[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_n2Txt1", CCLabelIF*, m_n2Txt[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timego", CCSprite*, m_timego);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progressTxt", CCLabelIF*, m_progressTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progressTxt2", CCLabelIFTTF*, m_progressTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progressTxt3", CCLabelIF*, m_progressTxt3);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpNode", CCNode*, m_helpNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpDesNode", CCNode*, m_helpDesNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpDesTxt", CCLabelIFTTF*, m_helpDesTxt);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemDesNode", CCNode*, m_itemDesNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrowSprDes", CCSprite*, m_arrowSprDes);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNameTxt", CCLabelIF*, m_itemNameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemDesTxt", CCLabelIF*, m_itemDesTxt);
    
    return false;
}

void AllianceDailyPublishCell::playAnimation(int time){
    if (time==0) {
        return;
    }
    m_touchNode->stopAllActions();
//    if (CCCommonUtils::isIosAndroidPad()) {
//        m_touchNode->setScale(2.4);
//    }else {
        m_touchNode->setScale(1);
//    }
    
    playAni=0;
    CCScaleTo* scaleTo1 = CCScaleTo::create(0.15,1,1.2);
    CCScaleTo* scaleTo2 = CCScaleTo::create(0.15,1,0.2);
    CCScaleTo* scaleTo3 = CCScaleTo::create(0.1,1,1);
    CCCallFunc* call  = CCCallFunc::create(this, callfunc_selector(AllianceDailyPublishCell::callback));
    m_touchNode->runAction(CCSequence::create(CCDelayTime::create((time-1)*0.3),scaleTo1,call,scaleTo2,scaleTo3,CCCallFunc::create(this, callfunc_selector(AllianceDailyPublishCell::showData)), NULL));
}

void AllianceDailyPublishCell::callback(){
    if(m_particle){
        m_particle->setVisible(false);
        m_shiningNode->removeAllChildren();
        m_particle=NULL;
    }
    
}
void AllianceDailyPublishCell::showMsgs(){
    if (playAni!=0) {
        m_msgNode->setVisible(false);
        m_iconBgNode->setVisible(false);
    }else{
        m_msgNode->setVisible(true);
        m_iconBgNode->setVisible(true);
    }
}

bool AllianceDailyPublishCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    m_itemDesNode->setVisible(false);
    m_helpDesNode->setVisible(false);
    if(g_tab != m_viewType){
        return false;
    }
    if(isTouchInside(m_helpNode, pTouch)){
        return true;
    }
    if(isTouchInside(m_n1Spr[0], pTouch) && m_n1n[0]->isVisible()){
        return true;
    }
    if(isTouchInside(m_n1Spr[1], pTouch) && m_n1n[1]->isVisible()){
        return true;
    }
    if(isTouchInside(m_n1Spr[2], pTouch) && m_n1n[2]->isVisible()){
        return true;
    }
    if(isTouchInside(m_n1Spr[3], pTouch) && m_n1n[3]->isVisible()){
        return true;
    }
    if(isTouchInside(m_n1Spr[4], pTouch) && m_n1n[4]->isVisible()){
        return true;
    }
    if(isTouchInside(m_n1Spr[5], pTouch) && m_n1n[5]->isVisible()){
        return true;
    }
    return false;
}
void AllianceDailyPublishCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    string temp[6] = {"","","","","",""};
    if (g_tab == 1) {
        for (int i = 0;i < 6; i++) {
            temp[i] = baseRwdIdother[i];
        }
    }else{
        for (int i = 0;i < 6; i++) {
            temp[i] = baseRwdId[i];
        }
    }
    if(isTouchInside(m_helpNode, pTouch)){
        m_helpDesNode->setVisible(true);
        if(saveQuestInfo.m_acceptName==""){
            m_helpDesTxt->setString(_lang("134059").c_str());//如果在物资开采过程中，得到了联盟成员的协助，你将能获得额外奖励
            m_helpDesTxt->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        }else{
            string str = _lang("134027") + ":" + saveQuestInfo.m_acceptName;
            m_helpDesTxt->setString(str.c_str());//协助者（134027）：名字
            m_helpDesTxt->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        }
    }else if (isTouchInside(m_n1Spr[0], pTouch) && m_n1n[0]->isVisible()){
        if(temp[0]!="0"){
            m_itemDesNode->setVisible(true);
            m_itemDesNode->setPosition(ccp(0,-69));
            string name = _lang(CCCommonUtils::getPropById(temp[0], "name"));
            string description = _lang(CCCommonUtils::getPropById(temp[0], "description"));
            m_itemNameTxt->setString(name);
            m_itemDesTxt->setString(description);
            if (name != "" && description != "") {
                m_itemDesNode->setVisible(true);
//                if(CCCommonUtils::isIosAndroidPad()){
//                    m_itemDesNode->setPosition(ccp(0,40));
//                }else{
                    m_itemDesNode->setPosition(ccp(0,-69));
//                }
            }else{
                m_itemDesNode->setVisible(false);
            }

        }
        m_arrowSprDes->setPositionX(210);
    }else if (isTouchInside(m_n1Spr[1], pTouch)&& m_n1n[1]->isVisible()){
        if(temp[1]!="0"){
            m_itemDesNode->setVisible(true);
            m_itemDesNode->setPosition(ccp(0,-69));
            string name = _lang(CCCommonUtils::getPropById(temp[1], "name"));
            string description = _lang(CCCommonUtils::getPropById(temp[1], "description"));
            m_itemNameTxt->setString(name);
            m_itemDesTxt->setString(description);
            if (name != "" && description != "") {
                m_itemDesNode->setVisible(true);
//                if(CCCommonUtils::isIosAndroidPad()){
//                    m_itemDesNode->setPosition(ccp(0,40));
//                }else{
                    m_itemDesNode->setPosition(ccp(0,-69));
//                }
            }else{
                m_itemDesNode->setVisible(false);
            }

            
        }
        m_arrowSprDes->setPositionX(280);
    }else if (isTouchInside(m_n1Spr[2], pTouch)&& m_n1n[2]->isVisible()){
        if(temp[2]!="0"){  //奖励的id要更新
            string name = _lang(CCCommonUtils::getPropById(temp[2], "name"));
            string description = _lang(CCCommonUtils::getPropById(temp[2], "description"));
            m_itemNameTxt->setString(name);
            m_itemDesTxt->setString(description);
            if (name != "" && description != "") {
                m_itemDesNode->setVisible(true);
//                if(CCCommonUtils::isIosAndroidPad()){
//                    m_itemDesNode->setPosition(ccp(0,40));
//                }else{
                    m_itemDesNode->setPosition(ccp(0,-69));
//                }
            }else{
                m_itemDesNode->setVisible(false);
            }
        }
        m_arrowSprDes->setPositionX(353);
    }else if (isTouchInside(m_n1Spr[3], pTouch)&& m_n1n[3]->isVisible()){
        if(temp[3]!="0"){   //奖励的id要更新
            string name = _lang(CCCommonUtils::getPropById(temp[3], "name"));
            string description = _lang(CCCommonUtils::getPropById(temp[3], "description"));
            m_itemNameTxt->setString(name);
            m_itemDesTxt->setString(description);
            if (name != "" && description != "") {
                m_itemDesNode->setVisible(true);
//                if(CCCommonUtils::isIosAndroidPad()){
//                    m_itemDesNode->setPosition(ccp(0,40));
//                }else{
                    m_itemDesNode->setPosition(ccp(0,-69));
//                }
            }else{
                m_itemDesNode->setVisible(false);
            }
        }
        m_arrowSprDes->setPositionX(422);
    }else if (isTouchInside(m_n1Spr[4], pTouch)&& m_n1n[4]->isVisible()){
        if(temp[4]!="0"){   //奖励的id要更新
            string name = _lang(CCCommonUtils::getPropById(temp[4], "name"));
            string description = _lang(CCCommonUtils::getPropById(temp[4], "description"));
            m_itemNameTxt->setString(name);
            m_itemDesTxt->setString(description);
            if (name != "" && description != "") {
                m_itemDesNode->setVisible(true);
//                if(CCCommonUtils::isIosAndroidPad()){
//                    m_itemDesNode->setPosition(ccp(0,40));
//                }else{
                    m_itemDesNode->setPosition(ccp(0,-69));
//                }
            }else{
                m_itemDesNode->setVisible(false);
            }
        }
        m_arrowSprDes->setPositionX(495);
     }else if (isTouchInside(m_n1Spr[5], pTouch)&& m_n1n[5]->isVisible()){
        if(temp[5]!="0"){   //奖励的id要更新
            string name = _lang(CCCommonUtils::getPropById(temp[5], "name"));
            string description = _lang(CCCommonUtils::getPropById(temp[5], "description"));
            m_itemNameTxt->setString(name);
            m_itemDesTxt->setString(description);
            if (name != "" && description != "") {
                m_itemDesNode->setVisible(true);
//                if(CCCommonUtils::isIosAndroidPad()){
//                    m_itemDesNode->setPosition(ccp(0,40));
//                }else{
                    m_itemDesNode->setPosition(ccp(0,-69));
//                }
            }else{
                m_itemDesNode->setVisible(false);
            }
        }
        m_arrowSprDes->setPositionX(565);
    }
}
