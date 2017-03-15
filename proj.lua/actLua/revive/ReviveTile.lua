require "externCOK2"
require "commonCOK2"
require ("revive.Revive")

ReviveTile = class("ReviveTile",
    function()
        return NewBaseTileLuaInfo:create() 
    end
)
ReviveTile.__index = ReviveTile
function ReviveTile:create(path,params)
    local view = ReviveTile.new()
    if view:initView(path,params) == true then
        return view
    end
    return nil
end
function ReviveTile:initView(path,params)
    LuaController:getInstance():sendCMD("cemetery.get.info")

    print("ReviveTile:initView__1")
	self.path = path
    self.params = params
    self:setCityInfo(params)
    self.funcTable = {}
    if self:initTile(true) == false then
        return false
    end
    print("ReviveTile:initView__2 [ " .. tostring(MMM) .. " ]")
    local cityName = self.params:getProperty("cityName")
    print("ReviveTile:initView__3 [" .. cityName .. " ]")
    self:setTitleString(LuaController:getLang(cityName))
    local funcIdx = 1
    print("ReviveTile:initView__5 [0]")
    self:setButtonCount(1)
    self:setButtonName(1,LuaController:getLang("145002"))
    self:setButtonState(1,1001)
    local function infofun()
        -- body
        local chechV = "2.0.5"
        if CCCommonUtilsForLua:checkVersion(chechV) == false then
            local function confirmFun()
                print("confirmFun click")
            end
            local tip = LuaController:getLang("145035")
            local fun = cc.CallFunc:create(confirmFun)
            YesNoDialog:show(tip,fun)
            
            self:closeTile()
            return
        end
        print("ReviveTile:initView__5 [clickfun_callback_fun]")
        local rs = isInRevive()
        if rs == 2 then   
            print("ReviveTile:initView__5.2")
            local officer =  GlobalData:shared():getPlayerInfo():getProperty("officer")
            if officer == "216000" then --国王
                print("ReviveTile:initView__5.2.1")
                self:showReviveView()
            else
                print("ReviveTile:initView__5.2.2")
                local savekey = cc.UserDefault:getInstance():getIntegerForKey("__revive_open_key")
                print("ReviveTile:initView__5.2.2.1   [ " .. tostring(savekey) .. " ]")
                if savekey == 1 then
                    print("ReviveTile:initView__5.2.3.1")
                    self:showReviveView()
                else
                    print("ReviveTile:initView__5.2.3.2")
                    self:showReviveOpenView()
                end
            end
        else
            cc.UserDefault:getInstance():setIntegerForKey("__revive_open_key",0)
            print("ReviveTile:initView__5.3")
            self:showReviveOpenView()
        end
        print("ReviveTile:initView__5.6")
        -- self.funcTable[1]:release()
        self:closeTile()
    end
    local funInfo = cc.CallFunc:create(infofun)
    funInfo:retain()
    self:setButtonCallback(1,funInfo)
    self.funcTable[funcIdx] = funInfo
    funcIdx = funcIdx + 1
    print("ReviveTile:initView__5 [22]")
    self:addToParent()
    print("ReviveTile:initView__5 [23]")
    return true
end
function ReviveTile:showReviveView()
    require "revive.ReviveView"
    local view = ReviveView:create(self.path)
    if view ~= nil then
        PopupViewController:getInstance():addPopupInView(view)
    end
end
function ReviveTile:showReviveOpenView()
    require "revive.ReviveOpenView"
    local view1 = ReviveOpenView:create(self.path,self.params)
    if view1 ~= nil then
        PopupViewController:getInstance():addPopupInView(view1)
    end
end
function ReviveTile:closeTile()
    -- body
    -- for i,v in ipairs(self.funcTable) do
    --     print(i,v)
    --     if v ~= nil then
    --         v:release()
    --     end
    -- end
    self.funcTable = nil
    self:closeThis()
end
function ReviveTile:CMDCallbackForLua(cmdname,data)
    -- body
    print("ReviveTile:CMDCallbackForLua")
end
function ReviveTile:NotificationCallbackForLua(cmdname)
    -- body
    print("ReviveTile:NotificationCallbackForLua")
end
function ReviveTile:NotificationCallbackForLuaWithParams(cmdname,object)
    -- body
    print("ReviveTile:NotificationCallbackForLuaWithParams")
end