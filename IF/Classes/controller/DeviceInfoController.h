//
//  DeviceInfoController.h
//  IF
//
//  Created by 王国强 on 15/11/11.
//
//

#ifndef __IF__DeviceInfoController__
#define __IF__DeviceInfoController__

#include "CommonInclude.h"

class DeviceInfoController:public CCObject{
public:
    static DeviceInfoController *getInstance();
    DeviceInfoController();
    ~DeviceInfoController();
    
    void getBatteryStates(float fupdate);
    void startBatteryStateCollect();
    void waitToSendCheckCmd(float waitTime = 120.0f);
    void sendCheckCmd(float t);
    void collectProcess(string info);
    
};

#endif /* defined(__IF__DeviceInfoController__) */
