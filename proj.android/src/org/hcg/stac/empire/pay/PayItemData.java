package org.hcg.stac.empire.pay;

import java.io.Serializable;

import org.cocos2dx.ext.Native;
import org.hcg.stac.empire.publish.IPublishChannel;
import org.hcg.util.GameContext;

public class PayItemData implements Serializable {

	private static final long serialVersionUID = -598181875871093995L;

	private String userId;//用户
	private String toUserID = "";//赠送UID
	private String itemId;
	private int coin;//获得金币数
	private float price;
//	private String channel = "";

	
	public String getUserId(){
		return userId;
	}
	public void setUserId(String userId){
		this.userId = userId;
	}
	
	public String getToUserID(){
		return toUserID;
	}
	public void setToUserID(String toUid){
		this.toUserID = toUid;
	}
	
	public float getPrice(){
		return price;
	}
	public void setPrice(float price){
		this.price = price;
	}
	
	public int getCoin() {
		return coin;
	}

	public void setCoin(int coin) {
		this.coin = coin;
	}

	public String getItemId() {
		return itemId;
	}

	public void setItemId(String itemId) {
		this.itemId = itemId;
	}
	
//	public String getChannel(){
//		return channel;
//	}
//	public void setChannel(String itemType) {
//		this.channel = itemType;
//	}

	public String getProductName(){
		String productName = String.format("%d金币礼包", getCoin());
		if("9006".equals(getItemId()))
			productName = "月卡";
		return productName;
	}
	
	public String getProductNameEn(){
		String productName = String.format("%dGoldPackage", getCoin());
		if("9006".equals(getItemId()))
			productName = "MonthlyCard";
		return productName;
	}
	
	public String getOrderId(){
		//目前最大位数为40
		String orderId = String.format("%s_%d_%s_%d", 
				getUserId(), // UID   目前最大16位
				Native.nativeGetServerId(), // 服务器ID 4+1
				getItemId(), // xmlId 4+1
				System.currentTimeMillis() // 当前时间 13+1
				);
		return orderId;
	}



	
	
	/**
	 * 
	 * 支付配置
	 * 
	 */
	static class PaymentPrice {
		
		public static final float[] PRICE_USD = new float[] { 4.99f, 9.99f, 19.99f, 49.99f, 99.99f, 24.99f, 999.99f, 0.99f };
		public static final float[] PRICE_CNY = new float[] { 30.0f, 68.0f, 128.0f, 328.0f, 648.0f, 163.0f, 6498.0f, 6.0f };
		public static final float[] PRICE_THB = new float[] { 160.0f, 330.0f, 650.0f, 1650.0f, 3250.0f, 830.0f, 32500.0f, 35.0f };

		public static final String[] PRODUCT_COMMON = new String[] { "gold_1", "gold_2", "gold_3", "gold_4", "gold_5", "gold_6", "gold_7", "gold_8", "gold_11" };
		public static final String[] PRODUCT_NSTORE = new String[] { "gold_1", "gold_2", "gold_3", "gold_4", "gold_5", "gold_6", "gold_7", "gold_8" };
	}
	
	public static enum Currency{
		
		USD,
		CNY,
		THB,
		
	}
	
	
	private int getProductIndex(float price){
		int index = -1;
		for(int i=0;i<PaymentPrice.PRICE_USD.length;i++){
			if(PaymentPrice.PRICE_USD[i] == price)
				index = i;
		}
		return index;
	}

	public float getPaidAmount(Currency currency){
		float amount = 0;
		float[] priceList = PaymentPrice.PRICE_USD;
		if(currency.equals(Currency.CNY))
			priceList = PaymentPrice.PRICE_CNY;
		if(currency.equals(Currency.THB))
			priceList = PaymentPrice.PRICE_THB;
		int index = getProductIndex(price);
		if(index != -1){
			amount = priceList[index];
		}
		return amount;
	}
	
	public int getPaidAmountInCents(Currency currency){
		float amount = getPaidAmount(currency);
		return (int)(Math.ceil(amount*100));
	}
	
	public static float getPaidAmount(Currency currency,float price){
		PayItemData payItemData = new PayItemData();
		payItemData.setPrice(price);
		return payItemData.getPaidAmount(currency);
	}
	
	public static int getPaidAmountInCents(Currency currency,float price){
		PayItemData payItemData = new PayItemData();
		payItemData.setPrice(price);
		return payItemData.getPaidAmountInCents(currency);
	}
	
	//部分渠道通过productid支付
	public String getPaidProduct(){
		String product = "";
		String[] productList = PaymentPrice.PRODUCT_COMMON;
		if(GameContext.getGamePublisher().getPublishChannel().equals(IPublishChannel.PUBLISH_NSTORE))
			productList = PaymentPrice.PRODUCT_NSTORE;
		int index = getProductIndex(price);
		if(index != -1){
			product = productList[index];
		}
		return product;
	}

}
