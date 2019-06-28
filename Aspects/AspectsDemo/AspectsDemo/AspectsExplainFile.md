## 代码流程

 1. 首先判断能否被hook
 2. 如果能够被hook，记录相关数据结构
 3. 对象第一次被hook
 4. swizzling其forwardInvocation
 5. sel第一次被hook
 6. swizzing其selector
 
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
 
#### swizzing method
这里才是真正的核心逻辑，swizzingmethod主要有两个部分，一个是对对象的forwardInvocation进行swizzing，另一个是对传入的selector进行swizzing。

 


