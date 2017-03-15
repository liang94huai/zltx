//
//  GoldExchangeItemView_NEW.hpp
//  IF
//
//  Created by xxrdsg on 15/11/3.
//
//

#ifndef GoldExchangeItemView_NEW_hpp
#define GoldExchangeItemView_NEW_hpp

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"
#include "CCTableViewTouchIFCell.h"

class GoldExchangeItemView_NEW : public Layer
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    GoldExchangeItemView_NEW(GoldExchangeItem *goldExchangeItem):m_currdataItem(NULL){
        m_dataItem = goldExchangeItem;
        _ignoreAnchorPointForPosition = false;
        setAnchorPoint(Vec2(0,0));
    }
    static GoldExchangeItemView_NEW* create(GoldExchangeItem *goldExchangeItem);
    void setData(GoldExchangeItem* goldExchangeItem);
    
    string getTwoLines(string dollar);
protected:
private:
    
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void refreshData(CCObject* p = NULL);
    
    void onClickBlueBtn(Ref* pSender, Control::EventType eventType);
    void onClickGreenBtn(Ref* pSender, Control::EventType eventType);
    void payClick();
    
    CCSafeObject<Node> m_iconNode;
    
    CCSafeObject<CCLabelIF> m_desText;
    CCSafeObject<CCLabelIF> m_goldNum;
    CCSafeObject<CCLabelIF> m_goldAddNum;
    
    CCSafeObject<ControlButton> m_blueBtn;
    CCSafeObject<ControlButton> m_greenBtn;
    CCSafeObject<CCLabelIF> m_dollerNum;
    
    CCSafeObject<Node> m_hotNode;
    CCSafeObject<CCLabelIF> m_hotLabel;
    
    GoldExchangeItem *m_dataItem;
    GoldExchangeItem *m_currdataItem;
};

#endif /* GoldExchangeItemView_NEW_hpp */
