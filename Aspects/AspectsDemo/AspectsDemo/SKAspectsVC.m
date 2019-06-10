//
//  SKAspectsVC.m
//  AspectsDemo
//
//  Created by houzhiwei on 2019/5/29.
//  Copyright © 2019 shavekevin. All rights reserved.
//

#import "SKAspectsVC.h"

@interface SKAspectsVC ()

@end

@implementation SKAspectsVC

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    NSLog(@"子类的viewWillAppear=");
}

@end
