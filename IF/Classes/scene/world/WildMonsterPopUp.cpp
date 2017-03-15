//
//  WildMonsterPopUp.cpp
//  IF
//
//  Created by 李锐奇 on 14-7-29.
//
//

#include "WildMonsterPopUp.h"
#include "BattleView.h"
#include "WorldMapView.h"
#include "WorldMarchCheck.h"
WildMonsterPopUp *WildMonsterPopUp::create(WorldCityInfo &info){
    WildMonsterPopUp *ret = new WildMonsterPopUp(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool WildMonsterPopUp::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        auto bg = CCBLoadFile("MonsterAttackCCB", this, this);
        this->setContentSize(bg->getContentSize());
        
        CCLoadSprite::doResourceByCommonIndex(206, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(206, false);
            CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType_MONSTERLAYERBUST);
        });
        
        this->setModelLayerTouchCallback([&](cocos2d::CCTouch *pTouch){
            if(!isTouchInside(m_tileBg, pTouch) && (!isTouchInside(m_btn, pTouch) || !m_btn->isVisible())){
                if (!isTouchInside(m_itemTouchNode[0], pTouch) && !isTouchInside(m_itemTouchNode[1], pTouch) && !isTouchInside(m_itemTouchNode[2], pTouch) && !isTouchInside(m_itemTouchNode[3], pTouch)) {
                    this->closeSelf();
                }
            }
        });
        isChristmas = false;
        CCCommonUtils::setButtonTitle(m_btn, _lang("103701").c_str());
        string monsterId = m_info.fieldMonsterInfo.monsterId;
        
        
        std::string dialogId = CCCommonUtils::getPropById(monsterId, "name");
        string _flag = CCCommonUtils::getPropById(monsterId, "NPC");
        int leftNum = m_info.fieldMonsterInfo.christmasNum;
        if (_flag == "1"  && leftNum > 0) {
            CCCommonUtils::setButtonTitle(m_btn, _lang("150273").c_str());
            isChristmas = true;
            m_leftNumLb->setVisible(true);
            m_leftNumLb->setString(_lang_1("150272", CC_ITOA(leftNum)));//TODO:wdz
        }
        else{
            m_leftNumLb->setVisible(false);
        }
        string name  =  CCCommonUtils::getNameById(monsterId);
        std::string description = CCCommonUtils::getPropById(monsterId, "description");
        std::vector<std::string> vector;
        CCCommonUtils::splitString(description, ",", vector);
        if(description != ""){
            int index = rand() % vector.size();
            if(index >= vector.size()){
                index = vector.size() - 1;
            }
            this->m_text->setString(_lang(vector[index]));
        }
        
        m_iconNode->removeAllChildren();
        std::string icon = CCCommonUtils::getPropById(m_info.fieldMonsterInfo.monsterId, "monster") + "_bust.png";
        auto sprite  = CCLoadSprite::createSprite(icon.c_str(),true,CCLoadSpriteType_MONSTERLAYERBUST);
        if (icon != "monster_mache_1_bust.png" && icon != "monster_mache_2_bust.png")
        {
            sprite->setPosition(111,-68);
            m_txtNode->setPositionX(-124);    
        }
        m_iconNode->addChild(sprite);
        std::string showTip = _lang("103050");
        int level = atoi(CCCommonUtils::getPropById(m_info.fieldMonsterInfo.monsterId, "level").c_str());
        if (isChristmas) {
            //todoNothing
        }else{
            //杜威 关闭野怪等级限制
//            if(level > WorldController::getInstance()->currentMonsterLevel + 1){
//                std::string tip = _lang_1("103760", CC_ITOA(WorldController::getInstance()->currentMonsterLevel + 1));
//                showTip = tip + "\n" +  showTip;
//                this->m_btn->setVisible(false);
//            }
        }
        this->m_descriptionText->setString(showTip);
        if(m_btn->isVisible()){
//            this->m_btn->setPositionY(-220 - this->m_descriptionText->getContentSize().height);
//            if (CCCommonUtils::isIosAndroidPad())
//            {
//                m_btn->setPositionY(-480 - this->m_descriptionText->getContentSize().height);
//            }
        }
        if(!WorldController::isInSelfServer(m_info.tileServerId)){
            this->m_btn->setEnabled(false);
        }
        std::string itemStr = CCCommonUtils::getPropById(m_info.fieldMonsterInfo.monsterId, "reward");
        vector.clear();
        CCCommonUtils::splitString(itemStr, ",", vector);
        int index = 0;
        while (index < vector.size() && index < maxnum) {
            std::string itemId = vector[index];
            std::string itemName = CCCommonUtils::getNameById(itemId);
//            std::string iconPath = CCCommonUtils::getIcon(itemId);
//            auto icon = CCLoadSprite::createSprite(iconPath.c_str(),true,CCLoadSpriteType_GOODS);
//            CCCommonUtils::setSpriteMaxSize(icon, 108);
//            m_itemPicNode[index]->addChild(icon);
            CCCommonUtils::createGoodsIcon(atoi(itemId.c_str()), m_itemPicNode[index], Size(115,115));
            m_itemText[index]->setString(itemName);
            index++;
        }
        auto iconBg = CCLoadSprite::createSprite("tool_5.png");
        CCCommonUtils::setSpriteMaxSize(iconBg, 115,true);
        m_itemPicNode[3]->addChild(iconBg);
        auto sign = CCLoadSprite::createSprite("MonsterAttack_4.png");
        CCCommonUtils::setSpriteMaxSize(sign, 115, true);
        m_itemPicNode[3]->addChild(sign);
        m_itemText[3]->setString(_lang("105849"));

//        sign = CCLoadSprite::createSprite("icon_material.png");
//        CCCommonUtils::setSpriteMaxSize(sign, 108);
//        m_itemPicNode[0]->addChild(sign);
//        m_itemText[0]->setString(_lang("101451"));
        
        m_dropText->setString(_lang("101452"));
        std::string nameStr = CCCommonUtils::getNameById(m_info.fieldMonsterInfo.monsterId) + " LV." + CC_ITOA(level);
        m_nameAndLvText->setString(nameStr);
        ret = true;
    }
    return ret;
}

void WildMonsterPopUp::onExit(){
    WorldMapView::instance()->updateDirection();
    PopupBaseView::onExit();
}

void WildMonsterPopUp::onEnter(){
    WorldMapView::instance()->m_directionIcon->setVisible(false);
    WorldMapView::instance()->m_directionLabel->setVisible(false);
    PopupBaseView::onEnter();
}

//bool WildMonsterPopUp::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
//{
//    if(isTouchInside(this->m_hintBGNode, pTouch)){
//        m_startPoint =pTouch->getStartLocation();
//        return true;
//    }
//    return false;
//}
//
//void WildMonsterPopUp::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
//{
//    if(fabs(pTouch->getLocation().x-m_startPoint.x)>20||fabs(pTouch->getLocation().y-m_startPoint.y)>20){
//        return;
//    }
//    if (isTouchInside(m_hintBGNode,pTouch)) {
//        return;
//    }
//}

void WildMonsterPopUp::onAttackButton(CCObject * pSender, Control::EventType pCCControlEvent){
    if(!WorldController::getInstance()->canAttackMonster(m_info.fieldMonsterInfo.monsterId)){
        auto dict = CCDictionary::create();
        PopupViewController::getInstance()->addPopupInView(UseToolView::create(USE_TOOL_STAMINA,dict,"137565"));  //显示体力文本
        this->closeSelf();
        return;
    }
    if (m_info.cityType != FieldMonster) {
        CCCommonUtils::flyHint("", "", _lang("103730"));
        this->closeSelf();
        return;
    }
    unsigned int index = m_info.cityIndex;
    if(m_info.parentCityIndex != -1){
        index = m_info.parentCityIndex;
    }
    if (isChristmas) {
        
        unsigned int current = WorldController::getInstance()->getCurrentMarchCount();;
        unsigned int max = WorldController::getInstance()->getMaxMarchCount();
        if (current >= max) {
            CCCommonUtils::flyHint("", "", (_lang_1("108867", CC_ITOA((int)max))));
            return;
        }
        
        
        bool marchingAlertFlag = false;
        
        for(auto it :WorldController::getInstance()->m_marchInfo)
        {
            if (it.second.ownerType == PlayerSelf) {
                if((it.second.stateType == StateMarch && it.second.endPointIndex == m_info.cityIndex) || (it.second.stateType == StateReturn && it.second.startPointIndex == m_info.cityIndex))
                {
                    marchingAlertFlag = true;
                    break;
                }
            }
            
        }
        if (marchingAlertFlag) {
            CCCommonUtils::flyHint("", "", _lang("150307"));
            return;
        }
        
        WorldChristmasMarchCheck* cmd = new WorldChristmasMarchCheck(m_info.cityIndex,m_info.cityType);
        cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(WildMonsterPopUp::onMarch), NULL));
        cmd->sendAndRelease();
    }
    else{
        WorldController::getInstance()->openMarchDeploy(index,0);
    }
    this->closeSelf();
}
void WildMonsterPopUp::onMarch(CCObject* pObj){
    if (WorldMapView::instance()) {
        WorldMapView::instance()->attackChristmas(m_info.cityIndex, MethodChristmasMarch);
    }
//    auto cmd = new WorldMarchCommand(m_info.cityIndex,MethodChristmasMarch);
////    cmd->setCallback(CCCallFuncO::create(WorldMapView::instance(), callfuncO_selector(WorldMapView::onMarchCallback), NULL));
//    cmd->sendAndRelease();
}
bool WildMonsterPopUp::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_text", CCLabelIF*, this->m_text);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftNumLb", CCLabelIF*, this->m_leftNumLb);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn", CCControlButton*, this->m_btn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txtNode", CCNode*, this->m_txtNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemText1", CCLabelIF*, this->m_itemText[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemText2", CCLabelIF*, this->m_itemText[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemText3", CCLabelIF*, this->m_itemText[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemText4", CCLabelIF*, this->m_itemText[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_descriptionText", CCLabelIF*, this->m_descriptionText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemPicNode1", CCNode*, this->m_itemPicNode[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemPicNode2", CCNode*, this->m_itemPicNode[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemPicNode3", CCNode*, this->m_itemPicNode[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemPicNode4", CCNode*, this->m_itemPicNode[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemTouchNode1", CCNode*, this->m_itemTouchNode[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemTouchNode2", CCNode*, this->m_itemTouchNode[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemTouchNode3", CCNode*, this->m_itemTouchNode[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemTouchNode4", CCNode*, this->m_itemTouchNode[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameAndLvText", CCLabelIF*, this->m_nameAndLvText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_dropText", CCLabelIF*, this->m_dropText);

    return false;
}

SEL_CCControlHandler WildMonsterPopUp::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAttackButton", WildMonsterPopUp::onAttackButton);

    return NULL;
}
