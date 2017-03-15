require "externCOK2"
require "CCBReaderLoadCOK2"
require "commonCOK2"

GoldExchangeLuaCell  = GoldExchangeLuaCell or {}
ccb["GoldExchangeLuaCell"] = GoldExchangeLuaCell

GoldExchangeCell = class("GoldExchangeCell",
	function()
        return cc.Layer:create() 
	end
)
GoldExchangeCell.__index = GoldExchangeCell
function GoldExchangeCell:create(path,params)
	local node = GoldExchangeCell.new()
	node:init(path,params)
	return node
end
function GoldExchangeCell:init(path,params)
    self.rootPath = path
    self.data = params
    local  proxy = cc.CCBProxy:create()
    local ccbiUrl = self.rootPath .. "/ccbi/GoldExchangeLuaCell.ccbi"
	local  node  = CCBReaderLoad(ccbiUrl,proxy,GoldExchangeLuaCell)
    if(nil == node) then
        return
    end
   	--print "GoldExchangeCell:init"
    local  layer = tolua.cast(node,"cc.Layer")
    if nil ~= GoldExchangeLuaCell["m_nameLabel"] then
        self.m_nameLabel = tolua.cast(GoldExchangeLuaCell["m_nameLabel"],"cc.Label")
        if nil ~= self.m_nameLabel then
            self.m_nameLabel:setString("")
        end
    end
    --print "1"
    if nil ~= GoldExchangeLuaCell["m_numLabel"] then
        self.m_numLabel = tolua.cast(GoldExchangeLuaCell["m_numLabel"],"cc.Label")
        if nil ~= self.m_numLabel then
            local numStr = string.format(self.data[2])
            self.m_numLabel:setString(numStr)
        end
    end
    --print "2"
    if nil ~= GoldExchangeLuaCell["m_iconNode"] then
    	self.m_iconNode = tolua.cast(GoldExchangeLuaCell["m_iconNode"],"cc.LayerColor")
        if nil ~= self.m_iconNode then
            LuaController:addItemIcon(self.m_iconNode,self.data[1],self.m_nameLabel)
        end
    end
    self:addChild(node)
end