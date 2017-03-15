//
//  IFPanelLayer.hpp
//  IF
//
//  Created by wangdianzhen on 16/1/13.
//
//

#ifndef IFPanelLayer_hpp
#define IFPanelLayer_hpp

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "DefaultTableViewDelegate.h"
#include "CCTableViewTouchIFCell.h"
enum LayerType{
    TYPE_BLACK,
    TYPE_WHITE
};
class IFPanelLayer: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate{
public:
    static IFPanelLayer* create(LayerType type);
    IFPanelLayer(LayerType type):m_type(type){};
    virtual ~IFPanelLayer();
    void updateInfo();
    void onClickStoreBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickBagBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onTipBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
protected:
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    
    
private:
    virtual void onEnter();
    virtual void onExit();
    void refreashData();
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    void onCloseClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void updateAlliances(CCObject* data);
    
    CCSafeObject<CCControlButton> m_storeBtn;
    CCSafeObject<CCControlButton> m_bagBtn;
    CCSafeObject<CCNode> m_BGNode;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;
    CCSafeObject<ControlButton> m_btn;
    CCSafeObject<CCLabelIF> m_titleTxt;
    
    
    CCSafeObject<CCArray> allianceArray;
    CCSafeObject<CCTableView> m_tabView;
    int m_type;
    int m_total_receive;
};



class IFListCell :
public CCTableViewTouchIFCell

,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static IFListCell* create(AllianceInfo* info,CCNode* touchNode);
    IFListCell(AllianceInfo* info,CCNode* touchNode):m_info(info),m_touchNode(touchNode){};
    void updateAllianceInfo(AllianceInfo* info);
    void setHighlight();
    void unHighlight();
    
private:
    bool init();
    
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

    
    CCSafeObject<CCLabelIFTTF> m_nameTxt;
    CCSafeObject<CCLabelIFTTF> m_leaderTxt;
    CCSafeObject<CCLabelIFTTF> m_allianceNumTxt;
    CCSafeObject<CCLabelIFTTF> m_forceTxt;
    CCSafeObject<CCLabelIFTTF> m_appTxt;
    CCSafeObject<CCNode> m_btnView;
    CCSafeObject<CCNode> m_RestrictIcon;

    CCSafeObject<CCNode> m_appNode;
    CCSafeObject<CCNode> m_allianceIcon;
    CCSafeObject<CCNode> m_clickNode;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCLabelIFTTF> m_language;
    CCSafeObject<CCLabelIFTTF> m_allianceTxt;

    
    
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<AllianceInfo> m_info;
    CCPoint m_touchPos;
};





#endif /* IFPanelLayer_hpp */
