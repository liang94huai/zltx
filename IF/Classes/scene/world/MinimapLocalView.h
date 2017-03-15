//
//  MinimapLocalView.h
//  IF
//
//  Created by 杜威 on 16-9-7.
//
//

#ifndef __IF__MinimapLocalView__
#define __IF__MinimapLocalView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "HFViewport.h"
#include "WorldController.h"
#include "MinimapUIView.h"
#include "MinimapViewCCB.h"


class MinimapLocalView : public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,ITouchDelegate

{
public:
    static MinimapLocalView *create(MinimapViewCCB* minimap,Rect originPos,int serverId,int mapIndex);
    MinimapLocalView(MinimapViewCCB* minimap,Rect originPos,int serverId,int mapIndex);
    ~MinimapLocalView();
    virtual void update(float time);
    void gotoCityPos(Vec2 &cityPos);
private:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    int getMapIndexWithPoint(const cocos2d::CCPoint& worldLocation);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    // ITouchDelegate
    virtual bool onBeginScroll(const cocos2d::CCPoint& worldLocation);
    virtual void onStepScroll(const cocos2d::CCPoint& worldLocation,const cocos2d::CCPoint& delta);
    virtual void onEndScroll(const cocos2d::CCPoint& worldLocation);
    virtual void onSingleTouchBegin(const cocos2d::CCPoint& worldLocation);
    virtual void onSingleTouchEnd(const cocos2d::CCPoint& worldLocation);
    
    virtual void onStepZoom(const cocos2d::CCPoint& point1, const cocos2d::CCPoint& point2);
    virtual void onBeginZoom(const CCPoint& point1,const CCPoint& point2);
    virtual void onEndZoom(float targetZoom);
    virtual void autoMoveEnd(){};
    
    Vec2 getPointByWorldPoint(Vec2 &pt);
    Vec2 getWorldPointByPoint(const Vec2 &pt);
    Vec2 getPointByTilePos(Vec2 &pt);
    
    void moveBack();
    void scaleBack();
    void addCity(int index, int type , int level=1);
    void showSelfCity();
    void addAllianceCity();
    void setUIZorder(Node* node, float iOrder);
    CCSafeObject<HFViewport> m_viewPort;
    CCSafeObject<CCNode> m_container;
    CCSafeObject<CCNode> m_miniCCBNode;
    CCSafeObject<CCNode> m_miniRenderTextureNode;

    CCSafeObject<MinimapUIView> m_mapUILayer;
    CCSafeObject<MinimapViewCCB> m_MinimapViewCCB;
    CCSafeObject<CCNode> m_mapNode;
    CCSafeObject<CCSprite> m_mapBg;
    CCSafeObject<CCSprite> m_mapSpr;
    CCSafeObject<CCNode> m_phoneIcon;
    CCSafeObject<CCNode> m_cityNode;


    bool singleTouchClose;
    int m_mapW;
    int m_mapH;
    int m_currentMapIndex;
    float m_miniScale;
    int m_currentServerId;

    Rect m_OriginRect;
    bool m_isLongPressBegan;
    float m_isLongPressTime;
    Vec2 m_LongPressPos;
    bool m_isLongPress;

//    vector<CCSprite*> m_mapSpVec;
//    vector<CCRenderTexture*> m_mapRenderTextureVec;
};
#endif /* defined(__IF__MinimapLocalView__) */
