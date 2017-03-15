//
//  IFAllianceRankCmd.hpp
//  IF
//
//  Created by wangdianzhen on 16/1/11.
//
//

#ifndef IFAllianceRankCmd_hpp
#define IFAllianceRankCmd_hpp

#include "CommandBase.h"
#define ALLIANCE_RANK_CMD "al.permission.modify"

class IFAllianceRankCmd: public CommandBase {
public:
    IFAllianceRankCmd(std::string infoStr);
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    std::string m_infoStr;
};

#endif /* IFAllianceRankCmd_hpp */
