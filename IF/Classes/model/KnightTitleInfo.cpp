//
//  KnightTitleInfo.cpp
//  IF
//
//  Created by 付彬 on 16/1/7.
//
//

#include "KnightTitleInfo.hpp"

KnightTitleInfo::KnightTitleInfo(CCDictionary* dict)
{//<ItemSpec id="260100" level="1" site="0" color="0" name="104301" description="104302" icon="item701" material1="201010;1" material2="201060;1"/>
    this->itemId = dict->valueForKey("id")->intValue();
    this->name = dict->valueForKey("name")->getCString();
    this->des = dict->valueForKey("description")->getCString();
    this->icon = dict->valueForKey("icon")->getCString();
    
    this->bd_level = dict->valueForKey("building_level")->intValue();
    this->color = dict->valueForKey("color")->intValue();
    this->power = dict->valueForKey("power")->intValue();
    
    string useMate = dict->valueForKey("activate_array")->getCString();
    vector<string> tmpUseMateVec;
    CCCommonUtils::splitString(useMate, "|", tmpUseMateVec);
    for (int i=0; i<tmpUseMateVec.size(); i++)
    {
        if ( tmpUseMateVec[i] != "" ) {
            vector<string> tmpVec;
            CCCommonUtils::splitString(tmpUseMateVec[i], ";", tmpVec);
            if (tmpVec.size()>=2) {
                this->mateUseMap[ atoi(tmpVec[0].c_str()) ] = atoi(tmpVec[1].c_str());
            }
        }
    }
    
    string unlockMate = dict->valueForKey("activate_array")->getCString();
    vector<string> tmpUnlockMateVec;
    CCCommonUtils::splitString(unlockMate, "|", tmpUnlockMateVec);
    for (int i=0; i<tmpUnlockMateVec.size(); i++)
    {
        if ( tmpUnlockMateVec[i] != "" ) {
            vector<string> tmpVec;
            CCCommonUtils::splitString(tmpUnlockMateVec[i], ";", tmpVec);
            if (tmpVec.size()>=2) {
                this->mateUnlockMap[ atoi(tmpVec[0].c_str()) ] = atoi(tmpVec[1].c_str());
            }
        }
    }
    
    for (int i=1; i<=20; i++) {
        string key = "show";
        key = key+CC_ITOA(i);
        if (dict->objectForKey(key)) {
            this->showDias[i] = dict->valueForKey(key)->getCString();
        }else{
            break;
        }
    }
    
    for (int j=1; j<=20; j++) {
        string key = "num";
        key = key+CC_ITOA(j);
        if (dict->objectForKey(key)) {
            this->values[j] = dict->valueForKey(key)->floatValue();
        }else{
            break;
        }
    }
    
    for (int m=1; m<=20; m++) {
        string key = "para";
        key = key+CC_ITOA(m);
        if (dict->objectForKey(key)) {
            string tmpStr = dict->valueForKey(key)->getCString();
            vector<string> tmpVec;
            CCCommonUtils::splitString(tmpStr, "|", tmpVec);
            for (int n=0; n<tmpVec.size(); n++) {
                this->parasMap[m].push_back(atoi(tmpVec[n].c_str()));
            }
        }else{
            break;
        }
    }
}

string KnightTitleInfo::getEffFormatByOrd(int order)
{
    if (parasMap.find(order) != parasMap.end()) {
        if (parasMap[order].size()>0) {
            int eNum = parasMap[order][0];
            return CCCommonUtils::getEffFormat( CC_ITOA(eNum) );
        }
    }
    return "%";
}

string KnightTitleInfo::getEffPMByOrd(int order)
{
    if (parasMap.find(order) != parasMap.end()) {
        if (parasMap[order].size()>0) {
            int eNum = parasMap[order][0];
            return CCCommonUtils::getEffPM( CC_ITOA(eNum) );
        }
    }
    return "+";
}