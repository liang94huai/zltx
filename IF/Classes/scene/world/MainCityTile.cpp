//
//  MainCityTile.cpp
//  IF
//
//  Created by 杜 威 on 16-11-18.
//
//

#include "MainCityTile.h"
#include "AllianceManager.h"
#include "ActivityController.h"
#include "ScoutInfo.h"
#include "WorldCommand.h"
#include "WorldMapView.h"
#include "YesNoDialog.h"
#include "AddFavorite.h"
#include "RoleInfoView.h"
#include "ResourceTileInfoPopUpView.h"
#include "TipsView.h"
#include "SceneController.h"
#include "SoundController.h"
#include "EnemyInfoController.h"
#include "TipsWithPicView.h"
#include "ThroneTile.h"
#include "YuanJunTipView.h"
#include "ResourceTileInfoPopUpView.h"
#include "TroopBuildingDetailView.hpp"
bool MainCityTile::init() {
    NewBaseTileInfo::init();
    std::string resName = "";
    if (m_cityInfo.resource.type<ResourceStr.size()) {//防止越界崩了
        resName = ResourceStr[m_cityInfo.resource.type];
    }
    m_title->setString(CCString::createWithFormat("%d %s ",m_cityInfo.resource.lv,_lang(resName.c_str()).c_str())->getCString());
    requestDetail();
    onDetailCallback(NULL);
    bool flag = false;
    for(auto it = WorldController::getInstance()->m_marchInfo.begin(); it != WorldController::getInstance()->m_marchInfo.end(); it++){
        if(it->second.ownerType == PlayerSelf && it->second.endPointIndex == m_cityInfo.cityIndex){
            m_cityInfo.resource.troopMax = it->second.troopMax;
            m_cityInfo.resource.digSpeed = it->second.digSpeed;
            m_cityInfo.resource.digStartTime = it->second.digStartTime;
            m_cityInfo.resource.digStartNum = it->second.digStartNum;
            m_cityInfo.resource.changeCollectSpdTime = it->second.changeCollectSpdTime;

            flag = true;
            break;
        }
    }
    if(!flag){
        // no owner
        m_cityInfo.resource.troopMax = 0;
        m_cityInfo.resource.digSpeed = 0.0;
        m_cityInfo.resource.digStartTime = 0.0;
//        m_cityInfo.playerName = "";
    }
    
    m_cityInfo.resource.sum = 0;
    if(m_cityInfo.playerName != "" && m_cityInfo.playerName != GlobalData::shared()->playerInfo.name){
        addNameNode();
    }
    updateView();
    m_isInUpdate = false;
    addToParent();

    return true;
}

void MainCityTile::onEnter(){
    NewBaseTileInfo::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void MainCityTile::onExit(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    NewBaseTileInfo::onExit();
}

void MainCityTile::requestDetail() {
    m_isInUpdate = true;
    unsigned int index = m_cityInfo.cityIndex;
    if(m_cityInfo.parentCityIndex != -1){
        index = m_cityInfo.parentCityIndex;
    }
    auto cmd = new WorldDetailCommand(index,m_cityInfo.tileServerId);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(MainCityTile::onDetailCallback), nullptr));
    cmd->sendAndRelease();
}

void MainCityTile::onDetailCallback(cocos2d::CCObject *obj) {
    m_isInUpdate = false;
    if(SceneController::getInstance()->currentSceneId != SCENE_ID_WORLD){
        return;
    }
    if(!WorldController::getInstance()->isInWorld){
        return;
    }
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        return;
    }
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    if (!info) {
        return;
    }
    if(!this->getParent()){
        return;
    }
    if (!info->objectForKey("uid")) {
        // no owner
        map<string, MarchInfo>::iterator it;
        for(it = WorldController::getInstance()->m_marchInfo.begin(); it != WorldController::getInstance()->m_marchInfo.end(); it++){
            if(it->second.endPointIndex == m_cityInfo.cityIndex){
                auto pointer = WorldMapView::instance()->m_occupyPointerNode->getChildByTag(it->second.marchTag);
                if (pointer) {
                    pointer->removeFromParentAndCleanup(true);
                    //删除显示
                    auto now = WorldController::getInstance()->getTime();
                    it->second.endStamp = now;
                    it->second.action = MarchActionNone;
                    it->second.stateType = StateReturn;
                    if (WorldController::getInstance()->initWorld && WorldMapView::instance()){
                        WorldMapView::instance()->updateMarchTarget(it->second, now, 1000);
                    }
                    //删除数据
                    WorldController::getInstance()->m_marchInfo.erase(it);
                    //刷新数据
                    if (WorldController::getInstance()->initWorld && WorldMapView::instance()){
                        WorldMapView::instance()->m_map->updateDynamicMap();
                    }
                    break;
                }
            }
        }
        m_cityInfo.playerName = "";
    }else{
        m_playerUid = info->valueForKey("uid")->getCString();
    }
    updateView();
}

void MainCityTile::updateView() {
    unsigned int buttonCount = 0;
    m_allianceId=m_cityInfo.kingBuildInfo.allianceUid;
    if(GlobalData::shared()->playerInfo.isInAlliance())//有联盟
    {
        if(m_allianceId=="")//没人占领
        {
            //城池详情,侦查,攻击,组队攻击
            buttonCount = 4;
            setButtonCount(buttonCount);
            setButtonName(3, _lang("108607"));//攻击
            setButtonName(2, _lang("108608"));//组队攻击
            setButtonName(5, _lang("108730"));//城池详情
            setButtonName(4, _lang("108722"));//侦查
            
            setButtonState(3, ButtonMarch);
            setButtonState(2, ButtonRally);
            setButtonState(5, ButtonInformation);
            setButtonState(4, ButtonScout);


        }else
        {
            if(m_allianceId == GlobalData::shared()->playerInfo.allianceInfo.uid)//自己联盟
            {
                auto isGarrison=false;
                map<string, MarchInfo>::iterator it;
                unsigned int index = m_cityInfo.cityIndex;
                if(m_cityInfo.parentCityIndex != -1){
                    index = m_cityInfo.parentCityIndex;
                }
                for(it = WorldController::getInstance()->m_marchInfo.begin(); it != WorldController::getInstance()->m_marchInfo.end(); it++){
                    if(it->second.endPointIndex == index){
                        if(it->second.stateType==StateOccupy)
                        {
                            isGarrison=true;
                            break;
                        }
                    }
                }
                if(isGarrison)//有部队
                {
                    //部队详情,城池详情,返回,援助,集结援助
                    buttonCount = 5;
                    setButtonCount(buttonCount);
                    setButtonName(1, _lang("108725"));//返回
                    setButtonName(3, _lang("108728"));//士兵援助
                    setButtonName(2, _lang("110067"));//组队援助
                    setButtonName(5, _lang("108724"));//部队详情
                    setButtonName(4, _lang("108730"));//城池详情
                    
                    setButtonState(1, ButtonGoHome);
                    setButtonState(3, ButtonSupport);
                    setButtonState(2, ButtonRally);
                    setButtonState(5, ButtonViewTroop);
                    setButtonState(4, ButtonInformation);
                    
                }else
                {
                    //城池详情,援助,集结援助
                    buttonCount = 3;
                    setButtonCount(buttonCount);
                    setButtonName(3, _lang("108728"));//士兵援助
                    setButtonName(2, _lang("110067"));//组队援助
                    setButtonName(1, _lang("108730"));//城池详情
                    setButtonState(3, ButtonSupport);
                    setButtonState(2, ButtonRally);
                    setButtonState(1, ButtonInformation);
                }
            }else
            {
                //城池详情,侦查,攻击,组队攻击
                buttonCount = 4;
                setButtonCount(buttonCount);
                setButtonName(3, _lang("108607"));//攻击
                setButtonName(2, _lang("108608"));//组队攻击
                setButtonName(5, _lang("108730"));//城池详情
                setButtonName(4, _lang("108722"));//侦查
                
                setButtonState(3, ButtonMarch);
                setButtonState(2, ButtonRally);
                setButtonState(5, ButtonInformation);
                setButtonState(4, ButtonScout);
            }

        }
    }else
    {
        buttonCount = 1;
        setButtonCount(buttonCount);
        setButtonName(1, _lang("108742"));
        setButtonState(1, ButtonProfile);
    }
    
    this->m_iconNode->removeAllChildren();
    for (int i=1; i<=TOTAL_BUTTON_NUM; ++i) {
        setButtonCallback(i, cccontrol_selector(MainCityTile::onClickButton));
    }
}

void MainCityTile::update(float sec) {
    if (m_isInUpdate) {
        return;
    }
    if(SceneController::getInstance()->currentSceneId != SCENE_ID_WORLD){
        return;
    }
    if(!WorldController::getInstance()->isInWorld){
        return;
    }
    if(!this->getParent()){
        return;
    }

//    bool flag = false;
//    if (m_currentState != getState()) {
//        if(MainCitySelf == m_currentState && getState() == MainCityBlank){
//            this->closeThis();
//            return;
//        }
//        flag = true;
//    }
//    
//    auto now = WorldController::getInstance()->getTime();
//    
//    if (m_currentState != MainCityBlank) {
//        
//        auto duration = now - m_cityInfo.resource.digStartTime + 500;
//        unsigned int currentLoad = (m_cityInfo.resource.digSpeed * duration)/1000;
//        if(m_cityInfo.resource.changeCollectSpdTime != 0){
//            duration = now - m_cityInfo.resource.changeCollectSpdTime;
//            currentLoad = m_cityInfo.resource.digStartNum + (m_cityInfo.resource.digSpeed * duration)/1000;
//        }
//
//        unsigned load = MIN(m_cityInfo.resource.troopMax, m_cityInfo.resource.sum);
//        
//        if (currentLoad >= load && m_cityInfo.resource.sum != 0) {
//            flag = true;
//            currentLoad = load;
//        }
//        m_title->setString(CCString::createWithFormat("%s", CCCommonUtils::getResourceStr(m_cityInfo.resource.sum - currentLoad).c_str())->getCString());
//    }
//    if(flag){
//        requestDetail();
//    }
}

bool MainCityTile::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    return NewBaseTileInfo::onAssignCCBMemberVariable(pTarget, pMemberVariableName, pNode);
}

CCNode *MainCityTile::getNodeByType(std::string type){
    return getButton(3);
}

MainCityTileState MainCityTile::getState() {
    //std::string nameStr = m_cityInfo.playerName;
    unsigned int index = m_cityInfo.cityIndex;
    if(m_cityInfo.parentCityIndex != -1){
        index = m_cityInfo.parentCityIndex;
    }
    std::string nameStr =WorldController::getInstance()->m_cityInfo[index].playerName;
    bool isOccupied = (nameStr != "");
    if (isOccupied) {
        auto ownerInfo = WorldController::getInstance()->m_playerInfo.find(nameStr);
        if(ownerInfo == WorldController::getInstance()->m_playerInfo.end()){
            return MainCityBlank;
        }
        switch (ownerInfo->second.type) {
            case PlayerSelf:
                return MainCitySelf;
            case PlayerAlliance:
                return MainCityAlliance;
            case PlayerOther:
                return MainCityOther;
                
            default:
                break;
        }
    }
    return MainCityBlank;
}

SEL_CCControlHandler MainCityTile::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NewBaseTileInfo::onResolveCCBCCControlSelector(pTarget, pSelectorName);
}

BaseTileInfo* MainCityTile::getFavoriteView() {
//    return AddFavorite::create(m_cityInfo.cityIndex,m_title->getString());
    int restype = m_cityInfo.resource.type;
    string resourceName;
    if (restype == 0) {
        resourceName = _lang("102011");
    }else if(restype == 1){
        resourceName = _lang("108580");
    }else if(restype == 2){
        resourceName = _lang("102012");
    }else if(restype == 3){
        resourceName = _lang("102013");
    }else{
        resourceName = CCCommonUtils::getResourceNameByType(m_cityInfo.resource.type);
    }
    string Lv = CC_ITOA((int)m_cityInfo.resource.lv);
    resourceName = resourceName + " Lv." + Lv;
    return AddFavorite::create(m_cityInfo.cityIndex,resourceName);
}

void MainCityTile::onClickButton(CCObject * pSender, Control::EventType pCCControlEvent) {
    // todo
    if (!this->isInitEnd) {
        return;
    }
    auto button = dynamic_cast<CCControlButton*>(pSender);
    auto buttonState = getButtonState(button);
    WorldController::getInstance()->alertProectFlag = false;
    WorldController::getInstance()->m_alertStateFlag = -1;
    switch (buttonState) {
        case ButtonOccupy:
        case ButtonDispatch:
        case ButtonMarch: {
            
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            int hasOwner = 0;
            if(m_allianceId!="")//没人占领
            {
                hasOwner=1;
            }
            auto selfProtect = GlobalData::shared()->playerInfo.protectTimeStamp;
            auto now = WorldController::getInstance()->getTime();
            int index=m_cityInfo.parentCityIndex;
            auto func = [&,index,hasOwner](){
                WorldController::getInstance()->openMarchDeploy(index, hasOwner,0);
            };
            if (now < selfProtect) {
                YesNoDialog::show(_lang("150319").c_str(),func);
            }
            else{
                func();
            }
            closeThis();
        }
            break;
        case ButtonSupport:{
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            
            auto selfProtect = GlobalData::shared()->playerInfo.protectTimeStamp;
            auto now = WorldController::getInstance()->getTime();
            int index=m_cityInfo.parentCityIndex;
            
            auto func = [&,index](){
                YuanJunTipView::openYuanYunView(index, Main_City, 1);
            };
            if (now < selfProtect){
                YesNoDialog::show(_lang("150319").c_str(),func);
            }
            else{
                func();
            }
            
            closeThis();
        }
            break;
        case ButtonProfile: {
            RoleInfoView::createInfoByUid(m_playerUid);
        }
            break;
        case ButtonScout: {
            if(ActivityController::getInstance()->dragonBattleNeedTip()){
                YesNoDialog::show(_lang("140142").c_str(), NULL);
                return ;
            }
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            // judge self protect state
            auto selfProtect = GlobalData::shared()->playerInfo.protectTimeStamp;
            auto now = WorldController::getInstance()->getTime();
            
            unsigned int index = m_cityInfo.cityIndex;
            if(m_cityInfo.parentCityIndex != -1){
                index = m_cityInfo.parentCityIndex;
            }
            
            auto func = [&,index](){
                auto world = WorldMapView::instance();
                if (world) {
                    world->addPopupView(ScoutInfo::create(WorldController::getInstance()->m_cityInfo[index]));
                }
            };
            
            if (now < selfProtect) {
                YesNoDialog::show(_lang("150319").c_str(),func);
            } else {
                func();
            }
            closeThis();
        }
            break;
        case ButtonInformation: {
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            bool isSelfPower = false;
            if(m_cityInfo.allianceName != "")
            {
                //同盟是否
                if(GlobalData::shared()->getPlayerInfo()->isInAlliance())
                {
                    if(m_cityInfo.allianceName == GlobalData::shared()->getPlayerInfo()->allianceInfo.name)
                    {
                        isSelfPower = true;
                    }
                }
            }else{
                if(m_cityInfo.playerName == GlobalData::shared()->getPlayerInfo()->name)
                {
                    isSelfPower = true;
                }
            }
            //先判断是不是主城
            if(m_cityInfo.cityType == Main_City)
            {
                PopupViewController::getInstance()->addPopupView(TipsWithPicView::create(m_cityInfo));
            }
            //go
            else if(isSelfPower)
            {
                PopupViewController::getInstance()->addPopupView(ResourceTileInfoPopUpView::create(m_cityInfo));
            }else{
                PopupViewController::getInstance()->addPopupView(TipsWithPicView::create(m_cityInfo));
            }
            closeThis();
        }
            break;
        case ButtonViewTroop:{
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            int index = m_cityInfo.cityIndex;
            if(m_cityInfo.parentCityIndex != -1){
                index = m_cityInfo.parentCityIndex;
            }
            std::string uuid = "";
            for (auto it = WorldController::getInstance()->m_marchInfo.begin(); it != WorldController::getInstance()->m_marchInfo.end(); it++) {
                if(it->second.ownerType == PlayerSelf && it->second.endPointIndex == index){
                    uuid = it->second.uuid;
                    if(it->second.teamUid != ""){
                        uuid = it->second.uuid;
                    }
                }
            }
            PopupViewController::getInstance()->addPopupView(TroopBuildingDetailView::create(uuid,index));
            closeThis();
        }
            break;
        case ButtonRally:{
            if(ActivityController::getInstance()->dragonBattleNeedTip()){
                YesNoDialog::show(_lang("140142").c_str(), NULL);
                return ;
            }
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            if(!GlobalData::shared()->playerInfo.isInAlliance()){
                CCCommonUtils::flyHint("", "", _lang("115218"));
                closeThis();
                return;
            }
            unsigned int index = m_cityInfo.cityIndex;
            if(m_cityInfo.parentCityIndex != -1){
                index = m_cityInfo.parentCityIndex;
            }
            auto &player = WorldController::getInstance()->m_playerInfo[m_cityInfo.playerName];
            if(player.cityLv <6 || m_cityInfo.leagueAsn==""){
                //todo
            }
            auto selfProtect = GlobalData::shared()->playerInfo.protectTimeStamp;
            auto now = WorldController::getInstance()->getTime();
            auto func = [&,index](){
                AllianceManager::getInstance()->openAllianceMassView(index);
                closeThis();
            };
            if (now < selfProtect) {
                YesNoDialog::show(_lang("150319").c_str(),func);
            } else {
                func();
                closeThis();
            }
        }
            break;
        case ButtonGoHome: {
            
            auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
            unsigned int index = m_cityInfo.cityIndex;
            if(m_cityInfo.parentCityIndex != -1){
                index = m_cityInfo.parentCityIndex;
            }
            auto it = selfMarch.find(index);
            if (it == selfMarch.end()) {
                // todo : show error
                return;
            }
            string uuid = it->second;
            
            if (uuid.empty()) {
                // todo : show error
                return;
            }
            
            auto dict = CCDictionary::create();
            dict->setObject(CCString::create(uuid), "marchId");
            WorldMapView::instance()->afterMarchCancel(dict);

            vector<EnemyInfo>::iterator enemyIt = EnemyInfoController::getInstance()->m_enemyInfos.begin();
            for(auto &enemyInfo : EnemyInfoController::getInstance()->m_enemyInfos)
            {
                for(auto &marchInfo : WorldController::getInstance()->m_marchInfo)
                {
                    if((enemyInfo.type == ENEMY_TYPE_BATTLE || enemyInfo.type == ENEMY_TYPE_TEAM) && enemyInfo.uuid == marchInfo.second.uuid)
                    {
                        if(marchInfo.second.endPointIndex == index)
                        {
                            // 停止闪红
//                                enemyInfo.type = ENEMY_TYPE_TARGET_RETREAT;
                            EnemyInfoController::getInstance()->m_enemyInfos.erase(enemyIt);
                            break;
                        }
                    }
                }
                enemyIt++;
            }
        }
            break;
            
        default:
            break;
    }
    
    closeThis();
}
