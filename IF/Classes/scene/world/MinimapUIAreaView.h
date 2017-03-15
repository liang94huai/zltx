//
//  MinimapAreaView.h
//  IF
//
//  Created by 刘田厚 on 17-1-4.
//
//

#ifndef __IF__MinimapAreaView__
#define __IF__MinimapAreaView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "HFViewport.h"
#include "WorldController.h"
#include "DefaultTableViewDelegate.h"
#include "CCTableViewTouchIFCell.h"



class MinimapAreaView : public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate


{
public:
    static MinimapAreaView *create();
   
     
private:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    MinimapAreaView();
    ~MinimapAreaView();
    
protected:
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view);
    

    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);

   
    
    
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

    
    
    void onReturnBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
   
   
    void initLabel();
    
    void closeUIView();
   
private:
    static int cmp(const std::pair<int, double>& x, const std::pair<int, double>& y);
    void sortMapByValue();
    void updateTime(float dt);
private:
    
    CCSafeObject<CCScale9Sprite> m_buildBG2;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCNode> m_flowerNode;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCControlButton> m_closeBtn;
    
    CCSafeObject<CCLabelIF> m_title_1;
    CCSafeObject<CCLabelIF> m_title_2;
    CCSafeObject<CCLabelIF> m_title_3;
    CCSafeObject<CCLabelIF> m_title_4;
    CCSafeObject<CCLabelIF> m_title_5;
    CCSafeObject<CCLabelIF> m_no_lab;
    
    CCSafeObject<CCNode> m_noInfo;
    CCTableView* m_tabView;
    
    std::vector<std::pair<int, double>> m_VectRes;
   
};

class MiniAreaCell:
public CCBSelectorResolver

,public CCBMemberVariableAssigner
,public CCTableViewTouchIFCell
{
public:
    static MiniAreaCell* create(int cellIdx,PopupBaseView* view);
    MiniAreaCell(int cityId,PopupBaseView* view):m_iResidx(cityId),m_ccbView(view){};
    ~MiniAreaCell(){};
    
    
    void setData( int iIndex);
    
private:
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onGotoBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void initLab();
    
    CCSafeObject<CCScale9Sprite> m_redDi;
    CCSafeObject<CCScale9Sprite> m_bg1;
    CCSafeObject<CCScale9Sprite> m_bg2;
    
    CCSafeObject<CCLabelIF> m_resName;
    CCSafeObject<CCLabelIF> m_tilename;
    CCSafeObject<CCLabelIF> m_armNum;
    CCSafeObject<CCLabelIF> m_protime;
    CCSafeObject<CCLabelIF> m_gotoLab;
    CCSafeObject<PopupBaseView> m_ccbView;
    
    CCSafeObject<CCControlButton> m_GotoBtn;
    int      m_iResidx; //Cell idx
    int      m_iCellidx; //resIdx

    
    
    
    
    
};
#endif /* defined(__IF__MinimapAreaView__) */
