//
//  AddAllianceTipsView.hpp
//  IF
//
//  Created by 〜TIAN~ on 15/11/2.
//
//

#ifndef AddAllianceTipsView_hpp
#define AddAllianceTipsView_hpp

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "WorldController.h"

class AddAllianceTipsView : public PopupBaseView
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
{
public:
    static AddAllianceTipsView* create(int type);
    AddAllianceTipsView(int type){m_type  = type;};
    ~AddAllianceTipsView(){};
private:
    virtual void onEnter();
    virtual void onExit();
    
    bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    void onClickClose(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void joinSuccess(Ref* data);
private:
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_rewardLabel;
    CCSafeObject<CCLabelIF> m_tip1Label;
    CCSafeObject<CCLabelIF> m_tip2Label;
    CCSafeObject<CCLabelIF> m_tip3Label;
    CCSafeObject<CCLabelIF> m_tip4Label;
    CCSafeObject<Node> m_touchNode;
    CCSafeObject<CCControlButton> m_btnAlliance;
    
    
    int m_type;
};

#endif /* AddAllianceTipsView_hpp */
