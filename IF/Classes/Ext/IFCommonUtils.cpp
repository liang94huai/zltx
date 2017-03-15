//
//  IFCommonUtils.cpp
//  IF
//
//  Created by wangdianzhen on 16/1/13.
//
//

#include "IFCommonUtils.hpp"
bool IFCommonUtils::shouldForceChangePic(std::string picStr){
    if(GlobalData::shared()->newIcon_country.find(CCCommonUtils::getLanguage()) == string::npos){
        if(CCCommonUtils::isNewIcon(picStr)){
            return true;
        }
    }
    return false;
}