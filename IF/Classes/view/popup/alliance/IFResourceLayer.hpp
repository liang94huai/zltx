//
//  IFResourceLayer.hpp
//  IF
//
//  Created by wangdianzhen on 15/12/25.
//
//

#ifndef IFResourceLayer_hpp
#define IFResourceLayer_hpp
#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "AllianceTeamInfo.h"
#include "YuanJunInfo.h"
#include "CCClipNode.h"
#include "WorldController.h"
#include "HFHeadImgNode.h"
#include "CCTableViewTouchIFCell.h"

class IFResourceLayer : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static IFResourceLayer *create(WorldCityInfo& info);
    IFResourceLayer(WorldCityInfo& info);
    ~IFResourceLayer();
    void requestDetail();
    void update(float sec);
protected:
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onDetailCallback(CCObject* obj);
    void addLoadingAni();
    void removeLoadingAni(CCObject* p = NULL);
    void onTipBtnClick(CCObject *pSender, CCControlEvent event);
    void onCellClick(CCObject* data);
    void qianFanCallBack(CCObject *data);
    void refreshView();
    string getTuid();
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCControlButton> m_tipBtn;
    
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCLabelIF> m_statLabel;
    CCSafeObject<CCLabelIF> m_defLabel1;
    CCSafeObject<CCLabelIF> m_defLabel2;
    CCSafeObject<CCLabelIF> m_troopLabel1;
    CCSafeObject<CCLabelIF> m_troopLabel2;
    CCSafeObject<CCLabelIF> m_timeLabel1;
    CCSafeObject<CCLabelIF> m_timeLabel2;
    CCSafeObject<CCLabelIF> m_speedAddText;
    CCSafeObject<CCLabelIF> m_tipsLabel;
    CCSafeObject<CCSprite> m_loadingIcon;
    CCSafeObject<CCTableView> m_tabView;
    bool m_territoryFlag;//true:自己false:敌人
    WorldCityInfo& m_cityInfo;
    bool m_isInUpdate;
    int m_territoryStat;
    int soldierMax;
    int soldierAlready;
    std::string m_marchId;
    double m_digSpeed;
    double m_digStartTime;
    double m_changeCollectSpdTime;
    int m_troopMax;
    int m_digStartNum;
    bool m_ownerFlag;//true:是否为自己的建筑
    
    bool m_isMeCollect;
};

class IFResourceItemCell : public CCTableViewTouchIFCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner

{
public:
    static IFResourceItemCell *create(YuanJunInfo* info,CCNode* clickNode,int targetIndex,bool owner);
    void setData(YuanJunInfo* info);
    CCSafeObject<CCControlButton> m_btnReward;
private:
    IFResourceItemCell(YuanJunInfo* info,CCNode* clickNode,int targetIndex,bool owner):m_info(info),m_clickNode(clickNode),m_targetIndex(targetIndex),m_owner(owner){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    void onQianfanClick(CCObject *pSender, CCControlEvent event);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    void attendRally();
    void updateTime(float _time);
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_armyNum;
    CCSafeObject<CCLabelIF> m_statusTxt;
    CCSafeObject<CCLabelIF> m_woodTxt;
    CCSafeObject<CCLabelIF> m_foodTxt;
    CCSafeObject<CCLabelIF> m_ironTxt;
    CCSafeObject<CCLabelIF> m_stoneTxt;
    CCSafeObject<CCScale9Sprite> m_renderBg;
    CCSafeObject<CCNode> m_icon;

  

    CCSafeObject<CCNode> m_moveNode;
    CCSafeObject<CCSprite> m_arrow;
    CCSafeObject<CCNode> m_clickArea;
    CCSafeObject<CCNode> m_soldierNode;
    CCSafeObject<CCScale9Sprite> m_lineBg;

    CCSafeObject<CCLabelIF> m_scienceTxt;
    CCSafeObject<CCSprite> m_joinNode;
    CCSafeObject<CCNode> m_bgNodee;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    CCNode* m_clickNode;
    CCPoint  m_touchPos;
    YuanJunInfo* m_info;
    bool m_clickFlag;
    int m_targetIndex;
    bool m_owner;
};

#endif /* IFResourceLayer_hpp */
