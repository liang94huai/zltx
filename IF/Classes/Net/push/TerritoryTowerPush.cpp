//
//  TerritoryTowerPush.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/6/8.
//
//

#include "TerritoryTowerPush.h"
#include "WorldController.h"
#include "QueueController.h"
#include "SoundController.h"
#include "WorldMapView.h"
void TerritoryTowerPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    WorldController::getInstance()->forceUpdateTerritoryTowerTile = true;
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    if(params){
        if (params->objectForKey("uuid")) {
            string uuid = params->valueForKey("uuid")->getCString();
            if(WorldController::getInstance()->m_marchInfo.find(uuid) != WorldController::getInstance()->m_marchInfo.end()){
                auto &info = WorldController::getInstance()->m_marchInfo[uuid];
                if (info.ownerType == PlayerSelf) {
                    //unsigned int qid = QueueController::getInstance()->getMarchQueueQid(info.uuid);
                    //GlobalData::shared()->allQueuesInfo.erase(qid);
                    //CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUEUE_REMOVE);
                    if(info.stateType == StateOccupy){
                        info.territoryStat = params->valueForKey("stat")->intValue();
                        info.territoryFinishTime = params->valueForKey("updateTime")->doubleValue();
                        info.territoryStartTime = params->valueForKey("startTime")->doubleValue();
                        SoundController::sharedSound()->playEffects(Music_Sfx_world_click_march);
                        QueueController::getInstance()->addMarchQueueInfo(info.uuid);
                    }
                }
            }
        }
        
        if (params->objectForKey("pointId")) {
            int pointId = params->valueForKey("pointId")->intValue();
            auto it = WorldController::getInstance()->m_cityInfo.find(pointId);
            if (it != WorldController::getInstance()->m_cityInfo.end()) {
                if (params->objectForKey("buildingSpd")) {
                    it->second.m_towerInfo.buildSpeed = params->valueForKey("buildingSpd")->doubleValue() * 1000;
                }
                if (params->objectForKey("curDef")) {
                    it->second.m_towerInfo.defence = params->valueForKey("curDef")->intValue();
                }
                if (params->objectForKey("maxDef")) {
                    it->second.m_towerInfo.defenceMax = params->valueForKey("maxDef")->intValue();
                }
                if (params->objectForKey("stat")) {
                    it->second.m_towerInfo.state = (AAreaState)params->valueForKey("stat")->intValue();
                }
                if (params->objectForKey("updateTime")) {
                    it->second.m_towerInfo.updateTime = params->valueForKey("updateTime")->doubleValue() / 1000;
                }
                if (params->objectForKey("startTime")) {
                    it->second.m_towerInfo.startTime = params->valueForKey("startTime")->doubleValue() / 1000;
                }
                if (WorldMapView::instance()) {
                    WorldMapView::instance()->onRefreshAAreaBuilding(it->second.cityIndex, it->second.m_towerInfo.state, false, true);
                }
            }
        }
    }
}