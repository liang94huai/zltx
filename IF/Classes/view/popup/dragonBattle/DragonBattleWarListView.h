//
//  DragonBattleWarListView.h
//  IF
//
//  Created by ganxiaohua on 15/8/25.
//
//

#ifndef __IF__DragonBattleWarListView__
#define __IF__DragonBattleWarListView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCMultiColTableView.h"
#include "CCTableViewTouchIFCell.h"


class DragonWarCellDelegate
{
public:
    virtual void dragonWarCellClick(TableViewCell *cell)=0;
    virtual void dragonWarApplySuccess()=0;
};





class DragonBattleWarListView : public PopupBaseView
, public CCBSelectorResolver
, public cocos2d::extension::CCMultiColTableViewDataSource
, public CCBMemberVariableAssigner
, public DragonWarCellDelegate
{
public:
    static DragonBattleWarListView *create();
    DragonBattleWarListView(){};
    ~DragonBattleWarListView(){};
    
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
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onWarRankClick(CCObject *pSender, CCControlEvent event);
    void onClickHelp(CCObject *pSender, CCControlEvent event);
    void onManageClick(CCObject *pSender, CCControlEvent event);
    void onSureClick(CCObject *pSender, CCControlEvent event);
    
    void getData(CCObject *pSender);
    void refreshData(CCObject *param);
    
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCLabelIF> m_tipTxt;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCControlButton> m_historyBtn;
    CCSafeObject<CCControlButton> m_helpBtn;
    CCSafeObject<CCControlButton> m_manageBtn;
    CCSafeObject<CCControlButton> m_sureBtn;
    CCSafeObject<CCSprite> m_dragonBg;
    CCSafeObject<CCLabelIF> m_timeTip;
    
    CCSafeObject<CCMultiColTableView> m_tabView;
    CCSafeObject<WaitInterface> m_waitInterface;
    CCSafeObject<CCArray> m_data;
    
    
    void dragonWarCellClick(TableViewCell *cell);
    void dragonWarApplySuccess();
    int selectedIndex;
    
    bool checkAlreadySigned();
    std::vector<TableViewCell *>cellList;
    TableViewCell * getCellByIndex(int index);
};





class DragonWarCell : public CCTableViewTouchIFCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner

{
public:
    static DragonWarCell *create(CCDictionary* info,CCNode* clickArea);
    void setData(CCDictionary* info);
    CCSafeObject<CCControlButton> m_btnReward;
    int getCellIndex();
    void setDelegate(DragonWarCellDelegate *delegate);
    void updateCellSelectState(bool selected);
    
    void warApplyClick();
private:
    DragonWarCell(CCDictionary* info,CCNode* clickArea):m_info(info),m_clickArea(clickArea){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    void successCallBack(CCObject* param);
    void confirmFun();
    
    CCSafeObject<CCLabelIF> m_txt1;
    CCSafeObject<CCLabelIF> m_txt2;
    CCSafeObject<CCLabelIF> m_txt3;
    CCSafeObject<CCLabelIF> m_txt4;
    CCSafeObject<CCLabelIF> m_time1;
    CCSafeObject<CCLabelIF> m_time2;
    CCSafeObject<CCLabelIF> m_signTxt;
    CCSafeObject<CCNode> m_icon;
    CCSafeObject<CCNode> m_signNode;
    CCSafeObject<CCNode> m_clickNode;
    CCSafeObject<CCScale9Sprite> m_selectNode;
    CCNode* m_clickArea;
    int m_index;
    string beginTime;
    string endtime;
    DragonWarCellDelegate *myDelegate;
    CCDictionary* m_info;
};
#endif /* defined(__IF__DragonBattleWarListView__) */
