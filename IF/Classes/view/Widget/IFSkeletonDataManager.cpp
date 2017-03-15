//
//  IFSkeletonDataManager.cpp
//  IF
//
//  Created by 舒 圣林 on 13-12-2.
//
//

#include "IFSkeletonDataManager.h"


static IFSkeletonDataManager* s_sharedIFSkeletonDataManager = NULL;

IFSkeletonDataManager* IFSkeletonDataManager::shared(void)
{
	if (!s_sharedIFSkeletonDataManager)
	{
		s_sharedIFSkeletonDataManager = new IFSkeletonDataManager();
	}
	return s_sharedIFSkeletonDataManager;
}

void IFSkeletonDataManager::purge()
{
    for (SkeletonDataMap::iterator it = m_skeletonDataCache.begin(); it != m_skeletonDataCache.end(); ) {
        if( it->second->nRef <= 0 )
        {
            spSkeletonData* pData = it->second;
            
            AtlasMap::iterator itAtlas =  m_atlasCache.find(pData);
            if(  itAtlas != m_atlasCache.end() )
            {
                spAtlas_dispose(itAtlas->second);
            }
            
            spSkeletonData_dispose(pData);
            m_skeletonDataCache.erase( it++ );
        }
        else
            ++it;
    }
    
}

spSkeletonData* IFSkeletonDataManager::getSkeletonData(const string& skeletonNamePrefix, float scale /*= 1.0f */)
{
    
    string skeletonDataFile = skeletonNamePrefix;
    skeletonDataFile.append(".json");
    
    string skeletonAtlasFile = skeletonNamePrefix;
    skeletonAtlasFile.append(".atlas");
    
    spSkeletonData* data =  getSkeletonData(skeletonDataFile.c_str(),skeletonAtlasFile.c_str(),scale);
    return data;
}

spSkeletonData* IFSkeletonDataManager::getSkeletonData(const char* skeletonDataFile, const char* atlasFile, float scale  /*= 1.0f */)
{
    CCAssert(skeletonDataFile != NULL && atlasFile != NULL, " skeletonDataFile and atlas file nil");
    string key = skeletonDataFile;
    SkeletonDataMap::iterator it = m_skeletonDataCache.find(key);
    if (it != m_skeletonDataCache.end()) {
        return it->second;
    }
    CCLOG("start read atlas file: %s",atlasFile);
	spAtlas* atlas = spAtlas_createFromFile(atlasFile, 0);
	CCAssert(atlas, "Error reading atlas file.");
    CCLOG("end read atlas file: %s",atlasFile);
    
    CCLOG("start read json file: %s",skeletonDataFile);
	spSkeletonJson* json = spSkeletonJson_create(atlas);
    if( json == NULL )
        return NULL;
	json->scale = scale == 0 ? (1 / CCDirector::sharedDirector()->getContentScaleFactor()) : scale;
	spSkeletonData* skeletonData = spSkeletonJson_readSkeletonDataFile(json, skeletonDataFile);
	CCAssert(skeletonData, json->error ? json->error : "Error reading skeleton data file.");
    if( skeletonData == nullptr )
    {
        if( strcmp(skeletonDataFile, "Spine/World/rm.json") == 0 )
        {
            if( m_skeletonDataCache.begin() != m_skeletonDataCache.end() )
                return m_skeletonDataCache.begin()->second;
            
        }
        
        return getSkeletonData( "Spine/World/rm.json" , "World/World_3.atlas");
    }
	spSkeletonJson_dispose(json);
    CCLOG("end read json file: %s",skeletonDataFile);
    
    if(skeletonData){
        m_skeletonDataCache[key] = skeletonData;
        m_atlasCache[skeletonData]   = atlas;
    }
    return skeletonData;
}
IFSkeletonDataManager::IFSkeletonDataManager()
{
	
}

IFSkeletonDataManager::~IFSkeletonDataManager()
{
	for (SkeletonDataMap::iterator it = m_skeletonDataCache.begin(); it != m_skeletonDataCache.end(); ++it) {
        spSkeletonData_dispose(it->second);
    }
    m_skeletonDataCache.clear();
    for (AtlasMap::iterator it = m_atlasCache.begin(); it != m_atlasCache.end(); ++it) {
        spAtlas_dispose(it->second);
    }
    m_atlasCache.clear();
}
