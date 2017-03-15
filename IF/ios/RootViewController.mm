//
//  IFAppController.h
//  IF
//
//  Created by 吴 凌江 on 13-8-28.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//

#import "RootViewController.h"
#import "PreviewViewController.h"
#import "Reachability.h"

#include "../Classes/model/GlobalData.h"
#include "../Classes/Ext/3dtouch/OCCppHelper.h"
#include "../Classes/Ext/CCSafeNotificationCenter.h"

@interface RootViewController ()
{

}
@property (nonatomic,strong) Reachability * reachability;

@end

@implementation RootViewController

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

/*

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
    
}
 */

-(void)initViewEvent
{
    [self register3DTouch];
    [self checkReachability];
}

-(void)register3DTouch{
    if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 9.0)
    {
        if( self.traitCollection.forceTouchCapability == UIForceTouchCapabilityAvailable)
        {
            NSLog(@"你的手机支持3D Touch!");
            
            [self registerForPreviewingWithDelegate:self sourceView:self.view];
        }
        else
        {
            NSLog(@"你的手机暂不支持3D Touch!");
        }
    }
}

- (nullable UIViewController *)previewingContext:(id <UIViewControllerPreviewing>)previewingContext viewControllerForLocation:(CGPoint)point
{
    if(!GlobalData::shared()->peek_switch)
    {
        return nil;
    }
    
    if (GlobalData::shared()->peekPageType == peekType_playerInfo)
    {
        GlobalData::shared()->peekPageType = peekType_peek;
        
        previewingContext.sourceRect = CGRectMake(0, 0, previewingContext.sourceView.frame.size.width, previewingContext.sourceView.frame.size.height);
    
        UIViewController *childVC = [[PreviewViewController alloc] init] ;
        childVC.preferredContentSize = CGSizeMake(0.0f,450.f);
        childVC.view.backgroundColor = [UIColor orangeColor];
        
//        OCCppHelper::share()->notificationIOSUpdatePlayerInfo();
        
        return childVC;
    }

    

    return nil;
}


- (void)previewingContext:(id <UIViewControllerPreviewing>)previewingContext commitViewController:(UIViewController *)viewControllerToCommit
{
    
//    [self presentViewController:viewControllerToCommit animated:YES completion:nil];
    
    [self showViewController:viewControllerToCommit sender:self];
}

// Override to allow orientations other than the default portrait orientation.
// This method is deprecated on ios6
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return UIInterfaceOrientationIsLandscape( interfaceOrientation );
}

#pragma mark
#pragma mark Reachability Methods
#pragma mark
- (void)checkReachability
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(reachabilityChanged:) name:kReachabilityChangedNotification object:nil];
    self.reachability = [Reachability reachabilityForInternetConnection];
    [self.reachability startNotifier];
    [self updateInterfaceWithReachability:self.reachability];
}

/*!
 * Called by Reachability whenever status changes.
 */
- (void) reachabilityChanged:(NSNotification *)note
{
    Reachability* curReach = [note object];
    NSParameterAssert([curReach isKindOfClass:[Reachability class]]);
    [self updateInterfaceWithReachability:curReach];
}

- (void)updateInterfaceWithReachability:(Reachability *)reachability
{
    NetworkStatus status = [reachability currentReachabilityStatus];
    
    if(status == NotReachable)
    {
        //No internet
        NSLog(@"No Internet");
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("NetworkAvailable", CCBool::create(false));
    }
    else if (status == ReachableViaWiFi)
    {
        //WiFi
        NSLog(@"Reachable WIFI");
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("NetworkAvailable", CCBool::create(true));
    }
    else if (status == ReachableViaWWAN)
    {
        //3G
        NSLog(@"Reachable 3G");
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("NetworkAvailable", CCBool::create(true));
    }
}

// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (NSUInteger) supportedInterfaceOrientations{
#ifdef __IPHONE_6_0
    return UIInterfaceOrientationMaskPortrait;
#endif
}

- (BOOL) shouldAutorotate {
    return YES;
}

//fix not hide status on ios7
- (BOOL)prefersStatusBarHidden
{
    return YES;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}


@end
