//
//  AllianceWarResultView.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-26.
//
//

#include "AllianceWarResultView.h"
#include "QuestController.h"
#include "PopupViewController.h"
#include "RewardController.h"
#include "UIComponent.h"
#include "ChangePicCommand.h"
#include "ToolController.h"
#include "UseToolView.h"
#include "WorldController.h"
#include "ParticleFireAni.h"
#include "YuanJunDetailView.h"
#include "CancelAllianceTeamCommand.h"
#include "AllianceFlagPar.h"
#include "AllianceManager.h"
#include "AllianceFightInfo.h"
#include "WorldCommand.h"
#include "AllianceFightBulletinCommand.h"
#include "IFPanelLayer.hpp"
#include "IFNormalSkNode.h"

AllianceWarResultView *AllianceWarResultView::create(){
    AllianceWarResultView *ret = new AllianceWarResultView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AllianceWarResultView::onEnter(){
    this->setTitleName(_lang("115203"));
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void AllianceWarResultView::onExit(){
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool AllianceWarResultView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_touchPos = pTouch->getLocation();
    m_isMove = false;
    if (isTouchInside(m_infoList, pTouch)) {
        return true;
    }
    return false;
}

void AllianceWarResultView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint pos = pTouch->getLocation();
    if (fabsf(m_touchPos.y - pos.y)>30) {
        m_isMove = true;
    }
}

void AllianceWarResultView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (m_isMove) {
        m_isMove = false;
        CCPoint min = m_tabView->minContainerOffset();
        CCPoint curr = m_tabView->getContentOffset();
        if (curr.y > 30 && m_data->count()>10 && m_data->count()<500) {
            this->callMore();
        }
    }
}

bool AllianceWarResultView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(7, true);
        auto node = CCBLoadFile("AllianceWarResultView", this, this);
        this->setContentSize(node->getContentSize());
        
        CCLoadSprite::doResourceByCommonIndex(204, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(204, false);
            CCLoadSprite::doResourceByCommonIndex(206, false);
            CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_MONSTERLAYERLITTLE);
        });
       
        CCCommonUtils::setButtonTitle(m_btn, _lang("115845").c_str());
        if(!AllianceManager::getInstance()->isMeAllianceLeader() || true){
            m_btn->setVisible(false);
        }
        int preHeight = this->m_viewBg->getContentSize().height;
        changeBGHeight(m_viewBg);
        m_viewBg->setVisible(false);
        int dh = m_viewBg->getContentSize().height - preHeight;
        if (CCCommonUtils::isIosAndroidPad()) {
            dh = CCDirector::sharedDirector()->getWinSize().height - 2048;
        }
        this->m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height + dh));
        m_infoList->setPositionY(m_infoList->getPositionY()-dh);
        m_bottomNode->setPositionY(m_bottomNode->getPositionY() - dh);
        m_typeTxt->setString(_lang("115232"));
        m_attTxt->setString(_lang("115233"));
        m_desTxt->setString(_lang("115234"));
        m_timeTxt->setString(_lang("115235"));
    
        m_data = CCArray::create();
        
        m_tabView = CCTableView::create(this, m_infoList->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(Touch_Default);
        m_infoList->addChild(m_tabView);
        m_tabView->setBounceable(false);
        m_first = true;
        this->callMore();
        
        //effect
        if (CCFileUtils::sharedFileUtils()->isFileExist("Imperial/Imperial_30.atlas") &&
            CCFileUtils::sharedFileUtils()->isFileExist("Spine/UIComponent/chengfangchilun.json"))
        {
            auto chilun = new IFSkeletonAnimation("Spine/UIComponent/chengfangchilun.json", "Imperial/Imperial_30.atlas");
            if (chilun) {
                {
                    chilun->setVisibleStop(false);
                    chilun->setPositionX(-CCDirector::sharedDirector()->getWinSize().width/2);
                    chilun->setPositionY(60);
                    m_effectNode->addChild(chilun,3);
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

void AllianceWarResultView::getServerData(CCObject* data){
    this->removeLoadingAni(NULL);
    NetResult* result = dynamic_cast<NetResult*>(data);
    auto dic = _dict(result->getData());
    if(dic){
        CCArray* array = (CCArray*)dic->objectForKey("result");
        if(array){
            int count = array->count();
            for(int i=0;i<count;i++){
                auto item = dynamic_cast<CCString*>(array->objectAtIndex(i));
                if(item){
                    AllianceFightInfo* info = new AllianceFightInfo();
                    info->parseInfo(item->getCString());
                    m_data->addObject(info);
                    info->release();
                }
            }
        }
        CCPoint curr = m_tabView->getContentOffset();
        CCPoint max = m_tabView->maxContainerOffset();
        CCPoint min = m_tabView->minContainerOffset();
        m_tabView->reloadData();
        curr.y += m_tabView->minContainerOffset().y - min.y;
        m_tabView->setContentOffset(curr);
    }
    if(m_data->count()>0){
        m_tip->setString("");
    }else{
        m_tip->setString(_lang("115230"));
        
    }
}

void AllianceWarResultView::callMore(){
    int start = m_data->count();
    int end = start+20;
    AllianceFightBulletinCommand* cmd = new AllianceFightBulletinCommand(start,end);
    cmd->setCallback(CCCallFuncO::create(this,callfuncO_selector(AllianceWarResultView::getServerData), NULL));
    cmd->setFailCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceWarResultView::removeLoadingAni), NULL));
    cmd->sendAndRelease();
    addLoadingAni();
}

void AllianceWarResultView::addLoadingAni(){
    if (m_loadingIcon==nullptr) {
        m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
        m_loadingIcon->setAnchorPoint(ccp(0.5, 0.5));
        auto cSize = m_infoList->getContentSize();
        m_loadingIcon->setPosition(ccp(cSize.width/2,cSize.height/2));
    }
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
    if(m_loadingIcon->getParent()==NULL){
        m_infoList->addChild(m_loadingIcon,1000000);
    }
}

void AllianceWarResultView::removeLoadingAni(CCObject* obj){
    if(m_loadingIcon!=nullptr){
        m_loadingIcon->stopAllActions();
        m_loadingIcon->removeFromParent();
    }
}

bool AllianceWarResultView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_typeTxt", CCLabelIF*, this->m_typeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_attTxt", CCLabelIF*, this->m_attTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_desTxt", CCLabelIF*, this->m_desTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tip", CCLabelIF*, this->m_tip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_fireNode1", CCNode*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_fireNode2", CCNode*, this->m_fireNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_btn", ControlButton*, this->m_btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bottomNode", CCNode*, this->m_bottomNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_effectNode", CCNode*, this->m_effectNode);
    return false;
}

SEL_CCControlHandler AllianceWarResultView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtnClick", AllianceWarResultView::onBtnClick);
    return NULL;
}
void AllianceWarResultView::onBtnClick(CCObject* pObj,CCControlEvent pEvent){
    PopupViewController::getInstance()->addPopupInView(IFPanelLayer::create(TYPE_WHITE));
}

CCSize AllianceWarResultView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(1460, 217);
    }
    return CCSize(640, 71);
}

CCSize AllianceWarResultView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1460, 217);
    }
    return CCSize(640, 71);
}

CCTableViewCell* AllianceWarResultView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    AllianceWarResultCell* cell = (AllianceWarResultCell*)table->dequeueCell();
    AllianceFightInfo* info = (AllianceFightInfo*)m_data->objectAtIndex(idx);
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info,idx);
        }else{
            cell = AllianceWarResultCell::create(info,idx);
        }
    }
    return cell;
}

ssize_t AllianceWarResultView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data->count();
}

void AllianceWarResultView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

//

AllianceWarResultCell *AllianceWarResultCell::create(AllianceFightInfo* info,int index){
    AllianceWarResultCell *ret = new AllianceWarResultCell(info,index);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AllianceWarResultCell::onEnter(){
    CCNode::onEnter();
}

void AllianceWarResultCell::onExit(){
    CCNode::onExit();
}

bool AllianceWarResultCell::init(){
    auto node = CCBLoadFile("AllianceWarResultCell", this, this);
    this->setContentSize(node->getContentSize());
    setData(m_info,m_index);
    return true;
}

void AllianceWarResultCell::setData(AllianceFightInfo* info,int index){
    m_info = info;
    m_index = index;
    std::string uid = GlobalData::shared()->playerInfo.allianceInfo.uid;
    std::string attName = "";
    if(m_info->getAttAsn()!=""){
        attName.append("(");
        attName.append(m_info->getAttAsn());
        attName.append(")");
    }
    attName.append(m_info->getAttackName());
    if(m_info->getType()==8){
        attName = CCCommonUtils::getNameById(m_info->getMonsterId());
    }
    std::string defName = "";
    if(m_info->getDefAsn()!=""){
        defName.append("(");
        defName.append(m_info->getDefAsn());
        defName.append(")");
    }
    defName.append(m_info->getDefName());
string str1 = "";
    string str2 = "";
    if(m_info->getSelfIsAtt()){
        if(m_info->getWin()==0){
            m_lostNode->setVisible(false);
            m_winNode->setVisible(true);
            str1 = _lang("115236");
            str2 = _lang("115237");
            m_resultTxt1->setString(_lang("115236"));
            m_resultTxt2->setString(_lang("115237"));
        }else{
            m_lostNode->setVisible(true);
            m_winNode->setVisible(false);
            
            str1 = _lang("115237");
            str2 = _lang("115236");
            m_resultTxt1->setString(_lang("115237"));
            m_resultTxt2->setString(_lang("115236"));
        }
    }else{
        if(m_info->getWin()==1){
            m_lostNode->setVisible(false);
            m_winNode->setVisible(true);
           
            str1 = _lang("115237");
            str2 = _lang("115236");
            m_resultTxt1->setString(_lang("115237"));
            m_resultTxt2->setString(_lang("115236"));
        }else{
            m_lostNode->setVisible(true);
            m_winNode->setVisible(false);
           
            str1 = _lang("115236");
            str2 = _lang("115237");
            m_resultTxt1->setString(_lang("115236"));
            m_resultTxt2->setString(_lang("115237"));
        }
    }
    if(m_info->getcount() != 0){
        str1 = str1.append("X").append(CC_ITOA(m_info->getcount()));
        str2 = str2.append("X").append(CC_ITOA(m_info->getcount()));
    }
    m_resultTxt1->setString(str1.c_str());
    m_resultTxt2->setString(str2.c_str());
    if(attName.length()>17){
        attName = CCCommonUtils::subStrByUtf8(attName,0,16);
        attName.append("...");
    }
    m_attTxt->setString(attName);
    if(defName.length()>17){
        defName = CCCommonUtils::subStrByUtf8(defName,0,16);
        defName.append("...");
    }
    m_defTxt->setString(defName);
    
    m_iconNode->removeAllChildrenWithCleanup(true);
    CCSprite* pic = NULL;
    pic = CCLoadSprite::createSprite("tile_pop_icon4.png");
    if(m_info->getType()==8){
       // pic = CCLoadSprite::createSprite("tile_pop_icon21.png");
    }else if (m_info->getType()==6) {
        CCLoadSprite::doResourceByCommonIndex(206, true);
        //pic = CCLoadSprite::createSprite("scws.png",true,CCLoadSpriteType_MONSTERLAYERLITTLE);
        
        string bossName = CCCommonUtils::getNameById(m_info->getMonsterId());
        if(bossName.length()>17){
            bossName = CCCommonUtils::subStrByUtf8(bossName,0,16);
            bossName.append("...");
        }
        m_defTxt->setString(bossName);
        
    }else if (m_info->getType()!=4) {
        //pic = CCLoadSprite::createSprite("tile_pop_icon4.png");
    }else{
        //pic = CCLoadSprite::createSprite("tile_pop_icon7.png");
        pic->setPositionY(4);
    }
    CCCommonUtils::setSpriteMaxSize(pic, 75 ,true);
    m_iconNode->addChild(pic);
    auto dt = GlobalData::shared()->getWorldTime() - GlobalData::shared()->changeTime(m_info->getTime()/1000);
    string timestr = "";
    int timedt = 0;
    if(dt>=24*60*60){
        timedt =dt/(24*60*60);
        timestr = CC_ITOA(timedt);
        timestr.append(_lang("105592"));
    }else if(dt>=60*60){
        timedt =dt/(60*60);
        timestr = CC_ITOA(timedt);
        timestr.append(_lang("105591"));
    }else if(dt>=60){
        timedt =dt/60;
        timestr = CC_ITOA(timedt);
        timestr.append(_lang("105590"));
    }else{
        timestr ="1";
        timestr.append(_lang("105590"));
    }
    timestr.append(" ");
    timestr.append(_lang("105593"));
    this->m_timeTxt->setString(timestr);
}

bool AllianceWarResultCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_resultTxt1", CCLabelIF*, this->m_resultTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_resultTxt2", CCLabelIF*, this->m_resultTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_attTxt", CCLabelIF*, this->m_attTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_defTxt", CCLabelIF*, this->m_defTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_lostNode", CCNode*, this->m_lostNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_winNode", CCNode*, this->m_winNode);
    return false;
}

SEL_CCControlHandler AllianceWarResultCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}
