//
//  RoleInfoItemTitleOC.m
//  IF
//
//  Created by liuzengyou on 15/6/16.
//
//

#import "RoleInfoItemTitleOC.h"

@interface RoleInfoItemTitleOC ()
{

}
@end

@implementation RoleInfoItemTitleOC

-(instancetype)init{
    self = [super init];
    if (self) {
//        [self addTop];
    }
    return self;
}


+(instancetype)showPlayerInfoDetailItemTitle
{
   RoleInfoItemTitleOC *_RoleInfoItemOC =  [[[NSBundle mainBundle ]loadNibNamed:@"RoleInfoItemTitle" owner:nil options:nil] lastObject];

    return _RoleInfoItemOC;
}


 

- (void)dealloc {


    [_m_title release];
    [_m_titleBG release];
    [_m_title release];
    [super dealloc];
}
@end
