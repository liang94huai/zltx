//
//  ScrollTableView.cpp
//  IF
//
//  Created by ganxiaohua on 14-5-14.
//
//

#include "ScrollTableView.h"
#include "ArcScrollView.h"

ScrollTableView::ScrollTableView(){
    m_clickTime = 0;
    m_tTouchTime = 0;
    touchArea = NULL;
    clickArea = NULL;
    m_aniStatus = true;
    m_index = 0;
    m_arrIndex = 0;
}

ScrollTableView::~ScrollTableView()
{}

ScrollTableView* ScrollTableView::create(CCSize size, int cellH, int num)
{
    ScrollTableView *table = new ScrollTableView();
    table->initWithViewSize(size);
    table->autorelease();
    table->m_cellH = cellH;
    table->m_num = num;
    return table;
}

void ScrollTableView::setContentOffset(CCPoint offset, bool animated){
    if(m_touchEnd){
        scrollFinish();
        this->getContainer()->unscheduleAllSelectors();
        this->getContainer()->stopAllActions();
        unsigned long time = clock()/1000;
        float speed  = 0;
        if (time - m_tTouchTime <= 0) {
            gapY = 0;
        }
        else{
            speed = fabsf(gapY)/(time - m_tTouchTime);//这是防止按太长时间，还会运动的问题
            CCLOG("gapY:%f;  speed:%f;  time:%ld",fabsf(gapY),speed,(time - m_tTouchTime));
            float speedFlag = 0.4f;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            speedFlag = 0.9f;
#else
#endif
            if(speed < speedFlag)
            {
                gapY = 0;
            }
            
        }
        if(fabsf(gapY)>20){//是否可以惯性滚动
            this->schedule(schedule_selector(ScrollTableView::scrollingPos));
        }else{
            if(m_canClickMove && fabsf(gapY)<10 && gapY!=0){
                if(worldPoint.y>m_endPoint.y){
                    moveOneCell(true);
                }else{
                    moveOneCell(false);
                    
                }
            }else{
                setScrollPostion();
            }
        }
    }else{
        CCScrollView::setContentOffset(offset,animated);
    }
}

void ScrollTableView::scrollFinish(CCNode* p){
    if (_delegate != NULL)
    {
        _delegate->scrollViewDidScroll(this);
    }
}

void ScrollTableView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (!this->isVisible())
    {
        return;
    }
    m_touchEnd = true;
    m_endPoint = pTouch->getLocation();
    auto touchIter = std::find(_touches.begin(), _touches.end(), pTouch);
    if (touchIter != _touches.end())
    {
        if (_touches.size() == 1 && _touchMoved){
            //this->schedule(schedule_selector(CCScrollView::deaccelerateScrolling));
            gapY =  this->getContentOffset().y - m_touchBeganPoint.y;
            gapY = MIN(gapY,100);//200
            gapY = MAX(gapY,-100);//-200
            _dragging = false;
            float newX, newY;
            CCPoint maxInset, minInset;
            _container->setPosition(ccpAdd(_container->getPosition(), _scrollDistance));
            
            maxInset = this->maxContainerOffset();
            minInset = this->minContainerOffset();
            //check to see if offset lies within the inset bounds
            newX     = MIN(_container->getPosition().x, maxInset.x);
            newX     = MAX(newX, minInset.x);
            newY     = MIN(_container->getPosition().y, maxInset.y);
            newY     = MAX(newY, minY);

            this->setContentOffset(ccp(newX,newY),true);
        }else{
            if(m_canClickMove){
                if(worldPoint.y>m_startPoint.y){
                    moveOneCell(true);
                }else{
                    moveOneCell(false);
                }
            }else{
                //没有动
                m_aniStatus = true;
            }
        }
        _touches.erase(touchIter);
    }
    if(_touches.size() == 0)
    {
        _dragging = false;
        _touchMoved = false;
    }
}

void ScrollTableView::scrollingPos(float dt){
    CCLOG("dt=%f",dt);
    float subFactor = 0.4;//0.8
    gapY = subFactor*gapY;
    _container->setPosition(ccp(_container->getPositionX(),_container->getPositionY()+gapY*0.5));
    this->scrollFinish();
    if (fabsf(gapY) <= 0.00001 || _container->getPositionY()<-500 || _container->getPositionY()>500)
    {
        this->unschedule(schedule_selector(ScrollTableView::scrollingPos));
        this->setScrollPostion();
    }
}

void ScrollTableView::moveOneCell(bool up){
    auto & array = _container->getChildren();
//    int num = array.size();
    int minGap = 9999;
    for(auto child:array)
    {
        ArcScrollCell* cell = dynamic_cast<ArcScrollCell*>(child);
        if(cell->getWorldGapY()<minGap){
            minGap = cell->getWorldGapY();
            m_index = cell->m_info->m_index;
        }
    }
//    
//    for(int i=0;i<num;i++){
//        ArcScrollCell* cell = dynamic_cast<ArcScrollCell*>(array->objectAtIndex(i));
//        if(cell->getWorldGapY()<minGap){
//            minGap = cell->getWorldGapY();
//            m_index = cell->m_info->m_index;
//        }
//    }
    m_index = m_num - m_index -1;
    if(up){
        m_index = MAX(m_index-1,0);
    }else{
        m_index = MIN(m_index+1,m_num-1);
    }
    CCPoint pos;
    if(m_num%2==1){
        pos = ccp(0, (m_num/2-m_index)*m_cellH + m_cellH/2.0);
    }else{
        pos = ccp(0, (m_num/2-m_index)*m_cellH + m_cellH/2.0);
    }
    CCMoveTo* scroll = CCMoveTo::create(0.3, pos);
    CCActionInterval* action = CCEaseBackOut::create(scroll);
    this->scrollFinish();
    CCSequence* sc1 = CCSequence::create(action,CCCallFuncN::create(this, callfuncN_selector(ScrollTableView::scrollAniEnd)),NULL);
    _container->runAction(sc1);
}

void ScrollTableView::setScrollPostion(){
    auto & array = _container->getChildren();
//    int num = array->count();
    int minGap = 9999;
    int lastIndex = m_index;
    for(auto child:array)
    {
        ArcScrollCell* cell = dynamic_cast<ArcScrollCell*>(child);
        if(cell->getWorldGapY()<minGap){//取离中心点最近的点
            minGap = cell->getWorldGapY();
            m_index = cell->m_info->m_index;
        }
    }
//    for(int i = 0 ; i<num ; i++){
//        ArcScrollCell* cell = dynamic_cast<ArcScrollCell*>(array->objectAtIndex(i));
//        if(cell->getWorldGapY()<minGap){//取离中心点最近的点
//            minGap = cell->getWorldGapY();
//            m_index = cell->m_info->m_index;
//        }
//    }
    m_index = m_num - m_index -1;
    if(lastIndex==m_index && fabs(m_endPoint.y - m_startPoint.y)>30){
        if(m_endPoint.y - m_startPoint.y>0){
            m_index = m_index - 1;
        }else{
            m_index = m_index + 1;
        }
        m_index = MIN(m_index, m_num  -1);
        m_index = MAX(m_index, 0);
    }
    CCPoint pos;
    if(m_num%2==1){
        pos = ccp(0, (m_num/2-m_index)*m_cellH + m_cellH/2.0);
    }else{
        pos = ccp(0, (m_num/2-m_index)*m_cellH + m_cellH/2.0);
    }
    CCMoveTo* scroll = CCMoveTo::create(0.3, pos);
    CCActionInterval* action = CCEaseBackOut::create(scroll);
    this->scrollFinish();
    CCSequence* sc1 = CCSequence::create(action,CCCallFuncN::create(this, callfuncN_selector(ScrollTableView::scrollAniEnd)),NULL);
    _container->runAction(sc1);
}

void ScrollTableView::scrollAniEnd(CCNode* param){
    this->scrollFinish();
    m_aniStatus = true;
}

bool ScrollTableView::onTouchBegan(CCTouch* touch, CCEvent* event){
    if(touchArea==NULL || !isTouchInside(touchArea, touch)) return  false;
    if (!this->isVisible())
    {
        return false;
    }
    m_startPoint = touch->getLocation();
    m_touchEnd = false;
    m_canClickMove = false;
    m_moveUp = false;
    unsigned long currentTime = clock()/1000;
    unsigned long gapTime = currentTime-m_clickTime;
    if(gapTime<300){
        return false;
    }
    m_clickTime = currentTime;
    if(clickArea!=NULL && !isTouchInside(clickArea, touch)){
         m_canClickMove = true;
    }
    
    m_tTouchTime = m_clickTime;
    CCRect frame = getViewRect();
    
    //dispatcher does not know about clipping. reject touches outside visible bounds.
    if (_touches.size() > 2 ||
        _touchMoved||
        !frame.containsPoint(_container->convertToWorldSpace(_container->convertTouchToNodeSpace(touch))))
    {
        return false;
    }
    
    if (std::find(_touches.begin(), _touches.end(), touch) == _touches.end())
    {
        _touches.push_back(touch);
    }
    
    if (_touches.size() == 1)
    { // scrolling
        _touchPoint     = this->convertTouchToNodeSpace(touch);
        _touchMoved     = false;
        _dragging     = true; //dragging started
        _scrollDistance = Vec2(0.0f, 0.0f);
        _touchLength    = 0.0f;
    }
    else if (_touches.size()  == 2)
    {
        _touchPoint = (this->convertTouchToNodeSpace(_touches[0]).getMidpoint(
                                                                              this->convertTouchToNodeSpace(_touches[1])));
        
        _touchLength = _container->convertTouchToNodeSpace(_touches[0]).getDistance(
                                                                                    _container->convertTouchToNodeSpace(_touches[1]));
        _dragging  = false;
    }
    m_touchBeganPoint = this->getContentOffset();
    m_aniStatus = false;
    return true;
}

void ScrollTableView::onTouchMoved(CCTouch* touch, CCEvent* event)
{
    CCScrollView::onTouchMoved(touch,event);
}

CCPoint ScrollTableView::minContainerOffset(){
    int offY = 0;
    if(m_num%2==0){
        offY = abs((int)initPos.y);
    }else{
        offY = abs((int)initPos.y) + m_cellH;
    } 
    return ccp(_viewSize.width - _container->getContentSize().width*_container->getScaleX(),
               _viewSize.height - _container->getContentSize().height*_container->getScaleY() - 1000);//530
}

CCPoint ScrollTableView::maxContainerOffset(){
    int offy = 0;
    if(m_num%2==1){
       offy = m_cellH*m_num/2;
    }else{
        offy = m_cellH*(m_num+1)/2;
    }
    return ccp(0.0f, offy);
}