//
//  ReplayPreviewDelegate.h
//  IF
//
//  Created by liruiqi on 15/9/30.
//
//

#ifndef ReplayPreviewDelegate_h
#define ReplayPreviewDelegate_h

#include <ReplayKit/RPPreviewViewController.h>

@interface ReplayPreviewDelegate : NSObject <RPPreviewViewControllerDelegate> {

};
+ (id) sharedReplayDispather;
@end

#endif /* ReplayPreviewDelegate_h */
