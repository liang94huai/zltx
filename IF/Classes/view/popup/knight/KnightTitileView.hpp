//
//  KnightTitileView.hpp
//  IF
//
//  Created by 付彬 on 16/1/7.
//
//

#ifndef KnightTitileView_hpp
#define KnightTitileView_hpp

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "KnightTitleInfo.hpp"

class KnightTitileView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static KnightTitileView* create(int knightId=0);
    KnightTitileView(){};
    virtual ~KnightTitileView(){};
    CCNode* getGuideNode(string _key);
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(int knightId);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
//    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void onClickListBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickComposeBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void retData(CCObject* param);
    
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_bottomNode;
    
    CCSafeObject<CCControlButton> m_listBtn;
    CCSafeObject<CCControlButton> m_composeBtn;
    
    CCSafeObject<CCScrollView> m_scrollView;
    
    WaitInterface* m_waitInterface;
    int m_knightId;
};

#endif /* KnightTitileView_hpp */
