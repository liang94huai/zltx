//
//  RoleInfoItemTitleOC.h
//  IF
//
//  Created by liuzengyou on 15/6/16.
//
//

#import <UIKit/UIKit.h>

@interface RoleInfoItemTitleOC : UIView


@property (retain, nonatomic) IBOutlet UIImageView *m_titleBG;

@property (retain, nonatomic) IBOutlet UILabel *m_title;

+(instancetype)showPlayerInfoDetailItemTitle;

@end
