//
//  DragonFly.hpp
//  IF
//
//  Created by 王超一 on 15/12/3.
//
//

#ifndef DragonFly_hpp
#define DragonFly_hpp

#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "CCLabelBatch.h"
#include "CCAniNode.h"

class DragonFly : public CCAniNode
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
public:
    static DragonFly *create();
private:
    DragonFly(){};
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
};

#endif /* DragonFly_hpp */
