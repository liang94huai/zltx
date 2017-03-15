//
//  SettingObject.cpp
//  IF
//
//  Created by ganxiaohua on 15/12/2.
//
//

#include "SettingObject.hpp"

SettingObject::SettingObject(cocos2d::CCDictionary *dict){
    if (dict==NULL) {
        return ;
    }
    id = dict->valueForKey("id")->intValue();
    rank = dict->valueForKey("rank")->intValue();
    gm = dict->valueForKey("gm")->intValue();
    show = dict->valueForKey("show")->intValue();
    type = dict->valueForKey("type")->intValue();
    name = dict->valueForKey("name")->getCString();
}

void SettingObject::parse(cocos2d::CCDictionary *dict){
    if (dict==NULL) {
        return ;
    }
    if (dict->objectForKey("id")) {
        id = dict->valueForKey("id")->intValue();
    }
    if (dict->objectForKey("rank")) {
        rank = dict->valueForKey("rank")->intValue();
    }
    if (dict->objectForKey("gm")) {
        gm = dict->valueForKey("gm")->intValue();
    }
    if (dict->objectForKey("show")) {
        show = dict->valueForKey("show")->intValue();
    }
    if (dict->objectForKey("type")) {
        type = dict->valueForKey("type")->intValue();
    }
    if (dict->objectForKey("name")) {
        name = dict->valueForKey("name")->getCString();
    }
}