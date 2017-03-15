//
//  DragonZhenYingView.cpp
//  IF
//
//  Created by ganxiaohua on 15/9/28.
//
//

#include "DragonZhenYingView.h"
#include "ParticleController.h"
#include "ActivityController.h"
#include "QuestController.h"
#include "WorldController.h"
#include "GetAllianceMemberPointCommand.h"

static CCSize bgsize1 = CCSize(200.0,160);
static CCSize bgsize2 = CCSize(900.0,160);

DragonZhenYingView *DragonZhenYingView::create(){
    DragonZhenYingView *ret = new DragonZhenYingView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DragonZhenYingView::init(){
    bool ret = false;
    if (CCNode::init()) {
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(512, false);
        });
        CCLoadSprite::doResourceByCommonIndex(512, true);
        auto node = CCBLoadFile("DragonZhenYingView", this, this);
        this->setContentSize(node->getContentSize());
        
        m_proTimer = CCProgressTimer::create(CCLoadSprite::createSprite("zhenying_5.png"));
        m_proTimer->setType(kCCProgressTimerTypeBar);
        m_proTimer->setMidpoint(ccp(0,1));   // 设置进度方向
        m_proTimer->setBarChangeRate(ccp(1,0));  // 设置进度为水平还是垂直方向
        m_proTimer->setAnchorPoint(ccp(0, 0));
        m_progressNode->addChild(m_proTimer);
        m_proTimer->setPosition(ccp(0, -6));
        m_proTimer->setPercentage(0);
        
        m_rightPro = CCProgressTimer::create(CCLoadSprite::createSprite("zhenying_6.png"));
        m_rightPro->setType(kCCProgressTimerTypeBar);
        m_rightPro->setMidpoint(ccp(0,1));   // 设置进度方向
        m_rightPro->setBarChangeRate(ccp(1,0));  // 设置进度为水平还是垂直方向
        m_rightPro->setAnchorPoint(ccp(0, 0));
        m_progressNode->addChild(m_rightPro);
        m_rightPro->setPosition(ccp(212, -6));
        m_rightPro->setPercentage(0);
        
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        if(!CCCommonUtils::isIosAndroidPad()){
            m_topNode->setPosition(ccp((winSize.width-460)/2, winSize.height-135));
            m_txtNode->setPosition(ccp(0, winSize.height-190));
        }else{
            m_topNode->setPosition(ccp((winSize.width-920)/2, winSize.height - 350));
            m_txtNode->setPosition(ccp(0, winSize.height-450));
        }

        m_serverTxt->setString(_lang("140001"));
        
        if (CCCommonUtils::isIosAndroidPad()) {
            m_chuZhang= CCRichLabelTTF::create("0", "Helvetica", 36,CCSize(880,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
            m_chuZhang->setAnchorPoint(ccp(0, 0.5));
            m_chuZhang->setPosition(ccp(46,34));
            m_txtNode->addChild(m_chuZhang);
            
            m_qianCheng = CCRichLabelTTF::create("0", "Helvetica", 36,CCSize(880,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
            m_qianCheng->setAnchorPoint(ccp(0, 0.5));
            m_qianCheng->setPosition(ccp(46, -25));
            m_txtNode->addChild(m_qianCheng);
        }else{
            m_chuZhang= CCRichLabelTTF::create("", "Helvetica", 20,CCSize(450,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
            m_chuZhang->setPosition(ccp(12, -38)+m_serverTxt->getPosition());
            m_txtNode->addChild(m_chuZhang);
            
            m_qianCheng = CCRichLabelTTF::create("", "Helvetica", 20,CCSize(450,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
            m_qianCheng->setPosition(ccp(12, -80)+m_serverTxt->getPosition());
            m_txtNode->addChild(m_qianCheng);
        }


        if(GlobalData::shared()->campType==CAMP_BLUE){
            m_campTxt2->setString(_lang("140140"));
        }else{
            m_campTxt1->setString(_lang("140140"));
        }
        
        m_leftTime = 60000;
        selfTimes = 0;
        selfMaxTimes = 0;
        selfCnt = 0;
        enemyTimes = 0;
        enemyMaxTimes = 0;
        enemyCnt = 0;
        
        this->schedule(schedule_selector(DragonZhenYingView::updateTime), 1);
        updateTime(0);
        ret = true;
    }
    return ret;
}

void DragonZhenYingView::updateTime(float _time){
    long left = GlobalData::shared()->dbFightEndTime - GlobalData::shared()->getWorldTime();
    if (left<=0) {
        left = 0;
        this->unschedule(schedule_selector(DragonZhenYingView::updateTime));
    }
    m_timeTxt->setString(CC_SECTOA(left));
    
}


void DragonZhenYingView::onEnter(){
    CCNode::onEnter();
    setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonZhenYingView::onMemberDataBack), GET_MEMBER_POINT_BACK, NULL);
}

void DragonZhenYingView::onExit(){
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, GET_MEMBER_POINT_BACK);
    CCNode::onExit();
}

bool DragonZhenYingView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(m_arrow, pTouch)) {
        return true;
    }
    return false;
}

void DragonZhenYingView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(m_arrow, pTouch)) {
        changeStatus();
    }
}

void DragonZhenYingView::changeStatus(){
    string color1 = "[color=ff5092ff]";
    string color2 = "[color=ffA22200]";
    if (m_arrow->getRotation()==180) {
        m_arrow->setRotation(0);
        if(!CCCommonUtils::isIosAndroidPad()){
            m_txtBg->setContentSize(CCSize(486,88));
            m_arrowNode->setPositionX(486);
        }else{
            m_txtBg->setContentSize(bgsize2);
            m_arrowNode->setPositionX(bgsize2.width);
        }

        string t1 = color1 + string(CC_ITOA(enemyCnt)) + "[/color]";
        string t2 = color2 + string(CC_ITOA(selfCnt)) + "[/color]";
        if (GlobalData::shared()->campType==CAMP_BLUE) {
            t1 = color1 + string(CC_ITOA(selfCnt)) + "[/color]";//CAMP_BLUE 守序
            t2 = color2 + string(CC_ITOA(enemyCnt)) + "[/color]";
        }
        string infoStr = _lang_2("140051", t2.c_str(), t1.c_str());
        m_chuZhang->setString(infoStr.c_str());
        
        t1 = color1 + string(CC_ITOA(enemyTimes)) + "/" + string(CC_ITOA(enemyMaxTimes)) + "[/color]";
        t2 = color2 + string(CC_ITOA(selfTimes)) + "/" + string(CC_ITOA(selfMaxTimes)) + "[/color]";
        if (GlobalData::shared()->campType==CAMP_BLUE) {
            t1 = color1 + string(CC_ITOA(selfTimes)) + "/" + string(CC_ITOA(selfMaxTimes)) + "[/color]";
            t2 = color2 + string(CC_ITOA(enemyTimes)) + "/" + string(CC_ITOA(enemyMaxTimes)) + "[/color]";
        }
        infoStr = _lang_2("140105", t2.c_str(),t1.c_str());
        m_qianCheng->setString(infoStr.c_str());
    }else{
        m_arrow->setRotation(180);
        if(!CCCommonUtils::isIosAndroidPad()){
            m_txtBg->setContentSize(CCSize(100.0,88));
            m_arrowNode->setPositionX(100.0);
        }else{
            m_txtBg->setContentSize(bgsize1);
            m_arrowNode->setPositionX(bgsize1.width);
        }
        string t1 = "";
        t1.append(color2);
        t1.append(CC_ITOA(selfCnt));
        t1.append("[/color]");
        t1.append("/");
        t1.append(color1);
        t1.append(CC_ITOA(enemyCnt));
        t1.append("[/color]");
        if (GlobalData::shared()->campType==CAMP_BLUE) {
            t1 = "";
            t1.append(color2);
            t1.append(CC_ITOA(enemyCnt));
            t1.append("[/color]");
            t1.append("/");
            t1.append(color1);
            t1.append(CC_ITOA(selfCnt));
            t1.append("[/color]");
        }
        m_chuZhang->setString(t1.c_str());
        
        t1 = "";
        t1.append(color2);
        t1.append(CC_ITOA(selfTimes));
        t1.append("[/color]");
        t1.append("/");
        t1.append(color1);
        t1.append(CC_ITOA(enemyTimes));
        t1.append("[/color]");
        if (GlobalData::shared()->campType==CAMP_BLUE) {
            t1 = "";
            t1.append(color2);
            t1.append(CC_ITOA(enemyTimes));
            t1.append("[/color]");
            t1.append("/");
            t1.append(color1);
            t1.append(CC_ITOA(selfTimes));
            t1.append("[/color]");
        }
        m_qianCheng->setString(t1.c_str());
    }
}

void DragonZhenYingView::onMemberDataBack(CCObject* param){
    auto dict = _dict(param);
    if(dict->objectForKey("build")){
        CCArray *array = dynamic_cast<CCArray*>(dict->objectForKey("build"));
        CCObject *object;
        CCARRAY_FOREACH(array, object){
            auto oneBuild = _dict(object);
            string id = oneBuild->valueForKey("id")->getCString();
            string allianceId = oneBuild->valueForKey("allianceId")->getCString();
        }
    }
    double selfScore = 0;
    double enemyScore = 0;
    if(dict->objectForKey("bInfo")){
        CCArray *array = dynamic_cast<CCArray*>(dict->objectForKey("bInfo"));
        CCObject *object;
        CCARRAY_FOREACH(array, object){
            auto oneDic = _dict(object);
            string allianceId = oneDic->valueForKey("allianceId")->getCString();
            if (allianceId==GlobalData::shared()->playerInfo.allianceInfo.uid) {
                selfTimes = oneDic->valueForKey("leftPortTimes")->intValue();
                selfMaxTimes = oneDic->valueForKey("maxPortTimes")->intValue();
                selfCnt = oneDic->valueForKey("memCnt")->intValue();
                selfScore = oneDic->valueForKey("score")->doubleValue();
            }else{
                enemyTimes = oneDic->valueForKey("leftPortTimes")->intValue();
                enemyMaxTimes = oneDic->valueForKey("maxPortTimes")->intValue();
                enemyCnt = oneDic->valueForKey("memCnt")->intValue();
                enemyScore = oneDic->valueForKey("score")->doubleValue();
            }
        }
    }
    double max = ActivityController::getInstance()->wa_score_k10;
    int per = 0;
    if (GlobalData::shared()->campType==CAMP_BLUE) {
        string temp = _lang("140064");//140063 140151
        temp.append(":");
        temp.append(CC_CMDITOAD(enemyScore));
        m_leftTxt->setString(temp);
        temp = _lang("140063");//140063 140150
        temp.append(":");
        temp.append(CC_CMDITOAD(selfScore));
        m_rightTxt->setString(temp);
       
        per = enemyScore/max * 100;
        if (per>100) {
            per = 100;
        }
        m_proTimer->setPercentage(per);
        per = selfScore/max * 100;
        if (per>100) {
            per = 100;
        }
        m_rightPro->setPercentage(per);
    }else{
        string temp = _lang("140064");
        temp.append(":");
        temp.append(CC_CMDITOAD(selfScore));
        m_leftTxt->setString(temp);
        temp = _lang("140063");
        temp.append(":");
        temp.append(CC_CMDITOAD(enemyScore));
        m_rightTxt->setString(temp);

        per = selfScore/max * 100;
        if (per>100) {
            per = 100;
        }
        m_proTimer->setPercentage(per);
        per = enemyScore/max * 100;
        if (per>100) {
            per = 100;
        }
        m_rightPro->setPercentage(per);
    }

    m_arrow->setRotation(0);
    changeStatus();
}

bool DragonZhenYingView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_progressNode", CCNode*, this->m_progressNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftTxt", CCLabelIF*, this->m_leftTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightTxt", CCLabelIF*, this->m_rightTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_serverTxt", CCLabelIF*, this->m_serverTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txtNode", CCNode*, this->m_txtNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrowNode", CCNode*, this->m_arrowNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrow", CCNode*, this->m_arrow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txtBg", CCScale9Sprite*, this->m_txtBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_topNode", CCNode*, this->m_topNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_campTxt1", CCLabelIF*, this->m_campTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_campTxt2", CCLabelIF*, this->m_campTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickNode", CCNode*, this->m_clickNode);
    return false;
}

SEL_CCControlHandler DragonZhenYingView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}