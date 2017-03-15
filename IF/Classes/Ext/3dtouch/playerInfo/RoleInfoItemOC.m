//
//  RoleInfoItemOC.m
//  IF
//
//  Created by liuzengyou on 15/6/16.
//
//

#import "RoleInfoItemOC.h"

@interface RoleInfoItemOC ()
{

}
@end

@implementation RoleInfoItemOC

-(instancetype)init{
    self = [super init];
    if (self) {
//        [self addTop];
    }
    return self;
}


+(instancetype)showPlayerInfoDetailItem
{
   RoleInfoItemOC *_RoleInfoItemOC =  [[[NSBundle mainBundle ]loadNibNamed:@"RoleInfoItem" owner:nil options:nil] lastObject];

    return _RoleInfoItemOC;
}


 

- (void)dealloc {

    [_m_bg1 release];
    [_m_nameTxt release];
    [_m_valueTxt release];
    [_m_bg0 release];
    [super dealloc];
}
@end
