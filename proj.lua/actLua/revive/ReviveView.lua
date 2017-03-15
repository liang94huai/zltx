require "externCOK2"
require "commonCOK2"

ReviveLuaView  = ReviveLuaView or {}
ccb["ReviveLuaView"] = ReviveLuaView

ReviveView = class("ReviveView",
    function()
        return PopupBaseView:create() 
    end
)
ReviveView.__index = ReviveView
function ReviveView:create(path)
    local view = ReviveView.new()
    if view:initView(path) == true then
        return view
    end
    return nil
end
function ReviveView:initView(path)
    print("ReviveView:initView____1")
    self.path = path
    if self:init(true,0) == false then
        return false
    end
    print("ReviveView:initView____2")
    self.isPlayerRevive=false
    self.reviveTime = 0
    self.btn2State = 0
    self.blessTime = 0
    self.isClose = true
    self.tickCnt = 1
    self:setHDPanelFlag(true)
    local strPath = string.format(path)
    self.rootPath = strPath .. "/revive"
    loadLuaResource(self.rootPath .. "/resources/revive.plist")

    local isPad = CCCommonUtilsForLua:isIosAndroidPad()

    print("ReviveView:initView____3")
    local  proxy = cc.CCBProxy:create()
    ReviveLuaView.onClickBtn1 = function()
        self:onClickBtn1()
    end
    ReviveLuaView.onClickBtn2 = function()
        self:onClickBtn2()
    end
    ReviveLuaView.onClickBtnRank1 = function()
        self:onClickBtnRank1()
    end
    ReviveLuaView.onClickBtnRank2 = function()
        self:onClickBtnRank2()
    end
    ReviveLuaView.onClickBtnRight = function()
        self:onClickBtnRight()
    end
    ReviveLuaView.onClickBtnLeft = function()
        self:onClickBtnLeft()
    end

    print("ReviveView:initView____4")
    local ccbiURL = ""
    if isPad == false then
        ccbiURL = self.rootPath .. "/ccbi/ReviveLuaView.ccbi"
    else
        ccbiURL = self.rootPath .. "/ccbi/ReviveLuaViewPad.ccbi"
    end
    local nodeccb = CCBReaderLoad(ccbiURL,proxy,ReviveLuaView)
    if nodeccb == nil then
        return false
    end
    print("ReviveView:initView____5")
    if nil ~= ReviveLuaView["m_topNode"] then
        self.m_topNode = tolua.cast(ReviveLuaView["m_topNode"],"cc.Node")
    end
    if nil == self.m_topNode then
        return false
    end
    print("ReviveView:initView____6")
    if nil ~= ReviveLuaView["m_bottomNode"] then
        self.m_bottomNode = tolua.cast(ReviveLuaView["m_bottomNode"],"cc.Node")
    end
    if nil == self.m_bottomNode then
        return false
    end
    if nil ~= ReviveLuaView["m_sprTop"] then
        self.m_sprTop = tolua.cast(ReviveLuaView["m_sprTop"],"cc.LayerColor")
    end
    if nil == self.m_sprTop then
        return false
    end
    print("ReviveView:initView____7")
    if nil ~= ReviveLuaView["m_lblTitle"] then
        self.m_lblTitle = tolua.cast(ReviveLuaView["m_lblTitle"],"cc.Label")
    end
    if nil == self.m_lblTitle then
        return false
    end
    print("ReviveView:initView____8")
    if nil ~= ReviveLuaView["m_lblleft1"] then
        self.m_lblleft1 = tolua.cast(ReviveLuaView["m_lblleft1"],"cc.Label")
    end
    if nil == self.m_lblleft1 then
        return false
    end
    print("ReviveView:initView____9")
    if nil ~= ReviveLuaView["m_lblleft2"] then
        self.m_lblleft2 = tolua.cast(ReviveLuaView["m_lblleft2"],"cc.Label")
    end
    if nil == self.m_lblleft2 then
        return false
    end
    if nil ~= ReviveLuaView["m_lblleft3"] then
        self.m_lblleft3 = tolua.cast(ReviveLuaView["m_lblleft3"],"cc.Label")
    end
    if nil == self.m_lblleft3 then
        return false
    end
    if nil ~= ReviveLuaView["m_lblleft4"] then
        self.m_lblleft4 = tolua.cast(ReviveLuaView["m_lblleft4"],"cc.Label")
    end
    if nil == self.m_lblleft4 then
        return false
    end
    if nil ~= ReviveLuaView["m_lblleft5"] then
        self.m_lblleft5 = tolua.cast(ReviveLuaView["m_lblleft5"],"cc.Label")
    end
    if nil == self.m_lblleft5 then
        return false
    end
    print("ReviveView:initView____10")
    if nil ~= ReviveLuaView["m_lblright1"] then
        self.m_lblright1 = tolua.cast(ReviveLuaView["m_lblright1"],"cc.Label")
    end
    if nil == self.m_lblright1 then
        return false
    end
    print("ReviveView:initView____11")
    if nil ~= ReviveLuaView["m_lblright2"] then
        self.m_lblright2 = tolua.cast(ReviveLuaView["m_lblright2"],"cc.Label")
    end
    if nil == self.m_lblright2 then
        return false
    end
    if nil ~= ReviveLuaView["m_lblright3"] then
        self.m_lblright3 = tolua.cast(ReviveLuaView["m_lblright3"],"cc.Label")
    end
    if nil == self.m_lblright3 then
        return false
    end
    if nil ~= ReviveLuaView["m_lblright4"] then
        self.m_lblright4 = tolua.cast(ReviveLuaView["m_lblright4"],"cc.Label")
    end
    if nil == self.m_lblright4 then
        return false
    end
    if nil ~= ReviveLuaView["m_lblright5"] then
        self.m_lblright5 = tolua.cast(ReviveLuaView["m_lblright5"],"cc.Label")
    end
    if nil == self.m_lblright5 then
        return false
    end
    if nil ~= ReviveLuaView["m_leftNode1"] then
        self.m_leftNode1 = tolua.cast(ReviveLuaView["m_leftNode1"],"cc.Node")
    end
    if nil == self.m_leftNode1 then
        return false
    end
    if nil ~= ReviveLuaView["m_rightNode1"] then
        self.m_rightNode1 = tolua.cast(ReviveLuaView["m_rightNode1"],"cc.Node")
    end
    if nil == self.m_rightNode1 then
        return false
    end
    if nil ~= ReviveLuaView["m_bgLeft"] then
        self.m_bgLeft = tolua.cast(ReviveLuaView["m_bgLeft"],"ccui.Scale9Sprite")
    end
    if nil == self.m_bgLeft then
        return false
    end
    if nil ~= ReviveLuaView["m_bgRight"] then
        self.m_bgRight = tolua.cast(ReviveLuaView["m_bgRight"],"ccui.Scale9Sprite")
    end
    if nil == self.m_bgRight then
        return false
    end
    print("ReviveView:initView____14")
    if nil ~= ReviveLuaView["m_btn1"] then
        self.m_btn1 = tolua.cast(ReviveLuaView["m_btn1"],"cc.ControlButton")
    end
    if nil == self.m_btn1 then
        return false
    end
    print("ReviveView:initView____15")
    if nil ~= ReviveLuaView["m_btn2"] then
        self.m_btn2 = tolua.cast(ReviveLuaView["m_btn2"],"cc.ControlButton")
    end
    if nil == self.m_btn2 then
        return false
    end
    print("ReviveView:initView____16")
    if nil ~= ReviveLuaView["m_btnrank1"] then
        self.m_btnrank1 = tolua.cast(ReviveLuaView["m_btnrank1"],"cc.ControlButton")
    end
    if nil == self.m_btnrank1 then
        return false
    end
    print("ReviveView:initView____17")
    if nil ~= ReviveLuaView["m_btnrank2"] then
        self.m_btnrank2 = tolua.cast(ReviveLuaView["m_btnrank2"],"cc.ControlButton")
    end
    if nil == self.m_btnrank2 then
        return false
    end
    print("ReviveView:initView____18")
    for i=1,9 do
    	print(i)
    	local tmpName = "m_parnode" .. tostring(i)
    	if nil ~= ReviveLuaView[tmpName] then
	        self[tmpName] = tolua.cast(ReviveLuaView[tmpName],"cc.Node")
	    end
	    if nil == self[tmpName] then
	        return false
	    end
    end
    if nil ~= ReviveLuaView["m_nodeProp"] then
        self.m_nodeProp = tolua.cast(ReviveLuaView["m_nodeProp"],"cc.Node")
    end
    if nil == self.m_nodeProp then
        return false
    end
    if nil ~= ReviveLuaView["m_lblProp"] then
        self.m_lblProp = tolua.cast(ReviveLuaView["m_lblProp"],"cc.Label")
    end
    if nil == self.m_lblProp then
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
    self.m_sprTop:setPositionY(self.m_sprTop:getPositionY() - changeY)
    print("ReviveView:initView____19 [" .. tostring(changeY) .. " ]" )
    self.m_lblTitle:setString(LuaController:getLang("145032"))
    self.m_lblleft1:setString(LuaController:getLang("145030"))
    self.m_lblleft3:setString(LuaController:getLang("145012"))
    self.m_lblright1:setString(LuaController:getLang("145031"))
    self.m_lblright3:setString(LuaController:getLang("145012"))
    -- CCCommonUtilsForLua:setButtonTitle(self.m_btn2,LuaController:getLang("145013"))   
    CCCommonUtilsForLua:setButtonTitle(self.m_btnrank1,"")
    CCCommonUtilsForLua:setButtonTitle(self.m_btnrank2,"")
    self.m_lblleft5:setString(LuaController:getLang("145019"))
    self.m_lblright5:setString(LuaController:getLang("145020"))
    self.m_lblleft5:setVisible(false)
    self.m_lblright5:setVisible(false)
    print("ReviveView:initView____20")
    self:addChild(nodeccb)
    self.ccbNode = nodeccb
    
    self:addUIParticles()
    
    local function onNodeEvent(event)
        if event == "enter" then
            self:setTitleName(LuaController:getLang("145001"))
            self.officer = ""
            self.playerInfo = GlobalData:shared():getPlayerInfo()
            if self.playerInfo ~= nil then
                self.officer =  self.playerInfo:getProperty("officer")
            end
            self.isClose = true
            --test code
            -- self.officer = "216000"
            --end
            self:refreshView()
            -- print "ReviveView__enter"
        elseif event == "exit" then
            -- print "ReviveView__exit"
            self:onExit()
        end
    end
    self.ccbNode:registerScriptHandler(onNodeEvent)
    print("ReviveView:initView____21")
    schedule(self.m_bottomNode,scheduleDealWithFunc({target = self}),1) 

    if CEMETERY_INFO["ACT"] ~= nil then
        local endT = tonumber(CEMETERY_INFO["ACT"]["et"])
        local nT = GlobalData:shared():getTimeStamp()
        if endT > nT then
            local leftT = endT-nT
            performWithDelay(self.m_topNode,delayDealWithFunc({target = self}),leftT)
        end
    end
    return true
end
function ReviveView:callbackFunc()
    self.isClose = true
    self:closeSelf()
end

function ReviveView:addUIParticles()
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
function ReviveView:scheduleBack()
    if self.tickCnt >= 60 then
        self.tickCnt = 0
        LuaController:getInstance():sendCMD("cemetery.get.info")
    else
        self.tickCnt = self.tickCnt + 1
    end
    
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
function ReviveView:setLeftTime()
    self:scheduleBack()
    if nil ~=self.m_lblleft1 then
        self.m_lblleft1:setAnchorPoint(cc.p(0.5,0.5))
        self.m_lblleft1:setPositionY(225)
    end
end
function ReviveView:refreshLeftNode()
    print("ReviveView:refreshLeftNode____1")
    local total1 = LuaController:getCMDLang(tostring(CEMETERY_INFO["KINGDOM"]["totaldead"]))
    print("ReviveView:refreshLeftNode____2   [ " .. total1 .. " ]" )
    self.m_lblleft4:setString(total1)
    local contribution1 = LuaController:getCMDLang(tostring(CEMETERY_INFO["KINGDOM"]["totalcontribution"]))
    local contribution2 = LuaController:getCMDLang(tostring(CEMETERY_INFO["KINGDOM"]["needcontributionsum"]))
    contribution1 = contribution1 .. "/" .. contribution2
    print("ReviveView:refreshLeftNode____3     [ " .. contribution1 .. " ]")
    self.m_lblleft2:setString(contribution1)
    local revivestat = tonumber(CEMETERY_INFO["KINGDOM"]["revivestat"]) 
    print("ReviveView:refreshLeftNode____4     [ " .. tostring(revivestat) .. " ]")
    if revivestat == 1 then -- 接受捐赠状态
        self.reviveTime = 0
        print("ReviveView:refreshLeftNode____4.1")
        self.m_lblleft5:setVisible(false)
        self.m_leftNode1:setVisible(true)
        self.m_lblleft1:setVisible(true)
        self.m_lblleft2:setVisible(true)
        self.m_bgLeft:setVisible(true)
    elseif revivestat == 2 then  -- 可以复活状态，当捐赠满后，后端会修改状态值为2，当倒计时结束或者国王手动点击复活之后，状态改为关闭
        print("ReviveView:refreshLeftNode____4.2")
        self.m_leftNode1:setVisible(true)
        self.m_lblleft2:setVisible(false)
        if CEMETERY_INFO["CD"] ~= nil then
            self.reviveTime = tonumber(CEMETERY_INFO["CD"]["deadReviveCD"])
            print("ReviveView:refreshLeftNode____4.2.3    [ " .. tostring(self.reviveTime) .. " ]")
        end
        self:setLeftTime()
        self.m_lblleft1:setVisible(true)
        self.m_lblleft5:setVisible(false)
        self.m_bgLeft:setVisible(true)
    elseif revivestat == 3 then -- 关闭
        self.reviveTime = 0
        print("ReviveView:refreshLeftNode____4.3")  
        self.m_leftNode1:setVisible(false)
        self.m_lblleft1:setVisible(false)
        self.m_lblleft2:setVisible(false)
        self.m_bgLeft:setVisible(false)
        self.m_lblleft5:setVisible(true)
    end
    print("ReviveView:refreshLeftNode____5")
    self.m_parnode8:removeAllChildren(true)
    print("ReviveView:refreshLeftNode____5.1") 
    self.m_clipNodeLeft = nil
    self.m_clipNodeLeft1 = nil
    print("ReviveView:refreshLeftNode____5.2") 
    local sprLeft = CCLoadSprite:createSprite("fuhuo_leftbar.png")
    print("ReviveView:refreshLeftNode____5.3") 
    local barMax = tonumber(CEMETERY_INFO["KINGDOM"]["needcontributionsum"])
    local barCur = tonumber(CEMETERY_INFO["KINGDOM"]["totalcontribution"])
    print("ReviveView:refreshLeftNode____5.4  [ " .. tostring(barMax) .. ", " .. tostring(barCur) .. " ]") 
    local barH = 1
    if barMax > 0 then
        local percent = math.min(barCur/barMax,1)
        percent = math.max(percent,0)
        barH = sprLeft:getContentSize().height * percent
    end
    print("ReviveView:refreshLeftNode____6") 
    local nodew = sprLeft:getContentSize().width
    self.m_clipNodeLeft = CCClipNode:create(cc.size(nodew,barH))
    if self.m_clipNodeLeft ~= nil then
        local cpNode = tolua.cast(self.m_clipNodeLeft,"cc.Node")
        if cpNode ~= nil then
            print("ReviveView:refreshLeftNode____6.1") 
            cpNode:setAnchorPoint(cc.p(0, 0))
            self.m_parnode8:addChild(cpNode)
            
            local par1 = LuaController:createParticleForLua(self.rootPath .. "/particles/CrossServerRelive_bubbledown")
            if nil ~= par1 then
                par1:setAnchorPoint(cc.p(0, 1))
                cpNode:addChild(par1)
                par1:setPosition(123,barH)
            end

            print("ReviveView:refreshLeftNode____6.2") 
            sprLeft:setAnchorPoint(cc.p(0, 0))
            cpNode:addChild(sprLeft)

            local particles4 = LuaController:createParticleForLua(self.rootPath .. "/particles/CrossServerRelive_bubble")
            if nil ~= particles4 then
                cpNode:addChild(particles4)
                particles4:setPosition(123,100)
            end
        end
    end
    
    self.m_clipNodeLeft1 = CCClipNode:create(cc.size(nodew,324))
    if self.m_clipNodeLeft1 ~= nil then
        local cpNode1 = tolua.cast(self.m_clipNodeLeft1,"cc.Node")
        if cpNode1 ~= nil then
            cpNode1:setAnchorPoint(cc.p(0, 0))
            self.m_parnode8:addChild(cpNode1)
            local sprLeft1 = CCLoadSprite:createSprite("fuhuo_leftbar1.png")
            if sprLeft1 ~= nil then
                sprLeft1:setAnchorPoint(cc.p(0, 1))
                cpNode1:addChild(sprLeft1)
                sprLeft1:setPosition(0,barH)
            end

            local par = LuaController:createParticleForLua(self.rootPath .. "/particles/CrossServerRelive_bubbleup")
            if nil ~= par then
                cpNode1:addChild(par)
                par:setPosition(123,barH)
            end
        end
    end
    print("ReviveView:refreshLeftNode____7")
end
function ReviveView:refreshRightNode()
    print("ReviveView:refreshRightNode____1")
    if CEMETERY_INFO["PLAYER"] == nil then
        self.m_rightNode1:setVisible(false)
        self.m_lblright1:setVisible(false)
        self.m_lblright2:setVisible(false)
        self.m_bgRight:setVisible(false)
        -- self.m_btnright:setVisible(false)
        self.m_lblright5:setVisible(true)
        return
    end
    
    local total2 = LuaController:getCMDLang(tostring(CEMETERY_INFO["PLAYER"]["revivesumthistime"]))
    self.m_lblright4:setString(total2)
    local cv = tonumber(CEMETERY_INFO["PLAYER"]["contributionvalue"])
    local nv = tonumber(CEMETERY_INFO["PLAYER"]["needcontributionvalue"])
    local contribution21 = LuaController:getCMDLang(cv)
    local contribution22 = LuaController:getCMDLang(nv)
    contribution21 = contribution21 .. "/" .. contribution22
    self.m_lblright2:setString(contribution21)
    print("ReviveView:refreshRightNode____2  [ " .. tostring(cv) .. "," .. tostring(nv) .. " ]")
    if nv > 0 then
        print("ReviveView:refreshRightNode____2.1")
        self.m_lblright1:setVisible(true)
        self.m_lblright2:setVisible(true)
        self.m_bgRight:setVisible(true)
        self.m_lblright5:setVisible(false)
        self.m_rightNode1:setVisible(true)
    else
        print("ReviveView:refreshRightNode____2.2")
        self.m_rightNode1:setVisible(false)
        self.m_lblright1:setVisible(false)
        self.m_lblright2:setVisible(false)
        self.m_bgRight:setVisible(false)
        -- self.m_btnright:setVisible(false)
        self.m_lblright5:setVisible(true)
    end
    print("ReviveView:refreshRightNode____3")
    self.m_parnode9:removeAllChildren(true)
    self.m_clipNodeRight = nil
    self.m_clipNodeRight1 = nil
    local sprRight = CCLoadSprite:createSprite("fuhuo_rightbar.png")
    local barH = 1
    if nv > 0 then
        local percent = math.min(cv / nv, 1)
        percent = math.max(percent,0)
        barH = sprRight:getContentSize().height * percent
    end
    print("ReviveView:refreshRightNode____4")
    local nodew = sprRight:getContentSize().width
    self.m_clipNodeRight = CCClipNode:create(cc.size(nodew,barH))
    if self.m_clipNodeRight ~= nil then
        local cpNode = tolua.cast(self.m_clipNodeRight,"cc.Node")
        if cpNode ~= nil then
            cpNode:setAnchorPoint(cc.p(0, 0))
            self.m_parnode9:addChild(cpNode)
            
            local par1 = LuaController:createParticleForLua(self.rootPath .. "/particles/CrossServerRelive_bubbledown")
            if nil ~= par1 then
                par1:setAnchorPoint(cc.p(0, 1))
                cpNode:addChild(par1)
                par1:setPosition(120,barH)
            end
            
            sprRight:setAnchorPoint(cc.p(0, 0))
            cpNode:addChild(sprRight)

            local particles4 = LuaController:createParticleForLua(self.rootPath .. "/particles/CrossServerRelive_bubble")
            if nil ~= particles4 then
                cpNode:addChild(particles4)
                particles4:setPosition(120,100)
            end
        end
    end
    print("ReviveView:refreshRightNode____5")
    self.m_clipNodeRight1 = CCClipNode:create(cc.size(nodew,324))
    if self.m_clipNodeRight1 ~= nil then
        local cpNode1 = tolua.cast(self.m_clipNodeRight1,"cc.Node")
        if cpNode1 ~= nil then
            cpNode1:setAnchorPoint(cc.p(0, 0))
            self.m_parnode9:addChild(cpNode1)
            local sprRight1 = CCLoadSprite:createSprite("fuhuo_rightbar1.png")
            if sprRight1 ~= nil then
                sprRight1:setAnchorPoint(cc.p(0, 1))
                cpNode1:addChild(sprRight1)
                sprRight1:setPosition(0,barH)
            end

            local par = LuaController:createParticleForLua(self.rootPath .. "/particles/CrossServerRelive_bubbleup")
            if nil ~= par then
                cpNode1:addChild(par)
                par:setPosition(120,barH)
            end
        end
    end
    print("ReviveView:refreshRightNode____6")
end
function ReviveView:refreshBtn1()
    print("ReviveView:refreshBtn1____1")
    if self.officer == "" then  -- 无官职
        self.blessTime = 0
        print("ReviveView:refreshBtn1____1.1")
        self.m_btn1:setVisible(false)
        self.m_btn1:setEnabled(false)
    else   -- 有官职
        if self.officer == "216000" then
            local revivestat = tonumber(CEMETERY_INFO["KINGDOM"]["revivestat"]) 
            if revivestat == 2 then
                self.blessTime = 0
                self.m_btn1:setVisible(true)
                self.m_btn1:setEnabled(true)
                CCCommonUtilsForLua:setButtonTitle(self.m_btn1,LuaController:getLang("145024"))
                CCCommonUtilsForLua:setButtonTitleColor(self.m_btn1,cc.c3b(255, 255, 255))
                return
            end
        end

        print("ReviveView:refreshBtn1____1.2")
        local lastTime = 0
        local waitTime = 0
        if self.officer == "216000" then  -- king
            print("ReviveView:refreshBtn1____1.2.1")
            lastTime = tonumber(CEMETERY_INFO["KINGDOM"]["kingblessingtime"])  
            waitTime = tonumber(CEMETERY_INFO["KINGDOM"]["kingblessinterval"])
        else
            print("ReviveView:refreshBtn1____1.2.2")
            lastTime = tonumber(CEMETERY_INFO["KINGDOM"]["ministerblessingtime"])
            waitTime = tonumber(CEMETERY_INFO["KINGDOM"]["ministerblessinterval"])
        end
        print("ReviveView:refreshBtn1____1.3   [ " .. tostring(lastTime) .. "," ..  tostring(waitTime) .. " ]")
        
        local nowTime = GlobalData:shared():getTimeStamp()
        local passTime = nowTime -( lastTime + waitTime )
        if passTime > 0 then
            self.blessTime = 0
            CCCommonUtilsForLua:setButtonTitleColor(self.m_btn1,cc.c3b(255, 255, 255))
            print("ReviveView:refreshBtn1____1.3.1  [ " .. tostring(passTime) .. " ]") 
            CCCommonUtilsForLua:setButtonTitle(self.m_btn1,LuaController:getLang("145021"))
            self.m_btn1:setVisible(true)
            self.m_btn1:setEnabled(true)
        else
            self.blessTime = lastTime + waitTime
            CCCommonUtilsForLua:setButtonTitleColor(self.m_btn1,cc.c3b(255, 0, 0))
            print("ReviveView:refreshBtn1____1.3.2")
            self:scheduleBack()
            self.m_btn1:setVisible(true)
            self.m_btn1:setEnabled(false)
        end
    end
    print("ReviveView:refreshBtn1____2")  
end
function ReviveView:refreshBtn2()
    print("ReviveView:refreshBtn2____1") 
    local nv = 0
    if CEMETERY_INFO["PLAYER"] ~= nil then
        local cv = tonumber(CEMETERY_INFO["PLAYER"]["contributionvalue"])
        nv = tonumber(CEMETERY_INFO["PLAYER"]["needcontributionvalue"])
        if nv > 0 then
            if cv >= nv then
                CCCommonUtilsForLua:setButtonTitleColor(self.m_btn2,cc.c3b(223, 170, 112))
                CCCommonUtilsForLua:setButtonTitle(self.m_btn2,LuaController:getLang("145024")) 
                self.btn2State = 1
                self.m_btn2:setVisible(true)
                self.m_nodeProp:setVisible(false)
                return
            end
        end
    end
    CCCommonUtilsForLua:setButtonTitle(self.m_btn2,LuaController:getLang("145013")) 
    local propId = 200066
    local tInfo = ToolController:getInstance():getToolInfoForLua(propId)
    if tInfo == nil then
        self.m_btn2:setVisible(false)
        self.m_nodeProp:setVisible(false)
        return
    end

    print("ReviveView:refreshBtn2____2")
    local cnt = tInfo:getCNT()
    if cnt > 0 then
        CCCommonUtilsForLua:setButtonTitleColor(self.m_btn2,cc.c3b(223, 170, 112))
        -- self.m_lblProp:setColor(cc.c3b(223, 170, 112))
        self.m_lblProp:setString(tostring(cnt))
    else
        CCCommonUtilsForLua:setButtonTitleColor(self.m_btn2,cc.c3b(255, 0, 0))
        -- self.m_lblProp:setColor(cc.c3b(255, 0, 0))
        self.m_lblProp:setString("0")
    end
    
    print("ReviveView:refreshBtn2____3")
    local cemeterystat = tonumber(CEMETERY_INFO["KINGDOM"]["cemeterystat"]) 
    local btnVis = true
    if cemeterystat == 1 then
        local revivestat = tonumber(CEMETERY_INFO["KINGDOM"]["revivestat"]) 
        if revivestat ~= 1 then
            if nv <= 0 then
                btnVis =false
            end
        end
    else
        btnVis = false
    end
    self.m_btn2:setVisible(btnVis)
    self.m_nodeProp:setVisible(btnVis)
end
function ReviveView:refreshView()
    print("ReviveView:refreshView_____1")
    if CEMETERY_INFO == nil then
        return
    end
    print("ReviveView:refreshView_____2")
    print("ReviveView:refreshView_____3")
    if CEMETERY_INFO["KINGDOM"] == nil then
        return
    end 
    print("ReviveView:refreshView_____4")

    self:refreshLeftNode()
    self:refreshRightNode()
    self:refreshBtn1()
    self:refreshBtn2()
end
function ReviveView:onExit()
    if self.isClose == false then
        return
    end

    ReviveLuaView.onClickBtn1 = nil
    ReviveLuaView.onClickBtn2 = nil
    ReviveLuaView.onClickBtnRank1 = nil
    ReviveLuaView.onClickBtnRank2 = nil
    ReviveLuaView.onClickBtnLeft = nil
    ReviveLuaView.onClickBtnRight = nil
    if nil ~=self.m_bottomNode then
        self.m_bottomNode:stopAllActions()
    end
    if nil ~= self.m_topNode then
        self.m_topNode:stopAllActions()
    end
    if nil ~= self.ccbNode then
        self.ccbNode:unregisterScriptHandler()
    end
    local targetPlatform = cc.Application:getInstance():getTargetPlatform()
    if (3 == targetPlatform) then
        releaseLuaResourceForAndroid(self.rootPath .. "/resources/" , "revive")
    else
        releaseLuaResource(self.rootPath .. "/resources/revive")
    end
end
function ReviveView:onClickBtn1()
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
function ReviveView:onClickBtn2()
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
            LuaController:flyHint("","",LuaController:getLang1("145029",tInfo:getName()))
        end
    end
end
function ReviveView:onClickBtnRank1()
    print("ReviveView:onClickBtnRank1")
    self.isClose=false
    require "revive.ReviveRankView"
    local view1 = ReviveRankView:create(self.path,2)
    if view1 ~= nil then
        PopupViewController:getInstance():addPopupInView(view1)
    end
end
function ReviveView:onClickBtnRank2()
    print("ReviveView:onClickBtnRank2")
    -- self.isClose=false
    -- require "revive.ReviveRankView"
    -- local view1 = ReviveRankView:create(self.path,1)
    -- if view1 ~= nil then
    --     PopupViewController:getInstance():addPopupInView(view1)
    -- end
    
    local view1 = KingBattleRankView:create()
    if nil ~= view1 then
        PopupViewController:getInstance():addPopupView(view1)
    end
end
function ReviveView:onClickBtnRight()
    print("ReviveView:onClickBtnRight")
    if self.isPlayerRevive == false then
        self.isPlayerRevive = true
        LuaController:getInstance():sendCMD("cemetery.player.click.revive")
    end
    self.btn2State = 0
end
function ReviveView:onClickBtnLeft()
    print("ReviveView:onClickBtnLeft")
    LuaController:getInstance():sendCMD("cemetery.king.revive")
    self.m_leftNode1:setVisible(false)
    self.m_lblleft1:setVisible(false)
    self.m_lblleft2:setVisible(false)
    self.m_bgLeft:setVisible(false)
    self.m_lblleft5:setVisible(true)
end
function ReviveView:CMDCallbackForLua(cmdname,params)
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
function ReviveView:NotificationCallbackForLua(cmdname)
    -- body
    print("ReviveView:NotificationCallbackForLua")
end
function ReviveView:NotificationCallbackForLuaWithParams(cmdname,object)
    -- body
    print("ReviveView:NotificationCallbackForLuaWithParams")
end