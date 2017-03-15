//
//  CSBubbleView.h
//  chatservice_Debug
//
//  Created by 张彦涛 on 15/12/7.
//  Copyright © 2015年 张彦涛. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CSMessageModel.h"
#import "YYKit.h"
@interface CSBubbleView : UIView

@property (nonatomic,assign) UIEdgeInsets margin;
@property (nonatomic,strong)UIImageView *bubbleBackImageView;
@property (nonatomic,strong)UIView *showContentsView;
@property (strong,nonatomic)CSMessageModel *msgModel;
//text views
@property (strong, nonatomic) YYLabel *textLabel;
@property (strong, nonatomic) UILabel *translatExplainLabel;

@end
