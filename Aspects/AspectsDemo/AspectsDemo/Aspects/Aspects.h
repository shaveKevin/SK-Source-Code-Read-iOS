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

/// Adds a block of code before/instead/after the current `selector` for a specific class.
///
/// @param block Aspects replicates the type signature of the method being hooked.
/// The first parameter will be `id<AspectInfo>`, followed by all parameters of the method.
/// These parameters are optional and will be filled to match the block signature.
/// You can even use an empty block, or one that simple gets `id<AspectInfo>`.
///
/// @note Hooking static methods is not supported.
/// @return A token which allows to later deregister the aspect.
+ (id<AspectToken>)aspect_hookSelector:(SEL)selector
                      withOptions:(AspectOptions)options
                       usingBlock:(id)block
                            error:(NSError **)error;

/// Adds a block of code before/instead/after the current `selector` for a specific instance.
- (id<AspectToken>)aspect_hookSelector:(SEL)selector
                      withOptions:(AspectOptions)options
                       usingBlock:(id)block
                            error:(NSError **)error;

@end


typedef NS_ENUM(NSUInteger, AspectErrorCode) {
    AspectErrorSelectorBlacklisted,                   /// Selectors like release, retain, autorelease are blacklisted.
    AspectErrorDoesNotRespondToSelector,              /// Selector could not be found.
    AspectErrorSelectorDeallocPosition,               /// When hooking dealloc, only AspectPositionBefore is allowed.
    AspectErrorSelectorAlreadyHookedInClassHierarchy, /// Statically hooking the same method in subclasses is not allowed.
    AspectErrorFailedToAllocateClassPair,             /// The runtime failed creating a class pair.
    AspectErrorMissingBlockSignature,                 /// The block misses compile time signature info and can't be called.
    AspectErrorIncompatibleBlockSignature,            /// The block signature does not match the method or is too large.

    AspectErrorRemoveObjectAlreadyDeallocated = 100   /// (for removing) The object hooked is already deallocated.
};

extern NSString *const AspectErrorDomain;
