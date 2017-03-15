require "externCOK2"
require "commonCOK2"

ActivityRwdShowCell = class("ActivityRwdShowCell",
    function()
        return cc.Node:create() 
    end
)
ActivityRwdShowCell.__index = ActivityRwdShowCell
function ActivityRwdShowCell:create(params)
    local node = ActivityRwdShowCell.new()
    node:initView(params)
    return node
end
function ActivityRwdShowCell:initView(params)
    self.data = params
    if self:init() == true then
        local bg = CCLoadSprite:createSprite("icon_kuang.png")
        CCCommonUtilsForLua:setSpriteMaxSize(bg,75)
        self:addChild(bg)
        CCCommonUtilsForLua:createGoodsIcon(tonumber(params),self,cc.size(68,68))
        if CCCommonUtilsForLua:isIosAndroidPad()==true then
             self:setScale(2.0)
        end
    end
end