require "externCOK2"
require "commonCOK2"

local ACTDATA = nil
local LUAPATH = ""
-- 
-- class ActivityExcView
-- 
ActivityExcLuaCell  = ActivityExcLuaCell or {}
ccb["ActivityExcLuaCell"] = ActivityExcLuaCell
ActExcGlowLuaNode  = ActExcGlowLuaNode or {}
ccb["ActExcGlowLuaNode"] = ActExcGlowLuaNode

ActivityExcCell = class("ActivityExcCell",
    function()
        return cc.Layer:create() 
    end
)
ActivityExcCell.__index = ActivityExcCell
function ActivityExcCell:create(level)
    local node = ActivityExcCell.new()
    if node:initView(level) == true then
        return node
    end
    return nil
end
function ActivityExcCell:initView(level)
    print("ActivityExcCell:initView")
    self.level = level
    self.actId = ACTDATA:getProperty("actId")
    self.m_itemId = ACTDATA:getProperty("itemId")
    local tInfo = ToolController:getInstance():getToolInfoForLua(self.m_itemId)
    if tInfo == nil then
        return false
    end
    if self:init() == true then
        self.rootPath = LUAPATH .. "/" .. self.actId
        loadLuaResource(self.rootPath .. "/resources/1.plist")
        local  proxy = cc.CCBProxy:create()
        ActivityExcLuaCell.onClickBlue = function()
            print("ctivityExcLuaCell.onClickBlue")
            -- self:onClickBlue()
            if ActivityController:getInstance():startAct6Exchange(self.actId,self.level) == true then
                self.m_btnNode:setVisible(false)
                self.m_rwdedNode:setVisible(true)
            end
        end
        ActivityExcLuaCell.onClickGreen = function()
            print("ActivityExcLuaCell.onClickGreen")
            -- self:onClickGreen()
            if ActivityController:getInstance():startAct6Exchange(self.actId,self.level) == true then
                self.m_btnNode:setVisible(false)
                self.m_rwdedNode:setVisible(true)
            end
        end
        local ccbiURL = self.rootPath .. "/ccbi/ActivityExcLuaCell.ccbi"
        local node = CCBReaderLoad(ccbiURL,proxy,ActivityExcLuaCell)
        if node ~= nil then
            if nil ~= ActivityExcLuaCell["m_nameLabel"] then
                self.m_nameLabel = tolua.cast(ActivityExcLuaCell["m_nameLabel"],"cc.Label")
            end
            if nil == self.m_nameLabel then
                return false
            end
            if nil ~= ActivityExcLuaCell["m_rareLabel"] then
                self.m_rareLabel = tolua.cast(ActivityExcLuaCell["m_rareLabel"],"cc.Label")
            end
            if nil == self.m_rareLabel then
                return false
            end
            if nil ~= ActivityExcLuaCell["m_blueBtn"] then
                self.m_blueBtn = tolua.cast(ActivityExcLuaCell["m_blueBtn"],"cc.ControlButton")
            end
            if self.m_blueBtn == nil then
                return false
            end
            if nil ~= ActivityExcLuaCell["m_greenBtn"] then
                self.m_greenBtn = tolua.cast(ActivityExcLuaCell["m_greenBtn"],"cc.ControlButton")
            end
            if self.m_greenBtn == nil then
                return false
            end
            if nil ~= ActivityExcLuaCell["m_picNode"] then
                self.m_picNode = tolua.cast(ActivityExcLuaCell["m_picNode"],"cc.Node")
            end
            if self.m_picNode == nil then
                return false
            end
            if nil ~= ActivityExcLuaCell["m_needNum"] then
                self.m_needNum = tolua.cast(ActivityExcLuaCell["m_needNum"],"cc.Label")
            end
            if nil == self.m_needNum then
                return false
            end
            if nil ~= ActivityExcLuaCell["m_iconNode"] then
                self.m_iconNode = tolua.cast(ActivityExcLuaCell["m_iconNode"],"cc.Node")
            end
            if self.m_iconNode == nil then
                return false
            end
            if nil ~= ActivityExcLuaCell["m_numLabel"] then
                self.m_numLabel = tolua.cast(ActivityExcLuaCell["m_numLabel"],"cc.Label")
            end
            if nil == self.m_numLabel then
                return false
            end
            if nil ~= ActivityExcLuaCell["m_rwdedBg"] then
                self.m_rwdedBg = tolua.cast(ActivityExcLuaCell["m_rwdedBg"],"cc.Sprite")
            end
            if self.m_rwdedBg == nil then
                return false
            end
            if nil ~= ActivityExcLuaCell["m_rwdedLabel"] then
                self.m_rwdedLabel = tolua.cast(ActivityExcLuaCell["m_rwdedLabel"],"cc.Label")
            end
            if nil == self.m_rwdedLabel then
                return false
            end
            if nil ~= ActivityExcLuaCell["m_rwdedNode"] then
                self.m_rwdedNode = tolua.cast(ActivityExcLuaCell["m_rwdedNode"],"cc.Node")
            end
            if self.m_rwdedNode == nil then
                return false
            end
            if nil ~= ActivityExcLuaCell["m_btnNode"] then
                self.m_btnNode = tolua.cast(ActivityExcLuaCell["m_btnNode"],"cc.Node")
            end
            if self.m_btnNode == nil then
                return false
            end
            if nil ~= ActivityExcLuaCell["m_mainNode"] then
                self.m_mainNode = tolua.cast(ActivityExcLuaCell["m_mainNode"],"cc.Node")
            end
            if self.m_mainNode == nil then
                return false
            end
            if nil ~= ActivityExcLuaCell["m_glowNode"] then
                self.m_glowNode = tolua.cast(ActivityExcLuaCell["m_glowNode"],"cc.Node")
            end
            if self.m_glowNode == nil then
                return false
            end
            ActivityController:getInstance():createGoodsIcon(self.m_picNode, self.m_itemId, 40)
            self:addChild(node)
            self:setContentSize(node:getContentSize())
            self.ccbNode = node
            self.m_rareLabel:setColor(cc.c3b(124, 210, 25))
            self.m_numLabel:setColor(cc.c3b(255, 220, 166))
            self.m_nameLabel:setColor(cc.c3b(101, 116, 126))
            self.m_rwdedLabel:setString(LuaController:getLang("150333"))
            self.m_glowNode:setScaleX(0.85)
            self.m_glowNode:setScaleY(0.87)
            self.m_aniNode = CCAniNode:create()
            if self.m_aniNode ~= nil then
                local aNode = tolua.cast(self.m_aniNode,"cc.Node")
                if aNode ~= nil then
                    self.m_glowNode:addChild(aNode)
                    local ccbiURL1 = self.rootPath .. "/ccbi/ActExcGlowLuaNode.ccbi"
                    local  proxy1 = cc.CCBProxy:create()
                    local nodeglow = CCBReaderLoad(ccbiURL1,proxy1,ActExcGlowLuaNode)
                    if nodeglow ~= nil then
                        aNode:addChild(nodeglow)
                        aNode:setVisible(false)
                    end
                end
                self.m_aniNode:setAnimationManager(ActExcGlowLuaNode["mAnimationManager"])
            end

            self:setData(self.level,false)
            local function onNodeEvent(event)
                if event == "enter" then
                    -- print "ActivityExcCell__enter"
                elseif event == "exit" then
                    -- print "ActivityExcCell__exit"
                    self:onExit()
                end
            end
            self.ccbNode:registerScriptHandler(onNodeEvent)
        end
    end
    return true
end

function ActivityExcCell:setData(level,bAni)
    self.level = level

    if bAni == true then
        self.m_mainNode:setScale(1)
        self.m_mainNode:stopAllActions()
        local delay1 = cc.DelayTime:create(level * 0.033 * 3)
        local sc1 = cc.ScaleTo:create(0.033 * 3, 1, 1.2)
        local sc2 = cc.ScaleTo:create(0.033 * 3, 1, 0.8)
        local sc3 = cc.ScaleTo:create(0.033 * 3, 1, 0)
        local function refresh()
            -- body
            self:setData(self.level,false)
        end
        local fun = cc.CallFunc:create(refresh)
        local sc4 = cc.ScaleTo:create(0.033 * 4, 1, 1.1)
        local sc5 = cc.ScaleTo:create(0.033 * 4, 1, 1)
        local action = cc.Sequence:create(delay1, sc1, sc2, sc3, fun, sc4, sc5)
        self.m_mainNode:runAction(action)
        return
    end
    self.m_nameLabel:setString("")
    self.m_rareLabel:setString("")
    self.m_blueBtn:setVisible(false)
    self.m_greenBtn:setVisible(false)
    self.m_iconNode:removeAllChildren()
    self.m_numLabel:setString("")
    self.m_blueBtn:setEnabled(true)
    self.m_greenBtn:setEnabled(true)
    self.m_btnNode:setVisible(false)
    self.m_rwdedNode:setVisible(false)
    self.m_needNum:setString("")
    if self.m_aniNode ~= nil then
        local aNode = tolua.cast(self.m_aniNode,"cc.Node")
        if aNode ~= nil then
            aNode:setVisible(false)
        end
    end
    if (string.len(self.actId) == 0) then
        return
    end
    if ACTDATA ~= nil then
        local actItm = ACTDATA:getAct1ExcItemByKey(level)
        if actItm ~= nil then
            local rewardCnt = actItm:getRewardCnt()
            local itmcolor = actItm:getProperty("color") 
            if rewardCnt > 0 then
                local reItm = actItm:getRewardByIdx(0)
                if reItm ~= nil then
                    local reItemid = tonumber(reItm:getProperty("itemId"))
                    local tInfo = ToolController:getInstance():getToolInfoForLua(reItemid)
                    if tInfo ~= nil then
                        CCCommonUtilsForLua:createGoodsIcon(reItemid, self.m_iconNode, cc.size(77, 77))
                        local numStr = tostring(reItm:getProperty("num"))
                        self.m_numLabel:setString(LuaController:getCMDLang(numStr))
                        self.m_nameLabel:setString(tInfo:getName())
                        local w = self.m_nameLabel:getContentSize().width
                        local h = self.m_nameLabel:getContentSize().height
                        if (h > 30) then
                            if (itmcolor == 0) then
                                self.m_nameLabel:setPositionY(h / 2.0)
                            else
                                self.m_nameLabel:setPositionY(14)
                            end
                        end
                        local sss = "×" .. tostring(actItm:getProperty("needNum"))
                        self.m_needNum:setString(sss)
                    end
                end
            end
            if itmcolor == 0 then
                self.m_blueBtn:setVisible(true)
            else
                self.m_rareLabel:setString(LuaController:getLang("150360"))
                self.m_greenBtn:setVisible(true)
                if self.m_aniNode ~= nil then
                    local aNode = tolua.cast(self.m_aniNode,"cc.Node")
                    if aNode ~= nil then
                        aNode:setVisible(true)
                    end
                    self.m_aniNode:getAnimationManager():runAnimationsForSequenceNamed("Loop")
                end
            end
            if ActivityController:getInstance():isAct1ItemHasExchange(self.actId,self.level) == true then
                self.m_rwdedNode:setVisible(true)
                self.m_btnNode:setVisible(false)
                if self.m_aniNode ~= nil then
                    local aNode = tolua.cast(self.m_aniNode,"cc.Node")
                    if aNode ~= nil then
                        aNode:setVisible(false)
                    end
                end
            else
                self.m_rwdedNode:setVisible(false)
                self.m_btnNode:setVisible(true)
            end
        end
    end
end
function ActivityExcCell:onExit()
    ActivityExcCell.onClickGreen=nil
    ActivityExcCell.onClickBlue=nil
    if nil ~= self.ccbNode then
        self.ccbNode:unregisterScriptHandler()
    end
end
function ActivityExcCell:GetMsgRefreshEnd(bAni)
    -- body
    self:setData(self.level,bAni)
end
function ActivityExcCell:getLevel()
    return self.level
end


-- 
-- class ActivityExcView
-- 
ActivityExcLuaView  = ActivityExcLuaView or {}
ccb["ActivityExcLuaView"] = ActivityExcLuaView

ActivityExcView = class("ActivityExcView",
    function()
        return PopupBaseView:create() 
    end
)
ActivityExcView.__index = ActivityExcView
function ActivityExcView:create(path,params)
    local view = ActivityExcView.new()
    if view:initView(path,params) == true  then
        return view
    end
    return nil
end
function ActivityExcView:initView(path,params)
    LUAPATH = string.format(path)
    self.params = params
    self.actID = self.params:getProperty("id")
    if self:init(true,1) == false then
        return false
    end

    local excData = ActivityController:getInstance():getExc1DataByKey(self.actID) 
    if excData == nil then
        return false
    end
    ACTDATA = excData
    self.m_itemId = excData:getProperty("itemId")
    local tInfo = ToolController:getInstance():getToolInfoForLua(self.m_itemId)
    if tInfo == nil then
        return false
    end
    self:setHDPanelFlag(true)
    local isPad = CCCommonUtilsForLua:isIosAndroidPad()
    if (isPad == true) then
        self:setContentSize(cc.size(1536, 2048))
    else
        self:setContentSize(cc.size(640, 852))
    end
    self.rootPath = LUAPATH .. "/" .. self.actID
    loadLuaResource(self.rootPath .. "/resources/1.plist")
        
    local  proxy = cc.CCBProxy:create()
    ActivityExcLuaView.onClickRefresh = function()
        self:onClickRefresh()
    end
    ActivityExcLuaView.onHelpBtnClick = function()
        self:onHelpBtnClick()
    end
    local ccbiName = "ActivityExcLuaView.ccbi"
    if isPad == true then
        ccbiName = "ActivityExcLuaViewPad.ccbi"
    end 
    local ccbiURL = self.rootPath .. "/ccbi/" .. ccbiName
    local node = CCBReaderLoad(ccbiURL,proxy,ActivityExcLuaView)
    if node ~= nil then
        if nil ~= ActivityExcLuaView["m_titleTxt"] then
            self.m_titleTxt = tolua.cast(ActivityExcLuaView["m_titleTxt"],"cc.Label")
        end
        if self.m_titleTxt == nil then
            return false
        end
        if nil ~= ActivityExcLuaView["m_bgNode"] then
            self.m_bgNode = tolua.cast(ActivityExcLuaView["m_bgNode"],"cc.Node")
        end
        if self.m_bgNode == nil then
            return false
        end
        if nil ~= ActivityExcLuaView["m_listNode"] then
            self.m_listNode = tolua.cast(ActivityExcLuaView["m_listNode"],"cc.Node")
        end
        if self.m_listNode == nil then
            return false
        end
        if nil ~= ActivityExcLuaView["m_geLayer"] then
            self.m_geLayer = tolua.cast(ActivityExcLuaView["m_geLayer"],"cc.Layer")
        end
        if self.m_geLayer == nil then
            return false
        end
        if nil ~= ActivityExcLuaView["m_picNode"] then
            self.m_picNode = tolua.cast(ActivityExcLuaView["m_picNode"],"cc.Node")
        end
        if self.m_picNode == nil then
            return false
        end
        if nil ~= ActivityExcLuaView["m_desLabel"] then
            self.m_desLabel = tolua.cast(ActivityExcLuaView["m_desLabel"],"cc.Label")
        end
        if self.m_desLabel == nil then
            return false
        end
        if nil ~= ActivityExcLuaView["m_shalou"] then
            self.m_shalou = tolua.cast(ActivityExcLuaView["m_shalou"],"cc.Sprite")
        end
        if self.m_shalou == nil then
            return false
        end
        if nil ~= ActivityExcLuaView["m_timeLabel"] then
            self.m_timeLabel = tolua.cast(ActivityExcLuaView["m_timeLabel"],"cc.Label")
        end
        if self.m_timeLabel == nil then
            return false
        end
        if nil ~= ActivityExcLuaView["m_refreshBtn"] then
            self.m_refreshBtn = tolua.cast(ActivityExcLuaView["m_refreshBtn"],"cc.ControlButton")
        end
        if self.m_refreshBtn == nil then
            return
        end
        if nil ~= ActivityExcLuaView["m_numLabel"] then
            self.m_numLabel = tolua.cast(ActivityExcLuaView["m_numLabel"],"cc.Label")
        end
        if self.m_numLabel == nil then
            return false
        end
        if nil ~= ActivityExcLuaView["m_iconNode"] then
            self.m_iconNode = tolua.cast(ActivityExcLuaView["m_iconNode"],"cc.Node")
        end
        if self.m_iconNode == nil then
            return false
        end
        if nil ~= ActivityExcLuaView["m_btnLabel"] then
            self.m_btnLabel = tolua.cast(ActivityExcLuaView["m_btnLabel"],"cc.Label")
        end
        if self.m_btnLabel == nil then
            return false
        end
        if nil ~= ActivityExcLuaView["m_btnLabel1"] then
            self.m_btnLabel1 = tolua.cast(ActivityExcLuaView["m_btnLabel1"],"cc.Label")
        end
        if self.m_btnLabel1 == nil then
            return false
        end
        if nil ~= ActivityExcLuaView["m_btnLabel2"] then
            self.m_btnLabel2 = tolua.cast(ActivityExcLuaView["m_btnLabel2"],"cc.Label")
        end
        if self.m_btnLabel2 == nil then
            return false
        end
        if nil ~= ActivityExcLuaView["m_btnIconNode"] then
            self.m_btnIconNode = tolua.cast(ActivityExcLuaView["m_btnIconNode"],"cc.Node")
        end
        if self.m_btnIconNode == nil then
            return false
        end

        local screenSize = cc.Director:getInstance():getWinSize()
        local add = 0
        if isPad == true then
            add = screenSize.height - 2048
            add = add / 2.4
        else 
            add = screenSize.height - 852
        end
        self.m_bgNode:setPositionY(self.m_bgNode:getPositionY() - add)
        local bgNode = cc.Node:create()
        local h = 0
        while h < (852+add) do
            --do
            local spr = CCLoadSprite:createSprite("technology_09.png")
            spr:setPositionY(h)
            spr:setPositionX(0)
            spr:setAnchorPoint(cc.p(0,0))
            bgNode:addChild(spr)
            h = h + spr:getContentSize().height
        end
        self.m_bgNode:addChild(bgNode)

        self.m_listNode:setPositionY(self.m_listNode:getPositionY() - add)
        local listSize = self.m_listNode:getContentSize()
        self.m_listNode:setContentSize(cc.size(listSize.width,listSize.height+add))
        listSize = self.m_listNode:getContentSize()

        local layer = cc.Layer:create()
        if layer ~= nil then
            layer:setContentSize(cc.size(640, 250))
            layer:setAnchorPoint(cc.p(0, 0))
            layer:setTouchEnabled(true)
            layer:setSwallowsTouches(true)
            self.m_geLayer:addChild(layer)
        end
        self.m_iconNode:removeAllChildren()
        ActivityController:getInstance():createGoodsIcon(self.m_iconNode, self.m_itemId, 50)

        self.m_refreshBtn:setEnabled(true)
        self.m_desLabel:setColor(cc.c3b(197,130,71))
        self.m_timeLabel:setColor(cc.c3b(255, 229, 168))

        self.m_clipNode = CCClipNode:create(self.m_picNode:getContentSize())
        if self.m_clipNode ~= nil then
            local cpNode = tolua.cast(self.m_clipNode,"cc.Node")
            if cpNode ~= nil then
                cpNode:setAnchorPoint(cc.p(0, 0))
                self.m_picNode:addChild(cpNode)

                local spr1 = CCLoadSprite:createSprite("t_57034_ad1.png")
                if (spr1 ~= nil) then
                    spr1:setAnchorPoint(cc.p(0, 0))
                    spr1:setPositionY(-150)
                    cpNode:addChild(spr1)
                end
            end
        end

        self.m_tabView = cc.TableView:create(listSize)
        self.m_tabView:setDirection(cc.SCROLLVIEW_DIRECTION_VERTICAL)
        self.m_tabView:setVerticalFillOrder(cc.TABLEVIEW_FILL_TOPDOWN)
        self.m_tabView:setDelegate()
        self.m_listNode:addChild(self.m_tabView)
        self.m_tabView:registerScriptHandler(ActivityExcView.scrollViewDidScroll,cc.SCROLLVIEW_SCRIPT_SCROLL)
        -- self.m_tabView:registerScriptHandler(scrollViewDidZoom,cc.SCROLLVIEW_SCRIPT_ZOOM)
        self.m_tabView:registerScriptHandler(ActivityExcView.tableCellTouched,cc.TABLECELL_TOUCHED)
        self.m_tabView:registerScriptHandler(ActivityExcView.cellSizeForTable,cc.TABLECELL_SIZE_FOR_INDEX)
        self.m_tabView:registerScriptHandler(ActivityExcView.tableCellAtIndex,cc.TABLECELL_SIZE_AT_INDEX)
        self.m_tabView:registerScriptHandler(ActivityExcView.numberOfCellsInTableView,cc.NUMBER_OF_CELLS_IN_TABLEVIEW)
        

        self.m_desLabel:setString(LuaController:getLang("150329"))
        self.m_timeLabel:setString(LuaController:getSECLang(0))
        local tot = self.m_desLabel:getContentSize().width + 40 + self.m_timeLabel:getContentSize().width 
        self.m_desLabel:setPositionX(self.m_desLabel:getContentSize().width -126 - tot * 0.5)
        self.m_timeLabel:setPositionX(self.m_desLabel:getPositionX() + 40)
        self.m_shalou:setPositionX(self.m_desLabel:getPositionX() + 20)
        self.m_timeLabel:setString("")

        if (ActivityController:getInstance():isAct1DataInit(self.actID)==false) then
            ActivityController:getInstance():startGetAct6Data()
        else
            self:onEnterFrame()
        end

        self:addChild(node)
        self.ccbNode = node

        local schedulerEntry = nil
        local function enterframe()
            -- body
            self:onEnterFrame()
        end
        local function onNodeEvent(event)
            local scheduler = cc.Director:getInstance():getScheduler()
            if event == "enter" then
                print "ActivityExcView__enter"
                schedulerEntry = scheduler:scheduleScriptFunc(enterframe, 1.0, false)
                self:refreshView()
                self.m_titleTxt:setString(self.params:getProperty("name"))
                self.m_tabView:reloadData()
            elseif event == "exit" then
                print "ActivityExcView__exit"
                scheduler:unscheduleScriptEntry(schedulerEntry)
                self:onExit()
            end
        end
        self.ccbNode:registerScriptHandler(onNodeEvent)
    end
    return true
end
function ActivityExcView:refreshToolInfo()
    -- body
    if self.m_itemId ~= nil then
        local tinfo = ToolController:getInstance():getToolInfoForLua(tonumber(self.m_itemId))
        if tinfo ~= nil then
            local cntStr = tostring(tinfo:getCNT())
            print("ActivityExcView__refreshToolInfo" .. tostring(self.m_itemId) .. cntStr)
            local tot = "×" .. LuaController:getCMDLang(cntStr)
            self.m_numLabel:setString(tot)
        end
    end
end
function ActivityExcView:refreshBtn()
    -- body
    if ACTDATA ~= nil then
        self.m_btnIconNode:removeAllChildren()
        local flushTimes = ACTDATA:getProperty("flushTimes")
        local freeFreshNum = ACTDATA:getProperty("freeFreshNum")

        if (flushTimes < freeFreshNum) then
            self.m_btnLabel:setString(LuaController:getLang("103672"))
            self.m_btnLabel1:setString("")
            self.m_btnLabel2:setString("")
        else
            local num = 0
            local out = flushTimes - freeFreshNum + 1
            local freshcost = ACTDATA:getProperty("freshCost")
            local costSize = table.getn(freshcost)
            if costSize > 0 then
                if out < costSize then
                    num = freshcost[out]
                else
                    num = freshcost[costSize]
                end
            end
            self.m_btnLabel:setString("")
            if num > 0 then
                self.m_btnLabel1:setString(LuaController:getLang("150330"))
                local str = "×" .. LuaController:getCMDLang(tostring(num))
                self.m_btnLabel2:setString(str)
                ActivityController:getInstance():createGoodsIcon(self.m_btnIconNode, self.m_itemId, 40)
                local totw = self.m_btnLabel1:getContentSize().width + 42 + self.m_btnLabel2:getContentSize().width
                self.m_btnLabel1:setPositionX(164 - totw * 0.5 + self.m_btnLabel1:getContentSize().width)
                self.m_btnIconNode:setPositionX(self.m_btnLabel1:getPositionX() + 21)
                self.m_btnLabel2:setPositionX(self.m_btnLabel1:getPositionX() + 42)
            end
        end
    end
end
function ActivityExcView:refreshTimeLabel()
    if ACTDATA ~= nil then
        local gldata = GlobalData:shared()
        if gldata ~= nil then
            local left = ACTDATA:getProperty("refreshTime") - gldata:getTimeStamp()
            if left < 0 then
                left = 0
            end
            self.m_timeLabel:setString(LuaController:getSECLang(left))
        end
    end
end
function ActivityExcView:refreshView()
    -- body
    if (ActivityController:getInstance():isAct1DataInit(self.actID) == true) then
        local  dy1 = self.m_tabView:getContentOffset().y
        local minDy1 = self.m_tabView:minContainerOffset().y
        self.m_tabView:reloadData()
        local minDy2 = self.m_tabView:minContainerOffset().y
        local addDy = minDy2 - minDy1
        self.m_tabView:setContentOffset(cc.p(0,dy1 + addDy))

        self:refreshToolInfo()
        self:refreshTimeLabel()
        self:refreshBtn()
    end
end
function ActivityExcView:onClickRefresh()
    -- body
    if self.params == nil then
        return
    end
    
    if ActivityController:getInstance():isAct1DataInit(self.actID) then
        if ActivityController:getInstance():isCanRefreshByhand(self.actID) == false then
            CCCommonUtilsForLua:flyHint("","",LuaController:getLang("102198"))
        else
            ActivityController:getInstance():startAct6Refresh(self.actID)
        end
    end
end
function ActivityExcView:onHelpBtnClick()
    -- body
    local tips = LuaController:getLang("150331")
    PopupViewController:getInstance():addPopupView(TipsView:create(tips, cc.TEXT_ALIGNMENT_LEFT))
end
function ActivityExcView:onEnterFrame()
    -- body
    if ACTDATA ~= nil then
        self:refreshTimeLabel()
        if ActivityController:getInstance():isAct1DataShouldRefresh(self.actID) then
            ActivityController:getInstance():startAct6Refresh(self.actID)
        end
        if ActivityController:getInstance():checkAct1ReFreshTimes(self.actID) then
             self:refreshBtn()
        end
    end
end
function ActivityExcView:onExit()
    ActivityExcLuaView.onClickCMDBtn=nil
    ActivityExcLuaView.onClickNewBtn=nil
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
function ActivityExcView.scrollViewDidScroll(view)
    local mindy = view:minContainerOffset().y
    local dy = view:getContentOffset().y
    if dy < mindy then
        view:setContentOffset(cc.p(0,mindy))
    end
end

function ActivityExcView.tableCellTouched(table,cell)
    -- print("ActivityExcView__cell touched at index: " .. cell:getIdx())
end

function ActivityExcView.cellSizeForTable(table,idx) 
    return 640, 120
end

function ActivityExcView.tableCellAtIndex(table, idx)
    if ACTDATA == nil then
        return nil
    end
    if idx >= ACTDATA:getDataSize() then
        return nil
    end
    local excKey = ACTDATA:getAct1ExcKeyByIdx(idx)
    if excKey < 0 then
         return nil
    end
    local cell = table:dequeueCell()
    if cell ~= nil then 
        local node = cell:getChildByTag(100)
        if node ~= nil then
            node:setData(excKey,false)
        end
    else
        local node = ActivityExcCell:create(excKey)
        if node ~= nil then
            node:setTag(100)
            cell = cc.TableViewCell:new()
            cell:addChild(node)
        end
    end
    return cell
end

function ActivityExcView.numberOfCellsInTableView(table)
    --print("ActivityExcView:numberOfCellsInTableView")
    if ACTDATA == nil then
        --print("ActivityExcView:numberOfCellsInTableView_data_nil")
        return 0
    end
    local dataSize = ACTDATA:getDataSize()
    --print("ActivityExcView:numberOfCellsInTableView_data_size " .. tostring(dataSize))
    return dataSize
end
function ActivityExcView:CMDCallbackForLua(cmdname,data)
    -- body
    -- print("ActivityExcView:CMDCallbackForLua")
end
function ActivityExcView:NotificationCallbackForLuaWithParams(cmdname,object)
    -- body
    -- print("ActivityView:NotificationCallbackForLuaWithParams")
    if cmdname == "msg.act6.exc.success" then
        local cLv = object
        if cLv <= 0 then
            print("cLv is nil")
            return
        end
        if self.m_tabView == nil then
            return
        end
        local c = self.m_tabView:getContainer()
        if nil == c then
            return
        end
        local ccnt = c:getChildrenCount()
        if ccnt <=0 then
            return
        end

        local children = c:getChildren()
        if children == nil then
            return
        end
        for i=1,ccnt do
            local cell = children[i]
            if cell ~= nil then
                local node = cell:getChildByTag(100)
                if node ~= nil then
                    if node:getLevel() == cLv then
                        node:GetMsgRefreshEnd(false)
                        print("node:GetMsgRefreshEnd")
                        break
                    else
                        print("node:GetMsgRefreshEnd nn")
                    end
                end
            end
        end
    elseif cmdname == "msg.act6.refresh.success" then
        -- local clv = object
        print("back msg.act6.refresh.success")
        if self.m_tabView == nil then
            return
        end
        local c = self.m_tabView:getContainer()
        if nil == c then
            return
        end
        local ccnt = c:getChildrenCount()
        if ccnt <=0 then
            return
        end

        local children = c:getChildren()
        if children == nil then
            return
        end
        for i=1,ccnt do
            local cell = children[i]
            if cell ~= nil then
                local node = cell:getChildByTag(100)
                if node ~= nil then
                    node:GetMsgRefreshEnd(true)
                end
            end
        end
        self:refreshToolInfo()
        self:refreshBtn()
    end
end
function ActivityExcView:NotificationCallbackForLua(cmdname)
    -- body
    if cmdname == "msg.get.act6.data.success" then
        self:refreshView()
    elseif cmdname == "msg_refreash_tool_data" then
        self:refreshToolInfo()
    end
end