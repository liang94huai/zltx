//
//  ScienceListView.h
//  IF
//
//  Created by fubin on 14-3-19.
//
//

#ifndef IF_ScienceListView_h
#define IF_ScienceListView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "CCClipNode.h"

// 每个科技按钮是一组动画组合
struct SBtnAniInfo
{
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCScale9Sprite> m_nameBG;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCControlButton> m_btn;
    
    CCSafeObject<CCNode> m_node;
    CCSafeObject<CCNode> m_crNode;
    
    int m_iState;   // 0未开启， 1未开放， 2使用中
    int m_iType;
};

class ScienceListView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static ScienceListView* create(int buildId);
    ScienceListView();
    virtual ~ScienceListView(){};
    
    void updateInfo();
    CCNode *getGuideNode(std::string sid);
    void playClose(int type, bool force = false);
    virtual void update(float time);
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(int buildId);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    
    void updateScienceType(int pos, string name, string pic);
    int updateBtn(int iPos);
    
    void onBtn1Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtn2Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtn3Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtn4Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtn5Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtn6Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtn7Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onBtn8Click(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClearCDClick(CCObject * pSender, Control::EventType pCCControlEvent);

    void AnimationCallback();
    void AnimationClose();
    void playBtnSmoke(float _time);
    void addParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    void closeView(CCObject* params);
    void playCircleGlow();
    void stopCircleGlow(CCObject* obj);
    void addBGPic();
    void SetBtnLock(int pos);
    
    CCSafeObject<CCLabelIF> m_desLabel;
    
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCNode> m_infoList;
    
    CCSafeObject<CCNode> m_bottomNode;
    CCSafeObject<CCNode> m_fireNode1;
    CCSafeObject<CCNode> m_fireNode2;
    CCSafeObject<CCNode> m_upNode;
    
    CCSafeObject<CCNode> m_scrollNode;
    
    CCSafeObject<CCNode> m_sceinceNode;
    CCSafeObject<CCNode> m_bgContainer;
    CCSafeObject<CCLabelIF> m_descriptionText;
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCLabelIF> m_hintText;
    CCSafeObject<CCControlButton> m_btn;
    CCSafeObject<CCScale9Sprite> m_progrossBar;
    CCSafeObject<CCScale9Sprite> m_progressBG;
    CCSafeObject<CCClipNode>  m_barClipNode;
    
    CCSafeObject<CCLayer> m_smokeLayer;
    vector<CCParticleBatchNode*> m_parVec;
    CCSafeObject<CCScrollView> m_scrollView;
    
    // 8个按钮的图片及状态
    SBtnAniInfo m_BtnInfo[8];
    int m_BtnState[8];
    
private:
    int m_buildId;
    string m_title;
    int m_clickType;
    bool m_isInit;
};

#endif
