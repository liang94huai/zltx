//
//  PrincessTaskPush.hpp
//  IF
//
//  Created by 王超一 on 16/1/14.
//
//

#ifndef PrincessTaskPush_hpp
#define PrincessTaskPush_hpp

#include "cocos2d.h"
#include "CommonInclude.h"

class PrincessTaskComplete {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class PrincessTaskNew {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

#endif /* PrincessTaskPush_hpp */
