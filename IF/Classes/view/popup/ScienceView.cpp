//
//  ScienceView.cpp
//  IF
//
//  Created by 邹 程 on 13-11-1.
//
//

#include "ScienceView.h"
#include "ScienceCommand.h"
#include "YesNoDialog.h"
#include "PopupViewController.h"
#include "ScienceListView.h"
#include "UseCDToolView.h"
#include "QueueController.h"
#include "ScienceController.h"

#include "GeneralSkillListPopUpView.h"
#include "FunBuildController.h"
#include "ParticleController.h"
#include "ConfirmDialogView.h"
#include "ScienceCommand.h"
#include "CCMathUtils.h"
#include "SoundController.h"
#include "GuideController.h"
#include "IFNormalSkNode.h"

#define SCIENCE_TYPE_DOMESTIC 1
#define SCIENCE_TYPE_WARFARE  2
#define BATCH_TAG 457
#define MSG_BUILD_CELL "msg_build_cell"

#define MSG_ADD_SCIENCE_POPUP "_science_add"
#define MSG_REMOVE_SCIENCE_POPUP "_science_remove"

bool isReturn = true;
static int  tempY = 0;
static int m_openNum1 = 0;
static int m_scienceIdstatic = 0;
ScienceView* ScienceView::create(int buildId, int scienceType){
    ScienceView* ret = new ScienceView();
    if(ret && ret->init(buildId, scienceType)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

ScienceView::~ScienceView()
{
//    m_scrollView->removeAllChildren();
//    m_lineBatch = NULL;
//    m_commonBatch = NULL;
//    m_itemBatch = NULL;
//    m_iconBatch = NULL;
//    CCLoadSprite::doResourceByCommonIndex(4, false);
//    CCLoadSprite::doResourceByCommonIndex(5, false);
}

bool ScienceView::init(int buildId, int scienceType)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    setIsHDPanel(true);
    isReturn = true;
    m_buildId = buildId;
    m_scienceType = scienceType;
    CCLoadSprite::doResourceByCommonIndex(4, true);
    CCLoadSprite::doResourceByCommonIndex(5, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(4, false);
        CCLoadSprite::doResourceByCommonIndex(5, false);
        CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_SCIENCE);
    });
    
    auto tbg = CCLoadSprite::loadResource("technology_09.png");
    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
    int maxHight = CCDirector::sharedDirector()->getWinSize().height;
    int curHight = -500;
    while (curHight<maxHight) {
        auto bg = CCLoadSprite::createSprite("technology_09.png");
        if (CCCommonUtils::isIosAndroidPad()) {
            bg->setScale(2.4);
        }
        bg->setAnchorPoint(ccp(0, 1));
        bg->setPosition(ccp(0, curHight));
        curHight += bg->getContentSize().height;
        tBatchNode->addChild(bg);
    }
    this->addChild(tBatchNode);
    
    auto tmpCCB = CCBLoadFile("ScienceTreeView",this,this);//ScienceView
    if (CCCommonUtils::isIosAndroidPad()) {
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
    }
    else
        this->setContentSize(tmpCCB->getContentSize());
    
    setTitleName(CCCommonUtils::getNameById(CC_ITOA(m_scienceType)));
    
    if (!CCCommonUtils::isIosAndroidPad()) {
        int addHeight = getExtendHeight();
        int oldWidth = m_infoList->getContentSize().width;
        int oldHeight = m_infoList->getContentSize().height;
        m_infoList->setContentSize(CCSizeMake(oldWidth, oldHeight+addHeight));
        m_infoList->setPositionY(m_infoList->getPositionY()-addHeight);
        m_bottomNode->setPositionY(m_bottomNode->getPositionY() - addHeight);
        this->m_bgContainer->setPositionY(this->m_bgContainer->getPositionY() - addHeight);
    }
    m_scrollView = CCScrollView::create(m_infoList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_scrollView->setTouchPriority(Touch_Default);
    m_infoList->addChild(m_scrollView);
    m_desLabel->setString(_lang("121990"));
    reInitView();
    addBGPic();
    
    //effect
    if (CCFileUtils::sharedFileUtils()->isFileExist("Imperial/Imperial_30.atlas") &&
        CCFileUtils::sharedFileUtils()->isFileExist("Spine/UIComponent/chengfangchilun.json"))
    {
        auto chilun = new IFSkeletonAnimation("Spine/UIComponent/chengfangchilun.json", "Imperial/Imperial_30.atlas");
        if (chilun) {
            {
                chilun->setVisibleStop(false);
                m_effectNode->addChild(chilun,3);
                chilun->setPositionY(-120);
                spTrackEntry* entry = chilun->setAnimation(0.01, "animation", true);
                if(entry){
                    chilun->setTimeScale(0.5f);
                }
            }
        }
    }
    // for (int i=1; i<=4; i++) {
    //     auto particle = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
    //     m_fireNode1->addChild(particle);
        
    //     auto particle1 = ParticleController::createParticle(CCString::createWithFormat("UiFire_%d",i)->getCString());
    //     m_fireNode2->addChild(particle1);
    // }

    return true;
}

void ScienceView::reInitView()
{
    CCLoadSprite::doResourceByCommonIndex(4, true);
    CCLoadSprite::doResourceByCommonIndex(5, true);
    
    auto spLine = CCLoadSprite::loadResource("kji_line.png");//science_line.png
    m_lineBatch = CCSpriteBatchNode::createWithTexture(spLine->getTexture());
    m_scrollView->addChild(m_lineBatch);
    
    auto sp = CCLoadSprite::loadResource("technology_08.png");//science_line.png
    m_itemBatch = CCSpriteBatchNode::createWithTexture(sp->getTexture());
    m_scrollView->addChild(m_itemBatch);
    
    auto cellBG = CCLoadSprite::loadResource("technology_08.png");
    m_commonBatch = CCSpriteBatchNode::createWithTexture(cellBG->getTexture());
    m_scrollView->addChild(m_commonBatch);
    
    auto iconBC = CCLoadSprite::loadResource("science_def_a_1.png");
    m_iconBatch = CCSpriteBatchNode::createWithTexture(iconBC->getTexture());
    m_scrollView->addChild(m_iconBatch);
    
    if (GlobalData::shared()->scienceInfo.IsHasData) {
        updateInfo(NULL);
    }
    else {
        ScienceDataInitCommand * cmd = new ScienceDataInitCommand();
        cmd->sendAndRelease();
    }
}

void ScienceView::updateInfo(CCObject* obj)
{
    //<ItemSpec id="229990" name="107100" icon="ico229990" science="220000;220100;220200;220300;220400;220500;220600;220700;220800;220900;221000;221100"/>
    auto dict = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(m_scienceType));
    string scienceIds = dict->valueForKey("science")->getCString();
    std::vector<string> sciList;
    CCCommonUtils::splitString(scienceIds, ";", sciList);
    if (sciList.empty()) {
        CCLOG("ScienceView::updateInfo - sciList empty, scienceType(%d).", m_scienceType);
        return;
    }
    
    std::vector<int> idList;
    int maxPosY = 0;
    for (int i=0; i<sciList.size(); i++) {
        int scienceId = atoi(sciList[i].c_str());
        idList.push_back(scienceId);
        int curY = GlobalData::shared()->scienceInfo.infoMap[scienceId].posY;
        if (curY > maxPosY) {
            maxPosY = curY;
        }
    }
    
    int _itemH = 160+20;
    int _itemW = 80;
    int curY = 10;
    int curX = 0;
    if (CCCommonUtils::isIosAndroidPad()) {
        _itemH = 340+50;;
        _itemW = 160;
        curY = 0;
        curX = 125;
    }
    
    int maxHeight = _itemH * maxPosY;
    if (CCCommonUtils::isIosAndroidPad()) {
        maxHeight += 0;
    }
    else
        maxHeight += 50;
    m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,maxHeight));
   
    bool isset = false;
    for (int i=0; i<idList.size(); i++)
    {
        if (GlobalData::shared()->isScienceValid(idList[i]) == false) {
            CCLOG("ScienceView::updateInfo - invalid science id(%d).", idList[i]);
            continue;
        }
        
        auto& info = GlobalData::shared()->scienceInfo.infoMap[idList[i]];
        string relation = info.relation;
        int posx = curX + (info.posX-1)*_itemW;
        int posy = curY + (maxPosY - info.posY)*_itemH;
        
        auto cell = ScienceCell::create(idList[i], m_commonBatch, m_iconBatch, posx, posy);
        cell->setPosition(ccp(posx,posy));
        cell->setTouchNode(m_infoList);
        
        m_scrollView->addChild(cell);
        if(cell->m_isOpen && info.finishTime - GlobalData::shared()->getWorldTime() > 0 && m_infoList->getContentSize().height - cell->getPositionY() - posy > CCDirector::sharedDirector()->getWinSize().height/2 ){
            if(CCCommonUtils::isIosAndroidPad()){
                m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - cell->getPositionY()-1000));
            }
            else{
                m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - cell->getPositionY()-500));
            }
            isset = true;
        }
        if(!isset){
            m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - maxHeight));
        }
        if(m_openNum1){
            m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - tempY));
        }
    }
     m_openNum1 = 0;

    newAddLine(&idList, maxPosY);
}

void ScienceView::UpdateCellState(CCObject* obj)
{
    auto dict = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(m_scienceType));
    string scienceIds = dict->valueForKey("science")->getCString();
    std::vector<string> sciList;
    CCCommonUtils::splitString(scienceIds, ";", sciList);
    if (sciList.empty()) {
        CCLOG("ScienceView::UpdateCellState - sciList empty, scienceType(%d).", m_scienceType);
        return;
    }
    
    std::vector<int> idList;
    int maxPosY = 0;
    for (int i=0; i<sciList.size(); i++) {
        int scienceId = atoi(sciList[i].c_str());
        idList.push_back(scienceId);
        int curY = GlobalData::shared()->scienceInfo.infoMap[scienceId].posY;
        if (curY > maxPosY) {
            maxPosY = curY;
        }
    }
    newAddLine(&idList, maxPosY);
}

const int st_w = 80;
const int st_h = 160+20;
const int st_H = 130;
const int st_w_HD = 160;
const int st_h_HD = 340+50;
const int st_H_HD = 280;

void ScienceView::newAddLine(vector<int>* idList, int maxHeight)
{
    auto addLine = [](const CCPoint &startPt, std::vector<CCPoint> &endPt, std::vector<bool> openFlag, CCNode *parent, int maxH){
        if (CCCommonUtils::isIosAndroidPad()) {
            int curX = 140;
            int curY = -120;
            auto startRealPt = ccp((startPt.x) * st_w_HD + curX, (maxH-startPt.y) * st_h_HD+st_H_HD + curY);
            int index = 0;
            while (index < endPt.size()) {
                auto &pt = endPt[index];
                auto endRealPt = ccp((pt.x) * st_w_HD + curX, (maxH - pt.y) * st_h_HD + st_H_HD + curY);
                auto len = ccpDistance(endRealPt, startRealPt);
                CCSprite* lineBG = NULL;
                lineBG = CCLoadSprite::createSprite("kji_line.png");

                parent->addChild(lineBG);
                lineBG->setAnchorPoint(ccp(0.5, 0));
                lineBG->setPosition(startRealPt);
                float angle = -CCMathUtils::getAngle(startRealPt, endRealPt) + 90;
                lineBG->setRotation(angle);
                float s = len / lineBG->getContentSize().height;
                lineBG->setScaleY(s);
                
                index++;
            }
        }
        else {
            auto startRealPt = ccp((startPt.x) * st_w, (maxH-startPt.y) * st_h+st_H);
            int index = 0;
            while (index < endPt.size()) {
                auto &pt = endPt[index];
                auto endRealPt = ccp((pt.x) * st_w, (maxH - pt.y) * st_h + st_H);
                auto len = ccpDistance(endRealPt, startRealPt);
                auto lineBG = CCLoadSprite::createSprite("kji_line.png");

                parent->addChild(lineBG);
                lineBG->setAnchorPoint(ccp(0.5, 0));
                lineBG->setPosition(startRealPt);
                float angle = -CCMathUtils::getAngle(startRealPt, endRealPt) + 90;
                lineBG->setRotation(angle);
                float s = len / lineBG->getContentSize().height;
                lineBG->setScaleY(s);
                index++;
            }
        }
    };
    
    for (int i=0; i<idList->size(); i++) {
        auto& info = GlobalData::shared()->scienceInfo.infoMap[(*idList)[i]];
        
        bool isOpen = false;
        if(info.scienceLv>=1){
            isOpen=true;
        }
        
        string relation = info.relation;
        map<int, vector<int>> relationMap;
        vector<int> relationVecY;
        vector<string> relationPts;
        vector<CCPoint> endVec;
        vector<bool> relVec;
        CCCommonUtils::splitString(relation, "|", relationPts);
        for (int j=0; j<relationPts.size(); j++) {
            vector<string> tmpPt;
            CCCommonUtils::splitString(relationPts[j], ";", tmpPt);
            
            int ptX = atoi(tmpPt[1].c_str());
            int ptY = atoi(tmpPt[0].c_str());
            endVec.push_back(ccp(ptX, ptY));
            relVec.push_back(isOpen);
        }
        addLine(ccp(info.posX, info.posY), endVec, relVec, m_lineBatch, maxHeight);
    }
}

void ScienceView::onEnter()
{
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ScienceView::updateInfo), MSG_SCIENCE_DATA_REFRESH, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ScienceView::UpdateCellState), MSG_SCIENCE_CELL_REFRESH, NULL);
    
    UIComponent::getInstance()->showCloseBtn(true);
    
    setTitleName(CCCommonUtils::getNameById(CC_ITOA(m_scienceType)));
    
    if(m_openNum>0) {
        m_openNum1 = m_openNum;
        m_openNum++;
        this->retain();
        PopupViewController::getInstance()->removePopupView(this);
        PopupViewController::getInstance()->addPopupInView(ScienceView::create(m_buildId, m_scienceType));
        PopupViewController::getInstance()->addPopupView(SciencePopupView::create(m_scienceIdstatic));
        this->release();
    }
    else
        m_openNum++;
    
    if (GuideController::share()->getCurGuideID() == "3040400" || GuideController::share()->getCurGuideID() == "3040500") {
        auto& info = GlobalData::shared()->scienceInfo.infoMap[700100];
        int lastTime = info.finishTime - GlobalData::shared()->getWorldTime();
        if (info.scienceLv > 0 || lastTime > 0) {
            GuideController::share()->setGuideEnd();
        }
    }
}

void ScienceView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SCIENCE_DATA_REFRESH);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SCIENCE_CELL_REFRESH);
    if (m_openNum<=1) {
        m_scrollView->removeAllChildren();
        m_lineBatch = NULL;
        m_commonBatch = NULL;
        m_itemBatch = NULL;
        m_iconBatch = NULL;
    }
    
    UIComponent::getInstance()->showCloseBtn(false);
    
    CCLoadSprite::doResourceByCommonIndex(4, false);
    CCLoadSprite::doResourceByCommonIndex(5, false);
    
    CCNode::onExit();
}

void ScienceView::addBGPic(){
    int totalH = 0;
    auto size = CCDirector::sharedDirector()->getWinSize();
    while (totalH < size.height) {
//        auto sprite = CCLoadSprite::createSprite("kji_bg.png");
        auto sprite = CCCommonUtils::addFilterSprite("UI_UseSkill_picture_blackwhite.png", CCCommonUtils::covertHSBToRGB(55, 0.77, 1.0), 0, 1.04, 4.4);

        if (CCCommonUtils::isIosAndroidPad()) {
            sprite->setScale(2.4);
        }
        this->m_bgContainer->addChild(sprite);
        sprite->setAnchorPoint(ccp(0, 0));
        sprite->setPosition(ccp(0, totalH));
        totalH += sprite->getContentSize().height * sprite->getScaleY();
    }
}


SEL_CCControlHandler ScienceView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool ScienceView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgContainer", CCNode*, this->m_bgContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, this->m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode1", CCNode*, this->m_fireNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_fireNode2", CCNode*, this->m_fireNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_effectNode", CCNode*, this->m_effectNode);

    return false;
}

CCNode *ScienceView::getGuideNode(int sid){
    auto& arr = m_scrollView->getContainer()->getChildren();
    for(auto child : arr)
    {
        ScienceCell *cell = dynamic_cast<ScienceCell*>(child);
        if(cell && cell->m_scienceId == sid){
            return cell;
        }
    }
    
//    int i = 0;
//    while(i < arr->count()){
//        ScienceCell *cell = dynamic_cast<ScienceCell*>(arr->objectAtIndex(i++));
//        if(cell && cell->m_scienceId == sid){
//            return cell;
//        }
//    }
    return NULL;
}

void ScienceView::setAttentionTo(int sid){
    if(sid != 0){
        auto& arr = m_scrollView->getContainer()->getChildren();
        for(auto child : arr)
        {
            ScienceCell *cell = dynamic_cast<ScienceCell*>(child);
            if(cell && cell->m_scienceId == sid){
                cell->setAttention();
                int totalH = m_scrollView->getContentSize().height;
                m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - totalH + (totalH - cell->getPositionY()) - CCDirector::sharedDirector()->getWinSize().height / 2));
                
                break;
            }
        }
//        int i = 0;
//        while(i < arr->count()){
//            ScienceCell *cell = dynamic_cast<ScienceCell*>(arr->objectAtIndex(i++));
//            if(cell && cell->m_scienceId == sid){
//                cell->setAttention();
//                int totalH = m_scrollView->getContentSize().height;
//                m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - totalH + (totalH - cell->getPositionY()) - CCDirector::sharedDirector()->getWinSize().height / 2));
//                
//                break;
//            }
//        }
    }
}

void ScienceView::doWhenClose()
{
    if (isReturn) {
        PopupViewController::getInstance()->addPopupInView(ScienceListView::create(m_buildId));
    }
}

///////
ScienceCell* ScienceCell::create(int scienceId, CCSpriteBatchNode* batch, CCSpriteBatchNode* iconBatch, int posx, int posy) {
    auto ret = new ScienceCell();
    if (ret && ret->init(scienceId, batch, iconBatch, posx, posy)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

ScienceCell::~ScienceCell()
{
}

bool ScienceCell::init(int scienceId, CCSpriteBatchNode* batch, CCSpriteBatchNode* iconBatch, int posx, int posy)
{
    m_scienceId = scienceId;
    m_batch = batch;
    m_iconBatch = iconBatch;
    m_posx = posx;
    m_posy = posy;
    
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(4, false);
    });
    
    m_ccbNode = CCBLoadFile("ScienceCellView",this,this);//ScienceCellView
    
    this->setContentSize(m_ccbNode->getContentSize());
    if (CCCommonUtils::isIosAndroidPad())
    {
        this->setAnchorPoint(ccp(0.5, 0.5));
    }
    m_nameLabel->setString(CCCommonUtils::getNameById(CC_ITOA(scienceId)).c_str());
//    m_bg->removeFromParent();
//    m_bg->setPosition(ccpAdd(m_bg->getPosition(), ccp(posx, posy)));
//    m_batch->addChild(m_bg);
//    m_stateLabel->setString("");
    
    m_buildLv = 0;
    int bId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_SCIENE);
    if (bId>0) {
        auto& bInfo = FunBuildController::getInstance()->getFunbuildById(bId);
        m_buildLv = bInfo.level;
    }

    m_proTimer = CCProgressTimer::create(CCLoadSprite::createSprite("IconGlow.png"));
    m_proTimer->setPosition(Vec2(m_proTimer->getContentSize()/2)+Vec2(4,4));
    m_proTimer->setType(kCCProgressTimerTypeRadial);
    m_proTimer->setVisible(false);
    m_proTimer->setPercentage(0);
    m_bg->addChild(m_proTimer);
    m_timershow->setVisible(false);
    m_timecntslabel->setVisible(false);
    refreash(NULL);
    onEnterFrame(0);
    return true;
}

void ScienceCell::setAttention(){
    auto sprite = CCLoadSprite::createScale9Sprite("rect.png");
    if (CCCommonUtils::isIosAndroidPad()) {
        sprite->setScale(2);
    }
    this->addChild(sprite);
    sprite->setScale(this->getContentSize().width / sprite->getContentSize().width);
    sprite->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
    sprite->setOpacity(0);
    CCSequence *seque = CCSequence::create(CCFadeIn::create(0.3), CCFadeOut::create(0.3) ,NULL);
    CCSequence *seque1 = CCSequence::create(CCRepeat::create(seque, 5), CCRemoveSelf::create(), NULL);
    sprite->runAction(seque1);
}

void ScienceCell::refreash(CCObject* obj)
{
    // 貌似出现过m_scienceId无效的情况
    if (GlobalData::shared()->isScienceValid(m_scienceId) == false) {
        CCLOG("ScienceCell::refreash invalid scienceId(%d).", m_scienceId);
        m_picNode->removeAllChildren();
        return;
    }
    
    bool tmpSt = false;
    if(obj) {
        int sid = dynamic_cast<CCInteger*>(obj)->getValue();
        if (sid == m_scienceId) {
            m_proTimer->setVisible(false);
            m_particleNode->removeAllChildren();
            string tmpStart = "ScienceOut_";
            int count = 9;
            for (int i=1; i<count; i++) {
                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
                m_particleNode->addChild(particle);
                tmpSt = true;
            }
            m_picNode->removeAllChildren();
            string picName = CCCommonUtils::getIcon(CC_ITOA(m_scienceId));
            auto pic = CCLoadSprite::createSprite(picName.c_str(), true, CCLoadSpriteType_SCIENCE);
            if (pic) {
                if (CCCommonUtils::isIosAndroidPad()) {
                    CCCommonUtils::setSpriteMaxSize(pic, 240, true);
                }
                else {
                    CCCommonUtils::setSpriteMaxSize(pic, 120, true);
                }
                CCCommonUtils::setSpriteGray(m_bg, false);
                 CCCommonUtils::setSpriteGray(m_num_di, false);
                CCCommonUtils::setSpriteGray(m_labdi, false);
                
                m_picNode->addChild(pic);
            }
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCIENCE_RESEARCH_FINISH);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCIENCE_CELL_REFRESH);
        }
    }
    
    auto& info = GlobalData::shared()->scienceInfo.infoMap[m_scienceId];
    m_newIcon->setVisible(false);
    
    string tmpStr = CC_ITOA(info.scienceLv);
    tmpStr = tmpStr + "/" + CC_ITOA(info.scienceMaxLv);
    m_lvLabel->setString(tmpStr);
    
    if (info.scienceLv == info.scienceMaxLv) {
        this->getAnimationManager()->runAnimationsForSequenceNamed("Complete");
    }
    if(m_isInit && m_isOpen) {
        return;
    }
    
    string picName = CCCommonUtils::getIcon(CC_ITOA(m_scienceId));
    auto grayPic = nullptr;
    if (info.scienceLv<=0 && GlobalData::shared()->scienceInfo.infoMap[m_scienceId].getCondition()>0){
        if (!m_isInit) {
            m_type = -1;
        }
        m_isOpen = false;
    }
    else {
        if (!m_isInit || !m_isOpen) {
            m_picNode->removeAllChildren();
            auto pic = CCLoadSprite::createSprite(picName.c_str(), true, CCLoadSpriteType_SCIENCE);
            if (pic) {
                if (CCCommonUtils::isIosAndroidPad()) {
                    CCCommonUtils::setSpriteMaxSize(pic, 240, true);
                }
                else
                    CCCommonUtils::setSpriteMaxSize(pic, 120, true);
                CCCommonUtils::setSpriteGray(m_bg, false);
                CCCommonUtils::setSpriteGray(m_num_di, false);
                CCCommonUtils::setSpriteGray(m_labdi, false);
                m_picNode->addChild(pic);
            }
            
            m_isInit = true;
            m_isOpen = true;
            
            auto& m_info = GlobalData::shared()->scienceInfo.infoMap[m_scienceId];
            resItems.clear();
            CCCommonUtils::splitString(m_info.resCondition, "|", resItems);
            buildItems.clear();
            CCCommonUtils::splitString(m_info.buildCondition, "|", buildItems);
            if (m_info.scienceLv==0)
            {
                string newKey = CCUserDefault::sharedUserDefault()->getStringForKey(CC_ITOA(m_scienceId),"");
                if (newKey == "1") {
                    m_newIcon->setVisible(true);
                }
                    m_type = 0;
            }
            else {
                m_type = 0;
            }
        }
    }

    if (info.scienceLv<=0){
            m_picNode->removeAllChildren();
            auto grayPic = CCLoadSprite::createSprite(picName.c_str(), true, CCLoadSpriteType_SCIENCE);
            CCCommonUtils::setSpriteGray(grayPic, true);
            if (CCCommonUtils::isIosAndroidPad()) {
                CCCommonUtils::setSpriteMaxSize(grayPic, 240, true);
            }
            else
                CCCommonUtils::setSpriteMaxSize(grayPic, 120, true);
            CCCommonUtils::setSpriteGray(m_bg, true);
        CCCommonUtils::setSpriteGray(m_num_di, true);
        CCCommonUtils::setSpriteGray(m_labdi, true);
            m_picNode->addChild(grayPic);
//          m_nameLabel->setColor(ccGRAY);
    }
}

void ScienceCell::setTouchNode(CCNode* node)
{
    m_touchNode = node;
}

bool ScienceCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if(is_playing) {
        return false;
    }
    if (m_touchNode && isTouchInside(m_touchNode, pTouch) && isTouchInside(m_mainNode, pTouch)) {
        auto tmpPt = pTouch->getLocation();
        m_startY = tmpPt.y;
        return true;
    }
    return false;
}

void ScienceCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    
    auto tmpPt = pTouch->getLocation();
    if(fabs(tmpPt.y - m_startY) >= 20){
        return;
    }
    tempY = m_posy- tmpPt.y + CCDirector::sharedDirector()->getWinSize().height;
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           , CCString::createWithFormat("SC2_%d", m_scienceId));
    
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    PopupViewController::getInstance()->addPopupView(SciencePopupView::create(m_scienceId));
    m_scienceIdstatic = m_scienceId;
    if (m_newIcon->isVisible()) {
        CCUserDefault::sharedUserDefault()->setStringForKey(CC_ITOA(m_scienceId),"");
        CCUserDefault::sharedUserDefault()->flush();
        m_newIcon->setVisible(false);
    }
}

bool ScienceCell::iscanupfun(){
    bool isCanUp = true;
    auto& m_info = GlobalData::shared()->scienceInfo.infoMap[m_scienceId];
    vector<string> scienceConItem;
    CCCommonUtils::splitString(m_info.scienceCondition, ";", scienceConItem);
    for (int i=0; i<scienceConItem.size(); i++) {
        int curCond = atoi(scienceConItem[i].c_str());
        int scienceConditionLv = curCond%100;
        int scienceConditionId = curCond - scienceConditionLv;
        
        auto item1 = UpgradeCell::create(5, scienceConditionId, scienceConditionLv);
        if (!item1->m_isOk) {
            isCanUp = false;
        }
    }
    vector<string> buildItems;
    CCCommonUtils::splitString(m_info.buildCondition, "|", buildItems);
    for (int i=0; i<buildItems.size(); i++) {
        int buildId = atoi(buildItems[i].c_str());
        auto item1 = UpgradeCell::create(2, buildId, 0, true);
        if (!item1->m_isOk) {
            isCanUp = false;
        }
    }
    return isCanUp;
}

void ScienceCell::animationFunc()
{
    is_playing = false;
    string name = this->getAnimationManager()->getLastCompletedSequenceName();
    if (name == "LockOut") {
        m_particleNode->removeAllChildren();
        
        m_proTimer->setVisible(false);
        this->unschedule(schedule_selector(ScienceCell::updateTime));
        PopupViewController::getInstance()->addPopupView(SciencePopupView::create(m_scienceId));
    }
}

void ScienceCell::onEnter() {
    CCNode::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ScienceCell::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    setSwallowsTouches(false);
    setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ScienceCell::refreash), MSG_SCIENCE_RESEARCH_FINISH, NULL);
}

void ScienceCell::onExit() {
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ScienceCell::onEnterFrame), this);
    this->unschedule(schedule_selector(ScienceCell::updateTime));
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SCIENCE_RESEARCH_FINISH);
    
    if(m_picNode) {
        m_picNode->removeAllChildren();
    }
    m_batch->removeAllChildren();
    m_batch = NULL;
    m_iconBatch->removeAllChildren();
    m_iconBatch = NULL;
    CCNode::onExit();
}

SEL_CCControlHandler ScienceCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool ScienceCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, this->m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, this->m_mainNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvLabel", CCLabelIF*, this->m_lvLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCSprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockSpr", CCSprite*, this->m_lockSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode", CCNode*, m_particleNode);
     CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_completePar", CCNode*, m_completePar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newIcon", CCSprite*, this->m_newIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timecntslabel", CCLabelIF*, this->m_timecntslabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timershow", CCSprite*, this->m_timershow);
     CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num_di", CCSprite*, this->m_num_di);
     CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_num_di", CCSprite*, this->m_labdi);
    
    return false;
}

void ScienceCell::updateTime(float dt)
{
    auto& info = GlobalData::shared()->scienceInfo.infoMap[m_scienceId];
    if(m_isOpen) {
        int lastTime = info.finishTime - GlobalData::shared()->getWorldTime();
        if (lastTime>0) {   //升级部分
            
            m_proTimer->setVisible(true);
            m_proTimer->setPercentage((1.0- (double)lastTime/(double)info.time_need)*100);
            
        }
    }
}

void ScienceCell::onEnterFrame(float dt)
{
    auto& info = GlobalData::shared()->scienceInfo.infoMap[m_scienceId];
    if (info.scienceLv == info.scienceMaxLv) {
        m_timecntslabel->setVisible(false);
        m_timershow->setVisible(false);
        m_lvLabel->setColor(ccc3(254, 214, 119));
      //  m_nameLabel->setColor(ccc3(254, 214, 119));
    }
    else if(info.scienceLv < info.scienceMaxLv){
        if(GlobalData::shared()->scienceInfo.infoMap[m_scienceId].CanResearch()){
            m_lvLabel->setColor(ccc3(0, 190, 0));
           // m_nameLabel->setColor(ccc3(254, 214, 119));
        }
        else{
            m_lvLabel->setColor(ccc3(200, 200, 200));
          //  m_nameLabel->setColor(ccc3(200, 200, 200));
        }
    }
    if (info.scienceLv == info.scienceMaxLv) {
        return;
    }
    int lastTime = info.finishTime - GlobalData::shared()->getWorldTime();

    m_timecntslabel->setString(CCCommonUtils::timeLeftToCountDown(lastTime));
    if(m_isOpen) {
        int lastTime = info.finishTime - GlobalData::shared()->getWorldTime();
        if (lastTime>0) {   //升级部分
            if(m_type != 1) {
                m_type=1;
                m_particleNode->removeAllChildren();
                m_proTimer->setVisible(true);
                 m_proTimer->setPercentage((1.0- (double)lastTime/(double)info.time_need)*100);
                 this->schedule(schedule_selector(ScienceCell::updateTime), 1, -1, 1);
                auto particle = ParticleController::createParticle("researching");
                if(CCCommonUtils::isIosAndroidPad())
                {
                    particle->setScale(2);
                }
                m_particleNode->addChild(particle);
               // m_timershow->setVisible(true);
                m_timecntslabel->setVisible(true);
                m_timecntslabel->setString(CCCommonUtils::timeLeftToCountDown(lastTime));
            }
        }
        else {
            m_proTimer->setVisible(false);
            m_timershow->setVisible(false);
            m_timecntslabel->setVisible(false);
            bool m_isOk = true;
            for (int i=0; i<resItems.size(); i++) {
                vector<string> resCell;
                CCCommonUtils::splitString(resItems[i], ";", resCell);
                int resType = atoi(resCell[0].c_str());
                int resNum = atoi(resCell[1].c_str());
                m_isOk = CCCommonUtils::isEnoughResourceByType(resType, resNum);
                if (!m_isOk) {
                    break;
                }
            }
            if(m_type != 0 && m_type != 2) {
                m_type=0;
            }
        }
    }
    else { 
        if ( GlobalData::shared()->scienceInfo.infoMap[m_scienceId].CanResearch() ) {
            refreash(NULL);
        }
    }
}



SciencePopupView* SciencePopupView::create(int scienceId) {
    auto ret = new SciencePopupView();
    if (ret && ret->init(scienceId)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool SciencePopupView::init(int scienceId) {
    bool ret = false;
    if (PopupBaseView::init()) {
        setIsHDPanel(true);
        setModelLayerTouchPriority(Touch_Popup_Item);
        m_scienceId = scienceId;
        CCBLoadFile("ScienceUpView",this,this);//SciencePopupView
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        setContentSize(size);
        
        CCLoadSprite::doResourceByCommonIndex(5, true);
        
        m_scrollView = CCScrollView::create(m_infoList->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_infoList->addChild(m_scrollView);
        m_littleshadow->setVisible(false);
        m_mateneeded->setVisible(false);
        updateInfo(NULL);
        ret = true;
    }
    return ret;
}

void SciencePopupView::updateInfo(CCObject* obj)
{
    if (GlobalData::shared()->isScienceValid(m_scienceId) == false) {
        CCLOG("SciencePopupView::updateInfo - invalid scienceId(%d)", m_scienceId);
        return;
    }
    
    int type = 0;
   // m_nameLabel->setColor(ccc3(254,214,119));
    m_nameLabel->setString(CCCommonUtils::getNameById(CC_ITOA(m_scienceId)).c_str());
    string des = CCCommonUtils::getPropById(CC_ITOA(m_scienceId), "description");
    m_desLabel->setString(_lang(des).c_str());
    
    auto& info = GlobalData::shared()->scienceInfo.infoMap[m_scienceId];
    string tmpStr = CC_ITOA(info.scienceLv);
    tmpStr = tmpStr + "/" + CC_ITOA(info.scienceMaxLv);
    m_barLabel->setString(tmpStr.c_str());
    float pro = 0;
    if (info.scienceMaxLv>0) {
        pro = info.scienceLv*1.0/info.scienceMaxLv;
    }
    m_bar->setScaleX(pro);
    if (info.scienceLv >= info.scienceMaxLv) {
        type = 2;
    }
    
    _isOpen = true;
    
    // FIXME : 这里的贴图曾经有问题，但无法重现。
    m_picNode->removeAllChildren();
    string picName = CCCommonUtils::getIcon(CC_ITOA(m_scienceId));
    auto pic = CCLoadSprite::createSprite(picName.c_str(), true, CCLoadSpriteType_SCIENCE);
    if (pic) {
        if (info.getCondition() > 0 ) {
            CCCommonUtils::setSpriteGray(pic, true);
            _isOpen = false;
            type = 3;
        }
        CCCommonUtils::setSpriteMaxSize(pic, 120, true);
        m_picNode->addChild(pic);
    }
    
    if (info.finishTime > GlobalData::shared()->getWorldTime()) {
        CCCommonUtils::setButtonTitle(m_upBtn, _lang("104903").c_str());
        m_btnMsgLabel->setString(_lang("102102").c_str());
        if (CCCommonUtils::isIosAndroidPad()) {
            float fw = m_btnMsgLabel->getContentSize().width;
            float s = m_btnMsgLabel->getScaleX();
            if (fw>360.0) {//防爆框
                m_btnMsgLabel->setScaleX(360.0/fw*s);
            }
        }
        else
            m_btnMsgLabel->setMaxScaleXByWidth(180.0);
        m_infoNode->setVisible(false);
        CCCommonUtils::setButtonTitle(m_instantBtn, _lang("108532").c_str());
        CCCommonUtils::setButtonSprite(m_instantBtn, "mail_red_btn.png");
        m_isUping = true;
        type = 1;
    }
    else {
        CCCommonUtils::setButtonTitle(m_upBtn, _lang("121994").c_str());
        m_btnMsgLabel->setString(_lang("102102").c_str());
        if (CCCommonUtils::isIosAndroidPad()) {
            float fw = m_btnMsgLabel->getContentSize().width;
            float s = m_btnMsgLabel->getScaleX();
            if (fw>360.0) {//防爆框
                m_btnMsgLabel->setScaleX(360.0/fw*s);
            }
        }
        else
            m_btnMsgLabel->setMaxScaleXByWidth(180.0);
        CCCommonUtils::setButtonTitle(m_instantBtn, "");
        CCCommonUtils::setButtonSprite(m_instantBtn, "btn_green3.png");
        m_infoNode->setVisible(true);
        m_isUping = false;
    }
    
    vector<string> curPara;
    CCCommonUtils::splitString(info.para2, "|", curPara);
    vector<string> nextPara;
    CCCommonUtils::splitString(info.nextPara2, "|", nextPara);
    string curStr = "";
    if (curPara.size()>0) {
        curStr = curPara[0];
    }
    string nextStr = "";
    if (nextPara.size()>0) {
        nextStr = nextPara[0];
    }
    if (info.show == 1) {
        curStr += "%";
        nextStr += "%";
    }
    m_curLabel->setString(_lang_1("121995",""));
    m_nextLabel->setString(_lang_1("121996",""));
    m_curNumLabel->setString(curStr);
    m_nextNumLabel->setString(nextStr);
    
    onUpdateRes(type);
}

void SciencePopupView::onUpdateRes(int type)
{
    bool isCanUp = true;
    auto& m_info = GlobalData::shared()->scienceInfo.infoMap[m_scienceId];
    
    int _tmpTime = m_info.time_need/(1+CCCommonUtils::getEffectValueByNum(69)*1.0/100);
    _tmpGold = CCCommonUtils::getGoldByTime(_tmpTime);
    string timeInfo = CC_SECTOA(_tmpTime);
    m_timeLabel->setString(timeInfo.c_str());
    
    _resGold = 0;
    m_lfood = 0;
    m_lwood = 0;
    m_liron = 0;
    m_lstone = 0;
    
    int _itemH = 42;
    int curY = 0;
    int curX = 0;
    if (CCCommonUtils::isIosAndroidPad()) {
        _itemH = 84;
        curY = 100;
        curX = 0;
    }
    m_scrollView->getContainer()->removeAllChildren();
    m_inBtnGoldNum->setString("9999");
    if (type==0 || type==3) {
        m_mateneeded->setString(_lang("102130"));
        m_mateneeded->setVisible(true);
        m_littleshadow->setVisible(true);
        vector<string> resItems;
        CCCommonUtils::splitString(m_info.resCondition, "|", resItems);
        for (int i=0; i<resItems.size(); i++) {
            vector<string> resCell;
            CCCommonUtils::splitString(resItems[i], ";", resCell);
            int resType = atoi(resCell[0].c_str());
            int resNum = atoi(resCell[1].c_str());
            
            if(resNum>0) {
                auto item1 = UpgradeCell::create(0, resType, resNum, true);
                item1->setPosition(ccp(curX, curY));
                item1->setTouchNode(m_infoList,m_scienceId, 1);
                m_scrollView->addChild(item1);
                curY += _itemH;
                
                if (!item1->m_isOk) {
//                    isCanUp = false;
                }
                
                int tmpSub = resNum - CCCommonUtils::getCurResourceByType(resType);
                if (tmpSub>0) {
                    int subGold = CCCommonUtils::getResGlodByType(resType, tmpSub);
                    _tmpGold += subGold;
                    _resGold += subGold;
                    if (resType == Food) {
                        m_lfood = tmpSub;
                    }
                    else if (resType == Wood) {
                        m_lwood = tmpSub;
                    }
                    else if (resType == Iron) {
                        m_liron = tmpSub;
                    }
                    else if (resType == Stone) {
                        m_lstone = tmpSub;
                    }
                }
            }
        }
        m_inBtnGoldNum->setString(CC_ITOA(_tmpGold));
        
        if (true) {//type==3 || m_info.scienceLv<=0
            vector<string> scienceConItem;
            CCCommonUtils::splitString(m_info.scienceCondition, ";", scienceConItem);
            for (int i=0; i<scienceConItem.size(); i++) {
                int curCond = atoi(scienceConItem[i].c_str());
                int scienceConditionLv = curCond%100;
                int scienceConditionId = curCond - scienceConditionLv;
                
                auto item1 = UpgradeCell::create(5, scienceConditionId, scienceConditionLv);
                item1->setPosition(ccp(curX, curY));
                item1->setTouchNode(m_infoList,m_scienceId, 1);
                m_scrollView->addChild(item1);
                curY += _itemH;
                
                if (!item1->m_isOk) {
                    isCanUp = false;
                }
            }
        }
        
        vector<string> buildItems;
        CCCommonUtils::splitString(m_info.buildCondition, "|", buildItems);
        for (int i=0; i<buildItems.size(); i++) {
            int buildId = atoi(buildItems[i].c_str());
            auto item1 = UpgradeCell::create(2, buildId, 0, true);
            item1->setPosition(ccp(curX, curY));
            item1->setTouchNode(m_infoList,m_scienceId, 1);
            m_scrollView->addChild(item1);
            curY += _itemH;
            
            if (!item1->m_isOk && !m_isUping) {
                isCanUp = false;
                m_instantBtn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("Btn_grey.png"), CCControlStateDisabled);
                m_instantBtn->setEnabled(false);
            }
        }
        
        int qid = QueueController::getInstance()->canMakeItemByType(TYPE_SCIENCE, _tmpTime);
        if(QID_MAX == qid)
        {
            int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_SCIENCE);
            auto itemQueue = UpgradeCell::create(3, Food, qid);
            itemQueue->setPosition(ccp(curX, curY));
            itemQueue->setTouchNode(m_infoList,m_scienceId, 1);
            m_scrollView->addChild(itemQueue);
            curY += _itemH;
        }

    }
    else {
//        if (!_isOpen) {
//            vector<string> scienceConItem;
//            CCCommonUtils::splitString(m_info.scienceCondition, ";", scienceConItem);
//            auto &scienceMap = GlobalData::shared()->scienceInfo.infoMap;
//            vector<string> vecInfo;
//            for (int i=0; i<scienceConItem.size(); i++) {
//                int curCond = atoi(scienceConItem[i].c_str());
//                int scienceConditionLv = curCond%100;
//                int scienceConditionId = curCond - scienceConditionLv;
//                if (scienceMap.find(scienceConditionId) == scienceMap.end() || scienceMap[scienceConditionId].scienceLv < scienceConditionLv ) {
//                    string name = CCCommonUtils::getNameById(CC_ITOA(scienceConditionId));
//                    string tmp_Info = _lang_2("121989", name.c_str(), CC_ITOA(scienceConditionLv));// 121989
//                    vecInfo.push_back(tmp_Info);
//                }
//            }
//            
//            curY = (m_infoList->getContentSize().height-_itemH*vecInfo.size())/2;
//            for (int j=0; j<vecInfo.size(); j++) {
//                auto label = CCLabelIF::create(vecInfo[j].c_str(), 24, CCSizeMake(460, 0), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
//                label->setColor(ccRED);
//                int labelW = label->getContentSize().width*label->getOriginScaleX();
//                label->setPosition((m_infoList->getContentSize().width-labelW)/2, curY);
//                m_scrollView->addChild(label);
//                curY += _itemH;
//            }
//            
//            curY += _itemH;
//            curY -= 20;
//            curY = m_infoList->getContentSize().height;
//        }
    }
    m_upBtn->setEnabled(isCanUp);
    
    curY -= 23;
    if (CCCommonUtils::isIosAndroidPad()) {
        curY -= 30;
    }
    m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width,curY));
    m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - curY));
    if(curY <= m_infoList->getContentSize().height) {
        m_scrollView->setTouchEnabled(false);
    }
    
    m_timeBarNode->setVisible(false);
    _lastTime = -1;
    if(m_isUping) {
        m_upBtn->setEnabled(true);
        m_instantBtn->setEnabled(true);
        _lastTime = m_info.finishTime - GlobalData::shared()->getWorldTime();
        _sumTime = m_info.time_need;
    }
    else {
        if (!_isOpen) {
            m_upBtn->setEnabled(false);
            m_instantBtn->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("Btn_grey.png"), CCControlStateDisabled);
            m_instantBtn->setEnabled(false);
        }
    }
    onChangeBG(type);
    onEnterFrame(0);
}

void SciencePopupView::onChangeBG(int type)
{
    if (type==1 || type==2) {
        m_bigBG->setVisible(false);
        m_smallBG->setVisible(true);
        m_listBG->setVisible(false);
        m_btnNode->setPositionY(77);
        if (type==1) {//正在升级m_littleshadow
            if(CCCommonUtils::isIosAndroidPad()){
                m_upBtn->setPositionX(-240);
                m_infoNode->setPositionX(0);
            }
            else{
                m_upBtn->setPositionX(0);
                m_infoNode->setPosition(m_upBtn->getPosition());
            }
            CCCommonUtils::setButtonTitle(m_upBtn, "");
            m_btnMsgLabel->setString(_lang("104903"));
            m_inBtnGoldNum->setString("99");
            m_maxMsgLabel->setString("");
            m_maxBg->setVisible(false);
            m_infoNode->setVisible(true);
            
            m_instantBtn->setVisible(false);
            m_instantBtn->setEnabled(false);
        }
        else {//达到最大级
            m_btnNode->setVisible(false);
            m_maxMsgLabel->setString(_lang("121987"));
             m_maxBg->setVisible(true);
            m_nextNode->setVisible(false);
        }
        m_mainNode->setPositionY(m_mainNode->getPositionY()-200);
    }
    else {
        m_bigBG->setVisible(true);
        m_smallBG->setVisible(false);
        m_listBG->setVisible(true);
        m_maxMsgLabel->setString("");
         m_maxBg->setVisible(false);
        m_btnNode->setPositionY(-218.6);
    }
}

void SciencePopupView::onEnter() {
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SciencePopupView::closeView), MSG_SCIENCE_CLOSE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SciencePopupView::updateInfo), MSG_SCIENCE_RESEARCH_FINISH, NULL);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(SciencePopupView::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
    PopupBaseView::onEnter();
}

void SciencePopupView::onExit() {
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SCIENCE_CLOSE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SCIENCE_RESEARCH_FINISH);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(SciencePopupView::onEnterFrame), this);
    setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool SciencePopupView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void SciencePopupView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_bigBG->isVisible() && isTouchInside(m_bigNode, pTouch)) {
        return ;
    }
    else if (m_smallBG->isVisible() && isTouchInside(m_smallNode, pTouch)) {
        return ;
    }
    PopupViewController::getInstance()->removePopupView(this);
}

SEL_CCControlHandler SciencePopupView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickCloseBtn", SciencePopupView::onClickCloseBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onInstantClick", SciencePopupView::onInstantClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onResearchBtnClick", SciencePopupView::onResearchBtnClick);
    return NULL;
}

bool SciencePopupView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_closeBtn", CCControlButton*, m_closeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_instantBtn", CCControlButton*, m_instantBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upBtn", CCControlButton*, m_upBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bar", CCScale9Sprite*, m_bar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeBar", CCScale9Sprite*, m_timeBar);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_barLabel", CCLabelIF*, m_barLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_curLabel", CCLabelIF*, m_curLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextLabel", CCLabelIF*, m_nextLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_curNumLabel", CCLabelIF*, m_curNumLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextNumLabel", CCLabelIF*, m_nextNumLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockSpr", CCSprite*, m_lockSpr);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnMsgLabel", CCLabelIF*, m_btnMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inBtnGoldNum", CCLabelIF*, m_inBtnGoldNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode", CCNode*, m_infoNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeBarNode", CCNode*, m_timeBarNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bigNode", CCNode*, m_bigNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_smallNode", CCNode*, m_smallNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_maxMsgLabel", CCLabelIF*, m_maxMsgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_maxBg", CCScale9Sprite*, m_maxBg);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bigBG", CCNode*, m_bigBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_smallBG", CCNode*, m_smallBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listBG", CCScale9Sprite*, m_listBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode", CCNode*, m_btnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mainNode", CCNode*, m_mainNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_curNode", CCNode*, m_curNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nextNode", CCNode*, m_nextNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mateneeded", CCLabelIF*, m_mateneeded);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_littleshadow", CCScale9Sprite*, m_littleshadow);
    return false;
}

void SciencePopupView::onEnterFrame(float dt)
{
    if(_lastTime>=0) {
        m_timeBarNode->setVisible(true);
        m_timeLabel->setString(CC_SECTOA(_lastTime));
        _lastTime--;
        float pro = _lastTime*1.0/_sumTime;
        m_timeBar->setScaleX(1-pro);
        _tmpGold = CCCommonUtils::getGoldByTime(_lastTime);
        m_inBtnGoldNum->setString(CC_ITOA(_tmpGold));
        if(_lastTime<0) {
            PopupViewController::getInstance()->removePopupView(this);
        }
    }
}

void SciencePopupView::onClickCloseBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    PopupViewController::getInstance()->removePopupView(this);
}

void SciencePopupView::closeView(CCObject* obj)
{
    PopupViewController::getInstance()->removePopupView(this);
}

void SciencePopupView::onInstantClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_isUping) {
        YesNoDialog::show( _lang("101320").c_str() , CCCallFunc::create(this, callfunc_selector(SciencePopupView::onYesCancel)));
    }
    else {
        YesNoDialog::showButtonAndGold(_lang("102370").c_str(), CCCallFunc::create(this, callfunc_selector(SciencePopupView::onOKInstantResearch)),m_btnMsgLabel->getString().c_str(), _tmpGold);
    }
}

void SciencePopupView::onOKInstantResearch()
{
    isReturn = false;
    if (_tmpGold > GlobalData::shared()->playerInfo.gold) {
        YesNoDialog::gotoPayTips();
        return;
    }
    ScienceController::getInstance()->startDirResearchScience(m_scienceId);
    PopupViewController::getInstance()->removePopupView(this);
}

void SciencePopupView::onYesCancel()
{
    string key = QueueController::getInstance()->getKey(m_scienceId, TYPE_SCIENCE);
    int qid = QueueController::getInstance()->getQueueQidByKey(key);
    if (qid>0) {
        GlobalData::shared()->scienceInfo.infoMap[m_scienceId].finishTime = 0;
        QueueController::getInstance()->startCancelQueue(qid);
        isReturn = false;
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SCIENCE_CELL_STATE);
        PopupViewController::getInstance()->removePopupView(this);
    }
}

void SciencePopupView::onResearchBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    
    if (m_isUping) {
        YesNoDialog::showTime( _lang("102283").c_str() , CCCallFunc::create(this, callfunc_selector(SciencePopupView::onOKCostCD)), _lastTime, _lang("104903").c_str(), false);
    }
    else {
        auto& info = GlobalData::shared()->scienceInfo.infoMap[m_scienceId];
        int qid = QueueController::getInstance()->canMakeItemByType(TYPE_SCIENCE, info.time_need);
        if(QID_MAX == qid)
        {
            CCCommonUtils::flyHint("icon_time.png", "", _lang("102504"));
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUILD_CELL);
//            CCCommonUtils::flyText(_lang("102152"));
            return;
        }
        if (_resGold>0) {
            PopupViewController::getInstance()->addPopupView(ConfirmDialogView::create(m_lfood,m_lwood,m_liron,m_lstone,0,_resGold
                                                                                       ,this,callfunc_selector(SciencePopupView::lastResearch)
                                                                                       , _lang("102314"), _lang("121994")),false);
        }
        else {
            lastResearch();
        }
    }
}

void SciencePopupView::onOKCostCD()
{
    if (m_isUping) {
        string key = QueueController::getInstance()->getKey(m_scienceId, TYPE_SCIENCE);
        int qid = QueueController::getInstance()->getQueueQidByKey(key);
        if (qid>0) {
            _tmpGold = CCCommonUtils::getGoldByTime(_lastTime);
            QueueController::getInstance()->startCCDQueue(qid, "", false, _tmpGold,"",4);
            
        }
        isReturn = false;
    }
    PopupViewController::getInstance()->removePopupView(this);
}

void SciencePopupView::lastResearch()
{
    isReturn = false;
    
    
    ScienceController::getInstance()->startResearchScience(m_scienceId, _resGold);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           , CCString::createWithFormat("SC3_%d_research", m_scienceId));
    PopupViewController::getInstance()->removePopupView(this);
}

CCNode *SciencePopupView::getGuideNode(int sid){
    if(this->m_scienceId == sid && m_upBtn->isEnabled()){
        return this->m_upBtn;
    }
    return NULL;
}
