//
//  DailyActiveView.cpp
//  IF
//
//  Created by chenfubi on 15/4/10.
//
//

#include "DailyActiveView.h"
#include "DailyActiveController.h"
#include "TipsView.h"
#include "PopupViewController.h"
#include "DailyActiveBoxInfoView.h"
#include "IFDailyActiveNode.h"
#include "ParticleController.h"
#include "FunBuildController.h"
#include "DynamicResourceController.h"
#include "GuideController.h"
#include "AchievementNewPopUpView.h"

#define OFFSETX 30

static string GuideKey = "";

DailyActiveView* DailyActiveView::create()
{
	DailyActiveView* ret = new DailyActiveView();
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DailyActiveView::init()
{
	bool bRet = false;
	do
	{
        if (!PopupBaseView::init())
        {
            break;
        }
                   

        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(502, true);
        CCLoadSprite::doResourceByCommonIndex(101, true);
        if (DynamicResourceController::getInstance()->checkGoodsResource())
        {
            DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_GOODS_TEXTURE,false);
        }
        setCleanFunction([](){
            auto pop = dynamic_cast<AchievementNewPopUpView*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(!pop){
                CCLoadSprite::doResourceByCommonIndex(502, false);
                CCLoadSprite::doResourceByCommonIndex(101, false);
                DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_GOODS_TEXTURE,true);
            }
           
        });
        
        
        m_rewardWidth = 0;
        auto ccb = CCBLoadFile("DailyactiveView", this, this);
        this->setContentSize(ccb->getContentSize());
        if(m_topdizi)
        {
            CCCommonUtils::addNewDizi(m_topdizi);
        }
        int extH = getExtendHeight();
        m_listNode->setContentSize(CCSizeMake(m_listNode->getContentSize().width, m_listNode->getContentSize().height + extH));
        
        auto orgSize = CCDirector::sharedDirector()->getVisibleSize();
        CCLayerColor *color = CCLayerColor::create(ccc4(0, 0, 0, 255),orgSize.width,orgSize.height);
        this->addChild(color);
        color->setZOrder(-10);
        color->setPositionY(ccb->getPositionY() - 340);
        
        onGameTick(0);
        
        m_titleTTF1->setString(_lang("133103"));
        m_numTTF0->setString(_lang("133105"));
        m_numTTF1->setString(CC_ITOA(DailyActiveController::shared()->getActivePoints()));
        m_numTTF1->setPositionX(m_numTTF0->getPositionX()+m_numTTF0->getContentSize().width*m_numTTF0->getOriginScaleX() + 10);

        m_data = CCArray::create();
        
        m_tableView = CCMultiColTableView::create(this, m_listNode->getContentSize());
        m_tableView->setDirection(kCCScrollViewDirectionVertical);
        m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tableView->setMultiColTableViewDelegate(this);
        m_tableView->setTouchPriority(Touch_Default);
        m_listNode->addChild(m_tableView);
        
        m_scrollView = CCScrollView::create(m_scrollNode->getContentSize()+CCSize(0, 100));
        m_scrollView->setDirection(kCCScrollViewDirectionHorizontal);
        m_scrollView->setTouchPriority(Touch_Default);
        m_scrollView->setAnchorPoint(ccp(0, 0));
        m_scrollView->setDelegate(this);
        m_scrollNode->addChild(m_scrollView);
        
        m_flyArrow = CCSprite::create();
        auto per1Spr = CCSprite::create();
        CCCommonUtils::makeEffectSpr(per1Spr, "guide_hand_%d.png", 4, 0.1);
        per1Spr->setAnchorPoint(Vec2(0.5,1));
       // per1Spr->setPosition(ccp(-100,-100))
        per1Spr->setRotation(45);
        m_flyArrow->addChild(per1Spr);
        m_flyArrow->setVisible(false);
        m_flyArrow->setName("m_flyArrow");
        m_handNode->addChild(m_flyArrow);
        
        m_headNode2 = CCNode::create();
        CCSprite* sp = CCLoadSprite::createSprite("daily_quest_shape.png");
        if (CCCommonUtils::isIosAndroidPad()) {
            sp->setScale(2);
            sp->setPositionY(50.0f);
        }
        else
            sp->setPositionY(30.0f);
        m_headNode2->addChild(sp, 1);
        CCLabelIFBMFont* labelBm = CCLabelIFBMFont::create("", "Arial_Bold_Border.fnt");
        labelBm->setDimensions(CCSize(0,30));
        labelBm->setColor({255,255,255});
        labelBm->setAnchorPoint(ccp(0.5,0.5));
        labelBm->setTag(2);
        if (CCCommonUtils::isIosAndroidPad()) {
            labelBm->setScale(2);
        }
        m_headNode2->addChild(labelBm, 2);
        
        if (DailyActiveController::shared()->getHasInit())
        {
            reloadData(NULL);
        }
        else
        {
            m_waitInterface = GameController::getInstance()->showWaitInterface(m_listNode);
            m_waitInterface->setAnchorPoint(ccp(0.5, 0.5));
            m_waitInterface->setPosition(ccp(m_listNode->getContentSize().width/2, -m_listNode->getContentSize().height/2));
            DailyActiveController::shared()->getDataFormServer();
        }
        
        bRet = true;
        
        auto& build = FunBuildController::getInstance()->getFunbuildById(FUN_BUILD_MAIN_CITY_ID);
        if (build.level <= 6) {
            if (CCCommonUtils::isTestPlatformAndServer("Guide_DailyActive") && DailyActiveController::shared()->getHaveGetBoxNum()==0) {
                GuideController::share()->checkSubGuide("3190100");
            }
        }
        
        
        CCCommonUtils::setButtonTitle(m_DailyBtn, _lang("133104").c_str());
        
        CCCommonUtils::setButtonTitle(m_TaskBtn, _lang("107500").c_str());
        updateBtnType(0);
        
	}while(0);
	return bRet;
}

CCNode* DailyActiveView::getGuideNode(string _key)
{
    if (_key == "DailyActive_guide1" && m_guide1Node) {
        GuideKey = _key;
        return m_guide1Node;
    }else if (_key == "DailyActive_guide2" && m_listNode) {
        GuideKey = _key;
        return m_listNode;
    }
    return NULL;
}

void DailyActiveView::reloadData(cocos2d::CCObject *pObj)
{
    if (m_waitInterface != nullptr)
    {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    m_isBack = false;
    m_pointArray = CCPointArray::create(0);
    
    m_data = DailyActiveController::shared()->getSortItemArr();
    reInitBoxInfo(NULL);
    m_tableView->setTouchEnabled(true);
    m_tableView->reloadData();
    if (m_pointArray->count() <= 0)
    {
        scheduleUpdate();
    }
}

void DailyActiveView::reInitBoxInfo(cocos2d::CCObject *pObj)
{
//    加进度条头上的光效
    addScrollChild();
    m_headNode->removeAllChildren();
//    m_headNode->setPosition(ccp(-280.0f, 0.0f));
    ccColor4F beginColor = {0, 0, 0, 0};
    ccColor4F endColor = {0, 0, 0, 0};
    for (int i=1; i<=3; i++)
    {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("Loading_%d",i)->getCString());
        switch (i)
        {
            case 1:
                beginColor = {255/255.0f, 200/255.0f, 100/255.0f, 100/255.0f};
                break;
            case 2:
                beginColor = {255/255.0f, 200/255.0f, 0/255.0f, 100/255.0f};
                break;
            case 3:
                beginColor = {255/255.0f, 255/255.0f, 255/255.0f, 100/255.0f};
                break;
            default:
                break;
        }
        particle->setStartColor(beginColor);
        particle->setEndColor(endColor);
        m_headNode->addChild(particle);
    }
    
    m_hasGoWidth = 0;
    int totalScore = DailyActiveController::shared()->getTotalPoint();
    int nowScore = DailyActiveController::shared()->getPreActPoint();
    nowScore = nowScore>totalScore?totalScore:nowScore;
    m_hasGoWidth = getGoWidth(nowScore);
    m_progress->setPreferredSize(CCSizeMake(OFFSETX+m_hasGoWidth, m_progress->getContentSize().height));
    
    LabelBMFont* label = (LabelBMFont*)m_headNode2->getChildByTag(2);
    label->setString(CC_ITOA(nowScore));
    
    nowScore = DailyActiveController::shared()->getActivePoints();
    DailyActiveController::shared()->setPreActPoint(DailyActiveController::shared()->getActivePoints());
    nowScore = nowScore>totalScore?totalScore:nowScore;
    m_needGoWidth = getGoWidth(nowScore);
    
    float maxdx = m_scrollView->maxContainerOffset().x;
    
    if (m_hasGoWidth > 560)
    {
        m_scrollView->setContentOffset(ccp(560-m_hasGoWidth-115, 0));
    }
    
    if (m_rewardWidth > 0) {
        int offsetX = -m_rewardWidth + defaultWidth+50;
        offsetX = offsetX<0?offsetX:0;
        m_scrollView->setContentOffset(ccp(offsetX, 0));
        onShowHand(m_rewardWidth + offsetX);
    }
    
    float x = m_progress->getPreferredSize().width-OFFSETX;
    m_headNode->setPositionX(x);
    m_headNode2->setPositionX(x);
    
    unscheduleUpdate();
    m_scrollView->setTouchEnabled(false);
}

float DailyActiveView::getGoWidth(int nowScore)
{
    float goWidth = 0.0f;
    
    map<int, DailyActiveBoxInfo> boxMap = DailyActiveController::shared()->m_boxInfo;
    map<int, DailyActiveBoxInfo>::iterator l_it = boxMap.begin();
    int i = 0;
    int perKey = 0;
    float maxPosX = 0.0f;
    float minPosX = 0.0f;
    float maxPoint = 0;
    for (; l_it != boxMap.end(); l_it++)
    {
        if (nowScore <= l_it->second.point)
        {
            maxPoint = l_it->second.point;
            maxPosX = m_posMap[l_it->first];
            break;
        }
        perKey = l_it->first;
    }
    minPosX = perKey==0?0.0f:m_posMap[perKey];
    int minPoint = perKey==0?0:boxMap[perKey].point;
    float offSexX = nowScore==0?0.0f:44.0f;
    goWidth = ((nowScore-minPoint)/(maxPoint-minPoint))*(maxPosX-minPosX)+minPosX+offSexX;
    return goWidth;
}

void DailyActiveView::addScrollChild()
{
    m_posMap.clear();
    m_progress->removeFromParent();
    if (CCCommonUtils::isIosAndroidPad()) {
        m_progress->setPosition(ccp(-OFFSETX, 88));
    }
    else
        m_progress->setPosition(ccp(-OFFSETX, 44));
    m_headNode->removeFromParent();
    if (CCCommonUtils::isIosAndroidPad()) {
        m_headNode2->setPosition(ccp(0, 100));
        m_headNode->setPosition(ccp(0, 100));
    }
    else {
        m_headNode2->setPosition(ccp(0, 55));
        m_headNode->setPosition(ccp(0, 55));
    }
    m_scrollView->addChild(m_progress);
    m_scrollView->addChild(m_headNode);
    DailyActiveBoxInfoCell* cell0 = DailyActiveBoxInfoCell::create("");
    cell0->setPosition(-23, 15);
  //  m_scrollView->addChild(cell0);
    
    defaultWidth = 112;
    if(CCCommonUtils::isIosAndroidPad()){
        defaultWidth = 224;
    }
    
    map<int, DailyActiveBoxInfo> boxMap = DailyActiveController::shared()->m_boxInfo;
    map<int, DailyActiveBoxInfo>::iterator l_it = boxMap.begin();
    int i = 0;
    float posX = 0.0f;
    float progressWidth = 0.0f;
    float rewardPro = 0.0f;
    bool isHaveReward = false;
    for (; l_it != boxMap.end(); l_it++)
    {
        int sprType = 0;
        if (FunBuildController::getInstance()->getMainCityLv() >= 20) {
            if (i == 3) {
                sprType = 1;
            }
            else if (i == 7) {
                sprType = 2;
            }
        }
        DailyActiveBoxInfoCell* cell = DailyActiveBoxInfoCell::create(CC_ITOA(l_it->first),sprType);
        cell->setTag(i+1);
        m_scrollView->addChild(cell);
        posX = defaultWidth*i + 70;
        if(CCCommonUtils::isIosAndroidPad())
        {
            posX = defaultWidth*i + 140;
        }
        
        cell->setPositionX(posX);
        m_posMap[l_it->first] = posX;
        progressWidth+=defaultWidth;
        i++;
        
        if (l_it->second.state == BOX_COMPLETE || l_it->second.state == BOX_GET_REWARD)
        {
            if (!isHaveReward) {
                rewardPro += defaultWidth;
            }
            if (l_it->second.state == BOX_COMPLETE) {
                m_rewardWidth = rewardPro;
                isHaveReward = true;
            }
        }
    }
    
    m_scrollView->addChild(m_headNode2);
    if (CCCommonUtils::isIosAndroidPad()) {
        progressWidth+=230;
    }
    else {
        progressWidth+=115;
    }

    m_scrollView->setContentSize(CCSizeMake(progressWidth, m_scrollView->getContentSize().height));
}

void DailyActiveView::update(float dt)
{
    m_hasGoWidth += dt*60;
    CCLabelIFBMFont* labelIf = (CCLabelIFBMFont*)m_headNode2->getChildByTag(2);
    int score = atoi(labelIf->getString());
    score += 1;
    score = score>DailyActiveController::shared()->getActivePoints()?DailyActiveController::shared()->getActivePoints():score;
    labelIf->setString(CC_ITOA(score));
    if (m_hasGoWidth >= m_needGoWidth)
    {
        labelIf->setString(CC_ITOA(DailyActiveController::shared()->getActivePoints()));
        m_hasGoWidth = m_needGoWidth;
        unscheduleUpdate();
        m_scrollView->setTouchEnabled(true);
    }
    if (m_needGoWidth > 560 && m_hasGoWidth > 280)
    {
        if (m_rewardWidth <= 0) {
            m_scrollView->setContentOffset(ccp(m_scrollView->getContentOffset().x-dt*60, 0));
        }
    }
    float x = m_progress->getPreferredSize().width-OFFSETX;
    m_headNode->setPositionX(x);
    m_headNode2->setPositionX(x);
    m_progress->setPreferredSize(CCSizeMake(m_hasGoWidth+OFFSETX, m_progress->getContentSize().height));
}

void DailyActiveView::onGameTick(float dt)
{
    double timeStamp = GlobalData::shared()->getTimeStamp();
    double tomorrowTime = DailyActiveController::shared()->getTomorrowTime();
    if(tomorrowTime <= timeStamp)
    {
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_listNode);
        m_waitInterface->setAnchorPoint(ccp(0.5, 0.5));
        m_waitInterface->setPosition(ccp(m_listNode->getContentSize().width/2, -m_listNode->getContentSize().height/2));
        DailyActiveController::shared()->resetAllData();
    }
    else
    {
        double lastTime = GlobalData::shared()->tomorrow_time - timeStamp;
        m_titleTTF0->setString(_lang_1("133106", CC_SECTOA(lastTime)));
    }
}

void DailyActiveView::cellCallBack()
{
    if (m_isBack)
    {
        return;
    }
    m_isBack = true;
    for (int i = 0; i < m_pointArray->count(); i++)
    {
        playParticle(m_pointArray->getControlPointAtIndex(i));
    }
    scheduleUpdate();
}

void DailyActiveView::playParticle(CCPoint p)
{
    CCPoint startP = m_particleNode->convertToNodeSpace(p);
    CCPoint endP = m_headNode->getPosition();
    endP.x = OFFSETX+m_hasGoWidth;
    endP = m_headNode->getParent()->convertToWorldSpace(endP);
    endP = m_particleNode->convertToNodeSpace(endP);
    
    string tmpStart = "Collection_expF_";//Collection_Star_ Collection_expF_
    int count = 3;
    for (int i=1; i<=count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        if (i==1) {
            particle->setStartColor(ccc4f(1.0, 0.77, 0, 1.0));
            particle->setEndColor(ccc4f(0, 0, 0, 0));
        }else if (i==2) {
            particle->setStartColor(ccc4f(1.0, 0.55, 0, 1.0));
            particle->setEndColor(ccc4f(1, 0.96, 0.5, 0));
        }else if (i==3) {
            particle->setStartColor(ccc4f(1.0, 0.9, 0, 1.0));
            particle->setEndColor(ccc4f(0, 0, 0, 0));
        }
        
        if (i == 2) {
            particle->setPositionType(kCCPositionTypeFree);
        } else {
            particle->setPositionType(kCCPositionTypeRelative);
        }
        particle->setPosition(startP);
        m_particleNode->addChild(particle);
        
        auto func = CCCallFuncO::create(this, callfuncO_selector(DailyActiveView::particleBack), particle);
        
        CCActionInterval * moveTo = CCMoveTo::create(2.5,endP);
        CCActionInterval * easeExponentialIn= CCEaseExponentialOut::create(moveTo);
        
        CCActionInterval * delT = CCDelayTime::create(0.5);
        CCActionInterval * fadeOut = CCFadeOut::create(0.2);
        
        auto funcPowerUI = nullptr;
        CCActionInterval * seqFadeOut = CCSequence::create(delT, fadeOut, nullptr);
        CCSpawn* spawn = CCSpawn::create(easeExponentialIn,seqFadeOut,nullptr);
        particle->runAction(CCSequence::create(spawn, func, nullptr));
    }
}

void DailyActiveView::particleBack(cocos2d::CCObject *pObj)
{
    auto p = dynamic_cast<CCParticleSystemQuad*>(pObj);
    if (p)
    {
        p->removeFromParent();
    }
    if (m_particleNode->getChildrenCount() == 0)
    {
        map<string, DailyActiveItemInfo> info = DailyActiveController::shared()->m_itemInfo;
        map<string, DailyActiveItemInfo>::iterator l_it = info.begin();
        for (; l_it!=info.end(); l_it++)
        {
            DailyActiveController::shared()->m_itemInfo[l_it->first].isNew = false;
        }
        
//        m_tableView->setTouchEnabled(true);
        m_data = DailyActiveController::shared()->getSortItemArr();
        m_tableView->reloadData();
    }
}

bool DailyActiveView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void DailyActiveView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (GuideKey == "DailyActive_guide1" && m_guide1Node && isTouchInside(m_guide1Node, pTouch)) {
        GuideKey = "";
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::create("DailyActive_guide1"));
        
    }
    else if (GuideKey == "DailyActive_guide2" && m_listNode && isTouchInside(m_listNode, pTouch)) {
        GuideKey = "";
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::create("DailyActive_guide2"));
    }
}

void DailyActiveView::scrollViewDidScroll(CCScrollView *view)
{
    if (view != m_scrollView)
    {
        return;
    }
    float maxdx = m_scrollView->maxContainerOffset().x;
    float dx = m_scrollView->getContentOffset().x;
    if(dx > maxdx)
    {
        m_scrollView->setContentOffset(ccp(maxdx, 0));
    }
}

CCTableViewCell* DailyActiveView::gridAtIndex(CCMultiColTableView *multiTable, ssize_t idx)
{
    if (idx>=m_data->count())
    {
        return NULL;
    }
    DailyActiveCell* cell = (DailyActiveCell*)multiTable->dequeueGrid();
    string itemId = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();
    DailyActiveItemInfo itemInfo = DailyActiveController::shared()->m_itemInfo[itemId];
    if (itemInfo.isNew)
    {
        float x = 208*(idx%3);
        float y = 309*(idx/3);
        CCPoint p = ccpAdd(ccp(-238, -70), ccp(x, -y));
        p = m_subNode->convertToWorldSpace(p);
        m_pointArray->addControlPoint(p);
//        m_tableView->setTouchEnabled(false);
    }
    if (cell == NULL)
    {
        cell = DailyActiveCell::create(itemId);
    }
    else
    {
        cell->removeAllChildrenWithCleanup(true);
        //暂时解决按钮消失问题
        cell = DailyActiveCell::create(itemId);
    }
    if (itemInfo.isNew) {
        DailyActiveController::shared()->m_itemInfo[itemId].isNew = false;
    }
    return cell;
}

ssize_t DailyActiveView::numberOfCellsInTableView(CCMultiColTableView *multiTable)
{
    int cellNum = m_data->count();
    return (cellNum );
}

ssize_t DailyActiveView::numberOfGridsInCell(CCMultiColTableView *multiTable)
{
    return 1;
}

CCSize DailyActiveView::gridSizeForTable(CCMultiColTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSizeMake(1440, 280);
    }
    return CCSizeMake(620, 140);
}

void DailyActiveView::onEnter()
{
    CCNode::onEnter();
    
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    
    this->schedule(schedule_selector(DailyActiveView::onGameTick), 1.0f);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DailyActiveView::reloadData), DAILYQEUST_INIT, NULL);
    setTitleName(_lang("107500"));
}

void DailyActiveView::onExit()
{
    this->unschedule(schedule_selector(DailyActiveView::onGameTick));
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, DAILYQEUST_INIT);
    unscheduleUpdate();
    setTouchEnabled(false);
    CCNode::onExit();
}

bool DailyActiveView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_scrollNode", CCNode*, this->m_scrollNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_headNode", CCNode*, this->m_headNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTTF0", CCLabelIF*, this->m_titleTTF0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTTF1", CCLabelIF*, this->m_titleTTF1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numTTF0", CCLabelIF*, this->m_numTTF0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numTTF1", CCLabelIF*, this->m_numTTF1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_progress", CCScale9Sprite*, this->m_progress);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_topdizi", CCScale9Sprite*, this->m_topdizi);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_particleNode", CCNode*, this->m_particleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_subNode", CCNode*, this->m_subNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_handNode", CCNode*, this->m_handNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_guide1Node", CCNode*, this->m_guide1Node);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_DailyBtn", CCControlButton*, this->m_DailyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_TaskBtn", CCControlButton*, this->m_TaskBtn);

    return false;
}

SEL_CCControlHandler DailyActiveView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickDailyBtn", DailyActiveView::onClickDailyBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickTaskBtn", DailyActiveView::onClickTaskBtn);
    

    return NULL;
}

//任务 两个按钮方法
void DailyActiveView::onClickDailyBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    updateBtnType(0);
}

void DailyActiveView::onClickTaskBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    updateBtnType(1);
}

void DailyActiveView::updateBtnType(int iType) // 0 日常 1 普通
{
    if(iType == 0)
    {
        m_DailyBtn->setEnabled(false);
        m_TaskBtn->setEnabled(true);
    }else
    {
        m_DailyBtn->setEnabled(true);
        m_TaskBtn->setEnabled(false);
        
        //关闭当前页面
        //PopupViewController::getInstance()->removePopupView(this);
        PopupViewController::getInstance()->goBackPopupView();

        //打开
        PopupViewController::getInstance()->addPopupInView(AchievementNewPopUpView::create());
        
    }
}

void DailyActiveView::onShowHand(float posx)
{
    m_flyArrow->setPosition(ccp(posx, 0));
    m_flyArrow->setVisible(true);
    CCActionInterval * moveBy = CCMoveBy::create(0.5,ccp(20, -30));
    CCActionInterval * moveRBy = CCMoveBy::create(0.5,ccp(-20, 30));
    CCSequence* fadeAction = CCSequence::create(moveBy,moveRBy,NULL);
    CCActionInterval * fadeForever =CCRepeatForever::create((CCActionInterval* )fadeAction);
   // m_flyArrow->runAction(fadeForever);
    
    scheduleOnce(schedule_selector(DailyActiveView::hideFlyArrow), 5.0f);
}
void DailyActiveView::hideFlyArrow(float dt)
{
    m_flyArrow->setVisible(false);
    m_flyArrow->stopAllActions();
}


//CLASS DailyActiveCell
DailyActiveCell* DailyActiveCell::create(string itemId)
{
    DailyActiveCell* ret = new DailyActiveCell();
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

bool DailyActiveCell::init(string itemId)
{
    bool ret = false;
    do {
        auto ccb = CCBLoadFile("DailyactiveViewCell", this, this);
        this->setContentSize(ccb->getContentSize());
        setData(itemId);
        ret = true;
    } while (0);
    
    
    return ret;
}

void DailyActiveCell::setData(string itemId)
{
    m_itemId = itemId;
    DailyActiveItemInfo itemInfo = DailyActiveController::shared()->m_itemInfo[m_itemId];
    m_titleTTF->setString(itemInfo.name);
    
    float fh = m_titleTTF->getContentSize().height;
    if (CCCommonUtils::isIosAndroidPad())
    {
        if (fh>110) {//防爆框
            m_titleTTF->setScaleY(110/fh);
        }
    } else {
        if (fh>60) {//防爆框
            m_titleTTF->setScaleY(60/fh);
        }
    }
    
    string levelStr = "+";
    levelStr.append(CC_ITOA(itemInfo.point));
    m_levelTTF->setString(levelStr);
    
    float per = itemInfo.times/(itemInfo.totalTimes*1.0f);
    per = per>1?1:per;
    string num = CCString::createWithFormat("%.0f", per*100)->getCString();
    num.append("%");
    m_numTTF->setString(num);
    m_comTTF->setString(_lang("133109"));

    // progress
    m_progress->setScaleX(per);
    m_progressTxt->setString(num);
    
    m_icon->setDisplayFrame(CCLoadSprite::loadResource(itemInfo.icon.c_str()));

    CCCommonUtils::setSpriteMaxSize(m_icon, 138, true);
    
    m_completeNode->setVisible(false);
    m_lockNode->setVisible(false);
    m_spineNode->setVisible(false);
    m_spineNode->removeAllChildren();
    CCCommonUtils::setSpriteGray(m_bgIcon, false);
    CCCommonUtils::setSpriteGray(m_icon, false);
    CCCommonUtils::setSpriteGray(m_ttfBg, false);
    CCCommonUtils::setSpriteGray(m_dizi_01, false);
    CCCommonUtils::setSpriteGray(m_dizi_02, false);
    m_goBtn->setEnabled(true);
    m_levelTTF->setVisible(true);
    if (itemInfo.unlock || itemInfo.playAni)
    {
        m_lockNode->setVisible(true);
        CCCommonUtils::setSpriteGray(m_bgIcon, true);
        CCCommonUtils::setSpriteGray(m_icon, true);
        CCCommonUtils::setSpriteGray(m_ttfBg, true);
        CCCommonUtils::setSpriteGray(m_dizi_01, true);
        CCCommonUtils::setSpriteGray(m_dizi_02, true);
        m_levelTTF->setVisible(false);
        m_goBtn->setEnabled(false);
    
        //button
       // CCCommonUtils::setButtonSprite(m_goBtn, "Btn_grey.png");

        m_titleTTF->setColor(ccc3(50, 50, 50));
        if (itemInfo.playAni)
        {
            m_lockNode->setVisible(false);
            m_spineNode->setVisible(true);
            m_spineNode->removeAllChildren();
            (&DailyActiveController::shared()->m_itemInfo[m_itemId])->playAni = false;
            IFDailyActiveNode* n = IFDailyActiveNode::create();
            CCCallFunc* cb = CCCallFunc::create(this, callfunc_selector(DailyActiveCell::spineCallBack));
            n->setSpineState("", 0, false, cb);
            n->setAnchorPoint(ccp(0.5f, 0.5f));
            n->setScale(0.85);
//            m_spineNode->setPosition(ccp(-115, 120));
            m_spineNode->addChild(n);
        }
        else
        {
            CCObject* obj = NULL;
            CCSprite* sp = NULL;
            auto& arr = m_lockNode->getChildren();
            for(auto child : arr)
            {
                sp = dynamic_cast<CCSprite*>(child);
                if (sp)
                {
                    CCCommonUtils::setSpriteGray(sp, true);
                }
            }
//            CCARRAY_FOREACH(m_lockNode->getChildren(), obj)
//            {
//                sp = dynamic_cast<CCSprite*>(obj);
//                if (sp)
//                {
//                    CCCommonUtils::setSpriteGray(sp, true);
//                }
//            }
        }
    }
    else if(itemInfo.hasComplete)
    {
        m_goBtn->setVisible(false);
        m_levelTTF->setVisible(false);
        m_completeNode->setVisible(true);
        if (itemInfo.isNew)
        {
            doAniAction();
        }
    }
    
    CCCommonUtils::setButtonTitle(m_goBtn, _lang("108531").c_str());
    if (m_itemId == "5000004")
    {
       // m_goBtn->setVisible(false);
    }
}

void DailyActiveCell::doAniAction()
{
    CCNodeRGBA* rgbaNode = nullptr;
    float originY = 0.0f;
    for (int i = 0; i < m_completeNode->getChildrenCount(); i++)
    {
        rgbaNode = dynamic_cast<CCNodeRGBA*>(m_completeNode->getChildren().at(i));
        if (rgbaNode)
        {
            rgbaNode->setOpacity(0);
            originY = rgbaNode->getPositionY();
            rgbaNode->setPositionY(originY+100);
            CCMoveBy* moveBy = CCMoveBy::create(1.0f, ccp(0, -100));
            CCFadeIn* fadeIn = CCFadeIn::create(1.0f);
            CCSpawn* spa = CCSpawn::create(moveBy, fadeIn, nullptr);
            CCCallFunc* callBack = CCCallFunc::create(this, callfunc_selector(DailyActiveCell::aniComplete));
            CCSequence* seq = CCSequence::create(spa, callBack, nullptr);
            rgbaNode->runAction(seq);
        }
    }
}

void DailyActiveCell::aniComplete()
{
    DailyActiveView* view = dynamic_cast<DailyActiveView*>(PopupViewController::getInstance()->getCurrentPopupView());
    if (view)
    {
        view->cellCallBack();
    }
}

void DailyActiveCell::spineCallBack()
{
    CCCommonUtils::setSpriteGray(m_bgIcon, false);
    CCCommonUtils::setSpriteGray(m_icon, false);
    CCCommonUtils::setSpriteGray(m_ttfBg, false);
    m_lockNode->setVisible(false);
    m_spineNode->setVisible(false);
}

void DailyActiveCell::onGoClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    AutoSafeRef temp(this);
    
    PopupViewController::getInstance()->removeAllPopupView();
    DailyActiveController::shared()->goActTarget(m_itemId);
}

void DailyActiveCell::onEnter()
{
    CCNode::onEnter();
    
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void DailyActiveCell::onExit()
{
    setTouchEnabled(false);
    CCNode::onExit();
}

CCNode *DailyActiveCell::getParentPanel(){
    auto pop = dynamic_cast<DailyActiveView*>(PopupViewController::getInstance()->getCurrentPopupView());
    if(pop){
        return pop->m_listNode;
    }
    return NULL;
}

bool DailyActiveCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    auto node = getParentPanel();
    if(!node){
        return false;
    }
    if (isTouchInside(node, pTouch) && isTouchInside(m_bgIcon, pTouch))
    {
        if (GuideKey == "DailyActive_guide2") {
            return false;
        }
        m_ownNode->setScale(0.9);
        return true;
    }
    return false;
}

void DailyActiveCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    auto node = getParentPanel();
    if(!node){
        return;
    }

    m_ownNode->setScale(1.0f);
    if (ccpDistance(pTouch->getStartLocation(), pTouch->getLocation()) > 10)
    {
        return;
    }
    if (!isTouchInside(node, pTouch))
    {
        return;
    }
    if (isTouchInside(m_bgIcon, pTouch))
    {
        DailyActiveItemInfo info = DailyActiveController::shared()->m_itemInfo[m_itemId];
        if (info.unlock)
        {
            PopupViewController::getInstance()->addPopupView(TipsView::create(_lang_1("133108", CC_ITOA(info.unlockLevel))));
        }
        else
        {
            PopupViewController::getInstance()->addPopupView(DailyActiveItemInfoView::create(m_itemId));
        }
    }
}

bool DailyActiveCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_lockNode", CCNode*, this->m_lockNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_completeNode", CCNode*, this->m_completeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_spineNode", CCNode*, this->m_spineNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_ownNode", CCNode*, this->m_ownNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bgIcon", CCSprite*, this->m_bgIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_icon", CCSprite*, this->m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_ttfBg", CCSprite*, this->m_ttfBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_dizi_02", CCSprite*, this->m_dizi_02);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_dizi_01", CCSprite*, this->m_dizi_01);
    

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTTF", CCLabelIF*, this->m_titleTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numTTF", CCLabelIF*, this->m_numTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_comTTF", CCLabelIF*, this->m_comTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_levelTTF", CCLabelIF*, this->m_levelTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goBtn", CCControlButton*, this->m_goBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progress", CCScale9Sprite*, this->m_progress);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progressTxt", CCLabelIF*, this->m_progressTxt);
    return false;
}

SEL_CCControlHandler DailyActiveCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSelectBtnClick", AchievementNewView::onSelectBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoClick", DailyActiveCell::onGoClick);
    return NULL;
}



//class DailyActiveBoxInfoCell
DailyActiveBoxInfoCell* DailyActiveBoxInfoCell::create(string itemId, int sprType/*0*/)
{
    DailyActiveBoxInfoCell* ret = new DailyActiveBoxInfoCell();
    if (ret && ret->init(itemId,sprType))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DailyActiveBoxInfoCell::init(string itemId, int sprType)
{
    bool ret = false;
    do {
        auto ccb = CCBLoadFile("DailyActiveBoxInfoCell", this, this);
        this->setContentSize(ccb->getContentSize());
        this->setAnchorPoint(ccp(0.5, 0.5));
        m_btnTTF->setVisible(false);
        m_btnTTF->setString(_lang("133107"));
        m_itemId = itemId;
        m_sprType = sprType;
        if (m_itemId.length() == 0)
        {
            m_particleNode->setVisible(false);
            m_rewardSp->setVisible(false);
            m_pointTTF->setString("0");
            m_spr->setVisible(false);
        }
        else
        {
            DailyActiveBoxInfo info = DailyActiveController::shared()->m_boxInfo[atoi(m_itemId.c_str())];
            m_particleNode->setVisible(true);
            m_pointTTF->setString(CC_ITOA(info.point));
            m_spr->setVisible(true);
            if (m_sprType == 1) {
                auto ret = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("item658.png");
                if (ret == NULL) {
//                    m_rewardSp->setDisplayFrame(CCLoadSprite::loadResource("item302.png"));
                }
                else{}
//                    m_rewardSp->setDisplayFrame(CCLoadSprite::loadResource("item658.png"));
               // m_rewardNode->setScale(1.2);
            }
            else if (m_sprType == 2) {
                auto ret = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("item610.png");
                if (ret == NULL) {
//                    m_rewardSp->setDisplayFrame(CCLoadSprite::loadResource("item302.png"));
                }
//                else
//                    m_rewardSp->setDisplayFrame(CCLoadSprite::loadResource("item610.png"));
                //m_rewardNode->setScale(1.2);
            }
            updateInfo(nullptr);
            if(m_itemId == "")
            {
                this->setVisible(false);
            }

        }
        ret = true;
    } while (0);
    
    return ret;
}

//void DailyActiveBoxInfoCell::onRewardBtnClick(cocos2d::CCObject *pNode, CCControlEvent pEvent)
//{
//    PopupViewController::getInstance()->addPopupView(DailyActiveBoxInfoView::create(m_itemId));
//}

void DailyActiveBoxInfoCell::updateInfo(cocos2d::CCObject *pObj)
{
    m_particleNode->removeAllChildren();
    m_particleNode2->removeAllChildren();
    m_btnTTF->setVisible(false);
    if (m_itemId.length() == 0)
    {
        return;
    }
    DailyActiveBoxInfo info = DailyActiveController::shared()->m_boxInfo[atoi(m_itemId.c_str())];
    m_rewardSp->stopAllActions();
    if (info.state == BOX_COMPLETE)
    {
//        m_btnTTF->setVisible(true);
        string tmpStart = "Feedback_";
        int count = 4;
        for (int i=2; i<=count; i++)
        {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
            particle->setPositionType(kCCPositionTypeRelative);
            m_particleNode->addChild(particle);
        }
        auto particle2 = ParticleController::createParticle("Feedback_1");
        particle2->setPositionType(kCCPositionTypeRelative);
        m_particleNode2->addChild(particle2);
        this->getAnimationManager()->runAnimationsForSequenceNamed("Action");
    }
    else
    {
        this->getAnimationManager()->runAnimationsForSequenceNamed("Default");
    }
    if (info.state == BOX_GET_REWARD)
    {
        m_rewardSp->setDisplayFrame(CCLoadSprite::loadResource("daily_box_open.png"));
        if (CCCommonUtils::isIosAndroidPad()) {
            m_rewardSp->setScale(2.0f);
        }
        else
            m_rewardSp->setScale(1.0f);
    }
    
    if (info.state == BOX_COMPLETE || info.state == BOX_GET_REWARD)
    {
        CCCommonUtils::setSpriteGray(m_rewardSp, false);
    }
    else
    {
        CCCommonUtils::setSpriteGray(m_rewardSp, true);
    }
}

bool DailyActiveBoxInfoCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_rewardSp, pTouch))
    {
        if (GuideKey == "DailyActive_guide1") {
            return false;
        }
        if (CCCommonUtils::isIosAndroidPad()) {
            m_rewardSp->setScale(2.0*0.9);
        }
        else
            m_rewardSp->setScale(1.0*0.9);
        return true;
    }
    return false;
}

void DailyActiveBoxInfoCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    DailyActiveBoxInfo info = DailyActiveController::shared()->m_boxInfo[atoi(m_itemId.c_str())];
    if (info.state == BOX_GET_REWARD)
    {
        if (CCCommonUtils::isIosAndroidPad()) {
            m_rewardSp->setScale(2.0);
        }
        else
            m_rewardSp->setScale(1.0);
    }
    else
    {
        if (CCCommonUtils::isIosAndroidPad()) {
            m_rewardSp->setScale(2.0);
        }
        else
            m_rewardSp->setScale(1.0);
    }
    
    if (ccpDistance(pTouch->getStartLocation(), pTouch->getLocation()) > 10)
    {
        return;
    }
    
    if (info.state == BOX_GET_REWARD)
    {
        CCCommonUtils::flyHint("", "", _lang("133192"));
        return;
    }
    if(m_itemId == "")
    {
        return;
    }
    PopupViewController::getInstance()->addPopupView(DailyActiveBoxInfoView::create(m_itemId, this->getTag()));
}



void DailyActiveBoxInfoCell::onEnter()
{
    CCAniNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DailyActiveBoxInfoCell::updateInfo), DAILYQEUST_BOX_INIT, NULL);
}

void DailyActiveBoxInfoCell::onExit()
{
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, DAILYQEUST_BOX_INIT);
    CCAniNode::onExit();
}

bool DailyActiveBoxInfoCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_particleNode", CCNode*, this->m_particleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_particleNode2", CCNode*, this->m_particleNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardSp", CCSprite*, this->m_rewardSp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardNode", CCNode*, this->m_rewardNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_pointTTF", CCLabelIF*, this->m_pointTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnTTF", CCLabelIF*, this->m_btnTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_spr", CCSprite*, this->m_spr);
    return false;
}

SEL_CCControlHandler DailyActiveBoxInfoCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRewardBtnClick", DailyActiveBoxInfoCell::onRewardBtnClick);
    return nullptr;
}

