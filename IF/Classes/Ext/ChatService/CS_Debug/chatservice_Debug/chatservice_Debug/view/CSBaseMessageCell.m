//
//  CSBaseMessageCell.m
//  chatservice_Debug
//
//  Created by 张彦涛 on 15/12/7.
//  Copyright © 2015年 张彦涛. All rights reserved.
//

#import "CSBaseMessageCell.h"
#import "CSMessageModel.h"
#import "UIViewExt.h"
#import "Cellconfig.h"
@implementation CSBaseMessageCell

-(void)initialize{
//    CSBaseMessageCell *cell = [self appearance];
    self.resendButtonSize = cellConfig.resendButtonSize;
    self.activitySize = cellConfig.activitySize;
    self.bubbleMaxWidth = cellConfig.bubbleMaxWidth;
    self.leftBubbleMargin = cellConfig.leftBubbleMargin;
    self.rightBubbleMargin = cellConfig.rightBubbleMargin;
    self.avatarMargin = cellConfig.avatarMargin;
}

- (instancetype)initWithStyle:(UITableViewCellStyle)style
              reuseIdentifier:(NSString *)reuseIdentifier
                        model:(CSMessageModel *)vModel
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        
        [self initialize];
        [self _setupSubViews];
    }
    
    return self;
}
-(void)_setupSubViews{
   
    _cellView = [[CellView alloc]initWithFrame:CGRectZero];
    [self.contentView addSubview:_cellView];
 
    _bubbleView =[[CSBubbleView alloc]initWithFrame:CGRectZero];
    _bubbleView.backgroundColor =[UIColor clearColor];
    [self.cellView.bodyView addSubview:_bubbleView];
    //触摸事件
    [_cellView.reSendButton addTarget:self action:@selector(resendButtonAction) forControlEvents:UIControlEventTouchUpInside];
    UITapGestureRecognizer *tapRecognizer2 = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(avatarViewTapAction:)];
    [_cellView.avatarView addGestureRecognizer:tapRecognizer2];
    UITapGestureRecognizer *tapRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(bubbleViewTapAction:)];
    tapRecognizer.cancelsTouchesInView = NO;
//    tapRecognizer.delaysTouchesBegan = YES;
    [_bubbleView addGestureRecognizer:tapRecognizer];

}


- (BOOL)canBecomeFirstResponder
{
    return YES;
}

-(void)setMessageModel:(CSMessageModel *)messageModel{
    _messageModel = messageModel;
    [self.cellView setFrame:CGRectMake(0, 0, kMainScreenWidth, messageModel.cellHeight)];
    if (messageModel.isSelfSender) {
        _bubbleView.margin = _rightBubbleMargin;
    }else{
        _bubbleView.margin = _leftBubbleMargin;
    }
    _bubbleView.msgModel = messageModel;
  
}
#pragma mark - action
- (void)avatarViewTapAction:(UITapGestureRecognizer *)tapRecognizer
{
    if (_msgTableCellDelegate && [_msgTableCellDelegate respondsToSelector:@selector(msgCellAvatarViewSelected:)]) {
        [_msgTableCellDelegate msgCellAvatarViewSelected:self];
    }
    
}

- (void)bubbleViewTapAction:(UITapGestureRecognizer *)tapRecognizer
{
    
    NSLog(@"点击bubbleView");
    if (tapRecognizer.state == UIGestureRecognizerStateEnded) {
        if (_msgTableCellDelegate && [_msgTableCellDelegate respondsToSelector:@selector(msgcellBubbleViewSelected:)]) {
            [_msgTableCellDelegate msgcellBubbleViewSelected:self];
        }
    }
}
- (void)resendButtonAction
{
    if (_msgTableCellDelegate && [_msgTableCellDelegate respondsToSelector:@selector(msgcellResendButtonSelected:)]) {
        [_msgTableCellDelegate msgcellResendButtonSelected:self];
    }
}
@end
