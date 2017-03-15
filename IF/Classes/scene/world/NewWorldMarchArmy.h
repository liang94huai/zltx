//
//  NewWorldMarchArmy.h
//  IF
//
//  Created by 李锐奇 on 14-7-14.
//
//

#ifndef __IF__NewWorldMarchArmy__
#define __IF__NewWorldMarchArmy__

#include "CommonInclude.h"
#include "WorldController.h"

class RockAni : public CCObject{
public:
    static RockAni *create(CCNode *parent);
    void attack(CCPoint &startPt, CCPoint &endPt, float totalTime, unsigned int index, int rockIdx, bool useDelayTime = false);
    void attack2(CCPoint &startPt, CCPoint &endPt, unsigned int index, int rockIdx,int direction);
    ~RockAni();
private:
    bool init(CCNode *parent);
    CCPoint m_startPt;
    CCPoint m_endPt;
    CCSafeObject<CCSprite> m_rock;
    unsigned int m_attIndex;
    int m_rockIndex;
    void playRockAttackAnimation(std::string picName);
    void addParitcle(CCObject* obj);
    void setSockVisible();
};

class Arrow : public CCObject{
public:
    static Arrow *create(CCNode *parent);
    void attack(CCPoint &startPt, CCPoint &endPt, float totalTime);
    ~Arrow();
    void update(float time);
private:
    bool init(CCNode *parent);
    
    CCPoint m_startPt;
    CCPoint m_endPt;
    CCPoint m_currentPt;
    CCSafeObject<CCSprite> m_arrow;
};

class Soldier : public CCObject
{
public:
    static Soldier *create(CCSprite *parent, int type, float direction, CCPoint pt, std::string uuid, int row, int col);
    void setDirection(float direction);
    void walk();
    void attack();
    void playDeadAni(float delayTime);
    void dead(float _time);
    void setZorder(int zorder);
    void setPos(CCPoint &pt);
    CCPoint getCurrentPos();
    void spreadTo(CCObject *arr);
    void soldierDelete();
    bool clickOnSoldier(const CCPoint &pt);
    ~Soldier();
    void update(float time);
    CC_SYNTHESIZE(bool, m_isSpeSoldier, IsSpeSoldier);
protected:
    int frameCount;
    float frameTime;
    
private:
    Soldier(CCSprite *parent, int type, float direction, CCPoint pt, std::string uuid, int row, int col) :
    m_parent(parent),
    m_type(type),
    m_direction(direction),
    m_pos(pt),
    m_uuid(uuid),
    m_row(row),
    m_col(col)
    {};
    bool init();
    
    CCAnimate *getAnimate(float direction, int state);
    void resetAnimate(float direction, int state);
    void playDeadAniBack(CCObject* obj);
    void playAttackAnimation(float _time);
    void addParitcle();
    void addActParticle();
    std::string getLandType();
    void playRockAttackAnimation(float _time);
    void addWarParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    void addSpeParticleToBatch(cocos2d::CCParticleSystemQuad *particle);
    cocos2d::CCParticleSystemQuad * getParticleInBatchByName(const string& name);
    void removeAllSpeParticle();
    int getParticleTag();
    void setKingSpritePos();
    void removeAllParticle();
//    CCSafeObject<CCSprite> m_sprite;
    CCSafeObject<CCSprite> m_sprite;
    CCSafeObject<CCSprite> m_kingSprite1;
    CCSafeObject<CCSprite> m_kingSprite2;
    CCSafeObject<CCSprite> m_parent;
    CCSafeObject<Node> m_kingParNode;
    int m_type;
    float m_direction;
    int m_currentState;//0 walk 1 attack 2 dead
    CCPoint m_pos;
    std::string m_uuid;
    int m_row;
    int m_col;
//    vector<CCNode*> m_parVec;
    std::string m_currentParticleType;
    vector<ParticleBatchNode*> m_speParticleBatchVec;
};


class Phalanx : public CCObject{
public:
    static Phalanx *create(int type, float direction, int num, CCSprite* node, CCPoint pt, std::string uuid);
    void walk();
    void attack();
    void setDirection();
    void spreadTo(CCArray *data);
    void killed(int num);
    int getW();
    bool clickOnPhalanx(const CCPoint &pt);
    ~Phalanx();
    void setSpeRow();
protected:
    virtual bool init();
    
private:
    Phalanx(int type, float direction, int num, CCSprite* node, CCPoint pt, std::string uuid) :
    m_type(type)
    , m_direction(direction)
    , m_soldierNum(num)
    , m_sprite(node)
    , startPt(pt)
    , m_uuid(uuid)
    {};

    CCPoint getSoldierPos(int x, int y, float direction,CCPoint startPt);
    CCPoint getSoldierPosByRowAndCol(int row, int col, float direction, int rowCount, int colCount,CCPoint startPt, int totalW = 80, int totalH = 80);

    float m_direction;
    int m_soldierNum;
    int m_type;
    CCSafeObject<CCSprite> m_sprite;
    std::map<int, Soldier*> m_soldiers;
    CCPoint startPt;
    std::string m_uuid;
    int m_row;
    int m_col;
    int m_killed;
    int gapX;
};


class MarchArmy : public CCObject{
public:
    static MarchArmy *create(std::string uuid, CCSprite *parent);
    void walk();
    void attck();
    void beAttacked();
    void armyDelete();
    ~MarchArmy();
    static int getStartIndex(std::string uuid);
    static int getEndIndex(std::string uuid);
    static std::map<int, MarchSoldierInfo> &getMap(std::string uuid);
    bool clickOnArmy(const CCPoint &pt);
private:
    MarchArmy(std::string uuid, CCSprite *parent) : m_uuid(uuid), m_parent(parent){};
    virtual bool init();
    void initArmy();
    void playAttackAnimation();
    void addArmyByType(int type, float direction, int num, CCSprite *sprite, int startLine);
    void cityAttack(float _time);
    bool isHaveArmy(int type);
    bool isHaveHalanx(int type);
    void addSpeTail();
    int gapX;
    int gapY;
    std::string m_uuid;
    std::map<int, Phalanx*> m_phalanx;
    CCSafeObject<CCSprite> m_parent;
    int m_state;
};
#endif /* defined(__IF__NewWorldMarchArmy__) */
