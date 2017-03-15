//
//  FunBuild.cpp
//  IF
//
//  Created by fubin on 13-10-31.
//
//
#include "YesNoDialog.h"
#include "FunBuild.h"
#include "SceneController.h"
#include "SoundController.h"
#include "PopupViewController.h"
#include "ScienceListView.h"
#include "WatchtowerView.h"
#include "MainCityView.h"
#include "FunBuildController.h"
#include "BuildingHospitalPopUpView.h"
#include "AllianceMarketView.h"
#include "BattleView.h"
#include "QueueController.h"
#include "ToolController.h"
#include "WorldController.h"
#include "ImperialScene.h"
#include "BuildListView.h"

#include "FunBuildView.h"
#include "MakingSoldierView.h"
#include "ProductionSoldiersView.h"
#include "ItemStatusView.h"
#include "YesNoDialog.h"
#include "AllianceManager.h"
#include "ArmyController.h"
#include "GuideController.h"
#include "HarvestSkillAni.h"
#include "AllianceIntroTip.h"
#include "EquipmentController.h"
#include "ToolCreateView.h"
#include "SacrificePopUpView.h"
#include "DynamicResourceController.h"
#include "IFNormalSkNode.h"
#include "AllianceDailyController.h"
#include "AllianceDailyPublishView.h"

using namespace cocos2d;

const float nameWidth = 456;
const float nameHeight = 164;

/////////////
FunBuildLv* FunBuildLv::create(int itemId,CCLabelBatchNode* nameLayer,CCSpriteBatchNode* lvBatchNode)
{
    FunBuildLv *pRet = new FunBuildLv();
    if (pRet && pRet->init(itemId,nameLayer,lvBatchNode))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}
bool FunBuildLv::init(int itemId,CCLabelBatchNode* nameLayer,CCSpriteBatchNode* lvBatchNode){
    m_buildingKey = itemId;
    m_nameLayer = nameLayer;
    m_lvBatchNode = lvBatchNode;
    
    m_info = &((*FunBuildController::getInstance()->curBuildsInfo)[m_buildingKey]);
    
    m_mainNode = CCNode::create();
    this->addChild(m_mainNode);
    CCBLoadFile("picLv",m_mainNode,this);
    
    m_lvBG->removeFromParent();
    m_arrSpr->removeFromParent();
    m_arrStar->removeFromParent();
    
    m_lvBatchNode->addChild(m_lvBG);
    m_lvBatchNode->addChild(m_arrSpr);
    m_lvBatchNode->addChild(m_arrStar);
    
    int num = m_info->level;
    if (m_info->starNum >= 1 && FunBuildController::getInstance()->building_Miracle_open) {
        if(m_info->star_on > 0){
            num = m_info->starNum;
        }
    }
    auto lvStr=CCCommonUtils::getChineseLevelTxtByNum(num);
    m_lvLabel = CCLabelTTF::create(lvStr.c_str(), "Helvetica-Bold", 20 );
    m_nameLayer->addChild(m_lvLabel);
    //m_lvLabel = CCLabelBatch::create(CC_ITOA(num), m_nameLayer);
    m_lvLabel->setScale(0.5);
    m_lvLabel->setAnchorPoint(ccp(0.5,0.5));
    m_lvLabel->setColor(ccc3(0, 0, 0));
    m_lvLabel->setSkewY(27);
    
    m_lvBG->setVisible(true);
    m_lvLabel->setVisible(true);
    m_arrStar->setVisible(false);
    
    
    if(m_info->type == FUN_BUILD_QIJI){
        m_lvLabel->setVisible(false);
    }
    if(FunBuildController::getInstance()->checkOpenUpstar(m_info->type)){
//        if(FunBuildController::getInstance()->building_Miracle_open && m_info->starNum >= 1)
//        {
//            m_lvBG->setVisible(false);
//            m_lvLabel->setVisible(false);
//            m_arrStar->setVisible(true);
//        }else{
            m_lvBG->setVisible(true);
            m_lvLabel->setVisible(true);
            m_arrStar->setVisible(false);
//        }
    }
    return true;
}

void FunBuildLv::onEnter(){
    
}
void FunBuildLv::onExit(){
    
}

void FunBuildLv::onBuildLvDelete(){
    m_lvLabel->removeFromParentAndCleanup(true);
    m_lvBG->removeFromParentAndCleanup(true);
    m_arrSpr->removeFromParentAndCleanup(true);
    m_arrStar->removeFromParentAndCleanup(true);
}

void FunBuildLv::setFunBuildLvShow(bool isShow)
{
    m_lvNode->setVisible(isShow);
    m_arrNode->setVisible(isShow);
    m_arrSpr->setVisible(isShow);
    m_lvBG->setVisible(isShow);
    m_arrStar->setVisible(false);
}

void FunBuildLv::setNamePos(int x, int y)
{
    m_lvLabel->setPosition((m_lvNode->getPosition())+ccp(x, y));
    m_lvBG->setPosition(m_lvBG->getPosition()+ccp(x, y));
    m_arrSpr->setPosition(m_arrSpr->getPosition()+ccp(x, y));
    m_arrStar->setPosition(m_arrStar->getPosition()+ccp(x, y));
    m_arrY = m_arrSpr->getPositionY();
}

void FunBuildLv::onPlayUpIcon()
{
    if (!m_arrSpr &&!m_arrSpr->isVisible()) {
        return;
    }
    m_arrSpr->setPositionY(m_arrY);
    CCMoveBy* moveUp= CCMoveBy::create(0.1f, ccp(0,10));
    CCMoveBy* moveDown= CCMoveBy::create(0.1f, ccp(0,-10));
    CCMoveBy* moveUp2= CCMoveBy::create(0.1f, ccp(0,5));
    CCMoveBy* moveDown2= CCMoveBy::create(0.1f, ccp(0,-5));
    m_arrSpr->runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(1),moveUp,moveDown,moveUp2,moveDown2,NULL)));
}
void FunBuildLv::stopUpIcon()
{
    if (!m_arrSpr) {
        return;
    }
    m_arrSpr->stopAllActions();
    m_arrSpr->setPositionY(m_arrY);
}

void FunBuildLv::setLv(){
    auto num=m_info->level;
    m_lvLabel->setString(CC_ITOA(m_info->level));
    if(m_info->starNum >= 1){
        num=m_info->starNum;
    }
    auto lvStr=CCCommonUtils::getChineseLevelTxtByNum(num);
    m_lvLabel->setString(lvStr.c_str());
}

void FunBuildLv::onUpdateUpIcon(float dt)
{
    bool tmpIsLock = false;
    if (m_info->open>0 &&m_info->type != FUN_BUILD_MAIN && m_info->open>FunBuildController::getInstance()->getMainCityLv()) {
        tmpIsLock = true;
    }
    if(tmpIsLock)
    {
        setFunBuildLvShow(false);
        return;
    }

    if(FunBuildController::getInstance()->checkOpenUpstar(m_info->type)){
//        if(FunBuildController::getInstance()->building_Miracle_open && m_info->starNum >= 1)
//        {
//            m_lvBG->setVisible(false);
//            m_lvLabel->setVisible(true);
//            m_arrStar->setVisible(true);
//        }else{
            m_lvBG->setVisible(true);
            m_lvLabel->setVisible(true);
            m_arrStar->setVisible(false);
//        }
    }
    if(m_info->type == FUN_BUILD_WORKSHOP) {
        m_lvLabel->setVisible(false);
        m_arrSpr->setVisible(false);
        m_lvBG->setVisible(false);
        return;
    }
    
    if (m_info->state == FUN_BUILD_NORMAL && m_info->isUnLock()) {
        bool canUp = true;
        int tmpTime = m_info->time_need/(1+CCCommonUtils::getEffectValueByNum(68)*1.0/100);
        int qid = QueueController::getInstance()->canMakeItemByType(TYPE_BUILDING, tmpTime);
        if(QID_MAX == qid){
            canUp = false;
        }
        if (canUp) {
            if (GlobalData::shared()->resourceInfo.lMoney<m_info->silver_need
                || GlobalData::shared()->resourceInfo.totalWood()<m_info->wood_need
                || GlobalData::shared()->resourceInfo.totalFood()<m_info->food_need
                || GlobalData::shared()->resourceInfo.totalStone()<m_info->stone_need
                || GlobalData::shared()->resourceInfo.totalIron()<m_info->iron_need) {
                canUp = false;
            }
        }
        if (canUp && m_info->tool_need!="") {
            std::vector<string> toolItems;
            CCCommonUtils::splitString(m_info->tool_need, "|", toolItems);
            for (int i=0; i<toolItems.size(); i++) {
                string curTool = toolItems[i];
                std::vector<string> item;
                CCCommonUtils::splitString(curTool, ";", item);
                int id = atoi(item[0].c_str());
                int num = atoi(item[1].c_str());
                auto& info = ToolController::getInstance()->getToolInfoById(id);
                if(num > info.getCNT()){
                    canUp = false;
                }
            }
        }
        m_arrSpr->setVisible(canUp);
    }
    else {
        m_arrSpr->setVisible(false);
    }
    
    if(m_info->level >= GlobalData::shared()->MaxBuildLv){
        
        if(!FunBuildController::getInstance()->checkOpenUpstar(m_info->type)){
            m_arrSpr ->setVisible(false);
        }
        else{
            if (m_info->starNum >= GlobalData::shared()->starnum_k6) {
                m_arrSpr->setVisible(false);
            }
        }
    }
}

SEL_MenuHandler FunBuildLv::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName){
    return NULL;
}
SEL_CCControlHandler FunBuildLv::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName){
    return NULL;
}
bool FunBuildLv::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvNode", CCNode*, this->m_lvNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrNode", CCNode*, this->m_arrNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrSpr", CCSprite*, this->m_arrSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvBG", CCSprite*, this->m_lvBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrStar", CCSprite*, this->m_arrStar);
    return false;
}

/////////////
FunBuild* FunBuild::create(int itemId, CCLabelBatchNode* nameLayer,CCSpriteBatchNode* lvBatchNode)
{
    FunBuild *pRet = new FunBuild();
    if (pRet && pRet->initFunBuild(itemId,nameLayer,lvBatchNode))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

FunBuild* FunBuild::createTmpBuild(int itemId, int x, int y, CCSpriteBatchNode* batchNode, int zOrder, CCSpriteBatchNode* blentbatch, Node * pSpineLayer)
{
    FunBuild *pRet = new FunBuild();
    if (pRet && pRet->initTmpBuild(itemId,x,y,batchNode,zOrder,blentbatch, pSpineLayer))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool FunBuild::initTmpBuild(int itemId, int x, int y, CCSpriteBatchNode* batchNode, int zOrder, CCSpriteBatchNode* blentbatch, Node * pSpineLayer)
{
    if(m_sprArray) {
        m_sprArray->removeAllObjects();
        m_personArray->removeAllObjects();
        m_blentSprArray->removeAllObjects();
    }
    else {
        m_sprArray = CCArray::create();
        m_personArray = CCArray::create();
        m_blentSprArray = CCArray::create();
    }
    string pic = CCCommonUtils::getPropById(CC_ITOA(itemId), "pic");
    string ccbName = pic + "_" + CC_ITOA(GlobalData::shared()->contryResType) + "_1";
    m_mainNode = CCNode::create();
    this->addChild(m_mainNode);
    CCBLoadFile(ccbName.c_str(),m_mainNode,this);
    
    parentX = x;
    parentY = y;
    m_batchNode = batchNode;
    m_blentBatchNode = blentbatch;
    
    int tmpOrd = 10;
    if(m_mainNode) {
        auto& array = m_mainNode->getChildren();
        if (array.size()>0) {
            CCNode* tmpNode = dynamic_cast<CCNode*>(array.at(0));//ccb 中的layer层
            if (tmpNode) {
                auto& arr1 = tmpNode->getChildren();
                if (arr1.size()>0) {
                    CCNode* allSprNode = dynamic_cast<CCNode*>(arr1.at(0));//ccb 中的layer的 node
                    if (allSprNode) {
                        auto& arrSpr = allSprNode->getChildren();
                        if (true) {
                            for (int i=0; i<arrSpr.size(); i++) {
                                CCSprite* tmpSpr = dynamic_cast<CCSprite*>(arrSpr.at(i));
                                if (tmpSpr) {
                                    tmpSpr->removeFromParent();
                                    tmpSpr->getTexture()->setAntiAliasTexParameters();
                                    tmpSpr->setPosition(ccp(tmpSpr->getPositionX()+parentX, tmpSpr->getPositionY()+parentY));
                                    
                                    if (m_blentBatchNode) {
                                        auto batchBlend = m_blentBatchNode->getBlendFunc();
                                        auto particleBlend = tmpSpr->getBlendFunc();
                                        if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                                            m_blentBatchNode->addChild(tmpSpr);
                                            m_blentSprArray->addObject(tmpSpr);
                                        }
                                        else {
                                            m_batchNode->addChild(tmpSpr);
                                            m_sprArray->addObject(tmpSpr);
                                        }
                                    }
                                    else {
                                        m_batchNode->addChild(tmpSpr);
                                        m_sprArray->addObject(tmpSpr);
                                    }
                                    tmpSpr->setZOrder(zOrder*1000+tmpOrd);
                                    tmpOrd++;
                                    i--;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if(m_FunBuildLv) {
        m_FunBuildLv->setFunBuildLvShow(false);
    }
    
    //begin a by ljf
    initSpineNode(pic + "_" + CC_ITOA(GlobalData::shared()->contryResType) + "_1", pSpineLayer);
    //initParticle(itemId);
    //end a by ljf
    
    return true;
}

bool FunBuild::initFunBuild(int itemId, CCLabelBatchNode* nameLayer, CCSpriteBatchNode* lvBatchNode)
{
    m_gainSpr = NULL;
    STClickName = "";
    m_buildingKey = itemId;
    m_nameLayer = nameLayer;
    m_lvBatchNode = lvBatchNode;
    
    m_signNode = CCNode::create();   //这两行代码前移，避免return 后  m_signNode和m_upEffectNode没有构建
    m_upEffectNode = CCNode::create();
    m_forgeEffectNode = CCNode::create();
    
    if(m_sprArray) {
        m_sprArray->removeAllObjects();
        m_personArray->removeAllObjects();
        m_blentSprArray->removeAllObjects();
    }
    else {
        m_sprArray = CCArray::create();
        m_personArray = CCArray::create();
        m_blentSprArray = CCArray::create();
    }
    
    if(itemId > 1000)
    {
        if (m_buildingKey/1000 == FUN_BUILD_PRISON) {
            return true;
        }

        m_info = &((*FunBuildController::getInstance()->curBuildsInfo)[m_buildingKey]);
        
        m_mainNode = CCNode::create();
        this->addChild(m_mainNode);
        
        int lv = 1;
//        for (int i=0; i<m_info->picVec.size(); i++) {
//            int blv = atoi(m_info->picVec[i].c_str());
//            if (m_info->level<=blv) {
//                lv = i+1;
//                break;
//            }
//            else {
//                if (i == m_info->picVec.size()-1) {
//                    lv = 5;
//                }
//            }
//        }
//        lv = lv>5?5:lv;
        
        string ccbName = m_info->pic + "_" + CC_ITOA(GlobalData::shared()->contryResType) + "_" + CC_ITOA(lv);
        CCBLoadFile(ccbName.c_str(),m_mainNode,this);
        
        m_FunBuildLv = FunBuildLv::create(itemId,nameLayer,lvBatchNode);
        m_FunBuildLvNode->addChild(m_FunBuildLv);
        
        if(m_touchNode==NULL){
            return  false;
        }
        
        auto pt = m_touchNode->getPosition();
        mainWidth = (m_touchNode->getContentSize().width*m_touchNode->getScaleX()/2+pt.x)*2;
        mainHeight = (m_touchNode->getContentSize().height*m_touchNode->getScaleY()/2+pt.y)*2;
        
        if (m_batchNode && m_tile) {
            m_tile->removeFromParent();
            m_tile = NULL;
        }
        
        bool tmpIsLock = false;
        if (m_info->type == FUN_BUILD_WORKSHOP) {
            int retForge = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_FORGE);
            if (retForge==0) {
                tmpIsLock = true;
            }
        }
        else if (m_info->open>0 &&m_info->type != FUN_BUILD_MAIN && m_info->open>FunBuildController::getInstance()->getMainCityLv()) {
            tmpIsLock = true;
        }
        
        if (tmpIsLock) {
            m_lockIcon = CCLoadSprite::createSprite("build_lock.png");
            m_lockIcon->setAnchorPoint(ccp(0.5, 0.5));
            int lockX = mainWidth/2;
            int lockY = mainHeight/2;
            m_lockIcon->setPosition(ccp(lockX, lockY));
            m_FunBuildLv->setFunBuildLvShow(false);
        }
        onCheckOutPut();
        
        m_effectSpr = CCSprite::create();
        m_buildState = NULL;
        
        m_moveFrame = CCLoadSprite::createSprite("Tile_frame.png");
        m_moveFrame->setPosition(ccp(93, 56));
        m_moveFrame->setVisible(false);
        if (FunBuildController::getInstance()->getTileType(m_info->pos) == 1) {//内城
            m_moveFrame->setScale(1.3);
            m_moveFrame->setPosition(ccp(131, 67));
        }

    
        //begin a by ljf
//        initEffectState();
        initSpineNode(m_info->pic + "_" + CC_ITOA(GlobalData::shared()->contryResType) + "_1", m_spineLayer);
//        initParticle(m_info->type);
//        if(m_spineNode and m_nameLayer)
//        {
//            m_spineNode->setCameraMask(m_nameLayer->getCameraMask(), true);
//        }
        //end a by ljf
    }
    else
    {
        string tileName = "res_tile_1.png";
        if(itemId==17||itemId==19||itemId==23||itemId==25||itemId==28) {
            tileName = "res_tile_4.png";
        }else if(itemId==18||itemId==20||itemId==22||itemId==26){
            tileName = "res_tile_3.png";
        }else if(itemId==21||itemId==27||itemId==29){
            tileName = "res_tile_2.png";
        }else if(itemId==24||itemId==30){
            tileName = "res_tile_1.png";
        }
        
        m_tile = CCLoadSprite::createSprite(tileName.c_str());
        m_tile->getTexture()->setAntiAliasTexParameters();
        this->setContentSize(m_tile->getContentSize());
        m_tile->setAnchorPoint(ccp(0,0));
        mainWidth = m_tile->getContentSize().width;
        mainHeight = m_tile->getContentSize().height;
        
        auto& tileInfo = FunBuildController::getInstance()->m_tileMap[itemId];
        if (tileInfo.state == FUN_BUILD_LOCK || tileInfo.xmlOpen==1) {
            m_tile->setVisible(false);
        }
        
        m_moveFrame = CCLoadSprite::createSprite("Tile_frame.png");
        m_moveFrame->setPosition(ccp(93, 56));
        m_moveFrame->setVisible(false);
        if (FunBuildController::getInstance()->getTileType(itemId) == 1) {//内城
            m_moveFrame->setScale(1.3);
            m_moveFrame->setPosition(ccp(131, 67));
        }
        
        return true;
    }
    this->setContentSize(CCSizeMake(mainWidth, mainHeight));
    this->setAnchorPoint(ccp(0,0));
    
    
    if(this->getAnimationManager()->getSequenceId("Default Timeline") >=0) {
        this->getAnimationManager()->runAnimationsForSequenceNamed("Default Timeline");
    }
    
    if(m_info->type == FUN_BUILD_FORGE) {
        //auto particle = ParticleController::createParticle("SmithyFireLoop_2");
        //particle->setPosition(CCPoint(10, -20));
        //m_forgeEffectNode->addChild(particle);
    }
    return true;
}

void FunBuild::onShowBuildState(CCObject* params)
{
    int itemId = dynamic_cast<CCInteger*>(params)->getValue();
    if (itemId == m_buildingKey) {
        addFunBuildState();
    }
}

void FunBuild::onRemoveBuildState(CCObject* params)
{
    int itemId = dynamic_cast<CCInteger*>(params)->getValue();
    if (itemId == m_buildingKey) {
        removeFunBuildState();
        if(m_buildingKey/1000 != FUN_BUILD_FORT) {
            this->getAnimationManager()->runAnimationsForSequenceNamed("TrainingFadeOut");
        }
        if(m_info->type == FUN_BUILD_FORGE)
        {
            this->unschedule(schedule_selector(FunBuild::showForgeFire));
            onHideForgeFirePar();
        }
        
        if (m_info->type == FUN_BUILD_WORKSHOP) {
            onHideSpeParticle();
        }
        else {
            onHideParticle();
        }
        onShowParticleByType(2);
    }
    if(itemId==FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_FORGE)){
        //装备检测
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("showEquipTips");
    }
}

void FunBuild::unLockTile(CCObject* params)
{
    if(params) {
        int itemId = dynamic_cast<CCInteger*>(params)->getValue();
        if (itemId == m_buildingKey && m_tile) {
           // m_tile->setOpacity(0);
            m_tile->setVisible(true);
            auto delate = CCDelayTime::create(1.6);
            auto fadeIn = CCFadeIn::create(1.0);
            m_tile->runAction(CCSequence::create(delate,fadeIn,NULL));
        }
    }
}

void FunBuild::onOpenLock()
{
    if (m_lockIcon) {
        if (m_info->type == FUN_BUILD_WORKSHOP) {
            int retForge = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_FORGE);
            if (retForge!=0) {
                playLockJump();
                isUnLock = true;
            }
        }
        else if (m_info->open<=FunBuildController::getInstance()->getMainCityLv()) {
            playLockJump();
            isUnLock = true;
        }
    }
}

void FunBuild::playLockJump()
{
    CCActionInterval* move = CCMoveBy::create(0.5, ccp(0, 10));
    CCActionInterval * easeElasticIn= CCEaseElasticIn::create(move);
    CCActionInterval* move1 = CCMoveBy::create(0.5, ccp(0, -10));
    CCActionInterval * easeElasticIn1= CCEaseElasticIn::create(move1);
    CCSequence* sequence = CCSequence::create(easeElasticIn,easeElasticIn1,NULL);
    m_lockIcon->runAction(CCRepeatForever::create(sequence));
}

void FunBuild::playOpenLock()
{

    //tips 该建筑 解锁 103059=开启
    string info = _lang("103059").c_str();
    info.append(_lang(m_info->name.c_str()).c_str());
    CCCommonUtils::flyHint("", "", info.c_str());
    isUnLock = false;
    
    m_main_touchLayer=m_lockIcon->getParent()->getParent();
    auto zOder=m_lockIcon->getParent()->getZOrder();
    auto pos=m_lockIcon->getPosition();
    m_lockIcon->stopAllActions();
    m_lockIcon->setVisible(false);
    m_lockIcon->removeFromParent();
    m_lockIcon = NULL;
    
    const string spineJsonName = "Spine/Imperial/zhuchengjiesuo.json";
    const string spineAtlasName = "Imperial/Imperial_30.atlas";
    
    if (CCFileUtils::sharedFileUtils()->isFileExist(spineJsonName)&&CCFileUtils::sharedFileUtils()->isFileExist(spineAtlasName))
    {
        m_lockSpine = new IFSkeletonAnimation(spineJsonName.c_str(), spineAtlasName.c_str());
        if (m_lockSpine)
        {
            m_main_touchLayer->addChild(m_lockSpine,zOder);
            m_lockSpine->setVisibleStop(false);
            m_lockSpine->setPosition(pos);
            m_lockSpine->setAnimation(0, "animation", false);
            m_lockSpine->setCompleteListener([&](int trackIndx, int loopCnt){
                m_lockSpine->removeFromParent();
            });
        }
    }

    
    
    m_FunBuildLv->setFunBuildLvShow(true);
}

void FunBuild::setTileBatch(int x, int y, CCSpriteBatchNode* batchNode,CCSpriteBatchNode* downBatchNode,CCSpriteBatchNode* scaffoldsBatchNode,int zOrder)
{
    parentX = x;
    parentY = y;
    m_batchNode = batchNode;
    m_downBatchNode = downBatchNode;
    m_scaffoldsBatchNode=scaffoldsBatchNode;
    m_zOrder = zOrder;
    
    m_tile->setPosition(ccp(parentX, parentY));
    //m_tile->setOpacity(0);
    m_downBatchNode->addChild(m_tile);
    m_sprArray->addObject(m_tile);
    
    //m_tile->setZOrder(zOrder*1000);
    
//    if (m_fixBubble)
//    {
//        m_fixBubble->setPosition(parentPt + m_fixBubble->getPosition());
//        m_downBatchNode->addChild(m_fixBubble, m_tile->getZOrder() + 1);
//    }
//    if (m_lockIcon)
//    {
//        m_lockIcon->setPosition(parentPt + m_lockIcon->getPosition());
//        m_downBatchNode->addChild(m_lockIcon, m_tile->getZOrder() + 1);
//    }
//    
//    if (!m_moveFrame.isNULL()) {
        m_moveFrame->setPosition(ccp(parentX, parentY) + m_moveFrame->getPosition());
        m_downBatchNode->addChild(m_moveFrame);
        m_sprArray->addObject(m_moveFrame);
        m_moveFrame->setZOrder(zOrder*1000+1);
//    }
}

void FunBuild::addTips(){
    if(m_info->type == FUN_BUILD_SACRIFICE && !m_tipNode && !m_buildState){
        m_tipNode = CCNode::create();
        AllianceIntroTip *introTip =  AllianceIntroTip::createAutoAdjust(AllianceIntroTip_Top, _lang("115062"), 0);
        introTip->setTag(9);
        m_tipNode->addChild(introTip);
        introTip->setAnimationAndPlay(3,5);
        m_tipNode->setPosition(ccp(parentX+mainWidth/2, parentY + mainHeight));
        m_popLayer->addChild(m_tipNode,m_zOrder);
    }
}
void FunBuild::removeTips(){
    if(m_popLayer && m_tipNode){
        m_popLayer->removeChild(m_tipNode);
        m_tipNode=NULL;
    }
}
void FunBuild::setNamePos(int x, int y, CCLayer* sginLayer, CCLayer* popLayer, CCSpriteBatchNode* popBatchNode, CCSpriteBatchNode* batchNode,CCSpriteBatchNode* downBatchNode,CCSpriteBatchNode* scaffoldsBatchNode, int zOrder, CCSpriteBatchNode* blentbatch)
{
    if (m_buildingKey/1000 == FUN_BUILD_PRISON || m_buildingKey < 1000) {
        return ;
    }
    parentX = x;
    parentY = y;
    
    m_signLayer = sginLayer;
    m_popBatchNode = popBatchNode;
    m_batchNode = batchNode;
    m_downBatchNode = downBatchNode;
    m_scaffoldsBatchNode=scaffoldsBatchNode;
    m_blentBatchNode = blentbatch;
    m_popLayer = popLayer;
    
    if(m_FunBuildLv)
    {
        m_FunBuildLv->setNamePos(x+m_FunBuildLvNode->getPosition().x, y+m_FunBuildLvNode->getPosition().y);
    }
    
    if (m_lockIcon) {
        m_lockIcon->setPosition(m_lockIcon->getPosition()+ccp(x, y));
        m_popBatchNode->addChild(m_lockIcon);
    }
    
    m_signNode->setPosition(ccp(x,y));
    m_upEffectNode->setPosition(ccp(x+mainWidth/2,y+mainHeight/2));
    m_forgeEffectNode->setPosition(ccp(x+mainWidth/2,y+mainHeight/2));
    sginLayer->addChild(m_signNode);
    sginLayer->addChild(m_upEffectNode);
    sginLayer->addChild(m_forgeEffectNode);
    
    int tmpOffx = parentX;
    int tmpOffy = parentY;
    if(m_info->type==FUN_BUILD_BARRACK1 || m_info->type==FUN_BUILD_BARRACK2 || m_info->type==FUN_BUILD_BARRACK3
       || m_info->type==FUN_BUILD_BARRACK4 || m_info->type==FUN_BUILD_FORT || m_info->type==FUN_BUILD_CELLAR
       || m_info->type==FUN_BUILD_SMITHY || m_info->type==FUN_BUILD_TRAINFIELD || m_info->type==FUN_BUILD_FORGE || m_info->type==FUN_BUILD_SCIENE)
    {
        tmpOffx = 0;
        tmpOffy = 0;
        m_batchNode->setPosition(ccp(parentX, parentY));
    }
    
    m_zOrder = zOrder;
    int tmpOrd = 0;
    
    m_moveFrame->setPosition(ccp(tmpOffx, tmpOffy) + m_moveFrame->getPosition());
    downBatchNode->addChild(m_moveFrame);
    m_sprArray->addObject(m_moveFrame);
    m_moveFrame->setZOrder(zOrder*1000+tmpOrd);
    tmpOrd ++;
    
    if(m_mainNode) {
        auto& array = m_mainNode->getChildren();
        if (array.size()>0) {
            CCNode* tmpNode = dynamic_cast<CCNode*>(array.at(0));//ccb 中的layer层
            if (tmpNode) {
                auto arr1 = tmpNode->getChildren();
                if (arr1.size()>0) {
                    for (int k=0; k<arr1.size(); k++) {
                        CCNode* allSprNode = dynamic_cast<CCNode*>(arr1.at(k));//ccb 中的layer的 node
                        if (allSprNode) {
                            auto& arrSpr = allSprNode->getChildren();
                            if (true) {
                                int j = 0;
                                for (int i=0; i<arrSpr.size(); i++) {
                                    j++;
                                    CCSprite* tmpSpr = dynamic_cast<CCSprite*>(arrSpr.at(i));
                                    if (tmpSpr) {
                                        tmpSpr->removeFromParent();
                                        tmpSpr->getTexture()->setAntiAliasTexParameters();
                                        tmpSpr->setPosition(ccp(tmpSpr->getPositionX()+tmpOffx, tmpSpr->getPositionY()+tmpOffy));
                                        
                                        if (m_blentBatchNode) {
                                            auto batchBlend = m_blentBatchNode->getBlendFunc();
                                            auto particleBlend = tmpSpr->getBlendFunc();
                                            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                                                m_blentBatchNode->addChild(tmpSpr, zOrder*1000+tmpOrd);
                                                m_blentSprArray->addObject(tmpSpr);
                                            }
                                            else {
                                                m_batchNode->addChild(tmpSpr, zOrder*1000+tmpOrd);
                                                m_sprArray->addObject(tmpSpr);
                                            }
                                        }
                                        else {
                                            if(m_info->type == FUN_BUILD_MAIN && tmpSpr->getTag() == 100){
                                                if(m_spr1){
                                                    m_spr1=NULL;
                                                }
                                                if(m_customSkinPar){
                                                    m_customSkinPar->removeFromParent();
                                                    m_customSkinPar=NULL;
                                                }
                                                if(m_customSkinSkBatch)
                                                {
                                                    m_customSkinSkBatch->removeFromParentAndCleanup(true);
                                                    m_customSkinSkBatch = nullptr;
                                                }
                                                
                                                auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
                                                if(layer && ToolController::getInstance()->canChangeSkin() ){
                                                    string newName = ToolController::getInstance()->getMaincitySkin();
                                                    CCSpriteBatchNode* bachNode = layer->createCustomBatchLayer(newName);
                                                    if(bachNode){
                                                        newName.append(".png");
                                                        m_spr1 = CCLoadSprite::createSprite(newName.c_str());
                                                        m_spr1->setAnchorPoint(ccp(0, 0));
                                                        m_spr1->getTexture()->setAntiAliasTexParameters();
                                                        m_spr1->setPosition(ccp(-124.7,-71.0) + ccp(tmpOffx,tmpOffy));
                                                        
                                                        bachNode->addChild(m_spr1, zOrder*1000+tmpOrd);
                                                        m_sprArray->addObject(m_spr1);
                                                        addCustomSkinParticle();
                                                    }else{
                                                        m_batchNode->addChild(tmpSpr, zOrder*1000+tmpOrd);
                                                        m_sprArray->addObject(tmpSpr);
                                                    }
                                                }else{
                                                    m_batchNode->addChild(tmpSpr, zOrder*1000+tmpOrd);
                                                    m_sprArray->addObject(tmpSpr);
                                                }
                                                m_ccbSprPosition = tmpSpr->getPosition();
                                                m_spr1Zorder = zOrder*1000+tmpOrd;
                                            }else{
                                                m_batchNode->addChild(tmpSpr, zOrder*1000+tmpOrd);
                                                m_sprArray->addObject(tmpSpr);
                                            }
                                        }
    //                                    tmpSpr->setZOrder(zOrder*1000+tmpOrd);
                                        tmpOrd++;
                                        i--;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else {
    }
    if (m_info->type != FUN_BUILD_FORGE) {
        drowEffectSpr(zOrder, tmpOrd);
    }
//    drowEffectSpr(zOrder, tmpOrd);
    drowPersonSpr(zOrder, tmpOrd);
    
    if (isCanRecState()) {
        addFunBuildState();
        if(m_buildState) {
            m_buildState->onShowRecState();
        }
        if(m_info->type != FUN_BUILD_FORT) {
            //TODO FIXBUG
            //this->getAnimationManager()->runAnimationsForSequenceNamed("TrainingLoop");
        }
        onShowParticleByType(1);
    }
    if (m_info->state == FUN_BUILD_CREATE) {
        showUping();
    }
    
//    if(m_info->type == FUN_BUILD_WOOD || m_info->type == FUN_BUILD_FOOD || m_info->type == FUN_BUILD_IRON || m_info->type == FUN_BUILD_STONE) {
//        this->scheduleOnce(schedule_selector(FunBuild::onStartFlyOutPut), (m_info->pos%10)*2);
//    }
    if(m_info->type == FUN_BUILD_CELLAR){
        m_allianceNode = CCNode::create();
        m_popLayer->addChild(m_allianceNode,m_zOrder);
        if(AllianceDailyController::getInstance()->maxColorTip>-1){
            addAllianceNode(CCInteger::create(AllianceDailyController::getInstance()->maxColorTip));
        }
    }
}


void FunBuild::setSpineLayer(CCLayer* spineLayer)
{
    if(m_spineNode)
    {
        m_spineLayer = spineLayer;
        m_spineLayer->setPosition(ccp(parentX, parentY)+m_spineNode->getPosition());
        if(m_spineAni)
        {
            m_spineAni->removeFromParent();
            m_spineLayer->addChild(m_spineAni,3);
        }
    }
}
//begin a by ljf
void FunBuild::initSpineNode(string picName, Node * spineParent)
{
    if(m_spineNode)
    {
        const string spineJsonName = "Spine/Imperial/" + picName + "_spine.json";
        const string spineAtlasName = "Imperial/Imperial_30.atlas";

        if (CCFileUtils::sharedFileUtils()->isFileExist(spineJsonName) &&
            CCFileUtils::sharedFileUtils()->isFileExist(spineAtlasName))
        {
            m_spineAni = new IFSkeletonAnimation(spineJsonName.c_str(), spineAtlasName.c_str());
            if (m_spineAni) {
                {
                    m_spineAni->setVisibleStop(false);
                    m_spineNode->addChild(m_spineAni,3,1);
                    spTrackEntry* entry = m_spineAni->setAnimation(0, "Working", true);
//                    if(entry){
//                        m_spineAni->setTimeScale(entry->endTime/1.0f);
//                    }
                }
            }
        }
    }
}
//end a by ljf

void FunBuild::onGetMsgTmpSkinChange(Ref* ref)
{
    if (!m_info)
    {
        return;
    }
    if (m_info->type != FUN_BUILD_MAIN) {
        return;
    }
    if (!ref) {
        return;
    }
    __String* ccstr = dynamic_cast<__String*>(ref);
    if (!ccstr) {
        return;
    }
    string newName = ccstr->getCString();
    if (newName.empty()) {
        return;
    }
    string skinName = newName;
    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    removeSkinParticle();
    if (newName == "mainCity") {
        Sprite* spr = dynamic_cast<Sprite*>(m_batchNode->getChildByTag(100));
        if(layer)
            layer->removeCustomBatchLayer();
        if (!spr)
        {
            auto tmpspr = CCLoadSprite::createSprite("pic400000_2.png");
            tmpspr->setTag(100);
            tmpspr->setZOrder(m_spr1Zorder);
            tmpspr->setPosition(m_ccbSprPosition);
            tmpspr->setAnchorPoint(Vec2(0, 0));
            m_batchNode->addChild(tmpspr);
            m_sprArray->addObject(tmpspr);
        }
    } else {
        m_batchNode->removeChildByTag(100);
        auto batchNode = layer->createCustomBatchLayer(newName);
        if(batchNode){
            newName.append(".png");
            m_spr1 = CCLoadSprite::createSprite(newName.c_str());
            m_spr1->setAnchorPoint(ccp(0, 0));
            m_spr1->getTexture()->setAntiAliasTexParameters();
            m_spr1->setPosition(ccp(-124.7,-71.0) + ccp(parentX,parentY));
            
            batchNode->addChild(m_spr1, m_spr1Zorder);
            m_sprArray->addObject(m_spr1);
            addTmpSkinParticle(skinName);
        }else{
            auto tmpSpr = CCLoadSprite::createSprite("pic400000_2.png");
            tmpSpr->setTag(100);
            tmpSpr->setPosition(m_ccbSprPosition);
            tmpSpr->setAnchorPoint(Vec2(0, 0));
            m_batchNode->addChild(tmpSpr, m_spr1Zorder);
            m_sprArray->addObject(tmpSpr);
        }
    }
}

void FunBuild::onBuildDelete()
{
    if(!GlobalData::shared()->isInitFlag){
        return;
    }
    if (m_popBatchNode) {
        if (m_lockIcon && m_lockIcon->getParent() == m_popBatchNode) {
            m_popBatchNode->removeChild(m_lockIcon, true);
        }
    }
    
    if (m_signLayer) {
        hideUping();
        if (m_signNode) {
            m_signNode->removeFromParentAndCleanup(true);
        }
        if (m_upEffectNode) {
            m_upEffectNode->removeFromParentAndCleanup(true);
        }
        if (m_forgeEffectNode) {
            m_forgeEffectNode->removeFromParentAndCleanup(true);
        }
        if(m_customSkinPar){
            m_customSkinPar->removeFromParentAndCleanup(true);
            m_customSkinPar=NULL;
        }
        if (m_customSkinSkBatch)
        {
            m_customSkinSkBatch->removeFromParentAndCleanup(true);
            m_customSkinSkBatch = nullptr;
        }
        onHideParticle();
        onHideSpeParticle();
        onHideForgeFirePar();
    }
    
    if (m_nameLayer && m_FunBuildLv) {
        m_FunBuildLv->onBuildLvDelete();
    }
    
    removeFunBuildState();
    removeTips();
    for (int i=0; i<m_sprArray->count(); i++) {
        CCSprite* spr = dynamic_cast<CCSprite*>(m_sprArray->objectAtIndex(i));
        if (spr)
        {
            spr->removeFromParentAndCleanup(true);
        }
        
//        m_batchNode->removeChild(spr, true);
    }
    m_sprArray->removeAllObjects();
    
    for (int i=0; i<m_personArray->count(); i++) {
        Person* person = dynamic_cast<Person*>(m_personArray->objectAtIndex(i));
        person->onPersonDelete();
    }
    m_personArray->removeAllObjects();
    if (m_blentBatchNode) {
        for (int i=0; i<m_blentSprArray->count(); i++) {
            CCSprite* spr = dynamic_cast<CCSprite*>(m_blentSprArray->objectAtIndex(i));
            m_blentBatchNode->removeChild(spr, true);
        }
        m_blentSprArray->removeAllObjects();
    }
    
    if (m_spineLayer) {
//        if (m_spineAni) {
//            m_spineAni->removeFromParentAndCleanup(true);
//        }
        if (m_spineLayer) {
            m_spineLayer->removeAllChildrenWithCleanup(true);
        }
    }
    
    hideTmpBuild();
}

void FunBuild::onStartFlyOutPut()
{
    if (m_info->type == FUN_BUILD_STONE) {
        this->schedule(schedule_selector(FunBuild::onFlyOutPut), 720);
    }
    else if (m_info->type == FUN_BUILD_IRON) {
        this->schedule(schedule_selector(FunBuild::onFlyOutPut), 180);
    }
    else {
        this->schedule(schedule_selector(FunBuild::onFlyOutPut), 30);
    }
}

void FunBuild::onFlyOutPut(float _time)
{
    if (true) {
//        int ot = m_output/120;
//        if (m_info->type == FUN_BUILD_STONE) {
//            ot = m_output/5; // 720刷新
//        }
//        else if (m_info->type == FUN_BUILD_IRON) {
//            ot = m_output/20; // 180刷新
//        }
        int ot = FunBuildController::getInstance()->getOutPut(m_info->itemId);
        string temp = "+";
        temp = temp+CC_ITOA(ot);
        CCCommonUtils::flyUiResText(temp.c_str(), this->m_signNode,ccp(70, 60) ,ccGREEN, floating_type_ui1, 23);
    }
}

void FunBuild::onFlyOutPutByValue(int Value)
{
    int ot = Value;
    string temp = "+";
    temp = temp+CC_ITOA(ot);
    CCCommonUtils::flyUiResText(temp.c_str(), this->m_signNode,ccp(70, 60) ,ccGREEN, floating_type_ui1, 23);
}

void FunBuild::quickProduceTroop(CCObject* param)
{
    std::string paramString = dynamic_cast<CCString*>(param)->getCString();
    std::vector<string> paramVect;
    CCCommonUtils::splitString(paramString, ";", paramVect);
    int itemId = atoi(paramVect[0].c_str());
    bool bAutoRec = atoi(paramVect[1].c_str()) == 1? true:false;
    if (itemId == m_buildingKey && isCanRecState()) {
        addFunBuildState();
        if(m_buildState) {
            m_buildState->onShowRecState();
        }
        if(m_info->type != FUN_BUILD_FORT) {
            this->getAnimationManager()->runAnimationsForSequenceNamed("TrainingLoop");
        }
        onShowParticleByType(1);
        
        if(bAutoRec){
            if(m_buildState){
                m_buildState->autoClickRecBtn();
            }
        }
    }
}

void FunBuild::buildFunctionEffect(CCObject* param)
{
    if(!param){
        return;
    }
    int itemId = dynamic_cast<CCInteger*>(param)->getValue();
    if (itemId == m_buildingKey) {
        this->checkIncreasedStatus();
    }
}

void FunBuild::addWorkShopBuildState(CCObject* param){
    int btype = m_buildingKey/1000;
    if (m_popLayer && m_buildState==nullptr && btype==FUN_BUILD_WORKSHOP) {
        removeTips();
        m_buildState = FunBuildState::create(m_buildingKey);
        m_buildState->setPosition(ccp(parentX+mainWidth/2, parentY));
        m_popLayer->addChild(m_buildState,m_zOrder);
        
        isEffectRunning = false;
        m_signNode->removeAllChildren();
    }
}

void FunBuild::addFunBuildState()
{
    if (m_popLayer && m_buildState==nullptr) {
        removeTips();
        m_buildState = FunBuildState::create(m_buildingKey);
        if (m_buildState != nullptr) {
            m_buildState->setPosition(ccp(parentX+mainWidth/2, parentY));
            if (m_buildingKey == FUN_BUILD_MAIN_CITY_ID) {
                m_buildState->setPosition(ccp(parentX+mainWidth/2, parentY+100));
            }
            m_popLayer->addChild(m_buildState,m_zOrder);
        }
    }
}

void FunBuild::addAllianceNode(CCObject* param)
{
    if(m_info && m_info->type == FUN_BUILD_CELLAR){
        int color = dynamic_cast<CCInteger*>(param)->getValue();
        if (color>-1) {
            m_allianceNode->setVisible(true);
        }else{
            m_allianceNode->setVisible(false);
            return;
        }
        m_allianceNode->removeAllChildren();

        m_allianceNode->setContentSize(CCSizeMake(100, 100));
        m_allianceNode->setAnchorPoint(ccp(0.5, 0.5));
        auto btnbg = CCLoadSprite::createSprite("bnt_02.png");
        btnbg->setPosition(ccp(50,50));
        m_allianceNode->addChild(btnbg);
        
        string particleName = "Task_white";
        switch (color) {
            case 0:
                particleName = "Task_white";
                break;
            case 1:
                particleName = "Task_green";
                break;
            case 2:
                particleName = "Task_blue";
                break;
            case 3:
                particleName = "Task_purple";
                break;
            case 4:
                particleName = "Task_org";
                break;
            case 5:
                particleName = "Task_yell";
                break;
            default:
                break;
        }
        auto particle = ParticleController::createParticle(CCString::createWithFormat(particleName.c_str())->getCString());
        particle->setAnchorPoint(ccp(0.5, 0.5));
        particle->setPosition(ccp(50, 50));
        particle->setScale(1.2f);
        m_allianceNode->addChild(particle);
        
        auto tIcon = CCLoadSprite::createSprite("alliance_dail_quest_b.png");
        tIcon->setPosition(ccp(50,50));
        m_allianceNode->addChild(tIcon);
        
        m_allianceNode->setPosition(ccp(parentX+mainWidth/2, parentY+250));
    }
}

void FunBuild::removeFunBuildState()
{
    if (m_popLayer && m_buildState) {
        m_popLayer->removeChild(m_buildState);
        m_buildState = NULL;
    }
}

bool FunBuild::isCanRecState()
{
    int tqid = QID_MAX;
    int buildType = m_buildingKey/1000;
    int qType = CCCommonUtils::getQueueTypeByBuildType(buildType);
    if (qType>0) {
        tqid = QueueController::getInstance()->getCanRecQidByType(qType);
    }
    if (tqid != QID_MAX) {
        return true;
    }
    return false;
}

void FunBuild::onCheckOutPut()
{
    if(!GlobalData::shared()->isInitFlag){
        return;
    }
    if (m_info == NULL) {
        return;
    }
    if (m_info->type == FUN_BUILD_WOOD || m_info->type == FUN_BUILD_FOOD || m_info->type == FUN_BUILD_IRON || m_info->type == FUN_BUILD_STONE) {
        if(m_info->para.size() < 2){
            return;
        }
        m_output = atoi(m_info->para[1].c_str());
        if (m_info->type == FUN_BUILD_WOOD) {
            m_output = m_output*(MAX((100+CCCommonUtils::getEffectValueByNum(50))*1.0/100, 0));
        }
        else if (m_info->type == FUN_BUILD_FOOD) {
            m_output = m_output*(MAX((100+CCCommonUtils::getEffectValueByNum(53))*1.0/100, 0));
        }
        else if (m_info->type == FUN_BUILD_IRON) {
            m_output = m_output*(MAX((100+CCCommonUtils::getEffectValueByNum(52))*1.0/100, 0));
        }
        else if (m_info->type == FUN_BUILD_STONE) {
            m_output = m_output*(MAX((100+CCCommonUtils::getEffectValueByNum(51))*1.0/100, 0));
        }
    }
}

void FunBuild::onResetDirc(float _time)
{
    m_info->is_Dirc = false;
}
void FunBuild::showUping()
{
    m_signNode->removeAllChildren();
    if(m_info->type == FUN_BUILD_FORGE)
    {
        m_forgeEffectNode->removeAllChildren();
    }
    if ( m_info->is_Dirc ) {
        onShowParticleByType(1);
        this->scheduleOnce(schedule_selector(FunBuild::onResetDirc), 60);
        return;
    }
    
    int tmppX = 0;
    int tmppY = 0;

    if (m_info->type == FUN_BUILD_MAIN) {
        m_buildCCB = BuildCCB::create(CITYBUILD_MAIN);
        tmppX = -mainWidth/2  + 244;
        tmppY = -mainHeight/2 - 22;
    }
    else if (m_info->type == FUN_BUILD_TAVERN) {
        m_buildCCB = BuildCCB::create(CITYBUILD_MAIN);
        tmppX = -mainWidth/2  + 194;
        tmppY = -mainHeight/2 - 17;
    }
    else if (m_info->type == FUN_BUILD_FOOD || m_info->type == FUN_BUILD_WOOD || m_info->type == FUN_BUILD_IRON || m_info->type == FUN_BUILD_STONE || m_info->type == FUN_BUILD_HOSPITAL){
        m_buildCCB = BuildCCB::create(CITYBUILD_SMALL);
        tmppX = -mainWidth/2  - 24;
        tmppY = -mainHeight/2 - 24;
    }
    else if (m_info->type == FUN_BUILD_WALL) {
        m_buildCCB = BuildCCB::create(CITYBUILD_WALL);
        tmppX = -mainWidth/2  - 51;
        tmppY = -mainHeight/2 + 23;
    }
    else {
        m_buildCCB = BuildCCB::create(CITYBUILD_NORMAL);
        tmppX = -mainWidth/2  + 45;
        tmppY = -mainHeight/2 - 45;
    }
    int tmpOffx = parentX;
    int tmpOffy = parentY;
    
    if(m_info->type==FUN_BUILD_BARRACK1 || m_info->type==FUN_BUILD_BARRACK2 || m_info->type==FUN_BUILD_BARRACK3
       || m_info->type==FUN_BUILD_BARRACK4 || m_info->type==FUN_BUILD_FORT || m_info->type==FUN_BUILD_CELLAR
       || m_info->type==FUN_BUILD_SMITHY || m_info->type==FUN_BUILD_TRAINFIELD || m_info->type==FUN_BUILD_FORGE || m_info->type==FUN_BUILD_SCIENE)
    {
        tmpOffx = 0;
        tmpOffy = 0;
    }
    m_buildCCB->setNamePos(tmpOffx+mainWidth/2+tmppX, tmpOffy+mainHeight/2+tmppY, m_signLayer, m_scaffoldsBatchNode, m_zOrder);
    m_signNode->addChild(m_buildCCB);
    
    
//    string upBgPic = "inner_create.png";
//    if (m_info->type == FUN_BUILD_MAIN) {
//        upBgPic = "main_city_up.png";
//    }
//    auto _cbg = CCLoadSprite::createSprite(upBgPic.c_str());
//    _cbg->getTexture()->setAntiAliasTexParameters();
//    _cbg->setAnchorPoint(ccp(0.5,0));
//    _cbg->setPosition(ccp(mainWidth/2, 0));
//    m_signNode->addChild(_cbg);
    
    auto sprAni = CCSprite::create();
    CCCommonUtils::makeEffectSpr(sprAni, "inner_risk_%d.png", 18);
    sprAni->setAnchorPoint(ccp(0.5, 0.5));
    sprAni->setScale(2.0);
    int tmpY = 108;//_cbg->getContentSize().height;
    if (m_info->type == FUN_BUILD_MAIN) {
        tmpY = 220;
    }
    sprAni->setPosition(ccp(mainWidth/2, tmpY-40));
    
    auto garyCloud1 = CCSprite::create();
    CCCommonUtils::makeEffectSpr(garyCloud1, "gary_cloud_%d.png", 13);
    garyCloud1->setAnchorPoint(ccp(1, 0));
    garyCloud1->setPosition(ccp(mainWidth/2+30, -40));
    auto garyCloud2 = CCSprite::create();
    CCCommonUtils::makeEffectSpr(garyCloud2, "gary_cloud_%d.png", 13);
    garyCloud2->setFlipX(true);
    garyCloud2->setPosition(ccp(mainWidth/2+60, 20));
    
    m_signNode->addChild(sprAni);
    m_signNode->addChild(garyCloud1);
    m_signNode->addChild(garyCloud2);
    
    if (m_info->type == FUN_BUILD_MAIN) {
        garyCloud1->setPosition(ccp(mainWidth/2-70, -70));
        garyCloud2->setPosition(ccp(mainWidth/2-30, -20));
        
        auto garyCloud3 = CCSprite::create();
        CCCommonUtils::makeEffectSpr(garyCloud3, "gary_cloud_%d.png", 13);
        garyCloud3->setFlipX(true);
        garyCloud3->setPosition(ccp(mainWidth/2+100, 45));
        auto garyCloud4 = CCSprite::create();
        CCCommonUtils::makeEffectSpr(garyCloud4, "gary_cloud_%d.png", 13);
        garyCloud4->setFlipX(true);
        garyCloud4->setPosition(ccp(mainWidth/2+200, 100));
        
        m_signNode->addChild(garyCloud3);
        m_signNode->addChild(garyCloud4);
    }
    
    if(this->getAnimationManager()->getSequenceId("Update") >=0) {
        this->getAnimationManager()->runAnimationsForSequenceNamed("Update");
    }
    
    if ( !m_info->is_Dirc ) {
        addFunBuildState();
    }
}

void FunBuild::hideUping()
{
    if(m_buildCCB) {
        m_buildCCB->onBuildDelete();
        m_buildCCB->removeFromParent();
        m_buildCCB = NULL;
    }
    m_signNode->removeAllChildren();
}

void FunBuild::changeBg()
{
    if(m_buildCCB) {
        m_buildCCB->onBuildDelete();
        m_buildCCB->removeFromParent();
        m_buildCCB = NULL;
    }
    m_signNode->removeAllChildren();
}

void FunBuild::setTime(int times, bool updateLv)
{
    if(m_info && (m_info->state == FUN_BUILD_UPING || m_info->state == FUN_BUILD_DESTROY) && isShowUping==false)
    {
        isShowUping = true;
        showUping();
    }
    else if (isShowUping==true && times<=0)
    {
        isShowUping = false;
        hideUping();
        removeFunBuildState();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                               , CCString::createWithFormat("BU_%d_cd", m_buildingKey/1000));
    }
    
    if (m_FunBuildLv != nullptr && (times<=0 || updateLv)) {
        onCheckOutPut();
        m_FunBuildLv->setLv();
    }
    
    if(times > 0 && isShowUping) {
    }
}

void FunBuild::onUpdateUpIcon(float dt)
{
    if(m_info==NULL) {
        return;
    }
    m_FunBuildLv->onUpdateUpIcon(dt);
}

void FunBuild::onEnter() {
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FunBuild::onRemoveBuildState), MSG_REMOVE_STATE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FunBuild::onShowBuildState), MSG_SHOW_TIME_BAR, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FunBuild::unLockTile), MSG_UNLOCK_TILE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FunBuild::quickProduceTroop), MSG_QUICK_TROOPS, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FunBuild::buildFunctionEffect), MSG_BUILD_FUNCTION_EFFECT, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FunBuild::showHarvestAni), MSG_SHOW_SKILL_HARVEST, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FunBuild::playFortSkillEffect), MSG_SHOW_FORT_SKILL_EFFECT, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FunBuild::onMoveBuild), MSG_MOVE_BUILD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FunBuild::onCancelLastMoveBuild), MSG_CANCEL_LAST_MOVE_BUILD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FunBuild::onCancelMoveBuild), MSG_CANCEL_BUILD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FunBuild::playBuildShadow), MSG_PLAY_BUILD_SHADOW, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FunBuild::stopBuildShadow), MSG_STOP_BUILD_SHADOW, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FunBuild::addWorkShopBuildState), "addFunBuildState", NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FunBuild::addAllianceNode), "addAllianceMaxColor", NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FunBuild::onGetMsgTmpSkinChange), MSG_TMP_SKIN_CHANGE, nullptr);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FunBuild::setSoilderVisible), MSG_WALLSOILDER_VISIBLE, nullptr);
    if(m_info && m_info->type == FUN_BUILD_FORT)
    {
        this->scheduleOnce(schedule_selector(FunBuild::playFortSkillEffect), 0.45);
    }
}

void FunBuild::onExit() {
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REMOVE_STATE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SHOW_TIME_BAR);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_UNLOCK_TILE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_QUICK_TROOPS);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BUILD_FUNCTION_EFFECT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SHOW_SKILL_HARVEST);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SHOW_FORT_SKILL_EFFECT);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MOVE_BUILD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CANCEL_LAST_MOVE_BUILD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CANCEL_BUILD);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PLAY_BUILD_SHADOW);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_STOP_BUILD_SHADOW);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "addFunBuildState");
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "addAllianceMaxColor");
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TMP_SKIN_CHANGE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_WALLSOILDER_VISIBLE);
    CCNode::onExit();
}

bool FunBuild::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    bool ret = false;
    STClickName = "";
//  ret = isTouchInTexture(m_bg, pTouch);
    if (m_buildingKey<1000) {
        if (m_tile->isVisible()) {
            ret = isTouchInside(m_tile, pTouch);
        }
    }
    else {
        if (m_buildingKey/1000 == FUN_BUILD_PRISON) {
            ret = false;
        }
        else if(isTouchInside(m_allianceNode, pTouch)){
            ret = true;
        }
//        else if(m_buildingKey/1000 == FUN_BUILD_QIJI){
//            ret = false;
//        }
        else {
            if ( m_info && m_info->is_Dirc ) {
                return ret;
            }
            ret = isTouchInside(m_touchNode, pTouch);
        }
        
        if (!ret && m_buildingKey/1000 == FUN_BUILD_SACRIFICE) {
            if(m_tipNode && m_tipNode->getChildByTag(9)){
                AllianceIntroTip* tip = dynamic_cast<AllianceIntroTip*>(m_tipNode->getChildByTag(9));
                if (tip && tip->m_bg->getOpacity() > 0) {
                    ret = isTouchInside(tip->m_touchNode, pTouch);
                    if (ret) {
                        STClickName = CC_ITOA(FUN_BUILD_SACRIFICE);
                    }
                }
            }
        }
    }
    
    return ret;
}

CCPoint FunBuild::IsNeedMove(float x, float y, float scale)
{
    int retX = 0;
    int retY = 0;
    float bottomY = 150;
    float topY = 150;
    auto size = CCDirector::sharedDirector()->getWinSize();
    float minX = nameWidth*scale/2;
    float maxX = size.width-minX;
    float minY = nameHeight*scale+bottomY;
    float maxY = size.height-topY;
    
    if (x<minX) {
        retX = minX;
    }
    if (x>maxX) {
        retX = maxX;
    }
    if (x>maxX && x<minX) {
        retX = size.width/2;
    }
    
    if (y<minY) {
        retY = minY;
    }
    if (y>maxY) {
        retY = maxY;
    }
    if (y>maxY && y<minY) {
        retY = size.height/2;
    }
    
    return ccp(retX, retY);
}

void FunBuild::setGary()
{
    if(m_spr) {
        m_spr->setColor(ccGRAY);
    }
    else if (m_tile) {
        m_tile->setColor(ccGRAY);
    }
    if(m_spr1){
        m_spr1->setColor(ccGRAY);
    }
    if (m_buildingKey == FUN_BUILD_WALL_ID) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SET_GARY);
    }
}

void FunBuild::setWhite()
{
    if(m_spr) {
        m_spr->setColor(ccWHITE);
    }
    else if (m_tile) {
        m_tile->setColor(ccWHITE);
    }
    if(m_spr1){
        m_spr1->setColor(ccWHITE);
    }
    if (m_buildingKey == FUN_BUILD_WALL_ID) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SET_WHITE);
    }

}

void FunBuild::playBuildShadow(CCObject* obj){
    if (obj) {
        int key = (dynamic_cast<CCString*>(obj))->intValue();
        if (key == m_buildingKey) {
//            playShadow();
            CCTintTo* tinto1 = CCTintTo::create(0.5, 107, 205, 26);
            CCTintTo* tinto2 = CCTintTo::create(0.5, 255, 255, 255);
            CCSequence* seq = CCSequence::create(tinto1,tinto2,NULL);
            if(m_spr) {
                m_spr->runAction(CCRepeatForever::create(seq));
            }
            if(m_spr1){
                m_spr1->runAction(CCRepeatForever::create(seq));
            }
        }
    }
}
void FunBuild::stopBuildShadow(CCObject* obj){
    if (obj) {
        int key = (dynamic_cast<CCString*>(obj))->intValue();
        if (key == m_buildingKey) {
            stopShadow();
        }
    }
}

void FunBuild::playShadow()
{
    CCTintTo* tinto1 = CCTintTo::create(0.5, 166, 166, 166);
    CCTintTo* tinto2 = CCTintTo::create(0.5, 255, 255, 255);
    
    CCSequence* seq = CCSequence::create(tinto1,tinto2,NULL);
    if(m_spr) {
        m_spr->runAction(CCRepeatForever::create(seq));
    }
    if(m_spr1){
        m_spr1->runAction(CCRepeatForever::create(seq));
    }
    if (m_buildingKey == FUN_BUILD_WALL_ID) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PLAY_SHADOW);
    }
    
}

void FunBuild::stopShadow()
{
    if(m_spr) {
        m_spr->stopAllActions();
        m_spr->setColor(ccWHITE);
    }
    if(m_spr1){
        m_spr1->stopAllActions();
        m_spr1->setColor(ccWHITE);
    }
    if (m_buildingKey == FUN_BUILD_WALL_ID) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_STOP_SHADOW);
    }
}

void FunBuild::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isCanClick) {
        return;
    }

    if(isTouchInside(m_allianceNode, pTouch)){
//        m_allianceNode->setVisible(false);
        if (FunBuildController::getInstance()->getMainCityLv() >= 10) {
            PopupViewController::getInstance()->addPopupInView(AllianceDailyPublishView::create());
        }else{
            CCCommonUtils::flyHint("", "", _lang_1("134068", CC_ITOA(10)));
        }
        return;
    }

    m_clickType = 0;
    
    if (FunBuildController::getInstance()->OpenMoveBuildStat) {
        int pos = getCurPos();
        if (pos>0) {
            int cnt = (int)FunBuildController::getInstance()->WillMovePos.size();
            if (cnt == 0) {
                if (m_buildingKey>1000) {
                    FunBuildController::getInstance()->SetMoveBuildPos(pos);
                }
            }else if (cnt == 1){
                int firstPos = FunBuildController::getInstance()->WillMovePos[0];
                if (FunBuildController::getInstance()->getTileType(firstPos) == 2 && FunBuildController::getInstance()->getTileType(pos) == 2 && firstPos!=pos) {
                    FunBuildController::getInstance()->SetMoveBuildPos(pos);
                }else if (FunBuildController::getInstance()->getTileType(firstPos) == 1 && firstPos!=pos) {
                    if (FunBuildController::getInstance()->getTileType(pos) == 1) {
                        FunBuildController::getInstance()->SetMoveBuildPos(pos);
                    }
                }
            }
        }
    }else {
        isCanClick=false;
        setGary();
        scheduleOnce(schedule_selector(FunBuild::onClickThis), 0.1f);
    }
}

bool FunBuild::isTouchInBtns(CCTouch *pTouch, CCEvent *pEvent)
{
    return false;
}

void FunBuild::onSetClickType()
{
    m_clickType = 1;
}

bool FunBuild::isUnlockThis(){
    if(m_lockIcon && m_lockIcon->isVisible() && !isUnLock)
    {
        string tmpInfo = _lang_2("102184", _lang(m_info->name).c_str(), CC_ITOA(m_info->open));
        if (m_info->type == FUN_BUILD_WORKSHOP) {
            tmpInfo = _lang("111400");
        }
        CCCommonUtils::flyHint("", "", tmpInfo);
        return false;
    }
    return true;
}

void FunBuild::onClickThis(float _time)
{
    setWhite();
    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    if (layer) {
        layer->hideFlyArrow(0);
    }
    if (m_buildState && m_buildState->isCanGather() && layer && FunBuildController::getInstance()->canPointArrow) {
        layer->setPointArrowAni(m_buildingKey);
    }
    FunBuildController::getInstance()->canPointArrow = false;
    
    if(m_buildingKey<1000)
    {
        auto& tileInfo = FunBuildController::getInstance()->m_tileMap[m_buildingKey];
        if (tileInfo.state == FUN_BUILD_ULOCK) {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_SAVEPOS);
            if(layer) {
                layer->setUnMoveScence(false);
                layer->onMoveToBuildAndOpen(m_buildingKey, TYPE_POS_UP);
                SoundController::sharedSound()->playEffects(Music_Sfx_city_space);
                PopupViewController::getInstance()->addPopupInView(BuildListView::create(m_buildingKey));
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                                       , CCString::createWithFormat("MC_%d", m_buildingKey));
            }
        }
        
    }
    else
    {
        if(m_info->type == FUN_BUILD_PRISON)
        {
            CCCommonUtils::flyText(_lang("E100008"));
            scheduleOnce(schedule_selector(FunBuild::onCanClick), 0.2f);
            return;
        }
        
        if(m_lockIcon && m_lockIcon->isVisible() && !isUnLock)
        {
            string tmpInfo = _lang_2("102184", _lang(m_info->name).c_str(), CC_ITOA(m_info->open));
            if (m_info->type == FUN_BUILD_WORKSHOP) {
                tmpInfo = _lang("111400");
            }
            CCCommonUtils::flyHint("", "", tmpInfo);
            scheduleOnce(schedule_selector(FunBuild::onCanClick), 0.2f);
            return;
        }
        if (isUnLock) {
            playOpenLock();
            scheduleOnce(schedule_selector(FunBuild::onCanClick), 0.2f);
            return;
        }

        if(m_info->type == FUN_BUILD_WORKSHOP) {
            if (m_buildState && m_buildState->isCanClick()) {
                m_buildState->ClickState(); //点击状态
            }else {
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_SAVEPOS);
                if( layer )
                {
                    layer->setUnMoveScence(false);  //设成不动模式
                    layer->onMoveToBuildAndOpen(m_buildingKey, TYPE_POS_MID);  //打开模式，位置由类型确定
                }
                PopupViewController::getInstance()->addPopupView(ToolCreateView::create());  //
            }
            scheduleOnce(schedule_selector(FunBuild::onCanClick), 0.2f);
            return;
        }
        
        if (m_info->type == FUN_BUILD_SACRIFICE && STClickName == CC_ITOA(FUN_BUILD_SACRIFICE)) {
            PopupViewController::getInstance()->addPopupInView(SacrificePopUpView::create());
            scheduleOnce(schedule_selector(FunBuild::onCanClick), 0.2f);
            return;
        }
        
        if (true) {
            if (m_buildState && m_buildState->isCanClick()) {
                if(m_clickType==0 && layer) {
                    layer->onResetLastBuildId();
                    m_buildState->ClickState();
                }
                if (layer) {
                    layer->setQuestEffect(0);
                }
            }
            else {
                
                if (m_info->type == FUN_BUILD_BARRACK1 || m_info->type == FUN_BUILD_BARRACK2 || m_info->type == FUN_BUILD_BARRACK3 || m_info->type == FUN_BUILD_BARRACK4 ||m_info->type == FUN_BUILD_FORT ||m_info->type == FUN_BUILD_FORGE)
                {
                    int qtype = CCCommonUtils::getQueueTypeByBuildType(m_info->type);
                    if (qtype>0) {//弥补 在可收兵的时候，未显示收兵状态下的处理
                        int qid = QueueController::getInstance()->getCanRecQidByType(qtype);
                        if (qid != QID_MAX) {
                            if (m_info->type == FUN_BUILD_FORGE) {
                                //收装备
                                auto& qInfo = GlobalData::shared()->allQueuesInfo[qid];
                                EquipmentController::getInstance()->startFinishCrtEquip(qInfo.uuid);
                                QueueController::getInstance()->startFinishQueue(qid, false);
                            }else {
                                QueueController::getInstance()->collectSolider(qid,m_buildingKey);
                            }
                        }
                    }
                }
                
                if(layer) {
                    auto pt = layer->onGetSceenPt(parentX+mainWidth/2, parentY);
                    auto movePt = IsNeedMove(pt.x, pt.y, layer->onGetTouchLayerScale());
                    if(movePt.x>0 || movePt.y>0) {
                        int ex = pt.x;
                        int ey = pt.y;
                        if (movePt.x>0) {
                            ex = movePt.x;
                        }
                        if (movePt.y>0) {
                            ey = movePt.y;
                        }
                        layer->onMoveToPosDir(parentX+mainWidth/2, parentY, ex, ey);
                    }
                }
                
                int dh = 0;
                if (m_buildingKey==FUN_BUILD_MAIN_CITY_ID) {
                    dh = 100;
                }
                if (!m_info->is_Dirc && layer) {
                    layer->onShowBtnsView(parentX+mainWidth/2, parentY+dh, m_buildingKey);
//                    addFunBuildState();
                    playShadow();
                }
            }
            if (m_clickType==0 && layer) {
                layer->setUnMoveScence(false);
            }
            m_clickType = 0;
        }

        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                               , CCString::createWithFormat("MC_%d", m_buildingKey/1000));
    }
    scheduleOnce(schedule_selector(FunBuild::onCanClick), 0.2f);
}

void FunBuild::onCanClick(float _time)
{
    isCanClick = true;
}

void FunBuild::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
}

void FunBuild::onPlayUpEnd(bool isOnly)
{
    m_info = &((*FunBuildController::getInstance()->curBuildsInfo)[m_buildingKey]);
    this->unschedule(schedule_selector(FunBuild::onResetDirc));
    m_upEffectNode->stopAllActions();
    m_upEffectNode->removeAllChildren();
    //SoundController::sharedSound()->playEffects(Music_Sfx_city_complete);
    if (true) {
        float scale = 2.3;
        if (m_info->type==FUN_BUILD_WOOD || m_info->type==FUN_BUILD_FOOD || m_info->type==FUN_BUILD_IRON || m_info->type==FUN_BUILD_STONE || m_info->type==FUN_BUILD_BARRACK || m_info->type==FUN_BUILD_HOSPITAL ) {
            scale = 1.0;
        }
        if (m_info->type == FUN_BUILD_MAIN) {
            scale = 3;
            auto tmp_ptArray = CCPointArray::create(20);
            tmp_ptArray->addControlPoint(ccp(-140, 130));
            tmp_ptArray->addControlPoint(ccp(0, 300));
            tmp_ptArray->addControlPoint(ccp(100, 150));
            
            for (int j=0; j<tmp_ptArray->count(); j++) {
                for (int i=1; i<5; i++) {
                    auto particle = ParticleController::createParticle(CCString::createWithFormat("Fireworks_%d",i)->getCString());
                    particle->setPosition(tmp_ptArray->getControlPointAtIndex(j));
                    m_upEffectNode->addChild(particle);
                }
            }
            
            cocos2d::CCDictionary* result = CCDictionary::create();
            result->setObject(CCString::create(CC_ITOA(ITEM_FIREWORK2)), "itemId");
            if (m_info->level >= 10) {
                result->setObject(CCString::create(CC_ITOA(40)), "num");
            }
            
            auto callback = [=]{
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("showFirework", result);
            };
            auto delay = DelayTime::create(1.5);
            auto func = CallFunc::create(callback);
            CCSequence *sequene = CCSequence::create(delay, func, NULL);
            Director::getInstance()->getRunningScene()->runAction(sequene);
        }
        auto per1Spr = Sprite::create();
        per1Spr->setBlendFunc({GL_ONE, GL_ONE});
        per1Spr->setAnchorPoint(Vec2(0.5,0.5));
        per1Spr->setScale(scale);
        CCCommonUtils::makeOnceEffectSpr(per1Spr, "build_upgrade_%d.png", 10, 0.1);
        per1Spr->setPositionY(mainHeight * 0.2);
        m_upEffectNode->addChild(per1Spr);
        
        if(!isOnly) {
            auto& queue = QueueController::getInstance()->m_buildQueueInfo;
            auto it = queue.begin();
            for (; it != queue.end(); ++it)
            {
                if (it->second.type != TYPE_BUILDING) {
                    continue;
                }
                if (it->second.key == "") {
                    continue;
                }
                if (atoi(it->second.key.c_str()) != m_info->itemId) {
                    continue;
                }
                if ((int)(it->second.finishTime) > GlobalData::shared()->getWorldTime()) {
                    continue;
                }
                break;
            }
            if (it != queue.end()) {
                scheduleOnce(schedule_selector(FunBuild::onStopUpEffect), 0.6f);
                scheduleOnce(schedule_selector(FunBuild::playCompleteSound), 1.0f);
                it->second.startTime = 0;
                it->second.finishTime = 0;
                it->second.endTime = 0;
                it->second.key = "";
            } else {
                scheduleOnce(schedule_selector(FunBuild::onStopUpEffect), 2.6f);
                scheduleOnce(schedule_selector(FunBuild::playCompleteSound), 3.0f);
                if(m_info->starNum == 0 && (int)m_info->starRate == 0){
                    string para1 = _lang(m_info->name);
                    string para2 = string(" Lv") + CC_ITOA(m_info->level) + string(" ");
                    if(m_info->type != FUN_BUILD_QIJI){
                        CCCommonUtils::flyHint("", "", _lang_2("137480", para1.c_str(), para2.c_str()), 2, 0, true);
                    }
                    else{
                        CCCommonUtils::flyHint("", "", _lang("160024").c_str());
                    }
                }else{
                    if ((int)m_info->starRate == 0 ) {
                        CCCommonUtils::flyHint("", "", _lang_1("160020",  _lang(m_info->name).c_str()));
                    }
                    else{
                        CCCommonUtils::flyHint("", "", _lang_1("160019",  _lang(m_info->name).c_str()));
                    }
                }
                SoundController::sharedSound()->playEffects(Music_Sfx_UI_buildfinish);
            }
        } else {
            scheduleOnce(schedule_selector(FunBuild::playCompleteSound), 1.0f);
        }
        
    }
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("MC_%d_2", m_info->itemId));
}

void FunBuild::playCompleteSound(float _time)
{
    SoundController::sharedSound()->playEffects(Music_Sfx_city_complete);
}

void FunBuild::onStopUpEffect(float _time)
{
    if(!GlobalData::shared()->isInitFlag){
        return;
    }
    m_upEffectNode->removeAllChildren();
    
    if(m_info->type == FUN_BUILD_FORGE)
    {
        //auto particle = ParticleController::createParticle("SmithyFireLoop_2");
        //particle->setPosition(CCPoint(10, -20));
        //m_forgeEffectNode->addChild(particle);
    }
    
    long addPower = 0;
    if(m_info->level<= 30 && m_info->starNum == 0&& m_info->starRate == 0){
        addPower = m_info->mapPower[m_info->level];
    }else{
        addPower = m_info->getStarPower(m_info->starNum,m_info->starRate);
    }
    UIComponent::getInstance()->showFlyCBView(m_info->oldExp, addPower);
    scheduleOnce(schedule_selector(FunBuild::onPlayBall), 1.0f);
}

void FunBuild::onPlayBall(float _time)
{
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FLYBALL_EFFECT, CCInteger::create(m_info->itemId));
}

void FunBuild::canShowState()
{
    if (m_info->type == FUN_BUILD_MAIN) {
        auto now = WorldController::getInstance()->getTime();
        if (m_parVec.size()==0 && now < GlobalData::shared()->cityFireStamp) {
            onShowParticle();
        }
        else if (m_parVec.size()>0 && now >= GlobalData::shared()->cityFireStamp) {
            onHideParticle();
        }
    }
    else if (m_info->type == FUN_BUILD_SCIENE) {
        if (!isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_SCIENCE)>0) {
            addFunBuildState();
            this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
            isEffectRunning = true;
        }
        else if (isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_SCIENCE)<=0 ) {
            removeFunBuildState();
            this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
            isEffectRunning = false;
        }
    }
    else if (m_info->type == FUN_BUILD_FORGE) {
        if (!isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_FORGE)>0) {
            addFunBuildState();
            this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
            drowEffectSpr(0,0);
            isEffectRunning = true;
        }
        else if (isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_FORGE)<=0 ) {
//            removeFunBuildState();
//            this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
//            onHideParticle();
            isEffectRunning = false;
            
            int qid = QueueController::getInstance()->getCanRecQidByType(TYPE_FORGE);
            if (qid != QID_MAX) {
                if(m_buildState) {
                    m_buildState->onShowRecState();
                }
                this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
                onShowParticleByType(1);
            }else {
                removeFunBuildState();
                onHideParticle();
                this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
            }
        }
    }
    else if (m_info->type == FUN_BUILD_WORKSHOP) {
        if (!isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_MATE)>0) {
            addFunBuildState();
            this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
            onShowParticleByType(7);
            
            if (CCFileUtils::sharedFileUtils()->isFileExist("Spine/Imperial/lianjingongfang.json") &&
                CCFileUtils::sharedFileUtils()->isFileExist("Imperial/Imperial_30.atlas"))
            {
                auto animationObj = new IFSkeletonAnimation("Spine/Imperial/lianjingongfang.json","Imperial/Imperial_30.atlas");
                if (animationObj) {
                    animationObj->setVisibleStop(false);
                    animationObj->setPosition(ccp(100, -20));
                    m_signNode->addChild(animationObj);
                    spTrackEntry* entry = animationObj->setAnimation(0, "gongzuo", true);
                    animationObj->setTimeScale(entry->endTime/8.0f);
                    animationObj->autorelease();
                }
            }
            
            int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_MATE);
            if(qid != QID_MAX) {
                auto& qInfo = GlobalData::shared()->allQueuesInfo[qid];
                int sid = QueueController::getInstance()->getItemId(qInfo.key);
                auto icon = CCLoadSprite::createSprite( CCCommonUtils::getIcon(CC_ITOA(sid)).c_str() );
                icon->setPosition(ccp(100, 70));
                m_signNode->addChild(icon);
                CCActionInterval * moveBy1 = CCMoveBy::create(0.5,ccp(0, 20));
                CCActionInterval * moveBy2 = CCMoveBy::create(0.5,ccp(0, -20));
                CCActionInterval * repeat = CCRepeatForever::create( CCSequence::create(moveBy1, moveBy2, NULL) );
                icon->runAction(repeat);
            }
            m_signNode->setZOrder(999999);
            isEffectRunning = true;
        }
        else if (isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_MATE)<=0 ) {
            isEffectRunning = false;
            m_signNode->removeAllChildren();
            onHideSpeParticle();
            int qid = QueueController::getInstance()->getCanRecQidByType(TYPE_MATE);
            if (qid != QID_MAX) {
                if(m_buildState) {
                    m_buildState->onShowRecState();
                }
                this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
                onShowParticleByType(1);
                
                QueueController::getInstance()->refreshMateQueue(m_buildingKey);
            }else {
                removeFunBuildState();
                onHideSpeParticle();
                this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
            }
        }
        else if (!isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_MATE)<=0 ) {
             if(m_buildState==nullptr && isCanRecState()) {
                 int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_MATE);
                 auto& qInfo = GlobalData::shared()->allQueuesInfo[qid];
                 if (qInfo.allId!="") {
                     addFunBuildState();
                     if(m_buildState) {
                         m_buildState->onShowRecState();
                     }
                     onShowParticleByType(1);
                 }
             }else{
                 if (!isCanRecState()) {
                     removeFunBuildState();
                     onHideSpeParticle();
                 }
                 
             }
        }
    }
    else if (m_info->type == FUN_BUILD_HOSPITAL) {
        if (!isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_HOSPITAL)>0) {
            addFunBuildState();
            isEffectRunning = true;
        }
        else if (isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_HOSPITAL)<=0 ) {
            removeFunBuildState();
            isEffectRunning = false;
        }
        
        if (m_buildAnimatSt!="Help" && ArmyController::getInstance()->getTotalDead()>0) {
            this->getAnimationManager()->runAnimationsForSequenceNamed("Help");
            m_buildAnimatSt="Help";
            onShowParticleByType(4);
        }
        else if (m_buildAnimatSt!="Default" && ArmyController::getInstance()->getTotalDead()<=0) {
            this->getAnimationManager()->runAnimationsForSequenceNamed("Default");
            m_buildAnimatSt="Default";
            onHideParticle();
        }
    }
    else if (m_info->type == FUN_BUILD_BARRACK1) {
        if (!isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_FORCE)>0) {
            isEffectRunning = true;
            addFunBuildState();
            this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
        }
        else if (isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_FORCE)<=0 ) {
            isEffectRunning = false;
            int qid = QueueController::getInstance()->getCanRecQidByType(TYPE_FORCE);
            if (qid != QID_MAX) {
                if(m_buildState) {
                    m_buildState->onShowRecState();
                }
                this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
                onShowParticleByType(1);
            }else {
                removeFunBuildState();
                this->getAnimationManager()->runAnimationsForSequenceNamed("TrainingFadeOut");
            }
        }
    }
    else if (m_info->type == FUN_BUILD_BARRACK2) {
        if (!isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_RIDE_SOLDIER)>0) {
            isEffectRunning = true;
            addFunBuildState();
            this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
        }
        else if (isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_RIDE_SOLDIER)<=0 ) {
            isEffectRunning = false;
            int qid = QueueController::getInstance()->getCanRecQidByType(TYPE_RIDE_SOLDIER);
            if (qid != QID_MAX) {
                if(m_buildState) {
                    m_buildState->onShowRecState();
                }
                this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
                onShowParticleByType(1);
            }else {
                removeFunBuildState();
                this->getAnimationManager()->runAnimationsForSequenceNamed("TrainingFadeOut");
            }
        }
    }
    else if (m_info->type == FUN_BUILD_BARRACK3) {
        if (!isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_BOW_SOLDIER)>0) {
            isEffectRunning = true;
            addFunBuildState();
            this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
        }
        else if (isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_BOW_SOLDIER)<=0 ) {
            isEffectRunning = false;
            int qid = QueueController::getInstance()->getCanRecQidByType(TYPE_BOW_SOLDIER);
            if (qid != QID_MAX) {
                if(m_buildState) {
                    m_buildState->onShowRecState();
                }
                this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
                onShowParticleByType(1);
            }else {
                removeFunBuildState();
                this->getAnimationManager()->runAnimationsForSequenceNamed("TrainingFadeOut");
            }
        }
    }
    else if (m_info->type == FUN_BUILD_BARRACK4) {
        if (!isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_CAR_SOLDIER)>0) {
            isEffectRunning = true;
            addFunBuildState();
            this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
        }
        else if (isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_CAR_SOLDIER)<=0 ) {
            isEffectRunning = false;
            int qid = QueueController::getInstance()->getCanRecQidByType(TYPE_CAR_SOLDIER);
            if (qid != QID_MAX) {
                if(m_buildState) {
                    m_buildState->onShowRecState();
                }
                this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
                onShowParticleByType(1);
            }else {
                removeFunBuildState();
                this->getAnimationManager()->runAnimationsForSequenceNamed("TrainingFadeOut");
            }
        }
    }
    else if (m_info->type == FUN_BUILD_FORT) {
        if (!isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_FORT)>0) {
            isEffectRunning = true;
            addFunBuildState();
            this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
        }
        else if (isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_FORT)<=0 ) {
            isEffectRunning = false;
            int qid = QueueController::getInstance()->getCanRecQidByType(TYPE_FORT);
            if (qid != QID_MAX) {
                if(m_buildState) {
                    m_buildState->onShowRecState();
                }
                this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
                onShowParticleByType(1);
            }else {
                removeFunBuildState();
                this->getAnimationManager()->runAnimationsForSequenceNamed("TrainingFadeOut");
            }
        }
    }
    else if (m_info->type == FUN_BUILD_STABLE) {
        if (!isEffectRunning && GlobalData::shared()->playerInfo.allianceInfo.helpcount>0) {//有帮助需求
            isEffectRunning = true;
            addFunBuildState();
            m_buildState->onCheckAllianceHelp();
        }
        else if (isEffectRunning && GlobalData::shared()->playerInfo.allianceInfo.helpcount<=0 ) {//没有有帮助需求
            if(m_buildState) {
                m_buildState->onHideBtn(4);
            }
            if (m_info->state == FUN_BUILD_NORMAL) {
                removeFunBuildState();
            }
            isEffectRunning = false;
        }
    }
    else if (m_info->type == FUN_BUILD_WOOD || m_info->type == FUN_BUILD_FOOD || m_info->type == FUN_BUILD_IRON || m_info->type == FUN_BUILD_STONE) {
        if (!isEffectRunning && FunBuildController::getInstance()->canShowOutPut(m_info->itemId)) {
            addFunBuildState();
            isEffectRunning = true;
            if(m_info->type == FUN_BUILD_FOOD || m_info->type == FUN_BUILD_WOOD) {
                this->getAnimationManager()->runAnimationsForSequenceNamed("GrowthProcess");
                if (m_spineAni) {
                    m_spineAni->setAnimation(0.01, "GrowthProcess", false);
                    m_spineAni->addAnimation(0.01, "Full", true);
                }
            }
        }
        else if (isEffectRunning && !FunBuildController::getInstance()->canShowOutPut(m_info->itemId) && m_buildState && m_buildState->CanDel ) {
            removeFunBuildState();
            isEffectRunning = false;
            if(m_info->type == FUN_BUILD_FOOD || m_info->type == FUN_BUILD_WOOD) {
                this->getAnimationManager()->runAnimationsForSequenceNamed("Havest");
                if (m_spineAni) {
                    m_spineAni->setAnimation(0.01, "Havest", false);
                    m_spineAni->addAnimation(0.01, "Working", true);
                }
            }
        }
    }
    else if (m_info->type == FUN_BUILD_SACRIFICE) {
        if (!isEffectRunning && GlobalData::shared()->sacrificeInfo.canFreePray()==true && FunBuildController::getInstance()->TalkViewBId.compare(CC_ITOA(FUN_BUILD_SACRIFICE)) != 0) {
            addTips();
            isEffectRunning = true;
            onShowParticleByType(6);
        }
        else if (isEffectRunning && GlobalData::shared()->sacrificeInfo.canFreePray()==true && FunBuildController::getInstance()->TalkViewBId.compare(CC_ITOA(FUN_BUILD_SACRIFICE)) == 0) {
            removeTips();
            isEffectRunning = false;
            onHideSpeParticle();
        }
        else if (isEffectRunning && GlobalData::shared()->sacrificeInfo.canFreePray()==false) {
            removeTips();
            isEffectRunning = false;
            onHideSpeParticle();
        }
    }
}

void FunBuild::retTouch(CCTouch *pTouch, CCEvent *pEvent)
{
    if(pTouch==NULL)
        return;
    bool ret = false;
    if (m_buildingKey<1000) {
        ret = isTouchInside(m_tile, pTouch);
    }
    else {
        ret = isTouchInside(m_touchNode, pTouch);
    }
    
    if (ret) {
        this->onTouchEnded(pTouch, pEvent);
    }
}

void FunBuild::onPlayUpIcon()
{
    if(m_FunBuildLv)
    {
        m_FunBuildLv->onPlayUpIcon();
    }
}
void FunBuild::stopUpIcon()
{
    if(m_FunBuildLv)
    {
        m_FunBuildLv->stopUpIcon();
    }
}

void FunBuild::drowEffectSpr(int zOrder, int tmpOrd)
{
    int x = parentX;
    int y = parentY;
    
    if (m_info->type == FUN_BUILD_CELLAR) {
    }
    else if (m_info->type == FUN_BUILD_WATCH_TOWER) {
        onShowParticle();
    }
    else if (m_info->type == FUN_BUILD_SCIENE) {
    }
    else if (m_info->type == FUN_BUILD_FORT) {
    }
    else if (m_info->type == FUN_BUILD_TEMPLE) {
        onShowParticle();
    }
    else if (m_info->type == FUN_BUILD_FOOD) {
//        onShowParticleByType(5);
    }
    else if (m_info->type == FUN_BUILD_WOOD) {
    }
    else if (m_info->type == FUN_BUILD_STONE) {
    }
    else if (m_info->type == FUN_BUILD_SMITHY) {
        onShowParticle();
    }
    else if (m_info->type == FUN_BUILD_TAVERN) {
//        auto effSpr0 = CCSprite::create();
//        CCCommonUtils::makeTwoAnimatSpr(effSpr0, "tanhua_%d.png", 7, 1, 0.2 );
//        effSpr0->setPosition(ccp(x+30, y-10));
//        m_batchNode->addChild(effSpr0);
//        m_sprArray->addObject(effSpr0);
//        effSpr0->setZOrder(zOrder*1000+tmpOrd);
//        tmpOrd++;
    }
    else if (m_info->type == FUN_BUILD_BARRACK) {
    }
    else if (m_info->type == FUN_BUILD_BARRACK1) {
    }
    else if (m_info->type == FUN_BUILD_BARRACK2) {
    }
    else if (m_info->type == FUN_BUILD_BARRACK3) {
    }
    else if (m_info->type == FUN_BUILD_BARRACK4) {
    }
    else if (m_info->type == FUN_BUILD_FORGE) {
       // onShowParticle();
    }
    else if (m_info->type == FUN_BUILD_SACRIFICE){
//        auto particle0 = ParticleController::createParticle(CCString::createWithFormat("Wish_Pool_0")->getCString());
//        particle0->setPosition(ccp(parentX+135, parentY+150));
//        addParticleToBatch(particle0);
//        auto particle1 = ParticleController::createParticle(CCString::createWithFormat("Wish_Pool_1")->getCString());
//        particle1->setPosition(ccp(parentX+135, parentY+140));
//        addParticleToBatch(particle1);
//        auto particle2 = ParticleController::createParticle(CCString::createWithFormat("Wish_Pool_2")->getCString());
//        particle2->setPosition(ccp(parentX+135, parentY+110));
//        addParticleToBatch(particle2);
//        auto particle3 = ParticleController::createParticle(CCString::createWithFormat("Wish_Pool_3")->getCString());
//        particle3->setPosition(ccp(parentX+135, parentY+70));
//        addParticleToBatch(particle3);
//        auto particle4 = ParticleController::createParticle(CCString::createWithFormat("Wish_Pool_4")->getCString());
//        particle4->setPosition(ccp(parentX+135, parentY+70));
//        addParticleToBatch(particle4);
    }
    else if (m_info->type == FUN_BUILD_WORKSHOP)
    {
        onShowParticle();
    }
    else if (m_info->type == FUN_BUILD_TRAINFIELD)
    {
        onShowParticle();
    }
}

void FunBuild::drowPersonSpr(int zOrder, int tmpOrd)
{
    if(m_info->type == FUN_BUILD_MAIN || m_info->type == FUN_BUILD_WALL || m_info->type == FUN_BUILD_WAR)
    {
        m_ptArray = CCPointArray::create(20);
        if (m_info->type == FUN_BUILD_MAIN) {

        }
        else if (m_info->type == FUN_BUILD_WALL) {
            m_ptArray->addControlPoint(ccp(200, -100));
            m_ptArray->addControlPoint(ccp(-200, 100));

        }
        else if (m_info->type == FUN_BUILD_WAR) {
            m_ptArray->addControlPoint(ccp(100, -50));
            m_ptArray->addControlPoint(ccp(100, 50));
            m_ptArray->addControlPoint(ccp(-100, -50));
            m_ptArray->addControlPoint(ccp(-100, 50));

        }
    }
}

void FunBuild::onMoveBuild(CCObject* param)
{
    if (m_buildingKey<1000) {
        if (!m_tile->isVisible()) {
            return;
        }
    }
    
    int pos = getCurPos();
    if (pos < 0) {
        return;
    }
    
    CCString* posInfo = dynamic_cast<CCString*>(param);
    int paramPos = posInfo->intValue();
    int cnt = (int)FunBuildController::getInstance()->WillMovePos.size();
    if (cnt == 0) {
        //所有建筑都播放，地格不播放，只能点建筑
        if (m_buildingKey>1000) {
            float scale1 = 1.5;
            float scale2 = 1.3;
            if (FunBuildController::getInstance()->getTileType(pos) == 2) {
                scale1 = 1.15;
                scale2 = 1.0;
            }
            CCActionInterval * scaleto1 = CCScaleTo::create(0.5, scale1);
            CCActionInterval * scaleto2 = CCScaleTo::create(0.5, scale2);
            CCActionInterval * repeatForever =CCRepeatForever::create(CCSequence::create(scaleto1, scaleto2, NULL));
            m_moveFrame->runAction(repeatForever);
            
            m_moveFrame->setColor(ccGREEN);
            m_moveFrame->setVisible(true);
        }
    }else if (cnt == 1) {
        //paramPos 变颜色, 并取消掉 不可交换的建筑效果; 播放可以移动到地格
        if (pos == paramPos) {
            m_moveFrame->setColor(ccRED);
        }else {
            if (FunBuildController::getInstance()->getTileType(paramPos) == 2 && (FunBuildController::getInstance()->getTileType(pos) == 1 || FunBuildController::getInstance()->getTileType(pos) ==3)) {
                onCancelMoveBuild(NULL);
            }else if (FunBuildController::getInstance()->getTileType(pos) == 2 && (FunBuildController::getInstance()->getTileType(paramPos) == 1 || FunBuildController::getInstance()->getTileType(paramPos) == 3)) {
                onCancelMoveBuild(NULL);
            }else {
                if (m_buildingKey<1000) {
                    float scale1 = 1.5;
                    float scale2 = 1.3;
                    if (FunBuildController::getInstance()->getTileType(pos) == 2) {
                        scale1 = 1.15;
                        scale2 = 1.0;
                    }
                    CCActionInterval * scaleto1 = CCScaleTo::create(0.5, scale1);
                    CCActionInterval * scaleto2 = CCScaleTo::create(0.5, scale2);
                    CCActionInterval * repeatForever =CCRepeatForever::create(CCSequence::create(scaleto1, scaleto2, NULL));
                    m_moveFrame->runAction(repeatForever);
                    
                    m_moveFrame->setColor(ccGREEN);
                    m_moveFrame->setVisible(true);
                }
            }
        }
        
    }else if (cnt == 2) {
        //paramPos 变颜色
        if (pos == paramPos) {
            m_moveFrame->setColor(ccRED);
        }
    }
}

void FunBuild::onCancelLastMoveBuild(CCObject* param)
{
    if (m_buildingKey<1000) {
        if (!m_tile->isVisible()) {
            return;
        }
    }
    
    int pos = getCurPos();
    if (pos < 0) {
        return;
    }
    
    CCString* posInfo = dynamic_cast<CCString*>(param);
    int paramPos = posInfo->intValue();
    if (pos == paramPos) {
        m_moveFrame->setColor(ccGREEN);
    }
}

void FunBuild::onCancelMoveBuild(CCObject* param)
{
    if (m_buildingKey<1000) {
        if (!m_tile->isVisible()) {
            return;
        }
    }
    
    int pos = getCurPos();
    if (pos < 0) {
        return;
    }
    
    m_moveFrame->stopAllActions();
    m_moveFrame->setVisible(false);
}

int FunBuild::getCurPos()
{
    if (m_buildingKey>1000) {
        if (m_info) {
            if (FunBuildController::getInstance()->getTileType(m_info->pos) == 1 || FunBuildController::getInstance()->getTileType(m_info->pos) == 2) {
                return m_info->pos;
            }
            else {
                return -1;
            }
        }else {
            return -1;
        }
    }else {
        if (FunBuildController::getInstance()->getTileType(m_buildingKey) == 1 || FunBuildController::getInstance()->getTileType(m_buildingKey) == 2) {
            return m_buildingKey;
        }
        else {
            return -1;
        }
    }
}

#pragma mark -
#pragma mark CocosBuilder Part
SEL_MenuHandler FunBuild::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler FunBuild::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool FunBuild::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spr", CCSprite*, this->m_spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_FunBuildLvNode", CCNode*, this->m_FunBuildLvNode);

	//begin a by ljf
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spineNode", CCNode*, this->m_spineNode);
    //end a by ljf
    return false;
}

void FunBuild::addSpeParticleToBatch(cocos2d::CCParticleSystemQuad *particle)
{
    auto batchCount = m_speVec.size();
    while (batchCount--) {
        auto &batch = m_speVec[batchCount];
        if (batch) {
            auto batchBlend = batch->getBlendFunc();
            auto particleBlend = particle->getBlendFunc();
            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                batch->addChild(particle);
                return;
            }
        }
    }
    auto newBatch = ParticleController::createParticleBatch();
    m_signLayer->addChild(newBatch);
    newBatch->addChild(particle);
    m_speVec.push_back(newBatch);
}
void FunBuild::onHideSpeParticle()
{
    auto batchCount = m_speVec.size();
    while (batchCount--) {
        m_signLayer->removeChild(m_speVec[batchCount]);
    }
    m_speVec.clear();
}

void FunBuild::onHideForgeFirePar()
{
    if (m_upEffectNode) {
        auto batchCount = m_forgeFireParVec.size();
        while (batchCount--) {
            m_upEffectNode->removeChild(m_forgeFireParVec[batchCount]);
        }
        m_forgeFireParVec.clear();
    }
}

void FunBuild::addForgeFireParToBatch(cocos2d::CCParticleSystemQuad *particle)
{
    auto batchCount = m_forgeFireParVec.size();
    while (batchCount--) {
        auto &batch = m_forgeFireParVec[batchCount];
        if (batch) {
            auto batchBlend = batch->getBlendFunc();
            auto particleBlend = particle->getBlendFunc();
            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                batch->addChild(particle);
                return;
            }
        }
    }
    auto newBatch = ParticleController::createParticleBatch();
    m_upEffectNode->addChild(newBatch);
    newBatch->addChild(particle);
    particle->setAutoRemoveOnFinish(true);
    particle->setDuration(0.5f);
    m_forgeFireParVec.push_back(newBatch);
}

void FunBuild::addParticleToBatch(cocos2d::CCParticleSystemQuad *particle)
{
    auto batchCount = m_parVec.size();
    while (batchCount--) {
        auto &batch = m_parVec[batchCount];
        if (batch) {
            auto batchBlend = batch->getBlendFunc();
            auto particleBlend = particle->getBlendFunc();
            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                batch->addChild(particle);
                return;
            }
        }
    }
    auto newBatch = ParticleController::createParticleBatch();
    m_signLayer->addChild(newBatch);
    newBatch->addChild(particle);
    m_parVec.push_back(newBatch);
}

void FunBuild::onShowParticle()
{
    if (m_info->type == FUN_BUILD_MAIN) {
        auto tmp_ptArray = CCPointArray::create(20);
        tmp_ptArray->addControlPoint(ccp(parentX+270, parentY+210));
        tmp_ptArray->addControlPoint(ccp(parentX+20, parentY+180));
        tmp_ptArray->addControlPoint(ccp(parentX+180, parentY+200));
        tmp_ptArray->addControlPoint(ccp(parentX+120, parentY+80));
        tmp_ptArray->addControlPoint(ccp(parentX+360, parentY+200));
        tmp_ptArray->addControlPoint(ccp(parentX+220, parentY+300));
        string tmpStart = "CityFire_";
        int count = 5;
        for (int j=0; j<tmp_ptArray->count(); j++) {
            for (int i=1; i<count; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
                particle->setPosition(tmp_ptArray->getControlPointAtIndex(j));
                addParticleToBatch(particle);
            }
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SHOW_CITY_FIRE);
    }
    else if (m_info->type == FUN_BUILD_SCIENE) {
        string tmpStart = "Research_";
        int count = 3;
        for (int i=1; i<count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
            particle->setPosition(ccp(parentX+145, parentY+210));
            addParticleToBatch(particle);
        }
    }
    else if (m_info->type == FUN_BUILD_TEMPLE) {
        string tmpStart = "Temple_";
        int count = 9;
        for (int i=1; i<count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
            particle->setPosition(ccp(parentX+100, parentY+130));
            addParticleToBatch(particle);
        }
        
        auto particle1 = ParticleController::createParticle("TempleStar");
        particle1->setPosition(ccp(parentX+50, parentY+130));
        addParticleToBatch(particle1);
        auto particle2 = ParticleController::createParticle("TempleStar");
        particle2->setPosition(ccp(parentX+150, parentY+130));
        addParticleToBatch(particle2);
    }
    else if (m_info->type == FUN_BUILD_FORGE) {
        auto tmp_ptArray = CCPointArray::create(20);
        tmp_ptArray->addControlPoint(ccp(parentX+215, parentY+135));
        tmp_ptArray->addControlPoint(ccp(parentX+60, parentY+170));
        tmp_ptArray->addControlPoint(ccp(parentX+115, parentY+190));
        int count = 2;
        for (int j=0; j<tmp_ptArray->count(); j++) {
            for (int i=1; i<=count; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("Smoke_%d",i)->getCString());
                particle->setPosition(tmp_ptArray->getControlPointAtIndex(j));
                addParticleToBatch(particle);
            }
        }
        
        // 每隔6秒喷一次的火焰
        showForgeFire(0.0f);
        schedule(schedule_selector(FunBuild::showForgeFire),6.0f);
        
    }
    else if (m_info->type == FUN_BUILD_WATCH_TOWER) {
        auto particle1 = ParticleController::createParticle("fire_fenghuotai");
        particle1->setPosition(ccp(parentX + 172, parentY + 281));
        
        auto particle2 = ParticleController::createParticle("smoke_fenghuotai");
        particle2->setPosition(ccp(parentX + 172, parentY + 295));
        addParticleToBatch(particle2);
        addParticleToBatch(particle1);
    }
    else if (m_info->type == FUN_BUILD_WORKSHOP) {
        int count = 3;
        for (int i=1; i<=count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("ArmySpray_%d",i)->getCString());
            particle->setPosition(ccp(parentX+190, parentY+35));
            addParticleToBatch(particle);
        }
    }
    else if (m_info->type == FUN_BUILD_TRAINFIELD) {
        auto leftPos = Vec2(parentX + 95, parentY + 142);
        auto left1 = ParticleController::createParticle("pic427_fire");
        left1->setPosition(leftPos);
        auto left2 = ParticleController::createParticle("pic427_spot");
        left2->setPosition(leftPos);
        addParticleToBatch(left1);
        addParticleToBatch(left2);
        
        auto rightPos = Vec2(parentX + 241, parentY + 69);
        auto right1 = ParticleController::createParticle("pic427_fire");
        right1->setPosition(rightPos);
        auto right2 = ParticleController::createParticle("pic427_spot");
        right2->setPosition(rightPos);
        addParticleToBatch(right1);
        addParticleToBatch(right2);
        
    }
}

void FunBuild::showForgeFire(float dt)
{
    onHideForgeFirePar();
    for(int i=1; i<= 4; i++)
    {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("SmithyFire_%d",i)->getCString());
        particle->setPosition(ccp(0,-20));
        addForgeFireParToBatch(particle);
    }
}

void FunBuild::onHideParticle()
{
    auto batchCount = m_parVec.size();
    while (batchCount--) {
        m_signLayer->removeChild(m_parVec[batchCount]);
    }
    m_parVec.clear();
}

void FunBuild::onShowParticleByType(int type)
{
    if (m_info->type == FUN_BUILD_WORKSHOP){
        onHideSpeParticle();
    }
    if (type == 1) {
        if(m_info->type == FUN_BUILD_FORGE)
        {
            this->unschedule(schedule_selector(FunBuild::showForgeFire));
            onHideForgeFirePar();
            onHideParticle();
            for(int i=1;i<=3;i++)
            {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("Smithy_%d",i)->getCString());
                particle->setPosition(ccp(parentX+mainWidth/2, parentY+100));
                addParticleToBatch(particle);
            }
        }else if (m_info->type == FUN_BUILD_WORKSHOP) {
            onHideSpeParticle();
            for(int i=1;i<=3;i++)
            {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("ManualWorkshop_%d",i)->getCString());
                particle->setPosition(ccp(parentX+93, parentY+48));
                if (i==1) {
                    particle->setStartColor(ccc4f(0.4, 1.0, 0, 0.4));
                    particle->setEndColor(ccc4f(0, 0, 0, 0));
                }else if (i==2) {
                    particle->setStartColor(ccc4f(0.6, 1.0, 0, 0.4));
                    particle->setEndColor(ccc4f(0, 0, 0, 0));
                }else if (i==3) {
                    particle->setStartColor(ccc4f(1.0, 1.0, 1.0, 0.4));
                    particle->setEndColor(ccc4f(0.8, 1.0, 0.4, 0));
                }
                addSpeParticleToBatch(particle);
            }
        }else
        {
            string tmpStart = "Collection_Loop_";
            int count = 4;
            for (int i=1; i<=count; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
                particle->setPosition(ccp(parentX+mainWidth/2, parentY+100));
                addParticleToBatch(particle);
            }
        }
    }
    else if (type == 4) {
        string tmpStart = "Treatment_";
        int count = 3;
        for (int i=1; i<=count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
            particle->setPosition(ccp(parentX+mainWidth/2, parentY+100));
            addParticleToBatch(particle);
        }
    }
    else if (type == 5) {
        string tmpStart = "UiFireBarbecue_";
        int count = 3;
        for (int i=1; i<=count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
            particle->setPosition(ccp(parentX+mainWidth/2, parentY+60));
            addParticleToBatch(particle);
        }
    }
    else if (type == 2) {
        onPlayParticle();
    }
    else if (type == 6) {
//        auto particleL1 = ParticleController::createParticle(CCString::createWithFormat("Wish_Fountain_1")->getCString());
//        particleL1->setPosition(ccp(parentX+135, parentY+70));
//        addSpeParticleToBatch(particleL1);
//        auto particleL2 = ParticleController::createParticle(CCString::createWithFormat("Wish_Fountain_2")->getCString());
//        particleL2->setPosition(ccp(parentX+135, parentY+75));
//        addSpeParticleToBatch(particleL2);
//        auto particleL3 = ParticleController::createParticle(CCString::createWithFormat("Wish_Fountain_S")->getCString());
//        particleL3->setPosition(ccp(parentX+135, parentY+95));
//        addSpeParticleToBatch(particleL3);
//        auto particleL4 = ParticleController::createParticle(CCString::createWithFormat("Wish_Fountain_S")->getCString());
//        particleL4->setPosition(ccp(parentX+80, parentY+65));
//        addSpeParticleToBatch(particleL4);
//        auto particleL5 = ParticleController::createParticle(CCString::createWithFormat("Wish_Fountain_S")->getCString());
//        particleL5->setPosition(ccp(parentX+190, parentY+65));
//        addSpeParticleToBatch(particleL5);
    }
    else if (type == 7) {
        int count = 6;
        for (int i=1; i<=count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("ManualWorkshop_%d",i)->getCString());
            particle->setPosition(ccp(parentX+93, parentY+48));
            if (i==5 || i==6) {
                particle->setPosition(ccp(parentX+93, parentY+48+20));
            }
            addSpeParticleToBatch(particle);
        }
        count = 2;
        for (int i=1; i<=count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("Smoke_%d",i)->getCString());
            particle->setPosition(ccp(parentX+22, parentY+160));
            addSpeParticleToBatch(particle);
        }
    }
    
}

void FunBuild::onPlayParticle()
{
    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    if (layer) {
        auto pt = layer->onGetSceenPt(parentX+mainWidth/2, parentY+mainHeight);
        if(m_buildingKey/1000!=FUN_BUILD_FORGE && m_buildingKey/1000!=FUN_BUILD_WORKSHOP)
        {
            layer->onFlyTroopParticle(pt.x, pt.y, 1);
            if(m_buildingKey/1000 != FUN_BUILD_FORT) {
                layer->onPlayMoveTroops(m_buildingKey);
            }
        }
    }
}

void FunBuild::showTmpBuild(int itemId)
{
    hideTmpBuild();
    
    auto build = FunBuild::createTmpBuild(itemId, parentX, parentY, m_batchNode, m_zOrder, m_blentBatchNode, m_spineLayer);
    build->setTag(9);
//    build->playShadow();
    this->addChild(build);
}

void FunBuild::hideTmpBuild()
{
    CCNode* node = this->getChildByTag(9);
    if (node) {
        FunBuild* build = dynamic_cast<FunBuild*>(node);
        if (build) {
//            build->stopShadow();
            build->onBuildDelete();
        }
    }
    this->removeChildByTag(9);
}

void FunBuild::onPlaySpeEffect(bool show)
{
    float dt = 2.0;
    if (show) {
        m_effectSpr->setOpacity(0);
        m_effectSpr->setVisible(true);
        m_effectSpr->runAction(CCFadeIn::create(dt));
        for (int i=0; i<m_speEffectVec.size(); i++) {
            m_speEffectVec[i]->setOpacity(0);
            m_speEffectVec[i]->setVisible(true);
            m_speEffectVec[i]->runAction(CCFadeIn::create(dt));
        }
    }
    else {
        m_effectSpr->runAction(CCFadeOut::create(dt));
        for (int i=0; i<m_speEffectVec.size(); i++) {
            m_speEffectVec[i]->runAction(CCFadeOut::create(dt));
        }
        scheduleOnce(schedule_selector(FunBuild::onShowSpeEffect), dt);
    }
}
void FunBuild::onShowSpeEffect(float _time)
{
    m_effectSpr->setVisible(isEffectRunning);
    for (int i=0; i<m_speEffectVec.size(); i++) {
        m_speEffectVec[i]->setVisible(isEffectRunning);
    }
}

CCNode* FunBuild::getGuideNode(string _key)
{
    if(m_buildState) {
        return m_buildState->getGuideNode(_key);
    }
    return NULL;
}

void FunBuild::checkIncreasedStatus(){
    double effectTime = m_info->effectTime;
    int gapTime = effectTime - GlobalData::shared()->getWorldTime();
    if(gapTime>0 && effectTime>0){
        if(m_gainSpr!=nullptr){
            return;
        }
        int offX = 15;
        int offY = 0;
        int tmpBatchPosX = parentX;
        int tmpBatchPosY = parentY;
        m_gainSpr = CCSprite::create();
        m_gainSpr->setAnchorPoint({0.5,0});
        m_gainSpr->setBlendFunc({GL_ONE, GL_ONE});
        CCCommonUtils::makeEffectSpr(m_gainSpr, "Gain_%d.png", 8, 0.1);
        m_gainSpr->setOpacity(175);
        m_spineLayer->addChild(m_gainSpr,10);
//        m_batchNode->addChild(m_gainSpr);
        m_sprArray->addObject(m_gainSpr);
        string tmpStart = "IncreasedGlow_";
        int count = 3;
        for (int i=1; i<count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
//            particle->setPosition(ccp(parentX+70+offX, parentY+30+offY));
            particle->setPosition(ccp(parentX+100+offX, parentY+60+offY));
            addSpeParticleToBatch(particle);
        }
    }else{
        if(m_gainSpr){
            m_gainSpr->removeFromParentAndCleanup(true);
        }
        onHideSpeParticle();
    }
}

void FunBuild::showHarvestAni(CCObject* param){
    if(m_info==NULL) return ;
    CCString* time = dynamic_cast<CCString*>(param);
    addResTime = 0;
    if(time!=NULL){
        addResTime = atoi(time->getCString());
    }
    if(addResTime==0) return ;
    HarvestSkillAni* dropCCB = NULL;
    switch (m_info->type) {
        case FUN_BUILD_STONE:
            dropCCB = HarvestSkillAni::create(R_STONE);
            break;
        case FUN_BUILD_WOOD:
            dropCCB = HarvestSkillAni::create(R_WOOD);
            break;
        case FUN_BUILD_IRON:
            dropCCB = HarvestSkillAni::create(R_IRON);
            break;
        case FUN_BUILD_FOOD:
            dropCCB = HarvestSkillAni::create(R_FOOD);
            break;
//        case FUN_BUILD_SILVER:
//            dropCCB = HarvestSkillAni::create(R_SILVER);
//            break;
    }
    if(dropCCB){
        dropCCB->setScale(0.5);
        m_upEffectNode->addChild(dropCCB);
        this->scheduleOnce(schedule_selector(FunBuild::showFlyOut), 2.4);
    }
}

void FunBuild::showFlyOut(float _time){
    int add = FunBuildController::getInstance()->getOutPutPerSecond(m_info->itemId)*addResTime;
    if(add<=0) return;
    int ot = add;
    string temp = "+";
    temp = temp+CC_ITOA(ot);
    CCCommonUtils::flyUiResText(temp.c_str(), this->m_signNode,ccp(70, 60) ,ccGREEN, floating_type_ui1, 28);
}

void FunBuild::playFortSkillEffect(CCObject* params)
{
    playFortSkillEffect(0.0);
}
void FunBuild::playFortSkillEffect(float _time){
    if(m_info && m_info->type == FUN_BUILD_FORT)
    {
        string fortInfo =  GlobalData::shared()->m_skillFortInfo;
        if(fortInfo==""){
            return ;
        }
        std::vector<std::string> data;
        CCCommonUtils::splitString(fortInfo,"|",data);
        if(data.size()!=2) return ;
        
        auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
        if(layer) {
            layer->setUnMoveScence(false);
            layer->onMoveToBuildAndOpen(m_buildingKey, TYPE_POS_UP);
        }
        for(int i=1;i<3;i++){
            auto particle = ParticleController::createParticle(CCString::createWithFormat("Light_%d",i)->getCString());
            particle->setPosition(ccp(-10, 460));
            m_upEffectNode->addChild(particle);
        }
        for(int i=1;i<10;i++){
            if(i==4 ||i==5) continue;
            auto particle = ParticleController::createParticle(CCString::createWithFormat("CollectionOut_%d",i)->getCString());
            particle->setPosition(ccp(0, -30));
            m_upEffectNode->addChild(particle);
        }
        
        auto particle = ParticleController::createParticle(CCString::createWithFormat("Harvest_10")->getCString());
        particle->setPosition(ccp(-5, -55));
        m_upEffectNode->addChild(particle);
        
        int freeNum = atoi(data[1].c_str());
        string fortId = data[0];
        map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->fortList.find(fortId);
        if(it!=GlobalData::shared()->fortList.end()){
            it->second.free = it->second.free + freeNum;
        }
        string icon = "ico" + fortId+ ".png";
        CCSprite* iconSpr = CCLoadSprite::createSprite(icon.c_str());
        iconSpr->setPosition(ccp(0, 350));
        iconSpr->setOpacity(0);
        m_upEffectNode->addChild(iconSpr);
        CCSequence* cs = CCSequence::create(CCDelayTime::create(0.15),CCFadeIn::create(0.2),CCMoveTo::create(0.4, ccp(0, 100)),CCFadeOut::create(0.25),CCRemoveSelf::create(),NULL);
        iconSpr->runAction(cs);
        GlobalData::shared()->m_skillFortInfo = "";
        string addFort = "+" + data[1];
        CCCommonUtils::flyUiResText(addFort.c_str(), this->m_signNode,ccp(130, 120) ,ccGREEN, floating_type_ui1, 28);
    }
}
void FunBuild::addCustomSkinParticle(){
    removeSkinParticle();
    if(ToolController::getInstance()->canChangeSkin() && m_spr1){
        initCustomSkinParticle(ToolController::getInstance()->maincityConfig);
    }
}

void FunBuild::addTmpSkinParticle(string skin)
{
    removeSkinParticle();
    if (!m_spr1)
    {
        return;
    }
    map<string, string> config;
    DynamicResourceController::getInstance()->initMainCityConfig(skin, config);
    initCustomSkinParticle(config);
}

void FunBuild::initCustomSkinParticle(map<string, string>& config)
{
    //文件格式
    //        pcnt=2  --特效组个数
    //        p1=UiFire_1,UiFire_2,UiFire_3,UiFire_4,300,300   -- 特效名,...,posx,posy
    //        p2=UiFire_1,UiFire_2,UiFire_3,UiFire_4,0,0
    //        bpt=1|Effect_0000.png   -- 1 包内资源， 0 - 下载资源  | 贴图名
    //或者
    //        pcnt=2
    //        p1=JP_face_eff1,JP_face_eff2,JP_face_eff3,JP_face_eff4,300,300
    //        p2=JP_face_eff1,JP_face_eff2,JP_face_eff3,JP_face_eff4,0,0
    //        bpt=0|JP_face_eff1.png
    int count = 0;
    string fileName = "";
    bool picInPackage = false;
    auto pos = config.find("pcnt");
    if(pos != config.end()){
        count = atoi((*pos).second.c_str()) + 1;
    }
    pos = config.find("bpt");
    if (pos!=config.end()) {
        string tmp = (*pos).second;
        auto pos1 = tmp.find("|");
        if(pos1!=string::npos){
            string a = tmp.substr(0,pos1);
            string b = tmp.substr(pos1+1);
            picInPackage = (a=="0")?false:true;
            fileName = b;
        }
    }
    if(count == 0 || fileName.empty()){
        return;
    }
    
    m_customSkinPar = CCParticleBatchNode::createWithTexture(CCLoadSprite::loadResource(fileName.c_str())->getTexture());
    m_signLayer->addChild(m_customSkinPar);
    m_customSkinPar->setVisible(true);
    
    if(m_customSkinPar){
        string skinPath = DynamicResourceController::getInstance()->getDownloadPath() + "skinparticle/";
        for (int i=1; i<count; i++) {
            string key = string("p") + CC_ITOA(i);
            pos = config.find(key);
            if(pos==config.end())
                continue;
            string tmp = (*pos).second;
            vector<string> vTmp;
            CCCommonUtils::splitString(tmp, ",", vTmp);
            if(vTmp.size()<3){
                continue;
            }
            int posY = atoi(vTmp.at(vTmp.size()-1).c_str());
            int posX = atoi(vTmp.at(vTmp.size()-2).c_str());
            int iStart = 0;
            while (iStart<vTmp.size()-2) {
                CCParticleSystemQuad *particle = NULL;
                string pName = vTmp.at(iStart);
                if(picInPackage==true){
                    particle = ParticleController::createParticle(pName.c_str());
                }else{
                    pName = skinPath + pName;
                    particle = ParticleController::createParticleForLua(pName.c_str());
                }
                if(particle){
                    particle->setPosition(m_spr1->getPosition() + ccp(posX, posY));
                    m_customSkinPar->addChild(particle);
                }
                ++iStart;
            }
        }
    }
    
    
    
    int skcnt = 0;
    pos = config.find("skcnt");
    if (pos != config.end()) {
        skcnt = atoi(config["skcnt"].c_str());
    }
    if (skcnt > 0)
    {
        skcnt++;
        for (int i = 1; i < skcnt; ++i)
        {
            string skKey("sk");
            skKey += CC_ITOA(i);
            pos = config.find(skKey);
            if (pos != config.end()) {
                string skValue = pos->second;
                vector<string> skVec;
                CCCommonUtils::splitString(skValue, ",", skVec);
                if (skVec.size() != 4)
                {
                    continue;
                }
                int posX = atoi(skVec.at(2).c_str());
                int posY = atoi(skVec.at(3).c_str());
                string skName = skVec.at(0);
                string atlasName = skVec.at(1);
                string skAtlasFile = atlasName + ".atlas";
                string skAtlasFilePath = DynamicResourceController::getInstance()->getDownloadPath()+skAtlasFile;
                if (!CCFileUtils::sharedFileUtils()->isFileExist(skAtlasFilePath))
                {
                    continue;
                }
                IFNormalSkNode* skNode = IFNormalSkNode::create();
                IFSkeletonAnimation* pAni = skNode->setImperialSpineState(skName, atlasName, 0);
                if (!pAni) {
                    continue;
                }
                if (!m_customSkinSkBatch)
                {
                    m_customSkinSkBatch = IFSkeletonBatchLayer::create();
                    if (m_customSkinSkBatch) {
                        m_signLayer->addChild(m_customSkinSkBatch);
                    }
                }
                if (m_customSkinSkBatch)
                {
                    m_customSkinSkBatch->setVisible(true);
                    m_customSkinSkBatch->addChild(skNode);
                    skNode->setPosition(m_spr1->getPosition() + ccp(posX, posY));
                }
            }
        }
    }
    
    if (m_customSkinSkBatch) {
        bool skeUnderPar = false;
        pos = config.find("skUnderPar");
        if (pos != config.end()) {
            skeUnderPar = atoi(pos->second.c_str()) == 0? false : true;
        }
        if (skeUnderPar) {
            m_customSkinSkBatch->setZOrder(-1);
        } else {
            m_customSkinSkBatch->setZOrder(1);
        }
    }
}

void FunBuild::removeSkinParticle()
{
    if (m_customSkinPar)
    {
        m_customSkinPar->removeFromParentAndCleanup(true);
        m_customSkinPar = nullptr;
    }
    if (m_customSkinSkBatch)
    {
        m_customSkinSkBatch->removeFromParentAndCleanup(true);
        m_customSkinSkBatch = nullptr;
    }
}

bool FunBuild::IsNullBuildState()
{
    if (m_buildState == nullptr) {
        return true;
    }else {
        return false;
    }
}

void FunBuild::setStateBtnsVisible(bool visible)
{
    if (m_buildState) {
        if (visible) {
            m_buildState->setVisible(true);
        }
        else
            m_buildState->setVisible(false);
    }
}

void FunBuild::setSoilderVisible(CCObject *obj)
{
    int type = -1;
    if(obj==NULL || (dynamic_cast<CCInteger*>(obj)==NULL)){
        type = -1;
    }else {
        CCInteger* intObj = dynamic_cast<CCInteger*>(obj);
        type = intObj->getValue();
    }

    if (m_soider1 && m_soider2) {
        if (type == 0) {
            m_soider1->setVisible(false);
            m_soider2->setVisible(false);
        }
        else if (type == 1) {
            m_soider1->setOpacity(0);
            m_soider1->setVisible(true);
            m_soider2->setOpacity(0);
            m_soider2->setVisible(true);
            
            CCActionInterval * fadeTo1 = CCFadeTo::create(1.0, 255);
            CCFiniteTimeAction * spawn1 =CCSpawn::create(CCCallFunc::create(this, callfunc_selector(FunBuild::addSoilderVisibleParticle)),fadeTo1,NULL);
            
            CCSequence *sequene = CCSequence::create(CCDelayTime::create(5.0), spawn1, NULL);
            CCSequence *sequene1 = CCSequence::create(CCDelayTime::create(5.0), CCFadeTo::create(1.0, 255), NULL);
            m_soider1->runAction(sequene);
            m_soider2->runAction(sequene1);
        }
    }
}

void FunBuild::addSoilderVisibleParticle()
{
    Node* parNode1 = Node::create();
    Node* parNode2 = Node::create();
    for (int i = 0 ; i < 7; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("newop_guard_%d",i)->getCString());
        particle->setPosition(CCPointZero);
        if (i == 0) {
            particle->setPosition(ccp(0, 135));
        }
        parNode1->addChild(particle);
    }
    for (int i = 0 ; i < 7; i++) {
        auto particle =  ParticleController::createParticle(CCString::createWithFormat("newop_guard_%d",i)->getCString());
        particle->setPosition(CCPointZero);
        if (i == 0) {
            particle->setPosition(ccp(0, 135));
        }
        parNode2->addChild(particle);
    }
    
    if (m_soider1Par && m_soider2Par) {
        m_soider1Par->addChild(parNode1);
        m_soider2Par->addChild(parNode2);
    }
    scheduleOnce(schedule_selector(FunBuild::removeSoldierParticle), 5.0);
}

void FunBuild::removeSoldierParticle(float t)
{
    if (m_soider2Par && m_soider1Par) {
        m_soider1Par->removeAllChildren();
        m_soider2Par->removeAllChildren();
    }
}
/////////////
FunBuildState* FunBuildState::create(int itemId)
{
    FunBuildState *pRet = new FunBuildState();
    if (pRet && pRet->init(itemId))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool FunBuildState::init(int itemId)
{
    CCLoadSprite::doResourceByCommonIndex(510, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(510, false);
    });
    m_info = &((*FunBuildController::getInstance()->curBuildsInfo)[itemId]);
    m_buildingKey = itemId;
    m_qid = QID_MAX;
    int bType = m_buildingKey/1000;
    isChange = true;
    bool b_isGet = false;
    if (m_info->state == FUN_BUILD_NORMAL) {
        if(bType==FUN_BUILD_WOOD || bType==FUN_BUILD_FOOD || bType==FUN_BUILD_IRON || bType==FUN_BUILD_STONE) {
            b_isGet = true;
        }
        else {
            m_qType = CCCommonUtils::getQueueTypeByBuildType(m_buildingKey/1000);
            if (m_qType != -1) {
                m_qid = QueueController::getInstance()->getMinTimeQidByType(m_qType);
            }
        }
    }
    else {
        m_qType = TYPE_BUILDING;
        m_qid = QueueController::getInstance()->getMinTimeQidByType(m_qType, CC_ITOA(m_buildingKey));
    }
    
    if (!b_isGet) {
        if (m_qid != QID_MAX && m_qType != TYPE_BUILDING) {
            auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
//            m_sumTime = qInfo.finishTime - qInfo.startTime;
            m_sumTime = qInfo.totalTime;
            m_curTime = qInfo.finishTime - GlobalData::shared()->getWorldTime();
        }
        else {
            if(m_qType == TYPE_BUILDING) {
                if (m_info->state != FUN_BUILD_DESTROY) {
                    m_sumTime = m_info->time_need;
                }
                else {
                    m_sumTime = m_info->des_time;
                }
                m_curTime = m_info->updateTime - GlobalData::shared()->getWorldTime();
            }
            else {
                if (m_info->type == FUN_BUILD_STABLE) {
                }
                else {
                    return false;
                }
            }
        }
    }
    
    CCBLoadFile("BuildStateView",this,this);
    m_freeLabel->setString(_lang("103672"));
    m_freeNode->setVisible(false);
    m_recNode->setVisible(false);
    m_helpNode->setVisible(false);
    m_getNode->setVisible(false);
    
    m_freeNode->setScale(1.3);
    m_recNode->setScale(1.3);
    m_helpNode->setScale(1.3);
    m_getNode->setScale(1.4);
    
    if (b_isGet) {
        int resType = Wood;
        if(bType==FUN_BUILD_WOOD){
            resType = Wood;
        }
        else if(bType==FUN_BUILD_FOOD){
            resType = Food;
        }
        else if(bType==FUN_BUILD_IRON){
            resType = Iron;
        }
        else if(bType==FUN_BUILD_STONE){
            resType = Stone;
        }
        string picStr = CCCommonUtils::getResourceIconByType(resType);
        auto pic = CCLoadSprite::createSprite(picStr.c_str());
//        CCCommonUtils::setSpriteMaxSize(pic, 80, true);
        m_getIconNode->addChild(pic);
        m_getNode->setVisible(true);
    }
    else if (m_qType == TYPE_BUILDING) {
        m_msgNode->setVisible(false);
        string iconStr = "icon_shengji1.png";
        if (m_info->level >= 30) {
            iconStr = "rongyuxunzhang.png";
        }
        if(m_info->state != FUN_BUILD_UPING) {
            iconStr = "icon_jianzao.png";
        }
        auto tIcon = CCLoadSprite::createSprite(iconStr.c_str());
        m_iconNode->addChild(tIcon);
    }
    else {
        if (m_qid!=QID_MAX) {
            int buildType = m_buildingKey/1000;
            auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
            int sid = QueueController::getInstance()->getItemId(qInfo.key);
            string name = CCCommonUtils::getNameById(CC_ITOA(sid));
            int num = 0;
            if (buildType == FUN_BUILD_SCIENE || buildType == FUN_BUILD_FORGE || buildType == FUN_BUILD_WORKSHOP){
                m_msgLabel->setString(name);
            }
            else if (buildType == FUN_BUILD_HOSPITAL){
                long treatNum = ArmyController::getInstance()->getTotalTreat();
                string strtreatNum = "";
                if(treatNum <= 0){
                    strtreatNum = "";
                }else{
                    strtreatNum = CC_ITOA_K(treatNum);
                }
                string strNum = _lang("102114") + " " + strtreatNum;
                m_msgLabel->setString(strNum);
            }
            else {
                if(buildType == FUN_BUILD_FORT){
                    map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->fortList.find(CC_ITOA(sid));
                    if (it != GlobalData::shared()->fortList.end()) {
                        num = (it->second).train;
                    }
                }else{
                    map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->armyList.find(CC_ITOA(sid));
                    if (it != GlobalData::shared()->armyList.end()) {
                        num = (it->second).train;
                    }
                }
                FunBuildController::getInstance()->colTroopsNum[buildType] = num;
                if (num > 0) {
                    name = name + "   " +CC_ITOA(num);
                }
                m_msgLabel->setString(name);
            }
            
            string pic = "";
            if (buildType == FUN_BUILD_BARRACK1) {
                pic = "icon_bubing.png";
            }
            else if (buildType == FUN_BUILD_BARRACK2) {
                pic = "icon_qibing.png";
            }
            else if (buildType == FUN_BUILD_BARRACK3) {
                pic = "icon_gongbing.png";
            }
            else if (buildType == FUN_BUILD_BARRACK4) {
                pic = "icon_chebing.png";
            }
            else if (buildType == FUN_BUILD_FORT) {
                pic = "icon_xianjing.png";
            }
            else if (buildType == FUN_BUILD_SCIENE) {
                pic = "icon_keji.png";
            }
            else if (buildType == FUN_BUILD_FORGE) {
                pic = "icon_forging02.png";
            }
            else if (buildType == FUN_BUILD_HOSPITAL) {
                pic = "icon_hospital.png";
            }
            else if (buildType == FUN_BUILD_WORKSHOP) {
                pic = "icon_chilun.png";
            }
            auto tIcon = CCLoadSprite::createSprite(pic.c_str());
            m_iconNode->addChild(tIcon);
            
            string recPic = "shangBing.png";
            if (buildType == FUN_BUILD_FORT) {
                recPic = "icon_trap.png";
            }else if (buildType == FUN_BUILD_FORGE) {
                recPic = "icon_equip.png";
            }else if (buildType == FUN_BUILD_WORKSHOP) {
                vector<string> vec;
                string s = qInfo.allId;
                CCCommonUtils::splitString(s, ";", vec);
                if (vec.size()>0) {
                    recPic = CCCommonUtils::getIcon(vec[0]);
                }
            }
            auto utIcon = CCLoadSprite::createSprite(recPic.c_str());
            m_recIcon->addChild(utIcon);
        }
    }
    
    if(bType==FUN_BUILD_WOOD || bType==FUN_BUILD_FOOD || bType==FUN_BUILD_IRON || bType==FUN_BUILD_STONE || bType==FUN_BUILD_HOSPITAL || bType==FUN_BUILD_BARRACK) {
        m_freeNode->setPositionY(115);
        m_helpNode->setPositionY(115);
        m_getNode->setPositionY(100);
    } else {
        m_freeNode->setPositionY(195);
        m_helpNode->setPositionY(195);
        m_getNode->setPositionY(160);
    }
    
    if (!b_isGet) {
        m_bar->setScaleX(0);
        m_bar2->setScaleX(0);
        onEnterFrame(0);
        onCheckAllianceHelp();
    }
    else {
        m_curTime = 0;
        m_upNode->setVisible(false);
        onPlayCanRec(4);
    }
    if(bType==FUN_BUILD_WORKSHOP) {
        m_upNode->setPositionY(m_upNode->getPositionY()-20);
    }
    
    m_freeBtn->setTouchPriority(Touch_Imperial_City);
    m_helpBtn->setTouchPriority(Touch_Imperial_City);
    m_recBtn->setTouchPriority(Touch_Imperial_City);
    //m_getBtn->setTouchPriority(Touch_Imperial_City);
    m_getBtn->setEnabled(false);
    return true;
}


void FunBuildState::onCheckAllianceHelp()
{
    if(m_freeNode->isVisible() || m_recNode->isVisible()) {
        return;
    }
    
    if (m_qid!=QID_MAX && m_info->state != FUN_BUILD_DESTROY) {
        auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
        if (qInfo.type == TYPE_BUILDING || qInfo.type == TYPE_HOSPITAL || qInfo.type == TYPE_SCIENCE || qInfo.type == TYPE_FORGE) {
            if (GlobalData::shared()->playerInfo.isInAlliance() && qInfo.canHelp){
                int buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_STABLE);
                if (buildId>0) {
                    m_helpIconNode->removeAllChildren();
                    auto icon = CCLoadSprite::createSprite("alliances_help.png");
                    m_helpIconNode->addChild(icon);
                    if (qInfo.type == TYPE_HOSPITAL && CCCommonUtils::isKuaFu()) {
                        m_helpNode->setVisible(false);
                    }else{
                        m_helpNode->setVisible(true);
                    }
                    onPlayCanRec(3);
                }
            }
        }
    }
    else if (m_info->type == FUN_BUILD_STABLE && GlobalData::shared()->playerInfo.allianceInfo.helpcount>0) {
        m_helpIconNode->removeAllChildren();
        auto icon = CCLoadSprite::createSprite("allianceHelpAll.png");
        m_helpIconNode->addChild(icon);
        m_helpNode->setVisible(true);
        onPlayCanRec(3);
        if(m_info->state == FUN_BUILD_NORMAL) {
            m_upNode->setVisible(false);
        }
        else {
            m_upNode->setVisible(true);
        }
    }
}

void FunBuildState::onEnterFrame(float dt)
{
    if (m_qid == QID_MAX) {
        if (m_info->type == FUN_BUILD_STABLE) {//大使馆
            if (m_recNode->isVisible()||true) {//(!AllianceDailyController::getInstance()->isSwitchOpen())
                return ;
            }else{
                m_upNode->setVisible(false);
                m_recNode->setVisible(true);
                onPlayCanRec(2);
            }
            return ;
        }
        if(isChange && FunBuildController::getInstance()->isFullOutPut(m_info->itemId)) {
            isChange = false;
            m_getIconNode->removeAllChildren();
            int resType = Wood;
            if(m_info->type==FUN_BUILD_WOOD){
                resType = Wood;
            }
            else if(m_info->type==FUN_BUILD_FOOD){
                resType = Food;
            }
            else if(m_info->type==FUN_BUILD_IRON){
                resType = Iron;
            }
            else if(m_info->type==FUN_BUILD_STONE){
                resType = Stone;
            }
            string picStr = CCCommonUtils::getResourceIconByType(resType);
            auto pic = CCLoadSprite::createSprite(picStr.c_str());
//            CCCommonUtils::setSpriteMaxSize(pic, 80, true);
            m_getIconNode->addChild(pic);
            m_getNode->setVisible(true);
        }
        return;
    }
    auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
    if (m_curTime>=0) {
//        auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
        int tmpCurTime = GlobalData::shared()->getWorldTime();
        if(qInfo.startTime==0){
            qInfo.startTime = tmpCurTime;
        }
//        m_sumTime = qInfo.finishTime - qInfo.startTime;
        m_sumTime = qInfo.totalTime;
        m_curTime = qInfo.finishTime - tmpCurTime;
        m_curTime = MAX(0, m_curTime);
        m_sumTime = MAX(1, m_sumTime);
        if (qInfo.finishTime<=0) {
            m_curTime = 0;
            m_sumTime = 1;
        }
        
        m_timeLabel->setString(CC_SECTOA(m_curTime));
        float pro = m_curTime*1.0/m_sumTime;
        pro = pro>1?1:pro;
        m_bar->setScaleX(1-pro);
        m_bar2->setScaleX(1-pro);
        if (m_qType == TYPE_BUILDING && m_curTime<=GlobalData::shared()->GlobalData::shared()->freeSpdT)
        {
            string guideId = GuideController::share()->getCurrentId();
            if (guideId=="3071400" || guideId=="3072300" || guideId=="3072800" || guideId=="3074800"
                || guideId=="3071300" || guideId=="3072200" || guideId=="3072700" || guideId=="3074700")
            {
                m_freeNode->setVisible(false);
            }
            else {
                if (!m_freeNode->isVisible()) {
                    m_bar->setVisible(false);
                    
                    for (int i=1; i<=2; i++) {
                        auto particle = ParticleController::createParticle(CCString::createWithFormat("QueueGlow_%d",i)->getCString());
                        particle->setPosition(ccp(66, 66));
                        m_freeTouchNode->addChild(particle);
                    }
                    m_freeNode->setVisible(true);
                    m_helpNode->setVisible(false);
                    onPlayCanRec(1);
                }
            }
        }
        
        if (!qInfo.canHelp && m_helpNode->isVisible()) {
            m_helpNode->setVisible(false);
        }
        
        if(qInfo.type == TYPE_MATE) {
            if (qInfo.para>0) {
                m_recNode->setVisible(true);
                onPlayCanRec(2);
            }
        }
        
        if(m_curTime<=0) {
            m_upNode->setVisible(false);
            m_freeTouchNode->removeAllChildren();
            m_freeNode->setVisible(false);
            m_helpNode->setVisible(false);
            
            if (m_qType != TYPE_BUILDING) {
                m_recNode->setVisible(true);
                onPlayCanRec(2);
                if (m_buildingKey/1000 == FUN_BUILD_BARRACK2) {
                    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
                    if(layer) {
                        layer->setUnMoveScence(false);
                    }
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                                           , CCString::createWithFormat("BU_%d_cd", m_buildingKey/1000));
                }
            }
            else {
                if (m_info->type == FUN_BUILD_STABLE && GlobalData::shared()->playerInfo.allianceInfo.helpcount>0) {//如果大使馆升级完毕后，判断。
                    m_helpIconNode->removeAllChildren();
                    auto icon = CCLoadSprite::createSprite("allianceHelpAll.png");
                    m_helpIconNode->addChild(icon);
                    m_helpNode->setVisible(true);
                    onPlayCanRec(3);
                }
            }
        }
//        m_curTime--;
    }
    if(m_curTime<0 && qInfo.type == TYPE_MATE) {
        m_curTime = 0;
        m_upNode->setVisible(false);
    }
}

void FunBuildState::onPlayCanRec(int type)
{
    m_freeNode->stopAllActions();
//    m_recNode->stopAllActions();
//    m_helpNode->stopAllActions();
//    m_getNode->stopAllActions();
    if (m_recNode->getActionByTag(2)) {
        return;
    }
    if (m_helpNode->getActionByTag(3)) {
        return;
    }
    if (m_getNode->getActionByTag(4)) {
        return;
    }
    CCRotateBy* rt1 = CCRotateBy::create(0.1, 15);
    CCRotateBy* rt2 = CCRotateBy::create(0.1, -30);
    CCRotateBy* rt3 = CCRotateBy::create(0.1, 22);
    CCRotateBy* rt4 = CCRotateBy::create(0.1, -14);
    CCRotateBy* rt5 = CCRotateBy::create(0.1, 7);
    CCDelayTime* dt = CCDelayTime::create(5);
    CCSequence* seq = CCSequence::create(rt1, rt2, rt3, rt4, rt5, dt, NULL);
    if (type==1) {
//        m_freeNode->runAction(CCRepeatForever::create(seq));
    }
    else if (type == 2) {
        CCAction* action = CCRepeatForever::create(seq);
        action->setTag(2);
        m_recNode->runAction(action);
    }
    else if (type == 3) {
        CCAction* action = CCRepeatForever::create(seq);
        action->setTag(3);
        m_helpNode->runAction(action);
    }
    else if (type == 4) {
        CCAction* action = CCRepeatForever::create(seq);
        action->setTag(4);
        m_getNode->runAction(action);
    }
}

void FunBuildState::onShowRecState()
{
    m_curTime=0;
    if (m_qType != TYPE_BUILDING) {
        m_upNode->setVisible(false);
        m_recNode->setVisible(true);
        onPlayCanRec(2);
    }
}

void FunBuildState::showUpNode(bool isShow){
    m_upNode->setVisible(isShow);
}

void FunBuildState::onCheckTime(CCObject* params)
{
    int itemId = dynamic_cast<CCInteger*>(params)->getValue();
    if (m_buildingKey!=itemId) {
        return;
    }
    
    int key = 0;
    if (m_qType == TYPE_BUILDING) {
        m_qid = QueueController::getInstance()->getMinTimeQidByType(m_qType, CC_ITOA(m_buildingKey));
    }
    else {
        m_qid = QueueController::getInstance()->getMinTimeQidByType(m_qType);
    }
    
    if (m_qid != QID_MAX) {
        auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
        key = atoi(qInfo.key.c_str());
    }
    
    if (itemId == key) {
        auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
//        m_sumTime = qInfo.finishTime - qInfo.startTime;
        m_sumTime = qInfo.totalTime;
        m_curTime = qInfo.finishTime - GlobalData::shared()->getWorldTime();
    }

}

void FunBuildState::onEnter() {
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FunBuildState::onCheckTime), MSG_CHECK_TIME, NULL);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(FunBuildState::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    
    if(m_info->type==FUN_BUILD_WOOD || m_info->type==FUN_BUILD_FOOD || m_info->type==FUN_BUILD_IRON || m_info->type==FUN_BUILD_STONE){
        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(FunBuildState::onGetResAction), MSG_RES_GET, NULL);
        setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
        setTouchEnabled(true);
        //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
    }
    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    if(layer) {
        layer->hidePrincess(m_buildingKey);
    }
}

void FunBuildState::onExit() {
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHECK_TIME);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(FunBuildState::onEnterFrame), this);
    
    if(m_info->type==FUN_BUILD_WOOD || m_info->type==FUN_BUILD_FOOD || m_info->type==FUN_BUILD_IRON || m_info->type==FUN_BUILD_STONE){
        setTouchEnabled(false);
        CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_RES_GET);
    }
    CCNode::onExit();
}

SEL_MenuHandler FunBuildState::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

SEL_CCControlHandler FunBuildState::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFreeBtnClick", FunBuildState::onFreeBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRecBtnClick", FunBuildState::onRecBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHelpBtnClick", FunBuildState::onHelpBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGetBtnClick", FunBuildState::onGetBtnClick);
    return NULL;
}

bool FunBuildState::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upNode", CCNode*, this->m_upNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bar", CCScale9Sprite*, this->m_bar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgNode", CCNode*, this->m_msgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIF*, this->m_msgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bar2", CCScale9Sprite*, this->m_bar2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_freeNode", CCNode*, this->m_freeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_freeBtn", CCControlButton*, this->m_freeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_freeLabel", CCLabelIF*, this->m_freeLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpNode", CCNode*, this->m_helpNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpBtn", CCControlButton*, this->m_helpBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpIconNode", CCNode*, this->m_helpIconNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_recNode", CCNode*, this->m_recNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_recBtn", CCControlButton*, this->m_recBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_recIcon", CCNode*, this->m_recIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_freeTouchNode", CCNode*, this->m_freeTouchNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getNode", CCNode*, this->m_getNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getBtn", CCControlButton*, this->m_getBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getIconNode", CCNode*, this->m_getIconNode);
    return false;
}

bool FunBuildState::isCanClick()
{
    if (m_freeNode->isVisible() || m_recNode->isVisible() || m_helpNode->isVisible() || m_getNode->isVisible()) {
        return true;
    }
    return false;
}

bool FunBuildState::isCanGather()
{
    if (m_getNode->isVisible()) {
        return true;
    }
    return false;
}

void FunBuildState::ClickState()
{
    if (m_freeNode->isVisible() )
    {
        onFreeBtnClick(NULL,Control::EventType::TOUCH_DOWN);
    }
    else if (m_recNode->isVisible()) {
        onRecBtnClick(NULL,Control::EventType::TOUCH_DOWN);
    }
    else if (m_helpNode->isVisible()) {
        onHelpBtnClick(NULL,Control::EventType::TOUCH_DOWN);
    }
    else if (m_getNode->isVisible()) {
        onGetBtnClick(NULL,Control::EventType::TOUCH_DOWN);
    }
}


void FunBuildState::onFreeBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if ( FunBuildController::getInstance()->costCD(m_buildingKey, "", 0) )
    {
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
//                                                                               , CCString::createWithFormat("BU_%d_cd", m_buildingKey/1000));
    }
    else {
        CCLOG("dddd");
    }
}

void FunBuildState::onHelpBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    if (m_info->type == FUN_BUILD_STABLE && GlobalData::shared()->playerInfo.allianceInfo.helpcount>0) {
        if (m_qid!=QID_MAX) {
            auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
            if (GlobalData::shared()->playerInfo.isInAlliance() && qInfo.canHelp){//帮助请求，和处理别人帮助同时发送
                m_helpNode->stopAllActions();
                m_helpNode->setVisible(false);
                AllianceManager::getInstance()->callHelp(m_qid);
                AllianceManager::getInstance()->sendHelpAllToServer();
            }
        }
        else {
            m_helpNode->stopAllActions();
            m_helpNode->setVisible(false);
            AllianceManager::getInstance()->sendHelpAllToServer();
        }
    }
    else {
        m_helpNode->stopAllActions();
        m_helpNode->setVisible(false);
        AllianceManager::getInstance()->callHelp(m_qid);
    }
}
void FunBuildState::autoClickRecBtn(){
    this->onRecBtnClick(NULL,Control::EventType::TOUCH_DOWN);
}

void FunBuildState::onRecBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    int btype = m_buildingKey/1000;
    int qtype = CCCommonUtils::getQueueTypeByBuildType(btype);
    if (qtype>0) {
        int qid = QueueController::getInstance()->getCanRecQidByType(qtype);
        if (qid != QID_MAX) {
            unsigned long addPower = 0;
            map<int, QueueInfo>::iterator it =  GlobalData::shared()->allQueuesInfo.find(qid);
            if (it->second.type == TYPE_FORGE)
            {
                //收装备
                SoundController::sharedSound()->playEffects(Music_Sfx_city_harvest_equipment);
                QueueController::getInstance()->autoHavestEquip(qid, m_buildingKey);
            }
            else if (it->second.type == TYPE_MATE)
            {
                QueueController::getInstance()->autoHavestMateTool(qid, m_buildingKey);
            }
            else
            {
                //收兵
                GlobalData::shared()->playerInfo.lastBattlePower =  GlobalData::shared()->playerInfo.battlePower;
                if(it!=GlobalData::shared()->allQueuesInfo.end()){
                    std::string armyId = CC_ITOA(atoi((it->second).key.c_str()) / 100);
                    if(m_buildingKey/1000 == FUN_BUILD_FORT){
                        map<std::string, ArmyInfo>::iterator armyIt = GlobalData::shared()->fortList.find(armyId);
                        if(armyIt!=GlobalData::shared()->fortList.end()){
                            addPower = (unsigned long)( ((float)armyIt->second.train) * armyIt->second.power);
                            addPower = MAX(addPower, 0);
                            GlobalData::shared()->playerInfo.fortPower = GlobalData::shared()->playerInfo.fortPower + addPower;
                        }
                    }else{
                        map<std::string, ArmyInfo>::iterator armyIt = GlobalData::shared()->armyList.find(armyId);
                        if(armyIt!=GlobalData::shared()->armyList.end()){
                            addPower = (unsigned long)( ((float)armyIt->second.train) * armyIt->second.power);
                            addPower = MAX(addPower, 0);
                            GlobalData::shared()->playerInfo.armyPower = GlobalData::shared()->playerInfo.armyPower + addPower;
                        }
                    }
                }
                GlobalData::shared()->playerInfo.addPower = addPower;
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_COLLECT_SOLDIER_ADD_POWER);
                if (addPower>0) {
                    QueueController::getInstance()->collectSolider(qid,m_buildingKey);
                }else {
                    QueueController::getInstance()->startFinishQueue(qid, false);
                }
                //ArmyController::getInstance()->startFinishForce(itemId, (it->second).uuid,buildingKey);
                
                if (GuideController::share()->getCurrentId() == "3074000") {
                    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
                    if(layer) {
                        layer->setUnMoveScence(false);
                        layer->onMoveToPos(390, 600, TYPE_POS_MID, 4.0, 0.8, true);
                    }
                }
                
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                                       , CCString::createWithFormat("BU_%d_troop", m_buildingKey/1000));
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REMOVE_STATE, CCInteger::create(m_buildingKey));
            }
        }
        else {
            if (btype == FUN_BUILD_WORKSHOP) {
                map<int, QueueInfo>::iterator it;
                for (it = GlobalData::shared()->allQueuesInfo.begin(); it != GlobalData::shared()->allQueuesInfo.end(); it++) {
                    if((it->second).para>0 && (it->second).type==TYPE_MATE)
                    {
                        qid = it->first;
                        break;
                    }
                }
                if (qid != QID_MAX) {
                    QueueController::getInstance()->autoHavestMateTool(qid, m_buildingKey);
                }else{
                    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
                    if (layer) {
                        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_SAVEPOS);
                        layer->setUnMoveScence(false);
                        layer->onMoveToBuildAndOpen(m_buildingKey, TYPE_POS_MID);
                        PopupViewController::getInstance()->addPopupView(ToolCreateView::create());
                    }
                }
            }
        }
    }
}

void FunBuildState::onGetBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_getNode->isVisible() && m_parFinish) {
        m_parFinish = false;
        auto spr1per = Sprite::create();
        CCCommonUtils::makeOnceEffectSpr(spr1per, "shouji_1_%d.png", 6, 0.07);
        spr1per->setBlendFunc({GL_ONE,GL_ONE});
        spr1per->setTag(9999);
        spr1per->setScale(1.95);
        spr1per->setPosition(ccp(0,40));
        m_getNode->addChild(spr1per);
        FunBuildController::getInstance()->startGetFunBuildProduct(m_info->itemId);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                               , CCString::createWithFormat("BU_%d_collect", m_buildingKey/1000));
        this->scheduleOnce(schedule_selector(FunBuildState::removeGetParNode), 0.6);
    }
}

void FunBuildState::onHideBtn(int type)
{
    if(type == 4) {
        m_helpNode->stopAllActions();
        m_helpNode->setVisible(false);
    }
}

CCNode* FunBuildState::getGuideNode(string _key)
{
    if(_key=="cd") {
        if (m_freeNode->isVisible()) {
            return m_freeBtn;
        }
        else {
            return NULL;
        }
    }
    else if (_key=="troop" && m_recNode->isVisible()) {
        return m_recBtn;
    }
    else if (_key=="collect" && m_getNode->isVisible()) {
        return m_getBtn;
    }
    return NULL;
}

bool FunBuildState::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    int pop = PopupViewController::getInstance()->getCurrViewCount();
    if (pop <=0 && m_getBtn->isTouchInside(pTouch) && m_getNode->isVisible()) {
        CanDel = false;
        onGetBtnClick(NULL,Control::EventType::TOUCH_DOWN);
        return true;
    }
    return false;
}

void FunBuildState::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CanDel = true;
}
void FunBuildState::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_RES_GET, pTouch);
}
void FunBuildState::onTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CanDel = true;
}
void FunBuildState::onGetResAction(CCObject* params)
{
    CCTouch *pTouch = dynamic_cast<CCTouch*>(params);
    if (pTouch && m_getNode->isVisible() && m_getBtn->isTouchInside(pTouch)) {
        onGetBtnClick(NULL,Control::EventType::TOUCH_DOWN);
    }
}

void FunBuildState::removeGetParNode(float t)
{
    m_getNode->stopAllActions();
    m_getNode->setVisible(false);
    m_parFinish = true;
}