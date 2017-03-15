//
//  PrincessQuestView.cpp
//  IF
//
//  Created by 王超一 on 16/1/12.
//
//

#include "PrincessQuestView.hpp"
#include "PrincessQuestController.hpp"
#include "PopupViewController.h"
#include "RewardController.h"
#include "UIComponent.h"
#include "SoundController.h"
#include "QuestRewardCommand.h"
#include "LuaController.h"
#include "CCMathUtils.h"
#include "GuideController.h"
#include "FunBuildController.h"
#include "ParticleFireAni.h"
#include "PrincessQuestDetailView.hpp"

//static string m_guideKey = "";

PrincessQuestView *PrincessQuestView::create(){
    PrincessQuestView *ret = new PrincessQuestView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

PrincessQuestView::~PrincessQuestView(){
    
}
void PrincessQuestView::onEnter(){
    PopupBaseView::onEnter();
    CCLoadSprite::doResourceByCommonIndex(101, true);
    setSwallowsTouches(true);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    m_tabView->setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(PrincessQuestView::refreshView), PRINCESS_QUEST_STATE_UPDATE, NULL);
    this->refreshView(NULL);
}

void PrincessQuestView::onExit(){
    setTouchEnabled(false);
    m_tabView->setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, PRINCESS_QUEST_STATE_UPDATE);
    PopupBaseView::onExit();
}

bool PrincessQuestView::init(){
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
        CCLoadSprite::doResourceByCommonIndex(305, true);
        CCLoadSprite::doResourceByCommonIndex(6, true);
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
            CCLoadSprite::doResourceByCommonIndex(305, false);
            CCLoadSprite::doResourceByCommonIndex(6, false);
            CCLoadSprite::doResourceByCommonIndex(505, false);
        });
        auto bg = CCBLoadFile("PrincessQuestView", this, this);
        this->setContentSize(bg->getContentSize());
        setTitleName(_lang("2000294"));
        
        int preHeight = this->m_viewBg->getContentSize().height;
        changeBGHeight(m_viewBg);
        
        int dh = m_viewBg->getContentSize().height - preHeight;
        this->m_listContainer->setContentSize(CCSize(m_listContainer->getContentSize().width, m_listContainer->getContentSize().height + dh));
        
        ParticleFireAni* par = ParticleFireAni::create();
        m_fireNode1->addChild(par);
        
        ParticleFireAni* par2 = ParticleFireAni::create();
        m_fireNode2->addChild(par2);
        
        m_data = CCArray::create();
        m_tabView = CCTableView::create(this, m_listContainer->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setDelegate(this);
        m_tabView->setTouchPriority(2);
        m_listContainer->addChild(m_tabView);
        m_dialog->setString(_lang("2000295"));
        m_dialogBg->setContentSize(Size(m_dialogBg->getContentSize().width, m_dialog->getContentSize().height * m_dialog->getOriginScaleY() + 10));
        ret = true;
    }
    return ret;
}

void PrincessQuestView::refreshView(CCObject * obj){
    m_data->removeAllObjects();
    sortQuestInfo();
    
//    if(m_data->count()<=0){
//        m_noQuestText->setString(_lang("107530"));
//        m_noQuestText->setVisible(true);
//    }else{
//        m_noQuestText->setVisible(false);
//    }
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

int PrincessQuestView::getContentOffSety(){
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

bool PrincessQuestView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCScale9Sprite*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_dialogBg", CCScale9Sprite*, this->m_dialogBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_dialog", CCLabelIF*, this->m_dialog);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_princess", Node*, m_princess);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", Node*, m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", Node*, m_fireNode2);
    return false;
}


SEL_CCControlHandler PrincessQuestView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}
bool PrincessQuestView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_touchPoint = pTouch->getLocation();
    if (isTouchInside(m_princess, pTouch)) {
        return true;
    }
    return false;
}
void PrincessQuestView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (fabs(m_touchPoint.y - pTouch->getLocation().y)> 30) {
        return;
    }
    if (isTouchInside(m_princess, pTouch)) {
        refreshDialog();
    }
}

void PrincessQuestView::refreshDialog()
{
    string loadingTip1 = "";
    if (FunBuildController::getInstance()->talkTipsVec0.size() > 0) {
        int rIdx = CCMathUtils::getRandomInt(0, FunBuildController::getInstance()->talkTipsVec0.size()-1);
        string dialog = CCCommonUtils::getPropById(FunBuildController::getInstance()->talkTipsVec0[rIdx], "content");
        loadingTip1 = _lang(dialog);
    }
    if (loadingTip1==""){
        loadingTip1 = _lang("3140012");
    }
    m_dialog->setString(loadingTip1);
    m_dialogBg->setContentSize(Size(m_dialogBg->getContentSize().width, m_dialog->getContentSize().height * m_dialog->getOriginScaleY() + 10));
}

CCSize PrincessQuestView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(1488, 576);
    }
    
    return CCSize(620, 240);
}

CCSize PrincessQuestView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(1488, 576);
    }
    
    return CCSize(620, 240);
}

CCTableViewCell* PrincessQuestView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    PrincessQuestCell* cell = dynamic_cast<PrincessQuestCell*>( table->dequeueCell());
    PrincessQuestInfo* info = dynamic_cast<PrincessQuestInfo*>(m_data->objectAtIndex(idx));
    if( info == NULL )
        return  NULL ;
    
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info);
        }else{
            cell = PrincessQuestCell::create(info);
        }
    }
    return cell;
}

ssize_t PrincessQuestView::numberOfCellsInTableView(CCTableView *table){
    int count = m_data->count();
    return count;
}

void PrincessQuestView::scrollViewDidScroll(CCScrollView* view){
    float mindy = m_tabView->minContainerOffset().y;
    float dy = m_tabView->getContentOffset().y;
    if(dy<mindy){
        m_tabView->setContentOffset(ccp(0, mindy));
    }
}

//CCNode * PrincessQuestView::getGuideNode(string _key)
//{
//    return NULL;
//}

void PrincessQuestView::sortQuestInfo()
{
    CCArray* arr = CCArray::create();
    map<std::string, PrincessQuestInfo*>::iterator it;
    for(it = PrincessQuestController::getInstance()->currentAchList.begin();it!=PrincessQuestController::getInstance()->currentAchList.end();it++){
        if(it->second->main!=1){
            continue;
        }
        arr->addObject(it->second);
    }
    int num = arr->count()-1;
    for(int j=0;j<num;j++){
        for(int i=0;i<num-j;i++)
        {
            PrincessQuestInfo* quest1 = (PrincessQuestInfo*)arr->objectAtIndex(i);
            PrincessQuestInfo* quest2 = (PrincessQuestInfo*)arr->objectAtIndex(i+1);
            if(/*(quest1->state == Unfinish && quest2->state == Complete) || */quest1->newSortType >= quest2->newSortType)
            {
                arr->swap(i, i+1);
            }
        }
    }
    int leng = arr->count();
//    leng = leng>4?4:leng;
    for(int i=0;i<leng;i++ ){
        m_data->addObject(arr->objectAtIndex(i));
    }
}

PrincessQuestCell *PrincessQuestCell::create(PrincessQuestInfo* info){
    PrincessQuestCell *ret = new PrincessQuestCell(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
bool PrincessQuestCell::init(){
    auto bg = CCBLoadFile("PrincessQuestCell", this, this);
    this->setContentSize(bg->getContentSize());
    this->setData(m_info);
    return true;
}

//CCNode* PrincessQuestCell::getGuideNode()
//{
//    return NULL;
//}
void PrincessQuestCell::setData(PrincessQuestInfo* info){
    m_info = info;
    refresh();
}

void PrincessQuestCell::refresh()
{
    m_picHead->removeAllChildren();
    m_picHead->setScale(1.f);
    m_btnGoto->setVisible(false);
    CCCommonUtils::setButtonTitle(m_btnGoto, _lang("2000214").c_str());
    if (m_info->main == 1 && m_info->subTask != "") {
        vector<string> temp;
        CCCommonUtils::splitString(m_info->subTask, ";", temp);
        if (temp.size() == 1) {
            map<std::string, PrincessQuestInfo*>::iterator it = PrincessQuestController::getInstance()->currentAchList.find(temp[0]);
            if (it != PrincessQuestController::getInstance()->currentAchList.end()) {
                if (it->second->gotype != -1 && m_info->state == PrincessUnfinish) {
                    m_btnGoto->setVisible(true);
                }
            }
        }
    }
    
    m_descTxt->setString(m_info->name);
    m_timeTxt->setString(_lang("2000298"));////////
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
    if (m_info->main == 1 && m_info->subTask != "") {
        vector<string> temp;
        CCCommonUtils::splitString(m_info->subTask, ";", temp);
        if (temp.size() == 1) {
            map<std::string, PrincessQuestInfo*>::iterator it = PrincessQuestController::getInstance()->currentAchList.find(temp[0]);
            if (it != PrincessQuestController::getInstance()->currentAchList.end()) {
                int cur = it->second->curValue>m_info->maxValue?m_info->maxValue:it->second->curValue;
                std::string str = CC_CMDITOA(cur);
                str.append("/").append(CC_CMDITOA(m_info->maxValue));
                float pro = cur*1.0/m_info->maxValue;
                pro = pro>1?1:pro;
                m_progress->setScaleX(pro);
                m_progressTxt->setString(str);
            }
        }
        else if (temp.size() > 1) {
            vector<string> subTemp;
            CCCommonUtils::splitString(m_info->sub, ";", subTemp);
            int cur = subTemp.size()>m_info->maxValue?m_info->maxValue:subTemp.size();
            std::string str = CC_CMDITOA(cur);
            str.append("/").append(CC_CMDITOA(m_info->maxValue));
            float pro = cur*1.0/m_info->maxValue;
            pro = pro>1?1:pro;
            m_progress->setScaleX(pro);
            m_progressTxt->setString(str);
        }
    }
}
void PrincessQuestCell::update(float sec)
{
    if (m_info->state == PrincessComplete) {
        m_statTxt->setColor({255,217,157});
        m_statTxt->setVisible(true);
        m_statTxt->setString(_lang("133109"));
    }else {
        double now = GlobalData::shared()->getTimeStamp();
        double left = m_info->limitedTime - now;
        if (left > 0) {
            m_statTxt->setVisible(false);
            m_time->setVisible(true);
            m_time->setString(CC_DSECTOA(left));
        }else {
            m_time->setVisible(false);
            m_statTxt->setColor({199,27,0});
            m_statTxt->setVisible(true);
            m_statTxt->setString(_lang("2000296"));
        }
    }
}
void PrincessQuestCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void PrincessQuestCell::onExit(){
    setTouchEnabled(false);
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    CCNode::onExit();
}

bool PrincessQuestCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_descTxt", CCLabelIF*, this->m_descTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btnGoto", CCControlButton*, this->m_btnGoto);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_picHead", CCNode*, this->m_picHead);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_progress", CCScale9Sprite*, this->m_progress);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_progressTxt", CCLabelIF*, this->m_progressTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", Node*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_statTxt", CCLabelIF*, m_statTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_time", CCLabelIF*, m_time);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeTxt", CCLabelIF*, m_timeTxt);
    return false;
}

SEL_CCControlHandler PrincessQuestCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGotoBtnClick", PrincessQuestCell::onGotoBtnClick);
    return NULL;
}
void PrincessQuestCell::onGotoBtnClick(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->removeAllPopupView();
    PrincessQuestController::getInstance()->goToQuestTarget(m_info);
}
bool PrincessQuestCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_touchNode, pTouch)) {
        return true;
    }
    return false;
}
void PrincessQuestCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (isTouchInside(m_touchNode, pTouch)) {
        PopupViewController::getInstance()->addPopupInView(PrincessQuestDetailView::create(m_info));
    }
}