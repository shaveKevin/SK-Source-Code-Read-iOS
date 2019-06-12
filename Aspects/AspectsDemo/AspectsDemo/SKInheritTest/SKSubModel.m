//
//  SKSubModel.m
//  AspectsDemo
//
//  Created by houzhiwei on 2019/6/10.
//  Copyright © 2019 shavekevin. All rights reserved.
//

#import "SKSubModel.h"

@implementation SKSubModel
- (void)runInheritMode {
    NSLog(@"子类执行在父类执行之前=runInheritMode");
    [super runInheritMode];
    NSLog(@"子类执行在父类执行之后=runInheritMode");
}
@end
