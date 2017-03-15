//
//  GoldExchangeItemView_NEW.cpp
//  IF
//
//  Created by xxrdsg on 15/11/3.
//
//

#include "GoldExchangeItemView_NEW.hpp"
#include "PopupViewController.h"
#include "PayController.h"
#include "Utf8Utils.h"

GoldExchangeItemView_NEW* GoldExchangeItemView_NEW::create(GoldExchangeItem *goldExchangeItem)
{
    GoldExchangeItemView_NEW* ret = new GoldExchangeItemView_NEW(goldExchangeItem);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void GoldExchangeItemView_NEW::onEnter()
{
    Node::onEnter();
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GoldExchangeItemView_NEW::refreshData), PAYMENT_COMMAND_RETURN, NULL);
}
void GoldExchangeItemView_NEW::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, PAYMENT_COMMAND_RETURN);
    setTouchEnabled(false);
    GlobalData::shared()->isBind = false;
    Node::onExit();
}
bool GoldExchangeItemView_NEW::init()
{
    CCBLoadFile("GoldExchangeItemViewNew_NEW", this, this);
     CCLoadSprite::doResourceByCommonIndex(2, true);
            setCleanFunction([](){
               // CCLoadSprite::doResourceByCommonIndex(2, false);
            });
    m_hotLabel->setColor({255, 212, 82});
    m_desText->setColor({105, 166, 197});
    m_goldNum->setColor({254, 203, 0});
    m_goldAddNum->setColor({255, 132, 0});
    m_desText->setFntFile("Arial_Bold_Regular.fnt");
    m_goldAddNum->setFntFile("Arial_Bold_Regular.fnt");
    m_dollerNum->setFntFile("Arial_Bold_Regular.fnt");
    m_dollerNum->setColor({52, 252, 254});
    refreshData();
    return true;
}
bool GoldExchangeItemView_NEW::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", Node*, m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desText", CCLabelIF*, m_desText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldNum", CCLabelIF*, m_goldNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldAddNum", CCLabelIF*, m_goldAddNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_blueBtn", ControlButton*, m_blueBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_greenBtn", ControlButton*, m_greenBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dollerNum", CCLabelIF*, m_dollerNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotNode", Node*, m_hotNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotLabel", CCLabelIF*, m_hotLabel);
    
    return true;
}

SEL_CCControlHandler GoldExchangeItemView_NEW::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBlueBtn", GoldExchangeItemView_NEW::onClickBlueBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickGreenBtn", GoldExchangeItemView_NEW::onClickGreenBtn);
    return nullptr;
}

void GoldExchangeItemView_NEW::onClickBlueBtn(Ref* pSender, Control::EventType eventType)
{
    payClick();
}

void GoldExchangeItemView_NEW::onClickGreenBtn(Ref* pSender, Control::EventType eventType)
{
    payClick();
}

void GoldExchangeItemView_NEW::payClick()
{
    if(m_currdataItem){
        if(m_currdataItem->type=="3" && m_currdataItem->isPayCallBack){
            return;
        }
        PayController::getInstance()->callPayment(m_currdataItem,"GoldExchangeItemView");
    }else{
        PayController::getInstance()->callPayment(m_currdataItem,"GoldExchangeItemView");
    }
}

bool GoldExchangeItemView_NEW::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}
void GoldExchangeItemView_NEW::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void GoldExchangeItemView_NEW::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
}

void GoldExchangeItemView_NEW::refreshData(CCObject* p)
{
    // reset data by sale
    if(m_dataItem && atoi(m_dataItem->id.c_str())>0){
        m_currdataItem =m_dataItem;
        for (map<string, GoldExchangeItem*>::iterator it = GlobalData::shared()->goldExchangeList.begin(); it != GlobalData::shared()->goldExchangeList.end(); it++) {
            if(!it->second)
                continue;
            if(it->second->type == "1"||it->second->bought||it->second->popup_image=="new_recharge"){
                continue;
            }
            if(m_currdataItem->dollar==it->second->dollar){//同价格的礼包，用高级别的覆盖
                if(it->second->type == "2"||(it->second->type == "3"&&it->second->popup_image=="close")){//用popup值大的替换
                    if (it->second->popup>m_currdataItem->popup) {
                        m_currdataItem = it->second;
                    }
                }
            }
        }
        m_iconNode->removeAllChildren();
        m_goldNum->setString("");
        m_goldAddNum->setString("");
        m_hotLabel->setString("热");
        m_hotNode->setVisible(false);
        m_dollerNum->setString("");
        m_desText->setString("");
        CCCommonUtils::setButtonTitle(m_blueBtn, "购买");
        CCCommonUtils::setButtonTitle(m_greenBtn, "购买");
         
        int sprid =  atoi((m_dataItem->product_id.substr(m_dataItem->product_id.find("_")+1)).c_str())+1;
        if(sprid>6){
            sprid = 1;
        }
        string strID = CC_ITOA(sprid);
        string boxName = "Icon_Gold_0" + strID + "_gold_new.png";
        CCSprite* sprite = CCLoadSprite::createSprite(boxName.c_str());
        m_iconNode->addChild(sprite);
        if (m_currdataItem->type == "2") {
            m_blueBtn->setVisible(false);
            m_greenBtn->setVisible(true);
            string dollar = PayController::getInstance()->getDollarText(m_currdataItem->dollar,m_currdataItem->product_id);
            this->m_dollerNum->setString(getTwoLines(dollar).c_str());
            this->m_goldNum->setString(CC_CMDITOA(atoi(m_currdataItem->gold_doller.c_str())-m_currdataItem->price).c_str());
            string plus("+");
            this->m_goldAddNum->setString(plus + CC_CMDITOA(m_currdataItem->price));
            m_desText->setString(_lang("101238"));
            m_hotNode->setVisible(true);
        } else if (m_currdataItem->type == "3") {
            m_blueBtn->setVisible(false);
            m_greenBtn->setVisible(true);
            string dollar = PayController::getInstance()->getDollarText(m_currdataItem->dollar,m_currdataItem->product_id);
            this->m_dollerNum->setString(getTwoLines(dollar).c_str());
            this->m_goldNum->setString(CC_CMDITOA(atoi(m_currdataItem->gold_doller.c_str())-m_currdataItem->price).c_str());
            string plus("+");
            this->m_goldAddNum->setString(plus + CC_CMDITOA(m_currdataItem->price));
            m_desText->setString(_lang("101237"));
            m_hotNode->setVisible(true);
        } else {
            if (m_currdataItem->price > 0) {
                m_blueBtn->setVisible(false);
                m_greenBtn->setVisible(true);
                string dollar = PayController::getInstance()->getDollarText(m_currdataItem->dollar,m_currdataItem->product_id);
                this->m_dollerNum->setString(getTwoLines(dollar).c_str());
                this->m_goldNum->setString(CC_CMDITOA(atoi(m_currdataItem->gold_doller.c_str())-m_currdataItem->price).c_str());
                string plus("+");
                this->m_goldAddNum->setString(plus + CC_CMDITOA(m_currdataItem->price));
                m_desText->setString(_lang("101236"));
                m_hotNode->setVisible(true);
            } else {
                m_blueBtn->setVisible(true);
                m_greenBtn->setVisible(false);
                string dollar = PayController::getInstance()->getDollarText(m_currdataItem->dollar,m_currdataItem->product_id);
                this->m_dollerNum->setString(getTwoLines(dollar).c_str());
                this->m_goldNum->setString(CC_CMDITOA(atoi(m_currdataItem->gold_doller.c_str())).c_str());
            }
        }
        m_goldAddNum->setPositionX(m_goldNum->getPositionX() + m_goldNum->getContentSize().width * m_goldNum->getOriginScaleX());
    }else{
        m_currdataItem =m_dataItem;
        m_desText->setString("");
        m_goldNum->setString("");
        m_goldAddNum->setString("");
        m_hotLabel->setString("热");
        m_hotNode->setVisible(false);
        m_iconNode->removeAllChildren();
        
        m_greenBtn->setVisible(false);
        m_blueBtn->setVisible(true);

        m_dollerNum->setString(_lang("101026"));
        // if(GlobalData::shared()->analyticID=="mycard"){
        //     CCSprite *sprite = CCLoadSprite::createSprite("mycard.png");
        //     sprite->setScale(0.75);
        //     sprite->setPositionX(5);
        //     m_iconNode->addChild(sprite);
        // }else if(GlobalData::shared()->analyticID=="gash"){
        //     CCSprite *sprite = CCLoadSprite::createSprite("cash.png");
        //     sprite->setScale(0.8);
        //     sprite->setPositionX(5);
        //     m_iconNode->addChild(sprite);
        // }
        m_goldAddNum->setPositionX(m_goldNum->getPositionX() + m_goldNum->getContentSize().width * m_goldNum->getOriginScaleX());
    }
}

string GoldExchangeItemView_NEW::getTwoLines(string dollar)
{
    return dollar;
    string returnStr = dollar;
    if (returnStr.empty())
    {
        return returnStr;
    }
    int strBegin = 0;
    int numBegin = 0;
    for (int i = 0; i < dollar.size(); ++i) {
        
        if (dollar[i] >= '0' && dollar[i] <= '9') {
            numBegin = i;
            break;
        }
    }
    for (int i = 0; i < dollar.size(); ++i) {
        if (dollar[i] < '0' || dollar[i] > '9') {
            strBegin = i;
            break;
        }
    }
    if (strBegin == numBegin) {
        return returnStr;
    }
    string strstr("");
    string numstr("");
    if (strBegin < numBegin) {
        strstr = dollar.substr(0, numBegin);
        numstr = dollar.substr(numBegin, dollar.size() - numBegin);
        returnStr = numstr + "\n" + strstr;
    } else {
        numstr = dollar.substr(0, strBegin);
        strstr = dollar.substr(strBegin, dollar.size() - strBegin);
        returnStr = numstr + "\n" + strstr;
    }
    return returnStr;
}
