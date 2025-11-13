cmake_minimum_required(VERSION 3.15) # 建议使用更新的版本以获得更好的功能
project(MyLLVM-IR LANGUAGES CXX)

# --- 库的定义 ---

set(SOURCES
    src/Type.cpp
    src/Value.cpp
    src/Context.cpp
    src/Module.cpp
    src/IRBuilder.cpp
)

# 1. 创建llvm_ir静态库
add_library(llvm_ir STATIC ${SOURCES})

# 对于任何一个llvm_ir库的消费者，将 'include' 目录添加到其头文件搜索路径中
target_include_directories(llvm_ir PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


# 2.提供选项，让父项目决定是否构建测试，默认为 OFF。
option(MYLLVM_IR_BUILD_TESTS "Build the tests for MyLLVM-IR" OFF)

if(MYLLVM_IR_BUILD_TESTS)
    # 只有当选项开启时，才启用测试并下载GTest
    enable_testing()
    
    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    FetchContent_MakeAvailable(googletest)
    # 添加测试子目录。测试目录中的 target 应该链接到 GTest 和 llvm_ir
    add_subdirectory(test)
endif()