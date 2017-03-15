//
//  AllianceDailyController.cpp
//  IF
//
//  Created by 张军 on 15/10/22.
//
//

#include "AllianceDailyController.h"
#include "AllianceDailyCommand.h"
#include "FunBuildController.h"
#include "WorldController.h"
#include "GCMRewardController.h"
#include "ToolController.h"
#include "UIComponent.h"
#include "CountryChatCommand.h"

static AllianceDailyController *_instance = NULL;

AllianceDailyController* AllianceDailyController::getInstance() {
    if (!_instance) {
        _instance = new AllianceDailyController();
    }
    return _instance;
}

void AllianceDailyController::purgeData() {
    if( _instance )
    {
        CCDirector::sharedDirector()->getScheduler()->unschedule(schedule_selector(AllianceDailyController::onEnterFrame), _instance);
    }
    CC_SAFE_RELEASE_NULL( _instance );
    _instance = NULL;
}

AllianceDailyController::AllianceDailyController(){
    isGetRecord=false;
    publishCount = -1;
    refreshCount = -1;
    maxPublishRewardCount = -1;
    maxPublishItemUseCount = -1;
    itemPublishUseCount = -1;
    refreshTime = -1;
    maxAcceptCount = -1;
    conditionPublishLevel = -1;
    acceptCount = -1;
    maxPublishCount = -1;
    maxAllPublishCount = -1;
    maxFreeRefreshCount = -1;
    maxAcceptRewardCount = -1;
    itemPublishCount = -1;
    conditionAcceptLevel = -1;
    itemRefreshCount = -1;
    clearGoldCost = "";//加速所需金币
    conditionTime = 0;//入盟所需时间 （秒）
    
    m_sendCount=0;
    m_nextGold = 0;
    m_finishCount=0;
    maxColorTip=-1;
    
    isNumTip=true;
    
    swDaily="";
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(AllianceDailyController::onEnterFrame), this, 5.0,kCCRepeatForever, 0.0f, false);
}

AllianceDailyController::~AllianceDailyController() {
    
}

void AllianceDailyController::onEnterFrame(float time){
    if(!isSwitchOpen()){
        CCDirector::sharedDirector()->getScheduler()->unschedule(schedule_selector(AllianceDailyController::onEnterFrame), this);
        return;
    }

    string allianceId = GlobalData::shared()->playerInfo.allianceInfo.uid;
    if(allianceId == ""){
        return;
    }
    
    if (dailyFinishCount()+sendFinishCount()!=m_finishCount) {
        if (dailyFinishCount()+sendFinishCount()>m_finishCount) {
            isNumTip=true;
        }
        m_finishCount = dailyFinishCount()+sendFinishCount();
    }
    if(m_finishCount>0 && isNumTip){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(AllianceDailyFinishAdd);
    }
    double tmpCurTime = WorldController::getInstance()->getTime();
    if(tmpCurTime>refreshTime){
        startGetRecord();
    }
    checkMaxTipColor();
}

void AllianceDailyController::onReturnErrorCode(CCDictionary* dict){
    string error = dict->valueForKey("errorCode")->getCString();
    string str = "";
    if(error == "134053"){
        str = _lang_1(error, CC_ITOA(conditionTime/3600));//加入联盟{}小时后才能发布任务
        CCCommonUtils::flyHint("", "", str.c_str());
    }else{
        str = _lang(error);
        CCCommonUtils::flyHint("", "", str.c_str());
    }
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("noticeRemoveInterFace");
}

#pragma mark获取已发任务（我和其他人发的）
void AllianceDailyController::startGetDailyQuests(){
    if(DailyMap.size()==0){
        AllianceDailyCommand *cmd = new AllianceDailyCommand();
        cmd->sendAndRelease();
    }else{
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(OnGetDailyQuests);
    }
}

void AllianceDailyController::EndGetDailyQuests(CCDictionary* dict){    //获取已发任务
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyHint("", "", _lang(error));
        return;
    }
    else
    {
        if (dict->objectForKey("taskList")) {
            CCArray *arr = dynamic_cast<CCArray*>(dict->objectForKey("taskList"));
            for (int i=0; i<arr->count(); i++) {
                CCDictionary *tempDict = dynamic_cast<CCDictionary*>(arr->objectAtIndex(i));
                AllianceDailyQuestInfo questInfo = AllianceDailyQuestInfo(tempDict);
                if(GlobalData::shared()->playerInfo.uid==questInfo.m_publishUid){
                    if (questInfo.m_publishStat=="0") {
                        MyDailyMap[questInfo.m_uid]=questInfo;
                    }
                }
//                else{
                    double tmpCurTime = WorldController::getInstance()->getTime();
                    if (questInfo.m_acceptUid=="" && questInfo.m_vanishTime>tmpCurTime) {
                        //任务没人领取 且 没有过期
                        DailyMap[questInfo.m_uid]=questInfo;
                    }else if(questInfo.m_acceptUid==GlobalData::shared()->playerInfo.uid && questInfo.m_acceptStat=="0"){
                        //任务被我领取 且 没领过奖
                        DailyMap[questInfo.m_uid]=questInfo;
                    }
//                }
            }
        }
        doOrderDailyMap();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(OnGetDailyQuests);
        if (dailyFinishCount()+sendFinishCount()!=m_finishCount) {
            m_finishCount = dailyFinishCount()+sendFinishCount();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(AllianceDailyFinishAdd);
        }
    }
}

#pragma mark 任务排序
void AllianceDailyController::doOrderDailyMap(){
    finishMap.clear();
    doingMap.clear();
    colorMap.clear();
    colorMapSend.clear();
    if (DailyMap.size()>0) {
        map<string,AllianceDailyQuestInfo>::iterator it = DailyMap.begin();
        for (; it!=DailyMap.end(); it++) {
            AllianceDailyQuestInfo &info = it->second;
            orderDataMap(info,1);
        }
        addDataByMap(DailyMapData);
    }
    if (MyDailyMap.size()>0) {
        map<string,AllianceDailyQuestInfo>::iterator it = MyDailyMap.begin();
        for (; it!=MyDailyMap.end(); it++) {
            AllianceDailyQuestInfo &info = it->second;
            orderDataMap(info,2);
        }
        addDataByMap(MyDailyMapData);
    }
}
void AllianceDailyController::doOrderSendMap(){
    finishMap.clear();
    doingMap.clear();
    colorMap.clear();
    colorMapSend.clear();
    if (DailySendMap.size()>0) {
        map<string,AllianceDailyQuestInfo>::iterator it = DailySendMap.begin();
        for (; it!=DailySendMap.end(); it++) {
            AllianceDailyQuestInfo &info = it->second;
            orderDataMap(info,3);
        }
        addDataByMap(DailySendMapData);
    }
}

void AllianceDailyController::orderDataMap(AllianceDailyQuestInfo &info,int orderType){
    if (orderType==1) {
        if(info.m_acceptUid==GlobalData::shared()->playerInfo.uid){//我接的任务
            if(info.m_finishTime<WorldController::getInstance()->getTime()){//完成的
                finishMap[info.m_finishTime] = info.m_uid;
            }else{//正在做的
                doingMap[info.m_finishTime] = info.m_uid;
            }
        }else{
            colorMap[info.m_color][info.m_vanishTime] = info.m_uid;
        }
    }else if (orderType==2){
        if(info.m_acceptUid!=""){//被接的任务
            colorMap[info.m_color][info.m_vanishTime] = info.m_uid;
        }else{//没被接的
            colorMapSend[info.m_color][info.m_uid] = info.m_uid;
        }
    }else{
        colorMapSend[info.m_color][info.m_uid] = info.m_uid;
    }
}

void AllianceDailyController::addDataByMap(vector<string> &data){
    data.clear();
    if(finishMap.size()>0){
        map<double,string>::iterator finishIt = finishMap.begin();
        for (; finishIt!=finishMap.end(); finishIt++) {
            data.push_back(finishIt->second);
        }
    }
    if(doingMap.size()>0){
        map<double,string>::iterator doingIt = doingMap.begin();
        for (; doingIt!=doingMap.end(); doingIt++) {
            data.push_back(doingIt->second);
        }
    }
    if (colorMap.size()>0) {
        map<string,std::map<double,string>>::iterator colorIt = colorMap.end();
        while(colorIt!=colorMap.begin()) {
            colorIt--;
            if (colorIt->second.size()>0) {
                map<double,string>::iterator colorSubIt = colorIt->second.begin();
                for (; colorSubIt!=colorIt->second.end(); colorSubIt++) {
                    data.push_back(colorSubIt->second);
                }
            }
        }
    }
    if(colorMapSend.size() >3){
//        return;   //进来过
    }
    if (colorMapSend.size()>0) {
        map<string,std::map<string,string>>::iterator colorIt = colorMapSend.end();
        while(colorIt!=colorMapSend.begin()) {
            colorIt--;
            if (colorIt->second.size()>0) {
                map<string,string>::iterator colorSubIt = colorIt->second.begin();
                for (; colorSubIt!=colorIt->second.end(); colorSubIt++) {
                    data.push_back(colorSubIt->second);
                }
            }
        }
    }
    finishMap.clear();
    doingMap.clear();
    colorMap.clear();
    colorMapSend.clear();
}

#pragma mark获取我的待发任务
void AllianceDailyController::startGetSendQuests(){
    if(DailySendMap.size()==0){
        AllianceGetSendQuestsCommand *cmd = new AllianceGetSendQuestsCommand();
        cmd->sendAndRelease();
    }else{
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(OnGetDailySendQuests);
    }
}

void AllianceDailyController::endGetSendQuests(CCDictionary* dict){   //获取我的待发任务
    if(dict->objectForKey("errorCode"))
    {
        onReturnErrorCode(dict);
        return;
    }
    else
    {
        DailySendMap.clear();
        DailySendMapData.clear();
        if(dict->objectForKey("record")){
            CCDictionary* recordDict = dynamic_cast<CCDictionary*>(dict->objectForKey("record"));
            //刷新数据 更新界面
            if(recordDict->objectForKey("refreshTime")){
                refreshTime = recordDict->valueForKey("refreshTime")->doubleValue();
            }
        }
        if (dict->objectForKey("taskList")) {
            CCArray *arr = dynamic_cast<CCArray*>(dict->objectForKey("taskList"));
            for (int i=0; i<arr->count(); i++) {
                CCDictionary *tempDict = dynamic_cast<CCDictionary*>(arr->objectAtIndex(i));
                AllianceDailyQuestInfo questInfo = AllianceDailyQuestInfo(tempDict);
                if (dict->objectForKey("gold")) {
                    m_nextGold = dict->valueForKey("gold")->intValue();
                }
                if (questInfo.m_stat=="0") {
                    DailySendMap[questInfo.m_uid]=questInfo;
//                    if(DailySendMap.size() >3){
//                        return;
//                    }
                }
            
            }
        }
        doOrderSendMap();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(OnGetDailySendQuests);
    }
}
#pragma mark刷新待发任务
void AllianceDailyController::startRefreshSendQuests(){    //刷新待发任务
    AllianceRefreshSendQuestsCommand *cmd = new AllianceRefreshSendQuestsCommand();
    cmd->sendAndRelease();
}
void AllianceDailyController::endRefreshSendQuests(CCDictionary* dict){
    if(dict->objectForKey("errorCode"))
    {
        onReturnErrorCode(dict);
        return;
    }
    else
    {
        if (dict->objectForKey("remainGold")) {
            int tmpInt = dict->valueForKey("remainGold")->intValue();
            UIComponent::getInstance()->updateGold(tmpInt);
        }
        DailySendMap.clear();
        DailySendMapData.clear();
        if (dict->objectForKey("taskList")) {
            CCArray *arr = dynamic_cast<CCArray*>(dict->objectForKey("taskList"));
            for (int i=0; i<arr->count(); i++) {
                CCDictionary *tempDict = dynamic_cast<CCDictionary*>(arr->objectAtIndex(i));
                AllianceDailyQuestInfo questInfo = AllianceDailyQuestInfo(tempDict);
                if (questInfo.m_stat=="0") {
                    DailySendMap[questInfo.m_uid]=questInfo;
//                    if (DailySendMap.size() > 3) {
//                        return;
//                    }
                }
                
            }
        }
        if(dict->objectForKey("record")){
            CCDictionary* recordDict = dynamic_cast<CCDictionary*>(dict->objectForKey("record"));
            //刷新数据 更新界面
            refreshUserData(recordDict,false);
        }
        doOrderSendMap();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(AllianceDailyRefreshTableViewData,CCInteger::create(1));
    }
}

#pragma mark发布任务
void AllianceDailyController::startSendQuests(string uid,int gold,int freequeue){
    GameController::getInstance()->showWaitInterface();
    AllianceSendQuestsCommand *cmd = new AllianceSendQuestsCommand(uid,gold,freequeue);
    cmd->sendAndRelease();
}
void AllianceDailyController::endSendQuests(CCDictionary* dict){   //发布任务
    GameController::getInstance()->removeWaitInterface();
    if(dict->objectForKey("errorCode"))
    {
        onReturnErrorCode(dict);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_NOTIME_TIP);
        return;
    }
    else
    {
        AllianceDailyQuestInfo questInfo = AllianceDailyQuestInfo(dict);
        m_nextGold = questInfo.m_nextgold;
        questInfo.m_publishName = GlobalData::shared()->playerInfo.name;
        if(GlobalData::shared()->playerInfo.uid==questInfo.m_publishUid){
            MyDailyMap[questInfo.m_uid]=questInfo;
            DailyMap[questInfo.m_uid]=questInfo;
        }else{
            DailyMap[questInfo.m_uid]=questInfo;
        }
        //从map中删除已经发布的cell
        if(AllianceDailyController::getInstance()->DailySendMap.find(questInfo.m_uid)!=AllianceDailyController::getInstance()->MyDailyMap.end()){
            updateFreeQueueInfo(questInfo);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(CELLSHAPESHIFT,CCString::create(questInfo.m_uid));
        }
        ////////////////////////
        DailySendMap[questInfo.m_uid].m_stat="1";
        doOrderDailyMap();
        //刷新主界面金币数目
        if (dict->objectForKey("remainGold")) {
            int tmpInt = dict->valueForKey("remainGold")->intValue();
            UIComponent::getInstance()->updateGold(tmpInt);
        }
        if(dict->objectForKey("record")){
            CCDictionary* recordDict = dynamic_cast<CCDictionary*>(dict->objectForKey("record"));
            //刷新数据 更新界面
            //refreshUserData(recordDict);
            if(recordDict->objectForKey("publishCount")){
                publishCount = recordDict->valueForKey("publishCount")->intValue();
            }
            if(recordDict->objectForKey("maxPublishCount")){
                maxPublishCount = recordDict->valueForKey("maxPublishCount")->intValue();
            }
            if(recordDict->objectForKey("itemPublishCount")){
                itemPublishCount = recordDict->valueForKey("itemPublishCount")->intValue();
            }
            if(dict->objectForKey("maxAllPublishCount")){
                maxAllPublishCount = dict->valueForKey("maxAllPublishCount")->intValue();
            }
            m_sendCount=maxAllPublishCount-publishCount+itemPublishCount;
        }
        //发聊天
        if(isChatShareOpen() && CCUserDefault::sharedUserDefault()->getBoolForKey("ALLIANCE_DAILY_CHAT_ON", true)){
            int q_color = atoi(questInfo.m_color.c_str());
            if (q_color>=3) {
                string msg = "";//颜色|dailog|任务名称| [{name:名字1},{name:名字1}]
                msg = questInfo.m_color + "|134054|" + CCCommonUtils::getPropById(questInfo.m_id, "name") + "|[{\"name\":\"" + GlobalData::shared()->playerInfo.name + "\"}]";
                CountryChatCommand * cmd = new CountryChatCommand(CHAT_STATE_ALLIANCE_COMMAND,msg.c_str(),CHAT_TYPE_ALLIANCETASK_SHARE);
                cmd->sendAndRelease();
            }
        }
//        string taskId = "";
//        if (dict->objectForKey("taskId")) {
//            taskId = dict->valueForKey("taksId")->getCString();
//        }
//        double Durtime = questInfo.m_vanishTime - questInfo.m_publishTime;
//        if(CCCommonUtils::isPushNotify(11)){
//        if(1){
//            cocos2d::extension::CCDevice::pushNotice(std::atoi(taskId.c_str()), (int)Durtime,_lang("134066"),"0","14");
//        }
    }
}

#pragma mark获取奖励详情
void AllianceDailyController::startGetRewardDetail(string uid){
    if(DailyMap.find(uid)!=DailyMap.end()){
        std::map<string,AllianceDailyQuestInfo>::iterator it = DailyMap.find(uid);
        if (it->second.m_acceptReward=="") {
            GameController::getInstance()->showWaitInterface();
            AllianceGetDetailCommand *cmd = new AllianceGetDetailCommand(uid);
            cmd->sendAndRelease();
        }else{
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(AllianceDailyRefreshTableViewData);
        }
        
    }
}
void AllianceDailyController::endGetRewardDetail(CCDictionary* dict){   //获取奖励
    GameController::getInstance()->removeWaitInterface();
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyHint("", "", _lang(error));
        return;
    }
    else
    {
        string acceptReward="";
        if(dict->objectForKey("acceptReward")){
            string temp =dict->valueForKey("acceptReward")->getCString();
            if (temp != "") {
                acceptReward = orderRewards(temp);
            }
        }
        string uid="";
        if(dict->objectForKey("uid")){
            uid=dict->valueForKey("uid")->getCString();
        }
        if (DailyMap.find(uid)!=DailyMap.end()) {
            DailyMap[uid].m_acceptReward=acceptReward;
        }
        //更新数据
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(AllianceDailyRefreshTableViewData);
    }
}
#pragma mark添加一条可接任务
void AllianceDailyController::addOneDailyQuest(CCDictionary* dict){
    if(DailyMap.size()!=0){
        AllianceDailyQuestInfo questInfo = AllianceDailyQuestInfo(dict);
        if(DailyMap.find(questInfo.m_uid) == DailyMap.end()){
            DailyMap[questInfo.m_uid] = questInfo;
        }
    }
    doOrderDailyMap();
}
#pragma mark更新一条可接任务(任务被人领走)
void AllianceDailyController::updateOneDailyQuest(CCDictionary* dict){
    string taskUid="";
    if (dict->objectForKey("taskUid")) {
        taskUid=dict->valueForKey("taskUid")->getCString();
    }
    string publishName="";
    if (dict->objectForKey("publishName")) {
        publishName=dict->valueForKey("publishName")->getCString();
    }
    string acceptName="";
    if (dict->objectForKey("acceptName")) {
        acceptName=dict->valueForKey("acceptName")->getCString();
    }
    string acceptUid="";
    if (dict->objectForKey("acceptUid")) {
        acceptUid=dict->valueForKey("acceptUid")->getCString();
    }
    double startTime=0;
    if (dict->objectForKey("startTime")) {
        startTime=dict->valueForKey("startTime")->doubleValue();
    }
    double finishTime=0;
    if (dict->objectForKey("finishTime")) {
        finishTime=dict->valueForKey("finishTime")->doubleValue();
    }
    if (publishName!=GlobalData::shared()->playerInfo.name && DailyMap.find(taskUid) != DailyMap.end()){//别人任务被领走
        DailyMap.erase(DailyMap.find(taskUid));
//        vector<string>::iterator it = DailyMapData.begin();
//        for(;it<DailyMapData.end();it++){
//            if((*it)==taskUid){
//                DailyMapData.erase(it);
//                break;
//            }
//        }
    }
    if (publishName==GlobalData::shared()->playerInfo.name && acceptUid!="" && MyDailyMap.find(taskUid) != MyDailyMap.end()){//我的任务被领走
        MyDailyMap[taskUid].m_acceptUid = acceptUid;
        MyDailyMap[taskUid].m_startTime = startTime;
        MyDailyMap[taskUid].m_finishTime = finishTime;
        MyDailyMap[taskUid].m_acceptName = acceptName;
    }
    doOrderDailyMap();
    checkMaxTipColor();
}

#pragma mark接任务
void AllianceDailyController::startAcceptQuest(string uid){
    GameController::getInstance()->showWaitInterface();
    acceptUid = uid;
    AllianceAcceptQuestCommand *cmd = new AllianceAcceptQuestCommand(uid);
    cmd->sendAndRelease();
}
void AllianceDailyController::endAcceptQuest(CCDictionary* dict){   //接任务
    GameController::getInstance()->removeWaitInterface();
    if(dict->objectForKey("errorCode"))
    {
        onReturnErrorCode(dict);
        return;
    }
    else
    {
        double startTime = 0;
        if(dict->objectForKey("startTime")){
            startTime = dict->valueForKey("startTime")->doubleValue();
        }
        double finishTime = 0;
        if(dict->objectForKey("finishTime")){
            finishTime = dict->valueForKey("finishTime")->doubleValue();
        }
        if(DailyMap.find(acceptUid)!=DailyMap.end()){
            DailyMap[acceptUid].m_acceptUid = GlobalData::shared()->playerInfo.uid;
            DailyMap[acceptUid].m_startTime = startTime;
            DailyMap[acceptUid].m_finishTime = finishTime;
        }
        doOrderDailyMap();
        if(dict->objectForKey("record")){
            CCDictionary* recordDict = dynamic_cast<CCDictionary*>(dict->objectForKey("record"));
            //刷新数据 更新界面
            refreshUserData(recordDict);
        }else{
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(AllianceDailyRefreshTableViewData);
        }
        
        AllianceDailyQuestInfo questInfo = AllianceDailyQuestInfo(dict);
        //发聊天
        if(isChatShareOpen() && CCUserDefault::sharedUserDefault()->getBoolForKey("ALLIANCE_DAILY_CHAT_ON", true)){
            int q_color = atoi(questInfo.m_color.c_str());
            if (q_color>=3) {
                //134055={0}刚刚发布的联盟任务{1}已经被{0}接取
//                string msg = _lang_3("134055",questInfo.m_publishName.c_str(),questInfo.m_name.c_str(),GlobalData::shared()->playerInfo.name.c_str());
                string msg = "";//颜色|dailog|任务名称| [{name:名字1},{name:名字1}]
                msg = questInfo.m_color + "|134055|" + CCCommonUtils::getPropById(questInfo.m_id, "name") + "|[{\"name\":\"" + questInfo.m_publishName + "\"},{\"name\":\"" + GlobalData::shared()->playerInfo.name + "\"}]";
                CountryChatCommand * cmd = new CountryChatCommand(CHAT_STATE_ALLIANCE_COMMAND,msg.c_str(),CHAT_TYPE_ALLIANCETASK_SHARE);
                cmd->sendAndRelease();
            }
        }
        //        string taskId = "";
        //        if (dict->objectForKey("taksId")) {
        //            taskId = dict->valueForKey("taksId")->getCString();
        //        }
//                double Durtime = questInfo.m_finishTime - questInfo.m_startTime;
        //        if(CCCommonUtils::isPushNotify(11)){
        //        if(1){
        //            cocos2d::extension::CCDevice::pushNotice(std::atoi(taskId.c_str()), (int)Durtime,_lang("134066"),"0","14");
        //        }
        
        checkMaxTipColor();
    }
    
}

#pragma mark领奖
void AllianceDailyController::startGetReward(string uid){
    GameController::getInstance()->showWaitInterface();
    rewardUid = uid;
    AllianceDailyGetRewardCommand *cmd = new AllianceDailyGetRewardCommand(uid);
    cmd->sendAndRelease();
}
void AllianceDailyController::endGetReward(CCDictionary* dict){    //领奖
    GameController::getInstance()->removeWaitInterface();
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyHint("", "", _lang(error));
        return;
    }
    else
    {
        if (dict->objectForKey("goods")) {
            CCArray* arr = dynamic_cast<CCArray*>(dict->objectForKey("goods"));
            GCMRewardController::getInstance()->flyToolReward(arr);
            //同步资源
            for (int i=0; i<arr->count(); i++) {
                CCDictionary *goodsDict = dynamic_cast<CCDictionary*>(arr->objectAtIndex(i));
                int itemId = goodsDict->valueForKey("itemId")->intValue();
                int num = goodsDict->valueForKey("count")->intValue();
                string uuid = goodsDict->valueForKey("uuid")->getCString();
                ToolController::getInstance()->addTool(itemId, num, uuid);
            }
        }
        CCArray* darr = GCMRewardController::getInstance()->retReward(dict);
        GCMRewardController::getInstance()->flyReward(darr);
        
        if(DailyMap.find(rewardUid)!=DailyMap.end()){
            DailyMap.erase(DailyMap.find(rewardUid));
            vector<string>::iterator it = DailyMapData.begin();
            for(;it<DailyMapData.end();it++){
                if((*it)==rewardUid){
                    DailyMapData.erase(it);
                    break;
                }
            }
        }else if (MyDailyMap.find(rewardUid)!=MyDailyMap.end()){
            MyDailyMap.erase(MyDailyMap.find(rewardUid));
            vector<string>::iterator it = MyDailyMapData.begin();
            for(;it<MyDailyMapData.end();it++){
                if((*it)==rewardUid){
                    MyDailyMapData.erase(it);
                    break;
                }
            }
        }
        //更新数据
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(AllianceDailyRefreshTableViewData);
        if (dailyFinishCount()+sendFinishCount()!=m_finishCount) {
            m_finishCount = dailyFinishCount()+sendFinishCount();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(AllianceDailyFinishAdd);
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(AllianceDailyGetRewards);
    }
}

#pragma mark加速
void AllianceDailyController::startSpeedUp(string taskUid,string itemUUid){
    GameController::getInstance()->showWaitInterface();
    AllianceDailySpeedUpCommand *cmd = new AllianceDailySpeedUpCommand(taskUid,itemUUid);
    cmd->sendAndRelease();
}
void AllianceDailyController::endSpeedUp(CCDictionary* dict){    //加速
    GameController::getInstance()->removeWaitInterface();
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyHint("", "", _lang(error));
        return;
    }
    else
    {
        if (dict->objectForKey("remainGold")) {
            int tmpInt = dict->valueForKey("remainGold")->intValue();
            UIComponent::getInstance()->updateGold(tmpInt);
        }
        if (dict->objectForKey("finish") && dict->valueForKey("finish")->boolValue()) {
            if (dict->objectForKey("task")) {
                CCDictionary *taskDict = dynamic_cast<CCDictionary*>(dict->objectForKey("task"));
                string uid="";
                if(taskDict->objectForKey("uid")){
                    uid=taskDict->valueForKey("uid")->getCString();
                }
                if (uid!="" && DailyMap.find(uid)!=DailyMap.end()) {
                    DailyMap[uid].setDataByDict(taskDict);
                }
                doOrderDailyMap();
                //取消信息推动
//                string taskId = "";
//                if (dict->objectForKey("taksId")) {
//                    taskId = dict->valueForKey("taksId")->getCString();
//                }
//                if(taskId != ""){
//                    cocos2d::extension::CCDevice::cancelNotice(std::atoi(taskId.c_str()));
//                }
                //更新数据
                
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(AllianceDailyRefreshTableViewData);
            }
        }
    }
}
#pragma mark获取记录信息
void AllianceDailyController::startGetRecord(){
    if(isGetRecord==false && isSwitchOpen()){
        AllianceDailyRecordCommand *cmd = new AllianceDailyRecordCommand();
        cmd->sendAndRelease();
    }
}
void AllianceDailyController::endGetRecord(CCDictionary* dict){    //获取记录信息
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyHint("", "", _lang(error));
        CCDirector::sharedDirector()->getScheduler()->unschedule(schedule_selector(AllianceDailyController::onEnterFrame), this);
        return;
    }
    else
    {
        m_nextGold = dict->valueForKey("gold")->intValue();
        refreshUserData(dict);
    }
}

bool AllianceDailyController::canAcceptQuests(){
    int mLv = FunBuildController::getInstance()->getMainCityLv();
    if (mLv>=conditionAcceptLevel) {
        return true;
    }
    return false;
}

bool AllianceDailyController::canSendQuests(){
    int mLv = FunBuildController::getInstance()->getMainCityLv();
    if (mLv>=conditionPublishLevel) {
        return true;
    }
    return false;
}
#pragma mark开关
bool AllianceDailyController::isSwitchOpen(){
    if(CCCommonUtils::isKuaFu()){//跨服战
        return false;
    }
    if(canSendQuests() && canAcceptQuests()){//等级
        if (swDaily=="1") {
            return true;
        }else if (swDaily=="0"){
            return false;
        }else if(CCCommonUtils::checkVersion(swDaily)){
            return true;
        }
    }
    return false;
}

bool AllianceDailyController::isChatShareOpen(){
//    return true;
    if (swDaily=="0"){
        return false;
    }else if(!CCCommonUtils::checkVersion(swDaily)){
        return false;
    }
    
    bool versionSw = false;
    if (swDailyChat=="1") {
        versionSw = true;
    }else if (swDailyChat=="0"){
        versionSw = false;
    }else if(CCCommonUtils::checkVersion(swDailyChat)){
        versionSw = true;
    }
    if(versionSw){
        return true;
    }
    return false;
}
#pragma mark退出联盟提示相关
//1.是否存在已经接受的任务
bool AllianceDailyController::isAccept(){
    map<string,AllianceDailyQuestInfo>::iterator it = DailyMap.begin();
    for (; it!=DailyMap.end() ; it++){
        if(it->second.m_acceptUid==GlobalData::shared()->playerInfo.uid){
            return true;
        }
    }
    return false;
}
//2.是否已经发布任务
bool AllianceDailyController::isSend(){
    if(MyDailyMap.size()>0){
        return true;
    }
    return false;
}
//是否正在进行任务
bool AllianceDailyController::isHaveQuestAndNotFinish(){
    map<string,AllianceDailyQuestInfo>::iterator it = DailyMap.begin();
    for (; it!=DailyMap.end() ; it++){
        if(it->second.m_acceptUid==GlobalData::shared()->playerInfo.uid && it->second.m_finishTime>WorldController::getInstance()->getTime()){
            return true;
        }
    }
    return false;
}

#pragma mark清空全部任务
void AllianceDailyController::removeAllMap(){
    DailyMap.clear();
    MyDailyMap.clear();
    DailySendMap.clear();
    
    DailyMapData.clear();
    MyDailyMapData.clear();
    DailySendMapData.clear();
    
    isGetRecord=false;
    publishCount = -1;
    refreshCount = -1;
    maxPublishRewardCount = -1;
    maxPublishItemUseCount = -1;
    itemPublishUseCount = -1;
    refreshTime = -1;
    maxAcceptCount = -1;
    conditionPublishLevel = -1;
    acceptCount = -1;
    maxPublishCount = -1;
    maxFreeRefreshCount = -1;
    maxAcceptRewardCount = -1;
    itemPublishCount = -1;
    conditionAcceptLevel = -1;
    itemRefreshCount = -1;
    clearGoldCost = "";//加速所需金币
    conditionTime = 0;//入盟所需时间 （秒）
    
    m_sendCount=0;
    m_nextGold = 0;
    m_finishCount=0;
    maxColorTip=-1;
    isNumTip=true;
}

//发布完成的任务（发布的任务可领奖）个数
int AllianceDailyController::sendFinishCount(){
    int count = 0;
    map<string,AllianceDailyQuestInfo>::iterator it = MyDailyMap.begin();
    for (; it!=MyDailyMap.end() ; it++){
        if(it->second.m_acceptUid!="" && it->second.m_vanishTime < WorldController::getInstance()->getTime()){
            count++;
        }
        if(it->second.m_acceptUid=="" && it->second.m_vanishTime < WorldController::getInstance()->getTime()){
            count++;
        }
    }
    return count;
}
//接收完成的任务（领的任务可领奖）个数
int AllianceDailyController::dailyFinishCount(){
    int count = 0;
    map<string,AllianceDailyQuestInfo>::iterator it = DailyMap.begin();
    for (; it!=DailyMap.end() ; it++){
        if(it->second.m_acceptUid==GlobalData::shared()->playerInfo.uid && it->second.m_finishTime < WorldController::getInstance()->getTime()){
            count++;
        }
    }
    return count;
}

#pragma mark计算刷新所需金币
int AllianceDailyController::getRefreshNeedGold(){
    if(itemRefreshCount>0){
        return 0;
    }else{
        int count = refreshCount-maxFreeRefreshCount;
        count = MAX(0, count);
        if(refreshGoldMap.find(count+1)!=refreshGoldMap.end()){
            return refreshGoldMap[count+1];
        }else{
//            int maxCount = refreshGoldMap.size();
//            return refreshGoldMap[maxCount];
            return 1111;
        }
    }
    return 0;
}

#pragma mark更新记录数据
void AllianceDailyController::refreshUserData(CCDictionary *dict,bool isSend/* =true*/){
    isGetRecord = true;
    if(dict->objectForKey("publishCount")){
        publishCount = dict->valueForKey("publishCount")->intValue();
    }
    if(dict->objectForKey("refreshCount")){
        refreshCount = dict->valueForKey("refreshCount")->intValue();
    }
    if(dict->objectForKey("maxPublishRewardCount")){
        maxPublishRewardCount = dict->valueForKey("maxPublishRewardCount")->intValue();
    }
    if(dict->objectForKey("maxPublishItemUseCount")){
        maxPublishItemUseCount = dict->valueForKey("maxPublishItemUseCount")->intValue();
    }
    if(dict->objectForKey("itemPublishUseCount")){
        itemPublishUseCount = dict->valueForKey("itemPublishUseCount")->intValue();
    }
    if(dict->objectForKey("refreshTime")){
        refreshTime = dict->valueForKey("refreshTime")->doubleValue();
    }else{
        isGetRecord = false;
    }
    if(dict->objectForKey("maxAcceptCount")){
        maxAcceptCount = dict->valueForKey("maxAcceptCount")->intValue();
    }
    if(dict->objectForKey("conditionPublishLevel")){
        conditionPublishLevel = dict->valueForKey("conditionPublishLevel")->intValue();
    }
    if(dict->objectForKey("acceptCount")){
        acceptCount = dict->valueForKey("acceptCount")->intValue();
    }
    if(dict->objectForKey("maxPublishCount")){
        maxPublishCount = dict->valueForKey("maxPublishCount")->intValue();
    }
    if(dict->objectForKey("maxAllPublishCount")){
        maxAllPublishCount = dict->valueForKey("maxAllPublishCount")->intValue();
    }
    if(dict->objectForKey("maxFreeRefreshCount")){
        maxFreeRefreshCount = dict->valueForKey("maxFreeRefreshCount")->intValue();
    }
    if(dict->objectForKey("maxAcceptRewardCount")){
        maxAcceptRewardCount = dict->valueForKey("maxAcceptRewardCount")->intValue();
    }
    if(dict->objectForKey("itemPublishCount")){
        itemPublishCount = dict->valueForKey("itemPublishCount")->intValue();
    }
    if(dict->objectForKey("conditionAcceptLevel")){
        conditionAcceptLevel = dict->valueForKey("conditionAcceptLevel")->intValue();
    }
    if(dict->objectForKey("itemRefreshCount")){
        itemRefreshCount = dict->valueForKey("itemRefreshCount")->intValue();
    }
    if(dict->objectForKey("freequeue")){
        freequeue_c = dict->valueForKey("freequeue")->intValue();
    }
    if(dict->objectForKey("freequeueTime")){
        freequeuetime_c = dict->valueForKey("freequeueTime")->doubleValue();
    }
    if(dict->objectForKey("freequeueStartTime")){
        freequeuestarttime_c = dict->valueForKey("freequeueStartTime")->doubleValue();
    }
    if(dict->objectForKey("refreshGoldCost")){
        refreshGoldCost = dict->valueForKey("refreshGoldCost")->getCString();
        vector<string> vec;
        CCCommonUtils::splitString(refreshGoldCost, "|", vec);
        for (int i=0;i<vec.size();i++)
        {
            string str = vec[i];
            vector<string> vec2;
            CCCommonUtils::splitString(str, ":", vec2);
            if (vec2.size()==2) {
                int count = atoi(vec2[0].c_str());
                int price = atoi(vec2[1].c_str());
                refreshGoldMap[count] = price;
            }
        }
    }
    //加速所需金币
    if(dict->objectForKey("clearGoldCost")){
        clearGoldCost = dict->valueForKey("clearGoldCost")->getCString();
    }
    
    //增加发布次数所需金币
    if(dict->objectForKey("publishGoldCost")){
        publishGoldCost = dict->valueForKey("publishGoldCost")->getCString(); //"1:200|2:300|3:500|4:800|5:1000|6:1000"
        vector<string> vec;
        CCCommonUtils::splitString(publishGoldCost, "|", vec);
        for (int i=0;i<vec.size();i++)
        {
            string str = vec[i];
            vector<string> vec2;
            CCCommonUtils::splitString(str, ":", vec2);
            if (vec2.size()==2) {
                int count = atoi(vec2[0].c_str());
                int price = atoi(vec2[1].c_str());
                publishGoldMap[count] = price;
            }
        }
    }
    
    //入盟所需时间 （秒）
    if(dict->objectForKey("conditionTime")){
        conditionTime = dict->valueForKey("conditionTime")->intValue();
    }
    
    m_sendCount=maxAllPublishCount-publishCount+itemPublishCount;
    m_acceptCount=maxAcceptCount-acceptCount;
    m_refreshCount=maxFreeRefreshCount-refreshCount+itemRefreshCount;
    if((maxFreeRefreshCount<refreshCount) && itemRefreshCount>0){
        m_refreshCount = itemRefreshCount;
    }
    freequeueTotalTime = freequeuetime_c - freequeuestarttime_c;  //免费队列所需时间
    //更新界面
    if(isSend){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(AllianceDailyRefreshTableViewData);
    }
}
//获取加速金币
int AllianceDailyController::getSpeedGoldByColor(string color){
//    clearGoldCost 0:100;1:150;2:200;3:400;4:800;5:1200;6:2000
    int gold = 0;
    vector<string> vec;
    CCCommonUtils::splitString(clearGoldCost, ";", vec);
    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        string subStr =(*it).c_str();
        vector<string> subVec;
        CCCommonUtils::splitString(subStr, ":", subVec);
        if (subVec.size()==2) {
            string sub1 = subVec[0];
            string sub2 = subVec[1];
            if (color == sub1) {
                gold = atoi(sub2.c_str());
                return gold;
            }
        }
    }
    
    return gold;
}

int AllianceDailyController::getPublishNeedGold(){
    if(isfreeQueue()){
        return 0;
    }else{
        int count = publishCount+itemPublishUseCount;
        count = MAX(0, count);
        if(publishGoldMap.find(count+1)!=publishGoldMap.end()){
            return publishGoldMap[count+1];
        }else{
            return 1000;
        }
    }
    return 0;
}

#pragma mark别人发布的品质最高的任务
int AllianceDailyController::otherSendMaxColor(){
    int maxColor = -1;
    map<string,AllianceDailyQuestInfo>::iterator it = DailyMap.begin();
    for (; it!=DailyMap.end() ; it++){
        if(it->second.m_acceptUid!=GlobalData::shared()->playerInfo.uid && it->second.m_publishUid!=GlobalData::shared()->playerInfo.uid){
            int tempColor = atoi(it->second.m_color.c_str());
            if(tempColor>maxColor){
                maxColor = tempColor;
            }
        }
    }
    return maxColor;
}


void AllianceDailyController::checkMaxTipColor(){
    if(isSwitchOpen()){
        int iColor = otherSendMaxColor();
        if (maxColorTip!=iColor) {
            maxColorTip=iColor;
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("addAllianceMaxColor",CCInteger::create(maxColorTip));
        }
    }
}

int AllianceDailyController::isfreeQueue(){
    int free = 1;
    std::map<string,AllianceDailyQuestInfo>::iterator it = MyDailyMap.begin();
    for (; it!=MyDailyMap.end(); it++)
    {
        auto& tmp = (*it).second;
        if(tmp.m_freequeue == 1){
            return free = 1;
        }
    }
    return free = 0;
}

string AllianceDailyController::getfreeQueueId(){
    string tmpid = "";
    std::map<string,AllianceDailyQuestInfo>::iterator it = MyDailyMap.begin();
    for (; it!=MyDailyMap.end(); it++)
    {
        auto& tmp = (*it).second;
        if(tmp.m_freequeue == 1){
            return tmp.m_id;
        }
    }
    return tmpid;
}

int AllianceDailyController::getMydailyTaskNum(){
    return MyDailyMap.size();
}

void AllianceDailyController::startGetFreshInfo(){
    AllianceDailyFreshCommand *cmd = new AllianceDailyFreshCommand();
    cmd->sendAndRelease();
}
void AllianceDailyController::endGetfreshInfo(CCDictionary* dict){    //获取刷新数据
    if(dict->objectForKey("errorCode"))
    {
        onReturnErrorCode(dict);
        return;
    }
    else
    {
        if (dict->objectForKey("remainGold")) {
            int tmpInt = dict->valueForKey("remainGold")->intValue();
            UIComponent::getInstance()->updateGold(tmpInt);
        }
        DailySendMap.clear();
        DailySendMapData.clear();
        if (dict->objectForKey("taskList")) {
            CCArray *arr = dynamic_cast<CCArray*>(dict->objectForKey("taskList"));
            for (int i=0; i<arr->count(); i++) {
                CCDictionary *tempDict = dynamic_cast<CCDictionary*>(arr->objectAtIndex(i));
                AllianceDailyQuestInfo questInfo = AllianceDailyQuestInfo(tempDict);
                if (questInfo.m_stat=="0") {
                    DailySendMap[questInfo.m_uid]=questInfo;
                    if (DailySendMap.size() >3) {
                        CCLog(">>>>>>>>3");
                    }
                }
                
            }
        }
        if(dict->objectForKey("record")){
            CCDictionary* recordDict = dynamic_cast<CCDictionary*>(dict->objectForKey("record"));
            //刷新数据 更新界面
            refreshUserData(recordDict,false);
        }
        doOrderSendMap();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(AllianceDailyRefreshTableViewData,CCInteger::create(1));
    }
}

void AllianceDailyController::updateFreeQueueInfo(AllianceDailyQuestInfo m_info){
    if(freequeue_c){
        freequeue_c = 0;
        freequeuestarttime_c = m_info.m_publishTime;
        freequeuetime_c = m_info.m_vanishTime;
        freequeueTotalTime = freequeuetime_c - freequeuestarttime_c;
    }
}

string AllianceDailyController::orderRewards(string rewards){
    int goodsIdx=3;
    map<int ,map<int,string> > tempMap;
    vector<string> rewardVec;
    CCCommonUtils::splitString(rewards, "|", rewardVec);
    for (int i=0; i<rewardVec.size(); i++) {
        string subStr = rewardVec[i];   //goods,200321,2
        vector<string> subVec;
        CCCommonUtils::splitString(subStr, ",", subVec);
        if (subVec.size()==3) {
            string strType = subVec[0];  //auto& tool = ToolController::getInstance()->getToolInfoById(atoi(rId.c_str()));
            string strId = subVec[1];
            auto& tool = ToolController::getInstance()->getToolInfoById(atoi(strId.c_str()));
            string amount = tool.getPara();
            int idx = getIndexByName(strType);
            if (idx==3) {
                if (amount == "") {
                    amount = "0";
                }
                tempMap[idx].insert(pair<int, string>(atoi(amount.c_str()),subStr));
                goodsIdx++;
            }else{
                if (amount == "") {
                    amount = "0";
                }
                tempMap[idx].insert(pair<int, string>(atoi(amount.c_str()),subStr));
            }
        }
    }
    string rewardStr="";
    for (int j=0; j<tempMap.size(); j++) {
        map<int,string> ::iterator iter  =  tempMap[j].end();
        while(iter!=tempMap[j].begin()) {
            iter--;
            string str = iter->second;
            if(str != ""){
                rewardStr+=str;
                rewardStr+="|";
            }
        }
    }
    return rewardStr;
}

int AllianceDailyController::getIndexByName(std::string name){
    if("exp" == name){
        return 0;
    }
    if ("goods" == name) {
        return 1;
    }
    return 3;
}
