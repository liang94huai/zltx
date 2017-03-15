//
//  EquipLongjingView.hpp
//  IF
//
//  Created by 付彬 on 15/12/15.
//
//

#ifndef EquipLongjingView_hpp
#define EquipLongjingView_hpp

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCClipNode.h"

class EquipLongjingView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static EquipLongjingView* create();
    CCNode* getGuideNode(string _key);
protected:
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickSite1Btn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickSite2Btn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickSite3Btn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickSite4Btn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickSite5Btn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onOkBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickHelpBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onYesDestroyLongEquip();
    
    void onRefreshData(int type);
    void onShowData();
    void onRetSellEquip(CCObject* obj);
    void onRetBuyEquip(CCObject* obj);
    
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_infoNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCControlButton> m_btn;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    
    CCSafeObject<CCControlButton> m_color1Btn;
    CCSafeObject<CCControlButton> m_color2Btn;
    CCSafeObject<CCControlButton> m_color3Btn;
    CCSafeObject<CCControlButton> m_color4Btn;
    CCSafeObject<CCControlButton> m_color5Btn;
    CCSafeObject<CCControlButton> m_helpBtn;
    CCSafeObject<CCNode> m_parNode;
    
    CCSafeObject<CCScrollView> m_scrollView;
    
    int m_itemId;
    int m_curIdx;
    int m_page;
    int m_extH;
    bool m_isInitEnd;
    
    vector<int> m_curList;
    int m_sumLongPrice;
    WaitInterface* m_waitInterface;
};

class EquipLongjingCell :
public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCAniNode
{
public:
    static EquipLongjingCell* create(int itemId, int idx);
    EquipLongjingCell():m_itemId(0),m_idx(0){};
    void setData(int itemId, int idx);
    bool cellTouchEnded(CCTouch* pTouch);
    
private:
    bool init(int itemId, int idx);
    virtual void onEnter();
    virtual void onExit();
    
    void onBuyTool(CCObject *ccObj);
    void onCloseConfirm();
    void makeDataState();
    void onRetBuyTool(CCObject* obj);
    void setPutOn(CCObject* obj);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_particleNode;
    
    CCSafeObject<CCNode> m_lineNode;
    CCSafeObject<CCNode> m_subLineNode;
    
    CCSafeObject<CCNode> m_openLNode;
    CCSafeObject<CCNode> m_openSubLNode;
    
    CCSafeObject<CCNode> m_parNode;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCNode> m_putMarkNode;
    CCSafeObject<CCSprite> m_newTipSpr;
    
    int m_itemId;
    int m_equipId;
    int m_lockEquipId;
    int m_idx;
    int m_state;
    string m_equipUUid;
    bool m_isClick;
};

#endif /* EquipLongjingView_hpp */
