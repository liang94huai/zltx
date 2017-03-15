require "externCOK2"
require "commonCOK2"
require ("knight.KnightTitleInfo")

KnightTitleController = class("KnightTitleController")
KnightTitleController.__index = KnightTitleController

local _instance = nil

function KnightTitleController:getInstance()
	if nil == _instance then
		_instance = KnightTitleController:create()
	end
	return _instance
end

function KnightTitleController:create()
	local ret = KnightTitleController.new()
	ret:init()
	return ret
end

function KnightTitleController:purgeData()
	_instance = nil
end

function KnightTitleController:init()
	print("init knight xml start")
    self.KnightTitleInfoMap = {}
    self.m_curKnightTitleId = -1
    self.KnightEffMap = {}
    self.initState = 1

	local dict = LocalController:shared():DBXMLManager():getGroupByKey("knight_title");
	local ref = tolua.cast(dict,"cc.Ref")
	local groupDict = LuaController:getInstance():getDicLuaData(ref)

	print("init knight xml start1")

    if nil ~= groupDict then
    	for key, value in pairs(groupDict) do 
    		print("key::::::"..tostring(key))
    		self.KnightTitleInfoMap[ key ] = KnightTitleInfo:create( value )
    	end
    end

    print("init knight xml end")
end

function KnightTitleController:isInit()
	return self.initState
end

-- 后台返回初始化数据
function KnightTitleController:retInitData(dict)
    local ref = tolua.cast(dict,"cc.Ref")
    local params = LuaController:getInstance():getDicLuaData(ref)
    
    for i,v in ipairs(self.KnightTitleInfoMap) do
        v.unlock = 0
    end
    self.m_curKnightTitleId = -1;

    if params["unlock_title"] ~= nil then
        local unlockTable = string.split(params["unlock_title"],",")
        for k,v in pairs(unlockTable) do
            print("KnightTitleController date  id :::  "..tostring(v))
            self.KnightTitleInfoMap[v].unlock = 1
        end
    end
    
    if params["activity"] ~= nil then
        print("KnightTitleController 1  id :::  "..tostring(params["activity"]))
        self.m_curKnightTitleId = params["activity"]
    end
    self:MakeEffectValue()
    self.initState = 2
end

function KnightTitleController:MakeEffectValue()
    self.KnightEffMap = {}
    if nil ~= self.KnightTitleInfoMap[self.m_curKnightTitleId] then
        local kInfo = self.KnightTitleInfoMap[self.m_curKnightTitleId]
        for i=1,20 do
            if nil ~= kInfo.values[i] and nil ~= kInfo.parasMap[i] then
                local cnt = table.getn(kInfo.parasMap[i])
                for j=1,cnt do
                    local eNum = kInfo.parasMap[i][j]
                    if nil == self.KnightEffMap[ eNum ] then
                        self.KnightEffMap[ eNum ] = 0
                    end
                    self.KnightEffMap[ eNum ] = kInfo.values[i]+self.KnightEffMap[ eNum ]
                end
            end
        end

        GlobalData:shared():getPlayerInfo().knight_title = kInfo.power
        -- UIComponent::getInstance()->setUserData(); 改为发消息更新
    end
end

-- 变更称号
function KnightTitleController:startPutOnKnightTitle(itemId)
    local ccdict = CCDictionary:create()
    ccdict:setObject(CCString:create(itemId),"knightId")
    LuaController:getInstance():sendCMD("knight.puton.command",ccdict)
    return true
end

function KnightTitleController:retPutOnKnightTitle(params)
    local ref = tolua.cast(params,"cc.Ref")
    local dict = LuaController:getInstance():getDicLuaData(ref)
    if nil ~= dict["errorCode"] then
        local errorCode = dict["errorCode"]
        CCCommonUtilsForLua:flyText(LuaController:getLang(errorCode))
    elseif nil ~= dict["CurKnightTitle"] then
        print("put on knight title  id :::  "..tostring(dict["CurKnightTitle"]))
        self.m_curKnightTitleId = dict["CurKnightTitle"]
        self:MakeEffectValue()
    end
end

-- 解锁称号
function KnightTitleController:startUnlockKnightTitle(itemId)
    local ccdict = CCDictionary:create()
    ccdict:setObject(CCString:create(itemId),"knightId")
    LuaController:getInstance():sendCMD("knight.unlock.command",ccdict)
    return true
end

function KnightTitleController:retUnlockKnightTitle(params)
    local ref = tolua.cast(params,"cc.Ref")
    local dict = LuaController:getInstance():getDicLuaData(ref)
    if nil ~= dict["errorCode"] then
        local errorCode = dict["errorCode"]
        CCCommonUtilsForLua:flyText(LuaController:getLang(errorCode))
    else
        print("unlock knight knight_title ")
    end
end

function KnightTitleController:UpdateUnlockKnight(params)
    local buildLv = 0;
    local ref = tolua.cast(params,"cc.Ref")
    local dict = LuaController:getInstance():getDicLuaData(ref)
    if nil ~= dict["level"] then
        buildLv = dict["level"]
    end

    if buildLv == 0 then
        return
    end

    for i,v in ipairs(self.KnightTitleInfoMap) do
        if v.unlock == 0 then
            if table.getn(v.mateUnlockMap)==0 and v.bd_level<=buildLv then
                v.unlock = 1
            end
        end
    end

end

-- 合成徽章
function KnightTitleController:startComposeBedge(itemId)
    local ccdict = CCDictionary:create()
    ccdict:setObject(CCString:create(itemId),"knight")
    LuaController:getInstance():sendCMD("material.create",ccdict)
    return true
end
function KnightTitleController:retComposeBedge(dict)
    
end

-- 分解徽章
function KnightTitleController:startDecBedge(itemId)
    local ccdict = CCDictionary:create()
    ccdict:setObject(CCString:create(itemId),"itemId")
    LuaController:getInstance():sendCMD("material.decompse",ccdict)
    return true
end

function KnightTitleController:retDecBedge(dict)
    
end

-- 获取已经使用的徽章数量
function KnightTitleController:GetUseBedgeById(itemId)
    if self.m_curKnightTitleId > 0 then
        if nil ~= self.KnightTitleInfoMap[m_curKnightTitleId].mateUseMap[itemId] then
            return self.KnightTitleInfoMap[m_curKnightTitleId].mateUseMap[itemId]
        end
    else
        return 0
    end
end