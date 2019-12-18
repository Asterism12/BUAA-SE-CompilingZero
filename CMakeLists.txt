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
	tokenizer/token.h
	tokenizer/tokenizer.h
	tokenizer/tokenizer.cpp
	tokenizer/utils.hpp
	analyser/analyser.h
	analyser/analyser.cpp
)

set(main_src
	main.cpp
)

add_library(${PROJECT_LIB} ${lib_src})

add_executable(${PROJECT_EXE} ${main_src})

set_target_properties(${PROJECT_EXE} PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED ON
)

set_target_properties(${PROJECT_LIB} PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED ON
)

target_include_directories(${PROJECT_EXE} PRIVATE .)
target_include_directories(${PROJECT_LIB} PRIVATE .)

target_link_libraries(${PROJECT_EXE} ${PROJECT_LIB} argparse)