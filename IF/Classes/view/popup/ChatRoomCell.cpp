//
//  ChatRoomCell.cpp
//  IF
//
//
//

#include "ChatRoomCell.h"
#include "PopupViewController.h"
#include "LocalController.h"
#include "CCCommonUtils.h"
#include "CCLabelIF.h"
#include "ChatController.h"
#include "YesNoDialog.h"
#include "ChatLockCommand.h"
#include "ChatView.h"
#include "TranslateCommand.h"
#include "ChatIsInAllanceCommand.h"
#include "ApplyAllianceCommand.h"
#include "AllianceFlagPar.h"
#include "BattleReportMailPopUpView.h"
#include "MailController.h"
#include "MailReadCommand.h"
#include "DetectMailPopUpView.h"
#include "WorldController.h"

ChatRoomCell* ChatRoomCell::create(int idx, MailDialogInfo* dInfo,CCNode* touchNode){
    ChatRoomCell* ret = new ChatRoomCell();
    if(ret && ret->init(idx, dInfo,touchNode)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ChatRoomCell::init(int idx, MailDialogInfo* dInfo,CCNode* touchNode)
{
    bool ret = true;
    CCBLoadFile("ChatRoomCell",this,this);
    this->setContentSize(CCSizeMake(640, 130));
//    m_touchNode = touchNode;
    m_bgSize = m_chatBG->getContentSize();
    m_timeNodeY = m_timeNode->getPositionY();
    m_nameLabelPt = m_nameLabel->getPosition();
    m_vipLabelPt = m_vipLabel->getPosition();
    m_translatePt = m_translateNode->getPosition();
//    m_touchNode3Y = m_touchNode3->getPositionY();
    m_msgLabelY = m_msgLabel->getPositionY();
    m_msgDimensions = m_msgLabel->getDimensions();
    m_vipLabel->setFntFile("pve_fnt_title.fnt");
    m_isOriginal = false;
    m_vipLabel->setScale(0.7);
	m_translateBtn->setTouchPriority(Touch_Popup_Item);
	m_isCheckingInAllance = false;
	m_prevPopupCount = -1;
    m_headImgNode = HFHeadImgNode::create();
    setData(idx,dInfo,touchNode);

    return ret;
}

void ChatRoomCell::setData(int idx, MailDialogInfo* dInfo,CCNode* touchNode)
{
    m_idx = idx;
    m_picNode->removeAllChildren();
    m_chatBG->setContentSize(m_bgSize);
    m_sysBG->setContentSize(m_bgSize);
    m_touchNode1->setContentSize(m_bgSize);
    m_timeNode->setPositionY(m_timeNodeY);
    m_nameLabel->setPosition(m_nameLabelPt);
    m_vipLabel->setPosition(m_vipLabelPt);
    m_translateNode->setPosition(m_translatePt);
//    m_touchNode3->setPositionY(m_touchNode3Y);
    m_msgLabel->setPositionY(m_msgLabelY);
    m_msgLabel->setDimensions(m_msgDimensions);
    m_timeLabel->setString("");
    m_vipLabel->setString("");
//    int line = 0;
    float height = 0;
    mDialogInfo = dInfo;
    if(!mDialogInfo){
        m_userNode->setVisible(false);
        m_msgNode->setVisible(false);
        m_timeNode->setVisible(false);
        m_sysMsgNode->setVisible(false);
        return;
    }
    if(mDialogInfo->tipFlag==1){
        m_userNode->setVisible(false);
        m_msgNode->setVisible(false);
        m_timeNode->setVisible(false);
        m_sysMsgNode->setVisible(true);
        m_sysMsgLabel->setString(mDialogInfo->contents.c_str());
    }else{
        m_userNode->setVisible(true);
        m_msgNode->setVisible(true);
        m_timeNode->setVisible(true);
        m_sysMsgNode->setVisible(false);
        m_sysMsgLabel->setString("");
        CCSprite* pic = NULL;
        if(mDialogInfo->pic=="" || mDialogInfo->pic=="0"){
            pic = CCLoadSprite::createSprite("g044.png",true,CCLoadSpriteType_HEAD_ICON);
        }else{
            std::string head = mDialogInfo->pic;
            head.append(".png");
            pic = CCLoadSprite::createSprite(head.c_str());
        }
        CCCommonUtils::setSpriteMaxSize(pic, 80);
        m_picNode->addChild(pic);
        if (showCustomPic(mDialogInfo->picVer)){
            m_headImgNode->initHeadImgUrl2(m_picNode, CCCommonUtils::getCustomPicUrl(mDialogInfo->uid, mDialogInfo->picVer), 1.0f, 80, true);
        }
        string _name = mDialogInfo->senderName;
        if (mDialogInfo->asn != "") {
            _name = "(" + mDialogInfo->asn + ") " + mDialogInfo->senderName;
        }
        if(mDialogInfo->vip>0){
            m_vipLabel->setString(_lang_1("103001", CC_ITOA(mDialogInfo->vip)));
            float infoX = m_vipLabel->getPositionX()+m_vipLabel->getContentSize().width*m_vipLabel->getOriginScaleX()+ 10;
            m_nameLabel->setPositionX(infoX);
        }else{
            m_vipLabel->setString("");
            m_nameLabel->setPositionX(m_vipLabel->getPositionX());
        }
        m_nameLabel->setString(_name.c_str());
//        m_nameLabel->setColor({111,26,0});
        bool flag = checkSameLangBySystem(mDialogInfo->originalLang);
        
        if(!flag && GlobalData::shared()->translation && mDialogInfo->uid!=GlobalData::shared()->playerInfo.uid && mDialogInfo->translationMsg!=mDialogInfo->contents && mDialogInfo->translationMsg!=""){
            m_translateNode->setVisible(true);
        }else{
            if(mDialogInfo->uid==GlobalData::shared()->playerInfo.uid){
                m_translateNode->setVisible(false);
            }else{
                m_translateNode->setVisible(true);
            }
        }
        string typeCache="";
        if(mDialogInfo->originalLang!=""){
            string typeCacheKey = "LanguageTranslation.set."+mDialogInfo->originalLang;
            typeCache= CCUserDefault::sharedUserDefault()->getStringForKey(typeCacheKey.c_str());
        }
        m_msgLabel->setDimensions(CCSizeMake(450, 0));
        bool bTrans = true;
        if(mDialogInfo->translationMsg.empty()){
            bTrans=false;
            if(!GlobalData::shared()->translation || typeCache!=""  || mDialogInfo->uid==GlobalData::shared()->playerInfo.uid || flag){
                m_translateLabel->setString(_lang("105322"));
                //CCCommonUtils::setButtonTitle(m_translateBtn, "translate");
                m_msgLabel->setString(mDialogInfo->contents.c_str());
                m_isOriginal = true;
            }
        }else{
            if(mDialogInfo->uid==GlobalData::shared()->playerInfo.uid){
                bTrans=false;
            }else{
                bTrans=true;
            }
        }
        if(bTrans==true){
            if(mDialogInfo->isShowOrginal)
            {
                m_translateLabel->setString(_lang("105322"));
                m_msgLabel->setString(mDialogInfo->contents.c_str());
                m_isOriginal = true;
            }else{
                m_translateLabel->setString(_lang_1("105321", mDialogInfo->originalLang.c_str()));
                //CCCommonUtils::setButtonTitle(m_translateBtn, "original");
                m_msgLabel->setString(mDialogInfo->translationMsg.c_str());
                m_isOriginal = false;
            }
        }
        
        height = m_bgSize.height;
        float _tmpHeight = MAX(getTextDimension(mDialogInfo->translationMsg.c_str()).height,getTextDimension(mDialogInfo->contents.c_str()).height);
        float offsetH = 0;
        if (_tmpHeight > 50) {
            offsetH = _tmpHeight - 45;
            height += offsetH;
            m_msgLabel->setPositionY(66+offsetH);
            m_nameLabel->setPositionY(75+offsetH);
            m_timeNode->setPositionY(124+offsetH);
            m_translateNode->setPositionY(76+offsetH);
            m_vipLabel->setPositionY(m_nameLabel->getPositionY());
        }
        m_uid = mDialogInfo->uid;
        //        time_t createTime = GlobalData::shared()->changeTime(mDInfo->createTime);
        time_t infoTime = GlobalData::shared()->getTimeStamp();
        string lastTime = CCCommonUtils::timeStampToDHM(infoTime).substr(0,10);
        string nowTime = CCCommonUtils::timeStampToDHM(mDialogInfo->createTime).substr(0,10);
        if (lastTime == nowTime) {
            m_timeLabel->setString(CCCommonUtils::timeStampToHour(mDialogInfo->createTime).c_str());
        }else{
            m_timeLabel->setString(CCCommonUtils::timeStampToDHM(mDialogInfo->createTime).c_str());
        }
        if (mDialogInfo->fromUid == GlobalData::shared()->playerInfo.uid) {
            m_userNode->setPositionX(510);
            m_msgNode->setPositionX(-102);
            m_timeNode->setPositionX(-102);
            m_left->setVisible(false);
            m_right->setVisible(true);
            m_right->setColor(ccc3(230, 253, 233));
            m_chatBG->setColor(ccc3(230, 253, 233));
        }else{
            m_userNode->setPositionX(0);
            m_msgNode->setPositionX(0);
            m_timeNode->setPositionX(0);
            m_right->setVisible(false);
            m_left->setVisible(true);
            m_chatBG->setColor(ccWHITE);
        }
        m_chatBG->setVisible(true);
        m_chatBG->setPreferredSize(CCSize(m_bgSize.width,height));
        m_sysBG->setVisible(false);
    }
    std::string strTime = m_timeLabel->getString();
    if (height != m_bgSize.height)
    {
        float addHeight = height - m_bgSize.height;
        if (strTime!="") {
            m_timeNode->setPositionY(m_timeNodeY + addHeight);
        }
        if(addHeight>0){
            m_chatBG->setContentSize(CCSizeMake(m_bgSize.width, height));
            m_sysBG->setContentSize(CCSizeMake(m_bgSize.width, height));
            m_touchNode1->setContentSize(CCSizeMake(m_bgSize.width, height));
            m_nameLabel->setPositionY(m_nameLabelPt.y + addHeight);
            m_vipLabel->setPositionY(m_vipLabelPt.y + addHeight);
            m_translateNode->setPositionY(m_translatePt.y+addHeight);
            m_msgLabel->setPositionY(m_msgLabelY + addHeight);
            m_msgLabel->setDimensions(CCSizeMake(m_msgDimensions.width, m_msgDimensions.height+addHeight));
        }
    }
    m_timeNode->setVisible(strTime!="");
}

bool ChatRoomCell::checkSameLangBySystem(string oLang){
    bool flag = false;
    std::string sLang = LocalController::shared()->getLanguageFileName();
    if(oLang.find("zh")<oLang.length() && sLang.find("zh")<sLang.length()){
        flag = true;
    }else if(oLang==sLang || oLang==""){
        flag = true;
    }
    return flag;
}

void ChatRoomCell::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
        //CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChatCell::setTranslation),MSG_TRANSLATE, NULL);
}

void ChatRoomCell::onExit(){
    setTouchEnabled(false);
   
    //CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TRANSLATE);
    CCNode::onExit();
}

SEL_CCControlHandler ChatRoomCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
     //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTranslateClick", ChatCell::onTranslateClick);
    return NULL;
}

bool ChatRoomCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_userNode", CCNode*, this->m_userNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgNode", CCNode*, this->m_msgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIFTTF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIFTTF*, this->m_msgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_vipLabel", CCLabelIFBMFont*, this->m_vipLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIFTTF*, this->m_timeLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode1", CCNode*, this->m_touchNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode2", CCNode*, this->m_touchNode2);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode3", CCNode*, this->m_touchNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeNode", CCNode*, this->m_timeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_left", CCSprite*, m_left);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_right", CCSprite*, m_right);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_chatBG", CCScale9Sprite*, m_chatBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sysBG", CCScale9Sprite*, m_sysBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_translateNode", CCNode*, this->m_translateNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_translateLabel", CCLabelIF*, this->m_translateLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_translateBtn", CCControlButton*, this->m_translateBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_langNode", CCNode*, this->m_langNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sysMsgNode", CCNode*, this->m_sysMsgNode);
     CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sysMsgLabel", CCLabelIFTTF*, this->m_sysMsgLabel);
    return false;
}

void ChatRoomCell::onTranslateClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    if(!mDialogInfo)
        return;
    if(mDialogInfo->tipFlag == 1)
        return;
    if(m_isOriginal){
        if(mDialogInfo->translationMsg != ""){
            m_msgLabel->setString(mDialogInfo->translationMsg.c_str());
            m_translateLabel->setString(_lang_1("105321", mDialogInfo->originalLang.c_str()));
            m_isOriginal = false;
            mDialogInfo->isShowOrginal = false;
        }else{
            MailController::getInstance()->addLocalTranslate(mDialogInfo);
            mDialogInfo->isShowOrginal=true;
        }
    }else{
        if(mDialogInfo->contents != ""){
            m_msgLabel->setString(mDialogInfo->contents.c_str());
        }else{
        }
        m_translateLabel->setString(_lang("105322"));
        m_isOriginal = true;
        mDialogInfo->isShowOrginal=true;
    }
}

void ChatRoomCell::setTranslation(CCObject* param){
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dic = _dict(result->getData());
    if(dic && dic->objectForKey("translationMsg")){
        string msg = dic->valueForKey("translationMsg")->getCString();
        m_msgLabel->setString(msg.c_str());
       
        if(mDialogInfo){
            mDialogInfo->translationMsg = msg;
            if(mDialogInfo->originalLang.c_str()){
                m_translateLabel->setString(_lang_1("105321", mDialogInfo->originalLang.c_str()));
            }
        }
    }
}

bool ChatRoomCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if(m_translateBtn && isTouchInside(m_translateBtn, pTouch)){
         m_translateBtn->setHighlighted(true);
    }
    m_touchPoint = pTouch->getLocation();
    if(isTouchInside(m_touchNode1, pTouch) || isTouchInside(m_touchNode2, pTouch)) {
        return true;
    }
    return false;
}

void ChatRoomCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    m_translateBtn->setHighlighted(false);
    if (fabs( pTouch->getLocation().y - m_touchPoint.y) > 30) {
        return;
    }else if(isTouchInside(m_langNode, pTouch) && m_translateNode->isVisible()){
        onTranslateClick(NULL,Control::EventType::TOUCH_DOWN);
    }else if (isTouchInside(m_touchNode1, pTouch) || isTouchInside(m_touchNode2, pTouch) ) {
		if(m_prevPopupCount == -1){
			m_prevPopupCount = PopupViewController::getInstance()->getCurrViewCount();
		}
    }
}
void ChatRoomCell::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(m_translateBtn && isTouchInside(m_translateBtn, pTouch)){
        m_translateBtn->setHighlighted(true);
    }
}

bool ChatRoomCell::showCustomPic(int senderPicVer)
{
    bool ret = false;
    if (senderPicVer <= 999999 && senderPicVer >=1)
    {
        return true;
    }
    else
    {
        return false;
    }
    return ret;
}
CCSize ChatRoomCell::getTextDimension(const char *text)
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
