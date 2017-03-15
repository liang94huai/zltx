//
//  ACReturnValue.h
//  accesssdk
//
//  Created by zhao shuan on 11/17/14.
//  Copyright (c) 2014 zs_hoolai. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ACReturnValue : NSObject {
    NSString* code;
    NSString* group;
    NSString* desc;
}

@property (nonatomic, readonly) NSString* code;
@property (nonatomic, readonly) NSString* group;
@property (nonatomic, readonly) NSString* desc;

@end
