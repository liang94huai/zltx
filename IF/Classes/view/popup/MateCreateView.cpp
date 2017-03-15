//
//  MateCreateView.cpp
//  IF
//
//  Created by 付彬 on 16/1/6.
//
//

#include "MateCreateView.hpp"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "RollingText.h"
#include "YesNoDialog.h"
#include "FunBuildController.h"
#include "QueueController.h"
#include "EquipmentController.h"
#include "SoundController.h"
#include "ToolCreateView.h"
#include "SceneContainer.h"
#include "ImperialScene.h"
#include "StoreView.h"
#include "EquipMeltView.hpp"
#include "EquipComposeView.hpp"

MateCreateView* MateCreateView::create(int itemId,int type,int count,int nextCount)
{
    auto ret = new MateCreateView();
    if (ret && ret->init(itemId,type,count,nextCount)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MateCreateView::init(int itemId,int type,int count,int nextCount)
{
    bool ret = true;
    if (!PopupBaseView::init()) {
        return false;
    }
    m_type = type;
    
    CCLoadSprite::doResourceByCommonIndex(207, true);
    CCLoadSprite::doResourceByCommonIndex(306, true);
    CCLoadSprite::doResourceByCommonIndex(305, true);
    CCLoadSprite::doResourceByCommonIndex(11, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(207, false);
        CCLoadSprite::doResourceByCommonIndex(306, false);
        CCLoadSprite::doResourceByCommonIndex(305, false);
        CCLoadSprite::doResourceByCommonIndex(11, false);
    });
    
    setIsHDPanel(true);
    auto tmpCCB = CCBLoadFile("MateCreateView",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else
        this->setContentSize(tmpCCB->getContentSize());
    CCCommonUtils::setButtonTitle(m_crtBtn, _lang("119028").c_str());
    
    if (!CCCommonUtils::isIosAndroidPad()) {
        int extH = getExtendHeight();
        if(extH>=284){
            m_midNode->setScale(1.05f);
        }
        if(extH<=10){
            m_midNode->setScale(0.9f);
        }
        m_midNode->setPositionY(m_midNode->getPositionY()-extH/2);
        m_bottomNode->setPositionY(m_bottomNode->getPositionY()-extH);
        m_mateNode->setPositionY(m_mateNode->getPositionY()-extH+5);
        m_leftBG->setContentSize(CCSizeMake(m_leftBG->getContentSize().width, m_leftBG->getContentSize().height+extH));
        m_rightBG->setContentSize(CCSizeMake(m_rightBG->getContentSize().width, m_rightBG->getContentSize().height+extH));
        
    }
    m_popLayer = CCLayer::create();
    setTitleName(_lang("119028"));
    setPrcNodeTag();
    m_waitInterface = NULL;
    m_needLabel->setString(_lang("119020"));
    m_needLabel->setVisible(false);
    
    setData(itemId,count,nextCount);
    setParticle();
    
    return ret;
}

void MateCreateView::setPrcNodeTag()
{
    m_mateNode1->setTag(1);
    m_mateNode2->setTag(2);
    m_mateNode3->setTag(3);
    m_mateNode4->setTag(4);
    m_mateNode5->setTag(5);
}

void MateCreateView::setParticle(){
    playCanCrtParticle();
    switch (m_type) {
        case 1:{
            auto &eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
            m_colorT = eInfo.color+1;
            break;
        }
        default:{
            ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
            m_colorT = info.color+1;
            break;
        }
    }
    if (m_colorT > GOLDEN) {
        m_colorT = GOLDEN;
    }
    
    if (m_colorT == GREEN) {
        tmpStartColorVec.push_back(ccc4f(0, 0.588235294, 0, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.588235294, 1, 0.392156863, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.647058824, 0.784313725, 0, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0, 0.196078431, 0, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.392156863, 0.784313725, 0, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.588235294, 1, 0.392156863, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.588235294, 1, 0.392156863, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.588235294, 1, 0.392156863, 0.392156863));
        
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0.196078431, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0.196078431, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0.196078431, 0, 0));
        tmpEndColorVec.push_back(ccc4f(1, 0.784313725, 0.392156863, 0.392156863));
        tmpEndColorVec.push_back(ccc4f(0, 0.196078431, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0.196078431, 0, 0));
    }else if (m_colorT == BLUE) {
        tmpStartColorVec.push_back(ccc4f(0.011764706, 0.011764706, 1, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.392156863, 0.784313725, 1, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.392156863, 0.784313725, 1, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0, 0, 0.294117647, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.392156863, 0.784313725, 1, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.098039216, 0.392156863, 1, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.392156863, 0.784313725, 1, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.392156863, 0.784313725, 1, 0.392156863));
        
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0.196078431, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0.196078431, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0.196078431, 0));
        tmpEndColorVec.push_back(ccc4f(1, 1, 1, 0.392156863));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0.196078431, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0.196078431, 0));
    }else if (m_colorT == PURPLE) {
        tmpStartColorVec.push_back(ccc4f(0.196078431, 0.156862745, 1, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.784313725, 0.588235294, 1, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.784313725, 0.588235294, 1, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.058823529, 0.039215686, 0.294117647, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.784313725, 0.588235294, 1, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.058823529, 0.137254902, 0.784313725, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.784313725, 0.588235294, 1, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.784313725, 0.588235294, 1, 0.392156863));
        
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0.196078431, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0.196078431, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0.117647059, 0.019607843, 0.254901961, 0));
        tmpEndColorVec.push_back(ccc4f(1, 1, 1, 0.392156863));
        tmpEndColorVec.push_back(ccc4f(0, 0.019607843, 0.196078431, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0.019607843, 0.196078431, 0));
    }else if (m_colorT == ORANGE) {
        tmpStartColorVec.push_back(ccc4f(1, 0.098039216, 0, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.784313725, 0.098039216, 0, 0.196078431));
        tmpStartColorVec.push_back(ccc4f(1, 0.588235294, 0.098039216, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.196078431, 0.019607843, 0, 0.294117647));
        tmpStartColorVec.push_back(ccc4f(1, 0.392156863, 0.117647059, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(1, 0.294117647, 0, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(1, 0.784313725, 0.392156863, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(1, 0.784313725, 0.392156863, 0.392156863));
        
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0.058823529, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0.058823529, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(1, 0.784313725, 0.392156863, 0.392156863));
        tmpEndColorVec.push_back(ccc4f(0.392156863, 0.019607843, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0.392156863, 0.019607843, 0, 0));
    }else if (m_colorT == GOLDEN) {
        tmpStartColorVec.push_back(ccc4f(1, 0.784313725, 0, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(1, 0.784313725, 0, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(1, 0.784313725, 0, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.196078431, 0.156862745, 0, 0.294117647));
        tmpStartColorVec.push_back(ccc4f(0.588235294, 0.392156863, 0, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(0.588235294, 0.392156863, 0, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(1, 0.784313725, 0, 0.392156863));
        tmpStartColorVec.push_back(ccc4f(1, 0.784313725, 0, 0.392156863));
        
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(1, 0.882352941, 0.588235294, 0.392156863));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
        tmpEndColorVec.push_back(ccc4f(0, 0, 0, 0));
    }
}

void MateCreateView::setData(int itemId,int count,int nextCount)
{
    //    if(m_waitInterface!=NULL){
    //        m_waitInterface->remove();
    //        m_waitInterface = NULL;
    //    }
    m_itemId = itemId;
    
    
    m_mateNode1->removeAllChildren();
    m_mateNode2->removeAllChildren();
    m_mateNode3->removeAllChildren();
    m_mateNode4->removeAllChildren();
    m_mateNode5->removeAllChildren();
    m_mateBgNode5->removeAllChildren();
    
    for (int i=0; i<6; i++) {
        m_selectSpr[i]->setVisible(false);
    }
    
    
    int color;
    int id;
    switch (m_type) {
        case 1:{
            equipCountMap.clear();
            map<string, int>::iterator eit = EquipmentController::getInstance()->m_MyEquipmentMap.begin();
            for (; eit!=EquipmentController::getInstance()->m_MyEquipmentMap.end(); eit++)
            {
                auto& tmpInfo = EquipmentController::getInstance()->EquipmentInfoMap[eit->second];
                if(tmpInfo.showType == "no")
                {
                    int newCount = equipCountMap[eit->second] + 1;
                    equipCountMap[eit->second] = newCount;
                }
                
            }
            
            auto eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
            color = eInfo.color;
            id = eInfo.itemId;
            m_itemCount = count;
            m_itemCount = equipCountMap[eInfo.itemId];
            m_itemNextCount = nextCount;
            m_itemNextCount = equipCountMap[eInfo.itemId+1];
            
            equipVec.clear();
            map<int, EquipmentInfo>::iterator it;
            for(it=EquipmentController::getInstance()->EquipmentInfoMap.begin();it!=EquipmentController::getInstance()->EquipmentInfoMap.end();++it){
                if (it->second.des==eInfo.des) {
                    equipVec.push_back(it->second);
                }
            }
            
            if (equipVec.size()>0 && equipVec.size()<=6) {
                for(int i=0;i<equipVec.size();i++){
                    EquipmentInfo& temInfo = equipVec[i];
                    string iconPath = CCCommonUtils::getIcon(CC_ITOA(temInfo.itemId));
                    string bgPath = CCCommonUtils::getToolBgByColor(temInfo.color);
                    auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_EQUIP);
                    CCCommonUtils::setSpriteMaxSize(icon, 80);
                    auto bg = CCLoadSprite::createSprite(bgPath.c_str());
                    CCCommonUtils::setSpriteMaxSize(bg, 80);
                    m_sprNode[i]->removeAllChildren();
                    m_sprNode[i]->addChild(bg);
                    m_sprNode[i]->addChild(icon);
                    m_num[i]->setString(CC_ITOA(equipCountMap[temInfo.itemId]));
                    if (temInfo.itemId==eInfo.itemId) {
                        m_selectSpr[i]->setVisible(true);
                    }
                }
            }
            
            break;
        }
        default:{
            ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
            color = info.color;
            id = info.itemId;
            m_itemCount = info.getCNT();
            ToolInfo& nextInfo = ToolController::getInstance()->getToolInfoById(m_itemId+1);
            m_itemNextCount =nextInfo.getCNT();
            
            toolVec.clear();
            map<int, ToolInfo>::iterator it;
            for(it=ToolController::getInstance()->m_toolInfos.begin();it!=ToolController::getInstance()->m_toolInfos.end();++it){
                if (it->second.des==info.des) {
                    toolVec.push_back(it->second);
                }
            }
            
            if (toolVec.size()>0 && toolVec.size()<=6) {
                for(int i=0;i<toolVec.size();i++){
                    ToolInfo& temInfo = toolVec[i];
                    string iconPath = CCCommonUtils::getIcon(CC_ITOA(temInfo.itemId));
                    string bgPath = CCCommonUtils::getToolBgByColor(temInfo.color);
                    auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_GOODS);
                    CCCommonUtils::setSpriteMaxSize(icon, 80);
                    auto bg = CCLoadSprite::createSprite(bgPath.c_str());
                    CCCommonUtils::setSpriteMaxSize(bg, 80);
                    m_sprNode[i]->removeAllChildren();
                    m_sprNode[i]->addChild(bg);
                    m_sprNode[i]->addChild(icon);
                    m_num[i]->setString(CC_ITOA(temInfo.getCNT()));
                    if (temInfo.itemId==info.itemId) {
                        m_selectSpr[i]->setVisible(true);
                    }
                }
            }
            break;
        }
    }
    
    //    m_infoLabel->setString(_lang(info.des));
    
    if (color < GOLDEN) {
        string items = "";
        //        if(BagViewItemId.find(BROKEN_EQUIP_SPLIT)!=string::npos){
        //            vector<string> vect;
        //            CCCommonUtils::splitString(BagViewItemId, BROKEN_EQUIP_SPLIT, vect);
        //            if(vect.size()>0){
        //                items = vect.at(0);
        //            }
        //        }else{
        //            items = BagViewItemId;
        //        }
        items = CC_ITOA(id);
        
        auto addSpr1 = CCLoadSprite::createSprite("Forging_jiahao.png");
        m_mateNode1->addChild(addSpr1);
        place1=false;
        auto addSpr2 = CCLoadSprite::createSprite("Forging_jiahao.png");
        m_mateNode2->addChild(addSpr2);
        place2=false;
        auto addSpr3 = CCLoadSprite::createSprite("Forging_jiahao.png");
        m_mateNode3->addChild(addSpr3);
        place3=false;
        auto addSpr4 = CCLoadSprite::createSprite("Forging_jiahao.png");
        m_mateNode4->addChild(addSpr4);
        place4=false;
        
        string iconPath = CCCommonUtils::getIcon(items);
        string bgPath = CCCommonUtils::getToolBgByColor(color);
        string mateName = CCCommonUtils::getNameById(items);
        for (int i=0; i<m_itemCount; i++) {
            auto icon = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_GOODS);
            CCCommonUtils::setSpriteMaxSize(icon, 100);
            auto bg = CCLoadSprite::createSprite(bgPath.c_str());
            CCCommonUtils::setSpriteMaxSize(bg, 100);
            
            if (i==0) {
                m_mateNode1->removeAllChildren();
                m_mateNode1->addChild(bg);
                m_mateNode1->addChild(icon);
                m_mateNode1->addChild(m_mateName1);
                m_mateName1->setString(mateName);
                m_mateName1->setColor(getColor(color));
                place1=true;
            }else if (i==1) {
                m_mateNode2->removeAllChildren();
                m_mateNode2->addChild(bg);
                m_mateNode2->addChild(icon);
                m_mateNode2->addChild(m_mateName2);
                m_mateName2->setString(mateName);
                m_mateName2->setColor(getColor(color));
                place2=true;
            }else if (i==2) {
                m_mateNode3->removeAllChildren();
                m_mateNode3->addChild(bg);
                m_mateNode3->addChild(icon);
                m_mateNode3->addChild(m_mateName3);
                m_mateName3->setString(mateName);
                m_mateName3->setColor(getColor(color));
                place3=true;
            }else if (i==3) {
                m_mateNode4->removeAllChildren();
                m_mateNode4->addChild(bg);
                m_mateNode4->addChild(icon);
                m_mateNode4->addChild(m_mateName4);
                m_mateName4->setString(mateName);
                m_mateName4->setColor(getColor(color));
                place4=true;
                break;
            }
        }
        
        string nextIcon = CCCommonUtils::getIcon(CC_ITOA(m_itemId+1));
        string nextBG = CCCommonUtils::getToolBgByColor(color+1);
        string mateName5 = CCCommonUtils::getNameById(CC_ITOA(m_itemId+1));
        auto nicon = CCLoadSprite::createSprite(nextIcon.c_str(), true,CCLoadSpriteType_GOODS);
        CCCommonUtils::setSpriteMaxSize(nicon, 130, true);
        auto nbg = CCLoadSprite::createSprite(nextBG.c_str());
        CCCommonUtils::setSpriteMaxSize(nbg, 130, true);
        m_mateNode5->addChild(nicon);
        m_mateBgNode5->addChild(nbg);
        m_mateNode5->addChild(m_mateName5);
        m_mateName5->setString(mateName5);
        m_mateName5->setColor(getColor(color+1));
        
        if (m_iconNode1->getChildrenCount()==0) {
            auto icon1 = CCLoadSprite::createSprite(iconPath.c_str(), true,CCLoadSpriteType_GOODS);
            CCCommonUtils::setSpriteMaxSize(icon1, 50);
            auto bg1 = CCLoadSprite::createSprite(bgPath.c_str());
            CCCommonUtils::setSpriteMaxSize(bg1, 50);
            m_iconNode1->addChild(bg1);
            m_iconNode1->addChild(icon1);
        }
        
        if (m_iconNode2->getChildrenCount()==0) {
            auto icon1 = CCLoadSprite::createSprite(nextIcon.c_str(), true,CCLoadSpriteType_GOODS);
            CCCommonUtils::setSpriteMaxSize(icon1, 50);
            auto bg1 = CCLoadSprite::createSprite(nextBG.c_str());
            CCCommonUtils::setSpriteMaxSize(bg1, 50);
            m_iconNode2->addChild(bg1);
            m_iconNode2->addChild(icon1);
        }
        
        m_num1Label->setString(CC_ITOA(m_itemCount));
        
        m_num2Label->setString(CC_ITOA(m_itemNextCount));
        
    }
    else {
        place1=true;
        place2=true;
        place3=true;
        place4=true;
    }
    
    m_crtBtn->setEnabled(true);
    if (m_itemCount<4 && color < GOLDEN) {
        m_crtBtn->setEnabled(false);
        m_crtBtn->setVisible(false);
        m_needLabel->setVisible(true);
    } else if (color >= GOLDEN) {
        m_crtBtn->setEnabled(false);
        m_crtBtn->setVisible(false);
        m_needLabel->setVisible(false);
    }else if(m_itemCount>=4 && color < GOLDEN){
        m_crtBtn->setEnabled(true);
        m_crtBtn->setVisible(true);
        m_needLabel->setVisible(false);
    }
}

ccColor3B MateCreateView::getColor(int color){
    switch (color) {
        case 0:
            return TEXT_COLOR_WHITE;
        case 1:
            return TEXT_COLOR_GREEN;
        case 2:
            return TEXT_COLOR_BLUE;
        case 3:
            return TEXT_COLOR_PURPLE;
        case 4:
            return TEXT_COLOR_ORANGE;
        case 5:
            return TEXT_COLOR_YELLOW;
        default:
            break;
    }
    return {0,0,0};
}

void MateCreateView::refreashData(CCObject* obj)
{
    if(m_type==1){
        int count = m_itemCount - 4;
        int nextCount = m_itemNextCount + 1;
        setData(m_itemId,count,nextCount);
    }else{
        setData(m_itemId,0,0);
    }
    
    CCCallFunc* fun1 = CCCallFunc::create(this, callfunc_selector(MateCreateView::playEndCrtParticle));
    CCCallFunc* fun2 = CCCallFunc::create(this, callfunc_selector(MateCreateView::flyCrtDia));
    CCDelayTime* delay = CCDelayTime::create(1.25f);
    CCCallFunc* fun3 = CCCallFunc::create(this, callfunc_selector(MateCreateView::removeWaitInter));
    CCCallFunc* fun4 = CCCallFunc::create(this, callfunc_selector(MateCreateView::playCanCrtParticle));
    CCSequence* sq = CCSequence::create( fun1, fun2, delay ,fun3, fun4, NULL);
    this->runAction(sq);
    //合成完成飘字特效
    string mateName = CCCommonUtils::getNameById(CC_ITOA(m_itemId+1));
    string flyStr = _lang_1("111663", mateName.c_str());
    string color = CCCommonUtils::getPropById(CC_ITOA(m_itemId+1), "color");
    ccColor3B textColor = getColor(atoi(color.c_str()));
    CCCommonUtils::flyUiResText(flyStr, m_textFlyNode,ccp(0, 0) ,textColor, floating_type_ui1, 23);//ccGREEN
    
    if(m_type==1) {
        if (equipVec.size()>0 && equipVec.size()<=6) {
            for(int i=0;i<equipVec.size();i++){
                if (equipVec[i].itemId==(m_itemId+1)){
                    CCNode *fNode = CCNode::create();
                    CCCommonUtils::flyUiResText("+1", fNode,ccp(45, 0) ,ccGREEN, floating_type_ui1, 23);//ccGREEN
                    m_flyNum[i]->addChild(fNode);
                    break;
                }
            }
        }
    }else{
        if (toolVec.size()>0 && toolVec.size()<=6) {
            for(int i=0;i<toolVec.size();i++){
                if (toolVec[i].itemId==(m_itemId+1)){
                    CCNode *fNode = CCNode::create();
                    CCCommonUtils::flyUiResText("+1", fNode,ccp(45, 0) ,ccGREEN, floating_type_ui1, 23);//ccGREEN
                    m_flyNum[i]->addChild(fNode);
                    
                    if (toolVec[i].color>=EquipmentController::getInstance()->smithy_history_mate) {//材料合成记录
                        std::string str=GlobalData::shared()->playerInfo.name;
                        str+="|";
                        str+="1";
                        str+="|";
                        str+=CC_ITOA(toolVec[i].itemId);
                        EquipmentController::getInstance()->insertScrollInfo(str,true);
                    }
                    break;
                }
            }
        }
    }
    
}

void MateCreateView::flyCrtDia()
{
    //    CCCommonUtils::flyHint("", "", _lang("119032"));
}

void MateCreateView::removeWaitInter()
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
}

void MateCreateView::onEnter()
{
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MateCreateView::refreashData), MSG_MATE_CRT_END, NULL);
    setTouchEnabled(true);
}

void MateCreateView::onExit()
{
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    GlobalData::shared()->isBind = false;
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MATE_CRT_END);
    CCNode::onExit();
}

bool MateCreateView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return true;
}
void MateCreateView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(m_type==1) {
        if (equipVec.size()>0 && equipVec.size()<=6) {
            for(int i=0;i<equipVec.size();i++){
                if (isTouchInside(m_mateListNode[i],pTouch)){
                    CCLOG("m_mateListNode[%d]",i);
                    if(i==5){
                        CCCommonUtils::flyHint("", "", _lang("111662"));
                        //                        for (int i=0; i<5; i++) {
                        //                            m_selectSpr[i]->setVisible(false);
                        //                        }
                        //                        m_selectSpr[5]->setVisible(true);
                        
                    }else{
                        setData(equipVec[i].itemId,0,0);
                    }
                    setParticle();
                    return;
                }
            }
        }
    }else{
        if (toolVec.size()>0 && toolVec.size()<=6) {
            for(int i=0;i<toolVec.size();i++){
                if (isTouchInside(m_mateListNode[i],pTouch)){
                    CCLOG("m_mateListNode[%d]",i);
                    if(i==5){
                        CCCommonUtils::flyHint("", "", _lang("111662"));
                        //                        for (int i=0; i<5; i++) {
                        //                            m_selectSpr[i]->setVisible(false);
                        //                        }
                        //                        m_selectSpr[5]->setVisible(true);
                    }else{
                        setData(toolVec[i].itemId,0,0);
                    }
                    setParticle();
                    return;
                }
            }
        }
    }
    
    if (isTouchInside(m_mplace1,pTouch)){
        if (place1==false) {
            if(m_type==1) {
                PopupViewController::getInstance()->addPopupInView(StoreView::create(STORE_TYPE_GOTO_STORE));
            }else{
                gotoWorkShop(0);
            }
        }
        
    }else if (isTouchInside(m_mplace2,pTouch)){
        if (place2==false) {
            if(m_type==1) {
                PopupViewController::getInstance()->addPopupInView(StoreView::create(STORE_TYPE_GOTO_STORE));
            }else{
                gotoWorkShop(0);
            }
        }
    }else if (isTouchInside(m_mplace3,pTouch)){
        if (place3==false) {
            if(m_type==1) {
                PopupViewController::getInstance()->addPopupInView(StoreView::create(STORE_TYPE_GOTO_STORE));
            }else{
                gotoWorkShop(0);
            }
        }
    }else if (isTouchInside(m_mplace4,pTouch)){
        if (place4==false) {
            if(m_type==1) {
                PopupViewController::getInstance()->addPopupInView(StoreView::create(STORE_TYPE_GOTO_STORE));
            }else{
                gotoWorkShop(0);
            }
        }
    }
}

void MateCreateView::gotoWorkShop(float _time){
    AutoSafeRef temp(this);
    
    PopupViewController::getInstance()->removeAllPopupView();
    map<int, FunBuildInfo>::iterator it;
    map<int, FunBuildInfo> *curBuildsInfo = FunBuildController::getInstance()->curBuildsInfo;
    for(it=curBuildsInfo->begin(); it != curBuildsInfo->end(); ++it){
        if((it->second).type==FUN_BUILD_WORKSHOP){
            if(SceneController::getInstance()->currentSceneId == SCENE_ID_MAIN){
                auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
                
                if( layer )
                {
                    layer->clickWorkshop((it->second).itemId);
                    if (CCCommonUtils::isIosAndroidPad()) {
                        layer->setPxPy(-2385, -25);//材料工坊中间位置
                    }else{
                        layer->setPxPy(-2568, -400);//材料工坊中间位置
                    }
                }
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("setViewByMate",CCInteger::create(toolVec[0].itemId));
            }
        }
    }
}

void MateCreateView::onClickCrtBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    //    m_waitInterface= GameController::getInstance()->showWaitInterface(m_crtBtn);
    this->getAnimationManager()->runAnimationsForSequenceNamed("Create");
    Vector<FiniteTimeAction*> arrayOfActions;
    CCCallFunc* fun1 = CCCallFunc::create(this, callfunc_selector(MateCreateView::playStartCrtParticle));
    CCDelayTime* delay = CCDelayTime::create(0.5f);
    CCCallFunc* fun2 = CCCallFunc::create(this, callfunc_selector(MateCreateView::startMateCrt));
    arrayOfActions.pushBack(fun1);
    arrayOfActions.pushBack(delay);
    arrayOfActions.pushBack(fun2);
    CCSequence* sequenceAction = CCSequence::create(arrayOfActions);
    this->runAction(sequenceAction);
}

void MateCreateView::addPopParticleToBatch(cocos2d::CCParticleSystemQuad *particle)
{
    auto batchCount = m_parPopVec.size();
    while (batchCount--) {
        auto &batch = m_parPopVec[batchCount];
        if (batch) {
            auto batchBlend = batch->getBlendFunc();
            auto particleBlend = particle->getBlendFunc();
            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                batch->addChild(particle);
                return;
            }
        }
    }
    auto newBatch = ParticleController::createParticleBatch();
    m_popLayer->addChild(newBatch);
    newBatch->addChild(particle);
    m_parPopVec.push_back(newBatch);
}

void MateCreateView::playStartCrtParticle()
{
    removeParticle();
    m_popNode->addChild(m_popLayer);
    
    for (int j = 1; j <= 5; ++j)
    {
        if (j <= 4) {
            string tmpStart = "Crown_";
            int count = 4;
            for (int i=1; i<=count; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
                particle->setPosition(((CCNode*)(m_midNode->getChildByTag(j)))->getPosition());
                addPopParticleToBatch(particle);
            }
        } else {
            string tmpStart = "CollectionIn_";
            int count = 8;
            for (int i=1; i<=count; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
                particle->setPosition(((CCNode*)(m_midNode->getChildByTag(j)))->getPosition());
                addPopParticleToBatch(particle);
            }
        }
    }
}

void MateCreateView::removeParticle()
{
    auto batchCount = m_parPopVec.size();
    while (batchCount--) {
        m_popLayer->removeChild(m_parPopVec[batchCount]);
    }
    m_parPopVec.clear();
    m_popNode->removeAllChildren();
    m_endCrtNode->removeAllChildren();
}

void MateCreateView::startMateCrt()
{
    removeParticle();
    EquipmentController::getInstance()->startMaterialCreate(m_itemId,m_type);
}

void MateCreateView::playEndCrtParticle()
{
    removeParticle();
    m_popNode->addChild(m_popLayer);
    
    //    string tmpStart = "CardOut_";
    //    int count = 8;
    //    for (int i=1; i<=count; i++) {
    //        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
    //        particle->setPosition(((CCNode*)(m_midNode->getChildByTag(5)))->getPosition());
    //        addPopParticleToBatch(particle);
    //    }
    
    if (m_colorT == GOLDEN || m_colorT == ORANGE) {
        auto particle1 = ParticleController::createParticle("LevelUp_1");
        particle1->setPosition(((CCNode*)(m_midNode->getChildByTag(5)))->getPosition());
        addPopParticleToBatch(particle1);
        
        if (m_colorT == GOLDEN) {
            auto particle2 = ParticleController::createParticle("LevelUp_6");
            particle2->setPosition(((CCNode*)(m_midNode->getChildByTag(5)))->getPosition());
            addPopParticleToBatch(particle2);
        }
        
        auto particle3 = ParticleController::createParticle("LevelUp_7");
        particle3->setPosition(((CCNode*)(m_midNode->getChildByTag(5)))->getPosition());
        addPopParticleToBatch(particle3);
    }
    
    auto tmpNode = CCNode::create();
    m_popNode->addChild(tmpNode);
    tmpNode->setScale(1.5);
    string tmpStart = "Synthesis_";
    int count = 8;
    for (int i=1; i<=count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPosition(((CCNode*)(m_midNode->getChildByTag(5)))->getPosition());
        
        particle->setStartColor(tmpStartColorVec[i-1]);
        particle->setEndColor(tmpEndColorVec[i-1]);
        
        tmpNode->addChild(particle);
        //        addPopParticleToBatch(particle);
    }
    
    //    auto particle1 = ParticleController::createParticle("MallBag_1");
    //    auto particle2 = ParticleController::createParticle("MallBag_2");
    //    auto particle3 = ParticleController::createParticle("MallBag_3");
    //    m_endCrtNode->addChild(particle1);
    //    m_endCrtNode->addChild(particle2);
    //    m_endCrtNode->addChild(particle3);
    
    CCScaleTo* sc1 = CCScaleTo::create(0.25, 1.4, 1.4);
    CCDelayTime* delay = CCDelayTime::create(0.1);
    CCScaleTo* sc2 = CCScaleTo::create(0.25, 1, 1);
    CCSequence* sq = CCSequence::create(sc1, delay, sc2, NULL);
    m_mateNode5->runAction(sq);
    
    CCScaleTo* sc1Bg = CCScaleTo::create(0.25, 1.4, 1.4);
    CCDelayTime* delayBg = CCDelayTime::create(0.1);
    CCScaleTo* sc2Bg = CCScaleTo::create(0.25, 1, 1);
    CCSequence* sqBg = CCSequence::create(sc1Bg, delayBg, sc2Bg, NULL);
    m_mateBgNode5->runAction(sqBg);
    SoundController::sharedSound()->playEffects(Music_Sfx_UI_loteryrwd);
}

void MateCreateView::playCanCrtParticle()
{
    removeParticle();
    if(m_type==1){
        auto &eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
        if(m_itemCount < 4 || eInfo.color >= GOLDEN){
            return;
        }
    }else{
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
        if (info.getCNT() < 4 || info.color >= GOLDEN) {
            return;
        }
    }
    m_popNode->addChild(m_popLayer);
    string tmpStart = "UIGlow_LR_";
    int count = 3;
    for (int i=1; i <= count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPosition(ccp(0, 149));
        addPopParticleToBatch(particle);
    }
    for (int i=1; i <= count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPosition(ccp(0, -144));
        addPopParticleToBatch(particle);
    }
    tmpStart = "UIGlow_UD_";
    for (int i=1; i <= count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPosition(ccp(146, 5));
        addPopParticleToBatch(particle);
    }
    for (int i=1; i <= count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setPosition(ccp(-146, 5));
        addPopParticleToBatch(particle);
    }
}

SEL_CCControlHandler MateCreateView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCrtBtn", MateCreateView::onClickCrtBtn);
    return NULL;
}

bool MateCreateView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_midNode", CCNode*, m_midNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_textFlyNode", CCNode*, m_textFlyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, m_bottomNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode1", CCNode*, m_iconNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode2", CCNode*, m_iconNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num1Label", CCLabelIF*, m_num1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num2Label", CCLabelIF*, m_num2Label);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftBG", CCScale9Sprite*, m_leftBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightBG", CCScale9Sprite*, m_rightBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flyNum1", CCNode*, m_flyNum[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flyNum2", CCNode*, m_flyNum[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flyNum3", CCNode*, m_flyNum[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flyNum4", CCNode*, m_flyNum[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flyNum5", CCNode*, m_flyNum[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flyNum6", CCNode*, m_flyNum[5]);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateListNode1", CCNode*, m_mateListNode[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateListNode2", CCNode*, m_mateListNode[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateListNode3", CCNode*, m_mateListNode[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateListNode4", CCNode*, m_mateListNode[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateListNode5", CCNode*, m_mateListNode[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateListNode6", CCNode*, m_mateListNode[5]);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprNode1", CCNode*, m_sprNode[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprNode2", CCNode*, m_sprNode[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprNode3", CCNode*, m_sprNode[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprNode4", CCNode*, m_sprNode[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprNode5", CCNode*, m_sprNode[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprNode6", CCNode*, m_sprNode[5]);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num1", CCLabelIF*, m_num[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num2", CCLabelIF*, m_num[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num3", CCLabelIF*, m_num[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num4", CCLabelIF*, m_num[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num5", CCLabelIF*, m_num[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num6", CCLabelIF*, m_num[5]);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectSpr1", CCSprite*, m_selectSpr[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectSpr2", CCSprite*, m_selectSpr[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectSpr3", CCSprite*, m_selectSpr[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectSpr4", CCSprite*, m_selectSpr[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectSpr5", CCSprite*, m_selectSpr[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectSpr6", CCSprite*, m_selectSpr[5]);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode", CCNode*, m_mateNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateName1", CCLabelIF*, m_mateName1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateName2", CCLabelIF*, m_mateName2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateName3", CCLabelIF*, m_mateName3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateName4", CCLabelIF*, m_mateName4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateName5", CCLabelIF*, m_mateName5);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameMateLabel", CCLabelIF*, m_nameMateLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_crtBtn", CCControlButton*, m_crtBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode1", CCNode*, m_mateNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode2", CCNode*, m_mateNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode3", CCNode*, m_mateNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode4", CCNode*, m_mateNode4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode5", CCNode*, m_mateNode5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_popNode", CCNode*, m_popNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_endCrtNode", CCNode*, m_endCrtNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_needLabel", CCLabelIF*, m_needLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel", CCLabelIF*, m_infoLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateBgNode5", CCNode*, m_mateBgNode5);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mplace1", CCScale9Sprite*, m_mplace1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mplace2", CCScale9Sprite*, m_mplace2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mplace3", CCScale9Sprite*, m_mplace3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mplace4", CCScale9Sprite*, m_mplace4);
    return false;
}
