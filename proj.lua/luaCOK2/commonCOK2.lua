function loadLuaResource(path)
	--print "function loadLuaResource(path)"
	if (nil ~=path) then
		cc.SpriteFrameCache:getInstance():addSpriteFrames(path)
    end
end
function releaseLuaResource(path)
	--print "function releaseLuaResource(path)"
	
	if(nil ~= path) then
		local plistPath = path .. ".plist"
		print ("release plist: " .. plistPath)
		cc.SpriteFrameCache:getInstance():removeSpriteFramesFromFile(plistPath)
		local texturePath = path .. ".pvr.ccz"
		local texture  = cc.Director:getInstance():getTextureCache():getTextureForKey(texturePath)
        if(nil ~= texture) then
        	print ("release texture" .. texturePath)
            cc.Director:getInstance():getTextureCache():removeTextureForKey(texturePath)
        end
	end
end
function releaseLuaResourceForAndroid(path,res)
	--print "function releaseLuaResource(path)"
	
	if(nil ~= path) then
		local plistPath = path .. res .. ".plist"
		print ("release plist: " .. plistPath)
		cc.SpriteFrameCache:getInstance():removeSpriteFramesFromFile(plistPath)
		local texturePath = path .. "_alpha_" .. res .. "_alpha.pkm"
		local texture  = cc.Director:getInstance():getTextureCache():getTextureForKey(texturePath)
        if(nil ~= texture) then
        	print ("release texture" .. texturePath)
            cc.Director:getInstance():getTextureCache():removeTextureForKey(texturePath)
        end
        local texturePath1 = path .. "_alpha_" .. res .. ".pkm"
		local texture1  = cc.Director:getInstance():getTextureCache():getTextureForKey(texturePath1)
        if(nil ~= texture1) then
        	print ("release texture" .. texturePath1)
            cc.Director:getInstance():getTextureCache():removeTextureForKey(texturePath1)
        end
	end
end
function loadCommonResource(index,isLoad)
	--print "function loadCommonResource(index,isLoad)"
	LuaController:doResourceByCommonIndex(index,isLoad)
end
function isLibExist(path,libname)
	-- body
	if package.loaded[libname] ~= nil then
		print ("isLibExist___1")
		return true
	else
		print ("isLibExist___2")
		local path1 = ""
		local vvv = string.split(libname, ".")
		for i,v in ipairs(vvv) do
			if string.len(v) > 0 then
				path1 = path1 .. "/" .. v
			end
		end
		if path1 ~= "" then
			path1 = path .. path1 .. ".lua" 
			if cc.FileUtils:getInstance():isFileExist(path1) == true then 
				require (libname)
				return true
			end
		end
	end
	return false
end