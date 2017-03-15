//
//  GoldExchangeView_NEW.hpp
//  IF
//
//  Created by xxrdsg on 15/11/3.
//
//

#ifndef GoldExchangeView_NEW_hpp
#define GoldExchangeView_NEW_hpp

#include "CCCommonUtils.h"
#include "PopupBaseView.h"
#include "CommonInclude.h"

class GoldExchangeView_NEW: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public ScrollViewDelegate
{
public:
    static GoldExchangeView_NEW* create();
    GoldExchangeView_NEW();
    ~GoldExchangeView_NEW();
    static bool sortExchangeItem(GoldExchangeItem *item1, GoldExchangeItem *item2);
    virtual void scrollViewDidScroll(ScrollView* view);
protected:
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onCloseClick(CCObject *pSender, CCControlEvent event);
    void initList();
    
    void refresh(CCObject* obj);
    void getData();
    bool isGetData();
    
    CCSafeObject<CCLabelIF> m_goldText;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCNode> m_BGNode;
    CCSafeObject<CCNode> m_advertisNode;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<Node> m_geNode;
    
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<TableView> m_tableView;
    std::vector<GoldExchangeItem*> mCardList;
};

#endif /* GoldExchangeView_NEW_hpp */
