//
//  EquipComposeView.cpp
//  IF
//
//  Created by 付彬 on 15/12/22.
//
//

#include "EquipComposeView.hpp"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "RollingText.h"
#include "YesNoDialog.h"
#include "FunBuildController.h"
#include "QueueController.h"
#include "EquipmentController.h"
#include "SoundController.h"
#include "SceneContainer.h"
#include "StoreView.h"
#include "EquipmentCommand.h"
#include "EquipSiteView.h"
#include "EquipFinishView.h"
#include "StoreBuyConfirmDialog.h"
#include "EquipInfoView.hpp"

#define r1_color {216,177,146}
#define r2_color {224,149,78}
#define r3_color {66,156,224}

EquipComposeView* EquipComposeView::create(string equipUuid)
{
    auto ret = new EquipComposeView();
    if (ret && ret->init(equipUuid)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipComposeView::init(string equipUuid)
{
    bool ret = true;
    if (!PopupBaseView::init()) {
        return false;
    }
    m_equipUuid = equipUuid;
    m_equipId = EquipmentController::getInstance()->m_MyEquipmentMap[m_equipUuid];
    startPt = ccp(0,0);
    
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
    auto tmpCCB = CCBLoadFile("EquipComposeView",this,this);
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
        m_leftBG->setContentSize(CCSizeMake(m_leftBG->getContentSize().width, m_leftBG->getContentSize().height+extH));
        m_rightBG->setContentSize(CCSizeMake(m_rightBG->getContentSize().width, m_rightBG->getContentSize().height+extH));
        
    }
    m_popLayer = CCLayer::create();
    setPrcNodeTag();
    m_waitInterface = NULL;
    m_needLabel->setString(_lang("119020"));
    m_needLabel->setVisible(false);
    m_infoLabel->setString(_lang("111747"));
    
    generateData();
    setData();
    
    m_selNode->setVisible(false);
    m_idx = 0;
    m_tabView = CCMultiColTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setMultiColTableViewDelegate(this);
    m_tabView->setTouchPriority(Touch_Popup);
    m_infoList->addChild(m_tabView);
    
    return ret;
}

bool sortEquipBySite(int itemId1, int itemId2)
{
    auto& eInfo1 = EquipmentController::getInstance()->EquipmentInfoMap[itemId1];
    auto& eInfo2 = EquipmentController::getInstance()->EquipmentInfoMap[itemId2];
    if (eInfo1.universal > eInfo2.universal) {
        return true;
    }else if (eInfo2.universal > eInfo1.universal) {
        return false;
    }
    else {
        if (eInfo1.site<eInfo2.site) {
            return true;
        }else if (eInfo1.site > eInfo2.site){
            return false;
        }else {
            return false;
        }
    }
}
bool sortEquipBag(string str1, string str2)
{
    int itemId1 = EquipmentController::getInstance()->m_MyEquipmentMap[str1];
    int itemId2 = EquipmentController::getInstance()->m_MyEquipmentMap[str2];
    return sortEquipBySite(itemId1,itemId2);
}
bool sortEquipPutOn(string str1, string str2)
{
    int itemId1 = EquipmentController::getInstance()->m_MyOnEquip[str1];
    int itemId2 = EquipmentController::getInstance()->m_MyOnEquip[str2];
    return sortEquipBySite(itemId1,itemId2);
}

void EquipComposeView::generateData()
{
    m_selData.clear();
    m_data.clear();
    m_selData.push_back(m_equipUuid);
    m_selData.push_back("");
    m_selData.push_back("");
    m_selData.push_back("");
    
    auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_equipId];
    
    string universalId = "";
    map<int, EquipmentInfo>::iterator it = EquipmentController::getInstance()->EquipmentInfoMap.begin();
    for (; it != EquipmentController::getInstance()->EquipmentInfoMap.end(); it++) {
        if (it->second.level==eInfo.level && it->second.color==1 && it->second.universal==1) {
            universalId = CC_ITOA(it->second.itemId);
        }
    }
    
    m_toolId = 0;
    vector<int> toolVec = ToolController::getInstance()->m_typeTools[18];
    for (int i = 0; i<toolVec.size(); i++)
    {
        auto& toolInfo = ToolController::getInstance()->getToolInfoById(toolVec[i]);
        if (toolInfo.equipId == universalId) {
            m_toolId = toolInfo.itemId;
            break;
        }
    }
    
    int tmpIdx = 1;
    int hasPutBroean = 0;//已经放入的 万能装备，转换为绿色 对应等级的 万能装备
    map<string, int>::iterator eit = EquipmentController::getInstance()->m_MyEquipmentMap.begin();
    for (; eit!=EquipmentController::getInstance()->m_MyEquipmentMap.end(); eit++)
    {
        if (eit->first != m_equipUuid)
        {
            auto& tmpInfo = EquipmentController::getInstance()->EquipmentInfoMap[eit->second];
            if(tmpInfo.color == eInfo.color && tmpInfo.level == eInfo.level && tmpInfo.site!=6)
            {
                if (tmpIdx<4) {
                    if (tmpInfo.universal == 1) {
                        m_selData[tmpIdx] = eit->first;
                        hasPutBroean += pow(4, tmpInfo.color-1);
                        tmpIdx++;
                    }else {
                        m_data.push_back(eit->first);
                    }
                }
                else {
                    m_data.push_back(eit->first);
                }
            }
        }
    }
    
    sort(m_data.begin(), m_data.end(), sortEquipBag);
    while (tmpIdx < 4)
    {
        if (m_data.size()>0) {
            m_selData[tmpIdx] = m_data[0];
            m_data.erase(m_data.begin());
            tmpIdx++;
        }
        else {
            break;
        }
    }
    
    if(eInfo.universal == 1)
    {
        hasPutBroean += pow(4, eInfo.color-1);
    }
    m_hasPutOnBroean = hasPutBroean;
    
    if (m_data.size()<=0 && tmpIdx<4) {
        m_needBroean = pow(4, eInfo.color-1);// * (4-tmpIdx);
        m_haveBroean = EquipmentController::getInstance()->getCountBoreanById(m_equipId);//获取有多少万能装备
        string colorName = CCCommonUtils::getColorName(eInfo.color);
        int lastNum = MAX(0, m_needBroean-(m_haveBroean-hasPutBroean));
        m_needMsg = _lang_4("111750", CC_ITOA(lastNum), CC_ITOA(eInfo.level), CC_ITOA(eInfo.level), colorName.c_str());
        m_noMateLabel->setString(m_needMsg);
    }
    
    vector<string> tmpPutOnEquips;
    if (true) {//加入已经穿戴的装备
        map<string, int>::iterator onit = EquipmentController::getInstance()->m_MyOnEquip.begin();
        for (; onit!=EquipmentController::getInstance()->m_MyOnEquip.end(); onit++)
        {
            auto& tmpInfo = EquipmentController::getInstance()->EquipmentInfoMap[onit->second];
            if(tmpInfo.color == eInfo.color && tmpInfo.level == eInfo.level && tmpInfo.site!=6)
            {
                tmpPutOnEquips.push_back(onit->first);
            }
        }
    }
    //排序
    sort(tmpPutOnEquips.begin(), tmpPutOnEquips.end(), sortEquipPutOn);
    //排序end
    m_data.insert(m_data.end(), tmpPutOnEquips.begin(), tmpPutOnEquips.end());
    
    onUpdateComposeBtn();
}

void EquipComposeView::setPrcNodeTag()
{
    m_mateNode1->setTag(1);
    m_mateNode2->setTag(2);
    m_mateNode3->setTag(3);
    m_mateNode4->setTag(4);
    m_mateNode5->setTag(5);
}

void EquipComposeView::setParticle(){
    playCanCrtParticle();
    auto &eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_equipId];
    m_colorT = eInfo.color+1;
    
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

void EquipComposeView::setData()
{
    m_mateNode1->removeAllChildren();
    m_mateNode2->removeAllChildren();
    m_mateNode3->removeAllChildren();
    m_mateNode4->removeAllChildren();
    m_mateNode5->removeAllChildren();
    m_mateBgNode5->removeAllChildren();
    
    int color;
    int id;
    
    auto eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_equipId];
    color = eInfo.color;
    id = eInfo.itemId;
    m_color = color;
    
    if (color < GOLDEN) {
        string items = "";
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
        for (int i=0; i<m_selData.size(); i++)
        {
            if (m_selData[i] == "") {
                continue;
            }
            int tmpEquipId = EquipmentController::getInstance()->m_MyEquipmentMap[ m_selData[i] ];
            items = CC_ITOA(tmpEquipId);
            iconPath = CCCommonUtils::getIcon(items);
            mateName = CCCommonUtils::getNameById(items);
            
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
        
        string nextIcon = CCCommonUtils::getIcon(CC_ITOA(m_equipId+1));
        string nextBG = CCCommonUtils::getToolBgByColor(color+1);
        string mateName5 = CCCommonUtils::getNameById(CC_ITOA(m_equipId+1));
        auto nicon = CCLoadSprite::createSprite(nextIcon.c_str(), true,CCLoadSpriteType_GOODS);
        CCCommonUtils::setSpriteMaxSize(nicon, 130, true);
        auto nbg = CCLoadSprite::createSprite(nextBG.c_str());
        CCCommonUtils::setSpriteMaxSize(nbg, 130, true);
        m_mateNode5->addChild(nicon);
        m_mateBgNode5->addChild(nbg);
        m_mateNode5->addChild(m_mateName5);
        m_mateName5->setString(mateName5);
        m_mateName5->setColor(getColor(color+1));
        
    }
    else {
        place1=true;
        place2=true;
        place3=true;
        place4=true;
    }
}

ccColor3B EquipComposeView::getColor(int color){
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

void EquipComposeView::refreashData(CCObject* obj)
{
//    PopupViewController::getInstance()->goBackPopupView();
//    PopupViewController::getInstance()->addPopupView( EquipInfoView::create(m_equipUuid, true));
//    return;
    
    CCString* CCUuid = dynamic_cast<CCString*>(obj);
    if (CCUuid) {
        string uuid = CCUuid->getCString();
        if (uuid!="") {
            PopupViewController::getInstance()->goBackPopupView();
            PopupViewController::getInstance()->addPopupView( EquipInfoView::create(uuid, true) );
        }
    }
    
}

void EquipComposeView::flyCrtDia()
{
    //    CCCommonUtils::flyHint("", "", _lang("119032"));
}

void EquipComposeView::removeWaitInter()
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
}

void EquipComposeView::onEnter()
{
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipComposeView::refreashData), EQUIP_COMPOSE_END, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipComposeView::afterRefreshData), CHANGE_EQUIP_SEL, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipComposeView::onRetBuyTool), MSG_BUY_EQUIPMENT, NULL);
    setTouchEnabled(true);
    setTitleName(_lang("111743"));
}

void EquipComposeView::onExit()
{
    setTouchEnabled(false);
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    GlobalData::shared()->isBind = false;
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, EQUIP_COMPOSE_END);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, CHANGE_EQUIP_SEL);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BUY_EQUIPMENT);
    CCNode::onExit();
}

bool EquipComposeView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return true;
}
void EquipComposeView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_desNode->isVisible()) {
        if (!isTouchInside(m_desNode,pTouch)) {
            m_desNode->setVisible(false);
        }
        return;
    }
    
    if (m_selNode->isVisible()) {
        if (!isTouchInside(m_infoList,pTouch)) {
            m_selNode->setVisible(false);
        }
        return;
    }
    
    if (isTouchInside(m_mplace1,pTouch)){
        showTipNode(0);
    }else if (isTouchInside(m_mplace2,pTouch)){
        changeData(1);
    }else if (isTouchInside(m_mplace3,pTouch)){
        changeData(2);
    }else if (isTouchInside(m_mplace4,pTouch)){
        changeData(3);
    }else if (isTouchInside(m_mplace5,pTouch)){
        showTipNode(1);
    }
}

void EquipComposeView::onClickGotoBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    int buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_FORGE);
    if(buildId==0){
        //没有铁匠铺
        return;
    }
    int forge_qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_BUILDING,CC_ITOA(buildId));
    if (forge_qid != QID_MAX) {
        //铁匠铺在升级
        YesNoDialog::show(_lang("119975").c_str(),NULL);//
        return;
    }
    PopupViewController::getInstance()->removePopupView(this);
    PopupViewController::getInstance()->addPopupInView(EquipSiteView::create());
}

void EquipComposeView::onClickGetMore(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_toolId == 0) {
        PopupViewController::getInstance()->addPopupInView(StoreView::create(STORE_TYPE_GOTO_STORE));
        return;
    }
    
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_toolId);
    int price = 0;
    if(info.ishot&&info.isShowHot)
        price = info.hotprice;
    else
        price = info.price;
    int cCount = 0;
    
    StoreBuyConfirmDialog* dialog = nullptr;
    dialog = StoreBuyConfirmDialog::show(CCCommonUtils::getIcon(CC_ITOA(info.itemId)).c_str(), info.getName().c_str(), _lang(info.des).c_str(), price,info.color, CCCallFuncO::create(this, callfuncO_selector(EquipComposeView::onBuyTool), NULL),m_getMoreBtn->convertToWorldSpace(CCPointZero),Gold,cCount);
    
    if(dialog && price>0){
        int maxNum = floor(GlobalData::shared()->playerInfo.gold/price);
        maxNum = MAX(1, maxNum);
        
        dialog->showSliderBar(maxNum);
    }
}

void EquipComposeView::onBuyTool(CCObject *ccObj)
{
    int buyCount = 1;
    if(ccObj){
        CCInteger* cInt = dynamic_cast<CCInteger*>(ccObj);
        if(cInt){
            buyCount = cInt->getValue();
        }
    }
    int sum = EquipmentController::getInstance()->m_MyEquipmentMap.size()+EquipmentController::getInstance()->m_MyOnEquip.size()+buyCount;
    if (sum > EquipmentController::getInstance()->m_equipMaxSum) {
        CCCommonUtils::flyHint("", "", _lang("111757"));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUY_CONFIRM_OK);
        return;
    }
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_toolId);
    ToolController::getInstance()->buyEquip(info.itemId, buyCount);
}
void EquipComposeView::onRetBuyTool(CCObject *ccObj)
{
    auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_equipId];
    
    map<string, int>::iterator eit = EquipmentController::getInstance()->m_MyEquipmentMap.begin();
    for (; eit!=EquipmentController::getInstance()->m_MyEquipmentMap.end(); eit++)
    {
        if (eit->first != m_selData[0] && eit->first != m_selData[1] && eit->first != m_selData[2] && eit->first != m_selData[3])
        {
            auto& tmpInfo = EquipmentController::getInstance()->EquipmentInfoMap[eit->second];
            if(tmpInfo.level == eInfo.level && tmpInfo.universal == 1)
            {
                if (tmpInfo.color == eInfo.color) {
                    m_data.push_back(eit->first);
                }else if (tmpInfo.color != eInfo.color && tmpInfo.color==1) {
                    
                }
            }
        }
    }
    
    if (m_data.size() > 0)
    {//更新选择框 数据
        m_getMoreBtn->setVisible(false);
        m_gotoBtn->setVisible(false);
        m_noMateLabel->setString("");
        m_tabView->reloadData();
        m_selNode->setVisible(true);
    }
    else
    {//更新选择框提示文字
        m_haveBroean = EquipmentController::getInstance()->getCountBoreanById(m_equipId);//获取有多少万能装备
        string colorName = CCCommonUtils::getColorName(eInfo.color);
        int lastNum = MAX(0, m_needBroean-(m_haveBroean-m_hasPutOnBroean));
        m_needMsg = _lang_4("111750", CC_ITOA(lastNum), CC_ITOA(eInfo.level), CC_ITOA(eInfo.level), colorName.c_str());
        m_noMateLabel->setString(m_needMsg);
    }
}

void EquipComposeView::onClickCrtBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_selNode->isVisible()) {
        return;
    }
    auto eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_equipId+1];
    string colorName = CCCommonUtils::getColorName(eInfo.color);
    YesNoDialog::show(_lang_3("111752",  CC_ITOA(eInfo.level), colorName.c_str(), _lang(eInfo.name).c_str()), CCCallFunc::create(this, callfunc_selector(EquipComposeView::lastCompose)));
}

void EquipComposeView::lastCompose()
{
//    refreashData(NULL);
//    return;
    
    this->getAnimationManager()->runAnimationsForSequenceNamed("Create");
    Vector<FiniteTimeAction*> arrayOfActions;
    CCCallFunc* fun1 = CCCallFunc::create(this, callfunc_selector(EquipComposeView::playStartCrtParticle));
    CCDelayTime* delay = CCDelayTime::create(0.5f);
    CCCallFunc* fun2 = CCCallFunc::create(this, callfunc_selector(EquipComposeView::startMateCrt));
    arrayOfActions.pushBack(fun1);
    arrayOfActions.pushBack(delay);
    arrayOfActions.pushBack(fun2);
    CCSequence* sequenceAction = CCSequence::create(arrayOfActions);
    this->runAction(sequenceAction);
    
    m_equipLine0Spr->setVisible(false);
    m_equipLine1Spr->setVisible(false);
    m_equipLine2Spr->setVisible(false);
    m_equipLine3Spr->setVisible(false);
}

void EquipComposeView::addPopParticleToBatch(cocos2d::CCParticleSystemQuad *particle)
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

void EquipComposeView::playStartCrtParticle()
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

void EquipComposeView::removeParticle()
{
    auto batchCount = m_parPopVec.size();
    while (batchCount--) {
        m_popLayer->removeChild(m_parPopVec[batchCount]);
    }
    m_parPopVec.clear();
    m_popNode->removeAllChildren();
    m_endCrtNode->removeAllChildren();
}

void EquipComposeView::startMateCrt()
{
    removeParticle();
    string varItemids = m_selData[0]+","+m_selData[1]+","+m_selData[2]+","+m_selData[3];
    MaterialCreateCommand* cmd = new MaterialCreateCommand(varItemids,2);
    cmd->sendAndRelease();
}

void EquipComposeView::playEndCrtParticle()
{
    removeParticle();
    m_popNode->addChild(m_popLayer);
    
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
        
    }
    
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

void EquipComposeView::playCanCrtParticle()
{
    removeParticle();
    
//    auto &eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
//    if(m_itemCount < 4 || eInfo.color >= GOLDEN){
//        return;
//    }
    
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

SEL_CCControlHandler EquipComposeView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCrtBtn", EquipComposeView::onClickCrtBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickGotoBtn", EquipComposeView::onClickGotoBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickGetMore", EquipComposeView::onClickGetMore);
    return NULL;
}

bool EquipComposeView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_midNode", CCNode*, m_midNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_textFlyNode", CCNode*, m_textFlyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftBG", CCScale9Sprite*, m_leftBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightBG", CCScale9Sprite*, m_rightBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateName1", CCLabelIF*, m_mateName1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateName2", CCLabelIF*, m_mateName2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateName3", CCLabelIF*, m_mateName3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateName4", CCLabelIF*, m_mateName4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateName5", CCLabelIF*, m_mateName5);
    
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameMateLabel", CCLabelIF*, m_nameMateLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_crtBtn", CCControlButton*, m_crtBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_gotoBtn", CCControlButton*, m_gotoBtn);
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
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mplace5", CCNode*, m_mplace5);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selNode", CCNode*, m_selNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flyNode", CCNode*, m_flyNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrowSpr", CCSprite*, m_arrowSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getMoreBtn", CCControlButton*, m_getMoreBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noMateLabel", CCLabelIF*, m_noMateLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selTitleLabel", CCLabelIF*, m_selTitleLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode", CCNode*, m_desNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desbg", CCScale9Sprite*, m_desbg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLine1", CCSprite*, m_desLine1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLine2", CCSprite*, m_desLine2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_valueNode", CCNode*, m_valueNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desName", CCLabelIF*, m_desName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLv", CCLabelIF*, m_desLv);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIF*, m_msgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desPara1", CCLabelIF*, m_desPara1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNum1", CCLabelIF*, m_desNum1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desPara2", CCLabelIF*, m_desPara2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNum2", CCLabelIF*, m_desNum2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desPara3", CCLabelIF*, m_desPara3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNum3", CCLabelIF*, m_desNum3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desPara4", CCLabelIF*, m_desPara4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNum4", CCLabelIF*, m_desNum4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desPara5", CCLabelIF*, m_desPara5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNum5", CCLabelIF*, m_desNum5);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipLine0Spr", CCSprite*, m_equipLine0Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipLine1Spr", CCSprite*, m_equipLine1Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipLine2Spr", CCSprite*, m_equipLine2Spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipLine3Spr", CCSprite*, m_equipLine3Spr);
    
    return false;
}

void EquipComposeView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell)
{
    EquipComposeCell* equipCell = dynamic_cast<EquipComposeCell*>(cell);
    if (equipCell && m_selNode->isVisible()) {
        startPt = m_selNode->convertTouchToNodeSpace(m_tabView->m_pCurTouch);
        equipCell->cellTouchEnded(m_tabView->m_pCurTouch);
    }
}

cocos2d::CCSize EquipComposeView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(216, 204);
    }
    return CCSize(90, 85);
}

CCTableViewCell* EquipComposeView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, ssize_t idx)
{
    if(idx >= m_data.size()){
        return NULL;
    }
    
    string uuid = m_data[idx];
    EquipComposeCell* cell = (EquipComposeCell*)table->dequeueGrid();
    if(cell){
        cell->setData(uuid);
    }else{
        cell = EquipComposeCell::create(uuid);
    }
    return cell;
}

ssize_t EquipComposeView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    return ceil(m_data.size()*1.0/6);
}

ssize_t EquipComposeView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 6;
}

void EquipComposeView::changeData(int idx)
{
    m_idx = idx;
    m_getMoreBtn->setVisible(false);
    m_gotoBtn->setVisible(false);
//    auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    m_noMateLabel->setString("");
    m_selTitleLabel->setString("");
    
    CCCommonUtils::setButtonTitle(m_getMoreBtn, _lang("111676").c_str());//购买万能装备 _lang("119039").c_str()
    CCCommonUtils::setButtonTitle(m_gotoBtn, _lang("119012").c_str());
    
    auto eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_equipId];
    string colorName = CCCommonUtils::getColorName(eInfo.color);
    m_selTitleLabel->setString(_lang_2("111751", CC_ITOA(eInfo.level),colorName.c_str()));
    
    if (m_data.size()<=0) {
//        m_noMateLabel->setString(_lang("119024"));//亲爱的领主，你暂时没有合适的装备
        m_noMateLabel->setString(m_needMsg);
        m_getMoreBtn->setVisible(true);
        m_gotoBtn->setVisible(true);
        if (m_selData[1]!="" && m_selData[2]!="" && m_selData[3]!="") {
            m_noMateLabel->setString(_lang("111755"));
            m_getMoreBtn->setVisible(false);
            m_gotoBtn->setVisible(false);
        }
    }
    
    if (idx == 1) {
        if (CCCommonUtils::isIosAndroidPad()) {
            m_selNode->setPositionY(-463);
            m_arrowSpr->setPositionX(180.0);
        }
        else{
            m_selNode->setPositionY(m_mateNode2->getPositionY()-240);
            m_arrowSpr->setPositionX(m_mateNode2->getPositionX()-m_selNode->getPositionX());
        }
    }else if (idx == 2) {
        if (CCCommonUtils::isIosAndroidPad()) {
            m_selNode->setPositionY(-463);
            m_arrowSpr->setPositionX(1190.0);
        }
        else{
            m_selNode->setPositionY(m_mateNode3->getPositionY()-240);
            m_arrowSpr->setPositionX(m_mateNode3->getPositionX()-m_selNode->getPositionX());
        }
    }else if (idx == 3) {
        if (CCCommonUtils::isIosAndroidPad()) {
            m_selNode->setPositionY(-980);
            m_arrowSpr->setPositionX(680.0);
        }
        else{
            m_selNode->setPositionY(m_mateNode4->getPositionY()-240);
            m_arrowSpr->setPositionX(m_mateNode4->getPositionX()-m_selNode->getPositionX());
        }
    }
    
    m_tabView->reloadData();
    
    m_selNode->setVisible(true);
    
}

void EquipComposeView::showTipNode(int idx)
{
    int equipItemId = m_equipId;
    if (idx == 1) {
        equipItemId += 1;
    }
    
    auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[equipItemId];
    if (eInfo.universal == 1) {
        return;
    }
    
    string itemName = _lang(eInfo.name);
    m_desName->setString(itemName);
    string limitLv = "Lv:";
    limitLv+=CC_ITOA(eInfo.level);
    m_desLv->setString(limitLv);
    
    m_msgLabel->setString("");
    m_msgLabel->setPositionY(33);
    m_desLine2->setPositionY(38);
    
    m_desPara1->setString("");
    m_desPara2->setString("");
    m_desPara3->setString("");
    m_desPara4->setString("");
    m_desPara5->setString("");
    m_desNum1->setString("");
    m_desNum2->setString("");
    m_desNum3->setString("");
    m_desNum4->setString("");
    m_desNum5->setString("");
    m_desPara1->setColor(r1_color);
    m_desPara2->setColor(r1_color);
    m_desPara3->setColor(r1_color);
    m_desPara4->setColor(r1_color);
    m_desPara5->setColor(r1_color);
    m_desNum1->setColor(r2_color);
    m_desNum2->setColor(r2_color);
    m_desNum3->setColor(r2_color);
    m_desNum4->setColor(r2_color);
    m_desNum5->setColor(r2_color);
    
    vector<int> TmpNum;
    if (eInfo.site==6) {
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
    
    float nodeW = m_desNode->getContentSize().width;
    float cNodeH = 0;
    int j = 1;
    for (int i=0; i<TmpNum.size(); i++)
    {
        int tmpNum = TmpNum[i];
        string tmpInfo = "";
        if (eInfo.showDias.find(tmpNum) != eInfo.showDias.end()) {
            string dialog = eInfo.showDias[tmpNum];
            if (dialog!="" && eInfo.values.find(tmpNum)!=eInfo.values.end()) {
                string pf = eInfo.getEffFormatByOrd(tmpNum);
                string pm = eInfo.getEffPMByOrd(tmpNum);
                if (pf=="") {
                    tmpInfo = tmpInfo +pm+CC_ITOA(eInfo.values[tmpNum])+pf;
                }else {
                    tmpInfo = tmpInfo +pm+CC_ITOA_MONEY(eInfo.values[tmpNum])+pf;
                }
            }
            if (j==1) {
                m_desPara1->setString(_lang(dialog));
                m_desNum1->setString(tmpInfo);
            }else if (j==2) {
                m_desPara2->setString(_lang(dialog));
                m_desNum2->setString(tmpInfo);
            }else if (j==3) {
                m_desPara3->setString(_lang(dialog));
                m_desNum3->setString(tmpInfo);
            }else if (j==4) {
                m_desPara4->setString(_lang(dialog));
                m_desNum4->setString(tmpInfo);
            }else if (j==5) {
                m_desPara5->setString(_lang(dialog));
                m_desNum5->setString(tmpInfo);
            }
            cNodeH = 30*j;
            j++;
        }
    }

    m_valueNode->setPositionY(m_desLine2->getPositionY() + cNodeH );
    m_desLine1->setPositionY(m_valueNode->getPositionY() + 10);
    m_desName->setPositionY(m_desLine1->getPositionY() + 20 );
    m_desNode->setContentSize(CCSize(nodeW, m_desName->getPositionY()+20));
    m_desbg->setContentSize(m_desNode->getContentSize());
    m_desNode->setVisible(true);
}

void EquipComposeView::afterRefreshData(CCObject* p)
{
    if (m_curOptItem!="") {
        return;
    }
    m_curOptItem = "";
    CCString* strItemId = dynamic_cast<CCString*>(p);
    if (strItemId) {
        m_curOptItem = strItemId->getCString();
    }
    if (m_curOptItem=="") {
        return;
    }
    else {
        startFlyData(0);
//        scheduleOnce(schedule_selector(EquipComposeView::startFlyData), 0.1f);
    }
}

void EquipComposeView::startFlyData(float dt)
{
    string itemId = m_curOptItem;
    if (itemId=="") {
        return;
    }
    
    string bgPath = "";
    string iconPath = "";
    
    int eItemId = 0;
    if (EquipmentController::getInstance()->m_MyEquipmentMap.find(itemId)!=EquipmentController::getInstance()->m_MyEquipmentMap.end()) {
        eItemId = EquipmentController::getInstance()->m_MyEquipmentMap[itemId];
    }else {
        return;
    }
    auto& resEquip = EquipmentController::getInstance()->EquipmentInfoMap[eItemId];
    bgPath = CCCommonUtils::getToolBgByColor(resEquip.color);
    iconPath = CCCommonUtils::getIcon(CC_ITOA(eItemId));
    
    this->scheduleOnce(schedule_selector(EquipComposeView::playPutOnSound), 0.15f);
    _tmpBGPath = bgPath;
    _tmpIconPath = iconPath;
    _tmpName = _lang(resEquip.name);
    
    auto flyBG = CCLoadSprite::createSprite(bgPath.c_str());
    CCCommonUtils::setSpriteMaxSize(flyBG, 80);
    auto flyIcon = CCLoadSprite::createSprite(iconPath.c_str(),true,CCLoadSpriteType_EQUIP);
    CCCommonUtils::setSpriteMaxSize(flyIcon, 80);
    m_flyNode->setPosition(startPt);
    m_flyNode->addChild(flyBG);
    m_flyNode->addChild(flyIcon);
    float scpro = 1.0;
    if (CCCommonUtils::isIosAndroidPad()) {
        scpro = 2.4;
    }
    m_flyNode->setScale(1.0);
    CCActionInterval * moveTo = CCMoveTo::create(0.3,m_arrowSpr->getPosition()+ccp(0, 30));
    CCActionInterval * scaleTo = CCScaleTo::create(0.3, scpro*1.25);
    CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(EquipComposeView::flyEnd));
    CCSpawn* spwan = CCSpawn::create(CCSequence::create(moveTo,funcall,NULL), scaleTo, NULL);
    m_flyNode->runAction(spwan);
    
    //刷新数据
    string eUuid1 = m_selData[m_idx];
    m_selData[m_idx] = m_curOptItem;
    vector<string>::iterator it = m_data.begin();
    for (int i=0; i<m_data.size(); i++) {
        if (m_data[i] == m_curOptItem) {
            m_data.erase(it);
            break;
        }
        it++;
    }
    if (eUuid1!="") {
        m_data.push_back(eUuid1);
    }
    
    float miny = m_tabView->minContainerOffset().y;
    CCPoint pos  = m_tabView->getContentOffset();
    m_tabView->reloadData();
    float mincurry = m_tabView->minContainerOffset().y;
    pos.y+=(-miny+mincurry);
    m_tabView->setContentOffset(pos);
    
    m_getMoreBtn->setVisible(false);
    m_gotoBtn->setVisible(false);
    m_noMateLabel->setString("");
    if (m_data.size()<=0) {
        m_noMateLabel->setString(m_needMsg);
        m_getMoreBtn->setVisible(true);
        m_gotoBtn->setVisible(true);
        if (m_selData[1]!="" && m_selData[2]!="" && m_selData[3]!="") {
            m_noMateLabel->setString(_lang("111755"));
            m_getMoreBtn->setVisible(false);
            m_gotoBtn->setVisible(false);
        }
    }
    
    onUpdateComposeBtn();
}

void EquipComposeView::flyEnd()
{
    m_flyNode->removeAllChildren();
    auto bgIcon = CCLoadSprite::createSprite(_tmpBGPath.c_str());
    CCCommonUtils::setSpriteMaxSize(bgIcon, 100);
    auto icon = CCLoadSprite::createSprite(_tmpIconPath.c_str(),true,CCLoadSpriteType_EQUIP);
    CCCommonUtils::setSpriteMaxSize(icon, 100);
    icon->setTag(1);
    
    if (m_idx==1) {
        m_mateNode2->removeAllChildren();
        m_mateNode2->addChild(bgIcon);
        m_mateNode2->addChild(icon);
        m_mateNode2->addChild(m_mateName2);
        m_mateName2->setString(_tmpName);
        m_mateName2->setColor(getColor(m_color));
    }else if (m_idx==2) {
        m_mateNode3->removeAllChildren();
        m_mateNode3->addChild(bgIcon);
        m_mateNode3->addChild(icon);
        m_mateNode3->addChild(m_mateName3);
        m_mateName3->setString(_tmpName);
        m_mateName3->setColor(getColor(m_color));
    }else if (m_idx==3) {
        m_mateNode4->removeAllChildren();
        m_mateNode4->addChild(bgIcon);
        m_mateNode4->addChild(icon);
        m_mateNode4->addChild(m_mateName4);
        m_mateName4->setString(_tmpName);
        m_mateName4->setColor(getColor(m_color));
    }
    m_curOptItem = "";
}

void EquipComposeView::onUpdateComposeBtn()
{
    if (m_selData[1]!="" && m_selData[2]!="" && m_selData[3]!="")
    {
        m_crtBtn->setEnabled(true);
        playCanCrtParticle();
    }
    else
    {
        m_crtBtn->setEnabled(false);
        removeParticle();
    }
    
    if (m_selData[1]!="") {
        m_equipLine1Spr->setVisible(true);
    }else {
        m_equipLine1Spr->setVisible(false);
    }
    
    if (m_selData[2]!="") {
        m_equipLine2Spr->setVisible(true);
    }else {
        m_equipLine2Spr->setVisible(false);
    }
    
    if (m_selData[3]!="") {
        m_equipLine3Spr->setVisible(true);
    }else {
        m_equipLine3Spr->setVisible(false);
    }
}

void EquipComposeView::playPutOnSound(float dt)
{
    SoundController::sharedSound()->playEffects(Music_Sfx_UI_putonequipment);
}


// cell class :  EquipComposeCell
//
EquipComposeCell* EquipComposeCell::create(string equipUuid)
{
    auto ret = new EquipComposeCell();
    if (ret && ret->init(equipUuid)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipComposeCell::init(string equipUuid)
{
    bool ret = true;
    CCLoadSprite::doResourceByCommonIndex(306, true);
    CCLoadSprite::doResourceByCommonIndex(305, true);
    CCBLoadFile("EquipmentSelCell",this,this);
    setContentSize(CCSize(90, 85));
    if (CCCommonUtils::isIosAndroidPad()) {
        setContentSize(CCSize(216, 204));
    }
    setData(equipUuid);
    return ret;
}

void EquipComposeCell::setData(string equipUuid)
{
    m_equipUUid = equipUuid;
    m_picNode->removeAllChildren();
    m_numLabel->setString("");
    m_onSpr->setVisible(false);
    
    string bgStr = "";
    string iconStr = "";
    
    m_equipId = 0;
    if (EquipmentController::getInstance()->m_MyEquipmentMap.find(m_equipUUid)!=EquipmentController::getInstance()->m_MyEquipmentMap.end()) {
        m_equipId = EquipmentController::getInstance()->m_MyEquipmentMap[m_equipUUid];
    }
    else if (EquipmentController::getInstance()->m_MyOnEquip.find(m_equipUUid) != EquipmentController::getInstance()->m_MyOnEquip.end()) {
        m_equipId = EquipmentController::getInstance()->m_MyOnEquip[m_equipUUid];
        m_onSpr->setVisible(true);
    }
    
    auto& info = EquipmentController::getInstance()->EquipmentInfoMap[m_equipId];
    bgStr = CCCommonUtils::getToolBgByColor(info.color);
    iconStr = CCCommonUtils::getIcon(CC_ITOA(m_equipId));
    string lvMsg = "Lv.";
    lvMsg = lvMsg + CC_ITOA(info.level);
    m_numLabel->setString(lvMsg);
    
    auto bg = CCLoadSprite::createSprite(bgStr.c_str());
    CCCommonUtils::setSpriteMaxSize(bg, 75, true);
    m_picNode->addChild(bg);
    
    auto icon = CCLoadSprite::createSprite(iconStr.c_str(),true,CCLoadSpriteType_EQUIP);
    CCCommonUtils::setSpriteMaxSize(icon, 75, true);
    m_picNode->addChild(icon);
}

void EquipComposeCell::onEnter() {
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipComposeCell::retTakeOff), EQUIP_TAKEOFF_MSG, NULL);
}
void EquipComposeCell::onExit() {
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, EQUIP_TAKEOFF_MSG);
    CCNode::onExit();
}
SEL_CCControlHandler EquipComposeCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}
void EquipComposeCell::cellTouchEnded(CCTouch* pTouch)
{
    if (EquipmentController::getInstance()->m_MyOnEquip.find(m_equipUUid) != EquipmentController::getInstance()->m_MyOnEquip.end())
    {
        YesNoDialog::showVariableTitle(_lang("111748").c_str(),CCCallFunc::create(this, callfunc_selector(EquipComposeCell::onTakeOff)),_lang("119003").c_str());
    }
    else {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(CHANGE_EQUIP_SEL, CCString::create(m_equipUUid));
    }
}
bool EquipComposeCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_onSpr", CCSprite*, m_onSpr);
    return false;
}

void EquipComposeCell::onTakeOff()
{
    EquipmentController::getInstance()->startTakeOffEquipment(m_equipUUid);
}
void EquipComposeCell::retTakeOff(CCObject* obj)
{
    CCString* equipItemId = dynamic_cast<CCString*>(obj);
    if (equipItemId) {
        string uuid = equipItemId->getCString();
        if (uuid == m_equipUUid) {
            m_onSpr->setVisible(false);
        }
    }
}
