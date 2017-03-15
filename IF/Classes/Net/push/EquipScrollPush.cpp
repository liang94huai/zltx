//
//  EquipScrollPush.cpp
//  IF
//
//  Created by 张军 on 15/9/8.
//
//

#include "EquipScrollPush.h"
#include "EquipmentController.h"

void EquipScrollPush::handleResponse(CCDictionary *dict){
    if (!dict)
        return;
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    if (params->objectForKey("msg")) {
        string str = params->valueForKey("msg")->getCString();
        EquipmentController::getInstance()->insertScrollInfo(str,true);
    }

}