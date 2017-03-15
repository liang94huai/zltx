//
//  GetAllianceWarListCommand.h
//  IF
//
//  Created by ganxiaohua on 14-4-16.
//
//

#ifndef __IF__GetAllianceWarListCommand__
#define __IF__GetAllianceWarListCommand__

#include "CommandBase.h"
#define ALLIANCE_WAR_TEAM_LIST "alliance.team.ls"
#define ALLIANCE_WAR_TEAM_INFO "alliance.team.info"
class GetAllianceWarListCommand: public CommandBase {
public:
    GetAllianceWarListCommand();
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class GetAllianceWarInfoCommand: public CommandBase {
public:
    GetAllianceWarInfoCommand(string teamUuid):CommandBase(ALLIANCE_WAR_TEAM_INFO){
        m_teamUuid = teamUuid;
        putParam("teamId", CCString::create(teamUuid));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string m_teamUuid;
};
#endif /* defined(__IF__GetAllianceWarListCommand__) */
