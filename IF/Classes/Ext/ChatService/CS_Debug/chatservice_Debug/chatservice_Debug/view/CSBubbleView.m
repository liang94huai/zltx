//
//  CSBubbleView.m
//  chatservice_Debug
//
//  Created by 张彦涛 on 15/12/7.
//  Copyright © 2015年 张彦涛. All rights reserved.
//

#import "CSBubbleView.h"
#import "UIImage+resized.h"
#import "UIViewExt.h"
#import "Cellconfig.h"
@implementation CSBubbleView{
       
   
}

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        self.backgroundColor =[UIColor clearColor];
         [self _setUpSubViews];
    }
    
    return self;
}

-(void)_setUpSubViews{
    _bubbleBackImageView = [[UIImageView alloc]initWithFrame:CGRectZero];
    
    [self addSubview:_bubbleBackImageView];
    _showContentsView =[[UIView alloc]initWithFrame:CGRectZero];
    [self addSubview:_showContentsView];
}

-(void)setMsgModel:(CSMessageModel *)msgModel{
    _showContentsView.frame = CGRectMake(_margin.left, _margin.top, msgModel.textBubbleSize.width, msgModel.textBubbleSize.height);
    _msgModel = msgModel;
    
    
    self.textLabel.attributedText = msgModel.showAttContentsString;
    [self.textLabel sizeToFit];
    _textLabel.frame =CGRectMake(0, 0, msgModel.showContentsSize.width, msgModel.showContentsSize.height);
    [_showContentsView addSubview:_textLabel];
    
    
    
    if (msgModel.isSelfSender) {
        _bubbleBackImageView.image =[UIImage resizableImage:@"aio_user_bg_nor"];
    }else{
        _bubbleBackImageView.image =[UIImage resizableImage:@"aio_friend_bg_nor"];
    }
    self.timeLabel.text = msgModel.translatExplainString;
    self.timeLabel.textAlignment = NSTextAlignmentRight;
    _translatExplainLabel.frame =CGRectMake(0, _textLabel.bottom, _showContentsView.width, msgModel.showTranslatExplainSize.height);
    [_showContentsView addSubview:_translatExplainLabel];
}



-(YYLabel *)textLabel{
    if (_textLabel == nil) {
       // _textLabel.backgroundColor = [UIColor colorWithWhite:0.809 alpha:1.000];
        
        _textLabel = [YYLabel new];
        _textLabel.size = CGSizeMake(cellConfig.bubbleMaxWidth, MAXFLOAT);
        _textLabel.backgroundColor =[UIColor clearColor];
        _textLabel.userInteractionEnabled=YES;
        _textLabel.numberOfLines = 0;
        _textLabel.textVerticalAlignment = YYTextVerticalAlignmentTop;
        _textLabel.textColor = cellConfig.contentsTextColor;
        _textLabel.font = cellConfig.contentsTextFont;
        _textLabel.highlightTapAction = ^(UIView *containerView, NSAttributedString *text, NSRange range, CGRect rect) {
            
            YYLabel * pass=(YYLabel*)containerView;
            YYTextHighlight *highlight = [pass.textLayout.text attribute:YYTextHighlightAttributeName atIndex:range.location];
            NSDictionary *info = highlight.userInfo;
            NSString * targetStr=[text string];
            targetStr=[targetStr substringWithRange:NSMakeRange(range.location, range.length)];
            
            if (info.allKeys.count==0) {
                return ;
            }
            if ([[info.allKeys objectAtIndex:0] isEqualToString:@"超链接"]) {
                
                NSLog(@"超链接 : %@",targetStr);
                
            }else if ([[info.allKeys objectAtIndex:0] isEqualToString:@"坐标"]) {
                
                NSLog(@"坐标 : %@",targetStr);
            }
        };

    }
    return _textLabel;
}
-(UILabel *)timeLabel{
    if (_translatExplainLabel == nil) {
        _translatExplainLabel =[[UILabel alloc]initWithFrame:CGRectZero];
        _translatExplainLabel.backgroundColor =[UIColor clearColor];
        _translatExplainLabel.textColor =cellConfig.translatExplainTextColor;
        _translatExplainLabel.numberOfLines = 1;
        _translatExplainLabel.font  = cellConfig.translatExplainFont;
    }
    return _translatExplainLabel;
}
@end
