//
//  YYModel.h
//  YYModel <https://github.com/ibireme/YYModel>
//
//  Created by ibireme on 15/5/10.
//  Copyright (c) 2015 ibireme.
//
//  This source code is licensed under the MIT-style license found in the
//  LICENSE file in the root directory of this source tree.
//

#import <Foundation/Foundation.h>

#if __has_include(<YYModel/YYModel.h>)
FOUNDATION_EXPORT double YYModelVersionNumber;
FOUNDATION_EXPORT const unsigned char YYModelVersionString[];
#import <YYModel/NSObject+YYModel.h>
#import <YYModel/YYClassInfo.h>
#else
#import "NSObject+YYModel.h"
#import "YYClassInfo.h"
#endif
/*
 这里要说下这里的 #if __has_include(<YYModel/YYModel.h>)
这句话。这句话字面意思是如果头文件中包括了<YYModel/YYModel.h>  也就是说这个文件是否存在。如果能被引入 那么就返回YES 否则返回NO.
  如果已经引入了 YYModel 那么在这里继续引入 <YYModel/NSObject+YYModel.h>  和  <YYModel/YYClassInfo.h> 这两个类。 如果没有引入<>的 那么就引入两个子类 "NSObject+YYModel.h" 和 "YYClassInfo.h"
 然后我们就看到  FOUNDATION_EXPORT double YYModelVersionNumber; 宏定义定义了一个double 类型的 YYModelVersionNumber
 FOUNDATION_EXPORT 和#define 作用是一样的，使用第一种在检索字符串的时候可以用 ==  #define 需要使用isEqualToString 在效率上前者由于是基于地址的判断 速度会更快一些.
*
*/
/*

拓展：

#include / #import 语句有两种方式包含头文件，分别是使用双引号" "与左右尖括号< >。其区别是（对于不是使用完全文件路径名的）头文件的搜索顺序不同

使用双引号" "的头文件的搜索顺序：

包含该#include语句的源文件所在目录；
包含该#include语句的源文件的已经打开的头文件的逆序；
编译选项-I所指定的目录
环境变量INCLUDE所定义的目录
使用左右尖括号< >的头文件的搜索顺序：

编译选项-I所指定的目录
环境变量INCLUDE所定义的目录
 */
/*
 简单小结一下：
 1.这个类是头文件 主要是引入各个类 方便用户调用。以及采用<> 和""引入到工程中的区别
 2.还有就是采用了一种判断方式 头文件是否能引入指定文件。
 3.采用FOUNDATION_EXPORT 和#define  来定义常量的区别。
 
 */