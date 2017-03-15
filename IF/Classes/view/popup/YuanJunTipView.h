//
//  YuanJunTipView.h
//  IF
//
//  Created by ganxiaohua on 14-8-11.
//
//

#ifndef __IF__YuanJunTipView__
#define __IF__YuanJunTipView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "WorldController.h"

class YuanJunTipView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
{
public:
    static YuanJunTipView *create(int index);
    static YuanJunTipView *createByType(int index,WorldCityType cityType,int haveOwner,int maxGarrison=0,int currentGarrison=0);
    YuanJunTipView(int index):m_index(index){};
    ~YuanJunTipView(){};
    static void openYuanYunView(int pointId,WorldCityType cityType,int haveOwner,int maxGarrison=0,int currentGarrison=0);
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(WorldCityType cityType=OriginTile,int haveOwner=0,int maxGarrison=0,int currentGarrison=0);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onCancelClick(CCObject *pSender, CCControlEvent event);
    void onSendClick(CCObject *pSender, CCControlEvent event);
    void getData(CCObject *pSender);
    void setState();
    
    CCSafeObject<CCControlButton> m_cancelBtn;
    CCSafeObject<CCControlButton> m_sendBtn;
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCLabelIF> m_numTxt;
    CCSafeObject<CCLabelIF> m_tipTxt1;
    CCSafeObject<CCLabelIF> m_tipTxt2;
    CCSafeObject<CCNode> m_bg;
    CCSafeObject<CCNode> m_numNode;
    int m_index;
    int m_current;
    int m_maxSoldier;
    WorldCityType m_cityType;
    int m_haveOwner;
    void setTipTxtShow();
    
};
#endif /* defined(__IF__YuanJunTipView__) */
