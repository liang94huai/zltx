//
//  AllianceScienceView.h
//  IF
//
//  Created by fubin on 14-4-9.
//
//

#ifndef IF_AllianceScienceView_h
#define IF_AllianceScienceView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "DefaultTableViewDelegate.h"
#include "BackpackView.h"
#include "CCStarBar.h"
#include "CCTableViewTouchIFCell.h"
#include "CCTableViewTouchIFCell.h"
class AllScienceBigCell;
class AllScienceCell;

class AllianceScienceView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static AllianceScienceView* create();
    AllianceScienceView(){};
    virtual ~AllianceScienceView(){};

protected:
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void resetOpenLockVec();
    void updateInfoOnTouch(Ref* ref);
    void updateInfo(CCObject* obj);
    void updatePoint(CCObject* obj);
    void updateOpenPanel(CCObject* obj);
    void onEnterFrame(float dt);
    
    int getTotalH();
    int getFirstPos(vector<int>&);
    
    void onOkBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onTipBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onGuideClick(Ref* ref, Control::EventType);
    
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCControlButton> m_okBtn;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCNode> m_listNode;
    
    CCSafeObject<CCLabelIF> m_contrNameLabel;
    CCSafeObject<CCLabelIF> m_contrLabel;
    
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<ControlButton> m_guideBtn;
    
    vector<int> m_curList;
    
    int m_openItemId;
};

class AllScienceBigCell :
public CCBSelectorResolver
 
,public CCBMemberVariableAssigner
,public CCTableViewTouchIFCell
{
public:
    static AllScienceBigCell* create(int itemId, int type, int isLock, bool topLevel = false);
    AllScienceBigCell():m_itemId(0),m_type(0),m_canTouch(true),m_startY(0),m_topLevel(false){};
    void setData(int itemId, int type, int isLock, bool topLevel = false);
private:
    bool init(int itemId, int type, int isLock, bool topLevel);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickOpen(float rat);
    void onResetCanTouch();
    
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_floorLabel;
//    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCNode> m_myTouchNode;
    CCSafeObject<CCSprite> m_jiantouSpr;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCNode> m_mainNode;
    
    int m_itemId;
    int m_type;
    bool m_canTouch;
    int m_startY;
    bool m_topLevel;
};

class AllScienceCell :
public CCBSelectorResolver
,public CCBMemberVariableAssigner
 
,public Layer
{
public:
    static AllScienceCell* create(int r_type, int itemId, bool isLock);
    AllScienceCell():m_itemId(0),m_type(0),m_startY(0),m_bOpen(false),m_bLock(false){};
private:
    bool init(int r_type, int itemId, bool isLock);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onRefreshData(CCObject* obj);
    void onUpdateInfo(bool isUpdate = false);
    void onEnterFrame(float dt);
    void playTouchSearchAnim();
    void playTouchSearchShowTimeBarAnim();
    void playTouchSearchShowTimeBarAnimFinish();
    
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_yesTouchNode;
    CCSafeObject<CCNode> m_starNode;
    
    CCSafeObject<CCNode> m_lvNode;
    CCSafeObject<CCLabelIF> m_lvLabel;
    CCSafeObject<CCLabelIF> m_curLvLabel;
    CCSafeObject<CCLabelIF> m_nextLvLabel;
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCLabelIF> m_masterLabel;
    CCSafeObject<CCLabelIF> m_researshLabel;
    CCSafeObject<CCNode> m_timeBarNode;
    CCSafeObject<CCNode> m_timeBarAnimNode;
    CCSafeObject<CCNode> m_clipBarNode;
    CCSafeObject<CCSprite> m_timeRotationBGSpr1;
    CCSafeObject<CCSprite> m_timeRotationBGSpr2;
//    CCSafeObject<CCLabelIF> m_nameLabel;
    
    CCSafeObject<CCStarBar> m_starBar;
    CCSafeObject<CCClipNode> m_clipNode;
    
    CCSafeObject<Node> m_recNode;
    CCSafeObject<CCLabelIF> m_recLabel;
    
    int m_itemId;
    int m_type;
    int m_startY;
    bool m_bOpen;
    bool m_bLock;
};

#endif
