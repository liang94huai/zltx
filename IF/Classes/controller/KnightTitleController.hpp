//
//  KnightTitleController.hpp
//  IF
//
//  Created by 付彬 on 16/1/7.
//
//

#ifndef KnightTitleController_hpp
#define KnightTitleController_hpp

#include "CommonInclude.h"
#include "KnightTitleInfo.hpp"

class KnightTitleController : public CCObject{
public:
    static KnightTitleController* getInstance();
    static void purgeData();
    KnightTitleController();
    ~KnightTitleController();
    
    void retInitData(CCDictionary* dict);
    
    //变更称号
    bool startPutOnKnightTitle(int itemId);
    void retPutOnKnightTitle(CCDictionary* dict);
    
    //解锁称号
    bool startUnlockKnightTitle(int itemId);
    void retUnlockKnightTitle(CCDictionary* dict);
    
    //合成徽章
    bool startComposeBedge(int itemId);
    void retComposeBedge(CCDictionary* dict);
    //分解徽章
    bool startDecBedge(int itemId);
    void retDecBedge(CCDictionary* dict);
    
    //计算作用值
    void MakeEffectValue();
    
    //建筑升级解锁称号
    void UpdateUnlockKnight();
    
    //获取已经使用的徽章数量
    int GetUseBedgeById(int itemId);
    
    map<int, KnightTitleInfo> KnightTitleInfoMap;
    int m_curKnightTitleId;
    map<int, int> m_KnightLockMap;
    
    map<int, float> KnightEffMap;//称号作用
    
private:
};

#endif /* KnightTitleController_hpp */
