//
//  DragonBuildingTile.h
//  IF
//
//  Created by ganxiaohua on 15/9/30.
//
//

#ifndef __IF__DragonBuildingTile__
#define __IF__DragonBuildingTile__

#include "NewBaseTileInfo.h"
#include "ThroneTile.h"

class DragonBuildingTile : public ThroneTile {
public:
    
    DragonBuildingTile(WorldCityInfo& info):
    ThroneTile(info)
    {}
    
    static DragonBuildingTile* create(WorldCityInfo& info) {
        auto ret = new DragonBuildingTile(info);
        if (ret->init()) {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    
protected:
    virtual bool init();
    virtual std::string getShowText();
    virtual int getOwnerFlag();
    virtual void setDefenderWarTimeButtons();
    virtual void setAttackerWarTimeButtons();
    virtual void onClickSurpport(CCObject * pSender, Control::EventType pCCControlEvent);
    virtual void onClickOccupy(CCObject * pSender, Control::EventType pCCControlEvent);
    virtual void onClickScout(CCObject * pSender, Control::EventType pCCControlEvent);
    virtual void onClickRally(CCObject * pSender, Control::EventType pCCControlEvent);
    virtual bool isSelfBuild();
};
#endif /* defined(__IF__DragonBuildingTile__) */
