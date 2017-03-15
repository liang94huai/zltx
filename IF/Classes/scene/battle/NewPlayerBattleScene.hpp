//
//  NewPlayerBattleScene.hpp
//  IF
//
//  Created by 付彬 on 15/11/23.
//
//

#ifndef NewPlayerBattleScene_hpp
#define NewPlayerBattleScene_hpp

#include "CommonInclude.h"
#include "HFViewport.h"
#include "BattleArmy.h"
#include "IFSkeletonAnimation.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;


class NewPlayerBattleScene :public CCLayer
,public ITouchDelegate
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
{
public:
    NewPlayerBattleScene():m_bmIdx(0){};
    ~NewPlayerBattleScene(){};
    CREATE_FUNC(NewPlayerBattleScene);
    
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
    void destory();
protected:
    bool init();
    void initBattle();
    void onEnter();
    void onExit();
    
    void goBack(CCObject* obj);
    void goBack(float _time);
    void guidSoldier(float _time);
    
    void noticeToGen(CCObject* obj);
    void noticeToGen(float _time);
    void addCloud(float _time);
    void moveScreen1(float _time);
    void moveScreen11(float _time);
    void moveScreen12(float _time);
    void atBattleAttackEnd(float _time);
    
    int m_sIndex;
    float m_countTime;
    
    void aniToBegin3(CCObject* obj);
    void moveScreen2(float _time);
    void startAttackHei(float _time);
    void showVectory(float _time);
    void moveScreen3(float _time);
    void endToBegin3(float _time);
    void playShake(float _time);
private:
    void addWarParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    void onShowWarParticle();
    void onHideWarParticle();
    
    void removeSelf(float _time);
    void removeSelf(CCObject* obj);
    
    void startPlayChe(float _time);
    void playBigFireBall(float _time);
    void playEndBigBall(CCObject* obj);
    void playEndBigBall1(CCObject* obj);
    void skipBegin1(CCObject* obj);
    void endBegin1(float t);
    void addCoverLayer(float t);
    CCSafeObject<CCSpriteBatchNode> m_soldierNode;
    
    //    CCSafeObject<CCNode> m_mainNode;
    
    int m_startIndex;
    
    CCSafeObject<CCArray> m_attackChe;
    CCSafeObject<CCArray> m_atkCheNodes;
    
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_arrAttackers, ArrAttackers);
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_attGenerals, AttGenerals);
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_arrDefenders, ArrDefenders);
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_defGenerals, DefGenerals);
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_sequenceList, SequenceList);
    
    CC_SYNTHESIZE_SAFEOBJECT(CCLayer, m_spainLayer, SpainLayer);
    CC_SYNTHESIZE_SAFEOBJECT(CCSpriteBatchNode, m_jianNode, JianNode);
    CC_SYNTHESIZE_SAFEOBJECT(CCSpriteBatchNode, m_bloodNode, BloodNode);
    
    CCSafeObject<CCLayer> m_warParLayer;
    CCSafeObject<CCLayer> m_solParLayer;
    
    CCSafeObject<CCNode> m_gateNode;
    CCSafeObject<CCNode> m_wallDesAni;
    
    CCSafeObject<CCArray> m_warParNodes;
    CCSafeObject<CCArray> m_warSFiresNodes;
    CCSafeObject<CCArray> m_wolfSmokeNodes;
    
    vector<CCParticleBatchNode*> m_parVec;
    vector<CCParticleBatchNode*> m_warParVec;
    CCSafeObject<CCParticleBatchNode> m_guideParticleNode;
    CCSafeObject<LayerColor> m_colorLayer;
    
    IFSkeletonAnimation* m_mainAnimation;
    IFSkeletonAnimation* m_wallAnimation;
    
    int m_bmIdx;
};

#endif /* NewPlayerBattleScene_hpp */
