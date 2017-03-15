//
//  FavoriteList.h
//  IF
//
//  Created by 邹 程 on 14-2-26.
//
//

#ifndef __IF__FavoriteList__
#define __IF__FavoriteList__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "WorldController.h"
#include "PopupBaseView.h"
#include "CCTableViewTouchIFCell.h"
#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "DefaultTableViewDelegate.h"
#include "BackpackView.h"
#include "CCStarBar.h"
#include "CCTableViewTouchIFCell.h"
#include "CCTableViewTouchIFCell.h"

class FavoriteBigCell;
class FavoriteCell;

class FavoriteList: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate
{
public:
    static FavoriteList* create();
    FavoriteList(){};
    virtual ~FavoriteList(){};
    
protected:
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void updateInfo(CCObject* obj);
    void updateList(CCObject* obj);
    void closethis(CCObject* obj);
    void updateOpenPanel(CCObject* obj);
    void updateNum();
    void initOpenNum();
    
    void onOkBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onCheckBtnOccor(CCObject * pSender, Control::EventType pCCControlEvent);
    void onCheckOK(CCObject * pSender, Control::EventType pCCControlEvent);
    void onSelectClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onUnSelectClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onDelOpration(CCObject * pSender, Control::EventType pCCControlEvent);
    void onTouchSwallow(CCObject * pSender, Control::EventType pCCControlEvent);
    void delOprate();
    void updateDelNum(cocos2d::CCObject *obj);
    void onDeleteCallback(cocos2d::CCObject *obj);
    void cmdWithData();
    void delCheckDelInfo();
    void addCheckDelInfo();
    
    CCSafeObject<CCScale9Sprite> m_buildBG;
//    CCSafeObject<CCControlButton> m_okBtn;
//    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCNode> m_sprNode;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_topNode;
//    CCSafeObject<CCLabelIF> m_contrNameLabel;
//    CCSafeObject<CCLabelIF> m_contrLabel;
//    
    CCSafeObject<CCTableView> m_tabView;
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeNode<CCNode> m_bottomNode;
    CCSafeNode<CCControlButton> m_checkbtn;
    CCSafeNode<CCControlButton> m_checkokbtn;
    CCSafeNode<CCControlButton> m_rubbish;
    CCSafeObject<CCLabelIF> m_delNum;
    CCSafeObject<CCLabelIF> m_checkalllabel;
    CCSafeNode<CCNode> m_select;
    CCSafeNode<CCNode> m_unselect;
    ////
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCArray> m_flag;
    CCSafeObject<CCArray> m_friend;
    CCSafeObject<CCArray> m_enemy;
    std::vector<pair<double,FavoriteTile>> m_FavoVec;
    void generateData();
    void sortFavoriteList();
    static bool cmp(const std::pair<double,FavoriteTile>& item1, const std::pair<double,FavoriteTile>& item2);

    vector<int> m_curList;
    vector<int> m_styleNum;
    int m_currentTab;
    int m_openItemId;
    int m_flagNum;
    int m_friendNum;
    int m_enemyNum;
    int m_allSelect;
    bool m_checkall;
};

class FavoriteBigCell :
public CCBSelectorResolver

,public CCBMemberVariableAssigner
,public CCTableViewTouchIFCell
{
public:
    static FavoriteBigCell* create(int itemId, int type, int num, CCArray* alldata,bool openchecked,bool checkall);
    FavoriteBigCell():m_itemId(0),m_type(0),m_canTouch(true),m_startY(0),m_openchecked(false),m_checkall(false){};
    void setData(int itemId, int type, int num, CCArray* alldata,bool openchecked,bool checkall);
private:
    bool init(int itemId, int type, int num, CCArray* alldata,bool openchecked,bool checkall);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickOpen(float rat);
    void onResetCanTouch();
    void addAllItemData();
    
//    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_floorLabel;
    CCSafeObject<CCLabelIFTTF> m_titleLabel;
    //    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCNode> m_myTouchNode;
    CCSafeObject<CCSprite> m_jiantouSpr;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCNode> m_mainNode;
    CCSafeNode<CCScale9Sprite> m_left9sprite;
    CCSafeNode<CCScale9Sprite> m_right9sprite;
    
    CCSafeObject<CCArray> m_alldata;
    bool m_openchecked;
    bool m_checkall;
    int m_itemId;
    int m_type;
    int m_num;
    bool m_canTouch;
    int m_startY;
};

class FavoriteCell :
public CCBSelectorResolver
,public CCBMemberVariableAssigner

,public Layer
{
public:
    static FavoriteCell* create(int r_type, double index,bool openchecked,bool checkall);
    FavoriteCell():m_itemId(0),m_type(0),m_startX(0),m_bOpen(false),m_openchecked(0),m_checkall(false),m_index(0){};
private:
    bool init(int r_type, double index,bool openchecked, bool checkall);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    void onEditClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onSelectClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onUnSelectClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onRefreshData(CCObject* obj);
    void onUpdateInfo(bool isUpdate = false);
    int  gettheirDistance(CCPoint);
    void onEnterFrame(float dt);
    void onGotoFavoriteTile();
    void onEditFavorite();
    void GoRight();
    void GoLeft();
    string addDelInfo(FavoriteTile m_tile);
    string delDelInfo(FavoriteTile m_tile);
    int  getPictype(CCPoint pt);
    string getOrignalName(CCPoint pt);
    int  getresourceType(CCPoint pt,int type);
    void addPic(int type);
   
    CCSafeObject<CCLabelIFTTF> m_nameLabel;
//    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCNode> m_touchNode;
//    CCSafeObject<CCNode> m_yesTouchNode;
//    CCSafeObject<CCNode> m_starNode;
//    
//    CCSafeObject<CCNode> m_lvNode;
//    CCSafeObject<CCLabelIF> m_lvLabel;
//    CCSafeObject<CCLabelIF> m_curLvLabel;
//    CCSafeObject<CCLabelIF> m_nextLvLabel;
//    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCLabelIF> m_coorLabel;
    CCSafeNode<CCNode> m_toRight;
    CCSafeNode<CCNode> m_MoveNode;
    CCSafeNode<CCNode> m_select;
    CCSafeNode<CCNode> m_unselect;
    CCSafeObject<CCNode> m_picNode;
//    CCSafeObject<CCLabelIF> m_researshLabel;
//    CCSafeObject<CCNode> m_timeBarNode;
//    CCSafeObject<CCNode> m_timeBarAnimNode;
//    CCSafeObject<CCSprite> m_timeRotationBGSpr1;
//    CCSafeObject<CCSprite> m_timeRotationBGSpr2;
//    CCSafeObject<CCLabelIF> m_nameLabel;
    
    CCSafeObject<CCStarBar> m_starBar;
    CCSafeObject<CCClipNode> m_clipNode;
    
    double  m_index;
    bool m_openchecked;
    bool m_checkall;
    int m_itemId;
    int m_type;
    int m_startX;
    bool m_bOpen;
};


////old
//class FavoriteList;
//
//enum GotoButtonMoveDir
//{
//    GotoButtonMoveNone,
//    GotoButtonMoveLeft,
//    GotoButtonMoveRight
//};
//
//class FavoriteListCell : public CCTableViewCell
//,public CCBMemberVariableAssigner
//,public CCBSelectorResolver
//,public CCTargetedTouchDelegate
//{
//public:
//    
//    FavoriteListCell(const FavoriteTile& tile,FavoriteList* list):
//    m_tile(tile)
//    ,m_index(WorldController::getIndexByPoint(tile.point))
//    ,container(list)
//    ,m_gotoButtonClicked(true)
//    ,m_moveDir(GotoButtonMoveNone)
//    ,m_movedToLeftFlag(false)
//    {}
//    
//    static FavoriteListCell* create(const FavoriteTile& tile,FavoriteList* list) {
//        auto ret = new FavoriteListCell(tile,list);
//        if (ret->init()) {
//            ret->autorelease();
//        } else {
//            CC_SAFE_DELETE(ret);
//        }
//        return ret;
//    }
//    
//    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
//    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
//    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
//    
//    bool init();
//    
//    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
//    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
//    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
//    virtual void onEnter();
//    virtual void onExit();
//    
//    
//private:
//    const FavoriteTile& m_tile;
//    unsigned int m_index;
//    
//    FavoriteList * container;
//    
//    CCSafeObject<CCSprite> m_icon;
//    CCSafeObject<CCLabelIF> m_position;
//    CCSafeObject<CCLabelIF> m_deleteLabel;
//    CCSafeObject<CCLabelIF> m_editLabel;
//    CCSafeObject<CCLabelTTF> m_name;
//    CCSafeObject<CCScale9Sprite> m_editBG;
//    CCSafeObject<CCScale9Sprite> m_gotoTileBG1;
//    //    CCSafeObject<CCScale9Sprite> m_gotoTileBG2;
//    CCSafeObject<CCScale9Sprite> m_deleteBG;
//    CCSafeObject<CCNode> m_gotoButtonNode;
//    
//    bool m_gotoButtonClicked;
//    
//    void onDeleteFavorite();
//    void onEditFavorite();
//    void onGotoFavoriteTile();
//    CCPoint m_touchPoint;
//    GotoButtonMoveDir m_moveDir;
//    bool m_movedToLeftFlag;
//    
//    void updateListCellPosition(CCObject* p);
//    
//};
//
//
//
//class FavoriteList : public PopupBaseView
//,public CCTableViewDataSource
//,public CCBMemberVariableAssigner
//,public CCBSelectorResolver
//
//{
//    
//public:
//    
//    CREATE_FUNC(FavoriteList)
//    
//    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
//    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
//    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
//    
//    
//    virtual CCSize cellSizeForTable(CCTableView *table);
//    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
//    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
//    
//    void onClickButtonTab(CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
//    
//    void sortFavoriteList();
//    static bool cmp(const std::pair<double,FavoriteTile>& item1, const std::pair<double,FavoriteTile>& item2);
//    
//    bool init();
//    
//    virtual void onEnter();
//    virtual void onExit();
//    
//    void updateList(CCObject* obj);
//    
//    void onDeleteCallback(CCObject* obj);
//    
//    void closeThis();
//    
//    CCSize m_nodeSize;
//    CCTableView* m_table;
//    
//private:
//    CCSafeObject<CCNode> m_subNode;
//    CCSafeObject<CCScale9Sprite> m_subNodeBg;
//    CCSafeObject<CCControlButton> m_button[4];
//    CCSafeObject<CCLabelIF> m_noFavoTxt;
//    void generateData();
//    CCSafeObject<CCArray> m_data;
//    FavoriteType m_currentTab;
//    int m_FavoCount;
//    std::vector<pair<double,FavoriteTile>> m_FavoVec;
//};


#endif /* defined(__IF__FavoriteList__) */
