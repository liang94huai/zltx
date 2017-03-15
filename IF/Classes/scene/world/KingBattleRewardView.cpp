//
//  KingBattleRewardView.cpp
//  IF
//
//  Created by xiaoming on 15/12/7.
//
//

#include "KingBattleRewardView.hpp"
#include "AllianceActivityRewardView.h"

#define REWARD_CELL_WIDTH 500
#define REWARD_CELL_HEIGHT  80


KingBattleRewardView* KingBattleRewardView::create()
{
    KingBattleRewardView* ret = new KingBattleRewardView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool KingBattleRewardView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    CCLoadSprite::doResourceByCommonIndex(8, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(8, false);
    });

    setIsHDPanel(true);
    
    auto ccb = CCBLoadFile("KingdomThroneActivityReward",this,this);
    this->setContentSize(ccb->getContentSize());
    
    m_labelTitle->setString(_lang("170036"));
    m_tab1->setString(_lang("170044"));
    m_tab2->setString(_lang("170043"));
    m_tab3->setString(_lang("170037"));
    m_attackPrizeLabel->setString(_lang("170038"));
    m_defensePrizeLabel->setString(_lang("170038"));
    m_attackWinLabel->setString(_lang("170039"));
    
    
    //Hide other function
//    this->setModelLayerTouchCallback([&](cocos2d::CCTouch *pTouch){
//        this->touchPressed(pTouch->getLocation());
//    });
    m_btnNode->setVisible(false);
    
    
    
    
    

    bannerType=3;
    attackRewardArr = NULL;
    defenseRewardArr = NULL;
    singleRewardArr = NULL;
    m_attackScrollView=NULL;
    m_defenseScrollView=NULL;
    m_singleScrollView=NULL;
    
    CTFGetRewardCommand* cmd = new CTFGetRewardCommand();
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(KingBattleRewardView::getRewardDataCallBack), NULL));
    cmd->sendAndRelease();
    
    return true;
}

cocos2d::SEL_MenuHandler KingBattleRewardView::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
    return NULL;
}
cocos2d::extension::Control::Handler KingBattleRewardView::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseClick", KingBattleRewardView::onCloseClick);
    return NULL;
}
bool KingBattleRewardView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_labelTitle", CCLabelIF*, this->m_labelTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tab1", CCLabelIF*, this->m_tab1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tab2", CCLabelIF*, this->m_tab2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tab3", CCLabelIF*, this->m_tab3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn1Area", CCScale9Sprite*, this->m_btn1Area);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn2Area", CCScale9Sprite*, this->m_btn2Area);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn3Area", CCScale9Sprite*, this->m_btn3Area);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_attackNode", CCNode*, this->m_attackNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defenseNode", CCNode*, this->m_defenseNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_singleNode", CCNode*, this->m_singleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_attackScrollNode", CCScale9Sprite*, this->m_attackScrollNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defenseScrollNode", CCScale9Sprite*, this->m_defenseScrollNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_singleScrollNode", CCNode*, this->m_singleScrollNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_attackPrizeLabel", CCLabelIF*, this->m_attackPrizeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_attackWinLabel", CCLabelIF*, this->m_attackWinLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defensePrizeLabel", CCLabelIF*, this->m_defensePrizeLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode", CCNode*, this->m_btnNode);

    return false;
}

void KingBattleRewardView::onCloseClick(CCObject *pSender, CCControlEvent event)
{
    this->closeSelf();
}


void KingBattleRewardView::touchPressed(Vec2 touchPoint)
{
    Vec2 nodePoint = (m_btn1Area->getParent())->convertToNodeSpace(touchPoint);
    
    if(this->checkBannerTouched(touchPoint,m_btn1Area)){
        this->updateView(1);
    }
    else if(this->checkBannerTouched(touchPoint,m_btn2Area)){
        this->updateView(2);
    }
    else if (this->checkBannerTouched(touchPoint,m_btn3Area)){
        this->updateView(3);
    }
}

bool KingBattleRewardView::checkBannerTouched(Vec2 touchPoint,Node *checkNode)
{
    Vec2 nodePoint = (checkNode->getParent())->convertToNodeSpace(touchPoint);
    return checkNode->boundingBox().containsPoint(nodePoint);
}


void KingBattleRewardView::getRewardDataCallBack(CCObject *obj)
{
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        return;
    }
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    
    if (info->objectForKey("attSide")) {
        auto dic = (CCDictionary *)info->objectForKey("attSide");
        if (dic->objectForKey("reward")) {
            attackRewardArr = dynamic_cast<CCArray*>(dic->objectForKey("reward"));
            
        }
    }
    if (info->objectForKey("defSide")) {
        auto dic = (CCDictionary *)info->objectForKey("defSide");
        if (dic->objectForKey("reward")) {
            defenseRewardArr = dynamic_cast<CCArray*>(dic->objectForKey("reward"));
        }
    }
    if (info->objectForKey("member")) {
        singleRewardArr = dynamic_cast<CCArray*>(info->objectForKey("member"));
    }
    
    this->updateView(0);
}


void KingBattleRewardView::updateView(int newBanner)
{
    if (newBanner!=0) {
        if (bannerType==newBanner) {
            return;
        }
        else{
            bannerType=newBanner;
        }
    }
    m_tab1->setColor(this->getColor(bannerType==1));
    m_tab2->setColor(this->getColor(bannerType==2));
    m_tab3->setColor(this->getColor(bannerType==3));
    
    m_attackNode->setVisible(bannerType==1);
    m_defenseNode->setVisible(bannerType==2);
    m_singleNode->setVisible(bannerType==3);
    
    if (bannerType==3) {
        this->setSingleRewardScroll();
    }
    else{
        this->setKingRewardScroll();
    }
}

void KingBattleRewardView::setKingRewardScroll()
{
    CCScrollView **scrollView=NULL;
    CCNode *scrollNode=NULL;
    CCArray *dataArr=NULL;
    if(bannerType==1){
        scrollView = &m_attackScrollView;
        scrollNode=m_attackScrollNode;
        dataArr=attackRewardArr;
    }
    else{
        scrollView = &m_defenseScrollView;
        scrollNode=m_defenseScrollNode;
        dataArr=defenseRewardArr;
    }
    if(*scrollView!=NULL){
        CCNode *nodeTemp = (*scrollView)->getContainer();
        if (nodeTemp) {
            nodeTemp->setPosition(Vec2(0, (*scrollView)->getViewSize().height-nodeTemp->getContentSize().height));
        }
        return;
    }
    if(dataArr==NULL){
        return;
    }
    *scrollView = CCScrollView::create(scrollNode->getContentSize());
    (*scrollView)->setDirection(kCCScrollViewDirectionVertical);
    scrollNode->getParent()->addChild((*scrollView),100);
    CCNode *scrollContainer = CCNode::create();
    (*scrollView)->setContainer(scrollContainer);
    (*scrollView)->setPosition(scrollNode->getPosition());
    
    float rewardHeight=8;
    float cellHeight=REWARD_CELL_HEIGHT;
    for (int i=dataArr->count()-1; i>=0; i--) {
        CCDictionary* reward = (CCDictionary *)dataArr->objectAtIndex(i);
        AllianceActivityRewardCell* cell = AllianceActivityRewardCell::create(reward,true);
        if (cell==NULL) {
            continue;
        }
        scrollContainer->addChild(cell);
        rewardHeight=rewardHeight+cellHeight;
        cell->setPosition(Vec2(5, 8+(float)(dataArr->count()-i-1)*cellHeight));
    }
    
    
    scrollContainer->setContentSize(Size(REWARD_CELL_WIDTH, rewardHeight+2));
    scrollContainer->setPosition(Vec2(0, (*scrollView)->getViewSize().height-scrollContainer->getContentSize().height));
    (*scrollView)->updateInset();
}
void KingBattleRewardView::setSingleRewardScroll()
{
    if (singleRewardArr==NULL) {
        return;
    }
    if(m_singleScrollView!=NULL){
        CCNode *nodeTemp = m_singleScrollView->getContainer();
        if (nodeTemp) {
            nodeTemp->setPosition(Vec2(0, m_singleScrollView->getViewSize().height-nodeTemp->getContentSize().height));
        }
        return;
    }
    m_singleScrollView = CCScrollView::create(m_singleScrollNode->getContentSize());
    m_singleScrollView->setDirection(kCCScrollViewDirectionVertical);
    m_singleScrollNode->getParent()->addChild(m_singleScrollView,100);
    CCNode *scrollContainer = CCNode::create();
    m_singleScrollView->setContainer(scrollContainer);
    m_singleScrollView->setPosition(m_singleScrollNode->getPosition());
    
    m_singleScrollNode->getParent()->setVisible(true);
    float rewardHeight=5;
    for (int i=singleRewardArr->count()-1; i>=0; i--) {
        CCDictionary *dicMem = (CCDictionary *)singleRewardArr->objectAtIndex(i);
        KingBattleRewardCell *cell = KingBattleRewardCell::create(dicMem);
        if (cell==NULL) {
            continue;
        }
        scrollContainer->addChild(cell);
        cell->setPosition(Vec2(5, rewardHeight));
        rewardHeight=rewardHeight+cell->getContentSize().height;
    }
    
    scrollContainer->setContentSize(Size(REWARD_CELL_WIDTH, rewardHeight+10));
    scrollContainer->setPosition(Vec2(0, m_singleScrollView->getViewSize().height-scrollContainer->getContentSize().height));
    m_singleScrollView->updateInset();
}


Color3B KingBattleRewardView::getColor(bool isSelect)
{
    Color3B color;
    if(isSelect){
        color.r=238;
        color.g=180;
        color.b=34;
    }
    else{
        color.r=160;
        color.g=82;
        color.b=45;
    }
    return color;
}




KingBattleRewardCell * KingBattleRewardCell::create(CCDictionary *info)
{
    auto ret = new KingBattleRewardCell(info);
    if(ret){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
KingBattleRewardCell::KingBattleRewardCell(CCDictionary *info)
{
    if (Node::init()==false||info==NULL) {
        return;
    }
    Label *label = Label::createWithSystemFont("", "Helvetica", 20);
    label->setAnchorPoint(Vec2(0, 0));
    this->addChild(label);
    std::string strRank="";
    if (info->objectForKey("rank") && info->valueForKey("rank")->intValue()>0) {
        strRank = Value(info->valueForKey("rank")->intValue()).asString();
    }
    else if (info->objectForKey("min")&&info->objectForKey("max")){
        strRank = Value(info->valueForKey("min")->intValue()).asString() + "-" + Value(info->valueForKey("max")->intValue()).asString();
    }
    label->setString(_lang_1("170040", strRank.c_str()));
    float rewardHeight=0;
    float cellHeight = REWARD_CELL_HEIGHT;
    if (info->objectForKey("reward")) {
        CCArray *dataArr = (CCArray*)info->objectForKey("reward");
        for (int i=dataArr->count()-1; i>=0; i--) {
            CCDictionary* reward = (CCDictionary *)dataArr->objectAtIndex(i);
            AllianceActivityRewardCell* cell = AllianceActivityRewardCell::create(reward,true);
            this->addChild(cell);
            cell->setPosition(Vec2(5, 8+(float)(dataArr->count()-1-i)*cellHeight));
            rewardHeight=rewardHeight+cellHeight;
        }
        
    }
    auto bg = CCLoadSprite::createScale9Sprite("Activity_bg7.png");
    bg->setContentSize(Size(REWARD_CELL_WIDTH, rewardHeight+10));
    bg->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2));
    this->addChild(bg,-1);
    
    label->setPosition(Vec2(0, rewardHeight+15));
    this->setContentSize(Size(REWARD_CELL_WIDTH, rewardHeight+50));
    
}
