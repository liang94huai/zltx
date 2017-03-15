//
//  PushDrangonBattlePortChange.cpp
//  IF
//
//  Created by ganxiaohua on 15/9/29.
//
//

#include "PushDrangonBattlePortChange.h"
#include "WorldController.h"
#include "WorldMapView.h"

void PushDrangonBattlePortChange::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    
    int id = params->valueForKey("id")->intValue();
    std::string ai = params->valueForKey("ai")->getCString();
    int type = params->valueForKey("type")->intValue();
    double occupyST = params->valueForKey("occupyST")->doubleValue();//占领生效的时间
    
    auto itBuilding = WorldController::getInstance()->m_dbuildingInfo.find(id);
    if (itBuilding != WorldController::getInstance()->m_dbuildingInfo.end()) {
        WorldController::getInstance()->m_dbuildingInfo[id].occupyST = occupyST;
        WorldController::getInstance()->m_dbuildingInfo[id].ai = ai;
    }
    auto it = WorldController::getInstance()->m_cityInfo.find(id);
    if (it != WorldController::getInstance()->m_cityInfo.end()) {
        WorldController::getInstance()->m_cityInfo[id].dragonTileInfo.endTime = occupyST;
        WorldController::getInstance()->m_cityInfo[id].dragonTileInfo.allianceUid = ai;
        if (WorldMapView::instance()) {
            WorldMapView::instance()->addZhengYing(WorldController::getInstance()->m_cityInfo[id]);
        }
        int left =  (WorldController::getInstance()->getTime() - occupyST)/1000;
        if (type==TransferPoint && ai!="" && ai==GlobalData::shared()->playerInfo.allianceInfo.uid && left>=0) {
            WorldController::getInstance()->freePortCnt += WorldController::getInstance()->freePortCnt + 1;
        }
    }
    if (WorldMapView::instance()) {
        WorldMapView::instance()->addTransferArea();
    }
    if (type==TransferPoint) {
        if (WorldMapView::instance()) {
            WorldMapView::instance()->updateTransferPoint(id);
        }
    }
}