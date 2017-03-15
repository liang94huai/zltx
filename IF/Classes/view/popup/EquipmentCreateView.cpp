//
//  EquipmentCreateView.cpp
//  IF
//
//  Created by fubin on 14-12-18.
//
//

#include "EquipmentCreateView.h"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "ToolController.h"
#include "EquipmentController.h"
#include "EquipmentInfo.h"
#include "QueueController.h"
#include "CCThreadManager.h"
#include "YesNoDialog.h"
#include "FunBuildController.h"
#include "FunBuildInfo.h"
#include "EquipFinishView.h"
#include "UseToolView.h"
#include "SoundController.h"
#include "VipUtil.h"
#include "WorldMapView.h"
#include "SceneController.h"
#include "ImperialScene.h"
#include "EquipNewUseView.h"
#include "EquipmentBagView.h"
#include "ConfirmDialogView.h"
#include "StoreView.h"
#include "SacrificePopUpView.h"
#include "MateCreateView.hpp"

EquipmentCreateView* EquipmentCreateView::create(int itemId){
    EquipmentCreateView* ret = new EquipmentCreateView();
    if(ret && ret->init(itemId)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipmentCreateView::init(int itemId)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    int buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_FORGE);
    if (buildId==0) {
        return false;
    }
    auto& buildInfo = FunBuildController::getInstance()->getFunbuildById(buildId);
    int bdPara1 = atoi(buildInfo.para[1].c_str());
    int bdPara2 = atoi(buildInfo.para[2].c_str());
    
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(100, true);
    CCLoadSprite::doResourceByCommonIndex(305, true);
    CCLoadSprite::doResourceByCommonIndex(306, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(100, false);
        CCLoadSprite::doResourceByCommonIndex(305, false);
        CCLoadSprite::doResourceByCommonIndex(306, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_EQUIP);
    });
    
    m_itemId = itemId;
    m_subGuideKey = -1;
    _isUUid = true;
    m_canPlayAdd = true;
    startPt = ccp(0, 0);
    
    auto bg = CCBLoadFile("EquipmentCreateView",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else {
        setContentSize(bg->getContentSize());
        //    changeBGHeight(m_buildBG);
        int extH = getExtendHeight();
        m_middleNode->setPositionY(m_middleNode->getPositionY()-extH/2);
        m_bottomNode->setPositionY(m_bottomNode->getPositionY()-extH/2);
//        m_fireBG->setPositionY(m_fireBG->getPositionY()-extH/2);
//        m_fireBG->setVisible(false);
        m_btmMidNode->setPositionY(m_btmMidNode->getPositionY()+extH/4);
        m_titleNode->setPositionY(m_titleNode->getPositionY()-extH/2);
//        auto tbg = CCLoadSprite::loadResource("Forging_wall_zhu3.png");
//        auto bgBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
//        m_bgNode->addChild(bgBatchNode);
//        for (int i=0; ; i++) {
//            auto cellSpr1 = CCLoadSprite::createSprite("Forging_wall_zhu3.png");
//            cellSpr1->setFlipX(true);
//            cellSpr1->setAnchorPoint(ccp(0.5, 1));
//            cellSpr1->setPosition(ccp(0, -62*i));
//            bgBatchNode->addChild(cellSpr1);
//            
//            auto cellSpr2 = CCLoadSprite::createSprite("Forging_wall_zhu3.png");
//            cellSpr2->setAnchorPoint(ccp(0.5, 1));
//            cellSpr2->setPosition(ccp(573, -62*i));
//            bgBatchNode->addChild(cellSpr2);
//            
//            if (i*62 > extH) {
//                break;
//            }
//        }
//        auto mtbg = CCLoadSprite::loadResource("wall_01.png");
//        auto mbgBatchNode = CCSpriteBatchNode::createWithTexture(mtbg->getTexture());
//        m_midBgNode->addChild(mbgBatchNode);
//        for (int i=0; ; i++) {
//            auto cellSpr1 = CCLoadSprite::createSprite("wall_01.png");
//            cellSpr1->setFlipY(true);
//            cellSpr1->setAnchorPoint(ccp(0, 0.5));
//            cellSpr1->setPosition(ccp(0, -90*i-90));
//            mbgBatchNode->addChild(cellSpr1);
//            
//            auto cellSpr2 = CCLoadSprite::createSprite("wall_01.png");
//            cellSpr2->setFlipY(true);
//            cellSpr2->setAnchorPoint(ccp(1, 0.5));
//            cellSpr2->setPosition(ccp(640, -90*i-90));
//            mbgBatchNode->addChild(cellSpr2);
//            
//            if (i*90 > extH/2) {
//                break;
//            }
//        }
    }
    m_equipCrtBtn->setSwallowsTouches(false);
    m_popLayer = CCLayer::create();
    m_smokeLayer = CCLayer::create();
    m_particleNode->addChild(m_smokeLayer);
//    m_forgeLayer = CCLayer::create();
//    m_pNodeCenter->addChild(m_forgeLayer);
    
    
    m_flyArrow = CCLoadSprite::createSprite("UI_hand.png");
    m_flyArrow->setAnchorPoint(ccp(0, 0));
    m_flyArrow->setRotation(90);
    m_flyArrow->setVisible(false);
    m_handNode->addChild(m_flyArrow);
    
    m_selNode->setVisible(false);
    m_desNode->setVisible(false);
    m_idx = 0;
    m_tabView = CCMultiColTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setMultiColTableViewDelegate(this);
    m_tabView->setTouchPriority(Touch_Popup);
    m_infoList->addChild(m_tabView);
    
    m_getMoreBtn->setVisible(false);
    CCCommonUtils::setButtonTitle(m_getMoreBtn, _lang("119039").c_str());
    CCCommonUtils::setButtonTitle(m_equipCrtBtn, _lang("119006").c_str());
    setTitleName(_lang("119012"));
    
    auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    string strName = _lang(eInfo.name) + "   "+_lang("102203")+" " + CC_ITOA(eInfo.level);
    m_equipLabel->setString(strName);
    m_nameTxt->setString(strName);
    string iconPath = CCCommonUtils::getIcon(CC_ITOA(m_itemId));
    auto icon = CCLoadSprite::createSprite(iconPath.c_str(),true,CCLoadSpriteType_EQUIP);
    CCCommonUtils::setSpriteMaxSize(icon, 130, true);
    m_newEIconNode->addChild(icon);
    
    for (int i=WHITE; i<=GOLDEN; i++) {
        auto colorCell = EquipColorCell::create(i);
//        if (CCCommonUtils::isIosAndroidPad()) {
//            colorCell->setPosition(i*240, 0);
//        }
//        else
            colorCell->setPosition(i*100, 0);
        m_colorBarNode->addChild(colorCell);
        m_colorCellMap[i] = colorCell;
    }
    
    int tmp_costRes = 0;
    m_costRes = 0;
    m_isEnough = true;
    m_costNode->setVisible(false);
    m_barNode->setVisible(false);
    m_percentNode->setVisible(false);
    //隐藏按钮
    m_equipCrtBtn->setVisible(false);
    //显示材料不足提示
    m_errorLabel->setString(_lang("119020").c_str());
    m_errorLabel->setVisible(true);
    //隐藏锻造条
    m_forgeInner->setVisible(false);
    vector<string> resCostVec;
    CCCommonUtils::splitString(eInfo.cost, ";", resCostVec);
    if (resCostVec.size()==2) {
        tmp_costRes = atoi(resCostVec[1].c_str());
        int tmpCostRes = tmp_costRes * bdPara2*1.0/100;
        //前台读取数据的加成处理
        double effect = CCCommonUtils::getEffectValueByNum(203) * 1.0 / 100;
        tmpCostRes = tmpCostRes*1.0*(1-effect);
        tmp_costRes -= tmpCostRes;
        
        int resType = atoi(resCostVec[0].c_str());
        m_costResType = resType;
        string resIconPath = CCCommonUtils::getResourceIconByType(resType);
        auto resIcon = CCLoadSprite::createSprite(resIconPath.c_str());
        CCCommonUtils::setSpriteMaxSize(resIcon, 48);
        m_resIconNode->addChild(resIcon);
        int curNum = CCCommonUtils::getCurResourceByType(resType);
        m_resNumLabel->setColor({193,162,110});
        m_costRes = tmp_costRes;
        if (curNum<tmp_costRes) {
            m_isEnough = false;
            m_resNumLabel->setColor(ccRED);
        }
        m_resNumLabel->setString(CC_CMDITOA(curNum));
        m_costLabel->setString("/" + CC_CMDITOA(tmp_costRes));
        m_costLabel->setPositionX(m_resNumLabel->getPositionX() + m_resNumLabel->getContentSize().width*m_resNumLabel->getOriginScaleX());
        m_addResNode->setPositionX(m_costLabel->getPositionX() + m_costLabel->getContentSize().width*m_costLabel->getOriginScaleX()+m_addResBtn->getContentSize().width);
    }
    
    int costTime = eInfo.time*100.0/(100+bdPara1);
    //前台读取数据的加成处理
    double time_effect = CCCommonUtils::getEffectValueByNum(203) * 1.0 / 100;
    costTime = costTime*1.0/(1+time_effect);
    m_timeLabel->setString(CC_SECTOA(costTime));
    
    m_infoNode->setVisible(false);
    m_qid = QID_MAX;
    int tqid = QueueController::getInstance()->getMinTimeQidByType(TYPE_FORGE);
    if (tqid!=QID_MAX) {
        auto& qInfo = GlobalData::shared()->allQueuesInfo[tqid];
        int TmpItemId = QueueController::getInstance()->getItemId(qInfo.key);
        if ((TmpItemId - TmpItemId%10) == (m_itemId-m_itemId%10)) {
            m_curCreatingItemId = TmpItemId;
            m_qid = tqid;
            m_btnMsgLabel->setString(_lang("104903"));
            CCCommonUtils::setButtonTitle(m_equipCrtBtn, "");
            m_infoNode->setVisible(true);
            
            showParticle();
        }
    }
    
    string curCost = EquipmentController::getInstance()->m_curCreateCost;
    if(m_qid == QID_MAX || curCost=="") {
        onGenerateMateData();//统计所需材料
        onShowMates();
    }else {//显示当前增在锻造的装备的 材料
        //显示按钮
        m_equipCrtBtn->setVisible(true);
        //隐藏材料不足提示
        m_errorLabel->setVisible(false);
        //显示百分比
        m_percentNode->setVisible(true);
        
        vector<string> curCostVec;
        CCCommonUtils::splitString(curCost, "|", curCostVec);
        int i=0;
        if (eInfo.costLv>0) {
            m_noEquip->setVisible(false);
            m_equipAddSpr->setVisible(true);
            
            auto& ttEinfo = EquipmentController::getInstance()->EquipmentInfoMap[atoi(curCostVec[i].c_str())];
            string bgTT = CCCommonUtils::getToolBgByColor(ttEinfo.color);
            auto iconBgTT = CCLoadSprite::createSprite(bgTT.c_str());
            CCCommonUtils::setSpriteMaxSize(iconBgTT, 70, true);
            m_resEIconNode->addChild(iconBgTT);
            
            string strEquipTmp = CCCommonUtils::getIcon(curCostVec[i]);
            auto iconEquipTmp = CCLoadSprite::createSprite(strEquipTmp.c_str(), true,CCLoadSpriteType_EQUIP);
            CCCommonUtils::setSpriteMaxSize(iconEquipTmp, 70, true);
            iconEquipTmp->setTag(1);
            m_resEIconNode->addChild(iconEquipTmp);
            
            CCFadeTo* fadeTo1 = CCFadeTo::create(0.5, 120);
            CCFadeTo* fadeTo2 = CCFadeTo::create(0.5, 255);
            CCActionInterval * repeatForever =CCRepeatForever::create(CCSequence::create(fadeTo1,fadeTo2,NULL));
            iconEquipTmp->runAction(repeatForever);
            
            _isUUid = false;
            m_tmpEquips = curCostVec[i];
            i++;
        }
        
        int j = 0;
        for (; i<curCostVec.size(); i++) {
            
            string toolId = curCostVec[i];
            string strMate = CCCommonUtils::getIcon(toolId);
            auto iconMate = CCLoadSprite::createSprite(strMate.c_str(), true,CCLoadSpriteType_GOODS);
            iconMate->setTag(1);
            CCCommonUtils::setSpriteMaxSize(iconMate, 60, true);
            
            auto& tool = ToolController::getInstance()->getToolInfoById(atoi(toolId.c_str()));
            string bgPath = CCCommonUtils::getToolBgByColor(tool.color);
            auto bgIcon = CCLoadSprite::createSprite(bgPath.c_str());
            CCCommonUtils::setSpriteMaxSize(bgIcon, 60, true);
            m_selMates[j+1] = atoi(toolId.c_str());
            
            CCFadeTo* fadeTo1 = CCFadeTo::create(0.5, 120);
            CCFadeTo* fadeTo2 = CCFadeTo::create(0.5, 255);
            CCActionInterval * repeatForever =CCRepeatForever::create(CCSequence::create(fadeTo1,fadeTo2,NULL));
            iconMate->runAction(repeatForever);
            
            if (j==0) {
                m_mateSpr1->setVisible(false);
                m_mateNode1->addChild(bgIcon);
                m_mateNode1->addChild(iconMate);
            }else if (j==1) {
                m_mateSpr2->setVisible(false);
                m_mateNode2->addChild(bgIcon);
                m_mateNode2->addChild(iconMate);
            }else if (j==2) {
                m_mateSpr3->setVisible(false);
                m_mateNode3->addChild(bgIcon);
                m_mateNode3->addChild(iconMate);
            }else if (j==3) {
                m_mateSpr4->setVisible(false);
                m_mateNode4->addChild(bgIcon);
                m_mateNode4->addChild(iconMate);
            }
            j++;
        }
        
        onMakeCrtPro(true,true);
    }
    
    onEnterFrame(0);
    showErrorInfo();
    
    m_guide1Node->setVisible(false);
    m_guide2Node->setVisible(false);
    _openNum = 0;
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipmentCreateView::afterRefreshData), CHANGE_MATE_SEL, NULL);
    
    string curGuideId = GuideController::share()->getCurGuideID();
    if (curGuideId == "3110600" || curGuideId == "3110900") {
        CCUserDefault::sharedUserDefault()->setStringForKey("Equp_Guide", "1");
        CCUserDefault::sharedUserDefault()->flush();
    }
    string guideValue = CCUserDefault::sharedUserDefault()->getStringForKey("Equp_Guide", "");
    if (guideValue == "1") {
        m_subGuideKey = 1;
        onShowHand(1);
    }
    
    return true;
}
EquipmentCreateView::~EquipmentCreateView()
{
    CCLOGFUNC();
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, CHANGE_MATE_SEL);
}

void EquipmentCreateView::onEnter(){
    CCNode::onEnter();
    setTitleName(_lang("119012"));
    UIComponent::getInstance()->showEquipOrBagBtn(1);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipmentCreateView::afterRefreshData), CHANGE_MATE_SEL, NULL);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(EquipmentCreateView::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipmentCreateView::updateCrtRet), EQUIP_CREATE_END, NULL);
    
    _openNum++;
    if (_openNum>1 && m_qid == QID_MAX) {
        m_selNode->setVisible(false);
        onGenerateMateData();//统计所需材料
//        onShowMates(1);
        onEnterShowMates();
        onMakeCrtPro();
        showErrorInfo();
    }
    //同步钢材
    int curNum = CCCommonUtils::getCurResourceByType(m_costResType);
    m_resNumLabel->setColor({193,162,110});
    if (curNum<m_costRes) {
        m_isEnough = false;
        m_resNumLabel->setColor(ccRED);
    }
    m_resNumLabel->setString(CC_CMDITOA(curNum));
    m_costLabel->setPositionX(m_resNumLabel->getPositionX() + m_resNumLabel->getContentSize().width*m_resNumLabel->getOriginScaleX());
    m_addResNode->setPositionX(m_costLabel->getPositionX() + m_costLabel->getContentSize().width*m_costLabel->getOriginScaleX()+m_addResBtn->getContentSize().width);
    
    if (m_qid!=QID_MAX) {
        this->getAnimationManager()->runAnimationsForSequenceNamed("Start");
    }
    else {
        m_initEnd = false;
        this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(EquipmentCreateView::AnimationClose));
        this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
    }
}

void EquipmentCreateView::onExit(){
    GlobalData::shared()->isBind = false;
    UIComponent::getInstance()->showEquipOrBagBtn(0);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(EquipmentCreateView::onEnterFrame), this);
//    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, CHANGE_MATE_SEL);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, EQUIP_CREATE_END);
    setTouchEnabled(false);
    CCNode::onExit();
}

CCNode* EquipmentCreateView::getGuideNode(string _key)
{
    if (!m_initEnd) {
        return NULL;
    }
    if (_key == "FR3_1") {
        m_guide1Node->setVisible(true);
        return m_guide1Node;
    }else if (_key == "FR3_2") {
        m_guide2Node->setVisible(true);
        return m_guide2Node;
    }else if (_key == "FR3_3") {
        return m_equipCrtBtn;
    }
    return NULL;
}

void EquipmentCreateView::onGenerateMateData()
{
    //筛选可以锻造的材料
    m_tmpMates.clear();
    for(int i=0; i<ToolController::getInstance()->m_mateTools.size(); i++)
    {
        int toolId = ToolController::getInstance()->m_mateTools[i];
        auto& tool = ToolController::getInstance()->getToolInfoById(toolId);
        if (tool.getCNT()>0) {
            m_tmpMates[toolId] = tool.getCNT();
        }
    }
    
    //筛选可以锻造的 原始装备
    auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    if (eInfo.costLv>0)
    {
        this->m_myEquips = EquipmentController::getInstance()->FindEquipmentsByLv(eInfo.costLv, eInfo.level);
        this->m_myOnEquips = EquipmentController::getInstance()->FindOnEquipmentsByLv(eInfo.costLv, eInfo.level);
    }
}

void EquipmentCreateView::onEnterShowMates(){
    auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    for (int i=0; i<eInfo.mateVec.size(); i++)
    {
        int curMateId = m_selMates[i+1];
        if (curMateId>0 && m_tmpMates.find(curMateId) != m_tmpMates.end() && m_tmpMates[curMateId]>0) {
            m_tmpMates[ curMateId ] -= 1;
        }
    }
}

void EquipmentCreateView::onShowMates(int type)
{
    _isUUid = true;
    auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    m_selMates[1] = 0;
    m_selMates[2] = 0;
    m_selMates[3] = 0;
    m_selMates[4] = 0;
    if (type == 0) {
        m_resEIconNode->removeAllChildren();
        m_tmpEquips = "";
    }
    m_mateNode1->removeAllChildren();
    m_mateNode2->removeAllChildren();
    m_mateNode3->removeAllChildren();
    m_mateNode4->removeAllChildren();
    m_mateLabel1->setString("");
    m_mateLabel2->setString("");
    m_mateLabel3->setString("");
    m_mateLabel4->setString("");
    
    if (eInfo.costLv>0) {//显示需要的子装备
        m_noEquip->setVisible(false);
        m_equipAddSpr->setVisible(true);
        if (m_myEquips.size()>0) {
//            int sunId = EquipmentController::getInstance()->m_MyEquipmentMap[m_myEquips[0]];
//            auto& ttEinfo = EquipmentController::getInstance()->EquipmentInfoMap[sunId];
//            string bgTT = CCCommonUtils::getToolBgByColor(ttEinfo.color);
//            auto iconBgTT = CCLoadSprite::createSprite(bgTT.c_str());
//            CCCommonUtils::setSpriteMaxSize(iconBgTT, 80, true);
//            m_resEIconNode->addChild(iconBgTT);
//            
//            string strEquipTmp = CCCommonUtils::getIcon(CC_ITOA(sunId));
//            auto iconEquipTmp = CCLoadSprite::createSprite(strEquipTmp.c_str(), true,CCLoadSpriteType_EQUIP);
//            CCCommonUtils::setSpriteMaxSize(iconEquipTmp, 80, true);
//            iconEquipTmp->setTag(1);
//            m_resEIconNode->addChild(iconEquipTmp);
//            
//            m_tmpEquips = m_myEquips[0];
        }
        else {
//            int sunId = eInfo.itemId;
//            string strEquipTmp = CCCommonUtils::getIcon(CC_ITOA(sunId));
//            auto iconEquipTmp = CCLoadSprite::createSprite(strEquipTmp.c_str(), true,CCLoadSpriteType_EQUIP);
//            CCCommonUtils::setSpriteMaxSize(iconEquipTmp, 60, true);
//            CCCommonUtils::setSpriteGray(iconEquipTmp, true);
//            m_resEIconNode->addChild(iconEquipTmp);
        }
    }
    
    bool isNew=false;
    int mLv = EquipmentController::getInstance()->equipMake_newTips_mLv;
    if(FunBuildController::getInstance()->getMainCityLv()<=mLv){//新手
        isNew = true;
    }
    for (int i=0; i<eInfo.mateVec.size(); i++)
    {
        vector<string> tmpVec;
        CCCommonUtils::splitString(eInfo.mateVec[i], ";", tmpVec);
        if (tmpVec.size()>0 && tmpVec[0]!="") {
            
            string MaxMate = "";
            if(!isNew){//不是新手
                int tmpMateId = atoi(tmpVec[0].c_str());
                for (int color=GOLDEN; color>=WHITE; color--) {
                    int curMateId = tmpMateId+color;
                    if (m_tmpMates.find(curMateId) != m_tmpMates.end() && m_tmpMates[curMateId]>0) {
                        m_selMates[i+1] = curMateId;
                        m_tmpMates[ curMateId ] -= 1;
                        MaxMate = CC_ITOA(curMateId);
                        break;
                    }
                }
            }
            
            
            bool isHave = true;
            if (MaxMate=="") {
                isHave = false;
                MaxMate = tmpVec[0];
            }
            
            int mLv = EquipmentController::getInstance()->equipMake_newTips_mLv;
            
            string mateMsg = "";
            string strMate = CCCommonUtils::getIcon(MaxMate);
            auto iconMate = CCLoadSprite::createSprite(strMate.c_str(), true,CCLoadSpriteType_GOODS);
            CCCommonUtils::setSpriteMaxSize(iconMate, 60, true);
            iconMate->setTag(1);
            if (!isHave) {
                CCCommonUtils::setSpriteGray(iconMate, true);
                iconMate->setOpacity(180);
                mateMsg = "0/1";
            }else {
                auto& tool = ToolController::getInstance()->getToolInfoById(atoi(MaxMate.c_str()));
                string bgPath = CCCommonUtils::getToolBgByColor(tool.color);
                auto bgIcon = CCLoadSprite::createSprite(bgPath.c_str());
                CCCommonUtils::setSpriteMaxSize(bgIcon, 60, true);
                if (i==0) {
                    m_mateNode1->addChild(bgIcon);
                }else if (i==1) {
                    m_mateNode2->addChild(bgIcon);
                }else if (i==2) {
                    m_mateNode3->addChild(bgIcon);
                }else if (i==3) {
                    m_mateNode4->addChild(bgIcon);
                }
                mateMsg = "1/1";
            }
            
            if (i==0) {
                m_mateSpr1->setVisible(false);
                m_mateNode1->addChild(iconMate);
                m_mateLabel1->setString(mateMsg);
                if(mateMsg == "0/1")
                {
                    m_mateLabel1->setColor(ccRED);
                    if(isNew){
                        CCNode* tmpNode = m_mateNode1->getChildByTag(1);
                        if (tmpNode) {
                            CCSprite* icon = dynamic_cast<CCSprite*>(tmpNode);
                            if (icon) {
                                CCSprite* addSpr = CCLoadSprite::createSprite("icon_hospital.png");
                                m_mateNode1->addChild(addSpr);
                                float changeTime = 0.5;
                                CCFadeTo* fadeTo1 = CCFadeTo::create(changeTime, 0);
                                CCDelayTime* delay1 = CCDelayTime::create(1.0f);
                                CCFadeTo* fadeTo2 = CCFadeTo::create(changeTime, 255);
                                CCDelayTime* delay2 = CCDelayTime::create(changeTime);
                                CCActionInterval * repeatForever1 =CCRepeatForever::create(CCSequence::create(delay1,fadeTo1,delay2,delay1,delay2,fadeTo2,NULL));
                                icon->runAction(repeatForever1);
                                addSpr->setOpacity(0);
                                CCActionInterval * repeatForever2 =CCRepeatForever::create(CCSequence::create(delay1,delay2,fadeTo2,delay1,fadeTo1,delay2,NULL));
                                addSpr->runAction(repeatForever2);
                            }
                        }
                    }
                }
                else
                {
                    m_mateLabel1->setColor(ccc3(175,164,150));
                }
            }else if (i==1) {
                m_mateSpr2->setVisible(false);
                m_mateNode2->addChild(iconMate);
                m_mateLabel2->setString(mateMsg);
                if(mateMsg == "0/1")
                {
                    m_mateLabel2->setColor(ccRED);
                    if(isNew){
                        CCNode* tmpNode = m_mateNode2->getChildByTag(1);
                        if (tmpNode) {
                            CCSprite* icon = dynamic_cast<CCSprite*>(tmpNode);
                            if (icon) {
                                CCSprite* addSpr = CCLoadSprite::createSprite("icon_hospital.png");
                                m_mateNode2->addChild(addSpr);
                                float changeTime = 0.5;
                                CCFadeTo* fadeTo1 = CCFadeTo::create(changeTime, 0);
                                CCDelayTime* delay1 = CCDelayTime::create(1.0f);
                                CCFadeTo* fadeTo2 = CCFadeTo::create(changeTime, 255);
                                CCDelayTime* delay2 = CCDelayTime::create(changeTime);
                                CCActionInterval * repeatForever1 =CCRepeatForever::create(CCSequence::create(delay1,fadeTo1,delay2,delay1,delay2,fadeTo2,NULL));
                                icon->runAction(repeatForever1);
                                addSpr->setOpacity(0);
                                CCActionInterval * repeatForever2 =CCRepeatForever::create(CCSequence::create(delay1,delay2,fadeTo2,delay1,fadeTo1,delay2,NULL));
                                addSpr->runAction(repeatForever2);
                            }
                        }
                    }
                }
                else
                {
                    m_mateLabel2->setColor(ccc3(175,164,150));
                }
            }else if (i==2) {
                m_mateSpr3->setVisible(false);
                m_mateNode3->addChild(iconMate);
                m_mateLabel3->setString(mateMsg);
                if(mateMsg == "0/1")
                {
                    m_mateLabel3->setColor(ccRED);
                    if(isNew){
                        CCNode* tmpNode = m_mateNode3->getChildByTag(1);
                        if (tmpNode) {
                            CCSprite* icon = dynamic_cast<CCSprite*>(tmpNode);
                            if (icon) {
                                CCSprite* addSpr = CCLoadSprite::createSprite("icon_hospital.png");
                                m_mateNode3->addChild(addSpr);
                                float changeTime = 0.5;
                                CCFadeTo* fadeTo1 = CCFadeTo::create(changeTime, 0);
                                CCDelayTime* delay1 = CCDelayTime::create(1.0f);
                                CCFadeTo* fadeTo2 = CCFadeTo::create(changeTime, 255);
                                CCDelayTime* delay2 = CCDelayTime::create(changeTime);
                                CCActionInterval * repeatForever1 =CCRepeatForever::create(CCSequence::create(delay1,fadeTo1,delay2,delay1,delay2,fadeTo2,NULL));
                                icon->runAction(repeatForever1);
                                addSpr->setOpacity(0);
                                CCActionInterval * repeatForever2 =CCRepeatForever::create(CCSequence::create(delay1,delay2,fadeTo2,delay1,fadeTo1,delay2,NULL));
                                addSpr->runAction(repeatForever2);
                            }
                        }
                    }
                }
                else
                {
                    m_mateLabel3->setColor(ccc3(175,164,150));
                }
            }else if (i==3) {
                m_mateSpr4->setVisible(false);
                m_mateNode4->addChild(iconMate);
                m_mateLabel4->setString(mateMsg);
                if(mateMsg == "0/1")
                {
                    m_mateLabel4->setColor(ccRED);
                    if(isNew){
                        CCNode* tmpNode = m_mateNode4->getChildByTag(1);
                        if (tmpNode) {
                            CCSprite* icon = dynamic_cast<CCSprite*>(tmpNode);
                            if (icon) {
                                CCSprite* addSpr = CCLoadSprite::createSprite("icon_hospital.png");
                                m_mateNode4->addChild(addSpr);
                                float changeTime = 0.5;
                                CCFadeTo* fadeTo1 = CCFadeTo::create(changeTime, 0);
                                CCDelayTime* delay1 = CCDelayTime::create(1.0f);
                                CCFadeTo* fadeTo2 = CCFadeTo::create(changeTime, 255);
                                CCDelayTime* delay2 = CCDelayTime::create(changeTime);
                                CCActionInterval * repeatForever1 =CCRepeatForever::create(CCSequence::create(delay1,fadeTo1,delay2,delay1,delay2,fadeTo2,NULL));
                                icon->runAction(repeatForever1);
                                addSpr->setOpacity(0);
                                CCActionInterval * repeatForever2 =CCRepeatForever::create(CCSequence::create(delay1,delay2,fadeTo2,delay1,fadeTo1,delay2,NULL));
                                addSpr->runAction(repeatForever2);
                            }
                        }
                    }
                }
                else
                {
                    m_mateLabel4->setColor(ccc3(175,164,150));
                }
            }
        }
    }
    
    onMakeCrtPro();

    showErrorInfo();
}

void EquipmentCreateView::showErrorInfo()
{
    if (m_qid!=QID_MAX) {
        return;
    }
    
//    m_errorLabel->setString("");
//    m_equipCrtBtn->setVisible(true);
//    m_equipCrtBtn->setEnabled(true);
    
    auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    //检查是否占用队列
//    int tqid = QueueController::getInstance()->getMinTimeQidByType(TYPE_FORGE);
//    if (tqid!=QID_MAX && m_qid==QID_MAX) {
//        m_errorLabel->setString(_lang("102152"));
//        m_equipCrtBtn->setVisible(false);
//        return;
//    }
    
    //检查材料是否充足
    vector<int> curMates;
    for (int i=1; i<=4; i++) {
        if (m_selMates[i] > 0) {
            curMates.push_back(m_selMates[i]);
        }
    }
    
    if ((m_myEquips.size()>0 || eInfo.costLv==0) && eInfo.mateVec.size() == curMates.size() && (m_tmpEquips!=""||eInfo.level==1)) {//材料全了
//        CCCommonUtils::setButtonTitle(m_equipCrtBtn, _lang("119006").c_str());//锻造
    }else {
//        m_errorLabel->setString(_lang("119020"));
        m_equipCrtBtn->setEnabled(false);
        CCCommonUtils::setButtonTitle(m_equipCrtBtn, _lang("119020").c_str());//材料不足
        m_costNode->setVisible(false);
        m_percentNode->setVisible(false);
        //隐藏按钮
        m_equipCrtBtn->setVisible(false);
        //显示材料不足提示
        m_errorLabel->setString(_lang("119020").c_str());
        m_errorLabel->setVisible(true);
        //隐藏动画
        m_forgeInner2->removeAllChildrenWithCleanup(true);
        return;
    }

//    //钢材不足
//    if (!m_isEnough) {
////        m_errorLabel->setString(_lang("119021"));
////        m_equipCrtBtn->setVisible(false);
//        return;
//    }
}

void EquipmentCreateView::changeData(int idx)
{
    if (m_qid != QID_MAX) {
        return;
    }
    
    m_getMoreBtn->setVisible(false);
    auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    m_noMateLabel->setString("");
    m_selTitleLabel->setString("");
    m_selBottomLabel->setString("");
    m_data.clear();
    if (idx==0) {//装备数据
        m_Type = 0;
        CCCommonUtils::setButtonTitle(m_getMoreBtn, _lang("111676").c_str());//购买万能装备 _lang("119039").c_str()
        m_selTitleLabel->setString(_lang_2("119072", CC_ITOA(eInfo.costLv),CC_ITOA(eInfo.level)));
        for (int i=0; i<m_myEquips.size(); i++) {
            if (m_myEquips[i] != m_tmpEquips) {
                int eItemId = EquipmentController::getInstance()->m_MyEquipmentMap[m_myEquips[i]];
                auto& curInfo = EquipmentController::getInstance()->EquipmentInfoMap[eItemId];
                if (curInfo.site!=6) {
                    m_data.push_back(m_myEquips[i]);
                }
            }
        }
        
        for (int i=0; i<m_myOnEquips.size(); i++) {
            if (m_myOnEquips[i] != m_tmpEquips) {
                int eItemId = EquipmentController::getInstance()->m_MyOnEquip[m_myOnEquips[i]];
                auto& curInfo = EquipmentController::getInstance()->EquipmentInfoMap[eItemId];
                if (curInfo.site!=6) {
                    m_data.push_back(m_myOnEquips[i]);
                }
            }
        }
        
        
        if (m_data.size()<=0) {
            if (m_tmpEquips == "") {
                m_noMateLabel->setString(_lang("119024"));//亲爱的领主，你暂时没有合适的装备
                m_getMoreBtn->setVisible(true);
            }else {
                m_noMateLabel->setString(_lang_1("119067", _lang("119027").c_str()));//唯一的装备已放入，没有剩余了
            }
        }
    }
    else {//材料数据
        m_Type = 1;
        CCCommonUtils::setButtonTitle(m_getMoreBtn, _lang("119039").c_str());//获取材料
        string mateId = "";
        if (eInfo.mateVec.size()>=idx) {
            string mate = eInfo.mateVec[idx-1];
            vector<string> matecell;
            CCCommonUtils::splitString(mate, ";", matecell);
            if (matecell.size()>=1) {
                mateId = matecell[0];
            }
        }
        if (mateId=="") {
            return;
        }
        
        string mateName = EquipmentController::getInstance()->GetMateNameById(atoi(mateId.c_str()));
        m_selTitleLabel->setString(_lang_1("119038", mateName.c_str()));
        
        int tmpMateCnt = 0;
        int curMateId = atoi(mateId.c_str());
        for (int i=0; i<10; i++) {
            int tmpMateId = curMateId+i;
            if (m_tmpMates.find(tmpMateId) != m_tmpMates.end() && m_tmpMates[tmpMateId]>0 ) {
//                m_data.push_back(CC_ITOA(tmpMateId));
                tmpMateCnt += m_tmpMates[tmpMateId];
            }
            
            if ( ToolController::getInstance()->m_toolInfos.find(tmpMateId) != ToolController::getInstance()->m_toolInfos.end() )
            {
                m_data.push_back(CC_ITOA(tmpMateId));
            }
        }
        
        if (tmpMateCnt<=0 && m_selMates[idx]==0) {
//            m_data.clear();
//            m_noMateLabel->setString(_lang("119025"));//亲爱的领主，你暂时没有合适的材料
//            m_getMoreBtn->setVisible(true);
        }else if (tmpMateCnt<=0 && m_selMates[idx] !=0) {
            if (m_tmpMates[m_selMates[idx]] == 0) {
                string mateName = EquipmentController::getInstance()->GetMateNameById(atoi(mateId.c_str()));
//                m_noMateLabel->setString(_lang_1("119067", mateName.c_str()));
                m_selBottomLabel->setString(_lang_1("119067", mateName.c_str()));//唯一的{0}已放入，没有剩余了。
            }
        }
    }
    generateData();
    
    if (idx == 0) {
        if (CCCommonUtils::isIosAndroidPad()) {
            m_selNode->setPositionY(80.0);
            m_arrowSpr->setPositionX(695.0);
        }
        else{
            m_selNode->setPositionY(m_resEIconNode->getPositionY()-250);
            m_arrowSpr->setPositionX(m_resEIconNode->getPositionX()-m_selNode->getPositionX());
        }
    }else if (idx == 1) {
        if (CCCommonUtils::isIosAndroidPad()) {
            m_selNode->setPositionY(250.0);
            m_arrowSpr->setPositionX(380.0);
        }
        else{
            m_selNode->setPositionY(m_mateNode1->getPositionY()-250);
            m_arrowSpr->setPositionX(m_mateNode1->getPositionX()-m_selNode->getPositionX());
        }
    }else if (idx == 2) {
        if (CCCommonUtils::isIosAndroidPad()) {
            m_selNode->setPositionY(250.0);
            m_arrowSpr->setPositionX(1010.0);
        }
        else{
            m_selNode->setPositionY(m_mateNode2->getPositionY()-250);
            m_arrowSpr->setPositionX(m_mateNode2->getPositionX()-m_selNode->getPositionX());
        }
    }else if (idx == 3) {
        if (CCCommonUtils::isIosAndroidPad()) {
            m_selNode->setPositionY(380.0);
            m_arrowSpr->setPositionX(94.0);
        }
        else{
            m_selNode->setPositionY(m_mateNode3->getPositionY()-250);
            m_arrowSpr->setPositionX(m_mateNode3->getPositionX()-m_selNode->getPositionX());
        }
    }else if (idx == 4) {
        if (CCCommonUtils::isIosAndroidPad()) {
            m_selNode->setPositionY(380.0);
            m_arrowSpr->setPositionX(1300.0);
        }
        else{
            m_selNode->setPositionY(m_mateNode4->getPositionY()-250);
            m_arrowSpr->setPositionX(m_mateNode4->getPositionX()-m_selNode->getPositionX());
        }
    }
    m_idx = idx;
    m_selNode->setVisible(true);
    if ( m_subGuideKey >0 && m_subGuideKey < 4) {
        onShowHand(2);
    }
}

void EquipmentCreateView::afterRefreshData(CCObject* p)
{
    m_curOptItem = "";
    CCString* strItemId = dynamic_cast<CCString*>(p);
    if (strItemId) {
        m_curOptItem = strItemId->getCString();
    }
    if (m_curOptItem=="") {
        return;
    }
    else {
        scheduleOnce(schedule_selector(EquipmentCreateView::refreshData), 0.1f);
    }
    
    switch (m_idx) {
        case 1:
        {
            m_mateLabel1->setString("1/1");
            m_mateLabel1->setColor(ccc3(175,164,150));
        }
            break;
        case 2:
        {
            m_mateLabel2->setString("1/1");
            m_mateLabel2->setColor(ccc3(175,164,150));

        }
            break;
        case 3:
        {
            m_mateLabel3->setString("1/1");
            m_mateLabel3->setColor(ccc3(175,164,150));

        }
            break;
        case 4:
        {
            m_mateLabel4->setString("1/1");
            m_mateLabel4->setColor(ccc3(175,164,150));

        }
            break;
            
        default:
            break;
    }
}

void EquipmentCreateView::refreshData(float dt)
{
    string itemId = m_curOptItem;
    if (itemId=="") {
        return;
    }
    
    string bgPath = "";
    string iconPath = "";
    CCLoadSpriteType loadType = CCLoadSpriteType_GOODS;
    if (m_idx==0) {
        int eItemId = 0;
        if (EquipmentController::getInstance()->m_MyEquipmentMap.find(itemId)!=EquipmentController::getInstance()->m_MyEquipmentMap.end()) {
            eItemId = EquipmentController::getInstance()->m_MyEquipmentMap[itemId];
        }
        if(eItemId==0){
            eItemId = EquipmentController::getInstance()->m_MyOnEquip[itemId];
        }
        auto& resEquip = EquipmentController::getInstance()->EquipmentInfoMap[eItemId];
        bgPath = CCCommonUtils::getToolBgByColor(resEquip.color);
        iconPath = CCCommonUtils::getIcon(CC_ITOA(eItemId));
        
        m_tmpEquips = itemId;
        loadType = CCLoadSpriteType_EQUIP;
        this->scheduleOnce(schedule_selector(EquipmentCreateView::playPutOnSound), 0.15f);
        refreshEquipData();
    } else {
        auto& resTool = ToolController::getInstance()->getToolInfoById(atoi(itemId.c_str()));
        bgPath = CCCommonUtils::getToolBgByColor(resTool.color);
        iconPath = CCCommonUtils::getIcon(itemId);
        //数据交换
        if (m_selMates[m_idx] > 0) {
            m_tmpMates[ m_selMates[m_idx] ] += 1;
        }
        m_selMates[m_idx] = atoi(itemId.c_str());
        m_tmpMates[ m_selMates[m_idx] ] -= 1;
        this->scheduleOnce(schedule_selector(EquipmentCreateView::playPutOnSound), 0.15f);
        refreshMateData();
    }
    _tmpBGPath = bgPath;
    _tmpIconPath = iconPath;
    
    if (CCCommonUtils::isIosAndroidPad()) {
        auto flyBG = CCLoadSprite::createSprite(bgPath.c_str());
        CCCommonUtils::setSpriteMaxSize(flyBG, 80);
        auto flyIcon = CCLoadSprite::createSprite(iconPath.c_str(),true,loadType);
        CCCommonUtils::setSpriteMaxSize(flyIcon, 80);
        m_flyNode->setPosition(startPt);
        m_flyNode->addChild(flyBG);
        m_flyNode->addChild(flyIcon);
        m_flyNode->setScale(2.4);
        CCActionInterval * moveTo = CCMoveTo::create(0.3,m_arrowSpr->getPosition()+ccp(0, 30));
        CCActionInterval * scaleTo = CCScaleTo::create(0.3, 1.8);
        CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(EquipmentCreateView::flyEnd));
        CCSpawn* spwan = CCSpawn::create(CCSequence::create(moveTo,funcall,NULL), scaleTo, NULL);
        m_flyNode->runAction(spwan);
    }
    else {
        auto flyBG = CCLoadSprite::createSprite(bgPath.c_str());
        CCCommonUtils::setSpriteMaxSize(flyBG, 80);
        auto flyIcon = CCLoadSprite::createSprite(iconPath.c_str(),true,loadType);
        CCCommonUtils::setSpriteMaxSize(flyIcon, 80);
        m_flyNode->setPosition(startPt);
        m_flyNode->addChild(flyBG);
        m_flyNode->addChild(flyIcon);
        m_flyNode->setScale(1.0);
        CCActionInterval * moveTo = CCMoveTo::create(0.3,m_arrowSpr->getPosition()+ccp(0, 30));
        CCActionInterval * scaleTo = CCScaleTo::create(0.3, 0.75);
        CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(EquipmentCreateView::flyEnd));
        CCSpawn* spwan = CCSpawn::create(CCSequence::create(moveTo,funcall,NULL), scaleTo, NULL);
        m_flyNode->runAction(spwan);
        
        if (m_subGuideKey > 0 && m_subGuideKey < 4) {
            if (m_arrowSpr->getPosition().x < 320) {
                onShowHand(3);
            }else {
                onShowHand(1);
            }
            m_subGuideKey++;
        }else {
            m_flyArrow->stopAllActions();
            m_flyArrow->setVisible(false);
        }
    }
}

void EquipmentCreateView::flyEnd()
{
    m_flyNode->removeAllChildren();
    if (CCCommonUtils::isIosAndroidPad()) {
        m_flyNode->setScale(2.4);
    }
    else
        m_flyNode->setScale(1.0);
    CCLoadSpriteType loadType = (m_idx==0)?CCLoadSpriteType_EQUIP:CCLoadSpriteType_GOODS;
    auto bgIcon = CCLoadSprite::createSprite(_tmpBGPath.c_str());
    CCCommonUtils::setSpriteMaxSize(bgIcon, 60);
    auto icon = CCLoadSprite::createSprite(_tmpIconPath.c_str(),true,loadType);
    CCCommonUtils::setSpriteMaxSize(icon, 60);
    icon->setTag(1);
    
    if (m_idx==0) {
        m_resEIconNode->removeAllChildren();
        CCCommonUtils::setSpriteMaxSize(bgIcon, 70);
        CCCommonUtils::setSpriteMaxSize(icon, 70);
        m_resEIconNode->addChild(bgIcon);
        m_resEIconNode->addChild(icon);
    }else if (m_idx==1) {
        m_mateNode1->removeAllChildren();
        m_mateNode1->addChild(bgIcon);
        m_mateNode1->addChild(icon);
    }else if (m_idx==2) {
        m_mateNode2->removeAllChildren();
        m_mateNode2->addChild(bgIcon);
        m_mateNode2->addChild(icon);
    }else if (m_idx==3) {
        m_mateNode3->removeAllChildren();
        m_mateNode3->addChild(bgIcon);
        m_mateNode3->addChild(icon);
    }else if (m_idx==4) {
        m_mateNode4->removeAllChildren();
        m_mateNode4->addChild(bgIcon);
        m_mateNode4->addChild(icon);
    }
    
    onMakeCrtPro();
}

void EquipmentCreateView::refreshMateData()
{
    m_data.clear();
    m_Type = 1;
    string mateId = "";
    auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    if (eInfo.mateVec.size()>=m_idx) {
        string mate = eInfo.mateVec[m_idx-1];
        vector<string> matecell;
        CCCommonUtils::splitString(mate, ";", matecell);
        if (matecell.size()>=1) {
            mateId = matecell[0];
        }
    }
    if (mateId=="") {
        return;
    }
    
    int tmpMateCnt = 0;
    int curMateId = atoi(mateId.c_str());
    for (int i=0; i<10; i++) {
        int tmpMateId = curMateId+i;
        if (m_tmpMates.find(tmpMateId) != m_tmpMates.end() && m_tmpMates[tmpMateId]>0) {
//            m_data.push_back(CC_ITOA(tmpMateId));
            tmpMateCnt += m_tmpMates[tmpMateId];
        }
        
        if ( ToolController::getInstance()->m_toolInfos.find(tmpMateId) != ToolController::getInstance()->m_toolInfos.end() )
        {
            m_data.push_back(CC_ITOA(tmpMateId));
        }
    }
    
    float miny = m_tabView->minContainerOffset().y;
    CCPoint pos  = m_tabView->getContentOffset();
    m_tabView->reloadData();
    float mincurry = m_tabView->minContainerOffset().y;
    pos.y+=(-miny+mincurry);
    m_tabView->setContentOffset(pos);
}

void EquipmentCreateView::refreshEquipData()
{
    m_data.clear();
    m_Type = 0;
    
    for (int i=0; i<m_myEquips.size(); i++)
    {
        if (m_tmpEquips != m_myEquips[i]) {
            int eItemId = EquipmentController::getInstance()->m_MyEquipmentMap[m_myEquips[i]];
            auto& curInfo = EquipmentController::getInstance()->EquipmentInfoMap[eItemId];
            if (curInfo.site!=6) {
                m_data.push_back(m_myEquips[i]);
            }
        }
    }
    for (int i=0; i<m_myOnEquips.size(); i++)
    {
        if (m_myOnEquips[i] != m_tmpEquips) {
            int eItemId = EquipmentController::getInstance()->m_MyOnEquip[m_myOnEquips[i]];
            auto& curInfo = EquipmentController::getInstance()->EquipmentInfoMap[eItemId];
            if (curInfo.site!=6) {
                m_data.push_back(m_myOnEquips[i]);
            }
        }
    }
    
    m_noMateLabel->setString("");
    if (m_data.size()<=0) {
        if (m_tmpEquips != "") {
            m_noMateLabel->setString(_lang_1("119067", _lang("119027").c_str()));
        }
    }
    
    float miny = m_tabView->minContainerOffset().y;
    CCPoint pos  = m_tabView->getContentOffset();
    m_tabView->reloadData();
    float mincurry = m_tabView->minContainerOffset().y;
    pos.y+=(-miny+mincurry);
    m_tabView->setContentOffset(pos);
}

void EquipmentCreateView::generateData()
{
    m_tabView->reloadData();
}

SEL_CCControlHandler EquipmentCreateView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickEquipCrtBtn", EquipmentCreateView::onClickEquipCrtBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickGetMore", EquipmentCreateView::onClickGetMore);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHelpBtnClick", EquipmentCreateView::onHelpBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddResBtnClick", EquipmentCreateView::onAddResBtnClick);
    return NULL;
}

bool EquipmentCreateView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleNode", CCNode*, this->m_titleNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipCrtBtn", CCControlButton*, this->m_equipCrtBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_middleNode", CCNode*, this->m_middleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btmMidNode", CCNode*, this->m_btmMidNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode", CCNode*, this->m_particleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pNodeCenter", CCNode*, this->m_pNodeCenter);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pNodeLL", CCNode*, this->m_pNodeLL);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pNodeLR", CCNode*, this->m_pNodeLR);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pNodeSL", CCNode*, this->m_pNodeSL);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pNodeSR", CCNode*, this->m_pNodeSR);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pNodeEqiup", CCNode*, this->m_pNodeEqiup);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_forgeInner", CCNode*, this->m_forgeInner);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_forgeInner2", CCNode*, this->m_forgeInner2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_guide1Node", CCNode*, this->m_guide1Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_guide2Node", CCNode*, this->m_guide2Node);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newEIconNode", CCNode*, this->m_newEIconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipLabel", CCLabelIF*, this->m_equipLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg1Label", CCLabelIF*, this->m_msg1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg2Label", CCLabelIF*, this->m_msg2Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg3Label", CCLabelIF*, this->m_msg3Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg4Label", CCLabelIF*, this->m_msg4Label);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_costLabel", CCLabelIF*, this->m_costLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_colorBarNode", CCNode*, this->m_colorBarNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_percentNode", CCNode*, this->m_percentNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_midBgNode", CCNode*, this->m_midBgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireBG", CCScale9Sprite*, this->m_fireBG);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rectSpr", CCSprite*, this->m_rectSpr);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode", CCNode*, this->m_mateNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateLabel", CCLabelIF*, this->m_mateLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resEquipBG", CCScale9Sprite*, this->m_resEquipBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resEIconNode", CCNode*, this->m_resEIconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mate1BG", CCScale9Sprite*, this->m_mate1BG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mate2BG", CCScale9Sprite*, this->m_mate2BG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mate3BG", CCScale9Sprite*, this->m_mate3BG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mate4BG", CCScale9Sprite*, this->m_mate4BG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode1", CCNode*, this->m_mateNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode2", CCNode*, this->m_mateNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode3", CCNode*, this->m_mateNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateNode4", CCNode*, this->m_mateNode4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateLabel1", CCLabelIF*, this->m_mateLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateLabel2", CCLabelIF*, this->m_mateLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateLabel3", CCLabelIF*, this->m_mateLabel3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateLabel4", CCLabelIF*, this->m_mateLabel4);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noEquip", CCSprite*, this->m_noEquip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_equipAddSpr", CCNode*, this->m_equipAddSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateSpr1", CCSprite*, this->m_mateSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateSpr2", CCSprite*, this->m_mateSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateSpr3", CCSprite*, this->m_mateSpr3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateSpr4", CCSprite*, this->m_mateSpr4);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selNode", CCNode*, this->m_selNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode", CCNode*, this->m_desNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemBg", CCScale9Sprite*, this->m_itemBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desBg", CCScale9Sprite*, this->m_desBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrowSpr", CCSprite*, this->m_arrowSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrowSprDes", CCSprite*, this->m_arrowSprDes);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flyNode", CCNode*, this->m_flyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noMateLabel", CCLabelIF*, this->m_noMateLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getMoreBtn", CCControlButton*, this->m_getMoreBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selTitleLabel", CCLabelIF*, this->m_selTitleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selBottomLabel", CCLabelIF*, this->m_selBottomLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_costNode", CCNode*, this->m_costNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addResNode", CCNode*, this->m_addResNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addResBtn", CCControlButton*, this->m_addResBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resNumLabel", CCLabelIF*, this->m_resNumLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resIconNode", CCNode*, this->m_resIconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeBar", CCScale9Sprite*, this->m_timeBar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barLabel", CCLabelIF*, this->m_barLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barNode", CCNode*, this->m_barNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode", CCNode*, this->m_infoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnMsgLabel", CCLabelIF*, this->m_btnMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inBtnGoldNum", CCLabelIF*, this->m_inBtnGoldNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_errorLabel", CCLabelIF*, this->m_errorLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_popNode", CCNode*, this->m_popNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_par1Node", CCNode*, this->m_par1Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_par2Node", CCNode*, this->m_par2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_par3Node", CCNode*, this->m_par3Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_par4Node", CCNode*, this->m_par4Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_par5Node", CCNode*, this->m_par5Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_par6Node", CCNode*, this->m_par6Node);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpBtn", CCControlButton*, this->m_helpBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_handNode", CCNode*, this->m_handNode);
    
    return false;
}

void EquipmentCreateView::onHelpBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    GlobalData::shared()->isBind = true;
#endif
    CCCommonUtils::showHelpShiftFAQ("45202");

}

void EquipmentCreateView::onClickGetMore(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    if (m_Type==1) {//材料
        onYesGoToBuild();
    }else if (m_Type==0){//装备
        PopupViewController::getInstance()->addPopupInView(StoreView::create(STORE_TYPE_GOTO_STORE));
    }
}

void EquipmentCreateView::onAddResBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    CCLOGFUNC();
    PopupViewController::getInstance()->addPopupInView(SacrificePopUpView::create());
}

void EquipmentCreateView::onTipAddEquip()
{
    if (!m_selNode->isVisible() || m_idx!=0) {
        changeData(0);
    }
    return;
    
//    if (!m_canPlayAdd) {
//        return;
//    }
//    m_canPlayAdd = false;
    
//    m_equipAddSpr->stopAllActions();
//    CCActionInterval * scaleTo1 = CCScaleTo::create(0.15, 2);
//    CCActionInterval * scaleTo2 = CCScaleTo::create(0.15, 1.0);
//    CCActionInterval * scaleTo3 = CCScaleTo::create(0.15, 2);
//    CCActionInterval * scaleTo4 = CCScaleTo::create(0.15, 1.0);
//    CCActionInterval * sequence = CCSequence::create(scaleTo1, scaleTo2, scaleTo3, scaleTo4, NULL);
//    CCRepeat* repeatAni = CCRepeat::create(sequence, 2);
    
//    CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(EquipmentCreateView::flyEnd));
    
//    m_equipAddSpr->runAction(sequence);
    
//    this->scheduleOnce(schedule_selector(EquipmentCreateView::canPlayAdd), 2.5);
}

void EquipmentCreateView::canPlayAdd(float dt)
{
    m_canPlayAdd = true;
}

void EquipmentCreateView::onClickEquipCrtBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
//    auto equipView = EquipFinishView::create(m_itemId);
//    equipView->setCurPopType(0);
//    PopupViewController::getInstance()->addPopupView(equipView);
//    showPopParticle(1);
//    return;
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           , CCString::createWithFormat("FR3_3"));
    
    if (m_qid == QID_MAX) {
        int tmpQid = QueueController::getInstance()->getMinTimeQidByType(TYPE_FORGE);
        if (tmpQid == QID_MAX)
        {
//            if (!m_isEnough) {
////                int lackSilver = m_costRes-CCCommonUtils::getCurResourceByType(m_costResType);
////                int lackGold = CCCommonUtils::getResGlodByType(Silver, lackSilver);
////                PopupViewController::getInstance()->addPopupView(ConfirmLackSilverView::create(lackSilver,lackGold
////                                                                                           ,this
////                                                                                           ,callfunc_selector(EquipmentCreateView::onOKCreateEquip)
////                                                                                           , _lang("102312"), _lang("102104")),false);
////
////                onOKCreateEquip();
////                return;
//                int bid = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_SACRIFICE);
//                if (bid>0) {
//                    int curNum = CCCommonUtils::getCurResourceByType(m_costResType);
//                    //缺少钢材：{0}.许愿池中许愿可以快速获得钢材，立即前往？
//                    YesNoDialog::show(_lang_1("119070", CC_CMDITOA(m_costRes-curNum).c_str()), CCCallFunc::create(this, callfunc_selector(EquipmentCreateView::onGotoGetRes)));
//                }else {
//                    //缺少钢材，建造许愿池可以快速获得钢材，立即建造？
//                    YesNoDialog::show(_lang("119071").c_str(), CCCallFunc::create(this, callfunc_selector(EquipmentCreateView::onGotoGetRes)));
//                }
//                return;
//            }
            
            auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
            if (m_tmpEquips=="" && m_myEquips.size()>0 && eInfo.costLv>0) {
                onTipAddEquip();
                return;
            }
            
            if (GlobalData::shared()->playerInfo.level < eInfo.level) {
                YesNoDialog::show(_lang("119041").c_str(), CCCallFunc::create(this, callfunc_selector(EquipmentCreateView::onOKCreateEquip)));
            }else if (m_tmpEquips != "") {
                int tmpItemId = EquipmentController::getInstance()->m_MyEquipmentMap[m_tmpEquips];
                EquipmentInfo& info = EquipmentController::getInstance()->EquipmentInfoMap[tmpItemId];
                string colorName = CCCommonUtils::getColorName(info.color);
                string equipName = _lang(info.name);
                
                YesNoDialog::show(_lang_3("119050", colorName.c_str(), equipName.c_str(), CC_ITOA(info.level)), CCCallFunc::create(this, callfunc_selector(EquipmentCreateView::onOKCreateEquip)));
            }else {
                onOKCreateEquip();
            }
        }
        else {
            auto& qInfo = GlobalData::shared()->allQueuesInfo[tmpQid];
            int tmpCurTime = GlobalData::shared()->getWorldTime();
            int tmpTime = qInfo.finishTime - tmpCurTime;
            tmpTime = MAX(0, tmpTime);
            
            YesNoDialog::showTime( _lang("119034").c_str() , CCCallFunc::create(this, callfunc_selector(EquipmentCreateView::onCostGoldOtherQueue)), tmpTime, _lang("104903").c_str(), false);
        }
    }
    else {
        if(_tmpGold>0) {
            YesNoDialog::showTime( _lang("102283").c_str() , CCCallFunc::create(this, callfunc_selector(EquipmentCreateView::onCostGoldBack)), m_curTime, _lang("104903").c_str(), false);
        }
    }
}

void EquipmentCreateView::onGotoGetRes()
{
    int bid = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_SACRIFICE);
    auto world = WorldMapView::instance();
    if (world) {
        FunBuildController::getInstance()->willMoveToBuildType = FUN_BUILD_SACRIFICE;
        world->leaveWorld();
    }else {
        AutoSafeRef temp(this);
        
        PopupViewController::getInstance()->removeAllPopupView();
        auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
        if(layer) {
            if (bid>0) {
                layer->onMoveToBuildAndPlay(bid);
            }else {
                int itemId =layer->findCanBuildTile(FUN_BUILD_SACRIFICE);
                if(itemId==-1){
                    itemId =layer->findCanBuildTile(2);
                    if (itemId==-1) {
                        return;
                    }
                }
                layer->onMoveToBuildAndPlay(itemId);
            }
        }
    }
}

void EquipmentCreateView::onOKCreateEquip()
{
    silverCostGold=0;
    int lackSilver = m_costRes-CCCommonUtils::getCurResourceByType(m_costResType);
    if (lackSilver>0) {
        silverCostGold = CCCommonUtils::getResGlodByType(Silver, lackSilver);
        PopupViewController::getInstance()->addPopupView(ConfirmLackSilverView::create(lackSilver,silverCostGold
                                                                                       ,this
                                                                                       ,callfunc_selector(EquipmentCreateView::startCreateEquipment)
                                                                                       , _lang("111671"), _lang("102349")),false);
    }else{
        startCreateEquipment();
    }
}

void EquipmentCreateView::startCreateEquipment(){
    m_equipCrtBtn->setEnabled(false);
    vector<int> curMates;
    for (int i=1; i<=4; i++) {
        if (m_selMates[i] > 0) {
            curMates.push_back(m_selMates[i]);
        }
    }
    EquipmentController::getInstance()->startCreateEquipment(m_tmpEquips, CC_ITOA(m_itemId), curMates, silverCostGold);
}

void EquipmentCreateView::onCostGoldBack()
{
    if (m_qid != QID_MAX) {
        if (_tmpGold > 0) {
            if (GlobalData::shared()->playerInfo.gold < _tmpGold) {
                YesNoDialog::gotoPayTips();
                return;
            }
        }
        QueueController::getInstance()->startCCDQueue(m_qid, "", false, _tmpGold,"",5);
    }
}

void EquipmentCreateView::onCostGoldOtherQueue()
{
    int tmpQid = QueueController::getInstance()->getMinTimeQidByType(TYPE_FORGE);
    if (tmpQid != QID_MAX) {
        auto& qInfo = GlobalData::shared()->allQueuesInfo[tmpQid];
        int tmpCurTime = GlobalData::shared()->getWorldTime();
        int tmpTime = qInfo.finishTime - tmpCurTime;
        tmpTime = MAX(0, tmpTime);
        
        int tmpGold = CCCommonUtils::getGoldByTime(tmpTime);
        if (tmpGold > 0) {
            if (GlobalData::shared()->playerInfo.gold < tmpGold) {
                YesNoDialog::gotoPayTips();
                return;
            }
        }
        QueueController::getInstance()->startCCDQueue(tmpQid, "", false, tmpGold,"",5);
    }
}

void EquipmentCreateView::updateCrtRet(CCObject* p)
{
    m_equipCrtBtn->setEnabled(true);
    showParticle();
    playStartCrtParticle();
    this->getAnimationManager()->runAnimationsForSequenceNamed("Start");
    this->scheduleOnce(schedule_selector(EquipmentCreateView::playCrtSound), 0.0f);
    
    m_qid = QID_MAX;
    int tqid = QueueController::getInstance()->getMinTimeQidByType(TYPE_FORGE);
    if (tqid!=QID_MAX) {
        auto& qInfo = GlobalData::shared()->allQueuesInfo[tqid];
        int itemId = QueueController::getInstance()->getItemId(qInfo.key);
        if ((itemId - itemId%10) == (m_itemId-m_itemId%10)) {
            m_curCreatingItemId = itemId;
            m_qid = tqid;
            m_btnMsgLabel->setString(_lang("104903"));
            CCCommonUtils::setButtonTitle(m_equipCrtBtn, "");
            m_infoNode->setVisible(true);
        }
    }
    
    int curNum = CCCommonUtils::getCurResourceByType(m_costResType);
    m_resNumLabel->setColor({193,162,110});
    if (curNum<m_costRes) {
        m_isEnough = false;
        m_resNumLabel->setColor(ccRED);
    }
    m_resNumLabel->setString(CC_CMDITOA(curNum));
    m_costLabel->setPositionX(m_resNumLabel->getPositionX() + m_resNumLabel->getContentSize().width*m_resNumLabel->getOriginScaleX());
    m_addResNode->setPositionX(m_costLabel->getPositionX() + m_costLabel->getContentSize().width*m_costLabel->getOriginScaleX()+m_addResBtn->getContentSize().width);
    
    if (m_resEIconNode->getChildrenCount()>0) {
        CCNode* tmpNode = m_resEIconNode->getChildByTag(1);
        if (tmpNode) {
            CCSprite* icon = dynamic_cast<CCSprite*>(tmpNode);
            if (icon) {
                CCFadeTo* fadeTo1 = CCFadeTo::create(0.5, 120);
                CCFadeTo* fadeTo2 = CCFadeTo::create(0.5, 255);
                CCActionInterval * repeatForever =CCRepeatForever::create(CCSequence::create(fadeTo1,fadeTo2,NULL));
                icon->runAction(repeatForever);
            }
        }
    }
    
    if (m_mateNode1->getChildrenCount()>0) {
        CCNode* tmpNode = m_mateNode1->getChildByTag(1);
        if (tmpNode) {
            CCSprite* icon = dynamic_cast<CCSprite*>(tmpNode);
            if (icon) {
                CCFadeTo* fadeTo1 = CCFadeTo::create(0.5, 120);
                CCFadeTo* fadeTo2 = CCFadeTo::create(0.5, 255);
                CCActionInterval * repeatForever =CCRepeatForever::create(CCSequence::create(fadeTo1,fadeTo2,NULL));
                icon->runAction(repeatForever);
            }
        }
    }
    
    if (m_mateNode2->getChildrenCount()>0) {
        CCNode* tmpNode = m_mateNode2->getChildByTag(1);
        if (tmpNode) {
            CCSprite* icon = dynamic_cast<CCSprite*>(tmpNode);
            if (icon) {
                CCFadeTo* fadeTo1 = CCFadeTo::create(0.5, 120);
                CCFadeTo* fadeTo2 = CCFadeTo::create(0.5, 255);
                CCActionInterval * repeatForever =CCRepeatForever::create(CCSequence::create(fadeTo1,fadeTo2,NULL));
                icon->runAction(repeatForever);
            }
        }
    }
    
    if (m_mateNode3->getChildrenCount()>0) {
        CCNode* tmpNode = m_mateNode3->getChildByTag(1);
        if (tmpNode) {
            CCSprite* icon = dynamic_cast<CCSprite*>(tmpNode);
            if (icon) {
                CCFadeTo* fadeTo1 = CCFadeTo::create(0.5, 120);
                CCFadeTo* fadeTo2 = CCFadeTo::create(0.5, 255);
                CCActionInterval * repeatForever =CCRepeatForever::create(CCSequence::create(fadeTo1,fadeTo2,NULL));
                icon->runAction(repeatForever);
            }
        }
    }
    
    if (m_mateNode4->getChildrenCount()>0) {
        CCNode* tmpNode = m_mateNode4->getChildByTag(1);
        if (tmpNode) {
            CCSprite* icon = dynamic_cast<CCSprite*>(tmpNode);
            if (icon) {
                CCFadeTo* fadeTo1 = CCFadeTo::create(0.5, 120);
                CCFadeTo* fadeTo2 = CCFadeTo::create(0.5, 255);
                CCActionInterval * repeatForever =CCRepeatForever::create(CCSequence::create(fadeTo1,fadeTo2,NULL));
                icon->runAction(repeatForever);
            }
        }
    }
    
    onEnterFrame(0);
}

void EquipmentCreateView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell)
{
    EquipmentSelCell* equipCell = dynamic_cast<EquipmentSelCell*>(cell);
    if (equipCell && m_selNode->isVisible()) {
        startPt = m_selNode->convertTouchToNodeSpace(m_tabView->m_pCurTouch);
        equipCell->cellTouchEnded(m_tabView->m_pCurTouch);
    }
}

cocos2d::CCSize EquipmentCreateView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(216, 204);
    }
    return CCSize(90, 85);
}

CCTableViewCell* EquipmentCreateView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, ssize_t idx)
{
    if(idx >= m_data.size()){
        return NULL;
    }
    
    string itemId = m_data[idx];
    int num = 0;
    if (m_Type==1) {
        num = m_tmpMates[atoi(itemId.c_str())];
    }
    
    bool equipOn=false;
    for (int i=0; i<m_myOnEquips.size(); i++)
    {
        if (m_myOnEquips[i]==itemId)
        {
            equipOn=true;
            break;
        }
    }
    EquipmentSelCell* cell = (EquipmentSelCell*)table->dequeueGrid();
    if(cell){
        cell->setData(itemId, m_Type, num, m_itemId,equipOn);
    }else{
        cell = EquipmentSelCell::create(itemId, m_Type, num, m_itemId,equipOn);
    }
    return cell;
}

ssize_t EquipmentCreateView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    return ceil(m_data.size()*1.0/6);
}

ssize_t EquipmentCreateView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 6;
}

bool EquipmentCreateView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(m_selNode->isVisible()){
        if (!isTouchInside(m_infoList, pTouch)) {
            m_selNode->setVisible(false);
            onMakeCrtPro(false);
        }
        return false;
    }else if (m_desNode->isVisible()){
        if (!isTouchInside(m_desBg, pTouch)) {
            m_desNode->setVisible(false);
        }
    }else {
        if (isTouchInside(m_equipCrtBtn, pTouch)) {
            m_equipCrtBtn->onTouchBegan(pTouch, pEvent);
        }
    }
    return true;
}

void EquipmentCreateView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isTouchInside(m_resEquipBG, pTouch) || isTouchInside(m_mate1BG, pTouch) || isTouchInside(m_mate2BG, pTouch) || isTouchInside(m_mate3BG, pTouch) || isTouchInside(m_mate4BG, pTouch)) {
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    }
    if (m_guide1Node->isVisible() && isTouchInside(m_guide1Node, pTouch)) {
        m_guide1Node->setVisible(false);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                               , CCString::createWithFormat("FR3_1"));
    }
    else if (m_guide2Node->isVisible() && isTouchInside(m_guide2Node, pTouch)) {
        m_guide2Node->setVisible(false);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                               , CCString::createWithFormat("FR3_2"));
    }
    else if (isTouchInside(m_resEquipBG, pTouch) && !m_noEquip->isVisible()) {
        changeData(0);
    }
    else if (isTouchInside(m_mate1BG, pTouch) && !m_mateSpr1->isVisible()) {
        changeData(1);
    }
    else if (isTouchInside(m_mate2BG, pTouch) && !m_mateSpr1->isVisible()) {
        changeData(2);
    }
    else if (isTouchInside(m_mate3BG, pTouch) && !m_mateSpr1->isVisible()) {
        changeData(3);
    }
    else if (isTouchInside(m_mate4BG, pTouch) && !m_mateSpr1->isVisible()) {
        changeData(4);
    }
    else if (m_colorCellMap[WHITE]->isTouchIn(pTouch)) {
        SoundController::sharedSound()->playEffects(Music_Sfx_UI_jewel);
        onRefreshEquipInfo(WHITE,true,true);
    }
    else if (m_colorCellMap[GREEN]->isTouchIn(pTouch)) {
        SoundController::sharedSound()->playEffects(Music_Sfx_UI_jewel);
        onRefreshEquipInfo(GREEN,true,true);
    }
    else if (m_colorCellMap[BLUE]->isTouchIn(pTouch)) {
        SoundController::sharedSound()->playEffects(Music_Sfx_UI_jewel);
        onRefreshEquipInfo(BLUE,true,true);
    }
    else if (m_colorCellMap[PURPLE]->isTouchIn(pTouch)) {
        SoundController::sharedSound()->playEffects(Music_Sfx_UI_jewel);
        onRefreshEquipInfo(PURPLE,true,true);
    }
    else if (m_colorCellMap[ORANGE]->isTouchIn(pTouch)) {
        SoundController::sharedSound()->playEffects(Music_Sfx_UI_jewel);
        onRefreshEquipInfo(ORANGE,true,true);
    }
    else if (m_colorCellMap[GOLDEN]->isTouchIn(pTouch)) {
        SoundController::sharedSound()->playEffects(Music_Sfx_UI_jewel);
        onRefreshEquipInfo(GOLDEN,true,true);
    }
    else if (isTouchInside(m_equipCrtBtn, pTouch) && m_equipCrtBtn->isVisible() && m_equipCrtBtn->isEnabled()) {
        m_equipCrtBtn->onTouchEnded(pTouch, pEvent);
        onClickEquipCrtBtn(NULL, Control::EventType::TOUCH_DOWN);
    }
}

void EquipmentCreateView::onRefreshEquipInfo(int color,bool needRefresh/*=true*/,bool showDes/*=false*/)
{
    m_desNode->setVisible(showDes);
    for (int i=WHITE; i<=GOLDEN; i++) {
        if(i==color){
            m_colorCellMap[i]->setSelected();
            m_arrowSprDes->setPositionX(m_colorCellMap[i]->getPositionX()+m_colorBarNode->getPositionX()+m_colorCellMap[i]->getContentSize().width/2);
        }else{
            m_colorCellMap[i]->setNotSelected();
        }
    }
    
    if (color == WHITE) {
        m_equipLabel->setColor(ccc3(177, 172, 168));
        m_msg1Label->setColor(ccc3(177, 172, 168));
        m_msg2Label->setColor(ccc3(177, 172, 168));
        m_msg3Label->setColor(ccc3(177, 172, 168));
        m_msg4Label->setColor(ccc3(177, 172, 168));
    }else if (color == GREEN) {
        m_equipLabel->setColor(ccc3(109, 188, 15));
        m_msg1Label->setColor(ccc3(109, 188, 15));
        m_msg2Label->setColor(ccc3(109, 188, 15));
        m_msg3Label->setColor(ccc3(109, 188, 15));
        m_msg4Label->setColor(ccc3(109, 188, 15));
    }else if (color == BLUE) {
        m_equipLabel->setColor(ccc3(25, 175, 197));
        m_msg1Label->setColor(ccc3(25, 175, 197));
        m_msg2Label->setColor(ccc3(25, 175, 197));
        m_msg3Label->setColor(ccc3(25, 175, 197));
        m_msg4Label->setColor(ccc3(25, 175, 197));
    }else if (color == PURPLE) {
        m_equipLabel->setColor(ccc3(142, 55, 203));
        m_msg1Label->setColor(ccc3(142, 55, 203));
        m_msg2Label->setColor(ccc3(142, 55, 203));
        m_msg3Label->setColor(ccc3(142, 55, 203));
        m_msg4Label->setColor(ccc3(142, 55, 203));
    }else if (color == ORANGE) {
        m_equipLabel->setColor(ccc3(204, 127, 44));
        m_msg1Label->setColor(ccc3(204, 127, 44));
        m_msg2Label->setColor(ccc3(204, 127, 44));
        m_msg3Label->setColor(ccc3(204, 127, 44));
        m_msg4Label->setColor(ccc3(204, 127, 44));
    }else if (color == GOLDEN) {
        m_equipLabel->setColor(ccc3(198, 176, 28));
        m_msg1Label->setColor(ccc3(198, 176, 28));
        m_msg2Label->setColor(ccc3(198, 176, 28));
        m_msg3Label->setColor(ccc3(198, 176, 28));
        m_msg4Label->setColor(ccc3(198, 176, 28));
    }
    
    int msgCnt = 0;
    for (int i=0; i<=5; i++) {
        if (EquipmentController::getInstance()->EquipmentInfoMap.find(m_itemId-i) != EquipmentController::getInstance()->EquipmentInfoMap.end())
        {
            auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId-i];
            if (eInfo.color == color)
            {
                
                for (int i=1; i<=4; i++)
                {
                    string tmpInfo = "";
                    if (eInfo.showDias.find(i) != eInfo.showDias.end()) {
                        string dialog = eInfo.showDias[i];
                        if (dialog!="" && eInfo.values.find(i)!=eInfo.values.end()) {
                            tmpInfo = _lang(dialog)+" +"+CC_ITOA_MONEY(eInfo.values[i])+"%";
                            msgCnt++;
//                            tmpInfo = tmpInfo + _lang_1(dialog, CC_ITOA_1(eInfo.values[i]));
                        }
                    }
                    
                    if (i==1) {
                        m_msg1Label->setString(tmpInfo);
                        if(CCCommonUtils::isIosAndroidPad()){
                            float fw = m_msg1Label->getContentSize().width;
                            float s = m_msg1Label->getScaleX();
                            if (fw>900) {//防爆框
                                m_msg1Label->setScaleX(900/fw*s);
                            }
                        }
                        else
                            m_msg1Label->setMaxScaleXByWidth(520);
                    }else if (i==2) {
                        m_msg2Label->setString(tmpInfo);
                        if(CCCommonUtils::isIosAndroidPad()){
                            float fw = m_msg2Label->getContentSize().width;
                            float s = m_msg2Label->getScaleX();
                            if (fw>900) {//防爆框
                                m_msg2Label->setScaleX(900/fw*s);
                            }
                        }
                        else
                            m_msg2Label->setMaxScaleXByWidth(520);
                    }else if (i==3) {
                        m_msg3Label->setString(tmpInfo);
                        if(CCCommonUtils::isIosAndroidPad()){
                            float fw = m_msg3Label->getContentSize().width;
                            float s = m_msg3Label->getScaleX();
                            if (fw>900) {//防爆框
                                m_msg3Label->setScaleX(900/fw*s);
                            }
                        }
                        else
                            m_msg3Label->setMaxScaleXByWidth(520);
                    }else if (i==4) {
                        m_msg4Label->setString(tmpInfo);
                        if(CCCommonUtils::isIosAndroidPad()){
                            float fw = m_msg4Label->getContentSize().width;
                            float s = m_msg4Label->getScaleX();
                            if (fw>900) {//防爆框
                                m_msg4Label->setScaleX(900/fw*s);
                            }
                        }
                        else
                            m_msg4Label->setMaxScaleXByWidth(520);
                    }
                }
                
                break;
            }
        }
    }
    
    if(!CCCommonUtils::isIosAndroidPad()){
        if (msgCnt == 1) {
            m_msg1Label->setPositionY(-60);
        }else if (msgCnt == 2) {
            m_msg1Label->setPositionY(-47);
            m_msg2Label->setPositionY(-73);
        }else if (msgCnt == 3) {
            m_msg1Label->setPositionY(-33);
            m_msg2Label->setPositionY(-60);
            m_msg3Label->setPositionY(-87);
        }else if (msgCnt == 4) {
            m_msg1Label->setPositionY(-20);
            m_msg2Label->setPositionY(-47);
            m_msg3Label->setPositionY(-73);
            m_msg4Label->setPositionY(-99);
        }
    }
}

//void EquipmentCreateView::resetRectSpr(float dt)
//{
//    if (CCCommonUtils::isIosAndroidPad()) {
//        m_rectSpr->setScale(2.4f);
//    }
//    else
//        m_rectSpr->setScale(1.0f);
//}

void EquipmentCreateView::onMakeCrtPro(bool needRefresh/*=true*/,bool onEnter/*=false*/)
{
    auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    vector<int> curMates;
    for (int i=1; i<=4; i++) {
        if (m_selMates[i] > 0) {
            curMates.push_back(m_selMates[i]);
        }
    }
    
    int maxProIndex = 0;
    if ((m_tmpEquips!="" || eInfo.costLv==0) && eInfo.mateVec.size() == curMates.size()) {
        //材料全了
        m_proVec = EquipmentController::getInstance()->MakeEquipmentPro(m_tmpEquips, m_tmpEquips, curMates, _isUUid);
        string tmp = "";
        m_colorCellMap[WHITE]->setData(WHITE, m_proVec[0]);
        m_colorCellMap[GREEN]->setData(GREEN, m_proVec[1]);
        m_colorCellMap[BLUE]->setData(BLUE, m_proVec[2]);
        m_colorCellMap[PURPLE]->setData(PURPLE, m_proVec[3]);
        m_colorCellMap[ORANGE]->setData(ORANGE, m_proVec[4]);
        m_colorCellMap[GOLDEN]->setData(GOLDEN, m_proVec[5]);
        
        float maxPro = m_proVec[0];
        for(int i=1; i<m_proVec.size(); i++)
        {
            if (m_proVec[i] > maxPro)
            {
                maxPro = m_proVec[i];
                maxProIndex = i;
            }
        }
        //显示钢材和时间
        m_costNode->setVisible(true);
        //显示百分比
        m_percentNode->setVisible(true);
        //显示按钮
        m_equipCrtBtn->setVisible(true);
        //隐藏材料不足提示
        m_errorLabel->setVisible(false);
        //设置锻造按钮
        m_equipCrtBtn->setEnabled(true);
        if (!m_infoNode->isVisible()) {
            CCCommonUtils::setButtonTitle(m_equipCrtBtn, _lang("119006").c_str());//锻造
        }
        //隐藏材料框
        m_selNode->setVisible(false);
        
        string aniName = "2zu";
        if(eInfo.costLv==0){
           aniName = "2zu";
        }else{
            if(curMates.size()==2){
                aniName = "3zu";
            }else if (curMates.size()==3){
                aniName = "4zu";
            }else if (curMates.size()==4){
                aniName = "5zu";
            }
        }
        m_forgeInner2->removeAllChildrenWithCleanup(true);
        if (true) {
            auto animationObj = new IFSkeletonAnimation("Spine/Imperial/duanzao.json","Imperial/Imperial_30.atlas");
            if (animationObj) {
                
                animationObj->setPosition(CCPoint(-320, -493));
                m_forgeInner2->addChild(animationObj);
                if(onEnter){
                    spTrackEntry* entry = animationObj->setAnimation(0, aniName.c_str(), false);
                    animationObj->setTimeScale(entry->endTime/0.2f);
                }else{
                    animationObj->setAnimation(0, aniName.c_str(), false);
                }
                animationObj->autorelease();
            }
        }
        
        string guideValue = CCUserDefault::sharedUserDefault()->getStringForKey("Equp_Guide", "");
        if (guideValue == "1") {//进行锻造的强制引导
            CCUserDefault::sharedUserDefault()->setStringForKey("Equp_Guide", "");
            CCUserDefault::sharedUserDefault()->flush();
            GuideController::share()->setGuide("3111000");
        }
    }
    else {
        m_colorCellMap[WHITE]->setData(WHITE, 0);
        m_colorCellMap[GREEN]->setData(GREEN, 0);
        m_colorCellMap[BLUE]->setData(BLUE, 0);
        m_colorCellMap[PURPLE]->setData(PURPLE, 0);
        m_colorCellMap[ORANGE]->setData(ORANGE, 0);
        m_colorCellMap[GOLDEN]->setData(GOLDEN, 0);
    }
    
    onRefreshEquipInfo(maxProIndex,needRefresh);
}

void EquipmentCreateView::onEnterFrame(float dt)
{
    if (m_qid == QID_MAX) {
        return;
    }
    
    auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
    int tmpCurTime = GlobalData::shared()->getWorldTime();
    if(qInfo.startTime==0){
        qInfo.startTime = tmpCurTime;
    }
    m_sumTime = qInfo.finishTime - qInfo.startTime;
    m_curTime = qInfo.finishTime - tmpCurTime;
    m_curTime = MAX(0, m_curTime);
    m_sumTime = MAX(1, m_sumTime);
    
    if (m_curTime>0) {
        m_barNode->setVisible(true);
        m_costNode->setVisible(false);
        
        if (qInfo.finishTime<=0) {
            m_curTime = 0;
            m_sumTime = 1;
        }
        float pro = m_curTime*1.0/m_sumTime;
        pro = pro>1?1:pro;
        m_timeBar->setScaleX(1-pro);
        m_barLabel->setString(CC_SECTOA(m_curTime));
        
        _tmpGold = CCCommonUtils::getGoldByTime(m_curTime);
        m_inBtnGoldNum->setString(CC_ITOA(_tmpGold));
    }
    else {
        m_qid = QID_MAX;
        m_barNode->setVisible(false);
        m_infoNode->setVisible(false);
        
        m_costNode->setVisible(true);
        CCCommonUtils::setButtonTitle(m_equipCrtBtn, _lang("119006").c_str());//锻造
        
        onGenerateMateData();
        onShowMates();
        
        removeParticle();
        this->getAnimationManager()->runAnimationsForSequenceNamed("End");
        
        playCrtEndParticle();
    }
}

void EquipmentCreateView::playClose()
{
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(EquipmentCreateView::AnimationClose));
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
}

void EquipmentCreateView::AnimationClose()
{
    m_initEnd = true;
    this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
    string lastName = this->getAnimationManager()->getLastCompletedSequenceName();
    if (lastName == "FadeIn") {
        return;
    }
//#if ( COCOS2D_DEBUG == 2)
    EquipmentCreateView::closeView(NULL);
//#else
 //   CCThreadManager::getInstance()->runInMainThread(this, callfuncO_selector(EquipmentCreateView::closeView),NULL);
//#endif
}

void EquipmentCreateView::closeView(CCObject* params)
{
    PopupViewController::getInstance()->goBackPopupView();
}

void EquipmentCreateView::AnimationFadeIn()
{
    string name = this->getAnimationManager()->getLastCompletedSequenceName();
    if (name=="FadeIn" && m_qid!=QID_MAX) {
        this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
        this->getAnimationManager()->runAnimationsForSequenceNamed("Loop");
    }
}

void EquipmentCreateView::addParticleToBatch(cocos2d::CCParticleSystemQuad *particle) {
    auto batchCount = m_parVec.size();
    while (batchCount--) {
        auto &batch = m_parVec[batchCount];
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
    m_smokeLayer->addChild(newBatch);
    newBatch->addChild(particle);
    m_parVec.push_back(newBatch);
}

void EquipmentCreateView::showParticle()
{
    string tmpStart = "Forge_bg";
    int count = 3;
    for (int i=1; i<=count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        addParticleToBatch(particle);
    }
    showParticleCenter();
}

//void EquipmentCreateView::addParticleToForgeBatch(cocos2d::CCParticleSystemQuad *particle) {
//    auto batchCount = m_parForgeVec.size();
//    while (batchCount--) {
//        auto &batch = m_parForgeVec[batchCount];
//        if (batch) {
//            auto batchBlend = batch->getBlendFunc();
//            auto particleBlend = particle->getBlendFunc();
//            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
//                batch->addChild(particle);
//                return;
//            }
//        }
//    }
//    auto newBatch = ParticleController::createParticleBatch();
//    m_forgeLayer->addChild(newBatch);
//    newBatch->addChild(particle);
//    m_parForgeVec.push_back(newBatch);
//}

void EquipmentCreateView::showParticleCenter(){
    
    int tqid = QueueController::getInstance()->getMinTimeQidByType(TYPE_FORGE);
    if (tqid!=QID_MAX) {
        auto& qInfo = GlobalData::shared()->allQueuesInfo[tqid];
        int TmpItemId = QueueController::getInstance()->getItemId(qInfo.key);
        auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[TmpItemId];
        
        m_pNodeSL->removeAllChildrenWithCleanup(true);
        m_pNodeSR->removeAllChildrenWithCleanup(true);
        m_pNodeLL->removeAllChildrenWithCleanup(true);
        m_pNodeLR->removeAllChildrenWithCleanup(true);
        m_pNodeCenter->removeAllChildrenWithCleanup(true);
        m_pNodeEqiup->removeAllChildrenWithCleanup(true);
        
        auto particle1 = ParticleController::createParticle("Forge_cent");
        particle1->setScale(2.0);
        m_pNodeCenter->addChild(particle1);
        
        if(eInfo.costLv>0){
            auto particle6 = ParticleController::createParticle("Forge_equip");
            particle6->setScale(2.0);
            m_pNodeEqiup->addChild(particle6);
        }else{
            m_pNodeCenter->setPositionY(m_pNodeCenter->getPositionY()+60);
        }
        int mateCount = eInfo.mateVec.size();
        if (mateCount>=1) {
            auto particle2 = ParticleController::createParticle("Forge_SL");
            particle2->setScale(2.0);
            m_pNodeSL->addChild(particle2);
        }
        if (mateCount>=2) {
            auto particle3 = ParticleController::createParticle("Forge_SR");
            particle3->setScale(2.0);
            m_pNodeSR->addChild(particle3);
        }
        if (mateCount>=3) {
            auto particle4 = ParticleController::createParticle("Forge_LL");
            particle4->setScale(2.0);
            m_pNodeLL->addChild(particle4);
        }
        if (mateCount>=4) {
            auto particle5 = ParticleController::createParticle("Forge_LR");
            particle5->setScale(2.0);
            m_pNodeLR->addChild(particle5);
        }
    }
}

void EquipmentCreateView::removeParticle()
{
    auto batchCount = m_parVec.size();
    while (batchCount--) {
        m_smokeLayer->removeChild(m_parVec[batchCount]);
    }
    m_parVec.clear();
    removeForgeParticle();
}
void EquipmentCreateView::removeForgeParticle()
{
    m_pNodeSL->removeAllChildrenWithCleanup(true);
    m_pNodeSR->removeAllChildrenWithCleanup(true);
    m_pNodeLL->removeAllChildrenWithCleanup(true);
    m_pNodeLR->removeAllChildrenWithCleanup(true);
    m_pNodeCenter->removeAllChildrenWithCleanup(true);
    m_pNodeEqiup->removeAllChildrenWithCleanup(true);
}

void EquipmentCreateView::addPopParticleToBatch(cocos2d::CCParticleSystemQuad *particle)
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

void EquipmentCreateView::showPopParticle(float dt)
{
    removePopParticle(0);
    m_popNode->addChild(m_popLayer);
    return;
    
    int curColor = -1;
    if (m_curCreatingItemId>0) {
        auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_curCreatingItemId];
        curColor = eInfo.color;
        m_curCreatingItemId = 0;
    }
    
    string tmpStart = "MallBag_";
    int count = 7;
    for (int i=1; i<=count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        if (curColor == WHITE) {
            particle->setStartColor(ccc4f(1, 1, 1, 1));
        }else if (curColor == GREEN) {
            particle->setStartColor(ccc4f(0, 1, 0, 1));
        }else if (curColor == BLUE) {
            particle->setStartColor(ccc4f(0, 1, 1, 1));
        }else if (curColor == PURPLE) {
            particle->setStartColor(ccc4f(1, 0, 1, 1));
        }else if (curColor == ORANGE) {
            particle->setStartColor(ccc4f(1, 0.5, 0, 1));
        }else if (curColor == GOLDEN) {
            particle->setStartColor(ccc4f(1, 1, 0, 1));
        }
        particle->setPosition(m_par6Node->getPosition());
        addPopParticleToBatch(particle);
    }
    
    string strEquipTmp = CCCommonUtils::getIcon(CC_ITOA(m_itemId));
    auto iconEquipTmp = CCLoadSprite::createSprite(strEquipTmp.c_str(), true,CCLoadSpriteType_EQUIP);
    iconEquipTmp->setPosition(m_par6Node->getPosition());
    m_popLayer->addChild(iconEquipTmp);
    
    iconEquipTmp->setScale(0.1);
    CCFiniteTimeAction *pAction1 = CCSequence::create(CCScaleTo::create(0.35, 1.5),CCScaleTo::create(0.15, 1.2),NULL);
    iconEquipTmp->runAction(CCSequence::create(pAction1, NULL));
    
    this->scheduleOnce(schedule_selector(EquipmentCreateView::removePopParticle), 1);
}

void EquipmentCreateView::removePopParticle(float dt)
{
    auto batchCount = m_parPopVec.size();
    while (batchCount--) {
        m_popLayer->removeChild(m_parPopVec[batchCount]);
    }
    m_parPopVec.clear();
    
    m_popNode->removeAllChildren();
}

void EquipmentCreateView::playStartCrtParticle()
{
    removePopParticle(0);
    m_popNode->addChild(m_popLayer);
    
    string tmpStart = "Crown_";
    if (m_tmpEquips!="") {
        int count = 4;
        for (int i=1; i<=count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
            particle->setPosition(m_par1Node->getPosition());
            addPopParticleToBatch(particle);
        }
    }
    
    auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    for (int j=0; j<eInfo.mateVec.size(); j++) {
        auto pt = ccp(0, 0);
        if (j==0) {
            pt = m_par2Node->getPosition();
        } else if (j==1) {
            pt = m_par3Node->getPosition();
        } else if (j==2) {
            pt = m_par4Node->getPosition();
        } else if (j==3) {
            pt = m_par5Node->getPosition();
        }
        int count = 4;
        for (int i=1; i<=count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
            particle->setPosition(pt);
            addPopParticleToBatch(particle);
        }
    }
    
    string tmpStartT = "CollectionIn_";
    int count = 8;
    for (int i=1; i<=count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStartT.c_str(),i)->getCString());
        particle->setPosition(m_par6Node->getPosition());
        addPopParticleToBatch(particle);
    }
    
    this->scheduleOnce(schedule_selector(EquipmentCreateView::removePopParticle), 1);
}

void EquipmentCreateView::playCrtEndParticle()
{
    removePopParticle(0);
    m_popNode->addChild(m_popLayer);
    
    string tmpStartT = "CardOut_";
    int count = 8;
    for (int i=1; i<=count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStartT.c_str(),i)->getCString());
        particle->setPosition(m_par6Node->getPosition());
        addPopParticleToBatch(particle);
    }
    
    this->scheduleOnce(schedule_selector(EquipmentCreateView::showPopParticle), 1);
}

void EquipmentCreateView::playCrtSound(float dt)
{
    SoundController::sharedSound()->playEffects(Music_Sfx_UI_blacksmith);
}

void EquipmentCreateView::playPutOnSound(float dt)
{
    SoundController::sharedSound()->playEffects(Music_Sfx_UI_putonequipment);
}

void EquipmentCreateView::onYesGoToBuild()
{
    int bid = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_WORKSHOP);
    auto world = WorldMapView::instance();
    if (world) {
        FunBuildController::getInstance()->willMoveToBuildType = FUN_BUILD_WORKSHOP;
        world->leaveWorld();
    }else {
        AutoSafeRef temp(this);
        
        PopupViewController::getInstance()->removeAllPopupView();
        auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
        if(layer) {
            if (bid>0) {
                layer->onMoveToBuildAndPlay(bid);
            }else {
                int itemId =layer->findCanBuildTile(FUN_BUILD_WORKSHOP);
                if(itemId==-1){
                    itemId =layer->findCanBuildTile(2);
                    if (itemId==-1) {
                        return;
                    }
                }
                layer->onMoveToBuildAndPlay(itemId);
            }
        }
    }
}

void EquipmentCreateView::onShowHand(float posx)
{
    int curNum = CCCommonUtils::getCurResourceByType(m_costResType);
    auto& eInfo = EquipmentController::getInstance()->EquipmentInfoMap[m_itemId];
    if (! eInfo.canCrt || curNum<m_costRes) {
        m_flyArrow->setVisible(false);
        return;
    }
    
    if (posx == 1) {
        m_handNode->getParent()->setPosition(m_par2Node->getParent()->getPosition());
        m_handNode->setPosition(m_par2Node->getPosition());
    }
    else if (posx == 2)
    {
        if (m_Type==1)
        {
            int tmpIdx = 0;
            bool bst = false;
            for (; tmpIdx<m_data.size(); tmpIdx++)
            {
                int itemMateId = atoi(m_data[tmpIdx].c_str());
                if (m_tmpMates[itemMateId] > 0) {
                    bst = true;
                    break;
                }
            }
            if (bst) {
                int tmpW = 0;
                Vec2 pt = ccp(0, 0);
                if (CCCommonUtils::isIosAndroidPad()) {
                    tmpW = 216;
                    pt = ccp(120, 200);
                }
                else {
                    tmpW = 90;
                    pt = ccp(60, 100);
                }
                
                pt = pt + ccp(tmpIdx*tmpW, 0);
                Vec2 ptW = m_selNode->convertToWorldSpace(pt);
                Vec2 ptH = m_handNode->getParent()->convertToNodeSpace(ptW);
                m_handNode->setPosition(ptH);
                m_subGuideKey++;
            }else {
                m_flyArrow->setVisible(false);
                return;
            }
        }
        else {
            m_flyArrow->setVisible(false);
            return;
        }
    }
    else if (posx == 3) {
        m_handNode->getParent()->setPosition(m_par3Node->getParent()->getPosition());
        m_handNode->setPosition(m_par3Node->getPosition());
    }
    else {
        m_flyArrow->setVisible(false);
        return;
    }
    
    m_flyArrow->stopAllActions();
    m_flyArrow->setPosition(ccp(0, 0));
    m_flyArrow->setVisible(true);
    CCActionInterval * moveBy = CCMoveBy::create(0.5,ccp(20, -30));
    CCActionInterval * moveRBy = CCMoveBy::create(0.5,ccp(-20, 30));
    CCSequence* fadeAction = CCSequence::create(moveBy,moveRBy,NULL);
    CCActionInterval * fadeForever =CCRepeatForever::create((CCActionInterval* )fadeAction);
    m_flyArrow->runAction(fadeForever);
    
//    scheduleOnce(schedule_selector(EquipmentCreateView::hideFlyArrow), 5.0f);
}
void EquipmentCreateView::hideFlyArrow(float dt)
{
    m_flyArrow->setVisible(false);
    m_flyArrow->stopAllActions();
}

//class cell
EquipmentSelCell* EquipmentSelCell::create(string itemId, int type, int num, int equipId, bool equipOn)
{
    auto ret = new EquipmentSelCell();
    if (ret && ret->init(itemId, type, num, equipId,equipOn)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipmentSelCell::init(string itemId, int type, int num, int equipId, bool equipOn)
{
    bool ret = true;
    CCLoadSprite::doResourceByCommonIndex(306, true);
    CCLoadSprite::doResourceByCommonIndex(305, true);
    CCBLoadFile("EquipmentSelCell",this,this);
    if (CCCommonUtils::isIosAndroidPad()) {
        setContentSize(CCSize(216, 204));
    }
    else
        setContentSize(CCSize(90, 85));
    setData(itemId, type, num, equipId,equipOn);
    return ret;
}

void EquipmentSelCell::setData(string itemId, int type, int num, int equipId, bool equipOn)
{
    m_itemId = itemId;
    m_type = type;
    m_num = num;
    m_equipId = equipId;
    m_picNode->removeAllChildren();
    m_numLabel->setString("");
    m_onSpr->setVisible(false);
    
    string bgStr = "";
    string iconStr = "";
    CCLoadSpriteType loadType = CCLoadSpriteType_GOODS;
    if (m_type == 0) {//装备
        int eItemId = 0;
        if (EquipmentController::getInstance()->m_MyEquipmentMap.find(itemId)!=EquipmentController::getInstance()->m_MyEquipmentMap.end()) {
            eItemId = EquipmentController::getInstance()->m_MyEquipmentMap[itemId];
        }
        if(eItemId==0){
            eItemId = EquipmentController::getInstance()->m_MyOnEquip[m_itemId];
        }
        auto& info = EquipmentController::getInstance()->EquipmentInfoMap[eItemId];
        bgStr = CCCommonUtils::getToolBgByColor(info.color);
        iconStr = CCCommonUtils::getIcon(CC_ITOA(eItemId));
        loadType = CCLoadSpriteType_EQUIP;
        string lvMsg = "Lv.";
        lvMsg = lvMsg + CC_ITOA(info.level);
        m_numLabel->setString(lvMsg);
        m_onSpr->setVisible(equipOn);
    }
    else {
        int toolId = atoi(m_itemId.c_str());
        auto toolInfo = ToolController::getInstance()->getToolInfoById(toolId);
        bgStr = CCCommonUtils::getToolBgByColor(toolInfo.color);
        iconStr = CCCommonUtils::getIcon(m_itemId);
        m_numLabel->setString(CC_ITOA(m_num));
    }
    
    auto bg = CCLoadSprite::createSprite(bgStr.c_str());
    CCCommonUtils::setSpriteMaxSize(bg, 75, true);
    m_picNode->addChild(bg);
    
    auto icon = CCLoadSprite::createSprite(iconStr.c_str(),true,loadType);
    CCCommonUtils::setSpriteMaxSize(icon, 75, true);
    m_picNode->addChild(icon);
}

void EquipmentSelCell::onEnter() {
    CCNode::onEnter();
    //    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipmentListCell::hideDesNode), MSG_HIDE_DES_NODE, NULL);
}

void EquipmentSelCell::onExit() {
    //    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_HIDE_DES_NODE);
    CCNode::onExit();
}

void EquipmentSelCell::refreashData(CCObject* obj)
{
}

SEL_CCControlHandler EquipmentSelCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickExchange", EquipmentListCell::onClickExchange);
    return NULL;
}

void EquipmentSelCell::cellTouchEnded(CCTouch* pTouch)
{
    if (m_num==0 && m_type!=0) {
        int toolId = atoi(m_itemId.c_str());
        auto toolInfo = ToolController::getInstance()->getToolInfoById(toolId);
        string name = CCCommonUtils::getNameById(m_itemId);
        if (toolInfo.color==0) {
            auto _dialog = YesNoDialog::show(_lang_1("111677", name.c_str()),
                                             CCCallFuncO::create(this, callfuncO_selector(EquipmentSelCell::gotoWorkShopView), NULL));
            _dialog->setYesButtonTitle(_lang("111679").c_str());//gotoWorkShopView
        }else{
            auto _dialog = YesNoDialog::show(_lang_1("111677", name.c_str()),
                                             CCCallFuncO::create(this, callfuncO_selector(EquipmentSelCell::gotoWorkShopView), NULL),0,
                                             true,
                                             CCCallFuncO::create(this, callfuncO_selector(EquipmentSelCell::gotoMateView), NULL));
            _dialog->showCancelButton();
            _dialog->setYesButtonTitle(_lang("111679").c_str());//gotoWorkShopView
            _dialog->setNoButtonTitle(_lang("111678").c_str());//gotoMateView
        }
        
//        if (toolInfo.color == WHITE) {
//            YesNoDialog::show(_lang_1("119074", name.c_str()), NULL);
////            CCCommonUtils::flyHint("", "", _lang_1("119074", name.c_str()));
//        }else {
//            YesNoDialog::show(_lang_1("119073", name.c_str()), NULL);
////            CCCommonUtils::flyHint("", "", _lang_1("119073", name.c_str()));
//        }
    }else {
        //是否已穿上
        map<string, int>::iterator it = EquipmentController::getInstance()->m_MyOnEquip.begin();
        for (; it!=EquipmentController::getInstance()->m_MyOnEquip.end(); it++) {
            if (it->first==m_itemId) {
                CCLOG("m_MyOnEquip");
                YesNoDialog::show(_lang("111680").c_str(), CCCallFunc::create(this, callfunc_selector(EquipmentSelCell::gotoEquipNewUseView)));//是否卸下该装备
                return;
            }
        }
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(CHANGE_MATE_SEL, CCString::create(m_itemId));
    }
}

void EquipmentSelCell::gotoEquipNewUseView(){
    int eItemId = EquipmentController::getInstance()->m_MyOnEquip[m_itemId];
    auto& curInfo = EquipmentController::getInstance()->EquipmentInfoMap[eItemId];
    PopupViewController::getInstance()->addPopupInView(EquipNewUseView::create(curInfo.site,1));
}

void EquipmentSelCell::gotoWorkShopView(CCObject* p){
    CCLOGFUNC();
    AutoSafeRef temp(this);
    
    PopupViewController::getInstance()->removeAllPopupView();
    int intId = atoi(m_itemId.c_str());
    while (true) {
        auto toolInfo = ToolController::getInstance()->getToolInfoById(intId);
        if (toolInfo.color != WHITE) {
            intId--;
        }else{
            break;
        }
        if (intId<=0) {
            break;
        }
    }
    
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
                CCDictionary *dict = CCDictionary::create();
                dict->setObject(CCInteger::create(intId), "itemId");
                dict->setObject(CCInteger::create(m_equipId), "equipId");
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("setViewByEquip",dict);
            }
        }
    }
}
void EquipmentSelCell::gotoMateView(CCObject* p){
    CCLOGFUNC();
    int intId = atoi(m_itemId.c_str());
    auto toolInfo = ToolController::getInstance()->getToolInfoById(intId);
    if (toolInfo.color != WHITE) {
        intId-=1;
    }
    PopupViewController::getInstance()->addPopupInView(MateCreateView::create(intId,0,0,0));
}

void EquipmentSelCell::hideDesNode(CCObject* param)
{
}

bool EquipmentSelCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_onSpr", CCSprite*, m_onSpr);
    return false;
}

//EquipColorCell cell
EquipColorCell* EquipColorCell::create(int color)
{
    auto ret = new EquipColorCell();
    if (ret && ret->init(color)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EquipColorCell::init(int color)
{
    bool ret = true;
    m_color = color;
    CCBLoadFile("EquipColorCell",this,this);
//    if (CCCommonUtils::isIosAndroidPad()) {
//        setContentSize(CCSize(216, 216));
//    }
//    else
        setContentSize(CCSize(103, 55));
    
    string sprName = "diamond_white.png";
    if (color == GREEN) {
        sprName = "diamond_green.png";
    }else if (color == BLUE) {
        sprName = "diamond_blue.png";
    }else if (color == PURPLE) {
        sprName = "diamond_purple.png";
    }else if (color == ORANGE) {
        sprName = "diamond_red.png";
    }else if (color == GOLDEN) {
        sprName = "diamond_yellow.png";
    }
    CCSprite* spr = CCLoadSprite::createSprite(sprName.c_str());
    spr->setOpacity(150);
    m_sprNode->removeAllChildren();
    m_sprNode->addChild(spr);
    spr->setTag(1);
    
    setData(color, 0);
    return ret;
}

void EquipColorCell::setData(int color, float num)
{
    if (m_color != color) {
        return;
    }
    m_num = num;
    string tmp = "";
    m_numLabel->setString(tmp+CC_ITOA_MONEY(m_num)+"%");
}

void EquipColorCell::setSelected()
{
    dynamic_cast<CCSprite*>(m_sprNode->getChildByTag(1))->setOpacity(255);
}
void EquipColorCell::setNotSelected()
{
    dynamic_cast<CCSprite*>(m_sprNode->getChildByTag(1))->setOpacity(150);
}

void EquipColorCell::onEnter() {
    CCNode::onEnter();
    //    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(EquipmentListCell::hideDesNode), MSG_HIDE_DES_NODE, NULL);
}

void EquipColorCell::onExit() {
    //    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_HIDE_DES_NODE);
    CCNode::onExit();
}

void EquipColorCell::refreashData(CCObject* obj)
{
}

SEL_CCControlHandler EquipColorCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickExchange", EquipmentListCell::onClickExchange);
    return NULL;
}

bool EquipColorCell::isTouchIn(CCTouch* pTouch)
{
    if (isTouchInside(m_touchNode, pTouch)) {
        return true;
    }else {
        return false;
    }
}

bool EquipColorCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprNode", CCNode*, m_sprNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    return false;
}