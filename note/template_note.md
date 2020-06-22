#### 参数包
若两个形参包出现于同一模式中，则它们同时展开，而且它们必须有相同长度
```cpp
template<class ..._args1> struct zip {
    template<class ..._args2> struct with {
        typedef Tuple<Pair<+args1, _args2>...> type;
//        Pair<_args1, _args2>... 是包展开
//        Pair<_args1, _args2> 是模式
    };
};
```
若包展开内嵌于另一包展开中，则其所展开的是出现于最内层包展开的形参包，
并且必须在外围（而非最内层）的包展开中必须提及另一个包
```cpp
template<class ..._args>
    void g(_args... args) {
        f(const_cast<const _args*>(&args)...); 
 // const_cast<const _args*>(&args) 是模式，它同时展开两个包（_args 与 args）
 
        f(h(args...) + args...); // 嵌套包展开：
   // 内层包展开是“args...”，它首先展开
   // 外层包展开是 h(E1, E2, E3) + args 它被第二次展开
   // （成为 h(E1,E2,E3) + E1, h(E1,E2,E3) + E2, h(E1,E2,E3) + E3）
}
```
以下列举几种展开模式
```cpp
(函数实参列表)包展开可以出现在函数调用运算符的括号内，此情况下省略号左侧的最大表达式或花括号初始化器列表是被展开的模式。
f(const_cast<const Args*>(&args)...);
// f(const_cast<const E1*>(&X1), const_cast<const E2*>(&X2), const_cast<const E3*>(&X3))
f(h(args...) + args...); // 展开成
// f(h(E1,E2,E3) + E1, h(E1,E2,E3) + E2, h(E1,E2,E3) + E3)
```

```cpp
(有括号初始化器)包展开可出现于直接初始化器，函数式转型及其他语境（成员初始化器，new 表达式等）的括号之内，这种情况
下的规则与适用于上述函数调用表达式的规则相同。
Class c1(&args...);             // 调用 Class::Class(&E1, &E2, &E3)
Class c2 = Class(n, ++args...); // 调用 Class::Class(n, ++E1, ++E2, ++E3);
::new((void *)p) U(std::forward<Args>(args)...)
// std::allocator::allocate U(Args_1(args_1),Args_2(args_2),...)
```
```cpp
(花括号包围的初始化器)在花括号初始化器列表（花括号包围的初始化器和其他花括号初始化器列表的列表，用于列表初始化和其
他一些语境中）中，也可以出现包展开。
template<typename... Ts> void func(Ts... args){
    const int size = sizeof...(args) + 2;
    int res[size] = {1,args...,2};
    // 因为初始化器列表保证顺序，所以这可用于按顺序对包的每个元素调用函数：
    int dummy[sizeof...(Ts)] = { (std::cout << args, 0)... };
}
```
```cpp
(模板实参列表)包展开可用于模板形参列表的任何位置，前提是模板拥有与该展开相匹配的形参。
template<class A, class B, class...C> void func(A arg1, B arg2, C...arg3)
{
    container<A,B,C...> t1;  // 展开成 container<A,B,E1,E2,E3> 
    container<C...,A,B> t2;  // 展开成 container<E1,E2,E3,A,B> 
    container<A,C...,B> t3;  // 展开成 container<A,E1,E2,E3,B> 
}
```
```cpp
(函数形参列表)在函数形参列表中，若省略号出现于某个形参声明中（无论它是否指名函数形参包
例如在 Args ... args中）），则该形参声明是模式。
template<typename ...Ts> void f(Ts...) {}
f('a', 1);  // Ts... 展开成 void f(char, int)
f(0.1);     // Ts... 展开成 void f(double)
 
template<typename ...Ts, int... N> void g(Ts (&...arr)[N]) {}
int n[1];
g<const char, int>("a", n); // Ts (&...arr)[N] 展开成 
                            // const char (&)[2], int(&)[1]
注意：在模式 Ts (&...arr)[N] 中，省略号是最内层元素，而非如所有其他包展开中一样为其
最后元素。注意：Ts (&...)[N] 不被允许，因为 C++11 语法要求带括号的省略号形参拥有名
字：CWG #1488。
```
```cpp
(模板形参列表)包展开可以出现于模板形参列表中。
template<typename... T> struct value_holder
{
    template<T... Values> // 展开成非类型模板形参列表，
    struct apply { };     // 例如 <int, char, int(&)[5]>
};
```
```cpp
(基类说明符与成员初始化器列表)包展开可以用于指定类声明中的基类列表。典型情况下，这也
意味着其构造函数也需要在成员初始化器列表中使用包展开，以调用这些基类的构造函数。
template<class... Mixins>
class X : public Mixins... {
 public:
    X(const Mixins&... mixins) : Mixins(mixins)... { }
};
```
```cpp
(Lambda 俘获)包展开可以出现于 lambda 表达式的俘获子句中。
template<class ...Args>
void f(Args... args) {
    auto lm = [&, args...] { return g(args...); };
    lm();
}
```
```cpp
(sizeof... 运算符）sizeof... 也被归类为包展开。
template<class... Types>
struct count {
    static const std::size_t value = sizeof...(Types);
};
```

#### 折叠表达式
```cpp
1) 一元右折叠 (E op ...) 成为 (E op E_1(... op (E N-1 op E N)))
2) 一元左折叠 (... op E) 成为 (((E op E) op ...) op E N)
3) 二元右折叠 (E op ... op I) 成为 (E op (... op (E N−1 op (E N op I))))
4) 二元左折叠 (I op ... op E) 成为 ((((I op E op E op ...) op E N)

template<typename... Args>
bool all(Args... args) { return (... && args); }
 
bool b = all(true, true, true, false);
 // 在 all() 中，一元左折叠展开成
 //  return ((true && true) && true) && false;
 // b 为 false

将一元折叠用于零长包展开时，仅允许下列运算符:
1) 逻辑与（&&）。空包的值为 true
2) 逻辑或（||）。空包的值为 false
3) 逗号运算符（,）。空包的值为 void()

疑问：（若用作初值或形参包的表达式在顶层具有优先级低于转型的运算符，
则它可以加括号）
template<typename ...Args>
int sum(Args&&... args) {
//    return (args + ... + 1 * 2); // 错误：优先级低于转型的运算符
    return (args + ... + (1 * 2)); // OK
}
```
#### 变参数函数
|名称|含义|
|:---:|:---:|
|va_start|启用对可变函数实参的访问|
|va_arg|访问下一个可变函数实参|
|va_copy|(C++11)制造可变函数实参的副本|
|va_end|结束对可变函数实参的遍历|
|va_list|保有 va_start、va_arg、va_end 和 va_copy 所需的信息|

```cpp
#include <iostream>
#include <cstdarg>
void simple_printf(const char* fmt...)
{
    va_list args;
    va_start(args, fmt);
    while (*fmt != '\0') {
        if (*fmt == 'd') {
            int i = va_arg(args, int);
            std::cout << i << '\n';
        } else if (*fmt == 'c') {
            // 注意自动转换到整数类型
            int c = va_arg(args, int);
            std::cout << static_cast<char>(c) << '\n';
        } else if (*fmt == 'f') {
            double d = va_arg(args, double);
            std::cout << d << '\n';
        }
        ++fmt;
    }
    va_end(args);
}
int main()
{
    simple_printf("dcff", 3, 'a', 1.999, 42.5); 
}
```

