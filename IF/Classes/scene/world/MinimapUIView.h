//
//  MinimapUIView.h
//  IF
//
//  Created by 刘田厚 on 16-10-31.
//
//

#ifndef __IF__MinimapUIView__
#define __IF__MinimapUIView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "HFViewport.h"
#include "WorldController.h"
#include "DefaultTableViewDelegate.h"
#include "CCTableViewTouchIFCell.h"


#define  MINIMAP_UI_SELECT_POS "minmap.ui.sel.pos" //选择坐标

enum MarkType{
    MARK_ALLIANCE,  //联盟标记
    MARK_CITY,      //区域城池
    MARK_FAVOURATE, //个人
    MARK_POWER,     //势力
    
};

enum MapInfoType{
    SimType,        //13块 区域
    MoreType,       //62 块 小块
};

class MinimapUIView : public CCLayer
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCEditBoxDelegate
,public CCTableViewDataSource
,public DefaultTableViewDelegate


{
public:
    static MinimapUIView *create(int iServerID,int iMapIndex,PopupBaseView* view);
   
     
private:
    virtual bool init(int iServerID,int iMapIndex,PopupBaseView* view);
    virtual void onEnter();
    virtual void onExit();
    
    MinimapUIView();
    ~MinimapUIView();
    
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

    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    
    void editBoxReturn(CCEditBox *editBox);
    void editBoxEditingDidBegin(CCEditBox* editBox) {m_editOpen = true;}
    
   
    
    
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

    
    void onJumpBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onSerachBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onReturnBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onAllianceBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onPowerBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onCityBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onCheckBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onResBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);

    void setUIZorder(Node* node, float iOrder);
    void initEditBox();
    void initLabel();
    void addTableView();
    
    void showTop();
    void fadeTop();
    void showBotoom();
    void fadeBotoom();
    void showJianView();
    void showFanView();
    void showTableView();
    void fadeTableView();

    void updateTableView();
    void refreashData(CCObject* obj);

    void changeUIForMap();
    void closeUIView();
    void closeAllUIView();
    void jumpTarPos();
    void showSelfPower();
    
    void getShouldPos(Vec2 &curPos, CCSize size);
public:
    void jumpPoint(int x, int y, int serverID);
    
    
private:
    
    CCSafeObject<CCNode> m_topNode;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCNode> m_serachNode;
    CCSafeObject<CCNode> m_XeditNode;
    CCSafeObject<CCNode> m_YeditNode;
    CCSafeObject<CCNode> m_biaojiNode;
    CCSafeObject<CCNode> m_biaojiNode_2;
    CCSafeObject<CCNode> m_areaNode;
    CCSafeObject<CCNode> m_clipNode;

    CCSafeObject<CCEditBox> m_editXBox;
    CCSafeObject<CCEditBox> m_editYBox;
    
    CCSafeObject<CCControlButton> m_serachBtn;
    CCSafeObject<CCControlButton> m_jumpBtn;
    CCSafeObject<CCControlButton> m_returnBtn;
    
    CCSafeObject<CCSprite>  m_juanzhouSpr;
    CCSafeObject<CCSprite>  m_jianxiangSpr;
    CCSafeObject<CCSprite>  m_fanhuiSpr;
    
    CCSafeObject<PopupBaseView> m_ccbView;
    
    CCSafeObject<CCLabelIF> m_jumpLab;
    CCSafeObject<CCLabelIF> m_allianceLab;
    CCSafeObject<CCLabelIF> m_cityLab;
    CCSafeObject<CCLabelIF> m_checkLab;
    CCSafeObject<CCLabelIF> m_powerLab;
    CCSafeObject<CCLabelIF> m_MyResLab;

    
    
    //tableview
    CCSafeObject<CCNode> m_zoneNode;
    CCSafeObject<CCNode> m_listinfo;
    CCSafeObject<CCLabelIF> m_tileLab;
    CCSafeObject<CCLabelIF> m_noInfo;
    CCSafeObject<CCNode> m_tabTouchNode;
    
    
    CCTableView* m_tabView;
    
    Vec2 m_startMovePos;    //tableMove初始点
    Vec2 m_startZoneMovePos;    //tableMove初始点
    
    
    bool m_editOpen;
    bool m_TopOpen;
    bool m_BottomOpen;
    
    bool m_TableOpen;
    bool m_touchTable;
    bool m_tableIsMove;
    Vec2 m_TarPos;
    int  m_iServerID;
    int  m_iMapIndex;
    int  m_iSelectIndex;
    
    int  m_iTouchType;

    MarkType  m_MarkType;   //tableview 类型
    MapInfoType  m_mapInfoType; //显示详情
};

class MinmapUICell:
public CCBSelectorResolver

,public CCBMemberVariableAssigner
,public CCTableViewTouchIFCell
{
public:
    static MinmapUICell* create(MarkType iType, int cellIdx,int iSelectID, int imapIndex, CCNode* touchNode);
    MinmapUICell(): m_iType(MARK_ALLIANCE),m_iCellidx(0),m_touchNode(NULL),m_startPoint(0,0),m_clickInSide(false){};
    ~MinmapUICell(){};
    
    void showRed(bool isVisible);
    void setData(MarkType iType, int iIndex = -1);
    
private:
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(MarkType iType, int cellId,int iSelectID, int imapIndex,CCNode* touchNode);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    CCSafeObject<CCScale9Sprite> m_redDi;
    
    CCSafeObject<CCLabelIF> m_tilestate;
    CCSafeObject<CCLabelIF> m_tilename;
    CCSafeObject<CCLabelIF> m_tilepos;
    CCSafeObject<CCNode> m_clolorNode;
    CCSafeObject<CCLayerColor> m_layercolor;
    CCSafeObject<CCNode> m_touchNode;
    CCPoint m_startPoint;
    
    MarkType m_iType;
    int      m_iCellidx;
    int      m_iMapIndex;
    bool     m_clickInSide;
    
    
    
    
};
#endif /* defined(__IF__MinimapUIView__) */
