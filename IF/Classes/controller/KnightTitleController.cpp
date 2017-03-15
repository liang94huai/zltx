//
//  KnightTitleController.cpp
//  IF
//
//  Created by 付彬 on 16/1/7.
//
//

#include "KnightTitleController.hpp"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "LocalController.h"
#include "YesNoDialog.h"
#include "ToolController.h"
#include "FunBuildController.h"
#include "FunBuild.h"
#include "KnightTitleCommand.hpp"
#include "EquipmentCommand.h"
#include "LuaController.h"

static KnightTitleController *_instance = NULL;

KnightTitleController::KnightTitleController()
{
//    auto allDictInfo = LocalController::shared()->DBXMLManager()->getGroupByKey("equipment");
//    if(allDictInfo)
//    {
//        KnightTitleInfoMap.clear();
//        
//        CCDictElement* element;
//        CCDICT_FOREACH(allDictInfo, element)
//        {
//            string strId = element->getStrKey();
//            CCDictionary* dictInfo = _dict(element->getObject());
//            int id = atoi(strId.c_str());
//            KnightTitleInfoMap[id] = KnightTitleInfo(dictInfo);
//            m_KnightLockMap[id] = 0;
//        }
//    }
    CCLOG("init knight xml");
}

KnightTitleController::~KnightTitleController() {}

KnightTitleController* KnightTitleController::getInstance() {
    if (!_instance) {
        _instance = new KnightTitleController();
    }
    return _instance;
}

void KnightTitleController::purgeData() {
    CC_SAFE_RELEASE_NULL( _instance );
    _instance = NULL;
}

//后台返回初始化数据
void KnightTitleController::retInitData(CCDictionary* dict)
{
//    CCDictionary* knightDict = _dict(dict->objectForKey("knight_title"));
//    for (map<int,int>::iterator it = m_KnightLockMap.begin(); it!=m_KnightLockMap.end(); it++) {
//        it->second = 0;
//    }
//    m_curKnightTitleId = -1;
//    
//    string unlock_title = knightDict->valueForKey("unlock_title")->getCString();
//    vector<string> unlockVec;
//    CCCommonUtils::splitString(unlock_title, ",", unlockVec);
//    for (int i=0; i<unlockVec.size(); i++)
//    {
//        int itemId = atoi(unlockVec[i].c_str());
//        m_KnightLockMap[itemId] = 1;
//    }
//    
//    m_curKnightTitleId = knightDict->valueForKey("activity")->intValue();
//    MakeEffectValue();
    
    CCDictionary* knightDict = _dict(dict->objectForKey("knight_title"));
    LuaController::getInstance()->saveLoginData("init_knight", knightDict);
    CCLOG("test");
}

void KnightTitleController::MakeEffectValue()
{
    KnightEffMap.clear();
    if ( KnightTitleInfoMap.find(m_curKnightTitleId) != KnightTitleInfoMap.end() )
    {
        auto& kInfo = KnightTitleInfoMap[m_curKnightTitleId];
        for (int i=1; i<=20; i++)
        {
            if (kInfo.values.find(i) != kInfo.values.end() && kInfo.values[i]>0) {
                for (int j=0; j<kInfo.parasMap[i].size(); j++) {
                    int eNum = kInfo.parasMap[i][j];
                    KnightEffMap[eNum] += kInfo.values[i];
                }
            }
        }
//        int exc = kInfo.power - GlobalData::shared()->playerInfo.knight_title;
        GlobalData::shared()->playerInfo.knight_title = kInfo.power;
        UIComponent::getInstance()->setUserData();
    }
}

//变更称号
bool KnightTitleController::startPutOnKnightTitle(int itemId)
{
    KnightTitlePutOnCommand* cmd = new KnightTitlePutOnCommand(itemId);
    cmd->sendAndRelease();
    return true;
}

void KnightTitleController::retPutOnKnightTitle(CCDictionary* dict)
{
    LuaController::getInstance()->backCMD("knight_puton", dict);
//    if(dict->objectForKey("errorCode"))
//    {
//        string error = dict->valueForKey("errorCode")->getCString();
//        CCCommonUtils::flyText(_lang(error.c_str()));
//    }
//    else
//    {
//        CCLOG("test");
//        m_curKnightTitleId = dict->valueForKey("CurKnightTitle")->intValue();
//        MakeEffectValue();
//    }
}

//解锁称号
bool KnightTitleController::startUnlockKnightTitle(int itemId)
{
    KnightTitleUnlockCommand* cmd = new KnightTitleUnlockCommand(itemId);
    cmd->sendAndRelease();
    return true;
}

void KnightTitleController::retUnlockKnightTitle(CCDictionary* dict)
{
    LuaController::getInstance()->backCMD("knight_unlock", dict);
//    if(dict->objectForKey("errorCode"))
//    {
//        string error = dict->valueForKey("errorCode")->getCString();
//        CCCommonUtils::flyText(_lang(error.c_str()));
//    }
//    else
//    {
//        CCLOG("test");
//    }
}

void KnightTitleController::UpdateUnlockKnight()
{
    int buildLv = 0;
    int bid = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_KNIGHT);
    if (bid > 0) {
        auto& bInfo = FunBuildController::getInstance()->getFunbuildById(bid);
        buildLv = bInfo.level;
    }
    if (buildLv == 0) {
        return;
    }
    auto dict = CCDictionary::create();
    dict->setObject(CCString::create(CC_ITOA(buildLv)), "level");
    LuaController::getInstance()->backCMD("knight_update", dict);
//    map<int, int>::iterator it = m_KnightLockMap.begin();
//    for (; it!=m_KnightLockMap.end(); it++)
//    {
//        if (it->second == 0)
//        {
//            auto& kInfo = KnightTitleInfoMap[it->first];
//            if (kInfo.mateUnlockMap.size() <= 0)
//            {
//                if (kInfo.bd_level <= buildLv) {
//                    it->second = 1;
//                }
//            }
//        }
//    }
}

//合成徽章
bool KnightTitleController::startComposeBedge(int itemId)
{
    MaterialCreateCommand* cmd = new MaterialCreateCommand(CC_ITOA(itemId), 3);
    cmd->sendAndRelease();
    return true;
}
void KnightTitleController::retComposeBedge(CCDictionary* dict)
{
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        if(error=="102198"){//不显示材料不足弹出提示
            return;
        }
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else
    {
        CCLOG("test");
        if (dict->objectForKey("dec")) {//消耗掉的道具信息
            auto decDic = _dict(dict->objectForKey("dec"));
            ToolController::getInstance()->pushAddTool(decDic);
        }
        if (dict->objectForKey("add")) {//新增的道具信息
            auto addDic = _dict(dict->objectForKey("add"));
            ToolController::getInstance()->pushAddTool(addDic);
        }
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MATE_CRT_END);
    }

}
//分解徽章
bool KnightTitleController::startDecBedge(int itemId)
{
    MaterialDecomposeCommand* cmd = new MaterialDecomposeCommand(CC_ITOA(itemId));
    cmd->sendAndRelease();
    return true;
}
void KnightTitleController::retDecBedge(CCDictionary* dict)
{
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else
    {
        CCLOG("test");
        if (dict->objectForKey("dec")) {//消耗掉的道具信息
            auto decDic = _dict(dict->objectForKey("dec"));
            ToolController::getInstance()->pushAddTool(decDic);
        }
        if (dict->objectForKey("add")) {//新增的道具信息
            auto addDic = _dict(dict->objectForKey("add"));
            ToolController::getInstance()->pushAddTool(addDic);
        }
    }
}

//获取已经使用的徽章数量
int KnightTitleController::GetUseBedgeById(int itemId)
{
    if (m_curKnightTitleId > 0)
    {
        if ( KnightTitleInfoMap[m_curKnightTitleId].mateUseMap.find(itemId) != KnightTitleInfoMap[m_curKnightTitleId].mateUseMap.end())
        {
            return KnightTitleInfoMap[m_curKnightTitleId].mateUseMap[itemId];
        }
    }
    else
    {
        return 0;
    }
}

