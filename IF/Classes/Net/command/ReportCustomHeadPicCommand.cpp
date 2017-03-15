//
//  ReportCustomHeadPicCommand.cpp
//  IF
//
//  Created by xupengzhan on 15-9-1.
//
//

#include "ReportCustomHeadPicCommand.h"
#include "ChatServiceCocos2dx.h"
bool ReportCustomHeadPicCommand::handleRecieve(cocos2d::CCDictionary *dict){
    
    CCLOGFUNC("");
    if (dict->valueForKey("cmd")->compare(REPORT_PICVER) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        return false;
    }
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CCLog("多语言举报成功 == %s",_lang("105781").c_str());
    ChatServiceCocos2dx::flyHint("", "", _lang("105781"), 0.0, 0.0, true);
#endif
    return true;
}