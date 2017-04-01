//
//  UserUpgradeView.cpp
//  IF
//
//  Created by fubin on 14-4-8.
//
//

#include "UserUpgradeView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "GeneralManager.h"
#include "DataRecordCommand.h"
#include "RewardController.h"
#include "ParticleController.h"
#include "FlyRewardPic.h"
#include "UserLevelCommand.h"
#include "GuideController.h"
#include "SoundController.h"
//#include "CCLabel.h"
UserUpgradeView::UserUpgradeView(){
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UserUpgradeView::shareSuccess), MSG_FBFeedDialogResult, NULL);
};

UserUpgradeView::~UserUpgradeView(){
//    m_cellArr->release();
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FBFeedDialogResult);
};

void UserUpgradeView::shareSuccess(CCObject* param){
    this->scheduleOnce(schedule_selector(UserUpgradeView::delayClose), 0.12);
}

void UserUpgradeView::delayClose(float _time){
    if(m_waitInterFace){
        m_waitInterFace->remove();
        m_waitInterFace = NULL;
    }
   this->onRewardBtnClick(NULL, Control::EventType::TOUCH_DOWN);
}

UserUpgradeView* UserUpgradeView::create(){
    UserUpgradeView* ret = new UserUpgradeView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool UserUpgradeView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
//    m_cellArr = CCArray::create();
//    m_cellArr->retain();
    CCLoadSprite::doResourceByCommonIndex(505, true);
    CCLoadSprite::doResourceByCommonIndex(11, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(505, false);
        CCLoadSprite::doResourceByCommonIndex(11, false);
    });
    auto tmpCCB = CCBLoadFile("NewUserUpgradeView",this,this);
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    setContentSize(size);
    this->setModelLayerTouchCallback([&](cocos2d::CCTouch *pTouch){
        // detect touch inside panel
        if (isTouchInside(this, pTouch)) {
            this->onRewardBtnClick(NULL, Control::EventType::TOUCH_DOWN);
        }
    });
    vector_node.clear();
    vector_node.push_back(m_node1);
    vector_node.push_back(m_node2);
    vector_node.push_back(m_node3);
    vector_node.push_back(m_node4);
    vector_node.push_back(m_node5);
    vector_node.push_back(m_node6);
    vector_num.clear();
    vector_num.push_back(m_num1);
    vector_num.push_back(m_num2);
    vector_num.push_back(m_num3);
    vector_num.push_back(m_num4);
    vector_num.push_back(m_num5);
    vector_num.push_back(m_num6);
    vector_name.clear();
    vector_name.push_back(m_name1);
    vector_name.push_back(m_name2);
    vector_name.push_back(m_name3);
    vector_name.push_back(m_name4);
    vector_name.push_back(m_name5);
    vector_name.push_back(m_name6);
    vector_picNode.clear();
    vector_picNode.push_back(m_picNode1);
    vector_picNode.push_back(m_picNode2);
    vector_picNode.push_back(m_picNode3);
    vector_picNode.push_back(m_picNode4);
    vector_picNode.push_back(m_picNode5);
    vector_picNode.push_back(m_picNode6);
//    m_hatAnimNode->setPositionY(m_hatAnimNode->getPositionY()+40);
    m_titleLabel->setString(_lang("113184"));
    m_titleLabel->setColor({67,39,7});
    int userLv = GlobalData::shared()->lordInfo.levelUp+1;
    string lvInfo = CC_ITOA(userLv);
    if(userLv>=GlobalData::shared()->MaxPlayerLv) {
//        lvInfo += _lang("102183");
    }
   
    
    //等级
    m_lvLabel->setVisible(false);
    m_lvLabel->setString(lvInfo.c_str());
    auto label = LabelAtlas::create(lvInfo.c_str(), "player_upLv_num.png", 61.4, 85, 48);
    label->setAnchorPoint(Point::ANCHOR_MIDDLE);
    label->setPosition(ccp(0,0));
    //label->setString(lvInfo.c_str());
    m_levelNode->addChild(label,11);
    
    
    int lastEndowment = 0;
    int lastPower = 0;
    
    int key = 100100 + userLv-1;
    auto dict = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(key));
    int curEndowment = dict->valueForKey("endowment")->intValue();
    int curPower = dict->valueForKey("power")->intValue();
    if (userLv>2) {
        int lastkey = 100100 + (userLv-2);
        auto lastdict = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(lastkey));
        lastEndowment = lastdict->valueForKey("endowment")->intValue();
        lastPower = lastdict->valueForKey("power")->intValue();
    }
    
    m_powLabel->setFntFile("Arial_Bold_Regular.fnt");
    m_powNumLabel->setFntFile("Arial_Bold_Regular.fnt");
    m_skillLabel->setFntFile("Arial_Bold_Regular.fnt");
    m_skillNumLabel->setFntFile("Arial_Bold_Regular.fnt");
    m_powLabel->setString(_lang("105034"));
    string powerInfo = "";
    powerInfo.append(" +");
    powerInfo.append(CC_ITOA(curPower-lastPower));
    m_powNumLabel->setString(powerInfo);
    
    m_skillLabel->setString(_lang("105035"));
    string skillInfo = "";
    skillInfo.append(" +");
    skillInfo.append(CC_ITOA(curEndowment-lastEndowment));
    m_skillNumLabel->setString(skillInfo);
    
    float px=m_skillLabel->getPositionX() + m_skillLabel->getContentSize().width+20;
    
    //get reward item
    // devil 分享改成确定
//    m_fbTxt->setString(_lang("107098").c_str());
    m_fbTxt->setString(_lang("101274").c_str());
    
    string item =  dict->valueForKey("level_item")->getCString();   CCLOG("///i///%s",item.c_str());
    int offY = 150;
    if(item==""){
        return true;
    }
    CCCommonUtils::splitString(item, "|", vector_item);             CCLOG("///n///%s",vector_item[0].c_str());
    item =  dict->valueForKey("level_num")->getCString();          CCLOG("///n///%s",item.c_str());
    CCCommonUtils::splitString(item, "|", vector_number);
    
//    m_tabView = CCMultiColTableView::create(this, m_rewardlist->getContentSize());
//    m_tabView->setDirection(kCCScrollViewDirectionVertical);
//    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
//    m_tabView->setMultiColTableViewDelegate(this);
//    m_tabView->setTouchPriority(Touch_Default);
//    m_rewardlist->addChild(m_tabView);
//    m_tabView->setTouchEnabled(false);
//    m_tabView->reloadData();
    
    std:: map<string, CCDictionary*>::iterator it = GlobalData::shared()->shareFbmap.find("level_up");
    bool flag = false;
    if (it!=GlobalData::shared()->shareFbmap.end()) {
        CCDictionary* dict = it->second;
        if (dict) {
            int on = dict->valueForKey("ON")->intValue();
            if (on==1) {
                flag = true;
            }
        }
    }
    if(GlobalData::shared()->analyticID == "cn1" || GlobalData::shared()->analyticID.find("cn_")<GlobalData::shared()->analyticID.length()){//平台是国内的，不显示facebook 的share btn 按钮
        flag = false;
    }
    if(GlobalData::shared()->playerInfo.level>3 && flag){
//        m_likeNode->setPositionY(m_likeNode->getPositionY() - offY);
        m_likeNode->setVisible(true);
    }
    m_waitInterFace = NULL;
    if(GlobalData::shared()->playerInfo.level==6){
        CCPoint locPos = ccp(40, 70);
//        CCPoint pos = m_buildBG->convertToWorldSpace(locPos);
//        int len = (1-(pos.y/CCDirector::sharedDirector()->getWinSize().height))*100;
        GlobalData::shared()->isBind = true;

    }
    

    refreshView();
    return true;
}

void UserUpgradeView::doFinishFadeIn()
{
    aniFinish = true;
}

void UserUpgradeView::addNewPar(float t)
{
    auto particle = ParticleController::createParticle(CCString::createWithFormat("UserUpgrade")->getCString());
    particle->setPosition(0, 0);
    m_parNode->addChild(particle);
}

void UserUpgradeView::refreshView()
{
    int num = vector_item.size();
    int num1 = vector_number.size();
    int minCount = MIN(num, num1);
    int i = 0;
    int j = minCount;
    int type = R_GOODS;
    int value = atoi(vector_item[0].c_str());
    int count = atoi(vector_number[0].c_str());
    std::string str = "";
    while (vector_node.size() - j > 0) {
        if (j+1<vector_node.size()) {
            vector_node[j+1]->setVisible(false);
            j++;
        }
    }
    while (i < minCount && i < vector_node.size()) {
        string name = CCCommonUtils::getNameById(vector_item[i]);
        vector_name[i]->setString(name);
        vector_num[i]->setString(vector_number[i]);
        vector_picNode[i]->removeAllChildren();
        value = atoi(vector_item[i].c_str());
        auto icon = CCLoadSprite::createSprite(RewardController::getInstance()->getPicByType(type, value).c_str());
        CCCommonUtils::setSpriteMaxSize(icon, 80);
        vector_picNode[i]->addChild(icon);
        i++;
    }
}

void UserUpgradeView::onEnter(){
    PopupBaseView::onEnter();
    playLvAnim();
    m_dragon_effect_left->setBlendFunc({GL_ONE,GL_ONE});
    m_dragon_effect_right->setBlendFunc({GL_ONE,GL_ONE});
    aniFinish = false;
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(UserUpgradeView::doFinishFadeIn));
    this->getAnimationManager()->runAnimationsForSequenceNamed("Default Timeline");
    this->scheduleOnce(schedule_selector(UserUpgradeView::addNewPar), 1.0);
}

void UserUpgradeView::onExit(){
    GlobalData::shared()->isBind = false;
//    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(UserUpgradeView::playGiftAnim), this);
    PopupBaseView::onExit();
}

void UserUpgradeView::onShareBtnClick(CCObject * pSender, Control::EventType pCCControlEvent){
    m_fbShareBtn->setEnabled(false);
    cocos2d::extension::CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FBFeedDialogResult);

}

SEL_CCControlHandler UserUpgradeView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onShareBtnClick", UserUpgradeView::onShareBtnClick);
    return NULL;
}

bool UserUpgradeView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvLabel", CCLabelIF*, this->m_lvLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powLabel", CCLabelIF*, this->m_powLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_skillLabel", CCLabelIF*, this->m_skillLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powNumLabel", CCLabelIF*, this->m_powNumLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_skillNumLabel", CCLabelIF*, this->m_skillNumLabel);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rewardlist", CCNode*, this->m_rewardlist);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hatAnimNode", CCNode*, this->m_hatAnimNode);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_LvAnimNode", CCNode*, this->m_LvAnimNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_likeNode", CCNode*, this->m_likeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelNode", CCNode*, this->m_levelNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fbTxt", CCLabelIFTTF*, this->m_fbTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fbShareBtn", CCControlButton*, this->m_fbShareBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode1", Node*, m_picNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name1", CCLabelIF*, m_name1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num1", CCLabelIF*, m_num1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node1", Node*, m_node1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode2", Node*, m_picNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name2", CCLabelIF*, m_name2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num2", CCLabelIF*, m_num2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node2", Node*, m_node2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode3", Node*, m_picNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name3", CCLabelIF*, m_name3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num3", CCLabelIF*, m_num3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node3", Node*, m_node3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode4", Node*, m_picNode4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name4", CCLabelIF*, m_name4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num4", CCLabelIF*, m_num4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node4", Node*, m_node4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode5", Node*, m_picNode5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name5", CCLabelIF*, m_name5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num5", CCLabelIF*, m_num5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node5", Node*, m_node5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode6", Node*, m_picNode6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name6", CCLabelIF*, m_name6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num6", CCLabelIF*, m_num6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node6", Node*, m_node6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_parNode", Node*, m_parNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_par_outside", Node*, m_par_outside);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_par_inside", Node*, m_par_inside);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_par_star", Node*, m_par_star);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragon_effect_left", Sprite*, m_dragon_effect_left);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dragon_effect_right", Sprite*, m_dragon_effect_right);

    return false;
}

void UserUpgradeView::playLvAnim(){
    CCActionInterval * delatTime = CCDelayTime::create(0.15);
    CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(UserUpgradeView::playLvParticle));
    this->runAction(CCSequence::create(delatTime,  funcall,NULL));
}
void UserUpgradeView::playLvParticle(){
//    m_LvAnimNode->removeAllChildren();
    ParticleController::initParticle();
    
    auto praticle4 = ParticleController::createParticle("effect_star");
    praticle4->setScale(2);
    m_par_star->addChild(praticle4);
    auto praticle5 = ParticleController::createParticle("user_lvlup_inside");
    praticle5->setScale(1.4);
    m_par_inside->addChild(praticle5);
    auto praticle6 = ParticleController::createParticle("user_lvlup_outside");
    praticle6->setScale(1.6);
    praticle6->setOpacity(190);
    m_par_outside->addChild(praticle6);

    praticle4->setAutoRemoveOnFinish(true);
    praticle5->setAutoRemoveOnFinish(true);
    praticle6->setAutoRemoveOnFinish(true);
    
    SoundController::sharedSound()->playEffects(Music_Sfx_UI_levelup);

    m_falg = 0;
    m_endfalg = 0;
//    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(UserUpgradeView::playGiftAnim), this);
//    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(UserUpgradeView::playGiftAnim), this, 0.2f,5, 0.0f, false);

}

void UserUpgradeView::playGiftAnim(float t){
    m_falg++;
    auto newBatch = ParticleController::createParticleBatch();
    for (int i=1; i<5; i++) {
        
        auto praticle1 = ParticleController::createParticle(CCString::createWithFormat("Fireworks_%d",i)->getCString());
        praticle1->setAutoRemoveOnFinish(true);
        newBatch->addChild(praticle1);
    }
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    int maxX = size.width;
    int maxY = size.height;
    int x = rand() % maxX;
    int y = rand() % maxY;
    this->addChild(newBatch,1000,m_falg+1000);
    newBatch->setPosition(x, y);
    //给烟花加尾动画
    CCActionInterval * delatTime = CCDelayTime::create(0.35);
    CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(UserUpgradeView::playGiftEndAnim));
    this->runAction(CCSequence::create(delatTime,  funcall,NULL));
}
void UserUpgradeView::playGiftEndAnim(){
    m_endfalg++;
    auto node  = this->getChildByTag(m_endfalg+1000);
    if(node!=NULL){
        auto praticle1 = ParticleController::createParticle(CCString::createWithFormat("Fireworks_%d",5)->getCString());
        praticle1->setAutoRemoveOnFinish(true);
        node->addChild(praticle1);
    }

}
void UserUpgradeView::playGetRewardAnim(){
    int count = vector_node.size();
    float delay[count-1];

    vector<float> v;
    back_insert_iterator<std::vector<float> > p = back_inserter(v);
    for(int i = 0; i < count; ++i){
        *p = 0.1*(i+1);
    }
    random_shuffle(v.begin(), v.end());

    for (int i = 0; i<count; i++) {
        Node* cellNode = vector_node[i];
        if(cellNode->isVisible()){
            float delaytime = v[i];
            int type = R_GOODS;
            int value = atoi(vector_item[i].c_str());
            string picth = RewardController::getInstance()->getPicByType(type, value);
            CCPoint p = cellNode->getPosition();
            CCDirector::sharedDirector()->convertToGL(p);
            CCPoint p2 = cellNode->getParent()->convertToWorldSpace(p);
        //CCPoint localP = m_gView->convertToNodeSpaceAR(p2);
            FlyRewardPic::addFlyRewardAnim("goods",p2 , picth,delaytime);
            SoundController::sharedSound()->playEffects(Music_Sfx_UI_collect_item);
        }
    }
}

void UserUpgradeView::onSkillBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{

}

void UserUpgradeView::onOKBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (GlobalData::shared()->playerInfo.level == 2) {
        CCArray* p = CCArray::create();
        p->addObject(CCInteger::create(UPGRADE_OK));
        CCArray* d = CCArray::create();
        
        DataRecordCommand* cmd = new DataRecordCommand(OPEN_PANEL, p, d);
        cmd->sendAndRelease();
    }
    
    PopupViewController::getInstance()->removePopupView(this);
}
void UserUpgradeView::onRewardBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (aniFinish == false) {
        aniFinish = true;
        this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
        this->getAnimationManager()->runAnimations("Finish");
        return;
    }
    if(vector_item.size()>0){
        playGetRewardAnim();
    }
    int level = GlobalData::shared()->lordInfo.levelUp+1;
    UserLevelRewardCommand* cmd = new UserLevelRewardCommand(level);
    cmd->sendAndRelease();
    
    
//    if (level==3) {//引导使用新手礼包
//        if (CCCommonUtils::isTestPlatformAndServer("Guide_skill")) {
//            GuideController::share()->setGuide("3240100",false);
//        }
//        else
//            GuideController::share()->setGuide("3100100",false);//3100100
//    }
//    else if (level==5 && !GlobalData::shared()->isXiaoMiPlatForm() && !CCCommonUtils::IsBandOfAcount() ) {//引导绑定
//        GuideController::share()->setGuide("3150100",false);
//    }
//    else {
        string guideId = GuideController::share()->TmpGuideId;
        if (guideId != "") {
            GuideController::share()->setGuide(guideId, false);
        }
//    }
    
    this->closeSelf();
//    CCDictionary* tmp = CCDictionary::create();
//    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PLAYER_LEVELUP,tmp);
}
void UserUpgradeView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize UserUpgradeView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    return CCSize(140, 140);
}

CCTableViewCell* UserUpgradeView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, ssize_t idx){
    int num = vector_item.size();
    int type = R_GOODS;
    int value = atoi(vector_item[0].c_str());
    int count = atoi(vector_item[0].c_str());
    std::string str = "";
    if(idx >= num){
        return NULL;
    }
    CCTableViewCell* pCell = table->dequeueGrid();
    if(idx < num){
        if(!pCell){
            pCell = new CCTableViewCell();
            pCell->autorelease();
            CCNode* node = CCNode::create();
            auto bg = CCLoadSprite::createScale9Sprite("icon_kuang.png");
            bg->setPreferredSize(CCSize(98,98));
            node->addChild(bg);
            string name = CCCommonUtils::getNameById(vector_item[idx]);
            auto text = CCLabelIF::create(name.c_str());
            text->setDimensions(CCSize(140, 0));
            text->setAnchorPoint(ccp(0.5,1.0));
            text->setAlignment(kCCTextAlignmentCenter);
            text->setVerticalAlignment(kCCVerticalTextAlignmentTop);
            text->setFontSize(16);
            text->setColor({60,28,0});
            text->setPosition(ccp(0, -50));
            node->addChild(text);
            value = atoi(vector_item[idx].c_str());
            auto icon = CCLoadSprite::createScale9Sprite(RewardController::getInstance()->getPicByType(type, value).c_str());
            float scale = 94/icon->getContentSize().width;
            if(scale>1){
                scale = 1.0;
            }
            icon->setScale(scale);
            node->addChild(icon,999,999);
            node->setPosition(ccp(70,90));
            pCell->addChild(node,1000,1000);
//            m_cellArr->addObject(pCell);
        }else{
            
        }
    }
    return pCell;

}

ssize_t UserUpgradeView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    int num = vector_item.size();
    return ceil(num/3.0);
}

ssize_t UserUpgradeView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 3;
}
