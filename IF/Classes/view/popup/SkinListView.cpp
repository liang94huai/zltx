//
//  SkinListView.cpp
//  IF
//
//  Created by 邢晓瑞 on 15/11/18.
//
//

#include "SkinListView.hpp"
#include "ToolController.h"
#include "ImperialScene.h"
#include "SceneController.h"
#include "DynamicResourceController.h"
#include "WorldController.h"
#include "PopupViewController.h"
#include "YesNoDialog.h"

bool sortByOrderNum(const string& s1, const string& s2)
{
    auto& info1 = ToolController::getInstance()->getToolInfoById(atoi(s1.c_str()));
    auto& info2 = ToolController::getInstance()->getToolInfoById(atoi(s2.c_str()));
    if (info1.orderNum < info2.orderNum)
    {
        return true;
    } else {
        return false;
    }
}

SkinListView* SkinListView::create()
{
    auto ret = new SkinListView();
    if (ret && ret->init())
    {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
bool SkinListView::init()
{
    ArcPopupBaseView::init(TYPE_POS_UP);
    setIsHDPanel(true);
    auto size = Director::getInstance()->getWinSize();
    setContentSize(size);
    CCBLoadFile("SkinListView", this, this);
    
    float add = size.height - 852;
    if (CCCommonUtils::isIosAndroidPad())
    {
        add = size.height - 2048;
        add = add / 2.4;
    }
    m_bottomNode->setPositionY(m_bottomNode->getPositionY() - add / 4);
    
    m_iconNames.clear();
    m_itemIds.clear();
    m_names.clear();
    m_deses.clear();
    m_skinNames.clear();
    m_curIdx = 0;
    m_curItemId = "";
    m_isByBuy = false;
    
    //永久皮肤
    if (ToolController::getInstance()->haveSkin())
    {
        string skinStr = GlobalData::shared()->playerInfo.skin;
        vector<string> skinVec;
        CCCommonUtils::splitString(skinStr, ";", skinVec);
        for (auto it = skinVec.begin(); it != skinVec.end(); ++it)
        {
            string effectart = CCCommonUtils::getPropById((*it).c_str(), "effectart");
            vector<string> vec;
            CCCommonUtils::splitString(effectart, ";", vec);
            if(vec.size()==2){
                permanentSkins[(*it)] = vec[1];
            }
        }
    }
    
    int showPos = -1;
    string curSkin = "";
    if (ToolController::getInstance()->canChangeSkin())
    {
        curSkin = ToolController::getInstance()->getMaincitySkin();
    }
    ToolController::getInstance()->m_typeItems[100] = CCCommonUtils::getTypeArray(100);
    __Array* arr = ToolController::getInstance()->m_typeItems[100];
    auto arr1 = __Array::create();
    Ref* ref = nullptr;
    CCARRAY_FOREACH(arr, ref)
    {
        __Dictionary* dic = dynamic_cast<__Dictionary*>(ref);
        if (dic->objectForKey("icon")) {
            string iconName = dic->valueForKey("icon")->getCString();
            if (dic->objectForKey("id") && !iconName.empty()) {
                string itemId = dic->valueForKey("id")->getCString();
                if (!itemId.empty() && ToolController::getInstance()->m_toolInfos.find(atoi(itemId.c_str())) != ToolController::getInstance()->m_toolInfos.end()) {
                    ToolInfo& info = ToolController::getInstance()->m_toolInfos[atoi(itemId.c_str())];
                    if (info.price > 0  || info.getCNT() > 0) {
                        if (find(m_iconNames.begin(), m_iconNames.end(), iconName) == m_iconNames.end())
                        {
                            string cellName = CCCommonUtils::getPropById(itemId, "para");
                            vector<string> vec;
                            CCCommonUtils::splitString(cellName, ";", vec);
                            if (vec.size() < 2)
                            {
                                continue;
                            }
                            string skinId = CCCommonUtils::getPropById(itemId, "para1");
                            if (skinId.empty() || permanentSkins.find(skinId)!=permanentSkins.end() || info.color == 5)
                            {
                                continue;
                            }
                            if (ToolController::getInstance()->m_customSkinGroup.find(atoi(skinId.c_str())) == ToolController::getInstance()->m_customSkinGroup.end())
                            {
                                continue;
                            }
                            auto& skinInfo = ToolController::getInstance()->m_customSkinGroup[atoi(skinId.c_str())];
                            if (!DynamicResourceController::getInstance()->checkMainCityResource(skinInfo.skin, skinInfo.md5)) {
                                continue;
                            }
                            m_names.push_back(_lang(vec[0]));
                            m_deses.push_back(_lang(vec[1]));
                            m_iconNames.push_back(iconName);
                            m_skinNames.push_back(skinInfo.skin);
                            if (curSkin == skinInfo.skin)
                            {
                                showPos = m_iconNames.size() - 1;
                            }
                        }
                        m_itemIds[iconName].push_back(itemId);
                    }
                }
            }
        }
    }
    if (ToolController::getInstance()->canChangeSkin())
    {
        m_iconNames.push_back("pic400000_2");
        m_names.push_back(_lang("101474"));
        m_deses.push_back(_lang("101475"));
        m_itemIds["pic400000_2"].push_back("88");
        m_skinNames.push_back("mainCity");
    }
//    //永久皮肤
//    if (ToolController::getInstance()->haveSkin())
//    {
//        string skinStr = GlobalData::shared()->playerInfo.skin;
//        vector<string> skinVec;
//        CCCommonUtils::splitString(skinStr, ";", skinVec);
//        for (auto it = skinVec.begin(); it != skinVec.end(); ++it)
//        {
//            string effectart = CCCommonUtils::getPropById((*it).c_str(), "effectart");
//            vector<string> vec;
//            CCCommonUtils::splitString(effectart, ";", vec);
//            if(vec.size()==2){
//                string faceStr = vec[1];
//                string icon_face = "icon_"+faceStr;
//                m_iconNames.push_back(icon_face);
//                m_names.push_back(_lang("101474"));//101474=经典
//                m_deses.push_back(_lang("101475"));//101475=开启后，主城场景中和世界场景中的城堡外形改变为经典外形
//                permanentSkins[(*it)] = icon_face;
//                m_itemIds[icon_face].push_back((*it));
//                m_skinNames.push_back(faceStr);
//            }
//        }
//    }
    //永久皮肤
    map<string, string>::iterator skinIt = permanentSkins.begin();
    for (; skinIt!=permanentSkins.end() ; skinIt++)
    {
        if(ToolController::getInstance()->getMaincitySkin() == skinIt->second){
            continue;
        }
        string faceStr = skinIt->second;
        string icon_face = "icon_"+faceStr;
        m_iconNames.push_back(icon_face);
        m_itemIds[icon_face].push_back(skinIt->first);
        m_skinNames.push_back(faceStr);
        
        CCARRAY_FOREACH(arr, ref)
        {
            __Dictionary* dic = dynamic_cast<__Dictionary*>(ref);
            string itemId = dic->valueForKey("id")->getCString();
            string skinId = CCCommonUtils::getPropById(itemId, "para1");
            if(skinId == skinIt->first){
                string nameStr = CCCommonUtils::getPropById(itemId, "name");
                string desStr = CCCommonUtils::getPropById(itemId, "description");
                m_names.push_back(_lang(nameStr.c_str()));
                m_deses.push_back(_lang(desStr.c_str()));
                break;
            }
            
        }
    }

    
    if (m_iconNames.size() == 0)
    {
        return false;
    }
    if (showPos < 0)
    {
        showPos = m_iconNames.size() - 1;
    }
    if (!checkData())
    {
        return false;
    }
    for (int i = 0; i < m_iconNames.size(); ++i) {
        string zhanshi = "";
        if (m_iconNames[i] != "pic400000_2") {
            zhanshi = "zhanshi_";
            zhanshi = zhanshi + m_iconNames[i] + ".png";
        } else {
            zhanshi = "pic400000_2.png";
        }
        ArcInfo* info = new ArcInfo(i, m_names[i], zhanshi);
        arr1->addObject(info);
        info->release();
    }
    m_arcScroll = ArcScrollView::create(arr1, 2, showPos);
    m_arcScroll->setCallback(this, callfunc_selector(SkinListView::arcScollCallBack));
    m_arcNode->addChild(m_arcScroll);
    
    m_curIdx = showPos;
    refresh(showPos);
    
    return true;
}

void SkinListView::arcScollCallBack()
{
    refresh(m_arcScroll->m_currentIndex);
}

void SkinListView::onGetMsgToolChange(Ref* ref)
{
    refresh(m_curIdx);
}

void SkinListView::refresh(int idx)
{
    if (m_iconNames.size() <= 0 || idx >= m_iconNames.size()) {
        return;
    }
    m_curIdx = idx;
    m_curItemId = "";
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TMP_SKIN_CHANGE, __String::create(m_skinNames[idx]));
    m_desLabel->setString(m_deses[m_curIdx]);
    onEnterFrame(0);
    refreshDayCells();
}

void SkinListView::onGetMsgDayCellSelect(cocos2d::Ref * ref)
{
    if (!checkData()) {
        return;
    }
    if (!ref) {
        return;
    }
    __String* ccstr = dynamic_cast<__String*>(ref);
    if (!ccstr) {
        return;
    }
    string itemId = ccstr->getCString();
    if (std::find(m_itemIds[m_iconNames[m_curIdx]].begin(), m_itemIds[m_iconNames[m_curIdx]].end(), itemId) == m_itemIds[m_iconNames[m_curIdx]].end()) {
        m_curItemId = "";
    } else {
        m_curItemId = itemId;
    }
    refreshBtn();
}

void SkinListView::onGetMsgDayCellUnselect(cocos2d::Ref * ref)
{
    m_curItemId = "";
    refreshBtn();
}

void SkinListView::refreshBtn()
{
    m_buyNode->setVisible(true);
    m_goldNode->setVisible(true);
    if (m_curItemId.empty()) {
        m_buyNode->setVisible(false);
    } else {
        if (m_curItemId == "88" || permanentSkins.find(m_curItemId)!=permanentSkins.end()) {
            m_goldNode->setVisible(false);
            m_okLabel->setString(_lang("102137"));
            m_okLabel->setPositionY(0);
        } else {
            if (ToolController::getInstance()->m_toolInfos.find(atoi(m_curItemId.c_str())) == ToolController::getInstance()->m_toolInfos.end()) {
                m_buyNode->setVisible(false);
                return;
            }
            auto & info = ToolController::getInstance()->m_toolInfos[atoi(m_curItemId.c_str())];
            if (info.getCNT() > 0) {
                m_buyNode->setVisible(true);
                m_goldNode->setVisible(false);
                m_okLabel->setString(_lang("102137"));
                m_okLabel->setPositionY(0);
            } else if (info.price > 0) {
                m_buyNode->setVisible(true);
                m_goldNode->setVisible(true);
                m_okLabel->setString(_lang("104906"));
                m_okLabel->setPositionY(9);
                m_goldNum->setString(CC_CMDITOA(info.price));
            } else {
                m_buyNode->setVisible(false);
            }
        }
    }
}

void SkinListView::refreshTime()
{
    
}

void SkinListView::refreshDayCells()
{
    m_dayNode->removeAllChildrenWithCleanup(true);
    bool isPermanent = false;
    map<string, string>::iterator it = permanentSkins.begin();
    for (; it!=permanentSkins.end() ; it++)
    {
        string icon_face = "icon_"+it->second;
        if (icon_face == m_iconNames[m_curIdx]) {
            isPermanent = true;
            m_curItemId = it->first;
            break;
        }
    }
    if(!isPermanent){
        if (m_iconNames[m_curIdx] == "pic400000_2") {
            m_curItemId = "88";
        }else {
            vector<string> validIds;
            getValidIds(validIds);
            if (validIds.empty()) {
                m_curItemId = "";
            } else {
                string ownId = getOwnId(validIds);
                if (ownId.empty()) {
                    m_curItemId = *(validIds.begin());
                } else {
                    m_curItemId = ownId;
                }
            }
            for (int i = 0; i < validIds.size(); ++i) {
                float dw = 600 / validIds.size();
                auto cell = SkinListDayCell::create(validIds[i], validIds[i] == m_curItemId);
                cell->setPositionX(-300 + i * dw + dw / 2.0);
                cell->setPositionY(0);
                m_dayNode->addChild(cell);
            }
        }
    }
    refreshBtn();
}

string SkinListView::getOwnId(vector<string> & vec)
{
    if (vec.empty()) {
        return "";
    }
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        if (*it == "88" || permanentSkins.find((*it))!=permanentSkins.end() ) {
            continue;
        }
        int toolid = atoi((*it).c_str());
        auto& info = ToolController::getInstance()->m_toolInfos[toolid];
        if (info.getCNT() > 0) {
            return (*it);
        }
    }
    return "";
}

void SkinListView::getValidIds(vector<string> & vec)
{
    vector<string> idvec;
    int ownTypeCnt = 0;
    for (auto it = m_itemIds[m_iconNames[m_curIdx]].begin(); it != m_itemIds[m_iconNames[m_curIdx]].end(); ++it)
    {
        auto& info = ToolController::getInstance()->getToolInfoById(atoi((*it).c_str()));
        if (info.getCNT() > 0 ) {
            idvec.push_back(*it);
        }
    }
    
    for (auto it = m_itemIds[m_iconNames[m_curIdx]].begin(); it != m_itemIds[m_iconNames[m_curIdx]].end(); ++it)
    {
        auto& info = ToolController::getInstance()->getToolInfoById(atoi((*it).c_str()));
        if (info.getCNT() <= 0 && info.price > 0) {
            if (idvec.size() < 3)
            {
                idvec.push_back(*it);
            }
        }
    }
    
    //排序
    std::sort(idvec.begin(), idvec.end(), sortByOrderNum);
    vec.clear();
    if (idvec.size() <= 3)
    {
        for (auto it = idvec.begin(); it != idvec.end(); ++it) {
            vec.push_back(*it);
        }
    } else {
        for (int i = idvec.size() - 3; i < idvec.size(); ++i) {
            vec.push_back(idvec[i]);
        }
    }
}

void SkinListView::onEnterFrame(float t)
{
    map<int, CCDictionary* >::iterator it = ToolController::getInstance()->m_statusItems.find(100);
    int time = -1;
    if (it != ToolController::getInstance()->m_statusItems.end()) {
        time = (it->second->valueForKey("endTime")->doubleValue() - WorldController::getInstance()->getTime()) / 1000;
    }
    if (ToolController::getInstance()->getMaincitySkin() != m_skinNames[m_curIdx])
    {
        time = -1;
    }
    if (time > 0)
    {
        m_timeNode->setVisible(true);
        m_timeLabel->setString(CC_SECTOA(time));
    } else {
        m_timeNode->setVisible(false);
        m_timeLabel->setString("");
    }
}

void SkinListView::onGetMsgSkinConfigInit(Ref* ref)
{
    PopupViewController::getInstance()->goBackPopupView();
}

void SkinListView::onEnter()
{
    Node::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SkinListView::onGetMsgDayCellSelect), MSG_DAYCELL_SELECT, nullptr);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SkinListView::onGetMsgDayCellUnselect), MSG_DAYCELL_UNSELECT, nullptr);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SkinListView::onGetMsgToolChange), MSG_TOOL_CHANGE, nullptr);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SkinListView::onGetMsgSkinConfigInit), MSG_SKIN_CONFIG_INIT, nullptr);
    Director::getInstance()->getScheduler()->scheduleSelector(schedule_selector(SkinListView::onEnterFrame), this, 1, kRepeatForever, 0, false);
    setTouchEnabled(false);
    refresh(m_curIdx);
    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    if (layer) {
        layer->setUnMoveScence(false);
        layer->onMoveToBuildAndOpen(400000000, TYPE_POS_UP);
    }
}
void SkinListView::onExit()
{
    Director::getInstance()->getScheduler()->unscheduleSelector(schedule_selector(SkinListView::onEnterFrame), this);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_DAYCELL_SELECT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_DAYCELL_UNSELECT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TOOL_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SKIN_CONFIG_INIT);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFRESH_BUILD, __String::create("400000000"));
    Node::onExit();
}

bool SkinListView::onAssignCCBMemberVariable(Ref * pTarget, const char * pMemberVariableName, Node * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", Node*, m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", Scale9Sprite*, m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dayNode", Node*, m_dayNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_okBtn", ControlButton*, m_okBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_okLabel", CCLabelIF*, m_okLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arcNode", Node*, m_arcNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldNode", Node*, m_goldNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldNum", CCLabelIF*, m_goldNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyNode", Node*, m_buyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeNode", Node*, m_timeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    return false;
}

SEL_CCControlHandler SkinListView::onResolveCCBCCControlSelector(Ref * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOkClick", SkinListView::onOkClick);
    return nullptr;
}
bool SkinListView::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    return true;
}
void SkinListView::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    
}

void SkinListView::onOkClick(Ref* pSender, Control::EventType pControlEvent)
{
    m_isByBuy = false;
    if (m_curItemId.empty())
    {
        return;
    }
    if (m_curItemId == "88")
    {
        //101499=领主大人，使用经典城堡装扮将取消现在的城堡装扮，且不会返还任何金币，是否继续？
        YesNoDialog::show(_lang("101499").c_str(), CCCallFunc::create(this, callfunc_selector(SkinListView::sureToCancel)));
    }else if(permanentSkins.find(m_curItemId)!=permanentSkins.end()){
        //101499=领主大人，使用经典城堡装扮将取消现在的城堡装扮，且不会返还任何金币，是否继续？
        YesNoDialog::show(_lang("101499").c_str(), CCCallFunc::create(this, callfunc_selector(SkinListView::sureToCancel)));
    }
    else {
        auto it = ToolController::getInstance()->m_toolInfos.find(atoi(m_curItemId.c_str()));
        if (it == ToolController::getInstance()->m_toolInfos.end()) {
            return;
        }
        if ((it->second).getCNT() > 0) {
            onUseTool();
        } else {
            YesNoDialog::showButtonAndGold( _lang("104919").c_str() , CCCallFunc::create(this, callfunc_selector(SkinListView::onYes)), _lang("104906").c_str(), it->second.price);
        }
    }
}

void SkinListView::onYes()
{
    ToolController::getInstance()->buyTool(atoi(m_curItemId.c_str()), 1, CCCallFunc::create(this, callfunc_selector(SkinListView::onUseTool)),0,true,false,"UseItemStatusView");
}

void SkinListView::onUseTool()
{
    if(m_curItemId == "88" || permanentSkins.find(m_curItemId)!=permanentSkins.end()){
        sureToCancel();
    }else{
        ToolController::getInstance()->useTool(atoi(m_curItemId.c_str()),1,true,m_isByBuy);
    }
}

void SkinListView::sureToCancel()
{
    if(permanentSkins.find(m_curItemId)!=permanentSkins.end()){
        ToolController::getInstance()->useSkin(m_curItemId);
    }else{
        ToolController::getInstance()->cancelCustomSkin();
        PopupViewController::getInstance()->removeAllPopupView();
    }
}

bool SkinListView::checkData()
{
    int cnt = m_iconNames.size();
    if (cnt != m_names.size() || cnt != m_deses.size() || cnt != m_skinNames.size() || cnt != m_itemIds.size())
    {
        return false;
    }
    return true;
}

SkinListDayCell* SkinListDayCell::create(string itemId, bool isSelected)
{
    auto ret = new SkinListDayCell(itemId, isSelected);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
bool SkinListDayCell::init()
{
    CCBLoadFile("SkinListDayCell", this, this);
    setContentSize(Size(200, 50));
    m_spr->setVisible(m_isSelected);
    string des = CCCommonUtils::getPropById(m_itemId, "para2");
    m_dayLabel->setString(_lang(des));
    return true;
}
bool SkinListDayCell::onAssignCCBMemberVariable(Ref * pTarget, const char * pMemberVariableName, Node * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", Node*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spr", Sprite*, m_spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dayLabel", CCLabelIF*, m_dayLabel);
    return false;
}

bool SkinListDayCell::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    if (isTouchInside(m_touchNode, pTouch)) {
        return true;
    }
    return false;
}

void SkinListDayCell::onTouchMoved(Touch * ptouch, Event * pEvent)
{
    
}
void SkinListDayCell::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if (isTouchInside(m_touchNode, pTouch)) {
        if (!m_isSelected) {
            m_isSelected = true;
            m_spr->setVisible(true);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_DAYCELL_SELECT, __String::create(m_itemId));
        }
    }
}

void SkinListDayCell::onGetMsgOtherSelected(cocos2d::Ref *ref)
{
    if (!ref) {
        return;
    }
    __String* ccstr = dynamic_cast<__String*>(ref);
    if (!ccstr) {
        return;
    }
    string itemId = ccstr->getCString();
    if (m_itemId == itemId) {
        return;
    }
    m_isSelected = false;
    m_spr->setVisible(false);
}

void SkinListDayCell::onEnter()
{
    Node::onEnter();
    setTouchEnabled(true);
    setSwallowsTouches(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SkinListDayCell::onGetMsgOtherSelected), MSG_DAYCELL_SELECT, nullptr);
}
void SkinListDayCell::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_DAYCELL_SELECT);
    Node::onExit();
}