//
//  CrossServerStateLSCommand.cpp
//  IF
//
//  Created by ganxiaohua on 15/12/9.
//
//

#include "CrossServerStateLSCommand.hpp"
#include "WorldController.h"

bool CrossServerStateLSCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(CROSS_SERVER_STATE_LS_COMMAND) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        //CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
    }else{
        CCArray *arr = dynamic_cast<CCArray*>(params->objectForKey("list"));
        if(arr){
            int num = arr->count();
            for (int i=0; i<num; i++) {
                auto oneServerInfo = _dict(arr->objectAtIndex(i));
                int serverId = oneServerInfo->valueForKey("id")->intValue();
                int count = WorldController::getInstance()->m_serverList.size();
                for (int j=0; j<count; j++) {
                    if (WorldController::getInstance()->m_serverList[j].serverId==serverId) {
                        WorldController::getInstance()->m_serverList[j].enemyList.clear();
                        if (oneServerInfo->objectForKey("fightState")) {
                            WorldController::getInstance()->m_serverList[j].fightState = (ServerStateType)oneServerInfo->valueForKey("fightState")->intValue();
                        }
                        if(oneServerInfo->objectForKey("enemyList")){
                            CCArray* enemyList = dynamic_cast<CCArray*>(oneServerInfo->objectForKey("enemyList"));
                            if(enemyList!=NULL){
                                int enemyNum = enemyList->count();
                                for(int k=0;k<enemyNum;k++){
                                    ServerEnemyInfo enemyInfo;
                                    auto oneEnemy = _dict(enemyList->objectAtIndex(k));
                                    enemyInfo.serverId = oneEnemy->valueForKey("serverId")->intValue();
                                    enemyInfo.fightState = (ServerStateType)oneEnemy->valueForKey("state")->intValue();
                                    WorldController::getInstance()->m_serverList[j].enemyList.push_back(enemyInfo);
                                    
                                }
                            }
                        }
                        break;
                    }
                }
            }
        }
        //callSuccess(NetResult::create(Error_OK, params));
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SERVER_LIST_BACK, params);
    return true;
}