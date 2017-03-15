//
//  ArmyInfo.cpp
//  IF
//
//  Created by 李锐奇 on 14-2-28.
//
//

#include "ArmyInfo.h"
#include "GlobalData.h"
#include "TrainSoldiertimeCommand.h"
#include "FortCommand.h"
#include "QueueController.h"

ArmyInfo::ArmyInfo(cocos2d::CCDictionary *dict)
:power(1.0),train(0),march(0)
{
    if (dict->objectForKey("range")) {
        range = dict->valueForKey("range")->intValue();
    } else {
        range =0;
    }
    if (dict->objectForKey("id")) {
        armyId = dict->valueForKey("id")->getCString();
    } else {
        armyId ="";
    }
    if (dict->objectForKey("heal_silver")) {
        healSilver = dict->valueForKey("heal_silver")->floatValue();
    }else {
        healSilver =0.0f;
    }
    if (dict->objectForKey("heal_res")) {
        healRes = dict->valueForKey("heal_res")->floatValue();
    }else {
        healRes = 0.0f;
    }
    if (dict->objectForKey("heal_time")) {
        healTime = dict->valueForKey("heal_time")->floatValue();
    }else {
        healTime = 0.0f;
    }
    if (dict->objectForKey("time")) {
        time = dict->valueForKey("time")->intValue();
    }else {
        time = 0;
    }
    if (dict->objectForKey("free")) {
        free = dict->valueForKey("free")->floatValue();
    }else {
        free = 0.0f;
    }
    if (dict->objectForKey("upkeep")) {
        upkeep = dict->valueForKey("upkeep")->floatValue();
    }else {
        upkeep = 0.0f;
    }
    if (dict->objectForKey("defen")) {
        defen = dict->valueForKey("defen")->floatValue();
    }else {
        defen = 0.0f;
    }
    if (dict->objectForKey("move")) {
        move = dict->valueForKey("move")->intValue();
    }else {
        move = 0;
    }
    if (dict->objectForKey("attack")) {
        attack = dict->valueForKey("attack")->floatValue();
    }else {
        attack = 0.0f;
    }
    if (dict->objectForKey("defen")) {
        defence = dict->valueForKey("defen")->floatValue();
    }else {
        defence = 0.0f;
    }
    if (dict->objectForKey("pve")) {
        pve = dict->valueForKey("pve")->intValue();
    }else {
        pve = 0;
    }
    if (dict->objectForKey("wood")) {
        wood = dict->valueForKey("wood")->intValue();
    }else {
        wood = 0;
    }
    if (dict->objectForKey("food")) {
        food = dict->valueForKey("food")->intValue();
    }else {
        food = 0;
    }
    if (dict->objectForKey("silver")) {
        silver = dict->valueForKey("silver")->intValue();
    }else {
        silver = 0;
    }
    if (dict->objectForKey("iron")) {
        iron = dict->valueForKey("iron")->intValue();
    }else {
        iron = 0;
    }
    if (dict->objectForKey("stone")) {
        stone = dict->valueForKey("stone")->intValue();
    }else {
        stone = 0;
    }
    if (dict->objectForKey("level")) {
        level = dict->valueForKey("level")->intValue();
    }else {
        level = 0;
    }
    if (dict->objectForKey("load")) {
        load = dict->valueForKey("load")->intValue();
    }else {
        load = 0;
    }
    if (dict->objectForKey("speed")) {
        speed = dict->valueForKey("speed")->intValue();
    }else {
        speed = 0;
    }
    if (dict->objectForKey("march")) {
        march = dict->valueForKey("march")->floatValue();
    }else {
        march = 0.0f;
    }
    if (dict->objectForKey("health")) {
        health = dict->valueForKey("health")->floatValue();
    }else {
        health = 0.0f;
    }
    if(dict->objectForKey("power")){
        power = dict->valueForKey("power")->floatValue();
    }else{
        power = 1;
    }
    if(dict->objectForKey("train")){
        train = dict->valueForKey("train")->intValue();
    } else {
        train = 0;
    }
    armyLevel = atoi(armyId.c_str()) % 100;
    finishTime = 0;
    isArmy = true;
    setFinishiTime(dict);
}

void ArmyInfo::setArmyInfo(cocos2d::CCDictionary *dict)
{
    if (dict->objectForKey("range")) {
        range = dict->valueForKey("range")->intValue();
    }
    if (dict->objectForKey("id")) {
        armyId = dict->valueForKey("id")->getCString();
    }
    if (dict->objectForKey("heal_silver")) {
        healSilver = dict->valueForKey("heal_silver")->floatValue();
    }
    if (dict->objectForKey("heal_res")) {
        healRes = dict->valueForKey("heal_res")->floatValue();
    }
    if (dict->objectForKey("heal_time")) {
        healTime = dict->valueForKey("heal_time")->floatValue();
    }
    if (dict->objectForKey("time")) {
        time = dict->valueForKey("time")->floatValue();
    }
    if (dict->objectForKey("free")) {
        free = dict->valueForKey("free")->floatValue();
    }
    if (dict->objectForKey("upkeep")) {
        upkeep = dict->valueForKey("upkeep")->floatValue();
    }
    if (dict->objectForKey("defen")) {
        defen = dict->valueForKey("defen")->floatValue();
    }
    if (dict->objectForKey("move")) {
        move = dict->valueForKey("move")->intValue();
    }
    if (dict->objectForKey("attack")) {
        attack = dict->valueForKey("attack")->floatValue();
    }
    if (dict->objectForKey("defen")) {
        defence = dict->valueForKey("defen")->floatValue();
    }
    if (dict->objectForKey("pve")) {
        pve = dict->valueForKey("pve")->intValue();
    }
    if (dict->objectForKey("wood")) {
        wood = dict->valueForKey("wood")->intValue();
    }
    if (dict->objectForKey("food")) {
        food = dict->valueForKey("food")->intValue();
    }
    if (dict->objectForKey("silver")) {
        silver = dict->valueForKey("silver")->intValue();
    }
    if (dict->objectForKey("iron")) {
        iron = dict->valueForKey("iron")->intValue();
    }
    if (dict->objectForKey("stone")) {
        stone = dict->valueForKey("stone")->intValue();
    }
    if (dict->objectForKey("level")) {
        level = dict->valueForKey("level")->intValue();
    }
    if (dict->objectForKey("load")) {
        load = dict->valueForKey("load")->intValue();
    }
    if (dict->objectForKey("speed")) {
        speed = dict->valueForKey("speed")->intValue();
    }
    if (dict->objectForKey("march")) {
        march = dict->valueForKey("march")->floatValue();
    }
    if (dict->objectForKey("health")) {
        health = dict->valueForKey("health")->floatValue();
    }
    if (dict->objectForKey("power")) {
        power = dict->valueForKey("power")->floatValue();
    }
    if(dict->objectForKey("train")){
        train = dict->valueForKey("train")->floatValue();
    }
    if (dict->objectForKey("finish")) {
        finishTime = 0;
        setFinishiTime(dict);
    }
}

std::string ArmyInfo::getRealItemId(){
//    int id = atoi(armyId.c_str());
//    return CC_ITOA(id);
    return armyId;
}

std::string ArmyInfo::getName(){
    std::string itemId = getRealItemId();
    return CCCommonUtils::getNameById(itemId);
}

std::string ArmyInfo::getIcon(){
    std::string itemId = getRealItemId();
    return CCCommonUtils::getPropById(itemId, "icon") + ".png";
}

std::string ArmyInfo::getBodyIcon(){
    std::string itemId = getRealItemId();
    return "ico" + itemId+ ".png";
}

std::string ArmyInfo::getHeadIcon(){
    std::string itemId = getRealItemId();
    return "ico" + itemId+ "_small.png";
}

void ArmyInfo::setFinishiTime(cocos2d::CCDictionary *dict){
    std::string str = dict->valueForKey("finish")->getCString();
    if(str != ""){
        finishTime = atof(str.c_str()) / 1000;
        if(finishTime != 0){
            finishTime = GlobalData::shared()->changeTime(finishTime);
        }
        train = dict->valueForKey("train")->intValue();
    }
}

std::string ArmyInfo::getDesc(){
    std::string itemId = getRealItemId();
    std::string dialog = CCCommonUtils::getPropById(itemId, "description");
    return _lang(dialog).c_str();
}

std::string ArmyInfo::getBuildId(){
    std::string itemId = getRealItemId();
    std::string dialog = CCCommonUtils::getPropById(itemId, "buildId");
    return dialog;
}

void ArmyInfo::setEndTime(double time, bool isCancel){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    if(time == 0){
        if(!isCancel){
            free += train;
            std::string icon = getBodyIcon();
            std::string name = getName();
            if(isArmy){
                int index = atof(armyId.c_str())/100 - 1070 + 200;
                CCLoadSprite::doResourceByCommonIndex(index, true);
                CCCommonUtils::flyHint(icon, _lang("103673"), _lang_1("103674", name.c_str()));
                CCLoadSprite::doResourceByCommonIndex(index, false);
            }else{
                CCCommonUtils::flyHint(icon, _lang("103675"), _lang_1("103676", name.c_str()));
            }

        }
        train = 0;
    }
    finishTime = time;
}

std::string ArmyInfo::getAttrIcon(int index)
{
    std::string itemId = getRealItemId();
    std::string icon = CCCommonUtils::getPropById(itemId, "featureicon");
    vector<string> strArr;
    CCCommonUtils::splitString(icon, ";", strArr);
    if (index >= strArr.size())
    {
        return "";
    }
    return strArr.at(index);
}

void ArmyInfo::getAttrDesc(int index, vector<string>& descVec)
{
    /*type==0取title, type==1取info*/
    std::string itemId = getRealItemId();
    std::string desc = CCCommonUtils::getPropById(itemId, "featuredescription");
    vector<string> strArr;
    CCCommonUtils::splitString(desc, "|", strArr);
    if (index >= strArr.size())
    {
        descVec.push_back("");
        descVec.push_back("");
        return;
    }
    vector<string> strArr1;
    CCCommonUtils::splitString(strArr.at(index), ";", strArr1);
    if(strArr1.size() <= 1){
        descVec.push_back("");
        descVec.push_back("");
        return;
    }
    descVec.push_back(strArr1.at(0));
    descVec.push_back(strArr1.at(1));
}

void ArmyInfo::getLvInfo(int index, int star,vector<string>& descVec)
{
    /*type==0取title, type==1取info*/
    if(star <= 0){
        return;
    }
    if(index < 0 || index >= 4){
        return;
    }
    std::string itemId = getRealItemId();
    std::string desc = CCCommonUtils::getPropById(itemId, "StarFeatureiconLv");
    vector<string> strArr;
    CCCommonUtils::splitString(desc, "|", strArr);
    if (star > strArr.size())
    {
        descVec.push_back("");
        return;
    }
    vector<string> strArr1;
    CCCommonUtils::splitString(strArr.at(star-1), ";", strArr1);
    if(strArr1.size() <= 3){
        descVec.push_back("");
        return;
    }
    descVec.push_back(strArr1.at(index));
}

void ArmyInfo::getNextLvInfo(int index, int star,vector<string>& descVec)
{
    /*type==0取title, type==1取info*/
    if(star < 0){
        return;
    }
    if(index < 0 || index >= 4){
        return;
    }
    std::string itemId = getRealItemId();
    std::string desc = CCCommonUtils::getPropById(itemId, "StarFeatureiconNextLv");
    vector<string> strArr;
    CCCommonUtils::splitString(desc, "|", strArr);
    if (star >= strArr.size())
    {
        descVec.push_back("");
        return;
    }
    vector<string> strArr1;
    CCCommonUtils::splitString(strArr.at(star), ";", strArr1);
    if(strArr1.size() <= 3){
        descVec.push_back("");
        return;
    }
    descVec.push_back(strArr1.at(index));
}