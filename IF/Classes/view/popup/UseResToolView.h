//
//  UseResToolView.h
//  IF
//
//  Created by fubin on 14-3-13.
//
//

#ifndef IF_UseResToolView_h
#define IF_UseResToolView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "CCTableViewTouchIFCell.h"
#define RES_BUILD_VIEW "ResBuildView"
#define RES_BUILD_UP_VIEW "ResBuildUpView"
#define RES_SCIENCE_VIEW "ResScienceUpView"
#define RES_BUILD_CREATE_VIEW "ResBuildCreateView"

class UseResToolCell;

class UseResToolView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
, public CCMultiColTableViewDataSource
, public CCMultiColTableViewDelegate
{
public:
    static UseResToolView* create(int resType, string itemId="", string parentView="");
    UseResToolView():m_resType(0),m_itemId(""),m_parentView(""){};
    virtual ~UseResToolView(){};
    
protected:
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
	virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual ssize_t numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(int resType, string itemId, string parentView);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual void doWhenClose();
    
    void updateResInfo(CCObject* p);
    void updateBindResInfo();
    void updateInfo();
    void refreashData(CCObject* p);
    
    void onClickWoodBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickStoneBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickFoodBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickIronBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onTipBtnClick(CCObject *pSender, CCControlEvent event);
//    void onClickSilverBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCScale9Sprite> m_bindNodeBg;
    CCSafeObject<CCControlButton> m_woodBtn;
    CCSafeObject<CCControlButton> m_stoneBtn;
    CCSafeObject<CCControlButton> m_foodBtn;
    CCSafeObject<CCControlButton> m_ironBtn;
//    CCSafeObject<CCControlButton> m_silverBtn;
    
//    CCSafeObject<CCLabelIF> m_silverLabel;
    CCSafeObject<CCLabelIF> m_woodLabel;
    CCSafeObject<CCLabelIF> m_stoneLabel;
    CCSafeObject<CCLabelIF> m_foodLabel;
    CCSafeObject<CCLabelIF> m_ironLabel;
    CCSafeObject<CCLabelIF> m_bindResDesLabel;
    CCSafeObject<CCLabelIF> m_bindResLabel;
    
    CCSafeObject<CCNode> m_woodNode;
    CCSafeObject<CCNode> m_foodNode;
    CCSafeObject<CCNode> m_ironNode;
    CCSafeObject<CCNode> m_stoneNode;
    CCSafeObject<CCNode> m_bindNode;
    
    CCSafeObject<CCSprite> m_bindFoodIcon;
    CCSafeObject<CCSprite> m_bindWoodIcon;
    CCSafeObject<CCSprite> m_bindIronIcon;
    CCSafeObject<CCSprite> m_bindStoneIcon;
    CCSafeObject<CCSprite> m_itemIcon;
    CCSafeObject<CCSprite> m_bgTitle;
    
    CCSafeObject<CCMultiColTableView> m_tabView;
    
    int m_resType;
    string m_itemId;
    string m_parentView;
    std::vector<int>* m_resList;
    std::vector<int> m_curList;
};

class UseResToolCell :public CCBSelectorResolver
,public CCBMemberVariableAssigner
  
,public CCTableViewTouchIFCell
{
public:
    static UseResToolCell* create(int resType, int m_itemId,CCNode* touchNode);
    UseResToolCell():m_resType(0),m_itemId(0),m_price(0){};
    void setData(int resType,int itemId);
    void refreashData(CCObject* obj);
    void addWaitInterface(CCObject* obj);
private:
    bool init(int resType, int m_itemId,CCNode* touchNode);
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickUseBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickBuyBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onUseTool();
    void onYes();
    
    CCSafeObject<CCControlButton> m_useBtn;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCLabelIF> m_des2Label;
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCNode> m_picNode;
    
    CCSafeObject<CCNode> m_buyNode;
    CCSafeObject<CCControlButton> m_buyBtn;
    CCSafeObject<CCLabelIF> m_btnMsgLabel;
    CCSafeObject<CCLabelIF> m_inBtnGoldNum;
    
    CCNode* mTouchNode;
    WaitInterface* m_waitInterface;
    
    CCPoint m_touchPoint;
    
    int m_resType;
    int m_itemId;
    float m_price;
};

#endif
