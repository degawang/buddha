

#### `const`

```cpp
//指针本身是常量，声明时必须初始化，之后它存储的地址值就不能再改变
顶层const：指针本身是个常量
底层const：指针指向的对象是常量
//左侧const是底层const,右侧const是顶层const
//（靠近谁就限制谁，指针属于顶层，对象属于底层）
const int * const point = nullptr;

//执行对象拷贝时有限制，常量的底层const不能赋值给非常量的底层const
(可以避免修改底层const所指向的const数值)
std::const_cast 只能改变运算对象的底层const
//用于声明引用的const都是底层

//  Koenig lookup
```
