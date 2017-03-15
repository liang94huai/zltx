//
//  PermitionForServicePopupView.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/11/5.
//
//

#include "PermitionForServicePopupView.hpp"
PermitionForServicePopupView* PermitionForServicePopupView::create() {
    auto ret = new PermitionForServicePopupView();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool PermitionForServicePopupView::init() {
    if(PopupBaseView::init()){
        GlobalData::shared()->isBind = true;
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(305, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(7, false);
            CCLoadSprite::doResourceByCommonIndex(305, false);
        });
        
        auto ccb = CCBLoadFile("PermitionForService",this,this);
        this->setContentSize(ccb->getContentSize());
        m_title1->setString(_lang("160241"));
        CCCommonUtils::setButtonTitle(m_btn1, _lang("160248").c_str());
        int totalHeight = 0;
        m_labelNode = Node::create();
        for (int i = 0; i < 6; i++) {//160242~160247
            string contentStr = _lang(CC_ITOA(160242+i));
            m_contentLabel = Label::create();
            m_contentLabel->setDimensions(m_viewContainer1->getContentSize().width, 0);
            m_contentLabel->setColor(ccWHITE);
            m_contentLabel->setFontSize(24);
            m_contentLabel->setVerticalAlignment(kCCVerticalTextAlignmentTop);
            m_contentLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
            m_contentLabel->setAnchorPoint(ccp(0, 1));
            m_contentLabel->setString(contentStr.c_str());
            m_contentLabel->setPosition(ccp(0, totalHeight));
            m_labelNode->addChild(m_contentLabel);
            totalHeight -= m_contentLabel->getContentSize().height;
        }
        
        totalHeight = abs(totalHeight);
        if(totalHeight > m_viewContainer1->getContentSize().height + 5){
            CCScrollView* scroll = CCScrollView::create(m_viewContainer1->getContentSize());
            scroll->setContentSize(CCSize(m_viewContainer1->getContentSize().width, totalHeight));
            scroll->setPosition(ccp(0, 0));
            scroll->setDirection(kCCScrollViewDirectionVertical);
            scroll->setAnchorPoint(ccp(0, 0));
            m_viewContainer1->addChild(scroll);
            scroll->addChild(m_labelNode);
            m_labelNode->setPosition(ccp(0, scroll->getContentSize().height));
            scroll->setContentOffset(ccp(0, m_viewContainer1->getContentSize().height-totalHeight));
        }else{
            m_viewContainer1->addChild(m_labelNode);
            m_labelNode->setPosition(ccp(0, m_viewContainer1->getContentSize().height));
        }

        return true;
        
    }
    return false;
}

bool PermitionForServicePopupView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title1", Label*, this->m_title1)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewContainer1", Node*, this->m_viewContainer1)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn1", ControlButton*, this->m_btn1)
    return false;
}

SEL_CCControlHandler PermitionForServicePopupView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn1Click", PermitionForServicePopupView::onBtn1Click);
    return NULL;
}
void PermitionForServicePopupView::onBtn1Click(CCObject * pSender, Control::EventType pCCControlEvent) {
    CCUserDefault::sharedUserDefault()->setStringForKey("service_terms_check", "ok");
    CCUserDefault::sharedUserDefault()->flush();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(PERMITION_CHECK_CONFIRM);
    this->closeSelf();
}
void PermitionForServicePopupView::onEnter() {
    PopupBaseView::onEnter();
}

void PermitionForServicePopupView::onExit() {
    GlobalData::shared()->isBind = false;
    PopupBaseView::onExit();
}