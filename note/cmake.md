<!--
 * @autor: degawong
 * @date: Do not edit
 * @lastEditors: degawong
 * @lastEditTime: Do not edit
 * @description: Do not edi
 * @filePath: Do not edit
--> 


#输出与静态库同名的动态库
SET(LIBHELLO_SRC hello.c)
ADD_LIBRARY(hello SHARED ${LIBHELLO_SRC})
ADD_LIBRARY(hello_static STATIC ${LIBHELLO_SRC})
SET_TARGET_PROPERTIES(hello_static PROPERTIES OUTPUT_NAME "hello")
#导入外部链接库
#add_library(curl STATIC IMPORTED)  
set_target_properties(curl  
  PROPERTIES IMPORTED_LOCATION  
  ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libcurl.a)
#debug与release库分别设置

#设置新的文件夹
file(
	GLOB_RECURSE
	MARKDOWN_NOTE
	note/*.md
)

source_group(
	"note"
	FILES
	${MARKDOWN_NOTE}
)

# 适当安排 add_custom_target 命令位置，可以控制项目生成之前与完成之后进行相应的操作
## windows 导入动态库
```cmake
add_library(bar SHARED IMPORTED)
set_property(TARGET bar PROPERTY IMPORTED_IMPLIB c:/path/to/bar.lib)
set_property(TARGET bar PROPERTY IMPORTED_LOCATION c:/path/to/bar.dll)
add_executable(myexe src1.c src2.c)
target_link_libraries(myexe bar)
```			 