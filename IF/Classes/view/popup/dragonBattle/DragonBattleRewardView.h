//
//  DragonBattleRewardView.h
//  IF
//
//  Created by ganxiaohua on 15/9/6.
//
//

#ifndef __IF__DragonBattleRewardView__
#define __IF__DragonBattleRewardView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCMultiColTableView.h"
#include "AllianceTeamInfo.h"
#include "YuanJunInfo.h"
#include "AllianceFightInfo.h"

class DragonBattleRewardView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static DragonBattleRewardView *create();
    DragonBattleRewardView(){};
    ~DragonBattleRewardView(){};
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
    
    void updatePosition(int type,int index=-1);
    void updateData(CCObject* param);
    void onHelpClick(CCObject *pSender, CCControlEvent event);
    void getServerData(CCObject* param);
    
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCControlButton> m_infoBtn;
    
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<WaitInterface> m_waitInterface;
    
};

class DragonRewardTitleCell : public CCIFTouchNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static DragonRewardTitleCell *create(CCDictionary* info,int type,CCNode* clickArea);
    void setData(CCDictionary* info,int type);
    void reset();
    bool showTowMembers(int index);
    int getCellHeight();
    bool isOpen;
private:
    DragonRewardTitleCell(CCDictionary* info,int type,CCNode* clickArea):m_info(info),m_type(type),m_clickArea(clickArea){};
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
    
    CCSafeObject<CCLabelIFTTF> m_titleTxt;
    CCSafeObject<CCNode> m_rankIcon;
    CCSafeObject<CCNode> m_bg;
    CCSafeObject<CCSprite> m_arrow;
    CCNode* m_clickArea;
    CCSafeObject<CCNode> m_topContainer;
    
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCArray> m_srcData;
    CCSafeObject<CCArray> m_data;
    CCDictionary* m_info;
    int m_type;
    int m_totalH;
    
};

class DragonRewardSubTitleCell : public CCIFTouchNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
, public cocos2d::extension::CCMultiColTableViewDataSource
{
public:
    static DragonRewardSubTitleCell *create(CCDictionary* info,int index,int type,CCNode* clickArea);
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
    DragonRewardSubTitleCell(CCDictionary* info,int index,int type,CCNode* clickArea):m_info(info),m_index(index),m_type(type),m_clickArea(clickArea){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    CCSafeObject<CCLabelIFTTF> m_titleTxt;
    CCSafeObject<CCNode> m_rankIcon;
    CCSafeObject<CCNode> m_bg;
    CCSafeObject<CCSprite> m_arrow;
    CCNode* m_clickArea;
    
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCArray> m_srcData;
    CCSafeObject<CCArray> m_data;
    CCDictionary* m_info;
    int m_index;
    int m_type;
    int m_tableHeight;
};

class DragonRewardCell : public CCTableViewCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static DragonRewardCell *create(CCDictionary* info);
    void setData(CCDictionary* info);
    
private:
    DragonRewardCell(CCDictionary* info):m_info(info){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_numTxt;
    CCSafeObject<CCNode> m_icon;
    CCDictionary* m_info;
};
#endif /* defined(__IF__DragonBattleRewardView__) */
