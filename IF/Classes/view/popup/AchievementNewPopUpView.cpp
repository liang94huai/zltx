//
//  AchievementNewPopUpView.cpp
//  IF
//
//  Created by lifangkai on 14-7-8.
//
//

#include "AchievementNewPopUpView.h"

//
//  AchievementPopUpView.cpp
//  IF
//
//  Created by ganxiaohua on 14-6-19.
//
//

#include "AchievementNewPopUpView.h"
#include "QuestController.h"
#include "PopupViewController.h"
#include "RewardController.h"
#include "UIComponent.h"
#include "AchievementDetailPopUpView.h"
#include "SoundController.h"
#include "DropRdCCB.h"
#include "QuestRewardCommand.h"
#include "LuaController.h"
#include "CCMathUtils.h"
#include "GoldExchangeView_NEW.hpp"
#include "GuideController.h"
#include "DailyActiveBoxInfoView.h"
#include "DailyActiveView.h"
#include "IFNormalSkNode.h"

static CCPoint worldPt = ccp(0,0);
static string m_guideKey = "";
static int m_changeIndex = -1;

AchievementNewPopUpView *AchievementNewPopUpView::create(){
    AchievementNewPopUpView *ret = new AchievementNewPopUpView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

AchievementNewPopUpView::~AchievementNewPopUpView(){

}
void AchievementNewPopUpView::onEnter(){
    PopupBaseView::onEnter();
    CCLoadSprite::doResourceByCommonIndex(101, true);
    setSwallowsTouches(true);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    m_tabView->setTouchEnabled(true);
//    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AchievementNewPopUpView::refreshView), QUEST_STATE_UPDATE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AchievementNewPopUpView::onPlayDropRD), MSG_PLAY_DROP_RD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AchievementNewPopUpView::onUpdateStageInfo), QUEST_STAGE_UPDATE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AchievementNewPopUpView::onRetRewardInfo), GETRT_REWARD_INFO, NULL);
    this->refreshView(NULL);
    onUpdateStageInfo(NULL);
    if (GuideController::share()->getCurGuideID() == "3076000") {
        //m_stageNode->setVisible(false);
        //m_noQuestText->setVisible(false);
    }
}

void AchievementNewPopUpView::onExit(){
    setTouchEnabled(false);
    m_tabView->setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, QUEST_STATE_UPDATE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PLAY_DROP_RD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, QUEST_STAGE_UPDATE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GETRT_REWARD_INFO);
    PopupBaseView::onExit();
}

bool AchievementNewPopUpView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        if (CCCommonUtils::isIosAndroidPad())
        {
             std::string _path = CCFileUtils::sharedFileUtils()->getWritablePath()+ "dresource/" + COMMON_PATH_101_HD;
            if (CCFileUtils::sharedFileUtils()->isFileExist(_path)) {
                _path = CCFileUtils::sharedFileUtils()->getWritablePath()+ "dresource/" + COMMON_PATH_HD;
                CCLoadSprite::doResourceByPathIndex(_path, 101, true);
            }
            
        }
        
        CCLoadSprite::doResourceByCommonIndex(101, true);
        CCLoadSprite::doResourceByCommonIndex(6, true);
        CCLoadSprite::doResourceByCommonIndex(502, true);
        
        setCleanFunction([](){
            if (CCCommonUtils::isIosAndroidPad())
            {
                std::string _path = CCFileUtils::sharedFileUtils()->getWritablePath()+ "dresource/" + COMMON_PATH_101_HD;
                if (CCFileUtils::sharedFileUtils()->isFileExist(_path)) {
                    _path = CCFileUtils::sharedFileUtils()->getWritablePath()+ "dresource/" + COMMON_PATH_HD;
                    CCLoadSprite::doResourceByPathIndex(_path, 101, false);
                }
            }
            auto pop = dynamic_cast<DailyActiveView*>(PopupViewController::getInstance()->getCurrentPopupView());
            if(!pop){
                CCLoadSprite::doResourceByCommonIndex(101, false);
                CCLoadSprite::doResourceByCommonIndex(502, false);
                CCLoadSprite::doResourceByCommonIndex(6, false);
            }
            
        });
        auto bg = CCBLoadFile("QuestViewNewCCB", this, this);
        this->setContentSize(bg->getContentSize());
        setTitleName(_lang("107500"));
        
        int preHeight = this->m_viewBg->getContentSize().height;
        changeBGHeight(m_viewBg);
        
        int dh = m_viewBg->getContentSize().height - preHeight;
        this->m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width, m_listContainer->getContentSize().height + dh));
        m_listContainer->setPositionY(m_listContainer->getPositionY()-dh);
        
        m_data = CCArray::create();
        m_cells = CCArray::create();
        m_waitInterface=NULL;
        m_waitInterfaceStage=NULL;
        m_tabView = CCTableView::create(this, m_listContainer->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setDelegate(this);
        m_tabView->setTouchPriority(2);
        m_listContainer->addChild(m_tabView);
        CCCommonUtils::setButtonTitle(m_btnReward, _lang("107516").c_str());
        
        m_clipNode = CCClipNode::create(m_stagePro->getContentSize());
    
        m_clipNode->setAnchorPoint(ccp(0,0));
        m_stagePro->setAnchorPoint(ccp(0,0));
        m_stagePro->getParent()->addChild(m_clipNode);
        m_clipNode->setPosition(m_stagePro->getPosition());
        m_stagePro->removeFromParent();
        m_stagePro->setPosition(ccp(0,0));
        m_clipNode->addChild(m_stagePro);
        m_stageCurLabel->setFntFile("Arial_Bold_Border.fnt");
        
        ccColor4F beginColor = {0, 0, 0, 0};
        ccColor4F endColor = {0, 0, 0, 0};
        for (int i=1; i<=3; i++)
        {
//            auto particle = ParticleController::createParticle(CCString::createWithFormat("Loading_%d",i)->getCString());
//            switch (i)
//            {
//                case 1:
//                    beginColor = {255/255.0f, 200/255.0f, 100/255.0f, 100/255.0f};
//                    break;
//                case 2:
//                    beginColor = {255/255.0f, 200/255.0f, 0/255.0f, 100/255.0f};
//                    break;
//                case 3:
//                    beginColor = {255/255.0f, 255/255.0f, 255/255.0f, 100/255.0f};
//                    break;
//                default:
//                    break;
//            }
//            particle->setStartColor(beginColor);
//            particle->setEndColor(endColor);
//            m_particleNode->addChild(particle);
        }
        
        m_particleStageNode = CCNode::create();
        this->addChild(m_particleStageNode);
        
        m_handNode = CCNode::create();
        m_handNode->setPosition(m_boxIconNode->getPosition());
        m_stageNode->addChild(m_handNode);
        m_flyArrow = CCLoadSprite::createSprite("UI_hand.png");
        m_flyArrow->setAnchorPoint(ccp(0, 0));
        m_flyArrow->setRotation(90);
        m_flyArrow->setVisible(false);
        m_handNode->addChild(m_flyArrow);
        m_arrowPar = Node::create();
        m_handNode->addChild(m_arrowPar);
        
        m_recommendinfo = NULL;
        m_guideAutoSt = 0;
        
        CCCommonUtils::setButtonTitle(m_DailyBtn, _lang("133104").c_str());
        m_DailyBtn->setTitleColorForState(Color3B(183,183,183), CCControlStateNormal);
        m_DailyBtn->setTitleColorForState(Color3B(107,57,0), CCControlStateHighlighted);
        m_DailyBtn->setTitleColorForState(Color3B(107,57,0), CCControlStateDisabled);
        m_DailyBtn->setTitleColorForState(Color3B(107,57,0), CCControlStateSelected);
        
        CCCommonUtils::setButtonTitle(m_TaskBtn, _lang("107500").c_str());
        m_TaskBtn->setTitleColorForState(Color3B(183,183,183), CCControlStateNormal);
        m_TaskBtn->setTitleColorForState(Color3B(107,57,0), CCControlStateHighlighted);
        m_TaskBtn->setTitleColorForState(Color3B(107,57,0), CCControlStateDisabled);
        m_TaskBtn->setTitleColorForState(Color3B(107,57,0), CCControlStateSelected);
        updateBtnType(1);


        //     齿轮特效
        //m_effectChiLun->
        if (CCFileUtils::sharedFileUtils()->isFileExist("Imperial/Imperial_30.atlas") &&
        CCFileUtils::sharedFileUtils()->isFileExist("Spine/UIComponent/chengfangchilun.json"))
    {
        auto chilun = new IFSkeletonAnimation("Spine/UIComponent/chengfangchilun.json", "Imperial/Imperial_30.atlas");
        if (chilun) {
            {
                chilun->setVisibleStop(false);
                m_effectChiLun->addChild(chilun,3);
                chilun->setPositionY(-140);
                spTrackEntry* entry = chilun->setAnimation(0.01, "animation", true);
                if(entry){
                    chilun->setTimeScale(0.5f);
                }
            }
        }
    }
        ret = true;
    }
    return ret;
}

void AchievementNewPopUpView::onRewardBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
//    auto dropCCB1 = DropRdCCB::create(R_FOOD);
//    this->addChild(dropCCB1);
//    return;
    
    if (m_recommendinfo==NULL || QuestController::getInstance()->isGettingReward) {
        return;
    }
    
    if(m_recommendinfo->state!=COMPLETE || m_recommendinfo->curValue<m_recommendinfo->maxValue){
        AutoSafeRef temp(this);
        
        PopupViewController::getInstance()->removeAllPopupView();
        QuestController::getInstance()->goToQuestTarget(m_recommendinfo);
        return;
    }
    
    m_btnReward->setEnabled(false);
    auto dropCCB = DropRdCCB::create(r_type);
    if (CCCommonUtils::isIosAndroidPad())
    {
        dropCCB->setScale(2);
    }
    this->addChild(dropCCB);
    m_waitInterface= GameController::getInstance()->showWaitInterface(m_btnReward,-1);
    worldPt = m_btnReward->getParent()->convertToWorldSpace(m_btnReward->getPosition());
    QuestController::getInstance()->getReward(m_recommendinfo->itemId, true);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           , CCString::createWithFormat("AC_reward"));
}

void AchievementNewPopUpView::refreshView(CCObject * obj){
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    m_data->removeAllObjects();
    m_cells->removeAllObjects();
    setRecommendTaskInfo();
    if (CCCommonUtils::isTestPlatformAndServer("Quest_sort")) {
        newSortQuestInfo();
    }
    else {
        sortQuestInfo();
    }
    
    if(m_data->count()<=0){
        m_noQuestText->setString(_lang("107530"));
        m_noQuestText->setVisible(true);
    }else{
        //m_noQuestText->setVisible(false);
    }
    bool ishasChild = false;
    CCPoint pos;
    if(m_tabView->getContainer()->getChildrenCount()==0){
        ishasChild = true;
    }else{
        pos = m_tabView->getContentOffset();
    }
    m_tabView->reloadData();
    if(ishasChild){
    }else{

        CCPoint minPt = m_tabView->minContainerOffset();
        CCPoint maxPt = m_tabView->maxContainerOffset();
        if (pos.y > 0) {
            return;
        }
        if (pos.y > maxPt.y) {
            pos.y = maxPt.y;
        }
        else if (pos.y < minPt.y) {
            pos.y = minPt.y;
        }
        m_tabView->setContentOffset(pos);
    }
    if (obj && GuideController::share()->getCurGuideID() != "3076000") {
        onStageRdUpdate(NULL);
    }
}
int AchievementNewPopUpView::getContentOffSety(){
    int index = 0;
    int num = m_data->count()-1;
    for(int j=0;j<num;j++){
        QuestInfo* quest1 = (QuestInfo*)m_data->objectAtIndex(j);
        if(quest1->state==COMPLETE){
            break;
        }
        index++;
        j++;

    }
   
    return index;
}
void AchievementNewPopUpView::setRecommendTaskInfo(float dt)
{
//    m_recommendinfo = NULL;
//    m_recommendinfo = QuestController::getInstance()->getRecommendQuest(true);
    
    QuestInfo* curRecommendinfo = QuestController::getInstance()->getRecommendQuest(true);
    if (curRecommendinfo != m_recommendinfo) {
        bool stTmp = true;
        if (m_recommendinfo!=NULL) {
            stTmp = false;
            this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(AchievementNewPopUpView::updateRecommendTask));
            this->getAnimationManager()->runAnimationsForSequenceNamed("Refresh");
        }
        m_recommendinfo = curRecommendinfo;
        if (stTmp) {
            updateRecommendTask();
        }
    }
}

void AchievementNewPopUpView::updateRecommendTask()
{
    this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
    m_nameTxt->setString(_lang("107528").c_str());
    if(m_recommendinfo){
        QuestInfo* quest = m_recommendinfo;
        m_btnReward->setEnabled(false);
        if(m_recommendinfo->state==COMPLETE){
            m_btnReward->setEnabled(true);
            CCCommonUtils::setButtonTitle(m_btnReward, _lang("107516").c_str());
            //CCCommonUtils::setButtonSprite(m_btnReward, "btn_green3.png");
        }else{
            m_btnReward->setEnabled(true);
            CCCommonUtils::setButtonTitle(m_btnReward, _lang("2000214").c_str());
            //CCCommonUtils::setButtonSprite(m_btnReward, "but_blue.png");
        }
        if(quest->curValue>=quest->maxValue){
            m_btnReward->setEnabled(true);
            CCCommonUtils::setButtonTitle(m_btnReward, _lang("107516").c_str());
            //CCCommonUtils::setButtonSprite(m_btnReward, "btn_green3.png");
        }
        m_descTxt->setString(quest->name);
        std::string str = CC_CMDITOA(quest->curValue>quest->maxValue?quest->maxValue:quest->curValue);
        str.append("/");
        str.append(CC_CMDITOA(quest->maxValue));
        m_progressTxt->setString(str);
        int max = quest->maxValue;
        max = MAX(max, quest->curValue);
        float scale = quest->curValue*1.0/max;
        scale = MIN(scale,1);
        m_progress->setScaleX(scale);
        m_headNode->removeAllChildren();
        CCSprite* pic1 = NULL;
        if(m_recommendinfo->icon1!=""){
            std::string _picname = m_recommendinfo->icon1;
            if (CCCommonUtils::isIosAndroidPad())
            {
                std::vector<std::string> _strvet;
                CCCommonUtils::splitString(_picname.c_str(), ".", _strvet);
                _picname = _strvet[0] + "_hd." + _strvet[1];
                if (!CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(_picname.c_str()))
                {
                    _picname = m_recommendinfo->icon1;
                    m_headNode->setScale(2.f);
                }
            }
            pic1 = CCLoadSprite::createSprite(_picname.c_str());
            m_headNode->addChild(pic1);
            pic1->setAnchorPoint(ccp(0.5, 0.5));
        }
        if(m_recommendinfo->icon2!=""){
            std::string _picname = m_recommendinfo->icon2;
            if (CCCommonUtils::isIosAndroidPad())
            {
                std::vector<std::string> _strvet;
                CCCommonUtils::splitString(_picname.c_str(), ".", _strvet);
                _picname = _strvet[0] + "_hd." + _strvet[1];
                if (!CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(_picname.c_str()))
                {
                    _picname = m_recommendinfo->icon2;
                    m_headNode->setScale(2.f);
                }
            }
            CCSprite* pic = CCLoadSprite::createSprite(_picname.c_str());
            m_headNode->addChild(pic);
            pic->setAnchorPoint(ccp(0.5, 0.5));
            if(pic1!=NULL){
                pic->cocos2d::CCNode::setPosition(pic1->getContentSize().width/2-pic->getContentSize().width/2, -(pic1->getContentSize().height/2-pic->getContentSize().height/2));
            }
            
        }
        
        string rewardIcon = "";
        r_type = -1;
        double r_num = 0;
        if (m_recommendinfo->rewardshow == "wood") {
            rewardIcon = "item400.png";
            r_type = R_WOOD;
        }
        else if (m_recommendinfo->rewardshow == "food") {
            rewardIcon = "item403.png";
            r_type = R_FOOD;
        }
        else if (m_recommendinfo->rewardshow == "iron") {
            rewardIcon = "item402.png";
            r_type = R_IRON;
        }
        else if (m_recommendinfo->rewardshow == "stone") {
            rewardIcon = "item401.png";
            r_type = R_STONE;
        }
        else if (m_recommendinfo->rewardshow == "gold") {
            rewardIcon = "item406.png";
            r_type = R_GOLD;
        }
        m_rewardIcon->removeAllChildren();
        bool isGold = false;
        for (int i=0; i<m_recommendinfo->reward->count(); i++) {
            auto dic = dynamic_cast<CCDictionary*>(m_recommendinfo->reward->objectAtIndex(i));
            int type = dic->valueForKey("type")->intValue();
            if (type == r_type) {
                isGold = true;
                r_num = dic->valueForKey("value")->intValue();
                break;
            }
        }
        if (isGold) {
            auto rIcon = CCLoadSprite::createSprite(rewardIcon.c_str());
            rIcon->setAnchorPoint(ccp(0.5, 0.5));
            m_rewardIcon->addChild(rIcon);
            string tmpstr = ":";
            tmpstr = tmpstr + RES_ITOA_K_1(r_num);
           // m_rewardNum->setString(tmpstr.c_str());
            //m_rewardNum->setFntFile("pve_fnt_boss.fnt");
            //change my font
            m_rewardNum->setVisible(false);
            
            // 任务 -->艺术字
            auto label_1 = LabelAtlas::create(tmpstr.c_str(), "res_num_font.png", 25, 37, '.');
            label_1->setAnchorPoint(ccp(0,0.5));
            label_1->setPositionX(94/2);
            m_rewardIcon->addChild(label_1);
            string tmpstr_2 = RES_ITOA_K_2(r_num);
            auto label_2 = LabelAtlas::create(tmpstr_2.c_str(), "res_abc_font.png", 39, 37, '<');
            label_2->setAnchorPoint(ccp(0,0.5));
            label_2->setPositionX(94/2 + tmpstr.length()*25 );
            if(r_num>=1000)
            {
                m_rewardIcon->addChild(label_2);
            }
            
        }
    }
}

void AchievementNewPopUpView::onPlayDropRD(CCObject* obj)
{
    CCInteger* intObj = dynamic_cast<CCInteger*>(obj);
    if (intObj) {
        int tr_type = intObj->getValue();
        if (tr_type>-1) {
            auto dropCCB = DropRdCCB::create(tr_type);
            if (CCCommonUtils::isIosAndroidPad())
            {
                dropCCB->setScale(2.f);
            }
            this->addChild(dropCCB);
        }
    }
}

bool AchievementNewPopUpView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_effectChiLun", CCNode*, this->m_effectChiLun);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCScale9Sprite*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_progress", CCScale9Sprite*, this->m_progress);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_progressTxt", CCLabelIF*, this->m_progressTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_noQuestText", CCLabelIF*, this->m_noQuestText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_descTxt", CCLabelIF*, this->m_descTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnReward", CCControlButton*, this->m_btnReward);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_headNode", CCNode*, this->m_headNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_recommandBG", CCNode*, this->m_recommandBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardIcon", CCNode*, this->m_rewardIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rewardNum", CCLabelIFBMFont*, this->m_rewardNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_dropNode", CCNode*, this->m_dropNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_stageNode", CCNode*, this->m_stageNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_stageStartLabel", CCLabelIF*, this->m_stageStartLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_stageCurLabel", CCLabelIFBMFont*, this->m_stageCurLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_stageEndLabel", CCLabelIF*, this->m_stageEndLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_stagePro", CCScale9Sprite*, this->m_stagePro);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_stageMoveNode", CCNode*, this->m_stageMoveNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_stageBtn", CCControlButton*, this->m_stageBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_boxIconNode", CCNode*, this->m_boxIconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_particleNode", CCNode*, this->m_particleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_stageGuideNode", Node*, m_stageGuideNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_DailyBtn", CCControlButton*, this->m_DailyBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_TaskBtn", CCControlButton*, this->m_TaskBtn);

    return false;
}


SEL_CCControlHandler AchievementNewPopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRewardBtnClick", AchievementNewPopUpView::onRewardBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickStageBtn", AchievementNewPopUpView::onClickStageBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickDailyBtn", AchievementNewPopUpView::onClickDailyBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickTaskBtn", AchievementNewPopUpView::onClickTaskBtn);

    return NULL;
}
bool AchievementNewPopUpView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_touchPoint = pTouch->getLocation();
    if (isTouchInside(m_recommandBG,pTouch)){
        return true;
    }
    if (m_guideKey == "AC_normal" && isTouchInside(m_listContainer, pTouch) && m_listContainer) {
        return true;
    }
    if (m_guideKey == "AC_stage" && isTouchInside(m_stageGuideNode, pTouch) && m_stageGuideNode) {
        return true;
    }
    return false;
}
void AchievementNewPopUpView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (fabs(m_touchPoint.y - pTouch->getLocation().y)> 30) {
        return;
    }
    if(m_recommendinfo==NULL){
        return;
    }
    if (isTouchInside(m_recommandBG,pTouch)) {
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        if (CCCommonUtils::isIosAndroidPad())
        {
            PopupViewController::getInstance()->addPopupInView(AchievementDetailPopUpView::create(m_recommendinfo), true, false, false, true);
        }
        else
        {
            PopupViewController::getInstance()->addPopupInView(AchievementDetailPopUpView::create(m_recommendinfo));
        }
    }
    if (m_guideKey == "AC_normal" && isTouchInside(m_listContainer, pTouch) && m_listContainer) {
        m_guideKey = "";
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::create("AC_normal"));
    }
    if (m_guideKey == "AC_stage" && isTouchInside(m_stageGuideNode, pTouch) && m_stageGuideNode) {
        m_guideKey = "";
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::create("AC_stage"));
    }
}

void AchievementNewPopUpView::flyStageParticle()
{
    if (worldPt.x == 0 && worldPt.y == 0) {
        return;
    }
    if (m_stageNode->isVisible() == false) {
        return;
    }
    
    m_particleStageNode->removeAllChildrenWithCleanup(true);
    m_particleStageNode->stopAllActions();
    auto pt = this->convertToNodeSpace(worldPt);
    m_particleStageNode->setPosition(pt);
    worldPt = ccp(0, 0);
    
    m_particleNode->getPosition();
    auto endPt = m_particleNode->getParent()->convertToWorldSpace(m_particleNode->getPosition());
    endPt = this->convertToNodeSpace(endPt);
    
    string tmpStart = "Collection_expF_";//Collection_Star_ Collection_expF_
    int count = 3;
    for (int i=1; i<=count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        if (i==1) {
            particle->setStartColor(ccc4f(1.0, 0.77, 0, 1.0));
            particle->setEndColor(ccc4f(0, 0, 0, 0));
        }else if (i==2) {
            particle->setStartColor(ccc4f(1.0, 0.55, 0, 1.0));
            particle->setEndColor(ccc4f(1, 0.96, 0.5, 0));
        }else if (i==3) {
            particle->setStartColor(ccc4f(1.0, 0.9, 0, 1.0));
            particle->setEndColor(ccc4f(0, 0, 0, 0));
        }
        
        if (i == 2) {
            particle->setPositionType(kCCPositionTypeFree);
        } else {
            particle->setPositionType(kCCPositionTypeRelative);
        }
        m_particleStageNode->addChild(particle);
    }
    auto func = CCCallFunc::create(this, callfunc_selector(AchievementNewPopUpView::flyStageEnd));
    
    CCActionInterval * moveTo = CCMoveTo::create(0.7,endPt);
    CCActionInterval * easeExponentialIn= CCEaseExponentialOut::create(moveTo);
    
    CCActionInterval * delT = CCDelayTime::create(0.2);
    CCActionInterval * fadeOut = CCFadeOut::create(0.1);
    
    auto funcPowerUI = nullptr;
    CCActionInterval * seqFadeOut = CCSequence::create(delT, fadeOut, nullptr);
    CCSpawn* spawn = CCSpawn::create(easeExponentialIn,seqFadeOut,nullptr);
    m_particleStageNode->runAction(CCSequence::create(spawn, func, nullptr));
}

void AchievementNewPopUpView::flyStageEnd()
{
    m_particleStageNode->removeAllChildrenWithCleanup(true);
    onUpdateStageInfo(NULL);
}

CCSize AchievementNewPopUpView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(1536, 370);
    }
    
    return CCSize(640, 150);
}

CCSize AchievementNewPopUpView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(1536, 370);
    }
    
    return CCSize(640, 150);
}

CCTableViewCell* AchievementNewPopUpView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    AchievementNewTaskTableCell* cell = dynamic_cast<AchievementNewTaskTableCell*>( table->dequeueCell());
    QuestInfo* info = dynamic_cast<QuestInfo*>(m_data->objectAtIndex(idx));
    if( info == NULL )
        return  NULL ;
    
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info,idx,m_listContainer);
        }else{
            cell = AchievementNewTaskTableCell::create(info,idx,m_listContainer);
            m_cells->addObject(cell);
        }
    }
    return cell;
}

ssize_t AchievementNewPopUpView::numberOfCellsInTableView(CCTableView *table){
    int count = m_data->count();
    return count;
}

void AchievementNewPopUpView::scrollViewDidScroll(CCScrollView* view){
    float mindy = m_tabView->minContainerOffset().y;
    float dy = m_tabView->getContentOffset().y;
    if(dy<mindy){
        m_tabView->setContentOffset(ccp(0, mindy));
    }
}

CCNode * AchievementNewPopUpView::getGuideNode(string _key)
{
    if (_key=="AC_back") {
        return UIComponent::getInstance()->m_popupReturnBtn;
    }
    else if (_key=="AC_reward") {
        
        if(m_recommendinfo&&m_recommendinfo->state==COMPLETE){
            m_guideAutoSt = 0;
            return m_btnReward;
        }else {
            if (m_guideAutoSt==0) {
                m_guideAutoSt = 1;
                autoGuideNext();
//                scheduleOnce(schedule_selector(AchievementNewPopUpView::autoGuideNext), 0.1);
            }
            return NULL;
        }
    }
    else if (_key=="AC_normal") {
        m_guideKey = _key;
        return m_listContainer;
    }
    else if (_key=="AC_stage") {
        m_guideKey = _key;
        return m_stageGuideNode;
    }
    return NULL;
}

void AchievementNewPopUpView::autoGuideNext(float dt)
{
    if (m_guideAutoSt==1) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                               , CCString::createWithFormat("AC_reward"));
    }
}

void AchievementNewPopUpView::onClickStageBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    int curId = QuestController::getInstance()->m_stageRdId;
    int rewardId = atoi( CCCommonUtils::getPropById(CC_ITOA(curId), "reward").c_str() );
    if (QuestController::getInstance()->m_stageRDMap.find(rewardId) == QuestController::getInstance()->m_stageRDMap.end()) {
        m_waitInterfaceStage= GameController::getInstance()->showWaitInterface(m_stageBtn,-1);
        QuestController::getInstance()->startGetRewardById(rewardId);
    }else {
        onRetRewardInfo(NULL);
    }
}

//任务 两个按钮方法
void AchievementNewPopUpView::onClickDailyBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    updateBtnType(0);
}

void AchievementNewPopUpView::onClickTaskBtn(CCObject * pSender, CCControlEvent pCCControlEvent)
{
    updateBtnType(1);
}

void AchievementNewPopUpView::updateBtnType(int iType) // 0 日常 1 普通
{
 if(iType == 0)
 {
     m_DailyBtn->setEnabled(false);
     m_TaskBtn->setEnabled(true);
     
     //关闭当前页面
     //PopupViewController::getInstance()->removePopupView(this);
     PopupViewController::getInstance()->goBackPopupView();

     //打开日常
     PopupViewController::getInstance()->addPopupInView(DailyActiveView::create());
 }else
 {
     m_DailyBtn->setEnabled(true);
     m_TaskBtn->setEnabled(false);
 }
}

void AchievementNewPopUpView::onUpdateStageInfo(CCObject* obj)
{
    if(m_waitInterfaceStage!=NULL){
        m_waitInterfaceStage->remove();
        m_waitInterfaceStage = NULL;
    }
    int curId = QuestController::getInstance()->m_stageRdId;
    string iconPath = CCCommonUtils::getPropById(CC_ITOA(curId), "icon");
    if (iconPath=="" || !CCCommonUtils::isTestPlatformAndServer("quest_stage")) {
        //m_stageNode->setVisible(false);
       // return;
    }
    
    int curNum = QuestController::getInstance()->m_stageNum;
    int startNum = atoi( CCCommonUtils::getPropById(CC_ITOA(curId), "para0").c_str() );
    int endNum = atoi( CCCommonUtils::getPropById(CC_ITOA(curId), "para1").c_str() );
    
    m_stageStartLabel->setString(CC_ITOA(curNum-startNum));
    string tmpEndStr = " / ";
    m_stageEndLabel->setString(tmpEndStr+CC_ITOA(endNum-startNum));
    m_stageCurLabel->setString(CC_ITOA(curNum));
    
    float pro = (curNum-startNum)*1.0/(endNum-startNum);
    pro = MIN(pro, 1.0);
    pro = MAX(pro, 0);
    auto clipSize = m_stagePro->getContentSize();
    if(pro == 0){
    
        m_clipNode->setContentSize(CCSizeMake(clipSize.width*pro , clipSize.height));
    }
    else{
    
        m_clipNode->setContentSize(CCSizeMake(clipSize.width*pro + 25, clipSize.height)); //形状改变成三角状的进度条 30 -->偏差值
    }
    m_stageMoveNode->setPositionX(clipSize.width*pro);
    
    m_boxIconNode->removeAllChildren();
    string tmpPath = iconPath+".png";
    if ( iconPath == "") {
        tmpPath = "quest_gaojibaoxiang.png";
    }
    auto icon = CCLoadSprite::createSprite(tmpPath.c_str());
    if (iconPath != "quest_gaojibaoxiang") {
        icon->setAnchorPoint(ccp(0.4, 0.4));
        icon->setFlipX(true);
    }
    m_boxIconNode->addChild(icon);
    
    m_boxIconNode->stopAllActions();
    m_boxIconNode->setRotation(0);
    float hdScale = 1.0;
    if (CCCommonUtils::isIosAndroidPad())
    {
        hdScale = 2.0;
    }
    m_boxIconNode->setScale(1.0*hdScale);
    if (curNum>=endNum) {
        CCActionInterval * dely = CCDelayTime::create(1.0);
        CCActionInterval * scaleTo1 = CCScaleTo::create(0.2,1.0*hdScale+0.1);
        CCActionInterval * scaleTo2 = CCScaleTo::create(0.2,1.0*hdScale);
        CCActionInterval * ro1 = CCRotateTo::create(0.1,-7.5);
        CCActionInterval * ro2 = CCRotateTo::create(0.1,7.5);
        CCActionInterval * ro3 = CCRotateTo::create(0.1,0);
        CCSequence* sequence = CCSequence::create(scaleTo1, ro1, ro2, ro3, scaleTo2, dely, NULL);
        m_boxIconNode->runAction(CCRepeatForever::create(sequence));
        onShowHand(0);
    }else {
        CCCommonUtils::setSpriteGray(icon, true);
    }
}

void AchievementNewPopUpView::onStageRdUpdate(CCObject* obj)
{
    if(m_waitInterfaceStage!=NULL){
        m_waitInterfaceStage->remove();
        m_waitInterfaceStage = NULL;
    }
    flyStageParticle();
}

void AchievementNewPopUpView::onRetRewardInfo(CCObject* obj)
{
    if(m_waitInterfaceStage!=NULL){
        m_waitInterfaceStage->remove();
        m_waitInterfaceStage = NULL;
    }
    int curId = QuestController::getInstance()->m_stageRdId;
    int rewardId = atoi( CCCommonUtils::getPropById(CC_ITOA(curId), "reward").c_str() );
    int curNum = QuestController::getInstance()->m_stageNum;
    int startNum = atoi( CCCommonUtils::getPropById(CC_ITOA(curId), "para0").c_str() );
    int endNum = atoi( CCCommonUtils::getPropById(CC_ITOA(curId), "para1").c_str() );
    auto view = RewardShowInfoView::create( QuestController::getInstance()->m_stageRDMap[rewardId] );
    if (curNum < endNum) {
        view->setCallback(NULL, NULL, _lang("confirm"), _lang_1("2000291", CC_ITOA(endNum-curNum)));
    }else {
        view->setCallback(this, callfunc_selector(AchievementNewPopUpView::getStageRd), _lang("107516"), _lang("133251"));
    }
    string iconPath = CCCommonUtils::getPropById(CC_ITOA(curId), "icon");
    if (iconPath == "quest_gaojibaoxiang") {
        view->showTitle();
    }
    
    PopupViewController::getInstance()->addPopupView(view);
}

void AchievementNewPopUpView::getStageRd()
{
    m_waitInterfaceStage= GameController::getInstance()->showWaitInterface(m_stageBtn,-1);
    QuestController::getInstance()->startGetStageRd();
    
}

void AchievementNewPopUpView::onShowHand(float posx)
{
    if (m_flyArrow->isVisible()) {
        return;
    }
    m_flyArrow->stopAllActions();
    m_flyArrow->setPosition(ccp(posx, 0));
    m_flyArrow->setVisible(true);
    CCActionInterval * moveBy = CCMoveBy::create(0.5,ccp(20, -30));
    CCActionInterval * moveRBy = CCMoveBy::create(0.5,ccp(-20, 30));
    CCSequence* fadeAction = CCSequence::create(moveBy,moveRBy,NULL);
    CCActionInterval * fadeForever =CCRepeatForever::create((CCActionInterval* )fadeAction);
    m_flyArrow->runAction(fadeForever);
    auto particle1 = ParticleController::createParticle("FingerHit");
    particle1->setPosition(ccp(posx, 0));
    m_arrowPar->addChild(particle1);
    
    scheduleOnce(schedule_selector(AchievementNewPopUpView::hideFlyArrow), 5.0f);
}
void AchievementNewPopUpView::hideFlyArrow(float dt)
{
    m_flyArrow->setVisible(false);
    m_flyArrow->stopAllActions();
    m_arrowPar->removeAllChildren();
}

void AchievementNewPopUpView::sortQuestInfo()
{
    CCArray* arr = CCArray::create();
    map<std::string, QuestInfo*>::iterator it;
    for(it = QuestController::getInstance()->currentAchList.begin();it!=QuestController::getInstance()->currentAchList.end();it++){
        if(m_recommendinfo!=NULL){
            if(m_recommendinfo->itemId==it->second->itemId){
                continue;
            }
        }
        
        if(it->second->type==2){
            if(it->second->order2<=0){
                continue;
            }
            arr->addObject(it->second);
            //break;
        }
    }
    int num = arr->count()-1;
    for(int j=0;j<num;j++){
        for(int i=0;i<num-j;i++)
        {
            QuestInfo* quest1 = (QuestInfo*)arr->objectAtIndex(i);
            QuestInfo* quest2 = (QuestInfo*)arr->objectAtIndex(i+1);
            if(quest1->order2 > quest2->order2 || (quest1->state == ACCEPT && quest2->state != ACCEPT))//未接受的在前
            {
                arr->swap(i, i+1);
            }
        }
    }
    int leng = arr->count();
    //    leng = leng>4?4:leng;
    leng = leng>4?4:leng;
    for(int i=0;i<leng;i++ ){
        m_data->addObject(arr->objectAtIndex(i));
        
    }
}

void AchievementNewPopUpView::newSortQuestInfo()
{
    int leng = 4;
    int i = 0;
    int j = 0;
    int count1 = QuestController::getInstance()->speAchArray->count();
    int count2 = QuestController::getInstance()->castleAchArray->count();
    int count3 = QuestController::getInstance()->worldAchArray->count();
    int count4 = QuestController::getInstance()->monsterAchArray->count();
    int count5 = QuestController::getInstance()->otherAchArray->count();
    int count= MAX(count1, count2);
    count = MAX(count, count3);
    count = MAX(count, count4);
    count = MAX(count, count5);
    while (i < leng && j < count) {
        if (QuestController::getInstance()->speAchArray->count() > j && i < leng) {
            m_data->addObject(QuestController::getInstance()->speAchArray->objectAtIndex(j));
            i++;
        }
        if (QuestController::getInstance()->castleAchArray->count() > j && i < leng) {
            m_data->addObject(QuestController::getInstance()->castleAchArray->objectAtIndex(j));
            i++;
        }
        if (QuestController::getInstance()->worldAchArray->count() > j && i < leng) {
            m_data->addObject(QuestController::getInstance()->worldAchArray->objectAtIndex(j));
            i++;
        }
        if (QuestController::getInstance()->monsterAchArray->count() > j && i < leng) {
            m_data->addObject(QuestController::getInstance()->monsterAchArray->objectAtIndex(j));
            i++;
        }
        if (QuestController::getInstance()->otherAchArray->count() > j && i < leng) {
            m_data->addObject(QuestController::getInstance()->otherAchArray->objectAtIndex(j));
            i++;
        }
        j++;
    }
    
    int num = m_data->count()-1;
    for(int j=0;j<num;j++){
        for(int i=0;i<num-j;i++)
        {
            QuestInfo* quest1 = (QuestInfo*)m_data->objectAtIndex(i);
            QuestInfo* quest2 = (QuestInfo*)m_data->objectAtIndex(i+1);
            if((quest1->state != COMPLETE && quest2->state == COMPLETE))//已完成的在前
            {
                m_data->swap(i, i+1);
            }
        }
    }
}

//class AchievementNewTaskTableCell
AchievementNewTaskTableCell* AchievementNewTaskTableCell::create(QuestInfo *info, int index, Node* clickArea)
{
    AchievementNewTaskTableCell* ret = new AchievementNewTaskTableCell();
    if (ret && ret->init(info, index, clickArea))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AchievementNewTaskTableCell::init(QuestInfo *info, int index, Node* clickArea)
{
    this->setData(info, index, clickArea);
    return true;
}

void AchievementNewTaskTableCell::setData(QuestInfo *info, int index, Node* clickArea)
{
    if (mCell == NULL)
    {
        mCell = AchievementNewTaskCell::create(info, index,clickArea);
        addChild(mCell);
    }
    else
    {
        mCell->setData(info, index);
    }
}

CCNode* AchievementNewTaskTableCell::getGuideNode()
{
    return mCell->getGuideNode();
}

//class AchievementNewTaskCell
AchievementNewTaskCell *AchievementNewTaskCell::create(QuestInfo* info,int index,Node* clickArea){
    AchievementNewTaskCell *ret = new AchievementNewTaskCell(info,index,clickArea);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
bool AchievementNewTaskCell::init(){
    auto bg = CCBLoadFile("QuestCellTask", this, this);
    this->setContentSize(bg->getContentSize());
    CCCommonUtils::setButtonTitle(m_btnReward, _lang("107516").c_str());
    m_waitInterface = NULL;
    this->setData(m_info,m_index);
    return true;
}

CCNode* AchievementNewTaskCell::getGuideNode()
{
    if (m_btnReward && m_btnReward->isVisible() && m_btnReward->isEnabled()) {
        return m_btnReward;
    }
    return NULL;
}
void AchievementNewTaskCell::setData(QuestInfo* info,int index){
    m_info = info;
    m_index = index;
    if (m_index == m_changeIndex) {
        this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(AchievementNewTaskCell::animationCallBack));
        this->getAnimationManager()->runAnimationsForSequenceNamed("2");
    }
    else
       refresh();
}

void AchievementNewTaskCell::animationCallBack()
{
    m_changeIndex = -1;
    refresh();
}

void AchievementNewTaskCell::refresh()
{
    m_rewardClick = false;
    m_picHead->removeAllChildren();
    m_picHead->setScale(1.f);
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    if(m_info->state==COMPLETE){
        m_btnGoTo->setVisible(false);
        m_btnReward->setVisible(true);
        //   m_btnLabel->setVisible(true);
        if(m_info->itemId == "2900101" || m_info->itemId == "2900102" || m_info->itemId == "2900103"){
            if (m_info->itemId == "2900102") {
                CCCommonUtils::setButtonTitle(m_btnReward, _lang("133109").c_str());//领奖按钮显示【完成】，dialog=133109
            }else{
                CCCommonUtils::setButtonTitle(m_btnReward, _lang("107516").c_str());//rewardbutton文字变回原来的reward
            }
        }
    }else{
        m_btnGoTo->setVisible(true);
        m_btnReward->setVisible(false);
        //  m_btnLabel->setVisible(false);
    }
    if(m_info->curValue>=m_info->maxValue){
        m_btnReward->setVisible(true);
        m_btnGoTo->setVisible(false);
    }
    m_nameTxt->setString(m_info->name);
    //    std::string str = _lang("107526")+CC_CMDITOA(m_info->curValue>m_info->maxValue?m_info->maxValue:m_info->curValue);
    std::string str = CC_CMDITOA(m_info->curValue>m_info->maxValue?m_info->maxValue:m_info->curValue);
    str.append("/");
    str.append(CC_CMDITOA(m_info->maxValue));
    m_titleTxt->setString(_lang("107529"));
    //    m_descTxt->setString(str);
    CCSprite* pic1 =NULL;
    if(m_info->icon1!=""){
        std::string _picname = m_info->icon1;
        if (CCCommonUtils::isIosAndroidPad())
        {
            std::vector<std::string> _strvet;
            CCCommonUtils::splitString(_picname.c_str(), ".", _strvet);
            _picname = _strvet[0] + "_hd." + _strvet[1];
            auto __frame =  CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(_picname.c_str());
            if (!__frame)
            {
                _picname = m_info->icon1;
                m_picHead->setScale(2.f);
            }
        }
        
        pic1 = CCLoadSprite::createSprite(_picname.c_str());
        m_picHead->addChild(pic1);
    }
    if(m_info->icon2!=""){
        std::string _picname = m_info->icon2;
        if (CCCommonUtils::isIosAndroidPad())
        {
            std::vector<std::string> _strvet;
            CCCommonUtils::splitString(_picname.c_str(), ".", _strvet);
            _picname = _strvet[0] + "_hd." + _strvet[1];
            auto __frame =  CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(_picname.c_str());
            if (!__frame)
            {
                _picname = m_info->icon2;
                m_picHead->setScale(2.f);
            }
        }
        CCSprite* pic = CCLoadSprite::createSprite(_picname.c_str());
        m_picHead->addChild(pic);
        if(pic1!=NULL){
            pic->cocos2d::CCNode::setPosition(pic1->getContentSize().width/2-pic->getContentSize().width/2, -(pic1->getContentSize().height/2-pic->getContentSize().height/2));
        }
    }
    
    float pro = m_info->curValue*1.0/m_info->maxValue;
    pro = pro>1?1:pro;
    m_progress->setScaleX(pro);
    m_progressTxt->setString(str);
    
    //CCCommonUtils::setButtonSprite(m_btnReward, "btn_green3.png");
    if ((m_info->itemId == "2900101" || m_info->itemId == "2900102" || m_info->itemId == "2900103") && m_info->state == ACCEPT) {
        CCCommonUtils::setButtonTitle(m_btnReward, _lang("2000214").c_str());
        //        m_rewardClick = true;
        m_btnGoTo->setEnabled(false);
        m_btnGoTo->setVisible(false);
        m_btnReward->setVisible(true);
        //CCCommonUtils::setButtonSprite(m_btnReward, "but_blue.png");
    }
}

void AchievementNewTaskCell::onEnter(){
    CCNode::onEnter();
    setSwallowsTouches(false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 3, false);
}

void AchievementNewTaskCell::onExit(){
    setTouchEnabled(false);
    CCNode::onExit();
}

bool AchievementNewTaskCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_descTxt", CCLabelIF*, this->m_descTxt);
  //  CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnLabel", CCLabelIF*, this->m_btnLabel);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnReward", CCControlButton*, this->m_btnReward);
       CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnGoTo", CCControlButton*, this->m_btnGoTo);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_cellBG", CCNode*, this->m_cellBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_picHead", CCNode*, this->m_picHead);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_progress", CCScale9Sprite*, this->m_progress);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_progressTxt", CCLabelIF*, this->m_progressTxt);
    return false;
}

SEL_CCControlHandler AchievementNewTaskCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoToBtnClick", AchievementNewTaskCell::onGoToBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRewardBtnClick", AchievementNewTaskCell::onRewardBtnClick);
       // CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGoToBtnClick1", AchievementNewTaskCell::onGoToBtnClick1);
    return NULL;
}
void AchievementNewTaskCell::onGoToBtnClick(CCObject * pSender, Control::EventType pCCControlEvent){
    //SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    PopupViewController::getInstance()->addPopupInView(AchievementDetailPopUpView::create(m_info));
    
}
bool AchievementNewTaskCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_touchPoint = pTouch->getLocation();
    if (m_guideKey == "AC_normal") {
        return false;
    }
    if (isTouchInside(m_cellBG,pTouch) && isTouchInside(m_clickArea, pTouch)){
     return true;
    }
    return false;
}
void AchievementNewTaskCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (fabs(m_touchPoint.y - pTouch->getLocation().y)> 30) {
        return;
    }
    if (isTouchInside(m_cellBG,pTouch) && isTouchInside(m_clickArea, pTouch)) {
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        PopupViewController::getInstance()->addPopupInView(AchievementDetailPopUpView::create(m_info));
    }

}

void AchievementNewTaskCell::onRewardBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if ((m_info->itemId == "2900102" || m_info->itemId == "2900101") && m_info->state == ACCEPT)
    {
        int para = 1;
        if (m_info->itemId == "2900102") {//facebook like
            para = 2;
            GameController::getInstance()->gotoFaceBookLike();
        }
        else {//google 5星评价
            para = 1;
            GameController::getInstance()->updateVersion();
        }
        FSTaskCommand * cmd = new FSTaskCommand(para);
        cmd->sendAndRelease();
        CCCommonUtils::setButtonTitle(m_btnReward, _lang("107516").c_str());//rewardbutton文字变回原来的reward
        return;
    }
    
    if (m_info->itemId == "2900103" && m_info->state == ACCEPT) {
//        PopupViewController::getInstance()->addPopupView(GoldExchangeView::create(),false);
        PopupViewController::getInstance()->addPopupInView(GoldExchangeView_NEW::create());
        return;
    }
    
    if (m_rewardClick||QuestController::getInstance()->isGettingReward) {
        return;
    }
    
    int r_type = -1;
    if (m_info->rewardshow == "wood") {
        r_type = R_WOOD;
    }else if (m_info->rewardshow == "food") {
        r_type = R_FOOD;
    }else if (m_info->rewardshow == "iron") {
        r_type = R_IRON;
    }else if (m_info->rewardshow == "stone") {
        r_type = R_STONE;
    }else if (m_info->rewardshow == "gold") {
        r_type = R_GOLD;
    }
    if (r_type>-1) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification( MSG_PLAY_DROP_RD,CCInteger::create(r_type) );
    }
    
    m_changeIndex = m_index;
    m_rewardClick=true;
//    m_playAnimation=true;
    m_waitInterface=GameController::getInstance()->showWaitInterface(m_btnReward,-1);
    worldPt = m_btnReward->getParent()->convertToWorldSpace(m_btnReward->getPosition());
    QuestController::getInstance()->getReward(m_info->itemId);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           , CCString::createWithFormat("AC_reward"));
    
}

////////////////////////////
//奖励展示面板
RewardShowInfoView* RewardShowInfoView::create(CCArray* data)
{
    RewardShowInfoView* ret = new RewardShowInfoView();
    if (ret && ret->init(data))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool RewardShowInfoView::init(CCArray* data)
{
    bool bRet = false;
    do
    {
        if (!PopupBaseView::init())
        {
            break;
        }
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(502, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(502, false);
        });
        
        CCBLoadFile("DailyActiveBoxInfoView", this, this);
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        this->setContentSize(winSize);
        m_congratureNode->setVisible(false);
        
        int extH = this->getExtendHeight();
        if (!CCCommonUtils::isIosAndroidPad())
        {
            m_mainNode->setPositionY(m_mainNode->getPositionY() + extH/2);
        }
        m_titleTTF->setString(_lang("102139"));
        m_congratuTTF->setString(_lang("102139"));
        
        m_callbackTarget = NULL;
        m_callbackFunc = NULL;
        m_data = data;
        if (m_data == nullptr) {
            m_data = CCArray::create();
        }
        
        m_tableView = CCTableView::create(this, m_listNode->getContentSize());
        m_tableView->setDirection(kCCScrollViewDirectionVertical);
        m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_listNode->addChild(m_tableView);
        
        
        
        
        bRet = true;
    }while(0);
    return bRet;
}

void RewardShowInfoView::onEnter()
{
    CCNode::onEnter();
    setTouchEnabled(true);
    m_tableView->reloadData();
}

void RewardShowInfoView::onExit()
{
    setTouchEnabled(false);
    CCNode::onExit();
}

void RewardShowInfoView::onOkClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent)
{
    if (m_callbackTarget && m_callbackFunc) {
        (m_callbackTarget->*m_callbackFunc)();
    }
    closeSelf();
}

CCSize RewardShowInfoView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if (idx >= m_data->count())
    {
        return CCSizeZero;
    }
    return CCSizeMake(530, 100);
}

CCSize RewardShowInfoView::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(530, 100);
}

CCTableViewCell* RewardShowInfoView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if (idx >= m_data->count())
    {
        return NULL;
    }
    CCDictionary* dict = dynamic_cast<CCDictionary*>(m_data->objectAtIndex(idx));
    BoxInfoCell* cell = dynamic_cast<BoxInfoCell*>(table->dequeueCell());
    if (cell)
    {
        cell->setData(dict);
    }
    else
    {
        cell = BoxInfoCell::create(dict);
    }
    return cell;
}

ssize_t RewardShowInfoView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data->count();
}

bool RewardShowInfoView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void RewardShowInfoView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isTouchInside(m_touchNode, pTouch) && (ccpDistance(pTouch->getLocation(), pTouch->getStartLocation())<20))
    {
        closeSelf();
    }
}

bool RewardShowInfoView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_congratureNode", CCNode*, this->m_congratureNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode1", CCNode*, this->m_particleNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode2", CCNode*, this->m_particleNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTTF", CCLabelIF*, this->m_titleTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_congratuTTF", CCLabelIF*, this->m_congratuTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIF*, this->m_msgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_okBtn", CCControlButton*, this->m_okBtn);
    return false;
}

SEL_CCControlHandler RewardShowInfoView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOkClick", RewardShowInfoView::onOkClick);
    return NULL;
}

void RewardShowInfoView::setCallback(cocos2d::CCObject *target, SEL_CallFunc func, string btnName, string msg) {
    CCCommonUtils::setButtonTitle(m_okBtn, btnName.c_str());
    m_msgLabel->setString(msg);
    m_callbackTarget = target;
    m_callbackFunc = func;
}

void RewardShowInfoView::showTitle()
{
    m_congratureNode->setVisible(true);
    m_titleTTF->setVisible(false);
}
