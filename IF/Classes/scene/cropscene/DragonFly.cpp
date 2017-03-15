//
//  DragonFly.cpp
//  IF
//
//  Created by 王超一 on 15/12/3.
//
//

#include "DragonFly.hpp"

DragonFly *DragonFly::create(){
    DragonFly *ret = new DragonFly();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool DragonFly::init(){
    auto bg = CCBLoadFile("DragonFly", this, this);
    this->getAnimationManager()->runAnimationsForSequenceNamed("Fly");
    return true;
}
bool DragonFly::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    return false;
}
SEL_CCControlHandler DragonFly::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}