//
//  BedgeComposeView.hpp
//  IF
//
//  Created by 付彬 on 16/1/7.
//
//

#ifndef BedgeComposeView_hpp
#define BedgeComposeView_hpp

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "TimeExchangeView.h"
#include "KnightTitleInfo.hpp"
#include "CCClipNode.h"

class BedgeComposeView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static BedgeComposeView* create(int knightId=0);
    BedgeComposeView(){};
    virtual ~BedgeComposeView(){};
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

class BedgeBagNode : public CCAniNode
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static BedgeBagNode* create(int type);
    BedgeBagNode(){};
    virtual ~BedgeBagNode(){};
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(int type);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onClickLeftBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickRightBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    void updatePage(int page);
    
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCControlButton> m_leftBtn;
    CCSafeObject<CCControlButton> m_rightBtn;
    
    CCSafeObject<CCClipNode> m_clipNode;
    
    int m_type;
    int m_page;
    vector<int> m_bedgeIdVec;
};

class BedgeCell : public CCAniNode
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static BedgeCell* create(int bedgeId, int type);
    BedgeCell(){};
    virtual ~BedgeCell(){};
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(int bedgeId, int type);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void retData(CCObject* param);
    
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCScale9Sprite> m_touchBg;
    CCSafeObject<CCLabelIF> m_numLabel;
    int m_bedgeId;
    int m_type;
};

#endif /* BedgeComposeView_hpp */
