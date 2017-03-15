//
//  TransitionLayer.hpp
//  IF
//
//  Created by 王超一 on 15/12/16.
//
//

#ifndef TransitionLayer_hpp
#define TransitionLayer_hpp
#include "CommonInclude.h"

class TransitionLayer :
public CCNode
, public CCBMemberVariableAssigner
{
public:
    static TransitionLayer *create();
private:
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    bool init();
    virtual void onEnter();
    virtual void onExit();
    void doFinish();

    CCSafeObject<CCLabelIF> m_contentText;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCLayerColor> m_colorLayer;
};


#endif /* TransitionLayer_hpp */
