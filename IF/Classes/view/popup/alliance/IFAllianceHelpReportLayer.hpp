//
//  IFAllianceHelpReportLayer.hpp
//  IF
//
//  Created by wangdianzhen on 15/10/21.
//
//

#ifndef IFAllianceHelpReportLayer_hpp
#define IFAllianceHelpReportLayer_hpp

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "AllRankCommand.h"
#include "IFAllianceHelpReportInfo.hpp"
#include "AllianceManager.h"
#include "HFHeadImgNode.h"
#include "CCTableViewTouchIFCell.h"
class IFAllianceHelpReportLayer: public PopupBaseView
,public CCTableViewDataSource
,public DefaultTableViewDelegate
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
{
public:
    static IFAllianceHelpReportLayer* create();
    IFAllianceHelpReportLayer();
    ~IFAllianceHelpReportLayer();
protected:
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    //    bool virtual ccTouchBegan()
private:
    virtual void onEnter();
    virtual void onExit();
    
    void refreashData(CCObject* obj);
    
    virtual bool init( );
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    
    CCSafeObject<CCLabelIF> m_nameLbTitle;
    CCSafeObject<CCLabelIF> m_soldierLbTitle;
    CCSafeObject<CCLabelIF> m_timeLbTitle;
    CCSafeObject<CCLabelIF> m_noDataLb;
    
    CCSafeObject<CCNode> m_totalNode;
    
    CCSafeObject<CCTableView> m_tabView;
    void getRankData();
    void updateInfo();
    CCSafeObject<CCArray> m_data;
    int m_cityLv;
};




class IFAllianceHelpReportCell :
public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewTouchIFCell
{
public:
    static IFAllianceHelpReportCell* create(int index);
    IFAllianceHelpReportCell(int index):m_index(index){};
    void setData(int index);
    void printLog();
private:
    bool init(int index);
    virtual void onEnter();
    virtual void onExit();
    
    
    void refreashData(CCObject* obj);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    CCSafeObject<CCLabelIF> m_nameLb;
    CCSafeObject<CCLabelIF> m_countLb;
    CCSafeObject<CCLabelIF> m_timeLb;

    
    CCSafeObject<CCNode> m_headNode;
    

    
    int m_index;
    CCPoint m_startPoint;
    
};

#endif /* IFAllianceHelpReportLayer_hpp */
