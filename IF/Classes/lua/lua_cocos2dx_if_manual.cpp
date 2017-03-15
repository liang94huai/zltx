/****************************************************************************
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
#include "lua_cocos2dx_if_manual.h"
#include "cocos2d.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"
#include "CCLuaValue.h"
#include "cocos-ext.h"
#include "CCLuaEngine.h"
#include "scripting/lua-bindings/manual/cocos2d/LuaScriptHandlerMgr.h"
#include "CCIFTouchNode.h"
#include "lua_cocos2dx_if_auto.hpp"

USING_NS_CC;
USING_NS_CC_EXT;
static int tolua_cocos2d_CCIFTouchNode_registerScriptTouchHandler(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;
    
    int argc = 0;
    CCIFTouchNode* self = nullptr;
    
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"cc.CCIFTouchNode",0,&tolua_err)) goto tolua_lerror;
#endif
    
    self = static_cast<CCIFTouchNode*>(tolua_tousertype(tolua_S,1,0));
    
#ifndef TOLUA_RELEASE
    if (nullptr == self) {
        tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2d_CCIFTouchNode_registerScriptTouchHandler'\n", NULL);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (argc >=1 && argc <= 4) {
#ifndef TOLUA_RELEASE
        if (!toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) {
            goto tolua_lerror;
        }
#endif
        LUA_FUNCTION handler = toluafix_ref_function(tolua_S,2,0);
        
        ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, ScriptHandlerMgr::HandlerType::TOUCHES);
        return 0;
    }
    
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "CCIFTouchNode:registerScriptTouchHandler", argc, 1);
    return 0;
    
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_CCIFTouchNode_registerScriptTouchHandler'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_CCIFTouchNode_unregisterScriptTouchHandler(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;
    
    int argc = 0;
    CCIFTouchNode* self = nullptr;
    
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"cc.CCIFTouchNode",0,&tolua_err)) goto tolua_lerror;
#endif
    
    self = static_cast<CCIFTouchNode*>(tolua_tousertype(tolua_S,1,0));
    
#ifndef TOLUA_RELEASE
    if (nullptr == self) {
        tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2d_CCIFTouchNode_unregisterScriptTouchHandler'\n", NULL);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (0 == argc)
    {
        auto dict = static_cast<__Dictionary*>(self->getUserObject());
        if (dict != nullptr)
        {
            auto touchListenerAllAtOnce = static_cast<EventListenerTouchAllAtOnce*>(dict->objectForKey("touchListenerAllAtOnce"));
            auto touchListenerOneByOne = static_cast<EventListenerTouchOneByOne*>(dict->objectForKey("touchListenerOneByOne"));
            auto dispatcher = self->getEventDispatcher();
            if (nullptr != dispatcher)
            {
                dispatcher->removeEventListener(touchListenerAllAtOnce);
                dispatcher->removeEventListener(touchListenerOneByOne);
            }
        }
        
        ScriptHandlerMgr::getInstance()->removeObjectHandler((void*)self, ScriptHandlerMgr::HandlerType::TOUCHES);
        return 0;
    }
    
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "CCIFTouchNode:unregisterScriptTouchHandler", argc, 0);
    return 0;
    
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2d_CCIFTouchNode_unregisterScriptTouchHandler'.",&tolua_err);
    return 0;
#endif
}


static void extendCCIFTouchNode(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.CCIFTouchNode");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"registerScriptTouchHandler");
        lua_pushcfunction(tolua_S,tolua_cocos2d_CCIFTouchNode_registerScriptTouchHandler );
        lua_rawset(tolua_S,-3);
        lua_pushstring(tolua_S,"unregisterScriptTouchHandler");
        lua_pushcfunction(tolua_S,tolua_cocos2d_CCIFTouchNode_unregisterScriptTouchHandler);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}


int register_all_cocos2dx_if_manual(lua_State* tolua_S)
{
    extendCCIFTouchNode(tolua_S);
    return 0;
}

int register_if_module(lua_State* tolua_S)
{
    lua_getglobal(tolua_S, "_G");
    if (lua_istable(tolua_S,-1))//stack:...,_G,
    {
        register_all_cocos2dx_if(tolua_S);
        register_all_cocos2dx_if_manual(tolua_S);
    }
    lua_pop(tolua_S, 1);
    return 1;
}
