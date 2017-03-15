require "externCOK2"
require "CCBReaderLoadCOK2"
require "commonCOK2"

GoldExchangeLuaCell1  = GoldExchangeLuaCell1 or {}
ccb["GoldExchangeLuaCell1"] = GoldExchangeLuaCell1

GoldExchangeCell1 = class("GoldExchangeCell1",
    function()
        return cc.Layer:create() 
    end
)
GoldExchangeCell1.__index = GoldExchangeCell1
function GoldExchangeCell1:create(path,params)
    local node = GoldExchangeCell1.new()
    node:init(path,params)
    return node
end
function GoldExchangeCell1:init(path,params)
    self.rootPath = path
    self.data = params
    local  proxy = cc.CCBProxy:create()
    local ccbiUrl = self.rootPath .. "/ccbi/GoldExchangeLuaCell1.ccbi"
    local  node  = CCBReaderLoad(ccbiUrl,proxy,GoldExchangeLuaCell1)
    if(nil == node) then
        return
    end
    
    local  layer = tolua.cast(node,"cc.Layer")
    if nil ~= GoldExchangeLuaCell1["m_nameLabel"] then
        self.m_nameLabel = tolua.cast(GoldExchangeLuaCell1["m_nameLabel"],"cc.LabelTTF")
        if nil ~= self.m_nameLabel then
            --self.m_nameLabel:setString("")
        end
    end
    --print "GoldExchangeCell1:init___1"

    if nil ~= GoldExchangeLuaCell1["m_numLabel"] then
        --print "GoldExchangeCell1:init___1_1"
        self.m_numLabel = tolua.cast(GoldExchangeLuaCell1["m_numLabel"],"cc.LabelTTF")
        --print "GoldExchangeCell1:init___1_2"
        if nil ~= self.m_numLabel then
            --print "GoldExchangeCell1:init___1_3"
            local numStr = string.format(self.data[2])
            --print "GoldExchangeCell1:init___1_4"
            self.m_numLabel:setString(numStr)
        end
    end
    --print "GoldExchangeCell1:init___2"
    if nil ~= GoldExchangeLuaCell1["m_iconNode"] then
        self.m_iconNode = tolua.cast(GoldExchangeLuaCell1["m_iconNode"],"cc.LayerColor")
        if nil ~= self.m_iconNode then
            LuaController:addIconByType(self.m_iconNode,self.data[1],self.m_nameLabel,1,45)
        end
    end
    --print "GoldExchangeCell1:init___3"
    self:addChild(node)
end