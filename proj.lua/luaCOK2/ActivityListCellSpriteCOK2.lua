require "externCOK2"
require "CCBReaderLoadCOK2"
require "commonCOK2"

ActivityListCellSprite = class("ActivityListCellSprite", function() return cc.Layer:create() end)
ActivityListCellSprite.__index = ActivityListCellSprite

function ActivityListCellSprite:create( path, params)
    --print "---3----"
    local node = ActivityListCellSprite.new()
    --print "---4----"
    node:init(path, params)
    return node
end

function ActivityListCellSprite:init( path, params )
    --print "---5----"
    self.rootPath = string.format(path)
    --print "---6----"
    self.params = params
    --print "---7----"
    loadLuaResource(self.rootPath .. "/resources/activity_" .. self.params.id .. ".plist")
    --print "---8----"
    local frame = cc.SpriteFrameCache:getInstance():getSpriteFrame("activity_" .. self.params.id .. "_list_cell_head.png");
    --print "---9----"
    if nil ~= frame then
        --print "---10----"
        local sprite = cc.Sprite:createWithSpriteFrame(frame)
        --print "---11----"
        local function eventHandler( eventType )
            --print "---12----"
            if eventType == "enter" then
                --print "enter"
            elseif eventType == "exit" then
                --print "exit"
            elseif eventType == "cleanup" then
                --print "cleanup"
                --print "---13----"
                releaseLuaResource(self.rootPath .. "/resources/activity_" .. self.params.id)
                --print "---14----"
                sprite:unregisterScriptHandler()
            end
        end
        --print "---15----"
        sprite:registerScriptHandler(eventHandler)
        --print "---16----"
        self:addChild(sprite)
        --print "---17----"
    end
end