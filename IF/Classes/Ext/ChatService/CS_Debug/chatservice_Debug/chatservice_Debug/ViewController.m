//
//  ViewController.m
//  chatservice_Debug
//
//  Created by 张彦涛 on 15/11/30.
//  Copyright © 2015年 张彦涛. All rights reserved.
//

#import "ViewController.h"
#import "CSChatingVC.h"
@interface ViewController ()<UITableViewDataSource,UITableViewDelegate>
@property (nonatomic,strong)NSMutableArray *dataSourceArray;
@property (weak, nonatomic) IBOutlet UITableView *tableView;


@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.title = @"功能列表";
    _dataSourceArray = [NSMutableArray arrayWithArray:@[@"国家联盟视图"]];
    [_tableView reloadData];
}



- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return [_dataSourceArray count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    NSString *identifyString =@"cellIdentify";
    
   UITableViewCell *cell =  [tableView dequeueReusableCellWithIdentifier:identifyString];
    if (cell == nil) {
        cell =[[UITableViewCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:identifyString];
    }
    cell.textLabel.text = _dataSourceArray[indexPath.row];
  
    return cell;
}


-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    if(indexPath.row == 0){
        UIStoryboard *mainStoryBoard=[UIStoryboard storyboardWithName:@"Main" bundle:[NSBundle mainBundle]];
        CSChatingVC *chattingVC =[mainStoryBoard instantiateViewControllerWithIdentifier:@"ChattingVC"];
        [self.navigationController pushViewController:chattingVC animated:YES];
    }else{
        
    }

}

@end
