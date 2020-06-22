
#### ```std::atomic```
初等 std::atomic 模板可用任何满足 可复制构造 (CopyConstructible) 及 
可复制赋值 (CopyAssignable) 的 可平凡复制 (TriviallyCopyable) 类型 T 
特化。若下列任何值为 false 则程序为病式：

 - `std::is_copy_assignable<T>::value`
 - `std::is_move_assignable<T>::value`
 - `std::is_trivially_copyable<T>::value`
 - `std::is_copy_constructible<T>::value`
 - `std::is_move_constructible<T>::value`
 
 std::atomic<bool> 使用初等模板。它保证是标准布局结构体。

#### memory order

|类型|作用|
|:---:|:---:|
|memory_order_relaxed|宽松操作：没有同步或顺序制约，仅对此操作要求原子性（见下方宽松顺序）|
|memory_order_consume|有此内存顺序的加载操作，在其影响的内存位置进行消费操作：当前线程中依赖于当前加载的该值的读或写不能被重排到此加载前。其他释放同一原子变量的线程的对数据依赖变量的写入，为当前线程所可见。在大多数平台上，这只影响到编译器优化（见下方释放消费顺序）|
|memory_order_acquire|有此内存顺序的加载操作，在其影响的内存位置进行获得操作：当前线程中读或写不能被重排到此加载前。其他释放同一原子变量的线程的所有写入，能为当前线程所见（见下方释放获得顺序）|
|memory_order_release|有此内存顺序的存储操作进行释放操作：当前线程中的读或写不能被重排到此存储后。当前线程的所有写入，可见于获得该同一原子变量的其他线程释放获得顺序），并且对该原子变量的带依赖写入变得对于其他消费同一原子对象的线程可见（见下方释放消费顺序）|
|memory_order_acq_rel|带此内存顺序的读修改写操作既是获得操作又是释放操作。当前线程的读或写内存不能被重排到此存储前或后。所有释放同一原子变量的线程的写入可见于修改之前，而且修改可见于其他获得同一原子变量的线程|
|memory_order_seq_cst|有此内存顺序的加载操作进行获得操作，存储操作进行释放操作，而读修改写操作进行获得操作和释放操作，再加上存在一个单独全序，其中所有线程以同一顺序观测到所有修改（见下方序列一致顺序）|



#### ```std::lock&&std::scoped_lock```
`std::unique_lock`具有延迟上锁的功能。`std::lock`同时上锁多个互斥锁，之后再使用`std::lock_guard`分别
```cpp
std::lock(lock_1,lock_2);
std::lock_guard<mutex> lock_guard_1(lock_1, std::adopt_lock);
std::lock_guard<mutex> lock_guard_2(lock_2, std::adopt_lock);
                             ||
std::unique_lock<mutex> lock_guard_1(lock_1, std::defer_lock);
std::unique_lock<mutex> lock_guard_2(lock_2, std::defer_lock);
std::lock(lock_1,lock_2);
                             ||
std::scoped_lock(lock_1,lock_2);
```
接管经上锁两个互斥量，达到同时上锁（避免死锁）的目的。
#### ```std::shared_mutex```
与`std::shared_lock`与`std::unique_lock`配合使用，可以实现控制读写时分别获得与不可获得锁的特性。
比`std::mutex`更加灵活，但代价高。
```cpp
class ThreadSafeCounter {
public:
  // 多个线程/读者能同时读计数器的值。
  unsigned int get() const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return value_;
  }
  // 只有一个线程/写者能增加/写线程的值。
  void increment() {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    value_++;
  }
  // 只有一个线程/写者能重置/写线程的值。
  void reset() {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    value_ = 0;
  }
private:
  mutable std::shared_mutex mutex_;
  unsigned int value_ = 0;
};
```
#### ```std::call_once```
控制多个线程中只调用一次。
```cpp
std::call_once(std::once_flag& _once_flag, callable&& _callable, _args&&... args);_
```
#### `std::condition_variable`
`std::condition_variable`只能与`std::unique_lock<std::mutex>`一起使用。
`std::condition_variable_any`提供可与任何基本可锁定 (BasicLockable) 对象，
例如`std::shared_lock`一同使用的条件变量。
#### `std::condition_variable`与`notify_one，notify_all`
通知线程不必保有与等待线程所保有者相同互斥的锁；实际上这么做是劣化，
因为被通知线程将立即再次阻塞，以等待通知线程释放锁==>锁是cv等待的实体，
如果通知时候占有锁，则被通知的等待的cv将由于拿不到锁被虚假唤醒，再次进入等待，直到锁被释放。

#### `async`
若从`std::async`获得的`std::future`未被移动或绑定到引用，则在完整表达式结尾，`std::future` 
的析构函数将阻塞直至异步计算完成，实质上令如下代码同步：
```cpp
std::async(std::launch::async, []{ preview(); });
std::async(std::launch::async, []{ behind(); });
```
#### `std::atomic_thread_fence`
`栅栏原子同步`

`原子栅栏同步`

`栅栏栅栏同步`

线程 A 中的释放栅栏 FA 同步于线程 B 中的获得栅栏 FB。
```cpp
template<typename ..._args>
static std::shared_ptr<_derived> get_instance(_args&&... args) {
    auto _instance = __instance.load(std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_acquire);
    if (nullptr == _instance) {
        std::lock_guard<std::mutex> lock(__mutex);
        _instance = __instance.load(std::memory_order_relaxed);
        if (nullptr == _instace) {
            _instance = new(std::nothrow) _derived(std::forward<_args>(args)...);
            std::atomic_thread_fence(std::memory_order_release);
            __instance.store(_instance, std::memory_order_relaxed);
        }
    }
    return _instance;
}
```
 1). 释放栅栏在原子`__instance`写操作的前方。     
 2). 获取栅栏在原子`__instance`读操作的后方。     
 3). 释放栅栏线程中，所有先序于释放栅栏的非原子和宽松原子存储，将先发生于获取栅栏线程中
     来自同一位置的非原子和宽松原子加载。 
```cpp
代码解释：  
线程A中的释放栅栏，同步于线程B中的获得栅栏，线程A中释放栅栏之前的非原子与宽松原子存储将
先发生于线程B中获得栅栏之后的所有非原子与宽松原子加载。线程A中走到释放栅栏之前，拥有线程
锁`mutex`，线程B应该停顿在获得栅栏之后的锁定互斥锁的操作的步骤，等线程A释放线程的互斥锁
之后，原子变量`__instance`此时已经非空，所以在下一步加载原子变量`__instance`的时候，判
断条件失败，直接返回当前的临时变量`_instance`,这段添加了内存栅栏的代码，最多会对互斥锁进
行两次锁定操作，在以后的运行中不会进行加锁操作，提高了效率。
```

#### 无锁编程



