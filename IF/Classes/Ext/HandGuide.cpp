//
//  HandGuide.cpp
//  IF
//
//  Created by 王超一 on 15/11/12.
//
//

#include "HandGuide.hpp"

bool HandGuide::init(){
    m_hand = CCLoadSprite::createSprite("UI_hand.png");
    m_hand->setAnchorPoint(ccp(0, 0));
    if (CCCommonUtils::isIosAndroidPad() && CCCommonUtils::getIsHDViewPort())
    {
        m_hand->setScale(2.f);
    }
    this->addChild(m_hand);
    
    return true;
}

void HandGuide::play(CCPoint pos, int w, int h, int d){
    m_direction = d;
    if(m_direction < UP){
        m_direction = UP;
    }
    m_pos = pos;
    
    if(w == 0){
        w = 100;
    }
    if(h == 0){
        h = 100;
    }
    m_hand->stopAllActions();
    float  time = 0.5f;
    int offset = 50;
    CCMoveTo* move1 = NULL;
    CCMoveTo* move2 = NULL;
    
    switch(m_direction){
        case UP:
            m_hand->setRotation(90);
            if (CCCommonUtils::isIosAndroidPad()&& CCCommonUtils::getIsHDViewPort())
            {
                m_pos.y = m_pos.y-m_hand->getContentSize().height/2;
            }
            m_hand->setPosition(ccp(m_pos.x + w / 2, m_pos.y - m_hand->getContentSize().height / 2 - offset));
            move1= CCMoveTo::create(time, ccp(m_pos.x + w / 2, m_pos.y - m_hand->getContentSize().height / 2));
            move2= CCMoveTo::create(time, ccp(m_pos.x + w / 2, m_pos.y - m_hand->getContentSize().height / 2 - offset));
            
            break;
        case DOWN:
            m_hand->setRotation(0);
            if (CCCommonUtils::isIosAndroidPad()&& CCCommonUtils::getIsHDViewPort())
            {
                m_pos.y = m_pos.y+m_hand->getContentSize().height/2;
            }
            m_hand->setPosition(ccp(m_pos.x + w / 2, m_pos.y + m_hand->getContentSize().height / 2 + offset + h));
            move1= CCMoveTo::create(time, ccp(m_pos.x + w / 2, m_pos.y + m_hand->getContentSize().height / 2 + h));
            move2= CCMoveTo::create(time, ccp(m_pos.x + w / 2, m_pos.y + m_hand->getContentSize().height / 2 + offset + h));
            
            break;
        case LEFT:
            if (CCCommonUtils::isIosAndroidPad()&& CCCommonUtils::getIsHDViewPort())
            {
                m_pos.x = m_pos.x+m_hand->getContentSize().height/2;
            }
            m_hand->setPosition(ccp(m_pos.x + w / 2 + m_hand->getContentSize().width, m_pos.y + h / 2));
            move1= CCMoveTo::create(time, ccp(m_pos.x + offset + w / 2 + m_hand->getContentSize().width, m_pos.y + h / 2));
            move2= CCMoveTo::create(time, ccp(m_pos.x + w / 2 + m_hand->getContentSize().width, m_pos.y + h / 2));
            m_hand->setRotation(90);
            break;
        case RIGHT:
            if (CCCommonUtils::isIosAndroidPad()&& CCCommonUtils::getIsHDViewPort())
            {
                m_pos.x = m_pos.x-m_hand->getContentSize().height/2;
            }
            m_hand->setRotation(-90);
            m_hand->setPosition(ccp(m_pos.x - m_hand->getContentSize().width / 2, m_pos.y + h / 2));
            move1= CCMoveTo::create(time, ccp(m_pos.x - offset - m_hand->getContentSize().width / 2, m_pos.y + h / 2));
            move2= CCMoveTo::create(time, ccp(m_pos.x - m_hand->getContentSize().width / 2, m_pos.y + h / 2));
            break;
    }
    if(move1 && move2) {
        m_hand->setVisible(true);
        m_hand->runAction(CCRepeatForever::create(CCSequence::create(move1,move2,NULL)));
    }
    else {
        m_hand->setVisible(false);
    }
}

void HandGuide::stop(){
    m_hand->stopAllActions();
}

HandGuide* HandGuide::create(float delayPerUnit){
    HandGuide *pRet = new HandGuide(delayPerUnit);
    if (pRet && pRet->init())
    {
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}