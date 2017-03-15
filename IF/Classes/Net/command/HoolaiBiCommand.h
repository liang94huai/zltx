//
//  HoolaiBiCommand.h
//  IF
//
//  Created by xxrdsg on 15-6-3.
//
//

#ifndef __IF__HoolaiBiCommand__
#define __IF__HoolaiBiCommand__

#include "CommandBase.h"

class HoolaiBiInstallCommand: public CommandBase
{
public:
    HoolaiBiInstallCommand():CommandBase("hoolai.install")
    {
    };
protected:
    virtual bool handleRecieve(CCDictionary*);
};

class HoolaiBiDauCommand: public CommandBase
{
public:
    HoolaiBiDauCommand(): CommandBase("hoolai.dau")
    {
    }
protected:
    virtual bool handleRecieve(CCDictionary*);
};
#endif /* defined(__IF__HoolaiBiCommand__) */
