;(function(){
	var AccessSDKJS = function(){};

	AccessSDKJS.initSDK = function(){
		//无参数初始化
		alert("call initSDK");
		_OC_log("initSDK");
	};

	AccessSDKJS.initSDK = function(autoLogin) {
		//初始化，是否启动自动登录
		_OC_log("initSDK autoLogin:" + autoLogin);
	};

	AccessSDKJS.login = function(){
		//登录接口
		_OC_log("login");
	};

	AccessSDKJS.logout = function() {
		//登出接口
		_OC_log("logout");
	};

	AccessSDKJS.pay = function(itemName, amount, callbackInfo) {
		//支付接口
		_OC_log("pay itemName:" + itemName + " amount:" + amount + " callbackInfo:" + callbackInfo);
	};

	AccessSDKJS.setUserExtData = function(setUserExtData) {
		//用户上报接口
		_OC_log("setUserExtData");
	};

	AccessSDKJS.sendBIData =  function(metric, jsonData) {
		//bi上报
		_OC_log("sendBIData");
	};

})();