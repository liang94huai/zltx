function __G__TRACKBACK__(msg)
    --print("----------------------------------------")
    --print("LUA ERROR: " .. tostring(msg) .. "\n")
    --print(debug.traceback())
    --print("----------------------------------------")
end

collectgarbage("setpause", 100) 
collectgarbage("setstepmul", 5000)
require ("cocos.init")
require ("commonCOK2")

--init config
function initConfig(params,path)
	-- body
	print("initConfig")

	-- DATACONFIG = {}
	-- if params ~= nil then
	-- 	if params:objectForKey("wb_timing") ~= nil then
	-- 		local k1 = params:objectForKey("wb_timing"):valueForKey("k1"):intValue()
	-- 		local k2 = params:objectForKey("wb_timing"):valueForKey("k2"):intValue()
	-- 		DATACONFIG["wb_timing"] = {}
	-- 		DATACONFIG["wb_timing"]["k1"] = k1
	-- 		DATACONFIG["wb_timing"]["k2"] = k2
	-- 		print("DATACONFIG " .. tostring(k1) .." , " .. tostring(k2))
	-- 	end
	-- end
end
--activity test code
function showActPopup(path,actname)
	if actname == "57034" then
		if isLibExist(path,"57034.ActivityView") == true then
			local newLayer = ActivityView:create(path,actname)
			if nil~=newLayer then
				PopupViewController:getInstance():addPopupInView(newLayer)
			end
		end
	elseif actname == "knight_title" then
		if isLibExist(path,"knight.KnightTitleView") == true then
			local newLayer = KnightTitleView:create(path)
			if nil~=newLayer then
				PopupViewController:getInstance():addPopupInView(newLayer)
			end
		end
	end
end
function backCMD(params,cmdname,path)
	-- local json = require "dkjsonCOK2"
	-- local data = json.decode(params)
	print ("backCMD [" .. cmdname .. "]")

	-- test code
	-- send cmd
	-- local ccdict = CCDictionary:create()
 --    ccdict:setObject(CCString:create("stringkey1"),"a")
 --    local arr = CCArray:create()
 --    arr:addObject(CCString:create("stringkey2"))
 --    arr:addObject(CCString:create("stringkey3"))
 --    ccdict:setObject(arr,"c")
 --    LuaController:getInstance():sendCMD("login.init",ccdict)

 	--back cmd
	-- -- if cmdname == "login.init" then
	-- if cmdname == "init" then
 --        if params ~= nil then
 --            local obj = params:objectForKey("lottery")
 --            if obj ~= nil then
 --                local normal = params:objectForKey("lottery"):valueForKey("normal"):getCString()
 --                local super = params:objectForKey("lottery"):valueForKey("super"):getCString()
 --                print("normal ---[ " .. normal .. " ] , super --- [ " .. super .." ]")
 --            end
 --        end
 --    elseif cmdname == "push.user.update" then
 --    	if params ~= nil then
 --    		local exp = params:valueForKey("exp"):intValue()
 --    		local level = params:valueForKey("level"):intValue()
 --    		local maxExp = params:valueForKey("maxExp"):intValue()
 --    		print("exp: " .. tostring(exp) .. " level: " .. tostring(level) .. "maxExp: " .. tostring(maxExp))
 --    	end
 --    elseif cmdname == "push.task.complete" then
 --        --do
 --        local arr = params:objectForKey("task")
 --        if arr ~= nil then
 --        	local cnt = arr:count()
	--         for i=1,cnt do
	--         	local v1 = arr:objectAtIndex(i-1)
	--         	if v1 ~= nil then
	--         		local taskid = v1:valueForKey("id"):getCString()
	--         		local state = v1:valueForKey("state"):intValue()
	--         		print("taskid: " .. tostring(taskid) .. " state: " .. tostring(state))
	--         	end
	--         end
	--     end
 --    end
    
    if cmdname == "init_knight" then
    	if isLibExist(path,"knight.KnightTitleController") == true then
    		KnightTitleController:getInstance():retInitData(params)
    	end
	elseif cmdname == "knight_puton" then
		if isLibExist(path,"knight.KnightTitleController") == true then
    		KnightTitleController:getInstance():retPutOnKnightTitle(params)
    	end
	elseif cmdname == "knight_unlock" then
		if isLibExist(path,"knight.KnightTitleController") == true then
    		KnightTitleController:getInstance():retUnlockKnightTitle(params)
    	end
	elseif cmdname == "knight_update" then
		if isLibExist(path,"knight.KnightTitleController") == true then
			KnightTitleController:getInstance():UpdateUnlockKnight(params)
		end
    else
    	if isLibExist(path,"revive.Revive") == true then
    		backCemeteryCMD(cmdname,params)
    	end
    end

	local popup = PopupViewController:getInstance():getCurrentPopupView()
	if popup == nil then
		-- print ("cur popup is nil")
		return
	end
	if(popup:getIsLua() == true) then
		-- print ("CMDCallbackForLua")
		popup:CMDCallbackForLua(cmdname,params)
	end
end
function backNotification(cmdname,path)
	local popup = PopupViewController:getInstance():getCurrentPopupView()
	if popup == nil then
		-- print ("cur popup is nil")
		return
	end

	if(popup:getIsLua() == true) then
		-- print ("NotificationCallbackForLua")
		popup:NotificationCallbackForLua(cmdname)
	end
end
function backNotificationWithParams(cmdname,path,params)
	local popup = PopupViewController:getInstance():getCurrentPopupView()
	if popup == nil then
		-- print ("cur popup is nil")
		return
	end
	if(popup:getIsLua() == true) then
		-- print ("NotificationCallbackForLuaWithParams")
		popup:NotificationCallbackForLuaWithParams(cmdname,params)
	end
end
function onImperialEnter( )
	-- print "function onImperialEnter"
	-- local layer = LuaController:getInstance():getImperialTouchLayer()
	-- layer = tolua.cast(layer, "cc.Node")
	-- if nil == layer then
	-- 	return 
	-- end
	-- require("/57034/test.lua")
	-- local testSpr = TestSpr:create()
	-- if nil == testSpr then
	-- 	return
	-- end
	-- testSpr:setLocalZOrder(2045)
	-- layer:addChild(testSpr)
	-- testSpr:setPosition(cc.p(2040, 700))
end

function purgeActData()

end

function checkLoginDataState(path,name)
	if name == "init_knight" then
		if cc.FileUtils:getInstance():isFileExist(path .. "/knight/KnightTitleController.lua") then
    		require("knight.KnightTitleController")
			return KnightTitleController:getInstance():isInit()
		end
	elseif name == "" then
	end
	return 0
end
