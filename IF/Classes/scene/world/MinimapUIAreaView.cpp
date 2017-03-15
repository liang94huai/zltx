//
//  MinimapAreaView.cpp
//  IF
//
//  Created by 刘田厚 on 17-1-4.
//
//




#include "MinimapUIAreaView.h"
#include "SoundController.h"
#include "PopupViewController.h"
#include "MinimapViewCCB.h"
#include "WorldMapView.h"
#include "WorldController.h"
#include "MiniMapController.h"
#include "MinimapLocalView.h"

MinimapAreaView::MinimapAreaView(){}
MinimapAreaView::~MinimapAreaView(){
}


MinimapAreaView *MinimapAreaView::create()
{
    MinimapAreaView *ret = new MinimapAreaView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool MinimapAreaView::init()
{
    bool ret = false;
    if(PopupBaseView::init()){
       
        
        ret = true;
    }
    
     

    auto tmpCCB = CCBLoadFile("WorldMinViewArea",this,this);
    
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else
        this->setContentSize(tmpCCB->getContentSize());
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    
    if (!CCCommonUtils::isIosAndroidPad()) {
        int oldBgHeight = m_buildBG->getContentSize().height;
        changeBGHeight(m_buildBG);
        int newBgHeight = m_buildBG->getContentSize().height;
        int addHeight = winSize.height-852;
        int oldWidth = m_infoList->getContentSize().width;
        int oldHeight = m_infoList->getContentSize().height;
        m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
        m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
    }
    
    setTitleName(_lang("new100030").c_str());
    
    initLabel();
    
    //排序 , 采集时间长的放前面
    sortMapByValue();
    
    
    
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_tabView->setDelegate(this);
    m_infoList->addChild(m_tabView);
    
    return true;
}
void MinimapAreaView::initLabel()
{
    m_title_1->setFntFile("Arial_Bold_Border.fnt");
    m_title_1->setString(_lang("new100015").c_str());
    m_title_2->setFntFile("Arial_Bold_Border.fnt");
    m_title_2->setString(_lang("new100034").c_str());
    m_title_3->setFntFile("Arial_Bold_Border.fnt");
    m_title_3->setString(_lang("new100035").c_str());
    m_title_4->setFntFile("Arial_Bold_Border.fnt");
    m_title_4->setString(_lang("new100036").c_str());
    m_title_5->setFntFile("Arial_Bold_Border.fnt");
    m_title_5->setString(_lang("new100038").c_str());
    m_no_lab->setFntFile("Arial_Bold_Border.fnt");
    m_no_lab->setString(_lang("new100037").c_str());
}

void MinimapAreaView::onEnter()
{
    CCNode::onEnter();
    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    this->setTouchEnabled(true);
    this->schedule(schedule_selector(MinimapAreaView::updateTime), 1, -1, 1);
}

void MinimapAreaView::onExit()
{
    this->unschedule(schedule_selector(MinimapAreaView::updateTime));
    this->setTouchEnabled(false);
   
    CCNode::onExit();
}

SEL_CCControlHandler MinimapAreaView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseClick", MinimapAreaView::onReturnBtnClick);
}

bool MinimapAreaView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG2", CCScale9Sprite*, this->m_buildBG2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_flowerNode", CCNode*, this->m_flowerNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noInfo", CCNode*, this->m_noInfo);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_closeBtn", CCControlButton*, this->m_closeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title_1", CCLabelIF*, this->m_title_1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title_2", CCLabelIF*, this->m_title_2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title_3", CCLabelIF*, this->m_title_3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title_4", CCLabelIF*, this->m_title_4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title_5", CCLabelIF*, this->m_title_5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_no_lab", CCLabelIF*, this->m_no_lab);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleText", CCLabelIF*, this->m_titleText);

    return false;
}



CCSize MinimapAreaView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    return CCSizeMake(620, 80);
}
CCSize MinimapAreaView::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(620, 80);

}
CCTableViewCell* MinimapAreaView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    MiniAreaCell* cell = (MiniAreaCell*)table->dequeueCell();
    if(cell){
        cell->removeAllChildrenWithCleanup(true);
        cell = NULL;
    }
    //根据idx  获取 map的pointIndex
    if(idx<0 || idx >= m_VectRes.size())
    {
        return nullptr;
    }
    int iPointIndex = m_VectRes[idx].first;
    if(cell){
        cell->setData(iPointIndex);
        
    }else{
    
      cell = MiniAreaCell::create(idx,this);
      cell->setData(iPointIndex);
    
    }
    return cell;
}
ssize_t MinimapAreaView::numberOfCellsInTableView(CCTableView *table)
{
    int icellNum = WorldController::getInstance()->m_resCityInfo.size();
   
    return icellNum;
}
void MinimapAreaView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{

}

void MinimapAreaView::scrollViewDidScroll(CCScrollView* view)
{

}

int MinimapAreaView::cmp(const std::pair<int, double>& x, const std::pair<int, double>& y)
{
    
   return x.second < y.second;
}

//排序
void MinimapAreaView::sortMapByValue()
{
    for(auto it = WorldController::getInstance()->m_resCityInfo.begin(); it != WorldController::getInstance()->m_resCityInfo.end(); it++)
    {
        m_VectRes.push_back(std::make_pair(it->first, it->second.m_StartTime));
    }
    std::sort(m_VectRes.begin(), m_VectRes.end(), MinimapAreaView::cmp);
    
}

// 更新每个cell收获时间
void MinimapAreaView::updateTime(float delta)
{
    if(WorldController::getInstance()->m_resCityInfo.size() == 0)
    {
        m_noInfo->setVisible(true);
        //this->unschedule(schedule_selector(MinimapAreaView::updateTime));
    }else{
        m_noInfo->setVisible(false);
    }
    auto pos = m_tabView->getContentOffset();
    m_tabView->reloadData();
    m_tabView->setContentOffset(pos);
}

bool MinimapAreaView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    
    return true;
    
}

void MinimapAreaView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    

}
void MinimapAreaView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}


void MinimapAreaView::onReturnBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    
    closeUIView();
   
}
 

void MinimapAreaView::closeUIView()
{
    this->closeSelf();
}
 


MiniAreaCell* MiniAreaCell::create( int cityId,PopupBaseView* view)
{
    auto ret = new MiniAreaCell(cityId,view);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    
    return ret;
}


void MiniAreaCell::onEnter()
{
    CCNode::onEnter();
    setTouchEnabled(true);
}

void MiniAreaCell::onExit()
{
    setTouchEnabled(false);
   
    CCNode::onExit();
}

bool MiniAreaCell::init()
{
     bool ret = true;
    auto ccB =  CCBLoadFile("WorldMinViewAreaCell",this,this);
    this->setContentSize(ccB->getContentSize());
    initLab();
    //setData(m_iResidx);
    return ret;
}

void MiniAreaCell::initLab()
{
    m_resName->setFntFile("Arial_Bold_Border.fnt");
    m_tilename->setFntFile("Arial_Bold_Border.fnt");
    m_armNum->setFntFile("Arial_Bold_Border.fnt");
    m_protime->setFntFile("Arial_Bold_Border.fnt");
    m_gotoLab->setFntFile("Arial_Bold_Border.fnt");
    m_gotoLab->setString(_lang("108525").c_str());  ;
}
void MiniAreaCell::setData(int iIndex)
{
    m_iCellidx = iIndex;
    auto& pInfo = WorldController::getInstance()->m_resCityInfo.at(iIndex);
    
    auto dict = LocalController::shared()->DBXMLManager()->getObjectByKey(pInfo.m_uID);
    
    auto cityname = dict->valueForKey("name")->getCString();
    
    m_resName->setString(_lang(cityname).c_str());
    
    auto areaname = WorldController::getInstance()->getHomeOwnershipNameByCityIndex(iIndex);
    m_tilename->setString(areaname);
    m_armNum->setString(CC_ITOA(pInfo.m_GarArmNum));
    if(pInfo.m_GarArmNum == 0)
    {
        m_armNum->setString("无");
    }
    auto nowtime = WorldController::getInstance()->getTime()/1000;
    auto starttime = pInfo.m_StartTime/1000;
    int dtTime = nowtime - starttime;
    

    int needtime = dict->valueForKey("resTime")->intValue();
    if(needtime <= dtTime)
    {
        //stop change
        m_protime->setString("可收获");
    }else{
        
        m_protime->setString(CC_SECTOA(-dtTime+needtime));
    }
     

   
    m_bg1->setVisible(m_iResidx%2==0?false:true);
    //    m_bg2->setVisible(m_type==0?false:true);
    m_bg2->setVisible(m_iResidx%2==0?true:false);
}

bool MiniAreaCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resName", CCLabelIF*, m_resName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tilename", CCLabelIF*, m_tilename);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_armNum", CCLabelIF*, m_armNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_protime", CCLabelIF*, m_protime);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_gotoLab", CCLabelIF*, m_gotoLab);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg1", CCScale9Sprite*, m_bg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg2", CCScale9Sprite*, m_bg2);
    
    
    return false;
    
}

SEL_CCControlHandler MiniAreaCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGotoBtnClick", ::MiniAreaCell::onGotoBtnClick);
    return NULL;
}

void MiniAreaCell::onGotoBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    //go to
    
    auto  TarPos = WorldController::getPointByIndex(m_iCellidx);
    if(WorldController::getInstance()->isInMap(TarPos)){
        WorldMapView::instance()->gotoTilePoint(TarPos);
        
        if (!GlobalData::shared()->playerInfo.isInSelfServer()) {
            WorldMapView::instance()->removeCover();
            WorldMapView::instance()->addCover();
        }
        else
            WorldMapView::instance()->removeCover();
        
    }
    m_ccbView->closeSelf();
}

bool MiniAreaCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
    return false;
}

void MiniAreaCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
   
}
