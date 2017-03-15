//
//  AllianceDailyQuestInfo.cpp
//  IF
//
//  Created by 张军 on 15/10/22.
//
//

#include "AllianceDailyQuestInfo.h"
#include "ToolController.h"


AllianceDailyQuestInfo::AllianceDailyQuestInfo(CCDictionary *dict){
    m_id="";
    m_name="";
    m_description="";
    m_color="";
    m_stat="";
    m_uid="";
    m_publishUid="";
    m_extraReward="";
    m_baseReward="";
    m_finishTime=0;
    m_vanishTime=0;
    m_allianceId="";
    m_acceptReward="";
    m_publishName="";
    m_acceptStat="";
    m_publishStat="";
    m_acceptUid="";
    m_startTime=0;
    m_publishTime=0;
    m_acceptName="";
    m_freequeue = 0;
    m_nextgold = 0;
    m_picVer = -1;
    m_pic = "";
    m_nameid = "";
    setDataByDict(dict);
}

void AllianceDailyQuestInfo::setDataByDict(CCDictionary *dict){
    if (dict) {
        if (dict->objectForKey("taskId")) {
            m_id=dict->valueForKey("taskId")->getCString();
        }
        
        if (m_id!="") {
            m_name = _lang(CCCommonUtils::getPropById(m_id, "name"));
            m_nameid = CCCommonUtils::getPropById(m_id, "name");
            m_color = CCCommonUtils::getPropById(m_id, "color");
            m_description = _lang(CCCommonUtils::getPropById(m_id, "description"));
        }
        
        if (dict->objectForKey("stat")) {
            m_stat=dict->valueForKey("stat")->getCString();
        }
        if (dict->objectForKey("uid")) {
            m_uid=dict->valueForKey("uid")->getCString();
        }
        if (dict->objectForKey("publishUid")) {
            m_publishUid=dict->valueForKey("publishUid")->getCString();
        }
        if (dict->objectForKey("extraReward")) {
            m_extraReward=dict->valueForKey("extraReward")->getCString();
//            m_extraReward=orderRewards(m_extraReward);
        }
        if (dict->objectForKey("baseReward")) {
            m_baseReward=dict->valueForKey("baseReward")->getCString();
            m_baseReward=orderRewards(m_baseReward);
        }
        if (dict->objectForKey("finishTime")) {
            m_finishTime=dict->valueForKey("finishTime")->doubleValue();
        }
        if (dict->objectForKey("vanishTime")) {
            m_vanishTime=dict->valueForKey("vanishTime")->doubleValue();
        }
        if (dict->objectForKey("startTime")) {
            m_startTime=dict->valueForKey("startTime")->doubleValue();
        }
        
        if (dict->objectForKey("allianceId")) {
            m_allianceId=dict->valueForKey("allianceId")->getCString();
        }
        if (dict->objectForKey("acceptReward")) {
            m_acceptReward=dict->valueForKey("acceptReward")->getCString();
            m_acceptReward=orderRewards(m_acceptReward);
        }
        if (dict->objectForKey("publishName")) {
            m_publishName=dict->valueForKey("publishName")->getCString();
        }
        if (dict->objectForKey("acceptStat")) {
            m_acceptStat=dict->valueForKey("acceptStat")->getCString();
        }
        if (dict->objectForKey("publishStat")) {
            m_publishStat=dict->valueForKey("publishStat")->getCString();
        }
        if (dict->objectForKey("acceptUid")) {
            m_acceptUid=dict->valueForKey("acceptUid")->getCString();
        }
        if (dict->objectForKey("publishTime")) {
            m_publishTime=dict->valueForKey("publishTime")->doubleValue();
        }
        if (dict->objectForKey("acceptName")) {
            m_acceptName=dict->valueForKey("acceptName")->getCString();
        }
        if (dict->objectForKey("freequeue")){
            m_freequeue = dict->valueForKey("freequeue")->intValue();
        }
        if (dict->objectForKey("gold")){
            m_nextgold = dict->valueForKey("gold")->intValue();
        }
        if (dict->objectForKey("picVer")){
            m_picVer = dict->valueForKey("picVer")->intValue();
        }
        if (dict->objectForKey("pic")){
            m_pic = dict->valueForKey("pic")->getCString();
        }
        
    }
}

string AllianceDailyQuestInfo::orderRewards(string rewards){
    int goodsIdx=3;
    map<int ,map<int,string> > tempMap;
    vector<string> rewardVec;
    CCCommonUtils::splitString(rewards, "|", rewardVec);
    for (int i=0; i<rewardVec.size(); i++) {
        string subStr = rewardVec[i];   //goods,200321,2
        vector<string> subVec;
        CCCommonUtils::splitString(subStr, ",", subVec);
        if (subVec.size()==3) {
            string strType = subVec[0];  //auto& tool = ToolController::getInstance()->getToolInfoById(atoi(rId.c_str()));
            string strId = subVec[1];
            auto& tool = ToolController::getInstance()->getToolInfoById(atoi(strId.c_str()));
            string amount = tool.getPara();
            int idx = getIndexByName(strType);
            if (idx==3) {
                if (amount == "") {
                    amount = "0";
                }
                tempMap[idx].insert(pair<int, string>(atoi(amount.c_str()),subStr));
                goodsIdx++;
            }else{
                if (amount == "") {
                    amount = "0";
                }
                tempMap[idx].insert(pair<int, string>(atoi(amount.c_str()),subStr));
            }
        }
    }
    string rewardStr="";
    for (int j=0; j<tempMap.size(); j++) {
        map<int,string> ::iterator iter  =  tempMap[j].end();
        while(iter!=tempMap[j].begin()) {
            iter--;
            string str = iter->second;
            if(str != ""){
                rewardStr+=str;
                rewardStr+="|";
            }
        }
    }
    return rewardStr;
}

int AllianceDailyQuestInfo::getIndexByName(std::string name){
    if("exp" == name){
        return 0;
    }
//    if("wood" == name){
//        return 1;
//    }
//    if("food" == name){
//        return 2;
//    }
//    if("iron" == name){
//        return 3;
//    }
//    if("stone" == name){
//        return 4;//秘银
//    }
    if ("goods" == name) {
        return 1;
    }
    return 3;
}







