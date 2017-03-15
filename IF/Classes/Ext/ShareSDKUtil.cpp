//
//  SNSUtil.cpp
//  IF
//
//  Created by 刘增友 on 15/9/14.
//
//

#include "ShareSDKUtil.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#   include <jni.h>
#   include "platform/android/jni/JniHelper.h"

void ShareSDKUtil::screenShare(){};   // 截屏分享
void ShareSDKUtil::showShareActionSheet(const char* msg, const char* url)
{
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/hcg/IF/sharesdk/ShareSDKUtil", "showShareSDK", "(Ljava/lang/String;Ljava/lang/String;)V") ) {
        CCLOG("JNI: method loginPlatform not found!");
        return;
    }
    jstring _msg = minfo.env->NewStringUTF(msg);
    jstring _url = minfo.env->NewStringUTF(url);
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, _msg, _url);
    minfo.env->DeleteLocalRef(_msg);
    minfo.env->DeleteLocalRef(_url);
    minfo.env->DeleteLocalRef(minfo.classID);
}


#endif