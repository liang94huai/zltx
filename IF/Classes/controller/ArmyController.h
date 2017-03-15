//
//  ArmyController.h
//  IF
//
//  Created by 李锐奇 on 14-3-5.
//
//

#ifndef __IF__ArmyController__
#define __IF__ArmyController__

#include "CommonInclude.h"

enum ArmType{
    ARMY,
    FORT,
    TREAT_ARMY,
};

class ArmyController : public CCObject{
    
public:
    static ArmyController* getInstance();
    ArmyController() :
    treatFinishTime(0){};
    
    long getMaxNumByType(int type, bool isOri=false);
    
    long getTrainTime(int num, ArmyInfo &info);
    long getTrainSilver(int num, ArmyInfo &info);
    long getTrainWood(int num, ArmyInfo &info);
    long getTrainFood(int num, ArmyInfo &info);
    long getTrainIron(int num, ArmyInfo &info);
    long getTrainStone(int num, ArmyInfo &info);
    
    long getTreatTime(int num, TreatInfo &info);
    long getTreatSilver(int num, TreatInfo &info);
    long getTreatWood(int num, TreatInfo &info);
    long getTreatFood(int num, TreatInfo &info);
    long getTreatIron(int num, TreatInfo &info);
    long getTreatStone(int num, TreatInfo &info);
    long getTreatSolidierMaxNum();
    long getTreatSolidierTreatNum();
    long getTotalArmy();
    long getTotalArmyMan();
    long getTotalFree();
    long getTotalTreat();
    long getTotalDead();
    
    long getTotalFreeByType(int type);
    void refreshFortFreeNum(CCDictionary *dict);
    void refreshArmyFreeNum(CCDictionary *dict,bool refresh = true);
    void refreshTreatDeadNum(CCDictionary *dict);
    void refreshTreat(CCDictionary *dict);
    
    void setTreatFinishTime(time_t time);
    void setTreatEndTime(double time);
    void cancelTreate();
    
    long getTotalUpKeep();
    long getUpKeep(ArmyInfo *info);
    long getUpKeep(ArmyInfo *info, int num);
    
    float getLoad(ArmyInfo *info, bool isIncludeGeneralEffect = true, int num = 1);
    
    void startFinishForce(std::string armyId, std::string uid,int buildingKey);
    void startFinishFort(std::string armyId, std::string uid);
    void startFinishHospital(std::string uid);
    
    vector<long> getSoldierSqerNum();
    long getTotalFortNum();
    time_t treatFinishTime;
    bool costCD(int itemId, string toolUUID);
    int getStarlvById(std::string armyId);
    
    vector<string> getCreateSoldierIds(string sildiers, bool isFort);
};
#endif /* defined(__IF__ArmyController__) */
