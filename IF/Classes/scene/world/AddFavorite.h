//
//  AddFavorite.h
//  IF
//
//  Created by 邹 程 on 14-2-25.
//
//

#ifndef __IF__AddFavorite__
#define __IF__AddFavorite__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "BaseTileInfo.h"
#include "InputFieldMultiLine.h"

class AddFavorite :
public BaseTileInfo
{
public:

    AddFavorite(unsigned int cityIndex,const string& name,FavoriteType type,bool flag):
    m_index(cityIndex)
    ,m_name(name)
    ,m_type(type)
    ,m_flag(flag)
    ,BaseTileInfo(WorldController::getInstance()->m_cityInfo[cityIndex])
    {}
    
    static AddFavorite* create(unsigned int cityIndex,const string& name = "Untitled",FavoriteType type = Favorite,bool flag = true) {
        auto ret = new AddFavorite(cityIndex,name,type,flag);
        if (ret->init()) {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);

    bool init();
    
    void clickIcon(FavoriteType type);
    
    void keyPressedBtnOk(CCObject * pSender, Control::EventType pCCControlEvent);
    void shareBtnPress(CCObject * pSender, Control::EventType pCCControlEvent);
    
private:
    unsigned int m_index;
    std::string m_name;
    FavoriteType m_type;
    bool m_flag;
    bool m_shareBtn;
    CCSafeObject<CCLabelIF> m_location;
    CCSafeObject<CCLabelIF> m_tapLabel;
    CCSafeObject<CCLabelIF> m_choLabel;
    CCSafeObject<CCLabelIF> m_iconLabel[3];
    CCSafeObject<CCNode> m_editTitleNode;
    CCSafeObject<CCNode> m_icon[3];
    CCSafeObject<CCSprite> m_selectSprite;
    CCSafeObject<InputFieldMultiLine> m_editBox;
    
    CCSafeObject<CCLabelIF> m_titleNew;
    CCSafeObject<CCControlButton> m_btnOk;
};

#endif /* defined(__IF__AddFavorite__) */