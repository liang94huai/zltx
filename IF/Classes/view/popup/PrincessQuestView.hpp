//
//  PrincessQuestView.hpp
//  IF
//
//  Created by 王超一 on 16/1/12.
//
//

#ifndef PrincessQuestView_hpp
#define PrincessQuestView_hpp

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "PrincessLimitedQuestInfo.hpp"
#include "DefaultTableViewDelegate.h"
#include "CCTableViewTouchIFCell.h"
class PrincessQuestView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCTableViewDataSource
, public DefaultTableViewDelegate
{
public:
    static PrincessQuestView *create();
    ~PrincessQuestView();
//    CCNode* getGuideNode(string _key);
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
    void refreshView(CCObject* obj);
    int getContentOffSety();
    
    void sortQuestInfo();
    void refreshDialog();
    
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCScale9Sprite> m_viewBg;
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<Node> m_princess;
    CCSafeObject<CCLabelIF> m_dialog;
    CCSafeObject<Scale9Sprite> m_dialogBg;
    CCSafeObject<Node> m_fireNode1;
    CCSafeObject<Node> m_fireNode2;
    
    Point m_touchPoint;
};

class PrincessQuestCell :public CCTableViewTouchIFCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static PrincessQuestCell* create(PrincessQuestInfo* info);
    PrincessQuestCell(PrincessQuestInfo* info):m_info(info){};
//    CCNode* getGuideNode();
    void setData(PrincessQuestInfo* info);
    void update(float sec);
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onGotoBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void refresh();
    
    CCSafeObject<CCLabelIF> m_descTxt;
    CCSafeObject<CCNode> m_picHead;
    CCSafeObject<CCControlButton> m_btnGoto;
    CCSafeObject<CCScale9Sprite> m_progress;
    CCSafeObject<CCLabelIF> m_progressTxt;
    CCSafeObject<PrincessQuestInfo> m_info;
    CCSafeObject<Node> m_touchNode;
    CCSafeObject<CCLabelIF> m_statTxt;
    CCSafeObject<CCLabelIF> m_time;
    CCSafeObject<CCLabelIF> m_timeTxt;
};
#endif /* PrincessQuestView_hpp */
