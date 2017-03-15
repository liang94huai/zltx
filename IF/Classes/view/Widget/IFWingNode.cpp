//
//  IFWingNode.cpp
//  IF
//
//  Created by wangdianzhen on 15/9/8.
//
//

#include "IFWingNode.h"
#include "WorldMapView.h"
#include "DynamicResourceController.h"

bool IFWingNode::init()
{
    animationObj = NULL;
    return true;
    
    
    
    
    
}

IFWingNode::IFWingNode()
:m_skeName("")
,m_aniName("")
,m_entry(nullptr)
,m_nowTime(0)
{
}

IFSkeletonAnimation* IFWingNode::setSpineState(string skeName, int state, int direction, bool repeat/*=true*/,CCCallFunc* callFunc/*=nullptr*/,int loopCnt /*= 1*/)
{
    CCLOGFUNC("");
    m_skeName = skeName;
    m_state = state;
    m_loopCnt = loopCnt;
    switch (state)
    {
        case Rs_Spreading:
            m_aniName = "RS_Spreading";
            break;
        case Rs_Flying:
        {
            m_aniName = "RS_Flying";
            
            break;
        }
        case Rs_fadeout:
            m_aniName = "RS_fadeout";
            break;
        case Rs_fadein:
            m_aniName = "RS_fadein";
            break;
        default:
            break;
    }
    
    if(!animationObj)
    {
        std::string strJsonFileName = getJsonPath(m_skeName);
        std::string strAtlasFilename = getAtlasPath();
        
        if (!isSkeExit(m_skeName) || !isSkeTextureExit())
        {
            return nullptr;
        }
        CCLOGFUNC("");
        animationObj = new IFSkeletonAnimation(strJsonFileName.c_str(), strAtlasFilename.c_str());
        CCLOGFUNC("");
        addChild(animationObj);
    }
    else
    {
        animationObj->setToSetupPose();
    }
    CCLOGFUNC("");
    m_entry = animationObj->setAnimation(0,m_aniName.c_str(), repeat);
    CCLOGFUNC("");
    if(m_entry){
        animationObj->setTimeScale(m_entry->endTime/2.0f);
        if(callFunc)
        {
            CC_SAFE_RELEASE_NULL(m_callBack);
            m_callBack = callFunc;
            CC_SAFE_RETAIN(m_callBack);
            animationObj->setCompleteListener([&] (int trackIndex, int loopCount)
            {
                CC_SAFE_RETAIN(this);
                animationStateEvent(trackIndex,loopCount);
                CC_SAFE_RELEASE(this);
            });
        }
    }
//    if(m_state == Rs_Flying ){
//        unschedule(schedule_selector(IFWingNode::onGameTick));
//        schedule(schedule_selector(IFWingNode::onGameTick), 1.0);
//        m_nowTime = WorldController::getInstance()->getTime() / 1000;;
//    }
//    else{
//        CCDelayTime*  delayTime = CCDelayTime::create(1.0);
//        CCCallFunc* callFun = CCCallFunc::create(this, callfunc_selector(IFWingNode::onDelayFun));
//        this->runAction(CCSequence::create(delayTime,callFun,NULL));
//    }
    return animationObj;
}
void IFWingNode::onGameTick(float time){
    int passTime = WorldController::getInstance()->getTime() / 1000 - m_nowTime;
//    CCLOG("winglog   %d",passTime);
    if(passTime > 4){
        this->setSpineState(m_skeName, Rs_fadeout, 0);
    }
}
void IFWingNode::onDelayFun(){
    switch (m_state) {
        case Rs_Spreading:
            this->setSpineState(m_skeName, Rs_fadein, 0);
            break;
        case Rs_fadein:
            this->setSpineState(m_skeName, Rs_Flying, 0);
            break;
        case Rs_fadeout:
            this->setSpineState(m_skeName, Rs_fadein, 0);
            break;
            
        default:
            break;
    }
    
}
std::string IFWingNode::getAtlasPath(){

    std::string  localAtlasPath = "World/World_5.atlas";
    return localAtlasPath;
}

std::string IFWingNode::getJsonPath(std::string jsonName){

    std::string  jsonPath = CCString::createWithFormat("Spine/World/%s.json", jsonName.c_str())->getCString();
    return jsonPath;
}

bool IFWingNode::isSkeExit(string skeName)
{
        char strJsonFileName[256] = {0};
        char strAtlasFilename[256] = {0};
        sprintf(strJsonFileName, "Spine/World/%s.json", skeName.c_str());
        if (!CCFileUtils::sharedFileUtils()->isFileExist(strJsonFileName))
        {
            return false;
        }
    return true;
}

bool IFWingNode::isSkeTextureExit(){
        if (!CCFileUtils::sharedFileUtils()->isFileExist("World/World_5.atlas"))
        {
            return false;
        }
    return true;
    
}

float IFWingNode::getloopOneTime()
{
    if(m_entry)
    {
        return m_entry->endTime;
    }
    return 1.5f;
}

void IFWingNode::animationStateEvent(int trackIndex, int loopCount)
{
    spTrackEntry* entry = spAnimationState_getCurrent(animationObj->getState(), trackIndex);
    if(m_callBack && entry)
    {
        if(loopCount == m_loopCnt)
        {
            m_callBack->execute();
            CC_SAFE_RELEASE_NULL(m_callBack);
        }
    }
}

IFWingNode::~IFWingNode()
{
    unschedule(schedule_selector(IFWingNode::onGameTick));
    CC_SAFE_RELEASE_NULL(animationObj);
}