cmake_minimum_required(VERSION 3.15)
project(Krpc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#设置头文件目录，供所有子项目使用
#include_directories(${CMAKE_SOURCE_DIR}/src/include) #其中CMAKE_SOURCE_DIR表示当前目录

#查找porotbuf包
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})#Protobuf_INCLUDE_DIRS表示protobuf头文件目录

#设置全局链接库
set(LIBS
    protobuf
    pthread
    zookeeper_mt
    muduo_net
    muduo_base
    glog
)
#添加子目录
add_subdirectory(src)
add_subdirectory(example)
