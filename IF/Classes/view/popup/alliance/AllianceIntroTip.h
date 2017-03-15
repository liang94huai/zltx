//
//  AllianceIntroTip.h
//  IF
//
//  Created by ganxiaohua on 14-11-27.
//
//

#ifndef __IF__AllianceIntroTip__
#define __IF__AllianceIntroTip__

#include "CommonInclude.h"
enum AllianceIntroTip_Direction{
    AllianceIntroTip_Top,
    AllianceIntroTip_Bottom,
    AllianceIntroTip_Left,
    AllianceIntroTip_Right
};

class AllianceIntroTip : public CCIFTouchNode
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static AllianceIntroTip *create(bool up,CCPoint pos,string tipStr,string icon="");
    static AllianceIntroTip *createAutoAdjust(AllianceIntroTip_Direction dir,string tipStr,int maxWidth,int arrowOffsetX=0);
    void setAnimationAndPlay(int delayTime,int playTime);
    void setAutoChangeText(std::vector<string> mTextID,int delayTime,int playTime);
    void setText(std::string tipStr);
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCScale9Sprite> m_bg;
    void setTipsType(int type);
private:
    AllianceIntroTip(bool up,CCPoint pos,string tipStr,string icon) : m_up(up),m_pos(pos),m_tipStr(tipStr),m_delayTime(0),m_showTime(0),m_icon(icon),m_tipsType(0){};
    AllianceIntroTip(AllianceIntroTip_Direction dir,CCPoint pos,string tipStr) : m_direction(dir),m_pos(pos),m_tipStr(tipStr),m_delayTime(0),m_showTime(0),m_up(false),m_tipsType(0){};
    virtual bool init();
    bool initAutoAdjust(int maxWidth,int arrowOffset);
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    void playAnimation();
    void playAnimationOnce(CCObject* obj);
    void onHide(float t);
    bool m_up;
    AllianceIntroTip_Direction m_direction; //文字框的相对位置
    CCPoint m_pos;
    string m_tipStr;
    string m_icon;
    
    CCSafeObject<CCNode> m_arrow;
    CCSafeObject<CCLabelIF> m_tip;
    CCSafeObject<CCNode> m_iconNode;
    int m_delayTime;
    int m_showTime;
    bool m_isAutoAjust;
    void autoAjustLayout();
    vector<string> m_dialogID;
    void changeText();
    
    void refreshTouchNode();
    
    int m_tipsType;//默认为0，旅行商人为3,免费建造时间提示为4
    void setTouch(CCObject *obj);
};
#endif /* defined(__IF__AllianceIntroTip__) */
