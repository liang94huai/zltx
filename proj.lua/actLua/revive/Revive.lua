require "externCOK2"
require "commonCOK2"

--墓地复活数据解析
function praseCemetery(dict)
	-- body
	local ref = tolua.cast(dict,"cc.Ref")
	local params = LuaController:getInstance():getDicLuaData(ref)
	if CEMETERY_INFO == nil then
		CEMETERY_INFO={}
	end
    if params["kingdomCemetery"] ~= nil then
    	print("praseCemetery___kingdomCemetery___1")
        if CEMETERY_INFO["KINGDOM"] == nil then
    		CEMETERY_INFO["KINGDOM"] = {}
    	end
    	CEMETERY_INFO["KINGDOM"] = params["kingdomCemetery"]
        print("praseCemetery___kingdomCemetery___11")

        if CEMETERY_INFO["KINGDOM"] ~= nil then
	    	if CEMETERY_INFO["KINGDOM"]["ministerblessingtime"] ~= nil then
	    		local v1 = tonumber(CEMETERY_INFO["KINGDOM"]["ministerblessingtime"])
	    		if v1 > 0 then
	    			CEMETERY_INFO["KINGDOM"]["ministerblessingtime"] = v1 * 0.001
	    		end
	    	end
	    	if CEMETERY_INFO["KINGDOM"]["kingblessingtime"] ~= nil then
	    		local v1 = tonumber(CEMETERY_INFO["KINGDOM"]["kingblessingtime"])
	    		if v1 > 0 then
	    			CEMETERY_INFO["KINGDOM"]["kingblessingtime"] = v1 * 0.001
	    		end
	    	end
	    	if CEMETERY_INFO["KINGDOM"]["ministerblessinterval"] ~= nil then
	    		local v1 = tonumber(CEMETERY_INFO["KINGDOM"]["ministerblessinterval"])
	    		if v1 > 0 then
	    			CEMETERY_INFO["KINGDOM"]["ministerblessinterval"] = v1 * 60
	    		end
	    	end
	    	if CEMETERY_INFO["KINGDOM"]["kingblessinterval"] ~= nil then
	    		local v1 = tonumber(CEMETERY_INFO["KINGDOM"]["kingblessinterval"])
	    		if v1 > 0 then
	    			CEMETERY_INFO["KINGDOM"]["kingblessinterval"] = v1 * 60
	    		end
	    	end
	    end
    end
    if params["userCemetery"] ~= nil then
    	print("praseCemetery___userCemetery___1")
    	if CEMETERY_INFO["PLAYER"] == nil then
	    	CEMETERY_INFO["PLAYER"] = {}
	    end
	    CEMETERY_INFO["PLAYER"] = params["userCemetery"]
    	print("praseCemetery___userCemetery___7")
    end
    if params["activity"] ~= nil then
    	print("praseCemetery___activity___1")
    	if CEMETERY_INFO["ACT"] == nil then
	    	CEMETERY_INFO["ACT"] = {}
	    end
	    CEMETERY_INFO["ACT"] = params["activity"]
    	print("praseCemetery___activity___5")
    end
    if params["countDown"] ~= nil then
    	print("praseCemetery___countDown___1")
    	if CEMETERY_INFO["CD"] == nil then
	    	CEMETERY_INFO["CD"] = {}
	    end
        CEMETERY_INFO["CD"] = params["countDown"]
    	print("praseCemetery___countDown___3")

    	if CEMETERY_INFO["CD"] ~= nil then
	    	if CEMETERY_INFO["CD"]["cemeteryOpenCD"] ~= nil then
	    		local v1 = tonumber(CEMETERY_INFO["CD"]["cemeteryOpenCD"])
		        if v1 > 0 then
		            CEMETERY_INFO["CD"]["cemeteryOpenCD"] = v1 * 0.001
		        end 
	       	end
	       	if CEMETERY_INFO["CD"]["deadReviveCD"] ~= nil then
	       		local v1 = tonumber(CEMETERY_INFO["CD"]["deadReviveCD"])
		        if v1 > 0 then
		            CEMETERY_INFO["CD"]["deadReviveCD"] = v1 * 0.001
		        end
	       	end
	    end
    end
end
function backCemeteryCMD(cmdname,params)
	-- body
	if cmdname == "cemetery.get.info" then
    	praseCemetery(params)
    elseif cmdname == "cemetery.bless" then
    	praseCemetery(params)
    elseif cmdname == "cemetery.player.contribution" then
    	praseCemetery(params)
    elseif cmdname == "cemetery.player.click.revive" then
    	praseCemetery(params)
    end
end
function isInRevive()
	if CEMETERY_INFO == nil then
		print("isInRevive___no data1")
		return 0 -- no data
	end
    if CEMETERY_INFO["KINGDOM"] == nil then
    	print("isInRevive___no data2")
    	return 0 -- no data
    end
    if CEMETERY_INFO["ACT"] == nil then
    	print("isInRevive___no data3")
    	return 0 -- no data
    end

    local endT = tonumber(CEMETERY_INFO["ACT"]["et"])
    local nT = GlobalData:shared():getTimeStamp()
    if endT <= nT then
    	print("isInRevive___nt [ " .. nT .. " ]" .. " eT [ " .. endT .. " ]")
    	return 0 -- no data
    end

	local cemeterystat = tonumber(CEMETERY_INFO["KINGDOM"]["cemeterystat"])
	local revivestat = tonumber(CEMETERY_INFO["KINGDOM"]["revivestat"])
	if cemeterystat == 0 then  -- [等待国王开启复活活动]
		if revivestat == 0 then
			print("isInRevive___state[ to be open ]")
			return 1
		end
	else -- 墓地活动开启
    	local pnv = 0
    	if nil ~= CEMETERY_INFO["PLAYER"] then
            pnv = tonumber(CEMETERY_INFO["PLAYER"]["needcontributionvalue"])
        end
        if revivestat ~= 0 then -- [捐赠/等待国王复活王国兵/王国兵复活]
            if pnv > 0 then  -- 自己有死兵
            	print("isInRevive___state[ open and has dead ]")
            	return 2 
            else     --  自己无死兵
            	print("isInRevive___state[ open and no dead ]")
            	return 2   
            end
        end
    end
    print("isInRevive___no data4")
    return 0
end
function addBatchItemWithObj(obj,pnode,x,y,type,idx,cinfo,arr,path)
	-- body
	if cinfo ~= nil then
		local cType = cinfo:getProperty("cityType")
		print("cityinfo_type [ " .. tostring(cType) .. " ]")
	end
	if nil ~= pnode then
		local item = CCLoadSprite:createSprite("protect.png")
		if item ~= nil then
			pnode:addChild(item)
			item:setPosition(x,y)
			arr:addObject(item)
		end
	end
end

function addBatchItem(pnode,x,y,type,idx,cinfo,arr,path)
	-- body
	if cinfo ~= nil then
		local cType = cinfo:getProperty("cityType")
		print("cityinfo_type [ " .. tostring(cType) .. " ]")
	end
	if nil ~= pnode then
		local item = CCLoadSprite:createSprite("protect.png")
		if item ~= nil then
			pnode:addChild(item)
			item:setPosition(x,y)
			arr:addObject(item)
		end
	end
end


function getWorldSpriteImage(cinfo)
	-- body
	local resourceVal = cinfo:getProperty("resource")
	if resourceVal ~= nil then
		local rType = resourceVal["type"]
		if rType == 0 then
			return "0017.png"
		elseif rType == 1 then
		    return "0018.png"
		elseif rType == 2 then
		    return "0019.png"
		elseif rType == 3 then
		    return "0020.png"
		elseif rType == 4 then
			return "0041.png"
		elseif rType == 5 then
		    local iconName = CCCommonUtilsForLua:getPropById("100101", "resource")
		    if iconName == "" then
		    	return "0021.png"
		    else
		    	return (iconName .. ".png") 
		    end
		end
	end
end
function createReviveCity(x,y,idx,cinfo,arr,path)
	-- body
	-- local sprUrl = getWorldSpriteImage(cinfo)
	print("createReviveCity____1")
	-- add city sprite
	-- if cinfo:getProperty("parentCityIndex") == -1 then
	-- 	cinfo.parentCityIndex = 727206
	-- end
	if cinfo:getProperty("parentCityIndex") ~= cinfo:getProperty("cityIndex") then
		return
	end
	
	loadLuaResource(path .. "/revive/resources/revive_world.plist")
	print("createReviveCity____2")
	local under = cc.Node:create()
	print("createReviveCity____3")
	under:setAnchorPoint(cc.p(0,0))
	print("createReviveCity____4")
	under:setTag(idx)
	print("createReviveCity____5")
	under:setPosition(x,y)
	print("createReviveCity____6")
	local worldmap = WorldMapView:instance()
	if worldmap == nil then
		return 
	end
	local node = worldmap:getCityBatchNode()
	if node ~= nil then
		arr:addObject(under)
		node:addChild(under,idx)
	end
	print("createReviveCity____7")
	
	-- create city

	local spr2 = CCLoadSprite:createSprite("revive3.png")
	under:addChild(spr2)
	spr2:setPosition(0,64)
	local spr1 = CCLoadSprite:createSprite("revive2.png")
	print("createReviveCity____8")
	under:addChild(spr1)
	spr1:setPosition(15,206)
	local spr0 = CCLoadSprite:createSprite("revive1.png")
	print("createReviveCity____8")
	under:addChild(spr0)
	spr0:setPosition(15,310)
	under:setScale(0.8)
	print("createReviveCity____9")
	if WORLDCITY == nil then
		WORLDCITY={}
	end
	if WORLDCITY["revive"] == nil then
		LuaController:getInstance():sendCMD("cemetery.get.info")
		WORLDCITY["revive"]={}
		WORLDCITY["revive"][1] = path
		WORLDCITY["revive"][2] = "/revive/resources/"
		WORLDCITY["revive"][3] = "revive_world"
	end
	print("createReviveCity____10")

	-- create partical
	local isInView = false
	if CEMETERY_INFO ~= nil then
        if CEMETERY_INFO["KINGDOM"] ~= nil then
		    local cemeterystat = tonumber(CEMETERY_INFO["KINGDOM"]["cemeterystat"])
		    if cemeterystat == 1 then
		        isInView = true
		        local userHasDead = false
		        if nil ~= CEMETERY_INFO["PLAYER"] then
		            local nv = tonumber(CEMETERY_INFO["PLAYER"]["needcontributionvalue"])
		            if nv > 0 then
		                userHasDead = true
		            end
		        end
		        local revivestat = tonumber(CEMETERY_INFO["KINGDOM"]["revivestat"])
		        if revivestat == 0 then
		            if userHasDead == false then
		                isInView = false
		            end
		        elseif revivestat == 3 then
		            if userHasDead == false then
		                isInView = false
		            end
		        end
		    end
		end
    end
    if isInView == true then
    	local parPath = path .. "/revive/particles/"
    	local particle1 = LuaController:createParticleInPoolForLua(parPath .. "Towerflare")
		if particle1 ~= nil then
			particle1:setPosition(x+15,y+200)
			local tag = worldmap:getBatchTagForLua(200,cinfo:getProperty("cityIndex"))
			worldmap:addParticleToBatch(particle1,tag)
			arr:addObject(particle1)
		end
		local particle2 = LuaController:createParticleInPoolForLua(parPath .. "Spring_z")
		if particle2 ~= nil then
			particle2:setPosition(x-10,y+180)
			local tag = worldmap:getBatchTagForLua(200,cinfo:getProperty("cityIndex"))
			worldmap:addParticleToBatch(particle2,tag)
			arr:addObject(particle2)
		end
		local particle3 = LuaController:createParticleInPoolForLua(parPath .. "Spring_y")
		if particle3 ~= nil then
			particle3:setPosition(x+35,y+180)
			local tag = worldmap:getBatchTagForLua(200,cinfo:getProperty("cityIndex"))
			worldmap:addParticleToBatch(particle3,tag)
			arr:addObject(particle3)
		end
		local particle4 = LuaController:createParticleInPoolForLua(parPath .. "Waterflare_1")
		if particle4 ~= nil then
			particle4:setPosition(x-20,y+120)
			local tag = worldmap:getBatchTagForLua(200,cinfo:getProperty("cityIndex"))
			worldmap:addParticleToBatch(particle4,tag)
			arr:addObject(particle4)
		end
		local particle5 = LuaController:createParticleInPoolForLua(parPath .. "Waterflare_4")
		if particle5 ~= nil then
			particle5:setPosition(x+16,y+120)
			local tag = worldmap:getBatchTagForLua(200,cinfo:getProperty("cityIndex"))
			worldmap:addParticleToBatch(particle5,tag)
			arr:addObject(particle5)
		end
		local particle6 = LuaController:createParticleInPoolForLua(parPath .. "Waterflare_1")
		if particle6 ~= nil then
			particle6:setPosition(x+40,y+120)
			local tag = worldmap:getBatchTagForLua(200,cinfo:getProperty("cityIndex"))
			worldmap:addParticleToBatch(particle6,tag)
			arr:addObject(particle6)
		end
		local particle7 = LuaController:createParticleInPoolForLua(parPath .. "Waterflare_3")
		if particle7 ~= nil then
			particle7:setPosition(x+25,y+30)
			local tag = worldmap:getBatchTagForLua(200,cinfo:getProperty("cityIndex"))
			worldmap:addParticleToBatch(particle7,tag)
			arr:addObject(particle7)
		end
	end

	-- local under = CCLoadSprite:createSprite(sprUrl)
	-- if under ~= nil then
	-- 	under:setAnchorPoint(cc.p(0,0))
	-- 	under:setTag(idx)
	-- 	under:setPosition(x-128,y-64)
	-- 	print("addWorldUnderNode___init_sprite_1")
	-- 	local node = worldmap:getCityBatchNode()
	-- 	if node ~= nil then
	-- 		print("addWorldUnderNode___init_sprite_2")
	-- 		arr:addObject(under)
	-- 		node:addChild(under)
	-- 		print("addWorldUnderNode___init_sprite_3")
	-- 	end
	-- end
	-- -- add level tag
	-- local resTileKey = WorldController:getInstance():getProperty("resTileKey")
	-- print("addWorldUnderNode___init_leveltag_1")
	-- local bgStr = "building_level_overlay.png"
	-- local kl = 0
	-- local kh = 0
	-- print("addWorldUnderNode___init_leveltag_2")
	-- for i,v in ipairs(resTileKey) do
	-- 	for k,v1 in pairs(v) do
	-- 		print(i,k,v1)
	-- 	end
	-- end
	-- print("addWorldUnderNode___init_leveltag_3")
	-- if cType == 0 then
	-- 	kl = resTileKey[1]["k1"]
	-- 	kh = resTileKey[2]["k2"]
	-- elseif cType == 1 then
	-- 	kl = resTileKey[5]["k5"]
	-- 	kh = resTileKey[6]["k6"]
	-- elseif cType == 2 then
	-- 	kl = resTileKey[3]["k3"]
	-- 	kh = resTileKey[4]["k4"]
	-- elseif cType == 3 then
	-- 	kl = resTileKey[1]["k1"]
	-- 	kh = resTileKey[2]["k2"]
	-- elseif cType == 5 then
	-- 	kl = resTileKey[7]["k7"]
	-- 	kh = resTileKey[8]["k8"]
	-- end
	-- print("addWorldUnderNode___init_leveltag_4")
	-- local resourceVal = cinfo:getProperty("resource")
	-- print("addWorldUnderNode___init_leveltag_5 [ " .. tostring(resourceVal) .. " ]")
	-- if resourceVal ~= nil then
	-- 	local sum = resourceVal["sum"]
	-- 	if sum >= kh then
	-- 		bgStr = "resbd_level_01.png"
	-- 	elseif sum <= kl then
	-- 		bgStr = "building_level_overlay.png"
	-- 	else
	-- 		bgStr = "building_level_overlay.png"
	-- 	end
	-- end
	-- print("addWorldUnderNode___init_leveltag_6 [ " .. bgStr .. " ]")
	-- local item = CCLoadSprite:createSprite(bgStr)
	-- item:setScale(0.35*0.8)
	-- item:setPosition(x+30,y-25)
	-- print("addWorldUnderNode___init_leveltag_7")
	-- local zorder = 1
	-- local tagIdx = idx
	-- local itemTag = worldmap:getBatchTag(1, tagIdx)
	-- item:setTag(itemTag)
	-- print("addWorldUnderNode___init_leveltag_8")
	-- local bnode = worldmap:getBatchNode()
	-- if bnode ~= nil then
	-- 	print("addWorldUnderNode___init_leveltag_9")
	-- 	arr:addObject(item)
	-- 	bnode:addChild(item,zorder)
	-- end
	-- print("addWorldUnderNode___init_leveltag_10")
	-- -- label
	-- local lblNode = worldmap:getLabelNode()
	-- print("addWorldUnderNode___init_label_1")
	-- if lblNode~=nil then
	-- 	local lv = CCLabelBatch:create(tostring(resourceVal["lv"]),lblNode)
	-- 	print("addWorldUnderNode___init_label_2")
	-- 	arr:addObject(lv)
	-- 	local lbn = tolua.cast(lv,"cc.Node")
	-- 	print("addWorldUnderNode___init_label_3")
	-- 	if lbn ~= nil then
	-- 		lbn:setScale(0.6*0.8)
	-- 		print("addWorldUnderNode___init_label_4")
	-- 		lbn:setSkewY(22.5)
	-- 		lbn:setPosition(30+x,y-25)
	-- 		print("addWorldUnderNode___init_label_5")
	-- 	end
	-- end
end