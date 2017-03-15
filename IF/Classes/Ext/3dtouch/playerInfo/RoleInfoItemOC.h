//
//  RoleInfoItemOC.h
//  IF
//
//  Created by liuzengyou on 15/6/16.
//
//

#import <UIKit/UIKit.h>

@interface RoleInfoItemOC : UIView


@property (retain, nonatomic) IBOutlet UIImageView *m_bg1;
@property (retain, nonatomic) IBOutlet UILabel *m_nameTxt;
@property (retain, nonatomic) IBOutlet UILabel *m_valueTxt;
@property (retain, nonatomic) IBOutlet UIImageView *m_bg0;

+(instancetype)showPlayerInfoDetailItem;

@end
