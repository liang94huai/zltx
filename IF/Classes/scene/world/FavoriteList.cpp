//
//  FavoriteList.cpp
//  IF
//
//  Created by 邹 程 on 14-2-26.
//
//

#include "FavoriteList.h"
#include "WorldMapView.h"
#include "AddFavorite.h"
#include "YesNoDialog.h"
#include "UIComponent.h"

#ifndef MSG_WORLD_FAVO_RESET_POS
#define MSG_WORLD_FAVO_RESET_POS "world_favo_reset_position"
#endif
#ifndef CLOSE_THIS_FAVOR
#define CLOSE_THIS_FAVOR "close_this_favor"
#endif
#ifndef UPDATE_DEL_NUM
#define UPDATE_DEL_NUM "update_del_num"
#endif


vector<int> comOpenVect;   //用于记录cell的展开状态
int g_openid = -1;         //用于记录cell状态
vector<string> vectForDel; //index;serverId,记录删除信息

FavoriteList* FavoriteList::create(){
    FavoriteList* ret = new FavoriteList();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool FavoriteList::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    
    CCLoadSprite::doResourceByCommonIndex(6, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByCommonIndex(101, true);
    CCLoadSprite::doResourceByCommonIndex(504, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(6, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
        CCLoadSprite::doResourceByCommonIndex(101, false);
        CCLoadSprite::doResourceByCommonIndex(504, false);
    });
    
    auto tmpCCB = CCBLoadFile("FavorTypeView",this,this);
    this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    int extH = getExtendHeight();
    int subExtH =extH;
    //
    m_topNode->setPositionY(m_topNode->getPositionY() + subExtH);
    //
    m_buildBG->setContentSize(m_buildBG->getContentSize()+CCSizeMake(0,subExtH));
    //
    m_listNode->setContentSize(m_listNode->getContentSize()+CCSizeMake(0,subExtH));
    //
    m_titleTxt->setString(_lang("108521"));
    
    m_tabView = CCTableView::create(this, m_listNode->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_listNode->addChild(m_tabView);
    
    m_data = CCArray::create();
    m_flag = CCArray::create();
    m_friend = CCArray::create();
    m_enemy = CCArray::create();
    //初始化隐藏底部Node
    m_allSelect = false;
    m_checkall = false;
    vectForDel.clear();
    m_bottomNode->setVisible(false);
    sortFavoriteList();
    generateData();   //现在所有的数据已经放到m_data的数据结构中
    updateNum();
    initOpenNum();//保持原有打开模式
    updateInfo(NULL);
    updateDelNum(nullptr);
    m_checkalllabel->setString("Check All");
    return true;
}

void FavoriteList::updateInfo(CCObject* obj)
{
    //BigCell的罗列，记录开关状态、cell编号、包含条目
    m_tabView->reloadData();
}

void FavoriteList::initOpenNum(){
    comOpenVect.clear();
    int opennum = CCUserDefault::sharedUserDefault()->getIntegerForKey("Favor_open_num", -1);
    
    vector<int> vect(3,0);
    comOpenVect = vect;
    
    if(opennum <= 2 && opennum >= 0){
       comOpenVect[opennum] = 1;
    }
    
}

void FavoriteList::updateNum(){
    int num = m_data->count();
    int flagNum = m_flag->count();
    int friendNum = m_friend->count();
    int enemyNum = m_enemy->count();
    comOpenVect.clear();
    m_curList.clear();
    m_styleNum.clear();
    
    m_curList.push_back(0);
    comOpenVect.push_back(0);
    m_styleNum.push_back(flagNum);
    
    m_curList.push_back(1);
    comOpenVect.push_back(0);
    m_styleNum.push_back(friendNum);
    
    m_curList.push_back(2);
    comOpenVect.push_back(0);
    m_styleNum.push_back(enemyNum);
}

void FavoriteList::updateList(cocos2d::CCObject *obj) {
    sortFavoriteList();
    generateData();
    updateNum();
    m_tabView->reloadData();
}

void FavoriteList::closethis(CCObject* obj)
{
    this->closeSelf();
}

void FavoriteList::generateData(){
    m_data->removeAllObjects();
    m_flag->removeAllObjects();
    m_friend->removeAllObjects();
    m_enemy->removeAllObjects();
    auto tmpArrNew = CCArray::create();
    auto tmpArrCurrentServer = CCArray::create();
    auto tmpArrOther = CCArray::create();
    for (auto i = 0; i < m_FavoVec.size(); i++) {
        if(m_FavoVec[i].second.isNew){
            tmpArrNew->addObject(CCDouble::create(m_FavoVec[i].first));
        }else if(m_FavoVec[i].second.serverId == GlobalData::shared()->playerInfo.currentServerId){
            tmpArrCurrentServer->addObject(CCDouble::create(m_FavoVec[i].first));
        }else{
            tmpArrOther->addObject(CCDouble::create(m_FavoVec[i].first));
        }
        ////类型的分类放置
        if(m_FavoVec[i].second.type == 0){
            m_flag->addObject(CCDouble::create(m_FavoVec[i].first));
        }else if(m_FavoVec[i].second.type == 1){
            m_friend->addObject(CCDouble::create(m_FavoVec[i].first));
        }else{
            m_enemy->addObject(CCDouble::create(m_FavoVec[i].first));
        }
    }
    m_data->addObjectsFromArray(tmpArrOther);
    m_data->addObjectsFromArray(tmpArrCurrentServer);
    m_data->addObjectsFromArray(tmpArrNew);
    m_data->reverseObjects();
    tmpArrNew->removeAllObjects();
    tmpArrCurrentServer->removeAllObjects();
    tmpArrOther->removeAllObjects();
}

void FavoriteList::sortFavoriteList()
{
    m_FavoVec.clear();
    for(auto it = WorldController::getInstance()->m_favoriteTiles.begin(); it != WorldController::getInstance()->m_favoriteTiles.end(); it++)
    {
        if(!WorldController::getInstance()->isInCrossMap() && it->second.serverType==SERVER_BATTLE_FIELD){
            continue;
        }
        m_FavoVec.push_back(make_pair(it->first, it->second));
    }
    std::sort(m_FavoVec.begin(), m_FavoVec.end(), cmp);
}

bool FavoriteList::cmp(const std::pair<double,FavoriteTile>& item1, const std::pair<double,FavoriteTile>& item2)
{
    return item1.second.createTime < item2.second.createTime;
}

void FavoriteList::updateOpenPanel(CCObject *obj)
{
//    if(obj==NULL || (dynamic_cast<CCInteger*>(obj)==NULL)){
//        m_openItemId = -1;
//    }else {
//        CCInteger* intObj = dynamic_cast<CCInteger*>(obj);
//        m_openItemId = intObj->getValue();
//    }
}

void FavoriteList::onEnter()
{
    UIComponent::getInstance()->showPopupView(1);
    CCNode::onEnter();
    setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FavoriteList::updateInfo), MSG_WORLD_FAVO_RESET_POS, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FavoriteList::closethis), CLOSE_THIS_FAVOR, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FavoriteList::updateList), MSG_WORLD_FAVO_ADD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FavoriteList::updateDelNum), UPDATE_DEL_NUM, NULL);
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FavoriteList::updateOpenPanel), MSG_SCIENCE_LACKOF_RESOURCE, NULL);
}

void FavoriteList::onExit()
{
    CCUserDefault::sharedUserDefault()->setIntegerForKey("Favor_open_num", g_openid);
    CCUserDefault::sharedUserDefault()->flush();
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_WORLD_FAVO_RESET_POS);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, CLOSE_THIS_FAVOR);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_WORLD_FAVO_ADD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, UPDATE_DEL_NUM);
//    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SCIENCE_LACKOF_RESOURCE);
    setTouchEnabled(false);
    CCNode::onExit();
}


CCSize FavoriteList::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_curList.size()){
        return CCSizeZero;
    }
    if (comOpenVect[idx]!=0) {
        int num = m_styleNum[idx];
        if(num == 0){
            return CCSize(600, 60);;
        }
        return CCSize(600, 60+num*168);
    }
    else {
        return CCSize(600, 60);
    }
}

CCSize FavoriteList::cellSizeForTable(CCTableView *table)
{
    return CCSize(600, 60);
}

CCTableViewCell* FavoriteList::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_curList.size()){
        return NULL;
    }
    
    FavoriteBigCell* cell = dynamic_cast<FavoriteBigCell*>(table->dequeueCell());
    if(idx < m_curList.size()){
        if(cell){
            cell->setData(m_curList[idx],comOpenVect[idx], m_styleNum[idx], m_data, m_allSelect,m_checkall);
        }else{
            cell = FavoriteBigCell::create(m_curList[idx], comOpenVect[idx], m_styleNum[idx], m_data, m_allSelect,m_checkall);
        }
    }
    return cell;
}

ssize_t FavoriteList::numberOfCellsInTableView(CCTableView *table)
{
    return m_curList.size();
}

void FavoriteList::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
}

void FavoriteList::onCheckOK(CCObject * pSender, Control::EventType pCCControlEvent){
    m_checkbtn->setVisible(true);
    m_checkokbtn->setVisible(false);
    m_bottomNode->setVisible(false);
    m_select->setVisible(false);
    m_unselect->setVisible(false);
    m_allSelect = false;
    vectForDel.clear();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(UPDATE_DEL_NUM);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_WORLD_FAVO_RESET_POS);
}

void FavoriteList::onCheckBtnOccor(CCObject * pSender, Control::EventType pCCControlEvent)
{
    m_allSelect = true;
    m_checkall = false;
    vectForDel.clear();
    m_checkbtn->setVisible(false);
    m_checkokbtn->setVisible(true);
    m_select->setVisible(false);
    m_unselect->setVisible(true);
    m_bottomNode->setVisible(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(UPDATE_DEL_NUM);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_WORLD_FAVO_RESET_POS);
}

void FavoriteList::delCheckDelInfo(){
    int i = 0;
	int num  = m_data->count();
    while (i < num) {
        CCDouble* info = dynamic_cast<CCDouble*>(m_data->objectAtIndex(i));
        if (info!=NULL) {
            double index = info->getValue();
            auto it = WorldController::getInstance()->m_favoriteTiles.find(index);
            if(it != WorldController::getInstance()->m_favoriteTiles.end()){
                if ( g_openid == (FavoriteType)it->second.type) {
                    string result = "";
                    int index = WorldController::getIndexByPoint(it->second.point);
                    string strIndex = CC_ITOA(index);
                    int serverId = it->second.serverId;
                    string strServerId = CC_ITOA(serverId);
                    string str = strIndex+ ";" +strServerId;
                    auto iter = find(vectForDel.begin(), vectForDel.end(), str);
                    if(iter != vectForDel.end()){
                        vectForDel.erase(iter);
                    }
                }
            }
        }
        i++;
    }
}

void FavoriteList::addCheckDelInfo(){
    int i = 0;
    int num  = m_data->count();
    while (i < num) {
        CCDouble* info = dynamic_cast<CCDouble*>(m_data->objectAtIndex(i));
        if (info!=NULL) {
            double index = info->getValue();
            auto it = WorldController::getInstance()->m_favoriteTiles.find(index);
            if(it != WorldController::getInstance()->m_favoriteTiles.end()){
                if ( g_openid == (FavoriteType)it->second.type) {
                    string result = "";
                    int index = WorldController::getIndexByPoint(it->second.point);
                    string strIndex = CC_ITOA(index);
                    int serverId = it->second.serverId;
                    string strServerId = CC_ITOA(serverId);
                    string str = strIndex+ ";" +strServerId;
                    auto iter = find(vectForDel.begin(), vectForDel.end(), str);
                    if(iter == vectForDel.end()){
                        if(str != ""){
                            vectForDel.push_back(str);
                        }
                    }
                }
            }
        }
        i++;
    }
}

void FavoriteList::onSelectClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    m_select->setVisible(false);
    m_unselect->setVisible(true);
    delCheckDelInfo();
    m_checkall = false;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(UPDATE_DEL_NUM);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_WORLD_FAVO_RESET_POS);
}

void FavoriteList::onUnSelectClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    m_select->setVisible(true);
    m_unselect->setVisible(false);
    addCheckDelInfo();
    m_checkall = true;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_WORLD_FAVO_RESET_POS);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(UPDATE_DEL_NUM);
}

void FavoriteList::onTouchSwallow(CCObject * pSender, Control::EventType pCCControlEvent){
    //吞噬触摸操作
}

void FavoriteList::delOprate(){
    if(vectForDel.size() == 0){
        CCCommonUtils::flyHint("", "", _lang("108906"),3,0,true);
        return;
    }
    
    if(vectForDel.size()>0){
        YesNoDialog::show(_lang("108526").c_str(), CCCallFunc::create(this, callfunc_selector(FavoriteList::cmdWithData)));
    }
}

void FavoriteList::onDelOpration(CCObject * pSender, Control::EventType pCCControlEvent)
{
    delOprate();
}

void FavoriteList::cmdWithData(){
    string strcmd = "";
    Vector<string> ::iterator iter = vectForDel.begin();
    for (; iter != vectForDel.end(); iter++) {
        strcmd+= *iter;
        if (iter+1 != vectForDel.end()) {
            strcmd+="|";
        }
    }
    CCLog(strcmd.c_str());
    CCArray* p = CCArray::create();
    CCArray* d = CCArray::create();
    Vector<string> ::iterator iterde = vectForDel.begin();
    for (; iterde != vectForDel.end(); iterde++) {
        std::vector<string> perItem;
        CCCommonUtils::splitString(*iterde, ";", perItem);
        int m_point = 0;
        int m_server = 0;
        if (perItem.size()==2) {
            m_point = std::atoi(perItem[0].c_str());
            m_server = std::atoi(perItem[1].c_str());
        }
        p->addObject(CCInteger::create(m_point));
        int h = p->count();
        d->addObject(CCInteger::create(m_server));
    }
    int a = p->count();
    int b = d->count();
    auto cmd = new WorldDelFavormultiCommand(strcmd,p,d);
    cmd->setCallback(CCCallFuncO::create(this,callfuncO_selector(FavoriteList::onDeleteCallback), NULL));
    cmd->sendAndRelease();

}

void FavoriteList::onDeleteCallback(CCObject* data){
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_WORLD_FAVO_ADD);
    vectForDel.clear();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(UPDATE_DEL_NUM);
}

void FavoriteList::updateDelNum(cocos2d::CCObject *obj){
    int num = vectForDel.size();
    std::string str = "";
    if (num == 0) {
        str = "";
    }else{
        str.append("(");
        str.append(CC_ITOA(num));
        str.append(")");
    }
    m_delNum->setString(str);
}

bool FavoriteList::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (isTouchInside(m_touchNode, pTouch)) {
        return true;
    }
    if(isTouchInside(m_sprNode, pTouch)){
        return true;
    }
    return false;
}

void FavoriteList::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(isTouchInside(m_sprNode, pTouch)){
        delOprate();
        return;
    }
}

SEL_CCControlHandler FavoriteList::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCheckBtnOccor", FavoriteList::onCheckBtnOccor);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCheckOK", FavoriteList::onCheckOK);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSelectClick", FavoriteList::onSelectClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onUnSelectClick", FavoriteList::onUnSelectClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDelOpration", FavoriteList::onDelOpration);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTouchSwallow", FavoriteList::onTouchSwallow);
    
    return NULL;
}

bool FavoriteList::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprNode", CCNode*, this->m_sprNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_topNode", CCNode*, this->m_topNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_checkbtn", CCControlButton*, this->m_checkbtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_checkokbtn", CCControlButton*, this->m_checkokbtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rubbish", CCControlButton*, this->m_rubbish);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_delNum", CCLabelIF*, this->m_delNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_checkalllabel", CCLabelIF*, this->m_checkalllabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_select", CCNode*, this->m_select);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_unselect", CCNode*, this->m_unselect);
    return false;
}

///////FavoriteBigCell
//itemId 是当前cell的编号 || type是打开的状态 || num是当前cell包含的条目
FavoriteBigCell* FavoriteBigCell::create(int itemId, int type, int num,  CCArray* alldata, bool openchecked, bool checkall)
{
    auto ret = new FavoriteBigCell();
    if (ret && ret->init(itemId, type, num,alldata,openchecked,checkall)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool FavoriteBigCell::init(int itemId, int type, int num, CCArray* alldata,bool openchecked,bool checkall)
{
    bool ret = true;
    CCBLoadFile("FavorTypeCell",this,this);
    setData(itemId, type, num, alldata,openchecked,checkall);
    return ret;
}

void FavoriteBigCell::setData(int itemId, int type, int num,CCArray* alldata,bool openchecked,bool checkall)
{
    m_itemId = itemId;
    m_type = type;
    m_num = num;
    m_checkall = checkall;
    m_alldata = alldata;
    if (num < 0 || m_alldata == NULL || alldata->count() < 0) {
        return;
    }
    //名称标注
    if( m_titleLabel )
    {
        switch (itemId) {
            case 0:
                m_titleLabel->setString(_lang("108505").c_str());
                break;
            case 1:
                m_titleLabel->setString(_lang("108506").c_str());
                break;
            case 2:
                m_titleLabel->setString(_lang("108520").c_str());
                break;
            default:
                break;
        }
    }
    //深色背景的缩放
    if(num != 0 && type){
        int tempint = num;
        if(itemId == 2){
            if (tempint < 8) {
                tempint = 10; //放大背景
            }
        }
        m_left9sprite->setContentSize(CCSizeMake(320,55+tempint*168));
        m_right9sprite->setContentSize(CCSizeMake(320,55+tempint*168));
        m_left9sprite->setVisible(true);
        m_right9sprite->setVisible(true);
    }else{
        m_right9sprite->setVisible(false);
        m_left9sprite->setVisible(false);
    }
    m_floorLabel->setString(CC_ITOA(num));
    m_listNode->removeAllChildren();
    int curY = 0;
    if (m_type==0) {
        m_jiantouSpr->setRotation(0);
    }
    else {
        m_jiantouSpr->setRotation(90);
        int i = 0;
        int num = alldata->count();
        while (i < num) {
            CCDouble* info = dynamic_cast<CCDouble*>(alldata->objectAtIndex(i));
            if (info!=NULL) {
                double index = dynamic_cast<CCDouble*>(alldata->objectAtIndex(i))->getValue();
                auto it = WorldController::getInstance()->m_favoriteTiles.find(index);
                if(it != WorldController::getInstance()->m_favoriteTiles.end()){
                    if ( m_itemId == (FavoriteType)it->second.type) {
                        auto cell = FavoriteCell::create(m_itemId, index,openchecked,checkall);
                        cell->setAnchorPoint(ccp(0, 1));
                        cell->setPosition(ccp(0, curY));
                        m_listNode->addChild(cell);
                        curY -= 168;
                    }
                }
            }
            i++;
        }
    }
    m_mainNode->setPositionY(45+abs(curY));
}

void FavoriteBigCell::onEnter() {
    setTouchEnabled(true);
    CCNode::onEnter();
}

void FavoriteBigCell::onExit() {
    setTouchEnabled(false);
    CCNode::onExit();
}

bool FavoriteBigCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_myTouchNode", CCNode*, m_myTouchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_left9sprite", CCScale9Sprite*, m_left9sprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_right9sprite", CCScale9Sprite*, m_right9sprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_jiantouSpr", CCSprite*, m_jiantouSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_floorLabel", CCLabelIF*, m_floorLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIFTTF*, m_titleLabel);
    return false;
}

SEL_CCControlHandler FavoriteBigCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    return NULL;
}

void FavoriteBigCell::addAllItemData(){
    int i = 0;
    if (m_alldata==NULL) {
        return;
    }
    int num = m_alldata->count();
    while (i < num) {
        CCDouble* info = dynamic_cast<CCDouble*>(m_alldata->objectAtIndex(i));
        if (info!=NULL) {
            double index = info->getValue();
            auto it = WorldController::getInstance()->m_favoriteTiles.find(index);
            if(it != WorldController::getInstance()->m_favoriteTiles.end()){
                if ( m_itemId == (FavoriteType)it->second.type) {
                    string result = "";
                    int index = WorldController::getIndexByPoint(it->second.point);
                    string strIndex = CC_ITOA(index);
                    int serverId = it->second.serverId;
                    string strServerId = CC_ITOA(serverId);
                    string str = strIndex+ ";" +strServerId;
//                    int count = vectForDel.size();
//                    count = MIN(count,200);
//                    for (int k=0; k<=count; k++) {
//                        if (vectForDel[k]!=str) {
//                            vectForDel.push_back(str);
//                        }
//                    }
                    if(vectForDel.size() == 0){
                       vectForDel.push_back(str);
                    }else{
                        auto iter = find(vectForDel.begin(), vectForDel.end(), str);
                        if(iter == vectForDel.end()){
                            vectForDel.push_back(str);
                        }
                    }
                }
            }
        }
        i++;
    }
    
}

bool FavoriteBigCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_myTouchNode, pTouch)) {
        m_startY = pTouch->getLocation().y;
        return true;
    }
    return false;
}

void FavoriteBigCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    int mv = pTouch->getLocation().y - m_startY;
    if (abs(mv)>20) {
        return;
    }
    vectForDel.clear();
    if (m_type==0) { //使得cell处于展开状态
        m_type = 1;
        vector<int> vect(3,0);
        comOpenVect = vect;
        comOpenVect[m_itemId] = m_type;
        g_openid = m_itemId;
        onClickOpen(90);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_WORLD_FAVO_RESET_POS);
        if(m_checkall){
            addAllItemData();//对展开的已经check的条目信息进行记录
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(UPDATE_DEL_NUM);
    }
    else {           //使得cell处于闭合状态
        m_type = 0;
        g_openid = -1;
        comOpenVect[m_itemId] = m_type;
        onClickOpen(0);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_WORLD_FAVO_RESET_POS);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(UPDATE_DEL_NUM);
    }

}

void FavoriteBigCell::onClickOpen(float rat)
{
    CCActionInterval * rotateto = CCRotateTo::create(0.2, rat);
    m_jiantouSpr->runAction(rotateto);
}

void FavoriteBigCell::onResetCanTouch()
{
}

/////////////二级小cell
FavoriteCell* FavoriteCell::create(int r_type, double index,bool openchecked,bool checkall)
{
    auto ret = new FavoriteCell();
    if (ret && ret->init(r_type, index, openchecked,checkall)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool FavoriteCell::init(int r_type, double index,bool openchecked,bool checkall)
{
    bool ret = true;
    CCBLoadFile("FavorCell",this,this);
    m_type = r_type;
    m_index = index;
    m_openchecked = openchecked;
    m_checkall = checkall;
    WorldCityType cityType = OriginTile;;
    auto it = WorldController::getInstance()->m_favoriteTiles.find(m_index);
    if(it != WorldController::getInstance()->m_favoriteTiles.end()){
        CCPoint pt;
        pt.x = 0;
        pt.y = 0;
        cityType = (WorldCityType)getPictype(pt);
        this->addPic(cityType);
        string str = "";
        str.append(" X:");
        str.append(CC_ITOA(it->second.point.x));
        str.append(" Y:");
        str.append(CC_ITOA(it->second.point.y));
        if (GlobalData::shared()->playerInfo.selfServerId != it->second.serverId){
            str.append(" K:");
            str.append(CC_ITOA(it->second.serverId));
        }
        m_coorLabel->setString(str);
        m_nameLabel->setString(it->second.name.c_str());
    }
    if (m_openchecked) {
        if (m_checkall) {
            m_unselect->setVisible(false);
            m_select->setVisible(true);
        }else{
            m_unselect->setVisible(true);
            m_select->setVisible(false);
        }
    }else{
        m_unselect->setVisible(false);
        m_MoveNode->setPositionX(m_MoveNode->getPositionX() - 100);
    }
    
//    m_masterLabel->setString(dis);
    m_bOpen = false;
    return ret;
}

int FavoriteCell::gettheirDistance(CCPoint pt){
    double result = 0;
    CCPoint playpt = WorldController::getInstance()->selfPoint;
    result = (pt.x - playpt.x)*(pt.x - playpt.x)+(pt.y - playpt.y)*(pt.y - playpt.y);
    return (int)sqrt(result);
}

void FavoriteCell::onEnter() {
    CCNode::onEnter();
    setSwallowsTouches(false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
}
void FavoriteCell::onExit() {
    setTouchEnabled(false);
    CCNode::onExit();
}

void FavoriteCell::onEditClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    onEditFavorite();
    
}

void FavoriteCell::addPic(int favortype){
    m_picNode->removeAllChildrenWithCleanup(true);
    if(favortype == -1){
        string pic;
        pic = "orignal_tile_allpic.png";
        auto sprite = CCLoadSprite::createSprite(pic.c_str());
        if( sprite == nullptr )
            return;
        
        sprite->setScale(1);
        m_picNode->addChild(sprite);
    }
    else if (favortype == 0) {
        string pic;
        pic = "orignal_tile_allpic.png";
        auto sprite = CCLoadSprite::createSprite(pic.c_str());
        if( sprite == nullptr )
            return;
        sprite->setScale(1);
        m_picNode->addChild(sprite);
    }
    else if(favortype == 1){  //城堡
        auto sprite = CCLoadSprite::createSprite("quest_BigCastle_icon1.png");
        if( sprite == nullptr )
            return;
        sprite->setScale(1);
        m_picNode->addChild(sprite);
    }
    else if(favortype == 3) {  //木粮铁银刚金
        int temptype = 0;
        auto it = WorldController::getInstance()->m_favoriteTiles.find(m_index);
        if(it != WorldController::getInstance()->m_favoriteTiles.end()){
            temptype = getresourceType(it->second.point,favortype);
        }
        string path= "";
        switch (temptype) {
            case 0:
                path = "quest_Sawmill_icon1.png";
                break;
            case 1:
                path = "quest_Mithril Mine_icon1.png";
                break;
            case 2:
                path = "quest_Iron Mine_icon1.png";
                break;
            case 3:
                path = "quest_Farm_icon1.png";
                break;
            case 4:
                path = "quest_Mithril Mine_icon1.png";
                break;
            case 5:
                path = "0021.png";
                break;
            default:
                path = "orignal_tile_allpic.png";
                break;
        }
        auto sprite = CCLoadSprite::createSprite(path.c_str());
        if( sprite == nullptr )
            return;
        if(temptype == 5){
            sprite->setScale(0.75);
            sprite->setPosition(ccp(0,-15));
        }else{
            sprite->setScale(1);
        }
        m_picNode->addChild(sprite);
    }
    else if(favortype == 6){  //遗迹
        auto sprite = CCLoadSprite::createSprite("quest_Ruins_icon1.png");
        if( sprite == nullptr )
            return;
        sprite->setScale(0.85);
        m_picNode->addChild(sprite);
    }
    else if(favortype == 10){ //王座
        auto sprite = CCLoadSprite::createSprite("icon_Allance_head_throne.png");
        if( sprite == nullptr )
            return;
        sprite->setScale(0.7);
        m_picNode->addChild(sprite);
    }
    else if(favortype == 12){ //投石机
        auto sprite = CCLoadSprite::createSprite("dj_7.png");
        if( sprite == nullptr )
            return;
        sprite->setScale(0.75);
        sprite->setPosition(ccp(0,25));
        m_picNode->addChild(sprite);
    }
    else if(favortype == 14){ //联盟堡垒
        auto sprite = CCLoadSprite::createSprite("territory_building_lv1.png");
        if( sprite == nullptr )
            return;
        sprite->setScale(0.85);
        m_picNode->addChild(sprite);
    }
    else if(favortype == 18){ //联盟资源
        int temptype = 0;
        auto it = WorldController::getInstance()->m_favoriteTiles.find(m_index);
        if(it != WorldController::getInstance()->m_favoriteTiles.end()){
            temptype =  getresourceType(it->second.point,favortype);
        }
        string path= "";
        switch (temptype) {
            case 0:
                path = "territory_resource_wood.png";
                break;
            case 1:
                path = "territory_resource_stone.png";
                break;
            case 2:
                path = "territory_resource_iron.png";
                break;
            case 3:
                path = "territory_resource_food.png";
                break;
            default:
                break;
        }
        auto sprite = CCLoadSprite::createSprite(path.c_str());
        if( sprite == nullptr )
            return;
        sprite->setScale(1);
        m_picNode->addChild(sprite);
    }
    else if(favortype == 20){
        auto sprite = CCLoadSprite::createSprite("territory_tower0.png");
        if( sprite == nullptr )
            return;
        sprite->setScale(0.75);
        m_picNode->addChild(sprite);
    }
    else if(favortype == 21){ //tile_wareHouse
        auto sprite = CCLoadSprite::createSprite("territory_warehouse0.png");
        if( sprite == nullptr )
            return;
        sprite->setScale(1);
        m_picNode->addChild(sprite);
    }else{
        string pic;
        pic = "orignal_tile_allpic.png";
        auto sprite = CCLoadSprite::createSprite(pic.c_str());
        if( sprite == nullptr )
            return;
        sprite->setScale(1);
        m_picNode->addChild(sprite);
    }
  
}

int  FavoriteCell::getresourceType(CCPoint pt,int favorType){
    int type = 0;
    auto it = WorldController::getInstance()->m_favoriteTiles.find(m_index);
    if(it != WorldController::getInstance()->m_favoriteTiles.end()){
        type = it->second.restype;
    }
    return type;
}

string  FavoriteCell::getOrignalName(CCPoint pt){
    auto it = WorldController::getInstance()->m_favoriteTiles.find(m_index);
    if(it != WorldController::getInstance()->m_favoriteTiles.end()){
        int m_index = WorldController::getIndexByPoint(it->second.point);
        auto info = WorldController::getInstance()->m_cityInfo.find(m_index);
        if(info != WorldController::getInstance()->m_cityInfo.end()){
            if(info->second.cityType == 0){
                return info->second.cityName;
            }
        }
    }
    return "";
}

int FavoriteCell::getPictype(CCPoint pt){
    int type = 0;
    auto it = WorldController::getInstance()->m_favoriteTiles.find(m_index);
    if(it != WorldController::getInstance()->m_favoriteTiles.end()){
        type = it->second.citytype;
    }
    return type;
}

void FavoriteCell::onEditFavorite()
{
    auto world = WorldMapView::instance();
    if (world) {
        auto it = WorldController::getInstance()->m_favoriteTiles.find(m_index);
        if(it != WorldController::getInstance()->m_favoriteTiles.end()){
            int m_index = WorldController::getIndexByPoint(it->second.point);
            bool flag= false;
            PopupViewController::getInstance()->addPopupView(AddFavorite::create(m_index,it->second.name,it->second.type,flag));
        }
    }
}

string FavoriteCell::addDelInfo(FavoriteTile m_tile){
    int index = WorldController::getIndexByPoint(m_tile.point);
    if(index == 0){
        return "";
    }
    string strIndex = CC_ITOA(index);
    int serverId = m_tile.serverId;
    string strServerId = CC_ITOA(serverId);
    return strIndex+ ";" +strServerId;
}

string FavoriteCell::delDelInfo(FavoriteTile m_tile){
    string result = "";
    int index = WorldController::getIndexByPoint(m_tile.point);
    if(index == 0){
        return "";
    }
    string strIndex = CC_ITOA(index);
    int serverId = m_tile.serverId;
    string strServerId = CC_ITOA(serverId);
    return strIndex+ ";" +strServerId;
}

void FavoriteCell::onSelectClick(CCObject * pSender, Control::EventType pCCControlEvent){
    m_select->setVisible(false);
    m_unselect->setVisible(true);
    auto it = WorldController::getInstance()->m_favoriteTiles.find(m_index);
    if(it != WorldController::getInstance()->m_favoriteTiles.end()){
        string str = this->delDelInfo(it->second);
        auto iter = find(vectForDel.begin(), vectForDel.end(), str);
        if(iter != vectForDel.end()){
            vectForDel.erase(iter);
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(UPDATE_DEL_NUM);
    }
}

void FavoriteCell::onUnSelectClick(CCObject * pSender, Control::EventType pCCControlEvent){
    m_select->setVisible(true);
    m_unselect->setVisible(false);
    auto it = WorldController::getInstance()->m_favoriteTiles.find(m_index);
    if(it != WorldController::getInstance()->m_favoriteTiles.end()){
        string str = this->addDelInfo(it->second);
        if (str != "") {
            auto iter = find(vectForDel.begin(), vectForDel.end(), str);
            if (iter == vectForDel.end()) {
                vectForDel.push_back(str);
            }
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(UPDATE_DEL_NUM);
    }
}

SEL_CCControlHandler FavoriteCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onEditClick", FavoriteCell::onEditClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSelectClick", FavoriteCell::onSelectClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onUnSelectClick", FavoriteCell::onUnSelectClick);
    return NULL;
}
bool FavoriteCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIFTTF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_coorLabel", CCLabelIF*, m_coorLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_toRight", CCNode*, m_toRight);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_MoveNode", CCNode*, m_MoveNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_select", CCNode*, m_select);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_unselect", CCNode*, m_unselect);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    return false;
}

void FavoriteCell::onRefreshData(CCObject* obj)
{
    int sid = dynamic_cast<CCInteger*>(obj)->getValue();
    if (sid == m_itemId) {
        onUpdateInfo(true);
    }
}

void FavoriteCell::onUpdateInfo(bool isUpdate)
{
}

void FavoriteCell::onEnterFrame(float dt)
{
}

bool FavoriteCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_startX = pTouch->getLocation().y;
    if (isTouchInside(m_toRight, pTouch)) {
        return true;
    }
    if (isTouchInside(m_touchNode, pTouch)) {
        return true;
    }
    return false;
}

void FavoriteCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    int mvy = abs(pTouch->getLocation().y - m_startX);
    if (isTouchInside(m_toRight, pTouch)) {
        onEditFavorite();
        return;
    }
    if (isTouchInside(m_touchNode, pTouch) && mvy < 3){
        onGotoFavoriteTile();
        return;
    }
}

void FavoriteCell::GoLeft(){
    m_toRight->setPositionX(m_toRight->getPositionX() - 60);
    m_bOpen = false;
}

void FavoriteCell::GoRight(){
    m_toRight->setPositionX(m_toRight->getPositionX() + 60);
    m_bOpen = true;
}

void FavoriteCell::onGotoFavoriteTile()
{
    auto it = WorldController::getInstance()->m_favoriteTiles.find(m_index);
    if(it != WorldController::getInstance()->m_favoriteTiles.end()){
        int index = WorldController::getIndexByPoint(it->second.point);
        WorldController::getInstance()->openTargetIndex = index;
        //zym 2015.12.11
        PopupViewController::getInstance()->forceClearAll(true);
        WorldMapView::instance()->gotoTilePoint(it->second.point, it->second.serverId);
        //CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(CLOSE_THIS_FAVOR);
    }
}


