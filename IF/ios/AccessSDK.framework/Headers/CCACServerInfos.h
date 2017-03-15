//
//  CCACServerInfos.hpp
//  Access
//
//  Created by Hoolai on 15/11/17.
//  Copyright © 2015年 wsj_hoolai. All rights reserved.
//

#ifndef CCACServerInfos_hpp
#define CCACServerInfos_hpp

#include <stdio.h>
#include <string>
#include <map>
#include <list>
#include "CCACServerInfo.h"

using namespace std;

class CCACServerInfos {

public:
    CCACServerInfos();
    ~CCACServerInfos();
    
public:
    list<CCACServerInfo> userServerList;
    list<CCACServerInfo> serverList;

};

#endif /* CCACServerInfos_hpp */
