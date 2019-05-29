//
//  ViewController.m
//  YYCacheDemo
//
//  Created by houzhiwei on 2019/5/27.
//  Copyright © 2019 shavekevin. All rights reserved.
//

#import "ViewController.h"
#import <YYCache/YYCache.h>
#import <JSONKit-NoWarning/JSONKit.h>
#import "SKCacheListModel.h"
#import <YYModel/YYModel.h>
#import "SKCacheListVC.h"

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
