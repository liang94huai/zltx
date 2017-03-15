//
//  TipsWithPicView.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/4/2.
//
//

#include "TipsWithPicView.h"
#include "MiniMapController.h"

const float numPerRow = 1.0;
TipsWithPicView* TipsWithPicView::create(WorldCityInfo &info){
    TipsWithPicView *ret = new TipsWithPicView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TipsWithPicView::init(){
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(4, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(4, false);
        });
        
    if(m_info.cityType == Main_City)
    {
         auto tmpCCB = CCBLoadFile("TipsWithPicMainView",this,this,true);
        this->setContentSize(tmpCCB->getContentSize());
        
        initLabInfo2();
    }else if (m_info.cityType == ResourceTile)
    {
        auto tmpCCB = CCBLoadFile("TipsWithPicView",this,this,true);
        this->setContentSize(tmpCCB->getContentSize());
        
        initLabInfo();
      }
    }
        return true;
}
void TipsWithPicView::refreshData()
{
    
}


void TipsWithPicView::initLabInfo2()
{

    m_title->setFntFile("Arial_Bold_Border.fnt");
    m_title->setString(_lang("new100014").c_str());
    // m_title->setString("城池详情");
    
    m_cityNameTxt->setFntFile("Arial_Bold_Border.fnt");
    m_cityNameTxt->setString(_lang("new100015").c_str());
    // m_cityNameTxt->setString("城池名称:");
    
    m_cityName->setFntFile("Arial_Bold_Border.fnt");
    //m_title->setString(_lang("new100003").c_str());
    string id = "";
    if(m_info.cityType == Main_City)
    {
        id = m_info.kingBuildInfo.tid;
    }else if (m_info.cityType == ResourceTile)
    {

        int  iD = (m_info.resource.tId);
        id = CC_ITOA(iD);

         
    }
    
    auto dict = LocalController::shared()->DBXMLManager()->getObjectByKey(id);
    auto cityname = dict->valueForKey("name")->getCString();
    auto resHourInfo = dict->valueForKey("resPerHour")->getCString();
    auto resMaxInfo = dict->valueForKey("resPerHour")->getCString();
    auto garrisonCap = dict->valueForKey("garrisonCap")->getCString();
    onSplitResInfo(resHourInfo,m_mapResHourInfo);
    onSplitResInfo(resMaxInfo,m_mapMaxHourInfo);

    m_cityName->setString(_lang(cityname).c_str());
    
    auto areaname = WorldController::getInstance()->getHomeOwnershipNameByMapPos(m_info.cityTilePoint);
    m_zoneName->setString(areaname);
    
   // m_state->setString(m_info);
   // m_foodNumTxt->setString(m_info);
   // m_woodNumTxt->setString(m_info);
   // m_stoneNumTxt->setString(m_info);
   // m_foodNumTxt->setString(m_info);
    m_zoneName->setFntFile("Arial_Bold_Border.fnt");
    //m_zoneName->setString(_lang("new100003").c_str());
   // m_zoneName->setString(MiniMapController::getInstance()->getCityPosById(m_info.cityIndex)->m_ZoneName.c_str());
    
    m_zoneNameTxt->setFntFile("Arial_Bold_Border.fnt");
    m_zoneNameTxt->setString(_lang("new100016").c_str());
    // m_zoneNameTxt->setString("所处区域:");
    
    m_stateTxt->setFntFile("Arial_Bold_Border.fnt");
    m_stateTxt->setString(_lang("new100020").c_str());
    // m_stateTxt->setString("所属联盟:");
    
    m_state->setFntFile("Arial_Bold_Border.fnt");
    //m_state->setString(_lang("new100003").c_str()); 110090
    string fullName = _lang("110090").c_str();
    ccColor3B color = ccColor3B::WHITE;
    if (m_info.playerName != "")
    {
        color = ccColor3B::RED; //红色
        //联盟
        if (m_info.allianceName!= "")
        {
            fullName = string("[")+m_info.allianceName+string("] ");

            if (GlobalData::shared()->playerInfo.isInAlliance()) //同盟
            {
                if (m_info.allianceName == GlobalData::shared()->playerInfo.allianceInfo.name)
                {
                    color = ccc3(5,171,233);
                }
            }
        } else {
            fullName = m_info.playerName.c_str();
        }
         
    }
    
    
    
    m_state->setString(fullName.c_str());
    m_state->setColor(color);
    m_cityBuffTxt->setFntFile("Arial_Bold_Border.fnt");
    m_cityBuffTxt->setString(_lang("new100019").c_str());
    // m_cityBuffTxt->setString("城池加成:");
    m_buffTxt->setFntFile("Arial_Bold_Border.fnt");
    //m_title->setString(_lang("new100003").c_str());
    //m_buffTxt->setString("城池加成:");
    string strRate = _lang("new100021").c_str();
    auto buffinfo =  dict->valueForKey("resAddRatio")->getCString();
    string buffStr = strRate + string(CC_ITOA(atoi(buffinfo)/10)) + "%";
    m_buffTxt->setString(buffStr.c_str());
}

void TipsWithPicView::initLabInfo()
{
    m_title->setFntFile("Arial_Bold_Border.fnt");
    m_title->setString(_lang("new100014").c_str());
    // m_title->setString("城池详情");
    
    m_cityNameTxt->setFntFile("Arial_Bold_Border.fnt");
    m_cityNameTxt->setString(_lang("new100015").c_str());
    // m_cityNameTxt->setString("城池名称:");
    
    m_cityName->setFntFile("Arial_Bold_Border.fnt");
    //m_title->setString(_lang("new100003").c_str());
    string id = "";
    if(m_info.cityType == Main_City)
    {
        id = m_info.kingBuildInfo.tid;
    }else if (m_info.cityType == ResourceTile)
    {

        int  iD = (m_info.resource.tId);
        id = CC_ITOA(iD);

         
    }
    
    auto dict = LocalController::shared()->DBXMLManager()->getObjectByKey(id);
    auto cityname = dict->valueForKey("name")->getCString();
    auto resHourInfo = dict->valueForKey("resPerHour")->getCString();
    auto resMaxInfo = dict->valueForKey("resPerHour")->getCString();
    auto garrisonCap = dict->valueForKey("garrisonCap")->getCString();
    onSplitResInfo(resHourInfo,m_mapResHourInfo);
    onSplitResInfo(resMaxInfo,m_mapMaxHourInfo);

    m_cityName->setString(_lang(cityname).c_str()); 
    
    auto areaname = WorldController::getInstance()->getHomeOwnershipNameByMapPos(m_info.cityTilePoint);
    m_zoneName->setString(areaname);
    
   // m_state->setString(m_info);
   // m_foodNumTxt->setString(m_info);
   // m_woodNumTxt->setString(m_info);
   // m_stoneNumTxt->setString(m_info);
   // m_foodNumTxt->setString(m_info);
    m_zoneName->setFntFile("Arial_Bold_Border.fnt");
    //m_zoneName->setString(_lang("new100003").c_str());
   // m_zoneName->setString(MiniMapController::getInstance()->getCityPosById(m_info.cityIndex)->m_ZoneName.c_str());
    
    m_zoneNameTxt->setFntFile("Arial_Bold_Border.fnt");
    //m_zoneNameTxt->setString(_lang("new100003").c_str());
    m_zoneNameTxt->setString(_lang("new100016").c_str());
    
    m_stateTxt->setFntFile("Arial_Bold_Border.fnt");
    m_stateTxt->setString(_lang("new100018").c_str());
    // m_stateTxt->setString("占领状态:");
    
    m_state->setFntFile("Arial_Bold_Border.fnt");
    //m_state->setString(_lang("110090").c_str());
    string fullName = _lang("110090").c_str();
    ccColor3B color = ccColor3B::WHITE;
    if(m_info.playerName != "")
    {
        color = ccColor3B::RED; //红色
        //联盟
        if(m_info.allianceName!= "")
        {
            fullName = string("[")+m_info.allianceName+string("] ");

            if(GlobalData::shared()->playerInfo.isInAlliance()) //同盟
            {
                if(m_info.allianceName == GlobalData::shared()->playerInfo.allianceInfo.name)
                {
                    color = ccc3(5,171,233);
                }
            }
        }
        fullName.append(m_info.playerName.c_str());
    }
    
    
    
    m_state->setString(fullName.c_str());
    m_state->setColor(color);
    m_cityBuffTxt->setFntFile("Arial_Bold_Border.fnt");
    m_cityBuffTxt->setString(_lang("new100021").c_str());
    // m_cityBuffTxt->setString("城池加成:");
    
     m_ArmNameTxt->setFntFile("Arial_Bold_Border.fnt");
    m_ArmNameTxt->setString(_lang("new100017").c_str());
    // m_ArmNameTxt->setString("驻守数量:");

    m_armNum->setFntFile("Arial_Bold_Border.fnt");
    //m_caijiTxt->setString(_lang("new100003").c_str());
    string Armnum = string("--/") + string(garrisonCap);
    
    if(m_info.cityType == Main_City)
    {
        m_armNum->setString("--");
    }
    //加成信息
    string strRate = _lang("new100024").c_str();
    int iHeight = 0;
    int iAddH = 50;
    if(!isResTpye(3)) // 粮 3, 铁 2, 木 0 银 1
    {
        iHeight += iAddH;
        m_foodnode->setVisible(false);
    }else{
        m_foodnode->setVisible(true);
        m_foodNumTxt->setFntFile("Arial_Bold_Border.fnt");
        m_foodnode->setPositionY(m_foodnode->getPositionY() + iHeight);
        m_foodNumTxt->setString((string(CC_ITOA(isResTpye(3))+strRate).c_str()));
    }
    
    if(!isResTpye(0))
    {
        iHeight += iAddH;
        m_woodnode->setVisible(false);
    }else{
        m_woodnode->setVisible(true);
        m_woodnode->setPositionY(m_woodnode->getPositionY() + iHeight);
        m_woodNumTxt->setFntFile("Arial_Bold_Border.fnt");
        
        m_woodNumTxt->setString((string(CC_ITOA(isResTpye(0))+strRate).c_str()));
    }
    
    if(!isResTpye(1))
    {
        iHeight += iAddH;
        m_stonenode->setVisible(false);
    }else{
        m_stonenode->setVisible(true);
        m_stonenode->setPositionY(m_stonenode->getPositionY() + iHeight);

        m_stoneNumTxt->setFntFile("Arial_Bold_Border.fnt");
        
        m_stoneNumTxt->setString((string(CC_ITOA(isResTpye(1))+strRate).c_str()));
    }

    
    if(!isResTpye(2))
    {
        iHeight += iAddH;
        m_ironnode->setVisible(false);
    }else{
        m_ironnode->setVisible(true);
        m_ironnode->setPositionY(m_ironnode->getPositionY() + iHeight);
        

        m_ironNumTxt->setFntFile("Arial_Bold_Border.fnt");
        
        m_ironNumTxt->setString((string(CC_ITOA(isResTpye(2))+strRate).c_str()));
    }


    
}

void TipsWithPicView::onSplitResInfo(const char* strContent, map<int,int> &mapinfo)
{
    mapinfo.clear();
    vector<string> strVec;
    CCCommonUtils::splitString(strContent, "|", strVec);
    
    vector<string>::iterator it = strVec.begin();
    for(;it != strVec.end();it++)
    {
        vector<string> strInfo;
        CCCommonUtils::splitString((*it), ";", strInfo);
        if(strInfo.size() == 2)
        {
            mapinfo[atoi(strInfo[0].c_str())] = atoi(strInfo[1].c_str());

        }
    }
    
}


int TipsWithPicView::isResTpye(int iType)  //此类型资源是否在资源点产出
{
    int iperHour = 0;
    map<int,int>::iterator it = m_mapResHourInfo.begin();
    for(;it != m_mapResHourInfo.end();it++)
    {
        if(iType == (*it).first)
        {
            iperHour  = (*it).second;
            break;
        }
    }
    return iperHour;
}

void TipsWithPicView::onEnter(){
    PopupBaseView::onEnter();
    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    this->setTouchEnabled(true);
}

void TipsWithPicView::onExit(){
     this->setTouchEnabled(false);
    PopupBaseView::onExit();
}

bool TipsWithPicView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tochNode", CCNode*, this->m_tochNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title", CCLabelIF*, m_title);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cityNameTxt", CCLabelIF*, m_cityNameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cityName", CCLabelIF*, m_cityName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_zoneNameTxt", CCLabelIF*, m_zoneNameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_zoneName", CCLabelIF*, m_zoneName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stateTxt", CCLabelIF*, m_stateTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_state", CCLabelIF*, m_state);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodNumTxt", CCLabelIF*, m_foodNumTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woodNumTxt", CCLabelIF*, m_woodNumTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stoneNumTxt", CCLabelIF*, m_stoneNumTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ironNumTxt", CCLabelIF*, m_ironNumTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cityBuffTxt", CCLabelIF*, m_cityBuffTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ArmNameTxt", CCLabelIF*, this->m_ArmNameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_armNum", CCLabelIF*, this->m_armNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buffTxt", CCLabelIF*, this->m_buffTxt);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodIcon", CCSprite*, m_foodIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woodIcon", CCSprite*, m_woodIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ironIcon", CCSprite*, m_ironIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ironIcon", CCSprite*, m_ironIcon);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_foodnode", CCNode*, this->m_foodnode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_woodnode", CCNode*, this->m_woodnode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stonenode", CCNode*, this->m_stonenode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ironnode", CCNode*, this->m_ironnode);
    
    
    return false;
}

SEL_CCControlHandler TipsWithPicView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){

    return NULL;
}


bool TipsWithPicView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
   
    return true;
}
void TipsWithPicView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(!isTouchInside(m_tochNode, pTouch)){
        this->closeSelf();
    }
}
