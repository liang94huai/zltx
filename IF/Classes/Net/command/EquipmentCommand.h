//
//  EquipmentCommand.h
//  IF
//
//  Created by fubin on 14-12-3.
//
//

#ifndef IF_EquipmentCommand_h
#define IF_EquipmentCommand_h

#include "CommandBase.h"
#define EQU_CT_COMMAND "equipment.create"
#define EQU_PUTON_COMMAND "equip.putOn"
#define EQU_TAKEOFF_COMMAND "equip.takeOff"
#define EQU_DES_COMMAND "equipment.break"
#define MATERIAL_CT_COMMAND "material.create"
#define EQU_HAVEST_COMMAND "equip.q.finish"
#define EQU_SELL_LONG_COMMAND "dragonshard.sell"

#define EQU_SCROLLINFO_COMMAND "ep.bc.info"
#define EQU_SCROLLSWITCH_COMMAND "equip.scroll"

#define EQU_MELT_COMMAND "dragonshard.refine"
#define EQU_MELT_SAVE_COMMAND "ds.ref.confirm"

#define MATERIAL_DC_COMMAND "material.decompse"

class EquipmentCreateCommand;
class MaterialCreateCommand;

class EquipmentCommand: public CommandBase {
public:
    EquipmentCommand(string uuid, string cmd):
    CommandBase(cmd){
        putParam("uuid", CCString::create(uuid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class EquipmentCreateCommand: public CommandBase {
public:
    EquipmentCreateCommand(string uuid, string qItemId, vector<int> toolItemIds, int gold=0,int paramGold=0):
    CommandBase(EQU_CT_COMMAND){
        putParam("uuid", CCString::create(uuid));
        putParam("itemId", CCString::create(qItemId));
        putParam("gold", CCInteger::create(paramGold));
        auto arr = CCArray::create();
        for (int i=0; i<toolItemIds.size(); i++) {
            arr->addObject(CCInteger::create(toolItemIds[i]));
        }
        putParam("tools", arr);
        _gold = gold;
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    int _gold;
};

class MaterialCreateCommand: public CommandBase {
public:
    MaterialCreateCommand(string itemId,int type);
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    vector<string> m_Uuids;
    int m_type; //0-材料 1-装备
};

class HavestEquipCommand: public CommandBase {
public:
    HavestEquipCommand(string quuid):
    CommandBase(EQU_HAVEST_COMMAND){
        putParam("qUUID", CCString::create(quuid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class getScrollEquipInfoCommand: public CommandBase {
public:
    getScrollEquipInfoCommand():CommandBase(EQU_SCROLLINFO_COMMAND){
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class setScrollEquipSwitchCommand: public CommandBase {
public:
    setScrollEquipSwitchCommand(int setting):
    CommandBase(EQU_SCROLLSWITCH_COMMAND){
        putParam("setting", CCInteger::create(setting));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class EquipMeltCommand: public CommandBase {
public:
    EquipMeltCommand(string uuid, int gold):
    CommandBase(EQU_MELT_COMMAND){
        putParam("uuid", CCString::create(uuid));
        putParam("gold", CCInteger::create(gold));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class EquipMeltSaveCommand: public CommandBase {
public:
    EquipMeltSaveCommand(string uuid):
    CommandBase(EQU_MELT_SAVE_COMMAND){
        putParam("uuid", CCString::create(uuid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class MaterialDecomposeCommand: public CommandBase {
public:
    MaterialDecomposeCommand(string itemId):CommandBase(MATERIAL_DC_COMMAND){
        putParam("itemId", CCString::create(itemId));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

#endif
