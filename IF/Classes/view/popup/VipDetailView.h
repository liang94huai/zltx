//
//  VipDetailView.h
//  IF
//
//  Created by chenliang on 14-3-18.
//
//

#ifndef __IF__VipDetailView__
#define __IF__VipDetailView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCClipNode.h"

#define VIP_REFRESH "vip.refresh"

class VipDetailView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
  
//, public CCTableViewDataSource
{
public:
    static VipDetailView* create(int type = 0);
    
    VipDetailView(int type);
    
    static const float CELL_MARGIN;
    static const int MAX_CELL_NEEDED;
    void refresh();
    
protected:
//    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
//    virtual CCSize cellSizeForTable(CCTableView *table);
//    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
//    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void onShowVIPBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onShowSVIPBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void onAddPointBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onActivityPointBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onTipBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onTimer(float dt);
    void PlayComeInFromRightAnim();
    void PlayComeInFromLeftAnim();
    void ComeInAnimFinish();
    void PlayComeOutFromRightAnim();
    void PlayComeOutFromLeftAnim();
    void ComeOutAnimFinish();
    void refreshPageFlag();
    void PlayNodeExchangeAnim(int type);//node移动动画
    string getTimeStr(int secs);
    void setBtnState();
    void setVipProgress(CCObject* obj);
    void onAddVipAnim();
    void setGuideAnim();
    
    CCSafeObject<CCScrollView> m_scroll;
    
    CCSafeObject<CCControlButton> m_tipBtn;
    CCSafeObject<CCLabelIFBMFont> m_vipLvText;
    CCSafeObject<CCLabelIF> m_vipLeftTimeText;
    CCSafeObject<CCNode> m_vipAnimNode;
    CCSafeObject<Node> m_activeGuideNode;
    CCSafeObject<CCNode> m_picHeadNode;
    CCSafeObject<CCScale9Sprite> m_listBG;
    CCSafeObject<CCSprite> m_pageFlag;
    
    CCSafeObject<CCLabelIFBMFont> m_currentVIPText;
    CCSafeObject<CCLabelIFBMFont> m_NextVIPText;
    CCSafeObject<CCNode> m_downNode;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCNode> m_progressBarNode;
    CCSafeObject<CCLabelIF> m_loginDayText;
    CCSafeObject<CCLabelIFBMFont> m_lblVIP[4];
    CCSafeObject<CCLabelIF> m_vipNextPointNumText;
    CCSafeObject<CCLabelIF> m_vipLastPointNumText;
    CCSafeObject<CCLabelIF> m_vipCurrentPointNumText;
    CCSafeObject<CCLabelIF> m_desText;
    CCClipNode* m_nodeprogressBar;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCSprite> m_sprVip;
    CCSafeObject<CCSprite> m_sprActive[2];
    CCSafeObject<CCNode> m_barTouchNode;
    CCSafeObject<CCScale9Sprite> m_sprBar[4];
    //addNode
    CCSafeObject<CCControlButton> addPointBtn;
    CCSafeObject<CCLabelIF> addPointBtnTxt;
    CCSafeObject<CCNode> m_activeNode;
    CCSafeObject<CCLabelIF> m_activeTxt;
    CCSafeObject<CCSprite> m_activeSpr;
    
    CCSafeObject<CCControlButton> m_showVIPBtn;
    CCSafeObject<CCControlButton> m_showSVIPBtn;
    //SVIP
    CCSafeObject<CCSprite> m_sprLastPoint;
    CCSafeObject<CCNode> m_downNodeSVIP;
    CCSafeObject<CCNode> m_listContainerSVIP;
    CCSafeObject<CCSprite> m_pageFlagSVIP;
    CCSafeObject<CCNode> m_listNodeSVIP;
    CCSafeObject<CCScrollView> m_scrollSVIP;
    CCSafeObject<CCNode> m_BgNodeSVIP;
    CCSafeObject<CCSprite> m_svipUpTextSpr;
    CCSafeObject<CCScale9Sprite> m_headBgVip;
    CCSafeObject<CCSprite> m_headBgSvip;
    CCSafeObject<CCNode> m_vipNodeLeft;
    CCSafeObject<CCNode> m_svipNodeLeft;
    CCSafeObject<CCNode> m_vipNodeRight;
    CCSafeObject<CCNode> m_svipNodeRight;
    CCSafeObject<CCScale9Sprite> m_svipBuildBG;
    
    CCSize listContentSize;
//    CCPoint m_minPoint;
//    CCPoint m_maxPoint;
    
//    CCTableView* m_tableView;
    CCPoint m_touchPoint;
    CCPoint m_ListPoint;
    bool m_ismoveX;
    bool m_ismoving;
    CCSafeObject<CCArray> m_data;
//    CCDictionary* m_vipGroup;
    int m_vipLevel;
    
//    int m_intialIndex;
    int m_movedh;
    int m_movedhSVIP;
    int m_vipLevelSVIP;
    int m_SVIPShowLevel;
    
    int m_touchState; //0-null,1-active vip, 2 - buy point
    
    //CCArray m_freeVipPointsCells;
    //CCArray m_usedVipPointsCells;
    int m_type;////默认为0如果为1则是通过出征上限警告面板进入2为点击免费建造时间的提示进入
    bool m_firstIn;
    
    int m_vipType;//1-vip,2-svip
    void setVipType(int v_type);
    void setVipType1();
    void setVipType2();
    
    void setSvipView();
    
    int vip10Left;
    int vip10Right;
    int curPoint;
};

#endif /* defined(__IF__VipDetailView__) */
