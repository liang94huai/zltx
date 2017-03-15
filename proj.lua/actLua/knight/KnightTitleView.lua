require "externCOK2"
require "commonCOK2"
require ("knight.KnightTitleInfo")
require ("knight.KnightTitleController")

KnightTitleLuaView  = KnightTitleLuaView or {}
ccb["KnightTitleLuaView"] = KnightTitleLuaView

KnightTitleView = class("KnightTitleView",
    function()
        return PopupBaseView:create() 
    end
)
KnightTitleView.__index = KnightTitleView

function KnightTitleView:create(path)
    local view = KnightTitleView.new()
    if view:initView(path) == true then
        return view
    end
    return nil
end

function KnightTitleView:initView(path)
    print("KnightTitleView:initView____1")
    self.path = path
    if self:init(true,0) == false then
        return false
    end
    print("KnightTitleView:initView____2")
    self.isPlayerRevive=false
    self.reviveTime = 0
    self.btn2State = 0
    self.blessTime = 0
    self.isClose = true
    self:setHDPanelFlag(true)
    local strPath = string.format(path)
    self.rootPath = strPath .. "/knight"
    -- loadLuaResource(self.rootPath .. "/resources/revive.plist")

    local isPad = CCCommonUtilsForLua:isIosAndroidPad()

    print("KnightTitleView:initView____3")

    local  proxy = cc.CCBProxy:create()
    -- ReviveLuaView.onClickBtn1 = function()
    --     self:onClickBtn1()
    -- end
    print("KnightTitleView:initView____fb1")
    self:onResolveCCBCCControlSelector()
    

    print("KnightTitleView:initView____4")
    local ccbiURL = ""
    if isPad == false then
        ccbiURL = self.rootPath .. "/ccbi/KnightTitileLuaView.ccbi"
        print("KnightTitleView:initView____fb  ccb")
    else
        ccbiURL = self.rootPath .. "/ccbi/ReviveLuaViewPad.ccbi"
    end
    local nodeccb = CCBReaderLoad(ccbiURL,proxy,KnightTitleLuaView)
    if nodeccb == nil then
        return false
    end

    print("KnightTitleView:initView____5")
    if false == self:onAssignCCBMemberVariable() then
        print("KnightTitleView:initView____false")
        return false
    end

    
    local screenSize = cc.Director:getInstance():getWinSize()
    local changeY = 0
    if isPad == true then
        self:setContentSize(cc.size(1536, 2048))
        changeY = screenSize.height - 2048
    else
        self:setContentSize(cc.size(640, 852))
        changeY = screenSize.height - 852
    end

    self.m_bottomNode:setPositionY(self.m_bottomNode:getPositionY() - changeY)
    
    CCCommonUtilsForLua:setButtonTitle(self.m_composeBtn, LuaController:getLang("145001"))

    print("KnightTitleView:initView____20")
    self:addChild(nodeccb)
    self.ccbNode = nodeccb
    
    -- self:addUIParticles()
    
    local function onNodeEvent(event)
        if event == "enter" then
            self:onExit()
        elseif event == "exit" then
            self:onExit()
        end
    end
    self.ccbNode:registerScriptHandler(onNodeEvent)
    print("KnightTitleView:initView____21")

    -- schedule(self.m_bottomNode,scheduleDealWithFunc({target = self}),1) 
    return true
end

function KnightTitleView:onEnter()
    self:setTitleName(LuaController:getLang("145001"))
    self.playerInfo = GlobalData:shared():getPlayerInfo()
    self.isClose = true

    -- self:refreshView()
end

function KnightTitleView:onExit()
    if self.isClose == false then
        return
    end

    KnightTitleLuaView.onClickListBtn = nil
    KnightTitleLuaView.onClickComposeBtn = nil
    if nil ~=self.m_bottomNode then
        self.m_bottomNode:stopAllActions()
    end
    if nil ~= self.ccbNode then
        self.ccbNode:unregisterScriptHandler()
    end
    -- local targetPlatform = cc.Application:getInstance():getTargetPlatform()
    -- if (3 == targetPlatform) then
    --     releaseLuaResourceForAndroid(self.rootPath .. "/resources/" , "revive")
    -- else
    --     releaseLuaResource(self.rootPath .. "/resources/revive")
    -- end
end

function KnightTitleView:onResolveCCBCCControlSelector()
    print("KnightTitleView:initView____fb2")
    KnightTitleLuaView.onClickListBtn = function()
        self:onClickListBtn()
    end
    print("KnightTitleView:initView____fb3")
    KnightTitleLuaView.onClickComposeBtn = function()
        self:onClickComposeBtn()
    end
end

function KnightTitleView:onAssignCCBMemberVariable()
    if nil ~= KnightTitleLuaView["m_buildBG"] then
        self.m_buildBG = tolua.cast(KnightTitleLuaView["m_buildBG"],"cc.Scale9Sprite")
    end
    if nil == self.m_buildBG then
        print("KnightTitleView:initView____f1")
        return false
    end

    if nil ~= KnightTitleLuaView["m_listBtn"] then
        self.m_listBtn = tolua.cast(KnightTitleLuaView["m_listBtn"],"cc.ControlButton")
    end
    if nil == self.m_listBtn then
        print("KnightTitleView:initView____f2")
        return false
    end

    if nil ~= KnightTitleLuaView["m_composeBtn"] then
        self.m_composeBtn = tolua.cast(KnightTitleLuaView["m_composeBtn"],"cc.ControlButton")
    end
    if nil == self.m_composeBtn then
        print("KnightTitleView:initView____f3")
        return false
    end

    if nil ~= KnightTitleLuaView["m_bottomNode"] then
        self.m_bottomNode = tolua.cast(KnightTitleLuaView["m_bottomNode"],"cc.Node")
    end
    if nil == self.m_bottomNode then
        print("KnightTitleView:initView____f4")
        return false
    end

    -- if nil ~= ReviveLuaView["m_infoList"] then
    --     self.m_infoList = tolua.cast(ReviveLuaView["m_infoList"],"cc.Node")
    -- end
    -- if nil == self.m_infoList then
    --     return false
    -- end

    -- for i=1,9 do
    --     print(i)
    --     local tmpName = "m_parnode" .. tostring(i)
    --     if nil ~= ReviveLuaView[tmpName] then
    --         self[tmpName] = tolua.cast(ReviveLuaView[tmpName],"cc.Node")
    --     end
    --     if nil == self[tmpName] then
    --         return false
    --     end
    -- end

    -- if nil ~= ReviveLuaView["m_lblProp"] then
    --     self.m_lblProp = tolua.cast(ReviveLuaView["m_lblProp"],"cc.Label")
    -- end
    -- if nil == self.m_lblProp then
    --     return false
    -- end
end

function KnightTitleView:addUIParticles()
    for i=1,6 do
        print(i)
        local tmpName = "m_parnode" .. tostring(i)
        if self[tmpName] ~= nil then
            local particles1 = LuaController:createParticleForLua(self.rootPath .. "/particles/UICandle_2")
            if nil ~= particles1 then
                self[tmpName]:addChild(particles1)
                particles1:setPosition(0,0)
            end
        end
    end
    if self.m_parnode7 ~= nil then
        for i=0,2 do
            print(i)
            local fileName = self.rootPath .. "/particles/CrossServerRelive_" .. tostring(i)
            local par = LuaController:createParticleForLua(fileName)
            if nil ~= par then
                self.m_parnode7:addChild(par)
                par:setPosition(0,0)
            end
        end
    end
end

function KnightTitleView:scheduleBack()
    if self.reviveTime > 0 then
        if self.m_lblleft1 ~= nil then
            local nT = GlobalData:shared():getTimeStamp()
            local lastTime = self.reviveTime - nT
            -- print("ReviveView:scheduleBack____ [ " .. tostring(lastTime) .. " ]")
            if lastTime > 0 then
                local timeStr = LuaController:getSECLang(lastTime)
                self.m_lblleft1:setString(LuaController:getLang1("145016",timeStr))
            else
                -- LuaController:getInstance():sendCMD("cemetery.get.info")
                print("—————————— auto revive")
                self.m_lblleft1:setString("")
                CEMETERY_INFO["KINGDOM"]["revivestat"] = 3
                self:refreshLeftNode()
            end
        end
    end

    if self.blessTime > 0 then
        if self.m_btn1 ~= nil then
            local nowTime = GlobalData:shared():getTimeStamp()
            local passTime = self.blessTime - nowTime
            if passTime > 0 then
                local leftT = LuaController:getSECLang(passTime)
                CCCommonUtilsForLua:setButtonTitle(self.m_btn1,leftT)
            else
                print("—————————— clear bless cd")
                self:refreshBtn1()
            end
        end
    end
end

function KnightTitleView:setLeftTime()
    self:scheduleBack()
    if nil ~=self.m_lblleft1 then
        self.m_lblleft1:setAnchorPoint(cc.p(0.5,0.5))
        self.m_lblleft1:setPositionY(225)
    end
end

function KnightTitleView:onClickListBtn()
    print("ReviveView:refreshBtn1____1")
end

function KnightTitleView:onClickComposeBtn()
    print("ReviveView:refreshBtn2____1") 
end

function KnightTitleView:refreshView()
    print("ReviveView:refreshView_____1")
end



function KnightTitleView:onClickBtn1()
    print("ReviveView:onClickBtn1")
    if self.officer == "216000" then
        local revivestat = tonumber(CEMETERY_INFO["KINGDOM"]["revivestat"]) 
        if revivestat == 2 then
            self:onClickBtnLeft()
            return
        end
    end
    if self.blessTime == 0 then
        self.m_btn1:setEnabled(false)
        LuaController:getInstance():sendCMD("cemetery.bless")
    end
end

function KnightTitleView:onClickBtn2()
    print("ReviveView:onClickBtn2")
    if self.btn2State == 1 then
        self:onClickBtnRight()
    else
        local propId = 200066
        local tInfo = ToolController:getInstance():getToolInfoForLua(propId)
        if tInfo == nil then
            self.m_btn2:setVisible(false)
            return
        end
        local cnt = tInfo:getCNT()
        if cnt > 0 then
            LuaController:getInstance():sendCMD("cemetery.player.contribution")
        else
            LuaController:openBagView(1)
        end
    end
end

function KnightTitleView:onClickBtnRank1()
    print("ReviveView:onClickBtnRank1")
    self.isClose=false
    require "/revive/ReviveRankView"
    local view1 = ReviveRankView:create(self.path,2)
    if view1 ~= nil then
        PopupViewController:getInstance():addPopupInView(view1)
    end
end

function KnightTitleView:onClickBtnRank2()
    print("ReviveView:onClickBtnRank2")
    self.isClose=false
    require "/revive/ReviveRankView"
    local view1 = ReviveRankView:create(self.path,1)
    if view1 ~= nil then
        PopupViewController:getInstance():addPopupInView(view1)
    end
end

function KnightTitleView:onClickBtnRight()
    print("ReviveView:onClickBtnRight")
    if self.isPlayerRevive == false then
        self.isPlayerRevive = true
        LuaController:getInstance():sendCMD("cemetery.player.click.revive")
    end
    self.btn2State = 0
end

function KnightTitleView:onClickBtnLeft()
    print("ReviveView:onClickBtnLeft")
    LuaController:getInstance():sendCMD("cemetery.king.revive")
    self.m_leftNode1:setVisible(false)
    self.m_lblleft1:setVisible(false)
    self.m_lblleft2:setVisible(false)
    self.m_lblleft5:setVisible(true)
end

function KnightTitleView:CMDCallbackForLua(cmdname,params)
    -- body
    print("ReviveView:CMDCallbackForLua")
    if cmdname == "cemetery.get.info" then
        self:refreshView()
    elseif cmdname == "cemetery.bless" then
        self:refreshView()
    elseif cmdname == "cemetery.king.revive" then
        local ref = tolua.cast(params,"cc.Ref")
        local tmpData = LuaController:getInstance():getDicLuaData(ref)
        if tonumber(tmpData["success"]) == 1 then
            CEMETERY_INFO["CD"]["deadReviveCD"] = -1
            CEMETERY_INFO["KINGDOM"]["revivestat"] = 3
            self:refreshView()
        end
    elseif cmdname == "cemetery.player.contribution" then
        local propId = 200066
        local tInfo = ToolController:getInstance():getToolInfoForLua(propId)
        if tInfo ~= nil then
            local cnt = tInfo:getCNT()
            if cnt > 0 then
                cnt = cnt - 1
                tInfo:setCNT(cnt)
            end
        end
        self:refreshView()
    elseif cmdname == "cemetery.player.click.revive" then
        self:refreshView()
        self.isPlayerRevive=false
    end
end

function KnightTitleView:NotificationCallbackForLua(cmdname)
    -- body
    print("ReviveView:NotificationCallbackForLua")
end
function KnightTitleView:NotificationCallbackForLuaWithParams(cmdname,object)
    -- body
    print("ReviveView:NotificationCallbackForLuaWithParams")
end