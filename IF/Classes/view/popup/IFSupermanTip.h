//
//  IFSupermanTip.h
//  IF
//
//  Created by wangdianzhen on 15/9/9.
//
//

#ifndef __IF__IFSupermanTip__
#define __IF__IFSupermanTip__

#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "CCAniNode.h"
#include "PopupBaseView.h"
class IFSupermanTip:public  PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    IFSupermanTip(int type,string infoStr){
        m_type = type;
        m_infoStr = infoStr;
        m_istranslated = false;
        m_callbackStr = "";
    };
    ~IFSupermanTip(){};
    bool initData();
    static IFSupermanTip* create(int type,string infoStr);
private:
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onEditClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onSaveClick(CCObject * pSender, Control::EventType pCCControlEvent);
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
 
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_subNode;
    CCSafeObject<CCLabelIF> m_infoLb;
    CCSafeObject<CCNode> m_btnNode;
    CCSafeObject<CCControlButton> m_editBtn;
    CCSafeObject<CCControlButton> m_saveBtn;
    CCSafeObject<CCSprite> m_translateSp;
    CCSafeObject<CCSprite> m_lineSp;
    CCSafeObject<CCNode> m_editNode;
    CCSafeObject<CCLabelIFTTF> m_editLb;
    CCSafeObject<CCEditBox> m_announceEditBox;
private:
    int m_type;
    string  m_infoStr;
    CC_SYNTHESIZE(bool, m_istranslated, istranslated);
    string m_callbackStr;
private:
    void saveSuccess(CCObject* pObj);
    void translateBack(CCObject* pObj);
};


#endif /* defined(__IF__IFSupermanTip__) */
