//
//  ResourceTileInfoPopUpView.cpp
//  IF
//
//  Created by 李锐奇 on 14-8-21.
//
//

#include "ResourceTileInfoPopUpView.h"
#include "WorldCommand.h"
#include "WorldMapView.h"
#include "UseItemStatusView.h"
#include "ShowStatusItemCommand.h"
#include "SoldierIconCell.hpp"
#include "ArmyController.h"
#include "AllianceReinforceSoldier.h"

ResourceTileInfoPopUpView *ResourceTileInfoPopUpView::create(WorldCityInfo &info){
    ResourceTileInfoPopUpView *ret = new ResourceTileInfoPopUpView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


void ResourceTileInfoPopUpView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize ResourceTileInfoPopUpView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    return CCSize(250, 130);
}

CCTableViewCell* ResourceTileInfoPopUpView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, ssize_t idx){
    if(idx >= m_data->count()){
        return NULL;
    }
    ResourceTroopInfo* cell = (ResourceTroopInfo*)table->dequeueGrid();
    CCDictionary* info = dynamic_cast<CCDictionary*>(m_data->objectAtIndex(idx));
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info);
        }else{
            cell = ResourceTroopInfo::create(info);
        }
    }
    return cell;
}

ssize_t ResourceTileInfoPopUpView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    return ceil(m_data->count() * 1.0f / 2.0f);
}

ssize_t ResourceTileInfoPopUpView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 2;
}

bool ResourceTileInfoPopUpView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(4, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(4, false);
        });
        m_starTime =  WorldController::getInstance()->getTime();
        unsigned int index = m_info.cityIndex;
        if(m_info.parentCityIndex != -1){
            index = m_info.parentCityIndex;
        }
        if(WorldController::getInstance()->getIsSelfResCity(index))
        {
            m_starTime = WorldController::getInstance()->m_resCityInfo[index].m_StartTime;
        }
        WorldMapView::instance()->m_directionIcon->setVisible(false);
        WorldMapView::instance()->m_directionLabel->setVisible(false);
        auto bg = CCBLoadFile("TipsView", this, this);
        //m_closeBtn->setSwallowsTouches(false);
        this->setContentSize(bg->getContentSize());
        

        this->setPositionY(CCDirector::getInstance()->getWinSize().height-this->getContentSize().height);
        
        initLabInfo();
        m_data = CCArray::create();
        refreshView();
        
        
        ret = true;
    }
    return ret;
}

void ResourceTileInfoPopUpView::initLabInfo()
{
    m_title->setFntFile("Arial_Bold_Border.fnt");
    m_title->setString(_lang("new100014").c_str());
    // m_title->setString("城池详情");
    
    m_cityNameTxt->setFntFile("Arial_Bold_Border.fnt");
    m_cityNameTxt->setString(_lang("new100015").c_str());
    // m_cityNameTxt->setString("城池名称:");
    
    m_cityName->setFntFile("Arial_Bold_Border.fnt");
    //m_title->setString(_lang("new100003").c_str());
    string id = "";
    if(m_info.cityType == Main_City)
    {
        id = m_info.kingBuildInfo.tid;
    }else if (m_info.cityType == ResourceTile)
    {
        int  iD = (m_info.resource.tId);
        id = CC_ITOA(iD);
    }
     
    auto dict = LocalController::shared()->DBXMLManager()->getObjectByKey(id);
    auto cityname = dict->valueForKey("name")->getCString();
    auto resHourInfo = dict->valueForKey("resPerHour")->getCString();
    auto resMaxInfo = dict->valueForKey("resMax")->getCString();
    auto garrisonCap = dict->valueForKey("garrisonCap")->getCString();
    onSplitResInfo(resHourInfo,m_mapResSpeedInfo);
    onSplitResInfo(resMaxInfo,m_mapResMaxInfo);
    
    m_cityName->setString(_lang(cityname).c_str());
    
    auto areaname = WorldController::getInstance()->getHomeOwnershipNameByMapPos(m_info.cityTilePoint);
    m_zoneName->setString(areaname);
    
    // m_state->setString(m_info);
    // m_foodNumTxt->setString(m_info);
    // m_woodNumTxt->setString(m_info);
    // m_stoneNumTxt->setString(m_info);
    // m_foodNumTxt->setString(m_info);
    m_zoneName->setFntFile("Arial_Bold_Border.fnt");
    //m_zoneName->setString(_lang("new100003").c_str());
    // m_zoneName->setString(MiniMapController::getInstance()->getCityPosById(m_info.cityIndex)->m_ZoneName.c_str());
    
    m_zoneNameTxt->setFntFile("Arial_Bold_Border.fnt");
    m_zoneNameTxt->setString(_lang("new100016").c_str());
    // m_zoneNameTxt->setString("所处区域:");
    
    m_stateTxt->setFntFile("Arial_Bold_Border.fnt");
    m_stateTxt->setString(_lang("new100018").c_str());
    // m_stateTxt->setString("占领状态:");
    
    m_state->setFntFile("Arial_Bold_Border.fnt");
    //m_state->setString(_lang("new100003").c_str());
    //自己占领
    if(GlobalData::shared()->getPlayerInfo()->isInAlliance())
        
    {
        string strContent = string("[") +GlobalData::shared()->getPlayerInfo()->allianceInfo.name + string("]") +GlobalData::shared()->getPlayerInfo()->name;
        m_state->setString(strContent.c_str());
    }else{
        m_state->setString(GlobalData::shared()->getPlayerInfo()->name.c_str());
    }
    m_state->setColor(ccc3(0, 0, 0));
    m_cityBuffTxt->setFntFile("Arial_Bold_Border.fnt");
    m_cityBuffTxt->setString(_lang("new100021").c_str());
    // m_cityBuffTxt->setString("城池加成:");
    
    //加成信息
    
    int iHeight = 0;
    int iAddH = 50;
    //先判断要不要上一一个格子
    int iResSpeed = 0;
    //联盟采集 加成 resAddRatio
    //1.判断是否有联盟
    //2.判断所属主城是否被己方占领
    //3.得到resAddRatio字段 加成
    int addBuff = 0;
    auto  bidInfo = WorldController::getInstance()->m_mapMainCity.find(m_info.kingBuildInfo.tid);
    if(bidInfo !=  WorldController::getInstance()->m_mapMainCity.end())
    {
        if(GlobalData::shared()->getPlayerInfo()->isInAlliance() && bidInfo->second.allianceName == GlobalData::shared()->playerInfo.allianceInfo.name)
        {
            //自己的联盟 算上加成 获取加成值
            auto dict = LocalController::shared()->DBXMLManager()->getObjectByKey(m_info.kingBuildInfo.tid);
            string buffStr = dict->valueForKey("resAddRatio")->getCString();
            addBuff = atoi(buffStr.c_str())/10;
        }
    }
    
    string strRate = _lang("new100024").c_str();
    if( isResTpye(0,iResSpeed) + isResTpye(3,iResSpeed) + isResTpye(2,iResSpeed) + isResTpye(1,iResSpeed) == 1)
    {
        iHeight += iAddH;   //只有一种资源可以采集的情况!
        m_cityBuffTxt->setPositionY(m_cityBuffTxt->getPositionY() + iAddH);
    }
    if(!isResTpye(3,iResSpeed)) // 粮 3, 铁 2, 木 0 银 1
    {
        iHeight += iAddH;
        m_foodnode->setVisible(false);
        m_foodcaijinode->setVisible(false);
        m_stonecaijinode->setPositionX(m_stonecaijinode->getPositionX()-170);
    }else{
        m_foodnode->setVisible(true);
        m_foodcaijinode->setVisible(true);
        m_foodnode->setPositionY(m_foodnode->getPositionY() + iHeight);
        m_foodNumTxt->setFntFile("Arial_Bold_Border.fnt");
        string strRate = _lang("new100024").c_str();
        if(addBuff != 0)
        {
            m_foodNumTxt->setString((string(CC_ITOA(iResSpeed)+string("+")+string(CC_ITOA(addBuff/1000.0*iResSpeed))+strRate).c_str()));
        }else{
            m_foodNumTxt->setString((string(CC_ITOA(iResSpeed)+strRate).c_str()));
        }
        
    }
    
    if(!isResTpye(0,iResSpeed))
    {
        iHeight += iAddH;
        m_woodnode->setVisible(false);
        m_woodcaijinode->setVisible(false);
        m_ironcaijinode->setPositionX(m_ironcaijinode->getPositionX()-170);
    }else{
        m_woodnode->setVisible(true);
        m_woodcaijinode->setVisible(true);
        m_woodnode->setPositionY(m_woodnode->getPositionY() + iHeight);
        m_woodNumTxt->setFntFile("Arial_Bold_Border.fnt");
        
        if(addBuff != 0)
        {
            m_woodNumTxt->setString((string(CC_ITOA(iResSpeed)+string("+")+string(CC_ITOA(addBuff/1000.0*iResSpeed))+strRate).c_str()));
        }else{
            m_woodNumTxt->setString((string(CC_ITOA(iResSpeed)+strRate).c_str()));
        }
        
        if(!isResTpye(3,iResSpeed) && !isResTpye(1,iResSpeed))
        {
            m_woodcaijinode->setPositionY(m_woodcaijinode->getPositionY() + iAddH);

        }
           }
    
    if(!isResTpye(1,iResSpeed))
    {
        iHeight += iAddH;
        m_stonenode->setVisible(false);
        m_stonecaijinode->setVisible(false);

    }else{
        m_stonenode->setVisible(true);
        m_stonecaijinode->setVisible(true);
        m_stonenode->setPositionY(m_stonenode->getPositionY() + iHeight);
        
        m_stoneNumTxt->setFntFile("Arial_Bold_Border.fnt");
        if(addBuff != 0)
        {
            m_stoneNumTxt->setString((string(CC_ITOA(iResSpeed)+string("+")+string(CC_ITOA(addBuff/1000.0*iResSpeed))+strRate).c_str()));
        }else{
            m_stoneNumTxt->setString((string(CC_ITOA(iResSpeed)+strRate).c_str()));
        }
    }
    
    
    if(!isResTpye(2,iResSpeed))
    {
        iHeight += iAddH;
        m_ironnode->setVisible(false);
        m_ironcaijinode->setVisible(false);
    }else{
        m_ironnode->setVisible(true);
        m_ironcaijinode->setVisible(true);
        m_ironnode->setPositionY(m_ironnode->getPositionY() + iHeight);
        
        
        m_ironNumTxt->setFntFile("Arial_Bold_Border.fnt");
        
        if(addBuff != 0)
        {
            m_ironNumTxt->setString((string(CC_ITOA(iResSpeed)+string("+")+string(CC_ITOA(addBuff/1000.0*iResSpeed))+strRate).c_str()));
        }else{
            m_ironNumTxt->setString((string(CC_ITOA(iResSpeed)+strRate).c_str()));
        }
        
        if(!isResTpye(3,iResSpeed) && !isResTpye(1,iResSpeed))
        {
            m_ironcaijinode->setPositionY(m_ironcaijinode->getPositionY() + iAddH);
            
        }
        
    }
    m_ArmNoneTxt->setFntFile("Arial_Bold_Border.fnt");
    m_ArmNoneTxt->setString(_lang("new100025").c_str());
    //m_ArmNoneTxt->setString("无部队驻扎");
    
    
    m_armtile->setPositionY(m_armtile->getPositionY() + iHeight);
    m_noneArm->setPositionY(m_noneArm->getPositionY() + iHeight);
    
    m_infolist->setContentSize(CCSizeMake(m_infolist->getContentSize().width,m_infolist->getContentSize().height+iHeight));
    m_infolist->setPositionY(m_infolist->getPositionY() - iHeight);
    
    m__caijiNums[3]->setFntFile("Arial_Bold_Border.fnt");
    
    m__caijiNums[2]->setFntFile("Arial_Bold_Border.fnt");
    
    m__caijiNums[1]->setFntFile("Arial_Bold_Border.fnt");
    
    m__caijiNums[0]->setFntFile("Arial_Bold_Border.fnt");
    
 
    m_caijiTxt->setFntFile("Arial_Bold_Border.fnt");
    m_caijiTxt->setString(_lang("new100022").c_str());
    // m_caijiTxt->setString("采集状态:");

    m_ArmSumTxt->setFntFile("Arial_Bold_Border.fnt");
    m_ArmSumTxt->setString(_lang("new100023").c_str());
    // m_ArmSumTxt->setString("士兵总数:");

    m_ArmNameTxt->setFntFile("Arial_Bold_Border.fnt");
    m_ArmNameTxt->setString(_lang("new100017").c_str());
    // m_ArmNameTxt->setString("驻守数量:");

    m_armNum->setFntFile("Arial_Bold_Border.fnt");
    //m_caijiTxt->setString(_lang("new100003").c_str());
    //m_armNum->setString(garrisonCap);
    string Armnum = string("0/") + string(garrisonCap);
    m_armNum->setString(Armnum.c_str());
    
    m_ArmSum->setFntFile("Arial_Bold_Border.fnt");
    //m_caijiTxt->setString(_lang("new100003").c_str());
    m_ArmSum->setString("99999");

    
}

void ResourceTileInfoPopUpView::updateTime(float delta)
{
    //更新采集信息
    //CC_SECTOA
    auto nowtime = WorldController::getInstance()->getTime()/1000;
    auto starttime = m_starTime/1000;
    double dtTime = nowtime - starttime;
    map<int,int>::iterator it = m_mapResSpeedInfo.begin();
    for(;it != m_mapResSpeedInfo.end();it++)
    {

        int iIndex =  it->first;
        double speedCaiji = (double)(it->second)/3600;
        double sumRes = dtTime*speedCaiji;
        if(sumRes >= m_mapResMaxInfo[iIndex])
        {
            this->unschedule(schedule_selector(ResourceTileInfoPopUpView::updateTime));
            sumRes = m_mapResMaxInfo[iIndex];
            
            m__caijiNums[iIndex]->setString(CC_CMDITOAD(sumRes).c_str());
            continue;
        }
        string strCap = CC_CMDITOAD(sumRes);
        //考虑下大城的buff
        //m_cityInfo.kingBuildInfo.tid
        //判断下大城是否被自己的联盟占领
        //1. 大城的联盟name
        auto  bidInfo = WorldController::getInstance()->m_mapMainCity.find(m_info.kingBuildInfo.tid);
        if(bidInfo !=  WorldController::getInstance()->m_mapMainCity.end())
        {
            if(GlobalData::shared()->getPlayerInfo()->isInAlliance() && bidInfo->second.allianceName == GlobalData::shared()->playerInfo.allianceInfo.name)
            {
                //自己的联盟 算上加成 获取加成值
                auto dict = LocalController::shared()->DBXMLManager()->getObjectByKey(m_info.kingBuildInfo.tid);
                string buffStr = dict->valueForKey("resAddRatio")->getCString();
                int addBuff = atoi(buffStr.c_str());
                int addRes = sumRes*(addBuff/1000.0);
                strCap = CC_CMDITOAD(sumRes) + string(" + ") +CC_CMDITOAD(addRes);
            }
        }         
        m__caijiNums[iIndex]->setString(strCap.c_str());
        //上限 .tip ,收获tip
        
        
    }
//    for (int i = 0; i < m_mapResSpeedInfo.size(); ++i)
//    {
//        /* code */
//    }
    //auto dict = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(id));
    
    if(false)    //达到上限 ,停止update
    {
        
 
    }
}

void ResourceTileInfoPopUpView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    this->schedule(schedule_selector(ResourceTileInfoPopUpView::updateTime), 1, -1, 1);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    
}

void ResourceTileInfoPopUpView::onExit(){
    this->unschedule(schedule_selector(ResourceTileInfoPopUpView::updateTime));
 
    WorldMapView::instance()->updateDirection();
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool ResourceTileInfoPopUpView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
//    if(isTouchInside(m_toolUseBtn, pTouch))
//    {
//        return true;
//    }
//    if(isTouchInside(m_buildBG, pTouch)){
//        return false;
//    }

    return true;
}

void ResourceTileInfoPopUpView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
//    if(isTouchInside(m_toolUseBtn, pTouch))
//    {
//        onToolUseBtnClick(nullptr,Control::EventType::TOUCH_DOWN);
//    }
//    if(!isTouchInside(m_buildBG, pTouch)){
//        this->closeSelf();
//    }
     if(!isTouchInside(m_touchNode, pTouch)){
        this->closeSelf();
    }
}

void ResourceTileInfoPopUpView::onClickCloseBtn(cocos2d::CCObject * pSender, CCControlEvent pCCControlEvent)
{
    
//    PopupViewController::getInstance()->addPopupInView(UseItemStatusView::create(8,_lang("101406"),_lang("101448")));
    this->closeSelf();
}

SEL_CCControlHandler ResourceTileInfoPopUpView::onResolveCCBCCControlSelector(CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCloseBtn", ResourceTileInfoPopUpView::onClickCloseBtn);
    return NULL;
}

bool ResourceTileInfoPopUpView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infolist", CCNode*, this->m_infolist);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodnode", CCNode*, this->m_foodnode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woodnode", CCNode*, this->m_woodnode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stonenode", CCNode*, this->m_stonenode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ironnode", CCNode*, this->m_ironnode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodcaijinode", CCNode*, this->m_foodcaijinode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woodcaijinode", CCNode*, this->m_woodcaijinode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stonecaijinode", CCNode*, this->m_stonecaijinode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ironcaijinode", CCNode*, this->m_ironcaijinode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arminfo", CCNode*, this->m_arminfo);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noneArm", CCNode*, this->m_noneArm);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_armtile", CCNode*, this->m_armtile);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title", CCLabelIF*, this->m_title);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cityNameTxt", CCLabelIF*, this->m_cityNameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cityName", CCLabelIF*, this->m_cityName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_zoneNameTxt", CCLabelIF*, this->m_zoneNameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_zoneName", CCLabelIF*, this->m_zoneName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stateTxt", CCLabelIF*, this->m_stateTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_state", CCLabelIF*, this->m_state);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodNumTxt", CCLabelIF*, this->m_foodNumTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woodNumTxt", CCLabelIF*, this->m_woodNumTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodNumTxt", CCLabelIF*, this->m_foodNumTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stoneNumTxt", CCLabelIF*, this->m_stoneNumTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ironNumTxt", CCLabelIF*, this->m_ironNumTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cityBuffTxt", CCLabelIF*, this->m_cityBuffTxt);
    // 粮 3, 铁 2, 木 0 银 1
   // CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m__caijifoodNum", CCLabelIF*, this->m__caijifoodNum);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m__caijiwoodNum", CCLabelIF*, this->m__caijiwoodNum);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m__caijistoneNum", CCLabelIF*, this->m__caijistoneNum);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m__caijiironNum", CCLabelIF*, this->m__caijiironNum);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m__caijifoodNum", CCLabelIF*, this->m__caijiNums[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m__caijiwoodNum", CCLabelIF*, this->m__caijiNums[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m__caijistoneNum", CCLabelIF*, this->m__caijiNums[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m__caijiironNum", CCLabelIF*, this->m__caijiNums[2]);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_caijiTxt", CCLabelIF*, this->m_caijiTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ArmSumTxt", CCLabelIF*, this->m_ArmSumTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ArmSum", CCLabelIF*, this->m_ArmSum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ArmNameTxt", CCLabelIF*, this->m_ArmNameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_armNum", CCLabelIF*, this->m_armNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ArmNoneTxt", CCLabelIF*, this->m_ArmNoneTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ArmNoneNode", CCNode*, this->m_ArmNoneNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodIcon", CCSprite*, m_foodIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woodIcon", CCSprite*, m_woodIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ironIcon", CCSprite*, m_ironIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ironIcon", CCSprite*, m_ironIcon);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m__caijifoodIcon", CCSprite*, m__caijifoodIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m__caijiwoodIcon", CCSprite*, m__caijiwoodIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m__caijistoneIcon", CCSprite*, m__caijistoneIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m__caijiironIcon", CCSprite*, m__caijiironIcon);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_closeBtn", CCControlButton*, m_closeBtn);
 
    

    
    return false;
}

void ResourceTileInfoPopUpView::generateData(){
    
}

// void ResourceTileInfoPopUpView::update(float delta){
 
// }

void ResourceTileInfoPopUpView::refreshView(){
   
     m_tabView = CCMultiColTableView::create(this, CCSize(m_infolist->getContentSize()+CCSize(-10,-10)));
     m_tabView->setDirection(kCCScrollViewDirectionVertical);
     m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
     m_tabView->setMultiColTableViewDelegate(this);
     m_tabView->setPosition(ccp(5,0));
     m_infolist->addChild(m_tabView);
    //add bg
    auto soldierBG = CCLoadSprite::createScale9Sprite("brown_bar_bg.png");
    soldierBG->setAnchorPoint(ccp(0, 0));
    soldierBG->setContentSize(m_infolist->getContentSize());
    
    soldierBG->setPosition(0,0);
    m_infolist->addChild(soldierBG,-1);
     m_ArmNoneTxt->setVisible(true);
     m_arminfo->setVisible(false);
     getTroopData();
     //update(0.0f);
    if(m_info.cityType == Main_City)
    {
        getCapInfoData();
    }
 
}

void ResourceTileInfoPopUpView::troopDataBack(CCObject *obj){
    auto ret = dynamic_cast<NetResult*>(obj);
//    if (!ret || ret->getErrorCode() != Error_OK) {
//        // todo : parse error
//        return;
//    }
    m_data->removeAllObjects();
    int allNum = 0;
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    if (info && info->objectForKey("uuid")) {
        string uuid = info->valueForKey("uuid")->getCString();
        
        auto endStamp = info->valueForKey("endStamp")->doubleValue();
       // m_starTime = endStamp;
        if(info->objectForKey("soldiers")){
            auto arr = dynamic_cast<CCArray*>(info->objectForKey("soldiers"));
            int index = 0;
            while (index < arr->count()) {
                auto dict = dynamic_cast<CCDictionary*>(arr->objectAtIndex(index));
                int num = dict->valueForKey("count")->intValue();
                allNum += num;
                std::string armyId = dict->valueForKey("armyId")->getCString();
                CCDictionary *data = CCDictionary::create();
                data->setObject(CCString::create(CC_ITOA(num)), "count");
                data->setObject(CCString::create(armyId), "armyId");
                m_data->addObject(data);
                index++;
            }
        }
    }
    
    m_ArmSum->setString(CC_ITOA(allNum));
    
    string id = "";
    if(m_info.cityType == Main_City)
    {
        id = m_info.kingBuildInfo.tid;
    }else if (m_info.cityType == ResourceTile)
    {
        int  iD = (m_info.resource.tId);
        id = CC_ITOA(iD);
    }

    auto dict = LocalController::shared()->DBXMLManager()->getObjectByKey(id);
    
    auto garrisonCap = dict->valueForKey("garrisonCap")->getCString();
    string Armnum = string(CC_ITOA(allNum)) + string("/") + string(garrisonCap);
    m_armNum->setString(Armnum.c_str());
    if(m_data->count() == 0)
    {
        m_noneArm->setVisible(true);
        m_arminfo->setVisible(false);
        m_touchNode->setContentSize(Size(580.0,520));
        m_touchNode->setPosition(Vec2(42,278));
        m_mainNode->setPosition(Vec2::ZERO);
    }else{
        m_noneArm->setVisible(false);
        m_arminfo->setVisible(true);
        m_touchNode->setContentSize(Size(580.0,780));
        m_touchNode->setPosition(Vec2(42,23));
        m_mainNode->setPosition(Vec2(0,(278-23)/2));

    }
    m_tabView->reloadData();
}

 
void ResourceTileInfoPopUpView::onSplitResInfo(const char* strContent, map<int,int> &mapinfo)
{
    mapinfo.clear();
    vector<string> strVec;
    CCCommonUtils::splitString(strContent, "|", strVec);
    
    vector<string>::iterator it = strVec.begin();
    for(;it != strVec.end();it++)
    {
        vector<string> strInfo;
        CCCommonUtils::splitString((*it), ";", strInfo);
        if(strInfo.size() == 2)
        {
            mapinfo[atoi(strInfo[0].c_str())] = atoi(strInfo[1].c_str());
            
        }
    }
    
}


bool ResourceTileInfoPopUpView::isResTpye(int iType,int &iNum)  //此类型资源是否在资源点产出
{
    bool iperHour = 0;
    map<int,int>::iterator it = m_mapResSpeedInfo.begin();
    for(;it != m_mapResSpeedInfo.end();it++)
    {
        if(iType == (*it).first)
        {
            iNum  = (*it).second;
            iperHour = true;
            break;
        }
    }
    
    return iperHour;
}
void ResourceTileInfoPopUpView::getTroopData(){
    marchId = "";
    map<string, MarchInfo>::iterator it;
    unsigned int index = m_info.cityIndex;
    if(m_info.parentCityIndex != -1){
        index = m_info.parentCityIndex;
    }
    for(it = WorldController::getInstance()->m_marchInfo.begin(); it != WorldController::getInstance()->m_marchInfo.end(); it++){
        if(it->second.endPointIndex == index){
            marchId = it->first;
        }
    }
    
    auto cmd = new WorldMarchDetailUpdateCommand(marchId);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ResourceTileInfoPopUpView::troopDataBack), NULL));
    cmd->sendAndRelease();
}

void ResourceTileInfoPopUpView::getCapInfoData()
{
    AllianceReinforceSoldier* cmd = new AllianceReinforceSoldier(ALLIANCE_AL_REINFORCE_THRONE_TREBUCHET);
    unsigned int index = m_info.cityIndex;
    if(m_info.parentCityIndex != -1){
        index = m_info.parentCityIndex;
    }
    cmd->putParam("pointId", CCInteger::create(index));
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ResourceTileInfoPopUpView::capInfoBack), NULL));
    cmd->sendAndRelease();
}

void ResourceTileInfoPopUpView::capInfoBack(CCObject *obj)
{
    NetResult* result = dynamic_cast<NetResult*>(obj);
    auto dic = _dict(result->getData());
    if(dic)
    {
        auto maxnum = dic->valueForKey("maxSoldiers")->intValue();
        auto max = dic->valueForKey("max")->intValue();
    }
    
}

//---

ResourceTroopInfo *ResourceTroopInfo::create(CCDictionary *dict){
    ResourceTroopInfo *ret = new ResourceTroopInfo();
    if(ret && ret->init(dict)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ResourceTroopInfo::setData(CCDictionary *dict){
    m_dict = dict;
    refreshView();
}

bool ResourceTroopInfo::init(CCDictionary *dict){
    auto node = CCBLoadFile("TroopArmyCell", this, this);
    this->setContentSize(node->getContentSize());
    CCLoadSprite::doResourceByCommonIndex(204, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(204, false);
    });
    m_dict = dict;
    refreshView();
    return true;
}

bool ResourceTroopInfo::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numTxt", CCLabelIF*, this->m_numTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelTxt", CCLabelIF*, this->m_levelTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelNode", CCNode*, this->m_levelNode);
    return false;
}

void ResourceTroopInfo::refreshView(){
    std::string itemId = m_dict->valueForKey("armyId")->getCString();
    int num = m_dict->valueForKey("count")->intValue();
    m_nameTxt->setString(CCCommonUtils::getNameById(itemId));
    m_nameTxt->setDimensions(CCSize(140, 0));
    m_numTxt->setString(CC_CMDITOA(num));

    m_iconNode->removeAllChildren();
    string picName = GlobalData::shared()->armyList[itemId].getHeadIcon();
//    auto pic = CCLoadSprite::createSprite(picName.c_str());
//    m_iconNode->addChild(pic);
//    pic->setPositionY(-10);
//    CCCommonUtils::setSpriteMaxSize(pic, 110);
    int star = ArmyController::getInstance()->getStarlvById(itemId);
    auto pic = SoldierIconCell::create(picName, 110,itemId,true,star);
    m_iconNode->addChild(pic);
    pic->setPositionY(-10);
    this->m_levelNode->removeAllChildren();
    string num1 = itemId.substr(itemId.size()-2);
//    auto pic1= CCCommonUtils::getRomanSprite(atoi(num1.c_str())+1);
//    m_levelNode->addChild(pic1);
    m_levelTxt->setString(CCCommonUtils::getChineseLevelTxtByNum(atoi(num1.c_str())+1).c_str());
}

void ResourceTroopInfo::onEnter(){
    
}

void ResourceTroopInfo::onExit(){
    
}
