//
//  LeaveAllianceCommand.h
//  IF
//
//  Created by ganxiaohua on 14-7-9.
//
//

#ifndef __IF__LeaveAllianceCommand__
#define __IF__LeaveAllianceCommand__
#include "CommandBase.h"

#define ALLIANCE_AL_LEAVE "al.leave.v2"

class LeaveAllianceCommand: public CommandBase {
public:
    LeaveAllianceCommand(const string& msg,int post=0,string sendLocalTime="",const string& dialog="",CCArray* msgArr=NULL) : CommandBase(ALLIANCE_AL_LEAVE){

        putParam("msg", CCString::create(msg));
        putParam("lang",CCString::create(CCCommonUtils::getLanguage()));
        putParam("post",CCInteger::create(post));
        putParam("sendLocalTime",CCString::create(sendLocalTime));
        if (dialog!="") {
            putParam("dialog", CCString::create(dialog));
        }
        if (msgArr!=NULL) {
            putParam("msgarr", msgArr);
        }
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__LeaveAllianceCommand__) */
