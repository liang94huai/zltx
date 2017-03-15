//
//  MiraOpenView.cpp
//  IF
//
//  Created by 李朝辉 on 15-9-1.
//
//

#include "SceneController.h"
#include "ImperialScene.h"
#include "MiraOpenView.h"
#include "MiraOpenView.h"
#include "TileInfo.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "UIComponent.h"
#include "QijiInfoCommand.h"
#include "QueueController.h"


MiraOpenView* MiraOpenView::create(int buildId){
    MiraOpenView* ret = new MiraOpenView();
    if(ret && ret->init(buildId)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MiraOpenView::init(int buildId)
{
    if (!ArcPopupBaseView::init(TYPE_POS_UP)) {
        return false;
    }
    setIsHDPanel(true);
    m_isReturn = true;
    m_buildId = buildId;
    auto tmpCCB = CCBLoadFile("TileOpenView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    
    m_upBtnMsgLabel->setString(_lang("102305").c_str());
    //    m_btnMsgLabel->setString(_lang("102127").c_str());
    //    m_inBtnGoldNum->setString("1");
    
    m_scrollView = CCScrollView::create(m_infoList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_infoList->addChild(m_scrollView);
    
    updateInfo();
    m_openNum=0;
    
    m_mainNode->setPositionY(m_mainNode->getPositionY()-200);
    CCActionInterval * moveBy2 = CCMoveBy::create(0.25, ccp(0,200));
    m_mainNode->runAction(CCSequence::create(moveBy2, NULL));
    
    return true;

}

void MiraOpenView::updateInfo(CCObject* p)
{
    bool isCanUp = true;
    
    string name = "";
    int silver_need = 0;
    int wood_need = 0;
    int time_need = 0;
    int stone_need = 0;
    int food_need = 0;
    int iron_need = 0;
    string tool_need = "";
    string building = "";
    string destip = "";
    int level = 0;
    
    
    auto m_info = FunBuildController::getInstance()->m_bigTileMap[m_buildId];
    
    silver_need = m_info.silver_need;
    wood_need = m_info.wood_need;
    stone_need = m_info.stone_need;
    food_need = m_info.food_need;
    iron_need = m_info.iron_need;
    //destip = m_info.destip;
    
    _tmpGold = 0;
    m_nextLabel->setString("");
    m_nextMsgLabel->setString(_lang("160016"));
    
    int _itemH = 46;
    int curY = 25;
    int curX = 30;
    if (CCCommonUtils::isIosAndroidPad())
    {
        _itemH = 87;
        curX = 60;
        curY = 50;
    }
    m_scrollView->getContainer()->removeAllChildren();
    building = "428000;30|419000;30|415000;30|400000;30";
    if (building!="") {
        std::vector<string> buildItems;
        CCCommonUtils::splitString(building, "|", buildItems);
        for (int i=0; i<buildItems.size(); i++) {
            string curBuild = buildItems[i];
            std::vector<string> tmpbuildVec;
            CCCommonUtils::splitString(curBuild, ";", tmpbuildVec);
            int paramBuild = atoi(tmpbuildVec[0].c_str())+atoi(tmpbuildVec[1].c_str());
            auto itemBuild = UpgradeCell::create(2, paramBuild, 0);
            itemBuild->setPosition(ccp(curX, curY));
            itemBuild->setTouchNode(m_infoList,m_buildId);
            m_scrollView->addChild(itemBuild);
            curY += _itemH;
            
            if (!itemBuild->m_isOk) {
                isCanUp = false;
//                m_instantBtn->setEnabled(false);
            }
        }
    }

    
    m_upBtn->setEnabled(isCanUp);
    
    if (CCCommonUtils::isIosAndroidPad()) {
        curY -= 46;
    } else {
        curY -= 23;
    }
    m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,curY));
    m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - curY));
    m_scrollView->setTouchEnabled(false);
}

void MiraOpenView::onEnter()
{
    CCNode::onEnter();
    UIComponent::getInstance()->showResourceBar(true);
    if (m_openNum>0) {
        updateInfo();
    }
    
    m_openNum++;
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MiraOpenView::updateInfo), MSG_REFREASH_BUILD_UPGRADE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MiraOpenView::DelandCreateNew), MSG_CLOSEUNLOCK, NULL);
}

void MiraOpenView::onExit()
{
    UIComponent::getInstance()->showResourceBar(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REFREASH_BUILD_UPGRADE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REFREASH_BUILD_UPGRADE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CLOSEUNLOCK);
    CCNode::onExit();
}

SEL_CCControlHandler MiraOpenView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCreateOrUpClick", MiraOpenView::onCreateOrUpClick);
    
    return NULL;
}

bool MiraOpenView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upBtn", CCControlButton*, this->m_upBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextLabel", CCLabelIF*, this->m_nextLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextMsgLabel", CCLabelIF*, this->m_nextMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upBtnMsgLabel", CCLabelIF*, this->m_upBtnMsgLabel);
    return false;
}

//CCNode *MiraOpenView::getGuideNode(){
//    return NULL;
//}
//void MiraOpenView::onClickGetMore(cocos2d::CCObject * pSender, CCControlEvent pCCControlEvent)
//{
//    //TODO:
//    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
//    if (layer) {
//        int m_gotoBuildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_MAIN);
//        layer->onMoveToBuildAndPlay(m_gotoBuildId, true);
//        PopupViewController::getInstance()->removeAllPopupView();
//    }
//    
//}
void MiraOpenView::onCreateOrUpClick(cocos2d::CCObject * pSender, CCControlEvent pCCControlEvent)
{
    FunBuildController::getInstance()->startUnlockQJBuild();
}
void MiraOpenView::DelandCreateNew(CCObject* param)
{
    closeSelf();
}
void MiraOpenView::onOkUp()
{
}
