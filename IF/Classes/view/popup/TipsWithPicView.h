//
//  TipsWithPicView.h
//  IF
//
//  Created by 〜TIAN~ on 15/4/2.
//
//

#ifndef __IF__TipsWithPicView__
#define __IF__TipsWithPicView__

#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "WorldController.h"

class TipsWithPicView : public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
 
{
public:
    static TipsWithPicView* create(WorldCityInfo& info);
    TipsWithPicView(WorldCityInfo &info) : m_info(info){};
    virtual ~TipsWithPicView(){};
    
private:
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    
    
     CCSafeObject<CCLabelIF> m_title;
     CCSafeObject<CCLabelIF> m_cityNameTxt;
     CCSafeObject<CCLabelIF> m_cityName;
     CCSafeObject<CCLabelIF> m_zoneNameTxt;
     CCSafeObject<CCLabelIF> m_zoneName;
     CCSafeObject<CCLabelIF> m_stateTxt;
     CCSafeObject<CCLabelIF> m_state;
     CCSafeObject<CCLabelIF> m_buffTxt;
    
     CCSafeObject<CCLabelIF>     m_ArmNameTxt;
     CCSafeObject<CCLabelIF>     m_armNum;
     CCSafeObject<CCLabelIF> m_foodNumTxt;
     CCSafeObject<CCLabelIF> m_woodNumTxt;
     CCSafeObject<CCLabelIF> m_stoneNumTxt;
     CCSafeObject<CCLabelIF> m_ironNumTxt;
     CCSafeObject<CCLabelIF> m_cityBuffTxt;
    
     CCSafeObject<CCSprite>     m_foodIcon;
     CCSafeObject<CCSprite>     m_woodIcon;
     CCSafeObject<CCSprite>     m_stoneIcon;
     CCSafeObject<CCSprite>     m_ironIcon;
    
     CCSafeObject<CCNode>       m_foodnode;
     CCSafeObject<CCNode>       m_woodnode;
     CCSafeObject<CCNode>       m_stonenode;
     CCSafeObject<CCNode>       m_ironnode;
    
     CCSafeObject<CCNode>       m_tochNode;
    
    WorldCityInfo &m_info;
    
    map<int,int>    m_mapResHourInfo;
    map<int,int>    m_mapMaxHourInfo;
private :
    void initLabInfo();
    void initLabInfo2();
    void refreshData();
    void onSplitResInfo(const char* strContent, map<int,int> &mapinfo);
    int isResTpye(int iType);  //此类型资源是否在资源点产出,并返回产量速度
};

#endif /* defined(__IF__TipsWithPicView__) */
