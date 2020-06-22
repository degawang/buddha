
[toc]

#### new delete
#### 内存池 线程池，环形缓冲
- [ ] （多线程）线程池加入申请多个元素的操作（是否可以通过重载 operator new[] 与 operator delete[]完成？）-->不能使用单例模式

- [ ] `typename`
使用关键字typename标识嵌套从属类型名称, 但不需在基类列表和成员初始化列表内使用  
从属名称(dependent names): 模板(template)内出现的名称, 相依于某个模板(template)参数, 如T t;  
嵌套从属名称(nested dependent names):从属名称在class内呈嵌套装, 如T::const_iterator ci;  
非从属名称(non-dependent names): 不依赖任何template参数的名称, 如int value;  
- [ ] 死锁问题
1) 没有对互斥锁进行开锁（中途返回，没有使用RAII）
2) 单线程重复申请锁（非循环锁）
3) 双线程多锁申请，锁的顺序没有固定（std::lock,std::scoped_lock）
4) 试图申请环形锁
- [ ] 无锁编程
https://www.jianshu.com/p/3e122ee901c5
- [ ] 
- [ ] 
- [ ] 
- [ ] 
- [ ] 
- [ ] 
- [ ] 
- [ ] 
- [ ] 
- [ ] 
- [ ] 
- [ ] 
- [ ] 
- [ ] 
- [ ] 
- [ ] 
- [ ] 
- [ ] 

#### 重载数据结构的 XXX xxx[];

- [X] const
- [ ] using的必要位置

- [ ] std::remove_cv_ref && std::remove_ref_cv && std::decay
```cpp
template <typename T>
struct remove_all_ref_ptr { typedef T type; };

template <typename T>
struct remove_all_ref_ptr<T *> : public remove_all_ref_ptr<T> { };

template <typename T>
struct remove_all_ref_ptr<T * const> : public remove_all_ref_ptr<T> { };

template <typename T>
struct remove_all_ref_ptr<T * volatile> : public remove_all_ref_ptr<T> { };

template <typename T>
struct remove_all_ref_ptr<T * const volatile> : public remove_all_ref_ptr<T> { };

template <typename T>
struct remove_all_ref_ptr<T &> : public remove_all_ref_ptr<T> { };

template <typename T>
struct remove_all_ref_ptr<T &&> : public remove_all_ref_ptr<T> { };

 #include <type_traits>
 template <typename T>
 struct base_type : public ::std::remove_cv<typename remove_all_ref_ptr<T>::type> { };
```
```cpp
#include <iostream>
#include <type_traits>

template<typename T>
using Remove_cv_ref = std::remove_cv<typename std::remove_reference<T>::type>;
    
template<typename T>
using Remove_ref_cv = std::remove_reference<typename std::remove_cv<T>::type>;

int main()
{
    //remove_ref之后的类型是 const int,remove_cv之后的类型是int
    std::cout << std::is_same<typename Remove_cv_ref<const int&>::type, int>::value;
    //remove_cv之后的类型是const int&(对引用的const修饰全是底层const,remove_cv只能去除顶层的
    //const,只有const_cast可以去除底层const),remove_ref之后的类型是 const int
    std::cout << std::is_same<typename Remove_ref_cv<const int&>::type, int>::value;
}
```