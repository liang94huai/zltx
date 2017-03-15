//
//  ReportPlayChatCommand.cpp
//  IF
//
//  Created by xupengzhan on 15-10-9.
//
//

#include "ReportPlayChatCommand.h"

bool ReportPlayChatCommand::handleRecieve(cocos2d::CCDictionary *dict){
    
    CCLOGFUNC("");
    if (dict->valueForKey("cmd")->compare(REPORT_PALYER_CHAT) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        return false;
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CCCommonUtils::flyHint("", _lang("105394"),_lang("105394"));
#endif
    
    return true;
}
