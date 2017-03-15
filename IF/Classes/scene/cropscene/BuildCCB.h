//
//  BuildCCB.h
//  IF
//
//  Created by fubin on 14-10-8.
//
//

#ifndef IF_BuildCCB_h
#define IF_BuildCCB_h

#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "ParticleController.h"
#include "CCLabelBatch.h"
#include "CCAniNode.h"

#define CITYBUILD_MAIN 1
#define CITYBUILD_NORMAL 2
#define CITYBUILD_SMALL 3
#define CITYBUILD_WALL 4

class BuildCCB: public CCAniNode
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
{
public:
    BuildCCB():parentX(0),parentY(0),m_zOrder(0),m_signLayer(NULL),m_popLayer(NULL),m_batchNode(NULL){};
    ~BuildCCB(){};
    bool initBuildCCB(int type);
    static BuildCCB* create(int type);
    
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
    virtual void onEnter();
    virtual void onExit();
    void onBuildDelete();
    
    void setNamePos(int x, int y, CCLayer* sginLayer, CCSpriteBatchNode* batchNode, int zOrder);
    
protected:
    int parentX;
    int parentY;
    int m_zOrder;
    
    CCSafeObject<CCLayer> m_signLayer;
    CCSafeObject<CCLayer> m_popLayer;
    CCSafeObject<CCSpriteBatchNode> m_batchNode;
    
    CCSafeObject<CCNode> m_mainNode;
    
    CCSafeObject<CCArray> m_sprArray;
    CCSafeObject<CCArray> m_famuNodes;
    CCSafeObject<CCArray> m_datieNodes;
    
    void addParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    vector<CCParticleBatchNode*> m_parVec;
    
    void onShowParticle();
    void onHideParticle();
    void onEnterFrame(float dt);
    
private:
    
};

#endif
