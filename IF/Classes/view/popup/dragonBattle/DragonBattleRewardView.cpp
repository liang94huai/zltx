//
//  DragonBattleRewardView.cpp
//  IF
//
//  Created by ganxiaohua on 15/9/6.
//
//

#include "DragonBattleRewardView.h"
#include "PopupViewController.h"
#include "RewardController.h"
#include "UIComponent.h"
#include "DragonBattleRankCommand.h"
#include "DragonRankInfo.h"
#include "ActivityController.h"
#include "DragonBattleRewardInfoCommand.h"
#include "ToolController.h"

#define  DRAGON_BATTLE_REWARD_CLICK "DRAGON_BATTLE_REWARD_CLICK"

DragonBattleRewardView *DragonBattleRewardView::create(){
    DragonBattleRewardView *ret = new DragonBattleRewardView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonBattleRewardView::onEnter(){
    this->setTitleName(_lang("140001"));
    PopupBaseView::onEnter();
    setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonBattleRewardView::updateData), DRAGON_BATTLE_REWARD_CLICK, NULL);
    
}

void DragonBattleRewardView::onExit(){
    setTouchEnabled(false);
    if(m_waitInterface){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, DRAGON_BATTLE_REWARD_CLICK);
    PopupBaseView::onExit();
}

bool DragonBattleRewardView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void DragonBattleRewardView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void DragonBattleRewardView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    
}

bool DragonBattleRewardView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(502, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(502, false);
        });
        auto node = CCBLoadFile("DragonBattleRewardView", this, this);
        this->setContentSize(node->getContentSize());
        
        int preHeight = this->m_viewBg->getContentSize().height;
        changeBGHeight(m_viewBg);
        m_viewBg->setVisible(false);
        int dh = m_viewBg->getContentSize().height - preHeight;
        if (CCCommonUtils::isIosAndroidPad()) {
            dh = CCDirector::sharedDirector()->getWinSize().height - 2048;
        }
        this->m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height + dh));
        m_infoList->setPositionY(m_infoList->getPositionY()-dh);
        
        m_titleTxt->setString(_lang("140041"));
        m_data = CCArray::create();
        
        m_scrollView = CCScrollView::create(m_infoList->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_infoList->addChild(m_scrollView);
        m_scrollView->setVisible(false);
        
        for (int i=0; i<6; i++) {
            DragonRewardTitleCell* record = DragonRewardTitleCell::create(CCDictionary::create(), i,m_infoList);
            m_scrollView->addChild(record);
            m_data->addObject(record);
        }
        //updatePosition(0);
        
        DragonBattleRewardInfoCommand* cmd = new DragonBattleRewardInfoCommand();
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(DragonBattleRewardView::getServerData), NULL));
        cmd->sendAndRelease();
        
        m_waitInterface = GameController::getInstance()->showWaitInterface(m_infoList);
        m_waitInterface->setPosition(m_infoList->getContentSize().width/2, m_infoList->getContentSize().height/2);
        ret = true;
    }
    return ret;
}

void DragonBattleRewardView::getServerData(CCObject* param){
    m_scrollView->setVisible(true);
    if(m_waitInterface){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    updatePosition(0);
}

void DragonBattleRewardView::updateData(CCObject* param){
    CCString* strObj = dynamic_cast<CCString*>(param);
    if (strObj) {
        string str = strObj->getCString();
        int index = -1;
        int type = -1;
        std::vector<std::string> vArr;
        CCCommonUtils::splitString(str,"_",vArr);
        if (vArr.size()==1) {
            type = atoi(vArr[0].c_str());
        }
        if (vArr.size()==2) {
            type = atoi(vArr[0].c_str());
            index = atoi(vArr[1].c_str());
        }
        this->updatePosition(type,index);
    }
}

void DragonBattleRewardView::updatePosition(int type,int index){
    int total = 0;
    int num = m_data->count();
    //m_scrollView->setTouchEnabled(true);
    for (int i=0; i<num; i++) {
        DragonRewardTitleCell* record = dynamic_cast<DragonRewardTitleCell*>(m_data->objectAtIndex(i));
        if(type==i && index != -1){
            bool flag = record->showTowMembers(index);
            //m_scrollView->setTouchEnabled(!flag);
        }else if (type==i && !record->isOpen) {
            record->showTowMembers(index);
        }else{
            record->reset();
        }
        total += record->getCellHeight();
    }
    
    int scrollViewH = total;
    if (CCCommonUtils::isIosAndroidPad()) {
        total -= 80*2.4;
    }else{
        total -= 80;
    }
    for (int i=0; i<num; i++) {
        DragonRewardTitleCell* record = dynamic_cast<DragonRewardTitleCell*>(m_data->objectAtIndex(i));
        record->setPositionY(total);
        total -= record->getCellHeight();
    }
    m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,scrollViewH));
    m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - scrollViewH));
}

void DragonBattleRewardView::onHelpClick(CCObject *pSender, CCControlEvent event){
    CCCommonUtils::showHelpShiftSingleFAQ("45216");
}

bool DragonBattleRewardView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoBtn", CCControlButton*, this->m_infoBtn);
    return false;
}

SEL_CCControlHandler DragonBattleRewardView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHelpClick", DragonBattleRewardView::onHelpClick);
    return NULL;
}

//

DragonRewardTitleCell *DragonRewardTitleCell::create(CCDictionary* info,int type,CCNode* clickArea){
    DragonRewardTitleCell *ret = new DragonRewardTitleCell(info,type,clickArea);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonRewardTitleCell::onEnter(){
    setTouchEnabled(true);
    
    CCNode::onEnter();
}

void DragonRewardTitleCell::onExit(){
    setTouchEnabled(false);

    CCNode::onExit();
}

bool DragonRewardTitleCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(m_bg, pTouch) && isTouchInside(m_clickArea, pTouch)) {
        return true;
    }
    return false;
}

void DragonRewardTitleCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void DragonRewardTitleCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (ccpDistance(pTouch->getStartLocation(), pTouch->getLocation())>30) {
        return ;
    }
    if (isTouchInside(m_bg, pTouch) && isTouchInside(m_clickArea, pTouch)) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(DRAGON_BATTLE_REWARD_CLICK,CCString::create(CC_ITOA(m_type)));
    }
}

bool DragonRewardTitleCell::init(){
    auto node = CCBLoadFile("DragonRewardTitleCell", this, this);
    this->setContentSize(node->getContentSize());
    
    m_srcData = CCArray::create();
    m_topContainer = CCNode::create();
    this->addChild(m_topContainer);
    
    m_data = CCArray::create();
    string titleStr = "";
    switch (m_type) {
        case 0:
            titleStr = _lang("140029");
            break;
        case 1:
            titleStr = _lang_1("140028","");
            break;
        case 2:
            titleStr = _lang_1("140027","");
            break;
        case 3:
            titleStr = _lang_1("140026","");
            break;
        case 4:
            titleStr = _lang_1("140025","");
            break;
        case 5:
            titleStr = _lang_1("140024","");
            break;
    }
    m_titleTxt->setString(titleStr.c_str());
    m_rankIcon->removeAllChildrenWithCleanup(true);
    auto spr = CCLoadSprite::createSprite(CCString::createWithFormat("duanwei_%d.png",m_type+1)->getCString());
    spr->setScale(0.7);
    m_rankIcon->addChild(spr);
    
    std::vector<int> vArr;
    std::map<std::string,DragonRankInfo>::iterator it;
    for (it = ActivityController::getInstance()->dragonCatalogMap.begin(); it!=ActivityController::getInstance()->dragonCatalogMap.end(); it++) {
        int tempType = it->second.rank - 1;
        if (m_type==tempType) {
            vArr.push_back(it->second.level);
        }
    }
    isOpen = false;
    m_totalH = 0;
    if(m_type!=0){
        int num = vArr.size();
        for (int i=0; i<num; i++) {
            DragonRewardSubTitleCell* record = DragonRewardSubTitleCell::create(CCDictionary::create(),vArr[i],m_type,m_clickArea);
            m_data->addObject(record);
        }
    }else{

    }
    return true;
}

void DragonRewardTitleCell::initTopData(){
    m_topContainer->removeAllChildrenWithCleanup(true);
    int num = 0;
    if (m_srcData->count()<=0) {
        string index_limit = "";
        std::string mapKey = CC_ITOA(m_type+1);
        mapKey.append("_");
        mapKey.append(CC_ITOA(0));

        std::map<std::string,DragonRankInfo>::iterator it = ActivityController::getInstance()->dragonCatalogMap.find(mapKey);
        if (it!=ActivityController::getInstance()->dragonCatalogMap.end()) {
            int min_A = it->second.min_A;
            int max_A = it->second.max_A;
            num = ActivityController::getInstance()->getDragonRewards()->count();
            for (int i=0; i<num; i++) {
                auto dic = dynamic_cast<CCDictionary*>(ActivityController::getInstance()->getDragonRewards()->objectAtIndex(i));
                if (dic) {
                    int min = dic->valueForKey("min")->intValue();
                    int max = dic->valueForKey("max")->intValue();
                    if (min_A==min && max_A == max) {
                        CCArray* reward = dynamic_cast<CCArray*>(dic->objectForKey("reward"));
                        if (reward) {
                            m_srcData->addObjectsFromArray(reward);
                        }
                        break;
                    }
                }
            }
        }
    }
    num = m_srcData->count();
    
    if (CCCommonUtils::isIosAndroidPad()) {
        m_totalH = -10*2.4;
    }else{
        m_totalH = -10;
    }
    
    for (int i=0; i<num; i++) {
        auto onerank = dynamic_cast<CCDictionary*>(m_srcData->objectAtIndex(i));
        if (onerank) {
            DragonRewardCell* cell = DragonRewardCell::create(onerank);
            if (CCCommonUtils::isIosAndroidPad()) {
                m_totalH -= 134*2.4;
            }else{
                m_totalH -= 134;
            }
            cell->setPositionY(m_totalH);
            this->m_topContainer->addChild(cell);
            
        }
    }
    m_totalH = -m_totalH;
}

bool DragonRewardTitleCell::updateTowData(int index){
    int total = -60;
    if (CCCommonUtils::isIosAndroidPad()) {
        total = -60*2.4;
    }
    int num = m_data->count();
    bool openFlag = false;
    int tempIndex = index -1;
    for (int i=0; i<num; i++) {
        DragonRewardSubTitleCell* record = dynamic_cast<DragonRewardSubTitleCell*>(m_data->objectAtIndex(i));
        if (tempIndex==i && !record->isOpen) {
            record->showMember();
            openFlag = true;
        }else{
            record->reset();
        }
        record->setPositionY(total);
        total -= record->getCellHeight();
        if (record->getParent()==NULL) {
            this->addChild(record);
        }
    }
    m_totalH = -total;
    return  openFlag;
}

int DragonRewardTitleCell::getCellHeight(){
    int h = 90;
    if (CCCommonUtils::isIosAndroidPad()) {
        h = 90*2.4;
    }
    if (isOpen) {
        h += m_totalH;
    }
    return h;
}

void DragonRewardTitleCell::reset(){
    isOpen = false;
    m_arrow->setRotation(0);
    m_topContainer->removeAllChildrenWithCleanup(true);
    int num = m_data->count();
    for (int i=0; i<num; i++) {
        DragonRewardSubTitleCell* record = dynamic_cast<DragonRewardSubTitleCell*>(m_data->objectAtIndex(i));
        record->reset();
        record->removeFromParent();
    }
}

bool DragonRewardTitleCell::showTowMembers(int index){
    isOpen = true;
    m_arrow->setRotation(90);
    bool subOpenFlag = updateTowData(index);
    if (m_type==0) {
        initTopData();
    }
    return subOpenFlag;
}

void DragonRewardTitleCell::setData(CCDictionary* info,int type){
    
}

bool DragonRewardTitleCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIFTTF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rankIcon", CCNode*, this->m_rankIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bg", CCNode*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_arrow", CCSprite*, this->m_arrow);
    return false;
}

SEL_CCControlHandler DragonRewardTitleCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}
// tow
DragonRewardSubTitleCell *DragonRewardSubTitleCell::create(CCDictionary* info,int index,int type,CCNode* clickArea){
    DragonRewardSubTitleCell *ret = new DragonRewardSubTitleCell(info,index,type,clickArea);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonRewardSubTitleCell::onEnter(){
    setSwallowsTouches(false);
    setTouchEnabled(true);
 
    CCNode::onEnter();
}

void DragonRewardSubTitleCell::onExit(){
    setTouchEnabled(false);
 
    CCNode::onExit();
}

bool DragonRewardSubTitleCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(m_bg, pTouch) && isTouchInside(m_clickArea, pTouch)) {
        return true;
    }
    return false;
}

void DragonRewardSubTitleCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void DragonRewardSubTitleCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (ccpDistance(pTouch->getStartLocation(), pTouch->getLocation())>30) {
        return ;
    }
    if (isTouchInside(m_bg, pTouch) && isTouchInside(m_clickArea, pTouch)) {
        string temp = CC_ITOA(m_type);
        temp.append("_");
        temp.append(CC_ITOA(m_index));
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(DRAGON_BATTLE_REWARD_CLICK,CCString::create(temp.c_str()));
    }
}

bool DragonRewardSubTitleCell::init(){
    auto node = CCBLoadFile("DragonRewardSubTitleCell", this, this);
    this->setContentSize(node->getContentSize());
    
    m_srcData = CCArray::create();
    
    string titleStr = "";
    switch (m_type) {
        case 1:
            titleStr = _lang_1("140028",CC_ITOA(m_index));
            break;
        case 2:
            titleStr = _lang_1("140027",CC_ITOA(m_index));
            break;
        case 3:
            titleStr = _lang_1("140026",CC_ITOA(m_index));
            break;
        case 4:
            titleStr = _lang_1("140025",CC_ITOA(m_index));
            break;
        case 5:
            titleStr = _lang_1("140024",CC_ITOA(m_index));
            break;
    }
    m_titleTxt->setString(titleStr.c_str());
    m_rankIcon->removeAllChildrenWithCleanup(true);
    auto spr = CCLoadSprite::createSprite(CCString::createWithFormat("duanwei_%d.png",m_type+1)->getCString());
    spr->setScale(0.5);
    m_rankIcon->addChild(spr);
    
    auto levelBg = CCLoadSprite::createSprite("title_level_bg.png");
    levelBg->setScale(0.5);
    levelBg->setPositionY(-12);
    m_rankIcon->addChild(levelBg);
    
    auto roman = CCCommonUtils::getRomanSprite(m_index);
    roman->setScale(0.8);
    roman->setPositionY(-12);
    m_rankIcon->addChild(roman);
    
    m_data = CCArray::create();
    isOpen = false;
    m_tableHeight = 1800;
    int w = 640;
    if (CCCommonUtils::isIosAndroidPad()) {
        w = CCDirector::sharedDirector()->getWinSize().width;
        m_tableHeight = CCDirector::sharedDirector()->getWinSize().height*1.5;
    }
    CCNode* tNode = CCNode::create();
    tNode->setContentSize(CCSize(w,m_tableHeight));
    tNode->setAnchorPoint(ccp(0, 0));
    node->addChild(tNode,-1000);
    tNode->setPositionY(-m_tableHeight);
    
    m_tabView = CCMultiColTableView::create(this, tNode->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    tNode->addChild(m_tabView);
    
    m_tabView->setTouchEnabled(false);
    
    return true;
}

int DragonRewardSubTitleCell::getCellHeight(){
    int h = 0;
    if (CCCommonUtils::isIosAndroidPad()) {
        h = 54*2.4;
        if (isOpen) {
            h += m_data->count()*134*2.4;
        }
    }else{
        h = 54;
        if (isOpen) {
            h += m_data->count()*134;
        }
    }
    return h;
}

void DragonRewardSubTitleCell::reset(){
    isOpen = false;
    m_arrow->setRotation(270);
    m_data->removeAllObjects();
    m_tabView->reloadData();
}

void DragonRewardSubTitleCell::showMember(){
    isOpen = true;
    m_arrow->setRotation(0);
    m_data->removeAllObjects();
    if (m_srcData && m_srcData->count()<=0) {
        string index_limit = "";
        std::string mapKey = CC_ITOA(m_type+1);
        mapKey.append("_");
        mapKey.append(CC_ITOA(m_index));
        int num = 0;
        std::map<std::string,DragonRankInfo>::iterator it = ActivityController::getInstance()->dragonCatalogMap.find(mapKey);
        if (it!=ActivityController::getInstance()->dragonCatalogMap.end()) {
            int min_A = it->second.min_A;
            int max_A = it->second.max_A;
            num = ActivityController::getInstance()->getDragonRewards()->count();
            for (int i=0; i<num; i++) {
                auto dic = dynamic_cast<CCDictionary*>(ActivityController::getInstance()->getDragonRewards()->objectAtIndex(i));
                if (dic) {
                    int min = dic->valueForKey("min")->intValue();
                    int max = dic->valueForKey("max")->intValue();
                    if (min_A==min && max_A == max) {
                        CCArray* reward = dynamic_cast<CCArray*>(dic->objectForKey("reward"));
                        if (reward) {
                            m_srcData->addObjectsFromArray(reward);
                        }
                        break;
                    }
                }
            }
        }
    }
    m_data->addObjectsFromArray(m_srcData);
    m_tabView->reloadData();
}

void DragonRewardSubTitleCell::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell){
    
}

cocos2d::CCSize DragonRewardSubTitleCell::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table){
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1536, 134*2.4);
    }
    return CCSize(640, 134);
}

CCTableViewCell* DragonRewardSubTitleCell::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, ssize_t idx){
    if(idx >= m_data->count()){
        return NULL;
    }
    DragonRewardCell* cell = (DragonRewardCell*)table->dequeueGrid();
    CCDictionary* info = (CCDictionary*)m_data->objectAtIndex(idx);
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info);
        }else{
            cell = DragonRewardCell::create(info);
        }
    }
    return cell;
}

ssize_t DragonRewardSubTitleCell::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    int num = m_data->count();
    return num;
}

ssize_t DragonRewardSubTitleCell::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return 1;
}

void DragonRewardSubTitleCell::setData(CCDictionary* info,int index){
    
}

bool DragonRewardSubTitleCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIFTTF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_rankIcon", CCNode*, this->m_rankIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bg", CCNode*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_arrow", CCSprite*, this->m_arrow);
    return false;
}

SEL_CCControlHandler DragonRewardSubTitleCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}

//
DragonRewardCell *DragonRewardCell::create(CCDictionary* info){
    DragonRewardCell *ret = new DragonRewardCell(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void DragonRewardCell::onEnter(){
    CCNode::onEnter();
}

void DragonRewardCell::onExit(){
    CCNode::onExit();
}

bool DragonRewardCell::init(){
    auto node = CCBLoadFile("DragonRewardCell", this, this);
    this->setContentSize(node->getContentSize());
    setData(m_info);
    return true;
}

void DragonRewardCell::setData(CCDictionary* info){
    m_info = info;
    if (m_info==NULL) {
        return ;
    }
    std::string name = "";
    std::string icon = "";
    int type = m_info->valueForKey("type")->intValue();
    int value = m_info->valueForKey("value")->intValue();
    int id = 0;
    if(type==R_GOODS){
        auto valueObj = _dict(m_info->objectForKey("value"));
        id = valueObj->valueForKey("id")->intValue();
        name = RewardController::getInstance()->getNameByType(type,id);
        icon = RewardController::getInstance()->getPicByType(type,id);
        value = valueObj->valueForKey("num")->intValue();
    }else if(type==R_EFFECT){
        name = CCCommonUtils::getNameById(CC_ITOA(value));
        if (name=="") {
            switch (value) {
                case 502600:
                    name = _lang("138074");
                    break;
                case 502601:
                    name = _lang("138075");
                    break;
                case 502602:
                    name = _lang("138076");
                    break;
            }
        }
        icon = CCCommonUtils::getIcon(CC_ITOA(value));
        value = 1;
    }else{
        name = RewardController::getInstance()->getNameByType(type,value);
        icon = RewardController::getInstance()->getPicByType(type,value);
    }
    m_nameTxt->setString(name);
    string numStr = "";
    numStr = "X";
    numStr.append(CC_CMDITOA(value));
    if (type==R_EFFECT) {
        numStr = "";
    }
    m_numTxt->setString(numStr);
    int color = 2;
    if (type==R_GOODS) {
        if(ToolController::getInstance()->m_toolInfos.find(id)!=ToolController::getInstance()->m_toolInfos.end()){
            ToolInfo& info = ToolController::getInstance()->getToolInfoById(id);
            color = info.color;
        }
    }else if(type==R_EFFECT || type==R_CRYSTAL || type==R_WIN_POINT || type==R_GOLD){
        color = 5;
    }
    m_icon->removeAllChildrenWithCleanup(true);
    auto iconBg = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(color).c_str());
    CCCommonUtils::setSpriteMaxSize(iconBg, 84,true);
    m_icon->addChild(iconBg,-100);
    
    CCSprite* spr = CCLoadSprite::createSprite(icon.c_str());
    CCCommonUtils::setSpriteMaxSize(spr, 84);
    m_icon->addChild(spr);
}

bool DragonRewardCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numTxt", CCLabelIF*, this->m_numTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_icon", CCNode*, this->m_icon);
    return false;
}

SEL_CCControlHandler DragonRewardCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}