//
//  MinimapViewCCB.cpp
//  IF
//
//  Created by 杜威 on 16-9-7.
//
//

#include "MinimapViewCCB.h"
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
#include "MinimapUIView.h"
#include "MinimapLocalView.h"
#include "MiniMapController.h"

#define Legend_Node_Tag 10000
#define Title_Node_Tag 9999
#define Map_Node_Tag 10001//常驻7块 移动时添加3块 最多10块
#define Cover_Node_Tag 10011
#define Server_Banner_Tag 10012
#define Building_Icon_Node_Tag 1000001

MinimapViewCCB::MinimapViewCCB(int serverId):
m_currentServerId(serverId),
m_isLongPress(false),
m_isLongPressBegan(false),
m_isOpenMove(true),
m_isOrdinary(true){
};

MinimapViewCCB::~MinimapViewCCB(){
};

MinimapViewCCB *MinimapViewCCB::create(int serverId){
    MinimapViewCCB *ret = new MinimapViewCCB(serverId);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MinimapViewCCB::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByWorldIndex(6, false);
            CCLoadSprite::doResourceByWorldIndex(9, false);
            CCLoadSprite::doResourceByWorldIndex(10, false);
            CCLoadSprite::doResourceByWorldIndex(11, false);
            CCLoadSprite::doResourceByWorldIndex(12, false);
        });
        CCLoadSprite::doResourceByWorldIndex(6, true);
        CCLoadSprite::doResourceByWorldIndex(9, true);
        CCLoadSprite::doResourceByWorldIndex(10, true);
        CCLoadSprite::doResourceByWorldIndex(11, true);
        CCLoadSprite::doResourceByWorldIndex(12, true);
        
        singleTouchClose = false;
        isOpenLocal=false;
        this->_modelLayer->setTouchEnabled(false);
        CCSize winsize = CCDirector::sharedDirector()->getWinSize();
        this->setContentSize(winsize);
        auto winHeight=winsize.height>1136?winsize.height:1136;
        
        m_container = CCNode::create();
        m_miniCCBNode= CCNode::create();
        m_miniRenderTextureNode= CCNode::create();
        m_cityNode= CCNode::create();

        m_container->addChild(m_miniRenderTextureNode);
        m_container->addChild(m_miniCCBNode);
        m_container->addChild(m_cityNode);
        
        CCBLoadFile("MinimapView",m_miniCCBNode,this);
        m_regionNode->setVisible(!m_isOrdinary); 
        auto BgSize=m_mapBg->getBoundingBox().size;
        m_miniScale=winHeight/BgSize.height;
        
        m_mapW =BgSize.width*m_miniScale;
        m_mapH =BgSize.height*m_miniScale;
        m_mapNode->setScale(m_miniScale);


        //地图触摸事件管理
        m_viewPort = HFViewport::create();
        m_viewPort->setViewPortTarget(m_container);
        
        viewCenterPoint.x=m_mapW / 2;
        viewCenterPoint.y=m_mapH / 2;
        
        m_viewPort->setSceneSize(m_mapW,m_mapH);
        m_viewPort->allowZoom = false;
        m_viewPort->setZoomLimitationMin(1.0);
        m_viewPort->setZoomLimitationMax(1.0);
        m_viewPort->setWorldBound(CCRect(0,0,m_mapW,m_mapH));
        m_viewPort->setTouchDelegate(this);
    
        for(int i=0;i<MAP_COUNT;i++)
        {
            auto mapSp=m_mapSpVec[i];
            mapSp->setVisible(false);
            auto mRenderTexture = RenderTexture::create(mapSp->getBoundingBox().size.width*m_miniScale, mapSp->getBoundingBox().size.height*m_miniScale, kCCTexture2DPixelFormat_RGBA8888);
            mRenderTexture->ignoreAnchorPointForPosition(true);
            mRenderTexture->setPosition(mapSp->getPosition()*m_miniScale);
            mRenderTexture->setAnchorPoint(CCPointZero);
            m_miniRenderTextureNode->addChild(mRenderTexture,1,i+1);
            m_mapRenderTextureVec[i]=mRenderTexture;
            
            //开始准备绘制
            mRenderTexture->begin();
            //绘制使用的临时精灵，与原图是同一图片
            Sprite* pTempSpr = Sprite::createWithSpriteFrame(mapSp->displayFrame());
            pTempSpr->setScale(mapSp->getScale()*m_miniScale);
            pTempSpr->setPosition(ccp(mapSp->getBoundingBox().size.width / 2*m_miniScale, mapSp->getBoundingBox().size.height / 2*m_miniScale));
            //绘制
            pTempSpr->visit();
            //结束绘制
            mRenderTexture->end();
        }
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
         ccBlendFunc cbf = {GL_ONE,GL_ONE_MINUS_SRC_ALPHA};
         for (int i=0;i<REGION_COUNT;i++)
         {
             m_regionSpVec[i]->setBlendFunc(cbf);
         }
         m_regionSpr->setBlendFunc(cbf);
#endif

        this->addChild(m_container);
        this->addChild(m_viewPort);
        
        //UI
        m_mapUILayer = MinimapUIView::create(m_currentServerId,0,this);
        this->addChild(m_mapUILayer,1);
        //m_mapUILayer->setGlobalZOrder(0);
       // this->setUIZorder(m_mapUILayer, 1);
        //信息
        m_mapInfo = MinimapInfo::create();
        m_mapInfo->selfPt = WorldController::getIndexByPoint(WorldController::getInstance()->selfPoint);
        
        showSelfCity();
        addAllianceCity();
        ret = true;
    }
    return ret;
}

SEL_CCControlHandler MinimapViewCCB::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool MinimapViewCCB::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mapNode", CCNode*, this->m_mapNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mapBg", CCSprite*, this->m_mapBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_regionNode", CCNode*, this->m_regionNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_regionSpr", CCSprite*, this->m_regionSpr);
    //区域
    if (pTarget == this && strncmp(pMemberVariableName, "m_region",8) == 0) {
        char index[5] = "";
        strncpy(index, pMemberVariableName + 8, strlen(pMemberVariableName) - 8);
        int idx = atoi(index)-1;
        m_regionSpVec[idx] = dynamic_cast<CCSprite*>(pNode);
        return true;
    }
    //光标
    if (pTarget == this && strncmp(pMemberVariableName, "map",3) == 0) {
        char index[5] = "";
        strncpy(index, pMemberVariableName + 3, strlen(pMemberVariableName) - 3);
        int idx = atoi(index)-1;
        m_mapSpVec[idx] = dynamic_cast<CCSprite*>(pNode);
        return true;
    }
    return false;
}

void MinimapViewCCB::openMove(Object* obj)
{
    m_isOpenMove=true;
    m_viewPort->notMove=false;
    
    //箭头显示
    m_phoneIcon->setVisible(m_isOpenMove);
    //打开UPdate
     CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
    
}


void MinimapViewCCB::setUIZorder(Node* node, float iOrder)
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

void MinimapViewCCB::onEnter()
{
    PopupBaseView::onEnter();
    
    //协议
    auto cmd = new MapGetMainCityCommand();
    
    cmd->sendAndRelease();
    
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    
    if(WorldMapView::instance()){
        WorldMapView::instance()->m_touchDelegateView->notMove = true;
    }
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MinimapViewCCB::openMove), MINIMAP_OPRN_MOVE, NULL);
    
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MinimapView::onShowMarkLine), MSG_MARK_LINE_AL, NULL);
//    m_canClickMiniMap = 0;
    if( WorldMapView::instance() && WorldMapView::instance()->m_touchDelegateView){
        Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(WorldMapView::instance()->m_touchDelegateView);
    }
}
void MinimapViewCCB::onExit(){
    if( WorldMapView::instance() && WorldMapView::instance()->m_touchDelegateView){
        Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(WorldMapView::instance()->m_touchDelegateView);
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MINIMAP_OPRN_MOVE);
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    if(WorldMapView::instance()){
        WorldMapView::instance()->m_touchDelegateView->notMove = false;
    }

    setTouchEnabled(false);
    PopupBaseView::onExit();
}

void MinimapViewCCB::update(float time){
    if (m_isLongPressBegan)
    {
        m_isLongPressTime+=time;
        if (m_isLongPressTime>=LONG_PRESS_INTERVAL&&!m_isLongPress)
        {
            m_isLongPress=true;
            auto pt = getWorldPointByPoint(m_LongPressPos,m_miniScale);
            if(WorldController::getInstance()->isInMap(pt)){
                auto size = CCDirector::sharedDirector()->getWinSize();
                if (!m_phoneIcon->isVisible()) {
                    m_phoneIcon->setVisible(true);
                }
                m_phoneIcon->setPosition(m_LongPressPos);
                m_mapUILayer->jumpPoint(pt.x, pt.y, m_currentServerId);
            }
        }
    }
    
//    if(GlobalData::shared()->playerInfo.isInSelfServer()){
//        WorldActivityState state = WorldController::getInstance()->getKingActivityStateByType(WorldActivityType::FIGHT_OF_KING);
//        if(state == NotOpen){
//            return;
//        }
//        if(!m_timeText){
//            return;
//        }
//        auto it = WorldController::getInstance()->m_worldActivity.find(FIGHT_OF_KING);
//        if(it == WorldController::getInstance()->m_worldActivity.end()){
//            return;
//        }
//        WorldActivityInfo &activityInfo = it->second;
//        auto now = WorldController::getInstance()->getTime();
//        double time = 0;
//        double startTime = activityInfo.timeInfo.startTime;
//        double endTime = activityInfo.timeInfo.endTime;
//        if(state == OpenNoKing){
//            time = startTime - now;
//        }else if(state == PeaceTime){
//            time = startTime - now;
//        }else if(state == WarTime){
//            time = endTime - now;
//        }
//        time = time / 1000;
//        if(time < 0){
//            time = 0;
//        }
//        m_timeText->setString(CCCommonUtils::timeLeftToCountDown(time));
//    }
}

int MinimapViewCCB::getMapIndexWithPoint(const cocos2d::CCPoint& worldLocation)
{
    auto a=m_container->getPosition();
//    CCLOG("当偏移的: X = %f,Y = %f", a.x,a.y);

    for(int i=0;i<MAP_COUNT;i++)
    {
        auto mRenderTexture=m_mapRenderTextureVec[i];
        auto spr=mRenderTexture->getSprite();
        auto sprBoundingBox=spr->boundingBox();
        auto RenderTextureBoundingBox=mRenderTexture->boundingBox();
        auto mPos=Vec2(RenderTextureBoundingBox.getMinX()+sprBoundingBox.getMinX(),RenderTextureBoundingBox.getMinY()+sprBoundingBox.getMinY());
        auto mSize=Size(sprBoundingBox.size);
        auto mRect=Rect(mPos,mSize);
//        CCLOG("当坐标的: X = %f,Y = %f", mPos.x,mPos.y);
//        CCLOG("矩形: X1 = %f, X2 = %f,Y1 = %f,Y2 = %f", mRect.getMinX(),mRect.getMaxX(),mRect.getMinY(),mRect.getMaxY());
//        CCLOG("当点击的: X = %f,Y = %f", worldLocation.x,worldLocation.y);
        if (mRect.containsPoint(worldLocation)) {
            CCLOG("containsPoint:%d",i);
            ccColor4B color4B = {0, 0, 0, 0};
            
            Vec2 nodePos = spr->convertToNodeSpace(worldLocation+m_container->getPosition());
            unsigned int x = nodePos.x;
            unsigned int y = spr->getContentSize().height - nodePos.y;
            
//            CCLOG("当前点击的点的: X = %d,Y = %d", x,y);
            
            
            //通过画布拿到这张画布上每个像素点的信息，封装到CCImage中
            CCImage* pImage = mRenderTexture->newCCImage();
            //获取像素数据
            unsigned char* data_ = pImage->getData();
            unsigned int *pixel = (unsigned int *)data_;
            pixel = pixel + (y * (int)(spr->getContentSize().width)) * 1 + x * 1;
            //R通道
            color4B.r = *pixel & 0xff;
            //G通道
            color4B.g = (*pixel >> 8) & 0xff;
            //B通过
            color4B.b = (*pixel >> 16) & 0xff;
            //Alpha通道，我们有用的就是Alpha
            color4B.a = (*pixel >> 24) & 0xff;
            
//            CCLOG("当前点击的点的: alpha = %d", color4B.a);
            
            if (color4B.a > 0) {
                return i+1;
            }
        }
    }
    return 0;
}

CCPoint MinimapViewCCB::getRealPosByPoint(CCPoint &pt){
    auto disPoint = ccpSub(pt, viewCenterPoint);
    return ccp(viewCenterPoint.x + (disPoint.x-disPoint.y) * _tile_width / 2,
               viewCenterPoint.y - (disPoint.x+disPoint.y) * _tile_height / 2);
}

Vec2 MinimapViewCCB::getPointByTilePos(Vec2 tPos)
{
    auto index=WorldController::getIndexByPoint(tPos);
    auto ptByIndex = WorldController::getPointByIndex(index);
    auto disPoint = ccpSub(ptByIndex, WorldMapView::instance()->m_map->centerTilePoint);
    auto worldPt = ccp(WorldMapView::instance()->m_map->centerViewPoint.x+(disPoint.x-disPoint.y)*_halfTileSize.width,WorldMapView::instance()->m_map->centerViewPoint.y-(disPoint.x+disPoint.y)*_halfTileSize.height);
    return getPointByWorldPoint(worldPt,m_miniScale);
}

CCPoint MinimapViewCCB::getPointByWorldPoint(CCPoint &pt,float scale){
    auto centerViewPoint=WorldMapView::instance()->m_map->centerViewPoint;
    auto realPtSub = pt - centerViewPoint;
    auto Multiple=WIDTH * scale * 1.0f / _map_width;
    auto miniPtSub = realPtSub * Multiple;
    if (WorldController::getInstance()->currentMapType==DRAGON_MAP) {
        miniPtSub = realPtSub * WIDTH * scale * 1.0f / WorldController::getInstance()->_current_map_width;
    }
    auto miniPt = getRealPosByPoint(viewCenterPoint) + miniPtSub;
    return miniPt;
}

CCPoint MinimapViewCCB::getWorldPointByPoint(const CCPoint &pt,float scale){
    auto miniPtSub = pt - getRealPosByPoint(viewCenterPoint);
    auto Multiple=_map_width * 1.0f / (WIDTH * scale);
    auto realPtSub = miniPtSub * Multiple;
    if (WorldController::getInstance()->currentMapType==DRAGON_MAP) {
        realPtSub = miniPtSub * WorldController::getInstance()->_current_map_width * 1.0f / WIDTH * scale;
    }
    auto centerViewPoint=WorldMapView::instance()->m_map->centerViewPoint;
    auto realPt =  centerViewPoint+ realPtSub;
    auto indexPt = WorldMapView::instance()->m_map->getTileMapPointByViewPoint(realPt);
    return indexPt;
}

bool MinimapViewCCB::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return false;
}

void MinimapViewCCB::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
}

void MinimapViewCCB::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
}

// ITouchDelegate
bool MinimapViewCCB::onBeginScroll(const cocos2d::CCPoint& worldLocation){
//    CCLOG("移动开始X=========%f",worldLocation.x);
//    CCLOG("移动开始Y=========%f",worldLocation.y);
    m_isLongPressBegan=false;
    m_isLongPressTime=0;
    return !m_isOpenMove;;
}

void MinimapViewCCB::onStepScroll(const cocos2d::CCPoint& worldLocation,const cocos2d::CCPoint& delta){
//    CCLOG("移动中X=========%f",worldLocation.x);
//    CCLOG("移动中Y=========%f",worldLocation.y);
    m_isLongPressBegan=false;
    m_isLongPressTime=0;
}

void MinimapViewCCB::onEndScroll(const cocos2d::CCPoint& worldLocation){
//    CCLOG("移动结束X=========%f",worldLocation.x);
//    CCLOG("移动结束Y=========%f",worldLocation.y);
    m_isLongPressBegan=false;
    m_isLongPressTime=0;
}

void MinimapViewCCB::onSingleTouchBegin(const cocos2d::CCPoint& worldLocation){
//    CCLOG("点击开始X=========%f",worldLocation.x);
//    CCLOG("点击开始Y=========%f",worldLocation.y);
    if (!m_isOpenMove)
        return;
    m_isLongPressBegan=true;
    m_isLongPress=false;
    bool isTouchInCity = false;
    int index =int(m_cityNode->getChildrenCount())-1;
    while (index >= 0) {
        auto node = dynamic_cast<CCNode*>(m_cityNode->getChildren().at(index));
        auto size = node->getContentSize();
        CCRect rect = CCRect(node->getPositionX() - size.width / 2, node->getPositionY() - size.height / 2, size.width, size.height);
        if(rect.containsPoint(worldLocation)){
            int cityIndex = node->getTag();
            auto Pos = WorldController::getPointByIndex(cityIndex);
            
            auto ptByIndex = WorldController::getPointByIndex(cityIndex);
            auto disPoint = ccpSub(ptByIndex, WorldMapView::instance()->m_map->centerTilePoint);
            auto worldPt = ccp(WorldMapView::instance()->m_map->centerViewPoint.x+(disPoint.x-disPoint.y)*_halfTileSize.width,WorldMapView::instance()->m_map->centerViewPoint.y-(disPoint.x+disPoint.y)*_halfTileSize.height);
            m_LongPressPos = getPointByWorldPoint(worldPt,m_miniScale);
            isTouchInCity = true;
            break;
        }
        index--;
    }
    if(!isTouchInCity){
        m_LongPressPos = worldLocation;
    }
    m_isLongPressTime=0;
}
void MinimapViewCCB::onSingleTouchEnd(const cocos2d::CCPoint& worldLocation){
//    CCLOG("点击结束X=========%f",worldLocation.x);
//    CCLOG("点击结束Y=========%f",worldLocation.y);
    m_isLongPressBegan=false;
    m_isLongPressTime=0;
    if(!m_isLongPress&&m_isOpenMove)
    {
        this->m_CurrentMapIndex=getMapIndexWithPoint(worldLocation);
        if (this->m_CurrentMapIndex>0&&isOpenLocal==false)
        {
            isOpenLocal=true;
            auto mRenderTexture=m_mapRenderTextureVec[m_CurrentMapIndex-1];
            auto size = CCDirector::sharedDirector()->getWinSize();
            m_viewPort->scrollTo(-mRenderTexture->getPosition() + ccp(size.width / 2, size.height / 2),CCCallFunc::create(this, callfunc_selector(MinimapViewCCB::goToMinimapLocal1)), true);
        }
    }
}

void MinimapViewCCB::onStepZoom(const cocos2d::CCPoint& point1, const cocos2d::CCPoint& point2){
}

void MinimapViewCCB::onBeginZoom(const CCPoint& point1,const CCPoint& point2){
}

void MinimapViewCCB::onEndZoom(float targetZoom){
}

void MinimapViewCCB::moveBack(){
    auto size = CCDirector::sharedDirector()->getWinSize();
    auto pt = m_container->getPosition() - size / 2;
    float s = 6.0f;
    float time = 0.4f;
    CCSpawn *spawn =CCSpawn::createWithTwoActions(CCMoveTo::create(time, pt * s + size / 2), CCScaleTo::create(time, s));
    m_container->runAction(CCSequence::create(spawn,CCCallFunc::create(this, callfunc_selector(MinimapViewCCB::closeSelf)),NULL));
}

void MinimapViewCCB::scaleBack(){
    m_isOpenMove=true;
//    auto centerPt = getRealPosByPoint(viewCenterPoint);
//    auto size = CCDirector::sharedDirector()->getWinSize();
//    CCCallFunc *fun = NULL;
    
//    bool isOpened = CCUserDefault::sharedUserDefault()->getBoolForKey("isMiniMapOpened");
//    if(!isOpened && m_mapType!=DRAGON_TYPE){
        //fun = CCCallFunc::create(this, callfunc_selector(MinimapViewCCB::showResourceCallBack));
        //m_viewPort->scrollTo(-centerPt + ccp(size.width / 2, size.height / 2) - ccp(200, 200), fun, true);
//    }else{
        //fun = CCCallFunc::create(this, callfunc_selector(MinimapView::hideResource));
//    }
}

bool MinimapViewCCB::canShowAlliance(){
    return AllianceManager::getInstance()->checkRight(SEE_OTHER_INWORLD, GlobalData::shared()->playerInfo.allianceInfo.rank);
}

void MinimapViewCCB::setCityColour(int index, Color3B colour)
{
    m_regionSpVec[index-1]->setColor(colour);
}

void MinimapViewCCB::addCity(int index, int type, int level){//0 self 1 city 2 copycat
    // 添加图片
    std::string iconPic = "position_city.png";
    int zOrder = index;
    float s = 1.0f;
    if(type == 0){
        //iconPic = "mini_icon_myCity.png";
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
//    if (((type == 1) || (type == 2) || (type == 4)) && !canShowAlliance()) {
//        return;
//    }
    auto sprite = CCLoadSprite::createSprite(iconPic.c_str());
    m_cityNode->addChild(sprite);
    sprite->setTag(index);
    sprite->setZOrder(zOrder);
    
    CCPoint ptByIndex = WorldController::getPointByIndex(index);
    auto disPoint = ccpSub(ptByIndex, WorldMapView::instance()->m_map->centerTilePoint);
    CCPoint worldPt = ccp(WorldMapView::instance()->m_map->centerViewPoint.x+(disPoint.x-disPoint.y)*_halfTileSize.width,WorldMapView::instance()->m_map->centerViewPoint.y-(disPoint.x+disPoint.y)*_halfTileSize.height);
    auto cityPt = getPointByWorldPoint(worldPt,m_miniScale);

    sprite->setPosition(cityPt);
    sprite->setScale(s);
    sprite->setVisible(index!=0);
}

void MinimapViewCCB::showSelfCity(){
    if(m_currentServerId == GlobalData::shared()->playerInfo.selfServerId){
        addCity(m_mapInfo->selfPt, 0);
    }
    m_isOpenMove=false;
    auto disPoint = ccpSub(WorldMapView::instance()->m_map->currentTilePoint, WorldMapView::instance()->m_map->centerTilePoint);
    CCPoint worldPt = ccp(WorldMapView::instance()->m_map->centerViewPoint.x+(disPoint.x-disPoint.y)*_halfTileSize.width,WorldMapView::instance()->m_map->centerViewPoint.y-(disPoint.x+disPoint.y)*_halfTileSize.height);
    auto currentPt = getPointByWorldPoint(worldPt,m_miniScale);
    auto size = CCDirector::sharedDirector()->getWinSize();
    m_viewPort->scrollTo(-currentPt + ccp(size.width / 2, size.height / 2), NULL);
    
    m_phoneIcon = CCNode::create();
    CCBLoadFile("UIMAPArrow", m_phoneIcon, NULL);
    m_container->addChild(m_phoneIcon, 100);
    m_phoneIcon->setPosition(currentPt);
    this->setUIZorder(m_phoneIcon, 33);
    auto finalContainerPt = m_container->getPosition();
    float s = 6.0f;
    float time = 0.4f;
    float sEnd = 1.0f;
    m_container->setScale(s);
    auto containerPt = (m_container->getPosition() - size / 2) * s + size / 2;
    m_container->setPosition(containerPt);
    CCSpawn *spawn =CCSpawn::createWithTwoActions(CCMoveTo::create(time, finalContainerPt), CCScaleTo::create(time, sEnd));
    m_container->runAction(CCSequence::create(spawn,CCDelayTime::create(0.2),CCCallFunc::create(this, callfunc_selector(MinimapViewCCB::scaleBack)),NULL));
}

void MinimapViewCCB::addAllianceCity()
{
    //大城
    for (int i=1;i<=REGION_COUNT;i++)
    {
        auto miniCityInfo=MiniMapController::getInstance()->getCityPosByOrder(i);
        auto index=WorldController::getIndexByPoint(miniCityInfo->m_Pos);
        int level = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(miniCityInfo->m_CityId))->valueForKey("level")->intValue();
        addCity(index, 1,level);
        if (miniCityInfo->m_ColourId!="")
        {
            setCityColour(i,miniCityInfo->m_Colour);
        }
    }
}

void MinimapViewCCB::goToMinimapLocalBack(Node* sender)
{
    sender->removeFromParent();
    
    auto mRenderTexture=m_mapRenderTextureVec[m_CurrentMapIndex-1];
    auto spr=mRenderTexture->getSprite();
    auto sprBoundingBox=spr->boundingBox();
    auto RenderTextureBoundingBox=mRenderTexture->boundingBox();
    auto mPos=Vec2(RenderTextureBoundingBox.getMinX()+sprBoundingBox.getMinX(),RenderTextureBoundingBox.getMinY()+sprBoundingBox.getMinY());
    auto mSize=Size(sprBoundingBox.size);
    auto mRect=Rect(mPos,mSize);
    
    auto MinimapLocal = MinimapLocalView::create(this,mRect,m_currentServerId,m_CurrentMapIndex);
    PopupViewController::getInstance()->addPopupView(MinimapLocal);
    m_isOpenMove=false;
    m_viewPort->notMove=true;
    //箭头显示
    m_phoneIcon->setVisible(m_isOpenMove);
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
}

void MinimapViewCCB::goToMinimapLocal1()
{
    auto mapSp=m_mapSpVec[m_CurrentMapIndex-1];
    auto mBlink=Blink::create(0.5, 2);
    mapSp->runAction(CCSequence::create(mBlink,CCCallFunc::create(this, callfunc_selector(MinimapViewCCB::goToMinimapLocal2)),NULL));
}

void MinimapViewCCB::goToMinimapLocal2()
{
    auto mapSp=m_mapSpVec[m_CurrentMapIndex-1];
    mapSp->stopAllActions();
    auto Spr=Sprite::createWithSpriteFrameName(string("")+"world_map"+CC_CMDITOA(m_CurrentMapIndex)+".png");
    auto Proportion=mapSp->getContentSize().width/Spr->getContentSize().width;
    Spr->setGlobalZOrder(2);
    Spr->setScale(Proportion);
    Spr->setPosition(mapSp->getPosition());
    mapSp->getParent()->addChild(Spr,2);
    
    auto size = CCDirector::sharedDirector()->getWinSize();
    auto BgSize=m_mapBg->getBoundingBox().size;
    auto offx=m_container->getPosition().x;
    auto x=(-offx+size.width/2)/m_miniScale;
    auto pos=Vec2(x,BgSize.height/2);
    float time = 0.3f;
    float sEnd = 1.0f/m_miniScale;
    CCSpawn *spawn =CCSpawn::createWithTwoActions(CCMoveTo::create(time, pos), CCScaleTo::create(time, sEnd));
    Spr->runAction(CCSequence::create(spawn,CCCallFuncN::create(this, callfuncN_selector(MinimapViewCCB::goToMinimapLocalBack)),NULL));
}


void MinimapViewCCB::gotoCityPos(Vec2 &cityPos)
{
    if(m_isOpenMove)
    {
        auto pos=getPointByTilePos(cityPos);
        auto size = CCDirector::sharedDirector()->getWinSize();
        m_viewPort->scrollTo(-pos + ccp(size.width / 2, size.height / 2), NULL,true);
        if (!m_phoneIcon->isVisible()) {
            m_phoneIcon->setVisible(true);
        }
        m_phoneIcon->setPosition(pos);
    }
   
}

void MinimapViewCCB::IsShowOrdinary(bool isOrdinary)
{
    m_isOrdinary=isOrdinary;
    if(!isOrdinary)
    {
        for (int i=1;i<=REGION_COUNT;i++)
        {
            auto miniCityInfo=MiniMapController::getInstance()->getCityPosByOrder(i);
            if (miniCityInfo->m_ColourId!="")
            {
                setCityColour(i,miniCityInfo->m_Colour);
            }
        }
    }
    m_regionNode->setVisible(!m_isOrdinary);
}

void MinimapViewCCB::setRegionEffects(vector<int>& ciTyOrders)
{
    for (int i=0;i<REGION_COUNT;i++)
    {
        m_regionSpVec[i]->stopAllActions();
        m_regionSpVec[i]->setOpacity(80);
    }
    
    for(int i=0;i<ciTyOrders.size();i++)
    {
        auto index=ciTyOrders.at(i)-1;
        m_regionSpVec[index]->runAction(RepeatForever::create(Sequence::create(FadeTo::create(0.8,80),FadeTo::create(0.8,10),DelayTime::create(0.5),NULL)));
    }
}




