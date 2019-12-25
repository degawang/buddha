
project(buddha)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(module)
add_subdirectory(pattern)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(TARGET module_ai_pointer PROPERTY FOLDER "module")
set_property(TARGET module_time_clock PROPERTY FOLDER "module")
set_property(TARGET module_new_feature PROPERTY FOLDER "module")
set_property(TARGET module_class_utils PROPERTY FOLDER "module")
set_property(TARGET module_memory_pool PROPERTY FOLDER "module")
set_property(TARGET module_refer_count PROPERTY FOLDER "module")
set_property(TARGET module_thread_pool PROPERTY FOLDER "module")
set_property(TARGET pattern_factory_pattern PROPERTY FOLDER "pattern")
set_property(TARGET pattern_iterator_pattern PROPERTY FOLDER "pattern")
set_property(TARGET pattern_singleton_pattern PROPERTY FOLDER "pattern")

set(CMAKE_CXX_FLAGS
	"${CMAKE_CXX_FLAGS} -g -std=c++17")

file(GLOB_RECURSE
	 MARKDOWN_NOTE
	 note/*.md)

source_group("note"
			 FILES
			 ${MARKDOWN_NOTE})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}
					 SRCFILE)

add_executable(${PROJECT_NAME}
			   ${SRCFILE}
			   ${MARKDOWN_NOTE})

target_compile_options(${PROJECT_NAME}
					   PUBLIC
					   -g)

target_link_libraries(${PROJECT_NAME}
					  module_ai_pointer
					  module_time_clock
					  module_class_utils
					  module_memory_pool
					  module_refer_count
					  module_thread_pool
					  pattern_factory_pattern
					  pattern_singleton_pattern)

target_include_directories(${PROJECT_NAME}
						   PUBLIC
						   ${CMAKE_CURRENT_SOURCE_DIR})

set_directory_properties(PROPERTIES VS_STARTUP_PROJECT
						 ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME}
					  PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
					  "${CMAKE_CURRENT_SOURCE_DIR}/workspace/")
