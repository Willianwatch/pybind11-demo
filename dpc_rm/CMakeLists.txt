cmake_minimum_required(VERSION 3.10)
project(DPC_FW)

# 添加头文件目录
include_directories(include common)

# 添加源文件（递归查找所有.c文件）
file(GLOB_RECURSE COMMON_SOURCES "common/*.c")
file(GLOB_RECURSE SRC_SOURCES "src/*.c")

# 创建共享库（生成.dpc_fw.so/.dll）
add_library(dpc_fw SHARED ${COMMON_SOURCES} ${SRC_SOURCES})

# 可选：安装规则（将库安装到系统路径）
install(TARGETS dpc_fw DESTINATION lib)