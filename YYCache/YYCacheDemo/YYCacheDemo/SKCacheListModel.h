//
//  SKCacheListModel.h
//  YYCacheDemo
//
//  Created by houzhiwei on 2019/5/28.
//  Copyright © 2019 shavekevin. All rights reserved.
//

#import <Foundation/Foundation.h>
@class SKCacheListDetailModel;
NS_ASSUME_NONNULL_BEGIN

@interface SKCacheListModel : NSObject
// 创建时间
@property (nonatomic, copy) NSString *createTime;
// 资源类型
@property (nonatomic, assign) NSInteger sourceType;

@property (nonatomic, copy) NSString *ID;

@property (nonatomic, copy) NSString *type;

@property (nonatomic, assign) NSInteger status;

@property (nonatomic, strong) SKCacheListDetailModel *rootComment;


@end

NS_ASSUME_NONNULL_END
