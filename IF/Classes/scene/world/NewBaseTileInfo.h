//
//  NewBaseTileInfo.h
//  IF
//
//  Created by 李锐奇 on 14-8-5.
//
//

#ifndef __IF__NewBaseTileInfo__
#define __IF__NewBaseTileInfo__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "WorldController.h"
#include "PopupViewController.h"
#include "UseToolView.h"
#include "ToolController.h"
#include "BaseTileInfo.h"

#define TOTAL_BUTTON_NUM 6
#define ICON_TAG 10
#define BUTTON_TAG 20
#define ICON_DEL_TAG 30

//for lua use  -- add by js
class NewBaseTileLuaInfo : public PopupBaseView
,public CCBMemberVariableAssigner
,public CCBSelectorResolver

{
public:
    CREATE_FUNC(NewBaseTileLuaInfo);
    NewBaseTileLuaInfo():m_cityInfo(NULL){
    }
    void setCityInfo(WorldCityInfo *cinfo){
        m_cityInfo = cinfo;
    }
    WorldCityInfo* getCityInfo(){
        return m_cityInfo;
    }
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual bool init();
    virtual bool initTile(bool selfInit = true, bool isMarch = false);
    
    virtual BaseTileInfo* getFavoriteView();
    
    void keyPressedBtnFavorite(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void setFavoButtonVisible(bool isVisible);
    
    void setButtonCount(unsigned int count,bool isNotMark = false);
    unsigned int getButtonCount();
    
    void setButtonName(unsigned int order,const string& name);
    void setButtonCallback(unsigned int order,CCCallFunc* callFun);
    void setButtonState(unsigned int order,int state);
    void setButtonState(CCControlButton* button,int state);
    void setButtonEnable(int index, bool enable = false);
    std::string getButtonPic(int state);
    
    CCControlButton* getButton(unsigned int order);
    CCNode* getBtnNode(unsigned int index);
    CCNode* getNode(unsigned int index);
    CCLabelIF *getNameText(unsigned int index);
    int getButtonState(unsigned int order);
    int getButtonState(CCControlButton* button);
    void setTitleString(string title);
    
    virtual void closeThis();
    void resetPos(CCPoint &pt);
    
    void closeImmediately();
    
    CC_SYNTHESIZE(bool, isInitEnd, IsInitEnd);
    void addToParent();
    void addNameNode();
protected:
    WorldCityInfo* m_cityInfo;
    CCSafeObject<CCNode> m_node1;
    CCSafeObject<CCNode> m_node2;
    CCSafeObject<CCNode> m_node3;
    CCSafeObject<CCNode> m_node4;
    CCSafeObject<CCNode> m_node5;
    CCSafeObject<CCNode> m_node6;
    CCSafeObject<CCNode> m_btnNode1;
    CCSafeObject<CCNode> m_btnNode2;
    CCSafeObject<CCNode> m_btnNode3;
    CCSafeObject<CCNode> m_btnNode4;
    CCSafeObject<CCNode> m_btnNode5;
    CCSafeObject<CCNode> m_btnNode6;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCLabelIF> m_nameText1;
    CCSafeObject<CCLabelIF> m_nameText2;
    CCSafeObject<CCLabelIF> m_nameText3;
    CCSafeObject<CCLabelIF> m_nameText4;
    CCSafeObject<CCLabelIF> m_nameText5;
    CCSafeObject<CCLabelIF> m_nameText6;
    CCSafeObject<CCLabelIFTTF> m_nameAndAlliance;
    CCSafeObject<CCNode> m_nameNode;
    CCSafeObject<CCNode> m_bgCircleNode;
    CCSafeObject<CCLabelIFTTF> m_title;
    CCSafeObject<CCLabelIF> m_tilePoint;
    CCSafeObject<CCSprite> m_faveIcon;
    CCSafeObject<CCNode> m_addNode;
    CCSafeObject<CCControlButton> m_addBtn;
    CCSafeObject<CCScale9Sprite> m_infoBG;
private:
    std::vector<CCSafeObject<CCControlButton>> m_buttonStack;
    std::vector<int> m_buttonState;
    CCSafeObject<CCNode> m_ccb;
    void animationFunc();
    void animationInFunc();
    bool isInFavoriteList();
    void setButtonCallback1(unsigned int order,SEL_CCControlHandler handler);
    void onSetMarkBtn(int count, bool isNotMark);
    void onMarkLineClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void AnimationCallback();
    void onButtonClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    
    int infoY;
    bool isClosing;
    int m_btnCnt;
    int m_openNum;
    bool m_isMarch;
    bool m_isSpeFadeIn;
    std::vector<CCCallFunc*> m_buttonCalls;
};


class NewBaseTileInfo : public PopupBaseView
,public CCBMemberVariableAssigner
,public CCBSelectorResolver

{
public:
    
    NewBaseTileInfo(WorldCityInfo& info):
    m_cityInfo(info)
    {}
    static NewBaseTileInfo* create(WorldCityInfo& info) {
        auto ret = new NewBaseTileInfo(info);
        if (ret->init(true)) {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

    bool init(bool selfInit = false, bool isMarch = false);
    
    virtual BaseTileInfo* getFavoriteView();
    
    void keyPressedBtnFavorite(CCObject * pSender, Control::EventType pCCControlEvent);
    
    void setFavoButtonVisible(bool isVisible);
    
    void setButtonCount(unsigned int count,bool isNotMark = false);
    unsigned int getButtonCount();
    
    void setButtonName(unsigned int order,const string& name);
    void setButtonCallback(unsigned int order,SEL_CCControlHandler handler);
    void setButtonState(unsigned int order,TileButtonState state);
    void setButtonState(CCControlButton* button,TileButtonState state);
    void setButtonEnable(int index, bool enable = false);
    std::string getButtonPic(TileButtonState state);
    
    CCControlButton* getButton(unsigned int order);
    CCNode* getBtnNode(unsigned int index);
    CCNode* getNode(unsigned int index);
    CCLabelIF *getNameText(unsigned int index);
    TileButtonState getButtonState(unsigned int order);
    TileButtonState getButtonState(CCControlButton* button);
    
    virtual void closeThis();
    void resetPos(CCPoint &pt);
    WorldCityInfo& m_cityInfo;
    void closeImmediately();
    
    bool isInitEnd;

protected:
    
    CCSafeObject<CCNode> m_node1;
    CCSafeObject<CCNode> m_node2;
    CCSafeObject<CCNode> m_node3;
    CCSafeObject<CCNode> m_node4;
    CCSafeObject<CCNode> m_node5;
    CCSafeObject<CCNode> m_node6;
    CCSafeObject<CCNode> m_btnNode1;
    CCSafeObject<CCNode> m_btnNode2;
    CCSafeObject<CCNode> m_btnNode3;
    CCSafeObject<CCNode> m_btnNode4;
    CCSafeObject<CCNode> m_btnNode5;
    CCSafeObject<CCNode> m_btnNode6;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCLabelIF> m_nameText1;
    CCSafeObject<CCLabelIF> m_nameText2;
    CCSafeObject<CCLabelIF> m_nameText3;
    CCSafeObject<CCLabelIF> m_nameText4;
    CCSafeObject<CCLabelIF> m_nameText5;
    CCSafeObject<CCLabelIF> m_nameText6;
    CCSafeObject<CCLabelIFTTF> m_nameAndAlliance;
    CCSafeObject<CCNode> m_nameNode;
    CCSafeObject<CCNode> m_bgCircleNode;
    CCSafeObject<CCLabelIFTTF> m_title;
    CCSafeObject<CCLabelIF> m_tilePoint;
    CCSafeObject<CCSprite> m_faveIcon;
    CCSafeObject<CCNode> m_addNode;
    CCSafeObject<CCControlButton> m_addBtn;

    CCSafeObject<CCScale9Sprite> m_infoBG;
    CCSafeObject<CCLabelIF> m_pointLabel;
    CCSafeObject<CCScale9Sprite> m_underLine;
    void addToParent();
    void addNameNode();
private:
    std::vector<CCSafeObject<CCControlButton>> m_buttonStack;
    std::vector<TileButtonState> m_buttonState;
    CCSafeObject<CCNode> m_ccb;
    void animationFunc();
    void animationInFunc();
    bool isInFavoriteList();
    
    void onSetMarkBtn(int count, bool isNotMark);
    void onMarkLineClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void AnimationCallback();
    
    int infoY;
    bool isClosing;
    int m_btnCnt;
    int m_openNum;
    bool m_isMarch;
    bool m_isSpeFadeIn;
};

#endif /* defined(__IF__NewBaseTileInfo__) */
