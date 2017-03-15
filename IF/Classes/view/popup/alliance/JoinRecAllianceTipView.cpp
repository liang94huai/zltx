//
//  JoinRecAllianceTipView.cpp
//  IF
//
//  Created by 付彬 on 15/8/13.
//
//

#include "JoinRecAllianceTipView.h"
#include "SceneController.h"
#include "YesNoDialog.h"
#include "SoundController.h"
#include "AllianceManager.h"
#include "ApplyAllianceCommand.h"
#include "CCMathUtils.h"
#include "CreateAllianceView.h"
#include "PopupViewController.h"

using namespace cocos2d;

const char* iconMan[5] = {"g008","g024","g026","g032","g041"};
const char* iconWoman[6] = {"g007","g012","g015","g038","g044","g046"};
const char* msg[5] = {"102386", "102386", "102389", "102391", "102393"};

JoinRecAllianceTipView* JoinRecAllianceTipView::create()
{
    JoinRecAllianceTipView *pRet = new JoinRecAllianceTipView();
    if (pRet && pRet->initJoinRecAllianceTipView())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool JoinRecAllianceTipView::initJoinRecAllianceTipView()
{
    _clickType = 0;
    if (AllianceManager::getInstance()->recAllianceType == 1) { //推荐创建联盟
        CCBLoadFile("JoinRecAllianceTipView",this,this);
        AllianceManager::getInstance()->recAllianceType = 2;
        _clickType = 1;
        m_allianceLabel->setString("");
        m_nameLabel->setString(_lang("3000002"));
        CCCommonUtils::setButtonTitle(m_joinBtn, _lang("115007").c_str());
        if ( rand()%2 == 0) {
            m_infoLabel->setString( _lang("115580") );
        }else {
            m_infoLabel->setString( _lang("115581") );
        }
        
        auto icon = CCLoadSprite::createSprite("guidePlayerIcon.png");
        CCCommonUtils::setSpriteMaxSize(icon, 95);
        m_iconNode->addChild(icon);
        
        m_joinBtn->setSwallowsTouches(false);
        m_closeBtn->setSwallowsTouches(false);
    }
    else //推荐加入联盟
    {
        int idx = AllianceManager::getInstance()->recAllianceIdx;
        if (idx >= AllianceManager::getInstance()->recallianceArray.size()) {
            idx = 0;
        }
        if (AllianceManager::getInstance()->recallianceArray.size() == 0) {
            return false;
        }
        m_info = AllianceManager::getInstance()->recallianceArray[idx];
        AllianceManager::getInstance()->recAllianceIdx  ++;
        
        CCBLoadFile("JoinRecAllianceTipView",this,this);
        //    m_nameLabel->setString( GlobalData::shared()->playerInfo.name+" :" );
        
        std::string str = "(";
        str.append(m_info->shortName.c_str());
        str.append(") ");
        str.append(m_info->name.c_str());
        m_allianceLabel->setString( str );
        int msgIdx = CCMathUtils::getRandomInt(0, 4);
        m_infoLabel->setString( _lang(msg[msgIdx]) );
        
        CCCommonUtils::setButtonTitle(m_joinBtn, _lang("115001").c_str());
        
        string playerIcon = GlobalData::shared()->playerInfo.pic;
        bool bIsMan = false;
        for (int i=0; i<5; i++) {
            if (playerIcon == iconMan[i]) {
                bIsMan = true;
                break;
            }
        }
        string showIcon = iconWoman[0];
        if (bIsMan) {
            showIcon = iconWoman[ idx%6 ];
        }else {
            showIcon = iconWoman[ idx%5 ];
        }
        showIcon = showIcon+".png";
        auto icon = CCLoadSprite::createSprite(showIcon.c_str(),true,CCLoadSpriteType_HEAD_ICON);
        CCCommonUtils::setSpriteMaxSize(icon, 95);
        m_iconNode->addChild(icon);
        m_joinBtn->setSwallowsTouches(false);
        m_closeBtn->setSwallowsTouches(false);
        
        
        m_headImgNode = HFHeadImgNode::create();
        string url = "";
        if (AllianceManager::getInstance()->recAllianceUrlMap.find(m_info->uid) != AllianceManager::getInstance()->recAllianceUrlMap.end()) {
            url = AllianceManager::getInstance()->recAllianceUrlMap[m_info->uid];
            m_headImgNode->initHeadImgUrl2(m_iconNode, url, 1.0f, 102, true, ccp(1, -1));
            m_nameLabel->setString( AllianceManager::getInstance()->recAllianceNameMap[m_info->uid] );
        }else {
            AllianceManager::getInstance()->startGetAllianceUrl(m_info->uid);
        }
        //    m_headImgNode->initHeadImgUrl2(m_iconNode, GlobalData::shared()->playerInfo.getCustomPicUrl(), 1.0f, 102, true, ccp(1, -1));
    }
    return true;
}

void JoinRecAllianceTipView::onEnter() {
    CCNode::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
//    this->scheduleOnce(schedule_selector(JoinRecAllianceTipView::removeSelf),10);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(JoinRecAllianceTipView::updateHead), MSG_ALLIANCE_HEAD_UPDATE, NULL);
}
void JoinRecAllianceTipView::onExit() {
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ALLIANCE_HEAD_UPDATE);
    CCNode::onExit();
}

SEL_CCControlHandler JoinRecAllianceTipView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onJoinBtnClick", JoinRecAllianceTipView::onJoinBtnClick);
    return NULL;
}

bool JoinRecAllianceTipView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_joinBtn", CCControlButton*, this->m_joinBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_closeBtn", CCControlButton*, this->m_closeBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel", CCLabelIF*, this->m_infoLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceLabel", CCLabelIF*, this->m_allianceLabel);
    return false;
}

bool JoinRecAllianceTipView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_joinBtn, pTouch)) {
        m_joinBtn->onTouchBegan(pTouch, pEvent);
    }
    if (isTouchInside(m_closeBtn, pTouch)) {
        m_closeBtn->onTouchBegan(pTouch, pEvent);
    }
    return true;
}

void JoinRecAllianceTipView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    SoundController::sharedSound()->playEffects(Music_Sfx_city_building);
    if (isTouchInside(m_joinBtn, pTouch)) {
        m_joinBtn->onTouchEnded(pTouch, pEvent);
        onJoinBtnClick(NULL, CCControlEvent::TOUCH_DOWN);
        return;
    }
    if (isTouchInside(m_closeBtn, pTouch)) {
        m_closeBtn->onTouchEnded(pTouch, pEvent);
//        onCloseBtnClick(NULL, CCControlEvent::TOUCH_DOWN);
        this->getAnimationManager()->runAnimations("FadeOut");
        this->scheduleOnce(schedule_selector(JoinRecAllianceTipView::removeSelf),0.5);
        return;
    }
}

void JoinRecAllianceTipView::removeSelf(float dt)
{
    this->removeFromParent();
}

void JoinRecAllianceTipView::onJoinBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    if ( _clickType == 1 )
    {
        PopupViewController::getInstance()->addPopupView(CreateAllianceView::create(11));
    }
    else
    {
        
        ApplyAllianceCommand* cmd = new ApplyAllianceCommand(m_info->uid);
        cmd->setCallback(CCCallFuncO::create(AllianceManager::getInstance(), callfuncO_selector(AllianceManager::joinAllianceSuccess), NULL));
        cmd->sendAndRelease();
    }
    removeSelf(0);
}

void JoinRecAllianceTipView::onCloseBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    removeSelf(0);
}

void JoinRecAllianceTipView::updateHead(CCObject* obj)
{
    string url = "";
    CCString* str = dynamic_cast<CCString*>(obj);
    if(str){
        url = str->getCString();
    }
    if (url != "") {
        if (url.length() < 20 ) {
            m_iconNode->removeAllChildren();
            string showIcon = url+".png";
            auto icon = CCLoadSprite::createSprite(showIcon.c_str());
            CCCommonUtils::setSpriteMaxSize(icon, 95);
            m_iconNode->addChild(icon);
        }else {
            m_headImgNode->initHeadImgUrl2(m_iconNode, url, 1.0f, 95, true, ccp(1, -1));
        }
    }
    m_nameLabel->setString( AllianceManager::getInstance()->recAllianceNameMap[m_info->uid] );
}
