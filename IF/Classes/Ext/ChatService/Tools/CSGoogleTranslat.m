//
//  CSGoogleTranslat.m
//  IF
//
//  Created by 张彦涛 on 15/7/2.
//
//

#import "CSGoogleTranslat.h"
#import "AFNetworking.h"
#import "JSONKit.h"
#import "NSString+Extension.h"
//转化成UTF - 8
#define UnicodeStr(URL)  [URL stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]

#define kTranslatedUrl @"http://translate.elexapp.com/translate2.php"
#define kKeyString @"jv89#klnme_*@"
#define kCH @"cok"
@interface CSGoogleTranslat ()
{
    NSString *transString;//翻译后的文字
    NSString *transLangType;//翻译的语言类型
}

@end
@implementation CSGoogleTranslat

  /** sc=要查询的内容  sf=源语言标记  tf=目标语言标记array类型， ch='cok'， t=当前时间戳 ， sig=md5（sc+sf+tf+ch+t+'jv89#klnme_*@'） */

-(void)translationingWithText:(NSString *)vText
                    andWihtSL:(NSString *)vSL
                    andWithTL:(NSString *)vTL
               andResultBlock:(GoogleTranslatBlock)vBlock{
    
    long  currentTime = [[NSDate date] timeIntervalSince1970];
    if (vSL.length == 0 ) {
        vSL = @"";
    }
    NSString *timeString = [NSString stringWithFormat:@"%ld",currentTime];
    NSString *tlJsonString = [@[vTL] JSONString];
    NSString *md5String =[NSString stringWithFormat:@"%@%@%@%@%@%@",vText,vSL,tlJsonString,kCH,timeString,kKeyString];
    NSString *md5Encryption = [md5String md5];
    md5Encryption = [md5Encryption lowercaseString];
    DVLog(@"md5String :%@ \n 加密后的串:%@",md5String,md5Encryption);
    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:vText,@"sc",
                          vSL,@"sf",
                          tlJsonString,@"tf",
                          kCH,@"ch",
                          timeString,@"t",
                          md5Encryption,@"sig",
                          nil];
    
    AFHTTPRequestOperationManager *manager = [AFHTTPRequestOperationManager manager];
    
    manager.requestSerializer = [AFHTTPRequestSerializer serializer];
    
    manager.responseSerializer = [AFHTTPResponseSerializer serializer];
    
    [manager POST:kTranslatedUrl parameters:dict success:^(AFHTTPRequestOperation *operation, id responseObject) {
        
        
        NSString *objectString =[[NSString alloc]initWithData:responseObject encoding:NSUTF8StringEncoding];
        DVLog(@"%@",objectString);
        NSDictionary *dic = [CSGoogleTranslat dictionaryWithJsonString:objectString];
        if (dic ) {
            NSArray *keyArray = [dic allKeys];
            if ([keyArray containsObject:@"translateMsg"]) {
                NSString *transMsg = [dic objectForKey:@"translateMsg"];
                NSString *transLang = [dic objectForKey:@"targetLang"];
                NSString  *orangeLang = [dic objectForKey:@"originalLang"];
                if (  [transLang isKindOfClass:[NSString class]] && transMsg.length > 0) {
                    if (vBlock != nil) {
                        vBlock(ZYTGoogleTranslatHTTPStatus_sucess,transMsg,orangeLang);
                    }
                }else{
                    if (vBlock != nil) {
                        vBlock (ZYTGoogleTranslatHTTPStatus_error,objectString,nil);
                    }
                }
                
            }else{
                 if (vBlock != nil) {
                    vBlock (ZYTGoogleTranslatHTTPStatus_error,objectString,nil);
            
                 }
            }
        }else{
            if (vBlock != nil) {
                vBlock (ZYTGoogleTranslatHTTPStatus_error,objectString,nil);
                
            }

        }
        
        
        
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        DVLog(@"error  :%@",error);
         if (vBlock != nil) {
             vBlock (ZYTGoogleTranslatHTTPStatus_error,error,nil);
         }
    }];
    
    
}

/*!
 * @brief 把格式化的JSON格式的字符串转换成字典
 * @param jsonString JSON格式的字符串
 * @return 返回字典
 */
+ (NSDictionary *)dictionaryWithJsonString:(NSString *)jsonString {
    if (jsonString == nil) {
        return nil;
    }
    
    NSData *jsonData = [jsonString dataUsingEncoding:NSUTF8StringEncoding];
    NSError *err;
    NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData
                                                        options:NSJSONReadingMutableContainers
                                                          error:&err];
    if(err) {
        NSLog(@"json解析失败：%@",err);
        return nil;
    }
    return dic;
}

-(void)translationingWithSL:(NSString *)vSL
                  andWithTL:(NSString *)vTL
                  andWithHL:(NSString *)vHL
                  andWithIE:(NSString *)vIE
                  andWithOE:(NSString *)vOE
                   andWithQ:(NSString *)vQ
             andResultBlock:(GoogleTranslatBlock)vBlock{
    NSString *urlString =@"https://translate.google.com/translate_a/single?client=t";
    urlString = [urlString stringByAppendingString:[NSString stringWithFormat:@"&sl=%@&tl=%@&hl=%@&dt=t&ie=%@&oe=%@&q=%@",
                                                    vSL,
                                                    vTL,
                                                    vHL,
                                                    vIE,
                                                    vOE,
                                                    vQ]];
    
    
//    NSLog(@"urlString :%@",urlString);
    
    AFHTTPRequestOperationManager *manager = [AFHTTPRequestOperationManager manager];
    
    manager.requestSerializer = [AFHTTPRequestSerializer serializer];
    manager.responseSerializer = [AFHTTPResponseSerializer serializer];
    manager.requestSerializer.timeoutInterval = 60;
    [manager GET:UnicodeStr(urlString) parameters:nil success:^(AFHTTPRequestOperation *operation, id responseObject) {
        //        NSLog(@"%@",operation);
        NSString *string =[[NSString alloc]initWithData:responseObject encoding:4];
//        NSLog(@"%@",string);
       [self fenjieString:string];
        if (vBlock != nil) {
            vBlock(ZYTGoogleTranslatHTTPStatus_sucess,transString,transLangType);
        }
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
//        NSLog(@"error :%@",error);
        vBlock (ZYTGoogleTranslatHTTPStatus_error,error,nil);
    }];
    
    
    
}

-(void)fenjieString:(NSString *)vString {
//    DVLog(@"要翻译的文字 : %@",vString);
    NSArray *array =[vString componentsSeparatedByString:@",,,,"];
    if (array.count == 0) {
        return;
    }
    NSString *tempString =[array objectAtIndex:0];
//    NSLog(@"%@",tempString);
    NSArray *array1 =[tempString componentsSeparatedByString:@",,"];
    NSString *string1 =[array1 objectAtIndex:0];
    NSString *transLateLangTypeStr = [array1 objectAtIndex:1];
    transLateLangTypeStr =[transLateLangTypeStr stringByReplacingOccurrencesOfString: @"\"" withString:@""];
//    DVLog(@"原语言类型%@",transLateLangTypeStr);
    transLangType = transLateLangTypeStr;
//    NSLog(@"%@",string1);
    string1 =[string1 stringByReplacingOccurrencesOfString: @"[[[\"" withString:@""];
    string1 = [string1 stringByReplacingOccurrencesOfString:@"\"]]" withString:@""];
//    NSLog(@"%@",string1);
    NSArray *stringArray =[string1 componentsSeparatedByString:@"\",\""];
    NSString *lastString =[stringArray objectAtIndex:0];
//    DVLog(@"翻译后的语言%@",lastString);
    transString = lastString;

}

@end
