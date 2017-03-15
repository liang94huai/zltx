//
//  IFResourceLayer.cpp
//  IF
//
//  Created by wangdianzhen on 15/12/25.
//
//

#include "IFResourceLayer.hpp"
#include "PopupViewController.h"
#include "UIComponent.h"
#include "ChangePicCommand.h"
#include "WorldController.h"
#include "WorldCommand.h"
#include "SceneController.h"
#include "WorldMapView.h"
#include "YesNoDialog.h"
#include "AllianceTerritoryShowCommand.h"
#include "YuanJunInfo.h"
#include "ParticleFireAni.h"
#include "YuanJunDetailView.h"
#include "AllianceScienceView.h"
#include "TerritoryFunctionView.h"
#include "TerritoryTowerTipsView.h"
#define  MSG_TERRITORY_RES_DETAIL_CELL_CLICK       "msg_territory_res_detail_cell_click"

IFResourceLayer::IFResourceLayer(WorldCityInfo& info):m_cityInfo(info){
};

IFResourceLayer::~IFResourceLayer(){
};

IFResourceLayer *IFResourceLayer::create(WorldCityInfo& info){
    IFResourceLayer *ret = new IFResourceLayer(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void IFResourceLayer::onEnter(){
    UIComponent::getInstance()->showPopupView(1);
    PopupBaseView::onEnter();
    CCLoadSprite::doResourceByCommonIndex(504, true);
    CCLoadSprite::doResourceByCommonIndex(500, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(504, false);
        CCLoadSprite::doResourceByCommonIndex(500, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    m_tabView->reloadData();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(IFResourceLayer::onCellClick), MSG_TERRITORY_RES_DETAIL_CELL_CLICK, NULL);
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void IFResourceLayer::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TERRITORY_RES_DETAIL_CELL_CLICK);
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    PopupBaseView::onExit();
}

bool IFResourceLayer::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void IFResourceLayer::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    
}

bool IFResourceLayer::init(){
    if(PopupBaseView::init()){
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(504, true);
        CCLoadSprite::doResourceByCommonIndex(500, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(7, false);
            CCLoadSprite::doResourceByCommonIndex(504, false);
            CCLoadSprite::doResourceByCommonIndex(500, false);
        });
        setIsHDPanel(true);
        m_data = CCArray::create();
        
        requestDetail();
        auto node = CCBLoadFile("ResourceLayer", this, this);
        if (CCCommonUtils::isIosAndroidPad()) {
            this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        }
        else {
            this->setContentSize(node->getContentSize());
            int extH = getExtendHeight();
            m_infoList->setPositionY(m_infoList->getPositionY() - extH);
            m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height + extH));
            m_bottomNode->setPositionY(m_bottomNode->getPositionY() - extH);
            m_bgNode->setPositionY(m_bgNode->getPositionY() - extH);
        }

        addLoadingAni();
        
        auto tbg = CCLoadSprite::loadResource("technology_09.png");
        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
        auto picBg1 = CCLoadSprite::createSprite("technology_09.png");
        picBg1->setAnchorPoint(ccp(0, 0));
        picBg1->setPosition(ccp(0, 0));
        if (CCCommonUtils::isIosAndroidPad()) {
            picBg1->setScaleX(2.4);
        }
        tBatchNode->addChild(picBg1);
        int maxHeight = CCDirector::sharedDirector()->getWinSize().height - 79;
        int curHeight = picBg1->getContentSize().height;
        while(curHeight < maxHeight)
        {
            auto picBg2 = CCLoadSprite::createSprite("technology_09.png");
            picBg2->setAnchorPoint(ccp(0, 0));
            picBg2->setPosition(ccp(0, curHeight));
            if (CCCommonUtils::isIosAndroidPad()) {
                picBg2->setScaleX(2.4);
            }
            tBatchNode->addChild(picBg2);
            curHeight += picBg2->getContentSize().height;
        }
        m_bgNode->addChild(tBatchNode);
        
        ParticleFireAni* par = ParticleFireAni::create();
        m_fireNode1->addChild(par);
        
        ParticleFireAni* par2 = ParticleFireAni::create();
        m_fireNode2->addChild(par2);
        
        m_tabView = CCTableView::create(this, m_infoList->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(Touch_Default);
        m_infoList->addChild(m_tabView);

        m_isMeCollect = false;
        auto &selfMarch = WorldController::getInstance()->m_selfMarchUuid;
        auto it = selfMarch.find(m_cityInfo.cityIndex);
        if (it != selfMarch.end() && !(it->second.empty())) {
            // todo : show error
            m_isMeCollect = true;
        }
        
        
        
        std::string picStr = "504_bigGoldRes.png";
       
        auto picSprite = CCLoadSprite::createSprite(picStr.c_str());
        picSprite->setScale(0.8);
        if (picSprite) {
            if (CCCommonUtils::isIosAndroidPad()) {
                picSprite->setScale(1.5);
            }
            m_picNode->addChild(picSprite);
        }
        
        if (!WorldController::isInSelfServer(m_cityInfo.tileServerId)) {
            removeLoadingAni();
            refreshView();
        }
        m_isInUpdate = false;
        return true;
    }
    
    return false;
}

void IFResourceLayer::requestDetail() {
    m_isInUpdate = true;
    unsigned index = m_cityInfo.cityIndex;
    if (m_cityInfo.parentCityIndex != -1) {
        index = m_cityInfo.parentCityIndex;
    }
    if (WorldController::isInSelfServer(m_cityInfo.tileServerId)) {
        auto cmd = new NewResourceDetailCmd(index);
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(IFResourceLayer::onDetailCallback), nullptr));
        cmd->sendAndRelease();
    }
}

void IFResourceLayer::onDetailCallback(cocos2d::CCObject *obj) {
    removeLoadingAni();
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
    
    if (info->objectForKey("existMember")) {
        m_cityInfo.m_newResourceInfo.now_capacity = info->valueForKey("existMember")->intValue();
    }
    if (info->objectForKey("maxMember")) {
        m_cityInfo.m_newResourceInfo.max_capacity = info->valueForKey("maxMember")->intValue();
    }
    

    
    m_data->removeAllObjects();
    bool tempFlag = true;
    if (info->objectForKey("army")) {
        CCArray* array = (CCArray*)info->objectForKey("army");
        if(array){
            int count = array->count();
            for(int i=0;i<count;i++){
                auto item = _dict(array->objectAtIndex(i));
                YuanJunInfo* info = new YuanJunInfo();
                info->parseInfo(item);
                m_data->addObject(info);
                if (info->getUid()==GlobalData::shared()->playerInfo.uid) {
                    tempFlag = false;
                    m_marchId = info->getUuid();
                    if(WorldController::getInstance()->m_marchInfo.find(m_marchId) != WorldController::getInstance()->m_marchInfo.end()){
                        auto &marchInfo = WorldController::getInstance()->m_marchInfo[m_marchId];
                        m_digStartTime = marchInfo.digStartTime / 1000;
                        m_digStartNum = marchInfo.digStartNum;
                        m_digSpeed = marchInfo.digSpeed;
                        m_troopMax = marchInfo.troopMax;
                        m_changeCollectSpdTime = marchInfo.changeCollectSpdTime / 1000;
                    }
                }
                info->release();
            }
        }
    }
    
    if (tempFlag) {
        m_digStartTime = 0;
        m_digStartNum = 0;
        m_digSpeed = 0;
        m_troopMax = 0;
        m_changeCollectSpdTime = 0;
    }
    refreshView();
}

void IFResourceLayer::removeLoadingAni(CCObject* p)
{
    if(m_loadingIcon!=nullptr){
        m_loadingIcon->stopAllActions();
        m_loadingIcon->removeFromParent();
    }
}

void IFResourceLayer::addLoadingAni(){
    if (m_loadingIcon==nullptr) {
        m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
        m_loadingIcon->setAnchorPoint(ccp(0.5, 0.5));
        auto cSize = m_infoList->getContentSize();
        m_loadingIcon->setPosition(ccp(cSize.width/2,cSize.height/2));
    }
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
    if(m_loadingIcon->getParent()==NULL){
        m_infoList->addChild(m_loadingIcon,1000000);
    }
}

void IFResourceLayer::refreshView() {

    m_titleTxt->setString("");//todowdz
    m_statLabel->setColor({243,69,0});
    std::string strStat = "";//状态
    strStat = _lang("");//todowdz
    m_statLabel->setString(strStat);
    std::string strDef = CC_CMDITOA(double(m_cityInfo.m_newResourceInfo.sum));
    m_defLabel1->setString(_lang("115382"));
    m_defLabel2->setString(strDef);
    m_tipsLabel->setString(_lang_2("150443", CC_ITOA(m_cityInfo.m_newResourceInfo.now_capacity), CC_ITOA(m_cityInfo.m_newResourceInfo.max_capacity)));
    
    m_timeLabel1->setString(_lang_1("115491",""));
    int speed = round(m_digSpeed * 3600);
    int addSpeed = round(speed - speed / (1.0 + CCCommonUtils::getEffectValueByNum(65) / 100.0));
    string showStr = CC_CMDITOA(speed - addSpeed);
    m_timeLabel2->setString(showStr + string("/h"));
    if(addSpeed != 0){
        m_speedAddText->setVisible(true);
        m_speedAddText->setString(std::string("+ ") + CC_CMDITOA(addSpeed) + "/h");
        m_speedAddText->setColor({52,200,13});
        float sizeWidth = m_timeLabel2->getContentSize().width;
        m_speedAddText->setPositionX(m_timeLabel2->getPositionX() + sizeWidth * m_timeLabel2->getOriginScaleX() + 10);
    }
    else{
        m_speedAddText->setVisible(false);
    }
    
    m_troopLabel1->setString(_lang_1("108868", ""));
    m_troopLabel2->setString(CC_CMDITOA(m_digStartNum));
    m_tabView->reloadData();
}

void IFResourceLayer::update(float sec) {
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
 
    double now = GlobalData::shared()->getTimeStamp();
    
    auto time = now - m_digStartTime;
    int currentNum = m_digSpeed * time;
    if(m_changeCollectSpdTime != 0){
        time = now - m_changeCollectSpdTime;
        currentNum = m_digStartNum + m_digSpeed * time;
    }
    currentNum = MAX(0.0f, currentNum);
    currentNum = MIN(m_troopMax, currentNum);
    m_troopLabel2->setString(CC_CMDITOA(currentNum));
    double timePass = now - m_cityInfo.m_newResourceInfo.digStartTime;
    double resTemp = m_cityInfo.m_newResourceInfo.digSpeed * timePass;
    if(m_cityInfo.m_newResourceInfo.now_capacity == 0){
        resTemp = 0;
    }
    if (m_isMeCollect) {
        resTemp = 0;
    }
    double resLeft = m_cityInfo.m_newResourceInfo.sum - resTemp;
    resLeft = MAX(0.0f, resLeft);
    m_defLabel2->setString(CC_CMDITOA(double(resLeft)).c_str());
}

void IFResourceLayer::onCellClick(CCObject* data){
    CCString* uuid = dynamic_cast<CCString*>(data);
    if(uuid){
        int num = m_data->count();
        for (int i=0; i<num; i++) {
            YuanJunInfo* info = (YuanJunInfo*)m_data->objectAtIndex(i);
            if(info->getUuid()==uuid->getCString()){
                info->setOpen(!info->getOpen());
                break;
            }
        }
        m_tabView->reloadData();
    }
}

void IFResourceLayer::qianFanCallBack(CCObject *data)
{
   
}

void IFResourceLayer::onTipBtnClick(CCObject *pSender, CCControlEvent event){
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    GlobalData::shared()->isBind = true;
#endif
    CCCommonUtils::showHelpShiftSingleFAQ("45220");

}


bool IFResourceLayer::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_picNode", CCNode*, this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_fireNode1", CCNode*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_fireNode2", CCNode*, this->m_fireNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipBtn", CCControlButton*, this->m_tipBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_statLabel", CCLabelIF*, this->m_statLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_defLabel1", CCLabelIF*, this->m_defLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeLabel1", CCLabelIF*, this->m_timeLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_troopLabel1", CCLabelIF*, this->m_troopLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_defLabel2", CCLabelIF*, this->m_defLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeLabel2", CCLabelIF*, this->m_timeLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_speedAddText", CCLabelIF*, this->m_speedAddText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_troopLabel2", CCLabelIF*, this->m_troopLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipsLabel", CCLabelIF*, this->m_tipsLabel);
    return false;
}

SEL_CCControlHandler IFResourceLayer::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipBtnClick", IFResourceLayer::onTipBtnClick);
    return NULL;
}

CCSize IFResourceLayer::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    YuanJunInfo* obj = dynamic_cast<YuanJunInfo*>(m_data->objectAtIndex(idx));
    if (obj->getOpen()) {
        int num = obj->getSoldiers()->count();
        int row = num/2 + (num%2==0?0:1);
        int addH = row*100;
        if (CCCommonUtils::isIosAndroidPad()) {
            addH = row*200;
        }
        if (m_ownerFlag && obj->getUid() != GlobalData::shared()->playerInfo.uid ) {
            if (CCCommonUtils::isIosAndroidPad()) {
                return CCSize(1470, 250+addH+120);
            }
            return CCSize(620, 125+addH+60);
        }
        else {
            if (CCCommonUtils::isIosAndroidPad()) {
                return CCSize(1470, 250+addH+40);
            }
            return CCSize(620, 125+addH+20);
        }
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1470, 250);
    }
    return CCSize(620, 125);
}

CCSize IFResourceLayer::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1470, 250);
    }
    return CCSize(620, 125);
}

CCTableViewCell* IFResourceLayer::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    IFResourceItemCell* cell = (IFResourceItemCell*)table->dequeueCell();
    YuanJunInfo* info = (YuanJunInfo*)m_data->objectAtIndex(idx);
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info);
        }else{
            cell = IFResourceItemCell::create(info,m_infoList,m_cityInfo.cityIndex,m_ownerFlag);
        }
    }
    return cell;
}

ssize_t IFResourceLayer::numberOfCellsInTableView(CCTableView *table)
{
    return m_data->count();
}

void IFResourceLayer::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

IFResourceItemCell *IFResourceItemCell::create(YuanJunInfo* info,CCNode* clickNode,int targetIndex,bool owner){
    IFResourceItemCell *ret = new IFResourceItemCell(info,clickNode,targetIndex,owner);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void IFResourceItemCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
}

void IFResourceItemCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool IFResourceItemCell::init(){
    auto node = CCBLoadFile("ResourceItemCell", this, this);
    this->setContentSize(node->getContentSize());
    m_clickFlag = false;
    m_headImgNode = HFHeadImgNode::create();
    setData(m_info);
    return true;
}

void IFResourceItemCell::setData(YuanJunInfo* info){
    m_info = info;
    CCLoadSprite::doResourceByCommonIndex(204, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(204, false);
    });
    m_nameTxt->setString(m_info->getName().c_str());
    std::string numStr = _lang("108557");
    numStr.append(":");
    numStr.append(CC_CMDITOA(m_info->getNum()));
    m_armyNum->setString(numStr);
    m_bgNodee->setVisible(true);

    
   m_moveNode->setVisible(true);
    
    //    m_soldierNode->removeAllChildrenWithCleanup(true);
    std::string useArmyId = "";
    int num = m_info->getSoldiers()->count();
    int maxCount = 0;
    int count = 0;
    for(int i=0; i<num; i++){
        auto dic = _dict(m_info->getSoldiers()->objectAtIndex(i));
        count = dic->valueForKey("count")->intValue();
        std::string armyId = dic->valueForKey("armyId")->getCString();
        if(count>maxCount){
            maxCount = count;
            useArmyId = armyId;
        }
    }
    double haveTime = (m_info->getArrivalTime() - GlobalData::shared()->getWorldTime());
    if(m_info->getOpen()){
        num = m_info->getSoldiers()->count();
        int row = num/2 + (num%2==0?0:1);
        int addH = 0;
        if (m_owner  && m_info->getUid() != GlobalData::shared()->playerInfo.uid) {
            if (CCCommonUtils::isIosAndroidPad()) {
                addH = row*200 + 120;
            }
            else
                addH = row*100 + 60;
        }
        else {
            if (CCCommonUtils::isIosAndroidPad()) {
                addH = row*200 + 40;
            }
            else
                addH = row*100 + 20;
        }
        
        m_moveNode->setPosition(ccp(0, addH));
        m_renderBg->setPosition(ccp(0, addH));
        m_bgNodee->setPosition(ccp(0, addH));
        maxCount = 0;
        m_renderBg->setVisible(true);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_renderBg->setContentSize(CCSize(1470, addH));
        }
        else
            m_renderBg->setContentSize(CCSize(620,addH));
        m_renderBg->removeAllChildrenWithCleanup(true);
        for (int i=0; i<num; i++) {
            auto dic = _dict(m_info->getSoldiers()->objectAtIndex(i));
            YuanJunSoldierCell* cell = YuanJunSoldierCell::create(dic);
            int rowIndex = i/2;
            int col = i%2;
            if (CCCommonUtils::isIosAndroidPad()) {
                cell->setPosition(ccp(col==0?40:680, m_renderBg->getContentSize().height-220-rowIndex*200));
            }
            else
                cell->setPosition(ccp(col==0?20:340, m_renderBg->getContentSize().height-110-rowIndex*100));
            m_renderBg->addChild(cell);
            count = dic->valueForKey("count")->intValue();
            std::string armyId = dic->valueForKey("armyId")->getCString();
            if(count>maxCount){
                maxCount = count;
                useArmyId = armyId;
            }
        }
        
    }else{
        m_moveNode->setPosition(ccp(0, 0));
        m_bgNodee->setPosition(ccp(0, 0));


        m_renderBg->setVisible(false);
    }
    if(m_info->getGenerals() && m_info->getGenerals()->count()>0){
        std::string head = m_info->getPic();
        if(head==""||head=="0"){
            head = "g044";
        }
        head.append(".png");
        std::string uid = m_info->getUid();
        int picVer = m_info->getpicVer();
        m_icon->removeAllChildrenWithCleanup(true);
        CCSprite* headSpr = CCLoadSprite::createSprite(head.c_str(),true,CCLoadSpriteType_HEAD_ICON);
        CCCommonUtils::setSpriteMaxSize(headSpr, 78);
        m_icon->addChild(headSpr);
        if (CCCommonUtils::isUseCustomPic(picVer))
        {
            m_headImgNode->initHeadImgUrl2(m_icon, CCCommonUtils::getCustomPicUrl(uid, picVer), 1.0f, 78, true);
        }
    }else{
        m_icon->removeAllChildrenWithCleanup(true);
        std::string icon = "ico" + useArmyId+ "_small.png";
        CCSprite* headSpr = CCLoadSprite::createSprite(icon.c_str());
        CCCommonUtils::setSpriteMaxSize(headSpr, 78);
        m_icon->addChild(headSpr);
    }
    m_arrow->setRotation(m_info->getOpen()?270:180);
    
    if(haveTime<=0){
        m_statusTxt->setString(_lang("115380"));//todowdz
    }else{
        m_statusTxt->setString(_lang("115380"));
        this->unschedule(schedule_selector(IFResourceItemCell::updateTime));
        this->schedule(schedule_selector(IFResourceItemCell::updateTime),1);
        this->updateTime(0);
    }
}
void IFResourceItemCell::updateTime(float _time){
    double haveTime = (m_info->getArrivalTime() - GlobalData::shared()->getWorldTime());
    if(haveTime<=0){
        this->unschedule(schedule_selector(IFResourceItemCell::updateTime));
        IFResourceLayer* tInfoView = dynamic_cast<IFResourceLayer*>(PopupViewController::getInstance()->getCurrentPopupView());
        if (tInfoView) {
            tInfoView->requestDetail();
        }
        return ;
    }
}
bool IFResourceItemCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_touchPos = pTouch->getLocation();
    if(isTouchInside(m_clickArea, pTouch)){
        return true;
    }
    return false;
}

void IFResourceItemCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint end = pTouch->getLocation();
    if(isTouchInside(m_clickArea, pTouch) && isTouchInside(m_clickNode, pTouch) && fabsf(end.y - m_touchPos.y)<30){
        m_clickFlag = !m_clickFlag;
        if(m_info->getIndex()==2){
            if (!m_info->getCanUse()) {
                unsigned int current = WorldController::getInstance()->getCurrentMarchCount();
                unsigned int max = WorldController::getInstance()->getMaxMarchCount();
                if (current >= max) {
                    WorldController::getInstance()->showMarchAlert(max);
                    return;
                }
                else
                    attendRally();
            }
            else
                attendRally();
        }else if(m_info->getIndex()==3){
            PopupViewController::getInstance()->addPopupInView(AllianceScienceView::create());
        }else if(m_info->getIndex()==4){
            return;//////////////
        }else{
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TERRITORY_RES_DETAIL_CELL_CLICK,CCString::create(m_info->getUuid().c_str()));
        }
    }
}

void IFResourceItemCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void IFResourceItemCell::attendRally()
{
    WorldController::getInstance()->openMarchDeploy(m_targetIndex,0);
}

bool IFResourceItemCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_armyNum", CCLabelIF*, this->m_armyNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_statusTxt", CCLabelIF*, this->m_statusTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_woodTxt", CCLabelIF*, this->m_woodTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_foodTxt", CCLabelIF*, this->m_foodTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_ironTxt", CCLabelIF*, this->m_ironTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_stoneTxt", CCLabelIF*, this->m_stoneTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_renderBg", CCScale9Sprite*, this->m_renderBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_icon", CCNode*, this->m_icon);



    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_moveNode", CCNode*, this->m_moveNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_arrow", CCSprite*, this->m_arrow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_clickArea", CCNode*, this->m_clickArea);
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_soldierNode", CCNode*, this->m_soldierNode);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bgNodee", CCNode*, this->m_bgNodee);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_scienceTxt", CCLabelIF*, this->m_scienceTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_joinNode", CCSprite*, this->m_joinNode);
    return false;
}

SEL_CCControlHandler IFResourceItemCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}