//
//  ArcScrollView.cpp
//  IF
//
//  Created by ganxiaohua on 14-5-12.
//
//

#include "ArcScrollView.h"
#include "PopupViewController.h"
#include "CCMathUtils.h"
#include "SpriteSheetAni.h"
#include "ParticleController.h"
#include "CCGraySprite.h"

static  int radius = 400;

ArcScrollView* ArcScrollView::create(CCArray* array,int level,int pos,int star,int type){
    ArcScrollView* ret = new ArcScrollView(array,level,pos,star,type);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

ArcScrollView::~ArcScrollView(){
    m_cells->removeAllObjects();
    m_data->removeAllObjects();
    if(m_clickArea){
        m_clickArea->removeFromParentAndCleanup(true);
    }
    if(m_touchArea){
        m_touchArea->removeFromParentAndCleanup(true);
    }
}

void ArcScrollView::setShowPos(int pos){
    m_pos = pos;
    m_pos = m_data->count() - 1 - m_pos;
    if(m_pos<0) m_pos = 0;
    if(m_pos>m_data->count() - 1) m_pos = m_data->count() - 1;
    m_scrollView->getContainer()->stopAllActions();
    m_scrollView->unscheduleAllSelectors();
    CCPoint initPos;
    if(m_num%2==1){
        initPos = ccp(0, (m_num/2-m_pos)*m_cellH + m_cellH/2.0);
    }else{
        initPos = ccp(0, (m_num/2-m_pos)*m_cellH + m_cellH/2.0);
    }
    m_scrollView->setContentOffset(initPos);
}

bool ArcScrollView::init()
{
    if (!CCNode::init()) {
        return false;
    }
    
    int sizeW = 640;
    int sizeH = 852;
    if (CCCommonUtils::isIosAndroidPad())
    {
        radius = 800;
        this->setScale(1536/640);
    }
    this->setContentSize(CCSize(sizeW,sizeH));
//    CCLoadSprite::doLoadResourceAsync(COMMON_PATH, CCCallFuncO::create(this, callfuncO_selector(ArcScrollView::asyLoad), NULL), 4);
    CCLoadSprite::doResourceByCommonIndex(4, true);
    asyLoad(NULL);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(4, false);
    });
    //
//    CCNode* bg2 = CCLoadSprite::createSprite("arcScrollView_bg2.png");
//    bg2->setPosition(ccp(-283,0));
//    this->addChild(bg2,-10);
    
    m_touchArea = CCLoadSprite::createScale9Sprite("touch999999.png");
    int maxH = 600;
    m_touchArea->setContentSize(CCSize(450,maxH));
    m_touchArea->setPosition(ccp(70,0));
    m_touchArea->setOpacity(0);
    this->addChild(m_touchArea,-1000);
    
    m_num = m_data->count();
    m_cellH = 122;
    m_scrollView = ScrollTableView::create(CCSize(sizeW,MAX(m_cellH*m_num,sizeH)),m_cellH,m_num);
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_scrollView->setDelegate(this);
    m_scrollView->setTouchPriority(1);
    m_scrollView->touchArea = m_touchArea;
   
    if(m_num%2==0){
        m_scrollView->setPosition(ccp(-50 - 120, -m_cellH*(m_num+1)/2.0));
    }else{
        m_scrollView->setPosition(ccp(-50 - 120, -m_cellH*m_num/2.0));
    }
 
    if(m_level==1 || m_num<=1){
        m_scrollView->setTouchEnabled(false);
    }else{
        m_scrollView->setTouchEnabled(true);
    }
    this->addChild(m_scrollView);
    if(m_level!=1){
        m_clickArea = CCLoadSprite::createScale9Sprite("rect999999.png");
        m_clickArea->setContentSize(CCSize(350,m_cellH));
        m_clickArea->setPosition(ccp(160,0));
        m_clickArea->setOpacity(35);
        this->addChild(m_clickArea);
        m_scrollView->clickArea = m_clickArea;
        
        CCNode* cellBg = CCLoadSprite::createSprite("arrow.png");
        cellBg->setPosition(ccp(20,0));
        this->addChild(cellBg);
    }
        
    m_isMoving = false;
    m_data->reverseObjects();
    m_pos = m_data->count() - 1 - m_pos;
    if(m_pos<0) m_pos = 0;
    if(m_pos>m_data->count() - 1) m_pos = m_data->count() - 1;
    
    m_cells = CCArray::create();
    
    m_touchEnd = false;
    //this->scheduleOnce(schedule_selector(ArcScrollView::updateInfo), 0.02);
    return true;
}

void ArcScrollView::asyLoad(CCObject* p){
    int sizeW = 640;
    int sizeH = 852;
//    if (CCCommonUtils::isIosAndroidPad())
//    {
//        sizeW = 1536;
//        sizeH = 2048;
//    }
    CCScrollView* csView = CCScrollView::create(CCSize(sizeW,sizeH));
    csView->setDirection(kCCScrollViewDirectionVertical);
    csView->setPosition(ccp(-sizeW/2,-sizeH/2));
    csView->setTouchEnabled(false);
    this->addChild(csView,-1000);
    m_circle = CCLoadSprite::createSprite("arc_bg_circle.png");
    m_circle->setPosition(ccp(190+74,116));
    m_circle->setOpacity(0);
    csView->addChild(m_circle);
    if(m_level==1){
        m_circle->runAction(CCFadeIn::create(0.25));
    }else{
        m_circle->runAction(CCFadeIn::create(0.08));
    }
}

bool ArcScrollView::getAniState()
{
    if (m_scrollView) {
        return m_scrollView->m_aniStatus;
    }
    return true;
}

void ArcScrollView::moveOneCell(bool up){
    m_scrollView->moveOneCell(up);
}

bool ArcScrollView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(isTouchInside(m_touchArea, pTouch)){
        m_isInTouch = true;
    }else{
        m_isInTouch = false;
    }
    m_touchEnd = false;
    return true;
}

void ArcScrollView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    m_touchEnd = true;
}

void ArcScrollView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    m_touchEnd = false;
}

void ArcScrollView::updateInfo()
{
    m_scrollView->unscheduleAllSelectors();
    m_scrollView->getContainer()->stopAllActions();
    m_scrollView->getContainer()->removeAllChildrenWithCleanup(true);
    CCPoint circle = ccp(-283,0);
    CCPoint worldPoint = this->convertToWorldSpace(circle);//世界圆心坐标
    for(int i=0;i<m_num;i++){
        ArcInfo* info = (ArcInfo*)m_data->objectAtIndex(i);
        ArcScrollCell* cell  = ArcScrollCell::create(info,m_clickArea,worldPoint,m_level,m_starNum,m_type);
        if (m_type == 1 && i == m_pos) {
            cell->setGlowVisible(Integer::create(info->m_index));
        }
        m_scrollView->addChild(cell,i==m_pos?100:0);
        cell->setAnchorPoint(ccp(0.5, 0.5));
        m_cells->addObject(cell);
        if (m_level==1) {
            cell->setPosition(ccp(0,i*m_cellH));
            cell->setScale(0);
            CCActionInterval * scaleto = CCScaleTo ::create(0.2, 1);
            CCActionInterval * easeElasticIn= CCEaseElasticIn::create(scaleto);
            cell->runAction(easeElasticIn);
        }else{
            CCSequence* seq;
            if(i==m_pos){
                cell->setPosition(ccp(0,i*m_cellH));
                cell->setScale(0.1);
                cell->playAni(0.1);
                seq = CCSequence::create(CCScaleTo::create(0.1, 1),NULL);
            }else{
                cell->setPosition(ccp(0, m_pos*m_cellH));
                cell->setScale(0.001);
                seq = CCSequence::create(CCDelayTime::create(0.15),CCScaleTo::create(0.03, 1),CCMoveTo::create(0.15, ccp(0,i*m_cellH)),NULL);
            }
            cell->runAction(seq);
        }

    }
    CCPoint initPos;
    if(m_num%2==1){
        initPos = ccp(0, (m_num/2-m_pos)*m_cellH + m_cellH/2.0);
    }else{
        initPos = ccp(0, (m_num/2-m_pos)*m_cellH + m_cellH/2.0);
    }
    m_scrollView->setContentOffset(initPos);
    m_scrollView->initPos = initPos;
    m_scrollView->worldPoint = worldPoint;
    m_scrollView->minY = (m_num/2-(m_data->count() - 1))*m_cellH;
}

void ArcScrollView::onEnter()
{
    CCNode::onEnter();
    if(m_cells->count()<=0){
        this->updateInfo();
    }
    m_isInTouch = false;
    setSwallowsTouches(false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ArcScrollView::getMsgFun), MSG_ARC_CELL_CLICK, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ArcScrollView::touchViewFun), MSG_TOUCH_ARC_VIEW, NULL);
}

void ArcScrollView::onExit()
{
    m_isInTouch = false;
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ARC_CELL_CLICK);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TOUCH_ARC_VIEW);
    CCNode::onExit();
}

void ArcScrollView::callback() {
    if (m_callbackTarget && m_callbackFunc) {
        (m_callbackTarget->*m_callbackFunc)();
    }
}

CCNode* ArcScrollView::getShowAreaByPos(int pos){
    CCNode* showAreaNode = NULL;
    for (int i=0; i<m_cells->count(); i++) {
        ArcScrollCell* cell  = (ArcScrollCell*)m_cells->objectAtIndex(i);
        if(cell->m_info->m_index==pos){
            showAreaNode = cell->m_showArea;
            break;
        }
    }
    return showAreaNode;
}

void ArcScrollView::scrollViewDidScroll(CCScrollView* view){
    if(!m_touchEnd) return ;
    int lastIndex = m_currentIndex;
    for (int i=0; i<m_cells->count(); i++) {
        ArcScrollCell* cell  = (ArcScrollCell*)m_cells->objectAtIndex(i);
        if(cell->checkValide()){
            m_currentIndex = cell->m_info->m_index;
            if(lastIndex!=m_currentIndex){
                this->callback();
                if (m_type == 1) {
                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ARC_GLOW_VISIBLE, Integer::create(m_currentIndex));
                }
            }
            break;
        }
    }
}

void ArcScrollView::setCallback(cocos2d::CCObject *target, SEL_CallFunc func) {
    m_callbackTarget = target;
    m_callbackFunc = func;
}

void ArcScrollView::getMsgFun(CCObject* param){
    CCInteger* intObj = dynamic_cast<CCInteger*>(param);
    //CCLOG("m_type=%d",intObj->getValue());
    m_currentIndex = intObj->getValue();
    this->callback();

}

void ArcScrollView::touchViewFun(CCObject* param){
    m_isInTouch = (dynamic_cast<CCBool*>(param))->getValue();
}

ArcScrollCell* ArcScrollCell::create(ArcInfo* info,CCNode* clickNode,CCPoint worldPoint,int level,int star,int type)
{
    auto ret = new ArcScrollCell(info,clickNode,worldPoint,level,star,type);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ArcScrollCell::playAni(float delayTime){
//    auto sprite = CCLoadSprite::createSprite("arc_circle_10.png");
//    m_button->addChild(sprite);
//    SpriteSheetAni* ani = SpriteSheetAni::create("arc_circle_",1,true,5,0.06);
//    ani->setPosition(ccp(66, 66));
//    ani->play(delayTime);
//    m_button->addChild(ani);
//        
//    m_icon->setRotation(-90);
//    m_icon->setAnchorPoint(ccp(0.5, 0.5));
//    m_icon->setPosition(ccp(66, 66));
//    m_icon->setScaleX(0.2*m_maxIconScale);
//    m_icon->setOpacity(0);
//
//    CCSequence* seq1 = CCSequence::create(CCDelayTime::create(delayTime),CCFadeTo::create(0.08, 128),CCFadeTo::create(0.6, 200),CCFadeTo::create(0.3, 255),NULL);
//    CCSequence* seq2 = CCSequence::create(CCDelayTime::create(delayTime),CCRotateTo::create(0.4, 0),CCCallFunc::create(this, callfunc_selector(ArcScrollCell::playFinish)),NULL);
//    CCSequence* seq3 = CCSequence::create(CCDelayTime::create(delayTime),CCScaleTo::create(0.6, m_maxIconScale, m_maxIconScale),NULL);
//    CCSpawn* spa = CCSpawn::create(seq1,seq2,seq3,NULL);
//    m_icon->runAction(spa);
//    this->m_haveAni = true;
}

void ArcScrollCell::playFinish(){
    this->m_haveAni = false;
}

bool ArcScrollCell::checkValide(){
    //if(m_info->isLock) return false;
    bool flag = false;
    //CCPoint center = ccp(37,473);//圆心的世界坐标
    CCPoint center = m_worldPoint;//ccp(35,477);//圆心的世界坐标
    CCPoint pos = ccp(152,64);
    CCPoint p2 = m_buttonNode->convertToWorldSpace(pos);
    float value = (p2.y-center.y)/radius;
    float moveX = radius*cos(asin(value));
    CCPoint newpos = ccp(center.x+moveX,p2.y);//当前点的世界坐标
    float alphaH = center.y - newpos.y;
    if(alphaH>66 || alphaH<-66){
        flag = false;
    }else{
        flag = true;
    }

    return flag;
}

int  ArcScrollCell::getWorldGapY(){
    CCPoint center = m_worldPoint;
    CCPoint pos = ccp(152,64);
    CCPoint p2 = m_buttonNode->convertToWorldSpace(pos);
    float value = (p2.y-center.y)/radius;
    float moveX = radius*cos(asin(value));
    CCPoint newpos = ccp(center.x+moveX,p2.y);
    return abs((int)(center.y - newpos.y));
}

void ArcScrollCell::setData(ArcInfo* info,CCNode* node,CCPoint worldPoint){
    m_clickNode = node;
    m_info = info;
    m_worldPoint = worldPoint;
}

void ArcScrollCell::onEnter() {
    CCNode::onEnter();
    setSwallowsTouches(false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ArcScrollCell::setGlowVisible), MSG_ARC_GLOW_VISIBLE, NULL);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 2, false);
}

void ArcScrollCell::onExit() {
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ARC_GLOW_VISIBLE);
    CCNode::onExit();
    setTouchEnabled(false);
}


bool ArcScrollCell::init()
{
    CCBLoadFile("ArcScrollCell",this,this);
    this->setContentSize(CCSize(250,132));
    m_lockIcon = NULL;
    if(m_info->m_srcIndex!=-1){
        CCLoadSprite::doResourceByCommonIndex(m_info->m_srcIndex, true);
    }
    int sIndex = m_info->m_srcIndex;
    setCleanFunction([sIndex](){
        CCLoadSprite::doResourceByCommonIndex(4, false);
        if(sIndex!=-1){
            CCLoadSprite::doResourceByCommonIndex(sIndex, false);   
        }
    });
    bool bHasOneStar = m_starNum>=1;
    if(bHasOneStar){
        m_star->setVisible(true);
        m_lv->setVisible(true);
        m_lv->setString(CC_ITOA(m_starNum));
        m_txtBg->setScaleY(2.2);
    }else{
        m_star->setVisible(false);
        m_lv->setVisible(false);
    }
    if(m_info->m_id!=""){
        string num = m_info->m_id.substr(m_info->m_id.size()-2);
       // m_LvSpr = CCCommonUtils::getRomanSprite(atoi(num.c_str())+1);
        //m_LvSpr->setAnchorPoint(ccp(0.5, 0.5));
        //this->m_button->addChild(m_LvSpr,1002);
        //m_LvSpr->setPosition(ccp(66,22));
        auto lvStr=CCCommonUtils::getChineseLevelTxtByNum(atoi(num.c_str())+1);
        m_LvLabel = CCLabelIF::create(lvStr.c_str(), "Helvetica-Bold", 20 );
        m_LvLabel->setColor(ccc3(255,225,0));
        m_LvLabel->setPosition(ccp(66,22));
        this->m_button->addChild(m_LvLabel,1002);
    }
    if(m_info->isLock){
        m_icon = CCLoadSprite::createSprite(m_info->m_icon.c_str());
        m_lockIcon = CCLoadSprite::createSprite("iron_lock.png");
        m_lockIcon->setPosition(ccp(66,66));
        CCCommonUtils::setSpriteMaxSize(m_lockIcon, 40, true);
        m_button->addChild(m_lockIcon,1001);
        CCCommonUtils::setSpriteGray(m_lockIcon,true);
        CCCommonUtils::setSpriteGray(m_icon,true);
        if(m_LvLabel){
            m_LvLabel->setColor(ccBLACK);
        }
    }else{
//        m_icon = CCLoadSprite::createSprite(m_info->m_icon.c_str());
        m_icon = CCLoadSprite::createSprite(m_info->m_icon.c_str(), true, CCLoadSpriteType_GOODS);
    }
    if (m_info->isGrey) {
        CCCommonUtils::setSpriteGray(m_icon,true);
    }   
    m_maxIconScale = 1;
    CCCommonUtils::setSpriteMaxSize(m_icon, m_info->maxIconSize, true);
    m_maxIconScale = m_icon->getScale();
    m_icon->setPosition(ccp(66,66));
    this->m_button->addChild(m_icon,1000);
    this->m_buttonTxt->setString(m_info->m_title.c_str());
    if (CCCommonUtils::isIosAndroidPad())
    {
        this->m_buttonTxt->setDimensions(CCSize(300, 0));
    }
    m_haveAni = false;
//    if (m_type == 1) {
//        m_buttonTxt->setVisible(false);
//        m_lv->setVisible(false);
//        m_txtBg->setVisible(false);
//        m_star->setVisible(false);
//    }
//    setCleanFunction([](){
//        CCLoadSprite::doResourceByCommonIndex(4, false);
//    });
    return true;
}

void ArcScrollCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
//    float a = 0.5;
//    float r = 450;
//    float angle = asin(a)*180/PI;//角度
//    float ang = asin(a);//hu du
//    float moveX = r*sin(ang);
//    float moveY = r*cos(ang);
//    CCPoint center = ccp(0,0);
//    CCPoint pos = ccp(center.x+moveX,center.y+moveY);
    m_isDraging = true;
}

bool ArcScrollCell::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    m_isDraging = false;
    if(isTouchInside(m_button, pTouch)){
        return true;
    }
    return false;
}

void ArcScrollCell::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(m_isDraging) return ;
    if((m_level==1 && isTouchInside(m_button, pTouch))||(isTouchInside(m_button, pTouch) && isTouchInside(m_clickNode, pTouch))){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ARC_CELL_CLICK,CCInteger::create(m_info->m_index));
        if(m_level==1) return ;
        string tmpStart = "UIGlow_";
        int count = 3;
        for (int i = 1; i <= count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
            particle->setPosition(ccp(66, 66));
            m_button->addChild(particle,10000);
        }
    }
}

void ArcScrollCell::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags){
    //CCPoint center = ccp(37,473);//圆心的世界坐标
    CCPoint center = m_worldPoint;//ccp(35,477);//圆心的世界坐标
    CCPoint pos = ccp(152,64);
    CCPoint p2 = m_buttonNode->convertToWorldSpace(pos);
    float value = (p2.y-center.y)/radius;
    float moveX = radius*cos(asin(value));
    currentWorldPoint = ccp(center.x+moveX,p2.y);//当前点的世界坐标
    CCPoint local = m_buttonNode->convertToNodeSpace(currentWorldPoint);
    m_button->setPosition(ccp(local.x,local.y));
    m_selectedNode->setPosition(m_button->getPosition());
    m_txtNode->setPosition(local);
    m_showArea->setPosition(local);
    if(m_level!=1){
        float alphaH = center.y - currentWorldPoint.y;
        float alpValue;
        float scale = 0;
        if(alphaH>66 || alphaH<-66){
            float alp = abs(int(alphaH))/310.0;
            if(alp>1) alp = 1;
            alpValue= (1-alp)*255;
            scale = 1;//0.9*(1-alp*0.35);
            this->m_button->setOpacity(alpValue);
            this->m_button_2->setOpacity(alpValue);
            this->m_buttonTxt->setOpacity(alpValue);
            this->m_txtBg->setOpacity(alpValue);
            this->m_icon->setOpacity(alpValue);
            this->m_star->setOpacity(alpValue);
            this->m_lv->setOpacity(alpValue);

            if(m_lockIcon){
                this->m_lockIcon->setOpacity(alpValue);
            }
            if(this->m_LvSpr){
                this->m_LvSpr->setOpacity(alpValue);
                this->m_LvSpr->setScale(scale);
            }
            if(this->m_LvLabel){
                this->m_LvLabel->setOpacity(alpValue);
                this->m_LvLabel->setScale(scale);
            }
            this->m_button->setScale(scale);
            this->m_button_2->setOpacity(alpValue);
            this->m_buttonTxt->setScale(scale);
//            this->m_txtBg->setScale(scale);
            this->m_icon->setScale(scale*m_maxIconScale);
        }else{
            alpValue = 255;
            scale = 1;
            this->m_button->setOpacity(alpValue);
            this->m_button_2->setOpacity(alpValue);
            this->m_buttonTxt->setOpacity(alpValue);
            this->m_txtBg->setOpacity(alpValue);
            this->m_star->setOpacity(alpValue);
            this->m_lv->setOpacity(alpValue);
            if(m_lockIcon){
                this->m_lockIcon->setOpacity(alpValue);
            }
            if(this->m_LvSpr){
                this->m_LvSpr->setOpacity(255);
                this->m_LvSpr->setScale(scale);
            }
            if(this->m_LvLabel){
                this->m_LvLabel->setOpacity(255);
                this->m_LvLabel->setScale(scale);
            }
            this->m_button->setScale(scale);
            this->m_button_2->setOpacity(alpValue);
            this->m_buttonTxt->setScale(scale);
//            this->m_txtBg->setScale(scale);
            if(!m_haveAni){
                this->m_icon->setScale(scale*m_maxIconScale);
                this->m_icon->setOpacity(255);
            }
        }
    }
    CCNode::visit(renderer,parentTransform,parentFlags);
//    this->draw();
}

CCPoint ArcScrollCell::getBottomPoint(){
    CCPoint bpos = m_button->getPosition();
    CCPoint pos = m_buttonNode->convertToWorldSpace(ccp(bpos.x,bpos.y-66));
    return pos;
}

CCPoint ArcScrollCell::getUpPoint(){
    CCPoint bpos = m_button->getPosition();
    CCPoint pos = m_buttonNode->convertToWorldSpace(ccp(bpos.x,bpos.y+66));
    return pos;
}

bool ArcScrollCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buttonTxt", CCLabelIF*, this->m_buttonTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_button_2", CCSprite*, this->m_button_2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_button", CCScale9Sprite*, this->m_button);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buttonNode", CCNode*, this->m_buttonNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txtNode", CCNode*, this->m_txtNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txtBg", CCScale9Sprite*, this->m_txtBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_showArea", CCNode*, this->m_showArea);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lv", CCLabelIF*, this->m_lv);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_star", CCScale9Sprite*, this->m_star);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_selectedNode", Node*, this->m_selectedNode);
    return false;
}

void ArcScrollCell::setGlowVisible(CCObject *obj)
{
    if (m_type != 1 || m_info->isGrey) {
        return;
    }
    int index = -1;
    if(obj==NULL || (dynamic_cast<CCInteger*>(obj)==NULL)){
        index = -1;
    }else {
        CCInteger* intObj = dynamic_cast<CCInteger*>(obj);
        index = intObj->getValue();
    }
    
    if (index != m_info->m_index) {
        if (m_selectedNode->getChildByTag(1) && m_selectedNode->getChildByTag(2) && m_selectedNode->getChildByTag(3) && m_selectedNode->getChildByTag(4)) {
            m_selectedNode->getChildByTag(1)->stopAllActions();
            m_selectedNode->getChildByTag(2)->stopAllActions();
            m_selectedNode->getChildByTag(3)->stopAllActions();
            m_selectedNode->getChildByTag(4)->stopAllActions();
            m_selectedNode->setVisible(false);
        }
    }
    else {
        if (m_selectedNode->getChildByTag(1) && m_selectedNode->getChildByTag(2) && m_selectedNode->getChildByTag(3) && m_selectedNode->getChildByTag(4)) {
            m_selectedNode->setVisible(true);
            m_selectedNode->getChildByTag(1)->stopAllActions();
            m_selectedNode->getChildByTag(2)->stopAllActions();
            m_selectedNode->getChildByTag(3)->stopAllActions();
            m_selectedNode->getChildByTag(4)->stopAllActions();
            CCSequence *sequene = CCSequence::create(CCFadeTo::create(1, 102), CCFadeTo::create(1, 255), NULL);
            CCSequence *sequene1 = CCSequence::create(CCFadeTo::create(1, 102), CCFadeTo::create(1, 255), NULL);
            CCSequence *sequene2 = CCSequence::create(CCFadeTo::create(1, 102), CCFadeTo::create(1, 255), NULL);
            CCSequence *sequene3 = CCSequence::create(CCFadeTo::create(1, 102), CCFadeTo::create(1, 255), NULL);
            m_selectedNode->getChildByTag(1)->runAction(CCRepeatForever::create(sequene));
            m_selectedNode->getChildByTag(2)->runAction(CCRepeatForever::create(sequene1));
            m_selectedNode->getChildByTag(3)->runAction(CCRepeatForever::create(sequene2));
            m_selectedNode->getChildByTag(4)->runAction(CCRepeatForever::create(sequene3));
        }
    }
}
