//
//  EquipComposeView.hpp
//  IF
//
//  Created by 付彬 on 15/12/22.
//
//

#ifndef EquipComposeView_hpp
#define EquipComposeView_hpp

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "CCTableViewTouchIFCell.h"
#include "ToolController.h"
#include "EquipmentInfo.h"

#define CHANGE_EQUIP_SEL "change.equip.select"

class EquipComposeView : public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public cocos2d::extension::CCMultiColTableViewDataSource
,public cocos2d::extension::CCMultiColTableViewDelegate
{
public:
    static EquipComposeView* create(string equipUuid);
    EquipComposeView():m_equipUuid(""),m_equipId(0){};
    
private:
    bool init(string equipUuid);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
    virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual ssize_t numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void generateData();
    void changeData(int idx);
    void showTipNode(int idx);
    
    ccColor3B getColor(int color);
    void refreashData(CCObject* obj);
    void onClickCrtBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickGotoBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickGetMore(CCObject * pSender, Control::EventType pCCControlEvent);
    void setPrcNodeTag();
    void removeWaitInter();
    void flyCrtDia();
    
    void addPopParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    void removeParticle();
    void playStartCrtParticle();
    void playEndCrtParticle();
    void playCanCrtParticle();
    
    void startMateCrt();
    void lastCompose();
    void onUpdateComposeBtn();
    
    void afterRefreshData(CCObject* p);
    void startFlyData(float dt);
    void flyEnd();
    void playPutOnSound(float dt);
    
    void onBuyTool(CCObject *ccObj);
    void onRetBuyTool(CCObject *ccObj);
    
    CCSafeObject<CCNode> m_midNode;
    CCSafeObject<CCNode> m_textFlyNode;
    CCSafeObject<CCNode> m_bottomNode;
    
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
    CCSafeObject<CCNode> m_mplace5;
    
    CCSafeObject<CCNode> m_selNode;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_flyNode;
    CCSafeObject<CCSprite> m_arrowSpr;
    CCSafeObject<CCControlButton> m_getMoreBtn;
    CCSafeObject<CCControlButton> m_gotoBtn;
    CCSafeObject<CCLabelIF> m_noMateLabel;
    CCSafeObject<CCLabelIF> m_selTitleLabel;
    
    CCSafeObject<CCNode> m_desNode;
    CCSafeObject<CCScale9Sprite> m_desbg;
    CCSafeObject<CCSprite> m_desLine1;
    CCSafeObject<CCSprite> m_desLine2;
    CCSafeObject<CCNode> m_valueNode;
    CCSafeObject<CCLabelIF> m_desName;
    CCSafeObject<CCLabelIF> m_desLv;
    CCSafeObject<CCLabelIF> m_msgLabel;
    CCSafeObject<CCLabelIF> m_desPara1;
    CCSafeObject<CCLabelIF> m_desNum1;
    CCSafeObject<CCLabelIF> m_desPara2;
    CCSafeObject<CCLabelIF> m_desNum2;
    CCSafeObject<CCLabelIF> m_desPara3;
    CCSafeObject<CCLabelIF> m_desNum3;
    CCSafeObject<CCLabelIF> m_desPara4;
    CCSafeObject<CCLabelIF> m_desNum4;
    CCSafeObject<CCLabelIF> m_desPara5;
    CCSafeObject<CCLabelIF> m_desNum5;
    
    CCSafeObject<CCSprite> m_equipLine0Spr;
    CCSafeObject<CCSprite> m_equipLine1Spr;
    CCSafeObject<CCSprite> m_equipLine2Spr;
    CCSafeObject<CCSprite> m_equipLine3Spr;
    
    CCSafeObject<CCMultiColTableView> m_tabView;
    
    void setParticle();
    void setData();
    int m_colorT;
    string m_equipUuid;
    int m_equipId;
    string m_curOptItem;
    string _tmpBGPath;
    string _tmpIconPath;
    string _tmpName;
    int m_idx;
    CCPoint startPt;
    int m_toolId;
    
    int m_needBroean;
    int m_haveBroean;
    int m_hasPutOnBroean;
    int m_color;
    string m_needMsg;
    
    vector<string> m_selData;
    vector<string> m_data;
};

//...cell class
class EquipComposeCell :
public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewCell
{
public:
    static EquipComposeCell* create(string equipUuid);
    EquipComposeCell():m_equipUUid(""),m_equipId(0){};
    void setData(string equipUuid);
    void cellTouchEnded(CCTouch* pTouch);
private:
    bool init(string equipUuid);
    virtual void onEnter();
    virtual void onExit();
    
    void onTakeOff();
    void retTakeOff(CCObject* obj);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCLabelIF> m_numLabel;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCSprite> m_onSpr;
    
    string m_equipUUid;
    int m_equipId;
};

#endif /* EquipComposeView_hpp */
