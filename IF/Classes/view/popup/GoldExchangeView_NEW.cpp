//
//  GoldExchangeView_NEW.cpp
//  IF
//
//  Created by xxrdsg on 15/11/3.
//
//

#include "GoldExchangeView_NEW.hpp"
#include "PopupViewController.h"
#include "GoldExchangeItemView_NEW.hpp"
#include "DataRecordCommand.h"
#include "GoldExchangeInfoCommand.h"
#include "GoldExchangeSaleView.h"
#include "LuaController.h"
#include "GoldExchangeAdvertisingView_NEW.hpp"
#include "UIComponent.h"
#include "FreshRechargeView.hpp"

GoldExchangeView_NEW::GoldExchangeView_NEW(){
    
}

GoldExchangeView_NEW::~GoldExchangeView_NEW(){
    if(mCardList.size()>0){
        auto iter = mCardList.begin();
        while(iter!=mCardList.end()){
            delete (*iter);
            (*iter)=NULL;
            ++iter;
        }
        mCardList.clear();
    }
}

GoldExchangeView_NEW* GoldExchangeView_NEW::create(){
    GoldExchangeView_NEW* ret = new GoldExchangeView_NEW();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool GoldExchangeView_NEW::init(){
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(2, true);
        CCLoadSprite::doResourceByCommonIndex(8, true);
        CCLoadSprite::doResourceByCommonIndex(513, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(8, false);
            CCLoadSprite::doResourceByCommonIndex(513, false);
        });
        auto bg = CCBLoadFile("GoldExchangeView_NEW", this, this,true);
        CCSize winsize = CCDirector::sharedDirector()->getWinSize();
        this->setContentSize(winsize);
        if (!CCCommonUtils::isIosAndroidPad())
        {
            float addH = winsize.height - 852;
            m_listNode->setContentSize(Size(m_listNode->getContentSize().width, m_listNode->getContentSize().height + addH));
            m_bg->setPreferredSize(winsize);
        } else {
            float addH = winsize.height - 2048;
            addH = addH / 2.4;
            m_listNode->setContentSize(Size(m_listNode->getContentSize().width, m_listNode->getContentSize().height + addH));
        }
        float height = 0;
        auto fr = CCLoadSprite::loadResource("technology_09.png");
        SpriteBatchNode* batch = SpriteBatchNode::createWithTexture(fr->getTexture());
        while (height < winsize.height) {
            Sprite* pic = CCLoadSprite::createSprite("technology_09.png");
            pic->setAnchorPoint(Vec2(0, 0));
            pic->setPosition(0, height);
            batch->addChild(pic);
            height += pic->getContentSize().height;
        }
        m_BGNode->addChild(batch);
        auto layer = Layer::create();
        layer->setTouchEnabled(true);
        layer->setSwallowsTouches(true);
        layer->setContentSize(Size(640, 300));
        m_geNode->addChild(layer);
        if(isGetData()){
            getData();
        }else{
            refresh(NULL);
        }
        return true;
    }
    return false;
}
void GoldExchangeView_NEW::refresh(CCObject* obj){
    if (obj == NULL) {
        initList();
    }else{
        CCNode* node = m_advertisNode->getChildByTag(1000);
        if(node){
            CCSize listSize = m_listNode->getContentSize();
            float addH = node->getContentSize().height;
            listSize.height += addH;
            m_listNode->setContentSize(listSize);
            m_listNode->setPositionY(m_listNode->getPositionY()+addH);
            m_scrollView->setViewSize(listSize);
            m_scrollView->setContentOffset(ccp(0, m_listNode->getContentSize().height - m_scrollView->getContentSize().height));
            node->removeFromParent();
        }
    }
}
bool GoldExchangeView_NEW::isGetData(){
    time_t t;
    t =GlobalData::shared()->getWorldTime();
    if(CCCommonUtils::timeStampToMD(GlobalData::shared()->goldExchageRefreshStamp)==CCCommonUtils::timeStampToMD(t)){
        return false;
    }
    return true;
}
void GoldExchangeView_NEW::getData(){
    
    GoldExchangeInfoCommand* cmd = new GoldExchangeInfoCommand();
    cmd->sendAndRelease();
}

bool GoldExchangeView_NEW::sortExchangeItem(GoldExchangeItem *item1, GoldExchangeItem *item2){
    float a = atof(item1->dollar.c_str());
    float b = atof(item2->dollar.c_str());
    return (a<b);
    
}
void GoldExchangeView_NEW::initList(){
    m_scrollView = ScrollView::create(m_listNode->getContentSize());
    m_scrollView->setTouchMode(kCCTouchesOneByOne);
    m_scrollView->setTouchPriority(Touch_Default);
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    
    vector<GoldExchangeItem*> sortMap;
    map<string, GoldExchangeItem*> sortMap1;
    map<string, GoldExchangeItem*> sortMap2;
    
    for (map<string, GoldExchangeItem*>::iterator it = GlobalData::shared()->goldExchangeList.begin(); it != GlobalData::shared()->goldExchangeList.end(); it++) {
        if(!it->second)
            continue;
        if (it->second->type == "1") {
            int iID = ( 5 - atoi(it->second->type.c_str())) * 100000 + atoi(it->second->id.c_str());
            string id = CC_ITOA(iID);
            sortMap.push_back(it->second);
        }else if(it->second->type == "2"){
            int iID = it->second->popup*1000000+atoi(it->second->id.c_str());
            sortMap1[CC_ITOA(iID)] = it->second;
        }else if(it->second->type == "3"){
            int iID = it->second->popup*1000000+atoi(it->second->id.c_str());
            sortMap2[CC_ITOA(iID)] = it->second;
        }
    }
    std::sort(sortMap.begin(),sortMap.end(),sortExchangeItem);
    
    int i=0;
    float dy=0;
    CCNode* listNode = CCNode::create();
    m_scrollView->addChild(listNode);
    
    ///新手累积充值入口
    
    if (GlobalData::shared()->bFreshRechargeOpen)
    {
        FreshRechargeEnter* rukou = FreshRechargeEnter::create();
        dy -= 168;
        rukou->setPosition(Vec2(0, dy));
        listNode->addChild(rukou);
    }
    
    auto it = sortMap.begin();
    GoldExchangeItemView_NEW* cell=NULL;
    while (it != sortMap.end()){
        if (true) {
            if((*it)->type=="1" && (*it)->popup_image != "hide" && (*it)->popup_image != "envelope_gift" ){
                cell =  GoldExchangeItemView_NEW::create((*it));
                cell->setPositionX(0);
                listNode->addChild(cell);
                dy -= 145;
                cell->setPositionY(dy);
            }
        }
        it++;
    }
    if(GlobalData::shared()->analyticID == "mycard" || GlobalData::shared()->analyticID == "gash"){
        int addCount = 1;
        int iIndex = 0;
        while (iIndex<addCount) {
            GoldExchangeItem *item1 = new GoldExchangeItem();
            item1->id = CCString::createWithFormat("-%d",(iIndex+1))->getCString();
            mCardList.push_back(item1);
            cell =  GoldExchangeItemView_NEW::create(item1);
            cell->setPositionX(0);
            listNode->addChild(cell);
            dy -= 145;
            cell->setPositionY(dy);
            ++iIndex;
        }
    }
    if (abs(dy) < m_listNode->getContentSize().height)
    {
        dy = -m_listNode->getContentSize().height;
    }
    
    listNode->setPositionY(abs(dy));
    m_scrollView->setContentSize(CCSize(m_listNode->getContentSize().width, abs(dy)));
    m_scrollView->setContentOffset(ccp(0, m_listNode->getContentSize().height - abs(dy)));
    m_listNode->addChild(m_scrollView);
    m_scrollView->setDelegate(this);
    
    //显示 advertising
    GoldExchangeAdvertisingView_NEW* advertisingview = GoldExchangeAdvertisingView_NEW::create();
    m_advertisNode->addChild(advertisingview);
}

void GoldExchangeView_NEW::onEnter(){
    PopupBaseView::onEnter();
    setTitleName(_lang("101221"));
    UIComponent::getInstance()->showPopupView(8);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GoldExchangeView_NEW::refresh), GOLDEXCHANGE_SHOW_COMMAND, NULL);
}

void GoldExchangeView_NEW::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GOLDEXCHANGE_SHOW_COMMAND);
    PopupBaseView::onExit();
}

bool GoldExchangeView_NEW::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, this->m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_advertisNode", CCNode*, this->m_advertisNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_BGNode", CCNode*, this->m_BGNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldText", CCLabelIF*, this->m_goldText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_geNode", Node*, m_geNode);
    return false;
}

SEL_CCControlHandler GoldExchangeView_NEW::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}

void GoldExchangeView_NEW::onCloseClick(cocos2d::CCObject *pSender, CCControlEvent event){
    closeSelf();
}

void GoldExchangeView_NEW::scrollViewDidScroll(ScrollView* view)
{
    return;
    float mindy = m_scrollView->minContainerOffset().y ;
    float maxdy = m_scrollView->maxContainerOffset().y ;
    float dy = m_scrollView->getContentOffset().y;
    if (dy < mindy)
    {
        m_tableView->setContentOffset(ccp(0, mindy));
    }
    if (dy > maxdy) {
        m_scrollView->setContentOffset(ccp(0, maxdy));
    }
}