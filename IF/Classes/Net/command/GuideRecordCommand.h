//
//  GuideRecordCommand.h
//  IF
//
//  Created by 李锐奇 on 13-11-13.
//
//

#ifndef __IF__GuideRecordCommand__
#define __IF__GuideRecordCommand__

#include "CommandBase.h"

#define GUIDE_RECORD_COMMAND "stat.tt"
#define CLIENT_RECORD_SET_COMMAND "clientRecord.set"

class GuideRecordCommand : public CommandBase{
public:
    GuideRecordCommand(std::string stepId) : CommandBase(GUIDE_RECORD_COMMAND){
        putParam("id", CCString::create(stepId));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class ClientRecordSetCommand : public CommandBase{
public:
    ClientRecordSetCommand(std::string key) : CommandBase(CLIENT_RECORD_SET_COMMAND){
        putParam("set", CCString::create(key));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

#endif /* defined(__IF__GuideRecordCommand__) */
