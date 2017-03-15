//
//  CCAccessSDK.hpp
//  Access
//
//  Created by Hoolai on 15/11/17.
//  Copyright © 2015年 wsj_hoolai. All rights reserved.
//

#ifndef CCAccessSDK_hpp
#define CCAccessSDK_hpp

#include "CCAccessSDKCallback.h"
#include "CCACReturnValue.h"
#include "CCACServerInfo.h"
#include "CCACServerInfos.h"
#include "CCACUserLoginResponse.h"
#include "CCACUserExtDataKeys.h"
#include "CCACPushMessage.h"
#include <map>
#include <string>

using namespace std;

class CCAccessSDK {

public:
    CCAccessSDKCallback* sdkCallback;
    
private:
    CCAccessSDK();
    ~CCAccessSDK();

public:
    
    static CCAccessSDK* sharedInstance();
    
    void initSDK(CCAccessSDKCallback* platformCallback);
    
    void initSDK();
    
    void initSDK(bool auto_login);
    
    void checkUpdate(string version);
    
    void login();
    
    void login(string customParam);
    
    void logout();
    
    void accountManager();
    
    void pay(string itemName, int amount, string callbackInfo);
    
    void switchAccount();
    
    void exitGame(string msg);
    
    void getServerList(string version);
    
    void selectServer(string serverId);
    
    void setUserExtData(map<string, string> userExtData);
    
    void enterBBS();
    
    void pushMsg(CCACPushMessage msg);
    
    void sendBIData(string metric, string jsonData);
    
    string channel();
    
    string biChannel();
};

#endif /* CCAccessSDK_hpp */
