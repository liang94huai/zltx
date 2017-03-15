//
//  RecScienceAllianceCommand.hpp
//  IF
//
//  Created by 邢晓瑞 on 16/1/11.
//
//

#ifndef RecScienceAllianceCommand_hpp
#define RecScienceAllianceCommand_hpp

#include "CommandBase.h"

class RecScienceAllianceCommand: public CommandBase
{
public:
    RecScienceAllianceCommand(int isuse, string scienceId): CommandBase("al.targetscience.set"), m_isuse(isuse), m_scienceId(scienceId)
    {
        putParam("scienceId", __String::create(scienceId));
        putParam("isuse", __Integer::create(m_isuse));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    int m_isuse;
    string m_scienceId;
};

#endif /* RecScienceAllianceCommand_hpp */
