//
//  QuestInfo.cpp
//  IF
//
//  Created by ganxiaohua on 13-11-4.
//
//

#include "QuestInfo.h"

void QuestInfo::parse(CCDictionary *dict){
    if(!dict){
        return;
    }
    if( dict->objectForKey("id") )
        itemId = dict->valueForKey("id")->getCString();
    else
        itemId = "";
    
    if( dict->objectForKey("guide") )
        guide = dict->valueForKey("guide")->getCString();
    else
        guide = "";
    
    if(!dict->objectForKey("id")){
        return;
    }
    std::string idStr = dict->valueForKey("id")->getCString();
    if(idStr == ""){
        return;
    }
    int idKey = dict->valueForKey("id")->intValue();
    if(idKey < 0){
        return;
    }
    if (dict->objectForKey("type")) {
        type = dict->valueForKey("type")->intValue();
    }else {
        type =0;
    }
    
    if (dict->objectForKey("go")) {
        go = dict->valueForKey("go")->intValue();
    }else {
        go = 0;
    }
    if (dict->objectForKey("gotype")) {
        gotype = dict->valueForKey("gotype")->intValue();
    } else {
        gotype = 0;
    }
    if (dict->objectForKey("type1")) {
        type1 = dict->valueForKey("type1")->intValue();
    } else {
        type1 = 0;
    }
    if (dict->objectForKey("order1")) {
        order1 = dict->valueForKey("order1")->intValue();
    } else {
        order1 = 0;
    }
    if (dict->objectForKey("order2")) {
        order2 = dict->valueForKey("order2")->intValue();
    } else {
        order2 = 0;
    }
    if (dict->objectForKey("icon1")) {
        icon1 = dict->valueForKey("icon1")->getCString();
    } else {
        icon1 = "";
    }
    if (dict->objectForKey("icon2")) {
        icon2 = dict->valueForKey("icon2")->getCString();
    } else {
        icon2 = "";
    }
    if (dict->objectForKey("rewardshow")) {
        rewardshow = dict->valueForKey("rewardshow")->getCString();
    } else {
        rewardshow = "";
    }
    if (dict->objectForKey("profile")) {
        isShow = dict->valueForKey("profile")->intValue();
    }else {
        isShow = 0;
    }

    
    classType = -1;
    if (dict->objectForKey("type2")) {
        classType = dict->valueForKey("type2")->intValue();
    }
    para1 = dict->valueForKey("para1")->intValue();
    
    newSortType = 0;
    if (dict->objectForKey("pos1")) {
        newSortType = dict->valueForKey("pos1")->intValue();
    }
    trainOrder = -1;
    if (dict->objectForKey("order_train")) {
        trainOrder = dict->valueForKey("order_train")->intValue();
    }
    
    if(icon1!=""){
        icon1+=".png";
    }
    if(icon2!=""){
        icon2+=".png";
    }
    maxtype = (idKey/100)%100;
    level = idKey%100;
    int descMax = 0;
    std::string para2 = "";
    
    if( dict->objectForKey("para2") )
        para2 = dict->valueForKey("para2")->getCString();
    
    if(para2 == ""){
        descMax = 999999;
    }else{
        maxValue = dict->valueForKey("para2")->intValue();
        int itemidnum = std::atoi(itemId.c_str());
        if(itemidnum==2302301){
            descMax = maxValue/60;
            maxValue =descMax;
        }else
        if(itemidnum>2302301&&itemidnum<=2302309){
            descMax = maxValue/3600;
            maxValue =descMax;
        }else{
            descMax = maxValue;
        }
        
    }
//    if(maxtype==1){
//        std::string para2 = dict->valueForKey("para2")->getCString();
//        if(para2 == ""){
//            return;
//        }
//        std::vector<std::string> itemArr;
//        CCCommonUtils::splitString(para2,"|",itemArr);
//        for (int i=0; i<itemArr.size(); i++) {
//            maxValue += atoi(itemArr[i].c_str());
//        }
//        descMax = atoi(itemArr[0].c_str());
//        CCLOG("para2=%s",para2.c_str());
//    }else if(maxtype==4){
//        maxValue = dict->valueForKey("para2")->intValue();
//        if(itemId=="2000400"){
//            maxValue = maxValue/60;
//        }else{
//            maxValue = maxValue/3600;
//        }
//        descMax = maxValue;
//    }else{
//        maxValue = dict->valueForKey("para2")->intValue();
//        descMax = maxValue;
//    }
    
    if( dict->objectForKey( "target") )
        description = _lang(dict->valueForKey("target")->getCString());
    else
        description = "";
    
    std::string dialog= "";
    if( dict->objectForKey( "name") )
       dialog= dict->valueForKey("name")->getCString();
    
    if(dialog!=""){
        name = _lang_1(dialog, CC_ITOA(descMax));
    }else{
        name = "";
    }
    
    if( dict->objectForKey( "Order") )
        order = dict->valueForKey("Order")->intValue();
    else
        order = 0;
    
    curValue = 0;
    haveOpen = false;
}

void QuestInfo::parseServerDic(CCDictionary *dict){
    if(dict==NULL) return;
    if(dict->objectForKey("state")){
        state = dict->valueForKey("state")->intValue();
    }
    if(dict->objectForKey("num")){
        curValue = dict->valueForKey("num")->intValue();
    }
    if(dict->objectForKey("currPhase")){
        currPhase = dict->valueForKey("currPhase")->intValue();
    }
    if(dict->objectForKey("maxPhase")){
        maxPhase = dict->valueForKey("maxPhase")->intValue();
    }
    if(dict->objectForKey("reward")){
        CCArray* arr = dynamic_cast<CCArray*>(dict->objectForKey("reward"));
        if (arr == nullptr) return;
        CCArray *tmpArr = CCArray::create();
        tmpArr->retain();
        int i = 0;
        while (i < arr->count()) {
            auto dic = dynamic_cast<CCDictionary*>(arr->objectAtIndex(i));
            if( dic == NULL || dic->valueForKey("type")  == NULL  )
            {
                ++i;
                continue;
            }
            int type = dic->valueForKey("type")->intValue();
//            int value = dic->valueForKey("value")->intValue();
            if(true){//value != 0
                if(type == R_EXP){
                    tmpArr->insertObject(dic, 0);
                }else if(type == R_POWER){
                    if(tmpArr->count() == 0){
                        tmpArr->insertObject(dic, 0);
                    }else{
                        auto dic1 = dynamic_cast<CCDictionary*>(tmpArr->objectAtIndex(0));
                        if( dic1 )
                        {
                            int type1 = dic1->valueForKey("type")->intValue();
                            if(type1 == R_EXP){
                                tmpArr->insertObject(dic, 1);
                            }else{
                                tmpArr->insertObject(dic, 0);
                            }
                        }
                    }
                }else{
                    tmpArr->addObject(dic);
                }
            }
            i++;
        }
        arr->removeAllObjects();
        this->reward = tmpArr;
    }
    int itemidnum = std::atoi(itemId.c_str());
    if(itemidnum==2302301){
        curValue = curValue/60;
    }else
    if(itemidnum>2302301&&itemidnum<=2302309){
        curValue = curValue/3600;
    }

}