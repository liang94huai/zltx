//
//  ThreeDTouchController.h
//  IF
//
//  Created by liuzengyou on 14-1-21.
//
//

#ifndef __IF__ThreeDTouchController__
#define __IF__ThreeDTouchController__

#include "CommonInclude.h"

class WorldCityInfo;

class ThreeDTouchController : public CCObject {

public:
    static ThreeDTouchController* getInstance();
    void purge();
    
    void openPlayerInfo(WorldCityInfo& cityInfo);
    void sendGetUserInfoCommand(const std::string& player_uid);
    
private:
    
    void onDetailCallback(CCObject* obj);
    void onGetPlayerInfoCallback(CCObject* obj);
};

#endif /* defined(__IF__ThreeDTouchController__) */
