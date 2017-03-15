//
//  UserUpgradeView.h
//  IF
//
//  Created by fubin on 14-4-8.
//
//

#ifndef IF_UserUpgradeView_h
#define IF_UserUpgradeView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCMultiColTableView.h"


class UserUpgradeView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public cocos2d::extension::CCMultiColTableViewDataSource
,public cocos2d::extension::CCMultiColTableViewDelegate
{
public:
    static UserUpgradeView* create();
    UserUpgradeView();
    virtual ~UserUpgradeView();
    
protected:
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
	virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual ssize_t numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
        
    void onSkillBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onOKBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onRewardBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onShareBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void playLvAnim();
    void playLvParticle();
    
    void playGiftAnim(float _time);
    void playGiftEndAnim();
    void playGetRewardAnim();
    void shareSuccess(CCObject* param);
    void delayClose(float _time);
    void refreshView();
    void addNewPar(float t);
    void doFinishFadeIn();
//    CCSafeObject<CCMultiColTableView> m_tabView;
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_lvLabel;
    CCSafeObject<CCLabelIF> m_powLabel;
    CCSafeObject<CCLabelIF> m_skillLabel;
    CCSafeObject<CCLabelIF> m_powNumLabel;
    CCSafeObject<CCLabelIF> m_skillNumLabel;
//    CCSafeObject<CCNode>  m_rewardlist;
//    CCSafeObject<CCNode>  m_LvAnimNode;
//    CCSafeObject<CCNode>  m_hatAnimNode;
    CCSafeObject<CCNode>  m_likeNode;
    CCSafeObject<CCNode>  m_levelNode;
//    CCSafeObject<CCArray> m_cellArr;
    CCSafeObject<CCLabelIFTTF> m_fbTxt;
    CCSafeObject<CCControlButton> m_fbShareBtn;
    CCSafeObject<WaitInterface> m_waitInterFace;
    
    CCSafeObject<Node> m_picNode1;
    CCSafeObject<CCLabelIF> m_name1;
    CCSafeObject<CCLabelIF> m_num1;
    CCSafeObject<Node> m_node1;
    
    CCSafeObject<Node> m_picNode2;
    CCSafeObject<CCLabelIF> m_name2;
    CCSafeObject<CCLabelIF> m_num2;
    CCSafeObject<Node> m_node2;
    
    CCSafeObject<Node> m_picNode3;
    CCSafeObject<CCLabelIF> m_name3;
    CCSafeObject<CCLabelIF> m_num3;
    CCSafeObject<Node> m_node3;
    
    CCSafeObject<Node> m_picNode4;
    CCSafeObject<CCLabelIF> m_name4;
    CCSafeObject<CCLabelIF> m_num4;
    CCSafeObject<Node> m_node4;
    
    CCSafeObject<Node> m_picNode5;
    CCSafeObject<CCLabelIF> m_name5;
    CCSafeObject<CCLabelIF> m_num5;
    CCSafeObject<Node> m_node5;
    
    CCSafeObject<Node> m_picNode6;
    CCSafeObject<CCLabelIF> m_name6;
    CCSafeObject<CCLabelIF> m_num6;
    CCSafeObject<Node> m_node6;
    
    CCSafeObject<Node> m_parNode;

    vector<std::string> vector_item;
    vector<std::string> vector_number;
    
    vector<CCSafeObject<Node>> vector_node;
    vector<CCSafeObject<CCLabelIF>> vector_num;
    vector<CCSafeObject<CCLabelIF>> vector_name;
    vector<CCSafeObject<Node>> vector_picNode;
    
    CCSafeObject<Node> m_par_outside;
    CCSafeObject<Node> m_par_inside;
    CCSafeObject<Node> m_par_star;
    CCSafeObject<Sprite> m_dragon_effect_left;
    CCSafeObject<Sprite> m_dragon_effect_right;
    int m_falg;
    int m_endfalg;
    bool aniFinish;
};

#endif
