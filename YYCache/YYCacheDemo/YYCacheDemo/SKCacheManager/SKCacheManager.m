//
//  SKCacheManager.m
//  YYCacheDemo
//
//  Created by shavekevin on 2019/5/28.
//  Copyright © 2019 shavekevin. All rights reserved.
//

#import "SKCacheManager.h"


@interface SKCacheManager()

@property (nonatomic, strong) NSMutableArray *dataArray;

@end

@implementation SKCacheManager

+ (instancetype)sharedInstance {
    
    static SKCacheManager *manager = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        manager = [[SKCacheManager alloc]init];
    });
    return manager;

}

- (NSMutableArray *)dataArray {
    if (!_dataArray) {
        _dataArray = [NSMutableArray arrayWithCapacity:10];
    }
    return _dataArray;
}
@end
