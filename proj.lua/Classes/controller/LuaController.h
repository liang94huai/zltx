//
//  LuaController.h
//  IF
//
//  Created by Émilie.Jiang on 15/1/26.
//
//

#ifndef __IF__LuaController__
#define __IF__LuaController__

#include <string.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ActivityEventObj.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class LuaController:public CCObject{
public:
    LuaController();
    ~LuaController();
    //
    // for lua use
    //
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

    static void showFAQ(std::string faq);
    //活动中心
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
    //chat notice
    void closeChatNotice();
    CCNode* getChatNoticeContainer();
    //触发帐号绑定引导
    static void showBindGuide();
    LuaActivityInfo* getActivityInfo(string actID);
    void sendNotification(string msg);
    Node* getImperialTouchLayer();
    bool isInTutorial();
    int getFileVersion(string name);
    Dictionary* getDicLuaData(Ref*);
    Array* getArrLuaData(Ref*);
    void openJoinAllianceView();
};


#endif /* defined(__IF__LuaController__) */
