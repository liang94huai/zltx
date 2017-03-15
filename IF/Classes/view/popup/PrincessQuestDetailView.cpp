//
//  PrincessQuestDetailView.cpp
//  IF
//
//  Created by 王超一 on 16/1/12.
//
//

#include "PrincessQuestDetailView.hpp"
#include "PrincessQuestController.hpp"
#include "PopupViewController.h"
#include "RewardController.h"
#include "UIComponent.h"
#include "AchievementDetailPopUpView.h"
#include "SoundController.h"
#include "DropRdCCB.h"
#include "QuestRewardCommand.h"
#include "LuaController.h"
#include "CCMathUtils.h"
#include "GuideController.h"
#include "RollingText.h"
#include "ParticleFireAni.h"

//static string m_guideKey = "";

PrincessQuestDetailView *PrincessQuestDetailView::create(PrincessQuestInfo* info){
    PrincessQuestDetailView *ret = new PrincessQuestDetailView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

PrincessQuestDetailView::~PrincessQuestDetailView(){
    
}
void PrincessQuestDetailView::onEnter(){
    PopupBaseView::onEnter();
    CCLoadSprite::doResourceByCommonIndex(101, true);
    setSwallowsTouches(true);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    m_tabView->setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(PrincessQuestDetailView::refreshView), PRINCESS_QUEST_STATE_UPDATE, NULL);
    this->refreshView(NULL);
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void PrincessQuestDetailView::onExit(){
    setTouchEnabled(false);
    m_tabView->setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, PRINCESS_QUEST_STATE_UPDATE);
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    PopupBaseView::onExit();
}

bool PrincessQuestDetailView::init(){
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
        CCLoadSprite::doResourceByCommonIndex(5, true);
        CCLoadSprite::doResourceByCommonIndex(101, true);
        CCLoadSprite::doResourceByCommonIndex(7, true);
        CCLoadSprite::doResourceByCommonIndex(505, true);
        
        setCleanFunction([](){
            if (CCCommonUtils::isIosAndroidPad())
            {
                std::string _path = CCFileUtils::sharedFileUtils()->getWritablePath()+ "dresource/" + COMMON_PATH_101_HD;
                if (CCFileUtils::sharedFileUtils()->isFileExist(_path)) {
                    _path = CCFileUtils::sharedFileUtils()->getWritablePath()+ "dresource/" + COMMON_PATH_HD;
                    CCLoadSprite::doResourceByPathIndex(_path, 101, false);
                }
            }
            CCLoadSprite::doResourceByCommonIndex(5, false);
            CCLoadSprite::doResourceByCommonIndex(101, false);
            CCLoadSprite::doResourceByCommonIndex(7, false);
            CCLoadSprite::doResourceByCommonIndex(505, false);
        });
        auto bg = CCBLoadFile("PrincessQuestDetailView", this, this);
        this->setContentSize(bg->getContentSize());
        setTitleName(_lang("2000294"));
        
        int preHeight = this->m_viewBg->getContentSize().height;
        changeBGHeight(m_viewBg);
        
        int dh = m_viewBg->getContentSize().height - preHeight;
        this->m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width, m_listContainer->getContentSize().height + dh));
        if (CCCommonUtils::isIosAndroidPad()) {
            dh /= 2.4;
        }
        m_bottomNode->setPositionY(m_bottomNode->getPositionY()-dh);
        
        ParticleFireAni* par = ParticleFireAni::create();
        m_fireNode1->addChild(par);
        
        ParticleFireAni* par2 = ParticleFireAni::create();
        m_fireNode2->addChild(par2);
        
        m_data = CCArray::create();
        m_waitInterface=NULL;
        m_tabView = CCTableView::create(this, m_listContainer->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setDelegate(this);
        m_tabView->setTouchPriority(2);
        m_listContainer->addChild(m_tabView);
        CCCommonUtils::setButtonTitle(m_btnReward, _lang("107516").c_str());
        //主任务内容
        m_grey = false;
        m_nameTxt->setString(m_info->name);
        m_descTxt->setString(m_info->description);
        m_rwdTxt->setString(_lang("2000297"));
        m_timeTxt->setString(_lang("2000298"));
        m_timeOutTxt->setString(_lang("2000296"));
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
                }
                m_headNode->setScale(0.5f);
            }
            
            pic1 = CCLoadSprite::createSprite(_picname.c_str());
            m_headNode->addChild(pic1);
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
                }
                m_headNode->setScale(0.5f);
            }
            CCSprite* pic = CCLoadSprite::createSprite(_picname.c_str());
            m_headNode->addChild(pic);
            if(pic1!=NULL){
                pic->cocos2d::CCNode::setPosition(pic1->getContentSize().width/2-pic->getContentSize().width/2, -(pic1->getContentSize().height/2-pic->getContentSize().height/2));
            }
        }
        //滚动提示
        auto arr = CCArray::create();
        std::string nameStr = m_info->description;
        arr->addObject(CCString::create(nameStr));
        auto rollText = RollingText::create(arr, 500);
        m_tipNode->addChild(rollText);
        //奖励
        if (m_info->rwdInTime && m_info->rwdInTime->count() > 0) {
            setReward(m_info->rwdInTime);
        }
        if (m_info->rwdOutOfTime && m_info->rwdOutOfTime->count() > 0) {
            setReward(m_info->rwdOutOfTime, true);
        }
        ret = true;
    }
    return ret;
}

void PrincessQuestDetailView::setReward(CCArray* arr, bool inTime)
{
    if (inTime) {
        int count = arr->count();
        if (count > 3) {
            count = 3;
        }
        for (int i = 0 ; i < count; i++) {
            CCDictionary* dic = dynamic_cast<CCDictionary*>(arr->objectAtIndex(i));
            string num = "X";
            int type = dic->valueForKey("type")->intValue();
            string icon = "";
            string toolBgIcon = "";
            auto node = Node::create();
            node->setContentSize(CCSizeMake(100, 100));
            auto label = CCLabelIF::create();
            label->setFontSize(20);
            label->setColor({220,195,161});
            label->setAnchorPoint(ccp(1, 0.5));
            auto kuang = CCLoadSprite::createSprite("icon_kuang.png");
            CCCommonUtils::setSpriteMaxSize(kuang, 100);
            kuang->setPosition(50, 50);
            node->addChild(kuang);
            if (type == R_GOODS)
            {
                CCDictionary* value = (CCDictionary*)dic->objectForKey("value");
                string itemId = value->valueForKey("id")->getCString();
                ToolInfo toolInfo = ToolController::getInstance()->getToolInfoById(atoi(itemId.c_str()));
                icon = toolInfo.icon;
                icon.append(".png");
                num.append(CC_CMDITOA(value->valueForKey("num")->intValue()));
                toolBgIcon = CCCommonUtils::getToolBgByColor(toolInfo.color);
                CCCommonUtils::createGoodsIcon(atoi(itemId.c_str()), node, CCSize(100,100));
                label->setPosition(90,15);
            }
            else
            {
                icon = RewardController::getInstance()->getPicByType(type, -1);
                auto sprite = CCLoadSprite::createSprite(icon.c_str());
                CCCommonUtils::setSpriteMaxSize(sprite, 100);
                sprite->setPosition(50,50);
                node->addChild(sprite);
                label->setPosition(90,15);
                num = CC_CMDITOA(dic->valueForKey("value")->intValue());
            }
            label->setString(num);
            node->addChild(label);
            node->setPosition(i * 110, 0);
            node->setTag(i + 100);
            m_rwdExNode->addChild(node);
        }
    }
    else {
        if (arr->count() > 0) {
            CCDictionary* dic = dynamic_cast<CCDictionary*>(arr->objectAtIndex(0));
            string num = "X";
            int type = dic->valueForKey("type")->intValue();
            string icon = "";
            string toolBgIcon = "";
            auto node = Node::create();
            node->setContentSize(CCSizeMake(100, 100));
            auto label = CCLabelIF::create();
            label->setFontSize(20);
            label->setColor({220,195,161});
            label->setAnchorPoint(ccp(1, 0.5));
            auto kuang = CCLoadSprite::createSprite("icon_kuang.png");
            CCCommonUtils::setSpriteMaxSize(kuang, 100);
            kuang->setPosition(50, 50);
            node->addChild(kuang);
            if (type == R_GOODS)
            {
                CCDictionary* value = (CCDictionary*)dic->objectForKey("value");
                string itemId = value->valueForKey("id")->getCString();
                ToolInfo toolInfo = ToolController::getInstance()->getToolInfoById(atoi(itemId.c_str()));
                icon = toolInfo.icon;
                icon.append(".png");
                num.append(CC_CMDITOA(value->valueForKey("num")->intValue()));
                toolBgIcon = CCCommonUtils::getToolBgByColor(toolInfo.color);
                CCCommonUtils::createGoodsIcon(atoi(itemId.c_str()), node, CCSize(100,100));
                label->setPosition(90,15);
            }
            else
            {
                icon = RewardController::getInstance()->getPicByType(type, -1);
                auto sprite = CCLoadSprite::createSprite(icon.c_str());
                CCCommonUtils::setSpriteMaxSize(sprite, 100);
                sprite->setPosition(50,50);
                node->addChild(sprite);
                label->setPosition(90,15);
                num = CC_CMDITOA(dic->valueForKey("value")->intValue());
            }
            label->setString(num);
            node->addChild(label);
            node->setPosition(0, 0);
            node->setTag(99);
            m_rwdNode->addChild(node);
        }
    }
}

void PrincessQuestDetailView::update(float sec)
{
    if (m_info->state == PrincessUnfinish) {
        double now = GlobalData::shared()->getTimeStamp();
        double left = m_info->limitedTime - now;
        if (left > 0) {
            m_timeOutTxt->setVisible(false);
            m_time->setVisible(true);
            m_time->setColor({0,179,0});
            m_time->setString(CC_DSECTOA(left));
        }else {
            m_time->setVisible(false);
            m_timeTxt->setVisible(false);
            m_timeOutTxt->setVisible(true);
            setGrey();
        }
    }
    else if (m_info->state == PrincessComplete) {
        if (m_info->finishTime <= m_info->limitedTime) {
            m_timeTxt->setVisible(false);
            m_time->setString(_lang("2000297"));
            m_time->setColor({187,151,122});
            m_timeOutTxt->setVisible(false);
        }
        else {
            m_time->setVisible(false);
            m_timeTxt->setVisible(false);
            m_timeOutTxt->setVisible(true);
            setGrey();
        }
    }
}

void PrincessQuestDetailView::onRewardBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    m_btnReward->setEnabled(false);
//    auto dropCCB = DropRdCCB::create(r_type);
//    if (CCCommonUtils::isIosAndroidPad())
//    {
//        dropCCB->setScale(2);
//    }
//    this->addChild(dropCCB);
//    m_waitInterface= GameController::getInstance()->showWaitInterface(m_btnReward,-1);
    PrincessQuestController::getInstance()->getReward(m_info->itemId, true);
    PopupViewController::getInstance()->removePopupView(this);
}

void PrincessQuestDetailView::refreshView(CCObject * obj){
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    m_data->removeAllObjects();
    sortQuestInfo();
    
//    if(m_data->count()<=0){
//        m_noQuestText->setString(_lang("107530"));
//        m_noQuestText->setVisible(true);
//    }else{
//        m_noQuestText->setVisible(false);
//    }
    
    m_btnReward->setEnabled(false);
    //判断任务状态
    bool finish = true;
    if (m_info->main == 1) {
        vector<string> temp;
        CCCommonUtils::splitString(m_info->subTask, ";", temp);
        for (int i = 0; i < temp.size(); i++) {
            map<std::string, PrincessQuestInfo*>::iterator it = PrincessQuestController::getInstance()->currentAchList.find(temp[i]);
            if (it != PrincessQuestController::getInstance()->currentAchList.end()) {
                if (it->second->state == PrincessUnfinish || it->second->state == PrincessReward) {
                    finish = false;
                    break;
                }
            }
        }
    }
    if (m_info->state == PrincessComplete) {
        m_btnReward->setEnabled(true);
    }else if (m_info->state == PrincessUnfinish && finish) {
        m_btnReward->setEnabled(true);
    }
    //判断时间状态
    if (m_info->limitedTime < GlobalData::shared()->getTimeStamp() && m_info->state == PrincessUnfinish) {
        setGrey();
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
}

void PrincessQuestDetailView::setGrey()
{
    if (m_grey == false) {
        m_grey = true;
        if (m_rwdExNode && m_rwdExNode->getChildByTag(100)) {
            //图片置灰
            auto& children = m_rwdExNode->getChildByTag(100)->getChildren();
            for (auto child : children)
            {
                CCSprite* tmpSpr = dynamic_cast<CCSprite*>(child);
                if (tmpSpr) {
                    CCCommonUtils::setSpriteGray(tmpSpr, true);
                }
            }
        }
        if (m_rwdExNode && m_rwdExNode->getChildByTag(101)) {
            //图片置灰
            auto& children = m_rwdExNode->getChildByTag(101)->getChildren();
            for (auto child : children)
            {
                CCSprite* tmpSpr = dynamic_cast<CCSprite*>(child);
                if (tmpSpr) {
                    CCCommonUtils::setSpriteGray(tmpSpr, true);
                }
            }
        }
        if (m_rwdExNode && m_rwdExNode->getChildByTag(102)) {
            //图片置灰
            auto& children = m_rwdExNode->getChildByTag(102)->getChildren();
            for (auto child : children)
            {
                CCSprite* tmpSpr = dynamic_cast<CCSprite*>(child);
                if (tmpSpr) {
                    CCCommonUtils::setSpriteGray(tmpSpr, true);
                }
            }
        }
    }
}

int PrincessQuestDetailView::getContentOffSety(){
    int index = 0;
    int num = m_data->count()-1;
    for(int j=0;j<num;j++){
        PrincessQuestInfo* quest1 = (PrincessQuestInfo*)m_data->objectAtIndex(j);
        if(quest1->state==PrincessComplete){
            break;
        }
        index++;
        j++;
    }
    return index;
}

void PrincessQuestDetailView::onPlayDropRD(CCObject* obj)
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

bool PrincessQuestDetailView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCScale9Sprite*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_descTxt", CCLabelIF*, this->m_descTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnReward", CCControlButton*, this->m_btnReward);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_headNode", CCNode*, this->m_headNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", Node*, m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", Node*, m_fireNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipNode", Node*, m_tipNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", Node*, m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdTxt", CCLabelIF*, m_rwdTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeOutTxt", CCLabelIF*, m_timeOutTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeTxt", CCLabelIF*, m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_time", CCLabelIF*, m_time);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdNode", Node*, m_rwdNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rwdExNode", Node*, m_rwdExNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desName", CCLabelIF*, m_desName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desBg", Scale9Sprite*, m_desBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode", Node*, m_desNode);
    return false;
}


SEL_CCControlHandler PrincessQuestDetailView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRewardBtnClick", PrincessQuestDetailView::onRewardBtnClick);
    return NULL;
}
void PrincessQuestDetailView::setRewardDesc(int index)
{
    if (index == 3) {
        if (m_info->rwdInTime && m_info->rwdInTime->count() > 0) {
            CCDictionary* dic = dynamic_cast<CCDictionary*>(m_info->rwdInTime->objectAtIndex(0));
            int type = dic->valueForKey("type")->intValue();
            if (type == R_GOODS)
            {
                CCDictionary* value = (CCDictionary*)dic->objectForKey("value");
                string itemId = value->valueForKey("id")->getCString();
                ToolInfo toolInfo = ToolController::getInstance()->getToolInfoById(atoi(itemId.c_str()));
                m_desNode->setVisible(true);
                m_desName->setString(toolInfo.getName());
                m_desLabel->setString(_lang(toolInfo.des));
                float extH = m_desLabel->getContentSize().height - 80;
                extH = extH>0?extH:0;
                m_desBg->setPreferredSize(CCSizeMake(295, 149+extH));
            }

        }
    }
    else if (index >=0 && index <=2) {
        if (m_info->rwdOutOfTime && m_info->rwdOutOfTime->count() > index) {
            CCDictionary* dic = dynamic_cast<CCDictionary*>(m_info->rwdOutOfTime->objectAtIndex(index));
            int type = dic->valueForKey("type")->intValue();
            if (type == R_GOODS)
            {
                CCDictionary* value = (CCDictionary*)dic->objectForKey("value");
                string itemId = value->valueForKey("id")->getCString();
                ToolInfo toolInfo = ToolController::getInstance()->getToolInfoById(atoi(itemId.c_str()));
                m_desNode->setVisible(true);
                m_desName->setString(toolInfo.getName());
                m_desLabel->setString(_lang(toolInfo.des));
                float extH = m_desLabel->getContentSize().height - 80;
                extH = extH>0?extH:0;
                m_desBg->setPreferredSize(CCSizeMake(295, 149+extH));
            }
            
        }
    }
}
bool PrincessQuestDetailView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_touchPoint = pTouch->getLocation();
    if (m_rwdNode->getChildByTag(99) && isTouchInside(m_rwdNode->getChildByTag(99), pTouch)) {
        return true;
    }
    if (m_rwdExNode->getChildByTag(100) && isTouchInside(m_rwdExNode->getChildByTag(100), pTouch)) {
        return true;
    }
    if (m_rwdExNode->getChildByTag(101) && isTouchInside(m_rwdExNode->getChildByTag(101), pTouch)) {
        return true;
    }
    if (m_rwdExNode->getChildByTag(102) && isTouchInside(m_rwdExNode->getChildByTag(102), pTouch)) {
        return true;
    }
    return false;
}
void PrincessQuestDetailView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (fabs(m_touchPoint.y - pTouch->getLocation().y)> 30) {
        return;
    }
    if (m_rwdNode->getChildByTag(99) && isTouchInside(m_rwdNode->getChildByTag(99), pTouch)) {
        if (m_desNode->isVisible()) {
            m_desNode->setVisible(false);
        }
        else
            setRewardDesc(3);
    }
    if (m_rwdExNode->getChildByTag(100) && isTouchInside(m_rwdExNode->getChildByTag(100), pTouch)) {
        if (m_desNode->isVisible()) {
            m_desNode->setVisible(false);
        }
        else
            setRewardDesc(0);
    }
    if (m_rwdExNode->getChildByTag(101) && isTouchInside(m_rwdExNode->getChildByTag(101), pTouch)) {
        if (m_desNode->isVisible()) {
            m_desNode->setVisible(false);
        }
        else
            setRewardDesc(1);
    }
    if (m_rwdExNode->getChildByTag(102) && isTouchInside(m_rwdExNode->getChildByTag(102), pTouch)) {
        if (m_desNode->isVisible()) {
            m_desNode->setVisible(false);
        }
        else
            setRewardDesc(2);
    }
}

CCSize PrincessQuestDetailView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(1464, 360);
    }
    
    return CCSize(610, 150);
}

CCSize PrincessQuestDetailView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(1464, 360);
    }
    
    return CCSize(610, 150);
}

CCTableViewCell* PrincessQuestDetailView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    PrincessQuestDetailCell* cell = dynamic_cast<PrincessQuestDetailCell*>( table->dequeueCell());
    PrincessQuestInfo* info = dynamic_cast<PrincessQuestInfo*>(m_data->objectAtIndex(idx));
    if( info == NULL )
        return  NULL ;
    
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info);
        }else{
            cell = PrincessQuestDetailCell::create(info);
        }
    }
    return cell;
}

ssize_t PrincessQuestDetailView::numberOfCellsInTableView(CCTableView *table){
    int count = m_data->count();
    return count;
}

void PrincessQuestDetailView::scrollViewDidScroll(CCScrollView* view){
    float mindy = m_tabView->minContainerOffset().y;
    float dy = m_tabView->getContentOffset().y;
    if(dy<mindy){
        m_tabView->setContentOffset(ccp(0, mindy));
    }
}

//CCNode * PrincessQuestDetailView::getGuideNode(string _key)
//{
//    return NULL;
//}

void PrincessQuestDetailView::sortQuestInfo()
{
    vector<string> temp;
    CCCommonUtils::splitString(m_info->subTask, ";", temp);
    for (int i = 0; i < temp.size(); i++) {
        map<std::string, PrincessQuestInfo*>::iterator it = PrincessQuestController::getInstance()->currentAchList.find(temp[i]);
        if (it != PrincessQuestController::getInstance()->currentAchList.end()) {
            m_data->addObject(it->second);
        }
    }
}

PrincessQuestDetailCell *PrincessQuestDetailCell::create(PrincessQuestInfo* info){
    PrincessQuestDetailCell *ret = new PrincessQuestDetailCell(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
bool PrincessQuestDetailCell::init(){
    auto bg = CCBLoadFile("PrincessQuestDetailCell", this, this);
    this->setContentSize(bg->getContentSize());
    this->setData(m_info);
    return true;
}

//CCNode* PrincessQuestDetailCell::getGuideNode()
//{
//    return NULL;
//}
void PrincessQuestDetailCell::setData(PrincessQuestInfo* info){
    m_info = info;
    refresh();
}

void PrincessQuestDetailCell::refresh()
{
    m_picHead->removeAllChildren();
    m_picHead->setScale(1.f);
    m_btnGoTo->setVisible(false);
    m_complete->setVisible(false);
    if(m_info->state==PrincessComplete){
        m_btnGoTo->setVisible(false);
        m_complete->setVisible(true);
    }else if (m_info->state==PrincessUnfinish) {
        if (m_info->gotype != -1) {
            m_btnGoTo->setVisible(true);
            CCCommonUtils::setButtonTitle(m_btnGoTo, _lang("2000214").c_str());
        }
        m_complete->setVisible(false);
    }
    std::string str = CC_CMDITOA(m_info->curValue>m_info->maxValue?m_info->maxValue:m_info->curValue);
    str.append("/");
    str.append(CC_CMDITOA(m_info->maxValue));
    m_descTxt->setString(m_info->name);
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
            }
            m_picHead->setScale(0.5);
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
            }
            m_picHead->setScale(0.5);
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
}

//void PrincessQuestDetailCell::onEnter(){
//    CCNode::onEnter();
//}
//
//void PrincessQuestDetailCell::onExit(){
//    CCNode::onExit();
//}

bool PrincessQuestDetailCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_descTxt", CCLabelIF*, this->m_descTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnGoTo", CCControlButton*, this->m_btnGoTo);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_picHead", CCNode*, this->m_picHead);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_progress", CCScale9Sprite*, this->m_progress);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_progressTxt", CCLabelIF*, this->m_progressTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_complete", Sprite*, m_complete);
    return false;
}

SEL_CCControlHandler PrincessQuestDetailCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGotoBtnClick", ::PrincessQuestDetailCell::onGotoBtnClick);
    return NULL;
}
void PrincessQuestDetailCell::onGotoBtnClick(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->removeAllPopupView();
    PrincessQuestController::getInstance()->goToQuestTarget(m_info);
}
//bool PrincessQuestDetailCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
//{
//    return false;
//}
//void PrincessQuestDetailCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
//}
