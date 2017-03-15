//
//  HoolaiBISdkUtils.cpp
//  zltx
//
//  Created by tao.yu on 2017/3/8.
//
//

#include "HoolaiBISdkUtils.hpp"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "HoolaiFastAccess/CCAccessSDK.h"
#include "HoolaiFastAccess/GameProxy.h"
#else
#import <AccessSDK/CCAccessSDK.h>
#endif
#include "GlobalData.h"
#include "NetController.h"
#include "CCDevice.h"

const char* HoolaiBISdkUtils::getNetStatus()
{
    switch (cocos2d::extension::CCDevice::getNetworkStatus()) {
        case 0:
            return "NONE";
            break;
        case 1:
            return "WIFI";
            break;
        case 2:
            return "3G/4G";
            break;
        default:
            return "Others";
            break;
    }
}

std::string HoolaiBISdkUtils::makeBiJson(CCDictionary* arr)
{
    std::string result = "{";
    DictElement * pElement = NULL;
    CCDICT_FOREACH(arr, pElement){
        auto value = (CCString *)(pElement->getObject());
        auto ele = StringUtils::format("\"%s\":\"%s\"",pElement->getStrKey(),value->getCString());
        result.append(ele).append(",");
    }
    result.pop_back();
    result.append("}");
//    CCLOG("%s",result.c_str());
    return result;
}

void HoolaiBISdkUtils::sendInstall(CCDictionary* params)
{
    string metric = "Install";
    
    auto arr = __Dictionary::create();
    arr->setObject(CCString::create(GlobalData::shared()->platformUserUid), "userid");
    arr->setObject(CCString::create(CCUserDefault::sharedUserDefault()->getStringForKey("server_name")), "clientid");
    arr->setObject(CCString::create(cocos2d::extension::CCDevice::getChannel()), "source");
    arr->setObject(CCString::create(HoolaiBISdkUtils::getNetStatus()), "affiliate");
    arr->setObject(CCString::create(GlobalData::shared()->biChannel), "creative");
    arr->setObject(CCString::create(""), "family");
    arr->setObject(CCString::create(""), "genus");
    arr->setObject(CCString::create(""), "from_userid");
    arr->setObject(CCString::create(params->valueForKey("install_date")->getCString()), "install_date");
    arr->setObject(CCString::create(params->valueForKey("install_time")->getCString()), "install_time");
    
    string client_version = GlobalData::shared()->version;
    string phone_type = cocos2d::extension::CCDevice::getHandSetInfo();
    string phone_version = cocos2d::extension::CCDevice::getOSVersion();
    string ratio = CCString::createWithFormat("%d*%d", (int)(Director::getInstance()->getWinSize().height), (int)(Director::getInstance()->getWinSize().width))->getCString();
    string service = cocos2d::extension::CCDevice::getCarrierName();
    string download_from = GlobalData::shared()->biChannel;
    string extra = CCString::createWithFormat("client_version:%s,phone_type:%s,phone_version:%s,ratio:%s,service:%s,download_from:%s",client_version.c_str(),phone_type.c_str(),phone_version.c_str(),ratio.c_str(),service.c_str(),download_from.c_str())->getCString();
    arr->setObject(CCString::create(extra), "extra");
    arr->setObject(CCString::create(cocos2d::extension::CCDevice::getHoolaiUid()), "udid");
    arr->setObject(CCString::create(GlobalData::shared()->playerInfo.uid), "roleid");
    
//    string jsonData = CCJSONConverter::sharedConverter()->strFrom(arr);
    string jsonData = HoolaiBISdkUtils::makeBiJson(arr);
    CCAccessSDK::sharedInstance()->sendBIData(metric, jsonData);
}

void HoolaiBISdkUtils::sendDau(CCDictionary* params)
{
    string metric = "Dau";
    auto arr = __Dictionary::create();
    arr->setObject(CCString::create(GlobalData::shared()->platformUserUid), "userid");
    arr->setObject(CCString::create(CCUserDefault::sharedUserDefault()->getStringForKey("server_name")), "clientid");
    arr->setObject(CCString::create(cocos2d::extension::CCDevice::getChannel()), "source");
    arr->setObject(CCString::create(HoolaiBISdkUtils::getNetStatus()), "affiliate");
    arr->setObject(CCString::create(GlobalData::shared()->biChannel), "creative");
    arr->setObject(CCString::create(""), "family");
    arr->setObject(CCString::create(""), "genus");
    arr->setObject(CCString::create(NetController::shared()->getIp()), "ip");
    arr->setObject(CCString::create(""), "from_uid");
    arr->setObject(CCString::create(params->valueForKey("dau_date")->getCString()), "dau_date");
    arr->setObject(CCString::create(params->valueForKey("dau_time")->getCString()), "dau_time");
    
    string client_version = GlobalData::shared()->version;
    string phone_type = cocos2d::extension::CCDevice::getHandSetInfo();
    string phone_version = cocos2d::extension::CCDevice::getOSVersion();
    string ratio = CCString::createWithFormat("%d*%d", (int)(Director::getInstance()->getWinSize().height), (int)(Director::getInstance()->getWinSize().width))->getCString();
    string service = cocos2d::extension::CCDevice::getCarrierName();
    string download_from = GlobalData::shared()->biChannel;
    string extra = CCString::createWithFormat("client_version:%s,phone_type:%s,phone_version:%s,ratio:%s,service:%s,download_from:%s",client_version.c_str(),phone_type.c_str(),phone_version.c_str(),ratio.c_str(),service.c_str(),download_from.c_str())->getCString();
    arr->setObject(CCString::create(extra), "extra");
    arr->setObject(CCString::create(cocos2d::extension::CCDevice::getHoolaiUid()), "udid");
    arr->setObject(CCString::create(GlobalData::shared()->playerInfo.uid), "roleid");
    
    string jsonData = HoolaiBISdkUtils::makeBiJson(arr);
    CCAccessSDK::sharedInstance()->sendBIData(metric, jsonData);
}

void HoolaiBISdkUtils::sendPayment(CCDictionary* params)
{
    string metric = "Payment";
    params->setObject(CCString::create(GlobalData::shared()->platformUserUid), "userid");
    params->setObject(CCString::create(CCUserDefault::sharedUserDefault()->getStringForKey("server_name")), "clientid");
    params->setObject(CCString::create(GlobalData::shared()->biChannel), "provider");
    params->setObject(CCString::create(NetController::shared()->getIp()), "ip");
    string phone_type = cocos2d::extension::CCDevice::getHandSetInfo();
    string phone_version = cocos2d::extension::CCDevice::getOSVersion();
    string ratio = CCString::createWithFormat("%d*%d", (int)(Director::getInstance()->getWinSize().height), (int)(Director::getInstance()->getWinSize().width))->getCString();
    string extra = CCString::createWithFormat("user_level:%d,phone_type:%s,phone_version:%s,ratio:%s",GlobalData::shared()->playerInfo.level,phone_type.c_str(),phone_version.c_str(),ratio.c_str())->getCString();
    params->setObject(CCString::create(extra), "extra");
    params->setObject(CCString::create(cocos2d::extension::CCDevice::getHoolaiUid()), "udid");
    params->setObject(CCString::create(GlobalData::shared()->playerInfo.uid), "roleid");
    
    string jsonData = HoolaiBISdkUtils::makeBiJson(params);
    CCAccessSDK::sharedInstance()->sendBIData(metric, jsonData);
}

void HoolaiBISdkUtils::sendEconomy(CCDictionary *params)
{
    string metric = "Economy";
    params->setObject(CCString::create(GlobalData::shared()->platformUserUid), "userid");
    params->setObject(CCString::create(CCUserDefault::sharedUserDefault()->getStringForKey("server_name")), "clientid");
    params->setObject(CCString::create(""), "family");
    string download_from = GlobalData::shared()->biChannel;
    string extra = CCString::createWithFormat("download_from:%s,user_level:%d",download_from.c_str(),GlobalData::shared()->playerInfo.level)->getCString();
    params->setObject(CCString::create(extra), "extra");
    params->setObject(CCString::create(cocos2d::extension::CCDevice::getHoolaiUid()), "udid");
    params->setObject(CCString::create(GlobalData::shared()->playerInfo.uid), "roleid");
    string jsonData = HoolaiBISdkUtils::makeBiJson(params);
    CCAccessSDK::sharedInstance()->sendBIData(metric, jsonData);
}
