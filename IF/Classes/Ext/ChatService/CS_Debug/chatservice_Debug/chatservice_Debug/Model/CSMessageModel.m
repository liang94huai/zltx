//
//  CSMessageModel.m
//  chatservice_Debug
//
//  Created by 张彦涛 on 15/12/7.
//  Copyright © 2015年 张彦涛. All rights reserved.
//

#import "CSMessageModel.h"
#import "CSMessage.h"
#import "NSString+Extension.h"
#import "YYKit.h"
#import "Cellconfig.h"
@interface CSMessageModel()
{
 
        YYLabel* label;
 
}
@end
@implementation CSMessageModel
- (instancetype)initWithMessage:(CSMessage *)message{
    self = [super init];
    if (self) {
        _cellHeight = -1;
        _message = message;
        _isSelfSender = YES;
        _nickname = @"张三";
        _translatExplainString = @"4:30";
        _isTimeLabelShow = YES;
        _timeString = @"今天 16：30";
    }
   
    return self;
}
-(CGSize)needH:(NSMutableAttributedString*)text
{
    
 
    if (label==nil) {
        label=[YYLabel new];
    }

    label.numberOfLines = 0;
    label.font =cellConfig.contentsTextFont;
    label.size = CGSizeMake(cellConfig.bubbleMaxWidth, MAXFLOAT);
    label.frame=CGRectMake(0, 0, cellConfig.bubbleMaxWidth,222);
    label.textVerticalAlignment = YYTextVerticalAlignmentTop;
    label.attributedText = text;
    [label sizeToFit];
    return label.size;
   
}


-(NSMutableAttributedString*)returnText:(NSMutableAttributedString*)text
{
    
    //匹配坐标
    YYTextBorder *highlightBorder = [YYTextBorder new];
    highlightBorder.insets = UIEdgeInsetsMake(-2, 0, -2, 0);
    highlightBorder.cornerRadius = 3;
    highlightBorder.fillColor = [UIColor orangeColor];
    
    
    NSRegularExpression *regexCoordinate=
    [NSRegularExpression regularExpressionWithPattern:@"(1200|[1][0-1][0-9]{2}|[1-9][0-9]{2}|[1-9][0-9]|[0-9])([:：])(1200|[1][0-1][0-9]{2}|[1-9][0-9]{2}|[1-9][0-9]|[0-9])" options:kNilOptions error:NULL];
    NSArray *atResultsCoordinate = [regexCoordinate matchesInString:text.string options:0 range:text.rangeOfAll];
    for (NSTextCheckingResult *at in atResultsCoordinate) {
        if (at.range.location == NSNotFound && at.range.length <= 1) continue;
        if ([text attribute:YYTextHighlightAttributeName atIndex:at.range.location] == nil) {
            [text setColor:[UIColor blueColor] range:at.range];
            
            // 高亮状态
            YYTextHighlight *highlight = [YYTextHighlight new];
            [highlight setBackgroundBorder:highlightBorder];
            // 数据信息，用于稍后用户点击
            highlight.userInfo = @{@"坐标" : [text.string substringWithRange:NSMakeRange(at.range.location, at.range.length )]};
            [text setTextHighlight:highlight range:at.range];
        }
    }

    //匹配超链接
    

    NSRegularExpression *regexUrl=
    [NSRegularExpression regularExpressionWithPattern:@"((http[s]{0,1}|ftp)://[a-zA-Z0-9\\.\\-]+\\.([a-zA-Z]{2,4})(:\\d+)?(/[a-zA-Z0-9\\.\\-~!@#$%^&*+?:_/=<>]*)?)|(www.[a-zA-Z0-9\\.\\-]+\\.([a-zA-Z]{2,4})(:\\d+)?(/[a-zA-Z0-9\\.\\-~!@#$%^&*+?:_/=<>]*)?)" options:kNilOptions error:NULL];
    NSArray *atResultsUrl = [regexUrl matchesInString:text.string options:0 range:text.rangeOfAll];
    for (NSTextCheckingResult *at in atResultsUrl) {
        if (at.range.location == NSNotFound && at.range.length <= 1) continue;
        if ([text attribute:YYTextHighlightAttributeName atIndex:at.range.location] == nil) {
            [text setColor:[UIColor blueColor] range:at.range];
            
            // 高亮状态
            YYTextHighlight *highlight = [YYTextHighlight new];
            [highlight setBackgroundBorder:highlightBorder];
            // 数据信息，用于稍后用户点击
            highlight.userInfo = @{@"超链接" : [text.string substringWithRange:NSMakeRange(at.range.location, at.range.length )]};
            [text setTextHighlight:highlight range:at.range];
        }
    }

    
    //匹配表情
    NSRegularExpression *regex=
    [NSRegularExpression regularExpressionWithPattern:@"\\[[^ \\[\\]]+?\\]" options:kNilOptions error:NULL];
    NSArray *emoticonResults = [regex matchesInString:text.string options:0 range:text.rangeOfAll];
    NSUInteger emoClipLength = 0;
    for (NSTextCheckingResult *emo in emoticonResults) {
        if (emo.range.location == NSNotFound && emo.range.length <= 1) continue;
        NSRange range = emo.range;
        range.location -= emoClipLength;
        if ([text attribute:YYTextHighlightAttributeName atIndex:range.location]) continue;
        if ([text attribute:YYTextAttachmentAttributeName atIndex:range.location]) continue;
        NSString *emoString = [text.string substringWithRange:range];
        NSAttributedString *emoText;
        
        if ([emoString isEqualToString:@"[1]"]) {
            UIImage *image = [UIImage imageNamed:@"[sysChat].png"];
            if (!image) continue;
            emoText = [NSAttributedString attachmentStringWithEmojiImage:image fontSize:cellConfig.contentsTextSize needOffset:0 andImageFrame:CGRectMake(-2,-4, 40, 20)];

        }else{
            YYImage *image = [YYImage imageNamed:@"xxx.gif"];
            // image.preloadAllAnimatedImageFrames = YES;
            if (!image) continue;
            emoText = [NSAttributedString attachmentStringWithEmojiImage:image fontSize:cellConfig.contentsTextSize needOffset:0 andImageFrame:CGRectMake(1,-4, 20, 20)];

        }
        [text replaceCharactersInRange:range withAttributedString:emoText];
        emoClipLength += range.length - 1;
    }
    NSMutableParagraphStyle * paragraphStyle = [[NSMutableParagraphStyle alloc] init];
    [paragraphStyle setLineSpacing:3];
    [paragraphStyle setLineBreakMode:NSLineBreakByWordWrapping];
    [paragraphStyle setMaximumLineHeight:MAXFLOAT];
    [text addAttribute:NSParagraphStyleAttributeName value:paragraphStyle range:NSMakeRange(0, [text length])];
    
    
    return text;

}
-(void)calculateWithTextBubbleView{
    _showContentsString = _message.msg;
    
    
    NSMutableAttributedString *text = [NSMutableAttributedString new];
    [text appendAttributedString:[[NSMutableAttributedString alloc] initWithString:_showContentsString attributes:nil]];
    text.font=[UIFont systemFontOfSize:16];
    _showAttContentsString=[self returnText:text];
    _showContentsSize=[self needH:_showAttContentsString];
    
//    _showContentsSize = [_showContentsString sizeWithFont:[UIFont systemFontOfSize:14.0f] maxSize:CGSizeMake(kMainScreenWidth *0.6, MAXFLOAT)];
    
    
    
    _showTranslatExplainSize =[_translatExplainString sizeWithFont:cellConfig.translatExplainFont maxSize:CGSizeMake(cellConfig.bubbleMaxWidth, MAXFLOAT)];
    CGFloat tempWidth = 0;
    CGFloat tempHeight = 0;
    tempHeight = _showContentsSize.height + _showTranslatExplainSize.height ;
    if (_showContentsSize.width > _showTranslatExplainSize.width) {
        tempWidth = _showContentsSize.width;
    }else{
        tempWidth = _showTranslatExplainSize.width ;
    }
    _textBubbleSize =  CGSizeMake(tempWidth, tempHeight);
    _textBubbleBackImageSize = CGSizeMake(tempWidth +cellConfig.leftBubbleMargin.left + cellConfig.leftBubbleMargin.right,tempHeight + cellConfig.leftBubbleMargin.top+cellConfig.leftBubbleMargin.bottom);
}

#pragma mark -
#pragma mark getter
-(NSString *)messageId{
    return _message.mailId;
}
-(long)sequenceId{
    return _message.sequenceId;
}
-(CSMessageSendState)messageSendStatus{
    return (CSMessageSendState)_message.sendState;
}
-(CSMessageBodyType)messageBodyType{
    return [self gettingMsgBodyTypeWithMsgPost:_message.post];
}

-(CGFloat)cellHeight{
    CGFloat height = 0;
    if (_isNickNameShow == YES) {
        height = height + cellConfig.avatarMargin;//头像上边距
        height = height + cellConfig.messageNameHeight;
        height = height + _textBubbleBackImageSize.height;
          height = height + cellConfig.avatarMargin;//气泡下边距

    }else{
        height = height + cellConfig.avatarMargin;
         height = height + _textBubbleBackImageSize.height;
        height = height + cellConfig.avatarMargin;
    }
    if (_isTimeLabelShow == YES) {
        height = height + cellConfig.timeLabelHeight;
        if (height < cellConfig.avatarMargin+ cellConfig.avatarSize + cellConfig.timeLabelHeight) {
            return cellConfig.avatarMargin+ cellConfig.avatarSize + cellConfig.timeLabelHeight;
        }else{
            return height;
        }
    }else{
        if (height <cellConfig.avatarMargin+ cellConfig.avatarSize ) {
            return cellConfig.avatarMargin+ cellConfig.avatarSize ;
        }else{
            return height;
        }
    }

}

-(CSMessageBodyType )gettingMsgBodyTypeWithMsgPost:(int )vPost{
    if (vPost == 100) {
        return CSMessageBodyType_Text_ChatRoomSystemMsg;
    }else if (vPost == 200){
        return CSMessageBodyType_Text_ModMsg;
    }else if ( vPost == 0){
        return CSMessageBodyType_Text_Normal;
    }else if  (vPost == 1){
        return CSMessageBodyType_Text_System;
    }else if (vPost == 4){
        return CSMessageBodyType_Text_FightReport;
    }else if (vPost == 5){
        return CSMessageBodyType_Text_InvestigateReport;
    }else if ( vPost == 6){
        return CSMessageBodyType_Text_Loudspeaker;
    }else if (vPost == 7){
        return CSMessageBodyType_Text_EquipShare;
    }else if (vPost == 8){
        return CSMessageBodyType_Text_SayHello;
    }else if (vPost == 9){
        return CSMessageBodyType_Text_AllianceConciles;
    }else if (vPost == 10){
        return CSMessageBodyType_Text_TurntableShare;
    }else if (vPost == 11){
        return CSMessageBodyType_Text_AllianceTask;
    }else if (vPost == 12){
        return CSMessageBodyType_RedPackge;
    }else {
        return CSMessageBodyType_NotCanParse;
    }
}












@end
