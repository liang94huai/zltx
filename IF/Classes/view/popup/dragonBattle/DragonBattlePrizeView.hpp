//
//  DragonBattlePrizeView.hpp
//  IF
//
//  Created by xiaoming on 15/12/15.
//
//

#ifndef DragonBattlePrizeView_hpp
#define DragonBattlePrizeView_hpp

#include <stdio.h>
#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CommandBase.h"


#define DRAGON_BATTLE_PERSONAL_REWARD_COMMAND  "dragon.battle.personal.reward"
class DragonRewardCommand: public CommandBase {
public:
    DragonRewardCommand() :
    CommandBase(DRAGON_BATTLE_PERSONAL_REWARD_COMMAND){
    }
    bool handleRecieve(cocos2d::CCDictionary *dict)
    {
        if (dict->valueForKey("cmd")->compare(DRAGON_BATTLE_PERSONAL_REWARD_COMMAND) != 0)
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


class DragonBattlePrizeCell : public CCNode
{
public:
    static DragonBattlePrizeCell * create(CCDictionary *info);
    DragonBattlePrizeCell(CCDictionary *info);
};


class DragonBattlePrizeView: public PopupBaseView ,public CCBSelectorResolver ,public CCBMemberVariableAssigner
{
public:
    static DragonBattlePrizeView* create();
    bool init();
private:
    cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName);
    cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName);
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
    
    CCSafeObject<CCControlButton> m_winBtn;
    CCSafeObject<CCControlButton> m_loseBtn;
    CCSafeObject<CCNode> m_listLayer;
    CCSafeObject<CCNode> m_baseNode;
    void onClickWin(CCObject *pSender, CCControlEvent event);
    void onClickLose(CCObject *pSender, CCControlEvent event);
    
    CCSafeObject<CCArray> winRewardArr;
    CCSafeObject<CCArray> loseRewardArr;
    void updateScrollView(CCArray *dataArr);
    
    CCNode *scrollLayer;
    CCScrollView *m_scrollView;
    void getPrizeDataCallBack(CCObject *obj);
    
    CCArray * getRewardArr();
    Color3B getButtonColor(bool enable);
};



#endif /* DragonBattlePrizeView_hpp */
