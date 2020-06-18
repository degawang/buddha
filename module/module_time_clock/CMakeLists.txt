
project(module_time_clock)

cmake_minimum_required(
	VERSION
	3.0
)

set(
	CMAKE_CXX_STANDARD
	20
)

# include_directories(
	# ${CMAKE_CURRENT_SOURCE_DIR}
# )

file(
	GLOB_RECURSE SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/*
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
	${PROJECT_NAME}
	pattern_singleton_pattern
)
