//
//  LotteryAct2ShowView.cpp
//  IF
//
//  Created by chenfubi on 15/2/6.
//
//

#include "LotteryAct2ShowView.h"
#include "LotteryController.h"
#include "PopupViewController.h"
#include "CCMathUtils.h"
#include "CountryChatCommand.h"
#include "ChatServiceCocos2dx.h"
#include "LotteryActView.h"
#include "LotteryAct2View.h"


LotteryAct2ShowView* LotteryAct2ShowView::create()
{
    LotteryAct2ShowView* ret = new LotteryAct2ShowView();
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool LotteryAct2ShowView::init()
{
	bool bRet = false;
	do
	{
        if (!PopupBaseView::init())
        {
            break;
        }
        setIsHDPanel(true);
        CCBLoadFile("LotteryAct2ShowView", this, this);
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        this->setContentSize(winSize);
        int extH = getExtendHeight();
        m_midNode->setPositionY(m_midNode->getPositionY() + extH/2.0f);
        m_titleTTF->setString(_lang("111121"));
        initAniNode();
        CCCommonUtils::setButtonTitle(m_shareBtn, _lang("111132").c_str());
        if (GlobalData::shared()->bLotteryShareOpen)
        {
            m_shareBtn->setVisible(true);
        } else {
            m_shareBtn->setVisible(false);
            if (CCCommonUtils::isIosAndroidPad()) {
                m_titleTTF->setDimensions(Size(1100, 0));
            } else {
                m_titleTTF->setDimensions(Size(550, 0));
            }
        }
        
		bRet = true;
	}while(0);
	return bRet;
}

LotteryAct2ShowView* LotteryAct2ShowView::createWithInfoFromIOS(string info)
{
    auto ret = LotteryAct2ShowView::createWithInfo(info);
    if (ret)
    {
        ret->m_bFromIOS = true;
    }
    return ret;
}

LotteryAct2ShowView* LotteryAct2ShowView::createWithInfo(string info)
{
    LotteryAct2ShowView* ret = new LotteryAct2ShowView();
    if (ret && ret->initWithLotteryInfo(info)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool LotteryAct2ShowView::initWithLotteryInfo(string info)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    if (info.empty())
    {
        return false;
    }
    m_shareInfo = info;
    m_type = 1;
    CCLoadSprite::doResourceByCommonIndex(11, true,true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(11, false,true);
    });
    setIsHDPanel(true);
    CCBLoadFile("LotteryAct2ShowView", this, this);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    this->setContentSize(winSize);
    int extH = getExtendHeight();
    m_midNode->setPositionY(m_midNode->getPositionY() + extH/2.0f);
    m_titleTTF->setString(_lang("111121"));
    initAniNodeForShare(info);
//    m_shareBtn->setVisible(false);
    CCCommonUtils::setButtonTitle(m_shareBtn, _lang("111152").c_str());//111152=去试试运气
    m_shareBtn->setVisible(LotteryController::shared()->isLotteryOn());
    if (CCCommonUtils::isIosAndroidPad()) {
        m_titleTTF->setDimensions(Size(1100, 0));
    } else {
        m_titleTTF->setDimensions(Size(550, 0));
    }
    
    return true;
}

void LotteryAct2ShowView::initAniNodeForShare(string info)
{
    vector< vector<string> > infoVec;
    vector<string> tmpVec;
    CCCommonUtils::splitString(info, ";", tmpVec);
    for (auto it = tmpVec.begin(); it != tmpVec.end(); ++it) {
        vector<string> tmpVec2;
        CCCommonUtils::splitString(*it, ",", tmpVec2);
        if (tmpVec2.size() < 4)
        {
            return;
        }
        infoVec.push_back(tmpVec2);
    }
    
    
    CCNode* nodeB = NULL;
    CCNode* nodeC = NULL;
    CCNode* nodeC2 = NULL;
    CCNode* nodeC3 = NULL;
    CCNode* nodeC4 = NULL;
    CCSprite* bg = NULL;
    CCSprite* icon = NULL;
    CCSprite* iconBg = NULL;
    CCSprite* sIcon = NULL;
    CCLabelIF* numTTF = NULL;
    CCLabelIF* stepTTF = NULL;
    CCLabelIF* infoTTF = NULL;
    string itemId = "";
    string itemCnt = "1";
    int colorInt = 0;
    string color = "";
    vector<string> reward;
    int index = 0;
    for (int i = 0; i < infoVec.size(); i++)
    {
        int iChild  = atoi(infoVec[i][2].c_str());
        reward.clear();
        nodeB = m_aniListNode->getChildByTag(iChild);
        if (nodeB)
        {
            bg = (CCSprite*)nodeB->getChildByTag(3);
            nodeC = nodeB->getChildByTag(2);
            nodeC2 = nodeB->getChildByTag(1);
            nodeC3 = nodeB->getChildByTag(4);
            nodeC4 = nodeB->getChildByTag(5);
            icon = (CCSprite*)nodeC2->getChildByTag(1);
            numTTF = (CCLabelIF*)nodeC2->getChildByTag(2);
            iconBg = (CCSprite*)nodeC2->getChildByTag(3);
            nodeC2->removeChildByTag(4);
            
            infoTTF = (CCLabelIF*)nodeC3->getChildByTag(1);
            stepTTF = (CCLabelIF*)nodeC4->getChildByTag(1);
            
            sIcon = (CCSprite*)nodeC->getChildByTag(0);
            
            reward.push_back(infoVec[i][0]);
            reward.push_back(infoVec[i][1]);
            itemId = reward.at(0);
            itemCnt = reward.at(1);
            itemCnt = (itemCnt == "" || itemCnt == "0")?"1":itemCnt;
            nodeC2->setVisible(itemId.length());
            {
                auto layer = CCModelLayerColor::create();
                layer->setContentSize(Size(150, 150));
                layer->setColor(ccBLACK);
                layer->setOpacity(125);
                layer->setTouchEnabled(false);
                layer->setAnchorPoint(Vec2(0, 0));
                layer->setPosition(-75, -133);
                layer->setZOrder(-1);
                nodeC3->addChild(layer);
            }
            if (itemId.length() > 0)
            {
                icon->removeAllChildren();
                bg->setDisplayFrame(CCLoadSprite::loadResource("Lottery_Card1.png"));
                icon->setDisplayFrame(CCLoadSprite::loadResource(getIcon(itemId).c_str()));
                CCCommonUtils::setSpriteMaxSize(icon, 140);
                colorInt = atoi(getColor(itemId).c_str());
                color = CCCommonUtils::getToolBgByColor(colorInt);
                iconBg->setDisplayFrame(CCLoadSprite::loadResource(color.c_str()));
                numTTF->setString(itemCnt);
                int rewardstep = 0;
                rewardstep = atoi(infoVec[i][3].c_str());
                
                if(rewardstep>0){
                    nodeC3->setVisible(true);
                    nodeC4->setVisible(true);
                    infoTTF->setString(_lang("101312"));
                    std::string step = CC_ITOA(rewardstep);
                    stepTTF->setString(_lang_1("111130",step.c_str()));
                }
                int itemIdInt = atoi(itemId.c_str());
                if (200<=itemIdInt && itemIdInt<=205)
                {
                    CCLabelIFBMFont* sp = LotteryController::shared()->getLabelBMfont(itemIdInt);
                    sp->setScale(2);
                    icon->setVisible(false);
                    nodeC2->addChild(sp, 2, 4);
                }
                int index = atoi(getColor(itemId).c_str()) + 1;
                sIcon->setDisplayFrame(CCLoadSprite::loadResource(CCString::createWithFormat("Lottery_rating%d.png",index)->getCString()));
                LotteryController::addCountInfo(icon, itemId);
            }
        }
    }
}

void LotteryAct2ShowView::initAniNode()
{
    CCNode* nodeB = NULL;
    CCNode* nodeC = NULL;
    CCNode* nodeC2 = NULL;
    CCNode* nodeC3 = NULL;
    CCNode* nodeC4 = NULL;
    CCSprite* bg = NULL;
    CCSprite* icon = NULL;
    CCSprite* iconBg = NULL;
//    CCSprite* iconBg2 = NULL;
    CCSprite* sIcon = NULL;
    CCLabelIF* numTTF = NULL;
    CCLabelIF* stepTTF = NULL;
    CCLabelIF* infoTTF = NULL;
    string itemId = "";
    string itemCnt = "1";
    int colorInt = 0;
    string color = "";
    vector<string> reward;
    
    std::string posArrayStr = CCUserDefault::sharedUserDefault()->getStringForKey(LotteryAct2ShowView_PREVIEW_REWARD);
    vector<string> posArray;
    bool localData =false;
    string savePosArrayStr = "";
    if(!posArrayStr.empty()){
        CCCommonUtils::splitString(posArrayStr, ",", posArray);
        localData = true;
    }else{
        int iTmp = 0;
        while(iTmp<9){
            posArray.push_back(CC_ITOA(iTmp));
            ++iTmp;
        }
        localData=false;
    }
//     CCUserDefault::sharedUserDefault()->setStringForKey(LotteryAct2ShowView_PREVIEW_REWARD0, "");
    std::string stepArrayStr = CCUserDefault::sharedUserDefault()->getStringForKey(LotteryAct2ShowView_PREVIEW_REWARD0);
    vector<string> stepArray;
    string saveStepArrayStr = "";
    if(!stepArrayStr.empty()){
        CCCommonUtils::splitString(stepArrayStr, ",", stepArray);
    }
    if(stepArray.size()<9){
        stepArray.clear();
        int iTmp = 0;
        while(iTmp<9){
            stepArray.push_back("-1");
            ++iTmp;
        }
    }
    int index = 0;
    for (int i = 0; i < 9; i++)
    {
        int iChild  = 0;
        if(localData==false){
            int iIndex = CCMathUtils::getRandomInt(0, (posArray.size()-1)*100+99);
            iIndex = floor(iIndex/100);
            std::string indexStr = posArray.at(iIndex);
            iChild = atoi(indexStr.c_str());
            auto iter = posArray.begin();
            iter += iIndex;
            posArray.erase(iter);
            savePosArrayStr = savePosArrayStr.empty()?indexStr:savePosArrayStr+","+indexStr;
        }else{
            iChild =atoi(posArray.at(i).c_str());
        }
        reward.clear();
//        index = (m_itemIndex+i)%9;
        nodeB = m_aniListNode->getChildByTag(iChild);
        if (nodeB)
        {
            bg = (CCSprite*)nodeB->getChildByTag(3);
            nodeC = nodeB->getChildByTag(2);
            nodeC2 = nodeB->getChildByTag(1);
            nodeC3 = nodeB->getChildByTag(4);
            nodeC4 = nodeB->getChildByTag(5);
//            iconBg2 = (CCSprite*)nodeC2->getChildByTag(0);
            icon = (CCSprite*)nodeC2->getChildByTag(1);
            numTTF = (CCLabelIF*)nodeC2->getChildByTag(2);
            iconBg = (CCSprite*)nodeC2->getChildByTag(3);
            nodeC2->removeChildByTag(4);
            
            infoTTF = (CCLabelIF*)nodeC3->getChildByTag(1);
            stepTTF = (CCLabelIF*)nodeC4->getChildByTag(1);
            
            sIcon = (CCSprite*)nodeC->getChildByTag(0);
            {
                auto layer = CCModelLayerColor::create();
                layer->setContentSize(Size(150, 150));
                layer->setColor(ccBLACK);
                layer->setOpacity(125);
                layer->setTouchEnabled(false);
                layer->setAnchorPoint(Vec2(0, 0));
                layer->setPosition(-75, -133);
                layer->setZOrder(-1);
                nodeC3->addChild(layer);
            }
            LotteryController::shared()->getRewardInfo(i, reward);
            itemId = reward.at(0);
            itemCnt = reward.at(1);
            itemCnt = (itemCnt == "" || itemCnt == "0")?"1":itemCnt;
            nodeC2->setVisible(itemId.length());
            if (itemId.length() > 0)
            {
                icon->removeAllChildren();
                bg->setDisplayFrame(CCLoadSprite::loadResource("Lottery_Card1.png"));
                icon->setDisplayFrame(CCLoadSprite::loadResource(getIcon(itemId).c_str()));
                CCCommonUtils::setSpriteMaxSize(icon, 140);
                colorInt = atoi(getColor(itemId).c_str());
                color = CCCommonUtils::getToolBgByColor(colorInt);
                iconBg->setDisplayFrame(CCLoadSprite::loadResource(color.c_str()));
                numTTF->setString(itemCnt);
                int rewardstep = atoi(stepArray.at(i).c_str());
                
                if(rewardstep>-1){
                    vector<string> tmpV;
                    rewardstep = LotteryController::shared()->getStepByPos(rewardstep);
                }else{
                    vector<int> tmpIV;
                    if(LotteryController::shared()->getStep(itemId,itemCnt,tmpIV)){
                        stepArray[i] = CC_ITOA(tmpIV[0]);
                        rewardstep = tmpIV[1];
                    }
                }
                if(rewardstep>0){
                    nodeC3->setVisible(true);
                    nodeC4->setVisible(true);
                    infoTTF->setString(_lang("101312"));
                    std::string step = CC_ITOA(rewardstep);
                    stepTTF->setString(_lang_1("111130",step.c_str()));
                }
                int itemIdInt = atoi(itemId.c_str());
                if (200<=itemIdInt && itemIdInt<=205)
                {
                    CCLabelIFBMFont* sp = LotteryController::shared()->getLabelBMfont(itemIdInt);
                    sp->setScale(2);
                    icon->setVisible(false);
                    nodeC2->addChild(sp, 2, 4);
                }
                int index = atoi(getColor(itemId).c_str()) + 1;
                sIcon->setDisplayFrame(CCLoadSprite::loadResource(CCString::createWithFormat("Lottery_rating%d.png",index)->getCString()));
                LotteryController::addCountInfo(icon, itemId);
            }
        }
    }
    if(!savePosArrayStr.empty()){
        CCUserDefault::sharedUserDefault()->setStringForKey(LotteryAct2ShowView_PREVIEW_REWARD, savePosArrayStr);
    }
    auto iter1 = stepArray.begin();
    saveStepArrayStr = (*iter1);
    iter1++;
    while (iter1!=stepArray.end()) {
        saveStepArrayStr = saveStepArrayStr + ","+(*iter1);
        ++iter1;
    }
    CCUserDefault::sharedUserDefault()->setStringForKey(LotteryAct2ShowView_PREVIEW_REWARD0, saveStepArrayStr);
    CCUserDefault::sharedUserDefault()->flush();
}

void LotteryAct2ShowView::onShareBtnClick(Ref* pSender, CCControlEvent event)
{
    if(m_type == 0){
        PopupViewController::getInstance()->addPopupView(LotteryShareConfirmView::create());
    }else if(m_type == 1){
        AutoSafeRef temp(this);
        PopupViewController::getInstance()->removePopupView(this);
        if (LotteryController::shared()->lotteryInfo.type == 2)
        {
            PopupViewController::getInstance()->addPopupInView(LotteryAct2View::create());
        }
        else
        {
            PopupViewController::getInstance()->addPopupInView(LotteryActView::create());
        }
        
    }
}

void LotteryAct2ShowView::setDesNode(int index)
{
    if (index <= -1)
    {
        m_desNode->setVisible(false);
        return;
    }
    
    int position = index;
    m_desNode->setVisible(true);
    vector<string> infoVec;
    
    if (m_type == 0)
    {
        vector<std::string> infoVec1;
        std::string indexStr = CCUserDefault::sharedUserDefault()->getStringForKey(LotteryAct2ShowView_PREVIEW_REWARD);
        CCCommonUtils::splitString(indexStr, ",", infoVec1);
        auto iter = find(infoVec1.begin(), infoVec1.end(), CC_ITOA(position));
        if(iter!=infoVec1.end()){
            int iPos = iter - infoVec1.begin();
            LotteryController::shared()->getItemDes(iPos, infoVec, true);
        }
    } else {
        vector<string> lotteryInfoVec;
        map< int, vector<string> > infoMap;
        CCCommonUtils::splitString(m_shareInfo, ";", lotteryInfoVec);
        if (lotteryInfoVec.size() <= 0) {
            return;
        }
        for (int i = 0; i < lotteryInfoVec.size(); ++i)
        {
            vector<string> tVec;
            CCCommonUtils::splitString(lotteryInfoVec[i], ",", tVec);
            if (tVec.size() < 4) {
                return;
            }
            infoMap[i] = tVec;
        }
        int resultPos = -1;
        for (auto it = infoMap.begin(); it != infoMap.end(); ++it) {
            if (atoi(it->second.at(2).c_str()) == index)
            {
                resultPos = it->first;
                break;
            }
        }
        if (resultPos < 0) {
            return;
        }
        LotteryController::shared()->getItemDes(infoMap[resultPos].at(0), infoMap[resultPos].at(1), infoVec);
    }
    
    
    m_desName->setString(infoVec.at(0));
    m_desLabel->setString(infoVec.at(1));
    float extH = m_desLabel->getContentSize().height * m_desLabel->getOriginScaleY() - 80;
    if (CCCommonUtils::isIosAndroidPad())
    {
        extH = m_desLabel->getContentSize().height * m_desLabel->getOriginScaleY() - 200;
    }
    extH = extH>0?extH:0;
    m_desBg->setPreferredSize(CCSizeMake(295, 149+extH));
    if (CCCommonUtils::isIosAndroidPad()) {
        m_desBg->setPreferredSize(Size(590, 298 + extH));
    }
    CCNode* cell = NULL;
    cell = m_aniListNode->getChildByTag(index);
    CCPoint pos = cell->getPosition();
    if (index%3 == 2)
    {
        if (CCCommonUtils::isIosAndroidPad()) {
            pos = ccpAdd(pos, ccp(cell->getContentSize().width/2 - 500, 80));
        } else {
            pos = ccpAdd(pos, ccp(cell->getContentSize().width/2 - 295, 0));
        }
    }
    else
    {
        if (CCCommonUtils::isIosAndroidPad()) {
            pos = ccpAdd(pos, ccp(590/2, 80));
        } else {
            pos = ccpAdd(pos, ccp(295/2, 0));
        }
    }
    m_desNode->setPosition(pos);
}

string LotteryAct2ShowView::getIcon(string itemId)
{
    int id = atoi(itemId.c_str());
    return LotteryController::shared()->getLotteryIcon(id);
}

string LotteryAct2ShowView::getColor(string itemId)
{
    int id = atoi(itemId.c_str());
    if (id >= 200 && id <= 205)
    {
        return "5";
    }
    else if (id != R_GOODS && id < 100)
    {
        return "4";
    }
    else{
        return CCCommonUtils::getPropById(itemId, "color");
    }
}

void LotteryAct2ShowView::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void LotteryAct2ShowView::onExit()
{
    setTouchEnabled(false);
    CCNode::onExit();
}

int LotteryAct2ShowView::hasTouchNode(cocos2d::CCTouch *pTouch)
{
    CCNode* node = NULL;
    for (int i = 0; i < m_aniListNode->getChildrenCount(); i++)
    {
        node = m_aniListNode->getChildByTag(i);
        if (isTouchInside(node->getChildByTag(0), pTouch))
        {
            return i;
        }
    }
    return -1;
}

bool LotteryAct2ShowView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_aniIndex = hasTouchNode(pTouch);
    if (m_aniIndex > -1)
    {
        setDesNode(m_aniIndex);
        return true;
    }
    if (!isTouchInside(m_bg, pTouch))
    {
        return true;
    }
    return false;
}

void LotteryAct2ShowView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isTouchInside(m_bg, pTouch))
    {
        AutoSafeRef temp(this);
        PopupViewController::getInstance()->removePopupView(this);
        
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if (m_type == 1)//从聊天进来的
        {
            if (m_bFromIOS) {//从ios聊天进来的
                ChatServiceCocos2dx::showChatIOSFrom2dx();
            }
        }
#endif
        return;
    }
    m_aniIndex = -1;
    m_desNode->setVisible(false);
}

bool LotteryAct2ShowView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_aniList", CCNode*, m_aniListNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_midNode", CCNode*, m_midNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode", CCNode*, m_desNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desName", CCLabelIF*, m_desName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTTF", CCLabelIF*, m_titleTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desBg", CCScale9Sprite*, m_desBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_shareBtn", ControlButton*, m_shareBtn);
    return false;
}

SEL_CCControlHandler LotteryAct2ShowView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onShareBtnClick", LotteryAct2ShowView::onShareBtnClick);
    return NULL;
}



LotteryShareConfirmView* LotteryShareConfirmView::create()
{
    auto ret = new LotteryShareConfirmView();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool LotteryShareConfirmView::init()
{
    if (!PopupBaseView::init())
    {
        return false;
    }
    setIsHDPanel(true);
    this->setContentSize(Director::getInstance()->getWinSize());
    CCBLoadFile("LotteryShareConfirmView", this, this);
    
    if (GlobalData::shared()->lotteryShareType != 0 && GlobalData::shared()->lotteryShareType != 1 && GlobalData::shared()->lotteryShareType != 2)
    {
        return false;
    }
    m_desLabel->setString(_lang("111133"));
    m_allLabel->setString(_lang("111134"));
    m_kingLabel->setString(_lang("111135"));
    if (GlobalData::shared()->lotteryShareType == 0) {
        m_kingNode->setVisible(false);
        m_allNode->setPositionX(0);
    } else if (GlobalData::shared()->lotteryShareType == 1) {
        m_allNode->setVisible(false);
        m_kingNode->setPositionX(0);
    }
    
    return true;
}

void LotteryShareConfirmView::onEnter()
{
    Node::onEnter();
    setTouchEnabled(true);
}

void LotteryShareConfirmView::onExit()
{
    setTouchEnabled(false);
    Node::onExit();
}

bool LotteryShareConfirmView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}
void LotteryShareConfirmView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isTouchInside(m_touchNode, pTouch))
    {
        closeSelf();
    }
}

bool LotteryShareConfirmView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", Node*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allShareBtn", ControlButton*, m_allShareBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_kingShareBtn", ControlButton*, m_kingShareBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allLabel", CCLabelIF*, m_allLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_kingLabel", CCLabelIF*, m_kingLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allNode", Node*, m_allNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_kingNode", Node*, m_kingNode);
    return false;
}

SEL_CCControlHandler LotteryShareConfirmView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickAllSharBtn", LotteryShareConfirmView::onClickAllSharBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickKingShareBtn", LotteryShareConfirmView::onClickKingShareBtn);
    return nullptr;
}

void LotteryShareConfirmView::onClickAllSharBtn(Ref* pSender, Control::EventType event)
{
    
    if (GlobalData::shared()->playerInfo.isInAlliance())
    {
        string info = getLotteryInfo();
        string msg = _lang("111136");
        CountryChatCommand * cmd = new CountryChatCommand(CHAT_STATE_ALLIANCE_COMMAND,msg.c_str(),CHAT_TYPE_LOTTERY_SHARE, "", "111136", nullptr);
        cmd->putParam("lotteryInfo", __String::create(info));
        cmd->sendAndRelease();
        
        closeSelf();
    } else {
        CCCommonUtils::flyText(_lang("111138"));
        closeSelf();
    }
}

void LotteryShareConfirmView::onClickKingShareBtn(Ref* pSender, Control::EventType event)
{
    string info = getLotteryInfo();
    string msg = _lang("111136");
    CountryChatCommand * cmd = new CountryChatCommand(CHAT_STATE_COUNTRY_COMMAND,msg.c_str(),CHAT_TYPE_LOTTERY_SHARE, "", "111136", nullptr);
    cmd->putParam("lotteryInfo", __String::create(info));
    cmd->sendAndRelease();
    
    closeSelf();
}

string LotteryShareConfirmView::getLotteryInfo()
{
    vector<string> posVec;
    string posStr = UserDefault::getInstance()->getStringForKey(LotteryAct2ShowView_PREVIEW_REWARD, "");
    if (posStr.empty()) {
        return "";
    }
    CCCommonUtils::splitString(posStr, ",", posVec);
    if (posVec.empty()) {
        return "" ;
    }
    LotteryActInfo& info = LotteryController::shared()->lotteryInfo;
    if (info.rewardInfo.size() != posVec.size())
    {
        return "";
    }
    int cnt = posVec.size();
    string shareInfoStr("");
    for (int i = 0; i < cnt; ++i) {
        if (info.rewardInfo.find(i) == info.rewardInfo.end())
        {
            return "";
        }
        vector<string>& vec = info.rewardInfo[i];
        if (vec.size() < 2) {
            return "";
        }
        string mInfo("");
        mInfo.append(vec[0]).append(",").append(vec[1]).append(",");
        mInfo.append(posVec[i]).append(",");//position
        
        int sortIdx = 0;
//        for (int j = 0; j < info.rewardSort.size(); ++i) {
//            if (vec[0] == info.rewardSort[j])
//            {
//                sortIdx = j + 1;
//                break;
//            }
//        }
        
        std::string stepArrayStr = CCUserDefault::sharedUserDefault()->getStringForKey(LotteryAct2ShowView_PREVIEW_REWARD0);
        vector<string> stepArray;
        CCCommonUtils::splitString(stepArrayStr, ",", stepArray);
        int rewardstep = atoi(stepArray.at(i).c_str());
        sortIdx = LotteryController::shared()->getStepByPos(rewardstep);
        
        mInfo.append(CC_ITOA(sortIdx));//被翻开的顺序
        shareInfoStr.append(mInfo);
        shareInfoStr.append(";");
    }
    return shareInfoStr;
}


