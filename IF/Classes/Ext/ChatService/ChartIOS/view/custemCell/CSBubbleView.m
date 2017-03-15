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
#import "NSString+Cocos2dHelper.h"
#import "YYKit.h"
@implementation CSBubbleView{
       
   
}

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        self.backgroundColor =[UIColor clearColor];
        self.userInteractionEnabled = YES;
         [self _setUpSubViews];
    }
    
    return self;
}
-(void)clearContentSubViews{
    [_textLabel removeFromSuperview];
    _textLabel = nil;
    [_translatExplainLabel removeFromSuperview];
    _translatExplainLabel = nil;
    [_redPackgeTimeLabel removeFromSuperview];
    _redPackgeTimeLabel = nil;
    [_redPackgeTextLabel removeFromSuperview];
    _redPackgeTextLabel = nil;
    [_redPackgeSubExplainLabel removeFromSuperview];
    _redPackgeSubExplainLabel = nil;
    [_redPackgeLogoImageView removeFromSuperview];
    _redPackgeLogoImageView =nil;
}

-(void)_setUpSubViews{
    _bubbleBackImageView = [[UIImageView alloc]initWithFrame:CGRectZero];
    
    [self addSubview:_bubbleBackImageView];
    _showContentsView =[[UIView alloc]initWithFrame:CGRectZero];
    _showContentsView.backgroundColor =[UIColor clearColor];
    [self addSubview:_showContentsView];
}

-(void)setMsgModel:(CSMessageModel *)msgModel{
    [self clearContentSubViews];
    _msgModel = msgModel;
//    _showContentsView.backgroundColor=[UIColor greenColor];
     _showContentsView.frame = CGRectMake(_margin.left, _margin.top, msgModel.textBubbleSize.width, msgModel.textBubbleSize.height);
    if (msgModel.messageBodyType ==CSMessageBodyType_RedPackge){
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
            self.redPackgeLogoImageView.frame =CGRectMake(0, 6, 25, 30);
        }else{
             self.redPackgeLogoImageView.frame =CGRectMake(0, 5, 35, 50);
        }
       
        self.redPackgeLogoImageView.image =[UIImage imageNamed:@"cs_red_package"];
       
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
             self.redPackgeTextLabel.width= _showContentsView.width - _redPackgeLogoImageView.width -cellConfig.avatarMargin/2;
            self.redPackgeTextLabel.height = 30;
             _redPackgeTextLabel.left = _redPackgeLogoImageView.right+cellConfig.avatarMargin/2;
        }else{
             self.redPackgeTextLabel.width= _showContentsView.width - _redPackgeLogoImageView.width -cellConfig.avatarMargin;
            self.redPackgeTextLabel.height = 50;
             _redPackgeTextLabel.left = _redPackgeLogoImageView.right+cellConfig.avatarMargin;
        }
       
        _redPackgeTextLabel.text = msgModel.showContentsString;
       
        _redPackgeTextLabel.top = self.redPackgeLogoImageView.top;
        
        NSString *redPackageSubLableText = [NSString stringWithMultilingualWithKey:@"104986"];
        self.redPackgeSubExplainLabel.text =redPackageSubLableText;//104986=点击查看
        _redPackgeSubExplainLabel.left = _redPackgeLogoImageView.left;
          if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
              _redPackgeSubExplainLabel.top = 47;
          }else{
              _redPackgeSubExplainLabel.top = 75;
          }
        
        [_redPackgeSubExplainLabel sizeToFit];
        
        NSString *timeString = [[msgModel.timeString componentsSeparatedByString:@" "] lastObject];
        self.redPackgeTimeLabel.text = timeString;
        [_redPackgeTimeLabel sizeToFit];
        _redPackgeTimeLabel.right = _showContentsView.width;
        _redPackgeTimeLabel.top = _redPackgeSubExplainLabel.top;
    }else{
        
        self.textLabel.font = cellConfig.contentsTextFont;
        _textLabel.width=_msgModel.showContentsSize.width;
        _textLabel.height=_msgModel.showContentsSize.height;
        _textLabel.textLayout = _msgModel.newshowAttContentsString;
        _textLabel.textAlignment=NSTextAlignmentLeft;
        
        if (msgModel.isTranslatLabelShow == YES) {
            //显示翻译信息
            self.translatExplainLabel.text = msgModel.translatExplainString;
            self.translatExplainLabel.textAlignment = NSTextAlignmentRight;
            _translatExplainLabel.frame =CGRectMake(0, _textLabel.bottom, _showContentsView.width, msgModel.showTranslatExplainSize.height);
        }else{
            //不显示翻译信息
            [self.translatExplainLabel removeFromSuperview];
            self.translatExplainLabel= nil;
        }

    }
}



-(YYLabel *)textLabel{
    if (_textLabel == nil) {
        _textLabel = [YYLabel new];
        _textLabel.left=0;
        _textLabel.top=0;
        _textLabel.width = cellConfig.bubbleMaxWidth;
       
//        _textLabel.displaysAsynchronously = NO;
//        _textLabel.ignoreCommonProperties = YES;
//        _textLabel.fadeOnAsynchronouslyDisplay = NO;
//        _textLabel.fadeOnHighlight = NO;

        _textLabel.backgroundColor =[UIColor clearColor];
        //_textLabel.userInteractionEnabled=YES;
        _textLabel.numberOfLines = 0;
        _textLabel.textAlignment=NSTextAlignmentLeft;
        _textLabel.textVerticalAlignment = YYTextVerticalAlignmentTop;
        _textLabel.textColor = cellConfig.contentsTextColor;
        _textLabel.font = cellConfig.contentsTextFont;
        CSBubbleView *weakSelf = self;
        _textLabel.highlightTapAction = ^(UIView *containerView, NSAttributedString *text, NSRange range, CGRect rect) {
            
            YYLabel * pass=(YYLabel*)containerView;
            YYTextHighlight *highlight = [pass.textLayout.text attribute:YYTextHighlightAttributeName atIndex:range.location];
            NSDictionary *info = highlight.userInfo;
            NSString * targetStr=[text string];
            targetStr=[targetStr substringWithRange:NSMakeRange(range.location, range.length)];
            
            if (info.allKeys.count==0) {
                return ;
            }
            if ([[info.allKeys objectAtIndex:0] isEqualToString:kCSTouch_Link]) {
                DVLog(@"超链接 : %@",targetStr);
                if (weakSelf.bubbleViewDelegate &&[weakSelf.bubbleViewDelegate respondsToSelector:@selector(bubbleViewTouchDownStringWithBubbleView:andWithTouchType:andWithTouchString:)]) {
                    [weakSelf.bubbleViewDelegate bubbleViewTouchDownStringWithBubbleView:weakSelf andWithTouchType:kCSTouch_Link  andWithTouchString:targetStr];
                }
            }else if ([[info.allKeys objectAtIndex:0] isEqualToString:kCSTouch_Point]) {
                DVLog(@"坐标 : %@",targetStr);
                if (weakSelf.bubbleViewDelegate &&[weakSelf.bubbleViewDelegate respondsToSelector:@selector(bubbleViewTouchDownStringWithBubbleView:andWithTouchType:andWithTouchString:)]) {
                    [weakSelf.bubbleViewDelegate bubbleViewTouchDownStringWithBubbleView:weakSelf andWithTouchType:kCSTouch_Point andWithTouchString:targetStr];
                }
            }else if ([[info.allKeys objectAtIndex:0] isEqualToString:kCSTouch_EquipName]) {
                DVLog(@"装备名称: %@",targetStr);
                if (weakSelf.bubbleViewDelegate &&[weakSelf.bubbleViewDelegate respondsToSelector:@selector(bubbleViewTouchDownStringWithBubbleView:andWithTouchType:andWithTouchString:)]) {
                    [weakSelf.bubbleViewDelegate bubbleViewTouchDownStringWithBubbleView:weakSelf andWithTouchType:kCSTouch_EquipName andWithTouchString:targetStr];
                }
            }else if ([[info.allKeys objectAtIndex:0] isEqualToString:kCSTouch_AllianceTask]){
                DVLog(@"联盟任务 : %@",targetStr);
                if (weakSelf.bubbleViewDelegate &&[weakSelf.bubbleViewDelegate respondsToSelector:@selector(bubbleViewTouchDownStringWithBubbleView:andWithTouchType:andWithTouchString:)]) {
                    [weakSelf.bubbleViewDelegate bubbleViewTouchDownStringWithBubbleView:weakSelf andWithTouchType:kCSTouch_AllianceTask andWithTouchString:targetStr];
                }
            }
        };
        [_showContentsView addSubview:_textLabel];

 
    }
    return _textLabel;
}
-(UILabel *)translatExplainLabel{
    if (_translatExplainLabel == nil) {
        _translatExplainLabel =[[UILabel alloc]initWithFrame:CGRectZero];
        _translatExplainLabel.backgroundColor =[UIColor clearColor];
        _translatExplainLabel.textColor =cellConfig.translatExplainTextColor;
        _translatExplainLabel.numberOfLines = 1;
        _translatExplainLabel.font  = cellConfig.translatExplainFont;
         [_showContentsView addSubview:_translatExplainLabel];
    }
   

    return _translatExplainLabel;
}

-(UIImageView *)redPackgeLogoImageView{
    if (_redPackgeLogoImageView ==nil) {
        _redPackgeLogoImageView =[[UIImageView alloc]initWithFrame:CGRectZero];
         _redPackgeLogoImageView.backgroundColor =[UIColor clearColor];
        [_showContentsView addSubview:_redPackgeLogoImageView];
    }
    return _redPackgeLogoImageView;
}

-(UILabel *)redPackgeSubExplainLabel{
    if (_redPackgeSubExplainLabel == nil) {
        _redPackgeSubExplainLabel =[[UILabel alloc]initWithFrame:CGRectZero];
         _redPackgeSubExplainLabel.backgroundColor =[UIColor clearColor];
        _redPackgeSubExplainLabel.font =cellConfig.redSubTextFont;
        _redPackgeSubExplainLabel.textColor = cellConfig.translatExplainTextColor;
        [_showContentsView addSubview:_redPackgeSubExplainLabel];
    }
    return _redPackgeSubExplainLabel;
}

-(YYLabel *)redPackgeTextLabel{
    if (_redPackgeTextLabel == nil) {
        _redPackgeTextLabel =[[YYLabel alloc]initWithFrame:CGRectZero];
         _redPackgeTextLabel.backgroundColor =[UIColor clearColor];
        _redPackgeTextLabel.textVerticalAlignment = YYTextVerticalAlignmentTop;
        _redPackgeTextLabel.font =[UIFont systemFontOfSize:cellConfig.redPacketMsgTextFont];
        _redPackgeTextLabel.numberOfLines = 2;
        _redPackgeTextLabel.textColor = cellConfig.contentsTextColor;
        [_showContentsView addSubview:_redPackgeTextLabel];
    }
    return _redPackgeTextLabel;
}
-(UILabel *)redPackgeTimeLabel{
    if (_redPackgeTimeLabel == nil) {
        _redPackgeTimeLabel =[[UILabel alloc]initWithFrame:CGRectZero];
        _redPackgeTimeLabel.textAlignment = NSTextAlignmentRight;
         _redPackgeTimeLabel.backgroundColor =[UIColor clearColor];
        _redPackgeTimeLabel.font = cellConfig.redTimeTextFont;
        _redPackgeTimeLabel.textColor = cellConfig.translatExplainTextColor;
        [_showContentsView addSubview:_redPackgeTimeLabel];
    }
    return _redPackgeTimeLabel;
}
@end
