//
//  ServerInfos.h
//  accesssdk
//
//  Created by Hoolai on 15/2/26.
//  Copyright (c) 2015年 wsj_hoolai. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ServerInfos : NSObject {
    NSMutableArray* userServerList;
    NSMutableArray* serverList;
}

@property (nonatomic, readonly) NSMutableArray* userServerList;
@property (nonatomic, readonly) NSMutableArray* serverList;

@end
