require "externCOK2"
require "CCBReaderLoadCOK2"
require "commonCOK2"
require "Cocos2dCOK2"

GoldExchangeAdvLuaCell  = GoldExchangeAdvLuaCell or {}
ccb["GoldExchangeAdvLuaCell"] = GoldExchangeAdvLuaCell

GoldExchangeAdvCell = class("GoldExchangeAdvCell",
	function()
        return cc.Layer:create() 
	end
)
GoldExchangeAdvCell.__index = GoldExchangeAdvCell
function GoldExchangeAdvCell:create(parent,path,params)
    ----print "function GoldExchangeAdvCell:create(parent,path,params)"
	local node = GoldExchangeAdvCell.new()
	node:init(parent,path,params)
	return node
end
function GoldExchangeAdvCell:init(parent,path,params)
    ----print "function GoldExchangeAdvCell:init(parent,path,params)"
    local strPath = string.format(path)
	self.rootPath = strPath
    ----print "parent....is....."
    ----print (parent)
    self.parentNode = parent
    
    self:initData(params)
    self:initItems()
    self.startPos = cc.p(0, 0)

    loadLuaResource(self.rootPath .. "/resources/".. self.data[19] .."adv.plist")

    local  proxy = cc.CCBProxy:create()
    GoldExchangeAdvLuaCell.onClickCostBtn = function()
    	self:onClickCostBtn()
	end
    GoldExchangeAdvLuaCell.onPackageBtnClick = function()
        self:onPackageBtnClick()
    end
    local ccbiURL = strPath .. "/ccbi/GoldExchangeAdv"..self.data[19].."LuaCell_NEW.ccbi"
    ----print ("adv ccb :" .. ccbiURL)

	local ccbnode = CCBReaderLoad(ccbiURL,proxy,GoldExchangeAdvLuaCell)
    local  layer = tolua.cast(ccbnode,"cc.Layer")
    ----print "if nil ~= GoldExchangeAdvLuaCell[m_timeLabel] then"
    if nil ~= GoldExchangeAdvLuaCell["m_timeLabel"] then
        self.m_timeLabel = tolua.cast(GoldExchangeAdvLuaCell["m_timeLabel"],"cc.Label")
    end
    ----print "GoldExchangeAdvLuaCell m_percentLabel"
    if nil ~= GoldExchangeAdvLuaCell["m_percentLabel"] then
        self.m_percentLabel = tolua.cast(GoldExchangeAdvLuaCell["m_percentLabel"],"cc.Label")
    end
    ----print "GoldExchangeAdvLuaCell m_moreLabel"
    if nil ~= GoldExchangeAdvLuaCell["m_moreLabel"] then
        self.m_moreLabel = tolua.cast(GoldExchangeAdvLuaCell["m_moreLabel"],"cc.Label")
    end
    ----print "GoldExchangeAdvLuaCell m_desLabel"
    if nil ~= GoldExchangeAdvLuaCell["m_desLabel"] then
        self.m_desLabel = tolua.cast(GoldExchangeAdvLuaCell["m_desLabel"],"cc.Label")
    end
    ----print "GoldExchangeAdvLuaCell m_getLabel"
    if nil ~= GoldExchangeAdvLuaCell["m_getLabel"] then
        self.m_getLabel = tolua.cast(GoldExchangeAdvLuaCell["m_getLabel"],"cc.Label")
        if nil ~= self.m_getLabel then
    		--local getStr = string.format(LuaController:getLang1("115073",""))
    		--self.m_getLabel:setString(getStr)
    		self.m_getLabel:setVisible(false)
    	end
    end
    ----print "if nil ~= GoldExchangeAdvLuaCell[m_newPriceLabel] then"
    if nil ~= GoldExchangeAdvLuaCell["m_newPriceLabel"] then
        --print "nil ~= GoldExchangeAdvLuaCell[m_newPriceLabel]"
        self.m_newPriceLabel = tolua.cast(GoldExchangeAdvLuaCell["m_newPriceLabel"],"cc.Label")
    end
    ----print "if nil ~= GoldExchangeAdvLuaCell[m_newPriceLabel] then"

    if nil ~= GoldExchangeAdvLuaCell["m_getGoldNumText"] then
        self.m_getGoldNumText = tolua.cast(GoldExchangeAdvLuaCell["m_getGoldNumText"],"cc.LabelBMFont")
    end
    ----print "if nil ~= GoldExchangeAdvLuaCell[m_costBtn] then"

    if nil ~= GoldExchangeAdvLuaCell["m_costBtn"] then
        self.m_costBtn = tolua.cast(GoldExchangeAdvLuaCell["m_costBtn"],"cc.ControlButton")
        if nil ~= self.m_costBtn then
            --LuaController:addButtonLight(self.m_costBtn)
        end
    end
    ----print "if nil ~= GoldExchangeAdvLuaCell[m_showMoneyNode] then"

    if nil ~= GoldExchangeAdvLuaCell["m_showMoneyNode"] then
    	self.m_showMoneyNode = tolua.cast(GoldExchangeAdvLuaCell["m_showMoneyNode"],"cc.LayerColor")
    end
    ----print "if nil ~= GoldExchangeAdvLuaCell[m_showMoreNode] then"
    
    if nil ~= GoldExchangeAdvLuaCell["m_showMoreNode"] then
    	self.m_showMoreNode = tolua.cast(GoldExchangeAdvLuaCell["m_showMoreNode"],"cc.LayerColor")
    	if nil ~= self.m_showMoreNode then
    		--print ("show more node 111111")
            if self.m_showMoreNode:isVisible() == true then
                local function onTouchBegan(x, y)
                    --print "11"
                    if(nil ~= self.m_showMoreNode) then
                    --print "12"
                        if(nil ~= self.m_showMoreNode:getParent()) then
                    --print "13"
                            local pos = self.m_showMoreNode:getParent():convertToNodeSpace(CCPoint(x,y))
                    --print "14"
                            local rect = self.m_showMoreNode:getBoundingBox()
                    --print "15"
                            if(cc.rectContainsPoint(rect, pos) == true) then
                    --print "16"
                                self.startPos = cc.p(x, y)
                                return true
                            end 
                        end
                    end
                    return false
                end
                local function onTouchMoved(x, y)
                    --print "touch move"
                end
                local function onTouchEnded(x, y)
                    if(nil == self.m_showMoreNode) then
                        --print "touch node empty"
                        return
                    end

                    if self.m_showMoreNode:isVisible() == false then
                        --print "touch node hide"
                        return
                    end

                    if(nil == self.m_showMoreNode:getParent()) then
                        --print "touch node parent empty"
                        return
                    end

                    --print "local pos = self.m_showMoreNode:getParent():convertToNodeSpace(CCPoint(x,y))"
                    local pos0 = self.startPos
                    if nil == pos0 then
                        return
                    end
                    self.startPos = cc.p(0, 0)
                    local pos = self.m_showMoreNode:getParent():convertToNodeSpace(CCPoint(x,y))
                    --print "local pos = self.m_showMoreNode:getParent():convertToNodeSpace(CCPoint(x,y))"
                    local rect = self.m_showMoreNode:getBoundingBox()
                    if(cc.rectContainsPoint(rect, pos) == true) then
                        if cc.pGetDistance(pos0, cc.p(x, y)) < 10 then
                            local itemid = string.format(self.data[1])
                            LuaController:showDetailPopup(itemid)
                            --print "touch inside"
                            return
                        end
                    end
                    --print "touch outside"
                end

                local function onTouch(eventType, x, y)
                    --print (eventType)  
                    if eventType == "began" then
                        --print "return self:onTouchBegan(x, y)"  
                        return onTouchBegan(x, y)  
                    elseif eventType == "moved" then  
                        --print "return self:onTouchMoved(x, y) "
                        return onTouchMoved(x, y)  
                    else  
                        --print "return self:onTouchEnded(x, y) "
                        return onTouchEnded(x, y)  
                    end
                end
                ----print "self.m_showMoreNode:registerScriptTouchHandler(onTouch)"
                self.m_showMoreNode:registerScriptTouchHandler(onTouch)
                ----print "end self.m_showMoreNode:registerScriptTouchHandler(onTouch)"
                self.m_showMoreNode:setTouchEnabled(true)
                self.m_showMoreNode:setSwallowsTouches(false)
            end
        end
    end
    ----print "if nil ~= GoldExchangeAdvLuaCell[m_packageBtn] then"
    if nil ~= GoldExchangeAdvLuaCell["m_packageBtn"] then
        ----print "~~~~1"
        self.m_packageBtn = tolua.cast(GoldExchangeAdvLuaCell["m_packageBtn"],"cc.ControlButton")
        ----print(GoldExchangeAdvLuaCell["m_packageBtn"])
        ----print "~~~~2"

        if nil ~= self.m_packageBtn then
        ----print "~~~~3"
        
            local showPackBtn = false
        ----print "~~~~4"
            if self.data[21] ~= nil then
        ----print "~~~~5"
                if self.data[21] == "1" then
        ----print "~~~~6"
                    showPackBtn = true
        ----print "~~~~7"
                end
        ----print "~~~~8"
            end
        ----print "~~~~9"
            if showPackBtn == true then
        ----print "~~~~10"
                self.m_packageBtn:setVisible(true)
        ----print "~~~~11"
            else
        ----print "~~~~12"
                self.m_packageBtn:setVisible(false)
        ----print "~~~~13"
        ----print "~~~~19"
            end
        ----print "~~~~20"
            --LuaController:addButtonLight(self.m_costBtn)
        end
    end
    if nil ~= GoldExchangeAdvLuaCell["m_lblDesc1"] then
        self.m_lblDesc1 = tolua.cast(GoldExchangeAdvLuaCell["m_lblDesc1"],"cc.Label")
    end

    ----print "self:initView()"
    self:initView()
    ----print "after:initView()"
    local function scheduleBack()
    ----print "_____GoldExchangeAdvCell:scheduleBack"
    if(nil ~= self.m_timeLabel) then
        local curTime = LuaController:getWorldTime()
        local lastTime = 0
        local expTime = tonumber(self.data[14])
        local endTime = tonumber(self.data[13])
        if expTime>0 then
            local gapTime = endTime - curTime
            local count =  gapTime / (expTime * 3600)
            lastTime = endTime - (expTime*3600)*count-curTime;
        else
            lastTime = endTime - curTime
        end
        local timeStr = LuaController:getSECLang(lastTime)
        local sss = string.format(timeStr)
        self.m_timeLabel:setString(sss)
        if (endTime - curTime <= 0 ) then
            self.m_costBtn:setEnabled(false)
            self:removeAllEvent()
            LuaController:removeAllPopup()
        end 
    end
    end

    local function eventHandler( eventType )
            if eventType == "enter" then
                ----print " GoldExchangeAdvCell enter"
                scheduleBack()
                self.m_entryId = tonumber(ccbnode:getScheduler():scheduleScriptFunc(scheduleBack, 1, false))
            elseif eventType == "exit" then
                ----print "GoldExchangeAdvCell exit"
                if nil ~= self.m_entryId then
                    ccbnode:getScheduler():unscheduleScriptEntry(self.m_entryId)
                end
            elseif eventType == "cleanup" then
                ----print "GoldExchangeAdvCell cleanup"
                ccbnode:unregisterScriptHandler()
            end
    end
    ----print "ccbnode:registerScriptHandler(eventHandler)"
    ccbnode:registerScriptHandler(eventHandler)
    ----print "self.parentNode:addChild(ccbnode)"
    local ttt = tolua.cast(self.parentNode, "cc.Node")
    local ttt2 = tolua.cast(ccbnode, "cc.Node")
    ----print(ttt)
    ----print (ttt2)
    -- self.parentNode:addChild(ccbnode)
    self.parentNode:addChild(self)
    self:addChild(ccbnode)
    ----print "self:setContentSize(CCSize(540, 220))"
    self:setContentSize(CCSize(616, 246))
    ----print "ccbnode:setPositionY(-66)"
    -- ccbnode:setPositionY(-66)
    ----print "self.ccbNode = ccbnode"
    self.ccbNode = ccbnode
----print "12313"
    --[[local function onNodeEvent(event)
        if event == "enter" then
            ------print "gold_exchange_adv_cell_enter"
        elseif event == "exit" then
            ------print "gold_exchange_adv_cell_exit"
            self:removeAllEvent()
        end
    end
    self.ccbNode:registerScriptHandler(onNodeEvent)]]
end

function GoldExchangeAdvCell:initData(params)
    local paramsStr = string.format(params)
    ------print("params:" .. paramsStr)
    self.data = {}
    local index = 1
    local startI = 1
    local fIndex = string.find(paramsStr,",",startI)
    local tmpValue = "" 
    while (true) do
        tmpValue = string.sub(paramsStr,startI,fIndex-1)
        ------print(string.format(startI) .. "," .. string.format(fIndex-1) .. ", params" .. string.format(index) .. ":" .. tmpValue)
        self.data[index] = tmpValue
        index = index + 1
        startI = fIndex + 1
        fIndex = string.find(paramsStr,",",startI)
        if (fIndex == nil) then
            tmpValue = string.sub(paramsStr,startI,string.len(paramsStr))
            ------print( string.format(startI) .. "," .. string.format(string.len(paramsStr)) .. ", params" .. string.format(index) .. ":" .. tmpValue)
            self.data[index] = tmpValue
            break
        end
    end
end
function GoldExchangeAdvCell:initItems()
    -----print("GoldExchangeAdvCell:initItems1")
    self.items = nil
    if nil == self.data[7] then
        ----print("GoldExchangeAdvCell:initItems2")
        return
    end
    local itemsStr = string.format(self.data[7])
    if(itemsStr == "") then
        ----print("GoldExchangeAdvCell:initItems3")
        return
    end
    
    ----print("items:"..itemsStr)
    self.items = {}
    local itemIndex = 1
    local itemSIndex = 1
    local itemFIndex = string.find(itemsStr,"|",itemSIndex)

    if itemFIndex == nil then
        local itemValueTmp = string.sub(itemsStr,itemSIndex,string.len(itemsStr))
        local itemFindIndex =  string.find(itemValueTmp,";",1)
        local tabelValue1 = string.sub(itemValueTmp,1,itemFindIndex-1)
        local tabelVaule2 = string.sub(itemValueTmp,itemFindIndex+1,string.len(itemValueTmp))
        local iValue = {tabelValue1,tabelVaule2}
        self.items[itemIndex] = iValue;
        ----print("items" .. string.format(itemIndex) .. ":" .. tabelValue1 .."," .. tabelVaule2)
        return
    end


--    while(true) do
--        local itemValueTmp = string.sub(itemsStr,itemSIndex,itemFIndex-1)
--        local itemFindIndex =  string.find(itemValueTmp,";",1);
--        local tabelValue1 = string.sub(itemValueTmp,1,itemFindIndex-1)
--        local tabelVaule2 = string.sub(itemValueTmp,itemFindIndex+1,string.len(itemValueTmp))
--        local iValue = {tabelValue1,tabelVaule2}
--        self.items[itemIndex] = iValue;
        ----print("items" .. string.format(itemIndex) .. ":" .. tabelValue1 .."," .. tabelVaule2)
--        itemIndex = itemIndex + 1
--        itemSIndex =  itemFIndex + 1
--        itemFIndex = string.find(itemsStr,"|",itemSIndex)
--        if itemFIndex == nil then
--            itemValueTmp = string.sub(itemsStr,itemSIndex,string.len(itemsStr))
--            itemFindIndex =  string.find(itemValueTmp,";",1)
--            tabelValue1 = string.sub(itemValueTmp,1,itemFindIndex-1)
--            tabelVaule2 = string.sub(itemValueTmp,itemFindIndex+1,string.len(itemValueTmp))
--            iValue = {tabelValue1,tabelVaule2}
--            self.items[itemIndex] = iValue;
            ----print("items" .. string.format(itemIndex) .. ":" .. tabelValue1 .."," .. tabelVaule2)
--            return
--        end
--    end
end
function GoldExchangeAdvCell:initView()
    ----print "function GoldExchangeAdvCell:initView()"
    if nil ~= self.m_getGoldNumText then
        local numKey = string.format(self.data[3]);
        local goldstr = "Gold+"
        local numStr = string.format(LuaController:getCMDLang(numKey))
        local strstr = goldstr .. numStr
        local sss = string.format(strstr)
        self.m_getGoldNumText:setString(sss)
    end
    ----print "1"
    if nil ~= self.m_percentLabel then
        local percentStr = string.format(self.data[8])
        -- percentStr = percentStr .. "%"
        if self.data[19] == "hot_sale_1" then
            percentStr = percentStr .. "% OFF"
        else
            percentStr = percentStr .. "%"
        end
        print("(NEWCOK2-adv-cell)self.data[19]:"..self.data[19]..";percentStr:"..percentStr)
        print "aaaaaaa"
        -- local sss = string.format(percentStr)
        print "bbbbbbb"
        self.m_percentLabel:setString(percentStr)
        print "ccccccc"
        self.m_percentLabel:setScale(1.2)
    end
    ----print "2"
    local itemNull = false
    local itemStr = string.format(self.data[7])
    ----print "3"
    if string.len(itemStr) == 0 then
    	if nil ~= self.m_moreLabel then
    		self.m_moreLabel:setString("")
    	end
    	if nil ~= self.m_showMoreNode then
    		self.m_showMoreNode:setVisible(false)
    	end
    	if nil ~= self.m_desLabel then
    		self.m_desLabel:setString("")
    	end
    else
        if self.data[19] ~= "envelope_gift" then
            if nil ~= self.m_moreLabel then
                local moreStr = string.format(LuaController:getLang("102271"))
                self.m_moreLabel:setString(moreStr)
            end
            if nil ~= self.m_showMoreNode then
                self.m_showMoreNode:setVisible(true)
            end
            if nil ~= self.m_desLabel then
                local desStr = string.format(LuaController:getLang("101237"))
                self.m_desLabel:setString(desStr)
            end
        end
    end
    	
    ----print "4"
    if nil ~= self.m_newPriceLabel then
		local dollar = string.format(self.data[4])
    	local pID = string.format(self.data[11])
    	local newPrice = string.format(LuaController:getDollarString(dollar,pID))
    	self.m_newPriceLabel:setString(newPrice)
	end
    ----print "5"

    if self.data[19] == "envelope_gift" then
        if nil ~= self.m_lblDesc1 then
            if self.items[1] ~= nil then
                if self.items[1][2] ~= nil then
                    local itemCount = string.format(self.items[1][2])
                    local desStr = string.format(LuaController:getLang1("101045",itemCount))
                    self.m_lblDesc1:setString(desStr)
                end
            end
        end
        if nil ~= self.m_showMoneyNode then
            self.m_showMoneyNode:setVisible(false)
        end
        if nil ~= self.m_desLabel then
            self.m_desLabel:setVisible(false)
        end
        if nil ~= self.m_getLabel then
            self.m_getLabel:setVisible(false)
        end
    end
end
function GoldExchangeAdvCell:removeAllEvent()
    --releaseLuaResource(self.rootPath .. "/resources/".. self.data[19] .."adv")
    GoldExchangeAdvLuaCell.onClickCostBtn = nil
    if nil ~= self.m_showMoreNode then
        self.m_showMoreNode:unregisterScriptTouchHandler()
    end
    if nil ~= self.m_costBtn then
        self.m_costBtn:setEnabled(false)
    end
    if nil ~= self.m_timeLabel then
        self.m_timeLabel:stopAllActions()
    end
    --[[if nil ~= self.ccbNode then
        self.ccbNode:unregisterScriptHandler()
    end]]
end
--[[function GoldExchangeAdvCell:scheduleBack()
    ----print "_____GoldExchangeAdvCell:scheduleBack"
    if(self.m_timeLabel) then
        local curTime = LuaController:getWorldTime()
        local lastTime = 0
        local expTime = tonumber(self.data[14])
        local endTime = tonumber(self.data[13])
        if expTime>0 then
            local gapTime = endTime - curTime
            local count =  gapTime / (expTime * 3600)
            lastTime = endTime - (expTime*3600)*count-curTime;
        else
            lastTime = endTime - curTime
        end
        local timeStr = LuaController:getSECLang(lastTime)
        self.m_timeLabel:setString(timeStr)
        if (endTime - curTime <= 0 ) then
            self.m_costBtn:setEnabled(false)
            self:removeAllEvent()
            LuaController:removeAllPopup()
        end 
    end
end]]
function GoldExchangeAdvCell:onClickCostBtn()
    local itemid = string.format(self.data[1])
    LuaController:callPayment(itemid)
    self:removeAllEvent()
    LuaController:removeAllPopup()
end
function GoldExchangeAdvCell:onPackageBtnClick()
    if nil ~= self.data[21] then
        self:removeAllEvent()
        LuaController:removeAllPopup()
        local itemid = string.format(self.data[1])
        LuaController:toSelectUser(itemid,false,2)
    end
end
-- function GoldExchangeAdvCell:onTouchBegan(x, y)
--     --print "11"
-- 	if(nil ~= self.m_showMoreNode) then
--     --print "12"
--         if(nil ~= self.m_showMoreNode:getParent()) then
--     --print "13"
--             local pos = self.m_showMoreNode:getParent():convertToNodeSpace(CCPoint(x,y))
--     --print "14"
--             local rect = self.m_showMoreNode:getBoundingBox()
--     --print "15"
--             if(cc.rectContainsPoint(pos) == true) then
--     --print "16"
--                 return true
--             end 
--         end
--     end
--     return false
-- end
-- function GoldExchangeAdvCell:onTouchMoved(x, y)
-- 	--print "touch move"
-- end
-- function GoldExchangeAdvCell:onTouchEnded(x, y)
--     if(nil == self.m_showMoreNode) then
--     	--print "touch node empty"
--     	return
--     end

--     if self.m_showMoreNode:isVisible() == false then
--     	--print "touch node hide"
--     	return
--     end

--    	if(nil == self.m_showMoreNode:getParent()) then
--    		--print "touch node parent empty"
--    		return
--    	end

--     --print "local pos = self.m_showMoreNode:getParent():convertToNodeSpace(CCPoint(x,y))"
--     local pos = self.m_showMoreNode:getParent():convertToNodeSpace(CCPoint(x,y))
--     --print "local pos = self.m_showMoreNode:getParent():convertToNodeSpace(CCPoint(x,y))"
--     local rect = self.m_showMoreNode:getBoundingBox()
--     if(cc.rectContainsPoint(pos) == true) then
--     	local itemid = string.format(self.data[1])
--     	LuaController:showDetailPopup(itemid)
--     	--print "touch inside"
--     	return
--    	end
--    	--print "touch outside"
-- end
function GoldExchangeAdvCell:getCostBtnRect()
    ----print "function GoldExchangeAdvCell:getCostBtnRect()"
    if nil ~= self.m_costBtn then
        ----print "1"
        local size = self.m_costBtn:getContentSize()
        ----print "1"
        local pos = CCPoint(self.m_costBtn:getPosition())
        ------print ("pos1:[" .. string.format(pos.x) .. "," .. string.format(pos.y) .. "]")
        ----print "1"
        pos = self.m_costBtn:getParent():convertToWorldSpace(pos)
        ------print ("pos2:[" .. string.format(pos.x) .. "," .. string.format(pos.y) .. "]")
        ----print "1"
        pos = self.ccbNode:convertToNodeSpace(pos)
        ------print ("pos3:[" .. string.format(pos.x) .. "," .. string.format(pos.y) .. "]")
        ------print ("size:[" .. string.format(size.width) .. "," .. string.format(size.height) .. "]")
        return pos.x, pos.y, size.width, size.height
    end
    ----print "return 0,0,0,0"
    return 0,0,0,0
end