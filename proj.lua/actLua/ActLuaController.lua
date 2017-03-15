require "externCOK2"
require "commonCOK2"

ActLuaController = class("ActLuaController")
ActLuaController.__index = ActLuaController

local _instance = nil;

function ActLuaController:getInstance()
	if nil == _instance then
		_instance = ActLuaController:create()
	end
	return _instance
end

function ActLuaController:create()
	local ret = ActLuaController.new()
	ret:init()
	return ret
end

function ActLuaController:purgeData()
	_instance = nil
end

function ActLuaController:init()
	self.data = {}
end