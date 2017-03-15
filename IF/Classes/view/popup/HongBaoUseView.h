//
//  HongBaoUseView.h
//  IF
//
//  Created by 张军 on 15/11/17.
//
//

#ifndef HongBaoUseView_h
#define HongBaoUseView_h

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCTableViewTouchIFCell.h"
#include "ToolInfo.h"

class HongBaoUseView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
, public CCTableViewDataSource
, public DefaultTableViewDelegate
, public EditBoxDelegate
{
public:
    static HongBaoUseView* create(ToolInfo& info);
    HongBaoUseView(){};
    ~HongBaoUseView(){};

protected:
//    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);

    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void editBoxReturn(EditBox* editBox);
    virtual void editBoxEditingDidBegin(EditBox* editBox);
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(ToolInfo& info);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onDownBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onTipBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);

    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCLabelIF> m_goldTipTxt;
    CCSafeObject<CCLabelIF> m_numTipTxt;
    CCSafeObject<CCLabelIF> m_goldInputTipTxt;
    CCSafeObject<CCLabelIF> m_numInputTipTxt;
    CCSafeObject<CCLabelIF> m_downTxt;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_editNode1;
    CCSafeObject<CCNode> m_editNode2;
    CCSafeObject<CCControlButton> m_downBtn;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCControlButton> m_tipBtn;
    
    CCSafeObject<CCLabelIF> m_msgTipTxt;
    CCSafeObject<CCNode> m_editNode3;
    CCSafeObject<CCLabelIF> m_msgInputTipTxt;
    CCSafeObject<CCLabelIF> m_channelTxt;
    
    CCEditBox* m_editBox1;
    CCEditBox* m_editBox2;
    CCEditBox* m_editBox3;
    CCTableView* m_tabView;
    
    void sendRedPacket();
    int m_channel;//0王国1联盟
    int m_goldNum;
    int m_redNum;
    ToolInfo m_info;
    
    string max_gold;
    string max_red;
    string min_red;
    string max_hour;
};

class HongBaoUseViewCell : public CCTableViewTouchIFCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static HongBaoUseViewCell *create(int idx);
    
    void setData(int idx);
    int cellIndex;
private:
    HongBaoUseViewCell(){};
    virtual bool init(int idx);
    
    virtual void onEnter();
    virtual void onExit();
    
    void updateSelectInfo(CCObject *obj);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    CCSafeObject<CCSprite> m_selectSpr;
    CCSafeObject<CCSprite> m_selectSprBg;
    CCSafeObject<CCNode> m_headIconNode;
    CCSafeObject<CCLabelIF> m_upTxt;
    CCSafeObject<CCLabelIF> m_nameTxt;
    
    CCSafeObject<CCSprite> m_iconCover;
};

#endif /* HongBaoUseView_h */
