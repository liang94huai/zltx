//
//  WorldResGetView.cpp
//  IF
//
//  Created by 刘田厚 on 16-11-30.
//
//




#include "WorldResGetView.h"
#include "WorldController.h"
#include "WorldCommand.h"
#include "WorldMapView.h"
#include "FlyCell.h"
#include "UIComponent.h"
#include "GlobalData.h"

WorldResGetView* WorldResGetView::create(WorldCityInfo& info){
    WorldResGetView *ret = new WorldResGetView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

 
bool WorldResGetView::init(){
    if(CCLayer::init()){
        
        auto tmpCCB = CCBLoadFile("worldResGetView",this,this,true);
        m_starTime = WorldController::getInstance()->getTime();
        unsigned int index = m_info.cityIndex;
        if(m_info.parentCityIndex != -1){
            index = m_info.parentCityIndex;
        }
        if(WorldController::getInstance()->getIsSelfResCity(index))
        {
            m_starTime = WorldController::getInstance()->m_resCityInfo[index].m_StartTime;
        }
       // getTimeInfo();
        
        initLabel();
        refreshData();
      }
        return true;
}
SEL_CCControlHandler WorldResGetView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGetBtnClick", WorldResGetView::onGetBtnClick);

}

bool WorldResGetView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeNode", CCNode*, this->m_timeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getNode", CCNode*, this->m_getNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getBtn", CCControlButton*, this->m_getBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flagLab", CCLabelIF*, this->m_flagLab);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progress", CCScale9Sprite*, this->m_progress);

    return false;
}

void WorldResGetView::onEnter(){
    CCLayer::onEnter();
    //CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void WorldResGetView::onExit(){
     
    CCLayer::onExit();
    this->unschedule(schedule_selector(WorldResGetView::updateTime));
}
    
    
    
void WorldResGetView::onGetBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    if(m_iState == 0)
    {
        return;
    }
    
    int uId = -1;
    //发送收获请求 id
   // m_info.resource.tId;
    unsigned int index = m_info.cityIndex;
    if(m_info.parentCityIndex != -1){
        index = m_info.parentCityIndex;
    }
    uId = index;
    MapGetCityResourceCommand * cmd=new MapGetCityResourceCommand(uId);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(WorldResGetView::GetResDataBack), NULL));
    cmd->sendAndRelease();
    //changeState(0);

}

void WorldResGetView::getTimeInfo()
{
    string marchId = "";
    map<string, MarchInfo>::iterator it;
    unsigned int index = m_info.cityIndex;
    if(m_info.parentCityIndex != -1){
        index = m_info.parentCityIndex;
    }
    for(auto it = WorldController::getInstance()->m_marchInfo.begin(); it != WorldController::getInstance()->m_marchInfo.end(); it++){
        if(it->second.endPointIndex == index){
            marchId = it->first;
            
        }
    }
    
//    auto cmd = new WorldMarchDetailUpdateCommand(marchId);
//    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(WorldResGetView::GetInfoDataBack), NULL));
//    cmd->sendAndRelease();
}
void WorldResGetView::GetInfoDataBack(CCObject *obj)
{
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        return;
    }
    
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    if (info && info->objectForKey("uuid")) {
        auto endStamp = info->valueForKey("endStamp")->doubleValue();
        //m_starTime = endStamp;
        refreshData();
        
    }
}
void WorldResGetView::GetResDataBack(CCObject *obj)
{
    //按照收获的种类 显示不同的tip
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        return;
    }
    
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    auto &resourceInfo = GlobalData::shared()->resourceInfo;
    
    int newFood = 0;
    int newWood = 0;
    int newIron = 0;
    int newStone = 0;
    string tipContent = _lang("new100040").c_str();
    auto pStr = info->valueForKey("food");
    if(pStr->compare("")!=0){
        newFood = pStr->intValue();
        if (newFood - resourceInfo.lFood > 0) {
            onFlyOutPut(m_info.cityIndex, newFood - resourceInfo.lFood, Food);
            tipContent.append(StringUtils::format(" %s + %s",_lang("102211").c_str(),CC_CMDITOAD(newFood - resourceInfo.lFood).c_str()));
        }
    }
    pStr = info->valueForKey("wood");
    if(pStr->compare("")!=0){
        newWood = pStr->intValue();
        if (newWood - resourceInfo.lWood > 0) {
            onFlyOutPut(m_info.cityIndex, newWood - resourceInfo.lWood, Wood);
             tipContent.append(StringUtils::format(" %s + %s",_lang("102209").c_str(),CC_CMDITOAD(newWood - resourceInfo.lWood).c_str()));
        }
    }
    pStr = info->valueForKey("iron");
    if(pStr->compare("")!=0){
        newIron = pStr->intValue();
        if (newIron - resourceInfo.lIron > 0) {
            onFlyOutPut(m_info.cityIndex, newIron - resourceInfo.lIron, Iron);
             tipContent.append(StringUtils::format(" %s + %s",_lang("102210").c_str(),CC_CMDITOAD(newIron - resourceInfo.lIron).c_str()));
        }
    }
    pStr = info->valueForKey("stone");
    if(pStr->compare("")!=0){
        newStone = pStr->intValue();
        if (newStone - resourceInfo.lStone > 0) {
            onFlyOutPut(m_info.cityIndex, newStone - resourceInfo.lStone, Stone);
             tipContent.append(StringUtils::format(" %s + %s",_lang("102208").c_str(),CC_CMDITOAD(newStone - resourceInfo.lStone).c_str()));
        }
    }
    
    resourceInfo.setResourceData(info);
    
    //tip
    CCCommonUtils::flyHint("", "", tipContent, 3, 0, true);

    m_starTime = WorldController::getInstance()->getTime() ;
    auto cmd_2 = new MapGetCitySmallCommand();
     //再次请求下 来 刷新
    cmd_2->sendAndRelease();
    refreshData();
}

void WorldResGetView::refreshData()
{
    m_getBtn->setEnabled(false);
    m_getNode->setVisible(false);
    m_timeNode->setVisible(false);
    
    m_iState = 1;
    updateTime(0);
}    
     
void WorldResGetView::initLabel()
{
    m_timeLabel->setFntFile("Arial_Bold_Border.fnt");
    
    m_flagLab->setFntFile("Arial_Bold_Border.fnt");

}

void WorldResGetView::updateTime(float dt)
{
    auto nowtime = WorldController::getInstance()->getTime()/1000;
    auto starttime = m_starTime/1000;
    int dtTime = nowtime - starttime;
    int id = m_info.resource.tId;
    auto dict = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(id));
    int needtime = dict->valueForKey("resTime")->intValue();
    if(id == 0)
    {
         this->unschedule(schedule_selector(WorldResGetView::updateTime));
        closeSelf();
    }
    if(needtime <= dtTime)
    {
        //stop change
        changeState(1);
    }else{
        string strTime = _lang_1("new100033",CC_SECTOA(-dtTime+needtime));
        m_timeLabel->setString(strTime.c_str());
        double offscale = (dtTime*1.0)/needtime;
        m_progress->setScaleX(offscale);
        if (m_iState != 0)
            changeState(0);
    }

}

void WorldResGetView::changeState(int type) //0, 倒计时,1 可以领取状态 并停止计时器
{
    m_iState = type;
   
    switch (m_iState) {
        case 0:
            
            this->schedule(schedule_selector(WorldResGetView::updateTime), 1, -1, 1);
            m_getBtn->setEnabled(false);
            m_getNode->setVisible(false);
            m_timeNode->setVisible(true);
            stopBtnAni();
            break;
        case 1:
            this->unschedule(schedule_selector(WorldResGetView::updateTime));
            plyBtnAni();
            m_getBtn->setEnabled(true);
            m_getNode->setVisible(true);
            m_timeNode->setVisible(false);
            ;
            break;
            
        default:
            break;
    }
}

void WorldResGetView::stopBtnAni()
{

    m_getNode->stopActionByTag(4);
}

void WorldResGetView::plyBtnAni()
{
    CCRotateBy* rt1 = CCRotateBy::create(0.1, 30);
    CCRotateBy* rt2 = CCRotateBy::create(0.1, -60);
    CCRotateBy* rt3 = CCRotateBy::create(0.1, 44);
    CCRotateBy* rt4 = CCRotateBy::create(0.1, -28);
    CCRotateBy* rt5 = CCRotateBy::create(0.1, 14);
    CCDelayTime* dt = CCDelayTime::create(1);
    CCSequence* seq = CCSequence::create(rt1, rt2, rt3, rt4, rt5, dt, NULL);
    CCAction* action = CCRepeatForever::create(seq);
    action->setTag(4);
    m_getNode->runAction(action);

}
void WorldResGetView::closeSelf()
{
    this->removeFromParentAndCleanup(true);

} 


void WorldResGetView::onFlyOutPut(int cityIndex, int output, int forceResType)
{
    cityIndex = cityIndex - WorldController::getInstance()->_current_tile_count_x - 1;
    auto tilePoint = WorldController::getPointByIndex(cityIndex);
    auto pMap = WorldMapView::instance()->m_map;
    auto vpbp = pMap->getViewPointByTilePoint(tilePoint) * pMap->getScale();
    auto motifyMapPos = pMap->getPosition();
    auto pt = ccpAdd(vpbp,motifyMapPos);
//    pt = pt * pMap->getScale();
//    CCLOG("vpbp : %f, %f",vpbp.x, vpbp.y);
//    CCLOG("motifyMapPos : %f, %f",motifyMapPos.x, motifyMapPos.y);
//    CCLOG("pt : %f, %f",pt.x, pt.y);
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    pt.y = pt.y-winSize.height;
    string picStr = "Food";
    auto endPt = ccp(0,0);
    int resType = Wood;
    if (forceResType==Food) {
        endPt = ccp(200,-60);//winSize.height
        if(CCCommonUtils::isIosAndroidPad())
        {
            endPt = ccpMult(endPt, 2.4);
        }
        picStr = "Food";
        resType = Food;
    }else if (forceResType==Wood) {
        endPt = ccp(90,-40);
        if(CCCommonUtils::isIosAndroidPad())
        {
            endPt = ccpMult(endPt, 2.4);
        }
        picStr = "Wood";
        resType = Wood;
    }
    else if (forceResType==Iron) {
        endPt = ccp(320,-40);
        if(CCCommonUtils::isIosAndroidPad())
        {
            endPt = ccpMult(endPt, 2.4);
        }
        picStr = "Iron";
        resType = Iron;
    }
    else if (forceResType==Stone) {
        endPt = ccp(430,-40);
        if(CCCommonUtils::isIosAndroidPad())
        {
            endPt = ccpMult(endPt, 2.4);
        }
        picStr = "Stone";
        resType = Stone;
    }
    
    srand((unsigned)time(0));
    int tmp = output*10/1000;
    tmp = MAX(tmp, 2);
    tmp = MIN(tmp, 10);
    int idx = 0;
    while (tmp>0) {
        
        auto flyCell = FlyCell::create(pt, endPt, resType, picStr, 5, UIComponent::getInstance()->m_collectBatch, idx*0.2);
        UIComponent::getInstance()->m_collectNode->addChild(flyCell);
        tmp--;
        idx++;
    }
}
