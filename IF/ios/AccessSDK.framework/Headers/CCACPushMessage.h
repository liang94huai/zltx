//
//  CCACPushMessage.hpp
//  accesssdk
//
//  Created by Hoolai on 15/12/11.
//  Copyright © 2015年 wsj_hoolai. All rights reserved.
//

#ifndef CCACPushMessage_hpp
#define CCACPushMessage_hpp

#include <stdio.h>
#include <map>
#include <string>

using namespace std;

class CCACPushMessage {
    
public:
    long long fireDate;
    string alertBody;
    int badge;
    map<string, string> userInfo;
    string alertAction;
    
public:
    CCACPushMessage();
    ~CCACPushMessage();
};

#endif /* CCACPushMessage_hpp */
