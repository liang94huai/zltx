//
//  MinimapViewCCB.h
//  IF
//
//  Created by 杜威 on 16-9-7.
//
//

#ifndef __IF__MinimapViewCCB__
#define __IF__MinimapViewCCB__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "HFViewport.h"
#include "WorldController.h"
#include "MinimapUIView.h"
#include "MiniMapController.h"
static Vec2 viewCenterPoint;


#define WIDTH 861*2
#define MINIMAP_WIDTH WIDTH
#define MINIMAP_HEIGHT WIDTH/2

#define MINIMAP_HD_SCALE 1.5
#define MINIMAP_SCALE 1

#define LONG_PRESS_INTERVAL 1

#define  MINIMAP_OPRN_MOVE "minmap.open.move" //开启启动

class MinimapInfo : public CCObject{
public:
    CREATE_FUNC(MinimapInfo);
    virtual bool init(){return true;};
    int selfPt;
    int chiefPt;
    std::map<int, int> m_members;//second==1为原有 second==2为联盟哨塔
};

class MinimapViewCCB : public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
//,public CCTouchDelegate
,ITouchDelegate

{
public:
    static MinimapViewCCB *create(int serverId);
    MinimapViewCCB(int serverId);
    ~MinimapViewCCB();
    static CCPoint getPointByWorldPoint(CCPoint &pt,float scale);
    static CCPoint getWorldPointByPoint(const CCPoint &pt,float scale);
    static CCPoint getRealPosByPoint(CCPoint &pt);
    Vec2 getPointByTilePos(Vec2 tPos);
    virtual void update(float time);
    float m_miniScale;
    void gotoCityPos(Vec2 &cityPos);
    void IsShowOrdinary(bool isOrdinary);
    void setRegionEffects(vector<int>& ciTyOrders);
    bool isOpenLocal;
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
    
    void moveBack();
    void scaleBack();
    bool canShowAlliance();
    void setCityColour(int index, Color3B colour);
    void addCity(int index, int type, int level=1);
    void showSelfCity();
    void addAllianceCity();
    void goToMinimapLocal1();
    void goToMinimapLocal2();
    void goToMinimapLocalBack(Node* sender);
    void openMove(Object* obj);
   
    void setUIZorder(Node* node, float iOrder);
    
    CCSafeObject<HFViewport> m_viewPort;
    CCSafeObject<CCNode> m_container;
    CCSafeObject<CCNode> m_miniCCBNode;
    CCSafeObject<CCNode> m_miniRenderTextureNode;

    CCSafeObject<MinimapUIView> m_mapUILayer;
    CCSafeObject<CCNode> m_mapNode;
    CCSafeObject<CCNode> m_cityNode;
    CCSafeObject<CCNode> m_regionNode;
    CCSafeObject<CCSprite> m_mapBg;
    CCSafeObject<CCSprite> m_mapSpVec[MAP_COUNT];
    CCSafeObject<CCRenderTexture> m_mapRenderTextureVec[MAP_COUNT];
    CCSafeObject<CCSprite> m_regionSpr;
    CCSafeObject<CCSprite> m_regionSpVec[REGION_COUNT];
    CCSafeObject<CCNode> m_phoneIcon;
    CCSafeObject<MinimapInfo> m_mapInfo;

    
    bool singleTouchClose;
    int m_mapW;
    int m_mapH;
    int m_CurrentMapIndex;
    int m_currentServerId;
    bool m_isLongPressBegan;
    float m_isLongPressTime;
    Vec2 m_LongPressPos;
    bool m_isLongPress;
    bool m_isOpenMove;
    bool m_isOrdinary;
    
    
//    vector<CCSprite*> m_mapSpVec;
//    vector<CCRenderTexture*> m_mapRenderTextureVec;
};
#endif /* defined(__IF__MinimapViewCCB__) */
