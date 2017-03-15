//
//  ServerUpdateView.h
//  IF
//
//  Created by hujiuxing on 15-12-16.
//
//

#ifndef __IF__ServerUpdateView__
#define __IF__ServerUpdateView__

#include "CommonInclude.h"
#include "PopupBaseView.h"

class ServerUpdateView:public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static ServerUpdateView* create(void);
    ServerUpdateView() {}
    virtual ~ServerUpdateView(void) {}
    
    void onEnter();
    void onExit();
    bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void onClickOKBtn(CCObject *pSender, CCControlEvent event);
    void onClickHelpBtn(CCObject *pSender, CCControlEvent event);
    void onLinkClicked(CCObject *ccObj);
protected:
    bool getUrlString(string desStr,string & url,string &left );
private:
    
    CCSafeObject<CCNode>  m_mainNode;
    CCSafeObject<CCNode>  m_touchNode;
    
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIFTTF> m_noteLabel;
    
    CCSafeObject<CCRichLabelTTF> m_noteDRich;
    
    CCSafeObject<CCControlButton> m_helpBtn;
    CCSafeObject<CCControlButton> m_okBtn;
    
    std::string  m_linkURL;
};


#endif /* defined(__IF__ServerUpdateView__) */
