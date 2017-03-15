//
//  AllianceScienceTreeView.hpp
//  IF
//
//  Created by 邢晓瑞 on 16/1/8.
//
//

#ifndef AllianceScienceTreeView_hpp
#define AllianceScienceTreeView_hpp

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCAniNode.h"
#include "CCMathUtils.h"

class AllianceScienceTreeView:
public PopupBaseView,
public CCBSelectorResolver,
public CCBMemberVariableAssigner
{
public:
    AllianceScienceTreeView(int tab): m_tab(tab){};
    static AllianceScienceTreeView* create(int tab);
    bool init();
    void onEnter();
    void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void refreshView();
protected:
private:
    CCSafeObject<Node> m_bgContainer;
    CCSafeObject<Node> m_infoList;
    CCSafeObject<Node> m_bottomNode;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<Node> m_fireNode1;
    CCSafeObject<Node> m_fireNode2;
    
    CCSafeObject<ScrollView> m_scrollView;
    CCSafeObject<Node> m_mainNode;
    CCSafeObject<SpriteBatchNode> m_lineBatch;
    int m_tab;
};

class AllianceScienceTreeCell: public CCAniNode, public CCBMemberVariableAssigner
{
public:
    AllianceScienceTreeCell(int itemId): m_id(itemId){};
    static AllianceScienceTreeCell* create(int);
    bool init();
    void onEnter();
    void onExit();
    void refresh();
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onGetMsgRecSciEnd(Ref*);
    
    CCSafeObject<Node> m_particleNode;
    CCSafeObject<Sprite> m_bg;
    CCSafeObject<Node> m_completePar;
    CCSafeObject<Node> m_picNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_lvLabel;
    CCSafeObject<Node> m_mainNode;
    CCSafeObject<Sprite> m_lockSpr;
    CCSafeObject<Sprite> m_newIcon;
    CCSafeObject<Sprite> m_timershow;
    CCSafeObject<CCLabelIF> m_timecntslabel;
    CCSafeObject<Node> m_bottomNode;
    
    int m_id;
};

#endif /* AllianceScienceTreeView_hpp */
