//
//  FreshRechargeView.hpp
//  IF
//
//  Created by 邢晓瑞 on 15/11/12.
//
//

#ifndef FreshRechargeView_hpp
#define FreshRechargeView_hpp

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCTableViewTouchIFCell.h"
#include "ActivityController.h"

using namespace spine;

class FreshRechargeView: public PopupBaseView
, public CCTableViewDelegate
, public CCTableViewDataSource
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    static FreshRechargeView* create();
    bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual void scrollViewDidScroll(ScrollView* view);
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual cocos2d::CCSize tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual cocos2d::CCSize cellSizeForTable(TableView *table);
    virtual CCTableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void refreshView();
    void refreshLabel();
    void onGetMsgRwdEnd(Ref* ref);
    
protected:
private:
    CCSafeObject<Node> m_bgNode;
    CCSafeObject<Node> m_listNode;
    CCSafeObject<CCLabelIF> m_label;
    
    CCSafeObject<TableView> m_tabView;
};

class FreshRechargeCell: public CCTableViewTouchIFCell
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    FreshRechargeCell(int idx): m_idx(idx){};
    static FreshRechargeCell* create(int idx);
    void setData(int idx);
    bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onClickRwdBtn(Ref*, Control::EventType);
    void onBuyBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void showDescription(int pos);
    void createIcon(FrReReward& rwd, Node* node);
    bool isValid(FrReReward& rwd);
    
    CCSafeObject<Sprite> m_rwdedBg;
    CCSafeObject<CCLabelIF> m_groupLabel;
    CCSafeObject<CCLabelIF> m_rwdedLabel;
    CCSafeObject<ControlButton> m_rwdBtn;
    CCSafeObject<ControlButton> m_buyBtn;
    
    CCSafeObject<Node> m_showNode1;
    CCSafeObject<Sprite> m_showBg1;
    CCSafeObject<Node> m_iconNode1;
    CCSafeObject<CCLabelIF> m_numLabel1;
    
    CCSafeObject<Node> m_showNode2;
    CCSafeObject<Sprite> m_showBg2;
    CCSafeObject<Node> m_iconNode2;
    CCSafeObject<CCLabelIF> m_numLabel2;
    CCSafeObject<Node> m_touchNode1;
    
    CCSafeObject<Node> m_desNode;
    CCSafeObject<Scale9Sprite> m_desBg;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCLabelIF> m_desName;
    CCSafeObject<Node> m_touchNode2;
    
    int m_idx;
};

class FreshRechargeIcon: public CCIFTouchNode
, public CCBMemberVariableAssigner
{
public:
    static FreshRechargeIcon* create();
    bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    
    void onAnimationComplete(int, int);
    
    virtual void onEnter();
    virtual void onExit();
    
    CCSafeObject<Node> m_mainNode;
    CCSafeObject<Node> m_node1;
    CCSafeObject<Node> m_node2;
};

class FreshRechargeEnter: public Node
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    static FreshRechargeEnter* create();
    bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    void refreshLabel(Ref* ref);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName);
    
    void onClickEnterBtn(Ref*, Control::EventType);
    CCSafeObject<ControlButton> m_enterBtn;
    CCSafeObject<CCLabelIF> m_groupLabel;
    CCSafeObject<Node> m_tipNode;
};

class FreshRechargeTip: public Node
{
public:
    static FreshRechargeTip* create();
    bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    CCSafeObject<Node> m_mainNode;
    CCSafeObject<CCLabelIF> m_label;
    
    void refresh(Ref* ref);
};

#endif /* FreshRechargeView_hpp */
