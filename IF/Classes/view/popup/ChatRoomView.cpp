//
//  ChatRoomView.cpp
//  IF
//
//
//

#include "ChatRoomView.h"
#include "SceneController.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "ChatController.h"
#include "ChatRoomCell.h"
#include "YesNoDialog.h"
#include "ChatLockCommand.h"
#include "ChatUnLockCommand.h"
#include "MailWritePopUpView.h"
#include "GetUserInfoCommand.h"
#include "../../Ext/CCDevice.h"
#include "RoleInfoView.h"
#include "UIComponent.h"
#include "InvitesAllianceCommand.h"
#include "InputFieldMultiLine.h"
#include "JoinAllianceView.h"
#include "AlertAddAllianceView.h"
#include "CheckAllianceInfoView.h"
#include "MailController.h"
#include "ChatServiceCocos2dx.h"
#include "GetAllianceCommentsCommand.h"
#include "AllianceCommentShieldAdd.h"
#include "WorldController.h"
#include "AllianceSendCommentCommand.h"
#include "SceneContainer.h"
#include "ToolController.h"

#define CR_SHOW_COVER_NATIVE_INPUT          0

const float numPerRow = 1.0;
ChatRoomView* ChatRoomView::create(string roomid){
    ChatRoomView* ret = new ChatRoomView();
    if(ret && ret->init(roomid)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
	}
	
    return ret;
}

bool ChatRoomView::init(string roomid)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    this->m_roomID = roomid;
    setIsHDPanel(true);
    auto tmpCCB = CCBLoadFile("ChatRoomView",this,this);
    int oldBgHeight = m_buildBG->getContentSize().height;
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    tmpCCB->setContentSize(winsize);
    if (CCCommonUtils::isIosAndroidPad()) {
        winsize = CCSize(640, 852);
    }
    m_buildBG->setContentSize(CCSize(winsize.width+2,winsize.height));
    m_bg->setContentSize(CCSize(winsize.width+2,winsize.height));
    if (CCCommonUtils::isIosAndroidPad()) {
        winsize = CCDirector::sharedDirector()->getWinSize();
    }
    setContentSize(winsize);
    
    int newBgHeight = m_buildBG->getContentSize().height;
    int addHeight = newBgHeight - oldBgHeight;
    int oldWidth = m_infoList->getContentSize().width;
    int oldHeight = m_infoList->getContentSize().height;
    m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
    
    CCCommonUtils::setButtonTitle(m_sendBtn, _lang("105302").c_str());
    m_editBox = InputFieldMultiLine::create(CCSizeMake(460,60),"UI_Alliance_TextBox.png",24);
    m_editBox->setPosition(ccp(0,0));
    m_editBox->setMaxChars(500);
    m_editBox->setAnchorPoint(ccp(0,0));
	m_editBox->setShowLineNum(6);
	m_editBox->setLineNumber(2);
    m_editBox->setFontColor(ccBLACK);

    m_editBox->setSwallowsTouches(true);
    m_editBox->setMoveFlag(true);
    m_editBox->setTouchPriority(1);
    m_editNode->addChild(m_editBox);

    m_sendBtn->setTouchPriority(0);
    m_loadingMoreText->setString(_lang("105502"));
    m_loadingMoreText->setVisible(false);
    m_loadingMoreText->setPositionY(m_loadingMoreText->getPositionY()+addHeight);
    m_loadingMoreStartY = m_loadingMoreText->getPositionY();
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillBottomUp);
    m_tabView->setDelegate(this);
    m_tabView->setTouchPriority(Touch_Default);
    m_infoList->addChild(m_tabView);
    updateChatRoom(NULL);
    
    
    m_editBox->setText(ChatController::getInstance()->m_tmpSaveMsg.c_str());
    if(m_editBox->getLineNumber()>2){
        int changeH = m_editBox->getContentSize().height - 60;
        onAddlineEvent(CCInteger::create(changeH));
    }
    return true;
}


void ChatRoomView::onEnter(){
    CCNode::onEnter();
    setTitleName(_lang("105354"));
    MailController::getInstance()->setCurChatRoom(m_roomID);
    setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChatRoomView::onAddlineEvent), MSG_INPUT_ADD_LINE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChatRoomView::onEditTextEvent), MSG_INPUT_TEXT_EVENT, NULL);
	CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChatRoomView::onInputFieldCloseEvent), INPUTFIELD_CLOSE_EVENT, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChatRoomView::updateChatRoom), CHAT_ROOM_MSG_CHANGE, NULL);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ChatRoomView::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    this->unschedule(schedule_selector(ChatRoomView::reSetRequest));
}

void ChatRoomView::onExit(){
    MailController::getInstance()->setCurChatRoom("");
    GameController::getInstance()->removeWaitInterface();
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_INPUT_ADD_LINE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_INPUT_TEXT_EVENT);
	CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, INPUTFIELD_CLOSE_EVENT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, CHAT_ROOM_MSG_CHANGE);
    auto _currScene = CCDirector::sharedDirector()->getRunningScene();
    if (_currScene->getPositionY() != 0) {
        _currScene->setPositionY(0);
    }
    CCNode::onExit();
}
void ChatRoomView::onAddlineEvent(CCObject * params)
{
    int changeHeight = dynamic_cast<CCInteger*>(params)->getValue();
    m_tabView->setPositionY(m_tabView->getPositionY()+changeHeight);
    m_tabView->setViewSize(CCSizeMake(m_tabView->getViewSize().width, m_tabView->getViewSize().height-changeHeight));
    m_editBGNode->setPositionY(m_editBGNode->getPositionY()+changeHeight);
}
void ChatRoomView::onInputFieldCloseEvent(cocos2d::CCObject *params)
{
//    m_editBGNode->setPositionY(m_oldEditPositionY);
}
void ChatRoomView::onEditTextEvent(cocos2d::CCObject *params)
{
    string num = CC_ITOA( dynamic_cast<CCInteger*>(params)->getValue());
    m_inputNum->setString(num + "/500");
}
bool ChatRoomView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isTouchInside(m_editBox, pTouch)) {
        CCLOG("isTouchInside");
    }
    m_tabView->setBounceable(true);
    CCLOG("isTouchInside false");
    return true;
}
void ChatRoomView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    auto _currScene = CCDirector::sharedDirector()->getRunningScene();
    if (_currScene->getPositionY() != 0) {
        onInputFieldCloseEvent(NULL);
        _currScene->setPositionY(0);
    }
}


SEL_CCControlHandler ChatRoomView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPostBtnClick", ChatRoomView::onPostBtnClick);
    return NULL;
}

bool ChatRoomView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sendBtn", CCControlButton*, this->m_sendBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editNode", CCNode*, this->m_editNode);

    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editBg", CCScale9Sprite*, this->m_editBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editBGNode", CCNode*, this->m_editBGNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inputNum", CCLabelIF*, this->m_inputNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchEditNode", CCNode*, m_touchEditNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_loadingMoreText", CCLabelIF*, m_loadingMoreText);
    return false;
}
void ChatRoomView::onPostBtnClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    if (m_delayTime<ChatController::getInstance()->chat_interval)
    {
        CCCommonUtils::flyText(_lang("105307"));
        return;
    }
    m_delayTime=0;
    
    string msg = m_editBox->getText();
    string localTime = CC_ITOA(GlobalData::shared()->getWorldTime());
    auto iter = GlobalData::shared()->mailList.find(m_roomID);
    if(iter!=GlobalData::shared()->mailList.end()){
        if((*iter).second){
            MailController::getInstance()->addChatRoomSelfMsg(msg, (*iter).second->crGroupId,localTime);
            MailController::getInstance()->sendChatRoomMsg(msg, (*iter).second->crGroupId,localTime);
        }
    }
    m_editBox->setText("");
    m_editBox->closeIME();
    m_editBox->openIME();
}
static bool sortChatByTime(MailDialogInfo* info1, MailDialogInfo* info2){
    if(info1->createTime<info2->createTime){
        return false;
    }
    return true;
}
void ChatRoomView::updateChatRoom(CCObject* obj){
    mDialogList.clear();
    m_seqID = INT_MAX;
    auto iter = GlobalData::shared()->mailList.find(m_roomID);
    if(iter!=GlobalData::shared()->mailList.end()){
        if((*iter).second){
            CCObject *ccObj = NULL;
            CCARRAY_FOREACH((*iter).second->dialogs, ccObj){
                MailDialogInfo* dInfo = dynamic_cast<MailDialogInfo*>(ccObj);
                if(dInfo){
                    dInfo->status = 1;
                    mDialogList.push_back(dInfo);
                    if(dInfo->sequenceId < m_seqID){
                        m_seqID = dInfo->sequenceId;
                    }
                }
            }
        }
    }
    Vec2 offset = m_tabView->getContentOffset();
    sort(mDialogList.begin(), mDialogList.end(), sortChatByTime);
    m_cellNum = ceil(mDialogList.size() / numPerRow);
    if(m_cellNum>0){
        m_tabView->reloadData();
    }else{
        
    }
    if(obj == NULL){
        CCSize containerSize = m_tabView->getContainer()->getContentSize();
        CCSize viewSize = m_infoList->getContentSize();
        if(containerSize.height < viewSize.height){
            m_tabView->setContentOffset(CCSize(0, viewSize.height - containerSize.height));
        }else{
            m_tabView->setContentOffset(CCSize(0, 0));
        }
    }else{
        CCSize containerSize = m_tabView->getContainer()->getContentSize();
        CCSize viewSize = m_infoList->getContentSize();
        if(viewSize.height<containerSize.height){
            float minOffsetY = viewSize.height - containerSize.height;
            if( offset.y < minOffsetY){
                offset.y = minOffsetY;
            }
            if(offset.y>0){
                offset.y = 0;
            }
            m_tabView->setContentOffset(offset);
        }else{
            m_tabView->setContentOffset(CCSize(0, viewSize.height - containerSize.height));
        }
    }
}
void ChatRoomView::onEnterFrame(float dt){
    m_delayTime++;
    return;
}
void ChatRoomView::scrollViewDidScroll(CCScrollView* view){
    float curY = m_tabView->getContentOffset().y;
    float minY = m_tabView->minContainerOffset().y;
    if ((minY-curY) >30) {
        float dy = minY-curY;
        m_loadingMoreText->setPositionY(m_loadingMoreStartY-(dy-30));
        if (_lastPosY > curY) {//向下滑动
            if(_canRequest && m_seqID>1 && m_seqID<INT_MAX){
                _canRequest = false;
                m_loadingMoreText->setVisible(true);
                scheduleOnce(schedule_selector(ChatRoomView::reSetRequest), 5.0f);
                MailController::getInstance()->getChatRoomRecord(m_seqID, m_roomID);
            }else{
                m_loadingMoreText->setVisible(false);
            }
        }else{
            m_loadingMoreText->setVisible(false);
        }
    }else{
        m_loadingMoreText->setVisible(false);
    }
    _lastPosY = curY;
}

void ChatRoomView::reSetRequest(float _time){
    _canRequest = true;
}

void ChatRoomView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    ChatRoomCell* tmpCell = dynamic_cast<ChatRoomCell*>(cell);
    if (tmpCell) {
    }
}
CCSize ChatRoomView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx>=m_cellNum)
        return CCSizeZero;
    
    int _tmpHeight = 0;
    MailDialogInfo* info = dynamic_cast<MailDialogInfo*>(mDialogList.at(idx));
    if(info->tipFlag == 1){
         _tmpHeight = getTextDimension(info->contents.c_str()).height;
        _tmpHeight+=10;
    }else{
        if(!info->translationMsg.empty()){
            _tmpHeight = MAX(getTextDimension(info->contents.c_str()).height,getTextDimension(info->translationMsg.c_str()).height);
        }else{
            _tmpHeight = getTextDimension(info->contents.c_str()).height;
        }
        _tmpHeight+=60;
        if(_tmpHeight<130){
            _tmpHeight = 130;
        }
    }
    return CCSize(640,_tmpHeight);
    
}
CCSize ChatRoomView::getTextDimension(const char *text)
{
    int len = (int)strlen(text);
    if ((len==0)  ||  (len==1 && text[0] == '\n')) {
        CCLabelIFTTF *label = CCLabelIFTTF::create( "sample" , "Helvetica", 20 );
        label->setDimensions(CCSizeMake(450, 0));
        CCSize size = label->getContentSize();
        size.width = 0;
        return size;
    }
    CCLabelIFTTF *label = CCLabelIFTTF::create( text, "Helvetica", 20);
    label->setDimensions(CCSizeMake(450, 0));
    return label->getContentSize();
}

cocos2d::CCSize ChatRoomView::cellSizeForTable(CCTableView *table)
{
    return CCSize(640, 130);
}

CCTableViewCell* ChatRoomView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if (idx >= m_cellNum) {
        return NULL;
    }
    ChatRoomCell* cell = (ChatRoomCell*)table->dequeueCell();
    if(cell){
        cell->setData(idx,mDialogList.at(idx),m_infoList);
    }else{
        cell = ChatRoomCell::create(idx,mDialogList.at(idx),m_infoList);
    }
    return cell;
}

ssize_t ChatRoomView::numberOfCellsInTableView(CCTableView *table){
    return m_cellNum;
}
