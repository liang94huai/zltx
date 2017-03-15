//
//  MinimapLocalView.cpp
//  IF
//
//  Created by 杜威 on 16-9-7.
//
//

#include "MinimapLocalView.h"
#include "MiniMapController.h"
#include "WorldMapView.h"
#include "UIComponent.h"
#include "GetAllianceMemberPointCommand.h"
#include "ServerListPopUpView.h"
#include "WorldController.h"
#include "ParticleController.h"
#include "SoundController.h"
#include "SceneController.h"
#include "YesNoDialog.h"
#include "DynamicResourceController.h"
#include "FunBuildController.h"
#include "ActivityController.h"
#include "DragonZhenYingView.h"
#include "AllianceManager.h"

MinimapLocalView::MinimapLocalView(MinimapViewCCB* minimap,Rect originRect,int serverId,int mapIndex):
m_OriginRect(originRect),
m_MinimapViewCCB(minimap),
m_currentServerId(serverId),
m_currentMapIndex(mapIndex),
m_isLongPress(false),
m_isLongPressBegan(false){
};

MinimapLocalView::~MinimapLocalView(){
    m_MinimapViewCCB->isOpenLocal=false;
};

MinimapLocalView *MinimapLocalView::create(MinimapViewCCB* minimap,Rect originRect,int serverId,int mapIndex){
    MinimapLocalView *ret = new MinimapLocalView(minimap,originRect,serverId,mapIndex);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MinimapLocalView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByWorldIndex(10, true);
        CCLoadSprite::doResourceByWorldIndex(11, true);
        CCLoadSprite::doResourceByWorldIndex(12, true);
        
        singleTouchClose = false;
        this->_modelLayer->setTouchEnabled(false);
        CCSize winsize = CCDirector::sharedDirector()->getWinSize();
        this->setContentSize(winsize);

        m_container = CCNode::create();
        m_mapSpr=Sprite::createWithSpriteFrameName(string("")+"world_map"+CC_CMDITOA(m_currentMapIndex)+".png");
        auto BgSize=m_mapSpr->getContentSize();
        m_mapW =BgSize.width>winsize.width?BgSize.width:winsize.width;
        m_mapH =BgSize.height>winsize.height?BgSize.height:winsize.height;
        m_mapSpr->setPosition(m_mapW/2,m_mapH/2);
        
        auto mapSprBg=Sprite::createWithSpriteFrameName(string("")+"world_mapBg"+CC_CMDITOA(m_currentMapIndex)+".png");
        mapSprBg->setScale(2);
        auto offy=(mapSprBg->getContentSize().height*2-BgSize.height)/2;
        mapSprBg->setPosition(m_mapW/2,m_mapH/2-offy);

        m_mapBg=Sprite::createWithSpriteFrameName("world_mapMinBg.png");
        m_mapBg->setPosition(Vec2(winsize.width / 2, winsize.height / 2));
        m_mapBg->setScale(1136/m_mapBg->getContentSize().height);
        //m_container->addChild(m_mapBg);
        m_container->addChild(mapSprBg);
        m_container->addChild(m_mapSpr);
        
        
        
        
        //地图触摸事件管理
        m_viewPort = HFViewport::create();
        m_viewPort->setViewPortTarget(m_container);
        
        m_viewPort->setSceneSize(m_mapW,m_mapH);
        m_viewPort->allowZoom = false;
        m_viewPort->setZoomLimitationMin(1.0);
        m_viewPort->setZoomLimitationMax(1.0);
        m_viewPort->setWorldBound(CCRect(0,0,m_mapW,m_mapH));
        m_viewPort->setTouchDelegate(this);
        
        this->addChild(m_mapBg);
        this->addChild(m_container);
        this->addChild(m_viewPort);
        
        m_viewPort->scrollTo(-Vec2(m_mapW / 2, m_mapH / 2)+ ccp(winsize.width / 2, winsize.height / 2),NULL, false);
        
        m_phoneIcon = CCNode::create();
        CCBLoadFile("UIMAPArrow", m_phoneIcon, NULL);
        m_phoneIcon->setVisible(false);
        
        m_cityNode= CCNode::create();
        m_mapSpr->addChild(m_cityNode);
        m_mapSpr->addChild(m_phoneIcon,2);
        this->setUIZorder(m_phoneIcon, 33);

        m_mapUILayer = MinimapUIView::create(m_currentServerId,m_currentMapIndex,this);
        this->addChild(m_mapUILayer,1);
        
        addAllianceCity();
        ret = true;
    }
    return ret;
}
void MinimapLocalView::setUIZorder(Node* node, float iOrder)
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
SEL_CCControlHandler MinimapLocalView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool MinimapLocalView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    return false;
}

void MinimapLocalView::onEnter()
{
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}
void MinimapLocalView::onExit(){
    setTouchEnabled(false);
    m_MinimapViewCCB->isOpenLocal=false;
    PopupBaseView::onExit();
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
}

void MinimapLocalView::update(float time){
//    if (m_isLongPressBegan)
//    {
//        m_isLongPressTime+=time;
//        if (m_isLongPressTime>=LONG_PRESS_INTERVAL&&!m_isLongPress)
//        {
//            m_isLongPress=true;
//            auto pt = getWorldPointByPoint(m_LongPressPos);
//            if(WorldController::getInstance()->isInMap(pt)){
//                auto size = CCDirector::sharedDirector()->getWinSize();
//                if (!m_phoneIcon->isVisible()) {
//                    m_phoneIcon->setVisible(true);
//                }
//                m_phoneIcon->setPosition(m_LongPressPos);
//                m_mapUILayer->jumpPoint(pt.x, pt.y, m_currentServerId);
//            }
//        }
//    }
    
}

int MinimapLocalView::getMapIndexWithPoint(const cocos2d::CCPoint& worldLocation)
{
    return 0;
}


bool MinimapLocalView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return false;
}

void MinimapLocalView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
}

void MinimapLocalView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){

}

// ITouchDelegate
bool MinimapLocalView::onBeginScroll(const cocos2d::CCPoint& worldLocation){
    m_isLongPressBegan=false;
    m_isLongPressTime=0;
    return true;
}

void MinimapLocalView::onStepScroll(const cocos2d::CCPoint& worldLocation,const cocos2d::CCPoint& delta){
    m_isLongPressBegan=false;
    m_isLongPressTime=0;
}

void MinimapLocalView::onEndScroll(const cocos2d::CCPoint& worldLocation){
    m_isLongPressBegan=false;
    m_isLongPressTime=0;
}

void MinimapLocalView::onSingleTouchBegin(const cocos2d::CCPoint& worldLocation){
    if(m_mapSpr->getBoundingBox().containsPoint(worldLocation))
    {
        m_isLongPressBegan=true;
        m_isLongPress=false;
        bool isTouchInCity = false;
        int index =int(m_cityNode->getChildrenCount())-1;
        while (index >= 0) {
            auto node = dynamic_cast<CCNode*>(m_cityNode->getChildren().at(index));
            auto size = node->getContentSize();
            CCRect rect = CCRect(node->getPositionX() - size.width / 2, node->getPositionY() - size.height / 2, size.width, size.height);
            auto nodePos=m_mapSpr->convertToNodeSpace(worldLocation+m_container->getPosition());
            if(rect.containsPoint(nodePos)){
                int cityIndex = node->getTag();
                auto Pos = WorldController::getPointByIndex(cityIndex);
                
                auto ptByIndex = WorldController::getPointByIndex(cityIndex);
                auto disPoint = ccpSub(ptByIndex, WorldMapView::instance()->m_map->centerTilePoint);
                auto worldPt = ccp(WorldMapView::instance()->m_map->centerViewPoint.x+(disPoint.x-disPoint.y)*_halfTileSize.width,WorldMapView::instance()->m_map->centerViewPoint.y-(disPoint.x+disPoint.y)*_halfTileSize.height);
                m_LongPressPos = getPointByWorldPoint(worldPt);
                isTouchInCity = true;
                break;
            }
            index--;
        }
        if(!isTouchInCity){
            m_LongPressPos = m_mapSpr->convertToNodeSpace(worldLocation+m_container->getPosition());;
        }
        m_isLongPressTime=0;
    }
}
void MinimapLocalView::onSingleTouchEnd(const cocos2d::CCPoint& worldLocation){
    m_isLongPressBegan=false;
    m_isLongPressTime=0;
    
    auto pt = getWorldPointByPoint(m_LongPressPos);
    if(WorldController::getInstance()->isInMap(pt)){
        auto size = CCDirector::sharedDirector()->getWinSize();
        if (!m_phoneIcon->isVisible()) {
            m_phoneIcon->setVisible(true);
        }
        m_phoneIcon->setPosition(m_LongPressPos);
        m_mapUILayer->jumpPoint(pt.x, pt.y, m_currentServerId);
    }
}

void MinimapLocalView::onStepZoom(const cocos2d::CCPoint& point1, const cocos2d::CCPoint& point2){
}

void MinimapLocalView::onBeginZoom(const CCPoint& point1,const CCPoint& point2){
}

void MinimapLocalView::onEndZoom(float targetZoom){
}

void MinimapLocalView::moveBack(){
}

void MinimapLocalView::scaleBack(){

}

void MinimapLocalView::showSelfCity(){

}

Vec2 MinimapLocalView::getPointByTilePos(Vec2 &pt)
{
    auto index=WorldController::getIndexByPoint(pt);
    auto ptByIndex = WorldController::getPointByIndex(index);
    auto disPoint = ccpSub(ptByIndex, WorldMapView::instance()->m_map->centerTilePoint);
    auto worldPt = ccp(WorldMapView::instance()->m_map->centerViewPoint.x+(disPoint.x-disPoint.y)*_halfTileSize.width,WorldMapView::instance()->m_map->centerViewPoint.y-(disPoint.x+disPoint.y)*_halfTileSize.height);
    return getPointByWorldPoint(worldPt);
}


Vec2 MinimapLocalView::getPointByWorldPoint(Vec2 &pt)
{
    auto mapPos=m_MinimapViewCCB->getPointByWorldPoint(pt,m_MinimapViewCCB->m_miniScale);
    auto nodePos=mapPos-m_OriginRect.origin;
    auto BgSize=m_mapSpr->getContentSize();
    auto Proportion=BgSize.width/m_OriginRect.size.width;
    return nodePos*Proportion;
}
Vec2 MinimapLocalView::getWorldPointByPoint(const Vec2 &pt)
{
    auto BgSize=m_mapSpr->getContentSize();
    auto Proportion=BgSize.width/m_OriginRect.size.width;
    auto nodePos=pt/Proportion;
    auto mapPos=nodePos+m_OriginRect.origin;
    return m_MinimapViewCCB->getWorldPointByPoint(mapPos,m_MinimapViewCCB->m_miniScale);
}

void MinimapLocalView::addCity(int index, int type,int level)
{
    // 添加图片
    std::string iconPic = "position_city.png";
    float s = 1;
    if(type == 0){
         iconPic = "mini_icon_myCity.png";
    }else if(type == 1){
        std::string iconPicName="mini_icon_city_";
        iconPic = iconPicName+CC_ITOA(level)+".png";
    }else if(type == 2){
        iconPic = "mini_icon_copycat1.png";
    }else if(type == 3){
        iconPic = "relic_city.png";
    }else if(type == 4){
        iconPic = "position_territory.png";
    }

    auto sprite = CCLoadSprite::createSprite(iconPic.c_str());
    m_cityNode->addChild(sprite);
    sprite->setTag(index);
    
    auto ptByIndex = WorldController::getPointByIndex(index);
    auto disPoint = ccpSub(ptByIndex, WorldMapView::instance()->m_map->centerTilePoint);
    auto worldPt = ccp(WorldMapView::instance()->m_map->centerViewPoint.x+(disPoint.x-disPoint.y)*_halfTileSize.width,WorldMapView::instance()->m_map->centerViewPoint.y-(disPoint.x+disPoint.y)*_halfTileSize.height);
    auto cityPt = getPointByWorldPoint(worldPt);
    
    sprite->setPosition(cityPt);
    sprite->setScale(s);
    sprite->setVisible(index!=0);
}

void MinimapLocalView::addAllianceCity()
{
    //小城
    for(auto it_2 = WorldController::getInstance()->m_resCityInfo.begin(); it_2 != WorldController::getInstance()->m_resCityInfo.end(); it_2++){
        int RegionType=WorldController::getInstance()->getHomeOwnerRegionTypeByMapPos(it_2->second.m_point);
        if (RegionType==m_currentMapIndex)
        {
            addCity(it_2->second.m_point, 2);
        }
    }
    //大城
    auto CityIds=MiniMapController::getInstance()->getCityIdsByOrder(m_currentMapIndex);
    for(int j=0;j<CityIds.size();j++)
    {
        auto miniCityInfo=MiniMapController::getInstance()->getCityPosById(atoi(CityIds[j].c_str()));
        auto index=WorldController::getIndexByPoint(miniCityInfo->m_Pos);
        int level = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(miniCityInfo->m_CityId))->valueForKey("level")->intValue();
        addCity(index, 1,level);
    }
    
//    if(m_currentServerId == GlobalData::shared()->playerInfo.selfServerId){
//        addCity(WorldController::getIndexByPoint(WorldController::getInstance()->selfPoint), 0);
//    }
}

void MinimapLocalView::gotoCityPos(Vec2 &cityPos)
{
    auto cityPt=getPointByTilePos(cityPos);
    auto size = CCDirector::sharedDirector()->getWinSize();
    auto BgSize=m_mapSpr->getContentSize();
    auto pos=cityPt+(m_mapSpr->getPosition()-Vec2(BgSize.width/2,BgSize.height/2));
    m_viewPort->scrollTo(-pos + ccp(size.width / 2, size.height / 2), NULL,true);
    if (!m_phoneIcon->isVisible()) {
        m_phoneIcon->setVisible(true);
    }
    m_phoneIcon->setPosition(cityPt);
}



