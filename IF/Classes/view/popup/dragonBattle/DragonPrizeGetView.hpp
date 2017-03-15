//
//  DragonPrizeGetView.hpp
//  IF
//
//  Created by xiaoming on 15/12/15.
//
//

#ifndef DragonPrizeGetView_hpp
#define DragonPrizeGetView_hpp

#include <stdio.h>
#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CommandBase.h"



#define DRAGON_BATTLE_COMFIRM_REWARD_COMMAND  "dragon.battle.comfirm.reward"
class DragonComfirmCommand: public CommandBase {
public:
    DragonComfirmCommand() :
    CommandBase(DRAGON_BATTLE_COMFIRM_REWARD_COMMAND){
    }
    bool handleRecieve(cocos2d::CCDictionary *dict)
    {
        if (dict->valueForKey("cmd")->compare(DRAGON_BATTLE_COMFIRM_REWARD_COMMAND) != 0)
            return false;
        CCDictionary *params = _dict(dict->objectForKey("params"));
        if (params->objectForKey("errorCode")) {
            callFail(NetResult::createWithFail(params));
        } else {
            callSuccess(NetResult::createWithSuccess(params));
        }
        return true;
    }
};


#define DRAGON_BATTLE_GET_REWARD_COMMAND  "dragon.battle.get.reward"
class DragonGetCommand: public CommandBase {
public:
    DragonGetCommand() :
    CommandBase(DRAGON_BATTLE_GET_REWARD_COMMAND){
    }
    bool handleRecieve(cocos2d::CCDictionary *dict)
    {
        if (dict->valueForKey("cmd")->compare(DRAGON_BATTLE_GET_REWARD_COMMAND) != 0)
            return false;
        CCDictionary *params = _dict(dict->objectForKey("params"));
        if (params->objectForKey("errorCode")) {
            callFail(NetResult::createWithFail(params));
        } else {
            callSuccess(NetResult::createWithSuccess(params));
        }
        return true;
    }
};










class DragonPrizeGetView: public PopupBaseView ,public CCBSelectorResolver ,public CCBMemberVariableAssigner
{
public:
    static DragonPrizeGetView* create(CCDictionary *prizeData);
    bool init(CCDictionary *prizeData);
private:
    cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName);
    cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName);
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    void setPrizeScrollView(CCArray *dataArr);
    
    
    CCSafeObject<CCLayer> m_touchLayer;
    CCSafeObject<CCScale9Sprite> m_prizeSprite;
    CCSafeObject<CCLabelIF> m_titleLabel;
    CCSafeObject<CCLabelIF> m_descLabel;
    CCSafeObject<CCNode> m_winNode;
    CCSafeObject<CCNode> m_loseNode;
    CCSafeObject<CCNode> m_winEffectNode;
    CCSafeObject<CCNode> m_loseEffectNode;
    CCSafeObject<CCControlButton> m_getPrizeBtn;
    
    void comfirmRewardCallBack(CCObject *obj);
    void getRewardCallBack(CCObject *obj);
    void getRewardDataBack(CCDictionary *dataDic);
    void onGetClick(CCObject *pSender, CCControlEvent event);
    void getPrizeAnimeFinish(float dt);
    void addWinParticleEffect();
    void addLoseParticleEffect();
    
};



#endif /* DragonPrizeGetView_hpp */
