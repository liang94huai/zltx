//
//  ProductionSoldiersView.cpp
//  IF
//
//  Created by ganxiaohua on 14-5-16.
//
//

#include "ProductionSoldiersView.h"
#include "FunBuildController.h"
#include "ArmyController.h"
#include "YesNoDialog.h"
#include "PopupViewController.h"
#include "TrainSoldierCommand.h"
#include "FortCommand.h"
#include "UseCDToolView.h"
#include "TrainSoldierWithCommand.h"
#include "CCGraySprite.h"
#include "MakingSoldierView.h"
#include "SoldierInfoView.h"
#include "ConfirmDialogView.h"
#include "ParticleController.h"
#include "TipsView.h"
#include "UIComponent.h"
#include "SoundController.h"
#include "QueueController.h"
#include "LogoutCommand.h"
#include "GuideController.h"
#include "BuildMoreInfoView.h"

#define RIGHT_BD_SIZE 3

static CCClipNode* SetNodeClip(CCNode* pNode, Point ptAnchor)
{
    auto pos = pNode->getPosition();
    CCClipNode* pClipNode = CCClipNode::create(pNode->getContentSize().width, pNode->getContentSize().height);
    pNode->getParent()->addChild(pClipNode);
    
    pClipNode->setAnchorPoint(ptAnchor);
    pNode->retain();
    pNode->removeFromParent();
    pClipNode->addChild(pNode);
    pNode->setPosition(0, 0);
    pNode->release();
    
    pClipNode->setPosition(pos);
    return pClipNode;
}


ProductionSoldiersView::ProductionSoldiersView(int buildingId)
    :m_buildingId(buildingId)
    ,m_waitInterface(NULL)
    ,m_isWaitingSeverRes(false)
    ,m_isShowRefresh(false)
    ,m_iMaxAttack(0)
    ,m_iMaxDefence(0)
    ,m_iMaxHP(0)
    , m_bPlayAniInfo(false)
    , m_bSoldiersInTouch(false)
{
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this,callfuncO_selector(ProductionSoldiersView::immediatelyHarvestFinish),MSG_QUICK_TROOPS_HARVEST, NULL);
}

ProductionSoldiersView::~ProductionSoldiersView(){
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_QUICK_TROOPS_HARVEST);
}

ProductionSoldiersView* ProductionSoldiersView::create(int buildingId){
    ProductionSoldiersView* ret = new ProductionSoldiersView(buildingId);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool ProductionSoldiersView::init()
{
    if (!ArcPopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    m_guideKey = "";
    
    // m_isFort 表示是造兵还是造陷阱
    m_isFort = m_buildingId/1000 == FUN_BUILD_FORT;

    CCLoadSprite::doResourceByCommonIndex(4, true);
    CCLoadSprite::doResourceByCommonIndex(510, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByCommonIndex(508, true);
    CCBLoadFile("productionSoldierView",this,this);
    setContentSize(CCDirector::sharedDirector()->getWinSize());
    CCLoadSprite::doLoadResourceAsync(COMMON_PATH, CCCallFuncO::create(this, callfuncO_selector(ProductionSoldiersView::AsyLoadRes), NULL), 4);
    m_resIndex = m_buildingId/1000000 - 423 + 200;
    setCleanFunction([=](){
        CCLoadSprite::doResourceByCommonIndex(4, false);
        CCLoadSprite::doResourceByCommonIndex(510, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
        CCLoadSprite::doResourceByCommonIndex(508, false);
        if(!m_isFort && m_resIndex!=-1){
            CCLoadSprite::doResourceByCommonIndex(m_resIndex, false);
        }
    });
    int sliderW = 255;
    auto m_sliderBg = CCLoadSprite::createScale9Sprite("cs_jindutiaoBG.png");
    m_sliderBg->setInsetBottom(5);
    m_sliderBg->setInsetLeft(5);
    m_sliderBg->setInsetRight(5);
    m_sliderBg->setInsetTop(5);
    m_sliderBg->setAnchorPoint(ccp(0.5,0.5));
    m_sliderBg->setPosition(ccp(sliderW/2, 25));
    m_sliderBg->setContentSize(CCSize(sliderW,9));
    
    auto bgSp = CCLoadSprite::createSprite("cs_jindutiaoBG.png");
    bgSp->setVisible(false);
    auto proSp = CCLoadSprite::createSprite("cs_jindutiao.png");
    auto thuSp = CCLoadSprite::createSprite("cs_jindutiao_tr.png");
    if(CCCommonUtils::isIosAndroidPad())
    {
        thuSp->setScaleX(thuSp->getScaleX() * 0.77);
    }
    //bg
    auto _bg = CCLoadSprite::createSprite("frame_04.png");
    if(_bg)
    {
        auto _size = _bg->getContentSize();
        if(m_buildBG)
        {
            m_buildBG->setCapInsets(CCRect(_size.width/2,_size.height/2, 1, 1));
            if(m_buildBG_2)
            {
                m_buildBG_2->setCapInsets(CCRect(_size.width/2,_size.height/2, 1, 1));
            }
        }
    }


    m_slider = CCSliderBar::createSlider(m_sliderBg, proSp, thuSp);
    m_slider->setMinimumValue(0.0f);
    m_slider->setMaximumValue(1.0f);
    m_slider->setProgressScaleX(sliderW/proSp->getContentSize().width);
    m_slider->setTag(1);
    m_slider->setLimitMoveValue(15);
    m_slider->setPosition(ccp(-33, -49));
    if (CCCommonUtils::isIosAndroidPad()) {
        m_slider->setPosition(ccp(-137, -56));
        m_slider->setScaleX(2.6);
        m_slider->setScaleY(2.0);
    }
    m_slider->addTargetWithActionForControlEvents(this, cccontrol_selector(ProductionSoldiersView::moveSlider), CCControlEventValueChanged);
    m_sliderNode->addChild(m_slider, 1);
    
    m_jiaBtn->setZOrder(2);
    m_jianBtn->setZOrder(2);
    
    auto editSize = CCSizeMake(120,25);

    editSize = m_useEditNode->getContentSize();
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        editSize = CCSizeMake(editSize.width,editSize.height+5);
    #endif
    auto editpic =CCLoadSprite::createScale9Sprite("btn_bg_op.png");
    editpic->setContentSize(editSize);
    editpic->setInsetBottom(10);
    editpic->setInsetTop(10);
    editpic->setInsetRight(10);
    editpic->setInsetLeft(10);
    m_useEditTxt = CCEditBox::create(editSize,editpic);
    m_useEditTxt->setInputMode(kEditBoxInputModeNumeric);
    m_useEditTxt->setDelegate(this);
//    m_useEditTxt->setTouchPriority(Touch_Popup);
    m_useEditTxt->setMaxLength(12);
    m_useEditTxt->setReturnType(kKeyboardReturnTypeDone);
    m_useEditTxt->setPosition(ccp(40, 15));
    m_useEditTxt->setFontColor(ccBLACK);
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_useEditTxt->setPosition(ccp(editSize.width / 2.0, editSize.height / 2.0));
    }
    m_useEditNode->addChild(m_useEditTxt);
    
    std::string open_arms = "";
    m_buildingName = "";
    if(m_buildingId!=-1){
        int buildId = FunBuildController::getInstance()->getMaxLvBuildByType(m_buildingId/1000);// 423000 FUN_BUILD_BARRACK1
        if (buildId>0) {
            FunBuildInfo& info = FunBuildController::getInstance()->getFunbuildById(buildId);
            m_buildingLevel = info.level;
            open_arms = info.open_arms;
            m_buildingName = info.name;
            m_buildingstarLv = info.starNum;
        }
    }else{
        auto dictInfo = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(FUN_BUILD_BARRACK1));
        auto tmpBuild = FunBuildInfo(dictInfo);
        m_buildingLevel = 5;
        open_arms = tmpBuild.open_arms;
        CCLOG("error m_buildingId=%d",m_buildingId);
    }
    
    
    ///////////
    m_pos=0;
    int maxOpenLevel = 0;
    m_willNum = -1;
    m_armyIds = ArmyController::getInstance()->getCreateSoldierIds(open_arms, m_isFort);
    
//    m_armys = CCArray::create();
//    std::vector<std::string> armys;
//    CCCommonUtils::splitString(open_arms,"|",armys);
//    int size = armys.size();
//    std::vector<std::string> armyVector;
//    if(m_isFort){
//        for (int i=0; i<size; i++) {
//            armyVector.clear();
//            CCCommonUtils::splitString(armys[i],";",armyVector);
//            int level = atoi(armyVector[0].c_str());
//            map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->fortList.find(armyVector[1]);
//            if(it!=GlobalData::shared()->fortList.end()){
//                it->second.unlockLevel = level;
//                m_armyIds.push_back(it->first);
//                m_armys->addObject(&it->second);
//                if(m_buildingLevel>=it->second.unlockLevel && it->second.unlockLevel >= maxOpenLevel){
//                    m_pos = i;
//                    maxOpenLevel = it->second.unlockLevel;
//                }
//            }
//        }
//    }else{
//        for (int i=0; i<size; i++) {
//            armyVector.clear();
//            CCCommonUtils::splitString(armys[i],";",armyVector);
//            int level = atoi(armyVector[0].c_str());
//            map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->armyList.find(armyVector[1]);
//            if(it!=GlobalData::shared()->armyList.end()){
//                it->second.unlockLevel = level;
//                m_armyIds.push_back(it->first);
//                m_armys->addObject(&it->second);
//            }
//        }
//    }
//    
//    int num = m_armys->count()-1;
//    for(int j=0;j<num;j++){
//        for(int i=0;i<num-j;i++)
//        {
//            ArmyInfo* army1 = (ArmyInfo*)m_armys->objectAtIndex(i);
//            ArmyInfo* army2 = (ArmyInfo*)m_armys->objectAtIndex(i+1);
//            if(army1->unlockLevel > army2->unlockLevel)
//            {
//                m_armys->swap(i, i+1);
//            }
//        }
//    }
//    for(int i=0;i<num;i++){
//         ArmyInfo* army1 = (ArmyInfo*)m_armys->objectAtIndex(i);
//        if(m_buildingLevel>=army1->unlockLevel && army1->unlockLevel >= maxOpenLevel){
//            m_pos = i;
//            maxOpenLevel = army1->unlockLevel;
//        }
//    }
    
    for(int i=(m_armyIds.size()-1);i>=0;i--){
        string curIds = m_armyIds[i];
        if ( m_isFort ) {
            auto& aInfo = GlobalData::shared()->fortList[curIds];
            if (m_buildingLevel>=aInfo.unlockLevel && aInfo.unlockLevel >= maxOpenLevel) {
                m_pos = i;
                maxOpenLevel = aInfo.unlockLevel;
            }
        }else {
            auto& aInfo = GlobalData::shared()->armyList[curIds];
            if (m_buildingLevel>=aInfo.unlockLevel && aInfo.unlockLevel >= maxOpenLevel) {
                m_pos = i;
                maxOpenLevel = aInfo.unlockLevel;
            }
        }
    }
    
//    m_armys->reverseObjects();
//    m_pos = m_armys->count() - 1 -  m_pos;
    
    m_armyId = m_armyIds[m_pos];
    
    this->m_numTxt->setString(_lang_1("104907", ""));
    m_numValueTxt->setPositionX(this->m_numTxt->getContentSize().width * this->m_numTxt->getOriginScaleX() + this->m_numTxt->getPositionX()+5);
    
    int add = getExtendHeightWithoutTop();
    int add2 = getExtendHeight();
    
    if (!CCCommonUtils::isIosAndroidPad()){
        m_arcNode->setPosition(ccp(m_arcNode->getPositionX()-40, m_arcNode->getPositionY()+add/2));
        m_soldierNode->setPosition(ccp(m_soldierNode->getPositionX(), m_soldierNode->getPositionY()+add/2));
        m_nodeShortInfo->setPositionY(m_nodeShortInfo->getPositionY() + add/4);
    }
    
    if(m_isFort){
        m_btnTitle1->setString(_lang("102128").c_str());
        CCCommonUtils::setButtonTitle(this->m_trainBtn, _lang("102126").c_str());
        m_fortLimitTxt->setString(_lang_2("102143", CC_CMDITOA(getCurArmy()->free).c_str(), CC_CMDITOA(this->getMaxFort()).c_str()));
        m_nodeShortInfo->setVisible(false);
    }else{
        m_btnTitle1->setString(_lang("102132").c_str());
        CCCommonUtils::setButtonTitle(this->m_trainBtn, _lang("102131").c_str());
        
        // 因为这里要把坐标往上提，没太好的方法，只能写死相应的值
        if (CCCommonUtils::isIosAndroidPad()) {
            m_nodeDetail->setPositionY(m_soldierNode->getPositionY() + 920.f);
        }
        else {
            m_nodeDetail->setPositionY(m_soldierNode->getPositionY() + 350.f);            
        }
    }
    
    m_fortHelpBtn->setVisible(m_isFort);
    m_arcScroll = NULL;
    m_invalideSlider = false;
    haveClick = false;
    m_soldierNode->reorderChild(m_soldierBg,-10000);//
    
//    CCCommonUtils::setButtonTitle(m_clearCDBtn, _lang("104903").c_str());
    m_cdBarNode->setVisible(false);
    auto pos = m_progrossBar->getPosition();
    m_barClipNode = CCClipNode::create(m_progrossBar->getContentSize().width, m_progrossBar->getContentSize().height);
    m_progrossBar->getParent()->addChild(m_barClipNode);
    m_barClipNode->setAnchorPoint(ccp(0, 0));
    m_progrossBar->retain();
    m_progrossBar->removeFromParent();
    m_barClipNode->addChild(m_progrossBar);
    m_progrossBar->setPosition(0, 0);
    m_progrossBar->release();
    m_barClipNode->setPosition(pos);
    
    //////各项属性
    m_txtAttack->setString((_lang("103600")+": ").c_str());
    m_txtDefence->setString((_lang("103601")+": ").c_str());
    m_txtHP->setString((_lang("103605")+": ").c_str());
    
    m_qid = QID_MAX;
    int buildType = m_buildingId/1000;
    int qType = CCCommonUtils::getQueueTypeByBuildType(buildType);
    if (qType != -1) {
        m_qid = QueueController::getInstance()->getMinTimeQidByType(qType);
        if (m_qid != QID_MAX) {
            auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
            int itemId = QueueController::getInstance()->getItemId(qInfo.key);
            string name = CCCommonUtils::getNameById(CC_ITOA(itemId));
            m_nameText->setString(name);
            m_cdBtnTxt->setString(_lang("104903"));
            
            m_cdBarNode->setVisible(true);
            m_bottomNode->setVisible(false);
            m_resNode->setVisible(false);
        }
    }
//    if(FunBuildController::getInstance()->hasGetOneStar(m_buildingId)){
//        upStarMode();
//    }
    update(1.0f);
    
    // 初始短信息动画
    if (!m_isFort) {
        m_begArmyId = m_armyId;
        schedule(schedule_selector(ProductionSoldiersView::CheckForPlay));
        InitShortInfo();
    }

    return true;
}

void ProductionSoldiersView::upStarMode(){
    // 注意HD版界面没有m_starIcon
    m_detailInfo->setVisible(false);
    auto starSp = CCLoadSprite::createSprite("soldier_star.png");
    m_starIcon->addChild(starSp);
    m_starIcon->setScale(1);
    m_starIcon->setVisible(true);
}

ArmyInfo* ProductionSoldiersView::getCurArmy()
{
    if (m_isFort) {
        return &(GlobalData::shared()->fortList[m_armyId]);
    }else {
        return &(GlobalData::shared()->armyList[m_armyId]);
    }
}

ArmyInfo* ProductionSoldiersView::getArmy(std::string const& armyId)
{
    if (m_isFort) {
        return &(GlobalData::shared()->fortList[armyId]);
    }else {
        return &(GlobalData::shared()->armyList[armyId]);
    }
}

void ProductionSoldiersView::AsyLoadRes(CCObject* p){
    CCNode* node = CCNode::create();
    node->setPosition(ccp(-38.0, 350));
    m_soldierNode->addChild(node,-10);
    
    auto pic1 = CCLoadSprite::createSprite("black.png");
    pic1->setAnchorPoint(ccp(1, 0.5));
    pic1->setOpacity(0);
    node->addChild(pic1);
    pic1->runAction(CCFadeIn::create(0.1));
    
    auto pic2 = CCLoadSprite::createSprite("black.png");
    pic2->setAnchorPoint(ccp(0, 0.5));
    pic2->setFlipX(true);
    pic2->setOpacity(0);
    node->addChild(pic2);
    pic2->runAction(CCFadeIn::create(0.1));
    
    auto pic3 = CCLoadSprite::createSprite("black.png");
    pic3->setAnchorPoint(ccp(1.0, 0));
    pic3->setPosition(ccp(0, -412.0));
    pic3->setFlipY(true);
    pic3->setOpacity(0);
    node->addChild(pic3);
    pic3->runAction(CCFadeIn::create(0.1));
    
    auto pic4 = CCLoadSprite::createSprite("black.png");
    pic4->setAnchorPoint(ccp(0, 0));
    pic4->setPosition(ccp(0, -412.0));
    pic4->setFlipY(true);
    pic4->setFlipX(true);
    pic4->setOpacity(0);
    node->addChild(pic4);
    pic4->runAction(CCFadeIn::create(0.1));
    
}

void ProductionSoldiersView::setSoldierIdAndNum(string armyId, int num)
{
    m_willArmyId = armyId;
    m_willNum = num;
}

void ProductionSoldiersView::updateSoldierIdAndNum(string armyId, int num)
{
    int aid = -1;
    for (int i=0; i<m_armyIds.size(); i++) {
        if (armyId == m_armyIds[i]) {
            aid = i;
        }
    }
    
    if (aid == -1) {
        return;
    }
    
    m_arcScroll->m_currentIndex = aid;
    m_armyId = armyId;
    refresh();
    
    float value = num*1.0/this->getTrainMax();
    m_slider->setValue(value);
    
    ArmyInfo* m_info = getCurArmy();
    if(m_info==NULL)return ;
    if(num<=0 && value<=0){
        num = 1;
        m_invalideSlider = true;
        m_slider->setValue(1);
    }else if(num<1 && value>0){
        m_invalideSlider = true;
        float value = 1.0/this->getTrainMax();
        m_slider->setValue(value);
        num = 1;
    }
    refreshTrainNumText(num);
}

void ProductionSoldiersView::arcButtonClick(){
    //CCLOG("arcButtonClick index=%d",m_arcScroll->m_currentIndex);
//    if(m_armys->count()>m_arcScroll->m_currentIndex){
//        m_info = dynamic_cast< ArmyInfo*>(m_armys->objectAtIndex(m_arcScroll->m_currentIndex));
//    }
    
    if(m_armyIds.size()>m_arcScroll->m_currentIndex){
        m_armyId = m_armyIds[m_arcScroll->m_currentIndex];
    }
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    refresh();
}

void ProductionSoldiersView::addSoldierIcon(){
    int btype = m_buildingId/1000;
    CCPoint pos = ccp(0,172);
    if(btype == FUN_BUILD_FORT){
        pos = ccp(0,50);
    }
    else if(btype == FUN_BUILD_BARRACK4)
    {
        pos = ccp(0,120);
    }
    
    if(!m_isFort){
//        if (CCCommonUtils::isIosAndroidPad() && m_resIndex==200 && CCFileUtils::sharedFileUtils()->isFileExist(COMMON_PATH_200_HD))
//        {
//            CCLoadSprite::doResourceByPathIndex(COMMON_PATH_HD, m_resIndex, true);
//        }
//        else if (CCCommonUtils::isIosAndroidPad() && m_resIndex==201 && CCFileUtils::sharedFileUtils()->isFileExist(COMMON_PATH_201_HD))
//        {
//            CCLoadSprite::doResourceByPathIndex(COMMON_PATH_HD, m_resIndex, true);
//        }
//        else if (CCCommonUtils::isIosAndroidPad() && m_resIndex==202 && CCFileUtils::sharedFileUtils()->isFileExist(COMMON_PATH_202_HD))
//        {
//            CCLoadSprite::doResourceByPathIndex(COMMON_PATH_HD, m_resIndex, true);
//        }
//        else if (CCCommonUtils::isIosAndroidPad() && m_resIndex==203 && CCFileUtils::sharedFileUtils()->isFileExist(COMMON_PATH_203_HD))
//        {
//            CCLoadSprite::doResourceByPathIndex(COMMON_PATH_HD, m_resIndex, true);
//        }
//        else
            CCLoadSprite::doResourceByCommonIndex(m_resIndex, true);
    }
    ArmyInfo* m_info = getCurArmy();
    CCSprite* pic = CCLoadSprite::createSprite(m_info->getBodyIcon().c_str());
    //pic->setScale(m_isFort?1.5:1);
    pic->setPosition(pos);
    m_soldierIconNode->addChild(pic);
    if(m_info!=NULL && m_buildingLevel<m_info->unlockLevel){
        CCCommonUtils::setSpriteGray(pic,true);
    }
    if(btype==FUN_BUILD_BARRACK4){
        pic->setScale(0.85);
        m_soldierBg->setVisible(false);
    }else{
         m_soldierBg->setVisible(true);
    }
}

void ProductionSoldiersView::refreshResource(CCObject* p)
{
    CCDictionary * d = CCDictionary::create();
    d->retain();
    refresh(d);
    d->release();
}

void ProductionSoldiersView::refresh(CCObject* p)
{
    ArmyInfo* m_info = getCurArmy();
    m_soldierIconNode->removeAllChildrenWithCleanup(true);
    m_particleNode->removeAllChildrenWithCleanup(true);
    m_nodeShortInfo->setVisible(false);

    m_stoneNode->setVisible(FunBuildController::getInstance()->getMainCityLv()>=FunBuildController::getInstance()->building_base_k4);
    m_ironNode->setVisible(FunBuildController::getInstance()->getMainCityLv()>=FunBuildController::getInstance()->building_base_k3);
    
    if(FunBuildController::getInstance()->getMainCityLv() < FunBuildController::getInstance()->building_base_k3 && FunBuildController::getInstance()->getMainCityLv()<FunBuildController::getInstance()->building_base_k4){
        m_makeResNode->setPositionX(-90);
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_makeResNode->setPositionX(232.4);
        }
    }else if(FunBuildController::getInstance()->getMainCityLv() >= FunBuildController::getInstance()->building_base_k3 && FunBuildController::getInstance()->getMainCityLv()<FunBuildController::getInstance()->building_base_k4){
        m_ironNode->setPosition(282.0, 11);
        m_makeResNode->setPositionX(-173);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_ironNode->setPositionX(244.8);
            m_ironNode->setPositionY(47.5);
            m_makeResNode->setPositionX(72.4);
        }
    }else if(FunBuildController::getInstance()->getMainCityLv()>=FunBuildController::getInstance()->building_base_k4 && FunBuildController::getInstance()->getMainCityLv() < FunBuildController::getInstance()->building_base_k3){
        m_stoneNode->setPosition(282.0, 11);
        m_makeResNode->setPositionX(-173);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_stoneNode->setPositionX(244.8);
            m_stoneNode->setPositionY(46.1);
            m_makeResNode->setPositionX(72.4);
        }
    }else{
        m_makeResNode->setPositionX(-219);
        m_ironNode->setPosition(282.0, 11);
        m_stoneNode->setPosition(401.0, 11);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_makeResNode->setPositionX(-94.6);
            m_ironNode->setPositionX(244.8);
            m_stoneNode->setPositionX(571.6);
            m_stoneNode->setPositionY(46.1);
            m_ironNode->setPositionY(47.5);
        }
    }
    if(m_info==NULL){
        m_makeResNode->setVisible(false);
        m_bottomNode->setVisible(false);
        m_numValueTxt->setString("0");
        return ;
    }
    
    // 凡是有m_info的话，就更新右上角信息。
    if (m_isFort == false) {
        m_nodeShortInfo->setVisible(true);
        UpdatePlayShortAniTick();
    }
    
    if(m_info!=NULL && m_buildingLevel<m_info->unlockLevel){
        m_lockTxt->setString(_lang_2("102119", _lang(m_buildingName).c_str(), CC_ITOA(m_info->unlockLevel)));
        m_makeResNode->setVisible(false);
        m_bottomNode->setVisible(false);
        m_numValueTxt->setString("0");
        this->addSoldierIcon();
        m_lockTxt->setVisible(true);
        m_sliderNode->setVisible(false);
        return ;
    }
    this->addSoldierIcon();

    if(m_isFort){
        std::string particleName = "WeaponsFire_Wood";
        for (int i=2; i <= 6; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s_%d",particleName.c_str(),i)->getCString());
            m_particleNode->addChild(particle);
        }
    }
    m_makeResNode->setVisible(true);
    m_bottomNode->setVisible(true);
    m_sliderNode->setVisible(true);
    m_lockTxt->setString("");
    if(!m_info->isArmy){
        if(p==NULL){
            //默认值改为上次玩家使用金币建造的数量位置，若玩家上次没有使用金币建造，则默认为当前资源可建造陷阱数量
            int trainNum = 1;
            int lastGoldBuyCount = getLastGoldBuyCount(0);//上次玩家使用金币建造的数量
            if (lastGoldBuyCount>0) {
                trainNum = MIN(lastGoldBuyCount, this->getTrainMax());
            }else{
                trainNum = MIN(getFortResourceTrainMax(), this->getTrainMax());
            }
            float value = trainNum*1.0/this->getTrainMax();
            m_slider->setValue(value);
        }else if (m_slider->getValue()>1){
            m_slider->setValue(1);
        }
    }else{
        if((m_slider->getValue()>1 || p ==NULL) && m_willNum == -1){
            m_slider->setValue(1);
        }
    }
    m_woodTxt->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.totalWood()));
    m_stoneTxt->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.totalStone()));
    m_ironTxt->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.totalIron()));
    m_foodTxt->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.totalFood()));
    //m_moneyTxt->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lMoney));
    if(m_buildingId/1000 == FUN_BUILD_FORT){
        map<std::string, ArmyInfo>::iterator armyIt;
        int total = 0;
        for(armyIt = GlobalData::shared()->fortList.begin(); armyIt != GlobalData::shared()->fortList.end(); armyIt++){
            total += armyIt->second.free;
        }
        m_fortLimitTxt->setString(_lang_2("102143", CC_CMDITOA(total).c_str(), CC_CMDITOA(this->getMaxFort()).c_str()));
    }
    this->m_numValueTxt->setString(CC_CMDITOA(m_info->free+m_info->march));
    
    if (m_qid != QID_MAX) {
        m_cdBarNode->setVisible(true);
        m_bottomNode->setVisible(false);
        m_resNode->setVisible(false);
    }

}

int ProductionSoldiersView::getTrainTime(int num){
    ArmyInfo* m_info = getCurArmy();
    return ArmyController::getInstance()->getTrainTime(num, *m_info);
}

int ProductionSoldiersView::getTrainMax(){
    ArmyInfo* m_info = getCurArmy();
    map<int, FunBuildInfo>::iterator it;
    int trainTotal = 0;
    int armyTotal = 0;
    if(m_info->isArmy){
        for(it = GlobalData::shared()->imperialInfo.begin(); it != GlobalData::shared()->imperialInfo.end(); it++){
            if(it->first / 1000 == FUN_BUILD_BARRACK){
                trainTotal += atoi(it->second.para[1].c_str());
            }
        }
        
        trainTotal += FunBuildController::getInstance()->building_base_k2;
        return trainTotal + CCCommonUtils::getEffectValueByNum(PRODUCT_TRAIN_NUM_EFFECT);
    }else{
        map<std::string, ArmyInfo>::iterator armyIt;
        for(armyIt = GlobalData::shared()->fortList.begin(); armyIt != GlobalData::shared()->fortList.end(); armyIt++){
            armyTotal += armyIt->second.free;
        }
//        int resourceSupport = getFortResourceTrainMax();
        //最大值改为=min（陷阱容量，10小时可制造陷阱量）
        int tenHourCount = getFortCountByHour(10);//10小时可制造陷阱量
        int temp = min(tenHourCount,this->getMaxFort() - armyTotal);
        return MAX(temp, 1);
    }
    return 0;
}

int ProductionSoldiersView::getFortResourceTrainMax(){
    ArmyInfo* m_info = getCurArmy();
    int resourceSupport = INT_MAX;
    auto getMax = [](int max, int totalResource, int resourcePerNum) {
        if(resourcePerNum <= 0){
            return max;
        }
        int num = totalResource / resourcePerNum;
        return max > num ? num : max;
    };
    resourceSupport = getMax(resourceSupport, GlobalData::shared()->resourceInfo.totalFood(), m_info->food);
    resourceSupport = getMax(resourceSupport, GlobalData::shared()->resourceInfo.totalIron(), m_info->iron);
    resourceSupport = getMax(resourceSupport, GlobalData::shared()->resourceInfo.totalWood(), m_info->wood);
    resourceSupport = getMax(resourceSupport, GlobalData::shared()->resourceInfo.totalStone(), m_info->stone);
    
    return resourceSupport;
}

int ProductionSoldiersView::getMaxFort(){
    int total = 0;
    map<int, FunBuildInfo>::iterator it;
    for(it = GlobalData::shared()->imperialInfo.begin(); it != GlobalData::shared()->imperialInfo.end(); it++){
        if(it->first / 1000 == FUN_BUILD_WALL){
            total += it->second.para1;
        }
    }
    return total+CCCommonUtils::getEffectValueByNum(TRAP_NUM_MAX_EFFECT);
}
int ProductionSoldiersView::getFortRecommandCount(){
    ArmyInfo* m_info = getCurArmy();
    int time = m_info->time;
    map<int, FunBuildInfo>::iterator it;
    int total = 0;
    int effect = FORCE_PRODUCT_SPEED_EFFECT;
    for(it = GlobalData::shared()->imperialInfo.begin(); it != GlobalData::shared()->imperialInfo.end(); it++){
        if(it->first / 1000 == FUN_BUILD_FORT){
            total += it->second.para2;
        }
    }
    effect = FORT_PRODUCT_SPEED_EFFECT;
    int num = 14400 * (1.0f + CCCommonUtils::getEffectValueByNum(effect) / 100.0f) * (1.0f + total / 100.0f) / time;
    return num;
}
int ProductionSoldiersView::getFortCountByHour(int hourCount){
    ArmyInfo* m_info = getCurArmy();
    int time = m_info->time;
    map<int, FunBuildInfo>::iterator it;
    int total = 0;
    int effect = FORCE_PRODUCT_SPEED_EFFECT;
    for(it = GlobalData::shared()->imperialInfo.begin(); it != GlobalData::shared()->imperialInfo.end(); it++){
        if(it->first / 1000 == FUN_BUILD_FORT){
            total += it->second.para2;
        }
    }
    effect = FORT_PRODUCT_SPEED_EFFECT;
    int num = (hourCount*60*60) * (1.0f + CCCommonUtils::getEffectValueByNum(effect) / 100.0f) * (1.0f + total / 100.0f) / time;
    return num;
}
void ProductionSoldiersView::refreshTrainNumText(int num){
    ArmyInfo* m_info = getCurArmy();
    float pro = MAX( 1-CCCommonUtils::getEffectValueByNum(PRODUCT_TRAIN_RES_EFFECT)/100 , 0.001);
    double woodNum = m_info->wood * num * pro;
    double foodNum = m_info->food * num * pro;
    //double silverNum = m_info->silver * num;
    double stoneNum = m_info->stone * num * pro;
    double ironNum = m_info->iron * num * pro;
    m_notEnoughRes = false;
    int useGold = 0;
    if(woodNum>GlobalData::shared()->resourceInfo.totalWood()){
        m_useWoodTxt->setString(CC_ITOA_K(woodNum));
        m_useWoodTxt->setColor(ccRED);
        m_notEnoughRes = true;
        useGold += CCCommonUtils::getResGlodByType(Wood,woodNum-GlobalData::shared()->resourceInfo.totalWood());
    }else{
        m_useWoodTxt->setString(CC_ITOA_K(woodNum));
        m_useWoodTxt->setColor(ccBLACK);
    }
    
    if(stoneNum>GlobalData::shared()->resourceInfo.totalStone()){
        m_useStoneTxt->setString(CC_ITOA_K(stoneNum));
        m_useStoneTxt->setColor(ccRED);
        m_notEnoughRes = true;
        useGold += CCCommonUtils::getResGlodByType(Stone,stoneNum-GlobalData::shared()->resourceInfo.totalStone());
    }else{
        m_useStoneTxt->setString(CC_ITOA_K(stoneNum));
        m_useStoneTxt->setColor(ccBLACK);
    }
    
    if(ironNum>GlobalData::shared()->resourceInfo.totalIron()){
        m_useIronTxt->setString(CC_ITOA_K(ironNum));
        m_useIronTxt->setColor(ccRED);
        m_notEnoughRes = true;
        useGold += CCCommonUtils::getResGlodByType(Iron,ironNum-GlobalData::shared()->resourceInfo.totalIron());
    }else{
        m_useIronTxt->setString(CC_ITOA_K(ironNum));
        m_useIronTxt->setColor(ccBLACK);
    }

    if(foodNum>GlobalData::shared()->resourceInfo.totalFood()){
        m_useFoodTxt->setString(CC_ITOA_K(foodNum));
        m_useFoodTxt->setColor(ccRED);
        m_notEnoughRes = true;
        useGold += CCCommonUtils::getResGlodByType(Food,foodNum-GlobalData::shared()->resourceInfo.totalFood());
    }else{
        m_useFoodTxt->setString(CC_ITOA_K(foodNum));
        m_useFoodTxt->setColor(ccBLACK);
    }
    
//    if(silverNum>GlobalData::shared()->resourceInfo.lMoney){
//        m_useMoneyTxt->setString(CC_ITOA_K(GlobalData::shared()->resourceInfo.lMoney));
//        m_useMoneyTxt->setColor(ccRED);
//        m_notEnoughRes = true;
//        useGold += CCCommonUtils::getResGlodByType(Silver,silverNum-GlobalData::shared()->resourceInfo.lMoney);
//    }else{
//        m_useMoneyTxt->setString(CC_ITOA_K(silverNum));
//        m_useMoneyTxt->setColor({254,251,0});
//    }
    m_useEditTxt->setText(CC_CMDITOA(num).c_str());
    int time = getTrainTime(num);
    this->m_timeTxt->setString(CCCommonUtils::timeLeftToCountDown(time).c_str());
    this->m_numValueTxt->setString(CC_CMDITOAL((long)(m_info->free + m_info->march)));
    this->m_btnUseGoldTxt->setString(CC_ITOA(CCCommonUtils::getGoldByTime(time)+useGold));
    bool flag = true;
    if (m_isFort) {
        map<std::string, ArmyInfo>::iterator armyIt;
        int total = 0;
        for(armyIt = GlobalData::shared()->fortList.begin(); armyIt != GlobalData::shared()->fortList.end(); armyIt++){
            total += armyIt->second.free;
        }
        if(total>=this->getMaxFort()){
            flag = false;
        }
    }
    
    m_newIcon->setVisible(false);
    if (m_buildingLevel>=m_info->unlockLevel) {
        string newKey = CCUserDefault::sharedUserDefault()->getStringForKey(m_info->armyId.c_str(), "");
        if (newKey == "1") {
            m_newIcon->setVisible(true);
        }
    }
    
    if(this->getTrainMax() > 0  && m_buildingLevel>=m_info->unlockLevel && flag){
        this->m_immediateBtn->setEnabled(true);
        this->m_trainBtn->setEnabled(true);
    }else{
        this->m_immediateBtn->setEnabled(false);
        this->m_trainBtn->setEnabled(false);
    }
}

void ProductionSoldiersView::moveSlider(CCObject * pSender, Control::EventType pCCControlEvent){
    if (m_invalideSlider) {
        m_invalideSlider = false;
        return ;
    }
    ArmyInfo* m_info = getCurArmy();
    if(m_info==NULL)return ;
    float percent = m_slider->getValue();
    int num = percent*this->getTrainMax();
    if(num<=0 && this->getTrainMax()<=0){
        num = 1;
        m_invalideSlider = true;
        m_slider->setValue(1);
    }else if(num<1 && this->getTrainMax()>0){
        m_invalideSlider = true;
        float value = 1.0/this->getTrainMax();
        m_slider->setValue(value);
        num = 1;
    }
    refreshTrainNumText(num);
}
void ProductionSoldiersView::onEnter(){
    CCNode::onEnter();
    UIComponent::getInstance()->showResourceBar(true);
    if(!m_isFort){
        CCLoadSprite::doLoadResourceAsync(COMMON_PATH, CCCallFuncO::create(this, callfuncO_selector(ProductionSoldiersView::AsyLoadRes2), NULL), m_resIndex);
    }else{
        this->AsyLoadRes2(NULL);
    }
    this->refresh();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ProductionSoldiersView::refresh), MSG_UPDATE_ARMY_DATA, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ProductionSoldiersView::refreshResource), MSG_CITY_RESOURCES_UPDATE, NULL);
    
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 3, false);
}

void ProductionSoldiersView::AsyLoadRes2(CCObject* p){
    if(m_arcScroll==nullptr){
        auto m_arcArmys = CCArray::create();
        for(int i=0;i<m_armyIds.size();i++){
            
            if ( m_isFort ) {
                auto& aInfo = GlobalData::shared()->fortList[m_armyIds[i]];
                ArcInfo* info = new ArcInfo(i,CCString::createWithFormat("%s",aInfo.getName().c_str())->getCString(),aInfo.getBodyIcon(),aInfo.unlockLevel>m_buildingLevel,100,m_resIndex);
                m_arcArmys->addObject(info);
                info->release();
                
            }else {
                auto& aInfo = GlobalData::shared()->armyList[m_armyIds[i]];
                ArcInfo* info = new ArcInfo(i,CCString::createWithFormat("%s",aInfo.getName().c_str())->getCString(),aInfo.getHeadIcon(),aInfo.unlockLevel>m_buildingLevel,100,204,aInfo.armyId);
                m_arcArmys->addObject(info);
                info->release();
            }
        }
        
        m_arcScroll = ArcScrollView::create(m_arcArmys,2,m_pos,m_buildingstarLv);
        m_arcScroll->setCallback(this, callfunc_selector(ProductionSoldiersView::arcButtonClick));
        m_arcNode->addChild(m_arcScroll);
    }
    if (m_willNum>0) {
        updateSoldierIdAndNum(m_willArmyId, m_willNum);
    }
}

void ProductionSoldiersView::onExit(){
    m_willNum = -1;
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_UPDATE_ARMY_DATA);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CITY_RESOURCES_UPDATE);
    if(!m_isFort && m_resIndex!=-1){
//        if (CCCommonUtils::isIosAndroidPad() && m_resIndex==200 && CCFileUtils::sharedFileUtils()->isFileExist(COMMON_PATH_200_HD))
//        {
//            CCLoadSprite::doResourceByPathIndex(COMMON_PATH_HD, m_resIndex, false);
//        }
//        else if (CCCommonUtils::isIosAndroidPad() && m_resIndex==201 && CCFileUtils::sharedFileUtils()->isFileExist(COMMON_PATH_201_HD))
//        {
//            CCLoadSprite::doResourceByPathIndex(COMMON_PATH_HD, m_resIndex, false);
//        }
//        else if (CCCommonUtils::isIosAndroidPad() && m_resIndex==202 && CCFileUtils::sharedFileUtils()->isFileExist(COMMON_PATH_202_HD))
//        {
//            CCLoadSprite::doResourceByPathIndex(COMMON_PATH_HD, m_resIndex, false);
//        }
//        else if (CCCommonUtils::isIosAndroidPad() && m_resIndex==203 && CCFileUtils::sharedFileUtils()->isFileExist(COMMON_PATH_203_HD))
//        {
//            CCLoadSprite::doResourceByPathIndex(COMMON_PATH_HD, m_resIndex, false);
//        }
//        else
//            CCLoadSprite::doResourceByCommonIndex(m_resIndex, false);
    }
    UIComponent::getInstance()->showResourceBar(false);
    immediatelyHarvestFinish();
    setTouchEnabled(false);
    this->unscheduleAllSelectors();
    CCNode::onExit();
}
//ccTouch
bool ProductionSoldiersView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLOGFUNC();
    m_bSoldiersInTouch = true;
    
//    m_clipHP->setVisible(false);
//    m_clipDefence->setVisible(false);
//    m_clipAttack->setVisible(false);
    
    return true;
}

void ProductionSoldiersView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLOGFUNC();
    m_bSoldiersInTouch = false;
//    startRefreshGame();
    
    
//    m_clipHP->setVisible(true);
//    m_clipDefence->setVisible(true);
//    m_clipAttack->setVisible(true);
    
    if(isTouchInside(m_nodeTouchBar, pTouch)){
        cilckInfoBtn();
    }

}

void ProductionSoldiersView::startRefreshGame(float time){
    if (m_isWaitingSeverRes&&!m_isShowRefresh) {
        YesNoDialog::showVariableTitle(_lang("E100048").c_str(),CCCallFuncO::create(this, callfuncO_selector(ProductionSoldiersView::refreshGame), NULL),_lang("105248").c_str(),true);
        m_isShowRefresh=true;
    }
}

void ProductionSoldiersView::refreshGame(CCObject * pSender){
    CCLOGFUNC();
    m_isShowRefresh=false;
    LogoutCommand* cmd=new LogoutCommand();
    cmd->sendAndRelease();
}
void ProductionSoldiersView::refreshTest(CCObject * pSender){
    CCLOGFUNC();
    m_isShowRefresh=false;
}

void ProductionSoldiersView::onImmediateBtnClick(CCObject * pSender, Control::EventType pCCControlEvent){
    if(haveClick){
//        startRefreshGame();
        return ;
    }
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    m_waitInterface=GameController::getInstance()->showWaitInterface(m_immediateBtn);
    train(1);
}

void ProductionSoldiersView::onTrainBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if ( true && m_guideKey != "" )//假造兵
    {
        ArmyInfo* m_info = getCurArmy();
        int qType = CCCommonUtils::getQueueTypeByBuildType(m_buildingId/1000);
        int qid = QueueController::getInstance()->canMakeItemByType(qType, 0);
        if (qid != QID_MAX) {
            QueueInfo& info = GlobalData::shared()->allQueuesInfo[qid];
            info.startTime = GlobalData::shared()->getWorldTime();
            info.finishTime = info.startTime + 2;
            info.key = m_info->armyId+"00";
            
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                                   , CCString::createWithFormat("SO_train"));
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUICK_TROOPS,CCString::createWithFormat("%d;%d",m_buildingId,0));
        }
        PopupViewController::getInstance()->removeAllPopupView();
        return;
    }
    
    
    if(haveClick){
//        startRefreshGame();
        return ;
    }
    if(!checkEmptyQuene()){
        YesNoDialog *dialog = YesNoDialog::show(_lang("102106").c_str(), CCCallFuncO::create(this, callfuncO_selector(ProductionSoldiersView::gotoCDPanel),NULL), 0);
        dialog->setYesButtonTitle(_lang("101507").c_str());
        return;
    }
    if(m_notEnoughRes){
        int num = m_slider->getValue() * this->getTrainMax();
        int maxNum = getTrainMax();
        if(num > maxNum){
            num = maxNum;
        }
        ArmyInfo* m_info = getCurArmy();
        float pro = MAX( 1-CCCommonUtils::getEffectValueByNum(PRODUCT_TRAIN_RES_EFFECT)/100 , 0.001);
        int woodNum = m_info->wood * num * pro;
        int foodNum = m_info->food * num * pro;
        int silverNum = m_info->silver * num * pro;
        int stoneNum = m_info->stone * num * pro;
        int ironNum = m_info->iron * num * pro;
        int needWood = 0;
        int needFood = 0;
        int needSilver = 0;
        int needStone =0;
        int needIron = 0;
        int useGold = 0;
        if(woodNum>GlobalData::shared()->resourceInfo.totalWood()){
            needWood = woodNum-GlobalData::shared()->resourceInfo.totalWood();
            useGold += CCCommonUtils::getResGlodByType(Wood,needWood);
        }
        if(stoneNum>GlobalData::shared()->resourceInfo.totalStone()){
            needStone = stoneNum-GlobalData::shared()->resourceInfo.totalStone();
            useGold += CCCommonUtils::getResGlodByType(Stone,needStone);
        }
        if(ironNum>GlobalData::shared()->resourceInfo.totalIron()){
            needIron = ironNum-GlobalData::shared()->resourceInfo.totalIron();
            useGold += CCCommonUtils::getResGlodByType(Iron,needIron);
        }
        if(foodNum>GlobalData::shared()->resourceInfo.totalFood()){
            needFood = foodNum-GlobalData::shared()->resourceInfo.totalFood();
            useGold += CCCommonUtils::getResGlodByType(Food,needFood);
        }
//        if(silverNum>GlobalData::shared()->resourceInfo.lMoney){
//            needSilver = silverNum-GlobalData::shared()->resourceInfo.lMoney;
//            useGold += CCCommonUtils::getResGlodByType(Silver,needSilver);
//        }
        if (m_info->isArmy) {
            PopupViewController::getInstance()->addPopupView(ConfirmDialogView::create(needFood,needWood,needIron,needStone,needSilver,useGold,this,callfunc_selector(ProductionSoldiersView::confirmCallBack)),false);
        }else{
            PopupViewController::getInstance()->addPopupView(ConfirmDialogView::create(needFood,needWood,needIron,needStone,needSilver,useGold,this,callfunc_selector(ProductionSoldiersView::confirmCallBack),_lang("102180").c_str(),_lang("102126").c_str()),false);//
        }
        
    }else{
        train(0);
    } 
}

void ProductionSoldiersView::confirmCallBack(){
    train(0); 
}

void ProductionSoldiersView::onInfoBtnClick(CCObject * pSender, Control::EventType pCCControlEvent){
    cilckInfoBtn();
}

void ProductionSoldiersView::cmdStringToNormal(std::string &text){
    if(!text.empty()){
        size_t pos = text.find(",");
        while(pos!=string::npos){
            text.replace(pos, 1, "");
            pos=text.find(",");
        }
    }
}
void ProductionSoldiersView::editBoxTextChanged(CCEditBox* editBox, const std::string& text){
    std::string temp  =text;
    size_t pos = temp.find_first_not_of("1234567890");
    while (pos!=std::string::npos) {
        temp.replace(pos, 1, "");
        pos = temp.find_first_not_of("1234567890");
    }
    int a = atoi(temp.c_str());
    m_useEditTxt->setText(CC_CMDITOA(a).c_str());
}
void ProductionSoldiersView::editBoxReturn(CCEditBox *editBox){
    string numStr = m_useEditTxt->getText();
    int value = 1;
    if(!numStr.empty()){
        cmdStringToNormal(numStr);
        value = atoi(numStr.c_str());
    }
    value = MAX(1, value);
    value = MIN(value, getTrainMax());
    float precent = (float)value/(float)getTrainMax();
    m_slider->setValue(precent);
}
void ProductionSoldiersView::gotoCDPanel(CCObject* p){
    if (m_isShowRefresh) {
        return;
    }
    m_isWaitingSeverRes=false;
    haveClick = false;
    map<int, QueueInfo>::iterator it;
    auto curTime = GlobalData::shared()->getWorldTime();
    int btype = m_buildingId/1000;
    
    ArmyInfo* m_info = getCurArmy();
    if(btype == FUN_BUILD_FORT){
        map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->fortList.find(m_info->armyId);
        if(it!=GlobalData::shared()->fortList.end()){
            it->second.train = m_trainSoldier;
        }
    }else{
        map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->armyList.find(m_info->armyId);
        if(it!=GlobalData::shared()->armyList.end()){
            it->second.train = m_trainSoldier;
        }
    }
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           , CCString::createWithFormat("SO_train"));
    AutoSafeRef temp(this);
    PopupViewController::getInstance()->goBackPopupView();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUICK_TROOPS,CCString::createWithFormat("%d;%d",m_buildingId,0));
 
}
//记录上次用金币购买数量
void ProductionSoldiersView::writeLastGoldBuyCount(int count){
    ArmyInfo* m_info = getCurArmy();
    if(!m_info->isArmy){
        //记录上次用金币购买陷阱数量
        string udKey="trap_"+m_armyId;
        CCUserDefault::sharedUserDefault()->setIntegerForKey(udKey.c_str(), count);
        CCUserDefault::sharedUserDefault()->flush();
    }
}
//获取上次用金币购买数量
int ProductionSoldiersView::getLastGoldBuyCount(int defCount){
    int lastGoldBuyCount=0;
    ArmyInfo* m_info = getCurArmy();
    if(!m_info->isArmy){
        string udKey="trap_"+m_armyId;
        lastGoldBuyCount = CCUserDefault::sharedUserDefault()->getIntegerForKey(udKey.c_str(), defCount);//上次玩家使用金币建造的数量
    }
    return lastGoldBuyCount;
}

void ProductionSoldiersView::maxInstantCallBack(){
    ArmyInfo* m_info = getCurArmy();
    string armyId = m_info->armyId;
    if (armyId=="") return ;
    int num = this->getTrainMax();
    if(m_info->isArmy){
        TrainSoldierWithCommand *cmd= new TrainSoldierWithCommand(armyId, num, 1);
        cmd->sendAndRelease();
    }else{
        FortStartUseGoldCommand *cmd = new FortStartUseGoldCommand(armyId, num, 1);
        cmd->sendAndRelease();
    }
    PopupViewController::getInstance()->goBackPopupView(this);
}

void ProductionSoldiersView::train(int type){
    ArmyInfo* m_info = getCurArmy();
    if(!checkEmptyQuene() && type ==0){
        YesNoDialog *dialog = YesNoDialog::show(_lang("102106").c_str(), CCCallFuncO::create(this, callfuncO_selector(ProductionSoldiersView::gotoCDPanel),NULL), 0);
        dialog->setYesButtonTitle(_lang("101507").c_str());
        return;
    }
    string armyId = m_info->armyId;
    if(armyId == "") return ;
    int num = m_slider->getValue() * this->getTrainMax();
    int maxNum = getTrainMax();
    if(num > maxNum){
        num = maxNum;
    }
    float pro = MAX( 1-CCCommonUtils::getEffectValueByNum(PRODUCT_TRAIN_RES_EFFECT)/100 , 0.001);
    int woodNum = m_info->wood * num * pro;
    int foodNum = m_info->food * num * pro;
    int silverNum = m_info->silver * num * pro;
    int stoneNum = m_info->stone * num * pro;
    int ironNum = m_info->iron * num * pro;
    int needWood = 0;
    int needFood = 0;
    int needSilver = 0;
    int needStone = 0;
    int needIron = 0;
    int useGold = 0;
    if(woodNum>GlobalData::shared()->resourceInfo.totalWood()){
        needWood = woodNum-GlobalData::shared()->resourceInfo.totalWood();
        useGold += CCCommonUtils::getResGlodByType(Wood,needWood);
    }
    if(stoneNum>GlobalData::shared()->resourceInfo.totalStone()){
        needStone = stoneNum-GlobalData::shared()->resourceInfo.totalStone();
        useGold += CCCommonUtils::getResGlodByType(Stone,needStone);
    }
    if(ironNum>GlobalData::shared()->resourceInfo.totalIron()){
        needIron = ironNum-GlobalData::shared()->resourceInfo.totalIron();
        useGold += CCCommonUtils::getResGlodByType(Iron,needIron);
    }
    if(foodNum>GlobalData::shared()->resourceInfo.totalFood()){
        needFood = foodNum-GlobalData::shared()->resourceInfo.totalFood();
        useGold += CCCommonUtils::getResGlodByType(Food,needFood);
    }
//    if(silverNum>GlobalData::shared()->resourceInfo.lMoney){
//        needSilver = silverNum-GlobalData::shared()->resourceInfo.lMoney;
//        useGold += CCCommonUtils::getResGlodByType(Silver,needSilver);
//    }
    if(type == 2){
        maxInstantCallBack();
        return;
    }
    int time = getTrainTime(num);
    if(num == 0){
        return;
    }
    if(type == 0){
        if(GlobalData::shared()->playerInfo.gold < useGold){
            //YesNoDialog::gotoPayTips();
            YesNoDialog::showYesDialog(_lang("E100001").c_str());
            return ;
        }
        haveClick = true;
        m_trainSoldier = num;
        if (useGold>0) {
            writeLastGoldBuyCount(num);//把用金币购买的数量写入userdefault
        }
        m_isWaitingSeverRes=true;
        if(m_info->isArmy){
            TrainSoldierCommand *cmd= new TrainSoldierCommand(armyId, num,useGold>0?true:false);
            if (GuideController::share()->getCurGuideID() == GUIDE_TROOP_CRT_ED) {
                cmd->putParam("guideStep", CCString::create(GUIDE_TROOP_CRT_ED));
            }
            cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ProductionSoldiersView::gotoCDPanel), NULL));//refreshTest
            cmd->sendAndRelease();
        }else{
            FortStartCommand *cmd= new FortStartCommand(armyId, num,useGold>0?true:false);
            cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ProductionSoldiersView::gotoCDPanel), NULL));//gotoCDPanel
            cmd->sendAndRelease();
        }
        //this->scheduleOnce(schedule_selector(ProductionSoldiersView::startRefreshGame), 3.0f);
        SoundController::sharedSound()->playEffects(Music_Sfx_city_train);
    }else{
        int gold = CCCommonUtils::getGoldByTime(time)+useGold;
        if(CCCommonUtils::isEnoughResourceByType(Gold, gold)){
            std::string tmpStr = this->m_useEditTxt->getText();
            cmdStringToNormal(tmpStr);
            int num = atoi(tmpStr.c_str());
            writeLastGoldBuyCount(num);//把用金币购买的数量写入userdefault
            m_trainSoldier = num;
            if(m_info->isArmy){
                TrainSoldierWithCommand *cmd= new TrainSoldierWithCommand(armyId, num,1);
                cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ProductionSoldiersView::immediatelyFinishHandle), NULL));
                cmd->sendAndRelease();
            }else{
                FortStartUseGoldCommand *cmd = new FortStartUseGoldCommand(armyId, num,1);
                cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(ProductionSoldiersView::immediatelyFinishHandle), NULL));
                cmd->sendAndRelease();
            }
            SoundController::sharedSound()->playEffects(Music_Sfx_city_train);
            haveClick = true;
            //m_slider->setValue(this->getTrainMax());
        }else{
//            CCCommonUtils::flyText(_lang("104912"));
            YesNoDialog::gotoPayTips();
        }
    }
    //this->closeSelf();
}

void ProductionSoldiersView::immediatelyFinishHandle(CCObject* p){
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           , CCString::createWithFormat("SO_train"));
    
    haveClick = false;
    ArmyInfo* m_info = getCurArmy();
    if(m_buildingId/1000==FUN_BUILD_FORT){
        map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->fortList.find(m_info->armyId);
        if(it!=GlobalData::shared()->fortList.end()){
            it->second.train = m_trainSoldier;
        }
    }else{
        map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->armyList.find(m_info->armyId);
        if(it!=GlobalData::shared()->armyList.end()){
            it->second.train = m_trainSoldier;
        }
    }
    //PopupViewController::getInstance()->goBackPopupView();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_QUICK_TROOPS,CCString::createWithFormat("%d;%d",m_buildingId,1));
    
}
void ProductionSoldiersView::immediatelyHarvestFinish(CCObject *ccObj){
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
}

void ProductionSoldiersView::instantCallBack(){
    ArmyInfo* m_info = getCurArmy();
    std::string tmpStr =this->m_useEditTxt->getText();
    cmdStringToNormal(tmpStr);
    int num = atoi(tmpStr.c_str());
    if(m_info->isArmy){
        TrainSoldierWithCommand *cmd= new TrainSoldierWithCommand(m_info->armyId, num,1);
        cmd->sendAndRelease();
    }else{
        FortStartUseGoldCommand *cmd = new FortStartUseGoldCommand(m_info->armyId, num,1);
        cmd->sendAndRelease();
    }
    
    m_slider->setValue(this->getTrainMax());
    //PopupViewController::getInstance()->goBackPopupView(this);
}

bool ProductionSoldiersView::checkEmptyQuene(){
    int btype = m_buildingId/1000;
    int qtype = 0;
    switch (btype) {
        case FUN_BUILD_BARRACK1:
            qtype = TYPE_FORCE;
            break;
        case FUN_BUILD_BARRACK2:
            qtype = TYPE_RIDE_SOLDIER;
            break;
        case FUN_BUILD_BARRACK3:
            qtype = TYPE_BOW_SOLDIER;
            break;
        case FUN_BUILD_BARRACK4:
            qtype = TYPE_CAR_SOLDIER;
            break;
        case FUN_BUILD_FORT:
            qtype = TYPE_FORT;
            break;
        default:
            qtype = TYPE_FORCE;
            break;
    }
    map<int, QueueInfo> ::iterator queueIt;
    bool flag = true;
    QueueInfo* queue = NULL;
    for(queueIt = GlobalData::shared()->allQueuesInfo.begin(); queueIt != GlobalData::shared()->allQueuesInfo.end(); queueIt++){
        auto curTime = queueIt->second.getTime();
        if(queueIt->second.type==qtype && queueIt->second.finishTime > curTime){
            flag = false;
            queue = &queueIt->second;
            break;
        }
    }
    return flag;
}

void ProductionSoldiersView::onFortHelpClick(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("102466")) );
}
void ProductionSoldiersView::onAddClick(CCObject * pSender, Control::EventType pCCControlEvent){
    int num = m_slider->getValue() * this->getTrainMax();
    num = MIN(num, getTrainMax());
    num++;
    num = MIN(num, getTrainMax());
    float precent = (float)num/(float)getTrainMax();
    m_slider->setValue(precent);
}
void ProductionSoldiersView::onReduceClick(CCObject * pSender, Control::EventType pCCControlEvent){
    int num = m_slider->getValue() * this->getTrainMax();
    num = MAX(1, num);
    num--;
    num = MAX(1, num);
    float precent = (float)num/(float)getTrainMax();
    m_slider->setValue(precent);
}
cocos2d::SEL_MenuHandler ProductionSoldiersView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}
SEL_CCControlHandler ProductionSoldiersView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onImmediateBtnClick", ProductionSoldiersView::onImmediateBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTrainBtnClick", ProductionSoldiersView::onTrainBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInfoBtnClick", ProductionSoldiersView::onInfoBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFortHelpClick", ProductionSoldiersView::onFortHelpClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this,"onAddClick",ProductionSoldiersView::onAddClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this,"onReduceClick",ProductionSoldiersView::onReduceClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this,"onClearCDClick",ProductionSoldiersView::onClearCDClick);
    return NULL;
}

bool ProductionSoldiersView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_immediateBtn", CCControlButton*, this->m_immediateBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_trainBtn", CCControlButton*, this->m_trainBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arcNode", CCNode*, this->m_arcNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resNode", CCNode*, this->m_resNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numTxt", CCLabelIF*, this->m_numTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numValueTxt", CCLabelIF*, this->m_numValueTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woodTxt", CCLabelIF*, this->m_woodTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_useWoodTxt", CCLabelIF*, this->m_useWoodTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stoneTxt", CCLabelIF*, this->m_stoneTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_useStoneTxt", CCLabelIF*, this->m_useStoneTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ironTxt", CCLabelIF*, this->m_ironTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_useIronTxt", CCLabelIF*, this->m_useIronTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodTxt", CCLabelIF*, this->m_foodTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_useFoodTxt", CCLabelIF*, this->m_useFoodTxt);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moneyTxt", CCLabelIF*, this->m_moneyTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_useMoneyTxt", CCLabelIF*, this->m_useMoneyTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnTitle1", CCLabelIF*, this->m_btnTitle1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnUseGoldTxt", CCLabelIF*, this->m_btnUseGoldTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnTitle2", CCLabelIF*, this->m_btnTitle2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_useEditNode", CCNode*, this->m_useEditNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_soldierNode", CCNode*, this->m_soldierNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockTxt", CCLabelIF*, this->m_lockTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_makeResNode", CCNode*, this->m_makeResNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_soldierIconNode", CCNode*, this->m_soldierIconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_soldierBg", CCScale9Sprite*, this->m_soldierBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fortLimitTxt", CCLabelIF*, this->m_fortLimitTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woodNode", CCNode*, this->m_woodNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodNode", CCNode*, this->m_foodNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ironNode", CCNode*, this->m_ironNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stoneNode", CCNode*, this->m_stoneNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sliderNode", CCNode*, this->m_sliderNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode", CCNode*, this->m_particleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fortHelpBtn", CCControlButton*, this->m_fortHelpBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_jianBtn", CCControlButton*, this->m_jianBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_jiaBtn", CCControlButton*, this->m_jiaBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cdBarNode", CCNode*, this->m_cdBarNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", CCLabelIF*, this->m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progrossBar", CCScale9Sprite*, this->m_progrossBar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeText", CCLabelIF*, this->m_timeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clearCDBtn", CCControlButton*, this->m_clearCDBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cdBtnTxt", CCLabelIF*, this->m_cdBtnTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cdGoldTxt", CCLabelIF*, this->m_cdGoldTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_detailInfo", CCSprite*, this->m_detailInfo);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_starIcon", CCNode*, this->m_starIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newIcon", CCSprite*, this->m_newIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeDetail", CCNode*, this->m_nodeDetail);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeShortInfo", CCNode*, this->m_nodeShortInfo);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeTouchBar", CCSprite*, this->m_nodeTouchBar);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeProgress", CCNode*, this->m_nodeProgressSize);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txtAttack", CCLabelIF*, this->m_txtAttack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barAttack", CCScale9Sprite*, this->m_barAttack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barAttackBack", CCScale9Sprite*, this->m_barAttackBack);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txtDefence", CCLabelIF*, this->m_txtDefence);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barDefence", CCScale9Sprite*, this->m_barDefence);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barDefenceBack", CCScale9Sprite*, this->m_barDefenceBack);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txtHP", CCLabelIF*, this->m_txtHP);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barHP", CCScale9Sprite*, this->m_barHP);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barHPBack", CCScale9Sprite*, this->m_barHPBack);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG_2", CCScale9Sprite*, this->m_buildBG_2);
    
    return false;
}

CCNode* ProductionSoldiersView::getGuideNode(string _key)
{
    if (m_trainBtn->isEnabled()) {
        m_guideKey = _key;
        return m_trainBtn;
    }
    return NULL;
}

void ProductionSoldiersView::onClearCDClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    ClearCD();
}


void ProductionSoldiersView::update(float time)
{
    if (m_qid == QID_MAX) {
        return;
    }
    
    auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
    int tmpCurTime = GlobalData::shared()->getWorldTime();
    if(qInfo.startTime==0){
        qInfo.startTime = tmpCurTime;
    }
    int m_sumTime = qInfo.finishTime - qInfo.startTime;
    int m_curTime = qInfo.finishTime - tmpCurTime;
    m_curTime = MAX(0, m_curTime);
    m_sumTime = MAX(1, m_sumTime);
    if (qInfo.finishTime<=0) {
        m_curTime = 0;
        m_sumTime = 1;
    }
    
    m_tmpGold = CCCommonUtils::getGoldByTime(m_curTime);
    m_cdGoldTxt->setString(CC_ITOA(m_tmpGold));
    
    float pro = m_curTime*1.0/m_sumTime;
    pro = pro>1?1:pro;
    m_timeText->setString(CC_SECTOA(m_curTime));
    
    int totalW = 342;
    float w = totalW - pro*totalW;
    m_barClipNode->setContentSize(CCSize(w, m_progrossBar->getContentSize().height));
    
    if (m_curTime<=0) {
        m_qid = QID_MAX;
        m_cdBarNode->setVisible(false);
        m_bottomNode->setVisible(true);
        m_resNode->setVisible(true);
        PopupViewController::getInstance()->removeAllPopupView();
    }
}

void ProductionSoldiersView::ClearCD()
{
    if (m_tmpGold > 0) {
        if (GlobalData::shared()->playerInfo.gold < m_tmpGold) {
            YesNoDialog::gotoPayTips();
            return;
        }
    }
    QueueController::getInstance()->startCCDQueue(m_qid, "", false, m_tmpGold,"",2);
    PopupViewController::getInstance()->removeAllPopupView();
}

void ProductionSoldiersView::SetCurBarProgress()
{
    ArmyInfo* ai = getCurArmy();
    SetBarProgress(1, ai->attack, m_iMaxAttack);
    SetBarProgress(2, ai->defence, m_iMaxDefence);
    SetBarProgress(3, ai->health, m_iMaxHP);
    
    return;
}

void ProductionSoldiersView::SetAniBarProgress(float dt)
{
    ArmyInfo* aiBeg = getArmy(m_begArmyId);
    ArmyInfo* aiEnd = getArmy(m_armyId);
    
    SetBarProgress(1, dt, aiBeg->attack, aiEnd->attack, m_iMaxAttack);
    SetBarProgress(2, dt, aiBeg->defence, aiEnd->defence, m_iMaxDefence);
    SetBarProgress(3, dt, aiBeg->health, aiEnd->health, m_iMaxHP);
    
    return;
}

void ProductionSoldiersView::SetBarProgress(int type, float v, float vMax)
{
    Size s = m_nodeProgressSize->getContentSize();
    
    float pro;
    pro = v/vMax;
    pro = pro>1?1:pro;
    
    int iWidth = (pro * (float)s.width);
    if (iWidth > s.width) {
        iWidth = s.width;
    }
    if (iWidth < 0) {
        iWidth = 0;
    }
    
    CCNode* pProgressBar = NULL;
    CCScale9Sprite* pProgressBarBack = NULL;
    switch (type) {
    default:
    case 1:
        pProgressBar = m_clipAttack;
        pProgressBarBack = m_barAttackBack;
        break;
        
    case 2:
        pProgressBar = m_clipDefence;
        pProgressBarBack = m_barDefenceBack;
        break;
            
    case 3:
        pProgressBar = m_clipHP;
        pProgressBarBack = m_barHPBack;
        break;
    }

    Size s2(iWidth, s.height);
    
    // 背板缩放绘制
    if (pProgressBarBack) {
        pProgressBarBack->setPreferredSize(s2);
    }
    
    // 前景裁剪
    if (pProgressBar) {
        s2.width -= 3.f;
        pProgressBar->setContentSize(s2);
    }
    
    return;
}

void ProductionSoldiersView::SetBarProgress(int type, float dt, int iBeg, int iEnd, int iMax)
{
    const double dMax = 0.5f;
    float fCur = 0.f;
    
    if (dt < dMax) {
        fCur = (float)iBeg + (float)(iEnd - iBeg) * (dt / dMax);
    }
    else {
        fCur = iEnd;
    }
    
    SetBarProgress(type, fCur, iMax);
    return;
}

void ProductionSoldiersView::InitShortInfo()
{
    m_iMaxAttack = 10;
    m_iMaxDefence = 10;
    m_iMaxHP = 10;
    
    for (int i=0; i<m_armyIds.size(); ++i)
    {
        map<std::string, ArmyInfo>::iterator ite = GlobalData::shared()->armyList.find(m_armyIds[i]);
        if (ite == GlobalData::shared()->armyList.end()) {
            continue;
        }
        
        ArmyInfo& ai = ite->second;
        if (ai.attack > m_iMaxAttack) {
            m_iMaxAttack = ai.attack;
        }
        if (ai.defence > m_iMaxDefence) {
            m_iMaxDefence = ai.defence;
        }
        if (ai.health > m_iMaxHP) {
            m_iMaxHP = ai.health;
        }
    }
    
    // 因为在iPad模拟器上缩放有比较明显的走样，所以这里修改了一下这个滚动条的做法
    // 现在的做法是将滚动条在相同的位置绘制了两遍，背板是拉伸绘制，前面是截断绘制。
    // 这样截断之后显示的右边的边，就是背板的边。目前看起来走样比较小。
    
    // 初始化各个条
    m_clipHP = SetNodeClip(m_barHP, ccp(0, 0));
    m_clipDefence = SetNodeClip(m_barDefence, ccp(0, 0));
    m_clipAttack = SetNodeClip(m_barAttack, ccp(0, 0));
    
    m_barAttackBack->setPosition(m_clipAttack->getPositionX(), m_clipAttack->getPositionY());
    m_barDefenceBack->setPosition(m_clipDefence->getPositionX(), m_clipDefence->getPositionY());
    m_barHPBack->setPosition(m_clipHP->getPositionX(), m_clipHP->getPositionY());
    
    SetCurBarProgress();
    return;
}

void ProductionSoldiersView::CheckForPlay(float dt)
{
    // 已经相同了直接返回
    if (m_begArmyId == m_armyId) {
        return;
    }
    
    // 如果玩家拖动的话，就延迟0.5秒播放，防止他想看从1段到10段的差距
    // 如果玩家松开的话，那么就立即播放当前的
    const float fWait = 0.5f;
    if (m_bSoldiersInTouch) {
        double d = (utils::gettime() - m_fAniTick - fWait);
        if (d < 0.f) {
            //CCLOG("ProductionSoldiersView::CheckForPlay - (%0.2f)", d);
            return;
        }
    }
    
    //CCLOG("ProductionSoldiersView::CheckForPlay - StartPlayShortInfo!");
    StartPlayShortInfo();
    return;
}

void ProductionSoldiersView::PlayShortInfo(float dt)
{
    double d = (utils::gettime() - m_fAniTick);
    //CCLOG("ProductionSoldiersView::PlayShortInfo - d(%f)", d);

    if (d > 0.5f) {
        StopPlayShortInfo();
    }
    else {
        SetAniBarProgress(d);
    }
}

void ProductionSoldiersView::StartPlayShortInfo()
{
    // 如果现在已经在播放状态了，那么就直接设置到目标
    // 如果现在没在播放，加一个播放目标
    if (m_bPlayAniInfo) {
        return;
    }
    
    // 开始播放简短信息
    schedule(schedule_selector(ProductionSoldiersView::PlayShortInfo));
    m_bPlayAniInfo = true;
    m_fAniTick = utils::gettime();
    return;
}

void ProductionSoldiersView::StopPlayShortInfo()
{
    unschedule(schedule_selector(ProductionSoldiersView::PlayShortInfo));
    m_bPlayAniInfo = false;
    m_begArmyId = m_armyId;
    SetCurBarProgress();
    
    return;
}

void ProductionSoldiersView::UpdatePlayShortAniTick()
{
    // 每次玩家滚动的时候更新时间戳
    m_fAniTick = utils::gettime();
}

void ProductionSoldiersView::cilckInfoBtn()
{
    ArmyInfo* m_info = getCurArmy();
    //    PopupViewController::getInstance()->addPopupInView(SoldierInfoView::create(m_info,m_buildingId));
    if(m_buildingId/1000 == FUN_BUILD_FORT){
        PopupViewController::getInstance()->addPopupInView(SoldierInfoView::create(m_info,m_buildingId));
    }
    else{
        PopupViewController::getInstance()->addPopupView(SoldierMoreInfoView::create(m_info,m_buildingId,m_armyIds));
    }
    if (m_newIcon->isVisible()) {
        CCUserDefault::sharedUserDefault()->setStringForKey(m_info->armyId.c_str(), "");
        CCUserDefault::sharedUserDefault()->flush();
        m_newIcon->setVisible(false);
    }

    return;
}


