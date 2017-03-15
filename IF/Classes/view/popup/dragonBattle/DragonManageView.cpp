//
//  DragonManageView.cpp
//  IF
//
//  Created by ganxiaohua on 15/8/26.
//
//

#include "DragonManageView.h"
#include "PopupViewController.h"
#include "CommandBase.h"
#include "YesNoDialog.h"
#include "DragonBattleAllianceInfoCommand.h"
#include "DragonBattleAlliancePlayCommand.h"
#include "ActivityController.h"

#define  DRAGON_FIGHT_DATA_CHANGE "DRAGON_FIGHT_DATA_CHANGE"

DragonManageView *DragonManageView::create(){
    DragonManageView *ret = new DragonManageView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonManageView::onEnter(){
    this->setTitleName(_lang("140044"));
    PopupBaseView::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonManageView::updateData), DRAGON_FIGHT_DATA_CHANGE, NULL);
}

void DragonManageView::onExit(){
    if (m_waitInterface) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, DRAGON_FIGHT_DATA_CHANGE);
    PopupBaseView::onExit();
}

void DragonManageView::updateData(CCObject* param){
    this->refresh();
}

bool DragonManageView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void DragonManageView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    
}

bool DragonManageView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        CCLoadSprite::doResourceByCommonIndex(506, true);
        CCLoadSprite::doResourceByCommonIndex(7, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(506, false);
            CCLoadSprite::doResourceByCommonIndex(7, false);
        });
        setIsHDPanel(true);
        this->initCommonBg();
        auto node = CCBLoadFile("DragonManageView", this, this);
        this->setContentSize(node->getContentSize());
        int prev = m_viewBg->getContentSize().height;
        this->changeBGHeight(m_viewBg);
        int add = m_viewBg->getContentSize().height - prev;
        if (CCCommonUtils::isIosAndroidPad()) {
            add = CCDirector::sharedDirector()->getWinSize().height - 2048;
        }
        m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width,m_infoList->getContentSize().height + add));
        m_infoList->setPositionY(m_infoList->getPositionY() - add);
        m_viewBg->setVisible(false);
        
        m_txt1->setString(_lang_2("140045","",""));
        m_txt2->setString(_lang_1("140046",CC_ITOA(ActivityController::getInstance()->wa_info_k9)));
        m_nameLabel->setString(_lang("140047").c_str());
        m_powerLabel->setString(_lang("140038").c_str());
        m_stateLabel->setString(_lang("140048").c_str());
        
        CCSpriteFrame* sf = CCLoadSprite::getSF("57009_ad1.png");
        if (sf!=NULL) {
            m_dragonBg->initWithSpriteFrame(sf);
        }
        m_data = CCArray::create();
        
        m_tabView = CCMultiColTableView::create(this, m_infoList->getContentSize());
        m_tabView->setDirection(kCCScrollViewDirectionVertical);
        m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_tabView->setTouchPriority(Touch_Default);
        m_infoList->addChild(m_tabView);
        
        DragonBattleAllianceInfoCommand* cmd = new DragonBattleAllianceInfoCommand();
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(DragonManageView::getData), NULL));
        cmd->sendAndRelease();
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_infoList);
        m_waitInterface->setPosition(ccp(m_infoList->getContentSize().width/2, m_infoList->getContentSize().height/2));
        
        ret = true;
    }
    return ret;
}

void DragonManageView::getData(CCObject *param){
    if (m_waitInterface) {
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dic = _dict(result->getData());
    if(dic){
        CCArray* array = dynamic_cast<CCArray*>(dic->objectForKey("list"));
        if (array) {
            m_data->removeAllObjects();
            m_data->addObjectsFromArray(array);
            m_tabView->reloadData();
        }
        ActivityController::getInstance()->fightNum = dic->valueForKey("fightNum")->intValue();
        ActivityController::getInstance()->maxFightNum = dic->valueForKey("maxFightNum")->intValue();
        this->refresh();
    }
}

void DragonManageView::refresh(){
    m_txt1->setString(_lang_2("140045","",""));
    string temp = CC_ITOA(ActivityController::getInstance()->fightNum);
    temp.append("/");
    temp.append(CC_ITOA(ActivityController::getInstance()->maxFightNum));
    m_txt3->setString(temp);
}

bool DragonManageView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt1", CCLabelIF*, this->m_txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt2", CCLabelIF*, this->m_txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_txt3", CCLabelIF*, this->m_txt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_powerLabel", CCLabelIF*, this->m_powerLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_stateLabel", CCLabelIF*, this->m_stateLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleNode", CCNode*, this->m_titleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_dragonBg", CCSprite*, this->m_dragonBg);
    return false;
}

SEL_CCControlHandler DragonManageView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    
    return NULL;
}

void DragonManageView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize DragonManageView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1536, 103*2.4);
    }
    return CCSize(640, 103);
}

CCTableViewCell* DragonManageView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, ssize_t idx){
    if(idx >= m_data->count()){
        return NULL;
    }
    DragonMemberCell* cell = (DragonMemberCell*)table->dequeueGrid();
    CCDictionary* dic = dynamic_cast<CCDictionary*>(m_data->objectAtIndex(idx));
    if(idx < m_data->count()){
        if(cell){
            cell->setData(dic);
        }else{
            cell = DragonMemberCell::create(dic);
        }
    }
    return cell;
}

ssize_t DragonManageView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    return m_data->count();
}

ssize_t DragonManageView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 1;
}

//
DragonMemberCell *DragonMemberCell::create(CCDictionary* info){
    DragonMemberCell *ret = new DragonMemberCell(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonMemberCell::setData(CCDictionary* info){
    m_info = info;
    if (m_info==NULL) {
        return;
    }
    int rank = m_info->valueForKey("rank")->intValue();
    double power = m_info->valueForKey("power")->doubleValue();
    string name = m_info->valueForKey("name")->getCString();
    string abbr = m_info->valueForKey("abbr")->getCString();
    m_uid = m_info->valueForKey("uid")->getCString();
    isFight = m_info->valueForKey("fight")->intValue();
    string pic = m_info->valueForKey("pic")->getCString();
    auto cf = CCLoadSprite::loadResource(CCString::createWithFormat("Alliance_R%d.png",rank)->getCString());
    m_rankSpr->initWithSpriteFrame(cf);
    string temp = "";
    if (abbr!="") {
        temp.append("(");
        temp.append(abbr);
        temp.append(")");
    }
    temp.append(name);
    m_nameTxt->setString(temp);
    m_tipTxt->setString("");
    if(GlobalData::shared()->playerInfo.uid==m_uid){
        m_nameTxt->setColor(ccc3(255, 203, 0));
        m_powerTxt->setColor(ccc3(255, 203, 0));
    }else{
        m_nameTxt->setColor({ccc3(189,176,131)});
        m_powerTxt->setColor({ccc3(189,176,131)});
    }
    m_powerTxt->setString(CC_CMDITOAD(power));
    if (isFight==0) {
        m_battleBtn->setVisible(true);
        m_cancelBtn->setVisible(false);
        m_signSpr->setVisible(false);
    }else if(isFight==1){
        m_battleBtn->setVisible(false);
        m_cancelBtn->setVisible(true);
        m_signSpr->setVisible(true);
    }else if(isFight==2){
        m_battleBtn->setVisible(false);
        m_cancelBtn->setVisible(false);
        m_signSpr->setVisible(false);
        m_tipTxt->setString(_lang("140080"));
    }else{
        m_battleBtn->setVisible(false);
        m_cancelBtn->setVisible(false);
        m_signSpr->setVisible(false);
        m_tipTxt->setString(_lang("140079"));
    }
    pic.append(".png");
    m_icon->removeAllChildrenWithCleanup(true);
    auto spr = CCLoadSprite::createSprite(pic.c_str());
    m_icon->addChild(spr);
    CCCommonUtils::setSpriteMaxSize(spr, 60);
    if (GlobalData::shared()->playerInfo.allianceInfo.rank<4) {
        m_battleBtn->setVisible(false);
        m_cancelBtn->setVisible(false);
    }
}

void DragonMemberCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
}

void DragonMemberCell::onExit(){
    setTouchEnabled(false);
   
    CCNode::onExit();
}

bool DragonMemberCell::init(){
    auto bg = CCBLoadFile("DragonMemberCell", this, this);
    this->setContentSize(bg->getContentSize());
    m_uid = "";
    isFight = false;
    CCCommonUtils::setButtonTitle(m_cancelBtn, _lang("140050").c_str());
    CCCommonUtils::setButtonTitle(m_battleBtn, _lang("140049").c_str());
    setData(m_info);
    return true;
}

bool DragonMemberCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){

    return true;
}

void DragonMemberCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (ccpDistance(pTouch->getStartLocation(), pTouch->getLocation())>30) {
        return ;
    }
    
}

void DragonMemberCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
}

void DragonMemberCell::onClickBatlle(CCObject *pSender, CCControlEvent event){
    if (m_uid!="") {
        if(ActivityController::getInstance()->fightNum>=50){
            YesNoDialog::show(_lang("140141").c_str(), NULL);
        }else{
            DragonBattleAlliancePlayCommand* cmd = new DragonBattleAlliancePlayCommand(m_uid,1);
            cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(DragonMemberCell::successCallBack), NULL));
            cmd->sendAndRelease();
        }
    }
}

void DragonMemberCell::onCancelBatlle(CCObject *pSender, CCControlEvent event){
    if (m_uid!="") {
        DragonBattleAlliancePlayCommand* cmd = new DragonBattleAlliancePlayCommand(m_uid,0);
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(DragonMemberCell::successCallBack), NULL));
        cmd->sendAndRelease();
    }
}

void DragonMemberCell::successCallBack(CCObject* param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dic = _dict(result->getData());
    if(dic && dic->objectForKey("code")){
        int code = dic->valueForKey("code")->intValue();
        if (code==0) {//0 为成功的
            isFight = !isFight;
            m_battleBtn->setVisible(!isFight);
            m_cancelBtn->setVisible(isFight);
            m_info->setObject(CCString::create(isFight?"1":"0"), "fight");
            m_signSpr->setVisible(isFight);
            if (dic->objectForKey("count")) {
                ActivityController::getInstance()->fightNum = dic->valueForKey("count")->intValue();
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(DRAGON_FIGHT_DATA_CHANGE);
            }
        }
    }
}

bool DragonMemberCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_powerTxt", CCLabelIF*, this->m_powerTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_icon", CCNode*, this->m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rankSpr", CCSprite*, this->m_rankSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_signSpr", CCSprite*, this->m_signSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipTxt", CCLabelIF*, this->m_tipTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_cancelBtn", CCControlButton*, this->m_cancelBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_battleBtn", CCControlButton*, this->m_battleBtn);
    
    return false;
}

SEL_CCControlHandler DragonMemberCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBatlle", DragonMemberCell::onClickBatlle);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCancelBatlle", DragonMemberCell::onCancelBatlle);
    return NULL;
}