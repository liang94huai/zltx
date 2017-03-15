//
//  HoolaiBiCommand.cpp
//  IF
//
//  Created by xxrdsg on 15-6-3.
//
//

#include "HoolaiBiCommand.h"
#include "HoolaiBISdkUtils.hpp"

bool HoolaiBiInstallCommand::handleRecieve(CCDictionary* dict)
{
    if (dict->valueForKey("cmd")->compare("hoolai.install") != 0) {
        return false;
    }
    if (!dict->objectForKey("params")) {
        return false;
    }
    auto params = _dict(dict->objectForKey("params"));
    
    int result = atoi(params->valueForKey("result")->getCString());
    if (result) {
        HoolaiBISdkUtils::sendInstall(params);
    }
    
    return true;
}

bool HoolaiBiDauCommand::handleRecieve(CCDictionary* dict)
{
    if (dict->valueForKey("cmd")->compare("hoolai.dau") != 0) {
        return false;
    }
    if (!dict->objectForKey("params")) {
        return false;
    }
    auto params = _dict(dict->objectForKey("params"));
    int result = atoi(params->valueForKey("result")->getCString());
    if (result) {
        HoolaiBISdkUtils::sendDau(params);
    }
    return true;
}
