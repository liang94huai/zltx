//
//  ShowStatusItemCommand.h
//  IF
//
//  Created by ganxiaohua on 14-4-21.
//
//

#ifndef __IF__ShowStatusItemCommand__
#define __IF__ShowStatusItemCommand__

#include "CommandBase.h"
#define SHOW_STATUS_ITEM "show.status.item"
#define CANCEL_STATUS_ITEM "status.remove"
#define SKIN_USE_CMD "skin.use"

class ShowStatusItemCommand: public CommandBase {
public:
    ShowStatusItemCommand();
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class CancelStatusItemCommand: public CommandBase{
public:
    CancelStatusItemCommand(int statusid):CommandBase(CANCEL_STATUS_ITEM),m_statusId(statusid){
        putParam("statusId", CCInteger::create(statusid));
    };
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    int m_statusId;
};
//使用城堡皮肤
class SkinUseCommand: public CommandBase{
public:
    SkinUseCommand(string statusId):CommandBase(SKIN_USE_CMD){
        putParam("id", CCString::create(statusId));
    };
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};
//推送
class StatusPush{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};
#endif /* defined(__IF__ShowStatusItemCommand__) */
