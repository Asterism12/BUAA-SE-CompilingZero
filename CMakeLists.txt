# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("cc0")

add_subdirectory(3rd_party/argparse)

set(PROJECT_EXE ${PROJECT_NAME})
set(PROJECT_LIB "${PROJECT_NAME}_lib")

set(lib_src
	error/error.h
	error/error.cpp
)

set(main_src
	main.cpp
)

add_library(${PROJECT_LIB} ${lib_src})

add_executable(${PROJECT_EXE} ${main_src})

target_link_libraries(${PROJECT_EXE} ${PROJECT_LIB} argparse)