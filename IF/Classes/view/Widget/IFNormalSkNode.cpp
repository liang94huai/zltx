
#include "IFNormalSkNode.h"
#include "WorldMapView.h"
#include "DynamicResourceController.h"

bool IFNormalSkNode::init(){
    animationObj = NULL;
    return true;
}
IFNormalSkNode::IFNormalSkNode()
:m_skeName("")
,m_aniName("")
,m_entry(nullptr)
,m_nowTime(0)
{
}
IFSkeletonAnimation* IFNormalSkNode::setSpineState(string skeName, int texId, int direction, bool repeat/*=true*/,CCCallFunc* callFunc/*=nullptr*/,int loopCnt /*= 1*/)
{
    CCLOGFUNC("");
    m_skeName = skeName;
    m_texID = texId;
    m_loopCnt = loopCnt;
    m_aniName = "animation";
    m_dyPath = DynamicResourceController::getInstance()->getDownloadPath();
    if(!animationObj){
        std::string strJsonFileName = getJsonPath(m_skeName);
        std::string strAtlasFilename = getAtlasPath();
        
        if (!isSkeExit(m_skeName) || !isSkeTextureExit()){
            return nullptr;
        }
        CCLOGFUNC("");
        animationObj = new IFSkeletonAnimation(strJsonFileName.c_str(), strAtlasFilename.c_str());
        CCLOGFUNC("");
        addChild(animationObj);
    }else{
        animationObj->setToSetupPose();
    }
    CCLOGFUNC("");
    m_entry = animationObj->setAnimation(0,m_aniName.c_str(), repeat);
    CCLOGFUNC("");
    if(m_entry){
//        animationObj->timeScale = m_entry->endTime/2.0f;
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

IFSkeletonAnimation* IFNormalSkNode::setImperialSpineState(string skeName, string atlasName, int direction, bool repeat/*=true*/,CCCallFunc* callFunc/*=nullptr*/,int loopCnt /*= 1*/)
{
    CCLOGFUNC("");
    m_skeName = skeName;
    m_loopCnt = loopCnt;
    m_aniName = "animation";
    m_dyPath = DynamicResourceController::getInstance()->getDownloadPath();
    if(!animationObj){
        std::string strJsonFileName = getJsonPath(m_skeName);
        std::string strAtlasFilename = getImperialAtlasPath(atlasName);
        
        if (!isSkeExit(m_skeName) || !isImperialSkeTextureExit(atlasName)){
            return nullptr;
        }
        CCLOGFUNC("");
        animationObj = new IFSkeletonAnimation(strJsonFileName.c_str(), strAtlasFilename.c_str());
        CCLOGFUNC("");
        addChild(animationObj);
    }else{
        animationObj->setToSetupPose();
    }
    CCLOGFUNC("");
    m_entry = animationObj->setAnimation(0,m_aniName.c_str(), repeat);
    CCLOGFUNC("");
    if(m_entry){
        //        animationObj->timeScale = m_entry->endTime/2.0f;
        if(callFunc)
        {
            CC_SAFE_RELEASE_NULL(m_callBack);
            m_callBack = callFunc;
            CC_SAFE_RETAIN(m_callBack);
            animationObj->setCompleteListener ([&] (int trackIndex, int loopCount)
            {
                CC_SAFE_RETAIN(this);
                animationStateEvent(trackIndex,loopCount);
                CC_SAFE_RELEASE(this);
            });
        }
    }
    return animationObj;
}

void IFNormalSkNode::onGameTick(float time){
}
void IFNormalSkNode::onDelayFun(){
    
}

std::string IFNormalSkNode::getImperialAtlasPath(std::string atlasName)
{
    std::string atlasPath = CCString::createWithFormat("%s.atlas", atlasName.c_str())->getCString();
    return m_dyPath + atlasPath;
}

std::string IFNormalSkNode::getAtlasPath(){
    std::string  atlasPath = CCString::createWithFormat("sk_castle_%d_face.atlas", m_texID)->getCString();
    return m_dyPath + atlasPath;
}

std::string IFNormalSkNode::getJsonPath(std::string jsonName){
    std::string  jsonPath = CCString::createWithFormat("skinparticle/%s.json", jsonName.c_str())->getCString();
    return m_dyPath + jsonPath;
}

bool IFNormalSkNode::isSkeExit(string skeName){
    std::string jsonPath = CCString::createWithFormat("skinparticle/%s.json", skeName.c_str())->getCString();
    if (!CCFileUtils::sharedFileUtils()->isFileExist(m_dyPath + jsonPath)){
        return false;
    }
    if (FileUtils::getInstance()->getFileSize(m_dyPath + jsonPath) <= 0)
    {
        return false;
    }
    return true;
}

bool IFNormalSkNode::isSkeTextureExit(){
    std::string  atlasPath = CCString::createWithFormat("sk_castle_%d_face.atlas", m_texID)->getCString();
    if (!CCFileUtils::sharedFileUtils()->isFileExist(m_dyPath + atlasPath)){
        return false;
    }
    if (FileUtils::getInstance()->getFileSize(m_dyPath + atlasPath) <= 0)
    {
        return false;
    }
    return true;
    
}

bool IFNormalSkNode::isImperialSkeTextureExit(string atlasName)
{
    atlasName += ".atlas";
    if (!CCFileUtils::sharedFileUtils()->isFileExist(m_dyPath + atlasName))
    {
        return false;
    }
    if (FileUtils::getInstance()->getFileSize(m_dyPath + atlasName) <= 0) {
        return false;
    }
    return true;
}

float IFNormalSkNode::getloopOneTime(){
    if(m_entry){
        return m_entry->endTime;
    }
    return 1.5f;
}

void IFNormalSkNode::animationStateEvent(int trackIndex, int loopCount)
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

IFNormalSkNode::~IFNormalSkNode()
{
//    unschedule(schedule_selector(IFNormalSkNode::onGameTick));
    CC_SAFE_RELEASE_NULL(animationObj);
}