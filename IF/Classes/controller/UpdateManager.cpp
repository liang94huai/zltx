//
//  UpdateManager.cpp
//  IF
//
//  Created by 邹 程 on 13-11-13.
//
//

#include "UpdateManager.h"
#include "GlobalData.h"
#include "LocalController.h"

#include <curl/easy.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "external/unzip/unzip.h"
#include "GameController.h"
#include "CCThreadManager.h"

using namespace cocos2d;
using namespace std;

#define KEY_OF_APP_VERSION           "current-app-version-code"
#define KEY_OF_VERSION               "current-version-code"
#define KEY_OF_DOWNLOADED_VERSION    "downloaded-version-code"
#define TEMP_PACKAGE_FILE_NAME       "tmp_update.zip"
#define BUFFER_SIZE    8192
#define MAX_FILENAME   512

// Message type
#define UpdateMANAGER_MESSAGE_UPDATE_SUCCEED                0
#define UpdateMANAGER_MESSAGE_RECORD_DOWNLOADED_VERSION     1
#define UpdateMANAGER_MESSAGE_PROGRESS                      2
#define UpdateMANAGER_MESSAGE_ERROR                         3
#define UpdateMANAGER_MESSAGE_OVER                          4

// TODO
#define XML_USE_LOCAL 0  //1 本地 0 线上

const std::string _xmlFileName = "database.local.xml";
const std::string _iniFileName = "text_";

// Some data struct for sending messages

struct ErrorMessage
{
    UpdateManager::UMErrorCode code;
    UpdateManager* manager;
};

struct ProgressMessage
{
    int percent;
    UpdateManager* manager;
};

// Implementation of UpdateManager

UpdateManager::UpdateManager()
: _storagePath("")
, _version("")
, _packageUrl("")
, _downloadedVersion("")
, _curl(NULL)
, _tid(NULL)
, _connectionTimeout(0)
, _delegate(NULL)
{
    _schedule = new Helper();
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(_schedule, 0, false);
    _storagePath = CCFileUtils::sharedFileUtils()->getWritablePath();
    string _hostIP = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_IP,"");
    if (_hostIP == "") {
        _hostIP = SERVER_IP;
    }
#if COCOS2D_DEBUG == 1
    _packageUrl = "http://"+_hostIP+":8080/gameservice/getfile?appVersion=" + GlobalData::shared()->version;
#else
    _packageUrl = "";//"http://cok.eleximg.com/cok/config/"+GlobalData::shared()->version;
#endif

    CCLOG("XMLURL = %s",_packageUrl.c_str());
}

UpdateManager::~UpdateManager()
{
    if (_schedule)
    {
        CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(_schedule);
        _schedule->release();
    }
}

bool UpdateManager::checkUpdate()
{
    if (XML_USE_LOCAL) {
        sendErrorMessage(kNoNewVersion);
        CCLOG("use local xml file");
        return false;
    }
    string _lang = LocalController::shared()->getLanguageFileName();
    string tmpIniName = _iniFileName + _lang + ".ini";
    CCLOG("iniFileName %s",tmpIniName.c_str());
    // check app version first
    string lastAppVersion = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_OF_APP_VERSION);
    string currentAppVersion = GlobalData::shared()->version;
    if (lastAppVersion.compare(currentAppVersion) != 0) {
        auto del = [](string store,string name){
            string path = store + name;
            if (CCFileUtils::sharedFileUtils()->isFileExist(path)) {
                remove(path.c_str());
            }
        };
        // app version not compatible
        del(_storagePath,_xmlFileName);
        del(_storagePath,tmpIniName);
        CCUserDefault::sharedUserDefault()->setStringForKey(KEY_OF_APP_VERSION, currentAppVersion);
    }
    
    // Clear _version before assign new value.
    _version.clear();
    
    // get version from global data
    _version = GlobalData::shared()->xmlVersion;
    if (_version.empty()) {
        _version = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_OF_VERSION);
    }
    
#if COCOS2D_DEBUG == 1
    _packageUrl = _packageUrl + "&xmlVersion=" + GlobalData::shared()->xmlVersion + "&xmlLang=" + _lang;
#else
    _packageUrl = _packageUrl + "/config_" + GlobalData::shared()->xmlVersion + "_" + _lang+".zip";
#endif

    CCLOG("XML URL %s",_packageUrl.c_str());
    string recordedVersion = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_OF_VERSION);
    if (recordedVersion == _version && CCFileUtils::sharedFileUtils()->isFileExist(_storagePath+tmpIniName))
    {
        sendErrorMessage(kNoNewVersion);
        CCLOG("there is no new version");
        return false;
    }
    
    // before update , delete
    
    CCLOG("there is a new version: %s", _version.c_str());
    
    return true;
}

void  UpdateManager::UpdateManagerDownloadAndUncompress(cocos2d::CCObject  *data)
{
    UpdateManager* self = (UpdateManager*)data;
    
    do
    {
        if (self->_downloadedVersion != self->_version)
        {
            if (! self->downLoad()) break;
            
            // Record downloaded version.
            UpdateManager::UMMessage *msg1 = new UpdateManager::UMMessage();
            msg1->what = UpdateMANAGER_MESSAGE_RECORD_DOWNLOADED_VERSION;
            msg1->obj = self;
            self->_schedule->sendMessage(msg1);
        }
        
        // Uncompress zip file.
        if (! self->uncompress())
        {
            self->sendErrorMessage(UpdateManager::kUncompress);
            break;
        }
        
        // Record updated version and remove downloaded zip file
        UpdateManager::UMMessage *msg2 = new UpdateManager::UMMessage();
        msg2->what = UpdateMANAGER_MESSAGE_UPDATE_SUCCEED;
        msg2->obj = self;
        self->_schedule->sendMessage(msg2);
    } while (0);
    //zym 2015.12.21
    /*if (self->_tid)
    {
        delete self->_tid;
        self->_tid = NULL;
    }*/
    
    UpdateManager::UMMessage *msg2 = new UpdateManager::UMMessage();
    msg2->what = UpdateMANAGER_MESSAGE_OVER;
    msg2->obj = self;
    self->_schedule->sendMessage(msg2);

    
}

void UpdateManager::update()
{
    if (_tid) {
        sendErrorMessage(kPthread);
        CCLOG("pthread error!");
        return;
    }
    
    // Check if there is a new version.
    if (! checkUpdate()) return;
    
    // Is package already downloaded?
    _downloadedVersion = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_OF_DOWNLOADED_VERSION);
    
    //_tid = new pthread_t();
    //pthread_create(&(*_tid), NULL, UpdateManagerDownloadAndUncompress, this);
    CCThreadManager::getInstance()->runInNetThread(this, callfuncO_selector(UpdateManager::UpdateManagerDownloadAndUncompress),this);
}

bool UpdateManager::uncompress()
{
    // Open the zip file
    string outFileName = _storagePath + TEMP_PACKAGE_FILE_NAME;
    
    bool ret = CCCommonUtils::unzipToDirectory(outFileName, _storagePath);
    return ret;
}

/*
 * Create a direcotry is platform depended.
 */
bool UpdateManager::createDirectory(const char *path)
{
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    
    return true;
}

static size_t downLoadPackage(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    FILE *fp = (FILE*)userdata;
    size_t written = fwrite(ptr, size, nmemb, fp);
    return written;
}

int UpdateManagerProgressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
    UpdateManager* manager = (UpdateManager*)ptr;
    UpdateManager::UMMessage *msg = new UpdateManager::UMMessage();
    msg->what = UpdateMANAGER_MESSAGE_PROGRESS;
    
    ProgressMessage *progressData = new ProgressMessage();
    auto percent = (int)(nowDownloaded/totalToDownload*100);
    percent = percent < 0 ? 0 : percent;
    progressData->percent = percent;
    progressData->manager = manager;
    msg->obj = progressData;
    
    manager->_schedule->sendMessage(msg);
    
//    CCLOG("downloading... %d%%", percent);
    
    return 0;
}

bool UpdateManager::downLoad()
{
    string outFileName = _storagePath + TEMP_PACKAGE_FILE_NAME;
    bool ret = CCCommonUtils::downloadFile(_packageUrl, outFileName);
    return ret;
}

string UpdateManager::getVersion()
{
    return CCUserDefault::sharedUserDefault()->getStringForKey(KEY_OF_VERSION);
}

void UpdateManager::deleteVersion()
{
    CCUserDefault::sharedUserDefault()->setStringForKey(KEY_OF_VERSION, "");
}

void UpdateManager::setDelegate(UpdateManagerDelegateProtocol *delegate)
{
    _delegate = delegate;
}

void UpdateManager::setConnectionTimeout(unsigned int timeout)
{
    _connectionTimeout = timeout;
}

unsigned int UpdateManager::getConnectionTimeout()
{
    return _connectionTimeout;
}

void UpdateManager::sendErrorMessage(UpdateManager::UMErrorCode code)
{
    UMMessage *msg = new UMMessage();
    msg->what = UpdateMANAGER_MESSAGE_ERROR;
    
    ErrorMessage *errorMessage = new ErrorMessage();
    errorMessage->code = code;
    errorMessage->manager = this;
    msg->obj = errorMessage;
    
    _schedule->sendMessage(msg);
}

// Implementation of UpdateManagerHelper

UpdateManager::Helper::Helper()
{
    _messageQueue = new list<UMMessage*>();
    pthread_mutex_init(&_messageQueueMutex, NULL);
}

UpdateManager::Helper::~Helper()
{
    pthread_mutex_destroy(&_messageQueueMutex);
    delete _messageQueue;
}

void UpdateManager::Helper::sendMessage(UMMessage *msg)
{
    pthread_mutex_lock(&_messageQueueMutex);
    _messageQueue->push_back(msg);
    pthread_mutex_unlock(&_messageQueueMutex);
}

void UpdateManager::Helper::update(float dt)
{
    UMMessage *msg = NULL;
    
    if (!_messageQueue) {
        return;
    }
    
    // Returns quickly if no message
    pthread_mutex_lock(&_messageQueueMutex);
    if (0 == _messageQueue->size())
    {
        pthread_mutex_unlock(&_messageQueueMutex);
        return;
    }
    
    // Gets message
    msg = *(_messageQueue->begin());
    _messageQueue->pop_front();
    pthread_mutex_unlock(&_messageQueueMutex);
    
    switch (msg->what) {
        case UpdateMANAGER_MESSAGE_UPDATE_SUCCEED:
            handleUpdateSucceed(msg);
            
            break;
        case UpdateMANAGER_MESSAGE_RECORD_DOWNLOADED_VERSION:
            CCUserDefault::sharedUserDefault()->setStringForKey(KEY_OF_DOWNLOADED_VERSION,
                                                                ((UpdateManager*)msg->obj)->_version.c_str());
            CCUserDefault::sharedUserDefault()->flush();
            
            break;
        case UpdateMANAGER_MESSAGE_PROGRESS:
            if (((ProgressMessage*)msg->obj)->manager->_delegate)
            {
                ((ProgressMessage*)msg->obj)->manager->_delegate->onProgress(((ProgressMessage*)msg->obj)->percent);
            }
            
            delete (ProgressMessage*)msg->obj;
            
            break;
        case UpdateMANAGER_MESSAGE_ERROR:
            if(((ErrorMessage*)msg->obj)->code == kUncompress){
                CCUserDefault::sharedUserDefault()->setStringForKey(KEY_OF_DOWNLOADED_VERSION, "");
                CCUserDefault::sharedUserDefault()->flush();
            }
            
            // error call back
            if (((ErrorMessage*)msg->obj)->manager->_delegate)
            {
                ((ErrorMessage*)msg->obj)->manager->_delegate->onError(((ErrorMessage*)msg->obj)->code);
            }
            
            delete ((ErrorMessage*)msg->obj);
            
            break;
        case UpdateMANAGER_MESSAGE_OVER:  // 删除的
            
            if( GameController::getInstance()->m_manager == msg->obj )
                CC_SAFE_DELETE(GameController::getInstance()->m_manager);
            else
                delete (UpdateManager*) msg->obj;
            
            break;
        default:
            break;
    }
    
    delete msg;
}

void UpdateManager::Helper::handleUpdateSucceed(UMMessage *msg)
{
    UpdateManager* manager = (UpdateManager*)msg->obj;
    
    // Record new version code.
    CCUserDefault::sharedUserDefault()->setStringForKey(KEY_OF_VERSION, manager->_version.c_str());
    
    // Unrecord downloaded version code.
    CCUserDefault::sharedUserDefault()->setStringForKey(KEY_OF_DOWNLOADED_VERSION, "");
    CCUserDefault::sharedUserDefault()->flush();
        
    // Delete unloaded zip file.
    string zipfileName = manager->_storagePath + TEMP_PACKAGE_FILE_NAME;
    if (remove(zipfileName.c_str()) != 0)
    {
        CCLOG("can not remove downloaded zip file %s", zipfileName.c_str());
    }
    
    if (manager && manager->_delegate) manager->_delegate->onSuccess();
}
