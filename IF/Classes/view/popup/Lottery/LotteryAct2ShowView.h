//
//  LotteryAct2ShowView.h
//  IF
//
//  Created by chenfubi on 15/2/6.
//
//

#ifndef __IF__LotteryAct2ShowView__
#define __IF__LotteryAct2ShowView__

#include "CommonInclude.h"
#include "PopupBaseView.h"

class LotteryAct2ShowView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
//, public CCTouchDelegate
{
public:
    static LotteryAct2ShowView* create();
	virtual bool init();
    LotteryAct2ShowView():m_aniIndex(-1)/*, m_itemIndex(0)*/, m_type(0), m_shareInfo(""), m_bFromIOS(false){};
    virtual ~LotteryAct2ShowView(){};
    
    static LotteryAct2ShowView* createWithInfo(string info);
    static LotteryAct2ShowView* createWithInfoFromIOS(string info);
    bool initWithLotteryInfo(string info);
    void initAniNodeForShare(string info);
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onShareBtnClick(Ref* pSender, CCControlEvent event);
    
    void initAniNode();
    int hasTouchNode(cocos2d::CCTouch *pTouch);
    void setDesNode(int index);
    string getIcon(string itemId);
    string getColor(string itemId);
    
private:
    CCSafeObject<CCNode> m_aniListNode;
    CCSafeObject<CCNode> m_midNode;
    CCSafeObject<CCNode> m_desNode;
    CCSafeObject<CCLabelIF> m_desName;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCLabelIF> m_titleTTF;
    CCSafeObject<CCScale9Sprite> m_desBg;
    CCSafeObject<CCScale9Sprite> m_bg;
    
    CCSafeObject<CCControlButton> m_shareBtn;
    
    int m_aniIndex;
//    int m_itemIndex;
    int m_type;
    string m_shareInfo;
public:
    bool m_bFromIOS;
};

class LotteryShareConfirmView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static LotteryShareConfirmView* create();
    bool init();
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onClickAllSharBtn(Ref* pSender, Control::EventType event);
    void onClickKingShareBtn(Ref* pSender, Control::EventType event);
    
    string getLotteryInfo();
protected:
private:
    
    CCSafeObject<Node> m_touchNode;
    CCSafeObject<ControlButton> m_allShareBtn;
    CCSafeObject<ControlButton> m_kingShareBtn;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCLabelIF> m_allLabel;
    CCSafeObject<CCLabelIF> m_kingLabel;
    CCSafeObject<Node> m_allNode;
    CCSafeObject<Node> m_kingNode;
};
#endif /* defined(__IF__LotteryAct2ShowView__) */
