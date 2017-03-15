//
//  SettingXmlCommand.cpp
//  IF
//
//  Created by ganxiaohua on 15/12/2.
//
//

#include "SettingXmlCommand.hpp"

bool SettingXmlCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(SETTING_XML_COMMAND) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        //CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
    }else{
        CCArray* settings = dynamic_cast<CCArray*>(params->objectForKey("setting"));
        if (settings!=NULL) {
            int num = settings->count();
            for (int i=0; i<num; i++) {
                auto info = _dict(settings->objectAtIndex(i));
                int type = info->valueForKey("type")->intValue();
                auto it = GlobalData::shared()->settingMap.find(type);
                if (it!=GlobalData::shared()->settingMap.end()) {
                    GlobalData::shared()->settingMap[type].parse(info);
                }else{
                    GlobalData::shared()->settingMap[type] = SettingObject(info);
                }
                SettingObject sobj(info);
                
            }
        }
        callSuccess(NetResult::create(Error_OK, params));
    }
    return true;
}