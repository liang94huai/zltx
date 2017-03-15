//
//  NpcTalkView.h
//  IF
//
//  Created by fubin on 14-11-11.
//
//

#ifndef __IF__NpcTalkView__
#define __IF__NpcTalkView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"

class NpcTalkView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
   
{
public:
    static NpcTalkView* create(vector<string> dialogs);
    void setCallback(CCObject *target,SEL_CallFunc func);
private:
    NpcTalkView(){};
    
    bool init(vector<string> dialogs);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onCloseClick(CCObject *pSender, CCControlEvent event);
    void refreshWord();
    void onClose();
    void wordAnim();
    void animCallBack(float t);
    void onEnter();
    void onExit();
    
    bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    CCSafeObject<CCLabelIF> m_contentText;
    CCSafeObject<CCLabelIF> m_nameText;
    
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCNode> m_txtNode;
    CCSafeObject<CCNode> m_nextNode;
    
    vector<string> m_dialogs;
    int idx;
    CCObject *m_callbackTarget;
    SEL_CallFunc m_callbackFunc;
    bool animFinish;
};

#endif /* defined(__IF__NpcTalkView__) */
