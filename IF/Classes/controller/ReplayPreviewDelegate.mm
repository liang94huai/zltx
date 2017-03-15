//
//  ReplayPreviewDelegate.m
//  IF
//
//  Created by liruiqi on 15/9/30.
//
//

#import "ReplayPreviewDelegate.h"
#import "AppController.h"

@implementation ReplayPreviewDelegate

static ReplayPreviewDelegate* s_preplayDispatcher;

+ (id) sharedReplayDispather
{
    if (s_preplayDispatcher == nil) {
        s_preplayDispatcher = [[self alloc] init];
    }
    
    return s_preplayDispatcher;
}

- (id) init
{
    if( (self = [super init]) ) {
    }
    return self;
}

- (void) dealloc
{
    s_preplayDispatcher = nullptr;
    [super dealloc];
}

- (void)previewControllerDidFinish:(RPPreviewViewController *)previewController{
    [previewController dismissViewControllerAnimated:(NO) completion:^{
        [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationNone];
    }];
    
}
@end

