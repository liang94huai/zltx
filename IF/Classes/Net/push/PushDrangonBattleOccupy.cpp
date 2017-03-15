//
//  PushDrangonBattleOccupy.cpp
//  IF
//
//  Created by ganxiaohua on 15/10/23.
//
//

#include "PushDrangonBattleOccupy.hpp"
#include "WorldController.h"
#include "WorldMapView.h"

void PushDrangonBattleOccupy::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    
    int id = params->valueForKey("id")->intValue();
    std::string ai = params->valueForKey("ai")->getCString();
    int pointId = params->valueForKey("pointId")->intValue();
    string itemId = params->valueForKey("itemId")->getCString();
    string type = CCCommonUtils::getPropById(itemId, "type");
    int btype = atoi(type.c_str());
    string name = CCCommonUtils::getNameById(itemId);
    string camptype = _lang("140064");
    if (GlobalData::shared()->campType==CAMP_RED) {
        if (ai==GlobalData::shared()->playerInfo.allianceInfo.uid) {
            camptype = _lang("140064");
        }else{
            camptype = _lang("140063");
        }
    }else{
        if (ai==GlobalData::shared()->playerInfo.allianceInfo.uid) {
            camptype = _lang("140063");
        }else{
            camptype = _lang("140064");
        }
    }
    string tip  = "";
    switch (btype) {
        case Crystal:
        case Armory:
        case TrainingField:
            tip = _lang_2("140099",camptype.c_str(),name.c_str());
            break;
        case MedicalTower:
            tip = _lang_2("140100",camptype.c_str(),name.c_str());
            break;
        case BessingTower:
            tip = _lang_2("140101",camptype.c_str(),name.c_str());
            break;
        case TransferPoint:
        {
            string param2 = CCCommonUtils::getPropById(itemId, "parm2");
            tip = _lang_3("140102",camptype.c_str(),name.c_str(),param2.c_str());
        }
            break;
        case Barracks:
            tip = _lang_2("140103",camptype.c_str(),name.c_str());
            break;
        case DragonTower:
            tip = _lang_2("140104",camptype.c_str(),name.c_str());
            break;
        default:
            break;
    }
    auto it = WorldController::getInstance()->m_cityInfo.find(pointId);
    if (it != WorldController::getInstance()->m_cityInfo.end()) {
        WorldController::getInstance()->m_cityInfo[id].dragonTileInfo.allianceUid = ai;
    }
    
    double worldTime = GlobalData::shared()->getWorldTime();
    worldTime *= 1000;
    worldTime = GlobalData::shared()->renewTime(worldTime);
    auto time = worldTime+1000*60;
    if (tip!="") {
        CCCommonUtils::flySystemUpdateHint(time, true,tip.c_str(),FLY_HINT_SYSTEM);
    }
}