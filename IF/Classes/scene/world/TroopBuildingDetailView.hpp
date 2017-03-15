//
//  TroopBuildingDetailView.hpp
//  IF
//
//  Created by xiaoming on 15/11/25.
//
//

#ifndef TroopBuildingDetailView_hpp
#define TroopBuildingDetailView_hpp

#include <stdio.h>
#include "PopupBaseView.h"
#include "MarchDetailView.h"

class BuildingCellDelegate
{
public:
    virtual void buildingCellKickClick(std::string uuid)=0;
};




class TroopBuildingDetailView : public PopupBaseView ,public CCBMemberVariableAssigner , public BuildingCellDelegate
{
public:
    TroopBuildingDetailView(const string& marchId,int pointId);
    static TroopBuildingDetailView* create(const string& marchId,int pointId);
    bool init();
private:
    bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    string m_marchId;
    int m_pointId;
    CCSafeObject<CCLabelIF> m_title_label;
    CCSafeObject<CCNode> m_scrollLayer;
    CCSafeObject<CCNode> m_touchLayer;
    CCScrollView *m_scrollView;
    CCNode *m_scrollNode;
    
    void sendRequest();
    void onRequestCallback(CCObject* obj);
    void updateTroops(CCDictionary* info);
    
    void sendKickCommand(std::string udid);
    void kickRequestCallBack(CCObject *obj);
    void buildingCellKickClick(std::string uuid);
    std::vector<std::string>playKickedList;
    bool checkBeKicked(std::string uuid);
    std::string kickingUuid;
    MarchDetailInfo m_info;
};




class BuildingDetailCell : public CCNode
{
public:
    static BuildingDetailCell *create(vector<MarchSoldierDetail> *soldierList,std::string uuid,bool isOwner);
    BuildingDetailCell(vector<MarchSoldierDetail> *soldierList,std::string uuid,bool isOwner);
    void setDelegate(BuildingCellDelegate *buildingCellDelegate);
private:
    std::string uuid;
    void troopKickClick(CCObject* pSender, CCControlEvent event);
    BuildingCellDelegate *myDelegate;
    std::string userName;
};

#endif /* TroopBuildingDetailView_hpp */
