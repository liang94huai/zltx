//
//  BanTimeView.hpp
//  IF
//
//  Created by 三脚猫 on 15/12/17.
//
//

#ifndef BanTimeView_hpp
#define BanTimeView_hpp

#include "CommonInclude.h"
#include "PopupBaseView.h"

class BanTimeView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static BanTimeView* create( int type ,const char *content,const char * buttonName = _lang("confirm").c_str(), int timeJson = 0);
    BanTimeView();
    ~BanTimeView();
    
    void keypressedBtnCancel(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickNoticeBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void keyPressedBtnClose(CCObject * pSender, Control::EventType pCCControlEvent);
    void keypressedBtnOk(CCObject * pSender, Control::EventType pCCControlEvent);
    void showCancelButton();
    void setYesButtonTitle(const char *content);
    void setYesText(const char *content);
    void setNoButtonTitle(const char *content);
    
    CCSafeObject<CCControlButton> m_btnOk;
    CCSafeObject<CCControlButton> m_btnCancel;
    CCSafeObject<CCLabelIF> m_goldNum;
    CCSafeObject<CCLabelIF> m_btnText;
    CCSafeObject<CCSprite> m_goldIcon;
    CCSafeObject<CCNode> m_resNode;
    CCSafeObject<CCLabelIF> m_titleText;
    
    CCSafeObject<CCNode> m_noticeNode;
    CCSafeObject<CCControlButton> m_noticeBtn;
    CCSafeObject<CCSprite> m_noticeSpr;
    CCSafeObject<CCLabelIF> m_noticeLabel;
    CCSafeObject<CCNode> m_subNode;
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCNode> m_sprBG;
    CCSafeObject<CCLabelTTF> m_contentLabel;
    
    CCSafeObject<CCNode> m_muteTimeNode;
    
    CCSafeObject<CCLayer> m_subLayer;
    bool m_isAlliance;
    bool m_closeAll;
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(int type ,const char *content,const char * buttonName = _lang("confirm").c_str(), int timeJson = 0);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    void onUpdateLabel(float dt);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){};
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    //回调
    CC_SYNTHESIZE_SAFEOBJECT(CCCallFunc, m_onYes, YesCallback);
    CC_SYNTHESIZE_SAFEOBJECT(CCCallFunc, m_onNo, NoCallback);
    CC_SYNTHESIZE_SAFEOBJECT(CCCallFunc, m_onClose, CloseCallback);
    CC_SYNTHESIZE_SAFEOBJECT(CCCallFunc, m_onSel, SelCallback);
    
    
    // function callback
    int m_type;//1:loading,2:切换账号
    bool m_isTimeOver;
    std::string mTimeDes;
    std::function<void()> m_function;
    CC_SYNTHESIZE(bool, isCloseOnNoButton, CloseOnNoButton);
    CC_SYNTHESIZE(bool, disableWhenTouched, DisableWhenTouched);
    
    void serverUpdateInfo(const char * content, int timeJson);
    int m_Endtime;
};

#endif /* BanTimeView_hpp */
