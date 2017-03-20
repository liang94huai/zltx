//
//  NoticeView.h
//  IF
//
//  Created by duwei on 17-3-2.
//
//

#ifndef IF_NoticeView_h
#define IF_NoticeView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "ui/UIWebView.h"

class NoticeView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static NoticeView* create(string notice);
    NoticeView(){};
    virtual ~NoticeView(){};
    void onClose();
    void onPopupReturnClick(CCObject *pSender, CCControlEvent event);
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(string notice);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCLabelIF> m_label;
    CCSafeObject<CCLabelIF> m_label_2;
    
    CCSafeObject<CCNode> m_infoList;
    
    CCSafeObject<cocos2d::experimental::ui::WebView> m_webView;
};

#endif
