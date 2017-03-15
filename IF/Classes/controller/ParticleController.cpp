//
//  ParticleController.cpp
//  IF
//
//  Created by 吴 凌江 on 13-8-30.
//
//

#include "ParticleController.h"
#include "CCLoadSprite.h"
static ParticleDataManager* s_sharedParticleDataManager = NULL;
ParticleDataManager* ParticleDataManager::shared()
{
    if (!s_sharedParticleDataManager)
    {
        s_sharedParticleDataManager = new ParticleDataManager();
    }
    return s_sharedParticleDataManager;
}

void ParticleDataManager::purge()
{
    CC_SAFE_RELEASE_NULL(s_sharedParticleDataManager);
}

ParticleDataManager::ParticleDataManager()
{
    
}

ParticleDataManager::~ParticleDataManager()
{
    m_DataCache.clear();
}

ValueMap ParticleDataManager::getParticleValue(const string& name)
{
    char path[64];
    sprintf(path, "particle/%s.plist",name.c_str());
    ParticleDataMap::iterator it = m_DataCache.find(path);
    if (it != m_DataCache.end()) {
        return it->second;
    }
    auto m_sPlistFile = CCFileUtils::sharedFileUtils()->fullPathForFilename(path);
    ValueMap dict2 = FileUtils::getInstance()->getValueMapFromFile(m_sPlistFile.c_str());
    m_DataCache[path] = dict2;
    return dict2;
}

CCParticleSystemQuad* ParticleController::createFightingParticle(const char *particleName,float duration,CCPoint sourcePoint,bool autoRemove) {
    // init particle file path
    char path[64];
    sprintf(path, "particle/%s.plist",particleName);
    CCParticleSystemQuad *pRet = CCParticleSystemQuad::create(path);
    if(pRet) {
        pRet->setDuration(duration);
        pRet->setPosition(sourcePoint);
        pRet->setAutoRemoveOnFinish(autoRemove);
        return pRet;
    } else {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
};





class SelfParticle : public CCParticleSystemQuad {
protected:
    bool _insideBounds;
    bool _isDraw;
    
public:
    std::string _pathName;
    bool _inPool;
    bool _isUsePool;
    SelfParticle():_insideBounds(true),_isDraw(false),_inPool(false),_isUsePool(false) {};
    void initTextureBySpriteFrame(CCSpriteFrame* frame);
    void setInit();
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    virtual void removeFromParent() override;
    virtual void update(float dt) override {
        if(this->_insideBounds) {
            CCParticleSystemQuad::update(dt);
        }
    };
};

void SelfParticle::initTextureBySpriteFrame(CCSpriteFrame* frame) {
    
    _opacityModifyRGB = false;
    
    auto texture = frame->getTexture();
    auto rect = frame->getRect();
    
    this->setTextureWithRect(texture, rect);
}

void SelfParticle::setInit() {
    m_initTextureByPlist = false;
}

void SelfParticle::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) {
    // Don't do calculate the culling if the transform was not updated
    _insideBounds = ((flags & FLAGS_TRANSFORM_DIRTY) && !_contentSize.equals(Size::ZERO)) ? renderer->checkVisibility(transform, _contentSize) : _insideBounds;
    
    if(_insideBounds)
    {
        
        CCParticleSystemQuad::draw(renderer, transform, flags);
    }
}

static std::map<std::string, std::vector<SelfParticle*>> mCachedParticles;

void SelfParticle::removeFromParent() {
    if( _isUsePool) {

        // check item has in pool
        for(auto& item : mCachedParticles[_pathName]) {
            if( item == this )
            {
                return;
            }
        }
        if( mCachedParticles[_pathName].size() < 512 ) {
            this->_inPool = true;
            mCachedParticles[_pathName].push_back(this);
            this->retain();
        }
    }
    
    CCParticleSystemQuad::removeFromParent();
}


#define PARTICLE_PATH "World/Particle.plist"
//#define PARTICLE_PATH_1 "World/Particle_1.plist"
#define EXAMPLE_PARTICLE "Effect_0000.png"
//#define EXAMPLE_PARTICLE_1 "Effect_0249.png"

static bool _particle_init = false;
void ParticleController::initParticle() {
    if (_particle_init) {
        return;
    }
    cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PARTICLE_PATH);
//    cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PARTICLE_PATH_1);

    _particle_init = true;
}
CCParticleSystemQuad* ParticleController::createParticleForLua(const char* name){
    if (!_particle_init) {
        return nullptr;
    }
    std::string path = name;
    path += ".plist";
    auto pRet = new SelfParticle();
    
    if(pRet) {
        ssize_t __size = 0;
        unsigned char* buffer = nullptr;
         ValueMap dict2;
        std::string textureName("");
        pRet->setInit();
        __size = ZipUtils::ccInflateCCZFile(path.c_str(), &buffer);
        if (__size > 0)
        {
            textureName = pRet->initWithData(buffer, "");
            free(buffer);
            buffer = NULL;
        }
        else
        {
            auto m_sPlistFile = CCFileUtils::sharedFileUtils()->fullPathForFilename(path);
            dict2 = FileUtils::getInstance()->getValueMapFromFile(m_sPlistFile.c_str());
            pRet->initWithDictionary(dict2, "");
            textureName = dict2["textureFileName"].asString();
        }
        
        
        auto spriteFrame = CCLoadSprite::loadResource(textureName.c_str());
        pRet->setPositionType(kCCPositionTypeRelative);
        pRet->initTextureBySpriteFrame(spriteFrame);
        
        pRet->autorelease();
        
        return pRet;
    } else {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

CCParticleSystemQuad* ParticleController::createParticle(const string& name,CCPoint sourcePosition,int maxParticles, CCDictionary *rdict) {
    if (!_particle_init) {
        return nullptr;
    }
    CCLOG("particle name: %s",name.c_str());
    char path[64];
    sprintf(path, "particle/%s.plist",name.c_str());
    
    ssize_t __size = 0;
    unsigned char* buffer = nullptr;
    
    auto pRet = new SelfParticle();
    
    if(pRet) {
        ValueMap dict2;
 
//        if (rdict) {
//            dict = rdict;
//            dict->retain();
//        }
//        else {
        pRet->setInit();
        std::string textureName("");
            __size = ZipUtils::ccInflateCCZFile(path, &buffer);
            if (__size > 0)
            {
                textureName = pRet->initWithData(buffer, "", maxParticles);
            }
            else
            {
                auto m_sPlistFile = CCFileUtils::sharedFileUtils()->fullPathForFilename(path);
                dict2 = ParticleDataManager::shared()->getParticleValue(name);
                pRet->initWithDictionary(dict2, "");
                textureName = dict2["textureFileName"].asString();
            }
        auto spriteFrame = CCLoadSprite::loadResource(textureName.c_str());
        
        pRet->setPositionType(kCCPositionTypeRelative);
        pRet->initTextureBySpriteFrame(spriteFrame);
        pRet->setPosition(sourcePosition);
        pRet->autorelease();
        return pRet;
    } else {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}
CCParticleSystemQuad* ParticleController::createParticleInPoolForLua(const char *name){
    //crash修改，暂时关闭
//    if( mCachedParticles.find(name) != mCachedParticles.end()) {
//        if( mCachedParticles[name].size() > 0) {
//            CCParticleSystemQuad* ret = mCachedParticles[name].back();
//            mCachedParticles[name].pop_back();
//            ret->resetSystem();
//            return ret;
//        }
//    }
    SelfParticle* pRet = (SelfParticle*)ParticleController::createParticleForLua(name);
    if( pRet != NULL ) {
        pRet->_pathName = name;
        //crash修改，暂时关闭
        pRet->_isUsePool = false;
//        pRet->_isUsePool = true;
    }
    return pRet;
}
CCParticleSystemQuad* ParticleController::createParticleInPool(const string& name,CCPoint sourcePosition,int maxParticles, CCDictionary *rdict) {
    //crash修改，暂时关闭
//    if( mCachedParticles.find(name) != mCachedParticles.end()) {
//        if( mCachedParticles[name].size() > 0) {
//            CCParticleSystemQuad* ret = mCachedParticles[name].back();
//            mCachedParticles[name].pop_back();
//            ret->resetSystem();
//            return ret;
//        }
//    }
    
    SelfParticle* pRet = (SelfParticle*)ParticleController::createParticle(name, sourcePosition, maxParticles, rdict);
    if( pRet != NULL ) {
        pRet->_pathName = name;
        //crash修改，暂时关闭
        pRet->_isUsePool = false;
//        pRet->_isUsePool = true;
    }
    return pRet;
}

void ParticleController::releasePool() {
    for (auto& item: mCachedParticles) {
        for(auto& child : item.second ) {
            if (child != nullptr) {
                child->release();
            }
        }
        item.second.clear();
    }
    mCachedParticles.clear();
}

CCParticleBatchNode* ParticleController::createParticleBatch(CCParticleSystemQuad *particle) {
    if (!_particle_init) {
        return nullptr;
    }
//    std::string picName = "";
//    if(particle == NULL){
//        picName = EXAMPLE_PARTICLE;
//    }else{
//        GLuint name1 = particle->getTexture()->getName();
//        GLuint name2 = CCLoadSprite::loadResource(EXAMPLE_PARTICLE)->getTexture()->getName();
//        if(name1 == name2){
//            picName = EXAMPLE_PARTICLE;
//        }else{
//            picName = EXAMPLE_PARTICLE_1;
//        }
//    }
    auto ret = CCParticleBatchNode::createWithTexture(CCLoadSprite::loadResource(EXAMPLE_PARTICLE)->getTexture());
    if (ret) {
        return ret;
    } else {
        return nullptr;
    }
}
