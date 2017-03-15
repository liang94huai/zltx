//
//  ComposeView.cpp
//  IF
//
//  Created by 李朝辉 on 15-7-28.
//
//
#include "MatetialAni.h"
#include "ComposeAni.h"
#include "ComposeView.h"
#include "PopupViewController.h"
#include "RewardController.h"
#include "ToolController.h"
#include "YesNoDialog.h"
#include "ParticleController.h"
#include "StoreBagView.h"

ComposeView *ComposeView::create(int itemId){
    ComposeView *ret = new ComposeView(itemId);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ComposeView::onEnter(){
    PopupBaseView::onEnter();
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
	CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ComposeView::successCall), MSG_TOOL_FUSE, NULL);
}

void ComposeView::onExit(){
    setTouchEnabled(false);
    //CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TOOL_FUSE);
    PopupBaseView::onExit();
}

bool ComposeView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
if (!isTouchInside(m_bg, pTouch)) {
        return true;
    }
    int intcode = 0;
    vector<CCSprite*>::iterator pptIter = m_PicVect.begin();
    for ( pptIter = m_PicVect.begin(); pptIter != m_PicVect.end(); pptIter++) {
        if  (isTouchInside(dynamic_cast<CCNode*>((*pptIter)), pTouch)) {
            intcode = pptIter - m_PicVect.begin() +1;
            break;
        }
    }
    m_tip = NULL;
    if(InfoMat.size()>= intcode && intcode != 0){
        m_tip = TouchTipView::create(InfoMat[intcode-1]);
        if(m_tip == NULL){
            return true;
        }
        m_tip->setPosition((*pptIter)->getPositionX()-m_tip->m_bgSize.width/2, (*pptIter)->getPositionY()+ (*pptIter)->getContentSize().height+m_tip->m_bgSize.height);
        if(m_tipLocNode != NULL){
            m_tipLocNode->removeAllChildrenWithCleanup(true);
        }
        m_tipLocNode->addChild(m_tip);
        m_tip->setVisible(true);
    }
    return true;
}

void ComposeView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(m_tip != NULL){
        m_tip->setVisible(false);
        m_tip->removeAllChildrenWithCleanup(true);
    }
    vector<CCSprite*>::iterator pptIter = m_PicVect.begin();
    for ( pptIter = m_PicVect.begin(); pptIter != m_PicVect.end(); pptIter++) {
        (*pptIter)->setVisible(true);
    }
    if(!isTouchInside(m_bg, pTouch)){
        PopupViewController::getInstance()->removePopupView(this);
    }
}

bool ComposeView::init(){
    bool ret = false;
    if(PopupBaseView::init()){
        CCLoadSprite::doResourceByCommonIndex(11, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(11, false);
        });
        setIsHDPanel(true);
        CCBLoadFile("ComposeMaterial", this, this);
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        /////TODO
        
        DataClear();
//        string param1 = CCCommonUtils::getPropById(itemString, "para1");
//        string param2 = CCCommonUtils::getPropById(itemString, "para2");
//        string param3 = CCCommonUtils::getPropById(itemString, "para3");
//        itemIndex = 200414;
        ToolInfo& infoo = ToolController::getInstance()->getToolInfoById(itemIndex);
        int bg_width = m_bg->getContentSize().width;
        string param1 = infoo.para1; //合成物品Id
        string param2 = infoo.para2; //所需材料Id
        string param3 = infoo.para3; //所需材料个数Id
        if(param1 == "" || param2 == "" || param3 == ""){
            return false;
        }
        int IndexId = std::atoi(param1.c_str());
        ToolInfo& FuseInfo = ToolController::getInstance()->getToolInfoById(IndexId);//获取目标合成材料信息，设置图标、个数
        auto pic = CCLoadSprite::createSprite((FuseInfo.icon+".png").c_str());
        int TargetNumlabel = FuseInfo.getCNT();
        labeltar = CCLabelTTF::create(CC_ITOA(TargetNumlabel),"Arial",20);
        strTarNum = CC_ITOA(TargetNumlabel);
        labeltar->setPosition(CCPoint(0,-75));
        pic->setPosition(CCPoint(0,0));
        m_toCompose->addChild(pic);
        m_toCompose->addChild(labeltar);
        m_target->setString(FuseInfo.getName());
        m_targetdes->setString(_lang(FuseInfo.des));
        m_titleText->setString(_lang("137552"));
        m_materialneededText->setString(_lang("137553"));
        
        vector<std::string> pVector1;
        vector<std::string> pVector2;
        vector<std::string> PvectCNT;
        vector<std::string> PvectSprite;
        int num = GetIdVector(pVector1,pVector2,param2,param3);
        InfoMat = pVector1;
        InfoNeemNum = pVector2;
        GetNumIconVector(pVector1,PvectCNT,PvectSprite);
        CopyPicAndStrToBase(num, bg_width, pVector2, PvectCNT, PvectSprite);
        //bool ret = ToolController::getInstance()->useTool(m_itemId, 1 , true);
        CCCommonUtils::setButtonTitle(m_cancelBtn, _lang("119028").c_str());  //对应按钮实现字符串的初始化
        ret = true;
    }
    return ret;
}
int ComposeView::GetIdVector(vector<string>& pVector1,vector<string>& pVector2,string param2,string param3)
{
    int num = 1;
    if (param2.find('|') != string::npos) {
        CCCommonUtils::splitString(param2, "|", pVector1);
        CCCommonUtils::splitString(param3, "|", pVector2);
        num = pVector1.size();
    }
    else {
        CCCommonUtils::splitString(param2, ";", pVector1);
        CCCommonUtils::splitString(param3, ";", pVector2);
        int pos = find(pVector1.begin(), pVector1.end(), CC_ITOA(itemIndex))-pVector1.begin();
        pVector1.clear();
        pVector1.push_back(CC_ITOA(itemIndex));
        if(  pos>= pVector2.size() )
        { 
            return 0;
        }
        string strnum = pVector2[pos];
        pVector2.clear();
        pVector2.push_back(strnum);
        num = 1;
    }
    return num;
}
void ComposeView::GetNumIconVector(vector<string>& pVector1,vector<string>& PvectCNT,vector<string>& PvectSprite)
{
    char temp[10];
    vector<std::string>:: iterator pVect = pVector1.begin();
    for (; pVect != pVector1.end(); pVect++) {
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(std::atoi((*pVect).c_str()));
        sprintf(temp, "%d", info.getCNT());
        string str(temp);
        PvectCNT.push_back(str);
        PvectSprite.push_back(info.icon + ".png");
    }
}
void ComposeView::CopyPicAndStrToBase(int num, int bg_width, vector<string>& pVector2,vector<string>& PvectCNT, vector<string>& PvectSprite)
{
    int cnt = 0;
    if (num%2 == 0) {
        for (int i = -(num-1); i <= num-1; i+=2) {
            auto diban = CCLoadSprite::createSprite("Alliance diban.png");
            auto pic = CCLoadSprite::createSprite(PvectSprite[cnt].c_str());
            CCCommonUtils::setSpriteMaxSize(pic, 85,true);
            CCCommonUtils::setSpriteMaxSize(diban, 100,true);
            int temp = i*(bg_width/((num*2-1)*2) + pic->getContentSize().width/4);
            pic->setPosition(CCPoint(temp,35));
            m_PicVect.push_back(pic);
            diban->setPosition(CCPoint(temp,35));
            materialPos.push_back(CCPoint(temp,35));
            labelmat = CCLabelTTF::create((PvectCNT[cnt]+ "/"+pVector2[cnt]).c_str(),"Arial",20);
            labelArray.push_back(labelmat);
            strNum.push_back((PvectCNT[cnt]+ "/"+pVector2[cnt]).c_str());
            if(atoi(PvectCNT[cnt].c_str()) < atoi(pVector2[cnt].c_str()))
            {
                labelmat->setColor({255,0,0});
                bcompose = false;
            }
            labelmat->setPosition(CCPoint(temp,-30));
            strPos.push_back(CCPoint(temp,-30));
            m_materialposNode->addChild(diban);
            m_materialposNode->addChild(pic);
            m_labelNumNode->addChild(labelmat);
            cnt++;
        }
    }
    else{
        for(int i=-num/2 ;i<=num/2 ;i++){
            auto diban = CCLoadSprite::createSprite("Alliance diban.png");
            auto pic = CCLoadSprite::createSprite(PvectSprite[cnt].c_str());
            CCCommonUtils::setSpriteMaxSize(pic, 85,true);
            CCCommonUtils::setSpriteMaxSize(diban, 100,true);
            pic->setPosition(CCPoint(i*bg_width/(num+1),35));
            m_PicVect.push_back(pic);
            diban->setPosition(CCPoint(i*bg_width/(num+1),35));
            materialPos.push_back(CCPoint(i*bg_width/(num+1),35));
            labelmat = CCLabelTTF::create((PvectCNT[cnt]+ "/"+pVector2[cnt]).c_str(),"Arial",20);
            labelArray.push_back(labelmat);
            strNum.push_back((PvectCNT[cnt]+ "/"+pVector2[cnt]).c_str());
            if(atoi(PvectCNT[cnt].c_str()) < atoi(pVector2[cnt].c_str()))
            {
                labelmat->setColor({255,0,0});
                bcompose = false;
            }
            labelmat->setPosition(CCPoint(i*bg_width/(num+1),-30));
            strPos.push_back(CCPoint(i*bg_width/(num+1),-30));
            m_materialposNode->addChild(diban);
            m_materialposNode->addChild(pic);
            m_labelNumNode->addChild(labelmat);
            cnt ++;
        }
    }
}
void ComposeView::onSendClick(CCObject *pSender, CCControlEvent event){
}

void ComposeView::successCall(CCObject *param){
    //显示合成效果TODO
    CCInteger *inttemp = dynamic_cast<CCInteger*>(param);
    tarFuseNum = inttemp->getValue();
    this->AnimationPart(0);  //
    this->scheduleOnce(schedule_selector( ComposeView::ParticalShow), 0.9); //材料的延时效果显示
    this->scheduleOnce(schedule_selector( ComposeView::UpdateNumShow), 0.9); //材料的延时效果显示 数字更新
    this->scheduleOnce(schedule_selector( ComposeView::AnimationCom), 0.8); //材料的延时效果显示
    vector<CCSprite*>::iterator pptIter = m_PicVect.begin();
    for ( pptIter = m_PicVect.begin(); pptIter != m_PicVect.end(); pptIter++) {
        (*pptIter)->setVisible(true);
    }
}

void ComposeView::onFuseClick(CCObject *pSender, CCControlEvent event){
    if (bcompose == false) {
        YesNoDialog::show(_lang("137554").c_str(),NULL);
        return ;
    }
    else{
        PopupViewController::getInstance()->addPopupView(ToolNumSelectView::create(itemIndex,13));
//    ItemComposeCommand* cmd = new ItemComposeCommand(itemIndex);
//    cmd->setSuccessCallback(CCCallFuncO::create(this, callfuncO_selector(ComposeView::successCall),NULL));
//    cmd->sendAndRelease();
    }
}

bool ComposeView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleText", CCLabelIF*, this->m_titleText);  //CCB中变量与程序中变量的绑定
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_materialneededText", CCLabelIF*, this->m_materialneededText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_targetdes", CCLabelIF*, this->m_targetdes);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_target", CCLabelIF*, this->m_target);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_target", CCLabelIF*, this->m_target);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_cancelBtn", CCControlButton*, this->m_cancelBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_sendBtn", CCControlButton*, this->m_sendBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_numTxt", CCLabelIF*, this->m_numTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipTxt1", CCLabelIF*, this->m_tipTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipTxt2", CCLabelIF*, this->m_tipTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bg", CCNode*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_materialposNode", CCNode*, this->m_materialposNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_toCompose", CCNode*, this->m_toCompose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_tipLocNode", CCNode*, this->m_tipLocNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_labelNumNode", CCNode*, this->m_labelNumNode);
    return false;
}

SEL_CCControlHandler ComposeView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCancelClick", ComposeView::onFuseClick);  //CCB中按钮相应事件与函数的绑定
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSendClick", ComposeView::onSendClick);
    return NULL;
}

void ComposeView::UpdateNumShow(float _time)
{
    if(m_labelNumNode != NULL){
        m_labelNumNode->removeAllChildrenWithCleanup(true);
    }
    labelArray.clear();
    labeltar->setVisible(false);
    ToolInfo& infoo = ToolController::getInstance()->getToolInfoById(itemIndex);
    string param1 = infoo.para1; //合成物品Id
    int IndexId = std::atoi(param1.c_str());
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(IndexId);
    auto pic = CCLoadSprite::createSprite((info.icon+".png").c_str());
    pic->setPosition(CCPoint(0,0));
    m_toCompose->addChild(pic);
    labeltar = CCLabelTTF::create(CC_ITOA(info.getCNT()),"Arial",20);
    labeltar->setPosition(CCPoint(0,-75));
    labeltar->setVisible(true);
    m_toCompose->addChild(labeltar);
    vector<CCPoint>::iterator pPos = strPos.begin();
    vector<string> ::iterator pPos2 = InfoMat.begin();
    vector<string>::iterator pPos3 =InfoNeemNum.begin();
    for (; pPos!= strPos.end();pPos++,pPos2++,pPos3++ ) {
        int IndexId = std::atoi((*pPos2).c_str());
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(IndexId);
        char temp[20];
        sprintf(temp, "%d", info.getCNT());
        string str(temp);
        labelmat = CCLabelTTF::create((str+"/" +(*pPos3)).c_str(),"Arial",20);
        labelArray.push_back(labelmat);
        if(info.getCNT() < atoi((*pPos3).c_str()))
        {
            labelmat->setColor({255,0,0});
            bcompose = false;
        }
        labelmat->setPosition(*pPos);
        labelmat->setVisible(true);
        m_labelNumNode->addChild(labelmat);
    }
}

void ComposeView::ParticalShow(float _time)
{
    for (int i=1; i<9; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("CardOut_%d",i)->getCString());
        particle->setPosition(0, 0);
        m_toCompose->addChild(particle);
    }
}

void ComposeView::DataClear()
{
    strNum.clear();
    strPos.clear();
    InfoMat.clear();
    InfoNeemNum.clear();
    materialPos.clear();
    strTarNum = "";
    labelArray.clear();
    m_PicVect.clear();
}
void ComposeView::AnimationCom(float _time){
    ComposeAni* ani = ComposeAni::create();
    m_toCompose->addChild(ani);
}
void ComposeView::AnimationPart(float _time){
    if(m_tipLocNode != NULL){
        m_tipLocNode->removeAllChildrenWithCleanup(true);
    }
    vector<CCPoint> ::iterator pPos = materialPos.begin();
    for(;pPos != materialPos.end();pPos++)
    {
        MatetialAni* mat_ani = MatetialAni::create();
        mat_ani->setPosition(*pPos);
        m_tipLocNode->addChild(mat_ani);
    }
}

TouchTipView *TouchTipView::create(string itemId){
    TouchTipView *ret = new TouchTipView(itemId);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TouchTipView::init(){
    ToolInfo& infoo = ToolController::getInstance()->getToolInfoById(std::atoi(matitenId.c_str()));
//    string showTip = infoo.para1;   //要换成相关para
    string showTip = _lang(infoo.getPara().c_str()); //_lang_2("103704", CC_ITOA(34543), CC_ITOA(3453323));;
    if(showTip == ""){
        return false;
    }
    auto bg = CCLoadSprite::createScale9Sprite("ui_balloons.png");
    bg->setAnchorPoint(ccp(0, 1));
    this->addChild(bg);
    m_desText = CCLabelIF::create();
    m_desText->setColor(ccBLACK);
    m_desText->setFontSize(20);
    m_desText->setDimensions(CCSize(260, 0));  //自动分行
    m_desText->setAnchorPoint(ccp(0.5, 0.5));
    m_desText->setString(showTip);
    this->addChild(m_desText);
    bg->setPreferredSize(CCSize(m_desText->getContentSize().width * m_desText->getOriginScaleX()+10, m_desText->getContentSize().height * m_desText->getOriginScaleY() +50));
    m_desText->setPosition(bg->getContentSize().width / 2, -bg->getContentSize().height / 2 + 15);
    m_bgSize = bg->getContentSize();
    return true;
}

