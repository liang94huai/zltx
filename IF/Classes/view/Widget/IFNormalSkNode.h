
#ifndef __IF__IFNormalSkNode__
#define __IF__IFNormalSkNode__

#include "CommonInclude.h"
#include "IFSkeletonNode.h"
class IFNormalSkNode : public IFSkeletonNode
{
public:
    virtual bool init();
    CREATE_FUNC(IFNormalSkNode);
    IFNormalSkNode();
    virtual ~IFNormalSkNode();
    
public:
    IFSkeletonAnimation* setSpineState(string skeName, int texID, int direction, bool repeat = true, CCCallFunc* callFunc = nullptr,int loopCnt = 1);
    IFSkeletonAnimation* setImperialSpineState(string skeName, string atlasName, int direction, bool repeat = true, CCCallFunc* callfunc = nullptr, int loopCnt = 1);
    float getloopOneTime();
    bool isSkeExit(string skeName);
    bool isSkeTextureExit();
    std::string getAtlasPath();
    std::string getJsonPath(std::string jsonName);
    
    std::string getImperialAtlasPath(std::string atlasName);
    bool isImperialSkeTextureExit(string atlasName);
private:
    void animationStateEvent(int trackIndex, int loopCount);
    void onGameTick(float time);
    void onDelayFun();
private:
    string m_skeName;
    string m_aniName;
    int m_texID;
    int m_loopCnt;
    spTrackEntry* m_entry;
    double m_nowTime;
    string m_dyPath;
};

#endif /* defined(__IF__IFNormalSkNode__) */
