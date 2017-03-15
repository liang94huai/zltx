//
//  TroopInformationView.h
//  IF
//
//  Created by 邹 程 on 14-4-18.
//
//

#ifndef __IF__TroopInformationView__
#define __IF__TroopInformationView__

#include "BaseTileInfo.h"
#include "MarchDetailView.h"
#include "NewBaseTileInfo.h"

class TroopInformation : public CCNode {
public:
    
    static TroopInformation* create(CCDictionary* info) {
        auto ret = new TroopInformation();
        if (ret && ret->init(info)) {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    
    bool init(CCDictionary* info);
    
private:
    MarchDetailInfo m_info;
    
    CCDrawNode* m_drawNode;

};




class TroopInformationView : public BaseTileInfo {
public:
    
    TroopInformationView(const string& marchId,int pointId):
    m_marchId(marchId)
    ,BaseTileInfo(WorldController::getInstance()->m_cityInfo[0])
    ,m_pointId(pointId)
    {}
    
    static TroopInformationView* create(const string& marchId,int pointId=-1) {
        auto ret = new TroopInformationView(marchId,pointId);
        if (ret && ret->init()) {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    
    bool init();
    
    void sendRequest();
    void onRequestCallback(CCObject* obj);
    
private:
    string m_marchId;
    int m_pointId;
};



//class TroopInfoPanel : public BaseTileInfo
class TroopInfoPanel : public NewBaseTileInfo
//, public CCTouchDelegate
{
public:
    
    TroopInfoPanel(const string& marchId):
    m_marchId(marchId)
    //,BaseTileInfo(WorldController::getInstance()->m_cityInfo[0])
    ,NewBaseTileInfo(WorldController::getInstance()->m_cityInfo[0])
    {}
    
    static TroopInfoPanel* create(const string& marchId) {
        auto ret = new TroopInfoPanel(marchId);
        if (ret && ret->init()) {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    
    bool init();
    
    void timeCount(float dt);
    
    void clickSpeedUp(CCObject * pSender, Control::EventType pCCControlEvent);
    void onCallBackClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onMarchInfoClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    virtual void closeThis();
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

    string m_marchId;
    
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCLabelIF> m_posLabel;
    CCSafeObject<CCNode> m_touchNode;
};


class TroopArmyCell : public CCNode
, public CCBMemberVariableAssigner

{
public:
    static TroopArmyCell *create(std::string name,std::string num,std::string armyId,int star = 0);
private:
    TroopArmyCell(std::string name,std::string num,std::string armyId,int star) : m_nameStr(name),
    m_numStr(num),
    m_armyId(armyId),
    m_star(star)
    {};
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    std::string m_nameStr;
    std::string m_numStr;
    std::string m_armyId;
    int m_star;
    
    CCSafeObject<CCLabelIF> m_numTxt;
    CCSafeObject<CCLabelIF> m_nameTxt;
    CCSafeObject<CCLabelIF> m_levelTxt;
    
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCNode> m_levelNode;
};







#endif /* defined(__IF__TroopInformationView__) */
