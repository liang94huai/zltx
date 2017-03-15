//
//  ScienceView.h
//  IF
//
//  Created by 邹 程 on 13-11-1.
//
//

#ifndef __IF__ScienceView__
#define __IF__ScienceView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "ScienceInfo.h"
#include "CCClipNode.h"
#include "CCGraySprite.h"
#include "BuildUpgradeView.h"
#include "CCAniNode.h"

class SciencePopupView;
class ScienceCell;

class ScienceView :
public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
 
{
public:
    static ScienceView* create(int buildId, int scienceType);
    ScienceView():m_buildId(0),m_scienceType(0),m_openNum(0),m_reLoc(0){};
    virtual ~ScienceView();
    void updateInfo(CCObject* obj);
    void UpdateCellState(CCObject* obj);
    CCNode *getGuideNode(int sid);
    int m_scienceType;
    void setAttentionTo(int sid);
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(int buildId, int scienceType);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual void doWhenClose();
    void newAddLine(vector<int>* idList, int maxHeight);
    
    void decorateScienceViewByType(int type);
    void reInitView();
    void addBGPic();
    
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    
    CCSafeObject<CCScrollView> m_scrollView;
    CCSpriteBatchNode* m_commonBatch;
    CCSpriteBatchNode* m_iconBatch;
    
    map<int, ScienceCell*> m_scienceCells;
    CCSpriteBatchNode* m_itemBatch;
    CCSpriteBatchNode* m_lineBatch;
    CCSafeObject<CCAniNode> m_aniNode;
    CCSafeObject<CCNode> m_bgContainer;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;
    CCSafeObject<CCNode> m_effectNode;
    
    int m_buildId;
    int m_openNum;
    int m_reLoc;
};



class ScienceCell :
public CCAniNode
 
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static ScienceCell* create(int scienceId, CCSpriteBatchNode* batch, CCSpriteBatchNode* iconBatch, int posx, int posy);
    ScienceCell():m_scienceId(0),m_startY(0),m_posx(0),m_posy(0),m_batch(NULL),m_isOpen(false),m_isInit(false),m_time(0),m_type(0),m_open(0),is_playing(false){};
    virtual ~ScienceCell();
    void setTouchNode(CCNode* node);
    int m_scienceId;
    bool m_isOpen;
    void setAttention();
private:
    bool init(int scienceId, CCSpriteBatchNode* batch, CCSpriteBatchNode* iconBatch, int posx, int posy);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void refreash(CCObject* obj);
    void onEnterFrame(float dt);
    void updateTime(float dt);
    void animationFunc();
    bool iscanupfun();
    
    CCSafeObject<CCProgressTimer> m_proTimer;

    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_lvLabel;
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCSprite> m_lockSpr;
    CCSafeObject<CCSprite> m_newIcon;
    CCSafeObject<CCSprite> m_timershow;
    CCSafeObject<CCLabelIF> m_timecntslabel;
    CCSafeObject<CCSprite> m_num_di;
    CCSafeObject<CCSprite> m_bg;
    CCSafeObject<CCSprite> m_labdi;
    
//    CCSafeObject<CCScale9Sprite> m_lvBG;
    CCSafeObject<CCNode> m_particleNode;
     CCSafeObject<CCNode> m_completePar;
    
    CCSafeObject<CCNode> m_ccbNode;
    CCSafeObject<CCNode> m_touchNode;
    float m_startY;
    

    CCSafeObject<CCSpriteBatchNode> m_batch;
    CCSafeObject<CCSpriteBatchNode> m_iconBatch;
    
    bool m_isInit;
    int m_time;
    int m_type;
    int m_open;
    bool is_playing;
    
    int m_buildLv;
    
    vector<string> resItems;
    vector<string> buildItems;
public:
    int m_posx;
    int m_posy;
};




class SciencePopupView :
public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
 
{
public:
    static SciencePopupView* create(int scienceId);
    SciencePopupView():m_scienceId(0),m_isUping(false),_tmpGold(0),_resGold(0),_lastTime(-1),_sumTime(0),_isOpen(true){};
    virtual ~SciencePopupView(){};
    void updateInfo(CCObject* obj);
    CCNode *getGuideNode(int sid);
private:
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual bool init(int scienceId);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickCloseBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onInstantClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onResearchBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onYesCancel();
    
    void onUpdateRes(int type);
    void onEnterFrame(float dt);
    void closeView(CCObject* obj);
    void onChangeBG(int type);
    void lastResearch();
    
    void onOKCostCD();
    void onOKInstantResearch();
    
    CCSafeObject<CCControlButton> m_closeBtn;
    CCSafeObject<CCControlButton> m_instantBtn;
    CCSafeObject<CCControlButton> m_upBtn;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCScale9Sprite> m_bar;
    CCSafeObject<CCScale9Sprite> m_timeBar;
    
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCLabelIF> m_barLabel;
    CCSafeObject<CCLabelIF> m_curLabel;
    CCSafeObject<CCLabelIF> m_nextLabel;
    CCSafeObject<CCLabelIF> m_curNumLabel;
    CCSafeObject<CCLabelIF> m_nextNumLabel;
    CCSafeObject<CCSprite> m_lockSpr;
    
    CCSafeObject<CCLabelIF> m_btnMsgLabel;
    CCSafeObject<CCLabelIF> m_inBtnGoldNum;
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCLabelIF> m_mateneeded;
    CCSafeObject<CCScale9Sprite> m_littleshadow;
    CCSafeObject<CCNode> m_infoNode;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_timeBarNode;
    
    CCSafeObject<CCLabelIF> m_maxMsgLabel;
    CCSafeObject<CCScale9Sprite> m_maxBg;
     
    CCSafeObject<CCNode> m_bigBG;
    CCSafeObject<CCNode> m_smallBG;
    CCSafeObject<CCScale9Sprite> m_listBG;
    CCSafeObject<CCNode> m_btnNode;
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCNode> m_curNode;
    CCSafeObject<CCNode> m_nextNode;
    
    CCSafeObject<CCNode> m_bigNode;
    CCSafeObject<CCNode> m_smallNode;
    
    CCSafeObject<CCScrollView> m_scrollView;
    
    int m_scienceId;
    int m_isUping;
    int _tmpGold;
    int _resGold;
    int m_lfood;
    int m_lwood;
    int m_liron;
    int m_lstone;
    int _lastTime;
    int _sumTime;
    bool _isOpen;
};


#endif /* defined(__IF__ScienceView__) */
