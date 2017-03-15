//
//  ShopIAPHelper.h
//  GOE
//
//  Created by Limin on 12-4-1.
//  Copyright 2011年 Elex. All rights reserved.
//

#import "IAPHelper.h"
#import "XMBProgressHUD.h"

#define AOE_IAP_COUNT     6

extern NSString* gIapName[AOE_IAP_COUNT];
extern NSString* gIapMoreGoldName[AOE_IAP_COUNT];

@interface ShopIAPHelper : IAPHelper
{
	XMBProgressHUD *_hud;
}

@property (nonatomic, retain) XMBProgressHUD* hud;

+(ShopIAPHelper *) sharedHelper;
-(void)purchaseItem:(NSString*)productIdentifier buyItemId:(NSString *)itemId buyCount:(NSInteger)count;

-(void)showHUD;
-(void)hideHUD;

@end
