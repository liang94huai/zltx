//
//  IFCommonParNode.hpp
//  IF
//
//  Created by wangdianzhen on 16/1/4.
//
//

#ifndef IFCommonParNode_hpp
#define IFCommonParNode_hpp

#include "CommonInclude.h"
#include "IFSkeletonNode.h"
class IFCommonParNode : public IFSkeletonNode
{
public:
    virtual bool init();
    CREATE_FUNC(IFCommonParNode);
    IFCommonParNode();
    virtual ~IFCommonParNode();
    
public:
    IFSkeletonAnimation* setSpineState(string skeName, int state, int direction, bool repeat = true, CCCallFunc* callFunc = nullptr,int loopCnt = 1);
    float getloopOneTime();
    bool isSkeExit(string skeName);
    bool isSkeTextureExit();
    std::string getAtlasPath();
    std::string getJsonPath(std::string jsonName);
    std::string getAnimationName();
private:
    void animationStateEvent(int trackIndex, int loopCount);
private:
    string m_skeName;
    string m_aniName;
    int m_state;
    int m_loopCnt;
    spTrackEntry* m_entry;
};

#endif /* IFCommonParNode_hpp */
