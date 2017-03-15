//
//  ResourceTileInfoPopUpView.h
//  IF
//
//  Created by 李锐奇 on 14-8-21.
//
//

#ifndef __IF__ResourceTileInfoPopUpView__
#define __IF__ResourceTileInfoPopUpView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "WorldController.h"
#include "DefaultTableViewDelegate.h"
#include "CCMultiColTableView.h"
#include "DefaultTableViewDelegate.h"

class ResourceTileInfoPopUpView : public PopupBaseView
//, public CCTouchDelegate
, public CCBMemberVariableAssigner
, public cocos2d::extension::CCMultiColTableViewDataSource
, public cocos2d::extension::CCMultiColTableViewDelegate
{
public:
    static ResourceTileInfoPopUpView *create(WorldCityInfo &info);
     
protected:
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
	virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual ssize_t numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
    
private:
    ResourceTileInfoPopUpView(WorldCityInfo &info) : m_info(info){};
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    void onClickCloseBtn(cocos2d::CCObject * pSender, CCControlEvent pCCControlEvent);
    void generateData();
    void refreshView();
    void troopDataBack(CCObject *obj);
    void getTroopData();
    
    void getCapInfoData();
    void capInfoBack(CCObject *obj);
    
    void initLabInfo();
    
    void onSplitResInfo(const char* strContent, map<int,int> &mapinfo);
    bool isResTpye(int iType, int &iNum);  //此类型资源是否在资源点产出,并返回产量速度

    void updateTime(float dt);

    map<int,int>    m_mapResSpeedInfo;
    map<int,int>    m_mapResMaxInfo;

    WorldCityInfo &m_info;
    double  m_starTime;
    CCSafeObject<CCLabelIF>     m_title;
    CCSafeObject<CCLabelIF>     m_cityNameTxt;
    CCSafeObject<CCLabelIF>     m_cityName;
    CCSafeObject<CCLabelIF>     m_zoneNameTxt;
    CCSafeObject<CCLabelIF>     m_zoneName;
    CCSafeObject<CCLabelIF>     m_stateTxt;
    CCSafeObject<CCLabelIF>     m_state;

    CCSafeObject<CCLabelIF>     m_foodNumTxt;
    CCSafeObject<CCLabelIF>     m_woodNumTxt;
    CCSafeObject<CCLabelIF>     m_stoneNumTxt;
    CCSafeObject<CCLabelIF>     m_ironNumTxt;
    CCSafeObject<CCLabelIF>     m_cityBuffTxt;

    CCSafeObject<CCSprite>      m_foodIcon;
    CCSafeObject<CCSprite>      m_woodIcon;
    CCSafeObject<CCSprite>      m_stoneIcon;
    CCSafeObject<CCSprite>      m_ironIcon;

    CCSafeObject<CCLabelIF>     m__caijiNums[4];
    CCSafeObject<CCLabelIF>     m__caijifoodNum;
    CCSafeObject<CCLabelIF>     m__caijiwoodNum;
    CCSafeObject<CCLabelIF>     m__caijistoneNum;
    CCSafeObject<CCLabelIF>     m__caijiironNum;
    CCSafeObject<CCLabelIF>     m_caijiTxt;

    CCSafeObject<CCSprite>      m__caijifoodIcon;
    CCSafeObject<CCSprite>      m__caijiwoodIcon;
    CCSafeObject<CCSprite>      m__caijistoneIcon;
    CCSafeObject<CCSprite>      m__caijiironIcon;

    CCSafeObject<CCLabelIF>     m_ArmSumTxt;
    CCSafeObject<CCLabelIF>     m_ArmSum;
    CCSafeObject<CCLabelIF>     m_ArmNameTxt;
    CCSafeObject<CCLabelIF>     m_armNum;

    CCSafeObject<CCNode>       m_ArmNoneNode;
    CCSafeObject<CCLabelIF>     m_ArmNoneTxt;

    
    CCSafeObject<CCNode>       m_foodnode;
    CCSafeObject<CCNode>       m_woodnode;
    CCSafeObject<CCNode>       m_stonenode;
    CCSafeObject<CCNode>       m_ironnode;
    
    CCSafeObject<CCNode>       m_foodcaijinode;
    CCSafeObject<CCNode>       m_woodcaijinode;
    CCSafeObject<CCNode>       m_stonecaijinode;
    CCSafeObject<CCNode>       m_ironcaijinode;
    
    CCSafeObject<CCNode>        m_infolist;
    CCSafeObject<CCNode>        m_arminfo;
    CCSafeObject<CCNode>        m_noneArm;
    CCSafeObject<CCNode>        m_armtile;

    

    CCSafeObject<CCNode>       m_touchNode;
 
    CCSafeObject<CCNode>       m_mainNode;
    //行军信息
    CCSafeObject<CCArray> m_data;
    //部队view
    CCSafeObject<CCMultiColTableView> m_tabView;
    CCSafeObject<CCControlButton> m_closeBtn;

    
    std::string marchId;
};

class ResourceTroopInfo : public CCTableViewCell
, public CCBMemberVariableAssigner

{
public:
    static ResourceTroopInfo *create(CCDictionary *dict);
    void setData(CCDictionary *dict);
private:
    virtual bool init(CCDictionary *dict);
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);

    void refreshView();
    CCSafeObject<CCDictionary> m_dict;
    CCSafeObject<CCLabelIF> m_numTxt;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_levelTxt;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCNode> m_levelNode;

};

#endif /* defined(__IF__ResourceTileInfoPopUpView__) */
