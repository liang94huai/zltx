//
//  QijiInfoCommand.h
//  IF
//
//  Created by 李朝辉 on 15-9-1.
//
//

#ifndef __IF__QijiInfoCommand__
#define __IF__QijiInfoCommand__


#include "CommandBase.h"
#define MIRACLEUNLOCK "miracle.unlock"


class QijiInfoCommand: public CommandBase {
public:
    QijiInfoCommand() : CommandBase(MIRACLEUNLOCK){
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__QijiInfoCommand__) */
