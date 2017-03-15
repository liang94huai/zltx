/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "base/CCAutoreleasePool.h"
#include "base/ccMacros.h"
#include "CCDirector.h"

NS_CC_BEGIN

AutoreleasePool::AutoreleasePool()
: _name("")

{
    _managedObjectArray.reserve(150);
    //PoolManager::getInstance()->push(this);
}

AutoreleasePool::AutoreleasePool(const std::string &name)
: _name(name)

{
    _managedObjectArray.reserve(150);
    //PoolManager::getInstance()->push(this);
}

AutoreleasePool::~AutoreleasePool()
{
    CCLOGINFO("deallocing AutoreleasePool: %p", this);
    clear();
    
}

void AutoreleasePool::addObject(Ref* object)
{
    _managedObjectArray.push_back(object);
}

void AutoreleasePool::removeObject(Ref *object)
{
    std::vector<Ref*>::iterator itr = _managedObjectArray.begin();
    std::vector<Ref*>::iterator itrEnd = _managedObjectArray.end();
    while ( itr != itrEnd ) {
        if( *itr == object )
        {
            _managedObjectArray.erase(itr++ );
        }
        else
            ++itr;
    }
    
}

void AutoreleasePool::clear()
{
//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)

//#endif
    std::vector<Ref*> releasings;
    releasings.swap(_managedObjectArray);
    for (const auto &obj : releasings)
    {
        obj->release();
    }

}

bool AutoreleasePool::contains(Ref* object) const
{
    for (const auto& obj : _managedObjectArray)
    {
        if (obj == object)
            return true;
    }
    return false;
}

void AutoreleasePool::dump()
{
    CCLOG("autorelease pool: %s, number of managed object %d\n", _name.c_str(), static_cast<int>(_managedObjectArray.size()));
    CCLOG("%20s%20s%20s", "Object pointer", "Object id", "reference count");
    for (const auto &obj : _managedObjectArray)
    {
        CC_UNUSED_PARAM(obj);
        CCLOG("%20p%20u\n", obj, obj->getReferenceCount());
    }
}


//--------------------------------------------------------------------
//
// PoolManager
//
//--------------------------------------------------------------------

PoolManager* PoolManager::s_singleInstance = nullptr;

PoolManager* PoolManager::getInstance()
{
    if (s_singleInstance == nullptr)
    {
        s_singleInstance = new (std::nothrow) PoolManager();
        // Add the first auto release pool
       
    }
    return s_singleInstance;
}

void PoolManager::destroyInstance()
{
    delete s_singleInstance;
    s_singleInstance = nullptr;
}

PoolManager::PoolManager()
 //#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
: _isClearing(false)
//#endif
{
    int ret;
    if(( ret = pthread_mutexattr_init(&_poolMutexAttr)) != 0){
        CCAssert(0, "pthread_mutexattr_init FAIL");
    }
    
    pthread_mutexattr_settype(&_poolMutexAttr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&_poolMutex, &_poolMutexAttr);
    
    _releasePoolStack.reserve(10);
    _addPool =  new AutoreleasePool("cocos2d autorelease pool1");
    _releasePool = new AutoreleasePool("cocos2d autorelease pool2");
    push(_addPool);
    push(_releasePool);
}

PoolManager::~PoolManager()
{
    Ref::enableAutorelease(false);
    CCLOGINFO("deallocing PoolManager: %p", this);
    //zym 2016.1.5 先关闭ios的析构
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    clearAll(); // 防止删除的时候再有进入的数据，如PopupBaseView::~PopupBaseView
    clearAll(); // 多清一次
    _isClearing = true;
    while (!_releasePoolStack.empty())
    {
        AutoreleasePool* pool = _releasePoolStack.back();
        _releasePoolStack.pop_back();
        pool->clear();
        delete pool;
    }
#endif
    pthread_mutexattr_destroy( &_poolMutexAttr);
    pthread_mutex_destroy(&_poolMutex);
}


AutoreleasePool* PoolManager::getCurrentPool() const
{
    return _releasePool;
}

bool PoolManager::isObjectInPools(Ref* obj) const
{
    for (const auto& pool : _releasePoolStack)
    {
        if (pool->contains(obj))
            return true;
    }
    return false;
}

void PoolManager::push(AutoreleasePool *pool)
{
    _releasePoolStack.push_back(pool);
}

void PoolManager::pop()
{
    CC_ASSERT(!_releasePoolStack.empty());
    _releasePoolStack.pop_back();
}

void PoolManager::clearAll()                        // 把两个池都清掉掉
{
    pthread_mutex_lock(&_poolMutex);
    _isClearing = true;
    _addPool->clear();
    _releasePool->clear();
    _isClearing = false;
    pthread_mutex_unlock(&_poolMutex);
}
    // 只清除_releasePool,然后与_addPool交换位置
void PoolManager::autoReleasePool()       
{
    pthread_mutex_lock(&_poolMutex);
    _isClearing = true;
    _releasePool->clear();
    // 交换位置
    AutoreleasePool* pool  = _addPool;
    _addPool = _releasePool;
    _releasePool = pool;
    //#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    _isClearing = false;
    //#endif
    pthread_mutex_unlock(&_poolMutex);
}

void PoolManager::addObject(Ref* obj)
{
     pthread_mutex_lock(&_poolMutex);
    _addPool->addObject(obj);
     pthread_mutex_unlock(&_poolMutex);
}


void  PoolManager::removeObject(Ref* obj)
{
    pthread_mutex_lock(&_poolMutex);
    for (const auto& pool : _releasePoolStack)
    {
        pool->removeObject(obj);
    }
    pthread_mutex_unlock(&_poolMutex);
}

NS_CC_END
