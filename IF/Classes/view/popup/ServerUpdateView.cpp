//
//  ServerUpdateView.cpp
//  IF
//
//  Created by hujiuxing on 15-12-16.
//
//


#include "ServerUpdateView.h"
#include "PopupViewController.h"


ServerUpdateView* ServerUpdateView::create()
{
    auto ret = new ServerUpdateView();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ServerUpdateView::init()
{
    if (!PopupBaseView::init()) {
        return false;
    }
    
    CCLog("ServerUpdateView init" );
    
    setIsHDPanel(true);
    
    CCBLoadFile("ServerUpdateView", this, this);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    this->setContentSize(winSize);
    int extH = this->getExtendHeight();
    if (!CCCommonUtils::isIosAndroidPad())
    {
        m_mainNode->setPositionY(m_mainNode->getPositionY() + extH/2);
    }
    
    m_titleLabel->setString(_lang("113865"));
    
    string url="";
    string noteD = "";
    getUrlString(_lang("113869"),url,noteD);

    m_noteLabel->setString(noteD.c_str());
    
    if( url.length() >0 )
    {
        Size size = m_noteLabel->getContentSize();
        CCLog("ServerUpdateView CCRichLabelTTF::create" );
        
        m_noteDRich = CCRichLabelTTF::create("", "Helvetica",22,CCSize(450,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
        m_noteLabel->getParent()->addChild(m_noteDRich);
        m_noteDRich->setAnchorPoint( m_noteLabel->getAnchorPoint() );
        m_noteDRich->setPosition(m_noteLabel->getPosition() - Vec2(0,size.height));
        m_noteDRich->setString(url);
        m_noteDRich->setLinkTarget(0,CCCallFuncO::create(this, callfuncO_selector(ServerUpdateView::onLinkClicked), NULL));
    }
    
    CCCommonUtils::setButtonTitle(m_okBtn, _lang("110031").c_str());
   
    CCLog("ServerUpdateView init over");
  
    return true;
}

bool ServerUpdateView::getUrlString(string desStr,string & url,string &left )
{
    if( desStr.length()<=0 )
    {
        left = desStr;
        url  = "";
        return false;
    }
    
    std::string subText = "www";
    size_t pos = desStr.find(subText);
    
    if( pos == string::npos )
    {
        pos = desStr.find("http");
    }
    
    if( pos == string::npos )
    {
        pos = desStr.find("WWW");
    }
    
    if ( pos!= string::npos ) {
        
        left = desStr.substr(0,pos);
        std::string rightText = desStr.substr(pos,desStr.length() - pos);
        
        m_linkURL = rightText;
        
        CCLog("ServerUpdateView left= %s", left.c_str());
        CCLog("ServerUpdateView left= %s", rightText.c_str());
        
        url = "[u][color=ff0000ff][link bg=00000000 bg_click=ff80A26E]" + rightText + "[/link][/color][/u]" + "\n";
    }
    else
    {
        left =desStr;
    }
    
    return true;
}

void ServerUpdateView::onEnter()
{
    CCNode::onEnter();
    
    //updateRichLabel();
    
    CCLog("ServerUpdateView onEnter");
    
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
}

void ServerUpdateView::onExit()
{
    CCLog("ServerUpdateView onExit");
    
    setTouchEnabled(false);
    
    CCNode::onExit();
}

void ServerUpdateView::onLinkClicked(CCObject *ccObj)
{
    if( m_linkURL.size() > 4 )
    {
        GameController::getInstance()->goTurntoUrl(m_linkURL);
    }
}

bool ServerUpdateView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;

}
void ServerUpdateView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isTouchInside(m_touchNode, pTouch) && (ccpDistance(pTouch->getLocation(), pTouch->getStartLocation())<20))
    {
        PopupViewController::getInstance()->removePopupView(this);
    }
}


bool ServerUpdateView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_noteLabel", CCLabelIFTTF*, this->m_noteLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTTF", CCLabelIF*, this->m_titleLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_okBtn", CCControlButton*, this->m_okBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpBtn", CCControlButton*, this->m_helpBtn);
    
    return false;
}

SEL_CCControlHandler ServerUpdateView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOkClick", ServerUpdateView::onClickOKBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHelpBtnClick", ServerUpdateView::onClickHelpBtn);
    
    return NULL;
}

void ServerUpdateView::onClickHelpBtn(CCObject *pSender, CCControlEvent event)
{
    CCCommonUtils::showHelpShiftFAQ();
}

void ServerUpdateView::onClickOKBtn(CCObject *pSender, CCControlEvent event)
{
    PopupViewController::getInstance()->removePopupView(this);
}
