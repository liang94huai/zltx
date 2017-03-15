//
//  BattleWinOrLoseInfo.cpp
//  IF
//
//  Created by 〜TIAN~ on 17/1/3.
//
//

#include "BattleWinOrLoseInfo.h"
#include "WorldMapView.h"
 
int BattleWinOrLoseInfo::m_isExit = 0;
const float numPerRow = 1.0;
BattleWinOrLoseInfo* BattleWinOrLoseInfo::create(int iType,MarchInfo marchInfo){
//    if(m_isExit)
//    {
//        return nullptr;
//    }
    BattleWinOrLoseInfo *ret = new BattleWinOrLoseInfo(iType,marchInfo);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool BattleWinOrLoseInfo::init(){
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(4, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(4, false);
        });
        
        
        auto tmpCCB = CCBLoadFile("BattleWinOrLoseInfo",this,this,true);
        this->setContentSize(tmpCCB->getContentSize());
        initLabInfo();
        setData(m_type);
      }
    return true;
}

void BattleWinOrLoseInfo::initParticle(bool bWin)
{
    if (bWin) {
//        m_particle_1
    }
    else {
        
    }
}

void BattleWinOrLoseInfo::setData(int iType)
{
    bool isWin = false;
    if ( iType == 1 )
    {
        m_infoLab->setString("恭喜主公,获取大胜!!");
        isWin = true;
    }else if (iType == 2)
    {
        m_infoLab->setString("恭喜主公,赢得胜利!");
        isWin = true;
    
    }else if (iType == 3)
    {
        m_infoLab->setString("恭喜主公,险胜敌人!");
        isWin = true;
        
    }else if (iType == 4)
    {
        m_infoLab->setString("不要气馁,请主公再接再厉哦!");
        
    }else if (iType == 5)
    {
        m_infoLab->setString("不要气馁,请主公再接再厉哦!");
        
    }else if (iType == 6)
    {
        m_infoLab->setString("不要气馁,请主公再接再厉哦!");
        
    }
    
    string strRes = StringUtils::format("battle_re_0%d.png", iType);
    m_infoLab->setHorizontalAlignment(TextHAlignment::CENTER);
    m_title->setDisplayFrame(CCLoadSprite::loadResource(strRes.c_str()));
    if(isWin)
    {
        m_winDizi->setVisible(true);
        m_LoseDizi->setVisible(false);
        if(m_marchInfo.targetType == ResourceTile)
        {
            m_infoLab->setString(_lang("new100027").c_str());
        }else {
            m_infoLab->setString(_lang("new100028").c_str());
        }
    }else
    {
        m_winDizi->setVisible(false);
        m_LoseDizi->setVisible(true);
        
        m_infoLab->setString(_lang("new100029").c_str());
    }
    
}

 
void BattleWinOrLoseInfo::initLabInfo()
{
    m_infoLab->setFntFile("Arial_Bold_Border.fnt");
     m_tipLab->setFntFile("Arial_Bold_Border.fnt");
     m_returnLab->setFntFile("Arial_Bold_Border.fnt");
    //m_title->setString(_lang("new100014").c_str());
    m_infoLab->setString("失败,胜利!");
    m_tipLab->setString(_lang("new100026").c_str());
    m_returnLab->setString(_lang("105690").c_str());
    
    
}
 

void BattleWinOrLoseInfo::onEnter(){
    PopupBaseView::onEnter();
    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    this->setTouchEnabled(true);
    BattleWinOrLoseInfo::m_isExit = 1;
    
}

void BattleWinOrLoseInfo::onExit(){
    this->setTouchEnabled(false);
    BattleWinOrLoseInfo::m_isExit = 0;
    PopupBaseView::onExit();
}

void BattleWinOrLoseInfo::closeBtnOk(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    auto &cityInfo = WorldController::getInstance()->m_cityInfo[m_marchInfo.stateType == StateReturn ? m_marchInfo.startPointIndex : m_marchInfo.endPointIndex];
    auto skn=WorldMapView::instance()->m_flagNode->getChildByTag(WorldMapView::instance()->getBatchTag(ResFlagTag, cityInfo.parentCityIndex));
    if(skn&&m_type<4)
    {
        skn->setScale(0);
        WorldMapView::instance()->onAddResFlag(cityInfo,1/*= 0*/);
    }
    this->closeSelf();
}
bool BattleWinOrLoseInfo::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_LoseDizi", CCNode*, this->m_LoseDizi);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tochNode", CCNode*, this->m_tochNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_winDizi", CCNode*, this->m_winDizi);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title", CCSprite*, m_title);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLab", CCLabelIF*, m_infoLab);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tipLab", CCLabelIF*, m_tipLab);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_returnLab", CCLabelIF*, m_returnLab);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particle_1", Node*, m_particle_1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particle_2", Node*, m_particle_2);
    
    
    return false;
}

SEL_CCControlHandler BattleWinOrLoseInfo::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){

    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "closeBtnOk", BattleWinOrLoseInfo::closeBtnOk);
    return NULL;
}


bool BattleWinOrLoseInfo::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
   
    return true;
}
void BattleWinOrLoseInfo::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(!isTouchInside(m_tochNode, pTouch)){
        this->closeBtnOk(nullptr,Control::EventType::TOUCH_DOWN);
    }
}
