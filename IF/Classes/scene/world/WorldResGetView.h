//
//  WorldResGetView.h
//  IF
//
//  Created by 刘田厚 on 16-11-30.
//
//

#ifndef __IF__WorldResGetView__
#define __IF__WorldResGetView__

#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "WorldController.h"

class WorldResGetView : public CCLayer
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
 
{
public:
    static WorldResGetView* create(WorldCityInfo& info);
    WorldResGetView(WorldCityInfo &info) : m_info(info),m_iState(0){};
    virtual ~WorldResGetView(){};
    // devil 资源城收集资源动画
    void onFlyOutPut(int cityIndex, int output, int forceResType);
    
private:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    
    
protected:
    

    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);

   
    
    
    void onGetBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent);

    void refreshData();
    void getTimeInfo();
    void initLabel();
    
    void changeState(int type); //0, 倒计时,1 可以领取状态 并停止计时器
    
    void updateTime(float dt);
    void stopBtnAni();
    void plyBtnAni();
    
    void GetResDataBack(CCObject *obj);
    void GetInfoDataBack(CCObject *obj);
public:
    void closeSelf();    
    
private:
    
    WorldCityInfo &m_info;
    double  m_starTime;
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCNode> m_timeNode;
    CCSafeObject<CCNode> m_getNode; 
    CCSafeObject<CCControlButton> m_getBtn;
    
    CCSafeObject<CCScale9Sprite> m_progress;


    CCSafeObject<CCLabelIF> m_timeLabel ;
    CCSafeObject<CCLabelIF> m_flagLab ;

    int m_iState;   //当前的状态
    
};

 
    
    

#endif /* defined(__IF__WorldResGetView__) */
