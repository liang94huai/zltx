//
//  CommandBase.cpp
//  IF
//
//  Created by zhaohf on 13-9-04.
//
//

#include "CommandBase.h"
#include "NetController.h"
#include "CCThreadManager.h"
#include "../../view/guiExt/md5.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "utf8.h"
#endif
void CommandBase::send()
{
    if(GlobalData::shared()->useNewKey){
        std::vector <string> arr;
        CCDictElement* ele = NULL;
        CCDICT_FOREACH(m_data, ele)
        {
            if (ele && ele->getObject())
            {
                std::string key = ele->getStrKey();
                if(key!="newbasekey"){
                    arr.push_back(key);
                }
            }
        }
        std::sort(arr.begin(),arr.end(),[](const string a, const string b){
            return a < b;
        });
        string key= "";
        for(auto i:arr)
        {
            CCObject* item=m_data->objectForKey(i);
            if(item)
            {
                if(dynamic_cast<CCString*>(item))
                {
                    key+= dynamic_cast<CCString*>(item)->getCString();
                
                }
                else if(dynamic_cast<CCInteger*>(item))
                {
                    key+= CC_ITOA(dynamic_cast<CCInteger*>(item)->getValue());
                }
                else if(dynamic_cast<CCBoolean*>(item))
                {
                    key+= dynamic_cast<CCBoolean*>(item)->getValue()?"true":"false";
                }
            }
        }
        MD5 md5;
        string checkkey = key + "2FdVk1+1PKAesqBfgKtv/rnW7hiYFcA=";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        vector <unsigned short> utf16result;
        utf8::unchecked::utf8to16((char*)checkkey.c_str(), (char*)checkkey.c_str() + checkkey.length(), back_inserter(utf16result));
        // unsigned short utf16string[] = utf16result.;
        vector<unsigned char> utf8result;
        utf8::unchecked::utf16to8(utf16result.begin(), utf16result.end(), back_inserter(utf8result));
        md5.update((const void*)&(utf8result[0]), utf8result.size());
#else
        md5.update(checkkey.c_str(), checkkey.length());
#endif
        putParam("newbasekey", CCString::create(md5.toString()));
    }
    //    遍历输出发送结构
//    CCLOG("遍历网络发送输出发送结构======================开始");
//    CCDictElement * pElement;
//    CCDICT_FOREACH(m_data, pElement)
//    {
//        const char * key = pElement->getStrKey();
//        __String * value = static_cast<__String*>(pElement->getObject());
//        CCLOG("%s==%s",key,value->getCString());
//    }
//    CCLOG("遍历网络发送输出发送结构======================结束");
    NetController::shared()->send(this);
}

void CommandBase::cancel()
{
    setCallback(NULL);
}

void CommandBase::putParam(const std::string &key, cocos2d::CCObject *value)
{
    m_data->setObject(value, key);
}

void CommandBase::callFail(CCObject *result)
{
    if (!getFailCallback())
        return;
    
    CCCallFuncO * func=getFailCallback();
    func->retain();
    setFailCallback(NULL);
    func->setObject(result);
    func->execute();
    func->release();
}

void CommandBase::callSuccess(NetResult *result)
{
    if (!getSuccessCallback())
        return;
    
    CCCallFuncO * func=getSuccessCallback();
    func->retain();
    setSuccessCallback(NULL);
    func->setObject(result);
    func->execute();
    func->release();
}

