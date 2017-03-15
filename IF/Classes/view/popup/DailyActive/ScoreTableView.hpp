//
//  ScoreTableView.hpp
//  IF
//
//  Created by 付彬 on 16/1/5.
//
//

#ifndef ScoreTableView_hpp
#define ScoreTableView_hpp

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "ActivityEventObj.h"
#include "ActivityInfo.h"

class ScoreTableView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static ScoreTableView* create(ActivityEventObj* info);
    ScoreTableView(){};
    virtual ~ScoreTableView(){};
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(ActivityEventObj* info);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onHeroBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onAllianceBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onCountryBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onUpdateView();
    
    CCSafeObject<CCNode> m_bodyNode;
    CCSafeObject<CCNode> m_btnNode;
    CCSafeObject<CCControlButton> m_heroBtn;
    CCSafeObject<CCControlButton> m_allianceBtn;
    CCSafeObject<CCControlButton> m_countryBtn;
    
    CCSafeObject<CCNode> m_tabList;
    CCSafeObject<CCScrollView> m_topScrollView;
    
    ActivityEventObj* m_actHeroObj;
    ActivityEventObj* m_actAllianceObj;
    ActivityEventObj* m_actCountryObj;
    ActivityEventObj* m_actEventObj;
};

#endif /* ScoreTableView_hpp */
