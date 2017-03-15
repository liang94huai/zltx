//
//  Cellconfig.m
//  chatservice_Debug
//
//  Created by 张彦涛 on 15/12/13.
//  Copyright © 2015年 张彦涛. All rights reserved.
//

#import "Cellconfig.h"

@implementation Cellconfig


+ (Cellconfig *)sharedCellConfig
{
    static Cellconfig *sharedCellConfig = nil;
    static dispatch_once_t predicate;
    dispatch_once(&predicate, ^{
        sharedCellConfig = [[Cellconfig alloc] init];
    });
    return sharedCellConfig;
}

-(instancetype)init
{
    self = [super init];
    if (self) {
        [self _settingConfig];
    }
    return self;
}

-(void)_settingConfig{
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
        self.resendButtonSize = 20;
        self.activitySize = 20;
        self.bubbleMaxWidth = kMainScreenWidth * 0.55;
        self.leftBubbleMargin = UIEdgeInsetsMake(15, 25, 15, 20);
        self.rightBubbleMargin = UIEdgeInsetsMake(15, 20, 15, 25);
        self.avatarMargin = 10;
        
        self.timeLabelFont = [UIFont systemFontOfSize:12.0f];
        self.timeLabelHeight = 30;
        self.timeLabelTextColor = [UIColor whiteColor];
        self.timeLabelbackColor =[UIColor colorWithRed:204/255.0 green:206/255.0 blue:210/255.0 alpha:1];
        
        self.avatarSize = 40;
        self.avatarCornerRadius = 5;
        
        self.messageNameColor = [UIColor darkGrayColor];
        self.messageNameFont = [UIFont systemFontOfSize:13];
        self.messageNameHeight = 15;

        self.contentsTextSize = 16;
        self.contentsTextFont =[UIFont systemFontOfSize:16.0f];
        self.contentsTextColor =[UIColor blackColor];
        
        self.translatExplainFont =[UIFont systemFontOfSize:12];
        self.translatExplainTextColor =[UIColor grayColor];
        
    }else if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad){
        
    }
    
    
}



@end
