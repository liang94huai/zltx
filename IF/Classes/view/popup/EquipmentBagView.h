//
//  EquipmentBagView.h
//  IF
//
//  Created by fubin on 14-12-9.
//
//

#ifndef IF_EquipmentBagView_h
#define IF_EquipmentBagView_h

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "CCTableViewTouchIFCell.h"
#include "ToolController.h"
#include "EquipmentInfo.h"

class EquipmentBagView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
//, public CCTouchDelegate
,public CCTableViewDataSource
,public DefaultTableViewDelegate

{
public:
    static EquipmentBagView* create(bool isFromWorkshop = false);
    EquipmentBagView(){};
    virtual ~EquipmentBagView(){};
protected:
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(bool isFromWorkshop);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onClickMateBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickEquiBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickHelpBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void refreshView();
    void changeMateCnt(CCObject* obj);
    void refreashData(CCObject* obj);
    
    CCSafeObject<CCArray>m_data;
    
    CCSafeObject<CCNode> m_BGNode;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCScale9Sprite> m_bg2;
    CCSafeObject<CCControlButton> m_mateBtn;
    CCSafeObject<CCLabelIF> m_mateBtnTxt;
    CCSafeObject<CCControlButton> m_equiBtn;
    CCSafeObject<CCLabelIF> m_equiBtnTxt;
    CCSafeObject<CCControlButton> m_helpBtn;
    
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;
    
    CCTableView* m_tabView;
    
    void generateData();
    int m_page;
    int m_openNum;
    CCPoint m_startPoint;
    
    bool m_isFromWorkshop;
    void setFromWorkshop(CCObject *obj);
    void setRefreshView(CCObject *obj);
};

//...EquipmentBagCell class
class EquipmentBagCell :
public CCBSelectorResolver
 
,public CCBMemberVariableAssigner
,public CCTableViewTouchIFCell
{
public:
    static EquipmentBagCell* create(string itemId, int page, CCNode* touchNode);
    EquipmentBagCell():m_items(""),m_itemId(0),m_touchNode(NULL){};
    void setData(string itemId, int page);
    int m_itemId;
    
private:
    bool init(string itemId, int page, CCNode* touchNode);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickCrtBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void refreashData(CCObject* obj);
    void showDesByIndex(int index);
    void refreshItemData(int index, bool st=false);
    
    CCSafeObject<CCNode> m_itemNode1;
    CCSafeObject<CCNode> m_itemNode2;
    CCSafeObject<CCNode> m_itemNode3;
    CCSafeObject<CCNode> m_itemNode4;
    CCSafeObject<CCNode> m_itemNode5;
    CCSafeObject<CCNode> m_itemsNode;
    
    CCSafeObject<CCNode> m_picNode1;
    CCSafeObject<CCNode> m_picNode2;
    CCSafeObject<CCNode> m_picNode3;
    CCSafeObject<CCNode> m_picNode4;
    CCSafeObject<CCNode> m_picNode5;
    CCSafeObject<CCLabelIF> m_numLabel1;
    CCSafeObject<CCLabelIF> m_numLabel2;
    CCSafeObject<CCLabelIF> m_numLabel3;
    CCSafeObject<CCLabelIF> m_numLabel4;
    CCSafeObject<CCLabelIF> m_numLabel5;
    CCSafeObject<CCSprite> m_picBG1;
    CCSafeObject<CCSprite> m_picBG2;
    CCSafeObject<CCSprite> m_picBG3;
    CCSafeObject<CCSprite> m_picBG4;
    CCSafeObject<CCSprite> m_picBG5;
    CCSafeObject<CCSprite> m_barBG1;
    CCSafeObject<CCSprite> m_barBG2;
    CCSafeObject<CCSprite> m_barBG3;
    CCSafeObject<CCSprite> m_barBG4;
    CCSafeObject<CCSprite> m_barBG5;
    CCSafeObject<CCSprite> m_selectSpr;
    
    CCSafeObject<CCSprite> m_wor1;
    CCSafeObject<CCSprite> m_wor2;
    CCSafeObject<CCSprite> m_wor3;
    CCSafeObject<CCSprite> m_wor4;
    CCSafeObject<CCSprite> m_wor5;
    CCSafeObject<CCSprite> m_yes1Spr;
    CCSafeObject<CCSprite> m_yes2Spr;
    CCSafeObject<CCSprite> m_yes3Spr;
    CCSafeObject<CCSprite> m_yes4Spr;
    CCSafeObject<CCSprite> m_yes5Spr;
    
    CCSafeObject<CCNode> m_desNode;
    CCSafeObject<CCScale9Sprite> m_itemBg;
    CCSafeObject<CCSprite> m_arrowSpr;
    
    CCSafeObject<CCNode> m_equipNode;//装备展示内容
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_desLabel;
    
    CCSafeObject<CCNode> m_mateNode;//材料合成展示内容
    CCSafeObject<CCLabelIF> m_nameMateLabel;
    CCSafeObject<CCControlButton> m_crtBtn;
    CCSafeObject<CCNode> m_mateNode1;
    CCSafeObject<CCNode> m_mateNode2;
    CCSafeObject<CCNode> m_mateNode3;
    CCSafeObject<CCNode> m_mateNode4;
    CCSafeObject<CCNode> m_mateNode5;
    
    CCSafeObject<CCNode> m_touchNode;
    string m_items;
    int m_page;
    vector<string> m_idsVec;
    int m_selIndex;
    
    int m_boostBid;
    bool m_clickInSide;
    
    CCPoint m_startPoint;
    WaitInterface* m_waitInterface;
};

class MaterialInfoView :
public CCBSelectorResolver
,public CCBMemberVariableAssigner
 
,public PopupBaseView
{
public:
    static MaterialInfoView* create(int itemId);
    MaterialInfoView():m_itemId(-1){};

private:
    bool init(int itemId);
    
    
    void setData(int itemId);

    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickMateBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCControlButton> m_mateBtn;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_name1Label;
    CCSafeObject<CCLabelIF> m_info1Label;
    CCSafeObject<CCLabelIF> m_info2Label;
    CCSafeObject<CCLabelIF> m_numLabel;
    
    CCSafeObject<CCLabelIF> m_msg1Label;
    CCSafeObject<CCLabelIF> m_msg2Label;
    CCSafeObject<CCNode> m_picNode;
    
    WaitInterface* m_waitInterface;
    
    int m_itemId;
    
};

class EquipBreakItemView :
public CCBSelectorResolver
,public CCBMemberVariableAssigner
 
,public PopupBaseView
, public CCTableViewDataSource
{
public:
    static EquipBreakItemView* create(string data);
    EquipBreakItemView(){};
    void setData(string data);
    
protected:
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    
private:
    bool init(string data);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickOKBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCControlButton> m_okBtn;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_msg1Label;
    CCSafeObject<CCLabelIF> m_msg2Label;
    CCSafeObject<CCNode> m_infoList;
    
    CCSafeObject<CCTableView> m_tabView;
    vector<int> m_data;
    vector<int> m_numVec;
    
    int m_itemId;
};

class EquipMateCell :
public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewCell
{
public:
    static EquipMateCell* create(int itemId, int num);
    EquipMateCell():m_itemId(0), m_num(0){};
    void setData(int itemId, int num);
private:
    bool init(int itemId, int num);
    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_levelLabel;
    
    int m_itemId;
    int m_num;
};

#endif
