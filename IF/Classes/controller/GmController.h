//
//  GmController.h
//  IF
//
//  Created by xxrdsg on 15-2-27.
//
//

#ifndef __IF__GmController__
#define __IF__GmController__

#include "CommonInclude.h"

class GmController: public CCObject
{
public:
    static GmController* getInstance();
    GmController(): isGmLogin(0),gmUid("") {};
    ~GmController(){};
    
    int isGmLogin;
    string gmUid;
};


#endif /* defined(__IF__GmController__) */
