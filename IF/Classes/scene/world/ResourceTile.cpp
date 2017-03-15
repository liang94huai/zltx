//
//  ResourceTile.cpp
//  IF
//
//  Created by 邹 程 on 14-2-26.
//
//

#include "ResourceTile.h"
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
#include "YuanJunTipView.h"

bool ResourceTile::init() {
    NewBaseTileInfo::init();
    std::string resName = "";
    if (m_cityInfo.resource.type<ResourceStr.size()) {//防止越界崩了
        resName = ResourceStr[m_cityInfo.resource.type];
    }
    //m_title->setString(CCString::createWithFormat("%d %s ",m_cityInfo.resource.lv,_lang(resName.c_str()).c_str())->getCString());
    
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
   //
    m_isInUpdate = false;
    addToParent();
    this->setVisible(false);
    updateView();
    return true;
}

void ResourceTile::onEnter(){
    NewBaseTileInfo::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void ResourceTile::onExit(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    NewBaseTileInfo::onExit();
}

void ResourceTile::requestDetail() {
    m_isInUpdate = true;
    unsigned int index = m_cityInfo.cityIndex;
    if(m_cityInfo.parentCityIndex != -1){
        index = m_cityInfo.parentCityIndex;
    }
    auto cmd = new WorldDetailCommand(index,m_cityInfo.tileServerId);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ResourceTile::onDetailCallback), nullptr));
    cmd->sendAndRelease();
}

void ResourceTile::onDetailCallback(cocos2d::CCObject *obj) {
    m_isInUpdate = false;
    this->setVisible(true);
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
        if (info->objectForKey("q")) {
            m_cityInfo.resource.troopMax = info->valueForKey("q")->intValue();
            m_cityInfo.resource.digSpeed = info->valueForKey("spd")->floatValue();
            m_cityInfo.resource.digStartTime = info->valueForKey("sdt")->doubleValue();
            m_cityInfo.resource.digStartNum = info->valueForKey("collected")->intValue();
            m_cityInfo.resource.changeCollectSpdTime = info->valueForKey("changeCollectSpdTime")->doubleValue();
        }
    }

    if(info->objectForKey("tq")){
        m_cityInfo.resource.sum = info->valueForKey("tq")->intValue();
        if(m_cityInfo.resource.sum == 0){
            WorldMapView::instance()->m_map->updateDynamicMap();
            this->closeThis();
            return;
        }
//        else if(WorldMapView::instance()->getNeedOpenInfoView())
//        {
//            WorldMapView::instance()->setNeedOpenInfoView(false);
//            PopupViewController::getInstance()->addPopupView(ResourceTileInfoPopUpView::create(m_cityInfo));
//        }
    }
    if (info->objectForKey("hm"))
    {
        m_isHaveMacrh=info->valueForKey("hm")->boolValue();
    }
    
    if (info->objectForKey("max"))
    {
        m_iMaxGarrison=info->valueForKey("max")->intValue();
        m_iCurrentGarrison=info->valueForKey("cu")->intValue();
    }
    if (info->objectForKey("if"))
    {
        m_isFull=info->valueForKey("if")->boolValue();
    }
    if (info->objectForKey("territory_positive_effect")) {
        GlobalData::shared()->alliance_territory_positive_effect_gather = 1;
    }
    else
        GlobalData::shared()->alliance_territory_positive_effect_gather = 0;
    
    auto now = WorldController::getInstance()->getTime();
    auto duration = now - m_cityInfo.resource.digStartTime;
    unsigned int currentLoad = (m_cityInfo.resource.digSpeed * duration)/1000;
    if(m_cityInfo.resource.changeCollectSpdTime != 0){
        duration = now - m_cityInfo.resource.changeCollectSpdTime;
        currentLoad = m_cityInfo.resource.digStartNum + (m_cityInfo.resource.digSpeed * duration)/1000;
    }
    //big
    if (info->objectForKey("bid")) {
        m_cityInfo.kingBuildInfo.tid = info->valueForKey("bid")->getCString();
    }
//    unsigned int sumFix = m_currentState == ResourceBlank ? 0 : currentLoad;
//    m_title->setString(CCCommonUtils::getResourceStr(m_cityInfo.resource.sum - sumFix).c_str());
    updateView();
}

void ResourceTile::updateView() {
    this->m_iconNode->removeAllChildren();
//    auto sprite = CCLoadSprite::createSprite(CCCommonUtils::getResourceIconByType(m_cityInfo.resource.type).c_str());
//    CCCommonUtils::setSpriteMaxSize(sprite, 30);
//    this->m_iconNode->addChild(sprite);
    m_currentState = getState();
//    string owner = m_currentState == ResourceBlank ? _lang("108550") : m_cityInfo.playerName;
//    m_title->setString(owner);
    
    auto now = WorldController::getInstance()->getTime();
    auto duration = now - m_cityInfo.resource.digStartTime;
    unsigned int currentLoad = (m_cityInfo.resource.digSpeed * duration)/1000;
    if(m_cityInfo.resource.changeCollectSpdTime != 0){
        duration = now - m_cityInfo.resource.changeCollectSpdTime;
        currentLoad = m_cityInfo.resource.digStartNum + (m_cityInfo.resource.digSpeed * duration)/1000;
    }

    unsigned int sumFix = m_currentState == ResourceBlank ? 0 : currentLoad;
    
//    if(m_cityInfo.resource.sum == 0){
//        m_title->setString("");
//    }else{
//        m_title->setString(/*_lang("108543") + */CCCommonUtils::getResourceStr(m_cityInfo.resource.sum - sumFix).c_str());
//    }
    
    unsigned int buttonCount = 0;
    if(!WorldController::isInSelfServer(m_cityInfo.tileServerId)){
        buttonCount = 1;
        setButtonCount(buttonCount);
        if(m_currentState == ResourceBlank){
            setButtonName(1, _lang("108730"));//说明
            setButtonState(1, ButtonInformation);
        }else{
            setButtonName(1, _lang("108742"));//我的详情
            setButtonState(1, ButtonProfile);
        }
    }else{
        switch (m_currentState) {
            case ResourceSelf: {
                if (m_isHaveMacrh&&!m_isFull)//有部队,未满员
                {
                    buttonCount = 4;
                    setButtonCount(buttonCount);
                    setButtonName(2, _lang("108730"));//说明
                    setButtonState(2, ButtonInformation);
                    
                    setButtonName(5, _lang("108742"));//我的详情
                    setButtonState(5, ButtonProfile);
                    
                    setButtonName(4, _lang("108725"));//返回
                    setButtonState(4, ButtonGoHome);

                    setButtonName(3, _lang("115333"));//驻守
                    setButtonState(3, ButtonDispatch);
                }else if(m_isHaveMacrh&&m_isFull)//有部队,满员
                {
                    buttonCount = 3;
                    setButtonCount(buttonCount);
                    setButtonName(3, _lang("108725"));//返回
                    setButtonName(2, _lang("108730"));//说明
                    setButtonName(1, _lang("108742"));//我的详情
                    setButtonState(1, ButtonProfile);
                    setButtonState(2, ButtonInformation);
                    setButtonState(3, ButtonGoHome);
                }else if(!m_isHaveMacrh&&!m_isFull)//没部队,没满员
                {
                    buttonCount = 3;
                    setButtonCount(buttonCount);
                    setButtonName(3, _lang("115333"));//驻守
                    setButtonName(2, _lang("108730"));//说明
                    setButtonName(1, _lang("108742"));//我的详情
                    setButtonState(1, ButtonProfile);
                    setButtonState(2, ButtonInformation);
                    setButtonState(3, ButtonDispatch);
                }else if(!m_isHaveMacrh&&m_isFull)//没部队,满员
                {
                    buttonCount = 2;
                    setButtonCount(buttonCount);
                    setButtonName(2, _lang("108730"));//说明
                    setButtonName(3, _lang("108742"));//我的详情
                    setButtonState(2, ButtonInformation);
                    setButtonState(3, ButtonProfile);
                }
            }
                break;
            case ResourceAlliance: {
                buttonCount = 2;
                setButtonCount(buttonCount);
                setButtonName(2, _lang("108721"));//主公详情
                setButtonName(3, _lang("108730"));//说明
                setButtonState(2, ButtonProfile);
                setButtonState(3, ButtonInformation);
            }
                break;
            case ResourceOther: {
                buttonCount = 4;
                setButtonCount(buttonCount);
                setButtonName(3, _lang("108723"));//攻击
                setButtonName(2, _lang("108721"));//主公详情
                setButtonName(5, _lang("108722"));//侦查
                setButtonName(4, _lang("108730"));//说明
                setButtonState(3, ButtonMarch);
                setButtonState(2, ButtonProfile);
                setButtonState(5, ButtonScout);
                setButtonState(4, ButtonInformation);
            }
                break;
            
            default: {
                buttonCount = 3;
                setButtonCount(buttonCount);
                setButtonName(3, _lang("108720"));//占领
                setButtonName(2, _lang("108730"));//说明
                setButtonName(1, _lang("108722"));//侦查

                setButtonState(3, ButtonOccupy);
                setButtonState(2, ButtonInformation);
                setButtonState(1, ButtonScout);

            }
                break;
        }
    }
    
    for (int i=1; i<=TOTAL_BUTTON_NUM; ++i) {
        setButtonCallback(i, cccontrol_selector(ResourceTile::onClickButton));
    }
}

void ResourceTile::update(float sec) {
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

    bool flag = false;
    if (m_currentState != getState()) {
        if(ResourceSelf == m_currentState && getState() == ResourceBlank){
            this->closeThis();
            return;
        }
        flag = true;
    }
    
    auto now = WorldController::getInstance()->getTime();
    
//    if (m_currentState != ResourceBlank) {
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
    if(flag){
        requestDetail();
    }
}

bool ResourceTile::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    return NewBaseTileInfo::onAssignCCBMemberVariable(pTarget, pMemberVariableName, pNode);
}

CCNode *ResourceTile::getNodeByType(std::string type){
    return getButton(3);
}

ResourceTileState ResourceTile::getState() {
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
            return ResourceBlank;
        }
        switch (ownerInfo->second.type) {
            case PlayerSelf:
                return ResourceSelf;
            case PlayerAlliance:
                return ResourceAlliance;
            case PlayerOther:
                return ResourceOther;
                
            default:
                break;
        }
    }
    return ResourceBlank;
}

SEL_CCControlHandler ResourceTile::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NewBaseTileInfo::onResolveCCBCCControlSelector(pTarget, pSelectorName);
}

BaseTileInfo* ResourceTile::getFavoriteView() {
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

void ResourceTile::onClickButton(CCObject * pSender, Control::EventType pCCControlEvent) {
    // todo
    if (!this->isInitEnd) {
        return;
    }
    auto button = dynamic_cast<CCControlButton*>(pSender);
    auto buttonState = getButtonState(button);
    WorldController::getInstance()->alertProectFlag = false;
    WorldController::getInstance()->m_alertStateFlag = -1;
    switch (buttonState) {
        case ButtonDispatch:{
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            
            auto selfProtect = GlobalData::shared()->playerInfo.protectTimeStamp;
            auto now = WorldController::getInstance()->getTime();
            unsigned int index = m_cityInfo.cityIndex;
            if(m_cityInfo.parentCityIndex != -1){
                index = m_cityInfo.parentCityIndex;
            }
            auto func = [&,index](){
                YuanJunTipView::openYuanYunView(index, WorldCityType::ResourceTile, 1,m_iMaxGarrison,m_iCurrentGarrison);
            };
            if (now < selfProtect){
                YesNoDialog::show(_lang("150319").c_str(),func);
            }
            else{
                func();
            }
        }
            break;
        case ButtonOccupy:
        case ButtonMarch: {
            
            if (!m_cityInfo.playerName.empty()) {
                // have owner
                WorldController::getInstance()->m_alertStateFlag = 2;
                
                // judge self protect state
                auto selfProtect = GlobalData::shared()->playerInfo.protectTimeStamp;
                auto now = WorldController::getInstance()->getTime();
                
                unsigned int index = m_cityInfo.cityIndex;
                if(m_cityInfo.parentCityIndex != -1){
                    index = m_cityInfo.parentCityIndex;
                }
                auto func = [&,index](){
                    WorldController::getInstance()->openMarchDeploy(index,1,0,MethodSALLCity);
                };
                
                if (now < selfProtect) {
                    YesNoDialog::show(_lang("101438").c_str(),func);//E100050
                } else {
                    func();
                }
                
            } else {
                // no owner
                WorldController::getInstance()->m_alertStateFlag = 0;
                
                for(auto it :WorldController::getInstance()->m_marchInfo)
                {
                    if(it.second.stateType == StateMarch && it.second.endPointIndex == this->m_cityInfo.cityIndex)
                    {
                        WorldController::getInstance()->m_alertStateFlag = 1;
                        WorldController::getInstance()->marchingAlertFlag = true;
                        break;
                    }
                }
                WorldController::getInstance()->alertProectFlag = true;
                unsigned int index = m_cityInfo.cityIndex;
                if(m_cityInfo.parentCityIndex != -1){
                    index = m_cityInfo.parentCityIndex;
                }
                WorldController::getInstance()->openMarchDeploy(index,0,0,MethodSALLCity);
            }
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("WR_attack"));
        }
            break;
        case ButtonProfile: {
            RoleInfoView::createInfoByUid(m_playerUid);
        }
            break;
        case ButtonScout: {
            // judge self protect state
            auto selfProtect = GlobalData::shared()->playerInfo.protectTimeStamp;
            auto now = WorldController::getInstance()->getTime();
            
            unsigned int index = m_cityInfo.cityIndex;
            auto func = [&,index](){
                auto world = WorldMapView::instance();
                if (world) {
                    world->addPopupView(ScoutInfo::create(WorldController::getInstance()->m_cityInfo[index]));
                }
            };
            
            if (now < selfProtect) {
                YesNoDialog::show(_lang("101438").c_str(),func);//E100050
            } else {
                func();
            }
        }
            break;
        case ButtonInformation: {
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            string content = m_cityInfo.resource.type != Gold ? "108629" : "108631";
            if(m_currentState == ResourceSelf){
                PopupViewController::getInstance()->addPopupView(ResourceTileInfoPopUpView::create(m_cityInfo));
            }else{
                //PopupViewController::getInstance()->addPopupView(TipsView::create(_lang(content)));
               
                PopupViewController::getInstance()->addPopupView(TipsWithPicView::create(m_cityInfo));
                 
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
