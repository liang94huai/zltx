//
//  GameController.h
//  IF
//
//  Created by 邹 程 on 13-9-2.
//
//

#ifndef __IF__GameController__
#define __IF__GameController__

#include "CommonInclude.h"
#include "WaitInterface.h"
#include "UpdateManager.h"

#define CONGRATULATION_TAG 3245
#define AP_REWARD_TAG 495

typedef void (CCObject::*SEL_TIMETICK)(CCObject*,int);
typedef void (CCObject::*SEL_TIMEFINISH)(CCObject*);

#define timeTick_selector(_SELECTOR) (SEL_TIMETICK)(&_SELECTOR)
#define timeFinish_selector(_SELECTOR) (SEL_TIMEFINISH)(&_SELECTOR)

// 是否退出游戏
bool IsQuitGame();

class TimeStampObserver :
public CCObject
{
public:
    TimeStampObserver(CCObject *target,
                        SEL_TIMETICK timeTick,
                        SEL_TIMEFINISH timeFinish,
                        CCObject *objTick,
                        CCObject *objFinish)
    :m_target(target)
    ,m_timeTick(timeTick)
    ,m_timeFinish(timeFinish)
    ,m_objectTick(objTick)
    ,m_objectFinish(objFinish){}
        
    void doTimeTick(int timeLeft);
    void doTimeFinish();

private:
    CC_PROPERTY_READONLY(CCObject *, m_target, Target);
    CC_PROPERTY_READONLY(SEL_TIMETICK, m_timeTick, TimeTick);
    CC_PROPERTY_READONLY(SEL_TIMEFINISH, m_timeFinish, TimeFinish);
    CC_PROPERTY_READONLY(CCObject *, m_objectTick, ObjectTick);
    CC_PROPERTY_READONLY(CCObject *, m_objectFinish, ObjectFinish);
};




class GameController :public CCObject
 
,public UpdateManagerDelegateProtocol
{
public:
    static GameController* getInstance();
    static void purgeData();
    static int m_isInitSDK;    //有没有初始化
    static int m_isLoginSDK;  // 有没有登录账号
    ~GameController();
    
    //-----NetMsgLog-------
    void onNetMessageSave(float dt);
    
    void onTime(float dt);
    void clearCacheAndQuitGame(CCObject *obj);
    
    void quitGame(CCObject* p = NULL); //平台掉
    void quitGameSDKCallBack(CCObject* p = NULL); //平台确认
    void quitGameChannelCallBack(CCObject* p = NULL);
    
    void logoutGame(CCObject* p = NULL);
    void showLogoutPanel(CCObject* p = NULL);
    void recordPushData();
    void platformQuit();
    void platformCollectUserInfo(const std::string &userId,const std::string &userName,const std::string &userLevel);
    void enableQueryHistoryPurchase();
    
    UpdateManager *m_manager;
    virtual void onError(UpdateManager::UMErrorCode errorCode);
    virtual void onProgress(int percent);
    virtual void onSuccess();
    void reloadGame();
    void feedBack();
    //-----waitInterface------
    void showWaitInterface(CCObject* p = NULL);
    WaitInterface* showWaitInterface(CCNode* targetNode, int opacity=64);
    void removeWaitInterface(CCObject*p = NULL);
    void onTimer_remove(float f);
    CCSafeObject<WaitInterface> m_waitInterface;

    
    
    // time stamp
    void addTimeObserver(TimeStampType timeType,CCObject* target,SEL_TIMETICK timeTick,SEL_TIMEFINISH timeFinish = NULL,CCObject* objTick = NULL,CCObject* objFinish = NULL);
    void removeTimeObserver(TimeStampType timeType,CCObject* target);
    int timeObserverCount();
    
    //版本更新
    void updateVersion(CCObject* p = NULL);
    void goTurntoUrl(string url);
    //facebook like
    void gotoFaceBookLike();
    
    // midnight update
    void addMidnightObserver(float t);
    void midnightUpdate(CCObject* obj);
        
    //command connectionLost
    void doCmdConnectionLost(CCObject* obj);
    
    // user level up
    void doUserLevelUp(CCObject* obj);
    void delayLevelUp(CCObject* obj);
    void decorateLevelUp(CCObject* obj);
    void addEnergyForLevelUpFinish(CCObject* obj);
    void doFlyText(CCObject* obj);
    void doAppForegroundEvent(CCObject* obj);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void setLoadingLog(string _class,string func);
    void showBanAlert(const string& str);
    void showBanTimeAlert(const string& str , int banTime = 0);
    void feedBanBack();
    int getConnectRetryCount(){ return m_connectRetryCount;}
    std::string getCokLoadingLogString() { return _cokLoadingLogString;}
    
    long getTimeStep() { return m_timeStep;}
    long millisecondNow();
    void backLoading(); //掉线重回游戏登录界面
    
    void againConnect(); //掉线重回游戏登录界面
private:
    GameController();
    
    GameController& operator=(const GameController&) = default;
    GameController(const GameController&) = default;
    
    void onConnectError(cocos2d::CCObject *obj);
    void checkPurchaseInfoList(float dt);
    void redoPushData(float dt);
    void delayConnect(float dt);
    


    bool queryHistoryPurchaseEnabled = false;
    
    // time stamp
    typedef std::multimap< TimeStampType , TimeStampObserver > TimeStampObserverMap;
    TimeStampObserverMap timeObservers;
    bool timeObserverExisted(TimeStampType timeType,CCObject *target);
    void timeStampTick(TimeStamp &timeStamp,int leftTime);
    void timeStampFinish(TimeStamp &timeStamp);
    void reConnect(CCObject* obj);
    void changeAccount(CCObject *obj);

    long _logseq;
    
    int m_userLevelUpGift1;
    int m_userLevelUpGift2;
    int m_connectRetryCount;
    
    std::string _cokLoadingLogString;
    
    CC_SYNTHESIZE(string, m_loadingLog, LoadingLogString) ;
    long m_timeStep;
};
#endif /* defined(__IF__GameController__) */
