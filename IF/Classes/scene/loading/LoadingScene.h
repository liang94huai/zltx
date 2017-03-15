//
//  LoadingScene.h
//  IF
//
//  Created by 吴 凌江 on 13-8-28.
//
//

#ifndef __IF__LoadingScene__
#define __IF__LoadingScene__

#include "CommonInclude.h"
#include "CCClipNode.h"

class FlyRollingText;
class FlySystemUpdateHint;

struct sServerListCellInfo
{
    string ip;
    string port;
    string zone;
    string name;
    string id;
    string gameUid;
    string _uuid;
    string _buildCode;
    string lastLogin;
};

class LoadingScene :
public cocos2d::CCLayer
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
	~LoadingScene();
    CREATE_FUNC(LoadingScene);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onLoginClick(CCObject *pSender, CCControlEvent event);
    void onEnterGame(CCObject *pSender, CCControlEvent event);
    void onNoticeCallback(CCObject *pSender, CCControlEvent event);
protected:
	LoadingScene(void);
    // init ----
    
    void addVersionLabel();
    bool init();
    
    void doResourceUpdate(CCObject* p);
    void doLogin(CCObject* p);
    void doLoginInitError(CCObject* p);
    void doLoginInitBan(CCObject* p);
    void doLoginInit(float t);
    void doLoginInitFinish(Ref* p);
    void stepLoading(CCObject* p);
    void onConnectionLost(cocos2d::CCObject* obj);
    void onRelogin(CCObject* p);
    void forceUpdate(CCObject* p);
    void offerUpdate(CCObject* p);
    void gotoMainScene(float t);
    void onEnterMainScene(float t); // 3dtouch
    void showMainScene();
    void checkCanGotoMainScene(CCObject* p);
    
    void onServerUpdate(CCObject* p);
    
    void logoutSDK(float dt); //退出游戏 时 主动注销
private:
    
    virtual void onEnter();
    virtual void onExit();
    
    void onEnterInit();
    void clearCacheOK(CCObject *obj);
    void clearCacheCancel(CCObject *obj);
    void onNetworkAvailable(CCObject* p);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    bool isChina();
    void showLoading(CCObject* p);
    
    void stopLoading(CCObject* p);
    
    void loadingRest(float t);
    void loadingRestAsyn(Ref* arg);
    void unzipLocalfileAsyn(int type=0);
    void sendCmdLogin();
    void loadingAni(float t);
//    void selectLogin(float t);
    void sendCmdGetServerList(CCObject* p);
    void onGetServerList(CCHttpClient* client, CCHttpResponse* response);
    void onGetNoticeContent(CCHttpClient* client, CCHttpResponse* response);
    void setLoadingTips();
    void onEnterFrame(CCObject* p);
    void onLoginTimeout(float t);
    void onGetServerStatus(CCHttpClient* client, CCHttpResponse* response);

    void resendGetServerList(float time);
    
    bool isJP();
    void show45SecondAlert(float time);
    
    void updateLoadingTip(float _time);
    
    void _setProgressVisiable(bool bVisiable);
    void _refreshSelectView();
    void _setServerListViewVisiable(bool bVisiable);
    
    void initLoginLabel();
    
    void initScrolView();   //服务器列表更新
    void refreshScrolView();
    
    void refreshSelectViewByScrol();
    
    void selectServerList(CCObject* p);
    
    void gameLogout(CCObject* p);
    
    string m_curServerId; // 当前服务器id
    
    int m_responseCode;
    std::string m_tipDialog;
    int m_tipCount;

    int m_steps;
    int m_getServerListRetryTime;
    
    int m_iTouchType;   //
    int m_isEnterGame;  //
    CCSafeObject<Node> m_spineNode;
    CCSafeObject<Sprite> m_loadingBg;
    CCSafeObject<Sprite> m_loadingBarBG;
    CCSafeObject<Sprite> m_loadingBar;
    CCSafeObject<Sprite> m_loadingProgress;
    CCSafeObject<CCLabelIF> m_loadingTips;
    CCSafeObject<ControlButton> m_btnLogin;
    CCSafeObject<Sprite> m_logo;
    CCSafeObject<CCClipNode> m_barClipNode;
    CCSafeObject<Node> m_progressNode;
    CCSafeObject<Node> m_selectServerNode;
    CCSafeObject<Node> m_serverListNode;
    CCSafeObject<Node> m_sdkTouchNode;
    CCSafeObject<Node> m_serverListTouchNode;
    CCSafeObject<Node> m_serverListViewNode;
    
    CCSafeObject<CCLabelIF> m_sdkLabel;
    CCSafeObject<CCLabelIF> m_severLabel;
    CCSafeObject<CCLabelIF> m_changeSeverLabel;
    CCSafeObject<CCLabelIF> m_myServerName;
    CCSafeObject<CCLabelIF> m_allServerName;
    
    CCSafeObject<Node> m_hotNode;
    CCSafeObject<Node> m_newNode;
    CCSafeObject<Node> my_serverlist;
    CCSafeObject<Node> all_serverlist;
 
    CCSafeObject<CCScrollView> m_myScrollView;
    CCSafeObject<CCScrollView> m_allScrollView;
    
    CCSafeObject<FlySystemUpdateHint> m_rollHint;
    
    CCSize m_barSize;
    vector<std::string> m_loadingTipsArr;
    std::map<std::string,sServerListCellInfo> m_serverList;
};

class ServerInfoCell :
public Layer
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static ServerInfoCell* create(int type,string serverID,string playName,string serverName);
    ServerInfoCell(int type,string serverID,string playName,string serverName):m_type(type),m_serverID(serverID),m_playNameStr(playName),m_serverNameStr(serverName){};
    
    
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onSelectClick(CCObject *pSender, CCControlEvent event);
    CCSafeObject<CCLabelIF> m_serverName; 
    CCSafeObject<CCLabelIF> m_playName;
    
    CCSafeObject<CCNode> my_ServerNode;
    CCSafeObject<CCNode> all_ServerNode;
    
    
    int m_type;
    string m_serverID;
    string m_playNameStr;
    string m_serverNameStr;
    
};
#endif /* defined(__IF__LoadingScene__) */
