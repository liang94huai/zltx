//
//  QijiInfoCommand.cpp
//  IF
//
//  Created by 李朝辉 on 15-9-1.
//
//

#include "QijiInfoCommand.h"
#include "FunBuildController.h"

bool QijiInfoCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(MIRACLEUNLOCK) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        CCLOG("errocode: '%s'", pStr->getCString());
    }else{
        FunBuildController::getInstance()->endUnlockQJBuild(params);
    }
    return true;
}
