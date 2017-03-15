//
//  CCLineBatchedSprite.cpp
//  IF
//
//  Created by Luojunfeng on 14/12/25.
//
//

#include "CCLineBatchedSprite.h"
#include "WorldMapView.h"

NS_CC_BEGIN

#define STRINGIFY(A)  #A
const static GLchar* UVAnimationVert = STRINGIFY
(
#ifdef GL_ES
 precision highp float;
#endif
 
 attribute vec4 a_position;
 attribute vec2 a_texCoord;
 attribute vec4 a_color;
 
 varying vec4 v_fragmentColor;
 varying vec2 v_texCoord;
 
 void main()
{
    gl_Position = CC_MVPMatrix * a_position;
    v_fragmentColor = a_color;
    v_texCoord = a_texCoord;
});

const static GLchar* UVAnimationFrag = STRINGIFY
(
#ifdef GL_ES
 precision highp float;
#endif
 
 varying vec4 v_fragmentColor;
 varying vec2 v_texCoord;
 //uniform sampler2D CC_Texture0;
 
 void main()
{
    gl_FragColor =  v_fragmentColor * texture2D(CC_Texture0, v_texCoord);
});


bool CCLineBatchedSprite::getCrossPoint(const CCPoint& a,const CCPoint& b,const CCPoint& c,const CCPoint& d,CCPoint &p){
    // 三角形abc 面积的2倍
    float area_abc = (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
    // 三角形abd 面积的2倍
    float area_abd = (a.x - d.x) * (b.y - d.y) - (a.y - d.y) * (b.x - d.x);
    // 面积符号相同则两点在线段同侧,不相交 (对点在线段上的情况,本例当作不相交处理);
    if ( area_abc*area_abd>=0 ) {
        return false;
    }
    // 三角形cda 面积的2倍
    float area_cda = (c.x - a.x) * (d.y - a.y) - (c.y - a.y) * (d.x - a.x);
    // 三角形cdb 面积的2倍
    // 注意: 这里有一个小优化.不需要再用公式计算面积,而是通过已知的三个面积加减得出.
    float area_cdb = area_cda + area_abc - area_abd ;
    if (  area_cda * area_cdb >= 0 ) {
        return false;
    }
    //计算交点坐标
    float t = area_cda / (area_abd-area_abc);
    float dx= t*(b.x - a.x);
    float dy= t*(b.y - a.y);
    p = ccp(a.x+dx,a.y+dy);
    return true;
}
bool CCLineBatchedSprite::init(CCTexture2D* texture) {
    
    
    if (!texture) {
        return false;
    }
    
    if (!CCSprite::initWithTexture(texture)) {
        return false;
    }
    
    if (m_lineHeight == FLT_MIN) {
        m_lineHeight = texture->getPixelsHigh();
    }
    Texture2D::TexParams texParams = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
    texture->setTexParameters(&texParams);
    this->setTextureRect(CCRectZero);
    this->setAnchorPoint(ccp(0,0.5));
    
    //    m_lineWidth = m_start.getDistance(m_end);
    //    m_spScale = m_lineHeight / texture->getPixelsHigh();
    //    m_spHalfWidth = texture->getPixelsWide() / 2;
    //    CCPoint vs = ccpSub(m_start, m_end);
    //    this->setRotation(180.0f * (1 - atan2f(vs.y, vs.x)/ M_PI));
    //    this->setAnchorPoint(ccp(0,0.5));
    //    this->setTextureRect(CCRectZero);
    //    this->setScale(m_spScale);
    //    this->setPosition(m_start);
    //
    //    unsigned int cellW = texture->getPixelsWide();
    //    int totalW = (int)(m_lineWidth / m_spScale);
    //    int currentW = 0;
    //    int w = 0;
    //    int cellX = 0;
    //    int cellY = 0;
    //    while (w < MAX_LENGTH_PER_LINE && currentW <= totalW) {
    //        w += cellW;
    //        currentW +=cellW;
    //        if(w >= MAX_LENGTH_PER_LINE){
    //            auto cell = CCLineBatchedSpriteCell::create(m_updatingSpeed, m_spHalfWidth, CCRect(0, 0, w, texture->getPixelsHigh()));
    //            cell->setAnchorPoint(ccp(0, 0.5));
    //            cell->setPosition(ccp(cellX, cellY));
    //            cellX += MAX_LENGTH_PER_LINE;
    //            this->addChild(cell);
    //            if(currentW > totalW){
    //                break;
    //            }
    //            w = 0;
    //        }
    //        if(currentW > totalW){
    //            float lineW = w - (currentW - totalW);
    //            auto cell = CCLineBatchedSpriteCell::create(m_updatingSpeed, m_spHalfWidth, CCRect(0, 0, lineW, texture->getPixelsHigh()));
    //            cell->setAnchorPoint(ccp(0, 0.5));
    //            cell->setPosition(ccp(cellX, cellY));
    //            this->addChild(cell);
    //            cellX += lineW;
    //            break;
    //        }
    //    }
    return true;
}

void CCLineBatchedSprite::onEnter() {
    CCNode::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void CCLineBatchedSprite::onExit() {
    CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget(this);
    CCNode::onExit();
}

void CCLineBatchedSprite::setColor(const ccColor3B& color3){
    CCSprite::setColor(color3);
    //    auto arr = this->getChildren();
    //    for (auto child : arr)
    //    {
    //        auto cell = dynamic_cast<CCLineBatchedSpriteCell*>(child);
    //        if(cell){
    //            cell->setColor(color3);
    //        }
    //    }
}

static vector<CCPoint> crossPos;
void CCLineBatchedSprite::update(float delta) {
    
    if( !WorldMapView::instance() ) {
        return;
    }
    
    CCPoint sPos = WorldMapView::instance()->m_drawRoadNode->convertToWorldSpace(m_start);
    CCPoint ePos = WorldMapView::instance()->m_drawRoadNode->convertToWorldSpace(m_end);
    
    CCSize winSize(CCDirector::sharedDirector()->getWinSize());
    CCPoint xOrg0(0,0);
    CCPoint xOrg1(0,winSize.height);
    CCPoint yOrg2(winSize.width,0);
    CCPoint yOrg3(winSize.width,winSize.height);
    crossPos.clear();
    // 与y=0的交点
    CCPoint pCross1(0,0);
    if(getCrossPoint(sPos,ePos,xOrg1,xOrg0,pCross1)){
        crossPos.push_back(pCross1);
    }
    // 与y=winSize.width的交点
    CCPoint pCross2(0,0);
    if(getCrossPoint(sPos,ePos,yOrg3,yOrg2,pCross2)){
        crossPos.push_back(pCross2);
    }
    // 与x=0的交点
    CCPoint pCross3(0,0);
    if(getCrossPoint(sPos,ePos,xOrg0,yOrg2,pCross3)){
        crossPos.push_back(pCross3);
    }
    // 与x=winSize.height的交点
    CCPoint pCross4(0,0);
    if(getCrossPoint(sPos,ePos,xOrg1,yOrg3,pCross4)){
        crossPos.push_back(pCross4);
    }
    float maxY = MAX(MAX(MAX(xOrg0.y,xOrg1.y),yOrg2.y),yOrg3.y);
    float minY = MIN(MIN(MIN(xOrg0.y,xOrg1.y),yOrg2.y),yOrg3.y);
    float maxX = MAX(MAX(MAX(xOrg0.x,xOrg1.x),yOrg2.x),yOrg3.x);
    float minX = MIN(MIN(MIN(xOrg0.x,xOrg1.x),yOrg2.x),yOrg3.x);
    if(crossPos.size()==0){
        if((sPos.x>=minX && sPos.x<=maxX && sPos.y>=minY && sPos.y<=maxY) || (ePos.x>=minX && ePos.x<=maxX && ePos.y>=minY && ePos.y<=maxY)){
            crossPos.push_back(sPos);
            crossPos.push_back(ePos);
        }
    }else if(crossPos.size()==1){
        if((sPos.x>=minX && sPos.x<=maxX && sPos.y>=minY && sPos.y<=maxY)){
            crossPos.push_back(sPos);
        }else if((ePos.x>=minX && ePos.x<=maxX && ePos.y>=minY && ePos.y<=maxY)){
            crossPos.push_back(ePos);
        }
    }
    
    CCPoint sPos1,ePos1;
    float offsetStartDis = 0.0f;
    if(crossPos.size()>1){
        float dis1 = sPos.getDistance(crossPos.at(0));
        float dis2 = sPos.getDistance(crossPos.at(1));
        if(dis1>dis2){
            sPos1 = WorldMapView::instance()->m_drawRoadNode->convertToNodeSpace(crossPos.at(1));
            ePos1 = WorldMapView::instance()->m_drawRoadNode->convertToNodeSpace(crossPos.at(0));
            offsetStartDis = dis2;
        }else{
            sPos1 = WorldMapView::instance()->m_drawRoadNode->convertToNodeSpace(crossPos.at(0));
            ePos1 = WorldMapView::instance()->m_drawRoadNode->convertToNodeSpace(crossPos.at(1));
            offsetStartDis = dis1;
        }
        
        m_spScale = m_lineHeight / getTexture()->getPixelsHigh();
        m_spHalfWidth = getTexture()->getPixelsWide() / 2;
        this->setScale(m_spScale);
        this->setPosition(sPos1);
        CCPoint vs = ccpSub(sPos1, ePos1);
        this->setRotation(180.0f * (1 - atan2f(vs.y, vs.x)/ M_PI));
        
        m_lineWidth = (int)(sPos1.getDistance(ePos1) / m_spScale);
        float lineOffsetStartDis = offsetStartDis / m_spScale;
        float NOUSE;
        float temp = lineOffsetStartDis / (m_spHalfWidth * 2);
        float modoffset = modf(temp, &NOUSE);
        float startRectLeft = modoffset * m_spHalfWidth * 2;

//        if(m_lineWidth!=m_lastLineWidth){
//            //m_rect = CCRect(0, 0, m_lineWidth, getTexture()->getPixelsHigh());
//            m_lastLineWidth = m_lineWidth;
//            CCLOG("startorg[%d,%d],endorg[%d,%d],start[%d,%d],end[%d,%d]",(int)sPos.x,(int)sPos.y,(int)ePos.x,(int)ePos.y,(int)sPos1.x,(int)sPos1.y,(int)ePos1.x,(int)ePos1.y);
//            CCLOG("speed[%f],half_w[%f]",m_updatingSpeed,m_spHalfWidth);
//        }
        
        m_rect.setRect(startRectLeft, 0, m_lineWidth, getTexture()->getPixelsHigh());
        m_percentStack += delta * m_updatingSpeed / m_spHalfWidth;
        m_percentStack = m_percentStack >= 1.000 ? m_percentStack - 1.000 : m_percentStack;
        
        float offset = m_percentStack * m_spHalfWidth * 2;
        m_rect.origin.x += -(int)(offset);
        this->setTextureRect(m_rect);
        if(!this->isVisible()){
            this->setVisible(true);
        }
    }else{
        m_rect.setRect(0, 0, 0, 0);
        this->setTextureRect(CCRectZero);
        m_lastLineWidth=0;
        m_percentStack=0;
        this->setVisible(false);
    }
}
//void CCLineBatchedSprite::changeUpdatingSpeed(float speed) {
//    if (fabsf(speed - m_updatingSpeed) < 0.001) {
//        return;
//    }
//    m_updatingSpeed = speed;
//    m_percentStack = 0;
//}

//----- NEVER USER FOLLOW class ----
CCLineBatchedSpriteCell *CCLineBatchedSpriteCell::create(float updatingSpeed, float halfW, CCRect rect){
    CCLineBatchedSpriteCell *ret = new CCLineBatchedSpriteCell(updatingSpeed, halfW, rect);
    if(ret && ret->init()){
        
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool CCLineBatchedSpriteCell::init(){
    return CCSprite::init();
}

void CCLineBatchedSpriteCell::setSpeed(float speed){
    m_updatingSpeed = speed;
}

void CCLineBatchedSpriteCell::onEnter(){
    CCNode::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void CCLineBatchedSpriteCell::onExit(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleUpdateForTarget(this);
    CCNode::onExit();
}

void CCLineBatchedSpriteCell::update(float speed){
    if (m_updatingSpeed == FLT_MIN) {
        return;
    }
    
    m_percentStack += speed * m_updatingSpeed / m_spHalfWidth;
    m_percentStack = m_percentStack >= 1.000 ? m_percentStack - 1.000 : m_percentStack;
    
    float offset = m_percentStack * m_spHalfWidth * 2;
    m_rect.origin.x = -(int)(offset);
    this->setTextureRect(m_rect);
    
    CCPoint cellPos = this->getPosition();
    CCSize cellSize = this->getContentSize();
    
    CCPoint lb = this->getParent()->convertToWorldSpace(cellPos);
    CCPoint lt = this->getParent()->convertToWorldSpace(cellPos+ ccp(0,cellSize.height));
    CCPoint rt = this->getParent()->convertToWorldSpace(cellPos+ ccp(cellSize.width,cellSize.height));
    CCPoint rb = this->getParent()->convertToWorldSpace(cellPos+ ccp(cellSize.width,0));
    
    float maxY = MAX(MAX(MAX(lb.y,lt.y),rt.y),rb.y);
    float minY = MIN(MIN(MIN(lb.y,lt.y),rt.y),rb.y);
    float maxX = MAX(MAX(MAX(lb.x,lt.x),rt.x),rb.x);
    float minX = MIN(MIN(MIN(lb.x,lt.x),rt.x),rb.x);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCRect viewRect = CCRectMake(0, 0, winSize.width, winSize.height);
    if(minY > winSize.height || maxX < 0 || minX > winSize.width || maxY < 0){
        this->setVisible(false);
    }else{
        this->setVisible(true);
    }
    CCLOG("CCLineBatchedSpriteCell_______idx[%d] pos[%f,%f], size[%f,%f] , max[%f,%f], min[%f,%f] \n",this->getTag(),cellPos.x,cellPos.y,cellSize.width,cellSize.height,maxX,maxY,minX,minY);
}

//----- NEVER USER ABOVE class ----

bool CCLineBatchedLayer::init() {

// tips don't use shader and batched layer.
//    this->batchlayer = CCSpriteBatchNode::create(FOOTPRINTIMGPATH, 50);
//    CCGLProgram* p = CCShaderCache::sharedShaderCache()->addCustomGLProgram("LineBatchedLayer", UVAnimationVert, UVAnimationFrag);
//    this->batchlayer->setShaderProgram(p);

//#if CC_ENABLE_CACHE_TEXTURE_DATA
//    // Need to listen the event only when not use batchnode, because it will use VBO
//    auto listener = EventListenerCustom::create(EVENT_RENDERER_RECREATED, [this](EventCustom* event){
//        /** listen the event that renderer was recreated on Android/WP8 */
//        CCLOG("Tongyue LineBatchedLayer reload");
//        CCGLProgram* p = CCShaderCache::sharedShaderCache()->reloadCustomGLProgram("LineBatchedLayer", UVAnimationVert, UVAnimationFrag);
//        //this->batchlayer->setShaderProgram(p);
//    });
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
//#endif
    this->batchlayer = CCNode::create();
    CCNode::addChild(this->batchlayer);
    return true;
}

void CCLineBatchedLayer::addChild(CCLineBatchedSprite* child) {
    batchlayer->addChild(child);
}

CCNode* CCLineBatchedLayer::getChildByTag(int tag) {
    return batchlayer->getChildByTag(tag);
}

void CCLineBatchedLayer::removeAllChildren() {
    batchlayer->removeAllChildren();
}

NS_CC_END