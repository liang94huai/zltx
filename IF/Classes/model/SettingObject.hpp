//
//  SettingObject.hpp
//  IF
//
//  Created by ganxiaohua on 15/12/2.
//
//

#ifndef SettingObject_hpp
#define SettingObject_hpp

#include "cocos2d.h"

class SettingObject : public cocos2d::CCObject{
public:
    SettingObject(cocos2d::CCDictionary *dict);
    SettingObject():id(0),show(1),gm(0),type(-1),rank(9999),name(std::string()){};
    void parse(cocos2d::CCDictionary *dict);
    int id;
    int show;
    int type;
    int rank;
    int gm;
    std::string name;
};
#endif /* SettingObject_hpp */
