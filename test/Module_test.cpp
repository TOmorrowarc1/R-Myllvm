
#include "gtest/gtest.h"
#include "Module.hpp"
#include "Context.hpp"
#include "Type.hpp"
#include "Value.hpp"

namespace llvm {

// 测试Module的基本功能
TEST(ModuleTest, BasicFunctionalityTest) {
    LLVMContext context;
    Module module("test_module", &context);
    
    // 测试模块名称和上下文
    EXPECT_EQ(module.getContext(), &context);
    
    // 测试打印功能
    std::string print_result = module.print();
    EXPECT_TRUE(print_result.find("test_module") != std::string::npos);
}

// 测试函数管理
TEST(ModuleTest, FunctionManagementTest) {
    LLVMContext context;
    Module module("test_module", &context);
    
    // 获取类型
    Type* i32_type = context.getInt32Ty();
    Type* i8_type = context.getInt8Ty();
    std::vector<Type*> param_types = {i8_type, i8_type};
    FunctionType* func_type = context.getFunctionType(i32_type, param_types);
    
    // 测试获取不存在的函数
    Function* non_existent_func = module.getFunction("non_existent");
    EXPECT_EQ(non_existent_func, nullptr);
    
    // 创建函数
    Function* func1 = module.getOrCreateFunction("test_func", func_type);
    ASSERT_NE(func1, nullptr);
    EXPECT_EQ(func1->getName(), "test_func");
    EXPECT_EQ(func1->getType(), func_type);
    EXPECT_EQ(func1->getParent(), &module);
    
    // 获取已存在的函数
    Function* func2 = module.getFunction("test_func");
    ASSERT_NE(func2, nullptr);
    EXPECT_EQ(func1, func2);
    
    // 使用getOrCreateFunction获取已存在的函数
    Function* func3 = module.getOrCreateFunction("test_func", func_type);
    EXPECT_EQ(func1, func3);
    
    // 创建另一个函数
    FunctionType* another_func_type = context.getFunctionType(i8_type, {});
    Function* func4 = module.getOrCreateFunction("another_func", another_func_type);
    ASSERT_NE(func4, nullptr);
    EXPECT_EQ(func4->getName(), "another_func");
    EXPECT_EQ(func4->getType(), another_func_type);
    EXPECT_NE(func1, func4);
}

// 测试全局变量管理
TEST(ModuleTest, GlobalVariableManagementTest) {
    LLVMContext context;
    Module module("test_module", &context);
    
    // 获取类型
    Type* i32_type = context.getInt32Ty();
    ConstantInt* init_value = context.getIntConstant(static_cast<IntegerType*>(i32_type), 42);
    
    // 测试获取不存在的全局变量
    GlobalVariable* non_existent_var = module.getGlobalVariable("non_existent");
    EXPECT_EQ(non_existent_var, nullptr);
    
    // 创建全局变量
    GlobalVariable* var1 = module.getOrCreateGlobalVariable("test_var", i32_type, false, init_value);
    ASSERT_NE(var1, nullptr);
    EXPECT_EQ(var1->getName(), "test_var");
    EXPECT_EQ(var1->getType(), i32_type);
    EXPECT_EQ(var1->getInitialValue(), init_value);
    
    // 获取已存在的全局变量
    GlobalVariable* var2 = module.getGlobalVariable("test_var");
    ASSERT_NE(var2, nullptr);
    EXPECT_EQ(var1, var2);
    
    // 使用getOrCreateGlobalVariable获取已存在的全局变量
    GlobalVariable* var3 = module.getOrCreateGlobalVariable("test_var", i32_type, false, init_value);
    EXPECT_EQ(var1, var3);
    
    // 创建另一个全局变量
    ConstantInt* another_init_value = context.getIntConstant(static_cast<IntegerType*>(i32_type), 100);
    GlobalVariable* var4 = module.getOrCreateGlobalVariable("another_var", i32_type, true, another_init_value);
    ASSERT_NE(var4, nullptr);
    EXPECT_EQ(var4->getName(), "another_var");
    EXPECT_EQ(var4->getType(), i32_type);
    EXPECT_NE(var1, var4);
}

// 测试Module的打印功能
TEST(ModuleTest, PrintTest) {
    LLVMContext context;
    Module module("test_module", &context);
    
    // 获取类型
    Type* i32_type = context.getInt32Ty();
    Type* i8_type = context.getInt8Ty();
    
    // 创建函数
    std::vector<Type*> param_types = {i8_type, i8_type};
    FunctionType* func_type = context.getFunctionType(i32_type, param_types);
    Function* func = module.getOrCreateFunction("test_func", func_type);
    
    // 创建全局变量
    ConstantInt* init_value = context.getIntConstant(static_cast<IntegerType*>(i32_type), 42);
    GlobalVariable* var = module.getOrCreateGlobalVariable("test_var", i32_type, false, init_value);
    
    // 测试打印功能
    std::string print_result = module.print();
    EXPECT_TRUE(print_result.find("test_module") != std::string::npos);
    EXPECT_TRUE(print_result.find("test_func") != std::string::npos);
    EXPECT_TRUE(print_result.find("test_var") != std::string::npos);
}

// 测试复杂场景
TEST(ModuleTest, ComplexScenarioTest) {
    LLVMContext context;
    Module module("complex_module", &context);
    
    // 获取类型
    Type* i32_type = context.getInt32Ty();
    Type* i8_type = context.getInt8Ty();
    Type* i1_type = context.getInt1Ty();
    
    // 创建结构体类型
    StructType* struct_type = context.getStructType("MyStruct");
    std::vector<Type*> elements = {i32_type, i8_type, i1_type};
    struct_type->setBody(elements);
    
    // 创建数组类型
    ArrayType* array_type = context.getArrayType(i32_type, 10);
    
    // 创建多个函数
    std::vector<Type*> param_types1 = {i32_type, i8_type};
    FunctionType* func_type1 = context.getFunctionType(i32_type, param_types1);
    Function* func1 = module.getOrCreateFunction("func1", func_type1);
    
    std::vector<Type*> param_types2 = {struct_type, array_type};
    FunctionType* func_type2 = context.getFunctionType(i8_type, param_types2);
    Function* func2 = module.getOrCreateFunction("func2", func_type2);
    
    // 创建多个全局变量
    ConstantInt* init_value1 = context.getIntConstant(static_cast<IntegerType*>(i32_type), 100);
    GlobalVariable* var1 = module.getOrCreateGlobalVariable("var1", i32_type, false, init_value1);
    
    ConstantInt* init_value2 = context.getIntConstant(static_cast<IntegerType*>(i8_type), 42);
    GlobalVariable* var2 = module.getOrCreateGlobalVariable("var2", i8_type, true, init_value2);
    
    // 验证所有函数和全局变量都正确创建
    EXPECT_EQ(module.getFunction("func1"), func1);
    EXPECT_EQ(module.getFunction("func2"), func2);
    EXPECT_EQ(module.getGlobalVariable("var1"), var1);
    EXPECT_EQ(module.getGlobalVariable("var2"), var2);
    
    // 测试打印功能
    std::string print_result = module.print();
    EXPECT_TRUE(print_result.find("complex_module") != std::string::npos);
    EXPECT_TRUE(print_result.find("func1") != std::string::npos);
    EXPECT_TRUE(print_result.find("func2") != std::string::npos);
    EXPECT_TRUE(print_result.find("var1") != std::string::npos);
    EXPECT_TRUE(print_result.find("var2") != std::string::npos);
}

} // namespace llvm