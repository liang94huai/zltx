//
//  ReplayKitsController.m
//  IF
//
//  Created by liruiqi on 15/9/29.
//
//

#include "ReplayKitsController.hpp"
#include <ReplayKit/ReplayKit.h>
#include <ReplayKit/RPPreviewViewController.h>
#include "AppController.h"
#include "ReplayPreviewDelegate.h"
#include "UIComponent.h"

static ReplayKitsController *_sharedUtilies;
RPScreenRecorder *recorder;
RPPreviewViewController *previewController;

ReplayKitsController *ReplayKitsController::getInstance(){
    if(_sharedUtilies == NULL){
        _sharedUtilies = new ReplayKitsController();
        recorder = [RPScreenRecorder sharedRecorder];
    }
    return _sharedUtilies;
}

void ReplayKitsController::startRecord(){
    [recorder startRecordingWithMicrophoneEnabled:(true) handler:^(NSError * _Nullable error) {
        if(error == NULL){
            UIComponent::getInstance()->showRecordNode(true);
        }
    }];
//    [recorder startRecordingWithMicrophoneEnabled:(true) handler:NULL];
//    UIComponent::getInstance()->showRecordNode(true);
}

void ReplayKitsController::stopRecord(){
    [recorder stopRecordingWithHandler:^(RPPreviewViewController * _Nullable previewViewController, NSError * _Nullable error) {
        if (previewViewController != nil) {
            // Keep a reference for later use
            previewController = previewViewController;
            openReplay();
        }
        UIComponent::getInstance()->showRecordNode(false);
    }];
}

void ReplayKitsController::openReplay(){
    if (previewController != nil) {
        AppController* appDelegate = [UIApplication sharedApplication].delegate;
        previewController.previewControllerDelegate = [ReplayPreviewDelegate sharedReplayDispather];
//        if ( [previewController respondsToSelector:@selector(popoverPresentationController)] ) {
//            previewController.popoverPresentationController.sourceView = [appDelegate.viewController view];
//            [previewController setModalPresentationStyle:UIModalPresentationCurrentContext];
//            previewController.wantsFullScreenLayout = true;
//        }
        [appDelegate.viewController presentViewController:previewController animated:YES completion:nil];
    }
}

int ReplayKitsController::getStatus(){
    if(recorder.isRecording){
        return RECORDING;
    }
    return WAIT;
}

bool ReplayKitsController::isReplaySupport(){
//    std::string sDevPlatformInfo = std::string(SysUtilies::getDevicePlatformInfo());
//    if(sDevPlatformInfo == "iPhone 5s (GSM)"
//       || sDevPlatformInfo == "iPhone 5s (GSM+CDMA)"
//       || sDevPlatformInfo == "iPhone 6 Plus"
//       || sDevPlatformInfo == "iPhone 6"
//       || sDevPlatformInfo == "iPhone 6s Plus"
//       || sDevPlatformInfo == "iPhone 6s"
//       ){
//        return true;
//    }
//    
    return false;
}
