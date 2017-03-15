require "externCOK2"
require "commonCOK2"
require "57034.ActivityRwdShowCell"

ActivityLuaView  = ActivityLuaView or {}
ccb["ActivityLuaView"] = ActivityLuaView

ActivityView = class("ActivityView",
    function()
        return PopupBaseView:create() 
    end
)
ActivityView.__index = ActivityView
function ActivityView:create(path,params)
    local view = ActivityView.new()
    if view:initView(path,params) == true then
        return view
    end
    return nil
end
function ActivityView:initView(path,params)
	self.path = path
    if self:init(true,0) == false then
        return false
    end
    print("ActivityView:initView")
    local actObj = ActivityController:getInstance():getActObj(params)
    if actObj == nil then
        return false
    end
    self.params = actObj
    self.actname = actObj:getProperty("id")
    self:setHDPanelFlag(true)
    local strPath = string.format(path)
    self.rootPath = strPath .. "/" .. self.actname
    loadLuaResource(self.rootPath .. "/resources/1.plist")
    
    local  proxy = cc.CCBProxy:create()
    ActivityLuaView.onClickRwdBtn = function()
        self:onClickRwdBtn()
    end
    local isPad = CCCommonUtilsForLua:isIosAndroidPad()
    local ccbiName = "ActivityLuaView.ccbi"
    if isPad == true then
        ccbiName = "ActivityLuaViewPad.ccbi"
    end
    local ccbiURL = self.rootPath .. "/ccbi/" .. ccbiName
    local nodeccb = CCBReaderLoad(ccbiURL,proxy,ActivityLuaView)
    if nodeccb == nil then
        return false
    end

    if nil ~= ActivityLuaView["m_listNode"] then
        self.m_listNode = tolua.cast(ActivityLuaView["m_listNode"],"cc.LayerColor")
    end
    if nil == self.m_listNode then
        return false
    end
    if nil ~= ActivityLuaView["m_picNode"] then
        self.m_picNode = tolua.cast(ActivityLuaView["m_picNode"],"cc.Node")
    end
    if nil == self.m_picNode then
        return false
    end
    if nil ~= ActivityLuaView["m_titleLabel"] then
        self.m_titleLabel = tolua.cast(ActivityLuaView["m_titleLabel"],"cc.Label")
    end
    if nil == self.m_titleLabel then
        return false
    end
    if nil ~= ActivityLuaView["m_bottomNode"] then
        self.m_bottomNode = tolua.cast(ActivityLuaView["m_bottomNode"],"cc.LayerColor")
    end
    if nil == self.m_bottomNode then
        return false
    end
    if nil ~= ActivityLuaView["m_rwdBtn"] then
        self.m_rwdBtn = tolua.cast(ActivityLuaView["m_rwdBtn"],"cc.ControlButton")
    end
    if nil == self.m_rwdBtn then
        return false
    end

    
    local screenSize = cc.Director:getInstance():getWinSize()
    if isPad == true then
        self:setContentSize(cc.size(1536, 2048))
        self.m_bottomNode:setPositionY(self.m_bottomNode:getPositionY()-(screenSize.height - 2048))
        self.m_listNode:setPositionY(self.m_listNode:getPositionY() - (screenSize.height - 2048))
        local listSize = self.m_listNode:getContentSize()
        self.m_listNode:setContentSize(cc.size(listSize.width,listSize.height+screenSize.height - 2048))
    else
        self:setContentSize(cc.size(640, 852))
        self.m_bottomNode:setPositionY(self.m_bottomNode:getPositionY() - (screenSize.height - 852))
        self.m_listNode:setPositionY(self.m_listNode:getPositionY() - (screenSize.height - 852))
        local listSize = self.m_listNode:getContentSize()
        self.m_listNode:setContentSize(cc.size(listSize.width,listSize.height+screenSize.height-852))
    end
    local pExchange = self.params:getProperty("exchange")
    if (pExchange == "1") then
        self.m_bottomNode:setVisible(true)
    elseif (pExchange == "2") then
        self.m_bottomNode:setVisible(true)
    else
        self.m_bottomNode:setVisible(false)
        local listSize = self.m_listNode:getContentSize()
        if isPad == true then
            self.m_listNode:setPositionY(self.m_listNode:getPositionY() - 178)
            self.m_listNode:setContentSize(cc.size(listSize.width, listSize.height + 178))
        else
            self.m_listNode:setPositionY(self.m_listNode:getPositionY() - 92)
            self.m_listNode:setContentSize(cc.size(listSize.width, listSize.height + 92))
        end
    end
    
    self.m_titleLabel:setString(LuaController:getLang("150215"))
    CCCommonUtilsForLua:setButtonTitle(self.m_rwdBtn,LuaController:getLang("133076"))
        
    local node = cc.Node:create()
    local height = 0
    local rewardCnt = 0
    local rewardTab = self.params:getProperty("rewardIds")
    if rewardTab ~= nil then
        rewardCnt = table.getn(rewardTab)
    end
    local dialogCnt = 0
    local dialogTab = self.params:getProperty("showDialogs")
    if dialogTab ~= nil then
        dialogCnt = table.getn(dialogTab)
    end
    
    if rewardCnt > 0 then
        if dialogCnt > 0 then
            if isPad == true then
                height = height - 20
            else
                height = height - 20
            end

            for i,v in ipairs(dialogTab) do
                local vec = string.split(v,";")
                local vecCnt = table.getn(vec)
                local showStr = ""
                if (vecCnt == 1) then
                    showStr = LuaController:getLang(vec[1])
                elseif (vecCnt == 2) then
                    showStr = LuaController:getLang1(vec[1], CCCommonUtilsForLua:getNameById(vec[2]))
                elseif (vecCnt == 3) then
                    showStr = LuaController:getLang2(vec[1], CCCommonUtilsForLua:getNameById(vec[2]), CCCommonUtilsForLua:getNameById(vec[3]))
                elseif (vecCnt == 4) then
                    showStr = LuaController:getLang3(vec[1], CCCommonUtilsForLua:getNameById(vec[2]), CCCommonUtilsForLua:getNameById(vec[3]), CCCommonUtilsForLua:getNameById(vec[4]));
                end
                if string.len(showStr)>0 then
                    local label = CCLabelIF:create(showStr)
                    if (isPad == true) then
                        label:setFontSize(40)
                        label:setDimensions(cc.size(1300, 0))
                    else
                        label:setFontSize(20)
                        label:setDimensions(cc.size(580, 0))
                    end
                    label:setColor(cc.BLACK)
                    label:setAnchorPoint(cc.p(0.5, 1))
                    label:setHorizontalAlignment(cc.TEXT_ALIGNMENT_CENTER)
                    label:setVerticalAlignment(cc.VERTICAL_TEXT_ALIGNMENT_TOP)
                    local lblNode = tolua.cast(label,"cc.Node")
                    if lblNode ~= nil then
                        node:addChild(lblNode)
                        lblNode:setPosition(0,height)
                    end
                    height = height - (label:getContentSize().height * label:getOriginScaleY())
                end
            end
        end

        if isPad == true then
            height = height - 5
        else
            height = height - 5
        end
        local ifStr = LuaController:getLang("150217")
        local label1 = CCLabelIF:create(ifStr)
        if isPad == true then
            label1:setFontSize(36)
        else
            label1:setFontSize(18)
        end
        label1:setAnchorPoint(cc.p(0.5, 1))
        label1:setColor(cc.c3b(139,29,20))
        label1:setHorizontalAlignment(cc.TEXT_ALIGNMENT_CENTER)
        label1:setVerticalAlignment(cc.VERTICAL_TEXT_ALIGNMENT_TOP)
        local lblNode = tolua.cast(label1,"cc.Node")
        if lblNode ~= nil then
            node:addChild(lblNode)
            lblNode:setPosition(0, height)
        end
        height = height - (label1:getContentSize().height * label1:getOriginScaleY())


        local hang = math.ceil(rewardCnt / 6.0)
        for i=1,hang do
            if (i < hang) then
                for j=1,6 do
                    local idx = j + (i - 1) * 6
                    local cell = ActivityRwdShowCell:create(rewardTab[idx])
                    if isPad == true then
                        cell:setPositionX(- 160 * 6 / 2 + 160 / 2 + (j - 1) * 160)
                        cell:setPositionY(height - 80)
                    else
                        cell:setPositionX(- 80 * 6 / 2 + 80 / 2 + (j - 1) * 80)
                        cell:setPositionY(height - 40)
                    end
                    node:addChild(cell)
                end
                if isPad == true then
                    height = height - 80
                else
                    height = height - 80
                end
            else
                local leftCnt = rewardCnt - (i - 1) * 6
                for j=1,leftCnt do
                    local idx = j + (i - 1) * 6;
                    local cell = ActivityRwdShowCell:create(rewardTab[idx])
                    if isPad == true then
                        cell:setPositionX(- 160 * leftCnt / 2 + 160 / 2 + (j - 1) * 160)
                        cell:setPositionY(height - 80)
                    else
                        cell:setPositionX(- (80 * leftCnt) / 2 + 80 / 2 + (j - 1) * 80)
                        cell:setPositionY(height - 40)
                    end
                    node:addChild(cell)
                end
                if isPad == true then
                    height = height - 80
                else
                    height = height - 80
                end
            end
        end
        
        if isPad == true then
            height = height - 90
        else
            height = height - 10
        end

        local spr = CCLoadSprite:createSprite("t_Items_tips3_lottery.png")
        spr:setAnchorPoint(cc.p(0.5, 0.5))
        spr:setPosition(0, height)
        node:addChild(spr)
        if isPad == true then
            spr:setScale(2)
        end
        if isPad == true then
            height = height - 10
        else
            height = height - 10
        end

        local lable2 = CCLabelIF:create(LuaController:getLang("150216"))
        lable2:setAnchorPoint(cc.p(0.5, 1))
        lable2:setColor(cc.BLACK)
        if isPad == true then
            lable2:setFontSize(40)
        else
            lable2:setFontSize(20)
        end
        lable2:setHorizontalAlignment(cc.TEXT_ALIGNMENT_CENTER)
        lable2:setVerticalAlignment(cc.VERTICAL_TEXT_ALIGNMENT_TOP)
        local lblNode2 = tolua.cast(lable2,"cc.Node")
        if lblNode2 ~= nil then
            node:addChild(lblNode2)
            lblNode2:setPosition(0, height)
        end
        height = height - (lable2:getContentSize().height * lable2:getOriginScaleY())
        if isPad == true then
            height = height - 10
        else
            height = height - 10
        end
    end

    local mStory = LuaController:getLang(self.params:getProperty("story"))
    if mStory ~= nil then
        mStory = mStory .. "\n\n\n"
    end
    if string.len(mStory) > 0 then
        local label3 = CCLabelIF:create(mStory)
        if isPad == true then
            label3:setFontSize(40)
            label3:setDimensions(cc.size(1300, 0))
        else
            label3:setFontSize(20)
            label3:setDimensions(cc.size(580, 0))
        end
        label3:setAnchorPoint(cc.p(0.5, 1))
        label3:setColor(cc.c3b(85,62,31))
        label3:setHorizontalAlignment(cc.TEXT_ALIGNMENT_CENTER)
        label3:setVerticalAlignment(cc.VERTICAL_TEXT_ALIGNMENT_TOP)
        local lblNode3 = tolua.cast(label3,"cc.Node")
        if lblNode3 ~= nil then
            node:addChild(lblNode3)
            lblNode3:setPosition(0, height)
        end
        height = height - (label3:getContentSize().height * label3:getOriginScaleY())
    end

    local listSize = self.m_listNode:getContentSize()
    self.sView = cc.ScrollView:create()
    self.sView:setViewSize(listSize)
    local absH = math.abs(height)
    local slayer = cc.LayerColor:create(cc.c4b(255,219,117,0),listSize.width,absH)
    -- slayer:setContentSize(cc.size(listSize.width,absH))
    slayer:addChild(node)
    node:setPositionY(absH)
    node:setPositionX(listSize.width * 0.5)
    self.sView:setContainer(slayer)
    self.sView:updateInset()
    
    self.sView:setDirection(cc.SCROLLVIEW_DIRECTION_VERTICAL)
    if absH < listSize.height then
        self.sView:setTouchEnabled(false)
        slayer:setPositionY((listSize.height - absH ) * 0.5)
    else
        self.sView:setTouchEnabled(true)
        node:setPositionY(absH)
        self.sView:setClippingToBounds(true)
        self.sView:setBounceable(true)
        -- self.sView:registerScriptHandler(ActivityView.scrollViewDidScroll,cc.SCROLLVIEW_SCRIPT_SCROLL)
        self.sView:setContentOffset(cc.p(0,listSize.height-absH))
    end
    self.m_listNode:addChild(self.sView)
    self.sView:setDelegate()

    local node1 = cc.Node:create()
    local bgImg = CCLoadSprite:createSprite("t_57034_ad1.png")
    bgImg:setAnchorPoint(cc.p(0, 0))
    bgImg:setPosition(0, 0)
    node1:addChild(bgImg)  

    local nameTTF = CCLabelIF:create(self.params:getProperty("name"))
    nameTTF:setAnchorPoint(cc.p(0, 1))
    nameTTF:setFontSize(36)
    nameTTF:setColor(cc.c3b(255,219,117))
    nameTTF:setHorizontalAlignment(cc.TEXT_ALIGNMENT_LEFT)
    nameTTF:setVerticalAlignment(cc.VERTICAL_TEXT_ALIGNMENT_TOP)
    nameTTF:setDimensions(cc.size(0, 0))
    local nameNode = tolua.cast(nameTTF,"cc.Node")
    if nameNode ~= nil then
        nameNode:setPosition(16,245)
        node1:addChild(nameNode)
    end
    local infoTTF = CCLabelIF:create(self.params:getProperty("desc"))
    infoTTF:setAnchorPoint(cc.p(0, 1))
    infoTTF:setColor(cc.c3b(0,249,0))
    infoTTF:setHorizontalAlignment(cc.TEXT_ALIGNMENT_LEFT)
    infoTTF:setVerticalAlignment(cc.VERTICAL_TEXT_ALIGNMENT_TOP)
    infoTTF:setDimensions(cc.size(300, 0))
    local infoNode = tolua.cast(infoTTF,"cc.Node")
    if infoNode ~= nil then
        infoNode:setPosition(16,201)
        node1:addChild(infoNode)
    end
    
    self.m_picNode:addChild(node1)

    if isPad == true then
        local w = (1536.0 - 1156.0) / 2.0
        local h = 645
        node1:setContentSize(cc.size(0,0))
        node1:setScaleX(1156.0 / 640.0)
        node1:setScaleY(645.0 / 357.0)
        local layer1 = CCModelLayerColor:create()
        local cLayer = tolua.cast(layer1,"cc.LayerColor")
        if cLayer ~= nil then
            cLayer:setAnchorPoint(cc.p(0, 0))
            cLayer:setContentSize(cc.size(w, h))
            cLayer:setPosition(0-w, 0)
            cLayer:setOpacity(255)
            cLayer:setColor(cc.BLACK)
            self.m_picNode:addChild(cLayer)
        end

        local layer2 = CCModelLayerColor:create()
        local cLayer2 = tolua.cast(layer2,"cc.LayerColor")
        if cLayer2 ~= nil then
            cLayer2:setAnchorPoint(cc.p(0, 0))
            cLayer2:setContentSize(cc.size(w, h))
            cLayer2:setPosition(1156, 0)
            cLayer2:setOpacity(255)
            cLayer2:setColor(cc.BLACK)
            self.m_picNode:addChild(cLayer2)
        end
    end

    self:addChild(nodeccb)
    self.ccbNode = nodeccb
        
    local function onNodeEvent(event)
        if event == "enter" then
            self:setTitleName("")
            -- print "ActivityView__enter"
        elseif event == "exit" then
            -- print "ActivityView__exit"
            self:onExit()
        end
    end
    self.ccbNode:registerScriptHandler(onNodeEvent)


    if pExchange == "2" then
        if ActivityController:getInstance():isInExc2Data(self.params:getProperty("id")) == true then
            ActivityController:getInstance():startGetAct6Data()
        else
            local refinfo = ActivityController:getInstance():getAct2RefrshInfoByKey(self.params:getProperty("id"))
            if nil ~= refinfo then
                if ActivityController:getInstance():checkIsToday(refinfo.m_refreshTime) == false then
                    refinfo.m_refreshTime = GlobalData:shared():getTimeStamp()
                    refinfo.m_refresh = 0
                end
            end
        end
    elseif pExchange == "1" then
        if ActivityController:getInstance():isAct1DataInit(self.params:getProperty("id")) == false then
            ActivityController:getInstance():startGetAct6Data()
        else
            ActivityController:getInstance():checkAct1ReFreshTimes(self.params:getProperty("id"))
            if ActivityController:getInstance():isAct1DataShouldRefresh(self.params:getProperty("id")) == true then
                ActivityController:getInstance():startAct6Refresh(self.params:getProperty("id"))
            end
        end
    end
    
    return true
end

function ActivityView:onExit()
    ActivityLuaView.onClickRwdBtn=nil
    if nil ~= self.ccbNode then
        self.ccbNode:unregisterScriptHandler()
    end
    local targetPlatform = cc.Application:getInstance():getTargetPlatform()
    if (3 == targetPlatform) then
        releaseLuaResourceForAndroid(self.rootPath .. "/resources/" , "1")
    else
        releaseLuaResource(self.rootPath .. "/resources/1")
    end
end
function ActivityView.scrollViewDidScroll(view)
    -- print("ActivityView__scrollViewDidScroll")
end

function ActivityView:onClickRwdBtn()
    local exchange = self.params:getProperty("exchange")
    if exchange == "2" then
    elseif exchange == "1" then
        --self:closeSelf()
        require ("57034.ActivityExcView")
        local newLayer = ActivityExcView:create(self.path,self.params)
        if nil ~= newLayer then
            PopupViewController:getInstance():addPopupInView(newLayer)
        end
    end
end
function ActivityView:CMDCallbackForLua(cmdname,data)
    -- body
    -- print("ActivityView:CMDCallbackForLua")
end
function ActivityView:NotificationCallbackForLua(cmdname)
    -- body
    -- print("ActivityView:NotificationCallbackForLua")
end
function ActivityView:NotificationCallbackForLuaWithParams(cmdname,object)
    -- body
    -- print("ActivityView:NotificationCallbackForLuaWithParams")
end