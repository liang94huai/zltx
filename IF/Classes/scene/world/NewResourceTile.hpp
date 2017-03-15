//
//  NewResourceTile.hpp
//  IF
//
//  Created by wangdianzhen on 15/12/24.
//
//

#ifndef NewResourceTile_hpp
#define NewResourceTile_hpp

#include "BaseTileInfo.h"
#include "NewBaseTileInfo.h"


class NewResourceTile : public NewBaseTileInfo {
public:
    
    NewResourceTile(WorldCityInfo& info):
    NewBaseTileInfo(info)
    {}
    
    static NewResourceTile* create(WorldCityInfo& info) {
        auto ret = new NewResourceTile(info);
        if (ret->init()) {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    void requestDetail();
    void onDetailCallback(CCObject* obj);
    bool init();
    void onEnter();
    void onExit();
    void onClickButton(CCObject * pSender, Control::EventType pCCControlEvent);
    void updateView();
    void update(float sec);
    virtual BaseTileInfo* getFavoriteView();
    
private:
    bool m_isInUpdate;
    bool dataBack;
    void initLayer();
};


#endif /* NewResourceTile_hpp */
