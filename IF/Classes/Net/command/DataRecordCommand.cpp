//
//  DataRecordCommand.cpp
//  IF
//
//  Created by 李锐奇 on 14-1-10.
//
//

#include "DataRecordCommand.h"
#include "PayController.h"

bool DataRecordCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(OPEN_PANEL_COMMAND) != 0)
        return false;
    return true;
}
bool PayRecordCommand::isSendRecord = false;
bool PayRecordCommand::handleRecieve(cocos2d::CCDictionary *dict){
    if (dict->valueForKey("cmd")->compare(PAY_RECORD_COMMAND) != 0)
        return false;
    if(m_fromView=="callPaymentToFriend"){
        isSendRecord=false;
    }else{
        if(PayController::getInstance()->getPayCheck()==false){
            isSendRecord=false;
        }else{
            if(GlobalData::shared()->isChinaPlatForm()||GlobalData::shared()->isGoogle() || GlobalData::shared()->analyticID == "amazon"){
                CCDictionary* params = _dict(dict->objectForKey("params"));
                const CCString *pStr = params->valueForKey("errorCode");
                if (pStr->compare("")!=0) {
                    CCCommonUtils::flyText((_lang(pStr->getCString()).c_str()));
                    return true;
                }
                if(params->objectForKey("canbuy")){
                    bool canBuy = params->valueForKey("canbuy")->boolValue();
                    if(canBuy){
                        PayController::getInstance()->callPaymentCheckback(m_gItem,m_fromView);
                    }else{
                        CCCommonUtils::flyHint("", "", _lang("101053"));
                        if(params->objectForKey("exchange")){
                            GlobalData::shared()->goldExchangeList.clear();
                            CCCommonUtils::initExchange(params);
                        }
                    }
                }
                isSendRecord=false;
            }else{
                isSendRecord=false;
            }
        }
    }
    return true;
}