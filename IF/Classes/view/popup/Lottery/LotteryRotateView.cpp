//
//  LotteryRotateView.cpp
//  IF
//
//  Created by chenfubi on 15-03-02.
//
//

#include "LotteryRotateView.h"
#include "LotteryController.h"
#include "YesNoDialog.h"
#include "LotteryActCommand.h"
#include "SoundController.h"
#include "ParticleController.h"
#include "PopupViewController.h"

#define SCROLL_DEACCEL_RATE  0.95f
#define SCROLL_DEACCEL_DIST  1.0f
#define BOUNCE_DURATION      0.15f
#define INSET_RATIO          0.2f
#define MOVE_INCH            7.0f/160.0f
#define ANGLE_DEACCEL_RATE 0.95f
#define R_A 240
#define R_B 90

#define R_A_HD 240*1.3
#define R_B_HD 80*1.3

static float convertDistanceFromPointToInch(float pointDis)
{
    float factor = ( CCDirector::sharedDirector()->getOpenGLView()->getScaleX() + CCDirector::sharedDirector()->getOpenGLView()->getScaleY() ) / 2;
    return pointDis * factor / CCDevice::getDPI();
}


LotteryRotateView::LotteryRotateView(bool swallow)
: m_isSwallow(swallow)
,m_bDragging(false)
, m_pContainer(NULL)
, m_bTouchMoved(false)
, m_pTouches(NULL)
, currRotationAngle(0)
, startRotationAngle(0)
, m_tScrollAngel(0)
, m_fEndAngle(0)
, m_rotating(false)
, m_lastTouchMoveTime(0)
, m_enabled(true)
, m_startTouchTime(0)
, m_fEndTime3(0)
, m_fTimeGap3(0)
, m_fTimeCnt3(0)
, m_fTimeIdx3(0)
, m_AnimCellIndex(0)
, m_AnimCellAdding(false)
, m_rotateAngle(0)
, m_hasGetReward(false)
, m_speed3(0.0f)
, m_targetCell(NULL)
{
    _ignoreAnchorPointForPosition = false;
    setAnchorPoint(Vec2(0,0));
//    rotateArrX = {0.0f, 0.5f,   0.866f, 1.0f, 0.866f, 0.5f,    0.0f,  -0.5f,   -0.866f, -1.0f, -0.866f, -0.5f};
//    rotateArrY = {1.0f, 0.866f, 0.5f,   0.0f, -0.5f,  -0.866f, -1.0f, -0.866f, -0.5f,   0.0f,  0.5f,    0.866f};
//    rotateAngle = {0,   30,     60,     90,   120,    150,     180,   -150,    -120,    -90,   -60,     -30};
}

LotteryRotateView::~LotteryRotateView()
{
//    m_pTouches->release();
}

LotteryRotateView* LotteryRotateView::create(bool swallow)
{
    LotteryRotateView* pRet = new LotteryRotateView(swallow);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool LotteryRotateView::init()
{
    if (CCNode::init())
    {
         _ignoreAnchorPointForPosition = false;
        setAnchorPoint(Vec2(0,0));
        m_pContainer = CCNode::create();
        this->m_pContainer->ignoreAnchorPointForPosition(false);
        this->m_pContainer->setAnchorPoint(ccp(0.0f, 0.0f));
        
        m_pTouches = CCArray::create();//new CCArray();
        //m_pTouches->init();
        this->addChild(m_pContainer);
        m_pContainer->setPosition(ccp(55.0f, -10.0f));
        
        if (CCCommonUtils::isIosAndroidPad())
        {
//             m_pContainer->setPosition(ccp(55.0f*2, -10.0f*2));
        }
        
        m_rewardsContainer = CCNode::create();
        this->addChild(m_rewardsContainer);
        m_rewardsContainer->setPosition(ccp(0.0f, 250.0f));
        
        return true;
    }
    return false;
}

void LotteryRotateView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    if (m_isSwallow == false){
        setTouchEnabled(true);
    }
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LotteryRotateView::getCmdData), LOTTERYACTVIEW_GET, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LotteryRotateView::setSwallow),LOTTERYACTVIEW_GUIDE_FINISH,NULL);
}

void LotteryRotateView::onExit()
{
    SoundController::sharedSound()->stopAllEffects();
    setTouchEnabled(false);
    this->unschedule(schedule_selector(LotteryRotateView::rotatingHandler));
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, LOTTERYACTVIEW_GET);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, LOTTERYACTVIEW_GUIDE_FINISH);
    CCNode::onExit();
}

void LotteryRotateView::setSwallow(CCObject* pObj)
{
    setTouchEnabled(true);
}
void LotteryRotateView::_addRotateCell(int idx, const char*  itemStr){
    float rotateAngleArr[12] = {0, 30, 60,  90,   120,    150,     180,   -150,    -120,  -90, -60,   -30};
    
    float rotateAngle = rotateAngleArr[idx];
    
    CCSize size = m_pContainer->getContentSize();
    
//    CCLOG("_addRotateCell_id:%s,angle:%f",itemStr,rotateAngle);
    LotteryActCell* cell = LotteryActCell::create(itemStr);
    float radians = CC_DEGREES_TO_RADIANS(rotateAngle);
    cell->setAngle(radians);
    cell->setTag(idx);
    m_pContainer->addChild(cell);
    
}

void LotteryRotateView::_clearRotateCells(){
    int rotateAngleArr[12] = {0, 30, 60,  90,   120,    150,     180,   -150,    -120,  -90, -60,   -30};
    CCNode* rotatebgSp = getPanelChild();
    for(int i = 0; i < 12; i++){
        int rotateAngle = rotateAngleArr[i];
//        int rotatecellbgSpTag = 1000 + rotateAngle;
        int rotatecellImgSpTag = 2000 + rotateAngle;
        int rotatecellTFTag = 2500 + rotateAngle;
        
        rotatebgSp->removeChildByTag(rotatecellImgSpTag);
        //数量
        rotatebgSp->removeChildByTag(rotatecellTFTag);
    }
}

CCNode* LotteryRotateView::getPanelChild(){
    
    return m_pContainer;
}

bool LotteryRotateView::onTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (!this->isVisible())
    {
        return false;
    }
    if (m_rotating)
    {
        return false;
    }
    if (m_AnimCellAdding)
    {
        return false;
    }
    if (!m_enabled) {
//        CCNotificationCenter::sharedNotificationCenter()->postNotification("HFROTATENOTIFY",CCStringMake("TouchBegan"));
        return false;
    }

    //dispatcher does not know about clipping. reject touches outside visible bounds.
    if (m_pTouches->count() > 2 ||
        m_bTouchMoved)
    {
        return false;
    }
    
    if (!m_pTouches->containsObject(touch))
    {
        m_pTouches->addObject(touch);
    }
    
    m_startTouchTime = clock() / 1000;
    
    if (m_pTouches->count() == 1)
    { // scrolling
        m_tTouchPoint     = m_pContainer->convertToNodeSpace(touch->getLocation());
        m_startTouchPoint = m_pContainer->convertToNodeSpace(touch->getLocation());
        startRotationAngle = m_rotateAngle;
        currRotationAngle = startRotationAngle;
        
        m_rotating = false;
        
        m_bTouchMoved     = false;
        m_bDragging       = true; //dragging started
    }
    else if (m_pTouches->count() == 2)
    {
    }
    return true;
}
float LotteryRotateView::getAngleByTouchPoint(CCPoint touchPoint){
    CCPoint zeroPoint = ccp(0,0);
    float angleA = touchPoint.getAngle(zeroPoint);
    angleA = (angleA/M_PI) * 180;
    
    return angleA;
}
void LotteryRotateView::onTouchMoved(CCTouch* touch, CCEvent* event)
{
    if (!this->isVisible())
    {
        return;
    }
    if (m_rotating)
    {
        return;
    }
    if (!m_enabled) {
        return;
    }
    
    if (m_pTouches->containsObject(touch))
    {
        if (m_pTouches->count() == 1 && m_bDragging)
        {
            // scrolling
            CCPoint moveDistance;
            CCPoint lastPoint = touch->getPreviousLocation();
            CCPoint nowPoint = touch->getLocation();
            lastPoint = m_pContainer->convertToNodeSpace(lastPoint);
            nowPoint = m_pContainer->convertToNodeSpace(nowPoint);
            CCPoint lastLine = ccpSub(lastPoint, ccp(0, 0));
            CCPoint nowLine = ccpSub(nowPoint, ccp(0, 0));
            
            CCPoint startPoint = m_pContainer->convertToNodeSpace(touch->getStartLocation());
            float angle = CC_RADIANS_TO_DEGREES(ccpSub(nowPoint, startPoint).getAngle());
            CCLOG("angle:%f", angle);
            if (abs(angle) >= 60 && abs(angle) <= 120)
            {
                return;
            }
            
            moveDistance = ccpSub(nowPoint, m_tTouchPoint);
            if (!m_bTouchMoved)
            {
                moveDistance = CCPointZero;
            }
            
            m_tTouchPoint = nowPoint;
            m_bTouchMoved = true;
            float dis = 0.0f;
            dis = sqrtf(moveDistance.x*moveDistance.x + moveDistance.y*moveDistance.y);

            float inch = convertDistanceFromPointToInch(dis);
            if (!m_bTouchMoved && fabs(inch) < MOVE_INCH )
            {
                CCLOG("Invalid movement, distance = [%f, %f], disInch = %f", moveDistance.x, moveDistance.y, inch);
                m_pTouches->removeObject(touch);
                return;
            }
            m_lastTouchMoveTime = clock() / 1000;
            
            float angleGap = nowLine.getAngle(lastLine);
//            CCLOG("angleGap:%f", angleGap);
            setRotationAngle(-angleGap);
            
            m_tScrollAngel = CC_RADIANS_TO_DEGREES(-angleGap);
            
            if (m_tScrollAngel>=-360 && m_tScrollAngel<=-180) {
                m_tScrollAngel = m_tScrollAngel + 360;
            }
            else if (m_tScrollAngel<=360 && m_tScrollAngel>=180) {
                m_tScrollAngel = 360 - m_tScrollAngel;
            }
            
            this->setContentOffsetForMove();
        }
    }
}

bool LotteryRotateView::isChipEnough()
{
    int cost = LotteryController::shared()->getLotteryCost1() - GlobalData::shared()->resourceInfo.lChip;
    if (cost > 0)
    {
        int money = ceil(cost*LotteryController::shared()->getRate());
        CCCallFuncO* fun = CCCallFuncO::create(this, callfuncO_selector(LotteryRotateView::yesDialogBack), CCInteger::create(money));
        YesNoDialog::show(_lang("111116").c_str(), fun, money, false);
        return false;
    }
    return true;
}

void LotteryRotateView::yesDialogBack(CCObject* pObj)
{
    CCInteger* money = dynamic_cast<CCInteger*>(pObj);
    if (money && GlobalData::shared()->playerInfo.gold < money->getValue())
    {
        YesNoDialog::gotoPayTips();
        return;
    }
//    playAnimation(1);
}

#pragma mark playAnimation
void LotteryRotateView::playAnimation(int useMoney,int rotateType/* = 0*/)
{
    m_rewardsContainer->removeAllChildren();
    //设置转动类型 0-普通，1-连转10次
    LotteryController::shared()->rotateType = rotateType;
    m_startAddCnt1 = 0;
    m_startAddCnt2 = 0;

//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(LOTTERYACTVIEW_SEND, CCString::createWithFormat("%d", useMoney));
    CCDictionary* dict = CCDictionary::create();
    dict->setObject(ccs(LOTTERYACTVIEW_SEND), "key");
    dict->setObject(CCString::createWithFormat("%d", useMoney), "value");
    int direction = 0;//0为顺时针
    if (m_tScrollAngel > 0)
    {
        direction = 1;
    }
    dict->setObject(CCString::createWithFormat("%d", direction), "value1");
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(LOTTERYROTATEVIEW, dict);
    
    m_hasGetReward = false;
    m_rotating = true;
    
    //    setRotationAngle(0);
    m_rotateAngle = 0;
    m_ftimeGap4 = 0.0f;
    m_fTimeGap3 = 1.0f/24.0f;
    m_isSendNotify0 = false;
    m_isSendNotify1 = false;
    this->schedule(schedule_selector(LotteryRotateView::rotatingHandler), m_fTimeGap3);
}

void LotteryRotateView::getCmdData(CCObject *pObj)
{
    CCString* reward = CCString::create("");
    if (LotteryController::shared()->rotateType==1) {//0-普通，1-连转10次
        m_save10Arr = dynamic_cast<CCArray*>(pObj);
        LotteryController::shared()->m_save10Arr = dynamic_cast<CCArray*>(pObj);
        if(m_save10Arr && m_save10Arr->count()>0){
            CCDictionary* lastDict = dynamic_cast<CCDictionary*>(m_save10Arr->objectAtIndex(m_save10Arr->count()-1));
            int dictType = lastDict->valueForKey("type")->intValue();
            CCDictionary* rewardsDic = _dict(lastDict->objectForKey("result"));
            /*  里面的type 是
                RESOURCE, BOX_TIMES, REWARD 0 1 2*/
            if (dictType == 0) {//RESOURCE
                int resType = rewardsDic->valueForKey("type")->intValue();
                reward = CCString::create(CC_ITOA(resType));
            }else if (dictType == 1){//BOX_TIMES
                reward = CCString::create(CC_ITOA(100));
            }else if (dictType == 2){//REWARD
                int resType = rewardsDic->valueForKey("type")->intValue();
                if (resType == 7) {
                    CCDictionary* valueDic = _dict(rewardsDic->objectForKey("value"));
                    int itemId = valueDic->valueForKey("itemId")->intValue();
                    reward = CCString::create(CC_ITOA(itemId));
                }else{
                    reward = CCString::create(CC_ITOA(resType));
                }
            }
        }

    }else{
        reward = dynamic_cast<CCString*>(pObj);
    }
    CCLOG("reward:%s",reward->getCString());
    if (!reward || (reward && reward->compare("error")==0))
    {
        m_targetCell = NULL;
        CCDictionary* dict = CCDictionary::create();
        dict->setObject(ccs(LOTTERYACTVIEW_COM), "key");
        dict->setObject(ccs("error"), "value");
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(LOTTERYROTATEVIEW, dict);
//        rotateFinished();
        this->unschedule(schedule_selector(LotteryRotateView::rotatingHandler));
        m_rotating = false;
        return;
    }
    
    float rotateAngleArr[12] = {0, 30, 60,  90,   120,    150,     180,   -150,    -120,  -90, -60,   -30};
    float rotateAngle = 0.0f;
    LotteryActCell* cell2 = NULL;
    bool getTarget = false;
    for (int i = 0; i < m_pContainer->getChildrenCount(); i++)
    {
        cell2 = dynamic_cast<LotteryActCell*>(m_pContainer->getChildByTag(i));
        if (cell2)
        {
            rotateAngle = rotateAngleArr[i];
            cell2->setAngle(CC_DEGREES_TO_RADIANS(rotateAngle));
            if (!getTarget && reward->compare(cell2->getItemId().c_str())==0)
            {
                m_targetCell = cell2;
                getTarget = true;
            }
        }
    }
    doCellSort();
    if (m_targetCell == NULL)
    {
//        rotateFinished();
        this->unschedule(schedule_selector(LotteryRotateView::rotatingHandler));
        m_rotating = false;
        CCDictionary* dict = CCDictionary::create();
        dict->setObject(ccs(LOTTERYACTVIEW_COM), "key");
        dict->setObject(ccs("error"), "value");
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(LOTTERYROTATEVIEW, dict);
        return;
    }
    
    int index = m_targetCell->getTag();
    // 基准角度
    float addAngle = 0.0;
    if (LotteryController::shared()->rotateType==1) {//0-普通，1-连转10次
//        addAngle = 360;
    }
    if (m_tScrollAngel > 0) {
        m_fEndAngle = -(index+3)*30 + addAngle + (1 + ((int) m_speed3/360)) * 360.0f;//720
    }
    else {
        m_fEndAngle = -(index+3)*30.0f - addAngle - (1 + ((int) m_speed3/360)) * 360.0f;//360
    }
    
    m_fEndTime3 = fabs(m_fEndAngle)/m_speed3;// 一圈的时间
    m_fTimeCnt3 = (int) (m_fEndTime3/m_fTimeGap3)*1.3;
//    if (LotteryController::shared()->rotateType==1) {//0-普通，1-连转10次
//        addRewardsNodeBg();
//    }
//    if (m_fTimeCnt3 > 300)
//    {
//        m_fTimeCnt3 = m_fTimeCnt3/2;
//    }
    m_fTimeIdx3 = 0;
    m_rotateAngle = 0;
    m_hasGetReward = true;
}

#pragma mark 10连抽显示
void LotteryRotateView::addRewardsNodeBg(){
    m_rewardsContainer->removeAllChildren();
    auto spBg = CCLoadSprite::createScale9Sprite("huadongtiao3.png");
    spBg->setInsetBottom(5);//设置非拉伸区域与底部的距离
    spBg->setInsetLeft(5);
    spBg->setInsetRight(5);
    spBg->setInsetTop(5);
    spBg->setAnchorPoint(ccp(0.5,0.5));
    //    spBg->setPosition(ccp(290/2, 25));
    spBg->setContentSize(CCSize(550,200));
    
    m_rewardsContainer->addChild(spBg);
}

void LotteryRotateView::addRewardsNodeByIndex(int idx){
    
    nodeParticleIdx = idx;
    string iconStr = "";
    
    CCDictionary* dict = dynamic_cast<CCDictionary*>(m_save10Arr->objectAtIndex(idx));
    int dictType = dict->valueForKey("type")->intValue();
    CCDictionary* rewardsDic = _dict(dict->objectForKey("result"));
    if (dictType == 0) {//RESOURCE
        int resType = rewardsDic->valueForKey("type")->intValue();
        iconStr = LotteryController::shared()->getIcon(resType);
    }else if (dictType == 1){//BOX_TIMES
        iconStr = LotteryController::shared()->getIcon(100);
    }else if (dictType == 2){//REWARD
        int resType = rewardsDic->valueForKey("type")->intValue();
        if (resType == 7) {
            CCDictionary* valueDic = _dict(rewardsDic->objectForKey("value"));
            int itemId = valueDic->valueForKey("itemId")->intValue();
            iconStr = CCCommonUtils::getIcon(CC_ITOA(itemId));
        }else{
            iconStr = LotteryController::shared()->getIcon(resType);
        }
    }
    
    auto rewardIcon =CCLoadSprite::createSprite(iconStr.c_str());
    CCCommonUtils::setSpriteMaxSize(rewardIcon, 80);
    if (nodeParticleIdx<5) {
        rewardIcon->setPosition((100*nodeParticleIdx)-200, 45);
    }else{
        rewardIcon->setPosition((100*(nodeParticleIdx-5))-200, -45);
    }
    rewardIcon->setOpacity(0.5);
    CCActionInterval *fadeIn = CCFadeIn::create(0.3);
    rewardIcon->setScale(0.8);
    CCActionInterval *scale1 = CCScaleTo::create(0.1, 1.2);
    CCActionInterval *scale2 = CCScaleTo::create(0.1, 1.0);
    //粒子特效
    CCNode *pNode = CCNode::create();
    pNode->setPosition(rewardIcon->getPosition());
    m_rewardsContainer->addChild(pNode);
    auto func =CCCallFuncO::create(this, callfuncO_selector(LotteryRotateView::addRewardsNodeParticle),pNode);
    rewardIcon->runAction(CCSequence::create(fadeIn,scale1,scale2,func, NULL));
    
    m_rewardsContainer->addChild(rewardIcon);
    
//    string m_itemId="200380";
//    int id = atoi(m_itemId.c_str());
//    CCCommonUtils::flyHintWithDefault("Lottery_wood.png", "", _lang_1("104913", LotteryController::shared()->getLotteryName(id, 100).c_str()),0.5);
    //飘字特效
//    ccColor3B textColor = TEXT_COLOR_YELLOW;
//    CCCommonUtils::flyUiResText("Lottery_wood.png", m_rewardsContainer,ccp(0, 0) ,textColor, floating_type_ui1, 23);
}

//粒子特效
void LotteryRotateView::addRewardsNodeParticle(cocos2d::CCObject *obj){
    auto pNode = dynamic_cast<CCNode*>(obj);
    for (int i=1; i<=3; i++) {
        string pName = "UIGlow_";
        pName+=CC_ITOA(i);
        auto particle = ParticleController::createParticle(pName);
        pNode->addChild(particle);
    }
}
#pragma mark 显示10连抽列表
void LotteryRotateView::addShowItemListView(){
    if(LotteryController::shared()->rotateType==1){
        //显示10个奖品
        CCArray *arr = CCArray::create();
        for (int idx=0; idx<10; idx++) {
            CCDictionary *addDict = CCDictionary::create();
            string iconStr = "";// getIcon(m_itemId);
            string desStr = "";
            int count=0;
            
            CCDictionary* dict = dynamic_cast<CCDictionary*>(m_save10Arr->objectAtIndex(idx));
            int dictType = dict->valueForKey("type")->intValue();
            CCDictionary* rewardsDic = _dict(dict->objectForKey("result"));
            if (dictType == 0) {//RESOURCE
                int resType = rewardsDic->valueForKey("type")->intValue();
                iconStr = LotteryController::shared()->getIcon(resType);
                desStr = CCCommonUtils::getResourceNameByType(resType);
                count = rewardsDic->valueForKey("value")->intValue();
            }else if (dictType == 1){//BOX_TIMES
                iconStr = LotteryController::shared()->getIcon(100);
                desStr = _lang("111107");
                count = rewardsDic->valueForKey("value")->intValue();
            }else if (dictType == 2){//REWARD
                int resType = rewardsDic->valueForKey("type")->intValue();
                if (resType == R_GOODS) {
                    CCDictionary* valueDic = _dict(rewardsDic->objectForKey("value"));
                    int itemId = valueDic->valueForKey("itemId")->intValue();
                    iconStr = CCCommonUtils::getIcon(CC_ITOA(itemId));
                    desStr = CCCommonUtils::getNameById(CC_ITOA(itemId));
                    count = 1;
                }else{
                    iconStr = LotteryController::shared()->getIcon(resType);
                    if (resType == R_CHIP) {
                        desStr = _lang("111101");//111101=铜币
                    }else if (resType == R_DIAMOND) {
                        desStr = _lang("111102");//111102=龙币
                    }
                    count = rewardsDic->valueForKey("value")->intValue();
                }
            }
            
            
            addDict->setObject(CCString::create(iconStr), "iconStr");
            addDict->setObject(CCString::create(desStr), "desStr");
            addDict->setObject(CCInteger::create(count), "count");
            arr->addObject(addDict);
        }
        
//        PopupViewController::getInstance()->addPopupView(ShowItemListView::create(arr),false,false);
        ShowItemListView *view = ShowItemListView::create(arr);
        view->delegate = this;
        view->setTag(100001);
        view->setPosition(-320,-900);
        m_rewardsContainer->addChild(view);
    }
}

#pragma mark ShowItemListViewDelegate
void LotteryRotateView::onDelegateClose(){
    CCLOGFUNC();
    if(m_rewardsContainer->getChildByTag(100001)){
        m_rewardsContainer->getChildByTag(100001)->removeFromParentAndCleanup(true);
    }
}

void LotteryRotateView::onTouchEnded(CCTouch* touch, CCEvent* event)
{
    if (!this->isVisible())
    {
        return;
    }

    if (!m_enabled) {
        return;
    }
    
    long endTouchTime = clock() / 1000;
    
    if (m_pTouches->containsObject(touch))
    {
        if (m_pTouches->count() == 1 && m_bTouchMoved)
        {
        }
        m_pTouches->removeObject(touch);
    }

    if (m_pTouches->count() == 0)
    {
        m_bDragging = false;
        m_bTouchMoved = false;
    }

    CCPoint startPoint = m_pContainer->convertToNodeSpace(touch->getStartLocation());
//    CCPoint lastPoint = m_pContainer->convertToNodeSpace(touch->getPreviousLocation());
    CCPoint nowPoint = m_pContainer->convertToNodeSpace(touch->getLocation());
    CCPoint startLine = ccpSub(startPoint, ccp(0, 0));
    CCPoint nowLine = ccpSub(nowPoint, ccp(0, 0));
    
    float angle = CC_RADIANS_TO_DEGREES(ccpSub(nowLine, startPoint).getAngle());
    if (abs(angle) >= 60 && abs(angle) <= 120)
    {
        return;
    }
    
    float degreeGap = CC_RADIANS_TO_DEGREES(nowLine.getAngle(startLine));
    CCLOG("%f:", degreeGap);
    if (fabs(degreeGap) < 15)
    {
        m_rewardsContainer->removeAllChildren();
        return;
    }

    int spendTime = (int)(endTouchTime - m_lastTouchMoveTime) + 1;
    float speed = 100*fabs(m_tScrollAngel)/spendTime;
    m_speed3 = (fabs(degreeGap)) / ((endTouchTime - m_startTouchTime) / 1000.0f);

    CCLOG("speed:%f,speed3:%f",speed, m_speed3);
    if (speed < 6 || m_speed3 < 60.0f) {
        return;
    }
//    m_speed3 = m_speed3 * 2.0f;
    
    bool guide = CCUserDefault::sharedUserDefault()->getBoolForKey("LOTTERYGUIDE");
    if (!guide)
    {
        CCUserDefault::sharedUserDefault()->setBoolForKey("LOTTERYGUIDE", true);
        CCUserDefault::sharedUserDefault()->flush();
        CCDictionary* dict = CCDictionary::create();
        dict->setObject(ccs("HIDEGUIDENODE"), "key");
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(LOTTERYROTATEVIEW, dict);
    }
    
//    if (!isChipEnough())
//    {
//        return;
//    }
    SoundController::sharedSound()->playEffects(Music_Sfx_UI_roulette);
    playAnimation(0);
}

#pragma mark rotatingHandler
void LotteryRotateView::rotatingHandler(float dt){
    m_ftimeGap4 += dt;
//    CCLOG("m_ftimeGap4:%f", m_ftimeGap4);
    float angleGap = 10.0f;
    if (m_hasGetReward)
    {
        angleGap = fabs(m_fEndAngle) * (((100.0f-m_fTimeIdx3*(100.0f/m_fTimeCnt3))*(100.0f-m_fTimeIdx3*(100.0f/m_fTimeCnt3)) - (100.0f-(m_fTimeIdx3+1)*(100.0f/m_fTimeCnt3))*(100.0f-(m_fTimeIdx3+1)*(100.0f/m_fTimeCnt3)))/(100.0f*100.0f));
    }
    
    float rotateToAngle = m_rotateAngle;
    float radians = CC_DEGREES_TO_RADIANS(angleGap);
    
//    CCLOG("cctouchEND:%f, m_fEndTime3:%f, m_fTimeCnt3:%d", m_fEndAngle, m_fEndTime3, m_fTimeCnt3);
//    CCLOG("rotatingHandler-angleGap:%f, rotateToAngle:%f, m_fEndAngle:%f,radians:%f", angleGap, m_rotateAngle, m_fEndAngle,radians);
    if (m_tScrollAngel > 0) {
        
        if (rotateToAngle >= m_fEndAngle && m_hasGetReward) {
            rotateFinished();
            return;
        }
    }
    else {
        angleGap = -angleGap;
        radians = CC_DEGREES_TO_RADIANS(angleGap);
        if (rotateToAngle <= m_fEndAngle && m_hasGetReward) {
            rotateFinished();
            return;
        }
    }
    
    if (m_ftimeGap4 > 2.2f && !m_isSendNotify0)
    {
        m_isSendNotify0 = true;
        CCDictionary* dict = CCDictionary::create();
        dict->setObject(ccs("REMOVEPARTICLE"), "key");
        dict->setObject(ccs("1"), "value");
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(LOTTERYROTATEVIEW, dict);
        
        m_startAddCnt2 = m_fTimeCnt3-m_fTimeIdx3;
    }
    if (abs(m_rotateAngle*2.0) > abs(m_fEndAngle) && !m_isSendNotify1)
    {
        m_isSendNotify1 = true;
        CCDictionary* dict = CCDictionary::create();
        dict->setObject(ccs("REMOVEPARTICLE"), "key");
        dict->setObject(ccs("0"), "value");
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(LOTTERYROTATEVIEW, dict);
        
        m_startAddCnt1 = m_fTimeCnt3-m_fTimeIdx3;
    }
    int startAddCnt;
    startAddCnt=(m_startAddCnt1>m_startAddCnt2?m_startAddCnt1:m_startAddCnt2);
    
    setRotationAngle(radians);
    m_rotateAngle += angleGap;
    
    if (m_fTimeCnt3 != 0 && startAddCnt > 0 && LotteryController::shared()->rotateType==1 && m_fTimeIdx3 >= (m_fTimeCnt3 - startAddCnt)) {//0-普通，1-连转10次
        for (int i=0; i<10; i++) {
            int idx = (m_fTimeCnt3 - startAddCnt)+(startAddCnt*(0.1*i));
            if(m_fTimeIdx3==idx){
                if(i==0){
                    addRewardsNodeBg();
                }
                addRewardsNodeByIndex(i);
            }
        }
    }
    m_fTimeIdx3 = m_fTimeIdx3 + 1;
    if (m_fTimeIdx3 >= m_fTimeCnt3 && m_hasGetReward)
    {
        rotateFinished();
        addShowItemListView();
        return;
    }
}

void LotteryRotateView::onTouchCancelled(CCTouch* touch, CCEvent* event)
{
    if (!this->isVisible())
    {
        return;
    }
    m_pTouches->removeObject(touch);
    if (m_pTouches->count() == 0)
    {
        m_bDragging = false;
        m_bTouchMoved = false;
    }
}

void LotteryRotateView::doCellSort()
{
    LotteryActCell* cell = NULL;
    for (int i = 0; i < m_pContainer->getChildrenCount(); i++)
    {
        for (int j = 0; j < m_pContainer->getChildrenCount(); j++)
        {
            cell  = dynamic_cast<LotteryActCell*>(m_pContainer->getChildren().at(i));
            if (cell)
            {
                
            }
        }
    }
    
    m_depth = 0;
    map<LotteryActCell*, char> visited;
    map<LotteryActCell*, LotteryActCell*> objsMap;
    map<LotteryActCell*, vector<LotteryActCell*> > depObjs;
    
    
    auto &children = m_pContainer->getChildren();
    int max = children.size();
    
    LotteryActCell* objA;
    LotteryActCell* objB;
    
    for (int i = 0; i < max; ++i)
    {
        vector<LotteryActCell*> behind;
        
        objA = dynamic_cast<LotteryActCell*>(children.at(i));
        if( objA == NULL )
            continue;
        
        for (int j = 0; j < max; ++j)
        {
            if(i == j)
            {
                continue;
            }
            objB = dynamic_cast<LotteryActCell*>(children.at(j));
            if( objB == NULL )
                continue;
            
            if (objB->getPositionY() > objA->getPositionY())
            {
                behind.insert(behind.begin(),objB);
            }
        }
        
        objsMap.insert(objsMap.begin(),make_pair(objA, objA));
        depObjs.insert(depObjs.begin(),make_pair(objA, behind));
        
    }
    
    m_depth = 0;
    
    LotteryActCell* obj;
    for (int i = 0; i < max; ++i){
        obj = dynamic_cast<LotteryActCell*>(children.at(i));
        if( obj == NULL )
            continue;
        
        map<LotteryActCell*,char>::iterator it = visited.find(obj);
        if(it == visited.end()){
            place(obj, visited, objsMap, depObjs);
        }
    }
}

void LotteryRotateView::place(LotteryActCell* obj, map<LotteryActCell*, char> & visited,map<LotteryActCell*, LotteryActCell*> & objsMap,map<LotteryActCell*, vector<LotteryActCell*> > & deps)
{
    visited.insert(visited.begin(),make_pair(obj, '1'));
    vector<LotteryActCell*> dep = deps[obj];
    for (vector<LotteryActCell*>::iterator it = dep.begin(); it != dep.end(); ++it) {
        LotteryActCell* innerObj = *it;
        map<LotteryActCell*,char>::iterator itv = visited.find(innerObj);
        if(itv == visited.end()){
            place(innerObj, visited, objsMap, deps);
        }
    }
    if (m_depth != obj->getZOrder())
    {
        obj->setZOrder(m_depth);
        obj->setOrderOfArrival(m_depth);
    }
    ++m_depth;
}

CCRect LotteryRotateView::getViewRect()
{
    CCSize size = getPanelChild()->getContentSize();
    CCPoint screenPos = this->convertToWorldSpace(CCPointZero);
    
    float scaleX = this->getScaleX();
    float scaleY = this->getScaleY();
    
    for (CCNode *p = _parent; p != NULL; p = p->getParent()) {
        scaleX *= p->getScaleX();
        scaleY *= p->getScaleY();
    }
    
    if(scaleX<0.f) {
        screenPos.x += size.width*scaleX;
        scaleX = -scaleX;
    }
    
    if(scaleY<0.f) {
        screenPos.y += size.height*scaleY;
        scaleY = -scaleY;
    }

    return CCRectMake(getPanelChild()->getPositionX() - size.width/2, getPanelChild()->getPositionY() - size.height/2, size.width, size.height);
}

//void LotteryRotateView::draw()
//{
//    CCLayer::draw();
//    
//    CCRect rect = getViewRect();
//        ccDrawColor4B(255, 0, 0, 255);
////        ccDrawRect(ccp(0,0), ccp(150, 150));
//    ccDrawRect(ccp(rect.origin.x,rect.origin.y), ccp(rect.size.width, rect.size.height));
//        ccDrawColor4B(255, 255, 255, 255);
//}

void LotteryRotateView::setContentOffsetForMove()
{
    currRotationAngle = currRotationAngle + m_tScrollAngel;
    float angle = currRotationAngle;
    if (angle >= 360) {
        angle = angle - 360;
    }
    else if(angle <= -360){
        angle = angle + 360;
    }
    
    m_rotateAngle += angle;
    if (m_rotateAngle>0)
    {
        m_rotateAngle = fmodf(m_rotateAngle, 360.0f);
    }
    else
    {
        m_rotateAngle = fmodf(m_rotateAngle, -360.0f);
    }

}

#pragma mark rotateFinished
void LotteryRotateView::rotateFinished()
{
    if (m_rotating == false || m_targetCell == NULL)
    {
        m_rotating = false;
        this->unschedule(schedule_selector(LotteryRotateView::rotatingHandler));
        return;
    }
    this->unschedule(schedule_selector(LotteryRotateView::rotatingHandler));
    m_rotating = false;
    m_rotateAngle = m_fEndAngle;
    if (m_rotateAngle>0)
    {
        m_rotateAngle = fmodf(m_rotateAngle, 360.0f);
    }
    else
    {
        m_rotateAngle = fmodf(m_rotateAngle, -360.0f);
    }
    CCDictionary* dict = CCDictionary::create();
    dict->setObject(ccs(LOTTERYACTVIEW_COM), "key");
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(LOTTERYROTATEVIEW, dict);
}

void LotteryRotateView::setRotationAngle(float angle){
    LotteryActCell* cell = NULL;
    for (int i = 0; i < m_pContainer->getChildrenCount(); i++)
    {
        cell = dynamic_cast<LotteryActCell*>(m_pContainer->getChildren().at(i));
        if (cell)
        {
            cell->setAngle(cell->getAngle() + angle);
        }
    }
    doCellSort();
}

const char* LotteryRotateView::getRotateData() const{
    return "";
}
void LotteryRotateView::setRotateData(map<int, vector<string>> rewardMap, bool anim){
    m_rotatedata.clear();
    m_rotatedata = rewardMap;
    
    if (anim) {
        addAnimationRotateCells();
        return;
    }
    
    if (m_rotating) {
        this->unschedule(schedule_selector(LotteryRotateView::rotatingHandler));
        m_rotating = false;
    }
    
    map<int, vector<string>>::iterator it = m_rotatedata.begin();
    int i = 0;
    for (; it!=m_rotatedata.end(); it++)
    {
        _addRotateCell(i, it->second.at(0).c_str());
        i++;
    }
    doCellSort();
    
}

void LotteryRotateView::addAnimationRotateCells(){
    if (m_AnimCellAdding) {
        m_AnimCellAdding = false;
        this->unschedule(schedule_selector(LotteryRotateView::_addAnimationRotateCell));
    }
    
    m_AnimCellAdding = true;
    
    if (m_rotating) {
        this->unschedule(schedule_selector(LotteryRotateView::rotatingHandler));
        m_rotating = false;
    }

    _clearRotateCells();
    
//    float oldRotate = getPanelChild()->getRotationX();
//    setRotationAngle(0);
//    StringArray tArr;
//    CommUtil::splitStr(m_rotatedata, ",", tArr);
    
//    m_index = atoi(tArr[12].c_str());
    
    m_AnimCellIndex = 0;
    this->schedule(schedule_selector(LotteryRotateView::_addAnimationRotateCell), 1.0f/12.0f);
}

void LotteryRotateView::_addAnimationRotateCell(float dt){
    if (m_AnimCellIndex > 11) {
        m_AnimCellAdding = false;
        this->unschedule(schedule_selector(LotteryRotateView::_addAnimationRotateCell));
        return;
    }
    
//    StringArray tArr;
//    CommUtil::splitStr(m_rotatedata, ",", tArr);
//    _addRotateCell(m_AnimCellIndex, tArr[m_AnimCellIndex].c_str());
    m_AnimCellIndex = m_AnimCellIndex + 1;
}



//class LotteryActCell

LotteryActCell* LotteryActCell::create(string itemId)
{
    LotteryActCell* ret = new LotteryActCell;
    if (ret && ret->init(itemId))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool LotteryActCell::init(string itemId)
{
    bool ret = false;
    do {
        auto ccb = CCBLoadFile("LotteryActCell", this, this);
        this->setContentSize(ccb->getContentSize());
        this->setAnchorPoint(ccp(0.5, 0));
        m_itemId = itemId;
        setIconDisplayFrame();
        ret = true;
    } while (0);
    return ret;
}

void LotteryActCell::onEnter()
{
    CCNode::onEnter();
}

void LotteryActCell::onExit()
{
    CCNode::onExit();
}

void LotteryActCell::setAngle(float radiansAngle)
{
    float x = R_A*cos(radiansAngle);
    float y = R_B*sin(radiansAngle);
    if (CCCommonUtils::isIosAndroidPad())
    {
//        x = R_A_HD*cos(radiansAngle);
//        y = R_B_HD*sin(radiansAngle);
    }
//    CCLOG("_addRotateCell_x:%f,y:%f",x,y);
    this->setPosition(ccp(x, y));
    float DegreeAngle = CC_RADIANS_TO_DEGREES(radiansAngle);
    float scale = 0.6-0.1*abs(sin(CC_DEGREES_TO_RADIANS((DegreeAngle+90)/2)));
//    CCLOG("scale:%f",scale);
    this->setScale(scale);
    m_radians = CC_DEGREES_TO_RADIANS(DegreeAngle);
    setBottomBgFrame();
}

void LotteryActCell::setBottomBgFrame()
{
    float degree = int(CC_RADIANS_TO_DEGREES(m_radians))%360;
//    CCLOG("degree0:%f", degree);
    if (degree > 90 && degree <= 180)
    {
        degree -= 90;
    }
    else if (degree < 360 && degree > 270)
    {
        degree -= 270;
    }
    else if (degree <= 270 && degree > 180)
    {
        degree -= 180;
    }
    else if (degree < 0 && degree >= -90)
    {
        degree += 90;
    }
    else if (degree < -90 && degree >= -180)
    {
        degree += 180;
    }
    else if (degree < -180 && degree >= -270)
    {
        degree += 270;
    }
    else if (degree < -270 && degree >= -360)
    {
        degree += 360;
    }
    int index = degree/2;
    index = index==0?index=1:index;
//    CCLOG("m_radians:%f,degree:%f,index:%d", m_radians, degree, index);
    string name = CCString::createWithFormat("LuckDraw_%d.png",index)->getCString();
    m_bottomBg->setDisplayFrame(CCLoadSprite::loadResource(name.c_str()));
}

string LotteryActCell::getIcon(int type/*=-1*/)
{
    int swith = atoi(m_itemId.c_str());
    if (type != -1) {
        swith = type;
    }
    std::string fix = ".png";
    switch (swith)
    {
        case Silver:
            return "Lottery_Mithril" + fix;
        case Wood:
            return "Lottery_wood" + fix;
        case Iron:
            return "Lottery_stone" + fix;
        case Food:
            return "Lottery_food" + fix;
        case Stone:
            return "Lottery_Mithril" + fix;
        case Gold:
            return "Lottery_gold" + fix;
        case R_CHIP:
            return "Lottery_bargaining-chip1" + fix;
        case R_DIAMOND:
            return "Lottery_bargaining-chip2" + fix;
        case 100:
            return "Lottery_baoxiang.png";
        case 200200:
            return "Lottery_speedUp" + fix;
        case 200380:
            return "Lottery_energy" + fix;
        default:
            return "Lottery_baoxiang.png";
    }
    return std::string("");
}

void LotteryActCell::setIconDisplayFrame()
{
    m_icon->setDisplayFrame(CCLoadSprite::loadResource(getIcon().c_str()));
}

bool LotteryActCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon", CCSprite*, m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomBg", CCSprite*, m_bottomBg);
    return false;
}