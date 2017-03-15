//
//  SettingXmlCommand.hpp
//  IF
//
//  Created by ganxiaohua on 15/12/2.
//
//

#ifndef SettingXmlCommand_hpp
#define SettingXmlCommand_hpp

#include "CommandBase.h"

#define SETTING_XML_COMMAND "setting.xml"

class SettingXmlCommand: public CommandBase {
public:
    SettingXmlCommand() : CommandBase(SETTING_XML_COMMAND){
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* SettingXmlCommand_hpp */
