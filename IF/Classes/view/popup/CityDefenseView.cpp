//
//  CityDefenseView.cpp
//  IF
//
//  Created by fubin on 14-8-13.
//
//

#include "CityDefenseView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "FunBuildController.h"
#include "YesNoDialog.h"
#include "ParticleController.h"
#include "QueueController.h"
#include "UserUpgradeView.h"
#include "CCThreadManager.h"
#include "WorldController.h"
#include "CommandBase.h"
#include "AddDefenseCommand.h"
#include "BuyCityDefCommand.h"
#include "TipsView.h"
#include "GameController.h"
#include "ActivityController.h"
#include "IFNormalSkNode.h"

CityDefenseView* CityDefenseView::create(int buildId){
    CityDefenseView* ret = new CityDefenseView();
    if(ret && ret->init(buildId)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool CityDefenseView::init(int buildId)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(8, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(8, false);
        
    });
    m_buildId = buildId;
    m_st = 0;
    CCNode* __tmpNode = CCBLoadFile("CityDefenseView",this,this);
    
    this->setContentSize(CCDirector::sharedDirector()->getWinSize());

    changeBGHeight(m_buildBG);
    
    int addHeight = getExtendHeight();
    m_mainNode->setPositionY(m_mainNode->getPositionY()+addHeight);
    m_bNode->setPositionY(m_bNode->getPositionY()-addHeight);

    m_fullbar->setVisible(false);
    //m_defMsgLabel->setString(_lang("102307")+"\n"+_lang("102308")+"\n"+_lang("102309")+"\n"+_lang("102310"));
    m_defMsgLabel->setString(_lang("102341"));
    m_defLabel->setString(_lang("102303"));
    //CCCommonUtils::setButtonTitle(m_defBtn, _lang("102305").c_str());
    
    FunBuildInfo& m_info = FunBuildController::getInstance()->getFunbuildById(m_buildId);
    setTitleName(_lang("102206"));
    
    string str = _lang("102335");
    m_statusTxt->setString(_lang("102335"));
    double per = 0;
    if (GlobalData::shared()->alliance_territory_negative_effect > 0) {//联盟领地减益
        double effect = CCCommonUtils::getTerritoryNegativeEffectValueByNum(122) * 1.0 / 100;
        if (effect != 0 && GlobalData::shared()->worldConfig.fire[0] != 0) {
            per = GlobalData::shared()->worldConfig.fire1[0]*60*2/(GlobalData::shared()->worldConfig.fire[0]*(1+effect));
        }
    }
    else
        per = GlobalData::shared()->worldConfig.fire1[0]*60*2/GlobalData::shared()->worldConfig.fire[0];
    
    string temp = _lang_2("102337","1",CC_ITOA_1(per));
    m_statusDesc->setString(temp);
    m_fireLabel->setString(_lang("102339"));
    temp = CC_ITOA(GlobalData::shared()->worldConfig.fire1[2]);
    m_defLabel->setString(temp);
    
    m_timeTxt->setString(_lang("102336"));//
    temp = _lang_1("102338","");
    m_timeDesc->setString(temp);
    m_fuFeiBtnTxt->setString(_lang("102340"));
    temp = CC_ITOA(GlobalData::shared()->worldConfig.fire1[3]);
    m_fuFeiValue->setString(temp);
    
    if (CCFileUtils::sharedFileUtils()->isFileExist("Imperial/Imperial_30.atlas") &&
        CCFileUtils::sharedFileUtils()->isFileExist("Spine/UIComponent/chengfangchilun.json"))
    {
        auto chilun = new IFSkeletonAnimation("Spine/UIComponent/chengfangchilun.json", "Imperial/Imperial_30.atlas");
        if (chilun) {
            {
                chilun->setVisibleStop(false);
                chilun->setPositionX(-CCDirector::sharedDirector()->getWinSize().width/2);
                chilun->setPositionY(-5);
                m_bNode->addChild(chilun,3);
                spTrackEntry* entry = chilun->setAnimation(0.01, "animation", true);
                if(entry){
                    chilun->setTimeScale(0.5f);
                }
            }
        }
    }
    
    updateInfo();
    return true;
}

void CityDefenseView::updateInfo()
{
    onEnterFrame(0);
}



void CityDefenseView::onEnter()
{
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(CityDefenseView::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    CCNode::onEnter();
}

void CityDefenseView::onExit()
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(CityDefenseView::onEnterFrame), this);
    
    CCNode::onExit();
}

SEL_CCControlHandler CityDefenseView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickAddDefense", CityDefenseView::onClickAddDefense);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHelpClick", CityDefenseView::onHelpClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFuFeiClick", CityDefenseView::onFuFeiClick);
    return NULL;
}

bool CityDefenseView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bNode", CCNode*, this->m_bNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defLabel", CCLabelIF*, this->m_defLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defMsgLabel", CCLabelIF*, this->m_defMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireLabel", CCLabelIF*, this->m_fireLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defbarLabel", CCLabelIF*, this->m_defbarLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defBtn", CCControlButton*, this->m_defBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_defbar", CCScale9Sprite*, this->m_defbar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fullbar", CCScale9Sprite*, this->m_fullbar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_middleNode", CCNode*, this->m_middleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fufeiBtn", CCControlButton*, this->m_fufeiBtn);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_statusTxt", CCLabelIF*, this->m_statusTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_statusDesc", CCLabelIF*, this->m_statusDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeDesc", CCLabelIF*, this->m_timeDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fuFeiBtnTxt", CCLabelIF*, this->m_fuFeiBtnTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fuFeiValue", CCLabelIF*, this->m_fuFeiValue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fullTip", CCLabelIF*, this->m_fullTip);

    return false;
}

void CityDefenseView::onClickAddDefense(CCObject * pSender, Control::EventType pCCControlEvent)
{
    m_defBtn->setEnabled(false);
    m_defBtn->setColor(ccGRAY);
    
    auto cmd = new AddDefenseCommand();
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(CityDefenseView::onUpdateDefenseCallback), NULL));
    cmd->sendAndRelease();
    GameController::getInstance()->showWaitInterface();
}

void CityDefenseView::onHelpClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("102333"),kCCTextAlignmentLeft));
}

void CityDefenseView::onFuFeiClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    YesNoDialog::showButtonAndGold(_lang("102334").c_str(), CCCallFunc::create(this, callfunc_selector(CityDefenseView::fuFeiHandle)), _lang("102340").c_str(), GlobalData::shared()->worldConfig.fire1[3]);
}

void CityDefenseView::fuFeiHandle(){
    if (GlobalData::shared()->worldConfig.fire1[3] > GlobalData::shared()->playerInfo.gold) {
        YesNoDialog::gotoPayTips();
        return ;
    }
    BuyCityDefCommand* cmd = new BuyCityDefCommand();
    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(CityDefenseView::sendSuccess), NULL));
    cmd->sendAndRelease();
    m_fufeiBtn->setEnabled(false);
    GameController::getInstance()->showWaitInterface();
}

void CityDefenseView::sendSuccess(CCObject* data){
    auto ret = dynamic_cast<NetResult*>(data);
    if (!ret) {
        return;
    }
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    if(info->objectForKey("cityDef")){
        GlobalData::shared()->cityDefenseVal = info->valueForKey("cityDef")->intValue();
    }else{
        GlobalData::shared()->cityDefenseVal =  m_cityDefenseVal;
    }
    if(info->objectForKey("lastCityDefTime")){
        GlobalData::shared()->cityLastUpdateDefenseStamp = info->valueForKey("lastCityDefTime")->doubleValue();
    }
    GlobalData::shared()->cityStartFireStamp = WorldController::getInstance()->getTime();
    GlobalData::shared()->cityFireStamp = WorldController::getInstance()->getTime();
    
    onEnterFrame(0.0);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SHOW_CITY_FIRE);
}

void CityDefenseView::onEnterFrame(float dt)
{
    unsigned int fireRate = 0;
    if (GlobalData::shared()->serverType==SERVER_DRAGON_BATTLE) {
        fireRate = ActivityController::getInstance()->wa_fire_k1;
    }else if (GlobalData::shared()->serverType==SERVER_BATTLE_FIELD) {
        fireRate = GlobalData::shared()->wb_fire_k1;
    }else if (GlobalData::shared()->cityTileCountry == NEUTRALLY) {
        fireRate = GlobalData::shared()->worldConfig.fire[2];
    }else{
        if (GlobalData::shared()->alliance_territory_negative_effect > 0) {
            double effect = CCCommonUtils::getTerritoryNegativeEffectValueByNum(122) * 1.0 / 100;
            fireRate = GlobalData::shared()->worldConfig.fire[0]*(1+effect);
        }else{
            fireRate = GlobalData::shared()->worldConfig.fire[0];
        }

    }
    
    auto now = WorldController::getInstance()->getTime();
    unsigned int fix = 0;
    if (GlobalData::shared()->cityStartFireStamp < GlobalData::shared()->cityFireStamp) {
        double start = MIN(now, GlobalData::shared()->cityFireStamp);
        double duration = start - GlobalData::shared()->cityStartFireStamp;
        fix = (unsigned int)(duration * (double)fireRate / 3600000.0);
    }
    
    auto lastUpdate = GlobalData::shared()->cityLastUpdateDefenseStamp;
    double delay = GlobalData::shared()->cityUpdateDefenseDelayMin * 60000.0;
    if (now > lastUpdate + delay) {   //当未点击增加城防值时，提示增加城防值
        m_defBtn->setEnabled(true);
        m_defBtn->setColor(ccWHITE);
        m_fireLabel->setString(_lang("102339"));  //增加城防值
        string temp = CC_ITOA(GlobalData::shared()->worldConfig.fire1[2]);
        m_defLabel->setString(temp);
        CCCommonUtils::setButtonTitle(m_defBtn, "");
    } else {      //点击了增加城防值，提示下次再次增加城防值需要的冷却时间
        m_defBtn->setEnabled(false);
        m_defBtn->setColor(ccGRAY);
        int tmpTime = ((lastUpdate + delay)-now)/1000;
        CCCommonUtils::setButtonTitle(m_defBtn, CC_SECTOA(tmpTime));
        m_fireLabel->setString("");
        m_defLabel->setString("");
    }
    
    unsigned int cityDefMax = GlobalData::shared()->imperialInfo[FUN_BUILD_WALL_ID].para2 + CCCommonUtils::getEffectValueByNum(125);
    //unsigned int cityDefFix = GlobalData::shared()->cityDefenseVal - fix;
    unsigned int cityDefFix = GlobalData::shared()->cityDefenseVal - fix;   //初始剩余的防御值
    if(now >= GlobalData::shared()->cityFireStamp){
        cityDefFix = GlobalData::shared()->cityDefenseVal;
    }
    m_cityDefenseVal = cityDefFix;
    m_fufeiBtn->setEnabled(false);
    m_middleNode->setVisible(false);
    if (cityDefFix >= cityDefMax) {
        m_fullbar->setVisible(true);
        m_defBtn->setVisible(false);
        m_defBtn->setEnabled(false);
        m_defBtn->setColor(ccGRAY);
        m_defMsgLabel->setString("");
        m_fullTip->setString(_lang("102331"));   //目前没有人攻击你的城堡，你的城堡完好无损
        m_defLabel->setVisible(true);
    }
    else {  //城堡有损伤
        m_defBtn->setVisible(true);
        m_middleNode->setVisible(true);
        m_fullbar->setVisible(false);
        m_fullTip->setString("");
        if (now < GlobalData::shared()->cityFireStamp) {  //城堡还在燃烧
            m_fufeiBtn->setEnabled(true);
            m_defMsgLabel->setString(_lang("102332"));
            
            double dt = (GlobalData::shared()->cityFireStamp-now)/1000;
            string timestr = "";
            int timedt = 0;
            if(dt>=24*60*60){   //一天
                timedt =dt/(24*60*60);
                timestr = CC_ITOA(timedt);
                timestr.append(_lang("105592"));
            }else if(dt>=60*60){   //1小时
                timedt =dt/(60*60);
                timestr = CC_ITOA(timedt);
                timestr.append(_lang("105591"));
            }else if(dt>=60){     //大于1分钟
                timedt =dt/60;
                timestr = CC_ITOA(timedt);
                timestr.append(_lang("105590"));  //分钟
            }else{
                timestr ="1";
                timestr.append(_lang("105590"));
            }
        
            string str = _lang("102335");
            m_statusTxt->setString(_lang("102335"));
            string temp = "";
            double per = 0;
            if (GlobalData::shared()->alliance_territory_negative_effect >0) {//联盟领地减益
                double effect = CCCommonUtils::getTerritoryNegativeEffectValueByNum(122) * 1.0 / 100;
                if (GlobalData::shared()->serverType==SERVER_DRAGON_BATTLE) {
                    int rate = ActivityController::getInstance()->wa_fire_k1;
                    int perRate = rate/3600;
                    temp = _lang_2("102337",CC_ITOA(perRate),"1");
                }else if (GlobalData::shared()->serverType==SERVER_BATTLE_FIELD) {
                    int rate = GlobalData::shared()->wb_fire_k1;
                    int perRate = rate/3600;
                    temp = _lang_2("102337",CC_ITOA(perRate),"1");
                }else if (GlobalData::shared()->cityTileCountry == NEUTRALLY) {
                    per = GlobalData::shared()->worldConfig.fire[2]/(GlobalData::shared()->worldConfig.fire1[0]*60*2*effect);
                    temp = _lang_2("102337",CC_ITOA(per),"1");
                }else{
                    if (GlobalData::shared()->worldConfig.fire[0] != 0 && effect != 0) {
                        per = GlobalData::shared()->worldConfig.fire1[0]*60*2/(GlobalData::shared()->worldConfig.fire[0]*(1+effect));
                        temp = _lang_2("102337","1",CC_ITOA_1(per));
                    }
                }
            }
            else {
                if (GlobalData::shared()->serverType==SERVER_DRAGON_BATTLE) {
                    int rate = ActivityController::getInstance()->wa_fire_k1;
                    int perRate = rate/3600;
                    temp = _lang_2("102337",CC_ITOA(perRate),"1");
                }else if (GlobalData::shared()->serverType==SERVER_BATTLE_FIELD) {
                    int rate = GlobalData::shared()->wb_fire_k1;
                    int perRate = rate/3600;
                    temp = _lang_2("102337",CC_ITOA(perRate),"1");
                }else if (GlobalData::shared()->cityTileCountry == NEUTRALLY) {
                    per = GlobalData::shared()->worldConfig.fire[2]/(GlobalData::shared()->worldConfig.fire1[0]*60*2);
                    temp = _lang_2("102337",CC_ITOA(per),"1");
                }else{
                    per = GlobalData::shared()->worldConfig.fire1[0]*60*2/GlobalData::shared()->worldConfig.fire[0];
                    temp = _lang_2("102337","1",CC_ITOA_1(per));
                }
            }
            m_statusDesc->setString(temp);
            m_timeTxt->setString(_lang("102336"));
            m_timeDesc->setString(_lang_1("102338",timestr.c_str()));   //多少分钟后火焰将熄灭
        }
        else {   //城堡不再燃烧
            m_defMsgLabel->setString(_lang("102341")); //你的城堡不着火了，但需要修理
            m_timeTxt->setString(_lang("102336"));
            m_timeDesc->setString(_lang("102344"));
            m_statusTxt->setString(_lang("102342"));
            m_statusDesc->setString(_lang("102343"));
        }
    }
    float pro = cityDefFix*1.0/cityDefMax;
    pro = MIN(pro,1);
    pro = MAX(0, pro);
    m_defbar->setScaleX(pro);
    m_defbarLabel->setString(_lang_2("102170", CC_CMDITOA(cityDefFix).c_str(), CC_CMDITOA(cityDefMax).c_str()));
}

void CityDefenseView::onUpdateDefenseCallback(cocos2d::CCObject *obj) {
    GameController::getInstance()->removeWaitInterface();
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        return;
    }
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    GlobalData::shared()->cityDefenseVal = info->valueForKey("cityDefValue")->intValue();
    GlobalData::shared()->cityLastUpdateDefenseStamp = info->valueForKey("lastCityDefTime")->doubleValue();
    GlobalData::shared()->cityStartFireStamp = WorldController::getInstance()->getTime();
}
