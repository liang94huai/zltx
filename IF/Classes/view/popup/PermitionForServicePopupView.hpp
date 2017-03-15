//
//  PermitionForServicePopupView.hpp
//  IF
//
//  Created by 〜TIAN~ on 15/11/5.
//
//

#ifndef PermitionForServicePopupView_hpp
#define PermitionForServicePopupView_hpp

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "WorldController.h"

class PermitionForServicePopupView : public PopupBaseView
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
{
public:
    static PermitionForServicePopupView* create();
    PermitionForServicePopupView(){};
    ~PermitionForServicePopupView(){};
private:
    virtual void onEnter();
    virtual void onExit();
    
    bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    void onBtn1Click(CCObject * pSender, Control::EventType pCCControlEvent);
private:
    CCSafeObject<Label> m_title1;
    CCSafeObject<Node> m_viewContainer1;
    CCSafeObject<ControlButton> m_btn1;
    CCSafeObject<Label> m_contentLabel;
    CCSafeObject<Node> m_labelNode;
};

#endif /* PermitionForServicePopupView_hpp */
