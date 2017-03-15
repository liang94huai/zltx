//
//  ACServerInfo.h
//  accesssdk
//
//  Created by zhao shuan on 11/17/14.
//  Copyright (c) 2014 zs_hoolai. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ACServerInfo : NSObject<NSCoding> {
    NSString* serverId;
    NSString* serverName;
    NSString* serverAddress;
    int productId;
    NSString* status;
    int order;
    int hot;
    NSString* discription;
    BOOL purchaseOpen;
    NSString* purchaseNotifyUri;
}

@property (nonatomic, readonly) NSString* serverId;
@property (nonatomic, readonly) NSString* serverName;
@property (nonatomic, readonly) NSString* serverAddress;
@property (nonatomic, readonly) int productId;
@property (nonatomic, readonly) NSString* status;
@property (nonatomic, readonly) int order;
@property (nonatomic, readonly) int hot;
@property (nonatomic, readonly) NSString* discription;
@property (nonatomic, readonly) BOOL purchaseOpen;
@property (nonatomic, readonly) NSString* purchaseNotifyUri;

@end
