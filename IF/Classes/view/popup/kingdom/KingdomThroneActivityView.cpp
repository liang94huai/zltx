//
//  KingdomThroneActivityView.cpp
//  IF
//
//  Created by ganxiaohua on 15/12/1.
//
//

#include "KingdomThroneActivityView.hpp"
#include "PopupViewController.h"
#include "GameController.h"
#include "ManagerKingdomView.h"
#include "YesNoDialog.h"
#include "Utf8Utils.h"
#include "SceneController.h"
#include "WorldMapView.h"
#include "WorldController.h"
#include "ActivityEventObj.h"
#include "ActivityController.h"
#include "KingBattleRankView.hpp"
#include "DynamicResourceController.h"
KingdomThroneActivityView *KingdomThroneActivityView::create(){
    KingdomThroneActivityView *ret = new KingdomThroneActivityView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void KingdomThroneActivityView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
}

void KingdomThroneActivityView::onExit(){
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool KingdomThroneActivityView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(504, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(7, false);
            CCLoadSprite::doResourceByCommonIndex(504, false);
        });
        auto node = CCBLoadFile("KingdomThroneActivityView", this, this);
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        m_battleDesc->setString(_lang("170001"));
        m_battleTile->setString(_lang("170000"));
        CCCommonUtils::setButtonTitle(m_btnJoin, _lang("170004").c_str());
        CCCommonUtils::setButtonTitle(m_btnRank, _lang("170007").c_str());
        
        auto cf = CCLoadSprite::getSF("57037_ad1_small.png");
        if (cf!=NULL) {
            m_activityBg->initWithSpriteFrame(cf);
        }
        m_battlesStartTime = 0;
        m_serverName = "";
        m_targetServerId = -1;
        int num = ActivityController::getInstance()->activityArr->count();
        for (int i=0; i<num; i++) {
            auto temp = dynamic_cast<ActivityEventObj*>(ActivityController::getInstance()->activityArr->objectAtIndex(i));
            if (temp && temp->type==11) {
                if (temp->getKingdomList()!=NULL) {
                    int count = temp->getKingdomList()->count();
                    if (count>0) {
                        auto oneServer = _dict(temp->getKingdomList()->objectAtIndex(0));
                        m_battlesStartTime = oneServer->valueForKey("startTime")->doubleValue()/1000;
                        if (m_battlesStartTime>0) {
                            m_battlesStartTime = GlobalData::shared()->changeTime(m_battlesStartTime);
                        }
                        m_serverName = oneServer->valueForKey("serverName")->getCString();
                        m_targetServerId = oneServer->valueForKey("serverId")->intValue();
                    }
                }
                break;
            }
        }
        
        this->schedule(schedule_selector(KingdomThroneActivityView::updateTime), 1);
        updateTime(0);
        
        ret = true;
    }
    return ret;
}

void KingdomThroneActivityView::updateTime(float _time){
    double left = m_battlesStartTime - GlobalData::shared()->getWorldTime();
    string temp = "";
    if(m_targetServerId !=-1){
        temp = m_serverName;
        if (left>0) {
            temp.append("  ");
            temp.append(CC_SECTOA(left));
            temp = _lang_1("170002",temp.c_str());
        }else{
            temp.append("  ");
            temp = _lang_1("170005",temp.c_str());
        }
    }
    targetIntro->setString(temp);
    
    auto activityIt = WorldController::getInstance()->m_worldActivity.find(FIGHT_OF_KING);
    if(activityIt != WorldController::getInstance()->m_worldActivity.end()){
        auto &activityInfo = activityIt->second;
        double time = WorldController::getInstance()->getTime();
        left = (activityInfo.timeInfo.startTime - time)/1000;
        if(time >= activityInfo.timeInfo.startTime && time <= activityInfo.timeInfo.endTime){
            m_selfIntro->setString(_lang("170006"));
        }else{
            if (left>0) {
                temp = _lang("170003");
                temp.append("  ");
                temp.append(CC_SECTOA(left));
                m_selfIntro->setString(_lang_1("170002",temp.c_str()));
            }
        }
    }
    
}

void KingdomThroneActivityView::onClickJoin(CCObject *pSender, CCControlEvent event){
    if (m_targetServerId!=-1) {
        GlobalData::shared()->playerInfo.currentServerId = m_targetServerId;
    }
    CCPoint pt = ccp(600, 600);
    int m_worldIndex = WorldController::getInstance()->getIndexByPoint(pt);
    //PopupViewController::getInstance()->removeAllPopupView();
    //zym 2015.12.11
    PopupViewController::getInstance()->forceClearAll(true);
    if(SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
        WorldMapView::instance()->gotoTilePoint(pt);
        WorldMapView::instance()->openTilePanel(m_worldIndex);
    }else{
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, m_worldIndex);
    }
}

void KingdomThroneActivityView::onClickRank(CCObject *pSender, CCControlEvent event){
    PopupViewController::getInstance()->addPopupView(KingBattleRankView::create());
//    bool flag = ActivityController::getInstance()->checkServerCanJoin(GlobalData::shared()->playerInfo.currentServerId);
//    if (flag) {
//        PopupViewController::getInstance()->addPopupView(KingBattleRankView::create());
//    }
//    else{
//        YesNoDialog::show(_lang("170056").c_str(), NULL);
//    }
}

void KingdomThroneActivityView::onCloseClick(CCObject *pSender, CCControlEvent event){
    this->closeSelf();
}

bool KingdomThroneActivityView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void KingdomThroneActivityView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(!isTouchInside(m_viewBg, pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
    }
}

bool KingdomThroneActivityView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_battleDesc", CCLabelIF*, this->m_battleDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_battleTile", CCLabelIF*, this->m_battleTile);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_selfIntro", CCLabelIF*, this->m_selfIntro);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"targetIntro", CCLabelIF*, this->targetIntro);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnJoin", CCControlButton*, this->m_btnJoin);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnRank", CCControlButton*, this->m_btnRank);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_activityBg", CCSprite*, this->m_activityBg);
    return false;
}

SEL_CCControlHandler KingdomThroneActivityView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickJoin", KingdomThroneActivityView::onClickJoin);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickRank", KingdomThroneActivityView::onClickRank);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseClick", KingdomThroneActivityView::onCloseClick);
    return NULL;
}
