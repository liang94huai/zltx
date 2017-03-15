//
//  DragonZhenYingView.h
//  IF
//
//  Created by ganxiaohua on 15/9/28.
//
//

#ifndef __IF__DragonZhenYingView__
#define __IF__DragonZhenYingView__

#include "CommonInclude.h"
#include "ActivityEventObj.h"
#include "CCAniNode.h"

class DragonZhenYingView :  public CCAniNode
, public CCBMemberVariableAssigner
{
public:
    static DragonZhenYingView *create();
    CCSafeObject<CCNode> m_clickNode;
private:
    DragonZhenYingView(){};
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void updateTime(float _time);
    void changeStatus();
    void onMemberDataBack(CCObject* param);
    
    CCSafeObject<CCNode> m_progressNode;
    CCSafeObject<CCLabelIF> m_leftTxt;
    CCSafeObject<CCLabelIF> m_rightTxt;
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCLabelIF> m_serverTxt;
    CCSafeObject<CCRichLabelTTF> m_chuZhang;
    CCSafeObject<CCRichLabelTTF> m_qianCheng;
    CCSafeObject<CCNode> m_txtNode;
    CCSafeObject<CCNode> m_arrowNode;
    CCSafeObject<CCNode> m_arrow;
    CCSafeObject<CCScale9Sprite> m_txtBg;
    CCSafeObject<CCNode> m_topNode;
    CCSafeObject<CCLabelIF> m_campTxt1;
    CCSafeObject<CCLabelIF> m_campTxt2;
    CCSafeObject<CCProgressTimer> m_proTimer;
    CCSafeObject<CCProgressTimer> m_rightPro;
    int m_leftTime;
    int selfTimes;
    int selfMaxTimes;
    int selfCnt;
    int enemyTimes;
    int enemyMaxTimes;
    int enemyCnt;
};

#endif /* defined(__IF__DragonZhenYingView__) */
