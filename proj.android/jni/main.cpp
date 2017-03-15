#include "AppDelegate.h"
#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
//#include "platform/android/CCFileUtilsAndroid.h"
#include <jni.h>
#include <dlfcn.h>
#include <android/log.h>
#include "cocos2d.h"
#include "src/client/linux/handler/exception_handler.h"
#include "src/client/linux/handler/minidump_descriptor.h"
#include "external/curl/include/android/curl/curl.h"
#include "CCDevice.h"
//#include "android/asset_manager.h"
//#include "android/asset_manager_jni.h"
//#include <jni/JniHelper.h>

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace cocos2d;

extern "C"
{

size_t read_file(void* buff, size_t size, size_t nmemb, void* userp)
{
    size_t sizes = fread(buff, size, nmemb, (FILE *)userp);
    return sizes;
}

int SendFile(const char* file_name, bool bCreate)
{
	LOGD("崩溃包开始传了");
	if( bCreate )
	{
		FILE* w_file = fopen("/sdcard/dump.txt", "w");
		if (0 == w_file)
		{
			 LOGD("fopen create失败 " );
			return -1;
		}
		fwrite(file_name, strlen(file_name), 1, w_file);
		fclose(w_file); /*关闭文件*/
	}
	long  response_code = 0;
	CURLcode res;   //easy_handle定义的一些错误码
	 //初始化libcurl
	 res = curl_global_init(CURL_GLOBAL_ALL);
	 if (res != CURLE_OK )
	 {
		 LOGD("崩溃包传失败");
		string str;
		 str = curl_easy_strerror(res);
		 LOGD("curl_global_init失败%s",str.c_str());



	 	//return false;
	 }
	 CURL *curl = curl_easy_init();

	LOGD("崩溃包开始传了1");
	if (!curl) {
		LOGD("curl_easy_init失败原因 ");

		return false;
	}

	  CURLcode err_code = CURLE_OK;
	  //CURLcode (*curl_easy_setopt)(CURL *, CURLoption, ...);
	  //*(void**) (&curl_easy_setopt) = dlsym(curl_lib, "curl_easy_setopt");
	  curl_easy_setopt(curl, CURLOPT_URL, "http://public.cok.elexapp.com/client/crash/upload");
	 // (*curl_easy_setopt)(curl, CURLOPT_USERAGENT, kUserAgent);
	  // Set proxy information if necessary.
	 // if (!proxy.empty())
	 //   (*curl_easy_setopt)(curl, CURLOPT_PROXY, proxy.c_str());
	  //if (!proxy_user_pwd.empty())
	   // (*curl_easy_setopt)(curl, CURLOPT_PROXYUSERPWD, proxy_user_pwd.c_str());

	 // if (!ca_certificate_file.empty())
	  //  (*curl_easy_setopt)(curl, CURLOPT_CAINFO, ca_certificate_file.c_str());

	  struct curl_httppost *formpost = NULL;
	  struct curl_httppost *lastptr = NULL;
	  // Add form data.
//	  CURLFORMcode (*curl_formadd)(struct curl_httppost **, struct curl_httppost **, ...);
//	  *(void**) (&curl_formadd) = dlsym(curl_lib, "curl_formadd");
//	  map<string, string>::const_iterator iter = parameters.begin();
//	  for (; iter != parameters.end(); ++iter)
//	    (*curl_formadd)(&formpost, &lastptr,
//	                 CURLFORM_COPYNAME, iter->first.c_str(),
//	                 CURLFORM_COPYCONTENTS, iter->second.c_str(),
//	                 CURLFORM_END);

	  // Add form file.
	  //curl_formadd(&formpost,
	  //	    	                &lastptr,
	  //	    	                 CURLFORM_COPYNAME, "fileName",
	  	//    	                 CURLFORM_COPYCONTENTS, "logfile",
	  	//    	                 CURLFORM_END);
	  curl_formadd(&formpost, &lastptr,
	               CURLFORM_COPYNAME, "fileName",//file_part_name.c_str(),
	               CURLFORM_FILE, file_name,//upload_file.c_str(),
	               CURLFORM_END);
	  LOGD("崩溃包开始传了2");
	  curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
	  curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
	  // Disable 100-continue header.
	  struct curl_slist *headerlist = NULL;
	  char buf[] = "Expect:";
	 // struct curl_slist* (*curl_slist_append)(struct curl_slist *, const char *);
	  //*(void**) (&curl_slist_append) = dlsym(curl_lib, "curl_slist_append");
	  headerlist = curl_slist_append(headerlist, buf);
	  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

//	  if (response_body != NULL) {
//	    (*curl_easy_setopt)(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
//	    (*curl_easy_setopt)(curl, CURLOPT_WRITEDATA,
//	                     reinterpret_cast<void *>(response_body));
//	  }
	  LOGD("崩溃包开始传了3");
	  // Fail if 400+ is returned from the web server.
	  curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);

	 // CURLcode (*curl_easy_perform)(CURL *);
	  //*(void**) (&curl_easy_perform) = dlsym(curl_lib, "curl_easy_perform");
	  err_code = curl_easy_perform(curl);
//	  if (response_code != NULL) {
//	    CURLcode (*curl_easy_getinfo)(CURL *, CURLINFO, ...);
//	    *(void**) (&curl_easy_getinfo) = dlsym(curl_lib, "curl_easy_getinfo");
//	    (*curl_easy_getinfo)(curl, CURLINFO_RESPONSE_CODE, response_code);
//	  } 4+7
	  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
	  //const char* (*curl_easy_strerror)(CURLcode);
	  //*(void**) (&curl_easy_strerror) = dlsym(curl_lib, "curl_easy_strerror");
	//#ifndef NDEBUG
	if (err_code != CURLE_OK)
	{
	   string str;
	   str = curl_easy_strerror(err_code);
	   LOGD("失败原因:%s,%d, %d",str.c_str(), err_code, response_code);

	}
	else
	{
		remove(file_name);
		remove("/sdcard/dump.txt");
		LOGD("好了");
	}
	   //fprintf(stderr, "Failed to send http request to %s, error: %s\n",
	   //         url.c_str(),
	     //       (*curl_easy_strerror)(err_code));
	//#endif
	 // if (error_description != NULL)
	  //LOGD(curl_easy_strerror(err_code));

	  //void (*curl_easy_cleanup)(CURL *);
	  //*(void**) (&curl_easy_cleanup) = dlsym(curl_lib, "curl_easy_cleanup");
	  curl_easy_cleanup(curl);
	  curl_global_cleanup();
//	  if (formpost != NULL) {
//	    void (*curl_formfree)(struct curl_httppost *);
//	    *(void**) (&curl_formfree) = dlsym(curl_lib, "curl_formfree");
//	    (*curl_formfree)(formpost);
//	  }
}

int SendCrashFile( )
{
	FILE* w_file = fopen("/sdcard/dump.txt", "r");
	if (0 == w_file)
	{
		// LOGD("SendCrashFile fopen 失败 " );
		return -1;
	}
	char file_name[512] = {0};

	fread(file_name, sizeof(file_name)-1, 1, w_file);

	fclose(w_file); /*关闭文件*/
	//LOGD("崩溃文件是%s",file_name);

	SendFile(file_name, false);
}

int UpLoadFile(const char* file_name)
{
	 CURLcode res;   //easy_handle定义的一些错误码
	  struct HttpPost *formpost=0;
	  struct HttpPost *lastptr=0;
	    //初始化libcurl
	    res = curl_global_init(CURL_GLOBAL_ALL);
	    if (res != CURLE_OK)
	    {
	       // std::cerr << "init libcurl failed." << std::endl;
	        return -1;
	    }

	    //获取要上传的文件指针
	    FILE* r_file = fopen(file_name, "rb");
	    if (0 == r_file)
	    {
	       // std::cerr << "the file /" " << file_name << " /"that you will read is not exist" << std::endl;
	        return -1;
	    }
	    curl_formadd((curl_httppost**)&formpost,
	    	                 (curl_httppost**)&lastptr,
	    	                 CURLFORM_COPYNAME, "filename",
	    	                 CURLFORM_COPYCONTENTS, "logfile",
	    	                 CURLFORM_END);

	    curl_formadd((curl_httppost**)&formpost,
	                 (curl_httppost**)&lastptr,
	                 CURLFORM_COPYNAME, "filename",
	                 CURLFORM_COPYCONTENTS, "test.dmp",
	                 CURLFORM_END);

	    CURL* easy_handle;

	    easy_handle = curl_easy_init();
	    if (0 == easy_handle)
	    {
	      //  std::cout << "get a easy_handle handle fail!" << std::endl;
	        fclose(r_file);
	        curl_global_cleanup();
	        curl_easy_cleanup(easy_handle);
	        return -1;
	    }

	    // 获取文件大小
	    fseek(r_file, 0, 2);
	    int file_size = ftell(r_file);
	    rewind(r_file);

	    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://184.173.110.107:8086/client/crash/upload");        //获取URL地址
	    curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1);    //告诉easy_handle是做上传操作
	    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, &read_file);    //调用重写的读文件流函数
	    curl_easy_setopt(easy_handle, CURLOPT_READDATA, r_file);    //往read_file()函数中传入用户自定义的数据类型
	    curl_easy_setopt(easy_handle, CURLOPT_INFILE, r_file);      //定位作为上传的输入文件
	    curl_easy_setopt(easy_handle, CURLOPT_INFILESIZE, file_size);   //上传的字节数
	    curl_easy_setopt(easy_handle, CURLOPT_HTTPPOST, formpost);
	    //执行设置好的操作
	    res = curl_easy_perform(easy_handle);

	    //获取HTTP错误码
	    int HTTP_flag = 0;
	    curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE , &HTTP_flag);

	    //一般清理动作
	    fclose(r_file);
	    curl_global_cleanup();
	    curl_easy_cleanup(easy_handle);

	     // "操作本地文件:" << file_name << "; url:" << url << ".";
	    //检测HTTP错误码和执行操作结果
	    if (  (CURLE_OK != res))
	    {
	        //std::cout << "失败!" << std::endl;
	    	LOGD( "失败!%d, %d", HTTP_flag, res);
	        return -1;
	    }
	    else
	    {
	    	LOGD( "成功!");
	        //std::cout << "成功!" << std::endl;
	        return 0;
	    }
}

bool DumpCallback(const google_breakpad::MinidumpDescriptor& descriptor,
                  void* context,
                  bool succeeded) {
//	LOGD("dumpdumpdump\n");
	//LOGD("dumpdumpdump%s\n", descriptor.path());
 // printf("Dump path: %s\n", descriptor.path());
	SendFile(descriptor.path(), true);
	//崩溃堆栈log上传后，再上传崩溃logcat
    JniMethodInfo minfo;
    if (! JniHelper::getStaticMethodInfo(minfo, "org/hcg/IF/Jni", "saveCrashPID", "()V") ) {
        CCLOG("JNI: Jni method saveCrashPID not found!");
        return succeeded;
    }
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);

  return succeeded;
}
static google_breakpad::ExceptionHandler * pException = NULL;

JNIEXPORT void JNICALL Java_org_cocos2dx_ext_Native_nativeBreakpadDestroy()
{
	LOGD( "Java_org_cocos2dx_ext_Native_nativeBreakpadDestroy" );
	  volatile int* a = reinterpret_cast<volatile int*>(NULL);
	   *a = 1;
		int b = 0;
		//int m = 3/b;

		string keyStr = "";
		 auto iterb = keyStr.begin();
		        while (b<3) {
		        	*iterb = '2';

		            ++iterb;
		        }
		        LOGD( "Java_org_cocos2dx_ext_Native_nativeBreakpadDestroy1%d",2 );
}

JNIEXPORT void JNICALL Java_org_cocos2dx_ext_Native_nativeBreakpadDumpInit(JNIEnv*  env, jobject thiz , jstring text) {
    //string app = env->GetStringUTFChars(text, NULL);
	SendCrashFile();
    string app = JniHelper::jstring2string(text);
    LOGD( "breakpadinit:%s", app.c_str() );
    LOGD( "Java_org_cocos2dx_ext_nativeBreakpadDumpInit" );
	 string _appVersion = cocos2d::extension::CCDevice::getVersionName();
	 string _appCode = cocos2d::extension::CCDevice::getVersionCode();
	 google_breakpad::MinidumpDescriptor descriptor("/sdcard",  app+_appVersion+"-"+_appCode);
	 pException = new google_breakpad::ExceptionHandler(descriptor, NULL, DumpCallback,
	                                       NULL, true, -1);

}
}

//extern "C"
//{
//
//jint JNI_OnLoad(JavaVM *vm, void *reserved)
//{
//    JniHelper::setJavaVM(vm);
//
//    return JNI_VERSION_1_4;
//}
//
//
//void Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit(JNIEnv*  env, jobject thiz, jint w, jint h)
//{
//
//    JniMethodInfo methodInfo;
//    if (! JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/lib/Cocos2dxHelper","getAssetManager", "()Landroid/content/res/AssetManager;"))
//    {
//        methodInfo.env->DeleteLocalRef(methodInfo.classID);
//        return;
//    }
//    jobject assetManager = methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
//    methodInfo.env->DeleteLocalRef(methodInfo.classID);
//    AAssetManager* mgr = AAssetManager_fromJava(methodInfo.env, assetManager);
//    CCFileUtilsAndroid::setassetmanager(mgr);
//
//    if (!CCDirector::sharedDirector()->getOpenGLView())
//    {
//        CCEGLView *view = CCEGLView::sharedOpenGLView();
//        view->setFrameSize(w, h);
//
//        AppDelegate *pAppDelegate = new AppDelegate();
//        CCApplication::sharedApplication()->run();
//    }
//    else
//    {
//        ccGLInvalidateStateCache();
//        CCShaderCache::sharedShaderCache()->reloadDefaultShaders();
//        CCTextureCache::reloadAllTextures();
//        ccDrawInit();
//        //CCTextureCache::reloadAllTextures();
//        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_COME_TO_FOREGROUND, NULL);
//        CCDirector::sharedDirector()->setGLDefaultValues();
//    }
//}
//
//
//}

	//int m = 0;
	//m = 1/0;

void cocos_android_app_init (JNIEnv* env) {
    LOGD("cocos_android_app_init");
    AppDelegate *pAppDelegate = new AppDelegate();
}


