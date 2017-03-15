//
//  DragonStatisticsView.h
//  IF
//
//  Created by ganxiaohua on 15/8/31.
//
//

#ifndef __IF__DragonStatisticsView__
#define __IF__DragonStatisticsView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCTableViewTouchIFCell.h"
class DragonStatisticsView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static DragonStatisticsView *create();
    
protected:
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t
    numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onLeaveClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickHelpBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickTab1(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickTab2(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickTab3(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickTab4(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void updateData(CCObject* param);
    void callSuccess(CCObject* param);
    void getServerData(CCObject* param);
    void updateTime(float _time);
    void leavelDragon();
    
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCNode> m_selectSpr;
    CCSafeObject<CCLabelIF> m_tipTxt;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCLabelIF> m_introTxt;
    CCSafeObject<CCControlButton> m_tab1;
    CCSafeObject<CCControlButton> m_tab2;
    CCSafeObject<CCControlButton> m_tab3;
    CCSafeObject<CCControlButton> m_tab4;
    CCSafeObject<CCControlButton> m_leaveBtn;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCLabelIF> m_introTitle;
    CCSafeObject<CCNode> m_introNode;
    CCSafeObject<CCSprite> m_dragonBg;
    CCSafeObject<CCNode> m_selectBtn;
    CCSafeObject<CCNode> m_videoNode;
    CCSafeObject<CCNode> m_textNode;
    CCSafeObject<CCNode> m_buildBG;
    
    CCSafeObject<CCNode> m_tNode1;
    CCSafeObject<CCLabelIF> m_nodetxt1_0;
    CCSafeObject<CCLabelIF> m_nodetxt1_1;
    
    CCSafeObject<CCNode> m_tNode2;
    CCSafeObject<CCLabelIF> m_nodetxt2_0;
    CCSafeObject<CCLabelIF> m_nodetxt2_1;
    
    CCSafeObject<CCNode> m_tNode3;
    CCSafeObject<CCLabelIF> m_nodetxt3_0;
    CCSafeObject<CCLabelIF> m_nodetxt3_1;
    
    CCSafeObject<WaitInterface> m_waitInterface;
    
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCArray> m_srcData;
    int m_time;
};

class DragonStatisticsCell : public CCTableViewTouchIFCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner

{
public:
    static DragonStatisticsCell *create(CCDictionary* info,CCNode* clickArea);
    void setData(CCDictionary* info);
private:
    DragonStatisticsCell(CCDictionary* info,CCNode* clickArea):m_info(info),m_clickArea(clickArea){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    CCSafeObject<CCNode> m_openNode;
    CCSafeObject<CCNode> m_arrow;
    CCSafeObject<CCNode> m_clickNode;
    CCSafeObject<CCLabelIF> m_allianceName;
    CCSafeObject<CCLabelIF> m_scoreTxt;
    CCSafeObject<CCLabelIF> m_numTxt;
    CCSafeObject<CCLabelIF> m_winTxt;
    CCSafeObject<CCLabelIF> m_failTxt;
    CCSafeObject<CCLabelIF> m_killTxt;
    CCSafeObject<CCLabelIF> m_bNumTxt;
    CCSafeObject<CCNode> m_normalNode;
    CCSafeObject<CCScale9Sprite> m_enemySign;
    
    CCNode* m_clickArea;
    CCDictionary* m_info;
};
#endif /* defined(__IF__DragonStatisticsView__) */
