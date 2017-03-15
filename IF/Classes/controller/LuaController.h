//
//  LuaController.h
//  IF
//
//  Created by Émilie.Jiang on 15/1/26.
//
//

#ifndef __IF__LuaController__
#define __IF__LuaController__

#include "CommonInclude.h"
#include "ActivityEventObj.h"
#include "GoldExchangeItem.h"
#include "WorldController.h"
#include "NewBaseTileInfo.h"

#define msg_superman_des  "superManDes"
struct ExchangeGiftStruct{
    std::string exchangeId;
    std::string receiverId;
    std::string uid;
    double sendTime;
};
enum LuaDownloadType{
    LuaDownloadType_NONE,
    LuaDownloadType_LUA,
    LuaDownloadType_RESOURCE
};
enum TRANSLATETYPE{
    TRANSLATE_MAIL,
    TRANSLATE_SUPERMAN,
};
class LuaDownLoadRef : public Ref{
public:
    CREATE_FUNC(LuaDownLoadRef);
    CC_SYNTHESIZE(string, mpackname, PackName);
    CC_SYNTHESIZE(string, mpackversion, PackVersion);
    CC_SYNTHESIZE(string, mcheckMD5, CheckMD5);
    CC_SYNTHESIZE(int, mSize, Size);
    CC_SYNTHESIZE(int, mZipSize, ZipSize);
    LuaDownloadType getPackType(){
        return mpacktype;
    }
    void setPackType(LuaDownloadType t){
        mpacktype = t;
    }
    void copy(LuaDownLoadRef *param){
        if(!param)
            return;
        setPackName(param->getPackName());
        setPackVersion(param->getPackVersion());
        setCheckMD5(param->getCheckMD5());
        setSize(param->getSize());
        setZipSize(param->getZipSize());
        setPackType(param->getPackType());
    }
    bool bPlatform;
protected:
    bool init(){
        return true;
    }
private:
    LuaDownloadType mpacktype;
};
class LuaController:public CCObject{
public:
    LuaController();    // for lua use
    ~LuaController();
    
    void purgeLua();
    static void purgeData();
    static LuaController* getInstance();
    static void doResourceByCommonIndex(int commonIndex,bool isLoad);
    static std::string getResourceNameByType(int itemid);
    static CCParticleSystemQuad* createParticleForLua(const char *name);
    static CCParticleSystemQuad* createParticleInPoolForLua(const char *name);
    static std::string getLang(std::string &msg);
    static std::string getLang1(std::string &msg, std::string &param);
    static std::string getLang2(std::string &msg, std::string &param1,std::string &param2);
    static std::string getLang3(std::string &msg,std::string &param1,std::string &param2,std::string &param3);
    static std::string getLang4(std::string &msg,std::string &param1,std::string &param2,std::string &param3,std::string &param4);
    static std::string getLang5(std::string &msg,std::string &param1,std::string &param2,std::string &param3,std::string &param4,std::string &param5);
    static std::string getDollarString(string &dollar,string &productId);
    static std::string getCMDLang(std::string &msg);
    static void addButtonLight(CCNode* node);
    static void addItemIcon(CCNode *node,std::string &itemid,Label *lable);
    static void addIconByType(CCNode *node,std::string &itemid,Label *lable,int type,int size);
    static int getWorldTime();
    static int getTimeStamp();
    static void playEffects(std::string name);
    static std::string getSECLang(int time);
    static void callPaymentToFriend(std::string &itemid,std::string touid,std::string toName);
    static void callPayment(std::string &itemid);
    static void removeAllPopup();
    static void removeLastPopup();
    static void showDetailPopup(std::string &itemid);
    static bool checkSkeletonFile(const char* fileName);
    static void addSkeletonAnimation(CCNode* node,const char* skeletonDataFile, const char* atlasFile,const char* animation,float scale);
    static int getStringLength(std::string &str);
    static std::string getUTF8SubString(std::string &str,int start,int end);
    static void toSelectUser(std::string itemid,bool removeAllPop,int backTo);
    static void setTitleName(std::string _name);
    static std::string getPlatform();
    static std::string getLanguage();
    CCNode* getSaleViewContainer();
    CCNode* getAdvCellContainer();
    CCNode* getAdvCell();
    CCNode* getIconContainer();
    
    //hd
    static bool isIosAndroidPad();
    
    void sendCMD(std::string cmdName,CCDictionary *dict = NULL);
    void backCMD(std::string cmdName,CCDictionary *dict);
    static void showFAQ(std::string faq);
    void notificationToLua(std::string actID,Ref* ret);
    // for c++ use
    void showExchangeSale(CCNode *node,GoldExchangeItem* item);
    CCRect createExchangeAdvCell(CCNode *node,std::string &info);
    void updateExchangeAdvCell(CCNode *node,std::string &info);
    void createExchangeIcon(CCNode *node,std::string &info);
    bool checkLuaValid(std::string popImg,std::string md5,std::string type);
    bool checkNewAdvLuaValid(std::string popImg,std::string md5,std::string type);
    bool checkActLuaValid(ActivityEventObj* obj);
    bool showNoramlIcon(std::string popImg);
    bool checkADV(string popImg);
    friend void* LuaFileDownloadAndUncompress(void*);
    void downloadFinish(LuaDownLoadRef *downRef);
    void unzipFinish(LuaDownLoadRef *downRef);
    void changeUI(float t = 0.0);
//    bool checkStorageEnough(double checkSize);
//    void beginCheckStorage(float t = 0.0);
    
    //活动中心
    CCNode* createActivityView(ActivityEventObj* obj);
    CCNode* createActivityRwdView(ActivityEventObj* obj);
    CCNode* createActivityAdCell(ActivityEventObj* obj);
    CCNode* createActivityListCellSprite(ActivityEventObj* obj);
    void getActExcData();
    void showActRwdViewByActId(std::string actId);
    static CCSprite* createGoodsIcon(std::string& itemId, int size);
    static void removeAllPopupView();
    static void openBagView(int type);
    static CCDictionary* getGoodsInfo(std::string& itemId);
    static std::string getObjectType(CCObject* obj);
    static std::string retReward(CCArray*);
    static void flyHint(std::string icon, std::string titleText, std::string context);
    static void showTipsView(std::string, CCTextAlignment align = kCCTextAlignmentCenter);
    
    std::string getActTips(std::string actId);
    std::string getCollectLabel(std::string actId);
    std::string getBuyBtnLabel(std::string actId);
    std::string getActRefTip(std::string actId);
    
    //触发帐号绑定引导
    static void showBindGuide();
    //google translate
    void setTranslateMark();
    
    void translate(std::string orgContent,std::string targeLan, int linkidx = 0 ,TRANSLATETYPE type = TRANSLATE_MAIL);
    void onTranslateBack(std::string backStr);
    static size_t onTranslate(uint8_t* ptr,size_t size,size_t number,void *stream);

    //special translate for superman

    void onTranslateBack_superMan(std::string backStr);
    static size_t onTranslate_superMan(uint8_t* ptr,size_t size,size_t number,void *stream);
    //载入资源
    void loadResourceByNameAndPath(std::string name,std::string path, bool release = false);
    
    
    string luaVersion;//动态更新活动的版本号
    string luaMD5Code;//动态更新活动zip md5校验码
    int luaSize;//lua解压后文件大小
    int luazipSize; //lua zip包大小
//    string luapackMD5Code;
    
//
// Chat Notice
//
    void addChatNotice(CCDictionary *dict);
    void closeChatNotice();
    CCNode* getChatNoticeContainer();
    void showSceneNotice(bool b);
    void changeChatNotice(CCObject *ccObj = NULL);
    void addSystemUpdate(double countDown, bool isLogin,string tip,FlyHintType type,string icon);
    void ChatNoticeInView(CCObject *ccObj = NULL);
    void ChatNoticeInSceen(CCObject *ccObj = NULL);
    void clearChatNotice();
    
    //礼包赠送
    void updateExchangeGiftInfo(std::string toUID,std::string itemid, double sendTime);
    void initExchangeGiftInfo(CCArray *arr);
    bool canSendExchange(std::string &itemid,std::string uid,std::string toName,int type = 0,bool showTip = false);
    void backToExchange(int backTo);
    bool canTranslate();
    std::string encodeUrl(const std::string& szToEncode);
    
    bool unZipFile(LuaDownLoadRef *downRef);
    std::string m_writeablePath;
//    pthread_t *m_tid1;//unzip thread
    std::string getLuaPath();
    
    
    void parseConfig(CCDictionary *dict);
    //for lua activity  -- add by js
//    void clickActButton(int type);   -- not open yet
    void showLuaViewByActivity(string actID);
    void initActivityInfo(CCArray *array);
    bool checkValidActivity(string resName,string newmd5);
    LuaActivityInfo* getActivityInfo(string actID);    
    void sendNotification(string msg);
    //end
    
    //for lua world   -- add by js
    void setWorldConfig(CCArray *luaArr);
    void addWorldUnderNode(CCPoint pos, unsigned int index,WorldCityInfo *cinfo,CCArray * array);
//    void addWorldBatchItem(CCNode *pNode, CCPoint pos, int type, unsigned int index, WorldCityInfo *cinfo,CCArray * array,CCObject *ccObj = NULL);
    NewBaseTileLuaInfo* createTileView(WorldCityInfo *cinfo);
    void releaseWorldRes();
    //end
    
    //for lua Imperial --xxr
    void onImperialEnter();
    Node* getImperialTouchLayer();
    bool isInTutorial();
    int getFileVersion(string name);
    Dictionary* getDicLuaData(Ref*);
    Array* getArrLuaData(Ref*);
    //end
    
    void resetData();
    
    int checkLoginDataState(string name);
    void saveLoginData(string name, CCDictionary* dict);
    void openJoinAllianceView();
protected:
    void downLoadZip(CCObject* ccObj);
    void uncompressZip(CCObject* ccObj);
    bool isDownloading(std::string packname);
    bool isUncompressing(std::string packname);
    
private:
    void addLuaPath();
//    std::string getLuaPath();
    std::string getLuaFile();
    CCNode* m_saleViewContainer;
    CCNode* m_advCellContainer;
    CCNode* m_advCell;
    CCNode* m_iconContainer;
    bool m_isAddPath;
    
    vector<string> mDownQuene;
    vector<string> mUnzipQuene;
    void removeFromDown(string packname);
    void removeFromUnzip(string packname);
    
    std::string m_clientVersion;
    std::string m_serverVersion;
    std::string m_downedVersion;
    void resetVersionAndUpdate();
    void updateNewResource(string packName,string md5,bool bPlatform=false);
    bool checkUpdate();
    void startDownload(LuaDownLoadRef *downRef);
    
    bool downloadFile(LuaDownLoadRef *downRef);
    bool createDirectory(const char *path);
//    pthread_t *m_tid;
    
    CURL *m_curl;
    
//    std::string m_packagePath;
    bool isLuaOpen();
    
    bool initLua();
    bool m_initLuaEngine;
    std::string m_initVersion;
    
//    LuaDownloadType m_downloadType;
//    std::string m_packName;
    void replaceNewLuaFile();
    void replaceNewLuaFile1(int v);
    bool checkLuaFiles();
    void saveDownloadVersion(CCObject *ccObj);
    void saveClientVersion(CCObject *ccObj);
    void saveClientVersion1(CCObject *ccObj);
    bool initMD5Info();
    int getVersionData(std::string filePath);
    std::map<std::string,std::string> m_md5Map;
    
    //unzip
    void startUnzipFile(LuaDownLoadRef *downRef);
    
    //chat notice
    CCNode *mChatNoticeNode;
    void createChatNotice(CCDictionary *ccDict);
    std::vector<CCDictionary*> mChatNoticeV;
    std::vector<ExchangeGiftStruct> mExGiftList;
    
    //google translate
    std::string m_orgMsg;
    bool m_canTranslate;

    unordered_map<string,LuaActivityInfo*> mLuaActMap;
    
    bool checkLib(string libName,string newMD5);
    std::map<std::string,vector<std::string>> m_libMap;
//    std::map<std::string,std::string> m_waitToDownAct;
    vector<std::string> m_notificationList;
    unsigned char* getXXTeaDecode(string filepath,uint32_t &len);
    std::map<std::string,std::string> m_worldconfig;
    map<string, CCDictionary*> LoginInitDataMap;//临时寄存登录的数据
};


#endif /* defined(__IF__LuaController__) */
