//
//  HandGuide.hpp
//  IF
//
//  Created by 王超一 on 15/11/12.
//
//

#ifndef HandGuide_hpp
#define HandGuide_hpp
#include "CommonInclude.h"

class HandGuide: public CCNode
{
public:
    HandGuide(float delayPerUnit=0.1):m_pos(CCPoint()),m_direction(1),delayPerUnit(delayPerUnit){};
    ~HandGuide(){};
    bool init();
    void play(CCPoint pos, int w, int h, int d = 1);
    void stop();
    static HandGuide* create(float delayPerUnit=0.1);
protected:
    CCPoint m_pos;
    int  m_direction;
    float delayPerUnit;
    CCSafeObject<CCSprite> m_hand;
private:
    virtual bool isClickSwallowed(){return true;}
    virtual int getPriority(){return 0;}
};

#endif /* HandGuide_hpp */
