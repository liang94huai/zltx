//
//  EquipInfoView.hpp
//  IF
//
//  Created by 付彬 on 16/1/6.
//
//

#ifndef EquipInfoView_hpp
#define EquipInfoView_hpp

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"
#include "EquipmentInfo.h"

class EquipInfoView :
 public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public PopupBaseView
{
public:
    static EquipInfoView* createWithEquipId(int equipId);
    static EquipInfoView* createWithBrokenEquip(int itemId,int count,int nextCount);
    static EquipInfoView* create(string uuid, bool isAni=false);
    EquipInfoView():m_uuid("")
    ,m_waitInterface(NULL){};
    
private:
    bool initWithEquipId(int equipId);
    bool init(string uuid, bool isAni);
    bool initWithBrokenEquip(int itemId,int count,int nextCount);
    
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickDestroyBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickMateBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickComposeBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void showDisassembleAni(float t);
    
    void onYesDestroy();
    void onYesDestroyLongEquip();
    void refreashData(CCObject* obj);
    CCSafeObject<CCControlButton> m_mateBtn;
    CCSafeObject<CCControlButton> m_destroyBtn;
    CCSafeObject<CCControlButton> m_composeBtn;
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
    CCSafeObject<CCLabelIF> m_desLabel;
    
    CCSafeObject<CCLabelIF> m_msg1Label;
    CCSafeObject<CCLabelIF> m_msg2Label;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCNode> m_particle;
    CCSafeObject<CCNode> m_nodeEndLine;
    
    CCSafeObject<CCNode> m_propertyNode;
    //分解
    CCSafeObject<CCNode> m_disNode;
    CCSafeObject<CCNode> m_disSubNode;
    CCSafeObject<CCLabelIF> m_disText;
    CCSafeObject<CCLabelIF> m_disCount;
    CCSafeObject<CCLabelIF> m_disX;
    
    CCSafeObject<CCNode> m_rationNode;
    
    WaitInterface* m_waitInterface;
    
    void setData(string uuid);
    void setDataByEquipId(int equipId);
    
    void playBombEff();
    void playParticle();
    
    string m_uuid;
    string m_curHaveOnUuid;
    int m_itemId;
    int m_itemCount;
    int m_itemNextCount;
    int m_type; // 1-材料  0-装备
    bool m_isAni;
};

#endif /* EquipInfoView_hpp */
