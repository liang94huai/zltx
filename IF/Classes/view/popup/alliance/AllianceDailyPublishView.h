//
//  AllianceDailyPublishView.h
//  IF
//
//  Created by 张军 on 15/10/20.
//
//

#ifndef AllianceDailyPublishView_h
#define AllianceDailyPublishView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
//#include "VipUtil.h"
#include "DefaultTableViewDelegate.h"
#include "CCTableViewTouchIFCell.h"
#include "AllianceDailyQuestInfo.h"
#include "MerchantView.h"
#include "HFHeadImgNode.h"

#define cellMidNodeCount 6

class AllianceDailyPublishCell;
class MineInfo;

class AllianceDailyPublishView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
, public CCTableViewDataSource
, public DefaultTableViewDelegate
{
public:
    static AllianceDailyPublishView* create();
    AllianceDailyPublishView(){};
    ~AllianceDailyPublishView(){};
    
    void noticeRemoveInterFace(CCObject* p);
    void updateInfo(CCObject* p);
    void updateSendInfo(CCObject* p);
    void refreshTableViewData(CCObject* p);
    void getfreeQueueInfo(CCNode *p);
    void setViewByType(int viewType);
    void CellFlyAniCall(CCObject* p);
    void updateTimesTip(CCObject* p);
    void deleteInfoInSendMap(string uuid);
    void FlyToMine(int x,int y);
    void paticleStopcall();
    void prograssAnicall();
    void changeTaskNum();
    void setData();
//    void orderDataMap(AllianceDailyQuestInfo &info,int orderType);
//    void addDataByMap();
    void updateTime(float t);
    void progressAni(float t);
    void updateMineQueueinfo(); //更新队列信息
    void updateRefreshTime();   //更新时间
    double getfreequeueTotleTime();
    double getfreequeueFinishTime();
    void  getFreeQueueTime();
    void goBack();
protected:
//    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
//    virtual void scrollViewDidScroll(CCScrollView* view);
    
//    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onTabBtn1Click(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onTabBtn2Click(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onTabBtn3Click(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onDownBtn1Click(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onDownBtn2Click(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onDownBtn3Click(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onAddBtn1Click(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onAddBtn2Click(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onNoCellBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onTipBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void refreshByGold();
    void showInterFaceCover(int type);
    
    CCSafeObject<CCParticleSystemQuad> PaticleEff1;
    CCSafeObject<CCParticleSystemQuad> PaticleEff2;
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCScale9Sprite> m_viewBg;
    CCSafeObject<CCNode> m_infoList_3;
    CCSafeObject<CCTableView> m_tabView_3;
    CCSafeObject<CCNode> m_infoList_2;
    CCSafeObject<CCTableView> m_tabView_2;
    CCSafeObject<CCNode> m_infoList_1;
    CCSafeObject<CCTableView> m_tabView_1;
    
    CCSafeObject<CCNode> m_upNode;
    CCSafeObject<CCControlButton> m_tabBtn1;
    CCSafeObject<CCControlButton> m_tabBtn2;
    CCSafeObject<CCControlButton> m_tabBtn3;
    
    CCSafeObject<CCNode> m_queueNode;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCControlButton> m_downBtn1;
    CCSafeObject<CCNode> m_priceNode;
    CCSafeObject<CCLabelIF> m_priceLabel;
    CCSafeObject<CCLabelIF> m_inBtnLabel;
    CCSafeObject<CCControlButton> m_downBtn2;
    CCSafeObject<CCControlButton> m_downBtn3;
    CCSafeObject<CCNode> m_countNode1;
    CCSafeObject<CCControlButton> m_addBtn1;
    CCSafeObject<CCLabelIF> m_addTxt1;
    CCSafeObject<CCNode> m_countNode2;
    CCSafeObject<CCControlButton> m_addBtn2;
    CCSafeObject<CCLabelIF> m_addTxt2;
    
    CCSafeObject<CCLabelIF> m_noCellText;
    CCSafeObject<CCLabelIF> m_tasknum;
    CCSafeObject<CCLabelIF> m_freshtimelabel;
    CCSafeObject<CCControlButton> m_noCellBtn;
    CCSafeObject<CCControlButton> m_tipBtn;
    CCSafeObject<CCNode> m_infogreen;
    CCSafeObject<CCNode> m_totalNode;
    CCSafeObject<CCNode> m_subBg1;
    CCSafeObject<CCNode> m_subBg2;
    CCSafeObject<CCNode> m_subBg3;
    CCSafeObject<CCLabelIF> m_subBgTxt1;
    CCSafeObject<CCLabelIF> m_subBgTxt2;
    CCSafeObject<CCLabelIF> m_subBgTxt3;
    CCSafeObject<CCScale9Sprite> m_progressSpr;
    CCSafeObject<CCLabelIF> m_progressTxt;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCNode> m_particleNode;
    CCSafeObject<CCNode> m_particleNode1;
    CCSafeObject<CCParticleSystemQuad> m_particleEff;
    CCSafeObject<CCNode> m_partiEffNode;
    
    WaitInterface* m_waitInterface;
    AllianceDailyQuestInfo saveQuestInfo;
    vector<string> m_data_2;
    vector<string> m_data_1;
    vector<string> m_data_3;
    int m_viewType;
    int m_subType;
    float infoListH;
    bool b_cmdcontral;
    bool cellPalyAni;
    bool b_freeQueue;
    string m_itemUuid;
    double freequeueTime;
    string m_flyuuid;
    int m_mytasknum;
    bool b_fly;
//    Mineinfo *m_mineinfo;
    
//    vector<AllianceDailyPublishCell*> m_cellMap;
    map<int, AllianceDailyPublishCell> m_cellMap;
    
};

class Mineinfo{
    bool m_freeFlag;
};

//...cell class
class AllianceDailyPublishCell :public CCTableViewTouchIFCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
//public CCAniNode
//,public CCTableViewCell
{
public:
    static AllianceDailyPublishCell* create(string itemId, int subType, int viewType, int aniIdx);
    AllianceDailyPublishCell(){};
    void setData(string itemId, int openState, int viewType, int aniIdx);
    void showData();
    void showAnibyColor(string color);
    
    string m_itemUuid;
    int m_subType;
    int m_viewType;
    AllianceDailyQuestInfo saveQuestInfo;
    string cellType;
    string cellState;
    int playAni;
    bool b_publish;
    
private:
    bool init(string itemId, int subType, int viewType, int aniIdx);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void updateTime(float t);
    void freshData(CCObject* obj);
    void cellShapeShift(CCObject* obj);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void SpeedUpByGold();
    void playAnimation(int time);
    void showMsgs();
    void callback();//隐藏品质边
    void flycallback();//飞入动画

    
    
    void updateThisCellWithAni(CCObject* obj);
    void startSendQuestFun();
    string getIconPic(string str);
    
    void onSpeedBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onRewardBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onSendBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onStartBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onHelpBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    CCSafeObject<CCControlButton> m_speedBtn;
    CCSafeObject<CCControlButton> m_rewardBtn;
    CCSafeObject<CCControlButton> m_sendBtn;
    CCSafeObject<CCControlButton> m_helpBtn;
    CCSafeObject<CCNode> m_superNode;
    CCSafeObject<CCNode> m_queueNode;
    CCSafeObject<CCNode> m_handsNode;
    CCSafeObject<CCControlButton> m_startBtn;
    CCSafeObject<CCLabelIF> m_sendStateTxt;
    CCSafeObject<CCLabelIF> m_lblSuper;;
    CCSafeObject<CCNode> m_shiningNode;
    CCSafeObject<CCParticleSystemQuad> m_particle;
    
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCNode> m_vanishTimeNode;
    CCSafeObject<CCLabelIF> m_msgLabel;
    CCSafeObject<CCLabelIFTTF> m_helplabel;
    CCSafeObject<CCLabelIFTTF> m_playerName;
    CCSafeObject<CCLabelIF> m_playerfinish;
    CCSafeObject<CCLabelIF> m_btnLabel;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCNode> m_iconBgNode;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCNode> m_priceNode;
    CCSafeObject<CCLabelIF> m_buyBtn;
    CCSafeObject<CCLabelIF> m_priceBtn;
    CCSafeObject<CCNode> m_msgNode;
    CCSafeObject<CCNode> m_timeBar;
    CCSafeObject<CCScale9Sprite> m_progressSpr;
    CCSafeObject<CCLabelIF> m_progressTxt;
    CCSafeObject<CCLabelIFTTF> m_progressTxt2;
    CCSafeObject<CCLabelIF> m_progressTxt3;
    
    CCSafeObject<CCNode> m_hideMsgN1;
    CCSafeObject<CCLabelIF> m_N1TitleTxt;
    CCSafeObject<CCNode> m_n1n[6];
    CCSafeObject<CCSprite> m_n1Spr[6];
    CCSafeObject<CCLabelIF> m_n1Txt[6];
    CCSafeObject<CCNode> m_hideMsgN2;
    CCSafeObject<CCLabelIF> m_N2TitleTxt;
    CCSafeObject<CCNode> m_n2n[2];
    CCSafeObject<CCSprite> m_n2Spr[2];
    CCSafeObject<CCLabelIF> m_n2Txt[2];
    CCSafeObject<CCSprite> m_timego;
    
    CCSafeObject<CCNode> m_helpNode;
    CCSafeObject<CCNode> m_helpDesNode;
    CCSafeObject<CCLabelIFTTF> m_helpDesTxt;
    
    CCSafeObject<CCNode> m_itemDesNode;
    CCSafeObject<CCSprite> m_arrowSprDes;
    CCSafeObject<CCLabelIF> m_itemNameTxt;
    CCSafeObject<CCLabelIF> m_itemDesTxt;
    //加载用户头像
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    string exRwdId[2];
    string baseRwdId[6];
    string baseRwdIdother[6];
    
};


#endif /* AllianceDailyPublishView_h */
