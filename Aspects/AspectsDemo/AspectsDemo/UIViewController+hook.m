//
//  UIViewController+hook.m
//  AspectsDemo
//
//  Created by shavekevin on 2019/5/29.
//  Copyright © 2019 shavekevin. All rights reserved.
//

#import "UIViewController+hook.h"
#import <Aspects/Aspects.h>

@implementation UIViewController (hook)

+ (void)hook {
    
    [self aspect_hookSelector:@selector(viewWillAppear:)
                  withOptions:AspectPositionAfter
                   usingBlock:^(id<AspectInfo> aspectInfo, BOOL animated) {
                       UIViewController * vc = aspectInfo.instance;
                       NSLog(@"vc class is %@",vc);
                   } error:NULL];
    
    [self aspect_hookSelector:@selector(viewWillDisappear:)
                  withOptions:AspectPositionAfter
                   usingBlock:^(id<AspectInfo> aspectInfo, BOOL animated) {
                   } error:NULL];
}

@end
