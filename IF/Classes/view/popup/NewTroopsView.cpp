//
//  NewTroopsView.cpp
//  IF
//
//  Created by xxrdsg on 15-8-31.
//
//

#include "NewTroopsView.h"
#include "ArmyController.h"
#include "WorldController.h"
#include "MarchFormationView.h"
#include "PopupViewController.h"
#include "FunBuildController.h"
#include "soldierIconCell.hpp"
#include "GuideController.h"
#include "SoldierInfoView.h"
#include "WorldController.h"
#include "WorldCommand.h"
#include "QueueController.h"
#include "VipUtil.h"

static string guideKey = "";
static const char* troops_roman[30] =  {"一","二","三","四","五","六","七","八","九","十","十一","十二","十三","十四","十五","十六","十七","十八","十九","二十","廿一","廿二","廿三","廿四","廿五","廿六","廿七","廿八","廿九","三十"};

NewTroopsView* NewTroopsView::create()
{
    auto ret = new NewTroopsView();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool NewTroopsView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    guideKey = "";
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByCommonIndex(200, true);
    CCLoadSprite::doResourceByCommonIndex(201, true);
    CCLoadSprite::doResourceByCommonIndex(202, true);
    CCLoadSprite::doResourceByCommonIndex(203, true);
    CCLoadSprite::doResourceByCommonIndex(204, true);
    CCLoadSprite::doResourceByCommonIndex(504, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(7, false);
        CCLoadSprite::doResourceByCommonIndex(200, false);
        CCLoadSprite::doResourceByCommonIndex(201, false);
        CCLoadSprite::doResourceByCommonIndex(202, false);
        CCLoadSprite::doResourceByCommonIndex(203, false);
        CCLoadSprite::doResourceByCommonIndex(204, false);
        CCLoadSprite::doResourceByCommonIndex(504, false);
    });
    auto size = CCDirector::sharedDirector()->getWinSize();
    
    auto tmpCCB = CCBLoadFile("NewTroopsView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    int add = size.height - tmpCCB->getContentSize().height;
    // viewBG->setPositionY(viewBG->getPositionY() - add);
    m_listNode->setPositionY(m_listNode->getPositionY() - add);
    m_listNode->setContentSize(CCSize(m_listNode->getContentSize().width, m_listNode->getContentSize().height + add));
    m_scrollView = CCScrollView::create(m_listNode->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_scrollView->setContentSize(m_listNode->getContentSize());
    m_scrollView->setAnchorPoint(ccp(0, 0));
    m_scrollView->setPosition(ccp(0, 0));
    m_scrollView->setDelegate(this);
    m_scrollView->setBounceable(false);
    m_listNode->addChild(m_scrollView);
    m_mainNode = CCNode::create();
    m_scrollView->addChild(m_mainNode);
    
    if (WorldController::getInstance()->getCurrentMarchCount() > 0)
    {
        auto & marchInfo = WorldController::getInstance()->m_marchInfo;
        for (auto &march:marchInfo) {
            if (march.second.ownerType == PlayerSelf && march.second.uuid != "") {
                auto cmd = new WorldMarchDetailUpdateCommand(march.second.uuid);
                cmd->sendAndRelease();
            }
        }
    }
    
    refreshView(nullptr);
    auto& build = FunBuildController::getInstance()->getFunbuildById(FUN_BUILD_MAIN_CITY_ID);
    if (build.level <= 6 && CCCommonUtils::isTestPlatformAndServer("Guide_TrainField")) {
        GuideController::share()->checkSubGuide("3220100");
    }
    return true;
}

void NewTroopsView::addNewDizi(CCScale9Sprite* sprite)
{
    //1.left 2.right 3.top, 4. bottom 5.增加4个角map_line_K.png  map_txt_jiao.png
    if(sprite)
    {
        //1.
        auto size = sprite->getContentSize();
        
        auto sp_1 = CCLoadSprite::createSprite("map_line_K.png");
        auto tBatchNode = CCSpriteBatchNode::createWithTexture(sp_1->getTexture());
        
        auto cellHeight = sp_1->getContentSize().height;

        int  numHeight = ceil(size.height/cellHeight);
        
        int  numWidth = ceil(size.width/cellHeight);
        //left right
        for(int i = 0 ; i < numHeight; i++)
        {
            auto bg_1 = CCLoadSprite::createSprite("map_line_K.png");
            bg_1->setAnchorPoint(ccp(0.5,0));
            bg_1->setPosition(ccp(0,i*cellHeight));
            
            auto bg_2 = CCLoadSprite::createSprite("map_line_K.png");
            bg_2->setAnchorPoint(ccp(0.5,0));
            bg_2->setPosition(ccp(size.width,i*cellHeight));
            
            tBatchNode->addChild(bg_1);
            tBatchNode->addChild(bg_2);
            if(i+1 == numHeight)
            {
                bg_1->setAnchorPoint(ccp(0.5,1));
                bg_1->setPosition(ccp(0,size.height));
                
                bg_2->setAnchorPoint(ccp(0.5,1));
                bg_2->setPosition(ccp(size.width,size.height));

            }
        }
        
        //top bottom
        for(int i = 0 ; i < numWidth; i++)
        {
            auto bg_1 = CCLoadSprite::createSprite("map_line_K.png");
            bg_1->setRotation(90);
            bg_1->setAnchorPoint(ccp(0.5,0));
            bg_1->setPosition(ccp(i*cellHeight,size.height));
            
            auto bg_2 = CCLoadSprite::createSprite("map_line_K.png");
            bg_2->setRotation(90);
            bg_2->setAnchorPoint(ccp(0.5,0));
            bg_2->setPosition(ccp(i*cellHeight,0));
            
            tBatchNode->addChild(bg_1);
            tBatchNode->addChild(bg_2);
            if(i+1 == numWidth)
            {
                bg_1->setAnchorPoint(ccp(0.5,1));
                bg_1->setPosition(ccp(size.width,size.height));
                
                bg_2->setAnchorPoint(ccp(0.5,1));
                bg_2->setPosition(ccp(size.width,0));
                
            }
        }
        sprite->addChild(tBatchNode);
        
        //jiao
        
        auto jia_1 = CCLoadSprite::createSprite("map_txt_jiao.png");
        jia_1->setAnchorPoint(ccp(0,0));
        jia_1->setPosition(ccp(0,0)+ccp(-3,-3));
        sprite->addChild(jia_1);
        
        auto jia_2 = CCLoadSprite::createSprite("map_txt_jiao.png");
        jia_2->setFlipX(true);
        jia_2->setAnchorPoint(ccp(1,0));
        jia_2->setPosition(ccp(size.width,0)+ccp(3,-3));
        sprite->addChild(jia_2);
        
        auto jia_3 = CCLoadSprite::createSprite("map_txt_jiao.png");
        jia_3->setFlipX(true);
        jia_3->setFlipY(true);
        jia_3->setAnchorPoint(ccp(1,1));
        jia_3->setPosition(ccp(size.width,size.height )+ccp(3,3));
        sprite->addChild(jia_3);
        
        auto jia_4 = CCLoadSprite::createSprite("map_txt_jiao.png");
        jia_4->setFlipY(true);
        jia_4->setAnchorPoint(ccp(0,1));
        jia_4->setPosition(ccp(0,size.height)+ccp(-3,3));
        sprite->addChild(jia_4);


        
    }
}

void NewTroopsView::refreshView(cocos2d::CCObject * obj)
{
    float dy0 = 0;
    float cury0 = m_scrollView->getContentSize().height;
    if (m_scrollView->getContentSize().height > m_listNode->getContentSize().height) {
        dy0 = m_scrollView->getContentOffset().y;
    }
    bool hasMarch = false;
    
    float dy1 = m_scrollView->getContentOffset().y;
    float minDy1 = m_scrollView->minContainerOffset().y;
    
    float troops_cell_width = 190;
    float troops_cell_height = 290;
    int cellCnt = 3;
    
    int marchCellCnt = 6;
    float march_cell_width = 120;
    float march_cell_height = 180;
    if (CCCommonUtils::isIosAndroidPad())
    {
        troops_cell_width = 1536 / 5.0;
        troops_cell_height = 552;
        cellCnt = 5;
        
        marchCellCnt = 8;
        march_cell_width = 186;
        march_cell_height = 300;
    }
    m_mainNode->removeAllChildrenWithCleanup(true);
    int cury = 0;
    
    //俩按钮
    //判断是否有编队功能按钮:15本两个,SVIP根据等级取值
    int formationCount = 0;
    if (FunBuildController::getInstance()->getMainCityLv() >= 15) {
        formationCount += 2;
    }
    auto &playerInfo = GlobalData::shared()->playerInfo;
    int svipLv = playerInfo.SVIPLevel;
    if(VipUtil::isSVIP() && svipLv > 0 && playerInfo.vipEndTime > GlobalData::shared()->getWorldTime()) {
        int itemID = 7010+svipLv-1;
        int value = atoi(CCCommonUtils::getPropById(CC_ITOA(itemID), "troop").c_str());
        if (value > 0) {
            formationCount += value;
        }
    }
    if (formationCount > 0 && GlobalData::shared()->march_formation_switch == 2 && ArmyController::getInstance()->getTotalArmyMan() > 0) {
        auto cell = NewTroopsBtn::create();
        cell->setPosition(0, cury);
        m_mainNode->addChild(cell,1);
        if (CCCommonUtils::isIosAndroidPad())
        {
            cury -= 126;
        } else {
            cury -= 55;
        }
    }
    
    //army title
    auto armyTitle = NewTroopsArmyTitle::create();
    armyTitle->setPosition(0, cury);
    m_mainNode->addChild(armyTitle,2);
    m_guideNode1 = CCNode::create();
    m_guideNode1->setAnchorPoint(ccp(0, 1));
    m_guideNode1->setPosition(0, cury);
    if (CCCommonUtils::isIosAndroidPad())
    {
        cury -= 240;
        m_guideNode1->setContentSize(Size(1536, 224));
    } else {
        cury -= 125;
        m_guideNode1->setContentSize(Size(640, 108));
    }
    m_mainNode->addChild(m_guideNode1);
    
    //army cells map
    map< int, vector<string> > armyMap;
    int id = 0;
    int type = 0;
    for (auto it = GlobalData::shared()->armyList.rbegin(); it != GlobalData::shared()->armyList.rend(); ++it)
    {
        id = atoi((it->first).c_str());
        type = id / 100;
        if (it->second.free > 0) {
            switch (type) {
                case 1070:
                    armyMap[1].push_back(it->first);
                    break;
                case 1071:
                    armyMap[2].push_back(it->first);
                    break;
                case 1072:
                    armyMap[3].push_back(it->first);
                    break;
                case 1073:
                    armyMap[4].push_back(it->first);
                    break;
                    
                default:
                    break;
            }
        }
    }
    bool cellFlag = false;
    if (ArmyController::getInstance()->getTotalFree() > 0) {
        cellFlag = true;
    }
    bool st0 = false;
    
    //底子!!
    auto size = CCDirector::sharedDirector()->getWinSize();

    

    for (auto it = armyMap.begin(); it != armyMap.end(); ++it) {
       
        st0 = true;
        for (int i = 0; i < it->second.size(); ++i) {
            auto cell = NewTroopsCell::create(it->second.at(i));
            cell->setPosition((i % cellCnt) * troops_cell_width + 50, cury - floor(i * 1.0 / cellCnt ) * troops_cell_height - 25);
            m_mainNode->addChild(cell,2);
            if (cellFlag) {
                cellFlag = false;
                m_guideNode2 = Node::create();
                m_guideNode2->setContentSize(Size(troops_cell_width, troops_cell_height));
                m_guideNode2->setAnchorPoint(ccp(0, 1));
                m_guideNode2->setPosition(cell->getPosition());
                m_mainNode->addChild(m_guideNode2);
            }
        }
        int cellbgHeight = 290;
        cellbgHeight += (ceil(it->second.size() * 1.0 / cellCnt)-1) * troops_cell_height;
        cury -= (ceil(it->second.size() * 1.0 / cellCnt)) * troops_cell_height;
        if (it->second.size() > 0)
        {
            
            if (st0) {
                
                //底子!!
                auto cellBG_1 = CCLoadSprite::createScale9Sprite("alliance_mem_di.png");
                
                
                
                
                cellBG_1->setContentSize(CCSizeMake(size.width-60, cellbgHeight));
                cellBG_1->setPreferredSize(CCSizeMake(size.width-60, cellbgHeight));
                
                cellBG_1->setAnchorPoint(ccp(0, 0));
                cellBG_1->setPosition(Vec2(30,cury-10));
                addNewDizi(cellBG_1);
                m_mainNode->addChild(cellBG_1,1);
                
                
                
                cury -= (15);
                if (CCCommonUtils::isIosAndroidPad()) {
                    cury -= 15;
                }
                
            }
        }
    }
    
    if (!st0) {
        cury -= 70;//两大类间隔一定距离
        if (CCCommonUtils::isIosAndroidPad())
        {
            cury -= 70;
        }
    } else {
        cury -= 20;
        if (CCCommonUtils::isIosAndroidPad()) {
            cury -= 20;
        }
    }
    
    //显示出征部队
    if (m_marchInfo.size() > 0) {
        cury -= 20;
        if (CCCommonUtils::isIosAndroidPad()) {
            cury -= 20;
        }
        //添加!!!底子
        //left right
        auto sp_left = CCLoadSprite::createSprite("soliger_biaoti_tiao.png");
        auto sp_right = CCLoadSprite::createSprite("soliger_biaoti_tiao.png");
        sp_right->setFlipX(true);
        sp_left->setPosition(sp_left->getContentSize().width/2+ 5, cury);
        sp_right->setPosition(sp_left->getContentSize().width + 5 + sp_left->getContentSize().width/2, cury);

        m_mainNode->addChild(sp_left);
        m_mainNode->addChild(sp_right);
        
        auto sp_left_1 = CCLoadSprite::createSprite("soliger_yun.png");
        auto sp_right_1 = CCLoadSprite::createSprite("soliger_yun.png");
        sp_left_1->setFlipX(true);
        sp_left_1->setPosition(CCDirector::sharedDirector()->getWinSize().width/4, cury);
        sp_right_1->setPosition(CCDirector::sharedDirector()->getWinSize().width/4*3, cury);
        
        m_mainNode->addChild(sp_left_1);
        m_mainNode->addChild(sp_right_1);
        
        //flow
        
        
        auto label = CCLabelIF::create();
        label->setString(_lang("103656"));
        label->setColor({255, 220, 0});
        label->setFontSize(18);
        label->setAnchorPoint(Vec2(0.5, 0.5));
        label->setDimensions(Size(0, 0));
        label->setPositionX(320);
        label->setPositionY(cury);
        m_mainNode->addChild(label,1);
        if (CCCommonUtils::isIosAndroidPad())
        {
            label->setFontSize(44);
            label->setPositionX(768);
        }
        cury -= 40;
        if (CCCommonUtils::isIosAndroidPad()) {
            cury -= 40;
        }
        
        vector<string> vec;
        int allTotalTroop = 0;
        for (auto it = m_marchInfo.begin(); it != m_marchInfo.end(); ++it)
        {
            vec.push_back(it->first);
            allTotalTroop += it->second.totalTroop;
        }
        if (allTotalTroop > 0) {
            string msg = _lang("103656");
            msg += " ";
            msg += CC_CMDITOA(allTotalTroop);
            label->setString(msg);
        }
        //排序
        for (int i = 0; i < vec.size() - 1; ++i)
        {
            for (int j = i + 1; j < vec.size(); ++j)
            {
                string s1 = vec[i];
                string s2 = vec[j];
                if (m_marchInfo[s1].startStamp > m_marchInfo[s2].startStamp) {
                    vec[i] = s2;
                    vec[j] = s1;
                }
            }
        }
        for (int i = 0; i < vec.size(); ++i)
        {
            //出征部队的title
            int totalTroop = m_marchInfo[vec[i]].totalTroop;
            NewTroopsMarchTitle* marchTitle = NewTroopsMarchTitle::create(i, totalTroop, 0, vec[i]);
            marchTitle->setPosition(0, cury);
            m_mainNode->addChild(marchTitle);
            
            //
            cury -= 70;
            if (CCCommonUtils::isIosAndroidPad())
            {
                cury -= 85;
            }
            
            if (!m_bOpen[vec[i]])
            {
                continue;
            }
            marchTitle->setRotate();
            int nowIdx = 0;
            map<string, MarchSoldierDetail> tmpMarchMap;
            for (auto it = m_marchInfo[vec[i]].soldiers.begin(); it != m_marchInfo[vec[i]].soldiers.end(); ++it)
            {
                for (auto it1 = it->second.begin(); it1 != it->second.end(); ++it1)
                {
                    tmpMarchMap[(*it1).armyId] = (*it1);
                }
            }
            armyMap.clear();
            for (auto it = tmpMarchMap.rbegin(); it != tmpMarchMap.rend(); ++it)
            {
                int id = atoi(it->first.c_str());
                int type = id / 100;
                if (it->second.count > 0) {
                    switch (type) {
                        case 1070:
                            armyMap[1].push_back(it->first);
                            break;
                        case 1071:
                            armyMap[2].push_back(it->first);
                            break;
                        case 1072:
                            armyMap[3].push_back(it->first);
                            break;
                        case 1073:
                            armyMap[4].push_back(it->first);
                            break;
                        default:
                            break;
                    }
                }
            }
            vector<string> vec1;
            for (auto it = armyMap.begin(); it != armyMap.end(); ++it) {
                for (auto it1 = it->second.begin(); it1 != it->second.end(); ++it1) {
                    vec1.push_back(*it1);
                }
            }
            for (int i = 0; i < vec1.size(); ++i) {
                float addw = 20;
                if (CCCommonUtils::isIosAndroidPad())
                {
                    addw = 20;
                }
                auto cell = NewTroopsMarchCell::create(vec1.at(i), tmpMarchMap[vec1.at(i)].count);
                cell->setPosition((i % marchCellCnt) * march_cell_width + addw, cury - floor(i * 1.0 / marchCellCnt ) * march_cell_height);
                m_mainNode->addChild(cell);
            }
            cury -= (ceil(vec1.size() * 1.0 / marchCellCnt)) * march_cell_height;
            auto sc9 = CCLoadSprite::createScale9Sprite("alliance_mem_di.png");
            sc9->setAnchorPoint(Vec2(0, 0));
            float www1 = size.width-40;
            float hhh1 = 15;
            if (CCCommonUtils::isIosAndroidPad()) {
                www1 = size.width-40;
                hhh1 = 30;
            }
            sc9->setPreferredSize(Size(www1, (ceil(vec1.size() * 1.0 / marchCellCnt)) * march_cell_height + hhh1));
            sc9->setContentSize(Size(www1, (ceil(vec1.size() * 1.0 / marchCellCnt)) * march_cell_height + hhh1));
            sc9->setPositionY(cury+20);
            sc9->setPositionX(20);
            if (CCCommonUtils::isIosAndroidPad())
            {
                sc9->setPositionX(20);
            }
            sc9->setZOrder(-1);
            addNewDizi(sc9);
            m_mainNode->addChild(sc9);
            if (CCCommonUtils::isIosAndroidPad())
            {
                cury -= 10;
            }
        }
        
        cury -= 180;
        if (CCCommonUtils::isIosAndroidPad())
        {
            cury -= 300;
        }
    }
    
    
    //上下边界固定不可拖动
    if (abs(cury) < m_listNode->getContentSize().height)
    {
        cury -= (m_listNode->getContentSize().height - abs(cury));
    }
    
    m_scrollView->setContentSize(CCSize(m_listNode->getContentSize().width, abs(cury)));
    m_mainNode->setPosition(0, abs(cury));
    
    float minDy2 = m_scrollView->minContainerOffset().y;
    float addDy = minDy2 - minDy1;
    m_scrollView->setContentOffset(CCPoint(0, dy1 + addDy));

}

void NewTroopsView::scrollViewDidScroll(CCScrollView *view)
{
//    float mindy = m_scrollView->minContainerOffset().y ;
//    float maxdy = m_scrollView->maxContainerOffset().y ;
//    float dy = m_scrollView->getContentOffset().y;
//    if (dy < mindy)
//    {
//        m_scrollView->setContentOffset(ccp(0, mindy));
//    }
//    if (dy > maxdy) {
//        m_scrollView->setContentOffset(ccp(0, maxdy));
//    }
}

bool NewTroopsView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_guideNode1, pTouch)) {
        return true;
    }
    if (isTouchInside(m_guideNode2, pTouch)) {
        return true;
    }
    return false;
}

void NewTroopsView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (guideKey == "TrainField_1" && m_guideNode1) {
        if(isTouchInside(m_guideNode1, pTouch)) {
            guideKey = "";
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::create("TrainField_1"));
            return;
        }
    }
    if (guideKey == "TrainField_2" && m_guideNode2) {
        if(isTouchInside(m_guideNode2, pTouch)) {
            guideKey = "";
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::create("TrainField_2"));
            return;
        }
    }
}

void NewTroopsView::onEnter()
{
    CCNode::onEnter();
    setTouchEnabled(true);
//    setSwallowsTouches(false);
    setTitleName(_lang("108724"));
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(NewTroopsView::onGetMsgMarchData), GET_MARCH_DETAIL_UPDATE_COMMAND, nullptr);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(NewTroopsView::onGetMsgMarchTitleTouch), MSG_OUT_TRAP_TITLE_TOUCH, nullptr);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(NewTroopsView::refreshView), MSG_TROOP_NUMS_REFRESH, nullptr);
    refreshView(nullptr);
}

void NewTroopsView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GET_MARCH_DETAIL_UPDATE_COMMAND);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_OUT_TRAP_TITLE_TOUCH);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TROOP_NUMS_REFRESH);
    CCNode::onExit();
}

void NewTroopsView::onGetMsgMarchData(Ref* ref)
{
    if (!ref) {
        return;
    }
    auto dict = dynamic_cast<__Dictionary*>(ref);
    if (!dict) {
        return;
    }
    MarchDetailInfo t_info;
    t_info.setInfo(dict);
    if (!t_info.uuid.empty())
    {
        m_marchInfo[t_info.uuid] = t_info;
        m_bOpen[t_info.uuid] = false;
    }
    refreshView(nullptr);
}

void NewTroopsView::onGetMsgMarchTitleTouch(Ref* ref)
{
    if (!ref)
    {
        return;
    }
    __String* cs = dynamic_cast<__String*>(ref);
    if (!cs)
    {
        return;
    }
    string uuid = cs->getCString();
    if (uuid.empty())
    {
        return;
    }
    if (m_bOpen.find(uuid) == m_bOpen.end())
    {
        return;
    }
    m_bOpen[uuid] = !m_bOpen[uuid];
    refreshView(nullptr);
}

SEL_CCControlHandler NewTroopsView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return nullptr;
}
bool NewTroopsView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, this->m_listNode);
    return false;
}
Node* NewTroopsView::getGuideNode(string _key)
{
    guideKey = _key;
    if ( _key == "TrainField_1" && m_guideNode1 ) {
        return m_guideNode1;
    }
    if ( _key == "TrainField_2" && m_guideNode2) {
        return m_guideNode2;
    }
    return NULL;
}
////////-----------class NewTroopsCell
NewTroopsCell* NewTroopsCell::create(string identity, int cnt)
{
    auto ret = new NewTroopsCell(identity, cnt);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool NewTroopsCell::init()
{
    if (!CCNode::init()) {
        return false;
    }
    CCBLoadFile("NewTroopsCell", this, this);
    
    m_iconNode->removeAllChildrenWithCleanup(true);
//    m_levelLabel->setColor({160, 177, 200});
//    m_numLabel->setColor({160, 177, 200});
    
    
    int id = atoi(m_id.c_str());
    if (id >= 107000 && id <107400)
    {
        if (GlobalData::shared()->armyList.find(m_id) != GlobalData::shared()->armyList.end()) {
            auto& info = GlobalData::shared()->armyList[m_id];
//            auto icon = CCLoadSprite::createSprite((info.getBodyIcon()).c_str());
//            CCCommonUtils::setSpriteMaxSize(icon, 222, true);
//            if (id >= 107300)
//            {
//                CCCommonUtils::setSpriteMaxSize(icon, 200, true);
//            }
            SoldierIconCell* icon = nullptr;
            int star = ArmyController::getInstance()->getStarlvById(m_id);
            if (id >= 107300) {
                icon = SoldierIconCell::create(info.getBodyIcon().c_str(), 190,m_id,true,star,m_iconGrey);
            }else{
                icon = SoldierIconCell::create(info.getBodyIcon().c_str(), 200,m_id,true,star,m_iconGrey);
            }
            m_iconNode->addChild(icon);
            m_numLabel->setString(CC_CMDITOA(info.free));
            m_levelLabel->setString(troops_roman[info.armyLevel]);
        }
    } else {
        if (GlobalData::shared()->fortList.find(m_id) != GlobalData::shared()->fortList.end()) {
            auto& info = GlobalData::shared()->fortList[m_id];
            auto icon = CCLoadSprite::createSprite((info.getBodyIcon()).c_str());
            CCCommonUtils::setSpriteMaxSize(icon, 170, true);
            m_iconNode->addChild(icon);
            m_numLabel->setString(CC_CMDITOA(info.free));
            int level = id % 10;
            m_levelLabel->setString(troops_roman[level]);
        }
    }
    float w = m_numLabel->getContentSize().width * m_numLabel->getOriginScaleX();
    float curscale = m_numLabel->getScale();
    float maxw = 100.0;
    if (CCCommonUtils::isIosAndroidPad()) {
        maxw = 200.0;
    }
    if (w > maxw) {
        float scale = maxw / w * curscale;
        m_numLabel->setScale(scale);
    }
    
    return true;
}

void NewTroopsCell::onEnter()
{
    CCNode::onEnter();
    setSwallowsTouches(false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
}

void NewTroopsCell::onExit()
{
    CCNode::onExit();
}

bool NewTroopsCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, this->m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelLabel", CCLabelIF*, this->m_levelLabel);
    return false;
}

bool NewTroopsCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (isTouchInside(m_touchNode, pTouch) && guideKey == "") {
        return true;
    }
    return false;
}
void NewTroopsCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    float dis = ccpDistance(pTouch->getStartLocation(), pTouch->getLocation());
    if (dis > 10) {
        return;
    }
     if(isTouchInside(m_touchNode, pTouch)) {
         auto m_info = &GlobalData::shared()->armyList[m_id];
         if (m_info) {
             string buildId =m_info->getBuildId();
             if (buildId != "") {
                 int buildingId = std::atoi(buildId.c_str());
                 auto dictInfo = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(buildingId));
                 auto tmpBuild = FunBuildInfo(dictInfo);
                 string open_arms = tmpBuild.open_arms;
                 if(open_arms != ""){
                     int bid = FunBuildController::getInstance()->getMaxLvBuildByType( buildingId );
                     vector<string> m_armyIds = ArmyController::getInstance()->getCreateSoldierIds(open_arms, false);
                     auto view = SoldierMoreInfoView::create(m_info,bid,m_armyIds);
                     if (view) {
                         view->setCleanFunction([](){
                             CCLoadSprite::doResourceByCommonIndex(502, false);
                             CCLoadSprite::doResourceByCommonIndex(510, false);
                             CCLoadSprite::doResourceByCommonIndex(4, false);
                         });
                     }
                     PopupViewController::getInstance()->addPopupView(view);
                     return;
                 }
             }
         }
    }
}

//////------------class NewTroopsBtn
bool NewTroopsBtn::init()
{
    CCNode::init();
    CCBLoadFile("NewTroopsBtn", this, this);
    
    
    m_formationLab1->setString("1");
    m_formationLab2->setString("2");
    m_formationLab3->setString("3");
    m_formationLab4->setString("4");
    m_formationLab5->setString("5");
    //判断是否有编队功能按钮:15本两个,SVIP根据等级取值
    int formationCount = 0;
    if (FunBuildController::getInstance()->getMainCityLv() >= 15) {
        formationCount += 2;
    }
    auto &playerInfo = GlobalData::shared()->playerInfo;
    int svipLv = playerInfo.SVIPLevel;
    if(VipUtil::isSVIP() && svipLv > 0 && playerInfo.vipEndTime > GlobalData::shared()->getWorldTime()) {
        int itemID = 7010+svipLv-1;
        int value = atoi(CCCommonUtils::getPropById(CC_ITOA(itemID), "troop").c_str());
        if (value > 0) {
            formationCount += value;
        }
    }
    vector<CCSafeObject<Node>> formationVec;
    formationVec.clear();
    formationVec.push_back(m_formation1);
    formationVec.push_back(m_formation2);
    formationVec.push_back(m_formation3);
    formationVec.push_back(m_formation4);
    formationVec.push_back(m_formation5);
    if (formationCount > 0 && GlobalData::shared()->march_formation_switch == 2) {
        m_formationNode->setVisible(true);
        m_formation1->setVisible(false);
        m_formation2->setVisible(false);
        m_formation3->setVisible(false);
        m_formation4->setVisible(false);
        m_formation5->setVisible(false);
        for(int i = 0; i < formationCount; i++) {
            formationVec[i]->setVisible(true);
            formationVec[i]->setPositionX((i - formationCount + 1) * 60);
        }
    }
    else {
        m_formationNode->setVisible(false);
    }
    return true;
}

void NewTroopsBtn::onFormation1Click(CCObject *pSender, CCControlEvent event)
{
    auto view = MarchFormationView::create(1);
    if (view) {
        view->setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(8, false);
        });
    }
    PopupViewController::getInstance()->addPopupInView(view);
}

void NewTroopsBtn::onFormation2Click(CCObject *pSender, CCControlEvent event)
{
    auto view = MarchFormationView::create(2);
    if (view)
    {
        view->setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(8, false);
        });
    }
    PopupViewController::getInstance()->addPopupInView(view);
}

void NewTroopsBtn::onFormation3Click(CCObject *pSender, CCControlEvent event)
{
    auto view = MarchFormationView::create(3);
    if (view)
    {
        view->setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(8, false);
        });
    }
    PopupViewController::getInstance()->addPopupInView(view);
}

void NewTroopsBtn::onFormation4Click(CCObject *pSender, CCControlEvent event)
{
    auto view = MarchFormationView::create(4);
    if (view)
    {
        view->setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(8, false);
        });
    }
    PopupViewController::getInstance()->addPopupInView(view);
}

void NewTroopsBtn::onFormation5Click(CCObject *pSender, CCControlEvent event)
{
    auto view = MarchFormationView::create(5);
    if (view)
    {
        view->setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(8, false);
        });
    }
    PopupViewController::getInstance()->addPopupInView(view);
}

SEL_CCControlHandler NewTroopsBtn::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFormation1Click", NewTroopsBtn::onFormation1Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFormation2Click", NewTroopsBtn::onFormation2Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFormation3Click", NewTroopsBtn::onFormation3Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFormation4Click", NewTroopsBtn::onFormation4Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFormation5Click", NewTroopsBtn::onFormation5Click);
    return nullptr;
}

bool NewTroopsBtn::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formationNode", Node*, m_formationNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formation5", Node*, m_formation5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formation4", Node*, m_formation4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formation3", Node*, m_formation3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formation2", Node*, m_formation2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formation1", Node*, m_formation1);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formationLab5", CCLabelIF*, m_formationLab5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formationLab4", CCLabelIF*, m_formationLab4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formationLab3", CCLabelIF*, m_formationLab3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formationLab2", CCLabelIF*, m_formationLab2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formationLab1", CCLabelIF*, m_formationLab1);
    return false;
}


/////------------class NewTroopsArmyTitle
NewTroopsArmyTitle* NewTroopsArmyTitle::create()
{
    auto ret = new NewTroopsArmyTitle();
    if (ret && ret->init())
    {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool NewTroopsArmyTitle::init()
{
    CCNode::init();
    CCBLoadFile("NewTroopsArmyTitle", this, this);
    
    float dw = 10;
    if (CCCommonUtils::isIosAndroidPad()) {
        dw = 20;
    }
    m_armyLabel->setString(_lang("102149"));
//    m_armyLabel->setColor({127, 127, 127});
    m_armyNum->setString(CC_CMDITOA(ArmyController::getInstance()->getTotalArmyMan()));
    m_armyNum->setPositionX(m_armyLabel->getPositionX() + m_armyLabel->getContentSize().width * m_armyLabel->getOriginScaleX() + dw);
//    m_armyNum->setColor({160, 177, 202});
    
    m_marchLabel->setString(_lang("102141"));
    int maxMarchNum = WorldController::getInstance()->getMaxMarchCount();
    int nowMarchNum = maxMarchNum - WorldController::getInstance()->getCurrentMarchCount();
    nowMarchNum = max(0, nowMarchNum);
    string msg = CC_ITOA(nowMarchNum);
    msg += "/";
    msg += CC_ITOA(maxMarchNum);
    m_marchNum->setString(msg);
    m_marchNum->setPositionX(m_marchLabel->getPositionX() + m_marchLabel->getContentSize().width * m_marchLabel->getOriginScaleX() + dw);
//    m_marchLabel->setColor({127, 127, 127});
//    m_marchNum->setColor({127, 127, 127});
    
    m_foodSpdLabel->setString(_lang("102125"));
    int upKeep = ArmyController::getInstance()->getTotalUpKeep();
    msg = CC_CMDITOA(upKeep);
    msg += "/h";
    m_foodSpd->setString(msg);
    m_foodSpd->setPositionX(m_foodSpdLabel->getPositionX() + m_foodSpdLabel->getContentSize().width * m_foodSpdLabel->getOriginScaleX() + dw);
//    m_foodSpdLabel->setColor({127, 127, 127});
//    m_foodSpd->setColor({127, 127, 127});
    
    m_woundedLabel->setString(_lang("102186"));
    int deadNum = 0;
    for(auto Tit = GlobalData::shared()->treatList.begin(); Tit != GlobalData::shared()->treatList.end(); Tit++){
        deadNum += Tit->second.dead;
    }
    msg = CC_CMDITOA(deadNum) + "/" + CC_CMDITOA(ArmyController::getInstance()->getMaxNumByType(TREAT_ARMY));
    m_woundedNum->setString(msg);
    m_woundedNum->setPositionX(m_woundedLabel->getPositionX() + m_woundedLabel->getContentSize().width * m_woundedLabel->getOriginScaleX() + dw);
//    m_woundedLabel->setColor({127, 127, 127});
//    m_woundedNum->setColor({127, 127, 127});
    
    if (ArmyController::getInstance()->getTotalFree() > 0)
    {
        string ms = _lang("103694");
        ms += " ";
        ms += CC_CMDITOA(ArmyController::getInstance()->getTotalFree());
        m_myTroopLabel->setString(ms);
    } else {
        m_myTroopLabel->setString(_lang("103695"));
    }
    
//    m_myTroopLabel->setColor({72, 60, 44});
    
    
    return true;
}

SEL_CCControlHandler NewTroopsArmyTitle::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return nullptr;
}

bool NewTroopsArmyTitle::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_armyLabel", CCLabelIF*, this->m_armyLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_armyNum", CCLabelIF*, this->m_armyNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_marchLabel", CCLabelIF*, this->m_marchLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_marchNum", CCLabelIF*, this->m_marchNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woundedLabel", CCLabelIF*, this->m_woundedLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woundedNum", CCLabelIF*, this->m_woundedNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodSpdLabel", CCLabelIF*, this->m_foodSpdLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodSpd", CCLabelIF*, this->m_foodSpd);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_myTroopLabel", CCLabelIF*, this->m_myTroopLabel);
    return false;
}

/////-----------class NewTroopsTrapTitle
NewTroopsTrapTitle* NewTroopsTrapTitle::create()
{
    auto ret = new NewTroopsTrapTitle();
    if (ret && ret->init())
    {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool NewTroopsTrapTitle::init()
{
    CCNode::init();
    CCBLoadFile("NewTroopsTrapTitle", this, this);
    m_trapLabel->setString(_lang("102185"));
    //m_trapLabel->enableStroke(ccBLACK, 1);  //增加描边
    //m_trapNum->enableStroke(ccBLACK, 1);  //增加描边
    
    string msg = CC_CMDITOA(ArmyController::getInstance()->getTotalFortNum()) +"/"+ CC_CMDITOA( ArmyController::getInstance()->getMaxNumByType(FORT));
    m_trapNum->setString(msg);
    float dw = 10;
    if (CCCommonUtils::isIosAndroidPad()) {
        dw = 20;
    }
    m_trapNum->setPositionX(m_trapLabel->getPositionX() + m_trapLabel->getContentSize().width * m_trapLabel->getOriginScaleX() + dw);
//    m_trapLabel->setColor({127, 127, 127});
//    m_trapNum->setColor({127, 127, 127});
//    m_tipLabel->setColor({72, 60, 44});
    if (ArmyController::getInstance()->getTotalFortNum() <= 0)
    {
        m_tipLabel->setString(_lang("103696"));
    } else {
        m_tipLabel->setString("");
    }
    return true;
}

bool NewTroopsTrapTitle::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipLabel", CCLabelIF*, this->m_tipLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_trapLabel", CCLabelIF*, this->m_trapLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_trapNum", CCLabelIF*, this->m_trapNum);
    return false;
}

/////--------class NewTroopsMarchTitle------
bool NewTroopsMarchTitle::init()
{
    CCBLoadFile("NewTroopsMarchTitle", this, this, true);
//    m_trapLabel->setColor({127, 127, 127});
//    m_trapNum->setColor({127, 127, 127});
    m_trapLabel->setString(_lang_1("103657", CC_ITOA(m_idx + 1)));
    m_trapNum->setString(CC_CMDITOA(m_num));
    m_stateLabel->setString("");
    m_stateLabel->enableStroke(ccc3(238,212,183), 1);  //增加描边
    m_trapLabel->enableStroke(ccc3(238,212,183), 1);  //增加描边
    m_trapNum->enableStroke(ccc3(238,212,183), 1);  //增加描边
    int queueType = -1;
    for (auto it = GlobalData::shared()->allQueuesInfo.begin(); it != GlobalData::shared()->allQueuesInfo.end(); ++it) {
        if (it->second.uuid == m_marchUid)
        {
            queueType = it->second.type;
            break;
        }
    }
    string temp("");
    switch (queueType) {
        case TYPE_MARCH:
            temp = _lang("115351");
            break;
            
        case TYPE_OCCUPY_RESOURCE:
            temp = _lang("108735");
            break;
            
        case TYPE_OCCUPY_MAZE:
            temp = _lang("108736");
            break;
            
        case TYPE_OCCUPY_CAMP:
            temp = _lang("108737");
            break;
            
        case TYPE_ARMY_MASS:
            temp = _lang("115136");
            break;
            
        case TYPE_OCCUPY_ALLIANCE:
        case TYPE_STATION_TERRITORY:
            temp = _lang("115346");
            break;
            
        case TYPE_DESTROY_TERRITORY:
            temp = _lang("115347");
            break;
            
        case TYPE_BUILDING_TERRITORY:
            temp = _lang("115305");
            break;
            
        case TYPE_REPAIR_TERRITORY:
            temp = _lang("115305");
            break;
            
        case TYPE_OCCUPY_TERRITORY_RESOURCE://采集超级矿
            temp = _lang("108735");
            break;
            
        default:
            temp = _lang("115351");
            break;
    }
    // 注释掉 不显示部队状态信息
//    m_stateLabel->setString(temp);
    return true;
}

void NewTroopsMarchTitle::onEnter()
{
    Node::onEnter();
    setTouchEnabled(true);
    setSwallowsTouches(false);
}

void NewTroopsMarchTitle::onExit()
{
    Node::onExit();
}

bool NewTroopsMarchTitle::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    if (isTouchInside(m_touchNode, pTouch)) {
        return true;
    }
    return false;
}

void NewTroopsMarchTitle::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    float dis = ccpDistance(pTouch->getStartLocation(), pTouch->getLocation());
    if (dis > 10) {
        return;
    }
    retain();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_OUT_TRAP_TITLE_TOUCH, __String::create(m_marchUid));
    release();
}

bool NewTroopsMarchTitle::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_trapLabel", CCLabelIF*, m_trapLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_trapNum", CCLabelIF*, m_trapNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stateLabel", CCLabelIF*, m_stateLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", Node*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrow", Sprite*, m_arrow);
    return false;
}

void NewTroopsMarchTitle::setRotate()
{
     m_arrow->setDisplayFrame(CCLoadSprite::loadResource("Alliance_arrow_2.png"));
}

//---------class NewTroopsMarchCell----
bool NewTroopsMarchCell::init()
{
    CCLoadSprite::doResourceByCommonIndex(204, true);
    CCBLoadFile("NewTroopsMarchCell", this, this);
    m_iconNode->removeAllChildren();
    auto& info = GlobalData::shared()->armyList[m_armyId];
    auto icon = CCLoadSprite::createSprite(info.getHeadIcon().c_str());
    m_iconNode->addChild(icon);
//    m_numLabel->setColor({160, 177, 200});
    m_numLabel->setString(CC_CMDITOA(m_num));
//    m_levelLabel->setColor({160, 177, 200});
    m_levelLabel->setString(troops_roman[info.armyLevel]);
    return true;
}

bool NewTroopsMarchCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", Node*, m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelLabel", CCLabelIF*, m_levelLabel);
    return false;
}

