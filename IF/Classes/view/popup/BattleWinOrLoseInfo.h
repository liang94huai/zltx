//
//  BattleWinOrLoseInfo.h
//  IF
//
//  Created by 〜TIAN~ on 17/1/3.
//
//

#ifndef __IF__BattleWinOrLoseInfo__
#define __IF__BattleWinOrLoseInfo__

#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "WorldController.h"

class BattleWinOrLoseInfo : public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner

{
public:
    
    static int m_isExit;
    static BattleWinOrLoseInfo* create(int iType,MarchInfo marchInfo);
    BattleWinOrLoseInfo(int iType,MarchInfo marchInfo) : m_type(iType),m_marchInfo(marchInfo){};
    virtual ~BattleWinOrLoseInfo(){};
    void setData(int iType);
    
private:
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    void closeBtnOk(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent);
    void initParticle(bool bWin);
    
    CCSafeObject<CCSprite>  m_title;
    CCSafeObject<CCLabelIF> m_tipLab;
    CCSafeObject<CCLabelIF> m_infoLab;
    CCSafeObject<CCLabelIF> m_returnLab;
      
    CCSafeObject<CCNode>    m_LoseDizi;
    CCSafeObject<CCNode>    m_winDizi;
    
    CCSafeObject<CCNode>    m_tochNode;
    CCSafeObject<Node>      m_particle_1;
    CCSafeObject<Node>      m_particle_2;
    
    int m_type;
    MarchInfo m_marchInfo;
private :
    void initLabInfo();
    
};

#endif /* defined(__IF__BattleWinOrLoseInfo__) */
