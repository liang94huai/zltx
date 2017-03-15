//
//  EquipmentController.cpp
//  IF
//
//  Created by fubin on 14-12-2.
//
//

#include "EquipmentController.h"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "LocalController.h"
#include "YesNoDialog.h"
#include "QueueController.h"
#include "ToolController.h"
#include "EquipmentCommand.h"
#include "FunBuildController.h"
#include "EquipFinishView.h"
#include "EquipSiteView.h"
#include "EquipmentListView.h"
#include "EquipmentCreateView.h"
#include "EquipNewUseView.h"
#include "FunBuild.h"

static EquipmentController *_instance = NULL;

EquipmentController::EquipmentController()
{
    m_callBackFunc = NULL;
    m_refineTimes = 0;
    m_longSellLv = 25;
    m_equipMaxSum = 1000;
    m_isTip= true;
    m_meltKey = "";
    auto allDictInfo = LocalController::shared()->DBXMLManager()->getGroupByKey("equipment");
    if(allDictInfo)
    {
        CCDictElement* element;
        CCDICT_FOREACH(allDictInfo, element)
        {
            string strId = element->getStrKey();
            CCDictionary* dictInfo = _dict(element->getObject());
            int id = atoi(strId.c_str());
            EquipmentInfoMap[id] = EquipmentInfo(dictInfo);
            if (EquipmentInfoMap[id].color == 5) {
                SelEquipList.push_back(id);
            }
        }
    }
    
    auto equipSciEffDict = LocalController::shared()->DBXMLManager()->getGroupByKey("science_equip");
    if(equipSciEffDict)
    {
        CCDictElement* elem;
        CCDICT_FOREACH(equipSciEffDict, elem)
        {
            string strId = elem->getStrKey();
            CCDictionary* dictInfo = _dict(elem->getObject());
            int sciEff = dictInfo->valueForKey("science_effect")->intValue();
            int sciSite = dictInfo->valueForKey("site")->intValue();
            int sciToEff = dictInfo->valueForKey("effect")->intValue();
            equipSci_site[sciEff] = sciSite;
            equipSci_eff[sciEff][sciToEff] = 0;
        }
    }
    CCLOG("init equipment xml");
}

EquipmentController::~EquipmentController() {}

EquipmentController* EquipmentController::getInstance() {
    if (!_instance) {
        _instance = new EquipmentController();
    }
    return _instance;
}

void EquipmentController::purgeData() {
    CC_SAFE_RELEASE_NULL( _instance );
    _instance = NULL;
}

//后台返回初始化数据
void EquipmentController::retInitData(CCDictionary* dict)
{
    m_MyOnEquip.clear();
    m_MyEquipmentMap.clear();
    equipSciValueMap.clear();
    EquipEffMap.clear();
    auto arr = dynamic_cast<CCArray*>(dict->objectForKey("equip"));
    if( arr ){
        CCDictionary *item = NULL;
        for (int i=0; i<arr->count(); i++) {
            item = _dict(arr->objectAtIndex(i));
            string uuid = item->valueForKey("uuid")->getCString();
            int itemId = item->valueForKey("itemId")->intValue();
            int on = item->valueForKey("on")->intValue();
            if (on==1) {
                m_MyOnEquip[uuid] = itemId;
            }
            else {
                m_MyEquipmentMap[uuid] = itemId;
            }
//            m_MyEquipmentMap[uuid] = itemId;
            if (EquipmentInfoMap[itemId].site==6) {
                LongEquipmentInfoMap[uuid]=EquipmentInfoMap[itemId];
                EquipmentInfo &equipInfo = LongEquipmentInfoMap[uuid];
                string extraProperty = item->valueForKey("extraProperty")->getCString();
                string refineProperty = item->valueForKey("refineProperty")->getCString();
                equipInfo.setLongProperty(extraProperty);
                equipInfo.setLongRefinePro(refineProperty);
            }
        }
        MakeEffectValue();
    }
}

string EquipmentController::addEquip(CCArray* equipArr)//添加装备
{
    string ret="";
    if (equipArr)
    {
        CCDictionary *item = NULL;
        for (int i=0; i<equipArr->count(); i++)
        {
            item = _dict(equipArr->objectAtIndex(i));
            string uuid = item->valueForKey("uuid")->getCString();
            int itemId = item->valueForKey("itemId")->intValue();
            m_MyEquipmentMap[uuid] = itemId;
            if (ret=="") {
                ret = uuid;
            }
        }
    }
    return ret;
}

//合成装备
bool EquipmentController::startCreateEquipment(string uuid, string qItemId, vector<int> toolItemIds, int gold)
{
    int paramGold=0;
    if (gold>0) {
        paramGold=1;
        //判断金币数量是否足够
        if (GlobalData::shared()->playerInfo.gold < gold) {
            YesNoDialog::gotoPayTips();
            return false;
        }
    }
    
    m_curOptMate = toolItemIds;
    m_curOptEquip = uuid;
    EquipmentCreateCommand* cmd = new EquipmentCreateCommand(uuid, qItemId, toolItemIds,0,paramGold);
    cmd->sendAndRelease();
    return true;
}

void EquipmentController::retCreateEquipment(CCDictionary* dict)
{
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else
    {
        if (m_MyEquipmentMap.find(m_curOptEquip) != m_MyEquipmentMap.end()) {
            m_MyEquipmentMap.erase(m_curOptEquip);
        }
        for (int i=0; i<m_curOptMate.size(); i++) {
            auto& tool = ToolController::getInstance()->getToolInfoById(m_curOptMate[i]);
            int count =MAX(tool.getCNT() - 1, 0) ;
            tool.setCNT(count);
        }
        
        if (dict->objectForKey("queue")) {
            auto queue = _dict(dict->objectForKey("queue"));
            string key = QueueController::getInstance()->addQueueInfo(queue);
        }
        if (dict->objectForKey("resource")) {
            auto resInfo = _dict(dict->objectForKey("resource"));
            GlobalData::shared()->resourceInfo.setResourceData(resInfo);
        }
        int tmpInt = dict->valueForKey("gold")->intValue();
        UIComponent::getInstance()->updateGold(tmpInt);
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(EQUIP_CREATE_END);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("showEquipTips");
    }
    m_curOptEquip = "";
    m_curOptMate.clear();
}

void EquipmentController::startFinishCrtEquip(string quuid)
{
    HavestEquipCommand * cmd = new HavestEquipCommand(quuid);
    cmd->sendAndRelease();
}

void EquipmentController::retFinishCrtEquip(CCDictionary* dict)
{
    if (dict->objectForKey("uuid") && dict->objectForKey("itemId")) {
        string uuid = dict->valueForKey("uuid")->getCString();
        int itemId = dict->valueForKey("itemId")->intValue();
        m_MyEquipmentMap[uuid] = itemId;
        
        int qid = QueueController::getInstance()->getCanRecQidByType(TYPE_FORGE);
        if (qid != QID_MAX) {
            int bid = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_FORGE);
            QueueController::getInstance()->startFinishQueue(qid, false);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REMOVE_STATE, CCInteger::create(bid));
        }
        
        auto equipView = EquipFinishView::create(itemId);
        if (PopupViewController::getInstance()->getCurrViewCount()==0) {
            PopupViewController::getInstance()->addPopupView(equipView);
        }
        else {//有其他面板打开时 不显示升级面板
            PopupBaseView* curView = PopupViewController::getInstance()->getCurrentPopupView();
            EquipSiteView* siteView = dynamic_cast<EquipSiteView*>(curView);
            EquipmentListView* listView = dynamic_cast<EquipmentListView*>(curView);
            EquipmentCreateView* createView = dynamic_cast<EquipmentCreateView*>(curView);
            EquipNewUseView* newUseView = dynamic_cast<EquipNewUseView*>(curView);
            if (siteView || listView || createView || newUseView) {
                if (newUseView) {
                    equipView->setCurPopType( newUseView->m_weapType );
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(EQUIP_CLOSE_VIEW);
                }
                PopupViewController::getInstance()->addPopupView(equipView);
            }
            else {
                PopupViewController::getInstance()->pushPop(equipView);
            }
        }
    }
}

//分解装备
bool EquipmentController::startDesEquipment(string uuid)
{
    m_curOptEquip = uuid;
    EquipmentCommand* cmd = new EquipmentCommand(uuid, EQU_DES_COMMAND);
    cmd->sendAndRelease();
    return true;
}

void EquipmentController::retDesEquipment(CCDictionary* dict)
{
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else
    {
        CCLOG("destroy equipment end");
        if (m_MyEquipmentMap.find(m_curOptEquip) != m_MyEquipmentMap.end()) {
            m_MyEquipmentMap.erase(m_curOptEquip);
        }
        
        string mateItems = "";
        auto arr = dynamic_cast<CCArray*>(dict->objectForKey("goods"));
        if(  arr )
        {
            CCDictionary* item = NULL;
            for (int i=0; i<arr->count(); i++) {
                item = _dict(arr->objectAtIndex(i));
                int itemId = item->valueForKey("itemId")->intValue();
                int num = item->valueForKey("count")->intValue();
                string uuid = item->valueForKey("uuid")->getCString();
                auto &info = ToolController::getInstance()->getToolInfoById(itemId);
                int currentNum = info.getCNT();
                ToolController::getInstance()->addTool(itemId, num, uuid);
            
                mateItems = mateItems+CC_ITOA(itemId)+"|"+CC_ITOA(num - currentNum);
                if (i <= arr->count()-1) {
                    mateItems = mateItems+";";
                }
            }
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(EQUIP_BAG_SHOW_DES, CCString::create(mateItems));
    }
    m_curOptEquip = "";
}
//卖龙装备
bool EquipmentController::startSellLongEquipment(string uuid)
{
    m_curOptEquip = uuid;
    EquipmentCommand* cmd = new EquipmentCommand(uuid, EQU_SELL_LONG_COMMAND);
    cmd->putParam("newVersion", CCInteger::create(1));//后台使用，之前版本没有此参数
    cmd->sendAndRelease();
    return true;
}

void EquipmentController::retSellLongEquipment(CCDictionary* dict)
{
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else
    {
        CCLOG("destroy equipment end");
        vector<string> delUUidVec;
        if (m_MyEquipmentMap.find(m_curOptEquip) != m_MyEquipmentMap.end())
        {
//            m_MyEquipmentMap.erase(m_curOptEquip);
            
            int equipId = m_MyEquipmentMap[m_curOptEquip];
            auto& equip = EquipmentInfoMap[equipId];
            
            map<string, int>::iterator it = m_MyEquipmentMap.begin();
            for (; it!=m_MyEquipmentMap.end(); it++)
            {
                auto& tmpE = EquipmentInfoMap[it->second];
                if (tmpE.color == equip.color) {
                    delUUidVec.push_back(it->first);
                }
            }
            
            for (int i=0; i<delUUidVec.size(); i++) {
                m_MyEquipmentMap.erase(delUUidVec[i]);
                LongEquipmentInfoMap.erase(delUUidVec[i]);
            }
            
        }
        
        if(dict->valueForKey("crystal")){
            int longJing = dict->valueForKey("crystal")->intValue();
            UIComponent::getInstance()->updateLongJing(longJing);
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(EQUIP_BAG_SHOW_DES, NULL);
    }
    m_curOptEquip = "";
}

//穿戴装备
bool EquipmentController::startPutOnEquipment(string uuid, string oldUuid)
{
    int itemId = m_MyEquipmentMap[uuid];
    auto& eInfo = EquipmentInfoMap[itemId];
    if (eInfo.level > GlobalData::shared()->playerInfo.level) {
        CCCommonUtils::flyHint("", "", _lang("119040"));
        return false;
    }
    
    m_curOptEquip = uuid;
    m_oldEquip = oldUuid;
    EquipmentCommand* cmd = new EquipmentCommand(uuid, EQU_PUTON_COMMAND);
    cmd->sendAndRelease();
    return true;
}

void EquipmentController::retPutOnEquipment(CCDictionary* dict)
{
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else
    {
        if (m_curOptEquip!="" && m_MyEquipmentMap.find(m_curOptEquip)!=m_MyEquipmentMap.end())
        {
            string oldEquip = "";
            string curEquip = m_curOptEquip;
            if (m_oldEquip!="" && m_MyOnEquip.find(m_oldEquip)!=m_MyOnEquip.end()) {//之前穿的装备 换到仓库里
                m_MyEquipmentMap[m_oldEquip] = m_MyOnEquip[m_oldEquip];
                m_MyOnEquip.erase(m_oldEquip);
                oldEquip = m_oldEquip;
            }
            
            m_MyOnEquip[m_curOptEquip] = m_MyEquipmentMap[m_curOptEquip];
            m_MyEquipmentMap.erase(m_curOptEquip);
            MakeEffectValue();
            showPowerChangeEff(oldEquip, curEquip);
            
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(EQUIP_PUTON_MSG, CCString::create(m_curOptEquip));
            
            auto& tmpE = EquipmentInfoMap[m_MyOnEquip[m_curOptEquip]];
            string tmpItemId = CC_ITOA(m_MyOnEquip[m_curOptEquip]);
            string icon = CCCommonUtils::getIcon( tmpItemId );
            string colorName = CCCommonUtils::getColorName( tmpE.color );
            string equipName = _lang(tmpE.name);
            CCCommonUtils::flyHintWithDefault(icon, "", _lang_2("111739", colorName.c_str(), equipName.c_str()));
        }
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(EQUIP_CLOSE_VIEW);
    m_curOptEquip = "";
    m_oldEquip = "";
}

//卸下装备
bool EquipmentController::startTakeOffEquipment(string uuid)
{
    m_curOptEquip = uuid;
    EquipmentCommand* cmd = new EquipmentCommand(uuid, EQU_TAKEOFF_COMMAND);
    cmd->sendAndRelease();
    return true;
}

void EquipmentController::retTakeOffEquipment(CCDictionary* dict)
{
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else
    {
        if (m_curOptEquip!="" && m_MyOnEquip.find(m_curOptEquip)!=m_MyOnEquip.end()) {
            string oldEquip = m_curOptEquip;
            string curEquip = "";
            m_MyEquipmentMap[m_curOptEquip] = m_MyOnEquip[m_curOptEquip];
            m_MyOnEquip.erase(m_curOptEquip);
            MakeEffectValue();
            showPowerChangeEff(oldEquip, curEquip);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(EQUIP_TAKEOFF_MSG, CCString::create(m_curOptEquip));
            
            auto& tmpE = EquipmentInfoMap[m_MyEquipmentMap[m_curOptEquip]];
            string tmpItemId = CC_ITOA(m_MyEquipmentMap[m_curOptEquip]);
            string icon = CCCommonUtils::getIcon( tmpItemId );
            string colorName = CCCommonUtils::getColorName( tmpE.color );
            string equipName = _lang(tmpE.name);
            CCCommonUtils::flyHintWithDefault(icon, "", _lang_2("111740", colorName.c_str(), equipName.c_str()));
        }
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(EQUIP_CLOSE_VIEW);
    m_curOptEquip = "";
}

bool EquipmentController::startMaterialCreate(int toolId,int type)
{
    MaterialCreateCommand* cmd = new MaterialCreateCommand(CC_ITOA(toolId),type);
    cmd->sendAndRelease();
    return true;
}

void EquipmentController::retMaterialCreate(CCDictionary* dict)
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
        if (dict->objectForKey("dec")) {//消耗掉的道具信息
            auto decDic = _dict(dict->objectForKey("dec"));
            ToolController::getInstance()->pushAddTool(decDic);
        }
        if (dict->objectForKey("add")) {//新增的道具信息
            auto addDic = _dict(dict->objectForKey("add"));
            ToolController::getInstance()->pushAddTool(addDic);
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MATE_CRT_END);
    }
}

bool compareSelMember(int item1, int item2)
{//先按等级排序， 再按部位排序
    if( EquipmentController::getInstance()->EquipmentInfoMap.find(item1) == EquipmentController::getInstance()->EquipmentInfoMap.end() )
        return false;
    
    if( EquipmentController::getInstance()->EquipmentInfoMap.find(item2) == EquipmentController::getInstance()->EquipmentInfoMap.end() )
        return false;
    
    auto& info1 = EquipmentController::getInstance()->EquipmentInfoMap[item1];
    auto& info2 = EquipmentController::getInstance()->EquipmentInfoMap[item2];
    if (info1.level > info2.level) {
        return true;
    }
    else if (info1.level < info2.level) {
        return false;
    }
    else {
        if (info1.site > info2.site) {
            return true;
        }
        else {
            return false;
        }
    }
}

bool compareTakeMember(string item1, string item2)
{//先按是否是胚子排序，再按等级排序， 再按品质排序
    if( EquipmentController::getInstance()->m_MyEquipmentMap.find(item1) == EquipmentController::getInstance()->m_MyEquipmentMap.end() )
        return false;
    
    if( EquipmentController::getInstance()->m_MyEquipmentMap.find(item2) == EquipmentController::getInstance()->m_MyEquipmentMap.end() )
        return false;
    
    auto& info1 = EquipmentController::getInstance()->EquipmentInfoMap[ EquipmentController::getInstance()->m_MyEquipmentMap[item1] ];
    auto& info2 = EquipmentController::getInstance()->EquipmentInfoMap[ EquipmentController::getInstance()->m_MyEquipmentMap[item2] ];
    
    if(info1.showType != "no" && info2.showType == "no")
    {
        return true;
    }
    else if(info1.showType == "no" && info2.showType != "no")
    {
        return false;
    }
    else if (info1.level > info2.level) {
        return true;
    }
    else if (info1.level < info2.level) {
        return false;
    }
    else {
        if (info1.color > info2.color) {
            return true;
        }
        else {
            return false;
        }
    }
}

bool compareEquipMember(string item1, string item2)
{//先按等级排序 从小到大， 再按品质排序从小到大
    if( EquipmentController::getInstance()->m_MyEquipmentMap.find(item1) == EquipmentController::getInstance()->m_MyEquipmentMap.end() )
        return false;
    
    if( EquipmentController::getInstance()->m_MyEquipmentMap.find(item2) == EquipmentController::getInstance()->m_MyEquipmentMap.end() )
        return false;
    
    auto& info1 = EquipmentController::getInstance()->EquipmentInfoMap[ EquipmentController::getInstance()->m_MyEquipmentMap[item1] ];
    auto& info2 = EquipmentController::getInstance()->EquipmentInfoMap[ EquipmentController::getInstance()->m_MyEquipmentMap[item2] ];
    if(info1.showType == "no" && info2.showType != "no")
    {
        return true;
    }
    else if(info1.showType != "no" && info2.showType == "no")
    {
        return false;
    }
    else if (info1.level < info2.level) {
        return true;
    }
    else if (info1.level > info2.level) {
        return false;
    }
    else {
        if (info1.color < info2.color) {
            return true;
        }
        else {
            return false;
        }
    }
}

//按部件 和 材料 查找装备
void EquipmentController::SelEquipmentsByType(vector<int>& ret, int site, int mate )
{
    ret.clear();
    if (site!=-1) {
        for (int i=0; i<SelEquipList.size(); i++) {
            map<int, EquipmentInfo> ::iterator itor = EquipmentInfoMap.find(SelEquipList[i]);
            if( itor != EquipmentInfoMap.end() )
            {
                if (itor->second.site == site) {
                    ret.push_back(SelEquipList[i]);
                }
            }
           
        }
    }
    
    if (site==-1 && mate!=-1) {
        for (int i=0; i<SelEquipList.size(); i++) {
            //auto& info = EquipmentInfoMap[SelEquipList[i]];
            map<int, EquipmentInfo> ::iterator itor = EquipmentInfoMap.find(SelEquipList[i]);
            if( itor != EquipmentInfoMap.end() )
            {
                if (itor->second.mateMap.find(mate) != itor->second.mateMap.end()) {
                    ret.push_back(SelEquipList[i]);
                }
            }
        }
    }
    else {
        if (mate!=-1) {
            vector<int> lastRet;
            for (int i=0; i<ret.size(); i++) {
                //auto& info = EquipmentInfoMap[ret[i]];
                map<int, EquipmentInfo> ::iterator itor = EquipmentInfoMap.find(ret[i]);
                if( itor != EquipmentInfoMap.end() )
                {
                    if (itor->second.mateMap.find(mate) != itor->second.mateMap.end()) {
                        lastRet.push_back(ret[i]);
                    }
                }
            }
            sort(lastRet.begin(), lastRet.end(), compareSelMember);
            ret = lastRet;
            return ;
        }
    }
    sort(ret.begin(), ret.end(), compareSelMember);
    return ;
}

//按部件查找 现有 装备
vector<string> EquipmentController::FindEquipmentsByType(int site)
{
    vector<string> ret;
    map<string, int>::iterator it = m_MyEquipmentMap.begin();
    for (; it!=m_MyEquipmentMap.end(); it++) {
        int itemId = it->second;
        if (EquipmentInfoMap.find(itemId) != EquipmentInfoMap.end()) {
            if (EquipmentInfoMap[itemId].site == site) {
                ret.push_back(it->first);
            }
        }
    }
    sort(ret.begin(), ret.end(), compareTakeMember);
    return ret;
}

bool EquipmentController::IsExsitRealEquipBySite(int site)
{
    bool ret = false;
    map<string, int>::iterator it = m_MyEquipmentMap.begin();
    for (; it!=m_MyEquipmentMap.end(); it++) {
        int itemId = it->second;
        if (EquipmentInfoMap.find(itemId) != EquipmentInfoMap.end()) {
            if (EquipmentInfoMap[itemId].site == site && EquipmentInfoMap[itemId].showType != "no") {
                ret = true;
                break;
            }
        }
    }
    return ret;
}

bool EquipmentController::IsExsitEquipBySite(int site)
{
    bool ret = false;
    map<string, int>::iterator it = m_MyEquipmentMap.begin();
    for (; it!=m_MyEquipmentMap.end(); it++) {
        int itemId = it->second;
        if (EquipmentInfoMap.find(itemId) != EquipmentInfoMap.end()) {
            if (EquipmentInfoMap[itemId].site == site) {
                ret = true;
                break;
            }
        }
    }
    return ret;
}

//按部件查找 现有 装备
vector<string> EquipmentController::FindEquipmentsByLv(int needLv1, int needLv2)
{
    vector<string> ret;
    map<string, int>::iterator it = m_MyEquipmentMap.begin();
    for (; it!=m_MyEquipmentMap.end(); it++) {
        auto& tmpInfo = EquipmentController::getInstance()->EquipmentInfoMap[it->second];
        if (tmpInfo.level == needLv1 || tmpInfo.level == needLv2) {
            ret.push_back(it->first);
        }
    }
    sort(ret.begin(), ret.end(), compareEquipMember);
    return ret;
}
//按等级查找 已经穿上的装备
vector<string> EquipmentController::FindOnEquipmentsByLv(int needLv1, int needLv2)
{
    vector<string> ret;
    map<string, int>::iterator it = m_MyOnEquip.begin();
    for (; it!=m_MyOnEquip.end(); it++) {
        auto& tmpInfo = EquipmentController::getInstance()->EquipmentInfoMap[it->second];
        if (tmpInfo.level == needLv1 || tmpInfo.level == needLv2) {
            ret.push_back(it->first);
        }
    }
    sort(ret.begin(), ret.end(), compareEquipMember);
    return ret;
}

//按部件查找 已经穿上的装备
string EquipmentController::FindOnEquipBySite(int site)
{
    string ret = "";
    map<string, int>::iterator it = m_MyOnEquip.begin();
    for (; it!=m_MyOnEquip.end(); it++) {
        auto& tmpInfo = EquipmentInfoMap[it->second];
        if (tmpInfo.site == site) {
            ret = it->first;
            break;
        }
    }
    return ret;
}

//检查当前背包里是否有该类装备 可以是不同品质
bool EquipmentController::IsHaveEquipByLv(int lv, int selfLv)
{
    map<string, int>::iterator it = m_MyEquipmentMap.begin();
    for (; it!=m_MyEquipmentMap.end(); it++) {
        auto& tmpInfo = EquipmentController::getInstance()->EquipmentInfoMap[it->second];
        if (tmpInfo.level == lv || tmpInfo.level == selfLv) {
            return true;
        }
    }
    return false;
}

//检查是否有某个装备
string EquipmentController::IsHaveEquipById(int itemId)
{//优先返回已经穿戴的装备
    map<string, int>::iterator it2 = m_MyOnEquip.begin();
    for (; it2!=m_MyOnEquip.end(); it2++) {
        if (it2->second == itemId) {
            return it2->first;
        }
    }
    map<string, int>::iterator it = m_MyEquipmentMap.begin();
    for (; it!=m_MyEquipmentMap.end(); it++) {
        if (it->second == itemId) {
            return it->first;
        }
    }
    return "";
}

//检查是否有未使用的装备 且有佩戴位置
bool EquipmentController::IsCanSetEquipBySite(int site)
{
    bool ret = false;
    if (site == -1)
    {
        map<int, int> _havePutOnEquips;//已经穿上装备的部位
        map<string, int>::iterator itPutOn = m_MyOnEquip.begin();
        for (; itPutOn!=m_MyOnEquip.end(); itPutOn++) {
            auto& tmpInfo = EquipmentController::getInstance()->EquipmentInfoMap[itPutOn->second];
            _havePutOnEquips[tmpInfo.site] = 1;
        }
        
        map<string, int>::iterator it = m_MyEquipmentMap.begin();
        for (; it!=m_MyEquipmentMap.end(); it++) {
            auto& tmpInfo = EquipmentController::getInstance()->EquipmentInfoMap[it->second];
            if (_havePutOnEquips.find(tmpInfo.site) == _havePutOnEquips.end()) {
                ret = true;
            }
        }
        
    }
    else {//背包里已经有 该部位的 装备了
        ret = true;
        map<string, int>::iterator it = m_MyOnEquip.begin();
        for (; it!=m_MyOnEquip.end(); it++) {
            auto& tmpInfo = EquipmentController::getInstance()->EquipmentInfoMap[it->second];
            if (tmpInfo.site == site) {
                ret = false;
                break;
            }
        }
    }
    
    return ret;
}

bool EquipmentController::CheckSelEquipMate(int site, int mate)
{
    bool ret = false;
    map<int, int> tmpToolCntMap;
    vector<int> tmpVec ;
    SelEquipmentsByType(tmpVec, site, mate);
    
//    int buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_FORGE);
//    auto& buildInfo = FunBuildController::getInstance()->getFunbuildById(buildId);
//    int bdPara2 = atoi(buildInfo.para[2].c_str());
    
    for (int i=0; i<tmpVec.size(); i++) {
        //auto& info = EquipmentInfoMap[tmpVec[i]];
        map<int, EquipmentInfo> ::iterator itor = EquipmentInfoMap.find(tmpVec[i]);
        if( itor == EquipmentInfoMap.end() )
        {
            continue;
        }
        
        auto& info =itor->second;
        
        if (info.costLv>0) {
            if ( !IsHaveEquipByLv(info.costLv, info.level) ) {
                info.canCrt = false;
                continue;
            }
        }
        
//        int tmp_costRes = 0;
//        vector<string> resCostVec;
//        CCCommonUtils::splitString(info.cost, ";", resCostVec);
//        if (resCostVec.size()==2) {
//            tmp_costRes = atoi(resCostVec[1].c_str());
//            int tmpCostRes = tmp_costRes * bdPara2*1.0/100;
//            tmp_costRes -= tmpCostRes;
//            int resType = atoi(resCostVec[0].c_str());
//            int curNum = CCCommonUtils::getCurResourceByType(resType);
//            if (tmp_costRes > curNum) {
//                info.canCrt = false;
//                continue;
//            }
//        }

        
        bool st = true;
        if(info.mateMap.size()>0){
            for (map<int, int>::iterator it=info.mateMap.begin(); it!=info.mateMap.end(); it++) {
                if (tmpToolCntMap.find(it->first) == tmpToolCntMap.end()) {
                    tmpToolCntMap[it->first] = ToolController::getInstance()->getMateCntByType(it->first);
                }
                
                if (tmpToolCntMap[it->first] < it->second) {
                    st = false;
                    break;
                }
            }
        }
        
        info.canCrt = st;
        if (st) {
            ret = st;
        }
    }
    return ret;
}

vector<int> EquipmentController::CheckEquipSiteMake()
{
    vector<int> ret;
    for (int i=0; i<=7; i++) {
        if( CheckSelEquipMate(i, -1) ) {
            ret.push_back(i);
        }
    }
    return ret;
}

void EquipmentController::retInitConf(CCDictionary* dict)
{
    if (dict->objectForKey("k2")) {//基础值
        string defValue = dict->valueForKey("k2")->getCString();
        vector<string> tmpVec;
        CCCommonUtils::splitString(defValue, "|", tmpVec);
        if (tmpVec.size()==6) {
            for (int i=0; i<tmpVec.size(); i++) {
                m_defPro[i] = atof(tmpVec[i].c_str());
            }
        }
    }
    
    if (dict->objectForKey("k9")) {//衰减系数
        string defValue = dict->valueForKey("k9")->getCString();
        vector<string> tmpVec;
        CCCommonUtils::splitString(defValue, "|", tmpVec);
        if (tmpVec.size()==6) {
            for (int i=0; i<tmpVec.size(); i++) {
                m_colorPro[i] = atof(tmpVec[i].c_str());
            }
        }
    }
    
    if (dict->objectForKey("k10")) {//不同等级的装备 转换为相同品质材料的数量
        string defValue = dict->valueForKey("k10")->getCString();
        vector<string> tmpVec;
        CCCommonUtils::splitString(defValue, "|", tmpVec);
        for (int i=0; i<tmpVec.size(); i++)
        {
            vector<string> cellVec;
            CCCommonUtils::splitString(tmpVec[i], ";", cellVec);
            if (cellVec.size()==2) {
                m_lvToCnt[ atoi(cellVec[0].c_str()) ] = atoi(cellVec[1].c_str());
            }
        }
    }
    
    if (true) {//颜色参数
        for (int i=WHITE; i<=GOLDEN; i++) {
            m_colorWeight[i] = i+1;
        }
    }
}

vector<float> EquipmentController::MakeEquipmentPro(string uuid, string qItemId, vector<int> toolUuids, bool isUUid)
{
    map<int,int> m_colorNum;//颜色对应的编号
    map<int, int> tmpColorMap;//统计材料数量
    tmpColorMap[WHITE] = 0;
    tmpColorMap[GREEN] = 0;
    tmpColorMap[BLUE] = 0;
    tmpColorMap[PURPLE] = 0;
    tmpColorMap[ORANGE] = 0;
    tmpColorMap[GOLDEN] = 0;
    if (uuid != "") {
        if (isUUid && m_MyEquipmentMap.find(uuid) != m_MyEquipmentMap.end() ) {
            int eId = m_MyEquipmentMap[uuid];
            auto& eInfo = EquipmentInfoMap[eId];
            int tmpCnt = 1;
            if (m_lvToCnt.find(eInfo.level) != m_lvToCnt.end()) {
                tmpCnt = m_lvToCnt[eInfo.level];
            }
            tmpColorMap[ eInfo.color ] += tmpCnt;
        }
        else if (!isUUid) {
            auto& eInfo = EquipmentInfoMap[atoi(uuid.c_str())];
            int tmpCnt = 1;
            if (m_lvToCnt.find(eInfo.level) != m_lvToCnt.end()) {
                tmpCnt = m_lvToCnt[eInfo.level];
            }
            tmpColorMap[ eInfo.color ] += tmpCnt;
        }
    }
    
    for (int i=0; i<toolUuids.size(); i++) {
        auto& tool = ToolController::getInstance()->getToolInfoById(toolUuids[i]);
        tmpColorMap[ tool.color ] += 1;
    }
    
    int minColor = 99;
    int maxColor = 99;
    float cntPro = 100.0;
    map<int, float> tmpColorPro;//配置默认最小概率
    tmpColorPro[WHITE] = 0.0;
    tmpColorPro[GREEN] = 0.0;
    tmpColorPro[BLUE] = 0.0;
    tmpColorPro[PURPLE] = 0.0;
    tmpColorPro[ORANGE] = 0.0;
    tmpColorPro[GOLDEN] = 0.0;
    //计算最小颜色值 和 最大颜色值 从高品质 到 低品质 初始计算
    int TmpCNum = 1;
    for (int color=GOLDEN; color>=WHITE; color--)
    {
        if (tmpColorMap[color]>0) {
            tmpColorPro[color] = 0;
            if (maxColor == 99) {
                maxColor = color;
                minColor = color;
            }else {
                minColor = color;
            }
            
            m_colorNum[color] = TmpCNum;
            TmpCNum++;
            
        }else {
            if (maxColor == 99 && minColor == 99) {//还未找到最高品质材料时，更高品的装备 显示默认最小概率
                tmpColorPro[color] = m_defPro[color];
                cntPro -= tmpColorPro[color];
                m_colorNum[color] = 0;//编号为0
            }
            else { //已经找到 比当前品质高的材料了，所以当前品质概率为0
                tmpColorPro[color] = 0;
                
                m_colorNum[color] = TmpCNum;
                TmpCNum++;
            }
        }
    }
    
    if (minColor == maxColor) {
        tmpColorPro[maxColor] = cntPro;
    } else {
        //按照权重计算
        float sumWeight = 0;
        map<int, float> curWeightMap;
        for (int c=GOLDEN; c>=WHITE; c--)
        {
            if (m_colorNum[c]>0)
            {
                
                int tmpSum = 0;
                int tmpC = c;
                while (tmpC >= WHITE) {
                    tmpSum += tmpColorMap[tmpC];
                    tmpC--;
                }
                
                int tmpCCNum = m_colorNum[c];
                float tmpCCSum = 0.0;
                while (tmpCCNum >= 1) {
                    tmpCCSum += m_colorPro[tmpCCNum-1];
                    tmpCCNum--;
                }
                
                curWeightMap[c] = tmpColorMap[c]+tmpSum*tmpCCSum;
                sumWeight += curWeightMap[c];
            }
            else
            {
                curWeightMap[c] = 0;
            }
        }
        
        float lastPro = cntPro;
        for (int j=maxColor; j>=minColor; j--) {
            if (j==minColor) {
                tmpColorPro[j] = lastPro;
            }
            else {
                int tmpProTT = cntPro*100.0*curWeightMap[j]/sumWeight;
                tmpColorPro[j] = tmpProTT*1.0/100;
                lastPro -= tmpColorPro[j];
            }
        }
        
    }
    
    vector<float> ret;
    ret.push_back(tmpColorPro[WHITE]);
    ret.push_back(tmpColorPro[GREEN]);
    ret.push_back(tmpColorPro[BLUE]);
    ret.push_back(tmpColorPro[PURPLE]);
    ret.push_back(tmpColorPro[ORANGE]);
    ret.push_back(tmpColorPro[GOLDEN]);
    return ret;
}

void EquipmentController::MakeEffectValue()
{
    equipSciValueMap.clear();
    EquipEffMap.clear();
    map<string, int>::iterator it = m_MyOnEquip.begin();
    for (; it!=m_MyOnEquip.end() ; it++)
    {
        auto& eInfo = EquipmentInfoMap[it->second];
        if(eInfo.site==6){
            eInfo = LongEquipmentInfoMap[it->first];
        }
        for (int i=1; i<=5; i++)
        {
            if (eInfo.values.find(i) != eInfo.values.end() && eInfo.values[i]>0) {
                for (int j=0; j<eInfo.parasMap[i].size(); j++) {
                    int eNum = eInfo.parasMap[i][j];
                    EquipEffMap[eNum] += eInfo.values[i];
                }
            }
        }
    }
}

//播放战斗力加成特效
void EquipmentController::showPowerChangeEff(string oldEquip, string curEquip)
{
    if (oldEquip == "" && curEquip == "") {
        return;
    }
    double oldPower = 0;
    double newPower = 0;
    if (curEquip != "") {
        newPower = EquipmentInfoMap[m_MyOnEquip[curEquip]].power;
    }
    if (oldEquip != "") {
        oldPower = EquipmentInfoMap[m_MyEquipmentMap[oldEquip]].power;
    }
    double exc = newPower - oldPower;
    GlobalData::shared()->playerInfo.equipPower += exc;
    UIComponent::getInstance()->setUserData();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(EQUIP_POWER_EFFECT_SHOW, CCDouble::create(exc));
}
//提示可打造并穿戴装备
bool EquipmentController::isShowEquipTips(int site){
    bool show=false;
    //铁匠铺锻造处于空闲状态
    int tqid = QueueController::getInstance()->getMinTimeQidByType(TYPE_FORGE);
    if (tqid != QID_MAX) {
        return show;
    }
    for(int i=0;i<6;i++){
        if (site!=-1) {
            i=site;
        }
        //领主有为空的装备栏位
        bool equipOn=false;
        for (map<string, int>::iterator it = EquipmentController::getInstance()->m_MyOnEquip.begin(); it != EquipmentController::getInstance()->m_MyOnEquip.end(); it++) {
            auto& info = EquipmentController::getInstance()->EquipmentInfoMap[it->second];
            if (info.site==i) {
                equipOn=true;
                break;
            }
        }
        if (equipOn) {
            if (site!=-1) {
                break;
            }
            continue;
        }
        //装备栏位为空并且从未穿戴过装备（记录栏位穿戴状态：穿戴过、未穿戴过）
        string udString = "user_had_equip_";
        udString+=CC_ITOA(i);
        bool flag0 = CCUserDefault::sharedUserDefault()->getBoolForKey(udString.c_str(),false);
        if (flag0) {
            if (site!=-1) {
                break;
            }
            continue;
        }
        //储物箱中没有装备栏位对应的1级装备
        bool flag1=false;
        for (map<string, int>::iterator it = EquipmentController::getInstance()->m_MyEquipmentMap.begin(); it!=EquipmentController::getInstance()->m_MyEquipmentMap.end(); it++)
        {
            auto& tmpInfo = EquipmentController::getInstance()->EquipmentInfoMap[it->second];
            if(tmpInfo.showType != "no" && tmpInfo.site==i && tmpInfo.level==1)
            {
                flag1=true;
                break;
            }
        }
        if (flag1) {
            if (site!=-1) {
                break;
            }
            continue;
        }
        //装备栏位对应的1级装备所需锻造材料齐全
        bool flag2=false;
        int buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_FORGE);
        if (buildId>0) {
            EquipmentController::getInstance()->CheckSelEquipMate(i, -1);
            vector<int> tmp1 ;
            EquipmentController::getInstance()->SelEquipmentsByType(tmp1, i, -1);
            for (int j = 0; j < tmp1.size(); ++j) {
              //  auto& info = EquipmentController::getInstance()->EquipmentInfoMap[tmp1[j]];
                map<int, EquipmentInfo> ::iterator itor = EquipmentInfoMap.find(tmp1[j]);
                if( itor == EquipmentInfoMap.end() )
                {
                    continue;
                }
                auto& info = itor->second;
                
                if (info.level==1 && info.showType != "no" && info.canCrt) {
                    flag2=true;
                    break;
                }
            }
        }
        if (!flag2) {
            if (site!=-1) {
                break;
            }
            continue;
        }
        //符合条件
        show=true;
        break;
    }
    return show;
}

//获取锻造界面广播记录
void EquipmentController::getScrollInfo(){
    if (GlobalData::shared()->scrollEquipInfo.size()<1
        &&GlobalData::shared()->scrollMaterialInfo.size()<1
        &&GlobalData::shared()->scrollPrayInfo.size()<1) {
        getScrollEquipInfoCommand* cmd = new getScrollEquipInfoCommand();
        cmd->sendAndRelease();
    }else{
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("EquipSiteViewSetScroll");
    }
}
void EquipmentController::getScrollInfoEnd(CCDictionary* dict){
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else
    {
        if (dict->objectForKey("broadcast")) {
            GlobalData::shared()->scrollEquipInfo.clear();
            GlobalData::shared()->scrollMaterialInfo.clear();
            GlobalData::shared()->scrollPrayInfo.clear();
            CCArray *arr = dynamic_cast<CCArray*>(dict->objectForKey("broadcast"));
            for (int i=0; i<arr->count(); i++) {
                string str = dynamic_cast<CCString*>(arr->objectAtIndex(i))->getCString();
                insertScrollInfo(str);
            }
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("EquipSiteViewSetScroll");
    }
}
void EquipmentController::insertScrollInfo(string info,bool force){
    string str = info;
    vector<string> vec;
    CCCommonUtils::splitString(str, "|", vec);
    if (vec.size()==3 && vec[1]=="0") {//装备
        string equipInfo = "equipInfo:";
        string equipName = _lang(CCCommonUtils::getPropById(vec[2], "name"));
        string color = GetColorStr(atoi(CCCommonUtils::getPropById(vec[2], "color").c_str()));
        equipName+="【";
        equipName+=color;
        equipName+="】";
        equipInfo=_lang_2("111666", vec[0].c_str(), equipName.c_str());
        
        if (force) {
            GlobalData::shared()->scrollEquipInfo.insert(GlobalData::shared()->scrollEquipInfo.begin(), equipInfo);
            if (GlobalData::shared()->scrollEquipInfo.size()>3) {
                GlobalData::shared()->scrollEquipInfo.pop_back();
            }
        }else if (GlobalData::shared()->scrollEquipInfo.size()<4) {
            GlobalData::shared()->scrollEquipInfo.push_back(equipInfo);
        }
        
    }else if (vec.size()==4 && vec[1]=="0"){//装备
        string equipInfo = "equipInfo1:";
        string equipName = _lang(CCCommonUtils::getPropById(vec[2], "name"));
        string color = GetColorStr(atoi(CCCommonUtils::getPropById(vec[2], "color").c_str()));
        equipName+="【";
        equipName+=color;
        equipName+="】";
        equipInfo=_lang_2("111666", vec[0].c_str(), equipName.c_str());
        
        if (force) {
            GlobalData::shared()->scrollEquipInfo.insert(GlobalData::shared()->scrollEquipInfo.begin(), equipInfo);
            if (GlobalData::shared()->scrollEquipInfo.size()>3) {
                GlobalData::shared()->scrollEquipInfo.pop_back();
            }
        }else if (GlobalData::shared()->scrollEquipInfo.size()<4) {
            GlobalData::shared()->scrollEquipInfo.push_back(equipInfo);
        }
        
    }else if (vec.size()==3 && vec[1]=="1"){//材料
        string materialInfo = "materialInfo:";
        string materialName = _lang(CCCommonUtils::getPropById(vec[2], "name"));
        string color = GetColorStr(atoi(CCCommonUtils::getPropById(vec[2], "color").c_str()));
        materialName+="【";
        materialName+=color;
        materialName+="】";
        materialInfo=_lang_2("111667", vec[0].c_str(), materialName.c_str());
        
        if (force) {
            GlobalData::shared()->scrollMaterialInfo.insert(GlobalData::shared()->scrollMaterialInfo.begin(), materialInfo);
            if (GlobalData::shared()->scrollMaterialInfo.size()>3) {
                GlobalData::shared()->scrollMaterialInfo.pop_back();
            }
        }else if (GlobalData::shared()->scrollMaterialInfo.size()<4) {
            GlobalData::shared()->scrollMaterialInfo.push_back(materialInfo);
        }
        
    }else if (vec.size()==3 && vec[1]=="2"){//许愿 钢材
        string paryInfo = "paryInfo:";
        paryInfo=_lang_2("111668", vec[0].c_str(), vec[2].c_str());
        
        if (force) {
            GlobalData::shared()->scrollPrayInfo.insert(GlobalData::shared()->scrollPrayInfo.begin(), paryInfo);
            if (GlobalData::shared()->scrollPrayInfo.size()>3) {
                GlobalData::shared()->scrollPrayInfo.pop_back();
            }
        }else if (GlobalData::shared()->scrollPrayInfo.size()<4) {
            GlobalData::shared()->scrollPrayInfo.push_back(paryInfo);
        }
        
    }
}


string EquipmentController::GetColorStr(int color)
{
    string ret = "";
    switch (color) {
        case 0:
            ret="119043";
            break;
        case 1:
            ret="119044";
            break;
        case 2:
            ret="119045";
            break;
        case 3:
            ret="119046";
            break;
        case 4:
            ret="119047";
            break;
        case 5:
            ret="119048";
            break;
        case 6:
            ret="119049";
            break;
            
        default:
            break;
    }
    return _lang(ret);
}

//设置锻造界面广播开关
void EquipmentController::setScrollEquipSwitch(int sw){
    setScrollEquipSwitchCommand* cmd = new setScrollEquipSwitchCommand(sw);
    cmd->sendAndRelease();
}

string EquipmentController::GetMateNameById(int mateId)
{
    string ret = "";
    if (mateId == 201010 ) {
        ret = "119284";
    }if (mateId == 201020 ) {
        ret = "119285";
    }if (mateId == 201030 ) {
        ret = "119286";
    }if (mateId == 201040 ) {
        ret = "119287";
    }if (mateId == 201050 ) {
        ret = "119288";
    }if (mateId == 201060 ) {
        ret = "119289";
    }if (mateId == 201070 ) {
        ret = "119290";
    }if (mateId == 201080 ) {
        ret = "119291";
    }if (mateId == 201090 ) {
        ret = "119292";
    }if (mateId == 201100 ) {
        ret = "119293";
    }if (mateId == 201110 ) {
        ret = "119294";
    }if (mateId == 201120 ) {
        ret = "119295";
    }
    return _lang(ret);
}

//洗练龙晶
bool EquipmentController::startMeltEquip(string uuid,int type)
{
    EquipMeltCommand * cmd = new EquipMeltCommand(uuid, type);
    cmd->sendAndRelease();
    return true;
}
void EquipmentController::retMeltEquip(CCDictionary* dict)
{
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else
    {
//        (utf_string) c: dragonshard.refine
//        (sfs_object) p:
//        (long) gold: 9927393
//        (sfs_object) equip:
    //        (utf_string) itemId: 1101604
    //        (utf_string) uuid: a062396d490f4b61aa04769dfae4cac3
    //        (utf_string) extraProperty: 50@51@52@53,35.35,103035|156,15.26,113845|
    //        (int) on: 0
        
        if(dict->objectForKey("gold")){
            int gold = dict->valueForKey("gold")->intValue();
            UIComponent::getInstance()->updateGold(gold);
        }
        
        if(dict->objectForKey("times")){
            int refineTimes = dict->valueForKey("times")->intValue();
            m_refineTimes = refineTimes;
        }
        
        if(dict->objectForKey("equip")){
            auto equip = _dict(dict->objectForKey("equip"));
            string itemId = equip->valueForKey("itemId")->getCString();
            string uuid = equip->valueForKey("uuid")->getCString();
            string extraProperty = equip->valueForKey("extraProperty")->getCString();
            string refineProperty = equip->valueForKey("refineProperty")->getCString();
            int on = equip->valueForKey("on")->intValue();
            if ( LongEquipmentInfoMap.find(uuid)!=LongEquipmentInfoMap.end() )
            {
                LongEquipmentInfoMap[uuid].setLongRefinePro(refineProperty);
            }
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(EQUIP_MELT_SHOW, NULL);
    }
}
bool EquipmentController::startSaveMeltEquip(string uuid)
{
    EquipMeltSaveCommand * cmd = new EquipMeltSaveCommand(uuid);
    cmd->sendAndRelease();
    return true;
}
void EquipmentController::retSaveMeltEquip(CCDictionary* dict)
{
    if(dict->objectForKey("errorCode"))
    {
        string error = dict->valueForKey("errorCode")->getCString();
        CCCommonUtils::flyText(_lang(error.c_str()));
    }
    else
    {
        if(dict->objectForKey("equip")){
            auto equip = _dict(dict->objectForKey("equip"));
            string itemId = equip->valueForKey("itemId")->getCString();
            string uuid = equip->valueForKey("uuid")->getCString();
            string extraProperty = equip->valueForKey("extraProperty")->getCString();
            string refineProperty = equip->valueForKey("refineProperty")->getCString();
            int on = equip->valueForKey("on")->intValue();
            
            if ( LongEquipmentInfoMap.find(uuid)!=LongEquipmentInfoMap.end() )
            {
                LongEquipmentInfoMap[uuid].setLongProperty(extraProperty);
                LongEquipmentInfoMap[uuid].setLongRefinePro(refineProperty);
                if (on == 1) {
                    MakeEffectValue();
                }
            }
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(EQUIP_MELT_SHOW, NULL);
    }
}

bool EquipmentController::IsHaveLongjing(int lv, int color)
{
    map<string, EquipmentInfo>::iterator it = LongEquipmentInfoMap.begin();
    for (; it!= LongEquipmentInfoMap.end(); it++)
    {
        if (it->second.level == lv && it->second.color>=color) {
            return true;
        }
    }
    return false;
}

bool EquipmentController::IsOpenMeltFunc()
{
    if (m_meltKey == "0") {
        return false;
    }else if (m_meltKey == "1") {
        return true;
    }else if (m_meltKey != "") {
        if (CCCommonUtils::checkVersion(m_meltKey)) {
            return true;
        }else {
            return false;
        }
    }else {
        return true;
    }
}

vector< pair<Color4F, Color4F> > EquipmentController::getParticleColor(int color)
{
    vector< pair<Color4F, Color4F> > retVec;
    if (color == 1)
    {
        retVec.push_back( pair<Color4F, Color4F>(ccc4f(0.59, 0.75, 0, 0), ccc4f(0, 0, 0, 0)) );
        retVec.push_back( pair<Color4F, Color4F>(ccc4f(1.0, 0.82, 0.46, 1.0), ccc4f(0, 0, 0, 0)) );
        retVec.push_back( pair<Color4F, Color4F>(ccc4f(0.65, 1.0, 0, 1.0), ccc4f(0, 0, 0, 0)) );
    }
    else if (color == 2)
    {
        retVec.push_back( pair<Color4F, Color4F>(ccc4f(0, 0.54, 0.8, 0), ccc4f(0, 0, 0, 0)) );
        retVec.push_back( pair<Color4F, Color4F>(ccc4f(0, 1.0, 0.95, 1.0), ccc4f(0, 0, 0, 0)) );
        retVec.push_back( pair<Color4F, Color4F>(ccc4f(0.54, 0.36, 1.0, 1.0), ccc4f(0, 0, 0, 0)) );
    }
    else if (color == 3)
    {
        retVec.push_back( pair<Color4F, Color4F>(ccc4f(0.65, 0.2, 0.8, 0), ccc4f(0, 0, 0, 0)) );
        retVec.push_back( pair<Color4F, Color4F>(ccc4f(1.0, 0.58, 0.84, 1.0), ccc4f(0, 0, 0, 0)) );
        retVec.push_back( pair<Color4F, Color4F>(ccc4f(0.54, 0.36, 1.0, 1.0), ccc4f(0, 0, 0, 0)) );
    }
    else if (color == 4)
    {
        retVec.push_back( pair<Color4F, Color4F>(ccc4f(0.84, 0.4, 0, 0), ccc4f(0, 0, 0, 0)) );
        retVec.push_back( pair<Color4F, Color4F>(ccc4f(1.0, 0.6, 0.36, 1.0), ccc4f(0, 0, 0, 0)) );
        retVec.push_back( pair<Color4F, Color4F>(ccc4f(1.0, 0.35, 0, 1.0), ccc4f(0, 0, 0, 0)) );
    }
    else if (color == 5)
    {
        retVec.push_back( pair<Color4F, Color4F>(ccc4f(0.75, 0.36, 0, 0), ccc4f(0, 0, 0, 0)) );
        retVec.push_back( pair<Color4F, Color4F>(ccc4f(1.0, 0.82, 0.46, 1.0), ccc4f(0, 0, 0, 0)) );
        retVec.push_back( pair<Color4F, Color4F>(ccc4f(1.0, 0.82, 0, 1.0), ccc4f(0, 0, 0, 0)) );
    }
    return retVec;
}

int EquipmentController::getCountBoreanById(int equipId)
{
    int ret = 0;
    auto&curEquip = EquipmentInfoMap[equipId];
    
    map<string, int>::iterator it = m_MyEquipmentMap.begin();
    for (; it!=m_MyEquipmentMap.end(); it++)
    {
        auto& info = EquipmentInfoMap[it->second];
        if (info.universal==1 && info.level==curEquip.level && info.color<=curEquip.color)
        {
            ret += pow(4, info.color-1);
        }
    }
    return ret;
}

//获取装备科技作用值加成

float EquipmentController::getEffectSciValueByNum(int effNum)
{
    if (equipSciValueMap.find(effNum) != equipSciValueMap.end()) {// && equipSciValueMap[effNum]>0
        return equipSciValueMap[effNum];
    }
    
    float ret = 0.f;
    map<int, int>::iterator it = equipSci_site.begin();
    for (; it != equipSci_site.end(); it++)
    {
        float fEffectValue = 0.f;
        if(  GlobalData::shared()->effectValues.find(it->first) !=  GlobalData::shared()->effectValues.end() ) {
            fEffectValue = GlobalData::shared()->effectValues[it->first];
        }else {
            continue;
        }
        
        int site = it->second;
        if (site !=-1 && equipSci_eff[it->first].find(effNum) == equipSci_eff[it->first].end() )
        {
            continue;
        }
    
        if (site != -1)
        {
            string eUuid = FindOnEquipBySite(site);
            if (eUuid != "") {
                auto& eInfo = EquipmentInfoMap[m_MyOnEquip[eUuid]];
                if(eInfo.site==6){
                    eInfo = LongEquipmentInfoMap[eUuid];
                }
                for (int i=1; i<=5; i++)
                {
                    if (eInfo.values.find(i) != eInfo.values.end() && eInfo.values[i]>0) {
                        for (int j=0; j<eInfo.parasMap[i].size(); j++)
                        {
                            if (effNum == eInfo.parasMap[i][j]) {
                                ret += eInfo.values[i] * (fEffectValue/100);
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if (EquipEffMap.find(effNum) != EquipEffMap.end() ) {
                ret += EquipEffMap[effNum]*(fEffectValue/100);
            }
        }
    }
    equipSciValueMap[effNum] = ret;
    return ret;
}

void EquipmentController::reSetEquipSci(int effNum, bool force)
{
    if (force || equipSci_site.find(effNum) != equipSci_site.end()) {
        equipSciValueMap.clear();
    }
}