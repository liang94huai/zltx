//
//  SkinListView.hpp
//  IF
//
//  Created by 邢晓瑞 on 15/11/18.
//
//

#ifndef SkinListView_hpp
#define SkinListView_hpp

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "ArcPopupBaseView.h"
#include "ArcScrollView.h"

class SkinListView: public ArcPopupBaseView
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    static SkinListView* create();
    bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(Ref * pTarget, const char * pMemberVariableName, Node * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(Ref * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(Ref * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    
    void onGetMsgDayCellSelect(Ref*);
    void onGetMsgDayCellUnselect(Ref*);
    void onGetMsgSkinConfigInit(Ref*);
    
    void arcScollCallBack();
    void onOkClick(Ref* pSender, Control::EventType pControlEvent);
    void onYes();
    void onUseTool();
    void sureToCancel();
    
    bool checkData();
    void refresh(int);
    void onGetMsgToolChange(Ref* ref);
    void onEnterFrame(float t);
    void refreshTime();
    void refreshBtn();
    void refreshDayCells();
    
    void getValidIds(vector<string>&);
    string getOwnId(vector<string>&);
protected:
private:
    
    CCSafeObject<Node> m_bottomNode;
    CCSafeObject<Scale9Sprite> m_buildBG;
    CCSafeObject<Node> m_dayNode;
    CCSafeObject<ControlButton> m_okBtn;
    CCSafeObject<CCLabelIF> m_okLabel;
    CCSafeObject<Node> m_goldNode;
    CCSafeObject<CCLabelIF> m_goldNum;
    CCSafeObject<Node> m_buyNode;
    
    CCSafeObject<Node> m_timeNode;
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCLabelIF> m_desLabel;
    
    CCSafeObject<Node> m_arcNode;
    
    CCSafeObject<ArcScrollView> m_arcScroll;
    
    
    vector<string> m_iconNames;
    vector<string> m_names;
    vector<string> m_deses;
    vector<string> m_skinNames;
    map< string, vector<string> > m_itemIds;
    map< string, string > permanentSkins;
    
    string m_curItemId;
    int m_curIdx;
    bool m_isByBuy;
};

class SkinListDayCell: public CCIFTouchNode
, public CCBMemberVariableAssigner
{
public:
    SkinListDayCell(string itemId, bool isSelected): m_itemId(itemId), m_isSelected(isSelected){};
    static SkinListDayCell* create(string itemId, bool isSelected);
    bool init();
    virtual bool onAssignCCBMemberVariable(Ref * pTarget, const char * pMemberVariableName, Node * pNode);
    
    virtual void onEnter();
    virtual void onExit();
    
    void onGetMsgOtherSelected(Ref* ref);
    
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void onTouchMoved(Touch * ptouch, Event * pEvent);
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    
    CCSafeObject<Node> m_touchNode;
    CCSafeObject<Sprite> m_spr;
    CCSafeObject<CCLabelIF> m_dayLabel;
    
    string m_itemId;
    bool m_isSelected;
};

#endif /* SkinListView_hpp */
