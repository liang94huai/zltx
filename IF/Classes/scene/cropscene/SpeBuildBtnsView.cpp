//
//  SpeBuildBtnsView.cpp
//  IF
//
//  Created by fubin on 15/3/11.
//
//

#include "SpeBuildBtnsView.h"
#include "SceneController.h"
#include "SoundController.h"
#include "PopupViewController.h"
#include "FunBuildController.h"
#include "QueueController.h"
#include "ToolController.h"
#include "WorldController.h"
#include "ImperialScene.h"
#include "YesNoDialog.h"
#include "LotteryController.h"
#include "LotteryAct2View.h"
#include "LotteryActView.h"
#include "ChatServiceCocos2dx.h"
#include "DailyActiveController.h"
#include "DailyActiveView.h"
#include "PortActView.h"
#include "MonthCardView.h"
#include "ActivityController.h"
#include "TrainingView.h"
#include "LongJingStoreView.h"
#include "InviteForGiftView.h"
#include "FriendsView.h"
#include "EquipLongjingView.hpp"

SpeBuildBtnsView* SpeBuildBtnsView::create()
{
    SpeBuildBtnsView *pRet = new SpeBuildBtnsView();
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

bool SpeBuildBtnsView::init()
{
    CCBLoadFile("BuildBtnView",this,this);
    m_name1->setFntFile("Arial_Bold_Border.fnt");
    m_name2->setFntFile("Arial_Bold_Border.fnt");
    m_name3->setFntFile("Arial_Bold_Border.fnt");
    m_name4->setFntFile("Arial_Bold_Border.fnt");
    m_msg5->setFntFile("Arial_Bold_Border.fnt");
    m_goldLabel->setFntFile("Arial_Bold_Border.fnt");
    m_name5->setFntFile("Arial_Bold_Border.fnt");
    m_msgV5->setFntFile("Arial_Bold_Border.fnt");
    m_msg5->setFntFile("Arial_Bold_Border.fnt");
    
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(SpeBuildBtnsView::animationFunc));
    this->setScale(1.2);
    return true;
}

void SpeBuildBtnsView::onHide()
{
    m_initEnd = false;
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
}

void SpeBuildBtnsView::onShow(int buildId)
{
    if(buildId == SPE_BUILD_SHIP)
    {
        //码头 先  liu -->todo
        // 判断 签到是否领取
        if (PortActController::getInstance()->m_isRdLoginDay == 1) //0 今天没领 1今天已经领了
        {
            CCCommonUtils::flyHint("", "", _lang("new100041"));

        }else{
            //弹页面
            if (PortActController::getInstance()->m_loginDayMap.size() <= 0) {
                PortActController::getInstance()->startGetDailyRwdData();
                CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ActivityController::showDailyRwd), PORT_DATA_INIT, NULL);
            }else{
                ActivityController::getInstance()->showDailyRwd(nullptr);
            }
        }
        
        return;
    }
    m_buildingKey = buildId;
    m_toolNode->setVisible(false);
    
    m_nameLabel->setString("");
    m_name1->setString("");
    m_name2->setString("");
    m_name3->setString("");
    m_name4->setString("");
    m_name5->setString("");
    m_msgV5->setString("");
    m_icon1Node->removeAllChildren();
    m_icon2Node->removeAllChildren();
    m_icon3Node->removeAllChildren();
    m_icon4Node->removeAllChildren();
    m_icon5Node->removeAllChildren();
    
    m_btn1Node->setPosition(ccp(-156, 30));
    m_btn2Node->setPosition(ccp(-52, -10));
    m_btn3Node->setPosition(ccp(52, -10));
    m_btn4Node->setPosition(ccp(156, 30));
    m_btn5Node->setPosition(ccp(259, 107));
    
    m_spdNode->setVisible(false);
    
    m_btn1->setTouchPriority(Touch_Imperial_City);
    m_btn2->setTouchPriority(Touch_Imperial_City);
    m_btn3->setTouchPriority(Touch_Imperial_City);
    m_btn4->setTouchPriority(Touch_Imperial_City);
    m_btn5->setTouchPriority(Touch_Imperial_City);
    
    int cellCnt = 2;
    vector<string> btnIcons;
    m_funKeyVec.clear();
    bool bWrap = false;
    const int btnW = 110;
    m_nameNode->setPosition(ccp(0,300));
    if (m_buildingKey == SPE_BUILD_GOLD_BOX)
    {
        m_nameLabel->setString(_lang("102375"));
        if (ActivityController::getInstance()->m_isTrialOpen == 1)//试炼场
        {
            btnIcons.push_back("training_enter.png");
            m_funKeyVec.push_back("137525");
        }
        
        if (DailyActiveController::shared()->getDailyActiveOpenFlag() == 1) {//日常活动功能开放
            btnIcons.push_back("dailyActive_icon.png");
            m_funKeyVec.push_back("133190");
        }
        
        if (false && GlobalData::shared()->isOpenForum) {//论坛
            btnIcons.push_back("icon_forum.png");
            m_funKeyVec.push_back("105329");
        }
        if (!GlobalData::shared()->isXiaoMiPlatForm()) {//宝箱 国内平台，宝箱不显示
            btnIcons.push_back("icon_binding_FB.png");
            m_funKeyVec.push_back("107050");
        }
        
        if (LotteryController::shared()->isLotteryOn() && LotteryController::shared()->getOpenFlag2() == 1 && GlobalData::shared()->analyticID != "common") {//转盘打开
            btnIcons.push_back("icon_roulette_s.png");
            m_funKeyVec.push_back("111100");
        }
        
        //龙晶商店开关2
        string sw=GlobalData::shared()->dragonglass_control;
        bool isOpenLongJing = false;
        if (sw==""||sw=="0") {
            isOpenLongJing=false;
        }else if (sw=="1"){
            isOpenLongJing=true;
        }else {
            if (CCCommonUtils::checkVersion(sw)){
                isOpenLongJing=true;
            }else{
                isOpenLongJing=false;
            }
        }
        if (isOpenLongJing&&FunBuildController::getInstance()->getMainCityLv()>=15)//龙晶商店(试炼场)
        {
            btnIcons.push_back("icon_longJingShop.png");
            m_funKeyVec.push_back("111652");//水晶商城
        }
        
        bWrap = true;
    }else if( m_buildingKey == SPE_BUILD_SHIP){
        m_nameNode->setPosition(ccp(0, 280));
        m_nameLabel->setString(_lang("101023"));
//        btnIcons.push_back("monthcard_icon.png");
//        m_funKeyVec.push_back("101283");
        btnIcons.push_back("qiandao_icon.png");
        m_funKeyVec.push_back("133187");
        bWrap = true;
    }
    else if (m_buildingKey == SPE_BUILD_XIONGDI)
    {
        m_nameLabel->setString(_lang("105358"));
//        btnIcons.push_back("Friend_xiongdi_icon.png");
//        m_funKeyVec.push_back("105037");
        btnIcons.push_back("Friend_list_icon_new.png");
        m_funKeyVec.push_back("132103");
    }
    else if( m_buildingKey == SPE_BUILD_QIJI){
        // TODO...
        m_nameNode->setPosition(ccp(0, 280));  //--sun3
//        m_nameLabel->setString(_lang("101023"));
//        btnIcons.push_back("build_info_icon.png");
//        m_funKeyVec.push_back("1003");   //--sun3
//        btnIcons.push_back("newrd_icon.png");
//        m_funKeyVec.push_back("1004");
//        bWrap = true;
    }
    
    m_btn1Node->setVisible(false);
    m_btn1->setEnabled(false);
    if (btnIcons.size() >= 1) {
        auto icon = CCLoadSprite::createSprite(btnIcons[0].c_str());
        CCCommonUtils::setSpriteMaxSize(icon, 90, true);
        m_icon1Node->addChild(icon);
        m_btn1Node->setVisible(true);
        m_btn1->setEnabled(true);
        if(bWrap){
            m_name2->setDimensions(CCSize(btnW, 0));
        }else{
            m_name2->setDimensions(CCSize(0, 0));
        }
        m_name2->setString(_lang(m_funKeyVec[0]));
        
        std::string defLan = LocalController::shared()->getLanguageFileName();
        if(m_funKeyVec[0]=="133190" && defLan=="fr"){//"日常奖励"在法语爆框
            m_name2->setDimensions(CCSize(btnW+10, 0));
            m_name2->setFontSize(18);
        }
        
        cellCnt=1;
    }
    
    m_btn2Node->setVisible(false);
    m_btn2->setEnabled(false);
    if (btnIcons.size() >= 2) {
        auto icon = CCLoadSprite::createSprite(btnIcons[1].c_str());
        CCCommonUtils::setSpriteMaxSize(icon, 90, true);
        m_icon2Node->addChild(icon);
        m_btn2Node->setVisible(true);
        m_btn2->setEnabled(true);
        if(bWrap){
            m_name1->setDimensions(CCSize(btnW, 0));
        }else{
            m_name1->setDimensions(CCSize(0, 0));
        }
        m_name1->setString(_lang(m_funKeyVec[1]));
        cellCnt=2;
    }
    
    m_btn3Node->setVisible(false);
    m_btn3->setEnabled(false);
    if (btnIcons.size() >= 3) {
        auto icon = CCLoadSprite::createSprite(btnIcons[2].c_str());
        CCCommonUtils::setSpriteMaxSize(icon, 90, true);
        m_icon3Node->addChild(icon);
        m_btn3Node->setVisible(true);
        m_btn3->setEnabled(true);
        if(bWrap){
            m_name3->setDimensions(CCSize(btnW, 0));
        }else{
            m_name3->setDimensions(CCSize(0, 0));
        }
        m_name3->setString(_lang(m_funKeyVec[2]));
        cellCnt=3;
    }
    
    m_btn4Node->setVisible(false);
    m_btn4->setEnabled(false);
    if (btnIcons.size() >= 4) {
        auto icon = CCLoadSprite::createSprite(btnIcons[3].c_str());
        CCCommonUtils::setSpriteMaxSize(icon, 90, true);
        m_btn4Node->setVisible(true);
        m_btn4->setEnabled(true);
        cellCnt=4;
        if(bWrap){
            m_name4->setDimensions(CCSize(btnW, 0));
        }else{
            m_name4->setDimensions(CCSize(0, 0));
        }
        m_name4->setString(_lang(m_funKeyVec[3]));
        m_icon4Node->addChild(icon);
    }
    
    m_btn5Node->setVisible(false);
    m_btn5->setEnabled(false);
    if (btnIcons.size() >= 5) {
        auto icon = CCLoadSprite::createSprite(btnIcons[4].c_str());
        CCCommonUtils::setSpriteMaxSize(icon, 90, true);
        m_btn5Node->setVisible(true);
        m_btn5->setEnabled(true);
        cellCnt=5;
        if(bWrap){
            m_name5->setDimensions(CCSize(btnW, 0));
        }else{
            m_name5->setDimensions(CCSize(0, 0));
        }
        m_name5->setString(_lang(m_funKeyVec[4]));
        m_icon5Node->addChild(icon);
    }
    
    if (cellCnt==3) {
        m_btn1Node->setPosition(ccp(-107, 6));
        m_btn2Node->setPosition(ccp(0, -16));
        m_btn3Node->setPosition(ccp(107, 6));
    }
    else if (cellCnt==2) {
        m_btn1Node->setPosition(ccp(-52, -10));
        m_btn2Node->setPosition(ccp(52, -10));
    }
    else if (cellCnt==4) {
        m_btn1Node->setPosition(ccp(-156, 30));
        m_btn2Node->setPosition(ccp(-52, -10));
        m_btn3Node->setPosition(ccp(52, -10));
        m_btn4Node->setPosition(ccp(156, 30));
    }
    else if (cellCnt==1) {
        m_btn1Node->setPosition(ccp(0, -16));
    }
    else if (cellCnt==5) {
        m_btn1Node->setPosition(ccp(-156, 30));
        m_btn2Node->setPosition(ccp(-52, -10));
        m_btn3Node->setPosition(ccp(52, -10));
        m_btn4Node->setPosition(ccp(156, 30));
        m_btn5Node->setPosition(ccp(259, 107));
        if(m_buildingKey == SPE_BUILD_GOLD_BOX){//龙晶商店
            m_btn2Node->setPosition(ccp(-0, -16));
            m_btn3Node->setPosition(ccp(-107, 6));
            m_btn1Node->setPosition(ccp(107, 6));
            m_btn4Node->setPosition(ccp(200, 60));
            m_btn5Node->setPosition(ccp(-200, 60));
            m_name5->setVisible(false);
            m_msg5->setString(_lang(m_funKeyVec[4]));
        }
    }
    
    
    m_btn1Node->removeChildByTag(110);
    if (m_buildingKey == SPE_BUILD_GOLD_BOX && DailyActiveController::shared()->getDailyActiveOpenFlag()==1 && DailyActiveController::shared()->canGetReward())
    {
        CCNode* particleNode = CCNode::create();
        m_btn1Node->addChild(particleNode, 50, 110);
        string tmpStart = "QueueGlow_";
        int count = 2;
        for (int i=1; i<=count; i++)
        {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
            particle->setPositionType(kCCPositionTypeRelative);
            particleNode->addChild(particle);
        }
    }
    
    m_btnNum = cellCnt;
    this->setVisible(true);
    onShowInfo();
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
}

CCNode* SpeBuildBtnsView::getGuideNode(string _key)
{
    if (m_initEnd && this->isVisible() && m_toolNode->isVisible() && m_toolNode->getScale()>=1.0) {
        if(_key == "longjing") {
            guidKey = _key;
            return m_btn5;
        }
    }
    return NULL;
}


bool SpeBuildBtnsView::onShowInfo()
{
    m_toolNode->setVisible(true);
    return true;
}

void SpeBuildBtnsView::animationFunc()
{
    string name = this->getAnimationManager()->getLastCompletedSequenceName();
    if (name == "FadeOut") {
        this->setVisible(false);
    }
    else if (name == "FadeIn") {
        m_initEnd = true;
    }
}

void SpeBuildBtnsView::onEnter() {
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Imperial_City+1, true);
}

void SpeBuildBtnsView::onExit() {
    CCNode::onExit();
    setTouchEnabled(false);
}

bool SpeBuildBtnsView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (this->isVisible() && isTouchInside(m_touchNode, pTouch)) {
        return true;
    }
    return false;
}

void SpeBuildBtnsView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
}

void SpeBuildBtnsView::onEnterFrame(float dt)
{
}

SEL_MenuHandler SpeBuildBtnsView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

SEL_CCControlHandler SpeBuildBtnsView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn1Click", SpeBuildBtnsView::onBtn1Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn2Click", SpeBuildBtnsView::onBtn2Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn3Click", SpeBuildBtnsView::onBtn3Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn4Click", SpeBuildBtnsView::onBtn4Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn5Click", SpeBuildBtnsView::onBtn5Click);
    return NULL;
}

bool SpeBuildBtnsView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldLabel", CCLabelIF*, this->m_goldLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn1", CCControlButton*, this->m_btn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn2", CCControlButton*, this->m_btn2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn3", CCControlButton*, this->m_btn3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn4", CCControlButton*, this->m_btn4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn5", CCControlButton*, this->m_btn5);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn1Node", CCNode*, this->m_btn1Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn2Node", CCNode*, this->m_btn2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn3Node", CCNode*, this->m_btn3Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn4Node", CCNode*, this->m_btn4Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn5Node", CCNode*, this->m_btn5Node);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon1Node", CCNode*, this->m_icon1Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon2Node", CCNode*, this->m_icon2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon3Node", CCNode*, this->m_icon3Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon4Node", CCNode*, this->m_icon4Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon5Node", CCNode*, this->m_icon5Node);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_toolNode", CCNode*, this->m_toolNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spdNode", CCNode*, this->m_spdNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameNode", CCNode*, this->m_nameNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name1", CCLabelIF*, this->m_name1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name2", CCLabelIF*, this->m_name2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name3", CCLabelIF*, this->m_name3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name4", CCLabelIF*, this->m_name4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name5", CCLabelIF*, this->m_name5);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg5", CCLabelIF*, this->m_msg5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgV5", CCLabelIF*, this->m_msgV5);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    
    return false;
}

void SpeBuildBtnsView::onBtn1Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (guidKey!="") {
        onGuidClick();
        return;
    }
    onClickFunc(0);
}

void SpeBuildBtnsView::onBtn2Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (guidKey!="") {
        onGuidClick();
        return;
    }
    onClickFunc(1);
}

void SpeBuildBtnsView::onBtn3Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    
    if (guidKey!="") {
        onGuidClick();
        return;
    }
    onClickFunc(2);
}

void SpeBuildBtnsView::onBtn4Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (guidKey!="") {
        onGuidClick();
        return;
    }
    onClickFunc(3);
}

void SpeBuildBtnsView::onBtn5Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (guidKey!="") {
        onGuidClick();
        return;
    }
    onClickFunc(4);
}

void SpeBuildBtnsView::onGuidClick()
{
    if(guidKey=="longjing") {
        guidKey = "";
        onClickFunc(4);
    }
}

void SpeBuildBtnsView::onClickFunc(int idx)
{
    m_toolNode->setVisible(false);
    onHideShadow();
    
    if (m_funKeyVec.size() <= idx) {
        return;
    }
    
    string key = m_funKeyVec[idx];
    if (m_buildingKey == SPE_BUILD_GOLD_BOX)
    {
        if (key == "111100") {//转盘
            CCLOG("%s", GlobalData::shared()->fromCountry.c_str());
            if (GlobalData::shared()->fromCountry == "KR" || GlobalData::shared()->fromCountry == "kr")
            {//韩国玩家先进到这里受审
                if (LotteryController::shared()->getOpenFlag2_ko()==0)
                {
                    CCCommonUtils::flyHint("", "", _lang("111126"));
                    return;
                }
            }
            if (LotteryController::shared()->lotteryInfo.type == 2)
            {
                PopupViewController::getInstance()->addPopupInView(LotteryAct2View::create());
            }
            else
            {
                PopupViewController::getInstance()->addPopupInView(LotteryActView::create());
            }
        }else if (key == "107050") {//宝箱
        
        }else if (key == "105329") {//论坛
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
            ChatServiceCocos2dx::showForumFrom2dx(NULL);
#endif
        }else if (key == "133076"){
//            PopupViewController::getInstance()->addPopupInView(AprilFoolActView::create());
        }else if (key == "133190"){
            PopupViewController::getInstance()->addPopupInView(DailyActiveView::create());
        }else if (key == "137525") {
            PopupViewController::getInstance()->addPopupInView(Training1View::create());
        }else if (key == "111652"){//龙晶商店
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
//            PopupViewController::getInstance()->addPopupInView(LongJingStoreView::create());
            PopupViewController::getInstance()->addPopupInView(EquipLongjingView::create());
//            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("UI_goods"));
        }
    }else if(m_buildingKey == SPE_BUILD_SHIP){
//        if(key == "133187"){
//            PopupViewController::getInstance()->addPopupInView(PortActView::create());
//        }else if( key == "101283"){
//            PopupViewController::getInstance()->addPopupInView(MonthCardView::create());
//        }
        //屏bi //liu todo
    }else if(m_buildingKey == SPE_BUILD_QIJI){
//        if(key == "1003"){
//            PopupViewController::getInstance()->addPopupInView(MiracleView::create(423000010));
//        }else if( key == "1004"){
//            PopupViewController::getInstance()->addPopupInView(MiracleView::create(423000010));
//        }
    }
    else if (m_buildingKey == SPE_BUILD_XIONGDI)
    {
        if (key == "105037") {
//            PopupViewController::getInstance()->addPopupInView(InviteForGiftView::create());
        } else if (key == "132103") {
            PopupViewController::getInstance()->addPopupInView(FriendsView::create());
        }
    }
}

void SpeBuildBtnsView::onHideShadow(bool st)
{
    this->setVisible(false);
    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    if( layer )
    {
        layer->stopShadow(m_buildingKey,NULL,NULL);
        if (st) {
            layer->onResetLastBuildId();
        }
    }
}

void SpeBuildBtnsView::onHideBtn()
{
    m_toolNode->setVisible(false);
    onHideShadow(false);
}
