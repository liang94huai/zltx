//
//  IFWingNode.h
//  IF
//
//  Created by wangdianzhen on 15/9/8.
//
//

#ifndef __IF__IFWingNode__
#define __IF__IFWingNode__

#include "CommonInclude.h"
#include "IFSkeletonNode.h"
class IFWingNode : public IFSkeletonNode
{
public:
    virtual bool init();
    CREATE_FUNC(IFWingNode);
    IFWingNode();
    virtual ~IFWingNode();
    
public:
    IFSkeletonAnimation* setSpineState(string skeName, int state, int direction, bool repeat = true, CCCallFunc* callFunc = nullptr,int loopCnt = 1);
    float getloopOneTime();
    bool isSkeExit(string skeName);
    bool isSkeTextureExit();
    std::string getAtlasPath();
    std::string getJsonPath(std::string jsonName);
private:
    void animationStateEvent(int trackIndex, int loopCount);
    void onGameTick(float time);
    void onDelayFun();
private:
    string m_skeName;
    string m_aniName;
    int m_state;
    int m_loopCnt;
    spTrackEntry* m_entry;
    double m_nowTime;
};

#endif /* defined(__IF__IFWingNode__) */
