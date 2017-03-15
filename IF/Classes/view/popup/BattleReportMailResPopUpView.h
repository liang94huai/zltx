//
//  BattleReportMailResPopUpView.h
//  IF
//
//  Created by 李锐奇 on 14-3-26.
//
//

#ifndef __IF__BattleReportMailResPopUpView__
#define __IF__BattleReportMailResPopUpView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "HFHeadImgNode.h"

#include "MailMonsterCellInfo.h"
class BattleReportMailResPopUpView : public PopupBaseView
, public CCBMemberVariableAssigner
, public CCBSelectorResolver
//, public CCTouchDelegate

{
public:
    static BattleReportMailResPopUpView *create(MailMonsterCellInfo *info);
    
private:
    BattleReportMailResPopUpView(MailMonsterCellInfo *info) : m_info(info){};
    ~BattleReportMailResPopUpView();
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

    
    void onReportClick(CCObject *pSender, CCControlEvent event);
    void onDeleteClick(CCObject *pSender, CCControlEvent event);
    void onBattleDetailClick(CCObject *pSender, CCControlEvent event);
    void onAddSaveClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onReturnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onWriteClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onShareClick(CCObject *pSender, CCControlEvent event);
    void onOkDeleteMail();
    
    void shareSuccess(CCObject* param);
    
    void refreshView(CCObject* p = NULL);
    void getData();
    
    CCSafeObject<MailMonsterCellInfo> m_info;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCControlButton> m_reportBtn;
    CCSafeObject<CCControlButton> m_deleteBtn;
    CCSafeObject<CCLabelIF> m_mailTitle;
    CCSafeObject<CCLabelIF> m_attackText;
    CCSafeObject<CCScale9Sprite> m_hintBG;
    //    CCSafeObject<CCSprite> m_hintLine;
    CCSafeObject<CCLabelIF> m_hintText;
    //    CCSafeObject<CCLabelIF> m_upTitle;
    //    CCSafeObject<CCNode> m_winNode;
    CCSafeObject<CCNode> m_downNode;
    CCSafeObject<CCNode> m_totalNode;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_upNode;
    CCSafeObject<CCNode> m_firstNode;
    CCSafeObject<CCNode> m_failNode;
    
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    //    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCScale9Sprite> m_defBg;
    CCSafeObject<CCScale9Sprite> m_attBg;
    CCSafeObject<CCControlButton> m_detailBtn;
    CCSafeObject<CCControlButton> m_battleBtn;
    //CCSafeObject<CCLabelIF> m_deleteBtnTitle;
    //    CCSafeObject<CCSprite> m_savespr;
    //    CCSafeObject<CCSprite> m_unsavespr;
    CCSafeObject<CCControlButton> m_addSaveBtn;
    CCSafeObject<CCControlButton> m_writeBtn;
    CCSafeObject<CCControlButton> m_returnBtn;
    CCSafeObject<CCSprite> m_returnSpr;
    CCSafeObject<CCSprite> m_picBG;
    
    //    CCSafeObject<CCControlButton> m_unSaveBtn;
    CCSafeObject<CCControlButton> m_shareBtn;
    CCSafeObject<CCNode> m_ListNode;
    CCSafeObject<CCNode> m_battlePicNode;
    CCSafeObject<CCScrollView> m_scrollView;
    int m_totalH;
    int m_Listheight;
    bool isaddHeight;
    std::string attUid;
    std::string defUid;
    bool isatkHelp;
    bool isdefHelp;
    std::string ownerUid;//为了区分是查邮件的人，还是自己就是邮件的拥有者
};

#endif /* defined(__IF__BattleReportMailResPopUpView__) */
