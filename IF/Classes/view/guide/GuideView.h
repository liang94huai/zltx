//
//  GuideView.h
//  IF
//
//  Created by ganxiaohua on 13-11-6.
//
//

#ifndef __IF__GuideView__
#define __IF__GuideView__
#include "CommonInclude.h"
#include "QuestInfo.h"
#include "ArrowAni.h"
#include "HandGuide.hpp"
class GuideView : public CCLayer
  
//TODO::CCKeypadDelegate
//, public CCKeypadDelegate

{
public:
    CREATE_FUNC(GuideView);
    bool init();
    void onEnter();
    void onExit();
    void doPlot(std::string did, bool cover);
    void doPlot1(std::string did, bool cover);
    void changePlot1Pos(CCPoint p, int w, int h, int d=1);
    void playPlotOver(CCObject* param);
    void stopGuide(Ref* ref);
    void setArrow(CCPoint p, int w, int h, int d=1, bool fly = false);
    void setHand(CCPoint p, int w, int h, int d=1, bool fly = false);
    void removeGuideView(CCObject* p);
    void addToFlyContainer(CCNode* node);
    void clearFlyContainer();
    void addCover(CCRect& rect, int type, bool showFire = true);
    void setModelLayerOpacity(int num);
    void removeCover();
    void removeModelLayer();
    CCNode *getGuideNode();
    bool isMoving;
    virtual void update(float delta);
    virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* unused_event);
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* unused_event);
private:
   // void update(float t);
   // virtual void keyBackClicked(){}
    virtual bool isClickSwallowed(){return true;}
    virtual int getPriority(){return 0;}
    void addParticleToBatch(cocos2d::CCParticleSystemQuad *particle);

    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void gotoEnd(cocos2d::CCObject *pTarget, CCControlEvent touchEvent);
    void showGoToBtn();
    
    int _id;
    CCSafeObject<CCRenderTexture> _modelLayer;
    CCSafeObject<CCNode> _containerNode;
    CCSafeObject<CCNode> m_flyContainer;
    CCSafeObject<CCNode> m_particleContainer;
    CCSafeObject<CCControlButton> m_gotoEndBtn;
    CCSafeObject<CCNode> m_gotoNode;
    CCSafeObject<ArrowAni> _arrowAni;
    CCSafeObject<HandGuide> _handGuide;
    CCRect m_rect;
    vector<CCParticleBatchNode*> m_parVec;
    CCSafeObject<Sprite> m_goToEndSprite;
};
#endif /* defined(__IF__GuideView__) */
