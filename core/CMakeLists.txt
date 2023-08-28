cmake_minimum_required(VERSION 3.10)

project(ProcessReporterWincpp.exe)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加可执行文件
add_executable(main.cpp)

# 添加子目录以编译静态库
add_subdirectory(D:\msys64\mingw64\lib)

# 链接静态库
target_link_libraries(curl jsoncpp_static)
