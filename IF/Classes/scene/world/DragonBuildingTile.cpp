//
//  DragonBuildingTile.cpp
//  IF
//
//  Created by ganxiaohua on 15/9/30.
//
//

#include "DragonBuildingTile.h"
#include "SoundController.h"
#include "YesNoDialog.h"
#include "ActivityController.h"

bool DragonBuildingTile::init() {
    NewBaseTileInfo::init();
    if(ThroneTile::init()){
        if(isSelfBuild()){
            setDefenderWarTimeButtons();
        }else{
            setAttackerWarTimeButtons();
        }
        return true;
    }
    return false;
}

std::string DragonBuildingTile::getShowText(){
    if(m_cityInfo.dragonTileInfo.allianceUid != ""){
        return m_cityInfo.dragonTileInfo.allianceUid;
    }else if(m_cityInfo.dragonTileInfo.uid != ""){
        return m_cityInfo.dragonTileInfo.uid;
    }
    return "";
}

int DragonBuildingTile::getOwnerFlag(){
    int hasOwner = 0;
    if(m_cityInfo.dragonTileInfo.uid != "" || m_cityInfo.dragonTileInfo.allianceUid != ""){
        hasOwner = 1;
    }
    return hasOwner;
}

void DragonBuildingTile::setDefenderWarTimeButtons(){
    if(GlobalData::shared()->playerInfo.isInAlliance()){
        ThroneTile::setDefenderWarTimeButtons();
    }else{
        setButtonCount(2);
        setButtonName(2,_lang("110076")); //
        setButtonName(3,_lang("108724")); //
        
        setButtonState(2, ButtonInformation);
        setButtonState(3, ButtonViewTroop);
        
        setButtonCallback(2,cccontrol_selector(DragonBuildingTile::onInfoClick));
        setButtonCallback(3,cccontrol_selector(DragonBuildingTile::onTroopClick));
    }
}

void DragonBuildingTile::setAttackerWarTimeButtons(){
    string btnStr = _lang("140124");
    if(m_cityInfo.dragonTileInfo.allianceUid!="" || m_cityInfo.dragonTileInfo.uid != ""){
        btnStr = _lang("108723");
    }
    if(m_cityInfo.dragonTileInfo.uid != ""){
        setButtonCount(4);
        setButtonName(2,_lang("110076")); //
        setButtonName(3,btnStr.c_str()); //
        setButtonName(4,_lang("108726")); //
        setButtonName(5,_lang("108722")); //
        setButtonState(2, ButtonInformation);
        setButtonState(3, ButtonMarch);
        setButtonState(4, ButtonRally);
        setButtonState(5, ButtonScout);
        setButtonCallback(2,cccontrol_selector(DragonBuildingTile::onInfoClick));
        setButtonCallback(3,cccontrol_selector(DragonBuildingTile::onClickOccupy));
        setButtonCallback(4,cccontrol_selector(DragonBuildingTile::onClickRally));
        setButtonCallback(5,cccontrol_selector(DragonBuildingTile::onClickScout));
    }else{
        setButtonCount(3);
        setButtonName(2,_lang("110076")); //
        setButtonName(3,btnStr.c_str()); //
        setButtonName(1,_lang("108726")); //
        setButtonState(2, ButtonInformation);
        setButtonState(3, ButtonMarch);
        setButtonState(1, ButtonRally);
        setButtonCallback(2,cccontrol_selector(DragonBuildingTile::onInfoClick));
        setButtonCallback(3,cccontrol_selector(DragonBuildingTile::onClickOccupy));
        setButtonCallback(1,cccontrol_selector(DragonBuildingTile::onClickRally));
    }
    
}

void DragonBuildingTile::onClickSurpport(CCObject * pSender, Control::EventType pCCControlEvent){
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    if(ActivityController::getInstance()->dragonBattleNeedTip()){
        YesNoDialog::show(_lang("140142").c_str(), NULL);
        return ;
    }
    int index = m_cityInfo.parentCityIndex;
    if (m_cityInfo.cityType == SupplyPoint || m_cityInfo.cityType == BessingTower || m_cityInfo.cityType == MedicalTower || m_cityInfo.cityType == DragonTower || m_cityInfo.cityType == TransferPoint) {
        index = m_cityInfo.cityIndex;
    }
    WorldController::getInstance()->openMarchDeploy(index, 1);
    closeThis();
}

void DragonBuildingTile::onClickOccupy(CCObject * pSender, Control::EventType pCCControlEvent) {
    if(ActivityController::getInstance()->dragonBattleNeedTip()){
        YesNoDialog::show(_lang("140142").c_str(), NULL);
        return ;
    }
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    int hasOwner = getOwnerFlag();
    int index = m_cityInfo.parentCityIndex;
    if (m_cityInfo.cityType == SupplyPoint || m_cityInfo.cityType == BessingTower || m_cityInfo.cityType == MedicalTower || m_cityInfo.cityType == DragonTower || m_cityInfo.cityType == TransferPoint) {
        index = m_cityInfo.cityIndex;
    }
    auto it = WorldController::getInstance()->m_dbuildingInfo.find(index);
    if (it!=WorldController::getInstance()->m_dbuildingInfo.end()) {
        std::string version = it->second.vesion;
        bool flag = CCCommonUtils::checkVersion(version);
        if(!flag){
            YesNoDialog::show(_lang("140152").c_str(), NULL);
            return ;
        }
    }
    
    WorldController::getInstance()->openMarchDeploy(index, hasOwner);
    closeThis();
}

void DragonBuildingTile::onClickScout(CCObject * pSender, Control::EventType pCCControlEvent){
    if(ActivityController::getInstance()->dragonBattleNeedTip()){
        YesNoDialog::show(_lang("140142").c_str(), NULL);
        return ;
    }
    int index = m_cityInfo.parentCityIndex;
    if (m_cityInfo.cityType == SupplyPoint || m_cityInfo.cityType == BessingTower || m_cityInfo.cityType == MedicalTower || m_cityInfo.cityType == DragonTower || m_cityInfo.cityType == TransferPoint) {
        index = m_cityInfo.cityIndex;
    }
    auto it = WorldController::getInstance()->m_dbuildingInfo.find(index);
    if (it!=WorldController::getInstance()->m_dbuildingInfo.end()) {
        std::string version = it->second.vesion;
        bool flag = CCCommonUtils::checkVersion(version);
        if(!flag){
            YesNoDialog::show(_lang("140152").c_str(), NULL);
            return ;
        }
    }
    ThroneTile::onClickScout(pSender,pCCControlEvent);
}

void DragonBuildingTile::onClickRally(CCObject * pSender, Control::EventType pCCControlEvent){
    if(ActivityController::getInstance()->dragonBattleNeedTip()){
        YesNoDialog::show(_lang("140142").c_str(), NULL);
        return ;
    }
    int index = m_cityInfo.parentCityIndex;
    if (m_cityInfo.cityType == SupplyPoint || m_cityInfo.cityType == BessingTower || m_cityInfo.cityType == MedicalTower || m_cityInfo.cityType == DragonTower || m_cityInfo.cityType == TransferPoint) {
        index = m_cityInfo.cityIndex;
    }
    auto it = WorldController::getInstance()->m_dbuildingInfo.find(index);
    if (it!=WorldController::getInstance()->m_dbuildingInfo.end()) {
        std::string version = it->second.vesion;
        bool flag = CCCommonUtils::checkVersion(version);
        if(!flag){
            YesNoDialog::show(_lang("140152").c_str(), NULL);
            return ;
        }
    }
    ThroneTile::onClickRally(pSender,pCCControlEvent);
}

bool DragonBuildingTile::isSelfBuild(){
    bool flag = false;
    if(GlobalData::shared()->playerInfo.isInAlliance()){
        if(m_cityInfo.dragonTileInfo.allianceUid!="" && GlobalData::shared()->playerInfo.allianceInfo.uid == m_cityInfo.dragonTileInfo.allianceUid){
            flag =  true;
        }
    }else{
        if(m_cityInfo.dragonTileInfo.uid != "" && GlobalData::shared()->playerInfo.uid == m_cityInfo.dragonTileInfo.uid){
            flag =  true;
        }
    }
    return flag;
}