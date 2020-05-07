
project(buddha)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(module)
add_subdirectory(pattern)
add_subdirectory(algorithm)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(TARGET module_reflection PROPERTY FOLDER "module")
set_property(TARGET module_path_walker PROPERTY FOLDER "module")
set_property(TARGET module_time_clock PROPERTY FOLDER "module")
set_property(TARGET module_information PROPERTY FOLDER "module")
set_property(TARGET module_image_tools PROPERTY FOLDER "module")
set_property(TARGET module_memory_pool PROPERTY FOLDER "module")
set_property(TARGET module_thread_pool PROPERTY FOLDER "module")
set_property(TARGET module_meta_program PROPERTY FOLDER "module")
set_property(TARGET module_stl_algorithm PROPERTY FOLDER "module")
set_property(TARGET pattern_singleton_pattern PROPERTY FOLDER "pattern")

set_property(TARGET algorithm_base PROPERTY FOLDER "algorithm")
set_property(TARGET algorithm_derived PROPERTY FOLDER "algorithm")

set(CMAKE_CXX_FLAGS
	"${CMAKE_CXX_FLAGS} -g -std=c++latest")

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
					  algorithm_base
					  algorithm_derived
                      module_reflection
					  module_time_clock
					  module_information
					  module_image_tools
					  module_memory_pool
					  module_path_walker
					  module_thread_pool
					  module_meta_program
					  module_stl_algorithm
					  pattern_singleton_pattern)

target_include_directories(${PROJECT_NAME}
						   PUBLIC
						   ${CMAKE_CURRENT_SOURCE_DIR})

set_directory_properties(PROPERTIES VS_STARTUP_PROJECT
						 ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME}
					  PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
					  "${CMAKE_CURRENT_SOURCE_DIR}/workspace/")
