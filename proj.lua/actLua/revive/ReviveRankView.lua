require "externCOK2"
require "commonCOK2"


local DATALIST = {}
local ROOTPATH = ""
local RANKTYPE = 1
--
-- class ReviveRankCell
-- 

ReviveRankLuaCell  = ReviveRankLuaCell or {}
ccb["ReviveRankLuaCell"] = ReviveRankLuaCell

ReviveRankCell = class("ReviveRankCell",
    function()
        return cc.Layer:create() 
    end
)
ReviveRankCell.__index = ReviveRankCell
function ReviveRankCell:create(idx)
    local node = ReviveRankCell.new()
    if node:initView(idx) == true then
        return node
    end
    return nil
end
function ReviveRankCell:initView(idx)
    print("ReviveRankCell:initView")
    if self:init() == true then
        print("ReviveRankCell:initView1")
        loadLuaResource(ROOTPATH .. "/resources/revive.plist")
        local  proxy = cc.CCBProxy:create()
        local ccbiURL = ROOTPATH .. "/ccbi/ReviveRankLuaCell.ccbi"
        local node = CCBReaderLoad(ccbiURL,proxy,ReviveRankLuaCell)
        if node ~= nil then
            print("ReviveRankCell:initView2")
            if nil ~= ReviveRankLuaCell["m_hintBGNode"] then
                self.m_hintBGNode = tolua.cast(ReviveRankLuaCell["m_hintBGNode"],"cc.Node")
            end
            if nil == self.m_hintBGNode then
                return false
            end
            if nil ~= ReviveRankLuaCell["m_sprBG1"] then
                self.m_sprBG1 = tolua.cast(ReviveRankLuaCell["m_sprBG1"],"ccui.Scale9Sprite")
            end
            if nil == self.m_sprBG1 then
                return false
            end
            if nil ~= ReviveRankLuaCell["m_sprBG2"] then
                self.m_sprBG2 = tolua.cast(ReviveRankLuaCell["m_sprBG2"],"ccui.Scale9Sprite")
            end
            if nil == self.m_sprBG2 then
                return false
            end
            if nil ~= ReviveRankLuaCell["m_sprBG3"] then
                self.m_sprBG3 = tolua.cast(ReviveRankLuaCell["m_sprBG3"],"ccui.Scale9Sprite")
            end
            if nil == self.m_sprBG3 then
                return false
            end
            if nil ~= ReviveRankLuaCell["m_sprBG4"] then
                self.m_sprBG4 = tolua.cast(ReviveRankLuaCell["m_sprBG4"],"ccui.Scale9Sprite")
            end
            if nil == self.m_sprBG4 then
                return false
            end
            if nil ~= ReviveRankLuaCell["m_headNode"] then
                self.m_headNode = tolua.cast(ReviveRankLuaCell["m_headNode"],"cc.Node")
            end
            if nil == self.m_headNode then
                return false
            end
            if nil ~= ReviveRankLuaCell["m_otherheadSpr"] then
                self.m_otherheadSpr = tolua.cast(ReviveRankLuaCell["m_otherheadSpr"],"cc.Sprite")
            end
            if nil == self.m_otherheadSpr then
                return false
            end
            if nil ~= ReviveRankLuaCell["m_selfheadSpr"] then
                self.m_selfheadSpr = tolua.cast(ReviveRankLuaCell["m_selfheadSpr"],"cc.Sprite")
            end
            if nil == self.m_selfheadSpr then
                return false
            end
            if nil ~= ReviveRankLuaCell["m_text1"] then
                self.m_text1 = tolua.cast(ReviveRankLuaCell["m_text1"],"cc.Label")
            end
            if nil == self.m_text1 then
                return false
            end
            if nil ~= ReviveRankLuaCell["m_text2"] then
                self.m_text2 = tolua.cast(ReviveRankLuaCell["m_text2"],"cc.Label")
            end
            if nil == self.m_text2 then
                return false
            end
            if nil ~= ReviveRankLuaCell["m_text3"] then
                self.m_text3 = tolua.cast(ReviveRankLuaCell["m_text3"],"cc.Label")
            end
            if nil == self.m_text3 then
                return false
            end
            if nil ~= ReviveRankLuaCell["m_numspr1"] then
                self.m_numspr1 = tolua.cast(ReviveRankLuaCell["m_numspr1"],"cc.Sprite")
            end
            if nil == self.m_numspr1 then
                return false
            end
            if nil ~= ReviveRankLuaCell["m_numspr2"] then
                self.m_numspr2 = tolua.cast(ReviveRankLuaCell["m_numspr2"],"cc.Sprite")
            end
            if nil == self.m_numspr2 then
                return false
            end
            if nil ~= ReviveRankLuaCell["m_numspr3"] then
                self.m_numspr3 = tolua.cast(ReviveRankLuaCell["m_numspr3"],"cc.Sprite")
            end
            if nil == self.m_numspr3 then
                return false
            end
            if nil ~= ReviveRankLuaCell["m_numText"] then
                self.m_numText = tolua.cast(ReviveRankLuaCell["m_numText"],"cc.Label")
            end
            if nil == self.m_numText then
                return false
            end
            if nil ~= ReviveRankLuaCell["m_Starsprite"] then
                self.m_Starsprite = tolua.cast(ReviveRankLuaCell["m_Starsprite"],"cc.Sprite")
            end
            if nil == self.m_Starsprite then
                return false
            end
            
            self.m_headImgNode = HFHeadImgNode:create()

            print("ReviveRankCell:initView3")
            self:setData(idx)
            print("ReviveRankCell:initView4")
            local function onNodeEvent(event)
                if event == "enter" then
                    print "ReviveRankCell__enter"
                elseif event == "exit" then
                    print "ReviveRankCell__exit"
                    self:onExit()
                end
            end
            self:addChild(node)
            self:setContentSize(node:getContentSize())
            self.ccbNode = node
            self.ccbNode:registerScriptHandler(onNodeEvent)
            print("ReviveRankCell:initView5")
        end
    end
    return true
end

function ReviveRankCell:setData(idx)
    print("ReviveRankCell:setData1")
    self.index = idx
    self.params = DATALIST[idx]
    self.m_headNode:removeAllChildren()
    print("ReviveRankCell:setData2")
    local pic = nil
    if self.params["pic"] == "" then
        print("ReviveRankCell:setData3")
        pic = CCLoadSprite:createSprite("guidePlayerIcon.png")
        pic:setScale(0.7)
    else
        print("ReviveRankCell:setData4")
        local mpic = self.params["pic"] .. ".png"
        pic = CCLoadSprite:createSprite(mpic,true,7)
    end
    print("ReviveRankCell:setData5")
    CCCommonUtilsForLua:setSpriteMaxSize(pic, 60)
    self.m_headNode:addChild(pic)
    print("ReviveRankCell:setData6")
    if CCCommonUtilsForLua:isUseCustomPic(self.params["picVer"]) == true then
        print("ReviveRankCell:setData7")
        self.m_headImgNode:initHeadImgUrl2(self.m_headNode, CCCommonUtilsForLua:getCustomPicUrl(self.params["uid"], self.params["picVer"]), 1.0, 60, true)
    end
    print("ReviveRankCell:setData8")
    self.m_sprBG1:setVisible(false)
    print("ReviveRankCell:setData8_____1")
    self.m_sprBG2:setVisible(false)
    print("ReviveRankCell:setData8_____2")
    self.m_sprBG3:setVisible(false)
    print("ReviveRankCell:setData8_____3")
    self.m_numText:setVisible(false)
    print("ReviveRankCell:setData8_____4")
    self.m_numspr1:setVisible(false)
    print("ReviveRankCell:setData8_____5")
    self.m_numspr2:setVisible(false)
    print("ReviveRankCell:setData8_____6")
    self.m_numspr3:setVisible(false)
    print("ReviveRankCell:setData9")
    if idx==1 then
        print("ReviveRankCell:setData10")
        self.m_sprBG1:setVisible(true)
        self.m_numspr1:setVisible(true)
    elseif idx==2 then
        print("ReviveRankCell:setData11")
        self.m_sprBG2:setVisible(true)
        self.m_numspr2:setVisible(true)
    elseif idx==3 then
        print("ReviveRankCell:setData12")
        self.m_sprBG3:setVisible(true)
        self.m_numspr3:setVisible(true)
    else
        print("ReviveRankCell:setData13")
        self.m_numText:setVisible(true)
        self.m_numText:setString(tostring(idx))
    end
    print("ReviveRankCell:setData14")
    local myUID = GlobalData:shared():getPlayerInfo():getProperty("uid")
    print("ReviveRankCell:setData15")
    if self.params["uid"]==myUID then
        self.m_selfheadSpr:setVisible(true)
        self.m_otherheadSpr:setVisible(false)
        self.m_sprBG4:setVisible(true)
        self.m_sprBG1:setVisible(false)
        self.m_sprBG2:setVisible(false)
        self.m_sprBG3:setVisible(false)
        print("ReviveRankCell:setData16")
    else
        self.m_selfheadSpr:setVisible(false)
        self.m_otherheadSpr:setVisible(true)
        self.m_sprBG4:setVisible(false)
        print("ReviveRankCell:setData17")
    end
    print("ReviveRankCell:setData18")
    local allname = ""
    local name = ""
    if self.params["alliancename"] ~= nil then
        if self.params["alliancename"] == "" then
            allname = ""
            name = self.params["name"]
        else
            allname = LuaController:getLang1("108596",self.params["alliancename"])
            name = "(" .. self.params["abbr"] .. ")" .. self.params["name"]
        end
    else
        allname = ""
        name = self.params["name"]
    end
    print("ReviveRankCell:setData19")
    self.m_text2:setString(allname)
    if RANKTYPE == 1 then
        self.m_text3:setString(LuaController:getCMDLang(self.params["deadSum"]))
    elseif RANKTYPE == 2 then
        self.m_text3:setString(LuaController:getCMDLang(self.params["useItemSum"]))
    end
    print("ReviveRankCell:setData20")
    self.m_text1:setString(name)
    print("ReviveRankCell:setData21")
end
function ReviveRankCell:onExit()
    if nil ~= self.ccbNode then
        self.ccbNode:unregisterScriptHandler()
    end
end
function ReviveRankCell:getUID()
    if self.params ~= nil then
        return self.params["uid"]
    end
    return ""
end
function ReviveRankCell:getUName()
    if self.params ~= nil then
        return self.params["name"]
    end
    return ""
end




--
--  class ReviveRankView
--

ReviveRankLuaView  = ReviveRankLuaView or {}
ccb["ReviveRankLuaView"] = ReviveRankLuaView

ReviveRankView = class("ReviveRankView",
    function()
        return PopupBaseView:create() 
    end
)
ReviveRankView.__index = ReviveRankView
function ReviveRankView:create(path,params)
    local view = ReviveRankView.new()
    if view:initView(path,params) == true then
        return view
    end
    return nil
end
function ReviveRankView:initView(path,params)
    print("ReviveRankView:initView____1")
	self.path = path
    RANKTYPE = params

    if self:init(true,0) == false then
        return false
    end
    print("ReviveRankView:initView____2")
    
    self:setHDPanelFlag(true)
    local strPath = string.format(path)
    ROOTPATH = strPath .. "/revive"
    loadLuaResource(ROOTPATH .. "/resources/revive.plist")
    
    local isPad = CCCommonUtilsForLua:isIosAndroidPad()
    print("ReviveRankView:initView____3")
    local  proxy = cc.CCBProxy:create()

    print("ReviveRankView:initView____4")
    local ccbiURL = ""
    if isPad == true then
        ccbiURL = ROOTPATH .. "/ccbi/ReviveRankLuaViewPad.ccbi"
    else
        ccbiURL = ROOTPATH .. "/ccbi/ReviveRankLuaView.ccbi"
    end

    local nodeccb = CCBReaderLoad(ccbiURL,proxy,ReviveRankLuaView)
    if nodeccb == nil then
        return false
    end
    print("ReviveRankView:initView____5")
    if nil ~= ReviveRankLuaView["m_bgNode"] then
        self.m_bgNode = tolua.cast(ReviveRankLuaView["m_bgNode"],"cc.Node")
    end
    if nil == self.m_bgNode then
        return false
    end
    print("ReviveRankView:initView____6")
    if nil ~= ReviveRankLuaView["m_titleNode"] then
        self.m_titleNode = tolua.cast(ReviveRankLuaView["m_titleNode"],"cc.Node")
    end
    if nil == self.m_titleNode then
        return false
    end
    if nil ~= ReviveRankLuaView["m_listNode"] then
        self.m_listNode = tolua.cast(ReviveRankLuaView["m_listNode"],"cc.LayerColor")
    end
    if nil == self.m_listNode then
        return false
    end
    if nil ~= ReviveRankLuaView["m_timeNode"] then
        self.m_timeNode = tolua.cast(ReviveRankLuaView["m_timeNode"],"cc.Node")
    end
    if nil == self.m_timeNode then
        return false
    end
    if nil ~= ReviveRankLuaView["m_textTitle1"] then
        self.m_textTitle1 = tolua.cast(ReviveRankLuaView["m_textTitle1"],"cc.Label")
    end
    if nil == self.m_textTitle1 then
        return false
    end
    if nil ~= ReviveRankLuaView["m_textTitle2"] then
        self.m_textTitle2 = tolua.cast(ReviveRankLuaView["m_textTitle2"],"cc.Label")
    end
    if nil == self.m_textTitle2 then
        return false
    end
    if nil ~= ReviveRankLuaView["m_timeTitleText"] then
        self.m_timeTitleText = tolua.cast(ReviveRankLuaView["m_timeTitleText"],"cc.Label")
    end
    if nil == self.m_timeTitleText then
        return false
    end
    if nil ~= ReviveRankLuaView["m_timeLeftText"] then
        self.m_timeLeftText = tolua.cast(ReviveRankLuaView["m_timeLeftText"],"cc.Label")
    end
    if nil == self.m_timeLeftText then
        return false
    end
    if nil ~= ReviveRankLuaView["m_lblTip"] then
        self.m_lblTip = tolua.cast(ReviveRankLuaView["m_lblTip"],"cc.Label")
    end
    if nil == self.m_lblTip then
        return false
    end

    print("ReviveRankView:initView____7")
    self:setContentSize(nodeccb:getContentSize())
    local screenSize = cc.Director:getInstance():getWinSize()
    local changeH = 0
    local halfW = 0
    if isPad == true then
        changeH = screenSize.height - 2048
        halfW = 320
    else
        changeH = screenSize.height - 852
        halfW = screenSize.width * 0.5
    end
    local wCnt = 2
    local bgPerH = 160
    local hCnt = math.ceil(screenSize.height / bgPerH) + 1
    local startY = 0 -- changeH 
    print("total cnt [ " .. tostring(hCnt) .. " ]")
    for j=1,hCnt do
        print(j)
        local spr1 = CCLoadSprite:createSprite("fuhuo_bg-1.png")
        spr1:setAnchorPoint(cc.p(0,0.5))
        self.m_bgNode:addChild(spr1)
        spr1:setPosition(0,startY)
        
        local spr2 = CCLoadSprite:createSprite("fuhuo_bg-1.png")
        spr2:setAnchorPoint(cc.p(0,0.5))
        self.m_bgNode:addChild(spr2)
        spr2:setPosition(0,startY)
        spr1:setScaleX(-1)
        startY = startY - bgPerH
    end
    print("ReviveRankView:initView____8")
    startY = 0
    local offsetX = 25
    local spr3 = CCLoadSprite:createSprite("fuhuo_zhangshi1.png")
    spr3:setAnchorPoint(cc.p(0,0))
    self.m_bgNode:addChild(spr3)
    spr3:setPosition(offsetX - halfW,startY)
    local spr4 = CCLoadSprite:createSprite("fuhuo_zhangshi1.png")
    spr4:setAnchorPoint(cc.p(0,0))
    spr4:setScaleX(-1)
    self.m_bgNode:addChild(spr4)
    spr4:setPosition(halfW - offsetX,startY)
    print("ReviveRankView:initView____9")

    local spr5 = CCLoadSprite:createSprite("fuhuo_zhangshi3.png")
    spr5:setAnchorPoint(cc.p(0,1))
    local spr3H = spr3:getContentSize().height
    local scale5H = (screenSize.height - spr3H) / spr5:getContentSize().height
    spr5:setScaleY(scale5H)
    self.m_bgNode:addChild(spr5)
    spr5:setPosition(offsetX - halfW ,startY)
    local spr6 = CCLoadSprite:createSprite("fuhuo_zhangshi3.png")
    spr6:setAnchorPoint(cc.p(0,1))
    spr6:setScaleX(-1)
    spr6:setScaleY(scale5H)
    self.m_bgNode:addChild(spr6)
    spr6:setPosition(halfW - offsetX,startY)
    print("ReviveRankView:initView____10")

    local spr7 = CCLoadSprite:createSprite("Allance_frame02.png")
    local spr7W = spr7:getContentSize().width
    local spr7H = spr7:getContentSize().height * 0.5
    local scale7W = screenSize.height / spr7W
    print("scale: [ " .. scale7W .. " ]")
    spr7:setAnchorPoint(cc.p(0.5,0.5))
    spr7:setScaleX(scale7W)
    spr7:setRotation(-90)
    self.m_bgNode:addChild(spr7)
    spr7:setPosition(spr7H - halfW,80-(screenSize.height*0.5))
    local spr8 = CCLoadSprite:createSprite("Allance_frame02.png")
    spr8:setAnchorPoint(cc.p(0.5,0.5))
    spr8:setScaleX(scale7W)
    spr8:setRotation(90)
    self.m_bgNode:addChild(spr8)
    spr8:setPosition(halfW - spr7H,80-(screenSize.height*0.5))
    print("ReviveRankView:initView____11")

    
    startY = 50 
    local sCnt = math.ceil(screenSize.height / 150 )- 1
    print("total scnt [ " .. tostring(sCnt) .. " ]")
    if sCnt > 0 then
        for i=1,sCnt do
            print(i)
            local spr9 = CCLoadSprite:createSprite("fuhuo_Jlzy_di_1.png")
            spr9:setScaleX(-1)
            self.m_bgNode:addChild(spr9)
            spr9:setPosition(spr7H - halfW + 3,startY)
            local spr10 = CCLoadSprite:createSprite("fuhuo_Jlzy_di_1.png")
            self.m_bgNode:addChild(spr10)
            spr10:setPosition(halfW - spr7H - 3,startY)
            startY = startY - 150
        end
    end
    self.m_listNode:setPositionY(self.m_listNode:getPositionY() - changeH)
    local listSize = self.m_listNode:getContentSize()
    self.m_listNode:setContentSize(cc.size(listSize.width,listSize.height+changeH))
    listSize = self.m_listNode:getContentSize()

    print("ReviveRankView:initView____12")
    
    self:addChild(nodeccb)
    self.ccbNode = nodeccb
        
    local function onNodeEvent(event)
        if event == "enter" then
            if RANKTYPE == 1 then
                self:setTitleName(LuaController:getLang("145015"))
            else
                self:setTitleName(LuaController:getLang("145014"))
            end
            -- print "ReviveRankView__enter"
        elseif event == "exit" then
            -- print "ReviveRankView__exit"
            self:onExit()
        end
    end
    self.ccbNode:registerScriptHandler(onNodeEvent)
    print("ReviveRankView:initView____13")
    
    self.m_timeTitleText:setString(LuaController:getLang("145023"))
    self.m_textTitle1:setString(LuaController:getLang("115825"))
    self.m_textTitle2:setString(LuaController:getLang("150302"))
    -- init tableView
    self.m_tabView = cc.TableView:create(listSize)
    print("ReviveRankView:initView____14")
    self.m_tabView:setDirection(cc.SCROLLVIEW_DIRECTION_VERTICAL)
    print("ReviveRankView:initView____15")
    self.m_tabView:setVerticalFillOrder(cc.TABLEVIEW_FILL_TOPDOWN)
    print("ReviveRankView:initView____16")
    self.m_tabView:setDelegate()
    print("ReviveRankView:initView____17")
    self.m_listNode:addChild(self.m_tabView)
    print("ReviveRankView:initView____18")
    self.m_tabView:registerScriptHandler(ReviveRankView.scrollViewDidScroll,cc.SCROLLVIEW_SCRIPT_SCROLL)
    self.m_tabView:registerScriptHandler(ReviveRankView.tableCellTouched,cc.TABLECELL_TOUCHED)
    self.m_tabView:registerScriptHandler(ReviveRankView.cellSizeForTable,cc.TABLECELL_SIZE_FOR_INDEX)
    self.m_tabView:registerScriptHandler(ReviveRankView.tableCellAtIndex,cc.TABLECELL_SIZE_AT_INDEX)
    self.m_tabView:registerScriptHandler(ReviveRankView.numberOfCellsInTableView,cc.NUMBER_OF_CELLS_IN_TABLEVIEW)
    print("ReviveRankView:initView____20")

    if RANKTYPE == 1 then
        local ccdict = CCDictionary:create()
        ccdict:setObject(CCString:create("1"),"type")
        LuaController:getInstance():sendCMD("cemetery.get.ranking",ccdict) 
    else
        local ccdict = CCDictionary:create()
        ccdict:setObject(CCString:create("0"),"type")
        LuaController:getInstance():sendCMD("cemetery.get.ranking",ccdict)
    end

    return true
end
function ReviveRankView.scrollViewDidScroll(view)
end

function ReviveRankView.tableCellTouched(tab,cell)
    -- print("ActivityExcView__cell touched at index: " .. cell:getIdx())
    if cell ~= nil then 
        local node = cell:getChildByTag(100)
        if node ~= nil then
            local uid = node:getUID()
            local name = node:getUName()
            if name ~= "" then
                if uid ~= "" then
                    local pop = PlayerInfoBtnPopUpView:create()
                    pop:InitCommonBtns(name, uid)
                    PopupViewController:getInstance():addPopupView(pop, false)
                end
            end
        end
    end
end

function ReviveRankView.cellSizeForTable(tab,idx) 
    return 600, 110
end

function ReviveRankView.tableCellAtIndex(tab, idx)
    print("ReviveRankView.tableCellAtIndex [" .. idx .."]")
    if DATALIST == nil then
        print("ReviveRankView.tableCellAtIndex data nil ")
        return nil
    end
    if idx >= table.getn(DATALIST) then
        print("ReviveRankView.tableCellAtIndex idx larger ")
        return nil
    end
    local cell = tab:dequeueCell()
    if cell ~= nil then
        local node = cell:getChildByTag(100)
        if node ~= nil then
            node:setData(id+1)
        end
    else
        local node = ReviveRankCell:create(idx+1)
        if node ~= nil then
            node:setTag(100)
            cell = cc.TableViewCell:new()
            cell:addChild(node)
        end
    end
    return cell
end

function ReviveRankView.numberOfCellsInTableView(tab)
    if DATALIST == nil then
        return 0
    end
    return table.getn(DATALIST)
end
function ReviveRankView:onExit()
    if nil ~= self.ccbNode then
        self.ccbNode:unregisterScriptHandler()
    end
    -- local targetPlatform = cc.Application:getInstance():getTargetPlatform()
    -- if (3 == targetPlatform) then
    --     releaseLuaResourceForAndroid(ROOTPATH .. "/resources/" , "revive")
    -- else
    --     releaseLuaResource(ROOTPATH .. "/resources/revive")
    -- end
end

function ReviveRankView:CMDCallbackForLua(cmdname,data)
    -- body
    print("ReviveRankView:CMDCallbackForLua")
    if cmdname == "cemetery.get.ranking" then
        local ref = tolua.cast(data,"cc.Ref")
        local backTab = LuaController:getInstance():getDicLuaData(ref)
        if backTab["errorCode"] ~= nil then
            return
        end
        if tonumber(backTab["myRanking"]) <= 0 then
            self.m_timeLeftText:setString(LuaController:getLang("145036"))
        else
            self.m_timeLeftText:setString(tostring(backTab["myRanking"]))
        end
        
        DATALIST = backTab["serverRanking"]
        for k,v in pairs(DATALIST) do
            print(k,v)
            for k1,v1 in pairs(v) do
                print(k1,v1)
            end
        end
        if self.m_tabView ~= nil then
            self.m_tabView:reloadData()
        end
        if table.getn(DATALIST) > 0 then
            self.m_lblTip:setString("")
        else
            self.m_lblTip:setString(LuaController:getLang("145037"))
        end

    end
end
function ReviveRankView:NotificationCallbackForLua(cmdname)
    -- body
    print("ReviveRankView:NotificationCallbackForLua")
end
function ReviveRankView:NotificationCallbackForLuaWithParams(cmdname,object)
    -- body
    print("ReviveRankView:NotificationCallbackForLuaWithParams")
end