function __G__TRACKBACK__(msg)
    --print("----------------------------------------")
    --print("LUA ERROR: " .. tostring(msg) .. "\n")
    --print(debug.traceback())
    --print("----------------------------------------")
end

collectgarbage("setpause", 100) 
collectgarbage("setstepmul", 5000)

require ("commonCOK2")
function releaseWorldResource()
	-- body
	if WORLDCITY == nil then
		return
	end
	
	local targetPlatform = cc.Application:getInstance():getTargetPlatform()
	for k,v in pairs(WORLDCITY) do
		print(k,v)
		local len = table.getn(v)
		if len >= 3 then
		    if (3 == targetPlatform) then
		        releaseLuaResourceForAndroid(v[1] .. v[2] , v[3])
		    else
		        releaseLuaResource(v[1] .. v[2] .. v[3])
		    end
		end
	end
	WORLDCITY = nil
end
function createTileView(cinfo,path)
	-- body
	-- if CLASSTEST == nil then
	-- 	require("world.ClassTest")
	-- 	CLASSTEST = ClassTest:create()
	-- 	print("init CLASSTEST")
	-- end
	if cinfo ~= nil then
		local cType = cinfo:getProperty("cityType")
		print("cityinfo_type [ " .. tostring(cType) .. " ]")
	end
	local luaType = cinfo:getProperty("luaType")
	if luaType == 39 then  -- 复活
		local oT = cinfo:getProperty("openTime")
        local sT = cinfo:getProperty("startTime")
        local eT = cinfo:getProperty("endTime")
        local nT = GlobalData:shared():getTimeStamp()
        print("nt [ " .. tostring(nT) .. " ] ot [ " .. tostring(oT) .. " ] st [ " .. tostring(sT) .. " ] eT [ " .. tostring(eT) .. " ]")
        if oT < nT then
        	if isLibExist(path,"revive.ReviveTile") == true then
				local node = ReviveTile:create(path,cinfo)	
        		if node ~= nil then
					return node
				end
        	end
        end
	end
	print("NewBaseTileLuaInfo:create")
	local node1 = NewBaseTileLuaInfo:create()
	return node1
end

function addWorldUnderNode(x,y,idx,cinfo,arr,path)
	-- body
	local worldmap = WorldMapView:instance()
	if worldmap == nil then
		return 
	end
	if cinfo ~= nil then
		local cType = cinfo:getProperty("cityType")
		print("cityinfo_type [ " .. tostring(cType) .. " ]")
		
		local luaType = cinfo:getProperty("luaType")
		if luaType == 39 then  -- 复活
			print("cityinfo_type revive")
			local oT = cinfo:getProperty("openTime")
			local sT = cinfo:getProperty("startTime")
			local eT = cinfo:getProperty("endTime")
			local nT = GlobalData:shared():getTimeStamp()
			print("nt [ " .. tostring(nT) .. " ] ot [ " .. tostring(oT) .. " ] st [ " .. tostring(sT) .. " ] eT [ " .. tostring(eT) .. " ]")
			if oT > nT then
				return
			end
			if isLibExist(path,"revive.Revive") == true then
				createReviveCity(x,y,idx,cinfo,arr,path)
			end
		end

	end
end