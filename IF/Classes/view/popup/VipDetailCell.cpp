//
//  VipDetailCell.cpp
//  IF
//
//  Created by chenliang on 14-3-18.
//
//

#include "VipDetailCell.h"
#include "SoundController.h"
#include "PopupViewController.h"
#include "UseToolView.h"
#include "VipConstants.h"
#include "VipUtil.h"


const float VipDetailCell::CELL_WIDTH = 320;
const float VipDetailCell::CELL_HEIGHT = 428;

static const float INFO_AREA_HEIGHT = 287;

static const int ATTRIBUTE_COUNT = 14;

static const int CONTAINER_TAG = 999;

VipDetailCell* VipDetailCell::create(int index, int type/* = 0*/, int VIP_type/* = 1*/)
{
    auto ret = new VipDetailCell(index, type, VIP_type);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool VipDetailCell::init()
{
    bool ret = true;
   // CCBLoadFile("VipDetailCell",this,this);
    
    m_listNode = CCNode::create();
    this->addChild(m_listNode);
//    m_addVipPointsBtnLabel->setString(_lang("103003").c_str());
//    m_activateVipBtnLabel->setString(_lang("103004").c_str());
//    
//    m_infoScrollView = CCScrollView::create();
//    m_infoScrollView->setDirection(kCCScrollViewDirectionVertical);
//    m_infoScrollView->setPosition(CCPoint(0, 95));
//    m_infoScrollView->setAnchorPoint(CCPoint(0,0));
//    m_infoScrollView->setViewSize(CCSize(CELL_WIDTH, 287));
//    addChild(m_infoScrollView);
    setData(m_index);
    return ret;
}

void VipDetailCell::onEnter() {
    CCNode::onEnter();
}

void VipDetailCell::onExit() {
        CCNode::onExit();
}

SEL_CCControlHandler VipDetailCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    
    return NULL;
}

float VipDetailCell::getHG(){

    return this->getContentSize().height;
}

void VipDetailCell::setData(int index)
{
    int j = 0;
    if(m_VIPtype==1){//vip
        int itemID = 7000+index;
        int nextItemID = 7000+index+1;
        string a[10]={"hurt","attack","defense","health","troop","march","monster","body","speedup","wood"};
        int length = 10;
        for (int i =0; i<length; i++) {
            string key = a[i];
            bool isCreateCell = true;
            int value = VipUtil::getVipItemValueByKey(CC_ITOA(itemID), key);
            int nextvalue = VipUtil::getVipItemValueByKey(CC_ITOA(nextItemID), key);
            if(nextvalue<=0){
                isCreateCell = false;
            }
            if(isCreateCell){
                VipEffectCell* cell = VipEffectCell::create(key,value,key,nextvalue,m_type);
                m_listNode->addChild(cell);
                float height = cell->getHG();
                cell->setPositionY(-height*j);
                j++;
            }
        }
    }else if (m_VIPtype==2){//svip
        int itemID = 7010+index;
        int nextItemID = 7010+index+1;
//        int length = 20;
//        string a[20]={"equip","mass","health","rob","infantry","bow","cart","cavalry","fight","production","superpower","battle","autorepair","autogain","autoarmy","doubleskill","svipmall","svipemotion","sviptalk","svipshow"};

        
        string XMLDisplay = CCCommonUtils::getPropById("7099", "display");
        vector<string> vec;
        CCCommonUtils::splitString(XMLDisplay, "|", vec);
        
        for (int i =0; i<vec.size(); i++) {
            string key = vec[i];
            VipEffectSVIPCell* cell = VipEffectSVIPCell::create(key,itemID,nextItemID);
            m_listNode->addChild(cell);
            float height = cell->getHG();
            cell->setPositionY(-height*j);
            j++;
        }
    }
    
    
    m_listNode->setPositionY(135*j);
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_listNode->setPositionY(276 * j);
    }
    this->setContentSize(CCSize(614, 135*j));
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCSize(1470, 276 * j));
    }


}


bool VipDetailCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftText", CCLabelIF*, m_leftText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightText", CCLabelIF*, m_rightText);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftPicNode", CCNode*, m_leftPicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightPicNode", CCNode*, m_rightPicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftPicNode", CCNode*, m_leftPicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightPicNode", CCNode*, m_rightPicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, m_bg);
    
    return false;
}


VipEffectCell* VipEffectCell::create(string type1,int value1,string type2,int value2,int guide)
{
    auto ret = new VipEffectCell(type1,value1,type2,value2,guide);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool VipEffectCell::init()
{
    bool ret = true;
    auto bg = CCBLoadFile("VipEffectCell",this,this);
    this->setContentSize(bg->getContentSize());
    setData(m_type1,m_value1,m_type2,m_value2);
    return ret;
}

void VipEffectCell::onEnter() {
    CCNode::onEnter();
}

void VipEffectCell::onExit() {
        CCNode::onExit();
}

SEL_CCControlHandler VipEffectCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    
    return NULL;
}

float VipEffectCell::getHG(){
    
    return this->getContentSize().height;
}

void VipEffectCell::setData(string type1,int value1,string type2,int value2)
{
    string pic = "";
    if (type1=="speedup") {
//        this->m_showNode->setVisible(true);
//        this->m_hideNode->setVisible(false);
        m_leftText->setString(_lang_1("103034",""));
        m_rightText->setString(_lang_1("103034",""));
        pic = "skill2_650403.png";
    }else if (type1=="wood") {
        m_leftText->setString(_lang_1("103035",""));
        m_rightText->setString(_lang_1("103035",""));
        pic = "item603.png";
    }else if (type1=="food") {
        m_leftText->setString(_lang_1("103035",""));
        m_rightText->setString(_lang_1("103035",""));
        pic = "item603.png";
    }else if (type1=="iron") {
        m_leftText->setString(_lang_1("103035",""));
        m_rightText->setString(_lang_1("103035",""));
        pic = "item603.png";
    }else if (type1=="silver") {
        m_leftText->setString(_lang_1("103035",""));
        m_rightText->setString(_lang_1("103035",""));
        pic = "item603.png";
    }else if (type1=="attack") {
        m_leftText->setString(_lang_1("103037",""));
        m_rightText->setString(_lang_1("103037",""));
        pic = "skill2_606201.png";
    }else if (type1=="defense") {
        m_leftText->setString(_lang_1("103038",""));
        m_rightText->setString(_lang_1("103038",""));
        pic = "skill2_606202.png";
    }else if (type1=="march") {
        m_leftText->setString(_lang_1("103036",""));
        m_rightText->setString(_lang_1("103036",""));
        pic = "skill2_650000.png";
    }else if (type1=="health") {
        m_leftText->setString(_lang_1("103039",""));
        m_rightText->setString(_lang_1("103039",""));
        pic = "skill2_652205.png";
    }else if (type1=="hurt") {
        m_leftText->setString(_lang_1("103040",""));
        m_rightText->setString(_lang_1("103040",""));
        pic = "skill2_650205.png";
    }else if (type1=="troop") {
        m_leftText->setString(_lang_1("103041",""));
        m_rightText->setString(_lang_1("103041",""));
        pic = "skill2_603103.png";
    }else if (type1=="monster") {
        m_leftText->setString(_lang_1("103042",""));
        m_rightText->setString(_lang_1("103042",""));
        pic = "skill2_650400.png";
    }else if (type1=="body") {
        m_leftText->setString(_lang_1("103043",""));
        m_rightText->setString(_lang_1("103043",""));
        pic = "skill2_650405.png";
    }
    if (value1 == value2)
    {
        m_rightAddFlag->setVisible(false);
    }
    CCSprite* head = CCLoadSprite::createSprite(pic.c_str());
    CCSprite* head1 = CCLoadSprite::createSprite(pic.c_str());
    m_leftPicNode->addChild(head);
    m_rightPicNode->addChild(head1);
    float scale = 110/head->getContentSize().width;
    //scale = scale>1.0?1.0:scale;
    head->setScale(scale);
    head1->setScale(scale);
    if(value1==0){
        head->setColor({90,85,81});
        m_leftText->setColor({90,85,81});
        m_leftValueText->setColor({173,44,35});
    }
    string value1str = "";

    string value2str = "";
    if (type1=="troop") {
        value1str.append("+");
        value1str.append(CC_ITOA(value1));
        value2str.append("+");
        value2str.append(CC_ITOA(value2));
        m_leftValueText->setString(value1str);
        m_rightValueText->setString(value2str);
        
    }else if (type1=="speedup") {
        
        value1str.append(CC_ITOA(value1));
        value1str.append(_lang("103033"));
        value2str.append(CC_ITOA(value2));
        value2str.append(_lang("103033"));
        m_leftValueText->setString(value1str);
        m_rightValueText->setString(value2str);
        
    }else{
        value1str.append("+");
        value1str.append(CC_ITOA(value1));
        value1str.append("%");
        
        value2str.append("+");
        value2str.append(CC_ITOA(value2));
        value2str.append("%");
       
        m_leftValueText->setString(value1str);
        m_rightValueText->setString(value2str);
    }
    if (m_guide == 1 && type1=="troop") {
        setGuideAnim();
    }
}
void VipEffectCell::setGuideAnim(){
    auto sprite = CCLoadSprite::createScale9Sprite("rect.png");
    m_activeGuideNode->addChild(sprite);
    sprite->setContentSize(this->m_activeGuideNode->getContentSize());
    sprite->setPosition(this->m_activeGuideNode->getContentSize().width / 2, this->m_activeGuideNode->getContentSize().height / 2);
    sprite->setOpacity(0);
    CCSequence *seque = CCSequence::create(CCFadeIn::create(0.3), CCFadeOut::create(0.3) ,NULL);
    CCSequence *seque1 = CCSequence::create(CCRepeat::create(seque, 5), CCRemoveSelf::create(), NULL);
    sprite->runAction(seque1);
}
bool VipEffectCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightValueText", CCLabelIF*, m_rightValueText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftValueText", CCLabelIF*, m_leftValueText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftText", CCLabelIF*, m_leftText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightText", CCLabelIF*, m_rightText);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftPicNode", CCNode*, m_leftPicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightPicNode", CCNode*, m_rightPicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_showNode1", CCNode*, m_showNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_showNode2", CCNode*, m_showNode2);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightAddFlag", CCSprite*, m_rightAddFlag);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_activeGuideNode", Node*, m_activeGuideNode);
    return false;
}

#pragma mark VipEffectSVIPCell
VipEffectSVIPCell* VipEffectSVIPCell::create(string type,int itemID,int nextItemID)
{
    auto ret = new VipEffectSVIPCell(type,itemID,nextItemID);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool VipEffectSVIPCell::init()
{
    bool ret = true;
    auto bg = CCBLoadFile("SvipEffectCell",this,this);
    this->setContentSize(bg->getContentSize());
    setData();
    return ret;
}

void VipEffectSVIPCell::onEnter() {
    CCNode::onEnter();
}

void VipEffectSVIPCell::onExit() {
    CCNode::onExit();
}

SEL_CCControlHandler VipEffectSVIPCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    
    return NULL;
}

float VipEffectSVIPCell::getHG(){
    
    return this->getContentSize().height;
}

void VipEffectSVIPCell::setData()
{
    string pic = CCCommonUtils::getPropById("7100", m_type);
    string desStr = CCCommonUtils::getPropById("7099", m_type);
    m_leftText->setString(_lang(desStr));
    m_rightText->setString(_lang(desStr));
    
    string value1str = "";
    string value2str = "";
    int value1=1;
    int value2=1;
    string valStr1 = CCCommonUtils::getPropById(CC_ITOA(m_itemID), m_type);
    vector<string> vec;
    if (valStr1!="") {
        CCCommonUtils::splitString(valStr1, "|", vec);
        if (vec.size()==1) {
            value1str = valStr1;
            if(m_itemID==7009 && m_type=="health"){
                value1str = valStr1+"%";
            }
        }else if (vec.size()==2){
            if (vec[0]=="0") {
                value1str = _lang(vec[1]);
                if(vec[1]=="103077"){//103077=即将开启！
                    value1=0;
                }
            }else{
                value1str = vec[0] + _lang(vec[1]);
            }
        }else if(vec.size()==3){
            value1str=_lang_1(vec[1].c_str(), vec[2].c_str());
        }
    }else{
        value1=0;
    }
    
    bool showRightAddFlag=true;
    valStr1 = CCCommonUtils::getPropById(CC_ITOA(m_nextItemID), m_type);
    vec.clear();
    if (valStr1!="") {
        CCCommonUtils::splitString(valStr1, "|", vec);
        if (vec.size()==1) {
            value2str = valStr1;
        }else if (vec.size()==2){
            if (vec[0]=="0") {
                value2str = _lang(vec[1]);
                if(vec[1]=="103077"){//103077=即将开启！
                    m_rightAddFlag->setVisible(false);
                    value2=0;
                }
            }else{
                value2str = vec[0] + _lang(vec[1]);
            }
        }else if(vec.size()==3){
            value2str=_lang_1(vec[1].c_str(), vec[2].c_str());
            showRightAddFlag=false;
        }
    }else{
        value2=0;
    }
    
    if (value1str == value2str || showRightAddFlag==false)
    {
        m_rightAddFlag->setVisible(false);
    }
    CCSprite* head = CCLoadSprite::createSprite(pic.c_str());
    CCSprite* head1 = CCLoadSprite::createSprite(pic.c_str());
    m_leftPicNode->addChild(head);
    m_rightPicNode->addChild(head1);
    float scale = 90/head->getContentSize().width;
    scale = scale>1.0?1.0:scale;
    head->setScale(scale);
    head1->setScale(scale);
    if(value1==0){
//        head->setColor({90,85,81});
        CCSprite* cover = CCLoadSprite::createSprite("iron_lock.png");
        //cover->setScale(1.3);
        m_leftPicNode->addChild(cover);
//        m_leftText->setColor({90,85,81});
//        m_leftValueText->setColor({173,44,35});
    }
    if(value2==0){
//        head1->setColor({90,85,81});
        CCSprite* cover = CCLoadSprite::createSprite("iron_lock.png");
       // cover->setScale(1.3);
        m_rightPicNode->addChild(cover);
//        m_rightText->setColor({90,85,81});
//        m_rightValueText->setColor({173,44,35});
    }
    
    m_leftValueText->setString(value1str);
    m_rightValueText->setString(value2str);
}

bool VipEffectSVIPCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightValueText", CCLabelIF*, m_rightValueText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftValueText", CCLabelIF*, m_leftValueText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftText", CCLabelIF*, m_leftText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightText", CCLabelIF*, m_rightText);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftPicNode", CCNode*, m_leftPicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightPicNode", CCNode*, m_rightPicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_showNode1", CCNode*, m_showNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_showNode2", CCNode*, m_showNode2);
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightAddFlag", CCSprite*, m_rightAddFlag);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_activeGuideNode", Node*, m_activeGuideNode);
    return false;
}
