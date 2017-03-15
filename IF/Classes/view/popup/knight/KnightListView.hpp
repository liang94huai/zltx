//
//  KnightListView.hpp
//  IF
//
//  Created by 付彬 on 16/1/7.
//
//

#ifndef KnightListView_hpp
#define KnightListView_hpp

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "TimeExchangeView.h"
#include "KnightTitleInfo.hpp"

class KnightListView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static KnightListView* create(int knightId=0);
    KnightListView(){};
    virtual ~KnightListView(){};
    CCNode* getGuideNode(string _key);
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(int knightId);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void OnTipBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void retData(CCObject* param);
    
    CCSafeObject<CCNode> m_topNode;
    CCSafeObject<CCNode> m_infoList;
    
    CCSafeObject<CCScrollView> m_scrollView;
    
    WaitInterface* m_waitInterface;
    int m_knightId;
};

class KnightTitleCell : public CCAniNode
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static KnightTitleCell* create(int knightId);
    KnightTitleCell(){};
    virtual ~KnightTitleCell(){};
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(int bedgeId);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void retData(CCObject* param);
    
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCScale9Sprite> m_touchBg;
    CCSafeObject<CCControlButton> m_gotoBtn;
    CCSafeObject<CCLabelIF> m_titleLabel;
    int m_knightId;
};

#endif /* KnightListView_hpp */
