require "externCOK2"
require "commonCOK2"

KnightTitleInfo = class("KnightTitleInfo")
KnightTitleInfo.__index = KnightTitleInfo

function KnightTitleInfo:create(params)
    local ct = KnightTitleInfo.new()
    ct:parasDict(params)
    return ct
end

-- KnightTitleInfo.parasDict = function ( params )
--     -- body
-- end

function KnightTitleInfo:parasDict(params)
    -- body

    print("KnightTitleInfo:     params ：" .. tostring(params))

    self.itemId = params["id"]
    self.name = params["name"]
    self.des = params["description"]
    self.icon = params["icon"]
    self.bd_level = params["building_level"]
    self.color = params["color"]
    self.power = params["power"]
    self.unlock = 0

    print("KnightTitleInfo:print" .. tostring(self.itemId))

    self.showDias = {};
    self.values = {};
    self.parasMap = {};
    self.mateUseMap = {};--激活所需材料
    self.mateUnlockMap = {};--解锁所需材料

    if nil ~= params["activate_array"] then
        local vec = string.split(params["activate_array"],"|")
        for k,v in pairs(vec) do
            print(k,v)
            local tmpVec = string.split(v,";")
            local vecCnt = table.getn(tmpVec)
            if (vecCnt >= 2) then
                self.mateUseMap[ tmpVec[1] ] = tmpVec[2]
                print(tmpVec[1],tmpVec[2])
                print("*******************")
            end
        end
    end

    print("*******************  mate end *************")

    if nil ~= params["unlock_array"] then
        local vec = string.split(params["unlock_array"],"|")
        for k,v in pairs(vec) do
            print(k,v)
            local tmpVec = string.split(v,";")
            local vecCnt = table.getn(tmpVec)
            if (vecCnt >= 2) then
                self.mateUnlockMap[ tmpVec[1] ] = tmpVec[2]
                print(tmpVec[1],tmpVec[2])
                print("*******************")
            end
        end
    end

    for i=1,20 do
        local key = "show"..i
        print(tostring(key))
        if nil ~= params[key] then
            self.showDias[i] = params[key]
        else
            break;
        end
    end

    for i=1,20 do
        local key = "num"..i
        print(tostring(key))
        if nil ~= params[key] then
            self.values[i] = params[key]
        else
            break;
        end
    end

    for i=1,20 do
        local key = "para"..i
        print(tostring(key))
        if nil ~= params[key] then
            self.parasMap[i] = string.split(params[key],";")
        else
            break;
        end
    end
end

function KnightTitleInfo:getEffFormatByOrd(order)
    if nil ~= self.parasMap[order] then
        local cnt = table.getn(self.parasMap[order])
        if cnt>0 then
            local eNum = self.parasMap[order][1]
            return CCCommonUtilsForLua:getEffFormat(eNum)
        end
    end
    return "%"
end

function KnightTitleInfo:getEffPMByOrd(order)
    if (nil ~= self.parasMap[order]) then
        local cnt = table.getn(self.parasMap[order])
        if (cnt>0) then
            local eNum = self.parasMap[order][1]
            return CCCommonUtilsForLua:getEffPM(eNum)
        end
    end
    return "+"
end