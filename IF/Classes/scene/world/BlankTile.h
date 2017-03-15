//
//  BlankTile.h
//  IF
//
//  Created by 邹 程 on 14-2-26.
//
//

#ifndef __IF__BlankTile__
#define __IF__BlankTile__

#include "NewBaseTileInfo.h"

#define TRANS_KINGDOM_SECOND_CONFIRM "transKingdomSecondConfirm"
#define TRANS_KINGDOM_CONFIRMED "transKingdomConfirmed"

class BlankTile : public NewBaseTileInfo {
public:
    
    BlankTile(WorldCityInfo& info):
    NewBaseTileInfo(info)
    {}
    
    static BlankTile* create(WorldCityInfo& info) {
        auto ret = new BlankTile(info);
        if (ret->init()) {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
        
    bool init();
    virtual void onEnter();
    virtual void onExit();
    void onClickTeleport(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickTransKingdom(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickOccupy(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickInviteTeleport(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickPlace(CCObject * pSender, CCControlEvent pCCControlEvent);
    void setBtnState();
    void secondConfirm(CCObject* obj);
    void confirmed(CCObject* obj);
    void alertShow(CCObject *dict);
    virtual BaseTileInfo* getFavoriteView();
    bool canOccupy();
    int getNewBeginnerMoveItemId();
};

class TransKingdomAlertPopUpView : public PopupBaseView
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
{
public:
    TransKingdomAlertPopUpView(int type) : m_type(type){};
    
    static TransKingdomAlertPopUpView* create(int type) {
        auto ret = new TransKingdomAlertPopUpView(type);
        if (ret->init()) {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
private:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickOKBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickCancelBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onGameTick(float time);
    
    CCSafeObject<CCLabelIF> m_title;
    CCSafeObject<CCLabelIF> m_hintText;
    CCSafeObject<CCLabelIF> m_leftTime;
    CCSafeObject<CCControlButton> m_confirmBtn;
    CCSafeObject<CCControlButton> m_cancelBtn;
    int m_type;
};
#endif /* defined(__IF__BlankTile__) */
