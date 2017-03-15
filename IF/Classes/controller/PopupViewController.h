//
//  PopupViewController.h
//  IF
//
//  Created by 邹 程 on 13-8-29.
//
//

#ifndef __IF__PopupViewController__
#define __IF__PopupViewController__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "FlyHint.h"

#define MSG_SCROLL_TO_NEXT_MAIL "MSG_SCROLL_TO_NEXT_MAIL"
#define MSG_SCROLL_TO_PREVIOUS_MAIL "MSG_SCROLL_TO_PREVIOUS_MAIL"

class TouchSprite : public CCNode
{
public:
    CREATE_FUNC(TouchSprite);
    CCPoint m_clickPos;
    
private:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchCancelled(Touch* touch, Event* event){}
    
};

class PopupViewController :
public CCObject
{
public:
    enum ChangePopType
    {
        cPREVIOUS = 1,
        cNEXT,
    };
    static PopupViewController* getInstance();
    int addPopupView(PopupBaseView *view, bool useAnimation = false, bool needLayout = true);
    int addPopupInView(PopupBaseView *view, bool needLayout = true, bool useAnimation = false, bool openAnimation = false, bool isHD = true);
    int addPopupInViewWithAnim(PopupBaseView *view, bool needLayout = true);
    void addGoBackViewWithAnim(bool isCloseAnim = false);
    int changePopInView(PopupBaseView *view, int type);
    bool pushPop(PopupBaseView *view, bool force=false);
    void removePopupView(PopupBaseView *view,bool clareUI = true,bool isNewMailListPopup = false);
    void removeLastSystemMailPopupView();
//    void removePopupView(int _id);
    int getGoBackViewCount();
    int getCurrViewCount();
    void removeLastPopupView();
    void removeAllPopupView();
    void forceClearAll( bool bAutoRelease = false );  // 强制删除
    void addGoBackStack(bool isCloseAnim = false);
    int addPopupFromAndroidInView(PopupBaseView *view, bool needLayout=true) ;
    void addGoBackViewFromAndroidWithAnim(bool isCloseAnim);

    int goBackPopupView(PopupBaseView *removeView = NULL);
    int goBackPopupViewWithAnim(PopupBaseView *removeView = NULL,PopupBaseView *gobackView = NULL);
    PopupBaseView* getPopupView(int _id);
    PopupBaseView* getCurrentPopupView();
    void setTouchPos(CCPoint&);
    ~PopupViewController();
    
    void showPushPop();
    bool CanPopPushView;
    CCPoint getTouchPoint();
    bool getPlayingInAnim(){
        return m_isPlayingInAnim;
    }
    
    void addPopFlyHint(FlyHint* flyhint);
    void removePopFlyHint(FlyHint* flyhint);
    void setPrincessShow(bool show);
    bool getPrincessShow();
private:
    PopupViewController() = default;
    PopupViewController& operator=(const PopupViewController&) = default;
    PopupViewController(const PopupViewController&) = default;
    void whenPalyPopInAnim();
    void whenPalyBackInAnim();
    //左右切换的动画
    void whenPlayChangeAnim();
    void doResourceClear(float time);
    void popupCloseEvent(CCObject* params);
    void onGetMsgScrollLeft(Ref* ref);
    void onGetMsgScrollRight(Ref* ref);
    map<int, PopupBaseView*> m_stack;
    map<int, PopupBaseView*> m_gobackStack;
    vector<PopupBaseView*> m_pushPopStack;
    
    vector<FlyHint*> m_flyHintStack;
    
    PopupBaseView* m_currentInView;
    int m_currentId;
    TouchSprite* m_coverSprite;
    bool m_isPlayingInAnim;
    bool m_isPlayingChangingAnim;//面板左右切换 标记位
    bool m_princessTalkViewOnShow;//小公主气泡点击后下正在弹出对话
};


#endif /* defined(__IF__PopupViewController__) */
