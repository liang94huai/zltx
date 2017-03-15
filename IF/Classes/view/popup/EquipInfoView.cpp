//
//  EquipInfoView.cpp
//  IF
//
//  Created by 付彬 on 16/1/6.
//
//

#include "EquipInfoView.hpp"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "RollingText.h"
#include "YesNoDialog.h"
#include "FunBuildController.h"
#include "QueueController.h"
#include "EquipmentController.h"
#include "SoundController.h"
#include "StoreView.h"
#include "EquipMeltView.hpp"
#include "EquipComposeView.hpp"
#include "MateCreateView.hpp"
#include "EquipmentBagView.h"

#define eq_pro_color {66,156,224}

EquipInfoView* EquipInfoView::createWithEquipId(int equipId){
    auto ret = new EquipInfoView();
    if (ret && ret->initWithEquipId(equipId)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
bool EquipInfoView::initWithEquipId(int equipId){
    bool ret = true;
    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    
    if (PopupBaseView::init()) {
        setIsHDPanel(true);
        CCBLoadFile("EquipInfoView",this,this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        //隐藏分解
        m_disNode->setVisible(false);
        m_propertyNode->setVisible(true);
        m_type = 3;
        setDataByEquipId(equipId);
        this->setTitleName("");
    }
    return ret;
}

void EquipInfoView::setDataByEquipId(int equipId)
{
    //    m_mateBtn->setVisible(false);
    m_waitInterface = NULL;
    m_uuid = "";
    
    m_itemId = equipId;
    
    m_msg1Label->setVisible(false);
    m_destroyBtn->setVisible(false);
    CCCommonUtils::setButtonTitle(m_mateBtn, _lang("110031").c_str());//确定
    if (CCCommonUtils::isIosAndroidPad()) {
        m_mateBtn->setPreferredSize(CCSize(260, 80));
        m_mateBtn->setPosition(ccp(0,m_destroyBtn->getPositionY()));
    }
    else {
        m_mateBtn->setPreferredSize(CCSize(244, 80));
        m_mateBtn->setPosition(ccp(0,-270));
    }
    
    m_nameLabel->setString(_lang("119051"));
    if (CCCommonUtils::isIosAndroidPad()) {
        m_msg1Label->setFontSize(48);
    }
    else
        m_msg1Label->setFontSize(24);
    EquipmentInfo& info = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    if (info.site==6) {
        info=EquipmentController::getInstance()->LongEquipmentInfoMap[m_uuid];
    }
    m_curHaveOnUuid="";
    
    string strName = _lang(info.name) + "   "+_lang("102203")+" " + CC_ITOA(info.level);
    m_name1Label->setString(strName);
    m_msg1Label->setString(_lang("119057"));
    if (!CCCommonUtils::isIosAndroidPad()) {
        m_msg1Label->setPositionY(-230);
        m_nodeEndLine->setPositionY(-209);
    }
    if(info.showType == "no")
    {
        m_desLabel->setVisible(true);
        m_desLabel->setString(_lang(info.des));
    }
    else
    {
        m_msg2Label->setString(_lang("119056"));// 属性提升
        m_desLabel->setVisible(false);
        
        vector<int> TmpNum;
        if (info.site==6) {
            TmpNum.push_back(1);
            TmpNum.push_back(3);
            TmpNum.push_back(5);
            TmpNum.push_back(2);
            TmpNum.push_back(4);
        }else {
            TmpNum.push_back(1);
            TmpNum.push_back(2);
            TmpNum.push_back(3);
            TmpNum.push_back(4);
            TmpNum.push_back(5);
        }
        
        int j = 1;
        for (int i=0; i<TmpNum.size(); i++)
        {
            int tNum = TmpNum[i];
            string tmpInfo = "";
            if (info.showDias.find(tNum) != info.showDias.end()) {
                string dialog = info.showDias[tNum];
                if (dialog!="" && info.values.find(tNum)!=info.values.end()) {
                    string pf = info.getEffFormatByOrd(tNum);
                    string pm = info.getEffPMByOrd(tNum);
                    if (pf=="") {
                        tmpInfo = tmpInfo +pm+CC_ITOA(info.values[tNum])+pf;
                    }else {
                        tmpInfo = tmpInfo +pm+CC_ITOA_MONEY(info.values[tNum])+pf;
                    }
                }
                
                if (j==1) {
                    m_info1Label->setString(_lang(dialog));
                    m_num1Label->setString(tmpInfo);
                }else if (j==2) {
                    m_info2Label->setString(_lang(dialog));
                    m_num2Label->setString(tmpInfo);
                    if (info.site==6 && (tNum==2||tNum==4)) {
                        m_info2Label->setColor(eq_pro_color);
                        m_num2Label->setColor(eq_pro_color);
                    }
                }else if (j==3) {
                    m_info3Label->setString(_lang(dialog));
                    m_num3Label->setString(tmpInfo);
                    if (info.site==6 && (tNum==2||tNum==4)) {
                        m_info3Label->setColor(eq_pro_color);
                        m_num3Label->setColor(eq_pro_color);
                    }
                }else if (j==4) {
                    m_info4Label->setString(_lang(dialog));
                    m_num4Label->setString(tmpInfo);
                    if (info.site==6 && (tNum==2||tNum==4)) {
                        m_info4Label->setColor(eq_pro_color);
                        m_num4Label->setColor(eq_pro_color);
                    }
                }else if (j==5) {
                    m_info5Label->setString(_lang(dialog));
                    m_num5Label->setString(tmpInfo);
                    if (info.site==6 && (tNum==2||tNum==4)) {
                        m_info5Label->setColor(eq_pro_color);
                        m_num5Label->setColor(eq_pro_color);
                    }
                }
                j++;
            }
        }
    }
    
    string iconPath = CCCommonUtils::getIcon(CC_ITOA(m_itemId));
    string bgPath = CCCommonUtils::getToolBgByColor(info.color);
    
    auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_EQUIP);
    if (CCCommonUtils::isIosAndroidPad()) {
        CCCommonUtils::setSpriteMaxSize(icon, 200, true);
    }
    else
        CCCommonUtils::setSpriteMaxSize(icon, 100, true);
    auto bg = CCLoadSprite::createSprite(bgPath.c_str());
    if (CCCommonUtils::isIosAndroidPad()) {
        CCCommonUtils::setSpriteMaxSize(bg, 200, true);
    }
    else
        CCCommonUtils::setSpriteMaxSize(bg, 100, true);
    m_picNode->addChild(bg);
    m_picNode->addChild(icon);
}

EquipInfoView* EquipInfoView::create(string uuid, bool isAni)
{
    auto ret = new EquipInfoView();
    if (ret && ret->init(uuid, isAni)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipInfoView::init(string uuid, bool isAni)
{
    bool ret = true;
    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    
    if (PopupBaseView::init()) {
        setIsHDPanel(true);
        CCBLoadFile("EquipInfoView",this,this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        //隐藏分解
        m_isAni = isAni;
        m_disNode->setVisible(false);
        m_propertyNode->setVisible(true);
        m_type = 0;
        setData(uuid);
    }
    return ret;
}

EquipInfoView* EquipInfoView::createWithBrokenEquip(int itemId,int count,int nextCount){
    auto ret = new EquipInfoView();
    if (ret && ret->initWithBrokenEquip(itemId,count,nextCount)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipInfoView::initWithBrokenEquip(int itemId,int count,int nextCount){
    bool ret = true;
    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    
    if (PopupBaseView::init()) {
        setIsHDPanel(true);
        CCBLoadFile("EquipInfoView",this,this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        //隐藏分解
        m_disNode->setVisible(false);
        m_propertyNode->setVisible(true);
        this->m_type = 1;
        this->m_itemId = itemId;
        this->m_itemCount = count;
        this->m_itemNextCount = nextCount;
        
        auto frame = CCLoadSprite::loadResource("btn_01.png");
        auto graySpr = CCLoadSprite::createSprite("btn_01.png");
        auto lbl = CCLabelIF::create();
        if (CCCommonUtils::isIosAndroidPad()) {
            graySpr->setScaleX(390.0/graySpr->getContentSize().width);
            graySpr->setScaleY(145.0/graySpr->getContentSize().height);
            CCCommonUtils::setSpriteGray(graySpr, true);
            m_mateBtn->getParent()->addChild(graySpr);
            lbl->setColor(ccGRAY);
            lbl->setFontSize(40);
            m_mateBtn->getParent()->addChild(lbl);
            lbl->setString(_lang("119028"));
        }
        else {
            graySpr->setScaleX(195.0/graySpr->getContentSize().width);
            graySpr->setScaleY(78.0/graySpr->getContentSize().height);
            CCCommonUtils::setSpriteGray(graySpr, true);
            m_mateBtn->getParent()->addChild(graySpr);
            lbl->setColor(ccGRAY);
            lbl->setFontSize(20);
            m_mateBtn->getParent()->addChild(lbl);
            lbl->setString(_lang("119028"));
        }
        
        auto &eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
        m_nameLabel->setString(_lang("119051"));
        std::string strName = _lang(eInfo.name);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_msg1Label->setFontSize(40);
        }
        else
            m_msg1Label->setFontSize(20);
        m_name1Label->setString(strName);
        m_msg2Label->setString(_lang("108730"));//说明
        m_desLabel->setVisible(true);
        m_desLabel->setString(_lang(eInfo.des));
        m_nodeEndLine->setPositionY(-180);
        m_msg1Label->setPositionY(-220);
        if (eInfo.color == GOLDEN){
            m_mateBtn->setEnabled(false);
            graySpr->setVisible(true);
            lbl->setVisible(true);
            m_msg1Label->setVisible(true);
            m_msg1Label->setString(_lang("121987"));
        }
        else if(m_itemCount < 1) // 数量不足
        {
            m_mateBtn->setEnabled(false);
            graySpr->setVisible(true);
            lbl->setVisible(true);
            m_msg1Label->setVisible(true);
            m_msg1Label->setString(_lang("119080"));
        }
        else
        {
            graySpr->setVisible(false);
            lbl->setVisible(false);
            m_mateBtn->setEnabled(true);
            m_msg1Label->setVisible(false);
        }
        
        string iconPath = CCCommonUtils::getIcon(CC_ITOA(m_itemId));
        string bgPath = CCCommonUtils::getToolBgByColor(eInfo.color);
        
        auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_EQUIP);
        if (CCCommonUtils::isIosAndroidPad()) {
            CCCommonUtils::setSpriteMaxSize(icon, 200, true);
        }
        else
            CCCommonUtils::setSpriteMaxSize(icon, 100, true);
        auto bg = CCLoadSprite::createSprite(bgPath.c_str());
        if (CCCommonUtils::isIosAndroidPad()) {
            CCCommonUtils::setSpriteMaxSize(bg, 200, true);
        }
        else
            CCCommonUtils::setSpriteMaxSize(bg, 100, true);
        m_picNode->addChild(bg);
        m_picNode->addChild(icon);
        
        CCCommonUtils::setButtonTitle(m_mateBtn, _lang("119028").c_str());
        CCCommonUtils::setButtonTitle(m_destroyBtn, _lang("119052").c_str());
        
        m_destroyBtn->setPreferredSize(CCSize(200, 80));
        m_destroyBtn->setPosition(ccp(-120,-283));
        m_mateBtn->setPreferredSize(CCSize(200, 80));
        m_mateBtn->setPosition(ccp(120,-283));
        if (CCCommonUtils::isIosAndroidPad()) {
            m_mateBtn->setPosition(ccp(180,-480));
            m_destroyBtn->setPosition(ccp(-180,-480));
            m_desLabel->setPositionY(-200);
        }
        graySpr->setPosition(m_mateBtn->getPosition());
        lbl->setPosition(m_mateBtn->getPosition());
    }
    return ret;
}
void EquipInfoView::setData(string uuid)
{
    m_waitInterface = NULL;
    m_uuid = uuid;
    
    CCCommonUtils::setButtonTitle(m_composeBtn, _lang("119028").c_str());
    m_itemId = 0;
    if (EquipmentController::getInstance()->m_MyOnEquip.find(m_uuid) != EquipmentController::getInstance()->m_MyOnEquip.end())
    {//显示 卸下
        m_itemId = EquipmentController::getInstance()->m_MyOnEquip[m_uuid];
        m_msg1Label->setVisible(false);
        m_destroyBtn->setVisible(false);
        CCCommonUtils::setButtonTitle(m_mateBtn, _lang("119003").c_str());
        //        if (CCCommonUtils::isIosAndroidPad()) {
        //            m_mateBtn->setPreferredSize(CCSize(260, 80));
        //            m_mateBtn->setPosition(ccp(0,m_destroyBtn->getPositionY()));
        //        }
        //        else {
        //            m_mateBtn->setPreferredSize(CCSize(244, 80));
        //            m_mateBtn->setPosition(ccp(0,-270));
        //        }
    }
    else
    {
        m_itemId = EquipmentController::getInstance()->m_MyEquipmentMap[m_uuid];
        m_msg1Label->setVisible(false);
        m_destroyBtn->setVisible(true);
        CCCommonUtils::setButtonTitle(m_mateBtn, _lang("119002").c_str());
    }
    
    m_nameLabel->setString(_lang("119051"));
    if (CCCommonUtils::isIosAndroidPad()) {
        m_msg1Label->setFontSize(48);
    }
    else {
        m_msg1Label->setFontSize(24);
    }
    
    EquipmentInfo& info = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    if (info.site==6) {
        info=EquipmentController::getInstance()->LongEquipmentInfoMap[m_uuid];
    }
    
    //查找该部位是否已经装备
    m_curHaveOnUuid = EquipmentController::getInstance()->FindOnEquipBySite(info.site);
    
    string strName = _lang(info.name) + "   "+_lang("102203")+" " + CC_ITOA(info.level);
    m_name1Label->setString(strName);
    m_msg1Label->setString(_lang("119057"));
    if (!CCCommonUtils::isIosAndroidPad()) {
        m_msg1Label->setPositionY(-230);
        m_nodeEndLine->setPositionY(-209);
    }
    if(info.showType == "no")
    {
        m_desLabel->setVisible(true);
        m_desLabel->setString(_lang(info.des));
    }
    else
    {
        m_msg2Label->setString(_lang("119056"));// 属性提升
        m_desLabel->setVisible(false);
        
        vector<int> TmpNum;
        if (info.site==6) {
            TmpNum.push_back(1);
            TmpNum.push_back(3);
            TmpNum.push_back(5);
            TmpNum.push_back(2);
            TmpNum.push_back(4);
            
            m_msg1Label->setVisible(true);
            m_msg1Label->setString(_lang(""));
            
        }else {
            TmpNum.push_back(1);
            TmpNum.push_back(2);
            TmpNum.push_back(3);
            TmpNum.push_back(4);
            TmpNum.push_back(5);
        }
        
        int j = 1;
        for (int i=0; i<TmpNum.size(); i++)
        {
            int tNum = TmpNum[i];
            string tmpInfo = "";
            if (info.showDias.find(tNum) != info.showDias.end()) {
                string dialog = info.showDias[tNum];
                if (dialog!="" && info.values.find(tNum)!=info.values.end()) {
                    string pf = info.getEffFormatByOrd(tNum);
                    string pm = info.getEffPMByOrd(tNum);
                    if (pf=="") {
                        tmpInfo = tmpInfo +pm+CC_ITOA(info.values[tNum])+pf;
                    }else {
                        tmpInfo = tmpInfo +pm+CC_ITOA_MONEY(info.values[tNum])+pf;
                    }
                }
                
                if (j==1) {
                    m_info1Label->setString(_lang(dialog));
                    m_num1Label->setString(tmpInfo);
                }else if (j==2) {
                    m_info2Label->setString(_lang(dialog));
                    m_num2Label->setString(tmpInfo);
                    if (info.site==6 && (tNum==2||tNum==4)) {
                        m_info2Label->setColor(eq_pro_color);
                        m_num2Label->setColor(eq_pro_color);
                    }
                }else if (j==3) {
                    m_info3Label->setString(_lang(dialog));
                    m_num3Label->setString(tmpInfo);
                    if (info.site==6 && (tNum==2||tNum==4)) {
                        m_info3Label->setColor(eq_pro_color);
                        m_num3Label->setColor(eq_pro_color);
                    }
                }else if (j==4) {
                    m_info4Label->setString(_lang(dialog));
                    m_num4Label->setString(tmpInfo);
                    if (info.site==6 && (tNum==2||tNum==4)) {
                        m_info4Label->setColor(eq_pro_color);
                        m_num4Label->setColor(eq_pro_color);
                    }
                }else if (j==5) {
                    m_info5Label->setString(_lang(dialog));
                    m_num5Label->setString(tmpInfo);
                    if (info.site==6 && (tNum==2||tNum==4)) {
                        m_info5Label->setColor(eq_pro_color);
                        m_num5Label->setColor(eq_pro_color);
                    }
                }
                j++;
            }
        }
    }
    
    string iconPath = CCCommonUtils::getIcon(CC_ITOA(m_itemId));
    string bgPath = CCCommonUtils::getToolBgByColor(info.color);
    
    auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_EQUIP);
    if (CCCommonUtils::isIosAndroidPad()) {
        CCCommonUtils::setSpriteMaxSize(icon, 200, true);
    }
    else
        CCCommonUtils::setSpriteMaxSize(icon, 100, true);
    auto bg = CCLoadSprite::createSprite(bgPath.c_str());
    if (CCCommonUtils::isIosAndroidPad()) {
        CCCommonUtils::setSpriteMaxSize(bg, 200, true);
    }
    else
        CCCommonUtils::setSpriteMaxSize(bg, 100, true);
    m_picNode->addChild(bg);
    m_picNode->addChild(icon);
    //特效
    if(info.site==6){
        m_particle->removeAllChildren();
        for (int par=0; par<=2; par++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("DragonCristal_%d_%d",info.color,par)->getCString());
            m_particle->addChild(particle);
        }
        if (CCCommonUtils::isIosAndroidPad()) {
            m_particle->setScale(2.0);
        }
    }
    
    CCCommonUtils::setButtonTitle(m_destroyBtn, _lang("119052").c_str());
    if (info.site==6) {
        CCCommonUtils::setButtonTitle(m_destroyBtn, _lang("111692").c_str());//洗练
        if (!EquipmentController::getInstance()->IsOpenMeltFunc())
        {
            m_destroyBtn->setEnabled(false);
            m_destroyBtn->setVisible(false);
            if (CCCommonUtils::isIosAndroidPad()) {
                m_mateBtn->setPosition(ccp(0,m_destroyBtn->getPositionY()));
                m_mateBtn->setPreferredSize(CCSize(260, 80));
            }else {
                m_mateBtn->setPreferredSize(CCSize(244, 80));
                m_mateBtn->setPosition(ccp(0,-270));
            }
        }
        else {
            m_destroyBtn->setEnabled(true);
            m_destroyBtn->setVisible(true);
            
            if (CCCommonUtils::isIosAndroidPad()) {
                m_mateBtn->setPosition(ccp(240,m_destroyBtn->getPositionY()));
                m_destroyBtn->setPosition(ccp(-240,m_destroyBtn->getPositionY()));
                m_mateBtn->setPreferredSize(CCSize(260, 80));
            }else {
                m_mateBtn->setPosition(ccp(120,m_destroyBtn->getPositionY()));
                m_destroyBtn->setPosition(ccp(-120,m_destroyBtn->getPositionY()));
                m_mateBtn->setPreferredSize(CCSize(200, 80));
            }
        }
    }
    else {
        if (info.universal==1) {//万能装备
            m_disNode->setVisible(false);
            m_propertyNode->setVisible(true);
            m_msg1Label->setString("");
            
            m_nameLabel->setString(_lang("119051"));
            std::string strName = _lang(info.name);
            if (CCCommonUtils::isIosAndroidPad()) {
                m_msg1Label->setFontSize(40);
            }
            else {
                m_msg1Label->setFontSize(20);
            }
            
            m_name1Label->setString(strName);
            m_msg2Label->setString(_lang("108730"));//说明
            m_desLabel->setVisible(true);
            m_desLabel->setString(_lang(info.des));
            m_nodeEndLine->setPositionY(-180);
            m_msg1Label->setPositionY(-220);
            if (info.color == GOLDEN){
                m_msg1Label->setVisible(true);
                m_msg1Label->setString(_lang("121987"));
            }
            else
            {
                m_msg1Label->setVisible(false);
            }
            
            m_composeBtn->setPosition(m_mateBtn->getPosition());
            m_composeBtn->setScale(m_mateBtn->getScale());
            m_composeBtn->setVisible(true);
            m_mateBtn->setVisible(false);
        }
        else {
            if (info.color == GOLDEN){
                m_msg1Label->setVisible(true);
                m_msg1Label->setString(_lang("121987"));
            }
            if (CCCommonUtils::isIosAndroidPad()) {
                m_destroyBtn->setScale(1.3);
                m_destroyBtn->setVisible(true);
                m_destroyBtn->setPosition(ccp(-340, m_destroyBtn->getPositionY()));
                
                m_mateBtn->setScale(1.3);
                m_mateBtn->setPosition(ccp(0,m_destroyBtn->getPositionY()));
                
                m_composeBtn->setVisible(true);
            }else {
                m_destroyBtn->setScale(0.8);
                m_destroyBtn->setVisible(true);
                m_destroyBtn->setPosition(ccp(-180, m_destroyBtn->getPositionY()));
                
                m_mateBtn->setScale(0.8);
                m_mateBtn->setPosition(ccp(0,m_destroyBtn->getPositionY()));
                
                m_composeBtn->setVisible(true);
            }
        }
    }
}

void EquipInfoView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipInfoView::refreashData), EQUIP_BAG_SHOW_DES, NULL);
    
    if (m_isAni) {
        playBombEff();
        this->getAnimationManager()->runAnimations("FadeIn");
    }
}

void EquipInfoView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this,EQUIP_BAG_SHOW_DES);
    setTouchEnabled(false);
    CCNode::onExit();
}

bool EquipInfoView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_touchNode,pTouch)){
        return false;
    }
    return true;
}

void EquipInfoView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isTouchInside(m_touchNode,pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
    }
}

SEL_CCControlHandler EquipInfoView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickDestroyBtn", EquipInfoView::onClickDestroyBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickMateBtn", EquipInfoView::onClickMateBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickComposeBtn", EquipInfoView::onClickComposeBtn);
    return NULL;
}

bool EquipInfoView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeEndLine", CCNode*, m_nodeEndLine);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateBtn", CCControlButton*, m_mateBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_destroyBtn", CCControlButton*, m_destroyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_composeBtn", CCControlButton*, m_composeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name1Label", CCLabelIF*, m_name1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg1Label", CCLabelIF*, m_msg1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg2Label", CCLabelIF*, m_msg2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particle", CCNode*, m_particle);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info1Label", CCLabelIF*, m_info1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num1Label", CCLabelIF*, m_num1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info2Label", CCLabelIF*, m_info2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num2Label", CCLabelIF*, m_num2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info3Label", CCLabelIF*, m_info3Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num3Label", CCLabelIF*, m_num3Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info4Label", CCLabelIF*, m_info4Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num4Label", CCLabelIF*, m_num4Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_info5Label", CCLabelIF*, m_info5Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num5Label", CCLabelIF*, m_num5Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    //分解
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_propertyNode", CCNode*, m_propertyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_disNode", CCNode*, m_disNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_disSubNode", CCNode*, m_disSubNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_disText", CCLabelIF*, m_disText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_disCount", CCLabelIF*, m_disCount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_disX", CCLabelIF*, m_disX);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rationNode", CCNode*, m_rationNode);
    
    return false;
}

void EquipInfoView::playBombEff()
{
    auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    int cellw = 100;
    int bgw = 120;
    if (CCCommonUtils::isIosAndroidPad()) {
        cellw = 200;
        bgw = 240;
    }
    
    auto m_playNode = CCNode::create();
    m_rationNode->addChild(m_playNode);
    auto playBg = CCLoadSprite::createSprite("Items_icon_kuang.png");
    CCCommonUtils::setSpriteMaxSize(playBg, bgw, true);
    m_playNode->addChild(playBg);
    
    string bgPath = CCCommonUtils::getToolBgByColor(eInfo.color);
    auto bgIcon = CCLoadSprite::createSprite(bgPath.c_str());
    CCCommonUtils::setSpriteMaxSize(bgIcon, cellw, true);
    m_playNode->addChild(bgIcon);
    
    string iconPath = CCCommonUtils::getIcon(CC_ITOA(m_itemId));
    auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_EQUIP);
    CCCommonUtils::setSpriteMaxSize(icon, cellw, true);
    m_playNode->addChild(icon);
    
    CCActionInterval * rotateTo1 = CCRotateTo::create(0.1,180);
    CCActionInterval * rotateTo2 = CCRotateTo::create(0.1,360);
    CCActionInterval * rotateTo3 = CCRotateTo::create(0,0);
    CCActionInterval * repeat = CCRepeat::create(CCSequence::create(rotateTo1,rotateTo2,rotateTo3,NULL), 3);
    
    CCActionInterval * scaleTo0 = CCScaleTo::create(0.6,1.5);
    CCFiniteTimeAction * spawn0 =CCSpawn::create(repeat,scaleTo0,NULL);
    
    CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(EquipInfoView::playParticle));
    CCActionInterval * delayT = CCDelayTime::create(0.5);
    CCActionInterval * scaleTo1 = CCScaleTo::create(0.2,1.0);
    
    m_playNode->setScale(0);
    m_playNode->runAction(CCSequence::create(spawn0, funcall, delayT, scaleTo1, NULL));
}
void EquipInfoView::playParticle()
{
    //    m_rationNode->removeAllChildren();
}

void EquipInfoView::onClickComposeBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (EquipmentController::getInstance()->m_MyOnEquip.find(m_uuid) != EquipmentController::getInstance()->m_MyOnEquip.end())
    {
        CCCommonUtils::flyHint("", "", _lang("111748"));
        return;
    }
    else {
        EquipmentInfo& info = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
        if (info.color == GOLDEN){
            CCCommonUtils::flyHint("", "", _lang("121987"));
            return;
        }
        bool st = CCCommonUtils::isOpenFormDataCfg(EquipmentController::getInstance()->m_equipComposeKey);
        if (st) {
            PopupViewController::getInstance()->removePopupView(this);
            PopupViewController::getInstance()->addPopupInView(EquipComposeView::create(m_uuid));
        }
        else {
            CCCommonUtils::flyHint("", "", _lang("111753"));
        }
    }
}

void EquipInfoView::onClickDestroyBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    EquipmentInfo& info = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    if (info.site==6) {
        //        info=EquipmentController::getInstance()->LongEquipmentInfoMap[m_uuid];
        //        YesNoDialog::show(_lang_2("111657",  _lang(info.name).c_str(), info.cost.c_str()), CCCallFunc::create(this, callfunc_selector(EquipInfoView::onYesDestroyLongEquip)));
        
        string ran = CCCommonUtils::getPropById(CC_ITOA(m_itemId), "ran");
        if (ran == "") {
            CCCommonUtils::flyHint("", "", _lang("111719"));
            return;
        }
        
        PopupViewController::getInstance()->addPopupView(EquipMeltView::create(m_uuid));
        PopupViewController::getInstance()->removePopupView(this);
        
    }else{
        if (EquipmentController::getInstance()->m_MyOnEquip.find(m_uuid) != EquipmentController::getInstance()->m_MyOnEquip.end())
        {
            CCCommonUtils::flyHint("", "", _lang("111748"));
            return;
        }
        
        string colorName = CCCommonUtils::getColorName(info.color);
        //        YesNoDialog::show(_lang_3("119053", CC_ITOA(info.break_down), CC_ITOA(info.break_up), colorName.c_str()), CCCallFunc::create(this, callfunc_selector(EquipInfoView::onYesDestroy)));
        
        //分解
        m_disNode->setVisible(true);
        m_propertyNode->setVisible(false);
        
        string iconPath = CCCommonUtils::getIcon(CC_ITOA(m_itemId));
        string bgPath = CCCommonUtils::getToolBgByColor(info.color);
        auto icon = CCLoadSprite::createSprite("equip_dis_wenhao.png");
        CCCommonUtils::setSpriteMaxSize(icon, 40, true);
        auto bg = CCLoadSprite::createSprite(bgPath.c_str());
        //        if (CCCommonUtils::isIosAndroidPad()) {
        //            CCCommonUtils::setSpriteMaxSize(bg, 80, true);
        //        }
        //        else{
        CCCommonUtils::setSpriteMaxSize(bg, 40, true);
        //        }
        m_disSubNode->addChild(bg);
        m_disSubNode->addChild(icon);
        m_disText->setString(_lang_2("111682", CC_ITOA(info.break_up), colorName.c_str()));
        m_disCount->setString(CC_ITOA(info.break_down));
        m_disX->setString("X");
        
        m_destroyBtn->setVisible(false);
        m_composeBtn->setVisible(false);
        CCCommonUtils::setButtonTitle(m_mateBtn, _lang("111683").c_str());
        m_mateBtn->setVisible(true);
        if (CCCommonUtils::isIosAndroidPad()) {
            m_mateBtn->setPreferredSize(CCSize(260, 80));
            m_mateBtn->setPosition(ccp(0,m_destroyBtn->getPositionY()));
            m_mateBtn->setScale(1.5);
        }
        else {
            m_mateBtn->setPreferredSize(CCSize(244, 80));
            m_mateBtn->setPosition(ccp(0,-270));
            m_mateBtn->setScale(1.0);
        }
        if (!CCCommonUtils::isIosAndroidPad()) {
            m_msg1Label->setPositionY(-230);
            m_nodeEndLine->setPositionY(-209);
        }else{
            m_nodeEndLine->setPositionY(-215);
        }
        m_desLabel->setVisible(false);
        m_msg2Label->setString(_lang("108730"));//说明
        if(m_type==1){
            auto iter = EquipmentController::getInstance()->m_MyEquipmentMap.begin();
            while (iter!= EquipmentController::getInstance()->m_MyEquipmentMap.end()) {
                if((*iter).second == m_itemId){
                    m_uuid = (*iter).first;
                    break;
                }
                ++iter;
            }
        }
        m_type=2;
        //        this->scheduleOnce(schedule_selector(EquipInfoView::showDisassembleAni),0.2);
        showDisassembleAni(NULL);
        
    }
}

void EquipInfoView::showDisassembleAni(float t){
    float scale = 1.6;
    if (CCCommonUtils::isIosAndroidPad()) {
        scale = 2.0;
    }
    auto cArray = m_disSubNode->getChildren();
    for (int i=0; i<cArray.size(); i++) {
        CCNode *node =dynamic_cast<CCNode*>(cArray.at(i));
        CCActionInterval *scaleBy=CCScaleBy::create(0.1, scale);
        node->runAction(CCSequence::create(scaleBy,scaleBy->reverse(),NULL));
    }
    CCActionInterval *delay = CCDelayTime::create(0.2);
    CCActionInterval *labScaleBy=CCScaleBy::create(0.1, scale);
    m_disCount->runAction(CCSequence::create(delay,labScaleBy,labScaleBy->reverse(),NULL));
}

void EquipInfoView::onClickMateBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_type==1) {
        //材料合成
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("setFromWorkshop",CCInteger::create(0));
        PopupViewController::getInstance()->addPopupInView(MateCreateView::create(m_itemId,1,m_itemCount,m_itemNextCount));
        closeSelf();
    }else if(m_type==0){
        if (m_uuid==m_curHaveOnUuid) {//卸下
            EquipmentController::getInstance()->startTakeOffEquipment(m_uuid);
        }else{//装备
            EquipmentController::getInstance()->startPutOnEquipment(m_uuid, m_curHaveOnUuid);
        }
        closeSelf();
    }else if (m_type==2){
        //分解装备
        EquipInfoView::onYesDestroy();
    }else if (m_type==3){
        PopupViewController::getInstance()->removePopupView(this);
    }
}

void EquipInfoView::onYesDestroy()
{
    if(!m_uuid.empty()){
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_mateBtn);
        EquipmentController::getInstance()->startDesEquipment(m_uuid);
    }
}
void EquipInfoView::onYesDestroyLongEquip(){
    if(!m_uuid.empty()){
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_destroyBtn);
        EquipmentController::getInstance()->startSellLongEquipment(m_uuid);
    }
}

void EquipInfoView::refreashData(CCObject* obj)
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    if (obj) {
        CCString* mateData = dynamic_cast<CCString*>(obj);
        if (mateData) {
            string mateMsg = mateData->getCString();
            
            PopupViewController::getInstance()->addPopupView(EquipBreakItemView::create(mateMsg));
            PopupViewController::getInstance()->removePopupView(this);
        }
    }else{
        PopupViewController::getInstance()->removePopupView(this);
    }
}
