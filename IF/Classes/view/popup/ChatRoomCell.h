//
//  ChatRoomCell.h
//  IF
//
//
//

#ifndef IF_ChatRoomCell_h
#define IF_ChatRoomCell_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "../../Ext/CCDevice.h"
#include "HFHeadImgNode.h"
#include "MailDialogInfo.h"
#include "CCTableViewTouchIFCell.h"

class ChatRoomCell :
public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCTableViewTouchIFCell
{
public:
    static ChatRoomCell* create(int idx,MailDialogInfo* dInfo,CCNode* touchNode);
    ChatRoomCell():m_idx(0),mDialogInfo(NULL),m_uid(""){};
    ~ChatRoomCell(){};
    void setData(int idx, MailDialogInfo* dInfo,CCNode* touchNode);
private:
    virtual void onExit();
    virtual void onEnter();
    
    bool init(int idx,MailDialogInfo* dInfo, CCNode* touchNode);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void setTranslation(CCObject* msg);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTranslateClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    bool checkSameLangBySystem(string oLang);
    bool showCustomPic(int senderPicVer);
    
    CCSafeObject<CCNode> m_userNode;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCLabelIFTTF> m_nameLabel;
    CCSafeObject<CCNode> m_msgNode;
    CCSafeObject<CCLabelIFTTF> m_msgLabel;
    CCSafeObject<CCLabelIFTTF> m_timeLabel;
    CCSafeObject<CCNode> m_timeNode;
    CCSafeObject<CCNode> m_sysMsgNode;
    CCSafeObject<CCLabelIFTTF> m_sysMsgLabel;
    CCSafeObject<CCNode> m_touchNode1;
    CCSafeObject<CCNode> m_touchNode2;
//    CCSafeObject<CCNode> m_touchNode3;
    CCSafeObject<CCSprite> m_left;
    CCSafeObject<CCSprite> m_right;
    CCSafeObject<CCScale9Sprite> m_chatBG;
    CCSafeObject<CCScale9Sprite> m_sysBG;
    CCSafeObject<CCLabelIFBMFont> m_vipLabel;
//    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_translateNode;
    CCSafeObject<CCLabelIF> m_translateLabel;
    CCSafeObject<CCControlButton> m_translateBtn;
    CCSafeObject<CCNode> m_langNode;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    
    int m_idx;
    string m_uid;
    CCSize m_bgSize;
    int m_timeNodeY;
    CCPoint m_nameLabelPt;
    CCPoint m_vipLabelPt;
    CCPoint m_translatePt;
    int m_touchNode3Y;
    int m_msgLabelY;
    CCSize m_msgDimensions;
    CCPoint m_touchPoint;
	bool m_isOriginal;
	bool m_isCheckingInAllance;
	int m_prevPopupCount;
    MailDialogInfo* mDialogInfo;
    CCSize getTextDimension(const char *text);
};

#endif
