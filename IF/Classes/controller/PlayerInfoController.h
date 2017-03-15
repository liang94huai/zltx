//
//  PlayerInfoController.h
//  IF
//
//  Created by liutianhou on 17-2-23.
//
//

#ifndef __IF__PlayerInfoController__
#define __IF__PlayerInfoController__

#include "CommonInclude.h"
#include "GlobalData.h"


class PlayerInfoController: public CCObject
{
public:
    static PlayerInfoController* getInstance();
    ~PlayerInfoController();
    PlayerInfoController();
    void saveSDKdateEnter();
    void saveSDKdateRole();
    void saveSDKdateRoleUp();
private:
    void init();
    
};
#endif
