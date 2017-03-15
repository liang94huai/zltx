//
//  ToolCommand.h
//  IF
//
//  Created by fubin on 14-3-4.
//
//

#ifndef IF_ToolCommand_h
#define IF_ToolCommand_h

#include "CommandBase.h"
#define TOOL_BUY_COMMAND "item.buy"
#define TOOL_USE_COMMAND "item.use"
#define TOOL_HOT_GET_COMMAND "hot.item.get"
#define TOOL_HOT_BUY_COMMAND "hot.item.buy"

#define TOOL_MERCHANT_GET_COMMAND "hot.item.v2.get"
#define TOOL_MERCHANT_REFRESH_COMMAND "hot.item.refresh"
#define TOOL_MERCHANT_BUY_COMMAND "hot.item.v2.buy"

#define TOOL_CREATE_COMMAND "create.tool"
#define TOOL_HAVEST_COMMAND "havest.tool"
#define TOOL_QUEUE_COMMAND "tool.queue.buy"
#define TOOL_QUEUE_CANCEL_COMMAND "rm.tool"

#define TOOL_ITEM_COMPOSE "item.compose"

#define LONG_BUY_COMMAND "crystal.buy"
#define EQUIP_BUY_COMMAND "equipment.buy"

#define TOOL_REDPACKET_SEND "redPackets.sd"
#define TOOL_REDPACKET_GET "redPackets.rv"
#define TOOL_REDPACKET_GET_IDS "redPackets.rvd.id"
#define TOOL_REDPACKET_GET_STATUS "redPackets.status"

#define TOOL_VIPX_SCORE "vipx.score"

class ToolUseCommand;

class ToolBuyCommand: public CommandBase {
public:
    ToolBuyCommand(string cmd, string itemId, int num,bool buyAndUse,int batch = 0):
    CommandBase(cmd, CHECK_CMD_PARAM),m_buyAndUse(buyAndUse),m_num(num){
        putParam("itemId", CCString::create(itemId));
        putParam("num", CCInteger::create(num));
        if(batch>0){
            putParam("batch", CCInteger::create(batch));
        }
        auto dict = CCDictionary::create();
        dict->setObject(CCString::create(itemId), "itemId");
        this->setCheckDict(dict);
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    bool m_buyAndUse;
    int m_num;
private:
};

class ToolUseCommand: public CommandBase {
public:
    ToolUseCommand(string cmd, string uuid, int num):
    CommandBase(cmd){
        putParam("uuid", CCString::create(uuid));
        putParam("num", CCInteger::create(num));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};
//
class ToolHotGetCommand: public CommandBase {
public:
    ToolHotGetCommand():
    CommandBase(TOOL_HOT_GET_COMMAND){
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class ToolHotBuyCommand: public CommandBase {
public:
    ToolHotBuyCommand(string cmd, string itemId, int num,int price):
    CommandBase(cmd){
        putParam("itemId", CCString::create(itemId));
        putParam("num", CCInteger::create(num));
        putParam("price", CCInteger::create(price));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};
//行走商人CMD
//
class ToolMerchantGetCommand: public CommandBase {
public:
    ToolMerchantGetCommand():
    CommandBase(TOOL_MERCHANT_GET_COMMAND){
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};
class ToolMerchantRefreshCommand: public CommandBase {
public:
    ToolMerchantRefreshCommand():
    CommandBase(TOOL_MERCHANT_REFRESH_COMMAND){
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};
class ToolMerchantBuyCommand: public CommandBase {
public:
    ToolMerchantBuyCommand(const string itemId,const int itemNum,const int num,const int price,const int priceType):
    CommandBase(TOOL_MERCHANT_BUY_COMMAND){
        putParam("itemId", CCString::create(itemId));
        putParam("itemNum", CCInteger::create(itemNum));
        putParam("price", CCInteger::create(price));
        putParam("priceType", CCInteger::create(priceType));
        putParam("num", CCInteger::create(num));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class ToolCreateCommand: public CommandBase {
public:
    ToolCreateCommand(int itemId,int index=-1):
    CommandBase(TOOL_CREATE_COMMAND){
        putParam("itemId", CCString::create(CC_ITOA(itemId)));
        if (index>-1) {
            putParam("index", CCInteger::create(index));
        }
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};
class ToolHavestCommand: public CommandBase {
public:
    ToolHavestCommand(string quuid,int _index = 0):
    CommandBase(TOOL_HAVEST_COMMAND){
        putParam("quuid", CCString::create(quuid));
        putParam("index", CCInteger::create(_index));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};
//添加建造材料空位
class ToolQueueAddCommand: public CommandBase {
public:
    ToolQueueAddCommand(int num):
    CommandBase(TOOL_QUEUE_COMMAND){
        putParam("num", CCInteger::create(num));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

//取消建造材料
class ToolQueueCancelCommand: public CommandBase {
public:
    ToolQueueCancelCommand(int _index):
    CommandBase(TOOL_QUEUE_CANCEL_COMMAND){
        putParam("index", CCInteger::create(_index));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class ItemComposeCommand: public CommandBase {
public:
    ItemComposeCommand(int itemId,int pernum,bool allneed):CommandBase(TOOL_ITEM_COMPOSE)
    {
        m_itemId = itemId;
        m_perNum = pernum;
        m_allNeed = allneed;
        putParam("itemId", CCString::create(CC_ITOA(itemId)));
        putParam("pernum", CCInteger::create(m_perNum));
        putParam("allneed", CCInteger::create(m_allNeed));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    int m_itemId;
    int m_perNum;
    bool m_allNeed;
};

#pragma mark 红包
class RedPacketsSendCommand: public CommandBase {
public:
    RedPacketsSendCommand(int range,int total,int num,const string& uuid,const string& msg):CommandBase(TOOL_REDPACKET_SEND)
    {
        putParam("range", CCInteger::create(range));
        putParam("total", CCInteger::create(total));
        putParam("num", CCInteger::create(num));
        putParam("uuid", CCString::create(uuid));
        putParam("msg", CCString::create(msg));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class RedPacketsGetCommand: public CommandBase {
public:
    RedPacketsGetCommand(const string& uid, const string& serverId,bool isViewOnly = false):CommandBase(TOOL_REDPACKET_GET),m_uid(uid),m_serverId(serverId),m_isViewOnly(isViewOnly)
    {
        putParam("uid", CCString::create(uid));
        putParam("serverId", CCInteger::create(atoi(serverId.c_str())));
        if(isViewOnly)
        {
            putParam("info", CCInteger::create(1));
        }
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string m_uid;
    std::string m_serverId;
    bool m_isViewOnly;
};

class RedPacketUidsGetCommand: public CommandBase {
public:
    RedPacketUidsGetCommand():CommandBase(TOOL_REDPACKET_GET_IDS)
    {
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class GetRedPacketStatusCommand: public CommandBase {
public:
    GetRedPacketStatusCommand(string uid,string serverId):CommandBase(TOOL_REDPACKET_GET_STATUS),m_redPackageUid(uid),m_serverId(serverId)
    {
        putParam("uid", CCString::create(uid));
        putParam("serverId", CCInteger::create(atoi(serverId.c_str())));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    string m_redPackageUid;
    string m_serverId;
};

class TrimVipPointCommand: public CommandBase {
public:
    TrimVipPointCommand():CommandBase(TOOL_VIPX_SCORE){}
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

class EquipBuyCommand: public CommandBase {
public:
    EquipBuyCommand(string itemId, int num):CommandBase(EQUIP_BUY_COMMAND){
        putParam("itemId", CCString::create(itemId));
        putParam("count", CCInteger::create(num));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

#endif
