//
//  RoleInfoViewOC.m
//  IF
//
//  Created by liuzengyou on 15/6/16.
//
//

#import "RoleInfoViewOC.h"

@interface RoleInfoViewOC ()
{
    UIView *topView;
    UIImageView *topBackImageView;
   UIImageView *_huitiaoImage;
}
@end

@implementation RoleInfoViewOC

-(instancetype)init{
    self = [super init];
    if (self) {
//        [self addTop];
    }
    return self;
}


+(instancetype)showPlayerInfo
{
   RoleInfoViewOC *_RoleInfoView =  [[[NSBundle mainBundle ]loadNibNamed:@"RoleInfoViewOC" owner:nil options:nil] lastObject];

    return _RoleInfoView;
}


 

- (void)dealloc {
    [_m_achieveTTF release];
    [_m_achieveTTF release];
    [_m_medalTTF release];
    [_m_powerTxt release];
    [_m_killTxt release];
    [_m_headImg release];
    [_m_levelTxt release];
    [_m_serverTxt release];
    [_m_nameTxt release];
    [super dealloc];
}
@end
