//
//  IFAppController.h
//  IF
//
//  Created by on 13-8-28.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//
#import "WeiboSDK.h"
#import <AccessSDK/AccessAppDelegate.h>

@class RootViewController;

@interface AppController : AccessAppDelegate <UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate,UIApplicationDelegate> {
    UIWindow *window;
    RootViewController    *viewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) RootViewController *viewController;

@end

