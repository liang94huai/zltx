//
//  HongBaoGetView.cpp
//  IF
//
//  Created by 张军 on 15/11/19.
//
//

#include "HongBaoGetView.h"
#include "UIComponent.h"
#include "YesNoDialog.h"
#include "ToolController.h"
#include "WorldController.h"

#include "PopupViewController.h"

#pragma mark HongBaoInfo
HongBaoInfo::HongBaoInfo(CCDictionary* dict)
{
    updateInfo(dict);
}
HongBaoInfo* HongBaoInfo::create()
{
    auto ret = new HongBaoInfo();
    ret->autorelease();
    return ret;
}
HongBaoInfo* HongBaoInfo::create(CCDictionary* dict)
{
    auto ret = new HongBaoInfo();
    ret->updateInfo(dict);
    ret->autorelease();
    return ret;
}
void HongBaoInfo::updateInfo(cocos2d::CCDictionary *dict)
{
    if (dict->objectForKey("name")) {
        name = dict->valueForKey("name")->getCString();
    }
    if (dict->objectForKey("value")) {
        value = dict->valueForKey("value")->intValue();
    }
    if (dict->objectForKey("time")) {
        time = dict->valueForKey("time")->doubleValue();
    }
    if (dict->objectForKey("pic")) {
        pic = dict->valueForKey("pic")->getCString();
    }
    if (dict->objectForKey("player")) {
        player = dict->valueForKey("player")->getCString();
    }
    if (dict->objectForKey("picVer")) {
        picVer = dict->valueForKey("picVer")->intValue();
    }
}

#pragma mark HongBaoGetView
HongBaoGetView* HongBaoGetView::create(string redId,string serverId,bool isViewOnly){
    HongBaoGetView* ret = new HongBaoGetView();
    if(ret && ret->init(redId,serverId,isViewOnly)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool HongBaoGetView::init(string redId,string serverId,bool isViewOnly)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    hongBaoId=redId;
    m_serverId=serverId;
    m_isViewOnly = isViewOnly;
    setIsHDPanel(true);
    
    CCLoadSprite::doResourceByCommonIndex(11, true);
    CCLoadSprite::doResourceByCommonIndex(514, true);
    CCLoadSprite::doResourceByCommonIndex(6, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
        CCLoadSprite::doResourceByCommonIndex(514, false);
        CCLoadSprite::doResourceByCommonIndex(6, false);
        CCLoadSprite::doResourceByCommonIndex(7, false);
    });
    auto tmpCCB = CCBLoadFile("HongBaoGetView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    if (!CCCommonUtils::isIosAndroidPad()) {
        int addHeight = CCDirector::sharedDirector()->getWinSize().height - 852;
        m_bottomNode->setPositionY(m_bottomNode->getPositionY()-addHeight);
        m_infoList->setContentSize(CCSizeMake(m_infoList->getContentSize().width, m_infoList->getContentSize().height+addHeight));
    }
    
    //title
    m_titleTxt->setString(_lang("104994"));//红包详情
    //领红包，查看红包
    ToolController::getInstance()->stratGetRedPacket(hongBaoId,m_serverId,m_isViewOnly);
    return true;
}
//获取红包信息返回
void HongBaoGetView::onGetRedPacket(CCObject * obj){
    if(obj==NULL){
        PopupViewController::getInstance()->goBackPopupView();
        return;
    }
    CCDictionary *para = dynamic_cast<CCDictionary*>(obj);
    //领红包
    if (para->objectForKey("add") && para->objectForKey("gold")) {
        int addGold = para->valueForKey("add")->intValue();
        double gold = para->valueForKey("gold")->doubleValue();
        UIComponent::getInstance()->updateGold(gold);
        m_goldGetNode->setVisible(true);
        m_goldGetTxt->setString(CC_ITOA(addGold));
        m_goldGetTxt->setFontSize(45);
        m_goldGetTxt->setPositionY(m_goldGetTxt->getPositionY()+3);
        m_desTxt->setString(_lang("104976"));//已到账
    }else{//查看红包
        m_goldGetNode->setVisible(false);
//        m_desTxt->setPositionY(m_desTxt->getPositionY()+20);
        int recordGold = para->valueForKey("recordGold")->intValue();
        if(recordGold>0){
//            m_desTxt->setString(_lang("104977"));//您已领取过红包
            m_goldGetNode->setVisible(true);
            m_goldGetTxt->setString(CC_ITOA(recordGold));
            m_goldGetTxt->setFontSize(45);
            m_goldGetTxt->setPositionY(m_goldGetTxt->getPositionY()+3);
            m_desTxt->setString(_lang("104976"));//已到账
        }else{
//            m_desTxt->setString(_lang("104978"));//好可惜，您来晚了，红包已经被抢光了
            m_mNode->setPositionY(m_mNode->getPositionY()+120);
            m_infoList->setContentSize(CCSizeMake(m_infoList->getContentSize().width, m_infoList->getContentSize().height+120));
        }
        
    }
    
    //手气最佳
    int maxGetGold = 0;
    m_maxGetGold = 0;
    //最晚被领时间
    double lTime = 0;
    //被领钱数
    int getTotleGold = 0;
    if (para->objectForKey("records")) {
        CCArray* array = dynamic_cast<CCArray*>(para->objectForKey("records"));
        for (int i=0; i<array->count(); i++) {
            CCDictionary* arrDict = dynamic_cast<CCDictionary*>(array->objectAtIndex(i));

            int getGold = arrDict->valueForKey("value")->intValue();
            if (getGold > maxGetGold) {
                maxGetGold = getGold;
            }
            getTotleGold += getGold;
            if(arrDict->valueForKey("time")->doubleValue()>lTime){
                lTime=arrDict->valueForKey("time")->doubleValue();
            }
            
            HongBaoInfo *hbInfo = HongBaoInfo::create(arrDict);
            m_data.push_back(*hbInfo);
        }
        int maxCount=0;
        for (int i=0; i<array->count(); i++) {
            CCDictionary* arrDict = dynamic_cast<CCDictionary*>(array->objectAtIndex(i));
            int getGold = arrDict->valueForKey("value")->intValue();
            if (getGold == maxGetGold) {
                maxCount++;
                if (maxCount>1) {
                    maxGetGold=0;
                    break;
                }
            }
        }
    }
    lTime/=1000;
    
    if (para->objectForKey("info")) {
        CCDictionary* dict = dynamic_cast<CCDictionary*>(para->objectForKey("info"));
        //发红包人
        string senderName = dict->valueForKey("senderName")->getCString();
        m_nameTxt->setString(_lang_1("104988", senderName.c_str()));//104988		{0}的红包
        //发红包时间
        double sendTime = dict->valueForKey("sendTime")->doubleValue();
        sendTime/=1000;
        time_t time1 = sendTime;
        string tTime = CCCommonUtils::timeStampToDate(time1);
        m_timeTxt->setString(tTime);
        //设置头像
        string pic = dict->valueForKey("pic")->getCString();
        int picVer = dict->valueForKey("picVer")->intValue();
        string senderUid = dict->valueForKey("senderUid")->getCString();
        m_headIconNode->removeAllChildrenWithCleanup(true);
        if (pic.empty()) {
            pic = "g044.png";
        } else {
            pic.append(".png");
        }
        auto icon = CCLoadSprite::createSprite(pic.c_str(),true,CCLoadSpriteType_HEAD_ICON);
        CCCommonUtils::setSpriteMaxSize(icon, 84, true);
        m_headIconNode->addChild(icon);
        if (CCCommonUtils::isUseCustomPic(picVer)){
            m_headImgNode = HFHeadImgNode::create();
            m_headImgNode->initHeadImgUrl2(m_headIconNode, CCCommonUtils::getCustomPicUrl(senderUid, picVer), 1.0f, 84, true);
        }
        //红包总数
        int totalNum = dict->valueForKey("totalNum")->intValue();
        //已领红包数
        int getNum = dict->valueForKey("getNum")->intValue();
        //总钱数
        int goldTotal = dict->valueForKey("goldTotal")->intValue();
        if (getNum==totalNum) {//领取完=104995,104995={0}个红包，共{1}金币
            m_maxGetGold = maxGetGold;//领取完显示手气最佳
            m_getInfoTxt->setString(_lang_2("104995", CC_ITOA(totalNum), CC_ITOA(goldTotal)));//{0}个红包，共{1}金币
        }else{//未领取完=104991,104991=已领取{0}/{1}个，共{2}/{3}金币
            m_getInfoTxt->setString(_lang_4("104991", CC_ITOA(getNum), CC_ITOA(totalNum), CC_ITOA(getTotleGold), CC_ITOA(goldTotal)));//已领取{0}/{1}个，共{2}/{3}金币
        }
        //获取红包到期时间
        int lastTime = atoi(CCCommonUtils::getPropById("209639", "para").c_str());//红包Id 209639
        double vTime = sendTime + lastTime * 60.0 * 60.0;
        double tmpCurTime = WorldController::getInstance()->getTime()/1000;
        if (vTime<tmpCurTime) {//过期
            //    红包未领取完但是时间到被返还：160256 该红包已过期
            string str = _lang_1("160256", _lang("104983").c_str());//160256=该{0}已过期；104983=红包
            m_bottomMsg->setString(str);
        }else{
            if (getNum!=totalNum){
                //    当红包未领取完：dialog=104977 {0}小时未领取，剩余金币将退回
                m_bottomMsg->setString(_lang_1("104977", CC_ITOA(lastTime)));
            }else{
                if (lTime>sendTime) {
                    int dTime = lTime - sendTime;
                    if (dTime>3600) {
                        int hour = dTime/3600;
                        int min = (dTime%3600)/60;
                        string timeStr = _lang_2("160257", CC_ITOA(hour), CC_ITOA(min));//160257		{0}时{1}分
                        m_bottomMsg->setString(_lang_1("104997", timeStr.c_str()));//红包已被领取完：dialog=104997 {0}被抢光
                    }else if (dTime>60 && dTime<=3600){
                        int min = dTime/60;
                        int sec = dTime%60;
                        string timeStr = _lang_2("160258", CC_ITOA(min), CC_ITOA(sec));//160258		{0}分{1}秒
                        m_bottomMsg->setString(_lang_1("104997", timeStr.c_str()));//红包已被领取完：dialog=104997 {0}被抢光
                    }else{
                        m_bottomMsg->setString(_lang_1("104997", CC_ITOA(dTime)));//红包已被领取完：dialog=104997 {0}被抢光
                    }
                }
            }
        }
        
    }
    
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Popup);
    m_tabView->setDelegate(this);
    m_tabView->setTag(11);
    m_infoList->addChild(m_tabView);
}

void HongBaoGetView::onEnter(){
    CCNode::onEnter();
    UIComponent::getInstance()->showPopupView(1);
    UIComponent::getInstance()->m_popupReturnBtn->setVisible(false);
    
    this->setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(HongBaoGetView::onGetRedPacket), "HongBaoGetView_get", NULL);
}

void HongBaoGetView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "HongBaoGetView_get");
    this->setTouchEnabled(false);
    CCNode::onExit();
}

bool HongBaoGetView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}
void HongBaoGetView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    
}

#pragma mark tableView
//CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
CCSize HongBaoGetView::cellSizeForTable(CCTableView *table){
    return CCSizeMake(620, 120);
}

CCTableViewCell* HongBaoGetView::tableCellAtIndex(CCTableView *table, ssize_t idx){
    HongBaoGetViewCell* cell = (HongBaoGetViewCell*)table->dequeueCell();
    HongBaoInfo& info = m_data.at(idx);
    if (cell) {
        cell->setData(info);
    } else {
        cell = HongBaoGetViewCell::create(info);
    }
    if (m_maxGetGold > 0 && info.value == m_maxGetGold) {
        cell->setFire();
    }
    return cell;
}

ssize_t HongBaoGetView::numberOfCellsInTableView(CCTableView *table){
    return m_data.size();
}

void HongBaoGetView::tableCellTouched(CCTableView* table, CCTableViewCell* cell){
}

#pragma mark ccb
SEL_CCControlHandler HongBaoGetView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBackBtnClick", HongBaoGetView::onBackBtnClick);
    return NULL;
}
bool HongBaoGetView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_headIconNode", CCNode*, this->m_headIconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_goldGetTxt", CCLabelIF*, this->m_goldGetTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_desTxt", CCLabelIF*, this->m_desTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_getInfoTxt", CCLabelIF*, this->m_getInfoTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_goldGetNode", CCNode*, this->m_goldGetNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bottomMsg", CCLabelIF*, this->m_bottomMsg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_mNode", CCNode*, this->m_mNode);
    
    return false;
}

void HongBaoGetView::onBackBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    PopupViewController::getInstance()->goBackPopupView();
}

#pragma mark HongBaoGetViewCell
HongBaoGetViewCell* HongBaoGetViewCell::create(HongBaoInfo& hbInfo){
    auto ret = new HongBaoGetViewCell();
    if (ret && ret->init(hbInfo)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool HongBaoGetViewCell::init(HongBaoInfo& hbInfo){
    CCBLoadFile("HongBaoGetViewCell", this, this);
    setData(hbInfo);
    return true;
}

void HongBaoGetViewCell::setData(HongBaoInfo& hbInfo){
    m_Info = hbInfo;
    m_nameTxt->setString(hbInfo.name);
    m_goldTxt->setString(CC_ITOA(hbInfo.value));
    //领红包时间
    double sendTime = hbInfo.time;
    sendTime/=1000;
    time_t time1 = sendTime;
    string tTime = CCCommonUtils::timeStampToDate(time1);
    m_timeTxt->setString(tTime);
    //设置头像
    m_headIconNode->removeAllChildrenWithCleanup(true);
    string picPath = hbInfo.pic;
    if (picPath.empty()) {
        picPath = "g044.png";
    } else {
        picPath.append(".png");
    }
    auto icon = CCLoadSprite::createSprite(picPath.c_str(),true,CCLoadSpriteType_HEAD_ICON);
    CCCommonUtils::setSpriteMaxSize(icon, 84, true);
    m_headIconNode->addChild(icon);
    if (CCCommonUtils::isUseCustomPic(hbInfo.picVer))
    {
        m_headImgNode = HFHeadImgNode::create();
        m_headImgNode->initHeadImgUrl2(m_headIconNode, CCCommonUtils::getCustomPicUrl(hbInfo.player, hbInfo.picVer), 1.0f, 84, true);
    }
    m_luckyTxt->setString("");
}

void HongBaoGetViewCell::setFire(){
    m_luckyTxt->setString(_lang("160260"));//160260		手气最佳
    CCSprite* picSpr = CCLoadSprite::createSprite("common514_hb_fire.png");
    float px = m_luckyTxt->getPositionX() - m_luckyTxt->getContentSize().width - picSpr->getContentSize().width;
    float py = m_luckyTxt->getPositionY();
    picSpr->setPosition(CCPoint(px,py));
    this->addChild(picSpr);
}

bool HongBaoGetViewCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return false;
}

void HongBaoGetViewCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}

void HongBaoGetViewCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}

bool HongBaoGetViewCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldTxt", CCLabelIF*, m_goldTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeTxt", CCLabelIF*, m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headIconNode", CCNode*, m_headIconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_luckyTxt", CCLabelIF*, m_luckyTxt);
    return false;
}

SEL_CCControlHandler HongBaoGetViewCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickAcceptBtn", HongBaoGetViewCell::onClickAcceptBtn);
    return NULL;
}

void HongBaoGetViewCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(HongBaoGetViewCell::updateSelectInfo), "HongBaoGetViewCell_Select", NULL);
}

void HongBaoGetViewCell::onExit(){
//    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "HongBaoGetViewCell_Select");
    setTouchEnabled(false);
    CCNode::onExit();
}




