require "externCOK2"
require "CCBReaderLoadCOK2"
require "commonCOK2"

ActivityAdLuaCell = ActivityAdLuaCell or {}
ccb["ActivityAdLuaCell"] = ActivityAdLuaCell

ActivityAdCell = class("ActivityAdCell", function() return cc.Layer:create() end)
ActivityAdCell.__index = ActivityAdCell

function ActivityAdCell:create( path, params)
    --print "---203----"

    local node = ActivityAdCell.new()
    --print "---204----"

    node:init(path, params)
    return node
end

function ActivityAdCell:init( path, params )
    --print "---205----"

    self.rootPath = string.format(path)
    --print "---206----"
    
    self.params = params
    --print "---207----"
    
    loadLuaResource(self.rootPath .. "/resources/activity_" .. self.params.id .. ".plist")

    --print "---208----"

    local proxy = cc.CCBProxy:create()
    --print "---209----"

    local ccbiURL = self.rootPath .. "/ccbi/Activity" .. self.params.id .. "AdLuaCell.ccbi"
    --print "---210----"

    local node = CCBReaderLoad(ccbiURL, proxy, ActivityAdLuaCell)
    --print "---211----"

    local layer = tolua.cast(node, "cc.Layer")
    --print "---212----"
    

    if nil ~= ActivityAdLuaCell["m_nameLabel"] then
        --print "---213----"
        self.m_nameLabel = tolua.cast(ActivityAdLuaCell["m_nameLabel"], "cc.Label")
        --print "---214----"
        if nil ~= self.m_nameLabel then
            --print "---215----"
            self.m_nameLabel:setString(self.params.name)
            --print "---216----"
            if self.params.id ~= "57006" then
                --print "---217----"
                local tc = self.m_nameLabel:getColor()
                --print "---218----"
                tc.r = 255
                --print "---219----"
                tc.g = 219
                tc.b = 117
                if self.params.id == "57014" then
                    --print "---220----"
                    tc.r = 144
                    tc.g = 234
                    tc.b = 255
                end
                if self.params.id == "57015" then
                    --print "---221----"
                    tc.r = 0
                    tc.g = 228
                    tc.b = 255
                end
                --print "---222----"
                if self.params.id == "57022" then
                    tc.r = 204
                    tc.g = 180
                    tc.b = 114
                end
                if self.params.id == "57023" then
                    tc.r = 238
                    tc.g = 199
                    tc.b = 105
                end
                if self.params.id == "57026" then
                    tc.r = 255
                    tc.g = 250
                    tc.b = 231
                end
                if self.params.id == "57030" then
                    tc.r = 255
                    tc.g = 248
                    tc.b = 208
                end
                if self.params.id == "57035" then
                    tc.r = 255
                    tc.g = 248
                    tc.b = 208
                end
                self.m_nameLabel:setColor(tc)
            end
        end
    end

    if nil ~= ActivityAdLuaCell["m_desLabel"] then
        self.m_desLabel = tolua.cast(ActivityAdLuaCell["m_desLabel"], "cc.Label")
        if nil ~= self.m_desLabel then
            self.m_desLabel:setString(self.params.desc)
            if self.params.id ~= "57006" then
                local tc = self.m_desLabel:getColor()
                tc.r = 0
                tc.g = 249
                tc.b = 0
                if self.params.id == "57014" then
                    tc.r = 209
                    tc.g = 132
                    tc.b = 189
                end
                if self.params.id == "57015" then
                    tc.r = 246
                    tc.g = 255
                    tc.b = 255
                end
                if self.params.id == "57022" then
                    tc.r = 204
                    tc.g = 180
                    tc.b = 114
                end
                if self.params.id == "57026" then
                    tc.r = 249
                    tc.g = 140
                    tc.b = 33
                end
                if self.params.id == "57027" then
                    tc.r = 249
                    tc.g = 222
                    tc.b = 183
                end
                if self.params.id == "57030" then
                    tc.r = 255
                    tc.g = 236
                    tc.b = 130
                end
                if self.params.id == "57035" then
                    tc.r = 255
                    tc.g = 236
                    tc.b = 130
                end
                self.m_desLabel:setColor(tc)
            end
        end
    end

    if nil ~= ActivityAdLuaCell["m_timeLabel"] then
        self.m_timeLabel = tolua.cast(ActivityAdLuaCell["m_timeLabel"], "cc.Label")
    end

    if nil ~= node then
        local function tick(  )
            --print ("tick()")
            --print "---223----"
            --if nil == self.m_timeLabel then return end
            --print "---224----"
            --if nil ~= self.m_timeLabel and "6" == self.params.type then
                --self.m_timeLabel:setString("")
                --return
            --end
            local nowTime = LuaController:getWorldTime()
            --print (nowTime)
            --print "---225----"
            local startTime = tonumber(self.params.startTime)
            --print "---226----"

            local endTime = tonumber(self.params.endTime)
            --print "---227----"

            if nowTime < startTime then
            --print "---228----"

                self.m_timeLabel:setString(string.format(LuaController:getLang1("105804", string.format(LuaController:getSECLang(startTime - nowTime)))))
            elseif nowTime< endTime then
            --print "---229----"

                self.m_timeLabel:setString(string.format(LuaController:getLang1("105805", string.format(LuaController:getSECLang(endTime - nowTime)))))
            else
            --print "---230----"

                self.m_timeLabel:setString(string.format(LuaController:getLang("105800")))
            end
        end
        local function eventHandler( eventType )
            if eventType == "enter" then
                --print " adcell enter"
                tick()
                self.m_entryId = tonumber(node:getScheduler():scheduleScriptFunc(tick, 1, false))
            elseif eventType == "exit" then
                --print "ad cell exit"
                if nil ~= self.m_entryId then
                    node:getScheduler():unscheduleScriptEntry(self.m_entryId)
                end
            elseif eventType == "cleanup" then
                --print "ad cell cleanup"
                releaseLuaResource(self.rootPath .. "/resources/activity_" .. self.params.id)
                node:unregisterScriptHandler()
            end
        end
        --print "---225----"
        node:registerScriptHandler(eventHandler)
        --print "---226----"
        self:addChild(node)
        --print "---227----"
    end
end