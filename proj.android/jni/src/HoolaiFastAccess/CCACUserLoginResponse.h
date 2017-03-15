//
//  CCACUserLoginResponse.hpp
//  Access
//
//  Created by Hoolai on 15/11/17.
//  Copyright © 2015年 wsj_hoolai. All rights reserved.
//

#ifndef CCACUserLoginResponse_hpp
#define CCACUserLoginResponse_hpp

#include <stdio.h>
#include "CCACServerInfo.h"
#include <ctime>
#include <string>
#include <map>

using namespace std;

class CCACUserLoginResponse {
public:
    long long uid;
    string accessToken;
    string nickName;
    string channelUid;
    string channel;
    int productId;
    
public:
    virtual~CCACUserLoginResponse(){}
};

#endif /* CCACUserLoginResponse_hpp */
