
# 适当安排 add_custom_target 命令位置，可以控制项目生成之前与完成之后进行相应的操作
## windows 导入动态库
```cmake
add_library(bar SHARED IMPORTED)
set_property(TARGET bar PROPERTY IMPORTED_IMPLIB c:/path/to/bar.lib)
set_property(TARGET bar PROPERTY IMPORTED_LOCATION c:/path/to/bar.dll)
add_executable(myexe src1.c src2.c)
target_link_libraries(myexe bar)
```