--Create an class.
function class(classname, super)
    local superType = type(super)
    local cls

    if superType ~= "function" and superType ~= "table" then
        superType = nil
        super = nil
    end

    if superType == "function" or (super and super.__ctype == 1) then
        -- inherited from native C++ Object
        cls = {}

        if superType == "table" then
            -- copy fields from super
            for k,v in pairs(super) do cls[k] = v end
            cls.__create = super.__create
            cls.super    = super
        else
            cls.__create = super
        end

        cls.ctor    = function() end
        cls.__cname = classname
        cls.__ctype = 1

        function cls.new(...)
            --print "----123"
            local instance = cls.__create(...)
            --print "-----124"
            -- copy fields from class to native object
            for k,v in pairs(cls) do instance[k] = v end
            --print "-----125"
            instance.class = cls
            --print "-----126"
            instance:ctor(...)
            --print "-----127"
            return instance
        end

    else
        -- inherited from Lua Object
        if super then
            cls = clone(super)
            cls.super = super
        else
            cls = {ctor = function() end}
        end

        cls.__cname = classname
        cls.__ctype = 2 -- lua
        cls.__index = cls

        function cls.new(...)
            local instance = setmetatable({}, cls)
            instance.class = cls
            instance:ctor(...)
            return instance
        end
    end

    return cls
end

function schedule(node, callback, delay)
    --print "function schedule(node, callback, delay)"
    --print "delay = cc.DelayTime:create(delay)"
    local delay = cc.DelayTime:create(delay)
    --print "local callfunc = cc.CallFunc:create(callback)"
    local callfunc = cc.CallFunc:create(callback)
    --print "local sequence = cc.Sequence:initWithTwoActions(delay, callfunc)"
    local sequence = cc.Sequence:create(delay, callfunc)
    --print "local action = cc.RepeatForever:create(sequence)"
    local action = cc.RepeatForever:create(sequence)
    --print "node:runAction(action)"
    node:runAction(action)
    --print "return action"
    return action
end

function performWithDelay(node, callback, delay)
    local delay = cc.DelayTime:create(delay)
    local callfunc = cc.CallFunc:create(callback)
    local sequence = cc.Sequence:create(delay, callfunc)
    node:runAction(sequence)
    return sequence
end

function delayDealWithFunc(...)   
    --print "function delayDealWithFunc(...)"            
    local args = ...
    local ret = function ()
        --print "local ret = function ()"
        args.target:callbackFunc()
    end
    return ret        
end
function scheduleDealWithFunc( ... )
    --print "function scheduleDealWithFunc( ... )"
    local args = ...
    local ret = function ()
        --print "local ret = function ()"
        args.target:scheduleBack()
    end
    return ret
end
function isTouchInside( node, x, y )
    if nil == node then return false end
    if nil == node:getParent() then return false end
    local pos = node:getParent():convertToNodeSpace(cc.p(x, y))
    local rect = node:getBoundingBox()
    if cc.rectContainsPoint(rect, pos) == true then 
        return true
    else
        return false
    end
end