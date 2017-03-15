//
//  PushGmNotice.cpp
//  IF
//
//  Created by ganxiaohua on 15/8/21.
//
//

#include "PushGmNotice.h"

void PushGmNotice::handleResponse(CCDictionary *dict)
{
    if (!dict)
        return;
    cocos2d::CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) return;
    string content = params->valueForKey("content")->getCString();
    CCLOG("content:%s",content.c_str());
    if (content != "") {
        double worldTime = GlobalData::shared()->getWorldTime();
        worldTime *= 1000;
        worldTime = GlobalData::shared()->renewTime(worldTime);
        auto time = worldTime+1000*60;
        CCCommonUtils::flySystemUpdateHint(time, true,content,FLY_HINT_SYSTEM);
    }
}
