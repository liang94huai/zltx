//
//  IFScoutPush.cpp
//  IF
//
//  Created by wangdianzhen on 15/11/9.
//
//

#include "IFScoutPush.hpp"
#include "WorldMapView.h"
#include "SceneController.h"
void IFScoutPush::handleResponse(cocos2d::CCDictionary *dict)
{
    if(!dict){
        return;
    }
    cocos2d::CCDictionary* result = dynamic_cast<cocos2d::CCDictionary*>(dict->objectForKey("params"));
    if (result->objectForKey("pointId")) {
        int pointIndex = result->valueForKey("pointId")->intValue();
        if (SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD) {
            if (WorldMapView::instance()) {
                if (WorldController::getInstance()->m_cityInfo.find(pointIndex) != WorldController::getInstance()->m_cityInfo.end()) {
                    auto cityInfo = WorldController::getInstance()->m_cityInfo[pointIndex];
                    cityInfo.isScouted = true;
                    WorldMapView::instance()->createCity(cityInfo);
                }
            }
        }
    }
}