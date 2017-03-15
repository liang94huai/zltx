//
//  CrossServerStateLSCommand.hpp
//  IF
//
//  Created by ganxiaohua on 15/12/9.
//
//

#ifndef CrossServerStateLSCommand_hpp
#define CrossServerStateLSCommand_hpp

#include "CommandBase.h"

#define CROSS_SERVER_STATE_LS_COMMAND "cross.server.state.ls"

class CrossServerStateLSCommand: public CommandBase {
public:
    CrossServerStateLSCommand() : CommandBase(CROSS_SERVER_STATE_LS_COMMAND){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* CrossServerStateLSCommand_hpp */
