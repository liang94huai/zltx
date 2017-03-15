//
//  IFAllianceHelpReportLayer.cpp
//  IF
//
//  Created by wangdianzhen on 15/10/21.
//
//

#include "IFAllianceHelpReportLayer.hpp"
#include "PlayerInfoBtnPopUpView.h"
#include "PopupViewController.h"
IFAllianceHelpReportLayer* IFAllianceHelpReportLayer::create(){
    IFAllianceHelpReportLayer* pRet = new IFAllianceHelpReportLayer();
    if (pRet && pRet->init()) {
        return pRet;
    }
    else{
        CC_SAFE_DELETE(pRet);
    }
    return NULL;
}
IFAllianceHelpReportLayer::IFAllianceHelpReportLayer(){
}
IFAllianceHelpReportLayer::~IFAllianceHelpReportLayer(){
    
}
void IFAllianceHelpReportLayer::onEnter()
{
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(IFAllianceHelpReportLayer::refreashData), ALLIANCE_HELP_RANK, NULL);
    setTitleName(_lang("115563"));
    m_nameLbTitle->setString(_lang("115564").c_str());
    m_soldierLbTitle->setString(_lang("115565"));
    m_timeLbTitle->setString(_lang("115235"));
}

void IFAllianceHelpReportLayer::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ALLIANCE_HELP_RANK);
    CCNode::onExit();
}
bool IFAllianceHelpReportLayer::init( ){
    if (!PopupBaseView::init()) {
        return false;
    }
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByCommonIndex(200, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(7, false);
        CCLoadSprite::doResourceByCommonIndex(200, false);
    });
    setIsHDPanel(true);
    auto ccbNode = CCBLoadFile("HelpReportLayer",this ,this);
    this->setContentSize(ccbNode->getContentSize());
    int oldBgHeight = m_buildBG->getContentSize().height;
    changeBGHeight(m_buildBG);
    int newBgHeight = m_buildBG->getContentSize().height;
    int extendHeight = newBgHeight - oldBgHeight + 10;
    CCSize oldListSize = m_infoList->getContentSize();
    m_infoList->setContentSize(CCSizeMake(oldListSize.width, oldListSize.height + extendHeight));
    m_infoList->setPositionY(m_infoList->getPositionY() - extendHeight);
    
    m_totalNode->setPositionY(m_totalNode->getPositionY() - extendHeight);
    m_data = CCArray::create();
    getRankData();
    auto bgSp = CCLoadSprite::createSprite("technology_09.png");
    int modelHeight = bgSp->getContentSize().height;
    
    int count = 0;
    while (count * modelHeight < m_infoList->getContentSize().height + modelHeight) {
        auto childSp = CCLoadSprite::createSprite("technology_09.png");
        childSp->setAnchorPoint(CCPointMake(0.5, 0));
        childSp->setPositionY(modelHeight * count);
        m_totalNode->addChild(childSp);
        count++;
    }
    
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(0);
    m_tabView->setDelegate(this);
    m_infoList->addChild(m_tabView);
    
    return true;

}
void IFAllianceHelpReportLayer::getRankData(){
    GameController::getInstance()->showWaitInterface();
    HelpReportCmd* cmd = new HelpReportCmd();
    cmd->sendAndRelease();
}
void IFAllianceHelpReportLayer::updateInfo()
{
    if (AllianceManager::getInstance()->m_helpReportListVec.size() == 0) {
        m_noDataLb->setVisible(true);
        m_noDataLb->setString(_lang("115569"));
    }
    else{
        m_noDataLb->setVisible(false);
    }
    GameController::getInstance()->removeWaitInterface();
    m_tabView->reloadData();
}
void IFAllianceHelpReportLayer::refreashData(CCObject* obj)
{
    updateInfo();
}
SEL_CCControlHandler IFAllianceHelpReportLayer::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    
    return NULL;
}

bool IFAllianceHelpReportLayer::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLbTitle", CCLabelIF*, this->m_nameLbTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_soldierLbTitle", CCLabelIF*, this->m_soldierLbTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLbTitle", CCLabelIF*, this->m_timeLbTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noDataLb", CCLabelIF*, this->m_noDataLb);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_totalNode", CCNode*, this->m_totalNode);
    
    return false;
}

CCSize IFAllianceHelpReportLayer::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    return CCSize(625, 100);
    
}
CCSize IFAllianceHelpReportLayer::cellSizeForTable(CCTableView *table)
{
    return CCSize(625, 100);
}
CCTableViewCell* IFAllianceHelpReportLayer::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx > AllianceManager::getInstance()->m_helpReportListVec.size()){
        return NULL;
    }
    IFAllianceHelpReportCell* cell = (IFAllianceHelpReportCell*)table->dequeueCell();
    if(cell){
        cell->setData(idx);
    }else{
        cell = IFAllianceHelpReportCell::create(idx);
    }
    
    return cell;
}
ssize_t IFAllianceHelpReportLayer::numberOfCellsInTableView(CCTableView *table)
{
    int num = AllianceManager::getInstance()->m_helpReportListVec.size();
    return num;
}
void IFAllianceHelpReportLayer::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

void IFAllianceHelpReportLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell){
    CCLOG("cellIndex: %i",cell->getIdx());
    IFAllianceHelpReportCell* cellItem = dynamic_cast<IFAllianceHelpReportCell*>(cell);
    cellItem->printLog();
    
}
void IFAllianceHelpReportCell::printLog(){
    CCLOG("cellPrint:  %d",m_index);
}
IFAllianceHelpReportCell* IFAllianceHelpReportCell::create(int index)
{
    auto ret = new IFAllianceHelpReportCell(index);
    if (ret && ret->init(index)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool IFAllianceHelpReportCell::init(int index)
{
    bool ret = true;
    auto layer =  CCBLoadFile("HelpReportCell",this,this);
    this->setContentSize(layer->getContentSize());
    m_index = index;
    m_headImgNode = HFHeadImgNode::create();
    setData(index);
    return ret;
}

void IFAllianceHelpReportCell::setData(int index)
{
    m_index = index;
    auto info = AllianceManager::getInstance()->m_helpReportListVec.at(index);
    if(info){
        m_nameLb->setString(info->getname().c_str());
        time_t time = info->gettime();
        string timeStr = CCCommonUtils::timeStampToDate(time);
        m_timeLb->setString(timeStr.c_str());
        string countStr = CC_ITOA(info->getcount());
        m_countLb->setString(countStr.c_str());
        m_headNode->removeAllChildren();
        CCSprite* pic;
        if(info->getpic()==""){
            pic = CCLoadSprite::createSprite("guidePlayerIcon.png");
//            pic->setScale(0.7);
        }else{
            string mpic = info->getpic()+".png";
            pic = CCLoadSprite::createSprite(mpic.c_str(),true,CCLoadSpriteType_HEAD_ICON);
        }
        CCCommonUtils::setSpriteMaxSize(pic, 70);
        m_headNode->addChild(pic);
        if (CCCommonUtils::isUseCustomPic(info->getpicVer()))
        {
            m_headImgNode->initHeadImgUrl2(m_headNode, CCCommonUtils::getCustomPicUrl(info->getuid(), info->getpicVer()), 1.0f, 70, true);
        }
    }
}

void IFAllianceHelpReportCell::onEnter() {
    CCNode::onEnter();
//    this->
    setTouchEnabled(true);
//    Director::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);//
}

void IFAllianceHelpReportCell::onExit() {
//    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    setTouchEnabled(false);
    CCNode::onExit();
}
void IFAllianceHelpReportCell::refreashData(CCObject* obj)
{
    
    
}

SEL_CCControlHandler IFAllianceHelpReportCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}
bool IFAllianceHelpReportCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLb", CCLabelIF*, m_nameLb);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_countLb", CCLabelIF*, m_countLb);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLb", CCLabelIF*, m_timeLb);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headNode", CCNode*, m_headNode);
    
    return false;
}

bool IFAllianceHelpReportCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
        m_startPoint = pTouch->getLocation();
        return true;
    
}

void IFAllianceHelpReportCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(fabs(pTouch->getLocation().y - m_startPoint.y)>20||fabs(pTouch->getLocation().x - m_startPoint.x)>20){
        return;
    }
    if (isTouchInside(this, pTouch)) {
        auto info = AllianceManager::getInstance()->m_helpReportListVec.at(m_index);
        
        PlayerInfoBtnPopUpView* pop =PlayerInfoBtnPopUpView::create();
        pop->initHelpBtns(info);
        PopupViewController::getInstance()->addPopupView(pop, false);
    }
    
    
}
