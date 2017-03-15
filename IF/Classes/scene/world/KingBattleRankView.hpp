//
//  KingBattleRankView.hpp
//  IF
//
//  Created by xiaoming on 15/12/2.
//
//

#ifndef KingBattleRankView_hpp
#define KingBattleRankView_hpp

#include <stdio.h>
#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCMultiColTableView.h"
#include "DefaultTableViewDelegate.h"
#include "CommandBase.h"


#define CTF_GET_RANK_COMMAND  "ctf.get.rank"
class CTFGetRankCommand: public CommandBase {
public:
    CTFGetRankCommand() :
    CommandBase(CTF_GET_RANK_COMMAND){
    }
    bool handleRecieve(cocos2d::CCDictionary *dict)
    {
        if (dict->valueForKey("cmd")->compare(CTF_GET_RANK_COMMAND) != 0)
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


class RankDataInfo : public CCObject
{
public:
    RankDataInfo(CCDictionary *dic);
    ~RankDataInfo();
    std::string getRankNum();
    std::string getPlayerName();
    std::string getKillNum();
    std::string getDeadNum();
private:
    std::string rankNum;
    std::string playerName;
    std::string killNum;
    std::string deadNum;
};

class KingBattleRankView: public PopupBaseView ,public CCBSelectorResolver ,public CCBMemberVariableAssigner , public CCTableViewDataSource, public DefaultTableViewDelegate
{
public:
    static KingBattleRankView* create();
    bool init();
private:
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    void onEnter();
    void onExit();
    CCSafeObject<CCTableView> m_tabView;
    
    
    cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName);
    cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName);
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    
    
    void updateInfo(CCObject *obj);
    void clearRankDataList();
    std::vector<RankDataInfo *>rankDataList;
    
    CCSafeObject<CCLabelIF> m_titleTxt;
    CCSafeObject<CCLabelIF> m_rankTxt;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_killTxt;
    CCSafeObject<CCLabelIF> m_deadTxt;
    CCSafeObject<CCLabelIF> m_myRankTxt;
    CCSafeObject<CCNode> m_listLayer;
    
    CCSafeObject<CCLabelIF> m_defenseTxt;
    CCSafeObject<CCLabelIF> m_attackTxt;
    CCSafeObject<CCNode> m_defenseTouch;
    CCSafeObject<CCNode> m_attackTouch;
    
    CCSafeObject<CCControlButton> m_prizeBtn;
    
    CCSafeObject<CCDictionary> defenseDic;
    CCSafeObject<CCDictionary> attackDic;
    
    
    void onCloseClick(CCObject *pSender, CCControlEvent event);
    void onPrizeClick(CCObject *pSender, CCControlEvent event);
    
    void updateRankData(CCDictionary *info);
    void touchPressed(Vec2 touchPoint);
    
    CCDictionary *lastDataDic;
    Color3B getLabelColor(bool isSelected);
    
    float designScale;
    
    Label *labelNoData;
    
};


class RankDataCell : public CCTableViewCell
{
public:
    static RankDataCell * create(RankDataInfo *info);
    RankDataCell(RankDataInfo *info);
    void updateData(RankDataInfo *info);
private:
    Label *labelRank;
    Label *labelName;
    Label *labelKill;
    Label *labelDead;
    void setLabel(Label **label,Vec2 pos,float fontSize);
    CCSprite *topIcon;
};



#endif /* KingBattleRankView_hpp */
