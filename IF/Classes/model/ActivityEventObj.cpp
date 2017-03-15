//
//  ActivityEventObj.cpp
//  IF
//
//  Created by ganxiaohua on 15-1-5.
//
//

#include "ActivityEventObj.h"
#include "ActivityController.h"

void ActivityEventObj::parse(CCDictionary *dict){
    if(dict==NULL) return ;
    if(dict->objectForKey("startTime")){
        startTime = dict->valueForKey("startTime")->doubleValue() / 1000;
        startTime = GlobalData::shared()->changeTime(startTime);
    }
    if(dict->objectForKey("endTime")){
        endTime = dict->valueForKey("endTime")->doubleValue() / 1000;
        endTime = GlobalData::shared()->changeTime(endTime);
    }
    if(dict->objectForKey("id")){
        id = dict->valueForKey("id")->getCString();
    }
    if(dict->objectForKey("reward")){
        m_reward = dynamic_cast<CCArray*>(dict->objectForKey("reward"));
    }
    if (dict->objectForKey("exchange")){
        exchange = dict->valueForKey("exchange")->getCString();
    }
    if (dict->objectForKey("popup")) {
        popup = dict->valueForKey("popup")->intValue();
    } else {
        popup = -1;
    }
    
    name = CCCommonUtils::getNameById(id);
    desc = _lang(CCCommonUtils::getPropById(id, "desc").c_str());
    descDia = CCCommonUtils::getPropById(id, "desc").c_str();
    desc_info = CCCommonUtils::getPropById(id, "desc_info");
    type = atoi(CCCommonUtils::getPropById(id, "type").c_str());
    sort = atoi(CCCommonUtils::getPropById(id, "sort").c_str());
    activityid = CCCommonUtils::getPropById(id, "activityid");
    if (dict->objectForKey("activityid")) {
        activityid = dict->valueForKey("activityid")->getCString();
    }
    Advertise_pic = CCCommonUtils::getPropById(id, "Advertise_pic");
    story = CCCommonUtils::getPropById(id, "story");
    num = CCCommonUtils::getPropById(id, "num");
    {
        showDialogs.clear();
        string dias = CCCommonUtils::getPropById(id, "way");
        CCCommonUtils::splitString(dias, "|", showDialogs);
        rewardIds.clear();
        string tids = CCCommonUtils::getPropById(id, "item");
        CCCommonUtils::splitString(tids, ";", rewardIds);
    }
    if(m_reward == nullptr)
    {
        m_reward = CCArray::create();
    }
    if(type==3){
        if(dict->objectForKey("siegeST")){
            double siegeST = dict->valueForKey("siegeST")->doubleValue()/1000;
            siegeST = GlobalData::shared()->changeTime(siegeST);
            ActivityController::getInstance()->aActivityInfo.siegeST = siegeST;
        }
        if(dict->objectForKey("siegeET")){
            double siegeET = dict->valueForKey("siegeET")->doubleValue()/1000;
            siegeET = GlobalData::shared()->changeTime(siegeET);
            ActivityController::getInstance()->aActivityInfo.siegeET = siegeET;
        }
    }
    if(dict->objectForKey("kingdomList")){
        m_kingdomList = dynamic_cast<CCArray*>(dict->objectForKey("kingdomList"));
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string code = dict->valueForKey("androidmd5")->getCString();
    if(!code.empty()){
        this->md5 = code;
    }
#else
    std::string code = dict->valueForKey("iosmd5")->getCString();
    if(!code.empty()){
        this->md5 =code;
    }
#endif
}

CCDictionary* ActivityEventObj::toDic()
{
    CCDictionary* dic = CCDictionary::create();
    dic->setObject(CCString::create(name), "name");
    dic->setObject(CCString::create(CC_ITOA(type)), "type");
    dic->setObject(CCString::create(CC_ITOA(startTime)), "startTime");
    dic->setObject(CCString::create(CC_ITOA(endTime)), "endTime");
    dic->setObject(CCString::create(id), "id");
    dic->setObject(CCString::create(desc), "desc");
    dic->setObject(CCString::create(descDia), "descDia");
    dic->setObject(CCString::create(desc_info), "desc_info");
    dic->setObject(CCString::create(CC_ITOA(sort)), "sort");
    dic->setObject(CCString::create(activityid), "activityid");
    dic->setObject(CCString::create(Advertise_pic), "Advertise_pic");
    dic->setObject(CCString::create(exchange), "exchange");
    dic->setObject(CCString::create(story), "story");
    dic->setObject(CCString::create(num), "num");
    return dic;
}


////////////////////////////////////////////////////////////////////////////
//
//
//   LuaActivityInfo
//
//
////////////////////////////////////////////////////////////////////////////
void LuaActivityInfo::setInfo(CCDictionary *_dict){
    this->actID = _dict->valueForKey("id")->intValue();
    if(_dict->objectForKey("startTime")){
        this->startTime = _dict->valueForKey("startTime")->doubleValue();
    }else{
        this->startTime = 0;
    }
    if(_dict->objectForKey("endTime")){
        this->endTime = _dict->valueForKey("endTime")->doubleValue();
    }else{
        this->endTime = 0;
    }
    if(_dict->objectForKey("openPoint")){
        this->openPoint = (LuaActOpenPoint)_dict->valueForKey("openPoint")->intValue();
    }else{
        this->openPoint = MAINUI_1;
    }
    if(_dict->objectForKey("openRef")){
        this->openRef = _dict->valueForKey("openRef")->getCString();
    }else{
        this->openRef = "";
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string code = _dict->valueForKey("androidmd5")->getCString();
    if(!code.empty()){
        this->md5 = code;
    }
#else
    std::string code = _dict->valueForKey("iosmd5")->getCString();
    if(!code.empty()){
        this->md5 =code;
    }
#endif
}
bool LuaActivityInfo::isExpire(){
    if(startTime > 0 && endTime > 0){
        double time =  GlobalData::shared()->getWorldTime();
        double sT = GlobalData::shared()->changeTime(startTime);
        double eT = GlobalData::shared()->changeTime(endTime);
        auto tmp = time - sT;
        auto tmp1 = eT - time;
        if(tmp>0 && tmp1>0){
            return false;
        }
    }else if(startTime == 0 && endTime == 0){
        return false;
    }
    
    return true;
}