//
//  AllianceRankAttrView.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-6.
//
//

#include "AllianceRankAttrView.h"
#include "AllianceManager.h"
#include "IFAllianceRankCmd.hpp"
float hd_cell_height = 64.2;
float hd_cell_width = 1400;
float dw = (hd_cell_width - 661 - 8) / 5.0;

AllianceRankAttrView *AllianceRankAttrView::create(){
    AllianceRankAttrView *ret = new AllianceRankAttrView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AllianceRankAttrView::onEnter(){
    PopupBaseView::onEnter();
    this->setTitleName(_lang("115904"));
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

void AllianceRankAttrView::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALERT_ALLIANCE_VIEW_CLOSE);
    PopupBaseView::onExit();
}

bool AllianceRankAttrView::init(){
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    
    m_state = 1;
    auto it = AllianceManager::getInstance()->m_rankState.begin();
    
    while (it != AllianceManager::getInstance()->m_rankState.end()) {
        string key = it->first;
       int state = it->second;
        int index = 0;
        if (key == "0_2") {
            index = 0;
        }else if (key == "0_1"){
            index = 1;
        }else if (key == "1_2"){
            index = 2;
        }else if (key == "1_1"){
            index = 3;
        }else if (key == "2_2"){
            index = 4;
        }else if (key == "2_1"){
            index = 5;
        }
        m_rightState.insert(make_pair(index, make_pair(key, state)));
        
        it++;
    }
    auto node = CCBLoadFile("AllianceRankAttrView", this, this);
    this->setContentSize(node->getContentSize());
    int prevH = m_viewBg->getContentSize().height;
    changeBGHeight(m_viewBg);
    int changeH = m_viewBg->getContentSize().height;
    int add = changeH - prevH;
    if(GlobalData::shared()->playerInfo.allianceInfo.rank != 5){
        m_btn->setVisible(false);
    }
    int maxHeight = 1135;
    if (CCCommonUtils::isIosAndroidPad()) {
        maxHeight = 2580;
        add =  CCDirector::sharedDirector()->getWinSize().height - 2048;;
    }
    m_scrollBg->setContentSize(CCSize(m_scrollBg->getContentSize().width,MIN(m_scrollBg->getContentSize().height + add + 10, maxHeight)));
    m_scrollNode->setContentSize(CCSize(m_scrollNode->getContentSize().width,m_scrollNode->getContentSize().height+add));
    m_scrollView = CCScrollView::create(m_scrollNode->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_scrollView->setTouchPriority(Touch_Popup);
    
    int modelHeight = 735;
    int lbHeight = 400;
    if (CCCommonUtils::isIosAndroidPad()) {
        modelHeight = 1880;
        lbHeight = 700;
    }
    
    m_scrollNode->addChild(m_scrollView,10000);
    m_rankNode->removeFromParent();
    m_rankNode->setPosition(ccp(0, modelHeight+lbHeight-10));
    m_scrollView->addChild(m_rankNode);
//    m_scrollView->addChild(CCLayerColor::create(ccc4(0, 0, 0, 100), m_scrollNode->getContentSize().width, modelHeight+lbHeight));
    m_scrollView->setContentSize(CCSize(m_scrollNode->getContentSize().width,modelHeight+lbHeight));
    m_scrollView->setContentOffset(ccp(0, m_scrollNode->getContentSize().height - modelHeight - lbHeight));
    m_scrollView->setTouchEnabled(true);
    
    
    
    m_scrollView->setTouchEnabled(true);
    string tip = _lang("115927");
    tip += "\n";
    tip += _lang("115928");
    m_tip->setString(tip.c_str());
    changeBtnLb();
    m_txt0->setString(_lang("115910"));
    m_txt1->setString(_lang("115909"));
    m_txt2->setString(_lang("115908"));
    m_txt3->setString(_lang("115907"));
    m_txt4->setString(_lang("115913"));
    m_txt5->setString(_lang("115916"));
    m_txt6->setString(_lang("115915"));
    m_txt7->setString(_lang("115905"));
    m_txt8->setString(_lang("115906"));
    m_txt9->setString(_lang("115912"));
    m_txt10->setString(_lang("115911"));
    m_txt11->setString(_lang("115921"));
    m_txt12->setString(_lang("115920"));
    m_txt13->setString(_lang("115919"));
    m_txt14->setString(_lang("133061"));
    m_txt15->setString(_lang("115931"));
    m_txt16->setString(_lang("115918"));
    m_txt17->setString(_lang("115917"));
    m_txt18->setString(_lang("115914"));
    m_txt19->setString(_lang("115904"));
    m_txt20->setString(_lang("115903"));
    m_txt21->setString(_lang("115902"));
    m_txt22->setString(_lang("115901"));
    m_txt23->setString(_lang("115900"));
    m_txt24->setString(_lang("115834"));
    m_txt25->setString(_lang("115835"));
    for (int i=0; i<26; i++) {
        if(AllianceManager::getInstance()->v_rank5[i]==1){
            CCSprite* spr = CCLoadSprite::createSprite("icon_complete.png");
            spr->setPosition(ccp(323, 695.9 - i*34.9));
            spr->setScale(0.7);
            m_listContainer->addChild(spr);
        }
        
        if(AllianceManager::getInstance()->v_rank4[i]==1){
            CCSprite* spr = CCLoadSprite::createSprite("icon_complete.png");
            spr->setPosition(ccp(323 + 70 , 695.9 - i*34.9));
            spr->setScale(0.7);
            m_listContainer->addChild(spr);
        }
        if(AllianceManager::getInstance()->v_rank3[i]==1){
            CCSprite* spr = CCLoadSprite::createSprite("icon_complete.png");
            spr->setPosition(ccp(323 + 68*2, 695.9 - i*34.9));
            spr->setScale(0.7);
            m_listContainer->addChild(spr);
        }
        if(AllianceManager::getInstance()->v_rank2[i]==1){
            CCSprite* spr = CCLoadSprite::createSprite("icon_complete.png");
            spr->setPosition(ccp(323+ 68*3, 695.9 - i*34.9));
            spr->setScale(0.7);
            if (i < 23) {
               m_listContainer->addChild(spr);
            }
            
        }
        if(AllianceManager::getInstance()->v_rank1[i]==1){
            CCSprite* spr = CCLoadSprite::createSprite("icon_complete.png");
            spr->setPosition(ccp(323+ 68*4, 695.9 - i*34.9));
            spr->setScale(0.7);
            if (i < 23) {
                m_listContainer->addChild(spr);
            }
        }
    }
    initNodeState(m_mailR2Node);
    initNodeState(m_mailR1Node);
    initNodeState(m_memberR2Node);
    initNodeState(m_memberR1Node);
    initNodeState(m_onlineR1Node);
    initNodeState(m_onlineR2Node);
    return  true;
}

bool AllianceRankAttrView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankTxt5", CCLabelIF*, this->m_rankTxt5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankTxt4", CCLabelIF*, this->m_rankTxt4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankTxt3", CCLabelIF*, this->m_rankTxt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankTxt2", CCLabelIF*, this->m_rankTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankTxt1", CCLabelIF*, this->m_rankTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tip", CCLabelIF*, this->m_tip);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt0", CCLabelIF*, this->m_txt0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt1", CCLabelIF*, this->m_txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt2", CCLabelIF*, this->m_txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt3", CCLabelIF*, this->m_txt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt4", CCLabelIF*, this->m_txt4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt5", CCLabelIF*, this->m_txt5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt6", CCLabelIF*, this->m_txt6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt7", CCLabelIF*, this->m_txt7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt8", CCLabelIF*, this->m_txt8);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt9", CCLabelIF*, this->m_txt9);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt10", CCLabelIF*, this->m_txt10);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt11", CCLabelIF*, this->m_txt11);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt12", CCLabelIF*, this->m_txt12);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt13", CCLabelIF*, this->m_txt13);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt14", CCLabelIF*, this->m_txt14);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt15", CCLabelIF*, this->m_txt15);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt16", CCLabelIF*, this->m_txt16);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt17", CCLabelIF*, this->m_txt17);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt18", CCLabelIF*, this->m_txt18);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt19", CCLabelIF*, this->m_txt19);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt20", CCLabelIF*, this->m_txt20);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt21", CCLabelIF*, this->m_txt21);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt22", CCLabelIF*, this->m_txt22);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt23", CCLabelIF*, this->m_txt23);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt24", CCLabelIF*, this->m_txt24);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt25", CCLabelIF*, this->m_txt25);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankNode", CCNode*, this->m_rankNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scrollNode", CCNode*, this->m_scrollNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scrollBg", CCNode*, this->m_scrollBg);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailR1Node", CCNode*, this->m_mailR1Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailR2Node", CCNode*, this->m_mailR2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_memberR1Node", CCNode*, this->m_memberR1Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_memberR2Node", CCNode*, this->m_memberR2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_onlineR1Node", CCNode*, this->m_onlineR1Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_onlineR2Node", CCNode*, this->m_onlineR2Node);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mask1", CCNode*, this->m_mask1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mask2", CCNode*, this->m_mask2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn", CCControlButton*, this->m_btn);
    
    return false;
}

bool AllianceRankAttrView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return true;
}

void AllianceRankAttrView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (m_state == 0) {
        if (isTouchInside(m_mailR1Node, pTouch)) {
            changeRight(m_mailR1Node);
        }
        if (isTouchInside(m_mailR2Node, pTouch)) {
            changeRight(m_mailR2Node);
        }
        if (isTouchInside(m_memberR1Node, pTouch)) {
            changeRight(m_memberR1Node);
        }
        if (isTouchInside(m_memberR2Node, pTouch)) {
            changeRight(m_memberR2Node);
        }
        if (isTouchInside(m_onlineR1Node, pTouch)) {
            changeRight(m_onlineR1Node);
        }
        if (isTouchInside(m_onlineR2Node, pTouch)) {
            changeRight(m_onlineR2Node);
        }
    }
}
void AllianceRankAttrView::initNodeState(Node* parentNode){
    int tag = parentNode->getTag();
    if (m_rightState[tag].second == 1) {
        CCSprite* spr = CCLoadSprite::createSprite("icon_complete.png");
        spr->setPosition(Vec2(31, 15));
        spr->setScale(0.7);
        parentNode->addChild(spr);
    }
}
void AllianceRankAttrView::changeRight(Node* touchNode){
    int tag = touchNode->getTag();
    if (touchNode->getChildrenCount() > 0) {
        touchNode->removeAllChildren();
        m_rightState[tag].second = 0;
    }
    else{
        CCSprite* spr = CCLoadSprite::createSprite("icon_complete.png");
        spr->setPosition(Vec2(31, 15));
        spr->setScale(0.7);
        touchNode->addChild(spr);
        m_rightState[tag].second = 1;
    }
}
void AllianceRankAttrView::changeBtnLb(){
    if(m_state){
        CCCommonUtils::setButtonTitle(m_btn, _lang("115832").c_str());//修改权限
        m_mask1->setVisible(false);
        m_mask2->setVisible(false);
    }else{
        CCCommonUtils::setButtonTitle(m_btn, _lang("115035").c_str());
        m_mask1->setVisible(true);
        m_mask2->setVisible(true);
    }
}
SEL_CCControlHandler AllianceRankAttrView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtnClick", AllianceRankAttrView::onBtnClick);
    return NULL;
}
void AllianceRankAttrView::onBtnClick(CCObject* pSender,CCControlEvent event){
    if (!m_state) {
        string dataStr = "";
        auto it = m_rightState.begin();
        while (it != m_rightState.end()) {
            dataStr = dataStr + it->second.first + ":" + CC_ITOA(it->second.second) + ";";
            it++;
        }
        if (dataStr.empty()) {
            return;
        }
        IFAllianceRankCmd* cmd = new IFAllianceRankCmd(dataStr.substr(0,dataStr.length()-1));
        cmd->sendAndRelease();
    }
    m_state = !m_state;
    changeBtnLb();
}
