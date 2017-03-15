//
//  PrincessQuestController.cpp
//  IF
//
//  Created by 王超一 on 16/1/14.
//
//

#include "PrincessQuestController.hpp"
#include "GlobalData.h"
#include "CCSafeNotificationCenter.h"
#include "FunBuildController.h"
#include "ScienceController.h"
#include "PopupViewController.h"
#include "GuideController.h"
#include "SceneController.h"
#include "ImperialScene.h"
#include "WorldMapView.h"
#include "UIComponent.h"
#include "AppLibHelper.h"
#include "GCMRewardController.h"
#include "QueueController.h"
#include "ProductionSoldiersView.h"
#include "PrincessTaskRewardCommand.hpp"
static PrincessQuestController* _instance;

PrincessQuestController *PrincessQuestController::getInstance(){
    if(_instance == NULL){
        _instance = new PrincessQuestController();
    }
    return _instance;
}
void PrincessQuestController::purgeData() {
    if( _instance )
    {
    }
    CC_SAFE_RELEASE_NULL( _instance );
    _instance = NULL;
}
PrincessQuestController::PrincessQuestController()
{
    m_init = true;
    m_returnFromWorld = false;
    m_monsterFlag = false;
    tmpRewardId = 0;
}

PrincessQuestController::~PrincessQuestController() {
}

void PrincessQuestController::postQuestStateChangeNotification(){
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(PRINCESS_QUEST_STATE_UPDATE);
}

void PrincessQuestController::initQuest(CCArray* showTask){
    config.clear();
    isGettingReward = false;
    m_init = false;
    auto group = LocalController::shared()->DBXMLManager()->getGroupByKey("princess_quest");
    if(!group){
        return;
    }
    CCDictElement *ele;
    CCDICT_FOREACH(group, ele){
        std::string itemId = ele->getStrKey();
        CCDictionary *dict = dynamic_cast<CCDictionary*>(ele->getObject());
        if(dict == nullptr) {
            continue;
        }
        PrincessQuestInfo* quest = PrincessQuestInfo::create();
        quest->retain();
        quest->parse(dict);
        achList[itemId] = quest;
        map<int, int>::iterator it = achMap.find(quest->type);
        if(it!=achMap.end()){
            it->second = it->second+1;
        }else{
            achMap[quest->type] = 1;
        }
    }
    if(showTask){
        for (int i=0; i<showTask->count(); i++) {
            auto  dic = _dict(showTask->objectAtIndex(i));
            std::string itemId = dic->valueForKey("id")->getCString();
            map<std::string, PrincessQuestInfo*>::iterator it = achList.find(itemId);
            if(it!=achList.end()){
                it->second->parseFromServer(dic);
                it->second->maxStar = achMap[it->second->type];
                currentAchList[itemId] = it->second;
            }
        }
    }
}

void PrincessQuestController::updateTask(CCArray* tasks){
    if(tasks){
        std::string curItemId = "";
        for (int i=0; i<tasks->count(); i++) {
            auto  dic = _dict(tasks->objectAtIndex(i));
            std::string itemId = dic->valueForKey("id")->getCString();
            curItemId = itemId;
            map<std::string, PrincessQuestInfo*>::iterator it = currentAchList.find(itemId);
            if(it!=currentAchList.end()){
                int beforeS = it->second->state;
                it->second->parseFromServer(dic);
                int afterS = it->second->state;
                if (beforeS==0 && afterS==1) {
                    //完成一个任务;
                    if (it->second->guide != "") {
                        GuideController::share()->willGuide(it->second->guide);
                    }
                }
            }else{
                it = achList.find(itemId);
                if(it!=achList.end()){
                    int beforeS = it->second->state;
                    it->second->parseFromServer(dic);
                    map<int, int>::iterator itNum = achMap.find(it->second->type);
                    if(itNum!=achMap.end()){
                        it->second->maxStar = itNum->second;
                    }
                    currentAchList[itemId] = it->second;
                    
                    int afterS = it->second->state;
                    if (beforeS==0 && afterS==1) {
                        //完成一个任务;
                        if (it->second->guide != "") {
                            GuideController::share()->willGuide(it->second->guide);
                        }
                    }
                }
            }
        }
        
        if (currentAchList.find(curItemId)!=currentAchList.end() && currentAchList[curItemId]->state == PrincessComplete) {
            auto node = PopupViewController::getInstance()->getCurrentPopupView();
            UserUpgradeView* view = dynamic_cast<UserUpgradeView*>(node);
            if (view || node==NULL)
            {
                std::string guideId = "";
                map<std::string, PrincessQuestInfo*>::iterator it = achList.find(curItemId);
                if(it!=achList.end()){
                    guideId = it->second->guide;
                }
                if(guideId!="")
                {
                    GuideController::share()->setGuide(guideId);
                    if (view)
                    {
                        view->retain();
                        PopupViewController::getInstance()->removePopupView(view);
                        PopupViewController::getInstance()->pushPop(view, true);
                        view->release();
                    }
                }
            }
        }
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(PRINCESS_QUEST_STATE_UPDATE, CCInteger::create(0));
    }
}

void PrincessQuestController::getRewardSuccessHandle(std::string itemId){
    map<std::string, PrincessQuestInfo*>::iterator it = achList.find(itemId);
    if( it!=achList.end() )
    {
        if (itemId == "2100101") {///////////
            if (PopupViewController::getInstance()->getCurrViewCount() == 0) {
                UIComponent::getInstance()->CheckGuideUIShow();
            }
            AppLibHelper::sendAdjustTrack("tutorial_over");
            AppLibHelper::triggerEventCompletedTutorial();
        }
        currentAchList.erase(itemId);
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(PRINCESS_QUEST_STATE_UPDATE);
}

void PrincessQuestController::getReward(std::string itemId, bool isMain, bool showTip){
    isGettingReward = true;
    PrincessTaskRewardCommand *cmd = new PrincessTaskRewardCommand(itemId, showTip);
//    std::string md5Str = "";
//    md5Str.append(itemId).append(GlobalData::shared()->playerInfo.uid).append(QUESTRWDMD5);
//    MD5 md5;
//    md5.update(md5Str.c_str(), md5Str.length());
//    std::string md5Key = md5.toString();
//    cmd->putParam("sd", CCString::create(md5Key));
    cmd->sendAndRelease();
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(PrincessQuestController::resetGettingRewardFlag), this);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(PrincessQuestController::resetGettingRewardFlag), this, 5.0, 1, 0.0f, false);
}

void PrincessQuestController::resetGettingRewardFlag(float t){
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(PrincessQuestController::resetGettingRewardFlag), this);
    isGettingReward = false;
}

bool PrincessQuestController::goToQuestTarget(PrincessQuestInfo* quest, bool isSt){
    if(quest==NULL){
        return false;
    }
    bool ret = false;
    int type = quest->gotype;
    int itemId = quest->go;
    switch (type) {
        case 1:{
            
        }
        case 2:{
            if(SceneController::getInstance()->currentSceneId==SCENE_ID_WORLD && !isSt){
                m_returnFromWorld = true;
                AutoSafeRef temp(this);
                
                PopupViewController::getInstance()->removeAllPopupView();
                auto world = WorldMapView::instance();
                if (world) {
                    world->leaveWorld();
                }
            }else{
                
                m_returnFromWorld = false;
                auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
                if(layer) {
                    if(type==1){
                        itemId =layer->findCanBuildTile(itemId);
                        if(itemId==-1){
                            return ret;
                        }
                        ret = true;
                        layer->onMoveToBuildAndPlay(itemId);
                    }else{
                        
                        int limitLv = 0;
                        if (quest->classType == 8) {//建造 n个 m级的 建筑
                            int toLv1 = quest->para1 - itemId;
                            int toLv2 = quest->para1%100;
                            if (toLv1 == toLv2 && toLv1>1) {
                                limitLv = toLv1 - 1;
                            }
                        }
                        
                        int bid = FunBuildController::getInstance()->getMaxLvBuildByType(itemId, limitLv);
                        if(bid>0){
                            ret = true;
                            if (quest->classType != 2 && quest->classType != 22) {//不是造兵或者造陷阱的时候记录旧的镜头信息
                                layer->onMoveToBuildAndPlay(bid);
                                if (quest->classType == 14 || quest->classType == 1) {
                                    layer->setQuestEffect(2);
                                }
                                else
                                    layer->setQuestEffect(1);
                                FunBuildController::getInstance()->canPointArrow = true;
                                layer->showBuildBtns(bid);
                            }
                            
                            if (quest->classType == 2 || quest->classType == 22) { //造兵 造陷阱
                                layer->onMoveToBuildAndPlay(bid,false,false);
                                int needNum = quest->maxValue - quest->curValue;
                                string armyId = CC_ITOA(quest->para1);
                                
                                auto& build = FunBuildController::getInstance()->getFunbuildById(bid);
                                if (build.itemId>0 && build.state == FUN_BUILD_NORMAL)
                                {
                                    int qType = CCCommonUtils::getQueueTypeByBuildType(itemId);
                                    int qid1 = QueueController::getInstance()->getCanRecQidByType(qType);
                                    int qid2 = QueueController::getInstance()->canMakeItemByType(qType, 0);
                                    if (qid1 == QID_MAX && qid2 != QID_MAX) {
                                        ps_bid = bid;
                                        ps_needNum = needNum;
                                        ps_armyId = armyId;
                                        GuideController::share()->setLagStopTouch(1.0);
                                        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(PrincessQuestController::popupPSoilderView), this, 1.1,1, 0.0f, false);
                                    }
                                }
                                
                            }
                            
                        }
                    }
                }
            }
            break;
        }
        case 3:
            //layer->onMoveToBuildAndPlay(itemId);
            if (isSt) {
                break;
            }
            if(SceneController::getInstance()->currentSceneId==SCENE_ID_MAIN){
                if (quest->itemId == "2206801") {
                    m_monsterFlag = true;
                    int currentTime = WorldController::getInstance()->getTime()/1000;
                    CCUserDefault::sharedUserDefault()->setIntegerForKey(MSG_SHAKETIME, currentTime);
                    CCUserDefault::sharedUserDefault()->flush();
                    std::string isFirstPopKey = GlobalData::shared()->playerInfo.uid + "isFirstPop";
                    int isFirstPop = CCUserDefault::sharedUserDefault()->getIntegerForKey(isFirstPopKey.c_str(),0);
                    if (isFirstPop==0) {
                        WorldController::getInstance()->firstInWorld = true;
                        isFirstPop = 1;
                        CCUserDefault::sharedUserDefault()->setIntegerForKey(isFirstPopKey.c_str(), isFirstPop);
                        CCUserDefault::sharedUserDefault()->flush();
                    }
                }
                AutoSafeRef temp(this);
                
                PopupViewController::getInstance()->removeAllPopupView();
                SceneController::getInstance()->gotoScene(SCENE_ID_WORLD);
            }
            break;
        case 4:{
            if(SceneController::getInstance()->currentSceneId==SCENE_ID_WORLD){
                m_returnFromWorld = true;
                AutoSafeRef temp(this);
                
                PopupViewController::getInstance()->removeAllPopupView();
                auto world = WorldMapView::instance();
                if (world) {
                    world->leaveWorld();
                }
            }else{
                m_returnFromWorld = false;
                auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
                if(layer) {
                    layer->onMoveToSpeBuildAndPlay(itemId);
                }
            }
            break;
        }
        default:
            break;
    }
    return ret;
}

void PrincessQuestController::popupPSoilderView(float t)
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(PrincessQuestController::popupPSoilderView), this);
    auto view = ProductionSoldiersView::create(ps_bid);
    if (ps_needNum > 0) {
        view->setSoldierIdAndNum(ps_armyId, ps_needNum);
    }
    PopupViewController::getInstance()->addPopupInView(view);
}