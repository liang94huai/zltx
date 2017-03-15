//
//  SoldierInfoView.cpp
//  IF
//
//  Created by ganxiaohua on 14-5-29.
//
//

#include "SoldierInfoView.h"
#include "PopupViewController.h"
#include "ProductionSoldiersView.h"
#include "FunBuildController.h"
#include "ArmyController.h"
#include "UIComponent.h"
#include "ParticleController.h"
#include "fireandcomman.h"
#include "YesNoDialog.h"
#include "TipsView.h"
#include "NewTroopsView.h"

SoldierInfoView* SoldierInfoView::create(ArmyInfo* info,int buildingId){
    SoldierInfoView* ret = new SoldierInfoView(info,buildingId);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool SoldierInfoView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    bool m_isFort = m_buildingId/1000 == FUN_BUILD_FORT;
    int index = m_buildingId/1000000 - 423 + 200;
//    CCLoadSprite::doResourceByCommonIndex(index, true);
    CCLoadSprite::doResourceByCommonIndex(502, true);
    CCLoadSprite::doResourceByCommonIndex(510, true);
    setCleanFunction([index](){
//        CCLoadSprite::doResourceByCommonIndex(index, false);
        CCLoadSprite::doResourceByCommonIndex(502, false);
        CCLoadSprite::doResourceByCommonIndex(510, false);
    });
    
    

    CCBLoadFile("SoldierInfoView",this,this);
    setContentSize(CCDirector::sharedDirector()->getWinSize());
    int extH = getExtendHeight();
    m_mainNode->setPositionY(m_mainNode->getPositionY()+extH/2);
    m_bgColorLayer->setContentSize(CCSizeMake(m_bgColorLayer->getContentSize().width, m_bgColorLayer->getContentSize().height+extH));
    m_bgColorLayer->setPositionY(m_bgColorLayer->getPositionY() - extH);
    
//    this->setModelLayerTouchCallback([&](cocos2d::CCTouch *pTouch){
//        // detect touch inside panel
//        if (!isTouchInside(m_touchNode, pTouch)) {
//            this->onCloseclick(NULL,Control::EventType::TOUCH_DOWN);
//        }
//    });
    
    auto addBatchNode = [&](CCNode* node, const char* name){
        auto tbg = CCLoadSprite::loadResource(name);
        auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
        
        for (int i = 0; i < node->getChildrenCount(); i++)
        {
            auto bg = (CCNode*)(node->getChildren().at(i));
            bg->removeFromParent();
            tBatchNode->addChild(bg);
        }
        node->addChild(tBatchNode);
    };
    
    addBatchNode(m_bgNode1, "Ranking_tiaomu_bg8.png");
    addBatchNode(m_bgNode2, "Ranking_tiaomu_bg8.png");
    addBatchNode(m_bgNode3, "Ranking_tiaomu_bg8.png");
    addBatchNode(m_bgNode4, "Ranking_tiaomu_bg8.png");
    
    addBatchNode(m_bgNode5, "soldier_01.png");
    addBatchNode(m_bgNode6, "soldier_01.png");
    addBatchNode(m_bgNode7, "soldier_01.png");
    addBatchNode(m_bgNode8, "soldier_01.png");
    addBatchNode(m_bgNode9, "soldier_01.png");
    addBatchNode(m_bgNode10, "soldier_01.png");
    addBatchNode(m_bgNode11, "soldier_01.png");
    addBatchNode(m_bgNode12, "soldier_01.png");
    addBatchNode(m_bgNode13, "soldier_01.png");
    addBatchNode(m_bgNode14, "soldier_01.png");
    addBatchNode(m_bgNode15, "soldier_02.png");
    
    auto setBtnSp = [&](CCControlButton* btn){
        int tag = btn->getTag();
        string iconName = m_info->getAttrIcon(tag);
        CCLOG("iconName:%s", iconName.c_str());
        btn->setVisible(true);
        if (iconName == "")
        {
            btn->setVisible(false);
            return;
        }
        iconName.append(".png");
        btn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite(iconName.c_str()), CCControlStateNormal);
        btn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite(iconName.c_str()), CCControlStateHighlighted);
        btn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite(iconName.c_str()), CCControlStateDisabled);
    };
    setBtnSp(m_infoBtn0);
    setBtnSp(m_infoBtn1);
    setBtnSp(m_infoBtn2);
    
    CCPoint pos = ccp(0,0);
    if(m_buildingId/1000 == FUN_BUILD_FORT){
        pos = ccp(0,-80);
    }
//    CCSprite* pic = CCLoadSprite::createSprite(m_info->getBodyIcon().c_str());
    m_iconSp->setPosition(pos);
//    m_icon->addChild(pic);
    m_iconSp->setDisplayFrame(CCLoadSprite::loadResource(m_info->getBodyIcon().c_str()));
//    if(m_buildingId/1000==FUN_BUILD_BARRACK4){
//        pic->setScale(0.85);
//    }
    if(m_isFort){
        std::string particleName = "WeaponsFire_Wood";
        for (int i=2; i <= 6; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s_%d",particleName.c_str(),i)->getCString());
            particle->setPosition(0, -95);
            m_icon->addChild(particle);
        }
        m_upkeepNode->setVisible(false);
    }
//    m_soldierBg->setVisible(m_buildingId/1000!=FUN_BUILD_BARRACK4);
    
    string lang = LocalController::shared()->getLanguageFileName();
    if (lang == "en" || lang == "ru")
    {
        m_attackValue->setPositionY(m_attackValue->getPositionY()-1);
        m_defValue->setPositionY(m_defValue->getPositionY()-1);
        m_lifeValue->setPositionY(m_lifeValue->getPositionY()-1);
        m_rangeValue->setPositionY(m_rangeValue->getPositionY()-1);
        m_speedValue->setPositionY(m_speedValue->getPositionY()-1);
        m_weihtValue->setPositionY(m_weihtValue->getPositionY()-1);
        m_upkeepValue->setPositionY(m_upkeepValue->getPositionY()-1);
        m_fightValue->setPositionY(m_fightValue->getPositionY()-1);
    }
    m_nameTxt->setString(m_info->getName());
//    m_attrTxt->setString(_lang("102136").c_str());
    m_fightTxt->setString((_lang("105034")+": ").c_str());
    CCString *powerStr = CCString::createWithFormat("%.1f",m_info->power);
    m_fightValue->setString(powerStr->getCString());
    m_attackTxt->setString((_lang("103600")+": ").c_str());
    m_attackValue->setString(CC_CMDITOA(m_info->attack));
    m_defTxt->setString((_lang("103601")+": ").c_str());
    m_defValue->setString(CC_CMDITOA( m_info->defence));
    m_lifeTxt->setString((_lang("103605")+": ").c_str());
    m_lifeValue->setString(CC_CMDITOA( m_info->health));
    m_speedTxt->setString((_lang("103602")+": ").c_str());
    m_speedValue->setString(CC_CMDITOA( m_info->speed));
    m_weihtTxt->setString((_lang("103604")+": ").c_str());
    m_weihtValue->setString(CC_CMDITOA(ArmyController::getInstance()->getLoad(m_info)));
    m_rangeTxt->setString((_lang("103603")+": ").c_str());
    int range = m_info->range/10;
    range = MAX(range, 1);
    m_rangeValue->setString(CC_CMDITOA(range));
    m_numTxt->setString(_lang_1("104907", ""));
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_numTxt->setMaxScaleXByWidth(170);
    } else {
        m_numTxt->setMaxScaleXByWidth(75);
    }
    m_numValue->setString(CC_CMDITOA(m_info->free));
    m_upkeepTxt->setString((_lang("103606")+": "));
    m_upkeepValue->setString(CCString::createWithFormat("%.2f", m_info->upkeep)->getCString());
//    m_desc->setString(m_info->getDesc().c_str());
    m_starNode->removeAllChildrenWithCleanup(true);
    for (int i=0; i<10; i++) {
        CCSprite* spr = NULL;
        if(m_info->level>=(i+1)){
            spr = CCLoadSprite::createSprite("mail_save.png");
            spr->setScale(0.6);
        }else{
            spr = CCLoadSprite::createSprite("icon_gray.png");
        }
        spr->setVisible(false);
        spr->setPosition(ccp(-131.6+i*28,0));
        m_starNode->addChild(spr);
    }
    //this->closeSelf();
    if(m_info->isArmy){
        CCCommonUtils::setButtonTitle(m_okBtn, _lang("102135").c_str());
    }else{
        CCCommonUtils::setButtonTitle(m_okBtn, _lang("102121").c_str());
    }
    
    auto m_sliderBg = CCLoadSprite::createScale9Sprite("huadongtiao3.png");
    m_sliderBg->setInsetBottom(5);
    m_sliderBg->setInsetLeft(5);
    m_sliderBg->setInsetRight(5);
    m_sliderBg->setInsetTop(5);
    m_sliderBg->setAnchorPoint(ccp(0.5,0.5));
    m_sliderBg->setPosition(ccp(304/2, 25));
    m_sliderBg->setContentSize(CCSize(304,18));
    
    auto bgSp = CCLoadSprite::createSprite("huadongtiao_new.png");
    bgSp->setVisible(false);
    auto proSp = CCLoadSprite::createSprite("huadongtiao_new.png");
    auto thuSp = CCLoadSprite::createSprite("huadongtiao1.png");
    
    m_trainSlider = CCSliderBar::createSlider(m_sliderBg, proSp, thuSp);
    m_trainSlider->setMinimumValue(0.0f);
    m_trainSlider->setMaximumValue(1.0f);
    m_trainSlider->setProgressScaleX(300/proSp->getContentSize().width);
    m_trainSlider->setTag(1);
    m_trainSlider->setLimitMoveValue(15);
    m_trainSlider->setTouchPriority(Touch_Popup);
    m_trainSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(SoldierInfoView::sliderCallBack), CCControlEventValueChanged);
    m_sliderContainer->addChild(m_trainSlider, 1);
    
    auto editSize = m_editBoxNode->getContentSize();
    auto editpic =CCLoadSprite::createScale9Sprite("btn_bg_op.png");
    editpic->setContentSize(editSize);
    editpic->setInsetBottom(1);
    editpic->setInsetTop(1);
    editpic->setInsetRight(1);
    editpic->setInsetLeft(1);
    m_editBox = CCEditBox::create(editSize,editpic );
    m_editBox->setInputMode(kEditBoxInputModeNumeric);
    m_editBox->setFontColor(ccBLACK);
    m_editBox->setText("0");
    m_editBox->setDelegate(this);
 //   m_editBox->setTouchPriority(Touch_Popup);
    m_editBox->setMaxLength(12);
    m_editBox->setReturnType(kKeyboardReturnTypeDone);
    m_editBox->setPosition(ccp(editSize.width/2, editSize.height/2));
    m_editBoxNode->addChild(m_editBox);
//    m_okBtn->setTouchPriority(Touch_Popup_Item);
    
    refreshNum();
    m_jieguNode->setVisible(false);
    
    if(m_isFort){
        m_defenseNode->setVisible(false);
        m_lifeNode->setVisible(false);
        m_speedNode->setVisible(false);
        m_weightNode->setVisible(false);
        m_upkeepNode->setVisible(false);
        m_rangeNode->setPosition(m_defenseNode->getPosition());
        m_fightNode->setPosition(m_lifeNode->getPosition());
    }
    return true;
}
void SoldierInfoView::editBoxTextChanged(CCEditBox* editBox, const std::string& text){
    std::string temp  =text;
    size_t pos = temp.find_first_not_of("1234567890");
    while (pos!=std::string::npos) {
        temp.replace(pos, 1, "");
        pos = temp.find_first_not_of("1234567890");
    }
    int a = atoi(temp.c_str());
    std::string aaaa = CC_CMDITOA(a);
    editBox->setText(aaaa.c_str());
}

void SoldierInfoView::onEnter(){
//    UIComponent::getInstance()->hideReturnBtn();
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SoldierInfoView::refreshNum), ARMY_NUM_CHANGE, NULL);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}

void SoldierInfoView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ARMY_NUM_CHANGE);
    setTouchEnabled(false);
    CCNode::onExit();
}
void SoldierInfoView::sliderCallBack(CCObject*sender,CCControlEvent even){
    m_hireNum = m_trainSlider->getValue() * m_info->free;
    refreshNum();
}
void SoldierInfoView::editBoxReturn(CCEditBox *editBox){
    string numStr = m_editBox->getText();
    if(!numStr.empty()){
        size_t pos = numStr.find(",");
        while(pos!=string::npos){
            numStr.replace(pos, 1, "");
            pos=numStr.find(",");
        }
    }
    int num = atoi(numStr.c_str());
    if(num>m_info->free){
        num = m_info->free;
        
    }else if(num<0){
        num=0;
    }
    m_hireNum = num;
    if(m_info->free<=0){
        m_trainSlider->setValue(0.0);
    }else
        m_trainSlider->setValue(1.0f * num / m_info->free);
    if(m_info->free<=0){
        m_trainSlider->setTouchEnabled(false);
    }else{
        m_trainSlider->setTouchEnabled(true);
    }
    //    refreshNum();
}
void SoldierInfoView::refreshNum(CCObject* p){
    if(m_hireNum>m_info->free){
        m_hireNum = m_info->free;
    }
    m_editBox->setText(CC_CMDITOA(m_hireNum).c_str());
    if(m_hireNum>0){
        m_okBtn->setEnabled(true);
    }else{
        m_okBtn->setEnabled(false);
    }
    m_numValue->setString(CC_CMDITOA(m_info->free));
    string maxStr = "/"+CC_CMDITOA(m_info->free);
//    m_numMaxText->setString(maxStr);
    if(m_info->free<=0){
        m_trainSlider->setTouchEnabled(false);
    }else{
        m_trainSlider->setTouchEnabled(true);
    }
}
void SoldierInfoView::onOkclick(CCObject * pSender, Control::EventType pCCControlEvent){
    if(m_hireNum>0){
        if(m_info->isArmy){
            YesNoDialog::show(_lang("102317").c_str() , CCCallFunc::create(this, callfunc_selector(SoldierInfoView::goToDismissAction)));
        }else{
            YesNoDialog::show(_lang("102318").c_str() , CCCallFunc::create(this, callfunc_selector(SoldierInfoView::goToDismissAction)));
        }
    }
    m_jieguNode->setVisible(false);
    m_infoBtn0->setEnabled(true);
    m_infoBtn1->setEnabled(true);
    m_infoBtn2->setEnabled(true);
}
void SoldierInfoView::goToDismissAction(){
    if(m_info->isArmy){
        fireandcomman *fire = new fireandcomman(m_info->armyId, m_hireNum);
        fire->sendAndRelease();
    }else{
        fireandcomman *fire = new fireandcomman(m_info->armyId, m_hireNum,FORT_FIRE);
        fire->sendAndRelease();
    }
    m_hireNum = 0;
    m_trainSlider->setValue(1.0f * m_hireNum / m_info->free);
    
}
void SoldierInfoView::onAddClick(CCObject * pSender, Control::EventType pCCControlEvent){
    float value  = m_trainSlider->getValue() * m_info->free+1;
    if(m_info->free<=0){
        m_trainSlider->setValue(0.0);
    }else
        m_trainSlider->setValue(1.0f * value / m_info->free);
}
void SoldierInfoView::onSubClick(CCObject * pSender, Control::EventType pCCControlEvent){
    float value = m_trainSlider->getValue() * m_info->free-1;
    if(m_info->free<=0){
        m_trainSlider->setValue(0.0);
    }else
        m_trainSlider->setValue(1.0f * value / m_info->free);
}
void SoldierInfoView::onCloseclick(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->goBackPopupView();
}

void SoldierInfoView::onInfoDetailClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    CCControlButton* btn = (CCControlButton*)pSender;
    int tag = btn->getTag();
    vector<string> descVec;
    m_info->getAttrDesc(tag, descVec);
    string title = _lang(descVec.at(0));
    string info = _lang(descVec.at(1));
    PopupViewController::getInstance()->addPopupView(TipsNewView::create(title,info));
}

bool SoldierInfoView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_jieguNode->isVisible() && !isTouchInside(m_bgColorLayerS, pTouch))
    {
        if (m_editOpen)
        {
            m_editOpen = false;
            return false;
        }
        return true;
    }
    else if (isTouchInside(m_jieguSp, pTouch))
    {
        return true;
    }
    else if (!isTouchInside(m_touchNode, pTouch))
    {
        return true;
    }
    m_editOpen = false;
    return false;
}

void SoldierInfoView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_jieguNode->isVisible() && !isTouchInside(m_bgColorLayerS, pTouch))
    {
        m_jieguNode->setVisible(false);
        m_infoBtn0->setEnabled(true);
        m_infoBtn1->setEnabled(true);
        m_infoBtn2->setEnabled(true);
    }
    else if (isTouchInside(m_jieguSp, pTouch))
    {
        m_jieguNode->setVisible(true);
        m_infoBtn0->setEnabled(false);
        m_infoBtn1->setEnabled(false);
        m_infoBtn2->setEnabled(false);
    }
    else if (!isTouchInside(m_touchNode, pTouch))
    {
        onCloseclick(NULL, CCControlEventTouchUpInside);
    }
}

SEL_CCControlHandler SoldierInfoView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOkclick", SoldierInfoView::onOkclick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSubClick", SoldierInfoView::onSubClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddClick", SoldierInfoView::onAddClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInfoDetailClick", SoldierInfoView::onInfoDetailClick);
    return NULL;
}

bool SoldierInfoView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_okBtn", CCControlButton*, this->m_okBtn);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numMaxText", CCLabelIF*, this->m_numMaxText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, this->m_nameTxt);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_attrTxt", CCLabelIF*, this->m_attrTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_attackTxt", CCLabelIF*, this->m_attackTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_attackValue", CCLabelIF*, this->m_attackValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defTxt", CCLabelIF*, this->m_defTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defValue", CCLabelIF*, this->m_defValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lifeTxt", CCLabelIF*, this->m_lifeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lifeValue", CCLabelIF*, this->m_lifeValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rangeTxt", CCLabelIF*, this->m_rangeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rangeValue", CCLabelIF*, this->m_rangeValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_speedTxt", CCLabelIF*, this->m_speedTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_speedValue", CCLabelIF*, this->m_speedValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_weihtTxt", CCLabelIF*, this->m_weihtTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_weihtValue", CCLabelIF*, this->m_weihtValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numTxt", CCLabelIF*, this->m_numTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numValue", CCLabelIF*, this->m_numValue);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desc", CCLabelIF*, this->m_desc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon", CCNode*, this->m_icon);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_soldierBg", CCScale9Sprite*, this->m_soldierBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_starNode", CCNode*, this->m_starNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upkeepTxt", CCLabelIF*, this->m_upkeepTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upkeepValue", CCLabelIF*, this->m_upkeepValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upkeepNode", CCNode*, this->m_upkeepNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defenseNode", CCNode*, this->m_defenseNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lifeNode", CCNode*, this->m_lifeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rangeNode", CCNode*, this->m_rangeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_speedNode", CCNode*, this->m_speedNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_weightNode", CCNode*, this->m_weightNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sliderContainer", CCNode*, this->m_sliderContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editBoxNode", CCNode*, this->m_editBoxNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fightNode", CCNode*, this->m_fightNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fightValue", CCLabelIF*, this->m_fightValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fightTxt", CCLabelIF*, this->m_fightTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoBtn0", CCControlButton*, this->m_infoBtn0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoBtn1", CCControlButton*, this->m_infoBtn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoBtn2", CCControlButton*, this->m_infoBtn2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode1", CCNode*, this->m_bgNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode2", CCNode*, this->m_bgNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode3", CCNode*, this->m_bgNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode4", CCNode*, this->m_bgNode4);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode5", CCNode*, this->m_bgNode5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode6", CCNode*, this->m_bgNode6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode7", CCNode*, this->m_bgNode7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode8", CCNode*, this->m_bgNode8);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode9", CCNode*, this->m_bgNode9);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode10", CCNode*, this->m_bgNode10);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode11", CCNode*, this->m_bgNode11);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode12", CCNode*, this->m_bgNode12);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode13", CCNode*, this->m_bgNode13);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode14", CCNode*, this->m_bgNode14);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode15", CCNode*, this->m_bgNode15);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_jieguNode", CCNode*, this->m_jieguNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgColorLayer", CCLayerColor*, this->m_bgColorLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgColorLayerS", CCLayerColor*, this->m_bgColorLayerS);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconSp", CCSprite*, this->m_iconSp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_jieguSp", CCSprite*, this->m_jieguSp);
    return false;
}
//////////////////soldierMoreInfoskill/////////////
SoldierMoreInfoView* SoldierMoreInfoView::create(ArmyInfo* info,int buildingId,vector<string> Idvect){
    SoldierMoreInfoView* ret = new SoldierMoreInfoView(info,buildingId,Idvect);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool SoldierMoreInfoView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    bool m_isFort = m_buildingId/1000 == FUN_BUILD_FORT;
    int index = m_buildingId/1000000 - 423 + 200;
    CCLoadSprite::doResourceByCommonIndex(index, true);
    CCLoadSprite::doResourceByCommonIndex(502, true);
    CCLoadSprite::doResourceByCommonIndex(510, true);
    CCLoadSprite::doResourceByCommonIndex(4, true);
    setCleanFunction([=](){
        CCLoadSprite::doResourceByCommonIndex(index, false);
        CCLoadSprite::doResourceByCommonIndex(502, false);
        CCLoadSprite::doResourceByCommonIndex(510, false);
        CCLoadSprite::doResourceByCommonIndex(4, false);
    });
    //加载新面板
    CCBLoadFile("SoldierMoreInfoview",this,this);


    setContentSize(CCDirector::sharedDirector()->getWinSize());
    int extH = getExtendHeight();
    int subExtH =extH - 160;
    if(CCCommonUtils::isIosAndroidPad()){
        subExtH = extH - 20;
    }
    subExtH = 0;
    m_mainNode->setPositionY(m_mainNode->getPositionY()+extH);

    //m_topNode->setPositionY(m_topNode->getPositionY() + subExtH/2);
    m_bottomNode->setPositionY(m_bottomNode->getPositionY() - subExtH);
    //////
    m_BG->setContentSize(m_BG->getContentSize()+CCSizeMake(0, subExtH));

     auto _bg = CCLoadSprite::createSprite("frame_04.png");
    if(_bg)
    {
        auto _size = _bg->getContentSize();
        if(m_jieguBG)
        {
            m_jieguBG->setCapInsets(CCRect(_size.width/2,_size.height/2, 1, 1));
        }
    }
    ////
//    m_leftbar->setContentSize(m_leftbar->getContentSize()+CCSizeMake(subExtH, 0));
//    /////
//    m_rightbar->setContentSize(m_rightbar->getContentSize()+CCSizeMake(subExtH, 0));
    //////
   // m_midBG->setContentSize(m_midBG->getContentSize()+CCSizeMake(0, subExtH));
    //////
    //滚动区域设置
   //m_pic_bg
    if(m_pic_bg)
    {
       CCCommonUtils::addNewDizi(m_pic_bg,1);
    }
    m_infoList->setPositionY(m_infoList->getPositionY() - subExtH);
    m_infoList->setContentSize(m_infoList->getContentSize() + CCSizeMake(0, subExtH));
    
    m_scrollView = CCScrollView::create(m_infoList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_infoList->addChild(m_scrollView);
    m_index = 0;
    std::reverse(m_Idvect.begin(),m_Idvect.end());
    if(find(m_Idvect.begin(),m_Idvect.end(),m_info->armyId) == m_Idvect.end()){
        return false;
    }
    m_index = (find(m_Idvect.begin(),m_Idvect.end(),m_info->armyId))-m_Idvect.begin();
    //箭头状态更新
    arrowState();
    //士兵图标显示
//    soldierIconShow();
    //文本与数据
    textAndDatashow();
    //技能cell的设置
    skillDatacellSet();
    //滑动条
    sliderShow();
    //编辑框
    editOper();
    //升星时
    bstarLv();
    if(m_info->isArmy){
        CCCommonUtils::setButtonTitle(m_okBtn, _lang("102135").c_str());
    }else{
        CCCommonUtils::setButtonTitle(m_okBtn, _lang("102121").c_str());
    }
    //更新数字
    refreshNum();
    return true;
}

bool SoldierMoreInfoView::bstarLv(){
    FunBuildInfo& info = FunBuildController::getInstance()->getFunbuildById(m_buildingId);
    if(info.starNum >= 1){
        m_starlvNode->setVisible(true);
        m_starlvbel->setPositionX(m_starlvbel->getPositionX()+25);
        m_starlvbel->setString(CC_ITOA(info.starNum));
        return true;
    }
    m_starlvNode->setVisible(false);
    return false;
}

void SoldierMoreInfoView::editOper(){
    auto editSize = m_editBoxNode->getContentSize();
    auto editpic =CCLoadSprite::createScale9Sprite("btn_bg_op.png");
    editpic->setContentSize(editSize);
    editpic->setInsetBottom(1);
    editpic->setInsetTop(1);
    editpic->setInsetRight(1);
    editpic->setInsetLeft(1);
    m_editBox = CCEditBox::create(editSize,editpic );
    m_editBox->setInputMode(kEditBoxInputModeNumeric);
    m_editBox->setText("0");
    m_editBox->setDelegate(this);
    m_editBox->setFontColor(ccBLACK);
    //   m_editBox->setTouchPriority(Touch_Popup);
    m_editBox->setMaxLength(12);
    m_editBox->setReturnType(kKeyboardReturnTypeDone);
    m_editBox->setPosition(ccp(editSize.width/2, editSize.height/2));
    m_editBoxNode->addChild(m_editBox);
    //    m_okBtn->setTouchPriority(Touch_Popup_Item);

}

void SoldierMoreInfoView::onOkclick(CCObject * pSender, Control::EventType pCCControlEvent){
    if(m_index < 0 || m_index >= m_Idvect.size()){
        return;
    }
    std::string ind = m_Idvect.at(m_index);
    //更新数据
    auto m_info = GlobalData::shared()->armyList[ind];
    if(m_hireNum>0){
        if(m_info.isArmy){
            YesNoDialog::show(_lang("102317").c_str() , CCCallFunc::create(this, callfunc_selector(SoldierMoreInfoView::goToDismissAction)));
        }else{
            YesNoDialog::show(_lang("102318").c_str() , CCCallFunc::create(this, callfunc_selector(SoldierMoreInfoView::goToDismissAction)));
        }
    }
    m_jieguNode->setVisible(false);
}

void SoldierMoreInfoView::goToDismissAction(){
    if(m_index < 0 || m_index >= m_Idvect.size()){
        return;
    }
    std::string ind = m_Idvect.at(m_index);
    //更新数据
    auto m_info = GlobalData::shared()->armyList[ind];
    if(m_info.isArmy){
        fireandcomman *fire = new fireandcomman(m_info.armyId, m_hireNum);
        fire->sendAndRelease();
    }else{
        fireandcomman *fire = new fireandcomman(m_info.armyId, m_hireNum,FORT_FIRE);
        fire->sendAndRelease();
    }
    m_hireNum = 0;
    m_trainSlider->setValue(1.0f * m_hireNum / m_info.free);
    
}

void SoldierMoreInfoView::onAddClick(CCObject * pSender, Control::EventType pCCControlEvent){
    if(m_index < 0 || m_index >= m_Idvect.size()){
        return;
    }
    std::string ind = m_Idvect.at(m_index);
    //更新数据
    auto m_info = GlobalData::shared()->armyList[ind];
    float value  = m_trainSlider->getValue() * m_info.free+1;
    if(m_info.free<=0){
        m_trainSlider->setValue(0.0);
    }else
        m_trainSlider->setValue(1.0f * value / m_info.free);
}
void SoldierMoreInfoView::onSubClick(CCObject * pSender, Control::EventType pCCControlEvent){
    if(m_index < 0 || m_index >= m_Idvect.size()){
        return;
    }
    std::string ind = m_Idvect.at(m_index);
    //更新数据
    auto m_info = GlobalData::shared()->armyList[ind];
    float value = m_trainSlider->getValue() * m_info.free-1;
    if(m_info.free<=0){
        m_trainSlider->setValue(0.0);
    }else
        m_trainSlider->setValue(1.0f * value / m_info.free);
}

void SoldierMoreInfoView::sliderShow(){
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

    
    m_trainSlider = CCSliderBar::createSlider(m_sliderBg, proSp, thuSp);
    m_trainSlider->setMinimumValue(0.0f);
    m_trainSlider->setMaximumValue(1.0f);
    m_trainSlider->setProgressScaleX(sliderW/proSp->getContentSize().width);
    m_trainSlider->setPosition(ccp(-33, -49));

    m_trainSlider->setTag(1);
    m_trainSlider->setLimitMoveValue(15);
    m_trainSlider->setTouchPriority(Touch_Popup);
    m_trainSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(SoldierMoreInfoView::sliderCallBack), CCControlEventValueChanged);
    m_sliderContainer->addChild(m_trainSlider, 1);
    
}


void SoldierMoreInfoView::sliderCallBack(CCObject*sender,CCControlEvent even){
    if(m_index < 0 || m_index >= m_Idvect.size()){
        return;
    }
    std::string ind = m_Idvect.at(m_index);
    //更新数据
    auto m_info = GlobalData::shared()->armyList[ind];
    m_hireNum = m_trainSlider->getValue() * m_info.free;
    refreshNum();
}

bool SoldierMoreInfoView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (m_jieguNode->isVisible() && !isTouchInside(m_bgColorLayerS, pTouch))
    {
        if (m_editOpen)
        {
            m_editOpen = false;
            return false;
        }
        return true;
    }
    else if(isTouchInside(m_toLeft,pTouch) && m_leftenable){
        return true;
    }
    else if(isTouchInside(m_toRight,pTouch)&& m_rightenable){
        return true;
    }
    else if (isTouchInside(m_jieguSp, pTouch))
    {
        return true;
    }
//    else if (!isTouchInside(m_touchNode, pTouch))
//    {
//        return true;
//    }
    m_editOpen = false;
    return false;
}

void SoldierMoreInfoView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (m_jieguNode->isVisible() && !isTouchInside(m_bgColorLayerS, pTouch))
    {
        m_jieguNode->setVisible(false);
    }
    else if (isTouchInside(m_toLeft, pTouch))
    {
        onToLeft(NULL, CCControlEventTouchUpInside);
    }
    else if (isTouchInside(m_toRight, pTouch))
    {
        onToRight(NULL, CCControlEventTouchUpInside);
    }
    else if (isTouchInside(m_jieguSp, pTouch))
    {
        m_jieguNode->setVisible(true);
        m_trainSlider->setValue(0.0);
        refreshNum();
    }
}

void SoldierMoreInfoView::arrowState(){
    if(m_index <= 0 ){
//        m_toLeft->setVisible(false);
        CCCommonUtils::setSpriteGray(m_toLeft, true);
        CCCommonUtils::setSpriteGray(m_toRight, false);
//        m_toRight->setVisible(true);
        m_leftenable = false;
        m_rightenable = true;
        return;
    }
    if (m_index >= m_Idvect.size()-1) {
//        m_toLeft->setVisible(true);
//        m_toRight->setVisible(false);
        CCCommonUtils::setSpriteGray(m_toRight, true);
        CCCommonUtils::setSpriteGray(m_toLeft, false);
        m_leftenable = true;
        m_rightenable = false;
        return;
    }
//    m_toLeft->setVisible(true);
//    m_toRight->setVisible(true);
    CCCommonUtils::setSpriteGray(m_toRight, false);
    CCCommonUtils::setSpriteGray(m_toLeft, false);
    m_rightenable = true;
    m_leftenable = true;
    return;
}
void SoldierMoreInfoView::soldierIconShow(){
    CCPoint pos = ccp(0,0);
    m_iconSp->setPosition(pos);
    m_iconSp->setDisplayFrame(CCLoadSprite::loadResource(m_info->getBodyIcon().c_str()));
//    m_soldierNode->addChild(m_iconSp);
}

void SoldierMoreInfoView::textAndDatashow(){
    m_titlename->setString(_lang("102271").c_str());
    m_subtitlename1->setString(m_info->getName());
    m_subtitlename2->setString(_lang("160112").c_str());
    m_subtitlename3->setString(_lang("103698").c_str());
    //////各项属性
    m_fightTxt->setString((_lang("105034")+": ").c_str());
    CCString *powerStr = CCString::createWithFormat("%.1f",m_info->power);
    m_fightValue->setString(powerStr->getCString());
    m_attackTxt->setString((_lang("103600")+": ").c_str());
    m_attackValue->setString(CC_CMDITOA(m_info->attack));
    m_defTxt->setString((_lang("103601")+": ").c_str());
    m_defValue->setString(CC_CMDITOA( m_info->defence));
    m_lifeTxt->setString((_lang("103605")+": ").c_str());
    m_lifeValue->setString(CC_CMDITOA( m_info->health));
    m_speedTxt->setString((_lang("103602")+": ").c_str());
    m_speedValue->setString(CC_CMDITOA( m_info->speed));
    m_weihtTxt->setString((_lang("103604")+": ").c_str());
    m_weihtValue->setString(CC_CMDITOA(ArmyController::getInstance()->getLoad(m_info)));
    m_rangeTxt->setString((_lang("103603")+": ").c_str());
    int range = m_info->range/10;
    range = MAX(range, 1);
    m_rangeValue->setString(CC_CMDITOA(range));
    m_upkeepTxt->setString((_lang("103606")+": "));
    m_upkeepValue->setString(CCString::createWithFormat("%.2f", m_info->upkeep)->getCString());
    //////数目
    m_curnum->setString(CC_CMDITOA(m_info->free));
    
}

void SoldierMoreInfoView::skillDatacellSet(){
    int curX = 0;
    int curY = 0;
    int _itemH = 107;
    if(m_index < 0 || m_index >= m_Idvect.size()){
        return;
    }
    std::string ind = m_Idvect.at(m_index);
    m_scrollView->cleanup();
    FunBuildInfo& info = FunBuildController::getInstance()->getFunbuildById(m_buildingId);
    //cell1
    int starLvRes = std::atoi(GlobalData::shared()->soldierskillStar.c_str());
    if(info.level >= starLvRes){
        auto item1 = SoldierSkillCell::create(ind,m_buildingId,3,m_infoList);
        item1->setPosition(ccp(curX, curY));
        m_scrollView->addChild(item1);
        curY += _itemH;
    }
    //cell2
    auto item2 = SoldierSkillCell::create(ind,m_buildingId,2,m_infoList);
    item2->setPosition(ccp(curX, curY));
    m_scrollView->addChild(item2);
    curY += _itemH;
    //cell3
    auto item3 = SoldierSkillCell::create(ind,m_buildingId,1,m_infoList);
    item3->setPosition(ccp(curX, curY));
    m_scrollView->addChild(item3);
    curY += _itemH;
    //cell4
    auto item4 = SoldierSkillCell::create(ind,m_buildingId,0,m_infoList);
    item4->setPosition(ccp(curX, curY));
    m_scrollView->addChild(item4);
    curY += _itemH;
    //设置显示位置
    m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,curY));
    m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - curY));
}

void SoldierMoreInfoView::editBoxTextChanged(CCEditBox* editBox, const std::string& text){
    std::string temp  =text;
    size_t pos = temp.find_first_not_of("1234567890");
    while (pos!=std::string::npos) {
        temp.replace(pos, 1, "");
        pos = temp.find_first_not_of("1234567890");
    }
    int a = atoi(temp.c_str());
    std::string aaaa = CC_CMDITOA(a);
    editBox->setText(aaaa.c_str());
}

void SoldierMoreInfoView::editBoxReturn(CCEditBox *editBox){
    if(m_index < 0 || m_index >= m_Idvect.size()){
        return;
    }
    std::string ind = m_Idvect.at(m_index);
    //更新数据
    auto m_info = GlobalData::shared()->armyList[ind];
    string numStr = m_editBox->getText();
    if(!numStr.empty()){
        size_t pos = numStr.find(",");
        while(pos!=string::npos){
            numStr.replace(pos, 1, "");
            pos=numStr.find(",");
        }
    }
    int num = atoi(numStr.c_str());
    if(num>m_info.free){
        num = m_info.free;
        
    }else if(num<0){
        num=0;
    }
    m_hireNum = num;
    if(m_info.free<=0){
        m_trainSlider->setValue(0.0);
    }else
        m_trainSlider->setValue(1.0f * num / m_info.free);
    if(m_info.free<=0){
        m_trainSlider->setTouchEnabled(false);
    }else{
        m_trainSlider->setTouchEnabled(true);
    }
    //    refreshNum();
}

void SoldierMoreInfoView::refreshNum(CCObject* p){
    if(m_index < 0 || m_index >= m_Idvect.size()){
        return;
    }
    std::string ind = m_Idvect.at(m_index);
    //更新数据
    auto m_info = GlobalData::shared()->armyList[ind];
    if(m_hireNum>m_info.free){
        m_hireNum = m_info.free;
    }
    m_editBox->setText(CC_CMDITOA(m_hireNum).c_str());
    if(m_hireNum>0){
        m_okBtn->setEnabled(true);
    }else{
        m_okBtn->setEnabled(false);
    }
    m_curnum->setString(CC_CMDITOA(m_info.free));
    string maxStr = "/"+CC_CMDITOA(m_info.free);
    //    m_numMaxText->setString(maxStr);
    if(m_info.free<=0){
        m_trainSlider->setTouchEnabled(false);
    }else{
        m_trainSlider->setTouchEnabled(true);
    }
}

void SoldierMoreInfoView::onEnter(){
//    UIComponent::getInstance()->hideReturnBtn();
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SoldierMoreInfoView::refreshNum), ARMY_NUM_CHANGE, NULL);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    setSwallowsTouches(true);
//    int index = m_buildingId/1000000 - 423 + 200;
//    CCLoadSprite::doResourceByCommonIndex(index, true);
    soldierIconShow();
}

void SoldierMoreInfoView::onExit(){
//    int index = m_buildingId/1000000 - 423 + 200;
//    CCLoadSprite::doResourceByCommonIndex(index, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, ARMY_NUM_CHANGE);
    setTouchEnabled(false);
    CCNode::onExit();
}

void SoldierMoreInfoView::onToLeft(CCObject * pSender, Control::EventType pCCControlEvent){
//    if(m_index < 0){
//        m_toLeft->setVisible(false);
//        return;
//    }
    if(m_index < 0 || m_index >= m_Idvect.size()){
        return;
    }
    if (m_index != 0){
        m_index--;
    }
    refreshData(m_index);
    arrowState();
}

void SoldierMoreInfoView::onToRight(CCObject * pSender, Control::EventType pCCControlEvent){
//    if(m_index >= m_Idvect.size()){
//        m_toLeft->setVisible(false);
//        return;
//    }
    if(m_index < 0 || m_index >= m_Idvect.size()){
        return;
    }
    if (m_index + 1 != m_Idvect.size()) {
        m_index++;
    }
    refreshData(m_index);
    arrowState();
}

void SoldierMoreInfoView::refreshData(int index){
    if(m_index < 0 || m_index >= m_Idvect.size()){
        return;
    }
    std::string ind = m_Idvect.at(index);
    //更新数据
    auto tempInfo = GlobalData::shared()->armyList[ind];
    m_subtitlename1->setString(tempInfo.getName());
    m_fightTxt->setString((_lang("105034")+": ").c_str());
    CCString *powerStr = CCString::createWithFormat("%.1f",tempInfo.power);
    m_fightValue->setString(powerStr->getCString());
    m_attackTxt->setString((_lang("103600")+": ").c_str());
    m_attackValue->setString(CC_CMDITOA(tempInfo.attack));
    m_defTxt->setString((_lang("103601")+": ").c_str());
    m_defValue->setString(CC_CMDITOA( tempInfo.defence));
    m_lifeTxt->setString((_lang("103605")+": ").c_str());
    m_lifeValue->setString(CC_CMDITOA( tempInfo.health));
    m_speedTxt->setString((_lang("103602")+": ").c_str());
    m_speedValue->setString(CC_CMDITOA( tempInfo.speed));
    m_weihtTxt->setString((_lang("103604")+": ").c_str());
    m_weihtValue->setString(CC_CMDITOA(ArmyController::getInstance()->getLoad(&tempInfo)));
    m_rangeTxt->setString((_lang("103603")+": ").c_str());
    int range = tempInfo.range/10;
    range = MAX(range, 1);
    m_rangeValue->setString(CC_CMDITOA(range));
    m_upkeepTxt->setString((_lang("103606")+": "));
    m_upkeepValue->setString(CCString::createWithFormat("%.2f", tempInfo.upkeep)->getCString());
    //////数目
    m_curnum->setString(CC_CMDITOA(tempInfo.free));
    
    //////士兵图像
    CCPoint pos = ccp(0,0);
//    m_iconSp->cleanup();
    m_iconSp->initWithSpriteFrame(CCLoadSprite::loadResource(tempInfo.getBodyIcon().c_str()));
    m_iconSp->setPosition(pos);
//    m_iconSp->setDisplayFrame(CCLoadSprite::loadResource(tempInfo.getBodyIcon().c_str()));
    ///
    skillDatacellSet();
}

void SoldierMoreInfoView::onCloseClick(CCObject * pSender, Control::EventType pCCControlEvent){
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TROOP_NUMS_REFRESH);
    PopupViewController::getInstance()->goBackPopupView();
//    this->closeSelf();
}

SEL_CCControlHandler SoldierMoreInfoView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseClick", SoldierMoreInfoView::onCloseClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOkclick", SoldierMoreInfoView::onOkclick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSubClick", SoldierMoreInfoView::onSubClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddClick", SoldierMoreInfoView::onAddClick);
    return NULL;
}

bool SoldierMoreInfoView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_midBG", CCScale9Sprite*, this->m_midBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_jieguBG", CCScale9Sprite*, this->m_jieguBG);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightbar", CCScale9Sprite*, this->m_rightbar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftbar", CCScale9Sprite*, this->m_leftbar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomSp", CCSprite*, this->m_bottomSp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_topNode", CCNode*, this->m_topNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_BG", CCScale9Sprite*, this->m_BG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_okBtn", CCControlButton*, this->m_okBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editBoxNode", CCNode*, this->m_editBoxNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_closeBtn", CCControlButton*, this->m_closeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titlename", CCLabelIF*, this->m_titlename);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subtitlename1", CCLabelIF*, this->m_subtitlename1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subtitlename2", CCLabelIF*, this->m_subtitlename2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subtitlename3", CCLabelIF*, this->m_subtitlename3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_attackTxt", CCLabelIF*, this->m_attackTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_attackValue", CCLabelIF*, this->m_attackValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defTxt", CCLabelIF*, this->m_defTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defValue", CCLabelIF*, this->m_defValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lifeTxt", CCLabelIF*, this->m_lifeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lifeValue", CCLabelIF*, this->m_lifeValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rangeTxt", CCLabelIF*, this->m_rangeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rangeValue", CCLabelIF*, this->m_rangeValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_speedTxt", CCLabelIF*, this->m_speedTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_speedValue", CCLabelIF*, this->m_speedValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_weihtTxt", CCLabelIF*, this->m_weihtTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_weihtValue", CCLabelIF*, this->m_weihtValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upkeepTxt", CCLabelIF*, this->m_upkeepTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upkeepValue", CCLabelIF*, this->m_upkeepValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fightValue", CCLabelIF*, this->m_fightValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fightTxt", CCLabelIF*, this->m_fightTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_curnum", CCLabelIF*, this->m_curnum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconSp", CCSprite*, this->m_iconSp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_soldierNode", CCNode*, this->m_soldierNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_jieguNode", CCNode*, this->m_jieguNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgColorLayerS", CCLayerColor*, this->m_bgColorLayerS);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_jieguSp", CCSprite*, this->m_jieguSp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_toLeft", CCSprite*, this->m_toLeft);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_toRight", CCSprite*, this->m_toRight);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sliderContainer", CCNode*, this->m_sliderContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_starlvNode", CCNode*, this->m_starlvNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_starlvbel", CCLabelIF*, this->m_starlvbel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pic_bg", CCSprite*, this->m_pic_bg);


    return false;
}

///////////////soldierskill//////////////////////////////
SoldierSkillCell* SoldierSkillCell::create(string ind,int buildingId,int index,CCNode* clickNode)
{
    auto ret = new SoldierSkillCell(ind,buildingId,index,clickNode);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool SoldierSkillCell::init()
{
    CCBLoadFile("SoldierSkillCell",this,this);
    //加载技能的按钮图标
    CCLoadSprite::doResourceByCommonIndex(502, true);
    setCleanFunction([](){
        //                CCLoadSprite::doResourceByCommonIndex(index, false);
        CCLoadSprite::doResourceByCommonIndex(502, false);
    });
    getIconButton();
    //加载dialog描述
    getSkillDescrip();
    //升星模式
    if(m_index == 3){
        starMode();
    }
    bool ret = true;
    return ret;
}


void SoldierSkillCell::getSkillDescrip(){
    int tag = m_index;
    auto m_info = GlobalData::shared()->armyList[m_ind];
    vector<string> descVec;
    m_info.getAttrDesc(tag, descVec);
    string title = _lang(descVec.at(0));
    string info = _lang(descVec.at(1));
    string buildId =m_info.getBuildId();
    int bid = FunBuildController::getInstance()->getMaxLvBuildByType( atoi(buildId.c_str()) );
    auto &m_buildInfo = FunBuildController::getInstance()->getFunbuildById(bid);
    if(m_buildInfo.starNum >= 1){
        vector<string> LvVect;
        m_info.getLvInfo(m_index, m_buildInfo.starNum, LvVect);
        string lvstr = LvVect.at(0);
        if(std::atoi(lvstr.c_str())){
            title.append(" ");
            title.append("Lv.");
            title.append(lvstr);
        }
    }

    m_skilldes->setString(info);
    m_lvname->setString(title);
}

void SoldierSkillCell::starMode()
{
    auto m_info = GlobalData::shared()->armyList[m_ind];
    string buildId =m_info.getBuildId();
    int bid = FunBuildController::getInstance()->getMaxLvBuildByType( atoi(buildId.c_str()) );
    auto &m_buildInfo = FunBuildController::getInstance()->getFunbuildById(bid);
    //换肤
    bool bhasGetFiveStar = m_buildInfo.starNum >= 5;
    if(!bhasGetFiveStar){
        changeSkin();
    }
}

void SoldierSkillCell::changeSkin()
{
    this->m_mid->setVisible(true);
    this->m_up->setVisible(true);
    this->m_Lock->setVisible(true);
}

void SoldierSkillCell::getIconButton() {
    auto m_info = GlobalData::shared()->armyList[m_ind];
    auto setBtnSp = [&](CCControlButton* btn){
        string iconName = m_info.getAttrIcon(this->m_index);
        CCLOG("iconName:%s", iconName.c_str());
        btn->setVisible(true);
        if (iconName == "")
        {
            btn->setVisible(false);
            return;
        }
        iconName.append(".png");
        auto tmpIcon = CCLoadSprite::createSprite(iconName.c_str());
        CCCommonUtils::setButtonSprite(btn, iconName.c_str());
        btn->setPreferredSize(tmpIcon->getContentSize());
    };
    setBtnSp(m_infoBtn);
    return;
}

void SoldierSkillCell::onEnter() {
    CCNode::onEnter();
    setSwallowsTouches(false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
}

void SoldierSkillCell::onExit() {
    setTouchEnabled(false);
    CCNode::onExit();
}

void SoldierSkillCell::onInfoDetailClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    infoShow();
}

void SoldierSkillCell::infoShow(){
    auto m_info = GlobalData::shared()->armyList[m_ind];
    vector<string> descVec;
    vector<string> NextlvVec;
    m_info.getAttrDesc(this->m_index, descVec);
    string title = _lang(descVec.at(0));
    string info = _lang(descVec.at(1));
    string buildId =m_info.getBuildId();
    int bid = FunBuildController::getInstance()->getMaxLvBuildByType( atoi(buildId.c_str()) );
    auto &m_buildInfo = FunBuildController::getInstance()->getFunbuildById(bid);
    m_info.getNextLvInfo(m_index, m_buildInfo.starNum, NextlvVec);
    string nextlv = NextlvVec.at(0);
    if (title == "" ||info == "" || nextlv == "") {
        return;
    }
    if(m_index == 3 &&m_buildInfo.starNum < 5){
        //弹出提示框
        auto m_info = GlobalData::shared()->armyList[m_ind];
        vector<string> star_openLvVect;
        m_info.getNextLvInfo(3, 0, star_openLvVect);
        int m_lv = std::atoi(star_openLvVect.at(0).c_str());
        CCCommonUtils::flyHintWithDefault("", "", _lang_1("160114", CC_ITOA(m_lv)));
    }else{
        PopupViewController::getInstance()->addPopupView(TipsNewView::create(title,info,m_buildingId,nextlv));
    }

}

bool SoldierSkillCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    pt = pTouch->getLocation();
    if (isTouchInside(m_touchArea, pTouch))
    {
        return true;
    }
    return false;
}

void SoldierSkillCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCPoint pt1 = pTouch->getLocation();
    float dis = abs(pt.y - pt1.y);
    if (isTouchInside(m_touchArea, pTouch) && dis < 30) {
        infoShow();
    }
}

SEL_CCControlHandler SoldierSkillCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInfoDetailClick", SoldierSkillCell::onInfoDetailClick);
    return NULL;
}

bool SoldierSkillCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvname", CCLabelIF*, m_lvname);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_skilldes", CCLabelIF*, m_skilldes);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoBtn", CCControlButton*, this->m_infoBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mid", CCScale9Sprite*, this->m_mid);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_up", CCScale9Sprite*, this->m_up);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_Lock", CCSprite*, this->m_Lock);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchArea", CCNode*, this->m_touchArea);
    return false;
}

