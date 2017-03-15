//
//  AllianceScienceClassView.hpp
//  IF
//
//  Created by 邢晓瑞 on 16/1/8.
//
//

#ifndef AllianceScienceClassView_hpp
#define AllianceScienceClassView_hpp

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "ScienceListView.h"

class AllianceScienceClassView: public PopupBaseView, public CCBSelectorResolver, public CCBMemberVariableAssigner
{
public:
    AllianceScienceClassView(): m_OpenTab(0)
    {
        memset(m_BtnInfo, 0, sizeof(m_BtnInfo));
    }
    CREATE_FUNC(AllianceScienceClassView);
    bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onBtn1Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtn2Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtn3Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtn4Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickBtn(int i);
    
    void onFadeInCallBack();
    void onFadeOutCallBack();
    bool isTabValid(int i);
    
protected:
private:
    CCSafeObject<Node> m_bgContainer;
    CCSafeObject<Node> m_mainNode;
    CCSafeObject<CCLabelIF> m_hintText;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<Node> m_fireNode1;
    CCSafeObject<Node> m_fireNode2;
    
//    CCSafeObject<Node> m_node1;
//    CCSafeObject<Node> m_crNode1;
//    CCSafeObject<ControlButton> m_btn1;
//    CCSafeObject<Node> m_icon1Node;
//    CCSafeObject<Scale9Sprite> m_name1BG;
//    CCSafeObject<CCLabelIF> m_name1Label;
    
    SBtnAniInfo m_BtnInfo[4];
    
    int m_OpenTab;
};

#endif /* AllianceScienceClassView_hpp */
