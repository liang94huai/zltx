//
//  DailyActiveItemInfo.h
//  IF
//
//  Created by chenfubi on 15/4/10.
//
//

#ifndef __IF__DailyActiveItemInfo__
#define __IF__DailyActiveItemInfo__

#include "CommonInclude.h"


class DailyActiveItemInfo
{
public:
    DailyActiveItemInfo():unlockLevel(0),hasComplete(false),itemId(""),name(""),pic(""),target(""),desc(""),icon("Ranking_icon.png"),totalTimes(0),times(0),nowNum(0),totalNum(0),point(0),gotype(0),go(0),unlock(false),playAni(false),desc2(0),isShow(false), isNew(false), range(""), lastTimes(0){};
    ~DailyActiveItemInfo(){};
    
    void initData(string itemId);
    void initData(CCDictionary* dict);
    
    void updateData(CCDictionary* dict, bool setIsNew = false);
    
    string itemId;
    string name;
    string pic;
    string icon;
    string target;
    string desc;
    int totalNum;//单次需要采集总量
    int totalTimes;//次数
    int times;//现在次数
    int nowNum;//单次已经采集总量
    int point;
    int unlockLevel;
    int gotype;
    int go;
    bool hasComplete;
    bool unlock;
    bool playAni;
    int desc2;
    bool isShow;
    bool isNew;
    string range;//日常奖励完成时弹出提示的次数限制，使用前进行解析
    int lastTimes;//上次完成时的次数
};

#endif /* defined(__IF__DailyActiveItemInfo__) */
