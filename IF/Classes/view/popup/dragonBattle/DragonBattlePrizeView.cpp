//
//  DragonBattlePrizeView.cpp
//  IF
//
//  Created by xiaoming on 15/12/15.
//
//

#include "DragonBattlePrizeView.hpp"
#include "AllianceActivityRewardView.h"

DragonBattlePrizeView* DragonBattlePrizeView::create()
{
    DragonBattlePrizeView* ret = new DragonBattlePrizeView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DragonBattlePrizeView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    this->setTitleName(_lang("140001"));
    
    
    CCLoadSprite::doResourceByCommonIndex(512, true);
    CCLoadSprite::doResourceByCommonIndex(502, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(512, false);
        CCLoadSprite::doResourceByCommonIndex(502, false);
    });
    auto ccb = CCBLoadFile("DragonBattlePrize",this,this);
    this->setContentSize(ccb->getContentSize());
    int dh = TOP_HEIGHT+5;
    if (CCCommonUtils::isIosAndroidPad()) {
        dh = TOP_HEIGHT_HD+5;
    }
    m_listLayer->setPositionY(m_listLayer->getPositionY()-dh);
    m_baseNode->setPositionY(m_baseNode->getPositionY() - dh);
    
    CCCommonUtils::setButtonTitle(m_winBtn, _lang("140197").c_str());
    CCCommonUtils::setButtonTitle(m_loseBtn, _lang("140198").c_str());
    
    winRewardArr=NULL;
    loseRewardArr=NULL;
    
    
    m_scrollView = CCScrollView::create(m_listLayer->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_listLayer->getParent()->addChild(m_scrollView,100);
    scrollLayer = CCNode::create();
    m_scrollView->setContainer(scrollLayer);
    m_scrollView->setPosition(m_listLayer->getPosition());
    
    
    auto* cmd = new DragonRewardCommand();
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(DragonBattlePrizeView::getPrizeDataCallBack), NULL));
    cmd->sendAndRelease();
    
    return true;
}

void DragonBattlePrizeView::getPrizeDataCallBack(CCObject *obj)
{
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        return;
    }
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    if (info->objectForKey("win")) {
        winRewardArr=(CCArray *)info->objectForKey("win");
    }
    if (info->objectForKey("lose")) {
        loseRewardArr=(CCArray *)info->objectForKey("lose");
    }
    
    if (m_winBtn->isEnabled()&&m_loseBtn->isEnabled()) {
        this->updateScrollView(winRewardArr);
        m_winBtn->setEnabled(false);
        m_loseBtn->setEnabled(true);
        m_winBtn->setColor(this->getButtonColor(false));
        m_loseBtn->setColor(this->getButtonColor(true));
    }
    else{
        if (m_winBtn->isEnabled()) {
            this->updateScrollView(loseRewardArr);
        }
        else{
            this->updateScrollView(winRewardArr);
        }
    }
    
}

Color3B DragonBattlePrizeView::getButtonColor(bool enable)
{
    Color3B color;
    
    if (enable) {
        color.r=255;
        color.g=255;
        color.b=255;
    }
    else{
        color.r=255;
        color.g=99;
        color.b=71;
    }
    return color;
}


CCArray * DragonBattlePrizeView::getRewardArr()
{
    CCArray *arr =CCArray::create();
    for (int i=0; i<8; i++) {
        CCDictionary *dic = CCDictionary::create();
        dic->setObject(CCInteger::create(17), "type");
        dic->setObject(CCInteger::create(1), "value");
        arr->addObject(dic);
    }
    return arr;
}

cocos2d::SEL_MenuHandler DragonBattlePrizeView::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
    return NULL;
}
cocos2d::extension::Control::Handler DragonBattlePrizeView::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickWin", DragonBattlePrizeView::onClickWin);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickLose", DragonBattlePrizeView::onClickLose);
    return NULL;
}
bool DragonBattlePrizeView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_winBtn", CCControlButton*, this->m_winBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loseBtn", CCControlButton*, this->m_loseBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listLayer", CCNode*, this->m_listLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_baseNode", CCNode*, this->m_baseNode);
    return false;
}


void DragonBattlePrizeView::onClickWin(CCObject *pSender, CCControlEvent event)
{
    CCLOG("onClickWin~~");
    this->updateScrollView(winRewardArr);
    m_winBtn->setEnabled(false);
    m_loseBtn->setEnabled(true);
    m_winBtn->setColor(this->getButtonColor(false));
    m_loseBtn->setColor(this->getButtonColor(true));
}

void DragonBattlePrizeView::onClickLose(CCObject *pSender, CCControlEvent event)
{
    CCLOG("onClickLose~~");
    this->updateScrollView(loseRewardArr);
    m_winBtn->setEnabled(true);
    m_loseBtn->setEnabled(false);
    m_winBtn->setColor(this->getButtonColor(true));
    m_loseBtn->setColor(this->getButtonColor(false));
}

void DragonBattlePrizeView::updateScrollView(CCArray *dataArr)
{
    scrollLayer->removeAllChildren();
    if (dataArr==NULL) {
        return;
    }
    float rewardHeight = 0;
    
    float designScale =1;
    if (CCCommonUtils::isIosAndroidPad()) {
        designScale=2.4;
    }
    
    for (int i=dataArr->count()-1; i>=0; i--) {
        CCDictionary *dicTemp = (CCDictionary *)dataArr->objectAtIndex(i);
        DragonBattlePrizeCell *cell = DragonBattlePrizeCell::create(dicTemp);
        if (cell==NULL) {
            continue;
        }
        cell->setScale(designScale);
        scrollLayer->addChild(cell);
        cell->setPosition(Vec2(23*designScale,8+rewardHeight));
        rewardHeight=rewardHeight+cell->getContentSize().height*designScale+8;
    }
    float cellWidth = 598;
    scrollLayer->setContentSize(Size(cellWidth*designScale, rewardHeight+10));
    scrollLayer->setPosition(Vec2(0, m_scrollView->getViewSize().height-scrollLayer->getContentSize().height));
    m_scrollView->updateInset();
    
    
}

DragonBattlePrizeCell * DragonBattlePrizeCell::create(CCDictionary *info)
{
    auto ret = new DragonBattlePrizeCell(info);
    if(ret){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
DragonBattlePrizeCell::DragonBattlePrizeCell(CCDictionary *info)
{
    if (Node::init()==false||info==NULL) {
        return;
    }
    float rewardHeight = 0;
    float cellHeight = 90;
    float cellWidth = 600;
    
    if (info->objectForKey("reward")) {
        CCArray *dataArr = (CCArray*)info->objectForKey("reward");
        for (int i=dataArr->count()-1; i>=0; i--) {
            CCDictionary* reward = (CCDictionary *)dataArr->objectAtIndex(i);
            AllianceActivityRewardCell* cell = AllianceActivityRewardCell::create(reward,true);
            this->addChild(cell);
            cell->setPosition(Vec2(15, 20+(float)(dataArr->count()-1-i)*cellHeight*1.2));
            cell->setScale(1.2);
            rewardHeight=rewardHeight+cellHeight*1.2;
            cell->setLabelColor(120,97,67);
        }
        
    }
    auto bgTitle = CCLoadSprite::createScale9Sprite("jlzy_xx_bg_1.png");
    this->addChild(bgTitle);
    bgTitle->setContentSize(Size(cellWidth-2,36));
    this->setContentSize(Size(cellWidth, rewardHeight+bgTitle->getContentSize().height+20));
    bgTitle->setPosition(Vec2(bgTitle->getContentSize().width/2+1, rewardHeight+33));

    std::string strRank="";
    if (info->objectForKey("min")&&info->objectForKey("max")){
        if (info->valueForKey("max")->intValue()>20000) {
            strRank = _lang_1("140199", Value(info->valueForKey("min")->intValue()).asString().c_str());
        }
        else{
            strRank = _lang_2("140200", Value(info->valueForKey("min")->intValue()).asString().c_str(), Value(info->valueForKey("max")->intValue()).asString().c_str());
        }
    }
    
    auto label = Label::createWithSystemFont(strRank, "Helvetica", 20);
    label->setAnchorPoint(Vec2(0, 0.5));
    bgTitle->addChild(label);
    label->setPosition(Vec2(30, bgTitle->getContentSize().height/2));
    label->setColor(Color3B(203, 139, 116));
    
    
    auto spPoint = CCLoadSprite::createSprite("jlzy_xx_dian.png");
    bgTitle->addChild(spPoint);
    spPoint->setPosition(Vec2(10, bgTitle->getContentSize().height/2));
    
    auto bg = CCLoadSprite::createScale9Sprite("jlzy_xx_bg.png");
    bg->setContentSize(Size(this->getContentSize().width, this->getContentSize().height));
    this->addChild(bg,-1);
    bg->setAnchorPoint(Vec2(0, 0));
}

