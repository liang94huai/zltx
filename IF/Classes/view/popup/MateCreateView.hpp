//
//  MateCreateView.hpp
//  IF
//
//  Created by 付彬 on 16/1/6.
//
//

#ifndef MateCreateView_hpp
#define MateCreateView_hpp

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"
#include "ToolController.h"
#include "EquipmentInfo.h"

class MateCreateView :
public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public PopupBaseView
{
public:
    static MateCreateView* create(int itemId,int type,int count,int nextCount);
    MateCreateView():m_itemId(0),m_type(0),m_itemCount(0),m_itemNextCount(0){};
    
private:
    bool init(int itemId,int type,int count,int nextCount);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    ccColor3B getColor(int color);
    void refreashData(CCObject* obj);
    void onClickCrtBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void gotoWorkShop(float _time);//跳转材料工坊
    void setPrcNodeTag();
    void removeWaitInter();
    void flyCrtDia();
    
    void addPopParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    void removeParticle();
    void playStartCrtParticle();
    void playEndCrtParticle();
    void playCanCrtParticle();
    
    void startMateCrt();
    
    CCSafeObject<CCNode> m_midNode;
    CCSafeObject<CCNode> m_textFlyNode;
    CCSafeObject<CCNode> m_bottomNode;
    
    CCSafeObject<CCNode> m_iconNode1;
    CCSafeObject<CCNode> m_iconNode2;
    CCSafeObject<CCLabelIF> m_num1Label;
    CCSafeObject<CCLabelIF> m_num2Label;
    
    CCSafeObject<CCNode> m_flyNum[6];
    CCSafeObject<CCNode> m_mateListNode[6];
    CCSafeObject<CCNode> m_sprNode[6];
    CCSafeObject<CCLabelIF> m_num[6];
    CCSafeObject<CCSprite> m_selectSpr[6];
    CCSafeObject<CCNode> m_mateNode;
    
    CCSafeObject<CCLabelIF> m_mateName1;
    CCSafeObject<CCLabelIF> m_mateName2;
    CCSafeObject<CCLabelIF> m_mateName3;
    CCSafeObject<CCLabelIF> m_mateName4;
    CCSafeObject<CCLabelIF> m_mateName5;
    
    CCSafeObject<CCScale9Sprite> m_leftBG;
    CCSafeObject<CCScale9Sprite> m_rightBG;
    
    //    CCSafeObject<CCLabelIF> m_nameMateLabel;
    CCSafeObject<CCControlButton> m_crtBtn;
    CCSafeObject<CCNode> m_mateNode1;
    CCSafeObject<CCNode> m_mateNode2;
    CCSafeObject<CCNode> m_mateNode3;
    CCSafeObject<CCNode> m_mateNode4;
    CCSafeObject<CCNode> m_mateNode5;
    
    bool place1;
    bool place2;
    bool place3;
    bool place4;
    
    CCSafeObject<CCLayer> m_popLayer;
    CCSafeObject<CCNode> m_popNode;
    vector<CCParticleBatchNode*> m_parPopVec;
    
    CCSafeObject<CCNode> m_endCrtNode;
    CCSafeObject<CCLabelIF> m_needLabel;
    CCSafeObject<CCLabelIF> m_infoLabel;
    CCSafeObject<CCNode> m_mateBgNode5;
    
    WaitInterface* m_waitInterface;
    vector<ccColor4F> tmpStartColorVec;
    vector<ccColor4F> tmpEndColorVec;
    
    CCSafeObject<CCScale9Sprite> m_mplace1;
    CCSafeObject<CCScale9Sprite> m_mplace2;
    CCSafeObject<CCScale9Sprite> m_mplace3;
    CCSafeObject<CCScale9Sprite> m_mplace4;
    
    void setParticle();
    void setData(int itemId,int count,int nextCount);
    int m_colorT;
    int m_itemId;
    int m_itemCount;
    int m_itemNextCount;
    int m_type;
    
    vector<ToolInfo> toolVec;
    vector<EquipmentInfo> equipVec;
    map<int,int> equipCountMap;
};

#endif /* MateCreateView_hpp */
