cmake_minimum_required(VERSION 3.10)
project(MyLLVM-IR)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加头文件目录
include_directories(include)

# 下载并配置GTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# 添加源文件
set(SOURCES
    src/Type.cpp
    src/Value.cpp
    src/Context.cpp
    src/Module.cpp
)

# 创建静态库
add_library(llvm_ir STATIC ${SOURCES})

# 添加测试子目录
add_subdirectory(test)

# 启用测试
enable_testing()