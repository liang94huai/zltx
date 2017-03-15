//
//  MainCityTile.h
//  IF
//
//  Created by 杜 威 on 16-11-18.
//
//

#ifndef __IF__MainCityTile__
#define __IF__MainCityTile__

#include "BaseTileInfo.h"
#include "NewBaseTileInfo.h"
enum MainCityTileState {
    MainCityBlank
    ,MainCitySelf
    ,MainCityAlliance
    ,MainCityOther
};

class MainCityTile : public NewBaseTileInfo {
public:
    
    MainCityTile(WorldCityInfo& info):
    NewBaseTileInfo(info)
    {}
    
    static MainCityTile* create(WorldCityInfo& info) {
        auto ret = new MainCityTile(info);
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
    void onEnter();
    void onExit();
    void onClickButton(CCObject * pSender, Control::EventType pCCControlEvent);
    CCNode *getNodeByType(std::string type);
    void requestDetail();
    void onDetailCallback(CCObject* obj);
    void updateView();
    void update(float sec);
    virtual BaseTileInfo* getFavoriteView();
    MainCityTileState getState();
    string m_playerUid;
    string m_allianceId;
private:
    MainCityTileState m_currentState;
    bool m_isInUpdate;
    bool m_isHaveMacrh;
    bool m_isFull;
};

#endif /* defined(__IF__MainCityTile__) */
