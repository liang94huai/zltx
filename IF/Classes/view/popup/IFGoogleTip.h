//
//  IFGoogleTip.h
//  IF
//
//  Created by wangdianzhen on 15/8/19.
//
//

#ifndef __IF__IFGoogleTip__
#define __IF__IFGoogleTip__

#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "ParticleController.h"
#include "CCAniNode.h"
#include "CCClipNode.h"
#include "HFHeadImgNode.h"
class IFGoogleTip:public  CCAniNode
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    IFGoogleTip(){};
    ~IFGoogleTip(){};
    bool initData();
    static IFGoogleTip* create();
 
private:
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);

    
    CCSafeObject<Node> m_touchNode;
    CCSafeObject<CCLabelIF> m_loginLb;
    CCSafeObject<CCLabelIF> m_secondLb;
    CCSafeObject<CCLabelIF> m_thirdLb;
    CCSafeObject<CCControlButton> m_loginBtn;
    CCSafeObject<CCControlButton> m_secondBtn;
    CCSafeObject<CCControlButton> m_thirdBtn;
    
    
    CCSafeObject<CCSprite> m_sp1;
    CCSafeObject<CCSprite> m_sp2;
    CCSafeObject<CCSprite> m_sp3;
    
    
    void onLoginBtnClick(CCObject* pSender,CCControlEvent event);
    void onSecondBtnClick(CCObject* pSender,CCControlEvent event);
    void onThirdBtnClick(CCObject* pSender,CCControlEvent event);

    
    void removeOneself();
    int m_defaultWidth;
    int m_state;
    
};

#endif /* defined(__IF__IFGoogleTip__) */
