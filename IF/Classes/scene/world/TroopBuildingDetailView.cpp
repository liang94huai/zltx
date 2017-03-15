//
//  TroopBuildingDetailView.cpp
//  IF
//
//  Created by xiaoming on 15/11/25.
//
//

#include "TroopBuildingDetailView.hpp"
#include "WorldCommand.h"
#include "TroopInformationView.h"
#include "GlobalData.h"
#include "YesNoDialog.h"
#include "ArmyController.h"
TroopBuildingDetailView::TroopBuildingDetailView(const string& marchId,int pointId)
{
    m_marchId=marchId;
    m_pointId=pointId;
}
TroopBuildingDetailView* TroopBuildingDetailView::create(const string& marchId,int pointId)
{
    auto ret = new TroopBuildingDetailView(marchId,pointId);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool TroopBuildingDetailView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title_label", CCLabelIF*, this->m_title_label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scrollLayer", CCNode*, this->m_scrollLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchLayer", CCNode*, this->m_touchLayer);
    return false;
}

bool TroopBuildingDetailView::init()
{
    PopupBaseView::init();
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(4, true);
    CCLoadSprite::doResourceByCommonIndex(502, true);
    
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(4, false);
        CCLoadSprite::doResourceByCommonIndex(502, false);
        
    });
    auto ccb = CCBLoadFile("TroopBuildingDetailView",this,this);
    this->setContentSize(ccb->getContentSize());
    this->setModelLayerOpacity(0);
    this->setModelLayerTouchCallback([&](cocos2d::CCTouch *pTouch){
        if (!isTouchInside(m_touchLayer, pTouch) && ccpDistance(pTouch->getStartLocation(), pTouch->getLocation())<30)
        {
            this->closeSelf();
        }
    });
    m_scrollView = CCScrollView::create(m_scrollLayer->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_scrollView->setPosition(Vec2(m_scrollLayer->getPositionX()-m_scrollLayer->getContentSize().width/2,m_scrollLayer->getPositionY()-m_scrollLayer->getContentSize().height/2));
    ccb->addChild(m_scrollView,100);
    m_scrollNode= CCNode::create();
    m_title_label->setString(_lang("108585"));
    m_scrollView->setContainer(m_scrollNode);
    m_scrollView->setPosition(m_scrollLayer->getPosition());
    kickingUuid="";
    this->sendRequest();
    return true;
}

void TroopBuildingDetailView::sendRequest() {
    if(m_marchId == "" && m_pointId==-1){
        return;
    }
    if(m_pointId !=-1){
        auto cmd = new WorldGetOccupiedTroopsCommand(m_pointId);
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(TroopBuildingDetailView::onRequestCallback), NULL));
        cmd->sendAndRelease();
    }else{
        auto cmd = new WorldMarchDetailUpdateCommand(m_marchId);
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(TroopBuildingDetailView::onRequestCallback), NULL));
        cmd->sendAndRelease();
    }
    
}

void TroopBuildingDetailView::onRequestCallback(CCObject* obj)
{
    auto ret = dynamic_cast<NetResult*>(obj);
    if (!ret || ret->getErrorCode() != Error_OK) {
        // todo : parse error
        return;
    }
    auto info = dynamic_cast<CCDictionary*>(ret->getData());
    this->updateTroops(info);
}

void TroopBuildingDetailView::updateTroops(CCDictionary* info)
{
    m_scrollNode->removeAllChildren();
    if(info!=NULL){
        m_info.setInfo(info);
    }
    bool isOwner=strcmp(GlobalData::shared()->playerInfo.uid.c_str(), m_info.ownerUid.c_str())==0;
    BuildingDetailCell *lastCell = NULL;
    float totalHeight=0;
    for (auto it = m_info.soldiers.begin(); it != m_info.soldiers.end(); it++) {
        auto player = it->second;
        std::string uuid=m_info.userUuids[it->first];
        if(this->checkBeKicked(uuid)){
            continue;
        }
        
        BuildingDetailCell *cell = BuildingDetailCell::create(&player,uuid,isOwner);
        if (cell==NULL) {
            continue;
        }
        cell->setDelegate(this);
        m_scrollNode->addChild(cell);
        if (lastCell==NULL) {
            cell->setPosition(Vec2(0, 0));
        }
        else{
            cell->setPosition(Vec2(0, lastCell->getPositionY()+lastCell->getContentSize().height+10));
        }
        lastCell=cell;
        totalHeight = totalHeight+cell->getContentSize().height+10;
    }
    m_scrollNode->setContentSize(Size(m_scrollLayer->getContentSize().width, totalHeight));
    m_scrollView->updateInset();
    m_scrollNode->setPosition(Vec2(0,m_scrollView->getViewSize().height-m_scrollNode->getContentSize().height));
    
    
    
    if(totalHeight<1){
        auto label = CCLabelIFTTF::create(_lang("140174").c_str(), "Helvetica", 42);
        m_scrollLayer->addChild(label);
        if (CCCommonUtils::isIosAndroidPad()) {
            label->setScale(2.4);
        }
        label->setPosition(Vec2(m_scrollLayer->getContentSize().width/2, m_scrollLayer->getContentSize().height/2));
        //label->setColor({0,0,255});
    }
}

bool TroopBuildingDetailView::checkBeKicked(std::string uuid)
{
    bool beKicked=false;
    for (int i=0; i<playKickedList.size(); i++) {
        if (strcmp(playKickedList.at(i).c_str(), uuid.c_str())==0) {
            beKicked=true;
            break;
        }
    }
    return beKicked;
}

void TroopBuildingDetailView::buildingCellKickClick(std::string uuid)
{
    this->sendKickCommand(uuid);
}

void TroopBuildingDetailView::sendKickCommand(std::string uuid)
{
    kickingUuid=uuid;
    auto cmd = new WorldMarchKickCommand(uuid);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(TroopBuildingDetailView::kickRequestCallBack), NULL));
    cmd->sendAndRelease();
}
void TroopBuildingDetailView::kickRequestCallBack(CCObject *obj)
{
    if (strcmp(kickingUuid.c_str(), "")!=0) {
        std::string newUuid=kickingUuid;
        playKickedList.push_back(newUuid);
    }
    this->updateTroops(NULL);
}



//BuildingDetailCell///////


BuildingDetailCell *BuildingDetailCell::create(vector<MarchSoldierDetail> *soldierList,std::string uuid,bool isOwner)
{
    auto ret = new BuildingDetailCell(soldierList,uuid,isOwner);
    ret->autorelease();
    return ret;
}
BuildingDetailCell::BuildingDetailCell(vector<MarchSoldierDetail> *soldierList,std::string uuid,bool isOwner)
{
    CCNode::init();
    float designScale=1;
    if (CCCommonUtils::isIosAndroidPad()) {
        designScale=2.4;
    }
    float cellWidth=300*designScale;
    float cellHeight=140*designScale;
    int heightDif=10*designScale;
    int widthDif=20*designScale;
    std::string strName="";
    int totalNum=0;
    this->uuid=uuid;
    myDelegate=NULL;
    int listSize = (int)soldierList->size();
    int addI=0;
    if (listSize>2&&listSize%2==1) {
        addI=1;
    }
    for (int i=0; i<listSize; i++) {
        auto info = soldierList->at(i);
        int star = 0;
        if (info.playerName == GlobalData::shared()->playerInfo.name) {
            star = ArmyController::getInstance()->getStarlvById(info.armyId);
        }else{
            star = info.m_star;
        }
        TroopArmyCell* cell = TroopArmyCell::create(CCCommonUtils::getNameById(info.armyId), CCCommonUtils::getResourceStr(info.count), info.armyId,star);
        this->addChild(cell);
        cell->setScale(designScale);
        int countI=i;
        if(i>0){
            countI=i+addI;
        }
        float posX=widthDif+(countI%2)*cellWidth;
        float posY=heightDif+(countI/2)*cellHeight;
        cell->setPosition(Vec2(posX,posY));
        if (i==0) {
            strName=info.playerName;
        }
        totalNum = info.count + totalNum;
    }
    int numY=listSize/2;
    int extY=cellHeight;
    if(listSize%2==0){
        extY=0;
    }
    auto bg = CCLoadSprite::createScale9Sprite("jj_bt_bg.png");
    bg->setAnchorPoint(ccp(0, 0));
    bg->setContentSize(CCSize(cellWidth*2-widthDif*2,34*designScale));
    bg->setPosition(Vec2(0,(soldierList->size()/2)*cellHeight+extY+10));
    this->addChild(bg);
    
    auto labelName = CCLabelIFTTF::create(strName.c_str(), "Helvetica", 20*designScale);
    bg->addChild(labelName);
    labelName->setAnchorPoint(Vec2(0, 0.5));
    labelName->setPosition(Vec2(widthDif, bg->getContentSize().height/2));
    labelName->setColor({255,255,0});
    
    std::string strTotal = _lang("108638")+"："+CCCommonUtils::getResourceStr(totalNum);
    auto labelNum = CCLabelIFTTF::create(strTotal.c_str(), "Helvetica", 20*designScale);
    bg->addChild(labelNum);
    labelNum->setAnchorPoint(Vec2(1, 0.5));
    labelNum->setPosition(Vec2(bg->getContentSize().width-widthDif*2, bg->getContentSize().height/2));
    labelNum->setColor({255,255,0});
    
    if(isOwner || (GlobalData::shared()->playerInfo.isInAlliance() && GlobalData::shared()->playerInfo.allianceInfo.rank>=5)){
        CCControlButton *btn_kick = CCControlButton::create(CCLoadSprite::createScale9Sprite("jj_btn_qiansan.png"));
        btn_kick->setPreferredSize(CCSizeMake(25, 25));
        btn_kick->setScale(designScale);
        btn_kick->addTargetWithActionForControlEvents(this, cccontrol_selector(BuildingDetailCell::troopKickClick), CCControlEventTouchUpInside);
        btn_kick->setPosition(Vec2(bg->getContentSize().width-widthDif,bg->getContentSize().height/2));
        bg->addChild(btn_kick);
        
    }
    userName=strName;

    this->setContentSize(Size(cellWidth*2,extY+40*designScale+cellHeight*((float)numY)));
}

void BuildingDetailCell::setDelegate(BuildingCellDelegate *buildingCellDelegate)
{
    myDelegate=buildingCellDelegate;
}

void BuildingDetailCell::troopKickClick(CCObject* pSender, CCControlEvent event)
{
    if(myDelegate!=NULL){
        static bool firstKickClick=true;
        std::string uuidTemp=this->uuid;
        auto func = [&,uuidTemp](){
            firstKickClick=false;
            myDelegate->buildingCellKickClick(uuidTemp);
        };
        if (firstKickClick) {
            YesNoDialog::show(_lang_1("106208", userName.c_str()),func);
        }
        else{
            func();
        }
    }
}