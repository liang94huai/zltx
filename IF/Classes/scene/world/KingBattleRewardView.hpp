//
//  KingBattleRewardView.hpp
//  IF
//
//  Created by xiaoming on 15/12/7.
//
//

#ifndef KingBattleRewardView_hpp
#define KingBattleRewardView_hpp

#include <stdio.h>
#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CommandBase.h"


#define CTF_GET_REWARD_COMMAND  "ctf.reward.info"
class CTFGetRewardCommand: public CommandBase {
public:
    CTFGetRewardCommand() :
    CommandBase(CTF_GET_REWARD_COMMAND){
    }
    bool handleRecieve(cocos2d::CCDictionary *dict)
    {
        if (dict->valueForKey("cmd")->compare(CTF_GET_REWARD_COMMAND) != 0)
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

class KingBattleRewardCell : public CCNode
{
public:
    static KingBattleRewardCell * create(CCDictionary *info);
    KingBattleRewardCell(CCDictionary *info);
};



class KingBattleRewardView: public PopupBaseView ,public CCBSelectorResolver ,public CCBMemberVariableAssigner
{
public:
    static KingBattleRewardView* create();
    bool init();
private:
    cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName);
    cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName);
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
    CCSafeObject<CCLabelIF> m_labelTitle;
    CCSafeObject<CCLabelIF> m_tab1;
    CCSafeObject<CCLabelIF> m_tab2;
    CCSafeObject<CCLabelIF> m_tab3;
    CCSafeObject<CCScale9Sprite> m_btn1Area;
    CCSafeObject<CCScale9Sprite> m_btn2Area;
    CCSafeObject<CCScale9Sprite> m_btn3Area;
    
    CCSafeObject<CCNode> m_attackNode;
    CCSafeObject<CCNode> m_defenseNode;
    CCSafeObject<CCNode> m_singleNode;
    CCSafeObject<CCScale9Sprite> m_attackScrollNode;
    CCSafeObject<CCScale9Sprite> m_defenseScrollNode;
    CCSafeObject<CCNode> m_singleScrollNode;
    
    CCSafeObject<CCLabelIF> m_attackPrizeLabel;
    CCSafeObject<CCLabelIF> m_attackWinLabel;
    CCSafeObject<CCLabelIF> m_defensePrizeLabel;
    
    CCSafeObject<CCNode> m_btnNode;
    
    void onCloseClick(CCObject *pSender, CCControlEvent event);
    
    void touchPressed(Vec2 touchPoint);
    bool checkBannerTouched(Vec2 touchPoint,Node *checkNode);
    void getRewardDataCallBack(CCObject *obj);
    
    void updateView(int newBanner);
    int bannerType;
    
    
    CCSafeObject<CCArray> attackRewardArr;
    CCSafeObject<CCArray> defenseRewardArr;
    CCSafeObject<CCArray> singleRewardArr;
    void addDataCellToScrollView(CCArray *dataArr,CCSprite *prizeSprite);
    Color3B getColor(bool isSelect);
    
    
    void setKingRewardScroll();
    void setSingleRewardScroll();
    
    CCScrollView *m_attackScrollView;
    CCScrollView *m_defenseScrollView;
    CCScrollView *m_singleScrollView;
};


#endif /* KingBattleRewardView_hpp */
