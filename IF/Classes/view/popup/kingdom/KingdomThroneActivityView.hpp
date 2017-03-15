//
//  KingdomThroneActivityView.hpp
//  IF
//
//  Created by ganxiaohua on 15/12/1.
//
//

#ifndef KingdomThroneActivityView_hpp
#define KingdomThroneActivityView_hpp

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCMultiColTableView.h"
#include "KingdomExecCommand.h"
#include "OfficeInfo.h"

class KingdomThroneActivityView : public PopupBaseView
, public CCBSelectorResolver

, public CCBMemberVariableAssigner
{
public:
    static KingdomThroneActivityView *create();
    KingdomThroneActivityView(){};
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
    void onClickJoin(CCObject *pSender, CCControlEvent event);
    void onClickRank(CCObject *pSender, CCControlEvent event);
    void onCloseClick(CCObject *pSender, CCControlEvent event);
    void updateTime(float _time);
    
    CCSafeObject<CCNode> m_viewBg;
     CCSafeObject<CCLabelIF> m_battleTile;
    CCSafeObject<CCLabelIF> m_battleDesc;
    CCSafeObject<CCLabelIF> m_selfIntro;
    CCSafeObject<CCLabelIF> targetIntro;
    CCSafeObject<CCControlButton> m_btnJoin;
    CCSafeObject<CCControlButton> m_btnRank;
    CCSafeObject<CCSprite> m_activityBg;
    
    double m_battlesStartTime;
    std::string m_serverName;
    int m_targetServerId;
};
#endif /* KingdomThroneActivityView_hpp */
