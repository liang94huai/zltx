//
//  SendHeartCommand.cpp
//  IF
//
//  Created by 邢晓瑞 on 16/1/15.
//
//

#include "SendHeartCommand.hpp"

bool SendHeartCommand::handleRecieve(Dictionary* dict)
{
    if (dict->valueForKey("cmd")->compare(SEND_HEART_COMMAND) != 0)
    {
        return false;
    }
    auto params = _dict(dict->objectForKey("params"));
    if (params->objectForKey("errorCode")) {
        CCCommonUtils::flyHint("", "", _lang(params->valueForKey("errorCode")->getCString()));
        return false;
    } else {
        if (params->objectForKey("worldpoint"))
        {
            auto& map = ToolController::getInstance()->m_toolInfos;
            auto it = map.find(atoi(m_itemId.c_str()));
            if (it != map.end())
            {
                auto& info = it->second;
                if (info.getCNT() > 0) {
                    info.setCNT(info.getCNT() - 1);
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFREASH_TOOL_DATA);
                }
            }
            int index = params->valueForKey("worldpoint")->intValue();
            if (index < 0) {
                return true;
            }
            __Integer* ccint = __Integer::create(index);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SEND_HEART_COMMAND_SUC, ccint);
        }
    }
    return true;
}