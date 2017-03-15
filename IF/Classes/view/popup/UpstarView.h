//
//  UpstarView.h
//  IF
//
//  Created by 李朝辉 on 15-8-25.
//
//

#ifndef IF_UpstarView_h
#define IF_UpstarView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "ArcPopupBaseView.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "FunBuildInfo.h"
#include "cocos2d.h"

class UpstarCell;
class UpstarTitleCell;

class UpstarView: public ArcPopupBaseView
//class UpstarView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner

{
public:
    static UpstarView* create(int buildId, int pos=0);
    UpstarView():m_buildId(0),m_pos(0),_tmpGold(0),_resGold(0),_tmpTime(0),m_openNum(0),m_qid(QID_MAX),m_rentTime(0), m_progresscnt_2(0),m_progresscnt(0),StarRateCur(0){};
    virtual ~UpstarView(){ };//m_itemScrollView->getContainer()->removeAllChildren();
    CCNode *getGuideNode(int itemId);
private:
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchMoved (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual bool init(int buildId, int pos=0);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onCreateOrUpClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onInstantClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void onOkUp();
    void onLastUp();
    void showWarningInstantUp();
    void onOkInstantUp();
    void onGotoSpdView();
    void FameShow();
    void TextReloc();
    void BrownProgressShow(float time = 0);
    void YellowProgressShow(float time = 0);
    void ParticalShow(float time);
    void StopTimeParticle(float time);
    void playParticle();
    void callback();
    void callback2();
    
    void updateInfo(CCObject* obj);
    void updateParasInfo();
    void onTipBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onShowNextUnlockItem();
    string getBarrackDes(int type,int starnum);
    void starUpInfo();
    void onYesRent();
    void helpLoc();
    void showRentQueueDialog(float lastTime);
    bool m_bRent;
    
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCControlButton> m_instantBtn;
    CCSafeObject<CCControlButton> m_tipBtn;
    CCSafeObject<CCControlButton> m_upBtn;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_btnNode;
    CCSafeObject<CCLabelIF> m_demandLabel;

    CCSafeObject<CCNode> m_inBtnNode;
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCLabelIF> m_btnMsgLabel;
    CCSafeObject<CCLabelIF> m_inBtnGoldNum;
    CCSafeObject<CCLabelIF> m_StarNum;
    CCSafeObject<CCLabelIF> m_StarNum1;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    
    CCSafeObject<CCLabelIF> m_lvLabel;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_upBtnMsgLabel;
    CCSafeObject<CCNode> m_arrowNode;
    CCSafeObject<CCSprite> m_rArrow;
    CCSafeObject<CCParticleSystemQuad> PaticleEff1;
    CCSafeObject<CCParticleSystemQuad> PaticleEff2;
    CCSafeObject<CCFiniteTimeAction> sc;
    CCSafeObject<CCFiniteTimeAction> sc2;
    CCSafeObject<CCSprite> m_lArrow;
    CCSafeObject<CCProgressTimer> m_process;
    CCSafeObject<CCNode> m_prograssShowNode;
    CCSafeObject<CCNode> m_seek;
    CCSafeObject<CCNode> m_flagdata;
    CCSafeObject<CCNode> m_condition;
    CCSafeObject<CCPointArray> m_pointArray;
    CCSafeObject<CCLabelIF> m_proText;
    CCSafeObject<CCLabelIF> m_curpro;
    CCSafeObject<CCLabelIF> m_inc1;
    CCSafeObject<CCLabelIF> m_desToStar;
    CCSafeObject<CCLabelIF> m_onetext;
    CCSafeObject<CCLabelIF> m_oneleft;
    CCSafeObject<CCLabelIF> m_oneright;
    CCSafeObject<CCLabelIF> m_twotext;
    CCSafeObject<CCLabelIF> m_twoleft;
    CCSafeObject<CCLabelIF> m_tworight;
    CCSafeObject<CCLabelIF> m_threetext;
    CCSafeObject<CCLabelIF> m_threeleft;
    CCSafeObject<CCLabelIF> m_threeright;
    CCSafeObject<CCLabelIF> m_fourtext;
    CCSafeObject<CCLabelIF> m_fourleft;
    CCSafeObject<CCLabelIF> m_fourright;
    CCSafeObject<CCNode> m_para1Node;
    CCSafeObject<CCNode> m_para2Node;
    CCSafeObject<CCNode> m_para3Node;
    CCSafeObject<CCNode> m_para4Node;
    int m_buildId;
    int m_pos;
    bool m_isReturn;
    int _tmpGold;
    int _resGold;
    int _tmpTime;
    CCSafeObject<CCScrollView> m_scrollView;
//    CCScrollView* m_itemScrollView;
    int m_openNum;
    int m_qid;
    int m_rentTime;
    
    int m_lfood;
    int m_lwood;
    int m_liron;
    int m_lstone;
    
    int m_unlockItemWidth;
    int m_sumItemWidth;
    int m_x;
    int m_progresscnt;
    int m_progresscnt_2;
    float StarRateCur;
    
    vector<string> willUnlockItems;
};

class UpstarCell :
public CCIFTouchNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static UpstarCell* create(int type, int itemId, int value, bool isShow=false);
    UpstarCell():m_isOk(true),m_type(0),m_itemId(0),m_value(0)
    ,m_buildId(0),m_gotoBuildId(0),m_parentType(0),m_createInfo(""),tmpNum(0),tmpTime(0),m_tmpName(""),m_isShow(false){};
    void setTouchNode(CCNode* node, int buildId, int parentType=0);
    void setCreateBuildInfo(int buildId, int pos);
    bool m_isOk;
private:
    bool init(int type, int itemId, int value, bool isShow=false);
    virtual void onEnter();
    virtual void onExit();
    
    void refresh();
    void onEnterFrame(float dt);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void spdCallBack();
    void spdCallBackDragon();
    void rectCallBack(CCObject* obj);
    
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_valueLabel;
    CCSafeObject<CCLabelIF> m_value2Label;
    CCSafeObject<CCSprite> m_yesSprite;
    CCSafeObject<CCSprite> m_noSprite;
    
    CCSafeObject<CCScale9Sprite> m_btn;
    CCSafeObject<CCScale9Sprite> m_rectPic;
    CCSafeObject<CCNode> m_touchBtn;
    CCSafeObject<CCLabelIF> m_btnLabel;
    
    int m_type;
    int m_itemId;
    int m_value;
    int m_buildId;
    int m_gotoBuildId;
    int m_parentType;
    string m_createInfo;
    CCSafeObject<CCNode> m_touchNode;
    int tmpNum;
    int tmpTime;
    string m_tmpName;
    bool m_isShow;
};

class UpstarTitleCell:
public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static UpstarTitleCell* create(string msg);
    UpstarTitleCell(){};
private:
    bool init(string msg);
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCLabelIF> m_titleLabel;
};

#endif
