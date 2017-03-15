//
//  IFScoutPush.hpp
//  IF
//
//  Created by wangdianzhen on 15/11/9.
//
//

#ifndef IFScoutPush_hpp
#define IFScoutPush_hpp

#include "cocos2d.h"
/// mail数据下发
class IFScoutPush {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};



#endif /* IFScoutPush_hpp */
