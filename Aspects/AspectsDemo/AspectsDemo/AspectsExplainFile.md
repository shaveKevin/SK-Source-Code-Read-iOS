以下内容来源于：[面向切面编程之 Aspects 源码解析及应用
](https://wereadteam.github.io/2016/06/30/Aspects/)
## 代码流程

 1. 首先判断能否被hook
 2. 如果能够被hook，记录相关数据结构
 3. 对象第一次被hook
 4. swizzling其forwardInvocation
 5. sel第一次被hook
 6. swizzling其selector
 
### **从代码来看想要hook一个selector首先要添加一个aspect，可以通过类/实例方法。** 关键代码如下：
 ```
 static id aspect_add(id self, SEL selector, AspectOptions options, id block, NSError **error) {
    NSCParameterAssert(self);
    NSCParameterAssert(selector);
    NSCParameterAssert(block);

    __block AspectIdentifier *identifier = nil;
    // 线程安全内执行(如果方法允许被hook 那么返回hook对象)
    aspect_performLocked(^{
        //1.aspects_的方法是否允许被hook
        if (aspect_isSelectorAllowedAndTrack(self, selector, options, error)) {
        2.记录数据结构
            // 获取hook方法的容器
            AspectsContainer *aspectContainer = aspect_getContainerForObject(self, selector);
            identifier = [AspectIdentifier identifierWithSelector:selector object:self options:options block:block error:error];
            if (identifier) {
                // 如果能找到方法的标识，那么就把这个AspectIdentifier添加到容器中。
                [aspectContainer addAspect:identifier withOptions:options];

                // Modify the class to allow message interception.
                aspect_prepareClassAndHookSelector(self, selector, error);
            }
        }
    });
    return identifier;
}

 ```
 
#### 判断对象能否被hook
 对于对象实例而言，这里主要是根据黑名单，比如retain forwardInvocataion等这些方法在外部是不能被hook的(对于类对象还要确保在一个类的层级关系中，只能被hook一次，因此这里需要判断子类，父类有没有被hook，之所以做这样的实现，主要是为了避免死循环的出现。如果能hook，则继续下面的步骤。
 
#### swizzling method
这里才是真正的核心逻辑，swizzling method主要有两个部分，一个是对对象的forwardInvocation进行swizzling，另一个是对传入的selector进行swizzling。

```
// 核心部分  开始准备hook方法(这个self指的是hook方法的调用者)
static void aspect_prepareClassAndHookSelector(NSObject *self, SEL selector, NSError **error) {
    Class kclass = aspect_hookClass(self, error);
    Method targetMethod = class_getInstanceMethod(kclass, selector);
    IMP targetMethodIMP = method_getImplementation(targetMethod);
    if (!aspect_isMsgForwardIMP(targetMethodIMP)) {

        }
    }
}
```
#### swizzling forwardInvocation：
aspect_hookClass函数主要swizzling类/对象的forwardInvocation函数，aspects的真正的处理逻辑都是在forwardInvocation函数里面进行的。对于对象实例而言，源码中并没有直接swizzling对象的forwardInvocation方法，而是动态生成一个当前类的子类，并将当前对象与子类关联，然后替换子类的forwardInvocation方法(这里具体调用了objc_setClass(self, subclass)),将当前对象isa指针指向了subclass，同时修改了subclass以及其subclass的metaclass的class方法，使它返回当前对象的class。这个地方有点绕，它的原理类似于kvo的实现，它想要实现的效果是，将当前对象变成一个subclass实例，同时对于外部使用者而言，又能把它继续当成原对象使用，而且所有的swizzling操作都发生在子类，这样的好处是你不需要去更改对象本身的类，也就是，当你在remove aspects的时候，如果发现当前类对象的aspects都被移除了，那么，你可以将isa指针重新指回对象本身的类，从而消除了该对象的swizzling，同时也不会影响到其他该类的不同对象）。对于每一个对象而言，这样的动态对象只会生成一次，这里aspects_swizzlingForwardInvocation将似的forwardInvocation方法指向aspects自己的实现逻辑，具体代码如下：
```
static Class aspect_hookClass(NSObject *self, NSError **error) {
···
        // 生成动态子类，并swizzling forwardInvocation方法
        subclass = objc_allocateClassPair(baseClass, subclassName, 0);
        // hook父类的forwardInvocation方法
        aspect_swizzleForwardInvocation(subclass);
        // 方法注册
        objc_registerClassPair(subclass);
···
       // 这里将self设置为子类，其实是改了self的isa指针。
	    object_setClass(self, subclass);
	    return subclass;
}

static NSString *const AspectsForwardInvocationSelectorName = @"__aspects_forwardInvocation:";
static void aspect_swizzleForwardInvocation(Class klass) {
    NSCParameterAssert(klass);
    // If there is no method, replace will act like class_addMethod> .  如果方法不存在那么就add存在就替换
    IMP originalImplementation = class_replaceMethod(klass, @selector(forwardInvocation:), (IMP)__ASPECTS_ARE_BEING_CALLED__, "v@:@");
    if (originalImplementation) {
        class_addMethod(klass, NSSelectorFromString(AspectsForwardInvocationSelectorName), originalImplementation, "v@:@");
    }
    AspectLog(@"Aspects: %@ is now aspect aware.", NSStringFromClass(klass));
}

```
由于子类本身并没有实现forwardInvocation，隐藏返回的origin'allmplementation将为空值，所以也不会生成NSSelectorFromString(AspectsForwardInvocationSelectorName)。

#### swizzling selector
