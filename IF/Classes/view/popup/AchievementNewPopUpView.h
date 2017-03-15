//
//  AchievementNewPopUpView.h
//  IF
//
//  Created by lifangkai on 14-7-8.
//
//

#ifndef __IF__AchievementNewPopUpView__
#define __IF__AchievementNewPopUpView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "QuestController.h"
//#include "CCMultiColTableView.h"
#include "QuestInfo.h"
#include "DefaultTableViewDelegate.h"
#include "CCClipNode.h"

#define MSG_PLAY_DROP_RD "msg.play.drop.rd"
                        // 60   61   62   63   64   65
static char qq[7] = {' ', '<', '=', '>', '?', '@', 'A'};    //将K等 换算成ascii码对应字符
inline const char* RES_ITOA_K_1(long v){double t=v;if(t>=1000){int i=0;while(t/1000>=1){t=t/1000;i++;}return CCString::createWithFormat("%.1f",t)->getCString();}else{return CC_ITOA((int)v);}}
inline const char* RES_ITOA_K_1(double v){if(v>=1000){int i=0;while(v/1000>=1){v=v/1000;i++;}return CCString::createWithFormat("%.1f",v)->getCString();}else{return CC_ITOA((int)v);}}

inline const char* RES_ITOA_K_2(long v){double t=v;if(t>=1000){int i=0;while(t/1000>=1){t=t/1000;i++;}return CCString::createWithFormat("%c",qq[i])->getCString();}else{return CC_ITOA((int)v);}}
inline const char* RES_ITOA_K_2(double v){if(v>=1000){int i=0;while(v/1000>=1){v=v/1000;i++;}return CCString::createWithFormat("%c",qq[i])->getCString();}else{return CC_ITOA((int)v);}}

class AchievementNewPopUpView : public PopupBaseView
, public CCBSelectorResolver
  
, public CCBMemberVariableAssigner
, public CCTableViewDataSource
, public DefaultTableViewDelegate
{
public:
    static AchievementNewPopUpView *create();
    ~AchievementNewPopUpView();
    CCNode* getGuideNode(string _key);
    int getScrollPos();
    
protected:
    // scrollview delegate
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void scrollViewDidScroll(CCScrollView* view);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onRewardBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void refreshView(CCObject* obj);
    void setRecommendTaskInfo(float dt=0);
    void updateRecommendTask();
    int getContentOffSety();
    
    void autoGuideNext(float dt=0);
    
    void onPlayDropRD(CCObject* obj);
    void onDropOneCell(CCObject* obj);
    
    void onStageRdUpdate(CCObject* obj);
    void onClickStageBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void onRetRewardInfo(CCObject* obj);
    void getStageRd();
    
    void flyStageParticle();
    void flyStageEnd();
    void onUpdateStageInfo(CCObject* obj);
    
    void onShowHand(float posx);
    void hideFlyArrow(float dt);
    void sortQuestInfo();
    void newSortQuestInfo();
    
    //任务 两个按钮方法
    void onClickDailyBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickTaskBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void updateBtnType(int iType); // 0 日常 1 普通
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCScale9Sprite> m_viewBg;
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCArray> m_cells;
    CCSafeObject<CCNode> m_headNode;
    CCSafeObject<CCNode> m_recommandBG;
    CCSafeObject<CCNode> m_effectChiLun;
    
    
    CCSafeObject<CCLabelIF> m_noQuestText;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_descTxt;
    CCSafeObject<CCLabelIF> m_progressTxt;
    CCSafeObject<CCControlButton> m_btnReward;
    

    // 任务两个按钮
    CCSafeObject<CCControlButton> m_DailyBtn;
    CCSafeObject<CCControlButton> m_TaskBtn;
    
    CCSafeObject<CCScale9Sprite> m_progress;
    
    CCSafeObject<CCNode> m_rewardIcon;
    CCSafeObject<CCLabelIFBMFont> m_rewardNum;
    
    CCSafeObject<CCNode> m_dropNode;
    
    CCSafeObject<CCNode> m_stageNode;
    CCSafeObject<CCLabelIF> m_stageStartLabel;
    CCSafeObject<CCLabelIFBMFont> m_stageCurLabel;
    CCSafeObject<CCLabelIF> m_stageEndLabel;
    CCSafeObject<CCScale9Sprite> m_stagePro;
    CCSafeObject<CCNode> m_stageMoveNode;
    CCSafeObject<CCControlButton> m_stageBtn;
    CCSafeObject<CCNode> m_boxIconNode;
    CCSafeObject<CCNode> m_particleNode;
    CCSafeObject<CCClipNode> m_clipNode;
    
    CCSafeObject<CCNode> m_particleStageNode;
    
    CCSafeObject<CCNode> m_handNode;
    CCSafeObject<CCSprite> m_flyArrow;
    CCSafeObject<Node> m_arrowPar;
    CCSafeObject<Node> m_stageGuideNode;
    CCPoint m_touchPoint;
    QuestInfo* m_recommendinfo;
    WaitInterface* m_waitInterface;
    WaitInterface* m_waitInterfaceStage;
    int r_type;
    int m_dropNum;
    int m_guideAutoSt;
};

class AchievementNewTaskCell :public CCAniNode
,public CCBSelectorResolver
 
,public CCBMemberVariableAssigner
{
public:
    static AchievementNewTaskCell* create(QuestInfo* info,int index,Node* clickArea);
    AchievementNewTaskCell(QuestInfo* info,int index,Node* clickArea):m_info(info),m_index(index),m_clickArea(clickArea),m_rewardClick(false),m_playAnimation(false){};
    CCNode* getGuideNode();
    void setData(QuestInfo* info,int index);
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onGoToBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onRewardBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
  //  void onGoToBtnClick1(CCObject * pSender, Control::EventType pCCControlEvent);
    void animationCallBack();
    void refresh();
    CCSafeObject<CCNode> m_head;
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_descTxt;
   // CCSafeObject<CCLabelIF> m_btnLabel;
    CCSafeObject<CCNode> m_cellBG;
    CCSafeObject<CCNode> m_picHead;
    CCSafeObject<CCControlButton> m_btnGoTo;
    CCSafeObject<CCControlButton> m_btnReward;
    CCSafeObject<CCScale9Sprite> m_progress;
    CCSafeObject<CCLabelIF> m_progressTxt;
    WaitInterface* m_waitInterface;
    CCPoint m_touchPoint;
    QuestInfo* m_info;
    int m_index;
    bool m_rewardClick;
    bool m_playAnimation;
    Node* m_clickArea;
};

class AchievementNewTaskTableCell : public CCTableViewCell
{
public:
    static AchievementNewTaskTableCell* create(QuestInfo* info,int index,Node* clickArea);
    AchievementNewTaskTableCell():mCell(NULL){};
    ~AchievementNewTaskTableCell(){};
    void setData(QuestInfo* info,int index,Node* clickArea);
    CCNode* getGuideNode();
    
private:
    bool init(QuestInfo* info,int index,Node* clickArea);
    AchievementNewTaskCell* mCell;
};

//奖励展示面板
class RewardShowInfoView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCTableViewDataSource
, public DefaultTableViewDelegate
{
public:
    static RewardShowInfoView* create(CCArray* data);
    RewardShowInfoView(){};
    virtual ~RewardShowInfoView(){};
    void setCallback(cocos2d::CCObject *target, SEL_CallFunc func, string btnName, string msg="");
    void showTitle();
private:
    virtual bool init(CCArray* data);
    virtual void onEnter();
    virtual void onExit();
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    
    void onOkClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    
private:
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_congratureNode;
    CCSafeObject<CCNode> m_particleNode1;
    CCSafeObject<CCNode> m_particleNode2;
    CCSafeObject<CCLabelIF> m_titleTTF;
    CCSafeObject<CCLabelIF> m_congratuTTF;
    CCSafeObject<CCLabelIF> m_msgLabel;
    CCSafeObject<CCControlButton> m_okBtn;
    
    CCSafeObject<CCArray> m_data;
    
    CCTableView* m_tableView;
    
    CCObject *m_callbackTarget;
    SEL_CallFunc m_callbackFunc;
};
#endif /* defined(__IF__AchievementNewPopUpView__) */
