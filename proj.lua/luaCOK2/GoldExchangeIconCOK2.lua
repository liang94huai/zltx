require "externCOK2"
require "CCBReaderLoadCOK2"
require "commonCOK2"
require "Cocos2dCOK2"

GoldExchangeLuaIcon  = GoldExchangeLuaIcon or {}
ccb["GoldExchangeLuaIcon"] = GoldExchangeLuaIcon

GoldExchangeIcon = class("GoldExchangeIcon",
	function()
        return cc.Layer:create() 
	end
)
GoldExchangeIcon.__index = GoldExchangeIcon
function GoldExchangeIcon:create(parent,path,params)
    --print "GoldExchangeIcon:create(parent,path,params)"
	local node = GoldExchangeIcon.new()
	node:init(parent,path,params)
	return node
end
function GoldExchangeIcon:init(parent,path,params)
	local strPath = string.format(path)
    self.rootPath = strPath
    self.parentNode = parent
    self:setData(params)

    loadLuaResource(self.rootPath .. "/resources/".. self.data[19] .."icon.plist")

    local ccbiUrl = strPath .. "/" .. self:getCCBI()
    ----print(ccbiUrl)
    local proxy = cc.CCBProxy:create()
    --print "GoldExchangeIcon:init(parent,path,params)"
   	local ccbnode = CCBReaderLoad(ccbiUrl,proxy,GoldExchangeLuaIcon)
    if ccbnode ~= nil then
        --print "-----401----"
        local layer = tolua.cast(ccbnode,"cc.Layer")
        --print "-----402----"

        if nil ~= GoldExchangeLuaIcon["m_timeLabel"] then
        --print "-----403----"

            self.m_timeLabel = tolua.cast(GoldExchangeLuaIcon["m_timeLabel"],"cc.Label")
            --self.m_timeLabel = nil
        end
        --print "-----404----"

        if nil ~= GoldExchangeLuaIcon["m_ani1"] then
        --print "-----405----"

            self.m_ani1Layer = tolua.cast(GoldExchangeLuaIcon["m_ani1"],"cc.LayerColor")
        end
        --print "-----406----"

        if nil ~= GoldExchangeLuaIcon["m_ani2"] then
        --print "-----407----"

            self.m_ani2Layer = tolua.cast(GoldExchangeLuaIcon["m_ani2"],"cc.LayerColor")
        end
        --print "-----408----"

        self:initParticale()
        --print "-----409----"

        self:initSkeleton()
        --print "-----410----"

        if nil ~= self.parentNode then
        --print "-----411----"

            -- self.parentNode:addChild(ccbnode)
            self.parentNode:addChild(self)
            self:addChild(ccbnode)
        end
        --print "-----412----"

        if self.data[19] == "normal_gift" then
        --print "-----413----"

            if nil ~= self.data[10] then
                local keyStr = string.format(self.data[10])
                local titleStr = string.format(LuaController:getLang(keyStr))
                self.m_timeLabel:setString(titleStr)
            end
        else
        --print "-----414----"
        local function scheduleBack(  )
            --print "_____GoldExchangeIcon:scheduleBack"
            if self.data ~= nil then
            if nil ~= self.m_timeLabel then
            local curTime = LuaController:getWorldTime()
            local expTime = tonumber(self.data[14])
            local endTime = tonumber(self.data[13])
            local lastTime = endTime - curTime
            if expTime>0 then
                local count =(endTime- curTime)/(expTime*3600)
                lastTime=endTime-count*(expTime*3600)-curTime
            else
                lastTime = endTime - curTime
            end
            local timeStr = string.format(LuaController:getSECLang(lastTime))
            self.m_timeLabel:setString(timeStr)
            local isShow = 1
            if ((endTime - curTime) <= 0) then
                isShow = 0
            end
            if (string.format(self.data[15]) == "1") then
                isShow = 0
            end
            if isShow == 0 then
                self.m_timeLabel:setString("")
                self:removeAllEvent()
                if nil ~= self.parentNode then
                    self.parentNode:setVisible(false)
                end
            else
                if nil ~= self.parentNode then
                    self.parentNode:setVisible(true)
                end
            end
            end
            end
        end

         local function eventHandler( eventType )
            if eventType == "enter" then
                --print " GoldExchangeIcon enter"
                scheduleBack()
                self.m_entryId = tonumber(ccbnode:getScheduler():scheduleScriptFunc(scheduleBack, 1, false))
            elseif eventType == "exit" then
                --print "GoldExchangeIcon exit"
                if nil ~= self.m_entryId then
                    ccbnode:getScheduler():unscheduleScriptEntry(self.m_entryId)
                end
            elseif eventType == "cleanup" then
                --print "GoldExchangeIcon cleanup"
                ccbnode:unregisterScriptHandler()
            end
        end
        ccbnode:registerScriptHandler(eventHandler)
        end
    end
end
function GoldExchangeIcon:setData(params)
        --print "GoldExchangeIcon:setData(params)"

	local paramsStr = string.format(params)
    ----print("params:" .. paramsStr)
    self.data = {}
    local index = 1
    local startI = 1
    local fIndex = string.find(paramsStr,",",startI)
    local tmpValue = "" 
    while (true) do
        tmpValue = string.sub(paramsStr,startI,fIndex-1)
        ----print("params" .. string.format(index) .. ":" .. tmpValue)
        self.data[index] = tmpValue
        index = index + 1
        startI = fIndex + 1
        fIndex = string.find(paramsStr,",",startI)
        if (fIndex == nil) then
            tmpValue = string.sub(paramsStr,startI,string.len(paramsStr))
            ----print("params" .. string.format(index) .. ":" .. tmpValue)
            self.data[index] = tmpValue
            break
        end
    end
end
function GoldExchangeIcon:removeAllEvent()
    releaseLuaResource(self.rootPath .. "/resources/".. self.data[19] .."icon")
    if self.m_timeLabel ~= nil then
        self.m_timeLabel:stopAllActions()
    end
end
--[[function GoldExchangeIcon:scheduleBack()
    --print "_____GoldExchangeIcon:scheduleBack"
    if self.data ~= nil then
    	if nil ~= self.m_timeLabel then
            local curTime = LuaController:getWorldTime()
            local expTime = tonumber(self.data[14])
            local endTime = tonumber(self.data[13])
            local lastTime = endTime - curTime
            if expTime>0 then
                local count =(endTime- curTime)/(expTime*3600)
                lastTime=endTime-count*(expTime*3600)-curTime
            else
                lastTime = endTime - curTime
            end
            local timeStr = string.format(LuaController:getSECLang(lastTime))
            self.m_timeLabel:setString(timeStr)
            local isShow = 1
            if ((endTime - curTime) <= 0) then
                isShow = 0
            end
            if (string.format(self.data[15]) == "1") then
                isShow = 0
            end
            if isShow == 0 then
                self.m_timeLabel:setString("")
                self:removeAllEvent()
                if nil ~= self.parentNode then
                    self.parentNode:setVisible(false)
                end
            else
                if nil ~= self.parentNode then
                    self.parentNode:setVisible(true)
                end
            end
        end
    end
end]]

function GoldExchangeIcon:getCCBI()
    local actName = string.format(self.data[19])
    return "ccbi/GoldExchange"..actName.."LuaIcon.ccbi"
end
function GoldExchangeIcon:initSkeleton()
    --print "1"
    if nil==self.m_ani1Layer then
        return
    end
    --print "2"
    local skeletonPath = nil
    --print "3"
    local targetPlatform = cc.Application:getInstance():getTargetPlatform()
    --print "4"
    --if (kTargetAndroid == targetPlatform) then
    if (3 == targetPlatform) then
        -- android
    --print "5"
        targetPlatform = "android/"
    else
    --print "6"
        targetPlatform = "ios/"
    end

    local layerScale = 1
    local layerX = 0
    local layerY = 0
    local altas = ""
    local json = ""
    local aniName = "animation"

    --print "7"
    if self.data[19] == "resource_gift" then
    --print "8"
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "normal_giftditu-zylb")
        if bFile == false then
            return
        end
        ----print("build_alliance_initSkeleton")
        altas = self.rootPath .. "/skeleton/"..targetPlatform .."normal_giftditu-zylb.atlas"
        json = self.rootPath .. "/skeleton/"..targetPlatform .."normal_giftditu-zylb.json"

        layerScale = 0.9
        --layerX = 0
        layerY = -50
    elseif self.data[19] == "oneyear" then
    --print "9"
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "icon_oneyears")
        if bFile == false then
            return
        end

        altas = self.rootPath .. "/skeleton/"..targetPlatform .."icon_oneyears.atlas"
        json = self.rootPath .. "/skeleton/"..targetPlatform .."icon_oneyears.json"

        layerX = -10
        layerY = -55
    elseif self.data[19] == "cn_fight" then
    --print "10"
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "sk_cn_fight")
        if bFile == false then
            return
        end

        altas = self.rootPath .. "/skeleton/"..targetPlatform .."sk_cn_fight.atlas"
        json = self.rootPath .. "/skeleton/"..targetPlatform .."sk_cn_fight.json"

        layerX = -10
        layerY = -55
    elseif self.data[19] == "jp_fight" then
    --print "11"
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "sk_jp_fight")
        if bFile == false then
            return
        end

        altas = self.rootPath .. "/skeleton/"..targetPlatform .."sk_jp_fight.atlas"
        json = self.rootPath .. "/skeleton/"..targetPlatform .."sk_jp_fight.json"

        layerX = -10
        layerY = -55
    elseif self.data[19] == "qixi" then
    --print "12"
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "sk_qixi")
        if bFile == false then
            return
        end

        altas = self.rootPath .. "/skeleton/"..targetPlatform .."sk_qixi.atlas"
        json = self.rootPath .. "/skeleton/"..targetPlatform .."sk_qixi.json"

        layerX = -10
        layerY = -55
    elseif self.data[19] == "peace_gift" then
    --print "13"
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "sk_peace_gift")
        if bFile == false then
            return
        end
        altas = self.rootPath .. "/skeleton/"..targetPlatform .."sk_peace_gift.atlas"
        json = self.rootPath .. "/skeleton/"..targetPlatform .."sk_peace_gift.json"
        --aniName = "animation"
        --layerScale = 1
        layerX = -10
        layerY = -55
    elseif self.data[19] == "zhongqiu_gift" then
    --print "14"
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "sk_zhongqiu_gift")
        if bFile == false then
            return
        end
        altas = self.rootPath .. "/skeleton/"..targetPlatform .."sk_zhongqiu_gift.atlas"
        json = self.rootPath .. "/skeleton/"..targetPlatform .."sk_zhongqiu_gift.json"
        --aniName = "animation"
        layerScale = 0.5
        layerX = -10
        layerY = -55
    elseif self.data[19] == "de_fight" then
    --print "15"
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "sk_de_fight")
        if bFile == false then
            return
        end
        altas = self.rootPath .. "/skeleton/"..targetPlatform .."sk_de_fight.atlas"
        json = self.rootPath .. "/skeleton/"..targetPlatform .."sk_de_fight.json"
        --aniName = "animation"
        layerScale = 0.8
        layerX = -10
        layerY = -55
    elseif self.data[19] == "guoqing_gift" then
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "sk_guoqing_gift")
        if bFile == false then
            return
        end
        altas = self.rootPath .. "/skeleton/"..targetPlatform .."sk_guoqing_gift.atlas"
        json = self.rootPath .. "/skeleton/"..targetPlatform .."sk_guoqing_gift.json"
        --aniName = "animation"
        layerScale = 0.8
        layerX = -10
        layerY = -35
    elseif self.data[19] == "TW_gift" then
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "sk_TW_gift")
        if bFile == false then
            return
        end
        altas = self.rootPath .. "/skeleton/"..targetPlatform .."sk_TW_gift.atlas"
        json = self.rootPath .. "/skeleton/"..targetPlatform .."sk_TW_gift.json"
        --aniName = "animation"
        layerScale = 0.9
        layerX = 0
        layerY = -30
    elseif self.data[19] == "TW_gift1" then
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "sk_TW_gift1")
        if bFile == false then
            return
        end
        altas = self.rootPath .. "/skeleton/"..targetPlatform .."sk_TW_gift1.atlas"
        json = self.rootPath .. "/skeleton/"..targetPlatform .."sk_TW_gift1.json"
        --aniName = "animation"
        layerScale = 0.9
        layerX = 0
        layerY = -30
    elseif self.data[19] == "europe_gift" then
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "sk_europe_gift")
        if bFile == false then
            return
        end
        altas = self.rootPath .. "/skeleton/"..targetPlatform .."sk_europe_gift.atlas"
        json = self.rootPath .. "/skeleton/"..targetPlatform .."sk_europe_gift.json"
        aniName = "rukou"
        layerScale = 0.8
        layerX = -10
        layerY = -40
    elseif self.data[19] == "thousand" then
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "sk_thousand")
        if bFile == false then
            return
        end
        altas = self.rootPath .. "/skeleton/"..targetPlatform .."sk_thousand.atlas"
        json = self.rootPath .. "/skeleton/"..targetPlatform .."sk_thousand.json"
        aniName = "animation"
        layerScale = 1
        layerX = -10
        layerY = -30
    elseif self.data[19] == "google_park" then
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "sk_google_park")
        if bFile == false then
            return
        end
        altas = self.rootPath .. "/skeleton/"..targetPlatform .."sk_google_park.atlas"
        json = self.rootPath .. "/skeleton/"..targetPlatform .."sk_google_park.json"
        aniName = "entrancePark"
        layerScale = 1
        layerX = -10
        layerY = -55
    elseif self.data[19] == "Double_the_Joy" then
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "sk_Double_the_Joy")
        if bFile == false then
            return
        end
        altas = self.rootPath .. "/skeleton/"..targetPlatform .."sk_Double_the_Joy.atlas"
        json = self.rootPath .. "/skeleton/"..targetPlatform .."sk_Double_the_Joy.json"
        aniName = "animation"
        layerScale = 1
        layerX = -5
        layerY = -40
    elseif self.data[19] == "Thanksgiving_Gift" then
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "sk_Thanksgiving_Gift")
        if bFile == false then
            return
        end
        altas = self.rootPath .. "/skeleton/"..targetPlatform .."sk_Thanksgiving_Gift.atlas"
        json = self.rootPath .. "/skeleton/"..targetPlatform .."sk_Thanksgiving_Gift.json"
        aniName = "animation"
        layerScale = 1
        layerX = 4
        layerY = -43
    elseif self.data[19] == "Christmas_gift" then
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "sk_Christmas_gift")
        if bFile == false then
            return
        end
        altas = self.rootPath .. "/skeleton/"..targetPlatform .."sk_Christmas_gift.atlas"
        json = self.rootPath .. "/skeleton/"..targetPlatform .."sk_Christmas_gift.json"
        aniName = "animation"
        layerScale = 1
        layerX = -60
        layerY = -33
    elseif self.data[19] == "March_Acceleration" then
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "sk_March_Acceleration")
        if bFile == false then
            return
        end
        altas = self.rootPath .. "/skeleton/"..targetPlatform .."sk_March_Acceleration.atlas"
        json = self.rootPath .. "/skeleton/"..targetPlatform .."sk_March_Acceleration.json"
        aniName = "animation"
        layerScale = 1
        layerX = -25
        layerY = -33
    elseif self.data[19] == "Christmas_gift_jp" then
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "sk_Christmas_gift_jp")
        if bFile == false then
            return
        end
        altas = self.rootPath .. "/skeleton/"..targetPlatform .."sk_Christmas_gift_jp.atlas"
        json = self.rootPath .. "/skeleton/"..targetPlatform .."sk_Christmas_gift_jp.json"
        aniName = "animation"
        layerScale = 1
        layerX = -25
        layerY = -33
    elseif self.data[19] == "new_year" then
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "sk_new_year")
        if bFile == false then
            return
        end
        altas = self.rootPath .. "/skeleton/"..targetPlatform .."sk_new_year.atlas"
        json = self.rootPath .. "/skeleton/"..targetPlatform .."sk_new_year.json"
        aniName = "animation"
        layerScale = 1
        layerX = -15
        layerY = -33
    elseif self.data[19] == "last_sale" then
        local bFile = LuaController:checkSkeletonFile(targetPlatform .. "sk_last_sale")
        if bFile == false then
            return
        end
        altas = self.rootPath .. "/skeleton/"..targetPlatform .."sk_last_sale.atlas"
        json = self.rootPath .. "/skeleton/"..targetPlatform .."sk_last_sale.json"
        aniName = "animation"
        layerScale = 1
        layerX = -10
        layerY = -33
    end

    if json == "" then
        return
    end
    if altas == "" then
        return
    end

    self.m_ani1Layer:removeAllChildren(true)
    LuaController:addSkeletonAnimation(self.m_ani1Layer,json,altas,aniName,1)
    self.m_ani1Layer:setScale(layerScale)
    self.m_ani1Layer:setPosition(CCPoint(layerX,layerY))
end
function GoldExchangeIcon:initParticale()
    if self.data[19] == "wargift" then
        if nil ~= self.m_ani1Layer then
            local size = self.m_ani1Layer:getContentSize()
            local particles1 = LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesWar_1")
            if nil ~= particles1 then
                self.m_ani1Layer:addChild(particles1)
                particles1:setPosition(cc.p(size.width*0.5,size.height*0.2))
            end
            local particles2 = LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesWar_2")
            if nil ~= particles2 then
                self.m_ani1Layer:addChild(particles2)
                particles2:setPosition(cc.p(size.width*0.5,size.height*0.2))
            end
            local particles3 = LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesWar_3")
            if nil ~=particles3 then
                self.m_ani1Layer:addChild(particles3)
                particles3:setPosition(cc.p(size.width*0.5,size.height*0.2))
            end
            if nil ~= self.m_ani2Layer then
                local particles4 = LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesWar_4")
                if nil ~= particles4 then
                    self.m_ani2Layer:addChild(particles4)
                    particles4:setPosition(cc.p(size.width*0.6,size.height*0.37))
                end
                local particles5 = LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesWar_5")
                if nil ~= particles5 then
                    self.m_ani2Layer:addChild(particles5)
                    particles5:setPosition(cc.p(size.width*0.4,size.height*0.37))
                end
                local particles6 = LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesWar_6")
                if nil ~= particles6 then
                    self.m_ani2Layer:addChild(particles6)
                    particles6:setPosition(cc.p(size.width*0.5,size.height*0.5))
                end
            end
        end
    elseif self.data[19] == "kingdom" then
        if nil ~= self.m_ani2Layer then
            local size = self.m_ani2Layer:getContentSize()
            local particlesa= LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_1")
            if nil ~= particlesa then
                self.m_ani2Layer:addChild(particlesa)
                particlesa:setPosition(cc.p(size.width*0.5,size.height*0.5))
            end
            local particlesb = LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_2")
            if nil ~= particlesb then
                self.m_ani2Layer:addChild(particlesb)
                particlesb:setPosition(cc.p(size.width*0.5,size.height*0.5))
            end  
            local particlesc = LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_3")
            if nil ~= particlesc then
                self.m_ani2Layer:addChild(particlesc)
                particlesc:setPosition(cc.p(size.width*0.5,size.height*0.5))
            end
        end
    elseif self.data[19] == "build_kingdom" then
        if nil ~= self.m_ani2Layer then
            local size = self.m_ani2Layer:getContentSize()
            local particlesa= LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_1")
            if nil ~= particlesa then
                self.m_ani2Layer:addChild(particlesa)
                particlesa:setPosition(cc.p(size.width*0.5,size.height*0.5))
            end
            local particlesb = LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_2")
            if nil ~= particlesb then
                self.m_ani2Layer:addChild(particlesb)
                particlesb:setPosition(cc.p(size.width*0.5,size.height*0.5))
            end  
            local particlesc = LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_3")
            if nil ~= particlesc then
                self.m_ani2Layer:addChild(particlesc)
                particlesc:setPosition(cc.p(size.width*0.5,size.height*0.5))
            end
        end
    elseif self.data[19] == "alliance" then
        if nil ~= self.m_ani1Layer then
            local size = self.m_ani1Layer:getContentSize()
            local particlesa= LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesAlliance_1")
            if nil ~= particlesa then
                self.m_ani1Layer:addChild(particlesa)
                particlesa:setPosition(cc.p(size.width*0.5,size.height*0.5))
            end
            if nil ~= self.m_ani2Layer then
                local particlesb = LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesAlliance_2")
                if nil ~= particlesb then
                    self.m_ani2Layer:addChild(particlesb)
                    particlesb:setPosition(cc.p(size.width*0.38,size.height*0.65))
                end
                local particlesc = LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesAlliance_3")
                if nil ~= particlesc then
                    self.m_ani2Layer:addChild(particlesc)
                    particlesc:setPosition(cc.p(size.width*0.62,size.height*0.65))
                end
            end
        end
    elseif self.data[19] == "alliance1" then
        if nil ~= self.m_ani1Layer then
            local size = self.m_ani1Layer:getContentSize()
            local particlesa= LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesAlliance_1")
            if nil ~= particlesa then
                self.m_ani1Layer:addChild(particlesa)
                particlesa:setPosition(cc.p(size.width*0.5,size.height*0.5))
            end
            if nil ~= self.m_ani2Layer then
                local particlesb = LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesAlliance_2")
                if nil ~= particlesb then
                    self.m_ani2Layer:addChild(particlesb)
                    particlesb:setPosition(cc.p(size.width*0.38,size.height*0.65))
                end
                local particlesc = LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesAlliance_3")
                if nil ~= particlesc then
                    self.m_ani2Layer:addChild(particlesc)
                    particlesc:setPosition(cc.p(size.width*0.62,size.height*0.65))
                end
            end
        end
    elseif self.data[19] == "king" then
        if nil ~= self.m_ani2Layer then
            local size = self.m_ani2Layer:getContentSize()
            local particlesa= LuaController:createParticleForLua(self.rootPath .. "/particles/Rose_3")
            if nil ~= particlesa then
                self.m_ani2Layer:addChild(particlesa)
                particlesa:setPosition(cc.p(size.width*0.5,size.height*0.5))
            end
            local particlesb = LuaController:createParticleForLua(self.rootPath .. "/particles/VIPGlow_3")
            if nil ~= particlesb then
                self.m_ani2Layer:addChild(particlesb)
                particlesb:setPosition(cc.p(size.width*0.5,size.height*0.5))
            end
        end
    elseif self.data[19] == "war_resource" then
        if nil ~= self.m_ani2Layer then
            local size = self.m_ani2Layer:getContentSize()
            local particlesa= LuaController:createParticleForLua(self.rootPath .. "/particles/Rose_3")
            if nil ~= particlesa then
                self.m_ani2Layer:addChild(particlesa)
                particlesa:setPosition(cc.p(size.width*0.5,size.height*0.5))
            end
            local particlesb = LuaController:createParticleForLua(self.rootPath .. "/particles/VIPGlow_3")
            if nil ~= particlesb then
                self.m_ani2Layer:addChild(particlesb)
                particlesb:setPosition(cc.p(size.width*0.5,size.height*0.5))
            end
        end
    elseif self.data[19] == "spring" then
        if nil ~= self.m_ani1Layer then
            local size = self.m_ani1Layer:getContentSize()
            local particlesa= LuaController:createParticleForLua(self.rootPath .. "/particles/Rose_3")
            if nil ~= particlesa then
                self.m_ani1Layer:addChild(particlesa)
                particlesa:setPosition(cc.p(size.width*0.5,size.height*0.5))
            end
        end
    elseif self.data[19] == "build_alliance" then
        if nil ~= self.m_ani2Layer then
            local size = self.m_ani2Layer:getContentSize()
            local particlesb = LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_2")
            if nil ~= particlesb then
                self.m_ani2Layer:addChild(particlesb)
                particlesb:setPosition(cc.p(size.width*0.5,size.height*0.5))
            end  
            local particlesc = LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_3")
            if nil ~= particlesc then
                self.m_ani2Layer:addChild(particlesc)
                particlesc:setPosition(cc.p(size.width*0.5,size.height*0.5))
            end
        end
    elseif self.data[19] == "equip_build" then
        if nil ~= self.m_ani2Layer then
            local size = self.m_ani2Layer:getContentSize()
            local particlesa= LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesEquipBuild_1")
            if nil ~= particlesa then
                self.m_ani2Layer:addChild(particlesa)
                particlesa:setPosition(cc.p(size.width*0.5,0))
            end
            local particlesb= LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesEquipBuild_2")
            if nil ~= particlesb then
                self.m_ani2Layer:addChild(particlesb)
                particlesb:setPosition(cc.p(size.width*0.5,0))
            end
            local particlesc= LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesEquipBuild_3")
            if nil ~= particlesc then
                self.m_ani2Layer:addChild(particlesc)
                particlesc:setPosition(cc.p(size.width*0.5,0))
            end
        end
    elseif self.data[19] == "trap_build" then
        if nil ~= self.m_ani2Layer then
            local size = self.m_ani2Layer:getContentSize()
            local particlesa= LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesEquipBuild_1")
            if nil ~= particlesa then
                self.m_ani2Layer:addChild(particlesa)
                particlesa:setPosition(cc.p(size.width*0.5,size.height*0.5))
            end
            local particlesb= LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesEquipBuild_2")
            if nil ~= particlesb then
                self.m_ani2Layer:addChild(particlesb)
                particlesb:setPosition(cc.p(size.width*0.5,size.height*0.5))
            end
            local particlesc= LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesEquipBuild_3")
            if nil ~= particlesc then
                self.m_ani2Layer:addChild(particlesc)
                particlesc:setPosition(cc.p(size.width*0.5,size.height*0.5))
            end
        end
    elseif self.data[19] == "kingdom_war" then
        if nil ~= self.m_ani2Layer then
            local size = self.m_ani2Layer:getContentSize()
            local particlesa= LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesEquipBuild_1")
            if nil ~= particlesa then
                self.m_ani2Layer:addChild(particlesa)
                particlesa:setPosition(cc.p(size.width*0.5,0))
            end
            local particlesb= LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesEquipBuild_2")
            if nil ~= particlesb then
                self.m_ani2Layer:addChild(particlesb)
                particlesb:setPosition(cc.p(size.width*0.5,0))
            end
            local particlesc= LuaController:createParticleForLua(self.rootPath .. "/particles/ActivitiesEquipBuild_3")
            if nil ~= particlesc then
                self.m_ani2Layer:addChild(particlesc)
                particlesc:setPosition(cc.p(size.width*0.5,0))
            end
        end
    elseif self.data[19] == "ru_gift" then
        if nil ~= self.m_ani2Layer then
            local size = self.m_ani2Layer:getContentSize()
            local particlesa= LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_1")
            if nil ~= particlesa then
                self.m_ani2Layer:addChild(particlesa)
                particlesa:setPosition(cc.p(size.width*0.5,size.height*0.5))
            end
            local particlesb = LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_2")
            if nil ~= particlesb then
                self.m_ani2Layer:addChild(particlesb)
                particlesb:setPosition(cc.p(size.width*0.5,size.height*0.5))
            end  
            local particlesc = LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_3")
            if nil ~= particlesc then
                self.m_ani2Layer:addChild(particlesc)
                particlesc:setPosition(cc.p(size.width*0.5,size.height*0.5))
            end
        end
    elseif self.data[19] == "first" then
        if nil ~= self.m_ani2Layer then
            local size = self.m_ani2Layer:getContentSize()
            local particlesa= LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_1")
            if nil ~= particlesa then
                self.m_ani2Layer:addChild(particlesa)
                particlesa:setPosition(cc.p(size.width*0.5,0))
            end
            local particlesb = LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_2")
            if nil ~= particlesb then
                self.m_ani2Layer:addChild(particlesb)
                particlesb:setPosition(cc.p(size.width*0.5,size.height*0.5))
            end  
            local particlesc = LuaController:createParticleForLua(self.rootPath .. "/particles/KingdomIcon_3")
            if nil ~= particlesc then
                self.m_ani2Layer:addChild(particlesc)
                particlesc:setPosition(cc.p(size.width*0.5,size.height*0.5))
            end
        end
    elseif self.data[19] == "Double_the_Joy" then
        if nil ~= self.m_ani2Layer then
            local size = self.m_ani2Layer:getContentSize()
            for i=1,4 do
                local particle1= LuaController:createParticleForLua(self.rootPath .. "/particles/UiFire_"..string.format(i))
                if nil ~= particle1 then
                    self.m_ani2Layer:addChild(particle1)
                    particle1:setPosition(CCPoint(size.width*0.23,size.height*0.4))
                end
            end
        end
    end
end