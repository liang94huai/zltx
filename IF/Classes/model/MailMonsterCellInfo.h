//
//  MailMonsterCellInfo.h
//  IF
//
//  Created by lifangkai on 14-11-3.
//
//

#ifndef __IF__MailMonsterCellInfo__
#define __IF__MailMonsterCellInfo__

#include "MailInfo.h"
#include "MailMonsterInfo.h"
//继承 mailInfo

class MailMonsterCellInfo : public MailInfo{
public:
    static MailMonsterCellInfo* create();
    void parse(CCDictionary* dic);
    MailMonsterCellInfo():monster(NULL),totalNum(0),unread(0){};
    ~MailMonsterCellInfo();
    
    CCArray *monster;
    int totalNum;
    int unread;
};

class MailMonsterCellCellInfo : public MailInfo{
public:
    static MailMonsterCellCellInfo* create();
    void parse(CCDictionary* dic);
    MailMonsterCellCellInfo():monster(NULL),totalNum(0),unread(0){};
    ~MailMonsterCellCellInfo();
    
    MailMonsterInfo *monster;
    int totalNum;
    int unread;
};
#endif /* defined(__IF__MailMonsterCellInfo__) */
