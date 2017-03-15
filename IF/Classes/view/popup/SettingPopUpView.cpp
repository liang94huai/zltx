//
//  SettingPopUpView.cpp
//  IF
//
//  Created by 李锐奇 on 14-1-20.
//
//

#include "SettingPopUpView.h"
#include "MailPopUpView.h"
#include "PopupViewController.h"
#include "SuggestionView.h"
#include "SetAccountView.h"
#include "ShieldUnlockPopUpView.h"
#include "PopupViewController.h"
#include "LogoutCommand.h"
#include "PushSettingView.h"
#include "HelpView.h"
#include "BattleManager.h"
#include "SoundController.h"
#include "HowToPlayView.h"
#include "LoginServiceView.h"
#include "ChangeServerView.h"
#include "SocialPopUpView.h"
#include "ServerListPopUpView.h"
#include "CDkeyPopUpView.h"
#include "MODView.h"
#include "MODContactView.h"
#include "LanguageSettingView.h"
#include "InviteForGiftView.h"
#include "CCFlagWaveSprite.h"
#include "Search.h"
#include "ChatServiceCocos2dx.h"
#include "InnerSettingView.h"
#include "../../Ext/CCDevice.h"
#include "ChatController.h"
#include "TranslateOptimizeCommand.h"
#include "SettingXmlCommand.hpp"

#ifdef  ios9

#include "ReplayKitsController.hpp"
#endif
bool IsQuitGame();
bool SettingPopUpView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_popupBG", CCScale9Sprite*, this->m_popupBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_Tips", CCLabelIF*, this->m_tips);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeText", CCLabelIF*, this->m_timeText);
    //CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeTitleText", CCLabelIF*, this->m_timeTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listContainer", CCNode*, this->m_listContainer);
    return true;
}

SEL_CCControlHandler SettingPopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    return NULL;
} 

SettingPopUpView* SettingPopUpView::create(){
    SettingPopUpView* ret = new SettingPopUpView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
    
}
bool SettingPopUpView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
   return true;
}
void SettingPopUpView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    m_touchCount++;
    if(m_touchCount>60){//用于测试一个ios的回调数据
        this->unschedule(schedule_selector(SettingPopUpView::onTimer));
        string deepLink = "deepLink=";
        deepLink += CCUserDefault::sharedUserDefault()->getStringForKey("defferedDeepLinkKey","");
        m_timeText->setString(deepLink);
        m_timeText->setDimensions(CCSize(620,200));
        CCUserDefault::sharedUserDefault()->setStringForKey("defferedDeepLinkKey","");
    }
    if(m_touchCount>40&&!m_showElex)
    {
        m_showElex=true;
        m_showSettings.push_back(Setting_Elex);
        m_tabView->reloadData();
    }
}
void SettingPopUpView::onEnter(){
    m_touchCount = 0;
    setTouchEnabled(true);
    CCLoadSprite::doResourceByCommonIndex(208, true);
    PopupBaseView::onEnter();
    setTitleName(_lang("101218"));
    m_tabView->setTouchEnabled(true);
    m_tabView->reloadData();
    onTimer(1.0);
    this->schedule(schedule_selector(SettingPopUpView::onTimer),1.0);
}

void SettingPopUpView::onExit(){
    m_touchCount = 0;
    setTouchEnabled(false);
    m_tabView->setTouchEnabled(false);
    GameController::getInstance()->removeWaitInterface();
    this->unschedule(schedule_selector(SettingPopUpView::onTimer));
    PopupBaseView::onExit();
}
void SettingPopUpView::onTimer(float dt){
    time_t t = GlobalData::shared()->getWorldTime();
    string timestr =_lang("105259")+" "+CCCommonUtils::timeStampToUTCDate(t);
    if (GlobalData::shared()->playerInfo.gmFlag == 1) {
        timestr += " xml:" + GlobalData::shared()->xmlVersion;
    }
    m_timeText->setString(timestr);
    time++;
}
SettingPopUpView::~SettingPopUpView()
{
    if(  IsQuitGame()== false)
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_POPUP_REMOVE,CCString::create(CC_ITOA(PVT_more)));
}

bool SettingPopUpView::init(){
    
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(308, true);
        CCLoadSprite::doResourceByCommonIndex(208, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(308, true);
            CCLoadSprite::doResourceByCommonIndex(208, false);
        });
        auto bg = CCBLoadFile("SettingView", this, this);
        this->setContentSize(bg->getContentSize());
        changeBGHeight(m_popupBG);
        changeBGHeight(m_bg);
        //m_timeTitleText->setString(_lang("105259"));
        m_tips->setString(_lang("101220").c_str());
        m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width,getBGHeight() - 60));
        GlobalData::shared()->isShowCDkey = false;


        m_tabView = CCMultiColTableView::create(this, m_listContainer->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setMultiColTableViewDelegate(this);
        m_tabView->setTouchPriority(1);
        m_listContainer->addChild(m_tabView);
        m_showElex = false;
        time = 0;
        
        int num = GlobalData::shared()->settingMap.size();
        if (num<=0) {
            SettingXmlCommand* cmd = new SettingXmlCommand();
            cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(SettingPopUpView::callBack), NULL));
            cmd->sendAndRelease();
            GameController::getInstance()->showWaitInterface();
        }else{
            generateShowSetting();
            m_tabView->reloadData();
        }
        ret = true;
    }
    return ret;
}

void SettingPopUpView::callBack(CCObject* param){
    GameController::getInstance()->removeWaitInterface();
    if (m_tabView!=NULL) {
        generateShowSetting();
        m_tabView->reloadData();
    }
}

CCNode* SettingPopUpView::getGuideNode(string _key)
{
    if (_key == "Set_bangding")
    {
        auto& arr = m_tabView->getContainer()->getChildren();
        for (int i=0; i<arr.size(); i++)
        {
            CCTableViewCell* view = dynamic_cast<CCTableViewCell*>(arr.at(i));
            if (view) {
                auto& cellGrideArr = view->getChildren();
                for (int j=0; j<cellGrideArr.size(); j++) {
                    SettingCell* cell = dynamic_cast<SettingCell*>(cellGrideArr.at(j));
                    if (cell && cell->m_index == Setting_BINDING) {
                        return cell->m_iconBG;
                    }
                }
            }
        }
    }
    return NULL;
}

void SettingPopUpView::generateShowSetting(){
    m_showSettings.clear();
    bool inviteFlag = GlobalData::shared()->isInviteOpen;
    bool activityFlag = false;
    for (auto it=GlobalData::shared()->settingMap.begin(); it!=GlobalData::shared()->settingMap.end(); it++) {
        bool pushFlag = it->second.show==1;
        if (it->second.gm==1 && GlobalData::shared()->playerInfo.gmFlag==1) {
            pushFlag = true;
        }else if(it->second.show==1){
            if(it->second.type == Setting_Forum){
                pushFlag = false;
                //#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
                if(GlobalData::shared()->isOpenForum && GlobalData::shared()->analyticID != "cn_dangle"){
                    pushFlag = true;
                }
                //#endif
            }
            else if(it->second.type == Setting_Translate_Optimization){
                pushFlag = false;
                if(GlobalData::shared()->isOpenTranslateOptimization){
                    pushFlag = true;
                }
            }
            else if(it->second.type == Setting_INVITE){
                if(!inviteFlag || GlobalData::shared()->isXiaoMiPlatForm() || GlobalData::shared()->analyticID == "cn1"){
                    pushFlag = false;
                }
            }
            else if(it->second.type == Setting_CDKEY){
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
                if(!activityFlag){
                    pushFlag = false;
                }
#endif
                pushFlag = false;
            }
            else if(it->second.type==Setting_Lobi){
                pushFlag = false;
                std::string language = LocalController::shared()->getLanguageFileName();
                if(language=="ja"){
                    pushFlag = true;
                }
            }else if(it->second.type == Setting_BINDING && GlobalData::shared()->cantSwtichAccount()){
                pushFlag = false;
            }else if(it->second.type==Setting_VK){
                pushFlag = false;
                std::string language = LocalController::shared()->getLanguageFileName();
                if(language=="ru"){
                    pushFlag = true;
                }
            }else if(it->second.type==Setting_WeiBo){
                pushFlag = false;
                std::string language = LocalController::shared()->getLanguageFileName();
                if(language=="zh_CN"){
                    pushFlag = true;
                    if(GlobalData::shared()->analyticID == "cn_ewan"
                       || GlobalData::shared()->analyticID == "cn_dangle"
                       || GlobalData::shared()->analyticID == "cn_uc"
                       ){
                        pushFlag = false;
                    }
                }
            }else if(it->second.type == Setting_lang || it->second.type == Setting_SOCIAL){
                if(GlobalData::shared()->isXiaoMiPlatForm()){
                    pushFlag = false;
                }
            }else if(it->second.type == Setting_XiaoMi){
                if(!GlobalData::shared()->isXiaoMiPlatForm()
                   || GlobalData::shared()->analyticID == "cn_nearme"
                   || GlobalData::shared()->analyticID == "cn_am"
                   || GlobalData::shared()->analyticID == "cn_ewan"
                   || GlobalData::shared()->analyticID == "cn_uc"
                   ){
                    pushFlag = false;
                }
            }else if(it->second.type == Setting_KO){
                pushFlag = false;
                std::string language = LocalController::shared()->getLanguageFileName();
                if(language=="ko"){
                    pushFlag = true;
                }
            }else if (it->second.type == Setting_Record){
                pushFlag = false;
                if(GlobalData::shared()->recordFlag == 1){
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
#ifdef  ios9
                    if(ReplayKitsController::getInstance()->isReplaySupport()){
                        pushFlag = true;
                    }
#endif
#endif
                }
            }
        }
        // 本地屏蔽功能
        if(it->second.type == Setting_Push || it->second.type == Setting_innerSetting || it->second.type == Setting_CDKEY)
        {
            pushFlag = true;
        }else{
            pushFlag = false;
        }
        if(pushFlag){
            m_showSettings.push_back(it->second.type);
        }
    }
    if(m_showSettings.size()>0){
        std::sort(m_showSettings.begin(),m_showSettings.end(),sortByRank);
    }
}

bool SettingPopUpView::sortByRank(int type1, int type2){
    auto it1 = GlobalData::shared()->settingMap.find(type1);
    auto it2 = GlobalData::shared()->settingMap.find(type2);
    return it1->second.rank<it2->second.rank;
    
}


void SettingPopUpView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
//    SettingCell* wCell = dynamic_cast<SettingCell*>(cell);
//    if(wCell == NULL){
//        return;
//    }
//    wCell->touchEvent();
}

cocos2d::CCSize SettingPopUpView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    if(CCCommonUtils::isIosAndroidPad())
    {
//        return CCSize(150*2.35, 180*2);
        return CCSize(150*1.9, 180*2);
    }
    return CCSize(150, 180);
}

CCTableViewCell* SettingPopUpView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, ssize_t idx){
    if(idx >= m_showSettings.size()){
        return NULL;
    }
    
    SettingCell* cell = (SettingCell*)table->dequeueGrid();
    int showIndex = m_showSettings[idx];
    if(cell){
        cell->setData(showIndex);
    }else{
        cell = SettingCell::create(showIndex);
    }
    return cell;
}

ssize_t SettingPopUpView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    int size = m_showSettings.size();
    if(CCCommonUtils::isIosAndroidPad())
    {
        return ceil(size * 1.0f / 5.0f);
    }
    return ceil(size * 1.0f / 4.0f);
}
ssize_t SettingPopUpView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){

    if(CCCommonUtils::isIosAndroidPad())
    {
        return 5;
    }
        return 4;
}

//setting cell
bool SettingCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
//    CCLOG("test name %s", pMemberVariableName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemName", CCLabelIF*, this->m_itemName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_HintNode", CCNode*, this->m_HintNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconBG", CCSprite*, this->m_iconBG);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_HintNum", CCLabelIF*, this->m_HintNum);
    return true;
}

SEL_CCControlHandler SettingCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    return NULL;
}

bool SettingCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_touchPoint = pTouch->getLocation();
    if (isTouchInside(this->m_iconBG, pTouch)) {
        return true;
    }
    return false;
}
void SettingCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (fabs(pTouch->getLocation().x-m_touchPoint.x) > 30 || fabs(pTouch->getLocation().y-m_touchPoint.y) > 30)
    {
        return;
    }
    touchEvent();
}
void SettingCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
}

SettingCell* SettingCell::create(unsigned int index){
    SettingCell* ret = new SettingCell();
    if(ret && ret->init(index)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void SettingCell::onEnter(){
    CCTableViewCell::onEnter();
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup_Item, true);

    
}

void SettingCell::onExit(){

    setTouchEnabled(false);
    CCTableViewCell::onExit();
}

bool SettingCell::init(unsigned int index){
    if(CCTableViewCell::init()){
        auto bg = CCBLoadFile("SettingCell", this, this);
        this->setContentSize(bg->getContentSize());
//        if(CCCommonUtils::isIosAndroidPad())
//        {
//            this->setContentSize(bg->getContentSize()*2.2);
//        }
        this->m_HintNode->setVisible(false);
        setData(index);
        return true;
    }
    return false;
}

void SettingCell::setData(unsigned int index)
{
    m_index = index;
    
    //this->m_iconNode->addChild(_icon);
    //updateState();
    string name = "";
    auto it = GlobalData::shared()->settingMap.find(m_index);
    if (it!=GlobalData::shared()->settingMap.end()) {
        name = _lang(it->second.name);
    }
    switch (m_index) {
        case Setting_HTP:
            m_iconBG->removeAllChildren();
            break;
        case Setting_FeedBack:{
            m_iconBG->removeAllChildren();
            break;
        }
        case Setting_Account:{
            m_iconBG->removeAllChildren();
            break;
        }
        case Setting_Push:{
            m_iconBG->removeAllChildren();
            auto _icon = CCLoadSprite::createSprite("setconfig_push_pic.png");
            _icon->setPosition(ccp(57,57));
            m_iconBG->addChild(_icon);
            break;
        }
        case Setting_Blocked:
        {
            auto _icon = CCLoadSprite::createSprite("block.png");
            m_iconBG->removeAllChildren();
            _icon->setPosition(ccp(57,57));
            m_iconBG->addChild(_icon);
        }
            break;
//        case Setting_Update:
//            m_itemName->setString(_lang("102999").c_str());
//            break;
//        case Setting_Set:
//            m_itemName->setString(_lang("101218").c_str());
//            this->m_iconNode->addChild(_icon);
//            break;
        case Setting_innerSetting:{
            m_iconBG->removeAllChildren();
            auto _icon = CCLoadSprite::createSprite("icon_gear.png");
            _icon->setPosition(ccp(57,57));
            m_iconBG->addChild(_icon);
            break;
        }
        case Setting_Clause:{//条款
            m_iconBG->removeAllChildren();
            auto _icon = CCLoadSprite::createSprite("icon_clause.png");
            _icon->setPosition(ccp(57,57));
            m_iconBG->addChild(_icon);
            break;
        }
        case Setting_bgMusic:
        {
            bool _BGMusic = CCUserDefault::sharedUserDefault()->getBoolForKey(BG_MUSIC_ON, true);
            CCSprite * _icon;
            m_iconBG->removeAllChildren();
//            m_iconBG->removeChildByTag(999);
            if (_BGMusic)
            {
                _icon = CCLoadSprite::createSprite("music.png");
            }
            else
            {
                _icon = CCLoadSprite::createSprite("musicoff.png");
            }
            
            _icon->setTag(999);
            _icon->setPosition(ccp(57,57));
            m_iconBG->addChild(_icon);
        }
            break;
        case Setting_BINDING:
        {
            CCSprite * _icon;
            m_iconBG->removeAllChildren();
            string fbIcon = "facebook_icon2.png";
            if(GlobalData::shared()->analyticID != "market_global"){
                fbIcon = "icon_facebook2.png";
            }
            if(GlobalData::shared()->analyticID == "AppStore"){
                fbIcon = "icon_game-center.png";
            }
            if(GlobalData::shared()->analyticID == "cn1"){
                fbIcon = "icon_facebookweibo.png";
            }
            _icon = CCLoadSprite::createSprite(fbIcon.c_str());
            _icon->setTag(999);
            _icon->setPosition(ccp(57,57));
            m_iconBG->addChild(_icon);
            
            if (!CCCommonUtils::IsBandOfAcount()) {
                auto ptIcon = CCLoadSprite::createSprite("unlock_tipPt.png");
                ptIcon->setPosition(ccp(100, 100));
                m_iconBG->addChild(ptIcon);
            }
            
        }
            break;
        case Setting_EffectSound:
        {
            bool _effectMusic = CCUserDefault::sharedUserDefault()->getBoolForKey(EFFECT_MUSIC_ON, true);
            
            CCSprite * _icon;
            m_iconBG->removeAllChildren();
//            m_iconBG->removeChildByTag(999);
            
            if (_effectMusic)
            {
                _icon = CCLoadSprite::createSprite("sound.png");
            }
            else
            {
                _icon = CCLoadSprite::createSprite("soundoff.png");
            }
            
            _icon->setTag(999);
            _icon->setPosition(ccp(57,57));
            m_iconBG->addChild(_icon);
        }
            break;
        case Setting_lang:
        {
            m_iconBG->removeAllChildren();
            std::string language = LocalController::shared()->getLanguageFileName();
            string icon = LanguageSettingView::getLanguageIconByType(language);
            //大陆地区,看到台弯的，都是大陆国旗
            if(icon=="TW" && CCCommonUtils::checkTaiWanFlag()){
                icon = "CN";
            }
            icon+=".png";
            auto pic  = CCLoadSprite::createSprite(icon.c_str());
            if(pic){
                this->m_iconBG->addChild(pic);
                if(pic->getContentSize().width>111){
                    pic->setScale(0.6776);
                }else{
                    pic->setScale(0.88);
                }
                pic->setPosition(ccp(57,57));
            }
        }
            break;
        case Setting_Help:{
            m_iconBG->removeAllChildren();
            auto pic  = CCLoadSprite::createSprite("icon_help.png");
            if(pic){
                this->m_iconBG->addChild(pic);
                pic->setPosition(ccp(57,57));
            }
            break;
        }
        case Setting_Temp:{
            m_iconBG->removeAllChildren();
            break;
        }
        case Setting_GM:{
            m_iconBG->removeAllChildren();
            break;
        }
        case Setting_Elex:{
            m_iconBG->removeAllChildren();
            break;
        }
        case Setting_SOCIAL:{
            m_iconBG->removeAllChildren();
            CCSprite* icon = CCLoadSprite::createSprite("fans_event_icon.png");
            icon->setTag(999);
            icon->setPosition(ccp(57,57));
            m_iconBG->addChild(icon);
            break;
        }
        case Setting_CDKEY:{
            m_iconBG->removeAllChildren();
            CCSprite* icon = CCLoadSprite::createSprite("cd_key_icon.png");
            icon->setTag(999);
            icon->setPosition(ccp(57,57));
            m_iconBG->addChild(icon);
            break;
        }
        case Setting_INVITE:{
            m_iconBG->removeAllChildren();
            CCSprite* icon = CCLoadSprite::createSprite("invite_icon.png");
            icon->setTag(999);
            icon->setPosition(ccp(57,57));
            m_iconBG->addChild(icon);
            break;
        }
        case Setting_SEARCH:{
            m_iconBG->removeAllChildren();
            CCSprite* icon = CCLoadSprite::createSprite("search_icon.png");
            icon->setTag(999);
            icon->setPosition(ccp(57, 57));
            m_iconBG->addChild(icon);
            break;
        }
        case Setting_MOD:{
            m_iconBG->removeAllChildren();
            std::string iconStr = "contact-mod.png";
            if(GlobalData::shared()->playerInfo.isMOD()){
                name = _lang("132005");
                iconStr = "mange-mod.png";
            }
            CCSprite* icon = CCLoadSprite::createSprite(iconStr.c_str());
            icon->setPosition(ccp(57,57));
            m_iconBG->addChild(icon);
            break;
        }
        case Setting_Forum:{
            m_iconBG->removeAllChildren();
            CCSprite* icon = CCLoadSprite::createSprite("settingForum_btn.png");
            CCCommonUtils::setSpriteGray(icon, !GlobalData::shared()->isOpenForum);
            icon->setPosition(ccp(57,57));
            m_iconBG->addChild(icon);
            break;
        }
        case Setting_Lobi:{
            m_iconBG->removeAllChildren();
            CCSprite* icon = CCLoadSprite::createSprite("Setting_Lobi_btn.png");
            icon->setPosition(ccp(57,57));
            m_iconBG->addChild(icon);
            break;
        }
        case Setting_VK:{
            m_iconBG->removeAllChildren();
            CCSprite* icon = CCLoadSprite::createSprite("icon_vk_general.png");
            icon->setPosition(ccp(57,57));
            m_iconBG->addChild(icon);
            break;
        }
        case Setting_WeiBo:{
            m_iconBG->removeAllChildren();
            CCSprite* icon = CCLoadSprite::createSprite("weibo.png");
            icon->setPosition(ccp(57,57));
            m_iconBG->addChild(icon);
            break;
        }
        case Setting_XiaoMi:{
            m_iconBG->removeAllChildren();
            CCSprite* icon = CCLoadSprite::createSprite("fans_event_icon.png");
            icon->setPosition(ccp(57,57));
            m_iconBG->addChild(icon);
            break;
        }
        case Setting_KO:{
            m_iconBG->removeAllChildren();
            CCSprite* icon = CCLoadSprite::createSprite("fans_ko_icon.png");
            icon->setPosition(ccp(57,57));
            m_iconBG->addChild(icon);
            break;
        }
        case Setting_Record:{
            m_iconBG->removeAllChildren();
            CCSprite* icon = CCLoadSprite::createSprite("ReplayKit_Logo.png");
            icon->setPosition(ccp(57,57));
            m_iconBG->addChild(icon);
            break;
        }
        case Setting_Translate_Optimization:{
            m_iconBG->removeAllChildren();
            CCSprite* icon = CCLoadSprite::createSprite("settingTranslateImprove_btn.png");
            icon->setPosition(ccp(57,57));
            m_iconBG->addChild(icon);
            break;
        }
        default:
            m_iconBG->removeAllChildren();
            break;
    }
    m_itemName->setString(name.c_str());
}

void SettingCell::touchEvent()
{
    //auto _icon = CCLoadSprite::createSprite("temp_icon.png");
    switch (m_index) {
        case Setting_MOD:{
            if(GlobalData::shared()->playerInfo.isMOD()){
                PopupViewController::getInstance()->addPopupInView(MODView::create());
            }else{
                PopupViewController::getInstance()->addPopupInView(MODContactView::create());
            }
            break;
        }
        case Setting_HTP:
            PopupViewController::getInstance()->addPopupView(HowToPlayView::create());
            break;
        case Setting_FeedBack:
            PopupViewController::getInstance()->addPopupInView(SuggestionView::create());
            break;
        case Setting_Account:
            PopupViewController::getInstance()->addPopupInView(LoginServiceView::create(3));
            break;
            
        case Setting_Push:
            PopupViewController::getInstance()->addPopupInView(PushSettingView::create());
            break;
        case Setting_Blocked:
            PopupViewController::getInstance()->addPopupInView(ShieldUnlockPopUpView::create());
            break;
        case Setting_innerSetting:
            PopupViewController::getInstance()->addPopupInView(InnerSettingView::create());
            break;
        case Setting_Clause://新添加,服务条款
//            CCLOGFUNC();
//            PopupViewController::getInstance()->addPopupInView(ClauseView::create());
        {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
            GlobalData::shared()->isBind = true;
#endif
            CCCommonUtils::showHelpShiftSingleFAQ("45212");
        }
            break;
        case Setting_bgMusic:
        {
            bool _BGMusic = !CCUserDefault::sharedUserDefault()->getBoolForKey(BG_MUSIC_ON, true);
            CCUserDefault::sharedUserDefault()->setBoolForKey(BG_MUSIC_ON, _BGMusic);
            CCUserDefault::sharedUserDefault()->flush();
            
            CCSprite * _icon;
            m_iconBG->removeAllChildren();
//            this->m_iconNode->removeChildByTag(999);
            
            if (_BGMusic)
            {
                SoundController::sharedSound()->playBGMusic(SoundController::sharedSound()->getBG_NAME().c_str());
                _icon = CCLoadSprite::createSprite("music.png");
            }
            else
            {
                SoundController::sharedSound()->stopAllMusic();
                _icon = CCLoadSprite::createSprite("musicoff.png");
            };
            
            _icon->setTag(999);
            _icon->setPosition(ccp(57,57));
            m_iconBG->addChild(_icon);
        }
            break;
        case Setting_EffectSound:
        {
            bool _EffectMusic = !CCUserDefault::sharedUserDefault()->getBoolForKey(EFFECT_MUSIC_ON, true);
            CCUserDefault::sharedUserDefault()->setBoolForKey(EFFECT_MUSIC_ON, _EffectMusic);
            CCUserDefault::sharedUserDefault()->flush();
            
            CCSprite * _icon;
            m_iconBG->removeAllChildren();
//            this->m_iconNode->removeChildByTag(999);
            
            if (_EffectMusic)
            {
                _icon = CCLoadSprite::createSprite("sound.png");
            }
            else
            {
                _icon = CCLoadSprite::createSprite("soundoff.png");
            }
            
            _icon->setTag(999);
            _icon->setPosition(ccp(57,57));
            m_iconBG->addChild(_icon);
        }
            break;
        case Setting_lang:
        {
            PopupViewController::getInstance()->addPopupInView(LanguageSettingView::create());
//            std::string language = LocalController::shared()->getLanguageFileName();
//            if (language.compare("en"))
//            {
//                CCUserDefault::sharedUserDefault()->setStringForKey(KEY_PREFERRED_LANGUAGE, "en");
//                m_itemName->setString("CN");
//            }
//            else
//            {
//                CCUserDefault::sharedUserDefault()->setStringForKey(KEY_PREFERRED_LANGUAGE, "zh_CN");
//                m_itemName->setString("EN");
//            }
//            CCUserDefault::sharedUserDefault()->setStringForKey("current-version-code","");
//            CCUserDefault::sharedUserDefault()->flush();
//            
//            LogoutCommand* cmd=new LogoutCommand();
//            cmd->sendAndRelease();
        }
            break;
        case Setting_Help:{
            CCCommonUtils::showHelpShiftFAQ();
            break;
        }
        case Setting_Temp:
            BattleManager::shared()->requestBattleInfo(0,0);
            break;
        case Setting_BINDING:
            PopupViewController::getInstance()->addPopupInView(SetAccountView::create());
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("Set_bangding"));
            break;
        case Setting_GM:
            PopupViewController::getInstance()->addPopupInView(ChangeServeView::create());
            break;
        case Setting_SOCIAL:
            PopupViewController::getInstance()->addPopupInView(SocialPopUpView::create());
            break;
        case Setting_CDKEY:
            PopupViewController::getInstance()->addPopupView(CDkeyPopUpView::create());
            break;
        case Setting_INVITE:
//            PopupViewController::getInstance()->addPopupInView(InviteForGiftView::create());
            break;
        case Setting_SEARCH:
            PopupViewController::getInstance()->addPopupInView(SearchView::create());
            break;
        case Setting_Forum:
        {
           if(GlobalData::shared()->isOpenForum){
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
                ChatServiceCocos2dx::showForumFrom2dx(NULL);
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
                ChatServiceCocos2dx::openBBS();
#endif
            }
        }
         break;
        case Setting_Lobi:
            cocos2d::extension::CCDevice::updateVersion("https://web.lobi.co/game/clash_of_kings");
            break;
        case Setting_VK:
            cocos2d::extension::CCDevice::updateVersion("https://vk.com/clashofking");
            break;
        case Setting_WeiBo:
            cocos2d::extension::CCDevice::updateVersion("http://weibo.com/liewangdefenzheng");
            break;
        case Setting_Translate_Optimization:
        {
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
            if(GlobalData::shared()->isOpenTranslateOptimization)
            {
                if (ChatController::getInstance()->translateOptimizationUrl=="")
                {
                    ChatController::getInstance()->translateOptimize(METHOD_ADD_TRANSLATE);
                }
                else
                {
                    ChatServiceCocos2dx::showTranslationOptimizationFrom2dx(ChatController::getInstance()->translateOptimizationUrl.c_str());
                }
            }
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
            
#endif
        }
            break;
        case Setting_XiaoMi:
            if(GlobalData::shared()->analyticID == "cn_dangle")
                cocos2d::extension::CCDevice::updateVersion("http://ng.d.cn/coklwdfz/");
            else
                cocos2d::extension::CCDevice::updateVersion("http://cok.mi.com");
            break;
            
        case Setting_KO:
            cocos2d::extension::CCDevice::updateVersion("http://cafe.naver.com/clashofkings");
            break;
        case Setting_Record:
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
#ifdef  ios9
            ReplayKitsController::getInstance()->startRecord();
            PopupViewController::getInstance()->removeAllPopupView();
#endif
#endif
            break;
        default:
            break;
    }
}
/*
void SettingCell::updateState(){
    if (Setting_Update == m_index) {
        this->m_HintNode->stopAllActions();
        this->m_HintNode->setPositionY(-49);
        float px = m_HintNode->getPositionX();
        if(GlobalData::shared()->version == GlobalData::shared()->serverVersion){
            this->m_HintNode->setVisible(false);
        }else{
            this->m_HintNode->setVisible(true);
            CCMoveTo* moveUp= CCMoveTo::create(0.1f, ccp(px,-34));
            CCMoveTo* moveDown= CCMoveTo::create(0.1f, ccp(px,-49));
            CCMoveTo* moveUp2= CCMoveTo::create(0.1f, ccp(px,-39));
            this->m_HintNode->runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(1),moveUp,moveDown,moveUp2,moveDown,NULL)));
        }
    }
}
*/

