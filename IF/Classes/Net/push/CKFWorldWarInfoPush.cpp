//
//  CKFWorldWarInfoPush.cpp
//  IF
//
//  Created by ganxiaohua on 15/8/3.
//
//

#include "CKFWorldWarInfoPush.h"
#include "WorldController.h"
#include "WorldMapView.h"

void CKFWorldWarInfoPush::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    
    string uid = params->valueForKey("uid")->getCString();
    int srcPoint = params->valueForKey("srcPoint")->intValue();
    int srcStreakNum = params->valueForKey("srcStreakNum")->intValue();
    int dstPoint = params->valueForKey("dstPoint")->intValue();
    int dstStreakNum = params->valueForKey("dstStreakNum")->intValue();
    auto it = WorldController::getInstance()->m_cityInfo.find(srcPoint);
    if (it != WorldController::getInstance()->m_cityInfo.end()) {
        WorldController::getInstance()->m_cityInfo[srcPoint].winNum = srcStreakNum;
    }
    auto dstIt = WorldController::getInstance()->m_cityInfo.find(dstPoint);
    if (dstIt != WorldController::getInstance()->m_cityInfo.end()) {
        WorldController::getInstance()->m_cityInfo[dstPoint].winNum = dstStreakNum;
    }
    if (WorldMapView::instance()) {
        WorldMapView::instance()->updateWinNum(srcPoint);
        WorldMapView::instance()->updateWinNum(dstPoint);
    }
}