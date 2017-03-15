//
//  PrincessQuestDetailView.hpp
//  IF
//
//  Created by 王超一 on 16/1/12.
//
//

#ifndef PrincessQuestDetailView_hpp
#define PrincessQuestDetailView_hpp

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "QuestController.h"
#include "PrincessLimitedQuestInfo.hpp"
#include "DefaultTableViewDelegate.h"
#include "CCClipNode.h"

#define MSG_PLAY_DROP "msg.play.drop"

class PrincessQuestDetailView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCTableViewDataSource
, public DefaultTableViewDelegate
{
public:
    static PrincessQuestDetailView *create(PrincessQuestInfo* info);
    PrincessQuestDetailView(PrincessQuestInfo* info):m_info(info){};
    ~PrincessQuestDetailView();
//    CCNode* getGuideNode(string _key);
    int getScrollPos();
    void update(float sec);
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
    int getContentOffSety();
    void setReward(CCArray* arr, bool inTime = false);
    
    void setGrey();
    void onPlayDropRD(CCObject* obj);
    void setRewardDesc(int index);
    void sortQuestInfo();
    
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCScale9Sprite> m_viewBg;
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCNode> m_headNode;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_descTxt;
    CCSafeObject<CCControlButton> m_btnReward;
    CCSafeObject<Node> m_fireNode1;
    CCSafeObject<Node> m_fireNode2;
    CCSafeObject<Node> m_tipNode;
    CCSafeObject<Node> m_bottomNode;
    CCSafeObject<CCLabelIF> m_rwdTxt;
    CCSafeObject<CCLabelIF> m_timeOutTxt;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCLabelIF> m_time;
    CCSafeObject<Node> m_rwdNode;
    CCSafeObject<Node> m_rwdExNode;
//    CCSafeObject<CCNode> m_dropNode;
    CCSafeObject<Node> m_desNode;
    CCSafeObject<Scale9Sprite> m_desBg;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCLabelIF> m_desName;
    CCPoint m_touchPoint;
    WaitInterface* m_waitInterface;
    int r_type;
    int m_dropNum;
    CCSafeObject<PrincessQuestInfo> m_info;
    bool m_grey;
};

class PrincessQuestDetailCell :public CCTableViewCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static PrincessQuestDetailCell* create(PrincessQuestInfo* info);
    PrincessQuestDetailCell(PrincessQuestInfo* info):m_info(info){};
//    CCNode* getGuideNode();
    void setData(PrincessQuestInfo* info);
private:
    bool init();
//    virtual void onEnter();
//    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onGotoBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
//    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
//    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void refresh();
    CCSafeObject<CCLabelIF> m_descTxt;
    CCSafeObject<CCNode> m_picHead;
    CCSafeObject<CCControlButton> m_btnGoTo;
    CCSafeObject<Sprite> m_complete;
    CCSafeObject<CCScale9Sprite> m_progress;
    CCSafeObject<CCLabelIF> m_progressTxt;
    CCSafeObject<PrincessQuestInfo> m_info;
};
#endif /* PrincessQuestDetailView_hpp */
