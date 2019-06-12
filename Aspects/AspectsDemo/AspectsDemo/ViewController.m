//
//  ViewController.m
//  AspectsDemo
//
//  Created by shavekevin on 2019/5/29.
//  Copyright © 2019 shavekevin. All rights reserved.
//

#import "ViewController.h"
#import "SKAspectsVC.h"
#import "SKSubModel.h"

/*
 * Demo目的阅读Aspects源码
 * 收获
 * 1.了解aspects原理
 * 2.对runtime理解加深
 * 3.具体问题：为什么hook的时候hook到子类而父类没有hook到？
 * 4.具体问题：Aspects适用什么样的场景？
 */
@interface ViewController ()

@end

@implementation ViewController


- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.view.backgroundColor = [UIColor whiteColor];
    SKSubModel *subModel = [[SKSubModel alloc]init];
    [subModel runInheritMode];
}

- (void)viewWillAppear:(BOOL)animated {
   [super viewWillAppear:animated];
    NSLog(@"父类的viewWillAppear=");
}
- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    SKAspectsVC * spectsVC = [[SKAspectsVC alloc]init];
    [self.navigationController pushViewController:spectsVC animated:YES];
}
@end
