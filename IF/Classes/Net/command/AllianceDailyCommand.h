//
//  AllianceDailyCommand.h
//  IF
//
//  Created by 张军 on 15/10/22.
//
//

#ifndef AllianceDailyCommand_h
#define AllianceDailyCommand_h

#include "CommandBase.h"
#define DAILY_GET "al.task.get.published"
#define DAILY_GET_SEND "al.task.get.available"
#define DAILY_REFRESH_SEND "al.task.refresh"
#define DAILY_SEND_QUEST "al.task.publish"
#define DAILY_GET_DETAIL "al.task.get.detail"
#define DAILY_ACCEPT_QUEST "al.task.accept"
#define DAILY_GET_REWARD "al.task.award"
#define DAILY_SPEEDUP "al.task.spd"
#define DAILY_RECORD "al.task.record"
#define DAILY_AUTO_FRESH "al.task.auto.refresh"

//获取已发任务（我和其他人发的）
class AllianceDailyCommand: public CommandBase {
public:
    AllianceDailyCommand():
    CommandBase(DAILY_GET){
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};
//获取我的待发任务
class AllianceGetSendQuestsCommand: public CommandBase {
public:
    AllianceGetSendQuestsCommand():CommandBase(DAILY_GET_SEND){
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

//刷新待发任务
class AllianceRefreshSendQuestsCommand: public CommandBase {
public:
    AllianceRefreshSendQuestsCommand():CommandBase(DAILY_REFRESH_SEND){
        putParam("gold", CCInteger::create(0));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

//发布任务
class AllianceSendQuestsCommand: public CommandBase {
public:
    AllianceSendQuestsCommand(string uid,int gold,int freequeue):CommandBase(DAILY_SEND_QUEST){
        putParam("taskUid", CCString::create(uid));
        putParam("gold", CCInteger::create(gold));
        putParam("freequeue", CCInteger::create(freequeue));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

//获取奖励详情
class AllianceGetDetailCommand: public CommandBase {
public:
    AllianceGetDetailCommand(string uid):CommandBase(DAILY_GET_DETAIL){
        putParam("taskUid", CCString::create(uid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

//push处理
class PushAllianceDailyPublishCommand{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};
class PushAllianceDailyAcceptCommand{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

//领任务
class AllianceAcceptQuestCommand: public CommandBase {
public:
    AllianceAcceptQuestCommand(string uid):CommandBase(DAILY_ACCEPT_QUEST){
        putParam("taskUid", CCString::create(uid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

//领奖
class AllianceDailyGetRewardCommand: public CommandBase {
public:
    AllianceDailyGetRewardCommand(string uid):CommandBase(DAILY_GET_REWARD){
        putParam("taskUid", CCString::create(uid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

//加速
class AllianceDailySpeedUpCommand: public CommandBase {
public:
    AllianceDailySpeedUpCommand(string taskUid,string itemUuid):CommandBase(DAILY_SPEEDUP){
        putParam("taskUid", CCString::create(taskUid));
        if(itemUuid!=""){
            putParam("itemUUid", CCString::create(itemUuid));
        }
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

//获取记录信息
class AllianceDailyRecordCommand: public CommandBase {
public:
    AllianceDailyRecordCommand():CommandBase(DAILY_RECORD){
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

//获取刷新纪录
class AllianceDailyFreshCommand: public CommandBase {
public:
    AllianceDailyFreshCommand():CommandBase(DAILY_AUTO_FRESH){
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* AllianceDailyCommand_h */







