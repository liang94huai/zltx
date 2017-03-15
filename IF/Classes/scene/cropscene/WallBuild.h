//
//  WallBuild.h
//  IF
//
//  Created by fubin on 14-7-25.
//
//

#ifndef IF_WallBuild_h
#define IF_WallBuild_h

#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "FunBuildInfo.h"
#include "ParticleController.h"
#include "Person.h"
#include "CCLabelBatch.h"
#include "CCAniNode.h"

class WallBuild: public CCAniNode
 
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
{
public:
    WallBuild():m_key(0),mainWidth(0),mainHeight(0),m_isOpen(false),parentX(0),parentY(0),isCanClick(true),m_signLayer(NULL),m_popLayer(NULL),m_backBatchNode(NULL),m_frontBatchNode(NULL),m_st(0){};
    ~WallBuild(){};
    bool initWallBuild(int type);
    static WallBuild* create(int type);
    
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void onClickThis(float _time);
    
    virtual void onEnter();
    virtual void onExit();
    void setGary(CCObject* obj);
    void setWhite(CCObject* obj);
    void playFadeOut();
    void playShadow(CCObject* obj);
    void stopShadow(CCObject* obj);
    void onBuildDelete();
    void onPlayUnlock();
    
    void canShowState();
    void setNamePos(int x, int y, CCLayer* sginLayer, CCSpriteBatchNode* popBatchNode, CCSpriteBatchNode* batchNode, int zOrder);
    int m_key;
    int mainWidth;
    int mainHeight;
    bool m_isOpen;
    
protected:
    int parentX;
    int parentY;
    bool isCanClick;
    
    CCSafeObject<CCLayer> m_signLayer;
    CCSafeObject<CCLayer> m_popLayer;
    CCSafeObject<CCSpriteBatchNode> m_backBatchNode;
    CCSafeObject<CCSpriteBatchNode> m_frontBatchNode;
    
    CCSafeObject<CCNode> m_mainNode;
    
    CCSafeObject<CCNode> m_signNode;
    CCSafeObject<CCNode> m_subNode;
    CCSafeObject<CCNode> m_upEffectNode;
    CCSafeObject<CCNode> m_fTouchNode;
    
    CCSafeObject<CCSprite> m_effectSpr;
    CCSafeObject<CCPointArray> m_ptArray;
    
    CCSafeObject<CCArray> m_backWalls;
    CCSafeObject<CCArray> m_frontWalls;
    CCSafeObject<CCArray> m_touchNodes;
    
    void drowEffectSpr(int zOrder, int tmpOrd);
    void drowPersonSpr(int zOrder, int tmpOrd);
    
    void onCanClick(float _time);
    void addParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    vector<CCParticleBatchNode*> m_parVec;
    
    void onShowParticle();
    void onHideParticle();
    void onEnterFrame(float dt);
    int m_st;
    
private:
    
};

#endif
