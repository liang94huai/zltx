//
//  MinimapUIView.cpp
//  IF
//
//  Created by 刘田厚 on 16-10-31.
//
//




#include "MinimapUIView.h"
#include "SoundController.h"
#include "PopupViewController.h"
#include "MinimapViewCCB.h"
#include "WorldMapView.h"
#include "MinimapUIView.h"
#include "WorldController.h"
#include "MiniMapController.h"
#include "MinimapLocalView.h"
#include "MinimapUIAreaView.h"
#include "ClippingNodeExt.h"

MinimapUIView::MinimapUIView():m_editOpen(false),m_TopOpen(false),m_BottomOpen(false),m_TarPos(0,0),m_iServerID(0),m_MarkType(MarkType::MARK_ALLIANCE),m_TableOpen(false),m_touchTable(false),m_tableIsMove(false),m_iMapIndex(0),m_iSelectIndex(-1),m_startMovePos(0,0),m_startZoneMovePos(0,0),m_mapInfoType(MapInfoType::SimType){}
MinimapUIView::~MinimapUIView(){
}


MinimapUIView *MinimapUIView::create(int iServerID,int iMapIndex,PopupBaseView* view)
{
    MinimapUIView *ret = new MinimapUIView();
    if(ret && ret->init(iServerID,iMapIndex,view)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool MinimapUIView::init(int iServerID,int iMapIndex,PopupBaseView* view)
{
    bool ret = false;
    if(CCLayer::init()){
       
        m_iServerID = iServerID;
        m_iMapIndex = iMapIndex;
        ret = true;
    }
    
    //just do
    
    this->m_ccbView = view;

    CCBLoadFile("WorldMinView",this,this,true);
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    
    m_topNode->setPositionY(m_topNode->getPositionY() + winSize.height - DEF_HEIGHT);
    
    m_juanzhouSpr->setRotation(30.0);
    if(m_iMapIndex == 0)
    {
        m_areaNode->setVisible(false);
    }else{
        m_areaNode->setVisible(false);
    }
    auto clipeNode = CCScrollView::create(m_clipNode->getContentSize());
    clipeNode->setTouchEnabled(false);
    m_clipNode->addChild(clipeNode);
    m_biaojiNode->removeFromParent();
    clipeNode->addChild(m_biaojiNode);
    
    m_biaojiNode_2->removeFromParent();
    clipeNode->addChild(m_biaojiNode_2);
    
    initEditBox();
    initLabel();
    addTableView();
    changeUIForMap();
    showSelfPower();
    return true;
}

void MinimapUIView::initEditBox()
{
    auto editSize = m_XeditNode->getContentSize()-CCSize(20,0);
    auto editpic_01 = CCLoadSprite::createScale9Sprite("btn_bg_op.png");
    editpic_01->setContentSize(editSize);
    editpic_01->setInsetBottom(1);
    editpic_01->setInsetTop(1);
    editpic_01->setInsetRight(1);
    editpic_01->setInsetLeft(1);
    m_editXBox = CCEditBox::create(editSize,editpic_01);
    m_editXBox->setFontColor(ccc3(0,0,0));
    m_editXBox->setInputMode(kEditBoxInputModeNumeric);
    m_editXBox->setText("0");
    m_editXBox->setDelegate(this);
    // m_editBox->setTouchPriority(Touch_Default);
    m_editXBox->setReturnType(kKeyboardReturnTypeDone);
    m_editXBox->setPosition(ccp(editSize.width/2, editSize.height/2));
    m_XeditNode->addChild(m_editXBox);
    
    auto editpic_02 = CCLoadSprite::createScale9Sprite("btn_bg_op.png");
    editpic_02->setContentSize(editSize);
    editpic_02->setInsetBottom(1);
    editpic_02->setInsetTop(1);
    editpic_02->setInsetRight(1);
    editpic_02->setInsetLeft(1);
    m_editYBox = CCEditBox::create(editSize,editpic_02);
    m_editYBox->setFontColor(ccc3(0,0,0));
    m_editYBox->setInputMode(kEditBoxInputModeNumeric);
    m_editYBox->setText("0");
    m_editYBox->setDelegate(this);
    // m_editBox->setTouchPriority(Touch_Default);
    m_editYBox->setReturnType(kKeyboardReturnTypeDone);
    m_editYBox->setPosition(ccp(editSize.width/2, editSize.height/2));
    m_YeditNode->addChild(m_editYBox);

}

void MinimapUIView::initLabel()
{
    m_jumpLab->setFntFile("Arial_Bold_Border.fnt");
    m_jumpLab->setString(_lang("new100003").c_str());
    m_allianceLab->setFntFile("Arial_Bold_Border.fnt");
    m_allianceLab->setString(_lang("102161").c_str());
    m_cityLab->setFntFile("Arial_Bold_Border.fnt");
    m_cityLab->setString(_lang("new100005").c_str());

    m_checkLab->setFntFile("Arial_Bold_Border.fnt");
    m_checkLab->setString(_lang("new100006").c_str());
    m_powerLab->setFntFile("Arial_Bold_Border.fnt");
    m_powerLab->setString(_lang("new100007").c_str());
    m_MyResLab->setFntFile("Arial_Bold_Border.fnt");
   // m_MyResLab->setString(_lang("new100006").c_str());
    m_MyResLab->setString("我的领地");
    m_tileLab->setFntFile("Arial_Bold_Border.fnt");
    m_noInfo->setFntFile("Arial_Bold_Border.fnt");
}

void MinimapUIView::addTableView()
{
    
    m_tabView = CCTableView::create(this, m_listinfo->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_tabView->setDelegate(this);
    m_listinfo->addChild(m_tabView);
}

void MinimapUIView::updateTableView()
{
    switch (m_MarkType) {
        case MARK_ALLIANCE:
            m_tileLab->setString(_lang("new100008").c_str());
            if(m_iMapIndex != 0)
            {
                if(MiniMapController::getInstance()->getAllianceMapSize(m_iMapIndex) == 0)
                {
                    m_noInfo->setVisible(true);
                    m_noInfo->setString(_lang("new100009").c_str());
                }else{
                    m_noInfo->setVisible(false);
                }
            }else {
                if(MiniMapController::getInstance()->getAllianceInfoSize() == 0)
                {
                    m_noInfo->setVisible(true);
                    m_noInfo->setString(_lang("new100009").c_str());
                }else{
                    m_noInfo->setVisible(false);
                }
            }
           
            ;
            break;
        case MARK_CITY:
            m_noInfo->setVisible(false);
            m_tileLab->setString(_lang("new100010").c_str());
            ;
            break;
            
        case MARK_FAVOURATE:
            m_tileLab->setString(_lang("new100011").c_str());
           // 我的标记
            //WorldController::getInstance()->m_favoriteTiles;
            if(WorldController::getInstance()->m_favoriteTiles.size() == 0)
            {
                m_noInfo->setVisible(true);
                m_noInfo->setString(_lang("new100012").c_str());
            }else{
                m_noInfo->setVisible(false);
            }
            ;
            break;
        case MARK_POWER:
            m_tileLab->setString(_lang("new100013").c_str());
            if(MiniMapController::getInstance()->getPowerInfoSize() == 0)
            {
                m_noInfo->setVisible(true);
                m_noInfo->setString(_lang("new100004").c_str());
            }else{
                m_noInfo->setVisible(false);
            }
            ;
            break;
        default:
            break;
    }
    m_iSelectIndex = -1;
   
    m_tabView->reloadData();
    if(!m_TableOpen)
    {
        showTableView();
    }
}

void MinimapUIView::showTop()
{
    if(m_TopOpen)
    {
        return;
    }
    m_TopOpen = true;
    
    //to do
  //  m_serachNode->setVisible(true);
    int cur_X = -620;
    int tar_X = 0;
    auto moveto = CCMoveTo::create(0.2, Vec2(tar_X, 0));
    m_serachNode->runAction(moveto);
}

void MinimapUIView::fadeTop()
{
    m_TopOpen = false;
    
    //to do
    //m_serachNode->setVisible(false);
    int cur_X = 0;
    int tar_X = -620;
    auto moveto = CCMoveTo::create(0.2, Vec2(tar_X, 0));
    m_serachNode->runAction(moveto);
}

void MinimapUIView::showBotoom()
{
    m_BottomOpen = true;
    
    //to do
    //m_biaojiNode->setVisible(true);
    if(m_iMapIndex != 0)
        
    {
        int cur_X = 582;
        int tar_X = 235;
        auto moveto = CCMoveTo::create(0.2, Vec2(tar_X, 55));
        m_biaojiNode_2->runAction(moveto);
        m_juanzhouSpr->setRotation(0);
    
    }else {
        int cur_X = 582;
        int tar_X = 88+110;
        auto moveto = CCMoveTo::create(0.2, Vec2(tar_X, 55));
        m_biaojiNode->runAction(moveto);
        m_juanzhouSpr->setRotation(0);
    }
}
void MinimapUIView::fadeBotoom()
{
    m_BottomOpen = false;
    
    if(m_TableOpen )
    {
        fadeTableView();
    }
    //to do
    
    //m_biaojiNode->setVisible(false);
    //m_biaojiNode->setVisible(true);
    if(m_iMapIndex != 0)
        
    {
        int cur_X = 235;
        int tar_X = 582;
        auto moveto = CCMoveTo::create(0.2, Vec2(tar_X, 55));
        m_biaojiNode_2->runAction(moveto);
        m_juanzhouSpr->setRotation(30.0);
        
    }else {
        int cur_X = 88;
        int tar_X = 676;
        auto moveto = CCMoveTo::create(0.2, Vec2(tar_X, 55));
        m_biaojiNode->runAction(moveto);
        m_juanzhouSpr->setRotation(30.0);
    }
}

void MinimapUIView::showTableView()
{
    m_TableOpen = true;
    m_jianxiangSpr->setVisible(false);
    
    int cur_Y = -300;
    int tar_Y = m_zoneNode->getPositionY();;
    auto moveto = CCMoveTo::create(0.2, Vec2(150, tar_Y));
    m_zoneNode->runAction(moveto);
    
}

void MinimapUIView::fadeTableView()
{
    m_TableOpen = false;
    if(m_iMapIndex == 0)
    {
        m_jianxiangSpr->setVisible(true);
    }
    
   
    int cur_Y = 0;
    int tar_Y = m_zoneNode->getPositionY();
    auto moveto = CCMoveTo::create(0.2, Vec2(658, tar_Y));
    m_zoneNode->runAction(moveto);
    
    showSelfPower();

}

void MinimapUIView::showSelfPower()
{
    //显示自己的联盟势力
    PopupBaseView* pView = m_ccbView;
    if(MiniMapController::getInstance()->getSelfPowerInfo())
    {
        if(dynamic_cast<MinimapViewCCB*>(pView))
        {
            dynamic_cast<MinimapViewCCB*>(pView)->setRegionEffects(MiniMapController::getInstance()->getSelfPowerInfo()->m_cityVector);
        }
    }else{
        std::vector<int> noArray;
        if(dynamic_cast<MinimapViewCCB*>(pView))
        {
            dynamic_cast<MinimapViewCCB*>(pView)->setRegionEffects(noArray);
        }
    }

}

void MinimapUIView::showJianView()
{
    m_mapInfoType = MapInfoType::SimType;
    
    //do
    
    m_jianxiangSpr->setSpriteFrame(CCLoadSprite::loadResource("world_xiang.png"));
    
    PopupBaseView* pView = m_ccbView;
    if(dynamic_cast<MinimapViewCCB*>(pView))
    {
        dynamic_cast<MinimapViewCCB*>(pView)->IsShowOrdinary(true);
    }
}

void MinimapUIView::showFanView()
{
    m_mapInfoType = MapInfoType::MoreType;
    
    //do
    m_jianxiangSpr->setSpriteFrame(CCLoadSprite::loadResource("world_jian_1.png"));
    PopupBaseView* pView = m_ccbView;
    if(dynamic_cast<MinimapViewCCB*>(pView))
    {
        dynamic_cast<MinimapViewCCB*>(pView)->IsShowOrdinary(false);
    }
}

//如果是从小地图过来的
void MinimapUIView::changeUIForMap()
{
    if(m_iMapIndex != 0)
    {
        m_jianxiangSpr->setVisible(false);
        m_biaojiNode_2->setVisible(true);
        m_biaojiNode->setVisible(false);
    }else{
        m_jianxiangSpr->setVisible(true);
        m_biaojiNode_2->setVisible(false);
        m_biaojiNode->setVisible(true);
    }
}

void MinimapUIView::getShouldPos(Vec2 &curPos, CCSize size)
{
    auto ScreenSize = Director::getInstance()->getWinSize();
    if(curPos.x < 0)
    {
        curPos.x = 0;
    }
    
    if(curPos.x > ScreenSize.width - size.width)
    {
        curPos.x = ScreenSize.width - size.width;
    }
    
    if(curPos.y < 0)
    {
        curPos.y = 0;
    }
    
    if(curPos.y > ScreenSize.height - size.height)
    {
        curPos.y = ScreenSize.height - size.height;
    }

    
    
}
void MinimapUIView::refreashData(CCObject* obj)
{
    if(obj && dynamic_cast<CCInteger*>(obj)){  //
        int cellidx = dynamic_cast<CCInteger*>(obj)->getValue();
        //得到数据 根据索引 和 m_type
        m_iSelectIndex = cellidx;
        CCPoint pos  = m_tabView->getContentOffset();
        
        m_tabView->reloadData();
        
        m_tabView->setContentOffset(pos);
        //根据索引找坐标
        Vec2 tarPos = Vec2(555,555);
        
        
        //
        PopupBaseView* pView = m_ccbView;
        switch (m_MarkType) {
            case MARK_ALLIANCE:
                if(m_iMapIndex != 0){ //小地图 //
                    
                    if(MiniMapController::getInstance()->getAllianceMapByIndex(m_iMapIndex,m_iSelectIndex))
                    {
                        tarPos = MiniMapController::getInstance()->getAllianceMapByIndex(m_iMapIndex,m_iSelectIndex)->m_Pos;
                    }
                    
                    if(dynamic_cast<MinimapLocalView*>(pView))
                    {
                        dynamic_cast<MinimapLocalView*>(pView)->gotoCityPos(tarPos);
                        this->jumpPoint(tarPos.x, tarPos.y, m_iServerID);
                    }
                }else{
                    
                    if(MiniMapController::getInstance()->getAllianceByIndex(m_iSelectIndex))
                    {
                        tarPos = MiniMapController::getInstance()->getAllianceByIndex(m_iSelectIndex)->m_Pos;
                    }
                    if(dynamic_cast<MinimapViewCCB*>(pView))
                    {
                        dynamic_cast<MinimapViewCCB*>(pView)->gotoCityPos(tarPos);
                        this->jumpPoint(tarPos.x, tarPos.y, m_iServerID);
                        
                    }
                    
                }

                break;
            case MARK_CITY:
                
                 if(m_iMapIndex != 0){ //小地图 //
                    
                     if(MiniMapController::getInstance()->getCityMapByIndex(m_iMapIndex,m_iSelectIndex))
                     {
                         tarPos = MiniMapController::getInstance()->getCityMapByIndex(m_iMapIndex,m_iSelectIndex)->m_Pos;
                     }
                     
                     if(dynamic_cast<MinimapLocalView*>(pView))
                     {
                         dynamic_cast<MinimapLocalView*>(pView)->gotoCityPos(tarPos);
                         this->jumpPoint(tarPos.x, tarPos.y, m_iServerID);
                     }
                 }else{
                     
                     if(MiniMapController::getInstance()->getCityInfoByIndex(m_iSelectIndex))
                     {
                         tarPos = MiniMapController::getInstance()->getCityInfoByIndex(m_iSelectIndex)->m_Pos;
                     }
                     if(dynamic_cast<MinimapViewCCB*>(pView))
                     {
                         dynamic_cast<MinimapViewCCB*>(pView)->gotoCityPos(tarPos);
                         this->jumpPoint(tarPos.x, tarPos.y, m_iServerID);

                     }
                     
                 }
                
//                ;
                break;
                
            case MARK_FAVOURATE:
                
             
                if(WorldController::getInstance()->getFavoriteKeyByIndex(m_iSelectIndex))
                {
                   
                    
                    tarPos = WorldController::getInstance()->getFavoriteKeyByIndex(m_iSelectIndex)->point;
                    
                    //m_tilename->setString("标记");  todo  类型
                    if(dynamic_cast<MinimapViewCCB*>(pView))
                    {
                        dynamic_cast<MinimapViewCCB*>(pView)->gotoCityPos(tarPos);
                        this->jumpPoint(tarPos.x, tarPos.y, m_iServerID);
                    }
                }

//                ;
                break;
            case MARK_POWER:
                if(MiniMapController::getInstance()->getPowerInfoByIndex(m_iSelectIndex))
                {
                    if(dynamic_cast<MinimapViewCCB*>(pView))
                    {
                        dynamic_cast<MinimapViewCCB*>(pView)->setRegionEffects(MiniMapController::getInstance()->getPowerInfoByIndex(m_iSelectIndex)->m_cityVector);
                        if(MiniMapController::getInstance()->getPowerInfoByIndex(m_iSelectIndex)->m_cityVector.size()>0)
                        {
                           tarPos = MiniMapController::getInstance()->getCityPosByOrder(MiniMapController::getInstance()->getPowerInfoByIndex(m_iSelectIndex)->m_cityVector.front())->m_Pos;
                            dynamic_cast<MinimapViewCCB*>(pView)->gotoCityPos(tarPos);
                        
                        }
                    }
                    if(m_mapInfoType == MapInfoType::SimType)
                    {
                        showFanView();
                    }
                }
                ;
                break;
            default:
                break;
        }

    }
}

void MinimapUIView::onEnter()
{
    Layer::onEnter();
    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    this->setTouchEnabled(true);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MinimapUIView::refreashData), MINIMAP_UI_SELECT_POS, NULL);

}

void MinimapUIView::onExit()
{
    this->setTouchEnabled(false);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MINIMAP_UI_SELECT_POS);
    Layer::onExit();
}

SEL_CCControlHandler MinimapUIView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onJumpBtnClick", MinimapUIView::onJumpBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSerachBtnClick", MinimapUIView::onSerachBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onReturnBtnClick", MinimapUIView::onReturnBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAllianceBtnClick", MinimapUIView::onAllianceBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCityBtnClick", MinimapUIView::onCityBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCheckBtnClick", MinimapUIView::onCheckBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPowerBtnClick", MinimapUIView::onPowerBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onResBtnClick", MinimapUIView::onResBtnClick);
}

bool MinimapUIView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clipNode", CCNode*, this->m_clipNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_topNode", CCNode*, this->m_topNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_serachNode", CCNode*, this->m_serachNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_XeditNode", CCNode*, this->m_XeditNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_YeditNode", CCNode*, this->m_YeditNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_biaojiNode", CCNode*, this->m_biaojiNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_biaojiNode_2", CCNode*, this->m_biaojiNode_2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "areaNode", CCNode*, this->m_areaNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_serachBtn", CCControlButton*, this->m_serachBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_jumpBtn", CCControlButton*, this->m_jumpBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_returnBtn", CCControlButton*, this->m_returnBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_juanzhouSpr", CCSprite*, this->m_juanzhouSpr);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_jianxiangSpr", CCSprite*, this->m_jianxiangSpr);
     CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fanhuiSpr", CCSprite*, this->m_fanhuiSpr);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_jumpLab", CCLabelIF*, this->m_jumpLab);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerLab", CCLabelIF*, this->m_powerLab);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_checkLab", CCLabelIF*, this->m_checkLab);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_MyResLab", CCLabelIF*, this->m_MyResLab);

    if(m_iMapIndex != 0)
    {
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cityLab_2", CCLabelIF*, this->m_cityLab);
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_AllianceLab_2", CCLabelIF*, this->m_allianceLab);
    }else{
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceLab", CCLabelIF*, this->m_allianceLab);
        
        CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cityLab", CCLabelIF*, this->m_cityLab);
           }

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_zoneNode", CCNode*, this->m_zoneNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listinfo", CCNode*, this->m_listinfo);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tileLab", CCLabelIF*, this->m_tileLab);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noInfo", CCLabelIF*, this->m_noInfo);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tabTouchNode", CCNode*, this->m_tabTouchNode);

    return false;
}


void MinimapUIView::editBoxTextChanged(CCEditBox* editBox, const std::string& text)
{
    std::string temp  =text;
    size_t pos = temp.find_first_not_of("1234567890");
    while (pos!=std::string::npos) {
        temp.replace(pos, 1, "");
        pos = temp.find_first_not_of("1234567890");
    }
    int a = atoi(temp.c_str());
     //加入坐标长度限制
    if(a>860 || a<0)
    {
        //tip 限制
        CCCommonUtils::flyHint("", "", "不可超出坐标限制!!");
        return;
    }
    std::string aaaa = CC_CMDITOA(a);
    
   
    editBox->setText(aaaa.c_str());
}

void MinimapUIView::editBoxReturn(CCEditBox *editBox)
{
    int x = atoi(m_editXBox->getText());
    int y = atoi(m_editYBox->getText());
    if( x>860 || x<0 || y>860 || y<0){
        //tip 限制
        CCCommonUtils::flyHint("", "", "不可超出坐标限制!!");
        m_editXBox->setText(CC_CMDITOA(m_TarPos.x).c_str());
        m_editYBox->setText(CC_CMDITOA(m_TarPos.y).c_str());
    }else{
        m_TarPos = Vec2(x, y);
    }
}


CCSize MinimapUIView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    return CCSizeMake(450, 50);
}
CCSize MinimapUIView::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(450, 50);

}
CCTableViewCell* MinimapUIView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    MinmapUICell* cell = (MinmapUICell*)table->dequeueCell();
    if(cell){
        cell->removeAllChildrenWithCleanup(true);
        cell = NULL;
    }
    
//    if(cell){
//        cell->setData(m_MarkType,m_iSelectIndex);
////        if(idx == m_iSelectIndex){
////            cell->showRed(true);
////        }else{
////            cell->showRed(false);
////        }
//         ;
//    }else{
    
      cell = MinmapUICell::create(m_MarkType,(int)idx, m_iSelectIndex,m_iMapIndex ,m_listinfo);
    
//    }
    return cell;
}
ssize_t MinimapUIView::numberOfCellsInTableView(CCTableView *table)
{
    int icellNum = 0;
    switch (m_MarkType) {
        case MARK_CITY:
            if(m_iMapIndex != 0)
            {
                icellNum = (int)(MiniMapController::getInstance()->getCityMapSize(m_iMapIndex));

            }else{
                icellNum = (int)(MiniMapController::getInstance()->getCityInfoSize());

            }
            ;
            break;
            
        case MARK_POWER:
            
            icellNum = (int)(MiniMapController::getInstance()->getPowerInfoSize());

            ;
            break;
            
        case MARK_FAVOURATE:
            
            icellNum = (int)(WorldController::getInstance()->m_favoriteTiles.size());

            ;
            break;
            
        case MARK_ALLIANCE:
            
            if(m_iMapIndex != 0)
            {
                icellNum = (int)(MiniMapController::getInstance()->getAllianceMapSize(m_iMapIndex));
                
            }else{
                icellNum = (int)(MiniMapController::getInstance()->getAllianceInfoSize());
                
            }
            ;
            break;
            
        default:
            break;
    }
    return icellNum;
}
void MinimapUIView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{

}

void MinimapUIView::scrollViewDidScroll(CCScrollView* view)
{

}


bool MinimapUIView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_editOpen){
        m_editOpen = false;
        return false;
    }
   
    
    m_iTouchType = 0;
    
    if(/*!m_TableOpen && */isTouchInside(m_juanzhouSpr,pTouch))
    {
        m_juanzhouSpr->setScale(1.1);
        m_iTouchType = 1;
        return true;
    }
    
    else if( isTouchInside(m_jianxiangSpr,pTouch) && m_jianxiangSpr->isVisible())
    {
        m_jianxiangSpr->setScale(1.1);
        m_iTouchType = 2;
        return true;
    }
    
    else if(isTouchInside(m_fanhuiSpr,pTouch))
    {
        m_fanhuiSpr->setScale(1.1);
        m_iTouchType = 3;
        return true;
    }
    
    if(m_TableOpen  )
    {
        
        m_touchTable = isTouchInside(m_zoneNode, pTouch);
        if(m_touchTable)
        {
            m_tableIsMove = isTouchInside(m_tabTouchNode, pTouch);
            if(m_tableIsMove)
            {
                m_startMovePos = pTouch->getLocation();
                m_startZoneMovePos    = m_zoneNode->getPosition();
            }
            return true;
        }
        
    }
    return false;
    
}

void MinimapUIView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(!isTouchInside(m_fanhuiSpr,pTouch))
    {
        m_fanhuiSpr->setScale(1);
    }
    if(!isTouchInside(m_jianxiangSpr,pTouch) && m_jianxiangSpr->isVisible())
    {
        m_jianxiangSpr->setScale(1);
    }
    if(!isTouchInside(m_juanzhouSpr,pTouch))
    {
        m_juanzhouSpr->setScale(1);
    }
    
    if(m_TableOpen && m_tableIsMove)
    {
        auto tarPos = pTouch->getLocation();
        auto disPos = tarPos - m_startMovePos;
        //边界判断
        Vec2 ZoneTarPos = Vec2(m_startZoneMovePos + disPos);
        //auto size = Director::getInstance()->getWinSize();
        this->getShouldPos(ZoneTarPos, m_zoneNode->getContentSize());
        m_zoneNode->setPosition(ZoneTarPos);
    }

}
void MinimapUIView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
//    if(m_TableOpen && (!isTouchInside(m_zoneNode, pTouch)) && !m_touchTable)
//    {
//        fadeTableView();
//        return;
//    }
    m_tableIsMove = false;
    
    switch (m_iTouchType) {
        case 0:
            ;
            break;
            
        case 1:
            if(isTouchInside(m_juanzhouSpr,pTouch))
            {
                m_juanzhouSpr->setScale(1);
                
              
                if(m_BottomOpen)
                {
                    fadeBotoom();
                }else{
                    showBotoom();
                }
            }
            ;
            break;
            
        case 3:
            if(isTouchInside(m_fanhuiSpr, pTouch))
            {
                m_fanhuiSpr->setScale(1);
                closeUIView();
            }
            ;
            break;
        case 2:
            if(isTouchInside(m_jianxiangSpr, pTouch) && m_jianxiangSpr->isVisible())
            {
                m_jianxiangSpr->setScale(1);
                if(m_mapInfoType == MapInfoType::SimType)
                {
                    showFanView();
                }else if (m_mapInfoType == MapInfoType::MoreType)
                {
                    showJianView();
                }
            }
            ;
            break;
            

        default:
            break;
    }
   
    m_iTouchType = 0;
}

void MinimapUIView::onJumpBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    
    jumpTarPos();
    
}

void MinimapUIView::onSerachBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    
    if(m_TopOpen)
    {
        fadeTop();
    }else{
        showTop();
    }


}

void MinimapUIView::onReturnBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    
    closeUIView();
   
}

void MinimapUIView::onAllianceBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
//    if(m_TableOpen)
//    {
//        return;
//    }
//    if(m_touchTable)
//    {
//        return;
//    }
    if(m_zoneNode->getPositionY()<100 && m_zoneNode->getPositionY()>-160 )
    {
        return;
    }
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    
    //
    m_MarkType = MARK_ALLIANCE;
    //请求数据,之后
    updateTableView();

}

void MinimapUIView::onPowerBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
//    if(m_TableOpen)
//    {
//        return;
//    }
//     if(m_touchTable)
//     {
//         return;
//     }
    if(m_zoneNode->getPositionY()<100 && m_zoneNode->getPositionY()>-160 )
    {
        return;
    }
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    
    //
    m_MarkType = MARK_POWER;
    //请求数据,之后
    updateTableView();
}

void MinimapUIView::onCityBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
//    if(m_TableOpen)
//    {
//        return;
//    }
//    if(m_touchTable)
//    {
//        return;
//    }
    if(m_zoneNode->getPositionY()<100 && m_zoneNode->getPositionY()>-160 )
    {
        return;
    }
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    
    m_MarkType = MARK_CITY;
    //请求数据,之后
    updateTableView();

}

void MinimapUIView::setUIZorder(Node* node, float iOrder)
{
    if(node == NULL)
    {
        return;
    }
    
    node->setGlobalZOrder(iOrder);
    auto arr = node->getChildren();
    if (arr.size()>0) {
        for (auto child : arr)
        {
            CCNode *pNode = dynamic_cast<CCNode*>(child);
            this->setUIZorder(pNode, iOrder);
        }
    }
}
void MinimapUIView::onResBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
//    if(m_touchTable)
//    {
//        return;
//    }
    //add 我的领地信息
    return;
    auto view = MinimapAreaView::create();
    if(view)
    {
        this->addChild(view );
        this->setUIZorder(view,101);
    }
    
}

void MinimapUIView::onCheckBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
//    if(m_touchTable)
//    {
//        return;
//    }
    if(m_zoneNode->getPositionY()<100 && m_zoneNode->getPositionY()>-160 )
    {
        return;
    }
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    
    m_MarkType = MARK_FAVOURATE;
    //请求数据,之后
    updateTableView();

}

void MinimapUIView::jumpTarPos()
{
    
    if(WorldController::getInstance()->isInMap(m_TarPos)){
        WorldMapView::instance()->gotoTilePoint(m_TarPos,m_iServerID);
        GlobalData::shared()->playerInfo.currentServerId = m_iServerID;
        if (!GlobalData::shared()->playerInfo.isInSelfServer()) {
            WorldMapView::instance()->removeCover();
            WorldMapView::instance()->addCover();
        }
        else
            WorldMapView::instance()->removeCover();
        
    }
    closeAllUIView();

}
void MinimapUIView::closeAllUIView()
{
    PopupViewController::getInstance()->removeAllPopupView();
}

void MinimapUIView::closeUIView()
{
    if(m_TableOpen){
        fadeTableView();
    }else{
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MINIMAP_OPRN_MOVE,NULL);
        PopupViewController::getInstance()->removePopupView(m_ccbView);
    }
}

void MinimapUIView::jumpPoint(int x, int y, int serverID)
{
    m_TarPos = Vec2(x,y);
    m_iServerID = serverID;
    //change pos
    string strX = CC_CMDITOA(x);
    string strY = CC_CMDITOA(y);
    m_editXBox->setText(strX.c_str());
    m_editYBox->setText(strY.c_str());
    
    if(!m_editOpen)
    {
        showTop();
    }
}


MinmapUICell* MinmapUICell::create(MarkType iType, int cityId, int iSelectID,int imapIndex, CCNode* touchNode)
{
    auto ret = new MinmapUICell();
    if (ret && ret->init(iType,cityId,iSelectID,imapIndex,touchNode)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


void MinmapUICell::onEnter()
{
    CCNode::onEnter();
    setTouchEnabled(true);
}

void MinmapUICell::onExit()
{
    setTouchEnabled(false);
   
    CCNode::onExit();
}

bool MinmapUICell::init(MarkType iType, int cellId, int iSelectID,int imapIndex, CCNode* touchNode)
{
     bool ret = true;
    CCBLoadFile("WorldMinViewZoneCell",this,this);
    m_iType = iType;
    m_iCellidx = cellId;
    m_touchNode = touchNode;
    m_iMapIndex = imapIndex;
    
    //lab
    m_tilestate->setFntFile("Arial_Bold_Border.fnt");
   // m_tilestate->setString("跳转");
    m_tilename->setFntFile("Arial_Bold_Border.fnt");
   // m_tilename->setString("联盟");
    m_tilepos->setFntFile("Arial_Bold_Border.fnt");
    //m_tilepos->setString("城市");
   
    showRed(false);
    this->setContentSize(m_redDi->getContentSize());
    
    setData(iType,iSelectID);
    return ret;
}

void MinmapUICell::showRed(bool isVisible)
{
    m_redDi->setVisible(isVisible);
}

void MinmapUICell::setData(MarkType iType, int iIndex)
{
    m_tilestate->setVisible(true);
    m_clolorNode->setVisible(false);
    //根据类型 获取不同的数据
    m_iType = iType;
    switch (m_iType) {
        case MARK_ALLIANCE:
           // m_tilename->setString("联盟");
            if(m_iMapIndex != 0)
            {
                if(MiniMapController::getInstance()->getAllianceMapByIndex(m_iMapIndex,m_iCellidx))
                {
                    
                    m_tilestate->setString(MiniMapController::getInstance()->getAllianceMapByIndex(m_iMapIndex,m_iCellidx)->m_Name.c_str());
                    m_tilepos->setString(CCString::createWithFormat("(%.0f   %.0f)",MiniMapController::getInstance()->getAllianceMapByIndex(m_iMapIndex,m_iCellidx)->m_Pos.x,MiniMapController::getInstance()->getAllianceMapByIndex(m_iMapIndex,m_iCellidx)->m_Pos.y)->getCString());
                    m_tilename->setString(MiniMapController::getInstance()->getAllianceMapByIndex(m_iMapIndex,m_iCellidx)->m_ZoneName.c_str());
                    

                }

            }else{
            
                if(MiniMapController::getInstance()->getAllianceByIndex(m_iCellidx))
                {
                    
                    m_tilestate->setString(MiniMapController::getInstance()->getAllianceByIndex(m_iCellidx)->m_Name.c_str());
                    m_tilepos->setString(CCString::createWithFormat("(%.0f   %.0f)",MiniMapController::getInstance()->getAllianceByIndex(m_iCellidx)->m_Pos.x,MiniMapController::getInstance()->getAllianceByIndex(m_iCellidx)->m_Pos.y)->getCString());
                    
                };
                m_tilename->setString(MiniMapController::getInstance()->getAllianceByIndex(m_iCellidx)->m_ZoneName.c_str());

            }
                
            
            
            break;
        case MARK_CITY:
            //m_tilename->setString("城市");
            if(m_iMapIndex != 0)
            {
                if(MiniMapController::getInstance()->getCityMapByIndex(m_iMapIndex,m_iCellidx))
                {
                    if(MiniMapController::getInstance()->getCityMapByIndex(m_iMapIndex,m_iCellidx)->m_OccupiedState == OccupiedState::OCCUPIED_NOT)
                    {
                        m_tilestate->setString(_lang("new100001").c_str());
                        m_tilestate->setColor(ccc3(255, 255, 255));
                    }else if (MiniMapController::getInstance()->getCityMapByIndex(m_iMapIndex,m_iCellidx)->m_OccupiedState == OccupiedState::OCCUPIED_MY)
                    {
                        m_tilestate->setString(_lang("new100002").c_str());
                        m_tilestate->setColor(ccc3(0,255,0));
                    }else
                    {
                        m_tilestate->setString(_lang("new100002").c_str());
                        m_tilestate->setColor(ccc3(255,0,0));
                    }
                    m_tilepos->setString(CCString::createWithFormat("(%.0f   %.0f)",MiniMapController::getInstance()->getCityMapByIndex(m_iMapIndex,m_iCellidx)->m_Pos.x,MiniMapController::getInstance()->getCityMapByIndex(m_iMapIndex,m_iCellidx)->m_Pos.y)->getCString());
                    m_tilename->setString(MiniMapController::getInstance()->getCityMapByIndex(m_iMapIndex,m_iCellidx)->m_Name.c_str());// zone 1~13
                }
            }else{
                if(MiniMapController::getInstance()->getCityInfoByIndex(m_iCellidx))
                {
                    if(MiniMapController::getInstance()->getCityInfoByIndex(m_iCellidx)->m_OccupiedState == OccupiedState::OCCUPIED_NOT)
                    {
                        m_tilestate->setString(_lang("new100001").c_str());
                        m_tilestate->setColor(ccc3(255, 255, 255));
                    }else if (MiniMapController::getInstance()->getCityInfoByIndex(m_iCellidx)->m_OccupiedState == OccupiedState::OCCUPIED_MY)
                    {
                        m_tilestate->setString(_lang("new100002").c_str());
                        m_tilestate->setColor(ccc3(0,255,0));
                    }else
                    {
                        m_tilestate->setString(_lang("new100002").c_str());
                        m_tilestate->setColor(ccc3(255,0,0));
                    }
                     m_tilepos->setString(CCString::createWithFormat("(%.0f     %.0f)",MiniMapController::getInstance()->getCityInfoByIndex(m_iCellidx)->m_Pos.x,MiniMapController::getInstance()->getCityInfoByIndex(m_iCellidx)->m_Pos.y)->getCString());
                   m_tilename->setString(MiniMapController::getInstance()->getCityInfoByIndex(m_iCellidx)->m_Name.c_str());// zone 1~13
                }
            }
            
            
            ;
            break;
            
        case MARK_FAVOURATE:
        {
                if(WorldController::getInstance()->getFavoriteKeyByIndex(m_iCellidx))
                {
                    m_tilestate->setString(WorldController::getInstance()->getFavoriteKeyByIndex(m_iCellidx)->name.c_str());
                    
                     m_tilepos->setString(CCString::createWithFormat("(%.0f     %.0f)",WorldController::getInstance()->getFavoriteKeyByIndex(m_iCellidx)->point.x,WorldController::getInstance()->getFavoriteKeyByIndex(m_iCellidx)->point.y)->getCString());
                    string zonename = WorldController::getInstance()->getHomeOwnershipNameByMapPos(WorldController::getInstance()->getFavoriteKeyByIndex(m_iCellidx)->point);
                    m_tilename->setString(zonename.c_str());  //todo  类型
                    
                }
            
            ;
        }
            break;
        case MARK_POWER:
           // m_tilename->setString("势力");
            m_tilestate->setVisible(false);
            m_clolorNode->setVisible(true);
            if(MiniMapController::getInstance()->getPowerInfoByIndex(m_iCellidx))
            {
                
                m_layercolor->setColor((MiniMapController::getInstance()->getPowerInfoByIndex(m_iCellidx)->m_Colour));
               // m_tilepos->setString(CCString::createWithFormat("占领%d个",(int)(MiniMapController::getInstance()->getPowerInfoByIndex(m_iCellidx)->m_cityVector.size()))->getCString());
             //待写
                m_tilepos->setString(_lang_1("new100031", CC_ITOA((int)(MiniMapController::getInstance()->getPowerInfoByIndex(m_iCellidx)->m_cityVector.size()))));
            }
            m_tilename->setString((MiniMapController::getInstance()->getPowerInfoByIndex(m_iCellidx)->m_AllianceName.c_str()));

            
            ;
            break;
        default:
            break;
    }
    
    bool iIsVisible = (iIndex == m_iCellidx);
    showRed(iIsVisible);

}

bool MinmapUICell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tilestate", CCLabelIF*, m_tilestate);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tilename", CCLabelIF*, m_tilename);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tilepos", CCLabelIF*, m_tilepos);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_redDi", CCScale9Sprite*, m_redDi);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clolorNode", CCNode*, m_clolorNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_layercolor", CCLayerColor*, m_layercolor);
    
    
    return false;
    
}

SEL_CCControlHandler MinmapUICell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool MinmapUICell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_clickInSide = false;
    m_startPoint = pTouch->getStartLocation();
    if (isTouchInside(m_touchNode,pTouch)){
        m_clickInSide = true;
        if(isTouchInside(this,pTouch)){
            return true;
        }
    }
    return false;
}

void MinmapUICell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint endPoint = pTouch->getLocation();
    if(fabs(endPoint.x-m_startPoint.x)>2||fabs(endPoint.y-m_startPoint.y)>2){
        return;
    }
    //发送消息
    
//    showRed(true);
    auto interIndex = CCInteger::create(m_iCellidx);
   
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MINIMAP_UI_SELECT_POS,interIndex);
    //showRed(true);
}
