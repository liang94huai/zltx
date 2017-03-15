//
//  ScoutInfo.h
//  IF
//
//  Created by 邹 程 on 14-3-5.
//
//

#ifndef __IF__ScoutInfo__
#define __IF__ScoutInfo__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "WorldController.h"
#include "BaseTileInfo.h"

class ScoutInfo : public BaseTileInfo
{
public:
    
    ScoutInfo(WorldCityInfo& info):
    BaseTileInfo(info)
    {}
    
    static ScoutInfo* create(WorldCityInfo& info) {
        auto ret = new ScoutInfo(info);
        if (ret->init()) {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    
    bool init();
    
    void onDetailCallback(CCObject* obj);
        
    void onClickButtonScout(CCObject * pSender, Control::EventType pCCControlEvent);
    void onMialBtnCilck(CCObject * pSender, Control::EventType pCCControlEvent);
private:
    void initMailInfo();
    void onGameTick(float Time);
    void readMailSuccess(CCObject* param);
    int getLastTime();
protected:
    unsigned int m_costFood;
    unsigned int m_costTime;
    
private:
    CCSafeObject<CCLabelIF> m_owner;
    CCSafeObject<CCLabelIF> m_country;
    CCSafeObject<CCLabelIF> m_time;
    CCSafeObject<CCLabelIF> m_tilePoint;
    CCSafeObject<CCLabelIF> m_cost;
    CCSafeObject<CCLabelIF> m_information;
    CCSafeObject<CCLabelIF> m_buttonName;
    CCSafeObject<CCNode> m_hideNode;
    
    CCSafeObject<CCLabelIF> m_scoutTime;
    CCSafeObject<Label> m_name;
    CCSafeObject<CCNode> m_picContainer;
    
    CCSafeObject<CCLabelIF> m_titleNew;
    CCSafeObject<CCControlButton> m_btnScout;
    
    
     
    CCSafeObject<CCNode> m_scoutNode;
    CCSafeObject<CCNode> m_mailNode;
    CCSafeObject<CCLabelIF> m_mailBtnLb;
    CCSafeObject<CCLabelIF> m_lastScoutTimeLb;
    
    string m_mailUid;
};

#endif /* defined(__IF__ScoutInfo__) */
