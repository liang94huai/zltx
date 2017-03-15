//
//  IFAllianceRankCmd.cpp
//  IF
//
//  Created by wangdianzhen on 16/1/11.
//
//

#include "IFAllianceRankCmd.hpp"
#include "NetController.h"
#include "AllianceManager.h"

IFAllianceRankCmd::IFAllianceRankCmd(std::string infoStr)
: CommandBase(ALLIANCE_RANK_CMD),m_infoStr(infoStr)
{
    putParam("rankData", CCString::create(m_infoStr.c_str()));
}

bool IFAllianceRankCmd::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(ALLIANCE_RANK_CMD) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        if(pStr->compare("E000000")==0){
            //无效处理，不提示给玩家
        }else{
            CCCommonUtils::flyText(pStr->getCString());
        }
    }else{
        AllianceManager::getInstance()->initRankStateInfo(params);
        CCCommonUtils::flyHint("", "", _lang("115863"));
        callSuccess(NetResult::create());
    }
    return true;
}