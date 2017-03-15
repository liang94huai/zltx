package com.clash.of.util;

import org.json.JSONException;

public class Purchase {
    String orderId;
    String openid;
    String itemId;

    public Purchase(String orderId, String openid, String itemId) throws JSONException {
    	this.orderId = orderId;
    	this.openid = openid;
    	this.itemId = itemId;
    }

    public String getOrderId() { return orderId; }
    public String getOpenid() { return openid; }
    public String getItemId() { return itemId; }

    @Override
    public String toString() { return "PurchaseInfo(orderId:" + orderId + " openid:" + openid + " itemId:" + itemId + ")"; }
}
