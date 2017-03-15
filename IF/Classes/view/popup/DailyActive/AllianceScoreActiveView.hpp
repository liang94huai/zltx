//
//  AllianceScoreActiveView.hpp
//  IF
//
//  Created by 付彬 on 15/12/2.
//
//

#ifndef AllianceScoreActiveView_hpp
#define AllianceScoreActiveView_hpp

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "TimeExchangeView.h"
#include "DailyRwdView.h"
#include "ActivityEventObj.h"
#include "ActivityInfo.h"
#include "SingleScoreActiveView.hpp"

class AllianceScoreActiveView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static AllianceScoreActiveView* create(ActivityEventObj* info, bool isInner=false);
    AllianceScoreActiveView(){};
    virtual ~AllianceScoreActiveView(){};
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(ActivityEventObj* info, bool isInner);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void OnTipBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void retData(CCObject* param);
    void retRankData(CCObject* param);
    void refreshData();
    void clickShowRD(int day);
    void onGameTick(float time);
    
    CCSafeObject<CCNode> m_topNode;
    CCSafeObject<CCNode> m_midNode;
    
    CCSafeObject<CCNode> m_tabList;
    CCSafeObject<CCNode> m_infoList;
    
    CCSafeObject<CCScale9Sprite> m_viewBg;
        CCSafeObject<CCNode> m_bgNode;
    //    CCSafeObject<CCNode> m_particleNode;
    
    CCSafeObject<CCControlButton> m_tipBtn;
    CCSafeObject<CCLabelIF> m_subTitleLabel;
    CCSafeObject<CCLabelIF> m_sub2TitleLabel;
    CCSafeObject<CCLabelIF> m_msgLabel;
    CCSafeObject<CCLabelIF> m_ttLabel;
    CCSafeObject<CCLabelIF> m_scoreTLabel;
    CCSafeObject<CCLabelIF> m_scoreLabel;
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCScale9Sprite> m_bar;
    CCSafeObject<CCScale9Sprite> m_bar2;
    
    CCSafeObject<CCSprite> m_scoreIcon;
    CCSafeObject<CCSprite> m_box1OpenIcon;
    CCSafeObject<CCSprite> m_box2OpenIcon;
    CCSafeObject<CCSprite> m_box3OpenIcon;
    
    CCSafeObject<CCLabelIF> m_cup1Label;
    CCSafeObject<CCLabelIF> m_cup2Label;
    CCSafeObject<CCLabelIF> m_cup3Label;
    
    CCSafeObject<CCNode> m_touch1Node;
    CCSafeObject<CCNode> m_touch2Node;
    CCSafeObject<CCNode> m_touch3Node;
    CCSafeObject<CCNode> m_touch4Node;
    
    CCSafeObject<CCSprite> m_arrowSpr;
    CCSafeObject<CCLabelIF> m_pt1Label;
    CCSafeObject<CCLabelIF> m_pt2Label;
    CCSafeObject<CCLabelIF> m_pt3Label;
    CCSafeObject<CCLabelIF> m_pt4Label;
    
    CCSafeObject<CCScrollView> m_topScrollView;
    CCSafeObject<CCScrollView> m_scrollView;
    
    WaitInterface* m_waitInterface;
    int m_type;
    int m_panelType;
    int m_startY;
    string m_guideKey;
    ActivityEventObj* m_actEventObj;
    ActivityInfo* m_actInfo;
};

#endif /* AllianceScoreActiveView_hpp */
