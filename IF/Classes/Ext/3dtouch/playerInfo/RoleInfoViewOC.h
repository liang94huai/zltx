//
//  RoleInfoView.h
//  IF
//
//  Created by liuzengyou on 15/6/16.
//
//

#import <UIKit/UIKit.h>

@interface RoleInfoViewOC : UIView


@property (retain, nonatomic) IBOutlet UILabel *m_achieveTTF;
@property (retain, nonatomic) IBOutlet UILabel *m_medalTTF;
@property (retain, nonatomic) IBOutlet UILabel *m_powerTxt;
@property (retain, nonatomic) IBOutlet UILabel *m_killTxt;
@property (retain, nonatomic) IBOutlet UIImageView *m_headImg;
@property (retain, nonatomic) IBOutlet UILabel *m_levelTxt;
@property (retain, nonatomic) IBOutlet UILabel *m_serverTxt;
@property (retain, nonatomic) IBOutlet UILabel *m_nameTxt;

@property (nonatomic,strong) UILabel *titleName;

+(instancetype)showPlayerInfo;

@end
