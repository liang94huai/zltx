//
//  NSDate+Extension.m
//  IF
//
//  Created by 张彦涛 on 16/1/13.
//
//

#import "NSDate+Extension.h"
#import "NSDateFormatter+Category.h"
@implementation NSDate (Extension)


+(NSString *)currentTimeStringForSecond{
   return  [[NSDateFormatter defaultDateFormatter] stringFromDate:[NSDate date]];
}
@end
