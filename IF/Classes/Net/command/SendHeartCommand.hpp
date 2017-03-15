//
//  SendHeartCommand.hpp
//  IF
//
//  Created by 邢晓瑞 on 16/1/15.
//
//

#ifndef SendHeartCommand_hpp
#define SendHeartCommand_hpp

#include "CommandBase.h"
#include "ToolController.h"
#define SEND_HEART_COMMAND "heart.send"
#define MSG_SEND_HEART_COMMAND_SUC "MSG_SEND_HEART_COMMAND_SUC"
#define MSG_IVITE_VIEW_CHOOSE_TO_PLAYER "MSG_IVITE_VIEW_CHOOSE_TO_PLAYER"

class SendHeartCommand: public CommandBase
{
public:
    SendHeartCommand(const string& uuid, const string& itemId): CommandBase(SEND_HEART_COMMAND), m_toUid(uuid), m_itemId(itemId)
    {
        putParam("uid", __String::create(m_toUid));
        putParam("itemId", __String::create(m_itemId));
        string itemuuid = (ToolController::getInstance()->getToolInfoById(atoi(itemId.c_str()))).uuid;
        putParam("uuid", __String::create(itemuuid));
    };
protected:
    virtual bool handleRecieve(Dictionary* dic);
private:
    string m_toUid;
    string m_itemId;
};

#endif /* SendHeartCommand_hpp */
