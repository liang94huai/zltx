//
//  ReportPlayChatCommand.h
//  IF
//
//  Created by xupengzhan on 15-10-9.
//
//

#ifndef __IF__ReportPlayChatCommand__
#define __IF__ReportPlayChatCommand__

#include "CommandBase.h"

#define REPORT_PALYER_CHAT "chat.report"

class ReportPlayChatCommand : public CommandBase{
public:
    ReportPlayChatCommand(std::string uid,std::string content) :
    CommandBase(REPORT_PALYER_CHAT){
        putParam("reportUid", CCString::create(uid));
        putParam("content", CCString::create(content));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__ChatTestCommand__) */
