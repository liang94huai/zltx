//
//  HongBaoUseView.cpp
//  IF
//
//  Created by 张军 on 15/11/17.
//
//

#include "HongBaoUseView.h"
#include "UIComponent.h"
#include "YesNoDialog.h"
#include "ToolController.h"
#include "WorldController.h"
#include "ChatController.h"
#include "ChatServiceCocos2dx.h"
#include "ChatView.h"
#include "Utf8Utils.h"
#include "HongBaoGetView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"

#define msgLength 16

HongBaoUseView* HongBaoUseView::create(ToolInfo& info){
    HongBaoUseView* ret = new HongBaoUseView();
    if(ret && ret->init(info)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool HongBaoUseView::init(ToolInfo& info)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    m_info = info;
    CCLoadSprite::doResourceByCommonIndex(514, true);
    CCLoadSprite::doResourceByCommonIndex(513, true);
    CCLoadSprite::doResourceByCommonIndex(6, true);
    CCLoadSprite::doResourceByCommonIndex(205, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(514, false);
        CCLoadSprite::doResourceByCommonIndex(513, false);
        CCLoadSprite::doResourceByCommonIndex(6, false);
        CCLoadSprite::doResourceByCommonIndex(205, false);
    });
    auto tmpCCB = CCBLoadFile("HongBaoUseView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    if (!CCCommonUtils::isIosAndroidPad()) {
        int addHeight = CCDirector::sharedDirector()->getWinSize().height - 852;
        m_bottomNode->setPositionY(m_bottomNode->getPositionY()-addHeight);
        m_infoList->setContentSize(CCSizeMake(m_infoList->getContentSize().width, m_infoList->getContentSize().height+addHeight));
    }
    
    auto spriteText1 = CCLoadSprite::createScale9Sprite("common514_hb_inputBg.png");
    m_editBox1 = CCEditBox::create(CCSizeMake(470, 56), spriteText1);
    m_editBox1->setInputMode(kEditBoxInputModeNumeric);
    m_editBox1->setMaxLength(100);
    m_editBox1->setFontSize(24);
    m_editBox1->setFontColor(ccBLACK);
    m_editBox1->setReturnType(kKeyboardReturnTypeDone);
    m_editBox1->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
    m_editBox1->setText("");
    m_editBox1->setPlaceHolder("");
    m_editBox1->setDelegate(this);
    m_editBox1->setTag(1001);
    m_editNode1->addChild(m_editBox1);
    
    auto spriteText2 = CCLoadSprite::createScale9Sprite("common514_hb_inputBg.png");
    m_editBox2 = CCEditBox::create(CCSizeMake(470, 56), spriteText2);
    m_editBox2->setInputMode(kEditBoxInputModeNumeric);
    m_editBox2->setMaxLength(100);
    m_editBox2->setFontSize(24);
    m_editBox2->setFontColor(ccBLACK);
    m_editBox2->setReturnType(kKeyboardReturnTypeDone);
    m_editBox2->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
    m_editBox2->setText("");
    m_editBox2->setPlaceHolder("");
    m_editBox2->setDelegate(this);
    m_editBox2->setTag(1002);
    m_editNode2->addChild(m_editBox2);
    
    auto spriteText3 = CCLoadSprite::createScale9Sprite("common514_hb_inputBg.png");
    m_editBox3 = CCEditBox::create(CCSizeMake(470, 56), spriteText3);
    m_editBox3->setMaxLength(msgLength*3);
    m_editBox3->setFontName("Helvetica");
    m_editBox3->setFontSize(24);
    m_editBox3->setFontColor(ccBLACK);
    m_editBox3->setReturnType(kKeyboardReturnTypeDone);
    m_editBox3->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
    m_editBox3->setText("");
    m_editBox3->setPlaceHolder("");
    m_editBox3->setDelegate(this);
    m_editBox3->setTag(1003);
    m_editNode3->addChild(m_editBox3);
    
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Popup);
    m_tabView->setDelegate(this);
    m_tabView->setTag(11);
    m_infoList->addChild(m_tabView);
    
    max_gold="0";
    max_red="0";
    min_red="0";
    max_hour="0";
    vector<string> vec;
    string s = m_info.para1;
    CCCommonUtils::splitString(s, ";", vec);
    if (vec.size()==3) {
        max_gold=vec[0];
        max_red=vec[2];
        min_red=vec[1];
    }
    max_hour = m_info.para2;
    m_numTipTxt->setString(_lang("104968"));//红包个数
    m_goldTipTxt->setString(_lang("104980"));//金币数目
    m_numInputTipTxt->setString(_lang_2("104990", min_red.c_str(),max_red.c_str()));//可发放{0}至{1}个
    m_numInputTipTxt->setColor({103,74,40});
    m_goldInputTipTxt->setString(_lang_1("104989",max_gold.c_str()));//最多放入{0}金币
    m_goldInputTipTxt->setColor({103,74,40});
    m_downTxt->setString(_lang("104969"));//每个红包的金币数目随机
    m_channel=-1;
    m_downBtn->setEnabled(false);
    CCCommonUtils::setButtonTitle(m_downBtn, _lang("104970").c_str());//		发放
    
    m_channelTxt->setString(_lang("160253"));//发送至
    m_msgTipTxt->setString(_lang("160252"));//留言
    string msgStr = _lang_1("104973", _lang("104983").c_str());//104973=各位朋友，快来抢{0}啦！104983=红包
    m_msgInputTipTxt->setString(msgStr);
    m_msgInputTipTxt->setColor({103,74,40});
    
    m_tipBtn->setVisible(true);
    return true;
}

void HongBaoUseView::onEnter(){
    CCNode::onEnter();
    UIComponent::getInstance()->showPopupView(1);
    m_titleTxt->setString(_lang_1("104966",_lang("104983").c_str()));//发放{0} 红包
    
    this->setTouchEnabled(true);
//    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(HongBaoUseView::updateInfo), OnGetDailyQuests, NULL);

}

void HongBaoUseView::onExit(){
//    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, OnGetDailyQuests);
    this->setTouchEnabled(false);
    CCNode::onExit();
}

bool HongBaoUseView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}
void HongBaoUseView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    
}
#pragma mark editBox
void HongBaoUseView::editBoxReturn(EditBox* editBox){
    string getTxt1 = m_editBox1->getText();
    string getTxt2 = m_editBox2->getText();
    string getTxt3 = m_editBox3->getText();
    if (getTxt1=="") {
        m_goldInputTipTxt->setVisible(true);
    }
    if (getTxt2==""){
        m_numInputTipTxt->setVisible(true);
    }
    if (getTxt3==""){
        m_msgInputTipTxt->setVisible(true);
    }
    
    int goldNum = atoi(getTxt1.c_str());
    int redNum = atoi(getTxt2.c_str());
    int editTag = editBox->getTag();
    if (editTag==1001 && getTxt1!="") {
        if(goldNum>GlobalData::shared()->playerInfo.gold){//判断金币是否够
            YesNoDialog::show(_lang("E100001").c_str(),NULL);//E100001=金币不足
            m_editBox1->setText("");
            m_goldInputTipTxt->setVisible(true);
            return;
        }
        if(goldNum>atoi(max_gold.c_str())){
            YesNoDialog::show(_lang_1("104989",max_gold.c_str()), NULL);//最多放入{0}金币
            m_editBox1->setText("");
            m_goldInputTipTxt->setVisible(true);
            return;
        }
        if (getTxt2!="" && goldNum<redNum) {
            YesNoDialog::show(_lang_1("104972", CC_ITOA(redNum)), NULL);//领主大人，您放入红包的金币，不足以发放{0}个红包
            m_editBox1->setText("");
            m_goldInputTipTxt->setVisible(true);
            return;
        }
    }else if (editTag==1002 && getTxt2!=""){
        if(redNum>atoi(max_red.c_str()) || redNum<atoi(min_red.c_str())){
            YesNoDialog::show(_lang_2("104990", min_red.c_str(),max_red.c_str()), NULL);//可发放{0}至{1}个
            m_editBox2->setText("");
            m_numInputTipTxt->setVisible(true);
            return;
        }
        if (getTxt1!="" && goldNum<redNum) {
            YesNoDialog::show(_lang_1("104972", CC_ITOA(redNum)), NULL);//领主大人，您放入红包的金币，不足以发放{0}个红包
            m_editBox2->setText("");
            m_numInputTipTxt->setVisible(true);
            return;
        }
    }else if (editTag==1003 && getTxt3.length()>msgLength*3){
        getTxt3 = getTxt3.substr(0,msgLength*3);
        YesNoDialog::show(_lang_1("150382", CC_ITOA(redNum)), NULL);//留言过长，将会自动截取文本
        m_editBox3->setText(getTxt3.c_str());
    }

    if (getTxt1!="" && getTxt2!="" && m_channel!=-1) {
        m_downBtn->setEnabled(true);
    }else{
        m_downBtn->setEnabled(false);
    }
}

void HongBaoUseView::editBoxEditingDidBegin(EditBox* editBox){
    int editTag = editBox->getTag();
    if (editTag==1001) {
        m_goldInputTipTxt->setVisible(false);
    }else if (editTag==1002){
        m_numInputTipTxt->setVisible(false);
    }else if (editTag==1003){
        m_msgInputTipTxt->setVisible(false);
    }
}

#pragma mark tableView
//CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
CCSize HongBaoUseView::cellSizeForTable(CCTableView *table){
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSizeMake(1488, 312);
    }
    return CCSizeMake(620, 130);
}

CCTableViewCell* HongBaoUseView::tableCellAtIndex(CCTableView *table, ssize_t idx){
    HongBaoUseViewCell* cell = (HongBaoUseViewCell*)table->dequeueCell();
    if (cell) {
        cell->setData(idx);
    } else {
        cell = HongBaoUseViewCell::create(idx);
    }
    return cell;
}

ssize_t HongBaoUseView::numberOfCellsInTableView(CCTableView *table){
    if (GlobalData::shared()->playerInfo.isInAlliance()) {
        return 2;
    }
    return 1;
}

void HongBaoUseView::tableCellTouched(CCTableView* table, CCTableViewCell* cell){
    HongBaoUseViewCell* HBcell = (HongBaoUseViewCell*)cell;
    m_channel = HBcell->cellIndex;
    string getTxt1 = m_editBox1->getText();
    string getTxt2 = m_editBox2->getText();
    if (getTxt1!="" && getTxt2!="" && m_channel!=-1) {
        m_downBtn->setEnabled(true);
    }else{
        m_downBtn->setEnabled(false);
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("HongBaoUseViewCell_Select",CCInteger::create(m_channel));
}

#pragma mark ccb
SEL_CCControlHandler HongBaoUseView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDownBtnClick", HongBaoUseView::onDownBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipBtnClick", HongBaoUseView::onTipBtnClick);
    return NULL;
}
bool HongBaoUseView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_goldTipTxt", CCLabelIF*, this->m_goldTipTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numTipTxt", CCLabelIF*, this->m_numTipTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_goldInputTipTxt", CCLabelIF*, this->m_goldInputTipTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numInputTipTxt", CCLabelIF*, this->m_numInputTipTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_downTxt", CCLabelIF*, this->m_downTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_editNode1", CCNode*, this->m_editNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_editNode2", CCNode*, this->m_editNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_downBtn", CCControlButton*, this->m_downBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipBtn", CCControlButton*, this->m_tipBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_msgTipTxt", CCLabelIF*, this->m_msgTipTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_editNode3", CCNode*, this->m_editNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_msgInputTipTxt", CCLabelIF*, this->m_msgInputTipTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_channelTxt", CCLabelIF*, this->m_channelTxt);
    
    return false;
}

void HongBaoUseView::onDownBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    m_goldNum = atoi(m_editBox1->getText());
    m_redNum = atoi(m_editBox2->getText());
    
    if(m_redNum>atoi(max_red.c_str()) || m_redNum<atoi(min_red.c_str())){
        YesNoDialog::show(_lang_2("104990", min_red.c_str(),max_red.c_str()), NULL);//可发放{0}至{1}个
        return;
    }
    if(m_goldNum>atoi(max_gold.c_str())){
        YesNoDialog::show(_lang_1("104989",max_gold.c_str()), NULL);//最多放入{0}金币
        return;
    }
    
    if (m_goldNum<m_redNum) {
        YesNoDialog::show(_lang_1("104972", CC_ITOA(m_redNum)), NULL);//领主大人，您放入红包的金币，不足以发放{0}个红包
        return;
    }
    string showStr = _lang_2("104971", CC_ITOA(m_goldNum), max_hour.c_str());//104971		领主大人，将发放总共{0}金币的红包，{1}小时后未被领取的红包将以邮件的形式返还给您
    YesNoDialog::show(showStr.c_str(), CCCallFunc::create(this, callfunc_selector(HongBaoUseView::sendRedPacket)));//领主大人，您放入红包的金币，不足以发放{0}个红包
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
bool correctUtfBytesBao(const char* bytes, int & index) {
    index = 0;
    while (*bytes != '\0') {
        unsigned char utf8 = *(bytes++);
        ++index;
        // Switch on the high four bits.
        switch (utf8 >> 4) {
            case 0x00:
            case 0x01:
            case 0x02:
            case 0x03:
            case 0x04:
            case 0x05:
            case 0x06:
            case 0x07:
                // Bit pattern 0xxx. No need for any extra bytes.
                break;
            case 0x08:
            case 0x09:
            case 0x0a:
            case 0x0b:
            case 0x0f:
                /*
                 * Bit pattern 10xx or 1111, which are illegal start bytes.
                 * Note: 1111 is valid for normal UTF-8, but not the
                 * modified UTF-8 used here.
                 */
                // *(bytes-1) = '?';
                return false;
                break;
            case 0x0e:
                // Bit pattern 1110, so there are two additional bytes.
                utf8 = *(bytes++);
                if ((utf8 & 0xc0) != 0x80) {
                    // *(bytes-1) = 0;
                    //--bytes;
                    //*(bytes-2) = 0;
                    return false;
                    break;
                }
                
                // Fall through to take care of the final byte.
            case 0x0c:
            case 0x0d:
                // Bit pattern 110x, so there is one additional byte.
                utf8 = *(bytes++);
                if ((utf8 & 0xc0) != 0x80) {
                    
                    //--bytes;
                    //if(three)--bytes;
                    // *(bytes-2) = 0;
                    // *(bytes-1)=0;
                    return false;
                }
                break;
        }
    }
    return true;
}
#endif

#pragma mark 发红包

void HongBaoUseView::sendRedPacket(){
    string msgStr = m_editBox3->getText();
    if (msgStr=="") {
        msgStr = _lang_1("104973", _lang("104983").c_str());//104973=各位朋友，快来抢{0}啦！104983=红包
    }
 //   int length= msgStr.length();
 //   if (length>msgLength*3) {
//        msgStr = msgStr.substr(0,msgLength*3);
 //   }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    int errIndex = 0;
    if( !correctUtfBytesBao( msgStr.c_str(), errIndex ) )
    {
        CCLOG("sendRedPacket zym error:%s,%d",msgStr.c_str(), errIndex);
        msgStr = "";
    }
#endif
    ToolController::getInstance()->stratSendRedPacket(m_channel, m_goldNum, m_redNum, m_info.uuid,msgStr);//0世界1联盟
    PopupViewController::getInstance()->goBackPopupView(this);
    
    int chatType = CHAT_COUNTRY;
    if(m_channel==1)
    {
        chatType = CHAT_ALLIANCE;
    }
    //打开聊天窗口
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
    CCLOG("GlobalData::shared()->android_native_chat = %d", GlobalData::shared()->android_native_chat);
    if(GlobalData::shared()->android_native_chat == 1){
        if(!ChatServiceCocos2dx::isChatShowing){
            CCLOG("onChatBtnClick 1 showChatActivityFrom2dx");
            if (chatType==CHAT_COUNTRY) {
                ChatServiceCocos2dx::m_channelType=CHANNEL_TYPE_COUNTRY;
            }else if(chatType==CHAT_ALLIANCE) {
                ChatServiceCocos2dx::m_channelType=CHANNEL_TYPE_ALLIANCE;
            }
            //            ChatServiceCocos2dx::m_isInMailDialog=false;
            ChatServiceCocos2dx::m_isNoticItemUsed=false;
            ChatServiceCocos2dx::showChatActivityFrom2dx();
        }
    }else{
        PopupViewController::getInstance()->addPopupInView(ChatView::create(chatType),false);
    }
#elif(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    if(ChatServiceCocos2dx::enableNativeChat){
        if (!ChatServiceCocos2dx::isChatShowing_fun()){
            //聊天入口
            CCLOG("onChatBtnClick 1 showChatIOSFrom2dx");
            if (chatType==CHAT_COUNTRY) {
                ChatServiceCocos2dx::m_channelType=CHANNEL_TYPE_COUNTRY;
            }else if(chatType==CHAT_ALLIANCE) {
                ChatServiceCocos2dx::m_channelType=CHANNEL_TYPE_ALLIANCE;
            }
            ChatServiceCocos2dx::m_isInMailDialog=false;
            ChatServiceCocos2dx::showChatIOSFrom2dx();
        }
        
    }else{
        PopupViewController::getInstance()->addPopupInView(ChatView::create(chatType),false);
    }
    
#endif
}

void HongBaoUseView::onTipBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent){
//    ToolController::getInstance()->stratGetRedPacket("70aec53f3f364bbab82495af0d3af03b");//
//    PopupViewController::getInstance()->addPopupInView(HongBaoGetView::create("94c8417d78834080932fe765e2814419","1"));
    CCCommonUtils::showHelpShiftFAQ("3960");
}

#pragma mark HongBaoUseViewCell
HongBaoUseViewCell* HongBaoUseViewCell::create(int idx){
    auto ret = new HongBaoUseViewCell();
    if (ret && ret->init(idx)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool HongBaoUseViewCell::init(int idx){
    CCBLoadFile("HongBaoUseViewCell", this, this);
    setData(idx);
    return true;
}

void HongBaoUseViewCell::setData(int idx){
    cellIndex = idx;
    m_selectSpr->setVisible(false);
    m_selectSprBg->setVisible(true);
    if (cellIndex==0) {//王国
        
//        int currentServerId = GlobalData::shared()->playerInfo.currentServerId;
//        string serverName = GlobalData::shared()->playerInfo.serverName;
//        string serverStr = serverName + " #" + CC_ITOA(currentServerId);
//        m_upTxt->setString(serverStr);//{0} #564
        m_nameTxt->setString(_lang("104992"));//国家聊天
        //icon
        CCSprite* alIconSpr = CCLoadSprite::createSprite("common514_hb_wangguan.png");
        CCCommonUtils::setSpriteMaxSize(alIconSpr,94);
        m_headIconNode->addChild(alIconSpr);
        m_iconCover->setVisible(false);
    }else if (cellIndex==1 && GlobalData::shared()->playerInfo.isInAlliance()){//联盟
//        string alliancStr = "("+GlobalData::shared()->playerInfo.allianceInfo.shortName+") "+GlobalData::shared()->playerInfo.allianceInfo.name;
//        m_upTxt->setString(alliancStr);//（联盟简称）联盟名字
        m_nameTxt->setString(_lang("105605"));//联盟聊天
        
        string alIconStr = GlobalData::shared()->playerInfo.allianceInfo.getAllianceIcon();
        CCSprite* alIconSpr = CCLoadSprite::createSprite(alIconStr.c_str());
        CCCommonUtils::setSpriteMaxSize(alIconSpr,104);
//        alIconSpr->setPosition(CCPoint(startX+50, py));
        m_headIconNode->addChild(alIconSpr);
        m_iconCover->setVisible(false);
    }
}

bool HongBaoUseViewCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return false;
}

void HongBaoUseViewCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}

void HongBaoUseViewCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
}

bool HongBaoUseViewCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectSpr", CCSprite*, m_selectSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectSprBg", CCSprite*, m_selectSprBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headIconNode", CCNode*, m_headIconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upTxt", CCLabelIF*, m_upTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconCover", CCSprite*, m_iconCover);

    return false;
}

SEL_CCControlHandler HongBaoUseViewCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickAcceptBtn", HongBaoUseViewCell::onClickAcceptBtn);
    return NULL;
}

void HongBaoUseViewCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(HongBaoUseViewCell::updateSelectInfo), "HongBaoUseViewCell_Select", NULL);
}

void HongBaoUseViewCell::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "HongBaoUseViewCell_Select");
    setTouchEnabled(false);
    CCNode::onExit();
}

void HongBaoUseViewCell::updateSelectInfo(CCObject *obj){
    int selectIdx = dynamic_cast<CCInteger*>(obj)->getValue();
    m_selectSpr->setVisible(selectIdx == cellIndex);
    m_selectSprBg->setVisible(selectIdx != cellIndex);

}
