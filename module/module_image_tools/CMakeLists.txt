
project(module_image_tools)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 20)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

aux_source_directory(. SRCFILE)

file(GLOB_RECURSE HEADFILE ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(${PROJECT_NAME} ${SRCFILE} ${HEADFILE})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
