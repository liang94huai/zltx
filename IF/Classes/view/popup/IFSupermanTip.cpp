//
//  IFSupermanTip.cpp
//  IF
//
//  Created by wangdianzhen on 15/9/9.
//
//

#include "IFSupermanTip.h"
#include "GetUserInfoCommand.h"
#include "LuaController.h"

IFSupermanTip* IFSupermanTip::create(int type,string infoStr)
{
    IFSupermanTip *pRet = new IFSupermanTip(type, infoStr);
    if (pRet && pRet->initData())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool IFSupermanTip::initData()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    
    CCLoadSprite::doResourceByCommonIndex(11, true);
    
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false);
    });
    setIsHDPanel(true);
    setModelLayerDisplay(false);
    CCBLoadFile("superManTip",this,this);
    m_infoLb->setString(_lang("113183"));
    m_btnNode->setVisible(false);
    m_translateSp->setVisible(false);
    m_lineSp->setVisible(true);
    CCCommonUtils::setButtonTitle(m_editBtn, _lang("115034").c_str());
    CCCommonUtils::setButtonTitle(m_saveBtn, _lang("115035").c_str());
    m_editLb->setColor(ccc3(176, 161, 130));
    switch (m_type) {
        case 0:// other no des
        {
            m_bgNode->setScaleY(0.5);
            CCPoint fixPos = CCPointMake(m_bgNode->getContentSize().width* m_bgNode->getScaleX() /2, - m_bgNode->getContentSize().height* m_bgNode->getScaleY() /2);
            fixPos.x = fixPos.x - m_infoLb->getContentSize().width / 2 - 34;
            m_subNode->setPosition(fixPos);
            m_lineSp->setVisible(false);
        }
            break;
        case 1://  other have des
            m_bgNode->setScaleY(0.6);
            m_editLb->setString(m_infoStr.c_str());
            m_translateSp->setVisible(true);
            break;
        case 2:// me no des
            m_editLb->setString(_lang("113186").c_str());
            m_editLb->setColor(ccc3(97, 90, 77));
            m_btnNode->setVisible(true);
            m_saveBtn->setEnabled(false);
            break;
        case 3:// me have des
            m_editLb->setString(m_infoStr.c_str());
            m_btnNode->setVisible(true);
            m_saveBtn->setEnabled(false);
            break;
        default:
            break;
    }
    return true;
}

void IFSupermanTip::onEnter() {
    CCNode::onEnter();
    setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(IFSupermanTip::translateBack), msg_superman_des, NULL);
}
void IFSupermanTip::translateBack(CCObject* pObj){
    if(pObj){
        string desStr = dynamic_cast<CCString*>(pObj)->getCString();
        m_editLb->setString(desStr.c_str());
        m_istranslated = true;
        m_callbackStr = desStr;
    }
}
void IFSupermanTip::onExit() {
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, msg_superman_des);
    CCNode::onExit();
}

bool IFSupermanTip::onTouchBegan(CCTouch* pTouch,CCEvent* pEvent){

    if(isTouchInside(m_editBtn, pTouch) || isTouchInside(m_saveBtn, pTouch)){
       return false;
    }
    return true;
}
void IFSupermanTip::onTouchEnded(CCTouch* pTouch,CCEvent* pEvent){
    if(!isTouchInside(m_bgNode, pTouch)){
        if (this->getParent()) {
            this->removeFromParentAndCleanup(true);
            return;
        }
    }
    if(isTouchInside(m_translateSp, pTouch)){
        CCLOGFUNC("todotranslate!");
        if (!m_infoStr.empty()) {
            if (m_istranslated) {
                m_editLb->setString(m_infoStr.c_str());
                m_istranslated = false;
            }
            else{
                if(m_callbackStr.empty()){
                    LuaController::getInstance()->translate(m_infoStr, LocalController::shared()->getLanguageFileName(),0,TRANSLATE_SUPERMAN);
                }
                else{
                    m_editLb->setString(m_callbackStr.c_str());
                    m_istranslated = true;
                }
            }
        }
        
    }
}
void IFSupermanTip::onEditClick(CCObject * pSender, Control::EventType pCCControlEvent){
    CCLOGFUNC("edit!");
    auto sprite9 = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
    m_announceEditBox = CCEditBox::create(CCSizeMake(490,40),sprite9);
    if (CCCommonUtils::isIosAndroidPad())
    {
        sprite9 = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
        m_announceEditBox = CCEditBox::create(CCSizeMake(1176,60),sprite9);
    }
    m_announceEditBox->setMaxLength(199);
    m_announceEditBox->setFontSize(20);
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_announceEditBox->setFontSize(40);
    }
    m_announceEditBox->setFontColor(ccBLACK);
    m_announceEditBox->setReturnType(kKeyboardReturnTypeDone);
    m_announceEditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
    m_announceEditBox->setText(m_infoStr.c_str());//
    m_editNode->addChild(m_announceEditBox);
    m_editBtn->setEnabled(false);
    m_saveBtn->setEnabled(true);
    m_editLb->setVisible(false);
}
void IFSupermanTip::onSaveClick(CCObject * pSender, Control::EventType pCCControlEvent){
    CCLOGFUNC("save!");
    m_editBtn->setEnabled(true);
    m_saveBtn->setEnabled(false);
    m_announceEditBox->setEnabled(false);
    m_editLb->setVisible(true);
    m_editLb->setString(m_announceEditBox->getText());
    supermanDesSaveCommand * command = new supermanDesSaveCommand(m_announceEditBox->getText());
//    command->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(IFSupermanTip::saveSuccess), NULL));
    command->sendAndRelease();
    m_editLb->setColor(ccc3(176, 161, 130));
    m_editLb->setString(m_announceEditBox->getText());
    m_editNode->removeAllChildrenWithCleanup(true);
    m_announceEditBox = NULL;
}
void IFSupermanTip::saveSuccess(CCObject* pObj){
    //
}

SEL_CCControlHandler IFSupermanTip::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{

    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onEditClick", IFSupermanTip::onEditClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSaveClick", IFSupermanTip::onSaveClick);
    return NULL;
}

bool IFSupermanTip::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subNode", CCNode*, this->m_subNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode", CCNode*, this->m_btnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editNode", CCNode*, this->m_editNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLb", CCLabelIF*, this->m_infoLb);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editLb", CCLabelIFTTF*, this->m_editLb);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editBtn", CCControlButton*, this->m_editBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_saveBtn", CCControlButton*, this->m_saveBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_translateSp", CCSprite*, this->m_translateSp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lineSp", CCSprite*, this->m_lineSp);

    return false;
}


