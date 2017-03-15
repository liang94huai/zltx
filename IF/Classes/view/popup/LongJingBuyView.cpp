 
#include "CommonInclude.h"
#include "LongJingBuyView.h"
#include "PopupViewController.h"
#include "ParticleController.h"
#include "YesNoDialog.h"
#include "UseResToolView.h"
#include "SoundController.h"
#include "ToolController.h"
#include "AllianceManager.h"
#include "GoldExchangeView_NEW.hpp"
#include "EquipmentController.h"

#define r1_color {216,177,146}
#define r2_color {224,149,78}
#define r3_color {66,156,224}

LongJingBuyView::~LongJingBuyView(void){
}

LongJingBuyView::LongJingBuyView():m_effNodePos(CCPointZero),m_effNodeSize(CCSizeZero),m_waitInterface(NULL),m_onClose(NULL),m_numCurrent(1),m_numAll(1){
}

LongJingBuyView* LongJingBuyView::show(const char *url,const char* title,const char* desc,int price,int color,CCCallFuncO * func,CCPoint startPos,int priceType,long cCount){
    LongJingBuyView* dialog = new LongJingBuyView();
    dialog->init(url,title,desc,price,color,priceType,cCount);
    dialog->setYesCallback(func);
    dialog->setStartPos(startPos);
    PopupViewController::getInstance()->addPopupView(dialog, false);
    dialog->release();
    return dialog;
}

bool LongJingBuyView::init(const char* url, const char* title,const char* desc,int price,int color,int priceType,long cCount){
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    bool bRet=false;
    CCLoadSprite::doResourceByCommonIndex(11, true);
    setCleanFunction([](){
//        CCLoadSprite::doResourceByCommonIndex(11, false);
    });
    if (CCBLoadFile("LongJingBuyView", this, this)) {
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        int maxWidth = m_subNode->getContentSize().width;
        int maxHeight = m_subNode->getContentSize().height;
        //label可滑动，放在scrollview上
        CCLabelIF* label = CCLabelIF::create();
        label->setDimensions(CCSize(maxWidth, 0));
        label->setString(desc);
        label->setColor(ccc3(169, 132, 71));
        label->setFontSize(24);
        label->setVerticalAlignment(kCCVerticalTextAlignmentTop);
        label->setHorizontalAlignment(kCCTextAlignmentLeft);
        int totalHeight = label->getContentSize().height * label->getOriginScaleY();
        if(totalHeight > maxHeight){
            CCScrollView* scroll = CCScrollView::create(CCSize(maxWidth, maxHeight));
            scroll->setContentSize(CCSize(maxWidth, totalHeight));
            scroll->addChild(label);
            m_subNode->addChild(scroll);
            scroll->setPosition(CCPointZero);
            scroll->setDirection(kCCScrollViewDirectionVertical);
            scroll->setAnchorPoint(ccp(0, 0));
            scroll->setContentOffset(ccp(0, maxHeight-totalHeight));
        }else{
            m_subNode->addChild(label);
            label->setAnchorPoint(ccp(0, 1));
            label->setPosition(ccp(0,maxHeight));
        }
        auto iconSize = m_sprIconBG->getContentSize();
        iconSize.width -= 20;
        iconSize.height -= 20;
        float scale = 1;
        if(color>=0){
            auto picBG = CCLoadSprite::createSprite(CCCommonUtils::getToolBgByColor(color).c_str());
            auto picSize = picBG->getContentSize();
            float scale = MIN(iconSize.width/picSize.width,iconSize.height/picSize.height);
            picBG->setScale(scale);
            m_nodeIcon->addChild(picBG);
            picBG->setPosition(m_sprIconBG->getPosition());
        }
        auto pic = CCLoadSprite::createSprite(url,true,CCLoadSpriteType_GOODS);
        auto picSize = pic->getContentSize();
        scale = MIN(iconSize.width/picSize.width,iconSize.height/picSize.height);
        pic->setScale(scale);
        m_nodeIcon->addChild(pic);
        pic->setPosition(m_sprIconBG->getPosition());
        m_lblTitle->setString(title);
        m_lblDesc->setString(_lang("111730"));
        m_btnOk->setTouchPriority(1);
        
        m_costNode->removeAllChildren();
        if (priceType == -1) {
            auto costIcon = CCLoadSprite::createSprite("Contribution_icon1.png");
            costIcon->setScale(0.7);
            if (costIcon) {
                m_costNode->addChild(costIcon);
            }
            int dy = 14;
            if (CCCommonUtils::isIosAndroidPad())
            {
                dy = 24;
            }
            m_costNode->setPositionY(m_costNode->getPositionY() - dy - 5);
            m_costNum->setPositionY(m_costNum->getPositionY() - dy);
            auto btnLabel = CCLabelIF::create();
            btnLabel->setString(_lang("102148"));
            btnLabel->setColor(ccWHITE);
            btnLabel->setFontSize(24);
            if (CCCommonUtils::isIosAndroidPad())
            {
                btnLabel->setFontSize(48);
            }
            btnLabel->setAnchorPoint(ccp(0.5, 0.5));
            btnLabel->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
            btnLabel->setHorizontalAlignment(kCCTextAlignmentCenter);
            m_costNode->getParent()->addChild(btnLabel);
            btnLabel->setPositionY(btnLabel->getPositionY() + dy);
            btnLabel->setZOrder(1);
            m_costNode->setZOrder(2);
        }
        else if(priceType < WorldResource_Max){
            auto costIcon = CCLoadSprite::createSprite(CCCommonUtils::getResourceIconByType(priceType).c_str());
            if(costIcon){
                m_costNode->addChild(costIcon);
            }
        }else{
            CCNode *priceNode = CCNode::create();
            auto priceSpr = CCLoadSprite::createSprite("Items_icon_kuang2.png");
            priceNode->addChild(priceSpr);
            auto priceSize = priceSpr->getContentSize();
            CCCommonUtils::createGoodsIcon(priceType, priceNode,priceSize-CCSize(20, 20));
            float scale = 38.0f/priceSize.width;
            priceNode->setScale(scale);
            priceNode->setPositionY(-5);
            m_costNode->addChild(priceNode);
        }
        
        m_sprBG->setVisible(false);
        
        m_price = price;
        m_priceType = priceType;
        
        setCostString();
        bRet=true;
    }
    return bRet;
}
void LongJingBuyView::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LongJingBuyView::removeWaitInter), MSG_ALLIANCE_BUY_FAIL, NULL);
    if (getParent() && (getParent()->getChildByTag(BUILDINGVIEW_TAG) || getParent()->getChildByTag(YESNODLG_TAG))){
    }else{
        setTag(YESNODLG_TAG);
    }
    showDialog();
}
void LongJingBuyView::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ALLIANCE_BUY_FAIL);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BUY_CONFIRM_OK_WITHOUT_TWEEN);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BUY_CONFIRM_OK);
    setTouchEnabled(false);
   
    if(m_waitInterface){
        m_waitInterface->remove();
        m_waitInterface=NULL;
    }
    CCNode::onExit();
}

void LongJingBuyView::setLimitMsg(int equipId)
{
    m_limitLabel->setString("");
    m_btnOk->setEnabled(true);
    auto& info = EquipmentController::getInstance()->EquipmentInfoMap[equipId];
    if (info.prv_level) {
        auto& needEquip = EquipmentController::getInstance()->EquipmentInfoMap[info.prv_level];
        string colorName = CCCommonUtils::getColorName(needEquip.color);
        string equipName = _lang(needEquip.name);
        m_limitLabel->setString(_lang_2("111688", colorName.c_str(), equipName.c_str()));
        if ( EquipmentController::getInstance()->IsHaveEquipById(info.prv_level) != "" ) {
            m_limitLabel->setColor(ccGREEN);
            m_btnOk->setEnabled(true);
        }else {
            m_limitLabel->setColor(ccRED);
            m_btnOk->setEnabled(false);
        }
    }
    
    m_subNode->removeAllChildren();
    string strEquip = CC_ITOA(equipId);
    string show1 = CCCommonUtils::getPropById(strEquip, "show1");
    string num1 = CCCommonUtils::getPropById(strEquip, "num1");
    string para1 = CCCommonUtils::getPropById(strEquip, "para1");
    string dialog_initial = CCCommonUtils::getPropById(strEquip, "dialog_initial");
    string num_initial = CCCommonUtils::getPropById(strEquip, "num_initial");
    string ran_initial = CCCommonUtils::getPropById(strEquip, "ran_initial");
    
    vector<string> sVec;
    CCCommonUtils::splitString(show1, "|", sVec);
    vector<string> nVec;
    CCCommonUtils::splitString(num1, "|", nVec);
    vector<string> pVec;
    CCCommonUtils::splitString(para1, "|", pVec);
    vector<string> extSVec;
    CCCommonUtils::splitString(dialog_initial, "|", extSVec);
    vector<string> extNVec;
    CCCommonUtils::splitString(num_initial, "|", extNVec);
    vector<string> extPVec;
    CCCommonUtils::splitString(ran_initial, "|", extPVec);
    
    m_desPara1->setString("");
    m_desPara2->setString("");
    m_desPara3->setString("");
    m_desPara4->setString("");
    m_desPara5->setString("");
    m_desNum1->setString("");
    m_desNum2->setString("");
    m_desNum3->setString("");
    m_desNum4->setString("");
    m_desNum5->setString("");
    m_desPara1->setColor(r1_color);
    m_desPara2->setColor(r1_color);
    m_desPara3->setColor(r1_color);
    m_desPara4->setColor(r1_color);
    m_desPara5->setColor(r1_color);
    m_desNum1->setColor(r2_color);
    m_desNum2->setColor(r2_color);
    m_desNum3->setColor(r2_color);
    m_desNum4->setColor(r2_color);
    m_desNum5->setColor(r2_color);
    
    int tmpY = 0;
    int paraH = 30;
    float scp = 1.0;
    if (sVec.size()==1 && nVec.size()==1 && pVec.size()==1)
    {
        string format1 = "%";
        string pm1 = "+";
        vector<string> tmpVec1;
        CCCommonUtils::splitString(pVec[0], "@", tmpVec1);
        if (tmpVec1.size()>0) {
            format1 = CCCommonUtils::getEffFormat(tmpVec1[0]);
            pm1 = CCCommonUtils::getEffPM(tmpVec1[0]);
        }
        
        m_desPara1->setString(_lang(sVec[0].c_str()));
        m_desNum1->setString( pm1+nVec[0]+format1);
        if (extSVec.size()==1 && extNVec.size()==1 && extPVec.size()==1) {
            string fEx1 = "%";
            string exPm1 = "+";
            vector<string> tpVec1;
            CCCommonUtils::splitString(extPVec[0], "@", tpVec1);
            if (tpVec1.size()>0) {
                fEx1 = CCCommonUtils::getEffFormat(tpVec1[0]);
                exPm1 = CCCommonUtils::getEffPM(tpVec1[0]);
            }
            m_desPara2->setString(_lang(extSVec[0]).c_str());
            m_desNum2->setString( exPm1+extNVec[0]+fEx1 );
            m_desPara2->setColor(r3_color);
            m_desNum2->setColor(r3_color);
        }
    }
    else if (sVec.size()==2 && nVec.size()==2 && pVec.size()==2)
    {
        string format1 = "%";
        string pm1 = "+";
        vector<string> tmpVec1;
        CCCommonUtils::splitString(pVec[0], "@", tmpVec1);
        if (tmpVec1.size()>0) {
            format1 = CCCommonUtils::getEffFormat(tmpVec1[0]);
            pm1 = CCCommonUtils::getEffPM(tmpVec1[0]);
        }
        string format2 = "%";
        string pm2 = "+";
        vector<string> tmpVec2;
        CCCommonUtils::splitString(pVec[1], "@", tmpVec2);
        if (tmpVec2.size()>0) {
            format2 = CCCommonUtils::getEffFormat(tmpVec2[0]);
            pm2 = CCCommonUtils::getEffPM(tmpVec2[0]);
        }
        
        m_desPara1->setString(_lang(sVec[0].c_str()));
        m_desNum1->setString( pm1+nVec[0]+format1);
        int h1 = m_desPara1->getContentSize().height*m_desPara1->getOriginScaleY()*scp;
        if (h1 > paraH) {
            tmpY += h1-paraH;
            m_para2Node->setPositionY(m_para2Node->getPositionY()-tmpY);
        }
        
        m_desPara2->setString(_lang(sVec[1].c_str()));
        m_desNum2->setString( pm2+nVec[1]+format2);
        int h2 = m_desPara2->getContentSize().height*m_desPara2->getOriginScaleY()*scp;
        if (h2 > paraH) {
            tmpY += h2-paraH;
            m_para3Node->setPositionY(m_para3Node->getPositionY()-tmpY);
        }
        
        if (extSVec.size()>=1 && extNVec.size()>=1 && extPVec.size()>=1) {
            string fEx1 = "%";
            string exPm1 = "+";
            vector<string> tpVec1;
            CCCommonUtils::splitString(extPVec[0], "@", tpVec1);
            if (tpVec1.size()>0) {
                fEx1 = CCCommonUtils::getEffFormat(tpVec1[0]);
                exPm1 = CCCommonUtils::getEffPM(tpVec1[0]);
            }
            m_desPara3->setString(_lang(extSVec[0]).c_str());
            m_desNum3->setString( exPm1+extNVec[0]+fEx1 );
            m_desPara3->setColor(r3_color);
            m_desNum3->setColor(r3_color);
            int h3 = m_desPara3->getContentSize().height*m_desPara3->getOriginScaleY()*scp;
            if (h3 > paraH) {
                tmpY += h3-paraH;
                m_para4Node->setPositionY(m_para4Node->getPositionY()-tmpY);
            }
        }
        if (extSVec.size()>=2 && extNVec.size()>=2 && extPVec.size()>=2) {
            string fEx1 = "%";
            string exPm1 = "+";
            vector<string> tpVec1;
            CCCommonUtils::splitString(extPVec[1], "@", tpVec1);
            if (tpVec1.size()>0) {
                fEx1 = CCCommonUtils::getEffFormat(tpVec1[0]);
                exPm1 = CCCommonUtils::getEffPM(tpVec1[0]);
            }
            m_desPara4->setString(_lang(extSVec[1]).c_str());
            m_desNum4->setString( exPm1+extNVec[1]+fEx1 );
            m_desPara4->setColor(r3_color);
            m_desNum4->setColor(r3_color);
        }
    }
    else if (sVec.size()==3 && nVec.size()==3 && pVec.size()==3)
    {
        string format1 = "%";
        string pm1 = "+";
        vector<string> tmpVec1;
        CCCommonUtils::splitString(pVec[0], "@", tmpVec1);
        if (tmpVec1.size()>0) {
            format1 = CCCommonUtils::getEffFormat(tmpVec1[0]);
            pm1 = CCCommonUtils::getEffPM(tmpVec1[0]);
        }
        string format2 = "%";
        string pm2 = "+";
        vector<string> tmpVec2;
        CCCommonUtils::splitString(pVec[1], "@", tmpVec2);
        if (tmpVec2.size()>0) {
            format2 = CCCommonUtils::getEffFormat(tmpVec2[0]);
            pm2 = CCCommonUtils::getEffPM(tmpVec2[0]);
        }
        string format3 = "%";
        string pm3 = "+";
        vector<string> tmpVec3;
        CCCommonUtils::splitString(pVec[2], "@", tmpVec3);
        if (tmpVec3.size()>0) {
            format3 = CCCommonUtils::getEffFormat(tmpVec3[0]);
            pm3 = CCCommonUtils::getEffPM(tmpVec3[0]);
        }
        m_desPara1->setString(_lang(sVec[0].c_str()));
        m_desNum1->setString( pm1+nVec[0]+format1);
        int h1 = m_desPara1->getContentSize().height*m_desPara1->getOriginScaleY()*scp;
        if (h1 > paraH) {
            tmpY += h1-paraH;
            m_para2Node->setPositionY(m_para2Node->getPositionY()-tmpY);
        }
        m_desPara2->setString(_lang(sVec[1].c_str()));
        m_desNum2->setString( pm2+nVec[1]+format2);
        int h2 = m_desPara2->getContentSize().height*m_desPara2->getOriginScaleY()*scp;
        if (h2 > paraH) {
            tmpY += h2-paraH;
            m_para3Node->setPositionY(m_para3Node->getPositionY()-tmpY);
        }
        m_desPara3->setString(_lang(sVec[2].c_str()));
        m_desNum3->setString( pm3+nVec[2]+format3);
        int h3 = m_desPara3->getContentSize().height*m_desPara3->getOriginScaleY()*scp;
        if (h3 > paraH) {
            tmpY += h3-paraH;
            m_para4Node->setPositionY(m_para4Node->getPositionY()-tmpY);
        }
        if (extSVec.size()>=1 && extNVec.size()>=1 && extPVec.size()>=1) {
            string fEx1 = "%";
            string exPm1 = "+";
            vector<string> tpVec1;
            CCCommonUtils::splitString(extPVec[0], "@", tpVec1);
            if (tpVec1.size()>0) {
                fEx1 = CCCommonUtils::getEffFormat(tpVec1[0]);
                exPm1 = CCCommonUtils::getEffPM(tpVec1[0]);
            }
            m_desPara4->setString(_lang(extSVec[0]).c_str());
            m_desNum4->setString( exPm1+extNVec[0]+fEx1 );
            m_desPara4->setColor(r3_color);
            m_desNum4->setColor(r3_color);
            int h4 = m_desPara4->getContentSize().height*m_desPara4->getOriginScaleY()*scp;
            if (h4 > paraH) {
                tmpY += h4-paraH;
                m_para5Node->setPositionY(m_para5Node->getPositionY()-tmpY);
            }
        }
        if (extSVec.size()>=2 && extNVec.size()>=2 && extPVec.size()>=2) {
            string fEx1 = "%";
            string exPm1 = "+";
            vector<string> tpVec1;
            CCCommonUtils::splitString(extPVec[1], "@", tpVec1);
            if (tpVec1.size()>0) {
                fEx1 = CCCommonUtils::getEffFormat(tpVec1[0]);
                exPm1 = CCCommonUtils::getEffPM(tpVec1[0]);
            }
            m_desPara5->setString(_lang(extSVec[1]).c_str());
            m_desNum5->setString( exPm1+extNVec[1]+fEx1 );
            m_desPara5->setColor(r3_color);
            m_desNum5->setColor(r3_color);
        }
    }
}

void LongJingBuyView::removeWaitInter(CCObject* obj)
{
    if (m_waitInterface) {
        m_waitInterface->remove();
        m_waitInterface = nullptr;
    }
}
void LongJingBuyView::showDialog(){
    CCPoint endPos = m_nodeIcon->getPosition();
    float endScale = m_nodeIcon->getScale();
    CCSize nodeSize = CCSizeMake(150, 150);
    m_nodeIcon->setScale(0.8);
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_nodeIcon->setScale(0.8 * endScale);
    }
    m_startPos = m_nodeIcon->getParent()->convertToNodeSpace(m_startPos);
    if (m_priceType != -1) {
        m_startPos.x += nodeSize.width * 0.4;
        m_startPos.y += nodeSize.height * 0.4;
    }
    m_nodeIcon->setPosition(m_startPos);
    m_nodeIcon->runAction(CCSequence::create(CCEaseExponentialOut::create(CCMoveTo::create(0.5, endPos)),NULL));
    m_nodeIcon->runAction(CCSequence::create(CCScaleTo::create(0.25, endScale),NULL));
    m_nodeIcon->runAction(CCSequence::create(CCDelayTime::create(0.15),CCCallFunc::create(this, callfunc_selector(LongJingBuyView::showBG)),NULL));
}
void LongJingBuyView::showBG(){
    m_sprBG->setVisible(true);
    m_sprBG->runAction(CCSequence::create(CCFadeIn::create(0.1),NULL));
}
void LongJingBuyView::closeDialog(){
    if(m_sprBG){
        m_sprBG->stopAllActions();
        m_nodeIcon->stopAllActions();
        
        if(m_onClose){
            m_onClose->execute();
            m_onClose = NULL;
        }
        closeSelf();
    }
}
bool LongJingBuyView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isTouchInside(m_touchNode, pTouch)) {
        return false;
    }
    m_sprBG->getParent()->runAction(CCSequence::create(CCScaleTo::create(0.25, 0),CCCallFunc::create(this, callfunc_selector(LongJingBuyView::closeDialog)),NULL));
//    m_nodeIcon->runAction(CCSequence::create(CCScaleTo::create(0.25, 0),NULL));
    return true;
}

void LongJingBuyView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    
}
#pragma mark -
#pragma mark CocosBuilder Part
SEL_MenuHandler LongJingBuyView::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler LongJingBuyView::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "keyPressedBtnOk", LongJingBuyView::keypressedBtnOk);
    return NULL;
}
bool LongJingBuyView::onAssignCCBMemberVariable(CCObject * pTarget, const char* pMemberVariableName, CCNode * pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnOk", CCControlButton*, this->m_btnOk);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_costNum", CCLabelIF*, this->m_costNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_costNode", CCNode*, this->m_costNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeIcon", CCNode*, this->m_nodeIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprIconBG", CCSprite*, this->m_sprIconBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblDesc", CCLabelIF*, this->m_lblDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_limitLabel", CCLabelIF*, this->m_limitLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subNode", CCNode*, this->m_subNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblTitle", CCLabelIF*, this->m_lblTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprBG", CCNode*, this->m_sprBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNum1", CCLabelIF*, this->m_desNum1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNum2", CCLabelIF*, this->m_desNum2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNum3", CCLabelIF*, this->m_desNum3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNum4", CCLabelIF*, this->m_desNum4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNum5", CCLabelIF*, this->m_desNum5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desPara1", CCLabelIF*, this->m_desPara1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desPara2", CCLabelIF*, this->m_desPara2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desPara3", CCLabelIF*, this->m_desPara3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desPara4", CCLabelIF*, this->m_desPara4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desPara5", CCLabelIF*, this->m_desPara5);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_para1Node", CCNode*, this->m_para1Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_para2Node", CCNode*, this->m_para2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_para3Node", CCNode*, this->m_para3Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_para4Node", CCNode*, this->m_para4Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_para5Node", CCNode*, this->m_para5Node);
    return false;
}

#pragma mark -
#pragma mark Control Events
void LongJingBuyView::onOkBuyResource(){
    if(m_priceType < WorldResource_Max && m_priceType >= 0){
        if(m_priceType == Gold){
//            PopupViewController::getInstance()->addPopupView(GoldExchangeView::create(),false);
            PopupViewController::getInstance()->addPopupInView(GoldExchangeView_NEW::create());
        }else{
            PopupViewController::getInstance()->addPopupInView(UseResToolView::create(m_priceType));
        }
    }
}
void LongJingBuyView::keypressedBtnOk(CCObject * pSender, CCControlEvent pCCControlEvent) {
    if(m_waitInterface){
        return;
    }
    CCLOG("---------keypressedBtnOk--------------");
    int price = m_price * m_numCurrent;
    if(m_priceType == -1) {
        if(GlobalData::shared()->playerInfo.allianceInfo.alliancepoint < price){
            CCCommonUtils::flyHint("", "", _lang("115827"));
            return;
        }
    }
    else if(m_priceType<WorldResource_Max){
        if (!CCCommonUtils::isEnoughResourceByType(m_priceType,price)) {
            if(m_priceType < WorldResource_Max && m_priceType >= 0){
                if(m_priceType == Gold){
                   
                    YesNoDialog::gotoPayTips();
                    if(m_onClose){
                        m_onClose->execute();
                        m_onClose = NULL;
                    }
                     this->closeSelf();
                }else if (m_priceType == LongJing){
                    std::string res =  CCCommonUtils::getResourceNameByType(m_priceType);
                    std::string showString = _lang_1("111656",res.c_str());
                    YesNoDialog *dialog = YesNoDialog::showYesDialog(showString.c_str(),false,NULL,false);
                    dialog->setYesButtonTitle(_lang("101274").c_str());//确定
                    closeDialog();
                }
                else{
                    std::string res =  CCCommonUtils::getResourceNameByType(m_priceType);
                    std::string showString = _lang_2("104943",res.c_str(), res.c_str());
                    YesNoDialog *dialog = YesNoDialog::showYesDialog(showString.c_str(),false,CCCallFunc::create(this, callfunc_selector(LongJingBuyView::onOkBuyResource)),false);
                    dialog->setYesButtonTitle(_lang_1("104944",res.c_str()));
                    closeDialog();
                }
            }
            return;
        }
        
    }else{
        auto &tInfo = ToolController::getInstance()->getToolInfoById(m_priceType);
        if(tInfo.getCNT() < price){
            YesNoDialog::showYesDialog(_lang_2("104957", tInfo.getName().c_str(), m_lblTitle->getString().c_str()));
            closeDialog();
            return;
        }
    }
    
    if(m_numCurrent>1){
        std::string des = m_lblTitle->getString() + std::string(" x ") + CC_ITOA(m_numCurrent);
        std::string showString = _lang_1("104954", des.c_str());
        if (m_priceType == -1) {
            showString = _lang_1("115817", des.c_str());
        }
        YesNoDialog::showYesDialog(showString.c_str(),false,CCCallFunc::create(this, callfunc_selector(LongJingBuyView::onOKBuy)),false);
    }else{
        onOKBuy();
    }
}
void LongJingBuyView::onOKBuy(){
    if (m_onYes) {
        m_waitInterface=GameController::getInstance()->showWaitInterface(this);
        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LongJingBuyView::playBuyEffect), MSG_BUY_CONFIRM_OK, NULL);
        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LongJingBuyView::playBuyWithoutEffect), MSG_BUY_CONFIRM_OK_WITHOUT_TWEEN, NULL);
        
        m_onYes->setObject(CCInteger::create(m_numCurrent));
        m_onYes->execute();
    }
}
void LongJingBuyView::playBuyWithoutEffect(CCObject *ccObj){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BUY_CONFIRM_OK_WITHOUT_TWEEN);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BUY_CONFIRM_OK);
    if(m_waitInterface){
        m_waitInterface->remove();
        m_waitInterface=NULL;
    }
    hideEffect();
}
void LongJingBuyView::playBuyEffect(CCObject *ccObj){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BUY_CONFIRM_OK_WITHOUT_TWEEN);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BUY_CONFIRM_OK);
    if(m_waitInterface){
        m_waitInterface->remove();
        m_waitInterface=NULL;
    }
    if(!m_sprBG){
        hideEffect();
        return;
    }
    m_sprBG->setVisible(false);
    CCPoint centrePos = m_sprBG->getPosition();
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_nodeIcon->runAction(CCSequence::create(CCScaleTo::create(1.4, 1 * 1.9),NULL));
    } else {
        m_nodeIcon->runAction(CCSequence::create(CCScaleTo::create(1.4, 1),NULL));
    }
    m_nodeIcon->runAction(CCSequence::create(CCEaseExponentialOut::create(CCMoveTo::create(1, centrePos)),NULL));
    m_nodeIcon->runAction(CCSequence::create(CCDelayTime::create(0.8),CCCallFunc::create(this, callfunc_selector(LongJingBuyView::showEffect)),NULL));
    m_nodeIcon->runAction(CCSequence::create(CCDelayTime::create(2.5),CCCallFunc::create(this, callfunc_selector(LongJingBuyView::flyIcon)),NULL));
    
    
}
void LongJingBuyView::showEffect(){
    SoundController::sharedSound()->playEffects(Music_Sfx_UI_secret);
    CCPoint centrePos = m_sprBG->getPosition();
    for (int i=1; i<=7; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("MallBag_%d",i)->getCString());
        particle->setPosition(centrePos);
        m_sprBG->getParent()->addChild(particle,m_nodeIcon->getZOrder()-1);
    }

}
void LongJingBuyView::flyIcon(){
    m_nodeIcon->runAction(CCSequence::create(CCScaleTo::create(0.3, 0),CCCallFunc::create(this, callfunc_selector(LongJingBuyView::hideEffect)),NULL));
}
void LongJingBuyView::hideEffect(){
        closeDialog();
}

void LongJingBuyView::setCostString(){
    int costVal = m_price * m_numCurrent;
    m_costNum->setString(CC_CMDITOA(costVal));
    if (m_priceType == -1) {
        if (GlobalData::shared()->playerInfo.allianceInfo.alliancepoint < costVal) {
            m_costNum->setColor(ccRED);
        } else {
            m_costNum->setColor({255, 225, 0});
        }
    }
    else if(m_priceType<WorldResource_Max){
        if (!CCCommonUtils::isEnoughResourceByType(m_priceType,costVal)){
            m_costNum->setColor(ccRED);
        }else{
            m_costNum->setColor(ccWHITE);
        }
    }else{
        auto &info = ToolController::getInstance()->getToolInfoById(m_priceType);
        if (info.getCNT()<costVal) {
            m_costNum->setColor(ccRED);
        }else{
            m_costNum->setColor(ccWHITE);
        }
    }
}
