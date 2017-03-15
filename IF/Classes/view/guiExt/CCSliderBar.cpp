//
//  CCSliderBar.cpp
//  IF
//
//  Created by ganxiaohua on 14-6-23.
//
//

#include "CCSliderBar.h"

CCSliderBar* CCSliderBar::createSlider(CCScale9Sprite* backgSprite, CCSprite* pogressSprite, CCSprite* thumbSprite){
    CCSliderBar* slider = new CCSliderBar();
    CCSprite* spr = CCSprite::create();
    spr->setContentSize(backgSprite->getContentSize());
    slider->initWithSprites(spr, pogressSprite, thumbSprite);
    slider->addChild(backgSprite,-1000);
    slider->m_bgWidth = backgSprite->getContentSize().width;
    backgSprite->setPosition(ccp(slider->getContentSize().width / 2, slider->getContentSize().height / 2));
    slider->autorelease();
    return slider;
}

void CCSliderBar::needsLayout(){
    if (NULL == this->getThumbSprite() || NULL == this->getBackgroundSprite() || NULL == this->getProgressSprite())
    {
        return;
    }
    // Update thumb position for new value
    float percent               = (_value - _minimumValue) / (_maximumValue - _minimumValue);
    
    CCPoint pos                 = this->getThumbSprite()->getPosition();
    pos.x                       = percent * (m_bgWidth-m_limitMoveValue*2)+m_limitMoveValue;
    this->getThumbSprite()->setPosition(pos);
    this->getSelectedThumbSprite()->setPosition(pos);
    this->getProgressSprite()->setScaleX(percent*m_maxScaleX);
}

void CCSliderBar::setProgressScaleX(float scaleX){
    m_maxScaleX = scaleX;
    this->needsLayout();
}

bool CCSliderBar::isTouch(CCNode* pNode, CCTouch* touch, int addY/*=0*/)
{
    if (!pNode || !pNode->getParent())
        return false;
    CCPoint touchLocation=pNode->getParent()->convertToNodeSpace(touch->getLocation());
    CCRect bBox=pNode->boundingBox();
    bBox.origin.y-=addY;
    return bBox.containsPoint(touchLocation);
}

bool CCSliderBar::onTouchBegan(CCTouch* touch, CCEvent* pEvent)
{
    if (!isTouchInside(touch) || !isEnabled() || !isVisible())
    {
        return false;
    }
    //    if(!isTouch(getThumbSprite(),touch)){
    //        CCPoint location = locationFromTouch(touch);
    //        sliderBegan(location);
    //        sliderEnded(CCPointZero);
    //        return false;
    //    }
    saveP = CCPointZero;
    if(!isTouch(getThumbSprite(),touch) && isPrecies){
        if (isTouch(getBackgroundSprite(),touch,8)) {
            m_touchEnd = false;
            saveP = locationFromTouch(touch);
            return true;
        }
        return false;
    }
    
    m_touchEnd = false;
    CCPoint location = locationFromTouch(touch);
    sliderBegan(location);
    return true;
}

void CCSliderBar::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (saveP.equals(CCPointZero)||(!isPrecies)) {
        CCPoint location = locationFromTouch(pTouch);
        sliderMoved(location);
    }
}

void CCSliderBar::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isPrecies) {
        CCPoint location = locationFromTouch(pTouch);
        float distance = location.getDistance(saveP);
        if (distance<3) {
            sliderBegan(location);
        }
    }
    m_touchEnd = true;
    sliderEnded(CCPointZero);
}

float CCSliderBar::valueForLocation(CCPoint location)
{
    float percent = (location.x-m_limitMoveValue)/ (getBackgroundSprite()->getContentSize().width-m_limitMoveValue*2);
    return MAX(MIN(_minimumValue + percent * (_maximumValue - _minimumValue), _maximumAllowedValue), _minimumAllowedValue);
}