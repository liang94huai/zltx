//
//  SoldierInfoView.h
//  IF
//
//  Created by ganxiaohua on 14-5-29.
//
//

#ifndef __IF__SoldierInfoView__
#define __IF__SoldierInfoView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"
#include "ArcPopupBaseView.h"
#include "CCSliderBar.h"

class SoldierInfoView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCEditBoxDelegate
//,public CCTouchDelegate
{
public:
    static SoldierInfoView* create(ArmyInfo* info,int buildingId);
    SoldierInfoView(ArmyInfo* info,int buildingId):m_info(info),m_buildingId(buildingId),m_hireNum(0),m_editOpen(false){};
    ~SoldierInfoView(){};
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
protected:
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void onOkclick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onAddClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onSubClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onCloseclick(CCObject * pSender, Control::EventType pCCControlEvent);
    void sliderCallBack(CCObject*sender,CCControlEvent even);
    void onInfoDetailClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void editBoxReturn(CCEditBox *editBox);
    void editBoxEditingDidBegin(CCEditBox* editBox) {m_editOpen = true;}
    void refreshNum(CCObject* p = NULL);
    void goToDismissAction();
    CCSafeObject<CCControlButton> m_okBtn;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_attrTxt;
    CCSafeObject<CCLabelIF> m_attackTxt;
    CCSafeObject<CCLabelIF> m_attackValue;
    CCSafeObject<CCLabelIF> m_defTxt;
    CCSafeObject<CCLabelIF> m_defValue;
    CCSafeObject<CCLabelIF> m_lifeTxt;
    CCSafeObject<CCLabelIF> m_lifeValue;
    CCSafeObject<CCLabelIF> m_rangeTxt;
    CCSafeObject<CCLabelIF> m_rangeValue;
    CCSafeObject<CCLabelIF> m_speedTxt;
    CCSafeObject<CCLabelIF> m_speedValue;
    CCSafeObject<CCLabelIF> m_weihtTxt;
    CCSafeObject<CCLabelIF> m_weihtValue;
    CCSafeObject<CCLabelIF> m_numTxt;
    CCSafeObject<CCLabelIF> m_numValue;
    CCSafeObject<CCLabelIF> m_upkeepTxt;
    CCSafeObject<CCLabelIF> m_upkeepValue;
    CCSafeObject<CCLabelIF> m_desc;
    CCSafeObject<CCLabelIF> m_numMaxText;
    CCSafeObject<CCNode> m_icon;
    CCSafeObject<CCScale9Sprite> m_soldierBg;
    CCSafeObject<CCNode> m_starNode;
    CCSafeObject<CCNode> m_upkeepNode;
    CCSafeObject<CCNode> m_defenseNode;
    CCSafeObject<CCNode> m_lifeNode;
    CCSafeObject<CCNode> m_rangeNode;
    CCSafeObject<CCNode> m_speedNode;
    CCSafeObject<CCNode> m_weightNode;
    CCSafeObject<CCSliderBar> m_trainSlider;
    CCSafeObject<CCEditBox> m_editBox;
    CCSafeObject<CCNode> m_sliderContainer;
    CCSafeObject<CCNode> m_editBoxNode;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCControlButton> m_infoBtn0;
    CCSafeObject<CCControlButton> m_infoBtn1;
    CCSafeObject<CCControlButton> m_infoBtn2;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCNode> m_downNode;
    CCSafeObject<CCNode> m_bgNode1;
    CCSafeObject<CCNode> m_bgNode2;
    CCSafeObject<CCNode> m_bgNode3;
    CCSafeObject<CCNode> m_bgNode4;
    
    CCSafeObject<CCNode> m_bgNode5;
    CCSafeObject<CCNode> m_bgNode6;
    CCSafeObject<CCNode> m_bgNode7;
    CCSafeObject<CCNode> m_bgNode8;
    CCSafeObject<CCNode> m_bgNode9;
    CCSafeObject<CCNode> m_bgNode10;
    CCSafeObject<CCNode> m_bgNode11;
    CCSafeObject<CCNode> m_bgNode12;
    CCSafeObject<CCNode> m_bgNode13;
    CCSafeObject<CCNode> m_bgNode14;
    CCSafeObject<CCNode> m_bgNode15;
    CCSafeObject<CCNode> m_jieguNode;
    CCSafeObject<CCLayerColor> m_bgColorLayer;
    CCSafeObject<CCLayerColor> m_bgColorLayerS;
    
    CCSafeObject<CCSprite> m_iconSp;
    CCSafeObject<CCSprite> m_jieguSp;

    
    CCSafeObject<CCNode> m_fightNode;
    CCSafeObject<CCLabelIF> m_fightTxt;
    CCSafeObject<CCLabelIF> m_fightValue;
    
    ArmyInfo* m_info;
    int m_buildingId;
    int m_hireNum;
    bool m_editOpen;
};

class SoldierMoreInfoView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCEditBoxDelegate
//,public CCTouchDelegate
{
public:
    static SoldierMoreInfoView* create(ArmyInfo* info,int buildingId,vector<string> Idvect);
    SoldierMoreInfoView(ArmyInfo* info,int buildingId,vector<string> Idvect):m_info(info),m_buildingId(buildingId),m_hireNum(0),m_editOpen(false),m_Idvect(Idvect){};
    ~SoldierMoreInfoView(){};
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
protected:
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
//    
    void onOkclick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onAddClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onSubClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onToLeft(CCObject * pSender, Control::EventType pCCControlEvent);
    void onToRight(CCObject * pSender, Control::EventType pCCControlEvent);
//    void onCloseclick(CCObject * pSender, Control::EventType pCCControlEvent);
    void sliderCallBack(CCObject*sender,CCControlEvent even);
//    void onInfoDetailClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onCloseClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void editBoxReturn(CCEditBox *editBox);
    void editBoxEditingDidBegin(CCEditBox* editBox) {m_editOpen = true;}
    void skillDatacellSet(void );
    void textAndDatashow();
    void soldierIconShow();
    void arrowState();
    void refreshNum(CCObject* p = NULL);
    void sliderShow();
    void editOper();
    bool bstarLv();
    void goToDismissAction();
    void refreshData(int index);
    CCSafeObject<CCScale9Sprite> m_BG;
    CCSafeObject<CCScale9Sprite> m_jieguBG;
    CCSafeObject<CCControlButton> m_okBtn;
//    CCSafeObject<CCLabelIF> m_nameTxt;
//    CCSafeObject<CCLabelIF> m_attrTxt;
    CCSafeObject<CCLabelIF> m_attackTxt;
    CCSafeObject<CCLabelIF> m_attackValue;
    CCSafeObject<CCLabelIF> m_defTxt;
    CCSafeObject<CCLabelIF> m_defValue;
    CCSafeObject<CCLabelIF> m_lifeTxt;
    CCSafeObject<CCLabelIF> m_lifeValue;
    CCSafeObject<CCLabelIF> m_rangeTxt;
    CCSafeObject<CCLabelIF> m_rangeValue;
    CCSafeObject<CCLabelIF> m_speedTxt;
    CCSafeObject<CCLabelIF> m_speedValue;
    CCSafeObject<CCLabelIF> m_weihtTxt;
    CCSafeObject<CCLabelIF> m_weihtValue;
    CCSafeObject<CCLabelIF> m_upkeepTxt;
    CCSafeObject<CCLabelIF> m_upkeepValue;
    CCSafeObject<CCLabelIF> m_fightTxt;
    CCSafeObject<CCLabelIF> m_fightValue;
    CCSafeObject<CCSprite> m_iconSp;
    CCSafeObject<CCSliderBar> m_trainSlider;
    CCSafeObject<CCNode> m_sliderContainer;
    CCSafeObject<CCNode> m_starlvNode;
    CCSafeObject<CCSprite> m_pic_bg;
//    CCSafeObject<CCLabelIF> m_desc;
//    CCSafeObject<CCLabelIF> m_numMaxText;
//    CCSafeObject<CCNode> m_icon;
//    CCSafeObject<CCScale9Sprite> m_soldierBg;
//    CCSafeObject<CCNode> m_starNode;
//    CCSafeObject<CCNode> m_upkeepNode;
//    CCSafeObject<CCNode> m_defenseNode;
//    CCSafeObject<CCNode> m_lifeNode;
//    CCSafeObject<CCNode> m_rangeNode;
//    CCSafeObject<CCNode> m_speedNode;
//    CCSafeObject<CCNode> m_weightNode;
    CCSafeObject<CCEditBox> m_editBox;
    CCSafeObject<CCNode> m_editBoxNode;
//    CCSafeObject<CCScale9Sprite> m_buildBG;
//    CCSafeObject<CCControlButton> m_infoBtn0;
//    CCSafeObject<CCControlButton> m_infoBtn1;
//    CCSafeObject<CCControlButton> m_infoBtn2;
//    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCControlButton> m_closeBtn;
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCNode> m_topNode;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCSprite> m_bottomSp;
    CCSafeObject<CCScale9Sprite> m_leftbar;
    CCSafeObject<CCScale9Sprite> m_rightbar;
    CCSafeObject<CCScale9Sprite> m_midBG;
    
    
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCLabelIF> m_titlename;
    CCSafeObject<CCLabelIF> m_subtitlename1;
    CCSafeObject<CCLabelIF> m_subtitlename2;
    CCSafeObject<CCLabelIF> m_subtitlename3;
    CCSafeObject<CCLabelIF> m_curnum;
    CCSafeObject<CCNode> m_soldierNode;
    CCSafeObject<CCLabelIF> m_starlvbel;
    
//    CCSafeObject<CCNode> m_downNode;
//    CCSafeObject<CCNode> m_bgNode1;
//    CCSafeObject<CCNode> m_bgNode2;
//    CCSafeObject<CCNode> m_bgNode3;
//    CCSafeObject<CCNode> m_bgNode4;
//    
//    CCSafeObject<CCNode> m_bgNode5;
//    CCSafeObject<CCNode> m_bgNode6;
//    CCSafeObject<CCNode> m_bgNode7;
//    CCSafeObject<CCNode> m_bgNode8;
//    CCSafeObject<CCNode> m_bgNode9;
//    CCSafeObject<CCNode> m_bgNode10;
//    CCSafeObject<CCNode> m_bgNode11;
//    CCSafeObject<CCNode> m_bgNode12;
//    CCSafeObject<CCNode> m_bgNode13;
//    CCSafeObject<CCNode> m_bgNode14;
//    CCSafeObject<CCNode> m_bgNode15;
    CCSafeObject<CCNode> m_jieguNode;
    CCSafeObject<CCLayerColor> m_bgColorLayerS;
//
//    CCSafeObject<CCSprite> m_iconSp;
    CCSafeObject<CCSprite> m_jieguSp;
    CCSafeObject<CCSprite> m_toLeft;
    CCSafeObject<CCSprite> m_toRight;
//
//    CCSafeObject<CCNode> m_fightNode;

//    
    ArmyInfo* m_info;
    int m_buildingId;
    int m_hireNum;
    bool m_editOpen;
    vector<string> m_Idvect;
    int m_index;
    bool m_leftenable;
    bool m_rightenable;
};



class SoldierSkillCell:
public CCIFTouchNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static SoldierSkillCell* create(std::string ind,int buildingId,int index,CCNode* clickArea);
    SoldierSkillCell(string ind,int buildingId,int index,CCNode* clickArea):m_ind(ind),m_buildingId(buildingId),m_index(index),m_clickArea(clickArea){};
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    void onInfoDetailClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void getIconButton();
    void starMode();
    void changeSkin();
    void getSkillDescrip();
    void infoShow();
    
    CCSafeObject<CCLabelIF> m_lvname;
    CCSafeObject<CCLabelIF> m_skilldes;
    CCSafeObject<CCControlButton> m_infoBtn;
    CCSafeObject<CCScale9Sprite> m_up;
    CCSafeObject<CCScale9Sprite> m_mid;
    CCSafeObject<CCSprite> m_Lock;
    CCSafeObject<CCNode> m_clickArea;
    CCSafeObject<CCNode> m_touchArea;
    
    CCPoint pt;
    string m_ind;
    int m_buildingId;
    int m_index;
};
#endif /* defined(__IF__SoldierInfoView__) */
