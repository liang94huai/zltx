//
//  NoticeView.cpp
//  IF
//
//  Created by duwei on 17-3-2.
//
//

#include "NoticeView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"

NoticeView* NoticeView::create(string notice){
    NoticeView* ret = new NoticeView();
    if(ret && ret->init(notice)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool NoticeView::init(string notice)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    
    auto tmpCCB = CCBLoadFile("NoticeView",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    m_mainNode->setPositionY(m_mainNode->getPositionY()-getExtendHeight()/2);
    m_label->setString("游戏公告");
    
    CCLOG("old x, y: %f, %f", m_infoList->getPositionX(), m_infoList->getPositionY());
    
//    int oldBgHeight = m_buildBG->getContentSize().height;
//    //changeBGHeight(m_buildBG);
//    int newBgHeight = m_buildBG->getContentSize().height;
//    int addHeight = newBgHeight - oldBgHeight;
//    int oldWidth = m_infoList->getContentSize().width;
//    int oldHeight = m_infoList->getContentSize().height;
//    m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
    //m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
    
    CCLOG("cur x, y: %f, %f", m_infoList->getPositionX(), m_infoList->getPositionY());
    
    m_webView = cocos2d::experimental::ui::WebView::create();
    m_webView->setAnchorPoint(Vec2(0.5, 0));
    m_webView->setPosition(Vec2(m_infoList->getContentSize().width/2 , 0));
    m_webView->setContentSize(m_infoList->getContentSize());
    m_webView->setScalesPageToFit(true);
    m_webView->setBackgroundToTransparent();
//    std::string tmp="<br />尊敬的各位主公：<br /><br />&nbsp;&nbsp;&nbsp;&nbsp;欢迎参与首款SLG+RPG自由的战沙盘国战手游的测试，本次测试将首次开放大连服功能。您在完成天下大势内的相应任务后，可以跨越州地图进入更富饶的领地与不同服务器的联盟一决雌雄。由于游戏尚处测试阶段，部分功能难免存在瑕疵，若有任何意见及建议，微臣已经在官方交流群【604079789】恭候各位主公。<font color='3399ff'><a href='https://www.wenjuan.net/s/7vQFvu/'>填新服问卷拿钻石福利-立即前往</a></font>\n<br />与此同时，本次测试期间，玩家的充值额度将在【公测官方服务器】对固定账号进行返还，请牢记自己的充值账号并留意官方群内最新动态。<br /><br />&nbsp;&nbsp;&nbsp;&nbsp;1，充值500元以内享受1:1.2钻石返还，赠送部分不计算在内<br />&nbsp;&nbsp;&nbsp;&nbsp;2，充值501-5000元享受1:1.5钻石返还，赠送部分不计算在内<br />&nbsp;&nbsp;&nbsp;&nbsp;3，充值5001元以上享受1:2钻石返还，赠送部分不计算在内\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;官方运营团队<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2017年2月16日";
    m_webView->loadHTMLString(notice,"");
    m_infoList->addChild(m_webView);
    return true;
}

void NoticeView::onClose()
{
    m_infoList->setVisible(false);
    m_webView->setVisible(false);
    m_webView->goBack();
    PopupViewController::getInstance()->goBackPopupView();
}

void NoticeView::onEnter(){
    CCNode::onEnter();
}

void NoticeView::onExit(){
    CCNode::onExit();
}

void NoticeView::onPopupReturnClick(CCObject *pSender, CCControlEvent event)
{
    onClose();
}

SEL_CCControlHandler NoticeView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPopupReturnClick", NoticeView::onPopupReturnClick);
    return NULL;
}

bool NoticeView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_label", CCLabelIF*, this->m_label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    return false;
}
