//
//  adBertIADGameSDK.h
//  iOSSample
//
//  Created by Dennies Chang on 2015/1/28.
//  Copyright (c) 2015年 adbert. All rights reserved.
//

#import <AdSupport/AdSupport.h>
#import <Foundation/Foundation.h>
#import "MIPFileConnection.h"

#define ver @"adbert iOS Game SDK 1.7 build 7"
#define tagReturn 99999
#define returnAPI @"http://www.adbert.com.tw/portal/analysis/openreturn/"
#define GAME_ID @"2014081800016"
#define APPLY_ID @"3e49a5668fa2e99ef2787ec16cc83c27"


@interface adBertIADGameSDK : NSObject <MIPFileConnectionDelegate>
@property (nonatomic, strong) NSString *gameID;
@property (nonatomic, strong) NSString *applyID;

- (BOOL)initialize;

@end
