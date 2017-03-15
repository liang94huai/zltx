//
//  ResourceInfo.cpp
//  IF
//
//  Created by wangzhenlei on 13-9-30.
//
//

#include "CommonInclude.h"
#include "ResourceInfo.h"

void ResourceInfo::setResourceData(CCDictionary *dict){
    const CCString *pStr = dict->valueForKey("silver");
    if(pStr->compare("")!=0){
        lMoney = pStr->intValue();
    }
    pStr = dict->valueForKey("food");
    if(pStr->compare("")!=0){
        lFood = pStr->intValue();
    }
    pStr = dict->valueForKey("wood");
    if(pStr->compare("")!=0){
        lWood = pStr->intValue();
    }
    pStr = dict->valueForKey("iron");
    if(pStr->compare("")!=0){
        lIron = pStr->intValue();
    }
    pStr = dict->valueForKey("stone");
    if(pStr->compare("")!=0){
        lStone = pStr->intValue();
    }
    
    pStr = dict->valueForKey("food_bind");
    if(pStr->compare("")!=0){
        lFood_bind = pStr->doubleValue();
    }
    pStr = dict->valueForKey("wood_bind");
    if(pStr->compare("")!=0){
        lWood_bind = pStr->doubleValue();
    }
    pStr = dict->valueForKey("stone_bind");
    if(pStr->compare("")!=0){
        lStone_bind = pStr->doubleValue();
    }
    pStr = dict->valueForKey("iron_bind");
    if(pStr->compare("")!=0){
        lIron_bind = pStr->doubleValue();
    }
    
    pStr = dict->valueForKey("chip");
    if(pStr->compare("")!=0){
        lChip = pStr->intValue();
    }
    pStr = dict->valueForKey("diamond");
    if(pStr->compare("")!=0){
        lDiamond = pStr->intValue();
    }
    pStr = dict->valueForKey("dragonFood");
    if(pStr->compare("")!=0){
        lDragonFood = pStr->intValue();
    }
    pStr = dict->valueForKey("dragonGold");
    if(pStr->compare("")!=0){
        lDragonGold = pStr->intValue();
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CITY_RESOURCES_UPDATE);

}
double ResourceInfo::totalWood(){
    double value = lWood + lWood_bind;
    if (value >= INT32_MAX) {
        value = INT32_MAX;
    }
    return value;
}
double ResourceInfo::totalFood(){
    double value = lFood + lFood_bind;
    if (value >= INT32_MAX) {
        value = INT32_MAX;
    }
    return value;
}
double ResourceInfo::totalStone(){
    double value = lStone + lStone_bind;
    if (value >= INT32_MAX) {
        value = INT32_MAX;
    }
    return value;
}
double ResourceInfo::totalIron(){
    double value = lIron + lIron_bind;
    if (value >= INT32_MAX) {
        value = INT32_MAX;
    }
    return value;
}

void ResourceInfo::reduceWood(long delta , bool reduceAll){
    lWood -= delta;
    long tempDelta = 0;
    if (lWood < 0) {
        tempDelta = -lWood;
        lWood = 0;
    }
    if (reduceAll) {
        lWood_bind -= tempDelta;
        if (lWood_bind < 0) {
            lWood_bind = 0;
        }
    }
}
void ResourceInfo::reduceFood(long delta , bool reduceAll){
    lFood -= delta;
    long tempDelta = 0;
    if (lFood < 0) {
        tempDelta = -lFood;
        lFood = 0;
    }
    if (reduceAll) {
        lFood_bind -= tempDelta;
        if (lFood_bind < 0) {
            lFood_bind = 0;
        }
    }
}
void ResourceInfo::reduceStone(long delta , bool reduceAll){
    lStone -= delta;
    long tempDelta = 0;
    if (lStone < 0) {
        tempDelta = -lStone;
        lStone = 0;
    }
    if (reduceAll) {
        lStone_bind -= tempDelta;
        if (lStone_bind < 0) {
            lStone_bind = 0;
        }
    }
}
void ResourceInfo::reduceIron(long delta , bool reduceAll){
    lIron -= delta;
    long tempDelta = 0;
    if (lIron < 0) {
        tempDelta = -lIron;
        lIron = 0;
    }
    if (reduceAll) {
        lIron_bind -= tempDelta;
        if (lIron_bind < 0) {
            lIron_bind = 0;
        }
    }
}