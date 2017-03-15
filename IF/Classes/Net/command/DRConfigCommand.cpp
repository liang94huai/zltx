//
//  DRConfigCommand.cpp
//  IF
//
//  Created by Émilie.Jiang on 15/7/24.
//
//

#include "DRConfigCommand.h"
#include "DynamicResourceController.h"
//#include "LuaController.h"
bool DRConfigCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(DRConfigGet) != 0)
        return false;
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    //test code
    if(params->objectForKey("drconfig")){
        DynamicResourceController::getInstance()->initCommonInfo(params->objectForKey("drconfig"));
    }
    return true;
}

SendDeviceCollectCommand::SendDeviceCollectCommand():CommandBase(DeviceCollectSend){
//#if COCOS2D_DEBUG == 1
//    vector<string> processTmp;
//    cocos2d::extension::CCDevice::getProcessName(processTmp);
//    int idxPrint = 0 ;
//    auto iterPrint =  processTmp.begin();
//    string msg = "";
//    while (iterPrint!=processTmp.end()) {
//        CCLOG("process[%d]-[%s]",idxPrint,(*iterPrint).c_str());
//        msg = msg +  (*iterPrint).c_str() + string("|");
//        ++idxPrint;
//        ++iterPrint;
//    }
//    
//    msg = LuaController::getInstance()->encodeUrl(msg);
//    string url =  "https://p2cok.elexapp.com/debug.php?msg=" + msg;
//    GameController::getInstance()->goTurntoUrl(url);
//    CCLOG("processinfo: %s",url.c_str());
//    auto iterPrint1 =  GlobalData::shared()->phonestate.begin();
//    while (iterPrint1!=GlobalData::shared()->phonestate.end()) {
//        CCLOG("device[%s]-[%s]",(*iterPrint1).first.c_str(),(*iterPrint1).second.c_str());
//        ++iterPrint1;
//    }
//    idxPrint = 0 ;
//    vector<string> iplistTmp;
//    cocos2d::extension::CCDevice::getLocalIP(iplistTmp);
//    auto ipIter = iplistTmp.begin();
//    while (ipIter!=iplistTmp.end()) {
//        CCLOG("ip[%d]-[%s]",idxPrint,(*ipIter).c_str());
//        ++ipIter;
//        ++idxPrint;
//    }
//#endif
    m_hasParams=false;
    string info =  GlobalData::shared()->deviceCollect;
    //test code
//    info = "1;com.sec.android.app.twdvfs;com.android.contacts;com.android.123|2;net.bt.name=Android|2;net.bt.name=IOS|3";
    // info = "1;com.cyjh.gundam.service.ScriptService.p;com.cyjh.gundam;com.cyjh.gundam:download_server|3";
    
    if(info.empty() || info.compare("0")==0){
        return;
    }
    vector<string> cInfo;
    CCCommonUtils::splitString(info, "|", cInfo);
    CCArray *array2 = CCArray::create();
    auto iter = cInfo.begin();
    while (iter!=cInfo.end()) {
        if((*iter).find(":")!=string::npos){
            vector<string>  tmpV;
            CCCommonUtils::splitString((*iter), ";", tmpV);
            if(tmpV.size()>1){
                int type = atoi(tmpV.at(0).c_str());
                switch (type) {
                    case 1:  {// -- process
                        CCArray *array1 = CCArray::create();
                        vector<string> process;
                        cocos2d::extension::CCDevice::getProcessName(process);
                        auto pIter = tmpV.begin()+1;
                        while (pIter!=tmpV.end()) {
                            auto iter111 = std::find(process.begin(), process.end(), (*pIter));
                            if(iter111!=process.end()){
                                CCDictionary *ccDict = CCDictionary::create();
                                ccDict->setObject(CCInteger::create(1), (*pIter));
                                array1->addObject(ccDict);
                                CCLOG("process:[%s]-[%d]",(*pIter).c_str(),1);
                            }else{
                                CCLOG("process:[%s]-[%d]",(*pIter).c_str(),0);
                            }
                            ++pIter;
                        }
                        if(array1->count()>0){
                            m_hasParams=true;
                            this->putParam("1", array1);
                        }
                        break;
                    }
                    case 2:{ // -- devices
                        string value = tmpV.at(1);
                        auto pos1 = value.find_first_of("=");
                        string keyword = value.substr(0,pos1);
                        string keyvalue = value.substr(pos1+1);
                        auto iter00 = GlobalData::shared()->phonestate.find(keyword);
                        bool isFind = false;
                        if(iter00!=GlobalData::shared()->phonestate.end()){
                            if((*iter00).second.compare(keyvalue)==0){
                                isFind=true;
                            }
                        }
                        if(isFind==true){
                            CCDictionary* dict = CCDictionary::create();
                            dict->setObject(CCInteger::create(1), value);
                            array2->addObject(dict);
                            CCLOG("devices:[%s]-[%d]",value.c_str(),1);
                        }else{
                            CCLOG("devices:[%s]-[%d]",value.c_str(),0);
                        }
                        
                        break;
                    }
                    default:
                        break;
                }
            }
        }else{
            if((*iter).compare("3")==0){ // - ip
                CCArray *arrayIP = CCArray::create();
                vector<string> iplist;
                cocos2d::extension::CCDevice::getLocalIP(iplist);
                auto ipIter = iplist.begin();
                int ipidx = 0;
                while (ipIter!=iplist.end()) {
                    arrayIP->addObject(CCString::create((*ipIter)));
                    CCLOG("ip:[%d]-[%s]",ipidx,(*ipIter).c_str());
                    ++ipidx;
                    ++ipIter;
                }
                if(arrayIP->count()>0){
                    m_hasParams=true;
                    this->putParam("3", arrayIP);
                }
            }
        }
        ++iter;
    }
    if(array2->count()>0){
        m_hasParams=true;
        this->putParam("2", array2);
    }
}
SendDeviceCollectCommand::~SendDeviceCollectCommand(){
}
bool SendDeviceCollectCommand::getHasParams(){
    return m_hasParams;
}
bool SendDeviceCollectCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(DeviceCollectSend) != 0)
        return false;
    return true;
}
