require "externCOK2"
require "commonCOK2"
require ("revive.Revive")

ReviveOpenLuaView  = ReviveOpenLuaView or {}
ccb["ReviveOpenLuaView"] = ReviveOpenLuaView

ReviveOpenView = class("ReviveOpenView",
    function()
        return PopupBaseView:create() 
    end
)
ReviveOpenView.__index = ReviveOpenView
function ReviveOpenView:create(path,params)
    local view = ReviveOpenView.new()
    if view:initView(path,params) == true then
        return view
    end
    return nil
end
function ReviveOpenView:initView(path,params)
    print("ReviveOpenView:initView____1")
    self.path = path
    self.params = params
    self.isKing = false
    self.btnState = 0
    self.openTime = 0
    if self:init(true,0) == false then
        return false
    end
    print("ReviveOpenView:initView____2")
    
    self:setHDPanelFlag(true)
    local strPath = string.format(path)
    self.rootPath = strPath .. "/revive"
    loadLuaResource(self.rootPath .. "/resources/revive.plist")
    
    local isPad = CCCommonUtilsForLua:isIosAndroidPad()
    print("ReviveOpenView:initView____3")
    local  proxy = cc.CCBProxy:create()
    ReviveOpenLuaView.onClickBtn1 = function()
        self:onClickBtn1()
    end
    ReviveOpenLuaView.onTipBtnClick = function()
        self:onTipBtnClick()
    end

    print("ReviveOpenView:initView____4")
    local ccbiURL = ""
    if isPad == true then
        ccbiURL = self.rootPath .. "/ccbi/ReviveOpenLuaViewPad.ccbi"
    else
        ccbiURL = self.rootPath .. "/ccbi/ReviveOpenLuaView.ccbi"
    end
    local nodeccb = CCBReaderLoad(ccbiURL,proxy,ReviveOpenLuaView)
    if nodeccb == nil then
        return false
    end
    print("ReviveOpenView:initView____5")
    if nil ~= ReviveOpenLuaView["m_topNode"] then
        self.m_topNode = tolua.cast(ReviveOpenLuaView["m_topNode"],"cc.Node")
    end
    if nil == self.m_topNode then
        return false
    end
    print("ReviveOpenView:initView____6")
    if nil ~= ReviveOpenLuaView["m_bottomNode"] then
        self.m_bottomNode = tolua.cast(ReviveOpenLuaView["m_bottomNode"],"cc.Node")
    end
    if nil == self.m_bottomNode then
        return false
    end
    if nil ~= ReviveOpenLuaView["m_sprTop"] then
        self.m_sprTop = tolua.cast(ReviveOpenLuaView["m_sprTop"],"cc.LayerColor")
    end
    if nil == self.m_sprTop then
        return false
    end
    if nil ~= ReviveOpenLuaView["m_closeNode"] then
        self.m_closeNode = tolua.cast(ReviveOpenLuaView["m_closeNode"],"cc.Node")
    end
    if nil == self.m_closeNode then
        return false
    end
    if nil ~= ReviveOpenLuaView["m_waitingNode"] then
        self.m_waitingNode = tolua.cast(ReviveOpenLuaView["m_waitingNode"],"cc.Node")
    end
    if nil == self.m_waitingNode then
        return false
    end
    print("ReviveOpenView:initView____7")
    if nil ~= ReviveOpenLuaView["m_lblTitle"] then
        self.m_lblTitle = tolua.cast(ReviveOpenLuaView["m_lblTitle"],"cc.Label")
    end
    if nil == self.m_lblTitle then
        return false
    end
    print("ReviveOpenView:initView____8")
    if nil ~= ReviveOpenLuaView["m_lbl1"] then
        self.m_lbl1 = tolua.cast(ReviveOpenLuaView["m_lbl1"],"cc.Label")
    end
    if nil == self.m_lbl1 then
        return false
    end
    print("ReviveOpenView:initView____9")
    if nil ~= ReviveOpenLuaView["m_lbl2"] then
        self.m_lbl2 = tolua.cast(ReviveOpenLuaView["m_lbl2"],"cc.Label")
    end
    if nil == self.m_lbl2 then
        return false
    end
    print("ReviveOpenView:initView____10")
    if nil ~= ReviveOpenLuaView["m_lbl3"] then
        self.m_lbl3 = tolua.cast(ReviveOpenLuaView["m_lbl3"],"cc.Label")
    end
    if nil == self.m_lbl3 then
        return false
    end
    print("ReviveOpenView:initView____11")
    if nil ~= ReviveOpenLuaView["m_lbl4"] then
        self.m_lbl4 = tolua.cast(ReviveOpenLuaView["m_lbl4"],"cc.Label")
    end
    if nil == self.m_lbl4 then
        return false
    end
    print("ReviveOpenView:initView____12")
    if nil ~= ReviveOpenLuaView["m_lbl5"] then
        self.m_lbl5 = tolua.cast(ReviveOpenLuaView["m_lbl5"],"cc.Label")
    end
    if nil == self.m_lbl5 then
        return false
    end
    print("ReviveOpenView:initView____13")
    if nil ~= ReviveOpenLuaView["m_lbl6"] then
        self.m_lbl6 = tolua.cast(ReviveOpenLuaView["m_lbl6"],"cc.Label")
    end
    if nil == self.m_lbl6 then
        return false
    end
    if nil ~= ReviveOpenLuaView["m_lbl7"] then
        self.m_lbl7 = tolua.cast(ReviveOpenLuaView["m_lbl7"],"cc.Label")
    end
    if nil == self.m_lbl7 then
        return false
    end
    print("ReviveOpenView:initView____14")
    if nil ~= ReviveOpenLuaView["m_btn1"] then
        self.m_btn1 = tolua.cast(ReviveOpenLuaView["m_btn1"],"cc.ControlButton")
    end
    if nil == self.m_btn1 then
        return false
    end
    print("ReviveOpenView:initView____15")
    if nil ~= ReviveOpenLuaView["m_btn1"] then
        self.m_btn1 = tolua.cast(ReviveOpenLuaView["m_btn1"],"cc.ControlButton")
    end
    if nil == self.m_btn1 then
        return false
    end
    print("ReviveOpenView:initView____16")
    if nil ~= ReviveOpenLuaView["m_btnTip"] then
        self.m_btnTip = tolua.cast(ReviveOpenLuaView["m_btnTip"],"cc.ControlButton")
    end
    if nil == self.m_btnTip then
        return false
    end
    for i=1,6 do
        print(i)
        local tmpName = "m_parnode" .. tostring(i)
        if nil ~= ReviveOpenLuaView[tmpName] then
            self[tmpName] = tolua.cast(ReviveOpenLuaView[tmpName],"cc.Node")
        end
        if nil == self[tmpName] then
            return false
        end
    end
    print("ReviveOpenView:initView____17")

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
    self.m_sprTop:setPositionY(self.m_sprTop:getPositionY() - changeY)
    
    print("ReviveOpenView:initView____19 [" .. tostring(changeY) .. " ]" )
    self.m_lblTitle:setString(LuaController:getLang("145032"))

    print("ReviveOpenView:initView____20")
    self:addChild(nodeccb)
    self.ccbNode = nodeccb
    
    self:addUIParticles()

    local function onNodeEvent(event)
        if event == "enter" then
            self:setTitleName(LuaController:getLang("145001"))
            self:refreshView()
            -- print "ReviveOpenView__enter"
        elseif event == "exit" then
            -- print "ReviveOpenView__exit"
            self:onExit()
        end
    end
    self.ccbNode:registerScriptHandler(onNodeEvent)
    print("ReviveOpenView:initView____21")
    -- CCCommonUtilsForLua:setButtonTitle(self.m_btn1,LuaController:getLang("145009"))
    return true
end
function ReviveOpenView:addUIParticles()
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
    if self.m_parnode3 ~= nil then
        local particles2 = LuaController:createParticleForLua(self.rootPath .. "/particles/UICandle_1")
        if nil ~= particles2 then
            self.m_parnode3:addChild(particles2)
            particles2:setPosition(0,0)
        end
    end
    if self.m_parnode6 ~= nil then
        local particles3 = LuaController:createParticleForLua(self.rootPath .. "/particles/UICandle_1")
        if nil ~= particles3 then
            self.m_parnode6:addChild(particles3)
            particles3:setPosition(0,0)
        end
    end
end
function ReviveOpenView:scheduleBack()
    -- body
    if self.openTime <= 0 then
        return
    end

    if self.m_lbl7 ~= nil then
        local nT = GlobalData:shared():getTimeStamp()
        local lastTime = self.openTime - nT
        print("ReviveOpenView:scheduleBack____ [ " .. tostring(lastTime) .. " ]")
        if lastTime > 0 then
            if self.isKing == false then
                local timeStr = LuaController:getSECLang(lastTime)
                self.m_lbl7:setString(LuaController:getLang1("145008",timeStr))
            end
        else
            LuaController:getInstance():sendCMD("cemetery.get.info")
            self.m_lbl7:setString("")
            self.m_btn1:setVisible(false)
        end
    end
end
function ReviveOpenView:setWaiting()
    print("ReviveOpenView:setWaiting____1")
    local officer =  GlobalData:shared():getPlayerInfo():getProperty("officer")
--test code
    -- officer = "216000"
--end
    if officer == "216000" then
        print("ReviveOpenView:setWaiting____1.1")
        self.isKing = true
        self.btnState = 1
        self.m_lbl7:setString("")
        CCCommonUtilsForLua:setButtonTitle(self.m_btn1,LuaController:getLang("145007"))
        self.m_btn1:setVisible(true)
        if nil ~=self.m_lbl7 then
            self.m_lbl7:stopAllActions()
            schedule(self.m_lbl7,scheduleDealWithFunc({target = self}),1) 
        end
    else
        print("ReviveOpenView:setWaiting____1.2")
        self.isKing = false
        self.btnState = 0
        self.m_btn1:setVisible(false)
        self:scheduleBack()
        if nil ~= self.m_lbl7 then
            self.m_lbl7:stopAllActions()
            schedule(self.m_lbl7,scheduleDealWithFunc({target = self}),1)  
        end
    end
    print("ReviveOpenView:setWaiting____2")
    self.m_lbl2:setString(LuaController:getLang("145003"))
    self.m_lbl3:setString(LuaController:getLang("145004"))
    self.m_lbl5:setString(LuaController:getLang("145005"))
    local totaldead = tonumber(CEMETERY_INFO["KINGDOM"]["totaldead"])
    self.m_lbl4:setString(LuaController:getCMDLang(tostring(totaldead)))
    if CEMETERY_INFO["PLAYER"]~= nil then
        local deadsum = tonumber(CEMETERY_INFO["PLAYER"]["deadsum"])
        self.m_lbl6:setString(LuaController:getCMDLang(tostring(deadsum)))
    else
        self.m_lbl6:setString("0")
    end
    self.m_waitingNode:setVisible(true)
    self.m_closeNode:setVisible(false)
    print("ReviveOpenView:setWaiting____3")
end
function ReviveOpenView:setToOpen()
    print("ReviveOpenView:setToOpen____1")
    self.btnState = 2
    self.m_lbl7:setString("")
    CCCommonUtilsForLua:setButtonTitle(self.m_btn1,LuaController:getLang("145009"))
    self.m_btn1:setVisible(true)
    if nil ~= self.m_lbl7 then
        self.m_lbl7:stopAllActions()
    end
    print("ReviveOpenView:setToOpen____2")
end
function ReviveOpenView:setClose()
    print("ReviveOpenView:setClose____1")
    self.btnState = 0
    self.m_waitingNode:setVisible(false)
    self.m_closeNode:setVisible(true)
    self.m_lbl1:setString(LuaController:getLang("145003"))
    print("ReviveOpenView:setClose____2")
end
function ReviveOpenView:refreshView()
    print("ReviveOpenView:refreshView_____1")
    local st = isInRevive()
    if st == 2 then
        -- 正常的复活状态
        print("ReviveOpenView:refreshView_____2")
        self.openTime = 0
        self.m_lbl2:setString(LuaController:getLang("145003"))
        self.m_lbl3:setString(LuaController:getLang("145004"))
        self.m_lbl5:setString(LuaController:getLang("145005"))
        local totaldead = tonumber(CEMETERY_INFO["KINGDOM"]["totaldead"])
        self.m_lbl4:setString(LuaController:getCMDLang(tostring(totaldead)))
        local deadsum = 0
        if CEMETERY_INFO["PLAYER"] ~= nil then
            deadsum = tonumber(CEMETERY_INFO["PLAYER"]["deadsum"])
        end
        self.m_lbl6:setString(LuaController:getCMDLang(tostring(deadsum)))
        self.m_waitingNode:setVisible(true)
        self.m_closeNode:setVisible(false)
        self:setToOpen()
        return
    end

    if st == 1 then
        print("ReviveOpenView:refreshView_____3")
        if CEMETERY_INFO["CD"] ~= nil then
            print("ReviveOpenView:refreshView_____3.1.1")
            self.openTime = tonumber(CEMETERY_INFO["CD"]["cemeteryOpenCD"])
            if self.openTime > 0 then
                local nT = GlobalData:shared():getTimeStamp()
                local lastTime = self.openTime - nT
                if lastTime > 0 then
                    print("ReviveOpenView:refreshView_____3.1.1.1")
                    self:setWaiting()
                else
                    print("ReviveOpenView:refreshView_____3.1.1.2")
                    self:setToOpen()
                end
                return
            end
        end
    end
    
    print("ReviveOpenView:refreshView_____4")
    self:setClose()   
end
function ReviveOpenView:onExit()
    ReviveOpenLuaView.onClickBtn1 = nil
    ReviveOpenLuaView.onTipBtnClick = nil

    if nil ~= self.ccbNode then
        self.ccbNode:unregisterScriptHandler()
    end
    if nil ~=self.m_lbl7 then
        self.m_lbl7:stopAllActions()
    end
    local targetPlatform = cc.Application:getInstance():getTargetPlatform()
    if (3 == targetPlatform) then
        releaseLuaResourceForAndroid(self.rootPath .. "/resources/" , "revive")
    else
        releaseLuaResource(self.rootPath .. "/resources/revive")
    end
end
function ReviveOpenView:onClickBtn1()
    print("ReviveOpenView:onClickBtn1____1")
    if self.btnState == 0 then
        return
    end
    print("ReviveOpenView:onClickBtn1____2")
    cc.UserDefault:getInstance():setIntegerForKey("__revive_open_key",1)
    cc.UserDefault:getInstance():flush()
    if self.btnState == 1 then
        print("ReviveOpenView:onClickBtn1____2.1")
        LuaController:getInstance():sendCMD("king.open.cemetery")
        CEMETERY_INFO["KINGDOM"]["revivestat"] = 1
        CEMETERY_INFO["KINGDOM"]["cemeterystat"] = 1
        self.btnState = 0
    elseif self.btnState == 2 then
        print("ReviveOpenView:onClickBtn1____2.2")
        self:goToReviveView()
    end
    print("ReviveOpenView:onClickBtn1____3")
end
function ReviveOpenView:goToReviveView()
    self.btnState = 0
    self:closeSelf()
    require "revive.ReviveView"
    local view = ReviveView:create(self.path)
    if view ~= nil then
        PopupViewController:getInstance():addPopupInView(view)
    end
end
function ReviveOpenView:onTipBtnClick()
    print("ReviveOpenView:onTipBtnClick")
end
function ReviveOpenView:CMDCallbackForLua(cmdname,data)
    -- body
    print("ReviveOpenView:CMDCallbackForLua")
    if cmdname == "cemetery.get.info" then
        self:refreshView()
    elseif cmdname == "king.open.cemetery" then
        local ref = tolua.cast(data,"cc.Ref")
        local tmpData = LuaController:getInstance():getDicLuaData(ref)
        
        if tonumber(tmpData["flag"]) == 1 then
            self:goToReviveView()
        end
    end
end
function ReviveOpenView:NotificationCallbackForLua(cmdname)
    -- body
    print("ReviveOpenView:NotificationCallbackForLua")
end
function ReviveOpenView:NotificationCallbackForLuaWithParams(cmdname,object)
    -- body
    print("ReviveOpenView:NotificationCallbackForLuaWithParams")
end