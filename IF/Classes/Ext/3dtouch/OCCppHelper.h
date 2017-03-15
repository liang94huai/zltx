//
//  OCCppHelp.h
//  IF
//
//  Created by liuzengyou on 15/10/25.
//
//

#ifndef __IF__OCCppHelper__
#define __IF__OCCppHelper__


#include "cocos2d.h"
#include "../CCTypesExt.h"
#include "../../model/PlayerInfo.h"
#

#define IOS_kUPDATE_PLAYERINFO      @"ios_kUPDATE_PLAYERINFO"

class OCCppHelper: public cocos2d::CCObject {

public:
    OCCppHelper();
    static OCCppHelper* share();
    
    void notificationUpdatePlayerInfo(PlayerInfo* info, std::string medalIconId="", std::string achievePro="", std::string medalCntStr="");
    
    void notificationIOSUpdatePlayerInfo();
    
public:
    CCSafeObject<PlayerInfo> m_info;
    std::string m_medalIconId;
    std::string m_achievePro;
    std::string m_medalCntStr;
    
};

#endif