//
//  SoldierIconCell.cpp
//  IF
//
//  Created by 李朝辉 on 15/11/3.
//
//

#include "SoldierIconCell.hpp"
#include "FunBuildController.h"
#include "CCLoadSprite.h"




SoldierIconCell *SoldierIconCell::create(string icon,int size,string armyId,bool bself,int star,bool iconGrey){
    SoldierIconCell *ret = new SoldierIconCell();
    if(ret && ret->init(icon, size, armyId,bself,star,iconGrey)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool SoldierIconCell::init(string icon, int iconsize,string armyId,bool bself,int star,bool iconGrey){
    if(icon == ""){
        return true;
    }
    //////
    int commonIndex = std::atoi(armyId.c_str());
    commonIndex = commonIndex % 1000;
    commonIndex = commonIndex /100 + 200;
    auto sf = CCLoadSprite::getSF(icon.c_str());
    if (sf==NULL) {
        CCLoadSprite::doResourceByCommonIndex(commonIndex, true);
        setCleanFunction([commonIndex](){
            CCLoadSprite::doResourceByCommonIndex(commonIndex, false);
        });
    }
    string str = icon.substr(10,5);
    if (str == "small") {
       CCLoadSprite::doResourceByCommonIndex(204, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(204, false);
        });
    }
    
//    auto soidierIcon = CCLoadSprite::createSprite(icon.c_str());
    //////
//    CCLoadSprite::doResourceByCommonIndex(204, true);
//    CCLoadSprite::doResourceByCommonIndex(203, true);
//    CCLoadSprite::doResourceByCommonIndex(202, true);
//    CCLoadSprite::doResourceByCommonIndex(201, true);
//    CCLoadSprite::doResourceByCommonIndex(510, true);
//    setCleanFunction([](){
//        CCLoadSprite::doResourceByCommonIndex(204, false);
//        CCLoadSprite::doResourceByCommonIndex(203, false);
//        CCLoadSprite::doResourceByCommonIndex(202, false);
//        CCLoadSprite::doResourceByCommonIndex(201, false);
//        CCLoadSprite::doResourceByCommonIndex(510, false);
//    });

    m_size = iconsize;
    m_star = star;
    m_iconGrey = iconGrey;
    //士兵头像图标
    auto soidierIcon = CCLoadSprite::createSprite(icon.c_str());
    CCCommonUtils::setSpriteMaxSize(soidierIcon, m_size, true);
    if (m_iconGrey) {
        CCCommonUtils::setSpriteGray(soidierIcon, true);
    }
    this->addChild(soidierIcon);
    //添加星级标志
    auto starIcon = CCLoadSprite::createSprite("soldier_star_lv.png");
    float sizeWidth = soidierIcon->getContentSize().width;
    float sizeHeight = soidierIcon->getContentSize().height;
//    starIcon->setPosition(ccp(m_size*1.0/2,m_size*1.0/2));
    starIcon->setPosition(ccp(m_size*1.0*5/16,m_size*1.0*3/8));
    starIcon->setScale(0.8);
    if (m_iconGrey) {
        CCCommonUtils::setSpriteGray(starIcon, true);
    }
    this->addChild(starIcon);
    //添加星级文字
    CCLabelIFBMFont *label;
    if (bself) {
        //是自己的兵
        std::string valueStr = CC_ITOA(m_star);
        label = CCLabelIFBMFont::create(valueStr.c_str(), "pve_fnt_boss.fnt");
        label->setString(valueStr.c_str());
    }else{
        //不是自己的兵
        std::string Nochar = "";
//        int watchtoweerlv = FunBuildController::getInstance()->getWatchTowerStarLv();
        if (0) {
            Nochar = "?";
        }else{
            Nochar = CC_ITOA(m_star);
        }
        label = CCLabelIFBMFont::create(Nochar.c_str(), "pve_fnt_boss.fnt");
        label->setString(Nochar.c_str());
    }
    label->setFontSize(8);
    float spwidth = starIcon->getContentSize().width*starIcon->getScale();
    label->setPosition(ccp(m_size*1.0*5/16,m_size*1.0*3/8-2));
//    label->setPosition(ccp(m_size*1.0/2,m_size*1.0/2));
    this->addChild(label);
    //升星控制
    bool bhasStar = m_star >= 1;
    if(!bhasStar ){
        starIcon->setVisible(false);
        label->setVisible(false);
    }
    return true;
}

