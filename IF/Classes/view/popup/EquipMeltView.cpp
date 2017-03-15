//
//  EquipMeltView.cpp
//  IF
//
//  Created by 付彬 on 15/12/7.
//
//

#include "EquipMeltView.hpp"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "RollingText.h"
#include "YesNoDialog.h"
#include "FunBuildController.h"
#include "QueueController.h"
#include "EquipmentController.h"
#include "TipsView.h"
#include "CCMathUtils.h"

EquipMeltView* EquipMeltView::create(string uuid)
{
    auto ret = new EquipMeltView();
    if (ret && ret->init(uuid)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipMeltView::init(string uuid)
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
        CCBLoadFile("EquipMeltView",this,this);
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        m_propertyNode->setVisible(true);
        m_desNode->setVisible(false);
        setData(uuid);
    }
    return ret;
}

void EquipMeltView::setData(string uuid)
{
    m_waitInterface = NULL;
    m_uuid = uuid;
    
    m_itemId = 0;
    if (EquipmentController::getInstance()->m_MyOnEquip.find(m_uuid) != EquipmentController::getInstance()->m_MyOnEquip.end()) {
        m_itemId = EquipmentController::getInstance()->m_MyOnEquip[m_uuid];
    }else {
        m_itemId = EquipmentController::getInstance()->m_MyEquipmentMap[m_uuid];
    }
    
    m_nameLabel->setString(_lang("111692"));
    
    EquipmentInfo& info = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    if (info.site==6) {
        info=EquipmentController::getInstance()->LongEquipmentInfoMap[m_uuid];
    }
    //查找该部位是否已经装备
    m_curHaveOnUuid="";
    int curSite = info.site;
    map<string, int>::iterator it = EquipmentController::getInstance()->m_MyOnEquip.begin();
    for (; it!=EquipmentController::getInstance()->m_MyOnEquip.end(); it++) {
        auto& tmpInfo = EquipmentController::getInstance()->EquipmentInfoMap[it->second];
        if (tmpInfo.site == curSite) {
            m_curHaveOnUuid = it->first;
            break;
        }
    }
    
    string strName = _lang(info.name) + "   "+_lang("102203")+" " + CC_ITOA(info.level);
    m_name1Label->setString(strName);
    
    m_proNum = 0;
    m_msg2Label->setString(_lang("119056"));// 属性提升
    for (int i=1; i<=5; i++)
    {
        string tmpInfo = "";
        if (info.showDias.find(i) != info.showDias.end()) {
            string dialog = info.showDias[i];
            if (dialog!="" && info.values.find(i)!=info.values.end()) {
                string pf = info.getEffFormatByOrd(i);
                string pm = info.getEffPMByOrd(i);
                if (pf=="") {
                    tmpInfo = tmpInfo +pm+CC_ITOA(info.values[i])+pf;
                }else {
                    tmpInfo = tmpInfo +pm+CC_ITOA_MONEY(info.values[i])+pf;
                }
            }
            if (i==1) {
                m_info1Label->setString(_lang(dialog));
                m_num1Label->setString(tmpInfo);
                m_proNum = 1;
            }else if (i==2) {
                m_info2Label->setString(_lang(dialog));
                m_num2Label->setString(tmpInfo);
            }else if (i==3) {
                m_info3Label->setString(_lang(dialog));
                m_num3Label->setString(tmpInfo);
                m_proNum = 2;
            }else if (i==4) {
                m_info4Label->setString(_lang(dialog));
                m_num4Label->setString(tmpInfo);
            }else if (i==5) {
                m_info5Label->setString(_lang(dialog));
                m_num5Label->setString(tmpInfo);
                m_proNum = 3;
            }
        }
    }
    
    string iconPath = CCCommonUtils::getIcon(CC_ITOA(m_itemId));
    string bgPath = CCCommonUtils::getToolBgByColor(info.color);
    auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_EQUIP);
    auto bg = CCLoadSprite::createSprite(bgPath.c_str());
    CCCommonUtils::setSpriteMaxSize(bg, 100, true);
    CCCommonUtils::setSpriteMaxSize(icon, 100, true);
    
    if (m_proNum == 1) {
        m_l1Node->setPosition(m_l3Node->getPosition());
    }
    
    m_picNode->addChild(bg);
    m_picNode->addChild(icon);
    //特效
    if(info.site==6){
        m_particle->removeAllChildren();
        for (int par=0; par<=2; par++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("DragonCristal_%d_%d",info.color,par)->getCString());
            m_particle->addChild(particle);
        }
    }
    
    refreashData(NULL);
    makeRandData();
}

void EquipMeltView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipMeltView::refreashData), EQUIP_MELT_SHOW, NULL);
}

void EquipMeltView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this,EQUIP_MELT_SHOW);
    setTouchEnabled(false);
    CCNode::onExit();
}

bool EquipMeltView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void EquipMeltView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isTouchInside(m_touchNode,pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
    }
    else {
        if (m_desNode->isVisible()) {
            m_desNode->setVisible(false);
            return;
        }
    }
}

SEL_CCControlHandler EquipMeltView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickMeltBtn", EquipMeltView::onClickMeltBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickSaveBtn", EquipMeltView::onClickSaveBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickTipBtn", EquipMeltView::onClickTipBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickHelpBtn", EquipMeltView::onClickHelpBtn);
    return NULL;
}

bool EquipMeltView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeEndLine", CCNode*, m_nodeEndLine);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_saveBtn", CCControlButton*, m_saveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_meltBtn", CCControlButton*, m_meltBtn);
    
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
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoEx2Label", CCLabelIF*, m_infoEx2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numEx2Label", CCLabelIF*, m_numEx2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoEx4Label", CCLabelIF*, m_infoEx4Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numEx4Label", CCLabelIF*, m_numEx4Label);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_l1Node", CCNode*, m_l1Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_l2Node", CCNode*, m_l2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_l3Node", CCNode*, m_l3Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_l4Node", CCNode*, m_l4Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_l5Node", CCNode*, m_l5Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lEx2Node", CCNode*, m_lEx2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lEx4Node", CCNode*, m_lEx4Node);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_freeNode", CCNode*, m_freeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnMsgLabel", CCLabelIF*, m_btnMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inBtnGoldNum", CCLabelIF*, m_inBtnGoldNum);
    //分解
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_propertyNode", CCNode*, m_propertyNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipBtn", CCControlButton*, m_tipBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode", CCNode*, m_desNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desbg", CCScale9Sprite*, m_desbg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desTitleNode", CCNode*, m_desTitleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_disLabelNode", CCNode*, m_disLabelNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_disValueNode", CCNode*, m_disValueNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desName", CCLabelIF*, m_desName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLine1", CCSprite*, m_desLine1);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpBtn", CCControlButton*, m_helpBtn);
    return false;
}

void EquipMeltView::onClickMeltBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_desNode->isVisible()) {
        m_desNode->setVisible(false);
        return;
    }
    
    int refTimes = EquipmentController::getInstance()->m_refineTimes;
    int freeTimes = EquipmentController::getInstance()->m_refineFreeTimes;
    if (EquipmentController::getInstance()->m_isTip && refTimes >= freeTimes) {
        string cost = CCCommonUtils::getPropById(CC_ITOA(m_itemId), "refine_cost");
        YesNoDialog::show(_lang_1("111713", cost.c_str()), CCCallFunc::create(this, callfunc_selector(EquipMeltView::onYesMelt)));
        EquipmentController::getInstance()->m_isTip = false;
    }
    else {
        onYesMelt();
    }
}

void EquipMeltView::onClickSaveBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_desNode->isVisible()) {
        m_desNode->setVisible(false);
        return;
    }
    m_waitInterface = GameController::getInstance()->showWaitInterface(m_touchNode);
    m_waitInterface->setPosition(m_touchNode->getContentSize().width/2, m_touchNode->getContentSize().height/2);
    EquipmentController::getInstance()->startSaveMeltEquip(m_uuid);
}

void EquipMeltView::onClickHelpBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("111721"),kCCTextAlignmentLeft));
}

void EquipMeltView::onClickTipBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    m_desName->setString(_lang("111720"));
    string randomId = CCCommonUtils::getPropById(CC_ITOA(m_itemId), "ran");
    string dialogs = CCCommonUtils::getPropById(randomId, "dialog");
    string minNums = CCCommonUtils::getPropById(randomId, "min");
    string maxNums = CCCommonUtils::getPropById(randomId, "max");
    string effNums = CCCommonUtils::getPropById(randomId, "effect");
    vector<string> dialogVec;
    CCCommonUtils::splitString(dialogs, "|", dialogVec);
    vector<string> minVec;
    CCCommonUtils::splitString(minNums, "|", minVec);
    vector<string> maxVec;
    CCCommonUtils::splitString(maxNums, "|", maxVec);
    vector<string> effVec;
    CCCommonUtils::splitString(effNums, "|", effVec);
    
    m_disLabelNode->removeAllChildren();
    m_disValueNode->removeAllChildren();
    int curY = 0;
    int maxW1 = 0;
    int maxW2 = 0;
    for (int i=0; ; i++)
    {
        if (dialogVec.size()>i && minVec.size()>i && maxVec.size()>i && effVec.size()>i)
        {
            auto diaLabel = CCLabelIF::create(_lang(dialogVec[i]).c_str(), 20, CCSizeMake(0, 0), kCCTextAlignmentLeft, kCCVerticalTextAlignmentBottom);
            diaLabel->setAnchorPoint(ccp(0, 0));
            diaLabel->setColor(ccWHITE);
            diaLabel->setPosition(0, curY);
            m_disLabelNode->addChild(diaLabel);
            diaLabel->setColor(ccc3(197, 152, 15));
            int labelH = diaLabel->getContentSize().height*diaLabel->getOriginScaleY();
            int labelW1 = diaLabel->getContentSize().width*diaLabel->getOriginScaleX();
            if (labelW1>350) {
                diaLabel->setDimensions(CCSizeMake(350, 0));
                labelH = diaLabel->getContentSize().height*diaLabel->getOriginScaleY();
                labelW1 = diaLabel->getContentSize().width*diaLabel->getOriginScaleX();
            }
            maxW1 = MAX(maxW1, labelW1);
            
            vector<string> tmpEffNumVec;
            CCCommonUtils::splitString(effVec[i], ";", tmpEffNumVec);
            string format = "%";
            if (tmpEffNumVec.size()>0) {
                format = CCCommonUtils::getEffFormat(tmpEffNumVec[0]);
            }
            string valueInfo = minVec[i]+format+"~"+maxVec[i]+format;
            auto valLabel = CCLabelIF::create(valueInfo.c_str(), 20, CCSizeMake(0, 0), kCCTextAlignmentRight, kCCVerticalTextAlignmentBottom);
            valLabel->setAnchorPoint(ccp(1, 0));
            valLabel->setColor(ccWHITE);
            valLabel->setPosition(0, curY);
            m_disValueNode->addChild(valLabel);
            valLabel->setColor(ccc3(65, 183, 40));
            int labelW2 = valLabel->getContentSize().width*valLabel->getOriginScaleX();
            maxW2 = MAX(maxW2, labelW2);
            
            curY += labelH;
        }
        else {
            break;
        }
    }
    
    int sumW = 40 + maxW1 + 50 + maxW2 +40;
    int sumH = 40 + curY + 30 + 40;
    m_desNode->setContentSize(CCSizeMake(sumW, sumH));
    m_desbg->setPreferredSize(CCSizeMake(sumW, sumH));
    m_desbg->setPosition(sumW, 0);
    m_disLabelNode->setPosition(ccp(40, 40));
    m_disValueNode->setPosition(ccp(sumW-40, 40));
    m_desTitleNode->setPosition(ccp(sumW/2, 50+curY));
    
    int lw = m_desLine1->getContentSize().width;
    float lpro = sumW*0.8/lw;
    m_desLine1->setScaleX(lpro);
    
    m_desNode->setVisible(true);
}

void EquipMeltView::onYesMelt()
{
    m_waitInterface = GameController::getInstance()->showWaitInterface(m_touchNode);
    m_waitInterface->setPosition(m_touchNode->getContentSize().width/2, m_touchNode->getContentSize().height/2);
    
    CCActionInterval * delaytime = CCDelayTime::create(0.1);
    CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(EquipMeltView::playRandEff));
    CCSequence* sequence = CCSequence::create(delaytime,funcall, NULL);
    CCRepeat* repeat = CCRepeat::create(sequence, 5);
    CCCallFunc * lfuncall= CCCallFunc::create(this, callfunc_selector(EquipMeltView::onLastMelt));
    this->runAction(CCSequence::create(repeat, lfuncall, NULL));
}

void EquipMeltView::onLastMelt()
{
    EquipmentInfo& info = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    if (info.site==6 && m_uuid!="") {
        EquipmentController::getInstance()->startMeltEquip(m_uuid);
    }
}

void EquipMeltView::makeRandData()
{
    string randomId = CCCommonUtils::getPropById(CC_ITOA(m_itemId), "ran");
    string dialogs = CCCommonUtils::getPropById(randomId, "dialog");
    string minNums = CCCommonUtils::getPropById(randomId, "min");
    string maxNums = CCCommonUtils::getPropById(randomId, "max");
    string effNums = CCCommonUtils::getPropById(randomId, "effect");
    vector<string> dialogVec;
    CCCommonUtils::splitString(dialogs, "|", dialogVec);
    vector<string> minVec;
    CCCommonUtils::splitString(minNums, "|", minVec);
    vector<string> maxVec;
    CCCommonUtils::splitString(maxNums, "|", maxVec);
    vector<string> effVec;
    CCCommonUtils::splitString(effNums, "|", effVec);
    
    for (int i=0; ; i++)
    {
        if (dialogVec.size()>i && minVec.size()>i && maxVec.size()>i && effVec.size()>i)
        {
            vector<string> tmpEffNumVec;
            CCCommonUtils::splitString(effVec[i], ";", tmpEffNumVec);
            string format = "%";
            string pm = "+";
            if (tmpEffNumVec.size()>0) {
                format = CCCommonUtils::getEffFormat(tmpEffNumVec[0]);
                pm = CCCommonUtils::getEffPM(tmpEffNumVec[0]);
            }
            string msg2 = pm+maxVec[i]+format;
            m_randData.push_back( pair<string,string>(_lang(dialogVec[i]), msg2) );
        }
        else {
            break;
        }
    }
}

void EquipMeltView::playRandEff()
{
    int rIdx = CCMathUtils::getRandomInt(0, m_randData.size()-1);
    m_infoEx2Label->setString( m_randData[rIdx].first );
    m_numEx2Label->setString( m_randData[rIdx].second );
    
    if (m_info4Label->getString() != "") {
        int lIdx = CCMathUtils::getRandomInt(0, m_randData.size()-1);
        m_infoEx4Label->setString( m_randData[lIdx].first );
        m_numEx4Label->setString( m_randData[lIdx].second );
    }
}

void EquipMeltView::refreashData(CCObject* obj)
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    CCCommonUtils::setButtonTitle(m_saveBtn, _lang("111693").c_str());
    
    m_infoEx2Label->setString("");
    m_numEx2Label->setString("");
    m_infoEx4Label->setString("");
    m_numEx4Label->setString("");
    EquipmentInfo& info = EquipmentController::getInstance()->LongEquipmentInfoMap[m_uuid];
    
    int refTimes = EquipmentController::getInstance()->m_refineTimes;
    int freeTimes = EquipmentController::getInstance()->m_refineFreeTimes;
    if (refTimes >= freeTimes)
    {
        string cost = CCCommonUtils::getPropById(CC_ITOA(info.itemId), "refine_cost");
        CCCommonUtils::setButtonTitle(m_meltBtn, "");
        m_btnMsgLabel->setString(_lang("111692"));
        m_inBtnGoldNum->setString(cost);
        m_freeNode->setVisible(true);
        m_msg1Label->setString(_lang_1("111690", CC_ITOA(0)));
    }
    else {
        m_msg1Label->setString(_lang_1("111690", CC_ITOA(freeTimes - refTimes)));
        CCCommonUtils::setButtonTitle(m_meltBtn, _lang("111691").c_str());
        m_freeNode->setVisible(false);
    }
    
    if (info.refineProperty != "")
    {//显示替换属性
        m_saveBtn->setEnabled(true);
        
        vector<string> pVec2;
        vector<string> nVec2;
        vector<string> sVec2;
        vector<string> vec;
        CCCommonUtils::splitString(info.refineProperty, "|", vec);
        for (int i=0; i<vec.size(); i++) {
            string vStr = vec[i];
            vector<string> vec1;
            CCCommonUtils::splitString(vStr, ",", vec1);
            if (vec1.size()==3) {
                pVec2.push_back(vec1[0]);
                nVec2.push_back(vec1[1]);
                sVec2.push_back(vec1[2]);
            }
        }
        
        if (pVec2.size()>=1 && nVec2.size()>=1 && sVec2.size()>=1) {
            string tmpInfo = "";
            string pf = CCCommonUtils::getEffFormat(pVec2[0]);
            string pm = CCCommonUtils::getEffPM(pVec2[0]);
            if (pf=="") {
                tmpInfo = tmpInfo +pm+CC_ITOA(atoi(nVec2[0].c_str()))+pf;
            }else {
                tmpInfo = tmpInfo +pm+CC_ITOA_MONEY( atof(nVec2[0].c_str()) )+pf;
            }
            m_infoEx2Label->setString(_lang(sVec2[0]));
            m_numEx2Label->setString(tmpInfo);
        }
        
        if (pVec2.size()>=2 && nVec2.size()>=2 && sVec2.size()>=2) {
            string tmpInfo = "";
            string pf = CCCommonUtils::getEffFormat(pVec2[1]);
            string pm = CCCommonUtils::getEffPM(pVec2[1]);
            if (pf=="") {
                tmpInfo = tmpInfo +pm+CC_ITOA(atoi(nVec2[1].c_str()))+pf;
            }else {
                tmpInfo = tmpInfo +pm+CC_ITOA_MONEY( atof(nVec2[1].c_str()) )+pf;
            }
            m_infoEx4Label->setString(_lang(sVec2[1]));
            m_numEx4Label->setString(tmpInfo);
        }
        
    }
    else
    {
        m_saveBtn->setEnabled(false);
        
        for (int i=1; i<=5; i++)
        {
            string tmpInfo = "";
            if (info.showDias.find(i) != info.showDias.end()) {
                string dialog = info.showDias[i];
                if (dialog!="" && info.values.find(i)!=info.values.end()) {
                    string pf = info.getEffFormatByOrd(i);
                    string pm = info.getEffPMByOrd(i);
                    if (pf=="") {
                        tmpInfo = tmpInfo +pm+CC_ITOA(info.values[i])+pf;
                    }else {
                        tmpInfo = tmpInfo +pm+CC_ITOA_MONEY(info.values[i])+pf;
                    }
                }
                if (i==1) {
                    m_info1Label->setString(_lang(dialog));
                    m_num1Label->setString(tmpInfo);
                }else if (i==2) {
                    m_info2Label->setString(_lang(dialog));
                    m_num2Label->setString(tmpInfo);
                    m_infoEx2Label->setString(_lang("111722"));
                }else if (i==3) {
                    m_info3Label->setString(_lang(dialog));
                    m_num3Label->setString(tmpInfo);
                }else if (i==4) {
                    m_info4Label->setString(_lang(dialog));
                    m_num4Label->setString(tmpInfo);
                    m_infoEx4Label->setString(_lang("111722"));
                }else if (i==5) {
                    m_info5Label->setString(_lang(dialog));
                    m_num5Label->setString(tmpInfo);
                }
            }
        }
    }
    
    if (m_infoEx4Label->getString()=="") {
        m_l2Node->setPositionY( m_l4Node->getPositionY()+15 );
        m_lEx2Node->setPositionY( m_lEx4Node->getPositionY()+15 );
    }
    
//    int lH = -30;
//    if (m_proNum == 1) {
//        m_l2Node->setPosition(m_l1Node->getPosition() + ccp(0, lH));
//    }else if (m_proNum == 2) {
//        m_l2Node->setPosition(m_l3Node->getPosition() + ccp(0, lH));
//    }else if (m_proNum == 3) {
//        m_l2Node->setPosition(m_l5Node->getPosition() + ccp(0, lH));
//    }
//    m_lEx2Node->setPosition(m_l2Node->getPosition() + ccp(0, lH));
//    
//    m_l4Node->setPosition(m_lEx2Node->getPosition() + ccp(0, lH));
//    m_lEx4Node->setPosition(m_l4Node->getPosition() + ccp(0, lH));
}
