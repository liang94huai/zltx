//
//  SoldierIconCell.hpp
//  IF
//
//  Created by 李朝辉 on 15/11/3.
//
//

#ifndef SoldierIconCell_hpp
#define SoldierIconCell_hpp
#include "CommonInclude.h"


class SoldierIconCell : public CCNode{
public:
    static SoldierIconCell* create(string icon,int size,string armyId = "",bool bself = true,int star = 0,bool iconGrey = false);
    SoldierIconCell():m_icon(""),m_size(0){
    }
private:
    bool init(string icon, int size,string armyId,bool bself,int star,bool iconGrey);
    string m_icon;
    int m_size;
    int m_star;
    bool m_iconGrey;
};

#endif /* SoldierIconCell_hpp */
