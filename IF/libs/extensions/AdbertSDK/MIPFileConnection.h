//
//  MIPFileConnection.h
//
//  Created by Dennies Chang on 2011/6/23.
//  Copyright 2011年 Move In Pocket Co., Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

#define showMIPFileConnectionLog NO

@protocol MIPFileConnectionDelegate;

@interface MIPFileConnection : NSURLConnection {
    NSString *htusername, *htpassword;
    
    BOOL bFinished;
    BOOL bReplaceExistingFile;
    BOOL bReplaceExistingFileWithDifferentSize;
    BOOL bTimeout;
    BOOL bKeepCount;
    BOOL bProcessDataByDelegate;

    NSMutableURLRequest *_request;    
    NSOutputStream *fileStream;
    unsigned long long initPos;
    unsigned long long totalFileLength;
    unsigned long long currentDataLength;
    int iReloadCountDown;
    int tag;
}

@property (nonatomic, assign)BOOL bProcessDataByDelegate;
@property (nonatomic, strong)NSString *targetURL;
@property (nonatomic, strong)NSString *localSavePath;
@property (nonatomic, assign)BOOL bReplaceExistingFile;
@property (nonatomic, assign)BOOL bReplaceExistingFileWithDifferentSize;
@property __weak id <MIPFileConnectionDelegate> delegate;
@property (nonatomic, assign)unsigned long long totalFileLength;
@property (nonatomic, assign)unsigned long long initPos;
@property (nonatomic, assign)int tag;
@property (nonatomic, strong)NSMutableData *receiveData;
@property (nonatomic, assign)NSDate *notBeforeDate;
@property (assign)BOOL bReplaceFileWithNewerDate;
@property (nonatomic, readonly)NSString *lastModifiedDate;
@property (nonatomic, readonly)NSInteger statusCode;

- (id)initWithRequest:(NSURLRequest *)request delegate:(id)realDelegate;
- (id)initWithTargetURL:(NSString *)pTargetURL;
- (id)initWithTargetURL:(NSString *)pTargetURL useCache:(BOOL)bCache;
- (id)initWithTargetURL:(NSString *)pTargetURL saveToFile:(NSString *)pLocalSavePath;
- (id)initWithTargetURL:(NSString *)pTargetURL saveToFile:(NSString *)pLocalSavePath useCache:(BOOL)bCache;

- (void)setHTTPChallengeUserName:(NSString *)username andPassword:(NSString *)password;
- (void)start;
- (void)stopAsPause;

@end



@protocol MIPFileConnectionDelegate <NSObject>

@optional
- (void)connection:(MIPFileConnection *)connection didFailWithError:(NSError *)error;
- (void)connectionDidFinishLoading:(MIPFileConnection *)connection;
- (void)connectionCancelledByUser:(MIPFileConnection *)connection;
- (void)connection:(MIPFileConnection *)connection updateDataProgressing:(unsigned long long)dataPosition;
- (void)connection:(MIPFileConnection *)connection didReceiveData:(NSData *)data;

@end