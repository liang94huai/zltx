//
//  CheckEmulatorCommand.h
//  IF
//
//  Created by wangguoqiang on 15-11-16.
//
//

#ifndef __IF__CheckEmulatorCommand__
#define __IF__CheckEmulatorCommand__

#include "CommandBase.h"

#define CHECK_RESULT "check.result"

class CheckEmulatorCommand: public CommandBase {
public:
    CheckEmulatorCommand(
                         std::string qemuBkpt = "",
                         std::string batteryStates = "",
                         std::string signCode = "",
                         std::string collectProcess = ""
                         ) : CommandBase(CHECK_RESULT)
    {
        if(qemuBkpt != "")
            putParam("qB", CCString::create(qemuBkpt));
        if(batteryStates != "")
            putParam("bS", CCString::create(batteryStates));
        if(signCode != "")
            putParam("sC", CCString::create(signCode));
        int hasProcess = 0;
        if(collectProcess != "")
            hasProcess = 1;
        putParam("cP", CCInteger::create(hasProcess));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__CheckEmulatorCommand__) */
