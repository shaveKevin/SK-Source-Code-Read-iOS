//
//  ViewController.m
//  YYCacheDemo
//
//  Created by shavekevin on 2019/5/27.
//  Copyright © 2019 shavekevin. All rights reserved.
//

#import "ViewController.h"
#import "YYCache.h"
#import <JSONKit-NoWarning/JSONKit.h>
#import "SKCacheListModel.h"
#import <YYModel/YYModel.h>
#import "SKCacheListVC.h"
/*
 * Demo目的阅读YYCache源码
 * 收获
 * 1.了解YYCache原理
 * 2.对LRU算法理解加深
 * 3.具体问题：YYCache如何实现缓存？
 * 4.具体问题：YYCache适用什么样的场景？
 */
@interface ViewController ()

@property (nonatomic, strong) YYCache *cache;

@property (nonatomic, strong) NSMutableArray *dataArray;
@end

@implementation ViewController

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    NSLog(@"1");
}
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.cache = [YYCache cacheWithName:@"StasticCache"];
////    [self storeData];
//    [self loadJsonData];
    BOOL containsObject = [self.cache containsObjectForKey:@"dataArray"];
    if (containsObject) {
        id value = [self.cache objectForKey:@"dataArray"];
        NSLog(@"value=%@",value);
    }
    
}

- (void)loadJsonData {
    
    NSString *file = [[NSBundle mainBundle] pathForResource:@"DataList"ofType:@"json"];
    NSData *data = [NSData dataWithContentsOfFile:file];
    NSError *error;
    id jsonObject = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingAllowFragments error:&error];
    NSArray *array = [NSArray yy_modelArrayWithClass:[SKCacheListModel class] json:[[jsonObject  valueForKey:@"data"] valueForKey:@"items"]];
    [self.dataArray addObjectsFromArray:array];
    NSLog(@"%@",[self.dataArray JSONString]);
    [self.cache setObject:[[jsonObject  valueForKey:@"data"] valueForKey:@"items"] forKey:@"dataArray"];
    
}

- (void)storeData {
    NSString *value = @"storeObject";
    NSString *key = @"key";
    [self.cache setObject:value forKey:key];
}

- (NSMutableArray *)dataArray  {
    if (!_dataArray) {
        _dataArray = [NSMutableArray arrayWithCapacity:10];
    }
    return _dataArray;
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    SKCacheListVC *vc = [[SKCacheListVC alloc]init];
    [self.navigationController pushViewController:vc animated:YES];
}

@end
