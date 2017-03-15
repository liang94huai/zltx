//
//  SNSUtil.h
//  IF
//
//  Created by 刘增友 on 15/9/14.
//
//

#ifndef __IF__ShareSDKUtil__
#define __IF__ShareSDKUtil__


#include "cocos2d.h"
#include "cocos-ext.h"

namespace ShareSDKUtil {
    
    void initShareSDKUtil(); // 初始化shareSDK
    void screenShare();   // 截屏分享
    void showShareActionSheet(const char* msg="", const char* url=""); // 分享菜单
};


#endif /* defined(__IF__ShareSDKUtil__) */
