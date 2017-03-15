//
//  LongJingStoreView.h
//  IF
//
//  Created by zhangjun-zj on 15-8-14.
//
//

#ifndef IF_LongJingStoreView_h
#define IF_LongJingStoreView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "DefaultTableViewDelegate.h"
#include "BackpackView.h"
#include "ToolInfo.h"
#include "StoreMallView.h"
#include "CCTableViewTouchIFCell.h"
class ToolCell;
class ToolRewardCell;

class LongJingCellDelegate
{
public:
    virtual void showTipNode(int itemId,CCTouch *pTouch)=0;
    virtual void hideTipNode()=0;
};

class LongJingStoreView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
,public LongJingCellDelegate
{
public:
    static LongJingStoreView* create(StoreType storeType = STORE_TYPE_STORE,
                             StoreResourceType storeResourceType = STORE_RESOURCE_TYPE_WAR);
    LongJingStoreView(StoreType storeType = STORE_TYPE_STORE,
              StoreResourceType storeResourceType = STORE_RESOURCE_TYPE_WAR):m_type(storeType),m_cityLv(0){};
    virtual ~LongJingStoreView(){};
    void updateInfo();
protected:
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view);
    //LongJingCellDelegate
    virtual void showTipNode(int itemId,CCTouch *pTouch);
    virtual void hideTipNode();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickHelpBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickBagBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickStoreBtn(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    void generateData();
    void generateBagData();
    
    void changeData(int type);
    
    CCSafeObject<CCNode> m_storeSubNode;
    CCSafeObject<CCNode> m_bagNode;
    
    CCSafeObject<CCControlButton> m_storeBtn;
    CCSafeObject<CCControlButton> m_bagBtn;
    
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCNode> m_BGNode;
    
    CCSafeObject<CCNode> m_desNode;
    CCSafeObject<CCScale9Sprite> m_desbg;
    CCSafeObject<CCLabelIF> m_desName;
    CCSafeObject<CCSprite> m_desLine1;
    CCSafeObject<CCNode> m_valueNode;
    CCSafeObject<CCLabelIF> m_desNum1;
    CCSafeObject<CCLabelIF> m_desNum2;
    CCSafeObject<CCLabelIF> m_desNum3;
    CCSafeObject<CCLabelIF> m_desNum4;
    CCSafeObject<CCLabelIF> m_desNum5;
    CCSafeObject<CCLabelIF> m_desPara1;
    CCSafeObject<CCLabelIF> m_desPara2;
    CCSafeObject<CCLabelIF> m_desPara3;
    CCSafeObject<CCLabelIF> m_desPara4;
    CCSafeObject<CCLabelIF> m_desPara5;
    CCSafeObject<CCSprite> m_desLine2;
    CCSafeObject<CCLabelIF> m_desLv;
    CCSafeObject<CCLabelIF> m_msgLabel;
    
    CCSafeObject<CCTableView> m_tabView;
    
    int m_type;
    std::vector<int> m_curList;
    std::vector<int>* m_dataList;
    CCSafeObject<CCArray> m_data;
    int m_cityLv;
    
    int addHeight;
    CCSize listSize;
};

//...cell class
class LongJingCell :
public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewTouchIFCell
{
public:
    static LongJingCell* create(int itemId, int itemId1, CCNode* touchNode);
    LongJingCell();
    void setData(int itemId, int itemId1);
    
    LongJingCellDelegate* delegate;
private:
    bool init(int itemId, int itemId1, CCNode* touchNode);
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void refreashData(CCObject* obj);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickBuyBtn1(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onClickBuyBtn2(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onYes();
    void onCloseConfirm();
    void onBuyTool1(CCObject *ccObj = NULL);
    void onBuyTool2(CCObject *ccObj = NULL);
    void onRetBuyTool();
    void setDesNode(int itemId,int index);
    void setLeft(float dt);
    void setRight(float dt);
    void setLeftHot(bool ishot,int num);
    void setRightHot(bool ishot,int num);
    void setLeftNewHot(float flag);
    void setRightNewHot(float flag);
    
    int getParticalNum(int color);
    
    void addParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    
    CCSafeObject<CCLabelIF> m_nameLabel1;
    CCSafeObject<CCLabelIF> m_nameLabel2;
    CCSafeObject<CCLabelIF> m_priceLabel1;
    CCSafeObject<CCLabelIF> m_priceLabel2;
    CCSafeObject<CCNode> m_picNode1;
    CCSafeObject<CCNode> m_picNode2;
    CCSafeObject<CCNode> m_itemNode1;
    CCSafeObject<CCNode> m_itemNode2;
    CCSafeObject<CCSprite> m_picBG1;
    CCSafeObject<CCSprite> m_picBG2;
    CCSafeObject<CCNode> m_desNode;
    CCSafeObject<CCLabelIF> m_desName;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCScale9Sprite> m_buyBtnSpr1;
    CCSafeObject<CCScale9Sprite> m_buyBtnSpr2;
    CCSafeObject<CCScale9Sprite> m_buyBtngraySpr1;
    CCSafeObject<CCScale9Sprite> m_buyBtngraySpr2;
    CCSafeObject<CCScale9Sprite> m_saleoutSpr1;
    CCSafeObject<CCScale9Sprite> m_saleoutSpr2;
    CCSafeObject<CCSprite> m_goldSpr1;
    CCSafeObject<CCSprite> m_goldSpr2;
    
    CCSafeObject<CCNode> m_itemHotNode1;
    CCSafeObject<CCSprite> m_hotSpr1;
    CCSafeObject<CCSprite> m_hotgraySpr1;
    CCSafeObject<CCSprite> m_hotNumSpr1;
    CCSafeObject<CCSprite> m_hotNumgraySpr1;
    CCSafeObject<CCLabelIF> m_hotdesText1;
    CCSafeObject<CCLabelIF> m_hotNum1Text;
    CCSafeObject<CCScale9Sprite> m_hotLineSpr1;
    CCSafeObject<CCLabelIF> m_hotpriceLabel1;
    
    CCSafeObject<CCNode> m_itemHotNode2;
    CCSafeObject<CCSprite> m_hotSpr2;
    CCSafeObject<CCSprite> m_hotgraySpr2;
    CCSafeObject<CCSprite> m_hotNumSpr2;
    CCSafeObject<CCSprite> m_hotNumgraySpr2;
    CCSafeObject<CCLabelIF> m_hotdesText2;
    CCSafeObject<CCLabelIF> m_hotNum2Text;
    CCSafeObject<CCScale9Sprite> m_hotLineSpr2;
    CCSafeObject<CCLabelIF> m_hotpriceLabel2;
    
    CCSafeObject<CCNode> m_particleL;
    CCSafeObject<CCNode> m_particleR;
    CCSafeObject<CCNode> m_frontLNode;
    CCSafeObject<CCNode> m_frontRNode;
    
    CCSafeObject<CCNode> m_newHotNodeL;
    CCSafeObject<CCSprite> m_newHotSprPL;
    CCSafeObject<CCLabelIF> m_newHotTxtL;
    CCSafeObject<CCSprite> m_newHotSprRL;
    CCSafeObject<CCNode> m_newHotNodeR;
    CCSafeObject<CCSprite> m_newHotSprPR;
    CCSafeObject<CCLabelIF> m_newHotTxtR;
    CCSafeObject<CCSprite> m_newHotSprRR;
    
    CCSafeObject<CCNode> m_touchNode;
    int m_itemId;
    int m_itemId1;
    int m_price;
    int m_boostBid;
    bool m_clickInSide;
    bool m_isClick;
    CCPoint m_startPoint;
    int m_num;
    
    vector<CCParticleBatchNode*> m_parVec;
    CCSafeObject<CCLayer> m_bLayer;
    
};

class LongJingBagView: public CCAniNode
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static LongJingBagView* create(CCSize size);
    LongJingBagView(){};
    virtual ~LongJingBagView(){};
protected:
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(CCSize size);
    
    void generateBagData();
    void refreashData(CCObject* obj);
    
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCTableView> m_tabView;
    
    CCSafeObject<CCArray> m_data;
};

#endif
