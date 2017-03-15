//
//  EquipMeltView.hpp
//  IF
//
//  Created by 付彬 on 15/12/7.
//
//

#ifndef EquipMeltView_hpp
#define EquipMeltView_hpp

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"
#include "ToolController.h"
#include "EquipmentInfo.h"

class EquipMeltView :
public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public PopupBaseView
{
public:
    static EquipMeltView* create(string uuid);
    EquipMeltView():m_uuid(""){};
    
private:
    bool init(string uuid);
    
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickMeltBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickSaveBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickTipBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickHelpBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void onYesMelt();
    void onLastMelt();
    void refreashData(CCObject* obj);
    void makeRandData();
    void playRandEff();
    
    CCSafeObject<CCControlButton> m_tipBtn;
    CCSafeObject<CCControlButton> m_saveBtn;
    CCSafeObject<CCControlButton> m_meltBtn;
    
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_name1Label;
    CCSafeObject<CCLabelIF> m_info1Label;
    CCSafeObject<CCLabelIF> m_num1Label;
    CCSafeObject<CCLabelIF> m_info2Label;
    CCSafeObject<CCLabelIF> m_num2Label;
    CCSafeObject<CCLabelIF> m_info3Label;
    CCSafeObject<CCLabelIF> m_num3Label;
    CCSafeObject<CCLabelIF> m_info4Label;
    CCSafeObject<CCLabelIF> m_num4Label;
    CCSafeObject<CCLabelIF> m_info5Label;
    CCSafeObject<CCLabelIF> m_num5Label;
    
    CCSafeObject<CCLabelIF> m_infoEx2Label;
    CCSafeObject<CCLabelIF> m_numEx2Label;
    CCSafeObject<CCLabelIF> m_infoEx4Label;
    CCSafeObject<CCLabelIF> m_numEx4Label;
    
    CCSafeObject<CCNode> m_l1Node;
    CCSafeObject<CCNode> m_l2Node;
    CCSafeObject<CCNode> m_l3Node;
    CCSafeObject<CCNode> m_l4Node;
    CCSafeObject<CCNode> m_l5Node;
    CCSafeObject<CCNode> m_lEx2Node;
    CCSafeObject<CCNode> m_lEx4Node;
    
    CCSafeObject<CCNode> m_freeNode;
    CCSafeObject<CCLabelIF> m_btnMsgLabel;
    CCSafeObject<CCLabelIF> m_inBtnGoldNum;
    
    CCSafeObject<CCLabelIF> m_msg1Label;
    CCSafeObject<CCLabelIF> m_msg2Label;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCNode> m_particle;
    CCSafeObject<CCNode> m_nodeEndLine;
    
    CCSafeObject<CCNode> m_propertyNode;
    
    CCSafeObject<CCNode> m_desNode;
    CCSafeObject<CCScale9Sprite> m_desbg;
    CCSafeObject<CCNode> m_desTitleNode;
    CCSafeObject<CCNode> m_disLabelNode;
    CCSafeObject<CCNode> m_disValueNode;
    CCSafeObject<CCLabelIF> m_desName;
    CCSafeObject<CCSprite> m_desLine1;
    
    CCSafeObject<CCControlButton> m_helpBtn;
    vector< pair<string, string> > m_randData;
    
    WaitInterface* m_waitInterface;
    
    void setData(string uuid);
    string m_uuid;
    string m_curHaveOnUuid;
    int m_itemId;
    int m_itemCount;
    int m_itemNextCount;
    int m_proNum;
};

#endif /* EquipMeltView_hpp */
