//
//  DeviceInfoController.cpp
//  IF
//
//  Created by 王国强 on 15/11/11.
//
//

#include "DeviceInfoController.h"
#include "CheckEmulatorCommand.h"
#include "../Ext/CCDevice.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"

#endif

static DeviceInfoController *_instance = NULL;
DeviceInfoController *DeviceInfoController::getInstance(){
    if(_instance == NULL){
        _instance = new DeviceInfoController();
    }
    return _instance;
}

DeviceInfoController::DeviceInfoController() {
    
}

DeviceInfoController::~DeviceInfoController() {
    
}

void DeviceInfoController::waitToSendCheckCmd(float waitTime){
    
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(DeviceInfoController::sendCheckCmd), this);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(DeviceInfoController::sendCheckCmd), this, 0, 0, waitTime,false);
}

void DeviceInfoController::sendCheckCmd(float t){
    
    std::string qemuBkpt = CCUserDefault::sharedUserDefault()->getStringForKey("qemuBkpt", "");
    
    std::string collect_bas = CCUserDefault::sharedUserDefault()->getStringForKey("collect_bas", "");
    CCCommonUtils::splitString(collect_bas, "|", GlobalData::shared()->batteryStates);
    
    std::string signCode = CCUserDefault::sharedUserDefault()->getStringForKey("theSignCode", "");
    
    std::string collectProcess = CCUserDefault::sharedUserDefault()->getStringForKey("cpr_data", "");
    
    
    CheckEmulatorCommand *cmd = new CheckEmulatorCommand(qemuBkpt,collect_bas,signCode,collectProcess);
    cmd->sendAndRelease();
    
}


void DeviceInfoController::startBatteryStateCollect(){
    
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(DeviceInfoController::getBatteryStates), this);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(DeviceInfoController::getBatteryStates), this, 300.0f, -1, 0,false);
}

void DeviceInfoController::collectProcess(string info){
    
    vector<string> process;
    cocos2d::extension::CCDevice::getProcessName(process);
    
    vector<string>  tmpV;
    CCCommonUtils::splitString((info), ";", tmpV);
    
    string cprStr = "";
    string tmpS = "";
    
    
    for (int i=0; i<tmpV.size(); i++) {
        tmpS = tmpV[i];
        auto iter111 = std::find(process.begin(), process.end(), tmpS);
        if(iter111 != process.end()){
            cprStr.append(tmpS.c_str());
            if (i != tmpV.size() - 1) {
                cprStr.append(";");
            }
        }
        
    }
    
    CCUserDefault::sharedUserDefault()->setStringForKey("cpr_data", cprStr.c_str());
    
    
}



void DeviceInfoController::getBatteryStates(float fupdate){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string battery = getBatteryState();
    vector<string> bs;
    CCCommonUtils::splitString(battery, "|", bs);
    
    std::string batteryState = "";
    if (bs.size() > 0) {
        for (int i=0; i<bs.size(); i++) {
            string it = bs[i];
            vector<string> bss;
            CCCommonUtils::splitString(it, ":", bss);
            std::string str = bss[1].c_str();
            if (str.compare("true") == 0) {
                str = "1";
            }
            if (str.compare("false") == 0) {
                str = "0";
            }
            batteryState = batteryState + str;
            if(i < bs.size()-1)
                batteryState.append("-");
            
        }
    }
//    CCLog("batteryState::::::%s",batteryState.c_str());
    
    vector<string> gbs = GlobalData::shared()->batteryStates;
//    CCLog("gbs.size.....start::%d",gbs.size());
    while (gbs.size() >= 5) {
        gbs.erase(gbs.begin());
//        CCLog("gbs.size.....while::%d",gbs.size());
    }
//    CCLog("gbs.size.....whileend::%d",gbs.size());
    gbs.push_back(batteryState);
//    CCLog("gbs.size.....end::%d",gbs.size());
    GlobalData::shared()->batteryStates = gbs;
    
    std::string ret = "";
    for (int i=0; i<gbs.size(); i++) {
        std::string it = gbs[i];
        ret = ret + it;
        if(i < gbs.size()-1)
            ret.append("|");
    }
    CCUserDefault::sharedUserDefault()->setStringForKey("collect_bas", ret);
//    CCLog("collect_bas::%s",ret.c_str());
#endif
    
}


