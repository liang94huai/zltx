//
//  NewTroopsView.h
//  IF
//
//  Created by xxrdsg on 15-8-31.
//
//

#ifndef __IF__NewTroopsView__
#define __IF__NewTroopsView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "MarchDetailView.h"

#define MSG_OUT_TRAP_TITLE_TOUCH "MSG_OUT_TRAP_TITLE_TOUCH"
#define MSG_TROOP_NUMS_REFRESH "msg_troop_nums_refresh"

class NewTroopsView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCScrollViewDelegate
{
public:
    static NewTroopsView* create();
    NewTroopsView(){};
    void refreshView(CCObject*);
    
    virtual void onEnter();
    virtual void onExit();
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    Node* getGuideNode(string _key);
    
    void onGetMsgMarchData(Ref* ref);
    void onGetMsgMarchTitleTouch(Ref* ref);
    void addNewDizi(CCScale9Sprite* sprite);
protected:
    
private:
    
    CCSafeObject<CCNode> m_listNode;
    
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCNode> m_guideNode1;
    CCSafeObject<CCNode> m_guideNode2;
    map<string, MarchDetailInfo> m_marchInfo;
    map<string, bool> m_bOpen;
};

class NewTroopsCell: public CCIFTouchNode
, public CCBMemberVariableAssigner
{
public:
    static NewTroopsCell* create(string identity, int cnt = -1);
    NewTroopsCell(string identity, int cnt): m_id(identity), m_cnt(cnt), m_iconGrey(false){};
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
protected:
private:
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCLabelIF> m_levelLabel;
    string m_id;
    bool m_iconGrey;
    
    int m_cnt;//部队数量 默认为-1
};

class NewTroopsBtn: public CCNode, public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    CREATE_FUNC(NewTroopsBtn);
    virtual bool init();
    
    void onFormation1Click(CCObject *pSender, CCControlEvent event);
    void onFormation2Click(CCObject *pSender, CCControlEvent event);
    void onFormation3Click(CCObject *pSender, CCControlEvent event);
    void onFormation4Click(CCObject *pSender, CCControlEvent event);
    void onFormation5Click(CCObject *pSender, CCControlEvent event);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
protected:
private:
    CCSafeObject<Node> m_formationNode;
    CCSafeObject<Node> m_formation5;
    CCSafeObject<Node> m_formation4;
    CCSafeObject<Node> m_formation3;
    CCSafeObject<Node> m_formation2;
    CCSafeObject<Node> m_formation1;
    CCSafeObject<CCLabelIF> m_formationLab5;
    CCSafeObject<CCLabelIF> m_formationLab4;
    CCSafeObject<CCLabelIF> m_formationLab3;
    CCSafeObject<CCLabelIF> m_formationLab2;
    CCSafeObject<CCLabelIF> m_formationLab1;
};

class NewTroopsArmyTitle: public CCNode, public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    static NewTroopsArmyTitle* create();
    virtual bool init();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
protected:
private:
    CCSafeObject<CCLabelIF> m_armyLabel;
    CCSafeObject<CCLabelIF> m_armyNum;
    CCSafeObject<CCLabelIF> m_marchLabel;
    CCSafeObject<CCLabelIF> m_marchNum;
    CCSafeObject<CCLabelIF> m_woundedLabel;
    CCSafeObject<CCLabelIF> m_woundedNum;
    CCSafeObject<CCLabelIF> m_foodSpdLabel;
    CCSafeObject<CCLabelIF> m_foodSpd;
    CCSafeObject<CCLabelIF> m_myTroopLabel;
};

class NewTroopsTrapTitle: public CCNode
, public CCBMemberVariableAssigner
{
public:
    static NewTroopsTrapTitle* create();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
protected:
private:
    CCSafeObject<CCLabelIF> m_tipLabel;
    CCSafeObject<CCLabelIF> m_trapLabel;
    CCSafeObject<CCLabelIF> m_trapNum;
};

class NewTroopsMarchTitle: public CCIFTouchNode
, public CCBMemberVariableAssigner
{
public:
    NewTroopsMarchTitle(int idx, int num, int state, string marchUid): m_idx(idx), m_num(num), m_state(state), m_marchUid(marchUid){};
    static NewTroopsMarchTitle* create(int idx, int num, int state, string marchUid)
    {
        auto ret = new NewTroopsMarchTitle(idx, num, state, marchUid);
        if (ret && ret->init())
        {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    void setRotate();
    
    virtual void onEnter();
    virtual void onExit();
protected:
private:
    CCSafeObject<CCLabelIF> m_trapLabel;
    CCSafeObject<CCLabelIF> m_trapNum;
    CCSafeObject<CCLabelIF> m_stateLabel;
    CCSafeObject<Node> m_touchNode;
    CCSafeObject<Sprite> m_arrow;
    
    int m_idx;
    int m_num;
    int m_state;
    string m_marchUid;
};

class NewTroopsMarchCell: public Node
, public CCBMemberVariableAssigner
{
public:
    NewTroopsMarchCell(string armyId, int num): m_armyId(armyId), m_num(num){}
    static NewTroopsMarchCell* create(string armyId, int num)
    {
        auto ret = new NewTroopsMarchCell(armyId, num);
        if (ret && ret->init())
        {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
protected:
private:
    CCSafeObject<Node> m_iconNode;
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCLabelIF> m_levelLabel;
    
    string m_armyId;
    int m_num;
};

#endif /* defined(__IF__NewTroopsView__) */
