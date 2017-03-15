//
//  CCACServerInfo.hpp
//  Access
//
//  Created by Hoolai on 15/11/17.
//  Copyright © 2015年 wsj_hoolai. All rights reserved.
//

#ifndef CCACServerInfo_hpp
#define CCACServerInfo_hpp

#include <stdio.h>
#include <string>
#include <map>

using namespace std;

class CCACServerInfo {
    
public:
    CCACServerInfo(){}
    ~CCACServerInfo(){}
    
public:
    string serverId;
    string serverName;
    string serverAddress;
    int productId;
    string status;
    int order;
    int hot;
    string description;
    bool purchaseOpen;
    string purchaseNotifyUri;
};

#endif /* CCACServerInfo_hpp */
