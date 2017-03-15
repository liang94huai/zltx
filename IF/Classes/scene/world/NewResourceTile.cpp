//
//  NewResourceTile.cpp
//  IF
//
//  Created by wangdianzhen on 15/12/24.
//
//

#include "NewResourceTile.hpp"

#include "WorldCommand.h"
#include "WorldMapView.h"
#include "YesNoDialog.h"
#include "AddFavorite.h"
#include "SceneController.h"
#include "SoundController.h"
#include "EnemyInfoController.h"
#include "AllianceManager.h"
#include "GlobalData.h"
#include "AllianceInfoView.h"
#include "PopupViewController.h"
#include "CheckAllianceInfoView.h"
#include "TipsWithPicView.h"
#include "TerritoryResourceDetailView.h"
#include "GetAllianceListCommand.h"
#include "FunBuildController.h"
#include "TerritoryTowerTipsView.h"
#include "YuanJunTipView.h"
#include "IFResourceLayer.hpp"
bool NewResourceTile::init() {
    NewBaseTileInfo::init();
    dataBack = false;
    m_cityInfo.m_newResourceInfo.digSpeed = 0;
    m_cityInfo.m_newResourceInfo.digStartTime = 0;
    m_cityInfo.m_newResourceInfo.sum = 0;
    initLayer();
    updateView();
    requestDetail();//todowdz 暂时不要sendcmd
//    onDetailCallback(NULL);
//    m_nameNode->removeAllChildren();
//    auto bg = CCLoadSprite::createSprite("name_bg.png");
//    m_nameNode->addChild(bg);

    m_isInUpdate = false;
    

    
    addToParent();
    return true;
}

void NewResourceTile::onEnter(){
    NewBaseTileInfo::onEnter();
}

void NewResourceTile::onExit(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    NewBaseTileInfo::onExit();
}

void NewResourceTile::requestDetail() {
    m_isInUpdate = true;
    auto cmd = new WorldDetailCommand(m_cityInfo.parentCityIndex,m_cityInfo.tileServerId);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(NewResourceTile::onDetailCallback), nullptr));
    cmd->sendAndRelease();
}

void NewResourceTile::onDetailCallback(cocos2d::CCObject *obj) {
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
    
    if (info) {
        dataBack = true;
    }
    unsigned int index = m_cityInfo.cityIndex;
    if (m_cityInfo.parentCityIndex != -1) {
        index = m_cityInfo.parentCityIndex;
    }
    auto cityIt = WorldController::getInstance()->m_cityInfo.find(index);
    if (cityIt != WorldController::getInstance()->m_cityInfo.end()) {
       //todowdz callbackdata
        if(info->objectForKey("tq")){
            cityIt->second.m_newResourceInfo.sum = info->valueForKey("tq")->intValue();
        }
        
        if(info->objectForKey("spd")){
            cityIt->second.m_newResourceInfo.digSpeed = info->valueForKey("spd")->intValue();
        }
        
        if(info->objectForKey("sdt")){
            cityIt->second.m_newResourceInfo.digStartTime = info->valueForKey("sdt")->doubleValue() / 1000;
        }
        if (info->objectForKey("existMember")) {
            m_cityInfo.m_newResourceInfo.now_capacity = info->valueForKey("existMember")->intValue();
        }
        if (info->objectForKey("maxMember")) {
            m_cityInfo.m_newResourceInfo.max_capacity = info->valueForKey("maxMember")->intValue();
        }
    }
    if (dataBack) {
        CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
    }
}

void NewResourceTile::initLayer(){
    this->m_iconNode->removeAllChildren();
    this->m_nameNode->removeAllChildren();
    m_title->setString("");
    unsigned int index = m_cityInfo.cityIndex;
    if (m_cityInfo.parentCityIndex != -1) {
        index = m_cityInfo.parentCityIndex;
    }
    auto cityIt = WorldController::getInstance()->m_cityInfo.find(index);
    if (cityIt != WorldController::getInstance()->m_cityInfo.end()) {
        auto sprite = CCLoadSprite::createSprite(CCCommonUtils::getResourceIconByType(Gold).c_str());
        CCCommonUtils::setSpriteMaxSize(sprite, 30);
        this->m_iconNode->addChild(sprite);
        
    }
}
void NewResourceTile::updateView() {
    //设置按钮
    unsigned int buttonCount = 0;
    if(!WorldController::isInSelfServer(m_cityInfo.tileServerId)){
        setButtonCount(1);
        setButtonName(1, _lang("115370"));
        setButtonState(1, ButtonFunction);
    }else{
        auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
        auto it = selfMarch.find(m_cityInfo.parentCityIndex);
        if (it != selfMarch.end()) {//判断是否已经有部队正在采集中，如有则变成返回
            buttonCount = 2;
            setButtonCount(buttonCount);
            setButtonName(2, _lang("115331"));
            setButtonState(2, ButtonInformation);
            setButtonName(3, _lang("108725"));
            setButtonState(3, ButtonGoHome);
        }
        else {
            buttonCount = 2;
            setButtonCount(buttonCount);
            setButtonName(2, _lang("115331"));
            setButtonState(2, ButtonInformation);
            setButtonName(3, _lang("115386"));
            setButtonState(3, ButtonGather);
        }
    }
    
    for (int i=1; i<=TOTAL_BUTTON_NUM; ++i) {
        setButtonCallback(i, cccontrol_selector(NewResourceTile::onClickButton));
    }
}

void NewResourceTile::update(float sec) {
    if(SceneController::getInstance()->currentSceneId != SCENE_ID_WORLD){
        return;
    }
    if(!WorldController::getInstance()->isInWorld){
        return;
    }
    if(!this->getParent()){
        return;
    }

    unsigned int index = m_cityInfo.cityIndex;
    if (m_cityInfo.parentCityIndex != -1) {
        index = m_cityInfo.parentCityIndex;
    }
    auto cityIt = WorldController::getInstance()->m_cityInfo.find(index);
    if (cityIt != WorldController::getInstance()->m_cityInfo.end()) {
        double now = GlobalData::shared()->getTimeStamp();
        double duration = now - cityIt->second.m_newResourceInfo.digStartTime;
        duration = MAX(duration, 0.0f);
        double currentLoad = cityIt->second.m_newResourceInfo.digSpeed * duration;
        currentLoad = m_cityInfo.m_newResourceInfo.now_capacity == 0 ? 0 :currentLoad;
        auto resouceSum = cityIt->second.m_newResourceInfo.sum - currentLoad;
        
        //                CCLog("territoryresource...time:%s",CC_ITOA(duration));
        //                CCLog("territoryresource...load:%d",currentLoad);
        resouceSum = MAX(resouceSum, 1);
        m_title->setString(CCString::createWithFormat("%s", CCCommonUtils::getResourceStr(resouceSum).c_str())->getCString());
        m_title->setColor(ccWHITE);
    }
}

bool NewResourceTile::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    return NewBaseTileInfo::onAssignCCBMemberVariable(pTarget, pMemberVariableName, pNode);
}


SEL_CCControlHandler NewResourceTile::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NewBaseTileInfo::onResolveCCBCCControlSelector(pTarget, pSelectorName);
}

BaseTileInfo* NewResourceTile::getFavoriteView() {
    string superSourceName = _lang("150442");
    return AddFavorite::create(m_cityInfo.parentCityIndex,superSourceName);
}

void NewResourceTile::onClickButton(CCObject * pSender, Control::EventType pCCControlEvent) {
    // todo
    auto button = dynamic_cast<CCControlButton*>(pSender);
    auto buttonState = getButtonState(button);
    WorldController::getInstance()->alertProectFlag = false;
    WorldController::getInstance()->m_alertStateFlag = -1;
    switch (buttonState) {
        case ButtonGather:{
            if (m_cityInfo.m_newResourceInfo.now_capacity < m_cityInfo.m_newResourceInfo.max_capacity) {
                
                
                bool marchingAlertFlag = false;
                for(auto it :WorldController::getInstance()->m_marchInfo)
                {
                    if (it.second.ownerType == PlayerSelf) {
                        if((it.second.stateType == StateMarch && it.second.endPointIndex == m_cityInfo.parentCityIndex))
                        {
                            marchingAlertFlag = true;
                            break;
                        }
                    }
                }
                if (marchingAlertFlag) {
                    CCCommonUtils::flyHint("", "", _lang("150307"));
                    return;
                }
                WorldController::getInstance()->openMarchDeploy(m_cityInfo.parentCityIndex,0);
            }else{
                CCCommonUtils::flyHint("", "", _lang("150453"));
            }
            
        }
            break;
        case ButtonFunction:{
            PopupViewController::getInstance()->addPopupView(TerritoryTowerTipsView::create(m_cityInfo, false,true));
        }
            break;
        case ButtonInformation: {
            unsigned int index = m_cityInfo.cityIndex;
            if (m_cityInfo.parentCityIndex != -1) {
                index = m_cityInfo.parentCityIndex;
            }
            PopupViewController::getInstance()->addPopupInView(IFResourceLayer::create(WorldController::getInstance()->m_cityInfo[index]));
        }
            break;
        case ButtonGoHome: {
            auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
            auto it = selfMarch.find(m_cityInfo.parentCityIndex);
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
        }
            break;
            
        default:
            break;
    }
    closeThis();
}