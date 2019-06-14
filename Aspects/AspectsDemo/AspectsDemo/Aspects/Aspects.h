//
//  Aspects.h
//  Aspects - A delightful, simple library for aspect oriented programming.
//
//  Copyright (c) 2014 Peter Steinberger. Licensed under the MIT license.
//

#import <Foundation/Foundation.h>
//这里是hook的时机(可参考SKSubModel中runInheritMode方法的执行)
typedef NS_OPTIONS(NSUInteger, AspectOptions) {
    AspectPositionAfter   = 0,            /// Called after the original implementation (default)默认是在本来方法实现执行完之后进行执行(这个时机类似于继承父类方法中调用super方法的时机)
    AspectPositionInstead = 1,            /// Will replace the original implementation. 方法的替换(类似于父类实现了某个方法子类重写了，但是没用继承。这样就不会走父类方法，由子类进行自由发挥)
    AspectPositionBefore  = 2,            /// Called before the original implementation.在hook目标方法之前执行，类似于继承中子类调用super 之前执行
    AspectOptionAutomaticRemoval = 1 << 3 /// Will remove the hook after the first execution.方法执行一次就不再执行。类似于我们的单例的创建
};

/// Opaque Aspect Token that allows to deregister the hook.
@protocol AspectToken <NSObject>

/// Deregisters an aspect.注销一个Aspect 注销成功的时候返回YES 失败的时候返回NO
/// @return YES if deregistration is successful, otherwise NO.
- (BOOL)remove;

@end

/// The AspectInfo protocol is the first parameter of our block syntax.AspectInfo 协议中的第一个block语法中的参数
@protocol AspectInfo <NSObject>

/// The instance that is currently hooked.当前hook的对象
- (id)instance;

/// The original invocation of the hooked method. hook原生的方法的invocation
/// target 方法之执行的对象，selector hook的方法例如：{:} aspects__viewWillAppear: argument 2: {B} 0 ''
- (NSInvocation *)originalInvocation;
///??
/// All method arguments, boxed. This is lazily evaluated.承载所有方法参数的容器 ???这个方法的作用有待评估
- (NSArray *)arguments;

@end

/**
 Aspects uses Objective-C message forwarding to hook into messages. This will create some overhead. Don't add aspects to methods that are called a lot. Aspects is meant for view/controller code that is not called a 1000 times per second.

 Adding aspects returns an opaque token which can be used to deregister again. All calls are thread safe.
 切面编程用OC的消息转发机制hook消息，这会产生一定的消耗。如果一个方法被调用很多遍，不要使用aspects进行hook。使用切面编程意味着视图和容器VC一秒内调用次数不能超过1000次。
 添加一个切面编程的对象会返回一个token，这个token被用来注销切面编程。所有的调用都是线程安全的。
 */
@interface NSObject (Aspects)

/// Adds a block of code before/instead/after the current `selector` for a specific(类方法) class.(为指定的类的方法添加一个block代码，在方法执行之前，替换方法，或者方法执行之后)
///
/// @param block Aspects replicates the type signature of the method being hooked.(当方法被hook之到的时候做的操作)
/// The first parameter will be `id<AspectInfo>`, followed by all parameters of the method.（block的第一个参数为id<AspectInfo>）
/// These parameters are optional and will be filled to match the block signature.(当匹配到对应的block快的时候，这些参数会被填充)
/// You can even use an empty block, or one that simple gets `id<AspectInfo>`.(你甚至可以用一个空的block)
///
/// @note Hooking static methods is not supported.(静态方法是不允许被hook的)
/// @return A token which allows to later deregister the aspect.(返回一个token用于注销这个aspect)
+ (id<AspectToken>)aspect_hookSelector:(SEL)selector
                      withOptions:(AspectOptions)options
                       usingBlock:(id)block
                            error:(NSError **)error;

/// Adds a block of code before/instead/after the current `selector` for a specific instance.(实例方法)
- (id<AspectToken>)aspect_hookSelector:(SEL)selector
                      withOptions:(AspectOptions)options
                       usingBlock:(id)block
                            error:(NSError **)error;

@end


typedef NS_ENUM(NSUInteger, AspectErrorCode) {
    AspectErrorSelectorBlacklisted,                   /// Selectors like release, retain, autorelease are blacklisted. 方法被release retain和autorelease被加到黑名单中。
    AspectErrorDoesNotRespondToSelector,              /// Selector could not be found.被hook的方法找不到
    AspectErrorSelectorDeallocPosition,               /// When hooking dealloc, only AspectPositionBefore is allowed.当hook dealloc的时候
    AspectErrorSelectorAlreadyHookedInClassHierarchy, /// Statically hooking the same method in subclasses is not allowed.hook子类中相同的方法不被允许
    AspectErrorFailedToAllocateClassPair,             /// The runtime failed creating a class pair.运行时创建一个类失败
    AspectErrorMissingBlockSignature,                 /// The block misses compile time signature info and can't be called. 在编译期Block缺少signature也不会被调用
    AspectErrorIncompatibleBlockSignature,            /// The block signature does not match the method or is too large.block signature和方法不匹配或者太大了

    AspectErrorRemoveObjectAlreadyDeallocated = 100   /// (for removing) The object hooked is already deallocated.当被hook的方法被销毁的时候
};

extern NSString *const AspectErrorDomain;
