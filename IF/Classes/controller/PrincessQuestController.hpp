//
//  PrincessQuestController.hpp
//  IF
//
//  Created by 王超一 on 16/1/14.
//
//

#ifndef PrincessQuestController_hpp
#define PrincessQuestController_hpp

#define PRINCESS_QUEST_STATE_UPDATE "princess_quest_state_update"
enum PrincessQuestState{
    PrincessUnfinish,//未完成
    PrincessComplete,//完成//前台根据完成时间自行判断是否在限时内完成
    PrincessReward,//已领奖
};

#include "CommonInclude.h"
#include "PrincessLimitedQuestInfo.hpp"

class PrincessQuestController:public CCNode{//前台只会接受四种（当前为四种）不同的主线任务（支线任务除外）
public:                                     //排序按照前台pos字段进行排列显示位置
    static PrincessQuestController *getInstance();
    static void purgeData();
    PrincessQuestController();
    ~PrincessQuestController();

    void postQuestStateChangeNotification();
    //加任务
    void initQuest(CCArray* showTask);
    void updateTask(CCArray* tasks);
    
    void addQuest(std::string itemId, int state, int type);
    //删除任务
    void removeQuest(std::string itemId, bool postNotification = true);
    //更新状态
    void updateQuestState(std::string itemId, int state, bool postNotification = true);
    //领奖
    void getReward(std::string itemId, bool isMain=false, bool showTip = true);
    //
    void setQuestReward(std::string questId, CCArray *arr);
    
    bool goToQuestTarget(PrincessQuestInfo *info, bool isSt = false);

    void getRewardSuccessHandle(std::string itemId);
    
    void resetGettingRewardFlag(float t);
    bool isGettingReward;
    
    map<int, int> achMap;
    map<std::string, PrincessQuestInfo*> achList;
    map<std::string, PrincessQuestInfo*> currentAchList;//当前显示的成就
    
    bool m_init;
    bool m_returnFromWorld;
    
    bool m_monsterFlag;//当前正在推荐的是怪物任务
    
private:
    void insertToArr(CCArray *arr, std::string itemId, bool isComplete);
    //按para1取配置
    map<int, CCDictionary*> config;
    
    int tmpRewardId;
    
    void popupPSoilderView(float t);
    int ps_bid;
    int ps_needNum;
    string ps_armyId;
};
#endif /* PrincessQuestController_hpp */
