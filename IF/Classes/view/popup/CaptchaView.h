//
//  CaptchaView.h
//  IF
//
//  Created by liruiqi on 15/9/25.
//
//

#ifndef __IF__CaptchaView__
#define __IF__CaptchaView__

#include "CCCommonUtils.h"
#include "PopupBaseView.h"


#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "WorldController.h"
#include "InputFieldMultiLine.h"

#define MAX_FAIL_RETRY_TIME 3

class CaptchaView : public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCEditBoxDelegate
{
public:
    static CaptchaView* create(CCDictionary *dict);
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    bool init(CCDictionary *dict);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onSendBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onCancelBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onRefreshClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void removeWaitInterface();
    virtual void editBoxReturn(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    
    void sendDataBack(CCObject *obj);
    void getDataBack(CCHttpClient* client, CCHttpResponse* response);
    void doWhenGetDataFailed();
    void checkFail();
    void checkSuccess();
private:
    CCSafeObject<CCNode> m_editBoxContainer;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCControlButton> m_okBtn;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_lineNode;
    CCSafeObject<InputFieldMultiLine> m_captchaEdit;
    CCSafeObject<CCDictionary> m_dict;
    CCSafeObject<CCLabelIF> m_hintText;
    CCSafeObject<CCLabelIF> m_changText;
    CCSafeObject<WaitInterface> m_waitInterface;
    bool isGetPic;
    int m_failRetryTime;
};
#endif /* defined(__IF__CaptchaView__) */
