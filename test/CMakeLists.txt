# 测试源文件
set(TEST_SOURCES
    Type_test.cpp
    Value_test.cpp
)

# 创建测试可执行文件
add_executable(run_tests ${TEST_SOURCES})

# 链接GTest库和我们的项目库
target_link_libraries(run_tests PRIVATE gtest_main llvm_ir)

# 添加测试
include(GoogleTest)
gtest_discover_tests(run_tests)