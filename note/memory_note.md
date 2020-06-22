



#### C++ memory order
==C++ 里的memory order是用来指定原子操作（atomic operation）和同线程内其他非原子
操作之间的执行顺序的。==
C++ memory order是对atomic操作的一种约束，通常这个atomic是一个多线程共享的变量，
因此我们要约定当前CPU操作这个变量的顺序。我们所谈到的memory order都是针对共享变
量的，这个变量可以是atomic的也可以是non-atomic的，但是一定是共享的。

- （release-acquire组合）虽然relaxed并不保证happens-before关系，但是在同一线程
里，release会保证在其之前的原子store操作都能被看见， acquire能保证同一线程中的后
续的load都能读到最新值（可以认为是一个`memory fence`,release保证之前的所有
`原子`store`操作都已经完成，`acquire`保证之后的原子load操作都能读到acquire同步
语句之前store的数值）。
- （release-consume组合）consume的特性只会对其carries-a-dependency-to的变量具
有synchronsizes-with功能。(c++17弃用，custom语义不够)
-  Visual Studio ，其中默认设置下，每个 volatile 写拥有释放语义，而每个 volatile
读拥有获得语义（ MSDN ），故而可将 volatile 对象用于线程间同步。

- 同步
- happen-befor
- 

#### `std::atomic_flag_test_and_set, std::atomic_flag_test_and_set_explicit`
原子地更改 p 所指向的 std::atomic_flag 的状态为`设置`（ true ），并返回其先前所保有的值
自旋锁LOCK时自旋锁是自己轮询状态，如果不引入中断机制，会有大量计算资源浪费到轮询本身上；常用的做
法是使用yield切换到其他线程执行，或直接使用sleep暂停当前线程.
```cpp
std::atomic<T>::exchange,原子地以 desired 替换底层值。操作为读-修改-写操作。根据 order 的值影响内存。
返回修改前的原子的数值。
    std::atomic_flag lock = ATOMIC_FLAG_INIT;
    for (int cnt = 0; cnt < 100; ++cnt) {
        while(std::atomic_flag_test_and_set_explicit(&lock, std::memory_order_acquire))
             ; // 自旋直至获得锁
        std::cout << "Output from thread " << n << '\n';
        std::atomic_flag_clear_explicit(&lock, std::memory_order_release);
    }

```

#### `std::atomic<T>::exchange && std::atomic<T>::compare_exchange_weak && std::atomic<T>::compare_exchange_strong`

```cpp
bool compare_exchange_xxx( T& expected, T desired,
                            std::memory_order success, 
                            std::memory_order failure ) volatile noexcept;
bool compare_exchange_xxx( T& expected, T desired,
                            std::memory_order order =
                                std::memory_order_seq_cst ) volatile noexcept;

若成功更改底层原子值则为 true ，否则为 false

允许函数的弱形式 (1-2) 虚假地失败，即表现如同 *this != expected ，纵使它们相等 ？？？

Weak版本如果数据符合条件被修改，其也可能返回false，就好像不符合修改状态一致；而Strong版
本不会有这个问题，但在某些平台上Strong版本比Weak版本慢。

原子地比较 *this 和 expected 的对象表示 (C++20 前)值表示 (C++20 起)，而若它们逐位相等，则以 desired 
替换前者（进行读修改写操作）。否则，将 *this 中的实际值加载进 expected （进行加载操作）。

if(*this == expected){
    *this = desired;
}else{
    expected = *this;
}

若有失败成功之分的情况，order 用于读修改写操作和加载操作，除了若 order == std::memory_order_acq_rel 
或 order == std::memory_order_release ，则加载操作分别使用 std::memory_order_acquire 和 std::memory_order_relaxed 。 
```
```cpp
template<typename T>
class stack
{
    std::atomic<node<T>*> head;
 public:
    void push(const T& data)
    {
      node<T>* new_node = new node<T>(data);
      new_node->next = head.load(std::memory_order_relaxed);
      while(!head.compare_exchange_weak(new_node->next, new_node,
                                        std::memory_order_release,
                                        std::memory_order_relaxed)){}
    }
    void pop()
   {
        // 使用内存栅栏进行同步
        Node* node = head_.load();
        while (node && !head_.compare_exchange_strong(node, node->next)) {}
        if (node) delete node;
  }
}
```
`弱版本比较和交换会要求循环，而强版本不要求时，推荐用强版本,弱版本性能好`
