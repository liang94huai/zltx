//
//  SendHeartPush.cpp
//  IF
//
//  Created by 邢晓瑞 on 16/1/15.
//
//

#include "SendHeartPush.hpp"
#include "SceneContainer.h"
#include "WorldMapView.h"
#include "ToolController.h"

void SendHeartPush::handleResponse(Dictionary* dict)
{
    if (!dict) {
        return;
    }
    auto params = _dict(dict->objectForKey("params"));
    string cmd = dict->valueForKey("cmd")->getCString();
    if (cmd == "push.heart.having") {
        if (!GlobalData::shared()->playerInfo.isInSelfServer())
            return;
        if (GlobalData::shared()->serverType >= SERVER_BATTLE_FIELD)
            return;
        if (!params->objectForKey("point") || !params->objectForKey("effecttype") || !params->objectForKey("time") || !params->objectForKey("receiveuid"))
        {
            return;
        }
        int index = params->valueForKey("point")->intValue();
        int effecttype = params->valueForKey("effecttype")->intValue();
        int timestamp = (int)((params->valueForKey("time")->doubleValue()) / 1000);
        
        string uid = params->valueForKey("receiveuid")->getCString();
        if (uid == GlobalData::shared()->playerInfo.uid)
        {
            if (params->objectForKey("name")) {
                string name = params->valueForKey("name")->getCString();
                if (effecttype == 1) {
                    CCCommonUtils::flyHint("", "", _lang_1("150538", name.c_str()));
                }
            }
        }
        bool force = false;
        auto it = ToolController::getInstance()->m_heartEndTime.find(index);
        if (it != ToolController::getInstance()->m_heartEndTime.end())
        {
            if (timestamp > it->second) {
                it->second = timestamp;
                force = true;
            }
        } else {
            ToolController::getInstance()->m_heartEndTime[index] = timestamp;
            force = true;
        }
        if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD && WorldMapView::instance() && GlobalData::shared()->playerInfo.isInSelfServer() && GlobalData::shared()->serverType < SERVER_BATTLE_FIELD){
            WorldMapView::instance()->addSendHeartParticle(index, timestamp, effecttype, force);
        }
    }
}