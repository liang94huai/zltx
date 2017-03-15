//
//  HoolaiBISdkUtils.hpp
//  zltx
//
//  Created by tao.yu on 2017/3/8.
//
//

#ifndef HoolaiBISdkUtils_hpp
#define HoolaiBISdkUtils_hpp

#include <stdio.h>
#include "CommonInclude.h"

namespace HoolaiBISdkUtils {
    
    const char* getNetStatus();
    std::string makeBiJson(CCDictionary* arr);
    void sendInstall(CCDictionary* params);
    void sendDau(CCDictionary* params);
    void sendPayment(CCDictionary* params);
    void sendEconomy(CCDictionary* params);
    
}
#endif /* HoolaiBISdkUtils_hpp */
