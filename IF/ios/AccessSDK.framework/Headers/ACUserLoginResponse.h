//
//  ACUserLoginResponse.h
//  accesssdk
//
//  Created by zhao shuan on 11/17/14.
//  Copyright (c) 2014 zs_hoolai. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ACServerInfo.h"

@interface ACUserLoginResponse : NSObject<NSCoding> {
    long long uid;
    NSString* accessToken;
    ACServerInfo* lastServerInfo;
    NSString* nickName;
    NSDictionary* extendInfo;
    NSString* channelUid;
    NSString* channel;
    NSDate* expirationDate;
    NSInteger productId;
}

@property (nonatomic, readonly) long long uid;
@property (nonatomic, readonly) NSString* accessToken;
@property (nonatomic, readonly) ACServerInfo* lastServerInfo;
@property (nonatomic, readonly) NSString* nickName;
@property (nonatomic, readonly) NSDictionary* extendInfo;
@property (nonatomic, readonly) NSString* channelUid;
@property (nonatomic, readonly) NSString* channel;
@property (nonatomic, retain) NSDate* expirationDate;
@property (nonatomic, readonly) NSInteger productId;

@end
