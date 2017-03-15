//
//  DragonManageView.h
//  IF
//
//  Created by ganxiaohua on 15/8/26.
//
//

#ifndef __IF__DragonManageView__
#define __IF__DragonManageView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCMultiColTableView.h"
#include "CCTableViewTouchIFCell.h"
class DragonManageView : public PopupBaseView
, public CCBSelectorResolver
, public cocos2d::extension::CCMultiColTableViewDataSource
, public CCBMemberVariableAssigner
{
public:
    static DragonManageView *create();
    DragonManageView(){};
    ~DragonManageView(){};
    
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
    virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual ssize_t numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void getData(CCObject *pSender);
    void updateData(CCObject* param);
    void refresh();
    
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCLabelIF> m_txt1;
    CCSafeObject<CCLabelIF> m_txt2;
    CCSafeObject<CCLabelIF> m_txt3;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_powerLabel;
    CCSafeObject<CCLabelIF> m_stateLabel;
    CCSafeObject<CCNode> m_titleNode;
    CCSafeObject<CCSprite> m_dragonBg;
    CCSafeObject<CCMultiColTableView> m_tabView;
    CCSafeObject<WaitInterface> m_waitInterface;
    CCSafeObject<CCArray> m_data;
};

class DragonMemberCell : public CCTableViewTouchIFCell
,public CCBSelectorResolver
,public CCBMemberVariableAssigner

{
public:
    static DragonMemberCell *create(CCDictionary* info);
    void setData(CCDictionary* info);
private:
    DragonMemberCell(CCDictionary* info):m_info(info){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);

    void onClickBatlle(CCObject *pSender, CCControlEvent event);
    void onCancelBatlle(CCObject *pSender, CCControlEvent event);
    void successCallBack(CCObject* param);
    
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_powerTxt;
    CCSafeObject<CCSprite> m_rankSpr;
    CCSafeObject<CCSprite> m_signSpr;
    CCSafeObject<CCLabelIF> m_tipTxt;
    CCSafeObject<CCControlButton> m_cancelBtn;
    CCSafeObject<CCNode> m_icon;
    CCSafeObject<CCControlButton> m_battleBtn;
    CCSafeObject<CCDictionary> m_info;
    string m_uid;
    int isFight;
};
#endif /* defined(__IF__DragonManageView__) */
