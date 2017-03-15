//
//  AllianceDailyController.h
//  IF
//
//  Created by 张军 on 15/10/22.
//
//

#ifndef AllianceDailyController_h
#define AllianceDailyController_h

#include "CommonInclude.h"
#include "AllianceDailyQuestInfo.h"

#define OnGetDailyQuests "OnGetDailyQuests"
#define OnGetDailySendQuests "OnGetDailySendQuests"
#define AllianceDailyRefreshTableViewData "AllianceDailyRefreshTableViewData"
#define OnCellRefreshData "OnCellRefreshData"
#define AllianceDailyFinishAdd "AllianceDailyFinishAdd"
#define AllianceDailyGetRewards "AllianceDailyGetRewards"
#define AllianceMineQueue   "AllianceMinequeue"
#define cellflyAnimationCall "cellflyAnimationCall"
#define CELLSHAPESHIFT "cellShapeShift"
#define MSG_NOTIME_TIP "msg_notime_tip"

class AllianceDailyController : public CCObject{
public:
    static AllianceDailyController* getInstance();
    static void purgeData();
    AllianceDailyController();
    ~AllianceDailyController();
    void onEnterFrame(float time);
    
    void onReturnErrorCode(CCDictionary* dict);
    //获取已发任务（我和其他人发的）
    void startGetDailyQuests();
    void EndGetDailyQuests(CCDictionary* dict);
    //获取我的待发任务
    void startGetSendQuests();
    void endGetSendQuests(CCDictionary* dict);
    //刷新待发任务
    void startRefreshSendQuests();
    void endRefreshSendQuests(CCDictionary* dict);
    //发布任务
    void startSendQuests(string uid,int gold = 0,int freequeue = 0);
    void endSendQuests(CCDictionary* dict);
    //获取奖励详情
    void startGetRewardDetail(string uid);
    void endGetRewardDetail(CCDictionary* dict);
    //添加一条可接任务
    void addOneDailyQuest(CCDictionary* dict);
    //更新一条可接任务(任务被人领走)
    void updateOneDailyQuest(CCDictionary* dict);
    //接任务
    void startAcceptQuest(string uid);
    void endAcceptQuest(CCDictionary* dict);
    //领奖
    void startGetReward(string uid);
    void endGetReward(CCDictionary* dict);
    //加速
    void startSpeedUp(string taskUid,string itemUUid);
    void endSpeedUp(CCDictionary* dict);
    //获取记录信息
    void startGetRecord();
    void endGetRecord(CCDictionary* dict);
    //开关
    bool canAcceptQuests();
    bool canSendQuests();
    bool isSwitchOpen();
    bool isChatShareOpen();
    //获取刷新信息
    void startGetFreshInfo();
    void endGetfreshInfo(CCDictionary* dict);
    //退出联盟提示相关
    //1.是否存在已经接受的任务
    bool isAccept();
    //2.是否已经发布任务
    bool isSend();
    //是否正在进行任务
    bool isHaveQuestAndNotFinish();
    //发布完成的任务（发布的任务可领奖）个数
    int sendFinishCount();
    //接收完成的任务（领的任务可领奖）个数
    int dailyFinishCount();
    //别人发布的品质最高的任务
    int otherSendMaxColor();
    void checkMaxTipColor();
    
    //清空全部任务
    void removeAllMap();
    //是否有免费队列
    int isfreeQueue();
    //获取免费队列的id
    string getfreeQueueId();
    //获取我的任务的数目
    int getMydailyTaskNum();
    
    //排序特殊处理
    int getIndexByName(std::string name);
    string orderRewards(string rewards);
    
    
    std::map<string,AllianceDailyQuestInfo> DailyMap;
    std::map<string,AllianceDailyQuestInfo> MyDailyMap;
    std::map<string,AllianceDailyQuestInfo> DailySendMap;
    
    vector<string> DailyMapData;  //排序后的数据
    vector<string> MyDailyMapData;
    vector<string> DailySendMapData;
    
    std::map<double,string> finishMap;
    std::map<double,string> doingMap;
    std::map<string,std::map<double,string>> colorMap;
    std::map<string,std::map<string,string>> colorMapSend;
    void doOrderDailyMap();
    void doOrderSendMap();
    void addDataByMap(vector<string> &data);
    void orderDataMap(AllianceDailyQuestInfo &info,int orderType);

    string swDaily;
    string swDailyChat;
    
    string acceptUid;
    string rewardUid;
    
    //记录信息
    bool isGetRecord;
    int publishCount;
    int refreshCount;
    int maxPublishRewardCount;
    int maxPublishItemUseCount;
    int itemPublishUseCount;
    double refreshTime;
    int maxAcceptCount;
    int conditionPublishLevel;
    int acceptCount;
    int maxPublishCount;
    int maxFreeRefreshCount;
    int maxAcceptRewardCount;
    int itemPublishCount;
    int conditionAcceptLevel;
    int itemRefreshCount;
    //优化
    //队列信息
    int freequeue_c;  //是否为免费队列
    double freequeuetime_c;  //免费队列的结束时间
    double freequeuestarttime_c;  //免费队列的开始时间
    double freequeueTotalTime;  //免费队列所需时间
    //更新队列信息
    void updateFreeQueueInfo(AllianceDailyQuestInfo m_info);
    //总的可用次数
    int maxAllPublishCount;
    string refreshGoldCost;//刷新花费金币
    string clearGoldCost;//加速所需金币
    string publishGoldCost;//增加发布次数所需金币
    int conditionTime;//入盟所需时间 （秒）
    //结果信息
    int m_sendCount;
    int m_acceptCount;
    int m_refreshCount;
    std::map<int,int> refreshGoldMap;
    std::map<int,int> publishGoldMap;
    int m_finishCount;
    int getRefreshNeedGold();//计算刷新所需金币
    //更新记录数据
    void refreshUserData(CCDictionary *dict,bool isSend =true);
    //获取加速金币
    int getSpeedGoldByColor(string color);
    //获取发布所需金币
    int getPublishNeedGold();
    int m_nextGold;
    
   
    
    bool isNumTip;
    int maxColorTip;
};

#endif /* AllianceDailyController_h */
