
project(arcsoft_portrait_relighting)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)

set(
	CMAKE_MODULE_PATH
	${CMAKE_CURRENT_SOURCE_DIR}/marco
)

include(
	marco
)

add_definitions(
	-Dtv=portrait_relighting
	#-D_ENABLE_LOG_PERFORMANCE
	-DSTB_IMAGE_IMPLEMENTATION
	-DSTB_IMAGE_WRITE_IMPLEMENTATION
)

# set(
	# CMAKE_CXX_FLAGS
	# "-O3"
# )

# if(${ANDROID_ABI} STREQUAL "armeabi-v7a")
	# include_directories(${ANDROID_NDK}/sysroot/usr/include/arm-linux-androideabi) 
# message(STATUS "1")

# elseif(${ANDROID_ABI} STREQUAL "arm64-v8a")
	# include_directories(${ANDROID_NDK}/sysroot/usr/include/aarch64-linux-android)
# message(STATUS "${ANDROID_NDK}/sysroot/usr/include/aarch64-linux-android")
# else() 
	# include_directories(${ANDROID_NDK}/sysroot/usr/include/arm-linux-androideabi)
# message(STATUS "3")
# endif()

add_subdirectory(algorithm)

# set(
	# CMAKE_CXX_FLAGS
	# "${CMAKE_CXX_FLAGS} -g -std=c++latest"
# )

# file(
	# GLOB_RECURSE
	# MARKDOWN_NOTE
	# note/*.md
# )

# source_group(
	# "note"
	# FILES
	# ${MARKDOWN_NOTE}
# )

file(
	GLOB_RECURSE SRCS
	src/* 
	input/inc/*
	output/inc/*
)

filter(
	SRCS
)

add_library(
	${PROJECT_NAME}
	${SRCS}
	$<TARGET_OBJECTS:toy>
	$<TARGET_OBJECTS:toy_core>
	$<TARGET_OBJECTS:toy_fourcc>
)

target_link_libraries(
	${PROJECT_NAME}
	toy
	toy_fourcc
)

target_include_directories(
	${PROJECT_NAME} PUBLIC
	src
	output/inc
	src/style/sketch
	src/style/cartoon
	input/inc/common
	input/inc/fundamental
)

# set output directory
# set_target_properties(
	# ${PROJECT_NAME} PROPERTIES
	# ARCHIVE_OUTPUT_DIRECTORY
	# ${CMAKE_CURRENT_SOURCE_DIR}/output/${ANDROID_ABI}
	# LIBRARY_OUTPUT_DIRECTORY
	# ${CMAKE_CURRENT_SOURCE_DIR}/output/${ANDROID_ABI}
# )
if(CMAKE_SYSTEM_NAME MATCHES "Windows")

	file(
		GLOB UISRCS 
		${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	)
	
	file(
		GLOB_RECURSE UISRCS
		${UISRCS}
		output/inc/*
		3rdparty/inc/*
		3rdparty/function/*
	)

	filter(
		UISRCS
	)

	file(
		GLOB_RECURSE
		LIBFILE
		3rdparty/*.lib
		input/lib/windows/x64/*.lib
	)
	
	target_link_libraries(
		${PROJECT_NAME}
		${LIBFILE}
	)

	# 32 64 debug release
	file(
		GLOB DLLLIBRARY
		input/model/*
		input/lib/windows/x64/*.dll
		3rdparty/lib/fundamental/*.dll
		#${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/*.dll
	)
	
	set(
		WINDOWS_PACKAGE
		${CMAKE_CURRENT_SOURCE_DIR}/output/package${CMAKE_STATIC_LIBRARY_SUFFIX}
	)
	add_custom_command(
		OUTPUT ${WINDOWS_PACKAGE}
		COMMAND lib /OUT:${WINDOWS_PACKAGE} $<TARGET_FILE:toy> $<TARGET_FILE:toy_core>  $<TARGET_FILE:${PROJECT_NAME}>
		DEPENDS toy toy_core ${PROJECT_NAME}
		COMMENT "package windows depend libraries into one package..."
	)

	add_executable(
		arcsoft_ui
		${UISRCS}
	)

	add_custom_target(
		windows_package
		DEPENDS ${WINDOWS_PACKAGE}
	)

	add_dependencies(
		arcsoft_ui
		windows_package
	)

	target_link_libraries(
		arcsoft_ui
		debug
		${LIBFILE}
		${WINDOWS_PACKAGE}
		optimized
		${LIBFILE}
		${WINDOWS_PACKAGE}
	)

	target_include_directories(
		arcsoft_ui PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/src
		${CMAKE_CURRENT_SOURCE_DIR}/output/inc
		${CMAKE_CURRENT_SOURCE_DIR}/input/inc/common
		${CMAKE_CURRENT_SOURCE_DIR}/input/inc/fundamental
		${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/function
		${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/inc/common
		${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/inc/fundamental
	)

	set_directory_properties(
		PROPERTIES VS_STARTUP_PROJECT
		arcsoft_ui
	)

	set_target_properties(
		arcsoft_ui
		PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
		"${CMAKE_CURRENT_SOURCE_DIR}/workspace/"
	)

	# 	CMAKE_ARCHIVE_OUTPUT_DIRECTORY CMAKE_LIBRARY_OUTPUT_DIRECTORY useless
	set_target_properties(
		arcsoft_ui PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY
		${CMAKE_CURRENT_SOURCE_DIR}/output
		EXECUTABLE_OUTPUT_PATH 
		${CMAKE_CURRENT_SOURCE_DIR}/output
		LIBRARY_OUTPUT_DIRECTORY
		${CMAKE_CURRENT_SOURCE_DIR}/output
		RUNTIME_OUTPUT_DIRECTORY 
		${CMAKE_CURRENT_SOURCE_DIR}/output
	)

	add_custom_command(
		TARGET arcsoft_ui POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different 
		${DLLLIBRARY}
		${CMAKE_CURRENT_SOURCE_DIR}/workspace/
		COMMENT "${CMAKE_SYSTEM_NAME} platform : copying algorithm dll library to execute program directory..."
	)
	
	set_property(
		GLOBAL PROPERTY
		USE_FOLDERS on
	)

	set_property(
		TARGET
		${PROJECT_NAME}
		PROPERTY FOLDER "algorithm"
	)

	set_property(
		TARGET
		windows_package
		PROPERTY FOLDER "package"
	)

	# automate build windows library
	# file(
		# GLOB SLN_PROJECT
		# ${PROJECT_BINARY_DIR}/*.sln
	# )
	
	# set(
		# CMAKE_BUILD_WINDOWS_LIBRARY
		# "${CMAKE_VS_MSBUILD_COMMAND}" ${SLN_PROJECT}
		## "${CMAKE_VS_DEVENV_COMMAND}" ${SLN_PROJECT} /Build ${CMAKE_BUILD_TYPE}
	# )
elseif(CMAKE_SYSTEM_NAME MATCHES "Android")	

	set(
		ANDROID_PACKAGE
		package${CMAKE_STATIC_LIBRARY_SUFFIX}
	)

	# set(
		# target_object
		# $<$<TARGET_OBJECTS:toy>>
	# )

	# find_program(
		# AR NAMES
		# x86_64-w64-mingw32-gcc-ar
	# )

	# message(
		# STATUS
		# "CMAKE_SHARED_LIBRARY_SUFFIX  ==> ${CMAKE_SHARED_LIBRARY_SUFFIX}"
		# "CMAKE_STATIC_LIBRARY_SUFFIX  ==> ${CMAKE_STATIC_LIBRARY_SUFFIX}"
	# )

	execute_process(
		COMMAND
		${CMAKE_COMMAND} -E 
		make_directory ${CMAKE_CURRENT_SOURCE_DIR}/output/${ANDROID_ABI}
	)

	add_custom_command(
		VERBATIM COMMAND_EXPAND_LISTS
		OUTPUT ${ANDROID_PACKAGE}
		# COMMAND ${CMAKE_AR} ARGS -x $<TARGET_FILE:toy>
		COMMAND tar cvf ${ANDROID_ABI}.tar ${CMAKE_CURRENT_SOURCE_DIR}/output/${ANDROID_ABI}
		#COMMAND ${CMAKE_AR} ARGS crs ${ANDROID_PACKAGE} *${CMAKE_CXX_OUTPUT_EXTENSION}
		#COMMAND ${CMAKE_AR} ARGS crs ${ANDROID_PACKAGE} $<TARGET_OBJECTS:toy>
		#COMMAND ${CMAKE_AR} ARGS crs ${ANDROID_PACKAGE} $<TARGET_FILE:toy> $<TARGET_FILE:toy_core> $<TARGET_FILE:${PROJECT_NAME}>
		DEPENDS toy toy_core ${PROJECT_NAME}
		# workspace is the library destination
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output/${ANDROID_ABI}
		COMMENT "package android depend libraries into one package..."
	)
	
	# add_custom_command(
		# OUTPUT ${ANDROID_PACKAGE}
		# COMMAND tar cvf ${ANDROID_ABI}.tar ../${ANDROID_ABI}
		# COMMAND ${CMAKE_AR} x $<TARGET_FILE:toy>
		# COMMAND ${CMAKE_AR} x $<TARGET_FILE:toy_core>
		# COMMAND ${CMAKE_AR} x $<TARGET_FILE:${PROJECT_NAME}>
		# can't find the *.o
		# COMMAND ${CMAKE_AR} cru ${ANDROID_PACKAGE} *.o
		# COMMAND ${CMAKE_RANLIB} ${ANDROID_PACKAGE}
		# COMMAND ar rcT ${ANDROID_PACKAGE} $<TARGET_FILE:toy> $<TARGET_FILE:toy_core> $<TARGET_FILE:${PROJECT_NAME}>
		# DEPENDS toy toy_core ${PROJECT_NAME}
		# WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output
		# COMMENT "package android depend libraries into one package..."
	# )

	# add_custom_target(
		# android_package
		# DEPENDS ${ANDROID_PACKAGE}
	# )

	# add_custom_target(
		# combined ALL
		# COMMAND ${CMAKE_CXX_ARCHIVE_CREATE} libcombined.a $<TARGET_FILE:toy> $<TARGET_FILE:toy_core>
	# )

	# add_dependencies(
		# ${PROJECT_NAME}
		# android_package
	# )

	# set output directory
	# set_target_properties(
		# ${PROJECT_NAME} PROPERTIES
		# ARCHIVE_OUTPUT_DIRECTORY
		# ${CMAKE_CURRENT_SOURCE_DIR}/output/${ANDROID_ABI}
		# LIBRARY_OUTPUT_DIRECTORY
		# ${CMAKE_CURRENT_SOURCE_DIR}/output/${ANDROID_ABI}
	# )

	# copy the generated libraries to a destinate path
	# add_custom_command(
		# TARGET ${PROJECT_NAME}
		# POST_BUILD COMMAND
		# ${CMAKE_COMMAND} -E copy_if_different
		# $<TARGET_FILE:toy>
		# $<TARGET_FILE:toy_core>
		# $<TARGET_FILE:toy_fourcc>
		# $<TARGET_FILE:${PROJECT_NAME}>
		# ${CMAKE_CURRENT_SOURCE_DIR}/workspace
		# COMMENT "${CMAKE_SYSTEM_NAME} platform : copying generated algorithm library to execute program directory..."
	# )

	# add_custom_target(
		# package_static_library ALL 
		# VERBATIM COMMAND_EXPAND_LISTS 
		# COMMAND ${CMAKE_COMMAND} -E 
			  # make_directory "${PROJECT_BINARY_DIR}/$<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>/" 
	# )
	
	# package static library
	# add_custom_target(
		# package_static_library ALL
		# COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/package
		# COMMAND ${CMAKE_AR} x $<TARGET_FILE:toy>
		# COMMAND ${CMAKE_AR} x $<TARGET_FILE:toy_core>
		# COMMAND ${CMAKE_AR} x $<TARGET_FILE:toy_fourcc>
		# COMMAND ${CMAKE_AR} x $<TARGET_FILE:${PROJECT_NAME}>	
		# COMMAND ${CMAKE_AR} cru $<TARGET_FILE:${PROJECT_NAME}> *.o
		# COMMAND ${CMAKE_RANLIB} ${PROJECT_NAME}>.a
	# )
	
	# install(
		# TARGETS ${PROJECT_NAME}
		# EXPORT  ${PROJECT_NAME}
		# ARCHIVE DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/output/${ANDROID_ABI}
		# LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/output/${ANDROID_ABI}
		# RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/output/${ANDROID_ABI}
	# )
	
	# CMAKE_MAKE_PROGRAM ==> Enterprise/MSBuild/Current/Bin/MSBuild.exe
	# CMAKE_LINKER ==> Enterprise/VC/Tools/MSVC/14.26.28801/bin/Hostx64/x64/link.exe
	# CMAKE_CXX_CREATE_STATIC_LIBRARY ==> <CMAKE_LINKER> /lib /nologo <LINK_FLAGS> /out:<TARGET> <OBJECTS>
	# CMAKE_AR ==> android-ndk-r17b/toolchains/aarch64-linux-android-4.9/prebuilt/windows-x86_64/bin/aarch64-linux-android-ar.exe
	# CMAKE_RANLIB ==> android-ndk-r17b/toolchains/aarch64-linux-android-4.9/prebuilt/windows-x86_64/bin/aarch64-linux-android-ranlib.exe
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")

	set(
		IOS_PACKAGE
		# package${CMAKE_STATIC_LIBRARY_SUFFIX}
		${CMAKE_CURRENT_SOURCE_DIR}/output/package${CMAKE_STATIC_LIBRARY_SUFFIX}
	)

	add_custom_target(
		ios_package
		COMMAND libtool -static -o ${IOS_PACKAGE} $<TARGET_FILE:toy> $<TARGET_FILE:toy_core> $<TARGET_FILE:${PROJECT_NAME}>
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output
		DEPENDS toy toy_core ${PROJECT_NAME}
		COMMENT "package ios depend libraries into one package..."
	)
endif()
