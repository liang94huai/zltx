//
//  UpstarView.cpp
//  IF
//
//  Created by 李朝辉 on 15-8-25.
//
//

#include "UpstarView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "ToolController.h"
#include "QueueController.h"
#include "UseResToolView.h"
#include "WatchtowerView.h"
#include "MainCityView.h"
#include "BuildingHospitalPopUpView.h"
#include "ScienceListView.h"
#include "BuildMoreInfoView.h"
#include "StoreView.h"
#include "SceneController.h"
#include "ImperialScene.h"
#include "BatchBuyItemView.h"
#include "ConfirmDialogView.h"
#include "SoundController.h"
#include "CCFlagWaveSprite.h"
#include "ArmyInfo.h"
#include "GuideController.h"
#include "DynamicResourceController.h"

#define MSG_BUILD_CELL "msg_build_cell"
#define ALLTIME 4

UpstarView* UpstarView::create(int buildId, int pos){
    UpstarView* ret = new UpstarView();
    if(ret && ret->init(buildId, pos)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool UpstarView::init(int buildId, int pos)
{
    if (!ArcPopupBaseView::init(TYPE_POS_UP_LITTLE)) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(510, true);
    DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_GOODS_TEXTURE,false);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(510, false);
        DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_GOODS_TEXTURE,true);
    });

    m_isReturn = true;
    m_buildId = buildId;
    m_pos = pos;
    auto tmpCCB = CCBLoadFile("UpstarView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    m_inBtnGoldNum->setString("1");
    m_demandLabel->setString(_lang("102130"));
    m_scrollView = CCScrollView::create(m_infoList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_infoList->addChild(m_scrollView);
    updateInfo(NULL);
    m_openNum=0;
    m_upBtnMsgLabel->setString(_lang("102104").c_str());
    m_btnMsgLabel->setString(_lang("102127").c_str());
    vector<CCNode*> tmpVec;
    CCCommonUtils::getRGBAProtocols(m_mainNode, &tmpVec);
    for (int i=0; i<tmpVec.size(); i++) {
        CCActionInterval * fadeIn = CCFadeIn::create(0.5);
        tmpVec[i]->runAction(fadeIn);
    }
    m_arrowNode->setVisible(false);
    auto& m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);//进度条显示
    StarRateCur = m_info.starRate;
    this->scheduleOnce(schedule_selector( UpstarView::BrownProgressShow), 0); //进度的延时效果显示
    this->scheduleOnce(schedule_selector( UpstarView::YellowProgressShow), 0.4); //进度的延时效果显示
    TextReloc();//进度显示的定位
    FameShow();//定位荣誉勋章
    starUpInfo();//添加文本及数据信息
    helpLoc(); //help的自适应位置确定

    GuideController::share()->checkSubGuide(GUIDE_FIRSTSHOW_STAR);

    return true;
}
void UpstarView::TextReloc(){
    switch((int)StarRateCur){
        case 0:
            m_lvLabel->setPosition(ccp(22.7,196.6));
            break;
        case 10:
            m_lvLabel->setPosition(ccp(64.7,209.6));
            break;
        case 20:
            m_lvLabel->setPosition(ccp(105.7,212.6));
            break;
        case 30:
            m_lvLabel->setPosition(ccp(151.7,203.6));
            break;
        case 40:
            m_lvLabel->setPosition(ccp(192.7,181.6));
            break;
        case 50:
            m_lvLabel->setPosition(ccp(228.7,156.6));
            break;
        case 60:
            m_lvLabel->setPosition(ccp(249.7,125.6));
            break;
        case 70:
            m_lvLabel->setPosition(ccp(261.7,90.6));
            break;
        case 80:
            m_lvLabel->setPosition(ccp(253.7,52.6));
            break;
        case 90:
            m_lvLabel->setPosition(ccp(236.7,18.6));
            break;
        case 100:
            m_lvLabel->setPosition(ccp(211.7,-15.4));
            break;
        default:
            m_lvLabel->setPosition(ccp(-100,-100));
            break;
    }
    std:: string strtemp = CC_ITOA(StarRateCur);
    strtemp += "%";
    m_lvLabel->setString(strtemp.c_str());
    
}

void UpstarView::FameShow(){
    auto fame = CCLoadSprite::createSprite("rongyuxunzhang_hui.png");
    fame->setPosition(ccp(243.9,-37.2));
    m_prograssShowNode->addChild(fame,10);
}

void UpstarView::BrownProgressShow(float time){
    auto& m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
    auto spr = CCLoadSprite::createSprite("zongditiao.png");
    CCProgressTimer *pt=CCProgressTimer::create(spr);
    pt->setMidpoint(ccp(0.5,0.5));
    pt->setPosition(ccp(-2,-40));
    pt->setType(cocos2d::CCProgressTimerType(kCCProgressTimerTypeRadial));
    m_prograssShowNode->addChild(pt,1);
    CCProgressTo *t=CCProgressTo::create(ALLTIME*(m_info.starRate+10)/100 ,(StarRateCur+10)*90/360);  //100为进度条所占度数
    pt->setScale(1.8);
    pt->runAction(t);
}

void UpstarView::playParticle(){
    CCPoint pt0(19.4,218.5);
    CCPoint pt1(46.2,225.4);
    CCPoint pt2(98.6,234.8);
    CCPoint pt3(143.6,231.2);
    CCPoint pt4(195.0,212.6);
    CCPoint pt5(239.2,184.0);
    CCPoint pt6(272.2,148.3);
    CCPoint pt7(288.6,100.8);
    CCPoint pt8(288.6,58.0);
    CCPoint pt9(274.2,4.4);
    CCPoint pt10(247.2,-34.8);
    
    m_pointArray = CCPointArray::create(11);
    m_pointArray->addControlPoint(pt0);
    m_pointArray->addControlPoint(pt1);
    m_pointArray->addControlPoint(pt2);
    m_pointArray->addControlPoint(pt3);
    m_pointArray->addControlPoint(pt4);
    m_pointArray->addControlPoint(pt5);
    m_pointArray->addControlPoint(pt6);
    m_pointArray->addControlPoint(pt7);
    m_pointArray->addControlPoint(pt8);
    m_pointArray->addControlPoint(pt9);
    m_pointArray->addControlPoint(pt10);
    
    PaticleEff1= ParticleController::createParticle(CCString::createWithFormat("RaisingStars_%d",1)->getCString());  //--sun粒子效果
    PaticleEff1->setScale(1.5);
    PaticleEff1->setPosition(pt0);
    m_prograssShowNode->addChild(PaticleEff1,1);
    PaticleEff2= ParticleController::createParticle(CCString::createWithFormat("RaisingStars_%d",2)->getCString());
    PaticleEff2->setScale(1.5);
    PaticleEff2->setPosition(pt0);
    m_prograssShowNode->addChild(PaticleEff2,2);
    
    float dt = ALLTIME*1.0/10;
    CCActionInterval *forward2 = CCMoveTo::create(dt, pt1);
    CCActionInterval *forward3 = CCMoveTo::create(dt, pt2);
    CCActionInterval *forward4 = CCMoveTo::create(dt, pt3);
    CCActionInterval *forward5 = CCMoveTo::create(dt, pt4);
    CCActionInterval *forward6 = CCMoveTo::create(dt, pt5);
    CCActionInterval *forward7 = CCMoveTo::create(dt, pt6);
    CCActionInterval *forward8 = CCMoveTo::create(dt, pt7);
    CCActionInterval *forward9 = CCMoveTo::create(dt, pt8);
    CCActionInterval *forward10 = CCMoveTo::create(dt, pt9);
    CCActionInterval *forward11 = CCMoveTo::create(dt, pt10);
    CCCallFunc* call  = CCCallFunc::create(this, callfunc_selector(UpstarView::callback));
    CCCallFunc* call2  = CCCallFunc::create(this, callfunc_selector(UpstarView::callback2));
    //下面这行代码是创建一个动作序列
    m_progresscnt = 0;
    m_progresscnt_2 = 0;
    sc = CCSequence::create(call,forward2,call,forward3,call,forward4,call,forward5,call,forward6,call,
                            forward7,call,forward8,call,forward9,call,forward10,call,forward11,call,NULL);
    sc2 = CCSequence::create(call2,forward2->clone(),call2,forward3->clone(),call2,forward4->clone(),
                             call2,forward5->clone(),call2,forward6->clone(),call2,forward7->clone(),call2,
                             forward8->clone(),call2,forward9->clone(),call2,forward10->clone(),call2,
                             forward11->clone(),call2,NULL);
    
    PaticleEff1->runAction(sc);
    PaticleEff2->runAction(sc2);
}

void UpstarView::YellowProgressShow(float time){  //黄色显示的是0 10% 20%。。。100% 离散数据
    auto& m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
    auto spr = CCLoadSprite::createSprite("yellowditiao.png");
    CCProgressTimer* pt=CCProgressTimer::create(spr);
    pt->setMidpoint(ccp(0.5,0.5));
    pt->setPosition(ccp(-2,-40));
    pt->setType(cocos2d::CCProgressTimerType(kCCProgressTimerTypeRadial));
    m_prograssShowNode->addChild(pt,1);
    CCProgressTo *m_proto=CCProgressTo::create(ALLTIME*m_info.starRate/100,StarRateCur*90/360);  //100为进度条所占度数
    pt->setScale(1.8);
    pt->runAction(m_proto);
    playParticle();
}
void UpstarView::callback(){
    if(m_progresscnt*10 == (int)StarRateCur){
        PaticleEff1->stopAction(sc);
        m_prograssShowNode->removeChild(PaticleEff1);
        if((int)StarRateCur == 90)
        {
//            pt->stopAction(m_proto);
            auto fame = CCLoadSprite::createSprite("rongyuxunzhang.png");
            fame->setPosition(ccp(243.9,-37.2));
            m_seek->addChild(fame,20);
            this->scheduleOnce(schedule_selector( UpstarView::StopTimeParticle),0.01);
        }
    }
    m_progresscnt++;
}
void UpstarView::callback2(){
    CCLOG("judge");
    if(m_progresscnt_2*10 == StarRateCur){
        PaticleEff2->stopAction(sc2);
        m_prograssShowNode->removeChild(PaticleEff2);
        PaticleEff2= ParticleController::createParticle(CCString::createWithFormat("RaisingStars_%d",2)->getCString());
        if(m_progresscnt_2 == 10){
            PaticleEff2->setScale(3);
        }
        else if(m_progresscnt_2 == 8 || m_progresscnt_2 == 9){
            PaticleEff2->setScale(2);
        }
        else{
            PaticleEff2->setScale(1.5);
        }
        PaticleEff2->setPosition(m_pointArray->getControlPointAtIndex(m_progresscnt_2));
        m_prograssShowNode->addChild(PaticleEff2,2);
    }
    m_progresscnt_2++;
}
void UpstarView::StopTimeParticle(float time){
    for (int i=1; i<7; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("RaisingStars_fin_%d",i)->getCString());  //--sun粒子效果
        particle->setPosition(ccp(243.9,-37.2));
        particle->setScale(1);
        m_seek->addChild(particle);
    }
}
void UpstarView::ParticalShow(float time){
    for (int i=1; i<3; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("FinalRaisingStars_%d",i)->getCString());  //--sun粒子效果
        particle->setPosition(ccp(243.9,-37.2));
        particle->setScale(2);
        m_prograssShowNode->addChild(particle);
    }
}


void UpstarView::updateInfo(CCObject* obj)
{
    bool isCanUp = true;
    
    string name = "";
    int silver_need = 0;
    int wood_need = 0;
    int time_need = 0;
    int stone_need = 0;
    int food_need = 0;
    int iron_need = 0;
    string tool_need = "";
    string building = "";
    string destip = "";
    string star_building = "";
    int level = 0;
    int star_num = 0;
    float star_rate = 0;
    
    
    if (m_pos>0) {
        auto dictInfo = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(m_buildId));
        name = dictInfo->valueForKey("name")->getCString();
        silver_need = dictInfo->valueForKey("silver")->intValue();
        wood_need = dictInfo->valueForKey("wood")->intValue();
        time_need = dictInfo->valueForKey("time")->intValue();
        stone_need = dictInfo->valueForKey("stone")->intValue();
        food_need = dictInfo->valueForKey("food")->intValue();
        iron_need = dictInfo->valueForKey("iron")->intValue();
        tool_need = dictInfo->valueForKey("item")->getCString();
        building = dictInfo->valueForKey("building")->getCString();
        destip = dictInfo->valueForKey("starDescription")->getCString();
        star_num = dictInfo->valueForKey("starNum")->intValue();
        star_rate = dictInfo->valueForKey("starRate")->floatValue();
        star_building = dictInfo->valueForKey("star_building")->getCString();
    }
    else {
        auto& m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
        name = m_info.name;
        silver_need = m_info.silver_need;
        wood_need = m_info.wood_need;
        time_need = m_info.time_need;
        stone_need = m_info.stone_need;
        food_need = m_info.food_need;
        iron_need = m_info.iron_need;
        tool_need = m_info.tool_need;
        building = m_info.building;
        star_building = m_info.star_building;
        
        level = m_info.level;
        destip = m_info.tip;
        star_num = m_info.starNum;
        star_rate  = m_info.starRate;
    }
    _tmpTime = time_need;
    m_nameLabel->setString(_lang(name));
    std::string strtemp = CC_ITOA(star_num);
    m_StarNum->setString(strtemp.c_str());//当前星个数
    strtemp = CC_ITOA(star_num+1);
    m_StarNum1->setString(strtemp.c_str());//下一个星个数
    

    if (m_pos>0) {
        if (CCCommonUtils::isIosAndroidPad()) {
            m_nameLabel->setPositionY(m_nameLabel->getPositionY() + 19.4);  //名称
        } else {
            m_nameLabel->setPositionY(m_nameLabel->getPositionY()+10);
        }
    }
    float tmpEffect = CCCommonUtils::getEffectValueByNum(68);
    _tmpTime = time_need/(1+CCCommonUtils::getEffectValueByNum(68)*1.0/100);
    _tmpGold = CCCommonUtils::getGoldByTime(_tmpTime);
    _resGold = 0;
    
    string timeInfo = CC_SECTOA(_tmpTime);   //所需时间
    m_timeLabel->setString(timeInfo.c_str());
    
    int _itemH = 46;
    int curY = 25;
    int curX = 30;
    
    if (CCCommonUtils::isIosAndroidPad()) {
        _itemH = 46;
        curX = 25;
        curY = 30;
    }
    
    m_instantBtn->setEnabled(true);
    m_scrollView->getContainer()->removeAllChildren();
    
    m_lfood = 0;
    m_lwood = 0;
    m_liron = 0;
    m_lstone = 0;
    
    if (stone_need>0) {
        auto item3 = UpstarCell::create(0, Stone, stone_need);
        if(CCCommonUtils::isIosAndroidPad()){
            item3->setScale(0.4);
        }
        item3->setPosition(ccp(curX, curY));
        item3->setTouchNode(m_infoList,m_buildId);
        m_scrollView->addChild(item3);
        curY += _itemH;
        
        if (!item3->m_isOk) {
            //            isCanUp = false;
        }
        
        int tmpSub = stone_need - GlobalData::shared()->resourceInfo.totalStone();
        if (tmpSub>0) {
            _resGold += CCCommonUtils::getResGlodByType(Stone, tmpSub);
            m_lstone = tmpSub;
        }
    }
    if (iron_need>0) {
        auto item4 = UpstarCell::create(0, Iron, iron_need);
        if(CCCommonUtils::isIosAndroidPad()){
            item4->setScale(0.4);
        }
        item4->setPosition(ccp(curX, curY));
        item4->setTouchNode(m_infoList,m_buildId);
        m_scrollView->addChild(item4);
        curY += _itemH;
        
        if (!item4->m_isOk) {
            //            isCanUp = false;
        }
        
        int tmpSub = iron_need - GlobalData::shared()->resourceInfo.totalIron();
        if (tmpSub>0) {
            _resGold += CCCommonUtils::getResGlodByType(Iron, tmpSub);
            m_liron = tmpSub;
        }
    }
    if (food_need>0) {
        auto item5 = UpstarCell::create(0, Food, food_need);
        if(CCCommonUtils::isIosAndroidPad()){
            item5->setScale(0.4);
        }
        item5->setPosition(ccp(curX, curY));
        item5->setTouchNode(m_infoList,m_buildId);
        m_scrollView->addChild(item5);
        curY += _itemH;
        
        if (!item5->m_isOk) {
            //            isCanUp = false;
        }
        
        int tmpSub = food_need - GlobalData::shared()->resourceInfo.totalFood();
        if (tmpSub>0) {
            _resGold += CCCommonUtils::getResGlodByType(Food, tmpSub);
            m_lfood = tmpSub;
        }
    }
    
    
    if (wood_need>0) {
        auto item2 = UpstarCell::create(0, Wood, wood_need);
        if(CCCommonUtils::isIosAndroidPad()){
            item2->setScale(0.4);
        }
        item2->setPosition(ccp(curX, curY));
        item2->setTouchNode(m_infoList,m_buildId);
        m_scrollView->addChild(item2);
        curY += _itemH;
        
        if (!item2->m_isOk) {
            //            isCanUp = false;
        }
        
        int tmpSub = wood_need - GlobalData::shared()->resourceInfo.totalWood();
        if (tmpSub>0) {
            _resGold += CCCommonUtils::getResGlodByType(Wood, tmpSub);
            m_lwood = tmpSub;
        }
    }
    
    if (tool_need != "") {
        std::vector<string> toolItems;
        CCCommonUtils::splitString(tool_need, "|", toolItems);
        for (int i=0; i<toolItems.size(); i++) {
            string curTool = toolItems[i];
            std::vector<string> item;
            CCCommonUtils::splitString(curTool, ";", item);
            int id = atoi(item[0].c_str());
            int num = atoi(item[1].c_str());
            
            auto itemTool = UpstarCell::create(1, id, num);
            if(CCCommonUtils::isIosAndroidPad()){
                itemTool->setScale(0.4);
            }
            itemTool->setPosition(ccp(curX, curY));
            itemTool->setTouchNode(m_infoList,m_buildId);
            m_scrollView->addChild(itemTool);
            curY += _itemH;
            
            if (!itemTool->m_isOk) {
                auto toolInfo = ToolController::getInstance()->getToolInfoById(id);
                _resGold += (num - toolInfo.getCNT()) * toolInfo.price;
                //                isCanUp = false;
            }
        }
    }
    
    _tmpGold = _tmpGold + _resGold;
    m_inBtnGoldNum->setString(CC_ITOA(_tmpGold));

    
    if (building!="") {
        std::vector<string> buildItems;
        CCCommonUtils::splitString(building, "|", buildItems);
        for (int i=0; i<buildItems.size(); i++) {
            string curBuild = buildItems[i];
            std::vector<string> tmpbuildVec;
            CCCommonUtils::splitString(curBuild, ";", tmpbuildVec);
            int paramBuild = atoi(tmpbuildVec[0].c_str())+atoi(tmpbuildVec[1].c_str());
            auto itemBuild = UpstarCell::create(2, paramBuild, 0);
            if(CCCommonUtils::isIosAndroidPad()){
                itemBuild->setScale(0.4);
            }
            itemBuild->setPosition(ccp(curX, curY));
            itemBuild->setTouchNode(m_infoList,m_buildId);
            m_scrollView->addChild(itemBuild);
            curY += _itemH;
            
            if (!itemBuild->m_isOk) {
                isCanUp = false;
                m_instantBtn->setEnabled(false);
            }
        }
    }
//    star_building = "414001,412001,413001";
    if (star_building!="") {
        std::vector<string> buildItems;
        CCCommonUtils::splitString(star_building, ",", buildItems);
        for (int i=0; i<buildItems.size(); i++) {
            string curBuild = buildItems[i];
            int paramBuild = atoi(curBuild.c_str());
            auto itemBuild = UpstarCell::create(8, paramBuild, 0);
            if(CCCommonUtils::isIosAndroidPad()){
                itemBuild->setScale(0.4);
            }
            itemBuild->setPosition(ccp(curX, curY));
            itemBuild->setTouchNode(m_infoList,m_buildId);
            m_scrollView->addChild(itemBuild);
            curY += _itemH;
            if (!itemBuild->m_isOk) {
                isCanUp = false;
                m_instantBtn->setEnabled(false);
            }
        }
    }
    
    //没有队列，显示条件不足
    int qid = QueueController::getInstance()->canMakeItemByType(TYPE_BUILDING, 1);//_tmpTime, 取空闲队列，并不判断有效期是否够用
    if(QID_MAX == qid)
    {
        int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_BUILDING);
        auto itemQueue = UpstarCell::create(3, Food, qid);
        if(CCCommonUtils::isIosAndroidPad()){
            itemQueue->setScale(0.4);
        }
        itemQueue->setPosition(ccp(curX, curY));
        itemQueue->setTouchNode(m_infoList,m_buildId);
        m_scrollView->addChild(itemQueue);
        curY += _itemH;
        m_qid = qid;
    }
    else {
        m_qid = qid;
    }
    m_upBtn->setEnabled(isCanUp);
    curY -= 23;
    m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,curY));
    m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - curY));
    bool touchable = curY < m_infoList->getContentSize().height?false:true;
    m_scrollView->setTouchEnabled(touchable);
    
}

void UpstarView::onEnter()
{
    CCNode::onEnter();
    UIComponent::getInstance()->showResourceBar(true);
    if (m_openNum>0) {
        updateInfo(NULL);
    }
    
    m_openNum++;
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UpstarView::updateInfo), MSG_REFREASH_BUILD_UPGRADE, NULL);
    setTouchEnabled(true);
   // CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void UpstarView::onExit()
{
    UIComponent::getInstance()->showResourceBar(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REFREASH_BUILD_UPGRADE);
    setTouchEnabled(false);
   // CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCNode::onExit();
}

void UpstarView::onTipBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    GlobalData::shared()->isBind = true;
#endif
    CCCommonUtils::showHelpShiftSingleFAQ("45215");
}

bool UpstarView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
//    m_x = m_itemScrollView->getContentOffset().x;
    return true;
}
void UpstarView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
}
void UpstarView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
}

SEL_CCControlHandler UpstarView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCreateOrUpClick", UpstarView::onCreateOrUpClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInstantClick", UpstarView::onInstantClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipBtnClick", UpstarView::onTipBtnClick);
    return NULL;
}

bool UpstarView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upBtn", CCControlButton*, this->m_upBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_instantBtn", CCControlButton*, this->m_instantBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipBtn", CCControlButton*, this->m_tipBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inBtnNode", CCNode*, this->m_inBtnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode", CCNode*, this->m_btnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_condition", CCNode*, this->m_condition);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flagdata", CCNode*, this->m_flagdata);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnMsgLabel", CCLabelIF*, this->m_btnMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inBtnGoldNum", CCLabelIF*, this->m_inBtnGoldNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_StarNum", CCLabelIF*, this->m_StarNum);
     CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_StarNum1", CCLabelIF*, this->m_StarNum1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvLabel", CCLabelIF*, this->m_lvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upBtnMsgLabel", CCLabelIF*, this->m_upBtnMsgLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_demandLabel", CCLabelIF*, this->m_demandLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrowNode", CCNode*, this->m_arrowNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rArrow", CCSprite*, this->m_rArrow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lArrow", CCSprite*, this->m_lArrow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_prograssShowNode", CCNode*, this->m_prograssShowNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_seek", CCNode*, this->m_seek);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_proText", CCLabelIF*, this->m_proText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_curpro", CCLabelIF*, this->m_curpro);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inc1", CCLabelIF*, this->m_inc1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_onetext", CCLabelIF*, this->m_onetext);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_oneleft", CCLabelIF*, this->m_oneleft);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_oneright", CCLabelIF*, this->m_oneright);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_twotext", CCLabelIF*, this->m_twotext);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_twoleft", CCLabelIF*, this->m_twoleft);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tworight", CCLabelIF*, this->m_tworight);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_threetext", CCLabelIF*, this->m_threetext);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_threeleft", CCLabelIF*, this->m_threeleft);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_threeright", CCLabelIF*, this->m_threeright);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fourtext", CCLabelIF*, this->m_fourtext);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fourleft", CCLabelIF*, this->m_fourleft);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fourright", CCLabelIF*, this->m_fourright);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_para1Node", CCNode*, this->m_para1Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_para2Node", CCNode*, this->m_para2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_para3Node", CCNode*, this->m_para3Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_para4Node", CCNode*, this->m_para4Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desToStar", CCLabelIF*, this->m_desToStar);
    return false;
}

CCNode *UpstarView::getGuideNode(int itemId){
    if((m_buildId==itemId || m_buildId / 1000 == itemId) && m_upBtn->isEnabled()){
        return m_upBtn;
    }
    return NULL;
}

void UpstarView::onInstantClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    if (m_pos <=0) {
        auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
        if (m_info.type == FUN_BUILD_MAIN && m_info.level==(GlobalData::shared()->shield_base-1)) {
            YesNoDialog::show( _lang_1("108588", CC_ITOA(GlobalData::shared()->shield_base)) , CCCallFunc::create(this, callfunc_selector(UpstarView::showWarningInstantUp)));
        }
        else {
            showWarningInstantUp();
        }
    }
    else {
        showWarningInstantUp();
    }
}
void UpstarView::showWarningInstantUp(){
    std::string name = "";
    
    if(m_pos <=0){
        auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
        name = m_info.name;
    }else{
        auto dictInfo = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(m_buildId));
        name = dictInfo->valueForKey("name")->getCString();
    }
    std::string showString = _lang_1("102498",_lang(name).c_str());
    YesNoDialog::showButtonAndGold(showString.c_str(), CCCallFunc::create(this, callfunc_selector(UpstarView::onOkInstantUp)),m_btnMsgLabel->getString().c_str(), _tmpGold);
}
void UpstarView::onOkInstantUp()
{
    SoundController::sharedSound()->playEffects(Music_Sfx_city_build);
    
    for (int j = 1; j<willUnlockItems.size(); j++)
    {
        CCUserDefault::sharedUserDefault()->setStringForKey(willUnlockItems[j].c_str(),"1");
    }
    CCUserDefault::sharedUserDefault()->flush();
    
    if (m_pos <=0) {
        auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
        int upstar = 1;
        bool force = false;
        if (m_info.state == FUN_BUILD_NORMAL && FunBuildController::getInstance()->startUpFunBuild(m_buildId,_tmpGold,force,upstar))
        {
            m_isReturn = false;
//            PopupViewController::getInstance()->removeAllPopupView();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_GOBACK);
            PopupViewController::getInstance()->removeAllPopupView();
        }
    }
    else {
        if(FunBuildController::getInstance()->startOpenFunBuild(m_buildId, m_pos, _tmpGold))
        {
            m_isReturn = false;
            auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
            if( layer )
                layer->hideTmpBuild(m_pos);
//            PopupViewController::getInstance()->removeAllPopupView();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_GOBACK);
            PopupViewController::getInstance()->removeAllPopupView();
        }
    }
}

void UpstarView::onGotoSpdView()
{
    int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_BUILDING);
    string key = GlobalData::shared()->allQueuesInfo[qid].key;
    int tmpTime = GlobalData::shared()->allQueuesInfo[qid].finishTime - GlobalData::shared()->getWorldTime();
    int gold = CCCommonUtils::getGoldByTime(tmpTime);
    if (tmpTime<=GlobalData::shared()->freeSpdT) {
        gold = 0;
    }
    FunBuildController::getInstance()->costCD(atoi(key.c_str()), "", gold);
}

void UpstarView::onYesRent()
{
//    QueueController::getInstance()->rentQueue(m_qid, m_rentTime);
    if(m_bRent==true){
        QueueController::getInstance()->rentQueue(BUILD_QUEUE_2ND, m_rentTime,m_bRent);
    }else{
        QueueController::getInstance()->rentQueue(m_qid, m_rentTime,m_bRent);
    }
    m_bRent=false;
}

void UpstarView::showRentQueueDialog(float lastTime){
    if (lastTime < _tmpTime) { //第二队列剩余时间不满足升级所需时间，此时提示租用第二队列
        lastTime = MAX(0, lastTime);
        int needTime = _tmpTime - lastTime;
        const int baseTime = FunBuildController::getInstance()->building_base_k8;
        int needDay = ceil(needTime*1.0/86400);
        int minDay = baseTime / 86400 ;
        needDay = MAX(needDay, minDay);
        int day = 0;
        if(needDay%minDay!=0){
            int tmp = floor(needDay*1.0/minDay);
            day = tmp+1;
            needDay = day * minDay;
        }else{
            day = needDay/minDay;
        }
        needTime = day * baseTime;
        int needCost = day * FunBuildController::getInstance()->building_base_k9;
        m_rentTime = needTime;
        
        std::string showTip = _lang_1("102297", CC_ITOA(needDay));
        std::string t2 = "";
        if (m_pos <=0) {
            auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
            if(m_info.level > 0){
                t2 = _lang_1("102502",_lang(m_info.name).c_str());
            }else{
                t2 = _lang_1("102503",_lang(m_info.name).c_str());
            }
        }
        else
        {
            auto dictInfo = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(m_buildId));
            t2 = _lang_1("102503",_lang(dictInfo->valueForKey("name")->getCString()).c_str());
        }
        YesNoDialog::showQueueConfirm(showTip.c_str(),CCCallFunc::create(this, callfunc_selector(UpstarView::onYesRent)),_lang("102501").c_str(),t2.c_str(),lastTime,_tmpTime,_lang("102174").c_str(),needCost);
    }else{ //提示秒二号队列
        CCCommonUtils::flyHint("icon_time.png", "", _lang("160261"));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUILD_CELL);
    }
}


void UpstarView::onCreateOrUpClick(cocos2d::CCObject * pSender, CCControlEvent pCCControlEvent)
{
    int qid = QueueController::getInstance()->canMakeItemByType(TYPE_BUILDING, _tmpTime);
    if(QID_MAX == qid)
    {
        bool st = true;
        if (m_qid != QID_MAX) {
            auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
            if (qInfo.endTime!=0) { //获取的是2号队列但是已开启，可能空闲， 也 可能不空闲但是秒cd时间 要小于 1号队列
                int lastTime = qInfo.endTime - GlobalData::shared()->getWorldTime();
                showRentQueueDialog(lastTime); // 如果二号队列 剩余有效时间能满足 ，提示秒2号队列； 不满足，提示租用
                m_bRent=false;
                st = false;
            }
            else
            {
                if(FunBuildController::getInstance()->getMainCityLv() >= 3){
                    auto& qInfo = GlobalData::shared()->allQueuesInfo[BUILD_QUEUE_2ND];
                    int curTime = GlobalData::shared()->getWorldTime();
                    if(qInfo.endTime !=0 && qInfo.endTime <= curTime){//租用二号队列，目前二号队列 未开启
                        m_bRent=true;
                        showRentQueueDialog(0);
                        st = false;
                    }
                    else { //二号队列有效 且 二号队列正在使用中 且 m_qid 是一号队列， 一号队列正在使用中， 一号队列的秒cd时间最小
                        //提示秒一号队列， 即 提示秒 m_qid 的队列
                        CCCommonUtils::flyHint("icon_time.png", "", _lang("160261"));
                        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUILD_CELL);
                    }
                }
                else {
                    //二号队列暂时不能开启，可以提示 3级城堡后开启二级队列， 或者 提示 秒一号队列cd
                    CCCommonUtils::flyHint("icon_time.png", "", _lang("102120"));
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUILD_CELL);
                }
            }
        }
//-----------------
//        bool st = true;
//        if (m_qid != QID_MAX) {
//            auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
//            if (qInfo.endTime!=0) {
//                //可租用的队列
//                int lastTime = qInfo.endTime - GlobalData::shared()->getWorldTime();
//                if (lastTime < _tmpTime) {
//                    lastTime = MAX(0, lastTime);
//                    int needTime = _tmpTime - lastTime;
//                    const int baseTime = FunBuildController::getInstance()->building_base_k8;
//                    int needDay = ceil(needTime*1.0/86400);
//                    int minDay = baseTime / 86400 ;
//                    needDay = MAX(needDay, minDay);
//                    int day = 0;
//                    if(needDay%minDay!=0){
//                        int tmp = floor(needDay*1.0/minDay);
//                        day = tmp+1;
//                        needDay = day * minDay;
//                    }else{
//                        day = needDay/minDay;
//                    }
//                    needTime = day * baseTime;
//                    int needCost = day * FunBuildController::getInstance()->building_base_k9;
//                    m_rentTime = needTime;
//                    st = false;
//                    std::string showTip = _lang_1("102297", CC_ITOA(needDay));
//                    std::string t2 = "";
//                    auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
//                    if(m_info.level > 0){
//                        t2 = _lang_1("102502",_lang(m_info.name).c_str());
//                    }else{
//                        t2 = _lang_1("102503",_lang(m_info.name).c_str());
//                    }
//                    YesNoDialog::showQueueConfirm(showTip.c_str(),CCCallFunc::create(this, callfunc_selector(UpstarView::onYesRent)),_lang("102501").c_str(),t2.c_str(),lastTime,_tmpTime,_lang("102174").c_str(),needCost);
//                }
//            }
//        }
//
//        if (st) {
//            CCCommonUtils::flyHint("icon_time.png", "", _lang("102120"));
//            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUILD_CELL);
//        }
    }
    else {
        if (m_pos <=0) {
            auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
            if (m_info.type == FUN_BUILD_MAIN && m_info.level==(GlobalData::shared()->shield_base-1)) {
                YesNoDialog::show( _lang_1("108588", CC_ITOA(GlobalData::shared()->shield_base)) , CCCallFunc::create(this, callfunc_selector(UpstarView::onOkUp)));
            }
            else {
                onOkUp();
            }
        }
        else {
            onOkUp();
        }
    }
}

void UpstarView::onOkUp()
{
    if (_resGold>0) {
        string _dialog = "102312";//升级提示
        string _btnDialog = "102104";
        if (m_pos > 0) {
            _dialog = "102313";//建造提示
            _btnDialog = "102126";
        }
        //        YesNoDialog::showButtonAndGold( _lang_1(_dialog, CC_ITOA(_resGold)) , CCCallFunc::create(this, callfunc_selector(UpstarView::onLastUp)), _lang("").c_str(),_resGold);
        PopupViewController::getInstance()->addPopupView(ConfirmDialogView::create(m_lfood,m_lwood,m_liron,m_lstone,0,_resGold
                                                                                   ,this,callfunc_selector(UpstarView::onLastUp)
                                                                                   , _lang(_dialog), _lang(_btnDialog)),false);
    }
    else {
        onLastUp();
    }
}

void UpstarView::onLastUp()
{
    SoundController::sharedSound()->playEffects(Music_Sfx_city_build);
    for (int j = 1; j<willUnlockItems.size(); j++)
    {
        CCUserDefault::sharedUserDefault()->setStringForKey(willUnlockItems[j].c_str(),"1");
    }
    CCUserDefault::sharedUserDefault()->flush();
    
    bool isForce = false;
    if (_resGold>0) {
        isForce = true;
    }
    if (m_pos <=0) {
        auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
        int upstar = 1;
        if (m_info.state == FUN_BUILD_NORMAL && FunBuildController::getInstance()->startUpFunBuild(m_buildId, _resGold, isForce, upstar))
        {
            m_isReturn = false;
//            PopupViewController::getInstance()->removeAllPopupView();
            auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
            if( layer )
                layer->showBuildBtns(m_buildId);
            PopupViewController::getInstance()->removeAllPopupView();
        }
    }
    else {
        if(FunBuildController::getInstance()->startOpenFunBuild(m_buildId, m_pos, _resGold, isForce))
        {
            m_isReturn = false;
            auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
            if( layer )
                layer->hideTmpBuild(m_pos);
            PopupViewController::getInstance()->removeAllPopupView();
        }
    }
}
void UpstarView::helpLoc()
{
     int hightLoc = CCDirector::sharedDirector()->getWinSize().height ;
     int widthLoc = CCDirector::sharedDirector()->getWinSize().width ;
    if(CCCommonUtils::isIosAndroidPad()){
        m_tipBtn->setPosition(ccp(widthLoc - 60,hightLoc - 180));
    }
    else{
        m_tipBtn->setPosition(ccp(widthLoc - 30,hightLoc - 90));
    }
}
void UpstarView::starUpInfo()
{
    auto getAddStr = [](string curPara, string nextPara, bool isPersent, bool isAdd){
        string ret = "";
        if (isAdd) {
            ret = "+";
        }
        float add1 = atoi(nextPara.c_str()) - atoi(curPara.c_str());
        ret = ret+CC_ITOA(add1);
        if (isPersent) {
            ret = ret+"%";
        }
        return ret;
    };
    m_para1Node->setVisible(false);
    m_para2Node->setVisible(false);
    m_para3Node->setVisible(false);
    m_para4Node->setVisible(false);
    int tmpCnt = 0;
    auto& m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
    std::string strstr = CC_ITOA(m_info.starNum);
    m_StarNum->setString(strstr.c_str());//当前星个数
    strstr = CC_ITOA(m_info.starNum+1);
    m_StarNum1->setString(strstr.c_str());//下一个星个数
    m_proText->setString(_lang("160022"));//160022="进度:"
    std::string strtemp(CC_ITOA(m_info.starRate));
    strtemp.append("%");
    m_curpro->setString(strtemp.c_str());
    m_inc1->setString("+10%");
    m_inc1->setColor(ccc3(0, 255, 0));
    if((int)m_info.starRate >= 90){
        m_proText->setString(_lang("102176"));
        m_proText->setFontSize(15);
        m_curpro->setString(_lang("160001"));
        m_curpro->setFontSize(15);
        m_inc1->setString(CC_ITOA(m_info.starNum+1));
        m_inc1->setFontSize(15);
    }
    if (m_info.type==FUN_BUILD_WOOD || m_info.type==FUN_BUILD_STONE || m_info.type==FUN_BUILD_IRON || m_info.type==FUN_BUILD_FOOD){
        if((int)m_info.starRate >= 90)
        {
            m_onetext->setString(_lang("102122"));//每小时产量
            m_oneleft->setString(CC_ITOA(m_info.para1));
            m_twotext->setString(_lang("108543"));//容量
            m_twoleft->setString(CC_ITOA(m_info.para2));
            tmpCnt = 2;
            string nextParas = m_info.nextLevelParas;
            vector<string> nextVec;
            CCCommonUtils::splitString(nextParas, ",", nextVec);
            if (nextVec.size()>=2) {
                std::string str = "+";
                str.append(CC_ITOA(atoi(nextVec[0].c_str()) - m_info.para1));
                m_oneright->setString(str);
                str.clear();
                str = "+";
                str.append(CC_ITOA(atoi(nextVec[1].c_str()) - m_info.para2));
                m_tworight->setString(str);
            }
            m_para1Node->setVisible(true);
            m_para2Node->setVisible(true);
        }else{
            m_desToStar->setString(_lang(m_info.starDescription).c_str());
            if(CCCommonUtils::isIosAndroidPad()){
                m_desToStar->setFontSize(18);
            }else{
                m_desToStar->setFontSize(22);
            }
        }
    }
    else if(m_info.type == FUN_BUILD_QIJI || m_info.type == FUN_BUILD_MAIN){
        m_desToStar->setString(_lang(m_info.starDescription).c_str());
        if(CCCommonUtils::isIosAndroidPad()){
            m_desToStar->setFontSize(18);
        }else{
        m_desToStar->setFontSize(22);
        }
    }
    else if(m_info.type == FUN_BUILD_CELLAR){   //nextParas	std::__1::string	"740000,30800,123300,740000"
        if((int)m_info.starRate >= 90)
        {
            string nextParas = m_info.nextLevelParas;
            vector<string> nextVec;
            CCCommonUtils::splitString(nextParas, ",", nextVec);
            if (nextVec.size()>=4 && m_info.para.size()>=4) {
                m_para1Node->setVisible(true);//wood
                m_onetext->setString(CCCommonUtils::getResourceNameByType(Wood));
                m_oneleft->setString(m_info.para[1]);
                string next1Msg = getAddStr(m_info.para[1], nextVec[0], false, true);
                m_oneright->setString(next1Msg);
                m_para2Node->setVisible(true);//food
                m_twotext->setString(CCCommonUtils::getResourceNameByType(Food));
                m_twoleft->setString(m_info.para[4]);
                string next2Msg = getAddStr(m_info.para[4], nextVec[3], false, true);
                m_tworight->setString(next2Msg);
                tmpCnt = 2;
                int mlv = FunBuildController::getInstance()->getMainCityLv();
                if (mlv>=FunBuildController::getInstance()->building_base_k3) {
                    m_para3Node->setVisible(true);//iron
                    m_threetext->setString(CCCommonUtils::getResourceNameByType(Iron));
                    m_threeleft->setString(m_info.para[3]);
                    string next3Msg = getAddStr(m_info.para[3], nextVec[2], false, true);
                    m_threeright->setString(next3Msg);
                    tmpCnt++;
                }
                if (mlv>=FunBuildController::getInstance()->building_base_k4) {
                    m_para4Node->setVisible(true);
                    m_fourtext->setString(CCCommonUtils::getResourceNameByType(Stone));
                    m_fourleft->setString(m_info.para[2]);
                    string next4Msg = getAddStr(m_info.para[2], nextVec[1], false, true);
                    m_fourright->setString(next4Msg);
                    tmpCnt++;
                }
            }
        }
        else{
            m_desToStar->setString(_lang(m_info.starDescription).c_str());
            if(CCCommonUtils::isIosAndroidPad()){
                m_desToStar->setFontSize(18);
            }else{
                m_desToStar->setFontSize(22);
            }
        }
    }
    else if(m_info.type == FUN_BUILD_BARRACK){
        if((int)m_info.starRate >= 90)
        {
            string nextParas = m_info.nextLevelParas;
            vector<string> nextVec;
            CCCommonUtils::splitString(nextParas, ",", nextVec);
            if (nextVec.size()>=2 && m_info.para.size()>=2) {
                m_para1Node->setVisible(true);
                m_onetext->setString(_lang("102213"));
                m_oneleft->setString(m_info.para[1]);
                string next1Msg = getAddStr(m_info.para[1], nextVec[0], false, true);
                m_oneright->setString(next1Msg);
                m_para2Node->setVisible(true);
                m_twotext->setString(_lang("102264"));
                m_twoleft->setString(m_info.para[2]+"%");
                string next2Msg = getAddStr(m_info.para[2], nextVec[1], true, true);
                m_tworight->setString(next2Msg);
                tmpCnt = 2;
            }
        }
        else{
            m_desToStar->setString(_lang(m_info.starDescription).c_str());
            if(CCCommonUtils::isIosAndroidPad()){
                m_desToStar->setFontSize(18);
            }else{
                m_desToStar->setFontSize(22);
            }
        }
    }
    else if(m_info.type == FUN_BUILD_WALL){
        if((int)m_info.starRate >= 90)
        {
            string nextParas = m_info.nextLevelParas;
            vector<string> nextVec;
            CCCommonUtils::splitString(nextParas, ",", nextVec);
            if (nextVec.size()>=2 && m_info.para.size()>=2) {
                m_para1Node->setVisible(true);
                m_onetext->setString(_lang("102185"));
                m_oneleft->setString(m_info.para[1]);
                string next1Msg = getAddStr(m_info.para[1], nextVec[0], false, true);
                m_oneright->setString(next1Msg);
                
                m_para2Node->setVisible(true);
                m_twotext->setString(_lang("102206"));
                m_twoleft->setString(m_info.para[2]);
                string next2Msg = getAddStr(m_info.para[2], nextVec[1], false, true);
                m_tworight->setString(next2Msg);
                tmpCnt = 2;
            }
        }
        else{
            m_desToStar->setString(_lang(m_info.starDescription).c_str());
            if(CCCommonUtils::isIosAndroidPad()){
                m_desToStar->setFontSize(18);
            }else{
                m_desToStar->setFontSize(22);
            }
        }
    }
    else if(m_info.type == FUN_BUILD_HOSPITAL){
        if((int)m_info.starRate >= 90)
        {
            string nextParas = m_info.nextLevelParas;
            vector<string> nextVec;
            CCCommonUtils::splitString(nextParas, ",", nextVec);
            if (nextVec.size()>=1 && m_info.para.size()>=1) {
                m_para1Node->setVisible(true);
                m_onetext->setString(_lang("102186"));
                m_oneleft->setString(m_info.para[1]);
                string next1Msg = getAddStr(m_info.para[1], nextVec[0], false, true);
                m_oneright->setString(next1Msg);
                tmpCnt = 1;
            }
        }
        else{
            m_desToStar->setString(_lang(m_info.starDescription).c_str());
            if(CCCommonUtils::isIosAndroidPad()){
                m_desToStar->setFontSize(18);
            }else{
                m_desToStar->setFontSize(22);
            }
        }
    }
    else if(m_info.type == FUN_BUILD_TRAINFIELD){
        if((int)m_info.starRate >= 90)
        {
            string nextParas = m_info.nextLevelParas;
            vector<string> nextVec;
            CCCommonUtils::splitString(nextParas, ",", nextVec);
            if (nextVec.size()>=1 && m_info.para.size()>=1) {
                m_para1Node->setVisible(true);
                m_onetext->setString(_lang("102228"));
                m_oneleft->setString(m_info.para[1]);
                string next1Msg = getAddStr(m_info.para[1], nextVec[0], false, true);
                m_oneright->setString(next1Msg);
                tmpCnt = 1;
            }
        }else{
            m_desToStar->setString(_lang(m_info.starDescription).c_str());
            if(CCCommonUtils::isIosAndroidPad()){
                m_desToStar->setFontSize(18);
            }else{
                m_desToStar->setFontSize(22);
            }
        }
    }
    else if(m_info.type == FUN_BUILD_FORGE){
        if((int)m_info.starRate >= 90){
            string nextParas = m_info.nextLevelParas;
            vector<string> nextVec;
            CCCommonUtils::splitString(nextParas, ",", nextVec);
            if (nextVec.size()>=2 && m_info.para.size()>=2) {
                m_para1Node->setVisible(true);
                m_onetext->setString(_lang("102362"));
                m_oneleft->setString(m_info.para[1]);
                string next1Msg = getAddStr(m_info.para[1], nextVec[0], true, true);
                m_oneright->setString(next1Msg);
                
                m_para2Node->setVisible(true);
                m_twotext->setString(_lang("102363"));
                m_twoleft->setString(m_info.para[2]);
                string next2Msg = getAddStr(m_info.para[2], nextVec[1], true, true);
                m_tworight->setString(next2Msg);
                tmpCnt = 2;
            }
        }else{
            m_desToStar->setString(_lang(m_info.starDescription).c_str());
            if(CCCommonUtils::isIosAndroidPad()){
                m_desToStar->setFontSize(18);
            }else{
                m_desToStar->setFontSize(22);
            }
        }
    }
    else if(m_info.type == FUN_BUILD_ARROW_TOWER){  //lv30: "102200,30;102200,2502;102261,100;102200,23690"
        if((int)m_info.starRate >= 90){
            vector<string> vecItems;
            CCCommonUtils::splitString(m_info.starInformation, "|", vecItems);
            string curAtk = "";
            string nextAtk = "";
            string curSpd = "";
            string nextSpd = "";
            if (m_info.starNum < vecItems.size()) {
                string curMsg = "";
                string nextMsg = "";
                if(m_info.starNum == 0){
                    vector<string> vecLvItems;
                    CCCommonUtils::splitString(m_info.information, "|", vecLvItems);
                    curMsg = vecLvItems[30];
                    nextMsg = vecItems[m_info.starNum+1];
                }else{
                    curMsg = vecItems[m_info.starNum];
                    nextMsg = vecItems[m_info.starNum+1];
                }
                vector<string> cellItems;
                vector<string> cellNextItems;
                CCCommonUtils::splitString(curMsg, ";", cellItems);
                CCCommonUtils::splitString(nextMsg, ";", cellNextItems);
                if (cellItems.size()>=3 && cellNextItems.size()>=3) {
                    string atkStr = cellItems[1];
                    string spdStr = cellItems[2];
                    string nextAtkStr = cellNextItems[1];
                    string nextSpdStr = cellNextItems[2];
                    
                    vector<string> tinyItem1;
                    CCCommonUtils::splitString(atkStr, ",", tinyItem1);
                    curAtk = tinyItem1[1];
                    vector<string> nexttinyItem1;
                    CCCommonUtils::splitString(nextAtkStr, ",", nexttinyItem1);
                    nextAtk = nexttinyItem1[1];
                    
                    vector<string> tinyItem2;
                    CCCommonUtils::splitString(spdStr, ",", tinyItem2);
                    curSpd = tinyItem2[1];
                    vector<string> nexttinyItem2;
                    CCCommonUtils::splitString(nextSpdStr, ",", nexttinyItem2);
                    nextSpd = nexttinyItem2[1];
                }
            }
            
            m_para1Node->setVisible(true);
            m_onetext->setString(_lang("102259"));
            m_oneleft->setString(curAtk);
            string next1Msg = getAddStr(curAtk, nextAtk, false, true);
            m_oneright->setString(next1Msg);
            
            m_para2Node->setVisible(true);
            m_twotext->setString(_lang("102260"));
            m_twoleft->setString(curSpd);
            string next2Msg = getAddStr(curSpd, nextSpd, true, true);
            m_tworight->setString(next2Msg);
            tmpCnt = 2;
        }else{
            m_desToStar->setString(_lang(m_info.starDescription).c_str());
            if(CCCommonUtils::isIosAndroidPad()){
                m_desToStar->setFontSize(18);
            }else{
                m_desToStar->setFontSize(22);
            }
        }
    }
    else if(m_info.type == FUN_BUILD_TAVERN){
        if((int)m_info.starRate >= 90){
            string nextParas = m_info.nextLevelParas;
            vector<string> nextVec;
            CCCommonUtils::splitString(nextParas, ",", nextVec);
            if (nextVec.size()>=2 && m_info.para.size()>=2) {
                m_para1Node->setVisible(true);
                m_onetext->setString(_lang("102219"));
                m_oneleft->setString(m_info.para[1]);
                string next1Msg = getAddStr(m_info.para[1], nextVec[0], false, true);
                m_oneright->setString(next1Msg);
                
                m_para2Node->setVisible(true);
                m_twotext->setString(_lang("102220"));
                m_twoleft->setString(m_info.para[2]);
                string next2Msg = getAddStr(m_info.para[2], nextVec[1], false, false);
                m_tworight->setString(next2Msg);
                tmpCnt = 2;
            }
        }else{
            m_desToStar->setString(_lang(m_info.starDescription).c_str());
            if(CCCommonUtils::isIosAndroidPad()){
                m_desToStar->setFontSize(18);
            }else{
                m_desToStar->setFontSize(22);
            }
        }
    }
    else if(m_info.type == FUN_BUILD_SMITHY){
        if((int)m_info.starRate >= 90){
            string nextParas = m_info.nextLevelParas;
            vector<string> nextVec;
            CCCommonUtils::splitString(nextParas, ",", nextVec);
            if (nextVec.size()>=1 && m_info.para.size()>=1) {
                m_para1Node->setVisible(true);
                m_onetext->setString(_lang("102311"));
                m_oneleft->setString(m_info.para[1]);
                string next1Msg = getAddStr(m_info.para[1], nextVec[0], false, true);
                m_oneright->setString(next1Msg);
                tmpCnt = 1;
            }
        }else{
            m_desToStar->setString(_lang(m_info.starDescription).c_str());
            if(CCCommonUtils::isIosAndroidPad()){
                m_desToStar->setFontSize(18);
            }else{
                m_desToStar->setFontSize(22);
            }
        }
    }
    else if(m_info.type == FUN_BUILD_BARRACK1 ||m_info.type == FUN_BUILD_BARRACK2||m_info.type == FUN_BUILD_BARRACK3||m_info.type == FUN_BUILD_BARRACK4){
        if((int)m_info.starRate >= 90){
            std::string barrackdes = getBarrackDes(m_info.type,m_info.starNum);
            m_desToStar->setString(barrackdes.c_str());
            if(CCCommonUtils::isIosAndroidPad()){
                m_desToStar->setFontSize(18);
            }else{
                m_desToStar->setFontSize(22);
            }
        }else{
            m_desToStar->setString(_lang(m_info.starDescription).c_str());
            if(CCCommonUtils::isIosAndroidPad()){
                m_desToStar->setFontSize(18);
            }else{
                m_desToStar->setFontSize(22);
            }
        }
    }
    else if(m_info.type == FUN_BUILD_STABLE){
        if((int)m_info.starRate >= 90){
            string nextParas = m_info.nextLevelParas;
            vector<string> nextVec;
            CCCommonUtils::splitString(nextParas, ",", nextVec);
            if (nextVec.size()>=3 && m_info.para.size()>=3) {
                m_para1Node->setVisible(true);
                m_onetext->setString(_lang("102218"));
                m_oneleft->setString(m_info.para[1]);
                string next1Msg = getAddStr(m_info.para[1], nextVec[0], false, true);
                m_oneright->setString(next1Msg);
                
                m_para2Node->setVisible(true);
                m_twotext->setString(_lang("102221"));
                m_twoleft->setString(m_info.para[2]);
                string next2Msg = getAddStr(m_info.para[2], nextVec[1], false, true);
                m_tworight->setString(next2Msg);
                
                m_para3Node->setVisible(true);
                m_threetext->setString(_lang("102258"));
                m_threeleft->setString(m_info.para[3]);
                string next3Msg = getAddStr(m_info.para[3], nextVec[2], false, true);
                m_threeright->setString(next3Msg);
                tmpCnt = 3;
            }
        }else{
            m_desToStar->setString(_lang(m_info.starDescription).c_str());
            if(CCCommonUtils::isIosAndroidPad()){
                m_desToStar->setFontSize(18);
            }else{
                m_desToStar->setFontSize(22);
            }
        }
    }
    else if(m_info.type == FUN_BUILD_SACRIFICE){
        m_desToStar->setString(_lang(m_info.starTip).c_str());
        if(CCCommonUtils::isIosAndroidPad()){
            m_desToStar->setFontSize(18);
        }else{
            m_desToStar->setFontSize(22);
        }
    }
    if (tmpCnt==1) {
        m_para1Node->setPositionY(-10);
    }
    else if (tmpCnt==2) {
        m_para1Node->setPositionY(20);
        m_para2Node->setPositionY(-40);
    }
    else if (tmpCnt==3) {
        m_para1Node->setPositionY(25);
        m_para2Node->setPositionY(-20);
        m_para3Node->setPositionY(-65);
    }
}

string UpstarView::getBarrackDes(int type,int starnum){
    string result = "";
    if(starnum == 10){
        return result = " ";
        
    }
    switch (type) {
        case FUN_BUILD_BARRACK1:
            result = _lang_2("160105",CC_ITOA(starnum+1),CC_ITOA(starnum+1) );
            break;
        case FUN_BUILD_BARRACK2:
            result = _lang_2("160106",CC_ITOA(starnum+1),CC_ITOA(starnum+1) );
            break;
        case FUN_BUILD_BARRACK3:
            result = _lang_2("160107",CC_ITOA(starnum+1),CC_ITOA(starnum+1) );
            break;
        case FUN_BUILD_BARRACK4:
            result = _lang_2("160108",CC_ITOA(starnum+1),CC_ITOA(starnum+1) );
            break;
            
        default:
            break;
    }
    return result;
}
void UpstarView::onShowNextUnlockItem()
{
    //    return;
    int itemId = m_buildId;
    if (m_pos<=0) {
        itemId = m_buildId / 1000 ;
    }
    if (itemId<=0) {
        return;
    }
    string keys = "unlock_item";
    
    string msgInfo = CCCommonUtils::getPropById(CC_ITOA(itemId), keys);
    //    if (itemId == FUN_BUILD_SCIENE) {
    //        msgInfo = "1,700100,700200,730000|2,710000,711400,711500,730100,730200,730300,730400|3,720000|4,720100,720200,730500|5,700300,730600,730700,730800,730900|6,720300,720400,731000|7,700400,700500,710300|8,731100,731200,731300,731400|9,731500|10,700600,710500,720500,720600,720700|11,720800,731600|12,700700,710600|13,731700,731800,731900,732000|14,710400,732100|15,700800,711600,711700,720900|16,700900,701000,721000,732200,732300,732400,732500|17,732600|18,701200,701300,710700,732700,732800,732900,733000|19,721100,721200,733100|20,711000,733200|21,701400|22,701500,701600,711200,711300,721400,733300,733400,733500,733600|24,701700,733700|25,701800,721500,721600,721700,733800,733900,734000,734100|26,701900,711100|27,702000,702100,721800,734200|28,734300,734400,734500,734600|30,734700";
    //    }
    //    else if (itemId == FUN_BUILD_BARRACK1) {
    //        msgInfo = "4,107001|7,107002|10,107003|13,107004|16,107005|19,107006|22,107007|26,107008|30,107009";
    //    }
    //    else if (itemId == FUN_BUILD_BARRACK2) {
    //        msgInfo = "4,107101|7,107102|10,107103|13,107104|16,107105|19,107106|22,107107|26,107108|30,107109";
    //    }
    //    else if (itemId == FUN_BUILD_BARRACK3) {
    //        msgInfo = "4,107201|7,107202|10,107203|13,107204|16,107205|19,107206|22,107207|26,107208|30,107209";
    //    }
    //    else if (itemId == FUN_BUILD_BARRACK4) {
    //        msgInfo = "4,107301|7,107302|10,107303|13,107304|16,107305|19,107306|22,107307|26,107308|30,107309";
    //    }
    //    else if (itemId == FUN_BUILD_FORT) {
    //        msgInfo = "2,107910|3,107920|6,107901|8,107911|10,107921|12,107902|14,107912|16,107922|18,107903|20,107913|22,107923|24,107904|27,107914|30,107924";
    //    }
    //    else if (itemId == FUN_BUILD_MAIN) {
    //        msgInfo = "2,410000|3,425000|4,416000,403000,411000|5,426000,429000|6,401000,402000|7,407000|8,428000|10,414000|15,412000";
    //    }
    
    if (msgInfo=="") {
        return;
    }
    
    int nextLv = 1;
    if (m_pos<=0) {
        auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
        nextLv = m_info.level+1;
    }
    vector<string> msgItemVec;
    vector<string> finalVec;
    vector<string> nextVec;
    CCCommonUtils::splitString(msgInfo, "|", msgItemVec);
    for (int i=0; i<msgItemVec.size(); i++)
    {
        finalVec.clear();
        CCCommonUtils::splitString(msgItemVec[i], ",", finalVec);
        if (finalVec.size()>0 && finalVec[0] == CC_ITOA(nextLv)) {
            break;
        }
        
        if (atoi(finalVec[0].c_str()) > nextLv)
        {
            if (nextVec.size() == 0) {
                nextVec = finalVec;
            }
            else {
                if (atoi(nextVec[0].c_str()) > atoi(finalVec[0].c_str())) {
                    nextVec = finalVec;
                }
            }
        }
        
        if (i == msgItemVec.size()-1) {
            finalVec.clear();
        }
    }
    
    
    if (finalVec.size()<=0) {
        return;
    }
    
    if (itemId == FUN_BUILD_SCIENE) {
        CCLoadSprite::doResourceByCommonIndex(5, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(5, false);
        });
    }
    else if (itemId == FUN_BUILD_BARRACK1 || itemId == FUN_BUILD_BARRACK2 || itemId == FUN_BUILD_BARRACK3 || itemId == FUN_BUILD_BARRACK4) {
        CCLoadSprite::doResourceByCommonIndex(204, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(204, false);
        });
    }
    else {
        CCLoadSprite::doResourceByCommonIndex(101, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(101, false);
        });
    }
    
//    m_itemScrollView->getContainer()->removeAllChildren();
    
    int cellWidth = 180;
    if (CCCommonUtils::isIosAndroidPad())
    {
        cellWidth = 180 * 2.32;
    }
    int startX = 0;
    if (finalVec.size() == 2) {
        startX = (180-cellWidth)/2;
        if (CCCommonUtils::isIosAndroidPad())
        {
            startX = 0;
        }
    }
    int posY = 10;

    if (finalVec.size()==2) {
//        m_itemScrollView->setTouchEnabled(false);
    }else if (finalVec.size()>2) {
        m_arrowNode->setVisible(true);
        m_rArrow->setVisible(true);
        m_lArrow->setVisible(false);
    }
    m_unlockItemWidth = cellWidth;
    m_sumItemWidth = (finalVec.size()-1)*cellWidth;
}

void UpstarView::updateParasInfo()
{
    auto getAddStr = [](string curPara, string nextPara, bool isPersent, bool isAdd){
        string ret = "";
        if (isAdd) {
            ret = "+";
        }
        float add1 = atoi(nextPara.c_str()) - atoi(curPara.c_str());
        ret = ret+CC_ITOA(add1);
        if (isPersent) {
            ret = ret+"%";
        }
        return ret;
    };
    
    int tmpCnt = 0;
    auto& m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
    if (m_info.type == FUN_BUILD_WOOD || m_info.type == FUN_BUILD_FOOD || m_info.type == FUN_BUILD_IRON || m_info.type == FUN_BUILD_STONE) {
    }
    else if (m_info.type == FUN_BUILD_MAIN) {
    }
    else if (m_info.type == FUN_BUILD_BARRACK) {
    }
    else if (m_info.type == FUN_BUILD_HOSPITAL) {
    }
    else if (m_info.type == FUN_BUILD_WALL) {
    }
    else if (m_info.type == FUN_BUILD_TRAINFIELD) {
    }
    else if (m_info.type == FUN_BUILD_SMITHY) {
    }
    else if (m_info.type == FUN_BUILD_STABLE) {
    }
    else if (m_info.type == FUN_BUILD_TAVERN) {
    }
    else if (m_info.type == FUN_BUILD_FORGE) {
    }
    else if (m_info.type == FUN_BUILD_CELLAR) {
    }
    else if (m_info.type == FUN_BUILD_ARROW_TOWER) {
    }
}

/////////////////////////

UpstarCell* UpstarCell::create(int type, int itemId, int value, bool isShow)
{
    auto ret = new UpstarCell();
    if (ret && ret->init(type, itemId, value, isShow)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool UpstarCell::init(int type, int itemId, int value, bool isShow)
{
    DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_GOODS_TEXTURE,false);
    setCleanFunction([](){
        DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_GOODS_TEXTURE,true);
    });
    CCBLoadFile("ResUpCell02",this,this);
    m_type = type;
    m_itemId = itemId;
    m_value = value;
    tmpNum = 0;
    m_isShow = isShow;
    refresh();
    bool ret = true;
    return ret;
}

void UpstarCell::refresh()
{
    string tmpCellName = "";
    int tmpCellValue = 0;
    int tmpSumValue = 0;
    m_isOk = false;
    m_value2Label->setString("");
    
    string picName = "";
    if (m_type == 0) {//资源
        picName = CCCommonUtils::getResourceIconByType(m_itemId);
        tmpCellName = CCCommonUtils::getResourceNameByType(m_itemId);
        tmpCellValue = m_value;
        m_isOk = CCCommonUtils::isEnoughResourceByType(m_itemId, m_value);
        tmpSumValue = CCCommonUtils::getCurResourceByType(m_itemId);
    }
    else if (m_type == 1) {//道具
        auto& toolInfo = ToolController::getInstance()->getToolInfoById(m_itemId);
        tmpCellName = toolInfo.getName();//_lang(toolInfo.name);
        tmpCellValue = m_value;
        picName = toolInfo.icon + ".png";
        tmpSumValue = toolInfo.getCNT();
//        tmpSumValue = 1;
        if (tmpSumValue >= tmpCellValue) {
            m_isOk = true;
        }
    }
    else if (m_type == 2) {//建筑
        int buildId = m_itemId;
        int lv = buildId%1000;
        buildId = buildId-lv;
        auto buildDict = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(buildId));
        string name = buildDict->valueForKey("name")->getCString();
        tmpCellName = _lang(name);
        tmpCellValue = lv;
        if (FunBuildController::getInstance()->isExistBuildByTypeLv(buildId, lv)) {
            m_isOk = true;
        }
        picName = buildDict->valueForKey("pic")->getCString();
        if(GlobalData::shared()->contryResType==0){
            picName = picName +".png";
        }
        else{
            picName = picName + "_" + CC_ITOA(GlobalData::shared()->contryResType)+".png";
        }
        if (m_isShow) {
            picName = "build_icon.png";
        }
        if(!m_isOk) {
            m_gotoBuildId = FunBuildController::getInstance()->getMaxLvBuildByType(buildId);
        }
        if(m_itemId == 422030){
            m_gotoBuildId = SPE_BUILD_QIJI;
        }
    }
    else if (m_type == 3) {//没有空闲队列
        picName = "icon_time.png";
        string key = GlobalData::shared()->allQueuesInfo[m_value].key;
        int qType = GlobalData::shared()->allQueuesInfo[m_value].type;
        tmpTime = GlobalData::shared()->allQueuesInfo[m_value].finishTime - GlobalData::shared()->getWorldTime();
        
        if (qType == TYPE_BUILDING) {
            auto& bInfo = FunBuildController::getInstance()->getFunbuildById(atoi(key.c_str()));
            if (bInfo.state == FUN_BUILD_UPING) {
                tmpCellName = _lang_1("102273", _lang(bInfo.name).c_str());
            }
            else if (bInfo.state == FUN_BUILD_CREATE) {
                tmpCellName = _lang_1("102274", _lang(bInfo.name).c_str());
            }
        }
        else if (qType == TYPE_SCIENCE) {
            int scId = QueueController::getInstance()->getItemId(key);
            string scName = CCCommonUtils::getNameById(CC_ITOA(scId));
            tmpCellName = _lang_1("102273", scName.c_str());
        }
        
        tmpCellValue = m_value;
        m_isOk = false;
    }
    else if (m_type == 4) {
        if (m_itemId == R_POWER) {
            picName = "icon_combat.png";
        }
        else if (m_itemId == R_EXP) {
            picName = "icon_exp.png";
        }
        tmpCellName = "";
        tmpCellValue = m_value;
        m_isOk = false;
    }
    else if (m_type == 5) {
        auto &scienceMap = GlobalData::shared()->scienceInfo.infoMap;
        picName = "book.png";
        tmpCellValue = m_value;
        
        string name = CCCommonUtils::getNameById(CC_ITOA(m_itemId));
        tmpCellName = _lang_2("121989", name.c_str(), CC_ITOA(m_value));
        m_isOk = true;
        if (scienceMap.find(m_itemId) == scienceMap.end() || scienceMap[m_itemId].scienceLv < m_value ) {
            m_isOk = false;
        }
    }
//    else if (m_type == 6) {//龙建筑
//        int buildId = m_itemId;
//        int lv = buildId%1000;
//        buildId = buildId-lv;
//        auto buildDict = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(buildId));
//        string name = buildDict->valueForKey("name")->getCString();
//        tmpCellName = _lang(name);
//        tmpCellValue = lv;
//        if (DragonBuildingController::getInstance()->isExistBuildByTypeLv(buildId, lv) || FunBuildController::getInstance()->isExistBuildByTypeLv(buildId, lv)) {
//            m_isOk = true;
//        }
//        picName = buildDict->valueForKey("pic")->getCString();
//        //        if(GlobalData::shared()->contryResType==0){
//        //            picName = picName +".png";
//        //        }
//        //        else{
//        //
//        //        }
//        picName = picName + "_" + CC_ITOA(GlobalData::shared()->contryResType)+".png";
//        if (m_isShow) {
//            picName = "build_icon.png";
//        }
//        if(!m_isOk) {
//            m_gotoBuildId = DragonBuildingController::getInstance()->getMaxLvBuildByType(buildId);
//        }
//    }
//    else if (m_type == 7) {//没有空闲龙相关队列
//        picName = "icon_time.png";
//        string key = GlobalData::shared()->allQueuesInfo[m_value].key;
//        int qType = GlobalData::shared()->allQueuesInfo[m_value].type;
//        tmpTime = GlobalData::shared()->allQueuesInfo[m_value].finishTime - GlobalData::shared()->getWorldTime();
//        
//        if(qType == TYPE_DRAGON_BUILD) {
//            auto& bInfo = DragonBuildingController::getInstance()->getFunbuildById(atoi(key.c_str()));
//            if (bInfo.state == FUN_BUILD_UPING) {
//                tmpCellName = _lang_1("102273", _lang(bInfo.name).c_str());
//            }
//            else if (bInfo.state == FUN_BUILD_CREATE) {
//                tmpCellName = _lang_1("102274", _lang(bInfo.name).c_str());
//            }
//        }
//        tmpCellValue = m_value;
//        m_isOk = false;
//    }
    else if(m_type == 8){//城堡荣耀
        int buildId = m_itemId;
        int starnum = buildId%1000;
        buildId = buildId-starnum;
        auto buildDict = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(buildId));
        string name = buildDict->valueForKey("name")->getCString();
        tmpCellName = _lang(name);
        tmpCellValue = starnum;
        if(FunBuildController::getInstance()->isExistBuildByTypeStarnum(buildId, starnum)){
            m_isOk = true;
        }
        picName = buildDict->valueForKey("pic")->getCString();
        if(GlobalData::shared()->contryResType==0){
            picName = picName +".png";
        }
        else{
            picName = picName + "_" + CC_ITOA(GlobalData::shared()->contryResType)+".png";
        }
        if (m_isShow) {
            picName = "build_icon.png";
        }
        if(!m_isOk) {
            m_gotoBuildId = FunBuildController::getInstance()->getMaxStarnumBuildByType(buildId);
        }
    }
    
    m_picNode->removeAllChildren();
    auto pic = CCLoadSprite::createSprite(picName.c_str());
    if(pic)
    {
        if (m_type == 2 || m_type == 6) {
            if(m_isShow) {
                CCCommonUtils::setSpriteMaxSize(pic, 60, true);
            }
            else {
                CCCommonUtils::setSpriteMaxSize(pic, 40, true);
            }
        }
        else if (m_type == 5) {
            CCCommonUtils::setSpriteMaxSize(pic, 60, true);
        } else {
            CCCommonUtils::setSpriteMaxSize(pic, 40, true);
        }
        m_picNode->addChild(pic);
    }
    
    m_nameLabel->setColor(ccWHITE);
    m_valueLabel->setColor(ccWHITE);
    
    m_nameLabel->setString(tmpCellName.c_str());
    if (m_type == 2) {
        string tmpBuildName = tmpCellName + " " + _lang_1("102272", CC_CMDITOA(tmpCellValue).c_str());
        m_valueLabel->setString("");
        m_nameLabel->setString(tmpBuildName.c_str());
        m_nameLabel->setPositionX(m_nameLabel->getPositionX()+0);
    }
    else if (m_type == 6) {
        string tmpBuildName = tmpCellName + " " + _lang_1("102272", CC_CMDITOA(tmpCellValue).c_str());
        m_valueLabel->setString("");
        m_nameLabel->setString(tmpBuildName.c_str());
        m_nameLabel->setPositionX(m_nameLabel->getPositionX()+0);
    }
    else if (m_type == 3) {
        m_tmpName = tmpCellName;
        m_nameLabel->setFontSize(16);
        
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_nameLabel->setFontSize(40);
            m_nameLabel->setDimensions(CCSizeMake(510, 0));
        }
        m_nameLabel->setString(m_tmpName+" "+CC_SECTOA(tmpTime));
        m_valueLabel->setString("");
    }
    else if (m_type == 7) {
        m_tmpName = tmpCellName;
        m_nameLabel->setFontSize(16);
        m_nameLabel->setDimensions(CCSizeMake(210, 0));
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_nameLabel->setFontSize(40);
            m_nameLabel->setDimensions(CCSizeMake(510, 0));
        }
        m_nameLabel->setString(m_tmpName+" "+CC_SECTOA(tmpTime));
        m_valueLabel->setString("");
    }
    else if (m_type == 0) {
        if(m_isShow) {
            m_nameLabel->setString(CC_CMDITOA(tmpSumValue).c_str());
            m_valueLabel->setString(CCString::createWithFormat("/%s",CC_CMDITOA(tmpCellValue).c_str())->getCString());
            if (!m_isOk) {
                m_nameLabel->setColor(ccRED);
            }
            m_valueLabel->setPositionX(m_nameLabel->getPositionX()+m_nameLabel->getContentSize().width*m_nameLabel->getOriginScaleX());
        }
        else {
            m_nameLabel->setString("");
            m_valueLabel->setString(CCString::createWithFormat("%s",CC_CMDITOA(tmpCellValue).c_str())->getCString());
            if (!m_isOk) {
                m_valueLabel->setColor(ccRED);
            }
            m_valueLabel->setPositionX(m_nameLabel->getPositionX());
        }
    }
    else if (m_type == 4) {
        m_valueLabel->setString("");
        m_nameLabel->setString(CCString::createWithFormat("+%s",CC_CMDITOA(tmpCellValue).c_str())->getCString());
    }
    else if (m_type == 5) {
        m_nameLabel->setFontSize(16);
        m_nameLabel->setDimensions(CCSizeMake(210, 0));
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_nameLabel->setFontSize(40);
            m_nameLabel->setDimensions(CCSizeMake(510, 0));
        }
        m_valueLabel->setString("");
    }
    else if(m_type == 8){
        string tmpBuildName = tmpCellName + " " +_lang("160001") +" " + CC_CMDITOA(tmpCellValue);
        m_valueLabel->setString("");
        m_nameLabel->setString(tmpBuildName.c_str());
        m_nameLabel->setPositionX(m_nameLabel->getPositionX()+0);
    }
    else {
        m_valueLabel->setString(CC_CMDITOA(tmpCellValue).c_str());
        if (m_type==1) {
            m_valueLabel->setString(CC_CMDITOA(tmpSumValue).c_str());
            m_value2Label->setString(CCString::createWithFormat("/%s",CC_CMDITOA(tmpCellValue).c_str())->getCString());
            m_value2Label->setPositionX(m_valueLabel->getPositionX()+m_valueLabel->getContentSize().width*m_valueLabel->getOriginScaleX());
        }
    }
    
    m_btn->setVisible(false);
    m_btnLabel->setString("");
    if (m_isOk) {
        m_yesSprite->setVisible(true);
        m_noSprite->setVisible(false);
    }
    else {
        m_yesSprite->setVisible(false);
        m_noSprite->setVisible(true);
        if (m_type == 0) {
            //            m_noSprite->setVisible(false);
            m_btn->setVisible(true);
            m_btnLabel->setString(_lang("102153"));
        }
        else if (m_type == 1) {
            //            m_noSprite->setVisible(false);
            m_btn->setVisible(true);
            m_btnLabel->setString(_lang("102153"));
        }
        else if (m_type == 2 && m_gotoBuildId > 0) {
            //            m_noSprite->setVisible(false);
            m_btn->setVisible(true);
            m_btnLabel->setString(_lang("103658"));
            if (m_isShow) {
                m_btn->setVisible(false);
                m_btnLabel->setString("");
            }
        }
        else if (m_type == 3) {
            //            m_noSprite->setVisible(false);
            m_btn->setVisible(true);
            m_btnLabel->setString(_lang("104903"));
        }
        else if (m_type == 7) {
            //            m_noSprite->setVisible(false);
            m_btn->setVisible(true);
            m_btnLabel->setString(_lang("104903"));
        }
        else if (m_type == 6 && m_gotoBuildId > 0) {
            m_btn->setVisible(true);
            m_btnLabel->setString(_lang("103658"));
            if (m_isShow) {
                m_btn->setVisible(false);
                m_btnLabel->setString("");
            }
        }
        else if (m_type == 8 && m_gotoBuildId > 0) {
            m_btn->setVisible(true);
            m_btnLabel->setString(_lang("103658"));
            if (m_isShow) {
                m_btn->setVisible(false);
                m_btnLabel->setString("");
            }
        }
        if (CCCommonUtils::isIosAndroidPad())
        {
            float ls = m_btnLabel->getScaleX();
            float fw = getContentSize().width;
            if (fw>220) {//防爆框
                setScaleX(220/fw * ls);
            }
        } else {
            m_btnLabel->setMaxScaleXByWidth(100);
        }
        
        if(m_type == 1 || m_type == 2 || m_type == 3 || m_type == 5 || m_type == 6 || m_type == 7) {
            m_nameLabel->setColor(ccRED);
            m_valueLabel->setColor(ccRED);
        }
    }
    
    if (m_type == 4) {
        m_yesSprite->setVisible(false);
        m_noSprite->setVisible(false);
    }
}

void UpstarCell::setTouchNode(CCNode* node, int buildId, int parentType)
{
    m_touchNode = node;
    m_buildId = buildId;
    m_parentType = parentType;
}

void UpstarCell::setCreateBuildInfo(int buildId, int pos)
{
    m_createInfo = CC_ITOA(buildId);
    m_createInfo = m_createInfo+"|"+CC_ITOA(pos);
}

void UpstarCell::onEnter() {
    CCNode::onEnter();
    setSwallowsTouches(false);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(UpstarCell::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UpstarCell::rectCallBack), MSG_BUILD_CELL, NULL);
    if (tmpNum>0) {
        refresh();
    }
    tmpNum++;
}

void UpstarCell::onExit() {
    setTouchEnabled(false);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(UpstarCell::onEnterFrame), this);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BUILD_CELL);
    CCNode::onExit();
}

void UpstarCell::onEnterFrame(float dt)
{
    if (m_type == 0) {
        bool t_isOk = CCCommonUtils::isEnoughResourceByType(m_itemId, m_value);
        int tmpSumValue = CCCommonUtils::getCurResourceByType(m_itemId);
        if (m_isShow) {
            m_nameLabel->setString(CC_CMDITOA(tmpSumValue).c_str());
            if (!t_isOk) {
                m_nameLabel->setColor(ccRED);
            }
            else {
                m_nameLabel->setColor(ccWHITE);
            }
            m_valueLabel->setPositionX(m_nameLabel->getPositionX()+m_nameLabel->getContentSize().width*m_nameLabel->getOriginScaleX());
        }
        else {
            m_nameLabel->setString("");
            if (!t_isOk) {
                m_valueLabel->setColor(ccRED);
            }
            else {
                m_valueLabel->setColor(ccWHITE);
            }
            m_valueLabel->setPositionX(m_nameLabel->getPositionX());
        }
        if (m_isOk ^ t_isOk) {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REFREASH_BUILD_UPGRADE);
        }
        m_isOk = t_isOk;
    }
    
    if(m_type == 3) {
        tmpTime--;
        m_nameLabel->setString(m_tmpName+" "+CC_SECTOA(tmpTime));
        if (tmpTime<=0) {
            this->removeFromParent();
        }
    }
    if(m_type == 7) {
        tmpTime--;
        m_nameLabel->setString(m_tmpName+" "+CC_SECTOA(tmpTime));
        if (tmpTime<=0) {
            this->removeFromParent();
        }
    }
}

bool UpstarCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_btn->isVisible() && (m_type==0||m_type==1||m_type==2||m_type==3 || m_type == 6 || m_type == 7 || m_type == 8) && m_touchNode && isTouchInside(m_touchNode,pTouch) && isTouchInside(m_touchBtn,pTouch)) {
        m_btn->setScale(1.1);
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        return true;
    }
    return false;
}

void UpstarCell::onTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_btn->setScale(1.0);
}

void UpstarCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_btn->setScale(1.0);
    if (1) {//m_btn->isVisible() && (m_type==0||m_type==1||m_type==2||m_type==3) && m_touchNode && isTouchInside(m_touchNode,pTouch) && isTouchInside(m_touchBtn,pTouch)
        //        m_touchNode->setVisible(false);
        if (m_type == 1) {
            PopupViewController::getInstance()->addPopupInView(BatchBuyItemView::create(m_itemId, m_value));
        }
        else if (m_type == 2 && m_gotoBuildId>0) {
             auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
            if(m_gotoBuildId == SPE_BUILD_QIJI){
                if (layer) {
                    layer->onMoveToSpeBuildAndPlay(m_gotoBuildId, true);
                    PopupViewController::getInstance()->removeAllPopupView();
                }
                return;
            }
            if (layer) {
                layer->onMoveToBuildAndPlay(m_gotoBuildId, true);
                PopupViewController::getInstance()->removeAllPopupView();
            }
        }
        else if (m_type == 6 && m_gotoBuildId>0) {
//            auto layer = dynamic_cast<DragonScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
//            if (layer) {
//                layer->onMoveToBuildAndPlay(m_gotoBuildId, true);
//                PopupViewController::getInstance()->removeAllPopupView();
//            }
        }
        else if (m_type == 3) {
            if (m_parentType==0) {
                int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_BUILDING);
                int tmpTime = GlobalData::shared()->allQueuesInfo[qid].finishTime - GlobalData::shared()->getWorldTime();
                YesNoDialog::showTime( _lang("102120").c_str() , CCCallFunc::create(this, callfunc_selector(UpstarCell::spdCallBack)), tmpTime, _lang("104903").c_str());
            }
            else if (m_parentType==1) {
                int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_SCIENCE);
                int tmpTime = GlobalData::shared()->allQueuesInfo[qid].finishTime - GlobalData::shared()->getWorldTime();
                YesNoDialog::showTime( _lang("121986").c_str() , CCCallFunc::create(this, callfunc_selector(UpstarCell::spdCallBack)), tmpTime, _lang("104903").c_str(), false);
            }
        }
        else if (m_type == 7) {
            int qType = GlobalData::shared()->allQueuesInfo[m_value].type;
            if (qType == TYPE_DRAGON_BUILD) {
                int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_DRAGON_BUILD);
                int tmpTime = GlobalData::shared()->allQueuesInfo[qid].finishTime - GlobalData::shared()->getWorldTime();
                YesNoDialog::showTime( _lang("102120").c_str() , CCCallFunc::create(this, callfunc_selector(UpstarCell::spdCallBackDragon)), tmpTime, _lang("104903").c_str());
            }
        }
        else if (m_type == 8 && m_gotoBuildId>0) {
            auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
            if (layer) {
                layer->onMoveToBuildAndPlay(m_gotoBuildId, true);
                PopupViewController::getInstance()->removeAllPopupView();
            }
        }
        else {
            if(m_buildId!=0){
                if (m_parentType == 0) {
                    PopupViewController::getInstance()->addPopupInView(UseResToolView::create(m_itemId, CC_ITOA(m_buildId), RES_BUILD_UP_VIEW));
                }
                else if (m_parentType == 1) {
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCIENCE_CLOSE);
                    PopupViewController::getInstance()->addPopupInView(UseResToolView::create(m_itemId, CC_ITOA(m_buildId), RES_SCIENCE_VIEW));
                }
            }
            else {
                PopupViewController::getInstance()->addPopupInView(UseResToolView::create(m_itemId, m_createInfo, RES_BUILD_CREATE_VIEW));
            }
        }
    }
}

SEL_CCControlHandler UpstarCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCreateOrUpClick", BuildCell::onCreateOrUpClick);
    return NULL;
}

bool UpstarCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_valueLabel", CCLabelIF*, m_valueLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_value2Label", CCLabelIF*, m_value2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_yesSprite", CCSprite*, m_yesSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noSprite", CCSprite*, m_noSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn", CCScale9Sprite*, m_btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnLabel", CCLabelIF*, m_btnLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchBtn", CCNode*, m_touchBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rectPic", CCScale9Sprite*, m_rectPic);
    return false;
}

void UpstarCell::spdCallBackDragon()
{
//    int qType = GlobalData::shared()->allQueuesInfo[m_value].type;
//    if (qType == TYPE_DRAGON_BUILD) {
//        int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_DRAGON_BUILD);
//        string key = GlobalData::shared()->allQueuesInfo[qid].key;
//        int tmpTime = GlobalData::shared()->allQueuesInfo[qid].finishTime - GlobalData::shared()->getWorldTime();
//        int gold = CCCommonUtils::getGoldByTime(tmpTime);
//        if (tmpTime<=GlobalData::shared()->freeSpdT) {
//            gold = 0;
//        }
//        if (DragonBuildingController::getInstance()->costCD(atoi(key.c_str()), "", gold))
//        {
//            this->removeFromParent();
//        }
//    }
}

void UpstarCell::spdCallBack()
{
    if (m_parentType == 0) {
        int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_BUILDING);
        string key = GlobalData::shared()->allQueuesInfo[qid].key;
        int tmpTime = GlobalData::shared()->allQueuesInfo[qid].finishTime - GlobalData::shared()->getWorldTime();
        int gold = CCCommonUtils::getGoldByTime(tmpTime);
        if (tmpTime<=GlobalData::shared()->freeSpdT) {
            gold = 0;
        }
        if(FunBuildController::getInstance()->costCD(atoi(key.c_str()), "", gold))
        {
            this->removeFromParent();
        }
    }
    else if (m_parentType == 1) {
        int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_SCIENCE);
        string key = GlobalData::shared()->allQueuesInfo[qid].key;
        int tmpTime = GlobalData::shared()->allQueuesInfo[qid].finishTime - GlobalData::shared()->getWorldTime();
        int gold = CCCommonUtils::getGoldByTime(tmpTime);
        if( gold <= GlobalData::shared()->playerInfo.gold ){
            QueueController::getInstance()->startCCDQueue(qid, "", false, gold,"",1);
            this->removeFromParent();
        }
        else {
            YesNoDialog::gotoPayTips();
        }
    }
}

void UpstarCell::rectCallBack(CCObject* obj)
{
    if(m_type==3 || m_type == 7) {
        m_rectPic->setVisible(true);
        CCActionInterval* fadeout = CCFadeOut::create(0.4);
        CCActionInterval* fadein = CCFadeIn::create(0.4);
        CCSequence* seq = CCSequence::create(fadein, fadeout, NULL);
        m_rectPic->runAction(CCRepeat::create(seq, 2));
    }
}
