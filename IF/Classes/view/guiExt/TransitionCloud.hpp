//
//  TransitionCloud.hpp
//  IF
//
//  Created by 付彬 on 15/11/11.
//
//

#ifndef TransitionCloud_hpp
#define TransitionCloud_hpp

#include "CommonInclude.h"
#include "CCAniNode.h"

class TransitionCloud :
public CCAniNode
, public CCBMemberVariableAssigner
{
public:
    static TransitionCloud *create(float dt=1.0, int type=0);
    void start();
private:
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    bool init(float dt, int type);
    virtual void onEnter();
    virtual void onExit();
    void doFinishFadeIn();
    void playTransitionEnd(float t);
    void doFinish();
    void playVectoryPar(float t);
    CCSafeObject<Node> m_parNode;
    CCSafeObject<Node> m_par1Node;
    CC_SYNTHESIZE_READONLY(bool, m_isSendedFinishMsg, IsSendedFinishMsg);
    CC_SYNTHESIZE_READONLY(bool, m_isTransitionMiddleFinished, IsTransitionMiddleFinished);
    
    float m_delayTime;
    int m_type;
};

#endif /* TransitionCloud_hpp */
