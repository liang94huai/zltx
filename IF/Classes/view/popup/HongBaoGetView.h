//
//  HongBaoGetView.h
//  IF
//
//  Created by 张军 on 15/11/19.
//
//

#ifndef HongBaoGetView_h
#define HongBaoGetView_h

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCTableViewTouchIFCell.h"
#include "HFHeadImgNode.h"

class HongBaoInfo: public CCObject
{
public:
    void updateInfo(CCDictionary* dict);
    static HongBaoInfo* create();
    static HongBaoInfo* create(CCDictionary* dict);
    HongBaoInfo(CCDictionary* dict);
    HongBaoInfo(){};
    
    string name;
    int value;
    double time;
    string pic;
    string player;
    int picVer;
};

class HongBaoGetView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
, public CCTableViewDataSource
, public DefaultTableViewDelegate
{
public:
    static HongBaoGetView* create(string redId,string serverId,bool isViewOnly = false);
    HongBaoGetView():m_isViewOnly(false){};
    ~HongBaoGetView(){};
    
protected:
    //    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(string redId,string serverId,bool isViewOnly = false);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onBackBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void onGetRedPacket(CCObject * obj);
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCLabelIF> m_bottomMsg;
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCNode> m_headIconNode;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_goldGetTxt;
    CCSafeObject<CCLabelIF> m_desTxt;
    CCSafeObject<CCLabelIF> m_getInfoTxt;
    CCSafeObject<CCNode> m_goldGetNode;
    CCSafeObject<CCNode> m_mNode;
    
    CCSafeObject<HFHeadImgNode> m_headImgNode;

    CCTableView* m_tabView;
    
    string hongBaoId;
    string m_serverId;
    bool m_isViewOnly;
    vector<HongBaoInfo> m_data;
    int m_maxGetGold;
//    int getDataState;
};

class HongBaoGetViewCell : public CCTableViewTouchIFCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static HongBaoGetViewCell *create(HongBaoInfo& hbInfo);
    void setData(HongBaoInfo& hbInfo);
    void setFire();
private:
    HongBaoGetViewCell(){};
    virtual bool init(HongBaoInfo& hbInfo);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
   
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_goldTxt;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCNode> m_headIconNode;
    CCSafeObject<CCLabelIF> m_luckyTxt;
    
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    
    HongBaoInfo m_Info;
};


#endif /* HongBaoGetView_h */
