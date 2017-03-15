//
//  CCLoadSprite.cpp
//  DragonEmpire
//
//  Created by 邹 程 on 13-8-22.
//
//

#include "CCLoadSprite.h"
#include "SceneController.h"
#include "DynamicResourceController.h"

#define CC_2x2_WHITE_IMAGE_KEY  "cc_2x2_white_image"
static unsigned char cc_2x2_white_image[] = {
    // RGBA8888
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF
};

#define MEM_LIMIT 40

#define COMMON_COUNT 3
#define WORLD_COUNT 12

USING_NS_CC;
void CCLoadSprite::doLoadCommonResourceAsync(){

    if(DynamicResourceController::getInstance()->getInitedCommon()){
        DynamicResourceController::getInstance()->downLoadAllCommon();
    }else{
        DynamicResourceController::getInstance()->sendGetCommonCMD();
    }
}
cocos2d::CCSprite* CCLoadSprite::createSprite(const char *fileName) {
    cocos2d::CCSpriteFrame *test = loadResource(fileName);
    if (!test) {
        return CCSprite::create();
    }
    return cocos2d::CCSprite::createWithSpriteFrame(test);
}
cocos2d::CCSprite* CCLoadSprite::createSprite(const char* fileName, bool useDefault,CCLoadSpriteType type) {
    auto ret = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileName);
    if (ret == NULL && useDefault ==true){
        if(type == CCLoadSpriteType_GOODS){
            DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_GOODS_TEXTURE,false);
            ret = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileName);
            if(!ret){
                fileName = "MonsterAttack_4.png";
            }
            DynamicResourceController::getInstance()->checkGoodsResource();
        }else if (type == CCLoadSpriteType_MONSTERLAYERBUST){
            if(DynamicResourceController::getInstance()->checkMonsterLayerRes()){
                DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_MONSTER_TEXTURE_layer,false);
                ret = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileName);
                if(!ret){
                    fileName = "rm_bust.png";
                }
            }else{
                fileName = "rm_bust.png";
            }
        }
        else if (type == CCLoadSpriteType_MONSTERLAYERLITTLE){
            if(DynamicResourceController::getInstance()->checkMonsterLayerRes()){
                DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_MONSTER_TEXTURE_layer,false);
                ret = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileName);
                if(!ret){
                    fileName = "rm.png";
                }
            }else{
                fileName = "rm.png";
            }
        }
        else if (type == CCLoadSpriteType_HEAD_ICON){
            if(DynamicResourceController::getInstance()->checkMonsterLayerRes()){
                DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_HEAD_TEXTURE_ICON,false);
                ret = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileName);
                if(!ret){
                    fileName = "g044.png";
                }
            }else{
                fileName = "g044.png";
            }
        }
        else if (type == CCLoadSpriteType_HEAD_ICON_BUST){
            if(DynamicResourceController::getInstance()->checkMonsterLayerRes()){
                DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_HEAD_TEXTURE_ICON_BUST,false);
                ret = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileName);
                if(!ret){
                    fileName = "g044_bust.png";
                }
            }else{
                fileName = "g044_bust.png";
            }
        }
        else if (type == CCLoadSpriteType_HEAD_ICON_MIDDLE){
            if(DynamicResourceController::getInstance()->checkMonsterLayerRes()){
                DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_HEAD_TEXTURE_ICON_MIDDLE,false);
                ret = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileName);
                if(!ret){
                    fileName = "g044_middle.png";
                }
            }else{
                fileName = "g044_middle.png";
            }
        }
        else if (type == CCLoadSpriteType_EQUIP)  {
            if (DynamicResourceController::getInstance()->checkEquipmentResource())
            {
                DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_EQUIPMENT_TEXTURE,false);
                ret = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileName);
                if(!ret){
                    fileName = "MonsterAttack_4.png";
                }
            } else {
                fileName = "MonsterAttack_4.png";
            }
        }
        else if(type == CCLoadSpriteType_SCIENCE){
            if (DynamicResourceController::getInstance()->checkScienceResource())
            {
                DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_SCIENCE_TEXTURE,false);
                ret = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileName);
                if(!ret){
                    fileName = "no_iconFlag.png";
                }
            } else {
                fileName = "no_iconFlag.png";
            }
        }
        else{
            fileName = "no_iconFlag.png";
        }
    }
    return createSprite(fileName);
}

CCScale9Sprite* CCLoadSprite::createScale9Sprite(const char *fileName) {
    cocos2d::CCSpriteFrame *test = loadResource(fileName);
    if (!test) {
        return CCScale9Sprite::create();
    }
    return CCScale9Sprite::createWithSpriteFrame(test);
}
cocos2d::CCSpriteFrame* CCLoadSprite::loadLoacalResource(const char *fileName, bool useDefault, CCLoadSpriteType type){
    auto ret = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileName);
    if (ret == NULL && useDefault == true){
        if(type == CCLoadSpriteType_GOODS){
            DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_GOODS_TEXTURE,false);
            ret = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileName);
            if(!ret){
                fileName = "MonsterAttack_4.png";
            }
            DynamicResourceController::getInstance()->checkGoodsResource();
        }
    } else if (ret == nullptr && useDefault == false) {
        if (type == CCLoadSpriteType_ACTIVITY_RES) {
            return ret;
        }
    }
    return loadResource(fileName);
}
cocos2d::CCSpriteFrame* CCLoadSprite::loadResource(const char *fileName) {
    auto ret = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileName);
    if (ret == NULL) {
        // Gets the texture by key firstly.
        auto texture = CCTextureCache::sharedTextureCache()->textureForKey(CC_2x2_WHITE_IMAGE_KEY);
        // If texture wasn't in cache, create it from RAW data.
        if (NULL == texture)
        {
            Image* image = new Image();
            bool isOK = image->initWithRawData(cc_2x2_white_image, sizeof(cc_2x2_white_image), 2, 2, 8);
            CCLOG("Connot find [%s] ,use 2x2_white_image instead", fileName);
            CC_UNUSED_PARAM(isOK);
            CCASSERT(isOK, "The 2x2 empty texture was created unsuccessfully.");
            
            texture = Director::getInstance()->getTextureCache()->addImage(image, CC_2x2_WHITE_IMAGE_KEY);
            CC_SAFE_RELEASE(image);
//            CCImage* image = new CCImage();
//            bool isOK = image->initWithImageData(cc_2x2_white_image, sizeof(cc_2x2_white_image));
//            //bool isOK = image->initWithImageData(cc_2x2_white_image, sizeof(cc_2x2_white_image), CCImage::Format::RAW_DATA, 2, 2, 8);
//            CCAssert(isOK, "The 2x2 empty texture was created unsuccessfully.");
//            texture = CCTextureCache::sharedTextureCache()->addUIImage(image, CC_2x2_WHITE_IMAGE_KEY);
//            CC_SAFE_RELEASE(image);
        }
        return CCSpriteFrame::createWithTexture(texture, CCRectMake(0, 0, 2, 2));
    }
    return ret;
}

cocos2d::CCSpriteFrame* CCLoadSprite::getSF(const char *fileName) {
    auto ret = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileName);
    return ret;
}

void CCLoadSprite::loadCommonResource() {
    int generalIconIndex = 1;
//    for (int i=generalIconIndex; i<=COMMON_COUNT; i++) {
//        doLoadResource(COMMON_PATH, i);
//        doLoadResourceAsync(COMMON_PATH, NULL, i);
//    }
    
    doLoadResource(COMMON_PATH, 1);
    doLoadResourceAsync(COMMON_PATH, NULL, 2);
    doLoadResourceAsync(COMMON_PATH, NULL, 3);
    doLoadResourceAsync(COMMON_PATH, NULL, 9);
    
    if (CCCommonUtils::isIosAndroidPad())
    {
        std::string _path = CCFileUtils::sharedFileUtils()->getWritablePath()+DynamicResource_Folder + COMMON_PATH_1_HD;
        if (CCFileUtils::sharedFileUtils()->isFileExist(_path))
        {
            _path = CCFileUtils::sharedFileUtils()->getWritablePath()+DynamicResource_Folder + COMMON_PATH_HD;
//            doLoadResource(_path.c_str(), 1); // 高清图标
            doLoadResourceAsync(_path.c_str(), NULL, 1);
        }
    }
    if (CCCommonUtils::isUseSpeUI()) {
        string _path = CCFileUtils::sharedFileUtils()->getWritablePath() + DynamicResource_Folder + UI_RES;
        _path = CCString::createWithFormat(_path.c_str(), CCCommonUtils::getUISkinType())->getCString();
        if (CCFileUtils::sharedFileUtils()->isFileExist(_path)) {
            _path = CCFileUtils::sharedFileUtils()->getWritablePath() + DynamicResource_Folder + UI_RES;
//            doLoadResource(_path.c_str(), CCCommonUtils::getUISkinType()); //xxr 加载主UI皮肤
            doLoadResourceAsync(_path.c_str(), NULL, CCCommonUtils::getUISkinType()); //xxr 加载主UI皮肤
        }
    }
    
//    doLoadResource(COMMON_PATH, 308);
    
    doLoadResourceAsync (COMMON_PATH,NULL, 308); // 装备图标
}
void CCLoadSprite::releaseDynamicResourceByType(CCLoadSpriteType type){
    if(type == CCLoadSpriteType_GOODS){
        releaseDynamicResourceByName(DynamicResource_GOODS_TEXTURE);
    }else if (type == CCLoadSpriteType_MONSTERLAYERBUST || type == CCLoadSpriteType_MONSTERLAYERLITTLE){
        releaseDynamicResourceByName(DynamicResource_MONSTER_TEXTURE_layer);
    } else if (type == CCLoadSpriteType_EQUIP) {
        releaseDynamicResourceByName(DynamicResource_EQUIPMENT_TEXTURE);
    }
    else if (type == CCLoadSpriteType_HEAD_ICON) {
        releaseDynamicResourceByName(DynamicResource_HEAD_TEXTURE_ICON);
    }
    else if (type == CCLoadSpriteType_HEAD_ICON_BUST) {
        releaseDynamicResourceByName(DynamicResource_HEAD_TEXTURE_ICON_BUST);
    }
    else if (type == CCLoadSpriteType_HEAD_ICON_MIDDLE) {
        releaseDynamicResourceByName(DynamicResource_HEAD_TEXTURE_ICON_MIDDLE);
    }
}
void CCLoadSprite::releaseDynamicResourceByName(const char* name){
    DynamicResourceController::getInstance()->loadNameTypeResource(name, true);
}
void CCLoadSprite::releaseCommonResourceForBattle() {
//    int res[5] = {7,11,13,14,15};
//    for (int i=0; i<5; i++) {
//        doReleaseResource(COMMON_PATH, res[i]);
//    }
}

void CCLoadSprite::releaseCommonResourceForWorld() {
    for (int i=1; i<=COMMON_COUNT; i++) {
        doReleaseResource(COMMON_PATH, i);
    }
    if (CCCommonUtils::isUseSpeUI()) {
        string _path = CCFileUtils::sharedFileUtils()->getWritablePath() + DynamicResource_Folder + UI_RES;
        _path = CCString::createWithFormat(_path.c_str(), CCCommonUtils::getUISkinType())->getCString();
        if (CCFileUtils::sharedFileUtils()->isFileExist(_path)) {
            _path = CCFileUtils::sharedFileUtils()->getWritablePath() + DynamicResource_Folder + UI_RES;
            doReleaseResource(_path.c_str(), CCCommonUtils::getUISkinType()); //xxr 加载主UI皮肤
        }
    }
    doReleaseResource(COMMON_PATH, 308);
}

int CCLoadSprite::doResourceByCommonIndex(int commonIndex,bool isLoad,bool isUseCount) {
    int index = 0;
    if (isLoad) {
        if (!checkHaveLoadCommonRes(commonIndex)) {
            index = commonIndex;
        }
        doLoadResource(COMMON_PATH, commonIndex,false,isUseCount);
    } else {
        doReleaseResource(COMMON_PATH, commonIndex,isUseCount);
    }
    return index;
}

void CCLoadSprite::doResourceByWorldIndex(int worldIndex,bool isLoad){
    if (isLoad) {
        doLoadResource(WORLD_PATH, worldIndex);
    } else {
        doReleaseResource(WORLD_PATH, worldIndex);
    }
}

void CCLoadSprite::doResourceByPathIndex(string path,int commonIndex,bool isLoad)
{
    if (isLoad) {
        doLoadResource(path.c_str(), commonIndex);
    } else {
        doReleaseResource(path.c_str(), commonIndex);
    }
}

void CCLoadSprite::doResourceByImperialIndex(int imperialIndex,bool isLoad){
    if (isLoad) {
        doLoadResource(IMPERIAL_PATH, imperialIndex);
    } else {
        doReleaseResource(IMPERIAL_PATH, imperialIndex);
    }
}

void CCLoadSprite::doResourceByGeneralIndex(int commonIndex,bool isLoad) {
    if (isLoad) {
        doLoadResource(GENERAL_PATH, commonIndex);
    } else {
        doReleaseResource(GENERAL_PATH, commonIndex);
    }
}

void CCLoadSprite::doResourceByGeneralIndexAsync(int commonIndex)
{
    doLoadResourceAsync(GENERAL_PATH, NULL ,commonIndex);
}

int CCLoadSprite::m_currentResourceSceneId = -1;

void CCLoadSprite::loadResourceBySceneId(int sceneId,CCDictionary *dict) {
    
    // right now we just clear every scene texture, so don't need this check
//    // check mem limit
//    int totalBytes = CCTextureCache::sharedTextureCache()->getTotalTextureBytes();
//    if (totalBytes > MEM_LIMIT*1024*1024 && sceneId != m_currentResourceSceneId && sceneId != SCENE_ID_GENERAL) {
//        // release last scene resource
//        releaseResourceBySceneId(m_currentResourceSceneId);
//    }
    
    
    if (sceneId != SCENE_ID_GENERAL) {
        m_currentResourceSceneId = sceneId;
    }
    // load resource
    parseLoadOrRelease(true, sceneId,dict);
}

void CCLoadSprite::releaseResourceBySceneId(int sceneId,CCDictionary *dict) {
    parseLoadOrRelease(false, sceneId,dict);
}

void CCLoadSprite::parseLoadOrRelease(bool isLoad,int sceneId, CCDictionary *dict) {
    auto actor = [&isLoad](const char* path,int index){
        if (isLoad) {
            if(GlobalData::shared()->sceneResourceStepLoad == 1){
                doLoadResource(path, index, true);
            }else{
                doLoadResource(path, index);
            }
        } else {
            doReleaseResource(path, index);
        }
    };
    if(isLoad){
        FrameSpriteLoader::getInstance()->removeAll();
    }
    switch (sceneId) {
        case SCENE_ID_LOADING:{
            const char* path = LOADING_PATH;
            if (!dict) {
//                for (int i=1; i<3; i++) {
//                    actor(path,i);
//                }
//                loading_1  只有jp平台用到暂时移动到loadingscene里面加载了
                actor(path,2);
                actor(path,3);
//                actor(path,12);
            } else {
//                CCDictElement* pElement = NULL;
//                CCDICT_FOREACH(dict, pElement){
//                    int index = dynamic_cast<CCInteger*>(pElement->getObject())->getValue();
//                    actor(path,index);
//                }
                for (int i=4; i<6; i++) {
                    actor(path,i);
                }
            }
        }
            break;
        case SCENE_ID_WORLD:{
            const char* path = WORLD_PATH;
            if (!dict) {
                for (int i=1; i<=WORLD_COUNT; i++) {
                    actor(path,i);
                }
//                actor(path,1);//地块
//                actor(path,2);
//                actor(path,4);//地表
//                actor(path,7);//建筑
                if (DynamicResourceController::getInstance()->checkServerFightResource())
                {
                    DynamicResourceController::getInstance()->loadNameTypeResource(DynamicResource_SERVERFIGHT_TEXTURE,!isLoad);
                }
            } else {
            }
        }
            break;
        case SCENE_ID_IMPERIAL:
        case SCENE_ID_MAIN:{
            const char* path = IMPERIAL_PATH;
            unsigned long ulc = clock();
            if (isLoad) {
                for (int i=1; i<=7; i++) {
                    if (i!=2) {
                        actor(path,i+10*GlobalData::shared()->contryResType);
                    }
                }
            }
            else
            {
                for (int i=1; i<=7; i++) {
                    actor(path,i+10*GlobalData::shared()->contryResType);
                }
            }
            actor(path,0);
            actor(path,29);
            CCLOG("Imperial load resource %lu",clock()-ulc);
        }
            break;
        case SCENE_ID_BATTLE:{
            const char* path = BATTLE_PATH;
            if (!dict) {
                for (int i=1; i<=1; i++) {
                    actor(path,i);
                }
            } else {
                // todo: parse battle texture info
                CCDictElement* pElement = NULL;
                CCDICT_FOREACH(dict, pElement){
                    auto key = pElement->getStrKey();
                    std::string subKey = key;
                    subKey = subKey.substr(0,5);
                    if (strcmp(subKey.c_str(), "skill") == 0) {
                        auto param = (CCInteger*)pElement->getObject();
                        actor(path,param->getValue() + 1000);
                    } else if (strcmp(key, "bg") == 0) {
                        auto param = (CCInteger*)pElement->getObject();
                        actor(path,(param->getValue())*10 + 1);
                        actor(path,(param->getValue())*10 + 2);
                    } else if (strcmp(key, "soldier") == 0) {
                        auto param = (CCArray*)pElement->getObject();
                        CCObject* pObj;
                        const char* soldierPath = BATTLE_SOLDIER_PATH;
                        CCARRAY_FOREACH(param, pObj) {
                            CCString* str = (CCString*)pObj;
                            char buffer[256];
                            sprintf(buffer,soldierPath,str->getCString());
                            actor(buffer,0);
                        }
                    }
                }
            }
        }
            break;
            
        case SCENE_ID_PVE:{
            const char* path = PVE_PATH;
            if (!dict) {
                actor(path,3);
            } else {
                int pveSceneId = dynamic_cast<CCInteger*>(dict->objectForKey("pveSceneId"))->getValue();
                int pveIndex = pveSceneId - 99999;
                actor(path,pveIndex*10+1);
                actor(path,pveIndex*10+2);
            }
        }
            break;
            
        case SCENE_ID_CROP:
        case SCENE_ID_IRON:
        case SCENE_ID_BARRACKS:
        case SCENE_ID_WOOD:
        case SCENE_ID_MARKET:
            break;            
        case SCENE_ID_GENERAL: {
            const char* path = GENERAL_PATH;
            for (int i=1; i<=1; i++) {
                actor(path,i);
            }
        }
            break;
        case SCENE_ID_DRAGON:{
            const char* path = IMPERIAL_PATH;
            unsigned long ulc = clock();
            if (isLoad) {
                for (int i=1; i<=7; i++) {
                    if (i!=2) {
                        actor(path,i+10*GlobalData::shared()->contryResType);
                    }
                }
            }
            else
            {
                for (int i=1; i<=7; i++) {
                    actor(path,i+10*GlobalData::shared()->contryResType);
                }
            }
            actor(path,0);
            actor(path,29);
            CCLOG("dragon scene load resource %lu",clock()-ulc);
        }
            break;
        default:
            CCLOG("Unknown scene id : %d  Load resource failed!",sceneId);
            return;
    }
    if(isLoad){
        FrameSpriteLoader::getInstance()->start();
    }
}

static std::map<std::string, int> mResourceUseCount;

void CCLoadSprite::doLoadResource(const char* path, int index, bool frameLoad,bool isUseCount) {
    if (path) {
        char buffer[256];
        sprintf(buffer,path,index);
        if(isUseCount)
        {
            ResourceUsedController::getInstance()->addRetainCount(buffer);
        }
        if (CCTexture2D::useDownloadResource() == true){
            
            string tmpFile(buffer);
            string tmpPath;
            auto iter = tmpFile.find_last_of(".");
            if(iter!=string::npos){
                tmpFile = tmpFile.substr(0,iter);
            }
            iter = tmpFile.find_last_of("/");
            if(iter!=string::npos){
                tmpPath = tmpFile.substr(0,iter);
                tmpFile = tmpFile.substr(iter+1);
            }
            
            if(DynamicResourceController::getInstance()->checkCommonResource(tmpFile)){
                DynamicResourceController::getInstance()->loadNameTypeResource(tmpFile, false);
            }else if(DynamicResourceController::getInstance()->checkGeneralResource(tmpFile)){
                DynamicResourceController::getInstance()->loadNameTypeResource(tmpFile, false, false);
            }else{
                cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(buffer);
            }
            
        }else{
            
            if(frameLoad){
                FrameSpriteLoader::getInstance()->addToLoadArr(buffer);
            }else{
                cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(buffer);
            }
            
        }
    }
}

void CCLoadSprite::doReleaseResource(const char* path, int index,bool isUseCount) {
    if (path) {
        char buffer[256];
        sprintf(buffer,path,index);
        if(isUseCount)
        {
            if(ResourceUsedController::getInstance()->releaseCount(buffer)>0)
            {
                return;
            }
        }
        int useCount = ResourceUsedController::getInstance()->getUseCount(buffer);
        if (useCount>0) {//如果持有大于0的，不能释放
            return ;
        }
//        cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesAndTextureWithFile(buffer);
        cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(buffer); 
        CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    }
    
    
if (CCTexture2D::useDownloadResource() == true && path) {
        string path1 =  CCFileUtils::sharedFileUtils()->getWritablePath();
        path1.append(DynamicResource_Folder);
        string filePath = path;
        auto iter = filePath.find_last_of("/");
        if(iter!=string::npos){
            filePath = filePath.substr(iter+1);
            path1.append(filePath);
        }
        char buffer[256];
        sprintf(buffer,path1.c_str(),index);
        CCLog("android_test_doReleaseResource[%s]",buffer);
        if(CCFileUtils::sharedFileUtils()->isFileExist(buffer)){
            cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(buffer);
            CCTextureCache::sharedTextureCache()->removeUnusedTextures();
        }
    }
}

void CCLoadSprite::doLoadResourceAsync(const char *path, CCCallFuncO* selector, int index)
{
    char buffer[256];
    sprintf(buffer,path,index);
    if (cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->isSpriteFramesWithFileLoaded(buffer))
    {
        if (selector)
        {
            selector->retain();
            selector->setObject(NULL);
            selector->execute();
            selector->release();
        }
       
        return;
    }
    if (buffer)
    {
        ValueMap dict;
        string texturePath("");
        
        ssize_t __size = 0;
        unsigned char* _bufferContent = nullptr;
        
        __size = ZipUtils::ccInflateCCZFile(buffer, &_bufferContent);
        if (__size > 0)
        {
            uint8_t len1 = _bufferContent[4];
            //        printf("%d", len1);
            char tmp[512];
            memset(tmp, 0, sizeof(tmp));
            unsigned int len = strlen(tmp);
            memcpy(tmp+len, _bufferContent+5, len1);
            
            if (strlen(tmp)>0)
            {
                // build texture path relative to plist file
                texturePath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(tmp, buffer);
            }
            else
            {
                // build texture path by replacing file extension
                texturePath = buffer;
                
                // remove .xxx
                size_t startPos = texturePath.find_last_of(".");
                texturePath = texturePath.erase(startPos);
                
                // append .png
                texturePath = texturePath.append(".png");
                
                CCLOG("cocos2d: CCSpriteFrameCache: Trying to use file %s as texture", texturePath.c_str());
            }
            
        }
        else {
            
            std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(buffer);
            if (fullPath.empty())
            {
                return;
            }
            ValueMap dict2 = FileUtils::getInstance()->getValueMapFromFile(fullPath);
            if(dict2.empty())
            {
                return;
            }
            dict = dict2;
            auto metadataDict = dict2["metadata"].asValueMap();
            //CCDictionary* metadataDict = (CCDictionary*)dict->objectForKey("metadata");
            if (!metadataDict.empty())
            {
                // try to read  texture file name from meta data
                texturePath = metadataDict["textureFileName"].
                asString();
            }
            
            if (! texturePath.empty())
            {
                // build texture path relative to plist file
                texturePath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(texturePath.c_str(), buffer);
            }
            else
            {
                // build texture path by replacing file extension
                texturePath = buffer;
                
                // remove .xxx
                size_t startPos = texturePath.find_last_of(".");
                texturePath = texturePath.erase(startPos);
                
                // append .png
                texturePath = texturePath.append(".png");
                
                CCLOG("cocos2d: CCSpriteFrameCache: Trying to use file %s as texture", texturePath.c_str());
            }
            
        }
        auto callbackClass = new ImageLoadAsync();
        callbackClass->setSuccessCallback(selector);
        callbackClass->setPlistPath(buffer);
        callbackClass->setDic(dict);
        callbackClass->setBufferData(_bufferContent);
        callbackClass->setBufferLen(__size);
        
        CCTextureCache::sharedTextureCache()->addImageAsync(texturePath, CC_CALLBACK_1(ImageLoadAsync::addImageCallback,callbackClass));
    }
}

bool CCLoadSprite::checkHaveLoadCommonRes(int commonIndex){
    bool flag = false;
    char buffer[256];
    sprintf(buffer,COMMON_PATH,commonIndex);
    if(ResourceUsedController::getInstance()->getUseCount(buffer)>0)
    {
        flag = true;
    }
    return flag;
}

void ImageLoadAsync::addImageCallback(CCObject* obj)
{
    CCTexture2D* pTexture = dynamic_cast<CCTexture2D*>(obj);
    if (pTexture)
    {
        ValueMap dict = getDic();
        if (!dict.empty())
        {
            cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithDictionary(dict, pTexture);
            dict.clear();
            //dict->release();
        }
        else
        {
           unsigned char* _bufferData = getBufferData();
            ssize_t __size = getBufferLen();
             cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithData(_bufferData, __size, pTexture);
            free(_bufferData);
            _bufferData = NULL;
        }
        
        cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesName(getPlistPath().c_str());
        
        if (getSuccessCallback())
        {
            CCCallFuncO * func=getSuccessCallback();
            func->retain();
            setSuccessCallback(NULL);
            func->setObject(pTexture);
            func->execute();
            func->release();
        }
    }
    else
    {
        CCLOG("cocos2d: CCSpriteFrameCache: Couldn't load texture");
    }
    CC_SAFE_RELEASE(this);
}

CCSize CCLoadSprite::getSpriteSize(const char *path, int index, const char *fileName)
{
    char buffer[256];
    sprintf(buffer,path,index);

    if (buffer)
    {
        std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(buffer);
        CCDictionary *dict = CCDictionary::createWithContentsOfFileThreadSafe(fullPath.c_str());
        
        if (dict == NULL) {
            return CCSize(0,0);
        }
        
        string texturePath("");
        
        CCDictionary* framesDict = (CCDictionary*)dict->objectForKey("frames");
        if (framesDict)
        {
            CCDictionary* fileDict = (CCDictionary*)framesDict->objectForKey(fileName);
            if (fileDict) {
                string _strSize = fileDict->valueForKey("sourceSize")->getCString();
                dict->release();
                return cocos2d::SizeFromString(_strSize.c_str());
            }
        }
        dict->release();
    }
    return CCSize(0,0);
}

static FrameSpriteLoader *_instace = NULL;
FrameSpriteLoader *FrameSpriteLoader::getInstance(){
    if(_instace == NULL){
        _instace = new FrameSpriteLoader();
        _instace->init();
    }
    return _instace;
}

void FrameSpriteLoader::init(){
    m_loadSpriteArr = CCArray::create();
}

void FrameSpriteLoader::addToLoadArr(std::string path){
    m_loadSpriteArr->addObject(CCString::create(path));
}

void FrameSpriteLoader::start(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

void FrameSpriteLoader::update(float time){
    if(m_loadSpriteArr && m_loadSpriteArr->count() > 0){
        std::string filePath = dynamic_cast<CCString*>(m_loadSpriteArr->objectAtIndex(0))->getCString();
        m_loadSpriteArr->removeObjectAtIndex(0);
        if(filePath != ""){
//            CCLoadSprite::loadResource(filePath.c_str());
            cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(filePath.c_str());
        }
    }
    if(!m_loadSpriteArr || m_loadSpriteArr->count() == 0){
        CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(SPRITE_LOAD_COMPLETE);
    }
}

void FrameSpriteLoader::removeAll(){
    if(m_loadSpriteArr){
        m_loadSpriteArr->removeAllObjects();
    }
}

static ResourceUsedController *_usedResourceInstace = NULL;


ResourceUsedController *ResourceUsedController::getInstance(){
    if(_usedResourceInstace == NULL){
        _usedResourceInstace =  new ResourceUsedController();
    }
    return _usedResourceInstace;
}
void ResourceUsedController::releasePool()
{
    mResourceUseDict.clear();
}

void ResourceUsedController::addRetainCount(string path)
{
    if (mResourceUseDict.find(path) == mResourceUseDict.end())
    {
        mResourceUseDict[path] = 1;
    }
    else
    {
        mResourceUseDict[path] = mResourceUseDict[path]+1;
    }
}
int ResourceUsedController::releaseCount(string path)
{
    int count = 0;
    if (mResourceUseDict.find(path) != mResourceUseDict.end())
    {
         mResourceUseDict[path] = mResourceUseDict[path]-1;
        count = mResourceUseDict[path];
    }
    CCLOG("ResourceUsedController::%d",count);
    return count;
}

int ResourceUsedController::getUseCount(string path){
    int count = 0;
    if (mResourceUseDict.find(path) != mResourceUseDict.end())
    {
        count = mResourceUseDict[path];
    }
    return count;
}
