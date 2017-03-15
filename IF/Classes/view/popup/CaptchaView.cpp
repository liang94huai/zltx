//
//  CaptchaView.cpp
//  IF
//
//  Created by liruiqi on 15/9/25.
//
//

#include "CaptchaView.h"
#include "PopupViewController.h"
#include "UserBindCommand.h"
#include "md5.h"

CaptchaView* CaptchaView::create(CCDictionary *dict){
    CaptchaView *ret = new CaptchaView();
    if(ret && ret->init(dict)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool CaptchaView::init(CCDictionary *dict) {
    if(PopupBaseView::init()){
        setIsHDPanel(true);
//        CCLoadSprite::doResourceByCommonIndex(502, true);
//        CCLoadSprite::doResourceByCommonIndex(306, true);
//        setCleanFunction([](){
//            CCLoadSprite::doResourceByCommonIndex(502, false);
//            CCLoadSprite::doResourceByCommonIndex(306, false);
//        });
        m_waitInterface = NULL;
        m_failRetryTime = 0;
        m_dict = dict;
        isGetPic = false;
        auto ccb = CCBLoadFile("CaptchaViewCCB",this,this);
        this->setContentSize(ccb->getContentSize());
        m_changText->setString(_lang("E100188"));
//        m_changText->setString("huanyizhang");
        auto underLine = CCLoadSprite::createSprite("mail_line.png");
        m_lineNode->addChild(underLine);
        underLine->setScaleX(m_changText->getContentSize().width * m_changText->getOriginScaleX() / underLine->getContentSize().width);
        CCCommonUtils::setButtonTitle(m_okBtn, _lang("101274").c_str());
        m_hintText->setString(_lang("E100185"));
        auto xSize = m_editBoxContainer->getContentSize();
//        m_captchaEdit = InputFieldMultiLine::create(xSize, "UI_Chat_text.png", 30);
        m_captchaEdit = InputFieldMultiLine::create(xSize, "mail_check_bg.png", 30);
        m_captchaEdit->setAddH(5);
        m_captchaEdit->setAnchorPoint(ccp(0,0));
        m_captchaEdit->setMaxChars(16);
        m_captchaEdit->setLineNumber(1);
        m_captchaEdit->setFontColor(ccBLACK);
        m_captchaEdit->setPosition(ccp(0,0));
        m_captchaEdit->setSwallowsTouches(true);
        m_captchaEdit->setMoveFlag(true);
        m_captchaEdit->setTouchPriority(0);
        m_captchaEdit->setcalCharLen(true);
        m_captchaEdit->setOnlySingleLine(true);
        std::string nameTip = "";
        m_captchaEdit->setPlaceHolder(nameTip.c_str());
        m_editBoxContainer->addChild(m_captchaEdit);
        
        this->setModelLayerTouchCallback([&](cocos2d::CCTouch *pTouch){
            // detect touch inside panel
            if(isTouchInside(m_changText, pTouch) || isTouchInside(m_picNode, pTouch)){
                m_failRetryTime = 0;
                onRefreshClick(NULL, Control::EventType::TOUCH_DOWN);
                return;
            }
            if(!isTouchInside(m_touchNode, pTouch)){
                this->closeSelf();
            }
        });
        onRefreshClick(NULL, Control::EventType::TOUCH_DOWN);
        return true;
        
    }
    return false;
}

bool CaptchaView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, this->m_picNode)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editBoxContainer", CCNode*, this->m_editBoxContainer)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_okBtn", CCControlButton*, this->m_okBtn)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_touchNode", CCNode*, this->m_touchNode)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_lineNode", CCNode*, this->m_lineNode)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_hintText", CCLabelIF*, this->m_hintText)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_changText", CCLabelIF*, this->m_changText)
    
    return false;
}

SEL_CCControlHandler CaptchaView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSendBtnClick", CaptchaView::onSendBtnClick);

    return NULL;}

void CaptchaView::editBoxEditingDidBegin(CCEditBox* editBox){
    editBox->setText("");
}

void CaptchaView::editBoxReturn(CCEditBox *editBox) {

}

void CaptchaView::editBoxTextChanged(CCEditBox* editBox, const std::string& text){
    
}

void CaptchaView::onCancelBtnClick(CCObject * pSender, CCControlEvent pCCControlEvent){
    this->closeSelf();
}

void CaptchaView::onSendBtnClick(CCObject *pSender, CCControlEvent event) {
    if(isGetPic){
        return;
    }

    auto button = dynamic_cast<CCControlButton*>(pSender);
    if (button) {
        button->setEnabled(false);
    }
    std::string str = m_captchaEdit->getText();
    
    std::string googlePlay = m_dict->valueForKey("googlePlay")->getCString();
    std::string facebook = m_dict->valueForKey("facebook")->getCString();
    std::string device = m_dict->valueForKey("device")->getCString();
    std::string mail = m_dict->valueForKey("mail")->getCString();
    std::string pass = m_dict->valueForKey("pass")->getCString();
    int optType = m_dict->valueForKey("optType")->intValue();
    std::string preUid = m_dict->valueForKey("preUid")->getCString();
    std::string googlePlayName = m_dict->valueForKey("googlePlayName")->getCString();
    std::string facebookName = m_dict->valueForKey("facebookName")->getCString();
    std::string preName = m_dict->valueForKey("preName")->getCString();
    std::string bindPf = m_dict->valueForKey("bindPf")->getCString();
    std::string bindId = m_dict->valueForKey("bindId")->getCString();
    std::string bindName = m_dict->valueForKey("bindName")->getCString();
    UserBindCommand *cmd = new UserBindCommand(googlePlay, facebook, device, mail, pass, optType, preUid, googlePlayName, facebookName, preName, bindPf, bindId, bindName, str);
    cmd->sendAndRelease();
}

void CaptchaView::onRefreshClick(CCObject *pSender, CCControlEvent event) {
    if(isGetPic){
        return;
    }
    isGetPic = true;
    CCHttpRequest* request = new CCHttpRequest();
    
//    std::string ip = "http://10.1.6.72:8081/vcode/";
//#if COCOS2D_DEBUG == 2 || COCOS2D_DEBUG == 0
    std::string ip = "http://p1cok.elexapp.com/vcode/index.php";
//#endif
    if(CCCommonUtils::isChina()){
        ip = "http://p3cok.elexapp.com/vcode/index.php";
    }
#if COCOS2D_DEBUG == 1
    ip = "http://10.1.6.72:8081/vcode/";
#endif
    auto currentTime = time(NULL);
    std::string timeStr = CC_ITOA(double(currentTime));
    std::string key = "vcode_2S@FxC$3";
    std::string uid = GlobalData::shared()->playerInfo.uid;
    std::string token = MD5(key + MD5(MD5(uid).toString() + timeStr).toString()).toString();

    string url =  ip + CCString::createWithFormat("?ac=get&uid=%s&time=%s&token=%s", uid.c_str(), timeStr.c_str(), token.c_str())->getCString();
    CCLog("CaptchaView url: %s", url.c_str());
    request->setUrl(url.c_str());
    request->setRequestType(CCHttpRequest::Type::GET);
    request->setResponseCallback(this, httpresponse_selector(CaptchaView::getDataBack));
    CCHttpClient::getInstance()->setTimeoutForConnect(10);
    CCHttpClient::getInstance()->send(request);
    request->release();
    removeWaitInterface();
    m_waitInterface = WaitInterface::show(m_picNode, ccp(0, 0), m_picNode->getContentSize());
    m_waitInterface->setPosition(m_waitInterface->getPosition() + m_picNode->getContentSize() / 2);
}

void CaptchaView::removeWaitInterface(){
    if(m_waitInterface != nullptr){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
}

void CaptchaView::sendDataBack(CCObject *obj){
    m_okBtn->setEnabled(true);
    CCString *str = dynamic_cast<CCString*>(obj);
    if(str != NULL){
        std::string resultStr = str->getCString();
        if(resultStr == "1"){
            checkSuccess();
        }
    }
    checkFail();
}

void CaptchaView::checkFail(){
    
}

void CaptchaView::checkSuccess(){
    this->closeSelf();
}

void CaptchaView::doWhenGetDataFailed(){
    if(m_failRetryTime >= MAX_FAIL_RETRY_TIME){
        CCCommonUtils::flyHint("", "", _lang("E100086"));
        return;
    }
    onRefreshClick(NULL, Control::EventType::TOUCH_DOWN);
    m_failRetryTime++;
}

void CaptchaView::getDataBack(CCHttpClient* client, CCHttpResponse* response){
    removeWaitInterface();
    CCLog("CaptchaView data back");
    isGetPic = false;
    if (!response){
        doWhenGetDataFailed();
        return;
    }

    int m_responseCode = response->getResponseCode();
    CCLOG("response code: %d", m_responseCode);
    if (!response->isSucceed())
    {
        doWhenGetDataFailed();
        CCLog("response failed!\nerror buffer: %s", response->getErrorBuffer());
        return;
    }
    std::vector<char>* buffer = response->getResponseData();
    auto image = new CCImage();
    
    image->initWithImageData((unsigned char*)buffer->data(),buffer->size());
    
    //create texture
    auto texture = new CCTexture2D();
    bool isImage = texture->initWithImage(image);
    
    auto sprite = CCSprite::createWithTexture(texture);
    m_picNode->addChild(sprite);
    sprite->setScale(1.5);
    sprite->setAnchorPoint(ccp(0, 0));
    sprite->setPosition((m_picNode->getContentSize() - sprite->getContentSize() * sprite->getScale()) / 2);
    image->release();
}

void CaptchaView::onEnter() {
    PopupBaseView::onEnter();
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CaptchaView::sendDataBack), VERIFY_BACK, NULL);
}

void CaptchaView::onExit() {
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, VERIFY_BACK);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    removeWaitInterface();
    PopupBaseView::onExit();
}