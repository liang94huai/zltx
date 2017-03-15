//
//  CSChatingVC.m
//  chatservice_Debug
//
//  Created by 张彦涛 on 15/12/7.
//  Copyright © 2015年 张彦涛. All rights reserved.
//

#import "CSChatingVC.h"
#import "CSMessageModel.h"
#import "CSMessage.h"
#import "CSMessageTableCell.h"
@interface CSChatingVC ()<UITableViewDataSource,UITableViewDelegate,UIScrollViewDelegate,CSMsgTableCellDelegate>
{
        UILongPressGestureRecognizer *_lpgr;
    UIMenuItem *_copyMenuItem;
    UIMenuItem *_deleteMenuItem;
}
@property (nonatomic,strong)IBOutlet UITableView *tableView;
@property (nonatomic,strong)NSMutableArray *dataSourceArray ;

 @property (strong, nonatomic) NSIndexPath *menuIndexPath;

@property (strong, nonatomic) UIMenuController *menuController;
@end

@implementation CSChatingVC

- (void)viewDidLoad {
    [super viewDidLoad];
    self.title = @"cell列表";
    _dataSourceArray =[NSMutableArray array];
    _tableView.backgroundView =[[UIImageView alloc]initWithImage:[UIImage imageNamed:@"csChatTableBackImage"]];

    for (int x = 0; x<10; x++) {
          [self test];
    }
    _lpgr = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(handleLongPress:)];
    _lpgr.minimumPressDuration = 0.5;
    [self.tableView addGestureRecognizer:_lpgr];

    _tableView.dataSource = self;
    _tableView.delegate = self;
    [_tableView reloadData];

}

-(void)test{

    CSMessage  *msg = [[CSMessage alloc]init];
    msg.msg = @"123jklasdjf;alsfjsaldwww.baidu.comfj4123124";
    msg.sendState = CSMessageSendState_Delivering;
    CSMessageModel *msgModel = [[CSMessageModel alloc]initWithMessage:msg];
    msgModel.nickname = @"习惯";
    msgModel.isNickNameShow = YES;
    msgModel.isSelfSender = YES;
    msgModel.translatExplainString = @"由英文翻译";
    msgModel.avatarDefaultImageName = @"g015";

  

    [msgModel calculateWithTextBubbleView];
    [_dataSourceArray addObject:msgModel];
    
    

    CSMessage  *msg1 = [[CSMessage alloc]init];
    msg1.msg = @"sdf111:222aa[篮球]sfi10010ajsdlfj[篮球][篮球][左哼哼]askfjas[2]kdfjaksldjfsdfas";
    CSMessageModel *msgModel1 = [[CSMessageModel alloc]initWithMessage:msg1];
    msgModel1.nickname = @"守候";
    msgModel1.translatExplainString = @"原文";
    msgModel1.isNickNameShow = YES;
    msgModel1.avatarDefaultImageName = @"g012";
    msgModel1.isSelfSender = NO;
    msgModel1.isTimeLabelShow = NO;
    [msgModel1 calculateWithTextBubbleView];
    [_dataSourceArray addObject:msgModel1];

    
    
    _tableView.dataSource = self;
    _tableView.delegate = self;
    [_tableView reloadData];


    
    CSMessage  *msg2 = [[CSMessage alloc]init];
    msg2.msg = @"晕";
    CSMessageModel *msgModel2 = [[CSMessageModel alloc]initWithMessage:msg2];
    msgModel2.nickname = @"守候1sdf";
    msgModel2.translatExplainString = @"由中文翻译";
    msgModel2.isNickNameShow = NO;
    msgModel2.avatarDefaultImageName = @"g015";
    msgModel2.isSelfSender = NO;
      msgModel2.isTimeLabelShow = YES;
    [msgModel2 calculateWithTextBubbleView];
    [_dataSourceArray addObject:msgModel2];
    
    CSMessage  *msg3 = [[CSMessage alloc]init];
    msg3.msg = @"聊天测[1]试聊www.baidu.com天测底聊天测底聊天测[1]底聊天测底聊天测底聊[1]天测底聊天测底聊天测底聊天测底聊天测底[1]测底[大哭]聊天测底聊天测底测底[大哭]聊天测https://www.baidu.com底聊天测底测底[大哭]聊天测底聊天测底测底[大哭]聊天测底聊天测底测底[1][大哭]聊天测底聊天测底聊http://detail.zol.com.cn/cell_phone/index403798.shtml天测底聊天测底聊天测底[大哭]聊天测底聊天测底聊天测底聊天测底12345";
    CSMessageModel *msgModel3 = [[CSMessageModel alloc]initWithMessage:msg3];
    msgModel3.nickname = @"守候1sdf";
    msgModel3.translatExplainString = @"由中文翻译";
    msgModel3.isNickNameShow = YES;
    msgModel3.avatarDefaultImageName = @"g012";
    msgModel3.isSelfSender = NO;
    msgModel3.isTimeLabelShow = NO;
    [msgModel3 calculateWithTextBubbleView];
    [_dataSourceArray addObject:msgModel3];
    
    CSMessage  *msg4 = [[CSMessage alloc]init];
    msg4.sendState = CSMessageSendState_Failure;
    msg4.msg = @"聊天测试聊天asljfda;lskdfja;我们slkdjf;klasdjflkasdjfl;asdjflasdjflkasj";
    CSMessageModel *msgModel4 = [[CSMessageModel alloc]initWithMessage:msg4];
    msgModel4.nickname = @"守wuwuwu";
    msgModel4.translatExplainString = @"由中文翻译";
    msgModel4.isNickNameShow = YES;
    msgModel4.avatarDefaultImageName = @"g015";
    msgModel4.isSelfSender = YES;
    [msgModel4 calculateWithTextBubbleView];
    [_dataSourceArray addObject:msgModel4];

    
    CSMessage  *msg5 = [[CSMessage alloc]init];
    msg5.sendState = CSMessageSendState_Failure;
    msg5.msg = @"聊天测试聊天33:66asljfda;lskdfja;我们s777：324lkdjf;klasdjflkasdjfl;asdjflasdjflkasj";
    CSMessageModel *msgModel5 = [[CSMessageModel alloc]initWithMessage:msg5];
    msgModel5.nickname = @"守wuwuwu";
    msgModel5.translatExplainString = @"由中文翻译";
    msgModel5.isNickNameShow = YES;
    msgModel5.avatarDefaultImageName = @"g015";
    msgModel5.isSelfSender = NO;
    [msgModel5 calculateWithTextBubbleView];
    [_dataSourceArray addObject:msgModel5];
}

-(void)resumeOldLongPress{
    CSMessageTableCell *tempCell =[self.tableView cellForRowAtIndexPath:_menuIndexPath];
    if (tempCell) {
        [tempCell longRecognizerResumeNormal];
    }

}

- (void)handleLongPress:(UILongPressGestureRecognizer *)recognizer
{
    if (recognizer.state == UIGestureRecognizerStateBegan && [self.dataSourceArray count] > 0)
    {
        //去掉之前的选中
        [self resumeOldLongPress];
               CGPoint location = [recognizer locationInView:self.tableView];
        NSIndexPath * indexPath = [self.tableView indexPathForRowAtPoint:location];
 
     
            id object = [self.dataSourceArray objectAtIndex:indexPath.row];
            if (![object isKindOfClass:[NSString class]]) {
                CSMessageTableCell *cell = (CSMessageTableCell *)[self.tableView cellForRowAtIndexPath:indexPath];
                [cell becomeFirstResponder];
                [cell longRecognizerPresed];
                _menuIndexPath = indexPath;
                [self _showMenuViewController:cell.bubbleView  andIndexPath:indexPath messageType:cell.messageModel.messageBodyType];
            }
    }
}

- (void)cell:(UITableViewCell *)cell didClickInLabel:(YYLabel *)label textRange:(NSRange)textRang{

//    if (info[kWBLinkURLName]) {
//        WBURL *url = info[kWBLinkURLName];
//        WBPicture *pic = url.pics.firstObject;
//        
//    }

}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return _dataSourceArray.count;
}
-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
   static  NSString *identify = @"cell";

    CSMessageTableCell *cell = [tableView dequeueReusableCellWithIdentifier:identify];
    CSMessageModel *model =[_dataSourceArray objectAtIndex:indexPath.row];
    if (cell == nil) {
        cell =[[CSMessageTableCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:identify model:model];
        cell.msgTableCellDelegate = self;
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
    }
    cell.messageModel = model;
    return cell;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    CGFloat height =[(CSMessageModel *)_dataSourceArray[indexPath.row] cellHeight];
    return height;
    
//    return 120;
}

- (void)scrollViewDidScroll:(UIScrollView *)scrollView{
    if (scrollView == self.tableView) {
        [self.menuController setMenuVisible:NO animated:YES];
        [self  resumeOldLongPress];
    }
}

#pragma mark -
#pragma mark menu Item 弹出
- (void)_showMenuViewController:(UIView *)showInView
                   andIndexPath:(NSIndexPath *)indexPath
                    messageType:(CSMessageBodyType)messageType
{
    if (self.menuController == nil) {
        self.menuController = [UIMenuController sharedMenuController];
    }

    if (_deleteMenuItem == nil) {
        _deleteMenuItem = [[UIMenuItem alloc] initWithTitle:@"删除" action:@selector(deleteMenuAction:)];
    }
    
    if (_copyMenuItem == nil) {
        _copyMenuItem = [[UIMenuItem alloc] initWithTitle:@"复制" action:@selector(copyMenuAction:)];
    }
//
//    if (_transpondMenuItem == nil) {
//        _transpondMenuItem = [[UIMenuItem alloc] initWithTitle:NSLocalizedString(@"transpond", @"Transpond") action:@selector(transpondMenuAction:)];
//    }
//    
//    if (messageType == CSMessageBodyType_Text) {
        [self.menuController setMenuItems:@[_copyMenuItem, _deleteMenuItem]];
//    }
    [self.menuController setTargetRect:showInView.frame inView:showInView.superview];
    [self.menuController setMenuVisible:YES animated:YES];
}

-(void)deleteMenuAction:(UIMenuItem *)sender{
    [self resumeOldLongPress];
    NSLog(@"删除");
}
-(void)copyMenuAction:(UIMenuItem *)sender{
    [self resumeOldLongPress];
    NSLog(@"copy");
}
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
     
}


- (BOOL)canBecomeFirstResponder{
    return YES;
}

-(BOOL)canPerformAction:(SEL)action withSender:(id)sender{
    if (action == @selector(deleteMenuAction:) || action == @selector(copyMenuAction:)) {
        return YES;
    }
    return [super canPerformAction:action withSender:sender];
}


#pragma mark -
#pragma mark CSMsgTableCellDelegate

-(void)msgCellAvatarViewSelected:(CSBaseMessageCell *)vCell{
    NSLog(@"avatar 点击");
}
-(void)msgcellResendButtonSelected:(CSBaseMessageCell *)vCell{
    NSLog(@"重发点击");
}
-(void)msgcellBubbleViewSelected:(CSBaseMessageCell *)vCell{
    NSLog(@"bubble 点击");
}
@end
