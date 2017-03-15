//
//  GoldExchangeAdvertisingView_NEW.hpp
//  IF
//
//  Created by xxrdsg on 15/11/3.
//
//

#ifndef GoldExchangeAdvertisingView_NEW_hpp
#define GoldExchangeAdvertisingView_NEW_hpp

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"

class GoldExchangeAdvertisingView_NEW: public Layer
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public TableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static GoldExchangeAdvertisingView_NEW* create();
    ~GoldExchangeAdvertisingView_NEW();
protected:
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onClickCostBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void refreshView(CCObject* obj);
    void onPlayEnterFrame(float dt);
    void recoverScroll(CCObject* obj);
    void stopScroll(CCObject* obj);
    
    void errorPayment(CCObject* obj = NULL);
    void onShowError(float dt);


    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCNode> m_scrollContainer;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_pageNode;
    CCSafeObject<CCSprite> m_pageflag;
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCNode> m_animNode;
    CCTableView* m_tabView;
    
    void generateData();
    void generalDataAgain(float _time);
    void generalEndDataAgain(float _time);
    void setButtonState(int offX,bool isreset = true);
    void addButtonLight(bool bAdd);
    void showButtonLight(float _time);
    int m_x;
    int m_startX;
    int m_endX;
    
    GoldExchangeItem* m_dataItem;
};

//.............//
class GoldExchangeAdvertisingCell_NEW:
public CCTableViewCell
{
public:
    static GoldExchangeAdvertisingCell_NEW *create(GoldExchangeItem* item);
    GoldExchangeAdvertisingCell_NEW(GoldExchangeItem* item) : m_item(item) {};
    void setData(GoldExchangeItem* item);
private:
    bool init();
    
    virtual void onEnter();
    virtual void onExit();
    GoldExchangeItem* m_item;
    
    CC_SYNTHESIZE(CCRect, m_costBtnRect, CostBtnRect);
};


//...............//
class GoldExchangeAdvertisingCommCell_NEW : public Layer
, public CCBSelectorResolver

, public CCBMemberVariableAssigner
{
public:
    GoldExchangeAdvertisingCommCell_NEW(GoldExchangeItem *goldExchangeItem){
        m_dataItem = goldExchangeItem;
        
    }
    static GoldExchangeAdvertisingCommCell_NEW* create(GoldExchangeItem *goldExchangeItem);
    CCRect getCostBtnRect();
private:
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void refreshData();
    void onEnterFrame(float dt);
    void onClickCostBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickGivePackageBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCLabelBMFont> m_getGoldNumText;
    CCSafeObject<CCLabelIF> m_percentLabel;
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCLabelIF> m_newPriceLabel;
    CCSafeObject<CCNode> m_touchMoreNode1;
    CCSafeObject<CCControlButton> m_costBtn;
    CCSafeObject<CCNode> m_animNode;
    CCSafeObject<CCNode> m_showMoneyNode;
    CCSafeObject<CCControlButton> m_btnGivePackage;
    GoldExchangeItem *m_dataItem;
    
    CCPoint m_touchLocation;
};

#endif /* GoldExchangeAdvertisingView_NEW_hpp */
