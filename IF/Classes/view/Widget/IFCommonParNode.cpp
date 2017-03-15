//
//  IFCommonParNode.cpp
//  IF
//
//  Created by wangdianzhen on 16/1/4.
//
//

#include "IFCommonParNode.hpp"
#include "WorldMapView.h"
#include "DynamicResourceController.h"

bool IFCommonParNode::init()
{
    animationObj = NULL;
    return true;
}

IFCommonParNode::IFCommonParNode()
:m_skeName("")
,m_aniName("")
,m_entry(nullptr)
{
}

IFSkeletonAnimation* IFCommonParNode::setSpineState(string skeName, int state, int direction, bool repeat/*=true*/,CCCallFunc* callFunc/*=nullptr*/,int loopCnt /*= 1*/)
{
    CCLOGFUNC("");
    m_skeName = skeName;
    m_state = state;
    m_loopCnt = loopCnt;
    
    m_aniName = getAnimationName();
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
        animationObj->setTimeScale(m_entry->endTime/getloopOneTime());
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
    return animationObj;
}
std::string IFCommonParNode::getAtlasPath(){
    
    std::string  localAtlasPath = "World/World_5.atlas";
    return localAtlasPath;
}

std::string IFCommonParNode::getJsonPath(std::string jsonName){
    
    std::string  jsonPath = CCString::createWithFormat("Spine/World/%s.json", jsonName.c_str())->getCString();
    return jsonPath;
}

bool IFCommonParNode::isSkeExit(string skeName)
{
    char strJsonFileName[256] = {0};
    sprintf(strJsonFileName, "Spine/World/%s.json", skeName.c_str());
    if (!CCFileUtils::sharedFileUtils()->isFileExist(strJsonFileName))
    {
        return false;
    }
    return true;
}

bool IFCommonParNode::isSkeTextureExit(){
    if (!CCFileUtils::sharedFileUtils()->isFileExist("World/World_5.atlas"))
    {
        return false;
    }
    return true;
    
}

float IFCommonParNode::getloopOneTime()
{
    return 8.0f;
}
std::string IFCommonParNode::getAnimationName(){
    return "animation";
}
void IFCommonParNode::animationStateEvent(int trackIndex, int loopCount)
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

IFCommonParNode::~IFCommonParNode()
{
    CC_SAFE_RELEASE_NULL(animationObj);
}