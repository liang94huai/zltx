//
//  CCCommonUtils.h
//  IF
//
//  Created by 邹 程 on 13-9-16.
//
//

#ifndef __IF__CCCommonUtils__
#define __IF__CCCommonUtils__

#include <string>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "EquipInfo.h"
#include "GeneralInfo.h"
#include "CCFloatingText.h"
#include "GlobalData.h"
#include "cocosextV3.h"
#include "CCIFTouchNode.h"

#define _dict(_obj) CCCommonUtils::castDict((_obj))

#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

#define GOODS_ICON_TAG 99
#define GOODS_BG_TAG 100
#define GOODS_NUM_BG_TAG 101
#define GOODS_NUM_LBL_TAG 102
#define SERVER_SWITCH_HD "SERVER_SWITCH_HD"
#define SERVER_SWITCH_HD_2 "SERVER_SWITCH_HD_2"
#define DOWNLOAD_HD_CCB "DOWNLOAD_HD_CCB"
#define DOWNLOAD_HD_RES "DOWNLOAD_HD_RES"

#define UI_SKIN_TYPE_LAST_TIME "UI_SKIN_TYPE_LAST_TIME"
#define UI_SKIN_TYPE "UI_SKIN_TYPE"
#define UI_SKIN_RES_DOWNLOADED "UI_SKIN_RES_DOWNLOADED"


//订单信息存放的key
#define COK_PURCHASE_KEY "COKPurchaseInfoList"
//已成功订单号存放的key
#define COK_PURCHASE_SUCCESSED_KEY  "COK_PURCHASE_SUCCESSED_KEY"

#define IS_OPEN_PING_HELPSHIFT 0

#define IS_RECORD_LOD 0//是否记录日志;

namespace CCCommonUtils {
    enum InitOperations{
        InitStart,//起始标志，其他的要放到这个下面
        InitOpenUUID,//uuid更新标识，其他放在这个下面
        InitTestSwitches,
        InitTime,
        InitEffect,
        InitTranslation,
        InitGaid,
        InitEffectState,
        InitStatus,
        InitConfigData,
        InitPlayerInfo,
        InitWorld,
        InitPray,
        InitLord,
        InitPushSetting,
        InitBusinessMan,
        InitMonthLyCards,
        InitRecharge,
        InitMails,
        InitTrain,
        InitResource,
        InitEnemyInfo,
        InitAffair,
        InitWall,
        InitActivity,
        InitQueue,
        InitArmy,
        InitBuilding,
        InitDragonBuilding,
        InitDragonsInfo,
        InitAppLibHelper,
        InitGeneral,
        InitAllianceSceice,
        InitFort,
        InitTreat,
        InitExchange,
        InitEquip,
        InitKnight,
        InitItems,
        InitScience,
        InitTalent,
        InitGCMReward,
        InitTask,
        InitServerStopTime,
        InitMixInfo,
        InitFBShare,
        initparticle,
        initspine,
        DoWhenInitComplete,
        GetGameDataAfterInit,
        InitNewPlayerReward,
        InitCheckEmulatorData,
        InitSendHeart,
        InitAssets,//跨服胜点对象数据
        InitImperialSceneBegin,
        InitImperialSceneEnd = InitImperialSceneBegin + 16,
        InitEnd,//结束标志，其他的要放到这个上面
    };
//    std::string to_string(int d);
//    std::string to_string(float d);
//    std::string to_string(double d);
    std::string timeStampToUTCDate(time_t &t);
    std::string timeStampToUTCHSM(time_t &t);
    std::string timeStampToDate(time_t &t);
    std::string timeLeftToCountDown(int time,const char* separator = ":");
    
    std::string timeStampToDHM(time_t &t);
    std::string timeStampToHour(time_t &t);
    std::string timeStampToMD(time_t &t);
    std::string timeStampToYMD(time_t &t);
    std::string timeStampToHMS(time_t &t);
    
    long UTCDataToTimeStamp(const char* timeStamp);
    void addNewDizi(Node* sprite, int iType = 0);// 0 默认的, 1 小图
    
    void setButtonTitle(CCControlButton* button,const char* str);
    void setButtonTitleColor(CCControlButton *button, ccColor3B color);
    void setButtonSprite(CCControlButton* button,const char* str);
    void setButtonTitlePoint(CCControlButton* button,const CCPoint point);
    bool FileToVector(std::string const& strFileName, std::vector<std::string>& vLines);
    bool FileToMap(std::string const& strFileName, std::map<std::string, std::string>& kv);
    bool FileToMD5(std::string const& strFileName, std::string& strMD5);
    bool SaveMapToFile(std::map<std::string, std::string>& kv, std::string const& strFileName);
    CCArray* split(const char* src, const char* sym);
    void splitString(const std::string &strSrc, const std::string &strFind, std::vector<std::string> &arSplit);
    std::string replaceString(const std::string &str,const std::string &src,const std::string &des);
    void flyText(std::string, cocos2d::ccColor3B color=cocos2d::ccRED, float time = 0.5);
    void flyHint(std::string icon, std::string titleText, std::string contentText, float time = 3, float dy=0, bool isMid=false, float proY=0, std::string mailId="");
    void flyHintWithDefault(std::string icon, std::string titleText, std::string contentText,float time = 3,float dy=0);
    void flySystemUpdateHint(double countDown, bool isLogin,string tip,FlyHintType type,std::string icon="");
    void flyTextOnParent(std::string, cocos2d::CCNode *parent,cocos2d::CCPoint pos,cocos2d::ccColor3B color=cocos2d::ccRED);
    void flyUiResText(std::string, cocos2d::CCNode *parent,cocos2d::CCPoint pos,cocos2d::ccColor3B color=cocos2d::ccRED, FloatingTypeEnum floatType=floating_type_normal, int fontSize=20);
    void createGoodsIcon(int toolID, CCNode* mParent,CCSize defSize,CCLabelIF* mNum = NULL, CCLabelIF* mName = NULL, CCLabelIF* mDes =NULL,float op =255.0);
    std::string getBonusString(cocos2d::CCArray* rewards);
    //上传头像
    void onUploadPhoto(string uid, int srcCode, int idx);
    //heibai
    CCSprite *createGraySprite(const char* imageName);
    //武将头像
    std::string getGeneralPicById(std::string generalId);
    //武将半身像
    std::string getGeneralBustPicById(std::string generalId);
    //名字
    std::string getNameById(std::string xmlId);
    //获得xml配置相对应的属性
    std::string getPropById(std::string xmlId, std::string propName);
    std::string getParamByGroupAndKey(std::string groupId,std::string baseKey,std::string baseInfo, std::string desKey);
    std::string getQueueIconByType(int queueType);
    std::string getQueueNameByType(int queueType);
    int getQueueSortByType(int queueType);
    
    void clearGameCache();  // 3Dtouch使用清理缓存
    
    std::string getResourceIconByType(int resourceType);
    std::string getResourceNameByType(int resourceType);
    std::string getResourceProductByType(int resourceType);
    std::string getResourceIconByTypeNew(int resourceType);
    
    bool isEnoughResourceByType(int resourceType, int num);
    int getCurResourceByType(int resourceType);
    bool isPushNotify(int type);
    //获得兵种图标
    std::string getArmIconByType(int armType);
    //物品背景图片
    std::string getToolBgByColor(int color);
    //物品背景图片
    std::string getBoxBgByColor(int color);

    //英雄头像背景图片
    std::string getGenHeadBgByColor(int color);
    
    void fitContainerSizeAndAddChild(cocos2d::CCNode *container,cocos2d::CCNode *child,int type = 0);

    //更改Sprite变灰
    void setSpriteGray(CCSprite *sprite, bool gray);
    
    // 高斯模糊
    void setSpriteBlur(Sprite* sprite, const Vec2& blurSize);
    
    // 高斯膨胀
    void setSpriteDiffuse(cocos2d::Sprite *sprite, float outlineSize, cocos2d::Color3B outlineColor, cocos2d::Size textureSize, cocos2d::Color3B foregroundColor = cocos2d::Color3B::WHITE);
    //检查武将状态
    bool checkGeneralState(int _state);
    //icon字段读取
    std::string getIcon(std::string);
    //是否是装备物品
    bool isEquip(int itemid);
    //获得物品颜色
    cocos2d::ccColor3B getItemColor(int color);
    //获得颜色名称
    string getColorName(int color);
    //
    bool isLackOfGold(int i);
    //获得地形天赋字符串
    std::string getTalentStr(std::string talent, int type);
    //获得地形加成字符串
    std::string getTacticsStr(std::string talent, int type);
    //获得物品洗练属性字符串
    std::string getItemPropStr(EquipInfo* info, std::string att, int value);
    //获得碎片合成所需碎片数
    int getSoulExChangeNeedByColor(int color);
    //获得武将技能
    std::string getGeneralSkill(std::string generalId);
    //监测布阵格是否开放
    bool isFormationOpen(int index);
    //返回布阵已开几个格
    int getFormationOpenNum();
    //返回下次开布阵位的等级
    int getNextFormationOpenLevel();
    //根据建筑类型获得队列类型
    int getQueueTypeByBuildType(int buildType);
    
    void onNetworkAvailable(CCObject* p);
    
    //根据用户头像名称获取加载哪个大图
    int getBustIdxByName(string name);
    //localFN 本地取系统的语言的简称 如en zh，通过这个方法，取得对应的语言的全称，如 zh 通过多语言等，取得 简体中文
    std::string getLanguageFNByLocalSN(std::string localSN);
    
    //获取作用号
    float getEffectValueByNum(int num, bool isInludeGeneral = true);
    float getStateEffectValueByNum(int num);
    float getVipEffectValueByNum(int num);
    float getOfficerValueByNum(int num);
    float getTerritoryEffectValueByNum(int num);
    float getTerritoryNegativeEffectValueByNum(int num);
    string getEffFormat(string num);
    string getEffPM(string num);
    //功能按钮是否开启
    bool isBtnOpen(int btnIndex);
    //记录打点
    void recordStep(std::string index);
//    void recordStepByHttp(std::string index);
    
    void postEventLogToServer(const std::string& paramlog);
    void setLoadingStartTime();
    void finishLoadingTotalTime();
    
    //获得祭祀
    int getNormalPray();
    int getIronPray();
    
    //获得cd时间所花费的金币
    int getGoldByTime(int t);
    //获得某种资源所花费的金币
    int getResGlodByType(int type, int num);
    //获得购买物品的金币
    int getGoldBuyItem(std::string itemId);
    //设置图片最大宽度
    CCSprite* setSpriteMaxSize(CCSprite* spr, int limitNum, bool isForce=false);
    CCSprite* getRomanSprite(int num);
    //变灰色
    CCSprite* graylightWithCCSprite(CCSprite* oldSprite,bool isLight);
    
    //是否是测试版本
    bool isTestPlatformAndServer(string key="");
    
    //由dataconfig传递过来的开关判断
    bool isOpenFormDataCfg(string key);
    
    //获取国家图标名称
    string getCountryIconByName(string banner);
    
    //获取士兵等级 根据int
    string getChineseLevelTxtByNum(int nlevel);
    

    //是否有资源采集道具加成
    bool hasSpeedEffect(int type);
    double getSpeedEffectTime(int type);
    
    CCPoint getPointByString(std::string string,std::string separator=";");
    
    void getRGBAProtocols(CCNode* parent, vector<CCNode*> * proVec);
    
    std::string changeTimeAddUnit(int secs);
    std::string subStrByUtf8(const string &str,int index,int len);
    
    std::string getLanguage();
    //获取多语言的type与系统字段的对应关系
    std::string getMoreLanguageType(string sysTemKey);
    //生成序列帧动画
    CCAnimate* makeEffectAnimate(CCSprite* spr, string path, int num, float dt, int idx);
    void makeEffectSpr(CCSprite* spr, string path, int num, float dt=0.07, int idx=0);
    void makeOnceEffectSpr(CCSprite* spr, string path, int num, float delate=0, float dt=0.07);
    void makeTwoAnimatSpr(CCSprite* spr, string path, int num, int splitNum, float dt=0.07, int idx=0);
    void makeInternalEffectSpr(CCSprite* spr, string path, int num, float dt=0.07, int idx=0,float delaytime=0);
    void makeEffectSprWithAP(CCSprite* spr, const Vec2 &anchorPos, string path, int num, float dt=0.07, int idx=0);
    //判断账号是否绑定
    bool IsBandOfAcount();
    
    // safe cast dict
    CCDictionary* castDict(CCObject* obj);
    
    CCPoint getCrossPoint(vector<CCPoint> &vector, CCPoint &c, CCPoint &d);
    std::vector<CCPoint>& getCrossPoints(vector<CCPoint> &vector, CCPoint &c, CCPoint &d, int& nSize);
    CCSprite *addFilterSprite(std::string fileName, const ccColor3B &color, float bright = 0.0f, float contrast = 1.0f, float saturation = 1.0f);
    ccColor3B covertHSBToRGB(int h, float s, float v);
    CCSprite *getItemBGByColor(int color);
    void changeAllianceFlagBGByRank(CCSprite *sprite, int index, bool isGray);
    string getResourceStr(int resource);
    int getResourceLoadByType(int type);
    bool isServerCrossOpen();
    bool isAdriodPad();
    bool isAdriodPadHD();
    bool isIOSPad();
    bool isIosAndroidPad();
    bool isPad();
    void setIsUseHD(bool isHD);
    void setServerHDSwitch();
    bool getIsUseHD();
    void setIsHDViewPort(bool isHD);
    bool getIsHDViewPort();
    void addResSearchPath();
    void parseData(cocos2d::CCDictionary *params);
    //主UI节日期间换皮
    void setCurUISkinType();
    bool isUseSpeUI();
    int getUISkinType();
    //以下为初始化数据调用的函数，因为解析时间过长，分段解析
    void resetData(CCDictionary *params);//数据重置
    void refreshOpenUUID(CCDictionary *params);
    void initConfigData(CCDictionary *params);//config数据初始化
    void initTime(CCDictionary *params);//初始化时间
    void initEffect(CCDictionary *params);
    void initTranslation(CCDictionary *params);
    void initGaid(CCDictionary *params);
    void initEffectState(CCDictionary *params);
    void initStatus(CCDictionary *params);
    void initWorld(CCDictionary *params);
    void initPray(CCDictionary *params);
    void initLord(CCDictionary *params);
    void initPushSetting(CCDictionary *params);
    void initBusinessMan(CCDictionary *params);
    void initMonthLyCards(CCDictionary *params);
    void initFreshRecharge(__Dictionary* params);
    void initPlayerInfo(CCDictionary *params);
    void initMails(CCDictionary *params);
    void initTrain(CCDictionary *params);
    void initResource(CCDictionary *params);
    void initEnemyInfo(CCDictionary *params);
    void initAffair(CCDictionary *params);
    void initWall(CCDictionary *params);
    void initActivity(CCDictionary *params);
    void initQueue(CCDictionary *params);
    void initBuilding(CCDictionary *params);
    void initQijiBuilding(CCDictionary *params);
    void initDragonBuilding(CCDictionary *params);//初始化龙建筑信息
    void initDragonsInfo(CCDictionary *params);//初始化龙信息
    void initAppLibHelper(CCDictionary *params);
    void initGeneral(CCDictionary *params);
    void initAllianceSceice(CCDictionary *params);
    void initArmy(CCDictionary *params);
    void initFort(CCDictionary *params);
    void initTreat(CCDictionary *params);
    void initExchange(CCDictionary *params);
    void initEquip(CCDictionary *params);
    void initKnightTitle(CCDictionary *params);
    void initItems(CCDictionary *params);
    void initScience(CCDictionary *params);
    void initTalent(CCDictionary *params);
    void initGCMReward(CCDictionary *params);
    void initNewPlayerReward(CCDictionary *params);
    void initTask(CCDictionary *params);
    void initServerStopTime(CCDictionary *params);
    void initMixInfo(CCDictionary *params);
    void initFBShare(CCDictionary *params);
    void doWhenInitComplete(CCDictionary *params);
    void getGameDataAfterInit();
    void initOperation(int initType, CCDictionary *params);
    void initOperationOver();
    void initTestSwitches(CCDictionary *params);
    void initAssets(CCDictionary *params);
    void initSendHeart(Dictionary* params);
    void initCheckEmulatorData(CCDictionary *params);
    void initImperialSceneTexture(CCDictionary *params, int index);
    bool checkTaiWanFlag();
    std::string changeChinaFlag(string flag);//对大陆国旗的处理显示
    std::string changeHKToChinaFlag(string flag);//对HK旗的处理显示
    bool isShowFlagForChinese();
    bool isInSimulator();
    bool checkVersion(string version);//检测开关 当前版本大于等于version 返回true
    bool checkServer(string serverID);//检测开关 当前版serverID 是否开
    bool canOpenNewIcon();
    bool isNewIcon(string iconName);
    //字符串替换函数
    std::string& StringReplace(std::string& strBig, const std::string& strsrc, const std::string& strdst);
    bool useCaptcha();
    int getBindType(std::string bindStr);
    bool payParseData(cocos2d::CCDictionary *params);
    CCArray* getTypeArray(int type);
    string getCustomPicUrl(string uid, int picVer);
    string getCustomPicUrl1(const char * uid, int picVer);
    string getShakePicUrl(string picName);
    bool isUseCustomPic(int picVer);
    void showHelpShiftFAQ();
    void showDynamicForm();
    void sendHelpshiftLog(const char* IP, const char* uid, const char* tag);
    void showHelpShiftFAQ(string itemId);
    void showBanHelpShift();
    void showEmulatorBanHelpShift();
    void showHelpShiftSingleFAQ(string itemId);
    bool isChina();
    bool isFirstJoinAlliance();
    string getDragonBuildingNameByType(int type);
    string getDragonBuildingInfoByType(int type);
	std::string getVersionName();
    float getDragonValueByNum(int num);
    bool isUserNewUid();
    void sendConnectTimeToServer(int type, double time);
    double getLocalTime();
    bool isKuaFu();
    bool isKuaFuWangZhan();//跨服打王战
    // 同步下载url文件
    bool downloadFile(std::string const& url, std::string const& localfile);
    bool createDirectory(const char *path);
    // 将zip包解压缩到某个目录（目录要存在）
    bool unzipToDirectory(std::string const& strZip, std::string const& strDir, bool error_clear = false, bool s = false);
    void onAddFire(CCNode * parentNode);
    void localRecordLog(const char * format, ...);

    class ButtonNode: public CCIFTouchNode    {
    public:
        
        static ButtonNode* create(cocos2d::CCSize touchSize) {
            auto ret = new ButtonNode();
            if (ret && ret->init(touchSize)) {
                ret->autorelease();
            } else {
                CC_SAFE_DELETE(ret);
            }
            return ret;
        }
        
        bool init(cocos2d::CCSize touchSize) {
            bool ret = false;
            if (cocos2d::CCNode::init()) {
                m_touchSize = touchSize;
                ret = true;
            }
            return ret;
        }
        
        void setSwallow(bool isSwallow);
        void setPriority(int priority);
        
        bool isTouchInNode(cocos2d::CCTouch *pTouch);
        
        virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
        virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
        virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
        virtual void onTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
        
        virtual void onEnter();
        virtual void onExit();
    private:
        
        ButtonNode():
        m_touchSize(cocos2d::CCSizeZero)
        ,m_swallow(false)
        ,m_priority(1)
        ,m_touchBegan(NULL)
        ,m_touchMoved(NULL)
        ,m_touchEnded(NULL)
        ,m_touchCancelled(NULL)
        ,m_touchDoBegin(NULL)
        ,m_touchDoMove(NULL)
        ,m_touchDoEnd(NULL)
        ,m_touchDoCancel(NULL)
        {}
        
        void resetTouch();

        CC_SYNTHESIZE(cocos2d::CCSize, m_touchSize,TouchSize);
        CC_SYNTHESIZE_READONLY(bool, m_swallow,Swallow);
        CC_SYNTHESIZE_READONLY(int, m_priority,Priority);
        CC_SYNTHESIZE(std::function<bool(ButtonNode* that,cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)>, m_touchBegan,TouchBegan);
        CC_SYNTHESIZE(std::function<void(ButtonNode* that,cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)>, m_touchMoved,TouchMoved);
        CC_SYNTHESIZE(std::function<void(ButtonNode* that,cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)>, m_touchEnded,TouchEnded);
        CC_SYNTHESIZE(std::function<void(ButtonNode* that,cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)>, m_touchCancelled,TouchCancelled);
        CC_SYNTHESIZE(std::function<void(bool isTouchInNode)>, m_touchDoBegin,TouchDoBegin);
        CC_SYNTHESIZE(std::function<void(bool isTouchInNode)>, m_touchDoMove,TouchDoMove);
        CC_SYNTHESIZE(std::function<void(bool isTouchInNode)>, m_touchDoEnd,TouchDoEnd);
        CC_SYNTHESIZE(std::function<void(bool isTouchInNode)>, m_touchDoCancel,TouchDoCancel);
    };
    
};


class CCCommonUtilsForLua{
public:
    static std::string timeStampToUTCDate(time_t &t){
        return CCCommonUtils::timeStampToUTCDate(t);
    }
    static std::string timeStampToUTCHSM(time_t &t){
        return CCCommonUtils::timeStampToUTCHSM(t);
    }
    static std::string timeStampToDate(time_t &t){
        return CCCommonUtils::timeStampToDate(t);
    }
    static std::string timeLeftToCountDown(int time,const char* separator = ":"){
        return CCCommonUtils::timeLeftToCountDown(time,separator);
    }
    static std::string timeStampToDHM(time_t &t){
        return CCCommonUtils::timeStampToDHM(t);
    }
    static std::string timeStampToHour(time_t &t){
        return CCCommonUtils::timeStampToHour(t);
    }
    static std::string timeStampToMD(time_t &t){
        return CCCommonUtils::timeStampToMD(t);
    }
    static std::string timeStampToYMD(time_t &t){
        return CCCommonUtils::timeStampToYMD(t);
    }
    static std::string timeStampToHMS(time_t &t){
        return CCCommonUtils::timeStampToHMS(t);
    }
    static long UTCDataToTimeStamp(const char* timeStamp){
        return CCCommonUtils::UTCDataToTimeStamp(timeStamp);
    }
    static void setButtonTitle(CCControlButton* button,const char* str){
        CCCommonUtils::setButtonTitle(button,str);
    }
    static void setButtonTitleColor(CCControlButton *button, ccColor3B color){
        CCCommonUtils::setButtonTitleColor(button,color);
    }
    static void setButtonSprite(CCControlButton* button,const char* str){
        CCCommonUtils::setButtonSprite(button,str);
    }
    static void setButtonTitlePoint(CCControlButton* button,const CCPoint point){
        CCCommonUtils::setButtonTitlePoint(button,point);
    }
    static CCArray* split(const char* src, const char* sym){
        return CCCommonUtils::split(src,sym);
    }
    static std::string replaceString(const std::string &str,const std::string &src,const std::string &des){
        return CCCommonUtils::replaceString(str,src,des);
    }
    static void flyText(std::string str, cocos2d::ccColor3B color=cocos2d::ccRED, float time = 0.5){
        CCCommonUtils::flyText(str,color,time);
    }
    static void flyHint(std::string icon, std::string titleText, std::string contentText, float time = 3, float dy=0, bool isMid=false, float proY=0, std::string mailId=""){
        CCCommonUtils::flyHint(icon,titleText,contentText,time,dy,isMid,proY,mailId);
    }
    static void flyHintWithDefault(std::string icon, std::string titleText, std::string contentText,float time = 3,float dy=0){
        CCCommonUtils::flyHintWithDefault(icon,titleText,contentText,time,dy);
    }
    static void flySystemUpdateHint(double countDown, bool isLogin,string tip,FlyHintType type,std::string icon=""){
        CCCommonUtils::flySystemUpdateHint(countDown,isLogin,tip,type,icon);
    }
    static void flyTextOnParent(std::string text, cocos2d::CCNode *parent,cocos2d::CCPoint pos,cocos2d::ccColor3B color=cocos2d::ccRED){
        CCCommonUtils::flyTextOnParent(text,parent,pos,color);
    }
    static void flyUiResText(std::string text, cocos2d::CCNode *parent,cocos2d::CCPoint pos,cocos2d::ccColor3B color=cocos2d::ccRED, FloatingTypeEnum floatType=floating_type_normal, int fontSize=20){
        CCCommonUtils::flyUiResText(text,parent,pos,color,floatType,fontSize);
    }
    static void createGoodsIcon(int toolID, CCNode* mParent,CCSize defSize,CCLabelIF* mNum = NULL, CCLabelIF* mName = NULL, CCLabelIF* mDes =NULL,float op =255.0){
        CCCommonUtils::createGoodsIcon(toolID,mParent,defSize,mNum,mName,mDes,op);
    }
    static std::string getBonusString(cocos2d::CCArray* rewards){
        return CCCommonUtils::getBonusString(rewards);
    }
    static void onUploadPhoto(string uid, int srcCode, int idx){
        CCCommonUtils::onUploadPhoto(uid,srcCode,idx);
    }
    static CCSprite *createGraySprite(const char* imageName){
        return CCCommonUtils::createGraySprite(imageName);
    }
    static std::string getGeneralPicById(std::string generalId){
        return CCCommonUtils::getGeneralPicById(generalId);
    }
    static std::string getGeneralBustPicById(std::string generalId){
        return CCCommonUtils::getGeneralBustPicById(generalId);
    }
    static std::string getNameById(std::string xmlId){
        return CCCommonUtils::getNameById(xmlId);
    }
    static std::string getPropById(std::string xmlId, std::string propName){
        return CCCommonUtils::getPropById(xmlId,propName);
    }
    static std::string getParamByGroupAndKey(std::string groupId,std::string baseKey,std::string baseInfo, std::string desKey){
        return CCCommonUtils::getParamByGroupAndKey(groupId,baseKey,baseInfo,desKey);
    }
    static std::string getQueueIconByType(int queueType){
        return CCCommonUtils::getQueueIconByType(queueType);
    }
    static std::string getQueueNameByType(int queueType){
        return CCCommonUtils::getQueueNameByType(queueType);
    }
    static int getQueueSortByType(int queueType){
        return CCCommonUtils::getQueueSortByType(queueType);
    }
    static std::string getResourceIconByType(int resourceType){
        return CCCommonUtils::getResourceIconByType(resourceType);
    }
    static std::string getResourceNameByType(int resourceType){
        return CCCommonUtils::getResourceNameByType(resourceType);
    }
    static std::string getResourceProductByType(int resourceType){
        return CCCommonUtils::getResourceProductByType(resourceType);
    }
    static bool isEnoughResourceByType(int resourceType, int num){
        return CCCommonUtils::isEnoughResourceByType(resourceType,num);
    }
    static int getCurResourceByType(int resourceType){
        return CCCommonUtils::getCurResourceByType(resourceType);
    }
    static bool isPushNotify(int type){
        return CCCommonUtils::isPushNotify(type);
    }
    static std::string getArmIconByType(int armType){
        return CCCommonUtils::getArmIconByType(armType);
    }
    static std::string getToolBgByColor(int color){
        return CCCommonUtils::getToolBgByColor(color);
    }
    static std::string getGenHeadBgByColor(int color){
        return CCCommonUtils::getGenHeadBgByColor(color);
    }
    static void fitContainerSizeAndAddChild(cocos2d::CCNode *container,cocos2d::CCNode *child,int type = 0){
        CCCommonUtils::fitContainerSizeAndAddChild(container,child,type);
    }
    static void setSpriteGray(CCSprite *sprite, bool gray){
        CCCommonUtils::setSpriteGray(sprite,gray);
    }
    static bool checkGeneralState(int _state){
        return CCCommonUtils::checkGeneralState(_state);
    }
    static std::string getIcon(std::string str){
        return CCCommonUtils::getIcon(str);
    }
    static bool isEquip(int itemid){
        return CCCommonUtils::isEquip(itemid);
    }
    static cocos2d::ccColor3B getItemColor(int color){
        return CCCommonUtils::getItemColor(color);
    }
    static string getColorName(int color){
        return CCCommonUtils::getColorName(color);
    }
    static bool isLackOfGold(int i){
        return CCCommonUtils::isLackOfGold(i);
    }
    static std::string getTalentStr(std::string talent, int type){
        return CCCommonUtils::getTalentStr(talent,type);
    }
    static std::string getTacticsStr(std::string talent, int type){
        return CCCommonUtils::getTacticsStr(talent,type);
    }
    static std::string getItemPropStr(EquipInfo* info, std::string att, int value){
        return CCCommonUtils::getItemPropStr(info,att,value);
    }
    static int getSoulExChangeNeedByColor(int color){
        return CCCommonUtils::getSoulExChangeNeedByColor(color);
    }
    static std::string getGeneralSkill(std::string generalId){
        return CCCommonUtils::getGeneralSkill(generalId);
    }
    static bool isFormationOpen(int index){
        return CCCommonUtils::isFormationOpen(index);
    }
    static int getFormationOpenNum(){
        return CCCommonUtils::getFormationOpenNum();
    }
    static int getNextFormationOpenLevel(){
        return CCCommonUtils::getNextFormationOpenLevel();
    }
    static int getQueueTypeByBuildType(int buildType){
        return CCCommonUtils::getQueueTypeByBuildType(buildType);
    }
    static int getBustIdxByName(string name){
        return CCCommonUtils::getBustIdxByName(name);
    }
    static std::string getLanguageFNByLocalSN(std::string localSN){
        return CCCommonUtils::getLanguageFNByLocalSN(localSN);
    }
    static float getEffectValueByNum(int num, bool isInludeGeneral = true){
        return CCCommonUtils::getEffectValueByNum(num,isInludeGeneral);
    }
    static float getStateEffectValueByNum(int num){
        return CCCommonUtils::getStateEffectValueByNum(num);
    }
    static float getVipEffectValueByNum(int num){
        return CCCommonUtils::getVipEffectValueByNum(num);
    }
    static float getOfficerValueByNum(int num){
        return CCCommonUtils::getOfficerValueByNum(num);
    }
    static float getTerritoryEffectValueByNum(int num){
        return CCCommonUtils::getTerritoryEffectValueByNum(num);
    }
    static float getTerritoryNegativeEffectValueByNum(int num){
        return CCCommonUtils::getTerritoryNegativeEffectValueByNum(num);
    }
    static std::string getEffFormat(string num){
        return CCCommonUtils::getEffFormat(num);
    }
    static std::string getEffPM(string num){
        return CCCommonUtils::getEffPM(num);
    }
    static bool isBtnOpen(int btnIndex){
        return CCCommonUtils::isBtnOpen(btnIndex);
    }
    static void recordStep(std::string index){
        CCCommonUtils::recordStep(index);
    }
    static int getNormalPray(){
        return CCCommonUtils::getNormalPray();
    }
    static int getIronPray(){
        return CCCommonUtils::getIronPray();
    }
    static int getGoldByTime(int t){
        return CCCommonUtils::getGoldByTime(t);
    }
    static int getResGlodByType(int type, int num){
        return CCCommonUtils::getResGlodByType(type,num);
    }
    static int getGoldBuyItem(std::string itemId){
        return CCCommonUtils::getGoldBuyItem(itemId);
    }
    static CCSprite* setSpriteMaxSize(CCSprite* spr, int limitNum, bool isForce=false){
        return CCCommonUtils::setSpriteMaxSize(spr,limitNum,isForce);
    }
    static CCSprite* getRomanSprite(int num){
        return CCCommonUtils::getRomanSprite(num);
    }
    static CCSprite* graylightWithCCSprite(CCSprite* oldSprite,bool isLight){
        return CCCommonUtils::graylightWithCCSprite(oldSprite,isLight);
    }
    static bool isTestPlatformAndServer(string key=""){
        return CCCommonUtils::isTestPlatformAndServer(key);
    }
    static bool hasSpeedEffect(int type){
        return CCCommonUtils::hasSpeedEffect(type);
    }
    static double getSpeedEffectTime(int type){
        return CCCommonUtils::getSpeedEffectTime(type);
    }
    static CCPoint getPointByString(std::string text,std::string separator=";"){
        return CCCommonUtils::getPointByString(text,separator);
    }
    static std::string changeTimeAddUnit(int secs){
        return CCCommonUtils::changeTimeAddUnit(secs);
    }
    static std::string subStrByUtf8(const string &str,int index,int len){
        return CCCommonUtils::subStrByUtf8(str,index,len);
    }
    static std::string getLanguage(){
        return CCCommonUtils::getLanguage();
    }
    static std::string getMoreLanguageType(string sysTemKey){
        return CCCommonUtils::getMoreLanguageType(sysTemKey);
    }
    static void makeEffectSpr(CCSprite* spr, string path, int num, float dt=0.07, int idx=0){
        CCCommonUtils::makeEffectSpr(spr,path,num,dt,idx);
    }
    static void makeEffectSprWithAP(CCSprite* spr, const Vec2 &anchorPos, string path, int num, float dt=0.07, int idx=0){
        CCCommonUtils::makeEffectSprWithAP(spr,anchorPos,path,num,dt,idx);
    }
    
    static void makeOnceEffectSpr(CCSprite* spr, string path, int num, float delate=0, float dt=0.07){
        CCCommonUtils::makeOnceEffectSpr(spr,path,num,delate,dt);
    }
    static void makeTwoAnimatSpr(CCSprite* spr, string path, int num, int splitNum, float dt=0.07, int idx=0){
        CCCommonUtils::makeTwoAnimatSpr(spr,path,num,splitNum,dt,idx);
    }
    static void makeInternalEffectSpr(CCSprite* spr, string path, int num, float dt=0.07, int idx=0,float delaytime=0){
        CCCommonUtils::makeInternalEffectSpr(spr,path,num,dt,idx,delaytime);
    }
    static CCSprite *addFilterSprite(std::string fileName, const ccColor3B &color, float bright = 0.0f, float contrast = 1.0f, float saturation = 1.0f){
        return CCCommonUtils::addFilterSprite(fileName,color,bright,contrast,saturation);
    }
    static ccColor3B covertHSBToRGB(int h, float s, float v){
        return CCCommonUtils::covertHSBToRGB(h,s,v);
    }
    static CCSprite *getItemBGByColor(int color){
        return CCCommonUtils::getItemBGByColor(color);
    }
    static void changeAllianceFlagBGByRank(CCSprite *sprite, int index, bool isGray){
        CCCommonUtils::changeAllianceFlagBGByRank(sprite,index,isGray);
    }
    static string getResourceStr(int resource){
        return CCCommonUtils::getResourceStr(resource);
    }
    static int getResourceLoadByType(int type){
        return CCCommonUtils::getResourceLoadByType(type);
    }
    static bool isServerCrossOpen(){
        return CCCommonUtils::isServerCrossOpen();
    }
    static bool isAdriodPad(){
        return CCCommonUtils::isAdriodPad();
    }
    static bool isAdriodPadHD(){
        return CCCommonUtils::isAdriodPadHD();
    }
    static bool isIOSPad(){
        return CCCommonUtils::isIOSPad();
    }
    static bool isIosAndroidPad(){
        return CCCommonUtils::isIosAndroidPad();
    }
    static bool isPad(){
        return CCCommonUtils::isPad();
    }
    static void setIsUseHD(bool isHD){
        CCCommonUtils::setIsUseHD(isHD);
    }
    static void setServerHDSwitch(){
        CCCommonUtils::setServerHDSwitch();
    }
    static bool getIsUseHD(){
        return CCCommonUtils::getIsUseHD();
    }
    static void setIsHDViewPort(bool isHD){
        CCCommonUtils::setIsHDViewPort(isHD);
    }
    static bool getIsHDViewPort(){
        return CCCommonUtils::getIsHDViewPort();
    }
    static void addResSearchPath(){
        return CCCommonUtils::addResSearchPath();
    }
    static void setCurUISkinType(){
        return CCCommonUtils::setCurUISkinType();
    }
    static bool isUseSpeUI(){
        return CCCommonUtils::isUseSpeUI();
    }
    static int getUISkinType(){
        return CCCommonUtils::getUISkinType();
    }
    static bool checkTaiWanFlag(){
        return CCCommonUtils::checkTaiWanFlag();
    }
    static std::string changeChinaFlag(string flag){
        return CCCommonUtils::changeChinaFlag(flag);
    }
    static std::string changeHKToChinaFlag(string flag){
        return CCCommonUtils::changeHKToChinaFlag(flag);
    }
    static bool isShowFlagForChinese(){
        return CCCommonUtils::isShowFlagForChinese();
    }
    static bool isInSimulator(){
        return CCCommonUtils::isInSimulator();
    }
    static bool checkVersion(string version){
        return CCCommonUtils::checkVersion(version);
    }
    static bool checkServer(string serverID){
        return CCCommonUtils::checkServer(serverID);
    }
    static bool canOpenNewIcon(){
        return CCCommonUtils::canOpenNewIcon();
    }
    static bool isNewIcon(string iconName){
        return CCCommonUtils::isNewIcon(iconName);
    }
    static std::string& StringReplace(std::string& strBig, const std::string& strsrc, const std::string& strdst){
        return CCCommonUtils::StringReplace(strBig,strsrc,strdst);
    }
    static bool useCaptcha(){
        return CCCommonUtils::useCaptcha();
    }
    static int getBindType(std::string bindStr){
        return CCCommonUtils::getBindType(bindStr);
    }
    static CCArray* getTypeArray(int type){
        return CCCommonUtils::getTypeArray(type);
    }
    static string getCustomPicUrl(string uid, int picVer){
        return CCCommonUtils::getCustomPicUrl(uid,picVer);
    }
    static string getCustomPicUrl1(const char * uid, int picVer){
        return CCCommonUtils::getCustomPicUrl1(uid,picVer);
    }
    static string getShakePicUrl(string picName){
        return CCCommonUtils::getShakePicUrl(picName);
    }
    static bool isUseCustomPic(int picVer){
        return CCCommonUtils::isUseCustomPic(picVer);
    }

    static void showHelpShiftFAQ(){
        CCCommonUtils::showHelpShiftFAQ();
    }
    static void sendHelpshiftLog(const char* IP, const char* uid, const char* tag){
        CCCommonUtils::sendHelpshiftLog(IP,uid,tag);
    }
    static void showHelpShiftFAQ(string itemId){
        CCCommonUtils::showHelpShiftFAQ(itemId);
    }
    static void showBanHelpShift(){
        CCCommonUtils::showBanHelpShift();
    }
    static void showEmulatorBanHelpShift(){
        CCCommonUtils::showEmulatorBanHelpShift();
    }
    static void showHelpShiftSingleFAQ(string itemId){
        CCCommonUtils::showHelpShiftSingleFAQ(itemId);
    }
    static bool isChina(){
        return CCCommonUtils::isChina();
    }
    static bool isFirstJoinAlliance(){
        return CCCommonUtils::isFirstJoinAlliance();
    }
    static string getDragonBuildingNameByType(int type){
        return CCCommonUtils::getDragonBuildingNameByType(type);
    }
    static string getDragonBuildingInfoByType(int type){
        return CCCommonUtils::getDragonBuildingInfoByType(type);
    }
    static std::string getVersionName(){
        return CCCommonUtils::getVersionName();
    }
    static float getDragonValueByNum(int num){
        return CCCommonUtils::getDragonValueByNum(num);
    }
    
};

#endif /* defined(__IF__CCCommonUtils__) */
