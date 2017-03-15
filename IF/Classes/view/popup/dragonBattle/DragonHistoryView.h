//
//  DragonHistoryView.h
//  IF
//
//  Created by ganxiaohua on 15/8/27.
//
//

#ifndef __IF__DragonHistoryView__
#define __IF__DragonHistoryView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCMultiColTableView.h"
#include "AllianceTeamInfo.h"
#include "YuanJunInfo.h"
#include "AllianceFightInfo.h"
#include "DragonAllianceRankInfo.h"

class DragonHistoryView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static DragonHistoryView *create();
    DragonHistoryView(){};
    ~DragonHistoryView(){};

protected:

private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    void onRewardClick(CCObject *pSender, CCControlEvent event);
    
    void updatePosition(int type,int index=-1,bool update=false);
    void updateData(CCObject* param);
    void getSerVerData(CCObject* param);
    void updateCellData(CCObject* param);
    
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCLabelIF> m_tip;
    CCSafeObject<CCLabelIF> m_aTitleTxt;
    CCSafeObject<CCLabelIF> m_aRankTxt;
    CCSafeObject<CCLabelIF> m_titleValue;
    CCSafeObject<CCLabelIF> m_rankValue;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCControlButton> m_rewardBtn;
    CCSafeObject<CCNode> m_buttom;
    
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<WaitInterface> m_waitInterface;
    int m_totalH;
    
};

class DragonHistoryRecordCell : public CCIFTouchNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
, public cocos2d::extension::CCMultiColTableViewDataSource
{
public:
    static DragonHistoryRecordCell *create(int type,CCNode* clickArea);
    void setData(int type);
    void reset();
    bool showTowMembers(int index);
    int getCellHeight();
    void upatePosition();
    
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
    virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual ssize_t numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
    
    bool isOpen;
private:
    DragonHistoryRecordCell(int type,CCNode* clickArea):m_type(type),m_clickArea(clickArea){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    bool updateTowData(int index);
    void initTopData();
    void getServerData(CCObject* param);
    
    CCSafeObject<CCLabelIFTTF> m_titleTxt;
    CCSafeObject<CCNode> m_rankIcon;
    CCSafeObject<CCNode> m_bg;
    CCSafeObject<CCSprite> m_arrow;
    
    CCSafeObject<CCNode> m_topContainer;
    CCSafeObject<CCTableView> m_topTabView;
    CCSafeObject<CCArray> m_topData;
    CCSafeObject<CCNode> m_topNode;
    
    CCNode* m_clickArea;
    
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCArray> m_srcData;
    CCSafeObject<CCArray> m_data;
    int m_type;
    int m_totalH;
    bool m_haveSend;
    
};

class DragonHistoryTowCell : public CCIFTouchNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
, public cocos2d::extension::CCMultiColTableViewDataSource
{
public:
    static DragonHistoryTowCell *create(CCDictionary* info,int index,int type,CCNode* clickArea);
    void setData(CCDictionary* info,int index);
    void reset();
    void showMember();
    int getCellHeight();
    
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
    virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual ssize_t numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
    
    bool isOpen;
private:
    DragonHistoryTowCell(CCDictionary* info,int index,int type,CCNode* clickArea):m_info(info),m_index(index),m_type(type),m_clickArea(clickArea){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    void updateData(CCObject* param);
    void callMoreBack(CCObject* param);
    
    CCSafeObject<CCLabelIFTTF> m_titleTxt;
    CCSafeObject<CCNode> m_rankIcon;
    CCSafeObject<CCNode> m_clickNode;
    CCSafeObject<CCSprite> m_arrow;
    CCSafeObject<CCNode> m_tNode;
    CCNode* m_clickArea;
    
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCArray> m_srcData;
    CCSafeObject<CCArray> m_data;
    CCSafeObject<WaitInterface> m_waitInterface;
    CCDictionary* m_info;
    int m_index;
    int m_type;
    int m_tableHeight;
    bool m_haveSend;
};

class DragonHistoryMemberCell : public CCTableViewCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static DragonHistoryMemberCell *create(DragonAllianceRankInfo* info);
    void setData(DragonAllianceRankInfo* info);
    
private:
    DragonHistoryMemberCell(DragonAllianceRankInfo* info):m_info(info){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    CCSafeObject<CCLabelIF> m_allianceNameTxt;
    CCSafeObject<CCLabelIF> m_rankTxt;
    CCSafeObject<CCLabelIF> m_powerTxt;
    CCSafeObject<CCNode> m_flagNode;
    DragonAllianceRankInfo* m_info;
};

class DragonHistoryTopCell : public CCTableViewCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static DragonHistoryTopCell *create(DragonAllianceRankInfo* info);
    void setData(DragonAllianceRankInfo* info);
    
private:
    DragonHistoryTopCell(DragonAllianceRankInfo* info):m_info(info){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    CCSafeObject<CCNode> m_flagNode;
    CCSafeObject<CCLabelIF> m_rankTxt;
    CCSafeObject<CCLabelIF> m_powerTxt;
    CCSafeObject<CCLabelIF> m_kingdomTxt;
    CCSafeObject<CCLabelIF> m_winTxt;
    CCSafeObject<CCLabelIF> m_allianceNameTxt;
    DragonAllianceRankInfo* m_info;
};

#endif /* defined(__IF__DragonHistoryView__) */
