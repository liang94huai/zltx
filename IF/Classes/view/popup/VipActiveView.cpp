
#include "VipActiveView.h"
#include "PopupViewController.h"
#include "UseToolView.h"
#include "ToolController.h"
#include "VipUtil.h"

#define VIP_BENEFIT_KEY_COUNT 10
#define SVIP_BENEFIT_KEY_COUNT 20
#define VIP_ACTIVE_CELL_W 500
#define VIP_ACTIVE_CELL_H 130

string VipBenefitKey[VIP_BENEFIT_KEY_COUNT]={"hurt","attack","defense","health","troop","march","monster","body","speedup","wood"};
string SvipBenefitKey[SVIP_BENEFIT_KEY_COUNT]={"equip","doubleskill","svipmall","svipemotion","sviptalk","svipshow","rob","infantry","bow","cart","cavalry","health","fight","mass","production","superpower","battle","autorepair","autogain","autoarmy"};
float VIP_ACTIVE_VALUE_POSX;
VipActiveView::VipActiveView(){
}
VipActiveView::~VipActiveView(){
    
}
VipActiveView* VipActiveView::create(){
    VipActiveView* ret = new VipActiveView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
    
}
bool VipActiveView::init(){
    if(!PopupBaseView::init())
    {
        return false;
    }
    
    CCLoadSprite::doResourceByCommonIndex(6, true);
    CCLoadSprite::doResourceByCommonIndex(105, true);
    CCLoadSprite::doResourceByCommonIndex(500, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(6, false);
        CCLoadSprite::doResourceByCommonIndex(105, false);
        CCLoadSprite::doResourceByCommonIndex(500, false);
    });
    
    auto bg = CCBLoadFile("VipActiveView", this, this,true);
    this->setContentSize(bg->getContentSize());
    
    auto &playerInfo = GlobalData::shared()->playerInfo;
    if (VipUtil::isSVIP() && playerInfo.SVIPLevel>0) {
        int svipLv = playerInfo.SVIPLevel;
        m_titleLabel1->setString(_lang_1("103076", CC_ITOA(svipLv)));//103076=SVIP{0}
        m_desLabel->setString(_lang("103104"));//103104=您的SVIP状态已失效，继续激活SVIP来享受SVIP和VIP加成！
        m_titleLabel2->setString(_lang_1("103076", CC_ITOA(svipLv)));//103076=SVIP{0}
        setButtonTitle(m_activeBtn, _lang("103098").c_str());//103098=激活SVIP
        setButtonTitle(m_cancelBtn, _lang("103054").c_str());//103054=暂不
        
        int itemID = 7010+svipLv-1;
        for (int i =0; i<SVIP_BENEFIT_KEY_COUNT; i++) {
            string key = SvipBenefitKey[i];
            int value = atoi(CCCommonUtils::getPropById(CC_ITOA(itemID), key).c_str());
            if(value>0){
                VipBenefitStruct benefit = {key,value,(int)m_data.size(),itemID};
                m_data.push_back(benefit);
            }
        }
    }else{
        int vipLv = VipUtil::getVipLevel(playerInfo.vipPoints);
        m_titleLabel1->setString(_lang_1("103001", CC_ITOA(vipLv)));//103001=VIP{0}
        m_desLabel->setString(_lang("103053"));//103053=您的VIP状态已失效，继续激活VIP来享受VIP加成！
        m_titleLabel2->setString(_lang_1("103001", CC_ITOA(vipLv)));//103001=VIP{0}
        setButtonTitle(m_activeBtn, _lang("103003").c_str());//103003=激活VIP
        setButtonTitle(m_cancelBtn, _lang("103054").c_str());//103054=暂不

        int itemID = 7000+vipLv-1;
        if(itemID<7000){
            itemID = 7000;
        }
        for (int i =0; i<VIP_BENEFIT_KEY_COUNT; i++) {
            string key = VipBenefitKey[i];
            int value = VipUtil::getVipItemValueByKey(CC_ITOA(itemID), key);
            if(value>0){
                VipBenefitStruct benefit = {key,value,(int)m_data.size()};
                m_data.push_back(benefit);
            }
        }
    }
    
    m_tableView = CCTableView::create(this, m_nodeList->getContentSize());
    m_tableView->setDelegate(this);
    m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tableView->setTouchPriority(Touch_Default);
    m_nodeList->addChild(m_tableView);
    m_tableView->reloadData();
    return true;
}

void VipActiveView::onEnter(){
    PopupBaseView::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
}
void VipActiveView::onExit(){
    PopupBaseView::onExit();
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}
bool VipActiveView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_activeBtn", CCControlButton*, this->m_activeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cancelBtn", CCControlButton*, this->m_cancelBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel1", CCLabelIF*, this->m_titleLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLabel2", CCLabelIF*, this->m_titleLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, this->m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeList", CCNode*, this->m_nodeList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nodeTouch", CCNode*, this->m_nodeTouch);
    return true;
}
SEL_CCControlHandler VipActiveView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onActiveBtnClick", VipActiveView::onActiveBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseBtnClick", VipActiveView::onCloseBtnClick);
    return NULL;
}
void VipActiveView::onActiveBtnClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    auto dict = CCDictionary::create();
    PopupViewController::getInstance()->addPopupInView(UseToolView::create(USE_TOOL_VIP_ACTIVITY,dict,"103004"));
    this->closeSelf();
}
void VipActiveView::onCloseBtnClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent){
    this->closeSelf();
}
CCSize VipActiveView::tableCellSizeForIndex(CCTableView *table, ssize_t idx) {
    return CCSize(VIP_ACTIVE_CELL_W, VIP_ACTIVE_CELL_H);
};
CCSize VipActiveView::cellSizeForTable(CCTableView *table) {
    return CCSize(VIP_ACTIVE_CELL_W, VIP_ACTIVE_CELL_H);
};

CCTableViewCell* VipActiveView::tableCellAtIndex(CCTableView *table, ssize_t idx){
    int count = m_data.size();
    if(idx >= count){
        return NULL;
    }
    VipActiveCell* cell = (VipActiveCell*)table->dequeueCell();
    if(cell){
        cell->setData(m_data.at(idx));
    }else{
        cell = VipActiveCell::create(m_data.at(idx));
    }
    return cell;
}
ssize_t VipActiveView::numberOfCellsInTableView(CCTableView *table){
    return m_data.size();
}

bool VipActiveView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return true;
}
void VipActiveView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
}
void VipActiveView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (!isTouchInside(m_nodeTouch, pTouch)) {
        closeSelf();
    }
}



/////////////////////
//
//
//
/////////////////////
VipActiveCell::VipActiveCell(){
    
}
VipActiveCell::~VipActiveCell(){
    
}
VipActiveCell* VipActiveCell::create(VipBenefitStruct &benefit){
    auto ret = new VipActiveCell();
    if (ret && ret->init(benefit)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool VipActiveCell::init(VipBenefitStruct &benefit){
    if(!CCTableViewCell::create())
        return false;
    CCBLoadFile("VipActiveCell",this,this);
    setContentSize(CCSize(VIP_ACTIVE_CELL_W, VIP_ACTIVE_CELL_H));
    setData(benefit);
    return true;
}
void VipActiveCell::onEnter() {
    CCTableViewCell::onEnter();
    
}
void VipActiveCell::onExit() {
    CCTableViewCell::onExit();
}
void VipActiveCell::setData(VipBenefitStruct &benefit){
    string pic = "";
    string value = "";
    if (benefit.key==VipBenefitKey[0]) {
        m_lblName->setString(_lang_1("103040",""));
        pic = "skill2_650205.png";
    }else if(benefit.key==VipBenefitKey[1]){
        m_lblName->setString(_lang_1("103037",""));
        pic = "skill2_606201.png";
    }else if (benefit.key==VipBenefitKey[2]) {
        m_lblName->setString(_lang_1("103038",""));
        pic = "skill2_606202.png";
    }else if (benefit.key==VipBenefitKey[3]) {
        m_lblName->setString(_lang_1("103039",""));
        pic = "skill2_652205.png";
    }else if (benefit.key==VipBenefitKey[4]) {
        m_lblName->setString(_lang_1("103041",""));
        pic = "skill2_603103.png";
        value.append("+");
        value.append(CC_ITOA(benefit.value));
    }else if (benefit.key==VipBenefitKey[5]) {
        m_lblName->setString(_lang_1("103036",""));
        pic = "skill2_650000.png";
    }else if (benefit.key==VipBenefitKey[6]) {
        m_lblName->setString(_lang_1("103042",""));
        pic = "skill2_650400.png";
    }else if (benefit.key==VipBenefitKey[7]) {
        m_lblName->setString(_lang_1("103043",""));
        pic = "skill2_650405.png";
    }else if (benefit.key==VipBenefitKey[8]) {
        m_lblName->setString(_lang_1("103034",""));
        pic = "skill2_650403.png";
        value.append(CC_ITOA(benefit.value));
        value.append(_lang("103033"));
    }else if (benefit.key==VipBenefitKey[9]) {
        m_lblName->setString(_lang_1("103035",""));
        pic = "item603.png";
    }else if (benefit.key=="food") {
        m_lblName->setString(_lang_1("103035",""));
        pic = "item603.png";
    }else if (benefit.key=="iron") {
        m_lblName->setString(_lang_1("103035",""));
        pic = "item603.png";
    }else if (benefit.key=="silver") {
        m_lblName->setString(_lang_1("103035",""));
        pic = "item603.png";
    }else{
        //SVIP
        string desStr = CCCommonUtils::getPropById("7099", benefit.key);
        m_lblName->setString(_lang(desStr));
        pic = CCCommonUtils::getPropById("7100", benefit.key);
        
        string valStr1 = CCCommonUtils::getPropById(CC_ITOA(benefit.item_id), benefit.key);
        vector<string> vec;
        if (valStr1!="") {
            CCCommonUtils::splitString(valStr1, "|", vec);
            if (vec.size()==1) {
                value = valStr1;
            }else if (vec.size()==2){
                if (vec[0]=="0") {
                    value = _lang(vec[1]);
                }else{
                    value = vec[0] + _lang(vec[1]);
                }
            }
        }
    }
    if(value.empty()){
        value.append("+");
        value.append(CC_ITOA(benefit.value));
        value.append("%");
    }
    
    CCSprite* head = CCLoadSprite::createSprite(pic.c_str());
    m_sprPic->removeAllChildren();
    m_sprPic->addChild(head);
    CCCommonUtils::setSpriteMaxSize(head, 100);
    m_lblNum->setString(value);
    float posX1 = VIP_ACTIVE_VALUE_POSX;
    float w2 = m_sprAdd->getContentSize().width;
    if(benefit.idx == 0){
        float w1 = m_lblNum->getContentSize().width * m_lblNum->getOriginScaleX();
        float w = w1 + w2 + 5;
        float posX= m_lblName->getPositionX();
        posX1 = posX-w*0.5+w2*0.5;
        VIP_ACTIVE_VALUE_POSX = posX1;
    }
    m_sprAdd->setPositionX(posX1);
    posX1 += w2*0.5 + 5;
    m_lblNum->setPositionX(posX1);
}

SEL_CCControlHandler VipActiveCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    
    return NULL;
}
bool VipActiveCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprPic", CCNode*, this->m_sprPic);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblName", CCLabelIF*, this->m_lblName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lblNum", CCLabelIF*, this->m_lblNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprAdd", CCSprite*, this->m_sprAdd);
    return false;
}