//
//  RecScienceAllianceCommand.cpp
//  IF
//
//  Created by 邢晓瑞 on 16/1/11.
//
//

#include "RecScienceAllianceCommand.hpp"
#include "ScienceController.h"


bool RecScienceAllianceCommand::handleRecieve(Dictionary* dic)
{
    if (dic->valueForKey("cmd")->compare("al.targetscience.set") != 0)
        return false;
    __Dictionary* params = _dict(dic->objectForKey("params"));
    if (params->objectForKey("errorCode")) {
        
    } else {
        auto& map = ScienceController::getInstance()->allianceScienceMap;
        int scienceId = atoi(m_scienceId.c_str());
        if (map.find(scienceId) != map.end()) {
            if (m_isuse == 1) {
                map[scienceId].isRecommended = true;
            } else {
                map[scienceId].isRecommended = false;
            }
            __Dictionary* d = __Dictionary::create();
            d->setObject(__Integer::create(m_isuse), "m_isuse");
            d->setObject(__Integer::create(scienceId), "m_scienceId");
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("msg.recommend.al.science", d);
        }
    }
    return true;
}