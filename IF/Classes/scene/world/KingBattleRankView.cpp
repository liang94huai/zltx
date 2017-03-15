//
//  KingBattleRankView.cpp
//  IF
//
//  Created by xiaoming on 15/12/2.
//
//

#include "KingBattleRankView.hpp"
#include "KingBattleRewardView.hpp"
#include "PopupViewController.h"

#define RANK_DATA_HEIGHT   65
#define RANK_DATA_WIDTH   525


KingBattleRankView* KingBattleRankView::create()
{
    KingBattleRankView* ret = new KingBattleRankView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool KingBattleRankView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    
    setIsHDPanel(true);
    
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByCommonIndex(500, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(7, false);
        CCLoadSprite::doResourceByCommonIndex(500, false);
    });
    
    auto ccb = CCBLoadFile("KingdomThroneActivityrank",this,this);
    
    
    designScale=1;
    if (CCCommonUtils::isIosAndroidPad()) {
        designScale=2.4;
    }
    
    
    this->setContentSize(ccb->getContentSize());

    m_tabView = CCTableView::create(this, m_listLayer->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Popup);
    m_tabView->setDelegate(this);
    m_listLayer->addChild(m_tabView);
    
    
    this->setModelLayerTouchCallback([&](cocos2d::CCTouch *pTouch){
        this->touchPressed(pTouch->getLocation());
    });
    
    
    labelNoData = Label::create(_lang("170042"), "Helvetica", 40);
    labelNoData->setColor(Color3B(255, 217, 157));
    labelNoData->setDimensions(400, 400);
    m_listLayer->addChild(labelNoData);
    labelNoData->setPosition(Vec2(m_listLayer->getContentSize().width/2,m_listLayer->getContentSize().height/2+50));
    labelNoData->setVisible(false);
    

    defenseDic = NULL;
    attackDic = NULL;
    lastDataDic=NULL;
    
    CTFGetRankCommand* cmd = new CTFGetRankCommand();
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(KingBattleRankView::updateInfo), NULL));
    cmd->sendAndRelease();
    
    m_titleTxt->setString(_lang("170041"));
    m_rankTxt->setString(_lang("170008"));
    m_nameTxt->setString(_lang("170009"));
    m_killTxt->setString(_lang("170010"));
    m_deadTxt->setString(_lang("170011"));
    
    
    
    m_defenseTxt->setString(_lang("170058"));
    m_attackTxt->setString(_lang("170057"));
    
    CCCommonUtils::setButtonTitle(m_prizeBtn, _lang("133005").c_str());
    return true;
}



cocos2d::SEL_MenuHandler KingBattleRankView::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
    return NULL;
}
cocos2d::extension::Control::Handler KingBattleRankView::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseClick", KingBattleRankView::onCloseClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPrizeClick", KingBattleRankView::onPrizeClick);
    return NULL;
}

bool KingBattleRankView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankTxt", CCLabelIF*, this->m_rankTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_killTxt", CCLabelIF*, this->m_killTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_deadTxt", CCLabelIF*, this->m_deadTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defenseTxt", CCLabelIF*, this->m_defenseTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_attackTxt", CCLabelIF*, this->m_attackTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defenseTouch", CCNode*, this->m_defenseTouch);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_attackTouch", CCNode*, this->m_attackTouch);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_myRankTxt", CCLabelIF*, this->m_myRankTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listLayer", CCNode*, this->m_listLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_prizeBtn", CCControlButton*, this->m_prizeBtn);
    return false;
}


void KingBattleRankView::onEnter()
{
    PopupBaseView::onEnter();
}
void KingBattleRankView::onExit()
{
    this->clearRankDataList();
    PopupBaseView::onExit();
}

void KingBattleRankView::onCloseClick(CCObject *pSender, CCControlEvent event)
{
    this->closeSelf();
}
void KingBattleRankView::onPrizeClick(CCObject *pSender, CCControlEvent event)
{
    PopupViewController::getInstance()->addPopupView(KingBattleRewardView::create());
}

void KingBattleRankView::touchPressed(Vec2 touchPoint)
{
    Vec2 nodePoint = (m_defenseTouch->getParent())->convertToNodeSpace(touchPoint);
    
    if (m_defenseTouch->boundingBox().containsPoint(nodePoint)) {
        this->updateRankData(defenseDic);
    }
    else if (m_attackTouch->boundingBox().containsPoint(nodePoint)){
        this->updateRankData(attackDic);
    }
}


//TableViewDelegate


CCSize KingBattleRankView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= rankDataList.size()){
        return CCSizeZero;
    }
    return CCSize(RANK_DATA_WIDTH*designScale, RANK_DATA_HEIGHT*designScale);
}
CCSize KingBattleRankView::cellSizeForTable(CCTableView *table)
{
    return CCSize(RANK_DATA_WIDTH*designScale, RANK_DATA_HEIGHT*designScale);
}
CCTableViewCell* KingBattleRankView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= rankDataList.size()){
        return NULL;
    }
    RankDataCell* cell = (RankDataCell*)table->dequeueCell();
    if(cell){
        cell->updateData(rankDataList.at(idx));
    }else{;
        cell = RankDataCell::create(rankDataList.at(idx));
        cell->setScale(designScale);
    }
    return cell;
}
ssize_t KingBattleRankView::numberOfCellsInTableView(CCTableView *table)
{
    int num = ceil(rankDataList.size());
    return num;
}
void KingBattleRankView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
    
}
void KingBattleRankView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
}



void KingBattleRankView::updateInfo(CCObject *obj)
{
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        return;
    }
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    if (info->objectForKey("rank")) {
        CCArray *rankDataArr = dynamic_cast<CCArray*>(info->objectForKey("rank"));
        
        for (int i=0; i<rankDataArr->count(); i++) {
            auto dic = (CCDictionary *)rankDataArr->objectAtIndex(i);
            if (dic->valueForKey("serverId")) {
                if (dic->valueForKey("serverId")->intValue()==GlobalData::shared()->playerInfo.selfServerId) {
                    attackDic = dynamic_cast<CCDictionary*>(rankDataArr->objectAtIndex(i));
                }
                else{
                    defenseDic = dynamic_cast<CCDictionary*>(rankDataArr->objectAtIndex(i));
                }
            }
        }
    }
    CCDictionary *dataDic = defenseDic;
    if(dataDic ==NULL){
        dataDic=attackDic;
    }
    this->updateRankData(dataDic);

    
}

void KingBattleRankView::updateRankData(CCDictionary *info)
{
    if(info==NULL){
        return;
    }
    if(lastDataDic!=NULL && lastDataDic == info){
        return;
    }
    lastDataDic = info;
    bool isDefense = lastDataDic == defenseDic;
    
    m_defenseTxt->setColor(this->getLabelColor(isDefense));
    m_attackTxt->setColor(this->getLabelColor(!isDefense));
    
    
    this->clearRankDataList();
    std::string strRank=std::string(_lang("170042"));
    if (info->objectForKey("selfRank")) {
        int myRank = info->valueForKey("selfRank")->intValue();
        if (myRank>0) {
            strRank = std::string(_lang_1("108104", Value(myRank).asString().c_str()));
        }
    }
    m_myRankTxt->setString(strRank);
    if (info->objectForKey("list")) {
        CCArray *dataArr =(CCArray *)info->objectForKey("list");
        for (int i=0; i<dataArr->count(); i++) {
            CCDictionary *dic=(CCDictionary *)dataArr->objectAtIndex(i);
            RankDataInfo *rankData = new RankDataInfo(dic);
            rankDataList.push_back(rankData);
        }
    }
    labelNoData->setVisible(rankDataList.size()==0);
    m_tabView->reloadData();
}

Color3B KingBattleRankView::getLabelColor(bool isSelected)
{
    Color3B color;
    if(isSelected){
        color.r=255;
        color.g=194;
        color.b=64;
    }
    else{
        color.r=158;
        color.g=100;
        color.b=19;
    }
    return color;
}


void KingBattleRankView::clearRankDataList()
{
    if(rankDataList.size()>0){
        for(int i=0;i<rankDataList.size();i++){
            rankDataList.at(i)->release();
        }
        rankDataList.clear();
    }
}


RankDataInfo::RankDataInfo(CCDictionary *dic)
{
    if (dic->objectForKey("rank")) {
        rankNum=dic->valueForKey("rank")->getCString();
    }
    if (dic->objectForKey("name")) {
        playerName=std::string(dic->valueForKey("name")->getCString());
        if (dic->objectForKey("abbr")) {
            std::string abbr = std::string(dic->valueForKey("abbr")->getCString());
            if (strcmp(abbr.c_str(), "")!=0) {
                playerName = "("+abbr+")"+playerName;
            }
        }
    }
    if (dic->objectForKey("killCount")) {
        killNum=dic->valueForKey("killCount")->getCString();
    }
    if (dic->objectForKey("deadCount")) {
        deadNum=std::string(CCString::createWithFormat("%ld",dic->valueForKey("deadCount")->longValue())->getCString());
        deadNum=dic->valueForKey("deadCount")->getCString();
    }
}
RankDataInfo::~RankDataInfo()
{
    
}
std::string RankDataInfo::getRankNum()
{
    return rankNum;
}
std::string RankDataInfo::getPlayerName()
{
    return playerName;
}
std::string RankDataInfo::getKillNum()
{
    return killNum;
}
std::string RankDataInfo::getDeadNum()
{
    return deadNum;
}


RankDataCell * RankDataCell::create(RankDataInfo *info)
{
    RankDataCell* ret = new RankDataCell(info);
    return ret;
}
RankDataCell::RankDataCell(RankDataInfo *info)
{
    auto backImg = CCLoadSprite::createSprite("kingwar_ph_bg.png");
    backImg->setAnchorPoint(Vec2(0,0));
    backImg->setScaleX(3.2);
    this->addChild(backImg);
    topIcon=NULL;
    this->setLabel(&labelRank, Vec2(38, RANK_DATA_HEIGHT/2),20);
    this->setLabel(&labelName, Vec2(185, RANK_DATA_HEIGHT/2),18);
    this->setLabel(&labelKill, Vec2(345, RANK_DATA_HEIGHT/2),18);
    this->setLabel(&labelDead, Vec2(472, RANK_DATA_HEIGHT/2),18);
    this->updateData(info);
}

void RankDataCell::setLabel(Label **label,Vec2 pos,float fontSize)
{
    (*label)=Label::create("", "Arial", 20);
    this->addChild(*label);
    (*label)->setAnchorPoint(Vec2(0.5, 0.5));
    (*label)->setPosition(pos);
    //(*label)->setColor(Color3B(205, 133, 63));
}

void RankDataCell::updateData(RankDataInfo *info)
{
    if(info==NULL){
        return;
    }
    if(topIcon!=NULL){
        topIcon->removeFromParent();
        topIcon=NULL;
    }
    labelRank->setString(info->getRankNum());
    labelName->setString(info->getPlayerName());
    labelKill->setString(info->getKillNum());
    labelDead->setString(info->getDeadNum());
    
    int rankNum=atoi(info->getRankNum().c_str());
    labelRank->setVisible(rankNum>3);
    if(rankNum<=3){
        std::string fileName = "Alliance_Ranking"+Value(rankNum+1).asString()+".png";
        topIcon=CCLoadSprite::createSprite(fileName.c_str());
        if (topIcon) {
            this->addChild(topIcon);
            topIcon->setPosition(labelRank->getPosition());
        }
    }
    
}

