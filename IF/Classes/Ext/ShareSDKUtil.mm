//
//  SNSUtil.m
//  IF
//
//  Created by 刘增友 on 15/9/14.
//
//

#include <string>
#include "ShareSDKUtil.h"
//#include "AppController.h"
//#include "CCSafeNotificationCenter.h"
//#include "TypeDefinition.h"

//#import <ShareSDK/ShareSDK.h>
#import <ShareSDKExtension/SSEShareHelper.h>
#import <ShareSDKUI/ShareSDK+SSUI.h>
#import <ShareSDKUI/SSUIShareActionSheetStyle.h>
#import <ShareSDKUI/SSUIShareActionSheetCustomItem.h>
#import <ShareSDK/ShareSDK+Base.h>
#import <UIKit/UIKit.h>

//＝＝＝＝＝＝＝＝＝＝ShareSDK头文件＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
#import <ShareSDK/ShareSDK.h>
#import <ShareSDKConnector/ShareSDKConnector.h>
//以下是ShareSDK必须添加的依赖库：
//1、libicucore.dylib
//2、libz.dylib
//3、libstdc++.dylib
//4、JavaScriptCore.framework

//＝＝＝＝＝＝＝＝＝＝以下是各个平台SDK的头文件，根据需要继承的平台添加＝＝＝
//腾讯开放平台（对应QQ和QQ空间）SDK头文件
#import <TencentOpenAPI/TencentOAuth.h>
#import <TencentOpenAPI/QQApiInterface.h>
//以下是腾讯SDK的依赖库：
//libsqlite3.dylib

//微信SDK头文件
#import "WXApi.h"
//以下是微信SDK的依赖库：
//libsqlite3.dylib

//新浪微博SDK头文件
#import "WeiboSDK.h"
//新浪微博SDK需要在项目Build Settings中的Other Linker Flags添加"-ObjC"
//以下是新郎微博SDK的依赖库：
//ImageIO.framework

//人人SDK头文件
//#import <RennSDK/RennSDK.h>

//GooglePlus SDK头文件
//#import <GooglePlus/GooglePlus.h>
//以下是GooglePlus SDK的依赖库
//1、CoreMotion.framework
//2、CoreLocation.framework
//3、MediaPlayer.framework
//4、AssetsLibrary.framework
//5、AddressBook.framework

@interface ShareSDKIOS:UIViewController
{
    
}

/**
 *  面板
 */
@property (nonatomic, strong) UIView *panelView;

/**
 *  加载视图
 */
@property (nonatomic, strong) UIActivityIndicatorView *loadingView;

@property (nonatomic,strong) UIWindow *vc_win;

+(ShareSDKIOS*)shared;

@end


USING_NS_CC;


@implementation ShareSDKIOS

static ShareSDKIOS *_sharedSDKUtile;
+(ShareSDKIOS*)shared {
    if(_sharedSDKUtile == nil) {
        _sharedSDKUtile = [[ShareSDKIOS alloc] init];
    }
    return _sharedSDKUtile;
}

- (id)init {
    
    if (self.vc_win == nil)
    {
        UIWindow *window=[[UIWindow alloc]initWithFrame:[UIScreen mainScreen].bounds];
        self.vc_win = window;
        [self.vc_win addSubview:self.view];
    }
    
    
    //加载等待视图
    self.panelView = [[UIView alloc] initWithFrame:self.view.bounds];
    self.panelView.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
    self.panelView.backgroundColor = [UIColor colorWithWhite:0 alpha:0.3];
    
    self.loadingView = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
    self.loadingView.frame = CGRectMake((self.view.frame.size.width - self.loadingView.frame.size.width) / 2, (self.view.frame.size.height - self.loadingView.frame.size.height) / 2, self.loadingView.frame.size.width, self.loadingView.frame.size.height);
    self.loadingView.autoresizingMask = UIViewAutoresizingFlexibleBottomMargin | UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleRightMargin | UIViewAutoresizingFlexibleTopMargin;
    [self.panelView addSubview:self.loadingView];
    
    [self initSharedSDK];

    return self;
}

/**
 *  显示加载动画
 *
 *  @param flag YES 显示，NO 不显示
 */
- (void)showLoadingView:(BOOL)flag
{
    if (flag)
    {
        [self.view addSubview:self.panelView];
        [self.loadingView startAnimating];
    }
    else
    {
        [self.panelView removeFromSuperview];
    }
}

-(void)initSharedSDK
{
    //    PrintTest* _p = new PrintTest();
    //    _p->p
    /**
     *  设置ShareSDK的appKey，如果尚未在ShareSDK官网注册过App，请移步到http://mob.com/login 登录后台进行应用注册，
     *  在将生成的AppKey传入到此方法中。
     *  方法中的第二个参数用于指定要使用哪些社交平台，以数组形式传入。第三个参数为需要连接社交平台SDK时触发，
     *  在此事件中写入连接代码。第四个参数则为配置本地社交平台时触发，根据返回的平台类型来配置平台信息。
     *  如果您使用的时服务端托管平台信息时，第二、四项参数可以传入nil，第三项参数则根据服务端托管平台来决定要连接的社交SDK。
     */
    
    
    [ShareSDK registerApp:@"a62716a0883e"
          activePlatforms:@[
                            @(SSDKPlatformTypeSinaWeibo),
//                            @(SSDKPlatformTypeTencentWeibo),
                            @(SSDKPlatformTypeMail),
                            @(SSDKPlatformTypeSMS),
                            @(SSDKPlatformTypeFacebook),
                            @(SSDKPlatformTypeTwitter),
                            @(SSDKPlatformTypeWechat),
                            @(SSDKPlatformTypeQQ),
                            ]
                 onImport:^(SSDKPlatformType platformType) {
                     
                     switch (platformType)
                     {
                         case SSDKPlatformTypeWechat:
                             [ShareSDKConnector connectWeChat:[WXApi class]];
                             break;
                         case SSDKPlatformTypeQQ:
                             [ShareSDKConnector connectQQ:[QQApiInterface class] tencentOAuthClass:[TencentOAuth class]];
                             break;
                         case SSDKPlatformTypeSinaWeibo:
                             [ShareSDKConnector connectWeibo:[WeiboSDK class]];
                             break;
                         default:
                             break;
                     }
                     
                 }
          onConfiguration:^(SSDKPlatformType platformType, NSMutableDictionary *appInfo) {
              
              switch (platformType)
              {
                  case SSDKPlatformTypeSinaWeibo:
                      //设置新浪微博应用信息,其中authType设置为使用SSO＋Web形式授权
                      [appInfo SSDKSetupSinaWeiboByAppKey:@"2857911180"
                                                appSecret:@"91b9e1568de14cc6edd528d13ccc9f6c"
                                              redirectUri:@"https://api.weibo.com/oauth2/default.html"
                                                 authType:SSDKAuthTypeBoth];
                      break;
//                  case SSDKPlatformTypeTencentWeibo:
//                      //设置腾讯微博应用信息，其中authType设置为只用Web形式授权
//                      [appInfo SSDKSetupTencentWeiboByAppKey:@"801307650"
//                                                   appSecret:@"ae36f4ee3946e1cbb98d6965b0b2ff5c"
//                                                 redirectUri:@"http://www.sharesdk.cn"];
//                      break;
                  case SSDKPlatformTypeFacebook:
                      //设置Facebook应用信息，其中authType设置为只用SSO形式授权
                      [appInfo SSDKSetupFacebookByAppKey:@"1744111175850989"
                                               appSecret:@"4af43c09416ca70e5a663c181a9a7f9b"
                                                authType:SSDKAuthTypeBoth];
                      break;
                  case SSDKPlatformTypeTwitter:
                      [appInfo SSDKSetupTwitterByConsumerKey:@"vP5fz7Mp1mtDWXTz2n85UJdZM"
                                              consumerSecret:@"Ie12HgxRqrfzK9mO4iobAWgPPQgWbYoR9Ze0trp4deqZHlbV1L"
                                                 redirectUri:@"http://mob.com"];
                      break;
                  case SSDKPlatformTypeWechat:
                      [appInfo SSDKSetupWeChatByAppId:@"wx18e5b56e133c99a5"
                                            appSecret:@"413105ba07745f128991fd74730be7ce"];
                      break;
                  case SSDKPlatformTypeQQ:
                      [appInfo SSDKSetupQQByAppId:@"100371282"
                                           appKey:@"aed9b0303e3ed1e27bae87c33761161d"
                                         authType:SSDKAuthTypeBoth];
                      break;
                  default:
                      break;
              }
          }];
}

#pragma mark 显示分享菜单

/**
 *  显示分享菜单
 *
 *  @param view 容器视图
 */
- (void)showShareActionSheet:(NSString*) msg url:(NSString*)url
{
    /**
     * 在简单分享中，只要设置共有分享参数即可分享到任意的社交平台
     **/
//    __weak ShareSDKIOS *theController = self;
    
    //1、创建分享参数（必要）
    NSMutableDictionary *shareParams = [NSMutableDictionary dictionary];
    NSArray* imageArray = @[[UIImage imageNamed:@"shareImg.png"]];
    [shareParams SSDKSetupShareParamsByText:msg
                                     images:imageArray
                                        url:[NSURL URLWithString:url]
                                      title:@"分享标题"
                                       type:SSDKContentTypeAuto];
    
    //设置分享菜单栏样式（非必要）
    //        [SSUIShareActionSheetStyle setActionSheetBackgroundColor:[UIColor colorWithRed:249/255.0 green:0/255.0 blue:12/255.0 alpha:0.5]];
    //        [SSUIShareActionSheetStyle setActionSheetColor:[UIColor colorWithRed:21.0/255.0 green:21.0/255.0 blue:21.0/255.0 alpha:1.0]];
    //        [SSUIShareActionSheetStyle setCancelButtonBackgroundColor:[UIColor colorWithRed:21.0/255.0 green:21.0/255.0 blue:21.0/255.0 alpha:1.0]];
    //        [SSUIShareActionSheetStyle setCancelButtonLabelColor:[UIColor whiteColor]];
    //        [SSUIShareActionSheetStyle setItemNameColor:[UIColor whiteColor]];
    //        [SSUIShareActionSheetStyle setItemNameFont:[UIFont systemFontOfSize:10]];
    //        [SSUIShareActionSheetStyle setCurrentPageIndicatorTintColor:[UIColor colorWithRed:156/255.0 green:156/255.0 blue:156/255.0 alpha:1.0]];
    //        [SSUIShareActionSheetStyle setPageIndicatorTintColor:[UIColor colorWithRed:62/255.0 green:62/255.0 blue:62/255.0 alpha:1.0]];
    
    //2、分享
    [ShareSDK showShareActionSheet:self.view
                             items:[ShareSDK activePlatforms]
                       shareParams:shareParams
               onShareStateChanged:^(SSDKResponseState state, SSDKPlatformType platformType, NSDictionary *userData, SSDKContentEntity *contentEntity, NSError *error, BOOL end) {
                   
                   switch (state) {
                           
                       case SSDKResponseStateBegin:
                       {
//                           [theController showLoadingView:YES];
                           break;
                       }
                       case SSDKResponseStateSuccess:
                       {
                           UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"分享成功"
                                                                               message:nil
                                                                              delegate:nil
                                                                     cancelButtonTitle:@"确定"
                                                                     otherButtonTitles:nil];
                           [alertView show];
                           break;
                       }
                       case SSDKResponseStateFail:
                       {
                           if (platformType == SSDKPlatformTypeSMS && [error code] == 201)
                           {
                               UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"分享失败"
                                                                               message:@"失败原因可能是：1、短信应用没有设置帐号；2、设备不支持短信应用；3、短信应用在iOS 7以上才能发送带附件的短信。"
                                                                              delegate:nil
                                                                     cancelButtonTitle:@"OK"
                                                                     otherButtonTitles:nil, nil];
                               [alert show];
                               break;
                           }
                           else if(platformType == SSDKPlatformTypeMail && [error code] == 201)
                           {
                               UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"分享失败"
                                                                               message:@"失败原因可能是：1、邮件应用没有设置帐号；2、设备不支持邮件应用；"
                                                                              delegate:nil
                                                                     cancelButtonTitle:@"OK"
                                                                     otherButtonTitles:nil, nil];
                               [alert show];
                               break;
                           }
                           else
                           {
                               UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"分享失败"
                                                                               message:[NSString stringWithFormat:@"%@",error]
                                                                              delegate:nil
                                                                     cancelButtonTitle:@"OK"
                                                                     otherButtonTitles:nil, nil];
                               [alert show];
                               break;
                           }
                           break;
                       }
                       case SSDKResponseStateCancel:
                       {
                           UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"分享已取消"
                                                                               message:nil
                                                                              delegate:nil
                                                                     cancelButtonTitle:@"确定"
                                                                     otherButtonTitles:nil];
                           [alertView show];
                           break;
                       }
                       default:
                           break;
                   }
                   
                   if (state != SSDKResponseStateBegin)
                   {
//                       [theController showLoadingView:NO];
                   }
                   
               }];
}


#pragma mark 屏幕截图分享

/**
 *  屏幕截图分享
 */
- (void)screenShare
{
    /**
     * 使用ShareSDKExtension插件可以实现屏幕截图分享，对于原生界面和OpenGL的游戏界面同样适用
     * 通过使用SSEShareHelper可以调用屏幕截图分享方法，在方法的第一个参数里面可以取得截图图片和分享处理入口，只要构造分享内容后，将要分享的内容和平台传入分享处理入口即可。
     *
     * 小技巧：
     * 当取得屏幕截图后，如果shareHandler入口不满足分享需求（如截取屏幕后需要弹出分享菜单而不是直接分享），可以不调用shareHandler进行分享，而是在block中写入自定义的分享操作。
     * 这样的话截屏分享接口实质只充当获取屏幕截图的功能。
     **/
    
//    __weak ShareSDKIOS *theController = self;
//    [self showLoadingView:YES];
    
    [SSEShareHelper screenCaptureShare:^(SSDKImage *image, SSEShareHandler shareHandler) {
        
        if (!image)
        {
            //如果无法取得屏幕截图则使用默认图片
            image = [[SSDKImage alloc] initWithImage:[UIImage imageNamed:@"shareImg.png"] format:SSDKImageFormatJpeg settings:nil];
        }
        
        //构造分享参数
        NSMutableDictionary *shareParams = [NSMutableDictionary dictionary];
        [shareParams SSDKSetupShareParamsByText:@"使用ShareSDK分享，更便捷，更方便。"
                                         images:@[image]
                                            url:nil
                                          title:nil
                                           type:SSDKContentTypeImage];
        
        //回调分享
        if (shareHandler)
        {
            shareHandler (SSDKPlatformTypeSinaWeibo, shareParams);
        }
    }
                        onStateChanged:^(SSDKResponseState state, NSDictionary *userData, SSDKContentEntity *contentEntity, NSError *error) {
                            
                            switch (state) {
                                case SSDKResponseStateSuccess:
                                {
                                    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"分享成功"
                                                                                        message:nil
                                                                                       delegate:nil
                                                                              cancelButtonTitle:@"确定"
                                                                              otherButtonTitles:nil];
                                    [alertView show];
                                    break;
                                }
                                case SSDKResponseStateFail:
                                {
                                    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"分享失败"
                                                                                        message:[NSString stringWithFormat:@"%@", error]
                                                                                       delegate:nil
                                                                              cancelButtonTitle:@"确定"
                                                                              otherButtonTitles:nil];
                                    [alertView show];
                                    break;
                                }
                                case SSDKResponseStateCancel:
                                {
                                    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"分享已取消"
                                                                                        message:nil
                                                                                       delegate:nil
                                                                              cancelButtonTitle:@"确定"
                                                                              otherButtonTitles:nil];
                                    [alertView show];
                                    break;
                                }
                                default:
                                    break;
                            }
                            
//                            [self showLoadingView:NO];
                            
                        }];
}

@end


//  cpp
void ShareSDKUtil::screenShare()
{
    [[ShareSDKIOS shared] screenShare];
}

void ShareSDKUtil::initShareSDKUtil()
{
    [ShareSDKIOS shared];
}

void ShareSDKUtil::showShareActionSheet(const char* msg, const char* url)
{
    NSString* _msg = [NSString stringWithUTF8String:"分享内容"];
     NSString* _url = [NSString stringWithUTF8String:"http://mob.com"];
    
    [[ShareSDKIOS shared] showShareActionSheet:_msg url:_url];
}
