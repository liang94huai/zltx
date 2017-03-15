//
//  VipDetailView.cpp
//  IF
//
//  Created by chenliang on 14-3-18.
//
//

#include "VipDetailView.h"

#include "SoundController.h"
#include "PopupViewController.h"
#include "VipDetailCell.h"
#include "LocalController.h"
#include "VipUtil.h"
#include "StoreView.h"
#include "TipsView.h"
#include "ParticleController.h"
#include "UseToolView.h"
#include "ToolController.h"
#include "HFHeadImgNode.h"
#include "YesNoDialog.h"
#include "VipActiveView.h"

#define VIP_PROGRESS_BAR_WIDTH 488

const float VipDetailView::CELL_MARGIN = 20;
const int VipDetailView::MAX_CELL_NEEDED = 3;

VipDetailView::VipDetailView(int type):m_type(type),m_touchState(0)
{
}

bool VipDetailView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipBtn", CCControlButton*, this->m_tipBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipLvText", CCLabelIFBMFont*, this->m_vipLvText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipLeftTimeText", CCLabelIF*, this->m_vipLeftTimeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loginDayText", CCLabelIF*, this->m_loginDayText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_currentVIPText", CCLabelIFBMFont*, this->m_currentVIPText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_NextVIPText", CCLabelIFBMFont*, this->m_NextVIPText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progressBarNode", CCNode*, this->m_progressBarNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipAnimNode", CCNode*, this->m_vipAnimNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picHeadNode", CCNode*, this->m_picHeadNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listBG", CCScale9Sprite*, this->m_listBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pageFlag", CCSprite*, this->m_pageFlag);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipLastPointNumText", CCLabelIF*, this->m_vipLastPointNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipNextPointNumText", CCLabelIF*, this->m_vipNextPointNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipCurrentPointNumText", CCLabelIF*, this->m_vipCurrentPointNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desText", CCLabelIF*, this->m_desText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblVIP1", CCLabelIFBMFont*, this->m_lblVIP[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblVIP2", CCLabelIFBMFont*, this->m_lblVIP[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblVIP3", CCLabelIFBMFont*, this->m_lblVIP[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblVIP4", CCLabelIFBMFont*, this->m_lblVIP[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprVip", CCSprite*, this->m_sprVip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprActive1", CCSprite*, this->m_sprActive[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprActive2", CCSprite*, this->m_sprActive[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barTouchNode", CCNode*, this->m_barTouchNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprBar1", CCScale9Sprite*, this->m_sprBar[0]);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprBar2", CCScale9Sprite*, this->m_sprBar[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_activeGuideNode", Node*, m_activeGuideNode);
    //addNode
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "addPointBtn", CCControlButton*, addPointBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "addPointBtnTxt", CCLabelIF*, addPointBtnTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_activeNode", Node*, m_activeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_activeTxt", CCLabelIF*, m_activeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_activeSpr", CCSprite*, m_activeSpr);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_showVIPBtn", CCControlButton*, m_showVIPBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_showSVIPBtn", CCControlButton*, m_showSVIPBtn);
    //SVIP
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprLastPoint", CCSprite*, m_sprLastPoint);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNodeSVIP", CCNode*, m_downNodeSVIP);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listContainerSVIP", CCNode*, m_listContainerSVIP);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pageFlagSVIP", CCSprite*, m_pageFlagSVIP);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_BgNodeSVIP", CCNode*, m_BgNodeSVIP);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_svipUpTextSpr", CCSprite*, m_svipUpTextSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headBgVip", CCScale9Sprite*, m_headBgVip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headBgSvip", CCSprite*, m_headBgSvip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipNodeLeft", CCNode*, m_vipNodeLeft);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_svipNodeLeft", CCNode*, m_svipNodeLeft);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipNodeRight", CCNode*, m_vipNodeRight);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_svipNodeRight", CCNode*, m_svipNodeRight);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_svipBuildBG", CCScale9Sprite*, this->m_svipBuildBG);
    
    return true;
}

SEL_CCControlHandler VipDetailView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onShowVIPBtnClick", VipDetailView::onShowVIPBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onShowSVIPBtnClick", VipDetailView::onShowSVIPBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipBtnClick", VipDetailView::onTipBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddPointBtnClick", VipDetailView::onAddPointBtnClick);
    
    return NULL;
}
void VipDetailView::onShowVIPBtnClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){
    CCLOGFUNC();
    setVipType(1);
}
void VipDetailView::onShowSVIPBtnClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){
    CCLOGFUNC();
    setVipType(2);
}

void VipDetailView::onTipBtnClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){
    string content = "";
    auto &playerInfo = GlobalData::shared()->playerInfo;
    if(VipUtil::isSVIP() && playerInfo.SVIPLevel>0){
        content = _lang("103100")+"\n"+_lang("103101")+"\n"+_lang("103102")+"\n"+_lang("103103");
    }else{
        content = _lang("103031")+"\n"+"\n"+"\n"+"\n"+_lang("103032");
    }
    //CCCommonUtils::flyText("VIP Rule");
    PopupViewController::getInstance()->addPopupView(TipsVipView::create(_lang("103030"),content));
    
}
void VipDetailView::onAddPointBtnClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){
//    PopupViewController::getInstance()->addPopupView(VipActiveView::create());
//    return;
    // PopupViewController::getInstance()->addPopupInView(StoreView::create());
    
    int point = VipUtil::getMaxSpoint() - GlobalData::shared()->playerInfo.SVIPPoint;
    if (point>0) {
        auto dict = CCDictionary::create();
        PopupViewController::getInstance()->addPopupInView(UseToolView::create(USE_TOOL_VIP_PIC,dict,"103002"));
    }else{
        YesNoDialog::show(_lang("103105").c_str(), NULL);//103105=您的SVIP已经达到最高级，无需继续购买！//103057=您的VIP等级已经达到最高，无需继续购买！
    }
}
void VipDetailView::onActivityPointBtnClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){
    auto dict = CCDictionary::create();
    PopupViewController::getInstance()->addPopupInView(UseToolView::create(USE_TOOL_VIP_ACTIVITY,dict,"103004"));
    //PopupViewController::getInstance()->addPopupInView(StoreView::create());
}

VipDetailView* VipDetailView::create(int type){
    VipDetailView* ret = new VipDetailView(type);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
    
}

void VipDetailView::onEnter(){
    PopupBaseView::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(VipDetailView::setVipProgress), VIP_REFRESH, NULL);
    if(GlobalData::shared()->playerInfo.SVIPLevel>0){
        setTitleName(_lang("103075"));//103075=SVIP
    }else{
        setTitleName(_lang("103000"));//103000=VIP
    }
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    this->schedule(schedule_selector(VipDetailView::onTimer),1.0);
    setVipProgress(NULL);
    if (m_firstIn) {
        m_firstIn = false;
        if (m_type == 1) {
            m_vipLevel = 8;
        }
    }
    setSvipView();
}

void VipDetailView::onExit(){
    setTouchEnabled(false);
    this->unschedule(schedule_selector(VipDetailView::onTimer));
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, VIP_REFRESH);
    PopupBaseView::onExit();
}


bool VipDetailView::init(){
    
    bool ret = false;
    if(!PopupBaseView::init())
    {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(101, true);//quest_Output_icon2.png
    CCLoadSprite::doResourceByCommonIndex(5, true);//science_def_a_1.png
    CCLoadSprite::doResourceByCommonIndex(6, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);//Allance_team_Members_lock.png
    CCLoadSprite::doResourceByCommonIndex(8, true);
    CCLoadSprite::doResourceByCommonIndex(105, true);
    CCLoadSprite::doResourceByCommonIndex(514, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(101, false);//quest_Output_icon2.png
        CCLoadSprite::doResourceByCommonIndex(5, false);
        CCLoadSprite::doResourceByCommonIndex(6, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);//Allance_team_Members_lock.png
        CCLoadSprite::doResourceByCommonIndex(8, false);
        CCLoadSprite::doResourceByCommonIndex(105, false);
        CCLoadSprite::doResourceByCommonIndex(514, false);
    });

    m_SVIPShowLevel = VipUtil::getSvipShowLevel();
    m_firstIn = true;
    m_ismoving = false;
    m_movedh = 0;
    m_movedhSVIP = 0;
    m_data = CCArray::create();
    m_listNode = CCNode::create();
    auto bg = CCBLoadFile("VipMain", this, this);
    this->setContentSize(bg->getContentSize());
    setTitleName(_lang("103018"));
    int preHeight = this->m_buildBG->getContentSize().height;
    
    m_NextVIPText->setFntFile("pve_fnt_title.fnt");
    m_vipLvText->setFntFile("pve_fnt_title.fnt");
//    m_vipLeftTimeText->setFntFile("pve_fnt_title.fnt");
    m_currentVIPText->setFntFile("pve_fnt_title.fnt");
    int index = 0;
    while (index<4) {
        m_lblVIP[index]->setFntFile("pve_fnt_title.fnt");
        ++index;
    }
    auto &playerInfo = GlobalData::shared()->playerInfo;
    
    vip10Left=atoi(CCCommonUtils::getPropById("7009", "point").c_str());
    vip10Right=atoi(CCCommonUtils::getPropById("7010", "point").c_str());
    curPoint=playerInfo.vipPoints;

    m_loginDayText->setString(_lang("103117"));//升级到SVIP可获得全部VIP特权和更多权限
    changeBGHeight(m_buildBG);
    int dh = m_buildBG->getContentSize().height - preHeight;
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_buildBG->setContentSize(CCDirector::sharedDirector()->getWinSize());
        dh = CCDirector::sharedDirector()->getWinSize().height - 2048;
    }
    this->m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width, m_listContainer->getContentSize().height + dh));
    this->m_listBG->setContentSize(CCSize(m_listBG->getContentSize().width, m_listBG->getContentSize().height + dh));
    this->m_downNode->setPositionY(m_downNode->getPositionY() - dh);
    this->m_listContainerSVIP->setContentSize(CCSize(m_listContainerSVIP->getContentSize().width, m_listContainerSVIP->getContentSize().height + dh));
    this->m_downNodeSVIP->setPositionY(m_downNodeSVIP->getPositionY() - dh);
    this->m_svipBuildBG->setContentSize(CCSize(m_svipBuildBG->getContentSize().width, m_svipBuildBG->getContentSize().height + dh));
    
    
    listContentSize =m_listContainer->getContentSize();
    m_nodeprogressBar = CCClipNode::create(CCSize(VIP_PROGRESS_BAR_WIDTH,116));
//    if (CCCommonUtils::isIosAndroidPad())
//    {
//        m_progressBarNode->setContentSize(CCSize(1113, 208));
//    }
    m_nodeprogressBar->setAnchorPoint(ccp(0,0.5));
    this->m_progressBarNode->addChild(m_nodeprogressBar);
    if(VipUtil::isSVIP() && playerInfo.SVIPLevel>0){
        m_sprBar[2] = CCLoadSprite::createScale9Sprite("svip_jindutiao.png");
    }else{
        m_sprBar[2] = CCLoadSprite::createScale9Sprite("vip_jindutiao_pro.png");
    }
    m_sprBar[2]->setAnchorPoint(ccp(0.0, 0.5));
    m_sprBar[2]->setPositionY(29);
    m_sprBar[2]->setContentSize(Size(VIP_PROGRESS_BAR_WIDTH,17));
    m_nodeprogressBar->addChild(m_sprBar[2]);
    if(VipUtil::isSVIP() && playerInfo.SVIPLevel>0){
        m_sprBar[3] = CCLoadSprite::createScale9Sprite("svip_jindutiao.png");
    }else{
        m_sprBar[3] = CCLoadSprite::createScale9Sprite("vip_jindutiao_pro.png");
    }
   // m_sprBar[3]->setFlipX(true);
    m_sprBar[3]->setVisible(false);
    m_sprBar[3]->setAnchorPoint(ccp(0.0, 0.5));
    m_sprBar[3]->setPositionX(VIP_PROGRESS_BAR_WIDTH*0.5);
   if (CCCommonUtils::isIosAndroidPad()) {
       m_sprBar[3]->setPositionX(1113 * 0.5);
       float scaleX = 1113 * 1.0 / VIP_PROGRESS_BAR_WIDTH;
       float scaleY = 208 * 1.0 / 116;
       m_sprBar[2]->setScaleX(scaleX);
       m_sprBar[2]->setScaleY(scaleY);
       m_sprBar[3]->setScaleX(scaleX);
       m_sprBar[3]->setScaleY(scaleY);
   }
    m_sprBar[3]->setPositionY(29);
    m_nodeprogressBar->addChild(m_sprBar[3]);
    
    int w = m_listContainer->getContentSize().width;
    int h = m_listContainer->getContentSize().height;
    m_scroll = CCScrollView::create(CCSize(w, h));
    m_scroll->setAnchorPoint(ccp(0, 1));
    m_scroll->setTouchPriority(1);
    m_scroll->setDirection(kCCScrollViewDirectionVertical);
    m_listContainer->addChild(m_scroll);
    m_scroll->addChild(m_listNode);
    
    m_listNodeSVIP = CCNode::create();
    m_scrollSVIP = CCScrollView::create(CCSize(w, h));
    m_scrollSVIP->setAnchorPoint(ccp(0, 1));
    m_scrollSVIP->setTouchPriority(1);
    m_scrollSVIP->setDirection(kCCScrollViewDirectionVertical);
    m_listContainerSVIP->addChild(m_scrollSVIP);
    m_scrollSVIP->addChild(m_listNodeSVIP);
    
    ret = true;
    refresh();
    addPointBtnTxt->setString(_lang("138054"));//138054=获得积分
    
    setVipType(1);
    setSvipView();
    return ret;
}

void VipDetailView::refresh(){
    m_picHeadNode->removeAllChildren();
    if (GlobalData::shared()->playerInfo.isUseCustomPic()){
        auto headImgNode = HFHeadImgNode::create();
        headImgNode->initHeadImgUrl2(m_picHeadNode, GlobalData::shared()->playerInfo.getCustomPicUrl(), 1.0f, 130, true);
    }else{
        CCSprite* head = CCLoadSprite::createSprite(GlobalData::shared()->playerInfo.getPic().c_str());
        m_picHeadNode->addChild(head);
        CCCommonUtils::setSpriteMaxSize(head, 120,true);
    }
    auto &playerInfo = GlobalData::shared()->playerInfo;
    int vipLv = VipUtil::getVipLevel(playerInfo.vipPoints);
    if(playerInfo.vipEndTime>GlobalData::shared()->getWorldTime()){//vip有效,倒计时
        this->unschedule(schedule_selector(VipDetailView::onTimer));
        this->schedule(schedule_selector(VipDetailView::onTimer),1.0);
        onTimer(1.0);
        m_activeNode->setVisible(false);
    }else{
//        m_vipLeftTimeText->setString(_lang("103011").c_str());//103011=（未激活）
        m_vipLeftTimeText->setString("");
        m_activeNode->setVisible(true);
        m_activeTxt->setString(_lang("101097"));//101097		点击激活
        
        if (vipLv >= 8 && m_type == 1) {
            setGuideAnim();
            m_type = 0;
        }else if (m_type == 2) {
            setGuideAnim();
        }
    }
    
    setVipProgress(NULL);
}
void VipDetailView::setVipProgress(CCObject* obj){
    auto &playerInfo = GlobalData::shared()->playerInfo;
    m_desText->setString(_lang_1("103045", CC_ITOA(playerInfo.nextDayLoginVipReward)));
    curPoint = playerInfo.vipPoints;
    
    if(VipUtil::isSVIP() && curPoint>=vip10Left && curPoint<vip10Right && playerInfo.SVIPLevel==0){
        m_lblVIP[0]->setString(_lang("103000").c_str());//103000=VIP
        m_lblVIP[2]->setString(_lang("103075").c_str());//svipStr.c_str()
    }else if(VipUtil::isSVIP() && playerInfo.SVIPLevel>0){
        m_lblVIP[0]->setString(_lang("103075").c_str());//103075=SVIP _lang("103075").c_str()
        m_lblVIP[2]->setString(_lang("103075").c_str());//svipStr.c_str()
    }else{
        m_lblVIP[0]->setString(_lang("103000").c_str());//103000=VIP
        m_lblVIP[2]->setString(_lang("103000").c_str());
    }
    m_lblVIP[1]->setPositionX(m_lblVIP[0]->getPositionX() + m_lblVIP[0]->getContentSize().width * m_lblVIP[0]->getOriginScaleY());

    int vipLv = VipUtil::getVipLevel(playerInfo.vipPoints);

    m_vipLevel = vipLv;
    if((!VipUtil::isSVIP()) && vipLv>9){
        m_vipLevel = 9;
    }
//    else{
//    }
    
    if(playerInfo.SVIPLevel>m_SVIPShowLevel){
        m_vipLevelSVIP = m_SVIPShowLevel;
    }else if(playerInfo.SVIPLevel>0){
        m_vipLevelSVIP = playerInfo.SVIPLevel;
    }else{
        m_vipLevelSVIP = 1;
    }
    int currLvPoint = VipUtil::getVipItemValueByKey(CC_ITOA(m_vipLevel+7000-1), "point");
    int nextLvPoint = VipUtil::getVipItemValueByKey(CC_ITOA(m_vipLevel+7000), "point");
    m_vipCurrentPointNumText->setString(CC_CMDITOA(playerInfo.vipPoints));
    int point = playerInfo.vipPoints-currLvPoint;
    
    if(VipUtil::isSVIP() && curPoint>=vip10Left && curPoint<vip10Right && playerInfo.SVIPLevel==0){
        currLvPoint = atoi(CCCommonUtils::getPropById(CC_ITOA(m_vipLevel+7000-1), "point").c_str());
        nextLvPoint = atoi(CCCommonUtils::getPropById(CC_ITOA(m_vipLevel+7000), "point").c_str());
        m_vipLvText->setString(_lang_1("103001", CC_ITOA(vipLv)));//VIP{0}
        this->m_currentVIPText->setString(CC_ITOA(m_vipLevel));
        this->m_lblVIP[1]->setString(CC_ITOA(m_vipLevel));
        this->m_NextVIPText->setString("1");
        this->m_lblVIP[3]->setString("1");
    }else if(VipUtil::isSVIP() && playerInfo.SVIPLevel>0){
        m_desText->setString(_lang("103099"));//参加游戏活动，完成每日活跃或从商店购买均可获得SVIP点卡
        m_loginDayText->setString(_lang_1("103118",CC_ITOA(playerInfo.SVIPLevel)));//您已获得SVIP{0}权限,全部VIP特权
        m_vipLvText->setString(_lang_1("103076", CC_ITOA(playerInfo.SVIPLevel)));//103076=SVIP{0}
        currLvPoint = atoi(CCCommonUtils::getPropById(CC_ITOA(m_vipLevelSVIP+7010-1), "spoint").c_str());
        nextLvPoint = atoi(CCCommonUtils::getPropById(CC_ITOA(m_vipLevelSVIP+7010), "spoint").c_str());
        m_vipCurrentPointNumText->setString(CC_CMDITOA(playerInfo.SVIPPoint));
        this->m_currentVIPText->setString(CC_ITOA(m_vipLevelSVIP));
        this->m_lblVIP[1]->setString(CC_ITOA(m_vipLevelSVIP));
        this->m_NextVIPText->setString(CC_ITOA(m_vipLevelSVIP+1));
        this->m_lblVIP[3]->setString(CC_ITOA(m_vipLevelSVIP+1));
        point = playerInfo.SVIPPoint-currLvPoint;
    }else{
        m_vipLvText->setString(_lang_1("103001", CC_ITOA(vipLv)));//VIP{0}
        this->m_currentVIPText->setString(CC_ITOA(m_vipLevel));
        this->m_lblVIP[1]->setString(CC_ITOA(m_vipLevel));
        this->m_NextVIPText->setString(CC_ITOA(m_vipLevel+1));
        this->m_lblVIP[3]->setString(CC_ITOA(m_vipLevel+1));
    }

    m_vipLastPointNumText->setString("("+CC_CMDITOA(currLvPoint)+")");
    m_vipNextPointNumText->setString("("+CC_CMDITOA(nextLvPoint)+")");
    
    m_lblVIP[2]->setPositionX(m_lblVIP[3]->getPositionX() - m_lblVIP[3]->getContentSize().width * m_lblVIP[3]->getOriginScaleY());
    refreshPageFlag();
    
    m_listNode->removeChildByTag(1000);
    if (m_type == 1 && m_vipLevel < 8 && m_firstIn) {
        VipDetailCell* cell = VipDetailCell::create(7, m_type);//通过出征部队上限警告面板进入且VIP等级小于8下方跳转至VIP8的介绍
        m_listNode->addChild(cell, 0, 1000);
        this->m_listNode->setPositionY(-135);
        if (CCCommonUtils::isIosAndroidPad())
        {
            this->m_listNode->setPositionY(-270);
        }
        m_scroll->setContentSize(CCSize(m_listContainer->getContentSize().width, cell->getHG()));
        m_scroll->setContentOffset(ccp(0, m_listContainer->getContentSize().height - (cell->getHG())));
        this->m_currentVIPText->setString(CC_ITOA(8));
        this->m_NextVIPText->setString(CC_ITOA(9));
        this->m_pageFlag->setPositionX(-114+29*(7));
    }
    else {
        VipDetailCell* cell = nullptr;
        if(VipUtil::isSVIP() && curPoint>=vip10Left && playerInfo.SVIPLevel>=0){
            cell = VipDetailCell::create(-1,0,2);
        }else{
            cell = VipDetailCell::create(m_vipLevel-1, m_type);
        }
        m_listNode->addChild(cell, 0, 1000);
        this->m_listNode->setPositionY(-135);
        if (CCCommonUtils::isIosAndroidPad())
        {
            this->m_listNode->setPositionY(-270);
        }
        m_scroll->setContentSize(CCSize(m_listContainer->getContentSize().width, cell->getHG()));
        m_scroll->setContentOffset(ccp(0, m_listContainer->getContentSize().height - (cell->getHG())));
    }
    //SVIP cell********************************************************************************************
    
    m_listNodeSVIP->removeChildByTag(1000);
    VipDetailCell* cellSVIP = VipDetailCell::create(m_vipLevelSVIP-1, 0, 2);//m_type
    m_listNodeSVIP->addChild(cellSVIP, 0, 1000);
    this->m_listNodeSVIP->setPositionY(-135);
    if (CCCommonUtils::isIosAndroidPad())
    {
        this->m_listNodeSVIP->setPositionY(-270);
    }
    m_scrollSVIP->setContentSize(CCSize(m_listContainerSVIP->getContentSize().width, cellSVIP->getHG()));
    m_scrollSVIP->setContentOffset(ccp(0, m_listContainerSVIP->getContentSize().height - (cellSVIP->getHG())));
    //SVIP********************************************************************************************
    
    int needPoint =nextLvPoint-currLvPoint;
    if(needPoint==0){
        needPoint = 1.0;
    }
    float sacle =point/(needPoint*1.0);
    sacle = sacle>1.0?1.0:sacle;
    float total = VIP_PROGRESS_BAR_WIDTH * sacle;
//    if (CCCommonUtils::isIosAndroidPad()) {
//        total = 1113 * sacle;
//    }
    m_nodeprogressBar->setContentSize(CCSize(total, 58));
//    if(playerInfo.vipPoints < nextLvPoint  && playerInfo.vipPoints > currLvPoint){
//        m_sprVip->setVisible(true);
//        m_vipCurrentPointNumText->setVisible(true);
        m_sprVip->setPositionX(m_progressBarNode->getPositionX() + total);
        m_vipCurrentPointNumText->setPositionX(m_sprVip->getPositionX());
//    }
    
    setBtnState();
}
void VipDetailView::onAddVipAnim(){
     m_vipAnimNode->removeAllChildren();
    
    Sprite* spr = Sprite::create();
    CCCommonUtils::makeEffectSpr(spr, "vip_effect_0%d.png", 10, 0.1);
    m_vipAnimNode->addChild(spr);
}
void VipDetailView::setBtnState(){
    m_sprActive[1]->stopAllActions();
    int dtime = GlobalData::shared()->playerInfo.vipEndTime - GlobalData::shared()->getWorldTime();
    if(dtime>0){
        int index = 0;
        while (index<2) {
            CCCommonUtils::setSpriteGray(m_sprActive[index], false);
            ++index;
        }
        CCActionInterval * rotateto1 = CCRotateTo::create(20, 720);
        CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
        CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
        m_sprActive[1]->runAction(rotateForever);
        onAddVipAnim();
    }else{
        int index = 0;
        while (index<2) {
            CCCommonUtils::setSpriteGray(m_sprActive[index], true);
            ++index;
        }
        m_vipAnimNode->removeAllChildren();
        m_sprActive[1]->setRotation(30);
    }
}
void VipDetailView::setGuideAnim(){
    auto sprite = CCLoadSprite::createScale9Sprite("rect.png");
    m_activeGuideNode->addChild(sprite);
    sprite->setScale(this->m_activeGuideNode->getContentSize().width / sprite->getContentSize().width);
    sprite->setPosition(this->m_activeGuideNode->getContentSize().width / 2, this->m_activeGuideNode->getContentSize().height / 2);
    sprite->setOpacity(0);
    CCSequence *seque = CCSequence::create(CCFadeIn::create(0.3), CCFadeOut::create(0.3) ,NULL);
    CCSequence *seque1 = CCSequence::create(CCRepeat::create(seque, 5), CCRemoveSelf::create(), NULL);
    sprite->runAction(seque1);
}
void VipDetailView::onTimer(float dt){

    int dtime = GlobalData::shared()->playerInfo.vipEndTime - GlobalData::shared()->getWorldTime();
    if(dtime<0){
        this->unschedule(schedule_selector(VipDetailView::onTimer));
        m_vipLeftTimeText->setString("");
        m_activeNode->setVisible(true);
        m_activeTxt->setString(_lang("101097"));//_lang("101097")		点击激活
        //箭头移动动画
        m_activeSpr->stopAllActions();
        CCActionInterval * moveBy1 = CCMoveBy::create(0.5,ccp(-20, 0));
        CCActionInterval * delay = DelayTime::create(0.5);
        CCActionInterval * repeat = CCRepeatForever::create( CCSequence::create(moveBy1, moveBy1->reverse(), delay, NULL) );
        m_activeSpr->runAction(repeat);
        
        setBtnState();
    }else{
        m_activeNode->setVisible(false);
        string timestr = getTimeStr(dtime);
        m_vipLeftTimeText->setString(timestr.c_str());
    }
}
string VipDetailView::getTimeStr(int secs){
    int day = secs/(24*3600);
    secs = secs%(24*3600);
    int hour = secs/3600;
    secs = secs%(3600);
    int min = secs/60;
    secs = secs%(60);
    int sec = secs;
    std::string dayStr = _lang("105592");
    std::string hourStr = _lang("105591");
    std::string minStr = _lang("105590");
    string timeStr = "";
    if(day > 0){
        timeStr = CC_ITOA(day) + std::string(" d ");
    }
    if(hour<10){
        timeStr.append("0");
        timeStr.append(CC_ITOA(hour));
    }else{
        timeStr.append(CC_ITOA(hour));
    }
    timeStr.append(":");
    if(min<10){
        timeStr.append("0");
        timeStr.append(CC_ITOA(min));
    }else{
        timeStr.append(CC_ITOA(min));
    }
    timeStr.append(":");
    if(sec<10){
        timeStr.append("0");
        timeStr.append(CC_ITOA(sec));
    }else{
        timeStr.append(CC_ITOA(sec));
    }
    
    return timeStr;
}

bool VipDetailView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!m_ismoving) {
        if(isTouchInside(m_sprActive[0], pTouch)){
            int index = 0;
            while (index<2) {
                m_sprActive[index]->setColor(ccGRAY);
                ++index;
            }
            m_touchState = 1;
            return true;
        }else if(isTouchInside(m_barTouchNode, pTouch)){
            
            if(m_sprBar[2])
                m_sprBar[2]->setColor(ccGRAY);
        
            
            m_touchState = 2;
            return true;
        }
    }
    m_touchState = 0;
    m_touchPoint = pTouch->getLocation();
    if (!m_ismoving&&(isTouchInside(m_listContainer,pTouch)||isTouchInside(m_listContainerSVIP,pTouch))){
        return true;
    }
    return false;
}
void VipDetailView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(m_touchState == 1){
        int index = 0;
        while (index<2) {
            m_sprActive[index]->setColor(ccWHITE);
            ++index;
        }
        onActivityPointBtnClick(NULL, Control::EventType::TOUCH_DOWN);
    }else if(m_touchState==2){
        
        if(m_sprBar[2])
            m_sprBar[2]->setColor(ccWHITE);
        
        
        onAddPointBtnClick(NULL, Control::EventType::TOUCH_DOWN);
    }else{
        if(m_ismoveX&&fabs(m_touchPoint.x - pTouch->getLocation().x)>10){
            int dx =-m_touchPoint.x +pTouch->getLocation().x;
            if(m_vipType == 1){
                if(dx<0){
                    if(VipUtil::isSVIP() && m_vipLevel>=10){
//                        m_ismoving = false;
//                        m_ismoveX = false;
//                        m_scroll->setTouchEnabled(true);
                        PlayNodeExchangeAnim(0);
                    }else{
                        if((!VipUtil::isSVIP()) && m_vipLevel>=9){
                            m_ismoving = false;
                            m_ismoveX = false;
                            m_scroll->setTouchEnabled(true);
                        }else{
                            m_vipLevel++;
                            PlayComeInFromRightAnim();
                        }
                    }
                    
                }else{
                    if(m_vipLevel<=1){
                        m_ismoving = false;
                        m_ismoveX = false;
                        m_scroll->setTouchEnabled(true);
                        
                    }else{
                        m_vipLevel--;
                        PlayComeInFromLeftAnim();
                    }
                }
            }else if (m_vipType == 2){
                if(dx<0){
                    if(m_vipLevelSVIP>=m_SVIPShowLevel){
                        m_ismoving = false;
                        m_ismoveX = false;
                        m_scrollSVIP->setTouchEnabled(true);
                    }else{
                        m_vipLevelSVIP++;
                        PlayComeInFromRightAnim();
                    }
                    
                }else{
                    if(m_vipLevelSVIP<=1){
//                        m_ismoving = false;
//                        m_ismoveX = false;
//                        m_scrollSVIP->setTouchEnabled(true);
                        PlayNodeExchangeAnim(1);
                    }else{
                        m_vipLevelSVIP--;
                        PlayComeInFromLeftAnim();
                    }
                }
            }
        }else{
            m_ismoving = false;
            m_ismoveX = false;
            m_scroll->setTouchEnabled(true);
            m_scrollSVIP->setTouchEnabled(true);
        }
    }
    m_touchState = 0;
}
void VipDetailView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(m_touchState == 1 || m_touchState == 2)
        return;
    
    float dx =pTouch->getLocation().x-m_touchPoint.x;
    float dy =pTouch->getLocation().y-m_touchPoint.y;
    if(fabs(dx)<10&&fabs(dy)<10){
        return;
    }
    
    if(fabs(dy)>=fabs(dx)){//竖向
        m_ismoveX = false;
    }else{
        m_ismoving = true;
        m_ismoveX = true;
        m_scroll->setTouchEnabled(false);
        m_scrollSVIP->setTouchEnabled(false);
    }
}
void VipDetailView::PlayComeInFromRightAnim(){
    PlayComeOutFromRightAnim();
    VipDetailCell* cell = nullptr;
    if(m_vipType==1){
        if(m_vipLevel==10){
            cell = VipDetailCell::create(-1,0,2);
        }else{
            cell = VipDetailCell::create(m_vipLevel-1);
        }
        m_listNode->addChild(cell, 100, 1001);
    }else if(m_vipType==2){
        cell = VipDetailCell::create(m_vipLevelSVIP-1,0,2);
        m_listNodeSVIP->addChild(cell, 100, 1001);
    }else{
        return;
    }

    float x = cell->getPositionX();
    float y = cell->getPositionY();
    cell->setPositionX(this->getPositionX()+cell->getContentSize().width);
    CCMoveTo* userMove = CCMoveTo::create(0.3f, ccp(x,y));
    cell->runAction(CCSequence::create(
                                       CCEaseSineOut::create(userMove)
                                       , CCCallFunc::create(this, callfunc_selector(VipDetailView::ComeInAnimFinish))
                                       , NULL
                                       ));

}
void VipDetailView::ComeInAnimFinish(){
    
    
}
void VipDetailView::PlayComeOutFromRightAnim(){
    VipDetailCell* outNode = nullptr;
    if(m_vipType==1){
        outNode = dynamic_cast<VipDetailCell*>(m_listNode->getChildByTag(1000));
    }else if(m_vipType==2){
        outNode = dynamic_cast<VipDetailCell*>(m_listNodeSVIP->getChildByTag(1000));
    }else{
        return;
    }

    float x = outNode->getPositionX()-outNode->getContentSize().width;
    float y = outNode->getPositionY();
    CCMoveTo* userMove = CCMoveTo::create(0.3f, ccp(x,y));
    outNode->runAction(CCSequence::create(
                                                  CCEaseSineOut::create(userMove)
                                                  , CCCallFunc::create(this, callfunc_selector(VipDetailView::ComeOutAnimFinish))
                                                  , NULL
                                                  ));
}
void VipDetailView::ComeOutAnimFinish(){
    if(m_vipType==1){
        VipDetailCell* outNode = dynamic_cast<VipDetailCell*>(m_listNode->getChildByTag(1001));
        m_listNode->removeChildByTag(1000);
        outNode->setTag(1000);
        if(m_movedh>0){
            outNode->setPositionY(outNode->getPositionY()-m_movedh);
            m_movedh = 0;
        }
        m_ismoving = false;
        m_scroll->setTouchEnabled(true);
        refreshPageFlag();
        m_scroll->setContentSize(CCSize(m_listContainer->getContentSize().width, outNode->getHG()));
        m_scroll->setContentOffset(ccp(0, m_listContainer->getContentSize().height - (outNode->getHG())));
    }else if(m_vipType==2){
        VipDetailCell* outNode = dynamic_cast<VipDetailCell*>(m_listNodeSVIP->getChildByTag(1001));
        m_listNodeSVIP->removeChildByTag(1000);
        outNode->setTag(1000);
        if(m_movedhSVIP>0){
            outNode->setPositionY(outNode->getPositionY()-m_movedhSVIP);
            m_movedhSVIP = 0;
        }
        m_ismoving = false;
        m_scrollSVIP->setTouchEnabled(true);
        refreshPageFlag();
        m_scrollSVIP->setContentSize(CCSize(m_listContainerSVIP->getContentSize().width, outNode->getHG()));
        m_scrollSVIP->setContentOffset(ccp(0, m_listContainerSVIP->getContentSize().height - (outNode->getHG())));
    }
}
void VipDetailView::PlayComeInFromLeftAnim(){
    CCNode* listNode = nullptr;
    VipDetailCell* cell = nullptr;
    int level =1;
    if(m_vipType==1){
        listNode = m_listNode;
        level = m_vipLevel;
//        cell = VipDetailCell::create(level-1);
        if(level==10){
            cell = VipDetailCell::create(-1,0,2);
        }else{
            cell = VipDetailCell::create(level-1);
        }
    }else if(m_vipType==2){
        listNode = m_listNodeSVIP;
        level = m_vipLevelSVIP;
        cell = VipDetailCell::create(m_vipLevelSVIP-1,0,2);
    }else{
        return;
    }
    
    VipDetailCell* outNode = dynamic_cast<VipDetailCell*>(listNode->getChildByTag(1000));
    PlayComeOutFromLeftAnim();
    listNode->addChild(cell, 100, 1001);

    if(cell->getHG()<outNode->getHG()){
        if(m_vipType==1){
            m_movedh =(outNode->getHG()-cell->getHG());
            cell->setPositionY(cell->getPositionY()+m_movedh);
        }else if(m_vipType==2){
            m_movedhSVIP =(outNode->getHG()-cell->getHG());
            cell->setPositionY(cell->getPositionY()+m_movedhSVIP);
        }
    }
    float x = cell->getPositionX();
    float y = cell->getPositionY();
    cell->setPositionX(this->getPositionX()-cell->getContentSize().width);
    CCMoveTo* userMove = CCMoveTo::create(0.3f, ccp(x,y));
    cell->runAction(CCSequence::create(
                                       CCEaseSineOut::create(userMove)
                                       , CCCallFunc::create(this, callfunc_selector(VipDetailView::ComeInAnimFinish))
                                       , NULL
                                       ));
    
}
void VipDetailView::PlayComeOutFromLeftAnim(){
//    auto outNode = m_listNode->getChildByTag(1000);
//    if(m_vipType==2){
//        outNode = m_listNodeSVIP->getChildByTag(1000);
//    }
    VipDetailCell* outNode = nullptr;
    if(m_vipType==1){
        outNode = dynamic_cast<VipDetailCell*>(m_listNode->getChildByTag(1000));
    }else if(m_vipType==2){
        outNode = dynamic_cast<VipDetailCell*>(m_listNodeSVIP->getChildByTag(1000));
    }else{
        return;
    }
    float x = outNode->getPositionX()+outNode->getContentSize().width;
    float y = outNode->getPositionY();
    CCMoveTo* userMove = CCMoveTo::create(0.3f, ccp(x,y));
    outNode->runAction(CCSequence::create(
                                          CCEaseSineOut::create(userMove)
                                          , CCCallFunc::create(this, callfunc_selector(VipDetailView::ComeOutAnimFinish))
                                          , NULL
                                          ));
}
void VipDetailView::refreshPageFlag(){
    if(m_vipType==1){
        if(VipUtil::isSVIP() && m_vipLevel==10){    //SVIP暂不考虑
            this->m_currentVIPText->setString(_lang_1("103001",CC_ITOA(m_vipLevel)));
            this->m_NextVIPText->setString(_lang_1("103076","1"));
            this->m_pageFlag->setPositionX(-114+29*(m_vipLevel-1));
            
            m_vipNodeLeft->setVisible(true);
            m_svipNodeLeft->setVisible(false);
            m_vipNodeRight->setVisible(true);
            m_svipNodeRight->setVisible(true);
        }else{
            this->m_currentVIPText->setString(_lang_1("103001",CC_ITOA(m_vipLevel)));
            this->m_NextVIPText->setString(_lang_1("103001",CC_ITOA(m_vipLevel+1)));
            this->m_pageFlag->setPositionX(-114+29*(m_vipLevel-1));
            if(m_vipLevel+1 == 10)
            {
                m_vipNodeRight->setScaleX(1.2);
            }
            if(m_vipLevel  == 10)
            {
                m_vipNodeLeft->setScaleX(1.2);
            }
            m_vipNodeLeft->setVisible(true);
            m_svipNodeLeft->setVisible(false);
            m_vipNodeRight->setVisible(true);
            m_svipNodeRight->setVisible(false);
        }
    }else if(VipUtil::isSVIP() && m_vipType==2){
        this->m_currentVIPText->setString(_lang_1("103076", CC_ITOA(m_vipLevelSVIP)));
        this->m_NextVIPText->setString(_lang_1("103076", CC_ITOA(m_vipLevelSVIP+1)));
        this->m_pageFlagSVIP->setPositionX(-201+29*(m_vipLevelSVIP-1));
        m_vipNodeRight->setScaleX(1.2);
        m_vipNodeLeft->setScaleX(1.2);
        
        m_vipNodeLeft->setVisible(true);
        m_svipNodeLeft->setVisible(false);
        m_vipNodeRight->setVisible(true);
        m_svipNodeRight->setVisible(false);
    }
}

#pragma mark PlayNodeExchangeAnim
void VipDetailView::PlayNodeExchangeAnim(int type){
    if(type==0){//vip->svip
        CCPoint savePoint = m_listContainer->getPosition();
        m_listContainerSVIP->setPosition(m_listContainer->getPositionX()+m_listContainer->getContentSize().width, m_listContainer->getPositionY());
        m_listContainerSVIP->setVisible(true);
        
        CCMoveBy* userMove1 = CCMoveBy::create(0.3f, ccp(-m_listContainer->getContentSize().width,0));
        m_listContainer->runAction(userMove1);
        CCMoveTo* userMove2 = CCMoveTo::create(0.3f, savePoint);
        m_listContainerSVIP->runAction(CCSequence::create(userMove2, CCCallFunc::create(this, callfunc_selector(VipDetailView::setVipType2)), NULL));
    }else if (type==1){//svip->vip
        CCPoint savePoint = m_listContainerSVIP->getPosition();
        m_listContainer->setPosition(m_listContainerSVIP->getPositionX()-m_listContainerSVIP->getContentSize().width, m_listContainerSVIP->getPositionY());
        m_listContainer->setVisible(true);
        
        CCMoveBy* userMove1 = CCMoveBy::create(0.3f, ccp(m_listContainerSVIP->getContentSize().width,0));
        m_listContainerSVIP->runAction(userMove1);
        CCMoveTo* userMove2 = CCMoveTo::create(0.3f, savePoint);
        m_listContainer->runAction(CCSequence::create(userMove2, CCCallFunc::create(this, callfunc_selector(VipDetailView::setVipType1)), NULL));
    }
}

void VipDetailView::setVipType1(){
    setVipType(1);
    m_ismoving = false;
    m_ismoveX = false;
    m_scrollSVIP->setTouchEnabled(true);
    m_listContainerSVIP->setPosition(m_listContainer->getPosition());
}
void VipDetailView::setVipType2(){
    setVipType(2);
    m_ismoving = false;
    m_ismoveX = false;
    m_scroll->setTouchEnabled(true);
    m_listContainer->setPosition(m_listContainerSVIP->getPosition());
}

void VipDetailView::setVipType(int v_type){
    m_vipType = v_type;
    if (m_vipType==1) {//vip
        if (VipUtil::isSVIP() && curPoint>=vip10Left && curPoint<vip10Right) {
            m_listContainer->setVisible(false);
            m_downNode->setVisible(false);
            m_listContainerSVIP->setVisible(true);
            m_downNodeSVIP->setVisible(true);
        }
        m_listContainer->setVisible(true);
        m_downNode->setVisible(true);
        m_sprLastPoint->setVisible(VipUtil::isSVIP());
        m_listContainerSVIP->setVisible(false);
        m_downNodeSVIP->setVisible(false);
    }else if (VipUtil::isSVIP() && m_vipType==2){//svip
        m_listContainer->setVisible(false);
        m_downNode->setVisible(false);
        m_listContainerSVIP->setVisible(true);
        m_downNodeSVIP->setVisible(true);
    }
    refreshPageFlag();
}

#pragma mark setSvipView
void VipDetailView::setSvipView(){
    auto &playerInfo = GlobalData::shared()->playerInfo;
    if(VipUtil::isSVIP() && playerInfo.SVIPLevel>0){
        m_BgNodeSVIP->setVisible(true);
        m_svipUpTextSpr->setVisible(true);
        m_vipLvText->setString(CC_ITOA(playerInfo.SVIPLevel));//
        m_vipLvText->setPositionX(-45);
        m_sprActive[0]->setDisplayFrame(CCLoadSprite::loadResource("SVIP_huangsequanquan.png"));
        m_headBgVip->setVisible(false);
        m_headBgSvip->setVisible(true);
        setVipType(2);
    }else{
        
        m_BgNodeSVIP->setVisible(false);
        m_svipUpTextSpr->setVisible(false);
        m_headBgVip->setVisible(true);
        m_headBgSvip->setVisible(false);
    }
}
