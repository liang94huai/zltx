//
//  GetUserInfoCommand.h
//  IF
//
//  Created by fubin on 14-4-3.
//
//

#ifndef IF_GetUserInfoCommand_h
#define IF_GetUserInfoCommand_h

#include "CommandBase.h"
#define GET_USER_INFO_CMD "get.user.info"

class GetUserInfoCommand: public CommandBase {
public:
    GetUserInfoCommand(std::string uid) : CommandBase(GET_USER_INFO_CMD){
        putParam("uid", CCString::create(uid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#define USER_EQUIP_SHOW_CMD "equip.show"

class SetUserEquipShowCommand: public CommandBase {
public:
    SetUserEquipShowCommand(int isShow) : CommandBase(USER_EQUIP_SHOW_CMD){
        putParam("show", CCInteger::create(isShow));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#define DALTON_CMD "colorblind.set"

class daltoncmd: public CommandBase {
public:
    daltoncmd(int isOn) : CommandBase(DALTON_CMD){
        putParam("set", CCInteger::create(isOn));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#define MEDELSHOW_CMD "medal.show"

class medelshowcmd: public CommandBase {
public:
    medelshowcmd(int isOn) : CommandBase(MEDELSHOW_CMD){
        putParam("show", CCInteger::create(isOn));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#define SUPERMAN_GET_COMMAND "celebrity.desc.get"

class supermanDesGetCommand: public CommandBase {
public:
    supermanDesGetCommand(string uid,int serverId) : CommandBase(SUPERMAN_GET_COMMAND){
        putParam("uid", CCStringMake(uid));
        putParam("serverId", CCInteger::create(serverId));
        
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};


#define SUPERMAN_SAVE_COMMAND "celebrity.desc.save"

class supermanDesSaveCommand: public CommandBase {
public:
    supermanDesSaveCommand(string desStr) : CommandBase(SUPERMAN_SAVE_COMMAND){
        putParam("description", CCStringMake(desStr));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};


#endif
