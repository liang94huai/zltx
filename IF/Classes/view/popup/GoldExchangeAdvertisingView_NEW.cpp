//
//  GoldExchangeAdvertisingView_NEW.cpp
//  IF
//
//  Created by xxrdsg on 15/11/3.
//
//

#include "GoldExchangeAdvertisingView_NEW.hpp"
//#include "GoldExchangeAdvertisingCommCell_common.hpp"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "DataRecordCommand.h"
#include "ParticleController.h"
#include "ChangeNickNameView.h"
#include "RollingText.h"
#include "ChangePicView.h"
#include "ShieldUnlockPopUpView.h"
#include "SettingPopUpView.h"
#include "ToolController.h"
#include "YesNoDialog.h"
#include "ChangePicCommand.h"
#include "WorldController.h"
#include "RoleInfoView.h"
#include "GoldExchangeMoreListView.h"
#include "PaymentCommand.h"
#include "PayController.h"
#include "TipsView.h"
#include "GoldExchangeInfoCommand.h"
#include "RewardController.h"
#include "ButtonLightEffect.h"

#include "LuaController.h"

const int advertiseCellW_new = 616;

GoldExchangeAdvertisingView_NEW* GoldExchangeAdvertisingView_NEW::create(){
    GoldExchangeAdvertisingView_NEW* ret = new GoldExchangeAdvertisingView_NEW();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

GoldExchangeAdvertisingView_NEW::~GoldExchangeAdvertisingView_NEW(){
    CCObject* ccObj = NULL;
    string path = CCFileUtils::sharedFileUtils()->getWritablePath()+"lua/resources/";
    CCARRAY_FOREACH(m_data, ccObj){
        CCString *ccStr = dynamic_cast<CCString*>(ccObj);
        if(ccStr && ccStr->getCString()){
            auto iter = GlobalData::shared()->goldExchangeList.find(ccStr->getCString());
            if(iter!=GlobalData::shared()->goldExchangeList.end()){
                string popImg = (*iter).second->popup_image;
                if(popImg == "thanksgiving" || popImg=="winter" || popImg=="christmas" || popImg=="newyear" || popImg== "silent" || popImg == "throne" || popImg == "equip" || popImg =="lover"||popImg== "chunjie" || popImg == "1" || popImg == "2" || popImg == "new_recharge" || popImg == "month" || popImg == "throne" || popImg == "equip"){
                    
                }else{
                    if(!popImg.empty()){
                        string removeplist = path + popImg + "adv.plist";
                        string removetexture = path + popImg + "adv.pvr.ccz";
                        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(removeplist.c_str());
                        CCTexture2D *texture = CCTextureCache::sharedTextureCache()->textureForKey(removetexture.c_str());
                        if(texture!=NULL){
                            CCTextureCache::sharedTextureCache()->removeTexture(texture);
                        }
                    }
                }
            }
        }
    }
}
bool GoldExchangeAdvertisingView_NEW::init()
{
    m_data = CCArray::create();
    CCBLoadFile("GoldAdvertisingView_NEW",this,this);
    m_titleLabel->setColor({255, 194, 48});
    CCLoadSprite::doResourceByCommonIndex(6, true);
    setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(6, false);
        });
    m_tabView = TableView::create(this, m_scrollContainer->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionHorizontal);//
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchMode(kCCTouchesOneByOne);
    m_tabView->setTouchPriority(-1);
    m_tabView->setDelegate(this);
    m_tabView->setTouchEnabled(false);
    m_scrollContainer->addChild(m_tabView);
    
    m_animNode = CCNode::create();
    m_scrollContainer->getParent()->addChild(m_animNode);
    m_animNode->setVisible(false);
    
    refreshView(NULL);
    return true;
}

void GoldExchangeAdvertisingView_NEW::onEnter(){
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GoldExchangeAdvertisingView_NEW::refreshView), PAYMENT_COMMAND_RETURN, NULL);
    this->schedule(schedule_selector(GoldExchangeAdvertisingView_NEW::onPlayEnterFrame),10.0);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GoldExchangeAdvertisingView_NEW::recoverScroll), GOLDEXCHANGE_ADVERTISING_SCROLL, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GoldExchangeAdvertisingView_NEW::stopScroll), GOLDEXCHANGE_ADVERTISING_STOP_SCROLL, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GoldExchangeAdvertisingView_NEW::errorPayment), MSG_PAY_FAILED, NULL);
}
void GoldExchangeAdvertisingView_NEW::errorPayment(cocos2d::CCObject *obj){
    this->schedule(schedule_selector(GoldExchangeAdvertisingView_NEW::onShowError),0.3,1,0);
}
void GoldExchangeAdvertisingView_NEW::onShowError(float dt){
    PayController::getInstance()->showErrorDialog();
    this->unschedule(schedule_selector(GoldExchangeAdvertisingView_NEW::onShowError));
}
void GoldExchangeAdvertisingView_NEW::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, PAYMENT_COMMAND_RETURN);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GOLDEXCHANGE_ADVERTISING_STOP_SCROLL);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GOLDEXCHANGE_ADVERTISING_SCROLL);
    this->unschedule(schedule_selector(GoldExchangeAdvertisingView_NEW::onPlayEnterFrame));
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PAY_FAILED);
    CCNode::onExit();
}
void GoldExchangeAdvertisingView_NEW::onPlayEnterFrame(float dt){
    addButtonLight(false);
    int addX = -advertiseCellW_new;
    int offSetX = int((m_tabView->getContentOffset().x + addX) / advertiseCellW_new) * advertiseCellW_new;
    int minX  = m_tabView->minContainerOffset().x+20;
    if(offSetX > 0){
        offSetX = 0;
    }else
        if(offSetX < minX){
            offSetX = minX;
        }
    m_tabView->setContentOffsetInDuration(ccp(offSetX, 0), 0.5);
    this->scheduleOnce(schedule_selector(GoldExchangeAdvertisingView_NEW::showButtonLight), 0.5);
    setButtonState(offSetX,false);
}

void GoldExchangeAdvertisingView_NEW::generateData(){
    m_data->removeAllObjects();
    
    vector<unsigned int> tmpVector;
    GoldExchangeItem* envelopeitem = NULL;
    for (map<string, GoldExchangeItem*>::iterator it = GlobalData::shared()->goldExchangeList.begin(); it != GlobalData::shared()->goldExchangeList.end(); it++) {
        if(!it->second)
            continue;
        if(it->second->type == "5"){
            continue;
        }
        if(it->second->type == "1" && it->second->popup_image == "envelope_gift"){
            envelopeitem = it->second;
        }
        if(it->second->bought||(it->second->end<GlobalData::shared()->getWorldTime())){
            continue;
        }
        if(it->second->type == "3"&&it->second->popup_image!="close"){
            if(it->second->popup == 0){
                tmpVector.push_back(atoi((it->second->id).c_str()));
            }else{
                m_data->addObject(CCString::create(it->second->id));
            }
        }
    }
    sort(tmpVector.begin(), tmpVector.end());
    if(envelopeitem!=NULL){
        if(LuaController::getInstance()->checkLuaValid(envelopeitem->popup_image,envelopeitem->md5,envelopeitem->type) && LuaController::getInstance()->checkNewAdvLuaValid(envelopeitem->popup_image, envelopeitem->md5, envelopeitem->type)){
            tmpVector.insert(tmpVector.begin(), atoi(envelopeitem->id.c_str()));
        }
    }
    //排序
    int count = m_data->count();
    int i=0;
    while (i<count) {
        int j=0;
        string itemId1 = dynamic_cast<CCString*>(m_data->objectAtIndex(i))->getCString();
        map<string, GoldExchangeItem*>::iterator it1 = GlobalData::shared()->goldExchangeList.find(itemId1);
        while (j<i) {
            string itemId2 = dynamic_cast<CCString*>(m_data->objectAtIndex(j))->getCString();
            map<string, GoldExchangeItem*>::iterator it2 = GlobalData::shared()->goldExchangeList.find(itemId2);
            if(it1->second->popup>it2->second->popup){
                m_data->swap(i, j);
            }
            j++;
        }
        i++;
    }
    if(tmpVector.size()>0){
        auto iter0 = tmpVector.end()-1;
        while (iter0>=tmpVector.begin()) {
            CCString *strTmp = CCString::createWithFormat("%d",(*iter0));
            m_data->insertObject(strTmp, 0);
            --iter0;
        }
    }
    //控制礼包个数礼包，删除多余的
    string maxCount = CCCommonUtils::getPropById("79001", "k1");
    if(maxCount!="" && atoi(maxCount.c_str())>0){
        while (m_data->count() > atoi(maxCount.c_str())) {
            m_data->removeObjectAtIndex(m_data->count()-1);
        }
    }
    count = m_data->count();
    if(count>1){
        //将第一个插入到结尾
        string endItemId = dynamic_cast<CCString*>(m_data->objectAtIndex(0))->getCString();
        m_data->addObject(CCString::create(endItemId));
        
        //将最后一个插入到开始
        string startItemId = dynamic_cast<CCString*>(m_data->objectAtIndex(count-1))->getCString();
        m_data->insertObject(CCString::create(startItemId), 0);
    }
}
void GoldExchangeAdvertisingView_NEW::recoverScroll(CCObject* obj){
    this->unschedule(schedule_selector(GoldExchangeAdvertisingView_NEW::onPlayEnterFrame));
    this->schedule(schedule_selector(GoldExchangeAdvertisingView_NEW::onPlayEnterFrame),10.0);
    
}
void GoldExchangeAdvertisingView_NEW::stopScroll(CCObject* obj){
    this->unschedule(schedule_selector(GoldExchangeAdvertisingView_NEW::onPlayEnterFrame));
    
}
void GoldExchangeAdvertisingView_NEW::generalDataAgain(float _time){
    if(m_data->count()>1){
        addButtonLight(false);
        m_tabView->setContentOffset(ccp(-advertiseCellW_new, 0));
        addButtonLight(true);
    }
}
void GoldExchangeAdvertisingView_NEW::generalEndDataAgain(float _time){
    addButtonLight(false);
    int dx =-(m_data->count()-2)*advertiseCellW_new;
    m_tabView->setContentOffset(ccp(dx,0));
    addButtonLight(true);
}
void GoldExchangeAdvertisingView_NEW::refreshView(CCObject* obj){
    addButtonLight(false);
    generateData();
    bool ishasChild = false;
    CCPoint pos;
    if(m_tabView->getContainer()->getChildrenCount()==0){
        ishasChild = true;
    }else{
        pos = m_tabView->getContentOffset();
    }
    m_tabView->reloadData();
    int count = (m_data->count()>0)? m_data->count():1;
    if(!ishasChild){
        CCPoint minPt = m_tabView->minContainerOffset();
        CCPoint maxPt = m_tabView->maxContainerOffset();
        if (pos.x > maxPt.x) {
            pos.x = maxPt.x;
        }
        if (pos.x < minPt.x) {
            pos.x = minPt.x;
        }
        m_tabView->setContentOffset(pos);
    }else if (count>1){
        m_tabView->setContentOffset(ccp(-advertiseCellW_new, 0));
    }
    m_pageNode->removeAllChildren();
    int width = 30;
    
    if(count>1){
        count-=2;
        
    }
    m_startX= 0;
    if(count%2==1){
        m_startX = -int(count/2)*width;
    }else{
        m_startX = -int(count/2)*width+width/2;
    }
    for (int i=0; i<count; i++) {
        auto spr = CCLoadSprite::createSprite("VIP_fanye.png");
        m_pageNode->addChild(spr);
        spr->setPositionX(m_startX+width*i);
    }
    m_pageflag= CCLoadSprite::createSprite("VIP_fanye2.png");
    m_pageNode->addChild(m_pageflag);
    m_pageflag->setPositionX(0);
    
    setButtonState(m_tabView->getContentOffset().x,false);
    addButtonLight(true);
}


void GoldExchangeAdvertisingView_NEW::onClickCostBtn(CCObject * pSender, Control::EventType pCCControlEvent){
    if(m_dataItem==NULL){
        return;
    }
    PayController::getInstance()->callPayment(m_dataItem);
}

SEL_CCControlHandler GoldExchangeAdvertisingView_NEW::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCostBtn", GoldExchangeAdvertisingView_NEW::onClickCostBtn);
    
    return NULL;
}

void GoldExchangeAdvertisingView_NEW::setButtonState(int offX,bool isreset){
    
    if(m_data->count()<=1){
        this->unschedule(schedule_selector(GoldExchangeAdvertisingView_NEW::onPlayEnterFrame));
    }
    if(m_data->count()<=0){
        m_titleLabel->setString(_lang("101242"));
        m_bgNode->setVisible(true);
    }else{
        m_bgNode->setVisible(false);
    }
    
    int index = round(abs(offX) * 1.0 / advertiseCellW_new);
    m_dataItem = NULL;
    if(index < 0 || index >= m_data->count()){
        return;
    }else{
        std::string itemId = dynamic_cast<CCString*>(m_data->objectAtIndex(index))->getCString();
        map<string, GoldExchangeItem*>::iterator it =GlobalData::shared()->goldExchangeList.find(itemId.c_str());
        GoldExchangeItem* item = NULL;
        if ( it != GlobalData::shared()->goldExchangeList.end()){
            item = it->second;
            m_dataItem = item;
            m_titleLabel->setString("");
            if(item->type == "5"){
                m_titleLabel->setString(_lang("101281"));
            }else{
                if(item->popup_image=="thanksgiving"){
                    m_titleLabel->setString(_lang(item->name).c_str());
                }else if(item->popup_image == "winter") {
                    m_titleLabel->setString(_lang(item->name).c_str());
                } else {
                    m_titleLabel->setString(_lang(item->name).c_str());
                }
            }
        }
        if(m_data->count()<=1){
            return;
        }
        float delayTime = 0;
        if(isreset){
            delayTime = 0.2;
        }else{
            delayTime = 0.6;
        }
        if(index==m_data->count()-1){
            m_pageflag->setPositionX(m_startX);
            this->scheduleOnce(schedule_selector(GoldExchangeAdvertisingView_NEW::generalDataAgain),delayTime);
            
        }else if(index==0){
            m_pageflag->setPositionX(m_startX+30*(m_data->count()-2-1));
            this->scheduleOnce(schedule_selector(GoldExchangeAdvertisingView_NEW::generalEndDataAgain),delayTime);
        }else{
            m_pageflag->setPositionX(m_startX+30*(index-1));
        }
        
        
    }
}

bool GoldExchangeAdvertisingView_NEW::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel", CCLabelIF*, this->m_titleLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scrollContainer", CCNode*, this->m_scrollContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pageNode", CCNode*, this->m_pageNode);
    return false;
}

bool GoldExchangeAdvertisingView_NEW::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_scrollContainer, pTouch)){
        this->unschedule(schedule_selector(GoldExchangeAdvertisingView_NEW::showButtonLight));
        this->unschedule(schedule_selector(GoldExchangeAdvertisingView_NEW::onPlayEnterFrame));
        this->schedule(schedule_selector(GoldExchangeAdvertisingView_NEW::onPlayEnterFrame),10.0);
        m_x = 1000;
        return true;
    }else{
        return false;
    }
    
    
}

void GoldExchangeAdvertisingView_NEW::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_scrollContainer, pTouch)){
        int dx = pTouch->getLocation().x - pTouch->getStartLocation().x;
        if(dx > 10 || dx < -10){
            addButtonLight(false);
            if(m_x == 1000){
                m_x = m_tabView->getContentOffset().x;
            }
            int offX = m_x + dx;
            int min = m_tabView->minContainerOffset().x;
            int max = 200;
            if(offX > max){
                offX = max;
            }
            if(offX < min){
                offX = min;
            }
            m_tabView->setContentOffset(ccp(offX, 0), false);
        }
    }
}

void GoldExchangeAdvertisingView_NEW::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    
    int addX = -advertiseCellW_new / 2;
    int dx = pTouch->getStartLocation().x - pTouch->getLocation().x;
    if(dx > 20){
        addX = -advertiseCellW_new;
    }else if(dx < -20){
        addX = 0;
    }
    int offSetX = int((m_tabView->getContentOffset().x + addX) / advertiseCellW_new) * advertiseCellW_new;
    int minX  = m_tabView->minContainerOffset().x+20;
    if(offSetX > 0){
        offSetX = 0;
    }
    if(offSetX < minX){
        offSetX = minX;
    }
    m_tabView->setContentOffset(ccp(offSetX, 0), true);
    setButtonState(offSetX);
    this->scheduleOnce(schedule_selector(GoldExchangeAdvertisingView_NEW::showButtonLight), 0.15);
}

CCSize GoldExchangeAdvertisingView_NEW::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    return CCSize(advertiseCellW_new, 246);
}

CCSize GoldExchangeAdvertisingView_NEW::cellSizeForTable(CCTableView *table)
{
    return CCSize(advertiseCellW_new, 246);
}

CCTableViewCell* GoldExchangeAdvertisingView_NEW::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    
    string itemId = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();
    map<string, GoldExchangeItem*>::iterator it =GlobalData::shared()->goldExchangeList.find(itemId.c_str());
    GoldExchangeItem* item = NULL;
    if ( it != GlobalData::shared()->goldExchangeList.end()){
        item = it->second;
    }
    GoldExchangeAdvertisingCell_NEW* cell = (GoldExchangeAdvertisingCell_NEW*)table->dequeueCell();
    
    if(cell){
        cell->setData(item);
    }else{
        cell = GoldExchangeAdvertisingCell_NEW::create(item);
    }
    return cell;
}

ssize_t GoldExchangeAdvertisingView_NEW::numberOfCellsInTableView(CCTableView *table)
{
    int num = ceil(m_data->count() / 1.0f);
    return num;
}

void GoldExchangeAdvertisingView_NEW::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
    
}

void GoldExchangeAdvertisingView_NEW::scrollViewDidScroll(CCScrollView* view){
    CCLOG("%f", view->getContentOffset().x);
}
void GoldExchangeAdvertisingView_NEW::addButtonLight(bool bAdd){
    if(m_animNode){
        m_animNode->removeAllChildren();
        if(bAdd){
            CCPoint offset = m_tabView->getContentOffset();
            int index = ceil(-offset.x /advertiseCellW_new);
            GoldExchangeAdvertisingCell_NEW *cell = NULL;
            if(m_tabView && m_tabView->getContainer()){
                auto arr = m_tabView->getContainer()->getChildren();
                if( arr.size() > 0){
                    cell = dynamic_cast<GoldExchangeAdvertisingCell_NEW*>(arr.at(0));
                }
            }
            if(cell){
                CCRect rect = cell->getCostBtnRect();
                if(rect.equals(CCRectZero))
                    return;
                CCPoint pos = m_scrollContainer->getPosition() + rect.origin;
                m_animNode->setContentSize(rect.size);
                m_animNode->setPosition(pos);
                ButtonLightEffect* btnEffect = ButtonLightEffect::create(rect.size);
                m_animNode->addChild(btnEffect);
                m_animNode->setVisible(true);
            }
        }
    }
}
void GoldExchangeAdvertisingView_NEW::showButtonLight(float _time){
    addButtonLight(true);
}

//////--------------GoldExchangeAdvertisingCell_NEW
GoldExchangeAdvertisingCell_NEW *GoldExchangeAdvertisingCell_NEW::create(GoldExchangeItem* item){
    GoldExchangeAdvertisingCell_NEW *ret = new GoldExchangeAdvertisingCell_NEW(item);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void GoldExchangeAdvertisingCell_NEW::setData(GoldExchangeItem* item){
    this->removeAllChildren();
    m_item = item;
    m_costBtnRect = CCRectZero;
    if(m_item->type == "5"){
        
    }else{
        if(m_item->type=="1" && m_item->popup_image=="envelope_gift"){
            if(LuaController::getInstance()->checkLuaValid(m_item->popup_image,m_item->md5,m_item->type) && LuaController::getInstance()->checkNewAdvLuaValid(m_item->popup_image, m_item->md5, m_item->type)){
                std::string info0 = m_item->toString();
                m_costBtnRect = LuaController::getInstance()->createExchangeAdvCell(this,info0);
            }
        }else{
            if(m_item->popup_image=="thanksgiving" || m_item->popup_image=="winter" || m_item->popup_image=="christmas" || m_item->popup_image=="newyear" || m_item->popup_image == "silent" || m_item->popup_image == "throne" || m_item->popup_image == "equip" || m_item->popup_image =="lover"||m_item->popup_image== "chunjie"){
                //                auto cell = GoldExchangeHallweenCell::create(m_item);
                //                this->addChild(cell);
                //                cell->setPositionY(-66);
                //                m_costBtnRect = cell->getCostBtnRect();
            }else{
                if(LuaController::getInstance()->checkLuaValid(m_item->popup_image,m_item->md5,m_item->type) && LuaController::getInstance()->checkNewAdvLuaValid(m_item->popup_image, m_item->md5, m_item->type)){
                    std::string info = m_item->toString();
                    m_costBtnRect = LuaController::getInstance()->createExchangeAdvCell(this,info);
//                    auto cell = GoldExchangeAdvertisingCommCell_common::create(m_item);
//                    this->addChild(cell);
//                    m_costBtnRect = cell->getCostBtnRect();
                }else{
                    auto cell = GoldExchangeAdvertisingCommCell_NEW::create(m_item);
                    this->addChild(cell);
                    m_costBtnRect = cell->getCostBtnRect();
                }
            }
        }
    }
}

bool GoldExchangeAdvertisingCell_NEW::init(){
    this->setContentSize(CCSize(616, 246));
    setData(m_item);
    return true;
}

void GoldExchangeAdvertisingCell_NEW::onEnter(){
    CCTableViewCell::onEnter();
}

void GoldExchangeAdvertisingCell_NEW::onExit(){
    CCTableViewCell::onExit();
}
//......//
GoldExchangeAdvertisingCommCell_NEW* GoldExchangeAdvertisingCommCell_NEW::create(GoldExchangeItem *goldExchangeItem){
    GoldExchangeAdvertisingCommCell_NEW* ret = new GoldExchangeAdvertisingCommCell_NEW(goldExchangeItem);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GoldExchangeAdvertisingCommCell_NEW::init()
{
    
    CCBLoadFile("GoldAdvertisingCommonCell_NEW",this,this,false);
    this->setContentSize(CCSizeMake(616, 246));
    m_percentLabel->setColor({255, 222, 106});
    m_desLabel->setColor({197, 130, 71});
    m_timeLabel->setColor({255, 229, 168});
    m_newPriceLabel->setColor({255, 255, 255});
    refreshData();
    return true;
}
void GoldExchangeAdvertisingCommCell_NEW::refreshData(){
    string goldnum = "Gold+";
    goldnum += CC_CMDITOA(atoi(m_dataItem->gold_doller.c_str()));
    m_getGoldNumText->setString(goldnum);
    string perStr = m_dataItem->percent+"%";
    m_percentLabel->setString(perStr);
    m_desLabel->setString(_lang("101237"));
    string dollar = PayController::getInstance()->getDollarText(m_dataItem->dollar,m_dataItem->product_id);
    this->m_newPriceLabel->setString(dollar.c_str());
    m_btnGivePackage->setVisible(m_dataItem->send_mail);
    onEnterFrame(0.0);
}
CCRect GoldExchangeAdvertisingCommCell_NEW::getCostBtnRect(){
    CCSize size = m_costBtn->getContentSize();
    CCPoint pos = m_costBtn->getPosition();
    pos = m_costBtn->getParent()->convertToWorldSpace(pos);
    pos = this->convertToNodeSpace(pos);
    return CCRect(pos.x, pos.y, size.width, size.height);
}
void GoldExchangeAdvertisingCommCell_NEW::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    setSwallowsTouches(false);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GoldExchangeAdvertisingCommCell_NEW::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    
}

void GoldExchangeAdvertisingCommCell_NEW::onExit()
{
    
    setTouchEnabled(false);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(GoldExchangeAdvertisingCommCell_NEW::onEnterFrame), this);
    CCNode::onExit();
}

SEL_CCControlHandler GoldExchangeAdvertisingCommCell_NEW::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCostBtn", GoldExchangeAdvertisingCommCell_NEW::onClickCostBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickGivePackageBtn", GoldExchangeAdvertisingCommCell_NEW::onClickGivePackageBtn);
    return NULL;
}

bool GoldExchangeAdvertisingCommCell_NEW::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getGoldNumText", CCLabelBMFont*, this->m_getGoldNumText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_percentLabel", CCLabelIF*, this->m_percentLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, this->m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newPriceLabel", CCLabelIF*, this->m_newPriceLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchMoreNode1", CCNode*, this->m_touchMoreNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_costBtn", CCControlButton*, this->m_costBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_showMoneyNode", CCNode*, this->m_showMoneyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnGivePackage", CCControlButton*, this->m_btnGivePackage);
    return false;
}
void GoldExchangeAdvertisingCommCell_NEW::onClickCostBtn(CCObject * pSender, Control::EventType pCCControlEvent){
    if(m_dataItem==NULL){
        return;
    }
    if(m_dataItem->type=="3"&&m_dataItem->isPayCallBack){
        return;
    }
    // 屏蔽 ---liu
    CCCommonUtils::flyHint("", "", _lang("101223"));
    return;
    PayController::getInstance()->callPayment(m_dataItem);
}
void GoldExchangeAdvertisingCommCell_NEW::onClickGivePackageBtn(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){
    if(m_dataItem==NULL){
        return;
    }
    LuaController::toSelectUser(m_dataItem->id, true,2);
}
void GoldExchangeAdvertisingCommCell_NEW::onEnterFrame(float dt)
{
    int curTime = GlobalData::shared()->getWorldTime();
    int lastTime = 0;
    if (m_dataItem->time>0) {
        int gapTime =(m_dataItem->end - curTime);
        int count =gapTime/(m_dataItem->time*3600);
        lastTime = m_dataItem->end - (m_dataItem->time*3600)*count-curTime;
    }else{
        lastTime = m_dataItem->end - curTime;
    }
    
    m_timeLabel->setString(CC_SECTOA(lastTime));
    if (m_dataItem->end - curTime<=0) {
        PopupViewController::getInstance()->removeAllPopupView();
    }
}
bool GoldExchangeAdvertisingCommCell_NEW::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    m_touchLocation = pTouch->getLocation();
    if(isTouchInside(this->m_touchMoreNode1, pTouch)) {
        return true;
    }
    return false;
}
void GoldExchangeAdvertisingCommCell_NEW::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if (fabs(m_touchLocation.y - pTouch->getLocation().y) > 10 || fabs(m_touchLocation.x - pTouch->getLocation().x) > 10) {
        return;
    }
    if((isTouchInside(m_touchMoreNode1, pTouch))&&(m_dataItem->item != "")){
  //屏蔽 --liu
    CCCommonUtils::flyHint("", "", _lang("101223"));
    return;
//        CCLoadSprite::doResourceByCommonIndex(8, true);
        auto view = GoldExchangeMoreListView::create(atoi(m_dataItem->id.c_str()));
//        view->setCleanFunction([](){
//            CCLoadSprite::doResourceByCommonIndex(8, false);
//            CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_GOODS);
//        });
        PopupViewController::getInstance()->addPopupView(view);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GOLDEXCHANGE_ADVERTISING_STOP_SCROLL);
    }
}
void GoldExchangeAdvertisingCommCell_NEW::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}